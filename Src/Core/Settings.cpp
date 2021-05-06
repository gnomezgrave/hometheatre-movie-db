#include "Settings.h"

#include <QDir>
#include <QDebug>

Settings* Settings::p_instance = NULL;

Settings* Settings::GetSettings()
{
	if ( p_instance == NULL )
		p_instance = new Settings();

	return p_instance;
}

Settings::Settings(void)
{
	CheckAndCreateSettings();
}


Settings::~Settings(void)
{
}

void Settings::CheckAndCreateSettings()
{
	QString home	= QDir::homePath();
	s_settingsPath	= home + "/" + APP_NAME;
	s_imagesPath	= s_settingsPath + "/images";

	QDir dir(s_settingsPath);

	if ( !dir.exists() )
	{
		QDir homeFolder(home);
		homeFolder.mkdir(APP_NAME);
	}

	p_Settings = new QSettings(s_settingsPath + "/settings.ini", QSettings::IniFormat);
	p_Settings->sync();
	
}

bool Settings::AddFolderToScan( QString sPath )
{
	sPath = sPath.trimmed();
	QVariant val = p_Settings->value(SCAN_PATHS);
	QStringList paths = val.toStringList();

	if ( CheckInsideStringList(paths, sPath) )
	{
		return false;
	}

	paths.append(sPath);
	p_Settings->setValue( SCAN_PATHS, QVariant(paths) );
	return true;
}

bool Settings::RemoveFolderToScan( QString sPath )
{
	sPath = sPath.trimmed();
	QVariant val = p_Settings->value(SCAN_PATHS);
	QStringList paths = val.toStringList();

	if ( !CheckInsideStringList(paths, sPath) )
	{
		return false;
	}

	paths.removeAll(sPath);
	p_Settings->setValue( SCAN_PATHS, QVariant(paths) );
	return true;
}

bool Settings::CheckInsideStringList( QStringList list, QString str )
{
#ifdef Q_OS_WIN32
	if ( list.contains(str, Qt::CaseInsensitive) )
		return true;
#else
	if ( list.contains(str, Qt::CaseSensitive) )
		return true;
#endif
	return false;
}

QStringList Settings::GetFoldersList()
{
	return p_Settings->value(SCAN_PATHS).toStringList();
}

