#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include <stdio.h>
	#include <string.h>
	#include <wchar.h>
	#include "worlddat.h"
	#include "worlddef.h"
	#include "tiledef.h"
	#include "sys globals.h"
	#include "tile surface.h"
	#include "fileMan.h"
#endif

#include "VFS/vfs.h"
#include "XMLWriter.h"
// THIS FILE CONTAINS DEFINITIONS FOR TILESET FILES

void SetTilesetThreeTerrainValues();
void SetTilesetFourTerrainValues();

// Snap: This global gives the number of tilesets in JA2SET.DAT
// It is properly initialized in InitEngineTilesets,
// where it is read from JA2SET.DAT header.
UINT8 gubNumSets = MAX_TILESETS;

TILESET	gTilesets[ MAX_TILESETS ];


void InitEngineTilesets( )
{
	UINT32		cnt, cnt2, uiNumFiles;
//	FILE					*hfile;
	HWFILE			hfile;
	CHAR8			zName[32];
	UINT32		uiNumBytesRead;
	XMLWriter xmlw;

	//OPEN FILE
//	hfile = fopen( "BINARYDATA\\JA2SET.DAT", "rb" );
	hfile = FileOpen( "BINARYDATA\\JA2SET.DAT", FILE_ACCESS_READ, FALSE );
	if ( !hfile	)
	{
		SET_ERROR( "Cannot open tileset data file" );
		return;
	}

	xmlw.openNode("JA2SET");

	// READ # TILESETS and compare
//	fread( &gubNumSets, sizeof( gubNumSets ), 1, hfile );
	FileRead( hfile, &gubNumSets, sizeof( gubNumSets ), &uiNumBytesRead );
	// CHECK
	if ( gubNumSets > MAX_TILESETS )
	{
		// Report error
		SET_ERROR( "Too many tilesets in the data file" );
		return;
	}
	xmlw.addAttributeToNextValue("numTilesets",(int)gubNumSets);

	// READ #files
//	fread( &uiNumFiles, sizeof( uiNumFiles ), 1, hfile );
	FileRead( hfile, &uiNumFiles, sizeof( uiNumFiles ), &uiNumBytesRead );

	// COMPARE
	if ( uiNumFiles != NUMBEROFTILETYPES )
	{
		// Report error
		SET_ERROR( "Number of tilesets slots in code does not match data file" );
		return;
	}
	xmlw.addAttributeToNextValue("numFiles",(int)uiNumFiles);
	xmlw.openNode("tilesets");
	// Loop through each tileset, load name then files
	for ( cnt = 0; cnt < gubNumSets; cnt++ )
	{
		xmlw.addAttributeToNextValue("index",(int)cnt);
		xmlw.openNode("Tileset");
		//Read name
//		fread( &zName, sizeof( zName ), 1, hfile );
		FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );
		xmlw.addValue("Name",std::string(zName));

		// Read ambience value
//		fread( &(gTilesets[ cnt ].ubAmbientID), sizeof( UINT8), 1, hfile );
		FileRead( hfile, &(gTilesets[ cnt ].ubAmbientID), sizeof( UINT8 ), &uiNumBytesRead );
		xmlw.addValue("AmbientID",(int)gTilesets[ cnt ].ubAmbientID);

		// Set into tileset
		swprintf( gTilesets[ cnt ].zName, L"%S", zName );

		xmlw.openNode("Files");
		// Loop for files
		for ( cnt2 = 0; cnt2 < uiNumFiles; cnt2++ )
		{
			// Read file name
//			fread( &zName, sizeof( zName ), 1, hfile );
			FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );
			if(!std::string(zName).empty())
			{
				xmlw.addAttributeToNextValue("index",(int)cnt2);
				xmlw.addValue("file",std::string(zName));
			}

			// Set into database
			strcpy( gTilesets[ cnt ].TileSurfaceFilenames[ cnt2 ], zName );

		}
		xmlw.closeNode(); // Files
		xmlw.closeNode(); // tileset
	}
	xmlw.closeNode();

//	fclose( hfile );
	FileClose( hfile );

	xmlw.closeNode();
#ifdef USE_VFS
	xmlw.writeToFile("Ja2Set.dat.xml");
#endif
	// SET CALLBACK FUNTIONS!!!!!!!!!!!!!
	gTilesets[ TLS_CAVES_1 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetTwoTerrainValues;
	gTilesets[ TLS_AIRSTRIP ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_DEAD_AIRSTRIP ].MovementCostFnc	= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_PALACE ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_BALIME ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_BALIME_MUSEUM ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_TROPICAL ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_MEDUNA_INNER_TOWN ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_SAM ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_AIRPORT ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;

	gTilesets[ TLS_TROPICAL_1 ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
	gTilesets[ TLS_DESERT_SAM ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
}




void SetTilesetOneTerrainValues( )
{
		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = DIRT_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = DIRT_ROAD;

		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_FLOOR;

		// NOW ANY TERRAIN MODIFYING DEBRIS

}


void SetTilesetTwoTerrainValues( )
{
		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = DIRT_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = DIRT_ROAD;


		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_GROUND;

}


void SetTilesetThreeTerrainValues( )
{
		// DIFFERENCE FROM #1 IS THAT ROADS ARE PAVED

		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = PAVED_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = PAVED_ROAD;


		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_FLOOR;

		// NOW ANY TERRAIN MODIFYING DEBRIS

}

void SetTilesetFourTerrainValues( )
{
		// DIFFERENCE FROM #1 IS THAT FLOOR2 IS NOT FLAT_FLOOR BUT FLAT_GROUND

		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = DIRT_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = DIRT_ROAD;


		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_FLOOR;

		// NOW ANY TERRAIN MODIFYING DEBRIS

}
