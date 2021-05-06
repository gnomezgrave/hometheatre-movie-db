#pragma once

#include <QSettings>

#define APP_NAME	"HomeTheatre"
#define SCAN_PATHS	"ScanPaths"

class Settings
{
public:
	Settings(void);
	~Settings(void);

	static Settings* GetSettings();


	QStringList GetFoldersList();
	bool AddFolderToScan( QString sPath );
	bool RemoveFolderToScan( QString sPath );

	QString GetSettingsFolder(){ return s_settingsPath; }
	QString GetImagesFolder(){ return s_imagesPath; }

	void Save() { p_Settings->sync(); }

private:
	static Settings* p_instance;

	QSettings*	p_Settings;
	QString		s_settingsPath;
	QString		s_imagesPath;

	void CheckAndCreateSettings();

	bool CheckInsideStringList(QStringList list, QString str);
};

