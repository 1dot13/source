/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/PlanFactoryLibrary.h"
#include "../include/NullPlanFactory.h"
#include "../include/LegacyAIPlanFactory.h"
// XXX Add includes for new factories here XXX


#include "../../Utils/INIReader.h"

#undef max // Who the fuck writes MACROS not using CAPITAL LETTERS??? You, sir, have lost your coding license. Please hand over your compiler and leave. Now.

#include <stdexcept>
#include <limits>
#include <string>
#include <sstream>

namespace AI
{
    namespace tactical
    {
        PlanFactoryLibrary* PlanFactoryLibrary::instance_ = 0;

        /*@brief Initialize all PlanFactories and map them to the slots defined by the configuration file
          *
          * This is the place where new factories need to be "registred" in order for them to be available using the
          * settings in AI.ini 
          */
        PlanFactoryLibrary::PlanFactoryLibrary()
        {

            // ================================================================================
            // XXX vv Add new factory registrations here vv XXX

            registred_factories_[NullPlanFactory::get_name()] = new NullPlanFactory();
            registred_factories_[LegacyAIPlanFactory::get_name()] = new LegacyAIPlanFactory();

            // XXX ^^ Add new factory registrations here ^^ XXX
            // ================================================================================

		    CIniReader ini_reader("AI.ini", true);
            size_t num_slots = ini_reader.ReadInteger("Modularized Tactical AI", "NumFactories", 0, 0, std::numeric_limits<int>::max());
            for(size_t i(0); i<num_slots; ++i)
            {
                std::stringstream slot_index;
                slot_index<<"Factory_"<<i;
                std::string factory_name = ini_reader.ReadString("Modularized Tactical AI", const_cast<char*>(slot_index.str().c_str()), "LegacyAIFactory");
                ai_index_to_factory_mapping_.push_back(registred_factories_[factory_name]);
                if(!ai_index_to_factory_mapping_.back()->initialize_called())
                    ai_index_to_factory_mapping_.back()->initialize();
            }
        }

        /** @brief If no instance exists yet, create it. Return a pointer to the only existing instance of this library.
         *  @return Pointer to the only instance of the PlanFactoryLibrary
         */
        PlanFactoryLibrary* PlanFactoryLibrary::instance()
        {
            if(instance_ == 0)
                instance_ = new PlanFactoryLibrary();
            return instance_;
        }

        /**@brief Create a plan for the given input using the factory at the given index
         * @param index Index of the concrete plan factory to use, set in the constructor via ini settings
         * @param input The environmental data required for a factory to plan
         * @throws std::out_of_range for invalid index
         * @throws std::logic_error for valid index, but undefined factory (most likely due to typo in AI.ini)
         * @return Pointer to the only instance of the PlanFactoryLibrary
         */
        Plan* PlanFactoryLibrary::create_plan(size_t index, AIInputData& input) const
        {
            if(index >= ai_index_to_factory_mapping_.size())
                throw std::out_of_range("PlanFactoryLibrary detected invalid factory index");
            if(!ai_index_to_factory_mapping_[index])
                throw std::logic_error("PlanFactoryLibrary encountered a nullptr for a valid index (typo in AI.ini?)");
            return ai_index_to_factory_mapping_[index]->create_plan(input);
        }
    }
}

