//**************************************************************************
//
// Filename :	FileMan.c
//
//	Purpose :	function definitions for the memory manager
//
// Modification history :
//
//		24sep96:HJH		->creation
//	08Apr97:ARM	->Assign return value from Push() calls back to HStack
//					 handle, because it may possibly do a MemRealloc()
//		29Dec97:Kris Morness 
//									->Added functionality for setting file attributes which
//									allows for read-only attribute overriding
//									->Also added a simple function that clears all file attributes
//										to normal.
//
//		5 Feb 98:Dave French->extensive modification to support libraries
//
//**************************************************************************

//**************************************************************************
//
//				Includes
//
//**************************************************************************
	#include "Types.h"
	#include <stdlib.h>
	#include <malloc.h>
	#include <stdio.h>
	#include <direct.h>
	

	#include "windows.h"
	#include "FileMan.h"
	#include "MemMan.h"
	#include "Debug.h"
	#include "RegInst.h"
	#include "LibraryDataBase.h"
	#include "io.h"
	#include "sgp_logger.h"

using namespace std;

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_os_functions.h>
#include <vfs/Aspects/vfs_settings.h>


#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Tools/vfs_parser_tools.h>
#include <map>

struct SOperation
{
	enum EOperation
	{
		UNKNOWN, READ, WRITE,
	};
	EOperation op;
	SOperation() : op(UNKNOWN) {};
};

typedef std::map<vfs::IBaseFile*, SOperation> tFILEMAP;
static tFILEMAP s_mapFiles;

//**************************************************************************
//
//				Defines
//
//**************************************************************************

#define FILENAME_LENGTH					600

#define CHECKF(exp)	if (!(exp)) { return(FALSE); }
#define CHECKV(exp)	if (!(exp)) { return; }
#define CHECKN(exp)	if (!(exp)) { return(NULL); }
#define CHECKBI(exp) if (!(exp)) { return(-1); }

//**************************************************************************
//
//				Typedefs
//
//**************************************************************************

typedef struct FMFileInfoTag
{
	CHAR		strFilename[FILENAME_LENGTH];
	UINT8		uiFileAccess;
	UINT32	uiFilePosition;
	HANDLE	hFileHandle;

} FMFileInfo;	// for 'File Manager File Information'

typedef struct FileSystemTag
{
	FMFileInfo	*pFileInfo;
	UINT32	uiNumHandles;
	BOOLEAN	fDebug;
	BOOLEAN	fDBInitialized;

	CHAR		*pcFileNames;
	UINT32	uiNumFilesInDirectory;
} FileSystem;

//**************************************************************************
//
//				Variables
//
//**************************************************************************


//The FileDatabaseHeader
DatabaseManagerHeaderStruct gFileDataBase;


//FileSystem gfs;

WIN32_FIND_DATA Win32FindInfo[20];
BOOLEAN fFindInfoInUse[20] = {FALSE,FALSE,FALSE,FALSE,FALSE,
															FALSE,FALSE,FALSE,FALSE,FALSE,
															FALSE,FALSE,FALSE,FALSE,FALSE,
															FALSE,FALSE,FALSE,FALSE,FALSE };
HANDLE hFindInfoHandle[20] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
															INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };

// Snap: At program launch we build two directory catalogues:
// one for the default Data directory, the other for the custom Data directory.
TFileCat gDefaultDataCat;	// Init in InitializeStandardGamingPlatform (sgp.cpp)
TFileCat gCustomDataCat;	// Init in InitializeStandardGamingPlatform (sgp.cpp)

//**************************************************************************
//
//				Function Prototypes
//
//**************************************************************************

//**************************************************************************
//
//				Functions
//
//**************************************************************************

//**************************************************************************
//
// FileSystemInit
//
//		Starts up the file system.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		->creation
//
//**************************************************************************
BOOLEAN	InitializeFileManager(	STR strIndexFilename )
{
	RegisterDebugTopic( TOPIC_FILE_MANAGER, "File Manager" );
	return( TRUE );
}



//**************************************************************************
//
// FileSystemShutdown
//
//		Shuts down the file system.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

void ShutdownFileManager( void )
{
	UnRegisterDebugTopic( TOPIC_FILE_MANAGER, "File Manager" );
}


//**************************************************************************
//
// FileExists
//
//		Checks if a file exists.
//
// Parameter List :
//
//		STR	->name of file to check existence of
//
// Return Value :
//
//		BOOLEAN	->TRUE if it exists
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//	Oct 2005: Snap - Rewrote, made to check data catalogues
//
//**************************************************************************
BOOLEAN	FileExists( STR strFilename )
{
	return getVFS()->fileExists(vfs::Path(strFilename));
}

//**************************************************************************
//
// FileExistsNoDB
//
//		Checks if a file exists, but doesn't check the database files.
//
// Parameter List :
//
//		STR	->name of file to check existence of
//
// Return Value :
//
//		BOOLEAN	->TRUE if it exists
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//	Oct 2005: Snap - Rewrote, made to check data catalogues
//
//**************************************************************************
extern BOOLEAN	FileExistsNoDB( STR strFilename )
{
	return getVFS()->fileExists(vfs::Path(strFilename));
}

//**************************************************************************
//
// FileDelete
//
//		Deletes a file.
//
// Parameter List :
//
//		STR	->name of file to delete
//
// Return Value :
//
//		BOOLEAN	->TRUE if successful
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//**************************************************************************	
BOOLEAN	FileDelete( STR strFilename )
{
	return getVFS()->removeFileFromFS(vfs::Path(strFilename));
}

//**************************************************************************
//
// FileOpen
//
//		Opens a file.
//
// Parameter List :
//
//		STR	->filename
//		UIN32		->access - read or write, or both
//		BOOLEAN	->delete on close
//
// Return Value :
//
//		HWFILE	->handle of opened file
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//	Oct 2005: Snap - modified to work with the custom Data directory
//
//**************************************************************************
HWFILE FileOpen( STR strFilename, UINT32 uiOptions, BOOLEAN fDeleteOnClose, STR strProfilename )//dnl ch81 021213
{
	vfs::Path path(strFilename);
	vfs::IBaseFile *pFile = NULL;
	try
	{
		if(uiOptions & FILE_ACCESS_WRITE)
		{
			// 'vfs::CVirtualFile::SF_TOP' should be enough, but if for some strange reason
			// file creation fails, we will stop at a writable profile 
			// and won't unintentionally mess up a file from another profile
			vfs::COpenWriteFile open_w( path, true, false, vfs::CVirtualFile::SF_STOP_ON_WRITABLE_PROFILE);
			pFile = &open_w.file();
			open_w.release();
			s_mapFiles[pFile].op = SOperation::WRITE;
			return (HWFILE)pFile;
		}
		else if(uiOptions & FILE_ACCESS_READ)
		{
			if(strProfilename && strProfilename[0])
			{
				vfs::COpenReadFile open_r(vfs::tReadableFile::cast(getVFS()->getFile(path, strProfilename)));
				pFile = &open_r.file();
				open_r.release();
			}
			else
			{
				vfs::COpenReadFile open_r(path, vfs::CVirtualFile::SF_TOP);
				pFile = &open_r.file();
				open_r.release();
			}
			s_mapFiles[pFile].op = SOperation::READ;
			return (HWFILE)pFile;
		}
	}
	// sometimes a file is supposed to opened that does not exist (not tested with FileExists())
	// this operation can fail with an exception that the calling code doesn't catch
	// instead we catch it (any exception, not just CBasicException) here and return 0
	catch(vfs::Exception& ex) { SGP_ERROR(ex.what()); }
	catch(...)
	{ 
		SGP_ERROR( "Caught undefined exception" );
	}
	return 0;
}

//**************************************************************************
//
// FileClose
//
//
// Parameter List :
//
//		HWFILE hFile	->handle to file to close
//
// Return Value :
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************
void FileClose( HWFILE hFile )
{
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if(pFile)
	{
		pFile->close();
		s_mapFiles.erase(pFile);
	}
}

//**************************************************************************
//
// FileRead
//
//		To read a file.
//
// Parameter List :
//
//		HWFILE		->handle to file to read from
//		void	*	->source buffer
//		UINT32	->num bytes to read
//		UINT32	->num bytes read
//
// Return Value :
//
//		BOOLEAN	->TRUE if successful
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//		08Dec97:ARM		->return FALSE if bytes to read != bytes read
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

#ifdef JA2TESTVERSION
	extern UINT32 uiTotalFileReadTime;
	extern UINT32 uiTotalFileReadCalls;
	#include "Timer Control.h"

class TimeCounter
{
public:
	TimeCounter() : start_time(GetJA2Clock()) {}
	~TimeCounter()
	{
		uiTotalFileReadTime += GetJA2Clock() - start_time;
		uiTotalFileReadCalls++;
	}
private:
	UINT32 start_time;
};

#endif

BOOLEAN FileRead( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead )
{
#ifdef JA2TESTVERSION
	TimeCounter timer;
#endif
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if(pFile && (s_mapFiles[pFile].op == SOperation::READ))
	{
		vfs::tReadableFile *pRF = vfs::tReadableFile::cast(pFile);
		if(pRF)
		{
			UINT32 uiBytesRead = 0;
			try
			{
				uiBytesRead = pRF->read((vfs::Byte*)pDest, uiBytesToRead);
			}
			catch(std::exception& ex)
			{
				pRF->close();
				SGP_RETHROW(L"", ex);
			}

			if(puiBytesRead)
			{
				*puiBytesRead = uiBytesRead;
			}
			if(uiBytesToRead != uiBytesRead)
			{
				return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOLEAN FileReadLine( HWFILE hFile, std::string* pDest )
{
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if ( pFile && FileCheckEndOfFile( hFile ) == FALSE && (s_mapFiles[pFile].op == SOperation::READ) )
	{
		vfs::tReadableFile *pRF = vfs::tReadableFile::cast( pFile );
		if ( pRF && pDest )
		{
			vfs::CReadLine rl( *pRF, false );
			rl.getLine( *pDest );
			return TRUE;
		}
	}
	return FALSE;
}

BOOLEAN FileReadLine( HWFILE hFile, STR8 pDest, UINT32 uiDestSize, UINT32 *puiBytesRead )
{
	std::string	sBuffer;
	BOOLEAN result = (pDest != NULL) && FileReadLine( hFile, &sBuffer );
	if ( result )
	{
		if ( puiBytesRead )
			*puiBytesRead = sBuffer.length();
		
		UINT32 uiCountToCopy = min( sBuffer.length(), uiDestSize - 1 );
		sBuffer.copy( pDest, uiCountToCopy );
		pDest[uiCountToCopy] = '\0';  // method copy() does not put a null-terminator
	}
	return result;
}

//**************************************************************************
//
// FileWrite
//
//		To write a file.
//
// Parameter List :
//
//		HWFILE		->handle to file to write to
//		void	*	->destination buffer
//		UINT32	->num bytes to write
//		UINT32	->num bytes written
//
// Return Value :
//
//		BOOLEAN	->TRUE if successful
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//		08Dec97:ARM		->return FALSE if dwNumBytesToWrite != dwNumBytesWritten
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

BOOLEAN FileWrite( HWFILE hFile, PTR pDest, UINT32 uiBytesToWrite, UINT32 *puiBytesWritten )
{
	if(uiBytesToWrite == 0)//dnl ch38 110909
	{
		*puiBytesWritten = 0;
		return(TRUE);
	}
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if(pFile && (s_mapFiles[pFile].op == SOperation::WRITE))
	{
		vfs::tWritableFile *pWF = vfs::tWritableFile::cast(pFile);
		if(pWF)
		{
			UINT32 uiBytesWritten;
			try
			{
				uiBytesWritten = pWF->write((vfs::Byte*)pDest, uiBytesToWrite);
			}
			catch(std::exception& ex)
			{
				pWF->close();
				SGP_RETHROW(L"", ex);
			}

			if (uiBytesToWrite != uiBytesWritten)
			{
				return FALSE;
			}
			if ( puiBytesWritten )
			{
				*puiBytesWritten = uiBytesWritten;
			}
			return TRUE;
		}
	}
	return FALSE;
}

//**************************************************************************
//
// FileLoad
//
//		To open, read, and close a file.
//
// Parameter List :
//
//
// Return Value :
//
//		BOOLEAN	->TRUE if successful
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//		08Dec97:ARM		->return FALSE if bytes to read != bytes read (CHECKF is inappropriate?)
//
//**************************************************************************

BOOLEAN FileLoad( STR strFilename, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead )
{
	vfs::tReadableFile *pFile = getVFS()->getReadFile(vfs::Path(strFilename));
	vfs::COpenReadFile rfile(pFile);
	if(pFile)
	{
		UINT32 uiNumBytesRead;
		SGP_TRYCATCH_RETHROW(uiNumBytesRead = pFile->read((vfs::Byte*)pDest,uiBytesToRead), L"");

		if (uiBytesToRead != uiNumBytesRead)
		{
			return FALSE;
		}
		if ( puiBytesRead )
		{
			*puiBytesRead = uiNumBytesRead;
		}
		CHECKF( uiNumBytesRead == uiBytesToRead );
		return TRUE;
	}
	return FALSE;
}

//**************************************************************************
//
// FilePrintf
//
//		To printf to a file.
//
// Parameter List :
//
//		HWFILE	->handle to file to seek in
//		...		->arguments, 1st of which should be a string
//
// Return Value :
//
//		BOOLEAN	->TRUE if successful
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************
#ifndef DIM
# define DIM(x) (sizeof(x)/sizeof(x[0]))	/* made StringLen Save, Sergeant_Kolja, 2007-06-10 */
#endif


BOOLEAN _cdecl FilePrintf( HWFILE hFile, STR8	strFormatted, ... )
{
	CHAR8		strToSend[160]; /* itemdescription of item 0 will NOT fit if only 80 Chars per Line!, Sergeant_Kolja, 2007-06-10 */
	va_list	argptr;
	BOOLEAN fRetVal = FALSE;

	va_start(argptr, strFormatted);
	_vsnprintf( strToSend, DIM(strToSend), strFormatted, argptr ); /* made StringLen Save, Sergeant_Kolja, 2007-06-10 */
	strToSend[ DIM(strToSend)-1 ] = 0;
	va_end(argptr);
	
	fRetVal = FileWrite( hFile, strToSend, strlen(strToSend), NULL );
	return( fRetVal );
}

//**************************************************************************
//
// FileSeek
//
//		To seek to a position in a file.
//
// Parameter List :
//
//		HWFILE	->handle to file to seek in
//		UINT32	->distance to seek
//		UINT8		->how to seek
//
// Return Value :
//
//		BOOLEAN	->TRUE if successful
//					->FALSE if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

BOOLEAN FileSeek( HWFILE hFile, UINT32 uiDistance, UINT8 uiHow )
{
	INT32 iDistance = (INT32)uiDistance;

	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if(pFile)
	{
		vfs::IBaseFile::ESeekDir eSD;
		if ( uiHow == FILE_SEEK_FROM_START )
		{
			eSD = vfs::IBaseFile::SD_BEGIN;
		}
		else if ( uiHow == FILE_SEEK_FROM_END )
		{
			eSD = vfs::IBaseFile::SD_END;
			if( iDistance > 0 )
			{
				iDistance = -(iDistance);
			}
		}
		else
		{
			eSD = vfs::IBaseFile::SD_CURRENT;
		}

		if(s_mapFiles[pFile].op == SOperation::WRITE)
		{
			vfs::tWritableFile *pWF = vfs::tWritableFile::cast(pFile);
			if(pWF)
			{
				SGP_TRYCATCH_RETHROW(pWF->setWritePosition(iDistance, eSD), L"");
				return TRUE;
			}
		}
		else if(s_mapFiles[pFile].op == SOperation::READ)
		{
			vfs::tReadableFile *pRF = vfs::tReadableFile::cast(pFile);
			if(pRF)
			{
				SGP_TRYCATCH_RETHROW(pRF->setReadPosition(iDistance, eSD), L"");
				return TRUE;
			}
		}
		else
		{
			SGP_THROW(L"unknown operation");
		}
	}
	return FALSE;
}

//**************************************************************************
//
// FileGetPos
//
//		To get the current position in a file.
//
// Parameter List :
//
//		HWFILE	->handle to file
//
// Return Value :
//
//		INT32		->current offset in file if successful
//					->-1 if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

INT32 FileGetPos( HWFILE hFile )
{
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if(pFile && (s_mapFiles[pFile].op == SOperation::WRITE))
	{
		vfs::tWritableFile *pWF = vfs::tWritableFile::cast(pFile);
		if(pWF)
		{
			return pWF->getWritePosition();
		}
	}
	else if(pFile && (s_mapFiles[pFile].op == SOperation::READ))
	{
		vfs::tReadableFile *pRF = vfs::tReadableFile::cast(pFile);
		if(pRF)
		{
			return pRF->getReadPosition();
		}
	}

	return BAD_INDEX;
}

//**************************************************************************
//
// FileGetSize
//
//		To get the current file size.
//
// Parameter List :
//
//		HWFILE	->handle to file
//
// Return Value :
//
//		INT32		->file size in file if successful
//					->0 if not
//
// Modification history :
//
//		24sep96:HJH		->creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

UINT32 FileGetSize( HWFILE hFile )
{
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;
	if(pFile)
	{
		return pFile->getSize();
	}
	return 0;
}


BOOLEAN SetFileManCurrentDirectory( STR pcDirectory )
{
	try
	{
		vfs::OS::setCurrectDirectory(pcDirectory);
	}
	catch(vfs::Exception& ex)
	{
		SGP_ERROR(ex.what());
		return FALSE;
	}
	return TRUE;
}


BOOLEAN GetFileManCurrentDirectory( STRING512 pcDirectory )
{
	try
	{
		vfs::Path sDir;
		vfs::OS::getCurrentDirectory(sDir);
		strncpy(pcDirectory, sDir.to_string().c_str(), 512);
	}
	catch(vfs::Exception& ex)
	{
		SGP_ERROR(ex.what());
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Removes ALL FILES in the specified directory (and all subdirectories with their files if fRecursive is TRUE)
// Use EraseDirectory() to simply delete directory contents without deleting the directory itself
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN RemoveFileManDirectory( STRING512 pcDirectory, BOOLEAN fRecursive )
{
	// ignore 'recursive' flag, just delete every file in that subtree (but leave the directories)
	return getVFS()->removeDirectoryFromFS(pcDirectory);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Removes ALL FILES in the specified directory but leaves the directory alone.	Does not affect any subdirectories!
// Use RemoveFilemanDirectory() to also delete the directory itself, or to recursively delete subdirectories.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN EraseDirectory( STRING512 pcDirectory)
{
	// ignore 'recursive' flag, just delete every file in that subtree (but leave the directories)
	return getVFS()->removeDirectoryFromFS(pcDirectory);
}


BOOLEAN GetExecutableDirectory( STRING512 pcDirectory )
{
	vfs::Path exe_dir, exe_file;
	vfs::OS::getExecutablePath(exe_dir, exe_file);
	strncpy(pcDirectory, exe_dir.to_string().c_str(), 512);
	return true;
}

static vfs::CVirtualFileSystem::Iterator file_iter; 
BOOLEAN GetFileFirst( CHAR8 * pSpec, GETFILESTRUCT *pGFStruct )
{
	CHECKF( pSpec != NULL );
	CHECKF( pGFStruct != NULL );

	file_iter = getVFS()->begin(pSpec);
	if(!file_iter.end())
	{
		vfs::Path const& path = file_iter.value()->getName();
		std::string s = path.to_string();
		::size_t size = s.length();
		size = std::min< ::size_t>(size,260-1);
		sprintf( pGFStruct->zFileName, s.c_str());
		pGFStruct->zFileName[size] = 0;
		
		pGFStruct->iFindHandle = 0;
		pGFStruct->uiFileSize = file_iter.value()->getSize();
		pGFStruct->uiFileAttribs = ( file_iter.value()->implementsWritable() ? FILE_IS_NORMAL : FILE_IS_READONLY );

		return TRUE;
	}
	return FALSE;
}

BOOLEAN GetFileNext( GETFILESTRUCT *pGFStruct )
{
	if(!file_iter.end())
	{
		file_iter.next();
	}
	if(!file_iter.end())
	{
		vfs::Path const& path = file_iter.value()->getName();
		std::string s = path.to_string();
		::size_t size = s.length();
		size = std::min< ::size_t>(size,260-1);
		sprintf( pGFStruct->zFileName, s.c_str());
		pGFStruct->zFileName[size] = 0;

		pGFStruct->iFindHandle = 0;
		pGFStruct->uiFileSize = file_iter.value()->getSize();
		pGFStruct->uiFileAttribs = ( file_iter.value()->implementsWritable() ? FILE_IS_NORMAL : FILE_IS_READONLY );

		return TRUE;
	}
	return FALSE;
}

void GetFileClose( GETFILESTRUCT *pGFStruct )
{
	file_iter = vfs::CVirtualFileSystem::Iterator();
}


//returns true if at end of file, else false
BOOLEAN	FileCheckEndOfFile( HWFILE hFile )
{
	vfs::size_t current_position, max_position;
	vfs::IBaseFile *pFile = (vfs::IBaseFile*)hFile;

	if(pFile && (s_mapFiles[pFile].op == SOperation::WRITE))
	{
		vfs::tWritableFile *pWF = vfs::tWritableFile::cast(pFile);
		if(pWF)
		{
			current_position = pWF->getWritePosition();
			max_position = pWF->getSize();
			return current_position >= max_position;
		}
	}
	else if(pFile && (s_mapFiles[pFile].op == SOperation::READ))
	{
		vfs::tReadableFile *pRF = vfs::tReadableFile::cast(pFile);
		if(pRF)
		{
			current_position = pRF->getReadPosition();
			max_position = pRF->getSize();
			return current_position >= max_position;
		}
	}
	return FALSE;
}


UINT32 FileSize(STR strFilename)
{
	vfs::IBaseFile *pFile = getVFS()->getFile(vfs::Path(strFilename));
	if(pFile)
	{
		return pFile->getSize();
	}
	return 0;
}


UINT32 GetFreeSpaceOnHardDriveWhereGameIsRunningFrom( )
{
	STRING512		zExecDir;
	STRING512		zDrive;
	STRING512		zDir;
	STRING512		zFileName;
	STRING512		zExt;

	UINT32 uiFreeSpace = 0;

	GetExecutableDirectory( zExecDir );

	//get the drive letter from the exec dir
	_splitpath( zExecDir, zDrive, zDir, zFileName, zExt);

	sprintf( zDrive, "%s\\", zDrive );
	
	uiFreeSpace = GetFreeSpaceOnHardDrive( zDrive );

	return( uiFreeSpace );
}




UINT32 GetFreeSpaceOnHardDrive( STR pzDriveLetter )
{
	UINT32			uiBytesFree=0;

	UINT32			uiSectorsPerCluster=0;
	UINT32			uiBytesPerSector=0;
	UINT32			uiNumberOfFreeClusters=0;
	UINT32			uiTotalNumberOfClusters=0;

	if( !GetDiskFreeSpace( pzDriveLetter, (LPDWORD) &uiSectorsPerCluster, (LPDWORD) &uiBytesPerSector, 
			(LPDWORD) &uiNumberOfFreeClusters, (LPDWORD) &uiTotalNumberOfClusters ) )
	{
		UINT32 uiLastError = GetLastError();
		char zString[1024];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, uiLastError, 0, zString, 1024, NULL);

		return( TRUE );
	}

	uiBytesFree = uiBytesPerSector * uiNumberOfFreeClusters * uiSectorsPerCluster;

	return( uiBytesFree );
}

// Flugente: simple wrapper to check whether an audio file in mp3/ogg/wav format exists
BOOLEAN	SoundFileExists( STR strFilename, STR zFoundFilename )
{
	sprintf( zFoundFilename, "%s.mp3", strFilename );
	if ( !FileExists( zFoundFilename ) )
	{
		sprintf( zFoundFilename, "%s.ogg", strFilename );
		if ( !FileExists( zFoundFilename ) )
		{
			sprintf( zFoundFilename, "%s.wav", strFilename );
		}
	}

	return FileExists( zFoundFilename );
}
