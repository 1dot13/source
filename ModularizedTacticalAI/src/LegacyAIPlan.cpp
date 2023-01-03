/**
 * @file
 * @author feynman (bears-pit.com)
 */

#include "../include/LegacyAIPlan.h"

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
#include "../../Utils/Text.h"                   // Sniper warning
#include "../../Utils/message.h"                // ditto


namespace AI
{
    namespace tactical
    {
        LegacyAIPlan::LegacyAIPlan(SOLDIERTYPE* npc)
            : Plan(npc)
        {
        }


        void LegacyAIPlan::execute(PlanInputData& environment)
        {
            if(!environment.turn_based())
            {
                if ( (get_npc()->ubProfile != NO_PROFILE) && (gMercProfiles[ get_npc()->ubProfile ].ubMiscFlags3 & PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL ) )
                {
                    TriggerNPCWithGivenApproach( get_npc()->ubProfile, APPROACH_DONE_TRAVERSAL, FALSE );
                    gMercProfiles[ get_npc()->ubProfile ].ubMiscFlags3 &= (~PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL);
                    get_npc()->ubQuoteActionID = 0;
                    // wait a tiny bit
                    get_npc()->aiData.usActionData = 100;
                    get_npc()->aiData.bAction =  AI_ACTION_WAIT;
                    return;
                }
                if (get_npc()->bTeam == gbPlayerNum)
                {
                    if (environment.get_tactical_status().fAutoBandageMode)
                    {
                        get_npc()->aiData.bAction = DecideAutoBandage( get_npc() );
                        return;
                    }
                }
            }

            if ( get_npc()->bTeam != MILITIA_TEAM )
            {
                if ( !sniperwarning && get_npc()->aiData.bOrders == SNIPER )
                {
                    ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_WATHCHOUTFORSNIPERS] );
                    sniperwarning = TRUE;

					// Flugente: additional dialogue
					AdditionalTacticalCharacterDialogue_AllInCurrentSector( NO_PROFILE, ADE_SNIPERWARNING );
                }

                if (!biggunwarning && FindRocketLauncherOrCannon(get_npc()) != NO_SLOT )
                {
                    biggunwarning = TRUE;
                    //TODO: don't say this again after reloading a savegame
                    SayQuoteFromAnyBodyInSector( QUOTE_WEARY_SLASH_SUSPUCIOUS );
                }
            }
            get_npc()->aiData.fAIFlags &= (~AI_CAUTIOUS); // turn off cautious flag
            // if status override is set, bypass RED/YELLOW and go directly to GREEN!
            if ((get_npc()->aiData.bBypassToGreen) && (get_npc()->aiData.bAlertStatus < STATUS_BLACK))
            {
                get_npc()->aiData.bAction = DecideActionGreen(get_npc());
                if ( !gfTurnBasedAI )
                {
                    // reset bypass now
                    get_npc()->aiData.bBypassToGreen = 0;
                }
            }
            else
            {
                switch (get_npc()->aiData.bAlertStatus)
                {
                    case STATUS_GREEN:
                        get_npc()->aiData.bAction = DecideActionGreen(get_npc());
                        break;
                    case STATUS_YELLOW:
                        get_npc()->aiData.bAction = DecideActionYellow(get_npc());
                        break;
                    case STATUS_RED:
                        get_npc()->aiData.bAction = DecideActionRed(get_npc());
                        break;
                    case STATUS_BLACK:
                        get_npc()->aiData.bAction = DecideActionBlack(get_npc());
                        break;
                }
            }
            DEBUGAIMSG("Deciding for guynum "<<(int)get_npc()->ubID<<" at gridno "<<get_npc()->sGridNo<<", APs "<<get_npc()->bActionPoints<<
                    ", decided action: "<<(int)get_npc()->aiData.bAction<<", data "<<(int)get_npc()->aiData.usActionData);
        }

    } // namespace tactical
} // namespace AI

