#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include <String.h>
	#include "Strategic Mines.h"
	#include "Finances.h"
	#include "Strategic Town Loyalty.h"
	#include "strategic.h"
	#include "Game Clock.h"
	#include "strategicmap.h"
	#include "Random.h"
	#include "Soldier Profile.h"
	#include "Dialogue Control.h"
	#include "Map Screen Interface.h"
	#include "Quests.h"
	#include "Creature Spreading.h"
	#include "message.h"
	#include "Text.h"
	#include "Game Event Hook.h"
	#include "GameSettings.h"
	#include "Strategic AI.h"
	#include "Campaign Types.h"
	#include "history.h"
	#include "Facilities.h"
	#include "ASD.h"		// added by Flugente
	#include "Rebel Command.h"
#endif

#include "GameInitOptionsScreen.h"

#include "Strategic Mines LUA.h"
#include <vector>

// this .c file will handle the strategic level of mines and income from them


#define REMOVAL_RATE_INCREMENT	250		// the smallest increment by which removal rate change during depletion (use round #s)

#define LOW_MINE_LOYALTY_THRESHOLD	50	// below this the head miner considers his town's population disloyal

// Mine production is being processed 4x daily: 9am ,noon, 3pm, and 6pm.
// This is loosely based on a 6am-6pm working day of 4 "shifts".
#define MINE_PRODUCTION_NUMBER_OF_PERIODS 4						// how many times a day mine production is processed
#define MINE_PRODUCTION_START_TIME				(9 * 60)		// hour of first daily mine production event (in minutes)
#define MINE_PRODUCTION_PERIOD						(3 * 60)		// time seperating daily mine production events (in minutes)


// PRIVATE PROTOTYPES

// mine this mine
INT32 MineAMine( INT8 bMineIndex );

// remove actual ore from mine
UINT32 ExtractOreFromMine( INT8 bMineIndex, UINT32 uiAmount );

// get available workforce for the mine
INT32 GetAvailableWorkForceForMineForPlayer( INT8 bMineIndex );

// get workforce conscripted by enemy for mine
INT32 GetAvailableWorkForceForMineForEnemy( INT8 bMineIndex );

// how fast is the mine's workforce working for you?
INT32 GetCurrentWorkRateOfMineForPlayer( INT8 bMineIndex );

// how fast is workforce working for the enemy
INT32 GetCurrentWorkRateOfMineForEnemy( INT8 bMineIndex );


// DATA TABLES

// this table holds mine values that change during the course of the game and must be saved
//MINE_STATUS_TYPE gMineStatus[ MAX_NUMBER_OF_MINES ];
std::vector<MINE_STATUS_TYPE> gMineStatus;
INT32 MAX_NUMBER_OF_MINES; // equals gMineStatus.size()
std::vector<AssociatedMineSector> associatedMineSectors;

// this table holds mine values that never change and don't need to be saved
//MINE_LOCATION_TYPE gMineLocation[MAX_NUMBER_OF_MINES] =
//{
//	{		4,		4,		SAN_MONA		},
//	{		13,		4,		DRASSEN			},
//	{		14,		9,		ALMA				},
//	{		8,		8,		CAMBRIA			},
//	{		2,		2,		CHITZENA		},
//	{		3,		8,		GRUMM				},
//};

// the are not being randomized at all at this time
//UINT8 gubMineTypes[]={
//	GOLD_MINE,			// SAN MONA
//	SILVER_MINE,		// DRASSEN
//	SILVER_MINE,		// ALMA
//	SILVER_MINE,		// CAMBRIA
//	SILVER_MINE,		// CHITZENA
//	GOLD_MINE,			// GRUMM
//};

// These values also determine the most likely ratios of mine sizes after random production increases are done
//UINT32 guiMinimumMineProduction[]={
//	 0,		// SAN MONA
//	1000,		// DRASSEN
//	1500,		// ALMA
//	1500,		// CAMBRIA
//	500,		// CHITZENA
//	2000,		// GRUMM
//};

std::vector<HEAD_MINER_TYPE> gHeadMinerData;
INT32 NUM_HEAD_MINERS; // equals gHeadMinerData.size()
//HEAD_MINER_TYPE gHeadMinerData[NUM_HEAD_MINERS] =
//{
//	//	Profile #	running out		creatures!		all dead!		creatures again!		external face graphic
//	{	FRED,			17,				18,				27,					26,				MINER_FRED_EXTERNAL_FACE	},
//	{	MATT,			-1,				18,				32,					31,				MINER_MATT_EXTERNAL_FACE	},
//	{	OSWALD,			14,				15,				24,					23,				MINER_OSWALD_EXTERNAL_FACE	},
//	{	CALVIN,			14,				15,				24,					23,				MINER_CALVIN_EXTERNAL_FACE	},
//	{	CARL,			14,				15,				24,					23,				MINER_CARL_EXTERNAL_FACE	},
//};



/* gradual monster infestation concept was ditched, now simply IN PRODUCTION or SHUT DOWN

// percentage of workers working depends on level of mine infestation
UINT8 gubMonsterMineInfestation[]={
	100,
	99,
	95,
	70,
	30,
	1,
	0,
};
*/

// the static NPC dialogue faces
//extern UINT32 uiExternalStaticNPCFaces[];
extern FACETYPE	*gpCurrentTalkingFace;
extern UINT8			gubCurrentTalkingID;



void InitializeMines( void )
{
	INT32 i;
	
	// Don't execute hardcoded logic. Leave initialization to the Lua script.
	g_luaMines.InitializeMines();

	// Buggler: set mining site flag for AI Viewer.cpp
	for (i=0; i < MAX_NUMBER_OF_MINES; i++)
	{
		SectorInfo[ SECTOR( gMineStatus[i].sSectorX, gMineStatus[i].sSectorY ) ].uiFlags |= SF_MINING_SITE;
	}
#if 0
	UINT8 ubMineIndex;
	MINE_STATUS_TYPE *pMineStatus;
	UINT8 ubMineProductionIncreases;
	UINT8 ubDepletedMineIndex;
	UINT8 ubMinDaysBeforeDepletion = 20;


	// set up initial mine status
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		pMineStatus = &(gMineStatus[ ubMineIndex ]);

		memset( pMineStatus, 0, sizeof( *pMineStatus ) );

		pMineStatus->ubMineType = gubMineTypes[ ubMineIndex ];
		pMineStatus->uiMaxRemovalRate = guiMinimumMineProduction[ ubMineIndex ];
		pMineStatus->fEmpty = (pMineStatus->uiMaxRemovalRate == 0) ? TRUE : FALSE;
		pMineStatus->fRunningOut = FALSE;
		pMineStatus->fWarnedOfRunningOut = FALSE;
//		pMineStatus->bMonsters = MINES_NO_MONSTERS;
		pMineStatus->fShutDown = FALSE;
		pMineStatus->fPrevInvadedByMonsters = FALSE;
		pMineStatus->fSpokeToHeadMiner = FALSE;
		pMineStatus->fMineHasProducedForPlayer = FALSE;
		pMineStatus->fQueenRetookProducingMine = FALSE;
		gMineStatus->fShutDownIsPermanent = FALSE;
	}

	// randomize the exact size each mine.	The total production is always the same and depends on the game difficulty,
	// but some mines will produce more in one game than another, while others produce less

	// adjust for game difficulty
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
		case DIF_LEVEL_MEDIUM:
			ubMineProductionIncreases = 25;
			break;
		case DIF_LEVEL_HARD:
			ubMineProductionIncreases = 20;
			break;
		case DIF_LEVEL_INSANE:
			ubMineProductionIncreases = 15;
			break;
		default:
			ubMineProductionIncreases = 25;
			//Assert( 0 );
			return;
	}

	while (ubMineProductionIncreases > 0)
	{
		// pick a producing mine at random and increase its production
		do
		{
			ubMineIndex = ( UINT8 ) Random(MAX_NUMBER_OF_MINES);
		} while (gMineStatus[ubMineIndex].fEmpty);

		// increase mine production by 20% of the base (minimum) rate
		gMineStatus[ubMineIndex].uiMaxRemovalRate += (guiMinimumMineProduction[ubMineIndex] / 5);

		ubMineProductionIncreases--;
	}


	// choose which mine will run out of production.	This will never be the Alma mine or an empty mine (San Mona)...
	do
	{
		ubDepletedMineIndex = ( UINT8 ) Random(MAX_NUMBER_OF_MINES);
		// Alma mine can't run out for quest-related reasons (see Ian)
	} while (gMineStatus[ubDepletedMineIndex].fEmpty || (ubDepletedMineIndex == MINE_ALMA));

	// HEADROCK HAM 3.1: We can now select which mine runs out, or disable mine shutdown.
	// Make sure selection isn't 0 (no mine runs out) or invalid (San Mona, Alma)
	if (gGameExternalOptions.bWhichMineRunsOut > 0 && 
		gGameExternalOptions.bWhichMineRunsOut < MAX_NUMBER_OF_MINES &&
		gGameExternalOptions.bWhichMineRunsOut-1 != MINE_SAN_MONA &&
		gGameExternalOptions.bWhichMineRunsOut-1 != MINE_ALMA )
	{
		// Set depleted mine index.
		ubDepletedMineIndex = (gGameExternalOptions.bWhichMineRunsOut-1);
	}

	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		pMineStatus = &(gMineStatus[ ubMineIndex ]);

		// HEADROCK HAM 3.1: We can disable mine shutdown, so make sure the settings aren't telling us to do so.
		if (gGameExternalOptions.bWhichMineRunsOut != 0 && ubMineIndex == ubDepletedMineIndex)
		{
			if ( ubDepletedMineIndex == MINE_DRASSEN )
			{
				ubMinDaysBeforeDepletion = 20;
			}
			else
			{
				ubMinDaysBeforeDepletion = 10;
			}

			// the mine that runs out has only enough ore for this many days of full production
			pMineStatus->uiRemainingOreSupply = ubMinDaysBeforeDepletion * (MINE_PRODUCTION_NUMBER_OF_PERIODS * pMineStatus->uiMaxRemovalRate);

			// ore starts running out when reserves drop to less than 25% of the initial supply
			pMineStatus->uiOreRunningOutPoint = pMineStatus->uiRemainingOreSupply / 4;
		}
		else
		if (!pMineStatus->fEmpty)
		{
			// never runs out...
			pMineStatus->uiRemainingOreSupply = 999999999;		// essentially unlimited
			pMineStatus->uiOreRunningOutPoint = 0;
		}
		else
		{
			// already empty
			pMineStatus->uiRemainingOreSupply = 0;
			pMineStatus->uiOreRunningOutPoint = 0;
		}
	}
#endif
}


void HourlyMinesUpdate(void)
{
	UINT8 ubMineIndex;
	MINE_STATUS_TYPE *pMineStatus;
	UINT8 ubQuoteType;


	// check every non-empty mine
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		pMineStatus = &(gMineStatus[ ubMineIndex ]);

		if (pMineStatus->fEmpty)
		{
			// nobody is working that mine, so who cares
			continue;
		}

		// check if the mine has any monster creatures in it
		if (MineClearOfMonsters( ubMineIndex ))
		{
			// if it's shutdown, but not permanently
			if (IsMineShutDown( ubMineIndex ) && !pMineStatus->fShutDownIsPermanent)
			{
				// if we control production in it
				if (PlayerControlsMine( ubMineIndex ))
				{
					IssueHeadMinerQuote( ubMineIndex, HEAD_MINER_STRATEGIC_QUOTE_CREATURES_GONE );
				}

				//Force the creatures to avoid the mine for a period of time.	This gives the
				//player a chance to rest and decide how to deal with the problem.
				ForceCreaturesToAvoidMineTemporarily( ubMineIndex );

				// put mine back in service
				RestartMineProduction( ubMineIndex );
			}
		}
		else	// mine is monster infested
		{
			// 'Der be monsters crawling around in there, lad!!!

			// if it's still producing
			if (!IsMineShutDown( ubMineIndex ))
			{
				// gotta put a stop to that!

				// if we control production in it
				if (PlayerControlsMine( ubMineIndex ))
				{
					// 2 different quotes, depends whether or not it's the first time this has happened
					if (pMineStatus->fPrevInvadedByMonsters)
					{
						ubQuoteType = HEAD_MINER_STRATEGIC_QUOTE_CREATURES_AGAIN;
					}
					else
					{
						ubQuoteType = HEAD_MINER_STRATEGIC_QUOTE_CREATURES_ATTACK;
						pMineStatus->fPrevInvadedByMonsters = TRUE;

						if ( gubQuest[ QUEST_CREATURES ] == QUESTNOTSTARTED )
						{
							// start it now!
							//StartQuest( QUEST_CREATURES, gMineLocation[ ubMineIndex ].sSectorX, gMineLocation[ ubMineIndex ].sSectorY );
							StartQuest( QUEST_CREATURES, gMineStatus[ ubMineIndex ].sSectorX, gMineStatus[ ubMineIndex ].sSectorY );
						}
					}

					// tell player the good news...
					IssueHeadMinerQuote( ubMineIndex, ubQuoteType );
				}

				// and immediately halt all work at the mine (whether it's ours or the queen's).	This is a temporary shutdown
				ShutOffMineProduction( ubMineIndex );
			}
		}
	}
}


INT32 GetTotalLeftInMine( INT8 bMineIndex )
{
	// returns the value of the mine

	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	return ( gMineStatus[ bMineIndex ].uiRemainingOreSupply );
}


UINT32 GetMaxPeriodicRemovalFromMine( INT8 bMineIndex )
{
	// returns max amount that can be mined in a time period

	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// if mine is shut down
	if ( gMineStatus[ bMineIndex ].fShutDown)
	{
		return ( 0 );
	}

	// HEADROCK HAM B1: Affected by external INI option.
	return(( gMineStatus[ bMineIndex ].uiMaxRemovalRate * gGameExternalOptions.usMineIncomePercentage ) / 100);
}


UINT32 GetMaxDailyRemovalFromMine( INT8 bMineIndex )
{
	UINT32 uiAmtExtracted;

	// returns max amount that can be mined in one day

	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// if mine is shut down
	if ( gMineStatus[ bMineIndex ].fShutDown)
	{
		return ( 0 );
	}

	uiAmtExtracted = MINE_PRODUCTION_NUMBER_OF_PERIODS * gMineStatus[ bMineIndex ].uiMaxRemovalRate;

	// check if we will take more than there is
	if( uiAmtExtracted > gMineStatus[ bMineIndex ].uiRemainingOreSupply )
	{
		// yes, reduce to value of mine
		uiAmtExtracted = gMineStatus[ bMineIndex ].uiRemainingOreSupply;
	}
	// HEADROCK HAM B1: Affected by external INI option.
	uiAmtExtracted = (uiAmtExtracted * gGameExternalOptions.usMineIncomePercentage) / 100;

	return(uiAmtExtracted);
}


INT8 GetTownAssociatedWithMine( INT8 bMineIndex )
{
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	//return ( gMineLocation[ bMineIndex ].bAssociatedTown );
	return ( gMineStatus[ bMineIndex ].bAssociatedTown );
}


UINT32 ExtractOreFromMine( INT8 bMineIndex, UINT32 uiAmount )
{
	// will remove the ore from the mine and return the amount that was removed
	UINT32 uiAmountExtracted = 0;
	INT16 sSectorX, sSectorY;


	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// if mine is shut down
	if ( gMineStatus[ bMineIndex ].fShutDown)
	{
		return ( 0 );
	}

	// if not capable of extracting anything, bail now
	if (uiAmount == 0)
	{
		return ( 0 );
	}

	// will this exhaust the ore in this mine?
	if( uiAmount >= gMineStatus[ bMineIndex ].uiRemainingOreSupply )
	{
		// exhaust remaining ore
		uiAmountExtracted = gMineStatus[ bMineIndex ].uiRemainingOreSupply;
		gMineStatus[ bMineIndex ].uiRemainingOreSupply = 0;
		gMineStatus[ bMineIndex ].uiMaxRemovalRate = 0;
		gMineStatus[ bMineIndex ].fEmpty = TRUE;
		gMineStatus[ bMineIndex ].fRunningOut = FALSE;

		// tell the strategic AI about this, that mine's and town's value is greatly reduced
		GetMineSector( bMineIndex, &sSectorX, &sSectorY );
		StrategicHandleMineThatRanOut( ( UINT8 ) SECTOR( sSectorX, sSectorY ) );

		//AddHistoryToPlayersLog( HISTORY_MINE_RAN_OUT, gMineLocation[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineLocation[ bMineIndex ].sSectorX,	gMineLocation[ bMineIndex ].sSectorY );
		AddHistoryToPlayersLog( HISTORY_MINE_RAN_OUT, gMineStatus[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineStatus[ bMineIndex ].sSectorX,	gMineStatus[ bMineIndex ].sSectorY );
	}
	else	// still some left after this extraction
	{
		// set amount used, and decrement ore remaining in mine
		uiAmountExtracted = uiAmount;
		gMineStatus[ bMineIndex ].uiRemainingOreSupply -= uiAmount;

		// one of the mines (randomly chosen) will start running out eventually, check if we're there yet
		if (gMineStatus[ bMineIndex ].uiRemainingOreSupply < gMineStatus[ bMineIndex ].uiOreRunningOutPoint)
		{
			gMineStatus[ bMineIndex ].fRunningOut = TRUE;

			// round all fractions UP to the next REMOVAL_RATE_INCREMENT
			gMineStatus[ bMineIndex ].uiMaxRemovalRate = (UINT32) (((FLOAT) gMineStatus[ bMineIndex ].uiRemainingOreSupply / 10) / REMOVAL_RATE_INCREMENT + 0.9999) * REMOVAL_RATE_INCREMENT;


			// if we control it
			if (PlayerControlsMine(bMineIndex))
			{
				// and haven't yet been warned that it's running out
				if (!gMineStatus[ bMineIndex ].fWarnedOfRunningOut)
				{
					// that mine's head miner tells player that the mine is running out
					IssueHeadMinerQuote( bMineIndex, HEAD_MINER_STRATEGIC_QUOTE_RUNNING_OUT );
					gMineStatus[ bMineIndex ].fWarnedOfRunningOut = TRUE;
					//AddHistoryToPlayersLog( HISTORY_MINE_RUNNING_OUT, gMineLocation[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineLocation[ bMineIndex ].sSectorX,	gMineLocation[ bMineIndex ].sSectorY );
					AddHistoryToPlayersLog( HISTORY_MINE_RUNNING_OUT, gMineStatus[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineStatus[ bMineIndex ].sSectorX,	gMineStatus[ bMineIndex ].sSectorY );
				}
			}
		}
	}

	return( uiAmountExtracted );
}

INT32 GetAvailableWorkForceForMineForPlayer( INT8 bMineIndex )
{
	// look for available workforce in the town associated with the mine
	INT32 iWorkForceSize = 0;
	INT8 bTownId = 0;

	// return the loyalty of the town associated with the mine

	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// if mine is shut down
	if ( gMineStatus[ bMineIndex ].fShutDown)
	{
		return ( 0 );
	}

	// until the player contacts the head miner, production in mine ceases if in player's control
	if ( !gMineStatus[ bMineIndex ].fSpokeToHeadMiner)
	{
		return ( 0 );
	}

	//bTownId = gMineLocation[ bMineIndex ].bAssociatedTown;
	bTownId = gMineStatus[ bMineIndex ].bAssociatedTown;

	Assert ( GetTownSectorSize( bTownId ) != 0 );

	// get workforce size (is 0-100 based on local town's loyalty)
	iWorkForceSize = gTownLoyalty[ bTownId ].ubRating;

/*
	// adjust for monster infestation
	iWorkForceSize *= gubMonsterMineInfestation[ gMineStatus[ bMineIndex ].bMonsters ];
	iWorkForceSize /= 100;
*/

	// Flugente: if strategic disease is active, disease can affect out workforce, so the calculation gets slightly more complicated
	iWorkForceSize = (INT32)(iWorkForceSize * GetWorkforceEffectivenessWithDisease(bTownId, OUR_TEAM));

	// now adjust for town size.. the number of sectors you control
	iWorkForceSize *= GetTownSectorsUnderControl( bTownId );
	iWorkForceSize /= GetTownSectorSize( bTownId );

	// Flugente: workforce
	if ( gGameExternalOptions.fMineRequiresWorkers )
	{
		UINT16 maxworkforce = 0;
		UINT16 workforce = GetTownWorkers( bTownId, maxworkforce);

		if ( maxworkforce > 0 )
		{
			iWorkForceSize *= workforce;
			iWorkForceSize /= maxworkforce;
		}
	}

	return ( iWorkForceSize );
}

INT32 GetAvailableWorkForceForMineForEnemy( INT8 bMineIndex )
{
	// look for available workforce in the town associated with the mine
	INT32 iWorkForceSize = 0;
	INT8 bTownId = 0;

	// return the loyalty of the town associated with the mine

	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// if mine is shut down
	if ( gMineStatus[ bMineIndex ].fShutDown)
		return ( 0 );

	bTownId = gMineStatus[ bMineIndex ].bAssociatedTown;

	UINT8 townsize = GetTownSectorSize( bTownId );

	if ( !townsize )
		return 0;

	// get workforce size (is 0-100 based on REVERSE of local town's loyalty)
	iWorkForceSize = 100 - gTownLoyalty[ bTownId ].ubRating;

/*
	// adjust for monster infestation
	iWorkForceSize *= gubMonsterMineInfestation[ gMineStatus[ bMineIndex ].bMonsters ];
	iWorkForceSize /= 100;
*/

	// now adjust for town size.. the number of sectors you control
	iWorkForceSize *= (townsize - GetTownSectorsUnderControl( bTownId ));
	iWorkForceSize /= townsize;

	return iWorkForceSize;
}

INT32 GetCurrentWorkRateOfMineForPlayer( INT8 bMineIndex )
{
	INT32 iWorkRate = 0;

	// multiply maximum possible removal rate by the percentage of workforce currently working
	iWorkRate = (gMineStatus[ bMineIndex ].uiMaxRemovalRate * GetAvailableWorkForceForMineForPlayer( bMineIndex )) / 100;

	return( iWorkRate );
}

INT32 GetCurrentWorkRateOfMineForEnemy( INT8 bMineIndex )
{
	INT32 iWorkRate = 0;

	// multiply maximum possible removal rate by the percentage of workforce currently working
	iWorkRate = (gMineStatus[ bMineIndex ].uiMaxRemovalRate * GetAvailableWorkForceForMineForEnemy( bMineIndex )) / 100;

	return( iWorkRate );
}



INT32 MineAMine( INT8 bMineIndex )
{
	// will extract ore based on available workforce, and increment players income based on amount
	INT8 bMineType = 0;
	INT32 iAmtExtracted = 0;
	
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// is mine is empty
	if( gMineStatus[ bMineIndex ].fEmpty)
	{
		return 0;
	}

	// if mine is shut down
	if ( gMineStatus[ bMineIndex ].fShutDown)
	{
		return 0;
	}

	// who controls the PRODUCTION in the mine ?	(Queen receives production unless player has spoken to the head miner)
	if( PlayerControlsMine(bMineIndex) )
	{
		// player controlled
		iAmtExtracted = ExtractOreFromMine( bMineIndex , GetCurrentWorkRateOfMineForPlayer( bMineIndex ) );

		// SHOW ME THE MONEY!!!!
		if( iAmtExtracted > 0 )
		{
			// debug message
//			ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"%s - Mine income from %s = $%d", WORLDTIMESTR, pTownNames[ GetTownAssociatedWithMine( bMineIndex ) ], iAmtExtracted );

			// check type of mine
			bMineType = gMineStatus[ bMineIndex ].ubMineType;

			// if this is the first time this mine has produced income for the player in the game
			if ( !gMineStatus[ bMineIndex ].fMineHasProducedForPlayer )
			{
				// remember that we've earned income from this mine during the game
				gMineStatus[ bMineIndex ].fMineHasProducedForPlayer = TRUE;
				// and when we started to do so...
				gMineStatus[ bMineIndex ].uiTimePlayerProductionStarted = GetWorldTotalMin();
			}
		}
	}
	else	// queen controlled
	{
		// we didn't want mines to run out without player ever even going to them, so now the queen doesn't reduce the
		// amount remaining until the mine has produced for the player first (so she'd have to capture it).

		/*// WANNE: We do not want to give money to the player, when the queen has captured the mine!
		if ( gMineStatus[ bMineIndex ].fMineHasProducedForPlayer )
		{
			// don't actually give her money, just take production away
			//iAmtExtracted = ExtractOreFromMine( bMineIndex , GetCurrentWorkRateOfMineForEnemy( bMineIndex ) );
			iAmtExtracted = 0;
		}*/

		// Flugente: if the enemy controls a mine, simply don't lower the amount of ore in the mine, so it can still run out when the player controls it
		iAmtExtracted = GetCurrentWorkRateOfMineForEnemy( bMineIndex );
	}


	return iAmtExtracted;
}


void PostEventsForMineProduction(void)
{
	UINT8 ubShift;

	for (ubShift = 0; ubShift < MINE_PRODUCTION_NUMBER_OF_PERIODS; ubShift++)
	{
		AddStrategicEvent( EVENT_HANDLE_MINE_INCOME, GetWorldDayInMinutes() + MINE_PRODUCTION_START_TIME + (ubShift * MINE_PRODUCTION_PERIOD), 0 );
	}
}


void HandleIncomeFromMines( void )
{
	INT32 iIncome = 0;
	INT32 iIncome_Enemy = 0;	// Flugente: new AI gets money from mines

	INT32 iModifier = 0;

	// mine each mine, check if we own it and such
	for ( INT8 bCounter = 0; bCounter < MAX_NUMBER_OF_MINES; ++bCounter )
	{
		if ( PlayerControlsMine( bCounter ) )
		{
			// HEADROCK HAM 3.6: Add facility modifier as a percentage
			// add rebel command bonus
			iModifier = 100 + MineIncomeModifierFromFacility( bCounter ) + RebelCommand::GetMiningPolicyBonus(gMineStatus[bCounter].bAssociatedTown);

			// mine this mine
			iIncome += (MineAMine( bCounter ) * iModifier) / 100;

		}
		else
		{
			// mine this mine
			iIncome_Enemy += MineAMine( bCounter );
		}
	}

	// HEADROCK HAM B1: Affected by external INI Option.
	iIncome = (iIncome * gGameExternalOptions.usMineIncomePercentage) / 100;
	iIncome_Enemy = (iIncome_Enemy * gGameExternalOptions.usMineIncomePercentage) / 100;

	RebelCommand::RaidMines(iIncome, iIncome_Enemy);

	if( iIncome )
	{
		AddTransactionToPlayersBook( DEPOSIT_FROM_SILVER_MINE, 0, GetWorldTotalMin( ), iIncome );
	}

	if ( iIncome_Enemy )
	{
		AddStrategicAIResources( ASD_MONEY, iIncome_Enemy );
	}
}


UINT32 PredictDailyIncomeFromAMine( INT8 bMineIndex, BOOLEAN fIncludeFacilities )
{
	// predict income from this mine, estimate assumes mining situation will not change during next 4 income periods
	// (miner loyalty, % town controlled, monster infestation level, and current max removal rate may all in fact change)
	UINT32 uiAmtExtracted = 0;
	// HEADROCK HAM 3.6: Facilities can now modify income
	INT32 iModifier = 0;

	if(PlayerControlsMine(bMineIndex))
	{
		// get daily income for this mine (regardless of what time of day it currently is)
		uiAmtExtracted = MINE_PRODUCTION_NUMBER_OF_PERIODS * GetCurrentWorkRateOfMineForPlayer( bMineIndex );

		// check if we will take more than there is
		if( uiAmtExtracted > gMineStatus[ bMineIndex ].uiRemainingOreSupply )
		{
			// yes reduce to value of mine
			uiAmtExtracted = gMineStatus[ bMineIndex ].uiRemainingOreSupply;
		}
		if (bMineIndex && fIncludeFacilities)
		{
			// HEADROCK HAM 3.6: Facility modifier applied as a percentage
			// add rebel command bonus
			iModifier = 100 + MineIncomeModifierFromFacility( bMineIndex ) + RebelCommand::GetMiningPolicyBonus(gMineStatus[bMineIndex].bAssociatedTown);
			uiAmtExtracted = (uiAmtExtracted * iModifier) / 100;
		}
	}

	// HEADROCK HAM B1: Affected by external INI Option.
	return( ( uiAmtExtracted * gGameExternalOptions.usMineIncomePercentage ) / 100 ) ;
}


INT32 PredictIncomeFromPlayerMines( BOOLEAN fIncludeFacilities )
{
	INT32 iTotal = 0;
	INT8 bCounter = 0;

	for( bCounter = 0; bCounter < MAX_NUMBER_OF_MINES; bCounter++ )
	{
		// add up the total
		iTotal += PredictDailyIncomeFromAMine( bCounter, fIncludeFacilities );
	}

	return( iTotal );
}


INT32 CalcMaxPlayerIncomeFromMines( void )
{
	INT32 iTotal = 0;
	INT8 bCounter = 0;

	// calculate how much player could make daily if he owned all mines with 100% control and 100% loyalty
	for( bCounter = 0; bCounter < MAX_NUMBER_OF_MINES; bCounter++ )
	{
		// add up the total
		iTotal += (MINE_PRODUCTION_NUMBER_OF_PERIODS * gMineStatus[bCounter].uiMaxRemovalRate);
	}
	// HEADROCK HAM B1: Affected by external INI Option.
	iTotal = (iTotal * gGameExternalOptions.usMineIncomePercentage) / 100;

	return( iTotal );
}


// get index of this mine, return -1 if no mine found
INT8 GetMineIndexForSector( INT16 sX, INT16 sY )
{
	UINT8 ubMineIndex = 0;


	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		//if( ( gMineLocation[ ubMineIndex ].sSectorX == sX ) && ( gMineLocation[ ubMineIndex ].sSectorY == sY ) )
		if( ( gMineStatus[ ubMineIndex ].sSectorX == sX ) && ( gMineStatus[ ubMineIndex ].sSectorY == sY ) )
		{
			// yep mine here
			return( ubMineIndex );
		}
	}

	return( -1 );
}

void GetMineSector( UINT8 ubMineIndex, INT16 * psX, INT16 * psY )
{
	Assert(ubMineIndex < MAX_NUMBER_OF_MINES);

	//*psX = gMineLocation[ ubMineIndex ].sSectorX;
	//*psY = gMineLocation[ ubMineIndex ].sSectorY;
	*psX = gMineStatus[ ubMineIndex ].sSectorX;
	*psY = gMineStatus[ ubMineIndex ].sSectorY;
}


// get the index of the mine associated with this town
INT8 GetMineIndexForTown( INT8 bTownId )
{
	UINT8 ubMineIndex = 0;

	// given town id, send sector value of mine, a 0 means no mine for this town
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		//if( gMineLocation[ ubMineIndex ].bAssociatedTown == bTownId )
		if( gMineStatus[ ubMineIndex ].bAssociatedTown == bTownId )
		{
			return( ubMineIndex );
		}
	}

	return( -1 );
}


// get the sector value for the mine associated with this town
INT16 GetMineSectorForTown( INT8 bTownId )
{
	INT8 ubMineIndex;
	INT16 sMineSector = -1;

	// given town id, send sector value of mine, a 0 means no mine for this town
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		//if( gMineLocation[ ubMineIndex ].bAssociatedTown == bTownId )
		if( gMineStatus[ ubMineIndex ].bAssociatedTown == bTownId )
		{
			//sMineSector = gMineLocation[ ubMineIndex ].sSectorX + ( gMineLocation[ ubMineIndex ].sSectorY * MAP_WORLD_X );
			sMineSector = CALCULATE_STRATEGIC_INDEX(gMineStatus[ ubMineIndex ].sSectorX, gMineStatus[ ubMineIndex ].sSectorY );
			break;
		}
	}

	// -1 returned if the town doesn't have a mine
	return( sMineSector );
}


BOOLEAN IsThereAMineInThisSector( INT16 sX, INT16 sY )
{
	UINT8 ubMineIndex;

	// run through the list...if a mine here, great
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		//if( ( gMineLocation[ ubMineIndex ].sSectorX == sX ) && ( gMineLocation[ ubMineIndex ].sSectorY == sY ) )
		if( ( gMineStatus[ ubMineIndex ].sSectorX == sX ) && ( gMineStatus[ ubMineIndex ].sSectorY == sY ) )
		{
			return( TRUE );
		}
	}
	return( FALSE );
}


BOOLEAN PlayerControlsMine(INT8 bMineIndex)
{
	// a value of TRUE is from the enemy's point of view
	//if (StrategicMap[( gMineLocation[ bMineIndex ].sSectorX ) + ( MAP_WORLD_X * ( gMineLocation[ bMineIndex ].sSectorY ) )].fEnemyControlled == TRUE )
	if (StrategicMap[CALCULATE_STRATEGIC_INDEX( gMineStatus[bMineIndex].sSectorX, gMineStatus[bMineIndex].sSectorY )].fEnemyControlled == TRUE )
		return(FALSE);
	else
	{
		// player only controls the actual mine after he has made arrangements to do so with the head miner there
		if (gMineStatus[ bMineIndex ].fSpokeToHeadMiner)
		{
			return(TRUE);
		}
		else
		{
			return(FALSE);
		}
	}
}


BOOLEAN SaveMineStatusToSaveGameFile( HWFILE hFile )
{
	return g_luaMines.Save(hFile);

#if 0
	UINT32	uiNumBytesWritten;

	//Save the MineStatus
	FileWrite( hFile, gMineStatus, sizeof( MINE_STATUS_TYPE ) * MAX_NUMBER_OF_MINES, &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( MINE_STATUS_TYPE ) * MAX_NUMBER_OF_MINES )
	{
		return( FALSE );
	}

	return( TRUE );
#endif
}


BOOLEAN LoadMineStatusFromSavedGameFile( HWFILE hFile )
{
	return g_luaMines.Load(hFile);

#if 0
	UINT32	uiNumBytesRead;

	//Load the MineStatus
	FileRead( hFile, gMineStatus, sizeof( MINE_STATUS_TYPE ) * MAX_NUMBER_OF_MINES, &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( MINE_STATUS_TYPE ) * MAX_NUMBER_OF_MINES )
	{
		return( FALSE );
	}

	return( TRUE );
#endif
}


void ShutOffMineProduction( INT8 bMineIndex )
{
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	if ( !gMineStatus[ bMineIndex ].fShutDown )
	{
		gMineStatus[ bMineIndex ].fShutDown = TRUE;
		//AddHistoryToPlayersLog( HISTORY_MINE_SHUTDOWN, gMineLocation[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineLocation[ bMineIndex ].sSectorX,	gMineLocation[ bMineIndex ].sSectorY );
		AddHistoryToPlayersLog( HISTORY_MINE_SHUTDOWN, gMineStatus[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineStatus[ bMineIndex ].sSectorX,	gMineStatus[ bMineIndex ].sSectorY );
	}
}


void RestartMineProduction( INT8 bMineIndex )
{
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	if ( !gMineStatus[ bMineIndex ].fShutDownIsPermanent )
	{
		if ( gMineStatus[ bMineIndex ].fShutDown )
		{
			gMineStatus[ bMineIndex ].fShutDown = FALSE;
			//AddHistoryToPlayersLog( HISTORY_MINE_REOPENED, gMineLocation[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineLocation[ bMineIndex ].sSectorX,	gMineLocation[ bMineIndex ].sSectorY );
			AddHistoryToPlayersLog( HISTORY_MINE_REOPENED, gMineStatus[ bMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineStatus[ bMineIndex ].sSectorX,	gMineStatus[ bMineIndex ].sSectorY );
		}
	}
}


void MineShutdownIsPermanent( INT8 bMineIndex )
{
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	gMineStatus[ bMineIndex ].fShutDownIsPermanent = TRUE;
}


BOOLEAN IsMineShutDown( INT8 bMineIndex )
{
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	return(gMineStatus[ bMineIndex ].fShutDown);
}


UINT8 GetHeadMinerIndexForMine( INT8 bMineIndex )
{
	UINT8 ubMinerIndex = 0;
	UINT16 usProfileId = 0;


	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	// loop through all head miners, checking which town they're associated with, looking for one that matches this mine
	for (ubMinerIndex = 0; ubMinerIndex < NUM_HEAD_MINERS; ubMinerIndex++)
	{
		usProfileId = gHeadMinerData[ ubMinerIndex ].usProfileId;

		//if (gMercProfiles[ usProfileId ].bTown == gMineLocation[ bMineIndex ].bAssociatedTown)
		if (gMercProfiles[ usProfileId ].bTown == gMineStatus[ bMineIndex ].bAssociatedTown)
		{
			return(ubMinerIndex);
		}
	}

	// not found - yack!
	//Assert( FALSE );
	//return( 0 );

	// Do not assert but use an error code instead,
	// as a mod might intentionally want to not have a head miner at every mine.
	return -1;
}


//UINT16 GetHeadMinerProfileIdForMine( INT8 bMineIndex )
//{
//	return(gHeadMinerData[ GetHeadMinerIndexForMine( bMineIndex ) ].usProfileId);
//}


void IssueHeadMinerQuote( INT8 bMineIndex, UINT8 ubQuoteType )
{
	UINT8 ubHeadMinerIndex = 0;
	UINT16 usHeadMinerProfileId = 0;
	INT8 bQuoteNum = 0;
	UINT8 ubFaceIndex = 0;
	BOOLEAN fForceMapscreen = FALSE;
	INT16 sXPos, sYPos;


	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );
	Assert( ubQuoteType < NUM_HEAD_MINER_STRATEGIC_QUOTES );
	Assert( CheckFact( FACT_MINERS_PLACED, 0 ) );

	ubHeadMinerIndex = GetHeadMinerIndexForMine( bMineIndex );
	if (ubHeadMinerIndex == -1)
		return;
	usHeadMinerProfileId = gHeadMinerData[ ubHeadMinerIndex ].usProfileId;

	// make sure the miner ain't dead
	if (gMercProfiles[ usHeadMinerProfileId ].bLife < OKLIFE)
	{
		// debug message
		ScreenMsg( MSG_FONT_RED, MSG_DEBUG, L"Head Miner #%s can't talk (quote #%d)", gMercProfiles[ usHeadMinerProfileId ].zNickname, ubQuoteType);
		return;
	}

	bQuoteNum = gHeadMinerData[ ubHeadMinerIndex ].bQuoteNum[ ubQuoteType ];
	//Assert( bQuoteNum != -1 );
	if (bQuoteNum == -1)
		return;

	ubFaceIndex = ( UINT8 ) uiExternalStaticNPCFaces[ gHeadMinerData[ ubHeadMinerIndex ].ubExternalFaceIndex ];

	// transition to mapscreen is not necessary for "creatures gone" quote - player is IN that mine, so he'll know
	if ( ubQuoteType != HEAD_MINER_STRATEGIC_QUOTE_CREATURES_GONE )
	{
		fForceMapscreen = TRUE;
	}


	// decide where the miner's face and text box should be positioned in order to not obscure the mine he's in as it flashes
	switch ( bMineIndex )
	{
		case MINE_GRUMM:
			sXPos = DEFAULT_EXTERN_PANEL_X_POS;
			sYPos = DEFAULT_EXTERN_PANEL_Y_POS;
			break;
		case MINE_CAMBRIA:
			sXPos = DEFAULT_EXTERN_PANEL_X_POS;
			sYPos = DEFAULT_EXTERN_PANEL_Y_POS;
			break;
		case MINE_ALMA:
			sXPos = DEFAULT_EXTERN_PANEL_X_POS;
			sYPos = DEFAULT_EXTERN_PANEL_Y_POS;
			break;
		case MINE_DRASSEN:
			sXPos = DEFAULT_EXTERN_PANEL_X_POS;
			sYPos = 135;
			break;
		case MINE_CHITZENA:
			sXPos = DEFAULT_EXTERN_PANEL_X_POS;
			sYPos = 117;
			break;

		// there's no head miner in San Mona, this is an error!
		case MINE_SAN_MONA:
		default:
			//Assert( FALSE );
			sXPos = DEFAULT_EXTERN_PANEL_X_POS;
			sYPos = DEFAULT_EXTERN_PANEL_Y_POS;
			break;
	}

	SetExternMapscreenSpeechPanelXY( sXPos, sYPos );

	// cause this quote to come up for this profile id and an indicator to flash over the mine sector
	//HandleMinerEvent( gHeadMinerData[ ubHeadMinerIndex ].ubExternalFace, gMineLocation[ bMineIndex ].sSectorX, gMineLocation[ bMineIndex ].sSectorY, (INT16) bQuoteNum, fForceMapscreen );
	//HandleMinerEvent( gHeadMinerData[ ubHeadMinerIndex ].ubExternalFace, gMineStatus[ bMineIndex ].sSectorX, gMineStatus[ bMineIndex ].sSectorY, (INT16) bQuoteNum, fForceMapscreen );
	HandleMinerEvent( ubHeadMinerIndex, gMineStatus[ bMineIndex ].sSectorX, gMineStatus[ bMineIndex ].sSectorY, (INT16) bQuoteNum, fForceMapscreen );

	// stop time compression with any miner quote - these are important events.
	StopTimeCompression();
}


UINT8 GetHeadMinersMineIndex( UINT8 ubMinerProfileId)
{
	UINT8 ubMineIndex;

	// find which mine this guy represents
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		//if (gMineLocation[ ubMineIndex ].bAssociatedTown == gMercProfiles[ ubMinerProfileId ].bTown)
		if (gMineStatus[ ubMineIndex ].bAssociatedTown == gMercProfiles[ ubMinerProfileId ].bTown)
		{
			return(ubMineIndex);
		}
	}

	// not found!	Illegal profile id receieved or something is very wrong
	Assert(FALSE);
	return( 0 );
}


void PlayerSpokeToHeadMiner( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );

	// if this is our first time set a history fact
	if( gMineStatus[ ubMineIndex ].fSpokeToHeadMiner == FALSE )
	{
		//AddHistoryToPlayersLog( HISTORY_TALKED_TO_MINER, gMineLocation[ ubMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineLocation[ ubMineIndex ].sSectorX,	gMineLocation[ ubMineIndex ].sSectorY );
		AddHistoryToPlayersLog( HISTORY_TALKED_TO_MINER, gMineStatus[ ubMineIndex ].bAssociatedTown, GetWorldTotalMin( ), gMineStatus[ ubMineIndex ].sSectorX,	gMineStatus[ ubMineIndex ].sSectorY );
		gMineStatus[ ubMineIndex ].fSpokeToHeadMiner = TRUE;
	}
}


BOOLEAN IsHisMineRunningOut( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );
	return(gMineStatus[ ubMineIndex ].fRunningOut);
}

BOOLEAN IsHisMineEmpty( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );
	return(gMineStatus[ ubMineIndex ].fEmpty);
}

BOOLEAN IsHisMineDisloyal( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );

	//if (gTownLoyalty[ gMineLocation[ ubMineIndex ].bAssociatedTown ].ubRating < LOW_MINE_LOYALTY_THRESHOLD)
	if (gTownLoyalty[ gMineStatus[ ubMineIndex ].bAssociatedTown ].ubRating < LOW_MINE_LOYALTY_THRESHOLD)
	{
		// pretty disloyal
		return(TRUE);
	}
	else
	{
		// pretty loyal
		return(FALSE);
	}
}

BOOLEAN IsHisMineInfested( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );
	return(!MineClearOfMonsters( ubMineIndex ));
}

BOOLEAN IsHisMineLostAndRegained( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );

	if (PlayerControlsMine(ubMineIndex) && gMineStatus[ ubMineIndex ].fQueenRetookProducingMine)
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

void ResetQueenRetookMine( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );

	gMineStatus[ ubMineIndex ].fQueenRetookProducingMine = FALSE;
}

BOOLEAN IsHisMineAtMaxProduction( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );

	if (GetAvailableWorkForceForMineForPlayer( ubMineIndex ) == 100)
	{
		// loyalty is 100% and control is 100%
		return(TRUE);
	}
	else
	{
		// something not quite perfect yet
		return(FALSE);
	}
}


void QueenHasRegainedMineSector(INT8 bMineIndex)
{
	Assert( ( bMineIndex >= 0 ) && ( bMineIndex < MAX_NUMBER_OF_MINES ) );

	if (gMineStatus[ bMineIndex ].fMineHasProducedForPlayer)
	{
		gMineStatus[ bMineIndex ].fQueenRetookProducingMine = TRUE;
	}
}


BOOLEAN HasAnyMineBeenAttackedByMonsters(void)
{
	UINT8 ubMineIndex;

	// find which mine this guy represents
	for( ubMineIndex = 0; ubMineIndex < MAX_NUMBER_OF_MINES; ubMineIndex++ )
	{
		if (!MineClearOfMonsters( ubMineIndex ) || gMineStatus[ ubMineIndex ].fPrevInvadedByMonsters)
		{
			return(TRUE);
		}
	}

	return(FALSE);
}


void PlayerAttackedHeadMiner( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;
	INT8 bTownId;

	// get the index of his mine
	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );


	// if it's the first time he's been attacked
	if ( gMineStatus[ ubMineIndex ].fAttackedHeadMiner == FALSE )
	{
		// shut off production at his mine (Permanently!)
		ShutOffMineProduction( ubMineIndex );
		MineShutdownIsPermanent( ubMineIndex );

		// get the index of his town
		bTownId = GetTownAssociatedWithMine( ubMineIndex );
		// penalize associated town's loyalty
		DecrementTownLoyalty( bTownId, LOYALTY_PENALTY_HEAD_MINER_ATTACKED );

		// don't allow this more than once
		gMineStatus[ ubMineIndex ].fAttackedHeadMiner = TRUE;
	}
}


BOOLEAN HasHisMineBeenProducingForPlayerForSomeTime( UINT8 ubMinerProfileId )
{
	UINT8 ubMineIndex;

	ubMineIndex = GetHeadMinersMineIndex( ubMinerProfileId );

	if ( gMineStatus[ ubMineIndex ].fMineHasProducedForPlayer &&
			( ( GetWorldTotalMin() - gMineStatus[ ubMineIndex ].uiTimePlayerProductionStarted ) >= ( 24 * 60 ) ) )
	{
		return ( TRUE );
	}

	return( FALSE );
}

// get the id of the mine for this sector x,y,z...-1 is invalid
INT8 GetIdOfMineForSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT8 bMineIndex = -1;
	//INT16 sSectorValue;

	// are we even on the right level?
	if (bSectorZ < MINIMUM_VALID_Z_COORDINATE || bSectorZ > MAXIMUM_VALID_Z_COORDINATE)
	{
		// nope
		return( -1 );
	}

	// now get the sectorvalue
	//sSectorValue = SECTOR( sSectorX, sSectorY );

	// support surface
	if( bSectorZ == 0 )
	{
		bMineIndex = GetMineIndexForSector( sSectorX, sSectorY );
	}
#if 0
	// handle for first level
	else if( bSectorZ == 1 )
	{
		switch( sSectorValue )
		{
			// grumm
			case( SEC_H3 ):
			case( SEC_I3 ):
				bMineIndex = MINE_GRUMM;
			break;
			// cambria
			case( SEC_H8 ):
			case( SEC_H9 ):
				bMineIndex = MINE_CAMBRIA;
			break;
			// alma
			case( SEC_I14 ):
			case( SEC_J14 ):
				bMineIndex = MINE_ALMA;
			break;
			// drassen
			case( SEC_D13 ):
			case( SEC_E13 ):
				bMineIndex = MINE_DRASSEN;
			break;
			// chitzena
			case( SEC_B2 ):
				bMineIndex = MINE_CHITZENA;
			break;
			// san mona
			case( SEC_D4 ):
			case( SEC_D5 ):
				bMineIndex = MINE_SAN_MONA;
			break;
		}
	}
	else
	{
		// level 2
		switch( sSectorValue )
		{
			case( SEC_I3 ):
			case( SEC_H3 ):
			case( SEC_H4 ):
				bMineIndex = MINE_GRUMM;
			break;
		}
	}
#endif
	for (std::vector<AssociatedMineSector>::iterator it = associatedMineSectors.begin();
		it != associatedMineSectors.end(); ++it)
	{
		if (it->x == sSectorX && it->y == sSectorY && bSectorZ == it->z)
		{
			return it->mineID;
		}
	}

	return( bMineIndex ); // == -1
}



// use this for miner (civilian) quotes when *underground* in a mine
//BOOLEAN PlayerForgotToTakeOverMine( UINT8 ubMineIndex )
//{
//	MINE_STATUS_TYPE *pMineStatus;
//
//
//	Assert( ( ubMineIndex >= 0 ) && ( ubMineIndex < MAX_NUMBER_OF_MINES ) );
//
//	pMineStatus = &(gMineStatus[ ubMineIndex ]);
//
//	// mine surface sector is player controlled
//	// mine not empty
//	// player hasn't spoken to the head miner, but hasn't attacked him either
//	// miner is alive
//	//if ( (StrategicMap[( gMineLocation[ ubMineIndex ].sSectorX ) + ( MAP_WORLD_X * ( gMineLocation[ ubMineIndex ].sSectorY ) )].fEnemyControlled == FALSE ) &&
//	if ( (StrategicMap[( gMineStatus[ ubMineIndex ].sSectorX ) + ( MAP_WORLD_X * ( gMineStatus[ ubMineIndex ].sSectorY ) )].fEnemyControlled == FALSE ) &&
//		( !pMineStatus->fEmpty ) &&
//		( !pMineStatus->fSpokeToHeadMiner ) &&
//		( !pMineStatus->fAttackedHeadMiner ) &&
//		( gMercProfiles[ GetHeadMinerProfileIdForMine( ubMineIndex ) ].bLife > 0 ) )
//	{
//		return( TRUE );
//	}
//
//	return( FALSE );
//}



// use this to determine whether or not to place miners into a underground mine level
BOOLEAN AreThereMinersInsideThisMine( UINT8 ubMineIndex )
{
	MINE_STATUS_TYPE *pMineStatus;

	Assert(ubMineIndex < MAX_NUMBER_OF_MINES);

	pMineStatus = &(gMineStatus[ ubMineIndex ]);

	// mine not empty
	// mine clear of any monsters
	// the "shutdown permanently" flag is only used for the player never receiving the income - miners will keep mining
	if ( ( !pMineStatus->fEmpty ) && MineClearOfMonsters( ubMineIndex ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

// returns whether or not we've spoken to the head miner of a particular mine
BOOLEAN SpokenToHeadMiner( UINT8 ubMineIndex )
{
	return( gMineStatus[ ubMineIndex ].fSpokeToHeadMiner );
}
