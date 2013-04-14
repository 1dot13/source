/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef LEGACY_AI_PLAN_H_
#define LEGACY_AI_PLAN_H_

#include "Plan.h"

namespace AI
{
    namespace tactical
    {
        /**@class LegacyAIPlan
         * @brief Component/Concrete Product. Generates a Plan that executes the AI as it was before modularization.
         *
         * A simple encapsulation of the functions TurnBasedHandleNPCAI() and RTHandleAI(), defined in
         * TacticalAI/AIMain.cpp and TacticalAI/Realtime.cpp, respectively. The AI handles decisions for all kinds of
         * NPCs (enemies, zombies, civilians, ...)
         */
        class LegacyAIPlan: public Plan
        {
            private:
            public:
                virtual void execute(bool turn_based, PlanInputData& manipulated_object);
        };
    }
}

#endif

