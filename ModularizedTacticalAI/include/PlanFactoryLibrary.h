/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef PLAN_FACTORY_LIBRARY_H_
#define PLAN_FACTORY_LIBRARY_H_

#include <string>
#include <map>
#include <deque>

namespace AI
{
    namespace tactical
    {
        // forward declarations
        class AbstractPlanFactory;
        class Plan;
        struct AIInputData;

        /**@class PlanFactoryLibrary
         * @brief Singleton. A "library" containing all available PlanFactories, accessible via the strings returned by
         * their get_name() function.
         *
         */
        class PlanFactoryLibrary
        {
            private:
                static PlanFactoryLibrary* instance_;
                std::map<std::string, AbstractPlanFactory*> registred_factories_;
                std::deque<AbstractPlanFactory*> ai_index_to_factory_mapping_;
                PlanFactoryLibrary();
            public:
                static PlanFactoryLibrary* instance();
                Plan* create_plan(size_t index, AIInputData& input) const;
        };
    }
}

#endif

