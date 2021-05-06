#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QThread>

#include <Settings.h>
#include <Movie.h>

class FileWriter;
class MovieEngine;

class StorageManager : public QObject
{
	Q_OBJECT
public:
	StorageManager(MovieEngine* pMovieEngine);
	~StorageManager(void);

	void DownloadImages(Movie pMovie);


public slots:
	void downloadFinished(QNetworkReply* reply);
	void OnFileWriteCompleted(QString path);

private:
	MovieEngine*			p_MovieEngine;
	QNetworkAccessManager	networkManager;
	FileWriter*				p_FileWriter;
	QString					s_ImageDownloadURL;
	int						i_ImageWidth;

	Settings*				p_Settings;

	QThread					fileWriterThread;

	QMap<QNetworkReply*, QString>	map_requests;	// reply -> file name
	QMap<QString, Movie>			map_images;		// file -> movie
	QMap<QString, int>				map_ImageTypes;	// file -> 0=poster, 1=backdrop
	QMap<QNetworkReply*, QString>	map_requestsImgs; // dummy map to track the images


	void SendDownloadRequest( QString sBackdropUrl, Movie pMovie, int type, bool bOnlyUpdate = true );
	void RemoveTempImageData( QNetworkReply* reply );
};

