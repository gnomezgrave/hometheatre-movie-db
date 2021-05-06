#include <Src/UI/HomeTheatre.h>

#include <QtWidgets/QApplication>
#include <QFile>
#include <QMap>
#include <QList>
#include <QString>

#include<Movie.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QFile styleFile(":qdarkstyle/style.qss");
	styleFile.open(QFile::ReadOnly);

	qApp->setStyleSheet(styleFile.readAll());

	qRegisterMetaType< QMap<QString, Movie> >	("QMap<QString, Movie>");
	qRegisterMetaType< QList<Movie> >			("QList<Movie>");
	qRegisterMetaType< QMap<QString, Movie*> >	("QMap<QString, Movie*>");
	qRegisterMetaType< QList<Movie*> >			("QList<Movie*>");
	qRegisterMetaType< Movie >					("Movie");

	HomeTheatre w;
	w.show();
	return a.exec();
}
