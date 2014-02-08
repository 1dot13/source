#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
	#include "PreBattle Interface.h"
#else
	#include "worlddef.h"
	#include "worlddat.h"
	#include <stdio.h>
	#include <string.h>
	#include "wcheck.h"
	#include "stdlib.h"
	#include "time.h"
	#include "video.h"
	#include "debug.h"
	#include "worldman.h"
	#include "mousesystem.h"
	#include "sys globals.h"
	#include "screenids.h"
	#include "Render Fun.h"
	#include "font control.h"
	#include "lighting.h"
	#include "structure.h"
	#include "vobject.h"
	#include "Soldier Control.h"
	#include "isometric utils.h"
	#include "Interactive Tiles.h"
	#include "utilities.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "Handle UI.h"
	#include "opplist.h"
	#include "shading.h"
	#include "Animation Control.h"
	#include "World Items.h"
	#include "renderworld.h"
	#include "Radar Screen.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "Exit Grids.h"
	#include "tile surface.h"
	#include "rotting corpses.h"
	#include "Keys.h"
	#include "Map Information.h"
	#include "Exit Grids.h"
	#include "Summary Info.h"
	#include "Animated ProgressBar.h"
	#include "pathai.h"
	#include "EditorBuildings.h"
	#include "FileMan.h"
	#include "Map Edgepoints.h"
	#include "environment.h"
	#include "Shade Table Util.h"
	#include "Structure Wrap.h"
	#include "Scheduling.h"
	#include "EditorMapInfo.h"
	#include "pits.h"
	#include "Game Clock.h"
	#include "Buildings.h"
	#include "strategicmap.h"
	#include "overhead map.h"
	#include "SmokeEffects.h"
	#include "LightEffects.h"
	#include "meanwhile.h"
	#include "LoadScreen.h"//dnl ch30 150909
	#include "Interface Cursors.h"
	#include "Simple Render Utils.h"//dnl ch54 111009
///ddd
	#include "gamesettings.h"
	#include "editscreen.h"
	#include "Button Defines.h"
	#include "Editor Taskbar Utils.h"
#endif

#define	SET_MOVEMENTCOST( a, b, c, d )				( ( gubWorldMovementCosts[ a ][ b ][ c ] < d ) ? ( gubWorldMovementCosts[ a ][ b ][ c ] = d ) : 0 );
#define	FORCE_SET_MOVEMENTCOST( a, b, c, d )	( gubWorldMovementCosts[ a ][ b ][ c ] = d )
#define  SET_CURRMOVEMENTCOST( a, b )			SET_MOVEMENTCOST( usGridNo, a, 0, b ) 

#define	TEMP_FILE_FOR_TILESET_CHANGE				"jatileS34.dat"

#define	MAP_FULLSOLDIER_SAVED				0x00000001
#define	MAP_WORLDONLY_SAVED					0x00000002
#define	MAP_WORLDLIGHTS_SAVED				0x00000004
#define	MAP_WORLDITEMS_SAVED				0x00000008
#define	MAP_EXITGRIDS_SAVED					0x00000010
#define	MAP_DOORTABLE_SAVED					0x00000020
#define	MAP_EDGEPOINTS_SAVED				0x00000040
#define	MAP_AMBIENTLIGHTLEVEL_SAVED			0x00000080
#define	MAP_NPCSCHEDULES_SAVED				0x00000100

#ifdef JA2EDITOR
	extern BOOLEAN gfErrorCatch;
	extern CHAR16 gzErrorCatchString[256];
#endif

//dnl ch43 290909
//<SB> variable map size
INT32 guiWorldCols = OLD_WORLD_COLS;
INT32 guiWorldRows = OLD_WORLD_ROWS;
// размеры должны быть произведением 8
// size must be multiple of 8
//SB: resize all service array due to tactical map size change
extern UINT8 *gubGridNoMarkers;
extern UINT8 *gubFOVDebugInfoInfo;
extern INT16 gsFullTileDirections[MAX_FULLTILE_DIRECTIONS];
extern INT32 dirDelta[8];
extern INT16 DirIncrementer[8];
extern INT16 *gsCoverValue;
extern INT32 gsTempActionGridNo;
extern INT32 gsOverItemsGridNo;
extern INT32 gsOutOfRangeGridNo;
//</SB>

CHAR8 gubFilename[200];

// TEMP
BOOLEAN					gfForceLoadPlayers = FALSE;
CHAR8						gzForceLoadFile[100 ];
BOOLEAN					gfForceLoad		= FALSE;

UINT8						gubCurrentLevel;
INT32						giCurrentTilesetID = 0;
CHAR8						gzLastLoadedFile[ 260 ];

UINT32			gCurrentBackground = FIRSTTEXTURE;


// From memman.c in SGP
extern					UINT32		guiMemTotal;


CHAR8 TileSurfaceFilenames[NUMBEROFTILETYPES][32];
INT8	gbNewTileSurfaceLoaded[ NUMBEROFTILETYPES ];

void SetAllNewTileSurfacesLoaded( BOOLEAN fNew )
{
	memset( gbNewTileSurfaceLoaded, fNew, sizeof( gbNewTileSurfaceLoaded ) );
}

BOOLEAN gfInitAnimateLoading = FALSE;

// Local Functions
BOOLEAN LoadTileSurfaces( char pTileSurfaceFilenames[][32], UINT8 ubTilesetID );
BOOLEAN AddTileSurface( SGPFILENAME cFilename, UINT32 ubType, UINT8 ubTilesetID, BOOLEAN fGetFromRoot );
void DestroyTileSurfaces( void );
void ProcessTilesetNamesForBPP(void);
BOOLEAN IsRoofVisibleForWireframe( INT32 sMapPos );

#ifdef JA2UBMAPS
INT32						giOldTilesetUsed;
#endif


INT8 IsHiddenTileMarkerThere( INT32 sGridNo );
extern void SetInterfaceHeightLevel( );
extern void GetRootName( STR8 pDestStr, const STR8 pSrcStr );


void SaveMapLights( HWFILE hfile );
void LoadMapLights( INT8 **hBuffer );

// Global Variables
MAP_ELEMENT			*gpWorldLevelData;
INT32						*gpDirtyData;
UINT32					gSurfaceMemUsage;
//UINT8						gubWorldMovementCosts[ WORLD_MAX ][MAXDIR][2];
UINT8 (*gubWorldMovementCosts)[MAXDIR][2] = NULL;//dnl ch43 260909
//ddd для убыстрения поиска освещенных участков в патхаи.
BOOLEAN						gubWorldTileInLight[ MAX_ALLOWED_WORLD_MAX ];
BOOLEAN						gubIsCorpseThere[ MAX_ALLOWED_WORLD_MAX ];
INT32						gubMerkCanSeeThisTile[ MAX_ALLOWED_WORLD_MAX ];
//ddd
// set to nonzero (locs of base gridno of structure are good) to have it defined by structure code
INT16		gsRecompileAreaTop = 0;
INT16		gsRecompileAreaLeft = 0;
INT16		gsRecompileAreaRight = 0;
INT16		gsRecompileAreaBottom = 0;

//TIMER TESTING STUFF
#ifdef JA2TESTVERSION
	extern UINT32 uiLoadWorldTime;
	extern UINT32 uiTrashWorldTime;
	extern UINT32 uiLoadMapTilesetTime;
	extern UINT32 uiLoadMapLightsTime;
	extern UINT32 uiBuildShadeTableTime;
	extern UINT32 uiNumTablesLoaded;
	extern UINT32 uiNumTablesSaved;
	extern UINT32 uiNumImagesReloaded;
#endif

BOOLEAN DoorAtGridNo( INT32 iMapIndex )
{
	STRUCTURE *pStruct;
	pStruct = gpWorldLevelData[ iMapIndex ].pStructureHead;
	while( pStruct )
	{
		if( pStruct->fFlags & STRUCTURE_ANYDOOR )
			return TRUE;
		pStruct = pStruct->pNext;
	}
	return FALSE;
}

BOOLEAN OpenableAtGridNo( INT32 iMapIndex )
{
	STRUCTURE *pStruct;
	pStruct = gpWorldLevelData[ iMapIndex ].pStructureHead;
	while( pStruct )
	{
		if( pStruct->fFlags & STRUCTURE_OPENABLE )
			return TRUE;
		pStruct = pStruct->pNext;
	}
	return FALSE;
}

BOOLEAN FloorAtGridNo( INT32 iMapIndex )
{
	LEVELNODE	*pLand;
	UINT32 uiTileType;
	pLand = gpWorldLevelData[ iMapIndex ].pLandHead;
	// Look through all objects and Search for type
	while( pLand )
	{
		if ( pLand->usIndex != NO_TILE )
		{
			GetTileType( pLand->usIndex, &uiTileType );
			if ( uiTileType >= FIRSTFLOOR && uiTileType <= LASTFLOOR )
			{
				return TRUE;
			}
			pLand = pLand->pNext;
		}
	}
	return FALSE;
}

BOOLEAN GridNoIndoors( INT32 iMapIndex )
{
	if( gfBasement || gfCaves )
		return TRUE;
	if( FloorAtGridNo( iMapIndex ) )
		return TRUE;
	return FALSE;
}

void DOIT( )
{
//	LEVELNODE *			pLand;
	//LEVELNODE *			pObject;
	LEVELNODE	*			pStruct, *pNewStruct;
	//LEVELNODE	*			pShadow;
	INT32 uiLoop;

	// first level
 	for( uiLoop = 0; uiLoop < WORLD_MAX; uiLoop++ )
	{
		pStruct = gpWorldLevelData[ uiLoop ].pStructHead;

		while ( pStruct != NULL )
		{
			pNewStruct = pStruct->pNext;

			if ( pStruct->usIndex >= DEBRISWOOD1 && pStruct->usIndex <= DEBRISWEEDS10 )
			{
				AddObjectToHead( uiLoop, pStruct->usIndex );

				RemoveStruct( uiLoop, pStruct->usIndex );
			}

			pStruct = pNewStruct;
		}


	}

}



BOOLEAN InitializeWorld( )
{

	gTileDatabaseSize = 0;
	gSurfaceMemUsage = 0;
	giCurrentTilesetID = -1;
	
	#ifdef JA2UBMAPS
	giOldTilesetUsed = -1;
	#endif

	SetNumberOfTiles();

	// DB Adds the _8 to the names if we're in 8 bit mode.
	//ProcessTilesetNamesForBPP();

	// Memset tileset list
	memset( TileSurfaceFilenames, '\0', sizeof( TileSurfaceFilenames ) );

	// ATE: MEMSET LOG HEIGHT VALUES
	memset( gTileTypeLogicalHeight, 1, sizeof( gTileTypeLogicalHeight) );

	// Memset tile database
	memset( gTileDatabase, 0, sizeof(gTileDatabase) ); 

	// Init surface list
	memset( gTileSurfaceArray, 0, sizeof( gTileSurfaceArray ) );

	// Init default surface list
	memset( gbDefaultSurfaceUsed, 0, sizeof( gbDefaultSurfaceUsed ) );

	// Init same surface list
	memset( gbSameAsDefaultSurfaceUsed, 0, sizeof( gbSameAsDefaultSurfaceUsed ) );


	// Initialize world data

	gpWorldLevelData = (MAP_ELEMENT *)MemAlloc( WORLD_MAX * sizeof( MAP_ELEMENT ) );
	CHECKF( gpWorldLevelData );

	// Zero world
	memset( gpWorldLevelData, 0, WORLD_MAX * sizeof( MAP_ELEMENT ) );

	// Init room database
	InitRoomDatabase( );

	// INit tilesets
	InitEngineTilesets( );



	return( TRUE );

}

//dnl ch43 290909
void DeinitializeWorld()
{
	TrashWorld();
	if(gubGridNoMarkers)
		MemFree(gubGridNoMarkers);
	if(gsCoverValue)
		MemFree(gsCoverValue);
	if(gubBuildingInfo)
		MemFree(gubBuildingInfo);
	if(gusWorldRoomInfo)
		MemFree(gusWorldRoomInfo);
	if(gubWorldMovementCosts)
		MemFree(gubWorldMovementCosts);
	if(gpWorldLevelData)
		MemFree(gpWorldLevelData);
#ifdef _DEBUG
	if(gubFOVDebugInfoInfo)
		MemFree(gubFOVDebugInfoInfo);
#endif
	if(gpDirtyData)
		MemFree(gpDirtyData);
	DestroyTileSurfaces();
	FreeAllStructureFiles();
	DeallocateTileDatabase();
	ShutdownRoomDatabase();
}


BOOLEAN ReloadTilesetSlot( INT32 iSlot )
{
	return(TRUE);
}


BOOLEAN LoadTileSurfaces( char ppTileSurfaceFilenames[][32], UINT8 ubTilesetID )
{
	SGPFILENAME			cTemp;
	UINT32					uiLoop;

	UINT32					uiPercentage;
	//UINT32					uiLength;
	//UINT16					uiFillColor;
	STRING512				ExeDir;
	STRING512				INIFile;

	// Get Executable Directory
	GetExecutableDirectory( ExeDir );

	// Adjust Current Dir
	// CHECK IF DEFAULT INI OVERRIDE FILE EXISTS
	sprintf( INIFile, "%s\\engine.ini", ExeDir );
	if ( !FileExists( INIFile )	)
	{
		// USE PER TILESET BASIS
		sprintf( INIFile, "%s\\engine%d.ini", ExeDir, ubTilesetID );
	}

	// If no Tileset filenames are given, return error
	if (ppTileSurfaceFilenames == NULL)
	{
		return( FALSE );
	}
	else
	{
	for (uiLoop = 0; uiLoop < (UINT32)giNumberOfTileTypes; uiLoop++)
			strcpy( TileSurfaceFilenames[uiLoop], ppTileSurfaceFilenames[uiLoop] );//(ppTileSurfaceFilenames + (65 * uiLoop)) );
	}


	//uiFillColor = Get16BPPColor(FROMRGB(223, 223, 223));
	//StartFrameBufferRender( );
	//ColorFillVideoSurfaceArea( FRAME_BUFFER, 20, 399, 622, 420, uiFillColor );
	//ColorFillVideoSurfaceArea( FRAME_BUFFER, 21, 400, 621, 419, 0 );
	//EndFrameBufferRender( );

	//uiFillColor = Get16BPPColor(FROMRGB( 100, 0, 0 ));
	// load the tile surfaces
	SetRelativeStartAndEndPercentage( 0, 1, 35, L"Tile Surfaces" );
	for (uiLoop = 0; uiLoop < (UINT32)giNumberOfTileTypes; uiLoop++)
	{
	
	
		// ATE: Set flag indicating to use another default
		// tileset
	#ifdef JA2UBMAPS
		// 1 ) If we are going from JA2 to JA25
		if ( giOldTilesetUsed < DEFAULT_JA25_TILESET && ubTilesetID >= DEFAULT_JA25_TILESET )
		{
			gbDefaultSurfaceUsed[ uiLoop ] = FALSE;
		}
		// 2) From JA25 to JA2
		if ( ( giOldTilesetUsed >= DEFAULT_JA25_TILESET || giOldTilesetUsed == -1 ) && ubTilesetID < DEFAULT_JA25_TILESET )
		{
			gbDefaultSurfaceUsed[ uiLoop ] = FALSE;
		}
	#endif
		uiPercentage = (uiLoop * 100) / (giNumberOfTileTypes-1);
		RenderProgressBar( 0, uiPercentage );

		//uiFillColor = Get16BPPColor(FROMRGB( 100 + uiPercentage , 0, 0 ));
		//ColorFillVideoSurfaceArea( FRAME_BUFFER, 22, 401, 22 + uiLength, 418, uiFillColor );
		//InvalidateRegion( 0, 399, 640, 420 );
		//EndFrameBufferRender( );

		// The cost of having to do this check each time through the loop,
		// thus about 20 times, seems better than having to maintain two
		// almost completely identical functions
		if (ppTileSurfaceFilenames == NULL)
		{
			GetPrivateProfileString( "TileSurface Filenames", gTileSurfaceName[uiLoop], "", cTemp, SGPFILENAME_LEN, INIFile );
			if (*cTemp != '\0')
			{
				strcpy( TileSurfaceFilenames[uiLoop], cTemp );
				if (AddTileSurface( cTemp, uiLoop, ubTilesetID, TRUE ) == FALSE)
				{
					DestroyTileSurfaces( );
					return( FALSE );
				}
			}
			else
			{
				// Use default
				if (AddTileSurface( TileSurfaceFilenames[uiLoop], uiLoop, ubTilesetID, FALSE ) == FALSE)
				{
					DestroyTileSurfaces(	);
					return( FALSE );
				}

			}

		}
		else
		{
			GetPrivateProfileString( "TileSurface Filenames", gTileSurfaceName[uiLoop], "", cTemp, SGPFILENAME_LEN, INIFile );
			if (*cTemp != '\0')
			{
				strcpy( TileSurfaceFilenames[uiLoop], cTemp );
				if (AddTileSurface( cTemp, uiLoop, ubTilesetID, TRUE ) == FALSE)
				{
					DestroyTileSurfaces(	);
					return( FALSE );
				}
			}
			else
			{
				if (*(ppTileSurfaceFilenames[uiLoop]) != '\0')
				{
					if (AddTileSurface( ppTileSurfaceFilenames[uiLoop], uiLoop, ubTilesetID, FALSE ) == FALSE)
					{
						DestroyTileSurfaces( );
						return( FALSE );
					}
				}
				else
				{
					// USE FIRST TILESET VALUE!

					// ATE: If here, don't load default surface if already loaded...
					if ( !gbDefaultSurfaceUsed[ uiLoop ] )
					{
					
						#ifdef JA2UBMAPS
						if( ubTilesetID < DEFAULT_JA25_TILESET && uiLoop != SPECIALTILES )
						{
							strcpy( TileSurfaceFilenames[uiLoop], gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[uiLoop] );//(char *)(ppTileSurfaceFilenames + (65 * uiLoop)) );
							if (AddTileSurface( gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[uiLoop], uiLoop, TLS_GENERIC_1, FALSE ) == FALSE)
							{
								DestroyTileSurfaces(  );
								return( FALSE );
							}
						}
						else
						{
						  
						  if (  uiLoop == 123  )
							{					
								strcpy( TileSurfaceFilenames[123], gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[123] );//(char *)(ppTileSurfaceFilenames + (65 * uiLoop)) );
								if (AddTileSurface( gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[123], 123, TLS_GENERIC_1, FALSE ) == FALSE)
								{
									DestroyTileSurfaces(  );
									return( FALSE );
								}
							}	
							else if (  uiLoop == 124  )
							{					
								strcpy( TileSurfaceFilenames[124], gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[124] );//(char *)(ppTileSurfaceFilenames + (65 * uiLoop)) );
								if (AddTileSurface( gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[124], 124, TLS_GENERIC_1, FALSE ) == FALSE)
								{
									DestroyTileSurfaces(  );
									return( FALSE );
								}
							}
							else if (  uiLoop == 125 )
							{					
								strcpy( TileSurfaceFilenames[125], gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[125] );//(char *)(ppTileSurfaceFilenames + (65 * uiLoop)) );
								if (AddTileSurface( gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[125], 125, TLS_GENERIC_1, FALSE ) == FALSE)
								{
									DestroyTileSurfaces(  );
									return( FALSE );
								}
							}
							else if (  uiLoop == 127 )
							{					
								strcpy( TileSurfaceFilenames[127], gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[127] );//(char *)(ppTileSurfaceFilenames + (65 * uiLoop)) );
								if (AddTileSurface( gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[127], 127, TLS_GENERIC_1, FALSE ) == FALSE)
								{
									DestroyTileSurfaces(  );
									return( FALSE );
								}
							}
							else
							{
							strcpy( TileSurfaceFilenames[uiLoop], gTilesets[ DEFAULT_JA25_TILESET ].TileSurfaceFilenames[uiLoop] );//(char *)(ppTileSurfaceFilenames + (65 * uiLoop)) );
							if (AddTileSurface( gTilesets[ DEFAULT_JA25_TILESET ].TileSurfaceFilenames[uiLoop], uiLoop, DEFAULT_JA25_TILESET, FALSE ) == FALSE)
							{
								DestroyTileSurfaces(  );
								return( FALSE );
								}
							}
						}						
						#else
						strcpy( TileSurfaceFilenames[uiLoop], gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[uiLoop] );//(ppTileSurfaceFilenames + (65 * uiLoop)) );
						if (AddTileSurface( gTilesets[ TLS_GENERIC_1 ].TileSurfaceFilenames[uiLoop], uiLoop, TLS_GENERIC_1, FALSE ) == FALSE)
						{
							DestroyTileSurfaces(	);
							return( FALSE );
						}
						#endif
					}
					else
					{
						gbSameAsDefaultSurfaceUsed[ uiLoop ] = TRUE;
					}
				}
			}
		}
	}

	return( TRUE );
}

BOOLEAN AddTileSurface( STR8  cFilename, UINT32 ubType, UINT8 ubTilesetID, BOOLEAN fGetFromRoot )
{
	// Add tile surface
	PTILE_IMAGERY	TileSurf;
	CHAR8	cFileBPP[128];
	CHAR8	cAdjustedFile[ 128 ];

	// Delete the surface first!
	if ( gTileSurfaceArray[ ubType ] != NULL )
	{
		DeleteTileSurface( gTileSurfaceArray[ ubType ] );
		gTileSurfaceArray[ ubType ] = NULL;
	}

	// Adjust flag for same as default used...
	gbSameAsDefaultSurfaceUsed[ ubType ] = FALSE;

	// Adjust for BPP
	FilenameForBPP(cFilename, cFileBPP);

	if ( !fGetFromRoot )
	{
		// Adjust for tileset position
		sprintf( cAdjustedFile, "TILESETS\\%d\\%s", ubTilesetID, cFileBPP );
	}
	else
	{
		sprintf( cAdjustedFile, "%s", cFileBPP );
	}

	TileSurf = LoadTileSurface( cAdjustedFile );

	if ( TileSurf == NULL )
		return( FALSE );

	TileSurf->fType							= ubType;

	SetRaisedObjectFlag( cAdjustedFile, TileSurf );

	gTileSurfaceArray[ ubType ] = TileSurf;

	// OK, if we were not the default tileset, set value indicating that!
	
	#ifdef JA2UBMAPS
	// OK, if we were not the default tileset, set value indicating that!
	if ( ubTilesetID != TLS_GENERIC_1 && ubTilesetID != 0 )
	{
		gbDefaultSurfaceUsed[ ubType ] = FALSE;
	}
	else
	{
		gbDefaultSurfaceUsed[ ubType ] = TRUE;
	}
	#else
	if ( ubTilesetID != TLS_GENERIC_1 )
	{
		gbDefaultSurfaceUsed[ ubType ] = FALSE;
	}
	else
	{
		gbDefaultSurfaceUsed[ ubType ] = TRUE;
	}
	#endif
	
	gbNewTileSurfaceLoaded[ ubType ] = TRUE;

	return( TRUE );
}

extern BOOLEAN gfLoadShadeTablesFromTextFile;

void BuildTileShadeTables(  )
{
	// BF
	//STRING512			DataDir;
	//STRING512			ShadeTableDir;
	UINT32				uiLoop;
	CHAR8 				cRootFile[ 128 ];
	BOOLEAN				fForceRebuildForSlot = FALSE;

#ifdef JA2TESTVERSION
	UINT32				uiStartTime;
#endif
	static UINT8 ubLastRed = 255, ubLastGreen = 255, ubLastBlue = 255;

	#ifdef JA2TESTVERSION
		uiNumTablesLoaded = 0;
		uiNumTablesSaved = 0;
		uiNumImagesReloaded = 0;
		uiStartTime = GetJA2Clock();
	#endif

	// BF : adjust filename, instead of changing current directory (doesn't make sence with a VFS)
	//////////////Set the directory to the shadetable directory
	////////////GetFileManCurrentDirectory( DataDir );
	////////////sprintf( ShadeTableDir, "%s\\ShadeTables", DataDir );
	////////////if( !SetFileManCurrentDirectory( ShadeTableDir ) )
	////////////{
	////////////	AssertMsg( 0, "Can't set the directory to Data\\ShadeTable.	Kris' big problem!" );
	////////////}
	CHAR8 sIgnoreShadeTables[50];
	sprintf( sIgnoreShadeTables, "%s\\%s", "ShadeTables", "IgnoreShadeTables.txt");
	//hfile = FileOpen( sIgnoreShadeTables, FILE_ACCESS_READ, FALSE );
	//if( hfile )
	//{
	//	FileClose( hfile );
	if(FileExists(sIgnoreShadeTables))
	{
		gfForceBuildShadeTables = TRUE;
	}
	else
	{
		gfForceBuildShadeTables = FALSE;
	}
	//now, determine if we are using specialized colors.
	if( gpLightColors[0].peRed || gpLightColors[0].peGreen ||	gpLightColors[0].peBlue )
	{ //we are, which basically means we force build the shadetables.	However, the one
		//exception is if we are loading another map and the colors are the same.
		if( gpLightColors[0].peRed != ubLastRed ||
				gpLightColors[0].peGreen != ubLastGreen ||
				gpLightColors[0].peBlue != ubLastBlue )
		{	//Same tileset, but colors are different, so set things up to regenerate the shadetables.
			gfForceBuildShadeTables = TRUE;
		}
		else
		{ //same colors, same tileset, so don't rebuild shadetables -- much faster!
			gfForceBuildShadeTables = FALSE;
		}
	}

	if( gfLoadShadeTablesFromTextFile )
	{ //Because we're tweaking the RGB values in the text file, always force rebuild the shadetables
		//so that the user can tweak them in the same exe session.
		memset( gbNewTileSurfaceLoaded, 1, sizeof( gbNewTileSurfaceLoaded ) );
	}

	for (uiLoop = 0; uiLoop < (UINT32)giNumberOfTileTypes; uiLoop++)
	{
		if ( gTileSurfaceArray[ uiLoop ] != NULL )
		{
			// Don't Create shade tables if default were already used once!
			#ifdef JA2EDITOR
				if( gbNewTileSurfaceLoaded[ uiLoop ] || gfEditorForceShadeTableRebuild )
			#else
				if( gbNewTileSurfaceLoaded[ uiLoop ]  )
			#endif
				{
					fForceRebuildForSlot = FALSE;

					GetRootName( cRootFile, TileSurfaceFilenames[ uiLoop ] );

					if ( strcmp( cRootFile, "grass2" ) == 0 )
					{
						fForceRebuildForSlot = TRUE;
					}

					#ifdef JA2TESTVERSION
						uiNumImagesReloaded++;
					#endif
					RenderProgressBar( 0, uiLoop * 100 / giNumberOfTileTypes );
					CreateTilePaletteTables( gTileSurfaceArray[ uiLoop ]->vo, uiLoop, fForceRebuildForSlot );
				}
		}
	}

	// BF : see above
	////Restore the data directory once we are finished.
	//SetFileManCurrentDirectory( DataDir );

	ubLastRed = gpLightColors[0].peRed;
	ubLastGreen = gpLightColors[0].peGreen;
	ubLastBlue = gpLightColors[0].peBlue;

	#ifdef JA2TESTVERSION
		uiBuildShadeTableTime = GetJA2Clock() - uiStartTime;
	#endif
}

void DestroyTileShadeTables( )
{
	UINT32					uiLoop;

	for (uiLoop = 0; uiLoop < (UINT32)giNumberOfTileTypes; uiLoop++)
	{
		if ( gTileSurfaceArray[ uiLoop ] != NULL )
		{
			// Don't Delete shade tables if default are still being used...
			#ifdef JA2EDITOR
				if( gbNewTileSurfaceLoaded[ uiLoop ] || gfEditorForceShadeTableRebuild )
				{
					DestroyObjectPaletteTables( gTileSurfaceArray[ uiLoop ]->vo );
				}
			#else
				if( gbNewTileSurfaceLoaded[ uiLoop ] )
				{
					DestroyObjectPaletteTables( gTileSurfaceArray[ uiLoop ]->vo );
				}
			#endif
		}
	}
}

void DestroyTileSurfaces( )
{
	UINT32					uiLoop;

	for (uiLoop = 0; uiLoop < (UINT32)giNumberOfTileTypes; uiLoop++)
	{
		if ( gTileSurfaceArray[ uiLoop ] != NULL )
		{
			DeleteTileSurface( gTileSurfaceArray[ uiLoop ] );
			gTileSurfaceArray[ uiLoop ] = NULL;
		}
	}
}

void CompileWorldTerrainIDs( void )
{
	INT32			sGridNo;
	INT32			sTempGridNo;
	LEVELNODE		*pNode;
	TILE_ELEMENT	*pTileElement;
	UINT8			ubLoop;

 	for( sGridNo = 0; sGridNo < WORLD_MAX; sGridNo++ )
	{
		if (GridNoOnVisibleWorldTile( sGridNo ))
		{
			// Check if we have anything in object layer which has a terrain modifier
			pNode = gpWorldLevelData[ sGridNo ].pObjectHead;

			// ATE: CRAPOLA! Special case stuff here for the friggen pool since art was fu*ked up
			
			#ifdef JA2UBMAPS
			if ( giCurrentTilesetID == TEMP_19 )
			#else
			if ( giCurrentTilesetID == TLS_BALIME_MUSEUM )
			#endif
			{
				// Get ID
				if ( pNode != NULL )
				{
					if ( pNode->usIndex == ANOTHERDEBRIS4 || pNode->usIndex == ANOTHERDEBRIS6 ||pNode->usIndex == ANOTHERDEBRIS7 )
					{
						gpWorldLevelData[sGridNo].ubTerrainID = LOW_WATER;
						continue;
					}
				}
			}

			if (pNode == NULL || pNode->usIndex >= giNumberOfTiles || gTileDatabase[ pNode->usIndex ].ubTerrainID == NO_TERRAIN)
			{	// Try terrain instead!
				pNode = gpWorldLevelData[ sGridNo ].pLandHead;
			}
			pTileElement = &(gTileDatabase[ pNode->usIndex ]);
			if (pTileElement->ubNumberOfTiles > 1)
			{
				for (ubLoop = 0; ubLoop < pTileElement->ubNumberOfTiles; ubLoop++)
				{
					sTempGridNo = sGridNo + pTileElement->pTileLocData[ubLoop].bTileOffsetX + pTileElement->pTileLocData[ubLoop].bTileOffsetY * WORLD_COLS;
					gpWorldLevelData[sTempGridNo].ubTerrainID = pTileElement->ubTerrainID;
				}
			}
			else
			{
				gpWorldLevelData[sGridNo].ubTerrainID = pTileElement->ubTerrainID;
			}
		}
	}
}

BOOLEAN IsNotRestrictedWindow(STRUCTURE *	pStructure)
{	

	if (	(pStructure->fFlags & STRUCTURE_WALLNWINDOW) && gGameExternalOptions.fCanJumpThroughWindows
			&& !(pStructure->fFlags & STRUCTURE_SPECIAL)
			&& (pStructure->fFlags & STRUCTURE_OPEN)
			&& (pStructure->pDBStructureRef->pDBStructure->bPartnerDelta == NO_PARTNER_STRUCTURE)		)
	{
		//ручная проверка на заколоченые окна ;( {
		//build_13.sti - заколоченные окна ндекс: 
		
		LEVELNODE *pNode = NULL;// STRUCTURE	*pBase=NULL;
		UINT32 uiTileType=0;
		//pBase = FindStructure( pStructure->sGridNo, STRUCTURE_WALLNWINDOW );
		//if(pBase!= NULL)
			pNode = FindLevelNodeBasedOnStructure( pStructure->sGridNo, pStructure );
		//pNode = gpWorldLevelData[ pStructure->sGridNo ].pStructHead;
		if(pNode != NULL)
			GetTileType( pNode->usIndex, &uiTileType );

		UINT16 RestrSubIndex;
        Assert(pNode);
		GetSubIndexFromTileIndex( pNode->usIndex, (UINT16 *)&RestrSubIndex );

		//этот тип окон не содержится в 0 тайлсете, проверка нулевого тайлсета в этом случае не нужна
		if ( _stricmp( gTilesets[ giCurrentTilesetID ].TileSurfaceFilenames[ uiTileType ], "build_13.sti" ) == 0 
			//&& ( pNode->usIndex == 814 || pNode->usIndex == 816 || pNode->usIndex == 817 || pNode->usIndex == 823) -запрещает только в конкретном тайлсете ;( в другом не сработает
			&& (RestrSubIndex == 40 || RestrSubIndex == 41 || RestrSubIndex == 37 || RestrSubIndex == 38 
				|| RestrSubIndex == 43 || RestrSubIndex == 44 || RestrSubIndex == 46 || RestrSubIndex == 47) //номера фреймов в стишке
			
			)
			return FALSE;

		return TRUE;
	}

	return FALSE;
}
void CompileTileMovementCosts( INT32 usGridNo )
{
	UINT8						ubTerrainID;
	TILE_ELEMENT		TileElem;
	LEVELNODE *			pLand;

	STRUCTURE *			pStructure;
	BOOLEAN					fStructuresOnRoof;

	UINT8			ubDirLoop;

/*
*/

	if ( GridNoOnVisibleWorldTile( usGridNo ) )
	{
		// check for land of a different height in adjacent locations
		for ( ubDirLoop = 0; ubDirLoop < 8; ubDirLoop++ )
		{
			if ( gpWorldLevelData[ usGridNo ].sHeight !=
					 gpWorldLevelData[ usGridNo + DirectionInc( ubDirLoop ) ].sHeight )
			{
				SET_CURRMOVEMENTCOST( ubDirLoop, TRAVELCOST_OBSTACLE );
			}
		}

		// check for exit grids
		if ( ExitGridAtGridNo( usGridNo ) )
		{
			for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
			{
				SET_CURRMOVEMENTCOST( ubDirLoop, TRAVELCOST_EXITGRID );
			}
			// leave the roof alone, and continue, so that we can get values for the roof if traversable
		}

	}
	else
	{
		for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
		{
			SET_MOVEMENTCOST( usGridNo, ubDirLoop,  0, TRAVELCOST_OFF_MAP );
			SET_MOVEMENTCOST( usGridNo, ubDirLoop,  1, TRAVELCOST_OFF_MAP );
		}
		if (gpWorldLevelData[usGridNo].pStructureHead == NULL)
		{
			return;
		}
	}

	if (gpWorldLevelData[usGridNo].pStructureHead != NULL)
	{ // structures in tile
		// consider the land
		pLand = gpWorldLevelData[ usGridNo ].pLandHead;
		if ( pLand != NULL )
		{
			// Set TEMPORARY cost here
			// Get from tile database
			TileElem = gTileDatabase[ pLand->usIndex ];

			// Get terrain type
			ubTerrainID =	gpWorldLevelData[usGridNo].ubTerrainID; // = GetTerrainType( (INT16)usGridNo );

			for (ubDirLoop=0; ubDirLoop < NUM_WORLD_DIRECTIONS; ubDirLoop++)
			{
				SET_CURRMOVEMENTCOST( ubDirLoop, gTileTypeMovementCost[ ubTerrainID ] );
			}
		}

		// now consider all structures
		pStructure = gpWorldLevelData[usGridNo].pStructureHead;
		fStructuresOnRoof = FALSE;
		do
		{
			if (pStructure->sCubeOffset == STRUCTURE_ON_GROUND)
			{
				if (pStructure->fFlags & STRUCTURE_PASSABLE)
				{
					if (pStructure->fFlags & STRUCTURE_WIREFENCE && pStructure->fFlags & STRUCTURE_OPEN)
					{
						// prevent movement along the fence but allow in all other directions
						switch( pStructure->ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
							case INSIDE_TOP_LEFT:
								SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( EAST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( SOUTHEAST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( SOUTH, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_NOT_STANDING );
								break;

							case OUTSIDE_TOP_RIGHT:
							case INSIDE_TOP_RIGHT:
								SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( EAST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( SOUTHEAST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( SOUTH, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_NOT_STANDING );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_NOT_STANDING );
								break;
						}
					}
					// all other passable structures do not block movement in any way
				}
				else if (pStructure->fFlags & STRUCTURE_BLOCKSMOVES)
				{
					if ( (pStructure->fFlags & STRUCTURE_FENCE) && !(pStructure->fFlags & STRUCTURE_SPECIAL) )
					{
						// jumpable!
						switch( pStructure->ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
							case INSIDE_TOP_LEFT:
								// can be jumped north and south
								SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_FENCE );
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( EAST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( SOUTHEAST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( SOUTH, TRAVELCOST_FENCE );
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );
								// set values for the tiles EXITED from this location
								FORCE_SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTH, 0, TRAVELCOST_NONE );
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								FORCE_SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_NONE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo - 1, WEST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS - 1, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
								break;

							case OUTSIDE_TOP_RIGHT:
							case INSIDE_TOP_RIGHT:
								// can be jumped east and west
								SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( EAST, TRAVELCOST_FENCE );
								SET_CURRMOVEMENTCOST( SOUTHEAST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( SOUTH, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_FENCE );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );
								// set values for the tiles EXITED from this location
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTH, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								// make sure no obstacle costs exists before changing path cost to 0
								if ( gubWorldMovementCosts[ usGridNo + 1 ][ EAST ][ 0 ] < TRAVELCOST_BLOCKED )
								{
									FORCE_SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_NONE );
								}
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
								if ( gubWorldMovementCosts[ usGridNo - 1 ][ WEST ][ 0 ] < TRAVELCOST_BLOCKED )
								{
									FORCE_SET_MOVEMENTCOST( usGridNo - 1, WEST, 0, TRAVELCOST_NONE );
								}
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS - 1, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
								break;

							default:
								// corners aren't jumpable
								for (ubDirLoop=0; ubDirLoop < NUM_WORLD_DIRECTIONS; ubDirLoop++)
								{
									SET_CURRMOVEMENTCOST( ubDirLoop, TRAVELCOST_OBSTACLE );
								}
								break;
						}
 					}
					else if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_SANDBAG && StructureHeight( pStructure ) < 2 )
					{
						for (ubDirLoop=0; ubDirLoop < NUM_WORLD_DIRECTIONS; ubDirLoop++)
						{
							SET_CURRMOVEMENTCOST( ubDirLoop, TRAVELCOST_OBSTACLE );
						}

						if ( FindStructure( (usGridNo - WORLD_COLS), STRUCTURE_OBSTACLE ) == FALSE && FindStructure( (usGridNo + WORLD_COLS), STRUCTURE_OBSTACLE ) == FALSE )
						{
							FORCE_SET_MOVEMENTCOST( usGridNo, NORTH, 0, TRAVELCOST_FENCE );
							FORCE_SET_MOVEMENTCOST( usGridNo, SOUTH, 0, TRAVELCOST_FENCE );
						}

						if ( FindStructure( (usGridNo - 1), STRUCTURE_OBSTACLE ) == FALSE && FindStructure( (usGridNo + 1), STRUCTURE_OBSTACLE ) == FALSE )
						{
							FORCE_SET_MOVEMENTCOST( usGridNo, EAST, 0, TRAVELCOST_FENCE );
							FORCE_SET_MOVEMENTCOST( usGridNo, WEST, 0, TRAVELCOST_FENCE );
						}
					}
					else if ( (pStructure->fFlags & STRUCTURE_CAVEWALL ) )
					{
						for (ubDirLoop=0; ubDirLoop < NUM_WORLD_DIRECTIONS; ubDirLoop++)
						{
							SET_CURRMOVEMENTCOST( ubDirLoop, TRAVELCOST_CAVEWALL );
						}
					}
					else
					{
						for (ubDirLoop=0; ubDirLoop < NUM_WORLD_DIRECTIONS; ubDirLoop++)
						{
							SET_CURRMOVEMENTCOST( ubDirLoop, TRAVELCOST_OBSTACLE );
						}
					}
				}
				else if (pStructure->fFlags & STRUCTURE_ANYDOOR) /*&& (pStructure->fFlags & STRUCTURE_OPEN))*/
				{ // NB closed doors are treated just like walls, in the section after this

					if (pStructure->fFlags & STRUCTURE_DDOOR_LEFT && (pStructure->ubWallOrientation == INSIDE_TOP_RIGHT || pStructure->ubWallOrientation == OUTSIDE_TOP_RIGHT) )
					{
						// double door, left side (as you look on the screen)
						switch( pStructure->ubWallOrientation )
						{
							case OUTSIDE_TOP_RIGHT:
								if (pStructure->fFlags & STRUCTURE_BASE_TILE)
								{ // doorpost
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
									SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_CLOSED_W );
									SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_WALL );
									// corner
									SET_MOVEMENTCOST( usGridNo + 1 + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_WALL );
								}
								else
								{	// door
									SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_OPEN_W );
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_DOOR_OPEN_W );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_OPEN_NW );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_DOOR_OPEN_NW );
									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_DOOR_OPEN_W_W );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_DOOR_OPEN_NW_W );
								}
								break;

							case INSIDE_TOP_RIGHT:
								// doorpost
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
								SET_MOVEMENTCOST( usGridNo + 1,NORTHEAST, 0, TRAVELCOST_WALL );
								// door
								SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_OPEN_HERE );
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_DOOR_OPEN_HERE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_OPEN_N );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_DOOR_OPEN_N );
								SET_MOVEMENTCOST( usGridNo - 1, NORTHWEST, 0, TRAVELCOST_DOOR_OPEN_E );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_DOOR_OPEN_NE );
								break;

							default:
								// door with no orientation specified!?
								break;
						}
					}
					else if (pStructure->fFlags & STRUCTURE_DDOOR_RIGHT && (pStructure->ubWallOrientation == INSIDE_TOP_LEFT || pStructure->ubWallOrientation == OUTSIDE_TOP_LEFT) )
					{
						// double door, right side (as you look on the screen)
						switch( pStructure->ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
								if (pStructure->fFlags & STRUCTURE_BASE_TILE)
								{	// doorpost
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
									SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_CLOSED_N )
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_WALL );									;
									// corner
									SET_MOVEMENTCOST( usGridNo + 1 ,NORTHEAST, 0, TRAVELCOST_WALL );
								}
								else
								{ // door
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_DOOR_OPEN_N );
									SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_OPEN_N );
									SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_OPEN_NW );
									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_DOOR_OPEN_NW );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_DOOR_OPEN_N_N );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_DOOR_OPEN_NW_N );
								}
								break;

							case INSIDE_TOP_LEFT:
								// doorpost
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_WALL );
								// corner
								SET_MOVEMENTCOST( usGridNo + 1 ,NORTHEAST, 0, TRAVELCOST_WALL );
								// door
								SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_OPEN_HERE );
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_DOOR_OPEN_HERE );
								SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_OPEN_W );
								SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_DOOR_OPEN_W );
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHWEST, 0, TRAVELCOST_DOOR_OPEN_S );
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_DOOR_OPEN_SW );
								break;
							default:
								// door with no orientation specified!?
								break;
						}
					}
					else if (pStructure->fFlags & STRUCTURE_SLIDINGDOOR && pStructure->pDBStructureRef->pDBStructure->ubNumberOfTiles > 1)
					{
						switch( pStructure->ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
							case INSIDE_TOP_LEFT:
								// doorframe post in one corner of each of the tiles
								if (pStructure->fFlags & STRUCTURE_BASE_TILE)
								{
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
									SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_CLOSED_N );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_DOOR_CLOSED_N );

								}
								else
								{
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_DOOR_CLOSED_N);
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_CLOSED_N);
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_WALL );

								}
								break;
							case OUTSIDE_TOP_RIGHT:
							case INSIDE_TOP_RIGHT:
								// doorframe post in one corner of each of the tiles
								if (pStructure->fFlags & STRUCTURE_BASE_TILE)
								{
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
									SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_DOOR_CLOSED_HERE );

									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_CLOSED_W );
									SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_DOOR_CLOSED_W );
								}
								else
								{
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_WALL );

									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_DOOR_CLOSED_W );
									SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_CLOSED_W );
									SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_WALL );
								}
								break;
						}
					}
					else
					{
						// standard door
						switch( pStructure->ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
								if (pStructure->fFlags & STRUCTURE_BASE_TILE)
								{	// doorframe
									SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
									SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );

									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_CLOSED_N );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_WALL );

									// DO CORNERS
									SET_MOVEMENTCOST( usGridNo - 1, NORTHWEST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_WALL );


									//SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_OBSTACLE );
									//SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );
									//SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
									//SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
									// corner
									//SET_MOVEMENTCOST( usGridNo + 1 ,NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								}
								else if (!(pStructure->fFlags & STRUCTURE_SLIDINGDOOR))
								{ // door
									SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
									SET_CURRMOVEMENTCOST( EAST, TRAVELCOST_DOOR_OPEN_N );
									SET_MOVEMENTCOST( usGridNo - 1, WEST, 0, TRAVELCOST_DOOR_OPEN_NE );
									SET_MOVEMENTCOST( usGridNo - 1, NORTHWEST, 0, TRAVELCOST_WALL );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_DOOR_OPEN_N_N );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_DOOR_OPEN_NE_N );
								}
								break;

							case INSIDE_TOP_LEFT:
								SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
								SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_CLOSED_HERE );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );

								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_CLOSED_N );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );

								// DO CORNERS
								SET_MOVEMENTCOST( usGridNo - 1, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );

								// doorframe
								//SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_OBSTACLE );
								//SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );
								//SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								//SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
								// corner
								//SET_MOVEMENTCOST( usGridNo + 1 ,NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								// door
								if (!(pStructure->fFlags & STRUCTURE_SLIDINGDOOR))
								{
									SET_CURRMOVEMENTCOST( EAST, TRAVELCOST_DOOR_OPEN_HERE );
									SET_CURRMOVEMENTCOST( SOUTHEAST, TRAVELCOST_DOOR_OPEN_HERE );
									SET_MOVEMENTCOST( usGridNo - 1, WEST, 0, TRAVELCOST_DOOR_OPEN_E );
									SET_MOVEMENTCOST( usGridNo - 1, SOUTHWEST, 0, TRAVELCOST_DOOR_OPEN_E );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHEAST, 0, TRAVELCOST_DOOR_OPEN_S );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS - 1, NORTHWEST, 0, TRAVELCOST_DOOR_OPEN_SE );
								}
								break;

							case OUTSIDE_TOP_RIGHT:
								if (pStructure->fFlags & STRUCTURE_BASE_TILE)
								{ // doorframe
									SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
									SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_CLOSED_HERE );
									SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );

									SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
									SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_CLOSED_W );
									SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );

									// DO CORNERS
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
									SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );

									//SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
									//SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );
									//SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
									//SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
									// corner
									//SET_MOVEMENTCOST( usGridNo + 1 + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								}
								else if (!(pStructure->fFlags & STRUCTURE_SLIDINGDOOR))
								{	// door
									SET_CURRMOVEMENTCOST( SOUTH, TRAVELCOST_DOOR_OPEN_W );
									SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_DOOR_OPEN_W );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTH, 0, TRAVELCOST_DOOR_OPEN_SW );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHWEST, 0, TRAVELCOST_DOOR_OPEN_SW );
									SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_DOOR_OPEN_W_W );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_DOOR_OPEN_SW_W );
								}
								break;

							case INSIDE_TOP_RIGHT:
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_CLOSED_HERE );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );

								SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_CLOSED_W );
								SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );

								// DO CORNERS
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );

								// doorframe
								/*
								SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
								SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + 1,SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
								SET_MOVEMENTCOST( usGridNo + 1,NORTHEAST, 0, TRAVELCOST_OBSTACLE );
								// corner
								SET_MOVEMENTCOST( usGridNo - WORLD_COLS,  NORTHWEST, 0, TRAVELCOST_OBSTACLE );
								*/
								if (!(pStructure->fFlags & STRUCTURE_SLIDINGDOOR))
								{
									// door
									SET_CURRMOVEMENTCOST( SOUTH, TRAVELCOST_DOOR_OPEN_HERE );
									SET_CURRMOVEMENTCOST( SOUTHEAST, TRAVELCOST_DOOR_OPEN_HERE );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTH, 0, TRAVELCOST_DOOR_OPEN_S );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHEAST, 0, TRAVELCOST_DOOR_OPEN_S );
									SET_MOVEMENTCOST( usGridNo - 1, SOUTHWEST, 0, TRAVELCOST_DOOR_OPEN_E );
									SET_MOVEMENTCOST( usGridNo - WORLD_COLS - 1, NORTHWEST, 0, TRAVELCOST_DOOR_OPEN_SE );
								}
								break;

							default:
								// door with no orientation specified!?
								break;
						}
					}

					/*
					switch( pStructure->ubWallOrientation )
					{
						case OUTSIDE_TOP_LEFT:
						case INSIDE_TOP_LEFT:
							SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_OBSTACLE );
							SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_DOOR_CLOSED_HERE );
							SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );

							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_DOOR_CLOSED_N );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );

							// DO CORNERS
							SET_MOVEMENTCOST( usGridNo - 1, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
							break;

						case OUTSIDE_TOP_RIGHT:
						case INSIDE_TOP_RIGHT:
							SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_OBSTACLE );
							SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_DOOR_CLOSED_HERE );
							SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_OBSTACLE );

							SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_DOOR_CLOSED_W );
							SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );

							// DO CORNERS
							SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHWEST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_OBSTACLE );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_OBSTACLE );
							break;

						default:
							// wall with no orientation specified!?
							break;
					}
					*/


				}
				else if (pStructure->fFlags & STRUCTURE_WALLSTUFF )
				{
					//ATE: IF a closed door, set to door value
					switch( pStructure->ubWallOrientation )
					{
						case OUTSIDE_TOP_LEFT:
						case INSIDE_TOP_LEFT:
							SET_CURRMOVEMENTCOST( NORTHEAST, TRAVELCOST_WALL );
							SET_CURRMOVEMENTCOST( NORTH, TRAVELCOST_WALL );
							SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHEAST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTH, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_WALL );

							// DO CORNERS
							SET_MOVEMENTCOST( usGridNo - 1, NORTHWEST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS - 1, SOUTHWEST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_WALL );
							break;

						case OUTSIDE_TOP_RIGHT:
						case INSIDE_TOP_RIGHT:
							SET_CURRMOVEMENTCOST( SOUTHWEST, TRAVELCOST_WALL );
							SET_CURRMOVEMENTCOST( WEST, TRAVELCOST_WALL );
							SET_CURRMOVEMENTCOST( NORTHWEST, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + 1, SOUTHEAST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + 1, EAST, 0, TRAVELCOST_WALL );								
							SET_MOVEMENTCOST( usGridNo + 1, NORTHEAST, 0, TRAVELCOST_WALL );

							// DO CORNERS
							SET_MOVEMENTCOST( usGridNo - WORLD_COLS + 1, NORTHEAST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo - WORLD_COLS, NORTHWEST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS + 1, SOUTHEAST, 0, TRAVELCOST_WALL );
							SET_MOVEMENTCOST( usGridNo + WORLD_COLS, SOUTHWEST, 0, TRAVELCOST_WALL );
							break;

						default:
							// wall with no orientation specified!?
							break;
					}
				}
			}
			else
			{
				if (!(pStructure->fFlags & STRUCTURE_PASSABLE || pStructure->fFlags & STRUCTURE_NORMAL_ROOF))
				{
					// DNS:  Try a fix to prevent people from "permanently" blocking the roof
					if (!(pStructure->fFlags & STRUCTURE_PERSON))
					{
						fStructuresOnRoof = TRUE;
					}
				}
			}
			pStructure = pStructure->pNext;
		} while (pStructure != NULL);

		// HIGHEST LAYER
		if ((gpWorldLevelData[ usGridNo ].pRoofHead != NULL))
		{
			if (!fStructuresOnRoof)
			{
				for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
				{
					SET_MOVEMENTCOST( usGridNo, ubDirLoop,  1, TRAVELCOST_FLAT );
				}
			}
			else
			{
				for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
				{
					SET_MOVEMENTCOST( usGridNo, ubDirLoop,  1, TRAVELCOST_OBSTACLE );
				}
			}
		}
		else
		{
			for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
			{
				SET_MOVEMENTCOST( usGridNo, ubDirLoop,  1, TRAVELCOST_OBSTACLE );
			}
		}
	}
	else
	{ // NO STRUCTURES IN TILE
		// consider just the land

		// Get terrain type
		ubTerrainID =	gpWorldLevelData[usGridNo].ubTerrainID; // = GetTerrainType( (INT16)usGridNo );
		for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
		{
			SET_MOVEMENTCOST( usGridNo ,ubDirLoop, 0, gTileTypeMovementCost[ ubTerrainID ] );
		}

/*
		pLand = gpWorldLevelData[ usGridNo ].pLandHead;
		if ( pLand != NULL )
		{
			// Set cost here
			// Get from tile database
			TileElem = gTileDatabase[ pLand->usIndex ];

			// Get terrain type
			ubTerrainID =	GetTerrainType( (INT16)usGridNo );

			for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
			{
				SET_MOVEMENTCOST( usGridNo ,ubDirLoop, 0, gTileTypeMovementCost[ ubTerrainID ] );
			}
		}
*/
		// HIGHEST LEVEL
		if (gpWorldLevelData[ usGridNo ].pRoofHead != NULL)
		{
			for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
			{
				SET_MOVEMENTCOST( usGridNo, ubDirLoop,  1, TRAVELCOST_FLAT );
			}
		}
		else
		{
			for (ubDirLoop=0; ubDirLoop < 8; ubDirLoop++)
			{
				SET_MOVEMENTCOST( usGridNo, ubDirLoop,  1, TRAVELCOST_OBSTACLE );
			}
		}
	}
}

#define LOCAL_RADIUS 4

void RecompileLocalMovementCosts( INT32 sCentreGridNo )
{
	INT32		usGridNo;
	INT16		sGridX, sGridY;
	INT16		sCentreGridX, sCentreGridY;
	INT8		bDirLoop;

	ConvertGridNoToXY( sCentreGridNo, &sCentreGridX, &sCentreGridY );
	for( sGridY = sCentreGridY - LOCAL_RADIUS; sGridY < sCentreGridY + LOCAL_RADIUS; sGridY++ )
	{
		for( sGridX = sCentreGridX - LOCAL_RADIUS; sGridX < sCentreGridX + LOCAL_RADIUS; sGridX++ )
		{
			usGridNo = MAPROWCOLTOPOS( sGridY, sGridX );
			// times 2 for 2 levels, times 2 for UINT16s
//			memset( &(gubWorldMovementCosts[usGridNo]), 0, MAXDIR * 2 * 2 );
			if (usGridNo < WORLD_MAX)
			{
				for( bDirLoop = 0; bDirLoop < MAXDIR; bDirLoop++)
				{
					gubWorldMovementCosts[usGridNo][bDirLoop][0] = 0;
					gubWorldMovementCosts[usGridNo][bDirLoop][1] = 0;
				}
			}
		}
	}

	// note the radius used in this loop is larger, to guarantee that the
	// edges of the recompiled areas are correct (i.e. there could be spillover)
	for( sGridY = sCentreGridY - LOCAL_RADIUS - 1; sGridY < sCentreGridY + LOCAL_RADIUS + 1; sGridY++ )
	{
		for( sGridX = sCentreGridX - LOCAL_RADIUS - 1; sGridX < sCentreGridX + LOCAL_RADIUS + 1; sGridX++ )
		{
			usGridNo = MAPROWCOLTOPOS( sGridY, sGridX );
			if (usGridNo < WORLD_MAX)
			{
				CompileTileMovementCosts( usGridNo );
			}
		}
	}
}


void RecompileLocalMovementCostsFromRadius( INT32 sCentreGridNo, INT8 bRadius )
{
	INT32		usGridNo;
	INT16		sGridX, sGridY;
	INT16		sCentreGridX, sCentreGridY;
	INT8		bDirLoop;

	ConvertGridNoToXY( sCentreGridNo, &sCentreGridX, &sCentreGridY );
	if (bRadius == 0)
	{
		// one tile check only
		for( bDirLoop = 0; bDirLoop < MAXDIR; bDirLoop++)
		{
			gubWorldMovementCosts[sCentreGridNo][bDirLoop][0] = 0;
			gubWorldMovementCosts[sCentreGridNo][bDirLoop][1] = 0;
		}
		CompileTileMovementCosts( sCentreGridNo );
	}
	else
	{
		for( sGridY = sCentreGridY - bRadius; sGridY < sCentreGridY + bRadius; sGridY++ )
		{
			for( sGridX = sCentreGridX - bRadius; sGridX < sCentreGridX + bRadius; sGridX++ )
			{
				usGridNo = MAPROWCOLTOPOS( sGridY, sGridX );
				// times 2 for 2 levels, times 2 for UINT16s
	//			memset( &(gubWorldMovementCosts[usGridNo]), 0, MAXDIR * 2 * 2 );
				if (usGridNo < WORLD_MAX)
				{
					for( bDirLoop = 0; bDirLoop < MAXDIR; bDirLoop++)
					{
						gubWorldMovementCosts[usGridNo][bDirLoop][0] = 0;
						gubWorldMovementCosts[usGridNo][bDirLoop][1] = 0;
					}
				}
			}
		}

		// note the radius used in this loop is larger, to guarantee that the
		// edges of the recompiled areas are correct (i.e. there could be spillover)
		for( sGridY = sCentreGridY - bRadius - 1; sGridY < sCentreGridY + bRadius + 1; sGridY++ )
		{
			for( sGridX = sCentreGridX - bRadius - 1; sGridX < sCentreGridX + bRadius + 1; sGridX++ )
			{
				usGridNo = MAPROWCOLTOPOS( sGridY, sGridX );
				if (usGridNo < WORLD_MAX)
				{
					CompileTileMovementCosts( usGridNo );
				}
			}
		}
	}
}

void AddTileToRecompileArea( INT32 sGridNo )
{
	INT32	sCheckGridNo;
	INT16	sCheckX;
	INT16 sCheckY;

	// Set flag to wipe and recompile MPs in this tile
	if (sGridNo < 0 || sGridNo >= WORLD_MAX)
	{
		return;
	}

	gpWorldLevelData[ sGridNo ].ubExtFlags[0] |= MAPELEMENT_EXT_RECALCULATE_MOVEMENT;

	// check Top/Left of recompile region
	sCheckGridNo = NewGridNo( sGridNo, DirectionInc( NORTHWEST ) );
	sCheckX = sCheckGridNo % WORLD_COLS;
	sCheckY = sCheckGridNo / WORLD_COLS;
	if ( sCheckX < gsRecompileAreaLeft )
	{
		gsRecompileAreaLeft = sCheckX;
	}
	if ( sCheckY < gsRecompileAreaTop )
	{
		gsRecompileAreaTop = sCheckY;
	}

	// check Bottom/Right
	sCheckGridNo = NewGridNo( sGridNo, DirectionInc( SOUTHEAST ) );
	sCheckX = sCheckGridNo % WORLD_COLS;
	sCheckY = sCheckGridNo / WORLD_COLS;
	if ( sCheckX > gsRecompileAreaRight )
	{
		gsRecompileAreaRight = sCheckX;
	}
	if ( sCheckY > gsRecompileAreaBottom )
	{
		gsRecompileAreaBottom = sCheckY;
	}
}

void RecompileLocalMovementCostsInAreaWithFlags( void )
{
	INT32		usGridNo;
	INT16		sGridX, sGridY;
	INT8		bDirLoop;

	for( sGridY = gsRecompileAreaTop; sGridY <= gsRecompileAreaBottom; sGridY++ )
	{
		for( sGridX = gsRecompileAreaLeft; sGridX < gsRecompileAreaRight; sGridX++ )
		{
			usGridNo = MAPROWCOLTOPOS( sGridY, sGridX );
			if ( usGridNo < WORLD_MAX && gpWorldLevelData[ usGridNo ].ubExtFlags[0] & MAPELEMENT_EXT_RECALCULATE_MOVEMENT )
			{
				// wipe MPs in this tile!
				for( bDirLoop = 0; bDirLoop < MAXDIR; bDirLoop++)
				{
					gubWorldMovementCosts[usGridNo][bDirLoop][0] = 0;
					gubWorldMovementCosts[usGridNo][bDirLoop][1] = 0;
				}
				// reset flag
				gpWorldLevelData[ usGridNo ].ubExtFlags[0] &= (~MAPELEMENT_EXT_RECALCULATE_MOVEMENT); 
			}
		}
	}

	for( sGridY = gsRecompileAreaTop; sGridY <= gsRecompileAreaBottom; sGridY++ )
	{
		for( sGridX = gsRecompileAreaLeft; sGridX <= gsRecompileAreaRight; sGridX++ )
		{
			usGridNo = MAPROWCOLTOPOS( sGridY, sGridX );
			if (usGridNo < WORLD_MAX)
			{
				CompileTileMovementCosts( usGridNo );
			}
		}
	}
}

void RecompileLocalMovementCostsForWall( INT32 sGridNo, UINT8 ubOrientation )
{
	INT8		bDirLoop;
	INT16		sUp, sDown, sLeft, sRight;
	INT16		sX, sY;
	INT32		sTempGridNo;

	switch( ubOrientation )
	{
		case OUTSIDE_TOP_RIGHT:
		case INSIDE_TOP_RIGHT:
			sUp = -1;
			sDown = 1;
			sLeft = 0;
			sRight = 1;
			break;
		case OUTSIDE_TOP_LEFT:
		case INSIDE_TOP_LEFT:
			sUp = 0;
			sDown = 1;
			sLeft = -1;
			sRight = 1;
			break;
		default:
			return;
	}

	for ( sY = sUp; sY <= sDown; sY++ )
	{
		for ( sX = sLeft; sX <= sRight; sX++ )
		{
			sTempGridNo = sGridNo + sX + sY * WORLD_COLS;
			for( bDirLoop = 0; bDirLoop < MAXDIR; bDirLoop++)
			{
				gubWorldMovementCosts[sTempGridNo][bDirLoop][0] = 0;
				gubWorldMovementCosts[sTempGridNo][bDirLoop][1] = 0;
			}

			CompileTileMovementCosts( sTempGridNo );
		}
	}
}

// GLOBAL WORLD MANIPULATION FUNCTIONS
void CompileWorldMovementCosts(void)//dnl ch56 151009
{
	memset(gubWorldMovementCosts, 0, sizeof(UINT8)*WORLD_MAX*MAXDIR*2);
	CompileWorldTerrainIDs();
 	for(INT32 usGridNo=0; usGridNo<WORLD_MAX; usGridNo++)
		CompileTileMovementCosts(usGridNo);
}

// SAVING CODE
BOOLEAN SaveWorld(const STR8 puiFilename, FLOAT dMajorMapVersion, UINT8 ubMinorMapVersion)//dnl ch33 150909
{
#ifdef JA2EDITOR
	INT32			cnt;
	UINT32			uiSoldierSize;
	UINT32			uiType;
	UINT32			uiFlags;
	UINT32			uiBytesWritten;
	UINT32			uiNumWarningsCaught = 0;
	HWFILE			hfile;
	LEVELNODE		*pLand;
	LEVELNODE		*pObject;
	LEVELNODE		*pStruct;
	LEVELNODE		*pShadow;
	LEVELNODE		*pRoof;
	LEVELNODE		*pOnRoof;
	LEVELNODE		*pTailLand=NULL;
	UINT16			usNumExitGrids = 0;
	UINT16			usTypeSubIndex;
	UINT8			LayerCount;
	UINT8			ObjectCount;
	UINT8			StructCount;
	UINT8			ShadowCount;
	UINT8			RoofCount;
	UINT8			OnRoofCount;
	UINT8			ubType;
	UINT8			ubTypeSubIndex;
	UINT8			ubTest = 1;
	CHAR8			aFilename[2*FILENAME_BUFLEN];//dnl ch81 021213
	UINT8			ubCombine;
//	UINT8			bCounts[ WORLD_MAX ][8];
	UINT8**			bCounts = NULL;
	INT32			i;

	bCounts = (UINT8**)MemAlloc(WORLD_MAX*sizeof(UINT8*));
	for(i = 0; i<WORLD_MAX; i++)
		bCounts[i] = (UINT8*)MemAlloc(8);
	

	sprintf( aFilename, "MAPS\\%s", puiFilename );
	FileDelete(aFilename);// If file exist FileOpen will not truncate, so delete.
	// Open file
	hfile = FileOpen( aFilename, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );

	if ( !hfile )
	{
		return( FALSE );
	}
	//dnl ch33 150909
	FileWrite(hfile, &dMajorMapVersion, sizeof(FLOAT), &uiBytesWritten);
	if(dMajorMapVersion >= 4.00)
		FileWrite(hfile, &ubMinorMapVersion, sizeof(UINT8), &uiBytesWritten);
	if(!(dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION))
	{
		FileWrite(hfile, &WORLD_ROWS, sizeof(INT32), &uiBytesWritten);
		FileWrite(hfile, &WORLD_COLS, sizeof(INT32), &uiBytesWritten);
	}

	// Write FLAGS FOR WORLD
	//uiFlags = MAP_WORLDONLY_SAVED;
	uiFlags = 0;
	uiFlags |= MAP_FULLSOLDIER_SAVED;
	uiFlags |= MAP_EXITGRIDS_SAVED;
	uiFlags |= MAP_WORLDLIGHTS_SAVED;
	uiFlags |= MAP_DOORTABLE_SAVED;
	uiFlags |= MAP_WORLDITEMS_SAVED;
	uiFlags |= MAP_EDGEPOINTS_SAVED;
	if( gfBasement || gfCaves )
		uiFlags |= MAP_AMBIENTLIGHTLEVEL_SAVED;
	uiFlags |= MAP_NPCSCHEDULES_SAVED;

	FileWrite( hfile, &uiFlags, sizeof( INT32 ), &uiBytesWritten );

	// Write tileset ID
	FileWrite( hfile, &giCurrentTilesetID, sizeof( INT32 ), &uiBytesWritten );

	// WDS - Clean up inventory handling
	// Write SOLDIER CONTROL SIZE
	uiSoldierSize = SIZEOF_SOLDIERTYPE_POD; //SIZEOF_SOLDIERTYPE;
	FileWrite( hfile, &uiSoldierSize, sizeof( INT32 ), &uiBytesWritten );


	// REMOVE WORLD VISIBILITY TILES
	RemoveWorldWireFrameTiles( );


	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{

		// Write out height values
		FileWrite( hfile, &gpWorldLevelData[ cnt ].sHeight, sizeof( INT16 ), &uiBytesWritten );

	}

	// Write out # values - we'll have no more than 15 per level!
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// Determine number of land
		pLand = gpWorldLevelData[ cnt ].pLandHead;
		LayerCount = 0;

		while( pLand != NULL )
		{
			LayerCount++;
			pLand = pLand->pNext;
		}
		if( LayerCount > 15 )
		{
			swprintf( gzErrorCatchString, L"SAVE ABORTED!  Land count too high (%d) for gridno %d."
				L"  Need to fix before map can be saved!  There are %d additional warnings.",
				LayerCount, cnt, uiNumWarningsCaught );
			gfErrorCatch = TRUE;
			FileClose( hfile );
			return FALSE;
		}
		if( LayerCount > 10 )
		{
			uiNumWarningsCaught++;
			gfErrorCatch = TRUE;
			swprintf( gzErrorCatchString, L"Warnings %d -- Last warning:	Land count warning of %d for gridno %d.",
				uiNumWarningsCaught, LayerCount, cnt );
		}
		bCounts[ cnt ][ 0 ] = LayerCount;

		// Combine # of land layers with worlddef flags ( first 4 bits )
		ubCombine = (UINT8)( ( LayerCount&0xf ) | ( (gpWorldLevelData[ cnt ].uiFlags&0xf)<<4 ) );
		// Write combination
		FileWrite( hfile, &ubCombine, sizeof( ubCombine ), &uiBytesWritten );


		// Determine # of objects
		pObject = gpWorldLevelData[ cnt ].pObjectHead;
		ObjectCount = 0;
		while( pObject != NULL )
		{
			// DON'T WRITE ANY ITEMS
			if ( !(pObject->uiFlags & ( LEVELNODE_ITEM ) ) )
			{
				UINT32 uiTileType;
				//Make sure this isn't a UI Element
				GetTileType( pObject->usIndex, &uiTileType );
				if( uiTileType < FIRSTPOINTERS )
					ObjectCount++;
			}
			pObject = pObject->pNext;
		}
		if( ObjectCount > 15 )
		{
			swprintf( gzErrorCatchString, L"SAVE ABORTED!  Object count too high (%d) for gridno %d."
				L"  Need to fix before map can be saved!  There are %d additional warnings.",
				ObjectCount, cnt, uiNumWarningsCaught );
			gfErrorCatch = TRUE;
			FileClose( hfile );
			return FALSE;
		}
		if( ObjectCount > 10 )
		{
			uiNumWarningsCaught++;
			gfErrorCatch = TRUE;
			swprintf( gzErrorCatchString, L"Warnings %d -- Last warning:	Object count warning of %d for gridno %d.",
				uiNumWarningsCaught, ObjectCount, cnt );
		}
		bCounts[ cnt ][ 1 ] = ObjectCount;

		// Determine # of structs
		pStruct = gpWorldLevelData[ cnt ].pStructHead;
		StructCount = 0;
		while( pStruct != NULL )
		{
			// DON'T WRITE ANY ITEMS
			if ( !(pStruct->uiFlags & ( LEVELNODE_ITEM ) ) )
			{
				StructCount++;
			}
			pStruct = pStruct->pNext;
		}
		if( StructCount > 15 )
		{
			swprintf( gzErrorCatchString, L"SAVE ABORTED!  Struct count too high (%d) for gridno %d."
				L"  Need to fix before map can be saved!  There are %d additional warnings.",
				StructCount, cnt, uiNumWarningsCaught );
			gfErrorCatch = TRUE;
			FileClose( hfile );
			return FALSE;
		}
		if( StructCount > 10 )
		{
			uiNumWarningsCaught++;
			gfErrorCatch = TRUE;
			swprintf( gzErrorCatchString, L"Warnings %d -- Last warning:	Struct count warning of %d for gridno %d.",
				uiNumWarningsCaught, StructCount, cnt );
		}
		bCounts[ cnt ][ 2 ] = StructCount;

		ubCombine = (UINT8)( ( ObjectCount&0xf ) | ( (StructCount&0xf)<<4 ) );
		// Write combination
		FileWrite( hfile, &ubCombine, sizeof( ubCombine ), &uiBytesWritten );


		// Determine # of shadows
		pShadow = gpWorldLevelData[ cnt ].pShadowHead;
		ShadowCount = 0;
		while( pShadow != NULL )
		{
			// Don't write any shadowbuddys or exit grids
			if ( !(pShadow->uiFlags & ( LEVELNODE_BUDDYSHADOW	| LEVELNODE_EXITGRID ) ) )
			{
				ShadowCount++;
			}
			pShadow = pShadow->pNext;
		}
		if( ShadowCount > 15 )
		{
			swprintf( gzErrorCatchString, L"SAVE ABORTED!  Shadow count too high (%d) for gridno %d."
				L"  Need to fix before map can be saved!  There are %d additional warnings.",
				ShadowCount, cnt, uiNumWarningsCaught );
			gfErrorCatch = TRUE;
			FileClose( hfile );
			return FALSE;
		}
		if( ShadowCount > 10 )
		{
			uiNumWarningsCaught++;
			gfErrorCatch = TRUE;
			swprintf( gzErrorCatchString, L"Warnings %d -- Last warning:	Shadow count warning of %d for gridno %d.",
				uiNumWarningsCaught, ShadowCount, cnt );
		}
		bCounts[ cnt ][ 3 ] = ShadowCount;

		// Determine # of Roofs
		pRoof = gpWorldLevelData[ cnt ].pRoofHead;
		RoofCount = 0;
		while( pRoof != NULL )
		{
			// ATE: Don't save revealed roof info...
			if ( pRoof->usIndex != SLANTROOFCEILING1 )
			{
				RoofCount++;
			}
			pRoof = pRoof->pNext;
		}
		if( RoofCount > 15 )
		{
			swprintf( gzErrorCatchString, L"SAVE ABORTED!  Roof count too high (%d) for gridno %d."
				L"  Need to fix before map can be saved!  There are %d additional warnings.",
				RoofCount, cnt, uiNumWarningsCaught );
			gfErrorCatch = TRUE;
			FileClose( hfile );
			return FALSE;
		}
		if( RoofCount > 10 )
		{
			uiNumWarningsCaught++;
			gfErrorCatch = TRUE;
			swprintf( gzErrorCatchString, L"Warnings %d -- Last warning:	Roof count warning of %d for gridno %d.",
				uiNumWarningsCaught, RoofCount, cnt );
		}
		bCounts[ cnt ][ 4 ] = RoofCount;

		ubCombine = (UINT8)( ( ShadowCount&0xf ) | ( (RoofCount&0xf)<<4 ) );
		// Write combination
		FileWrite( hfile, &ubCombine, sizeof( ubCombine ), &uiBytesWritten );


		// Write OnRoof layer
		// Determine # of OnRoofs
		pOnRoof = gpWorldLevelData[ cnt ].pOnRoofHead;
		OnRoofCount = 0;

		while( pOnRoof != NULL )
		{
			OnRoofCount++;
			pOnRoof = pOnRoof->pNext;
		}
		if( OnRoofCount > 15 )
		{
			swprintf( gzErrorCatchString, L"SAVE ABORTED!  OnRoof count too high (%d) for gridno %d."
				L"  Need to fix before map can be saved!  There are %d additional warnings.",
				OnRoofCount, cnt, uiNumWarningsCaught );
			gfErrorCatch = TRUE;
			FileClose( hfile );
			return FALSE;
		}
		if( OnRoofCount > 10 )
		{
			uiNumWarningsCaught++;
			gfErrorCatch = TRUE;
			swprintf( gzErrorCatchString, L"Warnings %d -- Last warning:	OnRoof count warning of %d for gridno %d.",
				uiNumWarningsCaught, OnRoofCount, cnt );
		}
		bCounts[ cnt ][ 5 ] = RoofCount;


		// Write combination of onroof and nothing...
		ubCombine = (UINT8)( ( OnRoofCount&0xf ) );
		// Write combination
		FileWrite( hfile, &ubCombine, sizeof( ubCombine ), &uiBytesWritten );



	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{

		if ( bCounts[ cnt ][ 0 ] == 0 )
		{

			FileWrite( hfile, &ubTest, sizeof( UINT8 ), &uiBytesWritten );
			FileWrite( hfile, &ubTest, sizeof( UINT8 ), &uiBytesWritten );
		}
		else
		{

			// Write land layers
			// Write out land peices backwards so that they are loaded properly
			pLand = gpWorldLevelData[ cnt ].pLandHead;
			// GET TAIL
			while( pLand != NULL )
			{
				pTailLand = pLand;
				pLand = pLand->pNext;
			}

			while( pTailLand != NULL )
			{
				// Write out object type and sub-index
				GetTileType( pTailLand->usIndex, &uiType );
				ubType = (UINT8)uiType;
				GetTypeSubIndexFromTileIndexChar( uiType, pTailLand->usIndex, &ubTypeSubIndex );
				FileWrite( hfile, &ubType, sizeof( UINT8 ), &uiBytesWritten );
				FileWrite( hfile, &ubTypeSubIndex, sizeof( UINT8 ), &uiBytesWritten );

				pTailLand = pTailLand->pPrevNode;
			}
		}
	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{

		// Write object layer
		pObject = gpWorldLevelData[ cnt ].pObjectHead;
		while( pObject != NULL )
		{
			// DON'T WRITE ANY ITEMS
			if ( !(pObject->uiFlags & ( LEVELNODE_ITEM ) ) )
			{
				// Write out object type and sub-index
				GetTileType( pObject->usIndex, &uiType );
				//Make sure this isn't a UI Element
				if( uiType < FIRSTPOINTERS )
				{
					//We are writing 2 bytes for the type subindex in the object layer because the
					//ROADPIECES slot contains more than 256 subindices.
					ubType = (UINT8)uiType;
					GetTypeSubIndexFromTileIndex( uiType, pObject->usIndex, &usTypeSubIndex );
					FileWrite( hfile, &ubType, sizeof( UINT8 ), &uiBytesWritten );
					FileWrite( hfile, &usTypeSubIndex, sizeof( UINT16 ), &uiBytesWritten );
				}
			}
			pObject = pObject->pNext;
		}
	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// Write struct layer
		pStruct = gpWorldLevelData[ cnt ].pStructHead;
		while( pStruct != NULL )
		{
			// DON'T WRITE ANY ITEMS
			if ( !(pStruct->uiFlags & ( LEVELNODE_ITEM ) ) )
			{
				// Write out object type and sub-index
				GetTileType( pStruct->usIndex, &uiType );
				ubType = (UINT8)uiType;
				GetTypeSubIndexFromTileIndexChar( uiType, pStruct->usIndex, &ubTypeSubIndex );
					FileWrite( hfile, &ubType, sizeof( UINT8 ), &uiBytesWritten );
					FileWrite( hfile, &ubTypeSubIndex, sizeof( UINT8 ), &uiBytesWritten );
			}

			pStruct = pStruct->pNext;
		}
	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// Write shadows
		pShadow = gpWorldLevelData[ cnt ].pShadowHead;
		while( pShadow != NULL )
		{
			// Dont't write any buddys or exit grids
			if ( !(pShadow->uiFlags & ( LEVELNODE_BUDDYSHADOW | LEVELNODE_EXITGRID ) ) )
			{
				// Write out object type and sub-index
				// Write out object type and sub-index
				GetTileType( pShadow->usIndex, &uiType );
				ubType = (UINT8)uiType;
				GetTypeSubIndexFromTileIndexChar( uiType, pShadow->usIndex, &ubTypeSubIndex );
				FileWrite( hfile, &ubType, sizeof( UINT8 ), &uiBytesWritten );
				FileWrite( hfile, &ubTypeSubIndex, sizeof( UINT8 ), &uiBytesWritten );

			}
			else if( pShadow->uiFlags & LEVELNODE_EXITGRID )
			{	//count the number of exitgrids
				usNumExitGrids++;
			}

			pShadow = pShadow->pNext;
		}
	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		pRoof = gpWorldLevelData[ cnt ].pRoofHead;
		while( pRoof != NULL )
		{
			// ATE: Don't save revealed roof info...
			if ( pRoof->usIndex != SLANTROOFCEILING1 )
			{
				// Write out object type and sub-index
				GetTileType( pRoof->usIndex, &uiType );
				ubType = (UINT8)uiType;
				GetTypeSubIndexFromTileIndexChar( uiType, pRoof->usIndex, &ubTypeSubIndex );
				FileWrite( hfile, &ubType, sizeof( UINT8 ), &uiBytesWritten );
				FileWrite( hfile, &ubTypeSubIndex, sizeof( UINT8 ), &uiBytesWritten );
			}

			pRoof = pRoof->pNext;
		}
	}

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// Write OnRoofs
		pOnRoof = gpWorldLevelData[ cnt ].pOnRoofHead;
		while( pOnRoof != NULL )
		{
			// Write out object type and sub-index
			GetTileType( pOnRoof->usIndex, &uiType );
			ubType = (UINT8)uiType;
			GetTypeSubIndexFromTileIndexChar( uiType, pOnRoof->usIndex, &ubTypeSubIndex );
			FileWrite( hfile, &ubType, sizeof( UINT8 ), &uiBytesWritten );
			FileWrite( hfile, &ubTypeSubIndex, sizeof( UINT8 ), &uiBytesWritten );


			pOnRoof = pOnRoof->pNext;
		}
	}

	
		// Write out room information
		if(ubMinorMapVersion < 29){
			UINT8 tmproom;
			for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
			{
				tmproom = (UINT8)gusWorldRoomInfo[ cnt ];
				FileWrite( hfile, &tmproom, sizeof( UINT8 ), &uiBytesWritten );
			}
		}else{
			for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
			{
				FileWrite( hfile, &gusWorldRoomInfo[ cnt ], sizeof( UINT16 ), &uiBytesWritten );
			}
		}
	

	if ( uiFlags & MAP_WORLDITEMS_SAVED )
	{
		// Write out item information
		SaveWorldItemsToMap(hfile, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 150909
	}

	if( uiFlags & MAP_AMBIENTLIGHTLEVEL_SAVED )
	{
		FileWrite( hfile, &gfBasement, 1, &uiBytesWritten );
		FileWrite( hfile, &gfCaves, 1, &uiBytesWritten );
		FileWrite( hfile, &ubAmbientLightLevel, 1, &uiBytesWritten );
	}

	if( uiFlags & MAP_WORLDLIGHTS_SAVED )
	{
		SaveMapLights( hfile );
	}
#if 0//dnl ch74 191013 from 050611 Scheinworld reported problem with basement levels and similar maps which doesn't need entry points so decide to remove this check completely
	if(gMapInformation.sCenterGridNo == -1 || gMapInformation.ubEditorSmoothingType == SMOOTHING_NORMAL && (gMapInformation.sNorthGridNo == -1 && gMapInformation.sEastGridNo == -1 && gMapInformation.sSouthGridNo == -1 && gMapInformation.sWestGridNo == -1))//dnl ch17 290909
	{
		swprintf(gzErrorCatchString, L"SAVE ABORTED! Center and at least one of (N,S,E,W) entry point should be set.");
		gfErrorCatch = TRUE;
		FileClose(hfile);
		return(FALSE);
	}
#endif
	gMapInformation.ubMapVersion = ubMinorMapVersion;//dnl ch74 241013 all this years MapInfo saves incorrect version :-(
	SaveMapInformation(hfile, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 150909

	if( uiFlags & MAP_FULLSOLDIER_SAVED )
	{
		SaveSoldiersToMap(hfile, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 150909
	}
	if( uiFlags & MAP_EXITGRIDS_SAVED )
	{
		SaveExitGrids(hfile, usNumExitGrids, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 240909
	}
	if( uiFlags & MAP_DOORTABLE_SAVED )
	{
		SaveDoorTableToMap(hfile, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 240909
	}
	if( uiFlags & MAP_EDGEPOINTS_SAVED )
	{
		CompileWorldMovementCosts();
		GenerateMapEdgepoints(TRUE);//dnl ch43 290909
		SaveMapEdgepoints(hfile, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 240909
	}
	if( uiFlags & MAP_NPCSCHEDULES_SAVED )
	{
		SaveSchedules(hfile, dMajorMapVersion, ubMinorMapVersion);//dnl ch33 240909
	}

	FileClose( hfile );

	sprintf( gubFilename, puiFilename );


	for(i = 0; i<WORLD_MAX; i++)
		MemFree(bCounts[i]);
	MemFree(bCounts);

#endif //JA2EDITOR

	return( TRUE );
}


#define NUM_DIR_SEARCHES				5
INT8	bDirectionsForShadowSearch[ NUM_DIR_SEARCHES ] =
{
	WEST,
	SOUTHWEST,
	SOUTH,
	SOUTHEAST,
	EAST
};

void OptimizeMapForShadows( )
{
	INT32 cnt, dir;
	INT32 sNewGridNo;
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// CHECK IF WE ARE A TREE HERE
		if ( IsTreePresentAtGridNo( cnt ) )
		{
			// CHECK FOR A STRUCTURE A FOOTPRINT AWAY
			for ( dir = 0; dir < NUM_DIR_SEARCHES; dir++ )
			{
				sNewGridNo = NewGridNo( cnt, (UINT16)DirectionInc( bDirectionsForShadowSearch[ dir ] ) );

				if ( gpWorldLevelData[ sNewGridNo ].pStructureHead == NULL )
				{
					break;
				}
			}
			// If we made it here, remove shadow!
			// We're full of structures
			if ( dir == NUM_DIR_SEARCHES )
			{
				RemoveAllShadows( cnt );
				// Display message to effect
			}
		}
	}
}

void SetBlueFlagFlags( void )
{
	INT32		cnt;
	LEVELNODE * pNode;

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		pNode = gpWorldLevelData[ cnt ].pStructHead;
		while ( pNode )
		{
			if ( pNode->usIndex == BLUEFLAG_GRAPHIC)
			{
				gpWorldLevelData[ cnt ].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;
				break;
			}
			pNode = pNode->pNext;
		}
	}
}

void InitLoadedWorld( )
{
#ifndef JA2EDITOR//dnl ch85 030214 editor allows to load any map so rather skip this check
	//if the current sector is not valid, dont init the world
	if( gWorldSectorX == 0 || gWorldSectorY == 0 )
	{
		return;
	}
#endif
	// COMPILE MOVEMENT COSTS
	CompileWorldMovementCosts( );

	// COMPILE INTERACTIVE TILES
	CompileInteractiveTiles( );

	// COMPILE WORLD VISIBLIY TILES
	CalculateWorldWireFrameTiles( TRUE );

	LightSpriteRenderAll();

	OptimizeMapForShadows( );

	SetInterfaceHeightLevel( );

	// ATE: if we have a slide location, remove it!
	gTacticalStatus.sSlideTarget = NOWHERE;

	SetBlueFlagFlags();
}

#ifdef JA2EDITOR
//This is a specialty function that is very similar to LoadWorld, except that it
//doesn't actually load the world, it instead evaluates the map and generates summary
//information for use within the summary editor.  The header is defined in Summary Info.h,
//not worlddef.h -- though it's not likely this is going to be used anywhere where it would
//matter.
extern double MasterStart, MasterEnd;
extern BOOLEAN gfUpdatingNow;
//dnl ch28 250909
BOOLEAN EvaluateWorld(STR8 pSector, UINT8 ubLevel)
{
	FLOAT dMajorMapVersion;
	SUMMARYFILE *pSummary;
	HWFILE hfile;
	INT8 *pBuffer, *pBufferHead;
	UINT32 uiFlags, uiFileSize, uiBytesRead;
	INT32 i, cnt, iTilesetID;
	CHAR16 str[2*FILENAME_BUFLEN];
	CHAR8 szDirFilename[2*FILENAME_BUFLEN], szFilename[2*FILENAME_BUFLEN];//dnl ch81 021213
	UINT8 ubCombine, ubMinorMapVersion;
	UINT8 (*bCounts)[8] = NULL;
	// Make sure the file exists... if not, then return false
	sprintf(szFilename, pSector);
	if(ubLevel % 4)
		sprintf(szFilename+strlen(szFilename), "_b%d", ubLevel%4);
	if(ubLevel >= 4)
		strcat(szFilename, "_a");
	strcat(szFilename, ".dat");
	CHAR16 szFileName[FILENAME_BUFLEN];//dnl ch81 021213
	swprintf(szFileName, L"%S", pSector);
	if(ValidMapFileName(szFileName))
		strcpy(szFilename, pSector);
	sprintf(szDirFilename, "MAPS\\%s", szFilename);
#ifdef USE_VFS//dnl ch81 021213
	if(guiCurrentScreen == LOADSAVE_SCREEN)
		hfile = FileOpen(szDirFilename, FILE_ACCESS_READ, FALSE, gzProfileName);
	else
		hfile = FileOpen(szDirFilename, FILE_ACCESS_READ);
#else
	hfile = FileOpen(szDirFilename, FILE_ACCESS_READ, FALSE);
#endif
	if(!hfile)
		return(FALSE);
	uiFileSize = FileGetSize(hfile);
	pBuffer = (INT8*)MemAlloc(uiFileSize);
	pBufferHead = pBuffer;
	FileRead(hfile, pBuffer, uiFileSize, &uiBytesRead);
	FileClose(hfile);
	swprintf(str, L"Analyzing map %S", szFilename);
	if(!gfUpdatingNow)
		SetRelativeStartAndEndPercentage(0, 0, 100, str);
	else
		SetRelativeStartAndEndPercentage(0, (UINT16)MasterStart, (UINT16)MasterEnd, str);
	RenderProgressBar(0, 0);
	//clear the summary file info
	pSummary = (SUMMARYFILE*)MemAlloc(sizeof(SUMMARYFILE));
	Assert(pSummary);
	memset(pSummary, 0, sizeof(SUMMARYFILE));
	pSummary->ubSummaryVersion = GLOBAL_SUMMARY_VERSION;
	LOADDATA(&dMajorMapVersion, pBuffer, sizeof(FLOAT));
	pSummary->dMajorMapVersion = dMajorMapVersion;
	if(dMajorMapVersion >= 4.00)
		LOADDATA(&ubMinorMapVersion, pBuffer, sizeof(UINT8));
	INT32 iCurRowSize = WORLD_ROWS;
	INT32 iCurColSize = WORLD_COLS;
	if(dMajorMapVersion < 7.00)
	{
		WORLD_ROWS = OLD_WORLD_ROWS;
		WORLD_COLS = OLD_WORLD_COLS;
	}
	else
	{
		INT32 iRowSize, iColSize;
		LOADDATA(&iRowSize, pBuffer, sizeof(INT32));
		LOADDATA(&iColSize, pBuffer, sizeof(INT32));
		WORLD_ROWS = iRowSize;
		WORLD_COLS = iColSize;
	}
	gMapTrn.DisableTrn();//dnl ch44 290909
	// Read FLAGS FOR WORLD
	LOADDATA(&uiFlags, pBuffer, sizeof(INT32));
	// Read tilesetID
	LOADDATA(&iTilesetID, pBuffer, sizeof(INT32));
	pSummary->ubTilesetID = iTilesetID;
	// Skip soldier size
	pBuffer += sizeof(INT32);
	// Skip height values
	pBuffer += sizeof(INT16) * WORLD_MAX;
	bCounts = (UINT8(*)[8])MemAlloc(WORLD_MAX*8);
	memset(bCounts, 0, sizeof(UINT8)*WORLD_MAX*8);
	// Read layer counts
	for(cnt=0; cnt<WORLD_MAX; cnt++)
	{
		if(!(cnt % 2560))
			RenderProgressBar(0, (cnt/2560)+1);// 1 - 10
		// Read combination of land/world flags
		LOADDATA(&ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][0] = (UINT8)(ubCombine & 0x0F);
		UINT16 uiFlags = (UINT8)((ubCombine & 0xF0) >> 4);//gpWorldLevelData[cnt].uiFlags |= (UINT8)((ubCombine & 0xF0) >> 4);
		// Read #objects, structs
		LOADDATA( &ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][1] = (UINT8)(ubCombine & 0x0F);
		bCounts[cnt][2] = (UINT8)((ubCombine & 0xF0) >> 4);
		// Read shadows, roof
		LOADDATA(&ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][3] = (UINT8)(ubCombine & 0x0F);
		bCounts[cnt][4] = (UINT8)((ubCombine & 0xF0) >> 4);
		// Read OnRoof, nothing
		LOADDATA(&ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][5] = (UINT8)(ubCombine & 0x0F);
		bCounts[cnt][6] = bCounts[cnt][0] + bCounts[cnt][1] + bCounts[cnt][2] + bCounts[cnt][3] + bCounts[cnt][4] + bCounts[cnt][5];
	}
	// Skip all layers
	for(cnt = 0; cnt<WORLD_MAX; cnt++)
	{
		if(!(cnt % 320))
			RenderProgressBar(0, (cnt/320)+11);// 11 - 90
		pBuffer += sizeof(UINT16) * bCounts[cnt][6];
		pBuffer += bCounts[cnt][1];
	}
	// Extract highest room number
	//DBrot: More Rooms
	UINT8 ubRoomNum;
	UINT16	usRoomNum;
	if(ubMinorMapVersion < 29){
	for(cnt=0; cnt<WORLD_MAX; cnt++)
	{
		LOADDATA(&ubRoomNum, pBuffer, sizeof(ubRoomNum));
		if(ubRoomNum > pSummary->ubNumRooms)
			pSummary->ubNumRooms = ubRoomNum;
	}
	}else{
		for(cnt=0; cnt<WORLD_MAX; cnt++)
		{
			LOADDATA(&usRoomNum, pBuffer, sizeof(usRoomNum));
			if(usRoomNum > pSummary->ubNumRooms)
				pSummary->ubNumRooms = usRoomNum;
		}
	}

	if(uiFlags & MAP_WORLDITEMS_SAVED)
	{
		RenderProgressBar(0, 91);
		// Get number of items
		UINT32 temp;
		LOADDATA(&temp, pBuffer, 4);
		pSummary->usNumItems = temp;
		pSummary->uiNumItemsPosition = pBuffer - pBufferHead - 4;
		// Skip the contents of the world items
		WORLDITEM dummyItem;
		for(i=0; i<(INT32)pSummary->usNumItems; i++)
			dummyItem.Load(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
	}

	if(uiFlags & MAP_AMBIENTLIGHTLEVEL_SAVED)
		pBuffer += 3;

	if(uiFlags & MAP_WORLDLIGHTS_SAVED)
	{
		RenderProgressBar(0, 92);
		// Skip number of light palette entries
		UINT8 ubTemp;
		LOADDATA(&ubTemp, pBuffer, sizeof(UINT8));
		pBuffer += sizeof(SGPPaletteEntry) * ubTemp;
		// Get number of lights
		UINT16 usNumLights;
		LOADDATA(&usNumLights, pBuffer, sizeof(UINT16));
		pSummary->usNumLights = usNumLights;
		// Skip the light loading
		for(cnt=0; cnt<(INT32)pSummary->usNumLights; cnt++)
		{
			UINT8 ubStrLen;
			pBuffer += sizeof(LIGHT_SPRITE);
			LOADDATA(&ubStrLen, pBuffer, sizeof(UINT8));
			if(ubStrLen)
				pBuffer += ubStrLen;
		}
	}

	// Read Map Information
	pSummary->MapInfo.Load(&pBuffer, dMajorMapVersion);
	if(pSummary->MapInfo.ubMapVersion != ubMinorMapVersion)//dnl ch74 241013 stupid fix because MapInfo is incorrectly saved in all previous mapeditor if you edit existing map
		pSummary->MapInfo.ubMapVersion = ubMinorMapVersion;

	if(uiFlags & MAP_FULLSOLDIER_SAVED)
	{
		RenderProgressBar(0, 94);
		BASIC_SOLDIERCREATE_STRUCT basic;
		SOLDIERCREATE_STRUCT priority;
		TEAMSUMMARY *pTeam = NULL;
		pSummary->uiEnemyPlacementPosition = pBuffer - pBufferHead;
		for(i=0; i<pSummary->MapInfo.ubNumIndividuals ; i++)
		{
			basic.Load(&pBuffer, dMajorMapVersion);
			switch(basic.bTeam)
			{
			case ENEMY_TEAM:
				pTeam = &pSummary->EnemyTeam;
				break;
			case CREATURE_TEAM:
				pTeam = &pSummary->CreatureTeam;
				break;
			case MILITIA_TEAM:
				pTeam = &pSummary->RebelTeam;
				break;
			case CIV_TEAM:
				pTeam = &pSummary->CivTeam;
				break;
			}
			if(basic.bOrders == RNDPTPATROL || basic.bOrders == POINTPATROL)
			{
				//make sure the placement has at least one waypoint.
				if(!basic.bPatrolCnt)
					pSummary->ubEnemiesReqWaypoints++;
			}
			else if(basic.bPatrolCnt)
				pSummary->ubEnemiesHaveWaypoints++;
			if(basic.fPriorityExistance)
				pTeam->ubExistance++;
			switch(basic.bRelativeAttributeLevel)
			{
			case 0:
				pTeam->ubBadA++;
				break;
			case 1:
				pTeam->ubPoorA++;
				break;
			case 2:
				pTeam->ubAvgA++;
				break;
			case 3:
				pTeam->ubGoodA++;
				break;
			case 4:
				pTeam->ubGreatA++;
				break;
			}
			switch(basic.bRelativeEquipmentLevel)
			{
			case 0:
				pTeam->ubBadE++;
				break;
			case 1:
				pTeam->ubPoorE++;
				break;
			case 2:
				pTeam->ubAvgE++;	
				break;
			case 3:
				pTeam->ubGoodE++;
				break;
			case 4:
				pTeam->ubGreatE++;	
				break;
			}
			if(basic.fDetailedPlacement)
			{
				// Skip static priority placement, Clean up inventory handling
				priority.Load(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
				if(priority.ubProfile != NO_PROFILE)
					pTeam->ubProfile++;
				else
					pTeam->ubDetailed++;
				if(basic.bTeam == CIV_TEAM)
				{
					if(priority.ubScheduleID)
						pSummary->ubCivSchedules++;
					if(priority.bBodyType == COW)
						pSummary->ubCivCows++;
					else if(priority.bBodyType == BLOODCAT)
						pSummary->ubCivBloodcats++;
				}
			}
			if(basic.bTeam == ENEMY_TEAM)
			{
				switch(basic.ubSoldierClass)
				{
				case SOLDIER_CLASS_ADMINISTRATOR:
					pSummary->ubNumAdmins++;
					if(basic.fPriorityExistance)
						pSummary->ubAdminExistance++;
					if(basic.fDetailedPlacement)
					{
						if(priority.ubProfile != NO_PROFILE)
							pSummary->ubAdminProfile++;
						else
							pSummary->ubAdminDetailed++;
					}
					break;
				case SOLDIER_CLASS_ELITE:
					pSummary->ubNumElites++;
					if(basic.fPriorityExistance)
						pSummary->ubEliteExistance++;
					if(basic.fDetailedPlacement)
					{
						if(priority.ubProfile != NO_PROFILE)
							pSummary->ubEliteProfile++;
						else
							pSummary->ubEliteDetailed++;
					}
					break;
				case SOLDIER_CLASS_ARMY:
					pSummary->ubNumTroops++;
					if(basic.fPriorityExistance)
						pSummary->ubTroopExistance++;
					if(basic.fDetailedPlacement)
					{
						if(priority.ubProfile != NO_PROFILE)
							pSummary->ubTroopProfile++;
						else
							pSummary->ubTroopDetailed++;
					}
					break;
				}
			}
			else if(basic.bTeam == CREATURE_TEAM)
			{
				if(basic.bBodyType == BLOODCAT)
					pTeam->ubNumAnimals++;
			}
			pTeam->ubTotal++;
		}
		RenderProgressBar(0, 96);
	}

	if(uiFlags & MAP_EXITGRIDS_SAVED)
	{
		RenderProgressBar(0, 98);
		BOOLEAN fExitGridFound;
		UINT16 usNumExitGrids;
		UINT32 usMapIndex;
		EXITGRID ExitGrid;
		LOADDATA(&usNumExitGrids, pBuffer, sizeof(usNumExitGrids));
		for(i=0; i<usNumExitGrids; i++)
		{
			if(dMajorMapVersion < 7.0)
			{
				UINT16 usOldMapIndex;
				LOADDATA(&usOldMapIndex, pBuffer, sizeof(usOldMapIndex));
				usMapIndex = usOldMapIndex;
			}
			else
				LOADDATA(&usMapIndex, pBuffer, sizeof(usMapIndex));
			ExitGrid.Load(&pBuffer, dMajorMapVersion);
			fExitGridFound = FALSE;
			for(cnt=0; cnt<pSummary->ubNumExitGridDests; cnt++)
			{
				if(pSummary->ExitGrid[cnt].usGridNo == ExitGrid.usGridNo && pSummary->ExitGrid[cnt].ubGotoSectorX == ExitGrid.ubGotoSectorX && pSummary->ExitGrid[cnt].ubGotoSectorY == ExitGrid.ubGotoSectorY && pSummary->ExitGrid[cnt].ubGotoSectorZ == ExitGrid.ubGotoSectorZ)
				{
					// Same destination
					pSummary->usExitGridSize[cnt]++;
					fExitGridFound = TRUE;
					break;
				}
			}
			if(!fExitGridFound)
			{
				if(cnt >= 4)
					pSummary->fTooManyExitGridDests = TRUE;
				else
				{
					pSummary->ubNumExitGridDests++;
					pSummary->usExitGridSize[cnt]++;
					pSummary->ExitGrid[cnt].usGridNo = ExitGrid.usGridNo;
					pSummary->ExitGrid[cnt].ubGotoSectorX = ExitGrid.ubGotoSectorX;
					pSummary->ExitGrid[cnt].ubGotoSectorY = ExitGrid.ubGotoSectorY;
					pSummary->ExitGrid[cnt].ubGotoSectorZ = ExitGrid.ubGotoSectorZ;
					if(pSummary->ExitGrid[cnt].ubGotoSectorX != ExitGrid.ubGotoSectorX || pSummary->ExitGrid[cnt].ubGotoSectorY != ExitGrid.ubGotoSectorY)
						pSummary->fInvalidDest[cnt] = TRUE;
				}
			}
		}
	}

	if(uiFlags & MAP_DOORTABLE_SAVED)
	{
		DOOR Door;
		UINT8 ubNumDoors;
		LOADDATA(&ubNumDoors, pBuffer, sizeof(gubNumDoors));
		pSummary->ubNumDoors = ubNumDoors;
		for(cnt=0; cnt<pSummary->ubNumDoors; cnt++)
		{
			Door.Load(&pBuffer, dMajorMapVersion);
			if(Door.ubTrapID && Door.ubLockID)
				pSummary->ubNumDoorsLockedAndTrapped++;
			else if(Door.ubLockID)
				pSummary->ubNumDoorsLocked++;
			else if(Door.ubTrapID)
				pSummary->ubNumDoorsTrapped++;
		}
	}

	RenderProgressBar(0, 100);
	MemFree(pBufferHead);
	MemFree(bCounts);
	WriteSectorSummaryUpdate(szFilename, ubLevel, pSummary);
	WORLD_ROWS = iCurRowSize;
	WORLD_COLS = iCurColSize;
	return(TRUE);
}
#endif

extern UINT8 GetCurrentSummaryVersion();
extern void LoadShadeTablesFromTextFile();
BOOLEAN LoadWorld(const STR8 puiFilename, FLOAT* pMajorMapVersion, UINT8* pMinorMapVersion)//dnl ch44 290909
{
	HWFILE					hfile;
	FLOAT					dMajorMapVersion;
	UINT32					uiFlags;
	UINT32					uiBytesRead;
	UINT32					uiSoldierSize;
	UINT32					uiFileSize;
#ifdef JA2TESTVERSION
	UINT32					uiStartTime;
	UINT32					uiLoadWorldStartTime;
#endif
	INT32					cnt, cnt2;
	INT32					iTilesetID;
	UINT16					usTileIndex;
	UINT16					usTypeSubIndex;
	UINT8					ubType;
	UINT8					ubSubIndex;
	CHAR8					aFilename[2*FILENAME_BUFLEN];//dnl ch81 021213
	UINT8					ubCombine;
	UINT8					(*bCounts)[8] = NULL;
	INT8					*pBuffer;
	INT8					*pBufferHead;
	BOOLEAN					fGenerateEdgePoints = FALSE;
	UINT8					ubMinorMapVersion;
	INT32					i;

#ifdef JA2TESTVERSION
	uiLoadWorldStartTime = GetJA2Clock();
#endif

	LoadShadeTablesFromTextFile();

	// Append exension to filename!
	if(gfForceLoad)
		sprintf(aFilename, "MAPS\\%s", gzForceLoadFile);
	else
		sprintf(aFilename, "MAPS\\%s", puiFilename);
	// Open file
#ifdef USE_VFS//dnl ch81 021213
#ifdef JA2EDITOR
	if(guiCurrentScreen == LOADSAVE_SCREEN)
		hfile = FileOpen(aFilename, FILE_ACCESS_READ, FALSE, gzProfileName);
	else
		hfile = FileOpen(aFilename, FILE_ACCESS_READ);
#else
	hfile = FileOpen(aFilename, FILE_ACCESS_READ);
#endif
#else
	hfile = FileOpen(aFilename, FILE_ACCESS_READ, FALSE);
#endif
	if(!hfile)
	{
#ifndef JA2EDITOR
		SET_ERROR("Could not load map file %S", aFilename);
#endif
		return(FALSE);
	}
	// Get the file size and alloc one huge buffer for it. We will use this buffer to transfer all of the data from.
	uiFileSize = FileGetSize(hfile);
	pBuffer = (INT8*)MemAlloc(uiFileSize);
	pBufferHead = pBuffer;
	FileRead(hfile, pBuffer, uiFileSize, &uiBytesRead);
	FileClose(hfile);

	// RESET FLAGS FOR OUTDOORS/INDOORS
	gfBasement = FALSE;
	gfCaves = FALSE;

	// Flugente: select loadscreen hint
	if(!gfEditMode)//dnl ch74 211013
		SetNewLoadScreenHint();

	SetRelativeStartAndEndPercentage(0, 0, 1, L"Trashing world...");
#ifdef JA2TESTVERSION
	uiStartTime = GetJA2Clock();
#endif
	TrashWorld();
#ifdef JA2TESTVERSION
	uiTrashWorldTime = GetJA2Clock() - uiStartTime;
#endif
	LightReset();

	// Read JA2 Version ID
	LOADDATA(&dMajorMapVersion, pBuffer, sizeof(FLOAT));
	LOADDATA(&ubMinorMapVersion, pBuffer, sizeof(UINT8));
	if(pMajorMapVersion && pMinorMapVersion)//dnl ch79 291113
	{
		*pMajorMapVersion = dMajorMapVersion;
		*pMinorMapVersion = ubMinorMapVersion;
	}
	INT32 iRowSize = OLD_WORLD_ROWS;
	INT32 iColSize = OLD_WORLD_COLS;
	if(dMajorMapVersion >= 7.00)
	{
		LOADDATA(&iRowSize, pBuffer, sizeof(INT32));
		LOADDATA(&iColSize, pBuffer, sizeof(INT32));
	}

	// Actual world size of the map we loaded!
	INT32 iWorldSize = iRowSize * iColSize;

#ifdef JA2EDITOR
	// TODO.MAP: Make a new checkbox "Lock Rows & Cols". If set, load the map like it is now, with the set ROWS and COLS
	// If not set, load the map with the original rows and cols

	// WANNE: Only resize the map, when the checkbox is set
	if(gfResizeMapOnLoading && gMapTrn.SetTrnPar(iRowSize, iColSize, iNewMapWorldRows, iNewMapWorldCols))
	{
		// We enlarge the map
		SetWorldSize(iNewMapWorldRows, iNewMapWorldCols);

		// Uncheck "vanilla map saving", because it is not allowed on maps > 160x160
		gfVanillaMode = FALSE;//dnl ch74 191013
		UnclickEditorButton(OPTIONS_VANILLA_MODE);

		// Reset
		gfResizeMapOnLoading = FALSE;
	}
	else
	{
		// Mapsize has not changed
		SetWorldSize(iRowSize, iColSize);

		// We still have the "normal" map size AND the map is saved in vanilla format
		if ((iRowSize <= OLD_WORLD_ROWS && iRowSize <= OLD_WORLD_COLS) && (dMajorMapVersion == VANILLA_MAJOR_MAP_VERSION && ubMinorMapVersion == VANILLA_MINOR_MAP_VERSION))
		{
			// Check "vanilla map saving"
			gfVanillaMode = TRUE;//dnl ch74 191013
			ClickEditorButton(OPTIONS_VANILLA_MODE);
		}
		else
		{
			// Uncheck "vanilla map saving"
			gfVanillaMode = FALSE;//dnl ch74 191013
			UnclickEditorButton(OPTIONS_VANILLA_MODE);
		}
	}

	// Always reset to unchecked!
	UnclickEditorButton(OPTIONS_RESIZE_MAP_ON_LOADING);

#else
	SetWorldSize(iRowSize, iColSize);
#endif

	bCounts = (UINT8(*)[8])MemAlloc(WORLD_MAX*8);
	memset(bCounts, 0, sizeof(UINT8)*WORLD_MAX*8);

	// Read FLAGS FOR WORLD
	LOADDATA(&uiFlags, pBuffer, sizeof(INT32));

	LOADDATA(&iTilesetID, pBuffer, sizeof(INT32));

#ifdef JA2TESTVERSION
	uiStartTime = GetJA2Clock();
#endif
	Assert(LoadMapTileset(iTilesetID));
#ifdef JA2TESTVERSION
	uiLoadMapTilesetTime = GetJA2Clock() - uiStartTime;
#endif

	// Load soldier size
	LOADDATA(&uiSoldierSize, pBuffer, sizeof(INT32));
	// Set land index
	for(cnt=0; cnt<WORLD_MAX; cnt++)
	{
		if(!gMapTrn.IsTrn())
			break;
		if(GetTerrainType(cnt) == NO_TERRAIN)
			AddLandToHead(cnt, (UINT16)(rand()%10));
	}

	// Read height values
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		LOADDATA(&gpWorldLevelData[cnt].sHeight, pBuffer, sizeof(INT16));
		// Remove all land from position where old map will be loaded
		if(gMapTrn.IsTrn())
			RemoveAllLandsOfTypeRange(cnt, FIRSTTEXTURE, DEEPWATERTEXTURE);
	}

	SetRelativeStartAndEndPercentage(0, 35, 40, L"Counting layers...");
	RenderProgressBar(0, 100);
	// Read layer counts
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		// Read combination of land/world flags
		LOADDATA(&ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][0] = (UINT8)(ubCombine & 0x0F);
		gpWorldLevelData[cnt].uiFlags |= (UINT8)((ubCombine & 0xF0) >> 4);
		// Read #objects, structs
		LOADDATA( &ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][1] = (UINT8)(ubCombine & 0x0F);
		bCounts[cnt][2] = (UINT8)((ubCombine & 0xF0) >> 4);
		// Read shadows, roof
		LOADDATA( &ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][3] = (UINT8)(ubCombine & 0x0F);
		bCounts[cnt][4] = (UINT8)((ubCombine & 0xF0) >> 4);
		// Read OnRoof, nothing
		LOADDATA(&ubCombine, pBuffer, sizeof(UINT8));
		// Split
		bCounts[cnt][5] = (UINT8)(ubCombine & 0x0F);
	}

	SetRelativeStartAndEndPercentage(0, 40, 43, L"Loading land layers...");
	RenderProgressBar(0, 100);
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
 		// Read new values
		for(cnt2=0; cnt2<bCounts[cnt][0]; cnt2++)
		{
			LOADDATA(&ubType, pBuffer, sizeof(UINT8));
			LOADDATA(&ubSubIndex, pBuffer, sizeof(UINT8));
			// Get tile index
			GetTileIndexFromTypeSubIndex(ubType, ubSubIndex, &usTileIndex);
			// Add layer
			AddLandToHead(cnt, usTileIndex);
		}
	}

	SetRelativeStartAndEndPercentage(0, 43, 46, L"Loading object layer...");
	RenderProgressBar(0, 100);
	// New load require UINT16 for the type subindex due to the fact that ROADPIECES contain over 300 type subindices.
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		// Set objects
		for(cnt2=0; cnt2<bCounts[cnt][1]; cnt2++)
		{
			LOADDATA(&ubType, pBuffer, sizeof(UINT8));
			LOADDATA(&usTypeSubIndex, pBuffer, sizeof(UINT16));
			if(ubType >= FIRSTPOINTERS)
				continue;
			// Get tile index
			GetTileIndexFromTypeSubIndex(ubType, usTypeSubIndex, &usTileIndex);
			// Add layer
			AddObjectToTail(cnt, usTileIndex);
		}
	}

	SetRelativeStartAndEndPercentage(0, 46, 49, L"Loading struct layer...");
	RenderProgressBar(0, 100);
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		// Set structs
		for(cnt2=0; cnt2<bCounts[cnt][2]; cnt2++)
		{
			LOADDATA(&ubType, pBuffer, sizeof(UINT8));
			LOADDATA(&ubSubIndex, pBuffer, sizeof(UINT8));
			// Get tile index
			GetTileIndexFromTypeSubIndex(ubType, ubSubIndex, &usTileIndex);
			if(ubMinorMapVersion <= 25)
			{
				// Check patching for phantom menace struct data...
				if(gTileDatabase[usTileIndex].uiFlags & UNDERFLOW_FILLER)
					GetTileIndexFromTypeSubIndex(ubType, 1, &usTileIndex);
			}
			// Add layer
			AddStructToTail(cnt, usTileIndex);
		}
	}

	SetRelativeStartAndEndPercentage(0, 49, 52, L"Loading shadow layer...");
	RenderProgressBar(0, 100);
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		for(cnt2=0; cnt2<bCounts[cnt][3]; cnt2++)
		{
			LOADDATA(&ubType, pBuffer, sizeof(UINT8));
			LOADDATA(&ubSubIndex, pBuffer, sizeof(UINT8));
			// Get tile index
			GetTileIndexFromTypeSubIndex(ubType, ubSubIndex, &usTileIndex);
			// Add layer
			AddShadowToTail(cnt, usTileIndex);
		}
	}

	SetRelativeStartAndEndPercentage(0, 52, 55, L"Loading roof layer...");
	RenderProgressBar(0, 100);
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		for(cnt2=0; cnt2<bCounts[cnt][4]; cnt2++)
		{
			LOADDATA(&ubType, pBuffer, sizeof(UINT8));
			LOADDATA(&ubSubIndex, pBuffer, sizeof(UINT8));
			// Get tile index
			GetTileIndexFromTypeSubIndex(ubType, ubSubIndex, &usTileIndex);
			// Add layer
			AddRoofToTail(cnt, usTileIndex);
		}
	}

	SetRelativeStartAndEndPercentage(0, 55, 58, L"Loading on roof layer...");
	RenderProgressBar(0, 100);
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		for(cnt2=0; cnt2<bCounts[cnt][5]; cnt2++)
		{
			LOADDATA(&ubType, pBuffer, sizeof(UINT8));
			LOADDATA(&ubSubIndex, pBuffer, sizeof(UINT8));
			// Get tile index
			GetTileIndexFromTypeSubIndex(ubType, ubSubIndex, &usTileIndex);
			// Add layer
			AddOnRoofToTail(cnt, usTileIndex);
		}
	}

	// For old Russian Maps which have version 6.0
	if(dMajorMapVersion == 6.00 && ubMinorMapVersion < 27)
	{
		UINT32 uiNums[37];
		LOADDATA(uiNums, pBuffer, sizeof(INT32)*37);
		dMajorMapVersion = 5.00;
	}
	// For new maps
	else if(dMajorMapVersion == 6.00 && ubMinorMapVersion < 27 && MAJOR_MAP_VERSION == 6.00)
	{
		UINT32 uiNums[37];
		LOADDATA(uiNums, pBuffer, sizeof(UINT32)*37);
	}
	//now the data is discarded and when saved, as 6.27, you won't have this problem!

	SetRelativeStartAndEndPercentage(0, 58, 59, L"Loading room information...");
	RenderProgressBar(0, 100);
#ifdef JA2EDITOR
	gusMaxRoomNumber = 0;
	//DBrot: More Rooms
	for(i=0; i<iWorldSize; i++)
	{
		gMapTrn.GetTrnCnt(cnt=i);
		// Read room information, 2 byte for new files
		if(ubMinorMapVersion < 29){
			LOADDATA(&gusWorldRoomInfo[cnt], pBuffer, sizeof(INT8));
		}else{
			LOADDATA(&gusWorldRoomInfo[cnt], pBuffer, sizeof(UINT16));
		}
		
		// Got to set the max room number
		if(gusWorldRoomInfo[cnt] > gusMaxRoomNumber)
			gusMaxRoomNumber = gusWorldRoomInfo[cnt];
	}
	if(gusMaxRoomNumber < 65535)
		gusMaxRoomNumber++;
#else
	for(i=0; i<iWorldSize; i++){
		gMapTrn.GetTrnCnt(cnt=i);
		// Read room information, 2 byte for new files
		if(ubMinorMapVersion < 29){
			LOADDATA(&gusWorldRoomInfo[cnt], pBuffer, sizeof(INT8));
		}else{
			LOADDATA(&gusWorldRoomInfo[cnt], pBuffer, sizeof(UINT16));
		}
	}
	//LOADDATA(gubWorldRoomInfo, pBuffer, sizeof(INT8)*WORLD_MAX);
#endif
	memset(gubWorldRoomHidden, TRUE, sizeof(gubWorldRoomHidden));

	SetRelativeStartAndEndPercentage(0, 59, 61, L"Loading items...");
	RenderProgressBar(0, 100);
	if(uiFlags & MAP_WORLDITEMS_SAVED)
	{
		// Load out item information
		gfLoadPitsWithoutArming = TRUE;
		LoadWorldItemsFromMap(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
		gfLoadPitsWithoutArming = FALSE;
	}

	SetRelativeStartAndEndPercentage(0, 62, 85, L"Loading lights...");
	RenderProgressBar(0, 0);
	if(uiFlags & MAP_AMBIENTLIGHTLEVEL_SAVED)
	{
		//Ambient light levels are only saved in underground levels
		LOADDATA(&gfBasement, pBuffer, sizeof(BOOLEAN));
		LOADDATA(&gfCaves, pBuffer, sizeof(BOOLEAN));
		LOADDATA(&ubAmbientLightLevel, pBuffer, sizeof(UINT8));
	}
	else
	{
		//We are above ground.
		gfBasement = FALSE;
		gfCaves = FALSE;
		if(!gfEditMode && guiCurrentScreen != MAPUTILITY_SCREEN)
			ubAmbientLightLevel = GetTimeOfDayAmbientLightLevel();
		else
			ubAmbientLightLevel = 4;
	}
#ifdef JA2TESTVERSION
	uiStartTime = GetJA2Clock();
#endif
	if(uiFlags & MAP_WORLDLIGHTS_SAVED)
		LoadMapLights(&pBuffer);
	else
	{
		// Set some default value for lighting
		SetDefaultWorldLightingColors();
	}
	LightSetBaseLevel(ubAmbientLightLevel);
#ifdef JA2TESTVERSION
	uiLoadMapLightsTime = GetJA2Clock() - uiStartTime;
#endif

	SetRelativeStartAndEndPercentage(0, 85, 86, L"Loading map information...");
	RenderProgressBar(0, 0);
	LoadMapInformation(&pBuffer, dMajorMapVersion);
	// Translation routine for map grid numbers
	gMapTrn.GetTrnCnt(gMapInformation.sCenterGridNo);
	gMapTrn.GetTrnCnt(gMapInformation.sIsolatedGridNo);
	gMapTrn.GetTrnCnt(gMapInformation.sNorthGridNo);
	gMapTrn.GetTrnCnt(gMapInformation.sEastGridNo);
	gMapTrn.GetTrnCnt(gMapInformation.sWestGridNo);
	gMapTrn.GetTrnCnt(gMapInformation.sSouthGridNo);

	if(uiFlags & MAP_FULLSOLDIER_SAVED)
	{
		SetRelativeStartAndEndPercentage(0, 86, 87, L"Loading placements...");
		RenderProgressBar(0, 0);
		LoadSoldiersFromMap(&pBuffer, dMajorMapVersion, ubMinorMapVersion);
	}
	if(uiFlags & MAP_EXITGRIDS_SAVED)
	{
		SetRelativeStartAndEndPercentage(0, 87, 88, L"Loading exit grids...");
		RenderProgressBar(0, 0);
		LoadExitGrids(&pBuffer, dMajorMapVersion);
	}
	if(uiFlags & MAP_DOORTABLE_SAVED)
	{
		SetRelativeStartAndEndPercentage(0, 89, 90, L"Loading door tables...");
		RenderProgressBar(0, 0);
		LoadDoorTableFromMap(&pBuffer, dMajorMapVersion);
	}
	if(uiFlags & MAP_EDGEPOINTS_SAVED)
	{
		SetRelativeStartAndEndPercentage(0, 90, 91, L"Loading edgepoints...");
		RenderProgressBar(0, 0);
		if(!LoadMapEdgepoints(&pBuffer, dMajorMapVersion))
			fGenerateEdgePoints = TRUE;// Only if the map had the older edgepoint system
		//dnl ch44 290909 Map Edge points must be recreated they cannot be translated, so trash them.
		if(gMapTrn.IsTrn())
		{
			TrashMapEdgepoints();
			fGenerateEdgePoints = TRUE;
		}
	}
	else
		fGenerateEdgePoints = TRUE;
	if(uiFlags & MAP_NPCSCHEDULES_SAVED)
	{
		SetRelativeStartAndEndPercentage(0, 91, 92, L"Loading NPC schedules...");
		RenderProgressBar(0, 0);
		LoadSchedules(&pBuffer, dMajorMapVersion);
	}

	ValidateAndUpdateMapVersionIfNecessary();
	SetRelativeStartAndEndPercentage(0, 93, 94, L"Init Loaded World...");
	RenderProgressBar(0, 0);
	InitLoadedWorld();

	if(fGenerateEdgePoints)
	{
		SetRelativeStartAndEndPercentage(0, 94, 95, L"Generating map edgepoints...");
		RenderProgressBar(0, 0);
		CompileWorldMovementCosts();
		GenerateMapEdgepoints();
	}

	RenderProgressBar(0, 20);
	SetRelativeStartAndEndPercentage(0, 95, 100, L"General initialization...");
	// RESET AI!
	InitOpponentKnowledgeSystem();
	RenderProgressBar(0, 40);
	// Reset some override flags
	gfForceLoadPlayers = FALSE;
	gfForceLoad = FALSE;
	// CHECK IF OUR SELECTED GUY IS GONE!
	if(gusSelectedSoldier != NOBODY)
	{
		if(MercPtrs[gusSelectedSoldier]->bActive == FALSE)
			gusSelectedSoldier = NOBODY;
	}
	AdjustSoldierCreationStartValues();
	RenderProgressBar(0, 60);
	InvalidateWorldRedundency();
	// SAVE FILENAME
	strcpy(gzLastLoadedFile, puiFilename);
	LoadRadarScreenBitmap(puiFilename);
	RenderProgressBar(0, 80);
	sprintf(gubFilename, puiFilename);
	gfWorldLoaded = TRUE;
#ifdef JA2TESTVERSION
	uiLoadWorldTime = GetJA2Clock() - uiLoadWorldStartTime;
#endif
	// ATE: Not while updating maps!
	if(guiCurrentScreen != MAPUTILITY_SCREEN)
		GenerateBuildings();
	RenderProgressBar(0, 100);
	DequeueAllKeyBoardEvents();
	// Remove this rather large chunk of memory from the system now!
	MemFree(pBufferHead);
	MemFree(bCounts);
	return(TRUE);
}

//****************************************************************************************
//
//	Deletes everything then re-creates the world with simple ground tiles
//
//****************************************************************************************
BOOLEAN NewWorld( INT32 nMapRows,  INT32 nMapCols )
{
	UINT16				NewIndex;
	INT32					cnt;

	gusSelectedSoldier = gusOldSelectedSoldier = NOBODY;

	AdjustSoldierCreationStartValues( );

	TrashWorld();

	//SB
	SetWorldSize(nMapRows, nMapCols);

	// Create world randomly from tiles
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		// Set land index
		NewIndex = (UINT16)(rand( ) % 10);
		AddLandToHead( cnt, NewIndex );
	}


	InitRoomDatabase( );

	gfWorldLoaded = TRUE;

	return( TRUE );
}


void TrashWorld( void )
{
	MAP_ELEMENT		*pMapTile;
	LEVELNODE			*pLandNode;
	LEVELNODE			*pObjectNode;
	LEVELNODE			*pStructNode;
	LEVELNODE			*pShadowNode;
	LEVELNODE			*pMercNode;
	LEVELNODE			*pRoofNode;
	LEVELNODE			*pOnRoofNode;
	LEVELNODE			*pTopmostNode;
//	STRUCTURE			*pStructureNode;
	INT32					cnt;
	SOLDIERTYPE		*pSoldier;

	if( !gfWorldLoaded )
		return;


	// REMOVE ALL ITEMS FROM WORLD
	TrashWorldItems(  );

	// Trash the overhead map
	TrashOverheadMap( );

	//Reset the smoke effects.
	ResetSmokeEffects();

	//Reset the light effects
	ResetLightEffects();

	// Set soldiers to not active!
	//ATE: FOR NOW, ONLY TRASH FROM NPC UP!!!!
	//cnt = gTacticalStatus.Team[ gbPlayerNum ].bLastID + 1;
	cnt = 0;

	for ( pSoldier = MercPtrs[ cnt ]; cnt < MAX_NUM_SOLDIERS; pSoldier++, cnt++ )
	{
		if ( pSoldier->bActive )
		{
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				// Just delete levelnode
				pSoldier->pLevelNode = NULL;
			}
			else
			{
				// Delete from world
				TacticalRemoveSoldier( (UINT16)cnt );
			}
		}
	}

	// Reset attack busy since a militia might have been in the middle of radioing
	gTacticalStatus.ubAttackBusyCount = 0;

	RemoveCorpses( );

	// Remove all ani tiles...
	DeleteAniTiles( );


	//Kill both soldier init lists.
	UseEditorAlternateList();
	KillSoldierInitList();
	UseEditorOriginalList();
	KillSoldierInitList();

	//Remove the schedules
	DestroyAllSchedules();
#ifdef JA2UB
//Ja25 no meanwhiles
#else
	// on trash world sheck if we have to set up the first meanwhile
	HandleFirstMeanWhileSetUpWithTrashWorld( );
#endif
	// Create world randomly from tiles
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		pMapTile = &gpWorldLevelData[ cnt ];

		// Free the memory associated with the map tile link lists
		pLandNode = pMapTile->pLandHead;
		while ( pLandNode != NULL )
		{
			pMapTile->pLandHead = pLandNode->pNext;
			MemFree( pLandNode );
			pLandNode = pMapTile->pLandHead;
		}

		pObjectNode = pMapTile->pObjectHead;
		while ( pObjectNode != NULL )
		{
			pMapTile->pObjectHead = pObjectNode->pNext;
			MemFree( pObjectNode );
			pObjectNode = pMapTile->pObjectHead;
		}

		pStructNode = pMapTile->pStructHead;
		while ( pStructNode != NULL )
		{
			pMapTile->pStructHead = pStructNode->pNext;
			MemFree( pStructNode );
			pStructNode = pMapTile->pStructHead;
		}

		pShadowNode = pMapTile->pShadowHead;
		while ( pShadowNode != NULL )
		{
			pMapTile->pShadowHead = pShadowNode->pNext;
			MemFree( pShadowNode );
			pShadowNode = pMapTile->pShadowHead;
		}

		pMercNode = pMapTile->pMercHead;
		while ( pMercNode != NULL )
		{
			pMapTile->pMercHead = pMercNode->pNext;
			MemFree( pMercNode );
			pMercNode = pMapTile->pMercHead;
		}

		pRoofNode = pMapTile->pRoofHead;
		while ( pRoofNode != NULL )
		{
			pMapTile->pRoofHead = pRoofNode->pNext;
			MemFree( pRoofNode );
			pRoofNode = pMapTile->pRoofHead;
		}

		pOnRoofNode = pMapTile->pOnRoofHead;
		while ( pOnRoofNode != NULL )
		{
			pMapTile->pOnRoofHead = pOnRoofNode->pNext;
			MemFree( pOnRoofNode );
			pOnRoofNode = pMapTile->pOnRoofHead;
		}

		pTopmostNode = pMapTile->pTopmostHead;
		while ( pTopmostNode != NULL )
		{
			pMapTile->pTopmostHead = pTopmostNode->pNext;
			MemFree( pTopmostNode );
			pTopmostNode = pMapTile->pTopmostHead;
		}

		while (pMapTile->pStructureHead != NULL)
		{
			if (DeleteStructureFromWorld( pMapTile->pStructureHead ) == FALSE)
			{
				// ERROR!!!!!!
				break;
			}
		}

	}

	// Zero world
	memset( gpWorldLevelData, 0, WORLD_MAX * sizeof( MAP_ELEMENT ) );

	// Set some default flags
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		gpWorldLevelData[ cnt ].uiFlags |= MAPELEMENT_RECALCULATE_WIREFRAMES;
	}

	TrashDoorTable();
	TrashMapEdgepoints();
	TrashDoorStatusArray();

	//gfBlitBattleSectorLocator = FALSE;
	gfWorldLoaded = FALSE;
	sprintf( gubFilename, "none" );
}



void TrashMapTile(INT16 MapTile)
{
	MAP_ELEMENT		*pMapTile;
	LEVELNODE			*pLandNode;
	LEVELNODE			*pObjectNode;
	LEVELNODE			*pStructNode;
	LEVELNODE			*pShadowNode;
	LEVELNODE			*pMercNode;
	LEVELNODE			*pRoofNode;
	LEVELNODE			*pOnRoofNode;
	LEVELNODE			*pTopmostNode;

	pMapTile = &gpWorldLevelData[ MapTile ];

	// Free the memory associated with the map tile link lists
	pLandNode = pMapTile->pLandHead;
	while ( pLandNode != NULL )
	{
		pMapTile->pLandHead = pLandNode->pNext;
		MemFree( pLandNode );
		pLandNode = pMapTile->pLandHead;
	}
	pMapTile->pLandHead = pMapTile->pLandStart = NULL;

	pObjectNode = pMapTile->pObjectHead;
	while ( pObjectNode != NULL )
	{
		pMapTile->pObjectHead = pObjectNode->pNext;
		MemFree( pObjectNode );
		pObjectNode = pMapTile->pObjectHead;
	}
	pMapTile->pObjectHead = NULL;

	pStructNode = pMapTile->pStructHead;
	while ( pStructNode != NULL )
	{
		pMapTile->pStructHead = pStructNode->pNext;
		MemFree( pStructNode );
		pStructNode = pMapTile->pStructHead;
	}
	pMapTile->pStructHead = NULL;

	pShadowNode = pMapTile->pShadowHead;
	while ( pShadowNode != NULL )
	{
		pMapTile->pShadowHead = pShadowNode->pNext;
		MemFree( pShadowNode );
		pShadowNode = pMapTile->pShadowHead;
	}
	pMapTile->pShadowHead = NULL;

	pMercNode = pMapTile->pMercHead;
	while ( pMercNode != NULL )
	{
		pMapTile->pMercHead = pMercNode->pNext;
		MemFree( pMercNode );
		pMercNode = pMapTile->pMercHead;
	}
	pMapTile->pMercHead = NULL;

	pRoofNode = pMapTile->pRoofHead;
	while ( pRoofNode != NULL )
	{
		pMapTile->pRoofHead = pRoofNode->pNext;
		MemFree( pRoofNode );
		pRoofNode = pMapTile->pRoofHead;
	}
	pMapTile->pRoofHead = NULL;

	pOnRoofNode = pMapTile->pOnRoofHead;
	while ( pOnRoofNode != NULL )
	{
		pMapTile->pOnRoofHead = pOnRoofNode->pNext;
		MemFree( pOnRoofNode );
		pOnRoofNode = pMapTile->pOnRoofHead;
	}
	pMapTile->pOnRoofHead =		NULL;

	pTopmostNode = pMapTile->pTopmostHead;
	while ( pTopmostNode != NULL )
	{
		pMapTile->pTopmostHead = pTopmostNode->pNext;
		MemFree( pTopmostNode );
		pTopmostNode = pMapTile->pTopmostHead;
	}
	pMapTile->pTopmostHead =	NULL;

	while (pMapTile->pStructureHead != NULL)
	{
		DeleteStructureFromWorld( pMapTile->pStructureHead );
	}
	pMapTile->pStructureHead = pMapTile->pStructureTail = NULL;
}



BOOLEAN LoadMapTileset( INT32 iTilesetID )
{

#ifdef JA2UBMAPS
giOldTilesetUsed = giCurrentTilesetID;
#endif

	if ( iTilesetID >= gubNumSets )
	{
		return( FALSE );
	}

	// Init tile surface used values
	memset( gbNewTileSurfaceLoaded, 0, sizeof( gbNewTileSurfaceLoaded ) );

	if( iTilesetID == giCurrentTilesetID )
	{
		return( TRUE );
	}

	// Get free memory value nere
	gSurfaceMemUsage = guiMemTotal;

	// LOAD SURFACES
	CHECKF( LoadTileSurfaces( &(gTilesets[ iTilesetID ].TileSurfaceFilenames[0] ), (UINT8)iTilesetID ) != FALSE );

	// SET TERRAIN COSTS
	if ( gTilesets[ iTilesetID ].MovementCostFnc != NULL )
	{
		gTilesets[ iTilesetID ].MovementCostFnc( );
	}
	else
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Tileset %d has no callback function for movement costs. Using default.", iTilesetID) );
		SetTilesetOneTerrainValues( );
	}

	// RESET TILE DATABASE
	DeallocateTileDatabase( );

	CreateTileDatabase( );

	// SET GLOBAL ID FOR TILESET ( FOR SAVING! )
	giCurrentTilesetID = iTilesetID;

	return( TRUE );
}


BOOLEAN SaveMapTileset( INT32 iTilesetID )
{
//	FILE *hTSet;
	HWFILE hTSet;
	char zTilesetName[65];
	int	cnt;
	UINT32	uiBytesWritten;

	// Are we trying to save the default tileset?
	if ( iTilesetID == 0 )
		return( TRUE );

	sprintf( zTilesetName, "TSET%04d.SET", iTilesetID );

	// Open file
	hTSet = FileOpen( zTilesetName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );


	if ( !hTSet )
	{
		return( FALSE );
	}

	// Save current tile set in map file.
	for ( cnt = 0; cnt < giNumberOfTileTypes; cnt++ )
		FileWrite( hTSet, TileSurfaceFilenames[cnt], 65, &uiBytesWritten );
	FileClose( hTSet );

	return( TRUE );
}

void SetLoadOverrideParams( BOOLEAN fForceLoad, BOOLEAN fForceFile, CHAR8 *zLoadName )
{
	gfForceLoadPlayers 		= fForceLoad;
	gfForceLoad				= fForceFile;

	if ( zLoadName != NULL )
	{
		strcpy( gzForceLoadFile, zLoadName );
	}
}


void AddWireFrame( INT32 sGridNo, UINT16 usIndex, BOOLEAN fForced )
{
	LEVELNODE			*pTopmost, *pTopmostTail;


	pTopmost = gpWorldLevelData[ sGridNo ].pTopmostHead;

	while ( pTopmost != NULL )
	{
		// Check if one of the samer type exists!
		if ( pTopmost->usIndex == usIndex )
		{
			return;
		}
		pTopmost = pTopmost->pNext;
	}

	pTopmostTail = AddTopmostToTail( sGridNo, usIndex );

	if ( fForced )
	{
		pTopmostTail->uiFlags |= LEVELNODE_WIREFRAME;
	}

}


UINT16 GetWireframeGraphicNumToUseForWall( INT32 sGridNo, STRUCTURE *pStructure )
{
	LEVELNODE	*pNode = NULL;
	UINT8		ubWallOrientation;
	UINT16		usValue = 0;
	UINT16		usSubIndex;
	STRUCTURE	 *pBaseStructure;

	ubWallOrientation = pStructure->ubWallOrientation;

	pBaseStructure = FindBaseStructure( pStructure );

	if ( pBaseStructure )
	{
		// Find levelnode...
		pNode = gpWorldLevelData[sGridNo].pStructHead;
		while( pNode != NULL )
		{
			if (pNode->pStructureData == pBaseStructure)
			{
				break;
			}
			pNode = pNode->pNext;
		}

		if ( pNode != NULL )
		{
			// Get Subindex for this wall...
			GetSubIndexFromTileIndex( pNode->usIndex, &usSubIndex );

			// Check for broken peices...
			if ( usSubIndex == 48 || usSubIndex == 52 )
			{
				return( WIREFRAMES12 );
			}
			else if ( usSubIndex == 49 || usSubIndex == 53 )
			{
				return( WIREFRAMES13 );
			}
			else if ( usSubIndex == 50 || usSubIndex == 54 )
			{
				return( WIREFRAMES10 );
			}
			else if ( usSubIndex == 51 || usSubIndex == 55 )
			{
				return( WIREFRAMES11 );
			}
		}
	}

	switch( ubWallOrientation )
	{
		case OUTSIDE_TOP_LEFT:
		case INSIDE_TOP_LEFT:

		usValue = WIREFRAMES6;
			break;

		case OUTSIDE_TOP_RIGHT:
		case INSIDE_TOP_RIGHT:
			usValue = WIREFRAMES5;
			break;

	}

	return( usValue );
}

void CalculateWorldWireFrameTiles( BOOLEAN fForce )
{
	INT32			cnt;
	STRUCTURE		*pStructure;
	INT32			sGridNo;
	UINT8			ubWallOrientation;
	INT8			bHiddenVal;
	INT8			bNumWallsSameGridNo;
	INT32			usWireFrameIndex;

	// Create world randomly from tiles
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		if ( gpWorldLevelData[ cnt ].uiFlags & MAPELEMENT_RECALCULATE_WIREFRAMES || fForce )
		{
			if ( cnt == 8377 )
			{
				int i = 0;
			}

			// Turn off flag
			gpWorldLevelData[ cnt ].uiFlags &= (~MAPELEMENT_RECALCULATE_WIREFRAMES );

			// Remove old ones
			RemoveWireFrameTiles( cnt );

			bNumWallsSameGridNo = 0;

			// Check our gridno, if we have a roof over us that has not beenr evealed, no need for a wiereframe
			if ( IsRoofVisibleForWireframe( cnt ) && !( gpWorldLevelData[ cnt ].uiFlags & MAPELEMENT_REVEALED ) )
			{
				continue;
			}

			pStructure = gpWorldLevelData[ cnt ].pStructureHead;

			while ( pStructure != NULL )
			{
				// Check for doors
				if ( pStructure->fFlags & STRUCTURE_ANYDOOR )
				{
					// ATE: need this additional check here for hidden doors!
					if ( pStructure->fFlags & STRUCTURE_OPENABLE )
					{
						// Does the gridno we are over have a non-visible tile?
						// Based on orientation
						ubWallOrientation = pStructure->ubWallOrientation;

						switch( ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
							case INSIDE_TOP_LEFT:

								// Get gridno
								sGridNo = NewGridNo( cnt, DirectionInc( SOUTH ) );

								if ( IsRoofVisibleForWireframe( sGridNo ) && !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) )
								{
									AddWireFrame( cnt, WIREFRAMES4, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );
								}
								break;

							case OUTSIDE_TOP_RIGHT:
							case INSIDE_TOP_RIGHT:

								// Get gridno
								sGridNo = NewGridNo( cnt, DirectionInc( EAST ) );

								if ( IsRoofVisibleForWireframe( sGridNo ) && !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) )
								{
									AddWireFrame( cnt, WIREFRAMES3 , (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );
								}
								break;

						}
					}
				}
				// Check for windows
				else
				{
					if ( pStructure->fFlags & STRUCTURE_WALLNWINDOW )
					{
						// Does the gridno we are over have a non-visible tile?
						// Based on orientation
						ubWallOrientation = pStructure->ubWallOrientation;

						switch( ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
							case INSIDE_TOP_LEFT:

							// Get gridno
							  sGridNo = NewGridNo( cnt, DirectionInc( SOUTH ) );

							if ( IsRoofVisibleForWireframe( sGridNo ) && !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) )
							{
								  AddWireFrame( cnt, WIREFRAMES2, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );
							}
							break;

							case OUTSIDE_TOP_RIGHT:
							case INSIDE_TOP_RIGHT:

							// Get gridno
							sGridNo = NewGridNo( cnt, DirectionInc( EAST ) );

							if ( IsRoofVisibleForWireframe( sGridNo ) && !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) )
							{
								AddWireFrame( cnt, WIREFRAMES1, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );
							}
							break;

						}

					}

					// Check for walls
					if ( pStructure->fFlags & STRUCTURE_WALLSTUFF )
					{
						// Does the gridno we are over have a non-visible tile?
						// Based on orientation
						ubWallOrientation = pStructure->ubWallOrientation;

						usWireFrameIndex = GetWireframeGraphicNumToUseForWall( cnt, pStructure );

						switch( ubWallOrientation )
						{
							case OUTSIDE_TOP_LEFT:
							case INSIDE_TOP_LEFT:

								// Get gridno
								sGridNo = NewGridNo( cnt, DirectionInc( SOUTH ) );

								if ( IsRoofVisibleForWireframe( sGridNo ) )
								{
									bNumWallsSameGridNo++;

									AddWireFrame( cnt, usWireFrameIndex, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );

									// Check along our direction to see if we are a corner
									sGridNo = NewGridNo( cnt, DirectionInc( WEST ) );
									sGridNo = NewGridNo( sGridNo, DirectionInc( SOUTH ) );
									bHiddenVal = IsHiddenTileMarkerThere( sGridNo );
									// If we do not exist ( -1 ) or are revealed ( 1 )
									if ( bHiddenVal == -1 || bHiddenVal == 1 )
									{
										// Place corner!
										   AddWireFrame( cnt, WIREFRAMES9, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );
									}
								}
								break;

							case OUTSIDE_TOP_RIGHT:
							case INSIDE_TOP_RIGHT:

								// Get gridno
								sGridNo = NewGridNo( cnt, DirectionInc( EAST ) );

								if ( IsRoofVisibleForWireframe( sGridNo ) )
								{
									bNumWallsSameGridNo++;

									AddWireFrame( cnt, usWireFrameIndex, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );

									// Check along our direction to see if we are a corner
									sGridNo = NewGridNo( cnt, DirectionInc( NORTH ) );
									sGridNo = NewGridNo( sGridNo, DirectionInc( EAST ) );
									bHiddenVal = IsHiddenTileMarkerThere( sGridNo );
									// If we do not exist ( -1 ) or are revealed ( 1 )
									if ( bHiddenVal == -1 || bHiddenVal == 1 )
									{
										// Place corner!
										AddWireFrame( cnt, WIREFRAMES8, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 ) );
									}

								}
								break;

						}

					// Check for both walls
					if ( bNumWallsSameGridNo == 2 )
					{
						sGridNo = NewGridNo( cnt, DirectionInc( EAST ) );
						sGridNo = NewGridNo( sGridNo, DirectionInc( SOUTH ) );
						AddWireFrame( cnt, WIREFRAMES7, (BOOLEAN)( ( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) != 0 )  );
					}
			}
				}

				pStructure = pStructure->pNext;
			}
		}
	}
}


void RemoveWorldWireFrameTiles( )
{
	INT32					cnt;

	// Create world randomly from tiles
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		RemoveWireFrameTiles( cnt );
	}

}


void RemoveWireFrameTiles( INT32 sGridNo )
{
	LEVELNODE			*pTopmost, *pNewTopmost;
	TILE_ELEMENT *	pTileElement;

	pTopmost = gpWorldLevelData[ sGridNo ].pTopmostHead;

	while ( pTopmost != NULL )
	{
		pNewTopmost = pTopmost->pNext;

		if ( pTopmost->usIndex < giNumberOfTiles )
		{
			pTileElement = &(gTileDatabase[ pTopmost->usIndex ]);

			if ( pTileElement->fType == WIREFRAMES )
			{
				RemoveTopmost( sGridNo, pTopmost->usIndex );
			}
		}

		pTopmost = pNewTopmost;
	}

}



INT8 IsHiddenTileMarkerThere( INT32 sGridNo )
{
	STRUCTURE * pStructure;

	if ( !gfBasement )
	{
		pStructure = FindStructure( sGridNo, STRUCTURE_ROOF );

		if ( pStructure != NULL )
		{
			//if ( !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) )
			{
				return( 2 );
			}

			// if we are here, a roof exists but has been revealed
			return( 1 );
		}
	}
	else
	{
		//if ( InARoom( sGridNo, &ubRoom ) )
		{
			//if ( !( gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REVEALED ) )
			{
				return( 2 );
			}

			return( 1 );
		}
	}

	return( -1 );
}


void ReloadTileset( UINT8 ubID )
{
	CHAR8	aFilename[ 255 ];
	INT32 iCurrTilesetID = giCurrentTilesetID;

	// Set gloabal
	giCurrentTilesetID = ubID;

	// Save Map
	SaveWorld( TEMP_FILE_FOR_TILESET_CHANGE );

	//IMPORTANT:  If this is not set, the LoadTileset() will assume that
	//it is loading the same tileset and ignore it...
	giCurrentTilesetID = iCurrTilesetID;

	// Load Map with new tileset
	LoadWorld( TEMP_FILE_FOR_TILESET_CHANGE );

	// Delete file
	sprintf( aFilename, "MAPS\\%s", TEMP_FILE_FOR_TILESET_CHANGE );

	FileDelete( aFilename );

}


void SaveMapLights( HWFILE hfile )
{
	SOLDIERTYPE *pSoldier;
	SGPPaletteEntry	LColors[3];
	UINT8 ubNumColors;
	BOOLEAN fSoldierLight;
	UINT16 usNumLights = 0;
	UINT16 cnt, cnt2;
	UINT8 ubStrLen;
	UINT32	uiBytesWritten;

	ubNumColors = LightGetColors( LColors );

	// Save the current light colors!
	FileWrite( hfile, &ubNumColors, 1, &uiBytesWritten );
	FileWrite( hfile, LColors, sizeof(SGPPaletteEntry)*ubNumColors, &uiBytesWritten );


	//count number of non-merc lights.
	for( cnt = 0; cnt < MAX_LIGHT_SPRITES; cnt++ )
	{
		if( LightSprites[ cnt ].uiFlags & LIGHT_SPR_ACTIVE )
		{ //found an active light.  Check to make sure it doesn't belong to a merc.
			fSoldierLight = FALSE;
			for ( cnt2 = 0; cnt2 < MAX_NUM_SOLDIERS && !fSoldierLight; cnt2++ )
			{
				if( GetSoldier( &pSoldier, cnt2 ) )
				{
					if ( pSoldier->iLight == (INT32)cnt )
						fSoldierLight = TRUE;
				}
			}
			if( !fSoldierLight )
				usNumLights++;
		}
	}

	//save the number of lights.
	FileWrite( hfile, &usNumLights, 2, &uiBytesWritten );


	for( cnt = 0; cnt < MAX_LIGHT_SPRITES; cnt++ )
	{
		if( LightSprites[ cnt ].uiFlags & LIGHT_SPR_ACTIVE )
		{ //found an active light.  Check to make sure it doesn't belong to a merc.
			fSoldierLight = FALSE;
			for ( cnt2 = 0; cnt2 < MAX_NUM_SOLDIERS && !fSoldierLight; cnt2++ )
			{
				if ( GetSoldier( &pSoldier, cnt2 ) )
				{
					if ( pSoldier->iLight == (INT32)cnt )
						fSoldierLight = TRUE;
				}
			}
			if( !fSoldierLight )
			{ //save the light
				FileWrite( hfile, &LightSprites[ cnt ], sizeof( LIGHT_SPRITE ), &uiBytesWritten );

				ubStrLen = strlen( pLightNames[LightSprites[cnt].iTemplate] ) + 1 ;
				FileWrite( hfile, &ubStrLen, 1, &uiBytesWritten );
				FileWrite( hfile, pLightNames[LightSprites[cnt].iTemplate], ubStrLen, &uiBytesWritten );

			}
		}
	}
}

void LoadMapLights( INT8 **hBuffer )
{
	SGPPaletteEntry	LColors[3];
	UINT8 ubNumColors;
	UINT16 usNumLights;
	INT32 cnt;
	CHAR8	str[30];
	UINT8 ubStrLen;
	LIGHT_SPRITE	TmpLight;
	INT32 iLSprite;
	UINT32 uiHour;
	BOOLEAN fPrimeTime = FALSE, fNightTime = FALSE;

	//reset the lighting system, so that any current lights are toasted.
	LightReset();

	// read in the light colors!
	LOADDATA( &ubNumColors, *hBuffer, 1 );
	LOADDATA( LColors, *hBuffer, sizeof(SGPPaletteEntry)*ubNumColors );

	LOADDATA( &usNumLights, *hBuffer, 2 );

	ubNumColors = 1;

	// ATE: OK, only regenrate if colors are different.....
	//if ( LColors[0].peRed != gpLightColors[0].peRed ||
	//		LColors[0].peGreen != gpLightColors[0].peGreen ||
	//		LColors[0].peBlue != gpLightColors[0].peBlue )
	{
		LightSetColors( LColors, ubNumColors );
	}

	//Determine which lights are valid for the current time.
	if( !gfEditMode )
	{
		uiHour = GetWorldHour();
		if( uiHour >= NIGHT_TIME_LIGHT_START_HOUR || uiHour < NIGHT_TIME_LIGHT_END_HOUR )
		{
			fNightTime = TRUE;
		}
		if( uiHour >= PRIME_TIME_LIGHT_START_HOUR )
		{
			fPrimeTime = TRUE;
		}
	}

	for( cnt = 0; cnt < usNumLights; cnt++ )
	{
		LOADDATA( &TmpLight, *hBuffer, sizeof( LIGHT_SPRITE ) );
		LOADDATA( &ubStrLen, *hBuffer, 1 );

		//dnl ch44 280909 LIGHT_SPRITE translation
		gMapTrn.GetTrnXY(TmpLight.iOldX, TmpLight.iOldY);
		gMapTrn.GetTrnXY(TmpLight.iX, TmpLight.iY);

		if( ubStrLen )
		{
			LOADDATA( str, *hBuffer, ubStrLen );
		}

		str[ ubStrLen ] = 0;

		iLSprite = LightSpriteCreate( str, TmpLight.uiLightType );
		//if this fails, then we will ignore the light.
		// ATE: Don't add ANY lights of mapscreen util is on
		if( iLSprite != -1/* && guiCurrentScreen != MAPUTILITY_SCREEN*/ )//dnl ch79 301113 lights will be reset in map utility screen
		{
			if( !gfCaves || gfEditMode )
			{
				if( gfEditMode ||
					TmpLight.uiFlags & LIGHT_PRIMETIME && fPrimeTime ||
					TmpLight.uiFlags & LIGHT_NIGHTTIME && fNightTime ||
					!(TmpLight.uiFlags & (LIGHT_PRIMETIME | LIGHT_NIGHTTIME)) )
				{
					//power only valid lights.
					LightSpritePower( iLSprite, TRUE );
				}
			}
			LightSpritePosition( iLSprite, TmpLight.iX, TmpLight.iY );
			if( TmpLight.uiFlags & LIGHT_PRIMETIME )
				LightSprites[ iLSprite ].uiFlags |= LIGHT_PRIMETIME;
			else if( TmpLight.uiFlags & LIGHT_NIGHTTIME )
				LightSprites[ iLSprite ].uiFlags |= LIGHT_NIGHTTIME;
		}
	}
}


BOOLEAN IsRoofVisibleForWireframe( INT32 sMapPos )
{
	STRUCTURE * pStructure;

	if ( !gfBasement )
	{
		pStructure = FindStructure( sMapPos, STRUCTURE_ROOF );

		if ( pStructure != NULL )
		{
			return( TRUE );
		}
	}
	else
	{
		//if ( InARoom( sMapPos, &ubRoom ) )
		{
			//if ( !( gpWorldLevelData[ sMapPos ].uiFlags & MAPELEMENT_REVEALED ) )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

//dnl ch43 260909
void SetWorldSize(INT32 nWorldRows, INT32 nWorldCols)
{
	gMapTrn.ResizeTrnCfg(WORLD_ROWS, WORLD_COLS, nWorldRows, nWorldCols);//dnl ch45 011009

	INT32 o_WORLD_MAX = WORLD_MAX;
	WORLD_ROWS = nWorldRows;
	WORLD_COLS = nWorldCols;

	ResetScrollOverheadMap();//dnl ch45 031009
	CenterScreenAtMapIndex(MAPROWCOLTOPOS(WORLD_ROWS/2, WORLD_COLS/2));//dnl ch54 111009

	if(gubGridNoMarkers)
		MemFree(gubGridNoMarkers);
	gubGridNoMarkers = (UINT8*)MemAlloc(WORLD_MAX);
	memset(gubGridNoMarkers, 0, sizeof(UINT8)*WORLD_MAX);

	if(gsCoverValue)
		MemFree(gsCoverValue);
	gsCoverValue = (INT16*)MemAlloc(sizeof(INT16)*WORLD_MAX);
	memset(gsCoverValue, 0x7F, sizeof(INT16)*WORLD_MAX);

	// Init building structures and variables
	if(gubBuildingInfo)
		MemFree(gubBuildingInfo);
	gubBuildingInfo = (UINT8*)MemAlloc(WORLD_MAX);
	memset(gubBuildingInfo, 0, sizeof(UINT8)*WORLD_MAX);

	// Init room numbers
	//DBrot: More Rooms
	if(gusWorldRoomInfo)
		MemFree(gusWorldRoomInfo);
	gusWorldRoomInfo = (UINT16*)MemAlloc(sizeof(UINT16)*WORLD_MAX);
	memset(gusWorldRoomInfo, 0, sizeof(UINT16)*WORLD_MAX);

	if(gubWorldMovementCosts)
		MemFree(gubWorldMovementCosts);
	gubWorldMovementCosts = (UINT8(*)[MAXDIR][2])MemAlloc(WORLD_MAX*MAXDIR*2);
	memset(gubWorldMovementCosts, 0, sizeof(UINT8)*WORLD_MAX*MAXDIR*2);

	if(gpWorldLevelData != NULL)
		MemFree(gpWorldLevelData);
	// Initialize world data
	gpWorldLevelData = (MAP_ELEMENT*)MemAlloc(sizeof(MAP_ELEMENT)*WORLD_MAX);
	// Zero world
	memset(gpWorldLevelData, 0, sizeof(MAP_ELEMENT)*WORLD_MAX);

	ShutDownPathAI();
	InitPathAI();

#ifdef _DEBUG
	if(gubFOVDebugInfoInfo)
		MemFree(gubFOVDebugInfoInfo);
	gubFOVDebugInfoInfo = (UINT8*)MemAlloc(WORLD_MAX);
#endif

	dirDelta[0]= -WORLD_COLS;
	dirDelta[1]= 1-WORLD_COLS;
	dirDelta[3]= 1+WORLD_COLS;
	dirDelta[4]= WORLD_ROWS;
	dirDelta[5]= WORLD_ROWS-1;
	dirDelta[7]= -WORLD_ROWS-1;

	DirIncrementer[0] = -WORLD_ROWS;
	DirIncrementer[1] = 1-WORLD_ROWS;
	DirIncrementer[3] = 1+WORLD_ROWS;
	DirIncrementer[4] = WORLD_ROWS;
	DirIncrementer[5] = WORLD_ROWS-1;
	DirIncrementer[6] = -1;
	DirIncrementer[7] = -WORLD_ROWS-1;

	gsTempActionGridNo = NOWHERE;
	gsOverItemsGridNo = NOWHERE;
	gsOutOfRangeGridNo = NOWHERE;
	gsUITargetShotGridNo = NOWHERE;
	gsUIHandleShowMoveGridLocation = NOWHERE;	
	gusCurMousePos = 0;
}

//dnl ch44 290909 Translation routine
MAPTRANSLATION gMapTrn;

MAPTRANSLATION::MAPTRANSLATION()
{
	fTrn = FALSE;
	iTrnFromRows = OLD_WORLD_ROWS;
	iTrnFromCols = OLD_WORLD_COLS;
	iTrnToRows = OLD_WORLD_ROWS;
	iTrnToCols = OLD_WORLD_COLS;
	iResizeTrnFromRows = OLD_WORLD_ROWS;
	iResizeTrnFromCols = OLD_WORLD_COLS;
	iResizeTrnToRows = OLD_WORLD_ROWS;
	iResizeTrnToCols = OLD_WORLD_COLS;
}

MAPTRANSLATION::~MAPTRANSLATION()
{
	;
}

void MAPTRANSLATION::GetTrnCnt(INT32& cnt)
{
	if(!fTrn || cnt < 0)
		return;
	INT32 y = cnt / iTrnFromCols;
	INT32 x = cnt - y * iTrnFromCols;
	cnt = (x + (iTrnToCols - iTrnFromCols) / 2) + (y + (iTrnToRows - iTrnFromRows) / 2) * iTrnToCols;
}

void MAPTRANSLATION::GetTrnXY(INT16& x, INT16& y)
{
	if(!fTrn || x < 0 || y < 0)
		return;
	x += (iTrnToCols - iTrnFromCols) / 2;
	y += (iTrnToRows - iTrnFromRows) / 2;
}

void MAPTRANSLATION::ResizeTrnCfg(INT32 iFromRows, INT32 iFromCols, INT32 iToRows, INT32 iToCols)
{
	iResizeTrnFromRows = iFromRows;
	iResizeTrnFromCols = iFromCols;
	iResizeTrnToRows = iToRows;
	iResizeTrnToCols = iToCols;
}

void MAPTRANSLATION::ResizeTrnCnt(INT32& cnt)
{
	INT32 y = cnt / iResizeTrnFromCols;
	INT32 x = cnt - y * iResizeTrnFromCols;
	cnt = (x * iResizeTrnToCols / iResizeTrnFromCols) + (y * iResizeTrnToRows / iResizeTrnFromRows) * iResizeTrnToCols;
}

BOOLEAN MAPTRANSLATION::SetTrnPar(INT32 iFromRows, INT32 iFromCols, INT32 iToRows, INT32 iToCols)
{
	// WANNE: Only enlarging works currently, making the make small does not work
	if(iFromRows < iToRows && iFromCols < iToCols)
	{
		fTrn = TRUE;
		iTrnFromRows = iFromRows;
		iTrnFromCols = iFromCols;
		iTrnToRows = iToRows;
		iTrnToCols = iToCols;
	}
	else
		fTrn = FALSE;
	return(fTrn);
}

