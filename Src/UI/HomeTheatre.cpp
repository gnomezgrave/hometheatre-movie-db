#include <HomeTheatre.h>
#include <MovieEngine.h>
#include <MovieCard.h>
#include <Movie.h>
#include <TheMovieDBAPI.h>

#include <QDebug>
#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

HomeTheatre::HomeTheatre(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);
	progressBar->hide();
	//setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	connect(btnPreferences, SIGNAL(released()), this, SLOT(OnPreferencesClicked()));

	flowLayout = new FlowLayout();
	p_MovieEngine = new MovieEngine();

	frm_Items->setLayout(flowLayout);

	preferencesWnd = NULL;

	connect(this, SIGNAL(StartScanningFolders(QStringList, bool)), p_MovieEngine, SLOT(OnScanFolders(QStringList, bool)),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	connect(p_MovieEngine, SIGNAL(ScanningFoldersCompleted( QMap<QString, Movie>, int )), this, SLOT(OnScanningFoldersCompleted( QMap<QString, Movie>, int )),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	connect(p_MovieEngine, SIGNAL(LoadMoviesFromDBCompleted(int)), this, SLOT(OnLoadMoviesFromDBCompleted(int)),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	connect(p_MovieEngine, SIGNAL(MovieUpdated( Movie*)), this, SLOT(OnMovieUpdated(Movie*)),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	connect(p_MovieEngine, SIGNAL(MoviePosterUpdated( Movie )), this, SLOT(OnMoviePosterUpdated(Movie)),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));

	connect(this, SIGNAL(StartOnlineDataRetrieval()), p_MovieEngine, SLOT(OnStartOnlineDataRetrieval()),
		static_cast<Qt::ConnectionType>(Qt::QueuedConnection | Qt::UniqueConnection));


	emit p_MovieEngine->LoadMoviesFromDB();

}

HomeTheatre::~HomeTheatre()
{
	if ( preferencesWnd != NULL )
		preferencesWnd->close();
	
	delete preferencesWnd;
	delete p_MovieEngine;
}

void HomeTheatre::show()
{
	QMainWindow::show();
}

void HomeTheatre::OnPreferencesClicked()
{
	if ( preferencesWnd == NULL )
	{
		preferencesWnd = new PreferencesWnd(this);
		preferencesWnd->SetParentWnd(this);
	}

	preferencesWnd->setVisible(true);
}

void HomeTheatre::ReloadLibrary( QStringList folders, bool bHardReset /*= false*/  )
{
	emit StartScanningFolders(folders, bHardReset);
	progressBar->show();
}

void HomeTheatre::OnMovieSearchResult( Movie pMovie, QList<Movie> movieList )
{
	// TODO : Select the most matching Movie
	
	if ( movieList.size() == 0 )
	{
		qDebug() << "No data for " << pMovie.s_MovieTitle;
		return;
	}
	
}

void HomeTheatre::OnQueueProcessingFinished()
{
	qDebug() << "Queue is processed.";
}

void HomeTheatre::OnScanningFoldersCompleted( QMap<QString, Movie> mapMovies, int iAddedCount )
{
	/*foreach ( QString key , mapMovies.keys())
	{
		Movie* m = mapMovies.value(key);
		MovieCard* card = new MovieCard(this);
		card->SetMovie(m);
		m->AddListener(card);
		flowLayout->addWidget(card);
	}
	progressBar->hide();
	frmMoviesCount->show();
	lblMoviesCount->setText(QString::number(iAddedCount));
	QMessageBox::information(this, "Reload Completed!", QString::number(iAddedCount) + " movie" + (iAddedCount !=1 ? "s": "") + " added to the library.");

	StartOnlineDataRetrieval();*/
}

void HomeTheatre::OnLoadMoviesFromDBCompleted( int count )
{
	lbl_Loading->setText(QString("Loaded %1 movies loaded from the DB.").arg(QString::number(count)));
	emit StartScanningFolders( Settings::GetSettings()->GetFoldersList(), false );
}

void HomeTheatre::OnMovieUpdated( Movie* pMovie )
{
	flowLayout->AddOrUpdateMovieCard(pMovie);
}

void HomeTheatre::OnMoviePosterUpdated( Movie pMovie )
{
	flowLayout->UpdateMoviePoster(pMovie);
}

