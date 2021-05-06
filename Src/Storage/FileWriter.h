#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>

class FileWriter : public QObject
{
	Q_OBJECT

public:
	FileWriter(void);
	virtual ~FileWriter(void);

signals:
	void FileWriteCompleted(QString sPath);
public slots:
	void WriteToFile( QString sPath, QByteArray data  );
};

