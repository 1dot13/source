#include "connect.h"
#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "items.h"
	#include "Action Items.h"
	#include "handle Items.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "points.h"
	#include "tiledef.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "interface.h"
	#include "renderworld.h"
	#include "Animation Control.h"
	#include "font control.h"
	#include "render dirty.h"
	#include "World items.h"
	#include "text.h"
	#include "Timer Control.h"
	#include "wcheck.h"
	#include "interface items.h"
	#include "physics.h"
	#include "soldier profile.h"
	#include "interface dialogue.h"
	#include "quests.h"
	#include "message.h"
	#include "isometric utils.h"
	#include "los.h"
	#include "dialogue control.h"
	#include "ai.h"
	#include "soldier macros.h"
	#include "interface panels.h"
	#include "Strategic Town Loyalty.h"
	#include "soldier functions.h"
	#include "Map Screen Helicopter.h"
	#include "pathai.h"
	#include "fov.h"
	#include "MessageBoxScreen.h"
	#include "explosion control.h"
	#include "SkillCheck.h"
	#include "Campaign.h"
	#include "Random.h"
	#include "structure wrap.h"
	#include "interactive tiles.h"
	#include "SaveLoadMap.h"
	#include "ShopKeeper Interface.h"
	#include "Arms Dealer Init.h"
	#include "soldier add.h"
	#include "Sound Control.h"
	#include "squads.h"
	#include "rotting corpses.h"
	#include "soldier ani.h"
	#include "Opplist.h"
	#include "qarray.h"
	#include "render fun.h"
	#include "environment.h"
	#include "Map Information.h"
	#include "GameSettings.h"
	#include "end game.h"
	#include "interface control.h"
	#include "Map Screen Interface Map Inventory.h"
	// added by SANDRO
	#include "Game Clock.h" 
	#include "Morale.h"
#endif

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#endif

#define					NUM_ITEMS_LISTED			8
#define					NUM_ITEM_FLASH_SLOTS	50
#define					MIN_LOB_RANGE					6


ITEM_POOL_LOCATOR				FlashItemSlots[ NUM_ITEM_FLASH_SLOTS ];
UINT32									guiNumFlashItemSlots = 0;


LEVELNODE *AddItemGraphicToWorld( INVTYPE *pItem, INT32 sGridNo, UINT8 ubLevel );
INT8 GetListMouseHotSpot( INT16 sLargestLineWidth, INT8 bNumItemsListed, INT16 sFontX, INT16 sFontY, INT8 bCurStart );
void RemoveItemGraphicFromWorld( INVTYPE *pItem, INT32 sGridNo, UINT8 ubLevel, LEVELNODE *pLevelNode );

ITEM_POOL * GetItemPoolForIndex( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel );

INT32 GetFreeFlashItemSlot(void);
void RecountFlashItemSlots(void);
INT32	AddFlashItemSlot( ITEM_POOL *pItemPool, ITEM_POOL_LOCATOR_HOOK Callback, UINT8 ubFlags );
BOOLEAN RemoveFlashItemSlot( ITEM_POOL *pItemPool );

// Disgusting hacks: have to keep track of these values for accesses in callbacks
static SOLDIERTYPE *	gpTempSoldier;
static INT32					gsTempGridNo;
static INT8						bTempFrequency;

void BombMessageBoxCallBack( UINT8 ubExitValue );
void BoobyTrapMessageBoxCallBack( UINT8 ubExitValue );
void SwitchMessageBoxCallBack( UINT8 ubExitValue );
void BoobyTrapDialogueCallBack( void );
void MineSpottedDialogueCallBack( void );
void MineSpottedLocatorCallback( void );
void RemoveBlueFlagDialogueCallBack( UINT8 ubExitValue );
void MineSpottedMessageBoxCallBack( UINT8 ubExitValue );
void CheckForPickedOwnership( void );
void BoobyTrapInMapScreenMessageBoxCallBack( UINT8 ubExitValue );


BOOLEAN ContinuePastBoobyTrap( SOLDIERTYPE * pSoldier, INT32 sGridNo, INT8 bLevel, INT32 iItemIndex, BOOLEAN fInStrategic, BOOLEAN *pfSaidQuote );
extern BOOLEAN ItemIsCool( OBJECTTYPE * pObj );
extern INT8	gbItemPointerSrcSlot;
extern void MAPEndItemPointer( );
extern BOOLEAN	gfResetUIMovementOptimization;

BOOLEAN ItemPoolOKForPickup( SOLDIERTYPE * pSoldier, ITEM_POOL *pItemPool, INT8 bZLevel );

SOLDIERTYPE *		gpBoobyTrapSoldier;
ITEM_POOL *			gpBoobyTrapItemPool;
INT32						gsBoobyTrapGridNo;
INT8			gbBoobyTrapLevel;
BOOLEAN					gfDisarmingBuriedBomb;
extern BOOLEAN	gfDontChargeAPsToPickup;
INT8						gbTrapDifficulty;
BOOLEAN					gfJustFoundBoobyTrap = FALSE;

void StartBombMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo );

BOOLEAN	HandleCheckForBadChangeToGetThrough( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTargetSoldier, INT32 sTargetGridNo , INT8 bLevel ) 
{
	BOOLEAN						fBadChangeToGetThrough = FALSE;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleCheckForBadChangeToGetThrough"));

	if ( pTargetSoldier != NULL )
	{
		if ( SoldierToSoldierBodyPartChanceToGetThrough( pSoldier, pTargetSoldier, pSoldier->bAimShotLocation ) < OK_CHANCE_TO_GET_THROUGH )
		{
			fBadChangeToGetThrough = TRUE;
		}
	}
	else
	{
		if ( SoldierToLocationChanceToGetThrough( pSoldier, sTargetGridNo, (INT8) bLevel, 0, NOBODY ) < OK_CHANCE_TO_GET_THROUGH )
		{
			fBadChangeToGetThrough = TRUE;
		}
	}

	if ( fBadChangeToGetThrough )
	{
		if ( gTacticalStatus.sCantGetThroughSoldierGridNo != pSoldier->sGridNo ||	gTacticalStatus.sCantGetThroughGridNo != sTargetGridNo || gTacticalStatus.ubCantGetThroughID != pSoldier->ubID )
		{
			gTacticalStatus.fCantGetThrough = FALSE;
		}

		// Have we done this once already?
		if ( !gTacticalStatus.fCantGetThrough )
		{
			gTacticalStatus.fCantGetThrough								= TRUE;
			gTacticalStatus.sCantGetThroughGridNo					= sTargetGridNo;
			gTacticalStatus.ubCantGetThroughID						= pSoldier->ubID;
			gTacticalStatus.sCantGetThroughSoldierGridNo	= pSoldier->sGridNo;

			// PLay quote
			TacticalCharacterDialogue( pSoldier, QUOTE_NO_LINE_OF_FIRE );
			return( FALSE );
		}
		else
		{
			// Is this a different case?
			if ( gTacticalStatus.sCantGetThroughGridNo != sTargetGridNo || gTacticalStatus.ubCantGetThroughID != pSoldier->ubID || gTacticalStatus.sCantGetThroughSoldierGridNo != pSoldier->sGridNo )
			{
				// PLay quote
				gTacticalStatus.sCantGetThroughGridNo	= sTargetGridNo;
				gTacticalStatus.ubCantGetThroughID		= pSoldier->ubID;

				TacticalCharacterDialogue( pSoldier, QUOTE_NO_LINE_OF_FIRE );
				return( FALSE );
			}
		}
	}
	else
	{
		gTacticalStatus.fCantGetThrough = FALSE;
	}

	return( TRUE );
}




INT32 HandleItem( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel, UINT16 usHandItem, BOOLEAN fFromUI )
{
	SOLDIERTYPE				*pTargetSoldier = NULL;
	UINT16						usSoldierIndex;
	INT32							sTargetGridNo;
	INT16							sAPCost;
	INT32							sActionGridNo;
	UINT8							ubDirection = 0;
	INT32							sAdjustedGridNo;
	BOOLEAN						fDropBomb = FALSE;
	BOOLEAN						fAddingTurningCost = FALSE;
	BOOLEAN						fAddingRaiseGunCost = FALSE;
	LEVELNODE					*pIntNode;
	STRUCTURE					*pStructure;
	UINT16						usRaiseGunCost = 0;
	UINT16						usTurningCost = 0;


	// Remove any previous actions
	pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

	// here is where we would set a different value if the weapon mode is on
	// "attached weapon"
	pSoldier->usAttackingWeapon = usHandItem;

	// Find soldier flags depend on if it's our own merc firing or a NPC
	//if ( FindSoldier( sGridNo, &usSoldierIndex, &uiMercFlags, FIND_SOLDIER_GRIDNO )  )
	if ( ( usSoldierIndex = WhoIsThere2( sGridNo, bLevel ) ) != NOBODY )
	{
		pTargetSoldier = MercPtrs[ usSoldierIndex ];

		if ( fFromUI )
		{
			INT32 sInteractiveGridNo;

			// ATE: Check if we are targeting an interactive tile, and adjust gridno accordingly...
			pIntNode = GetCurInteractiveTileGridNoAndStructure( &sInteractiveGridNo, &pStructure );

			if ( pIntNode != NULL && pTargetSoldier == pSoldier )
			{
				// Truncate target soldier and update grid
				pTargetSoldier = NULL;
				sGridNo = sInteractiveGridNo;
			}
		}
	}

	// ATE: If in realtime, set attacker count to 0...
	if ( !(gTacticalStatus.uiFlags & INCOMBAT) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Setting attack busy count to 0 due to no combat" ) );
		gTacticalStatus.ubAttackBusyCount = 0;
	}

//	if ( pTargetSoldier )
//	{
//	pTargetSoldier->bBeingAttackedCount = 0;
//	}


	// Check our soldier's life for unconscious!
	if ( pSoldier->stats.bLife < OKLIFE )
	{
		return( ITEM_HANDLE_UNCONSCIOUS );
	}

	if (HandItemWorks( pSoldier, HANDPOS ) == FALSE )
	{
		return( ITEM_HANDLE_BROKEN );
	}

	if ( fFromUI && pSoldier->bTeam == gbPlayerNum && pTargetSoldier && (pTargetSoldier->bTeam == gbPlayerNum || pTargetSoldier->aiData.bNeutral) && pTargetSoldier->ubBodyType != CROW && Item[ usHandItem ].usItemClass != IC_MEDKIT )
	{
		if ( pSoldier->ubProfile != NO_PROFILE	)
		{
			// nice mercs won't shoot other nice guys or neutral civilians
			if ( (gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) && ( (pTargetSoldier->ubProfile == NO_PROFILE && pTargetSoldier->aiData.bNeutral) || gMercProfiles[ pTargetSoldier->ubProfile ].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) )
			{
				TacticalCharacterDialogue( pSoldier, QUOTE_REFUSING_ORDER );
				return( ITEM_HANDLE_REFUSAL );
			}
			if ( pTargetSoldier->ubProfile != NO_PROFILE )
			{
				// buddies won't shoot each other
				if ( WhichBuddy( pSoldier->ubProfile, pTargetSoldier->ubProfile ) != -1 )
				{
					TacticalCharacterDialogue( pSoldier, QUOTE_REFUSING_ORDER );
					return( ITEM_HANDLE_REFUSAL );
				}
			}

			// any recruited rebel will refuse to fire on another rebel or neutral nameless civ
			if ( pSoldier->ubCivilianGroup == REBEL_CIV_GROUP && (pTargetSoldier->ubCivilianGroup == REBEL_CIV_GROUP || ( pTargetSoldier->aiData.bNeutral && pTargetSoldier->ubProfile == NO_PROFILE && pTargetSoldier->ubCivilianGroup == NON_CIV_GROUP && pTargetSoldier->ubBodyType != CROW ) ) )
			{
				TacticalCharacterDialogue( pSoldier, QUOTE_REFUSING_ORDER );
				return( ITEM_HANDLE_REFUSAL );
			}
		}

	}

	// Check HAND ITEM
	if ( Item[ usHandItem ].usItemClass == IC_GUN || Item[ usHandItem ].usItemClass == IC_THROWING_KNIFE )
	{
		// WEAPONS
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleItem: checking for fingerprintID, item id = %d,id required = %d, imprint id = %d, soldier id = %d",usHandItem,Item[usHandItem].fingerprintid,pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID,pSoldier->ubProfile));
		if ( Item[usHandItem].fingerprintid )
		{
			// check imprint ID
			// NB not-imprinted value is NO_PROFILE
			// imprinted value is profile for mercs & NPCs and NO_PROFILE + 1 for generic dudes
			if (pSoldier->ubProfile != NO_PROFILE)
			{
				if ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID != pSoldier->ubProfile )
				{
					if ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID == NO_PROFILE )
					{
						// first shot using "virgin" gun... set imprint ID
						pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID = pSoldier->ubProfile;

						// this could be an NPC (Krott)
						if (pSoldier->bTeam == gbPlayerNum)
						{
							PlayJA2Sample( RG_ID_IMPRINTED, RATE_11025, HIGHVOLUME, 1, MIDDLE );

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"\"%s\"", TacticalStr[ GUN_GOT_FINGERPRINT ] );

							return( ITEM_HANDLE_BROKEN );
						}
					}
					else
					{
						// access denied!
						if (pSoldier->bTeam == gbPlayerNum)
						{
							PlayJA2Sample( RG_ID_INVALID, RATE_11025, HIGHVOLUME, 1, MIDDLE );

							//if (Random( 100 ) < (UINT32) pSoldier->stats.bWisdom)
							//{
							//	pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
							//}
							//else
							//{
							//	TacticalCharacterDialogue( pSoldier, QUOTE_USELESS_ITEM );
							//}
						}
						return( ITEM_HANDLE_BROKEN );
					}
				}
			}
			else
			{
				// guaranteed not to be controlled by the player, so no feedback required
				if ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID != (NO_PROFILE + 1) )
				{
					if ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID == NO_PROFILE )
					{
						pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID = (NO_PROFILE + 1);
					}
					else
					{
						return( ITEM_HANDLE_BROKEN );
					}
				}
			}
		}

		// IF we are not a throwing knife, check for ammo, reloading...
		if ( Item[ usHandItem ].usItemClass != IC_THROWING_KNIFE )
		{
			// CHECK FOR AMMO!
			if ( !EnoughAmmo( pSoldier, fFromUI, HANDPOS ) )
			{
				//ATE: Reflect that we need to reset for bursting
				pSoldier->flags.fDoSpread = FALSE;
				return( ITEM_HANDLE_NOAMMO );
			}

			// Check if we are reloading
			if ( (gTacticalStatus.uiFlags & REALTIME) || !(gTacticalStatus.uiFlags & INCOMBAT) )
			{
				if ( pSoldier->flags.fReloading )
				{
					return( ITEM_HANDLE_RELOADING );
				}
			}
		}

		// Get gridno - either soldier's position or the gridno
		// ??? Why isn't the target's gridno the same as the gridno of the soldier on that grid?
		// Real example:	Soldier fires at 16701.	It finds a soldier there.	That soldier's sGridNo is 16547.	So why
		// isn't this soldier at grid 16547?
		if ( pTargetSoldier != NULL )
		{
			sTargetGridNo	= pTargetSoldier->sGridNo;
		}
		else
		{
			sTargetGridNo	= sGridNo;
		}

		// If it's a player guy, check ChanceToGetThrough to play quote
		if ( fFromUI && (gTacticalStatus.uiFlags & TURNBASED ) && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			// Don't do if no spread!
			if ( !pSoldier->flags.fDoSpread )
			{
				if ( !HandleCheckForBadChangeToGetThrough( pSoldier, pTargetSoldier, sTargetGridNo , bLevel ) )
				{
					return( ITEM_HANDLE_OK );
				}
			}
		}


		// Get AP COSTS
		// ATE: OK something funny going on here - AI seems to NEED FALSE here,
		// Our guys NEED TRUE. We shoulkd at some time make sure the AI and
		// our guys are deducting/checking in the same manner to avoid
		// these differences.
		sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, TRUE, pSoldier->aiData.bAimTime );


		GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sTargetGridNo, TRUE, &fAddingTurningCost, &fAddingRaiseGunCost );
		usTurningCost = CalculateTurningCost(pSoldier, usHandItem, fAddingTurningCost);
		usRaiseGunCost = CalculateRaiseGunCost(pSoldier, fAddingRaiseGunCost);

		// If we are standing and are asked to turn AND raise gun, ignore raise gun...
		//CHRISL: Actually, the display value is based on the higher of turn and raise gun so we should do the same
		if ( gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
		{
			if ( fAddingTurningCost && fAddingRaiseGunCost )
			{
				if(usRaiseGunCost > usTurningCost)
					pSoldier->flags.fDontChargeTurningAPs = TRUE;
				else
					pSoldier->flags.fDontChargeReadyAPs = TRUE;
			}
		}
		/*else
		{
		// If raising gun, don't charge turning!
		if ( fAddingTurningCost )
		{
		pSoldier->flags.fDontChargeReadyAPs = TRUE;
		}
		}*/




		// If this is a player guy, show message about no APS
		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			if ( (pSoldier->ubProfile != NO_PROFILE) && (gMercProfiles[ pSoldier->ubProfile ].bDisability == PSYCHO) )
			{
				// psychos might possibly switch to burst if they can
				// Changed by ADB, rev 1513
				//if ( !pSoldier->bDoBurst && IsGunBurstCapable( pSoldier, HANDPOS, FALSE ) )
				// SANDRO - messed this a little.. decrease morale a bit when we would like to go psycho,
				// but have not a gun capable for it
				if ( PreRandom( 3 + pSoldier->aiData.bAimTime ) == 0 && !pSoldier->bDoBurst )
				{
					if ( IsGunBurstCapable( &pSoldier->inv[HANDPOS], FALSE, pSoldier ) )
					{
						// chance of firing burst if we have points... chance decreasing when ordered to do aimed shot

						// temporarily set burst to true to calculate action points
						pSoldier->bDoBurst = TRUE;
						sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, TRUE, pSoldier->aiData.bAimTime );
						// reset burst mode to false (which is what it was at originally)
						pSoldier->bDoBurst = FALSE;

						if ( EnoughPoints( pSoldier, sAPCost, 0, FALSE ) )
						{
							// we have enough points to do this burst, roll the dice and see if we want to change						
							pSoldier->DoMercBattleSound( BATTLE_SOUND_LAUGH1 );
							pSoldier->bDoBurst = TRUE;
							pSoldier->bWeaponMode = WM_BURST;
							pSoldier->bDoAutofire = 0;

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 26 ], pSoldier->name );
						}
					}
					else if ( !IsGunAutofireCapable( &pSoldier->inv[HANDPOS] ) )
					{
						// reduce morale for we are using a gun without burst or autofire and cannot go psycho
						//HandleMoraleEventForSoldier( pSoldier, MORALE_PSYCHO_UNABLE_TO_PSYCHO );
						HandleMoraleEvent( pSoldier, MORALE_PSYCHO_UNABLE_TO_PSYCHO, pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
					}
				}
			}

			if(pSoldier->bDoAutofire && ((gTacticalStatus.uiFlags & INCOMBAT) && (gTacticalStatus.uiFlags & TURNBASED))) //this is the code that introduces uncertainty into full-auto bursts
			{
				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("HandleItem: auto fire - setting dice sides, marksmanship = %d",pSoldier->stats.bMarksmanship));
				//UINT32 diceSides = RAND_MAX;
				//Madd: tried to make this more marksmanship dependent than agility, a level 10 auto-weapons specialist with 100 in all stats was wasting wayyy too many APs on this fucker
				//UINT32 diceSides = RAND_MAX / ( max(1,pSoldier->stats.bMarksmanship) / 10) ;

				//Kaiden: Had to change the minimum value to 10 instead of 1,
				//Rounding down resulted in division by zero and caused a crash.
				UINT32 diceSides = RAND_MAX / ( max(10,pSoldier->stats.bMarksmanship) / 10) ;

				DOUBLE avgAPadded;
				// SANDRO - Slightly changed this formula to make the auto weapons trait little more needed if new traits activated - 
				if( gGameOptions.fNewTraitSystem )
				{
					// also include possible squadleader bonus
					UINT8 uiEffExpLev = min( 10, (pSoldier->stats.bExpLevel + (gSkillTraitValues.ubSLEffectiveLevelInRadius * GetSquadleadersCountInVicinity( pSoldier, TRUE, FALSE ))));
					avgAPadded = max(((400.0f-2.0f*pSoldier->stats.bDexterity))*(90.0f-5.0f*(uiEffExpLev+gSkillTraitValues.ubAWUnwantedBulletsReduction*NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPONS_NT )))/2700.0f,1); //Important! don't make this zero, the formulae don't like it.
				}
				else
				{
					avgAPadded = max(((400.0f-2.0f*pSoldier->stats.bAgility))*(63.0f-5.0f*(pSoldier->stats.bExpLevel+2.0f*NUM_SKILL_TRAITS( pSoldier, AUTO_WEAPS_OT )))/2700.0f,1); //Important! don't make this zero, the formulae don't like it.
				}

				UINT32 chanceToMisfire = (UINT32)(((DOUBLE)diceSides*(2.0f*avgAPadded+1.0f-sqrt(4.0f*avgAPadded+1.0f)))/(2.0f*avgAPadded)); //derive the chace to misfire from the desired average AP overspent, derived suing
				UINT32 chanceToMisfireDry = (UINT32)(((DOUBLE)diceSides*(avgAPadded+1.0f-sqrt(2.0f*avgAPadded+1.0f)))/(avgAPadded)); //chance to misfire if weapon is dry.	Designed to waste avgAPadded/2 APs
				//soldiers get better at controlling bursts with levels, and reflex time (agility)
				UINT32 misfirePenaltyConst = (GetAutofireShotsPerFiveAPs(&pSoldier->inv[HANDPOS]))/5;
				UINT32 misfirePenaltyRand = ((GetAutofireShotsPerFiveAPs(&pSoldier->inv[HANDPOS]))%5)*20; //this is the remainder translated to a probability
				UINT32 misfirePenalty;
				UINT32 startAuto = pSoldier->bDoAutofire;
				//UINT32 startAPcost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, TRUE, 0 );
				UINT32 roll;

				// SANDRO - changed Random to PreRandom to avoid save-load mania
				if((pSoldier->ubProfile != NO_PROFILE) && (gMercProfiles[ pSoldier->ubProfile ].bDisability == PSYCHO) && PreRandom(100) < 20)
				{
					chanceToMisfire = diceSides;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 26 ], pSoldier->name );

					pSoldier->DoMercBattleSound( BATTLE_SOUND_LAUGH1 ); // Added the laugh sound when going Psycho on autofire - SANDRO
				}

				chanceToMisfire = __min(diceSides-1, chanceToMisfire); //cap the misfire chance, no-one has reflexes this bad
				chanceToMisfire = __max(1, chanceToMisfire); //cap the misfire chance, no-one has reflexes this good

				chanceToMisfireDry = __min(diceSides-1, chanceToMisfireDry); //cap the misfire chance, no-one has reflexes this bad
				chanceToMisfireDry = __max(1, chanceToMisfireDry); //cap the misfire chance, no-one has reflexes this good

				//CHRISL: Since LMGs can only use autofire, give them slightly better control over misfire
				if(Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubWeaponClass == MGCLASS && Weapon[pSoldier->inv[pSoldier->ubAttackingHand].usItem].ubWeaponType == GUN_LMG){
					chanceToMisfire = (INT32)(chanceToMisfire * .75f);
					chanceToMisfireDry = (INT32)(chanceToMisfireDry * .75f);
				}

				//roll dice for each AP to see if we failed to stop firing
				do
				{
					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"HandleItem: auto fire - Rolling dice");
					roll = PreRandom(diceSides); // changed Random to PreRandom - SANDRO
					//roll = rand();//Random(diceSides);
					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Rolled %d vs %d", roll, ((pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire)?chanceToMisfire:chanceToMisfireDry));

					misfirePenalty = misfirePenaltyConst + (Chance(misfirePenaltyRand)?1:0); //apply the base integral cost and the fractional cost (in the form of probablilite)

					pSoldier->bDoAutofire += misfirePenalty;
					sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, TRUE, pSoldier->aiData.bAimTime );
				}
				while(EnoughPoints( pSoldier, sAPCost, 0, FALSE ) && roll < ((pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire)?chanceToMisfire:chanceToMisfireDry));
				//note that we could misfire more bullets than we have rounds
				//this represents the soldier running out of ammo and not noticing
				//the max that can be lost this way is 1AP

				pSoldier->bDoAutofire -= misfirePenalty;
				sAPCost = CalcTotalAPsToAttack( pSoldier, sTargetGridNo, TRUE, pSoldier->aiData.bAimTime );

				if((__min(pSoldier->bDoAutofire,pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft) - startAuto) > 0 && pSoldier->bTeam == OUR_TEAM)
				{
					if (gGameExternalOptions.usBulletHideIntensity > 0)
					{
						// HEADROCK HAM 3.5: Non-accurate assessment.
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113HAMMessage[ 2 ], pSoldier->name );
					}
					else
					{// More than 1 round
						if (__min(pSoldier->bDoAutofire,pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft) - startAuto > 1)
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 62 ], pSoldier->name, __min(pSoldier->bDoAutofire,pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft) - startAuto );
						}
						// 1 round
						else
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 63 ], pSoldier->name ); // SANDRO - no need for attaching the number if only one round is shot
						}
					}
				}


				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Chance:%d.%d, AvgAps=%f, APs=%d", chanceToMisfire/(diceSides/100),chanceToMisfire%(diceSides/100), avgAPadded,sAPCost-startAPcost);

			}

			// Deduct points if our target is different!
			// if attacking a new target (or if the specific target is uncertain)

			// DEF:	Made into an event
			//		pSoldier->EVENT_FireSoldierWeapon( sTargetGridNo );
			if (fFromUI)
			{
				// set the target level; if the AI calls this it will have set the level already...
				pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;
			}

			if ( Item[ usHandItem ].usItemClass != IC_THROWING_KNIFE )
			{
				pSoldier->ubAttackingHand = HANDPOS;
				pSoldier->usAttackingWeapon = usHandItem;
				// If doing spread, set down the first gridno.....
				if ( pSoldier->flags.fDoSpread )
				{
					if ( pSoldier->sSpreadLocations[ 0 ] != 0 )
					{
						SendBeginFireWeaponEvent( pSoldier, pSoldier->sSpreadLocations[ 0 ] );
						if(is_server || (is_client && pSoldier->ubID <20) ) 
							send_fire( pSoldier, pSoldier->sSpreadLocations[ 0 ] );

						//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Handle Items.cpp: SendBeginFireWeaponEvent" );
					}
					else
					{
						SendBeginFireWeaponEvent( pSoldier, sTargetGridNo );
						if(is_server || (is_client && pSoldier->ubID <20) ) 
							send_fire( pSoldier, sTargetGridNo );

						//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Handle Items.cpp: SendBeginFireWeaponEvent" );
					}
				}
				else
				{
					SendBeginFireWeaponEvent( pSoldier, sTargetGridNo );
					if(is_server || (is_client && pSoldier->ubID <20) ) send_fire( pSoldier, sTargetGridNo );
				}

				// ATE: Here to make cursor go back to move after LAW shot...
				if ( fFromUI && Item[usHandItem].singleshotrocketlauncher)
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

			}
			else
			{
				UINT8 ubDirection;
				// Start knife throw attack

				// Get direction
				ubDirection = (UINT8)GetDirectionFromGridNo( sTargetGridNo, pSoldier );

				pSoldier->EVENT_SoldierBeginKnifeThrowAttack( sTargetGridNo, ubDirection );

			}

			if (fFromUI)
			{
				// Descrease aim by two if in real time
				if ( (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT) )
				{
					//pSoldier->aiData.bShownAimTime -= 2;
					//if ( pSoldier->aiData.bShownAimTime < REFINE_AIM_1 )
					//{
					//		pSoldier->aiData.bShownAimTime = REFINE_AIM_1;
					//}
					//pSoldier->flags.fPauseAim = TRUE;
				}

				// If in turn based - refresh aim to first level
				if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
				{
					pSoldier->aiData.bShownAimTime = REFINE_AIM_1;

					// Locate to soldier if he's about to shoot!
					if ( pSoldier->bTeam != gbPlayerNum	)
					{
						ShowRadioLocator( pSoldier->ubID, SHOW_LOCATOR_NORMAL );
					}

				}
			}

			// OK, set UI
			SetUIBusy( pSoldier->ubID );
		}
		else
		{
			//CHRISL: Need to reset this as it doesn't automatically get reset when we don't have enough APs to shot
			pSoldier->flags.fDoSpread = FALSE;
			return( ITEM_HANDLE_NOAPS );
		}

		return( ITEM_HANDLE_OK );
	}


	//TRY PUNCHING
	if ( Item[ usHandItem ].usItemClass == IC_PUNCH )
	{
		//INT16	sCnt;
		 INT32	sSpot;	
		UINT8		ubGuyThere;
		 INT32		sGotLocation = NOWHERE;
		BOOLEAN	fGotAdjacent = FALSE;

		for ( INT8 sCnt = 0; sCnt < NUM_WORLD_DIRECTIONS; sCnt++ )
		{
			sSpot = NewGridNo( pSoldier->sGridNo, DirectionInc( sCnt ) );

			// Make sure movement costs are OK....
			if ( gubWorldMovementCosts[ sSpot ][ sCnt ][ bLevel ] >= TRAVELCOST_BLOCKED )
			{
				continue;
			}

			// Check for who is there...
			ubGuyThere = WhoIsThere2( sSpot, pSoldier->pathing.bLevel );

			if ( pTargetSoldier != NULL && ubGuyThere == pTargetSoldier->ubID )
			{
				// We've got a guy here....
				// Who is the one we want......
				sGotLocation = sSpot;
				sAdjustedGridNo	= pTargetSoldier->sGridNo;
				ubDirection		= ( UINT8 )sCnt;
				break;
			}
		}
		
		if (TileIsOutOfBounds(sGotLocation))
		{
			// See if we can get there to punch
			sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
			if ( sActionGridNo != -1 )
			{
				// OK, we've got somebody...
				sGotLocation = sActionGridNo;

				fGotAdjacent = TRUE;
			}
		}

		// Did we get a loaction?		
		if (!TileIsOutOfBounds(sGotLocation))
		{
			pSoldier->sTargetGridNo = sGridNo;

			pSoldier->aiData.usActionData	= sGridNo;
			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sGotLocation && fGotAdjacent )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_PUNCH;
				pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;
				pSoldier->aiData.bPendingActionData3	= ubDirection;
				pSoldier->aiData.ubPendingActionAnimCount = 0;

				// WALK UP TO DEST FIRST
				pSoldier->EVENT_InternalGetNewSoldierPath( sGotLocation, pSoldier->usUIMovementMode, FALSE, TRUE );
			}
			else
			{
				pSoldier->aiData.bAction = AI_ACTION_KNIFE_STAB;
				pSoldier->EVENT_SoldierBeginPunchAttack( sAdjustedGridNo, ubDirection );
			}

			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			gfResetUIMovementOptimization = TRUE;

			return( ITEM_HANDLE_OK );
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}

	//USING THE MEDKIT
	if ( Item[ usHandItem ].usItemClass == IC_MEDKIT )
	{
		// ATE: AI CANNOT GO THROUGH HERE!
		INT32 usMapPos;
		BOOLEAN	fHadToUseCursorPos = FALSE;

		if (gTacticalStatus.fAutoBandageMode)
		{
			usMapPos = sGridNo;
		}
		else
		{
			GetMouseMapPos( &usMapPos );
		}

		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo == -1 )
		{
			// Try another location...
			sActionGridNo =	FindAdjacentGridEx( pSoldier, usMapPos, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

			if ( sActionGridNo == -1 )
			{
				return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
			}

			if ( !gTacticalStatus.fAutoBandageMode )
			{
				fHadToUseCursorPos = TRUE;
			}
		}

		// Calculate AP costs...
		sAPCost = GetAPsToBeginFirstAid( pSoldier );
		sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_GIVEAID;

				if ( fHadToUseCursorPos )
				{
					pSoldier->aiData.sPendingActionData2	= usMapPos;
				}
				else
				{
					if ( pTargetSoldier != NULL )
					{
						pSoldier->aiData.sPendingActionData2	= pTargetSoldier->sGridNo;
					}
					else
					{
						pSoldier->aiData.sPendingActionData2	= sGridNo;
					}
				}
				pSoldier->aiData.bPendingActionData3	= ubDirection;
				pSoldier->aiData.ubPendingActionAnimCount = 0;

				// WALK UP TO DEST FIRST
				pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
			}
			else
			{
				pSoldier->EVENT_SoldierBeginFirstAid( sAdjustedGridNo, ubDirection );
			}

			if ( fFromUI )
			{
				guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
			}

			return( ITEM_HANDLE_OK );
		}
		else
		{
			return( ITEM_HANDLE_NOAPS );
		}
	}

	ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( sGridNo, pSoldier->pathing.bLevel );
	BOOLEAN fCorpse = FALSE;
	if(pCorpse != NULL)
		fCorpse = IsValidDecapitationCorpse( pCorpse );
	if ( Item[usHandItem].wirecutters && pTargetSoldier == NULL && !pCorpse ) // Madd: quick fix to allow wirecutter/knives
	{
		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo != -1 )
		{
			// Calculate AP costs...
			sAPCost = GetAPsToCutFence( pSoldier );
			sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

			if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
			{
				// OK, set UI
				// ATE: Set UI here, not below so that if UI is unset, we don't set again!
				SetUIBusy( pSoldier->ubID );
				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_CUTFFENCE;
					pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;
					pSoldier->aiData.bPendingActionData3	= ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode , FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBeginCutFence( sAdjustedGridNo, ubDirection );
				}


				if ( fFromUI )
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

				return( ITEM_HANDLE_OK );
			}
			else
			{
				return( ITEM_HANDLE_NOAPS );
			}
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}

	if ( Item[usHandItem].toolkit )
	{
		UINT8 ubMercID;
		BOOLEAN	fVehicle = FALSE;
		INT32		sVehicleGridNo=-1;

		// For repair, check if we are over a vehicle, then get gridnot to edge of that vehicle!
		if ( IsRepairableStructAtGridNo( sGridNo, &ubMercID ) == 2 )
		{
			INT32 sNewGridNo;
			UINT8	ubDirection;

			sNewGridNo = FindGridNoFromSweetSpotWithStructDataFromSoldier( pSoldier, pSoldier->usUIMovementMode, 5, &ubDirection, 0, MercPtrs[ ubMercID ] );
			
			if (!TileIsOutOfBounds(sNewGridNo))
			{
				sGridNo = sNewGridNo;

				sVehicleGridNo = MercPtrs[ ubMercID ]->sGridNo;

				fVehicle = TRUE;
			}
		}

		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

		if ( sActionGridNo != -1 )
		{
			// Calculate AP costs...
			sAPCost = GetAPsToBeginRepair( pSoldier );
			sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

			if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
			{
				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_REPAIR;
					pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;

					if ( fVehicle )
					{
						pSoldier->aiData.sPendingActionData2	= sVehicleGridNo;
					}

					pSoldier->aiData.bPendingActionData3	= ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBeginRepair( sAdjustedGridNo, ubDirection );
				}

				// OK, set UI
				SetUIBusy( pSoldier->ubID );

				if ( fFromUI )
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

				return( ITEM_HANDLE_OK );
			}
			else
			{
				return( ITEM_HANDLE_NOAPS );
			}
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}

	if ( Item[usHandItem].gascan )
	{
		UINT8 ubMercID;
		 INT32		sVehicleGridNo=-1;

		// For repair, check if we are over a vehicle, then get gridnot to edge of that vehicle!
		if ( IsRefuelableStructAtGridNo( sGridNo, &ubMercID ) )
		{
				INT32 sNewGridNo;
			UINT8	ubDirection;

			sNewGridNo = FindGridNoFromSweetSpotWithStructDataFromSoldier( pSoldier, pSoldier->usUIMovementMode, 5, &ubDirection, 0, MercPtrs[ ubMercID ] );
			
			if (!TileIsOutOfBounds(sNewGridNo))
			{
				sGridNo = sNewGridNo;

				sVehicleGridNo = MercPtrs[ ubMercID ]->sGridNo;

			}
		}

		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

		if ( sActionGridNo != -1 )
		{
			// Calculate AP costs...
			sAPCost = GetAPsToRefuelVehicle( pSoldier );
			sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

			if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
			{
				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_FUEL_VEHICLE;
					pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;

					pSoldier->aiData.sPendingActionData2	= sVehicleGridNo;
					pSoldier->aiData.bPendingActionData3	= ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBeginRefuel( sAdjustedGridNo, ubDirection );
				}

				// OK, set UI
				SetUIBusy( pSoldier->ubID );

				if ( fFromUI )
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

				return( ITEM_HANDLE_OK );
			}
			else
			{
				return( ITEM_HANDLE_NOAPS );
			}
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}


	if ( Item[usHandItem].jar )
	{
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

		if ( sActionGridNo != -1 )
		{
			// Calculate AP costs...
			sAPCost = GetAPsToUseJar( pSoldier, sActionGridNo );
			sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

			if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
			{
				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_TAKEBLOOD;
					pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;
					pSoldier->aiData.bPendingActionData3	= ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBeginTakeBlood( sAdjustedGridNo, ubDirection );
				}

				// OK, set UI
				SetUIBusy( pSoldier->ubID );

				if ( fFromUI )
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

				return( ITEM_HANDLE_OK );
			}
			else
			{
				return( ITEM_HANDLE_NOAPS );
			}
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}

	if ( Item[usHandItem].canandstring )
	{
		STRUCTURE					*pStructure;
		LEVELNODE					*pIntTile;

		// Get structure info for in tile!
		 pIntTile = GetCurInteractiveTileGridNoAndStructure( &sGridNo, &pStructure );

		// We should not have null here if we are given this flag...
		if ( pIntTile != NULL )
		{
			sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, FALSE, TRUE );

			if ( sActionGridNo != -1 )
			{
				// Calculate AP costs...
				sAPCost = APBPConstants[AP_ATTACH_CAN];
				sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

				if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
				{
					// CHECK IF WE ARE AT THIS GRIDNO NOW
					if ( pSoldier->sGridNo != sActionGridNo )
					{
						// SEND PENDING ACTION
						pSoldier->aiData.ubPendingAction = MERC_ATTACH_CAN;
						pSoldier->aiData.sPendingActionData2	= sGridNo;
						pSoldier->aiData.bPendingActionData3	= ubDirection;
						pSoldier->aiData.ubPendingActionAnimCount = 0;

						// WALK UP TO DEST FIRST
						pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
					}
					else
					{
						pSoldier->EVENT_SoldierBeginTakeBlood( sGridNo, ubDirection );
					}

					// OK, set UI
					SetUIBusy( pSoldier->ubID );

					if ( fFromUI )
					{
						guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
					}

					return( ITEM_HANDLE_OK );
				}
				else
				{
					return( ITEM_HANDLE_NOAPS );
				}
			}
			else
			{
				return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
			}
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}

	// Check for remote detonator cursor....
	if ( Item[ usHandItem ].ubCursor == REMOTECURS )
	{
		sAPCost = APBPConstants[AP_USE_REMOTE];

		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			DeductPoints( pSoldier, sAPCost, 0 );
			if ( Item[usHandItem].xray )
			{
				PlayJA2Sample( USE_X_RAY_MACHINE, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

				ActivateXRayDevice( pSoldier );
				return( ITEM_HANDLE_OK );
			}
			else // detonator
			{
				// Save gridno....
				pSoldier->aiData.sPendingActionData2	= sGridNo;

				pSoldier->EVENT_SoldierBeginUseDetonator( );

				if ( fFromUI )
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

				// Now start anim....
				return( ITEM_HANDLE_OK );
			}
		}
		else
		{
			return( ITEM_HANDLE_NOAPS );
		}
	}


	// Check for mine.. anything without a detonator.....
	if ( Item[ usHandItem ].ubCursor == BOMBCURS )
	{
		fDropBomb = TRUE;
	}

	// Check for a bomb like a mine, that uses a pressure detonator
	if ( Item[ usHandItem ].ubCursor == INVALIDCURS )
	{
		// Found detonator...
		if ( IsDetonatorAttached( &(pSoldier->inv[ pSoldier->ubAttackingHand ] ) )	|| IsRemoteDetonatorAttached( &(pSoldier->inv[ pSoldier->ubAttackingHand ] ) ) )
		{
			fDropBomb = TRUE;
		}
	}

	if ( fDropBomb )
	{
		// Save gridno....
		pSoldier->aiData.sPendingActionData2	= sGridNo;

		if ( pSoldier->sGridNo != sGridNo )
		{
			// SEND PENDING ACTION
			pSoldier->aiData.ubPendingAction = MERC_DROPBOMB;
			pSoldier->aiData.ubPendingActionAnimCount = 0;

			// WALK UP TO DEST FIRST
			pSoldier->EVENT_InternalGetNewSoldierPath( sGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
		}
		else
		{
			pSoldier->EVENT_SoldierBeginDropBomb( );
		}

		// OK, set UI
		SetUIBusy( pSoldier->ubID );

		if ( fFromUI )
		{
			guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
		}

		return( ITEM_HANDLE_OK );
	}

	//USING THE BLADE
	if ( Item[ usHandItem ].usItemClass == IC_BLADE )
	{
		// See if we can get there to stab
		if ( pSoldier->ubBodyType == BLOODCAT )
		{
			sActionGridNo =	FindNextToAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		}
		else if ( CREATURE_OR_BLOODCAT( pSoldier ) && PythSpacesAway( pSoldier->sGridNo, sGridNo ) > 1 )
		{
			sActionGridNo =	FindNextToAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
			if (sActionGridNo == -1)
			{
				sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
			}
		}
		else
		{
			sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		}

		if ( sActionGridNo != -1 )
		{
			pSoldier->aiData.usActionData = sActionGridNo;

			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_KNIFEATTACK;
				pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;
				pSoldier->aiData.bPendingActionData3	= ubDirection;
				pSoldier->aiData.ubPendingActionAnimCount = 0;

				// WALK UP TO DEST FIRST
				pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
			}
			else
			{
				// for the benefit of the AI
				pSoldier->aiData.bAction = AI_ACTION_KNIFE_STAB;
				pSoldier->EVENT_SoldierBeginBladeAttack( sAdjustedGridNo, ubDirection );
			}

			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			if ( fFromUI )
			{
				guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				gfResetUIMovementOptimization = TRUE;
			}

			return( ITEM_HANDLE_OK );
		}
		else
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}
	}


	if ( Item[ usHandItem ].usItemClass == IC_TENTACLES )
	{
		// See if we can get there to stab
		//pSoldier->sTargetGridNo = sTargetGridNo;
		//pSoldier->sLastTarget = sTargetGridNo;
		//pSoldier->ubTargetID = WhoIsThere2( sTargetGridNo, pSoldier->bTargetLevel );

//		gTacticalStatus.ubAttackBusyCount++;
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!!!! Starting swipe attack, incrementing a.b.c in HandleItems to %d", gTacticalStatus.ubAttackBusyCount) );
		DebugAttackBusy( "Swipe attack\n");


		sAPCost = CalcTotalAPsToAttack( pSoldier, sGridNo, FALSE, pSoldier->aiData.bAimTime );

		DeductPoints( pSoldier, sAPCost, 0 );

		pSoldier->EVENT_InitNewSoldierAnim( QUEEN_SWIPE, 0 , FALSE );

		//FireWeapon( pSoldier, sTargetGridNo );
		pSoldier->aiData.bAction = AI_ACTION_KNIFE_STAB;

		return( ITEM_HANDLE_OK );
	}


	// THIS IS IF WE WERE FROM THE UI
	if ( Item[ usHandItem ].usItemClass == IC_GRENADE || Item[ usHandItem ].usItemClass == IC_LAUNCHER || Item[ usHandItem ].usItemClass == IC_THROWN )
	{
		INT32 sCheckGridNo;

		// Get gridno - either soldier's position or the gridno
		if ( pTargetSoldier != NULL )
		{
			sTargetGridNo	= pTargetSoldier->sGridNo;
		}
		else
		{
			sTargetGridNo	= sGridNo;
		}

		sAPCost = MinAPsToAttack( pSoldier, sTargetGridNo, TRUE );

		// Check if these is room to place mortar!
		if ( Item[usHandItem].mortar )
		{
			ubDirection = (UINT8)GetDirectionFromGridNo( sTargetGridNo, pSoldier );

			// Get new gridno!
			sCheckGridNo = NewGridNo( pSoldier->sGridNo, (UINT16)DirectionInc( ubDirection ) );

			if ( !OKFallDirection( pSoldier, sCheckGridNo, pSoldier->pathing.bLevel, ubDirection , pSoldier->usAnimState ) )
			{
				return( ITEM_HANDLE_NOROOM );
			}

			pSoldier->flags.fDontChargeAPsForStanceChange = TRUE;
		}
		else if ( Item[usHandItem].grenadelauncher )//usHandItem == GLAUNCHER || usHandItem == UNDER_GLAUNCHER )
		{
			GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sTargetGridNo, TRUE, &fAddingTurningCost, &fAddingRaiseGunCost );
			usTurningCost = CalculateTurningCost(pSoldier, usHandItem, fAddingTurningCost);
			usRaiseGunCost = CalculateRaiseGunCost(pSoldier, fAddingRaiseGunCost);

			// If we are standing and are asked to turn AND raise gun, ignore raise gun...
			//CHRISL: Actually, the display value is based on the higher of turn and raise gun so we should do the same
			if ( gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
			{
				if ( fAddingTurningCost && fAddingRaiseGunCost )
				{
					if(usRaiseGunCost > usTurningCost)
						pSoldier->flags.fDontChargeTurningAPs = TRUE;
					else
						pSoldier->flags.fDontChargeReadyAPs = TRUE;
				}
			}
			/*else
			{
			// If raising gun, don't charge turning!
			if ( fAddingTurningCost )
			{
			pSoldier->flags.fDontChargeReadyAPs = TRUE;
			}
			}*/
		}

		// If this is a player guy, show message about no APS
		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			pSoldier->ubAttackingHand = HANDPOS;
			pSoldier->usAttackingWeapon = usHandItem;
			pSoldier->bTargetLevel	= bLevel;

			// Look at the cursor, if toss cursor...
			if ( Item[ usHandItem ].ubCursor == TOSSCURS )
			{
				pSoldier->sTargetGridNo = sTargetGridNo;
				//	pSoldier->sLastTarget = sTargetGridNo;
				pSoldier->ubTargetID = WhoIsThere2( sTargetGridNo, pSoldier->bTargetLevel );

				// Increment attack counter...
//				gTacticalStatus.ubAttackBusyCount++;
				DebugAttackBusy( "Weapon fire\n");


				// ATE: Don't charge turning...
				pSoldier->flags.fDontChargeTurningAPs = TRUE;
				pSoldier->flags.fDontChargeAPsForStanceChange = TRUE;

				FireWeapon( pSoldier, sTargetGridNo );
			}
			else
			{

				SendBeginFireWeaponEvent( pSoldier, sTargetGridNo );
				if(is_server || (is_client && pSoldier->ubID <20) ) send_fire( pSoldier, sTargetGridNo );

			}

			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			return( ITEM_HANDLE_OK );

		}
		else
		{
			return( ITEM_HANDLE_NOAPS );
		}

		return( ITEM_HANDLE_OK );
	}

	// CHECK FOR BOMB....
	if ( Item[ usHandItem ].ubCursor == INVALIDCURS )
	{
		// Found detonator...
		if ( IsDetonatorAttached ( &(pSoldier->inv[ usHandItem ] ) )	|| IsRemoteDetonatorAttached( &(pSoldier->inv[ usHandItem ] ) ) )
		{
			StartBombMessageBox( pSoldier, sGridNo );

			if ( fFromUI )
			{
				guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
			}

			return( ITEM_HANDLE_OK );
		}
	}

	return( ITEM_HANDLE_OK );
}


void HandleSoldierDropBomb( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	// Does this have detonator that needs info?
	if ( IsDetonatorAttached ( &(pSoldier->inv[ HANDPOS ] ) ) || IsRemoteDetonatorAttached( &(pSoldier->inv[ HANDPOS ] ) ) )
	{
		StartBombMessageBox( pSoldier, sGridNo );
	}
	else
	{
		// We have something... all we do is place...
		if ( ArmBomb( &(pSoldier->inv[ HANDPOS ]), 0 ) )
		{
			// Snap: Do a skill check here as well
			INT32 iResult = SkillCheck( pSoldier, PLANTING_BOMB_CHECK, 0 );

			if ( iResult >= 0 )
			{
				// EXPLOSIVES GAIN (25):	Place a bomb, or buried and armed a mine
				StatChange( pSoldier, EXPLODEAMT, 25, FALSE );

				// SANDRO - STOMP traits - Demolitions bonus to trap level
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( pSoldier, DEMOLITIONS_NT ))
				{
					// +5 trap level for Demolitions trait
					pSoldier->inv[ HANDPOS ][0]->data.bTrap = __min( max( 10, (8 + gSkillTraitValues.ubDEPlacedBombLevelBonus)), (( EffectiveExplosive( pSoldier ) / 20) + (EffectiveExpLevel( pSoldier ) / 3) + gSkillTraitValues.ubDEPlacedBombLevelBonus) );
				}
				else
				{
					pSoldier->inv[ HANDPOS ][0]->data.bTrap = __min( 10, ( EffectiveExplosive( pSoldier ) / 20) + (EffectiveExpLevel( pSoldier ) / 3) );
				}
				pSoldier->inv[ HANDPOS ][0]->data.misc.ubBombOwner = pSoldier->ubID + 2;

				// we now know there is something nasty here
				gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;

				if (pSoldier->inv[ HANDPOS ].MoveThisObjectTo(gTempObject, 1) == 0) {
					AddItemToPool( sGridNo, &gTempObject, BURIED, pSoldier->pathing.bLevel, WORLD_ITEM_ARMED_BOMB, 0 );
				}
			}
			else
			{
				// EXPLOSIVES GAIN (10):	Failed to place a bomb, or bury and arm a mine
				StatChange( pSoldier, EXPLODEAMT, 10, FROM_FAILURE );

				// oops!	How badly did we screw up?
				if ( iResult < -20 )
				{
					// OOPS! ... BOOM!
					IgniteExplosion( NOBODY, pSoldier->sX, pSoldier->sY, (INT16) (gpWorldLevelData[pSoldier->sGridNo].sHeight), pSoldier->sGridNo, pSoldier->inv[ HANDPOS ].usItem, pSoldier->pathing.bLevel );
					pSoldier->inv[ HANDPOS ].MoveThisObjectTo(gTempObject, 1);
				}
			}
		}
	}
}

void HandleSoldierUseRemote( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	StartBombMessageBox( pSoldier, sGridNo );
}

void SoldierHandleDropItem( SOLDIERTYPE *pSoldier )
{
	// LOOK IN PANDING DATA FOR ITEM TO DROP, AND LOCATION
	if ( pSoldier->pTempObject != NULL )
	{
		if ( pSoldier->bVisible != -1 )
		{
			PlayJA2Sample( THROW_IMPACT_2, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		}

		AddItemToPool( pSoldier->sGridNo, pSoldier->pTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );
		NotifySoldiersToLookforItems( );

		OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
	}
}


void HandleSoldierThrowItem( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	// Determine what to do
	UINT8 ubDirection;

	// Set attacker to NOBODY, since it's not a combat attack
	pSoldier->ubTargetID = NOBODY;

	// Alrighty, switch based on stance!
	switch( gAnimControl[ pSoldier->usAnimState ].ubHeight )
	{
	case ANIM_STAND:

		// CHECK IF WE ARE NOT ON THE SAME GRIDNO
		if ( sGridNo == pSoldier->sGridNo )
		{
			pSoldier->PickDropItemAnimation( );
		}
		else
		{
			// CHANGE DIRECTION AT LEAST
			ubDirection = (UINT8)GetDirectionFromGridNo( sGridNo, pSoldier );

			pSoldier->SoldierGotoStationaryStance( );

			pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
			pSoldier->flags.fTurningUntilDone = TRUE;

			// Draw item depending on distance from buddy
			if ( GetRangeFromGridNoDiff( sGridNo, pSoldier->sGridNo ) < MIN_LOB_RANGE )
			{
				
				//ddd вожможно еще надо добавить условие проверки на класс кидаемого предмета - гранаты
				if( (pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM) && 
					( (pSoldier->ubBodyType == BIGMALE) || (pSoldier->ubBodyType == REGMALE) ) )
					pSoldier->usPendingAnimation = LOB_GRENADE_STANCE;
				else
					pSoldier->usPendingAnimation = LOB_ITEM;
				
			}
			else
			{  //ddd вожможно еще надо добавить условие проверки на класс кидаемого предмета - гранаты
				//INT32 uiItemClass; //такой код тоже работает. фальшвеер и граната из одного класса?
				//	uiItemClass= Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass;
				//if( (uiItemClass == IC_GRENADE) && 


				if( (pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM) && 
					( (pSoldier->ubBodyType == BIGMALE) || (pSoldier->ubBodyType == REGMALE) ) )
					pSoldier->usPendingAnimation = THROW_GRENADE_STANCE;
				else
					pSoldier->usPendingAnimation = THROW_ITEM;
			}

		}
		break;

		//<SB> crouch throwing
	case ANIM_PRONE:
			if ( sGridNo == pSoldier->sGridNo )
			{
				// OK, JUST DROP ITEM!
				if ( pSoldier->pTempObject != NULL )
				{
					AddItemToPool( sGridNo, pSoldier->pTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
					NotifySoldiersToLookforItems( );

					OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
				}
				break;
			}
			pSoldier->ChangeSoldierStance( ANIM_CROUCH );

		case ANIM_CROUCH:
		// CHECK IF WE ARE NOT ON THE SAME GRIDNO
		if ( sGridNo == pSoldier->sGridNo )
		{
			// OK, JUST DROP ITEM!
			if ( pSoldier->pTempObject != NULL )
			{
				AddItemToPool( sGridNo, pSoldier->pTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
				NotifySoldiersToLookforItems( );

				OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
			}
		}
		else
		{
				// CHANGE DIRECTION AT LEAST
			ubDirection = (UINT8)GetDirectionFromGridNo( sGridNo, pSoldier );

				pSoldier->SoldierGotoStationaryStance( );

			pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
				pSoldier->flags.fTurningUntilDone = TRUE;

				pSoldier->usPendingAnimation = THROW_ITEM_CROUCHED;
		}
			break;
//</SB>
	}

}


void SoldierGiveItem( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTargetSoldier, OBJECTTYPE *pObject, INT8 bInvPos )
{
	INT32 sActionGridNo, sAdjustedGridNo;
	UINT8	ubDirection;

	// Remove any previous actions
	pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

	// See if we can get there to stab
	sActionGridNo =	FindAdjacentGridEx( pSoldier, pTargetSoldier->sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
	if ( sActionGridNo != -1 )
	{
		// SEND PENDING ACTION
		pSoldier->aiData.ubPendingAction = MERC_GIVEITEM;

		pSoldier->bPendingActionData5 = bInvPos;
		// Copy temp object
		OBJECTTYPE::CopyToOrCreateAt(&pSoldier->pTempObject, pObject);

		pSoldier->aiData.sPendingActionData2	= pTargetSoldier->sGridNo;
		pSoldier->aiData.bPendingActionData3	= ubDirection;
		pSoldier->aiData.uiPendingActionData4 = pTargetSoldier->ubID;
		pSoldier->aiData.ubPendingActionAnimCount = 0;

		// Set soldier as engaged!
		pSoldier->flags.uiStatusFlags |= SOLDIER_ENGAGEDINACTION;

		// CHECK IF WE ARE AT THIS GRIDNO NOW
		if ( pSoldier->sGridNo != sActionGridNo )
		{
			// WALK UP TO DEST FIRST
			pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
		}
		else
		{
			pSoldier->EVENT_SoldierBeginGiveItem( );
			// CHANGE DIRECTION OF TARGET TO OPPOSIDE DIRECTION!
			pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );
		}

		// Set target as engaged!
		pTargetSoldier->flags.uiStatusFlags |= SOLDIER_ENGAGEDINACTION;

		return;
	}
	else
	{
		return;
	}
}

BOOLEAN SoldierDropItem( SOLDIERTYPE * pSoldier, OBJECTTYPE * pObj )
{
	OBJECTTYPE::CopyToOrCreateAt(&pSoldier->pTempObject, pObj);
	pSoldier->PickDropItemAnimation( );
	return( TRUE );
}

void SoldierPickupItem( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel )
{
	INT32 sActionGridNo;

	// Remove any previous actions
	pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

	sActionGridNo = AdjustGridNoForItemPlacement( pSoldier, sGridNo );

	// SET PENDING ACTIONS!
	pSoldier->aiData.ubPendingAction = MERC_PICKUPITEM;
	pSoldier->aiData.uiPendingActionData1 = iItemIndex;
	pSoldier->aiData.sPendingActionData2	= sActionGridNo;
	pSoldier->aiData.uiPendingActionData4 = sGridNo;
	pSoldier->aiData.bPendingActionData3	= bZLevel;
	pSoldier->aiData.ubPendingActionAnimCount = 0;

	// Deduct points!
	//sAPCost = GetAPsToPickupItem( pSoldier, sGridNo );
	//DeductPoints( pSoldier, sAPCost, 0 );
	SetUIBusy( pSoldier->ubID );

	// CHECK IF NOT AT SAME GRIDNO
	if ( pSoldier->sGridNo != sActionGridNo )
	{
		if ( pSoldier->bTeam == gbPlayerNum )
		{
			pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, TRUE, TRUE );

			// Say it only if we don;t have to go too far!
			if ( pSoldier->pathing.usPathDataSize > 5 )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_OK1 );
			}
		}
		else
		{
			pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
		}
	}
	else
	{
		// DO ANIMATION OF PICKUP NOW!
		PickPickupAnimation( pSoldier, pSoldier->aiData.uiPendingActionData1, pSoldier->aiData.uiPendingActionData4, pSoldier->aiData.bPendingActionData3 );
	}

}


void HandleAutoPlaceFail( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo )
{
	if (pSoldier->bTeam == gbPlayerNum)
	{
		// Place it in buddy's hand!
		if ( gpItemPointer == NULL )
		{
			InternalBeginItemPointer( pSoldier, &(gWorldItems[ iItemIndex ].object ), NO_SLOT );
		}
		else
		{
			// Add back to world...
			AddItemToPool( sGridNo, &(gWorldItems[ iItemIndex ].object ), 1 , pSoldier->pathing.bLevel, 0, -1 );

			// If we are a merc, say DAMN quote....
			if ( pSoldier->bTeam == gbPlayerNum )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
			}
		}
	}
}

void SoldierGetItemFromWorld( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel, BOOLEAN *pfSelectionList )
{
	ITEM_POOL		*		pItemPool;
	ITEM_POOL		*		pItemPoolToDelete = NULL;
	INT32						cnt = 0;
	BOOLEAN					fPickup;
	BOOLEAN					fFailedAutoPlace = FALSE;
	INT32						iItemIndexToDelete;
	BOOLEAN					fShouldSayCoolQuote = FALSE;
	BOOLEAN					fDidSayCoolQuote = FALSE;
	BOOLEAN		 fSaidBoobyTrapQuote = FALSE;
#ifdef JA2UB
	UINT16					usItem=0;
#endif
	// OK. CHECK IF WE ARE DOING ALL IN THIS POOL....
	if ( iItemIndex == ITEM_PICKUP_ACTION_ALL || iItemIndex == ITEM_PICKUP_SELECTION )
	{
		// DO all pickup!
		// LOOP THROUGH LIST TO FIND NODE WE WANT
		GetItemPool( sGridNo, &pItemPool, pSoldier->pathing.bLevel );

		while( pItemPool )
		{
			if ( ItemPoolOKForPickup( pSoldier, pItemPool, bZLevel ) )
			{
				fPickup = TRUE;

				if ( iItemIndex == ITEM_PICKUP_SELECTION )
				{
					//CHRISL: This can pose a problem.  If the next value in pfSelectionList didn't exist (which can happen if
					//	we've added an item to the world during this process), it'll pull whatever is in memory which is most
					//	likely a value larger then 0.
					//if ( !pfSelectionList[ cnt ] )
					if(pfSelectionList[cnt] != 1)
					{
						fPickup = FALSE;
					}
				}

				// Increment counter...
				//:ATE: Only incremrnt counter for items we can see..
				cnt++;

				if ( fPickup )
				{
					if ( ContinuePastBoobyTrap( pSoldier, sGridNo, bZLevel, pItemPool->iItemIndex, FALSE, &fSaidBoobyTrapQuote ) )
					{
						if ( ItemIsCool( &gWorldItems[ pItemPool->iItemIndex ].object ) )
						{
							fShouldSayCoolQuote = TRUE;
						}

						if (gWorldItems[ pItemPool->iItemIndex ].object.usItem == SWITCH)
						{
							// ask about activating the switch!
							bTempFrequency = gWorldItems[ pItemPool->iItemIndex ].object[0]->data.misc.bFrequency;
							gpTempSoldier = pSoldier;
							DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ ACTIVATE_SWITCH_PROMPT ] , GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, SwitchMessageBoxCallBack, NULL );
							pItemPool = pItemPool->pNext;
						}
						else
						{
							//CHRISL: Make sure non-ammo item status is a valid 0-100 number
							if(Item[gWorldItems[pItemPool->iItemIndex].object.usItem].usItemClass != IC_AMMO){
								gWorldItems[pItemPool->iItemIndex].object[0]->data.objectStatus = __max(0,gWorldItems[pItemPool->iItemIndex].object[0]->data.objectStatus);
								gWorldItems[pItemPool->iItemIndex].object[0]->data.objectStatus = __min(100,gWorldItems[pItemPool->iItemIndex].object[0]->data.objectStatus);
							}

							// Make copy of item
							gTempObject = gWorldItems[ pItemPool->iItemIndex ].object;

							if ( !AutoPlaceObject( pSoldier, &gTempObject, TRUE ) )
							{
								// check to see if the object has been swapped with one in inventory
								if ( gTempObject.usItem != gWorldItems[ pItemPool->iItemIndex ].object.usItem || gTempObject.ubNumberOfObjects != gWorldItems[ pItemPool->iItemIndex ].object.ubNumberOfObjects )
								{
									// copy back because item changed, and we must make sure the item pool reflects this.
									gWorldItems[ pItemPool->iItemIndex ].object = gTempObject;
								}

								pItemPoolToDelete = pItemPool;
								pItemPool = pItemPool->pNext;
								fFailedAutoPlace = TRUE;
								// continue, to try and place ay others...
								continue;
							}
							/*
							// handle theft.. will return true if theft has failed ( if soldier was caught )
							if( pSoldier->bTeam == OUR_TEAM )
							{
							// check to see if object was owned by another
							if( gTempObject.fFlags & OBJECT_OWNED_BY_CIVILIAN )
							{
							// owned by a civilian
							if( HandleLoyaltyAdjustmentForRobbery( pSoldier ) == TRUE )
							{
							// implememnt actual tactical reaction for theft..shoot robber, yell out, etc
							}

							// reset who owns object
							gTempObject.fFlags &= ~( OBJECT_OWNED_BY_CIVILIAN );
							}
							}
							*/

							//pItemPoolToDelete = pItemPool;
							iItemIndexToDelete = pItemPool->iItemIndex;
							pItemPool = pItemPool->pNext;
							RemoveItemFromPool( sGridNo, iItemIndexToDelete, pSoldier->pathing.bLevel );
						}
					}
					else
					{
						// boobytrap found... stop picking up things!
						break;
					}
				}
				else
				{
					pItemPool = pItemPool->pNext;
				}
			}
			else
			{
				pItemPool = pItemPool->pNext;
			}
		}

		// ATE; If here, and we failed to add any more stuff, put failed one in our cursor...
		if ( pItemPoolToDelete != NULL && fFailedAutoPlace )
		{
			gfDontChargeAPsToPickup = TRUE;
			HandleAutoPlaceFail( pSoldier, pItemPoolToDelete->iItemIndex, sGridNo );
			RemoveItemFromPool( sGridNo, pItemPoolToDelete->iItemIndex, pSoldier->pathing.bLevel );
			pItemPoolToDelete = NULL;
		}
	}
	else
	{
		// REMOVE ITEM FROM POOL
		if ( ItemExistsAtLocation( sGridNo, iItemIndex, pSoldier->pathing.bLevel ) )
		{

			if ( ContinuePastBoobyTrap( pSoldier, sGridNo, bZLevel, iItemIndex, FALSE, &fSaidBoobyTrapQuote ) )
			{
				if ( ItemIsCool( &gWorldItems[ iItemIndex ].object ) )
				{
					fShouldSayCoolQuote = TRUE;
				}

				if (gWorldItems[ iItemIndex ].object.usItem == SWITCH)
				{
					// handle switch
					bTempFrequency = gWorldItems[ iItemIndex ].object[0]->data.misc.bFrequency;
					gpTempSoldier = pSoldier;
					DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ ACTIVATE_SWITCH_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, SwitchMessageBoxCallBack, NULL );
				}
				else
				{
					//CHRISL: Make sure non-ammo item status is a valid 0-100 number
					if(Item[gWorldItems[iItemIndex].object.usItem].usItemClass != IC_AMMO){
						gWorldItems[iItemIndex].object[0]->data.objectStatus = __max(0,gWorldItems[iItemIndex].object[0]->data.objectStatus);
						gWorldItems[iItemIndex].object[0]->data.objectStatus = __min(100,gWorldItems[iItemIndex].object[0]->data.objectStatus);
					}

					/*
					// handle theft.. will return true if theft has failed ( if soldier was caught )
					if( pSoldier->bTeam == OUR_TEAM )
					{
					// check to see if object was owned by another
					if( gTempObject.fFlags & OBJECT_OWNED_BY_CIVILIAN )
					{
					// owned by a civilian
					if( HandleLoyaltyAdjustmentForRobbery( pSoldier ) == TRUE )
					{
					// implememnt actual tactical reaction for theft..shoot robber, yell out, etc
					}

					// reset who owns object
					gTempObject.fFlags &= ~( OBJECT_OWNED_BY_CIVILIAN );
					}
					}
					*/
					RemoveItemFromPool( sGridNo, iItemIndex, pSoldier->pathing.bLevel );

					if ( !AutoPlaceObject( pSoldier, &(gWorldItems[ iItemIndex ].object ), TRUE ) )
					{
						//ADB well we made an animation, should we still use up points?
						gfDontChargeAPsToPickup = TRUE;
						HandleAutoPlaceFail( pSoldier, iItemIndex, sGridNo );
					}
				}

			}
		}
	}

	// OK, check if potentially a good candidate for cool quote
	if ( fShouldSayCoolQuote && pSoldier->bTeam == gbPlayerNum )
	{
		// Do we have this quote..?
		if ( QuoteExp[ pSoldier->ubProfile ].QuoteExpGotGunOrUsedGun == QUOTE_FOUND_SOMETHING_SPECIAL )
		{
			// Have we not said it today?
			if ( !( pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE ) )
			{
				// set flag
				pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE;

				// Say it....
				// We've found something!
				TacticalCharacterDialogue( pSoldier, QUOTE_FOUND_SOMETHING_SPECIAL );

				fDidSayCoolQuote = TRUE;
			}
		}
	}
#ifdef JA2UB	
	//JA25 ub
	//if the item is valid
	if( usItem != 0 )
	{
		//handle the picking up of a new ja25 gun
		HandleNewGunComment( pSoldier, usItem, TRUE );
	}
#endif
	// Aknowledge....
	if( pSoldier->bTeam == OUR_TEAM && !fDidSayCoolQuote && !fSaidBoobyTrapQuote )
	{
		pSoldier->DoMercBattleSound( BATTLE_SOUND_GOTIT );
	}


	// OK partner......look for any hidden items!
	if ( pSoldier->bTeam == gbPlayerNum && LookForHiddenItems( sGridNo, pSoldier->pathing.bLevel, TRUE, 0 ) )
	{
		// WISDOM GAIN (5):	Found a hidden object
		StatChange( pSoldier, WISDOMAMT, 5, FALSE );

		// We've found something!
		TacticalCharacterDialogue( pSoldier, (UINT16)( QUOTE_SPOTTED_SOMETHING_ONE + Random( 2 ) ) );
	}

	gpTempSoldier = pSoldier;
	gsTempGridNo = sGridNo;
	SetCustomizableTimerCallbackAndDelay( 1000, CheckForPickedOwnership, TRUE );
}


void HandleSoldierPickupItem( SOLDIERTYPE *pSoldier, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel )
{
	ITEM_POOL		*pItemPool;
	UINT16				usNum;

	// Draw menu if more than one item!
	if ( GetItemPool( sGridNo, &pItemPool, pSoldier->pathing.bLevel ) )
	{
		// OK, if an enemy, go directly ( skip menu )
		if ( pSoldier->bTeam != gbPlayerNum )
		{
			// HEADROCK HAM 3.5: On-screen message when militia pick up items.
			if ( pSoldier->bTeam == MILITIA_TEAM )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113HAMMessage[4], Item[gWorldItems[ pItemPool->iItemIndex ].object.usItem].szItemName );
			}
			SoldierGetItemFromWorld( pSoldier, iItemIndex, sGridNo, bZLevel, NULL );
		}
		else
		{
			if (gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT ||
				(is_networked && is_client && FindWorldItemForBombInGridNo( sGridNo, pSoldier->pathing.bLevel ) != -1)) // 20091029 - OJW - Disarm player planted bombs!
			{
				// have the computer ask us if we want to proceed

				// override the item index passed in with the one for the bomb in this
				// tile
				iItemIndex = FindWorldItemForBombInGridNo( sGridNo, pSoldier->pathing.bLevel );
#ifdef JA2TESTVERSION
				if (iItemIndex == -1)
				{
					// WTF????
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_ERROR, L"Cannot find bomb item in gridno %d", sGridNo );
					return;
				}
#endif

				gpBoobyTrapItemPool = GetItemPoolForIndex( sGridNo, iItemIndex, pSoldier->pathing.bLevel );
				gpBoobyTrapSoldier = pSoldier;
				gsBoobyTrapGridNo = sGridNo;
				gbBoobyTrapLevel	= pSoldier->pathing.bLevel;
				gfDisarmingBuriedBomb = TRUE;
				gbTrapDifficulty = gWorldItems[ iItemIndex ].object[0]->data.bTrap;

				// WDS - Use local buffer for storing modified string instead of original copy
				CHAR16 buffer[99];
				wchar_t * ptr;
				wcscpy(buffer,TacticalStr[ DISARM_TRAP_PROMPT ]);
				if((gTacticalStatus.uiFlags & INCOMBAT) || (gTacticalStatus.fEnemyInSector))
				{
					wchar_t * string = L" (";
					wchar_t string2[20];

					ptr = wcscat(buffer, string);
					_ltow(GetAPsToDisarmMine( pSoldier ), string2, 10); // SANDRO
					ptr = wcscat(buffer, string2);
					string = L"AP)";
					ptr = wcscat(buffer, string);
				}
				else
				{
					ptr = wcscat(buffer, L"");
				}

				DoMessageBox( MSG_BOX_BASIC_STYLE, ptr, GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack, NULL );
			}
			else
			{
				// OK, only hidden items exist...
				if ( pSoldier->bTeam == gbPlayerNum && DoesItemPoolContainAllHiddenItems( pItemPool ) )
				{
					// He's touched them....
					if ( LookForHiddenItems( sGridNo, pSoldier->pathing.bLevel, TRUE, 0 ) )
					{
						// WISDOM GAIN (5):	Found a hidden object
						StatChange( pSoldier, WISDOMAMT, 5, FALSE );

						// We've found something!
						TacticalCharacterDialogue( pSoldier, (UINT16)( QUOTE_SPOTTED_SOMETHING_ONE + Random( 2 ) ) );
					}
					else
					{
						// Say NOTHING quote...
						pSoldier->DoMercBattleSound( BATTLE_SOUND_NOTHING );
					}
				}
				else
				{
					// If only one good item exists....
					if ( ( usNum = GetNumOkForDisplayItemsInPool( pItemPool, bZLevel ) ) == 1 )
					{
						// Find first OK item....
						while( !ItemPoolOKForDisplay( pItemPool, bZLevel	) )
						{
							pItemPool = pItemPool->pNext;
						}
						SoldierGetItemFromWorld( pSoldier, pItemPool->iItemIndex, sGridNo, bZLevel, NULL );
					}
					else
					{
						if ( usNum != 0 )
						{
							// Freeze guy!
							pSoldier->flags.fPauseAllAnimation = TRUE;

							InitializeItemPickupMenu( pSoldier, sGridNo, pItemPool, 0, 0, bZLevel );

							guiPendingOverrideEvent = G_GETTINGITEM;
						}
						else
						{
							pSoldier->DoMercBattleSound( BATTLE_SOUND_NOTHING );
						}
					}
				}
			}
		}
	}
	else
	{
		// Say NOTHING quote...
		pSoldier->DoMercBattleSound( BATTLE_SOUND_NOTHING );
	}

}


LEVELNODE *AddItemGraphicToWorld( INVTYPE *pItem, INT32 sGridNo, UINT8 ubLevel )
{
	UINT16			usTileIndex;
	LEVELNODE		*pNode;

	usTileIndex = GetTileGraphicForItem( pItem );

	// OK, Do stuff differently base on level!
	if ( ubLevel == 0 )
	{
		pNode = AddStructToTail( sGridNo, usTileIndex );
		//SET FLAG FOR AN ITEM
		pNode->uiFlags |= LEVELNODE_ITEM;
	}
	else
	{
		AddOnRoofToHead( sGridNo, usTileIndex );
		//SET FLAG FOR AN ITEM
		pNode = gpWorldLevelData[ sGridNo ].pOnRoofHead;
		pNode->uiFlags |= LEVELNODE_ITEM;
	}

	// DIRTY INTERFACE
	fInterfacePanelDirty = DIRTYLEVEL2;

	// DIRTY TILE
	gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_REDRAW;
	SetRenderFlags(RENDER_FLAG_MARKED);

	return( pNode );
}


void RemoveItemGraphicFromWorld( INVTYPE *pItem, INT32 sGridNo, UINT8 ubLevel, LEVELNODE *pLevelNode )
{
	LEVELNODE *pNode;

	// OK, Do stuff differently base on level!
	// Loop through and find pointer....
	if ( ubLevel == 0 )
	{
		pNode = gpWorldLevelData[ sGridNo ].pStructHead;
	}
	else
	{
		pNode = gpWorldLevelData[ sGridNo ].pOnRoofHead;
	}

	while( pNode != NULL )
	{
		if ( pNode == pLevelNode )
		{
			// Found one!
			if ( ubLevel == 0 )
			{
				RemoveStructFromLevelNode( sGridNo, pNode );
			}
			else
			{
				RemoveOnRoofFromLevelNode( sGridNo, pNode );
			}

			break;
		}

		pNode = pNode->pNext;
	}

	// DIRTY INTERFACE
	fInterfacePanelDirty = DIRTYLEVEL2;

	// DIRTY TILE
	gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_REDRAW;
	SetRenderFlags(RENDER_FLAG_MARKED);

	//TEMP RENDER FULL!!!
	SetRenderFlags(RENDER_FLAG_FULL);
}

// INVENTORY POOL STUFF
OBJECTTYPE* AddItemToPool( INT32 sGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 soldierID )
{
	return InternalAddItemToPool( &sGridNo, pObject, bVisible, ubLevel, usFlags, bRenderZHeightAboveLevel, soldierID, NULL );
}

OBJECTTYPE * AddItemToPoolAndGetIndex( INT32 sGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 soldierID, INT32 * piItemIndex )
{
	return( InternalAddItemToPool( &sGridNo, pObject, bVisible, ubLevel, usFlags, bRenderZHeightAboveLevel, soldierID, piItemIndex ) );
}

OBJECTTYPE* InternalAddItemToPool( INT32 *psGridNo, OBJECTTYPE *pObject, INT8 bVisible, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 soldierID, INT32 * piItemIndex )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;
	INT32				iWorldItem = 0;
	STRUCTURE		*pStructure, *pBase;
	INT16				sDesiredLevel;
	INT32       sNewGridNo = *psGridNo;
	LEVELNODE		*pNode;
	BOOLEAN			fForceOnGround = FALSE;
	BOOLEAN			fObjectInOpenable = FALSE;
	INT8		bTerrainID;

	// ATE: Check if the gridno is OK	
	if (TileIsOutOfBounds((*psGridNo)))
	{
		// Display warning.....
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Error: Item %d was given invalid grid location %d for item pool. Please Report.", pObject->usItem, (*psGridNo) );

		(*psGridNo) = sNewGridNo = gMapInformation.sCenterGridNo;

		// If no center grid number exists, shrug it off
		if (sNewGridNo == -1)
		{
			return( NULL );
		}
	}

	// CHECK IF THIS ITEM IS IN DEEP WATER....
	// IF SO, CHECK IF IT SINKS...
	// IF SO, DONT'T ADD!
	bTerrainID = GetTerrainType( *psGridNo );

	if ( TERRAIN_IS_WATER( bTerrainID) )
	{
		//		if ( Item[ pObject->usItem ].fFlags & ITEM_SINKS )
		if ( Item[ pObject->usItem ].sinks	)
		{
			return( NULL );
		}
	}

	// First things first - look at where we are to place the items, and
	// set some flags appropriately

	// On a structure?
	//Locations on roofs without a roof is not possible, so
	//we convert the onroof intention to ground.
	if( ubLevel && !FlatRoofAboveGridNo( *psGridNo ) )
	{
		ubLevel = 0;
	}

	if ( bRenderZHeightAboveLevel == -1 )
	{
		fForceOnGround = TRUE;
		bRenderZHeightAboveLevel = 0;
	}

	//CHRISL: We need to make sure that item stacks follow the OldInv stacking limits
	// Copy the object we're working with the gTempObject so that we can use pObject later in this function
	OBJECTTYPE poolObject = OBJECTTYPE(*pObject);
	UINT8 ubSlotLimit = ItemSlotLimit(&poolObject, STACK_SIZE_LIMIT);
	while(poolObject.ubNumberOfObjects>0)
	{
		poolObject.MoveThisObjectTo(*pObject,ubSlotLimit);

		// Check structure database
		if ( gpWorldLevelData[ *psGridNo ].pStructureHead && (pObject->usItem != OWNERSHIP) && (pObject->usItem != ACTION_ITEM) )
		{
			// Something is here, check obstruction in future
			sDesiredLevel = ubLevel ? STRUCTURE_ON_ROOF : STRUCTURE_ON_GROUND;
			pStructure = FindStructure( *psGridNo , STRUCTURE_BLOCKSMOVES );
			while( pStructure )
			{
				if( !(pStructure->fFlags &( STRUCTURE_PERSON | STRUCTURE_CORPSE ) ) && pStructure->sCubeOffset == sDesiredLevel )
				{
					// If we are going into a raised struct AND we have above level set to -1
					if ( StructureBottomLevel( pStructure ) != 1 && fForceOnGround )
					{
						break;
					}

					// Adjust the item's gridno to the base of struct.....
					pBase = FindBaseStructure( pStructure );

					// Get LEVELNODE for struct and remove!
					sNewGridNo = pBase->sGridNo;

					// Check for openable flag....
					if ( pStructure->fFlags & STRUCTURE_OPENABLE )
					{
						// ATE: Set a flag here - we need to know later that we're in an openable...
						fObjectInOpenable = TRUE;

						// Something of note is here....
						// SOME sort of structure is here.... set render flag to off
						usFlags |= WORLD_ITEM_DONTRENDER;

						// Openable.. check if it's closed, if so, set visiblity...
						if ( !( pStructure->fFlags & STRUCTURE_OPEN ) )
						{
							// -2 means - don't reveal!
							bVisible = -2;
						}

						bRenderZHeightAboveLevel = CONVERT_INDEX_TO_PIXELS( StructureHeight( pStructure ) );
						break;

					}
					// Else can we place an item on top?
					else if ( pStructure->fFlags & ( STRUCTURE_GENERIC ) )
					{
						UINT8 ubLevel0, ubLevel1, ubLevel2, ubLevel3;

						// If we are going into a raised struct AND we have above level set to -1
						if ( StructureBottomLevel( pStructure ) != 1 && fForceOnGround )
						{
							break;
						}

						// Find most dence area...
						if ( StructureDensity( pStructure, &ubLevel0, &ubLevel1, &ubLevel2, &ubLevel3 ) )
						{
							if ( ubLevel3 == 0 && ubLevel2 == 0 && ubLevel1 == 0 && ubLevel0 == 0 )
							{
								bRenderZHeightAboveLevel = 0;
							}
							else if ( ubLevel3 >= ubLevel0 && ubLevel3 >= ubLevel2 && ubLevel3 >= ubLevel1 )
							{
								bRenderZHeightAboveLevel = CONVERT_INDEX_TO_PIXELS( 4 );
							}
							else if ( ubLevel2 >= ubLevel0 && ubLevel2 >= ubLevel1 && ubLevel2 >= ubLevel3 )
							{
								bRenderZHeightAboveLevel = CONVERT_INDEX_TO_PIXELS( 3 );
							}
							else if ( ubLevel1 >= ubLevel0 && ubLevel1 >= ubLevel2 && ubLevel1 >= ubLevel3 )
							{
								bRenderZHeightAboveLevel = CONVERT_INDEX_TO_PIXELS( 2 );
							}
							else if ( ubLevel0 >= ubLevel1 && ubLevel0 >= ubLevel2 && ubLevel0 >= ubLevel3 )
							{
								bRenderZHeightAboveLevel = CONVERT_INDEX_TO_PIXELS( 1 );
							}
						}

						// Set flag indicating it has an item on top!
						pStructure->fFlags |= STRUCTURE_HASITEMONTOP;
						break;
					}

				}

				pStructure = FindNextStructure( pStructure, STRUCTURE_BLOCKSMOVES );
			}
		}

		if (pObject->usItem == SWITCH && !fObjectInOpenable )
		{
			if (bVisible != -2)
			{
				// switch items which are not hidden inside objects should be considered buried
				bVisible = BURIED;
				// and they are pressure-triggered unless there is a switch structure there
				if (FindStructure( *psGridNo, STRUCTURE_SWITCH ) != NULL)
				{
					(*pObject)[0]->data.misc.bDetonatorType = BOMB_SWITCH;
				}
				else
				{
					(*pObject)[0]->data.misc.bDetonatorType = BOMB_PRESSURE;
				}
			}
			else
			{
				// else they are manually controlled
				(*pObject)[0]->data.misc.bDetonatorType = BOMB_SWITCH;
			}
		}
		else if ( pObject->usItem == ACTION_ITEM )
		{
			switch( (*pObject)[0]->data.misc.bActionValue )
			{
			case ACTION_ITEM_SMALL_PIT:
			case ACTION_ITEM_LARGE_PIT:
				// mark as known about by civs and creatures
				gpWorldLevelData[ sNewGridNo ].uiFlags |= MAPELEMENT_ENEMY_MINE_PRESENT;
				break;
			default:
				break;
			}
		}

		if ( *psGridNo != sNewGridNo )
		{
			*psGridNo = sNewGridNo;
		}


		//First add the item to the global list.	This is so the game can keep track
		//of where the items are, for file i/o, etc.
		iWorldItem = AddItemToWorld( *psGridNo,	pObject, ubLevel, usFlags, bRenderZHeightAboveLevel, bVisible, soldierID );

		// Check for and existing pool on the object layer
		if ( GetItemPool( *psGridNo, &pItemPool, ubLevel ) )
		{

			// Add to exitsing pool
			// Add graphic
			pNode = AddItemGraphicToWorld( &(Item[ pObject->usItem ] ), *psGridNo, ubLevel );

			// Set pool head value in levelnode
			pNode->pItemPool = pItemPool;

			// Add New Node
			pItemPoolTemp = pItemPool;
			// Create new pool
			pItemPool = (ITEM_POOL *)MemAlloc( sizeof( ITEM_POOL ) );

			// Set Next to NULL
			pItemPool->pNext = NULL;
			// Set Item index
			pItemPool->iItemIndex = iWorldItem;
			// Get a link back!
			pItemPool->pLevelNode = pNode;

			if( pItemPoolTemp )
			{
				// Get last item in list
				while( pItemPoolTemp->pNext != NULL )
					pItemPoolTemp = pItemPoolTemp->pNext;

				// Set Next of previous
				pItemPoolTemp->pNext = pItemPool;
			}
			// Set Previous of new one
			pItemPool->pPrev = pItemPoolTemp;

		}
		else
		{
			pNode = AddItemGraphicToWorld( &(Item[ pObject->usItem ] ), *psGridNo, ubLevel );

			// Create new pool
			pItemPool = (ITEM_POOL *)MemAlloc( sizeof( ITEM_POOL ) );

			pNode->pItemPool = pItemPool;

			// Set prev to NULL
			pItemPool->pPrev = NULL;
			// Set next to NULL
			pItemPool->pNext = NULL;
			// Set Item index
			pItemPool->iItemIndex = iWorldItem;
			// Get a link back!
			pItemPool->pLevelNode = pNode;

			// Set flag to indicate item pool presence
			gpWorldLevelData[*psGridNo].uiFlags |= MAPELEMENT_ITEMPOOL_PRESENT;
		}

		// Set visible!
		pItemPool->bVisible = bVisible;

		// If bbisible is true, render makered world
		if ( bVisible == 1 && GridNoOnScreen( (*psGridNo ) ) )
		{
			//gpWorldLevelData[*psGridNo].uiFlags|=MAPELEMENT_REDRAW;
			//SetRenderFlags(RENDER_FLAG_MARKED);
			SetRenderFlags(RENDER_FLAG_FULL);
		}

		// Set flahs timer
		pItemPool->bFlashColor							= FALSE;
		pItemPool->sGridNo									= *psGridNo;
		pItemPool->ubLevel									= ubLevel;
		pItemPool->usFlags									= usFlags;
		pItemPool->bVisible									= bVisible;
		pItemPool->bRenderZHeightAboveLevel	= bRenderZHeightAboveLevel;

		// ATE: Get head of pool again....
		if ( GetItemPool( *psGridNo, &pItemPool, ubLevel ) )
		{
			AdjustItemPoolVisibility( pItemPool );
		}

		if (piItemIndex)
		{
			*piItemIndex = iWorldItem;
		}
	}
	return( &(gWorldItems[ iWorldItem ].object ) );
}


BOOLEAN ItemExistsAtLocation( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;

	// Check for an existing pool on the object layer
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{
		// LOOP THROUGH LIST TO FIND NODE WE WANT
		pItemPoolTemp = pItemPool;
		while( pItemPoolTemp != NULL )
		{
			if ( pItemPoolTemp->iItemIndex == iItemIndex )
			{
				return( TRUE );
			}
			pItemPoolTemp = pItemPoolTemp->pNext;
		}

	}

	return( FALSE );
}

BOOLEAN ItemTypeExistsAtLocation( INT32 sGridNo, UINT16 usItem, UINT8 ubLevel, INT32 * piItemIndex )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;

	// Check for an existing pool on the object layer
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{
		// LOOP THROUGH LIST TO FIND ITEM WE WANT
		pItemPoolTemp = pItemPool;
		while( pItemPoolTemp != NULL )
		{
			if ( gWorldItems[ pItemPoolTemp->iItemIndex ].object.usItem == usItem )
			{
				if ( piItemIndex )
				{
					*piItemIndex = pItemPoolTemp->iItemIndex;
				}
				return( TRUE );
			}
			pItemPoolTemp = pItemPoolTemp->pNext;
		}

	}

	return( FALSE );
}

BOOLEAN MarblesExistAtLocation( INT32 sGridNo, UINT8 ubLevel, INT32 * piItemIndex )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;

	// Check for an existing pool on the object layer
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{
		// LOOP THROUGH LIST TO FIND ITEM WE WANT
		pItemPoolTemp = pItemPool;
		while( pItemPoolTemp != NULL )
		{
			if ( Item[gWorldItems[ pItemPoolTemp->iItemIndex ].object.usItem].marbles )
			{
				if ( piItemIndex )
				{
					*piItemIndex = pItemPoolTemp->iItemIndex;
				}
				return( TRUE );
			}
			pItemPoolTemp = pItemPoolTemp->pNext;
		}

	}

	return( FALSE );
}


INT32 GetItemOfClassTypeInPool( INT32 sGridNo, UINT32 uiItemClass, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;

	// Check for an existing pool on the object layer
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{
		// LOOP THROUGH LIST TO FIND NODE WE WANT
		pItemPoolTemp = pItemPool;
		while( pItemPoolTemp != NULL )
		{
			if ( Item[ gWorldItems[ pItemPoolTemp->iItemIndex ].object.usItem ].usItemClass & uiItemClass )
			{
				return( pItemPoolTemp->iItemIndex );
			}
			pItemPoolTemp = pItemPoolTemp->pNext;
		}

	}

	return( -1 );
}

ITEM_POOL * GetItemPoolForIndex( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;

	// Check for an existing pool on the object layer
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{
		// LOOP THROUGH LIST TO FIND NODE WE WANT
		pItemPoolTemp = pItemPool;
		while( pItemPoolTemp != NULL )
		{
			if (pItemPoolTemp->iItemIndex == iItemIndex )
			{
				return( pItemPoolTemp );
			}
			pItemPoolTemp = pItemPoolTemp->pNext;
		}

	}

	return( NULL );
}



BOOLEAN DoesItemPoolContainAnyHiddenItems( ITEM_POOL *pItemPool )
{
	// LOOP THROUGH LIST TO FIND NODE WE WANT
	while( pItemPool != NULL )
	{
		if ( gWorldItems[ pItemPool->iItemIndex ].bVisible == HIDDEN_ITEM )
		{
			return( TRUE );
		}

		pItemPool = pItemPool->pNext;
	}

	return( FALSE );
}

BOOLEAN DoesItemPoolContainAllHiddenItems( ITEM_POOL *pItemPool )
{
	// LOOP THROUGH LIST TO FIND NODE WE WANT
	while( pItemPool != NULL )
	{
		if ( gWorldItems[ pItemPool->iItemIndex ].bVisible != HIDDEN_ITEM )
		{
			return( FALSE );
		}

		pItemPool = pItemPool->pNext;
	}

	return( TRUE );
}


BOOLEAN LookForHiddenItems( INT32 sGridNo, INT8 ubLevel, BOOLEAN fSetLocator, INT8 bZLevel )
{
	ITEM_POOL *pItemPool = NULL;
	ITEM_POOL *pHeadItemPool = NULL;
	BOOLEAN		fFound = FALSE;

	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{
		pHeadItemPool = pItemPool;

		// LOOP THROUGH LIST TO FIND NODE WE WANT
		while( pItemPool != NULL )
		{
			if ( gWorldItems[ pItemPool->iItemIndex ].bVisible == HIDDEN_ITEM && gWorldItems[ pItemPool->iItemIndex ].object.usItem != OWNERSHIP )
			{
				fFound = TRUE;

				gWorldItems[ pItemPool->iItemIndex ].bVisible = INVISIBLE;
			}

			pItemPool = pItemPool->pNext;
		}
	}

	// If found, set item pool visibility...
	if ( fFound )
	{
		SetItemPoolVisibilityOn( pHeadItemPool, INVISIBLE, fSetLocator );
	}

	return( fFound );
}


INT8 GetZLevelOfItemPoolGivenStructure( INT32 sGridNo, UINT8 ubLevel, STRUCTURE *pStructure )
{
	ITEM_POOL *pItemPool;

	if ( pStructure == NULL )
	{
		return( 0 );
	}

	// OK, check if this struct contains items....
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) == TRUE )
	{
		return( GetLargestZLevelOfItemPool( pItemPool ) );
	}
	return( 0 );
}

INT8 GetLargestZLevelOfItemPool( ITEM_POOL *pItemPool )
{
	// OK, loop through pools and get any height != 0........
	while( pItemPool != NULL )
	{
		if ( pItemPool->bRenderZHeightAboveLevel > 0 )
		{
			return( pItemPool->bRenderZHeightAboveLevel );
		}

		pItemPool = pItemPool->pNext;
	}

	return( 0 );
}

void RemoveItemPool( INT32 sGridNo, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;

	// Check for and existing pool on the object layer
	while ( GetItemPool( sGridNo, &pItemPool, ubLevel ) == TRUE )
	{
		RemoveItemFromPool( sGridNo, pItemPool->iItemIndex, ubLevel );
	}
}

void RemoveAllUnburiedItems( INT32 sGridNo, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;

	// Check for and existing pool on the object layer
	GetItemPool( sGridNo, &pItemPool, ubLevel );

	while( pItemPool )
	{
		if ( gWorldItems[ pItemPool->iItemIndex ].bVisible == BURIED)
		{
			pItemPool = pItemPool->pNext;
		}
		else
		{
			RemoveItemFromPool( sGridNo, pItemPool->iItemIndex, ubLevel );
			// get new start pointer
			GetItemPool( sGridNo, &pItemPool, ubLevel );
		}
	}
}


void LoopLevelNodeForShowThroughFlag( LEVELNODE *pNode, INT32 sGridNo, UINT8 ubLevel )
{
	while ( pNode != NULL )
	{
		if ( pNode->uiFlags & LEVELNODE_ITEM )
		{
			if ( ubLevel == 0 )
			{
				// If we are in a room....
				// if ( IsRoofPresentAtGridno( sGridNo ) || gfCaves || gfBasement )
				{
					pNode->uiFlags |= LEVELNODE_SHOW_THROUGH;
				}
			}
			else
			{
				pNode->uiFlags |= LEVELNODE_SHOW_THROUGH;
			}

			if( gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] )
			{
				pNode->uiFlags |= LEVELNODE_DYNAMIC;
			}

		}
		pNode = pNode->pNext;
	}
}

void HandleItemObscuredFlag( INT32 sGridNo, UINT8 ubLevel )
{
	LEVELNODE *pNode;

	if ( ubLevel == 0 )
	{
		pNode = gpWorldLevelData[ sGridNo ].pStructHead;
		LoopLevelNodeForShowThroughFlag( pNode, sGridNo,	ubLevel );
	}
	else
	{
		pNode = gpWorldLevelData[ sGridNo ].pOnRoofHead;
		LoopLevelNodeForShowThroughFlag( pNode, sGridNo, ubLevel );
	}
}


BOOLEAN SetItemPoolVisibilityOn( ITEM_POOL *pItemPool, INT8 bAllGreaterThan, BOOLEAN fSetLocator )
{
	ITEM_POOL		*pItemPoolTemp;
	BOOLEAN			fAtLeastModified = FALSE, fDeleted = FALSE;
	INT8				bVisibleValue;
	//OBJECTTYPE *pObj;

	pItemPoolTemp = pItemPool;
	while( pItemPoolTemp != NULL )
	{
		bVisibleValue = gWorldItems[ pItemPoolTemp->iItemIndex ].bVisible;

		// Update each item...
		if ( bVisibleValue != VISIBLE )
		{
			if ( gWorldItems[ pItemPoolTemp->iItemIndex ].object.usItem == ACTION_ITEM )
			{
				// NEVER MAKE VISIBLE!
				pItemPoolTemp = pItemPoolTemp->pNext;
				continue;
			}

			// If we have reached a visible value we should not modify, ignore...
			if ( bVisibleValue >= bAllGreaterThan && gWorldItems[ pItemPoolTemp->iItemIndex ].object.usItem != OWNERSHIP )
			{
				// Update the world value
				gWorldItems[ pItemPoolTemp->iItemIndex ].bVisible = VISIBLE;

				fAtLeastModified = TRUE;
			}

#ifdef obsoleteCode
			/*
			if ( gWorldItems[ pItemPoolTemp->iItemIndex ].object.usItem == ACTION_ITEM )
			{
			pObj = &(gWorldItems[ pItemPoolTemp->iItemIndex ].object);
			switch( (*pObj)[0]->data.misc.bActionValue )
			{
			case ACTION_ITEM_SMALL_PIT:
			case ACTION_ITEM_LARGE_PIT:
			if ((*pObj)[0]->data.misc.bDetonatorType == 0)
			{
			// randomly set to active or destroy the item!
			if (Random( 100 ) < 65)
			{
			ArmBomb( pObj, 0 ); // will be set to pressure type so freq is irrelevant
			gWorldItems[ pItemPoolTemp->iItemIndex ].usFlags |= WORLD_ITEM_ARMED_BOMB;
			AddBombToWorld( pItemPoolTemp->iItemIndex );
			}
			else
			{
			// get pointer to the next element NOW
			pItemPoolTemp	= pItemPoolTemp->pNext;
			// set flag so we don't traverse an additional time
			fDeleted = TRUE;
			// remove item from pool
			RemoveItemFromPool( pItemPool->sGridNo, pItemPool->iItemIndex, pItemPool->ubLevel );
			}
			}
			break;
			default:
			break;
			}
			}
			*/
#endif //obsoleteCode

			if (fDeleted)
			{
				// don't get the 'next' pointer because we did so above

				// reset fDeleted to false so we don't skip moving through the list more than once
				fDeleted = FALSE;
			}
			else
			{
				pItemPoolTemp						= pItemPoolTemp->pNext;
			}

		}
		else
		{
			pItemPoolTemp						= pItemPoolTemp->pNext;
		}
	}

	// If we didn;t find any that should be modified..
	if ( !fAtLeastModified )
	{
		return( FALSE );
	}


	// Update global pool bVisible to true ( if at least one is visible... )
	pItemPoolTemp = pItemPool;
	while( pItemPoolTemp != NULL )
	{
		pItemPoolTemp->bVisible = VISIBLE;

		pItemPoolTemp						= pItemPoolTemp->pNext;
	}

	// Handle obscured flag...
	HandleItemObscuredFlag( pItemPool->sGridNo, pItemPool->ubLevel );

	if ( fSetLocator )
	{
		SetItemPoolLocator( pItemPool );
	}

	return( TRUE );
}


void SetItemPoolVisibilityHidden( ITEM_POOL *pItemPool )
{
	ITEM_POOL		*pItemPoolTemp;

	pItemPoolTemp = pItemPool;
	while( pItemPoolTemp != NULL )
	{
		// Update the world value
		gWorldItems[ pItemPoolTemp->iItemIndex ].bVisible = HIDDEN_IN_OBJECT;
		pItemPoolTemp->bVisible = HIDDEN_IN_OBJECT;

		pItemPoolTemp						= pItemPoolTemp->pNext;
	}
}

// This determines the overall initial visibility of the pool...
// IF ANY are set to VISIBLE, MODIFY
void AdjustItemPoolVisibility( ITEM_POOL *pItemPool )
{
	ITEM_POOL		*pItemPoolTemp;
	BOOLEAN			fAtLeastModified = FALSE;

	pItemPoolTemp = pItemPool;
	while( pItemPoolTemp != NULL )
	{
		// DEFAULT ITEM POOL TO INVISIBLE....
		pItemPoolTemp->bVisible = INVISIBLE;

		// Update each item...
		// If we have reached a visible value we should not modify, ignore...
		if ( gWorldItems[ pItemPoolTemp->iItemIndex ].bVisible == VISIBLE )
		{
			fAtLeastModified = TRUE;
		}

		pItemPoolTemp						= pItemPoolTemp->pNext;
	}

	// Handle obscured flag...
	HandleItemObscuredFlag( pItemPool->sGridNo, pItemPool->ubLevel );

	// If we didn;t find any that should be modified..
	if ( !fAtLeastModified )
	{
		return;
	}

	// Update global pool bVisible to true ( if at least one is visible... )
	pItemPoolTemp = pItemPool;
	while( pItemPoolTemp != NULL )
	{
		pItemPoolTemp->bVisible = VISIBLE;

		pItemPoolTemp						= pItemPoolTemp->pNext;
	}

	// Handle obscured flag...
	HandleItemObscuredFlag( pItemPool->sGridNo, pItemPool->ubLevel );
}


BOOLEAN RemoveItemFromPool( INT32 sGridNo, INT32 iItemIndex, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;
	ITEM_POOL		*pItemPoolTemp;
	BOOLEAN			fItemFound = FALSE;
	LEVELNODE		*pObject;


	// Check for and existing pool on the object layer
	if ( GetItemPool( sGridNo, &pItemPool, ubLevel ) )
	{

		// REMOVE FROM LIST

		// LOOP THROUGH LIST TO FIND NODE WE WANT
		pItemPoolTemp = pItemPool;
		while( pItemPoolTemp != NULL )
		{
			if ( pItemPoolTemp->iItemIndex == iItemIndex )
			{
				fItemFound = TRUE;
				break;
			}
			pItemPoolTemp = pItemPoolTemp->pNext;
		}

		if ( !fItemFound )
		{
			// COULDNOT FIND ITEM? MAYBE SOMEBODY GOT IT BEFORE WE GOT THERE!
			return( FALSE );
		}

		// REMOVE GRAPHIC
		RemoveItemGraphicFromWorld( &(Item[ gWorldItems[ iItemIndex ].object.usItem ] ), sGridNo, ubLevel, pItemPoolTemp->pLevelNode );

		// IF WE ARE LOCATIONG STILL, KILL LOCATOR!
		if ( pItemPoolTemp->bFlashColor != 0 )
		{
			// REMOVE TIMER!
			RemoveFlashItemSlot( pItemPoolTemp );
		}

		// REMOVE PREV
		if ( pItemPoolTemp->pPrev != NULL )
		{
			pItemPoolTemp->pPrev->pNext = pItemPoolTemp->pNext;
		}

		// REMOVE NEXT
		if ( pItemPoolTemp->pNext != NULL )
		{
			pItemPoolTemp->pNext->pPrev = pItemPoolTemp->pPrev;
		}


		// IF THIS NODE WAS THE HEAD, SET ANOTHER AS HEAD AT THIS GRIDNO
		if ( pItemPoolTemp->pPrev == NULL )
		{
			// WE'RE HEAD
			if ( ubLevel == 0 )
			{
				pObject = gpWorldLevelData[ sGridNo ].pStructHead;
			}
			else
			{
				pObject = gpWorldLevelData[ sGridNo ].pOnRoofHead;
			}

			fItemFound = FALSE;
			// LOOP THORUGH OBJECT LAYER
			while( pObject != NULL )
			{
				if ( pObject->uiFlags & LEVELNODE_ITEM )
				{
					// ADJUST TO NEXT GUY FOR HEAD
					pObject->pItemPool = pItemPoolTemp->pNext;
					fItemFound = TRUE;
				}
				pObject = pObject->pNext;
			}

			if (!fItemFound)
			{
				// THIS WAS THE LAST ITEM IN THE POOL!
				gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_ITEMPOOL_PRESENT);
			}
		}

		// Find any structure with flag set as having items on top.. if this one did...
		if ( pItemPoolTemp->bRenderZHeightAboveLevel > 0 )
		{
			STRUCTURE		*pStructure;
			ITEM_POOL		*pTempPool;

			// Check if an item pool exists here....
			if ( !GetItemPool( pItemPoolTemp->sGridNo, &pTempPool, pItemPoolTemp->ubLevel ) )
			{
				pStructure = FindStructure( pItemPoolTemp->sGridNo , STRUCTURE_HASITEMONTOP );

				if ( pStructure != NULL )
				{
					// Remove...
					pStructure->fFlags &= (~STRUCTURE_HASITEMONTOP );

					// Re-adjust interactive tile...
					BeginCurInteractiveTileCheck( INTILE_CHECK_SELECTIVE );
				}
			}
		}

		AdjustItemPoolVisibility( pItemPoolTemp );

		// DELETE
		MemFree( pItemPoolTemp );

		RemoveItemFromWorld( iItemIndex );

		return( TRUE );
	}

	return( FALSE );
}

BOOLEAN MoveItemPools( INT32 sStartPos, INT32 sEndPos, INT8 bStartLevel, INT8 bEndLevel )
{
	// note, only works between locations on the ground
	ITEM_POOL		*pItemPool;
	WORLDITEM		TempWorldItem;

	// While there is an existing pool
	while( GetItemPool( sStartPos, &pItemPool, bStartLevel ) )
	{
		TempWorldItem = gWorldItems[ pItemPool->iItemIndex ];
		RemoveItemFromPool( sStartPos, pItemPool->iItemIndex, bStartLevel );
		//AddItemToPool( sEndPos, &(TempWorldItem.object), -1, TempWorldItem.ubLevel, TempWorldItem.usFlags, TempWorldItem.bRenderZHeightAboveLevel );
		AddItemToPool( sEndPos, &(TempWorldItem.object), -1, bEndLevel, TempWorldItem.usFlags, TempWorldItem.bRenderZHeightAboveLevel );
	}
	return( TRUE );
}

BOOLEAN	GetItemPool( INT32 usMapPos, ITEM_POOL **ppItemPool, UINT8 ubLevel )
{
	LEVELNODE *pObject;

	if ( ubLevel == 0 )
	{
		pObject = gpWorldLevelData[ usMapPos ].pStructHead;
	}
	else
	{
		pObject = gpWorldLevelData[ usMapPos ].pOnRoofHead;
	}

	(*ppItemPool) = NULL;

	// LOOP THORUGH OBJECT LAYER
	while( pObject != NULL )
	{
		if ( pObject->uiFlags & LEVELNODE_ITEM )
		{
			(*ppItemPool) = pObject->pItemPool;

			//DEF added the check because pObject->pItemPool was NULL which was causing problems
			if( *ppItemPool )
				return( TRUE );
			else
				return( FALSE );
		}

		pObject = pObject->pNext;
	}

	return( FALSE );
}

BOOLEAN	GetItemPoolFromGround( INT32 sMapPos, ITEM_POOL **ppItemPool )
{
	//if we know the level, we can avoid an if, and this function is called alot
	LEVELNODE *pObject = gpWorldLevelData[ sMapPos ].pStructHead;
	//ADB: let's not make 51200 calls to FileWrite ok?
//#ifdef JA2BETAVERSION
//	if ( pObject )
//	{
//		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("gpWorldLevelData, %d %d %d", pObject, sMapPos, (&gpWorldLevelData[25600-1]) + sizeof(MAP_ELEMENT) ) );
//	}
//#endif
	(*ppItemPool) = NULL;

	// LOOP THORUGH OBJECT LAYER
	while( pObject != NULL )
	{
		if ( pObject->uiFlags & LEVELNODE_ITEM )
		{
			(*ppItemPool) = pObject->pItemPool;

			//DEF added the check because pObject->pItemPool was NULL which was causing problems
			if( *ppItemPool )
				return( TRUE );
			else
				return( FALSE );
		}

		pObject = pObject->pNext;
	}

	return( FALSE );
}

void NotifySoldiersToLookforItems( )
{
	UINT32 cnt;
	SOLDIERTYPE *pSoldier;

	for ( cnt = 0; cnt < guiNumMercSlots; cnt++ )
	{
		pSoldier = MercSlots[ cnt ];

		if ( pSoldier != NULL )
		{
			pSoldier->flags.uiStatusFlags |= SOLDIER_LOOKFOR_ITEMS;
		}
	}

}


void AllSoldiersLookforItems( BOOLEAN fShowLocators )
{
	UINT32 cnt;
	SOLDIERTYPE *pSoldier;

	for ( cnt = 0; cnt < guiNumMercSlots; cnt++ )
	{
		pSoldier = MercSlots[ cnt ];

		if ( pSoldier != NULL )
		{
			RevealRoofsAndItems(pSoldier, TRUE, fShowLocators, pSoldier->pathing.bLevel, FALSE );
		}
	}

}


INT16 GetNumOkForDisplayItemsInPool( ITEM_POOL *pItemPool, INT8 bZLevel )
{
	INT32						cnt;

	//Determine total #
	cnt = 0;
	while( pItemPool != NULL )
	{
		if ( ItemPoolOKForDisplay( pItemPool, bZLevel ) )
		{
			cnt++;
		}

		pItemPool = pItemPool->pNext;
	}

	return( (UINT16) cnt );
}


BOOLEAN AnyItemsVisibleOnLevel( ITEM_POOL *pItemPool, INT8 bZLevel )
{
	if ( ( gTacticalStatus.uiFlags & SHOW_ALL_ITEMS ) )
	{
		return( TRUE );
	}

	//Determine total #
	while( pItemPool != NULL )
	{
		if ( pItemPool->bRenderZHeightAboveLevel == bZLevel )
		{
			if ( gWorldItems[ pItemPool->iItemIndex ].bVisible == VISIBLE )
			{
				return( TRUE );
			}
		}

		pItemPool = pItemPool->pNext;

	}

	return( FALSE );
}


BOOLEAN ItemPoolOKForDisplay( ITEM_POOL *pItemPool, INT8 bZLevel )
{
	if (gTacticalStatus.uiFlags&SHOW_ALL_ITEMS)
	{
		return( TRUE );
	}

	// Setup some conditions!
	if ( gWorldItems[ pItemPool->iItemIndex ].bVisible != VISIBLE	)
	{
		return( FALSE );
	}

	// If -1, it means find all
	if ( pItemPool->bRenderZHeightAboveLevel != bZLevel && bZLevel != -1 )
	{
		return( FALSE );
	}

	return( TRUE );
}

BOOLEAN ItemPoolOKForPickup( SOLDIERTYPE * pSoldier, ITEM_POOL *pItemPool, INT8 bZLevel )
{
	if (gTacticalStatus.uiFlags&SHOW_ALL_ITEMS)
	{
		return( TRUE );
	}

	if ( pSoldier->bTeam == gbPlayerNum )
	{
		// Setup some conditions!
		if ( gWorldItems[ pItemPool->iItemIndex ].bVisible != VISIBLE	)
		{
			return( FALSE );
		}
	}

	// If -1, it means find all
	if ( pItemPool->bRenderZHeightAboveLevel != bZLevel && bZLevel != -1 )
	{
		return( FALSE );
	}

	return( TRUE );
}


extern void HandleAnyMercInSquadHasCompatibleStuff( UINT8 ubSquad, OBJECTTYPE *pObject, BOOLEAN fReset );


BOOLEAN DrawItemPoolList( ITEM_POOL *pItemPool, INT32 sGridNo, UINT8 bCommand, INT8 bZLevel, INT16 sXPos, INT16 sYPos )
{
	INT16 sY;
	INVTYPE			*pItem;
	ITEM_POOL		*pTempItemPool;
	CHAR16 pStr[ 100 ];
	INT16		cnt = 0, sHeight = 0;
	INT16	sLargeLineWidth = 0, sLineWidth;
	BOOLEAN			fSelectionDone = FALSE;

	INT8				gbCurrentItemSel = 0;
	INT8				bNumItemsListed = 0;
	INT16				sFontX, sFontY;
	INT16				sLargestLineWidth = 30;
	INT8				bCurStart = 0;
	BOOLEAN			fDoBack;


	// Take a look at each guy in current sqaud and check for compatible ammo...

	// Determine how many there are
	// MOVE HEAD TO CURRENT START
	cnt = 0;
	pTempItemPool = pItemPool;
	while( pTempItemPool != NULL )
	{
		if ( cnt == bCurStart )
		{
			break;
		}

		// ATE: Put some conditions on this....
		if ( ItemPoolOKForDisplay( pTempItemPool, bZLevel ) )
		{
			cnt++;
		}

		pTempItemPool = pTempItemPool->pNext;
	}

	cnt = bCurStart;
	fDoBack = FALSE;
	while( pTempItemPool != NULL )
	{
		// IF WE HAVE MORE THAN THE SET AMOUNT, QUIT NOW!
		if ( cnt == ( bCurStart + NUM_ITEMS_LISTED ) )
		{
			cnt++;
			fDoBack = TRUE;
			break;
		}

		// ATE: Put some conditions on this....
		if ( ItemPoolOKForDisplay( pTempItemPool, bZLevel ) )
		{
			cnt++;
		}

		sHeight += GetFontHeight( SMALLFONT1 ) - 2;

		pTempItemPool = pTempItemPool->pNext;
	}


	pTempItemPool = pItemPool;
	while( pTempItemPool != NULL )
	{
		// ATE: Put some conditions on this....
		if ( ItemPoolOKForDisplay( pTempItemPool, bZLevel ) )
		{
			HandleAnyMercInSquadHasCompatibleStuff( (INT8)CurrentSquad( ), &(gWorldItems[ pTempItemPool->iItemIndex ].object ), FALSE );
		}

		pTempItemPool = pTempItemPool->pNext;
	}

	// IF COUNT IS ALREADY > MAX, ADD A PREV...
	if ( bCurStart >= NUM_ITEMS_LISTED )
	{
		cnt++;
	}

	bNumItemsListed = (INT8)cnt;


	//RENDER LIST!
	// Determine max length
	pTempItemPool = pItemPool;
	while( pTempItemPool != NULL )
	{
		if ( ItemPoolOKForDisplay( pTempItemPool, bZLevel ) )
		{
			// GET ITEM
			pItem = &Item[ gWorldItems[ pTempItemPool->iItemIndex ].object.usItem ];
			// Set string
			if ( gWorldItems[ pTempItemPool->iItemIndex ].object.ubNumberOfObjects > 1 )
			{
				swprintf( pStr, L"%s (%d)", ShortItemNames[ gWorldItems[ pTempItemPool->iItemIndex ].object.usItem ], gWorldItems[ pTempItemPool->iItemIndex ].object.ubNumberOfObjects );
			}
			else
			{
				swprintf( pStr, L"%s", ShortItemNames[ gWorldItems[ pTempItemPool->iItemIndex ].object.usItem ] );
			}

			// Get Width
			sLineWidth = StringPixLength( pStr,SMALLFONT1 );

			if ( sLineWidth > sLargeLineWidth )
			{
				sLargeLineWidth = sLineWidth;
			}
			sLargestLineWidth = sLargeLineWidth;
		}
		pTempItemPool = pTempItemPool->pNext;
	}

	// Determine where our mouse is!
	if ( sXPos > ( SCREEN_WIDTH - sLargestLineWidth ) )
	{
		sFontX = sXPos - sLargestLineWidth;
	}
	else
	{
		sFontX = sXPos + 15;
	}
	sFontY = sYPos;

	// Move up if over interface....
	if ( ( sFontY + sHeight ) > ( INV_INTERFACE_START_Y ) )
	{
		sFontY = ( INV_INTERFACE_START_Y ) - sHeight;
	}

	// Detertime vertiacal center
	sFontY -= ( sHeight / 2 );


	SetFont( SMALLFONT1 );
	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( FONT_MCOLOR_DKGRAY );

	// MOVE HEAD TO CURRENT START
	cnt = 0;
	while( pItemPool != NULL )
	{
		if ( cnt == bCurStart )
		{
			break;
		}

		if ( ItemPoolOKForDisplay( pItemPool, bZLevel ) )
		{
			cnt++;
		}

		pItemPool = pItemPool->pNext;
	}

	// START DISPLAY LOOP
	cnt = bCurStart;
	sY	= sFontY;

	// ADD PREV TO THIS LIST!
	if ( bCurStart >= NUM_ITEMS_LISTED )
	{
		// Set string
		if ( cnt == gbCurrentItemSel )
		{
			SetFontForeground( FONT_MCOLOR_LTGRAY );
		}
		else
		{
			SetFontForeground( FONT_MCOLOR_DKGRAY );
		}
		swprintf( pStr, TacticalStr[ ITEMPOOL_POPUP_PREV_STR ] );
		gprintfdirty( sFontX, sY, pStr );
		mprintf( sFontX, sY, pStr );
		sY += GetFontHeight( SMALLFONT1 ) - 2;
		cnt++;
	}

	while( pItemPool != NULL )
	{
		if ( bCommand == ITEMLIST_HANDLE )
		{
			if ( cnt == gbCurrentItemSel )
			{
				SetFontForeground( FONT_MCOLOR_LTGRAY );
			}
			else
			{
				SetFontForeground( FONT_MCOLOR_DKGRAY );
			}
		}


		if ( ItemPoolOKForDisplay( pItemPool, bZLevel ) )
		{
			// GET ITEM
			pItem = &Item[ gWorldItems[ pItemPool->iItemIndex ].object.usItem ];
			// Set string

			if ( gWorldItems[ pItemPool->iItemIndex ].object.ubNumberOfObjects > 1 )
			{
				swprintf( pStr, L"%s (%d)", ShortItemNames[ gWorldItems[ pItemPool->iItemIndex ].object.usItem ], gWorldItems[ pItemPool->iItemIndex ].object.ubNumberOfObjects );
			}
			else
			{
				swprintf( pStr, L"%s", ShortItemNames[ gWorldItems[ pItemPool->iItemIndex ].object.usItem ] );
			}

			gprintfdirty( sFontX, sY, pStr );
			mprintf( sFontX, sY, pStr );

			sY += GetFontHeight( SMALLFONT1 ) - 2;
			cnt++;
		}
		pItemPool = pItemPool->pNext;


		if ( fDoBack )
		{
			if ( cnt == ( bNumItemsListed - 1) )
			{
				break;
			}
		}

	}
	if ( fDoBack )
	{
		if ( cnt == ( bNumItemsListed - 1) )
		{
			// Set string
			if ( cnt == gbCurrentItemSel )
			{
					SetFontForeground( FONT_MCOLOR_LTGRAY );
				}
				else
				{
					SetFontForeground( FONT_MCOLOR_DKGRAY );
				}
			swprintf( pStr, TacticalStr[ ITEMPOOL_POPUP_MORE_STR ] );
				gprintfdirty( sFontX, sY, pStr );
				mprintf( sFontX, sY, pStr );
			}
			}

	return( fSelectionDone );

}


INT8 GetListMouseHotSpot( INT16 sLargestLineWidth, INT8 bNumItemsListed, INT16 sFontX, INT16 sFontY, INT8 bCurStart )
{
	INT16	cnt = 0;
	INT16	sTestX1, sTestX2, sTestY1, sTestY2;
	INT16	sLineHeight;
	INT8	gbCurrentItemSel = -1;
	INT8	bListedItems;

	sLineHeight = GetFontHeight( SMALLFONT1 ) - 2;

	sTestX1 = sFontX;
	sTestX2 = sFontX + sLargestLineWidth;


	bListedItems = ( bNumItemsListed - bCurStart );


	if ( gusMouseXPos < sTestX1 || gusMouseXPos > sTestX2 )
	{
		gbCurrentItemSel = -1;
	}
	else
	{
		// Determine where mouse is!
		for ( cnt = 0; cnt < bListedItems; cnt++ )
		{
			sTestY1 = sFontY + ( sLineHeight * cnt );
			sTestY2 = sFontY + ( sLineHeight * ( cnt + 1 ) );

			if ( gusMouseYPos > sTestY1 && gusMouseYPos < sTestY2 )
			{
				gbCurrentItemSel = (INT8)cnt;
				break;
			}
		}
	}

	// OFFSET START
	gbCurrentItemSel += bCurStart;

	return( gbCurrentItemSel );
}


void SetItemPoolLocator( ITEM_POOL *pItemPool )
{
	pItemPool->bFlashColor = 59;

	pItemPool->uiTimerID		= AddFlashItemSlot( pItemPool, NULL, 0 );

}

void SetItemPoolLocatorWithCallback( ITEM_POOL *pItemPool, ITEM_POOL_LOCATOR_HOOK Callback )
{
	pItemPool->bFlashColor = 59;

	pItemPool->uiTimerID		= AddFlashItemSlot( pItemPool, Callback, 0 );

}
















/// ITEM POOL INDICATOR FUNCTIONS



INT32 GetFreeFlashItemSlot(void)
{
	UINT32 uiCount;

	for(uiCount=0; uiCount < guiNumFlashItemSlots; uiCount++)
	{
		if(( FlashItemSlots[uiCount].fAllocated == FALSE ) )
			return((INT32)uiCount);
	}

	if(guiNumFlashItemSlots < NUM_ITEM_FLASH_SLOTS )
		return((INT32)guiNumFlashItemSlots++);

	return(-1);
}


void RecountFlashItemSlots(void)
{
	INT32 uiCount;

	for(uiCount=guiNumFlashItemSlots-1; (uiCount >=0) ; uiCount--)
	{
		if( ( FlashItemSlots[uiCount].fAllocated ) )
		{
			guiNumFlashItemSlots=(UINT32)(uiCount+1);
			break;
		}
	}
}


INT32	AddFlashItemSlot( ITEM_POOL *pItemPool, ITEM_POOL_LOCATOR_HOOK Callback, UINT8 ubFlags )
{
	INT32			iFlashItemIndex;

	if( ( iFlashItemIndex = GetFreeFlashItemSlot() )==(-1) )
		return(-1);

	ubFlags |= ITEM_LOCATOR_LOCKED;


	FlashItemSlots[ iFlashItemIndex ].pItemPool	= pItemPool;

	FlashItemSlots[ iFlashItemIndex ].bRadioFrame				= 0;
	FlashItemSlots[ iFlashItemIndex ].uiLastFrameUpdate = GetJA2Clock( );
	FlashItemSlots[ iFlashItemIndex ].Callback				= Callback;
	FlashItemSlots[ iFlashItemIndex ].fAllocated				= TRUE;
	FlashItemSlots[ iFlashItemIndex ].ubFlags						= ubFlags;


	return( iFlashItemIndex );
}


BOOLEAN RemoveFlashItemSlot( ITEM_POOL *pItemPool )
{
	UINT32 uiCount;

	CHECKF( pItemPool != NULL );

	for( uiCount=0; uiCount < guiNumFlashItemSlots; uiCount++)
	{
		if ( FlashItemSlots[ uiCount ].fAllocated )
		{
			if ( FlashItemSlots[ uiCount ].pItemPool == pItemPool )
			{
				FlashItemSlots[ uiCount ].fAllocated = FALSE;

				// Check if we have a callback and call it if so!
				if ( FlashItemSlots[ uiCount ].Callback != NULL )
				{
					FlashItemSlots[ uiCount ].Callback( );
				}

				return( TRUE );
			}
		}
	}

	return( TRUE );
}


void HandleFlashingItems( )
{
	UINT32 cnt;
	ITEM_POOL		*pItemPool;
	LEVELNODE		*pObject;
	ITEM_POOL_LOCATOR	*pLocator;
	BOOLEAN			fDoLocator = FALSE;

	if ( COUNTERDONE( CYCLERENDERITEMCOLOR ) )
	{
		RESETCOUNTER( CYCLERENDERITEMCOLOR );

		for ( cnt = 0; cnt < guiNumFlashItemSlots; cnt++ )
		{
			pLocator	= &( FlashItemSlots[ cnt ] );

			if ( pLocator->fAllocated )
			{
				fDoLocator = TRUE;

				if ( ( pLocator->ubFlags & ITEM_LOCATOR_LOCKED ) )
				{
					if ( gTacticalStatus.fLockItemLocators == FALSE )
					{
						// Turn off!
						pLocator->ubFlags &= (~ITEM_LOCATOR_LOCKED);
					}
					else
					{
						fDoLocator = FALSE;
					}
				}

				if ( fDoLocator )
				{
					pItemPool = pLocator->pItemPool;

					// Update radio locator
					{
						UINT32			uiClock;

						uiClock = GetJA2Clock( );

						// Update frame values!
						if ( ( uiClock - pLocator->uiLastFrameUpdate ) > 80 )
						{
							pLocator->uiLastFrameUpdate = uiClock;

							// Update frame
							pLocator->bRadioFrame++;

							if ( pLocator->bRadioFrame == 5 )
							{
								pLocator->bRadioFrame = 0;
							}
						}
					}

					// UPDATE FLASH COLOR VALUE
					pItemPool->bFlashColor--;

					if ( pItemPool->ubLevel == 0 )
					{
						pObject = gpWorldLevelData[ pItemPool->sGridNo ].pStructHead;
					}
					else
					{
						pObject = gpWorldLevelData[ pItemPool->sGridNo ].pOnRoofHead;
					}

					// LOOP THORUGH OBJECT LAYER
					while( pObject != NULL )
					{
						if ( pObject->uiFlags & LEVELNODE_ITEM )
						{
							if ( pItemPool->bFlashColor == 1 )
							{
								//pObject->uiFlags &= (~LEVELNODE_DYNAMIC);
								//pObject->uiFlags |= ( LEVELNODE_LASTDYNAMIC	);
							}
							else
							{
								//pObject->uiFlags |= LEVELNODE_DYNAMIC;
							}

						}

						pObject = pObject->pNext;
					}

					if ( pItemPool->bFlashColor == 1 )
					{
						pItemPool->bFlashColor = 0;

						// REMOVE TIMER!
						RemoveFlashItemSlot( pItemPool );

						SetRenderFlags( RENDER_FLAG_FULL );
					}
				}
			}
		}

		RecountFlashItemSlots( );

	}

}


void RenderTopmostFlashingItems( )
{
	UINT32 cnt;
	ITEM_POOL		*pItemPool;
	ITEM_POOL_LOCATOR	*pLocator;

	for ( cnt = 0; cnt < guiNumFlashItemSlots; cnt++ )
	{
		pLocator	= &( FlashItemSlots[ cnt ] );

		if ( pLocator->fAllocated )
		{
			if ( !( pLocator->ubFlags & ( ITEM_LOCATOR_LOCKED ) ) )
			{
				pItemPool = pLocator->pItemPool;

				// Update radio locator
				{
					FLOAT				dOffsetX, dOffsetY;
					FLOAT				dTempX_S, dTempY_S;
					INT16				sX, sY, sXPos, sYPos;
					INT32				iBack;

					ConvertGridNoToCenterCellXY( pItemPool->sGridNo, &sX, &sY );

					dOffsetX = (FLOAT)( sX - gsRenderCenterX );
					dOffsetY = (FLOAT)( sY - gsRenderCenterY );

					// Calculate guy's position
					FloatFromCellToScreenCoordinates( dOffsetX, dOffsetY, &dTempX_S, &dTempY_S );

					sXPos = ( ( gsVIEWPORT_END_X - gsVIEWPORT_START_X ) /2 ) + (INT16)dTempX_S;
					sYPos = ( ( gsVIEWPORT_END_Y - gsVIEWPORT_START_Y ) /2 ) + (INT16)dTempY_S - gpWorldLevelData[ pItemPool->sGridNo ].sHeight;

					// Adjust for offset position on screen
					sXPos -= gsRenderWorldOffsetX;
					sYPos -= gsRenderWorldOffsetY;
					sYPos -= pItemPool->bRenderZHeightAboveLevel;

					// Adjust for render height
					sYPos += gsRenderHeight;

					// Adjust for level height
					if ( pItemPool->ubLevel )
					{
						sYPos -= ROOF_LEVEL_HEIGHT;
					}

					// Center circle!
					sXPos -= 20;
					sYPos -= 20;

					iBack = RegisterBackgroundRect( BGND_FLAG_SINGLE, NULL, sXPos, sYPos, (INT16)(sXPos +40 ), (INT16)(sYPos + 40 ) );
					if ( iBack != -1 )
					{
						SetBackgroundRectFilled( iBack );
					}

					BltVideoObjectFromIndex(	FRAME_BUFFER, guiRADIO, pLocator->bRadioFrame, sXPos, sYPos, VO_BLT_SRCTRANSPARENCY, NULL );

					DrawItemPoolList( pItemPool, pItemPool->sGridNo	, ITEMLIST_DISPLAY, pItemPool->bRenderZHeightAboveLevel, sXPos, sYPos );

				}
			}
		}
	}

}




BOOLEAN VerifyGiveItem( SOLDIERTYPE *pSoldier, SOLDIERTYPE **ppTargetSoldier )
{
	SOLDIERTYPE *pTSoldier;
	UINT16 usSoldierIndex;
	OBJECTTYPE	*pObject;

	INT32 sGridNo;
	UINT8				ubDirection;
	UINT8				ubTargetMercID;

	// DO SOME CHECKS IF WE CAN DO ANIMATION.....

	// Get items from pending data
	pObject = pSoldier->pTempObject;

	sGridNo		= pSoldier->aiData.sPendingActionData2;
	ubDirection = pSoldier->aiData.bPendingActionData3;
	ubTargetMercID = (UINT8)pSoldier->aiData.uiPendingActionData4;

	usSoldierIndex = WhoIsThere2( sGridNo, pSoldier->pathing.bLevel );

	// See if our target is still available
	if ( usSoldierIndex != NOBODY )
	{
		// Check if it's the same merc!
		if ( usSoldierIndex != ubTargetMercID )
		{
			return( FALSE );
		}

		// Get soldier
		GetSoldier( &pTSoldier, usSoldierIndex );

		// Look for item in hand....

		(*ppTargetSoldier) = pTSoldier;

		return( TRUE );
	}
	else
	{
		if ( pSoldier->pTempObject != NULL )
		{
			AddItemToPool( pSoldier->sGridNo, pSoldier->pTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );

			// Place it on the ground!
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ ITEM_HAS_BEEN_PLACED_ON_GROUND_STR ], ShortItemNames[ pSoldier->pTempObject->usItem ] );

			// OK, disengage buddy
			pSoldier->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION );

			if ( ubTargetMercID != NOBODY )
			{
				MercPtrs[ ubTargetMercID ]->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION );
			}

			OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );

		}
	}

	return( FALSE );
}


void SoldierGiveItemFromAnimation( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE *pTSoldier;
	INT8				bInvPos;
	UINT8				ubProfile;

	INT32 sGridNo;
	UINT8				ubDirection;
	UINT8				ubTargetMercID;
	UINT16			usItemNum;
	BOOLEAN			fToTargetPlayer = FALSE;

	// Get items from pending data

	// Get objectype and delete
	pSoldier->pTempObject->MoveThisObjectTo(gTempObject);
	OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );

	bInvPos = pSoldier->bPendingActionData5;
	usItemNum = gTempObject.usItem;

	// ATE: OK, check if we have an item in the cursor from
	// this soldier and from this inv slot, if so, delete!!!!!!!
	if ( gpItemPointer != NULL )
	{
		if ( pSoldier->ubID == gpItemPointerSoldier->ubID )
		{
			if ( bInvPos == gbItemPointerSrcSlot && usItemNum == gpItemPointer->usItem	)
			{
				// Remove!!!
				EndItemPointer( );
			}
		}
	}

	sGridNo		= pSoldier->aiData.sPendingActionData2;
	ubDirection = pSoldier->aiData.bPendingActionData3;
	ubTargetMercID = (UINT8)pSoldier->aiData.uiPendingActionData4;

	// ATE: Deduct APs!
	DeductPoints( pSoldier, GetBasicAPsToPickupItem( pSoldier ), 0 ); // SANDRO

	if ( VerifyGiveItem( pSoldier, &pTSoldier ) )
	{
		// DAVE! - put stuff here to bring up shopkeeper.......

		//if the user just clicked on an arms dealer
		if( IsMercADealer( pTSoldier->ubProfile ) )
		{
			UnSetEngagedInConvFromPCAction( pSoldier );

			//if the dealer is Micky,
			/*
			if( pTSoldier->ubProfile == MICKY )
			{
			//and the items are alcohol, dont enter the shopkeeper
			if( GetArmsDealerItemTypeFromItemNumber( gTempObject.usItem ) == ARMS_DEALER_ALCOHOL )
			return;
			}
			*/

			if ( NPCHasUnusedRecordWithGivenApproach( pTSoldier->ubProfile, APPROACH_BUYSELL ) )
			{
				TriggerNPCWithGivenApproach( pTSoldier->ubProfile, APPROACH_BUYSELL, TRUE );
				return;
			}
			// now also check for buy/sell lines (Oct 13)
			/*
			else if ( NPCWillingToAcceptItem( pTSoldier->ubProfile, pSoldier->ubProfile, &gTempObject ) )
			{
			TriggerNPCWithGivenApproach( pTSoldier->ubProfile, APPROACH_GIVINGITEM, TRUE );
			return;
			}*/
			else if ( !NPCWillingToAcceptItem( pTSoldier->ubProfile, pSoldier->ubProfile, &gTempObject ) )
			{

				//Enter the shopkeeper interface
				EnterShopKeeperInterfaceScreen( pTSoldier->ubProfile );

				// removed the if, because if the player picked up an item straight from the ground or money strait from the money
				// interface, the item would NOT have a bInvPos, therefore it would not get added to the dealer, and would get deleted
				//				if ( bInvPos != NO_SLOT )
				{
					// MUST send in NO_SLOT, as the SKI wille expect it to exist in inv if not....
					AddItemToPlayersOfferAreaAfterShopKeeperOpen( &gTempObject );

					if ( bInvPos != NO_SLOT )
					{
						pSoldier->inv[ bInvPos ].initialize();
					}

					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				}

				return;
			}
		}

		// OK< FOR NOW HANDLE NPC's DIFFERENT!
		ubProfile = pTSoldier->ubProfile;

		// 1 ) PLayer to NPC = NPC
		// 2 ) Player to player = player;
		// 3 ) NPC to player = player;
		// 4 ) NPC TO NPC = NPC

		// Switch on target...
		// Are we a player dude.. ( target? )
	//	if ( ubProfile < FIRST_RPC || RPC_RECRUITED( pTSoldier ) || ubProfile >= GASTON )
		//new profiles by Jazz		
		if ( ( gProfilesAIM[ubProfile].ProfilId == ubProfile || gProfilesMERC[ubProfile].ProfilId == ubProfile || gProfilesIMP[ubProfile].ProfilId == ubProfile ) || RPC_RECRUITED( pTSoldier ) )			
		{
			fToTargetPlayer = TRUE;
		}


		if ( fToTargetPlayer )
		{
			// begin giving
			DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );

			// We are a merc, add!
			if ( !AutoPlaceObject( pTSoldier, &gTempObject, TRUE ) )
			{
				// Erase!
				if ( bInvPos != NO_SLOT )
				{
					DeleteObj( &( pSoldier->inv[ bInvPos ] ) );
					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				}

				AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );

				// We could not place it!
				// Drop it on the ground?
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ ITEM_HAS_BEEN_PLACED_ON_GROUND_STR ], ShortItemNames[ usItemNum ] );

				// OK, disengage buddy
				pSoldier->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION );
				pTSoldier->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION );
			}
			else
			{
				// Erase!
				if ( bInvPos != NO_SLOT )
				{
					DeleteObj( &( pSoldier->inv[ bInvPos ] ) );
					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				}

				// OK, it's given, display message!
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ ITEM_HAS_BEEN_GIVEN_TO_STR ], ShortItemNames[ usItemNum ], pTSoldier->name );
				if (usItemNum == MONEY)
				{
					// are we giving money to an NPC, to whom we owe money?
					if (pTSoldier->ubProfile != NO_PROFILE && gMercProfiles[pTSoldier->ubProfile].iBalance < 0)
					{
						gMercProfiles[pTSoldier->ubProfile].iBalance += gTempObject[0]->data.money.uiMoneyAmount;
						if (gMercProfiles[pTSoldier->ubProfile].iBalance >= 0)
						{
							// don't let the player accumulate credit (?)
							gMercProfiles[pTSoldier->ubProfile].iBalance = 0;

							// report the payment and set facts to indicate people not being owed money
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ GUY_HAS_BEEN_PAID_IN_FULL_STR ], pTSoldier->name );
						}
						else
						{
							// report the payment
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ GUY_STILL_OWED_STR ], pTSoldier->name, -gMercProfiles[pTSoldier->ubProfile].iBalance );
						}
					}
				}
			}
		}
		else
		{
			//CHRISL: The item we're handing over potentially came from an inventory slot but was placed into the cursor.  So why are we deleting any remaining
			//	items left in the inventory slot we started from.  When we moved the item from a stack into the cursor, we created a seperate stack and it's this
			//	new stack we should be exclusively working with.  Not the stack we took items from.
			// Erase!
//			if ( bInvPos != NO_SLOT )
//			{
//				DeleteObj(&pSoldier->inv[ bInvPos ]);
//				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
//			}

			// Now intiate conv
			InitiateConversation( pTSoldier, pSoldier, APPROACH_GIVINGITEM, (INT32)&gTempObject );
		}
	}

	// OK, disengage buddy
	pSoldier->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION );
	pTSoldier->flags.uiStatusFlags &= (~SOLDIER_ENGAGEDINACTION );

}



INT32 AdjustGridNoForItemPlacement( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	STRUCTURE		*pStructure;
	INT16				sDesiredLevel;
	INT32 sActionGridNo;
	BOOLEAN			fStructFound = FALSE;
	UINT8				ubDirection;
	INT32 sAdjustedGridNo;
	UINT8				ubTargetID;


	sActionGridNo = sGridNo;

	// Check structure database
	if( gpWorldLevelData[ sGridNo ].pStructureHead )
	{
		// Something is here, check obstruction in future
		sDesiredLevel = pSoldier->pathing.bLevel ? STRUCTURE_ON_ROOF : STRUCTURE_ON_GROUND;
		pStructure = FindStructure( sGridNo, STRUCTURE_BLOCKSMOVES );
		while( pStructure )
		{
			if( !(pStructure->fFlags & STRUCTURE_PASSABLE) && pStructure->sCubeOffset == sDesiredLevel )
			{
				// Check for openable flag....
				//if ( pStructure->fFlags & ( STRUCTURE_OPENABLE | STRUCTURE_HASITEMONTOP ) )
				{
					fStructFound = TRUE;
					break;
				}
			}
			pStructure = FindNextStructure( pStructure, STRUCTURE_BLOCKSMOVES );
		}
	}

	// ATE: IF a person is found, use adjacent gridno for it!
	ubTargetID = WhoIsThere2( sGridNo, pSoldier->pathing.bLevel );

	if ( fStructFound || ( ubTargetID != NOBODY && ubTargetID != pSoldier->ubID ) )
	{
		// GET ADJACENT GRIDNO
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, FALSE, FALSE );

		if ( sActionGridNo == -1 )
		{
			sActionGridNo = sAdjustedGridNo;
		}
	}

	return( sActionGridNo );
}


void StartBombMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo )
{
	UINT8 ubRoom;

	gpTempSoldier = pSoldier;
	gsTempGridNo = sGridNo;
	if (Item[ pSoldier->inv[HANDPOS].usItem].remotetrigger )
	{
		DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_BOMB_FREQUENCY_STR ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS, BombMessageBoxCallBack, NULL );
	}
	else if (pSoldier->inv[HANDPOS].usItem == REMOTETRIGGER)
	{
		// ATE ignore the commented-out code and add stuff to open the secret passage here
		/*
		switch( pSoldier->inv[HANDPOS].ubLocationID )
		{
		// check to make sure the appropriate sector is loaded
		}
		SetOffBombsByFrequency( pSoldier->ubID, pSoldier->inv[HANDPOS][0]->data.misc.bFrequency );
		*/

		// PLay sound....
		PlayJA2Sample( USE_STATUE_REMOTE, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );


		// Check what sector we are in....
		if ( gWorldSectorX == 3 && gWorldSectorY == MAP_ROW_O && gbWorldSectorZ == 0 )
		{
			if ( InARoom( pSoldier->sGridNo, &ubRoom ) && ubRoom == 4 )
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_OK1 );

				// Open statue
				ChangeO3SectorStatue( FALSE );

			}
			else
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
			}
		}
		else
		{
			pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
		}
	}
	else if ( IsDetonatorAttached ( &(pSoldier->inv[HANDPOS]) )	)
	{
		DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_TIMER_STR ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS, BombMessageBoxCallBack, NULL );
	}
	else if ( IsRemoteDetonatorAttached( &(pSoldier->inv[HANDPOS]) ) )
	{
		DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_REMOTE_FREQUENCY_STR ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS, BombMessageBoxCallBack, NULL );
	}
}

void BombMessageBoxCallBack( UINT8 ubExitValue )
{
	if (gpTempSoldier)
	{
		if (Item[ gpTempSoldier->inv[HANDPOS].usItem ].remotetrigger )
		{
			SetOffBombsByFrequency( gpTempSoldier->ubID, ubExitValue );
		}
		else
		{
			INT32 iResult;

			if (IsRemoteDetonatorAttached( &(gpTempSoldier->inv[HANDPOS]) ) )
			{
				iResult = SkillCheck( gpTempSoldier, PLANTING_REMOTE_BOMB_CHECK, 0 );
			}
			else
			{
				iResult = SkillCheck( gpTempSoldier, PLANTING_BOMB_CHECK, 0 );
			}

			if ( iResult >= 0 )
			{
				// EXPLOSIVES GAIN (25):	Place a bomb, or buried and armed a mine
				StatChange( gpTempSoldier, EXPLODEAMT, 25, FALSE );
			}
			else
			{
				// EXPLOSIVES GAIN (10):	Failed to place a bomb, or bury and arm a mine
				StatChange( gpTempSoldier, EXPLODEAMT, 10, FROM_FAILURE );

				// oops!	How badly did we screw up?
				if ( iResult >= -20 )
				{
					// messed up the setting
					if ( ubExitValue == 0 )
					{
						ubExitValue = 1;
					}
					else
					{
						// change up/down by 1
						ubExitValue = (UINT8) (ubExitValue + Random( 3 ) - 1);
					}
					// and continue
				}
				else
				{
					// OOPS! ... BOOM!
					IgniteExplosion( NOBODY, gpTempSoldier->sX, gpTempSoldier->sY, (INT16) (gpWorldLevelData[gpTempSoldier->sGridNo].sHeight), gpTempSoldier->sGridNo, gpTempSoldier->inv[ HANDPOS ].usItem, gpTempSoldier->pathing.bLevel );
					return;
				}
			}

			if ( ArmBomb( &(gpTempSoldier->inv[HANDPOS]), ubExitValue ) )
			{
				// SANDRO - STOMP traits - Demolitions bonus to trap level
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( gpTempSoldier, DEMOLITIONS_NT ))
				{
					// increase trap level for Demolitions trait
					gpTempSoldier->inv[ HANDPOS ][0]->data.bTrap = __min( max( 10, (8 + gSkillTraitValues.ubDEPlacedBombLevelBonus)), (( EffectiveExplosive( gpTempSoldier ) / 20) + (EffectiveExpLevel( gpTempSoldier ) / 3) + gSkillTraitValues.ubDEPlacedBombLevelBonus) );
				}
				else
				{
					gpTempSoldier->inv[ HANDPOS ][0]->data.bTrap = __min( 10, ( EffectiveExplosive( gpTempSoldier ) / 20) + (EffectiveExpLevel( gpTempSoldier ) / 3) );
				}
				
				// HACK IMMINENT!
				// value of 1 is stored in maps for SIDE of bomb owner... when we want to use IDs!
				// so we add 2 to all owner IDs passed through here and subtract 2 later
				if (gpTempSoldier->inv[HANDPOS].MoveThisObjectTo(gTempObject, 1) == 0) {
					gTempObject[0]->data.misc.ubBombOwner = gpTempSoldier->ubID + 2;
					AddItemToPool( gsTempGridNo, &gTempObject, 1, gpTempSoldier->pathing.bLevel, WORLD_ITEM_ARMED_BOMB, 0 );
				}
			}
		}
	}

}


BOOLEAN HandItemWorks( SOLDIERTYPE *pSoldier, INT8 bSlot )
{
	BOOLEAN							fItemJustBroke = FALSE, fItemWorks = TRUE;
	OBJECTTYPE *				pObj;

	pObj = &( pSoldier->inv[ bSlot ] );

	// if the item can be damaged, than we must check that it's in good enough
	// shape to be usable, and doesn't break during use.
	// Exception: land mines.	You can bury them broken, they just won't blow!
	//	if ( (Item[ pObj->usItem ].fFlags & ITEM_DAMAGEABLE) && (pObj->usItem != MINE) && (Item[ pObj->usItem ].usItemClass != IC_MEDKIT) && pObj->usItem != GAS_CAN )
	if ( (Item[ pObj->usItem ].damageable ) && (!Item[pObj->usItem].mine ) && (Item[ pObj->usItem ].usItemClass != IC_MEDKIT) && !Item[pObj->usItem].gascan )
	{
		// if it's still usable, check whether it breaks
		if ( (*pObj)[0]->data.objectStatus >= USABLE)
		{
			// if a dice roll is greater than the item's status
			if ( (Random(80) + 20) >= (UINT32) ((*pObj)[0]->data.objectStatus + 50) )
			{
				fItemJustBroke = TRUE;
				fItemWorks = FALSE;

				// item breaks, and becomes unusable...	so its status is reduced
				// to somewhere between 1 and the 1 less than USABLE
				(*pObj)[0]->data.objectStatus = (INT8) ( 1 + Random( USABLE - 1 ) );
			}
		}
		else	// it's already unusable
		{
			fItemWorks = FALSE;
		}

		if (!fItemWorks && pSoldier->bTeam == gbPlayerNum)
		{
			// merc says "This thing doesn't work!"
			TacticalCharacterDialogue( pSoldier, QUOTE_USELESS_ITEM );
			if (fItemJustBroke)
			{
				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
			}
		}
	}

	if ( fItemWorks && bSlot == HANDPOS && Item[ pObj->usItem ].usItemClass == IC_GUN )
	{
		// are we using two guns at once?
		if ( Item[ pSoldier->inv[SECONDHANDPOS].usItem ].usItemClass == IC_GUN &&
			pSoldier->inv[SECONDHANDPOS][0]->data.gun.bGunStatus >= USABLE &&
			pSoldier->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft > 0)
		{
			// check the second gun for breakage, and if IT breaks, return false
			return( HandItemWorks( pSoldier, SECONDHANDPOS ) );
		}
	}

	return( fItemWorks );
}


void SetOffBoobyTrapInMapScreen( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObject )
{
	UINT8 ubPtsDmg = 0;

	// check if trapped item is an explosive, if so then up the amount of dmg
	if( ( pObject->usItem == TNT )|| (	pObject->usItem == RDX ) )
	{
		// for explosive
		ubPtsDmg = 0;
	}
	else
	{
		// normal mini grenade dmg
		ubPtsDmg = 0;
	}

	// injure the inventory character
	pSoldier->SoldierTakeDamage( 0, ubPtsDmg, ubPtsDmg, TAKE_DAMAGE_EXPLOSION, NOBODY, NOWHERE, 0, TRUE );

	// play the sound
	PlayJA2Sample( EXPLOSION_1, RATE_11025, BTNVOLUME, 1, MIDDLEPAN );

}


void SetOffBoobyTrap( ITEM_POOL * pItemPool )
{
	if ( pItemPool )
	{
		INT16 sX, sY;
		sX = CenterX( pItemPool->sGridNo );
		sY = CenterY( pItemPool->sGridNo );
		IgniteExplosion( NOBODY, sX, sY, (INT16) (gpWorldLevelData[pItemPool->sGridNo].sHeight + pItemPool->bRenderZHeightAboveLevel), pItemPool->sGridNo, MINI_GRENADE, 0 );
		RemoveItemFromPool( pItemPool->sGridNo, pItemPool->iItemIndex, pItemPool->ubLevel );
	}

}

BOOLEAN ContinuePastBoobyTrap( SOLDIERTYPE * pSoldier, INT32 sGridNo, INT8 bLevel, INT32 iItemIndex, BOOLEAN fInStrategic, BOOLEAN *pfSaidQuote )
{
	BOOLEAN					fBoobyTrapKnowledge;
	INT8						bTrapDifficulty, bTrapDetectLevel;
	OBJECTTYPE *		pObj;

	pObj = &(gWorldItems[ iItemIndex ].object);

	(*pfSaidQuote) = FALSE;

	if ((*pObj)[0]->data.bTrap > 0)
	{
		if (pSoldier->bTeam == gbPlayerNum)
		{
			// does the player know about this item?
			fBoobyTrapKnowledge = (((*pObj).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED) > 0);

			// blue flag stuff?

			if (!fBoobyTrapKnowledge)
			{
				bTrapDifficulty = (*pObj)[0]->data.bTrap;
				bTrapDetectLevel = CalcTrapDetectLevel( pSoldier, FALSE );
				if (bTrapDetectLevel >= bTrapDifficulty)
				{
					// spotted the trap!
					(*pObj).fFlags |= OBJECT_KNOWN_TO_BE_TRAPPED;
					fBoobyTrapKnowledge = TRUE;

					// Make him warn us:

					// Set things up..
					gpBoobyTrapSoldier = pSoldier;
					gpBoobyTrapItemPool = GetItemPoolForIndex( sGridNo, iItemIndex, pSoldier->pathing.bLevel );
					gsBoobyTrapGridNo = sGridNo;
					gbBoobyTrapLevel	= pSoldier->pathing.bLevel;
					gfDisarmingBuriedBomb = FALSE;
					gbTrapDifficulty = bTrapDifficulty;

					// And make the call for the dialogue
					SetStopTimeQuoteCallback( BoobyTrapDialogueCallBack );
					TacticalCharacterDialogue( pSoldier, QUOTE_BOOBYTRAP_ITEM );

					(*pfSaidQuote) = TRUE;

					return( FALSE );
				}
			}

			gpBoobyTrapItemPool = GetItemPoolForIndex( sGridNo, iItemIndex, pSoldier->pathing.bLevel );
			if (fBoobyTrapKnowledge)
			{
				// have the computer ask us if we want to proceed
				gpBoobyTrapSoldier = pSoldier;
				gsBoobyTrapGridNo = sGridNo;
				gbBoobyTrapLevel	= pSoldier->pathing.bLevel;
				gfDisarmingBuriedBomb = FALSE;
				gbTrapDifficulty = (*pObj)[0]->data.bTrap;

				if( fInStrategic )
				{
					DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ DISARM_BOOBYTRAP_PROMPT ], MAP_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapInMapScreenMessageBoxCallBack, NULL );
				}
				else
				{
					DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ DISARM_BOOBYTRAP_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack, NULL );
				}
			}
			else
			{
				// oops!
				SetOffBoobyTrap( gpBoobyTrapItemPool );
			}

			return( FALSE );

		}
		// else, enemies etc always know about boobytraps and are not affected by them
	}

	return( TRUE );
}

void BoobyTrapDialogueCallBack( void )
{
	gfJustFoundBoobyTrap = TRUE;

	// now prompt the user...
	if( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
	{
		DoScreenIndependantMessageBox( TacticalStr[ DISARM_BOOBYTRAP_PROMPT ],	( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapInMapScreenMessageBoxCallBack );
	}
	else
	{
		DoScreenIndependantMessageBox( TacticalStr[ DISARM_BOOBYTRAP_PROMPT ],	( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack );
	}
}

void BoobyTrapMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( gfJustFoundBoobyTrap )
	{
		// NOW award for finding boobytrap
		// WISDOM GAIN:	Detected a booby-trap
		StatChange( gpBoobyTrapSoldier, WISDOMAMT, (UINT16) (3 * gbTrapDifficulty), FALSE );
		// EXPLOSIVES GAIN:	Detected a booby-trap
		StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (UINT16) (3 * gbTrapDifficulty), FALSE );
		gfJustFoundBoobyTrap = FALSE;
	}

	if (ubExitValue == MSG_BOX_RETURN_YES)
	{
		INT32						iCheckResult;
		// get the item
		gTempObject = gWorldItems[ gpBoobyTrapItemPool->iItemIndex ].object;

		// Snap: make it easier to disarm our own traps.
		// If we succede - we get exp, but if we fail - we pay fair and square!

		//CHRISL: first things first.  If we're in combat, we need to spend some APs to disarm the device
		if((gTacticalStatus.uiFlags & INCOMBAT) || (gTacticalStatus.fEnemyInSector))
		{
			// SANDRO was here, AP_DISARM_MINE changed to GetAPsToDisarmMine
			if(EnoughPoints(gpBoobyTrapSoldier, GetAPsToDisarmMine( gpBoobyTrapSoldier ), APBPConstants[BP_DISARM_MINE], TRUE))
				DeductPoints(gpBoobyTrapSoldier, GetAPsToDisarmMine( gpBoobyTrapSoldier ), APBPConstants[BP_DISARM_MINE], AFTERACTION_INTERRUPT);
			else
				return;
		}

		// NB owner grossness... bombs 'owned' by the enemy are stored with side value 1 in
		// the map. So if we want to detect a bomb placed by the player, owner is > 1, and
		// owner - 2 gives the ID of the character who planted it
		if ( gTempObject[0]->data.misc.ubBombOwner > 1 && ( (INT32)gTempObject[0]->data.misc.ubBombOwner - 2 >= gTacticalStatus.Team[ OUR_TEAM ].bFirstID && gTempObject[0]->data.misc.ubBombOwner - 2 <= gTacticalStatus.Team[ OUR_TEAM ].bLastID ) )
		{
			if ( gTempObject[0]->data.misc.ubBombOwner - 2 == gpBoobyTrapSoldier->ubID )
			{
				// my own boobytrap!
				iCheckResult = SkillCheck( gpBoobyTrapSoldier, DISARM_TRAP_CHECK, 40 );
			}
			else
			{
				// our team's boobytrap!
				iCheckResult = SkillCheck( gpBoobyTrapSoldier, DISARM_TRAP_CHECK, 20 );
			}
		}
		else
		{
			iCheckResult = SkillCheck( gpBoobyTrapSoldier, DISARM_TRAP_CHECK, 0 );
		}

		if (iCheckResult >= 0)
		{

			if ( gTempObject[0]->data.misc.ubBombOwner > 1 && ( (INT32)gTempObject[0]->data.misc.ubBombOwner - 2 >= gTacticalStatus.Team[ OUR_TEAM ].bFirstID && gTempObject[0]->data.misc.ubBombOwner - 2 <= gTacticalStatus.Team[ OUR_TEAM ].bLastID ) )
			{
				if ( gTempObject[0]->data.misc.ubBombOwner - 2 == gpBoobyTrapSoldier->ubID )
				{
					// disarmed my own boobytrap!
					StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (UINT16) (2 * gbTrapDifficulty), FALSE );
				}
				else
				{
					// disarmed our team's boobytrap!
					StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (UINT16) (4 * gbTrapDifficulty), FALSE );
					
					// SANDRO - merc records - trap removal count (don't count our own traps)
					gMercProfiles[ gpBoobyTrapSoldier->ubProfile ].records.usTrapsRemoved++;
				}
			}
			else
			{
				// disarmed a boobytrap!
				StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (UINT16) (6 * gbTrapDifficulty), FALSE );

				// SANDRO - merc records - trap removal count
				gMercProfiles[ gpBoobyTrapSoldier->ubProfile ].records.usTrapsRemoved++;
			}

			// have merc say this is good
			gpBoobyTrapSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );

			if (gfDisarmingBuriedBomb)
			{
				if (gTempObject.usItem == SWITCH)
				{
					// give the player a remote trigger instead
					CreateItem( REMOTEBOMBTRIGGER, (INT8) (1 + Random( 9 )), &gTempObject );
				}
				else if (gTempObject.usItem == ACTION_ITEM && gTempObject[0]->data.misc.bActionValue != ACTION_ITEM_BLOW_UP )
				{
					// give the player a detonator instead
					CreateItem( DETONATOR, (INT8) (1 + Random( 9 )), &gTempObject );
				}
				else
				{
					// switch action item to the real item type
					CreateItem( gTempObject[0]->data.misc.usBombItem, gTempObject[0]->data.misc.bBombStatus, &gTempObject );

					if (is_networked && is_client)
					{
						OBJECTTYPE TempAttachment;
						// attach whatever trigger the bomb originally had
						if (gWorldItems[ gpBoobyTrapItemPool->iItemIndex ].object[0]->data.misc.bDetonatorType == BOMB_REMOTE)
						{
							// attack a remote deonator, but they will need a trigger to use it :)
							CreateItem( REMDETONATOR, gTempObject[0]->data.misc.bBombStatus, &TempAttachment );
						}
						else 
						{
							// always just give a timed detonator so they can use it
							CreateItem( DETONATOR, gTempObject[0]->data.misc.bBombStatus, &TempAttachment );
						}
						
						if (ValidAttachment( TempAttachment.usItem , &gTempObject ))
						{
							gTempObject.AttachObject(NULL, &TempAttachment, FALSE);
						}
					}
				}

				// remove any blue flag graphic
				RemoveBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
			}
			else
			{
				gTempObject[0]->data.bTrap = 0;
				gTempObject.fFlags &= ~( OBJECT_KNOWN_TO_BE_TRAPPED );
			}

			// place it in the guy's inventory/cursor
			if ( AutoPlaceObject( gpBoobyTrapSoldier, &gTempObject, TRUE ) )
			{
				// OJW - 20091029 - disarm explosives
				if (is_networked && is_client)
						send_disarm_explosive( gsBoobyTrapGridNo , gpBoobyTrapItemPool->iItemIndex , gpBoobyTrapSoldier->ubID );
				// remove it from the ground
				RemoveItemFromPool( gsBoobyTrapGridNo, gpBoobyTrapItemPool->iItemIndex, gbBoobyTrapLevel );
			}
			else
			{
				//CHRISL: because the above AutoPlaceObject may fail due to an item swap, we need a few more checks
				//	before we assume it's an actual failure.  First, compare our tempObject with the pool item and
				//	see if they're the same object.  If the above AutoPlaceObject is a complete fail, they should be
				//	the same.
				if(gWorldItems[gpBoobyTrapItemPool->iItemIndex].object.operator == (gTempObject))
				{
					// make sure the item in the world is untrapped
					gWorldItems[ gpBoobyTrapItemPool->iItemIndex ].object[0]->data.bTrap = 0;
					gWorldItems[ gpBoobyTrapItemPool->iItemIndex ].object.fFlags &= ~( OBJECT_KNOWN_TO_BE_TRAPPED );

					// ATE; If we failed to add to inventory, put failed one in our cursor...
					gfDontChargeAPsToPickup = TRUE;
					HandleAutoPlaceFail( gpBoobyTrapSoldier, gpBoobyTrapItemPool->iItemIndex, gsBoobyTrapGridNo );
			
					// OJW - 20091029 - disarm explosives
					if (is_networked && is_client)
						send_disarm_explosive( gsBoobyTrapGridNo , gpBoobyTrapItemPool->iItemIndex , gpBoobyTrapSoldier->ubID );

					RemoveItemFromPool( gsBoobyTrapGridNo, gpBoobyTrapItemPool->iItemIndex, gbBoobyTrapLevel );
				}
				else
				{
					//We've done an item swap which caused the above AutoPlaceObject failure.  Try to place autoplace
					//	the object again.  If we fail, move the temp object to the cursor.
					if(!(AutoPlaceObject(gpBoobyTrapSoldier, &gTempObject, TRUE)))
					{
						gpItemPointer = new OBJECTTYPE;
						gTempObject.MoveThisObjectTo(*gpItemPointer,-1,gpBoobyTrapSoldier,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT);
						//CHRISL: Also make sure we list the ItemPointerSoldier as our BoobyTrapSoldier
						gpItemPointerSoldier = gpBoobyTrapSoldier;
					}
					// OJW - 20091029 - disarm explosives
					if (is_networked && is_client)
						send_disarm_explosive( gsBoobyTrapGridNo , gpBoobyTrapItemPool->iItemIndex , gpBoobyTrapSoldier->ubID );

					// remove it from the ground
					RemoveItemFromPool( gsBoobyTrapGridNo, gpBoobyTrapItemPool->iItemIndex, gbBoobyTrapLevel );
				}
			}
		}
		else
		{
			// oops! trap goes off
			StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (INT8) (3 * gbTrapDifficulty ), FROM_FAILURE );

			gpBoobyTrapSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );

			if (gfDisarmingBuriedBomb)
			{
				SetOffBombsInGridNo( gpBoobyTrapSoldier->ubID, gsBoobyTrapGridNo, TRUE, gbBoobyTrapLevel );
			}
			else
			{
				SetOffBoobyTrap( gpBoobyTrapItemPool );
			}
		}
	}
	else
	{
		if (gfDisarmingBuriedBomb)
		{
			DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ REMOVE_BLUE_FLAG_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, RemoveBlueFlagDialogueCallBack, NULL );
		}
		// otherwise do nothing
	}
}

void BoobyTrapInMapScreenMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( gfJustFoundBoobyTrap )
	{
		// NOW award for finding boobytrap

		// WISDOM GAIN:	Detected a booby-trap
		StatChange( gpBoobyTrapSoldier, WISDOMAMT, (UINT16) (3 * gbTrapDifficulty), FALSE );
		// EXPLOSIVES GAIN:	Detected a booby-trap
		StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (UINT16) (3 * gbTrapDifficulty), FALSE );
		gfJustFoundBoobyTrap = FALSE;
	}

	if (ubExitValue == MSG_BOX_RETURN_YES)
	{
		INT32						iCheckResult;
		iCheckResult = SkillCheck( gpBoobyTrapSoldier, DISARM_TRAP_CHECK, 0 );

		if (iCheckResult >= 0)
		{
			// disarmed a boobytrap!
			StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (UINT16) (6 * gbTrapDifficulty), FALSE );

			// SANDRO - merc records - trap removal count
			if ( gpBoobyTrapSoldier->ubProfile != NO_PROFILE )
				gMercProfiles[ gpBoobyTrapSoldier->ubProfile ].records.usTrapsRemoved++;


			// have merc say this is good
			gpBoobyTrapSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );

			// get the item
			gTempObject = *gpItemPointer;

			if (gfDisarmingBuriedBomb)
			{
				if (gTempObject.usItem == SWITCH)
				{
					// give the player a remote trigger instead
					CreateItem( REMOTEBOMBTRIGGER, (INT8) (1 + Random( 9 )), &gTempObject );
				}
				else if (gTempObject.usItem == ACTION_ITEM && gTempObject[0]->data.misc.bActionValue != ACTION_ITEM_BLOW_UP )
				{
					// give the player a detonator instead
					CreateItem( DETONATOR, (INT8) (1 + Random( 9 )), &gTempObject );
				}
				else
				{
					// switch action item to the real item type
					CreateItem( gTempObject[0]->data.misc.usBombItem, gTempObject[0]->data.misc.bBombStatus, &gTempObject );
				}
			}
			else
			{
				gTempObject[0]->data.bTrap = 0;
				gTempObject.fFlags &= ~( OBJECT_KNOWN_TO_BE_TRAPPED );
			}

			MAPEndItemPointer( );

			// place it in the guy's inventory/cursor
			if ( !AutoPlaceObject( gpBoobyTrapSoldier, &gTempObject, TRUE ) )
			{
				AutoPlaceObjectInInventoryStash( &gTempObject );
			}

			HandleButtonStatesWhileMapInventoryActive( );
		}
		else
		{
			// oops! trap goes off
			StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (INT8) (3 * gbTrapDifficulty ), FROM_FAILURE );

			gpBoobyTrapSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );

			if (gfDisarmingBuriedBomb)
			{
				SetOffBombsInGridNo( gpBoobyTrapSoldier->ubID, gsBoobyTrapGridNo, TRUE, gbBoobyTrapLevel );
			}
			else
			{
				SetOffBoobyTrap( gpBoobyTrapItemPool );
			}
		}
	}
	else
	{
		if (gfDisarmingBuriedBomb)
		{
			DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ REMOVE_BLUE_FLAG_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, RemoveBlueFlagDialogueCallBack, NULL );
		}
		// otherwise do nothing
	}
}



void SwitchMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		// Message that switch is activated...
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 60 ] );

		SetOffBombsByFrequency( gpTempSoldier->ubID,	bTempFrequency );
	}
}

BOOLEAN NearbyGroundSeemsWrong( SOLDIERTYPE * pSoldier, INT32 sGridNo, BOOLEAN fCheckAroundGridNo, INT32 * psProblemGridNo )
{
	INT32						sNextGridNo;
	// BOOLEAN fWorthChecking = FALSE, fProblemExists = FALSE, fDetectedProblem = FALSE;
	UINT8						ubDetectLevel, ubDirection;
	MAP_ELEMENT *		pMapElement;
	UINT32					fCheckFlag;
	UINT32					uiWorldBombIndex;
	OBJECTTYPE *		pObj;
	BOOLEAN					fMining, fFoundMetal = FALSE;
	//	ITEM_POOL *			pItemPool;
	UINT8						ubMovementCost;

	ubDetectLevel = 0;

	if (	FindMetalDetector( pSoldier ) != NO_SLOT )
	{
		fMining = TRUE;
	}
	else
	{
		fMining = FALSE;

		ubDetectLevel = CalcTrapDetectLevel( pSoldier, FALSE );
		/*
		if (pSoldier->bStealthMode)
		{
		ubDetectLevel++;
		}
		switch (pSoldier->usAnimState)
		{
		case CRAWLING:
		ubDetectLevel += 2;
		break;

		case SWATTING:
		ubDetectLevel++;
		break;

		default:
		break;
		}
		*/
	}

	if (pSoldier->bSide == 0)
	{
		fCheckFlag = MAPELEMENT_PLAYER_MINE_PRESENT;
	}
	else
	{
		fCheckFlag = MAPELEMENT_ENEMY_MINE_PRESENT;
	}

	// check every tile around gridno for the presence of "nasty stuff"
	for (ubDirection = 0; ubDirection < 8; ubDirection++)
	{
		if ( fCheckAroundGridNo )
		{
			// get the gridno of the next spot adjacent to lastGridno in that direction
			sNextGridNo = NewGridNo( sGridNo, (INT16) DirectionInc( (UINT8) ubDirection ) );

			// don't check directions that are impassable!
			ubMovementCost = gubWorldMovementCosts[ sNextGridNo ][ ubDirection ][ pSoldier->pathing.bLevel ];
			if ( IS_TRAVELCOST_DOOR( ubMovementCost ) )
			{
				ubMovementCost = DoorTravelCost( NULL, sNextGridNo, ubMovementCost, FALSE, NULL );
			}
			if ( ubMovementCost >= TRAVELCOST_BLOCKED)
			{
				continue;
			}
		}
		else
		{
			// we should just be checking the gridno
			sNextGridNo = sGridNo;
			ubDirection = 8; // don't loop
		}

		// if this sNextGridNo isn't out of bounds... but it never can be
		pMapElement = &(gpWorldLevelData[sNextGridNo]);

		if (pMapElement->uiFlags & fCheckFlag)
		{
			// already know there's a mine there
			continue;
		}

		// check for boobytraps
		for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
		{
			if (gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == sNextGridNo)
			{
				pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
				if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_PRESSURE && !((*pObj).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED) && (!((*pObj).fFlags & OBJECT_DISABLED_BOMB)) )
				{
					if ( fMining && (*pObj)[0]->data.bTrap <= 10 )
					{
						// add blue flag
						AddBlueFlag( sNextGridNo, pSoldier->pathing.bLevel );
						fFoundMetal = TRUE;
						break;
					}
					else if (ubDetectLevel >= (*pObj)[0]->data.bTrap)
					{
						if (pSoldier->flags.uiStatusFlags & SOLDIER_PC )
						{
							// detected exposives buried nearby...
							StatChange( pSoldier, EXPLODEAMT, (UINT16) ((*pObj)[0]->data.bTrap), FALSE );
							StatChange( pSoldier, WISDOMAMT, (UINT16) ((*pObj)[0]->data.bTrap), FALSE );

							// set item as known
							(*pObj).fFlags |= OBJECT_KNOWN_TO_BE_TRAPPED;
						}

						*psProblemGridNo = sNextGridNo;
						return( TRUE );
					}
				}
			}
		}

		/*
		// also check for metal items if using a metal detector
		if (fMining)
		{
		// add blue flags where we find metallic objects hidden
		GetItemPool( sNextGridNo, &pItemPool, pSoldier->pathing.bLevel );
		while( pItemPool )
		{
		if ( pItemPool->bVisible == BURIED || (pItemPool->bVisible != TRUE && gWorldItems[ pItemPool->iItemIndex ].object.bTrap > 0 ) )
		{
		pObj = &( gWorldItems[ pItemPool->iItemIndex ].object );
		if ( pObj->usItem == ACTION_ITEM && pObj->)
		{
		switch( (*pObj)[0]->data.misc.bActionValue )
		{
		case ACTION_ITEM_BLOW_UP:
		case ACTION_ITEM_LOCAL_ALARM:
		case ACTION_ITEM_GLOBAL_ALARM:
		// add blue flag
		AddBlueFlag( sNextGridNo, pSoldier->pathing.bLevel );
		fFoundMetal = TRUE;
		break;
		default:
		break;

		}
		}
		else if (Item[ pObj->usItem ].fFlags & ITEM_METAL)
		{
		// add blue flag
		AddBlueFlag( sNextGridNo, pSoldier->pathing.bLevel );
		fFoundMetal = TRUE;
		break;
		}
		}
		pItemPool = pItemPool->pNext;
		}
		}
		*/

	}

	*psProblemGridNo = NOWHERE;
	if (fFoundMetal)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

void MineSpottedDialogueCallBack( void )
{
	ITEM_POOL * pItemPool;

	// ATE: REALLY IMPORTANT - ALL CALLBACK ITEMS SHOULD UNLOCK
	gTacticalStatus.fLockItemLocators = FALSE;

	GetItemPool( gsBoobyTrapGridNo, &pItemPool, gbBoobyTrapLevel );

	// WDS - Automatically flag mines
	if (gGameExternalOptions.automaticallyFlagMines) {
		// play a locator at the location of the mine
		SetItemPoolLocator( pItemPool );

		AddBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
	} else {
		guiPendingOverrideEvent = LU_BEGINUILOCK;

		// play a locator at the location of the mine
		SetItemPoolLocatorWithCallback( pItemPool, MineSpottedLocatorCallback );
	}
}

void MineSpottedLocatorCallback( void )
{
	guiPendingOverrideEvent = LU_ENDUILOCK;

	// now ask the player if he wants to place a blue flag.
	DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ PLACE_BLUE_FLAG_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, MineSpottedMessageBoxCallBack, NULL );
}

void MineSpottedMessageBoxCallBack( UINT8 ubExitValue )
{
	if (ubExitValue == MSG_BOX_RETURN_YES)
	{
		// place a blue flag where the mine was found
		AddBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
	}
}

void RemoveBlueFlagDialogueCallBack( UINT8 ubExitValue )
{
	if (ubExitValue == MSG_BOX_RETURN_YES)
	{
		RemoveBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
	}
}

void AddBlueFlag( INT32 sGridNo ,INT8 bLevel )
{
	LEVELNODE *pNode;

	ApplyMapChangesToMapTempFile( TRUE );
	gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;

	pNode = AddStructToTail( sGridNo, BLUEFLAG_GRAPHIC );

	if ( pNode )
	{
		pNode->uiFlags |= LEVELNODE_SHOW_THROUGH;
	}

	ApplyMapChangesToMapTempFile( FALSE );
	RecompileLocalMovementCostsFromRadius( sGridNo, bLevel );
	SetRenderFlags(RENDER_FLAG_FULL);
}

void RemoveBlueFlag( INT32 sGridNo, INT8 bLevel )
{
	ApplyMapChangesToMapTempFile( TRUE );
	gpWorldLevelData[sGridNo].uiFlags &= ~(MAPELEMENT_PLAYER_MINE_PRESENT);

	if ( bLevel == 0 )
	{
		RemoveStruct( sGridNo, BLUEFLAG_GRAPHIC );
	}
	else
	{
		RemoveOnRoof( sGridNo, BLUEFLAG_GRAPHIC );
	}

	ApplyMapChangesToMapTempFile( FALSE );
	RecompileLocalMovementCostsFromRadius( sGridNo, bLevel );
	SetRenderFlags(RENDER_FLAG_FULL);
}

void MakeNPCGrumpyForMinorOffense( SOLDIERTYPE * pSoldier, SOLDIERTYPE *pOffendingSoldier )
{
	CancelAIAction( pSoldier, TRUE );

	switch( pSoldier->ubProfile )
	{
	case FREDO:
	case FRANZ:
	case HERVE:
	case PETER:
	case ALBERTO:
	case CARLO:
	case MANNY:
	case GABBY:
	case ARNIE:
	case HOWARD:
	case SAM:
	case FATHER:
	case TINA:
	case ARMAND:
	case WALTER:
		gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags3 |= PROFILE_MISC_FLAG3_NPC_PISSED_OFF;
		TriggerNPCWithIHateYouQuote( pSoldier->ubProfile );
		break;
	default:
		// trigger NPCs with quote if available
		AddToShouldBecomeHostileOrSayQuoteList( pSoldier->ubID );
		break;
	}

	if ( pOffendingSoldier )
	{
		pSoldier->aiData.bNextAction = AI_ACTION_CHANGE_FACING;
		pSoldier->aiData.usNextActionData = atan8( pSoldier->sX, pSoldier->sY, pOffendingSoldier->sX, pOffendingSoldier->sY );
	}
}


void TestPotentialOwner( SOLDIERTYPE * pSoldier )
{
	if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE )
	{
		if ( SoldierToSoldierLineOfSightTest( pSoldier, gpTempSoldier, TRUE, CALC_FROM_ALL_DIRS ) )
		{
			MakeNPCGrumpyForMinorOffense( pSoldier, gpTempSoldier );
		}
	}
}

void CheckForPickedOwnership( void )
{
	ITEM_POOL *				pItemPool;
	UINT8							ubProfile;
	UINT8							ubCivGroup;
	SOLDIERTYPE *			pSoldier;
	UINT8							ubLoop;

	// LOOP THROUGH LIST TO FIND NODE WE WANT
	GetItemPool( gsTempGridNo, &pItemPool, gpTempSoldier->pathing.bLevel );

	while( pItemPool )
	{
		if ( gWorldItems[ pItemPool->iItemIndex ].object.usItem == OWNERSHIP )
		{
			if ( gWorldItems[ pItemPool->iItemIndex ].object[0]->data.owner.ubOwnerProfile != NO_PROFILE )
			{
				ubProfile = (UINT8) gWorldItems[ pItemPool->iItemIndex ].object[0]->data.owner.ubOwnerProfile;
				pSoldier = FindSoldierByProfileID( ubProfile, FALSE );
				if ( pSoldier )
				{
					TestPotentialOwner( pSoldier );
				}
			}
			if ( gWorldItems[ pItemPool->iItemIndex ].object[0]->data.owner.ubOwnerCivGroup != NON_CIV_GROUP )
			{
				ubCivGroup = gWorldItems[ pItemPool->iItemIndex ].object[0]->data.owner.ubOwnerCivGroup;
				if ( ubCivGroup == HICKS_CIV_GROUP && CheckFact( FACT_HICKS_MARRIED_PLAYER_MERC, 0 ) )
				{
					// skip because hicks appeased
					pItemPool = pItemPool->pNext;
					continue;
				}
				for ( ubLoop = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; ubLoop <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ubLoop++ )
				{
					pSoldier = MercPtrs[ ubLoop ];
					if ( pSoldier && pSoldier->ubCivilianGroup == ubCivGroup )
					{
						TestPotentialOwner( pSoldier );
					}
				}
			}
		}
		pItemPool = pItemPool->pNext;
	}

}



void LoopLevelNodeForItemGlowFlag( LEVELNODE *pNode, INT32 sGridNo, UINT8 ubLevel, BOOLEAN fOn )
{
	while ( pNode != NULL )
	{
		if ( pNode->uiFlags & LEVELNODE_ITEM )
		{
			if ( fOn )
			{
				pNode->uiFlags |= LEVELNODE_DYNAMIC;
			}
			else
			{
				pNode->uiFlags &= (~LEVELNODE_DYNAMIC);
			}
		}
		pNode = pNode->pNext;
	}
}


void HandleItemGlowFlag( INT32 sGridNo, UINT8 ubLevel, BOOLEAN fOn )
{
	LEVELNODE *pNode;

	if ( ubLevel == 0 )
	{
		pNode = gpWorldLevelData[ sGridNo ].pStructHead;
		LoopLevelNodeForItemGlowFlag( pNode, sGridNo,	ubLevel, fOn );
	}
	else
	{
		pNode = gpWorldLevelData[ sGridNo ].pOnRoofHead;
		LoopLevelNodeForItemGlowFlag( pNode, sGridNo, ubLevel, fOn );
	}
}

void ToggleItemGlow( BOOLEAN fOn )
{
	INT32 cnt;

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		HandleItemGlowFlag( cnt, 0, fOn );
		HandleItemGlowFlag( cnt, 1, fOn );
	}

	if ( !fOn )
	{
		gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] = FALSE;
	}
	else
	{
		gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] = TRUE;
	}

	SetRenderFlags(RENDER_FLAG_FULL);
}

BOOLEAN ContinuePastBoobyTrapInMapScreen( OBJECTTYPE *pObject, SOLDIERTYPE *pSoldier )
{
	BOOLEAN					fBoobyTrapKnowledge;
	INT8						bTrapDifficulty, bTrapDetectLevel;

	if ((*pObject)[0]->data.bTrap > 0)
	{
		if (pSoldier->bTeam == gbPlayerNum)
		{
			// does the player know about this item?
			fBoobyTrapKnowledge = (((*pObject).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED) > 0);

			// blue flag stuff?

			if (!fBoobyTrapKnowledge)
			{
				bTrapDifficulty = (*pObject)[0]->data.bTrap;
				bTrapDetectLevel = CalcTrapDetectLevel( pSoldier, FALSE );
				if (bTrapDetectLevel >= bTrapDifficulty)
				{
					// spotted the trap!
					(*pObject).fFlags |= OBJECT_KNOWN_TO_BE_TRAPPED;
					fBoobyTrapKnowledge = TRUE;

					// Make him warn us:
					gpBoobyTrapSoldier = pSoldier;

					// And make the call for the dialogue
					SetStopTimeQuoteCallback( BoobyTrapDialogueCallBack );
					TacticalCharacterDialogue( pSoldier, QUOTE_BOOBYTRAP_ITEM );

					return( FALSE );
				}
			}

			if (fBoobyTrapKnowledge)
			{
				// have the computer ask us if we want to proceed
				gpBoobyTrapSoldier = pSoldier;
				gbTrapDifficulty = (*pObject)[0]->data.bTrap;
				DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ DISARM_BOOBYTRAP_PROMPT ], MAP_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapInMapScreenMessageBoxCallBack, NULL );
			}
			else
			{
				// oops!
				SetOffBoobyTrapInMapScreen( pSoldier, pObject );
			}

			return( FALSE );

		}
		// else, enemies etc always know about boobytraps and are not affected by them
	}

	return( TRUE );
}

// Well, clears all item pools
void ClearAllItemPools( )
{
	INT32 cnt;

	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		RemoveItemPool( cnt, 0 );
		RemoveItemPool( cnt, 1 );
	}
}


// Refresh item pools
void RefreshItemPools( WORLDITEM * pItemList, INT32 iNumberOfItems )
{
	ClearAllItemPools( );

	RefreshWorldItemsIntoItemPools(	pItemList, iNumberOfItems );
}


INT32 FindNearestAvailableGridNoForItem( INT32 sSweetGridNo, INT8 ubRadius )
{
	INT32 sTop, sBottom;
	INT32 sLeft, sRight;
	INT32 cnt1, cnt2, cnt3;
	INT32 sGridNo;
	INT32		uiRange, uiLowestRange = 999999;
	INT32 sLowestGridNo=0;
	INT32					leftmost;
	BOOLEAN	fFound = FALSE;
	SOLDIERTYPE soldier;
	INT16 ubSaveNPCAPBudget;
	UINT8 ubSaveNPCDistLimit;

	cnt3 = 0;

	//Save AI pathing vars.	changing the distlimit restricts how
	//far away the pathing will consider.
	ubSaveNPCAPBudget = gubNPCAPBudget;
	ubSaveNPCDistLimit = gubNPCDistLimit;
	gubNPCAPBudget = 0;
	gubNPCDistLimit = ubRadius;

	//create dummy soldier, and use the pathing to determine which nearby slots are
	//reachable.
	soldier.bTeam = 1;
	soldier.sGridNo = sSweetGridNo;

	sTop		= ubRadius;
	sBottom = -ubRadius;
	sLeft	= - ubRadius;
	sRight	= ubRadius;

	//clear the mapelements of potential residue MAPELEMENT_REACHABLE flags
	//in the square region.
	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + (WORLD_COLS * cnt1) + cnt2;
			if ( sGridNo >=0 && sGridNo < WORLD_MAX )
			{
				gpWorldLevelData[ sGridNo ].uiFlags &= (~MAPELEMENT_REACHABLE);
			}
		}
	}

	//Now, find out which of these gridnos are reachable
	//(use the fake soldier and the pathing settings)
	FindBestPath( &soldier, GRIDSIZE, 0, WALKING, COPYREACHABLE, 0 );//dnl ch50 071009

	uiLowestRange = 999999;

	for( cnt1 = sBottom; cnt1 <= sTop; cnt1++ )
	{
		leftmost = ( ( sSweetGridNo + ( WORLD_COLS * cnt1 ) )/ WORLD_COLS ) * WORLD_COLS;

		for( cnt2 = sLeft; cnt2 <= sRight; cnt2++ )
		{
			sGridNo = sSweetGridNo + ( WORLD_COLS * cnt1 ) + cnt2;
			if ( sGridNo >=0 && sGridNo < WORLD_MAX && sGridNo >= leftmost && sGridNo < ( leftmost + WORLD_COLS ) &&
				gpWorldLevelData[ sGridNo ].uiFlags & MAPELEMENT_REACHABLE )
			{
				// Go on sweet stop
				if ( NewOKDestination( &soldier, sGridNo, TRUE, soldier.pathing.bLevel ) )
				{
					uiRange = GetRangeInCellCoordsFromGridNoDiff( sSweetGridNo, sGridNo );

					if ( uiRange < uiLowestRange )
					{
						sLowestGridNo = sGridNo;
						uiLowestRange = uiRange;
						fFound = TRUE;
					}
				}
			}
		}
	}
	gubNPCAPBudget = ubSaveNPCAPBudget;
	gubNPCDistLimit = ubSaveNPCDistLimit;
	if ( fFound )
	{
		return sLowestGridNo;
	}
	return NOWHERE;
}


BOOLEAN CanPlayerUseRocketRifle( SOLDIERTYPE *pSoldier, BOOLEAN fDisplay )
{
	if ( Item[pSoldier->inv[ pSoldier->ubAttackingHand ].usItem].fingerprintid )
	{
		// check imprint ID
		// NB not-imprinted value is NO_PROFILE
		// imprinted value is profile for mercs & NPCs and NO_PROFILE + 1 for generic dudes
		if (pSoldier->ubProfile != NO_PROFILE)
		{
			if ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID != pSoldier->ubProfile )
			{
				// NOT a virgin gun...
				if ( pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.ubImprintID != NO_PROFILE )
				{
					// access denied!
					if (pSoldier->bTeam == gbPlayerNum)
					{
						PlayJA2Sample( RG_ID_INVALID, RATE_11025, HIGHVOLUME, 1, MIDDLE );

						if ( fDisplay )
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, L"\"%s\"", TacticalStr[ GUN_NOGOOD_FINGERPRINT ] );
						}
					}
					return( FALSE );
				}
			}
		}
	}
	return( TRUE );
}

/**
* jackaians: pretty bad function. Create an artificial ItemPool, where
* iItemIndex is not the index in the world, but is used to store the index
* of the opponent's slots.
* if only one item, return in ubIndexRet the item's index
*/
UINT8 StealItems(SOLDIERTYPE* pSoldier,SOLDIERTYPE* pOpponent, UINT8* ubIndexRet)
{
	UINT8		ubCount=0;
	ITEM_POOL	*pItemPool,*pTempItemPool,*pTempLastItemPool = 0;
	OBJECTTYPE	*pObject;
	UINT8		i;
	BOOLEAN		fStealItem = FALSE;

	//Create a temporary item pool, with index in Opponent's inventory as index
	pItemPool=NULL;
	for(i=0 ; i<pOpponent->inv.size(); i++)
	{
		fStealItem = FALSE;

		pObject=&pOpponent->inv[i];
		if ((pObject->exists() == true) && !(Item[pObject->usItem].defaultundroppable  )) // CHECK! Undroppable items cannot be stolen - SANDRO
		{
			// Is the enemy collapsed
			if ( pOpponent->stats.bLife < OKLIFE || pOpponent->bCollapsed )
			{
				// We can steal any of his items in the inventory
				fStealItem = TRUE;
			}
			else
			{
				// Check, if we can steal the item
				// CHRISL: Added new case definitions for new inventory pockets
				switch (i)
				{
					case HANDPOS:
					case SECONDHANDPOS:
					case GUNSLINGPOCKPOS:
					case KNIFEPOCKPOS:
					case BIGPOCK1POS:
					case BIGPOCK2POS:
					case BIGPOCK3POS:
					case BIGPOCK4POS:
					case BIGPOCK5POS:
					case BIGPOCK6POS:
					case BIGPOCK7POS:
					case MEDPOCK1POS:
					case MEDPOCK2POS:
					case MEDPOCK3POS:
					case MEDPOCK4POS:
					case SMALLPOCK1POS:
					case SMALLPOCK2POS:
					case SMALLPOCK3POS:
					case SMALLPOCK4POS:
					case SMALLPOCK5POS:
					case SMALLPOCK6POS:
					case SMALLPOCK7POS:
					case SMALLPOCK8POS:
					case SMALLPOCK9POS:
					case SMALLPOCK10POS:
					case SMALLPOCK11POS:
					case SMALLPOCK12POS:
					case SMALLPOCK13POS:
					case SMALLPOCK14POS:
					case SMALLPOCK15POS:
					case SMALLPOCK16POS:
					case SMALLPOCK17POS:
					case SMALLPOCK18POS:
					case SMALLPOCK19POS:
					case SMALLPOCK20POS:
					case SMALLPOCK21POS:
					case SMALLPOCK22POS:
					case SMALLPOCK23POS:
					case SMALLPOCK24POS:
					case SMALLPOCK25POS:
					case SMALLPOCK26POS:
					case SMALLPOCK27POS:
					case SMALLPOCK28POS:
					case SMALLPOCK29POS:
					case SMALLPOCK30POS:
					fStealItem = TRUE;
					break;
				default:
					fStealItem = FALSE;
					break;
				}
			}

			if (fStealItem == TRUE)
			{
				pTempItemPool = (ITEM_POOL*)MemAlloc(sizeof(ITEM_POOL));
				++ubCount;
				if (pItemPool == NULL)
				{
					pItemPool = pTempItemPool;
					pItemPool->pPrev = NULL;
				}
				else
				{
					pTempItemPool->pPrev = pTempLastItemPool;
					pTempLastItemPool->pNext = pTempItemPool;
				}
				pTempItemPool->pNext = NULL;
				pTempItemPool->iItemIndex = i;
				//very bad, jack, to test if we are stealing?
				pTempItemPool->pLevelNode=NULL;//finally not used
				pTempLastItemPool=pTempItemPool;
			}
		}
	}
	if (ubCount == 0)
	{
		pSoldier->DoMercBattleSound( BATTLE_SOUND_NOTHING );
		return( 0);
	}

	//if only one item
	if (ubCount == 1)
	{
		*ubIndexRet=pItemPool->iItemIndex;
		MemFree( pItemPool);
		return( 1);
	}

	// Freeze guy!
	pSoldier->flags.fPauseAllAnimation = TRUE;

	InitializeStealItemPickupMenu( pSoldier, pOpponent, pItemPool, ubCount);
	guiPendingOverrideEvent = G_GETTINGITEM;
	HandleTacticalUI(); // 0verhaul:	Added to keep this event from going out of style before it is acted on
	return( ubCount );
}


/**
* jackaians: function copied from SoldierGetItemFromWorld
* check user's choices and try to place them in his slots
*/
void SoldierStealItemFromSoldier( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pOpponent,ITEM_POOL *pItemPool, INT32 iItemIndex, INT32 sGridNo, INT8 bZLevel, BOOLEAN *pfSelectionList )
{
	ITEM_POOL		*pTempItemPool;
	INT32			cnt = 0;
	BOOLEAN			fPickup;
	BOOLEAN			fShouldSayCoolQuote = FALSE;
	BOOLEAN			fDidSayCoolQuote = FALSE;
	BOOLEAN			fNotEnoughAPs = FALSE; // added by SANDRO

	// OK. CHECK IF WE ARE DOING ALL IN THIS POOL....
	if ( iItemIndex == ITEM_PICKUP_ACTION_ALL || iItemIndex == ITEM_PICKUP_SELECTION )
	{
		pTempItemPool = pItemPool;

		while( pTempItemPool )
		{
			fPickup = TRUE;
			if ( iItemIndex == ITEM_PICKUP_SELECTION )
			{
				if ( !pfSelectionList[ cnt ] )
				{
					fPickup = FALSE;
				}
			}
			// Increment counter...
			cnt++;
			if ( fPickup )
			{
				////////////////////////////////////////////////////////////////////
				// SANDRO - added mechanism for APs needed to steal all items..
				if ( gGameExternalOptions.fEnhancedCloseCombatSystem )
				{
					if (pSoldier->bActionPoints >= GetBasicAPsToPickupItem( pSoldier ) )
					{					
						// Make copy of item
						gTempObject = pOpponent->inv[pTempItemPool->iItemIndex];
						if ( ItemIsCool( &gTempObject ) )
						{
							fShouldSayCoolQuote = TRUE;
						}
						if ( !AutoPlaceObject( pSoldier, &gTempObject, TRUE ) )
						{
							AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
						}
						DeleteObj(&pOpponent->inv[pTempItemPool->iItemIndex]);

						// add to merc records
						if ( pSoldier->ubProfile != NO_PROFILE )
							gMercProfiles[ pSoldier->ubProfile ].records.usItemsStolen++;
						
						DeductPoints( pSoldier, GetBasicAPsToPickupItem( pSoldier ), 0, AFTERACTION_INTERRUPT );
					}
					else
					{
						fNotEnoughAPs = TRUE;
					}
				}
				else // original code
				{
					// Make copy of item
					gTempObject = pOpponent->inv[pTempItemPool->iItemIndex];
					if ( ItemIsCool( &gTempObject ) )
					{
						fShouldSayCoolQuote = TRUE;
					}
					if ( !AutoPlaceObject( pSoldier, &gTempObject, TRUE ) )
					{
						AddItemToPool( pSoldier->sGridNo, &gTempObject, 1, pSoldier->pathing.bLevel, 0, -1 );
					}
					DeleteObj(&pOpponent->inv[pTempItemPool->iItemIndex]);
				}
				////////////////////////////////////////////////////////////////////
			}
			pTempItemPool = pTempItemPool->pNext;
		}
	}
	// OK, check if potentially a good candidate for cool quote
	if ( fShouldSayCoolQuote && pSoldier->bTeam == gbPlayerNum )
	{
		// Do we have this quote..?
		if ( QuoteExp_GotGunOrUsedGun[ pSoldier->ubProfile ] == QUOTE_FOUND_SOMETHING_SPECIAL )
		{
			// Have we not said it today?
			if ( !( pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE ) )
			{
				// set flag
				pSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_FOUND_SOMETHING_NICE;
				// Say it....
				// We've found something!
				TacticalCharacterDialogue( pSoldier, QUOTE_FOUND_SOMETHING_SPECIAL );
				fDidSayCoolQuote = TRUE;
			}
		}
	}
	// Aknowledge....
	if( pSoldier->bTeam == OUR_TEAM && !fDidSayCoolQuote )
	{
		pSoldier->DoMercBattleSound( BATTLE_SOUND_GOTIT );
	}

	// SANDRO - show a message, that we had insufficient APs to take all items
	if ( fNotEnoughAPs && pSoldier->bTeam == gbPlayerNum && gGameExternalOptions.fEnhancedCloseCombatSystem)
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NOT_ENOUGH_APS_TO_STEAL_ALL], pSoldier->name );
	}

	gpTempSoldier = pSoldier;
	gsTempGridNo = sGridNo;
	SetCustomizableTimerCallbackAndDelay( 1000, CheckForPickedOwnership, TRUE );
}
