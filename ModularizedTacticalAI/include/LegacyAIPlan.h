/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef LEGACY_AI_PLAN_H_
#define LEGACY_AI_PLAN_H_

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlan
         * @brief Component/Concrete Product. Wrapper/Re-Write of DecideAction()
         *
         * This plan began as a simple forwarding object for DecideAction(), and will, in the course of the AI redesign,
         * be split into elementary NPC actions. It is as such only a intermediate product used to bring structure where
         * the is currently none.
         */
        class LegacyAIPlan: public Plan
        {
            private:
            public:
                LegacyAIPlan(SOLDIERTYPE* npc);
                virtual void execute(PlanInputData& environment);
                virtual bool done() const {return false;}
        };
    }
}

#endif

