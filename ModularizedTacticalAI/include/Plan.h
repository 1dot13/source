/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef PLAN_H_
#define PLAN_H_

#include <iostream>

class SOLDIERTYPE;
struct TacticalStatusType;

namespace AI
{
    namespace tactical
    {
        /**@class PlanInputData
         * @brief Wrapper class for environmental data required for plan execution
         * 
         * Future versions might require other data types; without the wrapper, *every* Plan subclass ever created would
         * need to be changed in this event. With the wrapper, no change is required at all (see also AIInputData).
         */
        class PlanInputData
        {
            private:
                /// True iff we are in turn based mode
                bool turn_based_;
                /// Reference to the global variable gTacticalStatus (we want to keep this place tidy and don't use global variables here)
                const TacticalStatusType& tactical_status_;
                /// left undefined to prevent copying
                PlanInputData(const TacticalStatusType&);
                /// left undefined to prevent assignment
                PlanInputData& operator=(const TacticalStatusType&);
            public:
                /// Trivial member initialization
                PlanInputData(bool turn_based, const TacticalStatusType& tactical_status);
                /** @name Accessor functions */ /*@{*/
                /// Return a reference to a global "tactical status" struct, formerly known as gTacticalStatus
                const TacticalStatusType& get_tactical_status() const { return tactical_status_; }
                bool turn_based() const { return turn_based_; }
                /*@}*/
        };


        /**@class Plan
         * @brief Composite/Abstract Product. Base class for all plan compositions and components.
         * 
         * The Plan class provides a common interface for both plan compositions and plan components. The composite
         * design pattern chosen for plan representation allows objects inheriting from this class to form trees,
         * allowing a clean and easy representation of sub- and super-plans
         */
        class Plan
        {
            private:
                /// A pointer to the NPC who owns this plan.
                SOLDIERTYPE* npc_;
            protected:
                /// Return a pointer to the controlled NPC
                SOLDIERTYPE* get_npc() {return npc_;}
                /// Return a pointer to the controlled NPC
                const SOLDIERTYPE* get_npc() const {return npc_;}
            public:
                Plan(SOLDIERTYPE* npc);
                /// A virtual destructor is essential here, destruction will be handled through this interface.
                virtual ~Plan() { };
                /** @brief Plan execution is the encapsulation of an "action sequence", which can be used to make up
                 * higher-level building blocks
                 *
                 * Each plan objects represents a high-level action, implemented by means of lower-level actions. These
                 * lower level actions can be other Plans subtypes; at some point, a Plan object has no further
                 * sub-plans, i.e., the objet is no composition, but a component, forming a leaf in the Plan hierarchy.
                 * The components' lower level actions are formed by the fundamental NPC actions which must be made
                 * available outside the AI framework.
                 *
                 * @param environment Encapsulation of environmental data required for plan execution
                 */
                virtual void execute(PlanInputData& environment) = 0;
                /** Determine if the plan is finished.
                  *
                  * The semantics are not "this plan *can* be aborted" - every plan must anticipate that - but instead
                  * "this plan cannot be continued"; for example, a movement plan is finished once the destination is
                  * reached, as no further movement can be carried out by the plan.
                  *@return true iff the plan can no longer be executed (or execution will always lead to AI_ACTION_NONE)
                  */
                virtual bool done() const = 0;
        };
    }
}

#endif

