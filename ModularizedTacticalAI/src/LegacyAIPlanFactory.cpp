/** 
 * @file
 * @author feynman (bears-pit.com)
 */
#include "../include/LegacyAIPlanFactory.h"
#include "../include/LegacyAIPlan.h"

namespace AI
{
    namespace tactical
    {
        Plan* LegacyAIPlanFactory::create_plan(const AIInputData& input)
        {
            return new LegacyAIPlan();
        }

        void LegacyAIPlanFactory::update_plan(const AIInputData& input, Plan* plan_to_change)
        {
            // TODO: currently, everything is handled in the LegacyAIPlan. The goal is to modularize the AI, so that in
            // the end, the LegacyAIPlan is split into so many "new" AI Plan subclasses, that every case is covered.
            // Then the behavior of the old AI would be implemented here.
        }
    }
}

