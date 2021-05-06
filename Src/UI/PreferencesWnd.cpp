#include "PreferencesWnd.h"

#include <QFileDialog>
#include <QMessageBox>

#include <Settings.h>
#include <HomeTheatre.h>

PreferencesWnd::PreferencesWnd(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	hasFoldersChanged = false;

	btnGroup.addButton(btnFolders);
	btnGroup.addButton(btnSearch);

	LoadFoldersFromSettings();

	connect(btnAddDir, SIGNAL(clicked(bool)), this, SLOT(OnAddDirClicked(bool)));
	connect(btnRemoveDir, SIGNAL(clicked(bool)), this, SLOT(OnDelDirClicked(bool)));
	connect(btnClose, SIGNAL(clicked(bool)), this, SLOT(OnCloseClicked(bool)));
	connect(btnRescan, SIGNAL(clicked(bool)), this, SLOT(OnRescanClicked(bool)));
	connect(btnRebuild, SIGNAL(clicked(bool)), this, SLOT(OnRebuildClicked(bool)));
}


PreferencesWnd::~PreferencesWnd(void)
{
}

void PreferencesWnd::OnAddDirClicked( bool checked )
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if ( !dir.isEmpty() )
	{
		Settings* settings = Settings::GetSettings();
		if ( !settings->AddFolderToScan(dir) )
		{
			QMessageBox::warning(this, "Duplicate Path", "The folder you selected is already in the list.");
		}
		else
		{
			foldersList->addItem(dir);

			if ( lst_DeletedFolders.contains(dir) )
				lst_DeletedFolders.removeAll(dir);

			settings->Save();
			hasFoldersChanged = true;
		}
	}
}

void PreferencesWnd::LoadFoldersFromSettings()
{
	foldersList->clear();

	QStringList paths = Settings::GetSettings()->GetFoldersList();

	foreach ( QString path, paths )
	{
		foldersList->addItem(path);
	}
}

void PreferencesWnd::OnDelDirClicked( bool checked )
{
	if ( foldersList->selectedItems().size() != 0 )
	{
		QListWidgetItem* item = foldersList->selectedItems().at(0);
		QString path = item->text();
		Settings* settings = Settings::GetSettings();
		if ( !settings->RemoveFolderToScan(path) )
		{
			QMessageBox::warning(this, "Error", "An error occurred while removing the directory. Please try again.");
		}
		else
		{
			qDeleteAll(foldersList->selectedItems());
			lst_DeletedFolders.append(path);
			settings->Save();
			hasFoldersChanged = true;
		}
	}
}

void PreferencesWnd::OnCloseClicked( bool checked )
{
	if ( hasFoldersChanged )
	{
		hasFoldersChanged = false;
		OnRescanClicked(false);

	}
	lst_DeletedFolders.clear();
	hide();
}

void PreferencesWnd::OnRescanClicked( bool checked )
{
	QMessageBox::StandardButton reply = QMessageBox::warning(this, "Folders list changed!", "Do you want to reload the library?", QMessageBox::Yes | QMessageBox::No);

	if ( reply == QMessageBox::Yes )
	{
		parentWnd->ReloadLibrary(Settings::GetSettings()->GetFoldersList());
		//QMessageBox::information(this, "Reload Completed!", QString::number(iCount) + " movie" + (iCount !=1 ? "s": "") + " added to the library.");
	}
}

void PreferencesWnd::OnRebuildClicked( bool checked )
{
	QMessageBox::StandardButton reply = QMessageBox::warning(this, "Rebuild the library", "Your complete movie library will be reloaded. "\
		"But your saved data (i.e. ratings, watched movies) will not be erased. Do you want to proceed?", QMessageBox::Yes | QMessageBox::No);

	if ( reply == QMessageBox::Yes )
	{
		parentWnd->ReloadLibrary(Settings::GetSettings()->GetFoldersList(), true ); // hard reset
		//QMessageBox::information(this, "Rebuild Completed!", QString::number(iCount) + " movie" + (iCount !=1 ? "s": "") + " loaded to the library.");
	}
}
