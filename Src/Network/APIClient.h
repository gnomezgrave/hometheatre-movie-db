#pragma once

#include <QObject>
#include <QList>
#include <QString>
#include <Movie.h>

class APIClientSubscription;

class APIClient : public QObject
{
	Q_OBJECT

public:
	APIClient(void){}
	~APIClient(void){}

	virtual void AddMovieToSearchQueue(Movie pMovie, QMap<QString, QString> values) = 0;
	virtual void Subscribe( APIClientSubscription* subscriber ) = 0;

public slots:
	virtual void ProcessRequestQueue() = 0;

signals:
	void QueueProcessFinished();

};


class APIClientSubscription
{
public:
	virtual void OnMovieSearchResult(Movie pMovie, QList<Movie> movieList) = 0;
};
