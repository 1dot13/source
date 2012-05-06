#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include <stdio.h>
#include <string.h>
#include "wcheck.h"
#include "stdlib.h"
#include "debug.h"
#include "MemMan.h"
#include "Overhead Types.h"
#include "Soldier Control.h" // I need this here - SANDRO
#include "Animation Cache.h"
#include "Animation Data.h"
#include "Animation Control.h"
#include "weapons.h"
#include "soldier ani.h"
#include "Random.h"
#include "video.h"
#include "vobject_blitters.h"
#include "Sound Control.h"
#include "isometric utils.h"
#include "Handle UI.h"
#include "Event Pump.h"
#include "opplist.h"
#include "lighting.h"
#include "ai.h"
#include "renderworld.h"
#include "interactive tiles.h"
#include "points.h"
#include "message.h"
#include "world items.h"
#include "physics.h"
#include "soldier create.h"
#include "dialogue control.h"
#include "soldier functions.h"
#include "rotting corpses.h"
#include "merc entering.h"
#include "soldier add.h"
#include "soldier profile.h"
#include "soldier functions.h"
#include "interface.h"
#include "qarray.h"
#include "soldier macros.h"
#include "Strategic Town Loyalty.h"
#include "Squads.h"
#include "worldman.h"
#include "structure wrap.h"
#include "pathai.h"
#include "pits.h"
#include "text.h"
#include "npc.h"
#include "meanwhile.h"
#include "Explosion Control.h"
#include "fov.h"
#include "campaign.h"
#include "los.h"
#include "GameSettings.h"
#include "Boxing.h"
#include "drugs and alcohol.h"
#include "Smell.h"
#include "GameSettings.h"
#include "interface dialogue.h"
#include "Strategic Status.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
#include "connect.h"
#include "fresh_header.h"
#define		NO_JUMP											0
#define		MAX_ANIFRAMES_PER_FLASH			2
//#define		TIME_FOR_RANDOM_ANIM_CHECK	10
#define		TIME_FOR_RANDOM_ANIM_CHECK	2

BOOLEAN		gfLastMercTalkedAboutKillingID = NOBODY;

extern void AddFuelToVehicle( SOLDIERTYPE *pSoldier, SOLDIERTYPE *pVehicle );


DOUBLE		gHopFenceForwardSEDist[ NUMSOLDIERBODYTYPES ] = { 2.2, 0.7, 3.2, 0.7 };
DOUBLE		gHopFenceForwardNWDist[ NUMSOLDIERBODYTYPES ] = { 2.7, 1.0, 2.7, 1.0 };
DOUBLE		gHopFenceForwardFullSEDist[ NUMSOLDIERBODYTYPES ] = { 1.1, 1.0, 2.1, 1.1 };
DOUBLE		gHopFenceForwardFullNWDist[ NUMSOLDIERBODYTYPES ] = { 0.8, 0.2, 2.7, 0.8 };
DOUBLE		gFalloffBackwardsDist[ NUMSOLDIERBODYTYPES ] = { 1, 0.8, 1, 1 };
DOUBLE		gClimbUpRoofDist[ NUMSOLDIERBODYTYPES ] = { 2, 0.1, 2, 2 };
DOUBLE		gClimbUpRoofLATDist[ NUMSOLDIERBODYTYPES ] = { 0.7, 0.5, 0.7, 0.5 };
DOUBLE		gClimbDownRoofStartDist[ NUMSOLDIERBODYTYPES ] = { 5.0, 1.0, 1, 1 };
DOUBLE		gClimbUpRoofDistGoingLower[ NUMSOLDIERBODYTYPES ] = { 0.9, 0.1, 1, 1 };



BOOLEAN HandleSoldierDeath( SOLDIERTYPE *pSoldier , BOOLEAN *pfMadeCorpse );

void CheckForAndHandleSoldierIncompacitated( SOLDIERTYPE *pSoldier );
BOOLEAN CheckForImproperFireGunEnd( SOLDIERTYPE *pSoldier );
BOOLEAN OKHeightDest( SOLDIERTYPE *pSoldier, INT32 sNewGridNo );
BOOLEAN HandleUnjamAnimation( SOLDIERTYPE *pSoldier );

extern void HandleSystemNewAISituation( SOLDIERTYPE *pSoldier, BOOLEAN fResetABC );
extern void PlaySoldierFootstepSound( SOLDIERTYPE *pSoldier );
extern UINT8 NumCapableEnemyInSector( );
extern BOOLEAN gfKillingGuysForLosingBattle;

extern UINT8 gubInterruptProvoker;

extern UINT16 PickSoldierReadyAnimation( SOLDIERTYPE *pSoldier, BOOLEAN fEndReady );

// Animation code explanations!
//
//	0-399:	Actual animation frame indices
// 400-499:	Object manipulation codes
// 500-598:	Jump to frame x-501, same state.	Obvious limit 0-97
// 600-699:	Jump to new state x-600, frame 0
// 700-798:	Play sound
// 800-999:	Jump to new state x-700, frame 0 (starts with 100)
//

BOOLEAN AdjustToNextAnimationFrame( SOLDIERTYPE *pSoldier )
{
	EV_S_FIREWEAPON			SFireWeapon;

	UINT16				sNewAniFrame, anAniFrame;
	INT8					ubCurrentHeight;
	UINT16				usOldAnimState;
	static UINT32 uiJumpAddress = NO_JUMP;
	INT32 sNewGridNo;
	INT16					sX, sY;
	BOOLEAN				fStop;
	UINT32				cnt;
	UINT8					ubDiceRoll;						// Percentile dice roll
	UINT8					ubRandomHandIndex;		// Index value into random animation table to use base don what is in the guys hand...
	UINT16				usItem;
	RANDOM_ANI_DEF	*pAnimDef;
	UINT8					ubNewDirection;
	UINT8					ubDesiredHeight;
	BOOLEAN				bOKFireWeapon;
	BOOLEAN				bWeaponJammed;
	BOOLEAN				fFreeUpAttacker=FALSE;
	UINT16		usUIMovementMode;

	do
	{
		// Get new frame code
		sNewAniFrame = gusAnimInst[ pSoldier->usAnimState ][ pSoldier->usAniCode ];

		// Handle muzzle flashes
		if ( pSoldier->bMuzFlashCount > 0 )
		{
			// FLash for about 3 frames
			if ( pSoldier->bMuzFlashCount > MAX_ANIFRAMES_PER_FLASH )
			{
				pSoldier->bMuzFlashCount = 0;
				if ( pSoldier->iMuzFlash != -1 )
				{
					LightSpriteDestroy( pSoldier->iMuzFlash );
					pSoldier->iMuzFlash = -1;
				}
			}
			else
			{
				pSoldier->bMuzFlashCount++;
			}

		}

		if ( pSoldier->bBreathCollapsed )
		{
			// ATE: If we have fallen, and we can't get up... no
			// really, if we were told to collapse but have been hit after, don't
			// do anything...
			if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_HITSTOP | ANIM_HITFINISH ) )
			{
				pSoldier->bBreathCollapsed = FALSE;
			}
			else if ( pSoldier->stats.bLife == 0 )
			{
				// Death takes precedence...
				pSoldier->bBreathCollapsed = FALSE;
			}
			else if ( pSoldier->usPendingAnimation == FALLFORWARD_ROOF || pSoldier->usPendingAnimation == FALLOFF || pSoldier->usAnimState == FALLFORWARD_ROOF || pSoldier->usAnimState == FALLOFF )
			{
				pSoldier->bBreathCollapsed = FALSE;
			}
			else
			{
				// Wait here until we are free....
				if ( !pSoldier->flags.fInNonintAnim )
				{
					// UNset UI
					UnSetUIBusy( pSoldier->ubID );

					SoldierCollapse( pSoldier );

					pSoldier->bBreathCollapsed = FALSE;

					return( TRUE );
				}
			}
		}

		// Check for special code
		if ( sNewAniFrame < 399 )
		{

			// Adjust / set true ani frame
			// Use -1 because ani files are 1-based, these are 0-based
			pSoldier->ConvertAniCodeToAniFrame( (INT16)( sNewAniFrame - 1 ) );

			// Adjust frame control pos, and try again
			pSoldier->usAniCode++;
			break;
		}
		else if ( sNewAniFrame < 500 )
		{
			// Switch on special code
			switch( sNewAniFrame )
			{
			case 402:

				// DO NOT MOVE FOR THIS FRAME
				pSoldier->flags.fPausedMove = TRUE;
				break;

			case 403:

				// MOVE GUY FORWARD SOME VALUE
				MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)0.7 );

				break;

			case 404:

				// MOVE GUY BACKWARD SOME VALUE
				// Use same function as forward, but is -ve values!
				MoveMercFacingDirection( pSoldier , TRUE, (FLOAT)1 );
				break;

			case 405:

				return( TRUE );

			case 406:

				// Move merc up
				if ( pSoldier->ubDirection == NORTH )
				{
					pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->sHeightAdjustment + 2 )	);
				}
				else
				{
					pSoldier->SetSoldierHeight( (FLOAT)( pSoldier->sHeightAdjustment + 3 ) );
				}
				break;

			case 408:

				// CODE: SPECIAL MOVE CLIMB UP ROOF EVENT

				// Moved here because this represents "already on the roof", so breath collapses and interrupts should
				// keep the soldier on the roof where he belongs
				// Move merc up specific height
				pSoldier->SetSoldierHeight( (FLOAT)50 );

				{
					INT16		sXPos, sYPos;

					//sNewGridNo = NewGridNo( pSoldier->sGridNo, (UINT16)DirectionInc( pSoldier->ubDirection ) );
					ConvertMapPosToWorldTileCenter( pSoldier->sTempNewGridNo, &sXPos, &sYPos );
					pSoldier->EVENT_SetSoldierPosition( (FLOAT)sXPos, (FLOAT)sYPos );
				}

				// re-enable sight
				gTacticalStatus.uiFlags &= (~DISALLOW_SIGHT);

				// Move two CC directions
				pSoldier->EVENT_SetSoldierDirection( gTwoCCDirection[ pSoldier->ubDirection ] );

				pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

				// Set desired anim height!
				pSoldier->ubDesiredHeight = ANIM_CROUCH;

				// Madd
				usUIMovementMode = pSoldier->GetMoveStateBasedOnStance( gAnimControl[ pSoldier->usAnimState ].ubEndHeight );
				pSoldier->usUIMovementMode = usUIMovementMode;

				// ATE: Change interface level.....
				// CJC: only if we are a player merc
				if (pSoldier->bTeam == gbPlayerNum)
				{

					if (gTacticalStatus.fAutoBandageMode)
					{
						// in autobandage, handle as AI, but add roof marker too
						FreeUpNPCFromRoofClimb( pSoldier );
						HandlePlacingRoofMarker( pSoldier, pSoldier->sGridNo, TRUE, TRUE );
					}
					else
					{
						// OK, UNSET INTERFACE FIRST
						UnSetUIBusy( pSoldier->ubID );

						if ( pSoldier->ubID == gusSelectedSoldier )
						{
							ChangeInterfaceLevel( 1 );
						}
						HandlePlacingRoofMarker( pSoldier, pSoldier->sGridNo, TRUE, TRUE );
					}
				}
				else
				{
					FreeUpNPCFromRoofClimb( pSoldier );
				}

				// ATE: Handle sight...
				HandleSight( pSoldier,SIGHT_LOOK | SIGHT_RADIO | SIGHT_INTERRUPT );
				break;

			case 409:

				//CODE: MOVE DOWN
				pSoldier->SetSoldierHeight( (FLOAT)( pSoldier->sHeightAdjustment - 2 ) );
				break;

			case 410:

				// Move merc down specific height
				pSoldier->SetSoldierHeight( (FLOAT)0 );
				break;

			case 411:

				// CODE: SPECIALMOVE CLIMB DOWN EVENT
				// Move two C directions
				pSoldier->EVENT_SetSoldierDirection( gTwoCDirection[ pSoldier->ubDirection ] );

				// Remove the roof marker
				HandlePlacingRoofMarker( pSoldier, pSoldier->sGridNo, FALSE, TRUE );

				pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );
				// Adjust height
				pSoldier->SetSoldierHeight( (FLOAT)gClimbDownRoofStartDist[ pSoldier->ubBodyType ] );
				// Adjust position
				MoveMercFacingDirection( pSoldier , TRUE, (FLOAT)3.5 );
				break;

			case 412:

				// CODE: HANDLING PRONE DOWN - NEED TO MOVE GUY BACKWARDS!
				MoveMercFacingDirection( pSoldier , FALSE, (FLOAT).2 );
				break;

			case 413:

				// CODE: HANDLING PRONE UP - NEED TO MOVE GUY FORWARDS!
				MoveMercFacingDirection( pSoldier , TRUE, (FLOAT).2 );
				break;

			case 430:
				{
					DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"AdjustToNextAnimationFrame: case 430");
					// SHOOT GUN
					// MAKE AN EVENT, BUT ONLY DO STUFF IF WE OWN THE GUY!
					SFireWeapon.usSoldierID			= pSoldier->ubID;
					SFireWeapon.uiUniqueId			= pSoldier->uiUniqueSoldierIdValue;
					SFireWeapon.sTargetGridNo		= pSoldier->sTargetGridNo;
					SFireWeapon.bTargetLevel		= pSoldier->bTargetLevel;
					SFireWeapon.bTargetCubeLevel= pSoldier->bTargetCubeLevel;
					if((is_server && pSoldier->ubID<120) || (!is_server && is_client && pSoldier->ubID<20) || (!is_server && !is_client) )
					{
						//only carry on if own werc
						AddGameEvent( S_FIREWEAPON, 0, &SFireWeapon );
				
						//hayden
						if(is_server || (is_client && pSoldier->ubID <20) ) 
							send_fireweapon( &SFireWeapon );
					}

					OBJECTTYPE* pObjHand = pSoldier->GetUsedWeapon( &pSoldier->inv[HANDPOS] );

					//DIGICRAB: Burst UnCap
					//Loop around in the animation if we still have burst rounds to fire
					if (pSoldier->bDoBurst
						&& (
						pSoldier->bDoBurst <= ( (pSoldier->bDoAutofire)?(pSoldier->bDoAutofire):(GetShotsPerBurst( pObjHand ))	) 
						|| (( pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST && pSoldier->bDoBurst <= Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS])].ubShotsPerBurst))
						   )
						)
					{
						if(pSoldier->usAnimState == 44 || pSoldier->usAnimState == 109 || pSoldier->usAnimState == 108 && pSoldier->usAniCode == 33) //we are standing, crounching or prone, firing the fast shot
							pSoldier->usAniCode = 3;
						else if(pSoldier->usAnimState == 175 && pSoldier->usAniCode == 37) //we are firing down to something very close, last shot
							pSoldier->usAniCode = 14;
					}

					OBJECTTYPE* pObjUsed = pSoldier->GetUsedWeapon( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
					UINT16 usedGun = pSoldier->GetUsedWeaponNumber( &pSoldier->inv[ pSoldier->ubAttackingHand ] );
					//DIGICRAB: Burst Sound
					//This code is stolen from Tactical\Weapons.c - UseGun(...)
					if (pSoldier->bDoBurst && pSoldier->iBurstSoundID == NO_SAMPLE && Weapon[ usedGun ].sSound != 0 && Item[ usedGun ].usItemClass != IC_THROWING_KNIFE )
					{
						// Switch on silencer...
						INT16 noisefactor = GetPercentNoiseVolume( pObjUsed );
						if( noisefactor < MAX_PERCENT_NOISE_VOLUME_FOR_SILENCED_SOUND || Weapon[ usedGun ].ubAttackVolume <= 10 )
						{
							INT32 uiSound;

							uiSound = Weapon [	usedGun ].silencedSound;
							//if ( Weapon[ pSoldier->usAttackingWeapon ].ubCalibre == AMMO9 || Weapon[ pSoldier->usAttackingWeapon ].ubCalibre == AMMO38 || Weapon[ pSoldier->usAttackingWeapon ].ubCalibre == AMMO57 )
							//{
							//	uiSound = S_SILENCER_1;
							//}
							//else
							//{
							//	uiSound = S_SILENCER_2;
							//}

							//randomize the rate a bit so that the sound is more believable
							PlayJA2Sample( uiSound, 44100-Random(5000)-Random(5000)-Random(5000), SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

						}
						else
						{
							INT8 volume = HIGHVOLUME;
							if ( noisefactor < 100 ) volume = (volume * noisefactor) / 100;
							//randomize the rate a bit so that the sound is more believable
							PlayJA2Sample( Weapon[ usedGun ].sSound, 44100-Random(5000)-Random(5000)-Random(5000), SoundVolume( volume, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
						}
					}
				}
				break;

			case 431:

				// FLASH FRAME WHITE
				pSoldier->pForcedShade = White16BPPPalette;
				break;

			case 432:

				// PLAY RANDOM IMPACT SOUND!
				//	PlayJA2Sample( (UINT8)( BULLET_IMPACT_1 + Random(3) ), RATE_11025, MIDVOLUME, 1, MIDDLEPAN );

				// PLAY RANDOM GETTING HIT SOUND
				//	pSoldier->DoMercBattleSound( BATTLE_SOUND_HIT1 );

				break;

			case 433:

				// CODE: GENERIC HIT!

				CheckForAndHandleSoldierIncompacitated( pSoldier );


				break;

			case 434:

				// JUMP TO ANOTHER ANIMATION ( BLOOD ) IF WE WANT BLOOD
				uiJumpAddress = pSoldier->usAnimState;
				pSoldier->ChangeSoldierState( FLYBACK_HIT_BLOOD_STAND, 0, FALSE );
				return( TRUE );
				break;

			case 435:

				// HOOK FOR A RETURN JUMP
				break;

			case 436:

				// Loop through script to find entry address
				if ( uiJumpAddress == NO_JUMP )
				{
					break;
				}
				usOldAnimState = pSoldier->usAnimState;
				pSoldier->usAniCode	= 0;

				do
				{
					// Get new frame code
					anAniFrame = gusAnimInst[ uiJumpAddress ][ pSoldier->usAniCode ];

					if ( anAniFrame == 435 )
					{
						// START PROCESSING HERE
						pSoldier->ChangeSoldierState( (UINT16)uiJumpAddress, pSoldier->usAniCode, FALSE );
						return( TRUE );
					}
					// Adjust frame control pos, and try again
					pSoldier->usAniCode++;
				}
				while( anAniFrame != 999 );

				uiJumpAddress = NO_JUMP;

				if ( anAniFrame == 999 )
				{
					// Fail jump, re-load old anim
					pSoldier->ChangeSoldierState( usOldAnimState, 0, FALSE );
					return( TRUE );
				}
				break;

			case 437:

				// CHANGE DIRECTION AND GET-UP
				//sGridNo = NewGridNo( pSoldier->sGridNo, (UINT16)(-1 * DirectionInc( pSoldier->ubDirection ) ) );
				//ConvertMapPosToWorldTileCenter( pSoldier->sGridNo, &sXPos, &sYPos );
				//SetSoldierPosition( pSoldier, (FLOAT)sXPos, (FLOAT)sYPos );


				// Reverse direction
				pSoldier->EVENT_SetSoldierDirection(	gOppositeDirection[ pSoldier->ubDirection ] );
				pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

				pSoldier->ChangeSoldierState( GETUP_FROM_ROLLOVER, 0 , FALSE );
				return( TRUE );

			case 438:

				//CODE: START HOLD FLASH
				pSoldier->flags.fForceShade = TRUE;
				break;

			case 439:

				//CODE: END HOLD FLASH
				pSoldier->flags.fForceShade = FALSE;
				break;

			case 440:
				//CODE: Set buddy as dead!
				{
					BOOLEAN fMadeCorpse;

					// ATE: Piggyback here on stopping the burn sound...
					if ( pSoldier->usAnimState == CHARIOTS_OF_FIRE ||
						pSoldier->usAnimState == BODYEXPLODING )
					{
						SoundStop( pSoldier->aiData.uiPendingActionData1 );
					}


					CheckForAndHandleSoldierDeath( pSoldier, &fMadeCorpse );

					if ( fMadeCorpse )
					{
						return( FALSE );
					}
					else
					{
						return( TRUE );
					}
				}
				break;

			case 441:
				// CODE: Show muzzle flash
				if ( pSoldier->bVisible == -1 )
				{
					break;
				}

				if ( !pSoldier->flags.fMuzzleFlash )
				{
					break;
				}

				// DO ONLY IF WE'RE AT A GOOD LEVEL
				if ( ubAmbientLightLevel < MIN_AMB_LEVEL_FOR_MERC_LIGHTS )
				{
					break;
				}

				if( ( pSoldier->iMuzFlash=LightSpriteCreate("L-R03.LHT", 0 ) )==(-1))
				{
					return( TRUE );
				}

				LightSpritePower(pSoldier->iMuzFlash, TRUE);
				// Get one move forward
				{
					INT32	usNewGridNo;
					INT16 sXPos, sYPos;

					usNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( pSoldier->ubDirection ) );
					ConvertGridNoToCenterCellXY( usNewGridNo, &sXPos, &sYPos );
					LightSpritePosition( pSoldier->iMuzFlash, (INT16)(sXPos/CELL_X_SIZE), (INT16)(sYPos/CELL_Y_SIZE));

					// Start count
					pSoldier->bMuzFlashCount = 1;
				}
				break;

			case 442:

				//CODE: FOR A NON-INTERRUPTABLE SCRIPT - SIGNAL DONE
				pSoldier->flags.fInNonintAnim = FALSE;

				// ATE: if it's the begin cower animation, unset ui, cause it could
				// be from player changin stance
				if ( pSoldier->usAnimState == START_COWER )
				{
					UnSetUIBusy( pSoldier->ubID );
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - if pending interrupt flag was set for after-attack type of interupt, try to resolve it now
				else if ( gGameOptions.fImprovedInterruptSystem )
				{
					if ( ResolvePendingInterrupt( pSoldier, AFTERACTION_INTERRUPT ) )
					{
						pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
						pSoldier->ubPendingDirection = NO_PENDING_DIRECTION;
						// "artificially" set lock ui flag in this case
						if (pSoldier->bTeam == gbPlayerNum)
						{
							AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
							guiPendingOverrideEvent = LU_BEGINUILOCK;								
							HandleTacticalUI( );
						}
						return( TRUE );		
					}
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				break;

			case 443:

				// MOVE GUY FORWARD FOR FENCE HOP ANIMATION
				switch( pSoldier->ubDirection )
				{
				case SOUTH:
				case EAST:

					MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)gHopFenceForwardSEDist[ pSoldier->ubBodyType ] );
					break;

				case NORTH:
				case WEST:
					MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)gHopFenceForwardNWDist[ pSoldier->ubBodyType ] );
					break;
				}
				break;

			case 444:

				// CODE: End Hop Fence
				// MOVE TO FORCASTED GRIDNO
				sX = CenterX( pSoldier->sForcastGridno );
				sY = CenterY( pSoldier->sForcastGridno );

				pSoldier->EVENT_InternalSetSoldierPosition( (FLOAT) sX, (FLOAT) sY, FALSE, FALSE, FALSE );
				pSoldier->EVENT_SetSoldierDirection(	gTwoCDirection[ pSoldier->ubDirection ] );
				pSoldier->sZLevelOverride = -1;
				pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );


				if ( gTacticalStatus.bBoxingState == BOXING_WAITING_FOR_PLAYER || gTacticalStatus.bBoxingState == PRE_BOXING || gTacticalStatus.bBoxingState == BOXING )
				{
					BoxingMovementCheck( pSoldier );
				}

				if ( SetOffBombsInGridNo( pSoldier->ubID, pSoldier->sGridNo, FALSE, pSoldier->pathing.bLevel ))
				{
					pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );
					return( TRUE );
				}

				// If we are at our final destination, goto stance of our movement stance...
				// Only in realtime...
				//if ( !( gTacticalStatus.uiFlags & INCOMBAT ) )
				// This has to be here to make guys continue along fence path
#if 0
				if ( pSoldier->sGridNo == pSoldier->pathing.sFinalDestination )
				{
					if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != gAnimControl[ pSoldier->usUIMovementMode ].ubEndHeight )
					{
						// Goto Stance...
						pSoldier->flags.fDontChargeAPsForStanceChange = TRUE;
						pSoldier->ChangeSoldierStance( gAnimControl[ pSoldier->usUIMovementMode ].ubEndHeight );
						return( TRUE );
					}
					else
					{
						//pSoldier->SoldierGotoStationaryStance( );

						// Set UI Busy
						UnSetUIBusy( pSoldier->ubID );
						//return( TRUE );
					}
				}
#endif
				break;

			case 445:

				// CODE: MOVE GUY FORWARD ONE TILE, BASED ON WHERE WE ARE FACING
				switch( pSoldier->ubDirection )
				{
				case SOUTH:
				case EAST:

					MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)gHopFenceForwardFullSEDist[ pSoldier->ubBodyType ] );
					break;

				case NORTH:
				case WEST:

					MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)gHopFenceForwardFullNWDist[ pSoldier->ubBodyType ] );
					break;

				}
				break;

			case 446:

				// CODE: Turn pause move flag on
				pSoldier->flags.uiStatusFlags |= SOLDIER_PAUSEANIMOVE;
				break;

			case 447:

				// TRY TO FALL!!!
				if ( pSoldier->flags.fTryingToFall )
				{
					INT16 sLastAniFrame;

					// TRY FORWARDS...
					// FIRST GRIDNO
						sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( pSoldier->ubDirection ) );

					if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, pSoldier->ubDirection, FALLFORWARD_HITDEATH_STOP ) )
					{
						// SECOND GRIDNO
						// sNewGridNo = NewGridNo( sNewGridNo, (UINT16)( DirectionInc( pSoldier->ubDirection ) ) );

						// if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, pSoldier->ubDirection, FALLFORWARD_HITDEATH_STOP ) )
						{
							// ALL'S OK HERE...
							pSoldier->flags.fTryingToFall = FALSE;
							break;
						}
					}


					// IF HERE, INCREMENT DIRECTION
					// ATE: Added Feb1 - can be either direction....
					if ( pSoldier->flags.fFallClockwise )
					{
						pSoldier->EVENT_SetSoldierDirection(	gOneCDirection[ pSoldier->ubDirection ] );
					}
					else
					{
						pSoldier->EVENT_SetSoldierDirection(	gOneCCDirection[ pSoldier->ubDirection ] );
					}
					pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );
					sLastAniFrame = gusAnimInst[ pSoldier->usAnimState ][ ( pSoldier->usAniCode - 2 ) ];
					pSoldier->ConvertAniCodeToAniFrame( (INT16)( sLastAniFrame ) );

					if ( pSoldier->ubDirection == pSoldier->bStartFallDir )
					{
						// GO FORWARD HERE...
						pSoldier->flags.fTryingToFall = FALSE;
						break;;
					}
					// IF HERE, RETURN SO WE DONOT INCREMENT DIR
					return( TRUE );
				}
				break;


			case 448:
				{
					// CODE: HANDLE BURST
					// FIRST CHECK IF WE'VE REACHED MAX FOR GUN
					fStop = FALSE;

					OBJECTTYPE* pObjHand = pSoldier->GetUsedWeapon( &pSoldier->inv[HANDPOS] );

					if ( ( pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST && pSoldier->bDoBurst > Weapon[GetAttachedGrenadeLauncher(&pSoldier->inv[HANDPOS])].ubShotsPerBurst) 
						|| (pSoldier->bWeaponMode != WM_ATTACHED_GL_BURST && pSoldier->bDoBurst > ((pSoldier->bDoAutofire)?(pSoldier->bDoAutofire):(GetShotsPerBurst( pObjHand )))) )
					{
						DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"AdjustToNextAnimationFrame: Burst case 448, stopping because burst size too large");
						fStop = TRUE;
						fFreeUpAttacker = TRUE;
					}

					// CHECK IF WE HAVE AMMO LEFT, IF NOT, END ANIMATION!
					if ( !EnoughAmmo( pSoldier, FALSE, pSoldier->ubAttackingHand ) )
					{
						DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"AdjustToNextAnimationFrame: Burst case 448, stopping because not enough ammo");
						fStop = TRUE;
						fFreeUpAttacker = TRUE;
						if ( pSoldier->bTeam == gbPlayerNum	)
						{
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ BURST_FIRE_DEPLETED_CLIP_STR ] );
						}
					}
					// Why only check for a jam on the first bullet?
					else if (pSoldier->bDoBurst == 1)
					{
						// CHECK FOR GUN JAM
						bWeaponJammed = CheckForGunJam( pSoldier );
						if ( bWeaponJammed == TRUE )
						{
							DebugMsg(TOPIC_JA2,DBG_LEVEL_3,"AdjustToNextAnimationFrame: Burst case 448, stopping because weapon jammed");
							fStop = TRUE;
							fFreeUpAttacker = TRUE;
							// stop shooting!
							// Yeah, but what does this have to do with that?
							//							pSoldier->bBulletsLeft = 0;

							// OK, Stop burst sound...
							if ( pSoldier->iBurstSoundID != NO_SAMPLE )
							{
								SoundStop( pSoldier->iBurstSoundID );
							}

							if ( pSoldier->bTeam == gbPlayerNum	)
							{
								PlayJA2Sample( S_DRYFIRE1, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
								//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Gun jammed!" );
							}

							DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - aborting start of attack due to burst gun jam") );
							FreeUpAttacker( );
						}
						else if ( bWeaponJammed == 255 )
						{
							// Play intermediate animation...
							if ( HandleUnjamAnimation( pSoldier ) )
							{
								return( TRUE );
							}
						}
					}

					if ( fStop )
					{
						if(pSoldier->bDoAutofire) //reset the autofire cursor after firing
						{
							pSoldier->flags.autofireLastStep = FALSE;
							pSoldier->bDoAutofire = 1;
						}

						pSoldier->flags.fDoSpread = FALSE;
						pSoldier->bDoBurst = 1;
						// pSoldier->flags.fBurstCompleted = TRUE;
						if ( fFreeUpAttacker )
						{
							// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - aborting start of attack") );
							// FreeUpAttacker( pSoldier->ubID );
						}

						// ATE; Reduce it due to animation being stopped...
						// 0verhaul: No longer necessary or desired
						// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - Burst animation ended") );
						// ReduceAttackBusyCount( pSoldier->ubID, FALSE );


						if ( CheckForImproperFireGunEnd( pSoldier ) )
						{
							return( TRUE );
						}

						// END: GOTO AIM STANCE BASED ON HEIGHT
						// If we are a robot - we need to do stuff different here
						// 0verhaul:	Ya know, if the robot simply used the same animation for standing and rifle standing,
						// we probably wouldn't need this special case code.
						if ( AM_A_ROBOT( pSoldier ) )
						{
							pSoldier->ChangeSoldierState( STANDING, 0 , FALSE );
						}
						else
						{
							switch ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
							{
							case ANIM_STAND:
								pSoldier->ChangeSoldierState( AIM_RIFLE_STAND, 0 , FALSE );
								break;

							case ANIM_PRONE:
								pSoldier->ChangeSoldierState( AIM_RIFLE_PRONE, 0 , FALSE );
								break;

							case ANIM_CROUCH:
								pSoldier->ChangeSoldierState( AIM_RIFLE_CROUCH, 0 , FALSE );
								break;

							}
						}
						return( TRUE );
					}

					// MOVETO CURRENT SPREAD LOCATION
					if ( pSoldier->flags.fDoSpread )
					{
						if ( pSoldier->sSpreadLocations[ pSoldier->flags.fDoSpread - 1 ] != 0 )
						{
							pSoldier->EVENT_SetSoldierDirection( (INT8)GetDirectionToGridNoFromGridNo( pSoldier->sGridNo, pSoldier->sSpreadLocations[ pSoldier->flags.fDoSpread - 1 ] ) );
							pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );
						}
					}
				}
				break;

			case 449:

				//CODE: FINISH BURST
				pSoldier->flags.fDoSpread = FALSE;
				pSoldier->bDoBurst = 1;
				//				pSoldier->flags.fBurstCompleted = TRUE;
				break;

			case 450:

				//CODE: BEGINHOPFENCE
				// MOVE TWO FACGIN GRIDNOS
				sNewGridNo = NewGridNo( pSoldier->sGridNo, (UINT16)( DirectionInc( pSoldier->ubDirection ) ) );
				//dddokno{
					if ( gubWorldMovementCosts[ sNewGridNo ][ (UINT8)pSoldier->pathing.usPathingData[ pSoldier->pathing.usPathIndex ] ][ pSoldier->pathing.bLevel ] == TRAVELCOST_FENCE )
						sNewGridNo = NewGridNo( sNewGridNo, (UINT16)( DirectionInc( pSoldier->ubDirection ) ) );
				//dddokno}

				//comm by ddd
				//sNewGridNo = NewGridNo( sNewGridNo, (UINT16)( DirectionInc( pSoldier->ubDirection ) ) );
				pSoldier->sForcastGridno = sNewGridNo;
				break;


			case 451:

				// CODE: MANAGE START z-buffer override
				switch( pSoldier->ubDirection )
				{
				case NORTH:
				case WEST:

					pSoldier->sZLevelOverride = TOPMOST_Z_LEVEL;
					break;
				}
				break;

			case 452:

				// CODE: MANAGE END z-buffer override
				switch( pSoldier->ubDirection )
				{
				case SOUTH:
				case EAST:

					pSoldier->sZLevelOverride = TOPMOST_Z_LEVEL;
					break;

				case NORTH:
				case WEST:

					pSoldier->sZLevelOverride = -1;
					break;

				}
				break;

			case 453:

				//CODE: FALLOFF ROOF ( BACKWARDS ) - MOVE BACK SOME!
				// Use same function as forward, but is -ve values!
				MoveMercFacingDirection( pSoldier , TRUE, (FLOAT)gFalloffBackwardsDist[ pSoldier->ubBodyType ] );
				break;

			case 454:

				// CODE: HANDLE CLIMBING ROOF,
				// Move merc up
				if ( pSoldier->ubDirection == NORTH )
				{
					pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment + gClimbUpRoofDist[ pSoldier->ubBodyType ] ) );
				}
				else
				{
					pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment + gClimbUpRoofDist[ pSoldier->ubBodyType ] ) );
				}
				break;

			case 455:

				// MOVE GUY FORWARD SOME VALUE
				MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)gClimbUpRoofLATDist[ pSoldier->ubBodyType ] );

				// MOVE DOWN SOME VALUE TOO!
				pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment - gClimbUpRoofDistGoingLower[ pSoldier->ubBodyType ] ) );

				break;

			case 456:

				// CODE: HANDLE CLIMBING ROOF,
				// Move merc DOWN
				if ( pSoldier->ubDirection == NORTH )
				{
					pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment - gClimbUpRoofDist[ pSoldier->ubBodyType ] ) );
				}
				else
				{
					pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment - gClimbUpRoofDist[ pSoldier->ubBodyType ] ) );
				}
				break;

			case 457:

				// CODE: CHANGCE STANCE TO STANDING
				SendChangeSoldierStanceEvent( pSoldier, ANIM_STAND );
				break;

			case 459:

				// CODE: CHANGE ATTACKING TO FIRST HAND
				pSoldier->ubAttackingHand = HANDPOS;
				pSoldier->usAttackingWeapon = pSoldier->inv[pSoldier->ubAttackingHand].usItem;
				// Adjust fReloading to FALSE
				pSoldier->flags.fReloading = FALSE;
				break;

			case 458:

				// CODE: CHANGE ATTACKING TO SECOND HAND
				pSoldier->ubAttackingHand = SECONDHANDPOS;
				pSoldier->usAttackingWeapon = pSoldier->inv[pSoldier->ubAttackingHand].usItem;
				// Adjust fReloading to FALSE
				pSoldier->flags.fReloading = FALSE;
				break;

			case 460:
			case 461:

				//CODE: THORW ITEM
				// Launch ITem!
				if ( pSoldier->pTempObject != NULL && pSoldier->pThrowParams != NULL )
				{
					// ATE: If we are armmed...
					if ( pSoldier->pThrowParams->ubActionCode == THROW_ARM_ITEM )
					{
						//AXP 25.03.2007: MinAPsToThrow now actually returns the real cost, not 0
						// ATE: Deduct points!
						DeductPoints( pSoldier, MinAPsToThrow( pSoldier, pSoldier->sTargetGridNo, FALSE ), 0, AFTERACTION_INTERRUPT );
					}
					else
					{
						// ATE: Deduct points!
						DeductPoints( pSoldier, APBPConstants[AP_TOSS_ITEM], 0, AFTERACTION_INTERRUPT );
					}

					INT32 iRealObjectID = CreatePhysicalObject( pSoldier->pTempObject, pSoldier->pThrowParams->dLifeSpan,	pSoldier->pThrowParams->dX, pSoldier->pThrowParams->dY, pSoldier->pThrowParams->dZ, pSoldier->pThrowParams->dForceX, pSoldier->pThrowParams->dForceY, pSoldier->pThrowParams->dForceZ, pSoldier->ubID, pSoldier->pThrowParams->ubActionCode, pSoldier->pThrowParams->uiActionData, FALSE );

					// OJW - 20091002 - Explosives
					if (is_networked && is_client)
					{
						if (pSoldier->bTeam == 0 || (pSoldier->bTeam == 1 && is_server))
						{
							send_grenade( pSoldier->pTempObject , pSoldier->pThrowParams->dLifeSpan,	pSoldier->pThrowParams->dX, pSoldier->pThrowParams->dY, pSoldier->pThrowParams->dZ, pSoldier->pThrowParams->dForceX, pSoldier->pThrowParams->dForceY, pSoldier->pThrowParams->dForceZ, pSoldier->sTargetGridNo, pSoldier->ubID, pSoldier->pThrowParams->ubActionCode, pSoldier->pThrowParams->uiActionData, iRealObjectID , true);
						}
					}

					// Remove object
					//RemoveObjFrom( &(pSoldier->inv[ HANDPOS ] ), 0 );

					// Update UI
					DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );

					OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );

					MemFree( pSoldier->pThrowParams );
					pSoldier->pThrowParams = NULL;
				}
				break;

			case 462:

				// CODE: MOVE UP FROM CLIFF CLIMB
				pSoldier->aiData.dHeightAdjustment += (float)2.1;
				pSoldier->sHeightAdjustment = (INT16)pSoldier->aiData.dHeightAdjustment;
				// Move over some...
				//MoveMercFacingDirection( pSoldier , FALSE, (FLOAT)0.5 );
				break;

			case 463:

				// MOVE GUY FORWARD SOME VALUE
				// Creature move
				MoveMercFacingDirection( pSoldier, FALSE, (FLOAT)1.5 );
				break;

			case 464:

				// CODE: END CLIFF CLIMB
				pSoldier->aiData.dHeightAdjustment = (float)0;
				pSoldier->sHeightAdjustment = (INT16)pSoldier->aiData.dHeightAdjustment;

				// Set new gridno
				{
					INT32 sTempGridNo;
					INT16 sNewX, sNewY;

					//Get Next GridNo;
					sTempGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc(pSoldier->ubDirection ) );

					// Get center XY
					ConvertGridNoToCenterCellXY( sTempGridNo, &sNewX, &sNewY );

					// Set position
					pSoldier->EVENT_SetSoldierPosition( sNewX, sNewY );

					// Move two CC directions
					pSoldier->EVENT_SetSoldierDirection( gTwoCCDirection[ pSoldier->ubDirection ] );
					pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

					// Set desired anim height!
					pSoldier->ubDesiredHeight = ANIM_CROUCH;
					pSoldier->pathing.sFinalDestination = pSoldier->sGridNo;

				}
				break;

			case 465:

				// CODE: SET GUY TO LIFE OF 0
				pSoldier->stats.bLife = 0;
				break;

			case 466:

				// CODE: ADJUST TO OUR DEST HEIGHT
				if ( pSoldier->sHeightAdjustment != pSoldier->sDesiredHeight )
				{
					INT16 sDiff = pSoldier->sHeightAdjustment - pSoldier->sDesiredHeight;

					if ( abs( sDiff ) > 4 )
					{
						if ( sDiff > 0 )
						{
							// Adjust!
							pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment - 2 ) );
						}
						else
						{
							// Adjust!
							pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->aiData.dHeightAdjustment + 2 ) );
						}
					}
					else
					{
						// Adjust!
						pSoldier->SetSoldierHeight( (FLOAT)(pSoldier->sDesiredHeight) );
					}
				}
				else
				{
					// Goto eating animation
					if ( pSoldier->sDesiredHeight == 0 )
					{
						pSoldier->ChangeSoldierState( CROW_EAT, 0 , FALSE );
					}
					else
					{
						// We should leave now!
						TacticalRemoveSoldier( pSoldier->ubID );
						return( FALSE );
					}
					return( TRUE );
				}
				break;

			case 467:

				///CODE: FOR HELIDROP, SET DIRECTION
				pSoldier->EVENT_SetSoldierDirection( EAST );
				pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

				gfIngagedInDrop = FALSE;

				// OK, now get a sweetspot ( not the place we are now! )
				//	sNewGridNo =	FindGridNoFromSweetSpotExcludingSweetSpot( pSoldier, pSoldier->sGridNo, 5, &ubNewDirection );
				//sNewGridNo =	FindRandomGridNoFromSweetSpotExcludingSweetSpot( pSoldier, pSoldier->sGridNo, 3, &ubNewDirection );

				sNewGridNo =	FindGridNoFromSweetSpotExcludingSweetSpotInQuardent( pSoldier, pSoldier->sGridNo, 3, &ubNewDirection, SOUTHEAST );

				// Check for merc arrives quotes...
				HandleMercArrivesQuotes( pSoldier );

				// Find a path to it!
				pSoldier->EVENT_GetNewSoldierPath( sNewGridNo, WALKING );

				return( TRUE );
				break;

			case 468:

				// CODE: End PUNCH
				{
					BOOLEAN fNPCPunch = FALSE;

					// ATE: Put some code in for NPC punches...
					if ( pSoldier->flags.uiStatusFlags & SOLDIER_NPC_DOING_PUNCH )
					{
						fNPCPunch = TRUE;

						// Turn off
						pSoldier->flags.uiStatusFlags &= (~SOLDIER_NPC_DOING_PUNCH );

						// Trigger approach...
						TriggerNPCWithGivenApproach( pSoldier->ubProfile, (UINT8)pSoldier->aiData.uiPendingActionData4, FALSE );
					}


					// Are we a martial artist?
					{
						BOOLEAN fMartialArtist = FALSE;

						if ( pSoldier->ubProfile != NO_PROFILE && pSoldier->ubBodyType == REGMALE ) // SANDRO - added check for body type
						{
							// SANDRO - old/new traits
							if (gGameOptions.fNewTraitSystem)
							{
								if ( NUM_SKILL_TRAITS( pSoldier, MARTIAL_ARTS_NT ) >= ((gSkillTraitValues.fPermitExtraAnimationsOnlyToMA) ? 2 : 1 ) )
								{
									fMartialArtist = TRUE;
								}
							}
							else
							{
								if ( ProfileHasSkillTrait( pSoldier->ubProfile, MARTIALARTS_OT ) > 0 )
								{
									fMartialArtist = TRUE;
								}
							}
						}

						if ( gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_CROUCH )
						{
							if ( fNPCPunch )
							{
								pSoldier->ChangeSoldierStance( ANIM_STAND );
								return( TRUE );
							}
							else
							{
								pSoldier->ChangeSoldierState( CROUCHING, 0, FALSE );
								return( TRUE );
							}
						}
						else
						{
#ifdef JA2UB
					//Ja25 No meanwhiles		
					          	if ( fMartialArtist )
#else
							if ( fMartialArtist && !AreInMeanwhile( ) )

#endif
							{
								pSoldier->ChangeSoldierState( NINJA_BREATH, 0, FALSE );
								return( TRUE );
							}
							else
							{
								pSoldier->ChangeSoldierState( PUNCH_BREATH, 0, FALSE );
								return( TRUE );
							}
						}
					}
				}
				break;

			case 469:

				// CODE: Begin martial artist attack
				pSoldier->DoNinjaAttack( );
				return( TRUE );
				break;

			case 470:

				// CODE: CHECK FOR OK WEAPON SHOT!
				bOKFireWeapon =	OKFireWeapon( pSoldier );

				if ( bOKFireWeapon == FALSE )
				{
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Fire Weapon: Gun Cannot fire, code 470") );

					// OK, SKIP x # OF FRAMES
					// Skip 3 frames, ( a third ia added at the end of switch.. ) For a total of 4
					pSoldier->usAniCode += 4;

					// Reduce by a bullet...
					//						pSoldier->bBulletsLeft--;

					PlayJA2Sample( S_DRYFIRE1, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );

					// Free-up!
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - gun failed to fire") );
					FreeUpAttacker( );

				}
				else if ( bOKFireWeapon == 255 )
				{
					// Play intermediate animation...
					if ( HandleUnjamAnimation( pSoldier ) )
					{
						return( TRUE );
					}
				}
				break;

			case 471:

				// CODE: Turn pause move flag off
				pSoldier->flags.uiStatusFlags &= (~SOLDIER_PAUSEANIMOVE);
				break;

			case 472:

				{
					BOOLEAN fGoBackToAimAfterHit;

					// Save old flag, then reset. If we do nothing special here, at least go back
					// to aim if we were.
					fGoBackToAimAfterHit = pSoldier->flags.fGoBackToAimAfterHit;
					pSoldier->flags.fGoBackToAimAfterHit = FALSE;

					if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT ) )
					{
						switch( gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
						{
						case ANIM_STAND:

							// OK, we can do some cool things here - first is to make guy walkl back a bit...
							//	pSoldier->ChangeSoldierState( STANDING_HIT_BEGINCROUCHDOWN, 0, FALSE );
							//	return( TRUE );
							break;

						}
					}

					// CODE: HANDLE ANY RANDOM HIT VARIATIONS WE WISH TO DO.....
					if ( fGoBackToAimAfterHit )
					{
						if ( pSoldier->stats.bLife >= OKLIFE )
						{
							pSoldier->InternalSoldierReadyWeapon(pSoldier->ubDirection, FALSE );
						}
						return( TRUE );
					}
				}
				break;

			case 473:

				// CODE: CHECK IF WE HAVE JUST JAMMED / OUT OF AMMO, DONOT CONTINUE, BUT
				// GOTO STATIONARY ANIM
				if ( CheckForImproperFireGunEnd( pSoldier ) )
				{
					return( TRUE );
				}
				break;

			case 474:

				// CODE: GETUP FROM SLEEP
				pSoldier->ChangeSoldierStance( ANIM_STAND );
				return( TRUE );

			case 475:

				// CODE: END CLIMB DOWN ROOF
				pSoldier->ubDesiredHeight = ANIM_STAND;
				pSoldier->pathing.sFinalDestination = pSoldier->sGridNo;

				// re-enable sight
				gTacticalStatus.uiFlags &= (~DISALLOW_SIGHT);

				// ATE: Change interface level.....
				// CJC: only if we are a player merc
				if ( (pSoldier->bTeam == gbPlayerNum) && !gTacticalStatus.fAutoBandageMode)
				{
					if ( pSoldier->ubID == gusSelectedSoldier )
					{
						ChangeInterfaceLevel( 0 );
					}
					// OK, UNSET INTERFACE FIRST
					UnSetUIBusy( pSoldier->ubID );
				}
				else
				{
					FreeUpNPCFromRoofClimb( pSoldier );
				}
				pSoldier->usUIMovementMode = WALKING;

				// ATE: Handle sight...
				HandleSight( pSoldier,SIGHT_LOOK | SIGHT_RADIO | SIGHT_INTERRUPT );
				break;

			case 476:

				// CODE: GOTO PREVIOUS ANIMATION
				pSoldier->ChangeSoldierState( ( pSoldier->aiData.sPendingActionData2 ), (UINT8)( pSoldier->aiData.uiPendingActionData1 + 1 ), FALSE );
				return( TRUE );
				break;


			case 477:

				// CODE: Locate to target ( if an AI guy.. )
				if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) )
				{
					if ( pSoldier->bTeam != gbPlayerNum )
					{
						// only locate if the enemy is visible or he's aiming at a player
						if ( pSoldier->bVisible != -1 || (pSoldier->ubTargetID != NOBODY && MercPtrs[ pSoldier->ubTargetID ]->bTeam == gbPlayerNum) )
						{
							LocateGridNo( pSoldier->sTargetGridNo );
						}
					}
				}
				break;

			case 478:

				// CODE: Decide to turn from hit.......
				{
					INT8		bNewDirection;
					UINT32	uiChance;

					// ONLY DO THIS IF CERTAIN CONDITIONS ARISE!
					// For one, only do for mercs!
					if ( pSoldier->ubBodyType <= REGFEMALE )
					{
						// Secondly, don't if we are going to collapse
						if ( pSoldier->stats.bLife >= OKLIFE && pSoldier->bBreath > 0 && pSoldier->pathing.bLevel == 0 )
						{
							// Save old direction
							pSoldier->aiData.uiPendingActionData1 = pSoldier->ubDirection;

							// If we got a head shot...more chance of turning...
							if ( pSoldier->ubHitLocation != AIM_SHOT_HEAD )
							{
								uiChance = Random( 100 );

								// 30 % chance to change direction one way
								if ( uiChance	< 30 )
								{
									bNewDirection = gOneCDirection[ pSoldier->ubDirection ];
								}
								// 30 % chance to change direction the other way
								else if ( uiChance >= 30 && uiChance < 60 )
								{
									bNewDirection = gOneCCDirection[ pSoldier->ubDirection ];
								}
								// 30 % normal....
								else
								{
									bNewDirection = pSoldier->ubDirection;
								}

								pSoldier->EVENT_SetSoldierDirection( bNewDirection );
								pSoldier->EVENT_SetSoldierDesiredDirection( pSoldier->ubDirection );

							}
							else
							{
								// OK, 50% chance here to turn...
								uiChance = Random( 100 );

								if ( uiChance < 50 )
								{
									// OK, pick a larger direction to goto....
									pSoldier->flags.uiStatusFlags |= SOLDIER_TURNINGFROMHIT;
									// This becomes an attack busy situation
									// 0verhaul:  There is an attack busy problem with this.  The soldier could be in mid-turn
									// when another bullet is fired (auto-fire or dual-wield, for instance), and the soldier is
									// knocked down in the middle of the turn.  In such a case, the attack busy does not get
									// cancelled.  So if we indeed need to keep the attack busy (which may not be the case),
									// we will need to find a more reliable method.  For now, I'm going to cancel out the
									// ABC adjustment here and we'll see if there needs to be something in its place.
									//gTacticalStatus.ubAttackBusyCount++;
									DebugAttackBusy( String( "Soldier turning from a hit.  Not Increasing attack busy.  Now %d\n", gTacticalStatus.ubAttackBusyCount ) );

									// Pick evenly between both
									if ( Random( 50 ) < 25 )
									{
										bNewDirection = gOneCDirection[ pSoldier->ubDirection ];
										bNewDirection = gOneCDirection[ bNewDirection ];
										bNewDirection = gOneCDirection[ bNewDirection ];
									}
									else
									{
										bNewDirection = gOneCCDirection[ pSoldier->ubDirection ];
										bNewDirection = gOneCCDirection[ bNewDirection ];
										bNewDirection = gOneCCDirection[ bNewDirection ];
									}

									pSoldier->EVENT_SetSoldierDesiredDirection( bNewDirection );
								}
							}
						}
					}
					break;
				}

			case 479:

				// CODE: Return to old direction......
				if ( pSoldier->ubBodyType <= REGFEMALE )
				{
					// Secondly, don't if we are going to collapse
					//if ( pSoldier->stats.bLife >= OKLIFE && pSoldier->bBreath > 0 )
					//{
					///	if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_TURNINGFROMHIT ) )
					//	{
					///		pSoldier->ubDirection				= (INT8)pSoldier->aiData.uiPendingActionData1;
					//		pSoldier->pathing.bDesiredDirection = (INT8)pSoldier->aiData.uiPendingActionData1;
					//	}
					//}
				}
				break;

			case 480:

				// 0verhaul:	This is handled in the ReduceAttackBusyCount call
				// CODE: FORCE FREE ATTACKER
				// Release attacker
				//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, code 480") );

				//ReleaseSoldiersAttacker( pSoldier );

				//FREEUP GETTING HIT FLAG
				//pSoldier->flags.fGettingHit = FALSE;
				break;

			case 481:

				// CODE: CUT FENCE...
				CutWireFence( pSoldier->sTargetGridNo );
				break;

			case 482:

				// CODE: END CRIPPLE KICKOUT...
				KickOutWheelchair( pSoldier );
				break;

			case 483:

				// CODE: HANDLE DROP BOMB...
				HandleSoldierDropBomb( pSoldier, pSoldier->aiData.sPendingActionData2 );
				break;

			case 484:

				// CODE: HANDLE REMOTE...
				HandleSoldierUseRemote( pSoldier, pSoldier->aiData.sPendingActionData2 );
				break;

			case 485:

				// CODE: Try steal.....
//				UnSetUIBusy( pSoldier->ubID);
				UseHandToHand( pSoldier, pSoldier->aiData.sPendingActionData2, TRUE );
				//jackaians:
				//if we are not waiting for the pickup menu to be displayed
//				if (guiPendingOverrideEvent != G_GETTINGITEM)
//				{
//					PreventFromTheFreezingBug(pSoldier);
//				}
				break;

			case 486:

				// CODE: GIVE ITEM
				SoldierGiveItemFromAnimation( pSoldier );
			//	if (pSoldier->ubProfile != NO_PROFILE && pSoldier->ubProfile >= FIRST_NPC )
				//new profiles by Jazz	
				if (pSoldier->ubProfile != NO_PROFILE && (gProfilesNPC[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile ||
					gProfilesRPC[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile ||
					gProfilesVehicle[pSoldier->ubProfile].ProfilId == pSoldier->ubProfile))
				{
					TriggerNPCWithGivenApproach( pSoldier->ubProfile, APPROACH_DONE_GIVING_ITEM, FALSE );
				}
				break;

			case 487:

				// CODE: DROP ITEM
				SoldierHandleDropItem( pSoldier );
				break;

			case 489:


				//CODE: REMOVE GUY FRMO WORLD DUE TO EXPLOSION
				//pSoldier->ChangeSoldierState( RAISE_RIFLE, 0 , FALSE );
				//return( TRUE );
				//Delete guy
				//TacticalRemoveSoldier( pSoldier->ubID );
				//return( FALSE );
				break;

			case 490:

				// CODE: HANDLE END ITEM PICKUP
				//LOOK INTO HAND, RAISE RIFLE IF WE HAVE ONE....
				/*
				if ( pSoldier->inv[ HANDPOS ].exists() == true )
				{
				// CHECK IF GUN
				if ( Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass == IC_GUN )
				{
				if ( Weapon[ pSoldier->inv[ HANDPOS ].usItem ].ubWeaponClass != HANDGUNCLASS )
				{
				// RAISE
				pSoldier->ChangeSoldierState( RAISE_RIFLE, 0 , FALSE );
				return( TRUE );
				}

				}

				}
				*/
				break;

			case 491:

				// CODE: HANDLE RANDOM BREATH ANIMATION
				//if ( pSoldier->stats.bLife > INJURED_CHANGE_THREASHOLD )
				if ( pSoldier->stats.bLife >= OKLIFE )
				{
					// Increment time from last update
					pSoldier->uiTimeOfLastRandomAction++;

					if ( pSoldier->uiTimeOfLastRandomAction > TIME_FOR_RANDOM_ANIM_CHECK || pSoldier->stats.bLife < INJURED_CHANGE_THREASHOLD || GetDrunkLevel( pSoldier ) >= BORDERLINE )
					{
						pSoldier->uiTimeOfLastRandomAction = 0;

						// Don't do any in water!
						if ( !pSoldier->MercInWater( ) )
						{
							// OK, make a dice roll
							ubDiceRoll = (UINT8)Random( 100 );

							// Determine what is in our hand;
							usItem = pSoldier->inv[ HANDPOS ].usItem;

							// Default to nothing in hand ( nothing in quotes, we do have something but not just visible )
							ubRandomHandIndex = RANDOM_ANIM_NOTHINGINHAND;

							if ( pSoldier->inv[ HANDPOS ].exists() == true )
							{
								if ( Item[ usItem ].usItemClass == IC_GUN )
								{
									//										if ( (Item[ usItem ].fFlags & ITEM_TWO_HANDED) )
									if ( (Item[ usItem ].twohanded ) )
									{
										// Set to rifle
										ubRandomHandIndex = RANDOM_ANIM_RIFLEINHAND;
									}
									else
									{
										// Don't EVER do a trivial anim...
										break;
									}
								}
							}

							// Check which animation to play....
							for ( cnt = 0; cnt < MAX_RANDOM_ANIMS_PER_BODYTYPE; cnt++ )
							{
								pAnimDef = &( gRandomAnimDefs[ pSoldier->ubBodyType ][ cnt ] );

								if ( pAnimDef->sAnimID	!= 0 )
								{
									// If it's an injured animation and we are not in the threashold....
									if ( ( pAnimDef->ubFlags & RANDOM_ANIM_INJURED ) && pSoldier->stats.bLife >= INJURED_CHANGE_THREASHOLD )
									{
										continue;
									}

									// If we need to do an injured one, don't do any others...
									if ( !( pAnimDef->ubFlags & RANDOM_ANIM_INJURED ) && pSoldier->stats.bLife < INJURED_CHANGE_THREASHOLD )
									{
										continue;
									}

									// If it's a drunk animation and we are not in the threashold....
									if ( ( pAnimDef->ubFlags & RANDOM_ANIM_DRUNK ) && GetDrunkLevel( pSoldier ) < BORDERLINE )
									{
										continue;
									}

									// If we need to do an injured one, don't do any others...
									if ( !( pAnimDef->ubFlags & RANDOM_ANIM_DRUNK ) && GetDrunkLevel( pSoldier ) >= BORDERLINE )
									{
										continue;
									}

									// Check if it's our hand
									if ( pAnimDef->ubHandRestriction != RANDOM_ANIM_IRRELEVENTINHAND && pAnimDef->ubHandRestriction != ubRandomHandIndex )
									{
										continue;
									}

									// Check if it's casual and we're in combat and it's not our guy
									if ( ( pAnimDef->ubFlags & RANDOM_ANIM_CASUAL ) )
									{
										// If he's a bad guy, do not do it!
										if ( pSoldier->bTeam != gbPlayerNum	|| ( gTacticalStatus.uiFlags & INCOMBAT ) )
										{
											continue;
										}
									}

									// If we are an alternate big guy and have been told to use a normal big merc ani...
									if ( ( pAnimDef->ubFlags & RANDOM_ANIM_FIRSTBIGMERC ) && ( pSoldier->uiAnimSubFlags & SUB_ANIM_BIGGUYTHREATENSTANCE ) )
									{
										continue;
									}

									// If we are a normal big guy and have been told to use an alternate big merc ani...
									if ( ( pAnimDef->ubFlags & RANDOM_ANIM_SECONDBIGMERC ) && !( pSoldier->uiAnimSubFlags & SUB_ANIM_BIGGUYTHREATENSTANCE ) )
									{
										continue;
									}

									// Check if it's the proper height
									if ( pAnimDef->ubAnimHeight == gAnimControl[ pSoldier->usAnimState ].ubEndHeight )
									{
										// OK, If we rolled a value that lies within the range for this random animation, use this one!
										if ( ubDiceRoll >= pAnimDef->ubStartRoll && ubDiceRoll <= pAnimDef->ubEndRoll )
										{
											// Are we playing a sound
											if ( pAnimDef->sAnimID == RANDOM_ANIM_SOUND )
											{
												if ( pSoldier->ubBodyType == COW )
												{
													if ( Random( 2 ) == 1 )
													{
														if ( ( gTacticalStatus.uiFlags & INCOMBAT ) && pSoldier->bVisible == -1 )
														{
															// DO this every 10th time or so...
															if ( Random( 100 ) < 10 )
															{
																// Play sound
																PlayJA2SampleFromFile(	pAnimDef->zSoundFile, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
															}
														}
														else
														{

															// Play sound
															PlayJA2SampleFromFile( pAnimDef->zSoundFile, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
														}
													}
												}
												else
												{
													// Play sound
													PlayJA2SampleFromFile( pAnimDef->zSoundFile, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
												}
											}
											else
											{
												pSoldier->ChangeSoldierState( pAnimDef->sAnimID, 0 , FALSE );
											}
											return( TRUE );
										}
									}
								}
							}

						}
					}
				}
				break;

			case 492:


				// SIGNAL DODGE!
				// ATE: Only do if we're not inspecial case...
				if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_NPC_DOING_PUNCH ) )
				{
					SOLDIERTYPE *pTSoldier;
					UINT32 uiMercFlags;
					UINT16 usSoldierIndex;

					if ( FindSoldier( pSoldier->sTargetGridNo, &usSoldierIndex, &uiMercFlags, FIND_SOLDIER_GRIDNO ) )
					{
						GetSoldier( &pTSoldier, usSoldierIndex );

						// IF WE ARE AN ANIMAL, CAR, MONSTER, DONT'T DODGE
						if ( IS_MERC_BODY_TYPE( pTSoldier ) )
						{
							// ONLY DODGE IF WE ARE SEEN
							if ( pTSoldier->aiData.bOppList[ pSoldier->ubID ] != 0 || pTSoldier->bTeam == pSoldier->bTeam )
							{
								if ( gAnimControl[ pTSoldier->usAnimState ].ubHeight == ANIM_STAND )
								{
									// OK, stop merc....
									pTSoldier->EVENT_StopMerc( pTSoldier->sGridNo, pTSoldier->ubDirection );

									if ( pTSoldier->bTeam != gbPlayerNum )
									{
										CancelAIAction( pTSoldier, TRUE );
									}

									// Turn towards the person!
									pTSoldier->EVENT_SetSoldierDesiredDirection( GetDirectionFromGridNo( pSoldier->sGridNo, pTSoldier ) );

									// PLAY SOLDIER'S DODGE ANIMATION
									pTSoldier->ChangeSoldierState( DODGE_ONE, 0 , FALSE );

									// SANDRO - after dodging melee attack go to apropriate stance
									if ( (gAnimControl[ pTSoldier->usAnimState ].ubHeight == ANIM_STAND) && (Item[pTSoldier->inv[HANDPOS].usItem].usItemClass == IC_PUNCH))
									{
										if ((((NUM_SKILL_TRAITS( pTSoldier, MARTIAL_ARTS_NT ) >= ((gSkillTraitValues.fPermitExtraAnimationsOnlyToMA) ? 2 : 1 )) && gGameOptions.fNewTraitSystem ) ||
											(HAS_SKILL_TRAIT( pTSoldier, MARTIALARTS_OT ) && !gGameOptions.fNewTraitSystem ) ) &&
											 pTSoldier->ubBodyType == REGMALE )
										{
											//pTSoldier->usPendingAnimation = NINJA_GOTOBREATH;
											pTSoldier->usPendingAnimation = NINJA_BREATH ;
										}
										else
										{
											pTSoldier->usPendingAnimation = PUNCH_BREATH ;
										}
									}
									else if ( (gAnimControl[ pTSoldier->usAnimState ].ubHeight == ANIM_STAND) && (Item[pTSoldier->inv[HANDPOS].usItem].usItemClass == IC_BLADE))
									{
										//pTSoldier->usPendingAnimation = KNIFE_GOTOBREATH;
										pTSoldier->usPendingAnimation = KNIFE_BREATH ;
									}
								}
							}
						}
					}
				}
				break;

			case 493:

				//CODE: PICKUP ITEM!
				// CHECK IF THIS EVENT HAS BEEN SETUP
				//if ( pSoldier->aiData.ubPendingAction == MERC_PICKUPITEM )
				//{
				// DROP ITEM
				HandleSoldierPickupItem( pSoldier, pSoldier->aiData.uiPendingActionData1, pSoldier->aiData.uiPendingActionData4, pSoldier->aiData.bPendingActionData3 );
				// EVENT HAS BEEN HANDLED
				pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

				//}
				//else
				//{
				//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Soldier Ani: CODE 493 Error, Pickup item action called but not setup" );
				//}
				break;

			case 494:

				//CODE: OPEN STRUCT!
				// CHECK IF THIS EVENT HAS BEEN SETUP
				//if ( pSoldier->aiData.ubPendingAction == MERC_OPENSTRUCT )
				//{
				SoldierHandleInteractiveObject( pSoldier );

				// EVENT HAS BEEN HANDLED
				pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

				//}
				//else
				//{
				//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Soldier Ani: CODE 494 Error, OPen door action called but not setup" );
				//}
				break;

			case 495:

				if (pSoldier->aiData.bAction == AI_ACTION_UNLOCK_DOOR || (pSoldier->aiData.bAction == AI_ACTION_LOCK_DOOR && !(pSoldier->aiData.fAIFlags & AI_LOCK_DOOR_INCLUDES_CLOSE) ) )
				{
					// EVENT HAS BEEN HANDLED
					pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

					// do nothing here
				}
				else
				{

					pSoldier->aiData.fAIFlags &= ~(AI_LOCK_DOOR_INCLUDES_CLOSE);

					pSoldier->ubDoorOpeningNoise = DoorOpeningNoise( pSoldier );

					if ( SoldierHandleInteractiveObject( pSoldier ) )
					{
						// HANDLE SIGHT!
						//HandleSight(pSoldier,SIGHT_LOOK | SIGHT_RADIO | SIGHT_INTERRUPT );

						InitOpplistForDoorOpening();

						MakeNoise( pSoldier->ubID, pSoldier->sGridNo, pSoldier->pathing.bLevel, gpWorldLevelData[pSoldier->sGridNo].ubTerrainID, pSoldier->ubDoorOpeningNoise, NOISE_CREAKING );
						//	gfDelayResolvingBestSighting = FALSE;

						gubInterruptProvoker = pSoldier->ubID;
						AllTeamsLookForAll( TRUE );

						// ATE: Now, check AI guy to cancel what he was going....
						HandleSystemNewAISituation( pSoldier, TRUE );
					}

					// EVENT HAS BEEN HANDLED
					pSoldier->aiData.ubPendingAction		= NO_PENDING_ACTION;

				}


				break;

			case 496:
				// CODE: GOTO PREVIOUS ANIMATION
				pSoldier->ChangeSoldierState( pSoldier->usOldAniState, pSoldier->sOldAniCode, FALSE );
				return( TRUE );

			case 497:

				// CODE: CHECK FOR UNCONSCIOUS OR DEATH
				// IF 496 - GOTO PREVIOUS ANIMATION, OTHERWISE PAUSE ANIMATION
				if ( pSoldier->stats.bLife == 0 )
				{

					//HandleSoldierDeath( pSoldier );

					// If guy is now dead, and we have not played death sound before, play
					if ( pSoldier->stats.bLife == 0 && !pSoldier->flags.fDeadSoundPlayed	)
					{
						if ( pSoldier->usAnimState != JFK_HITDEATH )
						{
							pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
							pSoldier->flags.fDeadSoundPlayed = TRUE;
						}
					}

					if ( gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
					{
						// If we are dead, play some death animations!!
						switch( pSoldier->usAnimState )
						{
						case FLYBACK_HIT:
							pSoldier->ChangeSoldierState( FLYBACK_HIT_DEATH, 0, FALSE );
							break;

						case GENERIC_HIT_DEATHTWITCHNB:
						case FALLFORWARD_FROMHIT_STAND:
						case ENDFALLFORWARD_FROMHIT_CROUCH:

							pSoldier->ChangeSoldierState( GENERIC_HIT_DEATH, 0, FALSE );
							break;

						case FALLBACK_HIT_DEATHTWITCHNB:
						case FALLBACK_HIT_STAND:
							pSoldier->ChangeSoldierState( FALLBACK_HIT_DEATH, 0, FALSE );
							break;

						case PRONE_HIT_DEATHTWITCHNB:
						case PRONE_LAY_FROMHIT:

							pSoldier->ChangeSoldierState( PRONE_HIT_DEATH, 0, FALSE );
							break;

						case FALLOFF:
							pSoldier->ChangeSoldierState( FALLOFF_DEATH, 0, FALSE );
							break;

						case FALLFORWARD_ROOF:
							pSoldier->ChangeSoldierState( FALLOFF_FORWARD_DEATH, 0, FALSE);
							break;

						case ADULTMONSTER_DYING:
							pSoldier->ChangeSoldierState( ADULTMONSTER_DYING_STOP, 0, FALSE);
							break;

						case LARVAE_DIE:
							pSoldier->ChangeSoldierState( LARVAE_DIE_STOP, 0, FALSE);
							break;

						case QUEEN_DIE:
							pSoldier->ChangeSoldierState( QUEEN_DIE_STOP, 0, FALSE);
							break;

						case INFANT_DIE:
							pSoldier->ChangeSoldierState( INFANT_DIE_STOP, 0, FALSE);
							break;

						case CRIPPLE_DIE:
							pSoldier->ChangeSoldierState( CRIPPLE_DIE_STOP, 0, FALSE);
							break;

						case ROBOTNW_DIE:
							pSoldier->ChangeSoldierState( ROBOTNW_DIE_STOP, 0, FALSE);
							break;

						case CRIPPLE_DIE_FLYBACK:
							pSoldier->ChangeSoldierState( CRIPPLE_DIE_FLYBACK_STOP, 0, FALSE);
							break;

						default:
							// IF we are here - something is wrong - we should have a death animation here
							DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Ani: Death sequence needed for animation %d", pSoldier->usAnimState ) );

						}
					}
					else
					{
						BOOLEAN fMadeCorpse;

						CheckForAndHandleSoldierDeath( pSoldier, &fMadeCorpse );

						// ATE: Needs to be FALSE!
						return( FALSE );
					}
					return( TRUE );

				}
				else
				{
					// We can safely be here as well.. ( ie - next turn we may be able to get up )
					// DO SOME CHECKS HERE TO FREE UP ATTACKERS IF WE ARE WAITING AT SPECIFIC ANIMATIONS
					if ( ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_HITFINISH ) )
					{
						gfPotentialTeamChangeDuringDeath = TRUE;

						// 0verhaul: This is now already handled
						// Release attacker
						// DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, code 497 = check for death") );
						// ReleaseSoldiersAttacker( pSoldier );

						// ATE: OK - the above call can potentially
						// render the soldier bactive to false - check heare
						if ( !pSoldier->bActive )
						{
							return( FALSE );
						}

						gfPotentialTeamChangeDuringDeath = FALSE;

						// FREEUP GETTING HIT FLAG
						pSoldier->flags.fGettingHit = FALSE;
					}

					HandleCheckForDeathCommonCode( pSoldier );

					return( TRUE );
				}
				break;

			case 498:

				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - if pending interrupt flag was set for before-attack type of interupt, try to resolve it now
				if ( gGameOptions.fImprovedInterruptSystem )
				{
					if ( ResolvePendingInterrupt( pSoldier, BEFORESHOT_INTERRUPT ) )
					{	
						if ( pSoldier->flags.fTurningToShoot )
							pSoldier->flags.fTurningToShoot = FALSE;

						pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
						pSoldier->ubPendingDirection = NO_PENDING_DIRECTION;
						// "artificially" set lock ui flag in this case
						if (pSoldier->bTeam == gbPlayerNum)
						{
							AddTopMessage( COMPUTER_INTERRUPT_MESSAGE, Message[STR_INTERRUPT] );
							guiPendingOverrideEvent = LU_BEGINUILOCK;								
							HandleTacticalUI( );
						}
						return( TRUE );				
						break;
					}
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// CONDITONAL JUMP
				// If we have a pending animation, play it, else continue
				if ( pSoldier->usPendingAnimation != NO_PENDING_ANIMATION )
				{
					pSoldier->ChangeSoldierState( pSoldier->usPendingAnimation, 0, FALSE );
					pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
					return( TRUE );
				}
				break;

				// JUMP TO NEXT STATIONARY ANIMATION ACCORDING TO HEIGHT
			case 499:

				if (!(pSoldier->flags.uiStatusFlags & SOLDIER_PC))
				{
					if ( pSoldier->aiData.bAction == AI_ACTION_PULL_TRIGGER )
					{
						if ( pSoldier->usAnimState == AI_PULL_SWITCH && gTacticalStatus.ubAttackBusyCount == 0 && gubElementsOnExplosionQueue == 0 )
						{
							FreeUpNPCFromPendingAction( pSoldier );
						}
					}
					else if ( pSoldier->aiData.bAction == AI_ACTION_PENDING_ACTION
						|| pSoldier->aiData.bAction == AI_ACTION_OPEN_OR_CLOSE_DOOR
						|| pSoldier->aiData.bAction == AI_ACTION_YELLOW_ALERT
						|| pSoldier->aiData.bAction == AI_ACTION_RED_ALERT
						|| pSoldier->aiData.bAction == AI_ACTION_PULL_TRIGGER
						|| pSoldier->aiData.bAction == AI_ACTION_CREATURE_CALL
						|| pSoldier->aiData.bAction == AI_ACTION_UNLOCK_DOOR
						|| pSoldier->aiData.bAction == AI_ACTION_LOCK_DOOR	)
					{
						if ( pSoldier->usAnimState == PICKUP_ITEM || pSoldier->usAnimState == ADJACENT_GET_ITEM || pSoldier->usAnimState == ADJACENT_GET_ITEM_CROUCHED || pSoldier->usAnimState == DROP_ITEM || pSoldier->usAnimState == END_OPEN_DOOR || pSoldier->usAnimState == END_OPEN_DOOR_CROUCHED || pSoldier->usAnimState == CLOSE_DOOR || pSoldier->usAnimState == MONSTER_UP || pSoldier->usAnimState == AI_RADIO || pSoldier->usAnimState == AI_CR_RADIO || pSoldier->usAnimState == END_OPENSTRUCT || pSoldier->usAnimState == END_OPENSTRUCT_CROUCHED || pSoldier->usAnimState == QUEEN_CALL )
						{
							FreeUpNPCFromPendingAction( pSoldier );
						}
					}
				}

				ubDesiredHeight = pSoldier->ubDesiredHeight;

				// Check if we are at the desired height
				if ( pSoldier->ubDesiredHeight == gAnimControl[ pSoldier->usAnimState ].ubEndHeight || pSoldier->ubDesiredHeight == NO_DESIRED_HEIGHT )
				{
					// Adjust movement mode......
					if ( pSoldier->bTeam == gbPlayerNum && !pSoldier->flags.fContinueMoveAfterStanceChange )
					{
						usUIMovementMode =	pSoldier->GetMoveStateBasedOnStance( gAnimControl[ pSoldier->usAnimState ].ubEndHeight );

						// ATE: if we are currently running but have been told to walk, don't!
						if ( pSoldier->usUIMovementMode == RUNNING && usUIMovementMode == WALKING )
						{
							// No!
						}
						else
						{
							pSoldier->usUIMovementMode = usUIMovementMode;
						}
					}


					pSoldier->ubDesiredHeight = NO_DESIRED_HEIGHT;

					// 0verhaul:	This is moved to the animation state transition code to make sure it isn't sidestepped.
					// if (pSoldier->flags.fChangingStanceDueToSuppression)
					// {
					//	pSoldier->flags.fChangingStanceDueToSuppression = FALSE;
					//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Freeing up attacker - end of suppression stance change") );
					//	ReduceAttackBusyCount( pSoldier->ubSuppressorID, FALSE );
					// }

					if ( pSoldier->usPendingAnimation == NO_PENDING_ANIMATION && 
						( pSoldier->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE ) && 
						( pSoldier->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ON ) )
					{
						if ( gTacticalStatus.ubAttackBusyCount == 0 )
						{
							// OK, UNSET INTERFACE FIRST
							UnSetUIBusy( pSoldier->ubID );
							// ( before we could get interrupted potentially by an interrupt )
						}
					}

					// Check to see if we have changed stance and need to update visibility
					if ( gAnimControl[ pSoldier->usAnimState ].uiFlags & ANIM_STANCECHANGEANIM)
					{
						if ( pSoldier->usPendingAnimation == NO_PENDING_ANIMATION && 
							gTacticalStatus.ubAttackBusyCount == 0 && 
							pSoldier->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ENDING_UP_FROM_MOVE && 
							pSoldier->flags.bTurningFromPronePosition != TURNING_FROM_PRONE_ON )
						{
							HandleSight(pSoldier,SIGHT_LOOK | SIGHT_RADIO | SIGHT_INTERRUPT );
						}
						else
						{
							HandleSight(pSoldier,SIGHT_LOOK | SIGHT_RADIO );
						}

						// Keep ui busy if we are now in a hidden interrupt
						// say we're prone and we crouch, we may get a hidden
						// interrupt and in such a case we'd really like the UI
						// still locked
						if ( gfHiddenInterrupt )
						{
							guiPendingOverrideEvent	= LA_BEGINUIOURTURNLOCK;
							HandleTacticalUI( );
						}

						// ATE: Now, check AI guy to cancel what he was going....
						HandleSystemNewAISituation( pSoldier, TRUE );
					}

					// Have we finished opening doors?
					// 0verhaul:  Added additional check:  Are we told to stop at this point, maybe due to being interrupted?
					if ( !pSoldier->flags.fNoAPToFinishMove &&
						(pSoldier->usAnimState == END_OPEN_DOOR || 
						pSoldier->usAnimState == END_OPEN_DOOR_CROUCHED || 
						pSoldier->usAnimState == CRIPPLE_CLOSE_DOOR || 
						pSoldier->usAnimState == CRIPPLE_END_OPEN_DOOR ) )
					{
						// Are we told to continue movement...?
						if ( pSoldier->bEndDoorOpenCode == 1 )
						{
							// OK, set this value to 2 such that once we are into a new gridno,
							// we close the door!
							pSoldier->bEndDoorOpenCode = 2;

							// yes..
							pSoldier->EVENT_GetNewSoldierPath( pSoldier->pathing.sFinalDestination, pSoldier->usUIMovementMode );

							if ( !( gAnimControl[ pSoldier->usAnimState ].uiFlags & ( ANIM_MOVING ) ) )
							{								
								if (!TileIsOutOfBounds(pSoldier->sAbsoluteFinalDestination))
								{
									CancelAIAction( pSoldier, FORCE );
								}
							}

							// OK, this code, pSoldier->bEndDoorOpenCode will be set to 0 if anythiing
							// cuases guy to stop - StopMerc() will set it...

							return( TRUE );
						}
					}

					// Check if we should contine into a moving animation
					if ( pSoldier->usPendingAnimation != NO_PENDING_ANIMATION )
					{
						UINT16 usPendingAnimation = pSoldier->usPendingAnimation;

						pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
						pSoldier->ChangeSoldierState( usPendingAnimation, 0, FALSE );
						return( TRUE );
					}

					// Alrighty, do we wish to continue
					if ( pSoldier->flags.fContinueMoveAfterStanceChange )
					{
						// OK, if the code is == 2, get the path and try to move....
						if ( pSoldier->flags.fContinueMoveAfterStanceChange == 2 )
						{
							pSoldier->pathing.usPathIndex++;

							if ( pSoldier->pathing.usPathIndex > pSoldier->pathing.usPathDataSize )
							{
								pSoldier->pathing.usPathIndex = pSoldier->pathing.usPathDataSize;
							}

							if ( pSoldier->pathing.usPathIndex == pSoldier->pathing.usPathDataSize )
							{
								// Stop, don't do anything.....
								// 0verhaul:	Only if not at the final destination
								// Another reason for rebuilding the animation system.	This should be part of a common
								// path continuation code so that any other bug fixes won't need to be duplicated in other areas.
								if ( pSoldier->sGridNo != pSoldier->pathing.sFinalDestination)
								{
									if ( !pSoldier->EVENT_InternalGetNewSoldierPath( pSoldier->pathing.sFinalDestination, pSoldier->usUIMovementMode, 2, FALSE ) )
									{
									}
								}
							}
							else
							{
								pSoldier->EVENT_InitNewSoldierAnim( pSoldier->usUIMovementMode, 0 , FALSE );

								// UNSET LOCK PENDING ACTION COUNTER FLAG
								pSoldier->flags.uiStatusFlags &= ( ~SOLDIER_LOCKPENDINGACTIONCOUNTER );

							}
						}
						else
						{
							SelectMoveAnimationFromStance( pSoldier );
						}

						pSoldier->flags.fContinueMoveAfterStanceChange = FALSE;
						return( TRUE );
					}
					pSoldier->SoldierGotoStationaryStance( );
					return( TRUE );
				}
				else
				{
					ubCurrentHeight = gAnimControl[ pSoldier->usAnimState ].ubEndHeight;

					// We need to go more, continue
					if ( ubDesiredHeight == ANIM_STAND && ubCurrentHeight == ANIM_CROUCH )
					{
						// Return here because if now, we will skipp a few frames
						pSoldier->ChangeSoldierState( KNEEL_UP, 0 , FALSE );
						return( TRUE );
					}
					if ( ubDesiredHeight == ANIM_CROUCH && ubCurrentHeight == ANIM_STAND )
					{
						// Return here because if now, we will skipp a few frames
						pSoldier->ChangeSoldierState( KNEEL_DOWN, 0 , FALSE );
						return( TRUE );
					}
					else if ( ubDesiredHeight == ANIM_PRONE && ubCurrentHeight == ANIM_CROUCH )
					{
						// Return here because if now, we will skipp a few frames
						pSoldier->ChangeSoldierState( PRONE_DOWN, 0 , FALSE );
						return( TRUE );
					}
					else if ( ubDesiredHeight == ANIM_CROUCH && ubCurrentHeight == ANIM_PRONE )
					{
						// Return here because if now, we will skipp a few frames
						pSoldier->ChangeSoldierState( PRONE_UP, 0 , FALSE );
						return( TRUE );
					}
				}
				// IF we are here - something is wrong - we should have a death animation here
#ifdef JA2BETAVERSION
				ScreenMsg( FONT_ORANGE, MSG_BETAVERSION, L"Soldier Ani: GOTO Stance not chained properly: %d %d %d", ubDesiredHeight, ubCurrentHeight, pSoldier->usAnimState );
#endif

				pSoldier->SoldierGotoStationaryStance( );
				return( TRUE );
			}

			// Adjust frame control pos, and try again
			pSoldier->usAniCode++;

		}
		else if ( sNewAniFrame > 499 && sNewAniFrame < 599 )
		{
			// Jump,
			// Do not adjust, just try again
			pSoldier->usAniCode = sNewAniFrame - 501;
		}
		else if ( sNewAniFrame > 599 && sNewAniFrame <= 699 )
		{
			// Jump, to animation script
			pSoldier->EVENT_InitNewSoldierAnim( (UINT16)(sNewAniFrame - 600 ), 0 , FALSE );
			return( TRUE );
		}
		else if ( sNewAniFrame > 799 && sNewAniFrame <= 899 )
		{
			// Jump, to animation script ( But in the 100's range )
			pSoldier->EVENT_InitNewSoldierAnim( (UINT16)(sNewAniFrame - 700 ), 0 , FALSE );
			return( TRUE );
		}
		else if ( sNewAniFrame > 899 && sNewAniFrame <= 999 )
		{
			// Jump, to animation script ( But in the 200's range )
			pSoldier->EVENT_InitNewSoldierAnim( (UINT16)(sNewAniFrame - 700 ), 0 , FALSE );
			return( TRUE );
		}
		else if ( sNewAniFrame > 699 && sNewAniFrame < 799 )
		{
			switch( sNewAniFrame )
			{
			case 702:
				// Play fall to knees sound
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( FALL_1 + Random(2) ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				break;

			case 703:
			case 704:

				// Play footprints
				PlaySoldierFootstepSound( pSoldier );
				break;

			case 705:
				// PLay body splat sound
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)BODY_SPLAT_1, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 706:
				// PLay head splat
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)HEADSPLAT_1, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) , TRUE );
				break;

			case 707:
				// PLay creature battle cry
				PlayJA2StreamingSample( (UINT8)CREATURE_BATTLECRY_1, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				break;

			case 708:

				// PLay lock n' load sound for gun....
				// Get LNL sound for current gun
				{
					UINT16	usItem;
					UINT16	usSoundID;

					usItem = pSoldier->inv[ HANDPOS ].usItem;

					OBJECTTYPE* pObjUsed =  pSoldier->GetUsedWeapon( &pSoldier->inv[ HANDPOS ] );
					UINT16 usItemUsemUsed = pSoldier->GetUsedWeaponNumber( &pSoldier->inv[ HANDPOS ] );

					if ( pObjUsed->exists() == true )
					{
						usSoundID = Weapon[ usItemUsemUsed ].sLocknLoadSound;

						if ( usSoundID != 0 )
						{
							PlayJA2Sample( usSoundID, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
						}
					}
				}
				break;

			case 709:

				// Knife throw sound...
				PlayJA2Sample( Weapon[ THROWING_KNIFE ].sSound, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
				break;

			case 710:

				// Monster footstep in
				if ( SoldierOnScreen( pSoldier->ubID ) )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, ACR_STEP_1, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 711:

				// Monster footstep in
				if ( SoldierOnScreen( pSoldier->ubID ) )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, ACR_STEP_2, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 712:

				// Monster footstep in
				if ( SoldierOnScreen( pSoldier->ubID ) )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, LCR_MOVEMENT, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 713:

				// Monster footstep in
				if ( pSoldier->ubBodyType == INFANT_MONSTER )
				{
					if ( SoldierOnScreen( pSoldier->ubID ) )
					{
						PlaySoldierJA2Sample( pSoldier->ubID, BCR_DRAGGING, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
					}
				}
				break;

			case 714:

				// Lunges....
				PlaySoldierJA2Sample( pSoldier->ubID, ACR_LUNGE, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 715:

				// Swipe
				PlaySoldierJA2Sample( pSoldier->ubID, ACR_SWIPE, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 716:

				// Eat flesh
				PlaySoldierJA2Sample( pSoldier->ubID, ACR_EATFLESH, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 717:

				// Battle cry
				{
					INT32			iSoundID=0;
					BOOLEAN		fDoCry = FALSE;

					//if ( SoldierOnScreen( pSoldier->ubID ) )
					{
						switch( pSoldier->aiData.usActionData )
						{
						case CALL_1_PREY:

							if ( pSoldier->ubBodyType == QUEENMONSTER )
							{
								iSoundID = LQ_SMELLS_THREAT;
							}
							else
							{
								iSoundID = ACR_SMEEL_PREY;
							}
							fDoCry = TRUE;
							break;

						case CALL_MULTIPLE_PREY:

							if ( pSoldier->ubBodyType == QUEENMONSTER )
							{
								iSoundID = LQ_SMELLS_THREAT;
							}
							else
							{
								iSoundID = ACR_SMELL_THREAT;
							}
							fDoCry = TRUE;
							break;

						case CALL_ATTACKED:

							if ( pSoldier->ubBodyType == QUEENMONSTER )
							{
								iSoundID = LQ_ENRAGED_ATTACK;
							}
							else
							{
								iSoundID = ACR_SMELL_THREAT;
							}
							fDoCry = TRUE;
							break;

						case CALL_CRIPPLED:

							if ( pSoldier->ubBodyType == QUEENMONSTER )
							{
								iSoundID = LQ_CRIPPLED;
							}
							else
							{
								iSoundID = ACR_CRIPPLED;
							}
							fDoCry = TRUE;
							break;
						}

						if ( fDoCry )
						{
							PlayJA2Sample( iSoundID, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
						}
					}
				}
				break;

			case 718:


				PlaySoldierJA2Sample( pSoldier->ubID, LQ_RUPTURING, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 719:

				// Spit attack start sound...
				PlaySoldierJA2Sample( pSoldier->ubID, LQ_ENRAGED_ATTACK, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 720:

				// Spit attack start sound...
				PlaySoldierJA2Sample( pSoldier->ubID, LQ_WHIP_ATTACK, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 721:
				// Play fall from knees to ground...
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( FALL_TO_GROUND_1 + Random(3) ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				if ( pSoldier->usAnimState == FALLFORWARD_FROMHIT_STAND )
				{
					CheckEquipmentForFragileItemDamage( pSoldier, 20 );
				}
				break;

			case 722:
				// Play fall heavy
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( HEAVY_FALL_1 ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				if ( pSoldier->usAnimState == FALLFORWARD_FROMHIT_CROUCH )
				{
					CheckEquipmentForFragileItemDamage( pSoldier, 15 );
				}
				break;

			case 723:

				// Play armpit noise...
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( IDLE_ARMPIT ), RATE_11025, SoundVolume( LOWVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 724:

				// Play ass scratch
				// PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( IDLE_SCRATCH ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 725:

				// Play back crack
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( IDLE_BACKCRACK ), RATE_11025, SoundVolume( LOWVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 726:

				// Kickin door
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( KICKIN_DOOR ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 727:

				// Swoosh
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( SWOOSH_1 + Random( 6 ) ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 728:

				// Creature fall
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( ACR_FALL_1 ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 729:

				// grab roof....
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( GRAB_ROOF ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 730:

				// end climb roof....
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( LAND_ON_ROOF ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 731:

				// Stop climb roof..
				PlaySoldierJA2Sample( pSoldier->ubID, (UINT8)( FALL_TO_GROUND_1 + Random(3) ), RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 732:

				// Play die sound
				pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
				pSoldier->flags.fDeadSoundPlayed = TRUE;
				break;

			case 750:

				// CODE: Move Vehicle UP
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
				{
					//	pSoldier->SetSoldierHeight( (FLOAT)( pSoldier->sHeightAdjustment + 1 ) );
				}
				break;

			case 751:

				// CODE: Move vehicle down
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
				{
					//		pSoldier->SetSoldierHeight( (FLOAT)( pSoldier->sHeightAdjustment - 1 ) );
				}
				break;

			case 752:

				// Code: decapitate
				DecapitateCorpse( pSoldier, pSoldier->sTargetGridNo, pSoldier->bTargetLevel );
				break;

			case 753:

				// code: freeup attcker
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count..., CODE FROM ANIMATION %s ( %d )", gAnimControl[ pSoldier->usAnimState ].zAnimStr, pSoldier->usAnimState ) );
				DebugAttackBusy( String("@@@@@@@ Reducing attacker busy count..., CODE FROM ANIMATION %s ( %d )\n", gAnimControl[ pSoldier->usAnimState ].zAnimStr, pSoldier->usAnimState ) );
				// ReduceAttackBusyCount( pSoldier->ubID, FALSE );

				// ATE: Here, reduce again if creaturequeen tentical attack...
				// Uh, why not add a second 753 code to the queen swipe instead of adding code to make things more complex?
				if ( pSoldier->usAnimState == QUEEN_SWIPE )
				{
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Reducing attacker busy count for end of queen swipe\n" ) );
					DebugAttackBusy( "@@@@@@@ Reducing attacker busy count for end of queen swipe" );
					// ReduceAttackBusyCount( pSoldier->ubID, FALSE );
				}
				
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				// SANDRO - if pending interrupt flag was set for after-attack type of interupt, try to resolve it now
				if ( gGameOptions.fImprovedInterruptSystem )
				{
					ResolvePendingInterrupt( pSoldier, AFTERACTION_INTERRUPT );
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				break;

			case 754:

				HandleFallIntoPitFromAnimation( pSoldier->ubID );
				break;

			case 755 :

				DishoutQueenSwipeDamage( pSoldier );
				break;

			case 756:

				// Reload robot....
				{
					UINT8				ubPerson;
					SOLDIERTYPE	*pRobot;

					// Get pointer...
					ubPerson = WhoIsThere2( pSoldier->aiData.sPendingActionData2, pSoldier->pathing.bLevel );

					if ( ubPerson != NOBODY && MercPtrs[ ubPerson ]->flags.uiStatusFlags & SOLDIER_ROBOT )
					{
						pRobot = MercPtrs[ ubPerson ];

						ReloadGun( pRobot, &(pRobot->inv[ HANDPOS ] ), pSoldier->pTempObject );

						// OK, check what was returned and place in inventory if it's non-zero
						if ( pSoldier->pTempObject->exists() == true )
						{
							// Add to inv..
							AutoPlaceObject( pSoldier, pSoldier->pTempObject, TRUE );
						}

						OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
					}
				}
				break;

			case 757:

				// INcrement attacker busy count....
//				gTacticalStatus.ubAttackBusyCount++;
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("!!!!! Incrementing attacker busy count..., CODE FROM ANIMATION %s ( %d ) : Count now %d", gAnimControl[ pSoldier->usAnimState ].zAnimStr, pSoldier->usAnimState, gTacticalStatus.ubAttackBusyCount ) );
				DebugAttackBusy( String("!!!!! CODE FROM ANIMATION %s ( %d )\n", gAnimControl[ pSoldier->usAnimState ].zAnimStr, pSoldier->usAnimState ) );
				break;

			case 758:

				// Trigger after slap...
				TriggerNPCWithGivenApproach( QUEEN, APPROACH_DONE_SLAPPED , TRUE );
				break;

			case 759:

				// Getting hit by slap
				{
					SOLDIERTYPE *pTarget;

					pTarget = FindSoldierByProfileID( ELLIOT, FALSE );

					if ( pTarget )
					{
						pTarget->EVENT_InitNewSoldierAnim( SLAP_HIT, 0 , FALSE );

						// Play noise....
						//PlaySoldierJA2Sample( pTarget->ubID, ( S_SLAP_IMPACT ), RATE_11025, SoundVolume( HIGHVOLUME, pTarget->sGridNo ), 1, SoundDir( pTarget->sGridNo ), TRUE );

						//DoMercBattleSound( pTarget, (INT8)( BATTLE_SOUND_HIT1 + Random( 2 ) ) );

					}
				}
				break;

			case 760:

				// Get some blood.....
				// Corpse Id is from pending action data
				GetBloodFromCorpse( pSoldier );
				// Dirty interface....
				DirtyMercPanelInterface( pSoldier, DIRTYLEVEL2 );
				break;

			case 761:

				{
					// Dish out damage!
					MercPtrs[ pSoldier->aiData.uiPendingActionData4 ]->EVENT_SoldierGotHit( TAKE_DAMAGE_BLADE, (INT16) 25, (INT16) 25, gOppositeDirection[ pSoldier->ubDirection ], 50, pSoldier->ubID, 0, ANIM_PRONE, 0, 0 );
				}
				break;

			case 762:
				{
					// CODE: Set off Trigger
					INT8	bPanicTrigger;

					bPanicTrigger = ClosestPanicTrigger( pSoldier );
					SetOffPanicBombs( pSoldier->ubID, bPanicTrigger );
					// any AI guy has been specially given keys for this, now take them
					// away
					pSoldier->flags.bHasKeys = pSoldier->flags.bHasKeys >> 1;

				}
				break;

			case 763:

				// CODE: Drop item at gridno
				if ( pSoldier->pTempObject != NULL )
				{
					if ( pSoldier->bVisible != -1 )
					{
						PlayJA2Sample( THROW_IMPACT_2, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ) );
					}

					AddItemToPool( pSoldier->aiData.sPendingActionData2, pSoldier->pTempObject, 1, pSoldier->pathing.bLevel, 0 , -1 );
					NotifySoldiersToLookforItems( );

					OBJECTTYPE::DeleteMe( &pSoldier->pTempObject );
				}
				break;

			case 764:

				PlaySoldierJA2Sample( pSoldier->ubID, PICKING_LOCK, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 765:

				// Flyback hit - do blood!
				// PLace in existing tile and one back...
				{
						INT32 sNewGridNo;

					InternalDropBlood( pSoldier->sGridNo, pSoldier->pathing.bLevel, 0, (UINT8)(MAXBLOODQUANTITY), 1 );

					// Move forward one gridno....
						sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( gOppositeDirection[ pSoldier->ubDirection ] ) );

					InternalDropBlood( sNewGridNo, pSoldier->pathing.bLevel, 0, (UINT8)(MAXBLOODQUANTITY), 1 );

				}
				break;

			case 766:

				// Say COOL quote
				pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
				break;

			case 767:

				// Slap sound effect
				PlaySoldierJA2Sample( pSoldier->ubID, SLAP_2, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				break;

			case 768:

				// OK, after ending first aid, stand up if not in combat....
				if ( NumCapableEnemyInSector( ) == 0 )
				{
					// Stand up...
					pSoldier->ChangeSoldierStance( ANIM_STAND );
					return( FALSE );
				}
				break;

			case 769:

				// ATE: LOOK HERE FOR CODE IN INTERNALS FOR
				// REFUELING A VEHICLE
				// THE GAS_CAN IS IN THE MERCS MAIN HAND AT THIS TIME
				{
					UINT8				ubPerson;
					SOLDIERTYPE *pVehicle;

					// Get pointer to vehicle...
					ubPerson = WhoIsThere2( pSoldier->aiData.sPendingActionData2, pSoldier->pathing.bLevel );
					pVehicle = MercPtrs[ ubPerson ];

					// this is a ubID for soldiertype....
					AddFuelToVehicle( pSoldier, pVehicle );

					fInterfacePanelDirty = DIRTYLEVEL2;
				}
				break;

			case 770:

				PlayJA2Sample( USE_WIRE_CUTTERS, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				break;

			case 771:

				PlayJA2Sample( BLOODCAT_ATTACK, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				break;

			case 772:

				//CODE: FOR A REALTIME NON-INTERRUPTABLE SCRIPT - SIGNAL DONE
				pSoldier->flags.fRTInNonintAnim = FALSE;
				break;

			case 773:

				// Kneel up...
				if ( !pSoldier->bStealthMode )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, KNEEL_UP_SOUND, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 774:

				// Kneel down..
				if ( !pSoldier->bStealthMode )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, KNEEL_DOWN_SOUND, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 775:

				// prone up..
				if ( !pSoldier->bStealthMode )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, PRONE_UP_SOUND, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 776:

				// prone down..
				if ( !pSoldier->bStealthMode )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, PRONE_DOWN_SOUND, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 777:

				// picking something up
				if ( !pSoldier->bStealthMode )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, PICKING_SOMETHING_UP, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 778:
				if ( !pSoldier->bStealthMode )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, ENTER_DEEP_WATER_1, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 779:

				PlaySoldierJA2Sample( pSoldier->ubID, COW_FALL, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 780:

				PlaySoldierJA2Sample( pSoldier->ubID, COW_HIT_SND, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 781:

				PlaySoldierJA2Sample( pSoldier->ubID, ACR_DIE_PART2, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				break;

			case 782:

				PlaySoldierJA2Sample( pSoldier->ubID, CREATURE_DISSOLVE_1, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				break;

			case 784:

				PlaySoldierJA2Sample( pSoldier->ubID, CREATURE_FALL, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				break;

			case 785:

				if ( Random( 5 ) == 0 )
				{
					PlaySoldierJA2Sample( pSoldier->ubID, CROW_PECKING_AT_FLESH, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				}
				break;

			case 786:

				PlaySoldierJA2Sample( pSoldier->ubID, CROW_FLYING_AWAY, RATE_11025, SoundVolume( MIDVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 787:

				PlaySoldierJA2Sample( pSoldier->ubID, SLAP_1, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), FALSE );
				break;

			case 788:

				PlaySoldierJA2Sample( pSoldier->ubID, MORTAR_START, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 789:

				PlaySoldierJA2Sample( pSoldier->ubID, MORTAR_LOAD, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 790:

				PlaySoldierJA2Sample( pSoldier->ubID, COW_FALL_2, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 791:

				PlaySoldierJA2Sample( pSoldier->ubID, FENCE_OPEN, RATE_11025, SoundVolume( HIGHVOLUME, pSoldier->sGridNo ), 1, SoundDir( pSoldier->sGridNo ), TRUE );
				break;

			case 792:

				
				break;
			}
			// Adjust frame control pos, and try again
			pSoldier->usAniCode++;
		}
		else if ( sNewAniFrame == 999 )
		{

			// Go to start, by default
			pSoldier->usAniCode = 0;

		}
		else if ( sNewAniFrame > 999 )
		{
			// Jump, to animation script ( in the 300+ range )
			pSoldier->EVENT_InitNewSoldierAnim( (UINT16)(sNewAniFrame - 700 ), 0 , FALSE );
			return( TRUE );
		}

		// Loop here until we break on a real item!
	} while ( TRUE );

	// We're done
	return( TRUE );
}

#define MIN_DEADLINESS_FOR_LIKE_GUN_QUOTE			20

BOOLEAN ShouldMercSayHappyWithGunQuote( SOLDIERTYPE *pSoldier )
{
	// How do we do this....

	if ( QuoteExp[ pSoldier->ubProfile ].QuoteExpGotGunOrUsedGun == QUOTE_SATISFACTION_WITH_GUN_AFTER_KILL )
	{
		// For one, only once a day...
		if ( pSoldier->usQuoteSaidFlags & SOLDIER_QUOTE_SAID_LIKESGUN )
		{
			return( FALSE );
		}

		// is it a gun?
		if ( Item[ pSoldier->usAttackingWeapon ].usItemClass & IC_GUN )
		{
			// Is our weapon powerfull enough?
			if ( Weapon[ pSoldier->usAttackingWeapon ].ubDeadliness > MIN_DEADLINESS_FOR_LIKE_GUN_QUOTE )
			{
				// 20 % chance?
				if ( Random( 100 ) < 20 )
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}


void SayBuddyWitnessedQuoteFromKill( SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel )
{
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0);
//	UINT8	ubMercsInSector[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ] = { 0 };
	std::vector<INT8>	bBuddyIndex (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, -1);
//	INT8	bBuddyIndex[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ] = { -1 };
	INT8	bTempBuddyIndex;
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;
	UINT16	usQuoteNum;

	// Loop through all our guys and randomly say one from someone in our sector

	// set up soldier ptr as first element in mercptrs list
	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
	{
		// Add guy if he's a candidate...		
		if ( OK_INSECTOR_MERC( pTeamSoldier ) && !AM_AN_EPC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !(AM_A_ROBOT( pTeamSoldier )) && !pTeamSoldier->flags.fMercAsleep && !TileIsOutOfBounds(pTeamSoldier->sGridNo))
		{
			// Are we a buddy of killer?
			bTempBuddyIndex = WhichBuddy( pTeamSoldier->ubProfile, pKillerSoldier->ubProfile );

			if ( bTempBuddyIndex != -1 )
			{
				switch( bTempBuddyIndex )
				{
				case 0:
					if ( pTeamSoldier->usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_BUDDY_1_WITNESSED )
					{
						continue;
					}
					break;

				case 1:
					if ( pTeamSoldier->usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_BUDDY_2_WITNESSED )
					{
						continue;
					}
					break;

				case 2:
					if ( pTeamSoldier->usQuoteSaidExtFlags & SOLDIER_QUOTE_SAID_BUDDY_3_WITNESSED )
					{
						continue;
					}
					break;
				}

				// TO LOS check to killed
				// Can we see location of killer?
				if ( SoldierTo3DLocationLineOfSightTest( pTeamSoldier, pKillerSoldier->sGridNo,  pKillerSoldier->pathing.bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) == 0 )
				{
					continue;
				}


				// Can we see location of killed?
				if ( SoldierTo3DLocationLineOfSightTest( pTeamSoldier, sGridNo,  bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) == 0 )
				{
					continue;
				}

				// OK, a good candidate...
				ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
				bBuddyIndex[ ubNumMercs ]	 = bTempBuddyIndex;
				ubNumMercs++;
			}
		}
	}

	// If we are > 0
	if ( ubNumMercs > 0 )
	{
		// Do random check here...
		if ( Random( 100 ) < 20 )
		{
			ubChosenMerc = (UINT8)Random( ubNumMercs );

			switch( bBuddyIndex[ ubChosenMerc ] )
			{
			case 0:
				usQuoteNum = QUOTE_BUDDY_1_GOOD;
				MercPtrs[ ubMercsInSector[ ubChosenMerc ] ]->usQuoteSaidExtFlags |= SOLDIER_QUOTE_SAID_BUDDY_1_WITNESSED;
				break;

			case 1:
				usQuoteNum = QUOTE_BUDDY_2_GOOD;
				MercPtrs[ ubMercsInSector[ ubChosenMerc ] ]->usQuoteSaidExtFlags |= SOLDIER_QUOTE_SAID_BUDDY_2_WITNESSED;
				break;

			case 2:
				usQuoteNum = QUOTE_LEARNED_TO_LIKE_WITNESSED;
				MercPtrs[ ubMercsInSector[ ubChosenMerc ] ]->usQuoteSaidExtFlags |= SOLDIER_QUOTE_SAID_BUDDY_3_WITNESSED;
				break;
			}
			TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], usQuoteNum );
		}
	}
}


void HandleKilledQuote( SOLDIERTYPE *pKilledSoldier, SOLDIERTYPE *pKillerSoldier, INT32 sGridNo, INT8 bLevel )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt;
// WDS - make number of mercenaries, etc. be configurable
	std::vector<UINT8>	ubMercsInSector (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, 0);
//	UINT8	ubMercsInSector[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ] = { 0 };
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;
	BOOLEAN fDoSomeoneElse = FALSE;
	BOOLEAN	fCanWeSeeLocation = FALSE;

	gfLastMercTalkedAboutKillingID = pKilledSoldier->ubID;

	// Can we see location?
	fCanWeSeeLocation = ( SoldierTo3DLocationLineOfSightTest( pKillerSoldier, sGridNo,  bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) != 0 );


	// Are we killing mike?
	if ( pKilledSoldier->ubProfile == MIKE && pKillerSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
	{
		// Can we see?
		if ( fCanWeSeeLocation )
		{
			TacticalCharacterDialogue( pKillerSoldier, QUOTE_AIM_KILLED_MIKE );
		}
	}
	// Are we killing factory mamager?
	else if ( pKilledSoldier->ubProfile == DOREEN )
	{
		// Can we see?
		//f ( fCanWeSeeLocation )
		{
			TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLED_FACTORY_MANAGER );
		}
	}
	else
	{
		// Steps here...

		// If not head shot, just say killed quote

		// If head shot...

		// If we have a head shot saying,, randomly try that one

		// If not doing that one, search for anybody who can see person

		// If somebody did, play his quote plus attackers killed quote.

		// Checkf for headhot!
		if ( pKilledSoldier->usAnimState == JFK_HITDEATH )
		{
			//Randomliy say it!
			if ( Random( 100 ) < 40 )
			{
				TacticalCharacterDialogue( pKillerSoldier, QUOTE_HEADSHOT );
			}
			else
			{
				fDoSomeoneElse = TRUE;
			}

			if ( fDoSomeoneElse )
			{
				// Check if a person is here that has this quote....
				cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

				// run through list
				for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pTeamSoldier++ )
				{
					if ( cnt != pKillerSoldier->ubID )
					{
						if ( OK_INSECTOR_MERC( pTeamSoldier ) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_GASSED ) && !AM_AN_EPC( pTeamSoldier ) )
						{
							// Can we see location?
							if ( SoldierTo3DLocationLineOfSightTest( pTeamSoldier, sGridNo,  bLevel, 3, TRUE, CALC_FROM_ALL_DIRS ) )
							{
								ubMercsInSector[ ubNumMercs ] = (UINT8)cnt;
								ubNumMercs++;
							}
						}
					}
				}

				// Did we find anybody?
				if ( ubNumMercs > 0 )
				{
					ubChosenMerc = (UINT8)Random( ubNumMercs );

					// We have a random chance of not saying our we killed a guy quote
					if ( Random( 100 ) < 50 )
					{
						// Say this guys quote but the killer's quote as well....
						// if killed was not a plain old civ, say quote
						if (pKilledSoldier->bTeam != CIV_TEAM || pKilledSoldier->ubCivilianGroup != 0)
						{
							TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLED_AN_ENEMY );
						}
					}

					TacticalCharacterDialogue( MercPtrs[ ubMercsInSector[ ubChosenMerc ] ], QUOTE_HEADSHOT );
				}
				else
				{
					// Can we see?
					if ( fCanWeSeeLocation )
					{
						// Say this guys quote but the killer's quote as well....
						// if killed was not a plain old civ, say quote
						if (pKilledSoldier->bTeam != CIV_TEAM || pKilledSoldier->ubCivilianGroup != 0)
						{
							TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLED_AN_ENEMY );
						}
					}
				}
			}
		}
		else
		{
			// Can we see?
			if ( fCanWeSeeLocation )
			{
				// if killed was not a plain old civ, say quote
				if (pKilledSoldier->bTeam != CIV_TEAM || pKilledSoldier->ubCivilianGroup != 0)
				{
					// Are we happy with our gun?
					if ( ShouldMercSayHappyWithGunQuote( pKillerSoldier )	)
					{
						TacticalCharacterDialogue( pKillerSoldier, QUOTE_SATISFACTION_WITH_GUN_AFTER_KILL );
						pKillerSoldier->usQuoteSaidFlags |= SOLDIER_QUOTE_SAID_LIKESGUN;
					}
					else
						// Randomize between laugh, quote...
					{
						if ( Random( 100 ) < 33 && pKilledSoldier->ubBodyType != BLOODCAT )
						{
							// If it's a creature......
							if ( pKilledSoldier->flags.uiStatusFlags & SOLDIER_MONSTER )
							{
								TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLED_A_CREATURE );
							}
							else
							{
								TacticalCharacterDialogue( pKillerSoldier, QUOTE_KILLED_AN_ENEMY );
							}
						}
						else
						{
							if ( Random( 50 ) == 25 )
							{
								pKillerSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_LAUGH1 ) );
							}
							else
							{
								pKillerSoldier->DoMercBattleSound( (INT8)( BATTLE_SOUND_COOL1 ) );
							}
						}
					}

					// Buddy witnessed?
					SayBuddyWitnessedQuoteFromKill( pKillerSoldier, sGridNo, bLevel );

				}
			}
		}
	}
}



BOOLEAN HandleSoldierDeath( SOLDIERTYPE *pSoldier , BOOLEAN *pfMadeCorpse )
{
	BOOLEAN fBuddyJustDead = FALSE;
	*pfMadeCorpse = FALSE;

	if ( pSoldier->stats.bLife == 0 && !( pSoldier->flags.uiStatusFlags & SOLDIER_DEAD )	)
	{
		// Haydent/send death info
		if (is_networked)
		{			
			if(pSoldier->bTeam==0) 
				send_death(pSoldier);
			else if(pSoldier->bTeam <6 && ((gTacticalStatus.ubTopMessageType == PLAYER_TURN_MESSAGE) || (gTacticalStatus.ubTopMessageType == PLAYER_INTERRUPT_MESSAGE)))
				send_death(pSoldier);						
		}
				
		// Cancel services here...
		pSoldier->ReceivingSoldierCancelServices( );
		pSoldier->GivingSoldierCancelServices( );

		if ( pSoldier->iMuzFlash != -1 )
		{
			LightSpriteDestroy( pSoldier->iMuzFlash );
			pSoldier->iMuzFlash = -1;
		}
		if ( pSoldier->iLight != -1 )
		{
			LightSpriteDestroy( pSoldier->iLight );
		}

		//FREEUP GETTING HIT FLAG
		pSoldier->flags.fGettingHit = FALSE;

		// Find next closest team member!
		if ( pSoldier->bTeam == gbPlayerNum )
		{
			// Set guy to close panel!
			// ONLY IF VISIBLE ON SCREEN
			if ( IsMercPortraitVisible( pSoldier->ubID ) )
			{
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
			pSoldier->flags.fUIdeadMerc = TRUE;

			if ( !gfKillingGuysForLosingBattle )
			{
				// ATE: THIS IS S DUPLICATE SETTING OF SOLDIER_DEAD. Is set in StrategicHandlePlayerTeamMercDeath()
				// also, but here it's needed to tell tectical to ignore this dude...
				// until StrategicHandlePlayerTeamMercDeath() can get called after death skull interface is done
				pSoldier->flags.uiStatusFlags |= SOLDIER_DEAD;

			}
		}
		else
		{
			//////////////////////////////////////////////////////////////
			// SANDRO - some changes here
			UINT8   ubAttacker = pSoldier->ubAttackerID; 
			UINT8	ubAssister = pSoldier->ubPreviousAttackerID;
			// If attacker is nobody, and we died, then set the last attacker(if exists) as our killer 
			if ( ubAttacker == NOBODY )
			{
				if ( ubAssister != NOBODY )
				{
					ubAttacker = pSoldier->ubPreviousAttackerID;
					ubAssister = pSoldier->ubNextToPreviousAttackerID;
				}
				else if ( pSoldier->ubNextToPreviousAttackerID != NOBODY )
				{
					ubAttacker = pSoldier->ubNextToPreviousAttackerID;
					ubAssister = NOBODY;
				}
			}
			else
			{
				if ( ubAssister == NOBODY )
				{
					ubAssister = pSoldier->ubNextToPreviousAttackerID;
				}
			}

			//////////////////////////////////////////////////////////////

			{
				// IF this guy has an attacker and he's a good guy, play sound
				if ( ubAttacker != NOBODY )
				{								
					if ( MercPtrs[ pSoldier->ubAttackerID ] != NULL && MercPtrs[ pSoldier->ubAttackerID ]->bTeam == gbPlayerNum && gTacticalStatus.ubAttackBusyCount > 0 )
					{
						gTacticalStatus.fKilledEnemyOnAttack	= TRUE;
						gTacticalStatus.ubEnemyKilledOnAttack = pSoldier->ubID;
						gTacticalStatus.ubEnemyKilledOnAttackLocation = pSoldier->sGridNo;
						gTacticalStatus.bEnemyKilledOnAttackLevel = pSoldier->pathing.bLevel;
						gTacticalStatus.ubEnemyKilledOnAttackKiller = ubAttacker;

						// also check if we are in mapscreen, if so update soldier's list
						if( guiCurrentScreen == MAP_SCREEN )
						{
							ReBuildCharactersList( );
						}
					}
					else if ( pSoldier->bVisible == TRUE )
					{
						// We were a visible enemy, say laugh!
						if ( Random(3) == 0 && !CREATURE_OR_BLOODCAT( MercPtrs[ ubAttacker ] ) )
						{
							MercPtrs[ ubAttacker ]->DoMercBattleSound( BATTLE_SOUND_LAUGH1 );
						}
					}					
				}
			
				// Handle NPC Dead
				HandleNPCTeamMemberDeath( pSoldier );				
				
				// if a friendly with a profile, increment kills
				// militia also now track kills...
				if ( ubAttacker != NOBODY )
				{
					if ( MercPtrs[ ubAttacker ]->bTeam == gbPlayerNum )
					{
						// increment kills
						/////////////////////////////////////////////////////////////////////////////////////
						// SANDRO - experimental - more specific statistics of mercs
						switch(pSoldier->ubSoldierClass)
						{
							case SOLDIER_CLASS_ELITE :
								gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsElites++;
								break;
							case SOLDIER_CLASS_ARMY :
								gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsRegulars++;
								break;
							case SOLDIER_CLASS_ADMINISTRATOR :
								gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsAdmins++;
								break;
							case SOLDIER_CLASS_CREATURE :
								gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsCreatures++;
								break;
							default :
								if ( CREATURE_OR_BLOODCAT( pSoldier ) )
									gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsCreatures++;
								else if ( TANK( pSoldier ) )
									gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsTanks++;
								else if ( pSoldier->bTeam == CIV_TEAM && !pSoldier->aiData.bNeutral && pSoldier->bSide != gbPlayerNum )
									gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsHostiles++;
								else
									gMercProfiles[ MercPtrs[ ubAttacker ]->ubProfile ].records.usKillsOthers++;
								break;
						}
						//gMercProfiles[ MercPtrs[ pSoldier->ubAttackerID ]->ubProfile ].usKills++;
						/////////////////////////////////////////////////////////////////////////////////////
						gStrategicStatus.usPlayerKills++;
					}
					else if ( MercPtrs[ ubAttacker ]->bTeam == MILITIA_TEAM )
					{
						// get a kill! 2 points!
						MercPtrs[ ubAttacker ]->ubMilitiaKills += 2;
					}

				}
				
				if ( ubAssister != NOBODY && ubAssister != ubAttacker )
				{
					if ( MercPtrs[ ubAssister ]->bTeam == gbPlayerNum )
					{
						/////////////////////////////////////////////////////////////////////////////////////
						// SANDRO - new mercs' records
						if( MercPtrs[ ubAttacker ] != NULL )
						{
							if( MercPtrs[ ubAttacker ]->bTeam == gbPlayerNum )
								gMercProfiles[ MercPtrs[ ubAssister ]->ubProfile ].records.usAssistsMercs++;
							else if ( MercPtrs[ ubAttacker ]->bTeam == MILITIA_TEAM )
								gMercProfiles[ MercPtrs[ ubAssister ]->ubProfile ].records.usAssistsMilitia++;
							else
								gMercProfiles[ MercPtrs[ ubAssister ]->ubProfile ].records.usAssistsOthers++;
						}
						else
						{
							gMercProfiles[ MercPtrs[ ubAssister ]->ubProfile ].records.usAssistsOthers++;
						}
						//gMercProfiles[ MercPtrs[ ubAssister ]->ubProfile ].usAssists++;
						/////////////////////////////////////////////////////////////////////////////////////
					}
					else if ( MercPtrs[ ubAssister ]->bTeam == MILITIA_TEAM )
					{
						// get an assist - 1 points
						MercPtrs[ ubAssister ]->ubMilitiaKills += 1;
					}
				}
				/*
				// handle assist
				// if killer is assister, don't increment
				if ( pSoldier->ubPreviousAttackerID != NOBODY && pSoldier->ubPreviousAttackerID != pSoldier->ubAttackerID )
				{
				if ( MercPtrs[ pSoldier->ubPreviousAttackerID ]->bTeam == gbPlayerNum )
				{
				gMercProfiles[ MercPtrs[ pSoldier->ubPreviousAttackerID ]->ubProfile ].usAssists++;
				}
				else if ( MercPtrs[ pSoldier->ubPreviousAttackerID ]->bTeam == MILITIA_TEAM )
				{
				// get an assist - 1 points
				MercPtrs[ pSoldier->ubPreviousAttackerID ]->ubMilitiaKills += 1;
				}
				}
				*/
			}
		}

		if ( TurnSoldierIntoCorpse( pSoldier, TRUE, TRUE ) )
		{
			*pfMadeCorpse = TRUE;
		}

		// Remove mad as target, one he has died!
		RemoveManAsTarget( pSoldier );

		// Re-evaluate visiblitiy for the team!
		BetweenTurnsVisibilityAdjustments();

		// 0verhaul: This is now handled in the death state transitions
		// if ( pSoldier->bTeam != gbPlayerNum )
		// {
		//	if ( !pSoldier->flags.fDoingExternalDeath )
				//	{
		//		// Release attacker
		//		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, code 497 = handle soldier death") );
		//		ReleaseSoldiersAttacker( pSoldier );
		//	}
		// }

		if ( !( *pfMadeCorpse ) )
		{
			fBuddyJustDead = TRUE;
		}

	}

	if ( pSoldier->stats.bLife > 0 )
	{
		// If we are here - something funny has heppende
		// We either have played a death animation when we are not dead, or we are calling
		// this ani code in an animation which is not a death animation
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Soldier Ani: Death animation called when not dead..." );
	}

	return( fBuddyJustDead );
}


void HandlePlayerTeamMemberDeathAfterSkullAnimation( SOLDIERTYPE *pSoldier )
{
	// 0verhaul:	This is now handled in the death state transition.
	// Release attacker
	// if ( !pSoldier->flags.fDoingExternalDeath )
	// {
	//	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("@@@@@@@ Releasesoldierattacker, code 497 = handle soldier death") );
	//	ReleaseSoldiersAttacker( pSoldier );
	// }

	HandlePlayerTeamMemberDeath( pSoldier );

	// now remove character from a squad
	RemoveCharacterFromSquads( pSoldier );
}

BOOLEAN CheckForAndHandleSoldierDeath( SOLDIERTYPE *pSoldier, BOOLEAN *pfMadeCorpse )
{

	if ( HandleSoldierDeath( pSoldier, pfMadeCorpse ) )
	{
		// Select approriate death
		switch( pSoldier->usAnimState )
		{
		case FLYBACK_HIT_DEATH:
			pSoldier->ChangeSoldierState( FLYBACK_HITDEATH_STOP, 0, FALSE );
			break;

		case GENERIC_HIT_DEATH:
			pSoldier->ChangeSoldierState( FALLFORWARD_HITDEATH_STOP, 0, FALSE );
			break;

		case FALLBACK_HIT_DEATH:
			pSoldier->ChangeSoldierState( FALLBACK_HITDEATH_STOP, 0, FALSE );
			break;

		case PRONE_HIT_DEATH:
			pSoldier->ChangeSoldierState( PRONE_HITDEATH_STOP, 0, FALSE );
			break;

		case JFK_HITDEATH:
			pSoldier->ChangeSoldierState( JFK_HITDEATH_STOP, 0, FALSE );
			break;

		case FALLOFF_DEATH:
			pSoldier->ChangeSoldierState( FALLOFF_DEATH_STOP, 0, FALSE );
			break;

		case FALLOFF_FORWARD_DEATH:
			pSoldier->ChangeSoldierState( FALLOFF_FORWARD_DEATH_STOP, 0, FALSE );
			break;

		case WATER_DIE:
			pSoldier->ChangeSoldierState( WATER_DIE_STOP, 0, FALSE );
			break;

		case DEEP_WATER_DIE:
			pSoldier->ChangeSoldierState( DEEP_WATER_DIE_STOPPING, 0, FALSE );
			break;

		case COW_DYING:
			pSoldier->ChangeSoldierState( COW_DYING_STOP, 0, FALSE);
			break;

		case BLOODCAT_DYING:
			pSoldier->ChangeSoldierState( BLOODCAT_DYING_STOP, 0, FALSE);
			break;

		default:

			// IF we are here - something is wrong - we should have an animation stop here
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Soldier Ani: CODE 440 Error, Death STOP not handled" );
		}

		return( TRUE );
	}

	return( FALSE );
}


//#define TESTFALLBACK
//#define TESTFALLFORWARD

void CheckForAndHandleSoldierIncompacitated( SOLDIERTYPE *pSoldier )
{
	INT32					sNewGridNo;

	if ( pSoldier->stats.bLife < OKLIFE )
	{
		// Cancel services here...
		pSoldier->ReceivingSoldierCancelServices( );
		pSoldier->GivingSoldierCancelServices( );


		// If we are a monster, set life to zero ( no unconscious )
		switch( pSoldier->ubBodyType )
		{
		case ADULTFEMALEMONSTER:
		case AM_MONSTER:
		case YAF_MONSTER:
		case YAM_MONSTER:
		case LARVAE_MONSTER:
		case INFANT_MONSTER:
		case CRIPPLECIV:
		case ROBOTNOWEAPON:
		case QUEENMONSTER:
		case TANK_NW:
		case TANK_NE:

			pSoldier->stats.bLife = 0;
			break;
		}

		// OK, if we are in a meanwhile and this is elliot...
#ifdef JA2UB
//ja25: No queen
#else
		if ( AreInMeanwhile( ) )
		{
			SOLDIERTYPE *pQueen;

			pQueen = FindSoldierByProfileID( QUEEN, FALSE );

			if ( pQueen )
			{
				TriggerNPCWithGivenApproach( QUEEN, APPROACH_DONE_SLAPPED, FALSE );
			}
		}
#endif
		// We are unconscious now, play randomly, this animation continued, or a new death
		if ( pSoldier->CheckSoldierHitRoof( ) )
		{
			return;
		}

		// If guy is now dead, play sound!
		if ( pSoldier->stats.bLife == 0	)
		{
#ifdef JA2UB
//Ja25 No meanwhiles		
#else
			if ( !AreInMeanwhile() )
#endif
			{
				pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
				pSoldier->flags.fDeadSoundPlayed = TRUE;
			}
		}

		// Randomly fall back or forward, if we are in the standing hit animation
		if ( pSoldier->usAnimState == GENERIC_HIT_STAND || pSoldier->usAnimState == STANDING_BURST_HIT || pSoldier->usAnimState == RIFLE_STAND_HIT )
		{
			INT8		bTestDirection	= pSoldier->ubDirection;
			BOOLEAN		fForceDirection = FALSE;
			BOOLEAN		fDoFallback		= FALSE;
			BOOLEAN		fAlwaysFallBack = FALSE; // added by SANDRO


			// Lesh: lets fix dead humans fallback through obstacles

			// TRY FALLING BACKWARDS, ( ONLY IF WE ARE A MERC! )
#ifdef TESTFALLBACK
			if ( IS_MERC_BODY_TYPE( pSoldier ) )
#elif defined ( TESTFALLFORWARD )
			if ( 0 )
#else
			// SANDRO - if Martial Artist took someone down, always fall back if possible (for the fun)
			if ( pSoldier->ubAttackerID != NOBODY && gGameOptions.fNewTraitSystem )
			{ 
				if ( HAS_SKILL_TRAIT( MercPtrs[ pSoldier->ubAttackerID ], MARTIAL_ARTS_NT ) && (!MercPtrs[ pSoldier->ubAttackerID ]->usAttackingWeapon || Item[MercPtrs[ pSoldier->ubAttackerID ]->inv[HANDPOS].usItem].brassknuckles ) )
				{
					fAlwaysFallBack = TRUE;
				}
			}
			if ( (Random(100 ) > 40 || fAlwaysFallBack) && IS_MERC_BODY_TYPE( pSoldier ) && !IsProfileATerrorist( pSoldier->ubProfile ) )
#endif
			{
				// CHECK IF WE HAVE AN ATTACKER, TAKE OPPOSITE DIRECTION!
				if ( pSoldier->ubAttackerID != NOBODY )
				{
					// Find direction!
					bTestDirection = (INT8)GetDirectionFromGridNo( MercPtrs[ pSoldier->ubAttackerID ]->sGridNo, pSoldier );
					fForceDirection = TRUE;
				}

				sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( gOppositeDirection[ bTestDirection ] ) );

				if ( OKFallDirection( pSoldier, sNewGridNo, pSoldier->pathing.bLevel, bTestDirection, FLYBACK_HIT ) )
				{
					// CHECKED BEHIND GRIDS - OK
					fDoFallback = TRUE;
				}
				else
				{
					fDoFallback = FALSE;
				}

			}
			else
			{
				fDoFallback = FALSE;
			}

			if ( !fDoFallback )
			{
				// 1 ) REC DIRECTION
				// 2 ) SET FLAG FOR STARTING TO FALL
				pSoldier->BeginTyingToFall( );
				pSoldier->ChangeSoldierState( FALLFORWARD_FROMHIT_STAND, 0, FALSE );
				return;
			}
			else
			{
				// ALL'S OK HERE..... IF WE FORCED DIRECTION, SET!
				if ( fForceDirection )
				{
					pSoldier->EVENT_SetSoldierDesiredDirection( bTestDirection );
					pSoldier->EVENT_SetSoldierDirection( bTestDirection );
				}
				pSoldier->ChangeToFallbackAnimation( pSoldier->ubDirection );
				return;
			}
		}
		else if ( pSoldier->usAnimState == GENERIC_HIT_CROUCH || pSoldier->usAnimState == CIV_COWER_HIT)
		{
			pSoldier->ChangeSoldierState( FALLFORWARD_FROMHIT_CROUCH, 0 , FALSE);
			pSoldier->BeginTyingToFall( );
			return;
		}
		else if ( pSoldier->usAnimState == GENERIC_HIT_PRONE )
		{
			pSoldier->ChangeSoldierState( PRONE_LAY_FROMHIT, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == ADULTMONSTER_HIT )
		{
			pSoldier->ChangeSoldierState( ADULTMONSTER_DYING, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == LARVAE_HIT )
		{
			pSoldier->ChangeSoldierState( LARVAE_DIE, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == QUEEN_HIT )
		{
			pSoldier->ChangeSoldierState( QUEEN_DIE, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == CRIPPLE_HIT )
		{
			pSoldier->ChangeSoldierState( CRIPPLE_DIE, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == ROBOTNW_HIT )
		{
			pSoldier->ChangeSoldierState( ROBOTNW_DIE, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == INFANT_HIT )
		{
			pSoldier->ChangeSoldierState( INFANT_DIE, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == COW_HIT )
		{
			pSoldier->ChangeSoldierState( COW_DYING, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == BLOODCAT_HIT )
		{
			pSoldier->ChangeSoldierState( BLOODCAT_DYING, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == WATER_HIT )
		{
			pSoldier->ChangeSoldierState( WATER_DIE, 0 , FALSE );
			return;
		}
		else if ( pSoldier->usAnimState == DEEP_WATER_HIT )
		{
			pSoldier->ChangeSoldierState( DEEP_WATER_DIE, 0 , FALSE );
			return;
		}
		else
		{
			// We have missed something here - send debug msg
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Soldier Ani: Genmeric hit not chained" );
		}
	}

}


BOOLEAN CheckForAndHandleSoldierDyingNotFromHit( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->stats.bLife == 0 )
	{
		pSoldier->DoMercBattleSound( BATTLE_SOUND_DIE1 );
		pSoldier->flags.fDeadSoundPlayed = TRUE;

		// 0verhaul:	The bBeingAttackedCount is now obsolete.
		// Increment	being attacked count
		// pSoldier->bBeingAttackedCount++;

		// OJW - Send bleeding death
		if (is_networked)
		{
			if(pSoldier->bTeam==0) send_death(pSoldier);
			else if(pSoldier->bTeam <6 && ((gTacticalStatus.ubTopMessageType == PLAYER_TURN_MESSAGE) || (gTacticalStatus.ubTopMessageType == PLAYER_INTERRUPT_MESSAGE)))send_death(pSoldier);
			else if (pSoldier->bTeam < 6 && (is_server)) send_death(pSoldier);
		}
		if ( gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ] )
		{
			switch( pSoldier->usAnimState )
			{
			case FLYBACKHIT_STOP:
				pSoldier->ChangeSoldierState( FLYBACK_HIT_DEATH, 0, FALSE );
				break;

			case FALLFORWARD_FROMHIT_STAND:
			case FALLFORWARD_FROMHIT_CROUCH:
			case STAND_FALLFORWARD_STOP:
				pSoldier->ChangeSoldierState( GENERIC_HIT_DEATH, 0, FALSE );
				break;

			case FALLBACKHIT_STOP:
				pSoldier->ChangeSoldierState( FALLBACK_HIT_DEATH, 0, FALSE );
				break;

			case PRONE_LAYFROMHIT_STOP:
			case PRONE_LAY_FROMHIT:

				pSoldier->ChangeSoldierState( PRONE_HIT_DEATH, 0, FALSE );
				break;

			case FALLOFF_STOP:
				pSoldier->ChangeSoldierState( FALLOFF_DEATH, 0, FALSE );
				break;

			case FALLOFF_FORWARD_STOP:
				pSoldier->ChangeSoldierState( FALLOFF_FORWARD_DEATH, 0, FALSE);
				break;

			case ADULTMONSTER_HIT:
				pSoldier->ChangeSoldierState( ADULTMONSTER_DYING, 0 , FALSE );
				break;

			case LARVAE_HIT:
				pSoldier->ChangeSoldierState( LARVAE_DIE, 0 , FALSE );
				break;

			case QUEEN_HIT:
				pSoldier->ChangeSoldierState( QUEEN_DIE, 0 , FALSE );
				break;

			case CRIPPLE_HIT:
				pSoldier->ChangeSoldierState( CRIPPLE_DIE, 0 , FALSE );
				break;

			case ROBOTNW_HIT:
				pSoldier->ChangeSoldierState( ROBOTNW_DIE, 0 , FALSE );
				break;

			case INFANT_HIT:
				pSoldier->ChangeSoldierState( INFANT_DIE, 0 , FALSE );
				break;

			case COW_HIT:
				pSoldier->ChangeSoldierState( COW_DYING, 0 , FALSE );
				break;

			case BLOODCAT_HIT:
				pSoldier->ChangeSoldierState( BLOODCAT_DYING, 0 , FALSE );
				break;

			default:
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Control: Death state %d has no death hit", pSoldier->usAnimState ) );
				{
					BOOLEAN fMadeCorpse;
					CheckForAndHandleSoldierDeath( pSoldier, &fMadeCorpse );
				}
				break;

			}
		}
		else
		{
			BOOLEAN fMadeCorpse;

			CheckForAndHandleSoldierDeath( pSoldier, &fMadeCorpse );
		}
		return( TRUE );
	}

	return( FALSE );
}


BOOLEAN CheckForImproperFireGunEnd( SOLDIERTYPE *pSoldier )
{

	if ( AM_A_ROBOT( pSoldier ) )
	{
		return( FALSE );
	}

	OBJECTTYPE* pObjHand = pSoldier->GetUsedWeapon( &pSoldier->inv[ HANDPOS ] );

	// Check single hand for jammed status, ( or ammo is out.. )
	if ( (*pObjHand)[0]->data.gun.bGunAmmoStatus < 0 || (*pObjHand)[0]->data.gun.ubGunShotsLeft == 0 )
	{
		// If we have 2 pistols, donot go back!
		if ( Item[ pSoldier->inv[ SECONDHANDPOS ].usItem ].usItemClass != IC_GUN )
		{
			// OK, put gun down....
			pSoldier->InternalSoldierReadyWeapon(pSoldier->ubDirection, TRUE );
			return( TRUE );
		}
	}

	return( FALSE );

}


BOOLEAN OKHeightDest( SOLDIERTYPE *pSoldier, INT32 sNewGridNo )
{
	if ( pSoldier->pathing.bLevel == 0 )
	{
		return( TRUE );
	}

	// Check if there is a lower place here....
	if ( IsLowerLevel( sNewGridNo ) )
	{
		return( FALSE );
	}

	return( TRUE );
}


BOOLEAN HandleUnjamAnimation( SOLDIERTYPE *pSoldier )
{
	// OK, play intermediate animation here..... save in pending animation data, the current
	// code we are at!
	pSoldier->aiData.uiPendingActionData1 = pSoldier->usAniCode;
	pSoldier->aiData.sPendingActionData2	= pSoldier->usAnimState;
	// Check what animatnion we should do.....
	switch( pSoldier->usAnimState )
	{
	case SHOOT_RIFLE_STAND:
	case STANDING_BURST:
	case FIRE_STAND_BURST_SPREAD:
		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( STANDING_SHOOT_UNJAM, 0 , FALSE );
		return( TRUE );

	case PRONE_BURST:
	case SHOOT_RIFLE_PRONE:

		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( PRONE_SHOOT_UNJAM, 0 , FALSE );
		return( TRUE );

	case CROUCHED_BURST:
	case SHOOT_RIFLE_CROUCH:

		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( CROUCH_SHOOT_UNJAM, 0 , FALSE );
		return( TRUE );

	case SHOOT_DUAL_STAND:

		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( STANDING_SHOOT_DWEL_UNJAM, 0 , FALSE );
		return( TRUE );

	case SHOOT_DUAL_PRONE:

		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( PRONE_SHOOT_DWEL_UNJAM, 0 , FALSE );
		return( TRUE );

	case SHOOT_DUAL_CROUCH:

		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( CROUCH_SHOOT_DWEL_UNJAM, 0 , FALSE );
		return( TRUE );

	case FIRE_LOW_STAND:
	case FIRE_BURST_LOW_STAND:

		// Normal shoot rifle.... play
		pSoldier->ChangeSoldierState( STANDING_SHOOT_LOW_UNJAM, 0 , FALSE );
		return( TRUE );

	}

	return( FALSE );
}




#if 0
//OK, if here, if our health is still good, but we took a lot of damage, try to fall down!
if ( pSoldier->stats.bLife >= OKLIFE )
{
	// Randomly fall back or forward, if we are in the standing hit animation
	if ( pSoldier->usAnimState == GENERIC_HIT_STAND || pSoldier->usAnimState == RIFLE_STAND_HIT )
	{
		INT8			bTestDirection = pSoldier->ubDirection;
		BOOLEAN		fForceDirection = FALSE;
		BOOLEAN		fDoFallback			= FALSE;

		// As the damage pretty brutal?

		// TRY FALLING BACKWARDS, ( ONLY IF WE ARE A MERC! )
		if ( Random( 1000 ) > 40 && IS_MERC_BODY_TYPE( pSoldier ) )
		{
			// CHECK IF WE HAVE AN ATTACKER, TAKE OPPOSITE DIRECTION!
			if ( pSoldier->ubAttackerID != NOBODY )
			{
				// Find direction!
				bTestDirection = (INT8)GetDirectionFromGridNo( MercPtrs[ pSoldier->ubAttackerID ]->sGridNo, pSoldier );
				fForceDirection = TRUE;
			}

			sNewGridNo = NewGridNo( pSoldier->sGridNo, (UINT16)(-1 * DirectionInc( bTestDirection ) ) );

			if ( NewOKDestination( pSoldier, sNewGridNo, TRUE, pSoldier->pathing.bLevel ) && OKHeightDest( pSoldier, sNewGridNo ) )
			{
				// ALL'S OK HERE..... IF WE FORCED DIRECTION, SET!
				if ( fForceDirection )
				{
					pSoldier->EVENT_SetSoldierDirection( bTestDirection );
					pSoldier->EVENT_SetSoldierDesiredDirection( bTestDirection );
				}
				pSoldier->ChangeSoldierState( FALLBACK_HIT_STAND, 0, FALSE );
				return;
			}
		}
	}
}

#endif

BOOLEAN OKFallDirection( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT8 bLevel, UINT8 ubTestDirection, UINT16 usAnimState )
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"OKFallDirection");
	STRUCTURE_FILE_REF *	pStructureFileRef;
	UINT16								usAnimSurface;
	UINT8					bOverTerrainType;

	// WANNE - MP: MP crashed here, so I added the prevention	
	if (TileIsOutOfBounds(sGridNo))
		return ( FALSE );

	// How are the movement costs?
	if ( gubWorldMovementCosts[ sGridNo ][ ubTestDirection ][ bLevel ] > TRAVELCOST_SHORE )
	{
		return( FALSE );
	}

	bOverTerrainType = GetTerrainType( sGridNo);

	// WANNE.WATER: If our soldier is not on the ground level and the tile is a "water" tile, then simply set the tile to "FLAT_GROUND"
	// This should fix "problems" for special modified maps
	if ( TERRAIN_IS_WATER( bOverTerrainType) && bLevel > 0 )
		bOverTerrainType = FLAT_GROUND;

	//NOT ok if in water....
	if ( TERRAIN_IS_WATER( bOverTerrainType) )
	{
		return( FALSE );
	}

	// How are we for OK dest?
	if (!NewOKDestination( pSoldier, sGridNo, TRUE, bLevel ) )
	{
		return( FALSE );
	}

	usAnimSurface = DetermineSoldierAnimationSurface( pSoldier, usAnimState );
	pStructureFileRef = GetAnimationStructureRef( pSoldier->ubID, usAnimSurface, usAnimState );

	if ( pStructureFileRef )
	{
		UINT16		usStructureID;
		INT32			sTestGridNo;

		// must make sure that structure data can be added in the direction of the target

		usStructureID = pSoldier->ubID;

		// Okay this is really SCREWY but it's due to the way this function worked before and must
		// work now.	The function is passing in an adjacent gridno but we need to place the structure
		// data in the tile BEFORE.	So we take one step back in the direction opposite to bTestDirection
		// and use that gridno
		sTestGridNo = NewGridNo( sGridNo, DirectionInc( gOppositeDirection[ ubTestDirection ] ) );

		if ( ! OkayToAddStructureToWorld( sTestGridNo, bLevel, &(pStructureFileRef->pDBStructureRef[ gOneCDirection[ ubTestDirection ] ]), usStructureID ) )
		{
			// can't go in that dir!
			return( FALSE );
		}
	}

	return( TRUE );
}

BOOLEAN HandleCheckForDeathCommonCode( SOLDIERTYPE *pSoldier )
{
	// Do we have a primary pending animation?
	if ( pSoldier->usPendingAnimation2 != NO_PENDING_ANIMATION )
	{
		pSoldier->ChangeSoldierState( pSoldier->usPendingAnimation2, 0, FALSE );
		pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;
		return( TRUE );
	}

	// CHECK IF WE HAVE A PENDING ANIMATION HERE
	if ( pSoldier->usPendingAnimation != NO_PENDING_ANIMATION )
	{
		pSoldier->ChangeSoldierState( pSoldier->usPendingAnimation, 0, FALSE );
		pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
		return( TRUE );
	}

	// OTHERWISE, GOTO APPROPRIATE STOPANIMATION!
	pSoldier->bCollapsed = TRUE;

	// CC has requested - handle sight here...
	HandleSight( pSoldier, SIGHT_LOOK );

	// ATE: If it is our turn, make them try to getup...
	if ( gTacticalStatus.ubCurrentTeam == pSoldier->bTeam )
	{
		// Try to getup...
		pSoldier->BeginSoldierGetup( );

		// Check this to see if above worked
		if ( !pSoldier->bCollapsed )
		{
			return( TRUE );
		}
	}

	switch( pSoldier->usAnimState )
	{
	case FLYBACK_HIT:
		pSoldier->ChangeSoldierState( FLYBACKHIT_STOP, 0, FALSE );
		break;

	case GENERIC_HIT_DEATHTWITCHNB:
	case FALLFORWARD_FROMHIT_STAND:
	case ENDFALLFORWARD_FROMHIT_CROUCH:

		pSoldier->ChangeSoldierState( STAND_FALLFORWARD_STOP, 0, FALSE );
		break;

	case FALLBACK_HIT_DEATHTWITCHNB:
	case FALLBACK_HIT_STAND:
		pSoldier->ChangeSoldierState( FALLBACKHIT_STOP, 0, FALSE );
		break;

	case PRONE_HIT_DEATHTWITCHNB:
	case PRONE_LAY_FROMHIT:

		pSoldier->ChangeSoldierState( PRONE_LAYFROMHIT_STOP, 0, FALSE );
		break;

	case FALLOFF:
		pSoldier->ChangeSoldierState( FALLOFF_STOP, 0, FALSE );
		break;

	case FALLFORWARD_ROOF:
		pSoldier->ChangeSoldierState( FALLOFF_FORWARD_STOP, 0, FALSE);
		break;

	default:
		// IF we are here - something is wrong - we should have a death animation here
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Ani: unconscious hit sequence needed for animation %d", pSoldier->usAnimState ) );

	}
	// OTHERWISE, GOTO APPROPRIATE STOPANIMATION!
	pSoldier->bCollapsed = TRUE;

	// ATE: If it is our turn, make them try to getup...
	if ( gTacticalStatus.ubCurrentTeam == pSoldier->bTeam )
	{
		// Try to getup...
		pSoldier->BeginSoldierGetup( );

		// Check this to see if above worked
		if ( !pSoldier->bCollapsed )
		{
			return( TRUE );
		}
	}

	switch( pSoldier->usAnimState )
	{
	case FLYBACK_HIT:
		pSoldier->ChangeSoldierState( FLYBACKHIT_STOP, 0, FALSE );
		break;

	case GENERIC_HIT_DEATHTWITCHNB:
	case FALLFORWARD_FROMHIT_STAND:
	case ENDFALLFORWARD_FROMHIT_CROUCH:

		pSoldier->ChangeSoldierState( STAND_FALLFORWARD_STOP, 0, FALSE );
		break;

	case FALLBACK_HIT_DEATHTWITCHNB:
	case FALLBACK_HIT_STAND:
		pSoldier->ChangeSoldierState( FALLBACKHIT_STOP, 0, FALSE );
		break;

	case PRONE_HIT_DEATHTWITCHNB:
	case PRONE_LAY_FROMHIT:

		pSoldier->ChangeSoldierState( PRONE_LAYFROMHIT_STOP, 0, FALSE );
		break;

	case FALLOFF:
		pSoldier->ChangeSoldierState( FALLOFF_STOP, 0, FALSE );
		break;

	case FALLFORWARD_ROOF:
		pSoldier->ChangeSoldierState( FALLOFF_FORWARD_STOP, 0, FALSE);
		break;

	default:
		// IF we are here - something is wrong - we should have a death animation here
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Soldier Ani: unconscious hit sequence needed for animation %d", pSoldier->usAnimState ) );

	}

	return( TRUE );
}


void KickOutWheelchair( SOLDIERTYPE *pSoldier )
{
	INT32 sNewGridNo;

	// Move forward one gridno....
	sNewGridNo = NewGridNo( pSoldier->sGridNo, DirectionInc( pSoldier->ubDirection ) );

	// ATE: Make sure that the gridno is unoccupied!
	if ( !NewOKDestination( pSoldier, sNewGridNo, TRUE, pSoldier->pathing.bLevel ) )
	{
		// We should just stay put - will look kind of funny but nothing I can do!
		sNewGridNo = pSoldier->sGridNo;
	}

	pSoldier->EVENT_StopMerc( sNewGridNo, pSoldier->ubDirection );
	pSoldier->ubBodyType = REGMALE;
	if ( pSoldier->ubProfile == SLAY && pSoldier->bTeam == CIV_TEAM && !pSoldier->aiData.bNeutral )
	{
		HandleNPCDoAction( pSoldier->ubProfile, NPC_ACTION_THREATENINGLY_RAISE_GUN, 0 );
	}
	else
	{
		pSoldier->EVENT_InitNewSoldierAnim( STANDING, 0 , TRUE );
	}

	// If this person has a profile ID, set body type to regmale
	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		gMercProfiles[ pSoldier->ubProfile ].ubBodyType = REGMALE;
	}

}
