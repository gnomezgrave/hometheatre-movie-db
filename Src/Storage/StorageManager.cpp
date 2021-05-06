#include "StorageManager.h"

#include <QDir>

#include <Movie.h>
#include <FileWriter.h>
#include <MovieEngine.h>


StorageManager::StorageManager(MovieEngine* pMovieEngine) : p_MovieEngine(pMovieEngine)
{
	i_ImageWidth = 1000;
	p_Settings = Settings::GetSettings();
	p_FileWriter = new FileWriter();

	p_FileWriter->moveToThread(&fileWriterThread);
	fileWriterThread.start();

	QDir homeDir(p_Settings->GetSettingsFolder());
	QDir imgDir(p_Settings->GetImagesFolder());

	if ( !imgDir.exists() )
	{
		homeDir.mkdir("images");
	}

	s_ImageDownloadURL = QString("http://image.tmdb.org/t/p/w%1").arg(QString::number(i_ImageWidth));
	connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
	connect(p_FileWriter, SIGNAL(FileWriteCompleted(QString)), this, SLOT(OnFileWriteCompleted(QString)), Qt::QueuedConnection);
}


StorageManager::~StorageManager(void)
{
}

void StorageManager::DownloadImages( Movie pMovie )
{
	if ( pMovie.s_PosterPath.isEmpty() )
	{
		qDebug() << "Poster is empty : " << pMovie.s_MovieTitle;
	}
	else
	{
		SendDownloadRequest(pMovie.s_PosterPath, pMovie, 0);
	}

	if ( pMovie.s_BackdropPath.isEmpty() )
	{
		qDebug() << "Backdrop is empty : " << pMovie.s_MovieTitle;
	}
	else
	{
		SendDownloadRequest(pMovie.s_BackdropPath, pMovie, 1);
	}
}

void StorageManager::downloadFinished( QNetworkReply* reply )
{
	auto itr = map_requests.find(reply);

	if ( itr == map_requests.end() )
	{
		qDebug() << "Reply was not added before";
		return;
	}
	
	if ( reply->error() != QNetworkReply::NoError )
	{
		qDebug() << "Error " << reply->error();
		
		RemoveTempImageData(reply);

		return;
	}

	QString sPath = itr.value();
	QString sURL = (itr.key())->url().toDisplayString();

	sPath = p_Settings->GetImagesFolder() + sPath;

	QByteArray data = reply->readAll();
	
	p_FileWriter->WriteToFile(sPath, data);
	
	RemoveTempImageData(reply);

	map_requests.remove(reply);
	reply->deleteLater();

	qDebug() << sURL << " >> Written to : " << sPath;

}

void StorageManager::SendDownloadRequest( QString sResource, Movie movie, int type, bool bOnlyUpdate )
{
	
	QString sPath = p_Settings->GetImagesFolder() + sResource;

	QFile image(sPath);

	if ( image.exists() )
	{
		//qDebug() << "File already exists : " << sPath;
		
		if ( type == 0 )
			p_MovieEngine->UpdateMoviePoster(movie);

		return;
	}
	
	QString sURL = s_ImageDownloadURL + sResource;

	QNetworkReply* reply = networkManager.get(QNetworkRequest(sURL));
	map_requests.insert(reply, sResource);
	map_requestsImgs.insert(reply, sPath);
	map_images.insert(sPath, movie);
	map_ImageTypes.insert(sPath, type);
}

void StorageManager::OnFileWriteCompleted( QString path )
{
	if ( map_images.contains(path) )
	{
		Movie m = map_images.value(path);
		map_images.remove(path);
		
		if ( map_ImageTypes.value(path) == 0 ) //poster
		{
			p_MovieEngine->UpdateMoviePoster(m);
			map_ImageTypes.remove(path);
		}
	}
}

void StorageManager::RemoveTempImageData( QNetworkReply* reply )
{
	QString path = map_requestsImgs.value(reply, "");

	if ( !path.isEmpty() )
	{
		map_ImageTypes.remove(path);
		map_images.remove(path);
	}
	map_requestsImgs.remove(reply);
	map_requests.remove(reply);
}
