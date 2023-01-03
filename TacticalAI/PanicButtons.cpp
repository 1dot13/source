#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include "AIInternals.h"
	#include "ai.h"
	#include "pathai.h"
	#include "items.h"
	#include "World Items.h"
	#include "strategicmap.h"
	#include "Map Screen Interface Map.h"
	#include "Soldier Profile.h"
	#include "Quests.h"
#endif

#include "Queen Command.h"

void MakeClosestEnemyChosenOne()
{
	UINT32				cnt;
	INT16					sPathCost, sShortestPath = 1000;
	INT8					bOldKeys = -1;
	UINT8					ubClosestEnemy = NOBODY;
	SOLDIERTYPE *	pSoldier;
	INT8					bPanicTrigger;
	INT32					sPanicTriggerGridNo;

	if ( ! (gTacticalStatus.fPanicFlags & PANIC_TRIGGERS_HERE) )
	{
#ifdef BETAVERSION
		PopMessage("MakeClosestEnemyChosenOne: ERROR - Panic Trigger is NOWHERE");
#endif

		return;
	}

	if (!NeedToRadioAboutPanicTrigger() )
	{
		// no active panic triggers
		return;
	}

	// consider every enemy, looking for the closest capable, unbusy one
	for (cnt = 0; cnt < guiNumMercSlots; cnt++)
	{
		pSoldier = MercSlots[cnt];

		if (!pSoldier)	// if this merc is inactive, or not here
		{
			continue;
		}

		// if this merc is unconscious, or dead
		if (pSoldier->stats.bLife < OKLIFE)
		{
			continue;	// next soldier
		}

		// if this guy's too tired to go
		if (pSoldier->bBreath < OKBREATH)
		{
			continue;	// next soldier
		}

		if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y )
		{
			if ( pSoldier->ubProfile != WARDEN )
			{
				continue;
			}
		}
		else
		{
			// only consider for army guys
			if (pSoldier->bTeam != ENEMY_TEAM )
			{
				continue;
			}
		}

		// if this guy is in battle with opponent(s)
		if (pSoldier->aiData.bOppCnt > 0)
		{
			continue;	// next soldier
		}

		// if this guy is still in serious shock
		if (pSoldier->aiData.bShock > 2)
		{
			continue;	// next soldier
		}

		if ( pSoldier->pathing.bLevel != 0 )
		{
			// screw having guys on the roof go for panic triggers!
			continue;	// next soldier
		}

		bPanicTrigger = ClosestPanicTrigger( pSoldier );
		if (bPanicTrigger == -1)
		{
			continue; // next soldier
		}

		sPanicTriggerGridNo = gTacticalStatus.sPanicTriggerGridNo[ bPanicTrigger ];		
		if (TileIsOutOfBounds(sPanicTriggerGridNo))
		{
			// this should never happen!
			continue;
		}

		// remember whether this guy had keys before
		//bOldKeys = pSoldier->flags.bHasKeys;

		// give him keys to see if with them he can get to the panic trigger
		pSoldier->flags.bHasKeys = (pSoldier->flags.bHasKeys << 1) | 1;

		// we now path directly to the panic trigger


			// if he can't get to a spot where he could get at the panic trigger
			/*
			if ( FindAdjacentGridEx( pSoldier, gTacticalStatus.sPanicTriggerGridno, &ubDirection, &sAdjSpot, FALSE, FALSE ) == -1 )
			{
				pSoldier->flags.bHasKeys = bOldKeys;
				continue;			// next merc
			}
			*/


		// ok, this enemy appears to be eligible

		// FindAdjacentGrid set HandGrid for us.	If we aren't at that spot already
		if (pSoldier->sGridNo != sPanicTriggerGridNo)
		{
			// get the AP cost for this enemy to go to target position
			sPathCost = PlotPath( pSoldier, sPanicTriggerGridNo, FALSE, FALSE, FALSE, WALKING, FALSE, FALSE, 0);
		}
		else
		{
			sPathCost = 0;
		}

		// set his keys value back to what it was before this hack
		pSoldier->flags.bHasKeys = (pSoldier->flags.bHasKeys >> 1 );

		// if he can get there (or is already there!)
		if (sPathCost || (pSoldier->sGridNo == sPanicTriggerGridNo))
		{
			if (sPathCost < sShortestPath)
			{
				sShortestPath = sPathCost;
				ubClosestEnemy = pSoldier->ubID;
			}
		}
		//else
		//NameMessage(pSoldier,"can't get there...");
	}

	// if we found have an eligible enemy, make him our "chosen one"
	if (ubClosestEnemy < TOTAL_SOLDIERS)
	{
		gTacticalStatus.ubTheChosenOne = ubClosestEnemy;		// flag him as the chosen one

#ifdef TESTVERSION
		NumMessage("TEST MSG: The chosen one is ",TheChosenOne);
#endif

		pSoldier = MercPtrs[gTacticalStatus.ubTheChosenOne];
		if ( pSoldier->aiData.bAlertStatus < STATUS_RED )
		{
			pSoldier->aiData.bAlertStatus = STATUS_RED;
			CheckForChangingOrders( pSoldier );
		}
		SetNewSituation( pSoldier );	// set new situation for the chosen one
		pSoldier->flags.bHasKeys = (pSoldier->flags.bHasKeys << 1) | 1; // cheat and give him keys to every door
		//pSoldier->flags.bHasKeys = TRUE;
	}
#ifdef TESTVERSION
	else
		PopMessage("TEST MSG: Couldn't find anyone eligible to become TheChosenOne!");
#endif
}

void PossiblyMakeThisEnemyChosenOne( SOLDIERTYPE * pSoldier )
{
	INT32		iAPCost, iPathCost;
	//INT8		bOldKeys;
	INT8		bPanicTrigger;
	INT32		sPanicTriggerGridNo;
	UINT32	uiPercentEnemiesKilled = 0;

	if ( ! (gTacticalStatus.fPanicFlags & PANIC_TRIGGERS_HERE) )
	{
		return;
	}

	if ( pSoldier->pathing.bLevel != 0 )
	{
		// screw having guys on the roof go for panic triggers!
		return;
	}

	bPanicTrigger = ClosestPanicTrigger( pSoldier );
	if (bPanicTrigger == -1)
	{
		return;
	}

	sPanicTriggerGridNo = gTacticalStatus.sPanicTriggerGridNo[ bPanicTrigger ];

	if ( gTacticalStatus.Team[ENEMY_TEAM].bMenInSector + gTacticalStatus.ubArmyGuysKilled > 0 )
		uiPercentEnemiesKilled = (UINT32)( 100 * (UINT32)(gTacticalStatus.ubArmyGuysKilled) / (UINT32)( gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector + gTacticalStatus.ubArmyGuysKilled ) );

	if ( gTacticalStatus.ubPanicTolerance[ bPanicTrigger ] > uiPercentEnemiesKilled )
	{
		// not yet... not yet
		return;
	}

	//bOldKeys = pSoldier->flags.bHasKeys;
	pSoldier->flags.bHasKeys = (pSoldier->flags.bHasKeys << 1) | 1;

	// if he can't get to a spot where he could get at the panic trigger
	iAPCost = APBPConstants[AP_PULL_TRIGGER];
	if (pSoldier->sGridNo != sPanicTriggerGridNo)
	{
		iPathCost = PlotPath( pSoldier, sPanicTriggerGridNo, FALSE, FALSE, FALSE, RUNNING, FALSE, FALSE, 0);
		if (iPathCost == 0)
		{
			//pSoldier->flags.bHasKeys = bOldKeys;
			pSoldier->flags.bHasKeys = (pSoldier->flags.bHasKeys >> 1);
			return;
		}
		iAPCost += iPathCost;

	}

	if ( iAPCost <= pSoldier->CalcActionPoints( ) * 2)
	{
		// go!!!
		gTacticalStatus.ubTheChosenOne = pSoldier->ubID;
		return;
	}
	// else return keys to normal
	//pSoldier->flags.bHasKeys = bOldKeys;
	pSoldier->flags.bHasKeys = (pSoldier->flags.bHasKeys >> 1);
}


INT8 PanicAI(SOLDIERTYPE *pSoldier, UINT8 ubCanMove)
{
	BOOLEAN	fFoundRoute = FALSE;
	INT8	bSlot;
	INT32	iPathCost;
	INT8	bPanicTrigger;
	INT32	sPanicTriggerGridNo;
	UINT16	usMovementMode;
#ifdef DEBUGDECISIONS
	STR16 tempstr;
#endif

	DebugAI(AI_MSG_TOPIC, pSoldier, String("[Panic AI]"));

	// if there are panic bombs here
	if (gTacticalStatus.fPanicFlags & PANIC_BOMBS_HERE)
	{
		// if enemy is holding a portable panic bomb detonator, he tries to use it
		bSlot = FindTrigger( pSoldier );
		if (bSlot != NO_SLOT)
		{
			DebugAI(AI_MSG_TOPIC, pSoldier, String("found trigger, slot %d", bSlot));
			//////////////////////////////////////////////////////////////////////
			// ACTIVATE DETONATOR: blow up sector's panic bombs
			//////////////////////////////////////////////////////////////////////

			// if we have enough APs to activate it now
			if (pSoldier->bActionPoints >= APBPConstants[AP_USE_REMOTE])
			{
#ifdef TESTVERSION
				sprintf(tempstr,"TEST MSG: %s - ACTIVATING his DETONATOR!",pSoldier->name);
				PopMessage(tempstr);
#endif
				// blow up all the PANIC bombs!
				DebugAI(AI_MSG_TOPIC, pSoldier, String("blow up panic bomb"));
				return(AI_ACTION_USE_DETONATOR);
			}
			else	 // otherwise, wait a turn
			{
				pSoldier->aiData.usActionData = NOWHERE;
				DebugAI(AI_MSG_TOPIC, pSoldier, String("wait a turn"));
				return(AI_ACTION_NONE);
			}
		}
	}

	// no panic bombs, or no portable detonator

	// if there's a panic trigger here (DOESN'T MATTER IF ANY PANIC BOMBS EXIST!)
	if ( gTacticalStatus.fPanicFlags & PANIC_TRIGGERS_HERE )
	{
		DebugAI(AI_MSG_TOPIC, pSoldier, String("found panic trigger"));
		usMovementMode = DetermineMovementMode(pSoldier, AI_ACTION_GET_CLOSER);
		DebugAI(AI_MSG_TOPIC, pSoldier, String("movement mode %d", usMovementMode));

		// Have WE been chosen to go after the trigger?
		if (pSoldier->ubID == gTacticalStatus.ubTheChosenOne)
		{
			bPanicTrigger = ClosestPanicTrigger( pSoldier );

			if (bPanicTrigger == -1)
			{
				// augh!
				return( -1 );
			}

			sPanicTriggerGridNo = gTacticalStatus.sPanicTriggerGridNo[ bPanicTrigger ];
			DebugAI(AI_MSG_TOPIC, pSoldier, String("closest panic trigger %d, spot %d", bPanicTrigger, sPanicTriggerGridNo));

			// if not standing on the panic trigger
			if (pSoldier->sGridNo != sPanicTriggerGridNo)
			{
				// determine whether we can still get there
				//iPathCost = PlotPath( pSoldier, sPanicTriggerGridNo, FALSE, FALSE, FALSE, RUNNING, FALSE, FALSE, 0);
				iPathCost = PlotPath(pSoldier, sPanicTriggerGridNo, FALSE, FALSE, FALSE, usMovementMode, pSoldier->bStealthMode, FALSE, 0);

				if (iPathCost != 0)
				{
					DebugAI(AI_MSG_TOPIC, pSoldier, String("found route to panic trigger, path cost %d", iPathCost));
					fFoundRoute = TRUE;
				}
			}
			else
			{
				DebugAI(AI_MSG_TOPIC, pSoldier, String("standing at panic trigger spot"));
				fFoundRoute = TRUE;
			}

			// if we managed to find an adjacent spot
			if (fFoundRoute)
			{
				DebugAI(AI_MSG_TOPIC, pSoldier, String("have good route to panic triger"));
				/*
				 *** COMMENTED OUT BECAUSE WE DON'T HAVE SUPPORT ROUTINES YET

				 // make sure it's not in water (those triggers can't be pulled)
				 if (Water(Terrain(gTacticalStatus.sHandGrid),Structure(gTacticalStatus.sHandGrid)))
				 {
				 #ifdef BETAVERSION
				 PopMessage("BAD SCENARIO DESIGN: Enemies can't use this panic trigger!");
				 #endif
				 gTacticalStatus.ubTheChosenOne = NOBODY;	// strip him of his Chosen One status
				 // don't bother replacing him either, the next won't have more luck!
				 return(-1);
				 }
				*/

				// if we are at that spot now
				if (pSoldier->sGridNo == sPanicTriggerGridNo)
				{
					DebugAI(AI_MSG_TOPIC, pSoldier, String("we are at the spot, activate the trigger!"));
					////////////////////////////////////////////////////////////////
					// PULL THE PANIC TRIGGER!
					////////////////////////////////////////////////////////////////

					// and we have enough APs left to pull the trigger
					if (pSoldier->bActionPoints >= APBPConstants[AP_PULL_TRIGGER])
					{
						// blow up the all the PANIC bombs (or just the journal)
						pSoldier->aiData.usActionData = sPanicTriggerGridNo;

#ifdef TESTVERSION
						sprintf(tempstr,"TEST MSG: %s - PULLS PANIC TRIGGER at grid %d",
						pSoldier->name,pSoldier->aiData.usActionData);
						PopMessage(tempstr);
#endif
						DebugAI(AI_MSG_TOPIC, pSoldier, String("enough AP, activate trigger!"));
						return(AI_ACTION_PULL_TRIGGER);
					}
					else		// otherwise, wait a turn
					{
						DebugAI(AI_MSG_TOPIC, pSoldier, String("wait a turn, not enough AP"));
						pSoldier->aiData.usActionData = NOWHERE;
						return(AI_ACTION_NONE);
					}
				}
				else			// we are NOT at the HandGrid spot
				{
					// if we can move at least 1 square's worth
					if (ubCanMove)
					{
						// if we can get to the HandGrid spot to yank the trigger
						// animations don't allow trigger-pulling from water, so we won't!
						if (LegalNPCDestination(pSoldier,sPanicTriggerGridNo,ENSURE_PATH,NOWATER,0))
						{
							pSoldier->aiData.usActionData = sPanicTriggerGridNo;
							pSoldier->pathing.bPathStored = TRUE;

#ifdef DEBUGDECISIONS
							sprintf(tempstr,"%s - GETTING CLOSER to PANIC TRIGGER at grid %d (Trigger at %d)", pSoldier->name,pSoldier->aiData.usActionData,sPanicTriggerGridNo);
							AIPopMessage(tempstr);
#endif
							DebugAI(AI_MSG_TOPIC, pSoldier, String("move closet to panic trigger %d", sPanicTriggerGridNo));
							return(AI_ACTION_GET_CLOSER);
						}
						else		// Oh oh, the chosen one can't get to the trigger!
						{
#ifdef TESTVERSION
							PopMessage("TEST MSG: Oh oh!	!legalDest - ChosenOne can't get to the trigger!");
#endif
							DebugAI(AI_MSG_TOPIC, pSoldier, String("cannot move to the trigger, find another chosen one"));
							gTacticalStatus.ubTheChosenOne = NOBODY;	// strip him of his Chosen One status
							MakeClosestEnemyChosenOne();	 // and replace him!
						}
					}
					else		 // can't move, wait 1 turn
					{
						DebugAI(AI_MSG_TOPIC, pSoldier, String("cannot move, wait one turn"));
						pSoldier->aiData.usActionData = NOWHERE;
						return(AI_ACTION_NONE);
					}
				}
			}
			else	 // Oh oh, the chosen one can't get to the trigger!
			{
#ifdef TESTVERSION
				PopMessage("TEST MSG: Oh oh!	!adjacentFound - ChosenOne can't get to the trigger!");
#endif
				DebugAI(AI_MSG_TOPIC, pSoldier, String("cannot find route to the trigger, find another chosen one"));
				gTacticalStatus.ubTheChosenOne = NOBODY; // strip him of his Chosen One status
				MakeClosestEnemyChosenOne();	// and replace him!
			}
		}
	}

	// no action decided
	return(-1);
}

void InitPanicSystem( void )
{
	// start by assuming there is no panic bombs or triggers here
	gTacticalStatus.ubTheChosenOne				= NOBODY;
	FindPanicBombsAndTriggers();
}

INT8 ClosestPanicTrigger( SOLDIERTYPE * pSoldier )
{
	INT8		bLoop;
	INT16		sDistance;
	INT16		sClosestDistance = 1000;
	INT8		bClosestTrigger = -1;
	UINT32	uiPercentEnemiesKilled = 0;

	if ( gTacticalStatus.Team[ENEMY_TEAM].bMenInSector + gTacticalStatus.ubArmyGuysKilled > 0 )
		 uiPercentEnemiesKilled = (UINT32)(100 * (UINT32)(gTacticalStatus.ubArmyGuysKilled) / (UINT32)(gTacticalStatus.Team[ENEMY_TEAM].bMenInSector + gTacticalStatus.ubArmyGuysKilled));

	for ( bLoop = 0; bLoop < NUM_PANIC_TRIGGERS; ++bLoop )
	{		
		if (!TileIsOutOfBounds(gTacticalStatus.sPanicTriggerGridNo[ bLoop ]))
		{
			if ( gTacticalStatus.ubPanicTolerance[ bLoop ] > uiPercentEnemiesKilled )
			{
				// not yet... not yet...
				continue; // next trigger
			}

			// in Tixa
			if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y )
			{
				// screen out everyone but the warden
				if ( pSoldier->ubProfile != WARDEN )
				{
					break;
				}

				// screen out the second/later panic trigger if the first one hasn't been triggered				
				if ( bLoop > 0 && !TileIsOutOfBounds(gTacticalStatus.sPanicTriggerGridNo[ bLoop - 1 ]) )
				{
					break;
				}
			}

			sDistance = PythSpacesAway( pSoldier->sGridNo, gTacticalStatus.sPanicTriggerGridNo[ bLoop ] );
			if (sDistance < sClosestDistance)
			{
				sClosestDistance = sDistance;
				bClosestTrigger = bLoop;
			}
		}
	}

	return( bClosestTrigger );
}

BOOLEAN NeedToRadioAboutPanicTrigger( void )
{
	UINT32		uiPercentEnemiesKilled;
	INT8			bLoop;

	if ( !(gTacticalStatus.fPanicFlags & PANIC_TRIGGERS_HERE) || gTacticalStatus.ubTheChosenOne != NOBODY )
	{
		// already done!
		return( FALSE );
	}

	if ( gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector == 0 )
	{
		return( FALSE );
	}

	if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y )
	{
		SOLDIERTYPE * pSoldier;
		pSoldier = FindSoldierByProfileID( WARDEN, FALSE );
		if ( !pSoldier || pSoldier->ubID == gTacticalStatus.ubTheChosenOne )
		{
			return( FALSE );
		}
	}


	uiPercentEnemiesKilled = (UINT32)( 100 * (UINT32)(gTacticalStatus.ubArmyGuysKilled) / (UINT32)( gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector + gTacticalStatus.ubArmyGuysKilled ) );

	for ( bLoop = 0; bLoop < NUM_PANIC_TRIGGERS; bLoop++ )
	{
		// if the bomb exists and its tolerance has been exceeded		
		if ( (!TileIsOutOfBounds(gTacticalStatus.sPanicTriggerGridNo[ bLoop ])) && ( uiPercentEnemiesKilled >= gTacticalStatus.ubPanicTolerance[ bLoop ] ) )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


#define STAIRCASE_GRIDNO 12067
#define STAIRCASE_DIRECTION 0

INT8 HeadForTheStairCase( SOLDIERTYPE * pSoldier )
{
	UNDERGROUND_SECTORINFO * pBasementInfo;

	pBasementInfo = FindUnderGroundSector( 3, MAP_ROW_P, 1 );
	if ( pBasementInfo && pBasementInfo->uiTimeCurrentSectorWasLastLoaded != 0 && ( pBasementInfo->ubNumElites + pBasementInfo->ubNumTroops + pBasementInfo->ubNumAdmins ) < 5 )
	{
		return( AI_ACTION_NONE );
	}

	if ( PythSpacesAway( pSoldier->sGridNo, STAIRCASE_GRIDNO ) < 2 )
	{
		return( AI_ACTION_TRAVERSE_DOWN );
	}
	else
	{
		if ( LegalNPCDestination( pSoldier, STAIRCASE_GRIDNO, ENSURE_PATH, WATEROK, 0 ) )
		{
			pSoldier->aiData.usActionData = STAIRCASE_GRIDNO;
			return( AI_ACTION_GET_CLOSER );
		}
	}
	return( AI_ACTION_NONE );
}

#define WARDEN_ALARM_GRIDNO 9376
#define WARDEN_GAS_GRIDNO 9216
// in both cases, direction 6
