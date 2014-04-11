#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include "ai.h"
	#include "Weapons.h"
	#include "opplist.h"
	#include "Points.h"
	#include "PathAI.h"
	#include "WorldMan.h"
	#include "AIInternals.h"
	#include "Items.h"
	#include "message.h"
	#include "los.h"
	#include "assignments.h"
	#include "Soldier Functions.h"
	#include "Points.h"
	#include "GameSettings.h"
	#include "Buildings.h"
	#include "Soldier macros.h"
	#include "Render Fun.h"
	#include "strategicmap.h"
	#include "environment.h"
	#include "lighting.h"
	#include "Soldier Create.h"
	#include "SkillCheck.h" // added by SANDRO
	#include "Vehicles.h" // added by silversurfer
#endif

//////////////////////////////////////////////////////////////////////////////
// SANDRO - In this file, all APBPConstants[AP_CROUCH] and APBPConstants[AP_PRONE] were changed to GetAPsCrouch() and GetAPsProne()
//			On the bottom here, there are these functions made
//////////////////////////////////////////////////////////////////////

//
// CJC's DG->JA2 conversion notes
//
// Commented out:
//
// InWaterOrGas - gas stuff
// RoamingRange - point patrol stuff

extern UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady, BOOLEAN fHipStance );

UINT8 Urgency[NUM_STATUS_STATES][NUM_MORALE_STATES] =
{
	{URGENCY_LOW,  URGENCY_LOW,  URGENCY_LOW,  URGENCY_LOW,  URGENCY_LOW}, // green
	{URGENCY_HIGH, URGENCY_MED,  URGENCY_MED,  URGENCY_LOW,  URGENCY_LOW}, // yellow
	{URGENCY_HIGH, URGENCY_MED,  URGENCY_MED,  URGENCY_MED,  URGENCY_MED}, // red
	{URGENCY_HIGH, URGENCY_HIGH, URGENCY_HIGH, URGENCY_MED,  URGENCY_MED}  // black
};

UINT16 MovementMode[LAST_MOVEMENT_ACTION + 1][NUM_URGENCY_STATES] =
{
	{WALKING,	 WALKING,  WALKING }, // AI_ACTION_NONE

	{WALKING,  WALKING,  WALKING }, // AI_ACTION_RANDOM_PATROL
	{WALKING,  RUNNING,  RUNNING }, // AI_ACTION_SEEK_FRIEND
	{WALKING,  RUNNING,  RUNNING }, // AI_ACTION_SEEK_OPPONENT
	{RUNNING,  RUNNING,  RUNNING }, // AI_ACTION_TAKE_COVER
	{WALKING,  RUNNING,  RUNNING }, // AI_ACTION_GET_CLOSER

	{WALKING,  WALKING,  WALKING }, // AI_ACTION_POINT_PATROL,
	{WALKING,  RUNNING,  RUNNING }, // AI_ACTION_LEAVE_WATER_GAS,
	{WALKING,  SWATTING,  RUNNING }, // AI_ACTION_SEEK_NOISE,
	{RUNNING,  RUNNING,  RUNNING }, // AI_ACTION_ESCORTED_MOVE,
	{WALKING,  RUNNING,  RUNNING }, // AI_ACTION_RUN_AWAY,

	{RUNNING,  RUNNING,  RUNNING }, // AI_ACTION_KNIFE_MOVE
	{WALKING,  WALKING,  WALKING }, // AI_ACTION_APPROACH_MERC
	{RUNNING,  RUNNING,  RUNNING }, // AI_ACTION_TRACK
	{RUNNING,	 RUNNING,  RUNNING },	// AI_ACTION_EAT 
	{WALKING,	 RUNNING,  RUNNING},	// AI_ACTION_PICKUP_ITEM

	{WALKING,	 WALKING,  WALKING},	// AI_ACTION_SCHEDULE_MOVE
	{WALKING,	 WALKING,  WALKING},	// AI_ACTION_WALK
	{WALKING,	 RUNNING,  RUNNING},	// withdraw
	{WALKING,	 SWATTING,  SWATTING},	// flank left
	{WALKING,	 SWATTING,  SWATTING},	// flank right
	{RUNNING,	 RUNNING,  RUNNING},	// AI_ACTION_MOVE_TO_CLIMB
};

INT8 OKToAttack(SOLDIERTYPE * pSoldier, int target)
{
	// can't shoot yourself
	if (target == pSoldier->sGridNo)
		return(NOSHOOT_MYSELF);

	if (WaterTooDeepForAttacks(pSoldier->sGridNo))
		return(NOSHOOT_WATER);

	// make sure a weapon is in hand (FEB.8 ADDITION: tossable items are also OK)
	if (!WeaponInHand(pSoldier))
	{
		return(NOSHOOT_NOWEAPON);
	}

	// JUST PUT THIS IN ON JULY 13 TO TRY AND FIX OUT-OF-AMMO SITUATIONS

	if ( Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN)
	{
		if ( Item[pSoldier->inv[HANDPOS].usItem].cannon )
		{
			// look for another tank shell ELSEWHERE IN INVENTORY
			if ( FindLaunchable( pSoldier, pSoldier->inv[HANDPOS].usItem ) == NO_SLOT )
			//if ( !ItemHasAttachments( &(pSoldier->inv[HANDPOS]) ) )
			{
				return(NOSHOOT_NOLOAD);
			}
		}
		else if (pSoldier->inv[HANDPOS][0]->data.gun.ubGunShotsLeft == 0 /*SB*/ || 
			!(pSoldier->inv[HANDPOS][0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER) ||
			(pSoldier->IsValidSecondHandShotForReloadingPurposes( ) && 
			(pSoldier->inv[SECONDHANDPOS][0]->data.gun.ubGunShotsLeft == 0 || 
			!(pSoldier->inv[SECONDHANDPOS][0]->data.gun.ubGunState & GS_CARTRIDGE_IN_CHAMBER))))
		{
			return(NOSHOOT_NOAMMO);
		}
	}
	else if (Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_LAUNCHER)
	{
		if ( FindLaunchable( pSoldier, pSoldier->inv[HANDPOS].usItem ) == NO_SLOT )
		//if ( !ItemHasAttachments( &(pSoldier->inv[HANDPOS]) ) )
		{
			return(NOSHOOT_NOLOAD);
		}
	}

	return(TRUE);
}

BOOLEAN ConsiderProne( SOLDIERTYPE * pSoldier )
{
	INT32		sOpponentGridNo;
	INT8		bOpponentLevel;
	INT32		iRange;

	if (pSoldier->aiData.bAIMorale >= MORALE_NORMAL)
	{
		return( FALSE );
	}
	// We don't want to go prone if there is a nearby enemy
	ClosestKnownOpponent( pSoldier, &sOpponentGridNo, &bOpponentLevel );
	iRange = GetRangeFromGridNoDiff( pSoldier->sGridNo, sOpponentGridNo );
	if (iRange > 10)
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

UINT8 StanceChange( SOLDIERTYPE * pSoldier, INT16 ubAttackAPCost )
{
	// consider crouching or going prone

	if (PTR_STANDING)
	{
		if (pSoldier->bActionPoints - ubAttackAPCost >= GetAPsCrouch(pSoldier, TRUE))
		{
			if ( (pSoldier->bActionPoints - ubAttackAPCost >= GetAPsCrouch(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE)) && IsValidStance( pSoldier, ANIM_PRONE ) && ConsiderProne( pSoldier ) )
			{
				return( ANIM_PRONE );
			}
			else if ( IsValidStance( pSoldier, ANIM_CROUCH ) )
			{
				return( ANIM_CROUCH );
			}
		}
	}
	else if (PTR_CROUCHED)
	{
		if ( (pSoldier->bActionPoints - ubAttackAPCost >= GetAPsProne(pSoldier, TRUE)) && IsValidStance( pSoldier, ANIM_PRONE ) && ConsiderProne( pSoldier ) )
		{
			return( ANIM_PRONE );
		}
	}
	return( 0 );
}

UINT8 ShootingStanceChange( SOLDIERTYPE * pSoldier, ATTACKTYPE * pAttack, INT8 bDesiredDirection )
{
	// Figure out the best stance for this attack

	// We don't want to go through a lot of complex calculations here,
	// just compare the chance of the bullet hitting if we are
	// standing, crouched, or prone

	UINT16	usRealAnimState, usBestAnimState;
	INT8		bBestStanceDiff=-1;
	INT8		bLoop, bStanceNum, bStanceDiff, bAPsAfterAttack, bCurAimTime, bSetScopeMode;
	UINT32	uiChanceOfDamage, uiBestChanceOfDamage, uiCurrChanceOfDamage;
	UINT32	uiStanceBonus, uiMinimumStanceBonusPerChange = 20 - 3 * pAttack->ubAimTime;
	INT32		iRange;

	bStanceNum = 0;
	uiCurrChanceOfDamage = 0;

	bSetScopeMode = pSoldier->bScopeMode;
	pSoldier->bScopeMode = pAttack->bScopeMode;
	bAPsAfterAttack = pSoldier->bActionPoints - MinAPsToAttack( pSoldier, pAttack->sTarget, ADDTURNCOST, pAttack->ubAimTime, 1);
	pSoldier->bScopeMode = bSetScopeMode;
	if (bAPsAfterAttack < GetAPsCrouch(pSoldier, TRUE))
	{
		return( 0 );
	}
	// Unfortunately, to get this to work, we have to fake the AI guy's
	// animation state so we get the right height values
	usRealAnimState = pSoldier->usAnimState;
	usBestAnimState = pSoldier->usAnimState;
	uiBestChanceOfDamage = 0;
	iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, pAttack->sTarget );

	switch( gAnimControl[usRealAnimState].ubEndHeight )
	{
		// set a stance number comparable with our loop variable so we can easily compute
		// stance differences and thus AP cost
		case ANIM_STAND:
			bStanceNum = 0;
			break;
		case ANIM_CROUCH:
			bStanceNum = 1;
			break;
		case ANIM_PRONE:
			bStanceNum = 2;
			break;
	}
	for (bLoop = 0; bLoop < 3; bLoop++)
	{
		bStanceDiff = abs( bLoop - bStanceNum );
		if (bStanceDiff == 2 && bAPsAfterAttack < GetAPsCrouch(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE))
		{
			// can't consider this!
			continue;
		}

		switch( bLoop )
		{
			case 0:
				if ( !pSoldier->InternalIsValidStance( bDesiredDirection, ANIM_STAND ) )
				{
					continue;
				}
				pSoldier->usAnimState = STANDING;
				break;
			case 1:
				if ( !pSoldier->InternalIsValidStance( bDesiredDirection, ANIM_CROUCH ) )
				{
					continue;
				}
				pSoldier->usAnimState = CROUCHING;
				break;
			default:
				if ( !pSoldier->InternalIsValidStance( bDesiredDirection, ANIM_PRONE ) )
				{
					continue;
				}
				pSoldier->usAnimState = PRONE;
				break;
		}

		// Hack:	Assumes the cost to reach the target stance from the current stance is the same as going back.	Probably true.
		bCurAimTime = __min( bAPsAfterAttack - GetAPsToChangeStance( pSoldier, bStanceNum), pAttack->ubAimTime);
		// If can't fire at all from this stance, don't bother with the chances of hitting
		if (bCurAimTime < 0)
		{
			continue;
		}

		uiChanceOfDamage = SoldierToLocationChanceToGetThrough( pSoldier, pAttack->sTarget, pSoldier->bTargetLevel, pSoldier->bTargetCubeLevel, pAttack->ubOpponent ) * CalcChanceToHitGun( pSoldier, pAttack->sTarget, bCurAimTime, AIM_SHOT_TORSO ) / 100;
		if (uiChanceOfDamage > 0)
		{
			uiStanceBonus = 0;
			// artificially augment "chance of damage" to reflect penalty to be shot at various stances
			switch( pSoldier->usAnimState )
			{
				case CROUCHING:
					if (iRange > POINT_BLANK_RANGE + 10 * (AIM_PENALTY_TARGET_CROUCHED / 3))
					{
						uiStanceBonus = AIM_BONUS_CROUCHING;
					}
					else if (iRange > POINT_BLANK_RANGE)
					{
						// reduce chance to hit with distance to the prone/immersed target
						uiStanceBonus = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
					}
					break;
				case PRONE:
					if (iRange <= MIN_PRONE_RANGE)
					{
						// HATE being prone this close!
						uiChanceOfDamage = 0;
					}
					else //if (iRange > POINT_BLANK_RANGE)
					{
						// reduce chance to hit with distance to the prone/immersed target
						uiStanceBonus = 3 * ((iRange - POINT_BLANK_RANGE) / CELL_X_SIZE); // penalty -3%/tile
					}
					break;
				default:
					break;
			}
			// reduce stance bonus according to how much we have to change stance to get there
			//uiStanceBonus = uiStanceBonus * (4 - bStanceDiff) / 4;
			uiChanceOfDamage += uiStanceBonus;
		}

		if (bStanceDiff == 0)
		{
			uiCurrChanceOfDamage = uiChanceOfDamage;
		}
		if (uiChanceOfDamage > uiBestChanceOfDamage )
		{
			uiBestChanceOfDamage = uiChanceOfDamage;
			usBestAnimState = pSoldier->usAnimState;
			bBestStanceDiff = bStanceDiff;
		}
	}

	pSoldier->usAnimState = usRealAnimState;

	// return 0 or the best height value to be at
	if (bBestStanceDiff == 0 || ((uiBestChanceOfDamage - uiCurrChanceOfDamage) / bBestStanceDiff) < uiMinimumStanceBonusPerChange)
	{
		// better off not changing our stance!
		return( 0 );
	}
	else
	{
		return( gAnimControl[ usBestAnimState ].ubEndHeight );
	}
}


UINT16 DetermineMovementMode( SOLDIERTYPE * pSoldier, INT8 bAction )
{
	if ( pSoldier->flags.fUIMovementFast )
	{
		return( RUNNING );
	}
	else if ( CREATURE_OR_BLOODCAT( pSoldier ) )
	{
		if (pSoldier->aiData.bAlertStatus == STATUS_GREEN)
		{
			return( WALKING );
		}
		else
		{
			return( RUNNING );
		}
	}
	else if (pSoldier->ubBodyType == COW || pSoldier->ubBodyType == CROW)
	{
		return( WALKING );
	}
	else
	{
		if ( (pSoldier->aiData.fAIFlags & AI_CAUTIOUS) && (MovementMode[bAction][Urgency[pSoldier->aiData.bAlertStatus][pSoldier->aiData.bAIMorale]] == RUNNING) )
		{
			return( WALKING );
		}
		else if ( bAction == AI_ACTION_SEEK_NOISE && pSoldier->bTeam == CIV_TEAM && !IS_MERC_BODY_TYPE( pSoldier ) )
		{
			return( WALKING );
		}
		else if ( (pSoldier->ubBodyType == HATKIDCIV || pSoldier->ubBodyType == KIDCIV) && (pSoldier->aiData.bAlertStatus == STATUS_GREEN) && Random( 10 ) == 0 )
		{
			return( KID_SKIPPING );
		}
		else
		{
			return( MovementMode[bAction][Urgency[pSoldier->aiData.bAlertStatus][pSoldier->aiData.bAIMorale]] );
		}
	}
}

void NewDest(SOLDIERTYPE *pSoldier, INT32 usGridNo)
{
	// ATE: Setting sDestination? Tis does not make sense...
	//pSoldier->pathing.sDestination = usGridNo;
	BOOLEAN fSet = FALSE;

	if ( IS_MERC_BODY_TYPE( pSoldier ) && pSoldier->aiData.bAction == AI_ACTION_TAKE_COVER && (pSoldier->aiData.bOrders == DEFENSIVE || pSoldier->aiData.bOrders == CUNNINGSOLO || pSoldier->aiData.bOrders == CUNNINGAID ) && (SoldierDifficultyLevel( pSoldier ) >= 2) )
	{
		UINT16 usMovementMode;

		// getting real movement anim for someone who is going to take cover, not just considering
		usMovementMode = MovementMode[AI_ACTION_TAKE_COVER][Urgency[pSoldier->aiData.bAlertStatus][pSoldier->aiData.bAIMorale]];
		if ( usMovementMode != SWATTING )
		{
			// really want to look at path, see how far we could get on path while swatting
			if ( EnoughPoints( pSoldier, RecalculatePathCost( pSoldier, SWATTING ), 0, FALSE ) || (pSoldier->aiData.bLastAction == AI_ACTION_TAKE_COVER && pSoldier->usUIMovementMode == SWATTING ) )
			{
				pSoldier->usUIMovementMode = SWATTING;
			}
			else
			{
				pSoldier->usUIMovementMode = usMovementMode;
			}
		}
		else
		{
			pSoldier->usUIMovementMode = usMovementMode;
		}
		fSet = TRUE;
	}
	else
	{
		if ( pSoldier->bTeam == ENEMY_TEAM && pSoldier->aiData.bAlertStatus == STATUS_RED )
		{
			switch( pSoldier->aiData.bAction )
			{

				case AI_ACTION_MOVE_TO_CLIMB:
				case AI_ACTION_RUN_AWAY:
					pSoldier->usUIMovementMode = DetermineMovementMode( pSoldier, pSoldier->aiData.bAction );
					fSet = TRUE;
					break;
				default:
/*					if ( PreRandom( 5 - SoldierDifficultyLevel( pSoldier ) ) == 0 )
					{
						INT32 sClosestNoise = (INT16) MostImportantNoiseHeard( pSoldier, NULL, NULL, NULL );						
						if ( !TileIsOutOfBounds(sClosestNoise) && PythSpacesAway( pSoldier->sGridNo, sClosestNoise ) < MaxDistanceVisible() + 10 )
						{
							pSoldier->usUIMovementMode = SWATTING;
							fSet = TRUE;
						}
					}*/
					if ( !fSet )
					{
						pSoldier->usUIMovementMode = DetermineMovementMode( pSoldier, pSoldier->aiData.bAction );
						fSet = TRUE;
					}
					break;
			}

		}
		else
		{
			pSoldier->usUIMovementMode = DetermineMovementMode( pSoldier, pSoldier->aiData.bAction );
			fSet = TRUE;
		}

		if ( pSoldier->usUIMovementMode == SWATTING && !IS_MERC_BODY_TYPE( pSoldier ) )
		{
			pSoldier->usUIMovementMode = WALKING;
		}
	}

	//pSoldier->EVENT_GetNewSoldierPath( pSoldier->pathing.sDestination, pSoldier->usUIMovementMode );
	// ATE: Using this more versitile version
	// Last paramater says whether to re-start the soldier's animation
	// This should be done if buddy was paused for fNoApstofinishMove...
	pSoldier->EVENT_InternalGetNewSoldierPath( usGridNo, pSoldier->usUIMovementMode , FALSE, pSoldier->flags.fNoAPToFinishMove );

}


BOOLEAN IsActionAffordable(SOLDIERTYPE *pSoldier)
{
	INT16	bMinPointsNeeded = 0;
	INT8 bAPForStandUp = 0;
	INT8 bAPToLookAtWall = ( FindDirectionForClimbing( pSoldier, pSoldier->sGridNo, pSoldier->pathing.bLevel ) == pSoldier->ubDirection ) ? 0 : 1;

	//NumMessage("AffordableAction - Guy#",pSoldier->ubID);

	switch (pSoldier->aiData.bAction)
	{
		case AI_ACTION_NONE:                  // maintain current position & facing
			// no cost for doing nothing!
			break;

		case AI_ACTION_CHANGE_FACING:         // turn to face another direction
			bMinPointsNeeded = (INT8) GetAPsToLook( pSoldier );
			break;

		case AI_ACTION_RANDOM_PATROL:         // move towards a particular location
		case AI_ACTION_SEEK_FRIEND:           // move towards friend in trouble
		case AI_ACTION_SEEK_OPPONENT:         // move towards a reported opponent
		case AI_ACTION_TAKE_COVER:            // run for nearest cover from threat
		case AI_ACTION_GET_CLOSER:            // move closer to a strategic location
		case AI_ACTION_POINT_PATROL:          // move towards next patrol point
		case AI_ACTION_LEAVE_WATER_GAS:       // seek nearest spot of ungassed land
		case AI_ACTION_SEEK_NOISE:            // seek most important noise heard
		case AI_ACTION_ESCORTED_MOVE:         // go where told to by escortPlayer
		case AI_ACTION_RUN_AWAY:              // run away from nearby opponent(s)
		case AI_ACTION_APPROACH_MERC:
		case AI_ACTION_TRACK:
		case AI_ACTION_EAT:
		case AI_ACTION_SCHEDULE_MOVE:
		case AI_ACTION_WALK:
		case AI_ACTION_MOVE_TO_CLIMB:
			// for movement, must have enough APs to move at least 1 tile's worth
			bMinPointsNeeded = MinPtsToMove(pSoldier);
			break;

		case AI_ACTION_PICKUP_ITEM:           // grab things lying on the ground
			bMinPointsNeeded = __max( MinPtsToMove( pSoldier ), GetBasicAPsToPickupItem( pSoldier ) ); // SANDRO
			break;

		case AI_ACTION_OPEN_OR_CLOSE_DOOR:
		case AI_ACTION_UNLOCK_DOOR:
		case AI_ACTION_LOCK_DOOR:
			bMinPointsNeeded = MinPtsToMove(pSoldier);
			break;

		case AI_ACTION_DROP_ITEM:
			bMinPointsNeeded = GetBasicAPsToPickupItem( pSoldier ); // SANDRO
			break;

		case AI_ACTION_FIRE_GUN:              // shoot at nearby opponent
		case AI_ACTION_TOSS_PROJECTILE:       // throw grenade at/near opponent(s)
		case AI_ACTION_KNIFE_MOVE:            // preparing to stab adjacent opponent
		case AI_ACTION_THROW_KNIFE:
			// only FIRE_GUN currently actually pays extra turning costs!
			bMinPointsNeeded = MinAPsToAttack(pSoldier,pSoldier->aiData.usActionData,ADDTURNCOST,pSoldier->aiData.bAimTime);

#ifdef BETAVERSION
			if (ptsNeeded > pSoldier->bActionPoints)
			{
			/*
				sprintf(tempstr,"AI ERROR: %s has insufficient points for attack action %d at grid %d",
							pSoldier->name,pSoldier->aiData.bAction,pSoldier->aiData.usActionData);
				PopMessage(tempstr);
				*/
			}
#endif
			break;

		case AI_ACTION_PULL_TRIGGER:          // activate an adjacent panic trigger
			bMinPointsNeeded = APBPConstants[AP_PULL_TRIGGER];
			break;

		case AI_ACTION_USE_DETONATOR:         // grab detonator and set off bomb(s)
			bMinPointsNeeded = APBPConstants[AP_USE_REMOTE];
			break;

		case AI_ACTION_YELLOW_ALERT:          // tell friends opponent(s) heard
		case AI_ACTION_RED_ALERT:             // tell friends opponent(s) seen
		case AI_ACTION_CREATURE_CALL:				 // for now
			bMinPointsNeeded = APBPConstants[AP_RADIO];
			break;

		case AI_ACTION_CHANGE_STANCE:                // crouch
			bMinPointsNeeded = GetAPsCrouch(pSoldier, TRUE);
			break;

		case AI_ACTION_GIVE_AID:              // help injured/dying friend
			bMinPointsNeeded = 0;
			break;

		case AI_ACTION_CLIMB_ROOF:
			// SANDRO - improved this a bit
			if (pSoldier->pathing.bLevel == 0)
			{
				if( PTR_CROUCHED ) bAPForStandUp = (INT8)(GetAPsCrouch(pSoldier, TRUE));
				if( PTR_PRONE ) bAPForStandUp = GetAPsCrouch(pSoldier, TRUE) + GetAPsProne(pSoldier, TRUE);
				bMinPointsNeeded = GetAPsToClimbRoof( pSoldier, FALSE ) + bAPForStandUp + bAPToLookAtWall;
			}
			else
			{
				if( !PTR_CROUCHED ) bAPForStandUp = (INT8)(GetAPsCrouch(pSoldier, TRUE));
				bMinPointsNeeded = GetAPsToClimbRoof( pSoldier, TRUE ) + bAPForStandUp + bAPToLookAtWall;
			}
			break;

		case AI_ACTION_COWER:
		case AI_ACTION_STOP_COWERING:
		case AI_ACTION_LOWER_GUN:
		case AI_ACTION_END_COWER_AND_MOVE:
		case AI_ACTION_TRAVERSE_DOWN:
		case AI_ACTION_OFFER_SURRENDER:
			bMinPointsNeeded = 0;
			break;

		case AI_ACTION_STEAL_MOVE: // added by SANDRO
			//bMinPointsNeeded = GetAPsToStealItem( pSoldier, NULL, pSoldier->aiData.usActionData );;
			break;

		case AI_ACTION_JUMP_WINDOW:
			if((UsingNewInventorySystem() == true) && pSoldier->inv[BPACKPOCKPOS].exists() == true
				//JMich.BackpackClimb
				&& ((gGameExternalOptions.sBackpackWeightToClimb == -1) || (INT16)pSoldier->inv[BPACKPOCKPOS].GetWeightOfObjectInStack() + Item[pSoldier->inv[BPACKPOCKPOS].usItem].sBackpackWeightModifier > gGameExternalOptions.sBackpackWeightToClimb  )
				&& ((gGameExternalOptions.fUseGlobalBackpackSettings == TRUE) || (Item[pSoldier->inv[BPACKPOCKPOS].usItem].fAllowClimbing == FALSE)))
				bMinPointsNeeded = GetAPsToJumpThroughWindows( pSoldier, TRUE );
			else
				bMinPointsNeeded = GetAPsToJumpFence( pSoldier, FALSE );

			break;

		case AI_ACTION_FREE_PRISONER:
			bMinPointsNeeded = APBPConstants[AP_HANDCUFF];
			break;

		case AI_ACTION_USE_SKILL:
			bMinPointsNeeded = 10;	// TODO
			break;

		case AI_ACTION_DOCTOR:
		case AI_ACTION_DOCTOR_SELF:
			bMinPointsNeeded = 20;	// TODO
			break;

		default:
#ifdef BETAVERSION
			//NumMessage("AffordableAction - Illegal action type = ",pSoldier->aiData.bAction);
#endif
			break;
	}

	// check whether or not we can afford to do this action
	if (bMinPointsNeeded > pSoldier->bActionPoints)
	{
		return(FALSE);
	}
	else
	{
		return(TRUE);
	}
}

INT16 RandomFriendWithin(SOLDIERTYPE *pSoldier)
{
	UINT32				uiLoop;
	UINT16				usMaxDist;
	UINT8					ubFriendCount, ubFriendIDs[MAXMERCS], ubFriendID;
	UINT8				ubDirection;
	UINT8					ubDirsLeft;
	BOOLEAN				fDirChecked[8];
	BOOLEAN				fRangeRestricted = FALSE, fFound = FALSE;
	INT32				usDest, usOrigin;
	SOLDIERTYPE *	pFriend;


	// obtain maximum roaming distance from soldier's origin
	usMaxDist = RoamingRange(pSoldier,&usOrigin);

	// if our movement range is restricted

	// CJC: since RandomFriendWithin is only used in non-combat, ALWAYS restrict range.
	fRangeRestricted = TRUE;
	/*
	if (usMaxDist < MAX_ROAMING_RANGE)
	{
		fRangeRestricted = TRUE;
	}
	*/

	// if range is restricted, make sure origin is a legal gridno!
	if (fRangeRestricted && ((usOrigin < 0) || (usOrigin >= GRIDSIZE)))
	{
#ifdef BETAVERSION
		NameMessage(pSoldier,"has illegal origin, but his roaming range is restricted!",1000);
#endif
		return(FALSE);
	}

	ubFriendCount = 0;

	// build a list of the guynums of all active, eligible friendly mercs

	// go through each soldier, looking for "friends" (soldiers on same side)
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pFriend = MercSlots[ uiLoop ];

		// if this merc is inactive, not in sector, or dead
		if (!pFriend)
		{
			continue;
		}

		// skip ourselves
		if (pFriend->ubID == pSoldier->ubID)
		{
			continue;
		}

		// if this man not neutral, but is on my side, OR if he is neutral, but
		// so am I, then he's a "friend" for the purposes of random visitations
		if ((!pFriend->aiData.bNeutral && (pSoldier->bSide == pFriend->bSide)) ||
			(pFriend->aiData.bNeutral && pSoldier->aiData.bNeutral))
		{
			// if we're not already neighbors
			if (SpacesAway(pSoldier->sGridNo,pFriend->sGridNo) > 1)
			{
		// remember his guynum, increment friend counter
			ubFriendIDs[ubFriendCount++] = pFriend->ubID;
			}
		}
	}


	while (ubFriendCount && !fFound)
	{
		// randomly select one of the remaining friends in the list
		ubFriendID = ubFriendIDs[PreRandom(ubFriendCount)];

		// if our movement range is NOT restricted, or this friend's within range
		// use distance - 1, because there must be at least 1 tile 1 space closer
		if (!fRangeRestricted ||
			(SpacesAway(usOrigin,Menptr[ubFriendID].sGridNo) - 1) <= usMaxDist)
		{
			// should be close enough, try to find a legal->pathing.sDestination within 1 tile

			// clear dirChecked flag for all 8 directions
			for (ubDirection = 0; ubDirection < 8; ubDirection++)
			{
				fDirChecked[ubDirection] = FALSE;
			}

			ubDirsLeft = 8;

			// examine all 8 spots around 'ubFriendID'
			// keep looking while directions remain and a satisfactory one not found
			while ((ubDirsLeft--) && !fFound)
			{
				// randomly select a direction which hasn't been 'checked' yet
				do
				{
					ubDirection = (UINT8) Random(8);
				}
				while (fDirChecked[ubDirection]);

				fDirChecked[ubDirection] = TRUE;

				// determine the gridno 1 tile away from current friend in this direction
				usDest = NewGridNo(Menptr[ubFriendID].sGridNo,DirectionInc(ubDirection));

				// if that's out of bounds, ignore it & check next direction
				if (usDest == Menptr[ubFriendID].sGridNo)
				{
					continue;
				}

				// if our movement range is NOT restricted
				if (!fRangeRestricted || (SpacesAway(usOrigin,usDest) <= usMaxDist))
				{
					if (LegalNPCDestination(pSoldier,usDest,ENSURE_PATH,NOWATER, 0))
					{
						fFound = TRUE;			// found a spot
						pSoldier->aiData.usActionData = usDest;	// store this->pathing.sDestination
						pSoldier->pathing.bPathStored = TRUE;	// optimization - Ian
						break;					// stop checking in other directions
					}
				}
			}
		}

		if (!fFound)
		{
			ubFriendCount--;

			// if we hadn't already picked the last friend currently in the list
			if (ubFriendCount != ubFriendID)
			{
				ubFriendIDs[ubFriendID] = ubFriendIDs[ubFriendCount];
			}
		}
	}

	return(fFound);
}


INT32 RandDestWithinRange(SOLDIERTYPE *pSoldier)
{
	INT32 sRandDest = NOWHERE;
	INT32 usOrigin, usMaxDist;
	UINT8	ubTriesLeft;
	BOOLEAN fLimited = FALSE, fFound = FALSE;
	INT16 sMaxLeft, sMaxRight, sMaxUp, sMaxDown, sXRange, sYRange, sXOffset, sYOffset;
	INT16 sOrigX, sOrigY;
	INT16 sX, sY;
	//DBrot: More Rooms
	//UINT8	ubRoom = 0, ubTempRoom;
	UINT16 usRoom = 0, usTempRoom;
	sOrigX = sOrigY = -1;
	sMaxLeft = sMaxRight = sMaxUp = sMaxDown = sXRange = sYRange = -1;

	// Try to find a random->pathing.sDestination that's no more than maxDist away from
	// the given gridno of origin

	if (gfTurnBasedAI)
	{
		ubTriesLeft = 10;
	}
	else
	{
		ubTriesLeft = 1;
	}

	usMaxDist = RoamingRange(pSoldier,&usOrigin);

	if ( pSoldier->aiData.bOrders <= CLOSEPATROL && (pSoldier->bTeam == CIV_TEAM || pSoldier->ubProfile != NO_PROFILE ) )
	{
		// any other combo uses the default of ubRoom == 0, set above
		if ( !InARoom( pSoldier->aiData.sPatrolGrid[0], &usRoom ) )
		{
			usRoom = 0;
		}
	}

	// if the maxDist is truly a restriction
	if (usMaxDist < (MAXCOL - 1))
	{
		fLimited = TRUE;

		// determine maximum horizontal limits
		sOrigX = usOrigin % MAXCOL;
		sOrigY = usOrigin / MAXCOL;

		sMaxLeft	= min(usMaxDist, sOrigX);
		sMaxRight = min(usMaxDist,MAXCOL - (sOrigX + 1));

		// determine maximum vertical limits
		sMaxUp	= min(usMaxDist, sOrigY);
		sMaxDown = min(usMaxDist,MAXROW - (sOrigY + 1));

		sXRange = sMaxLeft + sMaxRight + 1;
		sYRange = sMaxUp + sMaxDown + 1;
	}

	if (pSoldier->ubBodyType == LARVAE_MONSTER)
	{
		// only crawl 1 tile, within our roaming range
		while ((ubTriesLeft--) && !fFound)
		{
			sXOffset = (INT16) Random( 3 ) - 1; // generates -1 to +1
			sYOffset = (INT16) Random( 3 ) - 1;

			if (fLimited)
			{
				sX = pSoldier->sGridNo % MAXCOL + sXOffset;
				sY = pSoldier->sGridNo / MAXCOL + sYOffset;
				if (sX < sOrigX - sMaxLeft || sX > sOrigX + sMaxRight)
				{
					continue;
				}
				if (sY < sOrigY - sMaxUp || sY > sOrigY + sMaxDown)
				{
					continue;
				}
				sRandDest = usOrigin + sXOffset + (MAXCOL * sYOffset);
			}
			else
			{
				sRandDest = usOrigin + sXOffset + (MAXCOL * sYOffset);
			}

			if (!LegalNPCDestination(pSoldier,sRandDest,ENSURE_PATH,NOWATER,0))
			{
				sRandDest = NOWHERE;
				continue;					// try again!
			}

			// passed all the tests,->pathing.sDestination is acceptable
			fFound = TRUE;
			pSoldier->pathing.bPathStored = TRUE;	// optimization - Ian
		}
	}
	else
	{
		// keep rolling random->pathing.sDestinations until one's satisfactory or retries used
		while ((ubTriesLeft--) && !fFound)
		{
			if (fLimited)
			{
				UINT8 ubTriesLeft2 = 128;
				do//dnl ch53 111009 This loop should increase search performance, but probably need some counter to prevent eventual endless loop
				{
					sXOffset = ((INT16)Random(sXRange)) - sMaxLeft;
					sYOffset = ((INT16)Random(sYRange)) - sMaxUp;
					sRandDest = usOrigin + sXOffset + (MAXCOL * sYOffset);
				}while(!GridNoOnVisibleWorldTile(sRandDest) && --ubTriesLeft2);
	#ifdef BETAVERSION
				if ((sRandDest < 0) || (sRandDest >= GRIDSIZE))
				{
					NumMessage("RandDestWithinRange: ERROR - Gridno out of range! = ",sRandDest);
					sRandDest = random(GRIDSIZE);
				}
	#endif
			}
			else
			{
				UINT8 ubTriesLeft2 = 128;
				do//dnl ch53 111009 This loop should increase search performance, but probably need some counter to prevent eventual endless loop
				{
					sRandDest = PreRandom(GRIDSIZE);
				}while(!GridNoOnVisibleWorldTile(sRandDest) && --ubTriesLeft2);
			}

			if ( usRoom && InARoom( sRandDest, &usTempRoom ) && usTempRoom != usRoom )
			{
				// outside of room available for patrol!
				sRandDest = NOWHERE;
				continue;
			}

			if (!LegalNPCDestination(pSoldier,sRandDest,ENSURE_PATH,NOWATER,0))
			{
				sRandDest = NOWHERE;
				continue;					// try again!
			}

			// passed all the tests,->pathing.sDestination is acceptable
			fFound = TRUE;
			pSoldier->pathing.bPathStored = TRUE;	// optimization - Ian
		}
	}

	return(sRandDest); // defaults to NOWHERE
}

INT32 ClosestReachableDisturbance(SOLDIERTYPE *pSoldier, UINT8 ubUnconsciousOK, BOOLEAN * pfChangeLevel )
{
	INT32		*psLastLoc, *pusNoiseGridNo;
	INT8		*pbLastLevel;
	INT32 sGridNo=-1;
	INT8		bLevel, bClosestLevel = -1;
	BOOLEAN	fClimbingNecessary, fClosestClimbingNecessary = FALSE;
	INT32		iPathCost;
	INT32		sClosestDisturbance = NOWHERE;
	UINT32	uiLoop;
	UINT32	closestConscious = NOWHERE,closestUnconscious = NOWHERE;
	INT32		iShortestPath = 1000;
	INT32		iShortestPathConscious = 1000,iShortestPathUnconscious = 1000;
	UINT8		*pubNoiseVolume;
	INT8		*pbNoiseLevel;
	INT8		*pbPersOL,*pbPublOL;
	INT32		sClimbGridNo;
	SOLDIERTYPE * pOpp;

	// CJC: can't trace a path to every known disturbance!
	// for starters, try the closest one as the crow flies
	INT32		sClosestEnemy = NOWHERE, sDistToClosestEnemy = 1000, sDistToEnemy;

	*pfChangeLevel = FALSE;

	pubNoiseVolume = &gubPublicNoiseVolume[pSoldier->bTeam];
	pusNoiseGridNo = &gsPublicNoiseGridNo[pSoldier->bTeam];
	pbNoiseLevel = &gbPublicNoiseLevel[pSoldier->bTeam];

	// hang pointers at start of this guy's personal and public opponent opplists
//	pbPersOL = &pSoldier->aiData.bOppList[0];
//	pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);
//	psLastLoc = &(gsLastKnownOppLoc[pSoldier->ubID][0]);

	// look through this man's personal & public opplists for opponents known
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpp = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpp)
		{
			continue;			// next merc
		}

		// if this merc is neutral/on same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpp ) || (pSoldier->bSide == pOpp->bSide) )
		{
			continue;			// next merc
		}

		pbPersOL = pSoldier->aiData.bOppList + pOpp->ubID;
		pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpp->ubID;
		psLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pOpp->ubID;
		pbLastLevel = gbLastKnownOppLevel[pSoldier->ubID] + pOpp->ubID;

		// if this opponent is unknown personally and publicly
		if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN))
		{
			continue;			// next merc
		}

		// this is possible if get here from BLACK AI in one of those rare
		// instances when we couldn't get a meaningful shot off at a guy in sight
		if ((*pbPersOL == SEEN_CURRENTLY) && (pOpp->stats.bLife >= OKLIFE))
		{
			// don't allow this to return any valid values, this guy remains a
			// serious threat and the last thing we want to do is approach him!
			return(NOWHERE);
		}

		// if personal knowledge is more up to date or at least equal
		if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
			(*pbPersOL == *pbPublOL))
		{
			// using personal knowledge, obtain opponent's "best guess" gridno
			sGridNo = *psLastLoc;
			bLevel = *pbLastLevel;
		}
		else
		{
			// using public knowledge, obtain opponent's "best guess" gridno
			sGridNo = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpp->ubID];
			bLevel = gbPublicLastKnownOppLevel[pSoldier->bTeam][pOpp->ubID];
		}

		// if we are standing at that gridno (!, obviously our info is old...)
		if (sGridNo == pSoldier->sGridNo)
		{
			continue;			// next merc
		}
		
		if (TileIsOutOfBounds(sGridNo))
		{
			// huh?
			continue;
		}

		sDistToEnemy = PythSpacesAway( pSoldier->sGridNo, sGridNo );
		if (sDistToEnemy < sDistToClosestEnemy )
		{
			sClosestEnemy = sGridNo;
			bClosestLevel = bLevel;
			sDistToClosestEnemy = sDistToEnemy;
		}

	}
	
	if (!TileIsOutOfBounds(sClosestEnemy))
	{
		iPathCost = EstimatePathCostToLocation( pSoldier, sClosestEnemy, bClosestLevel, FALSE, &fClimbingNecessary, &sClimbGridNo );
		// if we can get there
		if (iPathCost != 0)
		{
			if (fClimbingNecessary)
			{
				sClosestDisturbance = sClimbGridNo;
			}
			else
			{
				sClosestDisturbance = sClosestEnemy;
			}
			iShortestPath = iPathCost;
			fClosestClimbingNecessary = fClimbingNecessary;
		}
	}

	// if any "misc. noise" was also heard recently	
	if (!TileIsOutOfBounds(pSoldier->aiData.sNoiseGridno) && pSoldier->aiData.sNoiseGridno != sClosestDisturbance)
	{
		// test this gridno, too
		sGridNo = pSoldier->aiData.sNoiseGridno;
		bLevel = pSoldier->bNoiseLevel;

		// if we are there (at the noise gridno)
		if (sGridNo == pSoldier->sGridNo)
		{
			for(uiLoop=0; uiLoop<guiNumMercSlots; uiLoop++)//dnl ch58 160813
			{
				pOpp = MercSlots[uiLoop];
				if(pOpp && pSoldier->bSide == pOpp->bSide && pSoldier->ubID != pOpp->ubID&& pSoldier->aiData.sNoiseGridno == pOpp->aiData.sNoiseGridno)
				{
					pOpp->aiData.sNoiseGridno = NOWHERE;// Erase for all from the same team as it not useful anymore, this will avoid others to check already tested location
					pOpp->aiData.ubNoiseVolume = 0;
				}
			}
			pSoldier->aiData.sNoiseGridno = NOWHERE;		// wipe it out, not useful anymore
			pSoldier->aiData.ubNoiseVolume = 0;
		}
		else
		{
			// get the AP cost to get to the location of the noise
			iPathCost = EstimatePathCostToLocation( pSoldier, sGridNo, bLevel, FALSE, &fClimbingNecessary, &sClimbGridNo );
			// if we can get there
			if (iPathCost != 0)
			{
				if (fClimbingNecessary)
				{
					sClosestDisturbance = sClimbGridNo;
				}
				else
				{
					sClosestDisturbance = sGridNo;
				}
				iShortestPath = iPathCost;
				fClosestClimbingNecessary = fClimbingNecessary;
			}
		}
	}


	// if any PUBLIC "misc. noise" was also heard recently	
	if (!TileIsOutOfBounds(*pusNoiseGridNo) && *pusNoiseGridNo != sClosestDisturbance )
	{
		// test this gridno, too
		sGridNo = *pusNoiseGridNo;
		bLevel = *pbNoiseLevel;

		// if we are not NEAR the noise gridno...
		if ( pSoldier->pathing.bLevel != bLevel || PythSpacesAway( pSoldier->sGridNo, sGridNo ) >= 6 || SoldierTo3DLocationLineOfSightTest( pSoldier, sGridNo, bLevel, 0, FALSE, NO_DISTANCE_LIMIT ) == 0 )
		// if we are NOT there (at the noise gridno)
		//	if (sGridNo != pSoldier->sGridNo)
		{
			// get the AP cost to get to the location of the noise
			iPathCost = EstimatePathCostToLocation( pSoldier, sGridNo, bLevel, FALSE, &fClimbingNecessary, &sClimbGridNo );
			// if we can get there
			if (iPathCost != 0)
			{
				if (fClimbingNecessary)
				{
					sClosestDisturbance = sClimbGridNo;
				}
				else
				{
					sClosestDisturbance = sGridNo;
				}
				iShortestPath = iPathCost;
				fClosestClimbingNecessary = fClimbingNecessary;
			}
		}
		else
		{
			// degrade our public noise a bit
			//dnl ch58 160813
			//*pusNoiseGridNo -= 2;
			if(*pubNoiseVolume > 1)
				(*pubNoiseVolume)--;
		}
	}

#ifdef DEBUGDECISIONS	
	if (!TileIsOutOfBounds(sClosestDisturbance))
	{
		AINumMessage("CLOSEST DISTURBANCE is at gridno ",sClosestDisturbance);
	}
#endif

	*pfChangeLevel = fClosestClimbingNecessary;
	return(sClosestDisturbance);
}


INT32 ClosestKnownOpponent(SOLDIERTYPE *pSoldier, INT32 * psGridNo, INT8 * pbLevel)
{
	INT32 *psLastLoc,sGridNo, sClosestOpponent = NOWHERE;
	UINT32 uiLoop;
	INT32 iRange, iClosestRange = 1500;
	INT8	*pbPersOL,*pbPublOL;
	INT8	bLevel, bClosestLevel;
	SOLDIERTYPE * pOpp;

	bClosestLevel = -1;


	// NOTE: THIS FUNCTION ALLOWS RETURN OF UNCONSCIOUS AND UNREACHABLE OPPONENTS
	psLastLoc = &(gsLastKnownOppLoc[pSoldier->ubID][0]);

	// hang pointers at start of this guy's personal and public opponent opplists
	pbPersOL = &pSoldier->aiData.bOppList[0];
	pbPublOL = &(gbPublicOpplist[pSoldier->bTeam][0]);

	// look through this man's personal & public opplists for opponents known
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpp = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpp)
		{
			continue;			// next merc
		}

		// if this merc is neutral/on same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpp ) || (pSoldier->bSide == pOpp->bSide))
		{
			continue;			// next merc
		}

		// silversurfer: ignore empty vehicles
		if ( pOpp->ubWhatKindOfMercAmI == MERC_TYPE__VEHICLE && GetNumberInVehicle( pOpp->bVehicleID ) == 0 )
			continue;

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpp->ubProfile != SLAY)
		{
			continue;	// next opponent
		}

		pbPersOL = pSoldier->aiData.bOppList + pOpp->ubID;
		pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpp->ubID;
		psLastLoc = gsLastKnownOppLoc[pSoldier->ubID] + pOpp->ubID;

		// if this opponent is unknown personally and publicly
		if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN))
		{
			continue;			// next merc
		}

		// if personal knowledge is more up to date or at least equal
		if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) ||
			(*pbPersOL == *pbPublOL))
		{
			// using personal knowledge, obtain opponent's "best guess" gridno
			sGridNo = gsLastKnownOppLoc[pSoldier->ubID][pOpp->ubID];
			bLevel = gbLastKnownOppLevel[pSoldier->ubID][pOpp->ubID];
		}
		else
		{
			// using public knowledge, obtain opponent's "best guess" gridno
			sGridNo = gsPublicLastKnownOppLoc[pSoldier->bTeam][pOpp->ubID];
			bLevel = gbPublicLastKnownOppLevel[pSoldier->bTeam][pOpp->ubID];
		}

		// if we are standing at that gridno(!, obviously our info is old...)
		if (sGridNo == pSoldier->sGridNo)
		{
			continue;			// next merc
		}

		// this function is used only for turning towards closest opponent or changing stance
		// as such, if they AI is in a building,
		// we should ignore people who are on the roof of the same building as the AI
		if ( (bLevel != pSoldier->pathing.bLevel) && SameBuilding( pSoldier->sGridNo, sGridNo ) )
		{
			continue;
		}

		// I hope this will be good enough; otherwise we need a fractional/world-units-based 2D distance function
		//sRange = PythSpacesAway( pSoldier->sGridNo, sGridNo);
		iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

		if (iRange < iClosestRange)
		{
			iClosestRange = iRange;
			sClosestOpponent = sGridNo;
			bClosestLevel = bLevel;
		}
	}

#ifdef DEBUGDECISIONS	
	if (!TileIsOutOfBounds(sClosestOpponent))
	{
		AINumMessage("CLOSEST OPPONENT is at gridno ",sClosestOpponent);
	}
#endif

	if (psGridNo)
	{
		*psGridNo = sClosestOpponent;
	}
	if (pbLevel)
	{
		*pbLevel = bClosestLevel;
	}
	return( sClosestOpponent );
}

INT32 ClosestSeenOpponent(SOLDIERTYPE *pSoldier, INT32 * psGridNo, INT8 * pbLevel)
{
	INT32 sGridNo, sClosestOpponent = NOWHERE;
	UINT32 uiLoop;
	INT32 iRange, iClosestRange = 1500;
	INT8	*pbPersOL;
	INT8	bLevel, bClosestLevel;
	SOLDIERTYPE * pOpp;

	bClosestLevel = -1;

	// look through this man's personal & public opplists for opponents known
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpp = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpp)
		{
			continue;			// next merc
		}

		// if this merc is neutral/on same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpp ) || (pSoldier->bSide == pOpp->bSide))
		{
			continue;			// next merc
		}

		// silversurfer: ignore empty vehicles
		if ( pOpp->ubWhatKindOfMercAmI == MERC_TYPE__VEHICLE && GetNumberInVehicle( pOpp->bVehicleID ) == 0 )
			continue;

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpp->ubProfile != SLAY)
		{
			continue;	// next opponent
		}

		pbPersOL = pSoldier->aiData.bOppList + pOpp->ubID;

		// if this opponent is not seen personally
		if (*pbPersOL != SEEN_CURRENTLY)
		{
			continue;			// next merc
		}

		// since we're dealing with seen people, use exact gridnos
		sGridNo = pOpp->sGridNo;
		bLevel = pOpp->pathing.bLevel;

		// if we are standing at that gridno(!, obviously our info is old...)
		if (sGridNo == pSoldier->sGridNo)
		{
			continue;			// next merc
		}

		// this function is used only for turning towards closest opponent or changing stance
		// as such, if they AI is in a building,
		// we should ignore people who are on the roof of the same building as the AI
		if ( (bLevel != pSoldier->pathing.bLevel) && SameBuilding( pSoldier->sGridNo, sGridNo ) )
		{
			continue;
		}

		// I hope this will be good enough; otherwise we need a fractional/world-units-based 2D distance function
		//sRange = PythSpacesAway( pSoldier->sGridNo, sGridNo);
		iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

		if (iRange < iClosestRange)
		{
			iClosestRange = iRange;
			sClosestOpponent = sGridNo;
			bClosestLevel = bLevel;
		}
	}

#ifdef DEBUGDECISIONS	
	if (!TileIsOutOfBounds(sClosestOpponent))
	{
		AINumMessage("CLOSEST OPPONENT is at gridno ",sClosestOpponent);
	}
#endif

	if (psGridNo)
	{
		*psGridNo = sClosestOpponent;
	}
	if (pbLevel)
	{
		*pbLevel = bClosestLevel;
	}
	return( sClosestOpponent );
}


// special variant with a minor twist
INT32 ClosestSeenOpponentWithRoof(SOLDIERTYPE *pSoldier, INT32 * psGridNo, INT8 * pbLevel)
{
	INT32 sGridNo, sClosestOpponent = NOWHERE;
	UINT32 uiLoop;
	INT32 iRange, iClosestRange = 1500;
	INT8	*pbPersOL;
	INT8	bLevel, bClosestLevel;
	SOLDIERTYPE * pOpp;

	bClosestLevel = -1;

	// look through this man's personal & public opplists for opponents known
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pOpp = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pOpp)
		{
			continue;			// next merc
		}

		// if this merc is neutral/on same side, he's not an opponent
		if ( CONSIDERED_NEUTRAL( pSoldier, pOpp ) || (pSoldier->bSide == pOpp->bSide))
		{
			continue;			// next merc
		}

		// silversurfer: ignore empty vehicles
		if ( pOpp->ubWhatKindOfMercAmI == MERC_TYPE__VEHICLE && GetNumberInVehicle( pOpp->bVehicleID ) == 0 )
			continue;

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpp->ubProfile != SLAY)
		{
			continue;	// next opponent
		}

		pbPersOL = pSoldier->aiData.bOppList + pOpp->ubID;

		// if this opponent is not seen personally
		if (*pbPersOL != SEEN_CURRENTLY)
		{
			continue;			// next merc
		}

		// since we're dealing with seen people, use exact gridnos
		sGridNo = pOpp->sGridNo;
		bLevel = pOpp->pathing.bLevel;

		// if we are standing at that gridno(!, obviously our info is old...)
		if (sGridNo == pSoldier->sGridNo)
		{
			continue;			// next merc
		}

		// special: allow zombies to also find opponents on a roof
		// otherwise they have will never decide to climb a roof while they are seeing an enemy
		// this function is used only for turning towards closest opponent or changing stance
		// as such, if they AI is in a building,
		// we should ignore people who are on the roof of the same building as the AI
		/*if ( !pSoldier->IsZombie() && (bLevel != pSoldier->pathing.bLevel) && SameBuilding( pSoldier->sGridNo, sGridNo ) )
		{
			continue;
		}*/

		// I hope this will be good enough; otherwise we need a fractional/world-units-based 2D distance function
		//sRange = PythSpacesAway( pSoldier->sGridNo, sGridNo);
		iRange = GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, sGridNo );

		if (iRange < iClosestRange)
		{
			iClosestRange = iRange;
			sClosestOpponent = sGridNo;
			bClosestLevel = bLevel;
		}
	}

#ifdef DEBUGDECISIONS	
	if (!TileIsOutOfBounds(sClosestOpponent))
	{
		AINumMessage("CLOSEST OPPONENT is at gridno ",sClosestOpponent);
	}
#endif

	if (psGridNo)
	{
		*psGridNo = sClosestOpponent;
	}
	if (pbLevel)
	{
		*pbLevel = bClosestLevel;
	}
	return( sClosestOpponent );
}

INT32 ClosestPC( SOLDIERTYPE *pSoldier, INT32 * psDistance )
{
	// used by NPCs... find the closest PC

	// NOTE: skips EPCs!

	UINT8 ubLoop;
	SOLDIERTYPE		*pTargetSoldier;
	INT32					sMinDist = WORLD_MAX;
	INT32					sDist;
	INT32					sGridNo = NOWHERE;

	// Loop through all mercs on player team
	ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	for ( ; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++)
	{
		pTargetSoldier = Menptr + ubLoop;

		if (!pTargetSoldier->bActive || !pTargetSoldier->bInSector)
		{
			continue;
		}

		// if not conscious, skip him
		if (pTargetSoldier->stats.bLife < OKLIFE)
		{
		continue;
		}

		if ( AM_AN_EPC( pTargetSoldier ) )
		{
			continue;
		}

		sDist = PythSpacesAway(pSoldier->sGridNo,pTargetSoldier->sGridNo);

		// if this PC is not visible to the soldier, then add a penalty to the distance
		// so that we weight in favour of visible mercs
		if ( pTargetSoldier->bTeam != pSoldier->bTeam && pSoldier->aiData.bOppList[ ubLoop ] != SEEN_CURRENTLY )
		{
			sDist += 10;
		}

		if (sDist < sMinDist)
		{
			sMinDist = sDist;
			sGridNo = pTargetSoldier->sGridNo;
		}
	}

	if ( psDistance )
	{
		*psDistance = sMinDist;
	}

	return( sGridNo );
}

// Flugente: like ClosestPC(...), but does not account for covert or not visible mercs
INT32 ClosestUnDisguisedPC( SOLDIERTYPE *pSoldier, INT32 * psDistance )
{
	// used by NPCs... find the closest PC
	// NOTE: skips EPCs!

	UINT8 ubLoop;
	SOLDIERTYPE		*pTargetSoldier;
	INT32					sMinDist = WORLD_MAX;
	INT32					sDist;
	INT32					sGridNo = NOWHERE;

	// Loop through all mercs on player team
	ubLoop = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
	for ( ; ubLoop <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; ubLoop++)
	{
		pTargetSoldier = Menptr + ubLoop;

		if (!pTargetSoldier->bActive || !pTargetSoldier->bInSector)
			continue;
				
		// if not conscious, skip him
		if (pTargetSoldier->stats.bLife < OKLIFE)
			continue;

		if ( AM_AN_EPC( pTargetSoldier ) )
			continue;

		if ( pTargetSoldier->bSoldierFlagMask & (SOLDIER_COVERT_CIV|SOLDIER_COVERT_SOLDIER) )
			continue;

		sDist = PythSpacesAway(pSoldier->sGridNo,pTargetSoldier->sGridNo);

		// if this PC is not visible to the soldier, then add a penalty to the distance
		// so that we weight in favour of visible mercs
		if ( pTargetSoldier->bTeam != pSoldier->bTeam && pSoldier->aiData.bOppList[ ubLoop ] != SEEN_CURRENTLY )
			continue;

		if (sDist < sMinDist)
		{
			sMinDist = sDist;
			sGridNo = pTargetSoldier->sGridNo;
		}
	}

	if ( psDistance )
	{
		*psDistance = sMinDist;
	}

	return( sGridNo );
}

INT32 FindClosestClimbPointAvailableToAI( SOLDIERTYPE * pSoldier, INT32 sStartGridNo, INT32 sDesiredGridNo, BOOLEAN fClimbUp )
{
	INT32 sGridNo;
	INT32	sRoamingOrigin;
	INT16	sRoamingRange;

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_PC )
	{
		sRoamingOrigin = pSoldier->sGridNo;
		sRoamingRange = 99;
	}
	else
	{
		sRoamingRange = RoamingRange( pSoldier, &sRoamingOrigin );
	}

	// since climbing necessary involves going an extra tile, we compare against 1 less than the roam range...
	// or add 1 to the distance to the climb point

	sGridNo = FindClosestClimbPoint( pSoldier, sStartGridNo, sDesiredGridNo, fClimbUp );


	if ( PythSpacesAway( sRoamingOrigin, sGridNo ) + 1 > sRoamingRange )
	{
		return( NOWHERE );
	}
	else
	{
		return( sGridNo );
	}
}

BOOLEAN ClimbingNecessary( SOLDIERTYPE * pSoldier, INT32 sDestGridNo, INT8 bDestLevel )
{
	if (pSoldier->pathing.bLevel == bDestLevel)
	{
		if ( (pSoldier->pathing.bLevel == 0) || ( gubBuildingInfo[ pSoldier->sGridNo ] == gubBuildingInfo[ sDestGridNo ] ) )
		{
			return( FALSE );
		}
		else // different buildings!
		{
			return( TRUE );
		}
	}
	else
	{
		return( TRUE );
	}
}

INT32 GetInterveningClimbingLocation( SOLDIERTYPE * pSoldier, INT32 sDestGridNo, INT8 bDestLevel, BOOLEAN * pfClimbingNecessary )
{
	if (pSoldier->pathing.bLevel == bDestLevel)
	{
		if ( (pSoldier->pathing.bLevel == 0) || ( gubBuildingInfo[ pSoldier->sGridNo ] == gubBuildingInfo[ sDestGridNo ] ) )
		{
			// on ground or same building... normal!
			*pfClimbingNecessary = FALSE;
			return( NOWHERE );
		}
		else
		{
			// different buildings!
			// yes, pass in same gridno twice... want closest climb-down spot for building we are on!
			*pfClimbingNecessary = TRUE;
			return( FindClosestClimbPointAvailableToAI( pSoldier, pSoldier->sGridNo, pSoldier->sGridNo, FALSE ) );
		}
	}
	else
	{
		*pfClimbingNecessary = TRUE;
		// different levels
		if (pSoldier->pathing.bLevel == 0)
		{
			// got to go UP onto building
			return( FindClosestClimbPointAvailableToAI( pSoldier, pSoldier->sGridNo, sDestGridNo, TRUE ) );
		}
		else
		{
			// got to go DOWN off building
			return( FindClosestClimbPointAvailableToAI( pSoldier, pSoldier->sGridNo, pSoldier->sGridNo, FALSE ) );
		}
	}
}

INT16 EstimatePathCostToLocation( SOLDIERTYPE * pSoldier, INT32 sDestGridNo, INT8 bDestLevel, BOOLEAN fAddCostAfterClimbingUp, BOOLEAN * pfClimbingNecessary, INT32 * psClimbGridNo )
{
	INT16	sPathCost;
	INT32 sClimbGridNo;

	if (pSoldier->pathing.bLevel == bDestLevel)
	{
		if ( (pSoldier->pathing.bLevel == 0) || ( gubBuildingInfo[ pSoldier->sGridNo ] == gubBuildingInfo[ sDestGridNo ] ) )
		{
			// on ground or same building... normal!
			sPathCost = EstimatePlotPath( pSoldier, sDestGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0);
			*pfClimbingNecessary = FALSE;
			*psClimbGridNo = NOWHERE;
		}
		else
		{
			// different buildings!
			// yes, pass in same gridno twice... want closest climb-down spot for building we are on!
			sClimbGridNo = FindClosestClimbPointAvailableToAI( pSoldier, sDestGridNo, pSoldier->sGridNo, FALSE );			
			if (TileIsOutOfBounds(sClimbGridNo))
			{
				sPathCost = 0;
			}
			else
			{
				sPathCost = PlotPath( pSoldier, sClimbGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0 );
				if (sPathCost != 0)
				{
					// add in cost of climbing down
					if (fAddCostAfterClimbingUp)
					{
						// add in cost of later climbing up, too
						sPathCost += APBPConstants[AP_CLIMBOFFROOF] + APBPConstants[AP_CLIMBROOF];
						// add in an estimate of getting there after climbing down
						sPathCost += (APBPConstants[AP_MOVEMENT_FLAT] + APBPConstants[AP_MODIFIER_WALK]) * PythSpacesAway( sClimbGridNo, sDestGridNo );
					}
					else
					{
						sPathCost += APBPConstants[AP_CLIMBOFFROOF];
						// add in an estimate of getting there after climbing down, *but not on top of roof*
						sPathCost += (APBPConstants[AP_MOVEMENT_FLAT] + APBPConstants[AP_MODIFIER_WALK]) * PythSpacesAway( sClimbGridNo, sDestGridNo ) / 2;
					}
					*pfClimbingNecessary = TRUE;
					*psClimbGridNo = sClimbGridNo;
				}
			}
		}
	}
	else
	{
		// different levels
		if (pSoldier->pathing.bLevel == 0)
		{
			//got to go UP onto building
			sClimbGridNo = FindClosestClimbPointAvailableToAI( pSoldier,	pSoldier->sGridNo, sDestGridNo, TRUE );
		}
		else
		{
			// got to go DOWN off building
			sClimbGridNo = FindClosestClimbPointAvailableToAI( pSoldier, sDestGridNo, pSoldier->sGridNo, FALSE );
		}
		
		if (TileIsOutOfBounds(sClimbGridNo))
		{
			sPathCost = 0;
		}
		else
		{
			sPathCost = PlotPath( pSoldier, sClimbGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0);
			if (sPathCost != 0)
			{
				// add in the cost of climbing up or down
				if (pSoldier->pathing.bLevel == 0)
				{
					// must climb up
					sPathCost += APBPConstants[AP_CLIMBROOF];
					if (fAddCostAfterClimbingUp)
					{
						// add to path a rough estimate of how far to go from the climb gridno to the friend
						// estimate walk cost
						sPathCost += (APBPConstants[AP_MOVEMENT_FLAT] + APBPConstants[AP_MODIFIER_WALK]) * PythSpacesAway( sClimbGridNo, sDestGridNo );
					}
				}
				else
				{
					// must climb down
					sPathCost += APBPConstants[AP_CLIMBOFFROOF];
					// add to path a rough estimate of how far to go from the climb gridno to the friend
					// estimate walk cost
					sPathCost += (APBPConstants[AP_MOVEMENT_FLAT] + APBPConstants[AP_MODIFIER_WALK]) * PythSpacesAway( sClimbGridNo, sDestGridNo );
				}
				*pfClimbingNecessary = TRUE;
				*psClimbGridNo = sClimbGridNo;
			}
		}
	}

	return( sPathCost );
}

BOOLEAN GuySawEnemyThisTurnOrBefore( SOLDIERTYPE * pSoldier )
{
	UINT8		ubTeamLoop;
	UINT8		ubIDLoop;

	for ( ubTeamLoop = 0; ubTeamLoop < MAXTEAMS; ubTeamLoop++ )
	{
		if(!gTacticalStatus.Team[ubTeamLoop].bTeamActive)//dnl ch58 070913 skip any inactive teams
			continue;
		if ( gTacticalStatus.Team[ ubTeamLoop ].bSide != pSoldier->bSide )
		{
			// consider guys in this team, which isn't on our side
			for ( ubIDLoop = gTacticalStatus.Team[ ubTeamLoop ].bFirstID; ubIDLoop <= gTacticalStatus.Team[ ubTeamLoop ].bLastID; ubIDLoop++ )
			{
				// if this guy SAW an enemy recently...
				if ( pSoldier->aiData.bOppList[ ubIDLoop ] >= SEEN_CURRENTLY )
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}

INT32 ClosestReachableFriendInTrouble(SOLDIERTYPE *pSoldier, BOOLEAN * pfClimbingNecessary)
{
	UINT32 uiLoop;
	INT32 sPathCost, sClosestFriend = NOWHERE, sShortestPath = 1000, sClimbGridNo;
	BOOLEAN fClimbingNecessary, fClosestClimbingNecessary = FALSE;
	SOLDIERTYPE *pFriend;

	// civilians don't really have any "friends", so they don't bother with this
	if (PTR_CIVILIAN)
	{
		return(NOWHERE);
	}

	// consider every friend of this soldier (locations assumed to be known)
	for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
	{
		pFriend = MercSlots[ uiLoop ];

		// if this merc is inactive, at base, on assignment, or dead
		if (!pFriend)
		{
			continue;			// next merc
		}

		// if this merc is neutral or NOT on the same side, he's not a friend
		if (pFriend->aiData.bNeutral || (pSoldier->bSide != pFriend->bSide))
		{
			continue;			// next merc
		}

		// if this "friend" is actually US
		if (pFriend->ubID == pSoldier->ubID)
		{
			continue;			// next merc
		}

		// CJC: restrict "last one to radio" to only if that guy saw us this turn or last turn

		// if this friend is not under fire, and isn't the last one to radio
		if ( ! ( pFriend->aiData.bUnderFire || (pFriend->ubID == gTacticalStatus.Team[pFriend->bTeam].ubLastMercToRadio && GuySawEnemyThisTurnOrBefore( pFriend ) ) ) )
		{
			continue;			// next merc
		}

		// if we're already neighbors
		if (SpacesAway(pSoldier->sGridNo,pFriend->sGridNo) == 1)
		{
			continue;			// next merc
		}

		// get the AP cost to go to this friend's gridno
		sPathCost = EstimatePathCostToLocation( pSoldier, pFriend->sGridNo, pFriend->pathing.bLevel, TRUE, &fClimbingNecessary, &sClimbGridNo );

		// if we can get there
		if (sPathCost != 0)
		{
			//sprintf(tempstr,"Path cost to friend %s's location is %d",pFriend->name,pathCost);
			//PopMessage(tempstr);

			if (sPathCost < sShortestPath)
			{
				if (fClimbingNecessary)
				{
					sClosestFriend = sClimbGridNo;
				}
				else
				{
					sClosestFriend = pFriend->sGridNo;
				}

				sShortestPath = sPathCost;
				fClosestClimbingNecessary = fClimbingNecessary;
			}
		}
	}


#ifdef DEBUGDECISIONS	
	if (!TileIsOutOfBounds(sClosestFriend))
	{
		AINumMessage("CLOSEST FRIEND is at gridno ",sClosestFriend);
	}
#endif

	*pfClimbingNecessary = fClosestClimbingNecessary;
	return(sClosestFriend);
}

INT16 DistanceToClosestFriend( SOLDIERTYPE * pSoldier )
{
	// find the distance to the closest person on the same team
	UINT8 ubLoop;
	SOLDIERTYPE		*pTargetSoldier;
	INT16					sMinDist = 1000;
	INT16					sDist;

	// Loop through all mercs on player team
	ubLoop = gTacticalStatus.Team[ pSoldier->bTeam ].bFirstID;

	for ( ; ubLoop <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; ubLoop++ )
	{
		if (ubLoop == pSoldier->ubID)
		{
			// same guy - continue!
			continue;
		}

		pTargetSoldier = Menptr + ubLoop;

		if ( pSoldier->bActive && pSoldier->bInSector )
		{
			if (!pTargetSoldier->bActive || !pTargetSoldier->bInSector)
			{
				continue;
			}
			// if not conscious, skip him
			else if (pTargetSoldier->stats.bLife < OKLIFE)
			{
				continue;
			}
		}
		else
		{
			// compare sector #s
			if ( (pSoldier->sSectorX != pTargetSoldier->sSectorX) ||
				(pSoldier->sSectorY != pTargetSoldier->sSectorY) ||
				(pSoldier->bSectorZ != pTargetSoldier->bSectorZ) )
			{
				continue;
			}
			else if (pTargetSoldier->stats.bLife < OKLIFE)
			{
				continue;
			}
			else
			{
				// well there's someone who could be near
				return( 1 );
			}
		}

		sDist = SpacesAway(pSoldier->sGridNo,pTargetSoldier->sGridNo);

		if (sDist < sMinDist)
		{
			sMinDist = sDist;
		}
	}

	return( sMinDist );
}

BOOLEAN InWaterGasOrSmoke( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	if (WaterTooDeepForAttacks( sGridNo ))
	{
		return(TRUE);
	}

	// smoke
	if (gpWorldLevelData[sGridNo].ubExtFlags[ pSoldier->pathing.bLevel ] & MAPELEMENT_EXT_SMOKE)
	{
		return TRUE;
	}

	return InGas( pSoldier, sGridNo );
}

BOOLEAN InGasOrSmoke( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{
	// smoke
	if (gpWorldLevelData[sGridNo].ubExtFlags[ pSoldier->pathing.bLevel ] & (MAPELEMENT_EXT_SMOKE|MAPELEMENT_EXT_SIGNAL_SMOKE) )	
		return TRUE;

	return InGas(pSoldier,sGridNo);
}


INT16 InWaterOrGas(SOLDIERTYPE *pSoldier, INT32 sGridNo)
{
	if (WaterTooDeepForAttacks( sGridNo ))
	{
		return(TRUE);
	}

	return (INT16)InGas( pSoldier, sGridNo );
}

BOOLEAN InGas( SOLDIERTYPE *pSoldier, INT32 sGridNo )
{//WarmSteel - One square away from gas is still considered in gas, because it could expand any moment.
	//Note: this only works for gas that expands with one tile, but hey it's better than nothing!
	int iNeighbourGridNo;
	for(int iDir = 0; iDir < NUM_WORLD_DIRECTIONS; ++iDir)
 	{
		iNeighbourGridNo = sGridNo + DirectionInc(iDir);
		if(!TileIsOutOfBounds(iNeighbourGridNo))
		{
			// tear/mustard gas
			if((gpWorldLevelData[iNeighbourGridNo].ubExtFlags[pSoldier->pathing.bLevel] & (MAPELEMENT_EXT_TEARGAS|MAPELEMENT_EXT_MUSTARDGAS)) && !DoesSoldierWearGasMask(pSoldier))//dnl ch40 200909
			{
				return(TRUE);
			}
			// fire/creature gas
			if(gpWorldLevelData[iNeighbourGridNo].ubExtFlags[pSoldier->pathing.bLevel] & (MAPELEMENT_EXT_BURNABLEGAS|MAPELEMENT_EXT_CREATUREGAS))//dnl ch62 240813
			{
				return(TRUE);
			}
		}
	}

	return(FALSE);
}

BOOLEAN WearGasMaskIfAvailable( SOLDIERTYPE * pSoldier )
{
	INT8		bSlot, bNewSlot;

	bSlot = FindGasMask( pSoldier );
	if ( bSlot == NO_SLOT )
	{
		return( FALSE );
	}
	if ( bSlot == HEAD1POS || bSlot == HEAD2POS || bSlot == HELMETPOS )
	{
		return( FALSE );
	}
	if ( pSoldier->inv[ HEAD1POS ].exists() == false )
	{
		bNewSlot = HEAD1POS;
	}
	else if ( pSoldier->inv[ HEAD2POS ].exists() == false )
	{
		bNewSlot = HEAD2POS;
	}
	else
	{
		// screw it, going in position 1 anyhow
		bNewSlot = HEAD1POS;
	}

	RearrangePocket( pSoldier, bSlot, bNewSlot, TRUE );
	return( TRUE );
}

BOOLEAN InLightAtNight( INT32 sGridNo, INT8 bLevel )
{
	UINT8 ubBackgroundLightLevel;

	// do not consider us to be "in light" if we're in an underground sector
	if ( gbWorldSectorZ > 0 )
	{
		return( FALSE );
	}

	ubBackgroundLightLevel = GetTimeOfDayAmbientLightLevel();

	if ( ubBackgroundLightLevel < NORMAL_LIGHTLEVEL_DAY + 2 )
	{
		// don't consider it nighttime, too close to daylight levels
		return( FALSE );
	}

	// could've been placed here, ignore the light
	if ( InARoom( sGridNo, NULL ) )
	{
		return( FALSE );
	}

	// NB light levels are backwards, so a lower light level means the
	// spot in question is BRIGHTER

	if ( LightTrueLevel( sGridNo, bLevel ) < ubBackgroundLightLevel )
	{
		return( TRUE );
	}

	return( FALSE );
}

INT8 CalcMorale(SOLDIERTYPE *pSoldier)
{
 UINT32 uiLoop, uiLoop2;
 INT32 iOurTotalThreat = 0, iTheirTotalThreat = 0;
 INT16 sOppThreatValue, sFrndThreatValue, sMorale;
 INT32 iPercent;
 INT8	bMostRecentOpplistValue;
 INT8 bMoraleCategory;
 UINT8 *pSeenOpp; //,*friendOlPtr;
 INT8	*pbPersOL, *pbPublOL;
 SOLDIERTYPE *pOpponent,*pFriend;

 // if army guy has NO weapons left then panic!
 if ( pSoldier->bTeam == ENEMY_TEAM )
 {
	if ( FindAIUsableObjClass( pSoldier, IC_WEAPON ) == NO_SLOT )
	{
		return( MORALE_HOPELESS );
	}
 }

 // hang pointers to my personal opplist, my team's public opplist, and my
 // list of previously seen opponents
 pSeenOpp	= (UINT8 *) &(gbSeenOpponents[pSoldier->ubID][0]);

 // loop through every one of my possible opponents
 for (uiLoop = 0; uiLoop < guiNumMercSlots; uiLoop++)
 {
	pOpponent = MercSlots[ uiLoop ];

	// if this merc is inactive, at base, on assignment, dead, unconscious
	if (!pOpponent || (pOpponent->stats.bLife < OKLIFE))
	 continue;			// next merc

	// if this merc is neutral/on same side, he's not an opponent, skip him!
	if ( CONSIDERED_NEUTRAL( pSoldier, pOpponent ) || (pSoldier->bSide == pOpponent->bSide))
	 continue;			// next merc

		// Special stuff for Carmen the bounty hunter
		if (pSoldier->aiData.bAttitude == ATTACKSLAYONLY && pOpponent->ubProfile != SLAY)
		{
			continue;	// next opponent
		}

	pbPersOL = pSoldier->aiData.bOppList + pOpponent->ubID;
	pbPublOL = gbPublicOpplist[pSoldier->bTeam] + pOpponent->ubID;
	pSeenOpp = (UINT8 *)gbSeenOpponents[pSoldier->ubID] + pOpponent->ubID;

	// if this opponent is unknown to me personally AND unknown to my team, too
	if ((*pbPersOL == NOT_HEARD_OR_SEEN) && (*pbPublOL == NOT_HEARD_OR_SEEN))
	{
	 // if I have never seen him before anywhere in this sector, either
	 if (!(*pSeenOpp))
		continue;		// next merc

	 // have seen him in the past, so he remains something of a threat
	 bMostRecentOpplistValue = 0;		// uses the free slot for 0 opplist
	}
	else		 // decide which opplist is more current
	{
	 // if personal knowledge is more up to date or at least equal
	 if ((gubKnowledgeValue[*pbPublOL - OLDEST_HEARD_VALUE][*pbPersOL - OLDEST_HEARD_VALUE] > 0) || (*pbPersOL == *pbPublOL))
		bMostRecentOpplistValue = *pbPersOL;		// use personal
	 else
		bMostRecentOpplistValue = *pbPublOL;		// use public
	}

	iPercent = ThreatPercent[bMostRecentOpplistValue - OLDEST_HEARD_VALUE];

	sOppThreatValue = (iPercent * CalcManThreatValue(pOpponent,pSoldier->sGridNo,FALSE,pSoldier)) / 100;

	//sprintf(tempstr,"Known opponent %s, opplist status %d, percent %d, threat = %d",
	//			ExtMen[pOpponent->ubID].name,ubMostRecentOpplistValue,ubPercent,sOppThreatValue);
	//PopMessage(tempstr);

	// ADD this to their running total threatValue (decreases my MORALE)
	iTheirTotalThreat += sOppThreatValue;
	//NumMessage("Their TOTAL threat now = ",sTheirTotalThreat);

	// NOW THE FUN PART: SINCE THIS OPPONENT IS KNOWN TO ME IN SOME WAY,
	// ANY FRIENDS OF MINE THAT KNOW ABOUT HIM BOOST MY MORALE.	SO, LET'S GO
	// THROUGH THEIR PERSONAL OPPLISTS AND CHECK WHICH OF MY FRIENDS KNOW
	// SOMETHING ABOUT HIM AND WHAT THEIR THREAT VALUE TO HIM IS.

	for (uiLoop2 = 0; uiLoop2 < guiNumMercSlots; uiLoop2++)
	{
	 pFriend = MercSlots[ uiLoop2 ];

	 // if this merc is inactive, at base, on assignment, dead, unconscious
	 if (!pFriend || (pFriend->stats.bLife < OKLIFE))
		continue;		// next merc

	 // if this merc is not on my side, then he's NOT one of my friends

	 // WE CAN'T AFFORD TO CONSIDER THE ENEMY OF MY ENEMY MY FRIEND, HERE!
	 // ONLY IF WE ARE ACTUALLY OFFICIALLY CO-OPERATING TOGETHER (SAME SIDE)
	 if ( pFriend->aiData.bNeutral && !( pSoldier->ubCivilianGroup != NON_CIV_GROUP && pSoldier->ubCivilianGroup == pFriend->ubCivilianGroup ) )
		{
			continue;		// next merc
		}

		if ( pSoldier->bSide != pFriend->bSide )
		continue;		// next merc

	 // THIS TEST IS INVALID IF A COMPUTER-TEAM IS PLAYING CO-OPERATIVELY
	 // WITH A NON-COMPUTER TEAM SINCE THE OPPLISTS INVOLVED ARE NOT
	 // UP-TO-DATE.	THIS SITUATION IS CURRENTLY NOT POSSIBLE IN HTH/DG.

	 // ALSO NOTE THAT WE COUNT US AS OUR (BEST) FRIEND FOR THESE CALCULATIONS

			// subtract HEARD_2_TURNS_AGO (which is negative) to make values start at 0 and
			// be positive otherwise
	 iPercent = ThreatPercent[pFriend->aiData.bOppList[pOpponent->ubID] - OLDEST_HEARD_VALUE];

	// reduce the percentage value based on how far away they are from the enemy, if they only hear him
	if ( pFriend->aiData.bOppList[ pOpponent->ubID ] <= HEARD_LAST_TURN )
	{
		iPercent -= PythSpacesAway( pSoldier->sGridNo, pFriend->sGridNo ) * 2;
		if ( iPercent <= 0 )
		{
			//ignore!
			continue;
		}
	}

	 sFrndThreatValue = (iPercent * CalcManThreatValue(pFriend,pOpponent->sGridNo,FALSE,pSoldier)) / 100;

	 //sprintf(tempstr,"Known by friend %s, opplist status %d, percent %d, threat = %d",
	 //		 ExtMen[pFriend->ubID].name,pFriend->aiData.bOppList[pOpponent->ubID],ubPercent,sFrndThreatValue);
	 //PopMessage(tempstr);

	 // ADD this to our running total threatValue (increases my MORALE)
	 // We multiply by sOppThreatValue to PRO-RATE this based on opponent's
	 // threat value to ME personally.	Divide later by sum of them all.
	 iOurTotalThreat += sOppThreatValue * sFrndThreatValue;
	}

	// this could get slow if I have a lot of friends...
	//KeepInterfaceGoing();
	}


 // if they are no threat whatsoever
 if (!iTheirTotalThreat)
	sMorale = 500;		// our morale is just incredible
 else
	{
	// now divide sOutTotalThreat by sTheirTotalThreat to get the REAL value
	iOurTotalThreat /= iTheirTotalThreat;

	// calculate the morale (100 is even, < 100 is us losing, > 100 is good)
	sMorale = (INT16) ((100 * iOurTotalThreat) / iTheirTotalThreat);
	}


 if (sMorale <= 25)				// odds 1:4 or worse
	bMoraleCategory = MORALE_HOPELESS;
 else if (sMorale <= 50)		 // odds between 1:4 and 1:2
	bMoraleCategory = MORALE_WORRIED;
 else if (sMorale <= 150)		// odds between 1:2 and 3:2
	bMoraleCategory = MORALE_NORMAL;
 else if (sMorale <= 300)		// odds between 3:2 and 3:1
	bMoraleCategory = MORALE_CONFIDENT;
 else							// odds better than 3:1
	bMoraleCategory = MORALE_FEARLESS;


 switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:	bMoraleCategory--; break;
	case BRAVESOLO:	bMoraleCategory += 2; break;
	case BRAVEAID:	bMoraleCategory += 2; break;
	case CUNNINGSOLO:	break;
	case CUNNINGAID:	 break;
	case AGGRESSIVE:	bMoraleCategory++; break;
	}

 // make idiot administrators much more aggressive
 if ( pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR )
 {
	bMoraleCategory += 2;
 }


 // if still full of energy
 if (pSoldier->bBreath > 75)
	bMoraleCategory++;
 else
	{
	// if getting a bit low on breath
	if (pSoldier->bBreath < 40)
	 bMoraleCategory--;

	// if getting REALLY low on breath
	if (pSoldier->bBreath < 10)
	 bMoraleCategory--;
	}


 // if still very healthy
 if (pSoldier->stats.bLife > 75)
	bMoraleCategory++;
 else
	{
	// if getting a bit low on life
	if (pSoldier->stats.bLife < 40)
	 bMoraleCategory--;

	// if getting REALLY low on life
	if (pSoldier->stats.bLife < 20)
	 bMoraleCategory--;
	}


 // if soldier is currently not under fire
 if (!pSoldier->aiData.bUnderFire)
	bMoraleCategory++;


 // if adjustments made it outside the allowed limits
 if (bMoraleCategory < MORALE_HOPELESS)
	bMoraleCategory = MORALE_HOPELESS;
 else
	{
	if (bMoraleCategory > MORALE_FEARLESS)
	 bMoraleCategory = MORALE_FEARLESS;
	}

 // if only 1/4 of side left, reduce morale
 // and do this after we've capped all those other silly values
 /*
 if ( pSoldier->bTeam == ENEMY_TEAM && gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector <= gTacticalStatus.bOriginalSizeOfEnemyForce / 4 )
 {
	bMoraleCategory -= 2;
	if (bMoraleCategory < MORALE_HOPELESS)
	bMoraleCategory = MORALE_HOPELESS;
 }
 */

 // brave guys never get hopeless, at worst they get worried
 if (bMoraleCategory == MORALE_HOPELESS &&
	 (pSoldier->aiData.bAttitude == BRAVESOLO || pSoldier->aiData.bAttitude == BRAVEAID))
	bMoraleCategory = MORALE_WORRIED;

 // SANDRO - on Insane difficulty enemy morale cannot go below worried
 if (bMoraleCategory == MORALE_HOPELESS && gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
	bMoraleCategory = MORALE_WORRIED;

#ifdef DEBUGDECISIONS
		STR tempstr;
		 sprintf( tempstr, "Morale = %d (category %d)\n",
		pSoldier->aiData.bMorale,bMoraleCategory);
	DebugAI (tempstr);
#endif

 return(bMoraleCategory);
}

INT32 CalcManThreatValue( SOLDIERTYPE *pEnemy, INT32 sMyGrid, UINT8 ubReduceForCover, SOLDIERTYPE * pMe )
{
	INT32	iThreatValue = 0;
	BOOLEAN fForCreature = CREATURE_OR_BLOODCAT( pMe );

	// If man is inactive, at base, on assignment, dead, unconscious
	if (!pEnemy->bActive || !pEnemy->bInSector || !pEnemy->stats.bLife)
	{
		// he's no threat at all, return a negative number
		iThreatValue = -999;
		return(iThreatValue);
	}

	// in boxing mode, let only a boxer be considered a threat.
	if ( (gTacticalStatus.bBoxingState == BOXING) && !(pEnemy->flags.uiStatusFlags & SOLDIER_BOXER) )
	{
		iThreatValue = -999;
		return( iThreatValue );
	}

	if (fForCreature)
	{
		// health (1-100)
		iThreatValue += pEnemy->stats.bLife;
		// bleeding (more attactive!) (1-100)
		iThreatValue += pEnemy->bBleeding;
		// decrease according to distance
		iThreatValue = (iThreatValue * 10) / (10 + PythSpacesAway( sMyGrid, pEnemy->sGridNo ) );

	}
	else
	{
		// ADD twice the man's level (2-20)
		iThreatValue += EffectiveExpLevel(pEnemy); // SANDRO - find precise effective exp level

		// ADD man's total action points (10-35)
		iThreatValue += pEnemy->CalcActionPoints();

		// ADD 1/2 of man's current action points (4-17)
		iThreatValue += (pEnemy->bActionPoints / 2);

		// ADD 1/10 of man's current health (0-10)
		iThreatValue += (pEnemy->stats.bLife / 10);

		if (pEnemy->bAssignment < ON_DUTY )
		{
			// ADD 1/4 of man's protection percentage (0-25)
			iThreatValue += ArmourPercent( pEnemy ) / 4;

			// ADD 1/5 of man's marksmanship skill (0-20)
			iThreatValue += (pEnemy->stats.bMarksmanship / 5);

			if ( Item[ pEnemy->inv[HANDPOS].usItem ].usItemClass & IC_WEAPON )
			{
				// ADD the deadliness of the item(weapon) he's holding (0-50)
				iThreatValue += Weapon[pEnemy->inv[HANDPOS].usItem].ubDeadliness;
			}
		}

		// SUBTRACT 1/5 of man's bleeding (0-20)
		iThreatValue -= (pEnemy->bBleeding / 5);

		// SUBTRACT 1/10 of man's breath deficiency (0-10)
		iThreatValue -= ((100 - pEnemy->bBreath) / 10);

		// SUBTRACT man's current shock value
		iThreatValue -= pEnemy->aiData.bShock;
	}

	// if I have a specifically defined spot where I'm at (sometime I don't!)	
	if (!TileIsOutOfBounds(sMyGrid))
	{
		// ADD 10% if man's already been shooting at me
		if (pEnemy->sLastTarget == sMyGrid)
		{
			iThreatValue += (iThreatValue / 10);
		}
		else
		{
			// ADD 5% if man's already facing me
			if (pEnemy->ubDirection == atan8(CenterX(pEnemy->sGridNo),CenterY(pEnemy->sGridNo),CenterX(sMyGrid),CenterY(sMyGrid)))
			{
				iThreatValue += (iThreatValue / 20);
			}
		}
	}

	// if this man is conscious
	if (pEnemy->stats.bLife >= OKLIFE)
	{
		// and we were told to reduce threat for my cover		
		if (ubReduceForCover && (!TileIsOutOfBounds(sMyGrid)))
		{
			// Reduce iThreatValue to same % as the chance HE has shoot through at ME
			//iThreatValue = (iThreatValue * ChanceToGetThrough( pEnemy, myGrid, FAKE, ACTUAL, TESTWALLS, 9999, M9PISTOL, NOT_FOR_LOS)) / 100;
			//iThreatValue = (iThreatValue * SoldierTo3DLocationChanceToGetThrough( pEnemy, myGrid, FAKE, ACTUAL, TESTWALLS, 9999, M9PISTOL, NOT_FOR_LOS)) / 100;
			iThreatValue = (iThreatValue * SoldierToLocationChanceToGetThrough( pEnemy, sMyGrid, pMe->pathing.bLevel, 0, pMe->ubID ) ) / 100;
		}
	}
	else
	{
		// if he's still something of a threat
		if (iThreatValue > 0)
		{
			// drastically reduce his threat value (divide by 5 to 18)
			iThreatValue /= (4 + (OKLIFE - pEnemy->stats.bLife));
		}
	}

	// threat value of any opponent can never drop below 1
	if (iThreatValue < 1)
	{
		iThreatValue = 1;
	}

	//sprintf(tempstr,"%s's iThreatValue = ",pEnemy->name);
	//NumMessage(tempstr,iThreatValue);

#ifdef BETAVERSION	// unnecessary for real release
	// NOTE: maximum is about 200 for a healthy Mike type with a mortar!
	if (iThreatValue > 250)
	{
		sprintf(tempstr,"CalcManThreatValue: WARNING - %d has a very high threat value of %d",pEnemy->ubID,iThreatValue);

#ifdef RECORDNET
		fprintf(NetDebugFile,"\t%s\n",tempstr);
#endif

#ifdef TESTVERSION
		PopMessage(tempstr);
#endif

	}
#endif

	return(iThreatValue);
}

INT16 RoamingRange(SOLDIERTYPE *pSoldier, INT32 * pusFromGridNo)
{
	BOOL OppPosKnown = FALSE;
	if ( CREATURE_OR_BLOODCAT( pSoldier ) )
	{
		if ( pSoldier->aiData.bAlertStatus == STATUS_BLACK )
		{
			*pusFromGridNo = pSoldier->sGridNo; // from current position!
			return(MAX_ROAMING_RANGE);
		}
	}
	if ( pSoldier->aiData.bOrders == POINTPATROL || pSoldier->aiData.bOrders == RNDPTPATROL )
	{
		// roam near NEXT PATROL POINT, not from where merc starts out
		*pusFromGridNo = pSoldier->aiData.sPatrolGrid[pSoldier->aiData.bNextPatrolPnt];
	}
	else
	{
		// roam around where mercs started
		//*pusFromGridNo = pSoldier->sInitialGridNo;
		*pusFromGridNo = pSoldier->aiData.sPatrolGrid[0];
	}

	//Do we know about any opponent?
	for(UINT16 oppID = 0; oppID < MAX_NUM_SOLDIERS; oppID++)
	{
		if ( pSoldier->aiData.bOppList[oppID]  !=  NOT_HEARD_OR_SEEN &&  gbPublicOpplist[pSoldier->bTeam][oppID] != NOT_HEARD_OR_SEEN)
		{
			OppPosKnown = TRUE;
			break;
		}
	}

	//TODO: Externalize if people want?
	BOOL fLessRestrictiveRoaming = TRUE;

	switch (pSoldier->aiData.bOrders)
	{
		// JA2 GOLD: give non-NPCs a 5 tile roam range for cover in combat when being shot at
		case STATIONARY:			if (pSoldier->ubProfile != NO_PROFILE || (pSoldier->aiData.bAlertStatus < STATUS_BLACK && !(pSoldier->aiData.bUnderFire)))
									{
										return( 0 );
									}
									else
									{
										return( 5 );
									}		
		case ONGUARD:				return( 5 );
		case CLOSEPATROL:			if (pSoldier->aiData.bAlertStatus < STATUS_RED)
													{
														return( 5 );
													}
													else
													{
														if(!OppPosKnown || !fLessRestrictiveRoaming)
														{
															return( 15 );
														}
														else
														{
															return( 30 );
															//return( MAX_ROAMING_RANGE );
														}
													}
		case POINTPATROL:			if (pSoldier->aiData.bAlertStatus < STATUS_RED)
													{
														return( 10 );
													}
													else
													{
														if(!OppPosKnown || !fLessRestrictiveRoaming)
														{
															return( 20 );
														}
														else
														{
															return( 40 );
															//return( MAX_ROAMING_RANGE );
														}
													}	 // from nextPatrolGrid, not whereIWas
		case RNDPTPATROL:			if (pSoldier->aiData.bAlertStatus < STATUS_RED)
													{
														return( 10 );
													}
													else
													{
														if(!OppPosKnown || !fLessRestrictiveRoaming)
														{
															return( 20 );
														}
														else
														{
															//return( 40 );
															return( MAX_ROAMING_RANGE );
														}
													}// from nextPatrolGrid, not whereIWas
		case FARPATROL:				if (pSoldier->aiData.bAlertStatus < STATUS_RED)
													{
														return( 15 );
													}
													else
													{
														if(!OppPosKnown || !fLessRestrictiveRoaming)
														{
															return( 30 );
														}
														else
														{
															return( MAX_ROAMING_RANGE );
														}
													}
		case ONCALL:					if (pSoldier->aiData.bAlertStatus < STATUS_RED)
													{
														return( 10 );
													}
													else
													{
														if(!OppPosKnown || !fLessRestrictiveRoaming)
														{
															return( 30 );
														}
														else
														{
															//return(50);
															return( MAX_ROAMING_RANGE );
														}
													}
		case SEEKENEMY:				*pusFromGridNo = pSoldier->sGridNo; // from current position!
													return(MAX_ROAMING_RANGE);
		case SNIPER:				return ( 5 );
		default:
#ifdef BETAVERSION
			sprintf(tempstr,"%s has invalid orders = %d",pSoldier->GetName(),pSoldier->aiData.bOrders);
			PopMessage(tempstr);
#endif
			return(0);
	}
}


void RearrangePocket(SOLDIERTYPE *pSoldier, INT8 bPocket1, INT8 bPocket2, UINT8 bPermanent)
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"RearrangePocket");
	// NB there's no such thing as a temporary swap for now...
	// 0verhaul:  There is now!  If not permanent, don't lose weapon ready status because the
	// weapon will be restored after the trial situation is finished.
	//SwapObjs( &(pSoldier->inv[bPocket1]), &(pSoldier->inv[bPocket2]) );
	SwapObjs( pSoldier, bPocket1, bPocket2, bPermanent );

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"RearrangePocket done");
}

BOOLEAN FindBetterSpotForItem( SOLDIERTYPE * pSoldier, INT8 bSlot )
{
	// looks for a place in the slots to put an item in a hand or armour
	// position, and moves it there.
	if (bSlot >= BIGPOCKSTART)
	{
		return( FALSE );
	}
	if (pSoldier->inv[bSlot].exists() == false)
	{
		// well that's just fine then!
		return( TRUE );
	}

	if(FitsInSmallPocket(&pSoldier->inv[bSlot]) == false)
	{
		// then we're looking for a big pocket
		bSlot = FindEmptySlotWithin( pSoldier, BIGPOCKSTART, MEDPOCKFINAL );
	}
	else
	{
		// try a small pocket first
		bSlot = FindEmptySlotWithin( pSoldier, SMALLPOCKSTART, NUM_INV_SLOTS );
		if (bSlot == NO_SLOT)
		{
			bSlot = FindEmptySlotWithin( pSoldier, BIGPOCKSTART, MEDPOCKFINAL );
		}
	}
	if (bSlot == NO_SLOT)
	{
		return( FALSE );
	}
    DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"findbetterspotforitem: swapping items");
	RearrangePocket(pSoldier, HANDPOS, bSlot, FOREVER );		
	return( TRUE );
}

UINT8 GetTraversalQuoteActionID( INT8 bDirection )
{
	switch( bDirection )
	{
		case NORTHEAST: // east
			return( QUOTE_ACTION_ID_TRAVERSE_EAST );

		case SOUTHEAST: // south
			return( QUOTE_ACTION_ID_TRAVERSE_SOUTH );

		case SOUTHWEST: // west
			return( QUOTE_ACTION_ID_TRAVERSE_WEST );

		case NORTHWEST: // north
			return( QUOTE_ACTION_ID_TRAVERSE_NORTH );

		default:
			return( 0 );
	}
}

UINT8 SoldierDifficultyLevel( SOLDIERTYPE * pSoldier )
{
	INT8 bDifficultyBase;
	INT8 bDifficulty;

	DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("SoldierDifficultyLevel"));
	// difficulty modifier ranges from 0 to 100
	// and we want to end up with a number between 0 and 4 (4=hardest)
	// to a base of 1, divide by 34 to get a range from 1 to 3
	bDifficultyBase = 1 + ( CalcDifficultyModifier( pSoldier->ubSoldierClass ) / 34 );

	switch( pSoldier->ubSoldierClass )
	{
		case SOLDIER_CLASS_ADMINISTRATOR:
			bDifficulty = bDifficultyBase - 1;
			break;

		case SOLDIER_CLASS_ARMY:
			bDifficulty = bDifficultyBase;
			break;

		case SOLDIER_CLASS_ELITE:
			bDifficulty = bDifficultyBase + 1;
			break;

		// hard code militia;
		case SOLDIER_CLASS_GREEN_MILITIA:
			bDifficulty = 2;
			break;

		case SOLDIER_CLASS_REG_MILITIA:
			bDifficulty = 3;
			break;

		case SOLDIER_CLASS_ELITE_MILITIA:
			bDifficulty = 4;
			break;

#ifdef ENABLE_ZOMBIES
		case SOLDIER_CLASS_ZOMBIE:
			bDifficulty = bDifficultyBase;
			break;
#endif

		default:
			if (pSoldier->bTeam == CREATURE_TEAM)
			{
				bDifficulty = bDifficultyBase + pSoldier->pathing.bLevel / 4;
			}
			else // civ...
			{
				bDifficulty = (bDifficultyBase + pSoldier->pathing.bLevel / 4) - 1;
			}
			break;

	}

	bDifficulty = __max( bDifficulty, 0 );
	bDifficulty = __min( bDifficulty, 4 );

	return( (UINT8) bDifficulty );
}

BOOLEAN ValidCreatureTurn( SOLDIERTYPE * pCreature, INT8 bNewDirection )
{
	INT8	bDirChange;
	INT8	bTempDir;
	INT8	bLoop;
	BOOLEAN	fFound;

	bDirChange = (INT8) QuickestDirection( pCreature->ubDirection, bNewDirection );

	for( bLoop = 0; bLoop < 2; bLoop++ )
	{
		fFound = TRUE;

		bTempDir = pCreature->ubDirection;

		do
		{

			bTempDir += bDirChange;
			if (bTempDir < NORTH)
			{
				bTempDir = NORTHWEST;
			}
			else if (bTempDir > NORTHWEST)
			{
				bTempDir = NORTH;
			}
			if (!pCreature->InternalIsValidStance( bTempDir, ANIM_STAND ))
			{
				fFound = FALSE;
				break;
			}

		} while ( bTempDir != bNewDirection );

		if ( fFound )
		{
			break;
		}
		else if ( bLoop > 0 )
		{
			// can't find a dir!
			return( FALSE );
		}
		else
		{
			// try the other direction
			bDirChange = bDirChange * -1;
		}
	}

	return( TRUE );
}

INT32 RangeChangeDesire( SOLDIERTYPE * pSoldier )
{
	INT32 iRangeFactorMultiplier;

	iRangeFactorMultiplier = pSoldier->aiData.bAIMorale - 1;
	switch (pSoldier->aiData.bAttitude)
	{
		case DEFENSIVE:		iRangeFactorMultiplier += -1; break;
		case BRAVESOLO:		iRangeFactorMultiplier +=	2; break;
		case BRAVEAID:		iRangeFactorMultiplier +=	2; break;
		case CUNNINGSOLO:	iRangeFactorMultiplier +=	0; break;
		case CUNNINGAID:	iRangeFactorMultiplier +=	0; break;
		case ATTACKSLAYONLY:
		case AGGRESSIVE:	iRangeFactorMultiplier +=	1; break;
	}
	if ( gTacticalStatus.bConsNumTurnsWeHaventSeenButEnemyDoes > 0 )
	{
		iRangeFactorMultiplier += gTacticalStatus.bConsNumTurnsWeHaventSeenButEnemyDoes;
	}
	return( iRangeFactorMultiplier );
}

BOOLEAN ArmySeesOpponents( void )
{
	INT32				cnt;
	SOLDIERTYPE *		pSoldier;

	for ( cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++ )
	{
		pSoldier = MercPtrs[ cnt ];

		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE && pSoldier->aiData.bOppCnt > 0 )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}

#ifdef DEBUGDECISIONS
void AIPopMessage ( STR16 str )
{
	DebugAI(str);
}

void AIPopMessage ( const STR8	str )
{
	STR tempstr;
	sprintf( tempstr,"%s", str);
	DebugAI(tempstr);
}

void AINumMessage(const STR8	str, INT32 num)
{
	STR tempstr;
	sprintf( tempstr,"%s %d", str, num);
	DebugAI(tempstr);
}

void AINameMessage(SOLDIERTYPE * pSoldier,const STR8	str,INT32 num)
{
	STR tempstr;
	sprintf( tempstr,"%d %s %d",pSoldier->GetName() , str, num);
	DebugAI( tempstr );
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////
// HEADROCK:
//
// The following function(s) are part of my half-assed attempt to have the AI analyze the tactical
// situation, by comparing (known) squad sizes, the state of all combatants, and the orders of all
// friendlies. The idea is to return a value called "TacticalSituation" which can tell a combatant
// whether he should try to undertake a smarter course of action.
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
INT16 AssessTacticalSituation( INT8 bTeam )
{
	UINT16 ubFriendlyTeamTacticalValue = 0;
	UINT16 ubEnemyTeamTacticalValue = 0;
	UINT8 ubSoldierTacticalThreat;
	INT16 ubTacticalSituation;
	UINT16 cnt;
	SOLDIERTYPE * pSoldier;
	
	// begin loop through all MERCs.
	for ( cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++ )
	{
		pSoldier = MercPtrs[ cnt ];
		ubSoldierTacticalThreat = CalcStraightThreatValue( pSoldier );
		// Player-controlled Mercs are 1.5 times more threatening than AIs
		if (pSoldier->flags.uiStatusFlags & SOLDIER_PC)
			ubSoldierTacticalThreat = (UINT8)((float)ubSoldierTacticalThreat * 1.5);
		
		// Assess Threat
		if (bTeam == OUR_TEAM || bTeam == MILITIA_TEAM)
		{
			// Friendly!
			ubFriendlyTeamTacticalValue += ubSoldierTacticalThreat;
		}
		else
		{
			// Enemy!
			if ( TeamSeesOpponent( ENEMY_TEAM, pSoldier ) )
				ubEnemyTeamTacticalValue += ubSoldierTacticalThreat;

		}
	}

	// begin loop through all Militia.
	for ( cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++ )
	{
		pSoldier = MercPtrs[ cnt ];
		ubSoldierTacticalThreat = CalcStraightThreatValue( pSoldier );
		
		// Assess Threat
		if (bTeam == OUR_TEAM || bTeam == MILITIA_TEAM)
		{
			// Friendly!
			ubFriendlyTeamTacticalValue += ubSoldierTacticalThreat;
		}
		else
		{
			// Enemy!
			if ( TeamSeesOpponent( ENEMY_TEAM, pSoldier ) )
				ubEnemyTeamTacticalValue += ubSoldierTacticalThreat;
		}
	}	

	// begin loop through all Enemies.
	for ( cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++ )
	{
		pSoldier = MercPtrs[ cnt ];
		ubSoldierTacticalThreat = CalcStraightThreatValue( pSoldier );
		
		// Assess Threat

		if (bTeam == ENEMY_TEAM)
		{
			// Friendly!
			ubFriendlyTeamTacticalValue += ubSoldierTacticalThreat;
		}
		else
		{
			// Enemy!
			if ( TeamSeesOpponent( OUR_TEAM, pSoldier ) || TeamSeesOpponent ( MILITIA_TEAM, pSoldier) )
				ubEnemyTeamTacticalValue += ubSoldierTacticalThreat;
		}
		
	}

	ubTacticalSituation = ubEnemyTeamTacticalValue - ubFriendlyTeamTacticalValue;

	return (ubTacticalSituation);


}
*/

// HEADROCK: Function to check whether a team can see the specified soldier.
BOOLEAN TeamSeesOpponent( INT8 bTeam, SOLDIERTYPE * pOpponent )
{
	SOLDIERTYPE * pSoldier;
	UINT16 cnt;

	// This assertion can be safely removed, assuming the program does what it should. It simply checks
	// whether the "opponent" is on the same team being checked. That should be avoided when calling this
	// function.
	Assert( pOpponent->bTeam != bTeam );

	// We're checking Merc/Militia visibility
	if (bTeam == OUR_TEAM || bTeam == MILITIA_TEAM )
	{
		for ( cnt = gTacticalStatus.Team[ MILITIA_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ MILITIA_TEAM ].bLastID; cnt++ )
		{
			pSoldier = MercPtrs[ cnt ];

			if (pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE)
			{


				if (pSoldier->aiData.bOppList[ pOpponent->ubID ] == SEEN_CURRENTLY)
					return ( TRUE );
			}
		}
		for ( cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++ )
		{
			pSoldier = MercPtrs[ cnt ];

			if (pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE)
			{
				// This assertion can be safely removed, assuming the program does what it should. It simply checks
				// whether the "opponent" is on the same team being checked. That should be avoided when calling this
				// function.
				//Assert( pOpponent->bSide != bSide );

				if (pSoldier->aiData.bOppList[ pOpponent->ubID ] == SEEN_CURRENTLY)
					return ( TRUE );
			}
		}		
		
		return ( FALSE );
	}
	// Check enemy visibility
	else if (bTeam == ENEMY_TEAM)
	{
		for ( cnt = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ ENEMY_TEAM ].bLastID; cnt++ )
		{
			pSoldier = MercPtrs[ cnt ];

			if (pSoldier->bActive && pSoldier->bInSector && pSoldier->stats.bLife >= OKLIFE)
			{
				// This assertion can be safely removed, assuming the program does what it should. It simply checks
				// whether the "opponent" is on the same team being checked. That should be avoided when calling this
				// function.
				//Assert( pOpponent->bSide != bSide );

				if (pSoldier->aiData.bOppList[ pOpponent->ubID ] == SEEN_CURRENTLY)
					return ( TRUE );
			}
		}	
		return ( FALSE );
	}

	else
		return (FALSE);

}

// HEADROCK: Function to assess an enemy's threat value without "me" argument.
INT32 CalcStraightThreatValue( SOLDIERTYPE *pEnemy )
{
	INT32	iThreatValue = 0;

	// If man is inactive, at base, on assignment, dead, unconscious
	if (!pEnemy->bActive || !pEnemy->bInSector || !pEnemy->stats.bLife )
	{
		// he's no threat at all, return a negative number
		iThreatValue = 0;
		return(iThreatValue);
	}

	else
	{
		// ADD twice the man's level (2-20)
		iThreatValue += EffectiveExpLevel(pEnemy); // SANDRO - find precise effective exp level

		// ADD man's total action points (10-35)
		iThreatValue += pEnemy->CalcActionPoints();

		// ADD 1/2 of man's current action points (4-17)
		iThreatValue += (pEnemy->bActionPoints / 2);

		// ADD 1/10 of man's current health (0-10)
		iThreatValue += (pEnemy->stats.bLife / 10);

		if (pEnemy->bAssignment < ON_DUTY )
		{
			// ADD 1/4 of man's protection percentage (0-25)
			iThreatValue += ArmourPercent( pEnemy ) / 4;

			// ADD 1/5 of man's marksmanship skill (0-20)
			iThreatValue += (pEnemy->stats.bMarksmanship / 5);

			if ( Item[ pEnemy->inv[HANDPOS].usItem ].usItemClass & IC_WEAPON )
			{
				// ADD the deadliness of the item(weapon) he's holding (0-50)
				iThreatValue += Weapon[pEnemy->inv[HANDPOS].usItem].ubDeadliness;
			}
		}

		// SUBTRACT 1/5 of man's bleeding (0-20)
		iThreatValue -= (pEnemy->bBleeding / 5);

		// SUBTRACT 1/10 of man's breath deficiency (0-10)
		iThreatValue -= ((100 - pEnemy->bBreath) / 10);

		// SUBTRACT man's current shock value
		iThreatValue -= pEnemy->aiData.bShock;
	}

	// if this man is conscious
	if (pEnemy->stats.bLife < OKLIFE)
	{
		// if he's still something of a threat
		if (iThreatValue > 0)
		{
			// drastically reduce his threat value (divide by 5 to 18)
			iThreatValue /= (4 + (OKLIFE - pEnemy->stats.bLife));
		}
	}

	// threat value of any opponent can never drop below 1
	if (iThreatValue < 0)
	{
		iThreatValue = 0;
	}

	return(iThreatValue);
}

// Flugente: get the id of the closest soldier with a specific flag that we can currently see
UINT8 GetClosestFlaggedSoldierID( SOLDIERTYPE * pSoldier, INT16 aRange, UINT8 auTeam, UINT32 aFlag )
{
	UINT8				id = NOBODY;
	UINT32				uiLoop;
	SOLDIERTYPE *		pFriend;
	INT16				range = aRange;

	// go through each soldier, looking for "friends" (soldiers on same team)
	for (uiLoop = gTacticalStatus.Team[ auTeam ].bFirstID; uiLoop < gTacticalStatus.Team[ auTeam ].bLastID; ++uiLoop)
	{
		pFriend = MercPtrs[ uiLoop ];

		// if this merc is inactive, not in sector, or dead
		if (!pFriend)
			continue;
								
		// skip ourselves
		if (pFriend->ubID == pSoldier->ubID)
			continue;

		// must be on the same level
		if ( pFriend->pathing.bLevel != pSoldier->pathing.bLevel )
			continue;

		// this is not for tanks
		if ( TANK(pFriend) )
			return FALSE;
		
		// skip if this guy is dead
		if ( pFriend->stats.bLife <= 0 )
			continue;

		// check for flag
		if ( !(pFriend->bSoldierFlagMask & aFlag) )
			continue;

		// are we close enough?
		INT16 friendrange = SpacesAway(pSoldier->sGridNo, pFriend->sGridNo);
		if ( friendrange < range)
		{
			// can we see this guy?
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, pFriend->sGridNo, pSoldier->pathing.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
			{
				range = friendrange;
				id = pFriend->ubID;
			}
		}
	}
		
	return id;
}

// get the id of the closest soldier (closer than x tiles) of a specific team that is wounded that we can currently see
UINT8 GetClosestWoundedSoldierID( SOLDIERTYPE * pSoldier, INT16 aRange, UINT8 auTeam )
{
	UINT8				id = NOBODY;
	UINT32				uiLoop;
	SOLDIERTYPE *		pFriend;
	INT16				range = aRange;

	// go through each soldier, looking for "friends" (soldiers on same team)
	for (uiLoop = gTacticalStatus.Team[ auTeam ].bFirstID; uiLoop < gTacticalStatus.Team[ auTeam ].bLastID; ++uiLoop)
	{
		pFriend = MercPtrs[ uiLoop ];

		// if this merc is inactive, not in sector, or dead
		if (!pFriend)
			continue;
								
		// skip ourselves (if not allowed)
		if ( !gGameExternalOptions.fEnemyMedicsHealSelf && pFriend->ubID == pSoldier->ubID)
			continue;

		// must be on the same level
		if ( pFriend->pathing.bLevel != pSoldier->pathing.bLevel )
			continue;

		// this is not for tanks
		if ( TANK(pFriend) )
			return FALSE;
		
		// skip if this guy is dead, or not wounded (enough)
		if ( pFriend->stats.bLife <= 0 || pFriend->iHealableInjury < gGameExternalOptions.sEnemyMedicsWoundMinAmount )
			continue;

		// are we close enough?
		INT16 friendrange = SpacesAway(pSoldier->sGridNo, pFriend->sGridNo);
		if ( friendrange < range)
		{
			// can we see this guy?
			//if ( SoldierTo3DLocationLineOfSightTest( pSoldier, pFriend->sGridNo, pSoldier->pathing.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
			{
				range = friendrange;
				id = pFriend->ubID;
			}
		}
	}
		
	return id;
}

// get the id of the closest medic (closer than x tiles) of a specific team
UINT8 GetClosestMedicSoldierID( SOLDIERTYPE * pSoldier, INT16 aRange, UINT8 auTeam )
{
	UINT8				id = NOBODY;
	UINT32				uiLoop;
	SOLDIERTYPE *		pFriend;
	INT16				range = aRange;

	// go through each soldier, looking for "friends" (soldiers on same team)
	for (uiLoop = gTacticalStatus.Team[ auTeam ].bFirstID; uiLoop < gTacticalStatus.Team[ auTeam ].bLastID; ++uiLoop)
	{
		pFriend = MercPtrs[ uiLoop ];

		// if this merc is inactive, not in sector, or dead
		if (!pFriend)
			continue;
								
		// skip ourselves (we seek OTHER people)
		if ( pFriend->ubID == pSoldier->ubID)
			continue;

		// must be on the same level
		if ( pFriend->pathing.bLevel != pSoldier->pathing.bLevel )
			continue;

		// this is not for tanks
		if ( TANK(pFriend) )
			return FALSE;

		// skip this guy if he is dead or unconscious
		if ( pFriend->stats.bLife < OKLIFE )
			continue;
		
		// skip if this guy if he is no medic
		if ( !pFriend->CanMedicAI() )
			continue;

		// are we close enough?
		INT16 friendrange = SpacesAway(pSoldier->sGridNo, pFriend->sGridNo);
		if ( friendrange < range)
		{
			// can we see this guy?
			//if ( SoldierTo3DLocationLineOfSightTest( pSoldier, pFriend->sGridNo, pSoldier->pathing.bLevel, 3, TRUE, CALC_FROM_WANTED_DIR ) )
			{
				range = friendrange;
				id = pFriend->ubID;
			}
		}
	}
		
	return id;
}