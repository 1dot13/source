#pragma once

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlan
         * @brief Component/Concrete Product. Wrapper/Re-Write of DecideAction()
         *
         * Wrapper around boxer related AI uplifted from original DecideAction() routines
         */
        class LegacyAIBoxerPlan: public Plan
        {
            private:
            public:
                LegacyAIBoxerPlan(SOLDIERTYPE* npc);
                virtual void execute(PlanInputData& environment);
                virtual bool done() const {return false;}
        };
    }
}
