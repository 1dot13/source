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
	#include "Debug.h"
#endif

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>
#include "XML_TileSet.hpp"
#include "XMLWriter.h"
#include "GameSettings.h"

void ExportTilesets(vfs::Path const& filename);

// THIS FILE CONTAINS DEFINITIONS FOR TILESET FILES

void SetTilesetThreeTerrainValues();
void SetTilesetFourTerrainValues();

// Snap: This global gives the number of tilesets in JA2SET.DAT
// It is properly initialized in InitEngineTilesets,
// where it is read from JA2SET.DAT header.
UINT8 gubNumSets = MAX_TILESETS;

TILESET	gTilesets[ MAX_TILESETS ];

//extern bool g_bUseXML_Tilesets;
void InitEngineTilesets( )
{
	if(gGameExternalOptions.fUseXmlTileSets)
	{
		const vfs::Path tileset_filename(L"Ja2Set.dat.xml");
		if(!getVFS()->fileExists(tileset_filename))
		{
			SGP_TRYCATCH_RETHROW( ExportTilesets(tileset_filename), L"Could not export tileset XML file");
		}
		vfs::tReadableFile* file = getVFS()->getReadFile(tileset_filename);
		SGP_THROW_IFFALSE(file, 
			_BS(L"File '") << tileset_filename << L"' does not exist and could not be created" << _BS::wget);

		CTilesetReader tileset_reader(gTilesets);
		xml_auto::TGenericXMLParser<CTilesetReader> pars(&tileset_reader,NULL);
		
		SGP_TRYCATCH_RETHROW( pars.parseFile(file),
			_BS(L"Parser Error in file : ") << file->getPath() << _BS::wget );
	}
	else
	{
		UINT32		cnt, cnt2, uiNumFiles;
		//FILE		*hfile;
		HWFILE		hfile;
		CHAR8		zName[32];
		UINT32		uiNumBytesRead;

		//OPEN FILE
		hfile = FileOpen( "BINARYDATA\\JA2SET.DAT", FILE_ACCESS_READ, FALSE );
		if ( !hfile	)
		{
			SET_ERROR( "Cannot open tileset data file" );
			return;
		}

		// READ # TILESETS and compare
		FileRead( hfile, &gubNumSets, sizeof( gubNumSets ), &uiNumBytesRead );
		// CHECK
		if ( gubNumSets > MAX_TILESETS )
		{
			// Report error
			SET_ERROR( "Too many tilesets in the data file" );
			return;
		}

		// READ #files
		FileRead( hfile, &uiNumFiles, sizeof( uiNumFiles ), &uiNumBytesRead );

		// COMPARE
		if ( uiNumFiles != giNumberOfTileTypes )
		{
			// Report error
			SET_ERROR( "Number of tilesets slots in code does not match data file" );
			return;
		}

		// Loop through each tileset, load name then files
		for ( cnt = 0; cnt < gubNumSets; cnt++ )
		{
			//Read name
			FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );

			// Read ambience value
			FileRead( hfile, &(gTilesets[ cnt ].ubAmbientID), sizeof( UINT8 ), &uiNumBytesRead );

			// Set into tileset
			swprintf( gTilesets[ cnt ].zName, L"%S", zName );

			// Loop for files
			for ( cnt2 = 0; cnt2 < uiNumFiles; cnt2++ )
			{
				// Read file name
				FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );

				// Set into database
				strcpy( gTilesets[ cnt ].TileSurfaceFilenames[ cnt2 ], zName );
			}
		}

		FileClose( hfile );
	}

	#ifdef JA2UBMAPS
	gTilesets[ TLS_CAVES_1 ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetTwoTerrainValues;
	gTilesets[ TLS_AIRSTRIP ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_DEAD_AIRSTRIP ].MovementCostFnc		= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_14 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_18 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_19 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_26 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_27 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_28 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TEMP_29 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_TROPICAL_1 ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
	gTilesets[ TEMP_20 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
	#else
	// SET CALLBACK FUNTIONS!!!!!!!!!!!!!
	gTilesets[ TLS_CAVES_1 ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetTwoTerrainValues;
	gTilesets[ TLS_AIRSTRIP ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_DEAD_AIRSTRIP ].MovementCostFnc		= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_PALACE ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_BALIME ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_BALIME_MUSEUM ].MovementCostFnc		= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_TROPICAL ].MovementCostFnc	= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_MEDUNA_INNER_TOWN ].MovementCostFnc	= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_SAM ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_AIRPORT ].MovementCostFnc		= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;

	gTilesets[ TLS_TROPICAL_1 ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
	gTilesets[ TLS_DESERT_SAM ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
	#endif
}

#ifdef USE_VFS
void ExportTilesets(vfs::Path const& filename)
{
	UINT32	uiNumBytesRead = 0;
	CHAR8	zName[32];

	//OPEN FILE
	HWFILE hfile = FileOpen( "BINARYDATA\\JA2SET.DAT", FILE_ACCESS_READ, FALSE );
	SGP_THROW_IFFALSE(hfile, L"Cannot open tileset data file" );

	XMLWriter xmlw;
	xmlw.openNode("JA2SET");

	// READ # TILESETS and compare
	UINT8 numSets = 0;
	FileRead( hfile, &numSets, sizeof( numSets ), &uiNumBytesRead );
	
	// CHECK
	SGP_THROW_IFFALSE( numSets <= MAX_TILESETS, L"Too many tilesets in the data file" );
	xmlw.addAttributeToNextValue("numTilesets",(int)numSets);

	// READ #files
	UINT32 numFiles;
	FileRead( hfile, &numFiles, sizeof( numFiles ), &uiNumBytesRead );

	// COMPARE
	SGP_THROW_IFFALSE( numFiles == giNumberOfTileTypes,
		L"Number of tilesets slots in code does not match data file" );

	xmlw.addAttributeToNextValue("numFiles",(int)numFiles);
	xmlw.openNode("tilesets");
	// Loop through each tileset, load name then files
	for ( UINT8 cnt = 0; cnt < numSets; cnt++ )
	{
		xmlw.addAttributeToNextValue("index",(int)cnt);
		xmlw.openNode("Tileset");

		//Read name
		FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );
		xmlw.addValue("Name",std::string(zName));

		// Read ambience value
		UINT8 ambientID = 0;
		FileRead( hfile, &ambientID, sizeof( UINT8 ), &uiNumBytesRead );
		xmlw.addValue("AmbientID",(int)ambientID);

		xmlw.openNode("Files");
		// Loop for files
		for ( UINT32 cnt2 = 0; cnt2 < numFiles; cnt2++ )
		{
			// Read file name
			FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );
			if(!std::string(zName).empty())
			{
				xmlw.addAttributeToNextValue("index",(int)cnt2);
				xmlw.addValue("file",std::string(zName));
			}

			// Set into database
			strcpy( gTilesets[ cnt ].TileSurfaceFilenames[ cnt2 ], zName );
//ddd{ добавляем в индекс 250 файл окоп.
//		if((cnt==0)&&(cnt2+1 == uiNumFiles))
//		{ strcpy( gTilesets[ cnt ].TileSurfaceFilenames[ 151 ], "okop.sti" );
//				xmlw.AddAttributeToNextValue("index",(int)151); //for exp in xml
//				xmlw.AddValue("file",std::string("okop.sti"));
//		}
//ddd}
			
		}
		xmlw.closeNode(); // Files
		xmlw.closeNode(); // tileset
	}
	xmlw.closeNode();

	FileClose( hfile );

	xmlw.closeNode();
	xmlw.writeToFile(filename);
}
#endif

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
