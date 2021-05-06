#include "FileWriter.h"

#include <QFile>

FileWriter::FileWriter(void)
{
}


FileWriter::~FileWriter(void)
{
}

void FileWriter::WriteToFile( QString sPath, QByteArray data )
{
	QFile image(sPath);

	image.open(QIODevice::WriteOnly);
	image.write(data);
	image.close();

	emit FileWriteCompleted(sPath);
}
