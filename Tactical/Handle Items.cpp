#include "connect.h"
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
	#include "Map Information.h"
	#include "GameSettings.h"
	#include "end game.h"
	#include "interface control.h"
	#include "Map Screen Interface Map Inventory.h"
	// added by SANDRO
	#include "Morale.h"
	// added by Flugente
	#include "drugs and alcohol.h"
	#include "Food.h"
	// added by sevenfm - this is needed for _keydown(SHIFT) to work
	#include "english.h"

	#include <iostream>	// added by Flugente
	#include <fstream>	// added by Flugente
	#include "DisplayCover.h"		// added by Flugente
	#include "Queen Command.h"		// added by Flugente for FindUnderGroundSector(...)
	#include "LuaInitNPCs.h"		// added by Flugente
	#include "finances.h"			// added by Flugente
	#include "LaptopSave.h"			// added by Flugente
	#include "Game Clock.h"			// added by Flugente
	#include <vfs/Core/vfs_file_raii.h>		// added by Flugente for vfs-stuff
	#include "DynamicDialogue.h" // added by Flugente for SoldierRelation()
	#include "MessageBoxScreen.h"	// added by Flugente
	#include "Map Screen Interface.h"		// added by Flugente
	#include "Map Screen Interface Map.h"	// added by Flugente

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#endif

#define					NUM_ITEMS_LISTED			8
#define					NUM_ITEM_FLASH_SLOTS	50
#define					MIN_LOB_RANGE					6

// directory for fortification plans (located in Profiles sub-folder)
#define FORTIFICATIONPLAN_DIRECTORY						"FortificationPlan\\"

#define EQUIPMENTTEMPLATE_DIRECTORY						"GearTemplate\\"

extern BOOL GetBetterObject_InventoryPool( UINT16 usItem, INT16 status, UINT32& arLoop, UINT8& arIndex );
extern BOOL GetFittingAmmo_InventoryPool( UINT8 usCalibre, UINT8 usAmmoType, UINT32& arLoop );

extern BOOLEAN HandleNailsVestFetish( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, UINT16 usReplaceItem );
extern void PlaySplashSound(INT32 sGridNo);

extern std::vector<WORLDITEM> pInventoryPoolList;
extern INT32 GetAttachmentInfoIndex( UINT16 usItem );

extern INT32 giItemDescAmmoButton;

ITEM_POOL_LOCATOR				FlashItemSlots[ NUM_ITEM_FLASH_SLOTS ];
UINT32									guiNumFlashItemSlots = 0;
// sevenfm: remember network settings for last planted tripwire
UINT8 gubLastTripwire = 0;
// set to TRUE if shift+click was pressed for planting bomb - will use for Improved Bomb Planting feature
BOOLEAN gfShiftBombPlant = FALSE;

// Flugente: (de-)construct structures
STRUCTURE_DECONSTRUCT gStructureDeconstruct[STRUCTURE_DECONSTRUCT_MAX];
STRUCTURE_CONSTRUCT gStructureConstruct[STRUCTURE_CONSTRUCT_MAX];

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
void TacticalFunctionSelectionMessageBoxCallBack( UINT8 ubExitValue );		// Flugente: callback after deciding what tactical function to use
void CorpseMessageBoxCallBack( UINT8 ubExitValue );		// Flugente: callback after deciding what to do with a corpse
void BoobyTrapMessageBoxCallBack( UINT8 ubExitValue );
void SwitchMessageBoxCallBack( UINT8 ubExitValue );
void BoobyTrapDialogueCallBack( void );
void MineSpottedDialogueCallBack( void );
void MineSpottedLocatorCallback( void );
void RemoveBlueFlagDialogueCallBack( UINT8 ubExitValue );
INT32 CheckBombDisarmChance(void);
void ExtendedDisarmMessageBox(void);
void ExtendedBoobyTrapMessageBoxCallBack( UINT8 ubExitValue );
void HandleTakeNewBombFromInventory(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj);
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

// added by Flugente
void StartTacticalFunctionSelectionMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo,  INT8 bLevel );
void UpdateGear();
void StartCorpseMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo,  INT8 bLevel );

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

	//shadooow: automatically close EDB when opened and trying to use any weapon or action
	if (gfInItemDescBox)
	{
		DeleteItemDescriptionBox();
	}

	// Remove any previous actions
	pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

	// here is where we would set a different value if the weapon mode is on
	// "attached weapon"
	pSoldier->usAttackingWeapon = usHandItem;

	// sevenfm: set shift flag for auto-taking of next item from inventory
	if( fFromUI && _KeyDown(SHIFT) )
		gfShiftBombPlant = TRUE;
	else
		gfShiftBombPlant = FALSE;

	// Find soldier flags depend on if it's our own merc firing or a NPC
	//if ( FindSoldier( sGridNo, &usSoldierIndex, &uiMercFlags, FIND_SOLDIER_GRIDNO )  )
	if ( ( usSoldierIndex = WhoIsThere2( sGridNo, bLevel ) ) != NOBODY )
	{
		pTargetSoldier = MercPtrs[ usSoldierIndex ];

		// anv: don't try to heal interactive spots
		if (fFromUI && Item[usHandItem].usItemClass != IC_MEDKIT)
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

	// Check our soldier's life for unconscious!
	if ( pSoldier->stats.bLife < OKLIFE )
	{
		return( ITEM_HANDLE_UNCONSCIOUS );
	}

	if (HandItemWorks( pSoldier, HANDPOS ) == FALSE )
	{
		return( ITEM_HANDLE_BROKEN );
	}

	if (fFromUI &&
		pSoldier->bTeam == gbPlayerNum &&
		pSoldier->ubProfile != NO_PROFILE &&
		pTargetSoldier &&		
		Item[usHandItem].usItemClass != IC_MEDKIT &&
		!Item[usHandItem].gascan &&
		!ItemCanBeAppliedToOthers(usHandItem) &&
		!HasItemFlag(usHandItem, EMPTY_BLOOD_BAG) &&
		!HasItemFlag( usHandItem, MEDICAL_SPLINT ) )
	{
		if (pTargetSoldier->bTeam == gbPlayerNum || pTargetSoldier->aiData.bNeutral)
		{
			// anv: don't try to attack yourself, it will only cause deadlock
			if (pSoldier == pTargetSoldier)
			{
				TacticalCharacterDialogue(pSoldier, QUOTE_REFUSING_ORDER);
				return(ITEM_HANDLE_REFUSAL);
			}

			// nice mercs won't shoot other nice guys or neutral civilians
			if ((gMercProfiles[pSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) &&
				((pTargetSoldier->ubProfile == NO_PROFILE && pTargetSoldier->aiData.bNeutral && pTargetSoldier->ubBodyType != CROW) ||
				pTargetSoldier->ubProfile != NO_PROFILE && gMercProfiles[pTargetSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY))
			{
				TacticalCharacterDialogue(pSoldier, QUOTE_REFUSING_ORDER);
				return(ITEM_HANDLE_REFUSAL);
			}

			if (pTargetSoldier->ubProfile != NO_PROFILE)
			{
				// Flugente: as relations are now dynamic, check that instead
				// buddies won't shoot each other
				INT8 bOpinion = SoldierRelation(pSoldier, pTargetSoldier);

				if (bOpinion > BUDDY_OPINION - 5 || ((gMercProfiles[pSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) && bOpinion > HATED_OPINION + 5))
				{
					TacticalCharacterDialogue(pSoldier, QUOTE_REFUSING_ORDER);
					return(ITEM_HANDLE_REFUSAL);
				}
			}

			// any recruited rebel will refuse to fire on another rebel or neutral nameless civ
			if (pSoldier->ubCivilianGroup == REBEL_CIV_GROUP &&
				(pTargetSoldier->ubCivilianGroup == REBEL_CIV_GROUP ||
				(pTargetSoldier->aiData.bNeutral && pTargetSoldier->ubProfile == NO_PROFILE && pTargetSoldier->ubCivilianGroup == NON_CIV_GROUP && pTargetSoldier->ubBodyType != CROW)))
			{
				TacticalCharacterDialogue(pSoldier, QUOTE_REFUSING_ORDER);
				return(ITEM_HANDLE_REFUSAL);
			}

			// civgroup loyal will attack members of the same group
			if (pSoldier->ubCivilianGroup != NON_CIV_GROUP &&
				pSoldier->HasBackgroundFlag(BACKGROUND_CIVGROUPLOYAL) &&
				pTargetSoldier->ubCivilianGroup == pSoldier->ubCivilianGroup)
			{
				TacticalCharacterDialogue(pSoldier, QUOTE_REFUSING_ORDER);
				return(ITEM_HANDLE_REFUSAL);
			}
		}

		// animal friend will refuse to attack animals
		if (pSoldier->HasBackgroundFlag(BACKGROUND_ANIMALFRIEND) &&
			(pTargetSoldier->ubBodyType == CROW || pTargetSoldier->ubBodyType == COW || pTargetSoldier->ubBodyType == BLOODCAT) &&
			pSoldier->ubPreviousAttackerID != pTargetSoldier->ubID &&
			pSoldier->ubNextToPreviousAttackerID != pTargetSoldier->ubID)
		{
			TacticalCharacterDialogue(pSoldier, QUOTE_REFUSING_ORDER);
			return(ITEM_HANDLE_REFUSAL);
		}
	}

	INT32 usMapPos;
	GetMouseMapPos( &usMapPos );

	// Flugente: interactive actions
	if ( Item[usHandItem].ubCursor == INVALIDCURS || usHandItem == NONE )
	{
		UINT16 structindex;
		UINT16 possibleaction = InteractiveActionPossibleAtGridNo( usMapPos, pSoldier->pathing.bLevel, structindex );
		if ( possibleaction )//&& pSoldier->GetInteractiveActionSkill( usMapPos, pSoldier->pathing.bLevel, possibleaction ) )
		{
			// ATE: AI CANNOT GO THROUGH HERE!
			BOOLEAN	fHadToUseCursorPos = FALSE;

			// See if we can get there to stab
			sActionGridNo = FindAdjacentGridEx( pSoldier, usMapPos, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
			if ( sActionGridNo == NOWHERE )
			{
				// Try another location...
				sActionGridNo = FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );

				if ( sActionGridNo == NOWHERE )
				{
					return(ITEM_HANDLE_CANNOT_GETTO_LOCATION);
				}
			}

			// Calculate AP costs...
			sAPCost = 0;//GetAPsToHandcuff( pSoldier, sActionGridNo );
			sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

			if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
			{
				// OK, set UI
				SetUIBusy( pSoldier->ubID );

				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_INTERACTIVEACTION;

					if ( fHadToUseCursorPos )
					{
						pSoldier->aiData.sPendingActionData2 = usMapPos;
					}
					else
					{
						if ( pTargetSoldier != NULL )
						{
							pSoldier->aiData.sPendingActionData2 = pTargetSoldier->sGridNo;
						}
						else
						{
							pSoldier->aiData.sPendingActionData2 = sGridNo;
						}
					}

					pSoldier->aiData.bPendingActionData3 = (INT8)(possibleaction);
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierInteractiveAction( sAdjustedGridNo, possibleaction );

					UnSetUIBusy( pSoldier->ubID );
				}

				if ( fFromUI )
				{
					guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
				}

				return(ITEM_HANDLE_OK);
			}
			else
			{
				return(ITEM_HANDLE_NOAPS);
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


		GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sTargetGridNo, TRUE, &fAddingTurningCost, &fAddingRaiseGunCost, pSoldier->aiData.bAimTime );
		usTurningCost = CalculateTurningCost(pSoldier, usHandItem, fAddingTurningCost);
		usRaiseGunCost = CalculateRaiseGunCost(pSoldier, fAddingRaiseGunCost, sTargetGridNo, pSoldier->aiData.bAimTime);

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
			// Flugente: drugs can temporarily cause a merc to go psycho
			if ( DoesMercHaveDisability( pSoldier, PSYCHO ) )
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

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 26 ], pSoldier->GetName() );
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
				// Flugente: drugs can temporarily cause a merc to go psycho
				if ( DoesMercHaveDisability( pSoldier, PSYCHO ) && PreRandom( 100 ) < 20 )
				{
					chanceToMisfire = diceSides;
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 26 ], pSoldier->GetName() );

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
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113HAMMessage[ 2 ], pSoldier->GetName() );
					}
					else
					{// More than 1 round
						if (__min(pSoldier->bDoAutofire,pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft) - startAuto > 1)
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 62 ], pSoldier->GetName(), __min(pSoldier->bDoAutofire,pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft) - startAuto );
						}
						// 1 round
						else
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[ 63 ], pSoldier->GetName() ); // SANDRO - no need for attaching the number if only one round is shot
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
		// See if we can get there to stab
		sActionGridNo = FindAdjacentGridEx(pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE);

		if (sActionGridNo != NOWHERE)
		{
			pSoldier->sTargetGridNo = sGridNo;

			pSoldier->aiData.usActionData	= sGridNo;
			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_PUNCH;
				pSoldier->aiData.sPendingActionData2	= sAdjustedGridNo;
				pSoldier->aiData.bPendingActionData3	= ubDirection;
				pSoldier->aiData.ubPendingActionAnimCount = 0;

				// WALK UP TO DEST FIRST
				pSoldier->EVENT_InternalGetNewSoldierPath(sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
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
		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo == -1 )
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
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

				if ( pTargetSoldier != NULL )
				{
					pSoldier->aiData.sPendingActionData2	= pTargetSoldier->sGridNo;
				}
				else
				{
					pSoldier->aiData.sPendingActionData2	= sGridNo;
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
		if ( sActionGridNo != -1 && IsCuttableWireFenceAtGridNo( sGridNo ) )
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
					// Flugente: if we are trying to defuse a tripwire, call EVENT_SoldierDefuseTripwire() instead
					INT32 tripwirefound = FindWorldItemForTripwireInGridNo( sGridNo, pSoldier->pathing.bLevel, TRUE );
					if ( tripwirefound != -1 )
						pSoldier->EVENT_SoldierDefuseTripwire( sGridNo, tripwirefound );
					else
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
		else if( sActionGridNo == -1 )
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
				// OK, set UI
				SetUIBusy(pSoldier->ubID);

				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_REPAIR;
					pSoldier->aiData.sPendingActionData2 = fVehicle ? sVehicleGridNo : sAdjustedGridNo;
					pSoldier->aiData.bPendingActionData3 = ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBeginRepair(fVehicle ? sVehicleGridNo : sAdjustedGridNo, ubDirection );					
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
					pSoldier->aiData.sPendingActionData2	= sVehicleGridNo;
					pSoldier->aiData.bPendingActionData3	= ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBeginRefuel(sVehicleGridNo, ubDirection );
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

	// Flugente: (de-)construct structures
	if ( IsStructureDeconstructItem( usHandItem, sGridNo, pSoldier ) || IsStructureConstructItem( usHandItem, sGridNo, pSoldier ) )
	{
		// if there is a structure, see if we can deconstruct it, if not, see if we can construct something
		STRUCTURE* pStruct = FindStructure( sGridNo, (STRUCTURE_GENERIC | STRUCTURE_WIREFENCE) );

		if ( pStruct )
		{
			if ( !IsStructureDeconstructItem( usHandItem, sGridNo, pSoldier ) )
				return(ITEM_HANDLE_REFUSAL);

			sAPCost = GetAPsForMultiTurnAction( pSoldier, MTA_REMOVE_FORTIFY );
		}
		else
		{
			if ( !IsStructureConstructItem( usHandItem, sGridNo, pSoldier ) )
				return(ITEM_HANDLE_REFUSAL);

			sAPCost = GetAPsForMultiTurnAction( pSoldier, MTA_FORTIFY );
		}

		// we are at pSoldier->sGridNo and the action takes place at sGridNo
		// we need to find out in which direction we need to look
		// sActionGridNo is the gridno we are supposed to sit
		ubDirection = (UINT8)GetDirectionToGridNoFromGridNo( sGridNo, pSoldier->sGridNo );
			
		sActionGridNo = FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, FALSE, FALSE );

		if ( !TileIsOutOfBounds( sActionGridNo ) )
		{			
			// Calculate AP costs...
			sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

			if ( pSoldier->bActionPoints > 0 )
			{
				// CHECK IF WE ARE AT THIS GRIDNO NOW
				if ( pSoldier->sGridNo != sActionGridNo )
				{
					// SEND PENDING ACTION
					pSoldier->aiData.ubPendingAction = MERC_BUILD_FORTIFICATION;
					pSoldier->aiData.sPendingActionData2	= sGridNo;
					pSoldier->aiData.bPendingActionData3	= ubDirection;
					pSoldier->aiData.ubPendingActionAnimCount = 0;

					// WALK UP TO DEST FIRST
					pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
				}
				else
				{
					pSoldier->EVENT_SoldierBuildStructure( sGridNo, ubDirection );
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

	// Flugente: handcuffing people
	if (gGameExternalOptions.fAllowPrisonerSystem && HasItemFlag(usHandItem, HANDCUFFS) && pTargetSoldier && pTargetSoldier->bVisible >= 0)
	{
		if (!pTargetSoldier->CanBeCaptured())
			return ITEM_HANDLE_REFUSAL;

		// ATE: AI CANNOT GO THROUGH HERE!
		BOOLEAN	fHadToUseCursorPos = FALSE;

		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo == -1 )
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );

			/*if ( !gTacticalStatus.fAutoBandageMode )
			{
				fHadToUseCursorPos = TRUE;
			}*/
		}

		// Calculate AP costs...
		sAPCost = GetAPsToHandcuff( pSoldier, sActionGridNo );
		sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_HANDCUFF_PERSON;

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
				pSoldier->EVENT_SoldierHandcuffPerson( sAdjustedGridNo, ubDirection );

				UnSetUIBusy( pSoldier->ubID );
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

	// Flugente: camera
	if ( HasItemFlag( usHandItem, CAMERA ) )
	{
		sAPCost = APBPConstants[AP_CAMERA];

		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, gsInterfaceLevel, 0, TRUE, CALC_FROM_WANTED_DIR, TRUE ) )
			{
				TakePhoto( pSoldier, usMapPos, gsInterfaceLevel );

				return( ITEM_HANDLE_OK );
			}
			else
			{
				return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
			}
		}
		else
		{
			return( ITEM_HANDLE_NOAPS );
		}
	}

	UINT8 ubPerson = WhoIsThere2(usMapPos, pSoldier->pathing.bLevel);

	// Flugente: apply misc items to other soldiers
	// sevenfm: check that target soldier is visible
	if (ItemCanBeAppliedToOthers(usHandItem) && ubPerson != NOBODY && MercPtrs[ubPerson] && MercPtrs[ubPerson]->bVisible != 0)
	{
		// ATE: AI CANNOT GO THROUGH HERE!
		BOOLEAN	fHadToUseCursorPos = FALSE;

		// See if we can get there to stab
		sActionGridNo =	FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo == -1 )
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}

		// Calculate AP costs...
		sAPCost = GetAPsToApplyItem( pSoldier, sActionGridNo );
		sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints);

		if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_APPLYITEM;

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
				pSoldier->EVENT_SoldierApplyItemToPerson( sAdjustedGridNo, ubDirection );

				UnSetUIBusy( pSoldier->ubID );
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

	// Flugente: take blood from a donor
	if ( HasItemFlag( usHandItem, EMPTY_BLOOD_BAG ) )
	{
		// ATE: AI CANNOT GO THROUGH HERE!
		BOOLEAN	fHadToUseCursorPos = FALSE;

		// See if we can get there to stab
		sActionGridNo = FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo == -1 )
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}

		// Calculate AP costs...
		sAPCost = GetAPsToFillBloodbag( pSoldier, sActionGridNo );
		sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

		// if we are at the action gridno, the item is a bomb, but nobody is at the gridno, do not apply and do not return - we will plant the bomb instead (handlded later in this function)
		if ( Item[usHandItem].usItemClass == IC_BOMB && pSoldier->sGridNo == sActionGridNo && WhoIsThere2( usMapPos, pSoldier->pathing.bLevel ) == NOBODY )
		{
			;
		}
		else if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_FILLBLOODBAG;

				if ( fHadToUseCursorPos )
				{
					pSoldier->aiData.sPendingActionData2 = usMapPos;
				}
				else
				{
					if ( pTargetSoldier != NULL )
					{
						pSoldier->aiData.sPendingActionData2 = pTargetSoldier->sGridNo;
					}
					else
					{
						pSoldier->aiData.sPendingActionData2 = sGridNo;
					}
				}
				pSoldier->aiData.bPendingActionData3 = ubDirection;
				pSoldier->aiData.ubPendingActionAnimCount = 0;

				// WALK UP TO DEST FIRST
				pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
			}
			else
			{
				pSoldier->EVENT_SoldierTakeBloodFromPerson( sAdjustedGridNo, ubDirection );

				UnSetUIBusy( pSoldier->ubID );
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

	// Flugente: apply medical splint
	if ( HasItemFlag( usHandItem, MEDICAL_SPLINT ) )
	{
		// ATE: AI CANNOT GO THROUGH HERE!
		BOOLEAN	fHadToUseCursorPos = FALSE;

		// See if we can get there to stab
		sActionGridNo = FindAdjacentGridEx( pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE );
		if ( sActionGridNo == -1 )
		{
			return( ITEM_HANDLE_CANNOT_GETTO_LOCATION );
		}

		// Calculate AP costs...
		sAPCost = GetAPsToApplyItem( pSoldier, sActionGridNo );
		sAPCost += PlotPath( pSoldier, sActionGridNo, NO_COPYROUTE, FALSE, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

		// if we are at the action gridno, the item is a bomb, but nobody is at the gridno, do not apply and do not return - we will plant the bomb instead (handlded later in this function)
		if ( Item[usHandItem].usItemClass == IC_BOMB && pSoldier->sGridNo == sActionGridNo && WhoIsThere2( usMapPos, pSoldier->pathing.bLevel ) == NOBODY )
		{
			;
		}
		else if ( EnoughPoints( pSoldier, sAPCost, 0, fFromUI ) )
		{
			// OK, set UI
			SetUIBusy( pSoldier->ubID );

			// CHECK IF WE ARE AT THIS GRIDNO NOW
			if ( pSoldier->sGridNo != sActionGridNo )
			{
				// SEND PENDING ACTION
				pSoldier->aiData.ubPendingAction = MERC_MEDICALSPLINT;

				if ( fHadToUseCursorPos )
				{
					pSoldier->aiData.sPendingActionData2 = usMapPos;
				}
				else
				{
					if ( pTargetSoldier != NULL )
					{
						pSoldier->aiData.sPendingActionData2 = pTargetSoldier->sGridNo;
					}
					else
					{
						pSoldier->aiData.sPendingActionData2 = sGridNo;
					}
				}
				pSoldier->aiData.bPendingActionData3 = ubDirection;
				pSoldier->aiData.ubPendingActionAnimCount = 0;

				// WALK UP TO DEST FIRST
				pSoldier->EVENT_InternalGetNewSoldierPath( sActionGridNo, pSoldier->usUIMovementMode, FALSE, TRUE );
			}
			else
			{
				pSoldier->EVENT_SoldierApplySplintToPerson( sAdjustedGridNo, ubDirection );

				UnSetUIBusy( pSoldier->ubID );
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
						pSoldier->EVENT_SoldierBeginAttachCan( sGridNo, ubDirection );
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
		if ( HasAttachmentOfClass( &(pSoldier->inv[ pSoldier->ubAttackingHand ] ), (AC_DETONATOR | AC_REMOTEDET | AC_DEFUSE) ) )
		{
			fDropBomb = TRUE;
		}
	}

	if ( fDropBomb )
	{
		// Save gridno....
       pSoldier->aiData.sPendingActionData2    = sGridNo;                

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
			// See if we can get there to stab
			sActionGridNo = FindAdjacentGridEx(pSoldier, sGridNo, &ubDirection, &sAdjustedGridNo, TRUE, FALSE, true);
		}

		if ( sActionGridNo != NOWHERE )
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

		sAPCost = MinAPsToAttack( pSoldier, sTargetGridNo, TRUE, pSoldier->aiData.bAimTime, 0 );

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

			//pSoldier->flags.fDontChargeAPsForStanceChange = TRUE;//dnl ch72 270913 no reason why not charge for stance change
		}
		else if ( Item[usHandItem].grenadelauncher )//usHandItem == GLAUNCHER || usHandItem == UNDER_GLAUNCHER )
		{
			GetAPChargeForShootOrStabWRTGunRaises( pSoldier, sTargetGridNo, TRUE, &fAddingTurningCost, &fAddingRaiseGunCost, pSoldier->aiData.bAimTime );
			usTurningCost = CalculateTurningCost(pSoldier, usHandItem, fAddingTurningCost);
			usRaiseGunCost = CalculateRaiseGunCost(pSoldier, fAddingRaiseGunCost, sTargetGridNo, pSoldier->aiData.bAimTime );

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

				//dnl ch72 180913 decision is to charge for turning which was disabled in v1.12 also will turn off both options because is bad to charge APs before stance or turning really occurs
				// ATE: Don't charge turning...
				//pSoldier->flags.fDontChargeTurningAPs = TRUE;
				//pSoldier->flags.fDontChargeAPsForStanceChange = TRUE;

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
		if ( HasAttachmentOfClass( &(pSoldier->inv[pSoldier->ubAttackingHand]), (AC_DETONATOR | AC_REMOTEDET | AC_DEFUSE) ) || Item[usHandItem].tripwire )
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
	if ( HasAttachmentOfClass( &(pSoldier->inv[ HANDPOS ] ), (AC_DETONATOR | AC_REMOTEDET | AC_DEFUSE) ) || Item[ (&(pSoldier->inv[ HANDPOS ] ))->usItem ].tripwire == 1)
	{
		StartBombMessageBox( pSoldier, sGridNo );
	}
	else
	{
		// We have something... all we do is place...
		if ( ArmBomb( &(pSoldier->inv[ HANDPOS ]), 0 ) )
		{
			INT32 iResult = 0;

			if ( HasItemFlag( (pSoldier->inv[HANDPOS]).usItem, BEARTRAP ) )
			{
				iResult = SkillCheck( pSoldier, PLANTING_MECHANICAL_BOMB_CHECK, 0 );
			}
			else
			{
				iResult = SkillCheck( pSoldier, PLANTING_BOMB_CHECK, 0 );
			}

			if ( iResult >= 0 )
			{
				// Less explosives gain for placing tripwire
				if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].tripwire )
					StatChange( pSoldier, EXPLODEAMT, 1, FALSE );
				else if ( HasItemFlag( (pSoldier->inv[HANDPOS]).usItem, BEARTRAP ) )
					StatChange( pSoldier, MECHANAMT, 10, FALSE );
				else
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

				// Flugente: backgrounds
				if ( pSoldier->HasBackgroundFlag( BACKGROUND_TRAPLEVEL ) )
					pSoldier->inv[ HANDPOS ][0]->data.bTrap++;

				// anv: additional tile properties - modify trap level depending on its placement
				if(gGameExternalOptions.fAdditionalTileProperties && Item[ pSoldier->inv[ HANDPOS ].usItem ].tripwire != 1 )
				{
					ADDITIONAL_TILE_PROPERTIES_VALUES zAllTileValues = GetAllAdditonalTilePropertiesForGrid( sGridNo, pSoldier->pathing.bLevel );
					pSoldier->inv[ HANDPOS ][0]->data.bTrap += zAllTileValues.bTrapBonus;
				}
				pSoldier->inv[ HANDPOS ][0]->data.bTrap = __max( 0, pSoldier->inv[ HANDPOS ][0]->data.bTrap );

				pSoldier->inv[ HANDPOS ][0]->data.misc.ubBombOwner = pSoldier->ubID + 2;

				// Flugente: determine the direction we are looking at and apply that direction to our explosive
				pSoldier->inv[ HANDPOS ][0]->data.ubDirection = pSoldier->ubDirection;

				// no frequency known... give a default value, so we don't defuse it by accident
				gTempObject[0]->data.ubWireNetworkFlag = (TRIPWIRE_NETWORK_OWNER_ENEMY|TRIPWIRE_NETWORK_NET_1|TRIPWIRE_NETWORK_LVL_1);
				gTempObject[0]->data.bDefuseFrequency = 0;

				// we now know there is something nasty here
				gpWorldLevelData[ sGridNo ].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;

				if (pSoldier->inv[ HANDPOS ].MoveThisObjectTo(gTempObject, 1) == 0) 
				{
					AddItemToPool( sGridNo, &gTempObject, BURIED, pSoldier->pathing.bLevel, WORLD_ITEM_ARMED_BOMB, 0 );
					// sevenfm: take another item with same id from inventory, only REALTIME
                   HandleTakeNewBombFromInventory(pSoldier, &gTempObject);
					// sevenfm: change cursor back to action if successfully taken new bomb
					if ( gfShiftBombPlant && pSoldier->inv[ pSoldier->ubAttackingHand ].exists() )
						guiPendingOverrideEvent = M_CHANGE_TO_ACTION;
				}

				// Flugente: additional dialogue
				AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_BOMB_HAS_BEEN_PLANTED, pSoldier->inv[HANDPOS].usItem, 1 );
			}
			else
			{
				// Flugente: additional dialogue
				AdditionalTacticalCharacterDialogue_CallsLua( pSoldier, ADE_BOMB_HAS_BEEN_PLANTED, pSoldier->inv[HANDPOS].usItem, 0 );

				// beartraps don't explode...
				if ( HasItemFlag( (pSoldier->inv[HANDPOS]).usItem, BEARTRAP ) )
					return;

				// EXPLOSIVES GAIN (10):	Failed to place a bomb, or bury and arm a mine
				StatChange( pSoldier, EXPLODEAMT, 10, FROM_FAILURE );

				// oops!	How badly did we screw up?
				if ( iResult < -20 && Item[ pSoldier->inv[ HANDPOS ].usItem ].tripwire != 1 )
				{
					// OOPS! ... BOOM!
					IgniteExplosion( NOBODY, pSoldier->sX, pSoldier->sY, (INT16) (gpWorldLevelData[pSoldier->sGridNo].sHeight), pSoldier->sGridNo, pSoldier->inv[ HANDPOS ].usItem, pSoldier->pathing.bLevel, pSoldier->ubDirection, &pSoldier->inv[ HANDPOS ] );
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

void HandleTacticalFunctionSelection( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	if ( guiCurrentScreen == GAME_SCREEN )
		StartTacticalFunctionSelectionMessageBox( pSoldier, sGridNo, pSoldier->pathing.bLevel );
}


void HandleSoldierUseCorpse( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel )
{
	StartCorpseMessageBox( pSoldier, sGridNo, bLevel );
}

void HandleSoldierDefuseTripwire( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT32 sItem )
{
	gpBoobyTrapItemPool   = GetItemPoolForIndex( sGridNo, sItem, pSoldier->pathing.bLevel );
	gpBoobyTrapSoldier    = pSoldier;
	gsBoobyTrapGridNo     = sGridNo;
	gbBoobyTrapLevel      = pSoldier->pathing.bLevel;
	gfDisarmingBuriedBomb = FALSE;
	gbTrapDifficulty      = (gWorldItems[ sItem ].object)[0]->data.bTrap;

	DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ DISARM_BOOBYTRAP_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack, NULL );
}

void SoldierHandleDropItem( SOLDIERTYPE *pSoldier )
{
	// LOOK IN PANDING DATA FOR ITEM TO DROP, AND LOCATION
	if ( pSoldier->pTempObject != NULL )
	{
		if ( pSoldier->bVisible != -1 )
		{
			if (Water(pSoldier->sGridNo, pSoldier->pathing.bLevel))
			{
				UINT16 usItem = pSoldier->pTempObject->usItem;
				INT32 sGridNo = pSoldier->sGridNo;

				if (HasItemFlag(usItem, CORPSE))
					PlayJA2Sample(ENTER_DEEP_WATER_1, RATE_11025, SoundVolume(MIDVOLUME, sGridNo), 1, SoundDir(sGridNo));
				else if (Item[usItem].ubWeight > 10)
					PlayJA2Sample(ENTER_WATER_1, RATE_11025, SoundVolume(MIDVOLUME, sGridNo), 1, SoundDir(sGridNo));
				else
					PlaySplashSound(sGridNo);
			}
			else
			{
				PlayJA2Sample(THROW_IMPACT_2, RATE_11025, SoundVolume(MIDVOLUME, pSoldier->sGridNo), 1, SoundDir(pSoldier->sGridNo));
			}
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

	INT16 sXTest, sYTest;
	ConvertGridNoToCenterCellXY(pSoldier->sGridNo, &sXTest, &sYTest);
	//shadooow: if the grenade isn't thrown from soldier gridno, it should mean it is thrown using my grenade rolling feature
	BOOLEAN gbGrenadeRolling = pSoldier->pThrowParams->dX != sXTest || pSoldier->pThrowParams->dY != sYTest;

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

			if (gbGrenadeRolling)
			{
				if ((pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM) &&
					((pSoldier->ubBodyType == BIGMALE) || (pSoldier->ubBodyType == REGMALE)))
					pSoldier->usPendingAnimation = LOB_GRENADE_STANCE;
				else
					pSoldier->usPendingAnimation = LOB_ITEM;
			}
			// Draw item depending on distance from buddy
			else if ( GetRangeFromGridNoDiff( sGridNo, pSoldier->sGridNo ) < MIN_LOB_RANGE )
			{
				//ddd maybe need to add check for throwing item class - grenade
				if( (pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM) && 
					( (pSoldier->ubBodyType == BIGMALE) || (pSoldier->ubBodyType == REGMALE) ) )
					pSoldier->usPendingAnimation = LOB_GRENADE_STANCE;
				else
					pSoldier->usPendingAnimation = LOB_ITEM;
			}
			else			
			{
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

			if (gbGrenadeRolling)
			{
				pSoldier->usPendingAnimation = THROW_ITEM_CROUCHED;
			}
			else
			{
				pSoldier->usPendingAnimation = THROW_ITEM_CROUCHED;
			}
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
	BOOLEAN					fSaidBoobyTrapQuote = FALSE;
	UINT16					sAPCost;//Moa: added for deduct points
	BOOLEAN					fItemTaken = FALSE;//Moa: added for deduct points
#ifdef JA2UB
	UINT16					usItem=0;
#endif

	sAPCost = GetAPsToPickupItem( pSoldier, sGridNo );
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

							// Flugente: if we allow militia to use sector equipment, then mark gear they pick up, so that they drop it too.
							if ( pSoldier->bTeam == MILITIA_TEAM && gGameExternalOptions.fMilitiaUseSectorInventory )
							{
								if ( Item[gTempObject.usItem].usItemClass & IC_ARMOUR && gGameExternalOptions.fMilitiaUseSectorInventory_Armour )
								{
									gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
								}
								else if ( Item[gTempObject.usItem].usItemClass & IC_FACE && gGameExternalOptions.fMilitiaUseSectorInventory_Face )
								{
									gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
								}
								else if ( Item[gTempObject.usItem].usItemClass & (IC_BLADE|IC_PUNCH) && gGameExternalOptions.fMilitiaUseSectorInventory_Melee )
								{
									gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
								}
								else if ( Item[gTempObject.usItem].usItemClass & IC_GRENADE && gGameExternalOptions.fMilitiaUseSectorInventory_Grenade )
								{
									gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
								}
								else if ( Item[gTempObject.usItem].usItemClass & (IC_GUN|IC_AMMO) && gGameExternalOptions.fMilitiaUseSectorInventory_Gun )
								{
									gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
								}
								else if ( Item[gTempObject.usItem].usItemClass & IC_LAUNCHER && gGameExternalOptions.fMilitiaUseSectorInventory_Launcher )
								{
									gTempObject[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
								}

								if ( gWorldItems[ iItemIndex ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN )
									gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA_TABOO_GREEN;
								if ( gWorldItems[ iItemIndex ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE )
									gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA_TABOO_BLUE;
							}

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
							else
							{
								fItemTaken = TRUE;
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
			// Flugente: if item is tripwireactivated and is a planted bomb, call the defuse dialogue. We obviously know about the items' existence already...
			if ( gWorldItems[ iItemIndex ].object.exists() && gWorldItems[ iItemIndex ].object.fFlags & OBJECT_ARMED_BOMB && Item[gWorldItems[ iItemIndex ].object.usItem].tripwireactivation == 1 )
			{
				gpBoobyTrapItemPool = GetItemPoolForIndex( sGridNo, iItemIndex, pSoldier->pathing.bLevel );
				gpBoobyTrapSoldier = pSoldier;
				gsBoobyTrapGridNo = sGridNo;
				gbBoobyTrapLevel	= pSoldier->pathing.bLevel;
				gfDisarmingBuriedBomb = FALSE;
				gbTrapDifficulty = (gWorldItems[ iItemIndex ].object)[0]->data.bTrap;

				DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ DISARM_BOOBYTRAP_PROMPT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack, NULL );
			}
			else if ( ContinuePastBoobyTrap( pSoldier, sGridNo, bZLevel, iItemIndex, FALSE, &fSaidBoobyTrapQuote ) )
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

					// Flugente: if we allow militia to use sector equipment, then mark gear they pick up, so that they drop it too.
					if ( pSoldier->bTeam == MILITIA_TEAM && gGameExternalOptions.fMilitiaUseSectorInventory )
					{
						if ( Item[gWorldItems[ iItemIndex ].object.usItem].usItemClass & IC_ARMOUR && gGameExternalOptions.fMilitiaUseSectorInventory_Armour )
						{
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
						}
						else if ( Item[gWorldItems[ iItemIndex ].object.usItem].usItemClass & IC_FACE && gGameExternalOptions.fMilitiaUseSectorInventory_Face )
						{
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= IC_FACE;
						}
						else if ( Item[gWorldItems[ iItemIndex ].object.usItem].usItemClass & (IC_BLADE|IC_PUNCH) && gGameExternalOptions.fMilitiaUseSectorInventory_Melee )
						{
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
						}
						else if ( Item[gWorldItems[ iItemIndex ].object.usItem].usItemClass & IC_GRENADE && gGameExternalOptions.fMilitiaUseSectorInventory_Grenade )
						{
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
						}
						else if ( Item[gWorldItems[ iItemIndex ].object.usItem].usItemClass & (IC_GUN|IC_AMMO) && gGameExternalOptions.fMilitiaUseSectorInventory_Gun )
						{
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
						}
						else if ( Item[gWorldItems[ iItemIndex ].object.usItem].usItemClass & IC_LAUNCHER && gGameExternalOptions.fMilitiaUseSectorInventory_Launcher )
						{
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA;
						}

						if ( gWorldItems[ iItemIndex ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN )
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA_TABOO_GREEN;
						if ( gWorldItems[ iItemIndex ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE )
							gWorldItems[ iItemIndex ].object[0]->data.sObjectFlag |= TAKEN_BY_MILITIA_TABOO_BLUE;
					}
					
					if ( !AutoPlaceObject( pSoldier, &(gWorldItems[ iItemIndex ].object ), TRUE ) )
					{
						//ADB well we made an animation, should we still use up points?
						gfDontChargeAPsToPickup = TRUE;
						HandleAutoPlaceFail( pSoldier, iItemIndex, sGridNo );
					}
					else
					{
						fItemTaken = TRUE;
					}

					// Flugente: if we remove an object, we basically give it free to overwriting. Thus it is very, VERY bad to theninteract with it, as the code might jsut do that - 
					// the object data may be overwritten at any moment! Better remove it after manipulation is finished.
					RemoveItemFromPool( sGridNo, iItemIndex, pSoldier->pathing.bLevel );
				}
			}
		}
	}

	if (fItemTaken)
	{
		//deduct Points
		DeductPoints( pSoldier, sAPCost, 0 );
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
	if( iItemIndex != 0 )
	{
		//handle the picking up of a new ja25 gun
		HandleNewGunComment( pSoldier, iItemIndex, TRUE );
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
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113HAMMessage[4], Item[gWorldItems[ iItemIndex ].object.usItem].szItemName );
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

//				DoMessageBox( MSG_BOX_BASIC_STYLE, ptr, GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack, NULL );
				// sevenfm: extended disarm messagebox (inspect, remove blueflag, blow up). works in realtime and turnbased
				ExtendedDisarmMessageBox();
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

	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps
	if ( TERRAIN_IS_WATER( bTerrainID) && ubLevel > 0 )
		bTerrainID = FLAT_GROUND;

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

	// Flugente: if this item is a corpse, don't add it to the world. Instead spawn a corpse in its position
	if ( HasItemFlag(pObject->usItem, CORPSE) && AddCorpseFromObject(pObject, *psGridNo, ubLevel ) )
	{
		return( NULL );
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

	if ( (*pObject)[0]->data.sObjectFlag & TAKEN_BY_MILITIA_TABOO_GREEN )
	{
		(*pObject)[0]->data.sObjectFlag &= ~ TAKEN_BY_MILITIA_TABOO_GREEN;
		gWorldItems[ iWorldItem ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN;
	}
	if ( (*pObject)[0]->data.sObjectFlag & TAKEN_BY_MILITIA_TABOO_BLUE )
	{
		(*pObject)[0]->data.sObjectFlag &= ~ TAKEN_BY_MILITIA_TABOO_BLUE;
		gWorldItems[ iWorldItem ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE;
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

// Flugente 2016-04-09: this function seems dubious. The only difference from GetLargestZLevelOfItemPool is that this guarantees 0 if there is a structure.
// That doesn't make any sense, so I'm replacing all calls of this
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

void RevealAllUnburiedItems( INT32 sGridNo, UINT8 ubLevel )
{
	ITEM_POOL		*pItemPool;

	// Check for and existing pool on the object layer
	GetItemPool( sGridNo, &pItemPool, ubLevel );

	while ( pItemPool )
	{
		if ( gWorldItems[pItemPool->iItemIndex].bVisible != BURIED )
		{
			gWorldItems[pItemPool->iItemIndex].bVisible = VISIBLE;
			gWorldItems[pItemPool->iItemIndex].bRenderZHeightAboveLevel = 0;
			gWorldItems[pItemPool->iItemIndex].ubNonExistChance = 0;
			gWorldItems[pItemPool->iItemIndex].usFlags &= ( ~WORLD_ITEM_DONTRENDER );
		}

		pItemPool = pItemPool->pNext;
	}
		
	NotifySoldiersToLookforItems();
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

BOOLEAN MoveItemPools_ForDragging( INT32 sStartPos, INT32 sEndPos, INT8 bStartLevel, INT8 bEndLevel )
{
	// note, only works between locations on the ground
	ITEM_POOL*		pItemPool;
	WORLDITEM		TempWorldItem;
	
	// loop over all items and note the ones we have to move. Move them afterwards, if we do it during the operation we risk pointer issues
	std::vector<INT32> itemindexestomove_vector;

	if ( bStartLevel )
		GetItemPoolFromRoof( sStartPos, &pItemPool );
	else
		GetItemPoolFromGround( sStartPos, &pItemPool );
	
	while ( pItemPool )
	{
		TempWorldItem = gWorldItems[pItemPool->iItemIndex];

		// don't move an item if it's an armed bomb, an action item or if it isn't a fresh drop
		if ( TempWorldItem.object.usItem != ACTION_ITEM
			&& !( TempWorldItem.usFlags & WORLD_ITEM_ARMED_BOMB ) )
		{
			itemindexestomove_vector.push_back( pItemPool->iItemIndex );
		}

		pItemPool = pItemPool->pNext;
	}

	// now move the stuff
	for ( std::vector<INT32>::iterator it = itemindexestomove_vector.begin(), itend = itemindexestomove_vector.end(); it != itend; ++it )
	{
		TempWorldItem = gWorldItems[(*it)];
		
		RemoveItemFromPool( sStartPos, ( *it ), bStartLevel );
		AddItemToPool( sEndPos, &( TempWorldItem.object ), 1, bEndLevel, TempWorldItem.usFlags, TempWorldItem.bRenderZHeightAboveLevel );
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

BOOLEAN	GetItemPoolFromRoof( INT32 sMapPos, ITEM_POOL **ppItemPool )
{
	// Flugente: apparently the ...Ground version is called a lot, otherwise it would be smarter to just add an extra argument for level
	LEVELNODE *pObject = gpWorldLevelData[sMapPos].pOnRoofHead;

	( *ppItemPool ) = NULL;

	// LOOP THORUGH OBJECT LAYER
	while ( pObject != NULL )
	{
		if ( pObject->uiFlags & LEVELNODE_ITEM )
		{
			( *ppItemPool ) = pObject->pItemPool;

			//DEF added the check because pObject->pItemPool was NULL which was causing problems
			if ( *ppItemPool )
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
				if (gGameExternalOptions.gfShowBackpackOwner &&
					Item[gWorldItems[pItemPool->iItemIndex].object.usItem].usItemClass == IC_LBEGEAR &&
					LoadBearingEquipment[Item[gWorldItems[pItemPool->iItemIndex].object.usItem].ubClassIndex].lbeClass == BACKPACK &&
					gWorldItems[pItemPool->iItemIndex].soldierID != -1 &&
					MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID])
					swprintf(pStr, L"%s (%d) (%s)", ShortItemNames[gWorldItems[pTempItemPool->iItemIndex].object.usItem], gWorldItems[pTempItemPool->iItemIndex].object.ubNumberOfObjects, MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID]->GetName());
				else
					swprintf( pStr, L"%s (%d)", ShortItemNames[ gWorldItems[ pTempItemPool->iItemIndex ].object.usItem ], gWorldItems[ pTempItemPool->iItemIndex ].object.ubNumberOfObjects );
			}
			else
			{
				if (gGameExternalOptions.gfShowBackpackOwner &&
					Item[gWorldItems[pItemPool->iItemIndex].object.usItem].usItemClass == IC_LBEGEAR &&
					LoadBearingEquipment[Item[gWorldItems[pItemPool->iItemIndex].object.usItem].ubClassIndex].lbeClass == BACKPACK &&
					gWorldItems[pItemPool->iItemIndex].soldierID != -1 &&
					MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID])
					swprintf(pStr, L"%s (%s)", ShortItemNames[gWorldItems[pTempItemPool->iItemIndex].object.usItem], MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID]->GetName());
				else
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
				if (gGameExternalOptions.gfShowBackpackOwner &&
					Item[gWorldItems[pItemPool->iItemIndex].object.usItem].usItemClass == IC_LBEGEAR &&
					LoadBearingEquipment[Item[gWorldItems[pItemPool->iItemIndex].object.usItem].ubClassIndex].lbeClass == BACKPACK &&
					gWorldItems[pItemPool->iItemIndex].soldierID != -1 &&
					MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID])
					swprintf(pStr, L"%s (%d) (%s)", ShortItemNames[gWorldItems[pItemPool->iItemIndex].object.usItem], gWorldItems[pItemPool->iItemIndex].object.ubNumberOfObjects, MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID]->GetName());
				else
					swprintf( pStr, L"%s (%d)", ShortItemNames[ gWorldItems[ pItemPool->iItemIndex ].object.usItem ], gWorldItems[ pItemPool->iItemIndex ].object.ubNumberOfObjects );
			}
			else
			{
				if (gGameExternalOptions.gfShowBackpackOwner &&
					Item[gWorldItems[pItemPool->iItemIndex].object.usItem].usItemClass == IC_LBEGEAR &&
					LoadBearingEquipment[Item[gWorldItems[pItemPool->iItemIndex].object.usItem].ubClassIndex].lbeClass == BACKPACK &&
					gWorldItems[pItemPool->iItemIndex].soldierID != -1 &&
					MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID])
					swprintf(pStr, L"%s (%s)", ShortItemNames[gWorldItems[pItemPool->iItemIndex].object.usItem], MercPtrs[gWorldItems[pItemPool->iItemIndex].soldierID]->GetName());
				else
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

	for( uiCount=0; uiCount < guiNumFlashItemSlots; ++uiCount)
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
	//LEVELNODE		*pObject;
	ITEM_POOL_LOCATOR	*pLocator;
	BOOLEAN			fDoLocator = FALSE;

	if ( COUNTERDONE( CYCLERENDERITEMCOLOR ) )
	{
		RESETCOUNTER( CYCLERENDERITEMCOLOR );

		for ( cnt = 0; cnt < guiNumFlashItemSlots; ++cnt )
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
						UINT32 uiClock = GetJA2Clock( );

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

					// Flugente: as someone commented out the changes to uiFlags, this part serves no purpose, so I'm commenting it out
					/*if ( pItemPool->ubLevel == 0 )
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
					}*/

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

	for ( cnt = 0; cnt < guiNumFlashItemSlots; ++cnt )
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
		if ( gMercProfiles[ubProfile].Type == PROFILETYPE_AIM ||
			gMercProfiles[ubProfile].Type == PROFILETYPE_MERC ||
			gMercProfiles[ubProfile].Type == PROFILETYPE_IMP ||
			RPC_RECRUITED( pTSoldier ) )		
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
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ ITEM_HAS_BEEN_GIVEN_TO_STR ], ShortItemNames[ usItemNum ], pTSoldier->GetName() );
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
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ GUY_HAS_BEEN_PAID_IN_FULL_STR ], pTSoldier->GetName() );
						}
						else
						{
							// report the payment
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ GUY_STILL_OWED_STR ], pTSoldier->GetName(), -gMercProfiles[pTSoldier->ubProfile].iBalance );
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

// Flugente
void StartCorpseMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo,  INT8 bLevel )
{
	gpTempSoldier = pSoldier;
		
	wcscpy( gzUserDefinedButton[0], TacticalStr[ DECAPITATE_STR ] );
	wcscpy( gzUserDefinedButton[1], TacticalStr[ GUT_STR ] );
	wcscpy( gzUserDefinedButton[2], TacticalStr[ TAKE_CLOTHES_STR ] );
	wcscpy( gzUserDefinedButton[3], TacticalStr[ TAKE_BODY_STR ] );
	DoMessageBox( MSG_BOX_BASIC_MEDIUM_BUTTONS, TacticalStr[ CORPSE_SELECTION_STR ], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS, CorpseMessageBoxCallBack, NULL );
}

void StartBombMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo )
{
	//DBrot: More Rooms
	UINT16 usRoom;

	// sevenfm: cannot arm already armed bomb (for example, in inventory)
	if( pSoldier->inv[HANDPOS].fFlags & OBJECT_ARMED_BOMB )
	{
		DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ ARM_MESSAGE_ALREADY_ARMED ] , GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
		return;
	}

	gpTempSoldier = pSoldier;
	gsTempGridNo = sGridNo;
	if (Item[ pSoldier->inv[HANDPOS].usItem].remotetrigger )
	{
		wcscpy( gzUserDefinedButton[0], L"1" );
		wcscpy( gzUserDefinedButton[1], L"2" );
		wcscpy( gzUserDefinedButton[2], L"3" );
		wcscpy( gzUserDefinedButton[3], L"4" );
		wcscpy( gzUserDefinedButton[4], L"A" );
		wcscpy( gzUserDefinedButton[5], L"B" );
		wcscpy( gzUserDefinedButton[6], L"C" );
		wcscpy( gzUserDefinedButton[7], L"D" );

		DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_BOMB_OR_DEFUSE_FREQUENCY_STR ], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS, BombMessageBoxCallBack, NULL );
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
			if ( InARoom( pSoldier->sGridNo, &usRoom ) && usRoom == 4 )
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
	else if ( HasAttachmentOfClass( &(pSoldier->inv[HANDPOS]), AC_DEFUSE ) )
	{
		wcscpy( gzUserDefinedButton[0], L"1-A" );
		wcscpy( gzUserDefinedButton[1], L"1-B" );
		wcscpy( gzUserDefinedButton[2], L"1-C" );
		wcscpy( gzUserDefinedButton[3], L"1-D" );
		wcscpy( gzUserDefinedButton[4], L"2-A" );
		wcscpy( gzUserDefinedButton[5], L"2-B" );
		wcscpy( gzUserDefinedButton[6], L"2-C" );
		wcscpy( gzUserDefinedButton[7], L"2-D" );
		wcscpy( gzUserDefinedButton[8], L"3-A" );
		wcscpy( gzUserDefinedButton[9], L"3-B" );
		wcscpy( gzUserDefinedButton[10], L"3-C" );
		wcscpy( gzUserDefinedButton[11], L"3-D" );
		wcscpy( gzUserDefinedButton[12], L"4-A" );
		wcscpy( gzUserDefinedButton[13], L"4-B" );
		wcscpy( gzUserDefinedButton[14], L"4-C" );
		wcscpy( gzUserDefinedButton[15], L"4-D" );

       // sevenfm: zero out color values
       for( INT32 cnt = 0; cnt< NUM_CUSTOM_BUTTONS; cnt++)
       	gzUserDefinedButtonColor[cnt] = 0;

		if ( HasAttachmentOfClass( &(pSoldier->inv[ HANDPOS ] ), (AC_DETONATOR ) ) )
		{
			DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_DETONATE_AND_REMOTE_DEFUSE_FREQUENCY_STR ], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS, BombMessageBoxCallBack, NULL );
		}
		else if ( HasAttachmentOfClass( &(pSoldier->inv[ HANDPOS ] ), (AC_REMOTEDET) ) )
		{
			DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS,  TacticalStr[ CHOOSE_REMOTE_DETONATE_AND_REMOTE_DEFUSE_FREQUENCY_STR ], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS, BombMessageBoxCallBack, NULL );
		}
		else
		{
			// sevenfm: do not allow arming bombs with only REMOTE_DET attached
//			DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_REMOTE_DEFUSE_FREQUENCY_STR ], GAME_SCREEN, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS, BombMessageBoxCallBack, NULL );
			DoMessageBox( MSG_BOX_BASIC_STYLE, TacticalStr[ ARM_MESSAGE_NO_DETONATOR ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_OK, NULL, NULL );
		}
	}
	else if ( HasAttachmentOfClass( &(pSoldier->inv[ HANDPOS ] ), (AC_DETONATOR ) )	)
	{
		DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_TIMER_STR ], GAME_SCREEN, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS, BombMessageBoxCallBack, NULL );
	}
	else if ( HasAttachmentOfClass( &(pSoldier->inv[ HANDPOS ] ), (AC_REMOTEDET) ) )
	{
		DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_REMOTE_FREQUENCY_STR ], GAME_SCREEN, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS, BombMessageBoxCallBack, NULL );
	}
	else if ( Item[ (&(pSoldier->inv[HANDPOS]))->usItem ].tripwire == 1 )
	{
		wcscpy( gzUserDefinedButton[0], L"1-A" );
		wcscpy( gzUserDefinedButton[1], L"1-B" );
		wcscpy( gzUserDefinedButton[2], L"1-C" );
		wcscpy( gzUserDefinedButton[3], L"1-D" );
		wcscpy( gzUserDefinedButton[4], L"2-A" );
		wcscpy( gzUserDefinedButton[5], L"2-B" );
		wcscpy( gzUserDefinedButton[6], L"2-C" );
		wcscpy( gzUserDefinedButton[7], L"2-D" );
		wcscpy( gzUserDefinedButton[8], L"3-A" );
		wcscpy( gzUserDefinedButton[9], L"3-B" );
		wcscpy( gzUserDefinedButton[10], L"3-C" );
		wcscpy( gzUserDefinedButton[11], L"3-D" );
		wcscpy( gzUserDefinedButton[12], L"4-A" );
		wcscpy( gzUserDefinedButton[13], L"4-B" );
		wcscpy( gzUserDefinedButton[14], L"4-C" );
		wcscpy( gzUserDefinedButton[15], L"4-D" );

       // sevenfm: zero out color values
       for( INT32 cnt = 0; cnt< NUM_CUSTOM_BUTTONS; cnt++)
           gzUserDefinedButtonColor[cnt] = 0;
		
		// sevenfm: if SHIFT is pressed - plant tripwire with last network settings
		if( gfShiftBombPlant && gubLastTripwire > 0 )
			BombMessageBoxCallBack(gubLastTripwire);
		else
			DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, TacticalStr[ CHOOSE_TRIPWIRE_NETWORK ], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_SIXTEEN_BUTTONS, BombMessageBoxCallBack, NULL );
	}
}

// Flugente
void StartTacticalFunctionSelectionMessageBox( SOLDIERTYPE * pSoldier, INT32 sGridNo,  INT8 bLevel )
{
	if ( !pSoldier )
		return;

	gpTempSoldier = pSoldier;
	gsTempGridNo = sGridNo;

    // sevenfm: reorganized buttons order for new dialog
	// fill canteens
	wcscpy( gzUserDefinedButton[0], TacticalStr[FILL_CANTEEN_STR] );

	// remove covert property/clothes
	if ( gpTempSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER) )
		wcscpy( gzUserDefinedButton[1], TacticalStr[TAKE_OFF_DISGUISE_STR] );
	else
		wcscpy( gzUserDefinedButton[1], TacticalStr[TAKE_OFF_CLOTHES_STR] );

	// clean weapons - in realtime of the entire team, in turnbased only for the selected merc
//	wcscpy( gzUserDefinedButton[2], (gTacticalStatus.uiFlags & INCOMBAT) ? TacticalStr[CLEAN_ONE_GUN_STR] : TacticalStr[CLEAN_ALL_GUNS_STR] );
	// silversurfer: instant cleaning is not used anymore. Guns are cleaned via repair items assignment.
	wcscpy( gzUserDefinedButton[2], TacticalStr[UNUSED_STR] );

	// improve gear
	wcscpy( gzUserDefinedButton[3], TacticalStr[IMPROVEGEARBUTTON_STR] );

	// order militia to drop/pick up gear
	if ( gGameExternalOptions.fMilitiaUseSectorInventory )
	{
		wcscpy( gzUserDefinedButton[4], TacticalStr[MILITIA_DROP_EQ_STR] );
		wcscpy( gzUserDefinedButton[5], TacticalStr[MILITIA_PICK_UP_EQ_STR] );
	}
	else
	{
		wcscpy( gzUserDefinedButton[4], TacticalStr[UNUSED_STR] );
		wcscpy( gzUserDefinedButton[5], TacticalStr[UNUSED_STR] );
	}

	// if disguised, allow testing our disguise
	if ( gpTempSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER) )
		wcscpy( gzUserDefinedButton[6], TacticalStr[SPY_SELFTEST_STR] );
	else
		wcscpy( gzUserDefinedButton[6], TacticalStr[UNUSED_STR] );

	wcscpy( gzUserDefinedButton[7], TacticalStr[ UNUSED_STR ] );

	DoMessageBox( MSG_BOX_BASIC_MEDIUM_BUTTONS, TacticalStr[ FUNCTION_SELECTION_STR ], GAME_SCREEN, MSG_BOX_FLAG_GENERIC_EIGHT_BUTTONS, TacticalFunctionSelectionMessageBoxCallBack, NULL, MSG_BOX_DEFAULT_BUTTON_1 );
}

// Flugente: on a key press, we loop over each team members' inventory and exchange it with world items that have a higher status
// however, we ignore items that have inseparable attachments
// the goal is to simulate the player manually changing items, as that is rather tedious
OBJECTTYPE* GetBetterSectorObject( UINT16 usItem, INT16 status, UINT8& arIndex )
{
	OBJECTTYPE* pBestObj = NULL;

	INT16	beststatus = status;
	UINT32	slot = 0;
	BOOLEAN	found = FALSE;

	for ( UINT32 uiCount = 0; uiCount < guiNumWorldItems; ++uiCount )				// ... for all items in the world ...
	{
		if ( gWorldItems[uiCount].fExists && gWorldItems[uiCount].usFlags & WORLD_ITEM_REACHABLE &&
			 !(gWorldItems[uiCount].usFlags & WORLD_ITEM_ARMED_BOMB) && gWorldItems[uiCount].bVisible == VISIBLE && 
			 gWorldItems[uiCount].object.usItem == usItem )
		{			
			OBJECTTYPE* pObj = &(gWorldItems[uiCount].object);
			
			if ( pObj != NULL && pObj->exists() )
			{					
				for ( UINT8 i = 0; i < pObj->ubNumberOfObjects; ++i )
				{
					if ( (*pObj)[i]->data.objectStatus > beststatus )
					{
						// ignore if there are inseparable attachments - we wouldn't be able to change those by hand
						attachmentList::iterator iterend = (*pObj)[i]->attachments.end( );
						for ( attachmentList::iterator iter = (*pObj)[i]->attachments.begin( ); iter != iterend; ++iter )
						{
							if ( iter->exists() && Item[iter->usItem].inseparable )
							{
								continue;
							}
						}

						beststatus = (*pObj)[i]->data.objectStatus;
						slot = uiCount;
						arIndex = i;
						found = TRUE;
					}
				}
			}
		}
	}

	if ( found )
	{
		OBJECTTYPE* pObj = &(gWorldItems[slot].object);

		return pObj;
	}

	return NULL;
}

void UpdateGear()
{
	if ( gTacticalStatus.uiFlags & INCOMBAT )
		return;

	// no functionality if not in tactical or in combat, or nobody is here
	if ( (guiCurrentScreen != GAME_SCREEN && guiCurrentScreen != MSG_BOX_SCREEN) )
		return;
		
	UINT16									bMercID, bLastTeamID;
	SOLDIERTYPE*							pSoldier = NULL;

	bMercID = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	bLastTeamID = gTacticalStatus.Team[gbPlayerNum].bLastID;

	// loop through all mercs
	for ( pSoldier = MercPtrs[bMercID]; bMercID <= bLastTeamID; ++bMercID, ++pSoldier )
	{
		//if the merc is in this sector
		if ( pSoldier->bActive && pSoldier->bInSector && (pSoldier->sSectorX == gWorldSectorX) && (pSoldier->sSectorY == gWorldSectorY) && (pSoldier->bSectorZ == gbWorldSectorZ) )
		{
			// loop over inventory
			INT8 invsize = (INT8)pSoldier->inv.size( );									// remember inventorysize, so we don't call size() repeatedly

			for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )								// ... for all items in our inventory ...
			{
				if ( pSoldier->inv[bLoop].exists( ) )
				{
					OBJECTTYPE* pObj = &(pSoldier->inv[bLoop]);							// ... get pointer for this item ...

					if ( pObj != NULL )													// ... if pointer is not obviously useless ...
					{
						// before replacing it, merge ammo in stacks, the not-full mag being the visible one
						// by doing this we won't have so many used magazines lying around
						if ( (Item[pObj->usItem].usItemClass & IC_AMMO) && pObj->ubNumberOfObjects > 1 )
						{
							UINT32 ammocount = 0;
							for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
								ammocount += (*pObj)[i]->data.ubShotsLeft;

							UINT16 usMagIndex = Item[pObj->usItem].ubClassIndex;
							UINT16 magsize = Magazine[usMagIndex].ubMagSize;

							if ( magsize )
							{
								UINT8 fullmags = ammocount / magsize;

								UINT16 leftover = ammocount - fullmags * magsize;

								UINT8 totalmags = fullmags + (leftover > 0 ? 1 : 0);

								if ( totalmags < pObj->ubNumberOfObjects )
									pObj->RemoveObjectsFromStack( pObj->ubNumberOfObjects - totalmags );
								
								for ( INT16 i = 0; i < totalmags; ++i )
									(*pObj)[i]->data.ubShotsLeft = magsize;

								if ( leftover > 0 )
									(*pObj)[0]->data.ubShotsLeft = leftover;
							}
						}
						
						for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )				// ... there might be multiple items here (item stack), so for each one ...
						{
							// we could improve our gear by changing this object with another one
							if ( (*pObj)[i]->data.objectStatus < 100 )
							{
								// ignore if there are inseparable attachments - we wouldn't be able to change those by hand
								attachmentList::iterator iterend = (*pObj)[i]->attachments.end( );
								for ( attachmentList::iterator iter = (*pObj)[i]->attachments.begin( ); iter != iterend; ++iter )
								{
									if ( iter->exists( ) && Item[iter->usItem].inseparable )
									{
										continue;
									}
								}

								// see if we can find a better object in this sector
								UINT8 index = 0;
								OBJECTTYPE* pObj_Better = GetBetterSectorObject( pObj->usItem, (*pObj)[i]->data.objectStatus, index );

								if ( pObj_Better )
								{
									// WARNING! The correct way of doing this would be to exchange both objects
									// we are not doing this here on purpose, as we want to save time
									// instead, we switch status and a few other properties
									// Only do this if you know what you are doing - in the wrong location, we could do odd things to global pointers. Consider yourself warned!

									// tmp data
									INT16	objectStatus = (*pObj)[i]->data.objectStatus;
									INT8	bTrap = (*pObj)[i]->data.bTrap;
									UINT8	fUsed = (*pObj)[i]->data.fUsed;
									UINT8	ubImprintID = (*pObj)[i]->data.ubImprintID;
									FLOAT	bTemperature = (*pObj)[i]->data.bTemperature;
									UINT8	ubDirection = (*pObj)[i]->data.ubDirection;
									UINT32	ubWireNetworkFlag = (*pObj)[i]->data.ubWireNetworkFlag;
									INT8	bDefuseFrequency = (*pObj)[i]->data.bDefuseFrequency;
									INT16	sRepairThreshold = (*pObj)[i]->data.sRepairThreshold;
									UINT64	sObjectFlag = (*pObj)[i]->data.sObjectFlag;

									// set data on our object
									(*pObj)[i]->data.objectStatus = (*pObj_Better)[index]->data.objectStatus;
									(*pObj)[i]->data.bTrap = (*pObj_Better)[index]->data.bTrap;
									(*pObj)[i]->data.fUsed = (*pObj_Better)[index]->data.fUsed;
									(*pObj)[i]->data.ubImprintID = (*pObj_Better)[index]->data.ubImprintID;
									(*pObj)[i]->data.bTemperature = (*pObj_Better)[index]->data.bTemperature;
									(*pObj)[i]->data.ubDirection = (*pObj_Better)[index]->data.ubDirection;
									(*pObj)[i]->data.ubWireNetworkFlag = (*pObj_Better)[index]->data.ubWireNetworkFlag;
									(*pObj)[i]->data.bDefuseFrequency = (*pObj_Better)[index]->data.bDefuseFrequency;
									(*pObj)[i]->data.sRepairThreshold = (*pObj_Better)[index]->data.sRepairThreshold;
									(*pObj)[i]->data.sObjectFlag = (*pObj_Better)[index]->data.sObjectFlag;

									// set data on world object
									(*pObj_Better)[index]->data.objectStatus = objectStatus;
									(*pObj_Better)[index]->data.bTrap = bTrap;
									(*pObj_Better)[index]->data.fUsed = fUsed;
									(*pObj_Better)[index]->data.ubImprintID = ubImprintID;
									(*pObj_Better)[index]->data.bTemperature = bTemperature;
									(*pObj_Better)[index]->data.ubDirection = ubDirection;
									(*pObj_Better)[index]->data.ubWireNetworkFlag = ubWireNetworkFlag;
									(*pObj_Better)[index]->data.bDefuseFrequency = bDefuseFrequency;
									(*pObj_Better)[index]->data.sRepairThreshold = sRepairThreshold;
									(*pObj_Better)[index]->data.sObjectFlag = sObjectFlag;

									ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[IMPROVEGEARDESCRIBE_STR], pSoldier->GetName( ), Item[pObj->usItem].szItemName );
								}
							}
						}

						// if this is an ammo stack that could use more mags, try to fill it up
						if ( (Item[pObj->usItem].usItemClass & IC_AMMO) )
						{
							UINT8 ubSlotLimit = ItemSlotLimit( pObj, bLoop, pSoldier, FALSE );
							UINT16 usMagIndex = Item[pObj->usItem].ubClassIndex;
							UINT16 magsize = Magazine[usMagIndex].ubMagSize; 
							
							if ( magsize )
							{
								while ( pObj->ubNumberOfObjects < ubSlotLimit )
								{
									int add = ubSlotLimit - pObj->ubNumberOfObjects;

									UINT8 index = 0;
									OBJECTTYPE* pObj_Better = GetBetterSectorObject( pObj->usItem, 0, index );

									if ( pObj_Better )
									{
										pObj->AddObjectsToStack( *pObj_Better, add );
									
										ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[IMPROVEGEARPICKUPMAG_STR], pSoldier->GetName( ), Item[pObj->usItem].szItemName );

										// re-merge the magazines again, in case we picked up not full magazines
										UINT32 ammocount = 0;
										for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
											ammocount += (*pObj)[i]->data.ubShotsLeft;

										UINT8 fullmags = ammocount / magsize;

										UINT16 leftover = ammocount - fullmags * magsize;

										UINT8 totalmags = fullmags + (leftover > 0 ? 1 : 0);

										if ( totalmags < pObj->ubNumberOfObjects )
											pObj->RemoveObjectsFromStack( pObj->ubNumberOfObjects - totalmags );

										for ( INT16 i = 0; i < totalmags; ++i )
											(*pObj)[i]->data.ubShotsLeft = magsize;

										if ( leftover > 0 )
											(*pObj)[0]->data.ubShotsLeft = leftover;
									}
									else
										break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void BombMessageBoxCallBack( UINT8 ubExitValue )
{
	if (gpTempSoldier)
	{
		// sevenfm: remember last tripwire network settings
		if(Item[ gpTempSoldier->inv[HANDPOS].usItem ].tripwire )
			gubLastTripwire = ubExitValue;

		if (Item[ gpTempSoldier->inv[HANDPOS].usItem ].remotetrigger )
		{
			// Flugente: jamming can prevent bomb activation
			if ( !gSkillTraitValues.fVOJammingBlocksRemoteBombs || !SectorJammed() )
				SetOffBombsByFrequency( gpTempSoldier->ubID, ubExitValue );
		}
		else
		{
			INT32 iResult;
			
			if ( HasAttachmentOfClass( &(gpTempSoldier->inv[HANDPOS]), AC_REMOTEDET ) )
			{
				iResult = SkillCheck( gpTempSoldier, PLANTING_REMOTE_BOMB_CHECK, 0 );
			}
			else  if ( HasItemFlag( (gpTempSoldier->inv[HANDPOS]).usItem, BEARTRAP ) )
			{
				iResult = SkillCheck( gpTempSoldier, PLANTING_MECHANICAL_BOMB_CHECK, 0 );
			}
			else
			{
				iResult = SkillCheck( gpTempSoldier, PLANTING_BOMB_CHECK, 0 );
			}

			if ( iResult >= 0 )
			{
				// Less explosives gain for placing tripwire
				if ( Item[ gpTempSoldier->inv[ HANDPOS ].usItem ].tripwire == 1 )
					StatChange( gpTempSoldier, EXPLODEAMT, 5, FALSE );
				else if ( HasItemFlag( (gpTempSoldier->inv[HANDPOS]).usItem, BEARTRAP ) )
					StatChange( gpTempSoldier, MECHANAMT, 10, FALSE );
				else
					// EXPLOSIVES GAIN (25):	Place a bomb, or buried and armed a mine
					StatChange( gpTempSoldier, EXPLODEAMT, 25, FALSE );

				// Flugente: additional dialogue
				AdditionalTacticalCharacterDialogue_CallsLua( gpTempSoldier, ADE_BOMB_HAS_BEEN_PLANTED, gpTempSoldier->inv[HANDPOS].usItem, 1 );
			}
			else
			{
				// Flugente: additional dialogue
				AdditionalTacticalCharacterDialogue_CallsLua( gpTempSoldier, ADE_BOMB_HAS_BEEN_PLANTED, gpTempSoldier->inv[HANDPOS].usItem, 0 );

				// beartraps don't explode...
				if ( HasItemFlag( (gpTempSoldier->inv[HANDPOS]).usItem, BEARTRAP ) )
					return;

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
					// we can't blow up tripwire, no matter how bad we fail
					if ( Item[ gpTempSoldier->inv[ HANDPOS ].usItem ].tripwire != 1 )
					{
						// OOPS! ... BOOM!
						IgniteExplosion( NOBODY, gpTempSoldier->sX, gpTempSoldier->sY, (INT16) (gpWorldLevelData[gpTempSoldier->sGridNo].sHeight), gpTempSoldier->sGridNo, gpTempSoldier->inv[ HANDPOS ].usItem, gpTempSoldier->pathing.bLevel, gpTempSoldier->ubDirection, &gpTempSoldier->inv[ HANDPOS ] );
					}

					return;
				}
			}

			// Flugente: tripwire rolls are not planted - instead we spawn tripwire and plant that
			OBJECTTYPE* pObj = &(gpTempSoldier->inv[HANDPOS]);
			if ( HasItemFlag( gpTempSoldier->inv[HANDPOS].usItem, TRIPWIREROLL ) && Item[ gpTempSoldier->inv[ HANDPOS ].usItem ].usBuddyItem != NOTHING )
			{
				(*pObj)[0]->data.objectStatus--;

				if ( !(*pObj)[0]->data.objectStatus )
				{
					// Delete object
					DeleteObj( pObj );

					// dirty interface panel
					DirtyMercPanelInterface( gpTempSoldier, DIRTYLEVEL2 );
				}

				CreateItem( Item[ gpTempSoldier->inv[ HANDPOS ].usItem ].usBuddyItem, 100, &gTempObject );

				pObj = &gTempObject;
			}

			if ( ArmBomb( pObj, ubExitValue ) )
			{
				// SANDRO - STOMP traits - Demolitions bonus to trap level
				if ( gGameOptions.fNewTraitSystem && HAS_SKILL_TRAIT( gpTempSoldier, DEMOLITIONS_NT ))
				{
					// increase trap level for Demolitions trait
					(*pObj)[0]->data.bTrap = __min( max( 10, (8 + gSkillTraitValues.ubDEPlacedBombLevelBonus)), (( EffectiveExplosive( gpTempSoldier ) / 20) + (EffectiveExpLevel( gpTempSoldier ) / 3) + gSkillTraitValues.ubDEPlacedBombLevelBonus) );
				}
				else
				{
					(*pObj)[0]->data.bTrap = __min( 10, ( EffectiveExplosive( gpTempSoldier ) / 20) + (EffectiveExpLevel( gpTempSoldier ) / 3) );
				}

				// Flugente: backgrounds
				if ( gpTempSoldier->HasBackgroundFlag( BACKGROUND_TRAPLEVEL ) )
					(*pObj)[0]->data.bTrap++;
				
				// HACK IMMINENT!
				// value of 1 is stored in maps for SIDE of bomb owner... when we want to use IDs!
				// so we add 2 to all owner IDs passed through here and subtract 2 later
				if ( pObj != &(gpTempSoldier->inv[HANDPOS]) || gpTempSoldier->inv[HANDPOS].MoveThisObjectTo(gTempObject, 1) == 0) 
				{
					gTempObject[0]->data.misc.ubBombOwner = gpTempSoldier->ubID + 2;
					gTempObject[0]->data.ubDirection = gpTempSoldier->ubDirection;		// Flugente: direction of bomb is direction of soldier

					// Flugente: tripwire was called through a messagebox, but has to be buried nevertheless
					if ( Item[ (&gTempObject)->usItem ].tripwire == 1 )
					{
						AddItemToPool( gsTempGridNo, &gTempObject, BURIED, gpTempSoldier->pathing.bLevel, WORLD_ITEM_ARMED_BOMB, 0 );
						// sevenfm: set flag only if planting tripwire
						gpWorldLevelData[ gsTempGridNo ].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;
                       HandleTakeNewBombFromInventory(gpTempSoldier, &gTempObject);
						// sevenfm: change cursor back to action if successfully taken new bomb (also change mode back to action if using tripwire roll)
						if ( gfShiftBombPlant && gpTempSoldier->inv[ gpTempSoldier->ubAttackingHand ].exists() )
							guiPendingOverrideEvent = M_CHANGE_TO_ACTION;
					}
					else
						AddItemToPool( gsTempGridNo, &gTempObject, VISIBLE, gpTempSoldier->pathing.bLevel, WORLD_ITEM_ARMED_BOMB, 0 );
				}				
			}
		}
	}
}

// Flugente: callback after deciding what tactical function to use
void TacticalFunctionSelectionMessageBoxCallBack( UINT8 ubExitValue )
{
	if (gpTempSoldier)
	{
		INT32 nextGridNoinSight = NewGridNo( gpTempSoldier->sGridNo, DirectionInc( gpTempSoldier->ubDirection ) );
		INT8 level = gpTempSoldier->bTargetLevel;

		switch (ubExitValue)
		{
		case 1:
			SectorFillCanteens();
			break;
		case 2:
       		// undisguise or take off custom clothes 
			gpTempSoldier->Strip();
			break;
       case 3:
			// clean weapons - in realtime of the entire team, in turnbased only for the selected merc
		    //CleanWeapons( !(gTacticalStatus.uiFlags & INCOMBAT) );
			break;
       case 4:
		    UpdateGear();
			break;
		case 5:
			// militia drops all gear taken from sector inventory
			if ( gGameExternalOptions.fMilitiaUseSectorInventory )
				TeamDropAll( MILITIA_TEAM );
			break;
		case 6:
			// militia takes gear from sector inventory
			if ( gGameExternalOptions.fMilitiaUseSectorInventory )
				TeamRestock( MILITIA_TEAM );
			break;
		case 7:
			// test our disguise
			if (gpTempSoldier->usSoldierFlagMask & (SOLDIER_COVERT_CIV | SOLDIER_COVERT_SOLDIER))
				gpTempSoldier->SpySelfTest();
			break;

		case 8:
		default:
			break;
		}
	}
}

// Flugente: callback after deciding what to do with a corpse
void CorpseMessageBoxCallBack( UINT8 ubExitValue )
{
	if (gpTempSoldier)
	{
		INT32 nextGridNoinSight = gpTempSoldier->sGridNo;
		nextGridNoinSight = NewGridNo( nextGridNoinSight, DirectionInc( gpTempSoldier->ubDirection ) );

		INT8 level = gpTempSoldier->bTargetLevel;

		INT16 sAPCost = CalcTotalAPsToAttack( gpTempSoldier, nextGridNoinSight, FALSE, gpTempSoldier->aiData.bAimTime );

		BOOLEAN fDamageKnife = FALSE;

		switch (ubExitValue)
		{
		case 1:
			if ( DecapitateCorpse( gpTempSoldier, nextGridNoinSight, level ) )
				fDamageKnife = TRUE;

			DeductPoints( gpTempSoldier, sAPCost, 0 );			
			break;
		case 2:
			if ( GutCorpse( gpTempSoldier, nextGridNoinSight, level ) )
				fDamageKnife = TRUE;

			DeductPoints( gpTempSoldier, sAPCost, 0 );			
			break;
		case 3:
			StripCorpse( gpTempSoldier, nextGridNoinSight, level );
			DeductPoints( gpTempSoldier, sAPCost, 0 );
			break;
		case 4:
			TakeCorpse( gpTempSoldier, nextGridNoinSight, level );
			DeductPoints( gpTempSoldier, sAPCost, 0 );
			break;
		default:
			break;
		}

		// 35% chance to damage our knife a bit
		if ( fDamageKnife && Chance(35) )
		{
			if ( Item[ gpTempSoldier->inv[HANDPOS].usItem ].usItemClass & IC_BLADE )
			{
				gpTempSoldier->inv[HANDPOS][0]->data.objectStatus--;

				if ( Random(100) < Item[ gpTempSoldier->inv[HANDPOS].usItem ].usDamageChance )
				{
					gpTempSoldier->inv[HANDPOS][0]->data.sRepairThreshold--;
				}
			}
		}

		// we might get a disease from this...
		// if the corpse is already rotting, chance of infection is greatly increased
		ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( nextGridNoinSight, level );

		FLOAT modifier = GetCorpseRotFactor( pCorpse );

		HandlePossibleInfection( gpTempSoldier, NULL, INFECTION_TYPE_CONTACT_CORPSE, modifier );
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
	if ( Item[pObj->usItem].damageable && !Item[pObj->usItem].mine && (Item[pObj->usItem].usItemClass != IC_MEDKIT) && !Item[pObj->usItem].gascan && !IsStructureConstructItem( pObj->usItem, pSoldier->sGridNo, NULL ) )
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

				(*pObj)[0]->data.sRepairThreshold = min( (*pObj)[0]->data.objectStatus, (*pObj)[0]->data.sRepairThreshold - 2*Random(10) );
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
	INT8					bTrapDifficulty;
	INT16					bTrapDetectLevel;
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
		DoScreenIndependantMessageBox( TacticalStr[ DISARM_BOOBYTRAP_PROMPT ],	MSG_BOX_FLAG_YESNO, BoobyTrapInMapScreenMessageBoxCallBack );
	}
	else
	{
		DoScreenIndependantMessageBox( TacticalStr[ DISARM_BOOBYTRAP_PROMPT ],	MSG_BOX_FLAG_YESNO, BoobyTrapMessageBoxCallBack );
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

		if ( CheckBombDisarmChance() >= 0)
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
					OBJECTTYPE TempObject;
					CreateItem(gTempObject[0]->data.misc.usBombItem, gTempObject[0]->data.misc.bBombStatus, &TempObject);

					// also spawn attached guns/explosives
					if (gTempObject.usItem != ACTION_ITEM && (Item[gTempObject.usItem].tripwire || Item[gTempObject.usItem].usItemClass & IC_EXPLOSV))
					{
						// search for attached items
						OBJECTTYPE* pAttItem = NULL;

						// check all attachments
						attachmentList::iterator iterend = gTempObject[0]->attachments.end();
						for (attachmentList::iterator iter = gTempObject[0]->attachments.begin(); iter != iterend; ++iter)
						{
							//if ( iter->exists( ) && Item[iter->usItem].usItemClass == IC_GUN )
							if (iter->exists())
							{
								pAttItem = &(*iter);

								if (Item[iter->usItem].usItemClass == IC_GUN)
									AddItemToPool(gsBoobyTrapGridNo, pAttItem, 1, gbBoobyTrapLevel, 0, -1);
								else
									TempObject.AttachObject(NULL, pAttItem, FALSE);
							}
						}
					}

					gTempObject = TempObject;
					
					if (is_networked && is_client)
					{
						OBJECTTYPE TempAttachment;
						// attach whatever trigger the bomb originally had
						if (gWorldItems[ gpBoobyTrapItemPool->iItemIndex ].object[0]->data.misc.bDetonatorType == BOMB_REMOTE)
						{
							// attack a remote detonator, but they will need a trigger to use it :)
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

				// Flugente: set back all bomb parameters, otherwsie the bomb will still be armed in the inventory
				// set back ubWireNetworkFlag and bDefuseFrequency, but not the direction... bomb is still aimed, it is just turned off
				gTempObject.fFlags &= ~( OBJECT_ARMED_BOMB );
				gTempObject[0]->data.ubWireNetworkFlag = 0;
				gTempObject[0]->data.bDefuseFrequency = 0;
				gTempObject[0]->data.misc.bDetonatorType = 0;
				gTempObject[0]->data.misc.bDelay = 0;
				gTempObject[0]->data.misc.bFrequency = 0;
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
			gpBoobyTrapSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 ); 
			
			if ( HasItemFlag( gTempObject.usItem, BEARTRAP ) )
				return;

			StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (INT8)(3 * gbTrapDifficulty), FROM_FAILURE );			

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

		if ( HasItemFlag( gpItemPointer->usItem, BEARTRAP ) )
		{
			iCheckResult = SkillCheck( gpBoobyTrapSoldier, DISARM_MECHANICAL_TRAP_CHECK, 0 );
		}
		else
		{
			iCheckResult = SkillCheck( gpBoobyTrapSoldier, DISARM_TRAP_CHECK, 0 );
		}

		if (iCheckResult >= 0)
		{
			// disarmed a boobytrap!
			if ( HasItemFlag( gpItemPointer->usItem, BEARTRAP ) )
				StatChange( gpBoobyTrapSoldier, MECHANAMT, (3 * gbTrapDifficulty), FALSE );
			else
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
			gpBoobyTrapSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );

			// beartraps don't explode...
			if ( HasItemFlag( gpItemPointer->usItem, BEARTRAP ) )
				return;

			StatChange( gpBoobyTrapSoldier, EXPLODEAMT, (INT8) (3 * gbTrapDifficulty ), FROM_FAILURE );

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
	UINT16						ubDetectLevel;
	UINT8						ubDirection;
	MAP_ELEMENT *		pMapElement;
	UINT32					fCheckFlag;
	UINT32					uiWorldBombIndex;
	OBJECTTYPE *		pObj;
	BOOLEAN					fMining, fFoundMetal = FALSE;
	//	ITEM_POOL *			pItemPool;
	UINT8						ubMovementCost;
	INT8						bTrapBonus = 0;

	ubDetectLevel = 0;

	// Flugente: changed the way the metal detector works: now it detects mines only when it is used in hands, not if it is anywhere in the inventory.
	// Reason: with the new tripwire functionality and th new ability of a detector to spot mines several tiles away the detector s more useful.
	// However, the most likely persons to use it (explosive experts) are also those that plant those networks. They will often have a detector in their inventory.
	// If the detector just works if its in inventory, they autoflag the mines they are laying, which is bad. 
	// With this change, a detector must be in order to work.
	if ( FindMetalDetectorInHand( pSoldier ) != NO_SLOT )
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

	// sevenfm
	// pSoldier->aiData.bNeutral is needed to prevent neutral civs stepping on player's mines
	if (pSoldier->bSide == 0 || (pSoldier->aiData.bNeutral && gGameExternalOptions.bNeutralCiviliansAvoidPlayerMines))	
	{
		fCheckFlag = MAPELEMENT_PLAYER_MINE_PRESENT;
	}
	else
	{
		fCheckFlag = MAPELEMENT_ENEMY_MINE_PRESENT;
	}

	// anv: vehicles and passengers can't detect mines
	if ( pSoldier->flags.uiStatusFlags & ( SOLDIER_VEHICLE | SOLDIER_DRIVER | SOLDIER_PASSENGER ))	
	{
		return (FALSE);
	}

	// check every tile around gridno for the presence of "nasty stuff"
	for (ubDirection = 0; ubDirection < NUM_WORLD_DIRECTIONS; ++ubDirection)
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
			ubDirection = NUM_WORLD_DIRECTIONS; // don't loop
		}

		// if this sNextGridNo isn't out of bounds... but it never can be
		pMapElement = &(gpWorldLevelData[sNextGridNo]);

		if (pMapElement->uiFlags & fCheckFlag)
		{
			// already know there's a mine there
			// sevenfm
			// if we try to step on known (planted by player) mine we should consider it wrong
			if(!fCheckAroundGridNo)
			{
				*psProblemGridNo = sNextGridNo;
				return( TRUE );
			}else{
				continue;
			}
		}

		// check for boobytraps
		for (uiWorldBombIndex = 0; uiWorldBombIndex < guiNumWorldBombs; uiWorldBombIndex++)
		{
			if (gWorldBombs[uiWorldBombIndex].fExists && gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].sGridNo == sNextGridNo)
			{
				pObj = &( gWorldItems[ gWorldBombs[uiWorldBombIndex].iItemIndex ].object );
				// sevenfm: removed OBJECT_KNOWN_TO_BE_TRAPPED check as we don't want to step on bomb even if it's known bomb
				// if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_PRESSURE && !((*pObj).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED) && (!((*pObj).fFlags & OBJECT_DISABLED_BOMB)) )
				if ( (*pObj)[0]->data.misc.bDetonatorType == BOMB_PRESSURE && (!((*pObj).fFlags & OBJECT_DISABLED_BOMB)) )
				{
					// Flugente: some bombs cannot be found via metal detector
					if ( fMining && (*pObj)[0]->data.bTrap <= 20 && !( HasItemFlag(pObj->usItem, NO_METAL_DETECTION) || HasItemFlag((*pObj)[0]->data.misc.usBombItem, NO_METAL_DETECTION) ) )
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

							// sevenfm: we should stop only if trying to step on bomb or if we found new bomb				
							if(fCheckAroundGridNo && ( (*pObj).fFlags & OBJECT_KNOWN_TO_BE_TRAPPED ) )
								continue;

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
	BOOLEAN playerMine = FALSE;

	// ATE: REALLY IMPORTANT - ALL CALLBACK ITEMS SHOULD UNLOCK
	gTacticalStatus.fLockItemLocators = FALSE;

	// sevenfm: added check - if there is MAPELEMENT_PLAYER_MINE_PRESENT flag but there is no mine at tile
	// this should prevent crash in rare situations
	if( !GetItemPool( gsBoobyTrapGridNo, &pItemPool, gbBoobyTrapLevel ) || pItemPool == NULL || FindWorldItemForBuriedBombInGridNo( gsBoobyTrapGridNo, gbBoobyTrapLevel ) == -1 )
	{
		// remove blue flag and MINE_PRESENT flags
		RemoveBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
		return;
	}

	if(gpWorldLevelData[ pItemPool->sGridNo ].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT)
		playerMine = TRUE;

	// WDS - Automatically flag mines
	if (gGameExternalOptions.automaticallyFlagMines) {
		// play a locator at the location of the mine
		// sevenfm: only if it's not our mine
		if ( !playerMine )
			SetItemPoolLocator( pItemPool );

		AddBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
	} else {
		guiPendingOverrideEvent = LU_BEGINUILOCK;

		// play a locator at the location of the mine
		// sevenfm:	only if it's not our mine or we found new mine
		if( playerMine || pItemPool->usFlags & OBJECT_KNOWN_TO_BE_TRAPPED )
			MineSpottedLocatorCallback();
		else
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

	// player mine present
	if( gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT )
		RemoveMineFlagFromMapTempFile( sGridNo );

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
	DebugAI(AI_MSG_INFO, pSoldier, String("CancelAIAction: MakeNPCGrumpyForMinorOffense"));
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
	INT8					bTrapDifficulty;
	INT16					bTrapDetectLevel;

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
	UINT8 invsize = pOpponent->inv.size();
	for(i=0 ; i<invsize; ++i)
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
				// Flugente: if we are on the same team, allow guaranteed full access
				if ( AllowedToStealFromTeamMate(pSoldier->ubID, pOpponent->ubID) )
				{
					fStealItem = TRUE;

					// yuk, this is ugly... stealing items is counted as an attack. So what happens that if we steal from a teammember, the merc that we have stored in our targetD
					// (which will be the first merc hired when we land in Arulco) will react to our 'attack' and 'shoot back'.
					// so we now have 2 ways to resolve this issue:
					//	- remove the 'steal from teammember' stuff from the usual stealing stuff, and add it as a new action (like handcuffing), complete with building the steal-sub-menu and everything
					//	- or introduce a flag that prohibits teammembers from 'reaction-firing' on us. Set it upon stealing (here) and remove it after the steal menu is closed
					// or simplicity reasons, I will do #2 here. Until it breaks, then I'll be forced to do #1.
					pSoldier->usSoldierFlagMask |= SOLDIER_ACCESSTEAMMEMBER;

					// if we are Nails, don't allow taking our vest
					if ( pOpponent->ubProfile == 34 && i == VESTPOS )
						fStealItem = FALSE;
				}
				else
				{
					// Check, if we can steal the item
					// CHRISL: Added new case definitions for new inventory pockets
					switch (i)
					{
						case HANDPOS:
						case GUNSLINGPOCKPOS:
						{
							// Flugente: if item has a weapon sling attached, it can't be stolen
							if ( HasAttachmentOfClass(pObject, AC_SLING) )
								fStealItem = FALSE;
							else
								fStealItem = TRUE;

							break;
						}
						case SECONDHANDPOS:					
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
	// Aknowledge....
	if( pSoldier->bTeam == OUR_TEAM && !fDidSayCoolQuote )
	{
		pSoldier->DoMercBattleSound( BATTLE_SOUND_GOTIT );
	}

	// SANDRO - show a message, that we had insufficient APs to take all items
	if ( fNotEnoughAPs && pSoldier->bTeam == gbPlayerNum && gGameExternalOptions.fEnhancedCloseCombatSystem)
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_NOT_ENOUGH_APS_TO_STEAL_ALL], pSoldier->GetName() );
	}

	gpTempSoldier = pSoldier;
	gsTempGridNo = sGridNo;
	SetCustomizableTimerCallbackAndDelay( 1000, CheckForPickedOwnership, TRUE );
}

INT16 GetTileSetTindexToTileSetName( INT32 asTileSetId, std::string aTileSetName )
{
	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( INT16 iType = 0; iType < giNumberOfTileTypes; ++iType )
	{
		// if tileset is from the current tileset, check that
		if ( gTilesets[asTileSetId].TileSurfaceFilenames[iType][0] )
		{
			if ( !strcmp( aTileSetName.c_str( ), gTilesets[asTileSetId].TileSurfaceFilenames[iType] ) )
			{
				return iType;
			}
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[iType][0] )
		{
			if ( !strcmp( aTileSetName.c_str( ), gTilesets[0].TileSurfaceFilenames[iType] ) )
			{
				return iType;
			}
		}
	}

	return -1;
}

// can we construct a structure with this item?
BOOLEAN IsStructureConstructItem(UINT16 usItem, INT32 sGridNo, SOLDIERTYPE* pSoldier)
{
	if ( !usItem || TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	for ( UINT16 i = 0; i < STRUCTURE_CONSTRUCT_MAX; ++i )
	{
		if ( usItem == gStructureConstruct[i].usCreationItem )
			return TRUE;
	}

	return FALSE;
}

// can we remove a structure with this item?
BOOLEAN IsStructureDeconstructItem( UINT16 usItem, INT32 sGridNo, SOLDIERTYPE* pSoldier )
{
	if ( !usItem || TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	for ( UINT16 i = 0; i < STRUCTURE_DECONSTRUCT_MAX; ++i )
	{
		if ( usItem == gStructureDeconstruct[i].usDeconstructItem )
			return TRUE;
	}

	return FALSE;
}

BOOLEAN BuildFortification( INT32 sGridNo, SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj )
{	
	UINT32				fHeadType;
	UINT16				usUseIndex;
	INT16				sUseObjIndex = -1;
	INT32				iRandSelIndex = 1;
	BOOLEAN				fOkayToAdd;
	UINT8				ubDirection;

	// we need a valid soldier and a valid object
	if ( !pSoldier || !pObj )
		return FALSE;

	// no building on roofs
	if( gsInterfaceLevel > 0 )
		return FALSE;

	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;
	
	// don't build in water
	if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
		return FALSE;

	ubDirection = pSoldier->ubDirection;
	
	// we'll check wether this item is in our index of known creation items
	BOOLEAN indexfound = FALSE;
	INT16 structureconstructindex = -1;
	for (UINT16 i = 0; i < STRUCTURE_DECONSTRUCT_MAX; ++i)
	{
		if ( indexfound )
			break;

		if ( pObj->usItem == gStructureConstruct[i].usCreationItem )
		{
			switch ( ubDirection )
			{
			case NORTH:
				if ( !gStructureConstruct[i].northtilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].northtilevector[Random( gStructureConstruct[i].northtilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}
				break;
			case SOUTH:
				if ( !gStructureConstruct[i].southtilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].southtilevector[Random( gStructureConstruct[i].southtilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}
				break;
			case WEST:
				if ( !gStructureConstruct[i].westtilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].westtilevector[Random( gStructureConstruct[i].westtilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}
				break;
			case EAST:
				if ( !gStructureConstruct[i].easttilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].easttilevector[Random( gStructureConstruct[i].easttilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}
				break;

			default:
				// for all other directions, just pick one tile, it doesn not matter from which direction
				if ( !gStructureConstruct[i].northtilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].northtilevector[Random( gStructureConstruct[i].northtilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}

				if ( !indexfound && !gStructureConstruct[i].southtilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].southtilevector[Random( gStructureConstruct[i].southtilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}

				if ( !indexfound && !gStructureConstruct[i].westtilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].westtilevector[Random( gStructureConstruct[i].westtilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}

				if ( !indexfound && !gStructureConstruct[i].easttilevector.empty( ) )
				{
					usUseIndex = gStructureConstruct[i].easttilevector[Random( gStructureConstruct[i].easttilevector.size( ) )];
					structureconstructindex = i;
					indexfound = TRUE;
				}
				break;
			}
		}
	}

	if ( structureconstructindex < 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szMTATextStr[STR_MTA_CANNOT_BUILD] );
		return FALSE;
	}

	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
	{
		// if tileset is from the current tileset, check that
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], gStructureConstruct[structureconstructindex].szTileSetName, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[iType], gStructureConstruct[structureconstructindex].szTileSetName, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
	}

	if ( sUseObjIndex < 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szMTATextStr[ STR_MTA_CANNOT_BUILD ] );
		return FALSE;
	}

	// consumables are removed on creation
	if ( structureconstructindex >= 0 && gStructureConstruct[structureconstructindex].usItemStatusLoss )
	{
		if ( (*pObj)[0]->data.objectStatus <= gStructureConstruct[structureconstructindex].usItemStatusLoss )
		{
			UINT16 usItem = pObj->usItem;

			// Erase 'material' item from our hand - we 'use' it to build the structure
			DeleteObj( &(pSoldier->inv[HANDPOS]) );

			// sevenfm: auto-taking of items
			if ( !(gTacticalStatus.uiFlags & TURNBASED && gTacticalStatus.uiFlags & INCOMBAT) && gfShiftBombPlant )
			{
				pSoldier->TakeNewItemFromInventory( pObj->usItem );
			}
		}
		else
		{
			(*pObj)[0]->data.objectStatus -= gStructureConstruct[structureconstructindex].usItemStatusLoss;
		}
	}

	// TODO: if we create an earth pile, create a decal (signifiying the 'hole' we created at our position)

	// Check with Structure Database (aka ODB) if we can put the object here!
	fOkayToAdd = OkayToAddStructureToWorld( sGridNo, 0, gTileDatabase[(gTileTypeStartIndex[sUseObjIndex] + usUseIndex)].pDBStructureRef, INVALID_STRUCTURE_ID );
	if ( fOkayToAdd || (gTileDatabase[(gTileTypeStartIndex[sUseObjIndex] + usUseIndex)].pDBStructureRef == NULL) )
	{
		// Remove old graphic
		ApplyMapChangesToMapTempFile( TRUE );

		//dnl Remove existing structure before adding the same, seems to solve problem with stacking but still need test to be sure that is not removed something what should stay
		RemoveStruct( sGridNo, (UINT16)(gTileTypeStartIndex[sUseObjIndex] + usUseIndex) );//dnl
		// Actual structure info is added by the functions below
		AddStructToHead( sGridNo, (UINT16)(gTileTypeStartIndex[sUseObjIndex] + usUseIndex) );
		// For now, adjust to shadows by a hard-coded amount,

		// Add mask if in long grass
		GetLandHeadType( sGridNo, &fHeadType );

		RecompileLocalMovementCosts(sGridNo);

		// Turn off permanent changes....
		ApplyMapChangesToMapTempFile( FALSE );
		SetRenderFlags( RENDER_FLAG_FULL );

		return TRUE;
	}

	return FALSE;
}

BOOLEAN RemoveFortification( INT32 sGridNo, SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj )
{
	// we need a valid soldier and a valid object
	if ( !pSoldier || !pObj )
		return FALSE;

	// not on roofs
	if ( gsInterfaceLevel > 0 )
		return FALSE;

	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	STRUCTURE* pStruct = FindStructure( sGridNo, (STRUCTURE_GENERIC | STRUCTURE_WIREFENCE) );

	if ( pStruct != NULL )
	{
		// Get LEVELNODE for struct and remove!
		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode )
		{
			UINT16 usIndex = pNode->usIndex;
			UINT32 uiTileType = 0;
			if ( GetTileType( usIndex, &uiTileType ) )
			{
				for (UINT16 i = 0; i < STRUCTURE_DECONSTRUCT_MAX; ++i)
				{
					// item we have in our hand must be one with which we can deconstruct this
					if ( pObj->usItem == gStructureDeconstruct[i].usDeconstructItem )
					{
						// if tileset is from the current tileset, check that
						BOOLEAN found = FALSE;
						if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType][0] )
						{
							if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType], gStructureDeconstruct[i].szTileSetName, 11 ) )
								found = TRUE;
						}
						// otherwise, check first tileset (GENERIC 1)
						else if ( gTilesets[0].TileSurfaceFilenames[uiTileType][0] )
						{
							if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[uiTileType], gStructureDeconstruct[i].szTileSetName, 11 ) )
								found = TRUE;
						}

						if ( found )
						{
							// we have to check wether this specific structure can be removed. Just checking the tileset name won't be enough.
							// For example, we could have a set consisting of crates and piles of earth, which we want to remove via applying a shovel on it.
							// We only want to do this on the earth piles, of course - flattening crates via a shovel would be odd :-)
							UINT8 numbertofind = (UINT8)(pStruct->pDBStructureRef->pDBStructure->usStructureNumber);

							for ( size_t j = 0, size = gStructureDeconstruct[i].tilevector.size(); j < size; ++j)
							{
								if ( gStructureDeconstruct[i].tilevector[j] == numbertofind )
								{
									// Remove old graphic
									ApplyMapChangesToMapTempFile( TRUE );

									// if this is a wall, check wether the roof will collapse.
									// Yes, the player can damage himself by collapsing the roof of the house he is currently in. Such stupidity has to be punished.
									if ( pStruct->fFlags & STRUCTURE_WALL )
									{
										// this isn't an explosion, so the structural damage is moderate
										HandleRoofDestruction( sGridNo, 50 );
										HandleRoofDestruction( pSoldier->sGridNo, 50 );
									}

									RemoveStruct( sGridNo, pNode->usIndex );

									if ( !GridNoIndoors( sGridNo ) && gTileDatabase[ usIndex ].uiFlags & HAS_SHADOW_BUDDY && gTileDatabase[ usIndex ].sBuddyNum != -1 )
									{
										RemoveShadow( sGridNo, gTileDatabase[ usIndex ].sBuddyNum );
									}

									RecompileLocalMovementCosts(sGridNo);

									// Turn off permanent changes....
									ApplyMapChangesToMapTempFile( FALSE );
									SetRenderFlags( RENDER_FLAG_FULL );

									// eventually create item
									if ( gStructureDeconstruct[i].usItemToCreate && gStructureDeconstruct[i].usCreatedItemStatus )
									{
										// when creating the object, make it have at least 1% status
										CreateItem( gStructureDeconstruct[i].usItemToCreate, max(1, gStructureDeconstruct[i].usCreatedItemStatus), &gTempObject );

										AddItemToPool( sGridNo, &gTempObject, 1, 0, 0, -1 );
									}

									return TRUE;
								}
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}

UINT8	CheckBuildFortification( INT32 sGridNo, INT8 sLevel, UINT8 usIndex, UINT32 usStructureconstructindex )
{
	UINT16				usUseIndex;
	INT16				sUseObjIndex = -1;
	INT32				iRandSelIndex = 1;
	BOOLEAN				fOkayToAdd;
	
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return 1;

	// if we want to build on a roof, a roof is required
	if ( sLevel && !FlatRoofAboveGridNo( sGridNo ) )
		return 1;

	// don't build in water
	if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
		return 1;
			
	// we'll check wether this item is in our index of known creation items
	BOOLEAN indexfound = FALSE;
	INT16 structureconstructindex = 0;
	usUseIndex = usIndex;

	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
	{
		// if tileset is from the current tileset, check that
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], gStructureConstruct[usStructureconstructindex].szTileSetName, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[iType], gStructureConstruct[usStructureconstructindex].szTileSetName, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
	}

	if ( sUseObjIndex < 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szMTATextStr[STR_MTA_CANNOT_BUILD] );
		return 1;
	}

	// do not build into people
	if ( NOBODY != WhoIsThere2( sGridNo, sLevel ) )
		return 2;
	
	// Check with Structure Database (aka ODB) if we can put the object here!
	fOkayToAdd = OkayToAddStructureToWorld( sGridNo, sLevel, gTileDatabase[(gTileTypeStartIndex[sUseObjIndex] + usUseIndex)].pDBStructureRef, INVALID_STRUCTURE_ID );
	if ( fOkayToAdd || (gTileDatabase[(gTileTypeStartIndex[sUseObjIndex] + usUseIndex)].pDBStructureRef == NULL) )
	{
		return 0;
	}

	return 1;
}

BOOLEAN	BuildStructFromName( INT32 sGridNo, INT8 sLevel, const char* aStr, UINT8 usIndex )
{
	INT16				sUseObjIndex = -1;

	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	// if we want to build on a roof, a roof is required
	if ( sLevel && !FlatRoofAboveGridNo( sGridNo ) )
		return FALSE;

	// don't build in water
	if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
		return FALSE;

	// do not build into people
	if ( NOBODY != WhoIsThere2( sGridNo, sLevel ) )
		return FALSE;

	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
	{
		// if tileset is from the current tileset, check that
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], aStr, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[iType], aStr, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
	}

	if ( sUseObjIndex < 0 )
		return FALSE;

	// Check with Structure Database (aka ODB) if we can put the object here!
	BOOLEAN fOkayToAdd = OkayToAddStructureToWorld( sGridNo, sLevel, gTileDatabase[( gTileTypeStartIndex[sUseObjIndex] + usIndex )].pDBStructureRef, INVALID_STRUCTURE_ID );
	if ( fOkayToAdd || ( gTileDatabase[( gTileTypeStartIndex[sUseObjIndex] + usIndex )].pDBStructureRef == NULL ) )
	{
		// Remove old graphic
		ApplyMapChangesToMapTempFile( TRUE );

		//dnl Remove existing structure before adding the same, seems to solve problem with stacking but still need test to be sure that is not removed something what should stay
		// Actual structure info is added by the functions below
		if ( sLevel )
		{
			RemoveOnRoofStruct( sGridNo, (UINT16)( gTileTypeStartIndex[sUseObjIndex] + usIndex ) );

			AddOnRoofToTail( sGridNo, (UINT16)( gTileTypeStartIndex[sUseObjIndex] + usIndex ) );
		}
		else
		{
			RemoveStruct( sGridNo, (UINT16)( gTileTypeStartIndex[sUseObjIndex] + usIndex ) );

			AddStructToHead( sGridNo, (UINT16)( gTileTypeStartIndex[sUseObjIndex] + usIndex ) );
		}

		RecompileLocalMovementCosts( sGridNo );

		// Turn off permanent changes....
		ApplyMapChangesToMapTempFile( FALSE );
		SetRenderFlags( RENDER_FLAG_FULL );

		return TRUE;
	}

	return FALSE;
}

UINT16 gusTempDragBuildSoldierID = NOBODY;

BOOLEAN	BuildStructDrag( INT32 sGridNo, INT8 sLevel, UINT32 uiTileType, UINT8 usIndex, UINT16 usSoldierID )
{
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	// if we want to build on a roof, a roof is required
	if ( sLevel && !FlatRoofAboveGridNo( sGridNo ) )
		return FALSE;

	// don't build in water
	if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
		return FALSE;

	// do not build into people
	if ( NOBODY != WhoIsThere2( sGridNo, sLevel ) )
		return FALSE;
	
	if ( uiTileType < 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szMTATextStr[STR_MTA_CANNOT_BUILD] );
		return FALSE;
	}

	gusTempDragBuildSoldierID = usSoldierID;

	// Check with Structure Database (aka ODB) if we can put the object here!
	BOOLEAN fOkayToAdd = OkayToAddStructureToWorld( sGridNo, sLevel, gTileDatabase[( gTileTypeStartIndex[uiTileType] + usIndex )].pDBStructureRef, INVALID_STRUCTURE_ID );
	if ( fOkayToAdd || ( gTileDatabase[( gTileTypeStartIndex[uiTileType] + usIndex )].pDBStructureRef == NULL ) )
	{
		// Remove old graphic
		ApplyMapChangesToMapTempFile( TRUE );

		//dnl Remove existing structure before adding the same, seems to solve problem with stacking but still need test to be sure that is not removed something what should stay
		// Actual structure info is added by the functions below
		if ( sLevel )
		{
			RemoveOnRoofStruct( sGridNo, (UINT16)( gTileTypeStartIndex[uiTileType] + usIndex ) );

			AddOnRoofToTail( sGridNo, (UINT16)( gTileTypeStartIndex[uiTileType] + usIndex ) );
		}
		else
		{
			RemoveStruct( sGridNo, (UINT16)( gTileTypeStartIndex[uiTileType] + usIndex ) );

			AddStructToHead( sGridNo, (UINT16)( gTileTypeStartIndex[uiTileType] + usIndex ) );
		}

		RecompileLocalMovementCosts( sGridNo );

		// Turn off permanent changes....
		ApplyMapChangesToMapTempFile( FALSE );
		SetRenderFlags( RENDER_FLAG_FULL );

		gusTempDragBuildSoldierID = NOBODY;

		return TRUE;
	}

	gusTempDragBuildSoldierID = NOBODY;

	return FALSE;
}

BOOLEAN	BuildFortification( INT32 sGridNo, INT8 sLevel, UINT8 usIndex, UINT32 usStructureconstructindex )
{
	INT16				sUseObjIndex = -1;
	BOOLEAN				fOkayToAdd;

	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	// if we want to build on a roof, a roof is required
	if ( sLevel && !FlatRoofAboveGridNo( sGridNo ) )
		return FALSE;

	// don't build in water
	if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
		return FALSE;

	// do not build into people
	if ( NOBODY != WhoIsThere2( sGridNo, sLevel ) )
		return FALSE;
	
	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
	{
		// if tileset is from the current tileset, check that
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], gStructureConstruct[usStructureconstructindex].szTileSetName, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[iType][0] )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[iType], gStructureConstruct[usStructureconstructindex].szTileSetName, 10 ) )
			{
				sUseObjIndex = iType;
				break;
			}
		}
	}

	if ( sUseObjIndex < 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szMTATextStr[STR_MTA_CANNOT_BUILD] );
		return FALSE;
	}

	// Check with Structure Database (aka ODB) if we can put the object here!
	fOkayToAdd = OkayToAddStructureToWorld( sGridNo, sLevel, gTileDatabase[(gTileTypeStartIndex[sUseObjIndex] + usIndex)].pDBStructureRef, INVALID_STRUCTURE_ID );
	if ( fOkayToAdd || (gTileDatabase[(gTileTypeStartIndex[sUseObjIndex] + usIndex)].pDBStructureRef == NULL) )
	{
		// Remove old graphic
		ApplyMapChangesToMapTempFile( TRUE );

		//dnl Remove existing structure before adding the same, seems to solve problem with stacking but still need test to be sure that is not removed something what should stay
		// Actual structure info is added by the functions below
		if ( sLevel )
		{
			RemoveOnRoofStruct( sGridNo, (UINT16)(gTileTypeStartIndex[sUseObjIndex] + usIndex) );

			AddOnRoofToTail( sGridNo, (UINT16)(gTileTypeStartIndex[sUseObjIndex] + usIndex) );
		}
		else
		{
			RemoveStruct( sGridNo, (UINT16)(gTileTypeStartIndex[sUseObjIndex] + usIndex) );

			AddStructToHead( sGridNo, (UINT16)(gTileTypeStartIndex[sUseObjIndex] + usIndex) );
		}			

		RecompileLocalMovementCosts( sGridNo );

		// Turn off permanent changes....
		ApplyMapChangesToMapTempFile( FALSE );
		SetRenderFlags( RENDER_FLAG_FULL );

		return TRUE;
	}

	return FALSE;
}

BOOLEAN	CanRemoveFortification( INT32 sGridNo, INT8 sLevel, UINT32 usStructureconstructindex )
{
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;
	
	STRUCTURE* pStruct = GetTallestStructureOnGridno( sGridNo, sLevel );

	if ( pStruct != NULL )
	{
		// Get LEVELNODE for struct and remove!
		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode )
		{
			UINT16 usIndex = pNode->usIndex;
			UINT32 uiTileType = 0;
			if ( GetTileType( usIndex, &uiTileType ) )
			{
				// if tileset is from the current tileset, check that
				BOOLEAN found = FALSE;
				if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType][0] )
				{
					if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType], gStructureDeconstruct[usStructureconstructindex].szTileSetName, 11 ) )
						found = TRUE;
				}
				// otherwise, check first tileset (GENERIC 1)
				else if ( gTilesets[0].TileSurfaceFilenames[uiTileType][0] )
				{
					if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[uiTileType], gStructureDeconstruct[usStructureconstructindex].szTileSetName, 11 ) )
						found = TRUE;
				}

				if ( found )
				{
					// we have to check wether this specific structure can be removed. Just checking the tileset name won't be enough.
					// For example, we could have a set consisting of crates and piles of earth, which we want to remove via applying a shovel on it.
					// We only want to do this on the earth piles, of course - flattening crates via a shovel would be odd :-)
					UINT8 numbertofind = (UINT8)(pStruct->pDBStructureRef->pDBStructure->usStructureNumber);

					for ( size_t j = 0, size = gStructureDeconstruct[usStructureconstructindex].tilevector.size(); j < size; ++j )
					{
						if ( gStructureDeconstruct[usStructureconstructindex].tilevector[j] == numbertofind )
						{
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

BOOLEAN	IsDragStructurePresent( INT32 sGridNo, INT8 sLevel, UINT32& arusTileType, UINT16& arusStructureNumber, UINT8& arusHitpoints, UINT8& arusDecalFlags )
{
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	STRUCTURE* pStruct = GetTallestStructureOnGridnoDrag( sGridNo, sLevel );

	if ( pStruct != NULL )
	{
		// Get LEVELNODE for struct and remove!
		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode )
		{
			if ( GetTileType( pNode->usIndex, &arusTileType ) )
			{
				arusStructureNumber = pStruct->pDBStructureRef->pDBStructure->usStructureNumber;
				arusHitpoints = pStruct->ubHitPoints;
				arusDecalFlags = pStruct->ubDecalFlag;

				// if tileset is from the current tileset, check that
				for ( int i = 0; i < STRUCTURE_MOVEPOSSIBLE_MAX; ++i )
				{
					// if tileset is from the current tileset, check that
					bool found = FALSE;
					if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[arusTileType][0] )
					{
						if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[arusTileType], gStructureMovePossible[i].szTileSetName, 11 ) )
							found = true;
					}
					// otherwise, check first tileset (GENERIC 1)
					else if ( gTilesets[0].TileSurfaceFilenames[arusTileType][0] )
					{
						if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[arusTileType], gStructureMovePossible[i].szTileSetName, 11 ) )
							found = true;
					}

					if ( found )
					{
						for ( size_t j = 0, size = gStructureMovePossible[i].tilevector.size(); j < size; ++j )
						{
							if ( gStructureMovePossible[i].tilevector[j] == arusStructureNumber )
							{
								return TRUE;
							}
						}
					}
				}
			}
		}
	}

	return FALSE;
}

void	GetDragStructureXmlEntry( UINT32 ausTileType, UINT16 ausStructureNumber, int& arXmlVectorEntry )
{
	arXmlVectorEntry = -1;

	// if tileset is from the current tileset, check that
	for ( int i = 0; i < STRUCTURE_MOVEPOSSIBLE_MAX; ++i )
	{
		// if tileset is from the current tileset, check that
		bool found = FALSE;
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[ausTileType][0] )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[ausTileType], gStructureMovePossible[i].szTileSetName, 11 ) )
				found = true;
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[ausTileType][0] )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[ausTileType], gStructureMovePossible[i].szTileSetName, 11 ) )
				found = true;
		}

		if ( found )
		{
			for ( size_t j = 0, size = gStructureMovePossible[i].tilevector.size(); j < size; ++j )
			{
				if ( gStructureMovePossible[i].tilevector[j] == ausStructureNumber )
				{
					arXmlVectorEntry = i;

					return;
				}
			}
		}
	}
}

BOOLEAN	RemoveStructDrag( INT32 sGridNo, INT8 sLevel, UINT32 uiTileType )
{
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	STRUCTURE* pStruct = GetTallestStructureOnGridnoDrag( sGridNo, sLevel );

	if ( pStruct != NULL )
	{
		// Get LEVELNODE for struct and remove!
		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode )
		{
			UINT16 usIndex = pNode->usIndex;

			// Remove old graphic
			ApplyMapChangesToMapTempFile( TRUE );

			if ( sLevel )
			{
				RemoveOnRoofStruct( sGridNo, usIndex );
			}

			// if this is a wall, check wether the roof will collapse.
			// Yes, the player can damage himself by collapsing the roof of the house he is currently in. Such stupidity has to be punished.
			if ( pStruct->fFlags & STRUCTURE_WALL )
			{
				// this isn't an explosion, so the structural damage is moderate
				HandleRoofDestruction( sGridNo, 50 );
			}

			RemoveStruct( sGridNo, pNode->usIndex );

			if ( !GridNoIndoors( sGridNo ) && gTileDatabase[usIndex].uiFlags & HAS_SHADOW_BUDDY && gTileDatabase[usIndex].sBuddyNum != -1 )
			{
				RemoveShadow( sGridNo, gTileDatabase[usIndex].sBuddyNum );
			}

			RecompileLocalMovementCosts( sGridNo );

			// Turn off permanent changes....
			ApplyMapChangesToMapTempFile( FALSE );
			SetRenderFlags( RENDER_FLAG_FULL );

			return TRUE;
		}
	}

	return FALSE;
}

void CorrectDragStructData( INT32 sGridNo, INT8 sLevel, UINT8 ausHitpoints, UINT8 ausDecalFlags )
{
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return;

	STRUCTURE* pStruct = GetTallestStructureOnGridnoDrag( sGridNo, sLevel );

	if ( pStruct != NULL )
	{
		pStruct->ubHitPoints = ausHitpoints;
		pStruct->ubDecalFlag = ausDecalFlags;

		if ( pStruct->ubHitPoints < pStruct->pDBStructureRef->pDBStructure->ubHitPoints
			|| pStruct->ubDecalFlag & STRUCTURE_DECALFLAG_BLOOD )
		{
			gpWorldLevelData[sGridNo].uiFlags & MAPELEMENT_STRUCTURE_DAMAGED;

			//SetRenderFlags( RENDER_FLAG_FULL );
		}
	}
}

BOOLEAN	RemoveFortification( INT32 sGridNo, INT8 sLevel, UINT32 usStructureconstructindex )
{
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return FALSE;

	STRUCTURE* pStruct = GetTallestStructureOnGridno( sGridNo, sLevel );
	
	if ( pStruct != NULL )
	{
		// Get LEVELNODE for struct and remove!
		LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

		if ( pNode )
		{
			UINT16 usIndex = pNode->usIndex;
			UINT32 uiTileType = 0;
			if ( GetTileType( usIndex, &uiTileType ) )
			{	
				// if tileset is from the current tileset, check that
				BOOLEAN found = FALSE;
				if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType][0] )
				{
					if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[uiTileType], gStructureDeconstruct[usStructureconstructindex].szTileSetName, 11 ) )
						found = TRUE;
				}
				// otherwise, check first tileset (GENERIC 1)
				else if ( gTilesets[0].TileSurfaceFilenames[uiTileType][0] )
				{
					if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[uiTileType], gStructureDeconstruct[usStructureconstructindex].szTileSetName, 11 ) )
						found = TRUE;
				}

				if ( found )
				{
					// we have to check wether this specific structure can be removed. Just checking the tileset name won't be enough.
					// For example, we could have a set consisting of crates and piles of earth, which we want to remove via applying a shovel on it.
					// We only want to do this on the earth piles, of course - flattening crates via a shovel would be odd :-)
					UINT8 numbertofind = (UINT8)(pStruct->pDBStructureRef->pDBStructure->usStructureNumber);

					for ( size_t j = 0, size = gStructureDeconstruct[usStructureconstructindex].tilevector.size(); j < size; ++j )
					{
						if ( gStructureDeconstruct[usStructureconstructindex].tilevector[j] == numbertofind )
						{
							// Remove old graphic
							ApplyMapChangesToMapTempFile( TRUE );

							if ( sLevel )
							{
								RemoveOnRoofStruct( sGridNo, pNode->usIndex );
							}

							// if this is a wall, check wether the roof will collapse.
							// Yes, the player can damage himself by collapsing the roof of the house he is currently in. Such stupidity has to be punished.
							if ( pStruct->fFlags & STRUCTURE_WALL )
							{
								// this isn't an explosion, so the structural damage is moderate
								HandleRoofDestruction( sGridNo, 50 );
							}

							RemoveStruct( sGridNo, pNode->usIndex );

							if ( !GridNoIndoors( sGridNo ) && gTileDatabase[usIndex].uiFlags & HAS_SHADOW_BUDDY && gTileDatabase[usIndex].sBuddyNum != -1 )
							{
								RemoveShadow( sGridNo, gTileDatabase[usIndex].sBuddyNum );
							}

							RecompileLocalMovementCosts( sGridNo );

							// Turn off permanent changes....
							ApplyMapChangesToMapTempFile( FALSE );
							SetRenderFlags( RENDER_FLAG_FULL );
							
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

class FORTIFICATION_NODE
{
public:
	FORTIFICATION_NODE( INT32 a, INT8 b, BOOLEAN c, UINT32 d, UINT8 e ) :
		sGridNo( a ), sLevel( b ), fBuild( c ), structurexmlindex( d ), usIndex( e ) {}

	FORTIFICATION_NODE() :
		sGridNo( NOWHERE ), sLevel( 0 ), fBuild( TRUE ), structurexmlindex(0), usIndex( 0 ) {}

	void swap( FORTIFICATION_NODE& other )
	{
		std::swap( sGridNo, other.sGridNo );
		std::swap( sLevel, other.sLevel );
		std::swap( fBuild, other.fBuild );
		std::swap( structurexmlindex, other.structurexmlindex );
		std::swap( usIndex, other.usIndex );
	}

	INT32 sGridNo;
	INT8 sLevel;
	BOOLEAN fBuild;
	UINT32 structurexmlindex;
	UINT8 usIndex;
};

std::ostream& operator<<(std::ostream& stream, FORTIFICATION_NODE const& data)
{
	stream << data.sGridNo << " "
		<< (INT32)(data.sLevel) << " "
		<< (INT32)(data.fBuild) << " "
		<< (INT32)(data.structurexmlindex) << " "
		<< (INT32)(data.usIndex) << std::endl;

	return stream;
}

std::istream& operator>>(std::istream& stream, FORTIFICATION_NODE& data)
{
	FORTIFICATION_NODE     tmp;
	if ( stream >> tmp.sGridNo )
	{
		INT32 a, b, c, d;
		if ( stream >> a >> b >> c >> d )
		{
			tmp.sLevel = a;
			tmp.fBuild = (BOOLEAN)b;
			tmp.structurexmlindex = (UINT32)c;
			tmp.usIndex = (UINT8)d;

			data.swap( tmp );
		}
	}

	return stream;
}


typedef std::vector<FORTIFICATION_NODE> SectorFortificationVector;

std::map<UINT8, SectorFortificationVector> gSectorFortificationMap;

CHAR16 gCurrentSectorTileNamesChar16[STRUCTURE_CONSTRUCT_MAX][20];
CHAR16 gCurrentSectorTileIndexNamesChar16[STRUCTURE_CONSTRUCT_MAX][20];

std::vector<std::pair<INT16, STR16> > GetCurrentSectorTileSetVector()
{
	std::vector<std::pair<INT16, STR16> > vec;
	
	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
	{
		swprintf( gCurrentSectorTileNamesChar16[iType], L"" );

		// if tileset is from the current tileset, check that
		if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType][0] )
		{
			int nChars = MultiByteToWideChar( CP_ACP, 0, gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], -1, NULL, 0 );
			MultiByteToWideChar( CP_UTF8, 0, gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], -1, gCurrentSectorTileNamesChar16[iType], nChars );

			vec.push_back( std::make_pair( iType, gCurrentSectorTileNamesChar16[iType] ) );
		}
		// otherwise, check first tileset (GENERIC 1)
		else if ( gTilesets[0].TileSurfaceFilenames[iType][0] )
		{
			int nChars = MultiByteToWideChar( CP_ACP, 0, gTilesets[0].TileSurfaceFilenames[iType], -1, NULL, 0 );
			MultiByteToWideChar( CP_UTF8, 0, gTilesets[0].TileSurfaceFilenames[iType], -1, gCurrentSectorTileNamesChar16[iType], nChars );

			vec.push_back( std::make_pair( iType, gCurrentSectorTileNamesChar16[iType] ) );
		}
	}

	return vec;
}

// get a vector of all tilesets that are allowed to be built in this sector (the above filtered by structure construct/deconstruct basically)
std::vector<std::pair<INT16, STR16> > GetCurrentSectorAllowedFortificationTileSetVector( INT32 asTileSetId )
{
	std::vector<std::pair<INT16, STR16> > vec;
	std::set< std::pair<INT16, STR16> > pairset;

	// search wether structure exists in the current tilesets. If not, well, too bad
	for ( UINT16 i = 0; i < STRUCTURE_CONSTRUCT_MAX; ++i )
	{
		swprintf( gCurrentSectorTileNamesChar16[i], L"" );

		for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
		{
			// if tileset is from the current tileset, check that
			if ( gTilesets[asTileSetId].TileSurfaceFilenames[iType][0] )
			{
				if ( !_strnicmp( gTilesets[asTileSetId].TileSurfaceFilenames[iType], gStructureConstruct[i].szTileSetName, 11 ) )
				{
					int nChars = MultiByteToWideChar( CP_ACP, 0, gStructureConstruct[i].szTileSetDisplayName, -1, NULL, 0 );
					MultiByteToWideChar( CP_UTF8, 0, gStructureConstruct[i].szTileSetDisplayName, -1, gCurrentSectorTileNamesChar16[i], nChars );

					pairset.insert( std::make_pair( i, gCurrentSectorTileNamesChar16[i] ) );

					break;
				}
			}
			else
			{
				if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[iType], gStructureConstruct[i].szTileSetName, 11 ) )
				{
					int nChars = MultiByteToWideChar( CP_ACP, 0, gStructureConstruct[i].szTileSetDisplayName, -1, NULL, 0 );
					MultiByteToWideChar( CP_UTF8, 0, gStructureConstruct[i].szTileSetDisplayName, -1, gCurrentSectorTileNamesChar16[i], nChars );

					pairset.insert( std::make_pair( i, gCurrentSectorTileNamesChar16[i] ) );

					break;
				}
			}
		}
	}
	
	for ( std::set< std::pair<INT16, STR16> >::iterator it = pairset.begin( ); it != pairset.end( ); ++it )
	{
		vec.push_back( (*it) );
	}
		
	return vec;
}

// get all allowed indizes for a specific tileset. 'Allowed' as in: used in our structure construct entries
std::vector<std::pair<INT16, STR16> > GetTileSetIndexVector( INT16 aKey )
{
	std::vector<std::pair<INT16, STR16> > vec;

	if ( 0 <= aKey && aKey < STRUCTURE_CONSTRUCT_MAX )
	{
		std::set<UINT8> indexset;

		for ( INT16 j = 0; j < gStructureConstruct[aKey].northtilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aKey].northtilevector[j] );
		for ( INT16 j = 0; j < gStructureConstruct[aKey].southtilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aKey].southtilevector[j] );
		for ( INT16 j = 0; j < gStructureConstruct[aKey].easttilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aKey].easttilevector[j] );
		for ( INT16 j = 0; j < gStructureConstruct[aKey].westtilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aKey].westtilevector[j] );

		for ( std::set<UINT8>::iterator it = indexset.begin( ); it != indexset.end( ); ++it )
		{
			swprintf( gCurrentSectorTileIndexNamesChar16[(*it)], L"%d", (*it) );

			vec.push_back( std::make_pair( (*it), gCurrentSectorTileIndexNamesChar16[(*it)] ) );
		}
	}

	return vec;
}

std::string GetNameToTileSet( UINT8 aIndex )
{
	// if tileset is from the current tileset, check that
	if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aIndex][0] )
	{
		std::string str( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aIndex], std::find( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aIndex], gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aIndex] + 32, '\0' ) );

		return str;
	}
	// otherwise, check first tileset (GENERIC 1)
	else if ( gTilesets[0].TileSurfaceFilenames[aIndex][0] )
	{
		std::string str( gTilesets[0].TileSurfaceFilenames[aIndex], std::find( gTilesets[0].TileSurfaceFilenames[aIndex], gTilesets[0].TileSurfaceFilenames[aIndex] + 32, '\0' ) );

		return str;
	}

	return "";
}

INT16 GetStructureConstructIndexToTileset( INT16 aTileset )
{
	// find the structure construct entry that fits to this
	if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aTileset][0] )
	{
		for ( INT16 i = 0; i < STRUCTURE_CONSTRUCT_MAX; ++i )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aTileset], gStructureConstruct[i].szTileSetName, 11 ) )
			{
				return i;
			}
		}
	}
	// otherwise, check first tileset (GENERIC 1)
	else if ( gTilesets[0].TileSurfaceFilenames[aTileset][0] )
	{
		for ( INT16 i = 0; i < STRUCTURE_CONSTRUCT_MAX; ++i )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[aTileset], gStructureConstruct[i].szTileSetName, 11 ) )
			{
				return i;
			}
		}
	}

	return -1;
}

INT16 GetStructureDeConstructIndexToTileset( INT16 aTileset )
{
	// find the structure construct entry that fits to this
	if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aTileset][0] )
	{
		for ( INT16 i = 0; i < STRUCTURE_CONSTRUCT_MAX; ++i )
		{
			if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[aTileset], gStructureDeconstruct[i].szTileSetName, 11 ) )
			{
				return i;
			}
		}
	}
	// otherwise, check first tileset (GENERIC 1)
	else if ( gTilesets[0].TileSurfaceFilenames[aTileset][0] )
	{
		for ( INT16 i = 0; i < STRUCTURE_CONSTRUCT_MAX; ++i )
		{
			if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[aTileset], gStructureDeconstruct[i].szTileSetName, 11 ) )
			{
				return i;
			}
		}
	}

	return -1;
}

std::set<UINT8> GetStructureConstructDirectionIndizes( INT16 aEntry, BOOLEAN afNorth, BOOLEAN afEast, BOOLEAN afSouth, BOOLEAN afWest )
{
	std::set<UINT8> indexset;

	if ( aEntry >= 0 )
	{
		if ( afNorth )	for ( int j = 0; j < gStructureConstruct[aEntry].northtilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aEntry].northtilevector[j] );
		if ( afSouth )	for ( int j = 0; j < gStructureConstruct[aEntry].southtilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aEntry].southtilevector[j] );
		if ( afEast )	for ( int j = 0; j < gStructureConstruct[aEntry].easttilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aEntry].easttilevector[j] );
		if ( afWest )	for ( int j = 0; j < gStructureConstruct[aEntry].westtilevector.size( ); ++j )		indexset.insert( gStructureConstruct[aEntry].westtilevector[j] );
	}

	return indexset;
}

void AddFortificationPlanNode( INT32 sGridNo, INT8 sLevel, INT16 sFortificationStructure, UINT8 usFortificationTileLibraryIndex, BOOLEAN fAdd )
{
	if ( !gWorldSectorX || !gWorldSectorY )
		return;

	// no nonsense
	if ( sLevel < 0 || sLevel > 1 )
		return;

	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return;

	// if we want to build on a roof, a roof is required
	if ( sLevel && !FlatRoofAboveGridNo( sGridNo ) )
		return;

	// don't build in water
	if ( TERRAIN_IS_WATER( GetTerrainType( sGridNo ) ) )
		return;
	
	// we have to check whether what we are ordered to build can bere built here (this is necessary, as the selection menu can be avoided)
	BOOLEAN found = FALSE;

	if ( fAdd )
	{
		if ( sFortificationStructure >= 0 && sFortificationStructure < STRUCTURE_CONSTRUCT_MAX )
		{
			for ( INT32 iType = 0; iType < giNumberOfTileTypes; ++iType )
			{
				// if tileset is from the current tileset, check that
				if ( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType][0] )
				{
					if ( !_strnicmp( gTilesets[giCurrentTilesetID].TileSurfaceFilenames[iType], gStructureConstruct[sFortificationStructure].szTileSetName, 11 ) )
					{
						found = TRUE;

						break;
					}
				}
				else
				{
					if ( !_strnicmp( gTilesets[0].TileSurfaceFilenames[iType], gStructureConstruct[sFortificationStructure].szTileSetName, 11 ) )
					{
						found = TRUE;

						break;
					}
				}
			}
		}

		// check index as well
		if ( found )
		{
			std::vector<std::pair<INT16, STR16> > dropdownvector_2 = GetTileSetIndexVector( sFortificationStructure );

			for ( std::vector<std::pair<INT16, STR16> >::iterator it = dropdownvector_2.begin( ); it != dropdownvector_2.end(); ++it )
			{
				if ( (*it).first == usFortificationTileLibraryIndex )
				{
					found = TRUE;

					break;
				}
			}
		}
	}
	// if we try to add a 'remove structure' node, we determine the correct Deconstruct index (it would be annoying if the player has to always set that by themself)
	else
	{
		STRUCTURE* pStruct = GetTallestStructureOnGridno( sGridNo, sLevel );

		if ( pStruct != NULL )
		{
			// Get LEVELNODE for struct and remove!
			LEVELNODE* pNode = FindLevelNodeBasedOnStructure( pStruct->sGridNo, pStruct );

			if ( pNode )
			{
				UINT16 usIndex = pNode->usIndex;
				UINT32 uiTileType = 0;
				if ( GetTileType( usIndex, &uiTileType ) )
				{
					// as sFortificationStructure and usFortificationTileLibraryIndex are no references, we can reset them without any repercussions outside of this function
					sFortificationStructure = GetStructureDeConstructIndexToTileset( (INT16)uiTileType );

					if ( sFortificationStructure > -1 )
					{
						usFortificationTileLibraryIndex = (UINT8)(pStruct->pDBStructureRef->pDBStructure->usStructureNumber);

						UINT8 size = gStructureDeconstruct[sFortificationStructure].tilevector.size( );
						for ( UINT8 j = 0; j < size; ++j )
						{
							if ( gStructureDeconstruct[sFortificationStructure].tilevector[j] == usFortificationTileLibraryIndex )
							{
								found = TRUE;

								break;
							}
						}
					}
				}
			}
		}
	}

	if ( !found )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szFortificationText[0] );

		return;
	}
			
	FORTIFICATION_NODE node( sGridNo, sLevel, fAdd, sFortificationStructure, usFortificationTileLibraryIndex );

	UINT8 sector = SECTOR( gWorldSectorX, gWorldSectorY );

	if ( gSectorFortificationMap.find( sector ) != gSectorFortificationMap.end( ) )
	{
		// if a node already exists at this gridno, erase the old one. This allows to easily remove nodes
		SectorFortificationVector& vec = gSectorFortificationMap[sector];

		BOOLEAN alreadyexists = FALSE;
		SectorFortificationVector::iterator nodeitend = vec.end( );
		for ( SectorFortificationVector::iterator nodeit = vec.begin( ); nodeit != nodeitend; ++nodeit )
		{
			if ( node.sGridNo == (*nodeit).sGridNo && node.sLevel == (*nodeit).sLevel )
			{
				vec.erase( nodeit );

				alreadyexists = TRUE;
				break;
			}
		}

		if ( !alreadyexists )
		{
			// if specified, try to get adjacent nodes to match each other
			if ( fAdd && gStructureConstruct[node.structurexmlindex].fFortifyAdjacentAdjustment )
			{
				// check the adjacent nodes and alter the index fittingly (northern-southern vs eastern-western kind)		
				INT32 nextGridNoinSight = node.sGridNo;

				for ( int i = NORTH; i < NUM_WORLD_DIRECTIONS; i += 2 )
				{
					nextGridNoinSight = NewGridNo( node.sGridNo, DirectionInc( i ) );

					for ( SectorFortificationVector::iterator nodeit = vec.begin( ); nodeit != nodeitend; ++nodeit )
					{
						if ( nextGridNoinSight == (*nodeit).sGridNo && node.sLevel == (*nodeit).sLevel && node.structurexmlindex == (*nodeit).structurexmlindex )
						{
							// 2 adjacent nodes of the same type -> make them 'compatible'
							std::set<UINT8> indexset = GetStructureConstructDirectionIndizes( node.structurexmlindex,
																								(i == EAST || i == WEST), (i == NORTH || i == SOUTH), (i == EAST || i == WEST), (i == NORTH || i == SOUTH) );

							if ( !indexset.empty() )
							{
								std::set<UINT8>::iterator it = indexset.begin();

								int k = Random( indexset.size( ) );
								for (; k > 0; --k)
									++it;

								(*nodeit).usIndex = (*it);

								it = indexset.begin( );

								k = Random( indexset.size( ) );
								for ( ; k > 0; --k )
									++it;

								node.usIndex = (*it);

								break;
							}
						}
					}
				}
			}

			vec.push_back( node );
		}
	}
	// otherwise, create a new node vector
	else
	{
		SectorFortificationVector vec;
		vec.push_back( node );
		gSectorFortificationMap[ sector ] = vec;
	}

	// save the new plan
	SaveSectorFortificationPlan( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	UpdateFortificationPossibleAmount();
}

std::vector< std::pair<INT16, std::pair<UINT8, INT8> > > GetAllForticationGridNo( )
{
	std::vector< std::pair<INT16, std::pair<UINT8, INT8> > > gridnovector;

	if ( !gWorldSectorX || !gWorldSectorY )
		return gridnovector;

	UINT8 sector = SECTOR( gWorldSectorX, gWorldSectorY );

	SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

	if ( pSectorInfo && gSectorFortificationMap.find( sector ) != gSectorFortificationMap.end( ) )
	{
		SectorFortificationVector vec = gSectorFortificationMap[sector];

		SectorFortificationVector::iterator nodeitend = vec.end( );
		for ( SectorFortificationVector::iterator nodeit = vec.begin( ); nodeit != nodeitend; ++nodeit )
		{
			UINT16 type = FORTIFICATIONNODE_REMOVE;
			
			if ( (*nodeit).fBuild )
			{
				if ( gStructureConstruct[(*nodeit).structurexmlindex].fFortifyAdjacentAdjustment )
					type = FORTIFICATIONNODE_ADJACENTADJUSTMENT;
				else
					type = FORTIFICATIONNODE_NORMAL;
			}

			gridnovector.push_back( std::make_pair( (*nodeit).sGridNo, std::make_pair( type, (*nodeit).sLevel ) ) );
		}
	}

	return gridnovector;
}

void LoadSectorFortificationPlan( INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ )
{
	if ( !sSectorX || !sSectorY )
		return;

	CHAR8	filename[MAX_PATH];

	UINT8 sector = SECTOR( sSectorX, sSectorY );
	
	// get sector name for filename
	CHAR16 wSectorName[64];
	GetShortSectorString( sSectorX, sSectorY, wSectorName );
	
	if ( sSectorZ )
		sprintf( filename, "%s%S_%d.txt", FORTIFICATIONPLAN_DIRECTORY, wSectorName, sSectorZ );
	else
		sprintf( filename, "%s%S.txt", FORTIFICATIONPLAN_DIRECTORY, wSectorName );
	
	// get full path to save file
	vfs::Path vfsPath;
	vfs::COpenWriteFile rfile( filename, true );
	rfile->_getRealPath( vfsPath );
	std::string str = vfsPath.to_string( );
	
	const char* filenamewithpath = str.c_str( );

	// Read data
	std::ifstream file;
	file.open( filenamewithpath, std::ios::in );
	std::vector<FORTIFICATION_NODE> vec;

	std::copy( std::istream_iterator<FORTIFICATION_NODE>( file ),
			   std::istream_iterator<FORTIFICATION_NODE>( ),
			   std::back_inserter( vec )
			   );
	file.close( );

	gSectorFortificationMap[sector] = vec;
}

void SaveSectorFortificationPlan( INT16 sSectorX, INT16 sSectorY, INT8 sSectorZ )
{
	if ( !sSectorX || !sSectorY )
		return;

	UINT8 sector = SECTOR( sSectorX, sSectorY );

	if ( gSectorFortificationMap.find( sector ) != gSectorFortificationMap.end( ) )
	{
		SectorFortificationVector vec = gSectorFortificationMap[sector];
		
		if ( !vec.empty() )
		{
			CHAR8	filename[MAX_PATH];

			// get sector name for filename
			CHAR16 wSectorName[64];
			GetShortSectorString( sSectorX, sSectorY, wSectorName );

			if ( sSectorZ )
				sprintf( filename, "%s%S_%d.txt", FORTIFICATIONPLAN_DIRECTORY, wSectorName, sSectorZ );
			else
				sprintf( filename, "%s%S.txt", FORTIFICATIONPLAN_DIRECTORY, wSectorName );
			
			// get full path to save file
			// what we are doing here might seem rather odd. If the file does not exist, we are creating a new file via vfs... and then create a fresh binary file afterwards.
			// I found this to be the easiest solution for using both vfs pathing and being able to use a std::fstream
			// If you don't like it, fix it yourself, or make vfs not such a pain to use
			vfs::Path vfsPath;
			vfs::COpenWriteFile rfile( filename, true );
			rfile->_getRealPath( vfsPath );
			std::string str = vfsPath.to_string( );

			const char* filenamewithpath = str.c_str( );
			
			// write
			std::fstream binary_file( filenamewithpath, std::ios::out );

			SectorFortificationVector::iterator nodeitend = vec.end( );
			for ( SectorFortificationVector::iterator nodeit = vec.begin( ); nodeit != nodeitend; ++nodeit )
			{
				FORTIFICATION_NODE node = (*nodeit);

				binary_file << node;
			}

			binary_file.close( );
		}
	}
}

void UpdateFortificationPossibleAmount()
{
	if ( !gWorldSectorX || !gWorldSectorY )
		return;

	// update the sector value of how much can still be built by looping through the plan. Only works in the currently loaded sector
	UINT8 sector = SECTOR( gWorldSectorX, gWorldSectorY );
		
	if ( gSectorFortificationMap.find( sector ) != gSectorFortificationMap.end( ) )
	{
		SectorFortificationVector vec = gSectorFortificationMap[sector];

		FLOAT maxpossible = 0.0f;

		SectorFortificationVector::iterator nodeitend = vec.end( );
		for ( SectorFortificationVector::iterator nodeit = vec.begin(); nodeit != nodeitend; ++nodeit )
		{
			FORTIFICATION_NODE node = (*nodeit);
			
			if ( node.fBuild )
			{
				UINT8 buildpossiblestate = CheckBuildFortification( node.sGridNo, node.sLevel, node.usIndex, node.structurexmlindex );

				if ( buildpossiblestate == 0 )
				{
					maxpossible += gStructureConstruct[node.structurexmlindex].dCreationCost;
				}
				else if ( buildpossiblestate == 2 )
				{
					// for now, also add this as a possible thing to build if the obstacle is merely a person
					maxpossible += gStructureConstruct[node.structurexmlindex].dCreationCost;
				}
			}
			else if ( CanRemoveFortification( node.sGridNo, node.sLevel, node.structurexmlindex ) )
			{
				maxpossible += gStructureDeconstruct[node.structurexmlindex].dCreationCost;
			}
		}
		
		if ( gbWorldSectorZ )
		{
			UNDERGROUND_SECTORINFO *pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			if ( pSector )
			{
				pSector->dFortification_MaxPossible = maxpossible;

				// make sure the current progress isn't too high
				pSector->dFortification_UnappliedProgress = min( pSector->dFortification_UnappliedProgress, pSector->dFortification_MaxPossible );
			}
		}
		else
		{
			SECTORINFO *pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];

			if ( pSector )
			{
				pSector->dFortification_MaxPossible = maxpossible;

				// make sure the current progress isn't too high
				pSector->dFortification_UnappliedProgress = min( pSector->dFortification_UnappliedProgress, pSector->dFortification_MaxPossible );
			}
		}
	}
}

void HandleFortificationUpdate()
{
	if ( !gWorldSectorX || !gWorldSectorY )
		return;

	UINT8 sector = SECTOR( gWorldSectorX, gWorldSectorY );
	
	if ( gSectorFortificationMap.find( sector ) != gSectorFortificationMap.end() )
	{
		SectorFortificationVector vec = gSectorFortificationMap[sector];

		SectorFortificationVector::iterator nodeit = vec.begin();

		BOOLEAN peopleblockingnodes		= FALSE;
		BOOLEAN insufficientresources	= FALSE;

		FLOAT dFortification_UnappliedProgress = 0.0;
		FLOAT dFortification_MaxPossible = 0.0;

		if ( gbWorldSectorZ )
		{
			UNDERGROUND_SECTORINFO *pSector;
			pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			if ( pSector )
			{
				dFortification_UnappliedProgress	= pSector->dFortification_UnappliedProgress;
				dFortification_MaxPossible			= pSector->dFortification_MaxPossible;
			}
		}
		else
		{
			SECTORINFO *pSector;
			pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];

			if ( pSector )
			{
				dFortification_UnappliedProgress = pSector->dFortification_UnappliedProgress;
				dFortification_MaxPossible = pSector->dFortification_MaxPossible;
			}
		}
		
		// as we consume resources, we use a map to keep track of them (and lower the amount of looping over the inventory required)
		std::map< UINT16, INT32 > builditemobjectmap;

		// removing structures can gain us items. We use a map to store what we need and create items afterwards, which might save unnecessary creations (1 item at 80% is better than 80 with 1%)
		std::map< UINT16, UINT32 > itemcreatemap;
		INT32 itemdropoffgridno = NOWHERE;

		// in order to help the player know what items are missing, make a list of all required build items not present
		std::set< UINT16 > missingitemsmap;

		OBJECTTYPE* pBuildItemObject = NULL;
		INT32 slot = -1;

		while ( dFortification_UnappliedProgress > 0.0 )
		{
			if ( nodeit == vec.end( ) )
				break;

			FORTIFICATION_NODE node = (*nodeit);

			FLOAT cost = node.fBuild ? gStructureConstruct[node.structurexmlindex].dCreationCost : gStructureDeconstruct[node.structurexmlindex].dCreationCost;

			if ( cost <= dFortification_UnappliedProgress )
			{
				if ( node.fBuild )
				{
					UINT8 buildpossiblestate = CheckBuildFortification( node.sGridNo, node.sLevel, node.usIndex, node.structurexmlindex );

					if ( buildpossiblestate == 0 )
					{
						// check for build items and possibly consume them
						BOOLEAN itemconsumptionpassed = TRUE;

						if ( gStructureConstruct[node.structurexmlindex].usCreationItem )
						{
							itemconsumptionpassed = FALSE;

							UINT8 itemreduction = gStructureConstruct[node.structurexmlindex].usItemStatusLoss;

							std::map< UINT16, INT32 >::iterator objectit = builditemobjectmap.find( gStructureConstruct[node.structurexmlindex].usCreationItem );

							if ( objectit != builditemobjectmap.end( ) && objectit->second > -1 )
							{
								slot = objectit->second;
							}
							else
							{
								slot = GetFirstObjectInSectorPosition( gStructureConstruct[node.structurexmlindex].usCreationItem );

								if ( slot > -1 )
								{
									builditemobjectmap[gStructureConstruct[node.structurexmlindex].usCreationItem] = slot;
								}
							}

							if ( slot > -1 )
							{
								for ( INT16 i = 0; i < gWorldItems[slot].object.ubNumberOfObjects; ++i )
								{
									UINT8 reduce = min( (gWorldItems[slot].object)[i]->data.objectStatus, itemreduction );

									(gWorldItems[slot].object)[i]->data.objectStatus -= reduce;
									itemreduction -= reduce;

									if ( !(gWorldItems[slot].object)[i]->data.objectStatus )
									{
										gWorldItems[slot].object.RemoveObjectAtIndex( i );
									}
								}

								if ( gWorldItems[slot].object.ubNumberOfObjects < 1 )
								{
									if ( gWorldItems[slot].sGridNo != NOWHERE )
										RemoveItemFromPool( gWorldItems[slot].sGridNo, slot, gWorldItems[slot].ubLevel );

									gWorldItems[slot].fExists = FALSE;
								}
							}

							if ( !itemreduction )
							{
								itemconsumptionpassed = TRUE;
							}
						}

						// only if we successfully used resources a strucuture can be built
						if ( itemconsumptionpassed )
						{
							BuildFortification( node.sGridNo, node.sLevel, node.usIndex, node.structurexmlindex );

							dFortification_UnappliedProgress -= cost;
							dFortification_MaxPossible -= cost;
						}
						else
						{
							insufficientresources = TRUE;

							if ( gStructureConstruct[node.structurexmlindex].usCreationItem )
								missingitemsmap.insert( gStructureConstruct[node.structurexmlindex].usCreationItem );
						}
					}
					else if ( buildpossiblestate == 2 )
					{
						// note if some structures could not be built due to people being in the way
						peopleblockingnodes = TRUE;
					}
				}
				else if ( CanRemoveFortification( node.sGridNo, node.sLevel, node.structurexmlindex ) )
				{
					// check for build items and possibly consume them
					BOOLEAN itemconsumptionpassed = TRUE;

					if ( gStructureDeconstruct[node.structurexmlindex].usDeconstructItem )
					{
						itemconsumptionpassed = FALSE;

						std::map< UINT16, INT32 >::iterator objectit = builditemobjectmap.find( gStructureDeconstruct[node.structurexmlindex].usDeconstructItem );

						if ( objectit != builditemobjectmap.end( ) && objectit->second > -1 )
						{
							slot = objectit->second;
						}
						else
						{
							slot = GetFirstObjectInSectorPosition( gStructureDeconstruct[node.structurexmlindex].usDeconstructItem );

							if ( slot > -1 )
							{
								builditemobjectmap[gStructureDeconstruct[node.structurexmlindex].usDeconstructItem] = slot;
							}
						}

						if ( slot > -1 )
						{
							itemconsumptionpassed = TRUE;
						}
					}

					// only if we successfully used resources a strucuture can be built
					if ( itemconsumptionpassed )
					{
						if ( RemoveFortification( node.sGridNo, node.sLevel, node.structurexmlindex ) )
						{
							// eventually create item
							if ( gStructureDeconstruct[node.structurexmlindex].usItemToCreate && gStructureDeconstruct[node.structurexmlindex].usCreatedItemStatus )
							{
								std::map< UINT16, UINT32 >::iterator objectit = itemcreatemap.find( gStructureDeconstruct[node.structurexmlindex].usItemToCreate );

								if ( objectit != itemcreatemap.end( ) )
								{
									objectit->second += gStructureDeconstruct[node.structurexmlindex].usCreatedItemStatus;
								}
								else
								{
									itemcreatemap[gStructureDeconstruct[node.structurexmlindex].usItemToCreate] = gStructureDeconstruct[node.structurexmlindex].usCreatedItemStatus;
								}
							}

							// if the node's gridno is valid, we might use it
							if ( TileIsOutOfBounds( itemdropoffgridno ) && !TileIsOutOfBounds( node.sGridNo ) )
							{
								itemdropoffgridno = node.sGridNo;
							}

							dFortification_UnappliedProgress -= cost;
							dFortification_MaxPossible -= cost;
						}
					}
					else
					{
						insufficientresources = TRUE;

						if ( gStructureDeconstruct[node.structurexmlindex].usDeconstructItem )
							missingitemsmap.insert( gStructureDeconstruct[node.structurexmlindex].usDeconstructItem );
					}
				}
			}

			++nodeit;
		}

		// update progress
		if ( gbWorldSectorZ )
		{
			UNDERGROUND_SECTORINFO *pSector;
			pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

			if ( pSector )
			{
				pSector->dFortification_UnappliedProgress	= dFortification_UnappliedProgress;
				pSector->dFortification_MaxPossible			= dFortification_MaxPossible;
			}
		}
		else
		{
			SECTORINFO *pSector;
			pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];

			if ( pSector )
			{
				pSector->dFortification_UnappliedProgress = dFortification_UnappliedProgress;
				pSector->dFortification_MaxPossible = dFortification_MaxPossible;
			}
		}

		CHAR16 wSectorName[64];
		GetShortSectorString( gWorldSectorX, gWorldSectorY, wSectorName );

		// create items, or warn us if that isn't possible
		if ( !itemcreatemap.empty() )
		{
			if ( !TileIsOutOfBounds( itemdropoffgridno ) )
			{
				for ( std::map< UINT16, UINT32 >::iterator it = itemcreatemap.begin( ); it != itemcreatemap.end(); ++it )
				{
					while ( it->second > 0 )
					{
						UINT8 tmpstat = min( 100, it->second );
						it->second -= tmpstat;

						OBJECTTYPE tmpobject;

						CreateItem( it->first, tmpstat, &tmpobject );

						AddItemToPool( itemdropoffgridno, &tmpobject, 1, 0, 0, -1 );
					}
				}
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, szFortificationText[1], wSectorName );
			}
		}

		// warn us if we lack resources or if nodes are blocked
		if ( peopleblockingnodes || insufficientresources )
		{
			if ( peopleblockingnodes )
				ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, szFortificationText[2], wSectorName );

			if ( insufficientresources )
			{
				ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, szFortificationText[3], wSectorName );

				for ( std::set<UINT16>::iterator it = missingitemsmap.begin( ); it != missingitemsmap.end(); ++it )
				{
					ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L" - %s", Item[(*it)].szLongItemName );
				}
			}
		}

		UpdateFortificationPossibleAmount();
	}
}

INT32 GetFirstObjectInSectorPosition( UINT16 ausItem )
{
	for ( UINT32 uiCount = 0; uiCount < guiNumWorldItems; ++uiCount )
	{
		if ( gWorldItems[uiCount].fExists && gWorldItems[uiCount].object.usItem == ausItem )
		{
			return uiCount;
		}
	}

	return -1;
}

INT32 CheckBombDisarmChance(void)
{
	INT8 diff = 0;

    // NB owner grossness... bombs 'owned' by the enemy are stored with side value 1 in
    // the map. So if we want to detect a bomb placed by the player, owner is > 1, and
    // owner - 2 gives the ID of the character who planted it
    if ( gTempObject[0]->data.misc.ubBombOwner > 1 && ( (INT32)gTempObject[0]->data.misc.ubBombOwner - 2 >= gTacticalStatus.Team[ OUR_TEAM ].bFirstID && gTempObject[0]->data.misc.ubBombOwner - 2 <= gTacticalStatus.Team[ OUR_TEAM ].bLastID ) )
    {
		// my own boobytrap!
        if ( gTempObject[0]->data.misc.ubBombOwner - 2 == gpBoobyTrapSoldier->ubID )
			diff = 40;
		// our team's boobytrap!
        else
            diff = 20;
    }

	if ( HasItemFlag( gTempObject.usItem, BEARTRAP ) )
	{
		return SkillCheck( gpBoobyTrapSoldier, DISARM_MECHANICAL_TRAP_CHECK, diff );
	}

	return SkillCheck( gpBoobyTrapSoldier, DISARM_TRAP_CHECK, diff );
}

void ExtendedDisarmMessageBox(void)
{
	CHAR16 buf[256];
	INT16 disarmAP;

	disarmAP = GetAPsToDisarmMine( gpBoobyTrapSoldier );

	if( (gTacticalStatus.uiFlags & TURNBASED ) && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		swprintf( buf, L"%s %d", TacticalStr[ DISARM_DIALOG_DISARM ], disarmAP );
		wcscpy( gzUserDefinedButton[0], buf );
		swprintf( buf, L"%s %d", TacticalStr[ DISARM_DIALOG_INSPECT ], disarmAP / 2 );
		wcscpy( gzUserDefinedButton[1], buf );
		swprintf( buf, L"%s %d", TacticalStr[ DISARM_DIALOG_REMOVE_BLUEFLAG ], disarmAP /2 );
		wcscpy( gzUserDefinedButton[2], buf );
		swprintf( buf, L"%s %d", TacticalStr[ DISARM_DIALOG_BLOWUP ], disarmAP /4 );
		wcscpy( gzUserDefinedButton[3], buf );
	}
	else
	{
        wcscpy( gzUserDefinedButton[0], TacticalStr[ DISARM_DIALOG_DISARM ] );
        wcscpy( gzUserDefinedButton[1], TacticalStr[ DISARM_DIALOG_INSPECT ] );
        wcscpy( gzUserDefinedButton[2], TacticalStr[ DISARM_DIALOG_REMOVE_BLUEFLAG ] );
        wcscpy( gzUserDefinedButton[3], TacticalStr[ DISARM_DIALOG_BLOWUP ] );
	}
        DoMessageBox( MSG_BOX_BASIC_MEDIUM_BUTTONS, TacticalStr[ DISARM_BOOBYTRAP_PROMPT ], guiCurrentScreen, MSG_BOX_FLAG_GENERIC_FOUR_BUTTONS, ExtendedBoobyTrapMessageBoxCallBack, NULL, MSG_BOX_DEFAULT_BUTTON_1 );
}

void ExtendedBoobyTrapMessageBoxCallBack( UINT8 ubExitValue )
{
        INT32   iCheckResult;
		BOOLEAN playerMine = FALSE;
		INT16 disarmAP;
		INT16 disarmBP;
		BOOLEAN turnbased = FALSE;

		disarmAP = GetAPsToDisarmMine( gpBoobyTrapSoldier );
		disarmBP = APBPConstants[BP_DISARM_MINE];
		if( (gTacticalStatus.uiFlags & TURNBASED ) && (gTacticalStatus.uiFlags & INCOMBAT) )
			turnbased = TRUE;
		
		if(gpWorldLevelData[gsBoobyTrapGridNo].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT)
			playerMine=TRUE;

        if (ubExitValue == 1)
        {
			// this already checks AP in turnbased
                BoobyTrapMessageBoxCallBack(MSG_BOX_RETURN_YES);
        }
        else if (ubExitValue == 2)
        { 
			if( turnbased )
			{
				if(EnoughPoints(gpBoobyTrapSoldier, disarmAP / 2, disarmBP / 2 , TRUE))
					DeductPoints(gpBoobyTrapSoldier, disarmAP /2, disarmBP /2, AFTERACTION_INTERRUPT);
				else
					return;
			}
                iCheckResult=CheckBombDisarmChance();
                if(iCheckResult>60)
                        ScreenMsg( FONT_MCOLOR_WHITE, MSG_INTERFACE, TacticalStr[ INSPECT_RESULT_SAFE ] );
                else if(iCheckResult>20)
                        ScreenMsg( FONT_MCOLOR_LTGRAY, MSG_INTERFACE, TacticalStr[ INSPECT_RESULT_MOSTLY_SAFE ] );
                else if(iCheckResult>-20)
                        ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, TacticalStr[ INSPECT_RESULT_RISKY ] );
                else if(iCheckResult>-40)
                        ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, TacticalStr[ INSPECT_RESULT_DANGEROUS ] );
                else
                        ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, TacticalStr[ INSPECT_RESULT_HIGH_DANGER ] );
        }
        else if (ubExitValue == 3)
        {
			if( turnbased )
			{
				if(EnoughPoints(gpBoobyTrapSoldier, disarmAP / 2 , disarmBP / 2 , TRUE))
					DeductPoints(gpBoobyTrapSoldier, disarmAP / 2, disarmBP / 2, AFTERACTION_INTERRUPT);
				else
					return;
			}
                RemoveBlueFlag( gsBoobyTrapGridNo, gbBoobyTrapLevel );
			if( playerMine )
					gpWorldLevelData[gsBoobyTrapGridNo].uiFlags |= MAPELEMENT_PLAYER_MINE_PRESENT;
        }
        else if (ubExitValue == 4)
        {
			if( turnbased )
			{
				if(EnoughPoints(gpBoobyTrapSoldier, disarmAP / 4 , disarmBP / 4 , TRUE))
					DeductPoints(gpBoobyTrapSoldier, disarmAP / 4, disarmBP / 4, AFTERACTION_INTERRUPT);
				else
					return;
			}
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

void HandleTakeNewBombFromInventory(SOLDIERTYPE* pSoldier, OBJECTTYPE* pObj)
{
	if( !( (gTacticalStatus.uiFlags & TURNBASED ) && (gTacticalStatus.uiFlags & INCOMBAT) ) &&
			!pSoldier->inv[HANDPOS].exists() && gfShiftBombPlant )
	{	
       pSoldier->TakeNewBombFromInventory(pObj->usItem);
	}
}

// Flugente: interactive actions
void DoInteractiveAction( INT32 sGridNo, SOLDIERTYPE *pSoldier )
{
	// we need a valid soldier and a valid object
	if ( !pSoldier )
		return;
	
	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return;
	
	UINT16 structindex;
	UINT16 possibleaction = InteractiveActionPossibleAtGridNo( sGridNo, pSoldier->pathing.bLevel, structindex );

	UINT16 skill = pSoldier->GetInteractiveActionSkill( sGridNo, pSoldier->pathing.bLevel, possibleaction );

	INT32 difficulty = gInteractiveStructure[structindex].difficulty;
	INT32 luaactionid = gInteractiveStructure[structindex].luaactionid;

	// play sounds
	switch ( possibleaction )
	{
	case INTERACTIVE_STRUCTURE_HACKABLE:
		PlayJA2SampleFromFile( "Sounds\\keyboard_typing.wav", RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		break;

	case INTERACTIVE_STRUCTURE_READFILE:
		PlayJA2SampleFromFile( "Sounds\\book_pageturn1.wav", RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
		break;

	default:
		break;
	}

	if ( possibleaction == INTERACTIVE_STRUCTURE_HACKABLE )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[0], pSoldier->GetName( ) );

		// hacking is a multiturn-action - we only start the action here, the result is handled elsewhere
		pSoldier->StartMultiTurnAction( MTA_HACK, sGridNo );
	}
	// call lua with the action id - perhaps we might do something special here
	else if ( luaactionid >= 0 )
	{
		LuaHandleInteractiveActionResult( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, sGridNo, pSoldier->pathing.bLevel, pSoldier->ubID, possibleaction, luaactionid, difficulty, skill );
	}
	else
	{
		DoInteractiveActionDefaultResult( sGridNo, pSoldier->ubID, (skill >= difficulty) );
	}
}

// handle the default result of an interactive action
// This is called either if no lua action id is set, or by lua if this should happen as a supplement to whatever lua does
void DoInteractiveActionDefaultResult( INT32 sGridNo, UINT8 ubID, BOOLEAN aSuccess )
{
	SOLDIERTYPE* pSoldier = NULL;
	if ( ubID != NOBODY )
		pSoldier = MercPtrs[ubID];

	// we need a valid soldier and a valid object
	if ( !pSoldier )
		return;

	// needs to be a valid location
	if ( TileIsOutOfBounds( sGridNo ) )
		return;

	UINT16 structindex;
	UINT16 possibleaction = InteractiveActionPossibleAtGridNo( sGridNo, pSoldier->pathing.bLevel, structindex );

	switch ( possibleaction )
	{
		case INTERACTIVE_STRUCTURE_HACKABLE:
		{
			if ( aSuccess )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[1], pSoldier->GetName( ) );
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[2], pSoldier->GetName( ) );
			}
		}
		break;

		case INTERACTIVE_STRUCTURE_READFILE:
		{
			if ( aSuccess )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[3], pSoldier->GetName( ) );
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[4], pSoldier->GetName( ) );
			}
		}
		break;

		case INTERACTIVE_STRUCTURE_WATERTAP:
		{
			if ( aSuccess )
			{
				DrinkFromWaterTap( pSoldier );
			}
			else
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[5], pSoldier->GetName( ) );
			}
		}
		break;

		case INTERACTIVE_STRUCTURE_SODAMACHINE:
		{
			static UINT16 usSodaIndex = 1740;

			// we still have to determine whether a soda item exists, and whether we can pay for it
			if ( aSuccess )
			{
				aSuccess = FALSE;

				// determine soda item, only if this exists can we proceed to sell one
				if ( HasItemFlag( usSodaIndex, SODA ) || GetFirstItemWithFlag( &usSodaIndex, SODA ) )
				{
					if ( SpendMoney( pSoldier, Item[usSodaIndex].usPrice ) )
					{
						if ( CreateItem( usSodaIndex, 100, &gTempObject ) && AutoPlaceObjectAnywhere( pSoldier, &gTempObject, TRUE ) )
						{
							PlayJA2SampleFromFile( "Sounds\\soda_machine.wav", RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[6], pSoldier->GetName( ), Item[usSodaIndex].szItemName );

							aSuccess = TRUE;
						}
					}
					else
					{
						aSuccess = FALSE;

						// not enough money for a soda, hu? Too bad for you Flugente played the old Crusader Games, which teach you how to handle that particular issue!
						ScreenMsg( FONT_MCOLOR_RED, MSG_INTERFACE, L"The World Economic Consortium does not condone moochers!" );

						INT16 direction    = GetDirectionToGridNoFromGridNo( sGridNo, pSoldier->sGridNo );
						INT32 startgridno  = NewGridNo( sGridNo, DirectionInc(direction) );
						INT32 targetgridno = NewGridNo( startgridno, DirectionInc( direction ) );

						FLOAT x, y, z;

						GetTargetWorldPositions( NULL, targetgridno, &x, &y, &z );

						// fire at torso height
						z += 100.0f;

						// fire a AP rocket with damage 40. The ammotype currently gets rerolled to Ball, but it's not like anyone will complain
						FireBulletGivenTarget_NoObjectNoSoldier( 55, 2, 40, startgridno, z, x, y, z, 0 );
					}
				}
			}	
		}
		break;

		case INTERACTIVE_STRUCTURE_MINIGAME:
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[9] );
		}
		break;

		case INTERACTIVE_STRUCTURE_VARIOUS:
			break;

	default:
		break;
	}
}

// character spends money - either from inventory or the account
BOOLEAN SpendMoney( SOLDIERTYPE *pSoldier, UINT32 aAmount )
{
	if ( !pSoldier )
		return FALSE;

	INT8 invsize = (INT8)pSoldier->inv.size( );
	for ( INT8 bLoop = 0; bLoop < invsize; ++bLoop )
	{
		if ( pSoldier->inv[bLoop].exists( ) == true && pSoldier->inv[bLoop].usItem == MONEY )
		{
			OBJECTTYPE* pObj = &(pSoldier->inv[bLoop]);

			UINT32 remove = min( aAmount, (*pObj)[0]->data.money.uiMoneyAmount );
			(*pObj)[0]->data.money.uiMoneyAmount -= remove;
			aAmount -= remove;

			if ( (*pObj)[0]->data.money.uiMoneyAmount <= 0 )
				DeleteObj( &(pSoldier->inv[bLoop]) );

			if ( aAmount <= 0 )
				break;
		}
	}

	// if we didn't have enough money on us, transfer
	if ( aAmount > 0 && LaptopSaveInfo.iCurrentBalance >= aAmount )
	{
		AddTransactionToPlayersBook( TRANSFER_FUNDS_TO_MERC, pSoldier->ubProfile, GetWorldTotalMin( ), -aAmount );

		aAmount = 0;
	}

	// warning if we don't have enough money
	if ( aAmount > 0 )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szInteractiveActionText[7], pSoldier->GetName( ) );

	return (aAmount == 0);
}

// Flugente: gear templates
extern std::vector<std::string> gTemplateVector;

// this class is used to store our gear templates
class GEAR_NODE
{
public:
	GEAR_NODE(INT8 a, UINT16 b, UINT16 c) :
		slot( a ), item( b ), ammoitem(c){}

	GEAR_NODE() :
		slot( NOWHERE ), item( 0 ), ammoitem(0) {}

	void swap(GEAR_NODE& other)
	{
		std::swap( slot, other.slot );
		std::swap( item, other.item );
		std::swap( ammoitem, other.ammoitem );

		std::vector<UINT16> tmp = other.attachments;
		other.attachments = attachments;
		attachments = tmp;
	}

	INT8 slot;
	UINT16 item;
	UINT16 ammoitem;

	std::vector<UINT16>	attachments;
};

std::ostream& operator<<(std::ostream& stream, GEAR_NODE const& data)
{
	stream << (int)(data.slot) << " " << (UINT16)(data.item) << " " << (UINT16)(data.ammoitem);

	std::vector<UINT16> tmp = data.attachments;

	for ( std::vector<UINT16>::iterator it = tmp.begin( ); it != tmp.end( ); ++it )
		stream << " " << (*it);

	stream << std::endl;

	return stream;
}

std::vector<GEAR_NODE> LoadEquipmentTemplate( std::string aName )
{
	CHAR8	filename[MAX_PATH];

	sprintf(filename, "%s%s", EQUIPMENTTEMPLATE_DIRECTORY, aName.c_str());

	// get full path to save file
	vfs::Path vfsPath;
	vfs::COpenWriteFile rfile( filename, true );
	rfile->_getRealPath(vfsPath);
	std::string str = vfsPath.to_string();

	const char* filenamewithpath = str.c_str();

	// Read data
	std::ifstream file;
	file.open(filenamewithpath, std::ios::in);
	std::vector<GEAR_NODE> vec;

	std::string line;

	while ( std::getline( file >> std::ws, line ) )
	{
		if ( line[0] != '#' && line[0] != '/' )
		{
			std::istringstream iss( line );

			GEAR_NODE node;

			int num;
			if ( iss >> num && iss >> (UINT16)node.item )
			{
				node.slot = num;

				// additional data is optional
				if ( iss >> (UINT16)node.ammoitem )
				{
					while ( iss >> num )
					{
						node.attachments.push_back( num );
					}
				}

				vec.push_back(node);
			}
		}
	}
	
	file.close();

	return vec;
}

void SaveEquipmentTemplate(std::vector<GEAR_NODE> aVec, STR16 aName)
{
	CHAR8	filename[MAX_PATH];

	sprintf(filename, "%s%S.txt", EQUIPMENTTEMPLATE_DIRECTORY, aName);

	// get full path to save file
	// what we are doing here might seem rather odd. If the file does not exist, we are creating a new file via vfs... and then create a fresh binary file afterwards.
	// I found this to be the easiest solution for using both vfs pathing and being able to use a std::fstream
	// If you don't like it, fix it yourself, or make vfs not such a pain to use
	vfs::Path vfsPath;
	vfs::COpenWriteFile rfile(filename, true);
	rfile->_getRealPath(vfsPath);
	std::string str = vfsPath.to_string();

	const char* filenamewithpath = str.c_str();

	// write
	std::fstream binary_file(filenamewithpath, std::ios::out);

	for (std::vector<GEAR_NODE>::iterator it = aVec.begin(); it != aVec.end(); ++it)
	{
		GEAR_NODE node = (*it);

		binary_file << node;
	}

	binary_file.close();
}

void WriteEquipmentTemplate(SOLDIERTYPE* pSoldier, STR16 name)
{
	if (pSoldier)
	{
		std::vector<GEAR_NODE> vec;

		INT8 invsize = (INT8)pSoldier->inv.size();									// remember inventorysize, so we don't call size() repeatedly

		for (INT8 bLoop = 0; bLoop < invsize; ++bLoop)								// ... for all items in our inventory ...
		{
			if (pSoldier->inv[bLoop].exists())
			{
				// if this item is a gun, note what magazine is currently loaded
				UINT16 ammoitem = 0;
				if ( Item[(pSoldier->inv[bLoop]).usItem].usItemClass & IC_GUN )
					ammoitem = (pSoldier->inv[bLoop])[0]->data.gun.usGunAmmoItem;

				GEAR_NODE node( bLoop, (pSoldier->inv[bLoop]).usItem, ammoitem );

				// add all attachments
				attachmentList::iterator iterend = (pSoldier->inv[bLoop])[0]->attachments.end( );
				for ( attachmentList::iterator iter = (pSoldier->inv[bLoop])[0]->attachments.begin( ); iter != iterend; ++iter )
				{
					if ( iter->exists( ) )
						node.attachments.push_back( iter->usItem );
				}
				
				vec.push_back(node);
			}
		}

		SaveEquipmentTemplate(vec, name);
	}
}

std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(fd.cFileName);
			}
		}
		while (::FindNextFile(hFind, &fd));

		::FindClose(hFind);
	}
	return names;
}

// get a vector of all tilesets that are allowed to be built in this sector (the above filtered by structure construct/deconstruct basically)
void GetEquipmentTemplates()
{
	CHAR8	filename[MAX_PATH];

	sprintf(filename, "%sUpdate Gear.txt", EQUIPMENTTEMPLATE_DIRECTORY);

	// get full path to save file
	// what we are doing here might seem rather odd. If the file does not exist, we are creating a new empty file via vfs
	// in this case, this is rather useful, as the above file simply updates our gear, which is already neat
	// I found this to be the easiest solution for using both vfs pathing and being able to use a std::fstream
	// If you don't like it, fix it yourself, or make vfs not such a pain to use
	vfs::Path vfsPath;	
	vfs::COpenWriteFile rfile( filename, true );
	rfile->_getRealPath(vfsPath);

	vfs::Path dir, file;
	vfsPath.splitLast(dir, file);

	std::string str = dir.to_string();

	const char* filenamewithpath = str.c_str();

	gTemplateVector = get_all_files_names_within_folder( filenamewithpath );
}

void ReadEquipmentTable( SOLDIERTYPE* pSoldier, std::string name )
{
	// make sure the merc is actually in this sector
	if ( pSoldier )
	{
		UINT16 color = FONT_MCOLOR_WHITE;

		// for safety, test again
		if ( pSoldier->sSectorX != sSelMapX ||
			 pSoldier->sSectorY != sSelMapY ||
			 pSoldier->bSectorZ != iCurrentMapSectorZ )
		{
			ScreenMsg( color, MSG_INTERFACE, szGearTemplateText[3], pSoldier->GetName( ) );

			return;
		}

		if ( (gTacticalStatus.uiFlags & INCOMBAT || gTacticalStatus.fEnemyInSector) )
		{
			ScreenMsg( color, MSG_INTERFACE, szGearTemplateText[1] );

			return;
		}

		std::vector<GEAR_NODE> vec = LoadEquipmentTemplate(name);

		UINT16 usReloadSound = 0;
		BOOLEAN attachmentsound = FALSE;

		// refill canteens (for convenience only with drinkable, non-poisonous water)
		UINT8 waterquality = GetWaterQuality( sSelMapX, sSelMapY, iCurrentMapSectorZ );
		BOOLEAN refillwaterfromsector = (waterquality == WATER_DRINKABLE);

		// the temperature of the water in this sector (temperature reflects the quality)
		FLOAT wateraddtemperature = OVERHEATING_MAX_TEMPERATURE;

		// if we try to add an attachment that cannot be added by this function (we don't perform skill checks here), warn the player of the offending item
		UINT16 attachmenttowarnabout = NOTHING;
		
		// 1. loop over the gear we should pick up and remove mismatching items if we can find a fitting one in the sector
		for (std::vector<GEAR_NODE>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			GEAR_NODE node = (*it);

			if ( node.slot >= 0 && node.slot < NUM_INV_SLOTS && node.item != NOTHING )
			{
				// is there already an item here?
				if (pSoldier->inv[node.slot].exists())
				{
					if ( (pSoldier->inv[node.slot]).usItem != node.item )
					{
						// can we get the requested item in the first place?
						UINT32 poolslot = 0;
						UINT8 index = 0;
						if ( GetBetterObject_InventoryPool( node.item, 0, poolslot, index ) )
						{
							// We have to check whether we have any LBE equipped that isn't in the template
							// in that case we have to dump it and it's contents, otherwise it can block slots that are used in the template
							if ( node.slot >= VESTPOCKPOS && node.slot <= BPACKPOCKPOS )
							{
								std::vector<INT8> pocketKey;
								GetLBESlots( node.slot, pocketKey );

								// remove everything inside LBE
								for ( std::vector<INT8>::iterator lbeslotit = pocketKey.begin( ); lbeslotit != pocketKey.end( ); ++lbeslotit )
								{
									if ( pSoldier->inv[(*lbeslotit)].exists( ) )
									{
										AutoPlaceObjectInInventoryStash( &pSoldier->inv[(*lbeslotit)], pSoldier->sGridNo, pSoldier->pathing.bLevel );
										DeleteObj( &pSoldier->inv[(*lbeslotit)] );
									}
								}
							}
							// if we want to equip a two-handed item in our first hand, also drop whatever we have in the second hand
							else if ( node.slot == HANDPOS && Item[node.item].twohanded && pSoldier->inv[SECONDHANDPOS].exists( ) )
							{
								AutoPlaceObjectInInventoryStash( &pSoldier->inv[SECONDHANDPOS], pSoldier->sGridNo, pSoldier->pathing.bLevel );
								DeleteObj( &pSoldier->inv[SECONDHANDPOS] );
							}
							// if we are Nails and are ordered to drop our vest, refuse and complain!
							else if ( node.slot == VESTPOS && HandleNailsVestFetish( pSoldier, node.slot, NOTHING ) )
								continue;

							// drop item
							AutoPlaceObjectInInventoryStash( &pSoldier->inv[node.slot], pSoldier->sGridNo, pSoldier->pathing.bLevel );
							DeleteObj( &pSoldier->inv[node.slot] );
						}
					}
				}
			}
		}

		// 2. loop over the gear again and pick up what fits
		for ( std::vector<GEAR_NODE>::iterator it = vec.begin( ); it != vec.end( ); ++it )
		{
			GEAR_NODE node = (*it);

			if ( node.slot >= 0 && node.slot < NUM_INV_SLOTS )
			{
				// only if slot is still empty...
				if ( !pSoldier->inv[node.slot].exists( ) && node.item != NOTHING )
				{
					// see if we can find a better object in this sector and put it into our slot
					UINT32 poolslot = 0;
					UINT8 index = 0;
					if ( GetBetterObject_InventoryPool( node.item, 0, poolslot, index ) )
					{
						(pInventoryPoolList[poolslot].object).RemoveObjectAtIndex( index, &gItemPointer );

						if ( !PlaceObject( pSoldier, node.slot, &gItemPointer ) )
						{
							ScreenMsg( color, MSG_INTERFACE, szGearTemplateText[4], pSoldier->GetName( ), Item[node.item].szItemName );

							AutoPlaceObjectInInventoryStash( &gItemPointer, pSoldier->sGridNo, pSoldier->pathing.bLevel );
							DeleteObj( &gItemPointer );
						}
					}
					else
					{
						// if this is ammo, we might still be able to create a mag from different-sized mags
						if ( Item[node.item].usItemClass & IC_AMMO )
						{
							UINT16 usMagIndex = Item[node.item].ubClassIndex;

							UINT16 neededammo = Magazine[usMagIndex].ubMagSize;

							if ( neededammo > 0 )
							{
								UINT32 poolslot = 0;
								if ( GetFittingAmmo_InventoryPool( Magazine[usMagIndex].ubCalibre, Magazine[usMagIndex].ubAmmoType, poolslot ) )
								{
									UINT32 takeammo = min( neededammo, pInventoryPoolList[poolslot].object[pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1]->data.ubShotsLeft );
									
									pInventoryPoolList[poolslot].object[pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1]->data.ubShotsLeft -= takeammo;

									if ( !pInventoryPoolList[poolslot].object[pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1]->data.ubShotsLeft )
									{
										pInventoryPoolList[poolslot].object.RemoveObjectAtIndex( pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1 );
									}

									CreateAmmo( node.item, &gItemPointer, takeammo );

									if ( !PlaceObject( pSoldier, node.slot, &gItemPointer ) )
									{
										ScreenMsg( color, MSG_INTERFACE, szGearTemplateText[4], pSoldier->GetName( ), Item[node.item].szItemName );

										AutoPlaceObjectInInventoryStash( &gItemPointer, pSoldier->sGridNo, pSoldier->pathing.bLevel );
										DeleteObj( &gItemPointer );
									}
								}
							}
						}

						//if ( !pSoldier->inv[node.slot].exists( ) )
							//ScreenMsg( color, MSG_INTERFACE, L"%s found no %s to equip.", pSoldier->GetName( ), Item[node.item].szItemName );
					}
				}

				// if this slot is a LBE slot, we need to add attachments here. Otherwise we add the attachments after we fail to pick up other items as their LBE slots won't exist
				if ( pSoldier->inv[node.slot].exists() && node.slot >= VESTPOCKPOS && node.slot <= BPACKPOCKPOS )
				{
					// try to add attachments if nod entry fits the item, or if the node item is NOTHING (essentially a wildcard for this)
					if ( !node.attachments.empty( ) && (pSoldier->inv[node.slot]).usItem == node.item || NOTHING == node.item )
					{
						OBJECTTYPE* pObj = &(pSoldier->inv[node.slot]);

						for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
						{
							for ( std::vector<UINT16>::iterator attit = node.attachments.begin( ); attit != node.attachments.end( ); ++attit )
							{
								UINT16 item_attachment = (*attit);

								// only apply attachments if they don't require any checks!
								INT32 bAttachInfoIndex = GetAttachmentInfoIndex( item_attachment );

								if ( bAttachInfoIndex < 0 || AttachmentInfo[bAttachInfoIndex].bAttachmentSkillCheck == NO_CHECK )
								{
									UINT32 poolslot = 0;
									UINT8 index = 0;
									if ( GetBetterObject_InventoryPool( item_attachment, 0, poolslot, index ) )
									{
										( pInventoryPoolList[poolslot].object ).RemoveObjectAtIndex( index, &gItemPointer );

										BOOLEAN isAttachedNow = pObj->AttachObject( pSoldier, &gItemPointer, FALSE, i ); //do the actual attaching

										if ( !isAttachedNow )
										{
											attachmentsound = TRUE;

											//ScreenMsg( color, MSG_INTERFACE, L"%s could not attach %s to %s", pSoldier->GetName( ), Item[item_attachment].szItemName, Item[pObj->usItem].szItemName );

											AutoPlaceObjectInInventoryStash( &gItemPointer, pSoldier->sGridNo, pSoldier->pathing.bLevel );
											DeleteObj( &gItemPointer );
										}
									}
								}
								else
								{
									attachmenttowarnabout = item_attachment;
								}
							}
						}
					}
				}
			}
		}

		// 3. loop over all slots (not just those of the gear vector), improve, merge and fill stacks
		for ( int slot = HELMETPOS; slot < NUM_INV_SLOTS; ++slot )
		{
			if (pSoldier->inv[slot].exists())
			{
				OBJECTTYPE* pObj = &(pSoldier->inv[slot]);

				if ( Item[pObj->usItem].usItemClass & (IC_KIT | IC_MEDKIT) && pObj->ubNumberOfObjects > 1 )
				{
					CleanUpStack( pObj, pObj );
				}
				
				// if this is ammo, try merging mags first before replacing them, the not-full mag being the visible one
				// by doing this we won't have so many used magazines lying around
				if ( (Item[pObj->usItem].usItemClass & IC_AMMO) && pObj->ubNumberOfObjects > 1 )
				{
					UINT16 usMagIndex = Item[pObj->usItem].ubClassIndex;
					UINT16 magsize = Magazine[usMagIndex].ubMagSize;

					if ( magsize )
					{
						UINT32 ammocount = 0;
						for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
							ammocount += (*pObj)[i]->data.ubShotsLeft;

						UINT8 fullmags = ammocount / magsize;

						UINT16 leftover = ammocount - fullmags * magsize;

						UINT8 totalmags = fullmags + (leftover > 0 ? 1 : 0);

						if ( totalmags < pObj->ubNumberOfObjects )
							pObj->RemoveObjectsFromStack( pObj->ubNumberOfObjects - totalmags );

						for ( INT16 i = 0; i < totalmags; ++i )
							(*pObj)[i]->data.ubShotsLeft = magsize;

						if ( leftover > 0 )
							(*pObj)[0]->data.ubShotsLeft = leftover;
					}
				}

				// for every object in the stack, try to find a better improvement
				for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
				{
					// see if we can find a better object in this sector and put it into our slot
					UINT32 poolslot = 0;
					UINT8 index = 0;
					if ( (*pObj)[i]->data.objectStatus < 100 && GetBetterObject_InventoryPool( pObj->usItem, (*pObj)[i]->data.objectStatus, poolslot, index ) )
					{
						// WARNING! The correct way of doing this would be to exchange both objects
						// we are not doing this here on purpose, as we want to save time
						// instead, we switch status and a few other properties
						// Only do this if you know what you are doing - in the wrong location, we could do odd things to global pointers. Consider yourself warned!

						// tmp data
						INT16	objectStatus = (*pObj)[i]->data.objectStatus;
						INT8	bTrap = (*pObj)[i]->data.bTrap;
						UINT8	fUsed = (*pObj)[i]->data.fUsed;
						UINT8	ubImprintID = (*pObj)[i]->data.ubImprintID;
						FLOAT	bTemperature = (*pObj)[i]->data.bTemperature;
						UINT8	ubDirection = (*pObj)[i]->data.ubDirection;
						UINT32	ubWireNetworkFlag = (*pObj)[i]->data.ubWireNetworkFlag;
						INT8	bDefuseFrequency = (*pObj)[i]->data.bDefuseFrequency;
						INT16	sRepairThreshold = (*pObj)[i]->data.sRepairThreshold;
						UINT64	sObjectFlag = (*pObj)[i]->data.sObjectFlag;

						// set data on our object
						(*pObj)[i]->data.objectStatus = (pInventoryPoolList[poolslot].object)[index]->data.objectStatus;
						(*pObj)[i]->data.bTrap = (pInventoryPoolList[poolslot].object)[index]->data.bTrap;
						(*pObj)[i]->data.fUsed = (pInventoryPoolList[poolslot].object)[index]->data.fUsed;
						(*pObj)[i]->data.ubImprintID = (pInventoryPoolList[poolslot].object)[index]->data.ubImprintID;
						(*pObj)[i]->data.bTemperature = (pInventoryPoolList[poolslot].object)[index]->data.bTemperature;
						(*pObj)[i]->data.ubDirection = (pInventoryPoolList[poolslot].object)[index]->data.ubDirection;
						(*pObj)[i]->data.ubWireNetworkFlag = (pInventoryPoolList[poolslot].object)[index]->data.ubWireNetworkFlag;
						(*pObj)[i]->data.bDefuseFrequency = (pInventoryPoolList[poolslot].object)[index]->data.bDefuseFrequency;
						(*pObj)[i]->data.sRepairThreshold = (pInventoryPoolList[poolslot].object)[index]->data.sRepairThreshold;
						(*pObj)[i]->data.sObjectFlag = (pInventoryPoolList[poolslot].object)[index]->data.sObjectFlag;

						// set data on world object
						(pInventoryPoolList[poolslot].object)[index]->data.objectStatus = objectStatus;
						(pInventoryPoolList[poolslot].object)[index]->data.bTrap = bTrap;
						(pInventoryPoolList[poolslot].object)[index]->data.fUsed = fUsed;
						(pInventoryPoolList[poolslot].object)[index]->data.ubImprintID = ubImprintID;
						(pInventoryPoolList[poolslot].object)[index]->data.bTemperature = bTemperature;
						(pInventoryPoolList[poolslot].object)[index]->data.ubDirection = ubDirection;
						(pInventoryPoolList[poolslot].object)[index]->data.ubWireNetworkFlag = ubWireNetworkFlag;
						(pInventoryPoolList[poolslot].object)[index]->data.bDefuseFrequency = bDefuseFrequency;
						(pInventoryPoolList[poolslot].object)[index]->data.sRepairThreshold = sRepairThreshold;
						(pInventoryPoolList[poolslot].object)[index]->data.sObjectFlag = sObjectFlag;
					}
				}
				
				// pick up new items from the sector and put them on the stack
				UINT8 ubSlotLimit = ItemSlotLimit(pObj, slot, pSoldier, FALSE);

				while (pObj->ubNumberOfObjects < ubSlotLimit)
				{
					UINT32 poolslot = 0;
					UINT8 index = 0;

					// if this is ammo, we might still be able to create a mag from different-sized mags
					if ( Item[pObj->usItem].usItemClass & IC_AMMO )
					{
						UINT16 usMagIndex = Item[pObj->usItem].ubClassIndex;

						UINT16 neededammo = Magazine[usMagIndex].ubMagSize;

						if ( neededammo > 0 )
						{
							if ( GetFittingAmmo_InventoryPool( Magazine[usMagIndex].ubCalibre, Magazine[usMagIndex].ubAmmoType, poolslot ) )
							{
								UINT32 takeammo = min( neededammo, pInventoryPoolList[poolslot].object[pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1]->data.ubShotsLeft );

								pInventoryPoolList[poolslot].object[pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1]->data.ubShotsLeft -= takeammo;

								if ( !pInventoryPoolList[poolslot].object[pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1]->data.ubShotsLeft )
								{
									pInventoryPoolList[poolslot].object.RemoveObjectAtIndex( pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1 );
								}

								CreateAmmo( pObj->usItem, &gItemPointer, takeammo );

								if ( !PlaceObject( pSoldier, slot, &gItemPointer ) )
								{
									ScreenMsg( color, MSG_INTERFACE, szGearTemplateText[4], pSoldier->GetName(), Item[pObj->usItem].szItemName );

									AutoPlaceObjectInInventoryStash( &gItemPointer, pSoldier->sGridNo, pSoldier->pathing.bLevel );
									DeleteObj( &gItemPointer );
								}
								else
								{
									// if we are succesful, we will get here, otherwise we break
									continue;
								}
							}
						}

						break;
					}
					else if ( GetBetterObject_InventoryPool( pObj->usItem, 0, poolslot, index ) )
					{
						( pInventoryPoolList[poolslot].object ).RemoveObjectAtIndex( index, &gItemPointer );

						if ( !TryToStackInSlot( pSoldier, &gItemPointer, slot ) )
							break;
					}
					else
					{
						break;
					}
				}

				// try to improve attachments, too
				for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
				{
					for ( attachmentList::iterator iter = (*pObj)[i]->attachments.begin(); iter != (*pObj)[i]->attachments.end(); ++iter )
					{
						if ( iter->exists() )
						{
							// see if we can find a better object in this sector and put it into our slot
							UINT32 poolslot = 0;
							UINT8 index = 0;
							if ( (*iter)[0]->data.objectStatus < 100 && GetBetterObject_InventoryPool( iter->usItem, (*iter)[0]->data.objectStatus, poolslot, index ) )
							{
								// WARNING! The correct way of doing this would be to exchange both objects
								// we are not doing this here on purpose, as we want to save time
								// instead, we switch status and a few other properties
								// Only do this if you know what you are doing - in the wrong location, we could do odd things to global pointers. Consider yourself warned!

								// tmp data
								INT16	objectStatus = (*iter)[0]->data.objectStatus;
								INT8	bTrap = (*iter)[0]->data.bTrap;
								UINT8	fUsed = (*iter)[0]->data.fUsed;
								UINT8	ubImprintID = (*iter)[0]->data.ubImprintID;
								FLOAT	bTemperature = (*iter)[0]->data.bTemperature;
								UINT8	ubDirection = (*iter)[0]->data.ubDirection;
								UINT32	ubWireNetworkFlag = (*iter)[0]->data.ubWireNetworkFlag;
								INT8	bDefuseFrequency = (*iter)[0]->data.bDefuseFrequency;
								INT16	sRepairThreshold = (*iter)[0]->data.sRepairThreshold;
								UINT64	sObjectFlag = (*iter)[0]->data.sObjectFlag;

								// set data on our object
								(*iter)[0]->data.objectStatus = (pInventoryPoolList[poolslot].object)[index]->data.objectStatus;
								(*iter)[0]->data.bTrap = (pInventoryPoolList[poolslot].object)[index]->data.bTrap;
								(*iter)[0]->data.fUsed = (pInventoryPoolList[poolslot].object)[index]->data.fUsed;
								(*iter)[0]->data.ubImprintID = (pInventoryPoolList[poolslot].object)[index]->data.ubImprintID;
								(*iter)[0]->data.bTemperature = (pInventoryPoolList[poolslot].object)[index]->data.bTemperature;
								(*iter)[0]->data.ubDirection = (pInventoryPoolList[poolslot].object)[index]->data.ubDirection;
								(*iter)[0]->data.ubWireNetworkFlag = (pInventoryPoolList[poolslot].object)[index]->data.ubWireNetworkFlag;
								(*iter)[0]->data.bDefuseFrequency = (pInventoryPoolList[poolslot].object)[index]->data.bDefuseFrequency;
								(*iter)[0]->data.sRepairThreshold = (pInventoryPoolList[poolslot].object)[index]->data.sRepairThreshold;
								(*iter)[0]->data.sObjectFlag = (pInventoryPoolList[poolslot].object)[index]->data.sObjectFlag;

								// set data on world object
								(pInventoryPoolList[poolslot].object)[index]->data.objectStatus = objectStatus;
								(pInventoryPoolList[poolslot].object)[index]->data.bTrap = bTrap;
								(pInventoryPoolList[poolslot].object)[index]->data.fUsed = fUsed;
								(pInventoryPoolList[poolslot].object)[index]->data.ubImprintID = ubImprintID;
								(pInventoryPoolList[poolslot].object)[index]->data.bTemperature = bTemperature;
								(pInventoryPoolList[poolslot].object)[index]->data.ubDirection = ubDirection;
								(pInventoryPoolList[poolslot].object)[index]->data.ubWireNetworkFlag = ubWireNetworkFlag;
								(pInventoryPoolList[poolslot].object)[index]->data.bDefuseFrequency = bDefuseFrequency;
								(pInventoryPoolList[poolslot].object)[index]->data.sRepairThreshold = sRepairThreshold;
								(pInventoryPoolList[poolslot].object)[index]->data.sObjectFlag = sObjectFlag;
							}
						}
					}
				}
			}
		}

		// 4. add attachments. If an attachment cannot be attached - for example because a different one is blocking it - tell us so
		for ( std::vector<GEAR_NODE>::iterator it = vec.begin( ); it != vec.end( ); ++it )
		{
			GEAR_NODE node = (*it);
			
			// try to add attachments if nod entry fits the item, or if the node item is NOTHING (essentially a wildcard for this)
			if ( node.slot >= 0 && node.slot < NUM_INV_SLOTS && 
				 (node.slot < VESTPOCKPOS || node.slot > BPACKPOCKPOS) &&		// we already did that for LBE slots though, no need to repeat
				 !node.attachments.empty( ) && pSoldier->inv[node.slot].exists( ) &&
				 ((pSoldier->inv[node.slot]).usItem == node.item || NOTHING == node.item) )
			{
				OBJECTTYPE* pObj = &(pSoldier->inv[node.slot]);

				for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
				{
					for ( std::vector<UINT16>::iterator attit = node.attachments.begin( ); attit != node.attachments.end( ); ++attit )
					{
						UINT16 item_attachment = (*attit);

						// only apply attachments if they don't require any checks!
						INT32 bAttachInfoIndex = GetAttachmentInfoIndex( item_attachment );

						if ( bAttachInfoIndex < 0 || AttachmentInfo[bAttachInfoIndex].bAttachmentSkillCheck == NO_CHECK )
						{
							UINT32 poolslot = 0;
							UINT8 index = 0;
							if ( GetBetterObject_InventoryPool( item_attachment, 0, poolslot, index ) )
							{
								(pInventoryPoolList[poolslot].object).RemoveObjectAtIndex( index, &gItemPointer );

								BOOLEAN isAttachedNow = pObj->AttachObject( pSoldier, &gItemPointer, FALSE, i ); //do the actual attaching

								if ( !isAttachedNow )
								{
									attachmentsound = TRUE;

									//ScreenMsg( color, MSG_INTERFACE, L"%s could not attach %s to %s", pSoldier->GetName( ), Item[item_attachment].szItemName, Item[pObj->usItem].szItemName );

									AutoPlaceObjectInInventoryStash( &gItemPointer, pSoldier->sGridNo, pSoldier->pathing.bLevel );
									DeleteObj( &gItemPointer );
								}
							}
						}	
						else
						{
							attachmenttowarnabout = item_attachment;
						}
					}
				}
			}
		}

		// 5. reload guns, fill mags
		for ( int slot = HELMETPOS; slot < NUM_INV_SLOTS; ++slot )
		{
			if ( pSoldier->inv[slot].exists( ) )
			{
				if ( Item[(pSoldier->inv[slot]).usItem].usItemClass & (IC_KIT | IC_MEDKIT) )
				{
					UINT32 poolslot = 0;
					UINT8 index = 0;
					if ( GetBetterObject_InventoryPool( (pSoldier->inv[slot]).usItem, 0, poolslot, index ) )
					{
						DistributeStatus( &(pInventoryPoolList[poolslot].object), &(pSoldier->inv[slot]), 100);
					}
				}

				// if there is water in this sector, refill canteens
				if ( refillwaterfromsector && Item[(pSoldier->inv[slot]).usItem].canteen && Food[Item[(pSoldier->inv[slot]).usItem].foodtype].bDrinkPoints > 0 )
				{
					OBJECTTYPE* pObj = &(pSoldier->inv[slot]);

					for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
					{
						UINT16 status = (*pObj)[i]->data.objectStatus;
						UINT16 statusmmissing = max( 0, 100 - status );
						FLOAT temperature = (*pObj)[i]->data.bTemperature;

						(*pObj)[i]->data.objectStatus = 100;
						(*pObj)[i]->data.bTemperature = (status * temperature + statusmmissing * wateraddtemperature) / 100;
					}
				}
				
				if ( (Item[(pSoldier->inv[slot]).usItem].usItemClass & (IC_GUN | IC_AMMO)))
				{
					OBJECTTYPE* pObj = &(pSoldier->inv[slot]);

					for ( INT16 i = 0; i < pObj->ubNumberOfObjects; ++i )
					{
						UINT16 ammoitem = (pSoldier->inv[slot]).usItem;

						if ( (Item[(pSoldier->inv[slot]).usItem].usItemClass & IC_GUN) )
						{
							UINT8 calibre = Weapon[pObj->usItem].ubCalibre;
							UINT16 magsize = GetMagSize( pObj, i );

							ammoitem = (*pObj)[i]->data.gun.usGunAmmoItem;
							for ( std::vector<GEAR_NODE>::iterator it = vec.begin( ); it != vec.end( ); ++it )
							{
								// use the ammotype of the node, provided it exists and fits the gun or a wildcard
								if ( (*it).slot == slot && (*it).ammoitem != NOTHING && ( (*it).item == pObj->usItem || (*it).item == NOTHING ) )
								{
									ammoitem = (*it).ammoitem;

									if ( (*it).item == pObj->usItem )
										break;
								}
							}

							// if we somehow don't have a valid ammoitem (for example, if the gun was empty), find one
							if ( !ammoitem )
								ammoitem = DefaultMagazine( pObj->usItem );

							UINT16 usMagIndex = Item[ammoitem].ubClassIndex;

							// safety check: if the calibre or mag size doesn't fit, get a better mag
							if ( calibre != Magazine[usMagIndex].ubCalibre || magsize != Magazine[usMagIndex].ubMagSize )
							{
								ammoitem = FindReplacementMagazine( calibre, magsize, Magazine[usMagIndex].ubAmmoType );
								usMagIndex = Item[ammoitem].ubClassIndex;
							}

							// if the requested ammo isn't the only currently loaded, take that one out
							if ( (*pObj)[i]->data.gun.usGunAmmoItem != ammoitem && (*pObj)[i]->data.gun.usGunAmmoItem != NOTHING && ammoitem != NOTHING )
							{
								CreateAmmo( (*pObj)[i]->data.gun.usGunAmmoItem, &gItemPointer, (*pObj)[i]->data.gun.ubGunShotsLeft );

								AutoPlaceObjectInInventoryStash( &gItemPointer, pSoldier->sGridNo, pSoldier->pathing.bLevel );
								DeleteObj( &gItemPointer );

								(*pObj)[i]->data.gun.usGunAmmoItem = NOTHING;
								(*pObj)[i]->data.gun.ubGunShotsLeft = 0;
								(*pObj)[i]->data.gun.ubGunAmmoType = 0;
							}
						}

						UINT16 usMagIndex = Item[ammoitem].ubClassIndex;

						UINT16 neededammo = Magazine[usMagIndex].ubMagSize - (*pObj)[i]->data.ubShotsLeft;
						if ( (Item[(pSoldier->inv[slot]).usItem].usItemClass & IC_GUN) )
							neededammo = Magazine[usMagIndex].ubMagSize - (*pObj)[i]->data.gun.ubGunShotsLeft;
						
						while ( neededammo > 0 )
						{
							UINT32 poolslot = 0;
							if ( GetFittingAmmo_InventoryPool( Magazine[usMagIndex].ubCalibre, Magazine[usMagIndex].ubAmmoType, poolslot ) )
							{
								for ( INT16 j = pInventoryPoolList[poolslot].object.ubNumberOfObjects - 1; j >= 0; --j )
								{
									UINT32 takeammo = min( neededammo, pInventoryPoolList[poolslot].object[j]->data.ubShotsLeft );

									neededammo -= takeammo;
									
									pInventoryPoolList[poolslot].object[j]->data.ubShotsLeft -= takeammo;

									if ( (Item[(pSoldier->inv[slot]).usItem].usItemClass & IC_GUN) )
									{
										(*pObj)[i]->data.gun.usGunAmmoItem = ammoitem;
										(*pObj)[i]->data.gun.ubGunAmmoType = Magazine[usMagIndex].ubAmmoType;
										(*pObj)[i]->data.gun.ubGunShotsLeft += takeammo;

										usReloadSound = Weapon[pObj->usItem].sReloadSound;
									}
									else
									{
										(*pObj)[i]->data.ubShotsLeft += takeammo;
									}

									if ( !pInventoryPoolList[poolslot].object[j]->data.ubShotsLeft )
									{
										pInventoryPoolList[poolslot].object.RemoveObjectAtIndex( j );
									}

									if ( !neededammo )
										break;
								}
							}
							else
							{
								break;
							}

							if ( (Item[(pSoldier->inv[slot]).usItem].usItemClass & IC_GUN) )
								neededammo = Magazine[usMagIndex].ubMagSize - (*pObj)[i]->data.gun.ubGunShotsLeft;
							else
								neededammo = Magazine[usMagIndex].ubMagSize - (*pObj)[i]->data.ubShotsLeft;
						}
					}
				}
			}
		}

		// Immersion, let's play a reload sound
		if ( usReloadSound != 0 )
			PlayJA2Sample( usReloadSound, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		else if ( attachmentsound )
			PlayJA2Sample( ATTACH_TO_GUN, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

		// warn us if we tried to attach something we can't attach
		if ( attachmenttowarnabout != NOTHING )
		{
			ScreenMsg( color, MSG_INTERFACE, szGearTemplateText[5], Item[attachmenttowarnabout].szItemName, attachmenttowarnabout );
		}

		// 6. redraw inventory
		fTeamPanelDirty = TRUE;
		fMapPanelDirty = TRUE;
		fInterfacePanelDirty = DIRTYLEVEL2;
		fCharacterInfoPanelDirty = TRUE;

		// refresh ammo button in description box if it exists
		if ( giItemDescAmmoButton > -1 )
			MarkAButtonDirty( giItemDescAmmoButton ); // Required for tactical screen
	}
}

// Flugente: intel
void TakePhoto(SOLDIERTYPE* pSoldier, INT32 sGridNo, INT8 bLevel )
{
	if ( !pSoldier || TileIsOutOfBounds( sGridNo ) )
		return;

	// if we take a photo, take note of all rooms & NPC we see. We then send that to LUA, where we store anything worthwhile
	// later on, we can then sell worthwhile information 'gathered' this way for intel

	// to make this simple, check only tiles in a radius around the gridno we targetted
	INT16 sBaseX, sBaseY;
	ConvertGridNoToXY( sGridNo, &sBaseX, &sBaseY );

	UINT8 ubDirection = GetDirectionFromGridNo(sGridNo, pSoldier);

	// CHANGE DIRECTION AND GOTO ANIMATION NOW
	if (pSoldier->ubDirection != ubDirection)
	{
		pSoldier->flags.uiStatusFlags |= SOLDIER_LOOK_NEXT_TURNSOLDIER;//shadooow: fix for vision not updating
		pSoldier->EVENT_SetSoldierDesiredDirection(ubDirection);
		pSoldier->EVENT_SetSoldierDirection(ubDirection);
	}

	int radius = 3;

	for ( int x = -radius; x < radius; ++x )
	{
		int diff = sqrt( (FLOAT)(radius*radius - x*x) );

		for ( int y = -diff; y < diff; ++y )
		{
			INT32 newgridno = MAPROWCOLTOPOS( (sBaseY + y), ( sBaseX + x ));

			// can we see this gridno?
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, newgridno, bLevel, 0, TRUE, CALC_FROM_WANTED_DIR, TRUE ) )
			{
				// check if this is a room
				UINT16 room = NO_ROOM;
				if ( !bLevel )
					InARoom( newgridno, &room );

				// check if there is someone here
				UINT16 ubid = WhoIsThere2( newgridno, bLevel );

				LuaAddPhotoData( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, newgridno, bLevel, pSoldier->ubProfile, room, ( ubid == NOBODY ) ? NO_PROFILE : MercPtrs[ubid]->ubProfile );
			}
		}
	}

	DeductPoints( pSoldier, APBPConstants[AP_CAMERA], 0, 0 );

	// Play sound
	PlayJA2SampleFromFile( "Sounds\\camera1.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
}
