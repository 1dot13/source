#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include "ai.h"
	#include "AIInternals.h"
	#include "Isometric utils.h"
	#include "overhead.h"
	#include "soldier add.h"
	#include "Soldier Profile Type.h"
	#include "Items.h"
	#include "Weapons.h"
	#include "Soldier Macros.h"
	#include "Soldier Profile.h"
	#include "NPC.h"
	#include "Render Fun.h"
	#include "Quests.h"
	#include "GameSettings.h"
#endif
// needed to use the modularized tactical AI:
#include "ModularizedTacticalAI/include/Plan.h"
#include "ModularizedTacticalAI/include/PlanFactoryLibrary.h"
#include "ModularizedTacticalAI/include/AbstractPlanFactory.h"


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
			//DBrot: More Rooms
			//UINT8		ubRoom;
			UINT16 usRoom;

			if ( InARoom( pSoldier->sGridNo, &usRoom ) && IN_BROTHEL( usRoom ) )
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

	// Flugente: prisoners of war don't do anything
	if ( pSoldier->usSoldierFlagMask & SOLDIER_POW )
		return;

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
                    if(!pSoldier->ai_masterplan_) // if the Soldier has no plan, create one
                    {
                        if(pSoldier->bAIIndex == 0) // not yet initialized, use bTeam+1 as default
                            pSoldier->bAIIndex = pSoldier->bTeam + 1;
                        AI::tactical::AIInputData ai_input;
                        AI::tactical::PlanFactoryLibrary* plan_lib(AI::tactical::PlanFactoryLibrary::instance());
                        pSoldier->ai_masterplan_ = plan_lib->create_plan(pSoldier->bAIIndex, pSoldier, ai_input);
                    }
                    AI::tactical::PlanInputData plan_input(false, gTacticalStatus);
                    pSoldier->ai_masterplan_->execute(plan_input);
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
					//DBrot: More Rooms
					//UINT8		ubRoom;
					UINT16 usRoom;

					if ( InARoom( pSoldier->sGridNo, &usRoom ) && IN_BROTHEL( usRoom ) )
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
