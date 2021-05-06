#include "MovieCard.h"
#include <Movie.h>
#include <Settings.h>


MovieCard::MovieCard(QWidget *parent) : QWidget(parent)
{
	setupUi(this);
	setCursor(Qt::PointingHandCursor);
}


MovieCard::~MovieCard(void)
{
}

void MovieCard::SetMovie( Movie* movie )
{
	m_movie = movie;
	UpdateCard();
}

void MovieCard::OnMovieDeleted( Movie* movie )
{
	m_movie = NULL;
}

void MovieCard::UpdateCard()
{
	lbl_MovieName->setText(m_movie->s_MovieTitle);
	lbl_MovieYear->setText(QString::number(m_movie->i_Year));
	lbl_watched->setVisible(m_movie->b_Watched);

	if ( !m_movie->s_PosterPath.isEmpty() )
	{
		QString sPath = Settings::GetSettings()->GetImagesFolder() + m_movie->s_PosterPath;
		SetPoster(sPath);

	}
}

void MovieCard::UpdatePoster(QString posterPath)
{
	if (posterPath.isEmpty())
	{
		SetDefaultPoster();
		return;
	}

	if ( m_movie->s_PosterPath.isEmpty() )
	{
		m_movie->SetPoster(posterPath);
		SetPoster(posterPath);
	}

}

void MovieCard::SetPoster( QString sPath )
{
	if ( sPath.isEmpty() )
	{
		SetDefaultPoster();
		return;
	}
	
	sPath = Settings::GetSettings()->GetImagesFolder().append(sPath);

	QFile img(sPath);

	if ( img.exists() )
	{
		QPixmap pixMap(sPath);
		lbl_MoviePoster->setPixmap(pixMap);
	}
}

void MovieCard::SetDefaultPoster()
{
	QString sPath = ":/images/Default_Poster.png";
	SetPoster(sPath);
}
