#include "MovieEngine.h"

#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

#include <FolderScanner.h>
#include <StorageManager.h>
#include <TheMovieDBAPI.h>
#include <DBManager.h>

MovieEngine::MovieEngine(void)
{
	p_FolderScanner = new FolderScanner(this);
	p_FolderScanner->moveToThread(&folderScannerThread);
	folderScannerThread.start();

	p_APIClient = new TheMovieDBAPI();
	//p_APIClient->moveToThread(&apiRequestThread);
	p_APIClient->Subscribe(this);

	p_StorageManager = new StorageManager(this);

	p_DBManager = new DBManager();
	p_DBManager->moveToThread(&dbThread);
	dbThread.start();

	connect( this, SIGNAL(StartScanningFolders(QStringList, bool)), p_FolderScanner, SLOT(OnScanFolders(QStringList, bool)), Qt::QueuedConnection);
	connect( p_FolderScanner, SIGNAL(FolderScanningCompleted(QMap<QString, Movie>, int)), this, SLOT(OnScanningFoldersCompleted( QMap<QString, Movie>, int)),
		Qt::QueuedConnection);

	connect( this, SIGNAL(LoadMoviesFromDB()), p_DBManager, SLOT(LoadMoviesFromDB()), Qt::QueuedConnection);
	connect( p_DBManager, SIGNAL(LoadMoviesFromDBCompleted(QList<Movie>)), this, SLOT(OnLoadMoviesFromDBCompleted(QList<Movie>)),
		Qt::QueuedConnection);

	connect(this, SIGNAL(MovieUpdated( Movie*)), p_DBManager, SLOT(UpdateMovie(Movie*)),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));
	
}


MovieEngine::~MovieEngine(void)
{
	Reset();
}

const QMap<QString, Movie>* MovieEngine::GetMoviesMap()
{
	return p_FolderScanner->GetMoviesMap();
}

void MovieEngine::OnScanFolders( QStringList sFoldersList, bool bReset )
{
	emit StartScanningFolders( sFoldersList, bReset);
}

void MovieEngine::Reset()
{
	
}

Movie* MovieEngine::FindMovie( QString sMovieName )
{
	return NULL;
}

void MovieEngine::OnScanningFoldersCompleted( QMap<QString, Movie> mapMovies, int iAddedCount )
{
	MergeMovies(mapMovies);
	emit ScanningFoldersCompleted(mapMovies, iAddedCount);

	QThread::sleep(2);

	OnStartOnlineDataRetrieval();
}

void MovieEngine::OnStartOnlineDataRetrieval()
{
	foreach ( QString key , map_Movies.keys())
	{
		Movie* m = map_Movies.value(key);

		QMap<QString, QString> values;
		values["query"] = m->s_MovieTitle;
		values["year"] = QString::number(m->i_Year);
		p_APIClient->AddMovieToSearchQueue( *m, values );
	}

	p_APIClient->ProcessRequestQueue();
}

void MovieEngine::OnMovieSearchResult( Movie pMovie, QList<Movie> movieList )
{
	//qDebug() << pMovie.s_MovieTitle << " " << movieList.count();

	// TODO : Merge the movies

	if( movieList.count() != 0 )
	{
		foreach(Movie movie, movieList)
		{
			AddToOnlineDuplicatesMap(movie);
		}

		p_StorageManager->DownloadImages(movieList.at(0));
	}
}

void MovieEngine::OnLoadMoviesFromDBCompleted( QList<Movie> lstMovies )
{
	int iCount = 0;
	foreach(Movie m, lstMovies )
	{
		if ( AddMovie(m) )
			iCount++;
	}
	
	emit LoadMoviesFromDBCompleted(iCount);
}

bool MovieEngine::AddMovie( Movie movie )
{
	AddToDuplicatesMap(movie);

	if ( !map_Movies.contains( movie.GetUniqueKey() ) )
	{
		Movie* pMovie = new Movie(movie);
		map_Movies.insert(movie.GetUniqueKey(), pMovie);
		emit MovieUpdated(pMovie);
		return true;
	}

	return false;
}

void MovieEngine::AddToDuplicatesMap( Movie movie )
{
	QList<Movie>* pList = map_DuplicatesMap.value(movie.GetUniqueKey(), NULL);

	if ( pList == NULL )
	{
		pList = new QList<Movie>();
		map_DuplicatesMap.insert(movie.GetUniqueKey(), pList);
	}

	pList->append(movie);
}

void MovieEngine::MergeMovies( const QMap<QString, Movie> &mapScannedMovies )
{
	foreach(QString key, mapScannedMovies.keys())
	{
		Movie m = mapScannedMovies.value(key);

		if ( !map_Movies.contains( m.GetUniqueKey() ) )
		{
			AddMovie(m);
		}
	}
}

void MovieEngine::AddToOnlineDuplicatesMap( Movie movie )
{
	QList<Movie>* pList = map_OnlineDuplicatesMap.value(movie.GetUniqueKey(), NULL);

	if ( pList == NULL )
	{
		pList = new QList<Movie>();
		map_OnlineDuplicatesMap.insert(movie.GetUniqueKey(), pList);
	}

	pList->append(movie);
}

void MovieEngine::UpdateMoviePoster( Movie movie )
{
	Movie* pMovie = map_Movies.value(movie.GetUniqueKey(), NULL);

	if ( pMovie != NULL )
	{
		emit MoviePosterUpdated(movie);
	}
}

