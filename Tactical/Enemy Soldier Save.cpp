#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "Enemy Soldier Save.h"
#else
	#include "builddefines.h"
    #include "GameSettings.h"
	#include <stdio.h>
	#include <memory.h>

	#include "types.h"

	#include "strategicmap.h"
	#include "overhead.h"
	#include "isometric utils.h"

	#include "soldier add.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "debug.h"
	#include "Random.h"
	#include "items.h"


	#include "Map Information.h"
	#include "soldier profile.h"
	#include "EditorMercs.h"
	#include "Animation Data.h"
	#include "message.h"
	#include "Font Control.h"
	#include "Campaign Types.h"
	#include "Tactical Save.h"
	#include "Game Clock.h"
	#include "Queen Command.h"
	#include "Scheduling.h"
	#include "Soldier macros.h"		// added by Flugente
#endif

#include "GameVersion.h"
//ADB When a savegame is loaded, the enemy and civ stuff needs to be loaded and updated, but this can only happen
//when the temp file is loaded which can happen much later, it cannot load convert and save when updating the savegame
//therefore store a flag to load and convert later
int gEnemyPreservedTempFileVersion[256];
int gCivPreservedTempFileVersion[256];

BOOLEAN AddPlacementToWorld( SOLDIERINITNODE *pNode, GROUP *pGroup = NULL );

BOOLEAN CountNumberOfElitesRegularsAdminsAndCreaturesFromEnemySoldiersTempFile( UINT8 *pubNumElites, UINT8 *pubNumRegulars, UINT8 *pubNumAdmins, UINT8 *pubNumCreatures, UINT8 *pubNumTanks, UINT8 *pubNumJeeps );

BOOLEAN gfRestoringEnemySoldiersFromTempFile = FALSE;
BOOLEAN gfRestoringCiviliansFromTempFile = FALSE;

void RemoveCivilianTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void RemoveEnemySoldierTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	CHAR8		zMapName[ 128 ];
	if( GetSectorFlagStatus( sSectorX, sSectorY, bSectorZ, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS ) )
	{
		//Delete any temp file that is here and toast the flag that say's one exists.
		ReSetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS );

//		GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );
		//add the 'e' for 'Enemy preserved' to the front of the map name
//		sprintf( zMapName, "%s\\e_%s", MAPS_DIR, zTempName);

		GetMapTempFileName( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );

		//Delete the temp file.
		FileDelete( zMapName );
	}
}

void RemoveCivilianTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	//CHAR8		zTempName[ 128 ];
	CHAR8		zMapName[ 128 ];
	if( GetSectorFlagStatus( sSectorX, sSectorY, bSectorZ, SF_CIV_PRESERVED_TEMP_FILE_EXISTS ) )
	{
		//Delete any temp file that is here and toast the flag that say's one exists.
		ReSetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_CIV_PRESERVED_TEMP_FILE_EXISTS );
		//GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

		GetMapTempFileName( SF_CIV_PRESERVED_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );

		//Delete the temp file.
		FileDelete( zMapName );
	}
}


//OLD SAVE METHOD:	This is the old way of loading the enemies and civilians
BOOLEAN LoadEnemySoldiersFromTempFile()
{
	SOLDIERINITNODE *curr;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	INT32 i;
	INT32 slots = 0;
	UINT32 uiNumBytesRead;
	UINT32 uiTimeStamp;
	HWFILE hfile;
	INT16 sSectorX, sSectorY;
	CHAR8		zMapName[ 128 ];
	#ifdef JA2TESTVERSION
		CHAR8		zReason[256];
	#endif
	INT8 bSectorZ;
	UINT8 ubSectorID;
	UINT8 ubNumElites = 0, ubNumTroops = 0, ubNumAdmins = 0, ubNumCreatures = 0, ubNumTanks = 0, ubNumJeeps = 0;
	UINT8 ubStrategicElites, ubStrategicTroops, ubStrategicAdmins, ubStrategicCreatures, ubStrategicTanks, ubStrategicJeeps;

	gfRestoringEnemySoldiersFromTempFile = TRUE;

	//STEP ONE:	Set up the temp file to read from.

	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'e' for 'Enemy preserved' to the front of the map name
//	sprintf( zMapName, "%s\\e_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Open the file for reading
	hfile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hfile == 0 )
	{	//Error opening map modification file
		return FALSE;
	}

	//STEP TWO:	determine whether or not we should use this data.
	//because it is the demo, it is automatically used.

	FileRead( hfile, &sSectorY, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading sSectorY.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorY != sSectorY )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- sSectorY mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	LoadSoldierInitListLinks( hfile );

	//STEP THREE:	read the data

	FileRead( hfile, &sSectorX, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading sSectorX.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorX != sSectorX )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- sSectorX mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &slots,		4, &uiNumBytesRead );
	// WDS - make number of mercenaries, etc. be configurable
	if( (uiNumBytesRead != 4) || (slots > TOTAL_SOLDIERS) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading slots.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &uiTimeStamp, 4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading uiTimeStamp.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &bSectorZ, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading bSectorZ.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	if( GetWorldTotalMin() > uiTimeStamp + 300 )
	{ //the file has aged.	Use the regular method for adding soldiers.
		FileClose( hfile );
		RemoveEnemySoldierTempFile( sSectorX, sSectorY, bSectorZ );
		gfRestoringEnemySoldiersFromTempFile = FALSE;
		return TRUE;
	}

	if( gbWorldSectorZ != bSectorZ )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- bSectorZ mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	if( !slots )
	{ //no need to restore the enemy's to the map.	This means we are restoring a saved game.
		gfRestoringEnemySoldiersFromTempFile = FALSE;
		FileClose( hfile );
		return TRUE;
	}
	if( slots < 0 || slots >= (int)gGameExternalOptions.ubGameMaximumNumberOfEnemies )
	{ //bad IO!
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- illegal slot value of %d.	KM", slots );
		#endif
		goto FAIL_LOAD;
	}

	//For all the enemy slots (enemy/creature), clear the fPriorityExistance flag.	We will use these flags
	//to determine which slots have been modified as we load the data into the map pristine soldier init list.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->fPriorityExistance )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == CREATURE_TEAM || curr->pBasicPlacement->bTeam == CIV_TEAM )
			{
				curr->pBasicPlacement->fPriorityExistance = FALSE;
			}
		}
		curr = curr->next;
	}

	//get the number of enemies in this sector.
	if( bSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );
		if( !pSector )
		{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- Couldn't find underground sector info for (%d,%d,%d)	KM", sSectorX, sSectorY, bSectorZ );
		#endif
			goto FAIL_LOAD;
		}
		ubStrategicElites		= pSector->ubNumElites;
		ubStrategicTroops		= pSector->ubNumTroops;
		ubStrategicAdmins		= pSector->ubNumAdmins;
		ubStrategicTanks		= pSector->ubNumTanks;
		ubStrategicJeeps		= pSector->ubNumJeeps;
		ubStrategicCreatures = pSector->ubNumCreatures;
	}
	else
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubStrategicCreatures = pSector->ubNumCreatures;
		GetNumberOfEnemiesInSector( sSectorX, sSectorY, &ubStrategicAdmins, &ubStrategicTroops, &ubStrategicElites, &ubStrategicTanks, &ubStrategicJeeps );
	}

	for( i = 0; i < slots; i++ )
	{
		if ( !tempDetailedPlacement.Load(hfile, gEnemyPreservedTempFileVersion[SECTOR( sSectorX,sSectorY)], true) )
		{
			#ifdef JA2TESTVERSION
				sprintf( zReason, "EnemySoldier -- EOF while reading tempDetailedPlacment %d.	KM", i );
			#endif
			goto FAIL_LOAD;
		}

		curr = gSoldierInitHead;
		while( curr )
		{
			if( !curr->pBasicPlacement->fPriorityExistance )
			{
				if( !curr->pDetailedPlacement || curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == NO_PROFILE )
				{
					if( curr->pBasicPlacement->bTeam == tempDetailedPlacement.bTeam )
					{
						curr->pBasicPlacement->fPriorityExistance = TRUE;
						if( !curr->pDetailedPlacement )
						{ //need to upgrade the placement to detailed placement
							curr->pBasicPlacement->fDetailedPlacement = TRUE;
							curr->pDetailedPlacement = new SOLDIERCREATE_STRUCT(tempDetailedPlacement); //(SOLDIERCREATE_STRUCT*)MemAlloc( SIZEOF_SOLDIERCREATE_STRUCT );
						}
						else
						{
							//now replace the map pristine placement info with the temp map file version..
							*curr->pDetailedPlacement = tempDetailedPlacement;
						}

						curr->pBasicPlacement->fPriorityExistance	=	TRUE;
						curr->pBasicPlacement->ubDirection					= curr->pDetailedPlacement->ubDirection;
						curr->pBasicPlacement->bOrders						= curr->pDetailedPlacement->bOrders;
						curr->pBasicPlacement->bAttitude					= curr->pDetailedPlacement->bAttitude;
						curr->pBasicPlacement->ubBodyType					= curr->pDetailedPlacement->ubBodyType;
						curr->pBasicPlacement->fOnRoof						= curr->pDetailedPlacement->fOnRoof;
						curr->pBasicPlacement->ubSoldierClass			= curr->pDetailedPlacement->ubSoldierClass;
						curr->pBasicPlacement->ubCivilianGroup		= curr->pDetailedPlacement->ubCivilianGroup;
						curr->pBasicPlacement->fHasKeys						= curr->pDetailedPlacement->fHasKeys;
						curr->pBasicPlacement->usStartingGridNo		= curr->pDetailedPlacement->sInsertionGridNo;

						curr->pBasicPlacement->bPatrolCnt			= curr->pDetailedPlacement->bPatrolCnt;
						memcpy( curr->pBasicPlacement->sPatrolGrid, curr->pDetailedPlacement->sPatrolGrid,
							sizeof( INT32 ) * curr->pBasicPlacement->bPatrolCnt );

						if( curr->pBasicPlacement->bTeam == CIV_TEAM )
						{
							AddPlacementToWorld( curr );
							break;
						}
						else
						{
							//Add preserved placements as long as they don't exceed the actual population.
							switch( curr->pBasicPlacement->ubSoldierClass )
							{
								case SOLDIER_CLASS_ELITE:
									ubNumElites++;
									if( ubNumElites < ubStrategicElites )
									{
										AddPlacementToWorld( curr );
									}
									break;
								case SOLDIER_CLASS_ARMY:
									ubNumTroops++;
									if( ubNumTroops < ubStrategicTroops )
									{
										AddPlacementToWorld( curr );
									}
									break;
								case SOLDIER_CLASS_ADMINISTRATOR:
									ubNumAdmins++;
									if( ubNumAdmins < ubStrategicAdmins )
									{
										AddPlacementToWorld( curr );
									}
									break;
								case SOLDIER_CLASS_CREATURE:
									ubNumCreatures++;
									if( ubNumCreatures < ubStrategicCreatures )
									{
										AddPlacementToWorld( curr );
									}
									break;
								case SOLDIER_CLASS_TANK:
									if ( TANK( curr->pBasicPlacement ) )
									{
										++ubNumTanks;
										if( ubNumTanks < ubStrategicTanks )
										{
											AddPlacementToWorld( curr );
										}
									}
									break;
								case SOLDIER_CLASS_JEEP:
									if ( COMBAT_JEEP( curr->pBasicPlacement ) )
									{
										++ubNumJeeps;
										if ( ubNumJeeps < ubStrategicJeeps )
										{
											AddPlacementToWorld( curr );
										}
									}
									break;
							}
							break;
						}
					}
				}
			}
			curr = curr->next;
		}
	}

	FileRead( hfile, &ubSectorID, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading ubSectorID.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( ubSectorID != SECTOR( sSectorX, sSectorY ) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- ubSectorID mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	//now add any extra enemies that have arrived since the temp file was made.
	if ( ubStrategicTroops > ubNumTroops || ubStrategicElites > ubNumElites || ubStrategicAdmins > ubNumAdmins || ubStrategicTanks > ubNumTanks || ubStrategicJeeps > ubNumJeeps )
	{
		ubStrategicTroops = ( ubStrategicTroops > ubNumTroops ) ? ubStrategicTroops - ubNumTroops : 0;
		ubStrategicElites = ( ubStrategicElites > ubNumElites ) ? ubStrategicElites - ubNumElites : 0;
		ubStrategicAdmins = ( ubStrategicAdmins > ubNumAdmins ) ? ubStrategicAdmins - ubNumAdmins : 0;
		ubStrategicTanks = ( ubStrategicTanks > ubNumTanks ) ? ubStrategicTanks - ubNumTanks : 0;
		ubStrategicJeeps = (ubStrategicJeeps > ubNumJeeps) ? ubStrategicJeeps - ubNumJeeps : 0;
		AddSoldierInitListEnemyDefenceSoldiers( ubStrategicAdmins, ubStrategicTroops, ubStrategicElites, ubStrategicTanks, ubStrategicJeeps );
	}

	if( ubStrategicCreatures > ubNumCreatures )
	{
		ubStrategicCreatures;	//not sure if this wil ever happen.	If so, needs to be handled.
	}

	//successful
	FileClose( hfile );
	return TRUE;

	FAIL_LOAD:
		//The temp file load failed either because of IO problems related to hacking/logic, or
		//various checks failed for hacker validation.	If we reach this point, the "error: exit game"
		//dialog would appear in a non-testversion.
		FileClose( hfile );
		#ifdef JA2TESTVERSION
			AssertMsg( 0, zReason );
		#endif
		return FALSE;
}

#ifdef OBSOLETE_CODE
//OLD SAVE METHOD:	This is the older way of saving the civilian and the enemies placement into a temp file
BOOLEAN SaveEnemySoldiersToTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8	ubFirstIdTeam, UINT8 ubLastIdTeam, BOOLEAN fAppendToFile )
{
	SOLDIERINITNODE *curr;
	SOLDIERTYPE *pSoldier;
	INT32 i;
	INT32 slots = 0;
	INT32	iSlotsAlreadyInUse=0;
	UINT32 uiNumBytesWritten;
	UINT32 uiTimeStamp;
	HWFILE hfile;
	SCHEDULENODE *pSchedule;
	CHAR8		zMapName[ 128 ];
	UINT8 ubSectorID;

	//STEP ONE:	Prep the soldiers for saving...

	//modify the map's soldier init list to reflect the changes to the member's still alive...
	for( i = gTacticalStatus.Team[ ubFirstIdTeam ].bFirstID; i <= gTacticalStatus.Team[ ubLastIdTeam ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];

		if( pSoldier->bActive /*&& pSoldier->bInSector*/ && pSoldier->stats.bLife	)
		{ //soldier is valid, so find the matching soldier init list entry for modification.
			curr = gSoldierInitHead;
			while( curr && curr->pSoldier != pSoldier )
			{
				curr = curr->next;
			}
			if( curr && curr->pSoldier == pSoldier && pSoldier->ubProfile == NO_PROFILE )
			{ //found a match.

				if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
				{
					if( !curr->pDetailedPlacement )
					{ //need to upgrade the placement to detailed placement
						curr->pBasicPlacement->fDetailedPlacement = TRUE;
						curr->pDetailedPlacement = new SOLDIERCREATE_STRUCT; //(SOLDIERCREATE_STRUCT*)MemAlloc( SIZEOF_SOLDIERCREATE_STRUCT );
					}
					(*curr->pDetailedPlacement) = (*pSoldier);

					//If the soldier has a real schedule (not a default schedule), then store it.
					//All other cases should be 0.
					curr->pDetailedPlacement->ubScheduleID					= 0;
					if( pSoldier->ubScheduleID )
					{
						pSchedule = GetSchedule( pSoldier->ubScheduleID );
						if( pSchedule && !(pSchedule->usFlags & SCHEDULE_FLAGS_TEMPORARY) )
						{
							curr->pDetailedPlacement->ubScheduleID					= pSoldier->ubScheduleID;
						}
					}

					//we don't want the player to think that all the enemies start in the exact position when we
					//left the map, so randomize the start locations either current position or original position.
					if( PreRandom( 2 ) )
					{ //use current position
						curr->pDetailedPlacement->fOnRoof								= pSoldier->pathing.bLevel;
						curr->pDetailedPlacement->sInsertionGridNo			= pSoldier->sGridNo;
					}
					else
					{ //use original position
						curr->pDetailedPlacement->fOnRoof								= curr->pBasicPlacement->fOnRoof;
						curr->pDetailedPlacement->sInsertionGridNo			= curr->pBasicPlacement->sStartingGridNo;
					}
				}

				//DONE, now increment the counter, so we know how many there are.
				slots++;
			}
		}
	}
	if( !slots )
	{ //No need to save anything, so return successfully
		RemoveEnemySoldierTempFile( sSectorX, sSectorY, bSectorZ );
		return TRUE;
	}

	//STEP TWO:	Set up the temp file to write to.

	//Convert the current sector location into a file name
//	GetMapFileName( sSectorX, sSectorY, bSectorZ, zTempName, FALSE );

	//add the 'e' for 'Enemy preserved' to the front of the map name
//	sprintf( zMapName, "%s\\e_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );

	//if the file doesnt exist
	if( FileSize( zMapName ) == 0 )
	{
		//set it so we are not appending
		fAppendToFile = FALSE;
	}



	//if we are to append to the file
	if( fAppendToFile )
	{
		//Open the file for writing, Create it if it doesnt exist
		hfile = FileOpen( zMapName, FILE_ACCESS_READ	| FILE_OPEN_ALWAYS, FALSE );
		if( hfile == 0 )
		{	//Error opening map modification file
			return FALSE;
		}

		//advance for bytes and read the #of slots already used
		FileSeek( hfile, 4, FILE_SEEK_FROM_START );

		FileRead( hfile, &iSlotsAlreadyInUse, 4, &uiNumBytesWritten );
		if( uiNumBytesWritten != 4 )
		{
			goto FAIL_SAVE;
		}

		FileClose( hfile );

		//Open the file for writing, Create it if it doesnt exist
		hfile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
		if( hfile == 0 )
		{	//Error opening map modification file
			return FALSE;
		}

		slots += iSlotsAlreadyInUse;

		//advance for bytes and read the #of slots already used
		FileSeek( hfile, 4, FILE_SEEK_FROM_START );
		FileWrite( hfile, &slots, 4, &uiNumBytesWritten );
		if( uiNumBytesWritten != 4 )
		{
			goto FAIL_SAVE;
		}

		FileSeek( hfile, 0, FILE_SEEK_FROM_END );
	}
	else
	{
		//Open the file for writing, Create it if it doesnt exist
		hfile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
		if( hfile == 0 )
		{	//Error opening map modification file
			return FALSE;
		}
	}

	//if we are to append to the file
	if( !fAppendToFile )
	{
		FileWrite( hfile, &sSectorY, 2, &uiNumBytesWritten );
		if( uiNumBytesWritten != 2 )
		{
			goto FAIL_SAVE;
		}

		//STEP THREE:	Save the data
		SaveSoldierInitListLinks( hfile );

		FileWrite( hfile, &sSectorX, 2, &uiNumBytesWritten );
		if( uiNumBytesWritten != 2 )
		{
			goto FAIL_SAVE;
		}

		//This check may appear confusing.	It is intended to abort if the player is saving the game.	It is only
		//supposed to preserve the links to the placement list, so when we finally do leave the level with enemies remaining,
		//we will need the links that are only added when the map is loaded, and are normally lost when restoring a save.
		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			slots = 0;
		}

		FileWrite( hfile, &slots,		4, &uiNumBytesWritten );
		if( uiNumBytesWritten != 4 )
		{
			goto FAIL_SAVE;
		}

		uiTimeStamp = GetWorldTotalMin();
		FileWrite( hfile, &uiTimeStamp, 4, &uiNumBytesWritten );
		if( uiNumBytesWritten != 4 )
		{
			goto FAIL_SAVE;
		}

		FileWrite( hfile, &bSectorZ, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			goto FAIL_SAVE;
		}
	}

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
	{
		//if we are saving the game, we don't need to preserve the soldier information, just
		//preserve the links to the placement list.
		slots = 0;
		FileClose( hfile );
		SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS );
		return TRUE;
	}

	for( i = gTacticalStatus.Team[ ubFirstIdTeam ].bFirstID; i <= gTacticalStatus.Team[ ubLastIdTeam ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive /*&& pSoldier->bInSector*/ && pSoldier->stats.bLife )
		{ //soldier is valid, so find the matching soldier init list entry for modification.
			curr = gSoldierInitHead;
			while( curr && curr->pSoldier != pSoldier )
			{
				curr = curr->next;
			}
			if( curr && curr->pSoldier == pSoldier && pSoldier->ubProfile == NO_PROFILE )
			{ //found a match.
				if ( !curr->pDetailedPlacement->Save(hfile, FALSE) )
				{
					goto FAIL_SAVE;
				}
			}
		}
	}

	//if we are to append to the file
	if( !fAppendToFile )
	{
		ubSectorID = SECTOR( sSectorX, sSectorY );
		FileWrite( hfile, &ubSectorID, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			goto FAIL_SAVE;
		}
	}

	FileClose( hfile );
	SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS );
	return TRUE;

	FAIL_SAVE:
		FileClose( hfile );
		return FALSE;
}
#endif

BOOLEAN NewWayOfLoadingEnemySoldiersFromTempFile()
{
	SOLDIERINITNODE *curr;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	INT32 i;
	INT32 slots = 0;
	UINT32 uiNumBytesRead;
	UINT32 uiTimeStamp;
	HWFILE hfile;
	INT16 sSectorX, sSectorY;
	CHAR8		zMapName[ 128 ];
	#ifdef JA2TESTVERSION
		CHAR8		zReason[256];
	#endif
	INT8 bSectorZ;
	UINT8 ubSectorID;
	UINT8 ubNumElites = 0, ubNumTroops = 0, ubNumAdmins = 0, ubNumCreatures = 0, ubNumTanks = 0, ubNumJeeps = 0;
	UINT8 ubStrategicElites, ubStrategicTroops, ubStrategicAdmins, ubStrategicCreatures, ubStrategicTanks, ubStrategicJeeps;

	gfRestoringEnemySoldiersFromTempFile = TRUE;

	//STEP ONE:	Set up the temp file to read from.

	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'e' for 'Enemy preserved' to the front of the map name
//	sprintf( zMapName, "%s\\e_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );


	//Count the number of enemies ( elites, regulars, admins and creatures ) that are in the temp file.

	if( gbWorldSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		if( !pSector )
		{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- Couldn't find underground sector info for (%d,%d,%d)	KM", gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		#endif

			goto FAIL_LOAD;

		}
	}
	else
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

		ubNumElites = pSector->ubNumElites;
		ubNumTroops = pSector->ubNumTroops;
		ubNumAdmins = pSector->ubNumAdmins;
		ubNumTanks = pSector->ubNumTanks;
		ubNumJeeps = pSector->ubNumJeeps;
		ubNumCreatures = pSector->ubNumCreatures;
	}

	if( !( gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		// Get the number of enemies form the temp file
		CountNumberOfElitesRegularsAdminsAndCreaturesFromEnemySoldiersTempFile( &ubStrategicElites, &ubStrategicTroops, &ubStrategicAdmins, &ubStrategicCreatures, &ubStrategicTanks, &ubStrategicJeeps );

		//If any of the counts differ from what is in memory
		if ( ubStrategicElites != ubNumElites || ubStrategicTroops != ubNumTroops || ubStrategicAdmins != ubNumAdmins || ubStrategicCreatures != ubNumCreatures || ubStrategicTanks != ubNumTanks || ubStrategicJeeps != ubNumJeeps )
		{
			//remove the file
			RemoveEnemySoldierTempFile( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			return( TRUE );
		}
	}

	//reset
	ubNumElites = ubNumTroops = ubNumAdmins = ubNumCreatures = ubNumTanks = ubNumJeeps = 0;


	//Open the file for reading
	hfile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hfile == 0 )
	{	//Error opening map modification file
		return FALSE;
	}

	//STEP TWO:	determine whether or not we should use this data.
	//because it is the demo, it is automatically used.

	FileRead( hfile, &sSectorY, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading sSectorY.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorY != sSectorY )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- sSectorY mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

//	LoadSoldierInitListLinks( hfile );
	NewWayOfLoadingEnemySoldierInitListLinks( hfile );

	//STEP THREE:	read the data

	FileRead( hfile, &sSectorX, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading sSectorX.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorX != sSectorX )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- sSectorX mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &slots,		4, &uiNumBytesRead );
	// WDS - make number of mercenaries, etc. be configurable
	if( (uiNumBytesRead != 4) || (slots > TOTAL_SOLDIERS) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading slots.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &uiTimeStamp, 4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading uiTimeStamp.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &bSectorZ, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading bSectorZ.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gbWorldSectorZ != bSectorZ )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- bSectorZ mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	if( GetWorldTotalMin() > uiTimeStamp + 300 )
	{ //the file has aged.	Use the regular method for adding soldiers.
		FileClose( hfile );
		RemoveEnemySoldierTempFile( sSectorX, sSectorY, bSectorZ );
		gfRestoringEnemySoldiersFromTempFile = FALSE;
		return TRUE;
	}

	if( !slots )
	{ //no need to restore the enemy's to the map.	This means we are restoring a saved game.
		gfRestoringEnemySoldiersFromTempFile = FALSE;
		FileClose( hfile );
		return TRUE;
	}

//	// WANNE: Fix by Headrock:
	// Turns out that the program is saving the number of used Placement Slots to the temp file fine, but if that number is equal to the limit of how many enemies/civvies are allowed in a sector by the INI settings, it leads to an assertion error instead when trying to re-load that sector.
	if( slots < 0 || slots > (int)gGameExternalOptions.ubGameMaximumNumberOfEnemies )
	{ //bad IO!
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- illegal slot value of %d.	KM", slots );
		#endif
		goto FAIL_LOAD;
	}

	//For all the enemy slots (enemy/creature), clear the fPriorityExistance flag.	We will use these flags
	//to determine which slots have been modified as we load the data into the map pristine soldier init list.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->fPriorityExistance )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == CREATURE_TEAM )
			{
				curr->pBasicPlacement->fPriorityExistance = FALSE;
			}
		}
		curr = curr->next;
	}

	//get the number of enemies in this sector.
	if( bSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );
		if( !pSector )
		{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- Couldn't find underground sector info for (%d,%d,%d)	KM", sSectorX, sSectorY, bSectorZ );
		#endif
			goto FAIL_LOAD;
		}
		ubStrategicElites		= pSector->ubNumElites;
		ubStrategicTroops		= pSector->ubNumTroops;
		ubStrategicAdmins		= pSector->ubNumAdmins;
		ubStrategicTanks		= pSector->ubNumTanks;
		ubStrategicJeeps		= pSector->ubNumJeeps;
		ubStrategicCreatures = pSector->ubNumCreatures;
	}
	else
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubStrategicCreatures = pSector->ubNumCreatures;
		GetNumberOfEnemiesInSector( sSectorX, sSectorY, &ubStrategicAdmins, &ubStrategicTroops, &ubStrategicElites, &ubStrategicTanks, &ubStrategicJeeps );
	}

	for( i = 0; i < slots; i++ )
	{
		if ( !tempDetailedPlacement.Load(hfile, gEnemyPreservedTempFileVersion[SECTOR(sSectorX, sSectorY)], true) )
		{
			#ifdef JA2TESTVERSION
				sprintf( zReason, "EnemySoldier -- EOF while reading tempDetailedPlacment %d.	KM", i );
			#endif
			goto FAIL_LOAD;
		}
		curr = gSoldierInitHead;
		while( curr )
		{
			if( !curr->pBasicPlacement->fPriorityExistance )
			{
				if( curr->pBasicPlacement->bTeam == tempDetailedPlacement.bTeam )
				{
					curr->pBasicPlacement->fPriorityExistance = TRUE;
					if( !curr->pDetailedPlacement )
					{ //need to upgrade the placement to detailed placement
						curr->pBasicPlacement->fDetailedPlacement = TRUE;
						curr->pDetailedPlacement = new SOLDIERCREATE_STRUCT(tempDetailedPlacement); //(SOLDIERCREATE_STRUCT*)MemAlloc( SIZEOF_SOLDIERCREATE_STRUCT );
					}
					else
					{
						*curr->pDetailedPlacement = tempDetailedPlacement;
					}

					curr->pBasicPlacement->fPriorityExistance	=	TRUE;
					curr->pBasicPlacement->ubDirection					= curr->pDetailedPlacement->ubDirection;
					curr->pBasicPlacement->bOrders						= curr->pDetailedPlacement->bOrders;
					curr->pBasicPlacement->bAttitude					= curr->pDetailedPlacement->bAttitude;
					curr->pBasicPlacement->ubBodyType					= curr->pDetailedPlacement->ubBodyType;
					curr->pBasicPlacement->fOnRoof						= curr->pDetailedPlacement->fOnRoof;
					curr->pBasicPlacement->ubSoldierClass			= curr->pDetailedPlacement->ubSoldierClass;
					curr->pBasicPlacement->ubCivilianGroup		= curr->pDetailedPlacement->ubCivilianGroup;
					curr->pBasicPlacement->fHasKeys						= curr->pDetailedPlacement->fHasKeys;
					curr->pBasicPlacement->usStartingGridNo		= curr->pDetailedPlacement->sInsertionGridNo;

					curr->pBasicPlacement->bPatrolCnt			= curr->pDetailedPlacement->bPatrolCnt;
					memcpy( curr->pBasicPlacement->sPatrolGrid, curr->pDetailedPlacement->sPatrolGrid,
						sizeof( INT32 ) * curr->pBasicPlacement->bPatrolCnt );


					//Add preserved placements as long as they don't exceed the actual population.
					switch( curr->pBasicPlacement->ubSoldierClass )
					{
						case SOLDIER_CLASS_ELITE:
							ubNumElites++;
							if( ubNumElites <= ubStrategicElites )
							{
//def:								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_ARMY:
							ubNumTroops++;
							if( ubNumTroops <= ubStrategicTroops )
							{
//def:								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_ADMINISTRATOR:
							ubNumAdmins++;
							if( ubNumAdmins <= ubStrategicAdmins )
							{
//def:								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_CREATURE:
							ubNumCreatures++;
							if( ubNumCreatures <= ubStrategicCreatures )
							{
//def:								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_TANK:
							if ( TANK( curr->pBasicPlacement ) )
							{
								++ubNumTanks;
								if( ubNumTanks <= ubStrategicTanks )
								{
//def:								AddPlacementToWorld( curr );
								}
							}
							break;
						case SOLDIER_CLASS_JEEP:
							if ( COMBAT_JEEP( curr->pBasicPlacement ) )
							{
								++ubNumJeeps;
								if ( ubNumJeeps <= ubStrategicJeeps )
								{
									//def:								AddPlacementToWorld( curr );
								}
							}
							break;
					}
					break;
				}
			}
			curr = curr->next;
		}
	}

	FileRead( hfile, &ubSectorID, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- EOF while reading ubSectorID.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( ubSectorID != SECTOR( sSectorX, sSectorY ) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- ubSectorID mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	//now add any extra enemies that have arrived since the temp file was made.
	if ( ubStrategicTroops > ubNumTroops || ubStrategicElites > ubNumElites || ubStrategicAdmins > ubNumAdmins || ubStrategicTanks > ubNumTanks || ubStrategicJeeps > ubNumJeeps )
	{
		ubStrategicTroops = ( ubStrategicTroops > ubNumTroops ) ? ubStrategicTroops - ubNumTroops : 0;
		ubStrategicElites = ( ubStrategicElites > ubNumElites ) ? ubStrategicElites - ubNumElites : 0;
		ubStrategicAdmins = ( ubStrategicAdmins > ubNumAdmins ) ? ubStrategicAdmins - ubNumAdmins : 0;
		ubStrategicTanks = ( ubStrategicTanks > ubNumTanks ) ? ubStrategicTanks - ubNumTanks : 0;
		ubStrategicJeeps = (ubStrategicJeeps > ubNumJeeps) ? ubStrategicJeeps - ubNumJeeps : 0;
		AddSoldierInitListEnemyDefenceSoldiers( ubStrategicAdmins, ubStrategicTroops, ubStrategicElites, ubStrategicTanks, ubStrategicJeeps );
	}

	if( ubStrategicCreatures > ubNumCreatures )
	{
		ubStrategicCreatures;	//not sure if this wil ever happen.	If so, needs to be handled.
	}

	//set the number of enemies in the sector
	if( bSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		if( !pSector )
		{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- Couldn't find underground sector info for (%d,%d,%d)	KM", sSectorX, sSectorY, bSectorZ );
		#endif
			goto FAIL_LOAD;

/*
			pSector->ubElitesInBattle = ubStrategicElites;
			pSector->ubTroopsInBattle = ubStrategicTroops;
			pSector->ubAdminsInBattle = ubStrategicAdmins;
			pSector->ubCreaturesInBattle = ubStrategicCreatures;
*/
		}
	}
	else
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
/*
		pSector->ubElitesInBattle = ubStrategicElites;
		pSector->ubTroopsInBattle = ubStrategicTroops;
		pSector->ubAdminsInBattle = ubStrategicAdmins;
		pSector->ubCreaturesInBattle = ubStrategicCreatures;
*/
	}

	//if in battle, what about the ubNumInBAttle


	//successful
	FileClose( hfile );
	return TRUE;

	FAIL_LOAD:
		//The temp file load failed either because of IO problems related to hacking/logic, or
		//various checks failed for hacker validation.	If we reach this point, the "error: exit game"
		//dialog would appear in a non-testversion.
		FileClose( hfile );
		#ifdef JA2TESTVERSION
			AssertMsg( 0, zReason );
		#endif
		return FALSE;
}




BOOLEAN NewWayOfLoadingCiviliansFromTempFile()
{
	SOLDIERINITNODE *curr, *temp;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	INT32 i;
	INT32 slots = 0;
	UINT32 uiNumBytesRead;
	UINT32 uiTimeStamp;
	UINT32 uiTimeSinceLastLoaded;
	HWFILE hfile;
	INT16 sSectorX, sSectorY;
//	CHAR8		zTempName[ 128 ];
	CHAR8		zMapName[ 128 ];
	#ifdef JA2TESTVERSION
		CHAR8		zReason[256];
	#endif
	INT8 bSectorZ;
	UINT8 ubSectorID;
	BOOLEAN fDeleted;
//	UINT8 ubStrategicElites, ubStrategicTroops, ubStrategicAdmins, ubStrategicCreatures;

	gfRestoringCiviliansFromTempFile = TRUE;

	//STEP ONE:	Set up the temp file to read from.

	//Convert the current sector location into a file name
	//GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'e' for 'Enemy preserved' to the front of the map name
	//sprintf( zMapName, "%s\\c_%s", MAPS_DIR, zTempName);
	GetMapTempFileName( SF_CIV_PRESERVED_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Open the file for reading
	hfile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hfile == 0 )
	{	//Error opening map modification file
		return FALSE;
	}

	//STEP TWO:	determine whether or not we should use this data.
	//because it is the demo, it is automatically used.

	FileRead( hfile, &sSectorY, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- EOF while reading sSectorY.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorY != sSectorY )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- sSectorY mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	//LoadSoldierInitListLinks( hfile );
	NewWayOfLoadingCivilianInitListLinks( hfile );


	//STEP THREE:	read the data
	FileRead( hfile, &sSectorX, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- EOF while reading sSectorX.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorX != sSectorX )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- sSectorX mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &slots,		4, &uiNumBytesRead );
	// WDS - make number of mercenaries, etc. be configurable
	if( (uiNumBytesRead != 4) || (slots > TOTAL_SOLDIERS) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- EOF while reading slots.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &uiTimeStamp, 4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- EOF while reading uiTimeStamp.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	uiTimeSinceLastLoaded = GetWorldTotalMin() - uiTimeStamp;

	FileRead( hfile, &bSectorZ, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- EOF while reading bSectorZ.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gbWorldSectorZ != bSectorZ )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- bSectorZ mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

// WDS - DEBUG - Comment out the following ling to force reinit of a sector
	if( !slots )
	{
		//no need to restore the enemy's to the map.	This means we are restoring a saved game.
		gfRestoringCiviliansFromTempFile = FALSE;
		FileClose( hfile );
		return TRUE;
	}

	// WANNE: Fix by Headrock:
	// Turns out that the program is saving the number of used Placement Slots to the temp file fine, but if that number is equal to the limit of how many enemies/civvies are allowed in a sector by the INI settings, it leads to an assertion error instead when trying to re-load that sector.
	if( slots < 0 || slots > (int)gGameExternalOptions.ubGameMaximumNumberOfCivilians )
	{
		//bad IO!
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- illegal slot value of %d.	KM", slots );
		#endif
		goto FAIL_LOAD;
	}

	//For all the enemy slots (enemy/creature), clear the fPriorityExistance flag.	We will use these flags
	//to determine which slots have been modified as we load the data into the map pristine soldier init list.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->fPriorityExistance )
		{
			if( curr->pBasicPlacement->bTeam == CIV_TEAM )
			{
				curr->pBasicPlacement->fPriorityExistance = FALSE;
			}
		}
		curr = curr->next;
	}

	for( i = 0; i < slots; i++ )
	{
		if ( !tempDetailedPlacement.Load(hfile, gCivPreservedTempFileVersion[SECTOR(sSectorX, sSectorY)], true) )
		{
			#ifdef JA2TESTVERSION
				sprintf( zReason, "Civilian -- EOF while reading tempDetailedPlacment %d.	KM", i );
			#endif
			goto FAIL_LOAD;
		}
		curr = gSoldierInitHead;
		while( curr )
		{
			if( !curr->pBasicPlacement->fPriorityExistance )
			{
				if( !curr->pDetailedPlacement || curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == NO_PROFILE )
				{
					if( curr->pBasicPlacement->bTeam == tempDetailedPlacement.bTeam )
					{
						curr->pBasicPlacement->fPriorityExistance = TRUE;

						if( !curr->pDetailedPlacement )
						{
							//need to upgrade the placement to detailed placement
							curr->pDetailedPlacement = new SOLDIERCREATE_STRUCT;
						}
						//now replace the map pristine placement info with the temp map file version..
						*curr->pDetailedPlacement = tempDetailedPlacement;

						curr->pBasicPlacement->fPriorityExistance	=	TRUE;
						curr->pBasicPlacement->ubDirection					= curr->pDetailedPlacement->ubDirection;
						curr->pBasicPlacement->bOrders						= curr->pDetailedPlacement->bOrders;
						curr->pBasicPlacement->bAttitude					= curr->pDetailedPlacement->bAttitude;
						curr->pBasicPlacement->ubBodyType					= curr->pDetailedPlacement->ubBodyType;
						curr->pBasicPlacement->fOnRoof						= curr->pDetailedPlacement->fOnRoof;
						curr->pBasicPlacement->ubSoldierClass			= curr->pDetailedPlacement->ubSoldierClass;
						curr->pBasicPlacement->ubCivilianGroup		= curr->pDetailedPlacement->ubCivilianGroup;
						curr->pBasicPlacement->fHasKeys						= curr->pDetailedPlacement->fHasKeys;
						curr->pBasicPlacement->usStartingGridNo		= curr->pDetailedPlacement->sInsertionGridNo;

						curr->pBasicPlacement->bPatrolCnt			= curr->pDetailedPlacement->bPatrolCnt;
						memcpy( curr->pBasicPlacement->sPatrolGrid, curr->pDetailedPlacement->sPatrolGrid,
							sizeof( INT32 ) * curr->pBasicPlacement->bPatrolCnt );


						if( curr->pDetailedPlacement->bLife < curr->pDetailedPlacement->bLifeMax )
						{ //Add 4 life for every hour that passes.
							INT32 iNewLife;
							iNewLife = curr->pDetailedPlacement->bLife + uiTimeSinceLastLoaded / 15;
							iNewLife = min( curr->pDetailedPlacement->bLifeMax, iNewLife );
							curr->pDetailedPlacement->bLife = (INT8)iNewLife;
						}

						if( curr->pBasicPlacement->bTeam == CIV_TEAM )
						{
//def:				AddPlacementToWorld( curr );
							break;
						}

					}
				}
			}
			curr = curr->next;
		}
	}

	// now remove any non-priority placement which matches the conditions!
	curr = gSoldierInitHead;
	fDeleted = FALSE;
	while( curr )
	{
		if( !curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pDetailedPlacement || curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == NO_PROFILE )
			{
				if( curr->pBasicPlacement->bTeam == tempDetailedPlacement.bTeam )
				{
					// Save pointer to the next guy in the list
					// and after deleting, set the 'curr' to that guy
					temp = curr->next;
					RemoveSoldierNodeFromInitList( curr );
					curr = temp;
					fDeleted = TRUE;
				}
			}
		}
		if ( fDeleted )
		{
			// we've already done our pointer update so don't advance the pointer
			fDeleted = FALSE;
		}
		else
		{
			curr = curr->next;
		}
	}

	FileRead( hfile, &ubSectorID, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- EOF while reading ubSectorID.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	/*
	if( ubSectorID != SECTOR( sSectorX, sSectorY ) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Civilian -- ubSectorID mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	*/

	//successful
	FileClose( hfile );
	return TRUE;

	FAIL_LOAD:
		//The temp file load failed either because of IO problems related to hacking/logic, or
		//various checks failed for hacker validation.	If we reach this point, the "error: exit game"
		//dialog would appear in a non-testversion.
		FileClose( hfile );
		#ifdef JA2TESTVERSION
			AssertMsg( 0, zReason );
		#endif
		return FALSE;
}


//If we are saving a game and we are in the sector, we will need to preserve the links between the
//soldiers and the soldier init list.	Otherwise, the temp file will be deleted.
BOOLEAN NewWayOfSavingEnemyAndCivliansToTempFile( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, BOOLEAN fEnemy, BOOLEAN fValidateOnly )
{
	SOLDIERINITNODE *curr;
	SOLDIERTYPE *pSoldier;
	INT32 i;
	INT32 slots = 0;
	UINT32 uiNumBytesWritten;
	UINT32 uiTimeStamp;
	HWFILE hfile;
//	CHAR8		zTempName[ 128 ];
	CHAR8		zMapName[ 128 ];
	UINT8 ubSectorID;

	UINT8	ubStartID=0;
	UINT8	ubEndID = 0;



	//if we are saving the enemy info to the enemy temp file
	if( fEnemy )
	{
		ubStartID = ENEMY_TEAM;
		ubEndID = CREATURE_TEAM;
	}

	//else its the civilian team
	else
	{
		ubStartID = CIV_TEAM;
		ubEndID = CIV_TEAM;
	}



	//STEP ONE:	Prep the soldiers for saving...

	//modify the map's soldier init list to reflect the changes to the members still alive...
	for( i = gTacticalStatus.Team[ ubStartID ].bFirstID; i <= gTacticalStatus.Team[ ubEndID ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];

		//make sure the person is active, alive, in the sector, and is not a profiled person
		if( pSoldier && pSoldier->bActive /*&& pSoldier->bInSector*/ && pSoldier->stats.bLife && pSoldier->ubProfile == NO_PROFILE )
		{ //soldier is valid, so find the matching soldier init list entry for modification.
			curr = gSoldierInitHead;
			while( curr && curr->pSoldier != pSoldier )
			{
				curr = curr->next;
			}
			if( curr && curr->pSoldier == pSoldier && pSoldier->ubProfile == NO_PROFILE )
			{ //found a match.

				if( !fValidateOnly )
				{
					if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
					{
						if( !curr->pDetailedPlacement )
						{ //need to upgrade the placement to detailed placement
							curr->pBasicPlacement->fDetailedPlacement = TRUE;
							curr->pDetailedPlacement = new SOLDIERCREATE_STRUCT; //(SOLDIERCREATE_STRUCT*)MemAlloc( SIZEOF_SOLDIERCREATE_STRUCT );
						}
						*curr->pDetailedPlacement = *pSoldier;

						//we don't want the player to think that all the enemies start in the exact position when we
						//left the map, so randomize the start locations either current position or original position.
						if( PreRandom( 2 ) )
						{
							//use current position, which is already copied over
						}
						else
						{
							//use original position
							curr->pDetailedPlacement->fOnRoof				= curr->pBasicPlacement->fOnRoof;
							curr->pDetailedPlacement->sInsertionGridNo		= curr->pBasicPlacement->usStartingGridNo;
						}
					}
				}

				//DONE, now increment the counter, so we know how many there are.
				slots++;
			}
		}
	}

	if( !slots )
	{
		if( fEnemy )
		{
			//No need to save anything, so return successfully
			RemoveEnemySoldierTempFile( sSectorX, sSectorY, bSectorZ );
			return TRUE;
		}
		else
		{
			//No need to save anything, so return successfully
			RemoveCivilianTempFile( sSectorX, sSectorY, bSectorZ );
			return( TRUE );
		}

	}

	if( fValidateOnly )
	{
		return( TRUE );
	}

	//STEP TWO:	Set up the temp file to write to.

	//Convert the current sector location into a file name
	//GetMapFileName( sSectorX, sSectorY, bSectorZ, zTempName, FALSE );

	if( fEnemy )
	{
		//add the 'e' for 'Enemy preserved' to the front of the map name
		//sprintf( zMapName, "%s\\e_%s", MAPS_DIR, zTempName);
		GetMapTempFileName( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );
	}
	else
	{
		//add the 'e' for 'Enemy preserved' to the front of the map name
		//sprintf( zMapName, "%s\\c_%s", MAPS_DIR, zTempName);
		GetMapTempFileName( SF_CIV_PRESERVED_TEMP_FILE_EXISTS, zMapName, sSectorX, sSectorY, bSectorZ );
	}

	//Open the file for writing, Create it if it doesnt exist
	hfile = FileOpen( zMapName, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( hfile == 0 )
	{	//Error opening map modification file
		return FALSE;
	}

	FileWrite( hfile, &sSectorY, 2, &uiNumBytesWritten );
	if( uiNumBytesWritten != 2 )
	{
		goto FAIL_SAVE;
	}

	//STEP THREE:	Save the data

	//this works for both civs and enemies
	SaveSoldierInitListLinks( hfile );

	FileWrite( hfile, &sSectorX, 2, &uiNumBytesWritten );
	if( uiNumBytesWritten != 2 )
	{
		goto FAIL_SAVE;
	}

	//This check may appear confusing.	It is intended to abort if the player is saving the game.	It is only
	//supposed to preserve the links to the placement list, so when we finally do leave the level with enemies remaining,
	//we will need the links that are only added when the map is loaded, and are normally lost when restoring a save.
	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
	{
		slots = 0;
	}

	FileWrite( hfile, &slots,		4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
	{
		goto FAIL_SAVE;
	}

	uiTimeStamp = GetWorldTotalMin();
	FileWrite( hfile, &uiTimeStamp, 4, &uiNumBytesWritten );
	if( uiNumBytesWritten != 4 )
	{
		goto FAIL_SAVE;
	}

	FileWrite( hfile, &bSectorZ, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
	{
		goto FAIL_SAVE;
	}

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
	{
		//if we are saving the game, we don't need to preserve the soldier information, just
		//preserve the links to the placement list.
		slots = 0;
		FileClose( hfile );

		if( fEnemy )
		{
			SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS );
		}
		else
		{
			SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_CIV_PRESERVED_TEMP_FILE_EXISTS );
		}
		return TRUE;
	}

	for( i = gTacticalStatus.Team[ ubStartID ].bFirstID; i <= gTacticalStatus.Team[ ubEndID ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];
		// CJC: note that bInSector is not required; the civ could be offmap!
		if( pSoldier->bActive /*&& pSoldier->bInSector*/ && pSoldier->stats.bLife )
		{
			//soldier is valid, so find the matching soldier init list entry for modification.
			curr = gSoldierInitHead;
			while( curr && curr->pSoldier != pSoldier )
			{
				curr = curr->next;
			}
			if( curr && curr->pSoldier == pSoldier && pSoldier->ubProfile == NO_PROFILE )
			{
				//found a match.
				if ( !curr->pDetailedPlacement->Save(hfile, FALSE) )
				{
					goto FAIL_SAVE;
				}
			}
		}
	}

	ubSectorID = SECTOR( sSectorX, sSectorY );
	FileWrite( hfile, &ubSectorID, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
	{
		goto FAIL_SAVE;
	}

	FileClose( hfile );

	if( fEnemy )
	{
		SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS );
		gEnemyPreservedTempFileVersion[SECTOR(sSectorX, sSectorY)] = SAVE_GAME_VERSION;
	}
	else
	{
		SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_CIV_PRESERVED_TEMP_FILE_EXISTS );
		gCivPreservedTempFileVersion[SECTOR(sSectorX, sSectorY)] = SAVE_GAME_VERSION;
	}

	return TRUE;

	FAIL_SAVE:
		FileClose( hfile );
		return FALSE;
}





BOOLEAN CountNumberOfElitesRegularsAdminsAndCreaturesFromEnemySoldiersTempFile( UINT8 *pubNumElites, UINT8 *pubNumRegulars, UINT8 *pubNumAdmins, UINT8 *pubNumCreatures, UINT8 *pubNumTanks, UINT8 *pubNumJeeps )
{
//	SOLDIERINITNODE *curr;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	INT32 i;
	INT32 slots = 0;
	UINT32 uiNumBytesRead;
	UINT32 uiTimeStamp;
	HWFILE hfile;
	INT16 sSectorX, sSectorY;
	CHAR8		zMapName[ 128 ];
	#ifdef JA2TESTVERSION
		CHAR8		zReason[256];
	#endif
	INT8 bSectorZ;
	UINT8 ubSectorID;
//	UINT8 ubNumElites = 0, ubNumTroops = 0, ubNumAdmins = 0, ubNumCreatures = 0;
//	UINT8 ubStrategicElites, ubStrategicTroops, ubStrategicAdmins, ubStrategicCreatures;
		
	//make sure the variables are initialized
	*pubNumElites = 0;
	*pubNumRegulars = 0;
	*pubNumAdmins = 0;
	*pubNumTanks = 0;
	*pubNumJeeps = 0;
	*pubNumCreatures = 0;
	
	//STEP ONE:	Set up the temp file to read from.
	
	//Convert the current sector location into a file name
//	GetMapFileName( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTempName, FALSE );

	//add the 'e' for 'Enemy preserved' to the front of the map name
//	sprintf( zMapName, "%s\\e_%s", MAPS_DIR, zTempName);

	GetMapTempFileName( SF_ENEMY_PRESERVED_TEMP_FILE_EXISTS, zMapName, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//Open the file for reading
	hfile = FileOpen( zMapName, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
	if( hfile == 0 )
	{	//Error opening map modification file
		return FALSE;
	}

	//STEP TWO:	determine whether or not we should use this data.
	//because it is the demo, it is automatically used.

	FileRead( hfile, &sSectorY, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- EOF while reading sSectorY.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorY != sSectorY )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- sSectorY mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

//	LoadSoldierInitListLinks( hfile );
	LookAtButDontProcessEnemySoldierInitListLinks( hfile );

	//STEP THREE:	read the data

	FileRead( hfile, &sSectorX, 2, &uiNumBytesRead );
	if( uiNumBytesRead != 2 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- EOF while reading sSectorX.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	if( gWorldSectorX != sSectorX )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- sSectorX mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &slots,		4, &uiNumBytesRead );
	// WDS - make number of mercenaries, etc. be configurable
	if( (uiNumBytesRead != 4) || (slots > TOTAL_SOLDIERS) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- EOF while reading slots.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	FileRead( hfile, &uiTimeStamp, 4, &uiNumBytesRead );
	if( uiNumBytesRead != 4 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- EOF while reading uiTimeStamp.	KM" );
		#endif
		goto FAIL_LOAD;
	}
	
	FileRead( hfile, &bSectorZ, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- EOF while reading bSectorZ.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	if( gbWorldSectorZ != bSectorZ )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- bSectorZ mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	if( !slots )
	{
		//no need to restore the enemy's to the map.	This means we are restoring a saved game.
		FileClose( hfile );
		return TRUE;
	}

	if( slots < 0 || slots >= (int)gGameExternalOptions.ubGameMaximumNumberOfEnemies )
	{ //bad IO!
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- illegal slot value of %d.	KM", slots );
		#endif
		goto FAIL_LOAD;
	}

/*
	//get the number of enemies in this sector.
	if( bSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );
		if( !pSector )
		{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "EnemySoldier -- Couldn't find underground sector info for (%d,%d,%d)	KM", sSectorX, sSectorY, bSectorZ );
		#endif
			goto FAIL_LOAD;
		}
		ubStrategicElites		= pSector->ubNumElites;
		ubStrategicTroops		= pSector->ubNumTroops;
		ubStrategicAdmins		= pSector->ubNumAdmins;
		ubStrategicCreatures = pSector->ubNumCreatures;
	}
	else
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubStrategicCreatures = pSector->ubNumCreatures;
		GetNumberOfEnemiesInSector( sSectorX, sSectorY, &ubStrategicAdmins, &ubStrategicTroops, &ubStrategicElites );
	}
*/

	for( i = 0; i < slots; ++i )
	{
		if ( !tempDetailedPlacement.Load(hfile, gEnemyPreservedTempFileVersion[SECTOR( sSectorX,sSectorY)], true) )
		{
			#ifdef JA2TESTVERSION
				sprintf( zReason, "Check EnemySoldier -- EOF while reading tempDetailedPlacment %d.	KM", i );
			#endif
			goto FAIL_LOAD;
		}

		//increment the current type of soldier
		switch( tempDetailedPlacement.ubSoldierClass )
		{
			case SOLDIER_CLASS_ELITE:
				(*pubNumElites)++;
				break;
			case SOLDIER_CLASS_ARMY:
				(*pubNumRegulars)++;
				break;
			case SOLDIER_CLASS_ADMINISTRATOR:
				(*pubNumAdmins)++;
				break;
			case SOLDIER_CLASS_CREATURE:
				(*pubNumCreatures)++;
				break;
			case SOLDIER_CLASS_TANK:
				if ( TANK( (&tempDetailedPlacement) ) )
				{
					(*pubNumTanks)++;
				}
				break;
			case SOLDIER_CLASS_JEEP:
				if ( COMBAT_JEEP( (&tempDetailedPlacement) ) )
				{
					(*pubNumJeeps)++;
				}
				break;
		}
/*
		while( curr )
		{
			if( !curr->pBasicPlacement->fPriorityExistance )
			{
				if( curr->pBasicPlacement->bTeam == tempDetailedPlacement.bTeam )
				{
					curr->pBasicPlacement->fPriorityExistance = TRUE;
					if( !curr->pDetailedPlacement )
					{ //need to upgrade the placement to detailed placement
						curr->pDetailedPlacement = new SOLDIERCREATE_STRUCT;
					}
					//now replace the map pristine placement info with the temp map file version..
					*curr->pDetailedPlacement = tempDetailedPlacement;

					curr->pBasicPlacement->fPriorityExistance	=	TRUE;
					curr->pBasicPlacement->ubDirection					= curr->pDetailedPlacement->ubDirection;
					curr->pBasicPlacement->bOrders						= curr->pDetailedPlacement->bOrders;
					curr->pBasicPlacement->bAttitude					= curr->pDetailedPlacement->bAttitude;
					curr->pBasicPlacement->ubBodyType					= curr->pDetailedPlacement->ubBodyType;
					curr->pBasicPlacement->fOnRoof						= curr->pDetailedPlacement->fOnRoof;
					curr->pBasicPlacement->ubSoldierClass			= curr->pDetailedPlacement->ubSoldierClass;
					curr->pBasicPlacement->ubCivilianGroup		= curr->pDetailedPlacement->ubCivilianGroup;
					curr->pBasicPlacement->fHasKeys						= curr->pDetailedPlacement->fHasKeys;
					curr->pBasicPlacement->usStartingGridNo		= curr->pDetailedPlacement->sInsertionGridNo;

					curr->pBasicPlacement->bPatrolCnt			= curr->pDetailedPlacement->bPatrolCnt;
					memcpy( curr->pBasicPlacement->sPatrolGrid, curr->pDetailedPlacement->sPatrolGrid,
						sizeof( INT32 ) * curr->pBasicPlacement->bPatrolCnt );


					//Add preserved placements as long as they don't exceed the actual population.
					switch( curr->pBasicPlacement->ubSoldierClass )
					{
						case SOLDIER_CLASS_ELITE:
							ubNumElites++;
							if( ubNumElites <= ubStrategicElites )
							{
								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_ARMY:
							ubNumTroops++;
							if( ubNumTroops <= ubStrategicTroops )
							{
								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_ADMINISTRATOR:
							ubNumAdmins++;
							if( ubNumAdmins <= ubStrategicAdmins )
							{
								AddPlacementToWorld( curr );
							}
							break;
						case SOLDIER_CLASS_CREATURE:
							ubNumCreatures++;
							if( ubNumCreatures <= ubStrategicCreatures )
							{
								AddPlacementToWorld( curr );
							}
							break;
					}
					break;
				}
			}
			curr = curr->next;
		}
*/
	}

	FileRead( hfile, &ubSectorID, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- EOF while reading ubSectorID.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	if( ubSectorID != SECTOR( sSectorX, sSectorY ) )
	{
		#ifdef JA2TESTVERSION
			sprintf( zReason, "Check EnemySoldier -- ubSectorID mismatch.	KM" );
		#endif
		goto FAIL_LOAD;
	}

	//successful
	FileClose( hfile );
	return TRUE;

	FAIL_LOAD:
		//The temp file load failed either because of IO problems related to hacking/logic, or
		//various checks failed for hacker validation.	If we reach this point, the "error: exit game"
		//dialog would appear in a non-testversion.
		FileClose( hfile );
#ifdef JA2TESTVERSION
		AssertMsg( 0, zReason );
#endif

		return FALSE;
}




