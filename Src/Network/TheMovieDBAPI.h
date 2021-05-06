#pragma once

#include <APIClient.h>
#include <QNetworkAccessManager>

class TheMovieDBAPI : public APIClient
{
	Q_OBJECT
public:
	TheMovieDBAPI(void);
	virtual ~TheMovieDBAPI(void);

	virtual void AddMovieToSearchQueue( Movie pMovie, QMap<QString, QString> values);
	virtual void Subscribe( APIClientSubscription* subscriber ) { set_subscribers.insert(subscriber); }
	
public slots:
	virtual void ProcessRequestQueue();
	
private slots:	
	void replyFinished( QNetworkReply* reply );

signals:
	void OnMovieSearchResult(Movie pMovie, QList<Movie> movieList);

private:
	QString s_APIKey;
	QString s_URL;
	QString s_DefaultLang;

	QString s_IncludeAdult;

	QNetworkAccessManager networkManager;

	QSet<APIClientSubscription*> set_subscribers;

	QMap<QString, Movie> map_requests; // to store temp requests

	QMap<QString, Movie> map_queue; // URL -> Movie

	void DecodeResponse( QNetworkReply* reply );

};

