/**
* @file
* @author Flugente (bears-pit.com)
*/

#ifndef LEGACY_ARMED_VEHICLE_PLAN_H_
#define LEGACY_ARMED_VEHICLE_PLAN_H_

#include "Plan.h"

namespace AI
{
	namespace tactical
	{
		/**@class LegacyarmedVehiclePlan
		* @brief Component/Concrete Product. Wrapper/Re-Write of ZombieDecideAction()
		*
		* This plan began as a simple forwarding object for ZombieDecideAction(), and will, in the course of the AI redesign,
		* be split into elementary NPC actions. It is as such only a intermediate product used to bring structure where
		* the is currently none.
		*/
		class LegacyArmedVehiclePlan : public Plan
		{
		private:
		public:
			LegacyArmedVehiclePlan( SOLDIERTYPE* npc );
			virtual void execute( PlanInputData& environment );
			virtual bool done( ) const { return false; }
		};
	}
}

#endif	// LEGACY_ARMED_VEHICLE_PLAN_H_
