#include "../include/Plan.h"

#include "../../TacticalAI/ai.h"
#include "../../TacticalAI/AIInternals.h"       // ACTING_ON_SCHEDULE
#include "../../TacticalAI/NPC.h"               // NPCReachedDestination
#include "../../Tactical/Animation Control.h"   // defines ANIM_...
#include "../../Tactical/Soldier Macros.h"      // CREATURE_OR_BLOODCAT
#include "../../Tactical/opplist.h"             // EndMuzzleFlash
#include "../../Tactical/Dialogue Control.h"    // DialogueQueueIsEmpty
#include "../../TileEngine/Isometric Utils.h"   // defines NOWHERE
#include "../../Utils/Debug Control.h"          // LiveMessage
#include "../../Utils/Font Control.h"           // ScreenMsg about deadlock
#include "../../Utils/message.h"                // ditto
#include "../../TileEngine/Render Fun.h"        // defines InARoom
#include "../../Strategic/quests.h"             // IN_BROTHEL

 // FIXME uagh, these need to go
extern BOOLEAN gfUIInDeadlock;
extern UINT8 gUIDeadlockedSoldier;

void HandleAITacticalTraversal(SOLDIERTYPE* pSoldier);  // defined in TacticalAI/AIMain.cpp

namespace AI
{
    namespace tactical
    {
        PlanInputData::PlanInputData(bool turn_based, const TacticalStatusType& tactical_status)
            : turn_based_(turn_based),
              tactical_status_(tactical_status)
        {
        }

        Plan::Plan(SOLDIERTYPE* npc)
            : npc_(npc)
        {
        }

    }
}

