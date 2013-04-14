/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/NullPlanFactory.h"
#include "../include/NullPlan.h"
#include "../Tactical/Soldier Control.h"

namespace AI
{
    namespace tactical
    {
        Plan* NullPlanFactory::create_plan(const AIInputData& input)
        {
            return new NullPlan();
        }

        void NullPlanFactory::update_plan(const AIInputData& input, Plan* plan_to_change)
        {
            // the idea is to do nothing, so let's do it...
        }
    }
}

