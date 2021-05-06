#ifndef HOMETHEATRE_H
#define HOMETHEATRE_H

#include <QtWidgets/QMainWindow>
#include <QSettings>
#include <QThread>

#include "ui_HomeTheatre.h"
#include <FlowLayout.h>
#include <PreferencesWnd.h>

#include <Movie.h>
#include <StorageManager.h>

class MovieEngine;
class APIClient;


class HomeTheatre : public QMainWindow, public Ui::HomeTheatreClass
{
	Q_OBJECT

public:
	HomeTheatre(QWidget *parent = 0);
	~HomeTheatre();

	void show();

	void ReloadLibrary(QStringList folders, bool bHardReset = false );

	virtual void OnMovieSearchResult( Movie pMovie, QList<Movie> movieList);

public slots:
	void OnPreferencesClicked();
	void OnScanningFoldersCompleted( QMap<QString, Movie> mapMovies, int iAddedCount );
	void OnLoadMoviesFromDBCompleted(int count);

	void OnMovieUpdated(Movie* pMovie);
	void OnMoviePosterUpdated(Movie pMovie);

private slots:
	void OnQueueProcessingFinished();

signals:
	void StartScanningFolders(QStringList foldersList, bool bHardReset);
	void StartOnlineDataRetrieval();

private:
	FlowLayout* flowLayout;
	PreferencesWnd* preferencesWnd;

	MovieEngine* p_MovieEngine;

	QThread folderScannerThread;

};

#endif // HOMETHEATRE_H
