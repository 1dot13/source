#include "../include/RobotPlan.h"
#include "../../TacticalAI/ai.h"

namespace AI
{
    namespace tactical
    {
        RobotPlan::RobotPlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }


        void RobotPlan::execute(PlanInputData& environment)
        {
            switch (get_npc()->aiData.bAlertStatus)
            {
            case STATUS_GREEN:
                get_npc()->aiData.bAction = DecideActionGreenRobot(get_npc());
                break;
            case STATUS_YELLOW:
                get_npc()->aiData.bAction = DecideActionYellowRobot(get_npc());
                break;
            case STATUS_RED:
                get_npc()->aiData.bAction = DecideActionRedRobot(get_npc());
                break;
            case STATUS_BLACK:
                get_npc()->aiData.bAction = DecideActionBlackRobot(get_npc());
                break;
            }
        }

    } // namespace tactical
} // namespace AI
