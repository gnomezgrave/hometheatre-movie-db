#pragma once

#include <ui_PreferencesWnd.h>

#include <QDialog>
#include <QButtonGroup>


class HomeTheatre;

class PreferencesWnd : public QDialog, public Ui::PreferencesWnd
{
	Q_OBJECT

public:
	PreferencesWnd(QWidget *parent);
	virtual ~PreferencesWnd(void);

	HomeTheatre* GetParentWnd() const { return parentWnd; }
	void SetParentWnd(HomeTheatre* parent) { parentWnd = parent; }

public slots:
	void OnAddDirClicked(bool checked);
	void OnDelDirClicked(bool checked);
	void OnCloseClicked(bool checked);
	void OnRescanClicked(bool checked);
	void OnRebuildClicked(bool checked);

private:
	QButtonGroup btnGroup;
	bool hasFoldersChanged;

	HomeTheatre* parentWnd;

	QStringList lst_DeletedFolders;
	
	void LoadFoldersFromSettings();
};

