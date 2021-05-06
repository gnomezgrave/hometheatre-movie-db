#pragma once
#include <QThread>
#include <QNetworkAccessManager>
#include <QQueue>

class NetworkRequestManager : public QThread
{
public:
	NetworkRequestManager(void);
	virtual ~NetworkRequestManager(void);

private:
	QQueue<QString> requestQueue;
};

