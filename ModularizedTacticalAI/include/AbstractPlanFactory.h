/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef ABSTRACT_PLAN_FACTORY_H_
#define ABSTRACT_PLAN_FACTORY_H_

class SOLDIERTYPE;

namespace AI
{
    namespace tactical
    {
        /**@class AIInputData
         * @brief Wrapper class around the environmental data required to build an AI plan
         * 
         * At the moment, simply a wrapper around SOLDIERTYPE. Future versions might require more data; without the
         * wrapper, *every* Concrete Factory ever created would need to be changed in this event. With the wrapper, no
         * change is required at all.
         */
        struct AIInputData
        {
            SOLDIERTYPE* npc_to_plan_for_;
        };
        class Plan;
        /**@class AbstractPlanFactory
         * @brief Abstract Factory. Base class for all plan factories.
         * 
         * The Abstract Factory design pattern is used to create instances of Plan object hierarchies. The concrete
         * factories differ not necessarily in the concrete product they produce (as is otherwise the case when using
         * this design pattern), but in the structure of the compositions (plans are Composites) instead. Public
         * inheritance is required for two reasons: (a) several sub-class instances are packed in a container, and this
         * abstract class serves as the container's data type. And (b) implementation of create_plan() and update_plan()
         * is forced for concrete subclasses.
         */
        class AbstractPlanFactory
        {
            private:
                bool initialize_called_;
            public:
                AbstractPlanFactory() : initialize_called_(false) { }
                bool initialize_called() const { return initialize_called_; }
                /**@brief Used for delayed initialization of ressource-intensive initialization tasks
                 *
                 * Due to the design of the modularized AI, each factory's constructor must be called, even if the
                 * factory is not used. Therefore, memory- and cpu intensive tasks should not be performed in the
                 * constructor, but in this function instead. It will only be called if the factory is actually
                 * referenced in the AI.ini file.
                 */
                virtual void initialize() { };
                /**@brief Abstract Plan object hierarchy creation function
                 *
                 * The subclasses (i.e., Concrete Factories) implpmenting this routine contain
                 * the decision-making AI core.
                 *
                 * @param input A pointer to a structure containing all required input data in order for the AI
                 * algorithms to perform their task. At the moment, simply a wrapper around SOLDIERTYPE. Future versions
                 * might require other datatypes; without the wrapper, *every* Concrete Factory ever created would need
                 * to be changed in this event.
                 * @return A Plan object tree representing the produced strategy
                 */
                virtual Plan* create_plan(const AIInputData& input) = 0;
                /**@brief Abstract Plan object hierarchy update function, called in order to update an already created
                 *         plan
                 *
                 * @param input A pointer to a structure containing all required input data in order for the AI
                 *        algorithms to perform their task
                 * @param plan_to_change The Plan object hierarchy that is to be updated
                 */
                virtual void update_plan(const AIInputData& input, Plan* plan_to_change) = 0;
        };
    }
}

#endif

