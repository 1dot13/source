#ifndef _LIBRARY_DATABASE_H
#define _LIBRARY_DATABASE_H

#include "Types.h"
#include "windows.h"
#include "FileMan.h"

#define	FILENAME_SIZE									256

//#define	FILENAME_SIZE									40 + PATH_SIZE
#define	PATH_SIZE											80


#define NUM_FILES_TO_ADD_AT_A_TIME		20
#define INITIAL_NUM_HANDLES						20


#define	REAL_FILE_LIBRARY_ID					1022			

#define	DB_BITS_FOR_LIBRARY							10
#define	DB_BITS_FOR_FILE_ID							22

#define	DB_EXTRACT_LIBRARY( exp )				( exp >> DB_BITS_FOR_FILE_ID )
#define	DB_EXTRACT_FILE_ID( exp )				( exp & 0x3FFFFF )

#define DB_ADD_LIBRARY_ID( exp )				( exp << DB_BITS_FOR_FILE_ID )
#define DB_ADD_FILE_ID( exp )						( exp & 0xC00000 )

typedef	UINT32	HWFILE;


typedef struct
{
	CHAR8 sLibraryName[ FILENAME_SIZE ];					// The name of the library file on the disk
	BOOLEAN fOnCDrom;															// A flag specifying if its a cdrom library ( not implemented yet )
	BOOLEAN fInitOnStart;													// Flag specifying if the library is to Initialized at the begining of the game

} LibraryInitHeader;




#ifdef JA2
	#include "Ja2 Libs.h"
#elif UTIL
	#define NUMBER_OF_LIBRARIES 0
	typedef	FILETIME				SGP_FILETIME;
#else	//wizardry
	#include "WizLibs.h"
#endif

extern LibraryInitHeader gGameLibaries[];
extern	CHAR8	gzCdDirectory[ SGPFILENAME_LEN ];


#define		REAL_LIBRARY_FILE	"RealFiles.slf"

typedef struct	
{
	UINT32	uiFileID;						// id of the file ( they start at 1 )
	HANDLE	hRealFileHandle;		// if the file is a Real File, this its handle
} RealFileOpenStruct;


typedef struct
{
	STR			pFileName;
	UINT32	uiFileLength;
	UINT32	uiFileOffset;
} FileHeaderStruct;


typedef struct	
{
	UINT32	uiFileID;											// id of the file ( they start at 1 )
	UINT32	uiFilePosInFile;							// current position in the file
	UINT32	uiActualPositionInLibrary;		// Current File pointer position in actuall library
	FileHeaderStruct *pFileHeader;
} FileOpenStruct;




typedef struct
{
	STR			sLibraryPath;
	HANDLE	hLibraryHandle;
	UINT16	usNumberOfEntries;
	BOOLEAN	fLibraryOpen;
//	BOOLEAN	fAnotherFileAlreadyOpenedLibrary;				//this variable is set when a file is opened from the library and reset when the file is close.	No 2 files can have access to the library at 1 time.
	UINT32	uiIdOfOtherFileAlreadyOpenedLibrary;				//this variable is set when a file is opened from the library and reset when the file is close.	No 2 files can have access to the library at 1 time.
	INT32		iNumFilesOpen;
	INT32		iSizeOfOpenFileArray;
	FileHeaderStruct *pFileHeader;
	FileOpenStruct	*pOpenFiles;

//
//	Temp:	Total memory used for each library ( all memory allocated
//
	#ifdef JA2TESTVERSION
		UINT32	uiTotalMemoryAllocatedForLibrary;
	#endif

} LibraryHeaderStruct;


typedef struct
{
	INT32		iNumFilesOpen;
	INT32		iSizeOfOpenFileArray;
	RealFileOpenStruct *pRealFilesOpen;

} RealFileHeaderStruct;


typedef struct
{
	STR			sManagerName;
	LibraryHeaderStruct	*pLibraries;
	UINT16	usNumberOfLibraries;
	BOOLEAN	fInitialized;
	RealFileHeaderStruct RealFiles;
} DatabaseManagerHeaderStruct;





//typedef UINT32	HLIBFILE;



//*************************************************************************
//
//	NOTE!	The following structs are also used by the datalib98 utility
//
//*************************************************************************


#define FILE_OK							0
#define FILE_DELETED				0xff
#define FILE_OLD						1
#define FILE_DOESNT_EXIST		0xfe


typedef struct 
{
	CHAR8		sLibName[ FILENAME_SIZE ];
	CHAR8		sPathToLibrary[ FILENAME_SIZE ];
	INT32		iEntries;
	INT32		iUsed;
	UINT16	iSort;
	UINT16	iVersion;
	BOOLEAN	fContainsSubDirectories;
	INT32		iReserved;
} LIBHEADER;


typedef struct 
{
	CHAR8		sFileName[ FILENAME_SIZE ];
	UINT32	uiOffset;
	UINT32	uiLength;
	UINT8		ubState;
	UINT8		ubReserved;
	FILETIME	sFileTime;
	UINT16	usReserved2;
} DIRENTRY;



#ifdef __cplusplus
extern "C" {
#endif


//The FileDatabaseHeader
extern DatabaseManagerHeaderStruct gFileDataBase;



//Function Prototypes

BOOLEAN CheckForLibraryExistence( STR pLibraryName );
BOOLEAN InitializeLibrary( STR pLibraryName, LibraryHeaderStruct *pLibheader, BOOLEAN fCanBeOnCDrom );

BOOLEAN InitializeFileDatabase( );
BOOLEAN ReopenCDLibraries(void);
BOOLEAN ShutDownFileDatabase( );
BOOLEAN CheckIfFileExistInLibrary( STR pFileName );
INT16 GetLibraryIDFromFileName( STR pFileName );
HWFILE OpenFileFromLibrary( STR pName );
HWFILE CreateRealFileHandle( HANDLE hFile );
BOOLEAN CloseLibraryFile( INT16 sLibraryID, UINT32 uiFileID );
BOOLEAN GetLibraryAndFileIDFromLibraryFileHandle( HWFILE hlibFile, INT16 *pLibraryID, UINT32 *pFileNum );
BOOLEAN LoadDataFromLibrary( INT16 sLibraryID, UINT32 uiFileIndex, PTR pData, UINT32 uiBytesToRead, UINT32 *pBytesRead );
BOOLEAN LibraryFileSeek( INT16 sLibraryID, UINT32 uiFileNum, UINT32 uiDistance, UINT8 uiHowToSeek );


//used to open and close libraries during the game
BOOLEAN CloseLibrary( INT16 sLibraryID );
BOOLEAN OpenLibrary( INT16 sLibraryID );

BOOLEAN IsLibraryOpened( INT16 sLibraryID );

BOOLEAN GetLibraryFileTime( INT16 sLibraryID, UINT32 uiFileNum, SGP_FILETIME	*pLastWriteTime );


#ifdef __cplusplus
}
#endif



#endif
