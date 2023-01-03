/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef LEGACY_ZOMBIE_PLAN_H_
#define LEGACY_ZOMBIE_PLAN_H_

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyZombiePlan
         * @brief Component/Concrete Product. Wrapper/Re-Write of ZombieDecideAction()
         *
         * This plan began as a simple forwarding object for ZombieDecideAction(), and will, in the course of the AI redesign,
         * be split into elementary NPC actions. It is as such only a intermediate product used to bring structure where
         * the is currently none.
         */
        class LegacyZombiePlan: public Plan
        {
            private:
            public:
                LegacyZombiePlan(SOLDIERTYPE* npc);
                virtual void execute(PlanInputData& environment);
                virtual bool done() const {return false;}
        };
    }
}

#endif

