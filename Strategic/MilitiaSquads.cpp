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

DYNAMICRESTRICTIONS gDynamicRestrictions[5001];

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
// HEADROCK HAM 3.4: Added Leadership argument.
void GenerateMilitiaSquad(INT16 sMapX, INT16 sMapY, INT16 sTMapX, INT16 sTMapY, UINT8 ubBestLeadership )
{
	//////////////////////////////////////////////////////////////////////////////////////
	// HEADROCK HAM 3.3: Introduced considerable changes here on top of HAM 2 code. You can replace
	// the entire function with this new code.
	// Mostly, the changes are geared towards variable militia QUANTITY due to variable trainer Leadership. 
	// Also introduced minimum required leadership for training mobile militia, and minimum required 
	// leadership for training a full squad.
	//////////////////////////////////////////////////////////////////////////////////////

	SECTORINFO *pSourceSector = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );
	SECTORINFO *pTargetSector = &( SectorInfo[ SECTOR( sTMapX, sTMapY ) ] );

	//HEADROCK HAM B2.7: Track the number of militia at the source.
	UINT16 usTotalMilitiaAtSource = pSourceSector->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] + pSourceSector->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] + pSourceSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
	UINT16 usTotalMilitiaAtTarget = pTargetSector->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] + pTargetSector->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] + pTargetSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
	// Desired number of Greens, Regulars and Elites to create.
	UINT8 ubTargetGreen, ubTargetRegular, ubTargetElite;
	// Upgrade points for replacing militia with better ones.
	UINT8 ubUpgradePoints;
	// Calculate default number of militia to place in Target Sector.
	UINT8 ubMilitiaToTrain = gGameExternalOptions.guiNumMobileMilitiaTrained;
	// Percentage of troops that should be of any one type:
	UINT8 ubTargetElitePercent = 0;
	UINT8 ubTargetRegularPercent = 0;
	UINT8 ubTempLeadership = ubBestLeadership;

	// Does trainer have enough leadership to train a squad?
	if ( ubBestLeadership < gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia )
	{
		// This is punishment, basically. The merc still gets the benefit of improving his leadership through the training
		// session, but a lot of money has basically gone to waste. This is something to watch out for, shall we say?
		ubMilitiaToTrain = 0;
	}
	else
	{
		// HEADROCK HAM 3.6: New function to do Quantity Calculation based on leadership
		ubMilitiaToTrain = CalcNumMilitiaTrained(ubBestLeadership, TRUE);
	}

	// HEADROCK HAM 3.4: Composition of new Mobile Militia groups is now dictated by two INI settings controlling
	// the percentage of Elites and Regulars within the group. If the percentage for either is above 0, at least
	// one militia of that type will be created every time. Green militia are created based on whatever remains.

	// HEADROCK HAM 3.5: Base percentages off the trainer's Leadership...
	if ( gGameExternalOptions.fLeadershipAffectsMobileMilitiaQuality &&
		(ubBestLeadership >= gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia) )
	{
		// Find out how far between the minimum and maximum our character is.
		INT8 bRange = gGameExternalOptions.ubReqLeadershipForFullMobileTraining - gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia;
		ubBestLeadership = __min(ubBestLeadership, gGameExternalOptions.ubReqLeadershipForFullMobileTraining);
		ubBestLeadership -= gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia;

		UINT8 ubBestLeadershipPercentage = (ubBestLeadership * 100) / bRange;

		// There are five "grades", which in practice actually give a smooth transition from All-Green to All-Elite.
		// Please note that if elite training is disabled, all elites will later be converted to Regulars. In that case,
		// a character needs to have at least 60% of the total leadership range to train a full squad of regulars.
		if (ubBestLeadershipPercentage >= 0 && ubBestLeadershipPercentage < 20) // First step, Greens
		{
			ubTargetElitePercent = 0; // No elites
			ubTargetRegularPercent = 0; // No regulars
			// All green.
		}
		else if (ubBestLeadershipPercentage >= 20 && ubBestLeadershipPercentage < 40)
		{
			ubTargetElitePercent = 0; // No elites
			ubTargetRegularPercent = __max(1,(((ubBestLeadershipPercentage-19) * 50) / 20)); // Up to 50% regulars
			// Green is 100%-ubTargetRegularPercent. So it's between 0/0/100 and 0/50/50
		}
		else if (ubBestLeadershipPercentage >= 40 && ubBestLeadershipPercentage < 60)
		{
			ubTargetElitePercent = __max(1,(((ubBestLeadershipPercentage-39) * 33) / 20)); // Up to 33% elites
			ubTargetRegularPercent = __max(1,(100 - ubTargetElitePercent)/2); // Half of what's left is regulars
			// And half is green. This gives anywhere between 1/49/49 and 33/33/33.
		}
		else if (ubBestLeadershipPercentage >= 60 && ubBestLeadershipPercentage < 80)
		{
			UINT8 ubTargetGreenPercent = __max(1, (((20 - (ubBestLeadershipPercentage-60))*33) / 20));
			ubTargetElitePercent = (100-ubTargetGreenPercent)/2;
			ubTargetRegularPercent = ubTargetElitePercent;
			// Green percent drops as your leadership goes up, while Elite and Regular are equal.
			// This gives anywhere between 33/33/33 and 49/49/1
		}
		else if (ubBestLeadershipPercentage >= 80 && ubBestLeadershipPercentage < 100)
		{
			ubTargetElitePercent = 50 + (((ubBestLeadershipPercentage-80) * 50) / 20);
			ubTargetRegularPercent = 100 - ubTargetElitePercent;
			// Elites increase from 50% to 100% while regulars drop to 0%. No greens.
		}
		else // Best possible leadership = Full Elites
		{
			ubTargetElitePercent = 100;
			ubTargetRegularPercent = 0;
			// All elites.
		}
	}
	else
	// INI-defined percentages
	{
		ubTargetElitePercent = gGameExternalOptions.ubPercentRoamingMilitiaElites;
		ubTargetRegularPercent = gGameExternalOptions.ubPercentRoamingMilitiaRegulars;
	}

	ubTargetElite = (ubMilitiaToTrain * ubTargetElitePercent) / 100;
	if (ubTargetElitePercent) // If percentage is positive
		ubTargetElite = __max(1, ubTargetElite); // Create at least one.

	ubTargetRegular = (ubMilitiaToTrain * ubTargetRegularPercent) / 100;
	if (ubTargetRegularPercent) // If percentage is positive
		ubTargetRegular = __max(1, ubTargetRegular); // Create at least one.

	// Greens are created based on what's left of the MilitiaToTrain that has not already
	// been allocated to Regulars or Elites.
	ubTargetGreen = ubMilitiaToTrain - (ubTargetRegular + ubTargetElite);
	if (ubTargetRegularPercent+ubTargetElitePercent < 100) // If there's also a remainder, percentage-wise
		ubTargetGreen = __max(1, ubTargetGreen); // Create at least one.

	// Is Elite Militia allowed at all?
	if (!gGameExternalOptions.gfTrainVeteranMilitia ||
		GetWorldDay( ) < gGameExternalOptions.guiTrainVeteranMilitiaDelay)
	{
		// Convert all Elites to Regulars
		ubTargetRegular += ubTargetElite;
		ubTargetElite = 0;
	}

	// Make sure we're not training more militia than we're allowed. This kicks in when the number of militia
	// to train is lower than 3, for any reason (particularly, not meeting the minimum required leadership).
	while (ubTargetRegular+ubTargetElite+ubTargetGreen > ubMilitiaToTrain)
	{
		// Reduce target elite count first.
		if (ubTargetElite > 0)
		{
			ubTargetElite--;
			continue;
		}
		// If no more elites but still over the allowed value, then reduce one regular.
		if (ubTargetRegular > 0)
		{
			ubTargetRegular--;
			continue;
		}
		// No elites/regulars - reduce target green!
		ubTargetGreen--;
	}

	while (ubMilitiaToTrain > 0)
	{

		////////////////////////////////
		// Create Heterogenous Group
		//
		// As of HAM 3.4, homogenous groups are not treated any differently than these.
		////////////////////////////////		

		// Do we have room in the target sector?
		if ( usTotalMilitiaAtTarget < gGameExternalOptions.iMaxMilitiaPerSector )
		{
			if (ubTargetElite > 0)
			{
				// Add elite.
				StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1);
				ubTargetElite--;
				ubMilitiaToTrain--;
			}
			else if (ubTargetRegular > 0)
			{
				// Add regular.
				StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, 1);
				ubTargetRegular--;
				ubMilitiaToTrain--;
			}
			else if (ubTargetGreen > 0)
			{
				// Add green.
				StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, 1);
				ubTargetGreen--;
				ubMilitiaToTrain--;
			}
			else
			{
				// No one to add? Break the cycle.
				ubMilitiaToTrain = 0;
			}
		}

		// Full militia group? See if you can upgrade some.
		else
		{
			// Are we dealing with a full-size militia group that ISN'T full-quality yet? 
			if (pTargetSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] < usTotalMilitiaAtTarget &&
				pTargetSector->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] < gGameExternalOptions.iMaxMilitiaPerSector) // failsafe
			{
				// Have we got any Elites to add?
				if (ubTargetElite > 0)
				{
					// Add elite. This will effectively replace one lower class militia
					StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1);
					ubTargetElite--;
					ubMilitiaToTrain--;
				}
				else if (ubTargetRegular > 0 && // Got a regular
					(!gGameExternalOptions.gfTrainVeteranMilitia || // Not allowed to train Elites
					GetWorldDay( ) < gGameExternalOptions.guiTrainVeteranMilitiaDelay )) // Or not YET allowed to train elites
				{
					// Add a regular. This will effectively replace one Green militia
					StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1);
					ubTargetElite--;
					ubMilitiaToTrain--;	
				}
				// Else, we've got more men to train but no room for them. In this case, upgrade existing
				// militia to the next class, using our remainder men as "upgrade points".
				else
				{
					// Calculate upgrade points. 1 point per Green, 2 per Regular.
					ubUpgradePoints = ubTargetGreen + (2 * ubTargetRegular);

					// Is Elite training allowed?
					if ( gGameExternalOptions.gfTrainVeteranMilitia && // Elite training allowed
						(GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay) // Elite training allowed (time based)
						&& ubUpgradePoints >= 2) // Enough upgrade points to create an elite
					{
						// Add one elite
						StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, 1);
						if (ubTargetRegular > 0)
						{
							// Comes at a cost of one regular that we were supposed to train.
							ubTargetRegular--;
							ubMilitiaToTrain--;
						}
						else
						{
							// No regulars to train? Remove two greens instead.
							ubTargetGreen -= 2;
							ubMilitiaToTrain -= 2;
						}
					}
					// Else elite training was not allowed or we simply did not have enough points to add an elite
					else if (ubUpgradePoints >= 1)
					{
						// Add one regular instead of a green.
						StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, 1);
						ubTargetGreen--;
						ubMilitiaToTrain--;
					}
					else
					{
						// No more men to train. Break the cycle.
						ubMilitiaToTrain = 0;
					}
				}
			}
			else
			{
				// Sector is full-sized and full-quality. Break the cycle.
				ubMilitiaToTrain = 0;
			}
		}
	}

	// This reduces the group back to "maximum" size. It starts by eliminating extra greens, then regulars, then elites.
	// That produces a group of max size, with only the best troops remaining.
	while( CountMilitia(pTargetSector) > gGameExternalOptions.iMaxMilitiaPerSector )
	{
		if(pTargetSector->ubNumberOfCivsAtLevel[GREEN_MILITIA])
		{
			--pTargetSector->ubNumberOfCivsAtLevel[GREEN_MILITIA];
		}
		else if(pTargetSector->ubNumberOfCivsAtLevel[REGULAR_MILITIA])
		{
			--pTargetSector->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
		}
		else if(pTargetSector->ubNumberOfCivsAtLevel[ELITE_MILITIA])
		{
			--pTargetSector->ubNumberOfCivsAtLevel[ELITE_MILITIA];
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
	UINT8 ubChanceToSpreadOut;
	
	//////////////////////////////////////////////////////////
	// We begin by determining the group's chance to "spread out" into the next sector rather
	// than moving in en-masse. This is mainly affected by the presence of enemies.

	// Is the target a city or SAM site?
	if (IsThisSectorASAMSector( sTMapX, sTMapY, 0 ) ||
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

		if (NumEnemiesInFiveSectors( sMapX, sMapY ) == 0 &&
			NumEnemiesInFiveSectors( sTMapX, sTMapY ) > 0 )
		{
			// Target sector is threatened, while source sector is not.
			// Target sector gets all the militia it can, in preperation for a possible enemy attack.
			ubChanceToSpreadOut = 0;
		}
		else if (NumEnemiesInFiveSectors( sMapX, sMapY ) > 0 &&
				NumEnemiesInFiveSectors( sTMapX, sTMapY ) > 0 )
		{
			// Both sectors threatened. Militia will spread out between them, to maximize defensive capability.
			ubChanceToSpreadOut = 100;
		}	

		else if (NumEnemiesInFiveSectors( sMapX, sMapY ) > 0 &&
				NumEnemiesInFiveSectors( sTMapX, sTMapY ) == 0 )
		{
			// Source sector is threatened, while destination is not. The group does not
			// move at all, as it is already in a good position to defend.
			return;
		}
		else
		{
			///////////////////////////////////////////////////////
			// Both sectors not threatened. 
			// Alter spreadout chance based on number of troops in Source and destination sectors.

			if ( CountMilitia(pSectorInfo) >= gGameExternalOptions.iMaxMilitiaPerSector &&
				CountMilitia(pTSectorInfo) == 0 )
			{
				// Source group is full, target sector is empty. The source group gets 30% chance to 
				// spread out to that empty sector.
				ubChanceToSpreadOut = 30;
			}
			else if ( CountMilitia(pSectorInfo) && CountMilitia(pTSectorInfo) )
			{
				// Source and destination groups both exist. 50% chance to spread out evenly between them.
				ubChanceToSpreadOut = 50;
			}
			else
			{
				// Target sector is empty, source group is not full. Do not spread out.
				ubChanceToSpreadOut = 0;
			}
		}
	}

	// Should we spread out into the target sector, or move en-masse?
	if (PreRandom(100) < ubChanceToSpreadOut)
	{
		if ( CountMilitia(pSectorInfo) && CountMilitia(pTSectorInfo) )
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
			// Target sector is empty. Source group will split up into two equally-sized groups.
			bGreensSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ];
			bGreensDestTeam = bGreensSourceTeam / 2;

			bRegularsSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ];
			bRegularsDestTeam = bRegularsSourceTeam / 2;

			bElitesSourceTeam = pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ];
			bElitesDestTeam = bElitesSourceTeam / 2;

			// Add half team to target sector
			StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, bGreensDestTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, bRegularsDestTeam );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, bElitesDestTeam );
			
			// Remove half team from source sector
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, bGreensDestTeam );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, bRegularsDestTeam );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, bElitesDestTeam );
			
			// Units that move into an empty square do not get to move again.
			AddToBlockMoveList( sTMapX, sTMapY );
		}
	}
	else 
	{
		// Source team moves En-Masse to target sector.

		bNewDestGroupSize = __min((CountMilitia(pTSectorInfo) + CountMilitia(pSectorInfo)), (UINT8)gGameExternalOptions.iMaxMilitiaPerSector);
		bNewSourceGroupSize = __max(0,(CountMilitia(pTSectorInfo) + CountMilitia(pSectorInfo)) - gGameExternalOptions.iMaxMilitiaPerSector);

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
			// Entire group moves from Source to Target, leaving no one behind.			
			StrategicAddMilitiaToSector( sTMapX, sTMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicAddMilitiaToSector( sTMapX, sTMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
		
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, GREEN_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ GREEN_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, REGULAR_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] );
			StrategicRemoveMilitiaFromSector( sMapX, sMapY, ELITE_MILITIA, pSectorInfo->ubNumberOfCivsAtLevel[ ELITE_MILITIA ] );
				
			// Block target group from moving. It has just assimilated another group.
			AddToBlockMoveList( sTMapX, sTMapY );
		}
	}

	while( !fAlternativeMax && CountMilitia(pTSectorInfo) > gGameExternalOptions.iMaxMilitiaPerSector )
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
	UINT8 ubTargetEnemies = NumEnemiesInSector( sDMapX, sDMapY );

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

//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"M %ld, E%ld,%ld %ld, Diff %ld", CountAllMilitiaInSector( sMapX, sMapY ), sDMapX, sDMapY, NumEnemiesInSector( sDMapX, sDMapY ), ((INT32)CountAllMilitiaInSector( sMapX, sMapY )) - ((INT32)NumEnemiesInSector( sDMapX, sDMapY )) );

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
	if( !fForBattle && gfMSBattle && NumEnemiesInSector( sMapX, sMapY ) ) 
	{
		fReadyForBattle = FALSE;
	}
	else
	{
		fReadyForBattle = TRUE;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Check player-set restrictions. These are read from XML, and can be (but not always) influenced by
	// which cities we've already liberated, and which sectors we've already explored. This is handled
	// by a separate function.
	if( gGameExternalOptions.gflimitedRoaming && !IsSectorRoamingAllowed( SECTOR(sMapX, sMapY) ) )
	{
		// HEADROCK HAM 3.6: Training mobiles will also not take restrictions into account.
		if ( (fForBattle && fReadyForBattle) ||
			fForTraining )
		{
			// Allow movement into restricted sectors ONLY if they are for battle!
			fRoamingAllowed = TRUE;
		}
		else
		{
			fRoamingAllowed = FALSE;
		}
	}
	else
	{
		fRoamingAllowed = TRUE;
	}

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
			// Flags to store INI option settings.

			BOOLEAN fAllowReinforceCities = ( gGameExternalOptions.fAllowMobileReinforceCities && gGameExternalOptions.gfmusttrainroaming && gGameExternalOptions.gfAllowMilitiaGroups );
			BOOLEAN fAllowReinforceSAM = ( gGameExternalOptions.fAllowMobileReinforceSAM && gGameExternalOptions.gfmusttrainroaming && gGameExternalOptions.gfAllowMilitiaGroups );

			// Town->Town, Town->SAM, SAM->Town and SAM->SAM are disallowed for peaceful movement.
			if ( fSourceIsMajorTown || fSourceIsSamSite )
			{
				fCanBecomeGarrison = FALSE;
			}
			// Wilderness->Sam/Town.
			else
			{
				if (!NumEnemiesInSector( sMapX, sMapY ) && // Is the sector under our control at the moment?
					pTargetSector->fSurfaceWasEverPlayerControlled && // Has it ever been under control?
					CountMilitia(pTargetSector) < gGameExternalOptions.iMaxMilitiaPerSector ) // Is there room here for more militia?
				{
					if (fTargetIsSamSite && fAllowReinforceSAM)
					{
						fCanBecomeGarrison = TRUE;
					}
					else if (fTargetIsMajorTown && fAllowReinforceCities)
					{
						fCanBecomeGarrison = TRUE;
					}
				}
				else
				{
					fCanBecomeGarrison = FALSE;
				}
			}

			// RETURN.
			if ( fTraverseAllowed && fRoamingAllowed && fCanBecomeGarrison )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		// Trying to move peacefully out of a Town/SAM?
		else if (fSourceIsMajorTown || fSourceIsSamSite)
		{
			// This should ONLY be possible if we are explicitly training new militia groups.
			// Also, militia can no longer spawn directly into enemy-controlled territory.
			if (fForTraining && !(NumEnemiesInSector( sMapX, sMapY )))
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
			// Is allowed by INI?
			if (gGameExternalOptions.fAllowMilitiaMoveThroughMinorCities &&
				pTargetSector->fSurfaceWasEverPlayerControlled ) // Have we visited this sector before?
			{
				// Fine then. Works the same as wilderness->wilderness.
				fCanSimpleMove = TRUE;
			}
			else
			{
				// Movement through Minor Cities isn't allowed.
				fCanSimpleMove = FALSE;
			}
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
			GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, FALSE );

			if( uiDirNumber )
			{
				for( x = 1; x < uiDirNumber ; ++x )pMoveDir[x][1] += pMoveDir[x-1][1];
			//			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%ld,%ld", x, pMoveDir[x][1]);

				// HEADROCK HAM 3.6: Too many INI settings. Removed a couple, including MIN/MAX SQUAD SIZE.
//				iRandom = Random( pMoveDir[ uiDirNumber - 1 ][1] +
//					( uiMilitiaCount >= gGameExternalOptions.guiMinMilitiaSquadSize ? CHANCE_TO_MOVE_A_SQUAD : CHANCE_TO_MOVE_AN_UNFULL_SQUAD ) );
				iRandom = Random( pMoveDir[ uiDirNumber - 1 ][1] );


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

					// HEADROCK HAM 3.4: Moved the check to a separate function.
					if (gGameExternalOptions.gflimitedRoaming)
					{
						if (!IsSectorRoamingAllowed(pMoveDir[iRandomRes][0]))
							return;
					}

					// WDS bug fix for moving militia
					int targetX = SECTORX( pMoveDir[ iRandomRes ][0] );
					int targetY = SECTORY( pMoveDir[ iRandomRes ][0] );
					Assert(targetX >= 0 && targetX < MAP_WORLD_X);
					Assert(targetY >= 0 && targetY < MAP_WORLD_Y);
					MoveMilitiaSquad( sMapX, sMapY,  targetX, targetY, FALSE );

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
	// Variables for Direction Rating checks
	UINT16 pMoveDir[4][3];
	UINT8 uiDirNumber = 0;
	UINT32 iRandomRes = 0;
	UINT8 x;//,y;
	// Variables for checking other sectors in the same city
	UINT16 iCounter;
	UINT16 bTownId;
	// Tracks X/Y coordinates of another possible sector to train from (within same city)
	INT16 sCurrentX, sCurrentY;

	// Smart Militia Generator now locates the best sector for placement.
	UINT16 usBestLocalSectorID = 0;
	UINT16 usBestPerimeterSectorID = 0;
	UINT16 usBestLocalSectorRating = 0;
	UINT16 usBestPerimeterSectorRating = 0;
	// Selected Target coordinates
	UINT8 sTMapX = 0;
	UINT8 sTMapY = 0;

	// Leadership may affect quantity of militia generated.
	UINT8 ubBestLeadership = FindBestMilitiaTrainingLeadershipInSector ( sMapX, sMapY, 0, MOBILE_MILITIA );

	// If we're not allowing roaming groups, then we're not creating them either.
	if( !gGameExternalOptions.gfAllowMilitiaGroups )
		return;

	// Reset block move list. Does this cause issues with training in A1? Probably not.
	if( sMapX == 1 && sMapY == 1 )
	{
		ClearBlockMoveList();
		gfMSBattle = FALSE;
	}

	if( CheckInBlockMoveList( sMapX, sMapY ) )return;

	// Check all sectors adjacent to the training sector itself.
	GenerateDirectionInfosForTraining( sMapX, sMapY, &uiDirNumber, pMoveDir );

	if(uiDirNumber)// At least one available sector found, adjacent to Source Training Sector
	{
		for ( x = 0; x < uiDirNumber; x++)
		{
			// Best rating so far?
			if (pMoveDir[x][1] > usBestLocalSectorRating)
			{
				 // Mark this as the best "local" destination.
				 usBestLocalSectorID = pMoveDir[x][0];
				 usBestLocalSectorRating = pMoveDir[x][1];
			}
		}
	}

	// If we're training inside a city, check other city sectors for nearby available space. Maybe you'll find
	// a better place to put new mobiles?
	bTownId = GetTownIdForSector( sMapX, sMapY );
	if( bTownId != BLANK_SECTOR )
	{
		// We're training in a city.
		iCounter = 0;

		// Go through each city in the game
		while( pTownNamesList[ iCounter ] != 0 )
		{
			// Are we in this city?
			if( pTownNamesList[ iCounter] == bTownId )
			{
				sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
				sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

				// if sector has enemies or hasn't already been taken at least once, then
				if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || 
					NumEnemiesInSector( sCurrentX, sCurrentY ) > 0 )
				{
					// skip the rest. Generate no militia from this sector. 
					iCounter++;
					continue;
				}

				// Find out if any adjacent sectors have room in them.
				GenerateDirectionInfosForTraining( sCurrentX, sCurrentY, &uiDirNumber, pMoveDir );

				if(uiDirNumber)// At least one available adjacent sector has been found
				{
					// Go through each adjacent sector
					for( x = 0; x < uiDirNumber ; x++ )
					{
						// Have we found the best rating so far?
						if ( pMoveDir[x][1] > usBestPerimeterSectorRating )
						{
							usBestPerimeterSectorID = pMoveDir[x][0];
							usBestPerimeterSectorRating = pMoveDir[x][1];
						}
					}
				}
			}
			iCounter++;
		}
	}

	// Randomly choose whether to place locally or somewhere else around the city perimeter. 
	// Local and Perimeter have separate "weights" in this randomization, but you're much more
	// likely to place militia near the original source sector. That is because it has increased
	// chance of being selected both as the best local and as the best overall, and so might be
	// selected as the result of either random choice.

	// Make sure at least one sector was found with more than 0 rating
	if (usBestLocalSectorRating > 0 || usBestPerimeterSectorRating > 0)
	{
		// Found at least one location.
		iRandomRes = PreRandom( usBestLocalSectorRating + usBestPerimeterSectorRating );
		// Either go with randomization, or automatically select a Local sector if there is no better Perimeter sector.
		if (iRandomRes < usBestLocalSectorRating ||
			usBestLocalSectorRating >= usBestPerimeterSectorRating)
		{
			sTMapX = SECTORX( usBestLocalSectorID );
			sTMapY = SECTORY( usBestLocalSectorID );
	
				// Generate militia in the best sector immediately adjacent to the Source Training Sector.
			GenerateMilitiaSquad( sMapX, sMapY,	sTMapX, sTMapY, ubBestLeadership );
			AddToBlockMoveList( sTMapX, sTMapY );
	
			// Check for enemies in target sector
			if(	NumEnemiesInSector( sTMapX, sTMapY ) )
				{
				// Initiave battle
				gfMSBattle = TRUE;
				EnterAutoResolveMode( sTMapX, sTMapY );
			}
		}
		else
		{
			sTMapX = SECTORX( usBestPerimeterSectorID );
			sTMapY = SECTORY( usBestPerimeterSectorID );

			// Generate militia in the best sector found in this city's perimeter.
			GenerateMilitiaSquad( sCurrentX, sCurrentY,	sTMapX, sTMapY, ubBestLeadership );
			AddToBlockMoveList( sTMapX, sTMapY );

			// Check for enemies in target sector
			if( NumEnemiesInSector( sTMapX, sTMapY ) )
			{
				// Initiave battle
				gfMSBattle = TRUE;
				EnterAutoResolveMode( sTMapX, sTMapY );
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

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, TRUE );

	ZeroMemory( gpAttackDirs, sizeof( gpAttackDirs ) );

	gpAttackDirs[ guiDirNumber ][0] = uiNumGreen = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
	gpAttackDirs[ guiDirNumber ][1] = uiNumReg = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
	gpAttackDirs[ guiDirNumber ][2] = uiNumElite = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
	gpAttackDirs[ guiDirNumber ][3] = INSERTION_CODE_CENTER;

	guiDirNumber = uiDirNumber	+ 1;

	x = 0;
	while( CountMilitia(pSectorInfo ) < gGameExternalOptions.iMaxMilitiaPerSector &&
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

	GenerateDirectionInfos( sMapX, sMapY, &uiDirNumber, pMoveDir, FALSE, TRUE );

	for( x = 0; x < uiDirNumber ; ++x )
		if( CountAllMilitiaInSector( SECTORX( pMoveDir[ x ][ 0 ] ), SECTORY( pMoveDir[ x ][ 0 ] ) ) )
			fResult = TRUE;

	return fResult;
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

// HEADROCK HAM 3.4: This hardcoded function has been made obsolete by XML externalization.
/*void AdjustRoamingRestrictions()
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

		// HEADROCK HAM 3.3: Drassen SAM-Site (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(15,4)] = true;

		// HEADROCK HAM 3.3: Drassen ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(13,2)] = true;
		gDynamicRestrictMilitia[SECTOR(13,3)] = true;
		gDynamicRestrictMilitia[SECTOR(13,4)] = true;

		// HEADROCK HAM 3.3: Forgot Omerta!
		gDynamicRestrictMilitia[SECTOR(9,1)] = true;
		gDynamicRestrictMilitia[SECTOR(10,1)] = true;
	
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

		// HEADROCK HAM 3.3: Alma ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(13,8)] = true;
		gDynamicRestrictMilitia[SECTOR(14,8)] = true;
		gDynamicRestrictMilitia[SECTOR(13,9)] = true;
		gDynamicRestrictMilitia[SECTOR(14,9)] = true;
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

		// HEADROCK HAM 3.3: Chitzena SAM-Site (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(2,4)] = true;

		// HEADROCK HAM 3.3: Chitzena ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(2,1)] = true;
		gDynamicRestrictMilitia[SECTOR(2,2)] = true;
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

		// HEADROCK HAM 3.3: Cambria SAM-Site (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(8,9)] = true;

		// HEADROCK HAM 3.3: Cambria ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(8,6)] = true;
		gDynamicRestrictMilitia[SECTOR(9,6)] = true;
		gDynamicRestrictMilitia[SECTOR(8,7)] = true;
		gDynamicRestrictMilitia[SECTOR(9,7)] = true;
		gDynamicRestrictMilitia[SECTOR(8,8)] = true;

		// HEADROCK HAM 3.3: Forgot Omerta!
		gDynamicRestrictMilitia[SECTOR(9,1)] = true;
		gDynamicRestrictMilitia[SECTOR(10,1)] = true;
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

		// HEADROCK HAM 3.3: Balime ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(11,12)] = true;
		gDynamicRestrictMilitia[SECTOR(12,12)] = true;
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

		// HEADROCK HAM 3.3: Grumm ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(1,7)] = true;
		gDynamicRestrictMilitia[SECTOR(2,7)] = true;
		gDynamicRestrictMilitia[SECTOR(1,8)] = true;
		gDynamicRestrictMilitia[SECTOR(2,8)] = true;
		gDynamicRestrictMilitia[SECTOR(3,8)] = true;
	}

	if ( IsTownUnderCompleteControlByPlayer(MEDUNA) )
	{
		// HEADROCK HAM 3.3: Meduna ITSELF (for reinforcement purposes)
		gDynamicRestrictMilitia[SECTOR(3,14)] = true;
		gDynamicRestrictMilitia[SECTOR(4,14)] = true;
		gDynamicRestrictMilitia[SECTOR(5,14)] = true;
		gDynamicRestrictMilitia[SECTOR(3,15)] = true;
		gDynamicRestrictMilitia[SECTOR(4,15)] = true;
		gDynamicRestrictMilitia[SECTOR(3,16)] = true;
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
}*/

void AdjustRoamingRestrictions()
{
	UINT32 uiCapturedTownsFlag = 0;
	UINT16 cnt = 0;

	if (!gGameExternalOptions.fDynamicRestrictRoaming)
		return;

	// to do: Add something to clean up the entire array before setting the flags...

	for (cnt=1; cnt < NUM_TOWNS; cnt++)
	{
		if ( IsTownUnderCompleteControlByPlayer((UINT8)cnt) )
		{
			// Flag town as captured.
			uiCapturedTownsFlag |= (1 << (cnt-1));
		}
	}

	for (cnt=0; cnt < 256; cnt++)
	{
		gDynamicRestrictMilitia[cnt] = false;
	}

	for (cnt=0; (cnt < 5001 && gDynamicRestrictions[cnt].bSectorID >= 0); cnt++)
	{
		if (!(~uiCapturedTownsFlag & gDynamicRestrictions[cnt].uiReqTownFlags))
		{
			gDynamicRestrictMilitia[gDynamicRestrictions[cnt].bSectorID] = true;
		}
	}
}


// HEADROCK HAM B2.7: This is a copy of an existing function that generates possible movement directions for militia.
// Due to the widespread use of that function, I've decided to make a new one that calculates directions
// when generating militia. This function checks for available space at the target location. The stored results
// indicate the amount of space left in the target sector.
// HEADROCK HAM 3.6: Revamped to fix bugs.

void GenerateDirectionInfosForTraining( INT16 sMapX, INT16 sMapY, UINT8* uiDirNumber, UINT16 pMoveDir[4][3] )
{
	SECTORINFO *pSectorInfo;
	UINT8 ubMaxMilitia = gGameExternalOptions.iMaxMilitiaPerSector;

	*uiDirNumber = 0;
	UINT16 usMilitiaCapacity = 0;
	UINT16 usUpgradeCapacity = 0;

	// Clean up pMoveDir
	for (UINT8 x = 0; x < 4; x++)
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
		pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY - 1 ) ] );

		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY - 1, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - CountMilitia( pSectorInfo ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( pSectorInfo );

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
		pSectorInfo = &( SectorInfo[ SECTOR( sMapX - 1, sMapY ) ] );

		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX - 1, sMapY, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - CountMilitia( pSectorInfo ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( pSectorInfo );

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
		pSectorInfo = &( SectorInfo[ SECTOR( sMapX , sMapY + 1 ) ] );

		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX, sMapY + 1, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - CountMilitia( pSectorInfo ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( pSectorInfo );

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
		pSectorInfo = &( SectorInfo[ SECTOR( sMapX + 1 , sMapY ) ] );

		// Are militia allowed to travel in that direction from the current sector, for training purposes?
		if ( CheckStandardConditionsForDirection( sMapX, sMapY, sMapX +1 , sMapY, TRUE, FALSE ) )
		{
			// Count free slots. Each slot is worth 3 points.
			usMilitiaCapacity = 3 * (ubMaxMilitia - CountMilitia( pSectorInfo ));
			// Count militia that can be upgraded. Regular = 1 point, Green = 2 points.
			usUpgradeCapacity = MilitiaUpgradeSlotsCheck( pSectorInfo );

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

// HEADROCK HAM 3.4: Something that should've been done a long time ago - moving the check of roaming restrictions
// to a separate function. Returns TRUE if the sector can be traversed by militia.
BOOLEAN IsSectorRoamingAllowed( UINT32 uiSector )
{

	// EXPLORATION-BASED RESTRICTION
	// Is militia allowed to move through visited sectors?
	if (gGameExternalOptions.fUnrestrictVisited)
	{
		// Has the sector ever been visited?
		if ( SectorInfo[ uiSector ].fSurfaceWasEverPlayerControlled )
		{
			// Always return TRUE.
			return TRUE;
		}
	}
	// Please note that with exploration restrictions, an UNVISITED sector is not neccesarily RESTRICTED.

	// XML-BASED RESTRICTION
	// Is destination allowed by the Dynamic Restrictions Array?
	if( !gDynamicRestrictMilitia[ uiSector ] )
	{
		// Don't restrict movement if there are player mercs in the target sector! Otherwise, the player
		// cannot have militia follow him to attack the enemy.
		if (!PlayerMercsInSector_MSE( SECTORX(uiSector), SECTORY(uiSector), FALSE ))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

// HEADROCK HAM 3.6: Function returns the number of "upgrade slots" available at target sector. Basically, every GREEN
// Militia at the target sector counts as 2 upgrade slots, while every regular counts as 1.

UINT16 MilitiaUpgradeSlotsCheck( SECTORINFO * pSectorInfo )
{
	// Result variables
	UINT16 usNumUpgradeSlots = 0;

	// Target sector full of elites?	
	if (pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] >= gGameExternalOptions.iMaxMilitiaPerSector)
	{
		// Militia cannot be placed or moved here at all.
		return 0;
	}

	// If not allowed to train elites ( or not yet )
	if (!gGameExternalOptions.gfTrainVeteranMilitia || // Not allowed at all
		(GetWorldDay( ) < gGameExternalOptions.guiTrainVeteranMilitiaDelay)) // Not yet allowed.
	{
		// If sector full of Regulars and Elites?
		if (pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]+pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] >= gGameExternalOptions.iMaxMilitiaPerSector) // Target sector full of regular+elite militia
		{
			// Can't upgrade Regulars to Elites, so the sector is already full.
			return 0;
		}

		// Each Green we can convert to Regular counts as 2 slots.
		usNumUpgradeSlots = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA] * 2;

		return (usNumUpgradeSlots);
	}

	// Allowed to train Green->Regular, and Regular->Elite
	usNumUpgradeSlots = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA] * 2;
	usNumUpgradeSlots += pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];

	return (usNumUpgradeSlots);
}

