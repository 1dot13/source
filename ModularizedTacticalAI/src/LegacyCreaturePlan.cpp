/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/LegacyCreaturePlan.h"
#include "../../Tactical/Soldier Control.h"

INT8 CreatureDecideAction( SOLDIERTYPE *pSoldier );

namespace AI
{
    namespace tactical
    {
        LegacyCreaturePlan::LegacyCreaturePlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }

        void LegacyCreaturePlan::execute(PlanInputData& environment)
        {
            get_npc()->aiData.bAction = CreatureDecideAction( get_npc() );
        }
    }
}

