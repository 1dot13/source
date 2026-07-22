#include "../include/BoxerPlan.h"
#include "../../TacticalAI/ai.h"

namespace AI
{
    namespace tactical
    {
        LegacyAIBoxerPlan::LegacyAIBoxerPlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }


        void LegacyAIBoxerPlan::execute(PlanInputData& environment)
        {
            switch (get_npc()->aiData.bAlertStatus)
            {
                case STATUS_GREEN:
                    get_npc()->aiData.bAction = DecideActionGreenBoxer(get_npc());
                    break;
                case STATUS_YELLOW:
                    get_npc()->aiData.bAction = DecideActionGreenBoxer(get_npc());
                    break;
                case STATUS_RED:
                    get_npc()->aiData.bAction = DecideActionBlackBoxer(get_npc());
                    break;
                case STATUS_BLACK:
                    get_npc()->aiData.bAction = DecideActionBlackBoxer(get_npc());
                    break;
            }
        }

    } // namespace tactical
} // namespace AI

