#pragma once

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlan
         * @brief Component/Concrete Product. Wrapper/Re-Write of DecideAction()
         *
         * Wrapper around soldier related AI uplifted from original DecideAction() routines
         */
        class SoldierPlan : public Plan
        {
        private:
        public:
            SoldierPlan(SOLDIERTYPE* npc);
            virtual void execute(PlanInputData& environment);
            virtual bool done() const { return false; }
        };
    }
}
