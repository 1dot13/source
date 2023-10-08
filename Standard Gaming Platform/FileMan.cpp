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
	#include "Container.h"
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

void W32toSGPFileFind( GETFILESTRUCT *pGFStruct, WIN32_FIND_DATA *pW32Struct );

HANDLE	GetHandleToRealFile( HWFILE hFile, BOOLEAN *pfDatabaseFile );
HWFILE	CreateFileHandle( HANDLE hRealFile, BOOLEAN fDatabaseFile );
void		DestroyFileHandle( HWFILE hFile );
void		BuildFileDirectory( void );
INT32		GetFilesInDirectory( HCONTAINER hStack, CHAR *, HANDLE hFile, WIN32_FIND_DATA *pFind );

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
// FileDebug
//
//		To set whether or not we should print debug info.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		->creation
//
//**************************************************************************

void FileDebug( BOOLEAN f )
{
//	gfs.fDebug = f;
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


//**************************************************************************
//
// GetHandleToRealFile
//
//		
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

HANDLE GetHandleToRealFile( HWFILE hFile, BOOLEAN *pfDatabaseFile )
{
	HANDLE	hRealFile;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		*pfDatabaseFile = FALSE;
	}
	else
	{
		*pfDatabaseFile = TRUE;
		hRealFile = (HANDLE) hFile;
	}

	return(hRealFile);
}

//**************************************************************************
//
// CreateFileHandle
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		->creation
//
//**************************************************************************
/*

	not needed anymore

HWFILE CreateFileHandle( HANDLE hRealFile, BOOLEAN fDatabaseFile )
{
	UINT32		i, uiOldNumHandles;
	FMFileInfo		*pNewFileInfo;

	Assert( !fDatabaseFile || (fDatabaseFile && gfs.fDBInitialized) );

	// don't use 1st position - it'll confuse the users
	for ( i=1 ; i<gfs.uiNumHandles ; i++ )
	{
		if ( gfs.pFileInfo[i].hFileHandle == 0 && gfs.pFileInfo[i].hDBFile == 0 )
		{
			if ( fDatabaseFile )
				gfs.pFileInfo[i].hDBFile = (HDBFILE)hRealFile;
			else
				gfs.pFileInfo[i].hFileHandle = hRealFile;
			return( i );
		}
	}

	uiOldNumHandles = gfs.uiNumHandles;

	pNewFileInfo = (FMFileInfo *)MemRealloc( gfs.pFileInfo, gfs.uiNumHandles + NUM_FILES_TO_ADD_AT_A_TIME );
	if ( !pNewFileInfo )
	{
		// TBD: error error error
		return(0);
	}
	gfs.pFileInfo = (FMFileInfo *)pNewFileInfo;
	gfs.uiNumHandles = gfs.uiNumHandles + NUM_FILES_TO_ADD_AT_A_TIME;

	for ( i=uiOldNumHandles ; i<gfs.uiNumHandles ; i++ )
	{
		gfs.pFileInfo[i].hFileHandle = 0;
		gfs.pFileInfo[i].hDBFile = 0;
	}

	if ( fDatabaseFile )
		gfs.pFileInfo[uiOldNumHandles].hDBFile = (HDBFILE)hRealFile;
	else
		gfs.pFileInfo[uiOldNumHandles].hFileHandle = hRealFile;

	return(uiOldNumHandles);
}
*/

//**************************************************************************
//
// DestroyFileHandle
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		->creation
//
//**************************************************************************
/*
void DestroyFileHandle( HWFILE hFile )
{
	if ( hFile < gfs.uiNumHandles && hFile )
	{
		gfs.pFileInfo[hFile].hFileHandle = 0;
		gfs.pFileInfo[hFile].hDBFile = 0;
	}
}
*/



//**************************************************************************
//
// BuildFileDirectory
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		??nov96:HJH		->creation
//
//**************************************************************************

void BuildFileDirectory( void )
{

	return;	// temporary until container stuff is fixed
/*
	INT32					i, iNumFiles = 0;
	HANDLE				hFile, hFileIn;
	WIN32_FIND_DATA	find, inFind;
	BOOLEAN				fMore = TRUE;
	CHAR					cName[FILENAME_LENGTH], cDir[FILENAME_LENGTH], cSubDir[FILENAME_LENGTH];
	HCONTAINER			hStack;



	//
	//	First, push all the file names in the directory (and subdirectories)
	//	onto the stack.
	//

	GetProfileChar( "Startup", "InstPath", "", cDir );

	if ( strlen( cDir ) == 0 )
		return;

	hStack = CreateStack( 100, FILENAME_LENGTH );
	if (hStack == NULL)
	{
		FastDebugMsg(String("BuildFileDirectory: CreateStack Failed for the filename stack"));
		return;
	}

	find.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;

	strcpy( &(cDir[strlen(cDir)]), "\\*.*\0" );
	hFile = FindFirstFile( cDir, &find );
	while ( fMore )
	{
		if ( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( strcmp( find.cFileName, "." ) != 0 && strcmp( find.cFileName, ".." ) != 0 )
			{
				// a valid directory
				inFind.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
				strcpy( cSubDir, cDir );
				strcpy( &(cSubDir[strlen(cDir)-3]), find.cFileName );
				strcpy( &(cSubDir[strlen(cSubDir)]), "\\*.*\0" );
				hFileIn = FindFirstFile( cSubDir, &inFind );
				iNumFiles += GetFilesInDirectory( hStack, cSubDir, hFileIn, &inFind );
				FindClose( hFileIn );
			}
		}
		else
		{
			iNumFiles++;
			strcpy( cName, cDir );
			strcpy( &(cName[strlen(cName)-3]), find.cFileName );
			CharLower( cName );
			hStack = Push( hStack, cName );
		}
		find.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
		fMore = FindNextFile( hFile, &find );
	}
	FindClose( hFile );

	//
	//	Okay, we have all the files in the stack, now put them in place.
	//
	gfs.uiNumFilesInDirectory = iNumFiles;

	gfs.pcFileNames = (STR8)MemAlloc( iNumFiles * FILENAME_LENGTH );

	if ( gfs.pcFileNames )
	{
		for ( i=0 ; i<iNumFiles ; i++ )
		{
			Pop( hStack, (void *)(&gfs.pcFileNames[i*FILENAME_LENGTH]) );
		}
	}

	//
	//	Clean up.
	//

	DeleteStack( hStack );
*/
}





//**************************************************************************
//
// GetFilesInDirectory
//
//		Gets the files in a directory and the subdirectories.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		??nov96:HJH		->creation
//
//**************************************************************************

INT32 GetFilesInDirectory( HCONTAINER hStack, CHAR *pcDir, HANDLE hFile, WIN32_FIND_DATA *pFind )
{
	INT32					iNumFiles;
	WIN32_FIND_DATA	inFind;
	BOOLEAN				fMore;
	CHAR					cName[FILENAME_LENGTH], cDir[FILENAME_LENGTH];
	HANDLE				hFileIn;

	fMore = TRUE;
	iNumFiles = 0;

	while ( fMore )
	{
		if ( pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( strcmp( pFind->cFileName, "." ) != 0 && strcmp( pFind->cFileName, ".." ) != 0 )
			{
				// a valid directory - recurse and find the files in that directory

				inFind.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
				strcpy( cDir, pcDir );
				strcpy( &(cDir[strlen(cDir)-3]), pFind->cFileName );
				strcpy( &(cDir[strlen(cDir)]), "\\*.*\0" );
				hFileIn = FindFirstFile( cDir, &inFind );
				iNumFiles += GetFilesInDirectory( hStack, cDir, hFileIn, &inFind );
				FindClose( hFileIn );
			}
		}
		else
		{
			iNumFiles++;
			strcpy( cName, pcDir );
			strcpy( &(cName[strlen(cName)-3]), pFind->cFileName );
			CharLower( cName );
			hStack = Push( hStack, cName );
		}
		pFind->dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
		fMore = FindNextFile( hFile, pFind );
	}

	return(iNumFiles);
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


BOOLEAN DirectoryExists( STRING512 pcDirectory )
{
	UINT32	uiAttribs;
	DWORD		uiLastError;

	uiAttribs = GetFileAttributes( pcDirectory );

	if ( uiAttribs == 0xFFFFFFFF )
	{
		// an error, make sure it's the right error
		uiLastError =	GetLastError();

		if (uiLastError != ERROR_FILE_NOT_FOUND)
		{
			FastDebugMsg(String("DirectoryExists: ERROR - GetFileAttributes failed, error #%d on file %s", uiLastError, pcDirectory));
		}
	}
	else
	{
		// something's there, make sure it's a directory
		if ( uiAttribs & FILE_ATTRIBUTE_DIRECTORY )
		{
			return TRUE;
		}
	}

	// this could also mean that the name given is that of a file, or that an error occurred
	return FALSE;
}


BOOLEAN MakeFileManDirectory( STRING512 pcDirectory )
{
	return FALSE;
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

void W32toSGPFileFind( GETFILESTRUCT *pGFStruct, WIN32_FIND_DATA *pW32Struct )
{
	UINT32 uiAttribMask;

	// Copy the filename
	strcpy(pGFStruct->zFileName, pW32Struct->cFileName);

	// Get file size
	if ( pW32Struct->nFileSizeHigh != 0 )
		pGFStruct->uiFileSize = 0xffffffff;
	else
		pGFStruct->uiFileSize = pW32Struct->nFileSizeLow;

	// Copy the file attributes
	pGFStruct->uiFileAttribs = 0;

	for( uiAttribMask = 0x80000000; uiAttribMask > 0; uiAttribMask >>= 1)
	{
		switch( pW32Struct->dwFileAttributes & uiAttribMask )
		{
			case FILE_ATTRIBUTE_ARCHIVE:
				pGFStruct->uiFileAttribs |= FILE_IS_ARCHIVE;
				break;

			case FILE_ATTRIBUTE_DIRECTORY:
				pGFStruct->uiFileAttribs |= FILE_IS_DIRECTORY;
				break;
			
			case FILE_ATTRIBUTE_HIDDEN:
				pGFStruct->uiFileAttribs |= FILE_IS_HIDDEN;
				break;
			
			case FILE_ATTRIBUTE_NORMAL:	
				pGFStruct->uiFileAttribs |= FILE_IS_NORMAL;
				break;
			
			case FILE_ATTRIBUTE_READONLY:
				pGFStruct->uiFileAttribs |= FILE_IS_READONLY;
				break;
			
			case FILE_ATTRIBUTE_SYSTEM:
				pGFStruct->uiFileAttribs |= FILE_IS_SYSTEM;
				break;
			
			case FILE_ATTRIBUTE_TEMPORARY:
				pGFStruct->uiFileAttribs |= FILE_IS_TEMPORARY;
				break;

			case FILE_ATTRIBUTE_COMPRESSED:
				pGFStruct->uiFileAttribs |= FILE_IS_COMPRESSED;
				break;

			case FILE_ATTRIBUTE_OFFLINE:
				pGFStruct->uiFileAttribs |= FILE_IS_OFFLINE;
				break;
		}
	}
}



BOOLEAN FileCopy(STR strSrcFile, STR strDstFile, BOOLEAN fFailIfExists)
{
	return(CopyFile(strSrcFile, strDstFile, fFailIfExists));

// Not needed, use Windows CopyFile
/*
	HWFILE hFile;
	UINT32 uiSize;
	CHAR *pBuffer;
	UINT32 uiBytesRead, uiBytesWritten;


	// open source file
	hFile = FileOpen(strSrcFile, FILE_ACCESS_READ, FALSE);
	if (hFile == 0)
	{
		FastDebugMsg(String("FileCopy: FileOpen failed on Src file %s", strSrcFile));
	return(FALSE);
	}

	// get its size
	uiSize = FileGetSize(hFile);
	if (uiSize == 0)
	{
		FastDebugMsg(String("FileCopy: size is 0, Src file %s", strSrcFile));
	FileClose(hFile);
	return(FALSE);
	}

	// allocate a buffer big enough to hold the entire file
	pBuffer = MemAlloc(uiSize);
	if (pBuffer == NULL)
	{
		FastDebugMsg(String("FileCopy: ERROR - MemAlloc pBuffer failed, size %d", uiSize));
	FileClose(hFile);
		return(FALSE);
	}

	// read the file into memory
	if (!FileRead(hFile, pBuffer, uiSize, &uiBytesRead))
	{
		FastDebugMsg(String("FileCopy: FileRead failed, file %s", strSrcFile));
	FileClose(hFile);
	return(FALSE);
	}

	// close source file
	FileClose(hFile);


	// open destination file
	hFile = FileOpen(strDstFile, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE);
	if (hFile == 0)
	{
		FastDebugMsg(String("FileCopy: FileOpen failed on Dst file %s", strDstFile));
	return(FALSE);
	}

	// write buffer to the destination file
	if (!FileWrite(hFile, pBuffer, uiSize, &uiBytesWritten))
	{
		FastDebugMsg(String("FileCopy: FileWrite failed, file %s", strDstFile));
	FileClose(hFile);
	return(FALSE);
	}

	// close destination file
	FileClose(hFile);


	MemFree(pBuffer);
	pBuffer = NULL;
	return(TRUE);
*/
}

BOOLEAN FileMove(STR strOldName, STR strNewName)
{
	// rename
	return(MoveFile(strOldName, strNewName));
}

//Additions by Kris Morness
BOOLEAN FileSetAttributes( STR strFilename, UINT32 uiNewAttribs )
{
	UINT32	uiFileAttrib = 0;

	if( uiNewAttribs & FILE_ATTRIBUTES_ARCHIVE )
		uiFileAttrib |= FILE_ATTRIBUTE_ARCHIVE;

	if( uiNewAttribs & FILE_ATTRIBUTES_HIDDEN )
		uiFileAttrib |= FILE_ATTRIBUTE_HIDDEN;

	if( uiNewAttribs & FILE_ATTRIBUTES_NORMAL )
		uiFileAttrib |= FILE_ATTRIBUTE_NORMAL;

	if( uiNewAttribs & FILE_ATTRIBUTES_OFFLINE )
		uiFileAttrib |= FILE_ATTRIBUTE_OFFLINE;

	if( uiNewAttribs & FILE_ATTRIBUTES_READONLY )
		uiFileAttrib |= FILE_ATTRIBUTE_READONLY;

	if( uiNewAttribs & FILE_ATTRIBUTES_SYSTEM	)
		uiFileAttrib |= FILE_ATTRIBUTE_SYSTEM;

	if( uiNewAttribs & FILE_ATTRIBUTES_TEMPORARY )
		uiFileAttrib |= FILE_ATTRIBUTE_TEMPORARY;

	return SetFileAttributes( strFilename, uiFileAttrib );
}


UINT32 FileGetAttributes( STR strFilename )
{
	UINT32	uiAttribs = 0;
	UINT32	uiFileAttrib = 0;

	uiAttribs = GetFileAttributes( strFilename );

	if( uiAttribs == 0xFFFFFFFF )
		return( uiAttribs );

	if( uiAttribs & FILE_ATTRIBUTE_ARCHIVE )
		uiFileAttrib |= FILE_ATTRIBUTES_ARCHIVE;

	if( uiAttribs & FILE_ATTRIBUTE_HIDDEN )
		uiFileAttrib |= FILE_ATTRIBUTES_HIDDEN;

	if( uiAttribs & FILE_ATTRIBUTE_NORMAL )
		uiFileAttrib |= FILE_ATTRIBUTES_NORMAL;

	if( uiAttribs & FILE_ATTRIBUTE_OFFLINE )
		uiFileAttrib |= FILE_ATTRIBUTES_OFFLINE;

	if( uiAttribs & FILE_ATTRIBUTE_READONLY )
		uiFileAttrib |= FILE_ATTRIBUTES_READONLY;

	if( uiAttribs & FILE_ATTRIBUTE_SYSTEM	)
		uiFileAttrib |= FILE_ATTRIBUTES_SYSTEM;

	if( uiAttribs & FILE_ATTRIBUTE_TEMPORARY )
		uiFileAttrib |= FILE_ATTRIBUTES_TEMPORARY;

	if( uiAttribs & FILE_ATTRIBUTE_DIRECTORY )
		uiFileAttrib |= FILE_ATTRIBUTES_DIRECTORY;

	

	return( uiFileAttrib );
}

BOOLEAN FileClearAttributes( STR strFilename )
{
	return TRUE;
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



BOOLEAN GetFileManFileTime( HWFILE hFile, SGP_FILETIME	*pCreationTime, SGP_FILETIME *pLastAccessedTime, SGP_FILETIME *pLastWriteTime )
{
	return( FALSE );
}


INT32	CompareSGPFileTimes( SGP_FILETIME	*pFirstFileTime, SGP_FILETIME *pSecondFileTime )
{
	return( CompareFileTime( pFirstFileTime, pSecondFileTime ) );
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


//**************************************************************************
//
// AddSubdirectoryToPath
//
//		Puts a subdirectory of the current working directory into the current 
// task's system path. 
//
// Parameter List :
// Return Value :
// Modification history :
//
//		10June98:DB		->creation
//
//**************************************************************************
BOOLEAN AddSubdirectoryToPath(CHAR8 *pDirectory)
{
CHAR8	*pSystemPath;
CHAR8 *pPath;
UINT32 uiPathLen;

	// Check for NULL
	if(!pDirectory)
		return(FALSE);

	// Check for zero length string
	if(!strlen(pDirectory))
		return(FALSE);

	if((pSystemPath=(CHAR8 *)MemAlloc(_MAX_PATH))==NULL)
		return(FALSE);

	memset(pSystemPath, 0, _MAX_PATH);

	if((pPath=(CHAR8 *)MemAlloc(_MAX_PATH))==NULL)
	{
		MemFree(pSystemPath);
		return(FALSE);
	}

	memset(pPath, 0, _MAX_PATH);

	// Builds a path to the directory with the SR DLL files.
	_getcwd(pPath, _MAX_PATH);
	uiPathLen=strlen(pPath);
	if(uiPathLen)
		uiPathLen--;
	if(pPath[uiPathLen]!='\\')
		strcat(pPath, "\\");
	
	strcat(pPath, pDirectory);

	// Appends it to the path for the current task
	if(GetEnvironmentVariable("PATH", pSystemPath, _MAX_PATH))
	{
		strcat(pSystemPath, ";");
		strcat(pSystemPath, pPath);
		SetEnvironmentVariable("PATH", pSystemPath);
		MemFree(pSystemPath);
		MemFree(pPath);
		return(TRUE);
	}
	else
	{
		MemFree(pSystemPath);
		MemFree(pPath);
		return(FALSE);
	}
	
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
