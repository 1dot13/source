//**************************************************************************
//
// Filename :	FileMan.h
//
//	Purpose :	prototypes for the file manager
//
// Modification history :
//
//		24sep96:HJH				- Creation
//
//**************************************************************************

#ifndef _FILEMAN_H
#define _FILEMAN_H

//**************************************************************************
//
//				Includes
//
//**************************************************************************

#include "types.h"

#include "Windows.h"

#include "FileCat.h"

#include "Container.h"

//**************************************************************************
//
//				Defines
//
//**************************************************************************

#define MAX_FILENAME_LEN		48

#define FILE_ACCESS_READ		0x01
#define FILE_ACCESS_WRITE		0x02
#define FILE_ACCESS_READWRITE		0x03

#define FILE_CREATE_NEW				0x0010	// create new file. fail if exists
#define FILE_CREATE_ALWAYS			0x0020	// create new file. overwrite existing
#define FILE_OPEN_EXISTING			0x0040	// open a file. fail if doesn't exist
#define FILE_OPEN_ALWAYS			0x0080	// open a file, create if doesn't exist
#define FILE_TRUNCATE_EXISTING	0x0100	// open a file, truncate to size 0. fail if no exist

#define FILE_SEEK_FROM_START		0x01	// keep in sync with dbman.h
#define FILE_SEEK_FROM_END			0x02	// keep in sync with dbman.h
#define FILE_SEEK_FROM_CURRENT	0x04	// keep in sync with dbman.h

// GetFile file attributes
#define FILE_IS_READONLY				1
#define FILE_IS_DIRECTORY				2
#define FILE_IS_HIDDEN					4
#define FILE_IS_NORMAL					8
#define FILE_IS_ARCHIVE					16
#define FILE_IS_SYSTEM					32
#define FILE_IS_TEMPORARY				64
#define FILE_IS_COMPRESSED			128
#define FILE_IS_OFFLINE					256



//File Attributes settings
#define FILE_ATTRIBUTES_ARCHIVE				FILE_ATTRIBUTE_ARCHIVE
#define FILE_ATTRIBUTES_HIDDEN				FILE_ATTRIBUTE_HIDDEN
#define FILE_ATTRIBUTES_NORMAL				FILE_ATTRIBUTE_NORMAL
#define FILE_ATTRIBUTES_OFFLINE				FILE_ATTRIBUTE_OFFLINE
#define FILE_ATTRIBUTES_READONLY			FILE_ATTRIBUTE_READONLY
#define FILE_ATTRIBUTES_SYSTEM				FILE_ATTRIBUTE_SYSTEM
#define FILE_ATTRIBUTES_TEMPORARY			FILE_ATTRIBUTE_TEMPORARY
#define FILE_ATTRIBUTES_DIRECTORY			FILE_ATTRIBUTE_DIRECTORY

// Snap, Kaiden: This define duplicates a standard MFC define
// Added to resolve some intractable issue with MSVC6
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)

typedef	FILETIME				SGP_FILETIME;

//**************************************************************************
//
// Globals
//
//**************************************************************************

// Snap: At program launch we build two directory catalogues:
// one for the default Data directory, the other for the custom Data directory.
extern TFileCat gDefaultDataCat;	// Init in InitializeStandardGamingPlatform (sgp.cpp)
extern TFileCat gCustomDataCat;	// Init in InitializeStandardGamingPlatform (sgp.cpp)

//**************************************************************************
//
//				Function Prototypes
//
//**************************************************************************
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

extern BOOLEAN	InitializeFileManager(	STR strIndexFilename );

extern void		ShutdownFileManager( void );
extern void		FileDebug( BOOLEAN f );


BOOLEAN	FileExists( STR strFilename );
extern BOOLEAN	FileExistsNoDB( STR strFilename );
extern BOOLEAN	FileDelete( STR strFilename );
extern HWFILE	FileOpen( STR strFilename, UINT32 uiOptions, BOOLEAN fDeleteOnClose=FALSE, STR strProfilename=NULL );//dnl ch81 021213

extern void		FileClose( HWFILE );

extern BOOLEAN	FileRead( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead );
extern BOOLEAN	FileReadLine( HWFILE hFile, std::string* pDest );
extern BOOLEAN	FileReadLine( HWFILE hFile, STR8 pDest, UINT32 uiDestSize, UINT32 *puiBytesRead );
extern BOOLEAN	FileWrite( HWFILE hFile, PTR pDest, UINT32 uiBytesToWrite, UINT32 *puiBytesWritten );
extern BOOLEAN	FileLoad( STR filename, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead );

extern BOOLEAN _cdecl FilePrintf( HWFILE hFile, STR8	strFormatted, ... );

extern BOOLEAN	FileSeek( HWFILE, UINT32 uiDistance, UINT8 uiHow );
extern INT32	FileGetPos( HWFILE );

extern UINT32	FileGetSize( HWFILE );
extern UINT32 FileSize(STR strFilename);

BOOLEAN SetFileManCurrentDirectory( STR pcDirectory );
BOOLEAN GetFileManCurrentDirectory( STRING512 pcDirectory );
BOOLEAN GetExecutableDirectory( STRING512 pcDirectory );

BOOLEAN DirectoryExists( STRING512 pcDirectory );
BOOLEAN MakeFileManDirectory( STRING512 pcDirectory );

// WARNING: THESE DELETE ALL FILES IN THE DIRECTORY ( and all subdirectories if fRecursive is TRUE!! )
BOOLEAN RemoveFileManDirectory( STRING512 pcDirectory, BOOLEAN fRecursive);
BOOLEAN EraseDirectory( STRING512 pcDirectory);

typedef struct _GETFILESTRUCT_TAG {
	INT32 iFindHandle;
	CHAR8 zFileName[ 260 ];			// changed from UINT16, Alex Meduna, Mar-20'98
	UINT32 uiFileSize;
	UINT32 uiFileAttribs;
} GETFILESTRUCT;

BOOLEAN GetFileFirst( CHAR8 * pSpec, GETFILESTRUCT *pGFStruct );

BOOLEAN GetFileNext( GETFILESTRUCT *pGFStruct );
void GetFileClose( GETFILESTRUCT *pGFStruct );

BOOLEAN FileCopy(STR strSrcFile, STR strDstFile, BOOLEAN fFailIfExists);
BOOLEAN FileMove(STR strOldName, STR strNewName);

//Added by Kris Morness
BOOLEAN FileSetAttributes( STR filename, UINT32 uiNewAttribs );
UINT32	FileGetAttributes( STR filename );

BOOLEAN FileClearAttributes( STR strFilename );

//returns true if at end of file, else false
BOOLEAN	FileCheckEndOfFile( HWFILE hFile );



BOOLEAN GetFileManFileTime( HWFILE hFile, SGP_FILETIME	*pCreationTime, SGP_FILETIME *pLastAccessedTime, SGP_FILETIME *pLastWriteTime );

INT32 GetFilesInDirectory( HCONTAINER hStack, CHAR *pcDir, HANDLE hFile, WIN32_FIND_DATA *pFind );



// CompareSGPFileTimes() returns...
// -1 if the First file time is less than second file time. ( first file is older )
// 0 First file time is equal to second file time.
// +1 First file time is greater than second file time ( first file is newer ).
INT32	CompareSGPFileTimes( SGP_FILETIME	*pFirstFileTime, SGP_FILETIME *pSecondFileTime );

// One call comparison of file times, allowing for a certain leeway in cases where
// files times may be slightly different due to SourceSafe of copying
BOOLEAN FileIsOlderThanFile(CHAR8 *pcFileName1, CHAR8 *pcFileName2, UINT32 ulNumSeconds);

//	Pass in the Fileman file handle of an OPEN file and it will return..
//		if its a Real File, the return will be the handle of the REAL file
//		if its a LIBRARY file, the return will be the handle of the LIBRARY
HANDLE	GetRealFileHandleFromFileManFileHandle( HWFILE hFile );

BOOLEAN AddSubdirectoryToPath(CHAR8 *pDirectory);


//Gets the amount of free space on the hard drive that the main executeablt is runnning from
UINT32		GetFreeSpaceOnHardDriveWhereGameIsRunningFrom( );

//Gets the free hard drive space from the drive letter passed in.	It has to be the root dir.	( eg. c:\ )
UINT32		GetFreeSpaceOnHardDrive( STR pzDriveLetter );

// Flugente: simple wrapper to check whether an audio file in mp3/ogg/wav format exists
BOOLEAN	SoundFileExists( STR strFilename, STR zFoundFilename );

/*
#ifdef __cplusplus
}
#endif
*/

#endif
