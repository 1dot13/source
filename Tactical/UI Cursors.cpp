#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "items.h"
	#include "weapons.h"
	#include "Interface Cursors.h"

	#include "overhead.h"
	#include "Handle UI.h"
	#include "Animation Control.h"
	#include "points.h"
	#include "Sound Control.h"
	#include "Sys globals.h"
	#include "Isometric Utils.h"
	#include "Animation Data.h"
	#include "worldman.h"
	#include "ui cursors.h"
	#include "los.h"
	#include "renderworld.h"
	#include "interface.h"
	#include "ui cursors.h"
	#include "cursors.h"
	#include "cursor control.h"
	#include "structure wrap.h"
	#include "physics.h"
	#include "soldier macros.h"
	#include "text.h"
	#include "spread burst.h"
	#include "GameSettings.h"
	#include "Interactive Tiles.h"
	#include "PATHAI.H"
	#include "SkillCheck.h" // added by SANDRO
	#include "message.H" //ddd
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


// FUNCTIONS FOR ITEM CURSOR HANDLING
UINT8 HandleActivatedTargetCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos, BOOLEAN fShowAPs, BOOLEAN fRecalc, UINT32 uiCursorFlags );
UINT8 HandleNonActivatedTargetCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos, BOOLEAN fShowAPs, BOOLEAN fRecalc, UINT32 uiCursorFlags );
UINT8 HandleKnifeCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags );
UINT8 HandlePunchCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlagsl );
UINT8 HandleAidCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags );
UINT8 HandleActivatedTossCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubCursor );
UINT8 HandleNonActivatedTossCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fRecalc, UINT32 uiCursorFlags, UINT8 ubCursor );
UINT8 HandleWirecutterCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags );
UINT8 HandleRepairCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags );
UINT8 HandleRefuelCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags );
UINT8 HandleRemoteCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags );
UINT8 HandleBombCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags );
UINT8 HandleJarCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags );
UINT8 HandleTinCanCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags );

extern BOOLEAN	HandleCheckForBadChangeToGetThrough( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pTargetSoldier, INT32 sTargetGridNo , INT8 bLevel );


BOOLEAN gfCannotGetThrough = FALSE;
extern UINT32	guiUITargetSoldierId;
BOOLEAN	gfDisplayFullCountRing = FALSE;

UINT8	gubAimRingRanges[] = {30, 55, 75, 90}; // These are the cutoff ranges for the aiming rings
extern	INT8 gbNumBurstLocations;	//number of burst points


BOOLEAN GetMouseRecalcAndShowAPFlags( UINT32 *puiCursorFlags, BOOLEAN *pfShowAPs )
{
	UINT32						uiCursorFlags;
	static					BOOLEAN						fDoNewTile = FALSE;
	BOOLEAN						fRecalc = FALSE;
	BOOLEAN						fShowAPs = FALSE;

	// SET FLAGS FOR CERTAIN MOUSE MOVEMENTS
	GetCursorMovementFlags( &uiCursorFlags );

	// Force if we are currently cycling guys...
	if ( gfUIForceReExamineCursorData )
	{
		fDoNewTile = TRUE;
		fRecalc	= TRUE;
		gfUIForceReExamineCursorData = FALSE;
	}

	// IF CURSOR IS MOVING
	if ( uiCursorFlags & MOUSE_MOVING )
	{
			// IF CURSOR WAS PREVIOUSLY STATIONARY, MAKE THE ADDITIONAL CHECK OF GRID POS CHANGE
			//if ( uiCursorFlags & MOUSE_MOVING_NEW_TILE )
			{
				// Reset counter
				RESETCOUNTER( PATHFINDCOUNTER );
				fDoNewTile = TRUE;
			}
	}

	if ( uiCursorFlags & MOUSE_STATIONARY )
	{
			// ONLY DIPSLAY APS AFTER A DELAY
			if ( COUNTERDONE( PATHFINDCOUNTER ) )
			{
				// Don't reset counter: One when we move again do we do this!
				fShowAPs = TRUE;

				if ( fDoNewTile )
				{
					fDoNewTile = FALSE;
					fRecalc	= TRUE;
				}

			}

	}

	if ( puiCursorFlags )
	{
			(*puiCursorFlags) = uiCursorFlags;
	}

	if ( pfShowAPs )
	{
			(*pfShowAPs) = fShowAPs;
	}

	return( fRecalc );
}


// FUNCTIONS FOR CURSOR DETERMINATION!
UINT8	GetProperItemCursor( UINT8 ubSoldierID, UINT16 ubItemIndex, INT32 usMapPos, BOOLEAN fActivated )
{
	SOLDIERTYPE				*pSoldier;
	UINT32						uiCursorFlags;
	BOOLEAN						fShowAPs = FALSE;
	BOOLEAN						fRecalc = FALSE;
	INT32 sTargetGridNo = usMapPos;
	UINT8							ubCursorID=0;
	UINT8							ubItemCursor;

	pSoldier = MercPtrs[ ubSoldierID ];

	fRecalc = GetMouseRecalcAndShowAPFlags( &uiCursorFlags, &fShowAPs );

	// ATE: Update attacking weapon!
	// CC has added this attackingWeapon stuff and I need to update it constantly for
	// CTGH algorithms
	if ( gTacticalStatus.ubAttackBusyCount == 0 && Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass & IC_WEAPON )
	{
		pSoldier->usAttackingWeapon = pSoldier->inv[HANDPOS].usItem;
	}

	// Calculate target gridno!
	if ( gfUIFullTargetFound )
	{
		sTargetGridNo = MercPtrs[ gusUIFullTargetID ]->sGridNo;
	}
	else
	{
		sTargetGridNo = usMapPos;
	}


	ubItemCursor	=	GetActionModeCursor( pSoldier );

	switch( ubItemCursor )
	{
		case PUNCHCURS:

			// Determine whether gray or red!
			ubCursorID = HandlePunchCursor( pSoldier, sTargetGridNo, fActivated, uiCursorFlags );
			break;

		case KNIFECURS:
			//Madd: quick hack to make wirecutter cursor appear when using a knife that can cut through wire
			if ( Item[ubItemIndex].wirecutters && IsCuttableWireFenceAtGridNo( sTargetGridNo ) && pSoldier->pathing.bLevel == 0 )
			{
				ubCursorID = GOOD_WIRECUTTER_UICURSOR;
			}
			else
				ubCursorID = HandleKnifeCursor( pSoldier, sTargetGridNo, fActivated, uiCursorFlags );
			break;

		case AIDCURS:

			ubCursorID =  HandleAidCursor( pSoldier, usMapPos, fActivated, uiCursorFlags );
			break;

		case TARGETCURS:

			// Set merc glow script
			SetMercGlowFast( );

			if ( fActivated )
			{
				ubCursorID = HandleActivatedTargetCursor( pSoldier, sTargetGridNo, fShowAPs, fRecalc, uiCursorFlags );
			}
			else
			{
				ubCursorID = HandleNonActivatedTargetCursor( pSoldier, sTargetGridNo, fShowAPs, fRecalc, uiCursorFlags );
			}


			// ATE: Only do this if we are in combat!
			if ( gCurrentUIMode == ACTION_MODE && ( gTacticalStatus.uiFlags & INCOMBAT ) )
			{
				// Alrighty, let's change the cursor!
				if ( fRecalc && gfUIFullTargetFound )
				{
						// ATE: Check for ammo
						if ( IsValidTargetMerc( (UINT8)gusUIFullTargetID ) && EnoughAmmo( pSoldier, FALSE, HANDPOS ) &&
							(!pSoldier->IsValidSecondHandShotForReloadingPurposes( ) || EnoughAmmo( pSoldier, FALSE, SECONDHANDPOS) ) )
						{
							// IF it's an ememy, goto confirm action mode
							if ( ( guiUIFullTargetFlags & ENEMY_MERC ) && ( guiUIFullTargetFlags & VISIBLE_MERC ) && !( guiUIFullTargetFlags & DEAD_MERC ) && !gfCannotGetThrough )
							{
									guiPendingOverrideEvent = A_CHANGE_TO_CONFIM_ACTION;
							}

						}
				}
			}
			break;

		case TOSSCURS:
		case TRAJECTORYCURS:

			if ( fActivated )
			{
		if ( !gfUIHandlePhysicsTrajectory )
		{
				ubCursorID =	HandleNonActivatedTossCursor( pSoldier, sTargetGridNo, fRecalc, uiCursorFlags, ubItemCursor );
		}
		else
		{
				ubCursorID = HandleActivatedTossCursor( pSoldier, sTargetGridNo, ubItemCursor );
		}
			}
			else
			{
				ubCursorID =	HandleNonActivatedTossCursor( pSoldier, sTargetGridNo, fRecalc, uiCursorFlags, ubItemCursor );
			}

#if 0
			if ( gCurrentUIMode == ACTION_MODE && ubItemCursor == TRAJECTORYCURS && ( gTacticalStatus.uiFlags & INCOMBAT ) )
			{
				// Alrighty, let's change the cursor!
				if ( fRecalc && gfUIFullTargetFound )
				{
						// ATE: Check for ammo
						if ( IsValidTargetMerc( (UINT8)gusUIFullTargetID ) && EnoughAmmo( pSoldier, FALSE, HANDPOS ) )
						{
							// IF it's an ememy, goto confirm action mode
							if ( ( guiUIFullTargetFlags & ENEMY_MERC ) && ( guiUIFullTargetFlags & VISIBLE_MERC ) && !( guiUIFullTargetFlags & DEAD_MERC ) && !gfCannotGetThrough )
							{
									guiPendingOverrideEvent = A_CHANGE_TO_CONFIM_ACTION;
							}

						}
				}
			}
#endif
			break;

		case BOMBCURS:

			ubCursorID = HandleBombCursor( pSoldier, sTargetGridNo, fActivated, uiCursorFlags );
			break;

		case REMOTECURS:

			ubCursorID = HandleRemoteCursor( pSoldier, sTargetGridNo, fActivated, uiCursorFlags );
			break;

		case WIRECUTCURS:

			ubCursorID = HandleWirecutterCursor( pSoldier, sTargetGridNo, uiCursorFlags );
			break;


		case REPAIRCURS:

			ubCursorID = HandleRepairCursor( pSoldier, sTargetGridNo, uiCursorFlags );
			break;

		case JARCURS:

			ubCursorID = HandleJarCursor( pSoldier, sTargetGridNo, uiCursorFlags );
			break;

		case TINCANCURS:

			ubCursorID = HandleTinCanCursor( pSoldier, sTargetGridNo, uiCursorFlags );
			break;

		case REFUELCURS:

			ubCursorID = HandleRefuelCursor( pSoldier, sTargetGridNo, uiCursorFlags );
			break;

		case INVALIDCURS:

			ubCursorID =	INVALID_ACTION_UICURSOR;
			break;

	}

	if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		if ( gfUIFullTargetFound )
		{
			PauseRT( TRUE );
		}
		else
		{
			PauseRT( FALSE );
		}
	}


	return( ubCursorID );
}


// WANNE: Shows the target cursor over the enemy soldier
UINT8 HandleActivatedTargetCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos, BOOLEAN fShowAPs, BOOLEAN fRecalc, UINT32 uiCursorFlags )
{
	UINT8							switchVal;
	BOOLEAN							fEnoughPoints = TRUE;
	UINT8							bFutureAim;
	INT16							sAPCosts;
	INT16							sAPCostsMin;
	UINT16							usCursor=0;
	BOOLEAN							fMaxPointLimitHit = FALSE;
	UINT16							usInHand;

	UINT16	reverse = 0;

	// HEADROCK HAM 4: Required for new Aiming Level Limit function
	UINT8 maxAimLevels = AllowedAimingLevels(pSoldier, usMapPos);

		usInHand = pSoldier->inv[ HANDPOS ].usItem;

		if ( Item[ usInHand ].usItemClass != IC_THROWING_KNIFE )
		{
			// If we are in realtime, follow!
			if ( ( !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
			{
				if ( ( gAnimControl[ MercPtrs[ gusSelectedSoldier ]->usAnimState ].uiFlags & ANIM_STATIONARY ) )
				{
					if ( gUITargetShotWaiting )
					{
						guiPendingOverrideEvent = CA_MERC_SHOOT;
					}
				}

				//SoldierFollowGridNo( pSoldier, sMapPos );
			}

			// Check if we are reloading
			if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
			{
				if ( pSoldier->flags.fReloading || pSoldier->flags.fPauseAim )
				{
					return( ACTION_TARGET_RELOADING );
				}

			}
		}


		// Determine where we are shooting / aiming
		//if ( fRecalc )
		{
			DetermineCursorBodyLocation( (UINT8)gusSelectedSoldier, TRUE, TRUE );
		}

		if ( gTacticalStatus.uiFlags & TURNBASED && ( gTacticalStatus.uiFlags & INCOMBAT ) )
		{
			if(pSoldier->bDoAutofire)
			{
				if(pSoldier->bDoAutofire == 1) //try to maximize the # of bullets because 1 may not be optimal
				{
					INT16 sCurAPCosts, sAPCosts;

					sCurAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, pSoldier->aiData.bShownAimTime);

					do
					{
						pSoldier->bDoAutofire++;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts == sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire);
					pSoldier->bDoAutofire--;
				}

				gfUIAutofireBulletCount = TRUE;
				gsBulletCount = pSoldier->bDoAutofire;
				gsTotalBulletCount = pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft;

				if(pSoldier->flags.autofireLastStep) //set the orange tint on the numbers if we at the last step
					gTintBulletCounts = TRUE;
				else
					gTintBulletCounts = FALSE;

			}
			else
				gfUIAutofireBulletCount = FALSE;

			gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, (INT8)(pSoldier->aiData.bShownAimTime ) );
			gfUIDisplayActionPoints = TRUE;
			gfUIDisplayActionPointsCenter = TRUE;


			while(!EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) && pSoldier->bDoAutofire > 1) //oops, we don't have enough points if we are in auto-fire try to decrease bulletcount
			{
				pSoldier->bDoAutofire--;
				gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, (INT8)(pSoldier->aiData.bShownAimTime ) );
			}

			// If we don't have any points and we are at the first refine, do nothing but warn!
			if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
			{
				gfUIDisplayActionPointsInvalid = TRUE;

				fMaxPointLimitHit = TRUE;
			}
			else
			{
				// WANNE: How oft increased?
				bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + 1 );

				if ( bFutureAim <= maxAimLevels )
				{
					if (reverse == 0)
					{
						// WANNE: Current AP costs for targeting
						sAPCosts = MinAPsToAttack( pSoldier, usMapPos, TRUE ) + ( bFutureAim	);

						//gsCurrentActionPoints = sAPCosts;

						// Determine if we can afford!
						if ( !EnoughPoints( pSoldier, (INT16)sAPCosts, 0 , FALSE ) )
						{
							fEnoughPoints = FALSE;
						}
					}
					else
					{
						sAPCosts = MinAPsToAttack( pSoldier, usMapPos, TRUE ) + maxAimLevels - (bFutureAim + 1);

						gsCurrentActionPoints = sAPCosts;

						sAPCostsMin = MinAPsToAttack( pSoldier, usMapPos, TRUE );
						if (sAPCosts < sAPCostsMin)
						{
							fEnoughPoints = FALSE;
						}
					}

					//// Determine if we can afford!
					//if ( !EnoughPoints( pSoldier, (INT16)sAPCosts, 0 , FALSE ) )
					//{
					//	fEnoughPoints = FALSE;
					//}

				}

			}
		}

		if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{
			if ( !pSoldier->flags.fPauseAim )
			{
				if ( COUNTERDONE( TARGETREFINE )	)
				{
					// Reset counter
					RESETCOUNTER( TARGETREFINE );

					if ( pSoldier->bDoBurst )
					{
						pSoldier->aiData.bShownAimTime = REFINE_AIM_BURST;
					}
					else
					{
						pSoldier->aiData.bShownAimTime++;

						if ( pSoldier->aiData.bShownAimTime > maxAimLevels )
						{
							pSoldier->aiData.bShownAimTime = maxAimLevels;
						}
						else
						{
							if ( pSoldier->aiData.bShownAimTime % 2 )
							{
								PlayJA2Sample( TARG_REFINE_BEEP, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
							}
						}
					}
				}
			}
		}

		if ( fRecalc )
		{
			if ( gusUIFullTargetID != NOBODY )
			{
				if ( SoldierToSoldierBodyPartChanceToGetThrough( pSoldier, MercPtrs[ gusUIFullTargetID ], pSoldier->bAimShotLocation ) < OK_CHANCE_TO_GET_THROUGH )
				{
					gfCannotGetThrough = TRUE;
				}
				else
				{
					gfCannotGetThrough = FALSE;
				}
			}
			else
			{
				if ( SoldierToLocationChanceToGetThrough( pSoldier, usMapPos, (INT8) gsInterfaceLevel, pSoldier->bTargetCubeLevel, NOBODY ) < OK_CHANCE_TO_GET_THROUGH )
				{
					gfCannotGetThrough = TRUE;
				}
				else
				{
					gfCannotGetThrough = FALSE;
				}
			}
		}

		// OK, if we begin to move, reset the cursor...
		if ( uiCursorFlags & MOUSE_MOVING )
		{
			//gfCannotGetThrough = FALSE;
		}

		if ( pSoldier->flags.fDoSpread && pSoldier->bDoAutofire) //approximate spread-auto costs
		{
			if(gbNumBurstLocations > pSoldier->bDoAutofire)
			{
				INT16	sAPCosts;

				do
				{
					pSoldier->bDoAutofire++;
					sAPCosts = CalcTotalAPsToAttack( pSoldier, gsBurstLocations[0].sGridNo, TRUE, pSoldier->aiData.bShownAimTime);
				}
				while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire && gbNumBurstLocations >= pSoldier->bDoAutofire);
				pSoldier->bDoAutofire--;

				gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, gsBurstLocations[0].sGridNo, TRUE, pSoldier->aiData.bShownAimTime );
			}
		}

		if ( fMaxPointLimitHit )
		{
			// Check if we're in burst mode!
			if ( pSoldier->bDoBurst )
			{
				usCursor = ACTION_TARGETREDBURST_UICURSOR;
			}
			else if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
			{
				usCursor = RED_THROW_UICURSOR;
			}
			else
			{
				usCursor = ACTION_TARGETRED_UICURSOR;
			}
		}
		else if ( pSoldier->bDoBurst )
		{
			if ( pSoldier->flags.fDoSpread )
			{
				usCursor = ACTION_TARGETREDBURST_UICURSOR;
			}
			else
			{
				//usCursor = ACTION_TARGETREDBURST_UICURSOR;
				usCursor = ACTION_TARGETCONFIRMBURST_UICURSOR;

				// HEADROCK HAM B1: Burst cursor now shows multiple CTH bars (one for each bullet)
				// and also a Targetted Bodypart indicator.
				//if(pSoldier->bDoAutofire == 0 && gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
				if(pSoldier->bDoAutofire == 0)
				{
					if(UsingNewCTHSystem() == true)
					{
						//AXP 29.03.2007: Rooftop CtH fix. See below.
						INT8 bTempTargetLevel = pSoldier->bTargetLevel;
						pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;

						UINT32 uiHitChance;
						uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, pSoldier->aiData.bShownAimTime, pSoldier->bAimShotLocation );
						// HEADROCK HAM B2.7: CTH approximation?
						if (gGameExternalOptions.fApproximateCTH)
						{	
							uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
						}
						gfUICtHBar = TRUE;
						gbCtHBurstCount = 0;
						gCTHDisplay.MuzzleSwayPercentage = (gCTHDisplay.MuzzleSwayPercentage + uiHitChance)/2;
						gCTHDisplay.iShooterGridNo = pSoldier->sGridNo;
						gCTHDisplay.iTargetGridNo = usMapPos;
						
						// Calculate distance to target
						FLOAT dDeltaX = (FLOAT)(CenterX( pSoldier->sGridNo ) - CenterX( usMapPos ));
						FLOAT dDeltaY = (FLOAT)(CenterY( pSoldier->sGridNo ) - CenterY( usMapPos ));
						FLOAT d2DDistance = sqrt((dDeltaX*dDeltaX)+(dDeltaY*dDeltaY));

						CalcMagFactorSimple( pSoldier, d2DDistance, pSoldier->aiData.bShownAimTime );
						
						pSoldier->bTargetLevel = bTempTargetLevel;
					}
					else if(gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
					{
						if (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 2)
						{
							// Burst mode only
							OBJECTTYPE * pInHand;
							pInHand = &(pSoldier->inv[pSoldier->ubAttackingHand]);
							// Burst size
							gbCtHBurstCount = GetShotsPerBurst(pInHand);
							UINT8 i, saveDoBurst;
							// Save original burst value (should be 1 always, before the burst has been fired, but
							// this is just a failsafe
							saveDoBurst = pSoldier->bDoBurst;

							// For each bullet in the burst
							for (i=0;i<gbCtHBurstCount;i++)
							{
								//AXP 29.03.2007: Rooftop CtH fix. See below.
								INT8 bTempTargetLevel = pSoldier->bTargetLevel;
								pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;

								// Calculate hit chance (using the current bDoBurst for burst-penalty calculations)
								UINT32 uiHitChance;
								uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
								// HEADROCK HAM B2.7: CTH approximation?
								if (gGameExternalOptions.fApproximateCTH)
								{	
									uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
								}

								pSoldier->bTargetLevel = bTempTargetLevel;

								// Put result (burst CTH) into the array
								gbCtH[i] = (gbCtH[i]+uiHitChance)/2;

								// Increase burst size, so that the next time we run CalcChanceToHitGun we'll get a lower
								// result, based on our Burst Penalty
								pSoldier->bDoBurst++;
							}
							// Reset original burst status
							pSoldier->bDoBurst = saveDoBurst;
							// Activate CTH bar display
							gfUICtHBar = TRUE;
						}
						else // One "BASE CTH" bar, JA2 1.13 vanilla
						{
							//AXP 29.03.2007: Rooftop CtH fix. See below.
							INT8 bTempTargetLevel = pSoldier->bTargetLevel;
							pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;

							UINT32 uiHitChance;
							uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
							// HEADROCK HAM B2.7: CTH approximation?
							if (gGameExternalOptions.fApproximateCTH)
							{	
								uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
							}

							pSoldier->bTargetLevel = bTempTargetLevel;

							gfUICtHBar = TRUE;
							gbCtH[0] = (gbCtH[0]+uiHitChance)/2;
							gbCtHBurstCount = 1;
						}
					}
				}

				// HEADROCK HAM B2: Now autofire will show both its starting CTH and the CTH of the last
				// bullet in the volley.
				
				//else if(pSoldier->bDoAutofire > 0 && gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
				else if(pSoldier->bDoAutofire > 0)
				{
					if(UsingNewCTHSystem() == true)
					{
						//AXP 29.03.2007: Rooftop CtH fix. See below.
						INT8 bTempTargetLevel = pSoldier->bTargetLevel;
						pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;
							
						UINT32 uiHitChance;
						// Calculate CTH for the first bullet
						uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, pSoldier->aiData.bShownAimTime, pSoldier->bAimShotLocation );
						// HEADROCK HAM B2.7: CTH approximation?
						if (gGameExternalOptions.fApproximateCTH)
						{	
							uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
						}

						// CTH for the first bullet is entered into this array, and later displayed
						gCTHDisplay.MuzzleSwayPercentage = (gCTHDisplay.MuzzleSwayPercentage + uiHitChance)/2;
						gCTHDisplay.iShooterGridNo = pSoldier->sGridNo;
						gCTHDisplay.iTargetGridNo = usMapPos;

						// Flag to tell the program to draw two CTH bars.
						gfUICtHBar = TRUE;
						gbCtHBurstCount = 0;

						// Calculate distance to target
						FLOAT dDeltaX = (FLOAT)(CenterX( pSoldier->sGridNo ) - CenterX( usMapPos ));
						FLOAT dDeltaY = (FLOAT)(CenterY( pSoldier->sGridNo ) - CenterY( usMapPos ));
						FLOAT d2DDistance = sqrt((dDeltaX*dDeltaX)+(dDeltaY*dDeltaY));

						CalcMagFactorSimple( pSoldier, d2DDistance, pSoldier->aiData.bShownAimTime );

						pSoldier->bTargetLevel = bTempTargetLevel;
					}
					else if(gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
					{
						if (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 3)
						{
							gbCtHBurstCount = 1;

							//AXP 29.03.2007: Rooftop CtH fix. See below.
							INT8 bTempTargetLevel = pSoldier->bTargetLevel;
							pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;
							
							UINT32 uiHitChance;
							// Calculate CTH for the first bullet
							uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
							// HEADROCK HAM B2.7: CTH approximation?
							if (gGameExternalOptions.fApproximateCTH)
							{	
								uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
							}

							// CTH for the first bullet is entered into this array, and later displayed
							gbCtH[0] = (gbCtH[0]+uiHitChance)/2;

							// Fool the CTH formula into thinking we're already firing the last bullet in the volley,
							// by artificially altering bDoBurst (which tracks actual burst progression). The CTH formula
							// calculates Auto Penalty based on bDoBurst - (Autopen * (bDoBurst-1), basically).
							UINT8 saveDoBurst = pSoldier->bDoBurst;
							pSoldier->bDoBurst = pSoldier->bDoAutofire;
							
							uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
							// HEADROCK HAM B2.7: CTH approximation?
							if (gGameExternalOptions.fApproximateCTH)
							{	
								uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
							}

							pSoldier->bTargetLevel = bTempTargetLevel;
							// Return Burst State to original value
							pSoldier->bDoBurst = saveDoBurst;

							// CTH for the last bullet is entered into this array and later displayed.
							gbCtH[1] = (gbCtH[1]+uiHitChance)/2;
					
							// Flag to tell the program to draw two CTH bars.
							gbCtHAutoFire = TRUE;
							gfUICtHBar = TRUE;
						}
					}
				}
			}
		}
		else
		{
			//DIGICRAB: Graphical CtH
			//if(gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
			{
				//AXP 29.03.2007: Rooftop CtH fix. Temporarily set soldier targetlevel to the real interface
				//level (selected with TAB), so the first shot after switching TL is calculated correctly
				INT8 bTempTargetLevel = pSoldier->bTargetLevel;
				pSoldier->bTargetLevel = (INT8) gsInterfaceLevel;

				UINT32 uiHitChance;
				// SANDRO - precise calculation for throwing knives added
				if(UsingNewCTHSystem() == true)
				{
					uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
				}
				else
				{
					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						uiHitChance = CalcThrownChanceToHit( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
					}
					else
					{
						uiHitChance = CalcChanceToHitGun( pSoldier, usMapPos, (INT8)(pSoldier->aiData.bShownAimTime ), pSoldier->bAimShotLocation );
					}
				}
				// HEADROCK HAM B2.7: CTH approximation?
				if (gGameExternalOptions.fApproximateCTH)
				{	
					uiHitChance = ChanceToHitApproximation( pSoldier, uiHitChance );
				}

				if(UsingNewCTHSystem() == true)
				{
					gfUICtHBar = TRUE;
					gbCtHBurstCount = 0;

					// CTH for the first bullet is entered into this array, and later displayed
					gCTHDisplay.MuzzleSwayPercentage = (gCTHDisplay.MuzzleSwayPercentage + uiHitChance)/2;
					gCTHDisplay.iShooterGridNo = pSoldier->sGridNo;
					gCTHDisplay.iTargetGridNo = usMapPos;

					// Calculate distance to target
					FLOAT dDeltaX = (FLOAT)(CenterX( pSoldier->sGridNo ) - CenterX( usMapPos ));
					FLOAT dDeltaY = (FLOAT)(CenterY( pSoldier->sGridNo ) - CenterY( usMapPos ));
					FLOAT d2DDistance = sqrt((dDeltaX*dDeltaX)+(dDeltaY*dDeltaY));

					CalcMagFactorSimple( pSoldier, d2DDistance, pSoldier->aiData.bShownAimTime );
				}

				pSoldier->bTargetLevel = bTempTargetLevel;

				if(UsingNewCTHSystem() == false && gGameSettings.fOptions[ TOPTION_CTH_CURSOR ])
				{
					gfUICtHBar = TRUE;
					gbCtHBurstCount = 1;
					gbCtH[0] = (gbCtH[0]+uiHitChance)/2;
				}
			}
			switchVal = pSoldier->aiData.bShownAimTime;

			switch( switchVal )
			{
				case REFINE_AIM_1:

					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW1_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_THROWAIM1_UICURSOR;
						}
						else
						{
							usCursor = ACTION_THROWAIMCANT1_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW1_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_TARGETAIM1_UICURSOR;
						}
						else
						{
							usCursor = ACTION_TARGETAIMCANT1_UICURSOR;
						}
					}
					break;

				case REFINE_AIM_2:

					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW2_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
								usCursor =	ACTION_THROWAIM3_UICURSOR;
						}
						else
						{
								usCursor = ACTION_THROWAIMCANT2_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW2_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
								usCursor =	ACTION_TARGETAIM3_UICURSOR;
						}
						else
						{
								usCursor = ACTION_TARGETAIMCANT2_UICURSOR;
						}
					}
					break;

				case REFINE_AIM_3:

					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW3_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_THROWAIM5_UICURSOR;
						}
						else
						{
							usCursor = ACTION_THROWAIMCANT3_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW3_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_TARGETAIM5_UICURSOR;
						}
						else
						{
							usCursor = ACTION_TARGETAIMCANT3_UICURSOR;
						}
					}
					break;

				case REFINE_AIM_4:

					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW4_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_THROWAIM7_UICURSOR;
						}
						else
						{
							usCursor = ACTION_THROWAIMCANT4_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW4_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_TARGETAIM7_UICURSOR;
						}
						else
						{
							usCursor = ACTION_TARGETAIMCANT4_UICURSOR;
						}
					}
					break;

				case REFINE_AIM_5:

					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMFULL_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor =	ACTION_THROWAIM9_UICURSOR;
						}
						else
						{
							usCursor =	ACTION_THROWAIMCANT5_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMFULL_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor =	ACTION_TARGETAIM9_UICURSOR;
						}
						else
						{
							usCursor =	ACTION_TARGETAIMCANT5_UICURSOR;
						}
					}
					break;

				case REFINE_AIM_MID1:

					//usCursor =	ACTION_TARGETAIM2_UICURSOR;
					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW1_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_THROWAIM2_UICURSOR;
						}
						else
						{
							usCursor = ACTION_THROWAIMCANT1_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW1_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_TARGETAIM2_UICURSOR;
						}
						else
						{
							usCursor = ACTION_TARGETAIMCANT1_UICURSOR;
						}
					}
					break;

				case REFINE_AIM_MID2:

					//usCursor = ACTION_TARGETAIM4_UICURSOR;
					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW3_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
								usCursor =	ACTION_THROWAIM4_UICURSOR;
						}
						else
						{
								usCursor = ACTION_THROWAIMCANT3_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW3_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
								usCursor =	ACTION_TARGETAIM4_UICURSOR;
						}
						else
						{
								usCursor = ACTION_TARGETAIMCANT3_UICURSOR;
						}
					}

					break;

				case REFINE_AIM_MID3:

					//usCursor = ACTION_TARGETAIM6_UICURSOR;

					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMYELLOW4_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_THROWAIM6_UICURSOR;
						}
						else
						{
							usCursor = ACTION_THROWAIMCANT4_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMYELLOW4_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_TARGETAIM6_UICURSOR;
						}
						else
						{
							usCursor = ACTION_TARGETAIMCANT4_UICURSOR;
						}
					}

					break;

				case REFINE_AIM_MID4:
					//usCursor =	ACTION_TARGETAIM8_UICURSOR;
					if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_THROWAIMFULL_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_THROWAIM8_UICURSOR;
						}
						else
						{
							usCursor = ACTION_THROWAIMCANT5_UICURSOR;
						}
					}
					else
					{
						if ( gfDisplayFullCountRing )
						{
							usCursor = ACTION_TARGETAIMFULL_UICURSOR;
						}
						else if ( fEnoughPoints )
						{
							usCursor = ACTION_TARGETAIM8_UICURSOR;
						}
						else
						{
							usCursor = ACTION_TARGETAIMCANT5_UICURSOR;
						}
					}
					break;
			}
		}

		if ( !fMaxPointLimitHit )
		{
			// Remove flash flag!
			RemoveCursorFlags( gUICursors[ usCursor ].usFreeCursorName, CURSOR_TO_FLASH );
			RemoveCursorFlags( gUICursors[ usCursor ].usFreeCursorName, CURSOR_TO_PLAY_SOUND );

			if ( gfCannotGetThrough	)
			{
				SetCursorSpecialFrame( gUICursors[ usCursor ].usFreeCursorName, 1 );
			}
			else
			{
				if ( !InRange( pSoldier, usMapPos ) )
				{
					// OK, make buddy flash!
					SetCursorFlags( gUICursors[ usCursor ].usFreeCursorName, CURSOR_TO_FLASH );
					SetCursorFlags( gUICursors[ usCursor ].usFreeCursorName, CURSOR_TO_PLAY_SOUND );
				}
				else
				{
					SetCursorSpecialFrame( gUICursors[ usCursor ].usFreeCursorName, 0 );
				}
			}
		}
		return( (UINT8)usCursor );
}



// WANNE: Shows the target cursor if we are not hovering over an enemy soldier
UINT8 HandleNonActivatedTargetCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos , BOOLEAN fShowAPs, BOOLEAN fRecalc, UINT32 uiCursorFlags  )
{
	UINT16				usInHand;

	usInHand = pSoldier->inv[ HANDPOS ].usItem;

	if ( Item[ usInHand ].usItemClass != IC_THROWING_KNIFE )
	{
		if (( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{
			//DetermineCursorBodyLocation( (UINT8)gusSelectedSoldier, FALSE, fRecalc );
			DetermineCursorBodyLocation( (UINT8)gusSelectedSoldier, fShowAPs, fRecalc );

			if ( pSoldier->flags.fReloading || pSoldier->flags.fPauseAim )
			{
				return( ACTION_TARGET_RELOADING );
			}
		}

		//CHRISL: We need to only check the second hand if the weapon in the second hand is onehanded
		// Check for enough ammo...
		if ( !EnoughAmmo( pSoldier, FALSE, HANDPOS ) || (pSoldier->IsValidSecondHandShotForReloadingPurposes( ) && !EnoughAmmo( pSoldier, FALSE, SECONDHANDPOS) && !Item[pSoldier->inv[SECONDHANDPOS].usItem].twohanded ) )
		{
			// Check if ANY ammo exists.....
			if ( FindAmmoToReload( pSoldier, HANDPOS, NO_SLOT ) == NO_SLOT )
			{
				// OK, use BAD reload cursor.....
				return( BAD_RELOAD_UICURSOR );
			}
			else
			{
				// Check APs to reload...
				gsCurrentActionPoints = GetAPsToAutoReload( pSoldier );

				gfUIDisplayActionPoints = TRUE;
				//gUIDisplayActionPointsOffX = 14;
				//gUIDisplayActionPointsOffY = 7;

				// OK, use GOOD reload cursor.....
				return( GOOD_RELOAD_UICURSOR );
			}
		}
	}

	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		DetermineCursorBodyLocation( (UINT8)gusSelectedSoldier, fShowAPs, fRecalc );

		gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, (INT8)(pSoldier->aiData.bShownAimTime ) );

		gfUIDisplayActionPoints = TRUE;
		gfUIDisplayActionPointsCenter = TRUE;

		if ( fShowAPs )
		{
			if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
			{
				gfUIDisplayActionPointsInvalid = TRUE;
			}
		}
		else
		{
			//gfUIDisplayActionPointsBlack = TRUE;
			gfUIDisplayActionPoints = FALSE;
		}

	}

	if(gusUIFullTargetID == NOBODY && pSoldier->bDoAutofire && !pSoldier->flags.fDoSpread)	//reset autofire if we move the mouse off the target, however don't reset it if we are spread-bursting
	{
		if(gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ))
			pSoldier->bDoAutofire = 1;
		else
			pSoldier->bDoAutofire = 6;

		pSoldier->flags.autofireLastStep = FALSE;
	}

	//if ( gTacticalStatus.uiFlags & TURNBASED && !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		if ( fRecalc )
		{
			if ( SoldierToLocationChanceToGetThrough( pSoldier, usMapPos, (INT8) gsInterfaceLevel, pSoldier->bTargetCubeLevel, NOBODY	) < OK_CHANCE_TO_GET_THROUGH )
			{
				gfCannotGetThrough = TRUE;
			}
			else
			{
				gfCannotGetThrough = FALSE;
			}
		}

		// OK, if we begin to move, reset the cursor...
		if ( uiCursorFlags & MOUSE_MOVING )
		{
			gfCannotGetThrough = FALSE;
		}

		if ( gfCannotGetThrough )
		{
			if ( pSoldier->bDoBurst )
			{
				return(	ACTION_NOCHANCE_BURST_UICURSOR );
			}
			else if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
			{
				return(	BAD_THROW_UICURSOR );
			}
			else
			{
				return(	ACTION_NOCHANCE_SHOOT_UICURSOR );
			}
		}
	}

	// Determine if good range
	if ( !InRange( pSoldier, usMapPos ) )
	{
		// Flash cursor!
		// Check if we're in burst mode!
		if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
		{
			return(	FLASH_THROW_UICURSOR );
		}
		else if ( pSoldier->bDoBurst )
		{
			//return( ACTION_FIRSTAID_RED );
			return(	ACTION_FLASH_BURST_UICURSOR );
		}
		else
		{
			//return( ACTION_FIRSTAID_RED );
			return( ACTION_FLASH_SHOOT_UICURSOR );
		}
	}
	else
	{
		// Check if we're in burst mode!
		if ( Item[ usInHand ].usItemClass == IC_THROWING_KNIFE )
		{
			return(	GOOD_THROW_UICURSOR );
		}
		else if ( pSoldier->bDoBurst )
		{
			//return( ACTION_FIRSTAID_RED );
			return( ACTION_TARGETBURST_UICURSOR );
		}
		else
		{
			//return( ACTION_FIRSTAID_RED );
			return( ACTION_SHOOT_UICURSOR );
		}

	}

}


void DetermineCursorBodyLocation( UINT8 ubSoldierID, BOOLEAN fDisplay, BOOLEAN fRecalc )
{
	INT32 usMapPos;
	SOLDIERTYPE				*pTargetSoldier = NULL, *pSoldier;
	UINT16	usFlags;
	INT16	sMouseX, sMouseY, sCellX, sCellY, sScreenX, sScreenY;
	BOOLEAN	fOnGuy = FALSE;
	LEVELNODE		*pNode;

	pSoldier = MercPtrs[ ubSoldierID ];

	if ( gTacticalStatus.ubAttackBusyCount > 0 )
	{
		// ATE: Return if attacker busy count > 0, this
		// helps in RT with re-setting the flag to random...
		return;
	}

	if ( fRecalc )
	{
		// ALWAYS SET AIM LOCATION TO NOTHING
		pSoldier->bAimShotLocation = AIM_SHOT_RANDOM;

		if( !GetMouseMapPos( &usMapPos) )
		{
			return;
		}

		// Determine which body part it's on
		pNode = GetAnimProfileFlags( usMapPos, &usFlags, &pTargetSoldier, NULL );

		while( pNode != NULL )
		{
			if ( pTargetSoldier != NULL )
			{
				// ATE: Check their stance - if prone - return!
				if ( gAnimControl[ pTargetSoldier->usAnimState ].ubHeight == ANIM_PRONE )
				{
					return;
				}

				fOnGuy = TRUE;

				// Check if we have a half tile profile
				if ( usFlags & TILE_FLAG_NORTH_HALF )
				{
					// Check if we are in north half of tile!
					GetMouseXYWithRemainder( &sMouseX, &sMouseY, &sCellX, &sCellY );

					if ( sCellY > (CELL_Y_SIZE / 2) )
					{
						fOnGuy = FALSE;
					}
				}
				// Check if we have a half tile profile
				if ( usFlags & TILE_FLAG_SOUTH_HALF )
				{
					// Check if we are in north half of tile!
					GetMouseXYWithRemainder( &sMouseX, &sMouseY, &sCellX, &sCellY );

					if ( sCellY <= (CELL_Y_SIZE / 2) )
					{
						fOnGuy = FALSE;
					}
				}
				// Check if we have a half tile profile
				if ( usFlags & TILE_FLAG_WEST_HALF )
				{
					// Check if we are in north half of tile!
					GetMouseXYWithRemainder( &sMouseX, &sMouseY, &sCellX, &sCellY );

					if ( sCellX > (CELL_X_SIZE / 2) )
					{
						fOnGuy = FALSE;
					}
				}
				if ( usFlags & TILE_FLAG_EAST_HALF )
				{
					// Check if we are in north half of tile!
					GetMouseXYWithRemainder( &sMouseX, &sMouseY, &sCellX, &sCellY );

					if ( sCellX <= (CELL_X_SIZE / 2) )
					{
						fOnGuy = FALSE;
					}
				}
				if ( usFlags & TILE_FLAG_TOP_HALF )
				{
					// Check if we are in north half of tile!
					GetMouseXYWithRemainder( &sMouseX, &sMouseY, &sCellX, &sCellY );

					// Convert these to screen corrdinates
					FromCellToScreenCoordinates( sCellX, sCellY, &sScreenX, &sScreenY );

					// Check for Below...
					if ( sScreenX > (WORLD_TILE_Y / 2) )
					{
						fOnGuy = FALSE;
					}
				}
				if ( usFlags & TILE_FLAG_BOTTOM_HALF )
				{
					// Check if we are in north half of tile!
					GetMouseXYWithRemainder( &sMouseX, &sMouseY, &sCellX, &sCellY );

					// Convert these to screen corrdinates
					FromCellToScreenCoordinates( sCellX, sCellY, &sScreenX, &sScreenY );

					// Check for Below...
					if ( sScreenX <= (WORLD_TILE_Y / 2) )
					{
						fOnGuy = FALSE;
					}
				}


				// Check if mouse is iin bounding box of soldier
				if ( !IsPointInSoldierBoundingBox( pTargetSoldier, gusMouseXPos, gusMouseYPos ) )
				{
					fOnGuy = FALSE;
				}
			}

			if ( fOnGuy )
				break;

			pNode = GetAnimProfileFlags( usMapPos, &usFlags, &pTargetSoldier, pNode );

		}


		if ( !fOnGuy )
		{
			// Check if we can find a soldier here
			if ( gfUIFullTargetFound )
			{
				pTargetSoldier = MercPtrs[ gusUIFullTargetID ];

				if ( FindRelativeSoldierPosition( pTargetSoldier, &usFlags, gusMouseXPos, gusMouseYPos )	)
				{
						fOnGuy = TRUE;
				}
			}

		}


		if ( fOnGuy )
		{
			if ( IsValidTargetMerc( pTargetSoldier->ubID ) )
			{
				if ( usFlags & TILE_FLAG_FEET )
				{
					pSoldier->bAimShotLocation = AIM_SHOT_LEGS;
				}
				if ( usFlags & TILE_FLAG_MID )
				{
					pSoldier->bAimShotLocation = AIM_SHOT_TORSO;
				}
				if ( usFlags & TILE_FLAG_HEAD )
				{
					pSoldier->bAimShotLocation = AIM_SHOT_HEAD;
				}
			}
		}

	}
	// HEADROCK HAM B2: Now shows targetted bodypart for ALL firing modes. Note that the external toggle is not
	// taken into account yet.
	//	if ( fDisplay && !pSoldier->bDoAutofire )
	if ( fDisplay )
	{
		if ( gfUIFullTargetFound )
		{
			pTargetSoldier = MercPtrs[ gusUIFullTargetID ];

			if ( pTargetSoldier->ubBodyType == CROW )
			{
				pSoldier->bAimShotLocation = AIM_SHOT_LEGS;

				wcscpy( gzLocation, TacticalStr[ CROW_HIT_LOCATION_STR ] );

				if ( !pSoldier->bDoBurst )
					gfUIBodyHitLocation = TRUE;
				// HEADROCK: This'll toggle whether the bodypart targetting indicator shows up in the burst/auto
				// CTH cursors.
				else if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 2) )
					gfUIBodyHitLocation = TRUE;
				else if ( pSoldier->bDoBurst && pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 3) )
					gfUIBodyHitLocation = TRUE;
				return;
			}

			if ( !IS_MERC_BODY_TYPE( pTargetSoldier ) )
			{
				return;
			}

			switch( pSoldier->bAimShotLocation )
			{
				case AIM_SHOT_HEAD:

					// If we have a knife in hand, change string
					if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass == IC_BLADE )
					{
						wcscpy( gzLocation, TacticalStr[ NECK_HIT_LOCATION_STR ] );
					}
					else
					{
						wcscpy( gzLocation, TacticalStr[ HEAD_HIT_LOCATION_STR ] );
					}
					if ( !pSoldier->bDoBurst )
						gfUIBodyHitLocation = TRUE;
					// HEADROCK: This'll toggle whether the bodypart targetting indicator shows up in the burst/auto
					// CTH cursors.
					else if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 2) )
						gfUIBodyHitLocation = TRUE;
					else if ( pSoldier->bDoBurst && pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 3) )
						gfUIBodyHitLocation = TRUE;
					break;

				case AIM_SHOT_TORSO:
					wcscpy( gzLocation, TacticalStr[ TORSO_HIT_LOCATION_STR ] );
					if ( !pSoldier->bDoBurst )
						gfUIBodyHitLocation = TRUE;
					// HEADROCK: This'll toggle whether the bodypart targetting indicator shows up in the burst/auto
					// CTH cursors.
					else if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 2) )
						gfUIBodyHitLocation = TRUE;
					else if ( pSoldier->bDoBurst && pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 3) )
						gfUIBodyHitLocation = TRUE;
					break;

				case AIM_SHOT_LEGS:
					wcscpy( gzLocation, TacticalStr[ LEGS_HIT_LOCATION_STR ] );
					if ( !pSoldier->bDoBurst )
						gfUIBodyHitLocation = TRUE;
					// HEADROCK: This'll toggle whether the bodypart targetting indicator shows up in the burst/auto
					// CTH cursors.
					else if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 2) )
						gfUIBodyHitLocation = TRUE;
					else if ( pSoldier->bDoBurst && pSoldier->bDoAutofire && (gGameExternalOptions.ubNewCTHBars == 1 || gGameExternalOptions.ubNewCTHBars == 3) )
					gfUIBodyHitLocation = TRUE;
					break;
			}
		}
	}
}


UINT8 HandleKnifeCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags )
{
	INT16							sAPCosts;
	INT8							bFutureAim;
	BOOLEAN						fEnoughPoints = TRUE;

	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_MERCS );

	if ( fActivated )
	{
		DetermineCursorBodyLocation( pSoldier->ubID, TRUE, TRUE );

		if ( gfUIHandleShowMoveGrid )
		{
			gfUIHandleShowMoveGrid = 2;
		}

		// Calculate action points
		if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, sGridNo, TRUE, (INT8)(pSoldier->aiData.bShownAimTime ) );
			gfUIDisplayActionPoints = TRUE;
			gfUIDisplayActionPointsCenter = TRUE;

			// If we don't have any points and we are at the first refine, do nothing but warn!
			if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
			{
				gfUIDisplayActionPointsInvalid = TRUE;

				if ( pSoldier->aiData.bShownAimTime == REFINE_KNIFE_1 )
				{
					return( KNIFE_HIT_UICURSOR );
				}
			}

			// SANDRO - changed this
			bFutureAim = (INT8)( gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedBladedAttackes : REFINE_KNIFE_2 );

			sAPCosts = CalcTotalAPsToAttack( pSoldier, sGridNo, TRUE, (INT8)(bFutureAim / 2) );

			// Determine if we can afford!
			if ( !EnoughPoints( pSoldier, (INT16)sAPCosts, 0 , FALSE ) )
			{
				fEnoughPoints = FALSE;
			}

		}

		if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{
			if ( !pSoldier->flags.fPauseAim )
			{
				if ( COUNTERDONE( NONGUNTARGETREFINE )	)
				{
					// Reset counter
					RESETCOUNTER( NONGUNTARGETREFINE );

					if ( pSoldier->aiData.bShownAimTime == REFINE_KNIFE_1 )
					{
						PlayJA2Sample( TARG_REFINE_BEEP, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
					}

					pSoldier->aiData.bShownAimTime = REFINE_KNIFE_2;

				}
			}
		}


		//////////////////////////////////////////////////////////////
		// SANDRO - slightly changed the formula here
		if( pSoldier->aiData.bShownAimTime == REFINE_KNIFE_1 )
		{
			if ( gfDisplayFullCountRing )
			{
				return( KNIFE_YELLOW_AIM1_UICURSOR );
			}
			else if ( fEnoughPoints )
			{
				return( KNIFE_HIT_AIM1_UICURSOR );
			}
			else
			{
				return( KNIFE_NOGO_AIM1_UICURSOR );
			}
		}
		else if( pSoldier->aiData.bShownAimTime == ( gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedBladedAttackes : REFINE_KNIFE_2 ) )
		{
			if ( gfDisplayFullCountRing )
			{
				return( KNIFE_YELLOW_AIM2_UICURSOR );
			}
			else if ( fEnoughPoints )
			{
				return( KNIFE_HIT_AIM2_UICURSOR );
			}
			else
			{
				return( KNIFE_NOGO_AIM2_UICURSOR );
			}
		}
		else
		{
			Assert( FALSE );
			// no return value!
			return(0);
		}
		//////////////////////////////////////////////////////////////
	}
	else
	{
		gfUIDisplayActionPointsCenter = TRUE;

		// CHECK IF WE ARE ON A GUY ( THAT'S NOT SELECTED )!
		if ( gfUIFullTargetFound && !( guiUIFullTargetFlags & SELECTED_MERC ) )
		{
			DetermineCursorBodyLocation( pSoldier->ubID, TRUE, TRUE );
			return( KNIFE_HIT_UICURSOR );
		}
		else
		{
			return( KNIFE_REG_UICURSOR );
		}
	}
}


UINT8 HandlePunchCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags )
{
	INT16							sAPCosts;
	INT8							bFutureAim;
	BOOLEAN						fEnoughPoints = TRUE;

	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_MERCS );

	if ( fActivated )
	{
		DetermineCursorBodyLocation( pSoldier->ubID, TRUE, TRUE );

		if ( gfUIHandleShowMoveGrid )
		{
			gfUIHandleShowMoveGrid = 2;
		}

		// Calculate action points
		if ( gTacticalStatus.uiFlags & TURNBASED )
		{
			gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, sGridNo, TRUE, (INT8)(pSoldier->aiData.bShownAimTime ) );
			gfUIDisplayActionPoints = TRUE;
			gfUIDisplayActionPointsCenter = TRUE;

			// If we don't have any points and we are at the first refine, do nothing but warn!
			if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
			{
				gfUIDisplayActionPointsInvalid = TRUE;

				if ( pSoldier->aiData.bShownAimTime == REFINE_PUNCH_1 )
				{
					return( ACTION_PUNCH_RED );
				}
			}

			bFutureAim = (INT8)( REFINE_PUNCH_2 );

			sAPCosts = CalcTotalAPsToAttack( pSoldier, sGridNo, TRUE, (INT8)(bFutureAim / 2) );

			// Determine if we can afford!
			if ( !EnoughPoints( pSoldier, (INT16)sAPCosts, 0 , FALSE ) )
			{
				fEnoughPoints = FALSE;
			}

		}

		if ( ( ( gTacticalStatus.uiFlags & REALTIME ) || !( gTacticalStatus.uiFlags & INCOMBAT ) ) )
		{
			if ( !pSoldier->flags.fPauseAim )
			{
				if ( COUNTERDONE( NONGUNTARGETREFINE )	)
				{
					// Reset counter
					RESETCOUNTER( NONGUNTARGETREFINE );

					if ( pSoldier->aiData.bShownAimTime == REFINE_PUNCH_1 )
					{
						PlayJA2Sample( TARG_REFINE_BEEP, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
					}

					// SANDRO - make aimed punch less expensive for APS
					pSoldier->aiData.bShownAimTime = (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : REFINE_PUNCH_2);

				}
			}
		}

		//////////////////////////////////////////////////////////////
		// SANDRO - slightly changed the formula here
		if( pSoldier->aiData.bShownAimTime == REFINE_PUNCH_1)
		{
			if ( gfDisplayFullCountRing )
			{
				return( ACTION_PUNCH_YELLOW_AIM1_UICURSOR );
			}
			else if ( fEnoughPoints )
			{
				return( ACTION_PUNCH_RED_AIM1_UICURSOR );
			}
			else
			{
				return( ACTION_PUNCH_NOGO_AIM1_UICURSOR );
			}
		}
		else if ( pSoldier->aiData.bShownAimTime == (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : REFINE_PUNCH_2) )
		{
			if ( gfDisplayFullCountRing )
			{
				return( ACTION_PUNCH_YELLOW_AIM2_UICURSOR );
			}
			else if ( fEnoughPoints )
			{
				return( ACTION_PUNCH_RED_AIM2_UICURSOR );
			}
			else
			{
				return( ACTION_PUNCH_NOGO_AIM2_UICURSOR );
			}
		}
		else
		{
			Assert( FALSE );
			// no return value!
			return(0);
		}
		//////////////////////////////////////////////////////////////
	}
	else
	{
		gfUIDisplayActionPointsCenter = TRUE;

		// CHECK IF WE ARE ON A GUY ( THAT'S NOT SELECTED )!
		if ( gfUIFullTargetFound && !( guiUIFullTargetFlags & SELECTED_MERC ) )
		{
			DetermineCursorBodyLocation( pSoldier->ubID, TRUE, TRUE );
			return( ACTION_PUNCH_RED );
		}
		else
		{
			return( ACTION_PUNCH_GRAY );
		}
	}
}

UINT8 HandleAidCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags )
{
	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_MERCSFORAID );

	if ( fActivated )
	{
		return( ACTION_FIRSTAID_RED );
	}
	else
	{
		// CHECK IF WE ARE ON A GUY ( THAT'S NOT SELECTED )!
		if ( gfUIFullTargetFound ) // && !( guiUIFullTargetFlags & SELECTED_MERC ) )
		{
			return( ACTION_FIRSTAID_RED );
		}
		else
		{
			return( ACTION_FIRSTAID_GRAY );
		}
	}
}

UINT8 HandleActivatedTossCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT8 ubItemCursor )
{
	return( ACTION_TOSS_UICURSOR );
}


UINT8 HandleNonActivatedTossCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fRecalc, UINT32 uiCursorFlags, UINT8 ubItemCursor )
{
	INT32 sFinalGridNo;
	static BOOLEAN fBadCTGH = FALSE;
	BOOLEAN fArmed = FALSE;
	INT8		bLevel;
	OBJECTTYPE * pObj;

	// Check for enough ammo...
	if ( ubItemCursor == TRAJECTORYCURS )
	{
		fArmed = TRUE;

		if ( !EnoughAmmo( pSoldier, FALSE, HANDPOS ) )
		{
			// Check if ANY ammo exists.....
			if ( FindAmmoToReload( pSoldier, HANDPOS, NO_SLOT ) == NO_SLOT )
			{
				// OK, use BAD reload cursor.....
				return( BAD_RELOAD_UICURSOR );
			}
			else
			{
				// Check APs to reload...
				gsCurrentActionPoints = GetAPsToAutoReload( pSoldier );

				gfUIDisplayActionPoints = TRUE;
				//gUIDisplayActionPointsOffX = 14;
				//gUIDisplayActionPointsOffY = 7;

				// OK, use GOOD reload cursor.....
				return( GOOD_RELOAD_UICURSOR );
			}
		}
	}

	// Add APs....
	if ( gTacticalStatus.uiFlags & TURNBASED && ( gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		if ( ubItemCursor == TRAJECTORYCURS )
		{
		gsCurrentActionPoints = CalcTotalAPsToAttack( pSoldier, sGridNo, TRUE, (INT8)(pSoldier->aiData.bShownAimTime ) );
		}
		else
		{
			gsCurrentActionPoints = MinAPsToThrow( pSoldier, sGridNo, TRUE );
		}

		gfUIDisplayActionPoints = TRUE;
		gfUIDisplayActionPointsCenter = TRUE;

		// If we don't have any points and we are at the first refine, do nothing but warn!
		if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
		{
			gfUIDisplayActionPointsInvalid = TRUE;
		}
	}


	// OK, if we begin to move, reset the cursor...
	if ( uiCursorFlags & MOUSE_MOVING )
	{
		EndPhysicsTrajectoryUI( );
	}

	gfUIHandlePhysicsTrajectory = TRUE;

	if ( fRecalc )
	{
		// Calculate chance to throw here.....
		if ( sGridNo == pSoldier->sGridNo )
		{
			fBadCTGH = FALSE;
		}
		else
		{
			UINT16 glItem = GetAttachedGrenadeLauncher( &(pSoldier->inv[HANDPOS]));
			if ((pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )&& glItem != NONE )
			{
				OBJECTTYPE* pAttachment = FindAttachment( &(pSoldier->inv[HANDPOS]), glItem );

				if ( pAttachment )
				{
					if ( !CalculateLaunchItemChanceToGetThrough( pSoldier, pAttachment, sGridNo, (INT8)gsInterfaceLevel, (INT16)( gsInterfaceLevel * 256 ), &sFinalGridNo, fArmed, &bLevel, TRUE ) )
					{
						fBadCTGH = TRUE;
					}
					else
					{
						fBadCTGH = FALSE;
					}
					BeginPhysicsTrajectoryUI( sFinalGridNo, bLevel, fBadCTGH );
				}
			}
			else
			{
				OBJECTTYPE* pObject = &(pSoldier->inv[HANDPOS]);
				// Do we have a launcable?
				pObj = &(pSoldier->inv[HANDPOS]);
				for (attachmentList::iterator iter = (*pObj)[0]->attachments.begin(); iter != (*pObj)[0]->attachments.end(); ++iter) {
					if ( Item[ iter->usItem ].usItemClass & IC_EXPLOSV && iter->exists())
					{
						pObject = &(*iter);
						break;
					}
				}

				if ( !CalculateLaunchItemChanceToGetThrough( pSoldier, pObject, sGridNo, (INT8)gsInterfaceLevel, (INT16)( gsInterfaceLevel * 256 ), &sFinalGridNo, fArmed, &bLevel, TRUE ) )
				{
					fBadCTGH = TRUE;
				}
				else
				{
					fBadCTGH = FALSE;
				}
				BeginPhysicsTrajectoryUI( sFinalGridNo, bLevel, fBadCTGH );
			}
		}
	}

	if ( fBadCTGH )
	{
		return( BAD_THROW_UICURSOR );
	}
	return( GOOD_THROW_UICURSOR );
}


UINT8 HandleWirecutterCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags )
{
	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_WIREFENCE );

	// Are we over a cuttable fence?
	if ( IsCuttableWireFenceAtGridNo( sGridNo ) && pSoldier->pathing.bLevel == 0 )
	{
		return( GOOD_WIRECUTTER_UICURSOR );
	}

	return( BAD_WIRECUTTER_UICURSOR );
}


UINT8 HandleRepairCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags )
{
	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_REPAIR );

	// Are we over a cuttable fence?
	if ( IsRepairableStructAtGridNo( sGridNo, NULL ) && pSoldier->pathing.bLevel == 0 )
	{
		return( GOOD_REPAIR_UICURSOR );
	}

	return( BAD_REPAIR_UICURSOR );
}

UINT8 HandleRefuelCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags )
{
	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_REFUEL );

	// Are we over a cuttable fence?
	if ( IsRefuelableStructAtGridNo( sGridNo, NULL ) && pSoldier->pathing.bLevel == 0 )
	{
		return( REFUEL_RED_UICURSOR );
	}

	return( REFUEL_GREY_UICURSOR );
}


UINT8 HandleJarCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags )
{
	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_JAR );

	// Are we over a cuttable fence?
	if ( IsCorpseAtGridNo( sGridNo, pSoldier->pathing.bLevel ) )
	{
		return( GOOD_JAR_UICURSOR );
	}

	return( BAD_JAR_UICURSOR );
}


UINT8 HandleTinCanCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, UINT32 uiCursorFlags )
{
	STRUCTURE					*pStructure;
	INT32 sIntTileGridNo;
	LEVELNODE					*pIntTile;


	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_CAN );

	// Check if a door exists here.....
	pIntTile = GetCurInteractiveTileGridNoAndStructure( &sIntTileGridNo, &pStructure );

	// We should not have null here if we are given this flag...
	if ( pIntTile != NULL )
	{
		if (pStructure->fFlags & STRUCTURE_ANYDOOR)
		{
			return( PLACE_TINCAN_GREY_UICURSOR );
		}
	}

	return( PLACE_TINCAN_RED_UICURSOR );
}


UINT8 HandleRemoteCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags )
{

	// Calculate action points
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		gsCurrentActionPoints = GetAPsToUseRemote( pSoldier );
		gfUIDisplayActionPoints = TRUE;
		gfUIDisplayActionPointsCenter = TRUE;

		// If we don't have any points and we are at the first refine, do nothing but warn!
		if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
		{
			gfUIDisplayActionPointsInvalid = TRUE;
		}
	}

	if ( fActivated )
	{
		return( PLACE_REMOTE_RED_UICURSOR );
	}
	else
	{
		return( PLACE_REMOTE_GREY_UICURSOR );
	}
}


UINT8 HandleBombCursor( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fActivated, UINT32 uiCursorFlags )
{

	// DRAW PATH TO GUY
	HandleUIMovementCursor( pSoldier, uiCursorFlags, sGridNo, MOVEUI_TARGET_BOMB );

	// Calculate action points
	if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT) )
	{
		gsCurrentActionPoints = GetTotalAPsToDropBomb( pSoldier, sGridNo );
		gfUIDisplayActionPoints = TRUE;
		gfUIDisplayActionPointsCenter = TRUE;

		// If we don't have any points and we are at the first refine, do nothing but warn!
		if ( !EnoughPoints( pSoldier, gsCurrentActionPoints, 0 , FALSE ) )
			{
			gfUIDisplayActionPointsInvalid = TRUE;
		}
	}

	if ( fActivated )
	{
		return( PLACE_BOMB_RED_UICURSOR );
	}
	else
	{
		return( PLACE_BOMB_GREY_UICURSOR );
	}
}



void HandleEndConfirmCursor( SOLDIERTYPE *pSoldier )
{
	UINT16				usInHand;
	UINT8					ubItemCursor;

	// LOOK IN GUY'S HAND TO CHECK LOCATION
	usInHand = pSoldier->inv[HANDPOS].usItem;

	ubItemCursor	=	GetActionModeCursor( pSoldier );

	if ( ubItemCursor == TOSSCURS )
	{
	//	EndAimCubeUI( );
	}
}

void HandleLeftClickCursor( SOLDIERTYPE *pSoldier )
{
	UINT16				usInHand;
	UINT8					ubItemCursor;
	INT32 usGridNo;

	// LOOK IN GUY'S HAND TO CHECK LOCATION
	usInHand = pSoldier->inv[HANDPOS].usItem;

	ubItemCursor	=	GetActionModeCursor( pSoldier );

	// OK, if we are i realtime.. goto directly to shoot
	if ( ( ( gTacticalStatus.uiFlags & TURNBASED ) && !( gTacticalStatus.uiFlags & INCOMBAT ) ) )//&& ubItemCursor != TOSSCURS && ubItemCursor != TRAJECTORYCURS )
	{
		// GOTO DIRECTLY TO USING ITEM
		// ( only if not burst mode.. )
		if ( !pSoldier->bDoBurst )
		{
			guiPendingOverrideEvent = CA_MERC_SHOOT;
		}
		return;
	}

	if (!GetMouseMapPos( &usGridNo ) )
	{
		return;
	}

	gfUIForceReExamineCursorData = TRUE;

	gfDisplayFullCountRing = FALSE;

	switch( ubItemCursor )
	{
		case TARGETCURS:

			if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) )
			{
				pSoldier->aiData.bShownAimTime				= REFINE_AIM_1;
				pSoldier->flags.fPauseAim = FALSE;
			}
			else
			{
				pSoldier->aiData.bShownAimTime				= REFINE_AIM_1;
				pSoldier->flags.fPauseAim = FALSE;
			}
			// Reset counter
			RESETCOUNTER( TARGETREFINE );
			break;

		case PUNCHCURS:

			if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) )
			{
				pSoldier->aiData.bShownAimTime				= REFINE_PUNCH_1;
				pSoldier->flags.fPauseAim = FALSE;
			}
			else
			{
				pSoldier->aiData.bShownAimTime				= REFINE_PUNCH_1;
				pSoldier->flags.fPauseAim = FALSE;

			}
			// Reset counter
			RESETCOUNTER( NONGUNTARGETREFINE );
			break;


		case KNIFECURS:

			if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) )
			{
				pSoldier->aiData.bShownAimTime				= REFINE_KNIFE_1;
				pSoldier->flags.fPauseAim = FALSE;
			}
			else
			{
				pSoldier->aiData.bShownAimTime				= REFINE_KNIFE_1;
				pSoldier->flags.fPauseAim = FALSE;

			}
			// Reset counter
			RESETCOUNTER( NONGUNTARGETREFINE );
			break;

		case TOSSCURS:

			//BeginAimCubeUI( pSoldier, sGridNo, (INT8)gsInterfaceLevel, 0, 0 );
			//break;

		default:

			// GOTO DIRECTLY TO USING ITEM
			guiPendingOverrideEvent = CA_MERC_SHOOT;
	}

}




void HandleRightClickAdjustCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos )
{
	UINT16					usInHand;
	INT16					sAPCosts;
	INT8					bFutureAim;
	UINT8					ubCursor;
	SOLDIERTYPE				*pTSoldier;
	INT32					sGridNo;
	INT8					bTargetLevel;
	// SANDRO - added these two
	INT8 bAimTimeAddedForPunch = 2;
	INT8 bAimTimeAddedForKnife = 2;


	usInHand = pSoldier->inv[HANDPOS].usItem;

	ubCursor =	GetActionModeCursor( pSoldier );

	// HEADROCK HAM 4: Required for new Aiming Level Limit function
	UINT8 maxAimLevels = AllowedAimingLevels(pSoldier, usMapPos);

	// 'snap' cursor to target tile....
	if ( gfUIFullTargetFound )
	{
		usMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;
	}


	switch( ubCursor )
	{
		case TARGETCURS:

			// CHECK IF GUY HAS IN HAND A WEAPON
			if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire)
			{
				// Do nothing!
				// pSoldier->aiData.bShownAimTime = REFINE_AIM_BURST;
			}
			else if(pSoldier->bDoAutofire)
			{
				INT16	sCurAPCosts;

				if(pSoldier->flags.autofireLastStep)
				{
					pSoldier->bDoAutofire = 1;						//reset the bullet counter
					pSoldier->flags.autofireLastStep = FALSE;
					return;
				}


				if(pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire )
				{
					//Calculate how many bullets we need to fire to add at least one more AP
					sAPCosts = sCurAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts <= sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire)	//Increment the bullet count until we run out of APs or we spend the whole AP
					{
						pSoldier->bDoAutofire++;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}

					//we've stepped over the border and used up one more ap, now let's make sure that it is spent to maximize the bullets

					sCurAPCosts = sAPCosts;

					do
					{
						pSoldier->bDoAutofire++;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts == sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire);
					pSoldier->bDoAutofire--;

					sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, pSoldier->aiData.bShownAimTime);

					if(!EnoughPoints( pSoldier, sAPCosts, 0, FALSE ))		//We've not enough points to fire those bullets
					{
						pSoldier->flags.autofireLastStep = TRUE;
						pSoldier->bDoAutofire--;
					}
					else
						pSoldier->flags.autofireLastStep = FALSE; //both last step conditions are false
				}
				else
				{
					pSoldier->flags.autofireLastStep = TRUE;
				}
			}
			else
			{
				sGridNo					= usMapPos;
				bTargetLevel	= (INT8)gsInterfaceLevel;

				// Look for a target here...
				if ( gfUIFullTargetFound )
				{
					// Get target soldier, if one exists
					pTSoldier = MercPtrs[ gusUIFullTargetID ];
					sGridNo = pTSoldier->sGridNo;
					bTargetLevel = pSoldier->pathing.bLevel;

					if ( !HandleCheckForBadChangeToGetThrough( pSoldier, pTSoldier, sGridNo , bTargetLevel ) )
					{
						return;
					}
				}

				bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + 1 );

				if ( bFutureAim <= maxAimLevels )
				{
					sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, (INT8)(bFutureAim ) );

					// Determine if we can afford!
					if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
					{
						pSoldier->aiData.bShownAimTime+= 1;
						if ( pSoldier->aiData.bShownAimTime > maxAimLevels )
						{
							pSoldier->aiData.bShownAimTime = maxAimLevels;
						}
					}
					// Else - goto first level!
					else
					{
						if ( !gfDisplayFullCountRing )
						{
							gfDisplayFullCountRing = TRUE;
						}
						else
						{
							pSoldier->aiData.bShownAimTime = REFINE_AIM_1;
							gfDisplayFullCountRing = FALSE;
						}
					}
				}
				else
				{
					if ( !gfDisplayFullCountRing )
					{
						gfDisplayFullCountRing = TRUE;
					}
					else
					{
						pSoldier->aiData.bShownAimTime = REFINE_AIM_1;
						gfDisplayFullCountRing = FALSE;
					}
				}
			}
			break;


		case PUNCHCURS:

			// SANDRO - changed the formula here to make aimed punch less expensive for APS
			bAimTimeAddedForPunch = (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches : REFINE_PUNCH_2);

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + bAimTimeAddedForPunch );

			if ( bFutureAim <= bAimTimeAddedForPunch )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= bAimTimeAddedForPunch;

					if ( pSoldier->aiData.bShownAimTime > bAimTimeAddedForPunch )
					{
						pSoldier->aiData.bShownAimTime = bAimTimeAddedForPunch;
					}
				}
				// Else - goto first level!
				else
				{
					if ( !gfDisplayFullCountRing )
					{
						gfDisplayFullCountRing = TRUE;
					}
					else
					{
						pSoldier->aiData.bShownAimTime = REFINE_PUNCH_1;
						gfDisplayFullCountRing = FALSE;
					}
				}
			}
			else
			{
				if ( !gfDisplayFullCountRing )
				{
					gfDisplayFullCountRing = TRUE;
				}
				else
				{
					pSoldier->aiData.bShownAimTime = REFINE_PUNCH_1;
					gfDisplayFullCountRing = FALSE;
				}
			}
			break;


		case KNIFECURS:

			// SANDRO - changed the formula here to make aimed blade attack less expensive for APS
			bAimTimeAddedForKnife = (gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedBladedAttackes : REFINE_KNIFE_2);

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + bAimTimeAddedForKnife );

			if ( bFutureAim <= bAimTimeAddedForKnife )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= bAimTimeAddedForKnife;

					if ( pSoldier->aiData.bShownAimTime > bAimTimeAddedForKnife )
					{
						pSoldier->aiData.bShownAimTime = bAimTimeAddedForKnife;
					}
				}
				// Else - goto first level!
				else
				{
					if ( !gfDisplayFullCountRing )
					{
						gfDisplayFullCountRing = TRUE;
					}
					else
					{
						pSoldier->aiData.bShownAimTime = REFINE_KNIFE_1;
						gfDisplayFullCountRing = FALSE;
					}
				}
			}
			else
			{
				if ( !gfDisplayFullCountRing )
				{
					gfDisplayFullCountRing = TRUE;
				}
				else
				{
					pSoldier->aiData.bShownAimTime = REFINE_KNIFE_1;
					gfDisplayFullCountRing = FALSE;
				}
			}
			break;

		case TOSSCURS:

			//IncrementAimCubeUI( );
			break;

		default:

			ErasePath( TRUE );

	}

}


UINT8 GetActionModeCursor( SOLDIERTYPE *pSoldier )
{
	UINT8					ubCursor;
	UINT16				usInHand;

	// If we are an EPC, do nothing....
	//if ( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
	//{
	//	return( INVALIDCURS );
	//}

	// AN EPC is always not - attackable unless they are a robot!
	if ( AM_AN_EPC( pSoldier ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT ) )
	{
		return( INVALIDCURS );
	}

	// ATE: if a vehicle.... same thing
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return( INVALIDCURS );
	}

	// If we can't be controlled, returninvalid...
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_ROBOT )
	{
		if ( !pSoldier->CanRobotBeControlled( ) )
		{
			// Display message that robot cannot be controlled....
			return( INVALIDCURS );
		}
	}


	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL )
	{
		return( TRAJECTORYCURS );
	}
	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || ( pSoldier->bWeaponMode == WM_BURST && Item[pSoldier->inv[HANDPOS].usItem].grenadelauncher ) )
	{
		if ( gGameSettings.fOptions [ TOPTION_GL_BURST_CURSOR ] )
			return( TARGETCURS );
		else
			return ( TRAJECTORYCURS );	
	}
	if ( pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO || ( pSoldier->bWeaponMode == WM_AUTOFIRE && Item[pSoldier->inv[HANDPOS].usItem].grenadelauncher ) )
	{
		if ( gGameSettings.fOptions [ TOPTION_GL_BURST_CURSOR ] )
			return( TARGETCURS );
		else
			return ( TRAJECTORYCURS );
	}

	usInHand = pSoldier->GetUsedWeaponNumber( &pSoldier->inv[HANDPOS] );
	// Start off with what is in our hand
	ubCursor = Item[ usInHand ].ubCursor;

	// OK, check if what is in our hands has a detonator attachment...
	// Detonators can only be on invalidcurs things...
	if ( ubCursor == INVALIDCURS )
	{
		if ( IsDetonatorAttached( &(pSoldier->inv[HANDPOS]) ) )
		{
			ubCursor = BOMBCURS;
		}
		else if ( IsRemoteDetonatorAttached( &(pSoldier->inv[HANDPOS]) ) )
		{
			ubCursor = BOMBCURS;
		}
	}

	// Now check our terrain to see if we cannot do the action now...
	if ( WaterTooDeepForAttacks( pSoldier->sGridNo) )
	{
		ubCursor = INVALIDCURS;
	}

	// If we are out of breath, no cursor...
	if ( pSoldier->bBreath < OKBREATH && pSoldier->bCollapsed )
	{
		ubCursor = INVALIDCURS;
	}

	return( ubCursor );
}

// Switch on item, display appropriate feedback cursor for a click....
void HandleUICursorRTFeedback( SOLDIERTYPE *pSoldier )
{
	UINT8 ubItemCursor;

	ubItemCursor	=	GetActionModeCursor( pSoldier );

	switch( ubItemCursor )
	{
		case TARGETCURS:

			if ( pSoldier->bDoBurst )
			{
				//BeginDisplayTimedCursor( ACTION_TARGETREDBURST_UICURSOR, 500 );
			}
			else
			{
				if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass == IC_THROWING_KNIFE )
				{
					BeginDisplayTimedCursor( RED_THROW_UICURSOR, 500 );
				}
				else
				{
					BeginDisplayTimedCursor( ACTION_TARGETRED_UICURSOR, 500 );
				}
			}
			break;

		default:

			break;
	}

}


/* good code from lesh
BOOLEAN CanSoldierAffordAimTime( SOLDIERTYPE *pSoldier, INT16 usMapPos, INT8 bAimTime )
{
	INT8		maxAimLevels;
	INT16		sAPCosts;

	maxAimLevels = AllowedAimingLevels(pSoldier);

	if ( bAimTime < REFINE_AIM_1 || bAimTime > maxAimLevels )
		return FALSE;
	
	sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, bAimTime );
	
	if ( !EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
		return FALSE;

	return TRUE;
}

BOOLEAN CanSoldierAffordPunching( SOLDIERTYPE *pSoldier, INT16 usMapPos, INT8 bRefineTime )
{
	INT16		sAPCosts;

	if ( bRefineTime < REFINE_PUNCH_1 || bRefineTime > REFINE_PUNCH_2 )
		return FALSE;
	
	sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, bRefineTime );
	
	if ( !EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
		return FALSE;

	return TRUE;
}

BOOLEAN CanSoldierAffordKnifing( SOLDIERTYPE *pSoldier, INT16 usMapPos, INT8 bRefineTime )
{
	INT16		sAPCosts;

	if ( bRefineTime < REFINE_KNIFE_1 || bRefineTime > REFINE_KNIFE_2 )
		return FALSE;
	
	sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, bRefineTime );
	
	if ( !EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
		return FALSE;

	return TRUE;
}

BOOLEAN CanSoldierAffordBurstBullets( SOLDIERTYPE *pSoldier, INT16 usMapPos )
{
	INT16		sAPCosts;
	INT8		bSaveDoAutofire;

	if ( pSoldier->bDoAutofire < 1 )
	{
		pSoldier->bDoAutofire = 1;
		return FALSE;
	}
	
	if ( pSoldier->bDoAutofire > pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft )
	{
		pSoldier->bDoAutofire = pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft;
		return FALSE;
	}

	sAPCosts = CalcTotalAPsToAttack( pSoldier, usMapPos, TRUE, 0);

	if ( !EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
		return FALSE;
	
	return TRUE;
}


void HandleWheelAdjustCursor( SOLDIERTYPE *pSoldier, INT16 sMapPos, INT16 sDelta )
{
	UINT8					ubCursor;
	INT8					bFutureAim;	
	
	ubCursor =  GetActionModeCursor( pSoldier );

	// 'snap' cursor to target tile....
	if ( gfUIFullTargetFound )
	{
		sMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;
	}

	switch( ubCursor )
	{
		case TARGETCURS:
			// warn if bad chance to get through
			if ( gfUIFullTargetFound &&
				!HandleCheckForBadChangeToGetThrough( pSoldier, MercPtrs[ gusUIFullTargetID ],
							MercPtrs[ gusUIFullTargetID ]->sGridNo , MercPtrs[ gusUIFullTargetID ]->pathing.bLevel ) )
			{
				return;
			}

			if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire )
			{
			}
			else if( pSoldier->bDoAutofire )
			{
				// AUTOFIRE section
				INT16 sCurAPCosts, sFutureAPCost;

				if(pSoldier->flags.autofireLastStep)
				{
					pSoldier->flags.autofireLastStep = FALSE;
				}

				// change burst length until AP difference is shown
				sCurAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
				do
				{
					pSoldier->bDoAutofire += sDelta;
					sFutureAPCost = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
				} while ( CanSoldierAffordBurstBullets( pSoldier, sMapPos ) && (sFutureAPCost - sCurAPCosts) == 0 );
				
				// complement burst by adding bullets, until next bullet produce AP difference
				sFutureAPCost = sCurAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
				while ( CanSoldierAffordBurstBullets( pSoldier, sMapPos ) && (sFutureAPCost - sCurAPCosts) == 0 )
				{
					pSoldier->bDoAutofire += 1;	// now it goes as future burst length
					sFutureAPCost = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
				}

				// display red numbers, if it is last possible burst ap spenditure
				pSoldier->flags.autofireLastStep = !CanSoldierAffordBurstBullets( pSoldier, sMapPos );
				
				// restore last iteration bullet count (before ap difference)
				pSoldier->bDoAutofire--;
			}
			else
			{
				// SINGLE SHOT section
				bFutureAim = pSoldier->aiData.bShownAimTime + sDelta;
				
				if ( CanSoldierAffordAimTime( pSoldier, sMapPos, bFutureAim) )
				{
					pSoldier->aiData.bShownAimTime += sDelta;
					// display yellow circle, if it is last possible aim level
					gfDisplayFullCountRing = !CanSoldierAffordAimTime( pSoldier, sMapPos, bFutureAim + 1);
					
				}
			}
			break;
			
		case PUNCHCURS:
			// PUNCHING section
			bFutureAim = pSoldier->aiData.bShownAimTime + sDelta * REFINE_PUNCH_2;	// possibly dirty hack
			
			if ( CanSoldierAffordPunching( pSoldier, sMapPos, bFutureAim) )
			{
				pSoldier->aiData.bShownAimTime += sDelta * REFINE_PUNCH_2;
				// display yellow circle, if it is last possible aim level
				gfDisplayFullCountRing = !CanSoldierAffordPunching( pSoldier, sMapPos, bFutureAim + 1);
			}
			break;
		
		case KNIFECURS:
			// KNIFING section
			bFutureAim = pSoldier->aiData.bShownAimTime + sDelta * REFINE_KNIFE_2;	// possibly dirty hack
			
			if ( CanSoldierAffordKnifing( pSoldier, sMapPos, bFutureAim) )
			{
				pSoldier->aiData.bShownAimTime += sDelta * REFINE_KNIFE_2;
				// display yellow circle, if it is last possible aim level
				gfDisplayFullCountRing = !CanSoldierAffordKnifing( pSoldier, sMapPos, bFutureAim + 1);
			}
			break;

	};
}




*/
// HEADROCK HAM B2.7: This function gives us an "approximate" CTH bar, that doesn't show the exact value of
// CTH unless the character is well-trained. The worse trained a character is, the wilder the speculation.

UINT32 ChanceToHitApproximation( SOLDIERTYPE * pSoldier, UINT32 uiChance )
{
	UINT16 bExpLevelValue = EffectiveExpLevel(pSoldier); // SANDRO - changed to effective level calc
	UINT16 bMarksmanship = pSoldier->stats.bMarksmanship;
	UINT16 bWisdom = pSoldier->stats.bWisdom;
	UINT16 iNumStages, iNumStagesBase, StageSize, SubStageSize;
	UINT8 iSniper;
	UINT16 uiNewChance;
	UINT16 i;

	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		if ( gGameOptions.fNewTraitSystem ) // old/new traits - SANDRO
			iSniper = NUM_SKILL_TRAITS( pSoldier, SNIPER_NT );
		else
			iSniper = NUM_SKILL_TRAITS( pSoldier, PROF_SNIPER_OT );
	}
	else
	{
		// Dunno, but I get the strong feeling that we don't want CTH bars for non-mercs.
		return(0);
	}

	if (uiChance == 100)
	{
		// Sure hit. The player needs to set Maximum_CTH to 100 for this to happen. Normally, the
		// max chance is only 99.
		return(100);
	}
	if (uiChance == 0)
	{
		// Sure Miss. The player needs to set Minimum_CTH to 0 for this to happen. Normally, the
		// min chance is 1.
		return(0);
	}

	uiNewChance = uiChance;
	iNumStagesBase = __min(165,__max(0, ((bExpLevelValue * 10) - 30) + (bMarksmanship - 25) + (bWisdom - 50)));
	//iNumStagesBase = __max(255, iNumStagesBase);
	
	iNumStagesBase = __min(165, iNumStagesBase + (10 * iSniper));

	iNumStagesBase = (UINT16)((float)iNumStagesBase * (float)((float)iNumStagesBase / 195.0));

	// Gives a number between 2 and 51.
	iNumStages = __max(2,(100 * iNumStagesBase) / 300);

	// Find out how large the slices are
	StageSize = 100/iNumStages;
	// Correct iNumStages to match StageSize, +1 compensates for the rounding down of StageSize.
	iNumStages = (100/StageSize) + 1;
	// These sub-stages are used to determine which way to round the results.
	SubStageSize = __max(1, StageSize / 2);

	if (StageSize == 1)
		// CTH assessment is perfect. No need for the rest of this function
		return (uiChance);

	// Jump each "stage" and see whether the CTH is within this slice of the scale
	for (i=0; i<iNumStages; i++)
	{
		UINT16 CurrentStage = i * StageSize;
		UINT16 NextStage = (i+1) * StageSize;
		// Check whether the CTH should be rounded down or up.
		if ( (CurrentStage <= (UINT16) uiChance) && (CurrentStage + SubStageSize > (UINT16) uiChance) )
		{
			return (CurrentStage);
		}
		else if ( (CurrentStage + SubStageSize <= (UINT16) uiChance) && (NextStage > (UINT16) uiChance))
		{
			return (NextStage);
		}
	}

	// Something went wrong. Return 50, as in "God Knows"
	return (50);
}


/* //почти отлаженный  хедроковский код. доделать прицелы для кулака и ножыга
void HandleWheelAdjustCursor( SOLDIERTYPE *pSoldier, INT16 sMapPos, INT16 sDelta )
{
	UINT16					usInHand;
	INT16					sAPCosts;
	INT8					bFutureAim;
	UINT8					ubCursor;
	SOLDIERTYPE				*pTSoldier;
	INT16					sGridNo;
	INT8					bTargetLevel;


	usInHand = pSoldier->inv[HANDPOS].usItem;

	ubCursor =	GetActionModeCursor( pSoldier );

	UINT8 maxAimLevels = AllowedAimingLevels(pSoldier);

	// 'snap' cursor to target tile....
	if ( gfUIFullTargetFound )
	{
		sMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;
	}


	switch( ubCursor )
	{
		case TARGETCURS:

			// CHECK IF GUY HAS IN HAND A WEAPON
			if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire)
			{
				// Do nothing!
				// pSoldier->aiData.bShownAimTime = REFINE_AIM_BURST;
			}
			else if(pSoldier->bDoAutofire)
			{
				INT16	sCurAPCosts;

				if(pSoldier->bDoAutofire==1 && sDelta<0) return;

				if(pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire )
				{
					//Calculate how many bullets we need to fire to add at least one more AP
					sAPCosts = sCurAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts <= sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire)	//Increment the bullet count until we run out of APs or we spend the whole AP
					{
						pSoldier->bDoAutofire++;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
					}

					//we've stepped over the border and used up one more ap, now let's make sure that it is spent to maximize the bullets

					sCurAPCosts = sAPCosts;

					do
					{
						pSoldier->bDoAutofire+=sDelta;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);
					}
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts == sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire);
					pSoldier->bDoAutofire--;

					sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, 0);

					if(!EnoughPoints( pSoldier, sAPCosts, 0, FALSE ))		//We've not enough points to fire those bullets
					{
						pSoldier->flags.autofireLastStep = TRUE;
						pSoldier->bDoAutofire--;
					}
					else
						pSoldier->flags.autofireLastStep = FALSE; //both last step conditions are false
				}
				else
				{
					if(sDelta<0) 
					{
						pSoldier->flags.autofireLastStep = FALSE;
					pSoldier->bDoAutofire--;
					}
					else
					pSoldier->flags.autofireLastStep = TRUE;
				}
			}
			else //single shoot
			{
				sGridNo					= sMapPos;
				bTargetLevel	= (INT8)gsInterfaceLevel;

				// Look for a target here...
				if ( gfUIFullTargetFound )
				{
					// Get target soldier, if one exists
					pTSoldier = MercPtrs[ gusUIFullTargetID ];
					sGridNo = pTSoldier->sGridNo;
					bTargetLevel = pSoldier->pathing.bLevel;

					if ( !HandleCheckForBadChangeToGetThrough( pSoldier, pTSoldier, sGridNo , bTargetLevel ) )
						return;
				}

				bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + sDelta );
				if(bFutureAim <0)return;

				if ( bFutureAim <= maxAimLevels )
				{
					sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim ) );

					// Determine if we can afford!
					if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
					{
						pSoldier->aiData.bShownAimTime+= sDelta;
						if ( pSoldier->aiData.bShownAimTime > maxAimLevels )
							pSoldier->aiData.bShownAimTime = maxAimLevels;
						gfDisplayFullCountRing = FALSE;
					}
					
					else // Else - goto first level!
						gfDisplayFullCountRing = TRUE;
				}
				else
					gfDisplayFullCountRing = TRUE;

			}
			break;


		case PUNCHCURS:

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + REFINE_PUNCH_2 );

			if ( bFutureAim <= REFINE_PUNCH_2 )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= (sDelta*REFINE_PUNCH_2);

					if ( pSoldier->aiData.bShownAimTime > REFINE_PUNCH_2 )
					{
						pSoldier->aiData.bShownAimTime = REFINE_PUNCH_2;
					}
				}
				// Else - goto first level!
				else
				{
					if ( !gfDisplayFullCountRing )
					{
						gfDisplayFullCountRing = TRUE;
					}
					else
					{
						pSoldier->aiData.bShownAimTime = REFINE_PUNCH_1;
						gfDisplayFullCountRing = FALSE;
					}
				}
			}
			else
			{
				if ( !gfDisplayFullCountRing )
				{
					gfDisplayFullCountRing = TRUE;
				}
				else
				{
					pSoldier->aiData.bShownAimTime = REFINE_PUNCH_1;
					gfDisplayFullCountRing = FALSE;
				}
			}
			break;


		case KNIFECURS:

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + (sDelta*REFINE_KNIFE_2) );

			if ( bFutureAim <= REFINE_KNIFE_2 )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= REFINE_KNIFE_2;

					if ( pSoldier->aiData.bShownAimTime > REFINE_KNIFE_2 )
					{
						pSoldier->aiData.bShownAimTime = REFINE_KNIFE_2;
					}
				}
				// Else - goto first level!
				else
				{
					if ( !gfDisplayFullCountRing )
					{
						gfDisplayFullCountRing = TRUE;
					}
					else
					{
						pSoldier->aiData.bShownAimTime = REFINE_KNIFE_1;
						gfDisplayFullCountRing = FALSE;
					}
				}
			}
			else
			{
				if ( !gfDisplayFullCountRing )
				{
					gfDisplayFullCountRing = TRUE;
				}
				else
				{
					pSoldier->aiData.bShownAimTime = REFINE_KNIFE_1;
					gfDisplayFullCountRing = FALSE;
				}
			}
			break;

		case TOSSCURS:

			//IncrementAimCubeUI( );
			break;

		default:

			ErasePath( TRUE );

	}
}
*/



void HandleWheelAdjustCursor( SOLDIERTYPE *pSoldier, INT32 sMapPos, INT32 sDelta, INT16 brstmode )
{
	UINT16					usInHand;
	INT16					sAPCosts;
	INT8					bFutureAim;
	UINT8					ubCursor;
	SOLDIERTYPE				*pTSoldier;
	INT32					sGridNo;
	INT8					bTargetLevel;


	usInHand = pSoldier->inv[HANDPOS].usItem;

	ubCursor =	GetActionModeCursor( pSoldier );

	UINT8 maxAimLevels = AllowedAimingLevels(pSoldier, sMapPos);

	// 'snap' cursor to target tile....
	if ( gfUIFullTargetFound )
		sMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;
	
	int aimlevelchange = 1;
	int maxAimCanAfford = 0;
	int oldAim = pSoldier->aiData.bShownAimTime;
	int hitChance = 0;
	int maxCtH = 0; //maximum chance to hit
	int minAimForCtH = 0;  //minimum AP needed to get maximum CtH
	switch( ubCursor )
	{
		case TARGETCURS:

//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"cbef=%d",CalcChanceToHitGun( pSoldier, sMapPos, pSoldier->aiData.bShownAimTime, pSoldier->bAimShotLocation ) );
		if(pSoldier->bDoAutofire && (gfKeyState[18] || brstmode) ) //накрутка кол-ва пулей в очереди
			{
				INT16	sCurAPCosts;

				if(pSoldier->bDoAutofire==1 && sDelta<0) return;

				if(pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire )
				{
					//Calculate how many bullets we need to fire to add at least one more AP
					sAPCosts = sCurAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts <= sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire)	//Increment the bullet count until we run out of APs or we spend the whole AP
					{
						pSoldier->bDoAutofire++;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}

					//we've stepped over the border and used up one more ap, now let's make sure that it is spent to maximize the bullets

					sCurAPCosts = sAPCosts;

					do
					{
						pSoldier->bDoAutofire+=sDelta;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts == sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire);
					pSoldier->bDoAutofire--;

					sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);

					if(!EnoughPoints( pSoldier, sAPCosts, 0, FALSE ))		//We've not enough points to fire those bullets
					{
						pSoldier->flags.autofireLastStep = TRUE;
						pSoldier->bDoAutofire--;
					}
					else
						pSoldier->flags.autofireLastStep = FALSE; //both last step conditions are false
				}
				else
				{
					if(sDelta<0) 
					{
						pSoldier->flags.autofireLastStep = FALSE;
					pSoldier->bDoAutofire--;
					}
					else
					pSoldier->flags.autofireLastStep = TRUE;
				}
				return;
			}
///////////////////////////////
				sGridNo					= sMapPos;
				bTargetLevel	= (INT8)gsInterfaceLevel;

				// Look for a target here...
				if ( gfUIFullTargetFound )
				{
					// Get target soldier, if one exists
					pTSoldier = MercPtrs[ gusUIFullTargetID ];
					sGridNo = pTSoldier->sGridNo;
					bTargetLevel = pSoldier->pathing.bLevel;

					if ( !HandleCheckForBadChangeToGetThrough( pSoldier, pTSoldier, sGridNo , bTargetLevel ) )
					{
						return;
					}
				}


				bFutureAim = maxAimLevels;

				//Find the actual maximum aim that can be done.				
				while( bFutureAim > 0 ){
					//Can afford this many?
					sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, bFutureAim );
					if( !maxAimCanAfford && EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) ){
						maxAimCanAfford = bFutureAim;
					}
					//Find the least amount of aiming needed to get maximum CtH
					//Give some slack, but don't let the slack accumulate.
					//dd???{
					//hitChance = CalcChanceToHitGun( pSoldier, sMapPos, bFutureAim, pSoldier->bAimShotLocation );
					//if( hitChance >= maxCtH - 3){
					//	minAimForCtH = bFutureAim;
					//	if(hitChance >= maxCtH) maxCtH = hitChance;
					//}
					//ddd???}
					//Loop to 0.
					--bFutureAim;
				}

				bFutureAim = oldAim + sDelta;
				if( bFutureAim < REFINE_AIM_1 ) return;


				//Do it.
				if( bFutureAim < REFINE_AIM_1 ){
					//Went too low.  Set to max aim.
					bFutureAim = maxAimCanAfford;
					gfDisplayFullCountRing = TRUE;
				}else if( bFutureAim <= maxAimCanAfford ){
					//Aim level is doable.  No modification needed.
					gfDisplayFullCountRing = FALSE;
				}else{
					//Aim too high.  Give indicator that is is maxed, then reset aim refinement.
					if ( gfDisplayFullCountRing )
						return;
					else{
						//Display ring to indicate maximum aim reached.
						bFutureAim = oldAim;
						gfDisplayFullCountRing = TRUE;
					}
				}
				//Apply.
				pSoldier->aiData.bShownAimTime = bFutureAim;

//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"caft=%d",CalcChanceToHitGun( pSoldier, sMapPos, pSoldier->aiData.bShownAimTime, pSoldier->bAimShotLocation ) );
			break;


		case PUNCHCURS:

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + (sDelta*REFINE_PUNCH_2) );
if(bFutureAim<0) return;
			if ( bFutureAim <= REFINE_PUNCH_2 )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= (sDelta*REFINE_PUNCH_2);
					if ( pSoldier->aiData.bShownAimTime > REFINE_PUNCH_2 )
						pSoldier->aiData.bShownAimTime = REFINE_PUNCH_2;
					gfDisplayFullCountRing = FALSE;
				}
				else
					gfDisplayFullCountRing = TRUE;
			}
			else
				gfDisplayFullCountRing = TRUE;
			
			break;
							


		case KNIFECURS:

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + (sDelta*REFINE_KNIFE_2) );
if(bFutureAim<0) return;
			if ( bFutureAim <= REFINE_KNIFE_2 )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= REFINE_KNIFE_2;
					if ( pSoldier->aiData.bShownAimTime > REFINE_KNIFE_2 )
						pSoldier->aiData.bShownAimTime = REFINE_KNIFE_2;
					gfDisplayFullCountRing = FALSE;
				}
				else
					gfDisplayFullCountRing = TRUE;
			}
			else
				gfDisplayFullCountRing = TRUE;

			break;

		case TOSSCURS:

			//IncrementAimCubeUI( );
			break;

		default:

			ErasePath( TRUE );

	}

}

//обработка колеса без прицельной очереди if прицельная очередь отключена в .ини
void HandleWheelAdjustCursorWOAB( SOLDIERTYPE *pSoldier, INT32 sMapPos, INT32 sDelta )
{
	UINT16					usInHand;
	INT16					sAPCosts;
	INT8					bFutureAim;
	UINT8					ubCursor;
	SOLDIERTYPE				*pTSoldier;
	INT32					sGridNo;
	INT8					bTargetLevel;


	usInHand = pSoldier->inv[HANDPOS].usItem;

	ubCursor =	GetActionModeCursor( pSoldier );

	UINT8 maxAimLevels = AllowedAimingLevels(pSoldier, sMapPos);

	// 'snap' cursor to target tile....
	if ( gfUIFullTargetFound )
	{
		sMapPos = MercPtrs[ gusUIFullTargetID ]->sGridNo;
	}


	switch( ubCursor )
	{
		case TARGETCURS:

			// CHECK IF GUY HAS IN HAND A WEAPON
			if ( pSoldier->bDoBurst && !pSoldier->bDoAutofire)
			{
				// Do nothing!
				// pSoldier->aiData.bShownAimTime = REFINE_AIM_BURST;
			}
			else if(pSoldier->bDoAutofire)
			{
				INT16	sCurAPCosts;

				if(pSoldier->bDoAutofire==1 && sDelta<0) return;

				if(pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire )
				{
					//Calculate how many bullets we need to fire to add at least one more AP
					sAPCosts = sCurAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts <= sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft > pSoldier->bDoAutofire)	//Increment the bullet count until we run out of APs or we spend the whole AP
					{
						pSoldier->bDoAutofire++;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}

					//we've stepped over the border and used up one more ap, now let's make sure that it is spent to maximize the bullets

					sCurAPCosts = sAPCosts;

					do
					{
						pSoldier->bDoAutofire+=sDelta;
						sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);
					}
					while(EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) && sAPCosts == sCurAPCosts && pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire);
					pSoldier->bDoAutofire--;

					sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, pSoldier->aiData.bShownAimTime);

					if(!EnoughPoints( pSoldier, sAPCosts, 0, FALSE ))		//We've not enough points to fire those bullets
					{
						pSoldier->flags.autofireLastStep = TRUE;
						pSoldier->bDoAutofire--;
					}
					else
						pSoldier->flags.autofireLastStep = FALSE; //both last step conditions are false
				}
				else
				{
					if(sDelta<0) 
					{
						pSoldier->flags.autofireLastStep = FALSE;
					pSoldier->bDoAutofire--;
					}
					else
					pSoldier->flags.autofireLastStep = TRUE;
				}
			}
			else //single shoot
			{
				sGridNo					= sMapPos;
				bTargetLevel	= (INT8)gsInterfaceLevel;

				// Look for a target here...
				if ( gfUIFullTargetFound )
				{
					// Get target soldier, if one exists
					pTSoldier = MercPtrs[ gusUIFullTargetID ];
					sGridNo = pTSoldier->sGridNo;
					bTargetLevel = pSoldier->pathing.bLevel;

					if ( !HandleCheckForBadChangeToGetThrough( pSoldier, pTSoldier, sGridNo , bTargetLevel ) )
						return;
				}

				bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + sDelta );
				if(bFutureAim <0)return;

				if ( bFutureAim <= maxAimLevels )
				{
					sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim ) );

					// Determine if we can afford!
					if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
					{
						pSoldier->aiData.bShownAimTime+= sDelta;
						if ( pSoldier->aiData.bShownAimTime > maxAimLevels )
							pSoldier->aiData.bShownAimTime = maxAimLevels;
						gfDisplayFullCountRing = FALSE;
					}
					
					else // Else - goto first level!
						gfDisplayFullCountRing = TRUE;
				}
				else
					gfDisplayFullCountRing = TRUE;

			}
			break;

		case PUNCHCURS:

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + (sDelta*REFINE_PUNCH_2) );
if(bFutureAim<0) return;
			if ( bFutureAim <= REFINE_PUNCH_2 )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= (sDelta*REFINE_PUNCH_2);
					if ( pSoldier->aiData.bShownAimTime > REFINE_PUNCH_2 )
						pSoldier->aiData.bShownAimTime = REFINE_PUNCH_2;
					gfDisplayFullCountRing = FALSE;
				}
				else
					gfDisplayFullCountRing = TRUE;
			}
			else
				gfDisplayFullCountRing = TRUE;
			
			break;
							


		case KNIFECURS:

			bFutureAim = (INT8)( pSoldier->aiData.bShownAimTime + (sDelta*REFINE_KNIFE_2) );
if(bFutureAim<0) return;
			if ( bFutureAim <= REFINE_KNIFE_2 )
			{
				sAPCosts = CalcTotalAPsToAttack( pSoldier, sMapPos, TRUE, (INT8)(bFutureAim / 2) );

				// Determine if we can afford!
				if ( EnoughPoints( pSoldier, sAPCosts, 0, FALSE ) )
				{
					pSoldier->aiData.bShownAimTime+= REFINE_KNIFE_2;
					if ( pSoldier->aiData.bShownAimTime > REFINE_KNIFE_2 )
						pSoldier->aiData.bShownAimTime = REFINE_KNIFE_2;
					gfDisplayFullCountRing = FALSE;
				}
				else
					gfDisplayFullCountRing = TRUE;
			}
			else
				gfDisplayFullCountRing = TRUE;

			break;
	
		case TOSSCURS:

			//IncrementAimCubeUI( );
			break;

		default:

			ErasePath( TRUE );

	}
}