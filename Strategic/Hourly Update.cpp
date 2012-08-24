#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Assignments.h"
	#include "Strategic Town Loyalty.h"
	#include "Strategic Merc Handler.h"
	#include "Strategic Mines.h"
	#include "Campaign.h"
	#include "Morale.h"
	#include "Quests.h"
	#include "Game Clock.h"
	#include "Overhead.h"
	#include "Boxing.h"
	#include "jascreens.h"
	#include "screenids.h"
	#include "Items.h"
	#include "Random.h"
	#include "finances.h"
	#include "history.h"
	#include "Dialogue Control.h"
	#include "Strategic AI.h"
#endif

#include "Luaglobal.h"
#include "LuaInitNPCs.h"
#include "SaveLoadGame.h"
#include "GameSettings.h"
#include "connect.h"
#include "Options Screen.h"
#include "SaveLoadScreen.h"
#include "Text.h"
// HEADROCK HAM 3.5: Add facility code for hourly update of detection levels
#include "Facilities.h"
#include "Food.h"	// added by Flugente

void HourlyQuestUpdate( void );
void HourlyLarryUpdate( void );

extern INT32 GetCurrentBalance( void );
extern void PayOffSkyriderDebtIfAny( );
#if (defined JA2UB) 
//no UB
#else
void HourlyCheckIfSlayAloneSoHeCanLeave();
#endif

void UpdateRegenCounters( void );

void HandleMinuteUpdate()
{
}

// This function gets called every hour, on the hour.
// It spawns the handling of all sorts of stuff:
// Morale changes, assignment progress, town loyalties, etc.

void HandleHourlyUpdate()
{
CHAR16	zString[128]; 
	//if the game hasnt even started yet ( we havent arrived in the sector ) dont process this
	if ( DidGameJustStart() )
		return;
	if(is_networked)
		return;

	// hourly update of town loyalty
	HandleTownLoyalty();

	// Flugente: food update
	if ( gGameOptions.fFoodSystem )
		HourlyFoodUpdate();

	// hourly update of team assignments
	UpdateAssignments();

	// HEADROCK HAM 3.5: Tells the rest of the program whether we've got mercs working on detecting enemy units.
	UpdateStrategicDetectionLevel();

	// HEADROCK HAM 3.5: Re-calculates a modifier to Skyrider's flight costs based on mercs working in specific facilities.
	UpdateSkyriderCostModifier();

	// hourly update of hated/liked mercs
	UpdateBuddyAndHatedCounters();

	HandleHourlyRisks();

	// update morale!
	HourlyMoraleUpdate();

	// degrade camouflage
	HourlyCamouflageUpdate();

	// check mines for monster infestation/extermination
	HourlyMinesUpdate();

	// check how well the player is doing right now
	HourlyProgressUpdate();

	// do any quest stuff necessary
	HourlyQuestUpdate();

	HourlyLarryUpdate();
#if (defined JA2UB) 
// no UB
#else
	HourlyCheckIfSlayAloneSoHeCanLeave();
#endif
	// WDS - New AI
	HourlyCheckStrategicAI();

	PayOffSkyriderDebtIfAny();

	if ( GetWorldHour() % 6 == 0 ) // 4 times a day
	{
		UpdateRegenCounters();
	}

	// WANNE: This check should avoid the resaving of a loaded auto-save game, when entering tactical
	BOOLEAN doAutoSave = TRUE;
	if (lastLoadedSaveGameDay == GetWorldDay() && lastLoadedSaveGameHour == GetWorldHour() )
	{
		doAutoSave = FALSE;
	}

	if (doAutoSave)
	{
		if ( AutoSaveToSlot[1] == FALSE && AutoSaveToSlot[2] == FALSE && AutoSaveToSlot[3] == FALSE && AutoSaveToSlot[4] == FALSE )
			AutoSaveToSlot[0] = TRUE;

		if ((gGameExternalOptions.autoSaveTime != 0) && (GetWorldHour() % gGameExternalOptions.autoSaveTime == 0)) 
		{
				if ( AutoSaveToSlot[0] == TRUE )
				{
					if( CanGameBeSaved() )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT1);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT1,zString);
					}
					
					AutoSaveToSlot[0] = FALSE;
					AutoSaveToSlot[1] = TRUE;
					AutoSaveToSlot[2] = FALSE;
					AutoSaveToSlot[3] = FALSE;
					AutoSaveToSlot[4] = FALSE;
				}
				else if ( AutoSaveToSlot[1] == TRUE )
				{
					if( CanGameBeSaved() )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT2);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT2,zString);
					}
					
					AutoSaveToSlot[0] = FALSE;
					AutoSaveToSlot[1] = FALSE;
					AutoSaveToSlot[2] = TRUE;
					AutoSaveToSlot[3] = FALSE;
					AutoSaveToSlot[4] = FALSE;
				}
				else if ( AutoSaveToSlot[2] == TRUE )
				{
					if( CanGameBeSaved() )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT3);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT3,zString);
					}
					
					AutoSaveToSlot[0] = FALSE;
					AutoSaveToSlot[1] = FALSE;
					AutoSaveToSlot[2] = FALSE;
					AutoSaveToSlot[3] = TRUE;
					AutoSaveToSlot[4] = FALSE;
				}
				else if ( AutoSaveToSlot[3] == TRUE )
				{
					if( CanGameBeSaved() )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT4);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT4,zString);
					}
					AutoSaveToSlot[0] = FALSE;
					AutoSaveToSlot[1] = FALSE;
					AutoSaveToSlot[2] = FALSE;
					AutoSaveToSlot[3] = FALSE;
					AutoSaveToSlot[4] = TRUE;
					
				}
				else if ( AutoSaveToSlot[4] == TRUE )
				{
					if( CanGameBeSaved() )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT5);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT5,zString);
					}
					AutoSaveToSlot[0] = TRUE;
					AutoSaveToSlot[1] = FALSE;
					AutoSaveToSlot[2] = FALSE;
					AutoSaveToSlot[3] = FALSE;
					AutoSaveToSlot[4] = FALSE;
					
				}
		}
	}
}

void UpdateRegenCounters( void )
{
	UINT8	ubID;

	for ( ubID = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; ubID <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubID++ )
	{
		if ( MercPtrs[ ubID ]->bRegenBoostersUsedToday > 0 )
		{
			MercPtrs[ ubID ]->bRegenBoostersUsedToday--;
		}
	}
}

void HandleQuarterHourUpdate()
{
	//if the game hasnt even started yet ( we havent arrived in the sector ) dont process this
	if ( DidGameJustStart() )
		return;

	DecayTacticalMoraleModifiers( );
}


void HourlyQuestUpdate( void )
{
	UINT32 uiHour = GetWorldHour();
		
#ifdef LUA_HOURLY_QUEST_UPDATE
	LetLuaHourlyQuestUpdate(0);
#else

	// brothel
	if ( uiHour == 4 )
	{
		SetFactFalse( FACT_BROTHEL_OPEN );
	}
	else if ( uiHour == 20 )
	{
		SetFactTrue( FACT_BROTHEL_OPEN );
	}

	// bar/nightclub
	if ( uiHour == 15 )
	{
		UINT8 ubLoop;

		SetFactTrue( FACT_CLUB_OPEN );
		SetFactFalse( FACT_PAST_CLUB_CLOSING_AND_PLAYER_WARNED );

		// reset boxes fought
		for ( ubLoop = 0; ubLoop < NUM_BOXERS; ubLoop++ )
		{
			gfBoxerFought[ ubLoop ] = FALSE;
		}

		// if # of boxing matches the player has won is a multiple of
		// 3, and the boxers haven't rested, then make them rest
		if ( gfBoxersResting )
		{
			// done resting now!
			gfBoxersResting = FALSE;
			gubBoxersRests++;
		}
		else if ( gubBoxingMatchesWon / 3 > gubBoxersRests )
		{
			// time for the boxers to rest!
			gfBoxersResting = TRUE;
		}
	}
	else if ( uiHour == 2 )
	{
		SetFactFalse( FACT_CLUB_OPEN );
	}

	// museum
	if ( uiHour == 9 )
	{
		SetFactTrue( FACT_MUSEUM_OPEN );
	}
	else if ( uiHour == 18 )
	{
		SetFactFalse( FACT_MUSEUM_OPEN );
	}

#endif
}

#define BAR_TEMPTATION 4
#define NUM_LARRY_ITEMS 6
UINT16	LarryItems[ NUM_LARRY_ITEMS ][ 3 ] =
{
	// item, temptation, points to use
	{ ADRENALINE_BOOSTER,		5, 100 },
	{	ALCOHOL,							BAR_TEMPTATION, 25	},
	{ MEDICKIT,							4, 10	},
	{	WINE,									3, 50	},
	{ REGEN_BOOSTER,				3, 100 },
	{	BEER,									2, 100 },
};

#define LARRY_FALLS_OFF_WAGON 8

void HourlyLarryUpdate( void )
{
	SOLDIERTYPE *			pSoldier;
	INT8							bSlot = NO_SLOT, bBoozeSlot;
	INT8							bLarryItemLoop;
	UINT16						usTemptation = 0;
	UINT16						usCashAmount;
	BOOLEAN						fBar = FALSE;

	pSoldier = FindSoldierByProfileID( LARRY_NORMAL, TRUE );
	if ( !pSoldier )
	{
		pSoldier = FindSoldierByProfileID( LARRY_DRUNK, TRUE );
	}
	if ( pSoldier )
	{
		if ( pSoldier->bAssignment >= ON_DUTY )
		{
			return;
		}
		if ( pSoldier->flags.fBetweenSectors )
		{
			return;
		}
		if ( pSoldier->bActive && pSoldier->bInSector && (gTacticalStatus.fEnemyInSector || guiCurrentScreen == GAME_SCREEN ) )
		{
			return;
		}

		// look for items in Larry's inventory to maybe use
		for ( bLarryItemLoop = 0; bLarryItemLoop < NUM_LARRY_ITEMS; bLarryItemLoop++ )
		{
			bSlot = FindObj( pSoldier, LarryItems[ bLarryItemLoop ][ 0 ] );
			if ( bSlot != NO_SLOT )
			{
				usTemptation = LarryItems[ bLarryItemLoop ][ 1 ];
				break;
			}
		}

		// check to see if we're in a bar sector, if we are, we have access to alcohol
		// which may be better than anything we've got...
		if ( usTemptation < BAR_TEMPTATION && GetCurrentBalance() >= Item[ ALCOHOL ].usPrice )
		{
			if ( pSoldier->bSectorZ == 0 &&
						( ( pSoldier->sSectorX == 13 && pSoldier->sSectorY == MAP_ROW_D) ||
							( pSoldier->sSectorX == 13 && pSoldier->sSectorY == MAP_ROW_C) ||
							( pSoldier->sSectorX == 5 && pSoldier->sSectorY == MAP_ROW_C) ||
							( pSoldier->sSectorX == 6 && pSoldier->sSectorY == MAP_ROW_C) ||
							( pSoldier->sSectorX == 5 && pSoldier->sSectorY == MAP_ROW_D) ||
							( pSoldier->sSectorX == 2 && pSoldier->sSectorY == MAP_ROW_H)
						)
				)
			{
				// in a bar!
				fBar = TRUE;
				usTemptation = BAR_TEMPTATION;
			}
		}

		if ( usTemptation > 0 )
		{
			if ( pSoldier->ubProfile == LARRY_NORMAL )
			{
				gMercProfiles[ LARRY_NORMAL ].bNPCData += (INT8) Random( usTemptation );
				if ( gMercProfiles[ LARRY_NORMAL ].bNPCData >= LARRY_FALLS_OFF_WAGON )
				{
					if ( fBar )
					{
						// take $ from player's account
						usCashAmount = Item[ ALCOHOL ].usPrice;
						AddTransactionToPlayersBook ( TRANSFER_FUNDS_TO_MERC, pSoldier->ubProfile, GetWorldTotalMin() , -( usCashAmount ) );
						// give Larry some booze and set slot etc values appropriately
						// CHRISL: Change final parameter to allow dynamic control of inventory slots
						bBoozeSlot = FindEmptySlotWithin( pSoldier, HANDPOS, NUM_INV_SLOTS );
						if ( bBoozeSlot != NO_SLOT )
						{
							// give Larry booze here
							CreateItem( ALCOHOL, 100, &(pSoldier->inv[bBoozeSlot]) );
						}
						bSlot = bBoozeSlot;
						bLarryItemLoop = 1;
					}
					// ahhhh!!!
					SwapLarrysProfiles( pSoldier );
					if ( bSlot != NO_SLOT )
					{
						UseKitPoints( &(pSoldier->inv[ bSlot ]), LarryItems[ bLarryItemLoop ][ 2 ], pSoldier );
					}
				}
			}
			else
			{
				// NB store all drunkenness info in LARRY_NORMAL profile (to use same values)
				// so long as he keeps consuming, keep number above level at which he cracked
				gMercProfiles[ LARRY_NORMAL ].bNPCData = __max( gMercProfiles[ LARRY_NORMAL ].bNPCData, LARRY_FALLS_OFF_WAGON );
				gMercProfiles[ LARRY_NORMAL ].bNPCData += (INT8) Random( usTemptation );
				// allow value to keep going up to 24 (about 2 days since we subtract Random( 2 ) when he has no access )
				gMercProfiles[ LARRY_NORMAL ].bNPCData = __min( gMercProfiles[ LARRY_NORMAL ].bNPCData, 24 );
				if ( fBar )
				{
					// take $ from player's account
					usCashAmount = Item[ ALCOHOL ].usPrice;
					AddTransactionToPlayersBook ( TRANSFER_FUNDS_TO_MERC, pSoldier->ubProfile, GetWorldTotalMin() , -( usCashAmount ) );
					// give Larry some booze and set slot etc values appropriately
					// CHRISL: Change final parameter to allow dynamic control of inventory slots
					bBoozeSlot = FindEmptySlotWithin( pSoldier, HANDPOS, NUM_INV_SLOTS );
					if ( bBoozeSlot != NO_SLOT )
					{
						// give Larry booze here
						CreateItem( ALCOHOL, 100, &(pSoldier->inv[bBoozeSlot]) );
					}
					bSlot = bBoozeSlot;
					bLarryItemLoop = 1;
				}
				if ( bSlot != NO_SLOT )
				{
					// ahhhh!!!
					UseKitPoints( &(pSoldier->inv[ bSlot ]), LarryItems[ bLarryItemLoop ][ 2 ], pSoldier );
				}
			}
		}
		else if ( pSoldier->ubProfile == LARRY_DRUNK )
		{
			gMercProfiles[ LARRY_NORMAL ].bNPCData -= (INT8) Random( 2 );
			if ( gMercProfiles[ LARRY_NORMAL ].bNPCData <= 0 )
			{
				// goes sober!
				SwapLarrysProfiles( pSoldier );
			}
		}

	}

}
#if (defined JA2UB) 
// no JA25 UB
#else 
void HourlyCheckIfSlayAloneSoHeCanLeave()
{
	SOLDIERTYPE *pSoldier;
	pSoldier = FindSoldierByProfileID( SLAY, TRUE );
	if( !pSoldier )
	{
		return;
	}
	if( pSoldier->flags.fBetweenSectors )
	{
		return;
	}
	if( !pSoldier->bActive || !pSoldier->stats.bLife )
	{
		return;
	}
	if( PlayerMercsInSector( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY, pSoldier->bSectorZ ) == 1 )
	{
		if( Chance( 15 ) )
		{
			pSoldier->ubLeaveHistoryCode = HISTORY_SLAY_MYSTERIOUSLY_LEFT;
			TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING_NO_ASK_EQUIP, 0, 0 );
		}
	}
}
#endif
