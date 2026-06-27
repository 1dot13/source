#pragma once

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlan
         * @brief Component/Concrete Product. Wrapper/Re-Write of DecideAction()
         *
         * Wrapper around robot related AI uplifted from original DecideAction() routines
         */
        class RobotPlan : public Plan
        {
        private:
        public:
            RobotPlan(SOLDIERTYPE* npc);
            virtual void execute(PlanInputData& environment);
            virtual bool done() const { return false; }
        };
    }
}
