#include "Movie.h"

Movie::Movie(void)
{
	b_HasChanged	= false;
	b_RemainMissing = false;
	b_Watched		= false;
	i_Year			= 0;
	d_rating		= 0;
}

Movie::Movie( const Movie& movie )
{
	SetMovieID(movie.s_MovieID);
	SetMovieTitle(movie.s_MovieTitle);
	SetOriginalMovieTitle(movie.s_OrgMovieTitle);
	SetOverView( movie.s_Overview );
	SetYear( movie.i_Year );
	SetReleaseDate( movie.s_ReleaseDate );
	SetRating( movie.d_rating );
	SetPoster( movie.s_PosterPath );
	SetBackdrop( movie.s_BackdropPath );
	SetFilePath( movie.s_FilePath );
	SetWatched( movie.b_Watched );
	SetRemainMissing( movie.b_RemainMissing );
}


Movie::~Movie(void)
{
	BoradcastDelete();
}

void Movie::AddListener( MovieListener* listener )
{
	if ( !lst_Listeners.contains(listener) )
		lst_Listeners.append(listener);
}

void Movie::BoradcastDelete()
{
	foreach(MovieListener* listener, lst_Listeners )
	{
		listener->OnMovieDeleted(this);
	}
}

void Movie::SetMovieID( QString movieID )
{
	if ( s_MovieID == movieID )
		return;
	
	s_MovieID = movieID;
	map_updatedValues.insert(COL_MOVIE_ID, movieID);
	b_HasChanged = true;
}

void Movie::SetMovieTitle( QString movieTitle )
{
	if ( s_MovieTitle == movieTitle )
		return;

	s_MovieTitle = movieTitle;
	map_updatedValues.insert( COL_MOVIE_NAME, movieTitle);
	b_HasChanged = true;
}

void Movie::SetOriginalMovieTitle( QString orgMovieTitle )
{
	if ( s_OrgMovieTitle == orgMovieTitle )
		return;

	s_OrgMovieTitle = orgMovieTitle;
	map_updatedValues.insert( COL_ORG_TITLE, orgMovieTitle);
	b_HasChanged = true;
}

void Movie::SetOverView( QString overview )
{
	if ( s_Overview == overview )
		return;

	s_Overview = overview;
	map_updatedValues.insert( COL_OVERVIEW, overview);
	b_HasChanged = true;
}

void Movie::SetYear( int year )
{
	if ( i_Year == year )
		return;

	i_Year = year;
	map_updatedValues.insert( COL_YEAR, QString::number(year));
	b_HasChanged = true;
}

void Movie::SetReleaseDate( QString releaseDate )
{
	if ( s_ReleaseDate == releaseDate )
		return;

	s_ReleaseDate = releaseDate;
	map_updatedValues.insert( COL_RELEASE_DATE, releaseDate );
	b_HasChanged = true;
}

void Movie::SetRating( double rating )
{
	QString sRating = QString::number(rating, 'f', 2);
	if ( QString::number(d_rating, 'f', 2) == sRating )
		return;

	d_rating = rating;
	map_updatedValues.insert( COL_RATING, sRating );
	b_HasChanged = true;
}

void Movie::SetPoster( QString poster )
{
	if ( s_PosterPath == poster )
		return;

	s_PosterPath = poster;
	map_updatedValues.insert(COL_POSTER, poster);
	b_HasChanged = true;
}

void Movie::SetBackdrop( QString backdrop )
{
	if ( s_BackdropPath == backdrop )
		return;

	s_BackdropPath = backdrop;
	map_updatedValues.insert(COL_BACKDROP, backdrop);
	b_HasChanged = true;
}

void Movie::Save()
{
	map_updatedValues.clear();
	b_HasChanged = false;
}

void Movie::SetFilePath( QString filePath )
{
	if ( s_FilePath == filePath )
		return;

	s_FilePath = filePath;
	map_updatedValues.insert(COL_FILE_PATH, filePath);
	b_HasChanged = true;
}

void Movie::SetWatched( bool watched )
{
	if ( b_Watched == watched )
		return;

	b_Watched = watched;
	map_updatedValues.insert(COL_WATCHED, watched ? "true" : "false");
	b_HasChanged = true;
}

void Movie::SetRemainMissing( bool remainMissing )
{
	if ( b_RemainMissing == remainMissing )
		return;

	b_RemainMissing = remainMissing;
	map_updatedValues.insert(COL_REMAIN_MISSING, remainMissing ? "true" : "false");
	b_HasChanged = true;
}
