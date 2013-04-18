/** 
 * @file
 * @author feynman (bears-pit.com)
 */
#include "../include/LegacyAIPlanFactory.h"
#include "../include/LegacyAIPlan.h"
#include "../include/LegacyCreaturePlan.h"
#include "../include/LegacyZombiePlan.h"
#include "../include/CrowPlan.h"
#include "../include/PlanList.h"

#include "../../TacticalAI/AIInternals.h"      // DEBUGAIMSG
#include "../../Tactical/Soldier Control.h" // For SOLDIERTYPE definition
#include "../../Tactical/Animation Data.h"  // For the definition of, wait for it... BLOODCAT!

#include <stdio.h>


namespace AI
{
    namespace tactical
    {
        Plan* LegacyAIPlanFactory::create_plan(SOLDIERTYPE* npc, const AIInputData& input)
        {
            DEBUGAIMSG("Planning for "<<(int)npc->ubID);
            if((npc->flags.uiStatusFlags & SOLDIER_MONSTER) || npc->ubBodyType == BLOODCAT )
                return new LegacyCreaturePlan(npc);
            if(npc->ubBodyType == CROW)
            {
                PlanList* find_supper = new PlanList(npc);
                CrowSeekCorpsePlan* seek_corpse = new CrowSeekCorpsePlan(npc);
                CrowPeckPlan* peck = new CrowPeckPlan(npc, seek_corpse->get_corpse_grid());
                find_supper->add_subplan(seek_corpse);
                find_supper->add_subplan(peck);
                return find_supper;
            }
#ifdef ENABLE_ZOMBIES
            if(npc->IsZombie())
                return new LegacyZombiePlan(npc);
#endif
            return new LegacyAIPlan(npc);               // no special plan for other cases yet, return default legacy AI wrapper
        }


        void LegacyAIPlanFactory::update_plan(SOLDIERTYPE* npc, const AIInputData& input)
        {
            DEBUGAIMSG("Update called for "<<(int)npc->ubID<<" event: "<<input);
            if(!npc->ai_masterplan_)
                npc->ai_masterplan_ = create_plan(npc, input);
        }
    }
}

