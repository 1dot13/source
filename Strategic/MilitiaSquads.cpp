#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Town Militia.h"
	#include "Militia Control.h"
	#include "Campaign Types.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "Overhead.h"
	#include "Strategic Town Loyalty.h"
	#include "Utilities.h"
	#include "random.h"
	#include "text.h"
	#include "Map Screen Interface.h"
	#include "Interface.h"
	#include "Laptopsave.h"
	#include "finances.h"
	#include "Game Clock.h"
	#include "Assignments.h"
	#include "squads.h"
	#include "Soldier Create.h"
	#include "Dialogue Control.h"
	#include "GameSettings.h"
	#include "Queen Command.h"
	#include "math.h"
	#include "Auto Resolve.h"
	#include "Vehicles.h"
	#include "Tactical Save.h"
#endif

#include "connect.h"
#include "MilitiaSquads.h"
#include "Reinforcement.h"

// Debug defines

//#define DEBUG_SHOW_RATINGS
#define DEBUG_RATINGS_CONDITION !fForBattle


// will create a squad each n hours (12 for example). Should divide 24 without remainder
//#define CREATE_EACH_N_HOURS 12

// more these values - less the chances
#define CHANCE_TO_MOVE_A_SQUAD 5
#define CHANCE_TO_MOVE_AN_UNFULL_SQUAD 40

//#define DIV_OF_ORIGINAL_MILITIA 2
#define BASE_DIR_PRIORITY 5
// little chance to move if below minimum

//#define MINIMUM_MILITIA_SQUAD_SIZE 10
// should be 2* Minimum
//#define MAXIMUM_MILITIA_SQUAD_SIZE 30
 //#define MAXIMUM_MILITIA_SQUAD_SIZE_BATTLE MAXIMUM_MILITIA_SQUAD_SIZE

#define DIR_WITH_UNFULL_SQUAD_RATING_BONUS 100
// rating of battle, when there's equal number of soldiers
//#define DIR_RATING_OF_BATTLE 10
#define DIR_MIN_DIF 0.75

#define DIR_NORTH 0
#define DIR_EAST 1
#define DIR_SOUTH 2
#define DIR_WEST 3

INT32	iRestrictedSectorArraySize;
UINT32 gRestrictMilitia[256];
// HEADROCK HAM B1: Alternate array keeps track of dynamically unrestricted sectors
BOOLEAN gDynamicRestrictMilitia[ 256 ];
// HEADROCK HAM B1: Function that dynamically unrestricts sectors as we take over towns.
extern void AdjustRoamingRestrictions();

UINT8 gpAttackDirs[5][4]; // 0. Green Militia 1. Regular Militia 2. Elite Militia 3. Insertion code
UINT8 guiDirNumber = 0;
BOOLEAN gfMSBattle = FALSE;

INT16 sMSMapX, sMSMapY;

extern UINT8 GetTraversability( INT16 sStartSector, INT16 sEndSector );
extern void TriggerPrebattleInterface( UINT8 ubResult );
extern BOOLEAN gfEnterAutoResolveMode;


//Counts the number of live mercs in any given sector.
BOOLEAN PlayerMercsInSector_MSE( UINT8 ubSectorX, UINT8 ubSectorY, BOOLEAN fDontConsiderNextSector )
{
	GROUP *pGroup;
	PLAYERGROUP *pPlayer;
	UINT8 ubNumMercs = 0;
	pGroup = gpGroupList;
	while( pGroup )
	{
		if( pGroup->fPlayer )// && !pGroup->fBetweenSectors)
		{
			if ( ( pGroup->ubSectorX == ubSectorX && pGroup->ubSectorY == ubSectorY && pGroup->ubSectorZ == 0 ) ||
				( !fDontConsiderNextSector && ( pGroup->ubNextX == ubSectorX && pGroup->ubNextY == ubSectorY && pGroup->ubSectorZ == 0 ) ) )
			{
				//we have a group, make sure that it isn't a group containing only dead members.
				pPlayer = pGroup->pPlayerList;
				while( pPlayer )
				{
					// robots count as mercs here, because they can fight, but vehicles don't
					if( ( pPlayer->pSoldier->stats.bLife ) && !( pPlayer->pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
					{
						ubNumMercs++;
					}
					pPlayer = pPlayer->next;
				}
			}
		}
		pGroup = pGroup->next;
	}
	return ubNumMercs;
}


// get any enemy group id in the sector (if no group, will return 0 )
UINT8 GetEnemyGroupIdInSector( INT16 sMapX, INT16 sMapY )
{
	UINT8 ubRes = 0;
	GROUP *curr;
	curr = gpGroupList;
	while( curr )
	{
		if( curr->ubSectorX == sMapX && curr->ubSectorY == sMapY && !curr->fPlayer )
			if( !curr->ubGroupID )
				return curr->ubGroupID;
			else
				ubRes = curr->ubGroupID;
		curr = curr->next;
	}
	return ubRes;
}


// Count all militia in the sector
UINT8 CountMilitia(SECTORINFO *pSectorInfo)
{
	return pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA]+
	pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]+
	pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
}

// Creates militia at destination sector. The type and amount of militia depends on current sector's miltia type and amount
void GenerateMilitiaSquad(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	//HEADROCK HAM B2.7: Track the number of militia at the source.
	UINT16 bTotalMilitiaAtSource = pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] + pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] + pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
	UINT8 bTargetGreen, bTargetRegular, bTargetElite;
	UINT8 bUpgradePoints;

	if (gGameExternalOptions.fDiverseRoamingMilitiaGroups)
	{
		bTargetRegular = __max(1,bTotalMilitiaAtSource / (gGameExternalOptions.guiDivOfOriginalMilitia * 4));
		bTargetElite = __max(1,bTotalMilitiaAtSource / (gGameExternalOptions.guiDivOfOriginalMilitia * 4));
		bTargetGreen = (bTotalMilitiaAtSource / gGameExternalOptions.guiDivOfOriginalMilitia) - (bTargetRegular + bTargetElite);

		pSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );
		
		// Continue if we're dealing with a full group that isn't full of veterans
		if ( CountMilitia(pSectorInfo) >= gGameExternalOptions.guiMaxMilitiaSquadSize && pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] < CountMilitia(pSectorInfo) )
		{
			bUpgradePoints = bTargetGreen + (bTargetRegular * 2);
			bTargetElite += __min(bTargetRegular, pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]);
			bUpgradePoints -= (__min(bTargetRegular, pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]))*2;
			bTargetRegular = bUpgradePoints;
			bTargetGreen = 0;
		}
		
		// HEADROCK HAM B2.7: Create 1/2 green, 1/4 regular, 1/4 elite.
		StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, bTargetGreen);
		StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, bTargetRegular);
		StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, bTargetElite);
	}

	else
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] / gGameExternalOptions.guiDivOfOriginalMilitia);
		StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] / gGameExternalOptions.guiDivOfOriginalMilitia);
		StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] / gGameExternalOptions.guiDivOfOriginalMilitia);
	}

	pSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	while( CountMilitia(pSectorInfo) > gGameExternalOptions.guiMaxMilitiaSquadSize )
	{
		if(pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA])
		{
			--pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
		}
		else if(pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA])
		{
			--pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
		}
		else if(pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA])
		{
			--pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
		}
	}

	// Update the militia if the current sector is affected
	if (gfStrategicMilitiaChangesMade)
	{
		ResetMilitia();
	}
}

// Creates militia at destination sector and removes it from starting sector
void MoveMilitiaSquad(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY, BOOLEAN fAlternativeMax )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	SECTORINFO *pTSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	UINT8 bGreensSourceTeam = 0, bGreensDestTeam = 0;
	UINT8 bRegularsSourceTeam = 0, bRegularsDestTeam = 0;
	UINT8 bElitesSourceTeam = 0, bElitesDestTeam = 0;
	UINT8 bTotalGreens = 0, bTotalRegulars = 0, bTotalElites = 0;
	UINT8 bTotalGreensPercent = 0, bTotalRegularsPercent = 0, bTotalElitesPercent = 0;
	INT8 bNewSourceGroupSize = 0, bNewDestGroupSize = 0, bGroupSizeRatio = 0;

	// HEADROCK HAM B2.8
	// There are now several methods of militia join-up.
	if (PreRandom(100) < gGameExternalOptions.ubRoamingMilitiaSpreadsOutChance)
	{
		// Militia will attempt to spread out, averaging the size and composition of both groups.
		if ( CountMilitia(pSectorInfo) && CountMilitia(pTSectorInfo) )
		{
			bGreensSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
			bGreensDestTeam = pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
			bRegularsSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
			bRegularsDestTeam = pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
			bElitesSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
			bElitesDestTeam = pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

			bTotalGreens = bGreensSourceTeam + bGreensDestTeam;
			bTotalRegulars = bRegularsSourceTeam + bRegularsDestTeam;
			bTotalElites = bElitesSourceTeam + bElitesDestTeam;

			// Source team gets half of each group, rounded down
			bGreensSourceTeam = bTotalGreens / 2;
			bRegularsSourceTeam = bTotalRegulars / 2;
			bElitesSourceTeam = bTotalElites / 2;

			// Destination team gets half of each group, rounded up
			bGreensDestTeam = bTotalGreens - bGreensSourceTeam;
			bRegularsDestTeam = bTotalRegulars - bRegularsSourceTeam;
			bElitesDestTeam = bTotalElites - bElitesSourceTeam;

			// Erase ALL militia from both locations.
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, GREEN_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, REGULAR_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, ELITE_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );

			// Add new militia.
			StrategicAddMilitiaToSector( sMapX, sMapY, GREEN_MILITIA, bGreensSourceTeam );
			StrategicAddMilitiaToSector( sMapX, sMapY, REGULAR_MILITIA, bRegularsSourceTeam );
			StrategicAddMilitiaToSector( sMapX, sMapY, ELITE_MILITIA, bElitesSourceTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, bGreensDestTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, bRegularsDestTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, bElitesDestTeam );
		}
		else
		{
			StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
			
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
		}
	}
	else 
	{
		if ( gGameExternalOptions.fDiverseRoamingMilitiaGroups )
		{
			// Diverse Militia Join-up: Destination team gets as many troops as possible, but both teams
			// keep an averaged green-regular-elite ratio.
			bNewDestGroupSize = __min((CountMilitia(pTSectorInfo) + CountMilitia(pSectorInfo)), (UINT8)gGameExternalOptions.guiMaxMilitiaSquadSize);
			bNewSourceGroupSize = __max(0,(CountMilitia(pTSectorInfo) + CountMilitia(pSectorInfo)) - gGameExternalOptions.guiMaxMilitiaSquadSize);

			// If there are still going to be two teams after the transfer
			if ( bNewSourceGroupSize > 0 )
			{

				bGreensSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
				bGreensDestTeam = pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
				bRegularsSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
				bRegularsDestTeam = pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
				bElitesSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
				bElitesDestTeam = pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

				bTotalGreens = bGreensSourceTeam + bGreensDestTeam;
				bTotalRegulars = bRegularsSourceTeam + bRegularsDestTeam;
				bTotalElites = bElitesSourceTeam + bElitesDestTeam;

				bGroupSizeRatio = __max(1,(bNewDestGroupSize / bNewSourceGroupSize));

				// Destination team gets the majority of troops (based on the ratios), 
				// but will always leave at least 1 troop of each type (if available) for the source team
				bGreensDestTeam = __min(__min(bTotalGreens, ((bTotalGreens * bGroupSizeRatio) / (bGroupSizeRatio + 1) +1) ), __max(0,bTotalGreens-1));
				bRegularsDestTeam = __min(__min(bTotalRegulars, ((bTotalRegulars * bGroupSizeRatio) / (bGroupSizeRatio + 1) +1) ), __max(0,bTotalRegulars-1));
				bElitesDestTeam = __min(__min(bTotalElites, ((bTotalElites * bGroupSizeRatio) / (bGroupSizeRatio + 1) +1) ), __max(0,bTotalElites-1));

				// Source team gets the remainder (no less than one troop of each type, if available)
				bGreensSourceTeam = bTotalGreens - bGreensDestTeam;
				bRegularsSourceTeam = bTotalRegulars - bRegularsDestTeam;
				bElitesSourceTeam = bTotalElites - bElitesDestTeam;

				// Erase ALL militia from both locations.
				StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, GREEN_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, REGULAR_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, ELITE_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );

				// Add new militia.
				StrategicAddMilitiaToSector( sMapX, sMapY, GREEN_MILITIA, bGreensSourceTeam );
				StrategicAddMilitiaToSector( sMapX, sMapY, REGULAR_MILITIA, bRegularsSourceTeam );
				StrategicAddMilitiaToSector( sMapX, sMapY, ELITE_MILITIA, bElitesSourceTeam );
				StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, bGreensDestTeam );
				StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, bRegularsDestTeam );
				StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, bElitesDestTeam );
			}
			else
			{
				// Move all militia from source to destination.
				StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
				StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
				StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
			
				StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
				StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
			}
		}
		else
		{
		// normal join-up. Destination group gets all the best militia. Source team gets the leftovers.
		StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
		StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
		StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
		
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
		}
	}

	while( !fAlternativeMax && CountMilitia(pTSectorInfo) > gGameExternalOptions.guiMaxMilitiaSquadSize ||
		fAlternativeMax && CountMilitia(pTSectorInfo) > gGameExternalOptions.guiMaxMilitiaSquadSizeBattle )
	{
		if(pTSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA])
		{
			--pTSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
			++pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
		}
		else if(pTSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA])
		{
			--pTSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
			++pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
		}
		else if(pTSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA])
		{
			--pTSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
			++pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
		}
	}

	if (gfStrategicMilitiaChangesMade)
	{
		ResetMilitia();
	}
}

BOOLEAN MoveOneBestMilitiaMan(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
//	SECTORINFO *pTSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	if( pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, 1 );
		return TRUE;
	}
	if( pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, 1 );
		return TRUE;
	}
	if( pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, 1 );
		return TRUE;
	}
	return FALSE;
}

// Generate rating for a direction
UINT16 CountDirectionEnemyRating( INT16 sMapX, INT16 sMapY, UINT8 uiDir )
{
	DOUBLE ddMinAngle, ddMaxAngle;
	DOUBLE ddRes = 0;
	DOUBLE pix2 = 3.1415926 * 2; // pi * 2
	//UINT16 uiSector = 0;
	INT16 sLMX, sLMY;

	switch(uiDir)
	{
	case DIR_NORTH:
		ddMinAngle = -2.3561944; // -135 degrees
		ddMaxAngle = -0.7853981; // -45 degrees
		break;
	case DIR_EAST:
		ddMinAngle = -0.7853981; // -45 degrees
		ddMaxAngle = 0.7853981; // 45 degrees
		break;
	case DIR_SOUTH:
		ddMinAngle = 0.7853981; // 45 degrees
		ddMaxAngle = 2.3561944; // 135 degrees
		break;
	case DIR_WEST:
		ddMinAngle = 2.3561944; // 135 degrees
		ddMaxAngle = -2.3561944 + pix2; // 225 degrees(-135)
		break;
	default:
		return 0;
	}

	//for( uiSector = 0 ; uiSector < 256 ; ++uiSector )
	for( sLMX = MINIMUM_VALID_X_COORDINATE; sLMX <= MAXIMUM_VALID_X_COORDINATE ; ++sLMX )
		for( sLMY = MINIMUM_VALID_Y_COORDINATE; sLMY <= MAXIMUM_VALID_Y_COORDINATE ; ++sLMY )
	{
//		SECTORINFO *pSectorInfo = &( SectorInfo[ uiSector ] );
		UINT8 uiSumOfEnemyTroops = NumEnemiesInSector( sLMX, sLMY );
			//pSectorInfo->ubNumAdmins + pSectorInfo->ubNumTroops + pSectorInfo->ubNumElites;

		// there's an enemy
		if( uiSumOfEnemyTroops )
		{
			DOUBLE ddAngle;

			if( sLMX - sMapX == 0 )ddAngle = ( sLMY - sMapY > 0 ) ? 1.57079632 : -1.57079632;
				else ddAngle = atan2( (double)(sLMY - sMapY), (double)(sLMX - sMapX) );

			if( ddAngle < 0 && uiDir == DIR_WEST )ddAngle += pix2;

			// is it in a right direction?
			if( ddAngle >= ddMinAngle && ddAngle <= ddMaxAngle )
			{
				DOUBLE ddDistance = sqrt( pow( (double)(sLMY - sMapY), 2 ) + pow( (double)(sLMX - sMapX), 2 ) );

				ddRes += (DOUBLE)uiSumOfEnemyTroops / pow( ddDistance, 2 );

			}
		}
	}

	return (UINT16) ddRes;
}

UINT16 CountDirectionRating( INT16 sMapX, INT16 sMapY, UINT8 uiDir )
{
	INT32 iRes = BASE_DIR_PRIORITY + CountDirectionEnemyRating( sMapX, sMapY, uiDir );

	INT16 sDMapX, sDMapY;
	INT32 iDiff;

	switch(uiDir)
	{
	case DIR_NORTH:
		sDMapX = sMapX;
		sDMapY = sMapY - 1;
		break;
	case DIR_EAST:
		sDMapX = sMapX + 1;
		sDMapY = sMapY;
		break;
	case DIR_SOUTH:
		sDMapX = sMapX;
		sDMapY = sMapY + 1;
		break;
	case DIR_WEST:
		sDMapX = sMapX - 1;
		sDMapY = sMapY;
		break;
	default:
		return iRes;
	}

	if( CountAllMilitiaInSector( sDMapX, sDMapY ) &&
		(UINT32)( CountAllMilitiaInSector( sDMapX, sDMapY ) + CountAllMilitiaInSector( sMapX, sMapY ) ) <= gGameExternalOptions.guiMaxMilitiaSquadSize )
		iRes += DIR_WITH_UNFULL_SQUAD_RATING_BONUS;

	if( NumEnemiesInSector( sDMapX, sDMapY	) )
	{
	//	if( GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR )
			iDiff = (INT32)( (FLOAT)iRes * ( (FLOAT)CountAllMilitiaInFiveSectors( sDMapX, sDMapY ) / (FLOAT)NumEnemiesInFiveSectors( sDMapX, sDMapY ) ) );
	//	else
	//		iDiff = iRes * ( (FLOAT)CountAllMilitiaInFiveSectors( sMapX, sMapY ) / DIV_OF_ORIGINAL_MILITIA / (FLOAT)NumEnemiesInFiveSectors( sDMapX, sDMapY ) );

		if( iDiff > (INT32)( (FLOAT)DIR_MIN_DIF * (FLOAT)iRes ) )
			iRes = iDiff;
		else
			iRes = 0;
	}


// There's player's mercs! Go there
	if( PlayerMercsInSector_MSE( (UINT8)sDMapX, (UINT8)sDMapY, FALSE ) )
		iRes += 15000;// should be enough

//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"M %ld, E%ld,%ld %ld, Diff %ld", CountAllMilitiaInSector( sMapX, sMapY ), sDMapX, sDMapY, NumEnemiesInSector( sDMapX, sDMapY ), ((INT32)CountAllMilitiaInSector( sMapX, sMapY )) - ((INT32)NumEnemiesInSector( sDMapX, sDMapY )) );

	return iRes;
}


// Standard conditions the direction should fit
BOOLEAN CheckStandardConditionsForDirection( INT16 sSMapX, INT16 sSMapY, INT16 sMapX, INT16 sMapY, BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors )
{
	UINT8 uiTravab = GetTraversability( SECTOR( sSMapX , sSMapY ) , SECTOR( sMapX, sMapY ) );
	BOOLEAN fTargetCityAllowsRoaming;

 	if( uiTravab == GROUNDBARRIER || uiTravab == EDGEOFWORLD ) return FALSE;

	if( !fForBattle && gfMSBattle && NumEnemiesInSector( sMapX, sMapY ) ) return FALSE;

	// HEADROCK HAM B2.7 : This allows roaming militia to move into Minor City sectors, if liberated at least
	// once before
	if (GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR)
	{
		fTargetCityAllowsRoaming = TRUE;
	}
	else
	{
		if (gGameExternalOptions.fAllowMilitiaMoveThroughMinorCities)
		{
			if ( SectorInfo[SECTOR( sMapX, sMapY )].fSurfaceWasEverPlayerControlled )
			{
				fTargetCityAllowsRoaming = ( !gfMilitiaAllowedInTown[GetTownIdForSector( sMapX, sMapY )] );
			}
			else
			{			
				fTargetCityAllowsRoaming = FALSE;
			}
		}
		else
		{			
			fTargetCityAllowsRoaming = FALSE;
		}
	}

	//return (GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR || fWithCities) && !IsThisSectorASAMSector(	sMapX, sMapY , 0 ) &&
	//	(GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR || !fOnlyCitySectors);// &&
	return (fTargetCityAllowsRoaming || fWithCities) && !IsThisSectorASAMSector(	sMapX, sMapY , 0 ) &&
		(GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR || !fOnlyCitySectors);// &&
//		( !NumEnemiesInSector( sMapX, sMapY ) || GetEnemyGroupIdInSector(sMapX, sMapY ) || fForBattle ) &&
//		( fForBattle || CountAllMilitiaInSector( sMapX, sMapY ) < gGameExternalOptions.guiMaxMilitiaSquadSize || PlayerMercsInSector( sMapX, sMapY, 0 ) )
}

void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors )
{
	*uiDirNumber = 0;

#ifdef DEBUG_SHOW_RATINGS
	if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld-------------------", sMapX, sMapY );
#endif

	if(sMapY > MINIMUM_VALID_Y_COORDINATE && CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY - 1, fWithCities, fForBattle, fOnlyCitySectors ) )
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY - 1 );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_NORTH );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_NORTH;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	if(sMapX > MINIMUM_VALID_X_COORDINATE && CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX - 1, sMapY, fWithCities, fForBattle, fOnlyCitySectors ))
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX - 1, sMapY );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_WEST );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_WEST;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of west direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	if(sMapY < MAXIMUM_VALID_Y_COORDINATE && CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX, sMapY + 1, fWithCities, fForBattle, fOnlyCitySectors ))
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY + 1 );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_SOUTH );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_SOUTH;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of south direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	if(sMapX < MAXIMUM_VALID_X_COORDINATE && CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX + 1, sMapY, fWithCities, fForBattle, fOnlyCitySectors ))
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX + 1, sMapY );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_EAST );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_EAST;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of east direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}

}

UINT8 pBlockMoveList[256];
UINT8 uiBlockMoveListPointer;

void AddToBlockMoveList(INT16 sMapX, INT16 sMapY)
{
	pBlockMoveList[ uiBlockMoveListPointer++ ] = SECTOR( sMapX, sMapY );
}

void ClearBlockMoveList()
{
	uiBlockMoveListPointer = 0;
}

BOOLEAN CheckInBlockMoveList(INT16 sMapX, INT16 sMapY)
{
	UINT16 i;
	for( i = 0; i < uiBlockMoveListPointer ; ++i)
		if( sMapX == SECTORX(pBlockMoveList[i]) && sMapY == SECTORY(pBlockMoveList[i]) )return TRUE;

	return FALSE;
}

void UpdateMilitiaSquads(INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT32 iRandomRes = 0, iRandom = 0;
	UINT8 x;//,y;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT8 uiMilitiaCount;
	BOOLEAN fSourceCityAllowsRoaming;


	// If we don't want roaming militia
	// We shouldn't be here.
	if( !gGameExternalOptions.gfAllowMilitiaGroups )
		return;

	if( sMapX == 1 && sMapY == 1 )
	{
		ClearBlockMoveList();
		gfMSBattle = FALSE;
	}

	if( CheckInBlockMoveList( sMapX, sMapY ) )return;

	uiMilitiaCount = CountMilitia(pSectorInfo);

	if( !uiMilitiaCount )return;

	// Kaiden: Moved Create Militia code into CreateMilitiaSquads
	// Removed the code from here and added the procedure call
	// If it's time, and this is a city sector create a new group.
	// But only if we're not training our own.

	if( (!gGameExternalOptions.gfmusttrainroaming) && ( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ) )
	{
		if (GetWorldDay( ) < gGameExternalOptions.guiAllowMilitiaGroupsDelay)
			return;

		if( GetWorldHour() % gGameExternalOptions.guiCreateEachNHours )
			return;

		CreateMilitiaSquads( sMapX, sMapY );
		if (gfStrategicMilitiaChangesMade)
		{
			ResetMilitia();
		}
	}

	// HEADROCK HAM B2.7: If INI flag is set, allow militia to move out of Minor City sectors (Orta, Tixa, San Mona, etc).
	if (gGameExternalOptions.fAllowMilitiaMoveThroughMinorCities)
		fSourceCityAllowsRoaming = ( !gfMilitiaAllowedInTown[GetTownIdForSector( sMapX, sMapY )] );
	else
		fSourceCityAllowsRoaming = ( GetTownIdForSector( sMapX, sMapY ) == BLANK_SECTOR );

	// moving squad, if it is not a SAM site
	if( ( fSourceCityAllowsRoaming ) && (!IsThisSectorASAMSector(	sMapX, sMapY, 0 )) )
	{
		if( !PlayerMercsInSector_MSE( (UINT8)sMapX, (UINT8)sMapY, FALSE ) ) // and there's no player's mercs in the sector
		{
			if( GetWorldHour() % 2 )return;

			memset(pMoveDir, 0, sizeof(pMoveDir));
			GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, FALSE, FALSE );

			if( uiDirNumber )
			{
				for( x = 1; x < uiDirNumber ; ++x )pMoveDir[x][1] += pMoveDir[x-1][1];
			//			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld", x, pMoveDir[x][1]);

				iRandom = Random( pMoveDir[ uiDirNumber - 1 ][1] +
					( uiMilitiaCount >= gGameExternalOptions.guiMinMilitiaSquadSize ? CHANCE_TO_MOVE_A_SQUAD : CHANCE_TO_MOVE_AN_UNFULL_SQUAD ) );

				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Roll %ld", iRandomRes);

				iRandomRes = 256;

				for( x = 0; x < uiDirNumber; ++x)
					if( iRandom < pMoveDir[x][1] )
					{
						iRandomRes = x;
						break;
					}

					// "Decided" to stay here
					if(iRandomRes >= uiDirNumber)
						return;

					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld:Dir count %ld, Rand %ld. Go to %ld,%ld. Have %ld militia men", sMapX, sMapY, uiDirNumber, iRandomRes, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ), uiMilitiaCount);
					//Kaiden: if Restricted Sectors List option is turned on,
					// militia can't move to any sectors in the list.
					// Unless they are following a group of mercs.
					///////////////////////////////////////////////////////////////////
					// HEADROCK HAM B1: Restrict roamers by XML as normal, but also 
					// allow them to move through visited sectors. 
					// Also, there's an option now to use dynamic restriction. The 
					// AdjustRoamingRestrictions() function is used to dynamically 
					// define which sectors are restricted, based on city conquest.
					///////////////////////////////////////////////////////////////////

					if (gGameExternalOptions.gflimitedRoaming)
					{
						// Is militia allowed to move through visited sectors?
						if (gGameExternalOptions.bUnrestrictVisited)
						{
							// Scan each sector restricted in the XML.
							for (INT32 i=0;i<iRestrictedSectorArraySize; ++i)
							{
								DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RestrictedSectors, %d", gRestrictMilitia[i]));
								// If sector isn't explored yet
								if ( SectorInfo[ pMoveDir[iRandomRes][0] ].fSurfaceWasEverPlayerControlled == 0 )
								{
									// Is militia allowed to move through dynamically defined sectors?
									if (gGameExternalOptions.bDynamicRestrictRoaming)
									{
										// Is destination allowed by Dynamic Array?
										if( !gDynamicRestrictMilitia[pMoveDir[iRandomRes][0]] )
										{
											return;
										}
									}
									// Militia limited to XMLized sectors
									else
									{
										// Is destination allowed by XML?
										if(pMoveDir[iRandomRes][0] == gRestrictMilitia[i] )
										{
											return;
										}
									}
								}
							}
						}
						// Militia must adhere to restrictions, regardless of visited sectors
						else
						{
							for (INT32 i=0;i<iRestrictedSectorArraySize; ++i)
							{
								DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("RestrictedSectors, %d", gRestrictMilitia[i]));
								// Is militia allowed to move through dynamically defined sectors?
								if (gGameExternalOptions.bDynamicRestrictRoaming)
								{
									// Is destination allowed by Dynamic Array?
									if( !gDynamicRestrictMilitia[pMoveDir[iRandomRes][0]] )
									{
										return;
									}
								}
								// Militia limited to XMLized sectors
								else
								{
									// Is destination allowed by XML?
									if(pMoveDir[iRandomRes][0] == gRestrictMilitia[i] )
									{
										return;
									}
								}
							}
						}
					}

					// WDS bug fix for moving militia
					int targetX = SECTORX( pMoveDir[ iRandomRes ][0] );
					int targetY = SECTORY( pMoveDir[ iRandomRes ][0] );
					Assert(targetX >= 0 && targetX < MAP_WORLD_X);
					Assert(targetY >= 0 && targetY < MAP_WORLD_Y);
					MoveMilitiaSquad( sMapX, sMapY,  targetX, targetY, FALSE );
					AddToBlockMoveList( targetX, targetY );

					if ( gfStrategicMilitiaChangesMade)
					{
						ResetMilitia();
					}

					if( NumEnemiesInSector( targetX, targetY ) )
					{
						extern GROUP *gpBattleGroup;
						gpBattleGroup = GetGroup( GetEnemyGroupIdInSector( targetX, targetY ) );
		/*				GROUP* pEnemyGroup = GetGroup( GetEnemyGroupIdInSector( targetX, targetY ) );

						if(pEnemyGroup && pEnemyGroup->ubGroupID)
						{
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Attacking from %ld,%ld to %ld,%ld - enemy's group id %ld", sMapX, sMapY, targetX, SECTORY( pMoveDir[ iRandomRes ][0], pEnemyGroup->ubGroupID ));
							//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Arrival 1, Arrival 2");

							pEnemyGroup->ubPrevX = sMapX;
							pEnemyGroup->ubPrevY = sMapY;

							pEnemyGroup->ubNextX = targetX;
							pEnemyGroup->ubNextY = targetY;
		*/
							gfMSBattle = TRUE;

			//				GroupArrivedAtSector( pEnemyGroup->ubGroupID , TRUE, FALSE );
							EnterAutoResolveMode( targetX,  targetY );
		//				}
					}
			}
					else return;
		}
	}
}

	// Kaiden: Roaming Militia Training:
	// If we're training roaming militia,
	// we'll get our squad from here:
	// Don't need to check for delay, as this function won't be
	// called if there is a delay set.
void CreateMilitiaSquads(INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT32 iRandomRes = 0;
	UINT8 x;//,y;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	UINT8 uiMilitiaCount;
	// HEADROCK HAM B2.7: New values:
	UINT16 iCounter;
	BOOLEAN fFoundValidSector;
	UINT16 bTownId;
	INT16 sCurrentX, sCurrentY;


	// If we're not allowing roaming groups,
	// then we're not creating them either.
	// Remove this if you want to create them
	// for some other reason. But you also have to
	// remove the same line in UpdateMilitiaSquads
	// Or the ones you create won't move.
	// This function is only called from UpdateMilitiaSquads
	// and TownMilitiaTrainingCompleted
	if( !gGameExternalOptions.gfAllowMilitiaGroups )
		return;

	if( sMapX == 1 && sMapY == 1 )
	{
		ClearBlockMoveList();
		gfMSBattle = FALSE;
	}

	if( CheckInBlockMoveList( sMapX, sMapY ) )return;

	uiMilitiaCount = CountMilitia(pSectorInfo);

	if( !uiMilitiaCount )return;

	// Create new Militia Squad

	bTownId = GetTownIdForSector( sMapX, sMapY );

	if( bTownId != BLANK_SECTOR )
	{
		fFoundValidSector = FALSE;

		// Check all sectors adjacent to the training sector itself.
		GenerateDirectionInfosWithCapacityCheck( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, FALSE, FALSE );

		if(uiDirNumber)// && Random(100) < CHANCE_TO_GENERATE_A_SQUAD)
		{
			for( x = 1; x < uiDirNumber ; ++x )pMoveDir[x][1] += pMoveDir[x-1][1];

			iRandomRes = Random( pMoveDir[ uiDirNumber - 1 ][1] );

			for( x = 0; x < uiDirNumber; ++x)
				if( iRandomRes < pMoveDir[x][1] )
				{
					iRandomRes = x;
					break;
				}

			// shouldn't be!
			if(iRandomRes >= uiDirNumber)iRandomRes = 0;

			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld:Dir count %ld, Rand %ld. Go to %ld,%ld", sMapX, sMapY, uiDirNumber, iRandomRes, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ));

			GenerateMilitiaSquad( sMapX, sMapY,	SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) );
			fFoundValidSector = TRUE;
			AddToBlockMoveList( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) );

			if( NumEnemiesInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) && CountAllMilitiaInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) )
			{
/*				GROUP* pEnemyGroup = GetGroup( GetEnemyGroupIdInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) );

				if(pEnemyGroup && pEnemyGroup->ubGroupID)
				{
					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Attacking from %ld,%ld to %ld,%ld - enemy's group id %ld", sMapX, sMapY, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0], pEnemyGroup->ubGroupID ));
					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Arrival 1, Arrival 2");

					pEnemyGroup->ubPrevX = sMapX;
					pEnemyGroup->ubPrevY = sMapY;

					pEnemyGroup->ubNextX = SECTORX( pMoveDir[ iRandomRes ][0] );
					pEnemyGroup->ubNextY = SECTORY( pMoveDir[ iRandomRes ][0] );
*/
					gfMSBattle = TRUE;
//					GroupArrivedAtSector( pEnemyGroup->ubGroupID , TRUE, FALSE );

					EnterAutoResolveMode( SECTORX( pMoveDir[ iRandomRes ][0] ),	SECTORY( pMoveDir[ iRandomRes ][0] ) );
//				}
			}

		}
		// HEADROCK HAM B2.7: Smarter check - looks at each city sector, and checks if there's room around that
		// sector to place militia. Also relies on whether the sector has full militia before generating more.
		// This section only runs if the program has failed to find suitable sectors around the "home" sector
		// (where militia is being trained).

		iCounter = 0;
		if (gGameExternalOptions.fSmartRoamingMilitiaGenerator)
		{
			while( pTownNamesList[ iCounter ] != 0 && fFoundValidSector == FALSE )
			{
				if( pTownNamesList[ iCounter] == bTownId )
				{
					sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
					sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

					// if sector has enemies or hasn't already been taken at least once, then
					if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || NumEnemiesInSector( sCurrentX, sCurrentY ) > 0 )
					{
						// skip the rest. Generate no militia from this sector. 
						iCounter++;
						continue;
					}

					GenerateDirectionInfosWithCapacityCheck( sCurrentX, sCurrentY, &uiDirNumber, pMoveDir, FALSE, TRUE, FALSE );

					if(uiDirNumber)// && Random(100) < CHANCE_TO_GENERATE_A_SQUAD)
					{
						for( x = 1; x < uiDirNumber ; ++x )pMoveDir[x][1] += pMoveDir[x-1][1];

						iRandomRes = Random( pMoveDir[ uiDirNumber - 1 ][1] );

						for( x = 0; x < uiDirNumber; ++x)
							if( iRandomRes < pMoveDir[x][1] )
							{
								iRandomRes = x;
								break;
							}

						// shouldn't be!
						if(iRandomRes >= uiDirNumber)iRandomRes = 0;

						//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld:Dir count %ld, Rand %ld. Go to %ld,%ld", sMapX, sMapY, uiDirNumber, iRandomRes, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ));

						GenerateMilitiaSquad( sCurrentX, sCurrentY,	SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) );
						fFoundValidSector = TRUE;
						AddToBlockMoveList( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) );

						if( NumEnemiesInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) && CountAllMilitiaInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) )
						{
			/*				GROUP* pEnemyGroup = GetGroup( GetEnemyGroupIdInSector( SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0] ) ) );

							if(pEnemyGroup && pEnemyGroup->ubGroupID)
							{
								//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Attacking from %ld,%ld to %ld,%ld - enemy's group id %ld", sMapX, sMapY, SECTORX( pMoveDir[ iRandomRes ][0] ), SECTORY( pMoveDir[ iRandomRes ][0], pEnemyGroup->ubGroupID ));
								//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Arrival 1, Arrival 2");

								pEnemyGroup->ubPrevX = sMapX;
								pEnemyGroup->ubPrevY = sMapY;

								pEnemyGroup->ubNextX = SECTORX( pMoveDir[ iRandomRes ][0] );
								pEnemyGroup->ubNextY = SECTORY( pMoveDir[ iRandomRes ][0] );
			*/
								gfMSBattle = TRUE;
			//					GroupArrivedAtSector( pEnemyGroup->ubGroupID , TRUE, FALSE );

								EnterAutoResolveMode( SECTORX( pMoveDir[ iRandomRes ][0] ),	SECTORY( pMoveDir[ iRandomRes ][0] ) );
			//				}
						}
					}

				}
				iCounter++;
			}
		}
	}
	return;
}


extern BOOLEAN gfMSResetMilitia;

void DoMilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT8 x;
	UINT8 uiNumGreen = 0, uiNumReg = 0, uiNumElite = 0;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	BOOLEAN fMoreTroopsLeft[4] = {FALSE,FALSE,FALSE,FALSE};
	BOOLEAN fFirstLoop = TRUE;
	BOOLEAN fMilitiaMoved = FALSE;

	guiDirNumber = 0;

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir,
		( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ? TRUE : FALSE ), TRUE, FALSE );

	ZeroMemory( gpAttackDirs, sizeof( gpAttackDirs ) );

	gpAttackDirs[ guiDirNumber ][0] = uiNumGreen = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
	gpAttackDirs[ guiDirNumber ][1] = uiNumReg = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
	gpAttackDirs[ guiDirNumber ][2] = uiNumElite = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
	gpAttackDirs[ guiDirNumber ][3] = INSERTION_CODE_CENTER;

	guiDirNumber = uiDirNumber	+ 1;

	x = 0;
	while( CountMilitia(pSectorInfo ) < gGameExternalOptions.guiMaxMilitiaSquadSizeBattle &&
		( fMoreTroopsLeft[0] || fMoreTroopsLeft[1] || fMoreTroopsLeft[2] || fMoreTroopsLeft[3] || fFirstLoop ) )
	{
		fMoreTroopsLeft[ x ] = MoveOneBestMilitiaMan( SECTORX( pMoveDir[ x ][0] ), SECTORY( pMoveDir[ x ][0] ), sMapX, sMapY );

		if( fMoreTroopsLeft[ x ] )
		{
			fMilitiaMoved = TRUE;

			gpAttackDirs[ x + 1 ][0] += pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA] - uiNumGreen;
			gpAttackDirs[ x + 1 ][1] += pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA] - uiNumReg;
			gpAttackDirs[ x + 1 ][2] += pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] - uiNumElite;
			gpAttackDirs[ x + 1 ][3] = (UINT8)pMoveDir[ x ][2];

			uiNumGreen = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
			uiNumReg = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
			uiNumElite = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
		}

		++x;
		if( x >= uiDirNumber )
		{
			x = 0;
			fFirstLoop = FALSE;
		}
	}

	if (is_networked)
	{
		if (gfStrategicMilitiaChangesMade)
		{
			RemoveMilitiaFromTactical();
			if(is_server && MILITIA_ENABLED==1)
				PrepareMilitiaForTactical(FALSE);
		}
	}
	
	gfStrategicMilitiaChangesMade = FALSE;
}

void MSCallBack( UINT8 ubResult )
{
	if( ubResult == MSG_BOX_RETURN_YES )
	{
		gTacticalStatus.uiFlags |= WANT_MILITIA_REINFORCEMENTS;
		DoMilitiaHelpFromAdjacentSectors( sMSMapX, 	sMSMapY );
	}
}

BOOLEAN IsThereMilitiaInAdjacentSector( INT16 sMapX, INT16 sMapY )
{
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT8 x;
	BOOLEAN fResult = FALSE;

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir,
		( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ? TRUE : FALSE ), TRUE, FALSE );

	for( x = 0; x < uiDirNumber ; ++x )
		if( CountAllMilitiaInSector( SECTORX( pMoveDir[ x ][ 0 ] ), SECTORY( pMoveDir[ x ][ 0 ] ) ) )
			fResult = TRUE;

	return fResult;
}

void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY )
{
	sMSMapX = sMapX;
	sMSMapY = sMapY;

	if( !gGameExternalOptions.gfAllowReinforcements )
		return;

	gTacticalStatus.uiFlags &= (~WANT_MILITIA_REINFORCEMENTS);

	guiDirNumber = 0;

//	if( CountAllMilitiaInSector( sMapX, sMapY ) ) MSCallBack( MSG_BOX_RETURN_YES );

	// This is no longer a question of simply whether to have a full militia count, but also whether we want
	// reinforcements.	So if there are any available, always ask.
	if( IsThereMilitiaInAdjacentSector( sMapX, sMapY ) ) // && CountAllMilitiaInSector( sMapX, sMapY ) < gGameExternalOptions.guiMaxMilitiaSquadSizeBattle )
		DoScreenIndependantMessageBox( gzCWStrings[0], MSG_BOX_FLAG_YESNO, MSCallBack );
}

void MilitiaFollowPlayer( INT16 sMapX, INT16 sMapY, INT16 sDMapX, INT16 sDMapY )
{
	if( GetTownIdForSector( sMapX, sMapY ) != BLANK_SECTOR ||
		IsThisSectorASAMSector( sMapX, sMapY, 0 ) ||
		PlayerMercsInSector_MSE( (UINT8)sMapX, (UINT8)sMapY, TRUE ) )return;

	if( GetTownIdForSector( sDMapX, sDMapY ) != BLANK_SECTOR ||
		IsThisSectorASAMSector( sDMapX, sDMapY, 0 ) )return;


	MoveMilitiaSquad( sMapX, sMapY, sDMapX, sDMapY, FALSE );
}
// HEADROCK (HAM): New function to alter Restricted Roaming based on the recent capture of new towns. The area
// around the town will be unrestricted, as well as some road sectors leading away from the town.
// This function runs during sector conquest checks, and only if an entire town has been conquered. It also
// runs at day end, as well as on load/save.
void AdjustRoamingRestrictions()
{
	if (!gGameExternalOptions.bDynamicRestrictRoaming)
		return;

	// to do: Add something to clean up the entire array before setting the flags...

	if ( IsTownUnderCompleteControlByPlayer(DRASSEN) )
	{
		// DRASSEN OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(12,1)] = true;
		gDynamicRestrictMilitia[SECTOR(13,1)] = true;
		gDynamicRestrictMilitia[SECTOR(14,1)] = true;
		gDynamicRestrictMilitia[SECTOR(12,2)] = true;
		gDynamicRestrictMilitia[SECTOR(14,2)] = true;
		gDynamicRestrictMilitia[SECTOR(12,3)] = true;
		gDynamicRestrictMilitia[SECTOR(14,3)] = true;
		gDynamicRestrictMilitia[SECTOR(12,4)] = true;
		gDynamicRestrictMilitia[SECTOR(14,4)] = true;
		gDynamicRestrictMilitia[SECTOR(12,5)] = true;
		gDynamicRestrictMilitia[SECTOR(13,5)] = true;
		gDynamicRestrictMilitia[SECTOR(14,5)] = true;

		// SAM SITE DEFENSE
		gDynamicRestrictMilitia[SECTOR(15,5)] = true;

		// ROAD WEST TO OMERTA
		gDynamicRestrictMilitia[SECTOR(9,2)] = true;
		gDynamicRestrictMilitia[SECTOR(10,2)] = true;
		gDynamicRestrictMilitia[SECTOR(11,2)] = true;

		// ROAD SOUTH TO ALMA
		gDynamicRestrictMilitia[SECTOR(12,6)] = true;
	
	}

	if ( IsTownUnderCompleteControlByPlayer(ALMA) )
	{
		// ALMA OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(12,7)] = true;
		gDynamicRestrictMilitia[SECTOR(13,7)] = true;
		gDynamicRestrictMilitia[SECTOR(14,7)] = true;
		gDynamicRestrictMilitia[SECTOR(15,7)] = true;
		gDynamicRestrictMilitia[SECTOR(12,8)] = true;
		gDynamicRestrictMilitia[SECTOR(15,8)] = true;
		gDynamicRestrictMilitia[SECTOR(12,9)] = true;
		gDynamicRestrictMilitia[SECTOR(15,9)] = true;
		gDynamicRestrictMilitia[SECTOR(12,10)] = true;
		gDynamicRestrictMilitia[SECTOR(13,10)] = true;
		gDynamicRestrictMilitia[SECTOR(14,10)] = true;
		gDynamicRestrictMilitia[SECTOR(15,10)] = true;

		// ROAD SOUTH
		gDynamicRestrictMilitia[SECTOR(14,11)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(CHITZENA) )
	{
		// CHITZENA OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(1,1)] = true;
		gDynamicRestrictMilitia[SECTOR(3,1)] = true;
		gDynamicRestrictMilitia[SECTOR(1,2)] = true;
		gDynamicRestrictMilitia[SECTOR(3,2)] = true;
		gDynamicRestrictMilitia[SECTOR(1,3)] = true;
		gDynamicRestrictMilitia[SECTOR(2,3)] = true;
		gDynamicRestrictMilitia[SECTOR(3,3)] = true;

		// SAM SITE DEFENSE / ROAD SOUTH
		gDynamicRestrictMilitia[SECTOR(3,4)] = true;
		gDynamicRestrictMilitia[SECTOR(3,5)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(CAMBRIA) )
	{
		// CAMBRIA OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(7,5)] = true;
		gDynamicRestrictMilitia[SECTOR(8,5)] = true;
		gDynamicRestrictMilitia[SECTOR(9,5)] = true;
		gDynamicRestrictMilitia[SECTOR(10,5)] = true;
		gDynamicRestrictMilitia[SECTOR(7,6)] = true;
		gDynamicRestrictMilitia[SECTOR(10,6)] = true;
		gDynamicRestrictMilitia[SECTOR(7,7)] = true;
		gDynamicRestrictMilitia[SECTOR(10,7)] = true;
		gDynamicRestrictMilitia[SECTOR(7,8)] = true;
		gDynamicRestrictMilitia[SECTOR(9,8)] = true;
		gDynamicRestrictMilitia[SECTOR(10,8)] = true;
		
		// SAM SITE DEFENSE
		gDynamicRestrictMilitia[SECTOR(7,9)] = true;
		gDynamicRestrictMilitia[SECTOR(9,9)] = true;

		// ROAD TO OMERTA
		gDynamicRestrictMilitia[SECTOR(9,2)] = true;
		gDynamicRestrictMilitia[SECTOR(9,3)] = true;
		gDynamicRestrictMilitia[SECTOR(9,4)] = true;	

		// CROSSROADS WEST
		gDynamicRestrictMilitia[SECTOR(6,7)] = true;

		// ROAD TO ALMA
		gDynamicRestrictMilitia[SECTOR(11,7)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(BALIME) )
	{
		// BALIME OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(10,11)] = true;
		gDynamicRestrictMilitia[SECTOR(11,11)] = true;
		gDynamicRestrictMilitia[SECTOR(12,11)] = true;
		gDynamicRestrictMilitia[SECTOR(13,11)] = true;
		gDynamicRestrictMilitia[SECTOR(10,12)] = true;
		gDynamicRestrictMilitia[SECTOR(13,12)] = true;
		gDynamicRestrictMilitia[SECTOR(10,13)] = true;
		gDynamicRestrictMilitia[SECTOR(13,13)] = true;

		// WEST ROAD
		gDynamicRestrictMilitia[SECTOR(6,11)] = true;
		gDynamicRestrictMilitia[SECTOR(7,11)] = true;
		gDynamicRestrictMilitia[SECTOR(8,11)] = true;
		gDynamicRestrictMilitia[SECTOR(9,11)] = true;

		// SOUTH ROAD
		gDynamicRestrictMilitia[SECTOR(8,14)] = true;
		gDynamicRestrictMilitia[SECTOR(9,14)] = true;
		gDynamicRestrictMilitia[SECTOR(10,14)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(GRUMM) )
	{
		// GRUMM OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(1,6)] = true;
		gDynamicRestrictMilitia[SECTOR(2,6)] = true;
		gDynamicRestrictMilitia[SECTOR(3,6)] = true;
		gDynamicRestrictMilitia[SECTOR(3,7)] = true;
		gDynamicRestrictMilitia[SECTOR(4,7)] = true;
		gDynamicRestrictMilitia[SECTOR(4,8)] = true;
		gDynamicRestrictMilitia[SECTOR(3,9)] = true;
		gDynamicRestrictMilitia[SECTOR(4,9)] = true;

		// ROAD NORTH
		gDynamicRestrictMilitia[SECTOR(3,5)] = true;
		gDynamicRestrictMilitia[SECTOR(3,4)] = true;

		// ROAD EAST
		gDynamicRestrictMilitia[SECTOR(5,7)] = true;
		gDynamicRestrictMilitia[SECTOR(6,7)] = true;

		// ROAD SOUTH
		gDynamicRestrictMilitia[SECTOR(2,10)] = true;
		gDynamicRestrictMilitia[SECTOR(3,10)] = true;
		gDynamicRestrictMilitia[SECTOR(2,11)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(DRASSEN) && IsTownUnderCompleteControlByPlayer(CHITZENA) )
	{
		// ROAD EAST OF SAN-MONA
		gDynamicRestrictMilitia[SECTOR(7,3)] = true;
		gDynamicRestrictMilitia[SECTOR(8,3)] = true;
		gDynamicRestrictMilitia[SECTOR(9,3)] = true;
	
		// SAN MONA
		gDynamicRestrictMilitia[SECTOR(5,3)] = true;
		gDynamicRestrictMilitia[SECTOR(6,3)] = true;
		gDynamicRestrictMilitia[SECTOR(4,4)] = true;
		gDynamicRestrictMilitia[SECTOR(5,4)] = true;
	}	

	if ( IsTownUnderCompleteControlByPlayer(CAMBRIA) && ( IsTownUnderCompleteControlByPlayer(CHITZENA) || IsTownUnderCompleteControlByPlayer(DRASSEN) ) )
	{
		// ROAD TO SAN MONA
		gDynamicRestrictMilitia[SECTOR(7,3)] = true;
		gDynamicRestrictMilitia[SECTOR(8,3)] = true;
		gDynamicRestrictMilitia[SECTOR(9,3)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(CAMBRIA) && ( IsTownUnderCompleteControlByPlayer(DRASSEN) || IsTownUnderCompleteControlByPlayer(CHITZENA) || IsTownUnderCompleteControlByPlayer(GRUMM) ) )
	{
		// ROAD SOUTH-EAST OF SAN MONA
		gDynamicRestrictMilitia[SECTOR(6,4)] = true;
		gDynamicRestrictMilitia[SECTOR(7,4)] = true;

		// SAN MONA
		gDynamicRestrictMilitia[SECTOR(5,3)] = true;
		gDynamicRestrictMilitia[SECTOR(6,3)] = true;
		gDynamicRestrictMilitia[SECTOR(4,4)] = true;
		gDynamicRestrictMilitia[SECTOR(5,4)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(ALMA) && ( IsTownUnderCompleteControlByPlayer(DRASSEN) || IsTownUnderCompleteControlByPlayer(CAMBRIA) ) )		
	{
		// ROAD WEST OF ALMA
		gDynamicRestrictMilitia[SECTOR(11,7)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(CHITZENA) && ( IsTownUnderCompleteControlByPlayer(DRASSEN) || IsTownUnderCompleteControlByPlayer(CAMBRIA) || IsTownUnderCompleteControlByPlayer(GRUMM) ) )		
	{
		// SAN MONA
		gDynamicRestrictMilitia[SECTOR(5,3)] = true;
		gDynamicRestrictMilitia[SECTOR(6,3)] = true;
		gDynamicRestrictMilitia[SECTOR(4,4)] = true;
		gDynamicRestrictMilitia[SECTOR(5,4)] = true;

		// ROAD TO SAN MONA
		gDynamicRestrictMilitia[SECTOR(7,3)] = true;
		gDynamicRestrictMilitia[SECTOR(8,3)] = true;
		gDynamicRestrictMilitia[SECTOR(9,3)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(CAMBRIA) && IsTownUnderCompleteControlByPlayer(ALMA) )
	{
		// ROAD EAST OF CAMBRIA
		gDynamicRestrictMilitia[SECTOR(11,7)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(CAMBRIA) && ( IsTownUnderCompleteControlByPlayer(BALIME) || IsTownUnderCompleteControlByPlayer(GRUMM) || IsTownUnderCompleteControlByPlayer(ALMA) ) )
	{
		// UPGRADED SAM-SITE DEFENSE FOR CAMBRIA
		gDynamicRestrictMilitia[SECTOR(7,10)] = true;
		gDynamicRestrictMilitia[SECTOR(8,10)] = true;
		gDynamicRestrictMilitia[SECTOR(9,10)] = true; // Tixa. Won't be accessible unless Minor Cities are allowed.
	}

	if ( IsTownUnderCompleteControlByPlayer(CAMBRIA) && IsTownUnderCompleteControlByPlayer(GRUMM) )
	{
		// ESTONI
		gDynamicRestrictMilitia[SECTOR(6,8)] = true;

		// ESTONI MOUNTAIN GATE
		gDynamicRestrictMilitia[SECTOR(6,9)] = true;

		// SOUTHEAST ROAD TO SAN MONA
		gDynamicRestrictMilitia[SECTOR(6,4)] = true;
		gDynamicRestrictMilitia[SECTOR(7,4)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(BALIME) && IsTownUnderCompleteControlByPlayer(GRUMM) && IsTownUnderCompleteControlByPlayer(CAMBRIA) )
	{
		// GRUMM-CAMBRIA-BALIME ROAD
		gDynamicRestrictMilitia[SECTOR(6,10)] = true;
	}

	// CONTROL ALL TOWNS?
	if ( IsTownUnderCompleteControlByPlayer(BALIME) && IsTownUnderCompleteControlByPlayer(CAMBRIA) && IsTownUnderCompleteControlByPlayer(CHITZENA) && IsTownUnderCompleteControlByPlayer(DRASSEN) && IsTownUnderCompleteControlByPlayer(ALMA) && IsTownUnderCompleteControlByPlayer(GRUMM) )
	{
		// ROADS TO MEDUNA
		gDynamicRestrictMilitia[SECTOR(6,12)] = true;	
		gDynamicRestrictMilitia[SECTOR(7,14)] = true;
		gDynamicRestrictMilitia[SECTOR(2,12)] = true;	

		// MEDUNA OUTSKIRTS
		gDynamicRestrictMilitia[SECTOR(2,13)] = true;	
		gDynamicRestrictMilitia[SECTOR(3,13)] = true;	
		gDynamicRestrictMilitia[SECTOR(4,13)] = true;	
		gDynamicRestrictMilitia[SECTOR(5,13)] = true;	
		gDynamicRestrictMilitia[SECTOR(6,13)] = true;
		gDynamicRestrictMilitia[SECTOR(6,14)] = true;
	}
}

// HEADROCK HAM B2.7: This is a copy of an existing function that generates possible movement directions for militia.
// Due to the widespread use of that function, I've decided to make a new one that calculates directions
// when generating militia. This function checks for available space at the target location. It won't count green
// or regular militia in this total, as they will be replaced by higher grade militia when added.

void GenerateDirectionInfosWithCapacityCheck( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fWithCities, BOOLEAN fForBattle, BOOLEAN fOnlyCitySectors )
{
	SECTORINFO *pSectorInfo;

	*uiDirNumber = 0;

#ifdef DEBUG_SHOW_RATINGS
	if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld-------------------", sMapX, sMapY );
#endif

	pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY - 1 ) ] );
	if(CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY - 1, fWithCities, fForBattle, fOnlyCitySectors ) &&
		pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] < gGameExternalOptions.iMaxMilitiaPerSector )
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY - 1 );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_NORTH );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_NORTH;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	pSectorInfo = &( SectorInfo[ SECTOR( sMapX - 1, sMapY ) ] );
	if(CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX - 1, sMapY, fWithCities, fForBattle, fOnlyCitySectors ) &&
		pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] < gGameExternalOptions.iMaxMilitiaPerSector)	
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX - 1, sMapY );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_WEST );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_WEST;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of west direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	pSectorInfo = &( SectorInfo[ SECTOR( sMapX , sMapY + 1 ) ] );
	if(CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX, sMapY + 1, fWithCities, fForBattle, fOnlyCitySectors ) &&
		pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] < gGameExternalOptions.iMaxMilitiaPerSector )	
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY + 1 );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_SOUTH );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_SOUTH;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of south direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}
	pSectorInfo = &( SectorInfo[ SECTOR( sMapX + 1 , sMapY ) ] );
	if(CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX + 1, sMapY, fWithCities, fForBattle, fOnlyCitySectors ) &&
		pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] < gGameExternalOptions.iMaxMilitiaPerSector )	
	{
		pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX + 1, sMapY );

		if( fForBattle )
			pMoveDir[ *uiDirNumber ][1] = 0;
		else
			pMoveDir[ *uiDirNumber ][1] = CountDirectionRating( sMapX, sMapY, DIR_EAST );

		pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_EAST;

#ifdef DEBUG_SHOW_RATINGS
		if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of east direction - %ld", pMoveDir[ *uiDirNumber ][1]);
#endif

		++(*uiDirNumber);
	}

}
