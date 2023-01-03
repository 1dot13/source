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
	#include "Campaign.h"
	#include "message.h"
	#include "mapscreen.h"
	#include "Strategic Pathing.h"
	#include "GameVersion.h"
#endif

#include "connect.h"
#include "MilitiaSquads.h"
#include "Reinforcement.h"
#include "Inventory Choosing.h"		// added by Flugente for MoveOneMilitiaEquipmentSet() and MoveMilitiaEquipment()
#include "MilitiaIndividual.h"		// added by Flugente

// Debug defines

//#define DEBUG_SHOW_RATINGS
#ifdef DEBUG_SHOW_RATINGS
	#define DEBUG_RATINGS_CONDITION TRUE
	//#define DEBUG_RATINGS_CONDITION !fForBattle
#include "message.h"
#endif


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
		if ( pGroup->usGroupTeam == OUR_TEAM )// && !pGroup->fBetweenSectors)
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
UINT8 GetEnemyGroupIdInSector( INT16 sMapX, INT16 sMapY, UINT8 usTeam )
{
	UINT8 ubRes = 0;
	GROUP *curr = gpGroupList;

	while( curr )
	{
		if ( curr->ubSectorX == sMapX && curr->ubSectorY == sMapY && curr->usGroupTeam == usTeam )
		{
			if( !curr->ubGroupID )
				return curr->ubGroupID;
			else
				ubRes = curr->ubGroupID;
		}

		curr = curr->next;
	}

	return ubRes;
}

// Creates militia at destination sector and removes it from starting sector
void MoveMilitiaSquad(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY, BOOLEAN fAlternativeMax, BOOLEAN fSkipSpreading )
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	SECTORINFO *pTSectorInfo = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	UINT8 bGreensSourceTeam = 0, bGreensDestTeam = 0;
	UINT8 bRegularsSourceTeam = 0, bRegularsDestTeam = 0;
	UINT8 bElitesSourceTeam = 0, bElitesDestTeam = 0;
	UINT8 bTotalGreens = 0, bTotalRegulars = 0, bTotalElites = 0;
	UINT8 bTotalGreensPercent = 0, bTotalRegularsPercent = 0, bTotalElitesPercent = 0;
	INT8 bNewSourceGroupSize = 0, bNewDestGroupSize = 0, bGroupSizeRatio = 0;
	UINT8 ubChanceToSpreadOut;
		
	//////////////////////////////////////////////////////////
	// We begin by determining the group's chance to "spread out" into the next sector rather
	// than moving in en-masse. This is mainly affected by the presence of enemies.

	// Is the target a city or SAM site?
	if (fSkipSpreading || IsThisSectorASAMSector( sTMapX, sTMapY, 0 ) ||
		( GetTownIdForSector( sTMapX, sTMapY ) != BLANK_SECTOR &&
		  gfMilitiaAllowedInTown[GetTownIdForSector( sTMapX, sTMapY )] ))
	{
		// Mobile->Town movement is always en-masse. We do not want to reduce the number
		// of Garrison troops in the town! They can only be increased.
		ubChanceToSpreadOut = 0;
	}
	else
	{
		// Wilderness->Wilderness movement. Calculate Spread Out chance based on threats.

		UINT8 ubNumEnemiesNearOrigin = NumEnemiesInFiveSectors( sMapX, sMapY );
		UINT8 ubNumEnemiesNearTarget = NumEnemiesInFiveSectors( sTMapX, sTMapY );

		if (ubNumEnemiesNearOrigin == 0 &&
			ubNumEnemiesNearTarget > 0 )
		{
			// Target sector is threatened, while source sector is not.
			// Target sector gets all the militia it can, in preperation for a possible enemy attack.
			ubChanceToSpreadOut = 0;
		}
		else if (ubNumEnemiesNearOrigin > 0 &&
				ubNumEnemiesNearTarget == 0 )
		{
			// Source sector is threatened, while destination is not. The group does not
			// move at all, as it is already in a good position to defend.
			return;
		}

		else if (ubNumEnemiesNearOrigin > 0 &&
				ubNumEnemiesNearTarget > 0 )
		{
			// Both sectors threatened. 
			//Moa: removed below: reinforcement is handled elsewhere, the function below is even better then 50%/50% because it leaves
			//a chance to make one sector fully defended instead of possibly two losses. Talking about unfull groups - full groups wont change anyway.
			/*if (gGameExternalOptions.gfAllowReinforcements && !gGameExternalOptions.gfAllowReinforcementsOnlyInCity)
			{
				// Militia will spread out between the two sectors to maximize defensive capability, since they can 
				// always reinforce one another if either sector is attacked.
				ubChanceToSpreadOut = 100;
			}
			else*/
			{
				// Militia chance to spread out is based on whether the Origin or the Destination are under greater
				// threat. If the destination is more threatened, militia are more likely to reinforce it 
				// (move en-masse). If the origin is more threatened, militia are more likely to spread out evenly
				// between the two sectors, to give at least some chance of defeating ONE of the enemy groups.
				UINT16 usTotalNumEnemies = ubNumEnemiesNearTarget + ubNumEnemiesNearOrigin;
				ubChanceToSpreadOut = __min( 100, (ubNumEnemiesNearOrigin * 100) / usTotalNumEnemies );
			}
		}

		else
		{
			///////////////////////////////////////////////////////
			// Both sectors not threatened. 
			// Alter spreadout chance based on number of troops in Source and destination sectors.

			if ( NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM ) >= gGameExternalOptions.iMaxMilitiaPerSector &&
				 NumNonPlayerTeamMembersInSector( sTMapX, sTMapY, MILITIA_TEAM ) == 0 )
			{
				// Source group is full, target sector is empty. The source group gets 30% chance to 
				// spread out to that empty sector.
				ubChanceToSpreadOut = 30;
			}
			else if ( NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM ) && NumNonPlayerTeamMembersInSector( sTMapX, sTMapY, MILITIA_TEAM ) )
			{
				// Source and destination groups both exist. 50% chance to spread out evenly between them.
				ubChanceToSpreadOut = 50;
			}
			else
			{
				// Target sector is empty, source group is not full. Do not spread out.
				ubChanceToSpreadOut = 0;
			}
			
			ubChanceToSpreadOut = 0;
		}
	}

	// Should we spread out into the target sector, or move en-masse?
	if (PreRandom(100) < ubChanceToSpreadOut)
	{
		if ( NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM ) && NumNonPlayerTeamMembersInSector( sTMapX, sTMapY, MILITIA_TEAM ) )
		{
			// Source and Target sectors both have groups in them. Source group will attempt to spread out, 
			// averaging the size and composition of both groups.
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

			// Flugente: mobiles take along their gear
			// move only gear for those who come new into a sector
			UINT8 elites   = max(0, bElitesDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ]);
			UINT8 regulars = max(0, bRegularsDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ]);
			UINT8 greens   = max(0, bGreensDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ]);

			MoveMilitiaEquipment(sMapX, sMapY, sTMapX, sTMapY, elites, regulars, greens);

			// Flugente: update individual militia data
			ApplyTacticalLifeRatioToMilitia( );
			
			// Erase ALL militia from both locations.
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, GREEN_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, REGULAR_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, ELITE_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );

			// Flugente: move along individual militia data
			MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), greens, regulars, elites );

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
			// Target sector is empty. Source group will split up into two equally-sized groups.
			bGreensSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
			bGreensDestTeam = bGreensSourceTeam / 2;

			bRegularsSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
			bRegularsDestTeam = bRegularsSourceTeam / 2;

			bElitesSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
			bElitesDestTeam = bElitesSourceTeam / 2;

			// Flugente: mobiles take along their gear
			// move only gear for those who come new into a sector
			UINT8 elites   = max(0, bElitesDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ]);
			UINT8 regulars = max(0, bRegularsDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ]);
			UINT8 greens   = max(0, bGreensDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ]);

			MoveMilitiaEquipment(sMapX, sMapY, sTMapX, sTMapY, elites, regulars, greens);

			// Flugente: update individual militia data
			ApplyTacticalLifeRatioToMilitia( );
			
			// Remove half team from source sector
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, bGreensDestTeam );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, bRegularsDestTeam );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, bElitesDestTeam );

			// Flugente: move along individual militia data
			MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), greens, regulars, elites );

			// Add half team to target sector
			StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, bGreensDestTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, bRegularsDestTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, bElitesDestTeam );
			
			// Units that move into an empty square do not get to move again.
			AddToBlockMoveList( sTMapX, sTMapY );
		}
	}
	else 
	{
		// Source team moves En-Masse to target sector.

		bNewDestGroupSize = __min( (NumNonPlayerTeamMembersInSector( sTMapX, sTMapY, MILITIA_TEAM ) + NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM )), (UINT8)gGameExternalOptions.iMaxMilitiaPerSector );
		bNewSourceGroupSize = __max( 0, (NumNonPlayerTeamMembersInSector( sTMapX, sTMapY, MILITIA_TEAM ) + NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM )) - gGameExternalOptions.iMaxMilitiaPerSector );

		// If there are still going to be two teams after the transfer
		if ( bNewSourceGroupSize > 0 )
		{
			// Make sure that the two teams end up with the same ratio of Greens/Regulars/Elites
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

			// Correct fractions to full squad size at the target.
			while (bGreensSourceTeam + bRegularsSourceTeam + bElitesSourceTeam > 3 &&
					bGreensDestTeam + bRegularsDestTeam + bElitesDestTeam < bNewDestGroupSize)
			{
				if (bGreensSourceTeam > 0)
				{
					// Move a green to the dest team
					bGreensSourceTeam--;
					bGreensDestTeam++;
					continue;
				}
				if (bRegularsSourceTeam > 0)
				{
					// Move a regular to the dest team
					bRegularsSourceTeam--;
					bRegularsDestTeam++;
					continue;
				}
				if (bElitesSourceTeam > 0)
				{
					// Move an elite to the dest team
					bElitesSourceTeam--;
					bElitesDestTeam++;
					continue;
				}
			}

			// Flugente: mobiles take along their gear
			// move only gear for those who come new into a sector
			UINT8 elites   = max(0, bElitesDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ]);
			UINT8 regulars = max(0, bRegularsDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ]);
			UINT8 greens   = max(0, bGreensDestTeam - pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ]);
			
			MoveMilitiaEquipment(sMapX, sMapY, sTMapX, sTMapY, elites, regulars, greens);

			// Flugente: update individual militia data
			ApplyTacticalLifeRatioToMilitia( );
			
			// Erase ALL militia from both locations.
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, GREEN_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, REGULAR_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sTMapX, sTMapY, ELITE_MILITIA, pTSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );

			// Flugente: move along individual militia data
			MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), greens, regulars, elites );

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
			// Flugente: mobiles take along their gear
			MoveMilitiaEquipment(sMapX, sMapY, sTMapX, sTMapY, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ], pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ], pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ]);

			// Flugente: update individual militia data
			ApplyTacticalLifeRatioToMilitia( );

			UINT8 greens = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
			UINT8 regulars = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
			UINT8 elites = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];

			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, greens );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, regulars );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, elites );

			// Flugente: move along individual militia data
			MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), greens, regulars, elites );

			// Entire group moves from Source to Target, leaving no one behind.			
			StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, greens );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, regulars );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, elites );
			
			// Block target group from moving. It has just assimilated another group.
			AddToBlockMoveList( sTMapX, sTMapY );
		}
	}

	while ( !fAlternativeMax && NumNonPlayerTeamMembersInSector( sTMapX, sTMapY, MILITIA_TEAM ) > gGameExternalOptions.iMaxMilitiaPerSector )
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
	if ( MilitiaInSectorOfRank( sMapX, sMapY, ELITE_MILITIA ) )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, 1 );

		MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), 0, 0, 1 );

		return TRUE;
	}

	if ( MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA ) )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, 1 );

		MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), 0, 1, 0 );

		return TRUE;
	}

	if ( MilitiaInSectorOfRank( sMapX, sMapY, GREEN_MILITIA ) )
	{
		StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, 1 );
		StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, 1 );

		MoveIndividualMilitiaProfiles( SECTOR( sMapX, sMapY ), SECTOR( sTMapX, sTMapY ), 1, 0, 0 );

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
		UINT8 uiSumOfEnemyTroops = NumNonPlayerTeamMembersInSector( sLMX, sLMY, ENEMY_TEAM );
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
	FLOAT iDiff;

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

	UINT8 ubMaxSquadSize = gGameExternalOptions.iMaxMilitiaPerSector;

	UINT8 ubSourceGreens = SectorInfo[SECTOR(sMapX, sMapY)].ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
	UINT8 ubSourceRegulars = SectorInfo[SECTOR(sMapX, sMapY)].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
	UINT8 ubSourceElites = SectorInfo[SECTOR(sMapX, sMapY)].ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
	UINT8 ubTargetGreens = SectorInfo[SECTOR(sDMapX, sDMapY)].ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
	UINT8 ubTargetRegulars = SectorInfo[SECTOR(sDMapX, sDMapY)].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
	UINT8 ubTargetElites = SectorInfo[SECTOR(sDMapX, sDMapY)].ubNumberOfCivsAtLevel[ ELITE_MILITIA ];

	UINT8 ubSourceMilitia = ubSourceGreens + ubSourceRegulars + ubSourceElites;
	UINT8 ubTargetMilitia = ubTargetGreens + ubTargetRegulars + ubTargetElites;

	UINT16 usSourceEnemiesInFiveSectors = NumEnemiesInFiveSectors( sMapX, sMapY );
	UINT16 usTargetEnemiesInFiveSectors = NumEnemiesInFiveSectors( sDMapX, sDMapY );
	UINT8 ubTargetEnemies = NumNonPlayerTeamMembersInSector( sDMapX, sDMapY, ENEMY_TEAM );

	UINT16 usSourceMilitiaInFiveSectors = CountAllMilitiaInFiveSectors( sMapX, sMapY );
	UINT16 usTargetMilitiaInFiveSectors = CountAllMilitiaInFiveSectors( sDMapX, sDMapY );

	UINT8 bSourceTownId = GetTownIdForSector( sMapX, sMapY );
	UINT8 bTargetTownId = GetTownIdForSector( sDMapX, sDMapY );
	BOOLEAN fTargetSAMSite = IsThisSectorASAMSector( sDMapX, sDMapY, 0 );

	// Is target sector a city/SAM where militia can be recruited?
	if ( (bTargetTownId != BLANK_SECTOR && gfMilitiaAllowedInTown[bTargetTownId]) 
		|| fTargetSAMSite )
	{
		// Does roaming group have anything to contribute to this town/SAM?
		if ( ubTargetElites >= ubMaxSquadSize || // Target sector full of elites
			(ubTargetRegulars+ubTargetElites >= ubMaxSquadSize && ubSourceElites == 0) || // Target sector full of Elite+Regular, and source has no elites
			(ubTargetMilitia >= ubMaxSquadSize && ubSourceRegulars+ubSourceElites == 0) ) // Target sector is full of anything, Souce sector has no Elites or Regulars
		{
				// Ignore sector.
				return 0;
		}
	}

	// Else, target sector is a free-roaming sector.
	else
	{
		// Are both squads full of militia?
		if	(ubSourceMilitia >= ubMaxSquadSize && ubTargetMilitia >= ubMaxSquadSize)
		{
			// Are both squads the same composition?
			if ( ubSourceGreens == ubTargetGreens &&
				ubSourceRegulars == ubTargetRegulars &&
				ubSourceElites == ubTargetElites )
			{
				// No reason to merge or average the squads, so ignore this sector.
				return 0;
			}
		}

		// Does source group have anything to offer to the target group?
		else if (ubTargetElites >= ubMaxSquadSize || // Target full of elites already
			( ubTargetRegulars+ubTargetElites >= ubMaxSquadSize && ubSourceElites == 0 ) || // Target full of regulars and elites, source group has no elites
			( ubTargetMilitia >= ubMaxSquadSize && ubSourceElites + ubSourceRegulars == 0 ) ) // Target full of militia, and source group only has greens
		{
			// Source group has nothing to offer the target group.
			return 0;
		}

		// If source sector is threatened by enemies, while target sector is not, do not move away!
		else if (usSourceEnemiesInFiveSectors && !usTargetEnemiesInFiveSectors)
		{
			return 0;
		}
	}

	// Target sector has room for more militia?
	if( ubTargetMilitia < ubMaxSquadSize )
		// Bonus rating
		iRes += DIR_WITH_UNFULL_SQUAD_RATING_BONUS;

	// Modify desire for militia-initiated attack
	if( ubTargetEnemies )
	{
		iDiff = (FLOAT)(usTargetMilitiaInFiveSectors / usTargetEnemiesInFiveSectors);

		if( iDiff > (FLOAT)DIR_MIN_DIF )
			iRes = (INT32)((FLOAT)iRes * iDiff);
		else
			iRes = 0;
	}
	
	// Test for player mercs in target sector.
	if( PlayerMercsInSector_MSE( (UINT8)sDMapX, (UINT8)sDMapY, FALSE ))
	{
		// There are mercs at the target. Increase movement desire.
		if ( bTargetTownId == BLANK_SECTOR && !fTargetSAMSite)
		{
			iRes += 15000;// should be enough
		}
		// If the target is a city, only 50% chance to increase the desire. This avoids situations where
		// a Mobile group is stuck next to a merc-occupied city sector, unable to get in and unable to
		// move away...
		else if (((bTargetTownId != BLANK_SECTOR && gfMilitiaAllowedInTown[bTargetTownId]) || fTargetSAMSite) && 
			Random(2)==0)
		{
			iRes += 15000;
		}
	}

//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"M %ld, E%ld,%ld %ld, Diff %ld", CountAllMilitiaInSector( sMapX, sMapY ), sDMapX, sDMapY, NumNonPlayerTeamMembersInSector( sDMapX, sDMapY, ENEMY_TEAM ), ((INT32)CountAllMilitiaInSector( sMapX, sMapY )) - ((INT32)NumNonPlayerTeamMembersInSector( sDMapX, sDMapY, ENEMY_TEAM )) );

	return iRes;
}


// Standard conditions the direction should fit
/////////////////////////////////////////////////
// HEADROCK HAM 3.4: Please note that the following function is used for ANY restrictions on movement. It tests
// everything from "groundbarriers" (impassable sector edge), to Player-Set Mobile Militia Restrictions, to hardcoded 
// stuff like whether militia can move in or out of cities (or any other sector, for that matter).
//
// If you wish to change where militia can and can't move to, THIS IS IT. 
// All sub-functions called here are related to the same purpose.
//
BOOLEAN CheckStandardConditionsForDirection( INT16 sSMapX, INT16 sSMapY, INT16 sMapX, INT16 sMapY, BOOLEAN fForTraining, BOOLEAN fForBattle )
{
	SECTORINFO * pTargetSector = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	if (fForBattle)
	{
		// When checking for reinforcements for battle, everything is THE OTHER WAY AROUND. Don't ask me why.
		INT16 sTempMap;
		sTempMap = sMapX;
		sMapX = sSMapX;
		sSMapX = sTempMap;

		sTempMap = sMapY;
		sMapY = sSMapY;
		sSMapY = sTempMap;
	}

	// At the end of the function we will check the status of these flags to determine
	// whether movement is indeed allowed.
	BOOLEAN fTraverseAllowed = FALSE;
	BOOLEAN fReadyForBattle = FALSE;
	BOOLEAN fRoamingAllowed = FALSE;
	BOOLEAN fCanBecomeGarrison = FALSE;
	BOOLEAN fCanSimpleMove = FALSE;


	//////////////////////////////////////////////////////////////////////////////////////////
	// The first check makes sure that travel between the sectors is allowed at all. Ground-Barriers 
	// are special because they also prevent merc and enemy movement.
	UINT8 uiTravab = GetTraversability( SECTOR( sSMapX , sSMapY ) , SECTOR( sMapX, sMapY ) );
 	
	if( uiTravab == GROUNDBARRIER || uiTravab == EDGEOFWORLD )
	{
		fTraverseAllowed = FALSE;
	}
	else
	{
		fTraverseAllowed = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// If militia aren't set-up for battle, they can't move into an enemy sector. This prevents combat ever
	// being instigated by militia unless specifically requested to do so by setting all the appropriate "ready!" 
	// flags earlier in the code.
	if ( !fForBattle && gfMSBattle && NumNonPlayerTeamMembersInSector( sMapX, sMapY, ENEMY_TEAM ) )
	{
		fReadyForBattle = FALSE;
	}
	else
	{
		fReadyForBattle = TRUE;
	}

	fRoamingAllowed = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Movement into and out of city/sam sectors is more complicated to check. 
	// 
	// Movement out of a town/sam sector is only allowed when reinforcing other town/sam sectors. This means militia
	// don't move around town unless called to do so.
	// Movement from the wilderness into a town/sam is different in HAM 3.3, where roaming militias can go into town
	// and turn into Garrisons (regular standing militia). They can also reinforce the city when it comes under attack,
	// but only if allowed to do so by the current GameSettings.
	// Please note that Minor Cities are considered halfway between the two. These are cities like Estoni or
	// San Mona, where Militia cannot be trained. Roaming Militia can now be allowed to travel through these as they
	// would through wilderness areas. It also includes larger facilities like Tixa and Orta. Militia will not stay
	// there as garrisons, but may simply move through.

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Begin by setting flags for the source sector

	BOOLEAN fSourceIsMajorTown = FALSE;
	BOOLEAN fSourceIsMinorTown = FALSE;
	BOOLEAN fSourceIsSamSite = FALSE;

	if (GetTownIdForSector( sSMapX, sSMapY) != BLANK_SECTOR)
	{
		// A "Major" city is one where militia can be trained. Drassen, Chitzena, Meduna, etcetera.
		if (gfMilitiaAllowedInTown[GetTownIdForSector( sSMapX, sSMapY )])
		{
			fSourceIsMajorTown = TRUE;
			fSourceIsMinorTown = FALSE;
		}
		// A "Minor" city cannot train militia. This includes places like San Mona, Omerta,
		// and even Tixa and Estoni
		else
		{
			fSourceIsMajorTown = FALSE;
			fSourceIsMinorTown = TRUE;
		}
	}
	else
	{
		fSourceIsSamSite = IsThisSectorASAMSector( sSMapX, sSMapY, 0 );
	}

	// And the target sector

	BOOLEAN fTargetIsMajorTown = FALSE;
	BOOLEAN fTargetIsMinorTown = FALSE;
	BOOLEAN fTargetIsSamSite = FALSE;

	if (GetTownIdForSector( sMapX, sMapY) != BLANK_SECTOR)
	{
		if (gfMilitiaAllowedInTown[GetTownIdForSector( sMapX, sMapY )])
		{
			fTargetIsMajorTown = TRUE;
			fTargetIsMinorTown = FALSE;
		}
		else
		{
			fTargetIsMajorTown = FALSE;
			fTargetIsMinorTown = TRUE;
		}
	}
	else
	{
		fTargetIsSamSite = IsThisSectorASAMSector( sMapX, sMapY, 0 );
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	// First we run the test for battle reinforcement conditions.
	if ( fForBattle && fReadyForBattle )
	{
		// Is reinforcement allowed only in cities?
		if (gGameExternalOptions.gfAllowReinforcementsOnlyInCity)
		{
			// Make sure both origin and destination are cities.
			if ( fSourceIsMajorTown && fTargetIsMajorTown )
			{
				fReadyForBattle = TRUE;
			}
			else
			{
				// Reinforcing from Wilderness is impossible.
				fReadyForBattle = FALSE;
			}
		}
		
		// Battle reinforcements allowed from both cities and wilderness sectors.
		else
		{
			// Trying to do battle reinforcement from a town/sam to the wilderness?
			if ( ( fSourceIsMajorTown || fSourceIsSamSite ) &&
				!(fTargetIsMajorTown || fTargetIsSamSite ) )
			{
				// No! Static troops cannot reinforce Mobile troops for any reason.
				fReadyForBattle = FALSE;
			}
			else
			{
				fReadyForBattle = TRUE;
			}
		}

		// RETURN.
		if ( fTraverseAllowed && fReadyForBattle && fRoamingAllowed )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	// Else, if we are doing a "peaceful" movement. 
	else if ( !fForBattle )
	{
		// Are we trying to bolster a local garrison?
		if (fTargetIsSamSite || fTargetIsMajorTown)
		{
			return FALSE;
		}

		// Trying to move peacefully out of a Town/SAM?
		else if (fSourceIsMajorTown || fSourceIsSamSite)
		{
			// This should ONLY be possible if we are explicitly training new militia groups.
			// Also, militia can no longer spawn directly into enemy-controlled territory.
			if ( fForTraining && !NumNonPlayerTeamMembersInSector( sMapX, sMapY, ENEMY_TEAM ) )
			{
				fCanSimpleMove = TRUE;
			}
			else
			{
				// Otherwise always impossible. We don't want militia leaving the cities for any other reason!
				fCanSimpleMove = FALSE;
			}
		}

		// Test for Wilderness/Minor -> Minor
		else if (fTargetIsMinorTown)
		{
			// Movement through Minor Cities isn't allowed.
			fCanSimpleMove = FALSE;
		}
		// Wilderness/Minor -> Wilderness
		else
		{
			// Always possible.
			fCanSimpleMove = TRUE;
		}

		// RETURN.
		if ( fTraverseAllowed && fRoamingAllowed && fCanSimpleMove )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	return FALSE;
}

void GenerateDirectionInfos( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3], BOOLEAN fForTraining, BOOLEAN fForBattle )
{
	*uiDirNumber = 0;

#ifdef DEBUG_SHOW_RATINGS
	if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld-------------------", sMapX, sMapY );
#endif

	if(sMapY > MINIMUM_VALID_Y_COORDINATE && CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY - 1, fForTraining, fForBattle ) )
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
	if(sMapX > MINIMUM_VALID_X_COORDINATE && CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX - 1, sMapY, fForTraining, fForBattle ))
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
	if(sMapY < MAXIMUM_VALID_Y_COORDINATE && CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX, sMapY + 1, fForTraining, fForBattle ))
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
	if(sMapX < MAXIMUM_VALID_X_COORDINATE && CheckStandardConditionsForDirection(	sMapX, sMapY, sMapX + 1, sMapY, fForTraining, fForBattle ))
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
	for( UINT16 i = 0; i < uiBlockMoveListPointer ; ++i)
	{
		if( sMapX == SECTORX(pBlockMoveList[i]) && sMapY == SECTORY(pBlockMoveList[i]) )
			return TRUE;
	}

	return FALSE;
}

void MilitiaMovementOrder(UINT8 sector)
{
	INT16 sX = SECTORX(sector);
	INT16 sY = SECTORY(sector);

	int strategicsector = CALCULATE_STRATEGIC_INDEX( sX, sY );

	// if we are in gamescreen and a battle is going on, and this is the sector that militia moves from, don't move them. Suddenly disappearing would be... awkward
	if ( guiCurrentScreen == GAME_SCREEN && gTacticalStatus.uiFlags & INCOMBAT && gWorldSectorX == sX && gWorldSectorY == sY && !gbWorldSectorZ )
	{
		// remove all movement flags
		StrategicMap[strategicsector].usFlags &= ~MILITIA_MOVE_ALLDIRS;
		return;
	}

	// to stop the player from just setting someone in HQ to give orders and then reassigning them, we simply demand that there'll also be someone when the militia peform the travelling
	if ( !MercStaffsMilitaryHQ() )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gpStrategicString[STR_MILITIAMOVEMENT_NO_STAFF_ABORT] );

		// remove all movement flags
		StrategicMap[strategicsector].usFlags &= ~MILITIA_MOVE_ALLDIRS;
		return;
	}
	
	INT16 targetX = sX;
	INT16 targetY = sY;

	if ( StrategicMap[strategicsector].usFlags & MILITIA_MOVE_NORTH )
		--targetY;
	else if ( StrategicMap[strategicsector].usFlags & MILITIA_MOVE_WEST )
		--targetX;
	else if ( StrategicMap[strategicsector].usFlags & MILITIA_MOVE_EAST )
		++targetX;
	else if ( StrategicMap[strategicsector].usFlags & MILITIA_MOVE_SOUTH )
		++targetY;

	// remove all movement flags
	StrategicMap[strategicsector].usFlags &= ~MILITIA_MOVE_ALLDIRS;

	UINT8 targetsector = SECTOR(targetX, targetY);
	
	// if militia moves from the currently loaded sector, it has to drop all gear first
	if ( gGameExternalOptions.fMilitiaUseSectorInventory && gWorldSectorX == sX && gWorldSectorY == sY && !gbWorldSectorZ )
	{
		TeamDropAll( MILITIA_TEAM );
	}
	
	MoveMilitiaSquad( sX, sY, targetX, targetY, FALSE, TRUE );		

	if ( gfStrategicMilitiaChangesMade)
		ResetMilitia();

	if ( NumNonPlayerTeamMembersInSector( targetX, targetY, ENEMY_TEAM ) )
	{
		gfMSBattle = TRUE;

		EnterAutoResolveMode( targetX,  targetY );
	}
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

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, TRUE );

	ZeroMemory( gpAttackDirs, sizeof( gpAttackDirs ) );

	gpAttackDirs[ guiDirNumber ][0] = uiNumGreen = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
	gpAttackDirs[ guiDirNumber ][1] = uiNumReg = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
	gpAttackDirs[ guiDirNumber ][2] = uiNumElite = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
	gpAttackDirs[ guiDirNumber ][3] = INSERTION_CODE_CENTER;

	guiDirNumber = uiDirNumber	+ 1;

	x = 0;
	while ( NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM ) < gGameExternalOptions.iMaxMilitiaPerSector &&
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
			if(is_server && gMilitiaEnabled == 1)
				PrepareMilitiaForTactical(FALSE);
		}
	}
	
	gfStrategicMilitiaChangesMade = FALSE;
}

// Flugente: order sNumber reinforcements from src sector to target sector
BOOLEAN CallMilitiaReinforcements( INT16 sTargetMapX, INT16 sTargetMapY, INT16 sSrcMapX, INT16 sSrcMapY, UINT16 sNumber )
{
	UINT8 uiNumGreen = 0, uiNumReg = 0, uiNumElite = 0;
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sTargetMapX, sTargetMapY ) ] );

	guiDirNumber = 0;

	UINT8 insertioncode = INSERTION_CODE_CENTER;
	UINT8 movetype = THROUGH_STRATEGIC_MOVE;

	// determine from which direction militia should enter (and exit if this isn't possible)
	if ( sTargetMapX == sSrcMapX + 1 && sTargetMapY == sSrcMapY )
	{
		insertioncode = INSERTION_CODE_WEST;
		movetype = EAST_STRATEGIC_MOVE;		
	}
	else if ( sTargetMapX == sSrcMapX - 1 && sTargetMapY == sSrcMapY )
	{
		insertioncode = INSERTION_CODE_EAST;
		movetype = WEST_STRATEGIC_MOVE;
	}
	else if ( sTargetMapX == sSrcMapX && sTargetMapY == sSrcMapY + 1 )
	{
		insertioncode = INSERTION_CODE_NORTH;
		movetype = SOUTH_STRATEGIC_MOVE;
	}
	else if ( sTargetMapX == sSrcMapX && sTargetMapY == sSrcMapY - 1 )
	{
		insertioncode = INSERTION_CODE_SOUTH;
		movetype = NORTH_STRATEGIC_MOVE;
	}
	else
	{
		// no proper direction here... get out
		return FALSE;
	}

	// test wether travel from src to target is possible ( we cannot open the src map information, we'll rely on the xml data instead
	if( SectorInfo[ SECTOR(sSrcMapX,sSrcMapY) ].ubTraversability[ movetype ] == GROUNDBARRIER || SectorInfo[ SECTOR(sSrcMapX,sSrcMapY) ].ubTraversability[ movetype ] == EDGEOFWORLD )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Militia cannot traverse to this sector due to the terrain." );
		return FALSE;
	}

	guiDirNumber = 0;
		
	ZeroMemory( gpAttackDirs, sizeof( gpAttackDirs ) );

	gpAttackDirs[guiDirNumber][0] = uiNumGreen = MilitiaInSectorOfRank( sTargetMapX, sTargetMapY, GREEN_MILITIA );
	gpAttackDirs[guiDirNumber][1] = uiNumReg = MilitiaInSectorOfRank( sTargetMapX, sTargetMapY, REGULAR_MILITIA );
	gpAttackDirs[guiDirNumber][2] = uiNumElite = MilitiaInSectorOfRank( sTargetMapX, sTargetMapY, ELITE_MILITIA );
	gpAttackDirs[ guiDirNumber ][3] = INSERTION_CODE_CENTER;

	guiDirNumber = insertioncode + 1;

	// we can't move more than we are told, and can't add more than would fit into the sector
	UINT16 possibletomove = min( sNumber, gGameExternalOptions.iMaxMilitiaPerSector - NumNonPlayerTeamMembersInSector( sTargetMapX, sTargetMapY, MILITIA_TEAM ) );

	UINT16 sMilitiaMoved = 0;
	while ( sMilitiaMoved < possibletomove && MoveOneBestMilitiaMan( sSrcMapX, sSrcMapY, sTargetMapX, sTargetMapY ) )
	{
		++sMilitiaMoved;

		UINT8 militia_green = MilitiaInSectorOfRank( sTargetMapX, sTargetMapY, GREEN_MILITIA );
		UINT8 militia_troop = MilitiaInSectorOfRank( sTargetMapX, sTargetMapY, REGULAR_MILITIA );
		UINT8 militia_elite = MilitiaInSectorOfRank( sTargetMapX, sTargetMapY, ELITE_MILITIA );

		gpAttackDirs[guiDirNumber][0] += militia_green - uiNumGreen;
		gpAttackDirs[guiDirNumber][1] += militia_troop - uiNumReg;
		gpAttackDirs[guiDirNumber][2] += militia_elite - uiNumElite;
		gpAttackDirs[ guiDirNumber ][3] = insertioncode;

		uiNumGreen = militia_green;
		uiNumReg = militia_troop;
		uiNumElite = militia_elite;
	}

	guiDirNumber = 5;

	if ( !sMilitiaMoved )
		return FALSE;

	// we need to use this flag. If it wasn't set prior to this, we remove it again afterwards, otherwise all militia will join us if we are in combat
	BOOLEAN wantreinforcements = (gTacticalStatus.uiFlags & WANT_MILITIA_REINFORCEMENTS);
	gTacticalStatus.uiFlags |= WANT_MILITIA_REINFORCEMENTS;

	if (is_networked)
	{
		if (gfStrategicMilitiaChangesMade)
		{
			RemoveMilitiaFromTactical();
			if(is_server && gMilitiaEnabled == 1)
				PrepareMilitiaForTactical(FALSE);
		}
	}
	else
		PrepareMilitiaForTactical(FALSE);

	if ( !wantreinforcements )
		gTacticalStatus.uiFlags &= ~WANT_MILITIA_REINFORCEMENTS;
	
	gfStrategicMilitiaChangesMade = FALSE;

	return TRUE;
}

BOOLEAN MilitiaGroupEntersCurrentSector( UINT8 usGroupId, INT16 sMapX, INT16 sMapY )
{
	GROUP* pGroup = GetGroup( usGroupId );

	if ( !pGroup || pGroup->usGroupTeam != MILITIA_TEAM )
		return FALSE;

	INT16 sSrcMapX = pGroup->ubPrevX;
	INT16 sSrcMapY = pGroup->ubPrevY;
	
	UINT8 insertioncode = INSERTION_CODE_CENTER;
	UINT8 movetype = THROUGH_STRATEGIC_MOVE;

	// determine from which direction militia should enter (and exit if this isn't possible)
	if ( sMapX == sSrcMapX + 1 && sMapY == sSrcMapY )
	{
		insertioncode = INSERTION_CODE_WEST;
		movetype = EAST_STRATEGIC_MOVE;
	}
	else if ( sMapX == sSrcMapX - 1 && sMapY == sSrcMapY )
	{
		insertioncode = INSERTION_CODE_EAST;
		movetype = WEST_STRATEGIC_MOVE;
	}
	else if ( sMapX == sSrcMapX && sMapY == sSrcMapY + 1 )
	{
		insertioncode = INSERTION_CODE_NORTH;
		movetype = SOUTH_STRATEGIC_MOVE;
	}
	else if ( sMapX == sSrcMapX && sMapY == sSrcMapY - 1 )
	{
		insertioncode = INSERTION_CODE_SOUTH;
		movetype = NORTH_STRATEGIC_MOVE;
	}
	else
	{
		// no proper direction here... get out
		return FALSE;
	}

	// test wether travel from src to target is possible ( we cannot open the src map information, we'll rely on the xml data instead
	if ( SectorInfo[SECTOR( sSrcMapX, sSrcMapY )].ubTraversability[movetype] == GROUNDBARRIER || SectorInfo[SECTOR( sSrcMapX, sSrcMapY )].ubTraversability[movetype] == EDGEOFWORLD )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Militia cannot traverse to this sector due to the terrain." );
		return FALSE;
	}
	
	ZeroMemory( gpAttackDirs, sizeof(gpAttackDirs) );
		
	guiDirNumber = insertioncode + 1;

	gpAttackDirs[guiDirNumber][0] = pGroup->pEnemyGroup->ubNumAdmins;
	gpAttackDirs[guiDirNumber][1] = pGroup->pEnemyGroup->ubNumTroops;
	gpAttackDirs[guiDirNumber][2] = pGroup->pEnemyGroup->ubNumElites;
	gpAttackDirs[guiDirNumber][3] = insertioncode;
		
	guiDirNumber = 5;

	// we need to use this flag. If it wasn't set prior to this, we remove it again afterwards, otherwise all militia will join us if we are in combat
	BOOLEAN wantreinforcements = (gTacticalStatus.uiFlags & WANT_MILITIA_REINFORCEMENTS);
	gTacticalStatus.uiFlags |= WANT_MILITIA_REINFORCEMENTS;

	if ( is_networked )
	{
		if ( gfStrategicMilitiaChangesMade )
		{
			RemoveMilitiaFromTactical( );
			if ( is_server && gMilitiaEnabled == 1 )
				PrepareMilitiaForTactical( FALSE );
		}
	}
	else
		PrepareMilitiaForTactical( FALSE );

	if ( !wantreinforcements )
		gTacticalStatus.uiFlags &= ~WANT_MILITIA_REINFORCEMENTS;

	gfStrategicMilitiaChangesMade = FALSE;
	
	return TRUE;
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

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, TRUE );

	for ( UINT8 x = 0; x < uiDirNumber; ++x )
	{
		if ( NumNonPlayerTeamMembersInSector( SECTORX( pMoveDir[x][0] ), SECTORY( pMoveDir[x][0] ), MILITIA_TEAM ) )
			return TRUE;
	}

	return FALSE;
}

void MilitiaHelpFromAdjacentSectors( INT16 sMapX, INT16 sMapY )
{
	sMSMapX = sMapX;
	sMSMapY = sMapY;
	CHAR16 pStr[256];
	CHAR16 pStr2[128];

	GetSectorIDString( sMapX, sMapY, 0, pStr2, FALSE );

	if( !gGameExternalOptions.gfAllowReinforcements )
		return;

	gTacticalStatus.uiFlags &= (~WANT_MILITIA_REINFORCEMENTS);

	guiDirNumber = 0;

//	if( CountAllMilitiaInSector( sMapX, sMapY ) ) MSCallBack( MSG_BOX_RETURN_YES );

	// This is no longer a question of simply whether to have a full militia count, but also whether we want
	// reinforcements.	So if there are any available, always ask.
	if( IsThereMilitiaInAdjacentSector( sMapX, sMapY ) ) // && CountAllMilitiaInSector( sMapX, sMapY ) < gGameExternalOptions.guiMaxMilitiaSquadSizeBattle )
	{
		// HEADROCK HAM 3.5: Added sector name to reinforcements string.
		swprintf(pStr, gzCWStrings[0], pStr2);
		//DoScreenIndependantMessageBox( gzCWStrings[0], MSG_BOX_FLAG_YESNO, MSCallBack );
		DoScreenIndependantMessageBox( pStr, MSG_BOX_FLAG_YESNO, MSCallBack );
	}
}

// HEADROCK HAM B2.7: This is a copy of an existing function that generates possible movement directions for militia.
// Due to the widespread use of that function, I've decided to make a new one that calculates directions
// when generating militia. This function checks for available space at the target location. The stored results
// indicate the amount of space left in the target sector.
// HEADROCK HAM 3.6: Revamped to fix bugs.

void GenerateDirectionInfosForTraining( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3] )
{
	UINT8 ubMaxMilitia = gGameExternalOptions.iMaxMilitiaPerSector;

	*uiDirNumber = 0;
	UINT16 usMilitiaCapacity = 0;
	UINT16 usUpgradeCapacity = 0;

	// Clean up pMoveDir
	for (UINT8 x = 0; x < 4; ++x)
	{
		pMoveDir[x][0]=0;
		pMoveDir[x][1]=0;
		pMoveDir[x][2]=0;
	}

#ifdef DEBUG_SHOW_RATINGS
	if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld-------------------", sMapX, sMapY );
#endif

	// Check north
	if( sMapY > MINIMUM_VALID_Y_COORDINATE )
	{
		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY - 1, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - NumNonPlayerTeamMembersInSector( sMapX, sMapY - 1, MILITIA_TEAM ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( sMapX, sMapY - 1 );

			// Found at least one slot here?
			if (usUpgradeCapacity || usMilitiaCapacity)
			{			
				// Store data in return variable.
				pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY - 1 );
				pMoveDir[ *uiDirNumber ][1] = usMilitiaCapacity + usUpgradeCapacity;
				pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_NORTH;
	
				#ifdef DEBUG_SHOW_RATINGS
						if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
				#endif
	
				++(*uiDirNumber);
			}
		}
	}

	// Check west
	if( sMapX > MINIMUM_VALID_X_COORDINATE )
	{
		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX - 1, sMapY, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - NumNonPlayerTeamMembersInSector( sMapX - 1, sMapY, MILITIA_TEAM ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( sMapX - 1, sMapY );

			// Found at least one slot here?
			if (usUpgradeCapacity || usMilitiaCapacity)
			{	
				// Store data in return variable.
				pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX - 1, sMapY );
				pMoveDir[ *uiDirNumber ][1] = usMilitiaCapacity + usUpgradeCapacity;
				pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_WEST;

				#ifdef DEBUG_SHOW_RATINGS
						if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
				#endif

				++(*uiDirNumber);
			}
		}
	}

	// Check south
	if( sMapY < MAXIMUM_VALID_Y_COORDINATE )
	{
		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY + 1, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - NumNonPlayerTeamMembersInSector( sMapX, sMapY + 1, MILITIA_TEAM ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( sMapX, sMapY + 1 );

			// Found at least one slot here?
			if (usUpgradeCapacity || usMilitiaCapacity)
			{	
				// Store data in return variable.
				pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX, sMapY + 1 );
				pMoveDir[ *uiDirNumber ][1] = usMilitiaCapacity + usUpgradeCapacity;
				pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_SOUTH;

				#ifdef DEBUG_SHOW_RATINGS
						if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
				#endif

				++(*uiDirNumber);
			}
		}
	}

	// Check east
	if( sMapX < MAXIMUM_VALID_X_COORDINATE )
	{
		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX +1 , sMapY, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - NumNonPlayerTeamMembersInSector( sMapX + 1, sMapY, MILITIA_TEAM ) );
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( sMapX + 1, sMapY );

			// Found at least one slot here?
			if (usUpgradeCapacity || usMilitiaCapacity)
			{	
				// Store data in return variable.
				pMoveDir[ *uiDirNumber ][0] = SECTOR( sMapX + 1, sMapY );
				pMoveDir[ *uiDirNumber ][1] = usMilitiaCapacity + usUpgradeCapacity;
				pMoveDir[ *uiDirNumber ][2] = INSERTION_CODE_EAST;

				#ifdef DEBUG_SHOW_RATINGS
						if( DEBUG_RATINGS_CONDITION )ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Priority of north direction - %ld", pMoveDir[ *uiDirNumber ][1]);
				#endif

				++(*uiDirNumber);
			}
		}
	}
}

// HEADROCK HAM 3.6: Function returns the number of "upgrade slots" available at target sector. Basically, every GREEN
// Militia at the target sector counts as 2 upgrade slots, while every regular counts as 1.

UINT16 MilitiaUpgradeSlotsCheck( INT16 sMapX, INT16 sMapY )
{
	// Result variables
	UINT16 usNumUpgradeSlots = 0;

	// Target sector full of elites?	
	if ( MilitiaInSectorOfRank( sMapX, sMapY, ELITE_MILITIA ) >= gGameExternalOptions.iMaxMilitiaPerSector )
	{
		// Militia cannot be placed or moved here at all.
		return 0;
	}

	// If not allowed to train elites ( or not yet )
	if (!gGameExternalOptions.gfTrainVeteranMilitia || // Not allowed at all
		(GetWorldDay( ) < gGameExternalOptions.guiTrainVeteranMilitiaDelay)) // Not yet allowed.
	{
		// If sector full of Regulars and Elites?
		if ( MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA ) + MilitiaInSectorOfRank( sMapX, sMapY, ELITE_MILITIA ) >= gGameExternalOptions.iMaxMilitiaPerSector ) // Target sector full of regular+elite militia
		{
			// Can't upgrade Regulars to Elites, so the sector is already full.
			return 0;
		}

		// Each Green we can convert to Regular counts as 2 slots.
		usNumUpgradeSlots = MilitiaInSectorOfRank( sMapX, sMapY, GREEN_MILITIA ) * 2;

		return (usNumUpgradeSlots);
	}

	// Allowed to train Green->Regular, and Regular->Elite
	usNumUpgradeSlots = MilitiaInSectorOfRank( sMapX, sMapY, GREEN_MILITIA ) * 2;
	usNumUpgradeSlots += MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA );

	return (usNumUpgradeSlots);
}

// Flugente: militia movement
BOOLEAN SaveMilitiaMovementInformationToSaveGameFile( HWFILE hFile )
{
	UINT32		uiNumBytesWritten = 0;

	UINT8 number = MILITIA_PATROLS_MAX;

	//Save the number of elements
	FileWrite( hFile, &number, sizeof(UINT8), &uiNumBytesWritten );
	if ( uiNumBytesWritten != sizeof(UINT8) )
	{
		return(FALSE);
	}

	for ( UINT8 cnt = 0; cnt < MILITIA_PATROLS_MAX; ++cnt )
	{
		UINT32		uiNumOfNodes = 0;
		PathStPtr	pTempPath = gMilitiaPath[cnt].path;
		
		uiNumBytesWritten = 0;

		//loop through to get all the nodes
		while ( pTempPath )
		{
			++uiNumOfNodes;
			pTempPath = pTempPath->pNext;
		}

		//Save the number of the nodes
		FileWrite( hFile, &uiNumOfNodes, sizeof(UINT32), &uiNumBytesWritten );
		if ( uiNumBytesWritten != sizeof(UINT32) )
		{
			return(FALSE);
		}

		//loop through all the nodes and add them
		pTempPath = gMilitiaPath[cnt].path;

		//loop through nodes and save all the nodes
		while ( pTempPath )
		{
			//Save the number of the nodes
			FileWrite( hFile, pTempPath, sizeof(PathSt), &uiNumBytesWritten );
			if ( uiNumBytesWritten != sizeof(PathSt) )
			{
				return(FALSE);
			}

			pTempPath = pTempPath->pNext;
		}

		//Save the group id
		FileWrite( hFile, &gMilitiaPath[cnt].sGroupid, sizeof(INT16), &uiNumBytesWritten );
		if ( uiNumBytesWritten != sizeof(INT16) )
		{
			return(FALSE);
		}
	}

	return(TRUE);
}

BOOLEAN LoadMilitiaMovementInformationFromSavedGameFile( HWFILE hFile, UINT32 uiSavedGameVersion )
{
	UINT32		uiNumBytesRead;
	UINT32		uiTotalNodeCount = 0;
	UINT32		uiNodeCount = 0;
	PathSt		*pPath = NULL;
	UINT8		ubPassengerCnt = 0;
	UINT8		numpaths = 0;

	DeleteAllMilitiaPaths( );

	// if this is an older savegame, nothing to read
	if ( uiSavedGameVersion < MILITIA_PATH_PLOTTING )
		return TRUE;

	//Load the number of elements
	FileRead( hFile, &numpaths, sizeof(UINT8), &uiNumBytesRead );
	if ( uiNumBytesRead != sizeof(UINT8) )
	{
		return(FALSE);
	}

	if ( numpaths != MILITIA_PATROLS_MAX )
		return(FALSE);
	
	for ( UINT32 cnt = 0; cnt < MILITIA_PATROLS_MAX; ++cnt )
	{
		UINT32		uiNumOfNodes = 0;
		PathStPtr	pTempPath = NULL;
		PathStPtr	pTemp = NULL;
		UINT32		uiNumBytesRead = 0;
		INT16		groupid = 0;
			
		//Load the number of the nodes
		FileRead( hFile, &uiNumOfNodes, sizeof(UINT32), &uiNumBytesRead );
		if ( uiNumBytesRead != sizeof(UINT32) )
		{
			return(FALSE);
		}

		//load all the nodes
		for ( UINT32 cnt2 = 0; cnt2 < uiNumOfNodes; ++cnt2 )
		{
			//Allocate memory for the new node
			pTemp = (PathStPtr)MemAlloc( sizeof(PathSt) );
			if ( pTemp == NULL )
			{
				pTempPath = MoveToBeginningOfPathList( pTempPath );
				ClearStrategicPathList( pTempPath, -1 );
				return(FALSE);
			}

			memset( pTemp, 0, sizeof(PathSt) );

			//Load the node
			FileRead( hFile, pTemp, sizeof(PathSt), &uiNumBytesRead );
			if ( uiNumBytesRead != sizeof(PathSt) )
			{
				MemFree( pTemp );
				pTempPath = MoveToBeginningOfPathList( pTempPath );
				ClearStrategicPathList( pTempPath, -1 );
				return(FALSE);
			}

			//Put the node into the list 
			if ( cnt2 == 0 )
			{
				pTempPath = pTemp;
				pTemp->pPrev = NULL;
			}
			else
			{
				pTempPath->pNext = pTemp;
				pTemp->pPrev = pTempPath;

				pTempPath = pTempPath->pNext;
			}

			pTemp->pNext = NULL;
		}

		//move to beginning of list
		pTempPath = MoveToBeginningOfPathList( pTempPath );

		gMilitiaPath[cnt].path = pTempPath;
		if ( gMilitiaPath[cnt].path )
			gMilitiaPath[cnt].path->pPrev = NULL;

		//Load the number of the nodes
		FileRead( hFile, &groupid, sizeof(INT16), &uiNumBytesRead );
		if ( uiNumBytesRead != sizeof(INT16) )
		{
			return(FALSE);
		}

		gMilitiaPath[cnt].sGroupid = groupid;
	}

	return(TRUE);
}

// delete a militia group and transfer its militia to the sector they are in
void DissolveMilitiaGroup( UINT8 uGroupId )
{
	GROUP* pGroup = GetGroup( uGroupId );

	if ( pGroup && pGroup->usGroupTeam == MILITIA_TEAM )
	{
		ClearMercPathsAndWaypointsForAllInGroup( pGroup );

		StrategicAddMilitiaToSector( pGroup->ubSectorX, pGroup->ubSectorY, GREEN_MILITIA, pGroup->pEnemyGroup->ubNumAdmins );
		StrategicAddMilitiaToSector( pGroup->ubSectorX, pGroup->ubSectorY, REGULAR_MILITIA, pGroup->pEnemyGroup->ubNumTroops );
		StrategicAddMilitiaToSector( pGroup->ubSectorX, pGroup->ubSectorY, ELITE_MILITIA, pGroup->pEnemyGroup->ubNumElites );

		RemovePGroup( pGroup );
	}
}

void DissolveAllMilitiaGroupsInSector( INT16 sMapX, INT16 sMapY )
{
	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == sMapX && pGroup->ubSectorY == sMapY )
		{
			GROUP* pDeleteGroup = pGroup;
			pGroup = pGroup->next;

			DissolveMilitiaGroup( pDeleteGroup->ubGroupID );
		}
		else
			pGroup = pGroup->next;
	}
}

