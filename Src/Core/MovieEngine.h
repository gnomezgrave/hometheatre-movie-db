#pragma once

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QStringList>
#include <QSet>
#include <QMap>
#include <QList>
#include <QThread>

#include <Movie.h>
#include <APIClient.h>

class FolderScanner;
class StorageManager;
class DBManager;

class MovieEngine : public QObject, public APIClientSubscription
{
	Q_OBJECT

public:
	MovieEngine(void);
	~MovieEngine(void);

	void Reset();

	Movie*	FindMovie(QString sMovieName);
	bool	AddMovie(Movie movie);
	bool	UpdateMovie(const Movie* pMovie);
	void	UpdateMoviePoster(Movie movie);
	const	QMap<QString, Movie>* GetMoviesMap();

	virtual void OnMovieSearchResult(Movie movie, QList<Movie> movieList) override;


public slots:
	void OnStartOnlineDataRetrieval();
	void OnScanFolders( QStringList sFoldersList, bool bHardReset );
	void OnScanningFoldersCompleted( QMap<QString, Movie> mapMovies, int iAddedCount );
	void OnLoadMoviesFromDBCompleted(QList<Movie> lstMovies);

signals:
	void ScanningFoldersCompleted( QMap<QString, Movie> mapMovies, int iAddedCount );
	void LoadMoviesFromDBCompleted(int count);
	void MovieUpdated( Movie* pMovie );
	void MoviePosterUpdated( Movie pMovie );
	void StartScanningFolders(QStringList sFoldersList, bool bReset);
	void LoadMoviesFromDB();

private:
	QThread folderScannerThread;
	QThread apiRequestThread;
	QThread	dbThread;

	FolderScanner*	p_FolderScanner;
	StorageManager* p_StorageManager;
	DBManager*		p_DBManager;

	APIClient*		p_APIClient;

	QMap<QString, Movie*> map_Movies;	// unique key => Movie
	QMap<QString, Movie*> map_MovieIDs;
	QMap<QString, QList<Movie>* > map_DuplicatesMap;
	QMap<QString, QList<Movie>* > map_OnlineDuplicatesMap;

	Movie*	MergeAndGetTheFinalPicture( Movie pMovie, QList<Movie> movieList );
	void	AddToDuplicatesMap( Movie movie );
	void	AddToOnlineDuplicatesMap( Movie movie );

	void	MergeMovies(const QMap<QString, Movie> &mapMovies);

};

