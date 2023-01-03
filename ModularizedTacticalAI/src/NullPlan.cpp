/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/NullPlan.h"

#include "../../TacticalAI/ai.h" // for EndAIGuysTurn

namespace AI
{
    namespace tactical
    {
        NullPlan::NullPlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }

        /// Simply set the action to be performed to AI_ACTION_NONE
        void NullPlan::execute(PlanInputData& environment)
        {
            get_npc()->aiData.bAction = AI_ACTION_NONE;
        }
    }
}

