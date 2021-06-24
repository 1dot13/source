#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "Enemy Soldier Save.h"
#else
	#include "Types.h"
	#include "MemMan.h"
	#include "message.h"
	#include "Items.h"
	#include "Handle Items.h"
	#include "StrategicMap.h"
	#include "Tactical Save.h"
	#include <stdio.h>
	#include "Campaign Types.h"
	#include "SaveLoadGame.h"
	#include "WorldDef.h"
	#include "rotting corpses.h"
	#include "OverHead.h"
	#include "Keys.h"
	#include "Soldier Create.h"
	#include "Soldier Profile.h"
	#include "Isometric Utils.h"
	#include "Soldier Add.h"
	#include "Npc.h"
	#include "Ai.h"
	#include "Game Clock.h"
	#include "Animation Control.h"
	#include "Map Information.h"
	#include "SaveLoadMap.h"
	#include "debug.h"
	#include "Random.h"
	#include "quests.h"
	#include "Animated ProgressBar.h"
	#include "Text.h"
	#include "meanwhile.h"
	#include "Enemy Soldier Save.h"
	#include "SmokeEffects.h"
	#include "LightEffects.h"
	#include "PATHAI.H"
	#include "GameVersion.h"
	#include "strategic.h"
	#include "Map Screen Interface Map.h"
	#include "Strategic Status.h"
	#include "Soldier macros.h"
	#include "sgp.h"
	#include "MessageBoxScreen.h"
	#include "screenids.h"
	#include "Queen Command.h"
	#include "Map Screen Interface Map Inventory.h"
#endif
#include "Animation Control.h"
#include <vfs/Core/vfs.h>
BOOLEAN gfWasInMeanwhile = FALSE;


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

///////////////////////////////////////////////////////////////
//
// Global Defines
//
///////////////////////////////////////////////////////////////


//dnl ch46 031009
// It's used for save info from the NPCQuoteInfo class that can change.
class TempNPCQuoteInfoSave
{
public:
	UINT16 usFlags;
	INT16 sRequiredItem;
	INT32 sRequiredGridNo;
	INT32 usGoToGridNo;
};

#define		NPC_TEMP_QUOTE_FILE			"Temp\\NpcQuote.tmp"


///////////////////////////////////////////////////////////////
//
// Global Variables
//
///////////////////////////////////////////////////////////////


extern	NPCQuoteInfo *	gpNPCQuoteInfoArray[NUM_PROFILES];

extern UINT32	guiJA2EncryptionSet;
extern UINT8 gubEncryptionArray1[ BASE_NUMBER_OF_ROTATION_ARRAYS * 3 ][ NEW_ROTATION_ARRAY_SIZE ];
extern UINT8 gubEncryptionArray2[ BASE_NUMBER_OF_ROTATION_ARRAYS * 3 ][ NEW_ROTATION_ARRAY_SIZE ];
extern UINT8 gubEncryptionArray3[ BASE_NUMBER_OF_ROTATION_ARRAYS * 3 ][ NEW_ROTATION_ARRAY_SIZE ];
extern UINT8 gubEncryptionArray4[ BASE_NUMBER_OF_ROTATION_ARRAYS * 3 ][ NEW_ROTATION_ARRAY_SIZE ];

INT32 giErrorMessageBox = 0;

///////////////////////////////////////////////////////////////
//
// Function Prototypes
//
///////////////////////////////////////////////////////////////

void TempFileLoadErrorMessageReturnCallback( UINT8 ubRetVal );

BOOLEAN SaveWorldItemsToTempItemFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 uiNumberOfItems, WORLDITEM *pData );
BOOLEAN RetrieveTempFileFromSavedGame( HWFILE hFile, UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ );

BOOLEAN AddTempFileToSavedGame( HWFILE hFile, UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ );


BOOLEAN SaveRottingCorpsesToTempCorpseFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, std::vector<ROTTING_CORPSE_DEFINITION> aCorpseDefVector );
BOOLEAN LoadRottingCorpsesFromTempCorpseFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ );

void LoadNPCInformationFromProfileStruct();
void SaveNPCInformationToProfileStruct( );

BOOLEAN DoesTempFileExistsForMap( UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ );


INT16 GetSoldierIDFromAnyMercID(UINT8 ubMercID);


BOOLEAN SetUnderGroundSectorFlag( INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ, UINT32 uiFlagToSet );
BOOLEAN GetUnderGroundSectorFlagStatus( INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ, UINT32 uiFlagToCheck );

BOOLEAN LoadAndAddWorldItemsFromTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ );

BOOLEAN InitTempNpcQuoteInfoForNPCFromTempFile();
BOOLEAN SaveTempNpcQuoteInfoForNPCToTempFile( UINT8 ubNpcId );
BOOLEAN LoadTempNpcQuoteInfoForNPCFromTempFile( UINT8 ubNpcId );
UINT32	GetLastTimePlayerWasInSector();
UINT32 GetLastTimePlayerWasInSector(INT16 sMapX, INT16 sMapY, INT8 sMapZ);	// Flugente: get time for another sector
void		SetLastTimePlayerWasInSector();
void		SetLastTimePlayerWasInSector(INT16 sMapX, INT16 sMapY, INT8 sMapZ);	// Flugente: set last time sector was visited


extern void InitLoadedWorld( );
extern void ReduceAmmoDroppedByNonPlayerSoldiers( SOLDIERTYPE *pSoldier, INT32 iInvSlot );

extern void StripEnemyDetailedPlacementsIfSectorWasPlayerLiberated();

#ifdef JA2BETAVERSION
extern BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode );
#endif


void	SynchronizeItemTempFileVisbleItemsToSectorInfoVisbleItems( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fLoadingGame );
UINT32 UpdateLoadedSectorsItemInventory( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 uiNumberOfItems );

//ppp

///////////////////////////////////////////////////////////////
//
// Functions
//
///////////////////////////////////////////////////////////////



// SaveMapTempFilesToSavedGameFile() Looks for and opens all Map Modification files.  It add each mod file to the save game file.
BOOLEAN SaveMapTempFilesToSavedGameFile( HWFILE hFile )
{
	UNDERGROUND_SECTORINFO *TempNode = gpUndergroundSectorInfoHead;
	INT16 sMapX;
	INT16 sMapY;


	//Save the current sectors open temp files to the disk
//	SaveCurrentSectorsItemsToTempItemFile();

	//
	//Loop though all the array elements to see if there is a data file to be saved
	//

	// Lesh: try to fix save/reload havok
	//First look through the above ground sectors
	for( sMapY=1; sMapY<=16; sMapY++ )
	{
		for( sMapX=1; sMapX<=16; sMapX++ )
		{
			//Save the Temp Item Files to the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ITEM_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_ITEM_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			// Save the Rotting Corpse Temp file to the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ROTTING_CORPSE_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}


			// Save the Map Modifications Temp file to the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			//Save the Door Table temp file to the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_DOOR_TABLE_TEMP_FILES_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_DOOR_TABLE_TEMP_FILES_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			//Save the revealed status temp file to the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_REVEALED_STATUS_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_REVEALED_STATUS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			//Save the door status temp file to the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_DOOR_STATUS_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_DOOR_STATUS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_CIV_PRESERVED_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_CIV_PRESERVED_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS )
			{
				if ( !AddTempFileToSavedGame( hFile, SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}


			//if any other file is to be saved
		}
	}


	//then look throught all the underground sectors
	while( TempNode )
	{
		if( TempNode->uiFlags & SF_ITEM_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_ITEM_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_ROTTING_CORPSE_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		// Save the Map Modifications Temp file to the saved game file
		if( TempNode->uiFlags & SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}


		//Save the door table to the saved game file
		if( TempNode->uiFlags & SF_DOOR_TABLE_TEMP_FILES_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_DOOR_TABLE_TEMP_FILES_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		//Save the revealed status temp file to the saved game file
		if( TempNode->uiFlags & SF_REVEALED_STATUS_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_REVEALED_STATUS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		//Save the door status temp file to the saved game file
		if( TempNode->uiFlags & SF_DOOR_STATUS_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_DOOR_STATUS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_CIV_PRESERVED_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_CIV_PRESERVED_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS )
		{
			if ( !AddTempFileToSavedGame( hFile, SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		//ttt


		TempNode = TempNode->next;
	}


	return( TRUE );
}

extern int gEnemyPreservedTempFileVersion[256];
extern int gCivPreservedTempFileVersion[256];

// LoadMapTempFilesFromSavedGameFile() loads all the temp files from the saved game file and writes them into the temp directory
BOOLEAN	LoadMapTempFilesFromSavedGameFile( HWFILE hFile )
{
	UNDERGROUND_SECTORINFO *TempNode = gpUndergroundSectorInfoHead;
	UINT32	uiPercentage;
	UINT32	iCounter = 0;

	// HACK FOR GABBY
	if ( guiCurrentSaveGameVersion < 81 && (gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		if ( gMercProfiles[ GABBY ].bMercStatus != MERC_IS_DEAD )
		{
			// turn off alternate flags for the sectors he could be in
			// randomly place him in one of the two possible sectors
			SectorInfo[ SECTOR( 14, MAP_ROW_L ) ].uiFlags &= ~SF_USE_ALTERNATE_MAP;
			SectorInfo[ SECTOR(  8, MAP_ROW_L ) ].uiFlags &= ~SF_USE_ALTERNATE_MAP;

			if ( Random( 2 ) )
			{
				SectorInfo[ SECTOR( 11, MAP_ROW_H ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
				gMercProfiles[ GABBY ].sSectorX = 11;
				gMercProfiles[ GABBY ].sSectorY = MAP_ROW_H;
			}
			else
			{
				SectorInfo[ SECTOR(  4, MAP_ROW_I ) ].uiFlags |= SF_USE_ALTERNATE_MAP;
				gMercProfiles[ GABBY ].sSectorX = 4;
				gMercProfiles[ GABBY ].sSectorY = MAP_ROW_I;
			}
		}
	}

	//
	//Loop though all the array elements to see if there is a data file to be loaded
	//

	//First look through the above ground sectors
	for ( INT16 sMapY = 1; sMapY < MAP_WORLD_Y - 1; ++sMapY )
	{
		for ( INT16 sMapX = 1; sMapX < MAP_WORLD_X - 1; ++sMapX )
		{
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ITEM_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_ITEM_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;

				//sync up the temp file data to the sector structure data
				SynchronizeItemTempFileVisbleItemsToSectorInfoVisbleItems( sMapX, sMapY, 0, TRUE );

				// Flugente: commented out, as the update now happens in SynchronizeItemTempFileVisbleItemsToSectorInfoVisbleItems, which will save us one loading routine per inventory
				/*if (guiCurrentSaveGameVersion != SAVE_GAME_VERSION)
				{
					for (int z = 0; z < 4; ++z)
					{
						UpdateWorldItemsTempFile(sMapX, sMapY, z);
					}
				}*/
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ROTTING_CORPSE_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_DOOR_TABLE_TEMP_FILES_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_DOOR_TABLE_TEMP_FILES_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			//Get the revealed status temp file From the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_REVEALED_STATUS_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_REVEALED_STATUS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			//Get the revealed status temp file From the saved game file
			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_DOOR_STATUS_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_DOOR_STATUS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
				gEnemyPreservedTempFileVersion[SECTOR( sMapX,sMapY)] = guiCurrentSaveGameVersion;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_CIV_PRESERVED_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_CIV_PRESERVED_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;

				if ( guiCurrentSaveGameVersion < 78 && (gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
				{
					CHAR8 pMapName[ 128 ];

					// KILL IT!!! KILL KIT!!!! IT IS CORRUPTED!!!
					GetMapTempFileName( SF_CIV_PRESERVED_TEMP_FILE_EXISTS, pMapName, sMapX, sMapY, 0 );
					FileDelete( pMapName );

					// turn off the flag
					SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags &= (~SF_CIV_PRESERVED_TEMP_FILE_EXISTS);
				}

				gCivPreservedTempFileVersion[SECTOR( sMapX,sMapY)] = guiCurrentSaveGameVersion;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}

			if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS )
			{
				if ( !RetrieveTempFileFromSavedGame( hFile, SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, sMapX, sMapY, 0 ) )
					return FALSE;
			}
			
			//if any other file is to be saved

			++iCounter;

			//update the progress bar
			uiPercentage = (iCounter * 100) / (255);

			RenderProgressBar( 0, uiPercentage );
		}
	}
	
	//then look throught all the underground sectors
	while( TempNode )
	{
		if( TempNode->uiFlags & SF_ITEM_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_ITEM_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;

			//sync up the temp file data to the sector structure data
			SynchronizeItemTempFileVisbleItemsToSectorInfoVisbleItems( TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ, TRUE );
		}

		if( TempNode->uiFlags & SF_ROTTING_CORPSE_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_DOOR_TABLE_TEMP_FILES_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_DOOR_TABLE_TEMP_FILES_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		//Get the revealed status temp file From the saved game file
		if( TempNode->uiFlags & SF_REVEALED_STATUS_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_REVEALED_STATUS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		//Get the revealed status temp file From the saved game file
		if( TempNode->uiFlags & SF_DOOR_STATUS_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_DOOR_STATUS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_CIV_PRESERVED_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_CIV_PRESERVED_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;

			if ( guiCurrentSaveGameVersion < 78 && (gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
			{
				CHAR8 pMapName[ 128 ];

				// KILL IT!!! KILL KIT!!!! IT IS CORRUPTED!!!
				GetMapTempFileName( SF_CIV_PRESERVED_TEMP_FILE_EXISTS, pMapName, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ );
				FileDelete( pMapName );

				// turn off the flag
				TempNode->uiFlags &= (~SF_CIV_PRESERVED_TEMP_FILE_EXISTS);
			}
		}

		if( TempNode->uiFlags & SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		if( TempNode->uiFlags & SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS )
		{
			if ( !RetrieveTempFileFromSavedGame( hFile, SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, TempNode->ubSectorX, TempNode->ubSectorY, TempNode->ubSectorZ ) )
				return FALSE;
		}

		TempNode = TempNode->next;
	}
//ttt
	return( TRUE );
}


BOOLEAN UpdateWorldItemsTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32	uiTotalNumberOfItems=0;
	std::vector<WORLDITEM> pTotalSectorList;//dnl ch75 271013
	BOOLEAN fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &( uiTotalNumberOfItems ), FALSE );
	if (fReturn == false)
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("GetNumberOfWorldItemsFromTempItemFile failed" ) );
		Assert( fReturn );
	}

	if( uiTotalNumberOfItems > 0 )
	{
		// allocate space for the list
		pTotalSectorList.resize(uiTotalNumberOfItems);//dnl ch75 271013

		LoadWorldItemsFromTempItemFile( sMapX,  sMapY, bMapZ, pTotalSectorList );
		int backup = guiCurrentSaveGameVersion;
		guiCurrentSaveGameVersion = SAVE_GAME_VERSION;
		SaveWorldItemsToTempItemFile( sMapX, sMapY, bMapZ, uiTotalNumberOfItems, pTotalSectorList);
		guiCurrentSaveGameVersion = backup;
	}

	return TRUE;
}

/*
BOOLEAN UpdateENEMYTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	NewWayOfSavingEnemyAndCivliansToTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, TRUE, TRUE );
	NewWayOfSavingEnemyAndCivliansToTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, FALSE, TRUE );
	LoadWorldItemsFromTempItemFile( sMapX,  sMapY, bMapZ, pTotalSectorList );
	int backup = guiCurrentSaveGameVersion;
	guiCurrentSaveGameVersion = SAVE_GAME_VERSION;
	SaveWorldItemsToTempItemFile( sMapX, sMapY, bMapZ, uiTotalNumberOfItems, pTotalSectorList);
	guiCurrentSaveGameVersion = backup;
}


BOOLEAN UpdateCIVTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	LoadWorldItemsFromTempItemFile( sMapX,  sMapY, bMapZ, pTotalSectorList );
	int backup = guiCurrentSaveGameVersion;
	guiCurrentSaveGameVersion = SAVE_GAME_VERSION;
	SaveWorldItemsToTempItemFile( sMapX, sMapY, bMapZ, uiTotalNumberOfItems, pTotalSectorList);
	guiCurrentSaveGameVersion = backup;
	return TRUE;
}
*/

BOOLEAN SaveWorldItemsToTempItemFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 uiNumberOfItems, std::vector<WORLDITEM>& pData, BOOLEAN fUpdateVisibleItems )//dnl ch75 271013
{
	HWFILE	hFile;
	UINT32	uiNumBytesWritten=0;
	CHAR8		zMapName[ 128 ];

	GetMapTempFileName( SF_ITEM_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	//Open the file for writing, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}

	//Save the size of the ITem table
	FileWrite( hFile, &uiNumberOfItems, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}


	//if there are items to save..
	for (unsigned int x = 0; x < uiNumberOfItems; ++x)
	{
		//Save the ITem array
		if ( !pData[x].Save(hFile, FALSE) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}
	}

	FileClose( hFile );

	SetSectorFlag( sMapX, sMapY, bMapZ, SF_ITEM_TEMP_FILE_EXISTS );

	if( fUpdateVisibleItems )//dnl ch75 311013
		SynchronizeItemTempFileVisbleItemsToSectorInfoVisbleItems( sMapX, sMapY, bMapZ, FALSE );

	return( TRUE );
}



BOOLEAN LoadWorldItemsFromTempItemFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, std::vector<WORLDITEM>& pData )//dnl ch75 271013
{
	UINT32	uiNumBytesRead=0;
	HWFILE	hFile;
	CHAR8		zMapName[ 128 ];
	UINT32 uiNumberOfItems=0;

	GetMapTempFileName( SF_ITEM_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );


	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}


	// Load the size of the World ITem table
	FileRead( hFile, &uiNumberOfItems, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}

	// Load the World ITem table
	for (unsigned int x = 0; x < uiNumberOfItems; ++x)
	{
		if ( !pData[x].Load(hFile) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}
	}

	FileClose( hFile );

	return( TRUE );
}


BOOLEAN GetNumberOfWorldItemsFromTempItemFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 *pSizeOfData, BOOLEAN fIfEmptyCreate )
{
	UINT32	uiNumBytesRead=0;
	HWFILE	hFile;
	CHAR8		zMapName[ 128 ];
	UINT32	uiNumberOfItems=0;

	GetMapTempFileName( SF_ITEM_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );


	//Check if the file DOESNT exists
	if( !FileExistsNoDB( zMapName ) )
	{
		if( fIfEmptyCreate )
		{
			//ADB: I don't know why we are initing 10 WORLDITEMS then saving them, but that's what the code was.
			WORLDITEM	TempWorldItems[ 10 ];
			UINT32		uiNumberOfItems = 10;
			UINT32		uiNumBytesWritten=0;

			//If the file doesnt exists, create a file that has an initial amount of Items
			hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
			if( hFile == 0 )
			{
				//Error opening item modification file
				return( FALSE );
			}

			//write the the number of item in the maps item file
			FileWrite( hFile, &uiNumberOfItems, sizeof( UINT32 ), &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( UINT32 ) )
			{
				//Error Writing size of array to disk
				FileClose( hFile );
				return( FALSE );
			}

			//write the the number of item in the maps item file
			for (unsigned int x = 0; x < uiNumberOfItems; ++x)
			{
				if ( !TempWorldItems[x].Save(hFile, FALSE) )
				{
					//Error Writing size of array to disk
					FileClose( hFile );
					return( FALSE );
				}
			}

			//Close the file
			FileClose( hFile );
		}
		else
		{
			// the file doesnt exist
			*pSizeOfData = 0;

			return( TRUE );
		}
	}


	//Open the file for reading, if it exists
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}


	// Load the size of the World ITem table
	FileRead( hFile, &uiNumberOfItems, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}

	*pSizeOfData = uiNumberOfItems;

	FileClose( hFile );

	return( TRUE );
}

UINT32 GetNumberOfMovableItems( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	// not underground!
	if ( bMapZ )
		return 0;

	// open the inventory of the sector we are taking stuff from
	SECTORINFO *pSectorInfo_Target = &(SectorInfo[SECTOR( sMapX, sMapY )]);
	UINT32 uiTotalNumberOfRealItems_Target = 0;

	// use the new map
	std::vector<WORLDITEM> pWorldItem_Target;

	if ( (gWorldSectorX == sMapX) && (gWorldSectorY == sMapY) && (gbWorldSectorZ == bMapZ) )
	{
		uiTotalNumberOfRealItems_Target = guiNumWorldItems;
		pWorldItem_Target = gWorldItems;
	}
	else
	{
		// not loaded, load
		// get total number, visable and invisible
		BOOLEAN fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &(uiTotalNumberOfRealItems_Target), FALSE );
		Assert( fReturn );

		if ( uiTotalNumberOfRealItems_Target > 0 )
		{
			// allocate space for the list
			pWorldItem_Target.resize( uiTotalNumberOfRealItems_Target );//dnl ch75 271013

			// now load into mem
			LoadWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, pWorldItem_Target );
		}
	}
	
	UINT32 validitems = 0;

	for ( UINT32 uiCount = 0; uiCount < uiTotalNumberOfRealItems_Target; ++uiCount )				// ... for all items in the world ...
	{
		if ( pWorldItem_Target[uiCount].fExists )										// ... if item exists ...
		{
			// test wether item is reachable and allowed to be moved by this assignment
			if ( (pWorldItem_Target[uiCount].usFlags & WORLD_ITEM_REACHABLE) && !(pWorldItem_Target[uiCount].usFlags & WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE) && pWorldItem_Target[uiCount].bVisible > 0 )
			{
				OBJECTTYPE* pObj = &(pWorldItem_Target[uiCount].object);			// ... get pointer for this item ...

				if ( pObj != NULL && pObj->exists( ) )												// ... if pointer is not obviously useless ...
				{
					++validitems;
				}
			}
		}
	}

	return validitems;
}


BOOLEAN AddItemsToUnLoadedSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ, INT32 sGridNo, UINT32 uiNumberOfItemsToAdd, OBJECTTYPE *pObject, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 bVisible, BOOLEAN fReplaceEntireFile )
{
	UINT32	uiNumberOfItems=0;
	std::vector<WORLDITEM> pWorldItems;//dnl ch75 271013
	UINT32	cnt;
	UINT32	uiLoop1=0;
	
	if ( uiNumberOfItemsToAdd == 0 && fReplaceEntireFile == FALSE )
	{
		//Moa: nothing to do, so get out of here!
		return( TRUE );
	}

	if( !GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &uiNumberOfItems, TRUE ) )
	{
		//Errror getting the numbers of the items from the sector
		return( FALSE );
	}

	//Allocate memeory for the item
	pWorldItems.resize(uiNumberOfItems);

	//Load in the sectors Item Info
	if( !LoadWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, pWorldItems ) )
	{
		//error reading in the items from the Item mod file
		return( FALSE );
	}


	//if we are to replace the entire file
	if( fReplaceEntireFile )
	{
		//first loop through and mark all entries that they dont exists
		for( cnt=0; cnt<uiNumberOfItems; cnt++)
			pWorldItems[ cnt ].fExists = FALSE;

		//Now delete the item temp file
		DeleteTempItemMapFile( sMapX, sMapY, bMapZ );
	}

	UINT16 uiLastFoundSpot = 0;
	//loop through all the objects to add
	for( uiLoop1=0; uiLoop1 < uiNumberOfItemsToAdd; uiLoop1++)
	{
		//Loop through the array to see if there is a free spot to add an item to it
		for( cnt=uiLastFoundSpot; cnt < uiNumberOfItems; cnt++)
		{
			if( pWorldItems[ cnt ].fExists == FALSE )
			{
				// anv: remember first found free spot to not loop through entire inventory again for next added items
				uiLastFoundSpot = cnt;
				//We have found a free spot, break
				break;
			}		}

		if( cnt == ( uiNumberOfItems ) )
		{
#if 0//dnl ch75 271013
			//Error, there wasnt a free spot.  Reallocate memory for the array
			WORLDITEM* pTemp = new WORLDITEM[uiNumberOfItems + 1];
			if( pTemp == NULL )
			{
				//error realloctin memory
				return( FALSE );
			}
			for (unsigned int x = 0; x < uiNumberOfItems; ++x) {
				pTemp[x] = pWorldItems[x];
			}
			delete[] pWorldItems;
			pWorldItems = pTemp;

			//Increment the total number of item in the array
			uiNumberOfItems++;

			//set the spot were the item is to be added
			cnt = uiNumberOfItems - 1;
#else
			pWorldItems.resize(++uiNumberOfItems);
#endif
		}

		pWorldItems[ cnt ].fExists = TRUE;
		pWorldItems[ cnt ].sGridNo = sGridNo;
		pWorldItems[ cnt ].ubLevel = ubLevel;
		pWorldItems[ cnt ].usFlags = usFlags;
		pWorldItems[ cnt ].bVisible = bVisible;
		pWorldItems[ cnt ].bRenderZHeightAboveLevel = bRenderZHeightAboveLevel;


		//Check		
		if(TileIsOutOfBounds(sGridNo) && !( pWorldItems[ cnt ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )
		{
			pWorldItems[ cnt ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;

			// Display warning.....
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Error: Trying to add item ( %d: %s ) to invalid gridno in unloaded sector. Please Report.", pWorldItems[ cnt ].object.usItem, ItemNames[pWorldItems[ cnt ].object.usItem] );
		}


		pWorldItems[ cnt ].object = pObject[uiLoop1];

		if ( (*pObject)[uiLoop1]->data.sObjectFlag & TAKEN_BY_MILITIA_TABOO_GREEN )
		{
			(*pObject)[uiLoop1]->data.sObjectFlag &= ~TAKEN_BY_MILITIA_TABOO_GREEN;
			pWorldItems[ cnt ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN;
		}
		if ( (*pObject)[uiLoop1]->data.sObjectFlag & TAKEN_BY_MILITIA_TABOO_BLUE )
		{
			(*pObject)[uiLoop1]->data.sObjectFlag &= ~TAKEN_BY_MILITIA_TABOO_BLUE;
			pWorldItems[ cnt ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE;
		}
	}

	//Save the Items to the the file
	SaveWorldItemsToTempItemFile( sMapX, sMapY, bMapZ, uiNumberOfItems, pWorldItems );

	return( TRUE );
}

extern BOOLEAN gfInMeanwhile;
extern BOOLEAN EnableModifiedFileSetCache(BOOLEAN value);

BOOLEAN SaveCurrentSectorsInformationToTempItemFile( )
{
	BOOLEAN fShouldBeInMeanwhile = FALSE;
	if( gfWasInMeanwhile )
	{ //Don't save a temp file for the meanwhile scene map.
		gfWasInMeanwhile = FALSE;
		return TRUE;
	}
#ifdef JA2UB
//Ja25v No meanwhiles
#else
	else if( AreInMeanwhile() )
	{
		gfInMeanwhile = FALSE;
		fShouldBeInMeanwhile = TRUE;
	}
#endif
	//If we havent been to tactical yet
	if( ( gWorldSectorX == 0 ) && ( gWorldSectorY == 0 ) )
	{
		return( TRUE );
	}

	BOOLEAN cacheResetValue = EnableModifiedFileSetCache(TRUE);

	//Save the Blood, smell and the revealed status for map elements
	SaveBloodSmellAndRevealedStatesFromMapToTempFile();

	// sevenfm
	SaveMineFlagFromMapToTempFile();

	// handle all reachable before save
	HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Save the Items to the the file
	if( !SaveWorldItemsToTempItemFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, guiNumWorldItems, gWorldItems ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveWorldItemsToTempItemFile()" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	std::vector<ROTTING_CORPSE_DEFINITION> corpsedefvector;

	//Determine how many rotting corpses there are
	for ( INT32 iCount = 0; iCount < giNumRottingCorpse; ++iCount )
	{
		if ( gRottingCorpse[iCount].fActivated )
			corpsedefvector.push_back( gRottingCorpse[iCount].def );
	}

	//Save the rotting corpse array to the temp rotting corpse file
	if( !SaveRottingCorpsesToTempCorpseFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, corpsedefvector ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveRottingCorpsesToTempCorpseFile()" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//save the Doortable array to the temp door map file
	if( !SaveDoorTableToDoorTableTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveDoorTableToDoorTableTempFile()" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//save the 'revealed'status of the tiles
	if( !SaveRevealedStatusArrayToRevealedTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveRevealedStatusArrayToRevealedTempFile()" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//save the door open status to the saved game file
	if( !SaveDoorStatusArrayToDoorStatusTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveDoorStatusArrayToDoorStatusTempFile()" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//Save the enemies to the temp file
	if( !NewWayOfSavingEnemyAndCivliansToTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, TRUE, FALSE ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in NewWayOfSavingEnemyAndCivliansToTempFile( Enemy, Creature Team )" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//Save the civilian info to the temp file
	if( !NewWayOfSavingEnemyAndCivliansToTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, FALSE, FALSE ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in NewWayOfSavingEnemyAndCivliansToTempFile( Civ Team )" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//Save the smoke effects info to the temp file
	if( !SaveSmokeEffectsToMapTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveSmokeEffectsToMapTempFile" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}

	//Save the smoke effects info to the temp file
	if( !SaveLightEffectsToMapTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("SaveCurrentSectorsInformationToTempItemFile:  failed in SaveLightEffectsToMapTempFile" ) );
		EnableModifiedFileSetCache(cacheResetValue);
		return( FALSE );
	}


//ttt

	//Save any other info here


	//Save certain information from the NPC's soldier structure to the Merc structure
	SaveNPCInformationToProfileStruct( );

	//Save the time the player was last in the sector
	SetLastTimePlayerWasInSector();

	EnableModifiedFileSetCache(cacheResetValue);

#ifdef JA2UB
//Ja25 no meanwhile
#else
	if( fShouldBeInMeanwhile )
	{
		gfInMeanwhile = TRUE;
	}
#endif
	return( TRUE );
}


void HandleAllReachAbleItemsInTheSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	// find out which items in the list are reachable
	UINT32 uiCounter = 0;
	UINT8	ubDir, ubMovementCost;
	BOOLEAN fReachable = FALSE;
	INT32 sGridNo = NOWHERE, sGridNo2 = NOWHERE;
	INT32	sNewLoc;

	SOLDIERTYPE * pSoldier;
	BOOLEAN	fSecondary = FALSE;

	if ( guiNumWorldItems == 0 )
	{
		return;
	}

	if ( gMapInformation.sCenterGridNo != -1 )
	{
		sGridNo = gMapInformation.sCenterGridNo;
	}
	else if ( gMapInformation.sNorthGridNo != -1 )
	{
		// use any!
		sGridNo = gMapInformation.sNorthGridNo;
	}
	else if ( gMapInformation.sEastGridNo != -1 )
	{
		// use any!
		sGridNo = gMapInformation.sEastGridNo;
	}
	else if ( gMapInformation.sSouthGridNo != -1 )
	{
		// use any!
		sGridNo = gMapInformation.sSouthGridNo;
	}
	else if ( gMapInformation.sWestGridNo != -1 )
	{
		// use any!
		sGridNo = gMapInformation.sWestGridNo;
	}
	else
	{
		AssertMsg( 0, String( "Map %c%d_b%d does not have any entry points!", 'A' + gWorldSectorY - 1, gWorldSectorX, gbWorldSectorZ ) );
		return;
	}

	if ( gMapInformation.sIsolatedGridNo != -1 )
	{
		sGridNo2 = gMapInformation.sIsolatedGridNo;

		for( uiCounter = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; uiCounter <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++uiCounter )
		{
			pSoldier = MercPtrs[ uiCounter ];
			if ( pSoldier && pSoldier->bActive && pSoldier->stats.bLife > 0 && pSoldier->sSectorX == sSectorX && pSoldier->sSectorY == sSectorY && pSoldier->bSectorZ == bSectorZ )
			{
				if ( FindBestPath( pSoldier, sGridNo2, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, 0 ) )
				{
					fSecondary = TRUE;
					break;
				}
			}
		}

		if ( !fSecondary )
		{
			sGridNo2 = NOWHERE;
		}
	}

	GlobalItemsReachableTest( sGridNo, sGridNo2 );

	for( uiCounter = 0; uiCounter < guiNumWorldItems; ++uiCounter )
	{
		// reset reachablity
		fReachable = FALSE;

		// item doesn't exist, ignore it
		if( gWorldItems[ uiCounter ].fExists == FALSE )
		{
			continue;
		}

		// if the item is trapped then flag it as unreachable, period
		if ( gWorldItems[ uiCounter ].object[0]->data.bTrap > 0 )
		{
			fReachable = FALSE;
		}
		else if ( ItemTypeExistsAtLocation( gWorldItems[ uiCounter ].sGridNo, OWNERSHIP, gWorldItems[ uiCounter ].ubLevel, NULL ) )
		{
			fReachable = FALSE;
		}
		else if ( gWorldItems[ uiCounter ].object.usItem == CHALICE )
		{
			fReachable = FALSE;
		}
		else if ( gpWorldLevelData[ gWorldItems[ uiCounter ].sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
		{
			// the gridno itself is reachable so the item is reachable
			fReachable = TRUE;
		}
		else if ( gWorldItems[ uiCounter ].ubLevel > 0 )
		{
			// items on roofs are always reachable
			fReachable = TRUE;
		}
		else
		{
			// check the 4 grids around the item, if any is reachable...then the item is reachable
			for ( ubDir = 0; ubDir < NUM_WORLD_DIRECTIONS; ubDir += 2 )
			{
				sNewLoc = NewGridNo( gWorldItems[ uiCounter ].sGridNo, DirectionInc( ubDir ) );
				if ( sNewLoc != gWorldItems[ uiCounter ].sGridNo )
				{
					// then it's a valid gridno, so test it
					// requires non-wall movement cost from one location to the other!
					if ( gpWorldLevelData[ sNewLoc ].uiFlags & MAPELEMENT_REACHABLE )
					{
 						ubMovementCost = gubWorldMovementCosts[ gWorldItems[ uiCounter ].sGridNo ][ gOppositeDirection[ ubDir ] ][ 0 ];
						// if we find a door movement cost, if the door is open the gridno should be accessible itself
						if ( ubMovementCost != TRAVELCOST_DOOR && ubMovementCost != TRAVELCOST_WALL )
						{
							fReachable = TRUE;
							break;
						}
					}
				}
			}
		}

		if( fReachable )
		{
			gWorldItems[ uiCounter ].usFlags |= WORLD_ITEM_REACHABLE;
		}
		else
		{
			gWorldItems[ uiCounter ].usFlags &= ~( WORLD_ITEM_REACHABLE );
		}
	}
}


BOOLEAN LoadCurrentSectorsInformationFromTempItemsFile()
{
	BOOLEAN fUsedTempFile = FALSE;

	//
	// Load in the sectors ITems
	//
#ifdef JA2UB
//Ja25v no meanwhiles
#else
	if( AreInMeanwhile() )
	{ //There will never be a temp file for the meanwhile scene, so return TRUE.  However,
		//set a flag to not save it either!
		gfWasInMeanwhile = TRUE;

		// OK  - this is true except for interrotations - we need that item temp file to be
		// processed!
		if ( GetMeanwhileID() == INTERROGATION )
		{
			//If there is a file, load in the Items array
			if( DoesTempFileExistsForMap( SF_ITEM_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
			{
				if( !LoadAndAddWorldItemsFromTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
					return( FALSE );
			}

			gfWasInMeanwhile = FALSE;
		}
		return TRUE;
	}
#endif
	//if we are in an above ground sector

	//If there is a map modifications file, load the data from the temp file
	if ( DoesTempFileExistsForMap( SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if ( !LoadAllMapChangesFromMapTempFileAndApplyThem() )
			return( FALSE );
	}

	//If there is a file, load in the Items array
	if( DoesTempFileExistsForMap( SF_ITEM_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadAndAddWorldItemsFromTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
			return( FALSE );
	}

	//If there is a rotting corpse temp file, load the data from the temp file
	if( DoesTempFileExistsForMap( SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadRottingCorpsesFromTempCorpseFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
			return( FALSE );
	}

	//if there is a door table temp file, load the data from the temp file
	if( DoesTempFileExistsForMap( SF_DOOR_TABLE_TEMP_FILES_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadDoorTableFromDoorTableTempFile( ) )
			return( FALSE );
	}

	//if there is a revealed status temp file, load the data from the temp file
	if( DoesTempFileExistsForMap( SF_REVEALED_STATUS_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadRevealedStatusArrayFromRevealedTempFile( ) )
			return( FALSE );
	}

	//if there is a door status temp file, load the data from the temp file
	if( DoesTempFileExistsForMap( SF_DOOR_STATUS_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadDoorStatusArrayFromDoorStatusTempFile( ) )
			return( FALSE );
	}


	//if the save is an older version, use the old way of loading it up
	if( guiCurrentSaveGameVersion < 57 )
	{
		if( DoesTempFileExistsForMap( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
		{
			fUsedTempFile = TRUE;
			if( !LoadEnemySoldiersFromTempFile( ) )
				return( FALSE );
		}
	}

	//else use the new way of loading the enemy and civilian placements
	else
	{
		if( DoesTempFileExistsForMap( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
		{
			fUsedTempFile = TRUE;
			if( !NewWayOfLoadingEnemySoldiersFromTempFile( ) )
				return( FALSE );
		}

		if( DoesTempFileExistsForMap( SF_CIV_PRESERVED_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
		{
			fUsedTempFile = TRUE;
			if( !NewWayOfLoadingCiviliansFromTempFile( ) )
				return( FALSE );
		}
	}

	if( DoesTempFileExistsForMap( SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadSmokeEffectsFromMapTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
			return( FALSE );
	}

	if( DoesTempFileExistsForMap( SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS, gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fUsedTempFile = TRUE;
		if( !LoadLightEffectsFromMapTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
			return( FALSE );
	}

//ttt

	// Check to see if any npc are in this sector, if so load up some saved data for them
	LoadNPCInformationFromProfileStruct();

	//if we are loading a saved game
//	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
	{
		//Init the world since we have modified the map
		InitLoadedWorld();
	}


	// Get the last time the player was in the sector
	guiTimeCurrentSectorWasLastLoaded = GetLastTimePlayerWasInSector();

	if( fUsedTempFile )
	{
//		ValidateSoldierInitLinks( 3 );
	}


	StripEnemyDetailedPlacementsIfSectorWasPlayerLiberated();

	return( TRUE );
}


void SetLastTimePlayerWasInSector()
{
	if( !gbWorldSectorZ )
		SectorInfo[ SECTOR( gWorldSectorX,gWorldSectorY) ].uiTimeCurrentSectorWasLastLoaded = GetWorldTotalMin();
	else if( gbWorldSectorZ > 0 )
	{
		UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

		pTempNode = gpUndergroundSectorInfoHead;

		//loop through and look for the right underground sector
		while( pTempNode )
		{
			if( ( pTempNode->ubSectorX == gWorldSectorX ) &&
					( pTempNode->ubSectorY == gWorldSectorY ) &&
					( pTempNode->ubSectorZ == gbWorldSectorZ ) )
			{
				//set the flag indicating that ther is a temp item file exists for the sector
				pTempNode->uiTimeCurrentSectorWasLastLoaded = GetWorldTotalMin();
				return;	//break out
			}
			pTempNode = pTempNode->next;
		}
		#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Failed to Set the 'uiTimeCurrentSectorWasLastLoaded' for an underground sector" );
		#endif
	}
}

// Flugente: set last time sector was visited
void SetLastTimePlayerWasInSector(INT16 sMapX, INT16 sMapY, INT8 sMapZ)
{
	if( !sMapZ )
		SectorInfo[ SECTOR( sMapX,sMapY) ].uiTimeCurrentSectorWasLastLoaded = GetWorldTotalMin();
	else if( sMapZ > 0 )
	{
		UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

		pTempNode = gpUndergroundSectorInfoHead;

		//loop through and look for the right underground sector
		while( pTempNode )
		{
			if( ( pTempNode->ubSectorX == sMapX ) &&
					( pTempNode->ubSectorY == sMapY ) &&
					( pTempNode->ubSectorZ == sMapZ ) )
			{
				//set the flag indicating that ther is a temp item file exists for the sector
				pTempNode->uiTimeCurrentSectorWasLastLoaded = GetWorldTotalMin();
				return;	//break out
			}
			pTempNode = pTempNode->next;
		}
		#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Failed to Set the 'uiTimeCurrentSectorWasLastLoaded' for an underground sector" );
		#endif
	}
}


UINT32 GetLastTimePlayerWasInSector()
{
	if( !gbWorldSectorZ )
		return( SectorInfo[ SECTOR( gWorldSectorX,gWorldSectorY) ].uiTimeCurrentSectorWasLastLoaded );
	else if( gbWorldSectorZ > 0 )
	{
		UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

		pTempNode = gpUndergroundSectorInfoHead;

		//loop through and look for the right underground sector
		while( pTempNode )
		{
			if( ( pTempNode->ubSectorX == gWorldSectorX ) &&
					( pTempNode->ubSectorY == gWorldSectorY ) &&
					( pTempNode->ubSectorZ == gbWorldSectorZ ) )
			{
				//set the flag indicating that ther is a temp item file exists for the sector
				return( pTempNode->uiTimeCurrentSectorWasLastLoaded );
			}
			pTempNode = pTempNode->next;
		}

		#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Failed to Get the 'uiTimeCurrentSectorWasLastLoaded' from an underground sector" );
		#endif

		return( 0 );
	}
	return( 0 );
}

// Flugente: get time for another sector
UINT32 GetLastTimePlayerWasInSector(INT16 sMapX, INT16 sMapY, INT8 sMapZ)
{
	if( !sMapZ )
		return( SectorInfo[ SECTOR( sMapX,sMapY) ].uiTimeCurrentSectorWasLastLoaded );
	else if( sMapZ > 0 )
	{
		UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

		pTempNode = gpUndergroundSectorInfoHead;

		//loop through and look for the right underground sector
		while( pTempNode )
		{
			if( ( pTempNode->ubSectorX == sMapX ) &&
					( pTempNode->ubSectorY == sMapY ) &&
					( pTempNode->ubSectorZ == sMapZ ) )
			{
				//set the flag indicating that ther is a temp item file exists for the sector
				return( pTempNode->uiTimeCurrentSectorWasLastLoaded );
			}
			pTempNode = pTempNode->next;
		}

		#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Failed to Get the 'uiTimeCurrentSectorWasLastLoaded' from an underground sector" );
		#endif

		return( 0 );
	}
	return( 0 );
}




BOOLEAN LoadAndAddWorldItemsFromTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32	uiNumberOfItems=0;
	std::vector<WORLDITEM> pWorldItems;//dnl ch75 271013
	UINT32	cnt;
	INT32 sNewGridNo;

	//Get the number of items from the file
	if( !GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &uiNumberOfItems, TRUE ) )
	{
		//Error getting the numbers of the items from the sector
		return( FALSE );
	}

	if( uiNumberOfItems )
	{
		pWorldItems.resize(uiNumberOfItems);//dnl ch75 271013
	}
	else
	{
		//if there are no items in the temp, the player might have cleared all of them out, check to see
		//If we have already been to the sector
		if( GetSectorFlagStatus( sMapX, sMapY, bMapZ, SF_ALREADY_LOADED ) )
		{
			//
			//Completly replace the current sectors item table because all the items SHOULD be in the temp file!!
			//

			//Destroy the current sectors item table
			TrashWorldItems();

			//Add each item to the pool, handle below, outside of the if
		}

		//there are no items in the file
		return( TRUE );
	}

	//Load the World Items from the file
	if( !LoadWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, pWorldItems ) )
	{
		return( FALSE );
	}


	//If we have already been to the sector
	if( GetSectorFlagStatus( sMapX, sMapY, bMapZ, SF_ALREADY_LOADED ) )
	{
		//
		//Completly replace the current sectors item table because all the items SHOULD be in the temp file!!
		//

		//Destroy the current sectors item table
		TrashWorldItems();

		//Add each item to the pool, handle below, outside of the if
	}

	//
	//Append the items in the file with to the current sectors item table
	//

	//Loop through all the items loaded from the file
	for( cnt=0; cnt< uiNumberOfItems; cnt++)
	{
		//If the item in the array is valid
		if( pWorldItems[cnt].fExists )
		{
			//Check the flags to see if we have to find a gridno to place the items at
			if( pWorldItems[cnt].usFlags & WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO )
			{
				sNewGridNo = FindNearestAvailableGridNoForItem( pWorldItems[cnt].sGridNo, 5 );
								
				if(TileIsOutOfBounds(sNewGridNo))
				  sNewGridNo = FindNearestAvailableGridNoForItem( pWorldItems[cnt].sGridNo, 15 );
				
				if (!TileIsOutOfBounds(sNewGridNo))
				{
					pWorldItems[cnt].sGridNo = sNewGridNo;
				}
			}

			//If the item has an invalid gridno, use the maps entry point
			if( pWorldItems[cnt].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT )
			{
				pWorldItems[cnt].sGridNo = gMapInformation.sCenterGridNo;

				// if we move the item, recalculate the height
				pWorldItems[cnt].bRenderZHeightAboveLevel = -1;
			}

			//add the item to the world
			AddItemToPool( pWorldItems[cnt].sGridNo, &pWorldItems[cnt].object, pWorldItems[cnt].bVisible, pWorldItems[cnt].ubLevel, pWorldItems[cnt].usFlags, pWorldItems[cnt].bRenderZHeightAboveLevel, pWorldItems[cnt].soldierID );
		}
	}

	return( TRUE );
}





BOOLEAN AddTempFileToSavedGame( HWFILE hFile, UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	CHAR8		zMapName[ 128 ];

	GetMapTempFileName( uiType, zMapName, sMapX, sMapY, bMapZ );

	//Save the map temp file to the saved game file
	if( !SaveFilesToSavedGame( zMapName, hFile ) )
		return( FALSE );

	return( TRUE );
}



BOOLEAN RetrieveTempFileFromSavedGame( HWFILE hFile, UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	CHAR8		zMapName[ 128 ];

	GetMapTempFileName( uiType, zMapName, sMapX, sMapY, bMapZ );

	//Load the map temp file from the saved game file
 	if( !LoadFilesFromSavedGame( zMapName, hFile ) )
		return( FALSE );

	return( TRUE );
}



//Deletes the Temp map Directory
BOOLEAN InitTacticalSave( BOOLEAN fCreateTempDir )
{
#ifndef USE_VFS
	UINT32	uiRetVal;

	//If the Map Temp directory exists, removes the temp files
	uiRetVal = FileGetAttributes( MAPS_DIR );
	if( uiRetVal != 0xFFFFFFFF )
	{
		if( uiRetVal & FILE_ATTRIBUTES_DIRECTORY )
		{
			//Erase the directory
			if( !EraseDirectory( MAPS_DIR ) )
			{
				//error erasing the temporary maps directory
			}
		}
	}
	else
	{
		if( !MakeFileManDirectory( MAPS_DIR ) )
		{
			//Erro creating the temp map directory
			AssertMsg( 0, "Error creating the Temp Directory.");
		}
	}
#else
	EraseDirectory( MAPS_DIR );
#endif
	if( fCreateTempDir )
	{
		//Create the initial temp file for the Npc Quote Info
		InitTempNpcQuoteInfoForNPCFromTempFile();
	}
 	return( TRUE );
}


BOOLEAN SaveRottingCorpsesToTempCorpseFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, std::vector<ROTTING_CORPSE_DEFINITION> aCorpseDefVector )
{
	HWFILE	hFile;
	UINT32	uiNumBytesWritten=0;
	CHAR8	zMapName[ 128 ];
	
	// if we have no corpses at all, we have nothing to write - erase the 'corpse file exists flag', the next time we add a corpse, a new file will be created
	if ( aCorpseDefVector.empty() )
	{
		ReSetSectorFlag( sMapX, sMapY, bMapZ, SF_ROTTING_CORPSE_TEMP_FILE_EXISTS );

		return TRUE;
	}

	GetMapTempFileName( SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );
	
	//Open the file for writing, Create it if it doesnt exist
	hFile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file
		return( FALSE );
	}
	
	UINT32 uiNumberOfCorpses = aCorpseDefVector.size();
	
	//Save the number of the Rotting Corpses array table
	FileWrite( hFile, &uiNumberOfCorpses, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}

	//Loop through all the carcases in the array and save the active ones
	for( std::vector<ROTTING_CORPSE_DEFINITION>::iterator it = aCorpseDefVector.begin(); it != aCorpseDefVector.end(); ++it )
	{
		//Save the RottingCorpse info array
		FileWrite( hFile, &(*it), sizeof( ROTTING_CORPSE_DEFINITION ), &uiNumBytesWritten );
		if ( uiNumBytesWritten != sizeof( ROTTING_CORPSE_DEFINITION ) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}
	}

	FileClose( hFile );

	// Set the flag indicating that there is a rotting corpse Temp File
	SetSectorFlag( sMapX, sMapY, bMapZ, SF_ROTTING_CORPSE_TEMP_FILE_EXISTS );

	return( TRUE );
}

BOOLEAN DeleteTempItemMapFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT8 bSectorId = 0;
//	CHAR8		zTempName[ 128 ];
	CHAR8		zMapName[ 128 ];

	// grab the sector id
	bSectorId = SECTOR( sMapX, sMapY );
/*
	//Convert the current sector location into a file name
	GetMapFileName( sMapX,sMapY, bMapZ, zTempName, FALSE );

	sprintf( zMapName, "%s\\r_%s", MAPS_DIR, zTempName);
*/
	GetMapTempFileName( SF_ITEM_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	// the sector info flag being reset
	ReSetSectorFlag( sMapX, sMapY, bMapZ, SF_ITEM_TEMP_FILE_EXISTS );

	return( TRUE );
}


BOOLEAN LoadRottingCorpsesFromTempCorpseFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	HWFILE	hFile;
	UINT32	uiNumBytesRead=0;
	CHAR8		zMapName[ 128 ];
	UINT32	uiNumberOfCorpses=0;
	UINT32		cnt;
	ROTTING_CORPSE_DEFINITION		def;
	BOOLEAN                     fDontAddCorpse = FALSE;
	INT8                        bTownId;
	
	//Delete the existing rotting corpse array
	RemoveCorpses( );

	GetMapTempFileName( SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );
	
	//Check to see if the file exists
	if( !FileExists( zMapName ) )
	{
		//If the file doesnt exists, its no problem.
		return( TRUE );
	}

	//Open the file for reading
	hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hFile == 0 )
	{
		//Error opening map modification file,
		return( FALSE );
	}

	// Load the number of Rotting corpses
	FileRead( hFile, &uiNumberOfCorpses, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		//Error Writing size of array to disk
		FileClose( hFile );
		return( FALSE );
	}
	
	// WANNE: If we get wrong data (very high value) from the temp file, we assume we have wrong value and therefore set corpses to 0 which will not crash the game!
	if (uiNumberOfCorpses > 10000)
		uiNumberOfCorpses = 0;

	// Get town ID for use later....
	bTownId = GetTownIdForSector( gWorldSectorX, gWorldSectorY );

	for( cnt=0; cnt<uiNumberOfCorpses; ++cnt )
	{
		fDontAddCorpse = FALSE;

		// Load the Rotting corpses info
		FileRead( hFile, &def, sizeof( ROTTING_CORPSE_DEFINITION ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( ROTTING_CORPSE_DEFINITION ) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}
		
		//Check the flags to see if we have to find a gridno to place the rotting corpses at
		if( def.usFlags & ROTTING_CORPSE_FIND_SWEETSPOT_FROM_GRIDNO )
		{
			def.sGridNo = FindNearestAvailableGridNoForCorpse( &def, 5 );
			
			if (TileIsOutOfBounds(def.sGridNo))
			{
				def.sGridNo = FindNearestAvailableGridNoForCorpse(&def, 15);
				if (TileIsOutOfBounds(def.sGridNo))
				{
					//shadooow: if we still have invalid gridno skip adding this corpse, it will crash otherwise
					continue;
				}
			}
		}
		else if( def.usFlags & ROTTING_CORPSE_USE_NORTH_ENTRY_POINT )
		{
			def.sGridNo = gMapInformation.sNorthGridNo;
		}
		else if( def.usFlags & ROTTING_CORPSE_USE_SOUTH_ENTRY_POINT )
		{
			def.sGridNo = gMapInformation.sSouthGridNo;
		}
		else if( def.usFlags & ROTTING_CORPSE_USE_EAST_ENTRY_POINT )
		{
			def.sGridNo = gMapInformation.sEastGridNo;
		}
		else if( def.usFlags & ROTTING_CORPSE_USE_WEST_ENTRY_POINT )
		{
			def.sGridNo = gMapInformation.sWestGridNo;
		}
		//Recalculate the dx,dy info
		def.dXPos = CenterX( def.sGridNo );
		def.dYPos = CenterY( def.sGridNo );
		// If not from loading a save....
		if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
		{
			// ATE: Don't place corpses if
			// a ) in a town and b) indoors
			if ( gbWorldSectorZ == 0 )
			{
				if ( bTownId != BLANK_SECTOR )
				{
					// Are we indoors?
					if( FloorAtGridNo( def.sGridNo ) )
					{
						// OK, finally, check TOC vs game time to see if at least some time has passed
						if ( ( GetWorldTotalMin( ) - def.uiTimeOfDeath ) >= 30 )
						{
							fDontAddCorpse = TRUE;
						}
					}
				}
			}
		}

		if ( !fDontAddCorpse )
		{
			//add the rotting corpse info
			if( AddRottingCorpse( &def ) == -1 )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Failed to add a corpse to GridNo # %d", def.sGridNo ) );
				/*
				Assert( 0 );
				FileClose( hFile );
				return( FALSE );
				*/
			}
		}
	}

	FileClose( hFile );

	//Check to see if we have to start decomposing the corpses
	HandleRottingCorpses( );

	return( TRUE );
}


// Rewritten to load the temp file once, update with the list, and then write it.  This was getting insane as items piled up in sectors.
// A few dozen read, update, writes was okay but a few hundred is pushing it.

BOOLEAN AddWorldItemsToUnLoadedSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ, INT32 sGridNo, UINT32 uiNumberOfItemsToAdd, std::vector<WORLDITEM>& pWorldItem, BOOLEAN fOverWrite )//dnl ch75 271013
{
	UINT32 uiLoop;
	UINT32 uiLastItemPos;
	UINT32 uiNumberOfItems;
	std::vector<WORLDITEM> pWorldItems;//dnl ch75 271013

	if( !GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &uiNumberOfItems, TRUE ) )
	{
		//Errror getting the numbers of the items from the sector
		return( FALSE );
	}

	//Allocate memory for the item
	pWorldItems.resize(uiNumberOfItems);//dnl ch75 271013

	//Load in the sectors Item Info
	if( !LoadWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, pWorldItems ) )
	{
		//error reading in the items from the Item mod file
		return( FALSE );
	}

	//if we are to replace the entire file
	if( fOverWrite )
	{
		//first loop through and mark all entries that they dont exists
		for( UINT32 cnt=0; cnt<uiNumberOfItems; ++cnt)
			pWorldItems[ cnt ].fExists = FALSE;

		//Now delete the item temp file
		DeleteTempItemMapFile( sMapX, sMapY, bMapZ );
	}

	uiLastItemPos = 0;

	//loop through all the objects to add
	for( uiLoop=0; uiLoop < uiNumberOfItemsToAdd; ++uiLoop)
	{
		// Flugente: only add an item if it exists. Otherwise we will happily add items that are supposed to have been deleted
		if ( !pWorldItem[uiLoop].fExists )
			continue;

		//Loop through the array to see if there is a free spot to add an item to it
		for( ; uiLastItemPos < uiNumberOfItems; ++uiLastItemPos)
		{
			if( pWorldItems[ uiLastItemPos ].fExists == FALSE )
			{
				//We have found a free spot, break
				break;
			}
		}

		if( uiLastItemPos == ( uiNumberOfItems ) )
		{
#if 0//dnl ch75 271013
			//Error, there wasnt a free spot.  Reallocate memory for the array
			WORLDITEM* pTemp;
			pTemp = new WORLDITEM [uiNumberOfItems + 1];
			if( pTemp == NULL )
			{
				//error realloctin memory
				return( FALSE );
			}
			for (unsigned int x = 0; x < uiNumberOfItems; ++x) {
				pTemp[x] = pWorldItems[x];
			}
			delete[] pWorldItems;
			pWorldItems = pTemp;

			//Increment the total number of item in the array
			uiNumberOfItems++;
#else
			pWorldItems.resize(++uiNumberOfItems);
#endif
		}

		pWorldItems[ uiLastItemPos ].fExists = TRUE;
		pWorldItems[ uiLastItemPos ].sGridNo = pWorldItem[ uiLoop ].sGridNo;
		pWorldItems[ uiLastItemPos ].ubLevel = pWorldItem[ uiLoop ].ubLevel;
		pWorldItems[ uiLastItemPos ].usFlags = pWorldItem[ uiLoop ].usFlags;
		pWorldItems[ uiLastItemPos ].bVisible = pWorldItem[ uiLoop ].bVisible;
		pWorldItems[ uiLastItemPos ].bRenderZHeightAboveLevel = pWorldItem[ uiLoop ].bRenderZHeightAboveLevel;
		
		//Check		
		if(TileIsOutOfBounds(pWorldItem[ uiLoop ].sGridNo) && !( pWorldItems[ uiLastItemPos ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )
		{
			pWorldItems[ uiLastItemPos ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;

			// Display warning.....
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Error: Trying to add item ( %d: %s ) to invalid gridno in unloaded sector. Please Report.", pWorldItems[ uiLoop ].object.usItem, ItemNames[pWorldItems[ uiLoop ].object.usItem] );
		}


		pWorldItems[ uiLastItemPos ].object = pWorldItem[ uiLoop ].object;
	}

	//Save the Items to the the file
	SaveWorldItemsToTempItemFile( sMapX, sMapY, bMapZ, uiNumberOfItems, pWorldItems );

#if 0
// The old excruciatingly inefficient code
	for( uiLoop=0; uiLoop<uiNumberOfItems; uiLoop++)
	{
		//If the item exists
		if( pWorldItem[uiLoop].fExists )
		{
			AddItemsToUnLoadedSector( sMapX, sMapY, bMapZ, pWorldItem[uiLoop].sGridNo, 1, &pWorldItem[ uiLoop ].object, pWorldItem[ uiLoop ].ubLevel, pWorldItem[ uiLoop ].usFlags, pWorldItem[ uiLoop ].bRenderZHeightAboveLevel, pWorldItem[ uiLoop ].bVisible, fLoop );

			fLoop = FALSE;
		}
	}
#endif

	return( TRUE );
}

void SaveNPCInformationToProfileStruct( )
{
	UINT32								cnt;
	SOLDIERTYPE *					pSoldier;
	MERCPROFILESTRUCT *		pProfile;

	//Loop through the active NPC's

	// Only do this on save now... on traversal this is handled in the strategic code
	if ( !( gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		return;
	}

	for ( cnt = 0; cnt < guiNumMercSlots; cnt++)
	{
		pSoldier = MercSlots[ cnt ];

		//if it is an active NPC
		if ( pSoldier && pSoldier->ubProfile != NO_PROFILE && pSoldier->bTeam == CIV_TEAM )
		{
			//Save Temp Npc Quote Info array
			SaveTempNpcQuoteInfoForNPCToTempFile( pSoldier->ubProfile );

			pProfile = &(gMercProfiles[ pSoldier->ubProfile ]);

			pProfile->ubQuoteActionID = pSoldier->ubQuoteActionID;
			pProfile->ubQuoteRecord = pSoldier->ubQuoteRecord;

			// if the merc is NOT added due to flag set, return
			if ( pProfile->ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR )
			{
				continue;
			}

			if ( pProfile->ubMiscFlags3 & PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE )
			{
				continue;
			}

			pProfile->fUseProfileInsertionInfo = TRUE;
			pProfile->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
			//if ( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
			//{
				// if we are saving the game, save the NPC's current location
				pProfile->usStrategicInsertionData = pSoldier->sGridNo;
			//}
			/*
			else
			{
				// If the NPC is moving, save the final destination, else save the current location
				if ( pSoldier->pathing.sFinalDestination != pSoldier->sGridNo )
				{
					pProfile->usStrategicInsertionData = pSoldier->pathing.sFinalDestination;
				}
				else
				{
					pProfile->usStrategicInsertionData = pSoldier->sGridNo;
				}
			}
			*/

		}
	}
}

extern void EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( SOLDIERTYPE *pSoldier, FLOAT dNewXPos, FLOAT dNewYPos, BOOLEAN fUpdateDest,  BOOLEAN fUpdateFinalDest );

void LoadNPCInformationFromProfileStruct()
{
	//UINT32				cnt;
	//INT16				sSoldierID;
	//SOLDIERTYPE *		pSoldier;

	// CJC: disabled this Dec 21, 1998 as unnecessary (and messing up quote files for recruited/escorted NPCs
	return;
/* commented out the disabled code (jonathanl)
	for ( cnt = FIRST_RPC; cnt < NUM_PROFILES; cnt++ )
	{
		if ( gMercProfiles[ cnt ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
		{
			// don't load
			continue;
		}

		sSoldierID = GetSoldierIDFromAnyMercID( (UINT8)cnt );

		//if the soldier is not loaded, return
		if( sSoldierID == -1 )
			continue;

		//if we cant get a pointer to the soldier, continue
		if( !GetSoldier( &pSoldier, sSoldierID ) )
			continue;

		// load quote info if it exists
		if ( gMercProfiles[ cnt ].ubMiscFlags & PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS )
		{
			LoadTempNpcQuoteInfoForNPCFromTempFile( (UINT8)cnt );
		}

		// load insertion info
		//if ( gMercProfiles[ cnt ] )
		//{
		//	pSoldier->ubInsertionCode = pProfile->ubStrategicInsertionCode;
		//	pSoldier->usInsertionData = pProfile->usStrategicInsertionData;
		//}

	}
*/

/*
	INT16 sX, sY;
	UINT16	cnt;
	SOLDIERTYPE		*pSoldier;
	INT16			sSoldierID;
	INT16		sXPos, sYPos;

	sXPos = sYPos = 0;

	//Loop through the active NPC's
//	cnt = gTacticalStatus.Team[ OUR_TEAM ].bLastID + 1;
	for(cnt=FIRST_RPC; cnt<NUM_PROFILES; cnt++)
	{
		if ( gMercProfiles[ cnt ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
		{
			// don't load
			continue;
		}

		sSoldierID = GetSoldierIDFromAnyMercID( (UINT8)cnt );

		//if the soldier is not loaded, return
		if( sSoldierID == -1 )
			continue;

		//if we cant get a pointer to the soldier, continue
		if( !GetSoldier( &pSoldier, sSoldierID ) )
			continue;

		pSoldier->ubQuoteActionID = gMercProfiles[ cnt ].ubQuoteActionID;
		pSoldier->ubQuoteRecord = gMercProfiles[ cnt ].ubQuoteRecord;

		if( gMercProfiles[ cnt ].fUseProfileInsertionInfo == PROFILE_USE_GRIDNO )
		{
			sX = CenterX( gMercProfiles[ cnt ].sGridNo );
			sY = CenterY( gMercProfiles[ cnt ].sGridNo );

			//Load the Temp Npc Quote Info array

			//if the NPC has been recruited, continue
			if( gMercProfiles[ cnt ].ubMiscFlags & PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS )
				LoadTempNpcQuoteInfoForNPCFromTempFile( (UINT8)cnt );

			//if the NPC has been recruited, continue
			if( gMercProfiles[ cnt ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
				continue;

			//If the NPC was supposed to do something when they reached their target destination
			if( pSoldier->sGridNo == pSoldier->pathing.sFinalDestination )
			{
				if (pSoldier->ubQuoteRecord && pSoldier->ubQuoteActionID == QUOTE_ACTION_ID_CHECKFORDEST )
				{
					//the mercs gridno has to be the same as the final destination
					pSoldier->EVENT_SetSoldierPosition( (FLOAT) sX, (FLOAT) sY );

					NPCReachedDestination( pSoldier, FALSE );
				}
			}

			//If the NPC's gridno is not nowhere, set him to that position
			//if( !TileIsOutOfBounds(gMercProfiles[ cnt ].sGridNo) )
			if(!TileIsOutOfBounds(gMercProfiles[ cnt ].sGridNo))
			{
				if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
				{
					//Set the NPC's destination
					pSoldier->pathing.sDestination = gMercProfiles[ cnt ].sGridNo;
					pSoldier->pathing.sDestXPos = sXPos;
					pSoldier->pathing.sDestYPos = sYPos;

					// We have moved to a diferent sector and are returning to it, therefore the merc should be in the final dest
					EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( pSoldier, (FLOAT) sX, (FLOAT) sY, FALSE, TRUE );
				}

				//else we are saving
				else
				{

					//Set the NPC's position
	//				pSoldier->EVENT_SetSoldierPosition( (FLOAT) sX, (FLOAT) sY );
					EVENT_SetSoldierPositionAndMaybeFinalDestAndMaybeNotDestination( pSoldier, (FLOAT) sX, (FLOAT) sY, FALSE, FALSE );
				}
			}
		}

		//else if we are NOT to use the gridno, dont use it but reset the flag
		else if( gMercProfiles[ cnt ].fUseProfileInsertionInfo == PROFILE_DONT_USE_GRIDNO )
		{
			gMercProfiles[ cnt ].fUseProfileInsertionInfo = PROFILE_NOT_SET;
		}
	}
*/
}


BOOLEAN GetNumberOfActiveWorldItemsFromTempFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 *pNumberOfData )
{
	UINT32	uiNumberOfItems=0;
	std::vector<WORLDITEM> pWorldItems;//dnl ch75 271013
	UINT32	cnt;
	UINT32	uiNumberOfActive=0;
	BOOLEAN	fFileLoaded = FALSE;
	UNDERGROUND_SECTORINFO *TempNode = gpUndergroundSectorInfoHead;


	//
	// Load in the sectors ITems
	//

	//If there is a file, load in the Items array
	if( bMapZ  == 0 )
	{
		if( SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & SF_ITEM_TEMP_FILE_EXISTS )
			fFileLoaded = TRUE;
	}
	else
	{
		while( TempNode )
		{
			if( TempNode->ubSectorX == sMapX && TempNode->ubSectorY == sMapY && TempNode->ubSectorZ == bMapZ )
			{
				if( TempNode->uiFlags & SF_ITEM_TEMP_FILE_EXISTS )
					fFileLoaded = TRUE;

				break;
			}

			TempNode = TempNode->next;
		}
	}

	if( fFileLoaded )
	{
		//Get the number of items from the file
		if( !GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &uiNumberOfItems, TRUE ) )
		{
			//Error getting the numbers of the items from the sector
			return( FALSE );
		}

		//If there items in the data file
		if( uiNumberOfItems != 0 )
		{
			pWorldItems.resize(uiNumberOfItems);//dnl ch75 271013

			//Load the World Items from the file
			if( !LoadWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, pWorldItems ) )
				return( FALSE );

			uiNumberOfActive = 0;
			for( cnt=0; cnt<uiNumberOfItems; cnt++ )
			{
				if( pWorldItems[cnt].fExists )
					uiNumberOfActive++;
			}
		}
		*pNumberOfData = uiNumberOfActive;
	}
	else
		*pNumberOfData = 0;

	return( TRUE );
}


BOOLEAN DoesTempFileExistsForMap( UINT32 uiType, INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UNDERGROUND_SECTORINFO *TempNode = gpUndergroundSectorInfoHead;

	if( bMapZ == 0 )
	{
		return( (SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags & uiType) ? 1 : 0 );
	}
	else
	{
		while( TempNode )
		{
			if( TempNode->ubSectorX == sMapX && TempNode->ubSectorY == sMapY && TempNode->ubSectorZ == bMapZ )
			{
				return( (TempNode->uiFlags & uiType ) ? 1 : 0 );
			}
			TempNode = TempNode->next;
		}
	}

	return( FALSE );
}


INT16 GetSoldierIDFromAnyMercID(UINT8 ubMercID)
{
	UINT16 cnt;
	UINT8		ubLastTeamID;
	SOLDIERTYPE		*pTeamSoldier;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	ubLastTeamID = TOTAL_SOLDIERS;

  // look for all mercs on the same team,
  for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= ubLastTeamID; cnt++,pTeamSoldier++)
	{
		if( pTeamSoldier->bActive )
		{
			if ( pTeamSoldier->ubProfile == ubMercID )
			{
				return( cnt );
			}
		}
	}

	return( -1 );
}




//Initializes the NPC temp array
BOOLEAN InitTempNpcQuoteInfoForNPCFromTempFile()
{
	UINT32	uiNumBytesWritten;
	UINT8	ubCnt;
	TempNPCQuoteInfoSave TempNpcQuote[ NUM_NPC_QUOTE_RECORDS ];
	UINT32	uiSizeOfTempArray = sizeof( TempNPCQuoteInfoSave ) * NUM_NPC_QUOTE_RECORDS;
	UINT16	usCnt1;
	HWFILE	hFile;


	//Open the temp npc file
	hFile = FileOpen( NPC_TEMP_QUOTE_FILE, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening temp npc quote info
		return( FALSE );
	}


	//loop through all the npc accounts and write the temp array to disk
	//for( usCnt1=0; usCnt1< ( GASTON-FIRST_RPC ); usCnt1++)
	//new profiles by Jazz
	for( usCnt1=0; usCnt1< NUM_PROFILES; usCnt1++)
	{

		memset( TempNpcQuote, 0, uiSizeOfTempArray );

		//Loop through and build the temp array to save
		for( ubCnt=0; ubCnt<NUM_NPC_QUOTE_RECORDS; ubCnt++ )
		{
			if( gpNPCQuoteInfoArray[ usCnt1 ] )
			{
				TempNpcQuote[ ubCnt ].usFlags		= gpNPCQuoteInfoArray[ usCnt1 ][ ubCnt ].fFlags;
				TempNpcQuote[ ubCnt ].sRequiredItem = gpNPCQuoteInfoArray[ usCnt1 ][ ubCnt ].sRequiredItem;
				TempNpcQuote[ ubCnt ].usGoToGridNo	= gpNPCQuoteInfoArray[ usCnt1 ][ ubCnt ].usGoToGridNo;
			}
		}

		//Save the array to a temp file
		FileWrite( hFile, TempNpcQuote, uiSizeOfTempArray, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiSizeOfTempArray )
		{
			FileClose( hFile );
			return( FALSE );
		}
	}

	FileClose( hFile );
	return( TRUE );
}



BOOLEAN SaveTempNpcQuoteInfoForNPCToTempFile( UINT8 ubNpcId )
{
	UINT32	uiNumBytesWritten;
	UINT8	ubCnt;
	TempNPCQuoteInfoSave TempNpcQuote[ NUM_NPC_QUOTE_RECORDS ];
	UINT32	uiSizeOfTempArray = sizeof( TempNPCQuoteInfoSave ) * NUM_NPC_QUOTE_RECORDS;
	UINT32	uiSpotInFile = ubNpcId - FIRST_RPC;
	HWFILE	hFile=0;

	//if there are records to save
	if( gpNPCQuoteInfoArray[ ubNpcId ] )
	{

		hFile = FileOpen( NPC_TEMP_QUOTE_FILE, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
		if( hFile == 0 )
		{
			//Error opening temp npc quote info
			return( FALSE );
		}

		memset( TempNpcQuote, 0, uiSizeOfTempArray );

		//Loop through and build the temp array to save
		for( ubCnt=0; ubCnt<NUM_NPC_QUOTE_RECORDS; ubCnt++ )
		{
			TempNpcQuote[ ubCnt ].usFlags		= gpNPCQuoteInfoArray[ ubNpcId ][ ubCnt ].fFlags;
			TempNpcQuote[ ubCnt ].sRequiredItem = gpNPCQuoteInfoArray[ ubNpcId ][ ubCnt ].sRequiredItem;
			TempNpcQuote[ ubCnt ].usGoToGridNo	= gpNPCQuoteInfoArray[ ubNpcId ][ ubCnt ].usGoToGridNo;
		}

		//Seek to the correct spot in the file
		FileSeek( hFile, uiSpotInFile * uiSizeOfTempArray, FILE_SEEK_FROM_START );

		//Save the array to a temp file
		FileWrite( hFile, TempNpcQuote, uiSizeOfTempArray, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiSizeOfTempArray )
		{
			FileClose( hFile );
			return( FALSE );
		}

		//Set the fact that the merc has the temp npc quote data
		gMercProfiles[ ubNpcId ].ubMiscFlags |= PROFILE_MISC_FLAG_TEMP_NPC_QUOTE_DATA_EXISTS;

		FileClose( hFile );
	}


	return( TRUE );
}




BOOLEAN LoadTempNpcQuoteInfoForNPCFromTempFile( UINT8 ubNpcId )
{
	UINT32	uiNumBytesRead;
	UINT8		ubCnt;
	TempNPCQuoteInfoSave TempNpcQuote[ NUM_NPC_QUOTE_RECORDS ];
	UINT32	uiSizeOfTempArray = sizeof( TempNPCQuoteInfoSave ) * NUM_NPC_QUOTE_RECORDS;
	UINT32	uiSpotInFile = ubNpcId - FIRST_RPC;
	HWFILE	hFile;


	//Init the array
	memset( TempNpcQuote, 0, uiSizeOfTempArray );


	//If there isnt already memory allocated, allocate memory to hold the array
	if( gpNPCQuoteInfoArray[ ubNpcId ] == NULL )
	{
		gpNPCQuoteInfoArray[ ubNpcId ] = (NPCQuoteInfo *) MemAlloc( sizeof( NPCQuoteInfo ) * NUM_NPC_QUOTE_RECORDS );
		if( gpNPCQuoteInfoArray[ ubNpcId ] == NULL )
			return( FALSE );
	}


	hFile = FileOpen( NPC_TEMP_QUOTE_FILE, FILE_ACCESS_READ | FILE_OPEN_ALWAYS, FALSE );
	if( hFile == 0 )
	{
		//Error opening temp npc quote info
		return( FALSE );
	}

	//Seek to the correct spot in the file
	FileSeek( hFile, uiSpotInFile * uiSizeOfTempArray, FILE_SEEK_FROM_START );

	//Save the array to a temp file
	FileRead( hFile, TempNpcQuote, uiSizeOfTempArray, &uiNumBytesRead );
	if( uiNumBytesRead != uiSizeOfTempArray )
	{
		FileClose( hFile );
		return( FALSE );
	}



	//Loop through and build the temp array to save
	for( ubCnt=0; ubCnt<NUM_NPC_QUOTE_RECORDS; ubCnt++ )
	{
		gpNPCQuoteInfoArray[ ubNpcId ][ ubCnt ].fFlags				= TempNpcQuote[ ubCnt ].usFlags;
		gpNPCQuoteInfoArray[ ubNpcId ][ ubCnt ].sRequiredItem		= TempNpcQuote[ ubCnt ].sRequiredItem;
		gpNPCQuoteInfoArray[ ubNpcId ][ ubCnt ].usGoToGridNo		= TempNpcQuote[ ubCnt ].usGoToGridNo;
	}

	FileClose( hFile );

	return( TRUE );
}







void ChangeNpcToDifferentSector( UINT8 ubNpcId, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	if (gMercProfiles[ ubNpcId ].ubMiscFlags2 & PROFILE_MISC_FLAG2_LEFT_COUNTRY)
	{
		// override location, this person is OUTTA here
		sSectorX = 0;
		sSectorY = 0;
		bSectorZ = 0;
	}
	// Carmen no longer traverses out, he is temporarily removed instead

	gMercProfiles[ ubNpcId ].sSectorX = sSectorX;
	gMercProfiles[ ubNpcId ].sSectorY = sSectorY;
	gMercProfiles[ ubNpcId ].bSectorZ = bSectorZ;

	gMercProfiles[ ubNpcId ].fUseProfileInsertionInfo = FALSE;

	/*
	if (!gMercProfiles[ ubNpcId ].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR)
	{
		gMercProfiles[ ubNpcId ].fUseProfileInsertionInfo = PROFILE_DONT_USE_GRIDNO;
	}
	*/
}





BOOLEAN AddRottingCorpseToUnloadedSectorsRottingCorpseFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, ROTTING_CORPSE_DEFINITION *pRottingCorpseDef )
{
	HWFILE	hFile;
	UINT32	uiNumberOfCorpses;
	CHAR8	zMapName[ 128 ];
	UINT32	uiNumBytesRead;

	ROTTING_CORPSE_DEFINITION	def;
	std::vector<ROTTING_CORPSE_DEFINITION> corpsedefvector;

	// Flugente 2018-08-15: the old method of reading and writing in the same file at the same time is faulty.
	// We instead open the existing file if it exists, take it's contents, and write them with the new corpse to a new file

	GetMapTempFileName( SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, zMapName, sMapX, sMapY, bMapZ );

	if ( DoesTempFileExistsForMap( SF_ROTTING_CORPSE_TEMP_FILE_EXISTS, sMapX, sMapY, bMapZ ) && FileExists( zMapName ) )
	{
		//Open the file for reading
		hFile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
		if ( hFile == 0 )
		{
			//Error opening map modification file,
			return( FALSE );
		}

		// Load the number of Rotting corpses
		FileRead( hFile, &uiNumberOfCorpses, sizeof( UINT32 ), &uiNumBytesRead );
		if ( uiNumBytesRead != sizeof( UINT32 ) )
		{
			//Error Writing size of array to disk
			FileClose( hFile );
			return( FALSE );
		}

		// load existing corpses
		for ( UINT32 cnt = 0; cnt < uiNumberOfCorpses; ++cnt )
		{
			// Load the Rotting corpses info
			FileRead( hFile, &def, sizeof( ROTTING_CORPSE_DEFINITION ), &uiNumBytesRead );
			if ( uiNumBytesRead != sizeof( ROTTING_CORPSE_DEFINITION ) )
			{
				//Error Writing size of array to disk
				continue;
			}

			corpsedefvector.push_back( def );
		}

		FileClose( hFile );
	}

	corpsedefvector.push_back( *pRottingCorpseDef );

	// create the file anew
	SaveRottingCorpsesToTempCorpseFile( sMapX, sMapY, bMapZ, corpsedefvector );

	// Flugente: update number of corpses in this sector
	if ( !bMapZ )
	{
		SECTORINFO *pSectorInfo = &( SectorInfo[SECTOR( sMapX, sMapY )] );

		if ( pSectorInfo )
			pSectorInfo->usNumCorpses = corpsedefvector.size();
	}

	return( TRUE );
}



BOOLEAN SetUnderGroundSectorFlag( INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ, UINT32 uiFlagToSet )
{
	UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

	pTempNode = gpUndergroundSectorInfoHead;

	//loop through and look for the right underground sector
	while( pTempNode )
	{
		if( ( pTempNode->ubSectorX == (UINT8)sSectorX ) &&
				( pTempNode->ubSectorY == (UINT8)sSectorY ) &&
				( pTempNode->ubSectorZ == ubSectorZ ) )
		{
			//set the flag indicating that ther is a temp item file exists for the sector
			pTempNode->uiFlags |= uiFlagToSet;

			return( TRUE );
		}
		pTempNode = pTempNode->next;
	}

	return( FALSE );
}

BOOLEAN ReSetUnderGroundSectorFlag( INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ, UINT32 uiFlagToSet )
{
	UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

	pTempNode = gpUndergroundSectorInfoHead;

	//loop through and look for the right underground sector
	while( pTempNode )
	{
		if( ( pTempNode->ubSectorX == (UINT8)sSectorX ) &&
				( pTempNode->ubSectorY == (UINT8)sSectorY ) &&
				( pTempNode->ubSectorZ == ubSectorZ ) )
		{
			//set the flag indicating that ther is a temp item file exists for the sector
			pTempNode->uiFlags &= ~( uiFlagToSet );

			return( TRUE );
		}
		pTempNode = pTempNode->next;
	}

	return( FALSE );
}

BOOLEAN GetUnderGroundSectorFlagStatus( INT16 sSectorX, INT16 sSectorY, UINT8 ubSectorZ, UINT32 uiFlagToCheck )
{
	UNDERGROUND_SECTORINFO *pTempNode = gpUndergroundSectorInfoHead;

	pTempNode = gpUndergroundSectorInfoHead;

	//loop through and look for the right underground sector
	while( pTempNode )
	{
		if( ( pTempNode->ubSectorX == (UINT8)sSectorX ) &&
				( pTempNode->ubSectorY == (UINT8)sSectorY ) &&
				( pTempNode->ubSectorZ == ubSectorZ ) )
		{
			//set the flag indicating that ther is a temp item file exists for the sector
			if( pTempNode->uiFlags & uiFlagToCheck )
				return( TRUE );
			else
				return( FALSE );
		}
		pTempNode = pTempNode->next;
	}
	return( FALSE );
}


BOOLEAN SetSectorFlag( INT16 sMapX, INT16 sMapY, UINT8 bMapZ, UINT32 uiFlagToSet )
{
	if( uiFlagToSet == SF_ALREADY_VISITED )
	{

		// HEADROCK HAM 3.5: This is no longer required at all.
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		// HEADROCK HAM 3.4: Externalized. Hidden facilities listed on the facility array will be
		// added to the sector info now, upon the first visit.

		/*for(UINT32 cnt=0; (gFacilityLocations[cnt].uiSectorID >= 0 && gFacilityLocations[cnt].uiSectorID <= 256); cnt++ )
		{
			// Does the current record match the current sector?
			if (gFacilityLocations[cnt].uiSectorID == SECTOR( sMapX, sMapY ))
			{
				// Is the current record set to be revealed?
				if (gFacilityLocations[cnt].fHidden == 1)
				{
					// Reveal the facility.
					SectorInfo[ SECTOR( sMapX, sMapY) ].uiFacilitiesFlags |= (1 << (gFacilityLocations[cnt].uiFacilityType - 1));
				}
			}
		}*/
		

		// HEADROCK HAM 3.4: Externalized.
		/*
		// do certain things when particular sectors are visited
		if ( ( sMapX == TIXA_SECTOR_X ) && ( sMapY == TIXA_SECTOR_Y ) )
		{
			// Tixa prison (not seen until Tixa visited)
			SectorInfo[ SEC_J9 ].uiFacilitiesFlags |= SFCF_PRISON;
		}

		if ( ( sMapX == GUN_RANGE_X ) && ( sMapY == GUN_RANGE_Y ) && ( bMapZ == GUN_RANGE_Z ) )
		{
			// Alma shooting range (not seen until sector visited)
			SectorInfo[ SEC_H13 ].uiFacilitiesFlags |= SFCF_GUN_RANGE;
			SectorInfo[ SEC_H14 ].uiFacilitiesFlags |= SFCF_GUN_RANGE;
			SectorInfo[ SEC_I13 ].uiFacilitiesFlags |= SFCF_GUN_RANGE;
			SectorInfo[ SEC_I14 ].uiFacilitiesFlags |= SFCF_GUN_RANGE;
		}*/

		if ( !GetSectorFlagStatus( sMapX, sMapY, bMapZ, SF_ALREADY_VISITED ) )
		{
			// increment daily counter of sectors visited
			gStrategicStatus.ubNumNewSectorsVisitedToday++;
			if ( gStrategicStatus.ubNumNewSectorsVisitedToday == NEW_SECTORS_EQUAL_TO_ACTIVITY )
			{
				// visited enough to count as an active day
				UpdateLastDayOfPlayerActivity( (UINT16) GetWorldDay() );
			}

			for ( UINT8 i = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; i <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; i++ )
			{
				if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife && !(MercPtrs[ i ]->flags.uiStatusFlags & SOLDIER_VEHICLE) && MercPtrs[ i ]->ubProfile != NO_PROFILE &&
					MercPtrs[ i ]->sSectorX == sMapX && MercPtrs[ i ]->sSectorY == sMapY && MercPtrs[ i ]->bSectorZ == bMapZ && !MercPtrs[ i ]->flags.fBetweenSectors &&
					MercPtrs[ i ]->bAssignment != IN_TRANSIT && MercPtrs[ i ]->bAssignment != ASSIGNMENT_DEAD )
				{
					gMercProfiles[ MercPtrs[ i ]->ubProfile ].records.usSectorsDiscovered++;
				}
			}
		}
	}

	if( bMapZ == 0 )
		SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags |= uiFlagToSet;
	else
		SetUnderGroundSectorFlag( sMapX, sMapY, bMapZ, uiFlagToSet );

	return( TRUE );
}

BOOLEAN ReSetSectorFlag( INT16 sMapX, INT16 sMapY, UINT8 bMapZ, UINT32 uiFlagToSet )
{
	if( bMapZ == 0 )
		SectorInfo[ SECTOR( sMapX,sMapY) ].uiFlags &= ~( uiFlagToSet );
	else
		ReSetUnderGroundSectorFlag( sMapX, sMapY, bMapZ, uiFlagToSet );

	return( TRUE );
}

BOOLEAN GetSectorFlagStatus( INT16 sMapX, INT16 sMapY, UINT8 bMapZ, UINT32 uiFlagToSet )
{
	if( bMapZ == 0 )
		return( (SectorInfo[ SECTOR( sMapX, sMapY ) ].uiFlags & uiFlagToSet ) ? 1 : 0 );
	else
		return( (GetUnderGroundSectorFlagStatus( sMapX, sMapY, bMapZ, uiFlagToSet ) ) ? 1 : 0  );
}

BOOLEAN AddDeadSoldierToUnLoadedSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ, SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiFlags )
{
	UINT32			uiNumberOfItems;
	std::vector<WORLDITEM> pWorldItems;//dnl ch75 271013
	UINT8				i;
	UINT8				bCount=0;
	UINT16			uiFlagsForWorldItems=0;
	UINT16			usFlagsForRottingCorpse=0;
	ROTTING_CORPSE_DEFINITION		Corpse;
	INT16				sXPos, sYPos;
	UINT32			uiDeathAnim;
	UINT32			uiPossibleDeathAnims[] = {	GENERIC_HIT_DEATH,
																					FALLBACK_HIT_DEATH,
																					PRONE_HIT_DEATH,
																					FLYBACK_HIT_DEATH };
	//setup the flags for the items and the rotting corpses
	if( uiFlags & ADD_DEAD_SOLDIER_USE_GRIDNO )
	{
		uiFlagsForWorldItems		= 0;
		usFlagsForRottingCorpse	= 0;
	}

	else if( uiFlags & ADD_DEAD_SOLDIER_TO_SWEETSPOT )
	{
		uiFlagsForWorldItems		|= WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE;
		usFlagsForRottingCorpse	|= ROTTING_CORPSE_FIND_SWEETSPOT_FROM_GRIDNO;
	}
	else
		AssertMsg( 0, "ERROR!!	Flag not is Switch statement");
	
	//
	//Create an array of objects from the mercs inventory
	//

	//go through and and find out how many items there are
	uiNumberOfItems = 0;
	UINT8 invsize = pSoldier->inv.size();
	for ( i = 0; i < invsize; ++i )
	{
		if( pSoldier->inv[ i ].exists() == true )
		{
			// Flugente: if we don't want items to drop in autoresolve, this is the place to set that
			if ( pSoldier->bTeam != gbPlayerNum && !gGameExternalOptions.fNPCAutoresolveItemDrop )
			{
				// mark it undroppable...
				pSoldier->inv[i].fFlags |= OBJECT_UNDROPPABLE;
			}

			//if the item can be dropped
			if( !( pSoldier->inv[ i ].fFlags & OBJECT_UNDROPPABLE ) || pSoldier->bTeam == gbPlayerNum )
			{
				++uiNumberOfItems;
			}
		}
	}

  // If a robot, don't drop anything...
  if ( AM_A_ROBOT( pSoldier ) )
  {
    uiNumberOfItems = 0;
  }

	//if there are items to add
	if( uiNumberOfItems )
	{
		//allocate memory for the world item array
		pWorldItems.resize(uiNumberOfItems);//dnl ch75 271013

		//loop through all the soldiers items and add them to the world item array
		bCount = 0;
		for ( i = 0; i < invsize; ++i )
		{
			if( pSoldier->inv[ i ].exists() == true )
			{
				//if the item can be dropped
				if( !(pSoldier->inv[ i ].fFlags & OBJECT_UNDROPPABLE) || pSoldier->bTeam == gbPlayerNum )
				{
					ReduceAmmoDroppedByNonPlayerSoldiers( pSoldier, i );

					pWorldItems[ bCount ].fExists = TRUE;
					pWorldItems[ bCount ].sGridNo = sGridNo;
					pWorldItems[ bCount ].ubLevel = (UINT8)pSoldier->pathing.bLevel;
					pWorldItems[ bCount ].usFlags = uiFlagsForWorldItems;
					pWorldItems[ bCount ].bVisible = TRUE;
					pWorldItems[ bCount ].bRenderZHeightAboveLevel = 0;

					if ( Item[pSoldier->inv[i].usItem].damageable ) // Madd: drop crappier items on higher difficulty levels
					{
						// silversurfer: externalized this
						//pSoldier->inv[i][0]->data.objectStatus -= (gGameOptions.ubDifficultyLevel - 1) * Random(20);
						pSoldier->inv[i][0]->data.objectStatus -= Random( zDiffSetting[gGameOptions.ubDifficultyLevel].usLootStatusModifier );
						pSoldier->inv[i][0]->data.objectStatus = max(pSoldier->inv[i][0]->data.objectStatus,1); // never below 1%
					}
					pWorldItems[ bCount ].object = pSoldier->inv[i];
					++bCount;
				}
			}
		}

		//Add the soldiers items to an unloaded sector
		AddWorldItemsToUnLoadedSector( sMapX, sMapY, bMapZ, sGridNo, uiNumberOfItems, pWorldItems, FALSE );

		//CHRISL: Now that we've copied the dead soldiers items to pWorldItems, we need to actually remove the items from the dead soldier.  Otherwise we have
		//	actually duplicated the items and there's a chance that these duplicated items will again find their way to sector.
		for ( i = 0; i < invsize; ++i )
		{
			if( pSoldier->inv[ i ].exists() == true )
			{
				pSoldier->inv[i].initialize();
			}
		}
	}

  DropKeysInKeyRing( pSoldier, sGridNo, pSoldier->pathing.bLevel, 1, FALSE, 0, TRUE );

	//
	//Convert the soldier into a rottng corpse
	//

	memset( &Corpse, 0, sizeof( ROTTING_CORPSE_DEFINITION ) );

	// Setup some values!
	Corpse.ubBodyType							= pSoldier->ubBodyType;
	Corpse.sGridNo								= sGridNo;

	ConvertGridNoToXY( sGridNo, &sXPos, &sYPos );

	Corpse.dXPos									= (FLOAT)( CenterX( sXPos ) );
	Corpse.dYPos									= (FLOAT)( CenterY( sYPos ) );
	Corpse.sHeightAdjustment			= pSoldier->sHeightAdjustment;
	Corpse.bVisible								=	TRUE;

	SET_PALETTEREP_ID ( Corpse.HeadPal,		pSoldier->HeadPal );
	SET_PALETTEREP_ID ( Corpse.VestPal,		pSoldier->VestPal );
	SET_PALETTEREP_ID ( Corpse.SkinPal,		pSoldier->SkinPal );
	SET_PALETTEREP_ID ( Corpse.PantsPal,   pSoldier->PantsPal );

	Corpse.ubDirection	= pSoldier->ubDirection;

	// Set time of death
	Corpse.uiTimeOfDeath = GetWorldTotalMin( );

	//if the dead body shot be the result of a Jfk headshot, set it
	if( uiFlags & ADD_DEAD_SOLDIER__USE_JFK_HEADSHOT_CORPSE )
		uiDeathAnim = JFK_HITDEATH;

	//else chose a random death sequence
	else
		uiDeathAnim = uiPossibleDeathAnims[ Random( 4 ) ];


	// Set type
	Corpse.ubType	= (UINT8)gubAnimSurfaceCorpseID[ pSoldier->ubBodyType][ uiDeathAnim ];

	Corpse.usFlags |= usFlagsForRottingCorpse;

	//Add the rotting corpse info to the sectors unloaded rotting corpse file
	AddRottingCorpseToUnloadedSectorsRottingCorpseFile( sMapX, sMapY, bMapZ, &Corpse);

	return( TRUE );
}

BOOLEAN SaveTempNpcQuoteArrayToSaveGameFile( HWFILE hFile )
{
	return( SaveFilesToSavedGame( NPC_TEMP_QUOTE_FILE, hFile ) );
}

BOOLEAN LoadTempNpcQuoteArrayToSaveGameFile( HWFILE hFile )
{
	return( LoadFilesFromSavedGame( NPC_TEMP_QUOTE_FILE, hFile ) );
}

void TempFileLoadErrorMessageReturnCallback( UINT8 ubRetVal )
{
	gfProgramIsRunning = FALSE;
}

//if you call this function, make sure you return TRUE (if applicable) to make the game
//think it succeeded the load.  This sets up the dialog for the game exit, after the hacker
//message appears.
void InitExitGameDialogBecauseFileHackDetected()
{
	SGPRect CenteringRect= {0, 0, 639, 479 };

	// do message box and return
	giErrorMessageBox = DoMessageBox( MSG_BOX_BASIC_STYLE, pAntiHackerString[ ANTIHACKERSTR_EXITGAME ],
											GAME_SCREEN, MSG_BOX_FLAG_OK, TempFileLoadErrorMessageReturnCallback, &CenteringRect );
}

// CHRISL: New Checksum for LBENODE
UINT32 LBENODEChecksum( LBENODE * pNode )
{
	UINT32	uiChecksum = 1;
	UINT32	uiLoop;

	uiChecksum += (pNode->lbeClass + 1);
	uiChecksum *= (pNode->lbeIndex +1);
	uiChecksum += (pNode->ubID +1);

	UINT32 invsize = pNode->inv.size();
	for ( uiLoop = 0; uiLoop < invsize; ++uiLoop )
	{
		uiChecksum += pNode->inv[ uiLoop ].usItem;
	}

	return( uiChecksum );
}


//UINT8 gubEncryptionArray4[ BASE_NUMBER_OF_ROTATION_ARRAYS * 3 ][ NEW_ROTATION_ARRAY_SIZE ] =
UINT8 * GetRotationArray( void )
{
	// based on guiJA2EncryptionSet
	if ( guiJA2EncryptionSet < BASE_NUMBER_OF_ROTATION_ARRAYS * 6 )
	{
		if ( guiJA2EncryptionSet < BASE_NUMBER_OF_ROTATION_ARRAYS * 3 )
		{
			return( gubEncryptionArray1[ guiJA2EncryptionSet % (BASE_NUMBER_OF_ROTATION_ARRAYS * 3) ]);
		}
		else
		{
			return( gubEncryptionArray2[ guiJA2EncryptionSet % (BASE_NUMBER_OF_ROTATION_ARRAYS * 3) ]);
		}
	}
	else
	{
		if ( guiJA2EncryptionSet < BASE_NUMBER_OF_ROTATION_ARRAYS * 9 )
		{
			return( gubEncryptionArray3[ guiJA2EncryptionSet % (BASE_NUMBER_OF_ROTATION_ARRAYS * 3) ]);
		}
		else
		{
			return( gubEncryptionArray4[ guiJA2EncryptionSet % (BASE_NUMBER_OF_ROTATION_ARRAYS * 3) ]);
		}
	}
}

BOOLEAN NewJA2EncryptedFileRead( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead )
{
	UINT32	uiLoop;
	UINT8		ubArrayIndex = 0;
	UINT8		ubLastByte = 0;
	UINT8		ubLastByteForNextLoop;
	BOOLEAN	fRet;
	UINT8 *	pMemBlock;
	UINT8	* pubRotationArray;

	pubRotationArray = GetRotationArray();

	fRet = FileRead( hFile, pDest, uiBytesToRead, puiBytesRead );
	if ( fRet )
	{
		pMemBlock = (UINT8 *)pDest;
		for ( uiLoop = 0; uiLoop < *puiBytesRead; uiLoop++ )
		{
			ubLastByteForNextLoop = pMemBlock[ uiLoop ];
			pMemBlock[ uiLoop ] -= (ubLastByte + pubRotationArray[ ubArrayIndex ]);
			ubArrayIndex++;
			if ( ubArrayIndex >= NEW_ROTATION_ARRAY_SIZE )
			{
				ubArrayIndex = 0;
			}
			ubLastByte = ubLastByteForNextLoop;
		}
	}

	return( fRet );
}


BOOLEAN NewJA2EncryptedFileWrite( HWFILE hFile, PTR pDest, UINT32 uiBytesToWrite, UINT32 *puiBytesWritten )
{
	UINT32	uiLoop;
	UINT8		ubArrayIndex = 0;
	UINT8		ubLastByte = 0;//, ubTemp;
	UINT8 * pMemBlock;
	BOOLEAN	fRet;
	UINT8	* pubRotationArray;

	pMemBlock = (UINT8 *) MemAlloc( uiBytesToWrite );

	if ( !pMemBlock )
	{
		return( FALSE );
	}
	memset( pMemBlock, 0, uiBytesToWrite );

	pubRotationArray = GetRotationArray();

	memcpy( pMemBlock, pDest, uiBytesToWrite );
	for ( uiLoop = 0; uiLoop < uiBytesToWrite; uiLoop++ )
	{
		pMemBlock[ uiLoop ] += ubLastByte + pubRotationArray[ ubArrayIndex ];
		ubArrayIndex++;
		if ( ubArrayIndex >= NEW_ROTATION_ARRAY_SIZE )
		{
			ubArrayIndex = 0;
		}
		ubLastByte = pMemBlock[ uiLoop ];
	}

	fRet = FileWrite( hFile, pMemBlock, uiBytesToWrite, puiBytesWritten );

	MemFree( pMemBlock );

	return( fRet );

}


#define ROTATION_ARRAY_SIZE 46
UINT8 ubRotationArray[46] = { 132, 235, 125, 99, 15, 220, 140, 89, 205, 132, 254, 144, 217, 78, 156, 58, 215, 76, 163, 187, 55, 49, 65, 48, 156, 140, 201, 68, 184, 13, 45, 69, 102, 185, 122, 225, 23, 250, 160, 220, 114, 240, 64, 175, 057, 233 };

BOOLEAN JA2EncryptedFileRead( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead )
{
	UINT32	uiLoop;
	UINT8		ubArrayIndex = 0;
	//UINT8		ubLastNonBlank = 0;
	UINT8		ubLastByte = 0;
	UINT8		ubLastByteForNextLoop;
	BOOLEAN	fRet;
	UINT8 *	pMemBlock;

	fRet = FileRead( hFile, pDest, uiBytesToRead, puiBytesRead );
	if ( fRet )
	{
		pMemBlock = (UINT8 *)pDest;
		for ( uiLoop = 0; uiLoop < *puiBytesRead; uiLoop++ )
		{
			ubLastByteForNextLoop = pMemBlock[ uiLoop ];
			pMemBlock[ uiLoop ] -= (ubLastByte + ubRotationArray[ ubArrayIndex ]);
			ubArrayIndex++;
			if ( ubArrayIndex >= ROTATION_ARRAY_SIZE )
			{
				ubArrayIndex = 0;
			}
			ubLastByte = ubLastByteForNextLoop;
		}
	}

	return( fRet );
}


BOOLEAN JA2EncryptedFileWrite( HWFILE hFile, PTR pDest, UINT32 uiBytesToWrite, UINT32 *puiBytesWritten )
{
	UINT32	uiLoop;
	UINT8		ubArrayIndex = 0;
	//UINT8		ubLastNonBlank = 0;
	UINT8		ubLastByte = 0;//, ubTemp;
	UINT8 * pMemBlock;
	BOOLEAN	fRet;

	pMemBlock = (UINT8 *) MemAlloc( uiBytesToWrite );

	if ( !pMemBlock )
	{
		return( FALSE );
	}
	memset( pMemBlock, 0, uiBytesToWrite );


	memcpy( pMemBlock, pDest, uiBytesToWrite );
	for ( uiLoop = 0; uiLoop < uiBytesToWrite; uiLoop++ )
	{
		//ubTemp = pMemBlock[ uiLoop ];
		pMemBlock[ uiLoop ] += ubLastByte + ubRotationArray[ ubArrayIndex ];
		ubArrayIndex++;
		if ( ubArrayIndex >= ROTATION_ARRAY_SIZE )
		{
			ubArrayIndex = 0;
		}
		ubLastByte = pMemBlock[ uiLoop ];
		/*
		if ( pMemBlock[ uiLoop ] )
		{
			// store last non blank
			ubLastNonBlank = pMemBlock[ uiLoop ];

			pMemBlock[ uiLoop ] += ubRotationArray[ ubArrayIndex ];
			ubArrayIndex++;
			if ( ubArrayIndex >= ROTATION_ARRAY_SIZE )
			{
				ubArrayIndex = 0;
			}
		}
		else // zero byte
		{
			pMemBlock[ uiLoop ] = ubLastNonBlank + ubRotationArray[ ubArrayIndex ];
			ubArrayIndex++;
			if ( ubArrayIndex >= ROTATION_ARRAY_SIZE )
			{
				ubArrayIndex = 0;
			}
		}
		*/
	}

	fRet = FileWrite( hFile, pMemBlock, uiBytesToWrite, puiBytesWritten );

	MemFree( pMemBlock );

	return( fRet );

}

void GetMapTempFileName( UINT32 uiType, STR pMapName, INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	CHAR	zTempName[512];

	//Convert the current sector location into a file name
	GetMapFileName( sMapX,sMapY, bMapZ, zTempName, FALSE, FALSE );

	switch( uiType )
	{
		case SF_ITEM_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\i_%s", MAPS_DIR, zTempName);
			break;

		case SF_ROTTING_CORPSE_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\r_%s", MAPS_DIR, zTempName);
			break;

		case SF_MAP_MODIFICATIONS_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\m_%s", MAPS_DIR, zTempName);
			break;

		case SF_DOOR_TABLE_TEMP_FILES_EXISTS:
			sprintf( pMapName, "%s\\d_%s", MAPS_DIR, zTempName);
			break;

		case SF_REVEALED_STATUS_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\v_%s", MAPS_DIR, zTempName);
			break;

		case SF_DOOR_STATUS_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\ds_%s", MAPS_DIR, zTempName);
			break;

		case SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\e_%s", MAPS_DIR, zTempName);
			break;

		case SF_CIV_PRESERVED_TEMP_FILE_EXISTS:
			// NB save game version 0 is "saving game"
			if ( (gTacticalStatus.uiFlags & LOADING_SAVED_GAME) && guiCurrentSaveGameVersion != 0 && guiCurrentSaveGameVersion < 78 )
			{
				sprintf( pMapName, "%s\\c_%s", MAPS_DIR, zTempName);
			}
			else
			{
				sprintf( pMapName, "%s\\cc_%s", MAPS_DIR, zTempName);
			}
			break;

		case SF_SMOKE_EFFECTS_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\sm_%s", MAPS_DIR, zTempName);
			break;

		case SF_LIGHTING_EFFECTS_TEMP_FILE_EXISTS:
			sprintf( pMapName, "%s\\l_%s", MAPS_DIR, zTempName);
			break;

		default:
			Assert( 0 );
			break;
	}
}

UINT32	GetNumberOfVisibleWorldItemsFromSectorStructureForSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	UINT32 uiNumberOfItems=0;
	UNDERGROUND_SECTORINFO *pSector=NULL;

	//if the sector is above ground
	if( bMapZ == 0 )
	{
		uiNumberOfItems = SectorInfo[ SECTOR( sMapX, sMapY ) ].uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer;
	}
	else
	{
		//find the underground sector
		pSector = FindUnderGroundSector( sMapX, sMapY, bMapZ );
		if( pSector != NULL )
		{
			//get the number of items
			uiNumberOfItems = pSector->uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer;
		}
	}

	//if there is a sector loaded
	if( gfWorldLoaded )
	{
		//and it is the sector we are interested in
		if( sMapX == gWorldSectorX && sMapY == gWorldSectorY && bMapZ == gbWorldSectorZ )
		{
			//since items might have been added, update
			uiNumberOfItems = UpdateLoadedSectorsItemInventory( sMapX, sMapY, bMapZ, uiNumberOfItems );
		}
	}


	return( uiNumberOfItems );
}

void	SetNumberOfVisibleWorldItemsInSectorStructureForSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 uiNumberOfItems )
{
	//if the sector is above ground
	if( bMapZ == 0 )
	{
		SectorInfo[SECTOR( sMapX, sMapY )].uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer = uiNumberOfItems;
	}
	else
	{
		//find the underground sector
		UNDERGROUND_SECTORINFO* pSector = FindUnderGroundSector( sMapX, sMapY, bMapZ );
		if( pSector != NULL )
		{
			//get the number of items
			pSector->uiNumberOfWorldItemsInTempFileThatCanBeSeenByPlayer = uiNumberOfItems ;
		}
	}
}

//Moa 09/19/13: changed loop upperbound from uiTotalNumberOfRealItems to uiTotalNumberOfItems, 
// this effectivly removes one entire loop in GetNumberOfActiveWorldItemsFromTempFile and 2 file reads.
// also changed type of iCounter from INT32 to UINT32 and for in if-clauses from true to TRUE to get rid of unneccessary casts.
void SynchronizeItemTempFileVisbleItemsToSectorInfoVisbleItems( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fLoadingGame )
{
	UINT32 uiTotalNumberOfItems = 0;//, uiTotalNumberOfRealItems = 0;
	std::vector<WORLDITEM> pTotalSectorList;//dnl ch75 271013
	UINT32 uiItemCount = 0;
	UINT32 iCounter = 0;
	BOOLEAN	fReturn;

	/*
	// get total number, visable and invisible
	fReturn = GetNumberOfActiveWorldItemsFromTempFile( sMapX, sMapY, bMapZ, &( uiTotalNumberOfRealItems ) );
	if (fReturn == TRUE)
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("GetNumberOfActiveWorldItemsFromTempFile failed" ) );
		Assert( fReturn );
	}*/

	fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, bMapZ, &( uiTotalNumberOfItems ), FALSE );
	if (fReturn == TRUE)
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("GetNumberOfWorldItemsFromTempItemFile failed" ) );
		Assert( fReturn );
	}

	if( uiTotalNumberOfItems > 0 )
	{
		// allocate space for the list
		pTotalSectorList.resize(uiTotalNumberOfItems);//dnl ch75 271013

		// now load into mem
		LoadWorldItemsFromTempItemFile(  sMapX,  sMapY, bMapZ, pTotalSectorList );

		// now run through list and
		//for( iCounter = 0; ( UINT32 )( iCounter )< uiTotalNumberOfRealItems; iCounter++ )
		for ( iCounter = 0; iCounter < uiTotalNumberOfItems; ++iCounter )
		{
			// if visible to player, then state fact
			if ( IsMapScreenWorldItemVisibleInMapInventory( &pTotalSectorList[iCounter] ) )
			{
				uiItemCount += pTotalSectorList[iCounter].object.ubNumberOfObjects;
			}
		}
		
		// Flugente: if we load a game, we would save the data again in UpdateWorldItemsTempFile. 
		// But that function simply loads the data again and writes it. If we do the saving here, we can at least save the entire 'load it again' part
		if ( fLoadingGame && guiCurrentSaveGameVersion != SAVE_GAME_VERSION )
		{
			int backup = guiCurrentSaveGameVersion;
			guiCurrentSaveGameVersion = SAVE_GAME_VERSION;
			SaveWorldItemsToTempItemFile( sMapX, sMapY, bMapZ, uiTotalNumberOfItems, pTotalSectorList );
			guiCurrentSaveGameVersion = backup;
		}
	}
		
#ifdef JA2BETAVERSION
	if( fLoadingGame && guiCurrentSaveGameVersion >= 86 )
	{
		UINT32 uiReported = GetNumberOfVisibleWorldItemsFromSectorStructureForSector( sMapX, sMapY, bMapZ );

		if( uiItemCount != uiReported )
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"SynchronizeItemTempFile()  Error!  Reported %d, should be %d", uiReported, uiItemCount  );
	}
#endif

	//record the number of items
	SetNumberOfVisibleWorldItemsInSectorStructureForSector( sMapX, sMapY, bMapZ, uiItemCount );
}

UINT32 UpdateLoadedSectorsItemInventory( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 uiNumberOfItems )
{
	UINT32	uiCounter;
	UINT32	uiItemCounter=0;

	if( !gfWorldLoaded )
		return( 0 );

	//loop through all the world items
	for( uiCounter= 0; uiCounter < guiNumWorldItems; uiCounter++ )
	{
		//if the item CAN be visible in mapscreen sector inventory
		if( IsMapScreenWorldItemVisibleInMapInventory( &gWorldItems[ uiCounter ] ) )
		{
			//increment
			uiItemCounter += gWorldItems[ uiCounter ].object.ubNumberOfObjects;
		}
	}

	//if the item count is DIFFERENT
	if( uiItemCounter != uiNumberOfItems )
	{
		//Update the value in the sector info struct
		SetNumberOfVisibleWorldItemsInSectorStructureForSector( sMapX, sMapY, bMapZ, uiItemCounter );
	}

	//return the number of items
	return( uiItemCounter );
}
