#include "FolderScanner.h"

#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

#include <Movie.h>

FolderScanner::FolderScanner( MovieEngine* pMovieEngine ) : p_MovieEngine(pMovieEngine)
{
	lst_VideoExtensions << "*.264" << "*.3G2" << "*.3GP" << "*.3MM" << "*.3P2" << "*.60D" << "*.AAF" << "*.AEC" << "*.AEP" << "*.AEPX" << "*.AJP" << "*.AM4" << "*.AMV" << "*.ARF" << "*.ARV" << "*.ASD" << "*.ASF" << "*.ASX"
		<< "*.AVB" << "*.AVD" << "*.AVI" << "*.AVP" << "*.AVS" << "*.AVS" << "*.AX" << "*.AXM" << "*.BDMV" << "*.BIK" << "*.BIX" << "*.BOX" << "*.BPJ" << "*.BUP" << "*.CAMREC" << "*.CINE" << "*.CPI" << "*.CVC"
		<< "*.D2V" << "*.D3V" << "*.DAV" << "*.DCE" << "*.DDAT" << "*.DIVX" << "*.DKD" << "*.DLX" << "*.DMB" << "*.DM_84" << "*.DPG" << "*.DREAM" << "*.DSM" << "*.DV" << "*.DV2" << "*.DVM" << "*.DVR"
		<< "*.DVR   " << "*.DVX" << "*.DXR" << "*.EDL" << "*.ENC" << "*.EVO" << "*.F4V" << "*.FBR" << "*.FBZ" << "*.FCP" << "*.FCPROJECT" << "*.FLC" << "*.FLI" << "*.FLV" << "*.GTS" << "*.GVI" << "*.GVP" 
		<< "*.H3R" << "*.HDMOV" << "*.IFO" << "*.IMOVIEPROJ" << "*.IMOVIEPROJECT" << "*.IRCP" << "*.IRF" << "*.IRF" << "*.IVR" << "*.IVS" << "*.IZZ" << "*.IZZY" << "*.M1PG" << "*.M21" << "*.M21" << "*.M2P"
		<< "*.M2T" << "*.M2TS" << "*.M2V" << "*.M4E" << "*.M4U" << "*.M4V" << "*.MBF" << "*.MBT" << "*.MBV" << "*.MJ2" << "*.MJP" << "*.MK3D" << "*.MKV" << "*.MNV" << "*.MOCHA" << "*.MOD" << "*.MOFF" 
		<< "*.MOI" << "*.MOV" << "*.MP21" << "*.MP21" << "*.MP4" << "*.MP4V" << "*.MPEG" << "*.MPG" << "*.MPG2" << "*.MQV" << "*.MSDVD" << "*.MSWMM" << "*.MTS" << "*.MTV" << "*.MVB" << "*.MVP" << "*.MXF" 
		<< "*.MZT" << "*.NSV" << "*.OGV" << "*.OGX" << "*.PDS" << "*.PGI" << "*.PIV" << "*.PLB" << "*.PMF" << "*.PNS" << "*.PPJ" << "*.PRPROJ" << "*.PRTL" << "*.PSH" << "*.PVR" << "*.PXV" << "*.QT" << "*.QTL"
		<< "*.R3D" << "*.RATDVD" << "*.RM" << "*.RMS" << "*.RMVB" << "*.ROQ" << "*.RPF" << "*.RPL" << "*.RUM" << "*.RV" << "*.SDV" << "*.SFVIDCAP" << "*.SLC" << "*.SMK" << "*.SPL" << "*.SQZ" << "*.SUB" 
		<< "*.SVI" << "*.SWF" << "*.TDA3MT" << "*.THM" << "*.TIVO" << "*.TOD" << "*.TP0" << "*.TRP" << "*.TS" << "*.UDP" << "*.USM" << "*.VCR" << "*.VEG" << "*.VFT" << "*.VGZ" << "*.VIEWLET" << "*.VLAB" 
		<< "*.VMB" << "*.VOB" << "*.VP6" << "*.VP7" << "*.VRO" << "*.VSP" << "*.VVF" << "*.WD1" << "*.WEBM" << "*.WLMP" << "*.WMMP" << "*.WMV" << "*.WP3" << "*.WTV" << "*.XFL" << "*.XVID" << "*.ZM1" 
		<< "*.ZM2" << "*.ZM3" << "*.ZMV";
}


FolderScanner::~FolderScanner(void)
{
}

void FolderScanner::OnScanFolders(QStringList sFoldersList, bool bReset)
{
	if ( bReset )
	{
		map_Movies.clear();
	}
	
	int iCount = 0;
	
	foreach ( QString sFolder, sFoldersList )
	{
		iCount += ScanFolder(sFolder);
	}

	if ( iCount > 0 )
		emit FolderScanningCompleted(map_Movies, iCount );
}

int FolderScanner::ScanFolder( QString sFolder )
{
	// If already traversed, ignore (i.e. If the parent and the subfolder is both added to the list

	QDir dir(sFolder);

	if ( !dir.exists() )
	{
		QMessageBox::critical(NULL, "Error", "Invalid directory : " + sFolder);
		return 0;
	}

	return digFolder( sFolder );

}

int FolderScanner::digFolder( QString sPath )
{
	QDir dir(sPath);

	QFileInfoList dirInfoList = dir.entryInfoList(
		QDir::Dirs | QDir::NoDotAndDotDot,
		QDir::Name );

	QFileInfoList fileInfoList = dir.entryInfoList(
		lst_VideoExtensions,
		QDir::Files | QDir::NoSymLinks,
		QDir::Name );

	dirInfoList.append(fileInfoList); // join the files and sub-folders

	int iCount = 0;

	foreach (QFileInfo fileInfo, dirInfoList)
	{
		//qDebug() << fileInfo.filePath();

		if ( fileInfo.fileName().startsWith(".") )
			continue;

		if ( !fileInfo.exists())
			continue;

		if ( fileInfo.isFile() )
		{
			//qDebug() << fileInfo.fileName();

			if ( !map_Movies.contains(fileInfo.absoluteFilePath()) )
			{
				Movie movie = decodeMovie(fileInfo.fileName());
				map_Movies.insert(fileInfo.absoluteFilePath(), movie);
				iCount++;
			}
		}	
		else if ( fileInfo.isDir() )
		{
			iCount += digFolder( fileInfo.filePath() );
		}
	}
	return iCount;
}

Movie FolderScanner::decodeMovie( QFileInfo fileInfo )
{
	QString sFileName = fileInfo.fileName();
	QRegularExpression r1("[\\[\\{\\(][0-9]{4}"); // check for a year
	int i = sFileName.indexOf(r1);
	int yearStartIndex = i + 1;
	int year = 0;

	if ( i <= 0 )
	{
		QRegularExpression r2("[0-9]{4}");
		i = sFileName.indexOf(r2);
		yearStartIndex = i;
		// if the name starts with the year (i.e. 2000 : Space Oddisey)
		if ( i == 0 )
			i = sFileName.size();
	}

	if ( i < 0 )
		i = sFileName.lastIndexOf(".");
	else
		year = sFileName.left(yearStartIndex + 4).right(4).toInt();

	QString name = sFileName.left(i);
	name = name.replace("."," ").trimmed();

	//qDebug() << name << " - " << year << " => " << sFileName;

	Movie movie;

	movie.SetMovieTitle(name);
	movie.SetOriginalMovieTitle(name);
	movie.SetFilePath(fileInfo.absoluteFilePath());
	movie.SetYear(year);
	movie.s_DecodedMovieName = name;
	movie.m_fileInfo = fileInfo;

	return movie;
}
