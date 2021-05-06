#pragma once

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QMap>

#define COL_MOVIE_ID		"movie_id"
#define COL_MOVIE_NAME		"movie_name"
#define COL_ORG_TITLE		"org_title"
#define COL_RATING			"rating"
#define COL_OVERVIEW		"overview"
#define COL_POSTER			"poster"
#define COL_BACKDROP		"backdrop"
#define COL_RELEASE_DATE	"release_date"
#define COL_YEAR			"year"
#define COL_FILE_PATH		"file_path"
#define COL_WATCHED			"watched"
#define COL_REMAIN_MISSING	"remain_missing"

class MovieListener;

class Movie
{
public:
	Movie(void);
	Movie(const Movie& movie);
	~Movie(void);

private:
	bool	b_HasChanged;

public:
	void AddListener( MovieListener* listener);
	void BoradcastDelete();
	void Save();
	const QMap<QString, QString> GetChangedValues() { return map_updatedValues; }

	QString s_MovieID;
	QString s_DecodedMovieName;
	QString	s_MovieTitle;
	QString	s_OrgMovieTitle;
	QString	s_Overview;
	int		i_Year;
	double	d_rating;

	QString s_PosterPath;
	QString s_BackdropPath;

	QStringList lst_genreList;
	QString s_ReleaseDate;

	QString s_FilePath;
	bool	b_Watched;
	bool	b_RemainMissing;

	void	SetMovieID( QString movieID );
	void	SetMovieTitle( QString movieTitle );
	void	SetOriginalMovieTitle( QString orgMovieTitle );
	void	SetOverView( QString overview );
	void	SetYear( int year );
	void	SetReleaseDate( QString releaseDate );
	void	SetRating( double rating );
	void	SetPoster( QString poster );
	void	SetBackdrop( QString backdrop );
	void	SetFilePath( QString filePath );
	void	SetWatched( bool watched );
	void	SetRemainMissing( bool remainMissing );

	bool	HasChanged() { return b_HasChanged; }

	QString	GetUniqueKey() { return QString().append(s_MovieTitle).append(".").append(QString::number(i_Year)); }

	QFileInfo m_fileInfo;

	QList<MovieListener*> lst_Listeners;

	QMap<QString, QString> map_updatedValues;

};

class MovieListener
{
public:
	virtual void OnMovieDeleted(Movie* movie) = 0;
	virtual void OnMovieUpdated(Movie* movie) = 0;
};

