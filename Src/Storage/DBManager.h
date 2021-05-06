#pragma once

#include <QObject>
#include <QList>
#include <QSqlDatabase>

#include <Movie.h>

#define MOVIE_TABLE			"Movies"
#define MOVIE_TABLE_KEY		"movie_id"

class DBManager : public QObject
{
	Q_OBJECT

public:
	DBManager(void);
	~DBManager(void);

	

public slots:
	void LoadMoviesFromDB();
	bool UpdateMovie(Movie* pMovie);

signals:
	void LoadMoviesFromDBCompleted(QList<Movie> lstMovies);


private:
	QSqlDatabase m_db;

	QSqlQuery BuildUpdateQuery( Movie* pMovie, QMap<QString, QString> valuesMap );
	QSqlQuery BuildInsertQuery( Movie* pMovie, QMap<QString, QString> valuesMap );
};

