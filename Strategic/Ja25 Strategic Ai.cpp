#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "strategicmap.h"
	#include "strategic.h"
	#include "Strategic Mines.h"
	#include "types.h"
	#include "memory.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <math.h>
	#include <time.h>
	#include "jascreens.h"
	#include "worlddef.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "isometric utils.h"
	#include "worlddef.h"
	#include "worlddat.h"
	#include "text.h"
	#include "Soldier add.h"
	#include "soldier macros.h"
	#include "Strategic Pathing.h"
	#include "soldier create.h"
	#include "handle UI.h"
	#include "faces.h"
	#include "renderworld.h"
	#include "Map Information.h"
	#include "Assignments.h"
	#include "message.h"
	#include "Game Events.h"
	#include "quests.h"
	#include "FileMan.h"
	#include "animated progressbar.h"
	#include "Strategic Movement.h"
	#include "Campaign Types.h"
	#include "Exit Grids.h"
	#include "Tactical Save.h"
	#include "animation control.h"
	#include "squads.h"
	#include "Overhead.h"
	#include "Strategic Town Loyalty.h"
	#include "Queen Command.h"
	#include "cursor control.h"
	#include "PreBattle Interface.h"
	#include "gameloop.h"
	#include "Random.h"
	#include "ai.h"
	#include "keys.h"
	#include "Tactical Placement GUI.h"
	#include "Map Screen Helicopter.h"
	#include "Player Command.h"
	#include "Event Pump.h"
	#include "air raid.h"
	#include "Strategic Mines.h"
	#include "SaveLoadMap.h"
	#include "Militia Control.h"
	#include "gamesettings.h"
	#include "Town Militia.h"
	#include "sysutil.h"
	#include "Debug Control.h"
	#include "Queen Command.h"
	#include "Strategic Event Handler.h"
	#include "MessageBoxScreen.h"
	#include "interface dialogue.h"
	#include "Map Screen Interface.h"
	#include "Bullets.h"
	#include "physics.h"
	#include "Auto Resolve.h"
	#include "cursors.h"
#endif

#ifdef JA2UB
#include "Soldier Init List.h"
#include "Dialogue Control.h"
#include "Game Clock.h"
#include "opplist.h"
#include "Ja25Update.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "SaveLoadGame.h"
#include "Campaign.h"
#include "Strategic Status.h"
#include "ub_config.h"
//** Defines *******************************************************************

#define		JA25_SECTOR_H7_INITIAL_ATTACK													( 7 * 60 + 10 )

#define		JA25_TIME_BETWEEN_SAI_UPDATES_EASY										( 60 * 5 )
#define		JA25_TIME_BETWEEN_SAI_UPDATES_NORMAL									( 60 * 3 )
#define		JA25_TIME_BETWEEN_SAI_UPDATES_HARD										( 60 * 2 )

#define		JA25_SAI_RANDOM_TIME																	( 60 * 2 )

#define		JA25_MINIMUM_TIME_BETWEEN_ATTACKS											( 4 * 60 )  //4 hours

#define		JA25_SAI_DEBUG_FILE																		"JA25AiDebug.rtf"


//** Global Variables  *********************************************************

//extern	UINT32	guiSaveGameVersion;

INT32	giNumJA25Sectors = NUM_CAMPAIGN_JA25_SECTORS;


JA25_SAVE_INFO	gJa25SaveStruct;

JA25_SECTOR_AI	gJa25AiSectorStruct[CUSTOMSECTOR]; //*gJa25AiSectorStruct = NULL;

JA25_SECTOR_AI_MANAGER gJa25StrategicAi;


//BOOLEAN	gfEnemyShouldImmediatelySeekThePlayer=FALSE;

//** Prototypes ****************************************************************

//extern UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ, BOOLEAN fCampaignSector );


BOOLEAN		AddEnemiesToInitialSectorH7();
UINT32		GetNumberOfJA25EnemiesInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 *pNumAdmins, UINT8 *pNumTroops, UINT8 *pNumElites, UINT8 *pubNumTanks, UINT8 *pubNumJeeps );
void		SetNumberJa25EnemiesInSurfaceSector( INT32 iSectorID, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubNumJeeps );

void InitJa25InitialEnemiesInSector();
void InitJa25UnderGroundSectors();
void InitNumberOfEnemiesInAboveGroundSectors( );
void InitNumberOfEnemiesInUnderGroundSectors( );
void SetNumberOfJa25BloodCatsInSector( INT32 iSectorID, INT8 bNumBloodCats, INT8 bBloodCatPlacements  );
void SetNumberJa25EnemiesInUnderGroundSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubNumJeeps );

void ResetJa25SectorProbabilities();
BOOLEAN	InitJa25StrategicSectorAI( BOOLEAN fReset );


BOOLEAN		HandleAddingEnemiesToSector( INT16 sSaiSector, UINT8 ubNumEnemies, INT8 bAttackDirection, INT16 sGridNo );
BOOLEAN		HandleAddEnemiesToSectorPlayerIsntIn( INT16 sSaiSector, UINT8 ubNumEnemies );
void		Ja25SAI_DetermineWhichLevelToAttackFrom( INT16 sSaiSector, INT16 *psSector, INT8 *pbLevel );

BOOLEAN		AreAllPlayerMercTraversingBetweenSectors();

UINT32		GetMinimumTimeBetweenAttacks();

void			InitJohnKulbaInitialSector();

void			FixEnemyCounterInSectorBug();

void			AddEnemiesToFirstTunnelSector();
void			AddEnemiesToSecondTunnelSector();
UINT8			NumEnemiesToAttackFirstTunnelSector( UINT8 *pAdmins, UINT8 *pTroops, UINT8 *pElites, UINT8 *pTanks, UINT8 *pJeeps );
UINT8			NumEnemiesToAttackSecondTunnelSector( UINT8 *pAdmins, UINT8 *pTroops, UINT8 *pElites, UINT8 *pTanks, UINT8 *pJeeps );
void			RemoveAllEnemySoldierInitListLinks();
INT16			GetGridNoEnemyWillSeekWhenAttacking( INT8 bSaiSector );
void			SetH11NumEnemiesInSector();
BOOLEAN		HaveMercsEverBeenInComplex();
#ifdef JA2BETAVERSION
BOOLEAN		Ja25BetaDateToInvalidateExe();
#endif
//ppp
//void CopyJa25SectorAiData( JA25_SECTOR_AI *pDest, JA25_SECTOR_AI *pSource );

#ifdef JA2BETAVERSION
	BOOLEAN RecordJa25StrategicAiDecisions( INT16 sSectorAttacked, UINT8 ubNumEnemies );
	BOOLEAN OutputJA25SaiString( HWFILE hFile, CHAR *pString );
#endif



//** Functions  ****************************************************************



	UINT32	GetNumberOfJA25EnemiesInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 *pNumAdmins, UINT8 *pNumTroops, UINT8 *pNumElites, UINT8 *pNumTanks, UINT8 *pubNumJeeps )
{
	SECTORINFO *pSector;

	//if it is above ground
	if( bSectorZ == 0 )
	{
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];

		*pNumAdmins = pSector->ubNumAdmins;
		*pNumTroops = pSector->ubNumTroops;
		*pNumElites = pSector->ubNumElites;
		*pNumTanks = pSector->ubNumTanks;
		*pubNumJeeps = pSector->ubNumJeeps;
	}
	else
	{
		UNDERGROUND_SECTORINFO *pSector;

		pSector = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );
		if( pSector )
		{
			*pNumAdmins = pSector->ubNumAdmins;
			*pNumTroops = pSector->ubNumTroops;
			*pNumElites = pSector->ubNumElites;
			*pNumTanks = pSector->ubNumTanks;
			*pubNumJeeps = pSector->ubNumJeeps;
		}
	}

	return( 0 );
}

void InitJa25StrategicAi()
{
	//Initialize the array of new gun quotes
	InitNewGunArray();
/*
	#ifdef ENABLE_CUSTOM_MAP_INTERFACE
		//if the player is using a CUSTOM MAP SCENARIO disable the S.A.I.
		if( gJa25SaveStruct.fInCustomMap )
		{
			return;
		}
	#endif

	#ifdef JA2BETAVERSION
		InitJa25StrategicAiDecisions( FALSE );
	#endif
*/
	// Init and memset AI struct
//	giNumJA25Sectors = NUM_CAMPAIGN_JA25_SECTORS;
//	gJa25AiSectorStruct = MemAlloc( sizeof( JA25_SAVE_INFO ) * giNumJA25Sectors );
//	memset( gJa25AiSectorStruct, 0, sizeof( JA25_SAVE_INFO ) * giNumJA25Sectors );


	//add event at 7:03
	if ( gGameUBOptions.EventAttackInitialSectorIfPlayerStillThere == TRUE )
	{
		AddSameDayStrategicEvent( EVENT_ATTACK_INITIAL_SECTOR_IF_PLAYER_STILL_THERE, JA25_SECTOR_H7_INITIAL_ATTACK, 0 );
		AddEveryDayStrategicEventUsingSeconds(EVENT_ATTACK_INITIAL_SECTOR_IF_PLAYER_STILL_THERE,JA25_SECTOR_H7_INITIAL_ATTACK * 60,0);
	}

	//Init the underground sectors ( add them to the list of under ground )
	InitJa25UnderGroundSectors();

	//Init the # of enemies in all the sectors
	InitJa25InitialEnemiesInSector();

	//Init the Ja215 Stratigic AI
	InitJa25StrategicSectorAI( TRUE );
	
	gStrategicStatus.ubHighestProgress = CurrentPlayerProgressPercentage();

	//InitJohnKulbaInitialSector(); // wy³¹czone, skrypt lua
}


BOOLEAN ShouldEnemiesBeAddedToInitialSector()
{
	//if there are still players in the first sector
	if( gfWorldLoaded && 
			gWorldSectorX == JA2_5_START_SECTOR_X && 
			gWorldSectorY == JA2_5_START_SECTOR_Y &&
			AreAnyPlayerMercsStillInSector( JA2_5_START_SECTOR_X, JA2_5_START_SECTOR_Y, 0 ) )
	{
		//add enemies to the initial sector
		if( !AddEnemiesToInitialSectorH7() )
		{
			Assert( 0 );

			return( FALSE );
		}
	}

	return( TRUE );
}

BOOLEAN	AddEnemiesToInitialSectorH7()
{
	GROUP *pGroup;
	UINT8 ubSector;
	UINT32 uiWorldMin;

	UINT8	ubNumAdmins;
	UINT8	ubNumTroops;
	UINT8	ubNumElites;
	UINT8	ubNumTanks;
	UINT8	ubNumJeeps;

	UINT8	ubNumRemovedAdmins=0;
	UINT8	ubNumRemovedTroops=0;
	UINT8	ubNumRemovedElites=0;
	UINT8	ubNumRemovedTanks=0;
	UINT8	ubNumRemovedJeeps=0;

	ubSector = SECTOR( JA2_5_START_SECTOR_X, JA2_5_START_SECTOR_Y );

	//Get the number of enemies in the guard post sector
	GetNumberOfJA25EnemiesInSector( 8, MAP_ROW_H, 0, &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumTanks, &ubNumJeeps );

	//determine the #of enemies to travel to the initial heli sector
	if( ubNumAdmins > 1 )
		ubNumRemovedAdmins = ubNumAdmins / 3 + Random( 3 );

	if( ubNumTroops > 1 )
		ubNumRemovedTroops = ubNumTroops / 3 + Random( 3 );

	if( ubNumElites > 1 )
		ubNumRemovedElites = ubNumElites / 3 + + Random( 3 );

	if( ubNumTanks > 1 )
		ubNumRemovedTanks = ubNumTanks / 3 + + Random( 3 );

	if( ubNumJeeps > 1 )
		ubNumRemovedJeeps = ubNumJeeps / 3 + + Random( 3 );

	//deduct the # that are moving from the # in the guard post sector
//	SetNumberJa25EnemiesInSurfaceSector( SEC_H8, (UINT8)(ubNumAdmins-ubNumRemovedAdmins), (UINT8)(ubNumTroops-ubNumRemovedTroops), (UINT8)(ubNumElites-ubNumRemovedElites) );
	SetNumberJa25EnemiesInSector( 8, MAP_ROW_H, 0, (UINT8)(ubNumAdmins-ubNumRemovedAdmins), (UINT8)(ubNumTroops-ubNumRemovedTroops), (UINT8)(ubNumElites-ubNumRemovedElites), 
								  (UINT8)(ubNumTanks-ubNumRemovedTanks), (UINT8)(ubNumJeeps-ubNumRemovedJeeps) );

	uiWorldMin = GetWorldTotalMin();

	pGroup = CreateNewEnemyGroupDepartingFromSector( ubSector+1, ubNumRemovedAdmins, ubNumRemovedTroops, ubNumRemovedElites, ubNumRemovedTanks, ubNumRemovedJeeps );

	if( pGroup == NULL )
	{
		Assert( 0 );
		return( FALSE );
	}

	//Set the sector to attack
	pGroup->ubNextX = (UINT8)JA2_5_START_SECTOR_X;
	pGroup->ubNextY = (UINT8)JA2_5_START_SECTOR_Y;

	pGroup->uiTraverseTime = 10;

	pGroup->pEnemyGroup->ubIntention = ASSAULT;

	SetGroupArrivalTime( pGroup, uiWorldMin+1 );

	pGroup->ubMoveType = ONE_WAY;


//	AddStrategicEvent( EVENT_GROUP_ARRIVAL, pGroup->uiArrivalTime, pGroup->ubGroupID );
	AddAdvancedStrategicEvent( ONETIME_EVENT, EVENT_GROUP_ARRIVAL, GetWorldTotalSeconds( ) + 1, pGroup->ubGroupID );

	return( TRUE );
}

BOOLEAN	AreAnyPlayerMercsStillInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT32 iCnt;

	for( iCnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; iCnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCnt++ )
	{
		if( MercPtrs[ iCnt ]->bActive )
		{
			if( MercPtrs[ iCnt ]->sSectorX == sSectorX &&
					MercPtrs[ iCnt ]->sSectorY == sSectorY &&
					MercPtrs[ iCnt ]->bSectorZ == bSectorZ )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


void InitJa25InitialEnemiesInSector()
{
	//
	// Set the number of enemies in each of the sectors
	//
	//InitNumberOfEnemiesInAboveGroundSectors( ); //wy³¹czono, skrypt lua

	//Below Ground
	//InitNumberOfEnemiesInUnderGroundSectors( ); //wy³¹czono, skrypt lua
}

void InitJa25UnderGroundSectors()
{
/*	UNDERGROUND_SECTORINFO *curr;

	//Under the mine ( i13
	curr = NewUndergroundNode( 13, 9, 1, TRUE );
	Assert( curr );

	//under the power generator j13
	curr = NewUndergroundNode( 13, 10, 1, TRUE );
	Assert( curr );

	//the tunnels	J14, k14
	curr = NewUndergroundNode( 14, 10, 1, TRUE );
	Assert( curr );
	curr = NewUndergroundNode( 14, 11, 1, TRUE );
	Assert( curr );

	//under the complex, K15, level 1 & 2
	curr = NewUndergroundNode( 15, 11, 1, TRUE );
	Assert( curr );
	curr = NewUndergroundNode( 15, 11, 2, TRUE );
	Assert( curr );

	//under the complex, 2nd part, L15, level 2 & 3
	curr = NewUndergroundNode( 15, 12, 2, TRUE );
	Assert( curr );
	curr = NewUndergroundNode( 15, 12, 3, TRUE );
	Assert( curr );
*/
}

void SetNumberJa25EnemiesInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubNumJeeps )
{
	//if its a ground level
	
	if( bSectorZ == 0 )
	{
		SetNumberJa25EnemiesInSurfaceSector( SECTOR( sSectorX, sSectorY ), ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//its an underground level
	else
	{
		SetNumberJa25EnemiesInUnderGroundSector( sSectorX, sSectorY, bSectorZ, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}
	
}

void SetNumberJa25EnemiesInSurfaceSector( INT32 iSectorID, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubNumJeeps )
{
	SECTORINFO* pSector = &SectorInfo[iSectorID];
	pSector->ubNumAdmins = ubNumAdmins;
	pSector->ubNumTroops = ubNumTroops;
	pSector->ubNumElites = ubNumElites;
	pSector->ubNumTanks = ubNumTanks;
	pSector->ubNumJeeps = ubNumJeeps;
}

void SetNumberOfJa25BloodCatsInSector( INT32 iSectorID, INT8 bNumBloodCats, INT8 bBloodCatPlacements )
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ iSectorID ];
	pSector->bBloodCats = bNumBloodCats;

	pSector->bBloodCatPlacements = bBloodCatPlacements;
}

void SetNumberJa25EnemiesInUnderGroundSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, UINT8 ubNumTanks, UINT8 ubNumJeeps )
{
	UNDERGROUND_SECTORINFO *pSector=NULL;

	pSector = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );
	Assert( pSector );

	//Set the values
	pSector->ubNumAdmins = ubNumAdmins;
	pSector->ubNumTroops = ubNumTroops;
	pSector->ubNumElites = ubNumElites;
	pSector->ubNumTanks = ubNumTanks;
	pSector->ubNumJeeps = ubNumJeeps;
}

void InitNumberOfEnemiesInAboveGroundSectors( )
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps=0;

	//H7
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
		}
		SetNumberJa25EnemiesInSurfaceSector( SEC_H7, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}



	//SEC_H8:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 6 + Random( 4 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 13 + Random( 3 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 16 + Random( 6 );
				ubNumElites = 2 + Random( 2 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 19 + Random( 9 );
				ubNumElites = 2 + Random( 2 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_H8, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

		//Guard Post
	//SEC_H9:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 5 );
				ubNumElites = 1 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 16 + Random( 4 );
				ubNumElites = 1 + Random( 0 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 19 + Random( 4 );
				ubNumElites = 3 + Random( 2 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 19 + Random( 6 );
				ubNumElites = 3 + Random( 2 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_H9, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	// SEC_H10:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 6 + Random( 5 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 9 + Random( 3 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 4 );
				ubNumElites = 2 + Random( 4 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 20 + Random( 4 );
				ubNumElites = 2 + Random( 4 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_H10, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//SEC_H11
	SetH11NumEnemiesInSector();


	// SEC_I9:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 6 + Random( 5 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 9 + Random( 4 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 4 );
				ubNumElites = 2 + Random( 4 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 20 + Random( 6 );
				ubNumElites = 2 + Random( 4 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_I9, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//First part of town
	// SEC_I10:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 13 + Random( 6 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 16 + Random( 4 );
				ubNumElites = 1 + Random( 3 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 14 + Random( 4 );
				ubNumElites = 5 + Random( 3 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 20 + Random( 4 );
				ubNumElites = 5 + Random( 3 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_I10, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

		//Second part of town
	//SEC_I11:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 14 + Random( 7 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 16 + Random( 5 );
				ubNumElites = 2 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 14 + Random( 4 );
				ubNumElites = 5 + Random( 4 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 17 + Random( 4 );
				ubNumElites = 5 + Random( 4 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_I11, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//SEC_I12:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 8 + Random( 6 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 12 + Random( 5 );
				ubNumElites = 2 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 12 + Random( 6 );
				ubNumElites = 3 + Random( 2 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 20 + Random( 6 );
				ubNumElites = 3 + Random( 2 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_I12, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//Abandoned mine
	// SEC_I13:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 0 + Random( 0 );
				ubNumElites = 0 + Random( 0 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_I13, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	// SEC_J11:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 6 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 3 );
				ubNumElites = 3 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 11 + Random( 5 );
				ubNumElites = 6 + Random( 3 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 20 + Random( 5 );
				ubNumElites = 6 + Random( 3 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_J11, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	// SEC_J12:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 12 + Random( 4 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 4 );
				ubNumElites = 1 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 11 + Random( 3 );
				ubNumElites = 3 + Random( 2 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 21 + Random( 3 );
				ubNumElites = 3 + Random( 2 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_J12, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//Power Generator, Ground Level
	// SEC_J13:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 15 + Random( 6 );
				ubNumElites = 1 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 16 + Random( 5 );
				ubNumElites = 5 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 18 + Random( 4 );
				ubNumElites = 6 + Random( 3 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 18 + Random( 4 );
				ubNumElites = 7 + Random( 3 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_J13, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}

	//Complex, Ground Level
	// SEC_K15:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 14 + Random( 5 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 18 + Random( 3 );
				ubNumElites = 4 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 17 + Random( 3 );
				ubNumElites = 8 + Random( 2 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 17 + Random( 3 );
				ubNumElites = 8 + Random( 2 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_K15, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}
}

void InitNumberOfEnemiesInUnderGroundSectors( )
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps=0;

	//Mine Sector Level 1
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
	}
	SetNumberJa25EnemiesInSector( 13, 9, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );


	//Power Generator Level 1
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
	}
	SetNumberJa25EnemiesInSector( 13, 10, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );


	//Tunnel First Part, Level 1
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
	}
	SetNumberJa25EnemiesInSector( 14, 10, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );


	//Tunnel Second Part, Level 1
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 0 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
	}
	SetNumberJa25EnemiesInSector( 14, 11, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );



	//Complex, Level 1, K15
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 14 + Random( 6 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 14 + Random( 4 );
			ubNumElites = 4 + Random( 3 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 15 + Random( 4 );
			ubNumElites = 6 + Random( 4 );
			break;
	}
	SetNumberJa25EnemiesInSector( 15, 11, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );


	//Complex, Level 2, K15
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 10 + Random( 3 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 15 + Random( 2 );
			ubNumElites = 5 + Random( 2 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 12 + Random( 3 );
			ubNumElites = 10 + Random( 2 );
			break;
	}
	SetNumberJa25EnemiesInSector( 15, 11, 2, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );


	//Complex, Level 2, L15
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 12 + Random( 2 );
			ubNumElites = 1 + Random( 2 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 17 + Random( 2 );
			ubNumElites = 5 + Random( 2 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 10 + Random( 3 );
			ubNumElites = 14 + Random( 2 );
			break;
	}
	SetNumberJa25EnemiesInSector( 15, 12, 2, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );


	//Complex, Level 3, L15
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 26 + Random( 0 );
			ubNumElites = 2 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 20 + Random( 0 );
			ubNumElites = 8 + Random( 0 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 13 + Random( 0 );
			ubNumElites = 15 + Random( 0 );
			break;
	}
	SetNumberJa25EnemiesInSector( 15, 12, 3, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
}

void InitJa25SaveStruct()
{
	memset( &gJa25SaveStruct, 0, sizeof( JA25_SAVE_INFO ) );

	gJa25SaveStruct.iPowerGenFanPositionSndID = -1;
}


BOOLEAN SaveJa25SaveInfoToSaveGame( HWFILE hFile )
{
	UINT32 uiNumBytesWritten;
	UINT32	uiSize = sizeof( JA25_SAVE_INFO );
//	CHAR8		zString[512];

	//Make sure struct is 1000 bytes in size
//	sprintf( zString, "JA2 SaveInfo Struct != 1000, it is: %d", uiSize );
//	AssertMsg( uiSize == 1000, zString );

	//Make sure struct is 32 bytes in size
//	sprintf( zString, "AI manager != 32, it is: %d", sizeof( JA25_SECTOR_AI_MANAGER ) );
//	AssertMsg( sizeof( JA25_SECTOR_AI_MANAGER ) == 32, zString );

	//Make sure struct is 840 bytes in size
//	sprintf( zString, "AI sector Struct != 40, it is: %d", sizeof( JA25_SECTOR_AI ) );
//	AssertMsg( sizeof( JA25_SECTOR_AI ) == 40, zString );


	FileWrite( hFile, &gJa25SaveStruct, sizeof( JA25_SAVE_INFO ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( JA25_SAVE_INFO ) )
	{
		return( FALSE );
	}



	//
	// Ai Manager
	//
	FileWrite( hFile, &gJa25StrategicAi, sizeof( JA25_SECTOR_AI_MANAGER ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( JA25_SECTOR_AI_MANAGER ) )
	{
		return( FALSE );
	}

	//
	// Sector Info
	//

	FileWrite( hFile, &gJa25AiSectorStruct, sizeof( JA25_SECTOR_AI ) * CUSTOMSECTOR , &uiNumBytesWritten ); //NUM_CAMPAIGN_JA25_SECTORS
	if( uiNumBytesWritten != sizeof( JA25_SECTOR_AI ) * CUSTOMSECTOR  ) //NUM_CAMPAIGN_JA25_SECTORS 
	{
		return( FALSE );
	}

	// OK, now write out additional sectors
	/*
	{
		INT32 iNumCustomSectors = giNumJA25Sectors - NUM_CAMPAIGN_JA25_SECTORS;

		// Write out # of additional sectors
		FileWrite( hFile, &iNumCustomSectors, sizeof( iNumCustomSectors ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( iNumCustomSectors ) )
		{
			return( FALSE );
		}
*/
/*		if ( iNumCustomSectors > 0 )
		{
			FileWrite( hFile, &(gJa25AiSectorStruct[ NUM_CAMPAIGN_JA25_SECTORS ] ), sizeof( JA25_SECTOR_AI ) * iNumCustomSectors, &uiNumBytesWritten );
			if( uiNumBytesWritten != sizeof( JA25_SECTOR_AI ) * iNumCustomSectors )
			{
				return( FALSE );
			}
		}
*/
//	}

	return( TRUE );
}


BOOLEAN LoadJa25SaveInfoFromSavedGame( HWFILE hFile )
{
	UINT32 uiNumBytesRead;

	// Free old data
//	if ( gJa25AiSectorStruct )
//	{
//		MemFree( gJa25AiSectorStruct );
//		gJa25AiSectorStruct = NULL;
//	}

	FileRead( hFile, &gJa25SaveStruct, sizeof( JA25_SAVE_INFO ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( JA25_SAVE_INFO ) )
	{
		return( FALSE );
	}

	//
	// Ai Manager
	//
	FileRead( hFile, &gJa25StrategicAi, sizeof( JA25_SECTOR_AI_MANAGER ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( JA25_SECTOR_AI_MANAGER ) )
	{
		return( FALSE );
	}

	//
	// Sector Info
	//
	
	// Init array for campaign sectors
//	giNumJA25Sectors = NUM_CAMPAIGN_JA25_SECTORS;
///	gJa25AiSectorStruct = MemAlloc( giNumJA25Sectors * sizeof( JA25_SECTOR_AI ) );

	FileRead( hFile, &gJa25AiSectorStruct, sizeof( JA25_SECTOR_AI ) * CUSTOMSECTOR , &uiNumBytesRead ); //NUM_CAMPAIGN_JA25_SECTORS
	if( uiNumBytesRead != sizeof( JA25_SECTOR_AI ) * CUSTOMSECTOR  )  //NUM_CAMPAIGN_JA25_SECTORS
	{
		return( FALSE );
	}

//	FileRead( hFile, gJa25AiSectorStruct, sizeof( JA25_SECTOR_AI ) * NUM_CAMPAIGN_JA25_SECTORS, &uiNumBytesRead );
//	if( uiNumBytesRead != sizeof( JA25_SECTOR_AI ) * NUM_CAMPAIGN_JA25_SECTORS )
//	{
//		return( FALSE );
//	}
/*
	if( guiSaveGameVersion < 1008 )
	{
		InitJa25StrategicSectorAI( FALSE );

		gJa25StrategicAi.uiTimeOfLastBattleInMinutes = GetWorldTotalMin( );
		gJa25StrategicAi.uiTimeOfLastUpdate = gJa25StrategicAi.uiTimeOfLastBattleInMinutes;

		SectorInfo[ SEC_I13 ].ubNumTroops = 0;
		FixEnemyCounterInSectorBug();
	}
*/
/*	if( guiSaveGameVersion < 1006 )
	{
		InitJohnKulbaInitialSector();
	}

	if( guiSaveGameVersion < 1010 )
	{
		gJa25SaveStruct.bNewMercProfileIDForSayingMorrisNote = -1;
	}

	if( guiSaveGameVersion < 1011 )
	{
		//Add the enemies to the new sector
		SetH11NumEnemiesInSector();
	}
*/
	//for some reason the J13 level wasnt set up right
//	if( guiSaveGameVersion < 1012 )
//	{
		//
		// J13:	Power Generator, First Level
		//
//		gJa25AiSectorStruct[ JA25_J13_1 ].iSectorID = SEC_J13;
//		gJa25AiSectorStruct[ JA25_J13_1 ].bSectorZ = 1;

		//
		// I13:	First Level of Mine
		//
//		gJa25AiSectorStruct[ JA25_I13_1 ].iSectorID = SEC_I13;
//		gJa25AiSectorStruct[ JA25_I13_1 ].bSectorZ = 1;
//	}


	// ATE: At end, we added additional AI info....
//	if ( guiSaveGameVersion > 1012 )
//	{
		// Read in ja25 AI information that is > # of custom maps....
	//	UINT32 iNumCustomSectors;

	//	FileRead( hFile, &iNumCustomSectors, sizeof( UINT32 ), &uiNumBytesRead );
	//	if( uiNumBytesRead != sizeof( UINT32 ) )
	//	{
	//		return( FALSE );
	//	}

//		giNumJA25Sectors = giNumJA25Sectors + iNumCustomSectors;

	//	if ( giNumJA25Sectors > 0 )
	//	{
			// if # > 0, realloc and read in
//			gJa25AiSectorStruct = realloc( gJa25AiSectorStruct, giNumJA25Sectors * sizeof( JA25_SECTOR_AI ) );
//			FileRead( hFile, &(gJa25AiSectorStruct[ NUM_CAMPAIGN_JA25_SECTORS ] ), sizeof( JA25_SECTOR_AI ) * iNumCustomSectors, &uiNumBytesRead );
//			if( uiNumBytesRead != sizeof( JA25_SECTOR_AI ) * iNumCustomSectors )
	//		{
	//			return( FALSE );
	//		}
	//	}
//	}

	gJa25SaveStruct.iPowerGenFanPositionSndID = -1;

	return( TRUE );
}

void InitJa25StrategicAiBloodcats( )
{
	INT8	bNumBloodCats=0;


	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			bNumBloodCats = 3;
			break;
		case DIF_LEVEL_MEDIUM:
			bNumBloodCats = 4;
			break;
		case DIF_LEVEL_HARD:
			bNumBloodCats = 5;
			break;
	}
	SetNumberOfJa25BloodCatsInSector( SEC_I10, bNumBloodCats, 5 );
}


void HandleAddingEnemiesToTunnelMaps()
{
	BOOLEAN fInCombat = ( gTacticalStatus.uiFlags & INCOMBAT );
	UINT8 ubNumEnemies=0;

	//if the player has been in the complex
	if( HaveMercsEverBeenInComplex() )
	{
		//dont add any enemies to the tunnel maps
		return;
	}

	//if the player IS NOT in either of the tunnel sectors
	if( !AreAnyPlayerMercsStillInSector( 14, 10, 1 ) &&
			!AreAnyPlayerMercsStillInSector( 14, 11, 1 ) )
	{
		AddEnemiesToJa25TunnelMaps();
	}

	//else if the player IS NOT in the 2nd sector but in the first sector
	else if( AreAnyPlayerMercsStillInSector( 14, 10, 1 ) &&
					 !AreAnyPlayerMercsStillInSector( 14, 11, 1 ) )
	{
		ubNumEnemies = NumEnemiesToAttackFirstTunnelSector( NULL, NULL, NULL, NULL, NULL );

		//Add enemies to the first sector
		HandleAddEnemiesToSectorPlayerIsntIn( JA25_J14_1, ubNumEnemies );

		//Remember to move the enemies to be in the entrance to the tunnel
		SetJa25GeneralFlag( JA_GF__MOVE_ENEMIES_TO_EDGE_IN_TUNNEL_1 );

		AddEnemiesToSecondTunnelSector();
	}

	//else if the player is in the second sector
	else if( AreAnyPlayerMercsStillInSector( 14, 11, 1 ) )
	{
		ubNumEnemies = NumEnemiesToAttackSecondTunnelSector( NULL, NULL, NULL, NULL, NULL );

		//Add enemies to the first sector
		HandleAddEnemiesToSectorPlayerIsntIn( JA25_K14_1, ubNumEnemies );

		//Remember to move the enemies to be in the entrance to the tunnel
		SetJa25GeneralFlag( JA_GF__MOVE_ENEMIES_TO_EDGE_IN_TUNNEL_2 );
	}
}

void AddEnemiesToJa25TunnelMaps()
{
	if( gJa25SaveStruct.ubHowPlayerGotThroughFan == PG__PLAYER_BLEW_UP_FAN_TO_GET_THROUGH )
	{
		//if the player blew up the fan, add a ton of enemies to the sector
		AddEnemiesToFirstTunnelSector();

		AddEnemiesToSecondTunnelSector();
	}
	else if( gJa25SaveStruct.ubHowPlayerGotThroughFan == PG__PLAYER_STOPPED_FAN_TO_GET_THROUGH )
	{
	}
}

void AddEnemiesToFirstTunnelSector()
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps=0;

	NumEnemiesToAttackFirstTunnelSector( &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumTanks, &ubNumJeeps );

	SetNumberJa25EnemiesInSector( 14, 10, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
}

void AddEnemiesToSecondTunnelSector()
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps=0;

	NumEnemiesToAttackSecondTunnelSector( &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumTanks, &ubNumJeeps );
	SetNumberJa25EnemiesInSector( 14, 11, 1, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
}

UINT8 NumEnemiesToAttackFirstTunnelSector( UINT8 *pAdmins, UINT8 *pTroops, UINT8 *pElites, UINT8 *pTanks, UINT8 *pJeeps )
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps=0;
	
	//if the player blew up the fan, add a ton of enemies to the sector
	//1st Tunnel Sector
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 6 + Random( 0 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 6 + Random( 0 );
			ubNumElites = 1 + Random( 0 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 4 + Random( 0 );
			ubNumElites = 4 + Random( 0 );
			break;
	}

	if( pAdmins )
		*pAdmins = ubNumAdmins;

	if( pTroops )
		*pTroops = ubNumTroops;

	if( pElites )
		*pElites = ubNumElites;

	if( pTanks )
		*pTanks = ubNumTanks;

	if( pJeeps )
		*pJeeps = ubNumJeeps;

	return( ubNumAdmins + ubNumTroops + ubNumElites + ubNumTanks + ubNumJeeps );
}

UINT8 NumEnemiesToAttackSecondTunnelSector( UINT8 *pAdmins, UINT8 *pTroops, UINT8 *pElites, UINT8 *pTanks, UINT8 *pJeeps )
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps=0;
	
	//if the player blew up the fan, add a ton of enemies to the sector
	//1st Tunnel Sector
	//2nd Tunnel Sector
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 7 + Random( 2 );
			ubNumElites = 0 + Random( 0 );
			break;
		case DIF_LEVEL_MEDIUM:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 6 + Random( 2 );
			ubNumElites = 2 + Random( 2 );
			break;
		case DIF_LEVEL_HARD:
			ubNumAdmins = 0 + Random( 0 );
			ubNumTroops = 6 + Random( 0 );
			ubNumElites = 4 + Random( 3 );
			break;
	}

	if( pAdmins )
		*pAdmins = ubNumAdmins;
	if( pTroops )
		*pTroops = ubNumTroops;
	if( pElites )
		*pElites = ubNumElites;
	if( pTanks )
		*pTanks = ubNumTanks;
	if( pJeeps )
		*pJeeps = ubNumJeeps;

	return( ubNumAdmins + ubNumTroops + ubNumElites + ubNumTanks + ubNumJeeps );
}



void SetJa25GeneralFlag( UINT32 uiFlagToSet )
{
	gJa25SaveStruct.uiJa25GeneralFlags |= uiFlagToSet;
}

void ClearJa25GeneralFlag( UINT32 uiFlagToClear )
{
	gJa25SaveStruct.uiJa25GeneralFlags &= ~uiFlagToClear;
}

BOOLEAN IsJa25GeneralFlagSet( UINT32 uiFlagToCheck )
{
	return( ( gJa25SaveStruct.uiJa25GeneralFlags & uiFlagToCheck ) != 0 );
}

BOOLEAN InitJa25StrategicSectorAI( BOOLEAN fReset )
{
	if( fReset )
	{
		//Clear out the structures first
		memset( &gJa25StrategicAi, 0, sizeof( JA25_SECTOR_AI_MANAGER ) );
/*
		MemFree( gJa25AiSectorStruct ); 
		giNumJA25Sectors = NUM_CAMPAIGN_JA25_SECTORS;
		gJa25AiSectorStruct = MemAlloc( sizeof( JA25_SAVE_INFO ) * giNumJA25Sectors );
		memset( gJa25AiSectorStruct, 0, sizeof( JA25_SAVE_INFO ) * giNumJA25Sectors );
*/		
	}

	//Init the sector values
	if( !InitJa25SectorAi() )
	{
		return( FALSE );
	}

	gJa25StrategicAi.uiTimeOfLastBattleInMinutes = /*STARTING_TIME*/ gGameExternalOptions.iGameStartingTime / 60 + gGameExternalOptions.iFirstArrivalDelay /*FIRST_ARRIVAL_DELAY*/ / 60;
	gJa25StrategicAi.uiTimeOfLastUpdate = gJa25StrategicAi.uiTimeOfLastBattleInMinutes;

	//The player owns the starting sector
	//gJa25AiSectorStruct[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fPlayerControlled = TRUE;

	SectorInfo[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fSurfaceWasEverPlayerControlled = TRUE;


	return( TRUE );
}


BOOLEAN InitJa25SectorAi()
{
UINT32 ubCnt;
	//
	// H7: Initial sector
	//
if ( gGameUBOptions.pJA2UB == TRUE )
{

	for( ubCnt=0; ubCnt<CUSTOMSECTOR; ubCnt++)
	{
		gJa25AiSectorStruct[ ubCnt ].iSectorID = -1 ;
		gJa25AiSectorStruct[ ubCnt ].bSectorZ = 0;
		gJa25AiSectorStruct[ ubCnt ].bBaseNumEnemies = 0;
		gJa25AiSectorStruct[ ubCnt ].bRandomNumEnemies = 4;
		gJa25AiSectorStruct[ ubCnt ].bProbabilityOfAttacking = 0;
		gJa25AiSectorStruct[ ubCnt ].bMaxProbabilityForAttackingSector = 60;
		gJa25AiSectorStruct[ ubCnt ].ubMinimumProbabiltyBeforeAttack = 0;
		gJa25AiSectorStruct[ ubCnt ].fAutoDirection = 0;
		gJa25AiSectorStruct[ ubCnt ].ubInsertionDirection =0;
		gJa25AiSectorStruct[ ubCnt ].sInsertionGridNo = 0;
		gJa25AiSectorStruct[ ubCnt ].fCustomSector = TRUE;
		gJa25AiSectorStruct[ ubCnt ].ubLoadingScreenID =  0;
		gJa25AiSectorStruct[ ubCnt ].fEnemyEnrouteToAttack = FALSE;
		gJa25AiSectorStruct[ ubCnt ].fPlayerControlled = FALSE;
		gJa25AiSectorStruct[ ubCnt ].fPlayerHasLiberatedSectorBefore = FALSE;
	}
	
	gJa25AiSectorStruct[ JA25_H7 ].iSectorID = SEC_H7;
	gJa25AiSectorStruct[ JA25_H7 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_H7 ].bBaseNumEnemies = 2;
	gJa25AiSectorStruct[ JA25_H7 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_H7 ].bProbabilityOfAttacking = 3;
	gJa25AiSectorStruct[ JA25_H7 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_H7 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// H8:
	//
	gJa25AiSectorStruct[ JA25_H8 ].iSectorID = SEC_H8;
	gJa25AiSectorStruct[ JA25_H8 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_H8 ].bBaseNumEnemies = 2;
	gJa25AiSectorStruct[ JA25_H8 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_H8 ].bProbabilityOfAttacking = 3;
	gJa25AiSectorStruct[ JA25_H8 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_H8 ].ubMinimumProbabiltyBeforeAttack = 5;

	//
	// H9:	Guard Post
	//
	gJa25AiSectorStruct[ JA25_H9 ].iSectorID = SEC_H9;
	gJa25AiSectorStruct[ JA25_H9 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_H9 ].bBaseNumEnemies = 4;
	gJa25AiSectorStruct[ JA25_H9 ].bRandomNumEnemies = 3;

	gJa25AiSectorStruct[ JA25_H9 ].bProbabilityOfAttacking = 1;
	gJa25AiSectorStruct[ JA25_H9 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_H9 ].ubMinimumProbabiltyBeforeAttack = 15;

	//
	// H10:	
	//
	gJa25AiSectorStruct[ JA25_H10 ].iSectorID = SEC_H10;
	gJa25AiSectorStruct[ JA25_H10 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_H10 ].bBaseNumEnemies = 2;
	gJa25AiSectorStruct[ JA25_H10 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_H10 ].bProbabilityOfAttacking = 7;
	gJa25AiSectorStruct[ JA25_H10 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_H10 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// I9:
	//
	gJa25AiSectorStruct[ JA25_I9 ].iSectorID = SEC_I9;
	gJa25AiSectorStruct[ JA25_I9 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_I9 ].bBaseNumEnemies = 2;
	gJa25AiSectorStruct[ JA25_I9 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_I9 ].bProbabilityOfAttacking = 7;
	gJa25AiSectorStruct[ JA25_I9 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_I9 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// I10:	Varrez, First Part
	//
	gJa25AiSectorStruct[ JA25_I10 ].iSectorID = SEC_I10;
	gJa25AiSectorStruct[ JA25_I10 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_I10 ].bBaseNumEnemies = 2;
	gJa25AiSectorStruct[ JA25_I10 ].bRandomNumEnemies = 3;

	gJa25AiSectorStruct[ JA25_I10 ].bProbabilityOfAttacking = 10;
	gJa25AiSectorStruct[ JA25_I10 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_I10 ].ubMinimumProbabiltyBeforeAttack = 0;
	
	//
	// I11:	Varrez, Second Part
	//
	gJa25AiSectorStruct[ JA25_I11 ].iSectorID = SEC_I11;
	gJa25AiSectorStruct[ JA25_I11 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_I11 ].bBaseNumEnemies = 0;
	gJa25AiSectorStruct[ JA25_I11 ].bRandomNumEnemies = 0;

	gJa25AiSectorStruct[ JA25_I11 ].bProbabilityOfAttacking = 0;
	gJa25AiSectorStruct[ JA25_I11 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_I11 ].ubMinimumProbabiltyBeforeAttack = 20;

	//
	// I12
	//
	gJa25AiSectorStruct[ JA25_I12 ].iSectorID = SEC_I12;
	gJa25AiSectorStruct[ JA25_I12 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_I12 ].bBaseNumEnemies = 3;
	gJa25AiSectorStruct[ JA25_I12 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_I12 ].bProbabilityOfAttacking = 3;
	gJa25AiSectorStruct[ JA25_I12 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_I12 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// I13:	Abandoned Mine
	//
	gJa25AiSectorStruct[ JA25_I13 ].iSectorID = SEC_I13;
	gJa25AiSectorStruct[ JA25_I13 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_I13 ].bBaseNumEnemies = 2;
	gJa25AiSectorStruct[ JA25_I13 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_I13 ].bProbabilityOfAttacking = 1;
	gJa25AiSectorStruct[ JA25_I13 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_I13 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// I13:	First Level of Mine
	//
	gJa25AiSectorStruct[ JA25_I13_1 ].iSectorID = SEC_I13;
	gJa25AiSectorStruct[ JA25_I13_1 ].bSectorZ = 1;

	gJa25AiSectorStruct[ JA25_I13_1 ].bBaseNumEnemies = 0;
	gJa25AiSectorStruct[ JA25_I13_1 ].bRandomNumEnemies = 0;

	gJa25AiSectorStruct[ JA25_I13_1 ].bProbabilityOfAttacking = 0;
	gJa25AiSectorStruct[ JA25_I13_1 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_I13_1 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// J11:	
	//
	gJa25AiSectorStruct[ JA25_J11 ].iSectorID = SEC_J11;
	gJa25AiSectorStruct[ JA25_J11 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_J11 ].bBaseNumEnemies = 3;
	gJa25AiSectorStruct[ JA25_J11 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_J11 ].bProbabilityOfAttacking = 3;
	gJa25AiSectorStruct[ JA25_J11 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_J11 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// J12:	
	//
	gJa25AiSectorStruct[ JA25_J12 ].iSectorID = SEC_J12;
	gJa25AiSectorStruct[ JA25_J12 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_J12 ].bBaseNumEnemies = 3;
	gJa25AiSectorStruct[ JA25_J12 ].bRandomNumEnemies = 2;

	gJa25AiSectorStruct[ JA25_J12 ].bProbabilityOfAttacking = 3;
	gJa25AiSectorStruct[ JA25_J12 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_J12 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// J13:	Power Generator
	//
	gJa25AiSectorStruct[ JA25_J13 ].iSectorID = SEC_J13;
	gJa25AiSectorStruct[ JA25_J13 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_J13 ].bBaseNumEnemies = 4;
	gJa25AiSectorStruct[ JA25_J13 ].bRandomNumEnemies = 3;

	gJa25AiSectorStruct[ JA25_J13 ].bProbabilityOfAttacking = 1;
	gJa25AiSectorStruct[ JA25_J13 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_J13 ].ubMinimumProbabiltyBeforeAttack = 15;

	//
	// J13:	Power Generator, First Level
	//
	gJa25AiSectorStruct[ JA25_J13_1 ].iSectorID = SEC_J13;
	gJa25AiSectorStruct[ JA25_J13_1 ].bSectorZ = 1;

	gJa25AiSectorStruct[ JA25_J13_1 ].bBaseNumEnemies = 0;
	gJa25AiSectorStruct[ JA25_J13_1 ].bRandomNumEnemies = 0;

	gJa25AiSectorStruct[ JA25_J13_1 ].bProbabilityOfAttacking = 0;
	gJa25AiSectorStruct[ JA25_J13_1 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_J13_1 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// J14:	Tunnel, First Sector
	//
	gJa25AiSectorStruct[ JA25_J14_1 ].iSectorID = SEC_J14;
	gJa25AiSectorStruct[ JA25_J14_1 ].bSectorZ = 1;

	gJa25AiSectorStruct[ JA25_J14_1 ].bBaseNumEnemies = 0;
	gJa25AiSectorStruct[ JA25_J14_1 ].bRandomNumEnemies = 0;

	gJa25AiSectorStruct[ JA25_J14_1 ].bProbabilityOfAttacking = 0;
	gJa25AiSectorStruct[ JA25_J14_1 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_J14_1 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// K14:  Tunnel, second part
	//
	gJa25AiSectorStruct[ JA25_K14_1 ].iSectorID = SEC_K14;
	gJa25AiSectorStruct[ JA25_K14_1 ].bSectorZ = 1;

	gJa25AiSectorStruct[ JA25_K14_1 ].bBaseNumEnemies = 0;
	gJa25AiSectorStruct[ JA25_K14_1 ].bRandomNumEnemies = 0;

	gJa25AiSectorStruct[ JA25_K14_1 ].bProbabilityOfAttacking = 0;
	gJa25AiSectorStruct[ JA25_K14_1 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_K14_1 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// K15:	Complex, Part 1, Level 0
	//
	gJa25AiSectorStruct[ JA25_K15 ].iSectorID = SEC_K15;
	gJa25AiSectorStruct[ JA25_K15 ].bSectorZ = 0;

	gJa25AiSectorStruct[ JA25_K15 ].bBaseNumEnemies = 6;
	gJa25AiSectorStruct[ JA25_K15 ].bRandomNumEnemies = 3;

	gJa25AiSectorStruct[ JA25_K15 ].bProbabilityOfAttacking = 2;
	gJa25AiSectorStruct[ JA25_K15 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_K15 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// K15:	Complex, Part 1, Level 1
	//
	gJa25AiSectorStruct[ JA25_K15_1 ].iSectorID = SEC_K15;
	gJa25AiSectorStruct[ JA25_K15_1 ].bSectorZ = 1;

	gJa25AiSectorStruct[ JA25_K15_1 ].bBaseNumEnemies = 5;
	gJa25AiSectorStruct[ JA25_K15_1 ].bRandomNumEnemies = 3;

	gJa25AiSectorStruct[ JA25_K15_1 ].bProbabilityOfAttacking = 2;
	gJa25AiSectorStruct[ JA25_K15_1 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_K15_1 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// K15:	Complex, Part 1, Level 2
	//
	gJa25AiSectorStruct[ JA25_K15_2 ].iSectorID = SEC_K15;
	gJa25AiSectorStruct[ JA25_K15_2 ].bSectorZ = 2;

	gJa25AiSectorStruct[ JA25_K15_2 ].bBaseNumEnemies = 6;
	gJa25AiSectorStruct[ JA25_K15_2 ].bRandomNumEnemies = 4;

	gJa25AiSectorStruct[ JA25_K15_2 ].bProbabilityOfAttacking = 2;
	gJa25AiSectorStruct[ JA25_K15_2 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_K15_2 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// L15:	Complex, Part 2, Level 2
	//
	gJa25AiSectorStruct[ JA25_L15_2 ].iSectorID = SEC_L15;
	gJa25AiSectorStruct[ JA25_L15_2 ].bSectorZ = 2;

	gJa25AiSectorStruct[ JA25_L15_2 ].bBaseNumEnemies = 5;
	gJa25AiSectorStruct[ JA25_L15_2 ].bRandomNumEnemies = 3;

	gJa25AiSectorStruct[ JA25_L15_2 ].bProbabilityOfAttacking = 1;
	gJa25AiSectorStruct[ JA25_L15_2 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_L15_2 ].ubMinimumProbabiltyBeforeAttack = 0;

	//
	// L15:	Complex, Part 2, Level 3
	//
	gJa25AiSectorStruct[ JA25_L15_3 ].iSectorID = SEC_L15;
	gJa25AiSectorStruct[ JA25_L15_3 ].bSectorZ = 3;

	gJa25AiSectorStruct[ JA25_L15_3 ].bBaseNumEnemies = 0;
	gJa25AiSectorStruct[ JA25_L15_3 ].bRandomNumEnemies = 0;

	gJa25AiSectorStruct[ JA25_L15_3 ].bProbabilityOfAttacking = 0;
	gJa25AiSectorStruct[ JA25_L15_3 ].bMaxProbabilityForAttackingSector = 60;
	gJa25AiSectorStruct[ JA25_L15_3 ].ubMinimumProbabiltyBeforeAttack = 0;
	
}
	
	return( TRUE );
};


void CopyJa25SectorAiData( JA25_SECTOR_AI *pDest, JA25_SECTOR_AI *pSource )
{
	//Copy the contents of the structures
	memcpy( pDest, pSource, sizeof( JA25_SECTOR_AI ) );
}



BOOLEAN CanJa25SaiAttack()
{
	UINT32 uiCurrentTime=0;

	#ifdef ENABLE_CUSTOM_MAP_INTERFACE
		//if the player is using a CUSTOM MAP SCENARIO disable the S.A.I.
		if( gJa25SaveStruct.fInCustomMap )
		{
			return( FALSE );
		}
	#endif


	uiCurrentTime = GetWorldTotalMin( );

	if( gTacticalStatus.uiFlags & INCOMBAT )
	{
		return( FALSE );
	}


	if( ( gJa25StrategicAi.uiTimeOfLastBattleInMinutes + JA25_MINIMUM_TIME_BETWEEN_ATTACKS ) > uiCurrentTime )
	{
		//get out
		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN Ja25_AreEnemiesAbleToAttackSector( INT16 iSectorID, INT8 bSectorZ )
{
	//Can the strategic ai attack
	if( !CanJa25SaiAttack() )
	{
		//get out
		return( FALSE );
	}

	//
	// All Checks Failed, therefore, enemies can attack
	//

	return( TRUE );
}


void Ja25_UpdateTimeOfEndOfLastBattle( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	//
	// Reset timers
	//
	gsGridNoForMapEdgePointInfo = -1;
	gJa25StrategicAi.uiTimeOfLastBattleInMinutes = GetWorldTotalMin( );

	gJa25StrategicAi.uiTimeOfLastUpdate = GetWorldTotalMin( );

	ResetJa25SectorProbabilities();

	gJa25SaveStruct.fEnemyShouldImmediatelySeekThePlayer = FALSE;
	gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy = -1;
	
}


void ResetJa25SectorProbabilities()
{
/*	INT8 bCnt;

	//loop through the array
	for( bCnt=0; bCnt< giNumJA25Sectors; bCnt++ )
	{
		//if the SAI did the attack ( the varible only gets set when SAI is attacking
		if( gJa25SaveStruct.fEnemyShouldImmediatelySeekThePlayer )
		{
			gJa25AiSectorStruct[ bCnt ].bCurrentProbability = gJa25AiSectorStruct[ bCnt ].bCurrentProbability / 2;
		}
		else
		{
			gJa25AiSectorStruct[ bCnt ].bCurrentProbability = 0;
		}
	}
*/
}


//returns the element into the array of which sector the enemy will attack
INT8	GetTheFurthestSectorPlayerOwns()
{
	INT8	bCnt;
	BOOLEAN	fFoundLatest=FALSE;
	INT8	bSector=-1;
	
if ( gGameUBOptions.pJA2UB == TRUE )
{
	//Loop through from the end of the list
	for( bCnt=giNumJA25Sectors-1; bCnt>=0; bCnt-- )
	{
		if( gJa25AiSectorStruct[ bCnt ].fPlayerControlled )
		{
			fFoundLatest = TRUE;
			break;
		}
	}

	if( fFoundLatest )
	{
		//do sector specific logic to determine the sector
		//switch( bCnt )
		//{
			//default:
				bSector = bCnt;
				//break;
		//}
	}
		
}
	//There is no sector
	return( bSector );
}


void SetJa25SectorOwnedStatus( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, BOOLEAN fPlayerOwned )
{
	INT8 bSector;

	if ( gGameUBOptions.pJA2UB == TRUE )
	{
	//get the sector id
	bSector = (INT8)GetJA25SectorID( sSectorX, sSectorY, bSectorZ );

	//if it failed to find this array
	if( bSector == -1 )
	{
		//if this is the new sector
		if( sSectorX == 11 && sSectorY == 8 &&  bSectorZ == 0 )
		{
			//If its the end of combat mode, update sai timers
			Ja25_UpdateTimeOfEndOfLastBattle( sSectorX, sSectorY, bSectorZ );
		}
		else
		{
			//If its the end of combat mode, update sai timers
			Ja25_UpdateTimeOfEndOfLastBattle( sSectorX, sSectorY, bSectorZ );
			return;
		}
		return;
	}

	if( NumEnemiesInAnySector( sSectorX, sSectorY, bSectorZ ) != 0 )
	{
		return;
	}

	gJa25AiSectorStruct[ bSector ].fPlayerControlled = fPlayerOwned;

	//if its the players that is doing the taking over
	if( fPlayerOwned )
	{
		//Remeber that the player has Liberated this sector before
		gJa25AiSectorStruct[ bSector ].fPlayerHasLiberatedSectorBefore = TRUE;
	}

	//If its the end of combat mode, update sai timers
	Ja25_UpdateTimeOfEndOfLastBattle( sSectorX, sSectorY, bSectorZ );

	//if this is sector H8
	if( sSectorX == 8 && sSectorY == MAP_ROW_H && bSectorZ == 0 )
	{		
		AddStrategicEvent( EVENT_SECTOR_H8_DONT_WAIT_IN_SECTOR, GetWorldTotalMin() + ( 5 * 60 ), 0 );
	}
	
	}

}


INT16 GetJA25SectorID( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT8 bCnt=0;
	INT16	sSector = 0;
	
if ( gGameUBOptions.pJA2UB == TRUE )
{

	if ( gJa25AiSectorStruct == NULL )
	{
		return( -1 );
	}

	if( sSectorX == -1 || sSectorY == -1 )
	{
		return( -1 );
	}

	sSector = SECTOR( sSectorX, sSectorY );

	//loop through the array
	for( bCnt=0; bCnt< giNumJA25Sectors; bCnt++ )
	{
		//if this is the right sector
		if( gJa25AiSectorStruct[ bCnt ].iSectorID == sSector && gJa25AiSectorStruct[ bCnt ].bSectorZ == bSectorZ )
		{
			//return the array element
			return( bCnt );
		}
	}
}
	//failed to find the array
	return( -1 );
}


BOOLEAN	GetSectorJa25SaiWillAttack( INT8 *pbSectorToAttack, INT8 *pbFromDirection, INT16 *psGridNo )
{
	INT8	bSectorToAttackID=-1;
	INT8	bFurthestPlayerSector=-1;
	INT8	bAttackDirection=-1;
	INT16	sGridNo=-1;

	*pbSectorToAttack = -1;
	*pbFromDirection = -1;
	*psGridNo = -1;

	return( TRUE );
}



void JA25_HandleUpdateOfStrategicAi()
{
	UINT32 uiCurrentTime=0;
	INT8	bSectorToAttackID=-1;
	INT8	bAttackDirection=-1;
	INT8	bRandom=0;
	INT16	sGridNo=-1;

if ( gGameUBOptions.pJA2UB == TRUE )
{
	if ( !gJa25AiSectorStruct )
	{
		return;
	}

	uiCurrentTime = GetWorldTotalMin( );

	//if the ai cannot currently attack
	if( !CanJa25SaiAttack() )
	{
		return;
	}

	//if it isnt time to update
	if( ( uiCurrentTime - gJa25StrategicAi.uiTimeOfLastUpdate ) < GetMinimumTimeBetweenAttacks() )
	{
		return;
	}

	//Remeber that we are updating
	gJa25StrategicAi.uiTimeOfLastUpdate = uiCurrentTime;


//	gJa25StrategicAi.uiTimeOfLastBattleInMinutes

	//Get the sector the enemies will attack from
	if( !GetSectorJa25SaiWillAttack( &bSectorToAttackID, &bAttackDirection, &sGridNo ) )
	{
		return;
	}

	//Increment the probabilty of attacking
	gJa25AiSectorStruct[ bSectorToAttackID ].bCurrentProbability += gJa25AiSectorStruct[ bSectorToAttackID ].bProbabilityOfAttacking;

	//if we are above the maximum amount
	if( gJa25AiSectorStruct[ bSectorToAttackID ].bCurrentProbability > gJa25AiSectorStruct[ bSectorToAttackID ].bMaxProbabilityForAttackingSector )
	{
		gJa25AiSectorStruct[ bSectorToAttackID ].bCurrentProbability = gJa25AiSectorStruct[ bSectorToAttackID ].bMaxProbabilityForAttackingSector;
	}

	//Debug Check to see if the date is valid
#ifdef JA2BETAVERSION
//	if( !Ja25BetaDateToInvalidateExe() )
//	{
//		Assert( 0 );
//	}
#endif

	//Check wether the minimum prbability is above the current probabilty
	if( gJa25AiSectorStruct[ bSectorToAttackID ].ubMinimumProbabiltyBeforeAttack >= gJa25AiSectorStruct[ bSectorToAttackID ].bCurrentProbability )
	{
		//if so leave
		return;
	}


	//Get random #
	bRandom = Random( 120 );

	//if the enemies should attack
	if( bRandom < gJa25AiSectorStruct[ bSectorToAttackID ].bCurrentProbability )
	{
		INT16		sSectorToAttack;
		INT16		sSector;
		UINT8		ubNumEnemies=0;
		UINT8		ubSectorX = SECTORX( gJa25AiSectorStruct[ bSectorToAttackID ].iSectorID );
		UINT8		ubSectorY = SECTORY( gJa25AiSectorStruct[ bSectorToAttackID ].iSectorID );
		INT8		bSectorZ = gJa25AiSectorStruct[ bSectorToAttackID ].bSectorZ;
		UINT32	uiBaseNumEnemies=0;
		UINT32	uiRandomNumEnemies=0;


		//
		// The AI has decided to attack
		//

		// return if we have send guys already
		if ( gJa25AiSectorStruct[ bSectorToAttackID ].fEnemyEnrouteToAttack )
		{
			return;
		}

		//reset the current probability
		gJa25AiSectorStruct[ bSectorToAttackID ].bCurrentProbability = 0;

		//if the AI is attacking the same sector
		if( gJa25StrategicAi.iLastBattleSectorID == bSectorToAttackID )
		{
			gJa25StrategicAi.uiAttackedSameSectorCounter += 1;
		}
		else
		{
			//Other wise, reset the counter
			gJa25StrategicAi.uiAttackedSameSectorCounter = 0;
		}

		gJa25StrategicAi.iLastBattleSectorID = bSectorToAttackID;

		//if all the player mercs are between sector
		if( AreAllPlayerMercTraversingBetweenSectors() )
		{
			//then the ai shouldnt attack
			return;
		}

		sSectorToAttack = gJa25AiSectorStruct[ bSectorToAttackID ].iSectorID;

		sSector = sSectorToAttack;

		uiBaseNumEnemies = gJa25AiSectorStruct[ bSectorToAttackID ].bBaseNumEnemies + gJa25StrategicAi.uiAttackedSameSectorCounter;
		uiRandomNumEnemies = gJa25AiSectorStruct[ bSectorToAttackID ].bRandomNumEnemies + ( gJa25StrategicAi.uiAttackedSameSectorCounter * 2 );
		ubNumEnemies = uiBaseNumEnemies + Random( uiRandomNumEnemies );

		//Guarentee no more then 20 enemies in a sector
		if( ubNumEnemies > gGameExternalOptions.iMaxEnemyGroupSize ) //MAX_STRATEGIC_TEAM_SIZE )
		{
			ubNumEnemies = gGameExternalOptions.iMaxEnemyGroupSize; //MAX_STRATEGIC_TEAM_SIZE;
		}

		//if there are enemies to attack
		if( ubNumEnemies )
		{
			//if the player is NOT in this sector
			if( PlayerMercsInSector( ubSectorX, ubSectorY, bSectorZ ) )
			{
				//Setup an attack
				HandleAddingEnemiesToSector( bSectorToAttackID, ubNumEnemies, bAttackDirection, sGridNo );
			}
			else
			{
				HandleAddEnemiesToSectorPlayerIsntIn( bSectorToAttackID, ubNumEnemies );
			}

			#ifdef JA2BETAVERSION
				RecordJa25StrategicAiDecisions( bSectorToAttackID, ubNumEnemies );
			#endif

		}
	}

}

}

BOOLEAN HandleAddEnemiesToSectorPlayerIsntIn( INT16 sSaiSector, UINT8 ubNumEnemies )
{	
	if ( gGameUBOptions.pJA2UB == TRUE )
	{
		INT16 sSectorX = SECTORX( gJa25AiSectorStruct[sSaiSector].iSectorID );
		INT16 sSectorY = SECTORY( gJa25AiSectorStruct[sSaiSector].iSectorID );
		INT8  bSectorZ = gJa25AiSectorStruct[sSaiSector].bSectorZ;

		if( !( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			SetEnemyEncounterCode( ENEMY_INVASION_CODE );
		}

		SetNumberJa25EnemiesInSector( sSectorX, sSectorY, bSectorZ, 0, ubNumEnemies, 0, 0, 0 );

		SetThisSectorAsEnemyControlled( sSectorX, sSectorY, bSectorZ, FALSE );

		//Set up flag so enemies will go and find the player in that sector
		gJa25SaveStruct.fEnemyShouldImmediatelySeekThePlayer = TRUE;
		gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy = (INT8)sSaiSector;
	}

	return( TRUE );
}

BOOLEAN HandleAddingEnemiesToSector( INT16 sSaiSector, UINT8 ubNumEnemies, INT8 bAttackDirection, INT16 sGridNo )
{
	GROUP		*pGroup=NULL;
	INT16		sSectorToAttack;
	INT16		sSector;
	UINT32	uiWorldMin=0;
	INT8		bLevel=0;
if ( gGameUBOptions.pJA2UB == TRUE )
{
	sSectorToAttack = gJa25AiSectorStruct[ sSaiSector ].iSectorID;

	sSector = sSectorToAttack;

	switch( bAttackDirection )
	{
		case INSERTION_CODE_NORTH:
			sSector -= 16;
			break;
		case INSERTION_CODE_SOUTH:
			sSector += 16;
			break;
		case INSERTION_CODE_WEST:
			sSector -= 1;
			break;
		case INSERTION_CODE_EAST:
			sSector += 1;
			break;
	}


	uiWorldMin = GetWorldTotalMin();

	if( gJa25AiSectorStruct[ sSaiSector ].bSectorZ == 0 )
	{
		pGroup = CreateNewEnemyGroupDepartingFromSector( sSector, 0, ubNumEnemies, 0, 0, 0 );

		if( sGridNo != -1 )
		{
			gsGridNoForMapEdgePointInfo = sGridNo;

			bAttackDirection = INSERTION_CODE_GRIDNO;

			gfCantRetreatInPBI = TRUE;
		}
	}
	else
	{
		//Set the gridno the enemies will land at
		if( sGridNo == -1 )
		{
			Assert( 0 );
			return( FALSE );
		}

		//Remeber the gridno the enemies should arrive at
		gsGridNoForMapEdgePointInfo = sGridNo;

		//the enemies should arrive at a spcific gridno
		bAttackDirection = INSERTION_CODE_GRIDNO;
		
		//Determine which level the enemies should be created at
		Ja25SAI_DetermineWhichLevelToAttackFrom( sSaiSector, &sSector, &bLevel );

		//underground, we have to specify
		pGroup = CreateNewEnemyGroupDepartingFromSectorUsingZLevel( sSector, bLevel, 0, ubNumEnemies, 0, 0 );//was sSector

//		pGroup->pEnemyGroup->ubTroopsInBattle = ubNumEnemies;
	}


//	pGroup->ubSectorX = (UINT8)SECTORX(sSectorToAttack);
//	pGroup->ubSectorY = (UINT8)SECTORY(sSectorToAttack);
//	pGroup->ubSectorZ = gJa25AiSectorStruct[ sSaiSector ].bSectorZ;

	pGroup->ubNextX = (UINT8)SECTORX(sSectorToAttack);
	pGroup->ubNextY = (UINT8)SECTORY(sSectorToAttack);

	pGroup->uiTraverseTime = 1;
	pGroup->pEnemyGroup->ubIntention = ASSAULT;
	SetGroupArrivalTime( pGroup, uiWorldMin + 1 );
	pGroup->ubMoveType = ONE_WAY;

	SetEnemyEncounterCode( ENEMY_INVASION_CODE );

	if( gJa25AiSectorStruct[ sSaiSector ].bSectorZ == 0 )
	{
		AddStrategicEvent( EVENT_GROUP_ARRIVAL, pGroup->uiArrivalTime, pGroup->ubGroupID );

		// Set vairable telling us to not send any more!
		gJa25AiSectorStruct[ sSaiSector ].fEnemyEnrouteToAttack = TRUE;
	}
	else
	{
//		AddEnemiesToBattle( pGroup, bAttackDirection, 0, ubNumEnemies, 0, TRUE );

		pGroup->ubSectorX = (UINT8)SECTORX(sSectorToAttack);
		pGroup->ubSectorY = (UINT8)SECTORY(sSectorToAttack);
		pGroup->ubSectorZ = gJa25AiSectorStruct[ sSaiSector ].bSectorZ;

		pGroup->ubPrevX = pGroup->ubSectorX;
		pGroup->ubPrevY = pGroup->ubSectorY;
		pGroup->ubSectorX = pGroup->ubNextX;
		pGroup->ubSectorY = pGroup->ubNextY;
		pGroup->ubNextX = 0;
		pGroup->ubNextY = 0;


		InitPreBattleInterface( pGroup, TRUE );
	}

	//Set up flag so enemies will go and find the player in that sector
	gJa25SaveStruct.fEnemyShouldImmediatelySeekThePlayer = TRUE;
	gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy = (INT8)sSaiSector;
}
	return( TRUE );
}

void SetEnemiesToFindThePlayerMercs()
{
	INT32 cnt;
	SOLDIERTYPE             *pSoldier;
	INT16	sGridNoToGoto=0;

if ( gGameUBOptions.pJA2UB == TRUE )
{
	sGridNoToGoto = GetGridNoEnemyWillSeekWhenAttacking( gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy );

	//Reset fact that we send enemy to find player
	gJa25SaveStruct.fEnemyShouldImmediatelySeekThePlayer = FALSE;
	gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy = -1;

	//if its not a valid gridno
	if( sGridNoToGoto == 0 )
	{
		Assert( 0 );
		return;
	}

	cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
  for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++, pSoldier++)
	{	
		if( pSoldier->stats.bLife >= OKLIFE && pSoldier->bActive && pSoldier->bInSector )
		{
			// send soldier to centre of map, roughly
			pSoldier->aiData.sNoiseGridno = sGridNoToGoto;
			pSoldier->bNoiseLevel = 0;
			pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION / 2;

			pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
			pSoldier->flags.bHasKeys = TRUE;

			pSoldier->aiData.bOrders = SEEKENEMY;
		}
	}
}
}

INT16 GetGridNoEnemyWillSeekWhenAttacking( INT8 bSaiSector )
{
	INT16	sGridNo=0;
if ( gGameUBOptions.pJA2UB == TRUE )
{
	switch( bSaiSector )
	{
		case JA25_J14_1:
			sGridNo = 19749;
			break;

		case JA25_K14_1:
			sGridNo = 5978;
			break;

		default:

			//if for some reason the map doesnt have a center gridno
			if( gMapInformation.sCenterGridNo != -1 && gMapInformation.sCenterGridNo != 0 )
			{
				sGridNo = gMapInformation.sCenterGridNo;
			}
			else
			{
				INT8 bID = RandomSoldierIdForAnyMercInSector();

				if( bID != -1 )
				{
					sGridNo = Menptr[ bID ].sGridNo;
				}
			}

			break;
	}	
}
	return( sGridNo );
}

void Ja25SAI_DetermineWhichLevelToAttackFrom( INT16 sSaiSector, INT16 *psSector, INT8 *pbLevel )
{
	INT8	bLevel=0;
	INT16 sSector=0;
if ( gGameUBOptions.pJA2UB == TRUE )
{
	switch( sSaiSector )
	{
		case JA25_K15_1:

			sSector = SEC_K15;
			bLevel = 2;
			break;

		case JA25_K15_2:

			sSector = SEC_L15;
			bLevel = 2;
			break;

		case JA25_L15_2:

			sSector = SEC_L15;
			bLevel = 3;
			break;

		default:
			sSector = gJa25AiSectorStruct[ sSaiSector ].iSectorID;
			bLevel = gJa25AiSectorStruct[ sSaiSector ].bSectorZ;
			break;
	}	

	*psSector = sSector;
	*pbLevel = bLevel;

}
}

BOOLEAN AreAllPlayerMercTraversingBetweenSectors()
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier=NULL;
	GROUP *pGroup=0;

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
  for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++, pSoldier++)
	{	
		if( pSoldier->bActive )
		{
			pGroup = GetGroup( pSoldier->ubGroupID );

			if( pGroup != NULL )
			{
				//if the group is NOT between sector
				if( pGroup->fBetweenSectors == FALSE )
				{					
					return( FALSE );
				}
			}
		}
	}

	return( TRUE );
}


#ifdef JA2BETAVERSION

BOOLEAN RecordJa25StrategicAiDecisions( INT16 sSectorAttacked, UINT8 ubNumEnemies )
{
	/*
	CHAR8			zOutputString[512];
	HWFILE		hFile;

	//open up the debug file
	hFile = FileOpen( JA25_SAI_DEBUG_FILE, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return( FALSE );
	}

	//move the end of the file
	FileSeek( hFile, 0, FILE_SEEK_FROM_END );

	OutputJA25SaiString( hFile, "\n\n---  Strategic Ai Attack ---\n" );

	//Sector
	sprintf( zOutputString, "Sector Attacked:         %c%d: Level %d\n", 
															'A' + SECTORY( gJa25AiSectorStruct[ sSectorAttacked ].iSectorID ) - 1,
															SECTORX( gJa25AiSectorStruct[ sSectorAttacked ].iSectorID ),
															gJa25AiSectorStruct[ sSectorAttacked ].bSectorZ );
	OutputJA25SaiString( hFile, zOutputString );

	//Num enemies
	sprintf( zOutputString, "Num Enemies:             %d\n", ubNumEnemies );
	OutputJA25SaiString( hFile, zOutputString );

	//Time since last attack
	sprintf( zOutputString, "Time Since Last Attack:  %d hours\n", ( GetWorldTotalMin() - gJa25StrategicAi.uiTimeOfLastBattleInMinutes ) / 60  );
	OutputJA25SaiString( hFile, zOutputString );

	//Probability
	sprintf( zOutputString, "Probability of Attack:   %d/%d\n", gJa25AiSectorStruct[ sSectorAttacked ].bProbabilityOfAttacking, gJa25AiSectorStruct[ sSectorAttacked ].bCurrentProbability  );
	OutputJA25SaiString( hFile, zOutputString );

	FileClose( hFile );
*/
	return( TRUE );
}

BOOLEAN InitJa25StrategicAiDecisions( BOOLEAN fLoadedGame )
{
	HWFILE		hFile;

	//open up the debug file
	hFile = FileOpen( JA25_SAI_DEBUG_FILE, FILE_ACCESS_WRITE | FILE_OPEN_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return( FALSE );
	}

	if( fLoadedGame )
		OutputJA25SaiString( hFile, "\n\n\n\n\n\n****** Loaded Game ******\n\n");
	else
		OutputJA25SaiString( hFile, "\n\n\n\n\n\n****** New Game ******\n\n");

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			OutputJA25SaiString( hFile, "Easy Difficulty\n");
			break;
		case DIF_LEVEL_MEDIUM:
			OutputJA25SaiString( hFile, "Normal Difficulty\n");
			break;
		case DIF_LEVEL_HARD:
			OutputJA25SaiString( hFile, "Hard Difficulty\n");
			break;
	}

	OutputJA25SaiString( hFile, "\n\n\n");

	FileClose( hFile );

	return( FALSE );
}


BOOLEAN OutputJA25SaiString( HWFILE hFile, CHAR *pString )
{
	UINT32 uiNumBytesWritten;
	UINT32 uiStrLen;

	uiStrLen = strlen( pString );

	FileWrite( hFile, pString, uiStrLen, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiStrLen )
	{
		return( FALSE );
	}

	return( TRUE );
}

#endif

UINT32 GetMinimumTimeBetweenAttacks()
{
	UINT32 uiTime=0;

	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			uiTime = JA25_TIME_BETWEEN_SAI_UPDATES_EASY;
			break;
		case DIF_LEVEL_MEDIUM:
			uiTime = JA25_TIME_BETWEEN_SAI_UPDATES_NORMAL;
			break;
		case DIF_LEVEL_HARD:
			uiTime = JA25_TIME_BETWEEN_SAI_UPDATES_HARD;
			break;
	}

	//Add a random time to calculation
	uiTime += Random( JA25_SAI_RANDOM_TIME );

	return( uiTime );	
}

void InitJohnKulbaInitialSector()
{
	gJa25SaveStruct.ubJohnKulbaInitialSectorY = MAP_ROW_I;

	//Randomly choose John's starting sector
	if( Random( 100 ) < 50 )
	{
		gJa25SaveStruct.ubJohnKulbaInitialSectorX = 10;
	}
	else
	{
		gJa25SaveStruct.ubJohnKulbaInitialSectorX = 11;
	}
}

void HandleSayingDontStayToLongWarningInSectorH8()
{
	UINT8			ubMercArray[3];
	UINT8			ubNumMercs=0;
	SOLDIERTYPE *pSoldier=NULL;
	INT32			cnt;

	//if the player has advance passed this sector, leave
	
	if( gJa25AiSectorStruct[ JA25_H9 ].fPlayerControlled && gGameUBOptions.pJA2UB == TRUE )
	{
		return;
	}
	
	if ( SectorInfo[ (UINT8)SECTOR( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY ) ].fSurfaceWasEverPlayerControlled == TRUE && gGameUBOptions.pJA2UB == FALSE )
	{
		return;
	} 
	
	//if there are no enemies in the sector, leave
	if ( NumNonPlayerTeamMembersInSector( 8, 8, ENEMY_TEAM ) > 0 )
	{
		return;
	}

	//if all the mercs are traversing, leave
	if( AreAllPlayerMercTraversingBetweenSectors() )
	{
		return;
	}

	//if there are no mercs still in H8, leave
	if( PlayerMercsInSector( 8, 8, 0 ) == 0 )
	{
		return;
	}

	cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

	//Check to see if Gaston, Stogie or the PGC is on the team
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++,pSoldier++)
	{    
		//if the merc is alive, in this sector, etc...
		if( pSoldier->bActive	&&
				pSoldier->sSectorX == 8 &&
				pSoldier->sSectorY == 8 &&
				pSoldier->bSectorZ == 0 &&
				pSoldier->stats.bLife >= OKLIFE &&
				!pSoldier->flags.fBetweenSectors )
		{
			//if the merc is either PGC, gaston, or stogie
			if( pSoldier->ubProfile == GASTON_UB ||
					pSoldier->ubProfile == STOGIE_UB ||
					pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER )

			{
				ubMercArray[ ubNumMercs++ ] = pSoldier->ubID;
			}
		}
	}

	if( ubNumMercs == 0 )
	{
		return;
	}

	TacticalCharacterDialogue( &Menptr[ ubMercArray[ Random( ubNumMercs ) ] ], QUOTE_LAME_REFUSAL );
}

void FixEnemyCounterInSectorBug()
{
	INT32 iSectorCnt;
	SECTORINFO *pSector;

	//loop through all the sector and see if there has been 
	for( iSectorCnt=0; iSectorCnt<256 ; iSectorCnt++ )
	{
		//if the sector can never be rreached
		pSector = &SectorInfo[ iSectorCnt ];

		if( pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] == EDGEOFWORLD )
		{
			pSector->ubNumAdmins = 0;
			pSector->ubNumTroops = 0;
			pSector->ubNumElites = 0;
		}
	}

	pSector = &SectorInfo[ SEC_L15 ];
	pSector->ubNumAdmins = 0;
	pSector->ubNumTroops = 0;
	pSector->ubNumElites = 0;

}


void HandleEnricosUnderstandingEmail()
{
	//if the player just got the laptop working again, and we are to send an email when entering a new sector
	if( gJa25SaveStruct.fSendEmail_10_NextSector )
	{
		AddEmail( EMAIL_UNDERSTANDING, EMAIL_UNDERSTANDING_LENGTH, MAIL_ENRICO,  GetWorldTotalMin() , -1 , -1, TYPE_EMAIL_EMAIL_EDT);

		//Remeber we sent it
		gJa25SaveStruct.ubEmailFromSectorFlag |= SECTOR_EMAIL__ANOTHER_SECTOR;
		gJa25SaveStruct.fSendEmail_10_NextSector = FALSE;
	}
}


void HandleRemovingEnemySoldierInitLinksIfPlayerEverWonInSector()
{
	INT16	sJa25SaiSectorValue;

if ( gGameUBOptions.pJA2UB == TRUE )
{
	//Get the Ja25 SAI sector value for the sector that is being attacked
	sJa25SaiSectorValue = GetJA25SectorID( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//if the player has WON in this sector before, we must destroy the ENEMIES soldier init list links
	if( sJa25SaiSectorValue != -1 && gJa25AiSectorStruct[ sJa25SaiSectorValue ].fPlayerHasLiberatedSectorBefore )
	{
		//Toast the ENEMIES init links
		RemoveAllEnemySoldierInitListLinks();
	}
}	
}

void RemoveAllEnemySoldierInitListLinks()
{
	SOLDIERINITNODE *curr;

	curr = gSoldierInitHead;

	//Loop through the list
	while( curr )
	{
		// if this soldier is using detailed placement information, AND he is an enemy
		if( curr->pDetailedPlacement && curr->pDetailedPlacement->bTeam == ENEMY_TEAM )
		{ 
			MemFree( curr->pDetailedPlacement );
			curr->pDetailedPlacement = NULL;
			curr->pBasicPlacement->fDetailedPlacement = FALSE;
		}
		curr = curr->next;
	}
}

void Ja25HandleStartingAnyBattlesInOtherSectors()
{
	UINT8 ubNumEnemies=0;
	UINT8 ubNumPlayers=0;
	INT8 bCnt;
	INT8	bSectorX, bSectorY, bSectorZ;
	
if  ( gGameUBOptions.pJA2UB == TRUE )
{

	//if there is a world loaded
	if( gWorldSectorX != -1 && gWorldSectorY != -1 && gbWorldSectorZ != -1 )
	{
		ubNumEnemies = NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

		//if there is enemies in this sector
		if( ubNumEnemies != 0 )
		{
			//get out
			return;
		}
	}

	//loop through all the sectors and see if there any that have both enemy and player team mercs
	for( bCnt=0; bCnt< giNumJA25Sectors; bCnt++ )
	{
		bSectorX = SECTORX( gJa25AiSectorStruct[ bCnt ].iSectorID );
		bSectorY = SECTORX( gJa25AiSectorStruct[ bCnt ].iSectorID );
		bSectorZ = gJa25AiSectorStruct[ bCnt ].bSectorZ;

		ubNumEnemies = NumEnemiesInAnySector( bSectorX, bSectorY, bSectorZ );
		ubNumPlayers = PlayerMercsInSector( bSectorX, bSectorY, bSectorZ );

		//if there are both enemies and player mercs in the same sector
		if( ubNumEnemies != 0 && ubNumPlayers != 0 )
		{
//			GROUP *pEnemyGroup;
//			GROUP *pPlayerGroup;

			//we must start a battle
//			PrepareForPreBattleInterface( GROUP *pPlayerDialogGroup, GROUP *pInitiatingBattleGroup );
			return;
		}
	}
}
}

// -1 if enemy isnt supposed to seek player, otherwise SAI sectgor ID get returned
INT8 GetSectorEnemyIsToImmediatelySeekEnemyIn()
{
	INT16	sJa25SaiSectorValue;

	//if the currently loaded sector isnt the sector which the enemies are to attack
	sJa25SaiSectorValue = GetJA25SectorID( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	if( sJa25SaiSectorValue == -1 || sJa25SaiSectorValue != gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy )
	{
		return( -1 );
	}

	//if the enemy is to seek the player when entering the sector
	if( gJa25SaveStruct.fEnemyShouldImmediatelySeekThePlayer )
	{
		return( gJa25SaveStruct.bSectorTheEnemyWillSeekEnemy );
	}
	else
	{
		return( -1 );
	}
}


void SetH11NumEnemiesInSector()
{
	UINT8	ubNumAdmins=0;
	UINT8	ubNumTroops=0;
	UINT8	ubNumElites=0;
	UINT8	ubNumTanks=0;
	UINT8	ubNumJeeps = 0;

	// SEC_H11:
	{
		switch( gGameOptions.ubDifficultyLevel )
		{
			case DIF_LEVEL_EASY:
				ubNumAdmins = 1 + Random( 2 );
				ubNumTroops = 6 + Random( 3 );
				ubNumElites = 0 + Random( 0 );
				break;
			case DIF_LEVEL_MEDIUM:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 8 + Random( 3 );
				ubNumElites = 2 + Random( 2 );
				break;
			case DIF_LEVEL_HARD:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 10 + Random( 4 );
				ubNumElites = 4 + Random( 3 );
				break;
			case DIF_LEVEL_INSANE:
				ubNumAdmins = 0 + Random( 0 );
				ubNumTroops = 20 + Random( 4 );
				ubNumElites = 4 + Random( 3 );
				break;
		}

		SetNumberJa25EnemiesInSurfaceSector( SEC_H11, ubNumAdmins, ubNumTroops, ubNumElites, ubNumTanks, ubNumJeeps );
	}
}

BOOLEAN	HaveMercsEverBeenInComplex()
{

if (gGameUBOptions.pJA2UB == TRUE )
{
	//if the player has liberated K15 level 1
	if( gJa25AiSectorStruct[ JA25_K15_1 ].fPlayerHasLiberatedSectorBefore )
	{
		return( TRUE );
	}
	else if( AreAnyPlayerMercsStillInSector( 15, 11, 1 ) )
	{
		return( TRUE );
	}
}	
else if (gGameUBOptions.pJA2UB == FALSE )
{
	//if the player has liberated K15 level 1
	if( SectorInfo[ SEC_K15 ].uiTimeLastPlayerLiberated && gbWorldSectorZ == 1 )
	{
		return( TRUE );
	}
	else if( AreAnyPlayerMercsStillInSector( 15, 11, 1 ) )
	{
		return( TRUE );
	}
	
}

	return( FALSE );
}

#ifdef JA2BETAVERSION
BOOLEAN Ja25BetaDateToInvalidateExe()
{
	SYSTEMTIME sSystemTime;

	GetSystemTime( &sSystemTime );


	//if according to the system clock, we are past May 12, 2000, quit the game
	if( sSystemTime.wYear >= 2000 && sSystemTime.wMonth >= 5 && sSystemTime.wDay >= 12 )
	{
		return( FALSE );
	}

	return( TRUE );
}
#endif


void AddJA25AIDataToSector( JA25_SECTOR_AI *pSectorAIInfo )
{
	// See if there exists AI information here already...
/*	INT8 bSector;

	//get the sector id
	bSector = (INT8)GetJA25SectorID( (INT16)SECTORX( pSectorAIInfo->iSectorID ), (INT16)SECTORY( pSectorAIInfo->iSectorID ), pSectorAIInfo->bSectorZ );

	if ( bSector != -1 )
	{
		memcpy( &( gJa25AiSectorStruct[ bSector ] ), pSectorAIInfo, sizeof( JA25_SECTOR_AI ) );
		return;
	}

	// Increment array
	gJa25AiSectorStruct = realloc( gJa25AiSectorStruct, ( giNumJA25Sectors + 1 ) * sizeof( JA25_SECTOR_AI ) );

	memcpy( &( gJa25AiSectorStruct[ giNumJA25Sectors ] ), pSectorAIInfo, sizeof( JA25_SECTOR_AI ) );

	giNumJA25Sectors++;
*/
}

#endif