#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
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
	#include "PreBattle Interface.h"
	#include "Map Screen Interface Border.h"
	#include "interface control.h"
	#include "Map Screen Interface Map.h"
	#include "laptop.h"							// added by Flugente
	#include "Game Event Hook.h"				// added by Flugente
#endif

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
BOOLEAN gfAreWeTrainingMobile = FALSE;
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
// HEADROCK HAM 3.6: Duplicate function for handling callback on Mobile Militia training
void PayMobileMilitiaTrainingYesNoBoxCallback( UINT8 bExitValue );
void CantTrainMilitiaOkBoxCallback( UINT8 bExitValue );
void CantTrainMobileMilitiaOkBoxCallback( UINT8 bExitValue );

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

void TownMilitiaTrainingCompleted( SOLDIERTYPE *pTrainer, INT16 sMapX, INT16 sMapY )
{
	UINT8 ubMilitiaTrained = 0;
	BOOLEAN fFoundOne;
	INT16 sNeighbourX, sNeighbourY;
	UINT8 ubTownId;

	INT32 iMaxMilitiaPerSector = gGameExternalOptions.iMaxMilitiaPerSector;

	// HEADROCK HAM 3.6: Leadership may affect the resulting squad size.
	UINT8 ubTrainerEffectiveLeadership = FindBestMilitiaTrainingLeadershipInSector ( sMapX, sMapY, pTrainer->bSectorZ, TOWN_MILITIA );
	UINT8 iTrainingSquadSize = __min(iMaxMilitiaPerSector, CalcNumMilitiaTrained(ubTrainerEffectiveLeadership, FALSE));

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia1");

	// get town index
	ubTownId = StrategicMap[ sMapX + sMapY * MAP_WORLD_X ].bNameId;

	if( ubTownId == BLANK_SECTOR )
	{
		Assert( IsThisSectorASAMSector( sMapX, sMapY, 0 ) );
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

	// HEADROCK HAM 3.6: This is now VOLUNTARY.
	if( pTrainer->bAssignment == TRAIN_MOBILE )
	{
		CreateMilitiaSquads(sMapX, sMapY );
		
		// the trainer announces to player that he's finished his assignment.	Make his sector flash!
		AssignmentDone( pTrainer, TRUE, FALSE );

		// handle completion of town by training group
		HandleCompletionOfTownTrainingByGroupWithTrainer( pTrainer, MOBILE_MILITIA );
	}
	else
	{
		while (ubMilitiaTrained < iTrainingSquadSize)
		{
			// is there room for another militia in the training sector itself?
			if (CountAllMilitiaInSector(sMapX, sMapY) < iMaxMilitiaPerSector)
			{
				// great! Create a new GREEN militia guy in the training sector
				StrategicAddMilitiaToSector(sMapX, sMapY, GREEN_MILITIA, 1);
				if (sMapX == gWorldSectorX && sMapY == gWorldSectorY)
				{
					gfStrategicMilitiaChangesMade = TRUE;
				}
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
						if (CountAllMilitiaInSector(sNeighbourX, sNeighbourY) < iMaxMilitiaPerSector)
						{
							// great! Create a new GREEN militia guy in the neighbouring sector
							StrategicAddMilitiaToSector(sNeighbourX, sNeighbourY, GREEN_MILITIA, 1);

							if (sNeighbourX == gWorldSectorX && sNeighbourY == gWorldSectorY)
							{
								gfStrategicMilitiaChangesMade = TRUE;
							}

							fFoundOne = TRUE;
							break;
						}
					}
				}

				// if we still haven't been able to train anyone
				if (!fFoundOne)
				{
					// alrighty, then.	We'll have to *promote* guys instead.

					// are there any GREEN militia men in the training sector itself?
					if (MilitiaInSectorOfRank(sMapX, sMapY, GREEN_MILITIA) > 0)
					{
						// great! Promote a GREEN militia guy in the training sector to a REGULAR
						StrategicPromoteMilitiaInSector(sMapX, sMapY, GREEN_MILITIA, 1);
						fFoundOne = TRUE;
						if (sMapX == gWorldSectorX && sMapY == gWorldSectorY)
						{
							gfStrategicMilitiaChangesMade = TRUE;
						}
					}
					else
					{
						if( ubTownId != BLANK_SECTOR )
						{
							// dammit! Last chance - try to find other eligible sectors in the same town with a Green guy to be promoted
							InitFriendlyTownSectorServer(ubTownId, sMapX, sMapY);

							// check other eligible sectors in this town for room for another militia
							while( ServeNextFriendlySectorInTown( &sNeighbourX, &sNeighbourY ) )
							{
								// are there any GREEN militia men in the neighbouring sector ?
								if (MilitiaInSectorOfRank(sNeighbourX, sNeighbourY, GREEN_MILITIA) > 0)
								{
									// great! Promote a GREEN militia guy in the neighbouring sector to a REGULAR
									StrategicPromoteMilitiaInSector(sNeighbourX, sNeighbourY, GREEN_MILITIA, 1);

									if (sNeighbourX == gWorldSectorX && sNeighbourY == gWorldSectorY)
									{
										gfStrategicMilitiaChangesMade = TRUE;
									}

									fFoundOne = TRUE;
									break;
								}
							}
						}

						// Kaiden: Veteran militia training
						// This is essentially copy/pasted from above
						// But the names have been changed to protect the innocent
						if ((!fFoundOne) && (gGameExternalOptions.gfTrainVeteranMilitia)
							&& (GetWorldDay( ) >= gGameExternalOptions.guiTrainVeteranMilitiaDelay))
						{
							// are there any REGULAR militia men in the training sector itself?
							if (MilitiaInSectorOfRank(sMapX, sMapY, REGULAR_MILITIA) > 0)
							{
								// great! Promote a REGULAR militia guy in the training sector to a VETERAN
								StrategicPromoteMilitiaInSector(sMapX, sMapY, REGULAR_MILITIA, 1);
								if (sMapX == gWorldSectorX && sMapY == gWorldSectorY)
								{
									gfStrategicMilitiaChangesMade = TRUE;
								}

								fFoundOne = TRUE;
							}
							else
							{
								if( ubTownId != BLANK_SECTOR )
								{
									// dammit! Last chance - try to find other eligible sectors in the same town with a Regular guy to be promoted
									InitFriendlyTownSectorServer(ubTownId, sMapX, sMapY);

									// check other eligible sectors in this town for room for another militia
									while( ServeNextFriendlySectorInTown( &sNeighbourX, &sNeighbourY ) )
									{
										// are there any REGULAR militia men in the neighbouring sector ?
										if (MilitiaInSectorOfRank(sNeighbourX, sNeighbourY, REGULAR_MILITIA) > 0)
										{
											// great! Promote a Regular militia guy in the neighbouring sector to a Veteran
											StrategicPromoteMilitiaInSector(sNeighbourX, sNeighbourY, REGULAR_MILITIA, 1);

											if (sNeighbourX == gWorldSectorX && sNeighbourY == gWorldSectorY)
											{
												gfStrategicMilitiaChangesMade = TRUE;
											}

											fFoundOne = TRUE;
											break;
										}
									}
								}
							}
						}

						// if we still haven't been able to train anyone
						if (!fFoundOne)
						{
							// Well, that's it.	All eligible sectors of this town are full of REGULARs or ELITEs.
							// The training goes to waste in this situation.
							break; // the main while loop
						}
					}
				}
			}

			// next, please!
			++ubMilitiaTrained;
		}

		if (gfStrategicMilitiaChangesMade)
		{
			ResetMilitia();
		}

		// if anyone actually got trained
		if (ubMilitiaTrained > 0)
		{
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
			RecordNumMilitiaTrainedForMercs( sMapX, sMapY, pTrainer->bSectorZ, ubMilitiaTrained, FALSE );
		}

		// the trainer announces to player that he's finished his assignment.	Make his sector flash!
		AssignmentDone( pTrainer, TRUE, FALSE );

		// handle completion of town by training group
		HandleCompletionOfTownTrainingByGroupWithTrainer( pTrainer, TOWN_MILITIA );
	}

	// Flugente: if we trained militia, the PMC notices us and offers their services
	if ( gGameExternalOptions.fPMC )
		AddStrategicEvent( EVENT_PMC_EMAIL, GetWorldTotalMin() + 60 * (1 + Random(6)), 0 );
}


// feed this a SOLDIER_CLASS_, it will return you a _MITILIA rank, or -1 if the guy's not militia
INT8 SoldierClassToMilitiaRank(UINT8 ubSoldierClass)
{
	INT8 bRank = -1;

	switch( ubSoldierClass )
	{
		case SOLDIER_CLASS_GREEN_MILITIA:
			bRank = GREEN_MILITIA;
			break;
		case SOLDIER_CLASS_REG_MILITIA:
			bRank = REGULAR_MILITIA;
			break;
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


void StrategicAddMilitiaToSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT8 ubHowMany)
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


void StrategicPromoteMilitiaInSector(INT16 sMapX, INT16 sMapY, UINT8 ubCurrentRank, UINT8 ubHowMany)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	// damn well better have that many around to promote!
	//Assert(pSectorInfo->ubNumberOfCivsAtLevel[ ubCurrentRank ] >= ubHowMany);

	//KM : July 21, 1999 patch fix
	if( pSectorInfo->ubNumberOfCivsAtLevel[ ubCurrentRank ] < ubHowMany )
	{
		return;
	}

	pSectorInfo->ubNumberOfCivsAtLevel[ ubCurrentRank	 ] -= ubHowMany;
	pSectorInfo->ubNumberOfCivsAtLevel[ ubCurrentRank + 1 ] += ubHowMany;

	if (ubHowMany && sMapX == gWorldSectorX && sMapY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

	// update the screen display
	fMapPanelDirty = TRUE;
}


void StrategicRemoveMilitiaFromSector(INT16 sMapX, INT16 sMapY, UINT8 ubRank, UINT8 ubHowMany)
{
	SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR( sMapX, sMapY ) ] );

	// WANNE: BUGZILLA #357: Fixed assertion when miltia dies.
	// damn well better have that many around to remove!
	//Assert(pSectorInfo->ubNumberOfCivsAtLevel[ ubRank ] >= ubHowMany);

	//KM : July 21, 1999 patch fix
	if( pSectorInfo->ubNumberOfCivsAtLevel[ ubRank ] < ubHowMany )
	{
		return;
	}

	pSectorInfo->ubNumberOfCivsAtLevel[ ubRank ] -= ubHowMany;

	if (ubHowMany && sMapX == gWorldSectorX && sMapY == gWorldSectorY )
	{
		gfStrategicMilitiaChangesMade = TRUE;
	}

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
	if (Random(100) < uiChanceToLevel)
	{
		StrategicPromoteMilitiaInSector(sMapX, sMapY, ubCurrentRank, 1);
		if( ubCurrentRank == GREEN_MILITIA )
		{ //Attempt yet another level up if sufficient points
			if( ubRecentKillPts > 2 )
			{
				if( CheckOneMilitiaForPromotion( sMapX, sMapY, REGULAR_MILITIA, (UINT8)(ubRecentKillPts - 2) ) )
				{ //success, this militia was promoted twice
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
	UINT8 ubRank;
	UINT8 ubMilitiaCnt;
	UINT8 ubCount;
	UINT32 uiChanceToDefect;

	for( ubRank = 0; ubRank < MAX_MILITIA_LEVELS; ubRank++ )
	{
		ubMilitiaCnt = MilitiaInSectorOfRank(sMapX, sMapY, ubRank);

		// check each guy at each rank to see if he defects
		for (ubCount = 0; ubCount < ubMilitiaCnt; ubCount++)
		{
			switch( ubRank )
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
				default:
					Assert( 0 );
					return;
			}

			// roll the bones; should I stay or should I go now?	(for you music fans out there)
			if (Random(100) < uiChanceToDefect)
			{
				//B'bye!	(for you SNL fans out there)
				StrategicRemoveMilitiaFromSector(sMapX, sMapY, ubRank, 1);
			}
		}
	}
}


UINT8 CountAllMilitiaInSector(INT16 sMapX, INT16 sMapY)
{
	UINT8 ubMilitiaTotal = 0;
	UINT8 ubRank;

	// find out if there are any town militia in this SECTOR (don't care about other sectors in same town)
	for( ubRank = 0; ubRank < MAX_MILITIA_LEVELS; ubRank++ )
	{
		ubMilitiaTotal += MilitiaInSectorOfRank(sMapX, sMapY, ubRank);
	}

	return(ubMilitiaTotal);
}


UINT8 MilitiaInSectorOfRank(INT16 sMapX, INT16 sMapY, UINT8 ubRank)
{
	unsigned count = SectorInfo[ SECTOR( sMapX, sMapY ) ].ubNumberOfCivsAtLevel[ ubRank ];
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
	if( !bMapZ && StrategicMap[ sMapX + sMapY * MAP_WORLD_X ].fEnemyControlled == TRUE )
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
	INT32 iMilitiaTrainingCost = gGameExternalOptions.iMilitiaTrainingCost;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia2");

	CHAR16 sString[ 128 ];
	SGPRect pCenteringRect= {0 + xResOffset, 0, SCREEN_WIDTH - xResOffset, INV_INTERFACE_START_Y };
	INT32 iNumberOfSectors = 0;

	pMilitiaTrainerSoldier = pSoldier;

	// HEADROCK HAM 3.6: Which kind of militia are we training?
	UINT8 ubMilitiaType = 0;
	if (pSoldier->bAssignment == TRAIN_TOWN)
		ubMilitiaType = TOWN_MILITIA;
	else if (pSoldier->bAssignment == TRAIN_MOBILE)
		ubMilitiaType = MOBILE_MILITIA;
	else
		return;
	// grab total number of sectors
	iNumberOfSectors = GetNumberOfUnpaidTrainableSectors( ubMilitiaType );
	Assert( iNumberOfSectors > 0 );

	// Kaiden: Roaming Militia Training:
	// We want to charge more for Roaming

	// get total cost
	// HEADROCK HAM 3.6: Mobile and Garrison Militia now separate
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
		gfAreWeTrainingMobile = FALSE;
	}
	// Mobile
	else 
	{
		giTotalCostOfTraining = (iMilitiaTrainingCost*gGameExternalOptions.iMilitiaCostModifier) * iNumberOfSectors;
		Assert( giTotalCostOfTraining > 0 );
		gfAreWeTrainingMobile = TRUE;
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
		else
		{
			swprintf( sString, pMilitiaConfirmStrings[ 11 ], giTotalCostOfTraining );
			DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, CantTrainMobileMilitiaOkBoxCallback );
		}
		return;
	}

	// ok to start training, ask player


	if( iNumberOfSectors > 1 )
	{
		if (ubMilitiaType == TOWN_MILITIA)
		{
			swprintf( sString, pMilitiaConfirmStrings[ 7 ], iNumberOfSectors, giTotalCostOfTraining, pMilitiaConfirmStrings[ 1 ] );
		}
		else
		{
			swprintf( sString, pMilitiaConfirmStrings[ 13 ], iNumberOfSectors, giTotalCostOfTraining, pMilitiaConfirmStrings[ 1 ] );
		}
	}
	else
	{
		if (ubMilitiaType == TOWN_MILITIA)
		{
			swprintf( sString, L"%s%d. %s", pMilitiaConfirmStrings[ 0 ], giTotalCostOfTraining, pMilitiaConfirmStrings[ 1 ] );
		}
		else
		{
			swprintf( sString, L"%s%d. %s", pMilitiaConfirmStrings[ 14 ], giTotalCostOfTraining, pMilitiaConfirmStrings[ 1 ] );
		}
	}

	// if we are in mapscreen, make a pop up
	if( guiCurrentScreen == MAP_SCREEN )
	{
		// HEADROCK HAM 3.6: Separate callbacks for Mobile and Garrison militia
		if (ubMilitiaType == TOWN_MILITIA)
		{
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback );
		}
		else
		{
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayMobileMilitiaTrainingYesNoBoxCallback );
		}
	}
	else
	{
		if (ubMilitiaType == TOWN_MILITIA)
		{
			DoMessageBox( MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback, &pCenteringRect );
		}
		else
		{
			DoMessageBox( MSG_BOX_BASIC_STYLE, sString, GAME_SCREEN, MSG_BOX_FLAG_YESNO, PayMobileMilitiaTrainingYesNoBoxCallback, &pCenteringRect );
		}
	}

	return;
}

void DoContinueMilitiaTrainingMessageBox( INT16 sSectorX, INT16 sSectorY, const STR16 str, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
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
	INT32 iMilitiaTrainingCost = gGameExternalOptions.iMilitiaTrainingCost;

DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia3");

	sSectorX = pSoldier->sSectorX;
	sSectorY = pSoldier->sSectorY;

	bTownId = GetTownIdForSector( sSectorX, sSectorY );

	UINT8 ubMilitiaType = 0;
	if (pSoldier->bAssignment == TRAIN_TOWN)
	{
		ubMilitiaType = TOWN_MILITIA;
	}
	else if (pSoldier->bAssignment == TRAIN_MOBILE )
	{
		ubMilitiaType = MOBILE_MILITIA;
	}

	if (ubMilitiaType == TOWN_MILITIA)
	{
		Assert( SectorInfo[ SECTOR( sSectorX, sSectorY ) ].fMilitiaTrainingPaid == FALSE );
	}
	else if (ubMilitiaType == MOBILE_MILITIA)
	{
		Assert( SectorInfo[ SECTOR( sSectorX, sSectorY ) ].fMobileMilitiaTrainingPaid == FALSE );
	}

	pMilitiaTrainerSoldier = pSoldier;

	gfYesNoPromptIsForContinue = TRUE;

	//Moa: prevent continue training when at/above maximum
	if ( ubMilitiaType == MOBILE_MILITIA )
	{
		if ( 100 <= GetMobileMilitiaQuota( TRUE ) )
		{
			MilitiaTrainingRejected( MOBILE_MILITIA );
			return;
		}
	}

	// is there enough loyalty to continue training
	if( DoesSectorMercIsInHaveSufficientLoyaltyToTrainMilitia( pSoldier ) == FALSE )
	{
		// loyalty too low to continue training
		swprintf( sString, pMilitiaConfirmStrings[ 9 ], pTownNames[ GetTownIdForSector( sSectorX, sSectorY )], iMinLoyaltyToTrain );
		if (ubMilitiaType == TOWN_MILITIA)
		{
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMilitiaOkBoxCallback );
		}
		else if (ubMilitiaType == MOBILE_MILITIA)
		{
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMobileMilitiaOkBoxCallback );
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
	else if (ubMilitiaType == MOBILE_MILITIA)
	{
		// Test to see if not enough Town Militia
		SECTORINFO *pSectorInfo = &( SectorInfo[ SECTOR(pSoldier->sSectorX, pSoldier->sSectorY) ] );
		BOOLEAN fUnfullSectorFound = FALSE;

		// Test to see if not enough Town Militia
		// HEADROCK HAM 4: No need for this check anymore. Militia can now be trained separately thanks to being
		// able to keep them out of cities.
		/*

		if ( CountMilitia( pSectorInfo ) < gGameExternalOptions.iMaxMilitiaPerSector )
		{
			fUnfullSectorFound = TRUE;
		}
		else
		{
			iCounter = 0;
			while( pTownNamesList[ iCounter ] != 0 )
			{
				// Are we in this city?
				if( pTownNamesList[ iCounter] == bTownId )
				{
					INT16 sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
					INT16 sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

					pSectorInfo = &( SectorInfo[ SECTOR(sCurrentX, sCurrentY) ] );
					// if sector has enemies or hasn't already been taken at least once, then
					if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || 
						NumNonPlayerTeamMembersInSector( sCurrentX, sCurrentY, ENEMY_TEAM ) > 0 )
					{
						// skip the rest. This sector cannot generate militia anyway. 
						iCounter++;
						continue;
					}
					else
					{
						if (CountMilitia(pSectorInfo) < gGameExternalOptions.iMaxMilitiaPerSector )
						{
							// Found a controlled city sector that does not yet have a full garrison
							fUnfullSectorFound;
						}
					}
				}
				iCounter++;
			}
		}
		if (fUnfullSectorFound)
		{
			// At least one city sector is controlled but not full of garrison militia. Can't train mobiles!
			swprintf(sString, New113HAMMessage[9], gGameExternalOptions.iMaxMilitiaPerSector, pTownNames[bTownId]);
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMobileMilitiaOkBoxCallback );
			return;
		}
		*/

		//////////////////////////////////////////
		// Capacity check in nearby sectors
		UINT16 pMoveDir[4][3];
		UINT8 uiDirNumber = 0;
		BOOLEAN fFoundValidSector = FALSE;

		
		INT8 bTownId = GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY );
		GenerateDirectionInfosForTraining( pSoldier->sSectorX, pSoldier->sSectorY, &uiDirNumber, pMoveDir );
		// Found at least one suitable place to put Mobiles?
		if (uiDirNumber)
		{
			fFoundValidSector = TRUE;
		}
		// Try entire city.
		else
		{
			INT32 iCounter = 0;
			
			// Go through each city in the game
			while( pTownNamesList[ iCounter ] != 0 )
			{
				// Are we in this city?
				if( pTownNamesList[ iCounter] == bTownId )
				{
					INT16 sCurrentX = GET_X_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );
					INT16 sCurrentY = GET_Y_FROM_STRATEGIC_INDEX( pTownLocationsList[ iCounter ] );

					// if sector has enemies or hasn't already been taken at least once, then
					if ( !SectorInfo[ SECTOR(sCurrentX, sCurrentY) ].fSurfaceWasEverPlayerControlled || 
						 NumNonPlayerTeamMembersInSector( sCurrentX, sCurrentY, ENEMY_TEAM ) > 0 )
					{
						// skip the rest. This sector cannot generate militia anyway. 
						iCounter++;
						continue;
					}

					// Find out if any adjacent sectors have room in them.
					GenerateDirectionInfosForTraining( sCurrentX, sCurrentY, &uiDirNumber, pMoveDir );

					if(uiDirNumber)
					{
						fFoundValidSector = TRUE;
						break;
					}
				}
				iCounter++;
			}
		}

		// Couldn't find at least one sector to place Mobiles. Report "No room!"
		if (!fFoundValidSector) 
		{
			swprintf( sString, New113HAMMessage[ 8 ], pTownNames[ bTownId ], iMinLoyaltyToTrain );
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMobileMilitiaOkBoxCallback );
			return;
		}
	}

	// Kaiden: Roaming Militia Training:
	// Charging more to train Roaming Militia
	// Also Charging more for promotions over Training

	if( ubMilitiaType == MOBILE_MILITIA )
	{
		giTotalCostOfTraining = (iMilitiaTrainingCost*gGameExternalOptions.iMilitiaCostModifier);
		gfAreWeTrainingMobile = TRUE;
	}

	else
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
		gfAreWeTrainingMobile = FALSE;
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
		else if (ubMilitiaType == MOBILE_MILITIA)
		{
			DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_OK, CantTrainMobileMilitiaOkBoxCallback );
		}
		return;
	}

	// ok to continue, ask player

	GetSectorIDString( sSectorX, sSectorY, 0, sStringB, TRUE );
	if (ubMilitiaType == TOWN_MILITIA)
	{
		swprintf( sString, pMilitiaConfirmStrings[ 3 ], sStringB, pMilitiaConfirmStrings[ 4 ], giTotalCostOfTraining );
	}
	else if (ubMilitiaType == MOBILE_MILITIA)
	{
		swprintf( sString, pMilitiaConfirmStrings[ 12 ], sStringB, pMilitiaConfirmStrings[ 4 ], giTotalCostOfTraining );
	}

	// ask player whether he'd like to continue training
	//DoContinueMilitiaTrainingMessageBox( sSectorX, sSectorY, sString, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback );
	if (ubMilitiaType == TOWN_MILITIA)
	{
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayMilitiaTrainingYesNoBoxCallback );
	}
	else if (ubMilitiaType == MOBILE_MILITIA)
	{
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_YESNO, PayMobileMilitiaTrainingYesNoBoxCallback );
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

	return;
}


void CantTrainMilitiaOkBoxCallback( UINT8 bExitValue )
{
	MilitiaTrainingRejected( TOWN_MILITIA );
	return;
}

// HEADROCK HAM 3.6: Duplicate of the above, for Mobile Militia purposes.
void CantTrainMobileMilitiaOkBoxCallback( UINT8 bExitValue )
{
	MilitiaTrainingRejected( MOBILE_MILITIA );
	return;
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


	// get the sector value
	sSector = sSectorX + sSectorY * MAP_WORLD_X;

	for( sCounterA = sSectorX - ubScoutingRange; sCounterA <= sSectorX + ubScoutingRange; sCounterA++ )
	{
		for( sCounterB = sSectorY - ubScoutingRange; sCounterB <= sSectorY + ubScoutingRange; sCounterB++ )
		{
			// skip out of bounds sectors
			if ( ( sCounterA < 1 ) || ( sCounterA > 16 ) || ( sCounterB < 1 ) || ( sCounterB > 16 ) )
			{
				continue;
			}

			sSectorValue = SECTOR( sCounterA, sCounterB );

			// SANDRO - STOMP traits - Scouting check
			if (fScoutTraitCheck && gGameOptions.fNewTraitSystem && ScoutIsPresentInSquad( sCounterA, sCounterB ))
			{
				// if diagonal sector and not allowed
				if (!gSkillTraitValues.fSCDetectionInDiagonalSectors && 
					((sCounterA - 1 == sSectorX && sCounterB + 1 == sSectorY) ||
					 (sCounterA - 1 == sSectorX && sCounterB - 1 == sSectorY) ||
					 (sCounterA + 1 == sSectorX && sCounterB + 1 == sSectorY) ||
					 (sCounterA + 1 == sSectorX && sCounterB - 1 == sSectorY))) 
				{
					return( FALSE );
				}
				else
					return( TRUE );
			}
			else
			{
				// check if any sort of militia here
				if( SectorInfo[ sSectorValue ].ubNumberOfCivsAtLevel[ GREEN_MILITIA ] )
				{
					return( TRUE );
				}
				else if( SectorInfo[ sSectorValue ].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ] )
				{
					return( TRUE );
				}
				else if( SectorInfo[ sSectorValue ].ubNumberOfCivsAtLevel[ ELITE_MILITIA ] )
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
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

		iCounter++;
	}

		if (iMilitiaType == GREEN_MILITIA)
		{
			if (( iNumberOfMilitia == iMaxNumber	) && (fIncreaseCost))
				return( TRUE );
			else
				return( FALSE );

		}
		else if (iMilitiaType == REGULAR_MILITIA)
		{
			if (( iNumberOfMilitia == iMaxNumber ) && (fIncreaseCost))
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
	fSamSitePresent = IsThisSectorASAMSector(	sSectorX, sSectorY, 0 );

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

		pSoldier = &Menptr[ gCharactersList[ iCounter ].usSolID ];

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
		sCurrentSector = pCurrentSoldier->sSectorX + pCurrentSoldier->sSectorY * MAP_WORLD_X;

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
					pSoldier = &Menptr[ gCharactersList[ iCounter ].usSolID ];

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
					else
					{
						if (CanCharacterTrainMobileMilitia( pSoldier ) == TRUE )
						{
							if (SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMobileMilitiaTrainingPaid == FALSE)
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
		else
		{
			if (CanCharacterTrainMobileMilitia( pSoldier ) == TRUE )
			{
				if (SectorInfo[ SECTOR( pSoldier->sSectorX, pSoldier->sSectorY ) ].fMobileMilitiaTrainingPaid == FALSE )
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
	else if (ubMilitiaType == MOBILE_MILITIA)
	{
		SetAssignmentForList( TRAIN_MOBILE, 0 );
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
	INT32 iMilitiaTrainingCost = gGameExternalOptions.iMilitiaTrainingCost;
DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"Militia6");

	INT32 CostMultiplyer = 0;
	UINT8 ubMilitiaType = 0;

	// spend the money
	// Kaiden: Roaming Militia Training:
	// Charging more to train Roaming Militia
	// Or for promotions.

	if ( gfAreWeTrainingMobile)
	{
		Assert( SectorInfo[ ubSector ].fMobileMilitiaTrainingPaid == FALSE );
		// Mark sector as being paid up
		SectorInfo[ ubSector ].fMobileMilitiaTrainingPaid = TRUE;
		CostMultiplyer = gGameExternalOptions.iMilitiaCostModifier;
		gfAreWeTrainingMobile = FALSE;
		ubMilitiaType = MOBILE_MILITIA;
	}
	else 
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
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	for( iCounter = 0; iCounter <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; iCounter++ )
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
			else if (ubMilitiaType == MOBILE_MILITIA)
			{
				if( pSoldier->bAssignment == TRAIN_MOBILE )
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
	for ( pCheckedTrainer = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pCheckedTrainer++)
	{
		if (pCheckedTrainer->bActive && pCheckedTrainer->stats.bLife >= OKLIFE && 
			((ubMilitiaType == TOWN_MILITIA && pCheckedTrainer->bAssignment == TRAIN_TOWN) ||
			(ubMilitiaType == MOBILE_MILITIA && pCheckedTrainer->bAssignment == TRAIN_MOBILE) ) )
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
UINT8 CalcNumMilitiaTrained(UINT8 ubBestLeadership, BOOLEAN fMobile)
{

	UINT8 ubReqLeadershipForFull = 1;
	UINT8 ubMinLeadershipForTraining = 0;
	UINT8 ubLeadershipRange = 0;

	// Result value
	UINT8 ubMilitiaToTrain = 0;
	

	// Garrison Training
	if (!fMobile)
	{
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
	}

	// Mobile Militia
	else
	{
		// Default Squad Size for Mobiles, read from INI.
		ubMilitiaToTrain = gGameExternalOptions.guiNumMobileMilitiaTrained;

		// Is affected by Leadership?
		if (gGameExternalOptions.fLeadershipAffectsMobileMilitiaQuantity && 
			gGameExternalOptions.ubReqLeadershipForFullMobileTraining > gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia )
		{

			// Set these variables to their INI values.
			ubReqLeadershipForFull = gGameExternalOptions.ubReqLeadershipForFullMobileTraining;
			ubMinLeadershipForTraining = gGameExternalOptions.ubMinimumLeadershipToTrainMobileMilitia;

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
			// leadership was the minimum required, only 3 militia are trained.
			ubMilitiaToTrain = ((ubBestLeadership * ubMilitiaToTrain) / ubLeadershipRange) + 1;
			ubMilitiaToTrain = __min(ubMilitiaToTrain, gGameExternalOptions.guiNumMobileMilitiaTrained);
			ubMilitiaToTrain = __max(3, ubMilitiaToTrain);
		}
	}

	return (ubMilitiaToTrain);
}

// HEADROCK HAM 3.6: Duplicate function for handling callback on Mobile Militia training
void PayMobileMilitiaTrainingYesNoBoxCallback( UINT8 bExitValue )
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
				ContinueTrainingInThisSector( MOBILE_MILITIA );
			}
			else
			{
				StartTrainingInAllUnpaidTrainableSectors( MOBILE_MILITIA );
			}

#ifdef JA2BETAVERSION
			// put this BEFORE training gets handled to avoid detecting an error everytime a sector completes training
			// Checks Town and Mobile militia
			VerifyTownTrainingIsPaidFor();
#endif

			// this completes the training prompt sequence
			pMilitiaTrainerSoldier = NULL;
		}
		else	// can't afford it
		{
			StopTimeCompression();

			swprintf( sString, L"%s", pMilitiaConfirmStrings[ 2 ] );
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, CantTrainMobileMilitiaOkBoxCallback );
		}
	}
	else if( bExitValue == MSG_BOX_RETURN_NO )
	{
		StopTimeCompression();

		MilitiaTrainingRejected( MOBILE_MILITIA );
	}

	return;
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

				MILITIA_LIST_TYPE MilitiaList[256];

				// Go through every sector and count how many militia we've got in total.
				for (UINT16 cnt = 0; cnt < 256; cnt++)
				{
					SECTORINFO *pSectorInfo = &( SectorInfo[ cnt ] );
					UINT8 sMapX = SECTORX(cnt);
					UINT8 sMapY = SECTORY(cnt);

					MilitiaList[cnt].ubSectorId = (UINT8)cnt;

					UINT8 ubTownId = GetTownIdForSector( sMapX, sMapY );
					if ((ubTownId != BLANK_SECTOR && MilitiaTrainingAllowedInTown(ubTownId)) || // Major Town
						IsThisSectorASAMSector( sMapX, sMapY, 0 ) ) // SAM Site
					{
						MilitiaList[cnt].ubNumTownGreens = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
						MilitiaList[cnt].ubNumTownRegulars = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
						MilitiaList[cnt].ubNumTownElites = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
						MilitiaList[cnt].ubNumMobileGreens = 0;
						MilitiaList[cnt].ubNumMobileRegulars = 0;
						MilitiaList[cnt].ubNumMobileElites = 0;
					}
					else
					{
						MilitiaList[cnt].ubNumMobileGreens = pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA];
						MilitiaList[cnt].ubNumMobileRegulars = pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA];
						MilitiaList[cnt].ubNumMobileElites = pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA];
						MilitiaList[cnt].ubNumTownGreens = 0;
						MilitiaList[cnt].ubNumTownRegulars = 0;
						MilitiaList[cnt].ubNumTownElites = 0;
					}
				}

				// Resort list so that sectors with more militia are at the top of the array. Higher weight
				// for mobiles, and moreso for elites, then regulars, then greens.
				qsort( MilitiaList, 256, sizeof(MILITIA_LIST_TYPE), MilitiaListQsortCompare);

				while (uiMoneyUnpaid > 0)
				{
					if (MilitiaList[0].ubNumTownGreens +
						MilitiaList[0].ubNumTownRegulars +
						MilitiaList[0].ubNumTownElites +
						MilitiaList[0].ubNumMobileGreens +
						MilitiaList[0].ubNumMobileRegulars +
						MilitiaList[0].ubNumMobileElites == 0)
					{
						//All sectors are empty? Default to 0 payment.
						uiMoneyUnpaid = 0;
					}

					SECTORINFO *pSectorInfo = &( SectorInfo[ MilitiaList[0].ubSectorId ] );

					if (MilitiaList[0].ubNumMobileElites > 0)
					{
						// Remove one militia from sector
						pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA]--;
						// Adjust list entry
						MilitiaList[0].ubNumMobileElites--;
						// Reduce debt appropriately
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostMobileElite;
						// Increase tally of militia removed
						uiNumMilitiaDisbanded++;
					}
					else if (MilitiaList[0].ubNumMobileRegulars > 0)
					{
						pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]--;
						MilitiaList[0].ubNumMobileRegulars--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostMobileRegular;
						uiNumMilitiaDisbanded++;
					}
					else if (MilitiaList[0].ubNumMobileGreens > 0)
					{
						pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA]--;
						MilitiaList[0].ubNumMobileGreens--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostMobileGreen;
						uiNumMilitiaDisbanded++;
					}							
					else if (MilitiaList[0].ubNumTownElites > 0)
					{
						pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA]--;
						MilitiaList[0].ubNumTownElites--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostTownElite;
						uiNumMilitiaDisbanded++;
					}
					else if (MilitiaList[0].ubNumTownRegulars > 0)
					{
						pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA]--;
						MilitiaList[0].ubNumTownRegulars--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostTownRegular;
						uiNumMilitiaDisbanded++;
					}
					else if (MilitiaList[0].ubNumTownGreens > 0)
					{
						pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA]--;
						MilitiaList[0].ubNumTownGreens--;
						uiMoneyUnpaid -= gGameExternalOptions.usDailyCostTownGreen;
						uiNumMilitiaDisbanded++;
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
	if (((MILITIA_LIST_TYPE *)pArg1)->ubNumMobileElites > ((MILITIA_LIST_TYPE *)pArg2)->ubNumMobileElites)
	{
		return(-1);
	}
	else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumMobileRegulars > ((MILITIA_LIST_TYPE *)pArg2)->ubNumMobileRegulars)
	{
		return(-1);
	}
	else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumMobileGreens > ((MILITIA_LIST_TYPE *)pArg2)->ubNumMobileGreens)
	{
		return(-1);
	}
	else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumTownElites > ((MILITIA_LIST_TYPE *)pArg2)->ubNumTownElites)
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
		else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumMobileGreens < ((MILITIA_LIST_TYPE *)pArg2)->ubNumMobileGreens)
		{
			return(1);
		}
		else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumMobileRegulars < ((MILITIA_LIST_TYPE *)pArg2)->ubNumMobileRegulars)
		{
			return(1);
		}
		else if (((MILITIA_LIST_TYPE *)pArg1)->ubNumMobileElites < ((MILITIA_LIST_TYPE *)pArg2)->ubNumMobileElites)
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

	for (UINT16 cnt = 0; cnt < 256; cnt++)
	{
		SECTORINFO *pSectorInfo = &( SectorInfo[ cnt ] );
		UINT8 sMapX = SECTORX(cnt);
		UINT8 sMapY = SECTORY(cnt);

		UINT8 ubTownId = GetTownIdForSector( sMapX, sMapY );
		if ((ubTownId != BLANK_SECTOR && MilitiaTrainingAllowedInTown(ubTownId)) || // Major Town
			IsThisSectorASAMSector( sMapX, sMapY, 0 ) ) // SAM Site
		{
			uiTotalPayment += pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA] * gGameExternalOptions.usDailyCostTownGreen;
			uiTotalPayment += pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA] * gGameExternalOptions.usDailyCostTownRegular;
			uiTotalPayment += pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] * gGameExternalOptions.usDailyCostTownElite;
		}
		else
		{
			uiTotalPayment += pSectorInfo->ubNumberOfCivsAtLevel[GREEN_MILITIA] * gGameExternalOptions.usDailyCostMobileGreen;
			uiTotalPayment += pSectorInfo->ubNumberOfCivsAtLevel[REGULAR_MILITIA] * gGameExternalOptions.usDailyCostMobileRegular;
			uiTotalPayment += pSectorInfo->ubNumberOfCivsAtLevel[ELITE_MILITIA] * gGameExternalOptions.usDailyCostMobileElite;
		}
	}

	return (uiTotalPayment);
}