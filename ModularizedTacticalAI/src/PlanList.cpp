#include "../include/PlanList.h"
#include "../../TacticalAI/AIInternals.h"

#include <stdexcept>

namespace AI
{
    namespace tactical
    {
        PlanList::PlanList(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }

        PlanList::~PlanList()
        {
            for(std::deque<Plan*>::iterator subplan_iter(subplans_.begin()); subplan_iter != subplans_.end(); ++subplan_iter)
                delete *subplan_iter;
        }

        void PlanList::add_subplan(Plan* p)
        {
            if(!p)
                throw std::logic_error("No empty plans in PlanList allowed");
            subplans_.push_back(p);
        }

        void PlanList::execute(PlanInputData& environment)
        {
            while(!subplans_.empty() && subplans_.front()->done())
            {
                delete subplans_.front();
                subplans_.pop_front();
                DEBUGAIMSG("Plan done, switching to next...");
            }
            if(subplans_.empty())
                return;
            subplans_.front()->execute(environment);
        }

        bool PlanList::done() const
        {
            return subplans_.empty();
        }
    }
}

