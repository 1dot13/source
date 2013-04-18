/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef LEGACY_AI_PLAN_FACTORY_H_
#define LEGACY_AI_PLAN_FACTORY_H_

#include "AbstractPlanFactory.h"
#include <string>

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlanFactory
         * @brief Concrete Factory. Generates the appropriate LegacyAIPlan, making NPCs do exactly what it did before
         * this AI modularization/re-write.
         *
         * Currently, the factory handles the "outermost" layer of selection statements by instantiating plans that will
         * call CreatureDecideAction(), ZombieDecideAction() or DecideAction(), respectively. This is not the intended
         * use of factories, but for the time of the AI redesign a necessity nonetheless.
         */
        class LegacyAIPlanFactory : public AbstractPlanFactory
        {
            private:
            public:
                /// The name is required for registration in the PlanFactoryLibrary
                static std::string get_name() {return "LegacyAIPlanFactory";}
                virtual Plan* create_plan(SOLDIERTYPE* npc, const AIInputData& input);
                virtual void update_plan(SOLDIERTYPE* npc, const AIInputData& input);
        };
    }
}

#endif

