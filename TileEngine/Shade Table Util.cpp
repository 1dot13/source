#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include <stdio.h>
	#include "types.h"
	#include "lighting.h"
	#include "Shade Table Util.h"
	#include "video.h"
	#include "WorldDat.h"
	#include "Fileman.h"
#endif

#include <vfs/Core/vfs.h>

#define		SHADE_TABLE_DIR				"ShadeTables"



extern CHAR8 TileSurfaceFilenames[NUMBEROFTILETYPES][32]; // symbol already declared globally in worlddef.cpp (jonathanl)
BOOLEAN gfForceBuildShadeTables = FALSE;

#ifdef JA2TESTVERSION
	extern UINT32 uiNumTablesSaved;
	extern UINT32 uiNumTablesLoaded;
#endif

void DetermineRGBDistributionSettings()
{
#ifndef USE_VFS
	STRING512			DataDir;
	STRING512			ExecDir;
#endif
	STRING512			ShadeTableDir;
	UINT32				uiRBitMask, uiGBitMask, uiBBitMask;
	UINT32				uiPrevRBitMask, uiPrevGBitMask, uiPrevBBitMask;
	UINT32				uiNumBytesRead;
	HWFILE				hfile;
	BOOLEAN				fSaveRGBDist = FALSE;
	BOOLEAN				fCleanShadeTable = FALSE;
	BOOLEAN				fLoadedPrevRGBDist = FALSE;

#ifndef USE_VFS
	// Snap: save current directory
	GetFileManCurrentDirectory( DataDir );

	//First, determine if we have a file saved.	If not, then this is the first time, and
	//all shade tables will have to be built and saved to disk.	This can be time consuming, adding up to
	//3-4 seconds to the time of a map load.
	GetExecutableDirectory( ExecDir );
	sprintf( ShadeTableDir, "%s\\%s", DataDir, SHADE_TABLE_DIR );

	//Check to make sure we have a ShadeTable directory.	If we don't create one!
	if( !SetFileManCurrentDirectory( ShadeTableDir ) )
	{
		if( !MakeFileManDirectory( ShadeTableDir ) )
		{
			AssertMsg( 0, "ShadeTable directory doesn't exist, and couldn't create one!" );
		}
		if( !SetFileManCurrentDirectory( ShadeTableDir ) )
		{
			AssertMsg( 0, "Couldn't access the newly created ShadeTable directory." );
		}
		fSaveRGBDist = TRUE;
	}
	// directory existed or was created.
	SetFileManCurrentDirectory( DataDir );
#else
	// changing the current directory can screw up the VFS; don't do it
	// the creation of the SHADE_TABLE_DIR directory is done by the VFS
	sprintf( ShadeTableDir, "%s", SHADE_TABLE_DIR );
#endif

	CHAR8 sRGBDist[50];
	sprintf( sRGBDist, "%s\\%s", SHADE_TABLE_DIR, "RGBDist.dat");
	CHAR8 sResetShadeTables[50];
	sprintf( sResetShadeTables, "%s\\%s", SHADE_TABLE_DIR, "ResetShadeTables.txt");

	if( !fSaveRGBDist )
	{ //Load the previous RGBDist and determine if it is the same one
		if( !FileExists( sRGBDist ) || FileExists( sResetShadeTables ) )
		{ //Can't find the RGBDist.dat file.	The directory exists, but the file doesn't, which
			//means the user deleted the file manually.	Now, set it up to create a new one.
			fSaveRGBDist = TRUE;
			fCleanShadeTable = TRUE;
		}
		else
		{
			hfile = FileOpen( sRGBDist, FILE_ACCESS_READ, FALSE );
			if( !hfile )
			{
				AssertMsg( 0, "Couldn't open RGBDist.dat, even though it exists!" );
			}
			FileRead( hfile, &uiPrevRBitMask, sizeof( UINT32 ), &uiNumBytesRead );
			FileRead( hfile, &uiPrevGBitMask, sizeof( UINT32 ), &uiNumBytesRead );
			FileRead( hfile, &uiPrevBBitMask, sizeof( UINT32 ), &uiNumBytesRead );
			fLoadedPrevRGBDist = TRUE;
			FileClose( hfile );
		}
	}

	if( !GetPrimaryRGBDistributionMasks( &uiRBitMask, &uiGBitMask, &uiBBitMask ) )
	{
		AssertMsg( 0, "Failed to extract the current RGB distribution masks." );
	}
	if( fLoadedPrevRGBDist )
	{
		if( uiRBitMask != uiPrevRBitMask || uiGBitMask != uiPrevGBitMask || uiBBitMask != uiPrevBBitMask )
		{ //The user has changed modes since the last time he has played JA2.	This essentially can only happen if:
			//1)	The video card has been changed since the last run of JA2.
			//2)	Certain video cards have different RGB distributions in different operating systems such as
			//		the Millenium card using Windows NT or Windows 95
			//3)	The user has physically modified the RGBDist.dat file.
			fSaveRGBDist = TRUE;
			fCleanShadeTable = TRUE;
		}
	}
	if( fCleanShadeTable )
	{ //This means that we are going to remove all of the current shade tables, if any exist, and
		//start fresh.
		EraseDirectory( ShadeTableDir );
	}
	if( fSaveRGBDist )
	{ //The RGB distribution is going to be saved in a tiny file for future reference.	As long as the
		//RGB distribution never changes, the shade table will grow until eventually, all tilesets are loaded,
		//shadetables generated and saved in this directory.
		hfile = FileOpen( sRGBDist, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
		if( !hfile )
		{
			AssertMsg( 0, "Couldn't create RGBDist.dat for writing!" );
		}
		FileWrite( hfile, &uiRBitMask, sizeof( UINT32 ), &uiNumBytesRead );
		FileWrite( hfile, &uiGBitMask, sizeof( UINT32 ), &uiNumBytesRead );
		FileWrite( hfile, &uiBBitMask, sizeof( UINT32 ), &uiNumBytesRead );
		FileClose( hfile );
	}

	// Snap: Restore the data directory once we are finished.

	// BF : current directory was reset above
	//SetFileManCurrentDirectory( DataDir );
	
	//We're done, so restore the executable directory to JA2\Data.
	//sprintf( DataDir, "%s\\Data", ExecDir );
	//SetFileManCurrentDirectory( DataDir );
}

BOOLEAN LoadShadeTable( HVOBJECT pObj, UINT32 uiTileTypeIndex )
{
	HWFILE hfile;
	INT32 i;
	UINT32 uiNumBytesRead;
	CHAR8 ShadeFileName[ 100 ];
	CHAR8 *ptr;
	//ASSUMPTIONS:
	//We are assuming that the uiTileTypeIndex is referring to the correct file
	//stored in the TileSurfaceFilenames[].	If it isn't, then that is a huge problem
	//and should be fixed.	Also assumes that the directory is set to Data\ShadeTables.
	
	//BF
	//strcpy( ShadeFileName, TileSurfaceFilenames[ uiTileTypeIndex ] );
	sprintf( ShadeFileName, "%s\\%s", SHADE_TABLE_DIR, TileSurfaceFilenames[ uiTileTypeIndex ] );
	
	ptr = strstr( ShadeFileName, "." );
	if( !ptr )
	{
		return FALSE;
	}
	ptr++;
	sprintf( ptr, "sha" );

	//hfile = FileOpen( ShadeFileName, FILE_ACCESS_READ, FALSE );
	//if( !hfile )
	//{ //File doesn't exist, so generate it
	//	FileClose( hfile );
	if(!FileExists(ShadeFileName))
	{
		return FALSE;
	}
	hfile = FileOpen( ShadeFileName, FILE_ACCESS_READ, FALSE );

	//MISSING:	Compare time stamps.

	for( i = 0; i < 16; i++ )
	{
		pObj->pShades[ i ] = (UINT16 *) MemAlloc( 512 );
		Assert( pObj->pShades[ i ] );
		FileRead( hfile, pObj->pShades[ i ], 512, &uiNumBytesRead );
	}

	//The file exists, now make sure the
	FileClose( hfile );
	#ifdef JA2TESTVERSION
		uiNumTablesLoaded++;
	#endif
	return TRUE;
}

BOOLEAN SaveShadeTable( HVOBJECT pObj, UINT32 uiTileTypeIndex )
{
	HWFILE hfile;
	INT32 i;
	UINT32 uiNumBytesWritten;
	CHAR8 ShadeFileName[ 100 ];
	CHAR8 *ptr;
#ifdef JA2TESTVERSION
	uiNumTablesSaved++;
#endif
	//ASSUMPTIONS:
	//We are assuming that the uiTileTypeIndex is referring to the correct file
	//stored in the TileSurfaceFilenames[].	If it isn't, then that is a huge problem
	//and should be fixed.	Also assumes that the directory is set to Data\ShadeTables.
	
	// BF
	//strcpy( ShadeFileName, TileSurfaceFilenames[ uiTileTypeIndex ] );
	sprintf_s( ShadeFileName, "%s\\%s", SHADE_TABLE_DIR, TileSurfaceFilenames[ uiTileTypeIndex ] );

	ptr = strstr( ShadeFileName, "." );
	if( !ptr )
	{
		return FALSE;
	}
	ptr++;
	sprintf( ptr, "sha" );

	hfile = FileOpen( ShadeFileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if( !hfile )
	{
		FileClose( hfile );
		AssertMsg( 0, String( "Can't create %s", ShadeFileName ) );
		return FALSE;
	}
	for( i = 0; i < 16; i++ )
	{
		FileWrite( hfile, pObj->pShades[ i ], 512, &uiNumBytesWritten );
	}

	FileClose( hfile );
	return TRUE;
}



BOOLEAN DeleteShadeTableDir( )
{
	return( RemoveFileManDirectory( SHADE_TABLE_DIR, TRUE ) );
}
