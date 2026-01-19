#pragma once

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlan
         * @brief Component/Concrete Product. Wrapper/Re-Write of DecideAction()
         *
         * Wrapper around civilian/noncombatant related AI uplifted from original DecideAction() routines
         */
        class CivilianPlan : public Plan
        {
        private:
        public:
            CivilianPlan(SOLDIERTYPE* npc);
            virtual void execute(PlanInputData& environment);
            virtual bool done() const { return false; }
        };
    }
}
