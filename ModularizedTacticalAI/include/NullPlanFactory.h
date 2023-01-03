/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef NULL_PLAN_FACTORY_H_
#define NULL_PLAN_FACTORY_H_

#include "AbstractPlanFactory.h"
#include <string>

namespace AI
{
    namespace tactical
    {
        /**@class NullPlanFactory
         * @brief Concrete Factory. Generates the NullPlan, making a NPC do absolutely nothing.
         *
         * The purpose of this factory is two-fold. Firstly, it is a good starting point for new plan factories; to use
         * it as such, perform a

         * \c svn copy include/NullPlanFactory.h include/YourPlanFactory.h\n
         * \c svn copy src/NullPlanFactory.cpp src/YourPlanFactory.cpp
         *
         * Remember to
         *  - Adjust the include guards
         *  - Adjust the string returned by get_name()
         *  - Add a line in PlanFactoryLibrary.cpp to create an instance of the new factory in the library's
         *    constructor; otherwise, it won't be accessible via the configuration file.
         *
         * And secondly, it is a debugging tool. Change the configuration file to use this AI factory for a certain
         * AI_INDEX to have said group do... nothing (e.g. to measure performance differences).
         */
        class NullPlanFactory : public AbstractPlanFactory
        {
            private:
            public:
                static std::string get_name() {return "NullPlanFactory";}
                virtual Plan* create_plan(SOLDIERTYPE* npc, const AIInputData& input);
                virtual void update_plan(SOLDIERTYPE* npc, const AIInputData& input);
        };
    }
}

#endif

