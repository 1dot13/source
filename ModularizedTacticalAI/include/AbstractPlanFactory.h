/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#ifndef ABSTRACT_PLAN_FACTORY_H_
#define ABSTRACT_PLAN_FACTORY_H_

#include <iosfwd>

class SOLDIERTYPE;

namespace AI
{
    namespace tactical
    {
        /**@class AIInputData
         * @brief Wrapper class around the environmental data required to build or update an AI plan
         * 
         * The data contained in objects of this class are currently collected in HanldeNoise() and ManSeesMan(), both
         * defined in Tactical/opplist.cpp
         * Future versions might require more or different data; without the wrapper, *every* Concrete Factory ever
         * created would need to be changed in this event. With the wrapper, no change is required at all.
         */
        class AIInputData
        {
            private:
                enum event_type {no_event, auditive_event, visual_event};
                event_type event_type_;
                /// Only visual: the opponent seen by the updated NPC
                SOLDIERTYPE* opponent_;
                /// Only auditive: ID of the noise maker; (at least for NPC-generated sounds) ID of NPC
                int noise_maker_;
                /// Location of event
                int grid_no_;
                int level_;
                /// Only auditive: volume of heard sound (at the hearing NPC, not the source)
                int volume_;
                int type_;
                int caller1_;
                int caller2_;
            public:
                /// Empty types are used for overload resolution in order to generate different types of events with
                /// simple constructor calls.
                struct Auditive{ };
                struct Visual{ };
                /// Constructor for noise events
                AIInputData(Auditive, int noise_maker, int grid_no, int level, int volume, int type);
                /// Constructor for sight events
                AIInputData(Visual, SOLDIERTYPE* opponent, int grid_no, int level, int caller1, int caller2);
                /// Default constructor, called when no event occurred
                AIInputData();
                bool is_auditive_event() const {return event_type_ == auditive_event;}
                bool is_visual_event() const {return event_type_ == visual_event;}
                /// Allow the output function used for debugging to access the members directly
                friend std::ostream& operator<<(std::ostream& os, const AIInputData& i);
        };
        std::ostream& operator<<(std::ostream& os, const AIInputData& i);

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
                /// Flag set iff delayed initialization was already performed
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
                 * @param npc The NPC the plan is for
                 * @param input A reference to a structure containing all required input data in order for the AI
                 * algorithms to perform their task. At the moment, empty. Future versions
                 * might require other datatypes; without the wrapper, *every* Concrete Factory ever created would need
                 * to be changed in this event.
                 * @return A Plan object tree representing the produced strategy
                 */
                virtual Plan* create_plan(SOLDIERTYPE* npc, const AIInputData& input) = 0;

                /**@brief Abstract Plan object hierarchy update function, called in order to update an already created plan
                 *
                 * If no plan exists, it is created; this should not be the general case, but might happen for
                 * 'interrupted' NPCs that haven't had the chance to plan yet.
                 *
                 * @param npc A pointer to the npc owning the ai_masterplan_ to be changed.
                 * @param input A reference to a structure containing all required input data in order for the AI
                 * @post npc->ai_masterplan_ is not null
                 */
                virtual void update_plan(SOLDIERTYPE* npc, const AIInputData& input) = 0;
        };
    }
}

#endif

