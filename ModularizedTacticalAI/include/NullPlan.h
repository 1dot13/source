/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef NULL_PLAN_H_
#define NULL_PLAN_H_

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class NullPlan
         * @brief Component/Concrete Product. The NullPlan lets makes the NPC executing it do absolutely nothing.
         *
         * The purpose of this plan is two-fold. Firstly, it is a good starting point for new plans; to use it
         * as such, perform a

         * - \c svn copy include/NullPlan.h include/YourPlan.h
         * - \c svn copy src/NullPlan.cpp src/YourPlan.cpp
         *
         * Remember to
         *
         *  - Adjust the include guards
         *  - A concrete factory using this plan is required in order for it to be used
         *
         * And secondly, it is a debugging tool (cf. NullPlanFactory)
         */
        class NullPlan: public Plan
        {
            private:
            public:
                virtual void execute(bool turn_based, PlanInputData& manipulated_object);
        };
    }
}

#endif

