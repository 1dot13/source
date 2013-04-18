#ifndef PLAN_LIST_H_
#define PLAN_LIST_H_

#include "Plan.h"
#include <deque>

namespace AI
{
    namespace tactical
    {
        /**@class PlanList
         * @brief Composition/Abstract Product. Used to concatenate plans.
         * 
         * Plan lists can comprise an arbitrary number of sub-plans, executed in the order they were added via
         * add_subplan(), and switched to the next one once the done()-flag of a sub-plan becomes true.
         */
        class PlanList : public Plan
        {
            private: 
                /// The sub-plan sequence
                std::deque<Plan*> subplans_;
            public:
                PlanList(SOLDIERTYPE* npc);
                /// Add a plan to the list of sub-plans
                virtual void add_subplan(Plan* P);
                /// Execute the next plan in the list
                virtual void execute(PlanInputData& environment);
                /// Done will be set once the last plan is marked as done()
                virtual bool done() const;
                virtual ~PlanList();
        };
    }
}

#endif

