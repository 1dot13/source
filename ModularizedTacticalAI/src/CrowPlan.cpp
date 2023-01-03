/** 
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/CrowPlan.h"
#include "../../Tactical/Soldier Control.h"     // defines SOLDIERTYPE
#include "../../Tactical/Animation Control.h"   // defines CROW_FLY
#include "../../Tactical/Soldier Add.h"         // FindGridNoFromSweetSpot()
#include "../../TacticalAI/ai.h"                // AI_ACTION_...
#include "../../TacticalAI/AIInternals.h"       // AIDEBUGMSG
#include "../../Tactical/Rotting Corpses.h"     // FindNearestRottingCorpse()
#include "../../TileEngine/Isometric Utils.h"   // TileIsOutOfBounds()

namespace AI
{
    namespace tactical
    {
        CrowFlyAwayPlan::CrowFlyAwayPlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }

        void CrowFlyAwayPlan::execute(PlanInputData& environment)
        {
            CrowsFlyAway( get_npc()->bTeam );
        }

        CrowSeekCorpsePlan::CrowSeekCorpsePlan(SOLDIERTYPE* npc)
            : Plan(npc),
              corpse_grid_(FindNearestRottingCorpse(npc))
        {
        }

        void CrowSeekCorpsePlan::execute(PlanInputData& environment)
        {
            get_npc()->aiData.bAction =  AI_ACTION_NONE;
            if(TileIsOutOfBounds(corpse_grid_))
                return;
            // Walk to nearest corpse
            UINT8 unused;
            get_npc()->aiData.usActionData = FindGridNoFromSweetSpot( get_npc(), corpse_grid_, 4, &unused);
            if(TileIsOutOfBounds(get_npc()->aiData.usActionData))
                return;
            get_npc()->aiData.bAction = AI_ACTION_GET_CLOSER;
        }

        bool CrowSeekCorpsePlan::done() const
        {
            return SpacesAway( get_npc()->sGridNo, corpse_grid_ ) < 2;
        }

        int CrowSeekCorpsePlan::get_corpse_grid() const
        {
            return corpse_grid_;
        }

        CrowPeckPlan::CrowPeckPlan(SOLDIERTYPE* npc, int corpse_grid)
            : Plan(npc), corpse_grid_(corpse_grid)
        {
        }

        void CrowPeckPlan::execute(PlanInputData& environment)
        {
            get_npc()->aiData.bAction =  AI_ACTION_NONE;
            INT16 sFacingDir;
            if (TileIsOutOfBounds(corpse_grid_))
                return;

            // Change facing
            sFacingDir = GetDirectionFromGridNo( corpse_grid_, get_npc() );
            if ( sFacingDir != get_npc()->ubDirection )
            {
                get_npc()->aiData.usActionData = sFacingDir;
                get_npc()->aiData.bAction = AI_ACTION_CHANGE_FACING;
                return;
            }
            if (!environment.turn_based())
            {
                get_npc()->aiData.usActionData = 30000;
                get_npc()->aiData.bAction =  AI_ACTION_WAIT;
            }
        }

    } // namespace tactical
} // namespace AI

