#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "language defines.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <time.h>
	#include "sgp.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "renderworld.h"
	#include "structure.h"
	#include "Animation Control.h"
	#include "points.h"
	#include "overhead.h"
	#include "tile animation.h"
	#include "Interactive Tiles.h"
	#include "handle doors.h"
	#include "Sound Control.h"
	#include "interface.h"
	#include "keys.h"
	#include "message.h"
	#include "text.h"
	#include "random.h"
	#include "SkillCheck.h"
	#include "Dialogue Control.h"
	#include "strategicmap.h"
	#include "Quests.h"
	#include "Render Fun.h"
	#include "Map Screen Interface Map.h"
	#include "Soldier Profile.h"
	#include "Isometric Utils.h"
	#include "ai.h"
	#include "Soldier macros.h"
	#include "Event Pump.h"
	#include "GameSettings.h"
#endif
#include "fresh_header.h"
#include "connect.h"

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "mercs.h"
#include "ub_config.h"
#endif

BOOLEAN gfSetPerceivedDoorState = FALSE;


BOOLEAN HandleDoorsOpenClose( SOLDIERTYPE *pSoldier, INT32 sGridNo, STRUCTURE * pStructure, BOOLEAN fNoAnimations );

#ifdef JA2UB
void HandleForceingTheTunnelGate( UINT32 sGridNo ); //Ja25 UB
#endif

void HandleDoorChangeFromGridNo( SOLDIERTYPE *pSoldier, INT32 sGridNo, BOOLEAN fNoAnimations )
{
	STRUCTURE *			pStructure;
	DOOR_STATUS *		pDoorStatus;
	BOOLEAN					fDoorsAnimated = FALSE;

	pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );

	if ( pStructure == NULL )
	{
#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"ERROR: Told to handle door that does not exist at %d.", sGridNo );
#endif
		return;
	}

	fDoorsAnimated = HandleDoorsOpenClose( pSoldier, sGridNo, pStructure, fNoAnimations );
	if ( SwapStructureForPartner( sGridNo, pStructure ) != NULL)
	{
		RecompileLocalMovementCosts( sGridNo );
	}


	// set door busy
	pDoorStatus = GetDoorStatus( sGridNo );
	if ( pDoorStatus == NULL )
	{
#ifdef JA2TESTVERSION
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"ERROR: Told to set door busy but can't get door status at %d!", sGridNo );
#endif
		return;
	}

	// ATE: Only do if animated.....
	if ( fDoorsAnimated )
	{
		pDoorStatus->ubFlags |= DOOR_BUSY;
	}

	// Flugente: we have to redo flashlights
	pSoldier->HandleFlashLights();
}

UINT16 GetAnimStateForInteraction( SOLDIERTYPE *pSoldier, BOOLEAN fDoor, UINT16 usAnimState )
{
	switch( usAnimState )
	{
		case OPEN_DOOR:

		if ( pSoldier->ubBodyType == CRIPPLECIV )
		{
		return( CRIPPLE_OPEN_DOOR );
		}
		else
		{
			if ( fDoor )
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( OPEN_DOOR_CROUCHED );
				}
				else
				{
					return( usAnimState );
				}
			}
			else
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( BEGIN_OPENSTRUCT_CROUCHED );
				}
				else
				{
					return( BEGIN_OPENSTRUCT );
				}
			}
		}
			break;

		case CLOSE_DOOR:

		if ( pSoldier->ubBodyType == CRIPPLECIV )
		{
		return( CRIPPLE_CLOSE_DOOR );
		}
		else
		{
			if ( fDoor )
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( CLOSE_DOOR_CROUCHED );
				}
				else
				{
					return( usAnimState );
				}
			}
			else
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( OPEN_STRUCT_CROUCHED );
				}
				else
				{
					return( OPEN_STRUCT );
				}
			}
		}
			break;

		case END_OPEN_DOOR:

		if ( pSoldier->ubBodyType == CRIPPLECIV )
		{
		return( CRIPPLE_END_OPEN_DOOR );
		}
		else
		{
			if ( fDoor )
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( END_OPEN_DOOR_CROUCHED );
				}
				else
				{
					return( usAnimState );
				}
			}
			else
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( END_OPENSTRUCT_CROUCHED );
				}
				else
				{
					return( END_OPENSTRUCT );
				}
			}
		}
			break;


		case END_OPEN_LOCKED_DOOR:

		if ( pSoldier->ubBodyType == CRIPPLECIV )
		{
		return( CRIPPLE_END_OPEN_LOCKED_DOOR );
		}
		else
		{
			if ( fDoor )
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( END_OPEN_LOCKED_DOOR_CROUCHED );
				}
				else
				{
					return( END_OPEN_LOCKED_DOOR );
				}
			}
			else
			{
				if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
				{
					return( END_OPENSTRUCT_LOCKED_CROUCHED );
				}
				else
				{
					return( END_OPENSTRUCT_LOCKED );
				}
			}
		}
			break;

		case PICK_LOCK:

			if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
			{
		return( LOCKPICK_CROUCHED );
		}
		else
		{
		return( PICK_LOCK );
		}
		break;

		default:
			// should never happen!
			Assert( FALSE );
			return( usAnimState );
			break;
	}
}


void InteractWithClosedDoor( SOLDIERTYPE *pSoldier, UINT8 ubHandleCode )
{
	pSoldier->aiData.ubDoorHandleCode = ubHandleCode;

	switch( ubHandleCode )
	{
		case HANDLE_DOOR_OPEN:
		case HANDLE_DOOR_UNLOCK:
		case HANDLE_DOOR_EXAMINE:
		case HANDLE_DOOR_EXPLODE:
		case HANDLE_DOOR_LOCK:
		case HANDLE_DOOR_UNTRAP:
		case HANDLE_DOOR_CROWBAR:

			pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, TRUE, OPEN_DOOR ), 0, FALSE );
			break;

		case HANDLE_DOOR_FORCE:

			pSoldier->ChangeSoldierState( KICK_DOOR, 0 , FALSE );
			break;

		case HANDLE_DOOR_LOCKPICK:

			pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, TRUE, PICK_LOCK ), 0 , FALSE );
			break;
	}

}

BOOLEAN DoTrapCheckOnStartingMenu( SOLDIERTYPE * pSoldier, DOOR * pDoor )
{
	INT16	bDetectLevel;

	if ( pDoor && pDoor->fLocked && pDoor->ubTrapID != NO_TRAP && pDoor->bPerceivedTrapped == DOOR_PERCEIVED_UNKNOWN )
	{
		// check for noticing the trap
		bDetectLevel = CalcTrapDetectLevel( pSoldier, FALSE );
		if (bDetectLevel >= pDoor->ubTrapLevel)
		{
			// say quote, update status
			TacticalCharacterDialogue( pSoldier, QUOTE_BOOBYTRAP_ITEM );
			UpdateDoorPerceivedValue( pDoor );

		return( TRUE );
		}
	}

	return( FALSE );
}

void InteractWithOpenableStruct( SOLDIERTYPE *pSoldier, STRUCTURE *pStructure, UINT8 ubDirection, BOOLEAN fDoor )
{
	STRUCTURE *			pBaseStructure;
	BOOLEAN					fDoMenu = FALSE;
	DOOR		*				pDoor = NULL;
	DOOR_STATUS *		pDoorStatus;
	BOOLEAN		 fTrapsFound = FALSE;

	pBaseStructure = FindBaseStructure( pStructure );

	if ( fDoor )
	{
		// get door status, if busy then just return!
		pDoorStatus = GetDoorStatus( pBaseStructure->sGridNo );
		if ( pDoorStatus && (pDoorStatus->ubFlags & DOOR_BUSY) )
		{
			// Send this guy into stationary stance....
			pSoldier->EVENT_StopMerc( pSoldier->sGridNo, pSoldier->ubDirection );

			if ( pSoldier->bTeam == gbPlayerNum )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ DOOR_IS_BUSY ] );
			}
		else
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("Trying to open door and door is busy: %d", pSoldier->ubID ) );
		}
			return;
		}
	}

	pSoldier->EVENT_SetSoldierDesiredDirection( ubDirection );

	// Is the door opened?
	if ( pStructure->fFlags & STRUCTURE_OPEN )
	{
		if ( pSoldier->ubID <= gTacticalStatus.Team[ gbPlayerNum ].bLastID && !( pStructure->fFlags & STRUCTURE_SWITCH ) )
		{
			// Bring up menu to decide what to do....
			pSoldier->SoldierGotoStationaryStance( );

			pDoor = FindDoorInfoAtGridNo( pBaseStructure->sGridNo );
			if ( pDoor )
			{
				if ( DoTrapCheckOnStartingMenu( pSoldier, pDoor ) )
				{
					fTrapsFound = TRUE;
				}
			}

			// Pull Up Menu
			if ( !fTrapsFound )
			{
				// HEADROCK HAM 4: Why not just close the object? Why do we need a menu if there's only one option on it (CLOSE)?
				// P.S. I don't think it's even possible for opened structures to be trapped to begin with.
				//InitDoorOpenMenu( pSoldier, pStructure, ubDirection, TRUE );
				pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, CLOSE_DOOR ), 0, FALSE );
			}
		}
		else
		{
			// Easily close door....
			pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, CLOSE_DOOR ), 0, FALSE );
		}
	}
	else
	{
		// Bring up the menu, only if it has a lock!
		if ( pSoldier->ubID <= gTacticalStatus.Team[ gbPlayerNum ].bLastID )
		{
			pDoor = FindDoorInfoAtGridNo( pBaseStructure->sGridNo );

			if ( pDoor != NULL )
			{
		// Assume true
				fDoMenu = TRUE;

		// Check if it's locked.....
		// If not locked, don't bring it up!
		if ( !pDoor->fLocked )
		{
			fDoMenu = FALSE;
		}
			}
		}

		if ( fDoMenu )
		{
			// Bring up menu to decide what to do....
			pSoldier->SoldierGotoStationaryStance( );

			if ( DoTrapCheckOnStartingMenu( pSoldier, pDoor ) )
		{
		fTrapsFound = TRUE;
		}

			// Pull Up Menu
		if ( !fTrapsFound )
		{
		  // In autobandage mode, assuming no traps are found, try to automatically unlock the door.  If we can't, then we can't
		  if (gTacticalStatus.fAutoBandageMode)
		  {
			  AttemptToUnlockDoor( pSoldier, pDoor);
			  return;
		  }

			InitDoorOpenMenu( pSoldier, pStructure, ubDirection, FALSE );
		}
		else
		{
			UnSetUIBusy( pSoldier->ubID );
		}
		}
		else
		{
			pSoldier->aiData.ubDoorHandleCode = HANDLE_DOOR_OPEN;

			pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, OPEN_DOOR ), 0, FALSE );
		}
	}

}

void ProcessImplicationsOfPCMessingWithDoor( SOLDIERTYPE * pSoldier )
{
	//DBrot: More Rooms
	//UINT8						ubRoom;
	UINT16	usRoom;
	SOLDIERTYPE *		pGoon;
	// if player is hacking at a door in the brothel and a kingpin guy can see him
	if ( (InARoom( pSoldier->sGridNo, &usRoom ) && IN_BROTHEL( usRoom )) || (gWorldSectorX == 5 && gWorldSectorY == MAP_ROW_D && gbWorldSectorZ == 0 && (pSoldier->sGridNo == gModSettings.iBrothelDoor1 || pSoldier->sGridNo == gModSettings.iBrothelDoor2 || pSoldier->sGridNo == gModSettings.iBrothelDoor3 ) ) )//11010,11177,11176
	{
		UINT8		ubLoop;

		// see if a kingpin goon can see us
		for ( ubLoop = gTacticalStatus.Team[ CIV_TEAM ].bFirstID; ubLoop <= gTacticalStatus.Team[ CIV_TEAM ].bLastID; ubLoop++ )
		{
			pGoon = MercPtrs[ ubLoop ];
			if ( pGoon->ubCivilianGroup == KINGPIN_CIV_GROUP && pGoon->bActive && pGoon->bInSector && pGoon->stats.bLife >= OKLIFE && pGoon->aiData.bOppList[ pSoldier->ubID ] == SEEN_CURRENTLY )
			{
				MakeCivHostile( pGoon, 2 );
				if ( ! (gTacticalStatus.uiFlags & INCOMBAT) )
				{
					EnterCombatMode( pGoon->bTeam );
				}
			}
		}
	}

	if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y )
	{
		pGoon = FindSoldierByProfileID( WARDEN, FALSE );
		if ( pGoon && pGoon->aiData.bAlertStatus < STATUS_RED && PythSpacesAway( pSoldier->sGridNo, pGoon->sGridNo ) <= 5 )
		{
			// alert her if she hasn't been alerted
			pGoon->aiData.bAlertStatus = STATUS_RED;
			CheckForChangingOrders( pGoon );
			CancelAIAction( pGoon, TRUE );
		}
	}
}



BOOLEAN HandleOpenableStruct( SOLDIERTYPE *pSoldier, INT32 sGridNo, STRUCTURE *pStructure )
{
	BOOLEAN fHandleDoor = FALSE;
	INT16		sAPCost = 0, sBPCost = 0;
	DOOR		*pDoor;
	BOOLEAN	fTrapFound = FALSE;
	BOOLEAN	fDoAction = TRUE;
	BOOLEAN	fDoor	 = FALSE;

	// Are we a door?
	if (pStructure->fFlags & STRUCTURE_ANYDOOR)
	{
		fDoor = TRUE;
	}

	// Calculate basic points...

	// We'll add any aps for things like lockpicking, booting, etc

	// If we are already open....no need for lockpick checks, etc
	if ( pStructure->fFlags & STRUCTURE_OPEN )
	{
		// Set costs for these
		sAPCost = GetAPsToOpenDoor( pSoldier ); // SANDRO
		sBPCost = APBPConstants[BP_OPEN_DOOR];

		fHandleDoor = TRUE;
	}
	else
	{
		if ( pSoldier->bTeam == OUR_TEAM )
		{
			// Find locked door here....
			pDoor = FindDoorInfoAtGridNo( sGridNo );

			// Alrighty, first check for traps ( unless we are examining.... )
			if ( pSoldier->aiData.ubDoorHandleCode != HANDLE_DOOR_EXAMINE && pSoldier->aiData.ubDoorHandleCode != HANDLE_DOOR_UNTRAP && pSoldier->aiData.ubDoorHandleCode != HANDLE_DOOR_UNLOCK )
			{
				if ( pDoor != NULL )
				{
					// Do we have a trap? NB if door is unlocked disable all traps
					if (pDoor->fLocked && pDoor->ubTrapID != NO_TRAP)
					{

						fTrapFound = TRUE;

						// Set costs for these
						// Set AP costs to that of opening a door
						sAPCost = GetAPsToOpenDoor( pSoldier ); // SANDRO
						sBPCost = APBPConstants[BP_OPEN_DOOR];

						pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );

						// Did we inadvertently set it off?
						if ( HasDoorTrapGoneOff( pSoldier, pDoor ) )
						{
							// Kaboom
							// Code to handle trap here...
							HandleDoorTrap( pSoldier, pDoor );
							if (DoorTrapTable[pDoor->ubTrapID].fFlags & DOOR_TRAP_STOPS_ACTION)
							{
								// trap stops person from opening door!
								fDoAction	= FALSE;
							}
							if (!( DoorTrapTable[pDoor->ubTrapID].fFlags & DOOR_TRAP_RECURRING ) )
							{
								// trap only happens once
								pDoor->ubTrapLevel = 0;
								pDoor->ubTrapID = NO_TRAP;
							}
							UpdateDoorPerceivedValue( pDoor );
						}
						else
						{
							// If we didn't set it off then we must have noticed it or know about it already

							// do we know it's trapped?
							if ( pDoor->bPerceivedTrapped == DOOR_PERCEIVED_UNKNOWN )
							{
								switch( pDoor->ubTrapID )
								{
									case BROTHEL_SIREN:
										ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESCRIPTION_STR ], pDoorTrapStrings[ SIREN ] );
										break;
									case SUPER_ELECTRIC:
										ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESCRIPTION_STR ], pDoorTrapStrings[ ELECTRIC ] );
										break;
									default:
										ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESCRIPTION_STR ], pDoorTrapStrings[ pDoor->ubTrapID ] );
										break;
								}

								// Stop action this time....
								fDoAction	= FALSE;

								// report!
								TacticalCharacterDialogue( pSoldier, QUOTE_BOOBYTRAP_ITEM );
							}
							else
							{
								// Set it off!
								HandleDoorTrap( pSoldier, pDoor );
								if (DoorTrapTable[pDoor->ubTrapID].fFlags & DOOR_TRAP_STOPS_ACTION)
								{
									// trap stops person from opening door!
									fDoAction	= FALSE;
								}
								if (!( DoorTrapTable[pDoor->ubTrapID].fFlags & DOOR_TRAP_RECURRING ) )
								{
									// trap only happens once
									pDoor->ubTrapLevel = 0;
									pDoor->ubTrapID = NO_TRAP;
								}
							}
							UpdateDoorPerceivedValue( pDoor );

						}
					}
				}
			}

			if ( fDoAction )
			{
				// OK, switch based on how we are going to open door....
				switch( pSoldier->aiData.ubDoorHandleCode )
				{
					case HANDLE_DOOR_OPEN:

						// If we have no lock on door...
						if ( pDoor == NULL )
						{
							// Set costs for these
							sAPCost = GetAPsToOpenDoor( pSoldier ); // SANDRO
							sBPCost = APBPConstants[BP_OPEN_DOOR];

							// Open if it's not locked....
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							fHandleDoor = TRUE;
							break;
						}
						else
						{
							if ( pDoor->fLocked )
							{
								// it's locked....
								pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_LOCKED_DOOR ), 0, FALSE );
#ifdef JA2UB
								//JA25 UB
								//If this is the tunnel sector, and the merc failed opening the fence door, play a quote
								HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( pSoldier, FALSE );
#endif								
								
								// Do we have a quote for locked stuff?
								// Now just show on message bar
								if ( !AM_AN_EPC( pSoldier ) )
								{
									pSoldier->DoMercBattleSound( BATTLE_SOUND_LOCKED );
								}
								else
								{
									ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_HAS_BEEN_LOCKED_STR ] );
								}
							}
							else
							{
								pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
								fHandleDoor = TRUE;
							}
							UpdateDoorPerceivedValue( pDoor );
							break;
						}
						break;
#ifdef JA2UB
					case HANDLE_DOOR_FORCE:

						// Set costs for these
						sAPCost = GetAPsToOpenDoor( pSoldier ); // SANDRO
						sBPCost = APBPConstants[BP_BOOT_DOOR];

						// OK, using force, if we have no lock, just open the door!
						if ( pDoor == NULL )
						{
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							fHandleDoor = TRUE;

							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
						}
						else
						{
							//JA25 UB
							//If this is the tunnel sector, and the merc failed opening the fence door, play a quote
							if( HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( pSoldier, TRUE ) )
							{
							}
							else
							{
								// Attempt to force door
								if ( AttemptToSmashDoor( pSoldier, pDoor ) )
								{
									//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESTROYED_STR ] );
									// pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
									fHandleDoor = TRUE;
								}
								else
								{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_NOT_DESTROYED_STR ] );
									UpdateDoorPerceivedValue( pDoor );
								}
								ProcessImplicationsOfPCMessingWithDoor( pSoldier );
							}
						}
						//Handle Special code for the gate in the tunnel
						HandleForceingTheTunnelGate( sGridNo ); //JA25 UB
						break;
#else
					case HANDLE_DOOR_FORCE:

						// Set costs for these
						sAPCost = APBPConstants[AP_BOOT_DOOR];
						sBPCost = APBPConstants[BP_BOOT_DOOR];

						// OK, using force, if we have no lock, just open the door!
						if ( pDoor == NULL )
						{
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							fHandleDoor = TRUE;

							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
						}
						else
						{
							// Attempt to force door
							if ( AttemptToSmashDoor( pSoldier, pDoor ) )
							{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESTROYED_STR ] );
								// pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
								fHandleDoor = TRUE;
							}
							else
							{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_NOT_DESTROYED_STR ] );
								UpdateDoorPerceivedValue( pDoor );
							}
							ProcessImplicationsOfPCMessingWithDoor( pSoldier );
						}
						break;
#endif

					case HANDLE_DOOR_CROWBAR:

						// Set costs for these
						sAPCost = APBPConstants[AP_USE_CROWBAR];
						sBPCost = APBPConstants[BP_USE_CROWBAR];

						// OK, using force, if we have no lock, just open the door!
						if ( pDoor == NULL )
						{
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							fHandleDoor = TRUE;

							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
						}
						else
						{
							// Attempt to force door
							if ( AttemptToCrowbarLock( pSoldier, pDoor ) )
							{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESTROYED_STR ] );
								//pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
								fHandleDoor = TRUE;
							}
							else
							{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_NOT_DESTROYED_STR ] );
								UpdateDoorPerceivedValue( pDoor );
							}

							ProcessImplicationsOfPCMessingWithDoor( pSoldier );
						}
						break;

					case HANDLE_DOOR_EXPLODE:

						// Set costs for these
						sAPCost = GetAPsToBombDoor( pSoldier ); // SANDRO
						sBPCost = APBPConstants[BP_EXPLODE_DOOR];

						if ( pDoor == NULL )
						{
							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
						}
						else
						{
							// Attempt to force door
							if ( AttemptToBlowUpLock( pSoldier, pDoor ) )
							{
								//pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESTROYED_STR ] );
								fHandleDoor = TRUE;
							}
							else
							{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_NOT_DESTROYED_STR ] );
								UpdateDoorPerceivedValue( pDoor );
							}
							ProcessImplicationsOfPCMessingWithDoor( pSoldier );
						}
						break;

					case HANDLE_DOOR_LOCKPICK:

						// Set costs for these
						sAPCost = GetAPsToPicklock( pSoldier ); // SANDRO
						sBPCost = APBPConstants[BP_PICKLOCK];

						// Attempt to pick lock
						if ( pDoor == NULL )
						{
							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
						}
						else
						{
							if ( AttemptToPickLock( pSoldier, pDoor ) )
							{
								pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_HAS_BEEN_PICKED_STR ] );
								fHandleDoor = TRUE;
							}
							else
							{
#ifdef JA2UB
								//If this is the tunnel sector, and the merc failed opening the fence door, play a quote
								HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( pSoldier, FALSE );

#endif								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_HAS_NOT_BEEN_PICKED_STR ] );
							}
							ProcessImplicationsOfPCMessingWithDoor( pSoldier );
						}
						break;


					case HANDLE_DOOR_EXAMINE:

						// Set costs for these
						sAPCost = APBPConstants[AP_EXAMINE_DOOR];
						sBPCost = APBPConstants[BP_EXAMINE_DOOR];

						// Attempt to examine door
						// Whatever the result, end the open animation
						pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );

						if ( pDoor == NULL )
						{
							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
						}
						else
						{
#ifdef JA2UB						
							//If this is the tunnel sector, and the merc failed opening the fence door, play a quote
							HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( pSoldier, FALSE ); //JA25 UB
#endif
							if ( ExamineDoorForTraps( pSoldier, pDoor ) )
							{
								// We have a trap. Use door pointer to determine what type, etc
								TacticalCharacterDialogue( pSoldier, QUOTE_BOOBYTRAP_ITEM );
								switch( pDoor->ubTrapID )
								{
									case BROTHEL_SIREN:
										ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESCRIPTION_STR ], pDoorTrapStrings[ SIREN ] );
										break;
									case SUPER_ELECTRIC:
										ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESCRIPTION_STR ], pDoorTrapStrings[ ELECTRIC ] );
										break;
									default:
										ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_DESCRIPTION_STR ], pDoorTrapStrings[ pDoor->ubTrapID ] );
										break;
								}

								UpdateDoorPerceivedValue( pDoor );
							}
							else
							{
								ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_UNTRAPPED_STR ] );
							}
						}
						break;


					case HANDLE_DOOR_UNLOCK:

						// Set costs for these
						sAPCost = APBPConstants[AP_UNLOCK_DOOR];
						sBPCost = APBPConstants[BP_UNLOCK_DOOR];

						// OK, if we have no lock, show that!
						if ( pDoor == NULL )
						{
							// Open if it's not locked....
							//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							break;
						}
						else
						{

							// it's locked....
							// Attempt to unlock....
							if ( AttemptToUnlockDoor( pSoldier, pDoor ) )
							{
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_HAS_BEEN_UNLOCKED_STR ] );
								//pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );

								pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
								UpdateDoorPerceivedValue( pDoor );

								fHandleDoor = TRUE;
							}
							else
							{
								pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_LOCKED_DOOR ), 0, FALSE );
								// Do we have a quote for locked stuff?
								// Now just show on message bar
								//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_NOT_PROPER_KEY_STR ], pSoldier->GetName() );

#ifdef JA2UB								
								//If this is the tunnel sector, and the merc failed opening the fence door, play a quote
								if( HandlePlayerSayingQuoteWhenFailingToOpenGateInTunnel( pSoldier, FALSE ) ) //JA25 UB
								{
									// OK PLay damn battle sound
									if ( Random( 2 ) )
									{
										pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
									}
								}
#else
								// OK PLay damn battle sound
								if ( Random( 2 ) )
								{
									pSoldier->DoMercBattleSound( BATTLE_SOUND_CURSE1 );
								}
#endif
							}
						}
						break;


					case HANDLE_DOOR_UNTRAP:

						// Set costs for these
						sAPCost = GetAPsToUntrapDoor( pSoldier ); // SANDRO
						sBPCost = APBPConstants[BP_UNTRAP_DOOR];

						// OK, if we have no lock, show that!
						if ( pDoor == NULL )
						{
							// Open if it's not locked....
							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							break;
						}
						else
						{
							// Do we have a trap?
							if (pDoor->ubTrapID != NO_TRAP)
							{
								fTrapFound = TRUE;
							}

							if ( fTrapFound )
							{
								if ( AttemptToUntrapDoor( pSoldier, pDoor ) )
								{
									//ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_HAS_BEEN_UNTRAPPED_STR ] );
									pSoldier->DoMercBattleSound( BATTLE_SOUND_COOL1 );
									pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
									UpdateDoorPerceivedValue( pDoor );
									//fHandleDoor = TRUE;
								}
								else
								{
									pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_LOCKED_DOOR ), 0, FALSE );
									// Now just show on message bar
									HandleDoorTrap( pSoldier, pDoor );

									if (!( DoorTrapTable[pDoor->ubTrapID].fFlags & DOOR_TRAP_RECURRING ) )
									{
										// trap only happens once
										pDoor->ubTrapLevel = 0;
										pDoor->ubTrapID = NO_TRAP;
									}

									// Update perceived lock value
									UpdateDoorPerceivedValue( pDoor );
								}
							}
							else
							{
								ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_IS_NOT_TRAPPED_STR ] );
							}
						}
						break;


					case HANDLE_DOOR_LOCK:

						// Set costs for these
						sAPCost = APBPConstants[AP_LOCK_DOOR];
						sBPCost = APBPConstants[BP_LOCK_DOOR];

						// OK, if we have no lock, show that!
						if ( pDoor == NULL )
						{
							// Open if it's not locked....
							ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_THERE_IS_NO_LOCK_STR ] );
							pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
							break;
						}
						else
						{

							// it's locked....
							// Attempt to unlock....
							if ( AttemptToLockDoor( pSoldier, pDoor ) )
							{
								ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_LOCK_HAS_BEEN_LOCKED_STR ] );
								pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
								UpdateDoorPerceivedValue( pDoor );
							}
							else
							{
								pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_LOCKED_DOOR ), 0, FALSE );
								// Do we have a quote for locked stuff?
								// Now just show on message bar
								ScreenMsg( MSG_FONT_YELLOW, MSG_INTERFACE, TacticalStr[ DOOR_NOT_PROPER_KEY_STR ], pSoldier->GetName() );

								// Update perceived lock value
								UpdateDoorPerceivedValue( pDoor );
							}
						}
						break;
				}
			}
		}
		else
		{
			// Set costs for these
			// SANDRO - changed this
			sAPCost = GetAPsToOpenDoor( pSoldier );
			sBPCost = APBPConstants[BP_OPEN_DOOR];

			// Open if it's not locked....
			pSoldier->ChangeSoldierState( GetAnimStateForInteraction( pSoldier, fDoor, END_OPEN_DOOR ), 0, FALSE );
			fHandleDoor = TRUE;
		}
	}

	if ( fHandleDoor )
	{
		if ( fDoor )
		{
			if (is_networked)
			{
				// If we are the server, always handle the doors
				if (is_server)
					HandleDoorChangeFromGridNo( pSoldier, sGridNo, FALSE );
				// if we are the pure client, only handle doors for MERC (and not for enemies)!!
				// The reason for this is, because the method "receive_door" (client.cpp) handles the "HandleDoorChangeFromGridNo()"
				else
				{
					if (pSoldier->ubID <= 19)
						HandleDoorChangeFromGridNo( pSoldier, sGridNo, FALSE );
				}
				
				// WANNE - MP: The "receive_door" method only gets called for the pure clients and not for the server!
				// WANNE - MP: General Info!
				// If pure client calls "send_door" also the method "receive_door" (client.cpp) gets called
				// If server calls "send_door", the method "receive_door" on the server doest not get called.
				if(is_client)
					send_door( pSoldier, sGridNo, FALSE );
			}
			else
			{
				HandleDoorChangeFromGridNo( pSoldier, sGridNo, FALSE );
			}
		}
		else
		{
			HandleStructChangeFromGridNo( pSoldier, sGridNo );
		}
	}

	// Deduct points!
	//if ( fDoor )
	{
		DeductPoints( pSoldier, sAPCost, sBPCost );
	}

	return( fHandleDoor );
}


BOOLEAN HandleDoorsOpenClose( SOLDIERTYPE *pSoldier, INT32 sGridNo, STRUCTURE * pStructure, BOOLEAN fNoAnimations  )
{
	LEVELNODE	* pShadowNode;
	LEVELNODE * pNode;
	INT32				cnt;
	BOOLEAN					fOpenedGraphic = FALSE;
	ANITILE_PARAMS	AniParams;
	BOOLEAN					fDoAnimation = TRUE;
	STRUCTURE *	pBaseStructure;
	UINT32			uiSoundID;

	pBaseStructure = FindBaseStructure( pStructure );
	if (!pBaseStructure)
	{
		return( FALSE );
	}

	pNode = FindLevelNodeBasedOnStructure( pBaseStructure->sGridNo, pBaseStructure );
	if (!pNode)
	{
		return( FALSE );
	}

	// ATE: if we are about to swap, but have an animation playing here..... stop the animation....
	if ( ( pNode->uiFlags & LEVELNODE_ANIMATION ) )
	{
		if ( pNode->pAniTile != NULL )
		{
			if ( pNode->pAniTile->uiFlags & ANITILE_DOOR )
			{
				// ATE: No two doors can exist ( there can be only one )
				// Update value.. ie: prematurely end door animation
				// Update current frame...

				if ( pNode->pAniTile->uiFlags & ANITILE_FORWARD )
				{
					pNode->sCurrentFrame = pNode->pAniTile->sStartFrame + pNode->pAniTile->usNumFrames;
				}

				if ( pNode->pAniTile->uiFlags & ANITILE_BACKWARD )
				{
					pNode->sCurrentFrame = pNode->pAniTile->sStartFrame - pNode->pAniTile->usNumFrames;
				}

				pNode->sCurrentFrame = pNode->pAniTile->usNumFrames - 1;

				// Delete...
				DeleteAniTile( pNode->pAniTile );

				pNode->uiFlags &= ~( LEVELNODE_LASTDYNAMIC | LEVELNODE_UPDATESAVEBUFFERONCE );

				if ( GridNoOnScreen( pBaseStructure->sGridNo ) )
				{
					SetRenderFlags(RENDER_FLAG_FULL);
				}
			}
		}
	}


	pShadowNode = gpWorldLevelData[ sGridNo ].pShadowHead;


	// Check the graphic which is down!
	// Check for Open Door!
	cnt = 0;
	while( gOpenDoorList[ cnt ] != -1 )
	{
		// IF WE ARE A SHADOW TYPE
		if ( pNode->usIndex == gOpenDoorList[ cnt ] )
		{
			fOpenedGraphic = TRUE;
			break;
		}
		cnt++;
	};


	if ( !(pStructure->fFlags & STRUCTURE_OPEN) )
	{
		//ATE, the last parameter is the perceived value, I dont know what it is so could you please add the value?
		//ModifyDoorStatus( INT16 sGridNo, BOOLEAN fOpen, BOOLEAN fPercievedOpen )
		if ( gfSetPerceivedDoorState )
		{
			ModifyDoorStatus( sGridNo, TRUE, TRUE );
		}
		else
		{
			ModifyDoorStatus( sGridNo, TRUE, DONTSETDOORSTATUS );
		}

		if ( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_I )
		{
			DoPOWPathChecks();
		}

		if (pSoldier )
		{
			// OK, Are we a player merc or AI?
			if ( pSoldier->bTeam != gbPlayerNum )
			{
				// If an AI guy... do LOS check first....
				// If guy is visible... OR fading...
				if ( pSoldier->bVisible == -1 && !AllMercsLookForDoor( sGridNo, FALSE ) && !( gTacticalStatus.uiFlags&SHOW_ALL_MERCS ) )
				{
					fDoAnimation = FALSE;
				}
			}
		}
		else
		{
			// door opening by action item... just do a LOS check
			if ( !AllMercsLookForDoor( sGridNo, FALSE ) )
			{
				fDoAnimation = FALSE;
			}
		}

		if ( fNoAnimations )
		{
			fDoAnimation = FALSE;
		}

		if ( fDoAnimation	)
		{
			// Update perceived value
			ModifyDoorStatus( sGridNo, DONTSETDOORSTATUS, TRUE );

			if ( fOpenedGraphic )
			{
				memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
				AniParams.sGridNo							= sGridNo;
				AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
				AniParams.usTileType				= (UINT16)gTileDatabase[ pNode->usIndex ].fType;
				AniParams.usTileIndex					= pNode->usIndex;
				AniParams.sDelay							= INTTILE_DOOR_OPENSPEED;
				AniParams.sStartFrame					= pNode->sCurrentFrame;
				AniParams.uiFlags							= ANITILE_DOOR | ANITILE_FORWARD | ANITILE_EXISTINGTILE;
				AniParams.pGivenLevelNode			= pNode;

				CreateAnimationTile( &AniParams );
			}
			else
			{
				memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );
				AniParams.sGridNo							= sGridNo;
				AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
				AniParams.usTileType				= (UINT16)gTileDatabase[ pNode->usIndex ].fType;
				AniParams.usTileIndex					= pNode->usIndex;
				AniParams.sDelay							= INTTILE_DOOR_OPENSPEED;
				AniParams.sStartFrame					= pNode->sCurrentFrame;
				AniParams.uiFlags							= ANITILE_DOOR | ANITILE_BACKWARD | ANITILE_EXISTINGTILE;
				AniParams.pGivenLevelNode			= pNode;

				CreateAnimationTile( &AniParams );
			}
		}

		// SHADOW STUFF HERE
		//if ( pShadowNode != NULL )
		//{
		//	pShadowNode->uiFlags |= LEVELNODE_ANIMATION;
		//	pShadowNode->uiFlags |= LEVELNODE_ANIMATION_PLAYONCE;
		//	pShadowNode->uiFlags |= LEVELNODE_ANIMATION_FORWARD;
		//	if ( pShadowNode->uiFlags & LEVELNODE_ANIMATION_BACKWARD )
		//		pShadowNode->uiFlags ^= LEVELNODE_ANIMATION_BACKWARD;
		//	pShadowNode->sDelay		= INTTILE_DOOR_OPENSPEED;
		//}

		if ( fDoAnimation && pSoldier && pSoldier->ubDoorOpeningNoise)
		{
			// ATE; Default to normal door...
			uiSoundID = ( DROPEN_1 + Random( 3 ) );

			// OK, check if this door is sliding and is multi-tiled...
			if ( pStructure->fFlags & STRUCTURE_SLIDINGDOOR )
			{
				// Get database value...
				if ( pStructure->pDBStructureRef->pDBStructure->ubNumberOfTiles > 1 )
				{
					// change sound ID
					uiSoundID = GARAGE_DOOR_OPEN;
				}
			else if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_CLOTH )
			{
				// change sound ID
				uiSoundID = CURTAINS_OPEN;
			}
		}
		else if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_LIGHT_METAL ||
			pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_THICKER_METAL ||
			pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_HEAVY_METAL )
			{
				// change sound ID
				uiSoundID = METAL_DOOR_OPEN;
			}

			// OK, We must know what sound to play, for now use same sound for all doors...
			PlayJA2Sample( uiSoundID, RATE_11025, SoundVolume( MIDVOLUME, sGridNo ), 1, SoundDir( sGridNo ) );
		}

	}
	else
	{
		//ATE, the last parameter is the perceived value, I dont know what it is so could you please add the value?
		//ModifyDoorStatus( INT16 sGridNo, BOOLEAN fOpen, BOOLEAN fInitiallyPercieveOpen )

		if ( gfSetPerceivedDoorState )
		{
			ModifyDoorStatus( sGridNo, FALSE, FALSE );
		}
		else
		{
			ModifyDoorStatus( sGridNo, FALSE, DONTSETDOORSTATUS );
		}

		if (pSoldier )
		{
			// OK, Are we a player merc or AI?
			if ( pSoldier->bTeam != gbPlayerNum )
			{
				// If an AI guy... do LOS check first....
				// If guy is visible... OR fading...
				if ( pSoldier->bVisible == -1 && !AllMercsLookForDoor( sGridNo, FALSE ) && !( gTacticalStatus.uiFlags&SHOW_ALL_MERCS ) )
				{
					fDoAnimation = FALSE;
				}
			}
		}
		else
		{
			// door opening by action item... just do a LOS check
			if ( !AllMercsLookForDoor( sGridNo, FALSE ) )
			{
				fDoAnimation = FALSE;
			}
		}

		if ( fNoAnimations )
		{
			fDoAnimation = FALSE;
		}

		if ( fDoAnimation )
		{
			// Update perceived value
			ModifyDoorStatus( sGridNo, DONTSETDOORSTATUS, FALSE );

			memset( &AniParams, 0, sizeof( ANITILE_PARAMS ) );

			// ATE; Default to normal door...
			uiSoundID = ( DRCLOSE_1 + Random( 2 ) );

			// OK, check if this door is sliding and is multi-tiled...
			if ( pStructure->fFlags & STRUCTURE_SLIDINGDOOR )
			{
				// Get database value...
				if ( pStructure->pDBStructureRef->pDBStructure->ubNumberOfTiles > 1 )
				{
					// change sound ID
					uiSoundID = GARAGE_DOOR_CLOSE;
				}
		else if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_CLOTH )
			{
				// change sound ID
				uiSoundID = CURTAINS_CLOSE;
			}
		}
		else if ( pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_LIGHT_METAL ||
			pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_THICKER_METAL ||
			pStructure->pDBStructureRef->pDBStructure->ubArmour == MATERIAL_HEAVY_METAL )
			{
				// change sound ID
				uiSoundID = METAL_DOOR_CLOSE;
			}

	 AniParams.uiKeyFrame1Code			= ANI_KEYFRAME_DO_SOUND;
	 AniParams.uiUserData					= uiSoundID;
	 AniParams.uiUserData3					= sGridNo;


			if ( fOpenedGraphic )
			{
				AniParams.sGridNo							= sGridNo;
				AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
				AniParams.usTileType				= (UINT16)gTileDatabase[ pNode->usIndex ].fType;
				AniParams.usTileIndex					= pNode->usIndex;
				AniParams.sDelay							= INTTILE_DOOR_OPENSPEED;
				AniParams.sStartFrame					= pNode->sCurrentFrame;
				AniParams.uiFlags							= ANITILE_DOOR | ANITILE_BACKWARD | ANITILE_EXISTINGTILE;
				AniParams.pGivenLevelNode			= pNode;

		 AniParams.ubKeyFrame1					= pNode->sCurrentFrame - 2;

				CreateAnimationTile( &AniParams );
			}
			else
			{
				AniParams.sGridNo							= sGridNo;
				AniParams.ubLevelID						= ANI_STRUCT_LEVEL;
				AniParams.usTileType				= (UINT16)gTileDatabase[ pNode->usIndex ].fType;
				AniParams.usTileIndex					= pNode->usIndex;
				AniParams.sDelay							= INTTILE_DOOR_OPENSPEED;
				AniParams.sStartFrame					= pNode->sCurrentFrame;
				AniParams.uiFlags							= ANITILE_DOOR | ANITILE_FORWARD | ANITILE_EXISTINGTILE;
				AniParams.pGivenLevelNode			= pNode;

		AniParams.ubKeyFrame1					= pNode->sCurrentFrame + 2;

				CreateAnimationTile( &AniParams );
			}
		}

		//if ( pShadowNode != NULL )
		//{
		//	pShadowNode->uiFlags |= LEVELNODE_ANIMATION;
		//	pShadowNode->uiFlags |= LEVELNODE_ANIMATION_PLAYONCE;
		//	pShadowNode->uiFlags |= LEVELNODE_ANIMATION_BACKWARD;
		//	if ( pShadowNode->uiFlags & LEVELNODE_ANIMATION_FORWARD )
		//		pShadowNode->uiFlags ^= LEVELNODE_ANIMATION_FORWARD;
		//	pShadowNode->sDelay		= INTTILE_DOOR_OPENSPEED;
		//}

	}

	if ( fDoAnimation )
	{
		gTacticalStatus.uiFlags |= NOHIDE_REDUNDENCY;
		// FOR THE NEXT RENDER LOOP, RE-EVALUATE REDUNDENT TILES
		InvalidateWorldRedundency( );

		if ( GridNoOnScreen( sGridNo ) )
		{
			SetRenderFlags(RENDER_FLAG_FULL);
		}
	}

	return( fDoAnimation );
}

void SetDoorString( INT32 sGridNo )
{
	DOOR		*pDoor;
	DOOR_STATUS *		pDoorStatus;
	STRUCTURE *			pStructure;

	BOOLEAN	fTrapped = FALSE;

	// Try and get a door if one exists here
	pDoor = FindDoorInfoAtGridNo( sGridNo );

	if ( gfUIIntTileLocation == FALSE )
	{
		if ( pDoor == NULL )
		{
			wcscpy( gzIntTileLocation, TacticalStr[ DOOR_DOOR_MOUSE_DESCRIPTION ] );
			gfUIIntTileLocation = TRUE;
		}
		else
		{
			wcscpy( gzIntTileLocation, TacticalStr[ DOOR_DOOR_MOUSE_DESCRIPTION ] );
			gfUIIntTileLocation = TRUE;

			// CHECK PERCEIVED VALUE
			switch( pDoor->bPerceivedTrapped )
			{
				case DOOR_PERCEIVED_TRAPPED:

					wcscpy( gzIntTileLocation2, TacticalStr[ DOOR_TRAPPED_MOUSE_DESCRIPTION ] );
					gfUIIntTileLocation2 = TRUE;
					fTrapped = TRUE;
					break;
			}

			if ( !fTrapped )
			{
				// CHECK PERCEIVED VALUE
				switch( pDoor->bPerceivedLocked )
				{
					case DOOR_PERCEIVED_UNKNOWN:

						break;

					case DOOR_PERCEIVED_LOCKED:

						wcscpy( gzIntTileLocation2, TacticalStr[ DOOR_LOCKED_MOUSE_DESCRIPTION ] );
						gfUIIntTileLocation2 = TRUE;
						break;

					case DOOR_PERCEIVED_UNLOCKED:

						wcscpy( gzIntTileLocation2, TacticalStr[ DOOR_UNLOCKED_MOUSE_DESCRIPTION ] );
						gfUIIntTileLocation2 = TRUE;
						break;

					case DOOR_PERCEIVED_BROKEN:

						wcscpy( gzIntTileLocation2, TacticalStr[ DOOR_BROKEN_MOUSE_DESCRIPTION ] );
						gfUIIntTileLocation2 = TRUE;
						break;

				}
			}

		}
	}

	// ATE: If here, we try to say, opened or closed...
	if ( gfUIIntTileLocation2 == FALSE )
	{
#ifdef GERMAN

			wcscpy( gzIntTileLocation2, TacticalStr[ DOOR_DOOR_MOUSE_DESCRIPTION ] );
			gfUIIntTileLocation2 = TRUE;

			// Try to get doors status here...
			pDoorStatus = GetDoorStatus( sGridNo );
			if ( pDoorStatus == NULL || ( pDoorStatus != NULL && pDoorStatus->ubFlags & DOOR_PERCEIVED_NOTSET ) )
			{
				// OK, get status based on graphic.....
				pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
				if (pStructure)
				{
					if ( pStructure->fFlags & STRUCTURE_OPEN )
					{
						// Door is opened....
						wcscpy( gzIntTileLocation, pMessageStrings[ MSG_OPENED ] );
						gfUIIntTileLocation = TRUE;
					}
					else
					{
						// Door is closed
						wcscpy( gzIntTileLocation, pMessageStrings[ MSG_CLOSED ] );
						gfUIIntTileLocation = TRUE;
					}
				}
			}
			else
			{
				// Use percived value
				if ( pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN )
				{
					// Door is opened....
					wcscpy( gzIntTileLocation, pMessageStrings[ MSG_OPENED ] );
					gfUIIntTileLocation = TRUE;
				}
				else
				{
					// Door is closed
					wcscpy( gzIntTileLocation, pMessageStrings[ MSG_CLOSED ] );
					gfUIIntTileLocation = TRUE;
				}
			}
#else

			// Try to get doors status here...
			pDoorStatus = GetDoorStatus( sGridNo );
			if ( pDoorStatus == NULL || ( pDoorStatus != NULL && pDoorStatus->ubFlags & DOOR_PERCEIVED_NOTSET ) )
			{
				// OK, get status based on graphic.....
				pStructure = FindStructure( sGridNo, STRUCTURE_ANYDOOR );
				if (pStructure)
				{
					if ( pStructure->fFlags & STRUCTURE_OPEN )
					{
						// Door is opened....
						wcscpy( gzIntTileLocation2, pMessageStrings[ MSG_OPENED ] );
						gfUIIntTileLocation2 = TRUE;
					}
					else
					{
						// Door is closed
						wcscpy( gzIntTileLocation2, pMessageStrings[ MSG_CLOSED ] );
						gfUIIntTileLocation2 = TRUE;
					}
				}
			}
			else
			{
				// Use percived value
				if ( pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN )
				{
					// Door is opened....
					wcscpy( gzIntTileLocation2, pMessageStrings[ MSG_OPENED ] );
					gfUIIntTileLocation2 = TRUE;
				}
				else
				{
					// Door is closed
					wcscpy( gzIntTileLocation2, pMessageStrings[ MSG_CLOSED ] );
					gfUIIntTileLocation2 = TRUE;
				}
			}

#endif
	}

}

#ifdef JA2UB
//Ja25 UB
void HandleForceingTheTunnelGate( UINT32 sGridNo )
{
	//if this isnt the Last sector of the tunnel
	if( !( gWorldSectorX == 14 && gWorldSectorY == MAP_ROW_K && gbWorldSectorZ == 1 ) )
	{
		return;
	}

	//if it is not the right gridno
	if( sGridNo != 4742 )
	{
		return;
	}

	//If the player blew up the fan, then the enemies can hear it in the tunnel and prepare for it.
	gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__DID_PLAYER_MAKE_SOUND_GOING_THROUGH_TUNNEL_GATE;
}
#endif
