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
	#include "jascreens.h"
	#include "screenids.h"
	#include "Items.h"
	#include "Random.h"
	#include "finances.h"
	#include "history.h"
	#include "Dialogue Control.h"
	#include "Strategic AI.h"
	#include "Tactical Save.h"		// added by Flugente
	#include "DynamicDialogue.h"	// added by Flugente for HandleDynamicOpinions()
	#include "Drugs And Alcohol.h"	// added by Flugente for HourlyDrugUpdate()
	#include "Interface.h"			// added by Flugente
	#include "SkillCheck.h"			// added by Flugente
	#include "Isometric Utils.h"	// added by Flugente for NOWHERE
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
// anv: for hourly heli repair
#include "Vehicles.h"
#include "Map Screen Helicopter.h" 
// anv: transition to save screen in extreme iron man mode
#include "gameloop.h" 

void HourlyQuestUpdate();
void HourlyLarryUpdate();
void HourlySmokerUpdate();
void HourlyDisabilityUpdate();
void HourlyStealUpdate();	// Flugente: certain characters might steal equipment (backgrounds)
void HourlySnitchUpdate();	// anv: decreasing cooldown after exposition

extern INT32 GetCurrentBalance( void );
extern void PayOffSkyriderDebtIfAny( );
#ifdef JA2UB
//no UB
#else
void HourlyCheckIfSlayAloneSoHeCanLeave();
#endif

#ifdef JA2UB
//no UB
#else
void HourlyHelicopterRepair();
#endif

void HourlyGatheringInformation();

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

	HourlySmokerUpdate( );

	HourlyDisabilityUpdate();

	HourlyStealUpdate();

	HourlySnitchUpdate();

	HourlyGatheringInformation();

#ifdef JA2UB
// no UB
#else
	HourlyCheckIfSlayAloneSoHeCanLeave();
#endif
	// WDS - New AI
	HourlyCheckStrategicAI();

#ifdef JA2UB
// no UB
#else
	PayOffSkyriderDebtIfAny();

	HourlyHelicopterRepair();
#endif
	
	HourlyDrugUpdate();

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
					if( CanGameBeSaved() || ( gGameOptions.ubIronManMode == 3 && GetWorldHour() == gGameExternalOptions.ubExtremeIronManSavingHour ) )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT1);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT1,zString);

						AutoSaveToSlot[0] = FALSE;
						AutoSaveToSlot[1] = TRUE;
						AutoSaveToSlot[2] = FALSE;
						AutoSaveToSlot[3] = FALSE;
						AutoSaveToSlot[4] = FALSE;
					}
				}
				else if ( AutoSaveToSlot[1] == TRUE )
				{
					if( CanGameBeSaved() || ( gGameOptions.ubIronManMode == 3 && GetWorldHour() == gGameExternalOptions.ubExtremeIronManSavingHour ) )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT2);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT2,zString);

						AutoSaveToSlot[0] = FALSE;
						AutoSaveToSlot[1] = FALSE;
						AutoSaveToSlot[2] = TRUE;
						AutoSaveToSlot[3] = FALSE;
						AutoSaveToSlot[4] = FALSE;
					}			
				}
				else if ( AutoSaveToSlot[2] == TRUE )
				{
					if( CanGameBeSaved() || ( gGameOptions.ubIronManMode == 3 && GetWorldHour() == gGameExternalOptions.ubExtremeIronManSavingHour ) )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT3);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT3,zString);

						AutoSaveToSlot[0] = FALSE;
						AutoSaveToSlot[1] = FALSE;
						AutoSaveToSlot[2] = FALSE;
						AutoSaveToSlot[3] = TRUE;
						AutoSaveToSlot[4] = FALSE;
					}
				}
				else if ( AutoSaveToSlot[3] == TRUE )
				{
					if( CanGameBeSaved() || ( gGameOptions.ubIronManMode == 3 && GetWorldHour() == gGameExternalOptions.ubExtremeIronManSavingHour ) )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT4);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT4,zString);

						AutoSaveToSlot[0] = FALSE;
						AutoSaveToSlot[1] = FALSE;
						AutoSaveToSlot[2] = FALSE;
						AutoSaveToSlot[3] = FALSE;
						AutoSaveToSlot[4] = TRUE;
					}					
				}
				else if ( AutoSaveToSlot[4] == TRUE )
				{
					if( CanGameBeSaved() || ( gGameOptions.ubIronManMode == 3 && GetWorldHour() == gGameExternalOptions.ubExtremeIronManSavingHour ) )
					{
						guiPreviousOptionScreen = guiCurrentScreen;
						swprintf( zString, L"%s%d",pMessageStrings[ MSG_SAVE_AUTOSAVE_TEXT ],SAVE__TIMED_AUTOSAVE_SLOT5);
						DoAutoSave(SAVE__TIMED_AUTOSAVE_SLOT5,zString);

						AutoSaveToSlot[0] = TRUE;
						AutoSaveToSlot[1] = FALSE;
						AutoSaveToSlot[2] = FALSE;
						AutoSaveToSlot[3] = FALSE;
						AutoSaveToSlot[4] = FALSE;
					}			
				}
		}
	}

	// anv: in extreme iron man mode notify player it's time to save
	if( doAutoSave && gGameOptions.ubIronManMode == 3 && GetWorldHour() == gGameExternalOptions.ubExtremeIronManSavingHour )
	{
		// pause game
		if( gGameExternalOptions.ubExtremeIronManSavingTimeNotification == 1 )
		{		
			// not really player's doing but oh well
			HandlePlayerPauseUnPauseOfGame();
		}
		// automatically open save screen when it's time
		else if( gGameExternalOptions.ubExtremeIronManSavingTimeNotification == 0 )
		{
			gfSaveGame = TRUE;
			guiPreviousOptionScreen = guiCurrentScreen;
			SetPendingNewScreen( SAVE_LOAD_SCREEN );
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


void HourlyQuestUpdate()
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

// Flugente: abandoned the LarryItems for the new drug system
/*#define NUM_LARRY_ITEMS 6
UINT16	LarryItems[ NUM_LARRY_ITEMS ][ 3 ] =
{
	// item, temptation, points to use
	{ ADRENALINE_BOOSTER,		5, 100 },
	{	ALCOHOL,							BAR_TEMPTATION, 25	},
	{ MEDICKIT,							4, 10	},
	{	WINE,									3, 50	},
	{ REGEN_BOOSTER,				3, 100 },
	{	BEER,									2, 100 },
};*/

#define LARRY_FALLS_OFF_WAGON 8

void HourlyLarryUpdate()
{
	SOLDIERTYPE *				pSoldier = NULL;
	SOLDIERTYPE *				pOtherSoldier = NULL;
	INT8						bSlot = NO_SLOT, bBoozeSlot;
	INT8						bLarryItemLoop;
	UINT16						usTemptation = 0;
	UINT16						usCashAmount;
	BOOLEAN						fBar = FALSE;
	OBJECTTYPE*					pObj = NULL;

	for( UINT32 cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++cnt )
	{
		pSoldier = MercPtrs[ cnt ];

		if ( pSoldier && pSoldier->bAssignment < ON_DUTY && !pSoldier->flags.fBetweenSectors && pSoldier->bActive && pSoldier->bInSector && !(gTacticalStatus.fEnemyInSector || guiCurrentScreen == GAME_SCREEN ) )
		{
			if ( pSoldier->ubProfile == LARRY_NORMAL || pSoldier->ubProfile == LARRY_DRUNK || pSoldier->HasBackgroundFlag( BACKGROUND_DRUGUSE ) )
			{
				// Flugente: reworked this for the new drug system. We now loop over our entire inventory
				INT8 invsize = (INT8)pSoldier->inv.size();										// remember inventorysize, so we don't call size() repeatedly
				for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop)									// ... for all items in our inventory ...
				{
					if ( pSoldier->inv[bLoop].exists() && Item[ pSoldier->inv[bLoop].usItem ].drugtype > 0 )
					{
						pObj = &(pSoldier->inv[bLoop]);

						usTemptation = 5;

						// any drug will do... I'm not going to create a new tag for sth minor like this
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
					// anv: snitches stop mercs from getting wasted
					BOOLEAN fSnitchStoppedBehaviour = FALSE;
					for( INT32 cnt2 = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt2 <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++cnt2 )
					{					
						pOtherSoldier = MercPtrs[ cnt2 ];
						// note - snitches stop others, but can get wasted themselves (if they have drug use specifically set in background...)
						if( pOtherSoldier && !pOtherSoldier->flags.fBetweenSectors && pOtherSoldier->bActive && !pOtherSoldier->flags.fMercAsleep && pSoldier->ubProfile != pOtherSoldier->ubProfile )
						{
							if ( ProfileHasSkillTrait( pOtherSoldier->ubProfile, SNITCH_NT ) && !( pSoldier->usSoldierFlagMask2 & SOLDIER_PREVENT_MISBEHAVIOUR_OFF ) )
							{
								if( pSoldier->sSectorX == pOtherSoldier->sSectorX && pSoldier->sSectorY == pOtherSoldier->sSectorY && pSoldier->bSectorZ == pOtherSoldier->bSectorZ )
								{
									UINT16 bPreventChance = ( EffectiveLeadership( pOtherSoldier ) + EffectiveExpLevel( pOtherSoldier ) / 2);
									if (gGameOptions.fNewTraitSystem)
									{
										bPreventChance += 25 * NUM_SKILL_TRAITS( pOtherSoldier, SQUADLEADER_NT );
										bPreventChance -= 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_NT );
									}
									else
									{
										bPreventChance -= 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_OT );
									}
									// keep 1% chance no matter what
									bPreventChance = max( 0, min( 99, bPreventChance ) );
									if( bPreventChance > PreRandom( 100 ) )
									{
										// merc is not amused by being prevented
										HandleMoraleEvent( pSoldier, MORALE_PREVENTED_MISBEHAVIOUR, pSoldier->sSectorX, pSoldier->sSectorX, pSoldier->bSectorZ );
										// also here would be a place for dynamic relationship decrease between them
										// Flugente: then lets do that, shall we?
										AddOpinionEvent( pSoldier->ubProfile, pOtherSoldier->ubProfile, OPINIONEVENT_SNITCHINTERFERENCE );

										fSnitchStoppedBehaviour = TRUE;
										continue;
									}
								}
							}
						}
					}
					
					if ( fSnitchStoppedBehaviour )
						continue;

					if ( pSoldier->ubProfile == LARRY_DRUNK )
					{
						// NB store all drunkenness info in LARRY_NORMAL profile (to use same values)
						// so long as he keeps consuming, keep number above level at which he cracked
						gMercProfiles[ LARRY_NORMAL ].bNPCData = __max( gMercProfiles[ LARRY_NORMAL ].bNPCData, LARRY_FALLS_OFF_WAGON );
						gMercProfiles[ LARRY_NORMAL ].bNPCData += (INT8) Random( usTemptation );
						// allow value to keep going up to 24 (about 2 days since we subtract Random( 2 ) when he has no access )
						gMercProfiles[ LARRY_NORMAL ].bNPCData = __min( gMercProfiles[ LARRY_NORMAL ].bNPCData, 24 );
					}
					else
					{
						gMercProfiles[ pSoldier->ubProfile ].bNPCData += (INT8) Random( usTemptation );

						if ( gMercProfiles[ pSoldier->ubProfile ].bNPCData < LARRY_FALLS_OFF_WAGON )
							continue;
					}

					// Flugente: dynamic opinion
					HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_ADDICT, TRUE, FALSE );

					if ( fBar )
					{
						// take $ from player's account
						// silversurfer: changed the price to reflect the changed amount of 25% below
						//usCashAmount = Item[ ALCOHOL ].usPrice;
						usCashAmount = (UINT16)(Item[ ALCOHOL ].usPrice / 4.0f );
						AddTransactionToPlayersBook ( TRANSFER_FUNDS_TO_MERC, pSoldier->ubProfile, GetWorldTotalMin() , -( usCashAmount ) );
						// give Larry some booze and set slot etc values appropriately
						// CHRISL: Change final parameter to allow dynamic control of inventory slots
						bBoozeSlot = FindEmptySlotWithin( pSoldier, HANDPOS, NUM_INV_SLOTS );
						if ( bBoozeSlot != NO_SLOT )
						{
							// give Larry booze here
							// silversurfer: only give the merc a 25% bottle. This fixes the problem that the bottle of alcohol can go to any inventory slot even one that isn't available.
							// Now the bottle will be fully consumed below and vanishes from inventory before the player even gets to see it. This simulates going to a bar to have a drink there.
							//CreateItem( ALCOHOL, 100, &(pSoldier->inv[bBoozeSlot]) );
							CreateItem( ALCOHOL, 25, &(pSoldier->inv[bBoozeSlot]) );
						}
						bSlot = bBoozeSlot;
						bLarryItemLoop = 1;

						if ( pSoldier->ubProfile == LARRY_DRUNK )
						{
							SwapLarrysProfiles( pSoldier );
						}

						if ( bSlot != NO_SLOT )
						{
							UseKitPoints( &(pSoldier->inv[ bSlot ]), 25/*LarryItems[ bLarryItemLoop ][ 2 ]*/, pSoldier );
						}
					}
					else
					{
						if ( pObj )
							UseKitPoints( pObj, 100/*LarryItems[ bLarryItemLoop ][ 2 ]*/, pSoldier );
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
	}
}

// Flugente: mercs that are smokers occasionally consume smokes if they have some in their inventory
void HourlySmokerUpdate( )
{
	SOLDIERTYPE *				pSoldier = NULL;
	OBJECTTYPE*					pObj = NULL;

	for ( UINT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++cnt )
	{
		pSoldier = MercPtrs[cnt];

		if ( pSoldier && pSoldier->bActive && !pSoldier->flags.fMercAsleep )
		{
			// if we are a smoker, there is a chance that we will look fo cigarettes in our inventory, and consume them if we find any
			if ( Chance(33) && pSoldier->GetBackgroundValue( BG_SMOKERTYPE ) == 1 )
			{
				INT8 invsize = (INT8)pSoldier->inv.size( );											// remember inventorysize, so we don't call size() repeatedly
				for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )									// ... for all items in our inventory ...
				{
					if ( pSoldier->inv[bLoop].exists( ) && Item[pSoldier->inv[bLoop].usItem].cigarette )
					{
						pObj = &(pSoldier->inv[bLoop]);

						if ( ApplyConsumable( pSoldier, pObj, FALSE, FALSE ) )
						{
							break;
						}
					}
				}
			}
		}
	}
}

void HourlyDisabilityUpdate( )
{
	SOLDIERTYPE*				pSoldier = NULL;
	SOLDIERTYPE*				pOtherSoldier = NULL;

	for ( UINT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++cnt )
	{
		pSoldier = MercPtrs[cnt];

		if ( pSoldier && pSoldier->bActive && !pSoldier->flags.fMercAsleep )
		{
			// possibl self-harm
			if ( Chance(20) && DoesMercHaveDisability( pSoldier, SELF_HARM ) )
			{
				// don't do this if we are at low health, or in combat, or travelling, or a patient or doctor
				// only do this if we are rather healed
				if ( pSoldier->stats.bLife >= OKLIFE && pSoldier->stats.bLifeMax > 0 && (FLOAT)(pSoldier->stats.bLife) / (FLOAT)(pSoldier->stats.bLifeMax) > 0.9f 
					 && !pSoldier->flags.fBetweenSectors && !gTacticalStatus.fEnemyInSector
					 && !IS_PATIENT( pSoldier->bAssignment ) && pSoldier->bAssignment != IN_TRANSIT )
				{
					// anv: snitches stop mercs from getting wasted
					BOOLEAN fSnitchStoppedBehaviour = FALSE;
					for ( INT32 cnt2 = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt2 <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++cnt2 )
					{
						pOtherSoldier = MercPtrs[cnt2];

						// note - snitches stop others, but can get wasted themselves (if they have drug use specifically set in background...)
						if ( pOtherSoldier && !pOtherSoldier->flags.fBetweenSectors && pOtherSoldier->bActive && !pOtherSoldier->flags.fMercAsleep && pSoldier->ubProfile != pOtherSoldier->ubProfile )
						{
							if ( ProfileHasSkillTrait( pOtherSoldier->ubProfile, SNITCH_NT ) && !(pSoldier->usSoldierFlagMask2 & SOLDIER_PREVENT_MISBEHAVIOUR_OFF) )
							{
								if ( pSoldier->sSectorX == pOtherSoldier->sSectorX && pSoldier->sSectorY == pOtherSoldier->sSectorY && pSoldier->bSectorZ == pOtherSoldier->bSectorZ )
								{
									UINT16 bPreventChance = (EffectiveLeadership( pOtherSoldier ) + EffectiveExpLevel( pOtherSoldier ) / 2);
									if ( gGameOptions.fNewTraitSystem )
									{
										bPreventChance += 25 * NUM_SKILL_TRAITS( pOtherSoldier, SQUADLEADER_NT );
										bPreventChance -= 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_NT );
									}
									else
									{
										bPreventChance -= 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_OT );
									}

									// keep 1% chance no matter what
									bPreventChance = max( 0, min( 99, bPreventChance ) );
									if ( Chance( bPreventChance ) )
									{
										// merc is not amused by being prevented
										HandleMoraleEvent( pSoldier, MORALE_PREVENTED_MISBEHAVIOUR, pSoldier->sSectorX, pSoldier->sSectorX, pSoldier->bSectorZ );
										// also here would be a place for dynamic relationship decrease between them
										// Flugente: then lets do that, shall we?
										AddOpinionEvent( pSoldier->ubProfile, pOtherSoldier->ubProfile, OPINIONEVENT_SNITCHINTERFERENCE );

										fSnitchStoppedBehaviour = TRUE;
										continue;
									}
								}
							}
						}
					}

					if ( !fSnitchStoppedBehaviour )
					{
						// take damage, but not bleeding damage (otherwise we'd constantly have to check in on this merc and manually bandage them, which is tedious)
						INT8 oldbleeding = pSoldier->bBleeding;

						pSoldier->SoldierTakeDamage( 0, 1, 0, TAKE_DAMAGE_BLADE, pSoldier->ubID, NOWHERE, 0, FALSE );

						pSoldier->bBleeding = oldbleeding;

						// Flugente: dynamic opinions
						HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_ANNOYINGDISABILITY, TRUE, TRUE );
					}
				}
			}
		}
	}
}

// anv: decrease exposed snitch cooldown (for simplified exposition handling)
void HourlySnitchUpdate()
{
	for( INT32 cnt = 0; cnt < NUM_PROFILES; ++cnt )
	{
		if( gMercProfiles[cnt].ubSnitchExposedCooldown > 0 )
			gMercProfiles[cnt].ubSnitchExposedCooldown--;
	}
}

void HourlyStealUpdate()
{
	SOLDIERTYPE *				pSoldier = NULL;
	SOLDIERTYPE *				pOtherSoldier = NULL;
	INT8						bSlot = NO_SLOT;
	UINT16						usTemptation = 0;
	BOOLEAN						fBar = FALSE;
	OBJECTTYPE*					pObj = NULL;

	for( INT32 cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++cnt )
	{
		pSoldier = MercPtrs[ cnt ];

		// merc must be alive, not travelling and awake. If he is in the currently loaded sector, we may not be in tactical (we would see an item suddenly disappearing) and not in combat
		if ( pSoldier && !pSoldier->flags.fBetweenSectors && pSoldier->bActive && !pSoldier->flags.fMercAsleep && 
			!( ( ( gWorldSectorX == pSoldier->sSectorX ) && ( gWorldSectorY == pSoldier->sSectorY ) && (gbWorldSectorZ == pSoldier->bSectorZ ) ) && (gTacticalStatus.fEnemyInSector || guiCurrentScreen == GAME_SCREEN ))  )
		{
			if ( pSoldier->HasBackgroundFlag( BACKGROUND_SCROUNGING ) )
			{
				// 50% chance that we'll steal something
				if ( Chance(50) )
				{
					// anv: snitches prevent scrounging in the same sector
					BOOLEAN fSnitchStoppedBehaviour = FALSE;
					for( INT32 cnt2 = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt2 <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; ++cnt2 )
					{					
						pOtherSoldier = MercPtrs[ cnt2 ];
						// note - snitches stop others, but can scrounge themselves (if they have scrounging specifically set in background...)
						if( pOtherSoldier && !pOtherSoldier->flags.fBetweenSectors && pOtherSoldier->bActive && !pOtherSoldier->flags.fMercAsleep && pSoldier->ubProfile != pOtherSoldier->ubProfile )
						{
							if ( ProfileHasSkillTrait( pOtherSoldier->ubProfile, SNITCH_NT ) && !( pSoldier->usSoldierFlagMask2 & SOLDIER_PREVENT_MISBEHAVIOUR_OFF ) )
							{
								if( pSoldier->sSectorX == pOtherSoldier->sSectorX && pSoldier->sSectorY == pOtherSoldier->sSectorY && pSoldier->bSectorZ == pOtherSoldier->bSectorZ )
								{
									UINT16 bPreventChance = ( EffectiveLeadership( pOtherSoldier ) + EffectiveExpLevel( pOtherSoldier ) / 2);
									if (gGameOptions.fNewTraitSystem)
									{
										bPreventChance += 25 * NUM_SKILL_TRAITS( pOtherSoldier, SQUADLEADER_NT );
										bPreventChance -= 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_NT );
									}
									else
									{
										bPreventChance -= 25 * NUM_SKILL_TRAITS( pSoldier, STEALTHY_OT );
									}
									// keep 1% chance no matter what
									bPreventChance = max( 0, min( 99, bPreventChance ) );
									if( bPreventChance > PreRandom( 100 ) )
									{
										// merc is not amused by being prevented
										HandleMoraleEvent( pSoldier, MORALE_PREVENTED_MISBEHAVIOUR, pSoldier->sSectorX, pSoldier->sSectorX, pSoldier->bSectorZ );
										// also here would be a place for dynamic relationship decrease between them
										// Flugente: then lets do that, shall we?
										AddOpinionEvent( pSoldier->ubProfile, pOtherSoldier->ubProfile, OPINIONEVENT_SNITCHINTERFERENCE );

										fSnitchStoppedBehaviour = TRUE;

										continue;
									}
								}
							}
						}
					}

					if ( fSnitchStoppedBehaviour )
						continue;

					// we loop over this sector's inventory and look for something shiny. We will pick it up if we hae enough space in our inventory
					// open sector inv
					UINT32 uiTotalNumberOfRealItems = 0;
					std::vector<WORLDITEM> pWorldItem;//dnl ch75 271013
					BOOLEAN fReturn					= FALSE;

					if ( ( gWorldSectorX == pSoldier->sSectorX ) && ( gWorldSectorY == pSoldier->sSectorY ) && (gbWorldSectorZ == pSoldier->bSectorZ ) )
					{
						uiTotalNumberOfRealItems = guiNumWorldItems;

						if ( !uiTotalNumberOfRealItems )
							continue;

						pWorldItem = gWorldItems;
					}
					else
					{
						// not loaded, load
						// get total number, visable and invisible
						fReturn = GetNumberOfWorldItemsFromTempItemFile( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ, &( uiTotalNumberOfRealItems ), FALSE );
						Assert( fReturn );

						if( uiTotalNumberOfRealItems > 0 )
						{
							// allocate space for the list
							pWorldItem.resize(uiTotalNumberOfRealItems);//dnl ch75 271013

							if ( !uiTotalNumberOfRealItems )
								continue;

							// now load into mem
							LoadWorldItemsFromTempItemFile(  pSoldier->sSectorX,  pSoldier->sSectorY, pSoldier->bSectorZ, pWorldItem );
						}
					}

					FLOAT bestpriceweightratio = 50.0f;		// must have a value of at least 50$/100 gram
					OBJECTTYPE* pTargetObj = NULL;
					for( UINT32 uiCount = 0; uiCount < uiTotalNumberOfRealItems; ++uiCount )				// ... for all items in the world ...
					{
						if( pWorldItem[ uiCount ].fExists )										// ... if item exists ...
						{
							OBJECTTYPE* pObj = &(pWorldItem[ uiCount ].object);			// ... get pointer for this item ...

							if ( pObj != NULL && pObj->exists() )						// ... if pointer is not obviously useless ...
							{
								// test wether item is reachable and useable by militia
								if ( (pWorldItem[ uiCount ].usFlags & WORLD_ITEM_REACHABLE) )
								{
									// of course we prefer money above everything else
									if ( Item[pWorldItem[ uiCount ].object.usItem].usItemClass == IC_MONEY )
									{
										// give it a high enough value
										bestpriceweightratio = 1000.0f;
										pTargetObj = &(pWorldItem[ uiCount ].object);
									}
									// ignore objects without weight (how does that happen anyway? careless modders...)
									else if ( Item[pWorldItem[ uiCount ].object.usItem].ubWeight )
									{
										if ( (FLOAT)(Item[pWorldItem[ uiCount ].object.usItem].usPrice) / (FLOAT)(Item[pWorldItem[ uiCount ].object.usItem].ubWeight) > bestpriceweightratio )
										{
											bestpriceweightratio = (FLOAT)(Item[pWorldItem[ uiCount ].object.usItem].usPrice) / (FLOAT)(Item[pWorldItem[ uiCount ].object.usItem].ubWeight);
											pTargetObj = &(pWorldItem[ uiCount ].object);
										}
									}
								}
							}
						}
					}

					if ( pTargetObj )
					{
						if ( AutoPlaceObject( pSoldier, pTargetObj, TRUE ) )
						{
							// Flugente: dynamic opinion
							HandleDynamicOpinionChange( pSoldier, OPINIONEVENT_THIEF, TRUE, FALSE );
						}
					}

					// save the changed inventory
					// open sector inv
					if( ( gWorldSectorX == pSoldier->sSectorX )&&( gWorldSectorY == pSoldier->sSectorY ) && (gbWorldSectorZ == pSoldier->bSectorZ ) )
					{
						guiNumWorldItems = uiTotalNumberOfRealItems;
						gWorldItems = pWorldItem;
					}
					else
					{
						//Save the Items to the the file
						SaveWorldItemsToTempItemFile( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ, uiTotalNumberOfRealItems, pWorldItem );
					}
				}				
			}
		}
	}
}

#ifdef JA2UB
// no JA25 UB
#else 
void HourlyCheckIfSlayAloneSoHeCanLeave()
{
	if (gGameExternalOptions.fEnableSlayForever || gGameExternalOptions.ubHourlyChanceSlayWillLeave < 1) return;

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
		if( Chance( gGameExternalOptions.ubHourlyChanceSlayWillLeave ) )
		{
			pSoldier->ubLeaveHistoryCode = HISTORY_SLAY_MYSTERIOUSLY_LEFT;
			TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING_NO_ASK_EQUIP, 0, 0 );
		}
	}
}
#endif

#ifdef JA2UB
// no JA25 UB
#else 
void HourlyHelicopterRepair()
{
	if( gubHelicopterHoursToRepair == 0 )
	{
		return;
	}
	MERCPROFILESTRUCT *pMechanic;
	pMechanic = &(gMercProfiles[ WALDO ]);
	if( !pMechanic )
	{
		return;
	}
	if( !pMechanic->bLife )
	{
		return;
	}
	if( pVehicleList[ iHelicopterVehicleId ].sSectorX == gMercProfiles[ WALDO ].sSectorX && pVehicleList[ iHelicopterVehicleId ].sSectorY == gMercProfiles[ WALDO ].sSectorY )
	{
		gubHelicopterHoursToRepair--;
	}
	if( gubHelicopterHoursToRepair == 0 )
	{
		FinishHelicopterRepair();
	}
}
#endif

void HourlyGatheringInformation()
{
	SOLDIERTYPE *pSoldier;
	INT32 cnt=0;

	// go through list of characters, find all snicthes gathering info
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pSoldier++)
	{
		if( ( pSoldier->bActive ) && ( pSoldier->bAssignment == SNITCH_GATHER_RUMOURS || pSoldier->bAssignment == FACILITY_GATHER_RUMOURS ) )
		{
			HandleGatheringInformationBySoldier( pSoldier );
		}
	}
}
