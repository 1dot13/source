/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/LegacyAIPlan.h"

#include "../../TacticalAI/ai.h" // for EndAIGuysTurn


// Forward declarations for the two legacy functions called here
void RTHandleAI( SOLDIERTYPE * pSoldier );          // defined in TacticalAI/Realtime.cpp
void TurnBasedHandleNPCAI(SOLDIERTYPE *pSoldier);   // defined in TacticalAI/AIMain.cpp

namespace AI
{
    namespace tactical
    {
        void LegacyAIPlan::execute(bool turn_based, PlanInputData& manipulated_object)
        {
            if(turn_based)
			    TurnBasedHandleNPCAI(manipulated_object.controlled_npc_);
            else
			    RTHandleAI(manipulated_object.controlled_npc_);
        }
    }
}

