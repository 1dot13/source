#include "../include/CivilianPlan.h"
#include "../../TacticalAI/ai.h"
#include "NPC.h"
#include "Soldier Profile.h"

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
            if ( !environment.turn_based() )
            {
                if ( (get_npc()->ubProfile != NO_PROFILE) && (gMercProfiles[get_npc()->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL) )
                {
                    TriggerNPCWithGivenApproach(get_npc()->ubProfile, APPROACH_DONE_TRAVERSAL, FALSE);
                    gMercProfiles[get_npc()->ubProfile].ubMiscFlags3 &= (~PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL);
                    get_npc()->ubQuoteActionID = 0;
                    // wait a tiny bit
                    get_npc()->aiData.usActionData = 100;
                    get_npc()->aiData.bAction = AI_ACTION_WAIT;
                    return;
                }
            }

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

