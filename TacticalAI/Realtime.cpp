#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include "ai.h"
	#include "AIInternals.h"
	#include "Isometric utils.h"
	#include "Points.h"
	#include "overhead.h"
	#include "opplist.h"
	#include "rotting corpses.h"
	#include "soldier add.h"
	#include "Soldier Profile Type.h"
	#include "Items.h"
	#include "Weapons.h"
	#include "Soldier Macros.h"
	#include "Soldier Profile.h"
	#include "NPC.h"
	#include "Render Fun.h"
	#include "Quests.h"
#endif

INT8 RTPlayerDecideAction( SOLDIERTYPE * pSoldier )
{
	INT8 bAction=AI_ACTION_NONE;

	if (gTacticalStatus.fAutoBandageMode)
	{
		bAction = DecideAutoBandage( pSoldier );
	}
	else
	{
		bAction = DecideAction( pSoldier );
	}

	#ifdef DEBUGDECISIONS
		STR tempstr;
		sprintf(tempstr,"DecideAction: selected action %d, actionData %d\n\n",bAction,pSoldier->aiData.usActionData);
		DebugAI( tempstr );
	#endif

	return(bAction);
}

INT8 RTDecideAction(SOLDIERTYPE *pSoldier)
{
	if (CREATURE_OR_BLOODCAT( pSoldier ) )
	{
		return( CreatureDecideAction( pSoldier ) );
	}
	else if (pSoldier->ubBodyType == CROW)
	{
		return( CrowDecideAction( pSoldier ) );
	}
	else if (pSoldier->bTeam == gbPlayerNum)
	{
		return( RTPlayerDecideAction( pSoldier ) );
	}
	else
	{
		// handle traversal
		if ( (pSoldier->ubProfile != NO_PROFILE) && (gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags3 & PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL ) )
		{
			TriggerNPCWithGivenApproach( pSoldier->ubProfile, APPROACH_DONE_TRAVERSAL, FALSE );
			gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags3 &= (~PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL);
			pSoldier->ubQuoteActionID = 0;
			// wait a tiny bit
			pSoldier->aiData.usActionData = 100;
			return( AI_ACTION_WAIT );
		}

		return( DecideAction( pSoldier ) );
	}
}

UINT16 RealtimeDelay( SOLDIERTYPE * pSoldier )
{
	if ( PTR_CIV_OR_MILITIA && !(pSoldier->ubCivilianGroup == KINGPIN_CIV_GROUP ) )
	{
		return( (UINT16) REALTIME_CIV_AI_DELAY );
	}
	else if ( CREATURE_OR_BLOODCAT( pSoldier ) && !( pSoldier->aiData.bHunting ) )
	{
		return( (UINT16) REALTIME_CREATURE_AI_DELAY );
	}
	else
	{

		if ( pSoldier->ubCivilianGroup == KINGPIN_CIV_GROUP )
		{
			UINT8		ubRoom;

			if ( InARoom( pSoldier->sGridNo, &ubRoom ) && IN_BROTHEL( ubRoom ) )
			{
				return( (UINT16) (REALTIME_AI_DELAY / 3) );
			}
		}

		return( (UINT16) REALTIME_AI_DELAY );
	}

}


void RTHandleAI( SOLDIERTYPE * pSoldier )
{
#ifdef AI_PROFILING
	INT32 iLoop;
#endif

	if ((pSoldier->aiData.bAction != AI_ACTION_NONE) && pSoldier->aiData.bActionInProgress)
	{
		// if action should remain in progress
		if (ActionInProgress(pSoldier))
		{
			#ifdef DEBUGBUSY
				AINumMessage("Busy with action, skipping guy#",pSoldier->ubID);
			#endif
			// let it continue
			return;
	}
	}

	// if man has nothing to do
	if (pSoldier->aiData.bAction == AI_ACTION_NONE)
	{
		if (pSoldier->aiData.bNextAction == AI_ACTION_NONE)
		{
			// make sure this flag is turned off (it already should be!)
			pSoldier->aiData.bActionInProgress = FALSE;

			// truly nothing to do!
			RefreshAI( pSoldier );
		}

		// Since we're NEVER going to "continue" along an old path at this point,
		// then it would be nice place to reinitialize "pathStored" flag for
		// insurance purposes.
		//
		// The "pathStored" variable controls whether it's necessary to call
		// findNewPath() after you've called NewDest(). Since the AI calls
		// findNewPath() itself, a speed gain can be obtained by avoiding
		// redundancy.
		//
		// The "normal" way for pathStored to be reset is inside
		// SetNewCourse() [which gets called after NewDest()].
		//
		// The only reason we would NEED to reinitialize it here is if I've
		// incorrectly set pathStored to TRUE in a process that doesn't end up
		// calling NewDest()
		pSoldier->pathing.bPathStored = FALSE;

		// decide on the next action
#ifdef AI_PROFILING
		for (iLoop = 0; iLoop < 1000; iLoop++)
#endif
		{
			if (pSoldier->aiData.bNextAction != AI_ACTION_NONE)
			{
				if ( pSoldier->aiData.bNextAction == AI_ACTION_END_COWER_AND_MOVE )
				{
					if ( pSoldier->flags.uiStatusFlags & SOLDIER_COWERING )
					{
						pSoldier->aiData.bAction = AI_ACTION_STOP_COWERING;
						pSoldier->aiData.usActionData = ANIM_STAND;
					}
					else if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight < ANIM_STAND )
					{
						// stand up!
						pSoldier->aiData.bAction = AI_ACTION_CHANGE_STANCE;
						pSoldier->aiData.usActionData = ANIM_STAND;
					}
					else
					{
						pSoldier->aiData.bAction = AI_ACTION_NONE;
					}
					if ( pSoldier->sGridNo == pSoldier->aiData.usNextActionData )
					{
						// no need to walk after this
						pSoldier->aiData.bNextAction = AI_ACTION_NONE;
						pSoldier->aiData.usNextActionData = NOWHERE;
					}
					else
					{
						pSoldier->aiData.bNextAction = AI_ACTION_WALK;
						// leave next-action-data as is since that's where we want to go
					}
				}
				else
				{
					// do the next thing we have to do...
					pSoldier->aiData.bAction = pSoldier->aiData.bNextAction;
					pSoldier->aiData.usActionData = pSoldier->aiData.usNextActionData;
					pSoldier->bTargetLevel = pSoldier->aiData.bNextTargetLevel;
					pSoldier->aiData.bNextAction = AI_ACTION_NONE;
					pSoldier->aiData.usNextActionData = 0;
					pSoldier->aiData.bNextTargetLevel = 0;
				}
				if (pSoldier->aiData.bAction == AI_ACTION_PICKUP_ITEM)
				{
					// the item pool index was stored in the special data field
					pSoldier->aiData.uiPendingActionData1 = pSoldier->iNextActionSpecialData;
				}
			}			
			else if (!TileIsOutOfBounds(pSoldier->sAbsoluteFinalDestination))
			{
				if ( ACTING_ON_SCHEDULE( pSoldier ) )
				{
					pSoldier->aiData.bAction = AI_ACTION_SCHEDULE_MOVE;
				}
				else
				{
					pSoldier->aiData.bAction = AI_ACTION_WALK;
				}
				pSoldier->aiData.usActionData = pSoldier->sAbsoluteFinalDestination;
			}
			else
			{
				if (!(gTacticalStatus.uiFlags & ENGAGED_IN_CONV))
				{
					pSoldier->aiData.bAction = RTDecideAction( pSoldier );
				}
			}
		}
		// if he chose to continue doing nothing
		if (pSoldier->aiData.bAction == AI_ACTION_NONE)
		{
			#ifdef RECORDNET
				fprintf(NetDebugFile,"\tMOVED BECOMING TRUE: Chose to do nothing, guynum %d\n",pSoldier->ubID);
			#endif

			// do a standard wait before doing anything else!
			pSoldier->aiData.bAction = AI_ACTION_WAIT;
			//if (PTR_CIVILIAN && pSoldier->aiData.bAlertStatus != STATUS_BLACK)
			if ( PTR_CIV_OR_MILITIA && !(pSoldier->ubCivilianGroup == KINGPIN_CIV_GROUP ) )
			{
				pSoldier->aiData.usActionData = (UINT16) REALTIME_CIV_AI_DELAY;
			}
			else if ( CREATURE_OR_BLOODCAT( pSoldier ) && !( pSoldier->aiData.bHunting ) )
			{
				pSoldier->aiData.usActionData = (UINT16) REALTIME_CREATURE_AI_DELAY;
			}
			else
			{
				pSoldier->aiData.usActionData = (UINT16) REALTIME_AI_DELAY;
				if ( pSoldier->ubCivilianGroup == KINGPIN_CIV_GROUP )
				{
					UINT8		ubRoom;

					if ( InARoom( pSoldier->sGridNo, &ubRoom ) && IN_BROTHEL( ubRoom ) )
					{
						pSoldier->aiData.usActionData /= 3;
					}

				}
			}
		}
		else if (pSoldier->aiData.bAction == AI_ACTION_ABSOLUTELY_NONE)
		{
			pSoldier->aiData.bAction = AI_ACTION_NONE;
		}

	}

	// to get here, we MUST have an action selected, but not in progress...
	NPCDoesAct(pSoldier);

	// perform the chosen action
	pSoldier->aiData.bActionInProgress = ExecuteAction(pSoldier); // if started, mark us as busy
}
