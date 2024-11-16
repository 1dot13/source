#include "builddefines.h"

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
	#include "PreBattle Interface.h"
	#include "Map Screen Interface Border.h"
	#include "interface control.h"
	#include "Map Screen Interface Map.h"
	#include "laptop.h"							// added by Flugente
	#include "Game Event Hook.h"				// added by Flugente
	#include "MilitiaIndividual.h"				// added by Flugente
	#include "Campaign.h"						// added by Flugente
	#include "message.h"						// added by Flugente
	#include "Rebel Command.h"

// HEADROCK HAM 3: include these files so that a militia trainer's Effective Leadership can be determined. Used
// to determine the number of militia trained by this merc per session. In the future may also determine QUALITY
// of trained troops.
#include "SkillCheck.h"
#include "Soldier Control.h"
#include "soldier profile type.h"

#include "MilitiaSquads.h"
#define SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST 50

// temporary local global variables
UINT8 gubTownSectorServerTownId = BLANK_SECTOR;
INT16 gsTownSectorServerSkipX = -1;
INT16	gsTownSectorServerSkipY = -1;
UINT8 gubTownSectorServerIndex = 0;
BOOLEAN gfYesNoPromptIsForContinue = FALSE;		// this flag remembers whether we're starting new training, or continuing
INT32 giTotalCostOfTraining = 0;
BOOLEAN gfAreWePromotingGreen = FALSE;
BOOLEAN gfAreWePromotingRegular = FALSE;


//the completed list of sector soldiers for training militia
INT32 giListOfMercsInSectorsCompletedMilitiaTraining[ SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST ];
SOLDIERTYPE *pMilitiaTrainerSoldier = NULL;

// note that these sector values are STRATEGIC INDEXES, not 0-255!
INT16 gsUnpaidStrategicSector[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];

// the selected list of mercs
extern BOOLEAN fSelectedListOfMercsForMapScreen[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];

// towns with militia training allowed
BOOLEAN gfMilitiaAllowedInTown[MAX_TOWNS] =
{
	0,	// blank sector
	0,	// omerta
	1,	// drassen
	1,	// alma
	1,	// grumm
	0,	// tixa
	1,	// cambria
	0,	// san mona
	0,	// estoni
	0,	// orta
	1,	// balime
	1,	// meduna
	1,	// chitzena
};

// private prototypes
void PayMilitiaTrainingYesNoBoxCallback( UINT8 bExitValue );
void CantTrainMilitiaOkBoxCallback( UINT8 bExitValue );

void PayForTrainingInSector( UINT8 ubSector );

void InitFriendlyTownSectorServer(UINT8 ubTownId, INT16 sSkipSectorX, INT16 sSkipSectorY);
BOOLEAN ServeNextFriendlySectorInTown( INT16 *sNeighbourX, INT16 *sNeighbourY );

// HEADROCK HAM 3.6: Instead of adding more functions, I've added an argument that determines whether we're checking
// Garrison Militia payment versus Mobile Militie payment.
void BuildListOfUnpaidTrainableSectors( UINT8 ubMilitiaType );
INT32 GetNumberOfUnpaidTrainableSectors( UINT8 ubMilitiaType );
void ContinueTrainingInThisSector( UINT8 ubMilitiaType );
void StartTrainingInAllUnpaidTrainableSectors( UINT8 ubMilitiaType );
void ResetDoneFlagForAllMilitiaTrainersInSector( UINT8 ubSector, UINT8 ubMilitiaType );
void MilitiaTrainingRejected( UINT8 ubMilitiaType );

// HEADROCK HAM 3.6: Total upkeep costs for keeping militia.
UINT32 guiTotalUpkeepForMilitia = 0;

// HEADROCK HAM 3.6: Qsort for sectors with militia in them.
int MilitiaListQsortCompare(const void *pArg1, const void *pArg2);

#ifdef JA2BETAVERSION
void VerifyTownTrainingIsPaidFor( void );
#endif

// handle promoting a militia during militia training. return TRUE if militia could be promoted
BOOLEAN TownMilitiaTrainingPromotion( INT16 sMapX, INT16 sMapY, UINT8& arusPromotedTo )
{
	INT16 sNeighbourX, sNeighbourY;
	UINT8 ubTownId = StrategicMap[CALCULATE_STRATEGIC_INDEX( sMapX, sMapY )].bNameId;

	// alrighty, then.	We'll have to *promote* guys instead.

	// Flugente: check whether we have the resources to promote militia
	BOOLEAN fCanPromoteToRegular = TRUE;
	BOOLEAN fCanPromoteToElite = TRUE;
	if ( gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory )
	{
		FLOAT val_gun, val_armour, val_misc;
		GetResources( val_gun, val_armour, val_misc );

		// regular militia require an additional gun and armour
		if ( val_gun <= 1.0f || val_armour <= 1.0f )
		{
			fCanPromoteToRegular = FALSE;
			fCanPromoteToElite = FALSE;
		}
		// elite also require misc resources
		else if ( val_misc <= 1.0f )
		{
			fCanPromoteToElite = FALSE;
		}
	}

	// are there any GREEN militia men in the training sector itself?
	if ( fCanPromoteToRegular && MilitiaInSectorOfRank( sMapX, sMapY, GREEN_MILITIA ) > 0 )
	{
		// great! Promote a GREEN militia guy in the training sector to a REGULAR
		StrategicPromoteMilitiaInSector( sMapX, sMapY, GREEN_MILITIA, 1 );

		PromoteIndividualMilitia( SECTOR( sMapX, sMapY ), SOLDIER_CLASS_GREEN_MILITIA );

		if ( sMapX == gWorldSectorX && sMapY == gWorldSectorY )
			gfStrategicMilitiaChangesMade = TRUE;

		arusPromotedTo = SOLDIER_CLASS_REG_MILITIA;

		return TRUE;
	}
	else
	{
		if ( fCanPromoteToRegular && ubTownId != BLANK_SECTOR )
		{
			// dammit! Last chance - try to find other eligible sectors in the same town with a Green guy to be promoted
			InitFriendlyTownSectorServer( ubTownId, sMapX, sMapY );

			// check other eligible sectors in this town for room for another militia
			while ( ServeNextFriendlySectorInTown( &sNeighbourX, &sNeighbourY ) )
			{
				// are there any GREEN militia men in the neighbouring sector ?
				if ( MilitiaInSectorOfRank( sNeighbourX, sNeighbourY, GREEN_MILITIA ) > 0 )
				{
					// great! Promote a GREEN militia guy in the neighbouring sector to a REGULAR
					StrategicPromoteMilitiaInSector( sNeighbourX, sNeighbourY, GREEN_MILITIA, 1 );

					PromoteIndividualMilitia( SECTOR( sNeighbourX, sNeighbourY ), SOLDIER_CLASS_GREEN_MILITIA );

					if ( sNeighbourX == gWorldSectorX && sNeighbourY == gWorldSectorY )
						gfStrategicMilitiaChangesMade = TRUE;

					arusPromotedTo = SOLDIER_CLASS_REG_MILITIA;

					return TRUE;
				}
			}
		}

		// Kaiden: Veteran militia training
		// This is essentially copy/pasted from above
		// But the names have been changed to protect the innocent
		if ( fCanPromoteToElite && gGameExternalOptions.gfTrainVeteranMilitia && (GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay) )
		{
			// are there any REGULAR militia men in the training sector itself?
			if ( MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA ) > 0 )
			{
				// great! Promote a REGULAR militia guy in the training sector to a VETERAN
				StrategicPromoteMilitiaInSector( sMapX, sMapY, REGULAR_MILITIA, 1 );

				PromoteIndividualMilitia( SECTOR( sMapX, sMapY ), SOLDIER_CLASS_REG_MILITIA );

				if ( sMapX == gWorldSectorX && sMapY == gWorldSectorY )
					gfStrategicMilitiaChangesMade = TRUE;

				arusPromotedTo = SOLDIER_CLASS_ELITE_MILITIA;

				return TRUE;
			}
			else
			{
				if ( ubTownId != BLANK_SECTOR )
				{
					// dammit! Last chance - try to find other eligible sectors in the same town with a Regular guy to be promoted
					InitFriendlyTownSectorServer( ubTownId, sMapX, sMapY );

					// check other eligible sectors in this town for room for another militia
					while ( ServeNextFriendlySectorInTown( &sNeighbourX, &sNeighbourY ) )
					{
						// are there any REGULAR militia men in the neighbouring sector ?
						if ( MilitiaInSectorOfRank( sNeighbourX, sNeighbourY, REGULAR_MILITIA ) > 0 )
						{
							// great! Promote a Regular militia guy in the neighbouring sector to a Veteran
							StrategicPromoteMilitiaInSector( sNeighbourX, sNeighbourY, REGULAR_MILITIA, 1 );

							PromoteIndividualMilitia( SECTOR( sNeighbourX, sNeighbourY ), SOLDIER_CLASS_REG_MILITIA );

							if ( sNeighbourX == gWorldSectorX && sNeighbourY == gWorldSectorY )
								gfStrategicMilitiaChangesMade = TRUE;

							arusPromotedTo = SOLDIER_CLASS_ELITE_MILITIA;

							return TRUE;
						}
					}
				}
			}
		}
	}

	// if we still haven't been able to train anyone
	// Well, that's it.	All eligible sectors of this town are full of REGULARs or ELITEs.
	// The training goes to waste in this situation.
	return FALSE;
}

void TownMilitiaTrainingCompleted( SOLDIERTYPE *pTrainer, INT16 sMapX, INT16 sMapY )
{
	UINT8 ubMilitiaTrained = 0;
	BOOLEAN fFoundOne;
	INT16 sNeighbourX, sNeighbourY;
	UINT8 ubTownId;

	INT32 iMaxMilitiaPerSector = gGameExternalOptions.iMaxMilitiaPerSector;

	// HEADROCK HAM 3.6: Leadership may affect the resulting squad size.
	UINT8 ubTrainerEffectiveLeadership = FindBestMilitiaTrainingLeadershipInSector ( sMapX, sMapY, pTrainer->bSectorZ, TOWN_MILITIA );
	UINT8 iTrainingSquadSize = __min(iMaxMilitiaPerSector, CalcNumMilitiaTrained(ubTrainerEffectiveLeadership));
	UINT8 promotionstodo = 0;

	// Flugente: our pool of volunteers limits how many militia can be created
	// if we can't train as many militia as we should due to lack of volunteers, the excess training goes into promoting militia
	UINT8 promotionsfromvolunteers = iTrainingSquadSize;
	iTrainingSquadSize = min( iTrainingSquadSize, GetVolunteerPool( ) );
	
	if ( gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory )
	{
		FLOAT val_gun, val_armour, val_misc;
		GetResources( val_gun, val_armour, val_misc );

		iTrainingSquadSize = min( iTrainingSquadSize, (INT32)val_gun );
	}

	promotionsfromvolunteers -= iTrainingSquadSize;

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia1");

	// get town index
	ubTownId = StrategicMap[CALCULATE_STRATEGIC_INDEX( sMapX, sMapY )].bNameId;

	if( ubTownId == BLANK_SECTOR )
	{
		Assert( IsThisSectorASAMSector( sMapX, sMapY, 0 ) || RebelCommand::CanTrainMilitiaAnywhere() );
	}

	// force tactical to update militia status
	gfStrategicMilitiaChangesMade = FALSE;

	// ok, so what do we do with all this training?	Well, in order of decreasing priority:
	// 1) If there's room in training sector, create new GREEN militia guys there
	// 2) If not enough room there, create new GREEN militia guys in friendly sectors of the same town
	// 3) If not enough room anywhere in town, promote a number of GREENs in this sector into regulars
	// 4) If not enough GREENS there to promote, promote GREENs in other sectors.
	// 5) If all friendly sectors of this town are completely filled with REGULAR militia, then training effect is wasted
	
	// Kaiden: Roaming Militia Training:
	// If we're not training roaming militia,
	// then we will handle everything as normal.
	
	{
		// we either create new militia or promote existing ones if no space is left
		while ( ubMilitiaTrained + promotionstodo < iTrainingSquadSize )
		{
			// is there room for another militia in the training sector itself?
			if ( NumNonPlayerTeamMembersInSector( sMapX, sMapY, MILITIA_TEAM ) < iMaxMilitiaPerSector )
			{
				// great! Create a new GREEN militia guy in the training sector
				StrategicAddMilitiaToSector(sMapX, sMapY, GREEN_MILITIA, 1);

				if (sMapX == gWorldSectorX && sMapY == gWorldSectorY)
					gfStrategicMilitiaChangesMade = TRUE;

				// next, please!
				++ubMilitiaTrained;

				CreateNewIndividualMilitia( GREEN_MILITIA, MO_ARULCO, SECTOR( sMapX, sMapY ) );
			}
			else
			{
				fFoundOne = FALSE;

				if( ubTownId != BLANK_SECTOR )
				{
					InitFriendlyTownSectorServer(ubTownId, sMapX, sMapY);

					// check other eligible sectors in this town for room for another militia
					while( ServeNextFriendlySectorInTown( &sNeighbourX, &sNeighbourY ) )
					{
						// is there room for another militia in this neighbouring sector ?
						if ( NumNonPlayerTeamMembersInSector( sNeighbourX, sNeighbourY, MILITIA_TEAM ) < iMaxMilitiaPerSector )
						{
							// great! Create a new GREEN militia guy in the neighbouring sector
							StrategicAddMilitiaToSector(sNeighbourX, sNeighbourY, GREEN_MILITIA, 1);

							if (sNeighbourX == gWorldSectorX && sNeighbourY == gWorldSectorY)
								gfStrategicMilitiaChangesMade = TRUE;

							// next, please!
							++ubMilitiaTrained;

							CreateNewIndividualMilitia( GREEN_MILITIA, MO_ARULCO, SECTOR( sNeighbourX, sNeighbourY ) );

							fFoundOne = TRUE;
							break;
						}
					}
				}

				// if we still haven't been able to train anyone
				if (!fFoundOne)
				{
					// we need to promote militia then
					++promotionstodo;

					fFoundOne = TRUE;
				}
			}
		}

		// we have to remove the resources for trained militia before we do promotions, in order to remove these resource
		if ( gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory )
		{
			// use up resources for new militia
			AddResources( -ubMilitiaTrained, 0, 0 );
		}

		// handle promotions
		UINT8 promotions = 0;
		UINT8 promotedto = 0;
		UINT8 promotedto_regular = 0;
		UINT8 promotedto_elite = 0;
		while ( promotions < promotionstodo + promotionsfromvolunteers && TownMilitiaTrainingPromotion( sMapX, sMapY, promotedto ) )
		{
			++promotions;

			if ( promotedto == SOLDIER_CLASS_REG_MILITIA )
				++promotedto_regular;
			else
				++promotedto_elite;
		}

		if (gfStrategicMilitiaChangesMade)
		{
			ResetMilitia();
		}

		// if anyone actually got trained
		if (ubMilitiaTrained > 0)
		{
			AddVolunteers( -ubMilitiaTrained );

			// update the screen display
			fMapPanelDirty = TRUE;

			if( ubTownId != BLANK_SECTOR )
			{
				// HEADROCK HAM 3.6: Adjust loyalty based on how many militia were trained, compared to
				// how many can be trained (max).
				UINT32 uiTownLoyaltyBonus = (LOYALTY_BONUS_FOR_TOWN_TRAINING * ubMilitiaTrained) / gGameExternalOptions.iTrainingSquadSize;
				// loyalty in this town increases a bit because we obviously care about them...
				IncrementTownLoyalty( ubTownId, uiTownLoyaltyBonus );
			}

			// SANDRO - merc records (num militia trained)
			RecordNumMilitiaTrainedForMercs( sMapX, sMapY, pTrainer->bSectorZ, ubMilitiaTrained );
		}

		if ( gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory )
		{
			// promotion resources
			AddResources( - promotedto_regular - promotedto_elite, - promotedto_regular - promotedto_elite, -promotedto_elite );
		}

		// the trainer announces to player that he's finished his assignment.	Make his sector flash!
		AssignmentDone( pTrainer, TRUE, FALSE );

		// handle completion of town by training group
		HandleCompletionOfTownTrainingByGroupWithTrainer( pTrainer, TOWN_MILITIA );
	}

	// Flugente: if we trained militia, the PMC notices us and offers their services
	if (gGameExternalOptions.fPMC && !IsBookMarkSet(PMC_BOOKMARK) && !(gStrategicStatus.usEnricoEmailFlags & ENRICO_EMAIL_SENT_PMC))
	{	
		AddStrategicEvent(EVENT_PMC_EMAIL, GetWorldTotalMin() + 60 * (1 + Random(6)), 0);
		gStrategicStatus.usEnricoEmailFlags |= ENRICO_EMAIL_SENT_PMC;
	}

	if (gGameExternalOptions.fIndividualMilitia && !IsBookMarkSet(MILITIAROSTER_BOOKMARK) && !(gStrategicStatus.usEnricoEmailFlags & ENRICO_EMAIL_SENT_MILITIAROSTER))
	{
		AddStrategicEvent(EVENT_MILITIAROSTER_EMAIL, GetWorldTotalMin() + 60 * (1 + Random(4)), 0);
		gStrategicStatus.usEnricoEmailFlags |= ENRICO_EMAIL_SENT_MILITIAROSTER;
	}
}

// feed this a SOLDIER_CLASS_, it will return you a _MITILIA rank, or -1 if the guy's not militia
INT8 SoldierClassToMilitiaRank(UINT8 ubSoldierClass)
{
	INT8 bRank = -1;

	switch( ubSoldierClass )
	{
		case SOLDIER_CLASS_ADMINISTRATOR:
		case SOLDIER_CLASS_GREEN_MILITIA:
			bRank = GREEN_MILITIA;
			break;
		case SOLDIER_CLASS_ARMY:
		case SOLDIER_CLASS_REG_MILITIA:
			bRank = REGULAR_MILITIA;
			break;
		case SOLDIER_CLASS_ELITE:
		case SOLDIER_CLASS_ELITE_MILITIA:
			bRank = ELITE_MILITIA;
			break;
	}

	return(bRank);
}

// feed this a _MITILIA rank, it will return you a SOLDIER_CLASS_, or -1 if the guy's not militia
INT8 MilitiaRankToSoldierClass(UINT8 ubRank)
{
	INT8 bSoldierClass = -1;

	switch( ubRank )
	{
		case GREEN_MILITIA:
			bSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
			break;
		case REGULAR_MILITIA:
			bSoldierClass = SOLDIER_CLASS_REG_MILITIA;
			break;
		case ELITE_MILITIA:
			bSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
			break;
	}

	return(bSoldierClass);
}

void StrategicAddMilitiaToSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT16 ubHowMany)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	pSectorInfo->ubNumberOfCivsAtLevel[ ubRank ] += ubHowMany;

	if (ubHowMany && sMapX == gWorldSectorX && sMapY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

	// update the screen display
	fMapPanelDirty = TRUE;
}

void StrategicPromoteMilitiaInSector(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank, UINT16 ubHowMany)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	// damn well better have that many around to promote!
	//Assert(pSectorInfo->ubNumberOfCivsAtLevel[ ubCurrentRank ] >= ubHowMany);

	UINT16 stationary = MilitiaInSectorOfRankStationary( sMapX, sMapY, ubCurrentRank );
	UINT16 ingroups = MilitiaInSectorOfRankInGroups( sMapX, sMapY, ubCurrentRank );

	//KM : July 21, 1999 patch fix
	if ( ubCurrentRank >= ELITE_MILITIA || stationary + ingroups < ubHowMany )
	{
		return;
	}

	// determine how many static and  - if necessary - group-based militia we have to remove
	UINT16 reducestatic = min( stationary, ubHowMany );
	UINT16 reducegroups = min( ingroups, ubHowMany - reducestatic );
	
	pSectorInfo->ubNumberOfCivsAtLevel[ubCurrentRank] -= reducestatic;
	pSectorInfo->ubNumberOfCivsAtLevel[ubCurrentRank + 1] += reducestatic;

	GROUP *pGroup = gpGroupList;
	while ( pGroup && reducegroups )
	{
		if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == sMapX && pGroup->ubSectorY == sMapY )
		{
			if ( ubCurrentRank == GREEN_MILITIA )
			{
				UINT8 reduced = min( reducegroups, pGroup->pEnemyGroup->ubNumAdmins );
				pGroup->pEnemyGroup->ubNumAdmins -= reduced;
				pGroup->pEnemyGroup->ubNumTroops += reduced;
				reducegroups -= reduced;
			}
			else if ( ubCurrentRank == REGULAR_MILITIA )
			{
				UINT8 reduced = min( reducegroups, pGroup->pEnemyGroup->ubNumTroops );
				pGroup->pEnemyGroup->ubNumTroops -= reduced;
				pGroup->pEnemyGroup->ubNumElites += reduced;
				reducegroups -= reduced;
			}
		}
		pGroup = pGroup->next;
	}
	
	if (ubHowMany && sMapX == gWorldSectorX && sMapY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

	// update the screen display
	fMapPanelDirty = TRUE;
}

void StrategicRemoveMilitiaFromSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT16 ubHowMany)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	// WANNE: BUGZILLA #357: Fixed assertion when miltia dies.
	// damn well better have that many around to remove!
	//Assert(pSectorInfo->ubNumberOfCivsAtLevel[ ubRank ] >= ubHowMany);

	UINT16 stationary = MilitiaInSectorOfRankStationary( sMapX, sMapY, ubRank );
	UINT16 ingroups = MilitiaInSectorOfRankInGroups( sMapX, sMapY, ubRank );

	//KM : July 21, 1999 patch fix
	if ( stationary + ingroups < ubHowMany )
	{
		return;
	}

	// determine how many static and  - if necessary - group-based militia we have to remove
	UINT16 reducestatic = min( stationary, ubHowMany );
	UINT16 reducegroups = min( ingroups, ubHowMany - reducestatic );

	pSectorInfo->ubNumberOfCivsAtLevel[ubRank] -= reducestatic;

	GROUP *pGroup = gpGroupList;
	while ( pGroup && reducegroups )
	{
		if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == sMapX && pGroup->ubSectorY == sMapY )
		{
			if ( ubRank == GREEN_MILITIA )
			{
				UINT16 reduced = min( reducegroups, pGroup->pEnemyGroup->ubNumAdmins );
				pGroup->pEnemyGroup->ubNumAdmins -= reduced;
				pGroup->ubGroupSize -= reduced;
				reducegroups -= reduced;
			}
			else if ( ubRank == REGULAR_MILITIA )
			{
				UINT16 reduced = min( reducegroups, pGroup->pEnemyGroup->ubNumTroops );
				pGroup->pEnemyGroup->ubNumTroops -= reduced;
				pGroup->ubGroupSize -= reduced;
				reducegroups -= reduced;
			}
			else if ( ubRank == ELITE_MILITIA )
			{
				UINT16 reduced = min( reducegroups, pGroup->pEnemyGroup->ubNumElites );
				pGroup->pEnemyGroup->ubNumElites -= reduced;
				pGroup->ubGroupSize -= reduced;
				reducegroups -= reduced;
			}
		}
		pGroup = pGroup->next;
	}

	if (ubHowMany && sMapX == gWorldSectorX && sMapY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

	// update the screen display
	fMapPanelDirty = TRUE;
}

void StrategicRemoveAllStaticMilitiaFromSector( INT16 sMapX, INT16 sMapY, UINT8 ubRank )
{
	if ( ubRank < MAX_MILITIA_LEVELS )
		SectorInfo[SECTOR( sMapX, sMapY )].ubNumberOfCivsAtLevel[ubRank] = 0;
	
	if ( sMapX == gWorldSectorX && sMapY == gWorldSectorY )
		gfStrategicMilitiaChangesMade = TRUE;

	// update the screen display
	fMapPanelDirty = TRUE;
}

// kill pts are (2 * kills) + assists
UINT8 CheckOneMilitiaForPromotion(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank, UINT8 ubRecentKillPts)
{
	UINT32 uiChanceToLevel = 0;

	switch( ubCurrentRank )
	{
		case GREEN_MILITIA:
			// 2 kill pts minimum
			if (ubRecentKillPts >= 2)
			{
				// 25% chance per kill pt
				uiChanceToLevel = 25 * ubRecentKillPts;
			}
			break;
		case REGULAR_MILITIA:
			// 5 kill pts minimum
			if (ubRecentKillPts >= 5)
			{
				// 10% chance per kill pt.
				uiChanceToLevel = 10 * ubRecentKillPts;
			}
			break;
		case ELITE_MILITIA:
			return 0;
			break;

	}
	// roll the bones, and see if he makes it
	if ( Chance( uiChanceToLevel ) )
	{
		StrategicPromoteMilitiaInSector(sMapX, sMapY, ubCurrentRank, 1);
		if( ubCurrentRank == GREEN_MILITIA )
		{
			//Attempt yet another level up if sufficient points
			if( ubRecentKillPts > 2 )
			{
				if( CheckOneMilitiaForPromotion( sMapX, sMapY, REGULAR_MILITIA, (UINT8)(ubRecentKillPts - 2) ) )
				{
					//success, this militia was promoted twice
					return 2;
				}
			}
		}

		return 1;
	}

	return 0;
}

// call this if the player attacks his own militia
void HandleMilitiaDefections(INT16 sMapX, INT16 sMapY)
{
	UINT8 ubMilitiaCnt;
	UINT32 uiChanceToDefect;

	for ( UINT8 ubRank = 0; ubRank < MAX_MILITIA_LEVELS; ++ubRank )
	{
		ubMilitiaCnt = MilitiaInSectorOfRank(sMapX, sMapY, ubRank);

		if ( ubMilitiaCnt )
		{
			switch ( ubRank )
			{
			case GREEN_MILITIA:
				uiChanceToDefect = 50;
				break;
			case REGULAR_MILITIA:
				uiChanceToDefect = 75;
				break;
			case ELITE_MILITIA:
				uiChanceToDefect = 90;
				break;
			}

			// check each guy at each rank to see if he defects
			for ( UINT8 ubCount = 0; ubCount < ubMilitiaCnt; ++ubCount )
			{
				// roll the bones; should I stay or should I go now?	(for you music fans out there)
				if ( Chance( uiChanceToDefect ) )
				{
					//B'bye!	(for you SNL fans out there)
					StrategicRemoveMilitiaFromSector(sMapX, sMapY, ubRank, 1);
				}
			}
		}
	}
}

UINT16 MilitiaInSectorOfRank(INT16 sMapX, INT16 sMapY, UINT8 ubRank)
{
	return MilitiaInSectorOfRankStationary( sMapX, sMapY, ubRank ) + MilitiaInSectorOfRankInGroups( sMapX, sMapY, ubRank );
}

UINT16 MilitiaInSectorOfRankStationary( INT16 sMapX, INT16 sMapY, UINT8 ubRank )
{
	if ( ubRank < MAX_MILITIA_LEVELS )
		return SectorInfo[SECTOR( sMapX, sMapY )].ubNumberOfCivsAtLevel[ubRank];

	return 0;
}

UINT16 MilitiaInSectorOfRankInGroups( INT16 sMapX, INT16 sMapY, UINT8 ubRank )
{
	UINT16 count = 0;

	GROUP *pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == MILITIA_TEAM && pGroup->ubSectorX == sMapX && pGroup->ubSectorY == sMapY )
		{
			if ( ubRank == GREEN_MILITIA )
				count += pGroup->pEnemyGroup->ubNumAdmins;
			else if ( ubRank == REGULAR_MILITIA )
				count += pGroup->pEnemyGroup->ubNumTroops;
			else if ( ubRank == ELITE_MILITIA )
				count += pGroup->pEnemyGroup->ubNumElites;
		}
		pGroup = pGroup->next;
	}

	return count;
}


BOOLEAN SectorOursAndPeaceful( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	// if this sector is currently loaded
	if ( ( sMapX == gWorldSectorX ) && ( sMapY == gWorldSectorY ) && ( bMapZ == gbWorldSectorZ ) )
	{
		// and either there are enemies prowling this sector, or combat is in progress
		if ( gTacticalStatus.fEnemyInSector || ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			return FALSE;
		}
	}

	// if sector is controlled by enemies, it's not ours (duh!)
	if( !bMapZ && StrategicMap[CALCULATE_STRATEGIC_INDEX( sMapX, sMapY )].fEnemyControlled == TRUE )
	{
		return FALSE;
	}

	if( NumHostilesInSector( sMapX, sMapY, bMapZ ) )
	{
		return FALSE;
	}

	// safe & secure, s'far as we can tell
	return(TRUE);
}


void InitFriendlyTownSectorServer(UINT8 ubTownId, INT16 sSkipSectorX, INT16 sSkipSectorY)
{
	// reset globals
	gubTownSectorServerTownId = ubTownId;
	gsTownSectorServerSkipX = sSkipSectorX;
	gsTownSectorServerSkipY = sSkipSectorY;

	gubTownSectorServerIndex = 0;
}


// this feeds the X,Y of the next town sector on the town list for the town specified at initialization
// it will skip an entry that matches the skip X/Y value if one was specified at initialization
// MUST CALL InitFriendlyTownSectorServer() before using!!!
BOOLEAN ServeNextFriendlySectorInTown( INT16 *sNeighbourX, INT16 *sNeighbourY )
{
	INT32 iTownSector;
	INT16 sMapX, sMapY;
	BOOLEAN fStopLooking = FALSE;

	do
	{
		// have we reached the end of the town list?
		if (pTownNamesList[ gubTownSectorServerIndex ] == BLANK_SECTOR)
		{
			// end of list reached
			return(FALSE);
		}

		iTownSector = pTownLocationsList[ gubTownSectorServerIndex ];

		// if this sector is in the town we're looking for
		if( StrategicMap[ iTownSector ].bNameId == gubTownSectorServerTownId )
		{
			// A sector in the specified town.	Calculate its X & Y sector compotents
			sMapX = iTownSector % MAP_WORLD_X;
			sMapY = iTownSector / MAP_WORLD_X;

			// Make sure we're not supposed to skip it
			if ( ( sMapX != gsTownSectorServerSkipX ) || ( sMapY != gsTownSectorServerSkipY ) )
			{
				// check if it's "friendly" - not enemy controlled, no enemies in it, no combat in progress
				if (SectorOursAndPeaceful( sMapX, sMapY, 0 ))
				{
					// then that's it!
					*sNeighbourX = sMapX;
					*sNeighbourY = sMapY;

					fStopLooking = TRUE;
				}
			}
		}

		// advance to next entry in town list
		gubTownSectorServerIndex++;

	} while ( !fStopLooking );


	// found & returning a valid sector
	return(TRUE);
}

void HandleInterfaceMessageForCostOfTrainingMilitia( SOLDIERTYPE *pSoldier )
{
	INT32 iMilitiaTrainingCost = gGameExternalOptions.iMilitiaTrainingCost * RebelCommand::GetMilitiaTrainingCostModifier();
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia2");

	CHAR16 sString[ 128 ];
	SGPRect pCenteringRect= {0 + xResOffset, 0, SCREEN_WIDTH - xResOffset, INV_INTERFACE_START_Y };
	INT32 iNumberOfSectors = 0;

	pMilitiaTrainerSoldier = pSoldier;

	// HEADROCK HAM 3.6: Which kind of militia are we training?
	UINT8 ubMilitiaType = 0;
	if (pSoldier->bAssignment == TRAIN_TOWN)
		ubMilitiaType = TOWN_MILITIA;
	else
		return;

	// grab total number of sectors
	iNumberOfSectors = GetNumberOfUnpaidTrainableSectors( ubMilitiaType );
	Assert( iNumberOfSectors > 0 );
	
	// get total cost
	if (ubMilitiaType == TOWN_MILITIA) // Garrison
	{
		// If Regulars are maxed, and Elites are allowed, calculate cost to upgrade Regular->Elite
		if (IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, REGULAR_MILITIA )
		&& (gGameExternalOptions.gfTrainVeteranMilitia)
		&& (GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay))
		{
			giTotalCostOfTraining = (iMilitiaTrainingCost*gGameExternalOptions.iVeteranCostModifier) * iNumberOfSectors;
			Assert( giTotalCostOfTraining > 0 );
			gfAreWePromotingRegular = TRUE;
		}
		else if (IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, GREEN_MILITIA ))
		{
			giTotalCostOfTraining = (iMilitiaTrainingCost*gGameExternalOptions.iRegularCostModifier) * iNumberOfSectors;
			Assert( giTotalCostOfTraining > 0 );
			gfAreWePromotingGreen = TRUE;
		}
		// Normal training.
		else
		{
			giTotalCostOfTraining = iMilitiaTrainingCost * iNumberOfSectors;
			Assert( giTotalCostOfTraining > 0 );
		}
	}

	gfYesNoPromptIsForContinue = FALSE;

	if( LaptopSaveInfo.iCurrentBalance < giTotalCostOfTraining )
	{
		// Different message for Mobiles
		if (ubMilitiaType == TOWN_MILITIA)
		{
			swprintf( sString, pMilitiaConfirmStrings[ 8 ], giTotalCostOfTraining );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback );
		}
		return;
	}

	// ok to start training, ask player
	if ( ubMilitiaType == TOWN_MILITIA )
	{
		if ( iNumberOfSectors > 1 )
		{
			swprintf( sString, pMilitiaConfirmStrings[7], iNumberOfSectors, giTotalCostOfTraining, pMilitiaConfirmStrings[1] );
		}
		else
		{
			swprintf( sString, L"%s%d. %s", pMilitiaConfirmStrings[0], giTotalCostOfTraining, pMilitiaConfirmStrings[1] );
		}

		// if we are in mapscreen, make a pop up
		if ( guiCurrentScreen == MAP_SCREEN )
		{
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback );
		}
		else
		{
			DoMessageBox( MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback, &pCenteringRect );
		}
	}
}

void DoContinueMilitiaTrainingMessageBox( INT16 sSectorX, INT16 sSectorY, const STR16 str, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	if( sSectorX <= 10 && sSectorY >= 6 && sSectorY <= 11 )
	{
		DoLowerScreenIndependantMessageBox( str, usFlags, ReturnCallback );
	}
	else
	{
		DoScreenIndependantMessageBox( str, usFlags, ReturnCallback );
	}
}

void HandleInterfaceMessageForContinuingTrainingMilitia( SOLDIERTYPE *pSoldier )
{
	CHAR16 sString[ 128 ];
	INT16 sSectorX = 0, sSectorY = 0;
	CHAR16 sStringB[ 128 ];
	INT8 bTownId;
	BOOLEAN fIsFull = FALSE;
	INT32 iCounter = 0;

	INT32 iMinLoyaltyToTrain = gGameExternalOptions.iMinLoyaltyToTrain;
	INT32 iMilitiaTrainingCost = gGameExternalOptions.iMilitiaTrainingCost * RebelCommand::GetMilitiaTrainingCostModifier();

DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia3");

	sSectorX = pSoldier->sSectorX;
	sSectorY = pSoldier->sSectorY;

	bTownId = GetTownIdForSector( sSectorX, sSectorY );

	UINT8 ubMilitiaType = 0;
	if (pSoldier->bAssignment == TRAIN_TOWN)
	{
		ubMilitiaType = TOWN_MILITIA;
	}

	if (ubMilitiaType == TOWN_MILITIA)
	{
		Assert( SectorInfo[ SECTOR( sSectorX, sSectorY ) ].fMilitiaTrainingPaid == FALSE );
	}

	pMilitiaTrainerSoldier = pSoldier;

	gfYesNoPromptIsForContinue = TRUE;
	
	// is there enough loyalty to continue training
	if( DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( pSoldier ) == FALSE )
	{
		// loyalty too low to continue training
		swprintf( sString, pMilitiaConfirmStrings[ 9 ], pTownNames[ GetTownIdForSector( sSectorX, sSectorY )], iMinLoyaltyToTrain );
		if (ubMilitiaType == TOWN_MILITIA)
		{
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback );
		}
		return;
	}

	// Training town militia and town is full of Elites (or Regulars, if Elites are disabled)
	if (ubMilitiaType == TOWN_MILITIA && IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, ELITE_MILITIA ))
	{
		// we're full!!! go home!
		if ( bTownId == BLANK_SECTOR )
		{
			// wilderness SAM site
			GetSectorIDString( sSectorX, sSectorY, 0, sStringB, TRUE );
			void (*tempFptr)(INT16 , INT16 , INT8	, CHAR16 *, BOOLEAN) = GetSectorIDString;
			swprintf( sString, pMilitiaConfirmStrings[ 10 ], sStringB, tempFptr, iMinLoyaltyToTrain );
		}
		else
		{
			// town
			swprintf( sString, pMilitiaConfirmStrings[ 10 ], pTownNames[ bTownId ], iMinLoyaltyToTrain );
		}
		DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback );
		return;
	}
			
	{
		if (IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, REGULAR_MILITIA )
			&& (gGameExternalOptions.gfTrainVeteranMilitia)
			&& (GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay))
		{
			giTotalCostOfTraining = (iMilitiaTrainingCost*gGameExternalOptions.iVeteranCostModifier);
			gfAreWePromotingRegular = TRUE;
		}
		else if (IsMilitiaTrainableFromSoldiersSectorMaxed( pSoldier, GREEN_MILITIA ))
		{
			giTotalCostOfTraining = (iMilitiaTrainingCost*gGameExternalOptions.iRegularCostModifier);
			gfAreWePromotingGreen = TRUE;
		}
		else
		{
			giTotalCostOfTraining = (iMilitiaTrainingCost);
		}
	}

	// can player afford to continue training?
	if( LaptopSaveInfo.iCurrentBalance < giTotalCostOfTraining )
	{
		// can't afford to continue training
		swprintf( sString, pMilitiaConfirmStrings[ 8 ], giTotalCostOfTraining );
		if (ubMilitiaType == TOWN_MILITIA)
		{
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback );
		}
		return;
	}

	// ok to continue, ask player

	GetSectorIDString( sSectorX, sSectorY, 0, sStringB, TRUE );
	if (ubMilitiaType == TOWN_MILITIA)
	{
		swprintf( sString, pMilitiaConfirmStrings[ 3 ], sStringB, pMilitiaConfirmStrings[ 4 ], giTotalCostOfTraining );
	}

	// ask player whether he'd like to continue training
	//DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback );
	if (ubMilitiaType == TOWN_MILITIA)
	{
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback );
	}
}

// IMPORTANT: This same callback is used both for initial training and for continue training prompt
// use 'gfYesNoPromptIsForContinue' flag to tell them apart
void PayMilitiaTrainingYesNoBoxCallback( UINT8 bExitValue )
{
	CHAR16 sString[ 128 ];

	Assert( giTotalCostOfTraining > 0 );

	// yes
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		// does the player have enough
		if( LaptopSaveInfo.iCurrentBalance >= giTotalCostOfTraining )
		{
			if( gfYesNoPromptIsForContinue )
			{
				ContinueTrainingInThisSector( TOWN_MILITIA );
			}
			else
			{
				StartTrainingInAllUnpaidTrainableSectors( TOWN_MILITIA );
			}

#ifdef JA2BETAVERSION
			// put this BEFORE training gets handled to avoid detecting an error everytime a sector completes training
			VerifyTownTrainingIsPaidFor();
#endif

			// this completes the training prompt sequence
			pMilitiaTrainerSoldier = NULL;
		}
		else	// can't afford it
		{
			StopTimeCompression();

			swprintf( sString, L"%s", pMilitiaConfirmStrings[ 2 ] );
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback );
		}
	}
	else if( bExitValue == MSG_BOX_RETURN_NO )
	{
		StopTimeCompression();

		MilitiaTrainingRejected( TOWN_MILITIA );
	}
}


void CantTrainMilitiaOkBoxCallback( UINT8 bExitValue )
{
	MilitiaTrainingRejected( TOWN_MILITIA );
}

// IMPORTANT: This same callback is used both for initial training and for continue training prompt
// use 'gfYesNoPromptIsForContinue' flag to tell them apart
void MilitiaTrainingRejected( UINT8 ubMilitiaType )
{
	if( gfYesNoPromptIsForContinue )
	{
		// take all mercs in that sector off militia training
		ResetAssignmentOfMercsThatWereTrainingMilitiaInThisSector( pMilitiaTrainerSoldier->sSectorX, pMilitiaTrainerSoldier->sSectorY, ubMilitiaType );
	}
	else
	{
		// take all mercs in unpaid sectors EVERYWHERE off militia training
		ResetAssignmentsForMercsTrainingUnpaidSectorsInSelectedList( ubMilitiaType );
	}

#ifdef JA2BETAVERSION
	// put this BEFORE training gets handled to avoid detecting an error everytime a sector completes training
	// HEADROCK HAM 3.6: Checks Mobiles and Garrisons
	VerifyTownTrainingIsPaidFor();
#endif

	// this completes the training prompt sequence
	pMilitiaTrainerSoldier = NULL;
}

void HandleMilitiaStatusInCurrentMapBeforeLoadingNewMap( void )
{
	if ( gTacticalStatus.Team[ MILITIA_TEAM ].bSide != 0 )
	{
		// handle militia defections and reset team to friendly
		HandleMilitiaDefections( gWorldSectorX, gWorldSectorY );
		gTacticalStatus.Team[ MILITIA_TEAM ].bSide = 0;
	}
	else if( !gfAutomaticallyStartAutoResolve )
	{ //Don't promote militia if we are going directly to autoresolve to finish the current battle.
		HandleMilitiaPromotions();
	}
}

BOOLEAN CanSomeoneNearbyScoutThisSector( INT16 sSectorX, INT16 sSectorY, BOOLEAN fScoutTraitCheck ) // added argument  - SANDRO
{
	INT16 sSectorValue = 0, sSector = 0;
	INT16 sCounterA = 0, sCounterB = 0;
	UINT8 ubScoutingRange = 1;
	BOOLEAN bScout = FALSE;

	if ( ConcealedMercInSector( sSectorX, sSectorY, FALSE ) )
		return TRUE;

	// get the sector value
	sSector = CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY );

	for( sCounterA = sSectorX - ubScoutingRange; (sCounterA <= sSectorX + ubScoutingRange && !bScout); ++sCounterA )
	{
		for( sCounterB = sSectorY - ubScoutingRange; (sCounterB <= sSectorY + ubScoutingRange && !bScout); ++sCounterB )
		{
			// skip out of bounds sectors
			if ( ( sCounterA < 1 ) || ( sCounterA > 16 ) || ( sCounterB < 1 ) || ( sCounterB > 16 ) )
			{
				continue;
			}

			sSectorValue = SECTOR( sCounterA, sCounterB );

			// check if any sort of militia here
			if ( NumNonPlayerTeamMembersInSector( sCounterA, sCounterB, MILITIA_TEAM ) )
			{
				return( TRUE );
			}

			// SANDRO - STOMP traits - Scouting check
			if (fScoutTraitCheck && gGameOptions.fNewTraitSystem && ( ScoutIsPresentInSquad( sCounterA, sCounterB ) || ConcealedMercInSector( sCounterA, sCounterB, TRUE ) ) )
			{
				// if diagonal sector and not allowed
				if (!gSkillTraitValues.fSCDetectionInDiagonalSectors && 
					((sCounterA - 1 == sSectorX && sCounterB + 1 == sSectorY) ||
					 (sCounterA - 1 == sSectorX && sCounterB - 1 == sSectorY) ||
					 (sCounterA + 1 == sSectorX && sCounterB + 1 == sSectorY) ||
					 (sCounterA + 1 == sSectorX && sCounterB - 1 == sSectorY))) 
				{
					continue;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOLEAN IsTownFullMilitia( INT8 bTownId, INT8 iMilitiaType )
{
	INT32 iMaxMilitiaPerSector = gGameExternalOptions.iMaxMilitiaPerSector;

	INT32 iCounter = 0;
	INT16 sSectorX = 0, sSectorY = 0;
	INT32 iNumberOfMilitia = 0;
	INT32 iMaxNumber = 0;
	BOOLEAN fIncreaseCost = FALSE;

	while( pTownNamesList[ iCounter ] != 0 )
	{
		if( pTownNamesList[ iCounter ] == bTownId )
		{
			// get the sector x and y
			sSectorX = pTownLocationsList[ iCounter ] % MAP_WORLD_X;
			sSectorY = pTownLocationsList[ iCounter ] / MAP_WORLD_X;

			// if sector is ours get number of militia here
			if( SectorOursAndPeaceful( sSectorX, sSectorY, 0 ) )
			{
				//Kaiden: Checking for Price Hikes.
				if (iMilitiaType == GREEN_MILITIA)
				{
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, GREEN_MILITIA );
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );
					iMaxNumber += iMaxMilitiaPerSector;

					if (MilitiaInSectorOfRank( sSectorX, sSectorY, GREEN_MILITIA ) > 0)
						fIncreaseCost = TRUE;
				}
				else if (iMilitiaType == REGULAR_MILITIA)
				{
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );
					iMaxNumber += iMaxMilitiaPerSector;

					if (MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA ) > 0)
						fIncreaseCost = TRUE;
				}

				//Kaiden: OK So iMilitiaType should be ELITE_MILITIA
				// and we're just checking sectors for purposes
				// of training mobile militia or ending training

				// don't count GREEN militia, they can be trained into regulars first
				// Kaiden: Veteran Militia Training:
				// And we don't count regulars either if we're training veterans
				if (gGameExternalOptions.gfTrainVeteranMilitia && (iMilitiaType == ELITE_MILITIA)
					&& (GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay))
				{
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );
					iMaxNumber += iMaxMilitiaPerSector;
				}
				else if (iMilitiaType == ELITE_MILITIA)
				{
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
					iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );
					iMaxNumber += iMaxMilitiaPerSector;
				}
			}
		}

		++iCounter;
	}

	if (iMilitiaType == GREEN_MILITIA)
	{
		if (( iNumberOfMilitia == iMaxNumber ) && fIncreaseCost )
			return( TRUE );
		else
			return( FALSE );

	}
	else if (iMilitiaType == REGULAR_MILITIA)
	{
		if (( iNumberOfMilitia == iMaxNumber ) && fIncreaseCost)
			return( TRUE );
		else
			return( FALSE );
	}

	// now check the number of militia
	if ( iMaxNumber > iNumberOfMilitia )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN IsSAMSiteFullOfMilitia( INT16 sSectorX, INT16 sSectorY, INT8 iMilitiaType )
{
	BOOLEAN fSamSitePresent = FALSE;
	INT32 iNumberOfMilitia = 0;
	INT32 iMaxNumber = 0;
	INT32 iMaxMilitiaPerSector = gGameExternalOptions.iMaxMilitiaPerSector;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia5");
	// check if SAM site is ours?
	fSamSitePresent = IsThisSectorASAMSector(sSectorX, sSectorY, 0) || RebelCommand::CanTrainMilitiaAnywhere();

	if( fSamSitePresent == FALSE )
	{
		return( FALSE );
	}

	if( SectorOursAndPeaceful( sSectorX, sSectorY, 0 ) )
	{

		//Kaiden: If we're checking for specific Militia for Price Hikes:
		if (iMilitiaType == GREEN_MILITIA)
		{
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, GREEN_MILITIA );
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );

			if (( iNumberOfMilitia == iMaxMilitiaPerSector) &&
				(MilitiaInSectorOfRank( sSectorX, sSectorY, GREEN_MILITIA ) > 0))
				return( TRUE );
			else
				return( FALSE );

		}
		else if (iMilitiaType == REGULAR_MILITIA)
		{
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );

			if (( iNumberOfMilitia == iMaxMilitiaPerSector) &&
				(MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA ) > 0))
				return( TRUE );
			else
				return( FALSE );
		}
		// Kaiden: Ok we're not checking for militia promotions now,
		// so MilitiaType should be ELITE_MILITIA for purposes
		// of training Roaming or not.

		// don't count GREEN militia, they can be trained into regulars first

		// Kaiden: Veteran Militia Training:
		// And we don't count regulars either if we're training veterans
		if (gGameExternalOptions.gfTrainVeteranMilitia && (iMilitiaType == ELITE_MILITIA)
			&& (GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay))
		{
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );
			iMaxNumber += iMaxMilitiaPerSector;
		}
		else if (iMilitiaType == ELITE_MILITIA)
		{
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, REGULAR_MILITIA );
			iNumberOfMilitia += MilitiaInSectorOfRank( sSectorX, sSectorY, ELITE_MILITIA );
			iMaxNumber += iMaxMilitiaPerSector;
		}
	}


	// now check the number of militia
	if ( iMaxNumber > iNumberOfMilitia )
	{
		return( FALSE );
	}

	return( TRUE );
}


void HandleCompletionOfTownTrainingByGroupWithTrainer( SOLDIERTYPE *pTrainer, UINT8 ubMilitiaType )
{

	INT16 sSectorX = 0, sSectorY = 0;
	INT8 bSectorZ = 0;
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter = 0;


	// get the sector values
	sSectorX = pTrainer->sSectorX;
	sSectorY = pTrainer->sSectorY;
	bSectorZ = pTrainer->bSectorZ;

	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		// valid character?
		if( gCharactersList[ iCounter ].fValid == FALSE )
		{
			// nope
			continue;
		}

		pSoldier = gCharactersList[ iCounter ].usSolID;

		// valid soldier?
		if( pSoldier->bActive == FALSE )
		{
			continue;
		}

		// HEADROCK HAM 3.6: Separated Mobiles from Garrisons.
		// silversurfer: We only want to set assignments done if it's the SAME assignment as the trainer's.
		//if ( ubMilitiaType == TOWN_MILITIA)
		if ( pTrainer->bAssignment == pSoldier->bAssignment )
		{
			if( /*( pSoldier->bAssignment == TRAIN_TOWN ) &&*/ ( pSoldier->sSectorX == sSectorX )&&( pSoldier->sSectorY == sSectorY )&&( pSoldier->bSectorZ == bSectorZ ) )
			{
				// done assignment
				AssignmentDone( pSoldier, FALSE, FALSE );
			}
		}
/*		else
		{
			if( ( pSoldier->bAssignment == TRAIN_MOBILE ) && ( pSoldier->sSectorX == sSectorX )&&( pSoldier->sSectorY == sSectorY )&&( pSoldier->bSectorZ == bSectorZ ) )
			{
				// done assignment
				AssignmentDone( pSoldier, FALSE, FALSE );
			}
		}*/
	}


	return;
}

void AddSectorForSoldierToListOfSectorsThatCompletedMilitiaTraining( SOLDIERTYPE *pSoldier )
{
	INT32 iCounter = 0;
	INT16 sSector = 0, sCurrentSector = 0;
	SOLDIERTYPE *pCurrentSoldier = NULL;

	// get the sector value
	sSector = pSoldier->sSectorX + pSoldier->sSectorY * MAP_WORLD_X;

	while( giListOfMercsInSectorsCompletedMilitiaTraining[ iCounter ] != -1 )
	{
		// get the current soldier
		pCurrentSoldier = &Menptr[ giListOfMercsInSectorsCompletedMilitiaTraining[ iCounter ] ];

		// get the current sector value
		sCurrentSector = CALCULATE_STRATEGIC_INDEX( pCurrentSoldier->sSectorX, pCurrentSoldier->sSectorY );

		// is the merc's sector already in the list?
		// silversurfer: Doesn't matter if it's the same sector. We can have different assignments in the same sector so add the soldier to the list if he's on a different assignment.
		if( sCurrentSector == sSector && pCurrentSoldier->bAssignment == pSoldier->bAssignment )
		{
			// already here
			return;
		}

		iCounter++;

		Assert( iCounter < SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST );
	}

	// add merc to the list
	giListOfMercsInSectorsCompletedMilitiaTraining[ iCounter ] = pSoldier->ubID;

	return;
}

// clear out the list of training sectors...should be done once the list is posted
void ClearSectorListForCompletedTrainingOfMilitia( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < SIZE_OF_MILITIA_COMPLETED_TRAINING_LIST; iCounter++ )
	{
		giListOfMercsInSectorsCompletedMilitiaTraining[ iCounter ] = -1;
	}

	return;
}


void HandleContinueOfTownTraining( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter = 0;
	BOOLEAN fContinueEventPosted = FALSE;


	while( giListOfMercsInSectorsCompletedMilitiaTraining[ iCounter ] != -1 )
	{
		// get the soldier
		pSoldier = &Menptr[ giListOfMercsInSectorsCompletedMilitiaTraining[ iCounter ] ];

		if( pSoldier->bActive )
		{
			fContinueEventPosted = TRUE;
#ifdef JA2UB
//no UB
#else
			SpecialCharacterDialogueEvent( DIALOGUE_SPECIAL_EVENT_CONTINUE_TRAINING_MILITIA, pSoldier->ubProfile, 0, 0, 0, 0 );
#endif

			// now set all of these peoples assignment done too
			//HandleInterfaceMessageForContinuingTrainingMilitia( pSoldier );
		}

		// next entry
		iCounter++;
	}

	// now clear the list
	ClearSectorListForCompletedTrainingOfMilitia( );

	if( fContinueEventPosted )
	{
	// ATE: If this event happens in tactical mode we will be switching at some time to mapscreen...
	if ( guiCurrentScreen == GAME_SCREEN )
	{
		gfEnteringMapScreen = TRUE;
	}

		//If the militia view isn't currently active, then turn it on when prompting to continue training.
		if ( !fShowMilitia )
	{
		ToggleShowMilitiaMode();
	}
	}

	return;
}



void BuildListOfUnpaidTrainableSectors( UINT8 ubMilitiaType )
{
	INT32 iCounter = 0, iCounterB = 0;
	SOLDIERTYPE *pSoldier = NULL;

    // WDS - make number of mercenaries, etc. be configurable
	memset( gsUnpaidStrategicSector, 0, sizeof(gsUnpaidStrategicSector) );

	if( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
	{
		for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
		{
			// valid character?
			if( gCharactersList[ iCounter ].fValid )
			{
				// selected?
				if( ( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE ) || ( iCounter == bSelectedAssignChar ) )
				{
					pSoldier = gCharactersList[ iCounter ].usSolID;

					// HEADROCK HAM 3.6: Two different conditions depending on the type of militia being trained.
					if( ubMilitiaType == TOWN_MILITIA )
					{
						if (CanCharacterTrainMilitia( pSoldier ) == TRUE )
						{
							if (SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMilitiaTrainingPaid == FALSE)
							{
								gsUnpaidStrategicSector[ iCounter ] = CALCULATE_STRATEGIC_INDEX( pSoldier->sSectorX, pSoldier->sSectorY );
							}
						}
					}
				}
			}
		}
	}
	else
	{
		// handle for tactical
		pSoldier = &Menptr[ gusUIFullTargetID ];
		iCounter = 0;

		if (ubMilitiaType == TOWN_MILITIA)
		{
			if (CanCharacterTrainMilitia( pSoldier ) == TRUE )
			{
				if (SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMilitiaTrainingPaid == FALSE )
				{
					gsUnpaidStrategicSector[ iCounter ] = CALCULATE_STRATEGIC_INDEX( pSoldier->sSectorX, pSoldier->sSectorY );
				}
			}
		}
	}

	// now clean out repeated sectors
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1; iCounter++ )
	{
		if( gsUnpaidStrategicSector[ iCounter ] > 0 )
		{
			for( iCounterB = iCounter + 1 ; iCounterB < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterB++ )
			{
				if( gsUnpaidStrategicSector[ iCounterB ] == gsUnpaidStrategicSector[ iCounter ] )
				{
					gsUnpaidStrategicSector[ iCounterB ] = 0;
				}
			}
		}
	}
}



INT32 GetNumberOfUnpaidTrainableSectors( UINT8 ubMilitiaType )
{
	INT32 iCounter = 0;
	INT32 iNumberOfSectors = 0;

	BuildListOfUnpaidTrainableSectors( ubMilitiaType );

	// now count up the results
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( gsUnpaidStrategicSector[ iCounter ] > 0 )
		{
			iNumberOfSectors++;
		}
	}

	// return the result
	return( iNumberOfSectors );

}

void StartTrainingInAllUnpaidTrainableSectors( UINT8 ubMilitiaType )
{
	INT32 iCounter = 0;
	UINT8 ubSector;

	if (ubMilitiaType == TOWN_MILITIA)
	{
		SetAssignmentForList( TRAIN_TOWN, 0 );
	}

	BuildListOfUnpaidTrainableSectors( ubMilitiaType );

	// pay up in each sector
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( gsUnpaidStrategicSector[ iCounter ] > 0 )
		{
			// convert strategic sector to 0-255 system
			ubSector = STRATEGIC_INDEX_TO_SECTOR_INFO( gsUnpaidStrategicSector[ iCounter ] );
			PayForTrainingInSector( ubSector );
		}
	}
}

void ContinueTrainingInThisSector( UINT8 ubMilitiaType )
{
	UINT8 ubSector;

	Assert( pMilitiaTrainerSoldier );

	// pay up in the sector where pMilitiaTrainerSoldier is
	ubSector = SECTOR( pMilitiaTrainerSoldier->sSectorX, pMilitiaTrainerSoldier->sSectorY );
	PayForTrainingInSector( ubSector );
}


void PayForTrainingInSector( UINT8 ubSector )
{
	INT32 iMilitiaTrainingCost = gGameExternalOptions.iMilitiaTrainingCost * RebelCommand::GetMilitiaTrainingCostModifier();
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia6");

	INT32 CostMultiplyer = 0;
	UINT8 ubMilitiaType = 0;

	// spend the money
	
	{
		Assert( SectorInfo[ ubSector ].fMilitiaTrainingPaid == FALSE );
		if ( gfAreWePromotingGreen)
		{
			CostMultiplyer = gGameExternalOptions.iRegularCostModifier;
			gfAreWePromotingGreen = FALSE;
		}
		else if ( gfAreWePromotingRegular)
		{
			CostMultiplyer = gGameExternalOptions.iVeteranCostModifier;
			gfAreWePromotingRegular = FALSE;
		}
		else
		{
			CostMultiplyer = 1;
		}
		// Mark sector as having paid
		SectorInfo[ ubSector ].fMilitiaTrainingPaid = TRUE;
		ubMilitiaType = TOWN_MILITIA;
	}

	AddTransactionToPlayersBook( TRAIN_TOWN_MILITIA, ubSector, GetWorldTotalMin(), -( iMilitiaTrainingCost*CostMultiplyer ) );

	// reset done flags
	ResetDoneFlagForAllMilitiaTrainersInSector( ubSector, ubMilitiaType );
}

void ResetDoneFlagForAllMilitiaTrainersInSector( UINT8 ubSector, UINT8 ubMilitiaType )
{
	SOLDIERTYPE *pSoldier = NULL;
	
	for( INT32 iCounter = 0; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++iCounter )
	{
		pSoldier = &Menptr[ iCounter ];

		if( pSoldier->bActive )
		{
			if (ubMilitiaType == TOWN_MILITIA)
			{
				if( pSoldier->bAssignment == TRAIN_TOWN )
				{
					if( ( SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) == ubSector ) && ( pSoldier->bSectorZ == 0 ) )
					{
						pSoldier->flags.fDoneAssignmentAndNothingToDoFlag = FALSE;
						pSoldier->usQuoteSaidExtFlags &= ~SOLDIER_QUOTE_SAID_DONE_ASSIGNMENT;
					}
				}
			}
		}
	}
}

BOOLEAN MilitiaTrainingAllowedInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT8 bTownId;
	BOOLEAN fSamSitePresent = FALSE;


	if( bSectorZ != 0 )
	{
		return( FALSE );
	}

	fSamSitePresent = IsThisSectorASAMSector( sSectorX, sSectorY, bSectorZ );

	if( fSamSitePresent )
	{
		// all SAM sites may have militia trained at them
		return(TRUE);
	}


	bTownId = GetTownIdForSector( sSectorX, sSectorY );


	return( MilitiaTrainingAllowedInTown( bTownId ) );
}



BOOLEAN MilitiaTrainingAllowedInTown( INT8 bTownId )
{
	return( gfMilitiaAllowedInTown[bTownId] );
}

void BuildMilitiaPromotionsString( STR16 str )
{
	CHAR16 pStr[256];
	BOOLEAN fAddSpace = FALSE;
	swprintf( str, L"" );

	if( !gbMilitiaPromotions )
	{
		return;
	}
	if( gbGreenToElitePromotions > 1 )
	{
		swprintf( pStr, gzLateLocalizedString[22], gbGreenToElitePromotions );
		wcscat( str, pStr );
		fAddSpace = TRUE;
	}
	else if( gbGreenToElitePromotions == 1 )
	{
		wcscat( str, gzLateLocalizedString[29] );
		fAddSpace = TRUE;
	}

	if( gbGreenToRegPromotions > 1 )
	{
		if( fAddSpace )
		{
			wcscat( str, L" " );
		}
		swprintf( pStr, gzLateLocalizedString[23], gbGreenToRegPromotions );
		wcscat( str, pStr );
		fAddSpace = TRUE;
	}
	else if( gbGreenToRegPromotions == 1 )
	{
		if( fAddSpace )
		{
			wcscat( str, L" " );
		}
		wcscat( str, gzLateLocalizedString[30] );
		fAddSpace = TRUE;
	}

	if( gbRegToElitePromotions > 1 )
	{
		if( fAddSpace )
		{
			wcscat( str, L" " );
		}
		swprintf( pStr, gzLateLocalizedString[24], gbRegToElitePromotions );
		wcscat( str, pStr );
	}
	else if( gbRegToElitePromotions == 1 )
	{
		if( fAddSpace )
		{
			wcscat( str, L" " );
		}
		wcscat( str, gzLateLocalizedString[31] );
		fAddSpace = TRUE;
	}

	//Clear the fields
	gbGreenToElitePromotions = 0;
	gbGreenToRegPromotions = 0;
	gbRegToElitePromotions = 0;
	gbMilitiaPromotions = 0;
}

// HEADROCK HAM 3.3: This function finds the best Militia Trainer in the target sector. It may also take into account
// mercs' TEACHING skill, if told to. The function returns the calculated "Effective Leadership" of the selected
// trainer.
UINT8 FindBestMilitiaTrainingLeadershipInSector ( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT8 ubMilitiaType )
{
	UINT16 cnt = 0;
	SOLDIERTYPE * pCheckedTrainer;
	UINT16 usTrainerEffectiveLeadership = 0;
	UINT8 ubBestLeadership = 0;

	// HEADROCK HAM 3.3: Add search for best trainer in sector
	for ( pCheckedTrainer = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ++cnt, ++pCheckedTrainer )
	{
		if  (pCheckedTrainer->bActive && pCheckedTrainer->stats.bLife >= OKLIFE && (ubMilitiaType == TOWN_MILITIA && pCheckedTrainer->bAssignment == TRAIN_TOWN) )
		{
			if (pCheckedTrainer->sSectorX == sMapX && pCheckedTrainer->sSectorY == sMapY && pCheckedTrainer->bSectorZ == bMapZ )
			{
				usTrainerEffectiveLeadership = EffectiveLeadership(pCheckedTrainer);

				if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits
				{
					// -10% penalty for untrained mercs
					usTrainerEffectiveLeadership = (usTrainerEffectiveLeadership * (100 + gSkillTraitValues.bSpeedModifierTrainingMilitia) / 100);

					if (HAS_SKILL_TRAIT( pCheckedTrainer, TEACHING_NT ))
					{
						// bonus from Teaching trait
						usTrainerEffectiveLeadership = __min(100,(usTrainerEffectiveLeadership * (100 + gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia) / 100 ));
					}
				}
				// Effective leadership is modified by an INI-based percentage, once for every TEACHING trait level.
				else if ( gGameExternalOptions.usTeacherTraitEffectOnLeadership > 0 && gGameExternalOptions.usTeacherTraitEffectOnLeadership != 100 )
				{
					for (UINT8 i = 0; i < NUM_SKILL_TRAITS( pCheckedTrainer, TEACHING_OT ); i++ )
					{
						// percentage-based.
						usTrainerEffectiveLeadership = __min(100,((usTrainerEffectiveLeadership * gGameExternalOptions.usTeacherTraitEffectOnLeadership)/100));
					}
				}

				if (usTrainerEffectiveLeadership > ubBestLeadership)
				{
					ubBestLeadership = __min(100,usTrainerEffectiveLeadership);
				}
			}
		}
	}
	
	return(ubBestLeadership);
}

// HEADROCK HAM 3.6: Created a new function to make things easier. This calculates the target number of militia to
// be created.
UINT8 CalcNumMilitiaTrained(UINT8 ubBestLeadership)
{
	UINT8 ubReqLeadershipForFull = 1;
	UINT8 ubMinLeadershipForTraining = 0;
	UINT8 ubLeadershipRange = 0;

	// Result value
	UINT8 ubMilitiaToTrain = 0;
	
	// Garrison Training
	
	// Default Squad Size for Garrisons, read from INI.
	ubMilitiaToTrain = gGameExternalOptions.iTrainingSquadSize;

	// Is affected by Leadership?
	if (gGameExternalOptions.fLeadershipAffectsMilitiaQuantity &&
		gGameExternalOptions.ubReqLeadershipForFullTraining > gGameExternalOptions.ubMinimumLeadershipToTrainMilitia)
	{
		// Set these variables to their INI values.
		ubReqLeadershipForFull = gGameExternalOptions.ubReqLeadershipForFullTraining;
		ubMinLeadershipForTraining = gGameExternalOptions.ubMinimumLeadershipToTrainMilitia;

		// We make sure our effective leadership isn't above or below the limits. If so, reset to limits.
		// If there is a minimum required to train militia, we're ASSUMING that the character has enough leadership
		// otherwise they wouldn't even be here. But just in case he's skipped the check somehow, this will raise 
		// leadership to the minimum to avoid problems.
		ubBestLeadership = __max(ubBestLeadership, ubMinLeadershipForTraining);
		ubBestLeadership = __min(ubBestLeadership, ubReqLeadershipForFull);
			
		// Find out by how much we've beaten the skill check
		ubBestLeadership = ubBestLeadership - ubMinLeadershipForTraining;

		// Find the range between the minimum and maximum.
		ubLeadershipRange = ubReqLeadershipForFull - ubMinLeadershipForTraining;

		// Squad Size is determined by the relative value of the leadership within the range. Anyone meeting or 
		// exceeding the range (latter shouldn't happen) will train a full squad. Others will train less. If
		// leadership was the minimum required, only 1 militia is trained.
		ubMilitiaToTrain = ((ubBestLeadership * ubMilitiaToTrain) / ubLeadershipRange) + 1;
		ubMilitiaToTrain = __min(ubMilitiaToTrain, gGameExternalOptions.iTrainingSquadSize);
		ubMilitiaToTrain = __max(1, ubMilitiaToTrain);
	}
	
	return (ubMilitiaToTrain);
}

// HEADROCK HAM 3.6: Daily check for upkeep of all militia
void HandleMilitiaUpkeepPayment( void )
{
	UINT32 uiNewTotalUpkeep = CalcMilitiaUpkeep();

	if (guiTotalUpkeepForMilitia > 0)
	{
		// Yesterday night we already had some militia, so we're eligible to pay tonight.

		// Pay no more than what we owed yesterday. Militia added today don't get paid.
		UINT32 uiPayment = __min( uiNewTotalUpkeep, guiTotalUpkeepForMilitia );

		if (uiPayment > 0)
		{
			// Do we have enough money?
			if (LaptopSaveInfo.iCurrentBalance < (INT32)uiPayment)
			{
				INT32 uiMoneyUnpaid = uiPayment - LaptopSaveInfo.iCurrentBalance;

				// Find and remove militia across the map. This is done randomly, starting with elites.
				UINT32 uiNumMilitiaDisbanded = 0;
				UINT32 usGreenDisbanded = 0;
				UINT32 usRegularsDisbanded = 0;
				UINT32 usElitesDisbanded = 0;

				MILITIA_LIST_TYPE MilitiaList[256];

				// Go through every sector and count how many militia we've got in total.
				for (UINT16 cnt = 0; cnt < 256; ++cnt)
				{
					SECTORINFO *pSectorInfo = &( SectorInfo[ cnt ] );
					UINT8 sMapX = SECTORX(cnt);
					UINT8 sMapY = SECTORY(cnt);

					MilitiaList[cnt].ubSectorId = (UINT8)cnt;

					MilitiaList[cnt].ubNumTownGreens   = MilitiaInSectorOfRank( sMapX, sMapY, GREEN_MILITIA );
					MilitiaList[cnt].ubNumTownRegulars = MilitiaInSectorOfRank( sMapX, sMapY, REGULAR_MILITIA );
					MilitiaList[cnt].ubNumTownElites   = MilitiaInSectorOfRank( sMapX, sMapY, ELITE_MILITIA );
				}

				// Resort list so that sectors with more militia are at the top of the array. Higher weight
				// for mobiles, and moreso for elites, then regulars, then greens.
				qsort( MilitiaList, 256, sizeof(MILITIA_LIST_TYPE), MilitiaListQsortCompare);

				while (uiMoneyUnpaid > 0)
				{
					if (MilitiaList[0].ubNumTownGreens +
						MilitiaList[0].ubNumTownRegulars +
						MilitiaList[0].ubNumTownElites == 0)
					{
						//All sectors are empty? Default to 0 payment.
						uiMoneyUnpaid = 0;
					}

					INT16 sX = SECTORX( MilitiaList[0].ubSectorId );
					INT16 sY = SECTORY( MilitiaList[0].ubSectorId );

					if (MilitiaList[0].ubNumTownElites > 0)
					{
						StrategicRemoveMilitiaFromSector( sX, sY, ELITE_MILITIA, 1 );
						MilitiaList[0].ubNumTownElites--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostTown[ELITE_MILITIA];
						++uiNumMilitiaDisbanded;
						++usElitesDisbanded;

						DisbandIndividualMilitia( MilitiaList[0].ubSectorId, 0, 0, 1 );
					}
					else if (MilitiaList[0].ubNumTownRegulars > 0)
					{
						StrategicRemoveMilitiaFromSector( sX, sY, REGULAR_MILITIA, 1 );
						MilitiaList[0].ubNumTownRegulars--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostTown[REGULAR_MILITIA];
						++uiNumMilitiaDisbanded;
						++usRegularsDisbanded;

						DisbandIndividualMilitia( MilitiaList[0].ubSectorId, 0, 1, 0 );
					}
					else if (MilitiaList[0].ubNumTownGreens > 0)
					{
						StrategicRemoveMilitiaFromSector( sX, sY, GREEN_MILITIA, 1 );
						MilitiaList[0].ubNumTownGreens--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostTown[GREEN_MILITIA];
						++uiNumMilitiaDisbanded;
						++usGreenDisbanded;

						DisbandIndividualMilitia( MilitiaList[0].ubSectorId, 1, 0, 0 );
					}
					else
					{
						// Somehow there are none to remove?!
						Assert(1);
					}

					// Resort the list!
					qsort( MilitiaList, 256, sizeof(MILITIA_LIST_TYPE), MilitiaListQsortCompare);
				}

				// Pay all we can
				uiPayment = LaptopSaveInfo.iCurrentBalance;
				
				// Notify player that X militia have been automatically disbanded due to debt.
				CHAR16 sString[256];
				swprintf( sString, New113HAMMessage[21], uiNumMilitiaDisbanded );
				DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			}
			AddTransactionToPlayersBook( MILITIA_UPKEEP, 0, GetWorldTotalMin(), (-1) * (INT32)uiPayment );
		}
	}

	// Record tonight's upkeep costs
	guiTotalUpkeepForMilitia = uiNewTotalUpkeep;
}

int MilitiaListQsortCompare(const void *pArg1, const void *pArg2)
{
	if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownElites > ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownElites)
	{
		return(-1);
	}
	else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownRegulars > ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownRegulars)
	{
		return(-1);
	}
	else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownGreens > ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownGreens)
	{
		return(-1);
	}
	else
	{
		if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownGreens < ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownGreens)
		{
			return(1);
		}
		else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownRegulars < ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownRegulars)
		{
			return(1);
		}
		else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownElites < ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownElites)
		{
			return(1);
		}		
		else
		{
			return(0);
		}
	}
}

// Calculate amount to pay for militia upkeep today.
UINT32 CalcMilitiaUpkeep( void )
{
	UINT32 uiTotalPayment = 0;

	UINT32 militia_static[MAX_MILITIA_LEVELS] = {0, 0, 0};
	UINT32 militia_individual[MAX_MILITIA_LEVELS] = {0, 0, 0};

	for (UINT16 cnt = 0; cnt < 256; ++cnt)
	{
		SECTORINFO *pSectorInfo = &( SectorInfo[ cnt ] );
		UINT8 sMapX = SECTORX(cnt);
		UINT8 sMapY = SECTORY(cnt);

		for ( int i = GREEN_MILITIA; i < MAX_MILITIA_LEVELS; ++i )
			militia_static[i] += MilitiaInSectorOfRank( sMapX, sMapY, i );
	}

	// Flugente: if indivídual militia is on, calculate costs there and use the above as a fallback option
	if ( gGameExternalOptions.fIndividualMilitia )
	{
		uiTotalPayment = GetDailyUpkeep_IndividualMilitia( militia_individual[GREEN_MILITIA], militia_individual[REGULAR_MILITIA], militia_individual[ELITE_MILITIA] );
	}

	for ( int i = GREEN_MILITIA; i < MAX_MILITIA_LEVELS; ++i )
	{
		if ( militia_static[i] > militia_individual[i] )
		{
			uiTotalPayment += ( militia_static[i] - militia_individual[i] ) * gGameExternalOptions.usDailyCostTown[i];
		}
	}

	return uiTotalPayment * RebelCommand::GetMilitiaUpkeepCostModifier();
}

// Flugente: our militia volunteer pool is limited
INT32 GetVolunteerPool()
{
	// if this feature is off, we simply assume we have reserves
	if ( !gGameExternalOptions.fMilitiaVolunteerPool )
		return 999999;

	// the pool is actually a float number, only return full volunteers though
	return (INT32)LaptopSaveInfo.dMilitiaVolunteerPool;
}

// add/remove volunteers from pool
void AddVolunteers( FLOAT asNum )
{
	LaptopSaveInfo.dMilitiaVolunteerPool = max( 0, LaptopSaveInfo.dMilitiaVolunteerPool + asNum );
}

FLOAT CalcHourlyVolunteerGain()
{
	FLOAT loyalpopulation = 0.0f;					// the number of loyal citizens, from which we derive the hourly volunteer gain
	FLOAT populationmodifier = 1.0f;				// certain sectors/facilities under our control increase the volunteer gain

	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			// not if the enemy controls this sector
			if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].fEnemyControlled )
				continue;
			
			UINT8 ubTownID = StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].bNameId;
			if ( ubTownID != BLANK_SECTOR )
			{
				UINT16 population = GetSectorPopulation( sX, sY, FALSE );

				loyalpopulation += gTownLoyalty[ubTownID].ubRating * population / 100;
			}
			// modifier increase for every farm we control
			else if ( IsSectorFarm( sX, sY ) )
				populationmodifier += gGameExternalOptions.dMilitiaVolunteerMultiplierFarm;
		}
	}

	FLOAT hourlygain = log( 1.0 + loyalpopulation * populationmodifier * gGameExternalOptions.dMilitiaVolunteerGainFactorHourly);

	return hourlygain;
}

// every hour, controlled sectors add to our volunteer pool
void UpdateVolunteers()
{	
	AddVolunteers( CalcHourlyVolunteerGain() );
}

// Flugente: militia resources
BOOLEAN ConvertItemToResources( OBJECTTYPE& object, BOOLEAN fAll, FLOAT& arValue_Gun, FLOAT& arValue_Armour, FLOAT& arValue_Misc )
{
	UINT16 usItemType = object.usItem;
	UINT8 ubNumberOfObjects = fAll ? object.ubNumberOfObjects : 1;
	UINT8 coolness = Item[usItemType].ubCoolness;			
	UINT8 progress = HighestPlayerProgressPercentage( );
	FLOAT mod_progress = ResourceProgressModifier( progress );

	if ( Item[usItemType].usItemClass & IC_AMMO )
	{
		// different ammo items of the same caliber and ammotype have different coolness. To fix that, we simply always use that of the corresponing crate item. Should it not exist, we use 1.
		UINT8 ammocoolness = 1;

		for ( int iCrateLoop = 0; iCrateLoop < gMAXITEMS_READ; ++iCrateLoop )
		{
			// Is it the right ammo crate?
			if ( Item[iCrateLoop].usItemClass == IC_AMMO &&
				 Magazine[Item[iCrateLoop].ubClassIndex].ubMagType == AMMO_CRATE && // An ammo crate or box
				 Magazine[Item[iCrateLoop].ubClassIndex].ubCalibre == Magazine[Item[usItemType].ubClassIndex].ubCalibre && //Same caliber
				 Magazine[Item[iCrateLoop].ubClassIndex].ubAmmoType == Magazine[Item[usItemType].ubClassIndex].ubAmmoType ) // Same ammotype
			{
				// Found a crate for this ammo.
				ammocoolness = Item[iCrateLoop].ubCoolness;
				break;
			}
		}

		UINT32 bullets = 0;
		for ( UINT8 ubLoop = 0; ubLoop < ubNumberOfObjects; ++ubLoop )
		{
			bullets += object[ubLoop]->data.ubShotsLeft;
		}

		arValue_Gun += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Ammo_Bullet * ammocoolness * mod_progress * bullets);
	}
	else
	{
		for ( UINT8 ubLoop = 0; ubLoop < ubNumberOfObjects; ++ubLoop )
		{
			if ( Item[usItemType].usItemClass & IC_GUN )
			{
				arValue_Gun += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Gun * gGameExternalOptions.fMilitiaResources_WeaponMod[Weapon[usItemType].ubWeaponType] * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);

				// take into account any loaded mag
				if ( object[ubLoop]->data.gun.ubGunShotsLeft > 0 )
				{
					// different ammo items of the same caliber and ammotype have different coolness. To fix that, we simply always use that of the corresponing crate item. Should it not exist, we use 1.
					UINT8 ammocoolness = 1;

					for ( int iCrateLoop = 0; iCrateLoop < gMAXITEMS_READ; ++iCrateLoop )
					{
						// Is it the right ammo crate?
						if ( Item[iCrateLoop].usItemClass == IC_AMMO &&
							 Magazine[Item[iCrateLoop].ubClassIndex].ubMagType == AMMO_CRATE && // An ammo crate or box
							 Magazine[Item[iCrateLoop].ubClassIndex].ubCalibre == Magazine[Item[object[ubLoop]->data.gun.usGunAmmoItem].ubClassIndex].ubCalibre && //Same caliber
							 Magazine[Item[iCrateLoop].ubClassIndex].ubAmmoType == Magazine[Item[object[ubLoop]->data.gun.usGunAmmoItem].ubClassIndex].ubAmmoType ) // Same ammotype
						{
							// Found a crate for this ammo.
							ammocoolness = Item[iCrateLoop].ubCoolness;
							break;
						}
					}

					UINT32 bullets = object[ubLoop]->data.gun.ubGunShotsLeft;

					arValue_Gun += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Ammo_Bullet * ammocoolness * mod_progress * bullets);
				}
			}
			else if ( Item[usItemType].usItemClass & (IC_BLADE | IC_THROWING_KNIFE | IC_PUNCH | IC_THROWN) )
			{
				arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Melee * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & IC_LAUNCHER )
			{
				arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Gun * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & (IC_GRENADE) )
			{
				arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Grenade * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & (IC_BOMB) )
			{
				arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Bomb * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & IC_ARMOUR )
			{
				arValue_Armour += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Armour * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & IC_FACE )
			{
				arValue_Armour += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Face * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & IC_LBEGEAR )
			{
				arValue_Armour += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_LBE * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
			}
			else if ( Item[usItemType].usItemClass & IC_MISC )
			{
				UINT32 attachmentClass = Item[usItemType].attachmentclass;

				if ( attachmentClass & ( AC_MUZZLE | AC_SIGHT | AC_MAGWELL | AC_INTERNAL | AC_EXTERNAL | AC_EXTENDER | AC_IRONSIGHT | AC_FEEDER ) )
				{
					arValue_Gun += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_Low * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				if ( attachmentClass & ( AC_FOREGRIP | AC_STOCK | AC_LASER) )
				{
					arValue_Gun += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_Medium * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				if ( attachmentClass & (AC_BIPOD | AC_SCOPE | AC_UNDERBARREL ) )
				{
					arValue_Gun += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_High * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				else if ( attachmentClass & (AC_SLING | AC_MODPOUCH ) )
				{
					arValue_Armour += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_Low * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				else if ( attachmentClass & (AC_HELMET | AC_VEST | AC_PANTS ) )
				{
					arValue_Armour += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_Medium * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				else if ( attachmentClass & ( AC_DETONATOR | AC_BATTERY | AC_REMOTEDET | AC_DEFUSE) )
				{
					arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_Low * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				else if ( attachmentClass & (AC_GRENADE | AC_RIFLEGRENADE | AC_BAYONET) )
				{
					arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_Medium * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
				else if ( attachmentClass & ( AC_ROCKET ) )
				{
					arValue_Misc += (FLOAT)(gGameExternalOptions.fMilitiaResources_ItemClassMod_Attachment_High * coolness * mod_progress * object[ubLoop]->data.objectStatus / 100.0f);
				}
			}
			else
				continue;

			for ( attachmentList::iterator iter = object[ubLoop]->attachments.begin( ); iter != object[ubLoop]->attachments.end( ); ++iter )
			{
				if ( iter->exists( ) )
				{
					ConvertItemToResources( *iter, TRUE, arValue_Gun, arValue_Armour, arValue_Misc );
				}
			}
		}
	}

	return (arValue_Gun + arValue_Armour + arValue_Misc > 0.0001f);
}

void AddResources( FLOAT aValue_Gun, FLOAT aValue_Armour, FLOAT aValue_Misc )
{
	LaptopSaveInfo.dMilitiaGunPool		= max( 0, LaptopSaveInfo.dMilitiaGunPool + aValue_Gun );
	LaptopSaveInfo.dMilitiaArmourPool	= max( 0, LaptopSaveInfo.dMilitiaArmourPool + aValue_Armour );
	LaptopSaveInfo.dMilitiaMiscPool		= max( 0, LaptopSaveInfo.dMilitiaMiscPool + aValue_Misc );
}

void GetResources( FLOAT& arValue_Gun, FLOAT& arValue_Armour, FLOAT& arValue_Misc )
{
	arValue_Gun = LaptopSaveInfo.dMilitiaGunPool;
	arValue_Armour = LaptopSaveInfo.dMilitiaArmourPool;
	arValue_Misc = LaptopSaveInfo.dMilitiaMiscPool;
}

FLOAT ResourceProgressModifier( UINT8 aProgress )
{
	// value need to be between 1 and 100
	aProgress = max( 1, min( 100, aProgress ) );

	return (FLOAT)(gGameExternalOptions.fMilitiaResources_ProgressFactor / aProgress);
}

void DevalueResources( UINT8 aOldProgress, UINT8 aNewProgress )
{
	// both progress values need to be between 1 and 100
	aOldProgress = max( 1, min( 100, aOldProgress ) );
	aNewProgress = max( 1, min( 100, aNewProgress ) );

	// modifier is new progress value / old value
	FLOAT modifier = ResourceProgressModifier( aNewProgress ) / ResourceProgressModifier( aOldProgress );

	LaptopSaveInfo.dMilitiaGunPool		*= modifier;
	LaptopSaveInfo.dMilitiaArmourPool	*= modifier;
	LaptopSaveInfo.dMilitiaMiscPool		*= modifier;
}

// Flugente: intel
void AddIntel( FLOAT aValue, BOOLEAN aDoMessage )
{
	LaptopSaveInfo.dIntelPool = max( 0, LaptopSaveInfo.dIntelPool + aValue );

	if ( aDoMessage )
	{
		if ( aValue >= 0.0f )
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Gained %.2f intel.", aValue );
		else
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Spent %.2f intel.", -aValue );
	}

	if ( !IsBookMarkSet( INTELMARKET_BOOKMARK ) )
		AddStrategicEvent( EVENT_INTEL_ENRICO_EMAIL, GetWorldTotalMin() + 60 * ( 1 + Random( 3 ) ), 0 );
}

FLOAT GetIntel()
{
	return LaptopSaveInfo.dIntelPool;
}

// Flugente: raid 'personnel'
void AddRaidPersonnel( INT32 aBloodcats, INT32 aZombie, INT32 aBandits )
{
	LaptopSaveInfo.sRaidBloodcats	= max( 0, LaptopSaveInfo.sRaidBloodcats + aBloodcats );
	LaptopSaveInfo.sRaidZombies		= max( 0, LaptopSaveInfo.sRaidZombies + aZombie );
	LaptopSaveInfo.sRaidBandits		= max( 0, LaptopSaveInfo.sRaidBandits + aBandits );
}

void GetRaidPersonnel( INT32& arBloodcats, INT32& arZombie, INT32& arBandits )
{
	arBloodcats	= LaptopSaveInfo.sRaidBloodcats;
	arZombie	= LaptopSaveInfo.sRaidZombies;
	arBandits	= LaptopSaveInfo.sRaidBandits;
}
