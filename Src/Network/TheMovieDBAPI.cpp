#include "TheMovieDBAPI.h"

#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

#include <Movie.h>


TheMovieDBAPI::TheMovieDBAPI(void) : s_APIKey("ef05feba5592442abb1883d991ae5025")
{
	
	s_URL = QString("https://api.themoviedb.org/3/search/movie?api_key=%1").arg(s_APIKey);
	s_IncludeAdult = "false";
	s_DefaultLang = "en-US";

	connect( &networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)), Qt::QueuedConnection );
}


TheMovieDBAPI::~TheMovieDBAPI(void)
{
}

void TheMovieDBAPI::AddMovieToSearchQueue( Movie pMovie, QMap<QString, QString> values )
{
	if ( !values.contains("include_adult") && !s_IncludeAdult.isEmpty() )
		values.insert("include_adult", s_IncludeAdult);

	if ( !values.contains("language") && !s_DefaultLang.isEmpty() )
		values.insert("language", s_DefaultLang);
	
	QString sParams = "";

	foreach(auto e , values.keys())
	{
		sParams += "&" + QUrl::toPercentEncoding(e) + "=" + QUrl::toPercentEncoding( values.value(e) );
	}

	QString sURL = s_URL + sParams ;

	QNetworkRequest req(sURL);
	map_requests.insert(req.url().toDisplayString(), pMovie);
	
	map_queue.insert(sURL, pMovie);
	
}

void TheMovieDBAPI::replyFinished( QNetworkReply* reply )
{
	DecodeResponse(reply);
}

void TheMovieDBAPI::DecodeResponse( QNetworkReply* reply )
{
	QString sURL = reply->request().url().toDisplayString();

	map_queue.remove(sURL);
	
	if ( reply->error() != QNetworkReply::NoError )
	{
		qDebug() << reply->readAll();
		return;
	}

	QByteArray res = reply->readAll();

	reply->deleteLater();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(res);

	if ( jsonDoc.isEmpty() )
	{
		qDebug() << "Invalid JSON reply.";
		return;
	}

	Movie pMovie = map_requests.value(sURL);
	map_requests.remove(sURL);

	QJsonObject obj = jsonDoc.object();

	QJsonValue results = obj.value("results");

	QJsonArray arr = results.toArray();

	QList<Movie> movieList;

	foreach(QJsonValue v, arr)
	{
		QJsonObject o = v.toObject();

		Movie m;
		m.s_MovieID = o.value("id").toString();
		m.s_MovieTitle = o.value("title").toString();
		m.s_OrgMovieTitle = o.value("original_title").toString(m.s_MovieTitle);
		m.s_ReleaseDate = o.value("release_date").toString();
		m.s_DecodedMovieName = pMovie.s_DecodedMovieName;
		m.s_Overview = o.value("overview").toString("");
		m.s_PosterPath = o.value("poster_path").toString("");
		m.s_BackdropPath = o.value("backdrop_path").toString("");
		m.d_rating = o.value("vote_average").toDouble();
		m.i_Year = m.s_ReleaseDate.left(4).toInt(); // extract the year

		foreach ( QVariant v , o.value("genre_ids").toArray().toVariantList() )
		{
			m.lst_genreList << v.toString();
		}

		movieList.append(m);
	}

	foreach(APIClientSubscription* listener, set_subscribers)
	{
		listener->OnMovieSearchResult(pMovie, movieList);
	}

}

void TheMovieDBAPI::ProcessRequestQueue()
{
	foreach(auto entry, map_queue.keys())
	{
		QNetworkRequest req(entry);
		networkManager.get(req);
		QThread::currentThread()->msleep(25);
	}
}
