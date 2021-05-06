#pragma once

#include <ui_MovieCard.h>

#include <QListWidgetItem>
#include <Movie.h>

class MovieCard : public QWidget, public Ui::MovieCard, public MovieListener
{
public:
	MovieCard(QWidget* parent = NULL);
	virtual ~MovieCard(void);

	Movie* GetMovie() const { return m_movie; }
	void SetMovie(Movie* movie);

	void UpdateCard();

	void SetPoster( QString sPath );

	void UpdatePoster(QString posterPath);

	void SetDefaultPoster();

	virtual void OnMovieDeleted(Movie* movie);
	virtual void OnMovieUpdated(Movie* movie){}
private:
	Movie* m_movie;
	
};

