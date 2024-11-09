//Queen Command.c

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
	#include "CampaignStats.h"		// added by Flugente
	#include "ASD.h"				// added by Flugente
	#include "Interface Panels.h"
	#include "Strategic Transport Groups.h"

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

#include "GameInitOptionsScreen.h"

#include <vector>

//The sector information required for the strategic AI.  Contains the number of enemy troops,
//as well as intentions, etc.
//SECTORINFO SectorInfo[256];
std::vector<SECTORINFO> SectorInfo (256);

UNDERGROUND_SECTORINFO *gpUndergroundSectorInfoHead = NULL;
extern UNDERGROUND_SECTORINFO* gpUndergroundSectorInfoTail;
BOOLEAN gfPendingNonPlayerTeam[PLAYER_PLAN] = {FALSE, FALSE, FALSE, FALSE, FALSE};
UINT32 guiTurnCnt = 0, guiReinforceTurn = 0, guiArrived = 0;//dnl ch68 080913
extern void BuildUndergroundSectorInfoList();

extern void EndCreatureQuest();

extern GARRISON_GROUP *gGarrisonGroup;
extern INT32 giGarrisonArraySize;

#ifdef JA2TESTVERSION
extern BOOLEAN gfOverrideSector;
#endif

//Buggler: Externalized to gModSettings.iMeanwhileInterrogatePOWGridNo
//INT32 gsInterrogationGridNo[3] = { 7756, 7757, 7758 };

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

		for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; ++i )
		{
			pSoldier = MercPtrs[ i ];
			if( !pSoldier->bActive || !pSoldier->bInSector )
			{
				continue;
			}

			// Flugente: zombies are fine with having no weapons...
			if ( pSoldier->IsZombie() )
				continue;

			if( !pSoldier->inv[ HANDPOS ].usItem )
			{
				++iNumInvalid;
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
UINT16 NumHostilesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT16 ubNumHostiles = 0;

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
			ubNumHostiles = (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumCreatures + pSector->ubNumRobots + pSector->ubNumJeeps + pSector->ubNumTanks);
		}
	}
	else
	{
		SECTORINFO *pSector;
		GROUP *pGroup;

		//Count stationary hostiles
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubNumHostiles = (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumCreatures + pSector->ubNumRobots + pSector->ubNumJeeps + pSector->ubNumTanks);

		//Count mobile enemies
		pGroup = gpGroupList;
		while( pGroup )
		{
			if ( pGroup->usGroupTeam != OUR_TEAM && pGroup->usGroupTeam != MILITIA_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
			{
				ubNumHostiles += pGroup->ubGroupSize;
			}
			pGroup = pGroup->next;
		}
	}

	return ubNumHostiles;
}

UINT16 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT16 ubNumEnemies = 0;

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
			ubNumEnemies = (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumJeeps + pSector->ubNumTanks);
		}
	}
	else
	{
		SECTORINFO *pSector;
		GROUP *pGroup;

		//Count stationary enemies
		pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
		ubNumEnemies = (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumJeeps + pSector->ubNumTanks);

		//Count mobile enemies
		pGroup = gpGroupList;
		while( pGroup )
		{
			if ( pGroup->usGroupTeam != OUR_TEAM && pGroup->usGroupTeam != MILITIA_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
			{
				ubNumEnemies += pGroup->ubGroupSize;
			}
			pGroup = pGroup->next;
		}
	}

	return ubNumEnemies;
}

// returns how many members of a team are in a sector - not intended for OUR_TEAM!
UINT16 NumNonPlayerTeamMembersInSector( INT16 sSectorX, INT16 sSectorY, UINT8 ubTeam )
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	UINT16 ubNumTroops = 0;

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

	if ( ubTeam == ENEMY_TEAM )
	{
		ubNumTroops = (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumJeeps + pSector->ubNumTanks);
	
		if ( is_networked )
			ubNumTroops += numenemyLAN((UINT8)sSectorX,(UINT8)sSectorY ); //hayden
	}
	else if ( ubTeam == MILITIA_TEAM )
	{
		ubNumTroops = (pSector->ubNumberOfCivsAtLevel[0] + pSector->ubNumberOfCivsAtLevel[1] + pSector->ubNumberOfCivsAtLevel[2]);
	}
	else if ( ubTeam == CREATURE_TEAM )
	{
		ubNumTroops = pSector->ubNumCreatures;
	}

	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam == ubTeam && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			ubNumTroops += pGroup->ubGroupSize;
		}
		pGroup = pGroup->next;
	}

	return ubNumTroops;
}

// returns how many members of a team are in a sector - only intended for OUR_TEAM! POWs not included
UINT16 NumPlayerTeamMembersInSector( INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ )
{
	UINT16 teammemberspresent = 0;

	SOLDIERTYPE*		pTeamSoldier = NULL;
	UINT16				bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT16				bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	for ( pTeamSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, pTeamSoldier++ )
	{
		// we test several conditions before we allow adding an opinion
		// other merc must be active, have a profile, be someone else and not be in transit or dead
		if ( pTeamSoldier->bActive && !pTeamSoldier->flags.fBetweenSectors  && pTeamSoldier->stats.bLife > 0 && !(pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) &&
			 !(pTeamSoldier->bAssignment == IN_TRANSIT || pTeamSoldier->bAssignment == ASSIGNMENT_DEAD || pTeamSoldier->bAssignment == ASSIGNMENT_POW || pTeamSoldier->bAssignment == ASSIGNMENT_MINIEVENT || pTeamSoldier->bAssignment == ASSIGNMENT_REBELCOMMAND) &&
			 (pTeamSoldier->sSectorX == sSectorX && pTeamSoldier->sSectorY == sSectorY && pTeamSoldier->bSectorZ == sSectorZ) )
		{
			++teammemberspresent;
		}
	}

	return teammemberspresent;
}

UINT16 NumEnemyArmedVehiclesInSector( INT16 sSectorX, INT16 sSectorY, UINT8 usTeam )
{
	SECTORINFO *pSector;
	GROUP *pGroup;
	UINT16 ubNum = 0;

	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
	// probably isn't. But I need this bit to work.
	if ( sSectorX < MINIMUM_VALID_X_COORDINATE ||
		 sSectorX > MAXIMUM_VALID_X_COORDINATE ||
		 sSectorY < MINIMUM_VALID_Y_COORDINATE ||
		 sSectorY > MAXIMUM_VALID_Y_COORDINATE )
	{
		return (0);
	}

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE );
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE );
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];

	if ( usTeam == ENEMY_TEAM )
		ubNum = (UINT16)(pSector->ubNumTanks + pSector->ubNumJeeps);
	
	pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == usTeam && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			ubNum += pGroup->pEnemyGroup->ubNumTanks + pGroup->pEnemyGroup->ubNumJeeps;
		}
		pGroup = pGroup->next;
	}

	return ubNum;
}

UINT16 NumStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
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

	return (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumTanks + pSector->ubNumJeeps + pSector->ubNumRobots);
}

UINT16 NumMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY )
{
	GROUP *pGroup;
	SECTORINFO *pSector;
	UINT16 ubNumTroops;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	ubNumTroops = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam != OUR_TEAM && pGroup->usGroupTeam != MILITIA_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			ubNumTroops += pGroup->ubGroupSize;
		}
		pGroup = pGroup->next;
	}

	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
	if( pSector->ubGarrisonID == ROADBLOCK )
	{
		//consider these troops as mobile troops even though they are in a garrison
		ubNumTroops += (pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumJeeps + pSector->ubNumTanks);
	}

	return ubNumTroops;
}

void GetNumberOfMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps )
{
	GROUP *pGroup;
	SECTORINFO *pSector;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	//Now count the number of mobile groups in the sector.
	*pubNumTroops = *pubNumElites = *pubNumAdmins = *pubNumRobots = *pubNumTanks = *pubNumJeeps = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam != OUR_TEAM && pGroup->usGroupTeam != MILITIA_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			*pubNumTroops += pGroup->pEnemyGroup->ubNumTroops;
			*pubNumElites += pGroup->pEnemyGroup->ubNumElites;
			*pubNumAdmins += pGroup->pEnemyGroup->ubNumAdmins;
			*pubNumRobots += pGroup->pEnemyGroup->ubNumRobots;
			*pubNumTanks += pGroup->pEnemyGroup->ubNumTanks;
			*pubNumJeeps += pGroup->pEnemyGroup->ubNumJeeps;
		}
		pGroup = pGroup->next;
	}

	pSector = &SectorInfo[ SECTOR( sSectorX, sSectorY ) ];
	if( pSector->ubGarrisonID == ROADBLOCK )
	{
		//consider these troops as mobile troops even though they are in a garrison
		*pubNumAdmins += pSector->ubNumAdmins;
		*pubNumTroops += pSector->ubNumTroops;
		*pubNumElites += pSector->ubNumElites;
		*pubNumRobots += pSector->ubNumRobots;
		*pubNumTanks += pSector->ubNumTanks;
		*pubNumJeeps += pSector->ubNumJeeps;
	}
}

void GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( INT16 sSectorX, INT16 sSectorY, UINT16 usTeam, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps )
{
	GROUP *pGroup;
	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE);
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE);
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	//Now count the number of mobile groups in the sector.
	*pubNumTroops = *pubNumElites = *pubNumAdmins = *pubNumRobots = *pubNumTanks = *pubNumJeeps = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if ( pGroup->usGroupTeam == usTeam && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			*pubNumTroops += pGroup->pEnemyGroup->ubNumTroops;
			*pubNumElites += pGroup->pEnemyGroup->ubNumElites;
			*pubNumAdmins += pGroup->pEnemyGroup->ubNumAdmins;
			*pubNumRobots += pGroup->pEnemyGroup->ubNumRobots;
			*pubNumTanks += pGroup->pEnemyGroup->ubNumTanks;
			*pubNumJeeps += pGroup->pEnemyGroup->ubNumJeeps;
		}
		pGroup = pGroup->next;
	}
}


void GetNumberOfStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps )
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
	*pubNumRobots = pSector->ubNumRobots;
	*pubNumTanks = pSector->ubNumTanks;
	*pubNumJeeps = pSector->ubNumJeeps;
}

void GetNumberOfEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites, UINT16 *pubNumRobots, UINT16 *pubNumTanks, UINT16 *pubNumJeeps )
{
	UINT16 ubNumAdmins, ubNumTroops, ubNumElites, ubNumRobots, ubNumTanks, ubNumJeeps;

	GetNumberOfStationaryEnemiesInSector( sSectorX, sSectorY, pubNumAdmins, pubNumTroops, pubNumElites, pubNumRobots, pubNumTanks, pubNumJeeps );

	GetNumberOfMobileEnemiesInSector( sSectorX, sSectorY, &ubNumAdmins, &ubNumTroops, &ubNumElites, &ubNumRobots, &ubNumTanks, &ubNumJeeps );

	*pubNumAdmins += ubNumAdmins;
	*pubNumTroops += ubNumTroops;
	*pubNumElites += ubNumElites;
	*pubNumRobots += ubNumRobots;
	*pubNumTanks += ubNumTanks;
	*pubNumJeeps += ubNumJeeps;
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
		pSector->ubRobotsInBattle = 0;
		pSector->ubTanksInBattle = 0;
		pSector->ubJeepsInBattle = 0;
	}
	else if( !gbWorldSectorZ )
	{
		SECTORINFO *pSector;
		pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];
		//grab the number of each type in the stationary sector
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
		pSector->ubRobotsInBattle = 0;
		pSector->ubTanksInBattle = 0;
		pSector->ubJeepsInBattle = 0;
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
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY )
		{
			pGroup->pEnemyGroup->ubTroopsInBattle = 0;
			pGroup->pEnemyGroup->ubElitesInBattle = 0;
			pGroup->pEnemyGroup->ubRobotsInBattle = 0;
			pGroup->pEnemyGroup->ubAdminsInBattle = 0;
			pGroup->pEnemyGroup->ubTanksInBattle = 0;
			pGroup->pEnemyGroup->ubJeepsInBattle = 0;
		}
		pGroup = pGroup->next;
	}


	//Check to see if any of our mercs have abandoned the militia during a battle.  This is cause for a rather
	//severe loyalty blow.
	for( i = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; i <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; ++i )
	{
		if( MercPtrs[ i ]->bActive && MercPtrs[ i ]->bInSector && MercPtrs[ i ]->stats.bLife >= OKLIFE )
		{ //found one live militia, so look for any enemies/creatures.
			// NOTE: this is relying on ENEMY_TEAM being immediately followed by CREATURE_TEAM
			for( i = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; i <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID; ++i )
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

UINT16 NumFreeSlots( UINT8 ubTeam )
{
	UINT16 ubNumFreeSlots = 0;

	//Count the number of free enemy slots.  It is possible to have multiple groups exceed the maximum.
	for ( INT32 i = gTacticalStatus.Team[ubTeam].bFirstID; i <= gTacticalStatus.Team[ubTeam].bLastID; ++i )
	{
		if ( !Menptr[i].bActive )
			++ubNumFreeSlots;
	}

	// the militia team size can be restricted by the ini
	if ( ubTeam == MILITIA_TEAM )
	{
		ubNumFreeSlots = (UINT16)max( 0, (INT16)ubNumFreeSlots - (INT16)(gGameExternalOptions.ubGameMaximumNumberOfRebels - gGameExternalOptions.iMaxMilitiaPerSector) );
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
	unsigned ubNumAdmins, ubNumTroops, ubNumElites, ubNumRobots, ubNumTanks, ubNumJeeps;
	unsigned ubTotalAdmins, ubTotalElites, ubTotalRobots, ubTotalTroops, ubTotalTanks, ubTotalJeeps;
	unsigned totalCountOfStationaryEnemies = 0;
	unsigned totalCountOfMobileEnemies = 0;
	int sNumSlots;

	gfPendingNonPlayerTeam[ENEMY_TEAM] = FALSE;

	// rftr: clear cached transport groups
	ClearTransportGroupMap();

	if( gbWorldSectorZ > 0 )
		return PrepareEnemyForUndergroundBattle();

	// Add the invading group
	if ( gpBattleGroup && gpBattleGroup->usGroupTeam == ENEMY_TEAM )
	{
		//The enemy has instigated the battle which means they are the ones entering the conflict.
		//The player was actually in the sector first, and the enemy doesn't use reinforced placements
		HandleArrivalOfReinforcements( gpBattleGroup );

		// Reinforcement groups?  Bring it on!
		// Omerta has a special setting
		if ( gGameExternalOptions.gfAllowReinforcements &&
			( (GetTownIdForSector( gWorldSectorX, gWorldSectorY ) != OMERTA) || zDiffSetting[gGameOptions.ubDifficultyLevel].bAllowReinforcementsOmerta ) )
		{
			UINT16 pusMoveDir[4][3];
			UINT8 ubDirNumber = 0;
			GROUP *pGroup;
			SECTORINFO *pThisSector;

			// They arrived in multiple groups, so here they come
			pThisSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

			GenerateDirectionInfos( gWorldSectorX, gWorldSectorY, &ubDirNumber, pusMoveDir,	FALSE, TRUE );

			for( unsigned ubIndex = 0; ubIndex < ubDirNumber; ++ubIndex )
			{
				while ( NumMobileEnemiesInSector( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ) ) && GetNonPlayerGroupInSectorForReinforcement( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), ENEMY_TEAM ) )
				{
					pGroup = GetNonPlayerGroupInSectorForReinforcement( SECTORX( pusMoveDir[ubIndex][0] ), SECTORY( pusMoveDir[ubIndex][0] ), ENEMY_TEAM );

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
				if ( pGroup != gpBattleGroup && pGroup->usGroupTeam != OUR_TEAM && !pGroup->fVehicle &&
					  pGroup->ubSectorX == gpBattleGroup->ubSectorX &&
						pGroup->ubSectorY == gpBattleGroup->ubSectorY &&
						!pGroup->pEnemyGroup->ubAdminsInBattle &&
						!pGroup->pEnemyGroup->ubTroopsInBattle &&
						!pGroup->pEnemyGroup->ubElitesInBattle &&
						!pGroup->pEnemyGroup->ubRobotsInBattle &&
						!pGroup->pEnemyGroup->ubTanksInBattle &&
						!pGroup->pEnemyGroup->ubJeepsInBattle )
				{
					HandleArrivalOfReinforcements( pGroup );
				}

				// for transport groups, track how many enemies of each type we're adding so we can update drops for them
				if (pGroup->usGroupTeam == ENEMY_TEAM && pGroup->pEnemyGroup->ubIntention == TRANSPORT && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ)
				{
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ADMINISTRATOR, pGroup->pEnemyGroup->ubNumAdmins);
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ARMY, pGroup->pEnemyGroup->ubNumTroops);
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ELITE, pGroup->pEnemyGroup->ubNumElites);
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ROBOT, pGroup->pEnemyGroup->ubNumRobots);
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_JEEP, pGroup->pEnemyGroup->ubNumJeeps);
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_TANK, pGroup->pEnemyGroup->ubNumTanks);
				}

				pGroup = pGroup->next;
			}
		}

		UpdateTransportGroupInventory();

		ValidateEnemiesHaveWeapons();
		UnPauseGame();
		return ( ( BOOLEAN) ( gpBattleGroup->ubGroupSize > 0 ) );
	}

	// WDS Count the number of placements.  This will limit the maximum number of enemies we can place on the map
	SOLDIERINITNODE *curr = gSoldierInitHead;
	unsigned mapMaximumNumberOfEnemies = 0;
	while( curr )
	{
		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			++mapMaximumNumberOfEnemies;
		}
		curr = curr->next;
	}

	AssertGE (mapMaximumNumberOfEnemies, 0);
	
	if (mapMaximumNumberOfEnemies > gGameExternalOptions.ubGameMaximumNumberOfEnemies)
		mapMaximumNumberOfEnemies = gGameExternalOptions.ubGameMaximumNumberOfEnemies;
	//is there more enemies in the sector then we can actually place on map? 
	//are there more enemies in the sector then we can actually place on map? 
	gfPendingNonPlayerTeam[ENEMY_TEAM] = (NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, ENEMY_TEAM ) > mapMaximumNumberOfEnemies);

	pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

	// OJW - 20090403 - override max ai in co-op
	if (is_networked && is_server && gMaxEnemiesEnabled == 1)
	{
		float totalEnemies = (float)(pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumRobots + pSector->ubNumTanks + pSector->ubNumJeeps);
		ubTotalAdmins = (unsigned int)(((float)pSector->ubNumAdmins / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalTroops = (unsigned int)(((float)pSector->ubNumTroops / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalElites = (unsigned int)(((float)pSector->ubNumElites / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalRobots = (unsigned int)(((float)pSector->ubNumRobots / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalTanks = (unsigned int)(((float)pSector->ubNumTanks / totalEnemies) * mapMaximumNumberOfEnemies);
		ubTotalJeeps = (unsigned int)(((float)pSector->ubNumJeeps / totalEnemies) * mapMaximumNumberOfEnemies);
		totalEnemies = (float)(ubTotalAdmins + ubTotalTroops + ubTotalElites + ubTotalRobots + ubTotalTanks + ubTotalJeeps);

		// take care of any rounding losses
		while (totalEnemies < mapMaximumNumberOfEnemies)
		{
			// just fill out with standard troops
			++ubTotalTroops;
			++totalEnemies;
		}

		pSector->ubNumAdmins = ubTotalAdmins;
		pSector->ubNumTroops = ubTotalTroops;
		pSector->ubNumElites = ubTotalElites;
		pSector->ubNumRobots = ubTotalRobots;
		pSector->ubNumTanks = ubTotalTanks;
		pSector->ubNumJeeps = ubTotalJeeps;
		pSector->ubAdminsInBattle = 0;
		pSector->ubTroopsInBattle = 0;
		pSector->ubElitesInBattle = 0;
		pSector->ubRobotsInBattle = 0;
		pSector->ubTanksInBattle = 0;
		pSector->ubJeepsInBattle = 0;
	}
	else
	{
		if( pSector->uiFlags & SF_USE_MAP_SETTINGS )
		{
			//count the number of enemy placements in a map and use those
			SOLDIERINITNODE *curr = gSoldierInitHead;
			ubTotalAdmins = ubTotalTroops = ubTotalElites = ubTotalRobots = ubTotalJeeps = ubTotalTanks = 0;
			while( curr )
			{
				if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
				{
					switch( curr->pBasicPlacement->ubSoldierClass )
					{
						case SOLDIER_CLASS_ADMINISTRATOR:
						case SOLDIER_CLASS_BANDIT:				ubTotalAdmins++;	break;
						case SOLDIER_CLASS_ARMY:				ubTotalTroops++;	break;
						case SOLDIER_CLASS_ELITE:				ubTotalElites++;	break;
						case SOLDIER_CLASS_ROBOT:				ubTotalRobots++;	break;
						case SOLDIER_CLASS_TANK:				ubTotalTanks++;		break;
						case SOLDIER_CLASS_JEEP:				ubTotalJeeps++;		break;
					}
				}
				curr = curr->next;
			}
			pSector->ubNumAdmins = ubTotalAdmins;
			pSector->ubNumTroops = ubTotalTroops;
			pSector->ubNumElites = ubTotalElites;
			pSector->ubNumRobots = ubTotalRobots;
			pSector->ubNumTanks = ubTotalTanks;
			pSector->ubNumJeeps = ubTotalJeeps;
			pSector->ubAdminsInBattle = 0;
			pSector->ubTroopsInBattle = 0;
			pSector->ubElitesInBattle = 0;
			pSector->ubRobotsInBattle = 0;
			pSector->ubTanksInBattle = 0;
			pSector->ubJeepsInBattle = 0;
		}
		else
		{
			ubTotalAdmins = pSector->ubNumAdmins - pSector->ubAdminsInBattle;
			ubTotalTroops = pSector->ubNumTroops - pSector->ubTroopsInBattle;
			ubTotalElites = pSector->ubNumElites - pSector->ubElitesInBattle;
			ubTotalRobots = pSector->ubNumRobots - pSector->ubRobotsInBattle;
			ubTotalTanks  = pSector->ubNumTanks  - pSector->ubTanksInBattle;
			ubTotalJeeps = pSector->ubNumJeeps - pSector->ubJeepsInBattle;
		}
	}

	totalCountOfStationaryEnemies = ubTotalAdmins + ubTotalTroops + ubTotalElites + ubTotalRobots + ubTotalTanks + ubTotalJeeps;

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
		ubTotalRobots = min( mapMaximumNumberOfEnemies-ubTotalAdmins-ubTotalTroops-ubTotalElites, ubTotalRobots );
		ubTotalTanks  = min( mapMaximumNumberOfEnemies-ubTotalAdmins-ubTotalTroops-ubTotalElites-ubTotalRobots, ubTotalTanks );
		ubTotalJeeps  = min( mapMaximumNumberOfEnemies-ubTotalAdmins-ubTotalTroops-ubTotalElites-ubTotalRobots-ubTotalTanks, ubTotalJeeps );
	}

	pSector->ubAdminsInBattle += ubTotalAdmins;
	pSector->ubTroopsInBattle += ubTotalTroops;
	pSector->ubElitesInBattle += ubTotalElites;
	pSector->ubRobotsInBattle += ubTotalRobots;
	pSector->ubTanksInBattle += ubTotalTanks;
	pSector->ubJeepsInBattle += ubTotalJeeps;

#ifdef JA2TESTVERSION
	if( gfOverrideSector )
	{
		//if there are no troops in the current groups, then we're done.
		if ( !ubTotalAdmins && !ubTotalTroops && !ubTotalElites && !ubTotalTanks && !ubTotalJeeps && !ubTotalRobots)
			return FALSE;
		AddSoldierInitListEnemyDefenceSoldiers( ubTotalAdmins, ubTotalTroops, ubTotalElites, ubTotalTanks, ubTotalJeeps, ubTotalRobots );
		ValidateEnemiesHaveWeapons();
		UnPauseGame();
		return TRUE;
	}
#endif

	//Search for movement groups that happen to be in the sector.
	sNumSlots = NumFreeSlots( ENEMY_TEAM );
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
	sNumSlots -= ubTotalAdmins + ubTotalTroops + ubTotalElites + ubTotalRobots + ubTotalTanks + ubTotalJeeps;

	//Now, process all of the groups and search for both enemy and player groups in the sector.
	//For enemy groups, we fill up the slots until we have none left or all of the groups have been
	//processed.

	for( pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle &&
				 pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{ //Process enemy group in sector.
			const BOOLEAN isTransportGroup = pGroup->pEnemyGroup->ubIntention == TRANSPORT;
			if( sNumSlots > 0 )
			{
				AssertGE(pGroup->pEnemyGroup->ubNumAdmins, pGroup->pEnemyGroup->ubAdminsInBattle);
				ubNumAdmins = pGroup->pEnemyGroup->ubNumAdmins - pGroup->pEnemyGroup->ubAdminsInBattle;
				sNumSlots -= ubNumAdmins;
				if( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumAdmins += sNumSlots;
					sNumSlots = 0;
					gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				}
				pGroup->pEnemyGroup->ubAdminsInBattle += ubNumAdmins;
				ubTotalAdmins += ubNumAdmins;

				if (isTransportGroup)
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ADMINISTRATOR, ubNumAdmins);
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
					gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				}
				pGroup->pEnemyGroup->ubTroopsInBattle += ubNumTroops;
				ubTotalTroops += ubNumTroops;

				if (isTransportGroup)
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ARMY, ubNumTroops);
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
					gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				}
				pGroup->pEnemyGroup->ubElitesInBattle += ubNumElites;
				ubTotalElites += ubNumElites;

				if (isTransportGroup)
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ELITE, ubNumElites);
			}
			if( sNumSlots > 0 )
			{ //Add robots
				AssertGE(pGroup->pEnemyGroup->ubNumRobots, pGroup->pEnemyGroup->ubRobotsInBattle);
				ubNumRobots = pGroup->pEnemyGroup->ubNumRobots - pGroup->pEnemyGroup->ubRobotsInBattle;
				sNumSlots -= ubNumRobots;
				if( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumRobots += sNumSlots;
					sNumSlots = 0;
					gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				}
				pGroup->pEnemyGroup->ubRobotsInBattle += ubNumRobots;
				ubTotalRobots += ubNumRobots;

				if (isTransportGroup)
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ROBOT, ubNumRobots);
			}
			if( sNumSlots > 0 )
			{ //Add tanks
				AssertGE(pGroup->pEnemyGroup->ubNumTanks, pGroup->pEnemyGroup->ubTanksInBattle);
				ubNumTanks = pGroup->pEnemyGroup->ubNumTanks - pGroup->pEnemyGroup->ubTanksInBattle;
				sNumSlots -= ubNumTanks;
				if( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumTanks += sNumSlots;
					sNumSlots = 0;
					gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				}
				pGroup->pEnemyGroup->ubTanksInBattle += ubNumTanks;
				ubTotalTanks += ubNumTanks;

				if (isTransportGroup)
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_TANK, ubNumTanks);
			}
			if ( sNumSlots > 0 )
			{
				//Add jeeps
				AssertGE( pGroup->pEnemyGroup->ubNumJeeps, pGroup->pEnemyGroup->ubJeepsInBattle );
				ubNumJeeps = pGroup->pEnemyGroup->ubNumJeeps - pGroup->pEnemyGroup->ubJeepsInBattle;
				sNumSlots -= ubNumJeeps;
				if ( sNumSlots < 0 )
				{ //adjust the value to zero
					ubNumJeeps += sNumSlots;
					sNumSlots = 0;
					gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
				}
				pGroup->pEnemyGroup->ubJeepsInBattle += ubNumJeeps;
				ubTotalJeeps += ubNumJeeps;

				if (isTransportGroup)
					AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_JEEP, ubNumJeeps);
			}
			//NOTE:
			//no provisions for profile troop leader or retreat groups yet.
		}

		if ( pGroup->usGroupTeam == OUR_TEAM && !pGroup->fVehicle && !pGroup->fBetweenSectors &&
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
	if ( !ubTotalAdmins && !ubTotalTroops && !ubTotalElites && !ubTotalRobots && !ubTotalTanks && !ubTotalJeeps )
	{
		return FALSE;
	}

	AddSoldierInitListEnemyDefenceSoldiers( ubTotalAdmins, ubTotalTroops, ubTotalElites, ubTotalRobots, ubTotalTanks, ubTotalJeeps );

	//Now, we have to go through all of the enemies in the new map, and assign their respective groups if
	//in a mobile group, but only for the ones that were assigned from the
	//sNumSlots = mapMaximumNumberOfEnemies - totalCountOfStationaryEnemies;

	// Flugente: at this point, soldiers have already been placed, thus enough slots for them exist
	sNumSlots = min( mapMaximumNumberOfEnemies, NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, ENEMY_TEAM ) );

	pGroup = gpGroupList;
	unsigned firstSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
	unsigned lastSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bLastID;
	unsigned slotsAvailable = lastSlot-firstSlot+1;

	while( pGroup && sNumSlots > 0 )
	{
		if ( pGroup->usGroupTeam != OUR_TEAM && !pGroup->fVehicle &&
				 pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
		{
			ubNumAdmins = pGroup->pEnemyGroup->ubAdminsInBattle;
			ubNumTroops = pGroup->pEnemyGroup->ubTroopsInBattle;
			ubNumElites = pGroup->pEnemyGroup->ubElitesInBattle;
			ubNumRobots = pGroup->pEnemyGroup->ubRobotsInBattle;
			ubNumTanks = pGroup->pEnemyGroup->ubTanksInBattle;
			ubNumJeeps = pGroup->pEnemyGroup->ubJeepsInBattle;
			unsigned num = ubNumAdmins + ubNumTroops + ubNumElites + ubNumRobots + ubNumTanks + ubNumJeeps;

			AssertGE((int)slotsAvailable, sNumSlots);

			for (unsigned slot = firstSlot;	(slot <= lastSlot) && num && sNumSlots;	++slot)
			{
				pSoldier = &Menptr[ slot ];

				// Skip inactive and already grouped soldiers
				if (!pSoldier->bActive || pSoldier->ubGroupID)
				{
					// if this guy already has an ID, reduce the number of people who still need one
					--num;
					--sNumSlots;
					firstSlot = slot + 1;
					
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
							firstSlot = slot + 1;
						}
						break;
					case SOLDIER_CLASS_ARMY:
						if( ubNumTroops )
						{
							num--;
							sNumSlots--;
							ubNumTroops--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
							firstSlot = slot + 1;
						}
						break;
					case SOLDIER_CLASS_ELITE:
						if( ubNumElites )
						{
							num--;
							sNumSlots--;
							ubNumElites--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
							firstSlot = slot + 1;
						}
						break;
					// silversurfer: bugfix for Jaggzilla bug #623
					// Mike or Iggy can be part of the enemy team and they are created from an Elite but they don't have SOLDIER_CLASS_ELITE.
					// Therefore once this for loop was done ubNumElites was still 1 which caused an assertion error.
					case SOLDIER_CLASS_NONE:
						if( ubNumElites )
						{
							if ( pSoldier->ubProfile == MIKE || pSoldier->ubProfile == IGGY )
							{
								num--;
								sNumSlots--;
								ubNumElites--;
								pSoldier->ubGroupID = pGroup->ubGroupID;
								firstSlot = slot + 1;
							}
						}
						break;
					case SOLDIER_CLASS_ROBOT:
						if( ubNumRobots )
						{
							num--;
							sNumSlots--;
							ubNumRobots--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
							firstSlot = slot + 1;
						}
						break;
					case SOLDIER_CLASS_TANK:
						if( ubNumTanks )
						{
							num--;
							sNumSlots--;
							ubNumTanks--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
							firstSlot = slot + 1;
						}
						break;
					case SOLDIER_CLASS_JEEP:
						if ( ubNumJeeps )
						{
							num--;
							sNumSlots--;
							ubNumJeeps--;
							pSoldier->ubGroupID = pGroup->ubGroupID;
							firstSlot = slot + 1;
						}
						break;
				}

			}

			// Flugente: instead of just crashing the game without any explanation to the user, ignore this issue if it still exists.
			// The worst that should happen is a warning that a soldier has no group id.
			/*AssertEQ( ubNumElites , 0);
			AssertEQ( ubNumTroops , 0);
			AssertEQ( ubNumAdmins , 0);
			AssertEQ( ubNumTanks , 0);
			AssertEQ( ubNumJeeps , 0);
			AssertEQ( ubNumRobots , 0);
			AssertEQ( num , 0);*/
		}
		pGroup = pGroup->next;
	}

	UpdateTransportGroupInventory();

	ValidateEnemiesHaveWeapons();
	UnPauseGame();

	return TRUE;
}

BOOLEAN PrepareEnemyForUndergroundBattle()
{
	UNDERGROUND_SECTORINFO *pUnderground;
	unsigned ubTotalAdmins, ubTotalTroops, ubTotalElites, ubTotalRobots, ubTotalTanks, ubTotalJeeps;

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
				ubTotalRobots = (UINT8)(pUnderground->ubNumRobots - pUnderground->ubRobotsInBattle);
				ubTotalTanks = (UINT8)(pUnderground->ubNumTanks - pUnderground->ubTanksInBattle);
				ubTotalJeeps = (UINT8)(pUnderground->ubNumJeeps - pUnderground->ubJeepsInBattle);
				pUnderground->ubAdminsInBattle += ubTotalAdmins;
				pUnderground->ubTroopsInBattle += ubTotalTroops;
				pUnderground->ubElitesInBattle += ubTotalElites;
				pUnderground->ubRobotsInBattle += ubTotalRobots;
				pUnderground->ubTanksInBattle += ubTotalTanks;
				pUnderground->ubJeepsInBattle += ubTotalJeeps;
				AddSoldierInitListEnemyDefenceSoldiers( pUnderground->ubNumAdmins, pUnderground->ubNumTroops, pUnderground->ubNumElites, pUnderground->ubNumRobots, pUnderground->ubNumTanks, pUnderground->ubNumJeeps );
				ValidateEnemiesHaveWeapons();
				UnPauseGame();
			}
			return ((BOOLEAN)(pUnderground->ubNumAdmins + pUnderground->ubNumTroops + pUnderground->ubNumElites + pUnderground->ubNumRobots + pUnderground->ubNumTanks + pUnderground->ubNumJeeps > 0));
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
		case TANK_CAR:
			if( !pSoldier->bSectorZ )
			{
				pSector = &SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ];
				if( pSector->ubNumTanks )
				{
					pSector->ubNumTanks--;
				}
				if( pSector->ubTanksInBattle )
				{
					pSector->ubTanksInBattle--;
				}
			}
			else
			{
				UNDERGROUND_SECTORINFO *pUnderground;
				pUnderground = FindUnderGroundSector( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, (UINT8)pSoldier->bSectorZ );
				Assert( pUnderground );
				if( pUnderground->ubNumTanks)
				{
					pUnderground->ubNumTanks--;
				}
				if( pUnderground->ubTanksInBattle )
				{
					pUnderground->ubTanksInBattle--;
				}
			}
			break;

		case COMBAT_JEEP_CAR:
			if ( !pSoldier->bSectorZ )
			{
				pSector = &SectorInfo[SECTOR( pSoldier->sSectorX, pSoldier->sSectorY )];
				if ( pSector->ubNumJeeps )
				{
					pSector->ubNumJeeps--;
				}
				if ( pSector->ubJeepsInBattle )
				{
					pSector->ubJeepsInBattle--;
				}
			}
			else
			{
				UNDERGROUND_SECTORINFO *pUnderground;
				pUnderground = FindUnderGroundSector( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, (UINT8)pSoldier->bSectorZ );
				Assert( pUnderground );
				if ( pUnderground->ubNumJeeps )
				{
					pUnderground->ubNumJeeps--;
				}
				if ( pUnderground->ubJeepsInBattle )
				{
					pUnderground->ubJeepsInBattle--;
				}
			}
			break;
	}

	if( pSoldier->aiData.bNeutral || pSoldier->bTeam != ENEMY_TEAM && pSoldier->bTeam != CREATURE_TEAM )
		return;

	//we are recording an enemy death
	if( pSoldier->ubGroupID )
	{
		//The enemy was in a mobile group
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

		if ( pGroup->usGroupTeam == OUR_TEAM )
		{
#ifdef JA2BETAVERSION
				CHAR16 str[256];
				swprintf( str, L"Attempting to process player group %d thinking it's an enemy group in ProcessQueenCmdImplicationsOfDeath()", pSoldier->ubGroupID );
				DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
#endif
			return;
		}
		else if ( pGroup->usGroupTeam == MILITIA_TEAM )
		{
#ifdef JA2BETAVERSION
			CHAR16 str[256];
			swprintf( str, L"Attempting to process militia group %d thinking it's an enemy group in ProcessQueenCmdImplicationsOfDeath()", pSoldier->ubGroupID );
			DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
#endif
			return;
		}

		switch( pSoldier->ubSoldierClass )
		{
			case SOLDIER_CLASS_ROBOT:
				if( pGroup->pEnemyGroup->ubNumRobots )
				{
					pGroup->pEnemyGroup->ubNumRobots--;
				}
				if( pGroup->pEnemyGroup->ubRobotsInBattle )
				{
					pGroup->pEnemyGroup->ubRobotsInBattle--;
				}
				break;
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
						if ( pGroup->ubGroupSize <= iMaxEnemyGroupSize && pGroup->pEnemyGroup->ubNumElites != pGroup->pEnemyGroup->ubElitesInBattle && !gfPendingNonPlayerTeam[ENEMY_TEAM] ||
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
						if ( pGroup->ubGroupSize <= iMaxEnemyGroupSize && pGroup->pEnemyGroup->ubNumTroops != pGroup->pEnemyGroup->ubTroopsInBattle && !gfPendingNonPlayerTeam[ENEMY_TEAM] ||
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
						if ( pGroup->ubGroupSize <= iMaxEnemyGroupSize && pGroup->pEnemyGroup->ubNumAdmins != pGroup->pEnemyGroup->ubAdminsInBattle && !gfPendingNonPlayerTeam[ENEMY_TEAM] ||
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
				ubTotalEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumTanks + pSector->ubNumJeeps;
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
				case SOLDIER_CLASS_ROBOT:
					if( pSector->ubNumRobots )
					{
						pSector->ubNumRobots--;
					}
					if( pSector->ubRobotsInBattle )
					{
						pSector->ubRobotsInBattle--;
					}
					break;
				case SOLDIER_CLASS_CREATURE:
				case SOLDIER_CLASS_BANDIT:
				case SOLDIER_CLASS_ZOMBIE:
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
			UINT32 ubTotalEnemies = pSector->ubNumAdmins + pSector->ubNumTroops + pSector->ubNumElites + pSector->ubNumTanks + pSector->ubNumJeeps;
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
					case SOLDIER_CLASS_ROBOT:
						if( pSector->ubNumRobots )
						{
							pSector->ubNumRobots--;
						}
						if( pSector->ubRobotsInBattle )
						{
							pSector->ubRobotsInBattle--;
						}
						break;
					case SOLDIER_CLASS_CREATURE:
					case SOLDIER_CLASS_BANDIT:
					case SOLDIER_CLASS_ZOMBIE:
						if (pSoldier->ubBodyType == BLOODCAT)
						{
							if( pSector->ubNumBloodcats > 0 )
								pSector->ubNumBloodcats--;
						}
						else
						{
#ifdef JA2BETAVERSION
							if( ubTotalEnemies <= (UINT32)iMaxEnemyGroupSize && pSector->ubNumCreatures != pSector->ubCreaturesInBattle ||
								!pSector->ubNumCreatures || 
								!pSector->ubCreaturesInBattle ||
								pSector->ubNumCreatures > gGameExternalOptions.ubGameMaximumNumberOfCreatures || 
								pSector->ubCreaturesInBattle > gGameExternalOptions.ubGameMaximumNumberOfCreatures )
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

							if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_CREATURE )
							{
								if ( !pSector->ubNumCreatures && gWorldSectorX != 9 && gWorldSectorY != 10 )
								{
									//If the player has successfully killed all creatures in ANY underground sector except J9
									//then cancel any pending creature town attack.
									DeleteAllStrategicEventsOfType( EVENT_CREATURE_ATTACK );
								}

								// a monster has died.  Post an event to immediately check whether a mine has been cleared.
								AddStrategicEventUsingSeconds( EVENT_CHECK_IF_MINE_CLEARED, GetWorldTotalSeconds() + 15, 0 );

								if ( pSoldier->ubBodyType == QUEENMONSTER )
								{
									//Need to call this, as the queen is really big, and killing her leaves a bunch
									//of bad tiles in behind her.  Calling this function cleans it up.
									InvalidateWorldRedundency();
									//Now that the queen is dead, turn off the creature quest.
									EndCreatureQuest();
									EndQuest( QUEST_CREATURES, gWorldSectorX, gWorldSectorY );
								}
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
		iNumEnemiesInSector = NumNonPlayerTeamMembersInSector( pSoldier->sSectorX, pSoldier->sSectorY, ENEMY_TEAM );
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
	UINT8 ubNumRobots, ubNumElites, ubNumTroops, ubNumAdmins, ubNumTanks, ubNumJeeps;
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
	
	if ( (!PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, 0 ) && !NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, MILITIA_TEAM ))
		|| !NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY, ENEMY_TEAM ) )
		return;

	// Flugente: if there is an enemy helicopter here, it can add reinforcements
	if ( gTacticalStatus.Team[ENEMY_TEAM].bAwareOfOpposition )
	{
		EnemyHeliTroopDrop( SECTOR( gWorldSectorX, gWorldSectorY ) );
	}

	ubSlots = NumFreeSlots( ENEMY_TEAM );
	// silversurfer: Only if there are enemy troops in the sector already we can talk about "reinforcements". If no troops are there we will spawn the incoming troops without delay.
	if( gGameExternalOptions.sMinDelayEnemyReinforcements && gTacticalStatus.Team[ENEMY_TEAM].bTeamActive )//dnl ch68 080913
	{
		if(gTacticalStatus.Team[ENEMY_TEAM].bAwareOfOpposition == TRUE)
		{
			if(guiReinforceTurn == 0)
			{
				guiReinforceTurn = guiTurnCnt + gGameExternalOptions.sMinDelayEnemyReinforcements + Random(gGameExternalOptions.sRndDelayEnemyReinforcements+1);// first possible reinforcement
				ubSlots = 0;
			}
			else if(guiTurnCnt >= guiReinforceTurn)
			{
				guiReinforceTurn = guiTurnCnt + gGameExternalOptions.sMinDelayEnemyReinforcements/3 + Random(gGameExternalOptions.sRndDelayEnemyReinforcements+1);// any other reinforcement should appear more frequently
				ubNumAvailable = gGameExternalOptions.sMinEnterEnemyReinforcements + Random(gGameExternalOptions.sRndEnterEnemyReinforcements+1);// total allowed units to enter per reinforce turn
//SendFmtMsg("Enemy reinforcements: ubSlots=%d ubNumAvailable=%d", ubSlots, ubNumAvailable);
				if(ubSlots > ubNumAvailable)
					ubSlots = ubNumAvailable;
				guiArrived += (ubNumAvailable - ubSlots);
			}
			else
			{
				if(guiArrived > 0)
				{
					if(ubSlots > guiArrived)
						ubSlots = guiArrived;
					guiArrived -= ubSlots;
				}
				else
					ubSlots = 0;
			}
		}
		else
			guiReinforceTurn = guiArrived = 0;
//SendFmtMsg("Enemy reinforcements: guiTurnCnt=%d guiReinforceTurn=%d guiArrived=%d", guiTurnCnt, guiReinforceTurn, guiArrived);
	}

	if( !ubSlots )
	{
		//no available slots to add enemies to.  Try again later...
		return;
	}

	if ( !gfPendingNonPlayerTeam[ENEMY_TEAM] )
	{
		//Optimization.  No point in checking for group reinforcements if we know that there aren't any more enemies that can
		//be added to this battle.  This changes whenever a new enemy group arrives at the scene.

		while (ubSlots)
		{
			UINT8 ubInsertionCode = 255;

			if( gTacticalStatus.Team[ ENEMY_TEAM ].bAwareOfOpposition == TRUE )
				ubInsertionCode = DoReinforcementAsPendingNonPlayer( gWorldSectorX, gWorldSectorY, ENEMY_TEAM );

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

	if ( pSector->ubNumRobots + pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins + pSector->ubNumTanks + pSector->ubNumJeeps )
	{
		ubNumRobots = ubNumElites = ubNumTroops = ubNumAdmins = ubNumTanks = ubNumJeeps = 0;

		ubNumAvailable = pSector->ubNumRobots + pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins + pSector->ubNumTanks + pSector->ubNumJeeps
			- pSector->ubRobotsInBattle - pSector->ubElitesInBattle - pSector->ubTroopsInBattle - pSector->ubAdminsInBattle - pSector->ubTanksInBattle - pSector->ubJeepsInBattle;
		while( ubNumAvailable && ubSlots )
		{
			// So they just magically appear out of nowhere from the edge?

			//This group has enemies waiting for a chance to enter the battle.
			if( pSector->ubRobotsInBattle < pSector->ubNumRobots )
			{ //Add a robot
				pSector->ubRobotsInBattle++;
				ubNumAvailable--;
				ubSlots--;
				ubNumRobots++;
			}
			else if( pSector->ubElitesInBattle < pSector->ubNumElites )
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
			else if( pSector->ubTanksInBattle < pSector->ubNumTanks )
			{ //Add a tank
				pSector->ubTanksInBattle++;
				ubNumAvailable--;
				ubSlots--;
				ubNumTanks++;
			}
			else if ( pSector->ubJeepsInBattle < pSector->ubNumJeeps )
			{
				//Add a jeep
				pSector->ubJeepsInBattle++;
				ubNumAvailable--;
				ubSlots--;
				ubNumJeeps++;
			}
			else
			{
				AssertMsg( 0, "AddPossiblePendingEnemiesToBattle():  Logic Error -- by Kris" );
			}
		}


		if ( ubNumAdmins || ubNumTroops || ubNumElites || ubNumRobots || ubNumTanks || ubNumJeeps )
		{	//This group has contributed forces, then add them now, because different
			//groups appear on different sides of the map.
			UINT8 ubStrategicInsertionCode = ubPredefinedInsertionCode;
			//First, determine which entrypoint to use, based on the travel direction of the group.

			if( ubStrategicInsertionCode == 255 )
			{
				if ( NumNonPlayerTeamMembersInSector( gWorldSectorX + 1, gWorldSectorY, ENEMY_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_EAST;
				else if ( NumNonPlayerTeamMembersInSector( gWorldSectorX - 1, gWorldSectorY, ENEMY_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_WEST;
				else if ( NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY + 1, ENEMY_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_SOUTH;
				else if ( NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY - 1, ENEMY_TEAM ) )
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
				if ( !NumNonPlayerTeamMembersInSector( gWorldSectorX + 1, gWorldSectorY, MILITIA_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_EAST;
				else if ( !NumNonPlayerTeamMembersInSector( gWorldSectorX - 1, gWorldSectorY, MILITIA_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_WEST;
				else if ( !NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY + 1, MILITIA_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_SOUTH;
				else if ( !NumNonPlayerTeamMembersInSector( gWorldSectorX, gWorldSectorY - 1, MILITIA_TEAM ) )
					ubStrategicInsertionCode = INSERTION_CODE_NORTH;
			}

			if( ubStrategicInsertionCode < INSERTION_CODE_NORTH || ubStrategicInsertionCode > INSERTION_CODE_WEST )
				ubStrategicInsertionCode = INSERTION_CODE_NORTH + Random( 4 );

			AddEnemiesToBattle( 0, ubStrategicInsertionCode, ubNumAdmins, ubNumTroops, ubNumElites, ubNumRobots, ubNumTanks, ubNumJeeps, FALSE );
			gfPendingNonPlayerTeam[ENEMY_TEAM] = TRUE;
		}
	}

	ubPredefinedInsertionCode = 255;

	// Figure out which groups are in the sector, so we can have reinforcements arrive at random
	for (ubNumGroupsInSector = 0, pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
			++ubNumGroupsInSector;
	}
	pGroupInSectorList = (GROUP**) MemAlloc( ubNumGroupsInSector * sizeof( GROUP*));
	for (ubNumGroupsInSector = 0, pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == gWorldSectorX && pGroup->ubSectorY == gWorldSectorY && !gbWorldSectorZ )
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
		UINT8 currentgroupsize = pGroup->pEnemyGroup->ubElitesInBattle + pGroup->pEnemyGroup->ubTroopsInBattle + pGroup->pEnemyGroup->ubAdminsInBattle 
			+ pGroup->pEnemyGroup->ubRobotsInBattle + pGroup->pEnemyGroup->ubTanksInBattle + pGroup->pEnemyGroup->ubJeepsInBattle;
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
				// sevenfm: use Random() instead
				//UINT32 rndInsertionCode = GetRndNum(3);
				UINT32 rndInsertionCode = Random(4);
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
				// sevenfm: use Random() instead
				//UINT32 rndInsertionCode = GetRndNum(3);
				UINT32 rndInsertionCode = Random(4);
				ubInsertionCode = rndInsertionCode;

				//Assert(0);
			}
		}
		else
		{
			// The group has no movement orders.  Where did it come from?
			Assert(0);
		}
		
		if ( pGroup->pEnemyGroup->ubElitesInBattle < pGroup->pEnemyGroup->ubNumElites )
		{ //Add an elite troop
			pGroup->pEnemyGroup->ubElitesInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 0, 1, 0, 0, 0, FALSE );
		}
		else if( pGroup->pEnemyGroup->ubTroopsInBattle < pGroup->pEnemyGroup->ubNumTroops )
		{ //Add a regular troop.
			pGroup->pEnemyGroup->ubTroopsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 1, 0, 0, 0, 0, FALSE );
		}
		else if( pGroup->pEnemyGroup->ubAdminsInBattle < pGroup->pEnemyGroup->ubNumAdmins )
		{ //Add an admin troop
			pGroup->pEnemyGroup->ubAdminsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 1, 0, 0, 0, 0, 0, FALSE );
		}
		else if( pGroup->pEnemyGroup->ubRobotsInBattle < pGroup->pEnemyGroup->ubNumRobots)
		{ //Add a robot
			pGroup->pEnemyGroup->ubRobotsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 0, 0, 1, 0, 0, FALSE );
		}
		else if( pGroup->pEnemyGroup->ubTanksInBattle < pGroup->pEnemyGroup->ubNumTanks )
		{ //Add a tank
			pGroup->pEnemyGroup->ubTanksInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 0, 0, 0, 1, 0, FALSE );
		}
		else if ( pGroup->pEnemyGroup->ubJeepsInBattle < pGroup->pEnemyGroup->ubNumJeeps )
		{ //Add a jeep
			pGroup->pEnemyGroup->ubJeepsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 0, 0, 0, 0, 1, FALSE );
		}
		else
		{
			// WANNE: Instead of asserting, just add a regular troop
			//AssertMsg( 0, "AddPossiblePendingEnemiesToBattle():  Logic Error -- by Kris" );

			pGroup->pEnemyGroup->ubTroopsInBattle++;
			ubSlots--;
			AddEnemiesToBattle( pGroup, ubInsertionCode, 0, 1, 0, 0, 0, 0, FALSE );
		}
	}

	MemFree( pGroupInSectorList);

	if( ubSlots )
	{
		//After going through the process, we have finished with some free slots and no more enemies to add.
		//So, we can turn off the flag, as this check is no longer needed.
		gfPendingNonPlayerTeam[ENEMY_TEAM] = FALSE;
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

void AddEnemiesToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT16 ubNumAdmins, UINT16 ubNumTroops, UINT16 ubNumElites, UINT16 ubNumRobots, UINT16 ubNumTanks, UINT16 ubNumJeeps, BOOLEAN fMagicallyAppeared )
{
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo;
	UINT16 ubCurrSlot;
	UINT16 ubTotalSoldiers;
	UINT8 bDesiredDirection=0;
	
	// while transport groups can't normally reinforce, this covers the case where a transport group enters a sector (via normal movement)
	// where a battle is in progress.
	if (pGroup && pGroup->pEnemyGroup->ubIntention == TRANSPORT)
	{
		AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ADMINISTRATOR, ubNumAdmins);
		AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ARMY, ubNumTroops);
		AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_ELITE, ubNumElites);
		AddToTransportGroupMap(pGroup->ubGroupID, SOLDIER_CLASS_JEEP, ubNumJeeps);
	}

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
			pSector->ubNumAdmins		+= ubNumAdmins;
			pSector->ubAdminsInBattle	+= ubNumAdmins;
			pSector->ubNumTroops		+= ubNumTroops;
			pSector->ubTroopsInBattle	+= ubNumTroops;
			pSector->ubNumElites		+= ubNumElites;
			pSector->ubElitesInBattle	+= ubNumElites;
			pSector->ubNumRobots		+= ubNumRobots;
			pSector->ubRobotsInBattle	+= ubNumRobots;
			pSector->ubNumTanks			+= ubNumTanks;
			pSector->ubTanksInBattle	+= ubNumTanks;
			pSector->ubNumJeeps			+= ubNumJeeps;
			pSector->ubJeepsInBattle	+= ubNumJeeps;
		}
		else
		{
			UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			if( pSector )
			{
				pSector->ubNumAdmins		+= ubNumAdmins;
				pSector->ubAdminsInBattle	+= ubNumAdmins;
				pSector->ubNumTroops		+= ubNumTroops;
				pSector->ubTroopsInBattle	+= ubNumTroops;
				pSector->ubNumElites		+= ubNumElites;
				pSector->ubElitesInBattle	+= ubNumElites;
				pSector->ubNumRobots		+= ubNumRobots;
				pSector->ubRobotsInBattle	+= ubNumRobots;
				pSector->ubNumTanks			+= ubNumTanks;
				pSector->ubTanksInBattle	+= ubNumTanks;
				pSector->ubNumJeeps			+= ubNumJeeps;
				pSector->ubJeepsInBattle	+= ubNumJeeps;
			}
		}
		//Because the enemies magically appeared, have one of our soldiers say something...
		NotifyPlayersOfNewEnemies();
	}

	ubTotalSoldiers = ubNumAdmins + ubNumTroops + ubNumElites + ubNumRobots + ubNumTanks + ubNumJeeps;
	
#ifdef JA2UB
	if( gsGridNoForMapEdgePointInfo != -1 )
	{
		ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}

	if( ubStrategicInsertionCode == INSERTION_CODE_GRIDNO )
	{
		if( gsGridNoForMapEdgePointInfo == -1 )
		{
			Assert( 0 );
			gsGridNoForMapEdgePointInfo=0;
		}

		for( UINT8 ubCnt=0; ubCnt<32; ++ubCnt)
		{
			MapEdgepointInfo.sGridNo[ ubCnt ] = gsGridNoForMapEdgePointInfo;
		}

		MapEdgepointInfo.ubNumPoints = 32;
		MapEdgepointInfo.ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}
	else
	{
		ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, ubTotalSoldiers );
	}
#else
	ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, ubTotalSoldiers );
#endif	
	
	extern INT16 gsStrategicDiseaseOriginSector;

	// Flugente: we need to set the sector of origin to determine from which sector to take the disease ratio that affects health
	gsStrategicDiseaseOriginSector = -1;

	if ( !gbWorldSectorZ )
	{
		switch ( ubStrategicInsertionCode )
		{
		case INSERTION_CODE_NORTH:	gsStrategicDiseaseOriginSector = SECTOR( gWorldSectorX, gWorldSectorY - 1 );	break;
		case INSERTION_CODE_EAST:	gsStrategicDiseaseOriginSector = SECTOR( gWorldSectorX + 1, gWorldSectorY );	break;
		case INSERTION_CODE_SOUTH:	gsStrategicDiseaseOriginSector = SECTOR( gWorldSectorX, gWorldSectorY + 1 );	break;
		case INSERTION_CODE_WEST:	gsStrategicDiseaseOriginSector = SECTOR( gWorldSectorX - 1, gWorldSectorY );	break;
		default:
			break;
		}
	}
	
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
		else if( ubNumTroops && Random( ubTotalSoldiers ) < (ubNumElites + ubNumTroops) )
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
		else if( ubNumAdmins && Random( ubTotalSoldiers ) < (ubNumElites + ubNumTroops + ubNumAdmins) )
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
		else if( ubNumRobots && Random( ubTotalSoldiers ) < (ubNumElites + ubNumTroops + ubNumAdmins + ubNumRobots) )
		{
			ubNumRobots--;
			ubTotalSoldiers--;
			Assert(pSoldier = TacticalCreateEnemyRobot());
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
		else if( ubNumTanks && Random( ubTotalSoldiers ) < (ubNumElites + ubNumTroops + ubNumAdmins + ubNumRobots + ubNumTanks) && ubNumberOfVehicles < 9 )
		{
			ubNumTanks--;
			ubTotalSoldiers--;
			Assert(pSoldier = TacticalCreateEnemyTank());
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
		else if ( ubNumJeeps && Random( ubTotalSoldiers ) < (ubNumElites + ubNumTroops + ubNumAdmins + ubNumRobots + ubNumTanks + ubNumJeeps) && ubNumberOfVehicles < 9 )
		{
			ubNumJeeps--;
			ubTotalSoldiers--;
			Assert( pSoldier = TacticalCreateEnemyJeep( ) );
			if ( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if ( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ubCurrSlot++];
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
			pSoldier->usSoldierFlagMask |= SOLDIER_NO_AP;

			// Flugente: campaign stats
			if ( IsOurSoldier(pSoldier) )
				gCurrentIncident.usIncidentFlags |= INCIDENT_REINFORCEMENTS_PLAYERSIDE;
			else
				gCurrentIncident.usIncidentFlags |= INCIDENT_REINFORCEMENTS_ENEMY;
		}
	}

	// set this back so it doesn't affect others
	gsStrategicDiseaseOriginSector = -1;
	
#ifdef JA2UB
	gsGridNoForMapEdgePointInfo = -1;
#endif
}

void AddMilitiaToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT16 ubNumGreens, UINT16 ubNumRegulars, UINT16 ubNumElites, BOOLEAN fMagicallyAppeared )
{
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo;
	UINT16 ubCurrSlot;
	UINT16 ubTotalSoldiers;
	UINT8 bDesiredDirection = 0;

	switch ( ubStrategicInsertionCode )
	{
	case INSERTION_CODE_NORTH:		bDesiredDirection = SOUTHEAST;					break;
	case INSERTION_CODE_EAST:		bDesiredDirection = SOUTHWEST;					break;
	case INSERTION_CODE_SOUTH:		bDesiredDirection = NORTHWEST;					break;
	case INSERTION_CODE_WEST:		bDesiredDirection = NORTHEAST;					break;
	default:  AssertMsg( 0, "Illegal direction passed to AddEnemiesToBattle()" );	break;
	}
#ifdef JA2TESTVERSION
	ScreenMsg( FONT_RED, MSG_INTERFACE, L"Militia reinforcements have arrived!  (%d greens, %d regulars, %d elite)", ubNumGreens, ubNumRegulars, ubNumElites );
#endif

	if ( fMagicallyAppeared )
	{
		//update the strategic counters
		if ( !gbWorldSectorZ )
		{
			SECTORINFO *pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];
			pSector->ubNumberOfCivsAtLevel[GREEN_MILITIA] += ubNumGreens;
			pSector->ubNumberOfCivsAtLevel[REGULAR_MILITIA] += ubNumRegulars;
			pSector->ubNumberOfCivsAtLevel[ELITE_MILITIA] += ubNumElites;
		}
		else
		{
			// there are no underground militia... error if this ever happens
			AssertMsg( 0, "Tried to add militia to an underground sector" );
		}
	}

	ubTotalSoldiers = ubNumGreens + ubNumRegulars + ubNumElites;

#ifdef JA2UB
	if ( gsGridNoForMapEdgePointInfo != -1 )
	{
		ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}

	if ( ubStrategicInsertionCode == INSERTION_CODE_GRIDNO )
	{
		if ( gsGridNoForMapEdgePointInfo == -1 )
		{
			Assert( 0 );
			gsGridNoForMapEdgePointInfo = 0;
		}

		for ( UINT8 ubCnt = 0; ubCnt<32; ++ubCnt )
		{
			MapEdgepointInfo.sGridNo[ubCnt] = gsGridNoForMapEdgePointInfo;
		}

		MapEdgepointInfo.ubNumPoints = 32;
		MapEdgepointInfo.ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}
	else
	{
		ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, ubTotalSoldiers );
	}
#else
	ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, (ubTotalSoldiers) );
#endif	

	ubCurrSlot = 0;
	while ( ubTotalSoldiers )
	{
		if ( ubNumElites && Random( ubTotalSoldiers ) < ubNumElites )
		{
			ubNumElites--;
			ubTotalSoldiers--;
			Assert( pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_ELITE_MILITIA, gWorldSectorX, gWorldSectorY ) );
			if ( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if ( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ubCurrSlot++];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if ( ubNumRegulars && Random( ubTotalSoldiers ) < (ubNumElites + ubNumRegulars) )
		{
			ubNumRegulars--;
			ubTotalSoldiers--;
			Assert( pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_REG_MILITIA, gWorldSectorX, gWorldSectorY ) );
			if ( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if ( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ubCurrSlot++];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if ( ubNumGreens && Random( ubTotalSoldiers ) < (ubNumElites + ubNumRegulars + ubNumGreens) )
		{
			ubNumGreens--;
			ubTotalSoldiers--;
			Assert( pSoldier = TacticalCreateMilitia( SOLDIER_CLASS_GREEN_MILITIA, gWorldSectorX, gWorldSectorY ) );
			if ( pGroup )
			{
				pSoldier->ubGroupID = pGroup->ubGroupID;
			}

			pSoldier->ubInsertionDirection = bDesiredDirection;
			//Setup the position
			if ( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ubCurrSlot++];
			}
			else
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}

		// HEADROCK HAM 3.2: enemy reinforcements arrive with 0 APs.
		if ( gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 1 || gGameExternalOptions.ubReinforcementsFirstTurnFreeze == 2 )
		{
			pSoldier->bActionPoints = 0;

			// Flugente: due to a fix, also note here that the reinforcements get no APs.
			pSoldier->usSoldierFlagMask |= SOLDIER_NO_AP;

			// Flugente: campaign stats
			if ( IsOurSoldier( pSoldier ) )
				gCurrentIncident.usIncidentFlags |= INCIDENT_REINFORCEMENTS_PLAYERSIDE;
			else
				gCurrentIncident.usIncidentFlags |= INCIDENT_REINFORCEMENTS_ENEMY;
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
#ifndef JA2UB
	// Set flag...
	if( !( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE ) || !(gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE) )
	{
		if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTNOTSTARTED )
		{
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;
			StartQuest( QUEST_HELD_IN_ALMA, gWorldSectorX, gWorldSectorY );
		}
		else if (gubQuest[QUEST_HELD_IN_TIXA] == QUESTNOTSTARTED)
		{
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;
			StartQuest(QUEST_HELD_IN_TIXA, gWorldSectorX, gWorldSectorY);
		}
		else if (gubQuest[QUEST_INTERROGATION] == QUESTNOTSTARTED)
		{
			StartQuest( QUEST_INTERROGATION, gWorldSectorX, gWorldSectorY );
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE;

			// OK! - Schedule Meanwhile now!
			HandleInterrogationMeanwhileScene();
		}
		else
		{
			// Set both flags if we can't start any of the three POW quests
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE;
			gStrategicStatus.uiFlags |= STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE;
		}
	}
#endif
}

int CalculateMaximumPrisonerAmount()
{
#ifndef JA2UB
	if (gubQuest[QUEST_HELD_IN_ALMA] == QUESTNOTSTARTED) { return std::size(gModSettings.iInitialPOWGridNo); }
	if (gubQuest[QUEST_HELD_IN_TIXA] == QUESTNOTSTARTED) { return std::size(gModSettings.iTixaPrisonPOWGridNo); }
	if (gubQuest[QUEST_INTERROGATION] == QUESTNOTSTARTED) { return std::size(gModSettings.iMeanwhileInterrogatePOWGridNo); }
#endif
	return 0;
}

void EnemyCapturesPlayerSoldier( SOLDIERTYPE *pSoldier )
{
#ifndef JA2UB
	AssertNotNIL(pSoldier);

	// ATE: Check first if ! in player captured sequence already
	if ( ( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE ) && (gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_ESCAPE) )
	{
		return;
	}

	// If this is an EPC , just kill them...
	if ( AM_AN_EPC( pSoldier ) || AM_A_ROBOT(pSoldier))
	{
		pSoldier->stats.bLife = 0;
		pSoldier->iHealableInjury = 0; // added by SANDRO
		BOOLEAN fMadeCorpse;
		HandleSoldierDeath( pSoldier, &fMadeCorpse );
		return;
	}

	// can't capture mercs on mini event adventures
	if (pSoldier->bAssignment == ASSIGNMENT_MINIEVENT)
	{
		return;
	}
	
	if (pSoldier->bAssignment == ASSIGNMENT_REBELCOMMAND)
	{
		return;
	}
	
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	if (gStrategicStatus.ubNumCapturedForRescue >= CalculateMaximumPrisonerAmount())
	{
		SetTimeOfAssignmentChangeForMerc(pSoldier);
		return;
	}

	if (gStrategicStatus.ubNumCapturedForRescue < 3 && (gubQuest[QUEST_HELD_IN_ALMA] == QUESTNOTSTARTED || gubQuest[QUEST_HELD_IN_TIXA] == QUESTNOTSTARTED || gubQuest[QUEST_INTERROGATION] == QUESTNOTSTARTED))
	{
		// ATE: Patch fix If in a vehicle, remove from vehicle...
		TakeSoldierOutOfVehicle(pSoldier);

		HandleMoraleEvent(pSoldier, MORALE_MERC_CAPTURED, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ);

		// Change to POW....
		//-add him to a POW assignment/group
		if ((pSoldier->bAssignment != ASSIGNMENT_POW))
		{
			SetTimeOfAssignmentChangeForMerc(pSoldier);
		}

		ChangeSoldiersAssignment(pSoldier, ASSIGNMENT_POW);
		RemoveCharacterFromSquads(pSoldier);


		INT32 itemdropoffgridno = -1;
		// Is this the first one..?
		if ( gubQuest[QUEST_HELD_IN_ALMA] == QUESTNOTSTARTED )
		{
			//-teleport him to NE Alma sector (not Tixa as originally planned)
			pSoldier->aiData.bNeutral = TRUE;
			pSoldier->sSectorX = gModSettings.ubInitialPOWSectorX; //13
			pSoldier->sSectorY = gModSettings.ubInitialPOWSectorY; //9
			pSoldier->bSectorZ = 0;
			pSoldier->usStrategicInsertionData = gModSettings.iInitialPOWGridNo[gStrategicStatus.ubNumCapturedForRescue];
			itemdropoffgridno = gModSettings.iInitialPOWItemGridNo[gStrategicStatus.ubNumCapturedForRescue];
		}
		else if (gubQuest[QUEST_HELD_IN_TIXA] == QUESTNOTSTARTED)
		{
			//-teleport him to Tixa as originally planned
			pSoldier->aiData.bNeutral = TRUE;
			pSoldier->sSectorX = gModSettings.ubTixaPrisonSectorX;
			pSoldier->sSectorY = gModSettings.ubTixaPrisonSectorY;
			pSoldier->bSectorZ = 0;
			pSoldier->usStrategicInsertionData = gModSettings.iTixaPrisonPOWGridNo[gStrategicStatus.ubNumCapturedForRescue];
			itemdropoffgridno = gModSettings.iTixaPrisonPOWItemGridNo[gStrategicStatus.ubNumCapturedForRescue];
		}
		else //if ( gubQuest[QUEST_HELD_IN_ALMA] == QUESTDONE )
		{
			//-teleport him to N7
			pSoldier->sSectorX = gModSettings.ubMeanwhileInterrogatePOWSectorX; //7
			pSoldier->sSectorY = gModSettings.ubMeanwhileInterrogatePOWSectorY; //14
			pSoldier->bSectorZ = 0;
			pSoldier->usStrategicInsertionData = gModSettings.iMeanwhileInterrogatePOWGridNo[gStrategicStatus.ubNumCapturedForRescue];
			itemdropoffgridno = gModSettings.iMeanwhileInterrogatePOWItemGridNo[gStrategicStatus.ubNumCapturedForRescue];
		}
		
		// OK, drop all items!
		WORLDITEM WorldItem;
		std::vector<WORLDITEM> pWorldItem;
		UINT32 invsize = pSoldier->inv.size();
		for (UINT32 i = 0; i < invsize; ++i )
		{
			if ( pSoldier->inv[i].exists() )
			{
				WorldItem.fExists = TRUE;
				WorldItem.sGridNo = itemdropoffgridno;
				WorldItem.ubLevel = 0;
				WorldItem.usFlags = 0;
				WorldItem.bVisible = FALSE;
				WorldItem.bRenderZHeightAboveLevel = 0;
				pSoldier->inv[i].MoveThisObjectTo( WorldItem.object );
				pWorldItem.push_back( WorldItem );
			}
		}

		AddWorldItemsToUnLoadedSector( pSoldier->sSectorX, pSoldier->sSectorY, 0, itemdropoffgridno, pWorldItem.size(), pWorldItem, FALSE );

		// put him on the floor!!
		pSoldier->pathing.bLevel = 0;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;

		gStrategicStatus.ubNumCapturedForRescue++;

		//Bandaging him would prevent him from dying (due to low HP)
		pSoldier->bBleeding = 0;

		// wake him up
		if ( pSoldier->flags.fMercAsleep )
		{
			PutMercInAwakeState( pSoldier );
			pSoldier->flags.fForcedToStayAwake = FALSE;
		}

		//Set his life to 50% + or - 10 HP.
		INT8 oldlife = pSoldier->stats.bLife;
		pSoldier->stats.bLife = max(35, pSoldier->stats.bLifeMax / 2);
	
		if ( pSoldier->stats.bLife >= 45 )
		{
			pSoldier->stats.bLife += (INT8)(10 - Random( 21 ) );
		}
		
		// SANDRO - make the lost life insta-healable
		pSoldier->iHealableInjury = ((pSoldier->stats.bLifeMax - pSoldier->stats.bLife) * 100);

		// make him quite exhausted when found
		pSoldier->bBreath = pSoldier->bBreathMax = 50;
		pSoldier->sBreathRed = 0;
		pSoldier->flags.fMercCollapsedFlag = FALSE;


		RemoveSoldierFromTacticalSector(pSoldier, TRUE);
		RemovePlayerFromTeamSlotGivenMercID(pSoldier->ubID);
		SelectNextAvailSoldier(pSoldier);
	}
#endif 	
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
	if ( NumNonPlayerTeamMembersInSector( SECTORX( ubSectorID ), SECTORY( ubSectorID ), MILITIA_TEAM ) )
	{
		//militia in sector
		return TRUE;
	}

	if ( FindMovementGroupInSector( SECTORX( ubSectorID ), SECTORY( ubSectorID ), OUR_TEAM ) )
	{
		// player/militia in sector
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

// Flugente check whether a team has nearby members that can be added to the current battle
// Note that this requires separate counts of teammembers in the sector and those that are actually fighting
// This currently does not exist for MILITIA_TEAM, making such a check relatively useless
BOOLEAN CheckPendingNonPlayerTeam( UINT8 usTeam )
{
	if( gbWorldSectorZ )
		return FALSE;

	SECTORINFO *pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];

	if ( usTeam == ENEMY_TEAM )
	{
		if ( (pSector->ubNumElites + pSector->ubNumTroops + pSector->ubNumAdmins + pSector->ubNumTanks + pSector->ubNumJeeps + pSector->ubNumRobots) > 
			 (pSector->ubElitesInBattle + pSector->ubTroopsInBattle + pSector->ubAdminsInBattle + pSector->ubTanksInBattle + pSector->ubJeepsInBattle + pSector->ubRobotsInBattle) )
			return TRUE;
	}

	for (GROUP *pGroup = gpGroupList; pGroup; pGroup = pGroup->next)
	{
		if ( pGroup->usGroupTeam == usTeam
			&& !pGroup->fVehicle 
			&& pGroup->ubSectorX == gWorldSectorX 
			&& pGroup->ubSectorY == gWorldSectorY )
		{
			if ( usTeam == ENEMY_TEAM )
			{
				if ( pGroup->ubGroupSize > pGroup->pEnemyGroup->ubElitesInBattle + pGroup->pEnemyGroup->ubTroopsInBattle + pGroup->pEnemyGroup->ubAdminsInBattle 
					 + pGroup->pEnemyGroup->ubTanksInBattle + pGroup->pEnemyGroup->ubJeepsInBattle + pGroup->pEnemyGroup->ubRobotsInBattle )
					return TRUE;
			}
			else if ( usTeam == MILITIA_TEAM )
			{
				if ( pGroup->ubGroupSize > pSector->ubNumberOfCivsAtLevel[GREEN_MILITIA] + pSector->ubNumberOfCivsAtLevel[REGULAR_MILITIA] + pSector->ubNumberOfCivsAtLevel[ELITE_MILITIA] )
					return TRUE;
			}
		}
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
					TacticalCharacterDialogue( &Menptr[ bId1 ], QUOTE_UB_HANDLE_BLOODCATDEATHS );
				}
				else if( bId2 != NOBODY && Menptr[ bId2 ].ubProfile != TEX_UB )
				{
					TacticalCharacterDialogue( &Menptr[ bId2 ], QUOTE_UB_HANDLE_BLOODCATDEATHS );
				}
			}
		}
	}
}
#endif


UINT16 NumTurncoatsOfClassInSector( INT16 sSectorX, INT16 sSectorY, UINT8 aSoldierClass )
{
	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
	// probably isn't. But I need this bit to work.
	if ( sSectorX < MINIMUM_VALID_X_COORDINATE ||
		sSectorX > MAXIMUM_VALID_X_COORDINATE ||
		sSectorY < MINIMUM_VALID_Y_COORDINATE ||
		sSectorY > MAXIMUM_VALID_Y_COORDINATE )
	{
		return ( 0 );
	}

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE );
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE );
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	SECTORINFO* pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];

	UINT16 num = 0;

	switch ( aSoldierClass )
	{
	case SOLDIER_CLASS_ADMINISTRATOR:	num += pSector->ubNumAdmins_Turncoat;	break;
	case SOLDIER_CLASS_ARMY:            num += pSector->ubNumTroops_Turncoat; 	break;
	case SOLDIER_CLASS_ELITE:           num += pSector->ubNumElites_Turncoat;	break;
	default:							break;
	}

	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			switch ( aSoldierClass )
			{
			case SOLDIER_CLASS_ADMINISTRATOR:	num += pGroup->pEnemyGroup->ubNumAdmins_Turncoat;	break;
			case SOLDIER_CLASS_ARMY:            num += pGroup->pEnemyGroup->ubNumTroops_Turncoat; 	break;
			case SOLDIER_CLASS_ELITE:           num += pGroup->pEnemyGroup->ubNumElites_Turncoat;	break;
			default:							break;
			}
		}
		pGroup = pGroup->next;
	}

	return num;
}

BOOLEAN TurncoatsInSector( INT16 sSectorX, INT16 sSectorY )
{
	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
	// probably isn't. But I need this bit to work.
	if ( sSectorX < MINIMUM_VALID_X_COORDINATE ||
		sSectorX > MAXIMUM_VALID_X_COORDINATE ||
		sSectorY < MINIMUM_VALID_Y_COORDINATE ||
		sSectorY > MAXIMUM_VALID_Y_COORDINATE )
	{
		return ( 0 );
	}

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE );
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE );
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	SECTORINFO* pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];
	
	if ( pSector->ubNumAdmins_Turncoat
		|| pSector->ubNumTroops_Turncoat
		|| pSector->ubNumElites_Turncoat )
		return TRUE;
	
	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			if ( pGroup->pEnemyGroup->ubNumAdmins_Turncoat
				|| pGroup->pEnemyGroup->ubNumTroops_Turncoat
				|| pGroup->pEnemyGroup->ubNumElites_Turncoat )
				return TRUE;
		}
		pGroup = pGroup->next;
	}

	return FALSE;
}

bool RemoveOneTurncoat( INT16 sSectorX, INT16 sSectorY, UINT8 aSoldierClass, BOOLEAN alsoRemoveFromGroup )
{
	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
	// probably isn't. But I need this bit to work.
	if ( sSectorX < MINIMUM_VALID_X_COORDINATE ||
		sSectorX > MAXIMUM_VALID_X_COORDINATE ||
		sSectorY < MINIMUM_VALID_Y_COORDINATE ||
		sSectorY > MAXIMUM_VALID_Y_COORDINATE )
	{
		return false;
	}

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE );
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE );
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	SECTORINFO* pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];

	// count how many turncoats we have
	UINT16 num_static = 0;
	UINT16 num_groups = 0;

	switch ( aSoldierClass )
	{
	case SOLDIER_CLASS_ADMINISTRATOR:
		num_static = pSector->ubNumAdmins_Turncoat;
		break;
	case SOLDIER_CLASS_ARMY:
		num_static = pSector->ubNumTroops_Turncoat;
		break;
	case SOLDIER_CLASS_ELITE:
		num_static = pSector->ubNumElites_Turncoat;
		break;
	default:
		break;
	}

	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			switch ( aSoldierClass )
			{
			case SOLDIER_CLASS_ADMINISTRATOR:
				num_groups += pGroup->pEnemyGroup->ubNumAdmins_Turncoat;
				break;
			case SOLDIER_CLASS_ARMY:
				num_groups += pGroup->pEnemyGroup->ubNumTroops_Turncoat;
				break;
			case SOLDIER_CLASS_ELITE:
				num_groups += pGroup->pEnemyGroup->ubNumElites_Turncoat;
				break;
			default:
				break;
			}
		}
		pGroup = pGroup->next;
	}

	if ( num_static + num_groups )
	{
		if ( Chance( 100 * num_static / ( num_static + num_groups ) ) )
		{
			// add to garrison
			switch ( aSoldierClass )
			{
			case SOLDIER_CLASS_ADMINISTRATOR:
				pSector->ubNumAdmins_Turncoat--;
				if (alsoRemoveFromGroup)
				{
					pSector->ubNumAdmins--;
					pSector->ubAdminsInBattle -= (pSector->ubAdminsInBattle == 0 ? 0 : 1);
				}
				return true;
				break;
			case SOLDIER_CLASS_ARMY:
				pSector->ubNumTroops_Turncoat--;
				if (alsoRemoveFromGroup)
				{
					pSector->ubNumTroops--;
					pSector->ubTroopsInBattle -= (pSector->ubTroopsInBattle == 0 ? 0 : 1);
				}
				return true;
				break;
			case SOLDIER_CLASS_ELITE:
				pSector->ubNumElites_Turncoat--;
				if (alsoRemoveFromGroup)
				{
					pSector->ubNumElites--;
					pSector->ubElitesInBattle -= (pSector->ubElitesInBattle == 0 ? 0 : 1);
				}
				return true;
				break;
			default:
				break;
			}
		}
		else
		{
			GROUP* pGroup = gpGroupList;
			while ( pGroup )
			{
				if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
				{
					switch ( aSoldierClass )
					{
					case SOLDIER_CLASS_ADMINISTRATOR:
						if ( pGroup->pEnemyGroup->ubNumAdmins_Turncoat && pGroup->ubGroupSize && pGroup->pEnemyGroup->ubNumAdmins )
						{
							pGroup->pEnemyGroup->ubNumAdmins_Turncoat--;
							if (alsoRemoveFromGroup)
							{
								pGroup->pEnemyGroup->ubNumAdmins--;
								pGroup->pEnemyGroup->ubAdminsInBattle -= (pGroup->pEnemyGroup->ubAdminsInBattle == 0 ? 0 : 1);
								pGroup->ubGroupSize--;
							}
							return true;
						}
						break;
					case SOLDIER_CLASS_ARMY:
						if ( pGroup->pEnemyGroup->ubNumTroops_Turncoat && pGroup->ubGroupSize && pGroup->pEnemyGroup->ubNumTroops )
						{
							pGroup->pEnemyGroup->ubNumTroops_Turncoat--;
							if (alsoRemoveFromGroup)
							{
								pGroup->pEnemyGroup->ubNumTroops--;
								pGroup->pEnemyGroup->ubTroopsInBattle -= (pGroup->pEnemyGroup->ubTroopsInBattle == 0 ? 0 : 1);
								pGroup->ubGroupSize--;
							}
							return true;
						}
						break;
					case SOLDIER_CLASS_ELITE:
						if ( pGroup->pEnemyGroup->ubNumElites_Turncoat && pGroup->ubGroupSize && pGroup->pEnemyGroup->ubNumElites )
						{
							pGroup->pEnemyGroup->ubNumElites_Turncoat--;
							if (alsoRemoveFromGroup)
							{
								pGroup->pEnemyGroup->ubNumElites--;
								pGroup->pEnemyGroup->ubElitesInBattle -= (pGroup->pEnemyGroup->ubElitesInBattle == 0 ? 0 : 1);
								pGroup->ubGroupSize--;
							}
							return true;
						}
						break;
					default:
						break;
					}
				}
				pGroup = pGroup->next;
			}
		}
	}

	return false;
}

bool AddOneTurncoat( INT16 sSectorX, INT16 sSectorY, UINT8 aSoldierClass )
{
	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
	// probably isn't. But I need this bit to work.
	if ( sSectorX < MINIMUM_VALID_X_COORDINATE ||
		sSectorX > MAXIMUM_VALID_X_COORDINATE ||
		sSectorY < MINIMUM_VALID_Y_COORDINATE ||
		sSectorY > MAXIMUM_VALID_Y_COORDINATE )
	{
		return false;
	}

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE );
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE );
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	SECTORINFO* pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];

	// count how many 'slots' we have for more turncoats, then add one at a random position
	UINT16 freeslots_static = 0;
	UINT16 freeslots_groups = 0;

	switch ( aSoldierClass )
	{
	case SOLDIER_CLASS_ADMINISTRATOR:
		if ( pSector->ubNumAdmins_Turncoat < pSector->ubNumAdmins )
			freeslots_static = pSector->ubNumAdmins - pSector->ubNumAdmins_Turncoat;
		break;
	case SOLDIER_CLASS_ARMY:
		if ( pSector->ubNumTroops_Turncoat < pSector->ubNumTroops )
			freeslots_static = pSector->ubNumTroops - pSector->ubNumTroops_Turncoat;
		break;
	case SOLDIER_CLASS_ELITE:
		if ( pSector->ubNumElites_Turncoat < pSector->ubNumElites )
			freeslots_static = pSector->ubNumElites - pSector->ubNumElites_Turncoat;
		break;
	default:
		break;
	}

	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			switch ( aSoldierClass )
			{
			case SOLDIER_CLASS_ADMINISTRATOR:
				if ( pGroup->pEnemyGroup->ubNumAdmins_Turncoat < pGroup->pEnemyGroup->ubNumAdmins )
					freeslots_groups += pGroup->pEnemyGroup->ubNumAdmins - pGroup->pEnemyGroup->ubNumAdmins_Turncoat;
				break;
			case SOLDIER_CLASS_ARMY:
				if ( pGroup->pEnemyGroup->ubNumTroops_Turncoat < pGroup->pEnemyGroup->ubNumTroops )
					freeslots_groups += pGroup->pEnemyGroup->ubNumTroops - pGroup->pEnemyGroup->ubNumTroops_Turncoat;
				break;
			case SOLDIER_CLASS_ELITE:
				if ( pGroup->pEnemyGroup->ubNumElites_Turncoat < pGroup->pEnemyGroup->ubNumElites )
					freeslots_groups += pGroup->pEnemyGroup->ubNumElites - pGroup->pEnemyGroup->ubNumElites_Turncoat;
				break;
			default:
				break;
			}
		}
		pGroup = pGroup->next;
	}

	if ( freeslots_static + freeslots_groups )
	{
		if ( Chance( 100 * freeslots_static / ( freeslots_static + freeslots_groups ) ) )
		{
			// add to garrison
			switch ( aSoldierClass )
			{
			case SOLDIER_CLASS_ADMINISTRATOR:
				pSector->ubNumAdmins_Turncoat++;
				return true;
				break;
			case SOLDIER_CLASS_ARMY:
				pSector->ubNumTroops_Turncoat++;
				return true;
				break;
			case SOLDIER_CLASS_ELITE:
				pSector->ubNumElites_Turncoat++;
				return true;
				break;
			default:
				break;
			}
		}
		else
		{
			GROUP* pGroup = gpGroupList;
			while ( pGroup )
			{
				if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
				{
					switch ( aSoldierClass )
					{
					case SOLDIER_CLASS_ADMINISTRATOR:
						if ( pGroup->pEnemyGroup->ubNumAdmins_Turncoat < pGroup->pEnemyGroup->ubNumAdmins )
						{
							pGroup->pEnemyGroup->ubNumAdmins_Turncoat++;
							return true;
						}
						break;
					case SOLDIER_CLASS_ARMY:
						if ( pGroup->pEnemyGroup->ubNumTroops_Turncoat < pGroup->pEnemyGroup->ubNumTroops )
						{
							pGroup->pEnemyGroup->ubNumTroops_Turncoat++;
							return true;
						}
						break;
					case SOLDIER_CLASS_ELITE:
						if ( pGroup->pEnemyGroup->ubNumElites_Turncoat < pGroup->pEnemyGroup->ubNumElites )
						{
							pGroup->pEnemyGroup->ubNumElites_Turncoat++;
							return true;
						}
						break;
					default:
						break;
					}
				}
				pGroup = pGroup->next;
			}
		}
	}

	return false;
}

void CorrectTurncoatCount( INT16 sSectorX, INT16 sSectorY )
{
	// HEADROCK: This is a TEMPORARY fix to avoid the assertion error. Not sure this is the best solution,
	// probably isn't. But I need this bit to work.
	if ( sSectorX < MINIMUM_VALID_X_COORDINATE ||
		sSectorX > MAXIMUM_VALID_X_COORDINATE ||
		sSectorY < MINIMUM_VALID_Y_COORDINATE ||
		sSectorY > MAXIMUM_VALID_Y_COORDINATE )
	{
		return;
	}

	AssertGE( sSectorX, MINIMUM_VALID_X_COORDINATE );
	AssertLE( sSectorX, MAXIMUM_VALID_X_COORDINATE );
	AssertGE( sSectorY, MINIMUM_VALID_Y_COORDINATE );
	AssertLE( sSectorY, MAXIMUM_VALID_Y_COORDINATE );

	SECTORINFO* pSector = &SectorInfo[SECTOR( sSectorX, sSectorY )];

	if ( pSector->ubNumAdmins_Turncoat > pSector->ubNumAdmins )
	{
		pSector->ubNumTroops_Turncoat += pSector->ubNumAdmins_Turncoat - pSector->ubNumAdmins;
		pSector->ubNumAdmins_Turncoat = pSector->ubNumAdmins;
	}
	if ( pSector->ubNumTroops_Turncoat > pSector->ubNumTroops )
	{
		pSector->ubNumElites_Turncoat += pSector->ubNumTroops_Turncoat - pSector->ubNumTroops;
		pSector->ubNumTroops_Turncoat = pSector->ubNumTroops;
	}
	if ( pSector->ubNumElites_Turncoat > pSector->ubNumElites )
	{
		pSector->ubNumElites_Turncoat = pSector->ubNumElites;
	}

	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == ENEMY_TEAM && !pGroup->fVehicle && pGroup->ubSectorX == sSectorX && pGroup->ubSectorY == sSectorY )
		{
			if ( pGroup->pEnemyGroup->ubNumAdmins_Turncoat > pGroup->pEnemyGroup->ubNumAdmins )
			{
				pGroup->pEnemyGroup->ubNumTroops_Turncoat += pGroup->pEnemyGroup->ubNumAdmins_Turncoat - pGroup->pEnemyGroup->ubNumAdmins;
				pGroup->pEnemyGroup->ubNumAdmins_Turncoat = pGroup->pEnemyGroup->ubNumAdmins;
			}
			if ( pGroup->pEnemyGroup->ubNumTroops_Turncoat > pGroup->pEnemyGroup->ubNumTroops )
			{
				pGroup->pEnemyGroup->ubNumElites_Turncoat += pGroup->pEnemyGroup->ubNumTroops_Turncoat - pGroup->pEnemyGroup->ubNumTroops;
				pGroup->pEnemyGroup->ubNumTroops_Turncoat = pGroup->pEnemyGroup->ubNumTroops;
			}
			if ( pGroup->pEnemyGroup->ubNumElites_Turncoat > pGroup->pEnemyGroup->ubNumElites )
			{
				pGroup->pEnemyGroup->ubNumElites_Turncoat = pGroup->pEnemyGroup->ubNumElites;
			}
		}
		pGroup = pGroup->next;
	}
}


