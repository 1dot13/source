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
        /// Simply end the turn for the npc passed in the manipulated_object wrapper, then return.
        void NullPlan::execute(bool turn_based, PlanInputData& manipulated_object)
        {
            EndAIGuysTurn(manipulated_object.controlled_npc_);
        }
    }
}

