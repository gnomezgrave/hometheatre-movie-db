#pragma once

#include <QObject>
#include <QThread>
#include <QStringList>
#include <QMap>
#include <QSet>
#include <QFileInfo>

#include <Movie.h>

class MovieEngine;

class FolderScanner : public QObject
{
	Q_OBJECT
public:
	FolderScanner(MovieEngine* pMovieEngine);
	~FolderScanner(void);
	
	const QMap<QString, Movie>* GetMoviesMap() const { return &map_Movies; }

public slots:
	void OnScanFolders(QStringList sFoldersList, bool bReset);

signals:
	void FolderScanningCompleted(QMap<QString, Movie> mapMovies, int iAddedCount);

private:

	MovieEngine* p_MovieEngine;

	QStringList lst_VideoExtensions;
	QMap<QString, Movie> map_Movies;
	QSet<QString> set_ScannedPaths;
	QStringList lst_FoldersList;

	int ScanFolder( QString sFolder);
	int digFolder( QString sFolder);
	Movie decodeMovie(QFileInfo fileInfo);
};

