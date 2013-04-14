/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef PLAN_H_
#define PLAN_H_

class SOLDIERTYPE;

namespace AI
{
    namespace tactical
    {
        /**@class PlanInputData
         * @brief Wrapper class around the entity manipulated through plan execution
         * 
         * At the moment, simply a wrapper around SOLDIERTYPE. Future versions might require other datatypes; without
         * the wrapper, *every* Plan subclass ever created would need to be changed in this event. With the wrapper, no
         * change is required at all.
         */
        struct PlanInputData
        {
            SOLDIERTYPE* controlled_npc_;
        };
        /**@class Plan
         * @brief Composite/Abstract Product. Base class for all plan compositions and components.
         * 
         * The Plan class provides a common interface for both plan compositions and plan components.
         */
        class Plan
        {
            private:
            public:
                /** @brief Plan execution is the encapsulation of an action sequence, making up higher-level building blocks
                 *
                 * Each plan objects represents a high-level action, implemented by means of lower-level actions. These
                 * lower level actions can be other Plans subtypes; at some point, a Plan object has no further
                 * sub-plans, i.e., the objet is no composition, but a component, forming a leaf in the Plan hierarchy.
                 * The components' lower level actions are formed by the fundamental npc actions made available outside
                 * the AI framework.
                 *
                 * @param turn_based true if turn-based mode is active, false for real-time mode
                 * @param manipulated_object Encapsulation of around object(s) a plan may manipulate.
                 */
                virtual void execute(bool turn_based, PlanInputData& manipulated_object) = 0;
        };
    }
}

#endif

