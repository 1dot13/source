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
         * @brief Concrete Factory. Generates the LegacyAIPlan, making NPCs do exactly what it did before this AI
         * modularization/re-write.
         */
        class LegacyAIPlanFactory : public AbstractPlanFactory
        {
            private:
            public:
                static std::string get_name() {return "LegacyAIPlanFactory";}
                virtual Plan* create_plan(const AIInputData& input);
                virtual void update_plan(const AIInputData& input, Plan* plan_to_change);
        };
    }
}

#endif

