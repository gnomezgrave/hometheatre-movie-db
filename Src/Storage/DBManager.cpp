#include "DBManager.h"

#include <Settings.h>
#include <Movie.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


DBManager::DBManager(void)
{
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(Settings::GetSettings()->GetSettingsFolder()+"/"+"HomeTheatre.db");

	if (!m_db.open())
	{
		qDebug() << "Error: connection with database fail";
	}
}


DBManager::~DBManager(void)
{
}

bool DBManager::UpdateMovie( Movie* pMovie )
{
	QSqlQuery query;
	query.prepare("select movie_id from Movies where movie_id = (:id) ");
	query.bindValue(":id", pMovie->s_MovieID );

	if ( query.exec() )
	{
		if ( query.next() && pMovie->HasChanged())
		{
			query = BuildUpdateQuery(pMovie, pMovie->GetChangedValues());
		}
		else
		{
			query = BuildInsertQuery(pMovie, pMovie->GetChangedValues());
		}

		if ( query.exec() )
		{
			qDebug() << "Movie Added : " << pMovie->s_MovieTitle;
		}
		else
		{
			qDebug() << query.lastError();
			return false;
		}
	}
	else
	{
		qDebug() << query.lastError();
		return false;
	}

	return true;
}

QSqlQuery DBManager::BuildUpdateQuery( Movie* pMovie, QMap<QString, QString> valuesMap )
{
	QSqlQuery query;
	
	if ( valuesMap.isEmpty() )
		return query;

	QString sql = QString("update %1 set ").arg(MOVIE_TABLE);

	foreach ( QString key, valuesMap.keys())
	{
		sql += key + QString(" = ").append(":").append(key).append(", ");
	}
	
	sql = sql.left(sql.length() - 2);
	
	sql += QString(" where %1 = ").arg(COL_MOVIE_NAME).append(":").append(COL_MOVIE_NAME);

	
	query.prepare(sql);

	foreach ( QString key, valuesMap.keys())
	{
		query.bindValue(QString(":").append(key), valuesMap.value(key));
	}

	query.bindValue(QString(":").append(COL_MOVIE_NAME), pMovie->s_MovieTitle);

	//qDebug() << sql;

	return query;
}


QSqlQuery DBManager::BuildInsertQuery( Movie* pMovie, QMap<QString, QString> valuesMap )
{
	
	if ( valuesMap.isEmpty() )
	{
		valuesMap[COL_MOVIE_ID]			= pMovie->s_MovieID;
		valuesMap[COL_MOVIE_NAME]		= pMovie->s_MovieTitle;
		valuesMap[COL_ORG_TITLE]		= pMovie->s_OrgMovieTitle;
		valuesMap[COL_OVERVIEW]			= pMovie->s_Overview;
		valuesMap[COL_YEAR]				= QString::number(pMovie->i_Year);
		valuesMap[COL_RATING]			= QString::number(pMovie->d_rating, 'f', 2);
		valuesMap[COL_POSTER]			= pMovie->s_PosterPath;
		valuesMap[COL_BACKDROP]			= pMovie->s_BackdropPath;
		valuesMap[COL_RELEASE_DATE]		= pMovie->s_ReleaseDate;
		valuesMap[COL_FILE_PATH]		= pMovie->s_FilePath;
		valuesMap[COL_WATCHED]			= pMovie->b_Watched ? "true" : "false" ;
		valuesMap[COL_REMAIN_MISSING]	= pMovie->b_RemainMissing ? "true" : "false" ;
	}
	
	QString sql = QString("insert into %1(").arg(MOVIE_TABLE);

	foreach ( QString key, valuesMap.keys())
	{
		sql += key + QString(", ");
	}

	sql = sql.left(sql.length() - 2);

	sql += QString(") values (");

	foreach ( QString key, valuesMap.keys())
	{
		sql += QString().append(":").append(key).append(", ");
	}
	sql = sql.left(sql.length() - 2);
	sql += QString(")");

	QSqlQuery query;
	query.prepare(sql);

	foreach ( QString key, valuesMap.keys())
	{
		query.bindValue(QString(":").append(key), valuesMap.value(key));
	}

	//qDebug() << sql;

	return query;
}


void DBManager::LoadMoviesFromDB()
{
	QList<Movie> lstMovies;
	
	QSqlQuery query;
	if ( query.exec("select * from Movies") )
	{
		while ( query.next() )
		{
			QString movieID		= query.value(COL_MOVIE_ID).toString();
			QString movieName	= query.value(COL_MOVIE_NAME).toString();
			QString orgTitle	= query.value(COL_ORG_TITLE).toString();
			double	rating		= query.value(COL_RATING).toDouble();
			QString overview	= query.value(COL_OVERVIEW).toString();
			QString poster		= query.value(COL_POSTER).toString();
			QString backdrop	= query.value(COL_BACKDROP).toString();
			QString releaseDate = query.value(COL_RELEASE_DATE).toString();
			int		year		= query.value(COL_YEAR).toInt();

			Movie movie;
			movie.s_MovieID	=	movieID;
			movie.SetMovieTitle(movieName);
			movie.SetOriginalMovieTitle(orgTitle);
			movie.SetRating(rating);
			movie.SetOverView(overview);
			movie.SetPoster(poster);
			movie.SetBackdrop(backdrop);
			movie.SetReleaseDate(releaseDate);
			movie.SetYear(year);

			lstMovies.append(movie);
		}
	}
	emit LoadMoviesFromDBCompleted(lstMovies);
}
