#include "../include/CivilianPlan.h"
#include "../../TacticalAI/ai.h"

namespace AI
{
    namespace tactical
    {
        CivilianPlan::CivilianPlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }


        void CivilianPlan::execute(PlanInputData& environment)
        {
            switch (get_npc()->aiData.bAlertStatus)
            {
                case STATUS_GREEN:
                    get_npc()->aiData.bAction = DecideActionGreenCivilian(get_npc());
                    break;
                case STATUS_YELLOW:
                    get_npc()->aiData.bAction = DecideActionYellowCivilian(get_npc());
                    break;
                case STATUS_RED:
                    get_npc()->aiData.bAction = DecideActionRedCivilian(get_npc());
                    break;
                case STATUS_BLACK:
                    get_npc()->aiData.bAction = DecideActionBlackCivilian(get_npc());
                    break;
            }
        }

    } // namespace tactical
} // namespace AI

