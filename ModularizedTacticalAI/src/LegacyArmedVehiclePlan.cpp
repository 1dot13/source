/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "../include/LegacyArmedVehiclePlan.h"
#include "Soldier Control.h" // definition of SOLDIERTYPE
#include "aiinternals.h"

namespace AI
{
	namespace tactical
	{
		LegacyArmedVehiclePlan::LegacyArmedVehiclePlan( SOLDIERTYPE* npc )
			: Plan( npc )
		{
		}

		void LegacyArmedVehiclePlan::execute( PlanInputData& environment )
		{
			get_npc( )->aiData.bAction = ArmedVehicleDecideAction( get_npc( ) );
		}
	}
}

