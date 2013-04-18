/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef CROW_PLAN_H_
#define CROW_PLAN_H_

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class CrowSeekCorpsePlan
         * @brief Component/Concrete Product. Let a crow seek a corpse within 4 tiles radius, and go towards it.
         */
        class CrowSeekCorpsePlan : public Plan
        {
            private:
                /// The location of the corpse the crow shall go to
                int corpse_grid_;
            public:
                CrowSeekCorpsePlan(SOLDIERTYPE* npc);
                virtual void execute(PlanInputData& environment);
                /// The plan is marked as 'done' when the 'sweet spot' near the corpse is reached
                virtual bool done() const;
                int get_corpse_grid() const;
        };

        /**@class CrowPeckPlan
         * @brief Component/Concrete Product. Let a crow sitting next to a corpse peck.
         */
        class CrowPeckPlan : public Plan
        {
            private:
                int corpse_grid_;
            public:
                CrowPeckPlan(SOLDIERTYPE* npc, int corpse_grid);
                virtual void execute(PlanInputData& environment);
                /// The plan never ends without interrupt/plan update, pecking can take forever
                virtual bool done() const {return false;}
        };

        /**@class CrowFlyAwayPlan
         * @brief Component/Concrete Product. Let a crow leave the tactical screen.
         */
        class CrowFlyAwayPlan : public Plan
        {
            private:
            public:
                CrowFlyAwayPlan(SOLDIERTYPE* npc);
                virtual void execute(PlanInputData& environment);
                /// The plan is deleted with the crow once it leaves; 'done' is meaningless, flying away cannot be interrupted
                virtual bool done() const {return false;}
        };
    }
}

#endif

