//Queen Command.c

#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "Queen Command.h"
	#include "Strategic Event Handler.h"
	#include "Overhead Types.h"
	#include "strategicmap.h"
	#include "Soldier Init List.h"
	#include "debug.h"
	#include "Random.h"
	#include "Strategic Movement.h"
	#include "Overhead.h"
	#include "Strategic Pathing.h"
	#include "strategic.h"
	#include "message.h"
	#include "Map Edgepoints.h"
	#include "PreBattle Interface.h"
	#include "strategic status.h"
	#include "squads.h"
	#include "assignments.h"
	#include "items.h"
	#include "tactical save.h"
	#include "Soldier Ani.h"
	#include "Strategic AI.h"
	#include "GameSettings.h"
	#include "MessageBoxScreen.h"
	#include "screenids.h"
	#include "Strategic Town Loyalty.h"
	#include "Soldier Profile.h"
	#include "Quests.h"
	#include "jascreens.h"
	#include "Auto Resolve.h"
	#include "Game Event Hook.h"
	#include "Animation Data.h"
	#include "Game Clock.h"
	#include "renderworld.h"
	#include "Town Militia.h"
	#include "Dialogue Control.h"
	#include "Campaign Init.h"
	#include "meanwhile.h"
	#include "Soldier macros.h"
	#include "Morale.h"
#endif

#ifdef JA2BETAVERSION
	extern BOOLEAN gfClearCreatureQuest;
#endif

#include "connect.h"
#include "Reinforcement.h"
#include "MilitiaSquads.h"

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "Arms Dealer Init.h"
#endif

#include <vector>

//The sector information required for the strategic AI.  Contains the number of enemy troops,
//as well as intentions, etc.
//SECTORINFO SectorInfo[256];
std::vector<SECTORINFO> SectorInfo (256);

UNDERGROUND_SECTORINFO *gpUndergroundSectorInfoHead = NULL;
extern UNDERGROUND_SECTORINFO* gpUndergroundSectorInfoTail;
BOOLEAN gfPendingEnemies = FALSE;
extern void BuildUndergroundSectorInfoList();

extern void EndCreatureQuest();

extern GARRISON_GROUP *gGarrisonGroup;
extern INT32 giGarrisonArraySize;

#ifdef JA2TESTVERSION
extern BOOLEAN gfOverrideSector;
#endif

INT32 gsInterrogationGridNo[3] = { 7756, 7757, 7758 };

#ifdef JA2UB
INT32		gsGridNoForMapEdgePointInfo=-1;
#endif

#ifdef JA2UB
void HandleBloodCatDeaths( SECTORINFO *pSector );
#endif

extern void Ensure_RepairedGarrisonGroup( GARRISON_GROUP **ppGarrison, INT32 *pGarraySize );


void ValidateEnemiesHaveWeapons()
{
	#ifdef JA2BETAVERSION
		SGPRect CenteringRect= {0 + xResOffset, 0, SCREEN_WIDTH - xResOffset, SCREEN_HEIGHT };
		INT32 i, iErrorDialog;
		SOLDIERTYPE *pSoldier;
		INT32 iNumInvalid = 0;

		for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; i++ )
		{
			pSoldier = MercPtrs[ i ];
			if( !pSoldier->bActive || !pSoldier->bInSector )
			{
				continue;
			}

#ifdef ENABLE_ZOMBIES
			// Flugente: zombies are fine with having no weapons...
			if ( pSoldier->IsZombie() )
				continue;
#endif

			if( !pSoldier->inv[ HANDPOS ].usItem )
			{
				iNumInvalid++;
			}
			// WDS DEBUG
			// Uncommenting the following two lines will cause all the PLACED soldiers to instantly drop dead, 
			// which is useful for debugging some things
			//pSoldier->bBleeding = 10;
			//pSoldier->stats.bLife = 1;
		}

		// do message box and return
		if( iNumInvalid )
		{
			CHAR16 str[ 100 ];
			swprintf( str, L"%d enemies have been added without any weapons!  KM:0.  Please note sector.", iNumInvalid );
			iErrorDialog = DoMessageBox( MSG_BOX_BASIC_STYLE, str, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, &CenteringRect );
		}
	#endif
}

//Counts enemies and crepitus, but not bloodcats.
UINT8 NumHostilesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT8 ubNumHostiles = 0;

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );
	AssertGE( sSectorZ, MINIMUM_VALID_Z_COORDINATE );
	AssertLE( sSectorZ, MAXIMUM_VALID_Z_COORDINATE );

	if( sSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( sSectorX, sSectorY, (UINT8)sSectorZ );
		if( pSector )
		{
			ubNumHostiles = (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumCreatures);
		}
	}
	else
	{
		SECTORINFO *pSector;
		GROUP *pGroup;

		//Count stationary hostiles
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubNumHostiles = (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumCreatures);

		//Count mobile enemies
		pGroup = gpGroupList;
		while( pGroup )
		{
			if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY)
			{
				ubNumHostiles += pGroup->ubGroupSize;
			}
			pGroup = pGroup->next;
		}
	}

	return ubNumHostiles;
}

UINT8 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT8 ubNumEnemies = 0;

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );
	AssertGE( sSectorZ, MINIMUM_VALID_Z_COORDINATE );
	AssertLE( sSectorZ, MAXIMUM_VALID_Z_COORDINATE );

	if( sSectorZ )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( sSectorX, sSectorY, (UINT8)sSectorZ );
		if( pSector )
		{
			ubNumEnemies = (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites);
		}
	}
	else
	{
		SECTORINFO *pSector;
		GROUP *pGroup;

		//Count stationary enemies
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubNumEnemies = (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites);

		//Count mobile enemies
		pGroup = gpGroupList;
		while( pGroup )
		{
			if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
			{
				ubNumEnemies += pGroup->ubGroupSize;
			}
			pGroup = pGroup->next;
		}
	}

	return ubNumEnemies;
}

UINT8 NumEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	UINT8 ubNumTroops;

	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
    // probably isn't. But I need this bit to work.
    if (sSectorX < MINIMUM_VALID_X_COORDINATE ||
        sSectorX > MAXIMUM_VALID_X_COORDINATE ||
        sSectorY < MINIMUM_VALID_Y_COORDINATE ||
        sSectorY > MAXIMUM_VALID_Y_COORDINATE)
    {
        return (0);
    }

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
	ubNumTroops = (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites);
	
	if (is_networked)
		ubNumTroops += numenemyLAN((UINT8)sSectorX,(UINT8)sSectorY ); //hayden

	pGroup = gpGroupList;
	while( pGroup )
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			ubNumTroops += pGroup->ubGroupSize;
		}
		pGroup = pGroup->next;
	}
	return ubNumTroops;
}

UINT8 NumStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
{
	SECTORINFO *pSector;

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );
	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];

	if( pSector->ubGarrisonID == NO_GARRISON )
	{ //If no garrison, no stationary.
		return( 0 );
	}

	Ensure_RepairedGarrisonGroup( &gGarrisonGroup, &giGarrisonArraySize );	 /* added NULL fix, 2007-03-03, Sgt. Kolja */

	// don't count roadblocks as stationary garrison, we want to see how many enemies are in them, not question marks
	if ( gGarrisonGroup[ pSector->ubGarrisonID ].ubComposition == ROADBLOCK )
	{
		// pretend they're not stationary
		return( 0 );
	}

	return (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites);
}

UINT8 NumMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
{
	GROUP *pGroup;
	SECTORINFO *pSector;
	UINT8 ubNumTroops;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	ubNumTroops = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			ubNumTroops += pGroup->ubGroupSize;
		}
		pGroup = pGroup->next;
	}

	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
	if( pSector->ubGarrisonID == ROADBLOCK )
	{ //consider these troops as mobile troops even though they are in a garrison
		ubNumTroops += (UINT8)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites);
	}

	return ubNumTroops;
}

void GetNumberOfMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites )
{
	GROUP *pGroup;
	SECTORINFO *pSector;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	//Now count the number of mobile groups in the sector.
	*pubNumTroops = *pubNumElites = *pubNumAdmins = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			*pubNumTroops += pGroup->pEnemyGroup->ubNumTroops;
			*pubNumElites += pGroup->pEnemyGroup->ubNumElites;
			*pubNumAdmins += pGroup->pEnemyGroup->ubNumAdmins;
		}
		pGroup = pGroup->next;
	}

	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
	if( pSector->ubGarrisonID == ROADBLOCK )
	{ //consider these troops as mobile troops even though they are in a garrison
		*pubNumAdmins += pSector->ubNumAdmins;
		*pubNumTroops += pSector->ubNumTroops;
		*pubNumElites += pSector->ubNumElites;
	}

}

void GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites )
{
	GROUP *pGroup;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	//Now count the number of mobile groups in the sector.
	*pubNumTroops = *pubNumElites = *pubNumAdmins = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			*pubNumTroops += pGroup->pEnemyGroup->ubNumTroops;
			*pubNumElites += pGroup->pEnemyGroup->ubNumElites;
			*pubNumAdmins += pGroup->pEnemyGroup->ubNumAdmins;
		}
		pGroup = pGroup->next;
	}
}


void GetNumberOfStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites )
{
	SECTORINFO *pSector;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );
	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];

	//grab the number of each type in the stationary sector
	*pubNumAdmins = pSector->ubNumAdmins;
	*pubNumTroops = pSector->ubNumTroops;
	*pubNumElites = pSector->ubNumElites;
}

void GetNumberOfEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 *pubNumAdmins, UINT8 *pubNumTroops, UINT8 *pubNumElites )
{
	UINT8 ubNumAdmins, ubNumTroops, ubNumElites;

	GetNumberOfStationaryEnemiesInSector( sSectorX, sSectorY, pubNumAdmins, pubNumTroops, pubNumElites );

	GetNumberOfMobileEnemiesInSector( sSectorX, sSectorY, &ubNumAdmins, &ubNumTroops, &ubNumElites );

	*pubNumAdmins += ubNumAdmins;
	*pubNumTroops += ubNumTroops;
	*pubNumElites += ubNumElites;
}

void EndTacticalBattleForEnemy()
{
	GROUP *pGroup;
	INT32 i;

	//Clear enemies in battle for all stationary groups in the sector.
	if( gbWorldSectorZ > 0 )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
	}
	else if( !gbWorldSectorZ )
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
		//grab the number of each type in the stationary sector
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
		pSector->ubNumCreatures = 0;
		pSector->ubCreaturesInBattle = 0;
	}
	else	// negative
		return;

	//Clear this value so that profiled enemies can be added into battles in the future.
	gfProfiledEnemyAdded = FALSE;

	//Clear enemies in battle for all mobile groups in the sector.
	pGroup = gpGroupList;
	while( pGroup )
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY )
		{
			pGroup->pEnemyGroup->ubTroopsInBattle = 0;
			pGroup->pEnemyGroup->ubElitesInBattle = 0;
			pGroup->pEnemyGroup->ubAdminsInBattle = 0;
		}
		pGroup = pGroup->next;
	}


	//Check to see if any of our mercs have abandoned the militia during a battle.  This is cause for a rather
	//severe loyalty blow.
	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->bInSector && MercPtrs[ i ]->stats.bLife >= OKLIFE )
		{ //found one live militia, so look for any enemies/creatures.
			// NOTE: this is relying on ENEMY_TEAM being immediately followed by CREATURE_TEAM
			for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; i++ )
			{
				if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->bInSector && MercPtrs[ i ]->stats.bLife >= OKLIFE )
				{ //confirmed at least one enemy here, so do the loyalty penalty.
					HandleGlobalLoyaltyEvent( GLOBAL_LOYALTY_ABANDON_MILITIA, gWorldSectorX, gWorldSectorY, 0 );
					break;
				}
			}
			break;
		}
	}
}

UINT8 NumFreeEnemySlots()
{
	UINT8 ubNumFreeSlots = 0;
	INT32 i;
	SOLDIERTYPE *pSoldier;
	//Count the number of free enemy slots.  It is possible to have multiple groups exceed the maximum.
	for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ ENEMY_TEAM].bLastID; i++ )
	{
		pSoldier = &Menptr[ i ];
		if( !pSoldier->bActive )
			ubNumFreeSlots++;
	}
	return ubNumFreeSlots;
}


//Called when entering a sector so the campaign AI can automatically insert the
//correct number of troops of each type based on the current number in the sector
//in global focus (gWorldSectorX/Y)
BOOLEAN PrepareEnemyForSectorBattle()
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	SOLDIERTYPE *pSoldier;
	unsigned ubNumAdmins, ubNumTroops, ubNumElites;
	unsigned ubTotalAdmins, ubTotalElites, ubTotalTroops;
	unsigned totalCountOfStationaryEnemies = 0;
	unsigned totalCountOfMobileEnemies = 0;
	int sNumSlots;

	gfPendingEnemies = FALSE;

	if( gbWorldSectorZ > 0 )
		return PrepareEnemyForUndergroundBattle();

	// Add the invading group
	if (gpBattleGroup && !gpBattleGroup->fPlayer )
	{
		//The enemy has instigated the battle which means they are the ones entering the conflict.
		//The player was actually in the sector first, and the enemy doesn't use reinforced placements
		HandleArrivalOfReinforcements( gpBattleGroup );

		// Reinforcement groups?  Bring it on!
		if( gGameExternalOptions.gfAllowReinforcements &&
				!( (GetTownIdForSector( gWorldSectorX, gWorldSectorY ) == OMERTA )&&( gGameOptions.ubDifficultyLevel != DIF_LEVEL_INSANE ) ) )
		{
			UINT16 pusMoveDir[4][3];
			UINT8 ubDirNumber = 0;
			GROUP *pGroup;
			SECTORINFO *pThisSector;

			// They arrived in multiple groups, so here they come
			pThisSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

			GenerateDirectionInfos( gWorldSectorX, gWorldSectorY, &ubDirNumber, pusMoveDir,	FALSE, TRUE );

			for( unsigned ubIndex = 0; ubIndex < ubDirNumber; ubIndex++ )
			{
				while ( NumMobileEnemiesInSector( SECTORX( pusMoveDir[ ubIndex ][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) ) && GetEnemyGroupInSector( SECTORX( pusMoveDir[ ubIndex][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) ) )
				{
					pGroup = GetEnemyGroupInSector( SECTORX( pusMoveDir[ ubIndex][ 0 ] ), SECTORY( pusMoveDir[ ubIndex ][ 0 ] ) );

					pGroup->ubPrevX = pGroup->ubSectorX;
					pGroup->ubPrevY = pGroup->ubSectorY;

					pGroup->ubSectorX = pGroup->ubNextX = (UINT8)gWorldSectorX;
					pGroup->ubSectorY = pGroup->ubNextY = (UINT8)gWorldSectorY;
				}
			}

			//It is now possible that other enemy groups have also arrived.  Add them in the same manner.
			pGroup = gpGroupList;
			while( pGroup )
			{
				if( pGroup != gpBattleGroup && !pGroup->fPlayer && !pGroup->fVehicle &&
					  pGroup->ubSectorX == gpBattleGroup->ubSectorX &&
						pGroup->ubSectorY == gpBattleGroup->ubSectorY &&
						!pGroup->pEnemyGroup->ubAdminsInBattle &&
						!pGroup->pEnemyGroup->ubTroopsInBattle &&
						!pGroup->pEnemyGroup->ubElitesInBattle )
				{
					HandleArrivalOfReinforcements( pGroup );
				}
				pGroup = pGroup->next;
			}

		}

		ValidateEnemiesHaveWeapons();
		return ( ( BOOLEAN) ( gpBattleGroup->ubGroupSize > 0 ) );
	}

	// WDS Count the number of placements.  This will limit the maximum number of enemies we can place on the map
	SOLDIERINITNODE *curr = gSoldierInitHead;
	unsigned mapMaximumNumberOfEnemies = 0;
	while( curr ) {
		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM ) {
			++mapMaximumNumberOfEnemies;
		}
		curr = curr->next;
	}

	AssertGE (mapMaximumNumberOfEnemies, 0);

	if (mapMaximumNumberOfEnemies > gGameExternalOptions.ubGameMaximumNumberOfEnemies)
		mapMaximumNumberOfEnemies = gGameExternalOptions.ubGameMaximumNumberOfEnemies;

	gfPendingEnemies = (NumEnemiesInSector( gWorldSectorX, gWorldSectorY ) > mapMaximumNumberOfEnemies );

	pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

	// OJW - 20090403 - override max ai in co-op
	if (is_networked && is_server && gMaxEnemiesEnabled == 1)
	{
		float totalEnemies = (float)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites);
		ubTotalAdmins = (unsigned int)(((float)pSector->ubNumAdmins / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalTroops = (unsigned int)(((float)pSector->ubNumTroops / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalElites = (unsigned int)(((float)pSector->ubNumElites / totalEnemies) * mapMaximumNumberOfEnemies);
		totalEnemies = (float)(ubTotalAdmins + ubTotalTroops + ubTotalElites);

		// take care of any rounding losses
		while (totalEnemies < mapMaximumNumberOfEnemies)
		{
			// just fill out with standard troops
			ubTotalTroops++;
			totalEnemies++;
		}

		pSector->ubNumAdmins = ubTotalAdmins;
		pSector->ubNumTroops = ubTotalTroops;
		pSector->ubNumElites = ubTotalElites;
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
	}
	else
	{

	if( pSector->uiFlags & SF_USE_MAP_SETTINGS )
	{ //count the number of enemy placements in a map and use those
		SOLDIERINITNODE *curr = gSoldierInitHead;
		ubTotalAdmins = ubTotalTroops = ubTotalElites = 0;
		while( curr )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				switch( curr->pBasicPlacement->ubSoldierClass )
				{
					case SOLDIER_CLASS_ADMINISTRATOR:		ubTotalAdmins++;	break;
					case SOLDIER_CLASS_ARMY:				ubTotalTroops++;	break;
					case SOLDIER_CLASS_ELITE:				ubTotalElites++;	break;
				}
			}
			curr = curr->next;
		}
		pSector->ubNumAdmins = ubTotalAdmins;
		pSector->ubNumTroops = ubTotalTroops;
		pSector->ubNumElites = ubTotalElites;
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
	}
	else
	{
		ubTotalAdmins = pSector->ubNumAdmins - pSector->ubAdminsInBattle;
		ubTotalTroops = pSector->ubNumTroops - pSector->ubTroopsInBattle;
		ubTotalElites = pSector->ubNumElites - pSector->ubElitesInBattle;
		}
	}

	totalCountOfStationaryEnemies = ubTotalAdmins + ubTotalTroops + ubTotalElites;

	if( totalCountOfStationaryEnemies > mapMaximumNumberOfEnemies )
	{
		/*
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_RED, MSG_ERROR, L"The total stationary enemy forces in sector %c%d is %d. (max %d)",
				gWorldSectorY + 'A' - 1, gWorldSectorX, ubTotalAdmins + ubTotalTroops + ubTotalElites, mapMaximumNumberOfEnemies );
		#endif
		*/

		ubTotalAdmins = min( mapMaximumNumberOfEnemies, ubTotalAdmins );
		ubTotalTroops = min( mapMaximumNumberOfEnemies-ubTotalAdmins, ubTotalTroops );
		ubTotalElites = min( mapMaximumNumberOfEnemies-ubTotalAdmins-ubTotalTroops, ubTotalElites );
	}

	pSector->ubAdminsInBattle += ubTotalAdmins;
	pSector->ubTroopsInBattle += ubTotalTroops;
	pSector->ubElitesInBattle += ubTotalElites;

	#ifdef JA2TESTVERSION
	if( gfOverrideSector )
	{
		//if there are no troops in the current groups, then we're done.
		if( !ubTotalAdmins && !ubTotalTroops && !ubTotalElites )
			return FALSE;
		AddSoldierInitListEnemyDefenceSoldiers( ubTotalAdmins, ubTotalTroops, ubTotalElites );
		ValidateEnemiesHaveWeapons();
		return TRUE;
	}
	#endif

	//Search for movement groups that happen to be in the sector.
	sNumSlots = NumFreeEnemySlots();
	if (sNumSlots > (int)mapMaximumNumberOfEnemies)
		sNumSlots = mapMaximumNumberOfEnemies;

	/*
	 * WDS - this test no longer valid with the above change
	//Test:  All slots should be free at this point!
	if( sNumSlots != gTacticalStatus.Team[ENEMY_TEAM].bLastID - gTacticalStatus.Team[ENEMY_TEAM].bFirstID + 1 )
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_RED, MSG_ERROR, L"All enemy slots should be free at this point.  Only %d of %d are available.\nTrying to add %d admins, %d troops, and %d elites.",
				sNumSlots, gTacticalStatus.Team[ENEMY_TEAM].bLastID - gTacticalStatus.Team[ENEMY_TEAM].bFirstID + 1 ,
				ubTotalAdmins, ubTotalTroops, ubTotalElites );
		#endif
	}
	*/

	//Subtract the total number of stationary enemies from the available slots, as stationary forces take
	//precendence in combat.  The mobile forces that could also be in the same sector are considered later if
	//all the slots fill up.
	sNumSlots -= ubTotalAdmins + ubTotalTroops + ubTotalElites;

	//Now, process all of the groups and search for both enemy and player groups in the sector.
	//For enemy groups, we fill up the slots until we have none left or all of the groups have been
	//processed.

	for( pGroup = gpGroupList;
		 pGroup;
		 pGroup = pGroup->next)
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle &&
				 pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{ //Process enemy group in sector.
			if( sNumSlots > 0 )
			{
				AssertGE(pGroup->pEnemyGroup->ubNumAdmins, pGroup->pEnemyGroup->ubAdminsInBattle);
				ubNumAdmins = pGroup->pEnemyGroup->ubNumAdmins - pGroup->pEnemyGroup->ubAdminsInBattle;
				sNumSlots -= ubNumAdmins;
				if( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumAdmins += sNumSlots;
					sNumSlots = 0;
					gfPendingEnemies = TRUE;
				}
				pGroup->pEnemyGroup->ubAdminsInBattle += ubNumAdmins;
				ubTotalAdmins += ubNumAdmins;
			}
			if( sNumSlots > 0 )
			{ //Add regular army forces.
				AssertGE(pGroup->pEnemyGroup->ubNumTroops, pGroup->pEnemyGroup->ubTroopsInBattle);
				ubNumTroops = pGroup->pEnemyGroup->ubNumTroops - pGroup->pEnemyGroup->ubTroopsInBattle;
				sNumSlots -= ubNumTroops;
				if( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumTroops += sNumSlots;
					sNumSlots = 0;
					gfPendingEnemies = TRUE;
				}
				pGroup->pEnemyGroup->ubTroopsInBattle += ubNumTroops;
				ubTotalTroops += ubNumTroops;
			}
			if( sNumSlots > 0 )
			{ //Add elite troops
				AssertGE(pGroup->pEnemyGroup->ubNumElites, pGroup->pEnemyGroup->ubElitesInBattle);
				ubNumElites = pGroup->pEnemyGroup->ubNumElites - pGroup->pEnemyGroup->ubElitesInBattle;
				sNumSlots -= ubNumElites;
				if( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumElites += sNumSlots;
					sNumSlots = 0;
					gfPendingEnemies = TRUE;
				}
				pGroup->pEnemyGroup->ubElitesInBattle += ubNumElites;
				ubTotalElites += ubNumElites;
			}
			//NOTE:
			//no provisions for profile troop leader or retreat groups yet.
		}

		if( pGroup->fPlayer && !pGroup->fVehicle && !pGroup->fBetweenSectors &&
				pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{ //TEMP:  The player path needs to get destroyed, otherwise, it'll be impossible to move the
			//			 group after the battle is resolved.

			// no one in the group any more continue loop
			if( pGroup->pPlayerList == NULL )
			{
				continue;
			}

			// clear the movt for this grunt and his buddies
			RemoveGroupWaypoints( pGroup->ubGroupID );
		}
	}

	//if there are no troops in the current groups, then we're done.
	if( !ubTotalAdmins && !ubTotalTroops && !ubTotalElites )
	{
		return FALSE;
	}

	AddSoldierInitListEnemyDefenceSoldiers( ubTotalAdmins, ubTotalTroops, ubTotalElites );

	//Now, we have to go through all of the enemies in the new map, and assign their respective groups if
	//in a mobile group, but only for the ones that were assigned from the
	sNumSlots = mapMaximumNumberOfEnemies - totalCountOfStationaryEnemies;

	pGroup = gpGroupList;
	while( pGroup && sNumSlots > 0 )
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle &&
				 pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{
			ubNumAdmins = pGroup->pEnemyGroup->ubAdminsInBattle;
			ubNumTroops = pGroup->pEnemyGroup->ubTroopsInBattle;
			ubNumElites = pGroup->pEnemyGroup->ubElitesInBattle;
			unsigned num = ubNumAdmins + ubNumTroops + ubNumElites;

			unsigned firstSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
			unsigned lastSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bLastID;
			unsigned slotsAvailable = lastSlot-firstSlot+1;
			AssertGE((int)slotsAvailable, sNumSlots);

			for (unsigned slot = firstSlot;
				(slot <= lastSlot) && (num > 0);
				++slot)
			{
				pSoldier = &Menptr[ slot ];

				// Skip inactive and already grouped soldiers
				if (!pSoldier->bActive || pSoldier->ubGroupID)
				{
					continue;
				}

				// At this point we should not have added more soldiers than are in slots
				AssertGT( sNumSlots, 0 );

				switch( pSoldier->ubSoldierClass )
				{
					case SOLDIER_CLASS_ADMINISTRATOR:
						if( ubNumAdmins )
						{
							num--;
							sNumSlots--;
							ubNumAdmins--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
						}
						break;
					case SOLDIER_CLASS_ARMY:
						if( ubNumTroops )
						{
							num--;
							sNumSlots--;
							ubNumTroops--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
						}
						break;
					case SOLDIER_CLASS_ELITE:
						if( ubNumElites )
						{
							num--;
							sNumSlots--;
							ubNumElites--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
						}
						break;
				}
			}

			AssertEQ( ubNumElites , 0);
			AssertEQ( ubNumTroops , 0);
			AssertEQ( ubNumAdmins , 0);
			AssertEQ( num , 0);
		}
		pGroup = pGroup->next;
	}

	ValidateEnemiesHaveWeapons();

	return TRUE;
}

BOOLEAN PrepareEnemyForUndergroundBattle()
{
	UNDERGROUND_SECTORINFO *pUnderground;
	unsigned ubTotalAdmins, ubTotalTroops, ubTotalElites;

	pUnderground = gpUndergroundSectorInfoHead;
	while( pUnderground )
	{
		if( pUnderground->ubSectorX == gWorldSectorX &&
			  pUnderground->ubSectorY == gWorldSectorY &&
				pUnderground->ubSectorZ == gbWorldSectorZ )
		{ //This is the sector we are going to be fighting in.
			if( pUnderground->ubNumAdmins || pUnderground->ubNumTroops || pUnderground->ubNumElites )
			{
				ubTotalAdmins = (UINT8)(pUnderground->ubNumAdmins - pUnderground->ubAdminsInBattle);
				ubTotalTroops = (UINT8)(pUnderground->ubNumTroops - pUnderground->ubTroopsInBattle);
				ubTotalElites = (UINT8)(pUnderground->ubNumElites - pUnderground->ubElitesInBattle);
				pUnderground->ubAdminsInBattle += ubTotalAdmins;
				pUnderground->ubTroopsInBattle += ubTotalTroops;
				pUnderground->ubElitesInBattle += ubTotalElites;
				AddSoldierInitListEnemyDefenceSoldiers( pUnderground->ubNumAdmins, pUnderground->ubNumTroops, pUnderground->ubNumElites );
				ValidateEnemiesHaveWeapons();
			}
			return ( ( BOOLEAN) ( pUnderground->ubNumAdmins + pUnderground->ubNumTroops + pUnderground->ubNumElites > 0 ) );
		}
		pUnderground = pUnderground->next;
	}

	// underground sector not found in list
	Assert( FALSE );
	return FALSE;
}

//The queen AI layer must process the event by subtracting forces, etc.
void ProcessQueenCmdImplicationsOfDeath( SOLDIERTYPE *pSoldier )
{
	INT32 iNumEnemiesInSector;
	SECTORINFO *pSector;
	INT32 iMaxEnemyGroupSize = gGameExternalOptions.iMaxEnemyGroupSize;
    DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"QueenCommand");

	AssertNotNIL (pSoldier);

	EvaluateDeathEffectsToSoldierInitList( pSoldier );

	switch( pSoldier->ubProfile )
	{
#ifdef JA2UB	
		case 75://MORRIS:

			if( !pSoldier->bSectorZ )
			{
				pSector = &SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ];
				if( pSector->ubNumElites )
				{
					pSector->ubNumElites--;
				}
				if( pSector->ubElitesInBattle )
				{
					pSector->ubElitesInBattle--;
				}
			}
			 else
			{
				UNDERGROUND_SECTORINFO *pUnderground;
				pUnderground = FindUnderGroundSector( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, (UINT8)pSoldier->bSectorZ );
				Assert( pUnderground );
				if( pUnderground->ubNumElites )
				{
					pUnderground->ubNumElites--;
				}
				if( pUnderground->ubElitesInBattle )
				{
					pUnderground->ubElitesInBattle--;
				}
			}
			break;
#endif			
		case MIKE:
		case IGGY:
			if( pSoldier->ubProfile == IGGY && !gubFact[ FACT_IGGY_AVAILABLE_TO_ARMY ] )
			{ //Iggy is on our team!
				break;
			}
			if( !pSoldier->bSectorZ )
			{
				pSector = &SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ];
				if( pSector->ubNumElites )
				{
					pSector->ubNumElites--;
				}
				if( pSector->ubElitesInBattle )
				{
					pSector->ubElitesInBattle--;
				}
			}
			else
			{
				UNDERGROUND_SECTORINFO *pUnderground;
				pUnderground = FindUnderGroundSector( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, (UINT8)pSoldier->bSectorZ );
				Assert( pUnderground );
				if( pUnderground->ubNumElites )
				{
					pUnderground->ubNumElites--;
				}
				if( pUnderground->ubElitesInBattle )
				{
					pUnderground->ubElitesInBattle--;
				}
			}
			break;
	}

	if( pSoldier->aiData.bNeutral || pSoldier->bTeam != ENEMY_TEAM && pSoldier->bTeam != CREATURE_TEAM )
		return;
	//we are recording an enemy death
	if( pSoldier->ubGroupID )
	{ //The enemy was in a mobile group
		GROUP *pGroup;
		pGroup = GetGroup( pSoldier->ubGroupID );
		if( !pGroup )
		{
			#ifdef JA2BETAVERSION
				CHAR16 str[256];
				swprintf( str, L"Enemy soldier killed with ubGroupID of %d, and the group doesn't exist!", pSoldier->ubGroupID );
				DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
			#endif
			return;
		}
		if( pGroup->fPlayer )
		{
			#ifdef JA2BETAVERSION
				CHAR16 str[256];
				swprintf( str, L"Attempting to process player group thinking it's an enemy group in ProcessQueenCmdImplicationsOfDeath()", pSoldier->ubGroupID );
				DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
			#endif
			return;
		}
		switch( pSoldier->ubSoldierClass )
		{
			case SOLDIER_CLASS_ELITE:
				#ifdef JA2BETAVERSION
					if( !pGroup->pEnemyGroup->ubNumElites )
					{
						//swprintf( str, L"Enemy elite killed with ubGroupID of %d, but the group doesn't contain elites!", pGroup->ubGroupID );
						//DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
						break;
					}
					if( guiCurrentScreen == GAME_SCREEN )
					{
						if( pGroup->ubGroupSize <= iMaxEnemyGroupSize && pGroup->pEnemyGroup->ubNumElites != pGroup->pEnemyGroup->ubElitesInBattle && !gfPendingEnemies ||
								pGroup->ubGroupSize > iMaxEnemyGroupSize /* || pGroup->pEnemyGroup->ubNumElites > 50 || pGroup->pEnemyGroup->ubElitesInBattle > 50*/ )
						{
							DoScreenIndependantMessageBox( L"Group elite counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
						}
					}
				#endif
				if( pGroup->pEnemyGroup->ubNumElites )
				{
					pGroup->pEnemyGroup->ubNumElites--;
				}
				if( pGroup->pEnemyGroup->ubElitesInBattle )
				{
					pGroup->pEnemyGroup->ubElitesInBattle--;
				}
				break;
			case SOLDIER_CLASS_ARMY:
				#ifdef JA2BETAVERSION
					if( !pGroup->pEnemyGroup->ubNumTroops )
					{
						//swprintf( str, L"Enemy troop killed with ubGroupID of %d, but the group doesn't contain elites!", pGroup->ubGroupID );
						//DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
						break;
					}
					if( guiCurrentScreen == GAME_SCREEN )
					{
						if( pGroup->ubGroupSize <= iMaxEnemyGroupSize && pGroup->pEnemyGroup->ubNumTroops != pGroup->pEnemyGroup->ubTroopsInBattle && !gfPendingEnemies ||
								pGroup->ubGroupSize > iMaxEnemyGroupSize /*|| pGroup->pEnemyGroup->ubNumTroops > 50 || pGroup->pEnemyGroup->ubTroopsInBattle > 50*/ )
						{
							// haydent
							if (!is_client)
							{
								DoScreenIndependantMessageBox( L"Group troop counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
						
							}
						}
					}
				#endif
				if( pGroup->pEnemyGroup->ubNumTroops )
				{
					pGroup->pEnemyGroup->ubNumTroops--;
				}
				if( pGroup->pEnemyGroup->ubTroopsInBattle )
				{
					pGroup->pEnemyGroup->ubTroopsInBattle--;
				}
				break;
			case SOLDIER_CLASS_ADMINISTRATOR:
				#ifdef JA2BETAVERSION
					if( !pGroup->pEnemyGroup->ubNumAdmins )
					{
						//swprintf( str, L"Enemy administrator killed with ubGroupID of %d, but the group doesn't contain elites!", pGroup->ubGroupID );
						//DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
						break;
					}
					if( guiCurrentScreen == GAME_SCREEN )
					{
						if( pGroup->ubGroupSize <= iMaxEnemyGroupSize && pGroup->pEnemyGroup->ubNumAdmins != pGroup->pEnemyGroup->ubAdminsInBattle && !gfPendingEnemies ||
						pGroup->ubGroupSize > iMaxEnemyGroupSize /*|| pGroup->pEnemyGroup->ubNumAdmins > 50 || pGroup->pEnemyGroup->ubAdminsInBattle > 50*/ )
						{
							DoScreenIndependantMessageBox( L"Group admin counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
						}
					}
				#endif
				if( pGroup->pEnemyGroup->ubNumAdmins )
				{
					pGroup->pEnemyGroup->ubNumAdmins--;
				}
				if( pGroup->pEnemyGroup->ubAdminsInBattle )
				{
					pGroup->pEnemyGroup->ubAdminsInBattle--;
				}
				break;
		}
		if( pGroup->ubGroupSize )
			pGroup->ubGroupSize--;
		RecalculateGroupWeight( pGroup );
		if( !pGroup->ubGroupSize )
		{
			RemovePGroup( pGroup );
		}
	}
	else
	{ //The enemy was in a stationary defence group
		if( !gbWorldSectorZ || IsAutoResolveActive() )
		{ //ground level (SECTORINFO)
			SECTORINFO *pSector;
			#ifdef JA2BETAVERSION
			UINT32 ubTotalEnemies;
			#endif

//			if( !IsAutoResolveActive() )
//			{
				pSector = &SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ];
//			}
//			else
//			{
//				pSector = &SectorInfo[ GetAutoResolveSectorID() ];
//			}

			#ifdef JA2BETAVERSION
				ubTotalEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites;
			#endif

			switch( pSoldier->ubSoldierClass )
			{
				case SOLDIER_CLASS_ADMINISTRATOR:
					#ifdef JA2BETAVERSION
						if( guiCurrentScreen == GAME_SCREEN )
						{
							if( ubTotalEnemies <= gGameExternalOptions.ubGameMaximumNumberOfEnemies && pSector->ubNumAdmins != pSector->ubAdminsInBattle ||
								!pSector->ubNumAdmins || !pSector->ubAdminsInBattle ||
								pSector->ubNumAdmins > gGameExternalOptions.ubGameMaximumNumberOfEnemies || pSector->ubAdminsInBattle > gGameExternalOptions.ubGameMaximumNumberOfEnemies )
							{
								DoScreenIndependantMessageBox( L"Sector admin counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
							}
						}
					#endif
					if( pSector->ubNumAdmins )
					{
						pSector->ubNumAdmins--;
					}
					if( pSector->ubAdminsInBattle )
					{
						pSector->ubAdminsInBattle--;
					}
					break;
				case SOLDIER_CLASS_ARMY:
					#ifdef JA2BETAVERSION
						if( guiCurrentScreen == GAME_SCREEN )
						{
							if( ubTotalEnemies <= gGameExternalOptions.ubGameMaximumNumberOfEnemies && pSector->ubNumTroops != pSector->ubTroopsInBattle ||
									!pSector->ubNumTroops || !pSector->ubTroopsInBattle ||
									pSector->ubNumTroops > gGameExternalOptions.ubGameMaximumNumberOfEnemies || pSector->ubTroopsInBattle > gGameExternalOptions.ubGameMaximumNumberOfEnemies )
							{
								if(!is_client)DoScreenIndependantMessageBox( L"Sector troop counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
								//disabled: hayden.
							}
						}
					#endif
					if( pSector->ubNumTroops )
					{
						pSector->ubNumTroops--;
					}
					if( pSector->ubTroopsInBattle )
					{
						pSector->ubTroopsInBattle--;
					}
					break;
				case SOLDIER_CLASS_ELITE:
					#ifdef JA2BETAVERSION
						if( guiCurrentScreen == GAME_SCREEN )
						{
							if( ubTotalEnemies <= gGameExternalOptions.ubGameMaximumNumberOfEnemies && pSector->ubNumElites != pSector->ubElitesInBattle ||
									!pSector->ubNumElites || !pSector->ubElitesInBattle ||
									pSector->ubNumElites > gGameExternalOptions.ubGameMaximumNumberOfEnemies || pSector->ubElitesInBattle > gGameExternalOptions.ubGameMaximumNumberOfEnemies )
							{
								DoScreenIndependantMessageBox( L"Sector elite counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
							}
						}
					#endif
					if( pSector->ubNumElites )
					{
						pSector->ubNumElites--;
					}
					if( pSector->ubElitesInBattle )
					{
						pSector->ubElitesInBattle--;
					}
					break;
				case SOLDIER_CLASS_CREATURE:
					if( pSoldier->ubBodyType != BLOODCAT )
					{
						#ifdef JA2BETAVERSION
							if( guiCurrentScreen == GAME_SCREEN )
							{
								if( ubTotalEnemies <= (UINT32)iMaxEnemyGroupSize && pSector->ubNumCreatures != pSector->ubCreaturesInBattle ||
										!pSector->ubNumCreatures || !pSector->ubCreaturesInBattle ||
										pSector->ubNumCreatures > 50 || pSector->ubCreaturesInBattle > 50 )
								{
									DoScreenIndependantMessageBox( L"Sector creature counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
								}
							}
						#endif
						if( pSector->ubNumCreatures )
						{
							pSector->ubNumCreatures--;
						}
						if( pSector->ubCreaturesInBattle )
						{
							pSector->ubCreaturesInBattle--;
						}
					}
					else
					{
						if( pSector->bBloodCats )
						{
							pSector->bBloodCats--;
						}
#ifdef JA2UB						
							//JA25 UB
							//handle anything important when bloodcats die
							HandleBloodCatDeaths( pSector );
#endif
					}

					break;
			}
			RecalculateSectorWeight( (UINT8)SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) );
		}
		else
		{ //basement level (UNDERGROUND_SECTORINFO)
			UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			#ifdef JA2BETAVERSION
			UINT32 ubTotalEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites;
			#endif
			if( pSector )
			{
				switch( pSoldier->ubSoldierClass )
				{
					case SOLDIER_CLASS_ADMINISTRATOR:
						#ifdef JA2BETAVERSION
						if( ubTotalEnemies <= (UINT32)iMaxEnemyGroupSize && pSector->ubNumAdmins != pSector->ubAdminsInBattle ||
								!pSector->ubNumAdmins || !pSector->ubAdminsInBattle ||
								pSector->ubNumAdmins > 100 || pSector->ubAdminsInBattle > iMaxEnemyGroupSize )
						{
							DoScreenIndependantMessageBox( L"Underground sector admin counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
						}
						#endif
						if( pSector->ubNumAdmins )
						{
							pSector->ubNumAdmins--;
						}
						if( pSector->ubAdminsInBattle )
						{
							pSector->ubAdminsInBattle--;
						}
						break;
					case SOLDIER_CLASS_ARMY:
						#ifdef JA2BETAVERSION
						if( ubTotalEnemies <= (UINT32)iMaxEnemyGroupSize && pSector->ubNumTroops != pSector->ubTroopsInBattle ||
								!pSector->ubNumTroops || !pSector->ubTroopsInBattle ||
								pSector->ubNumTroops > 100 || pSector->ubTroopsInBattle > iMaxEnemyGroupSize )
						{
							DoScreenIndependantMessageBox( L"Underground sector troop counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
						}
						#endif
						if( pSector->ubNumTroops )
						{
							pSector->ubNumTroops--;
						}
						if( pSector->ubTroopsInBattle )
						{
							pSector->ubTroopsInBattle--;
						}
						break;
					case SOLDIER_CLASS_ELITE:
						#ifdef JA2BETAVERSION
						if( ubTotalEnemies <= (UINT32)iMaxEnemyGroupSize && pSector->ubNumElites != pSector->ubElitesInBattle ||
								!pSector->ubNumElites || !pSector->ubElitesInBattle ||
								pSector->ubNumElites > 100 || pSector->ubElitesInBattle > iMaxEnemyGroupSize )
						{
							DoScreenIndependantMessageBox( L"Underground sector elite counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
						}
						#endif
						if( pSector->ubNumElites )
						{
							pSector->ubNumElites--;
						}
						if( pSector->ubElitesInBattle )
						{
							pSector->ubElitesInBattle--;
						}
						break;
					case SOLDIER_CLASS_CREATURE:
						if (pSoldier->ubBodyType == BLOODCAT)
						{
							if( pSector->ubNumBloodcats > 0 )
								pSector->ubNumBloodcats--;
						}
						else
						{
							#ifdef JA2BETAVERSION
							if( ubTotalEnemies <= (UINT32)iMaxEnemyGroupSize && pSector->ubNumCreatures != pSector->ubCreaturesInBattle ||
									!pSector->ubNumCreatures || !pSector->ubCreaturesInBattle ||
									pSector->ubNumCreatures > gGameExternalOptions.ubGameMaximumNumberOfCreatures || pSector->ubCreaturesInBattle > gGameExternalOptions.ubGameMaximumNumberOfCreatures )
							{
								DoScreenIndependantMessageBox( L"Underground sector creature counters are bad.  What were the last 2-3 things to die, and how?  Save game and send to KM with info!!!", MSG_BOX_FLAG_OK, NULL );
							}
							#endif
							if( pSector->ubNumCreatures )
							{
								pSector->ubNumCreatures--;
							}
							if( pSector->ubCreaturesInBattle )
							{
								pSector->ubCreaturesInBattle--;
							}

							if( !pSector->ubNumCreatures && gWorldSectorX != 9 && gWorldSectorY != 10 )
							{ //If the player has successfully killed all creatures in ANY underground sector except J9
								//then cancel any pending creature town attack.
								DeleteAllStrategicEventsOfType( EVENT_CREATURE_ATTACK );
							}

							// a monster has died.  Post an event to immediately check whether a mine has been cleared.
							AddStrategicEventUsingSeconds( EVENT_CHECK_IF_MINE_CLEARED, GetWorldTotalSeconds() + 15, 0);

							if( pSoldier->ubBodyType == QUEENMONSTER )
							{
								//Need to call this, as the queen is really big, and killing her leaves a bunch
								//of bad tiles in behind her.  Calling this function cleans it up.
								InvalidateWorldRedundency();
								//Now that the queen is dead, turn off the creature quest.
								EndCreatureQuest();
								EndQuest( QUEST_CREATURES, gWorldSectorX, gWorldSectorY );
							}
						}
						break;
				}
			}
		}
	}
	if( !pSoldier->bSectorZ )
	{
		pSector = &SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ];
		iNumEnemiesInSector = NumEnemiesInSector( pSoldier->sSectorX, pSoldier->sSectorY );
		if( iNumEnemiesInSector )
		{
			if( pSector->bLastKnownEnemies >= 0 )
			{
				pSector->bLastKnownEnemies = (INT8)iNumEnemiesInSector;
			}
		}
		else
		{
			pSector->bLastKnownEnemies = 0;
		}
	}
}

//Well, not so rarely with mobile reinforcements!
//Rarely, there will be more enemies than supported by the engine.  In this case, these
//soldiers are waiting for a slot to be free so that they can enter the battle.  This
//essentially allows for an infinite number of troops, though only gGameExternalOptions.ubGameMaximumNumberOfEnemies at a time can fight.
//This is also called whenever an enemy group's reinforcements arrive because the code is
//identical, though it is highly likely that they will all be successfully added on the first call.
void AddPossiblePendingEnemiesToBattle()
{
	// Check if no world is loaded, and is not underground level
	if(!(gWorldSectorX > 0 && gWorldSectorY > 0 && gbWorldSectorZ == 0))//dnl ch57 161009
		return;

#ifdef JA2UB
	BOOLEAN fMagicallyAppeared=FALSE;
#endif

	UINT8 ubSlots, ubNumAvailable;
	UINT8 ubNumElites, ubNumTroops, ubNumAdmins;
	UINT8 ubNumGroupsInSector;
	UINT8 ubGroupIndex;
	GROUP *pGroup;
	SECTORINFO *pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
	GROUP **pGroupInSectorList;
	static UINT8 ubPredefinedInsertionCode = 255;

	// haydent
	if (is_networked)
	{
		if ((is_client && !is_server) || gEnemyEnabled == 0)
		{
			return;
		}
	}

	if( ( !PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, 0 ) && !CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY ) )
		|| !NumEnemiesInSector( gWorldSectorX, gWorldSectorY ) ) return;

	ubSlots = NumFreeEnemySlots();
	if( !ubSlots )
	{ //no available slots to add enemies to.  Try again later...
		return;
	}

	if( !gfPendingEnemies )
	{
		//Optimization.  No point in checking for group reinforcements if we know that there aren't any more enemies that can
		//be added to this battle.  This changes whenever a new enemy group arrives at the scene.

		while (ubSlots)
		{
			UINT8 ubInsertionCode = 255;

			if( gTacticalStatus.Team[ ENEMY_TEAM ].bAwareOfOpposition == TRUE )
				ubInsertionCode = DoReinforcementAsPendingEnemy( gWorldSectorX, gWorldSectorY );

			if (ubInsertionCode == 255)
			{
				break;
			}

			// Assume we added one since there are supposedly more available and room for them
			ubSlots--;
		}

		// It's probable that the "pending enemies" flag isn't working right.  So we'll go ahead and continue into
		// the group insertion code from here.
		//return;
	}

	if( pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins )
	{
		ubNumElites = ubNumTroops = ubNumAdmins = 0;

		ubNumAvailable = pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins - pSector->ubElitesInBattle - pSector->ubTroopsInBattle - pSector->ubAdminsInBattle;
		while( ubNumAvailable && ubSlots )
		{
			// So they just magically appear out of nowhere from the edge?

			//This group has enemies waiting for a chance to enter the battle.
			if( pSector->ubElitesInBattle < pSector->ubNumElites )
			{ //Add an elite troop
				pSector->ubElitesInBattle++;
				ubNumAvailable--;
				ubSlots--;
				ubNumElites++;
			}
			else if( pSector->ubTroopsInBattle < pSector->ubNumTroops )
			{ //Add a regular troop.
				pSector->ubTroopsInBattle++;
				ubNumAvailable--;
				ubSlots--;
				ubNumTroops++;
			}
			else if( pSector->ubAdminsInBattle < pSector->ubNumAdmins )
			{ //Add an elite troop
				pSector->ubAdminsInBattle++;
				ubNumAvailable--;
				ubSlots--;
				ubNumAdmins++;
			}
			else
			{
				AssertMsg( 0, "AddPossiblePendingEnemiesToBattle():  Logic Error -- by Kris" );
			}
		}


		if( ubNumAdmins || ubNumTroops || ubNumElites )
		{	//This group has contributed forces, then add them now, because different
			//groups appear on different sides of the map.
			UINT8 ubStrategicInsertionCode = ubPredefinedInsertionCode;
			//First, determine which entrypoint to use, based on the travel direction of the group.

			if( ubStrategicInsertionCode == 255 )
			{
				if( NumEnemiesInSector( gWorldSectorX + 1, gWorldSectorY ) )
					ubStrategicInsertionCode = INSERTION_CODE_EAST;
				else if( NumEnemiesInSector( gWorldSectorX - 1, gWorldSectorY ) )
					ubStrategicInsertionCode = INSERTION_CODE_WEST;
				else if( NumEnemiesInSector( gWorldSectorX, gWorldSectorY + 1 ) )
					ubStrategicInsertionCode = INSERTION_CODE_SOUTH;
				else if( NumEnemiesInSector( gWorldSectorX, gWorldSectorY - 1 ) )
					ubStrategicInsertionCode = INSERTION_CODE_NORTH;
			#ifdef JA2UB
				else if( gsGridNoForMapEdgePointInfo != -1 )
					{
						//Ja25: it doesnt matter the entry point at this point, it will become GRIDNO at a later point
						ubStrategicInsertionCode = INSERTION_CODE_NORTH;
						fMagicallyAppeared = FALSE;
					}
			#endif
			}

			if( ubStrategicInsertionCode == 255 )
			{
				if( !CountAllMilitiaInSector( gWorldSectorX + 1, gWorldSectorY ) )
					ubStrategicInsertionCode = INSERTION_CODE_EAST;
				else if( !CountAllMilitiaInSector( gWorldSectorX - 1, gWorldSectorY ) )
					ubStrategicInsertionCode = INSERTION_CODE_WEST;
				else if( !CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY + 1 ) )
					ubStrategicInsertionCode = INSERTION_CODE_SOUTH;
				else if( !CountAllMilitiaInSector( gWorldSectorX, gWorldSectorY - 1 ) )
					ubStrategicInsertionCode = INSERTION_CODE_NORTH;
			}

			if( ubStrategicInsertionCode < INSERTION_CODE_NORTH ||ubStrategicInsertionCode > INSERTION_CODE_WEST )
				ubStrategicInsertionCode = INSERTION_CODE_NORTH + Random( 4 );

			AddEnemiesToBattle( 0, ubStrategicInsertionCode, ubNumAdmins, ubNumTroops, ubNumElites, FALSE );
			gfPendingEnemies = TRUE;
		}
	}

	ubPredefinedInsertionCode = 255;

	// Figure out which groups are in the sector, so we can have reinforcements arrive at random
	for (ubNumGroupsInSector = 0, pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
			ubNumGroupsInSector++;
	}
	pGroupInSectorList = (GROUP**) MemAlloc( ubNumGroupsInSector * sizeof( GROUP*));
	for (ubNumGroupsInSector = 0, pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if( !pGroup->fPlayer && !pGroup->fVehicle && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{
			pGroupInSectorList[ ubNumGroupsInSector++] = pGroup;
		}
	}

	while( ubSlots && ubNumGroupsInSector )
	{
		UINT8 ubInsertionCode = 255;

		ubGroupIndex = Random( ubNumGroupsInSector);
		pGroup = pGroupInSectorList[ ubGroupIndex];

		// Flugente fix: check for underflow...
		UINT8 currentgroupsize = pGroup->pEnemyGroup->ubElitesInBattle + pGroup->pEnemyGroup->ubTroopsInBattle + pGroup->pEnemyGroup->ubAdminsInBattle;
		if ( currentgroupsize > pGroup->ubGroupSize )
			ubNumAvailable = 0;
		else
			ubNumAvailable = pGroup->ubGroupSize - currentgroupsize;

		if (!ubNumAvailable)
		{
			// Looks like we picked an empty group.  Make a note of it
			pGroupInSectorList[ ubGroupIndex] = pGroupInSectorList[ --ubNumGroupsInSector ];
			continue;
		}

		if( pGroup->ubPrevX && pGroup->ubPrevY )
		{
			if( pGroup->ubSectorX < pGroup->ubPrevX )
				ubInsertionCode = INSERTION_CODE_EAST;
			else if( pGroup->ubSectorX > pGroup->ubPrevX )
				ubInsertionCode = INSERTION_CODE_WEST;
			else if( pGroup->ubSectorY < pGroup->ubPrevY )
				ubInsertionCode = INSERTION_CODE_SOUTH;
			else if( pGroup->ubSectorY > pGroup->ubPrevY )
				ubInsertionCode = INSERTION_CODE_NORTH;
			else
			{
				// WANNE: Hack: If no valid insertion is found, get random insertion instead of Assert() error
				UINT32 rndInsertionCode = GetRndNum(3);				
				ubInsertionCode = rndInsertionCode;

				//Assert(0);
			}
		}
		else if( pGroup->ubNextX && pGroup->ubNextY )
		{
			if( pGroup->ubSectorX < pGroup->ubNextX )
				ubInsertionCode = INSERTION_CODE_EAST;
			else if( pGroup->ubSectorX > pGroup->ubNextX )
				ubInsertionCode = INSERTION_CODE_WEST;
			else if( pGroup->ubSectorY < pGroup->ubNextY )
				ubInsertionCode = INSERTION_CODE_SOUTH;
			else if( pGroup->ubSectorY > pGroup->ubNextY )
				ubInsertionCode = INSERTION_CODE_NORTH;
			else
			{
				// WANNE: Hack: If no valid insertion is found, get random insertion instead of Assert() error
				UINT32 rndInsertionCode = GetRndNum(3);				
				ubInsertionCode = rndInsertionCode;
				
				//Assert(0);
			}
		}
		else
		{
			// The group has no movement orders.  Where did it come from?
			Assert(0);
		}

		if( pGroup->pEnemyGroup->ubElitesInBattle < pGroup->pEnemyGroup->ubNumElites )
		{ //Add an elite troop
			pGroup->pEnemyGroup->ubElitesInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 0, 1, FALSE );
		}
		else if( pGroup->pEnemyGroup->ubTroopsInBattle < pGroup->pEnemyGroup->ubNumTroops )
		{ //Add a regular troop.
			pGroup->pEnemyGroup->ubTroopsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 1, 0, FALSE );
		}
		else if( pGroup->pEnemyGroup->ubAdminsInBattle < pGroup->pEnemyGroup->ubNumAdmins )
		{ //Add an admin troop
			pGroup->pEnemyGroup->ubAdminsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 1, 0, 0, FALSE );
		}
		else
		{
			// WANNE: Instead of asserting, just add a regular troop
			//AssertMsg( 0, "AddPossiblePendingEnemiesToBattle():  Logic Error -- by Kris" );

			pGroup->pEnemyGroup->ubTroopsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 1, 0, FALSE );
		}
	}

	MemFree( pGroupInSectorList);

	if( ubSlots )
	{ //After going through the process, we have finished with some free slots and no more enemies to add.
    //So, we can turn off the flag, as this check is no longer needed.
		gfPendingEnemies = FALSE;
	}
}

void NotifyPlayersOfNewEnemies()
{
	INT32 iSoldiers, iChosenSoldier, i;
	SOLDIERTYPE *pSoldier;
	BOOLEAN fIgnoreBreath = FALSE;

	iSoldiers = 0;
	for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
	{ //find a merc that is aware.
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bInSector && pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE && pSoldier->bBreath >= OKBREATH )
		{
			iSoldiers++;
		}
	}
	if( !iSoldiers )
	{ // look for an out of breath merc.
		fIgnoreBreath = TRUE;

		for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
		{ //find a merc that is aware.
			pSoldier = MercPtrs[ i ];
			if( pSoldier->bInSector && pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE )
			{
				iSoldiers++;
			}
		}
	}
	if( iSoldiers )
	{
		iChosenSoldier = Random( iSoldiers );
		for( i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
		{ //find a merc that is aware.
			pSoldier = MercPtrs[ i ];
			if( pSoldier->bInSector && pSoldier->bActive && pSoldier->stats.bLife >= OKLIFE &&
				( ( pSoldier->bBreath >= OKBREATH ) || fIgnoreBreath ) )
			{
				if( !iChosenSoldier )
				{
					// ATE: This is to allow special handling of initial heli drop
					if ( !DidGameJustStart() )
					{
						TacticalCharacterDialogueWithSpecialEvent( pSoldier, QUOTE_ENEMY_PRESENCE, 0, 0, 0 );
					}
					return;
				}
				iChosenSoldier--;
			}
		}
	}
	else
	{ //There is either nobody here or our mercs can't talk

	}
}

void AddEnemiesToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT8 ubNumAdmins, UINT8 ubNumTroops, UINT8 ubNumElites, BOOLEAN fMagicallyAppeared )
{
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo;
	UINT8 ubCurrSlot;
	UINT8 ubTotalSoldiers;
	UINT8 bDesiredDirection=0;

#ifdef JA2UB
	UINT8	ubCnt;
#endif

	switch( ubStrategicInsertionCode )
	{
		case INSERTION_CODE_NORTH:	bDesiredDirection = SOUTHEAST;										break;
		case INSERTION_CODE_EAST:		bDesiredDirection = SOUTHWEST;										break;
		case INSERTION_CODE_SOUTH:	bDesiredDirection = NORTHWEST;										break;
		case INSERTION_CODE_WEST:		bDesiredDirection = NORTHEAST;										break;
		default:  AssertMsg( 0, "Illegal direction passed to AddEnemiesToBattle()" );	break;
	}
	#ifdef JA2TESTVERSION
		ScreenMsg( FONT_RED, MSG_INTERFACE, L"Enemy reinforcements have arrived!  (%d admins, %d troops, %d elite)", ubNumAdmins, ubNumTroops, ubNumElites );
	#endif

	if( fMagicallyAppeared )
	{ //update the strategic counters
		if( !gbWorldSectorZ )
		{
			SECTORINFO *pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
			pSector->ubNumAdmins			+= ubNumAdmins;
			pSector->ubAdminsInBattle	+= ubNumAdmins;
			pSector->ubNumTroops			+= ubNumTroops;
			pSector->ubTroopsInBattle	+= ubNumTroops;
			pSector->ubNumElites			+= ubNumElites;
			pSector->ubElitesInBattle	+= ubNumElites;
		}
		else
		{
			UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			if( pSector )
			{
				pSector->ubNumAdmins			+= ubNumAdmins;
				pSector->ubAdminsInBattle	+= ubNumAdmins;
				pSector->ubNumTroops			+= ubNumTroops;
				pSector->ubTroopsInBattle	+= ubNumTroops;
				pSector->ubNumElites			+= ubNumElites;
				pSector->ubElitesInBattle	+= ubNumElites;
			}
		}
		//Because the enemies magically appeared, have one of our soldiers say something...
		NotifyPlayersOfNewEnemies();
	}

	ubTotalSoldiers = ubNumAdmins + ubNumTroops + ubNumElites;
	
	#ifdef JA2UB
	if( gsGridNoForMapEdgePointInfo != -1 )
	{
		ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}
	#endif
	
	#ifdef JA2UB
	if( ubStrategicInsertionCode == INSERTION_CODE_GRIDNO )
	{
		if( gsGridNoForMapEdgePointInfo == -1 )
		{
			Assert( 0 );
			gsGridNoForMapEdgePointInfo=0;
		}

		for( ubCnt=0; ubCnt<32;ubCnt++)
		{
			MapEdgepointInfo.sGridNo[ ubCnt ] = gsGridNoForMapEdgePointInfo;
		}

		MapEdgepointInfo.ubNumPoints = 32;
		MapEdgepointInfo.ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}
	else
	{
		ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, (UINT8)(ubNumAdmins+ubNumElites+ubNumTroops) );
	}
	#else
	    ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, (UINT8)(ubNumAdmins+ubNumElites+ubNumTroops) );
	#endif
	
	
	ubCurrSlot = 0;
	while( ubTotalSoldiers )
	{
		if( ubNumElites && Random( ubTotalSoldiers ) < ubNumElites )
		{
			ubNumElites--;
			ubTotalSoldiers--;
			Assert(pSoldier = TacticalCreateEliteEnemy());
			if( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if( ubNumTroops && (UINT8)Random( ubTotalSoldiers ) < (UINT8)(ubNumElites + ubNumTroops) )
		{
			ubNumTroops--;
			ubTotalSoldiers--;
			Assert(pSoldier = TacticalCreateArmyTroop());
			if( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if( ubNumAdmins && (UINT8)Random( ubTotalSoldiers ) < (UINT8)(ubNumElites + ubNumTroops + ubNumAdmins) )
		{
			ubNumAdmins--;
			ubTotalSoldiers--;
			Assert(pSoldier = TacticalCreateAdministrator());
			if( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		// HEADROCK HAM 3.2: enemy reinforcements arrive with 0 APs.
		if (gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 1 || gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 2)
		{			
			pSoldier->bActionPoints = 0;

			// Flugente: due to a fix, also note here that the reinforcements get no APs.
			pSoldier->bSoldierFlagMask |= SOLDIER_NO_AP;
		}
	}
	
	#ifdef JA2UB
		gsGridNoForMapEdgePointInfo = -1;
	#endif
}




BOOLEAN SaveUnderGroundSectorInfoToSaveGame( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	UINT32	uiNumOfRecords=0;
	UNDERGROUND_SECTORINFO *TempNode = gpUndergroundSectorInfoHead;

	//Loop through all the nodes to count how many there are
	while( TempNode )
	{
		uiNumOfRecords++;
		TempNode = TempNode->next;
	}


	//Write how many nodes there are
	FileWrite( hFile, &uiNumOfRecords, sizeof( UINT32 ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( UINT32 ) )
	{
		return(FALSE);
	}

	TempNode = gpUndergroundSectorInfoHead;

	//Go through each node and save it.
	while( TempNode )
	{
		FileWrite( hFile, TempNode, sizeof( UNDERGROUND_SECTORINFO ), &uiNumBytesWritten );
		if( uiNumBytesWritten != sizeof( UNDERGROUND_SECTORINFO ) )
		{
			return(FALSE);
		}

		TempNode = TempNode->next;
	}

	return( TRUE );
}

BOOLEAN LoadUnderGroundSectorInfoFromSavedGame( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT32	uiNumOfRecords=0;
	UINT32	cnt=0;
	UNDERGROUND_SECTORINFO *TempNode = NULL;
	UNDERGROUND_SECTORINFO *TempSpot = NULL;

	//Clear the current LL
	TrashUndergroundSectorInfo();

	//Read in the number of nodes stored
	FileRead( hFile, &uiNumOfRecords, sizeof( UINT32 ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( UINT32 ) )
	{
		return(FALSE);
	}

	for( cnt = 0; cnt< uiNumOfRecords; cnt ++)
	{

		//Malloc space for the new node
		TempNode = (UNDERGROUND_SECTORINFO *) MemAlloc( sizeof( UNDERGROUND_SECTORINFO ) );
		if( TempNode == NULL )
			return( FALSE );


		//read in the new node
		FileRead( hFile, TempNode, sizeof( UNDERGROUND_SECTORINFO ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( UNDERGROUND_SECTORINFO ) )
		{
			return(FALSE);
		}

		//If its the first time in, assign the node to the list
		if( cnt == 0 )
		{
			gpUndergroundSectorInfoHead = TempNode;
			TempSpot = gpUndergroundSectorInfoHead;
			TempSpot->next = NULL;
		}
		else
		{
			//assign the new node to the LL
			TempSpot->next = TempNode;

			//advance to the next node
			TempSpot = TempSpot->next;
			TempSpot->next = NULL;
			gpUndergroundSectorInfoTail = TempSpot;
		}

	}

#ifdef JA2BETAVERSION
	if( !uiNumOfRecords )
	{
		BuildUndergroundSectorInfoList();
		gfClearCreatureQuest = TRUE;
	}
#endif

	return( TRUE );
}


UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ )
{
	UNDERGROUND_SECTORINFO *pUnderground;
	pUnderground = gpUndergroundSectorInfoHead;

	//Loop through all the underground sectors looking for specified sector
	while( pUnderground )
	{
		//If the sector is the right one
		if( pUnderground->ubSectorX == sMapX &&
			  pUnderground->ubSectorY == sMapY &&
				pUnderground->ubSectorZ == bMapZ )
		{
			return( pUnderground );
		}
		pUnderground = pUnderground->next;
	}


	return( NULL );
}




void BeginCaptureSquence( )
{
	if( !( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE ) || !( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE ) )
	{
		gStrategicStatus.ubNumCapturedForRescue = 0;
	}
}

void EndCaptureSequence( )
{
#if (defined JA2UB) 
// no UB
#else
     
	// Set flag...
	if( !( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE ) || !(gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE) )
	{
		// CJC Dec 1 2002: fixing multiple captures:
		//gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;

		if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTNOTSTARTED )
		{
			// CJC Dec 1 2002: fixing multiple captures:
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;
			StartQuest( QUEST_HELD_IN_ALMA, gWorldSectorX, gWorldSectorY );
		}
		// CJC Dec 1 2002: fixing multiple captures:
		//else if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTDONE )
		else if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTDONE && gubQuest[ QUEST_INTERROGATION ] == QUESTNOTSTARTED )
		{
			StartQuest( QUEST_INTERROGATION, gWorldSectorX, gWorldSectorY );
			// CJC Dec 1 2002: fixing multiple captures:
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE;

			// OK! - Schedule Meanwhile now!
			{
				MEANWHILE_DEFINITION MeanwhileDef;

				MeanwhileDef.sSectorX = 7;
				MeanwhileDef.sSectorY = 14;
				MeanwhileDef.ubNPCNumber = QUEEN;
				MeanwhileDef.usTriggerEvent = 0;
				MeanwhileDef.ubMeanwhileID = INTERROGATION;

				ScheduleMeanwhileEvent( &MeanwhileDef, 10 );
			}
		}
		// CJC Dec 1 2002: fixing multiple captures
		else
		{
			// !?!? set both flags
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE;
		}
	}
#endif
}

void EnemyCapturesPlayerSoldier( SOLDIERTYPE *pSoldier )
{
	UINT32					i;
	BOOLEAN       fMadeCorpse;
	INT32         iNumEnemiesInSector;

	// TODO.WANNE: Hardcoded grid number
	static INT32 sAlmaCaptureGridNos[] = { 9208, 9688, 9215 }; //dnl!!!
	static INT32 sAlmaCaptureItemsGridNo[] = { 12246, 12406, 13046 }; //dnl!!!

	static INT32 sInterrogationItemGridNo[] = { 12089, 12089, 12089 }; //dnl!!!

	AssertNotNIL(pSoldier);

	// ATE: Check first if ! in player captured sequence already
	// CJC Dec 1 2002: fixing multiple captures
	if ( ( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE ) && (gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE) )
	{
		return;
	}

	// ATE: If maximum prisoners captured, return!
	if ( gStrategicStatus.ubNumCapturedForRescue > 3 )
	{
	return;
	}


  // If this is an EPC , just kill them...
  if ( AM_AN_EPC( pSoldier ) )
  {
	  pSoldier->stats.bLife = 0;
	  pSoldier->iHealableInjury = 0; // added by SANDRO
    HandleSoldierDeath( pSoldier, &fMadeCorpse );
    return;
  }

  if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
  {
    return;
  }

  // ATE: Patch fix If in a vehicle, remove from vehicle...
  TakeSoldierOutOfVehicle( pSoldier );


  // Are there anemies in ALMA? ( I13 )
  iNumEnemiesInSector = NumEnemiesInSector( 13, 9 );

  // IF there are no enemies, and we need to do alma, skip!
  if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTNOTSTARTED && iNumEnemiesInSector == 0 )
  {
	  InternalStartQuest( QUEST_HELD_IN_ALMA, gWorldSectorX, gWorldSectorY, FALSE );
	  InternalEndQuest( QUEST_HELD_IN_ALMA, gWorldSectorX, gWorldSectorY, FALSE );
  }

	HandleMoraleEvent( pSoldier, MORALE_MERC_CAPTURED, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

	// Change to POW....
	//-add him to a POW assignment/group
	if( ( pSoldier->bAssignment != ASSIGNMENT_POW )  )
	{
		SetTimeOfAssignmentChangeForMerc( pSoldier );
	}

	ChangeSoldiersAssignment( pSoldier, ASSIGNMENT_POW );
	// ATE: Make them neutral!
	if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTNOTSTARTED )
	{
		pSoldier->aiData.bNeutral = TRUE;
	}

	RemoveCharacterFromSquads( pSoldier );

	WORLDITEM			WorldItem;

	// Is this the first one..?
	if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTNOTSTARTED )
	{
		//-teleport him to NE Alma sector (not Tixa as originally planned)
		pSoldier->sSectorX = 13;
		pSoldier->sSectorY = 9;
		pSoldier->bSectorZ = 0;

		// put him on the floor!!
		pSoldier->pathing.bLevel = 0;

		// OK, drop all items!
		for ( i = 0; i < pSoldier->inv.size(); i++ )
		{
			if( pSoldier->inv[ i ].exists() == true )
			{
				WorldItem.fExists = TRUE;
				WorldItem.sGridNo = sAlmaCaptureItemsGridNo[ gStrategicStatus.ubNumCapturedForRescue ];
				WorldItem.ubLevel = 0;
				WorldItem.usFlags = 0;
				WorldItem.bVisible = FALSE;
				WorldItem.bRenderZHeightAboveLevel = 0;
				pSoldier->inv[ i ].MoveThisObjectTo(WorldItem.object);

				AddWorldItemsToUnLoadedSector( 13, 9, 0, sAlmaCaptureItemsGridNo[ gStrategicStatus.ubNumCapturedForRescue ], 1, &WorldItem, FALSE );
			}
		}

		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = sAlmaCaptureGridNos[ gStrategicStatus.ubNumCapturedForRescue ];

		gStrategicStatus.ubNumCapturedForRescue++;
	}
	else if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTDONE )
	{
		//-teleport him to N7
		pSoldier->sSectorX = 7;
		pSoldier->sSectorY = 14;
		pSoldier->bSectorZ = 0;

		// put him on the floor!!
		pSoldier->pathing.bLevel = 0;

		// OK, drop all items!
		for ( i = 0; i < pSoldier->inv.size(); i++ )
		{
			if( pSoldier->inv[ i ].exists() == true )
			{
				WorldItem.fExists = TRUE;
				WorldItem.sGridNo = sInterrogationItemGridNo[ gStrategicStatus.ubNumCapturedForRescue ];
				WorldItem.ubLevel = 0;
				WorldItem.usFlags = 0;
				WorldItem.bVisible = FALSE;
				WorldItem.bRenderZHeightAboveLevel = 0;
				pSoldier->inv[ i ].MoveThisObjectTo(WorldItem.object);

				AddWorldItemsToUnLoadedSector( 7, 14, 0, sInterrogationItemGridNo[ gStrategicStatus.ubNumCapturedForRescue ], 1, &WorldItem, FALSE );
			}
		}

		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = gsInterrogationGridNo[ gStrategicStatus.ubNumCapturedForRescue ];

		gStrategicStatus.ubNumCapturedForRescue++;
	}

	//Bandaging him would prevent him from dying (due to low HP)
	pSoldier->bBleeding = 0;
	pSoldier->bPoisonBleeding = 0;

	// wake him up
	if ( pSoldier->flags.fMercAsleep )
	{
		PutMercInAwakeState( pSoldier );
		pSoldier->flags.fForcedToStayAwake = FALSE;
	}

	//Set his life to 50% + or - 10 HP.
	INT8 oldlife = pSoldier->stats.bLife;
	pSoldier->stats.bLife = pSoldier->stats.bLifeMax / 2;
	if ( pSoldier->stats.bLife <= 35 )
	{
		pSoldier->stats.bLife = 35;
	}
	else if ( pSoldier->stats.bLife >= 45 )
	{
		pSoldier->stats.bLife += (INT8)(10 - Random( 21 ) );
	}

	// adjust poison points
	INT8 lifechanged = oldlife - pSoldier->stats.bLife;
	if ( lifechanged < 0 )
		pSoldier->bPoisonLife -= lifechanged;
	else if ( lifechanged > 0)
		pSoldier->bPoisonLife = max(pSoldier->bPoisonSum, pSoldier->bPoisonLife + lifechanged);

	// SANDRO - make the lost life insta-healable
	pSoldier->iHealableInjury = ((pSoldier->stats.bLifeMax - pSoldier->stats.bLife) * 100);

	// make him quite exhausted when found
	pSoldier->bBreath = pSoldier->bBreathMax = 50;
	pSoldier->sBreathRed = 0;
	pSoldier->flags.fMercCollapsedFlag = FALSE;
}


void HandleEnemyStatusInCurrentMapBeforeLoadingNewMap()
{
	INT32 i;
	BOOLEAN fMadeCorpse;
	INT8 bKilledEnemies = 0, bKilledCreatures = 0, bKilledRebels = 0, bKilledCivilians = 0;
	return;
	//If any of the soldiers are dying, kill them now.
	for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife < OKLIFE && MercPtrs[ i ]->stats.bLife )
		{
			MercPtrs[ i ]->stats.bLife = 0;
			MercPtrs[ i ]->iHealableInjury = 0; // added just for sure - SANDRO
			HandleSoldierDeath( MercPtrs[ i ], &fMadeCorpse );
			bKilledEnemies++;
		}
	}
	//Do the same for the creatures.
	for( i = gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife < OKLIFE && MercPtrs[ i ]->stats.bLife )
		{
			MercPtrs[ i ]->stats.bLife = 0;
			HandleSoldierDeath( MercPtrs[ i ], &fMadeCorpse );
			bKilledCreatures++;
		}
	}
	//Militia
	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife < OKLIFE && MercPtrs[ i ]->stats.bLife )
		{
			MercPtrs[ i ]->stats.bLife = 0;
			MercPtrs[ i ]->iHealableInjury = 0; // added just for sure - SANDRO
			HandleSoldierDeath( MercPtrs[ i ], &fMadeCorpse );
			bKilledRebels++;
		}
	}
	//Civilians
	for( i = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->stats.bLife < OKLIFE && MercPtrs[ i ]->stats.bLife )
		{
			MercPtrs[ i ]->stats.bLife = 0;
			MercPtrs[ i ]->iHealableInjury = 0; // added just for sure - SANDRO
			HandleSoldierDeath( MercPtrs[ i ], &fMadeCorpse );
			bKilledCivilians++;
		}
	}

// TEST MESSAGES ONLY!
	if( bKilledCivilians )
		ScreenMsg( FONT_BLUE, MSG_TESTVERSION, L"%d civilians died after you left the sector.", bKilledCivilians );
	if( bKilledRebels )
		ScreenMsg( FONT_BLUE, MSG_TESTVERSION, L"%d militia died after you left the sector.", bKilledRebels );
	if( bKilledEnemies )
		ScreenMsg( FONT_BLUE, MSG_TESTVERSION, L"%d enemies died after you left the sector.", bKilledEnemies );
	if( bKilledCreatures )
		ScreenMsg( FONT_BLUE, MSG_TESTVERSION, L"%d creatures died after you left the sector.", bKilledCreatures );

	if( !gbWorldSectorZ )
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR(gWorldSectorX, gWorldSectorY) ];
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
		pSector->ubCreaturesInBattle = 0;
		//RecalculateSectorWeight(
	}
	else if( gbWorldSectorZ > 0 )
	{
		UNDERGROUND_SECTORINFO *pSector;
		pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		if( !pSector )
			return;
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
		pSector->ubCreaturesInBattle = 0;
	}
}

BOOLEAN PlayerSectorDefended( UINT8 ubSectorID )
{
	SECTORINFO *pSector;
	pSector = &SectorInfo[ ubSectorID ];
	if( pSector->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] +
			pSector->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] +
			pSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] )
	{ //militia in sector
		return TRUE;
	}
	if( FindMovementGroupInSector( (UINT8)SECTORX( ubSectorID ), (UINT8)SECTORY( ubSectorID ), TRUE ) )
	{ //player in sector
		return TRUE;
	}
	return FALSE;
}

//Assumes gTacticalStatus.fEnemyInSector
BOOLEAN OnlyHostileCivsInSector()
{
	SOLDIERTYPE *pSoldier;
	INT32 i;
	BOOLEAN fHostileCivs = FALSE;

	//Look for any hostile civs.
	for( i = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife )
		{
			if( !pSoldier->aiData.bNeutral )
			{
				fHostileCivs = TRUE;
				break;
			}
		}
	}
	if( !fHostileCivs )
	{ //No hostile civs, so return FALSE
		return FALSE;
	}
	//Look for anybody else hostile.  If found, return FALSE immediately.
	for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife )
		{
			if( !pSoldier->aiData.bNeutral )
			{
				return FALSE;
			}
		}
	}
	for( i = gTacticalStatus.Team[ CREATURE_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife )
		{
			if( !pSoldier->aiData.bNeutral )
			{
				return FALSE;
			}
		}
	}
	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; i++ )
	{
		pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife )
		{
			if( !pSoldier->aiData.bNeutral )
			{
				return FALSE;
			}
		}
	}
	//We only have hostile civilians, don't allow time compression.
	return TRUE;
}

BOOLEAN CheckPendingEnemies()
{
	if( gbWorldSectorZ ) return FALSE;
	SECTORINFO *pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
	if( (pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins) > ( pSector->ubElitesInBattle + pSector->ubTroopsInBattle + pSector->ubAdminsInBattle) )
		return TRUE;
	for (GROUP *pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if( !pGroup->fPlayer 
			&& !pGroup->fVehicle 
			&& pGroup->ubSectorX == gWorldSectorX 
			&& pGroup->ubSectorY == gWorldSectorY
			&& pGroup->ubGroupSize > pGroup->pEnemyGroup->ubElitesInBattle + pGroup->pEnemyGroup->ubTroopsInBattle + pGroup->pEnemyGroup->ubAdminsInBattle)
			return TRUE;
	}
	return FALSE;
}

#ifdef JA2UB
void HandleBloodCatDeaths( SECTORINFO *pSector )
{
	//if the current sector is the first part of the town
	if( gWorldSectorX == 10 && gWorldSectorY == 9 && gbWorldSectorZ == 0 )
	{
		//if ALL the bloodcats are killed
		if( pSector->bBloodCats == 0 )
		{
			UINT8 bId1=-1;
			UINT8 bId2=-1;
			UINT8 bNum=0;

			SetFactTrue( FACT_PLAYER_KILLED_ALL_BETTYS_BLOODCATS );

			//Instantly have betties missing items show up
			//DailyCheckOnItemQuantities( TRUE );

			// Now have a merc say the killed bloodcat quote
			bNum = Get3RandomQualifiedMercs( &bId1, &bId2, NULL );

			//if there are some qualified mercs
			if( bNum != 0 )
			{
				//must make sure TEX doesnt say the quote
				if( bId1 != NOBODY && Menptr[ bId1 ].ubProfile != TEX_UB )
				{
					TacticalCharacterDialogue( &Menptr[ bId1 ], QUOTE_RENEW_REFUSAL_DUE_TO_LACK_OF_FUNDS );
				}
				else if( bId2 != NOBODY && Menptr[ bId2 ].ubProfile != TEX_UB )
				{
					TacticalCharacterDialogue( &Menptr[ bId2 ], QUOTE_RENEW_REFUSAL_DUE_TO_LACK_OF_FUNDS );
				}
			}
		}
	}
}
#endif
