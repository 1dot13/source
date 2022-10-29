#pragma optimize("", off)
/*
Rebel Command
by rftr

This is a feature that mostly affects the strategic layer of JA2. While the player takes care of the fighting, this feature allows
the rebels to provide passive bonuses that can help out the player in various ways.

The two new abilities for this feature are national Directives, which are bonuses that are always in effect, and Administrative Actions,
which are regional bonuses that are applied to towns that the player captures.

A new resource, Supplies, is accumulated passively and is used to purchase and improve Administrative Actions.
Directives can be improved with money.

At the start of the campaign, this feature is unavailable, but the player gains access to the ARC website as soon as they complete
the food delivery quest for the rebels.

Missions provide powerful temporary bonuses. To enable these bonuses, Supplies must be spent as well as sending
either a generic rebel agent or one of their own mercenaries, the latter providing better mission bonuses.


How to add a new directive:
- add to the RebelCommandDirectives enum in the header
- add to the RebelCommandDirectivesText enum below
- add strings to text files (szRebelCommandDirectivesText)
- add to GameSettings (gRebelCommandSettings) and RebelCommand_Settings.ini
- add to SetupInfo to read from game settings
- add to dropdown list in EnterWebsite
- add to switch in DailyUpdate (for completeness)
- add to switch in GetDirectiveEffect for website description
- add directive-specific effect

How to add a new admin action:
- add to the RebelCommandAdminActions enum in the header
- add strings to text files (szRebelCommandAdminActionsText)
- add to DailyUpdate (for completeness)
- add to GameSettings (gRebelCommandSettings) and RebelCommand_Settings.ini
- add to SetupInfo to read from game settings
- add to Init to add to pool of valid actions on game start
- add admin-action-specific effect
- if effect applies outside of towns, add help text range band as appropriate to SetupAdminActionBox

How to add a new mission:
- add to the RebelCommandAgentMissions enum in the header
- add strings to text files (szRebelCommandAgentMissionsText)
- add values to MissionHelpers::missionInfo table in SetupInfo()
- add to valid check in HandleStrategicEvent() (allows advance from first event/prepare to second event/active effect)
- add to SetupMissionAgentBox() (mission description and merc bonus text)
- add to PrepareMission()
- add mission-specific functions
- rftr todo: ???

Points of interest:
- Init() - set up rebel command for the first time
- SetupInfo() - set constants
- RenderWebsite() - draw the website
- RebelCommandSaveInfo	- pretty much everything important is in here

*/
#ifdef PRECOMPILEDHEADERS
#include "Laptop All.h"
#include "Strategic All.h"
#else
#include "Rebel Command.h"

#include "ASD.h"
#include "Button System.h"
#include "Campaign.h"
#include "Campaign Types.h"
#include "Cheats.h"
#include "Cursors.h"
#include "DropDown.h"
#include "english.h"
#include "finances.h"
#include "Font Control.h"
#include "Game Clock.h"
#include "Game Event Hook.h"
#include "GameSettings.h"
#include "GameVersion.h"
#include "input.h"
#include "Line.h"
#include "laptop.h"
#include "message.h"
#include "MessageBoxScreen.h"
#include "MilitiaIndividual.h"
#include "mousesystem.h"
#include "Overhead Types.h"
#include "Queen Command.h"
#include "Quests.h"
#include "random.h"
#include "SaveLoadGame.h"
#include "Soldier macros.h"
#include "Squads.h"
#include "strategic.h"
#include "strategicmap.h"
#include "Strategic Mines.h"
#include "Strategic Movement.h"
#include "Strategic Town Loyalty.h"
#include "Structure Wrap.h"
#include "Tactical Save.h"
#include "Text.h"
#include "Town Militia.h"
#include "Utilities.h"
#include "Vehicles.h"
#include "WCheck.h"
#include "WordWrap.h"
#endif

#include <unordered_map>
#include <unordered_set>

#define		DIRECTIVE_TEXT(id)		RCDT_##id##, RCDT_##id##_EFFECT, RCDT_##id##_DESC, RCDT_##id##_IMPROVE,
#define		MISSION_TEXT(id)		RCAMT_##id##_TITLE, RCAMT_##id##_DESC,

#define		ADMIN_ACTION_CHANGE_COST	15000

#define		REBEL_COMMAND_DROPDOWN		DropDownTemplate<DROPDOWN_REBEL_COMMAND_DIRECTIVE>::getInstance()

#define		WEBSITE_LEFT	LAPTOP_SCREEN_UL_X
#define		WEBSITE_TOP		LAPTOP_SCREEN_WEB_UL_Y + 3
#define		WEBSITE_WIDTH	500
#define		WEBSITE_HEIGHT	395

extern UINT32 gCoolnessBySector[256];
extern BOOLEAN gfTownUsesLoyalty[MAX_TOWNS];
extern GROUP *gpGroupList;

namespace RebelCommand
{

namespace ItemIdCache
{
	// cache these values on load so that we don't need to search for them every time something happens
	std::vector<UINT16> gasCans;
	std::vector<UINT16> firstAidKits;
	std::vector<UINT16> medKits;
	std::vector<UINT16> toolKits;
	std::vector<UINT16> ammo[10]; // coolness

	void Clear()
	{
		gasCans.clear();
		firstAidKits.clear();
		medKits.clear();
		toolKits.clear();
		for (int i = 0; i < 10; ++i)
		{
			ammo[i].clear();
		}
	}
}

namespace MissionHelpers
{
constexpr UINT16 DEEP_DEPLOYMENT_RANGE_BONUS_COVERT		= 1;
constexpr UINT16 DEEP_DEPLOYMENT_RANGE_BONUS_SCOUTING	= 2;
constexpr UINT16 DEEP_DEPLOYMENT_RANGE_BONUS_STEALTHY	= 3;
constexpr UINT16 DEEP_DEPLOYMENT_RANGE_BONUS_SURVIVAL	= 4;
constexpr UINT16 DISRUPT_ASD_STEAL_FUEL = 1;
constexpr UINT16 DISRUPT_ASD_DESTROY_RESERVES = 2;
constexpr UINT16 REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_COVERT	= 1;
constexpr UINT16 REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_DEPUTY	= 2;
constexpr UINT16 REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_SNITCH	= 3;
constexpr UINT16 REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_COVERT		= 1;
constexpr UINT16 REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_RADIO		= 2;
constexpr UINT16 REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_STEALTHY	= 3;
constexpr UINT16 SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_AUTO_WEAPONS	= 1;
constexpr UINT16 SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_COVERT			= 2;
constexpr UINT16 SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_DEMOLITIONS		= 3;
constexpr UINT16 SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_GUNSLINGER		= 4;
constexpr UINT16 SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_RANGER			= 5;
constexpr UINT16 SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_SNIPER			= 6;
constexpr UINT16 SABOTAGE_MECHANICAL_UNITS_COVERT			= 1;
constexpr UINT16 SABOTAGE_MECHANICAL_UNITS_DEMOLITIONS		= 2;
constexpr UINT16 SABOTAGE_MECHANICAL_UNITS_HEAVY_WEAPONS	= 3;
constexpr UINT16 TRAIN_MILITIA_ANYWHERE_TEACHING	= 1;
constexpr UINT16 SOLDIER_BOUNTIES_KINGPIN_OFFICER_PAYOUTS			= 1;
constexpr UINT16 SOLDIER_BOUNTIES_KINGPIN_VEHICLE_PAYOUTS			= 2;


typedef struct {
	std::vector<INT8> newSkills;
	std::vector<INT8> oldSkills;
	std::vector<UINT32> durationBonuses;
	std::vector<FLOAT> floatModifiers;
	std::vector<INT16> intModifiers;
	std::vector<UINT16> extraBits;
} MissionInfo;

// this vector serves as a comparison table to determine which bonuses will be applied to a mission
// see SetupInfo() for initialisation
std::vector<MissionInfo> missionInfo;

// cached param for firing the preparation/first event. used in the mission start confirmation popup callback
UINT32 missionParam;

void GetMissionInfo(RebelCommandAgentMissions mission, const MERCPROFILESTRUCT* merc, UINT32& durationBonus, FLOAT& floatModifier, INT16& intModifier, int& durationBonusSkill, int& floatModifierSkill, int& intModifierSkill, UINT16& extraBits)
{
	durationBonus = 0;
	durationBonusSkill = 0;
	floatModifier = 0;
	floatModifierSkill = 0;
	intModifier = 0;
	intModifierSkill = 0;
	extraBits = 0;

	if (merc == nullptr)
		return;

	if (mission == RCAM_NONE)
		return;

	const std::vector<INT8>* skills = gGameOptions.fNewTraitSystem ? &missionInfo[mission].newSkills : &missionInfo[mission].oldSkills;

	for (int i = 0; i < sizeof(merc->bSkillTraits) / sizeof(merc->bSkillTraits[0]); ++i)
	{
		for (size_t j = 0; j < skills->size(); ++j)
		{
			if (merc->bSkillTraits[i] == (*skills)[j])
			{
				if (missionInfo[mission].durationBonuses[j] > durationBonus)
				{
					durationBonus = missionInfo[mission].durationBonuses[j];
					durationBonusSkill = (*skills)[j];
				}
				
				if (missionInfo[mission].floatModifiers[j] > floatModifier)
				{
					floatModifier = missionInfo[mission].floatModifiers[j];
					floatModifierSkill = (*skills)[j];
					extraBits = missionInfo[mission].extraBits[j];
				}
				
				if (missionInfo[mission].intModifiers[j] > intModifier)
				{
					intModifier = missionInfo[mission].intModifiers[j];
					intModifierSkill = (*skills)[j];
					extraBits = missionInfo[mission].extraBits[j];
				}
			}
		}
	}
}

}

void DEBUG_DAY();

enum WebsiteState
{
	RCS_NATIONAL_OVERVIEW,
	RCS_REGIONAL_OVERVIEW,
	RCS_AGENT_OVERVIEW,
};

enum RebelCommandText // keep this synced with szRebelCommandText in the text files
{
	RCT_NATIONAL_OVERVIEW = 0,
	RCT_REGIONAL_OVERVIEW,
	RCT_AGENT_OVERVIEW,
	RCT_SELECT_VIEW,
	RCT_SWITCH_TO_REGIONAL,
	RCT_SWITCH_TO_NATIONAL,
	RCT_SWITCH_TO_AGENT,
	RCT_SUPPLIES,
	RCT_INCOMING_SUPPLIES,
	RCT_INTEL,
	RCT_PER_DAY,
	RCT_CURRENT_DIRECTIVE,
	RCT_IMPROVE_DIRECTIVE,
	RCT_IMPROVE_DIRECTIVE_PROMPT,
	RCT_INSUFFICIENT_FUNDS,
	RCT_DIRECTIVE_EFFECT_TIME,
	RCT_MILITIA_OVERVIEW,
	RCT_MILITIA_GREEN,
	RCT_MILITIA_REGULAR,
	RCT_MILITIA_ELITE,
	RCT_MILITIA_TOTAL,
	RCT_MILITIA_VOLUNTEERS,
	RCT_MILITIA_RESOURCES,
	RCT_MILITIA_RESOURCE_GUNS,
	RCT_MILITIA_RESOURCE_ARMOUR,
	RCT_MILITIA_RESOURCE_MISC,
	RCT_MILITIA_TRAINING_COST,
	RCT_MILITIA_UPKEEP_COST,
	RCT_MILITIA_TRAINING_SPEED_BONUS,
	RCT_MILITIA_COMBAT_BONUSES,
	RCT_MILITIA_PHYSICAL_BONUS,
	RCT_MILITIA_MARKSMANSHIP_BONUS,
	RCT_MILITIA_UPGRADE_STATS,
	RCT_MILITIA_UPGRADE_STATS_PROMPT,
	RCT_REGION,
	RCT_NEXT,
	RCT_PREV,
	RCT_ADMIN_TEAM,
	RCT_NONE,
	RCT_ACTIVE,
	RCT_INACTIVE,
	RCT_LOYALTY,
	RCT_LOYALTY_MAX,
	RCT_DEPLOY_ADMIN_TEAM,
	RCT_REACTIVATE_ADMIN_TEAM,
	RCT_CANNOT_DEPLOY_ADMIN_TEAM,
	RCT_ADMIN_ACTIONS_TUTORIAL1,
	RCT_ADMIN_ACTIONS_TUTORIAL2,
	RCT_ADMIN_ACTIONS_TUTORIAL3,
	RCT_ADMIN_ACTIONS,
	RCT_ADMIN_ACTION_ESTABLISH,
	RCT_ADMIN_ACTION_IMPROVE,
	RCT_ADMIN_ACTION_TIER,
	RCT_ADMIN_ACTION_COST,
	RCT_DEAD_DROP_INCOME,
	RCT_SMUGGLER_INCOME,
	RCT_BONUS_MILITIA_JOINED,
	RCT_WEBSITE_AVAILABLE,
	RCT_NOT_SAFE_TO_REACTIVATE_ADMIN_TEAM,
	RCT_MINE_RAID_SUCCESSFUL,
	RCT_INSUFFICIENT_INTEL_TO_CREATE_TURNCOATS,
	RCT_CHANGE_ADMIN_ACTION,
	RCT_CANCEL,
	RCT_CONFIRM,
	RCT_PREV_ARROW,
	RCT_NEXT_ARROW,
	RCT_CONFIRM_CHANGE_ADMIN_ACTION_PROMPT,
	RCT_INSUFFICIENT_SUPPLIES_ADMIN_ACTIONS_DISABLED,
	RCT_NEW_MISSIONS_AVAILABLE_TIME,
	RCT_NEW_MISSIONS_AVAILABLE_NOTIFICATION,
	RCT_MISSION_PREP_IN_PROGRESS,
	RCT_MISSION_DURATION_DAYS,
	RCT_MISSION_SUCCESS_CHANCE,
	RCT_MISSION_AGENT_REDACTED,
	RCT_MISSION_AGENT_NAME,
	RCT_MISSION_AGENT_LOCATION,
	RCT_MISSION_AGENT_ASSIGNMENT,
	RCT_MISSION_AGENT_CONTRACT_DAYS,
	RCT_MISSION_AGENT_CONTRACT_HOURS,
	RCT_MISSION_AGENT_CONTRACT_NONE,
	RCT_MISSION_AGENT_BONUS,
	RCT_MISSION_BONUS_SUCCESS_CHANCE,
	RCT_MISSION_BONUS_DEPLOY_RANGE,
	RCT_MISSION_BONUS_ASD_INCOME_REDUCTION,
	RCT_MISSION_BONUS_STEAL_FUEL,
	RCT_MISSION_BONUS_DESTROY_RESERVES,
	RCT_MISSION_BONUS_DECISION_TIME,
	RCT_MISSION_BONUS_UNALERTED_VISION_PENALTY,
	RCT_MISSION_BONUS_INFANTRY_GEAR_QUALITY,
	RCT_MISSION_BONUS_MECHANICAL_STAT_LOSS,
	RCT_MISSION_BONUS_MAX_TRAINERS,
	RCT_MISSION_BONUS_PAYOUT,
	RCT_MISSION_BONUS_PAYOUT_LIMIT_INCREASE,
	RCT_MISSION_BONUS_OFFICER_PAYOUT,
	RCT_MISSION_BONUS_VEHICLE_PAYOUT,
	RCT_MISSION_BONUS_DURATION,
	RCT_MISSION_CANT_START_NOT_IN_TOWN,
	RCT_MISSION_CANT_START_LOW_LOYALTY,
	RCT_MISSION_CANT_START_AGENT_UNAVAILABLE,
	RCT_MISSION_CANT_START_CONTRACT_EXPIRING,
	RCT_MISSION_CANT_USE_REBEL_AGENT,
	RCT_MISSION_CANT_START_BATTLE_IN_PROGRESS,
	RCT_MISSION_START_BUTTON,
	RCT_MISSION_VIEW_ACTIVE,
	RCT_MISSION_VIEW_LIST,
	RCT_MISSION_HELP_1,
	RCT_MISSION_HELP_2,
	RCT_MISSION_HELP_3,
	RCT_MISSION_NEXT_AVAILABILITY,
	RCT_MISSION_ACTIVE_MISSIONS,
	RCT_MISSION_LIST_PREPARING,
	RCT_MISSION_LIST_ACTIVE,
	RCT_MISSION_POPUP_PART1,
	RCT_MISSION_POPUP_PART2_GENERIC,
	RCT_MISSION_POPUP_PART2_MALE,
	RCT_MISSION_POPUP_PART2_FEMALE,
	RCT_MISSION_SUCCESS,
	RCT_MISSION_FAILURE,
	RCT_MISSION_EXPIRED,

};

enum RebelCommandHelpText // keep this synced with szRebelCommandHelpText in the text files
{
	RCHT_SUPPLIES = 0,
	RCHT_SUPPLIES_INCOME,
	RCHT_DIRECTIVES,
	RCHT_ADMIN_TEAM,
	RCHT_LOYALTY,
	RCHT_MAX_LOYALTY,
	RCHT_AA_TOWN_ONLY,
	RCHT_AA_TOWN_PLUS_ONE,
	RCHT_AA_TOWN_PLUS_TWO,
	RCHT_AA_TOWN_PLUS_THREE,
};

// this must be kept in the same order as RebelCommandDirectives in the header
enum RebelCommandDirectivesText // keep this synced with szRebelCommandDirectivesText in the text files
{
	RCDT_FIRST = -1,
	DIRECTIVE_TEXT(GATHER_SUPPLIES)
	DIRECTIVE_TEXT(SUPPORT_MILITIA)
	DIRECTIVE_TEXT(TRAIN_MILITIA)
	DIRECTIVE_TEXT(CREATE_PROPAGANDA)
	DIRECTIVE_TEXT(ELITE_MILITIA)
	DIRECTIVE_TEXT(HVT_STRIKES)
	DIRECTIVE_TEXT(SPOTTERS)
	DIRECTIVE_TEXT(RAID_MINES)
	DIRECTIVE_TEXT(CREATE_TURNCOATS)
	DIRECTIVE_TEXT(DRAFT)
};

enum RebelCommandAgentMissionsText // keep this synced with szRebelCommandAgentMissionsText in the text files
{
	MISSION_TEXT(DEEP_DEPLOYMENT)
	MISSION_TEXT(DISRUPT_ASD)
	MISSION_TEXT(GET_ENEMY_MOVEMENT_TARGETS)
	MISSION_TEXT(IMPROVE_LOCAL_SHOPS)
	MISSION_TEXT(REDUCE_STRATEGIC_DECISION_SPEED)
	MISSION_TEXT(REDUCE_UNALERTED_ENEMY_VISION)
	MISSION_TEXT(SABOTAGE_INFANTRY_EQUIPMENT)
	MISSION_TEXT(SABOTAGE_MECHANICAL_UNITS)
	MISSION_TEXT(SEND_SUPPLIES_TO_TOWN)
	MISSION_TEXT(SOLDIER_BOUNTIES_KINGPIN)
	MISSION_TEXT(TRAIN_MILITIA_ANYWHERE)
};

enum ChangeAdminActionState
{
	CAAS_INIT,
	CAAS_CHANGING,
};

enum MissionOverviewSubview
{
	MOS_MISSION_LIST,
	MOS_ACTIVE_MISSION_EFFECTS,
	MOS_HELP,
};

struct MissionFirstEvent
{
	BOOLEAN isFirstEvent;
	BOOLEAN sentGenericRebelAgent;
	BOOLEAN isMissionSuccess;
	UINT8 mercProfileId;
	UINT8 missionId;
	UINT8 missionDurationInHours;
	UINT8 extraBits;
};

struct MissionSecondEvent
{
	BOOLEAN isSecondEvent;
	BOOLEAN sentGenericRebelAgent;
	UINT8 mercProfileId;
	UINT8 missionId;
	UINT16 extraBits;
};

// serialisation/deserialisation functions for passing information into a strategic event param
UINT32 SerialiseMissionFirstEvent(BOOLEAN sentGenericRebelAgent, UINT8 mercProfileId, RebelCommandAgentMissions mission, UINT8 missionDuration, UINT8 extraBits)
{
	UINT32 ret = 0x00000000;

	if (!sentGenericRebelAgent)
		ret |= 0x01000000;

	ret |= (mercProfileId << 16);
	ret |= (static_cast<UINT8>(mission) << 8);
	ret |= missionDuration;

	// extraBits can only be 6 bits
	extraBits &= 0x3F;

	ret |= (extraBits << 25);

	return ret;
}

void DeserialiseMissionFirstEvent(UINT32 param, MissionFirstEvent& evt)
{
	evt.isFirstEvent =           ((param >> 31) & 0x00000001) == 0;
	evt.sentGenericRebelAgent =  ((param >> 24) & 0x00000001) == 0;
	evt.isMissionSuccess =        (param        & 0x000000FF) > 0;
	evt.mercProfileId =          ((param >> 16) & 0x000000FF);
	evt.missionId =              ((param >> 8)  & 0x000000FF);
	evt.missionDurationInHours =  (param        & 0x000000FF);
	evt.extraBits =              ((param >> 25) & 0x0000003F);
}

UINT32 SerialiseMissionSecondEvent(BOOLEAN sentGenericRebelAgent, UINT8 mercProfileId, RebelCommandAgentMissions mission, UINT16 extraBits)
{
	UINT32 ret = 0x80000000;

	if (!sentGenericRebelAgent)
		ret |= 0x00010000;

	ret |= (mercProfileId << 8);
	ret |= static_cast<UINT8>(mission);

	// extraBits can only be 14 bits
	extraBits &= 0x3FFF;

	ret |= (extraBits << 17);

	return ret;
}

void DeserialiseMissionSecondEvent(UINT32 param, MissionSecondEvent& evt)
{
	evt.isSecondEvent =         ((param >> 31) & 0x00000001) == 1;
	evt.sentGenericRebelAgent = ((param >> 16) & 0x00000001) == 0;
	evt.mercProfileId =         ((param >> 8)  & 0x000000FF);
	evt.missionId =              (param        & 0x000000FF);
	evt.extraBits =             ((param >> 17) & 0x0003FFF);
}

// website functions
template<typename voidFunc>
void ButtonHelper(GUI_BUTTON* btn, INT32 reason, voidFunc onClick);
INT32 CalcIncomingSuppliesPerDay(RebelCommandDirectives directive);
void ClearAllButtons();
void ClearAllHelpTextRegions();
void DeployOrReactivateAdminTeam(INT16 regionId);
void DropdownSetup();
void GetDirectiveEffect(const RebelCommandDirectives directive, STR16 text);
INT32 GetDirectiveImprovementCost(const RebelCommandDirectives directive);
INT32 GetMissionCost();
INT8 GetMissionSuccessChanceBonus(const MERCPROFILESTRUCT* merc);
void ImproveDirective(const RebelCommandDirectives directiveId);
void PurchaseAdminAction(INT32 regionId, INT32 actionIndex);
void RegionNavNext();
void RegionNavPrev();
void RenderHeader(RebelCommandText titleText);
void RenderNationalOverview();
void RenderRegionalOverview();
void RenderMissionOverview();
void SetDirectiveDescriptionHelpText(INT32 reason, MOUSE_REGION& region, RebelCommandDirectives text);
void SetRegionHelpText(INT32 reason, MOUSE_REGION& helpTextRegion, RebelCommandHelpText text);
void SetupAdminActionBox(const UINT8 actionIndex, const UINT16 descriptionText, const UINT16 buttonText);
BOOLEAN SetupMissionAgentBox(UINT16 x, UINT16 y, INT8 index);
void SetWebsiteView(WebsiteState newState);
void PrepareMission(INT8 index);
void ToggleWebsiteView();
void UpdateAdminActionChangeList(INT16 regionId);

void ApplyAdditionalASDEffects();
constexpr BOOLEAN CanAdminActionBeToggled(RebelCommandAdminActions action) { return action != RebelCommandAdminActions::RCAA_SUPPLY_LINE; }
BOOLEAN CanAdminActionBeUsed(INT32 regionIndex, INT32 actionIndex);
void CreateItemAtAirport(UINT16 itemId, INT16 status);
INT32 GetAdminActionCostForRegion(INT16 regionId);
INT16 GetAdminActionInRegion(INT16 regionId, RebelCommandAdminActions adminAction);
UINT8 GetRegionLoyalty(INT16 regionId);
void HandleScouting();
void SendSuppliesToTownMission();
INT16 SendSuppliesToTownDurationBonus(const MERCPROFILESTRUCT* merc);
void SetupInfo();
void UpgradeMilitiaStats();

// buttons
std::vector<INT32> btnIds;
ChangeAdminActionState adminActionChangeState;

// help text regions
MOUSE_REGION adminActionActiveTextRegion[5];
MOUSE_REGION adminActionHelpTextRegion[6];
MOUSE_REGION adminTeamHelpTextRegion;
MOUSE_REGION directiveDescriptionHelpTextRegion;
MOUSE_REGION loyaltyHelpTextRegion;
MOUSE_REGION maxLoyaltyHelpTextRegion;
MOUSE_REGION suppliesHelpTextRegion;
MOUSE_REGION suppliesIncomeHelpTextRegion;

BOOLEAN redraw = FALSE;
std::vector<RebelCommandAdminActions> adminActionChangeList;
INT8 adminActionChangeIndex = 0;
Info info;
INT16 iCurrentRegionId = 1;
INT32 iIncomingSuppliesPerDay = 0;
SaveInfo rebelCommandSaveInfo;
WebsiteState websiteState;
INT8 agentIndex[NUM_ARC_AGENT_SLOTS];
std::unordered_map<RebelCommandAgentMissions, UINT32> missionMap;
MissionOverviewSubview missionOverviewSubview = MOS_MISSION_LIST;

// website
template<typename voidFunc>
void ButtonHelper(GUI_BUTTON* btn, INT32 reason, voidFunc onClick)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= (BUTTON_CLICKED_ON);
	}
	else if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP && btn->uiFlags & BUTTON_CLICKED_ON)
	{
		btn->uiFlags &= ~(BUTTON_CLICKED_ON);

		onClick();

		RenderWebsite();
	}
}

INT32 CalcIncomingSuppliesPerDay(RebelCommandDirectives directive)
{
	const INT32 base = static_cast<INT32>(CurrentPlayerProgressPercentage() * gRebelCommandSettings.fIncomeModifier + (directive == RCD_GATHER_SUPPLIES ? rebelCommandSaveInfo.directives[RCD_GATHER_SUPPLIES].GetValue1() : 0));
	const INT32 supplyUpkeep = static_cast<INT32>(gRebelCommandSettings.fIncomeModifier + 0.5f);
	INT32 upkeepCount = 0;

	for (int a = FIRST_TOWN+1; a < NUM_TOWNS; ++a)
	{
		// ignore this region if there is no active admin team
		if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_ACTIVE)
			continue;

		for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
		{
			if (rebelCommandSaveInfo.regions[a].IsActive(b) && rebelCommandSaveInfo.regions[a].GetLevel(b) > 0)
				upkeepCount++;
		}
	}

	return base - upkeepCount * supplyUpkeep;
}

void ClearAllButtons()
{
	for (const auto btnId : btnIds)
	{
		RemoveButton(btnId);
	}
	btnIds.clear();
}

void ClearAllHelpTextRegions()
{
	for (int a = 0; a < 5; a++)
		MSYS_RemoveRegion(&adminActionActiveTextRegion[a]);
	for (int a = 0; a < 6; a++)
		MSYS_RemoveRegion(&adminActionHelpTextRegion[a]);
	MSYS_RemoveRegion(&adminTeamHelpTextRegion);
	MSYS_RemoveRegion(&directiveDescriptionHelpTextRegion);
	MSYS_RemoveRegion(&loyaltyHelpTextRegion);
	MSYS_RemoveRegion(&maxLoyaltyHelpTextRegion);
	MSYS_RemoveRegion(&suppliesHelpTextRegion);
	MSYS_RemoveRegion(&suppliesIncomeHelpTextRegion);
}

void DeployOrReactivateAdminTeam(INT16 regionId)
{
	INT16 numAdminTeams = 0;
	// ignore omerta when calculating admin team deployment costs
	for (int a = FIRST_TOWN + 1; a < NUM_TOWNS; ++a)
		if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_NONE)
			numAdminTeams++;

	if (rebelCommandSaveInfo.regions[regionId].adminStatus == RAS_INACTIVE)
	{
		if (IsTownUnderCompleteControlByEnemy(static_cast<INT8>(regionId)))
		{
			DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_NOT_SAFE_TO_REACTIVATE_ADMIN_TEAM], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
			return;
		}

		const INT32 cost = rebelCommandSaveInfo.regions[regionId].GetAdminReactivateCost(numAdminTeams);
		if (rebelCommandSaveInfo.iSupplies < cost)
			DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		else
		{
			rebelCommandSaveInfo.iSupplies -= cost;
			rebelCommandSaveInfo.regions[regionId].adminStatus = RAS_ACTIVE;
		}
	}
	else
	{
		const INT32 cost = rebelCommandSaveInfo.regions[regionId].GetAdminDeployCost(numAdminTeams);
		if (rebelCommandSaveInfo.iSupplies < cost)
			DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		else
		{
			rebelCommandSaveInfo.iSupplies -= cost;
			rebelCommandSaveInfo.regions[regionId].adminStatus = RAS_ACTIVE;
		}
	}

}

void DropdownSetup()
{
	// set directives list
	std::vector<std::pair<INT16, STR16>> directivesList;
	directivesList.push_back(std::make_pair(RCD_GATHER_SUPPLIES, szRebelCommandDirectivesText[RCDT_GATHER_SUPPLIES]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uSupportMilitiaProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_SUPPORT_MILITIA, szRebelCommandDirectivesText[RCDT_SUPPORT_MILITIA]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uTrainMilitiaProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_TRAIN_MILITIA, szRebelCommandDirectivesText[RCDT_TRAIN_MILITIA]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uCreatePropagandaProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_CREATE_PROPAGANDA, szRebelCommandDirectivesText[RCDT_CREATE_PROPAGANDA]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uEliteMilitiaProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_ELITE_MILITIA, szRebelCommandDirectivesText[RCDT_ELITE_MILITIA]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uHvtStrikesProgressRequirement && gGameExternalOptions.fEnemyRoles == TRUE)
		directivesList.push_back(std::make_pair(RCD_HVT_STRIKES, szRebelCommandDirectivesText[RCDT_HVT_STRIKES]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uSpottersProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_SPOTTERS, szRebelCommandDirectivesText[RCDT_SPOTTERS]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uRaidMinesProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_RAID_MINES, szRebelCommandDirectivesText[RCDT_RAID_MINES]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uCreateTurncoatsProgressRequirement)
		directivesList.push_back(std::make_pair(RCD_CREATE_TURNCOATS, szRebelCommandDirectivesText[RCDT_CREATE_TURNCOATS]));
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uDraftProgressRequirement && gGameExternalOptions.fMilitiaVolunteerPool == TRUE)
		directivesList.push_back(std::make_pair(RCD_DRAFT, szRebelCommandDirectivesText[RCDT_DRAFT]));

	REBEL_COMMAND_DROPDOWN.SetEntries(directivesList);
	REBEL_COMMAND_DROPDOWN.SetHelpText(szRebelCommandHelpText[RCHT_DIRECTIVES]);
	REBEL_COMMAND_DROPDOWN.SetSelectedEntryKey(rebelCommandSaveInfo.iSelectedDirective);
	REBEL_COMMAND_DROPDOWN.Create(WEBSITE_LEFT + 5, WEBSITE_TOP + 98);
}

BOOLEAN CanAdminActionBeUsed(INT32 regionIndex, INT32 actionIndex)
{
	if (!gGameExternalOptions.fRebelCommandEnabled) return FALSE;

	if (rebelCommandSaveInfo.regions[regionIndex].adminStatus != RAS_ACTIVE) return FALSE;

	if (rebelCommandSaveInfo.regions[regionIndex].GetLevel(actionIndex) == 0) return FALSE;

	if (CanAdminActionBeToggled(rebelCommandSaveInfo.regions[regionIndex].actions[actionIndex]) && !rebelCommandSaveInfo.regions[regionIndex].IsActive(actionIndex)) return FALSE;

	return TRUE;
}

void CreateItemAtAirport(UINT16 itemId, INT16 status)
{
	// create an item - use bobby ray's delivery coordinates/gridno
	OBJECTTYPE obj;
	const BOOLEAN success = CreateItem(itemId, status, &obj);
	if (!success)
	{
		ScreenMsg(FONT_MCOLOR_RED, MSG_INTERFACE, L"Warning - CreateItemAtAirport() failed for itemid %d", itemId);
		return;
	}
	const BOOLEAN airportSectorLoaded = gWorldSectorX == BOBBYR_SHIPPING_DEST_SECTOR_X && gWorldSectorY == BOBBYR_SHIPPING_DEST_SECTOR_Y && gbWorldSectorZ == BOBBYR_SHIPPING_DEST_SECTOR_Z;

	if (airportSectorLoaded)
	{
		SetOpenableStructureToClosed( BOBBYR_SHIPPING_DEST_GRIDNO, 0 );
		AddItemToPool( BOBBYR_SHIPPING_DEST_GRIDNO, &obj, -1, 0, 0, 0 );
	}
	else
	{
		AddItemsToUnLoadedSector(BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z, BOBBYR_SHIPPING_DEST_GRIDNO, 1, &obj, 0, 0, 0, -1, FALSE);
	}
}

INT32 GetAdminActionCostForRegion(INT16 regionId)
{
	INT16 totalLocalActions = 0;
	INT16 totalNationalActions = 0;

	// ignore omerta when calculating action costs
	for (int a = OMERTA+1; a < NUM_TOWNS; ++a)
	{
		for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
		{
			totalNationalActions += rebelCommandSaveInfo.regions[a].GetLevel(b);

			if (a == regionId)
				totalLocalActions += rebelCommandSaveInfo.regions[a].GetLevel(b);
		}
	}

	return (rebelCommandSaveInfo.uSupplyDropCount + totalNationalActions) * gRebelCommandSettings.iAdminActionCostIncreaseNational + totalLocalActions * gRebelCommandSettings.iAdminActionCostIncreaseRegional;
}

INT16 GetAdminActionInRegion(INT16 regionId, RebelCommandAdminActions adminAction)
{
	if (regionId >= FIRST_TOWN && regionId < NUM_TOWNS)
	{
		for (int idx = 0; idx < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++idx)
		{
			if (rebelCommandSaveInfo.regions[regionId].actions[idx] == adminAction && rebelCommandSaveInfo.regions[regionId].IsActive(idx))
			{
				return idx;
			}
		}
	}

	return -1;
}

UINT8 GetRegionLoyalty(INT16 regionId)
{
	return gTownLoyalty[regionId].ubRating;
}

void GetDirectiveEffect(const RebelCommandDirectives directive, STR16 text)
{
	switch (directive)
	{
	case RCD_GATHER_SUPPLIES:
	case RCD_CREATE_PROPAGANDA:
	case RCD_SUPPORT_MILITIA:
	case RCD_ELITE_MILITIA:
	case RCD_HVT_STRIKES:
	case RCD_SPOTTERS:
	case RCD_RAID_MINES:
		swprintf(text, szRebelCommandDirectivesText[directive * 4 + 1], rebelCommandSaveInfo.directives[directive].GetValue1());
		break;

	case RCD_TRAIN_MILITIA:
		swprintf(text, szRebelCommandDirectivesText[directive * 4 + 1], rebelCommandSaveInfo.directives[directive].GetValue1(), (100.f + rebelCommandSaveInfo.directives[directive].GetValue2()) / 100.f);
		break;

	case RCD_CREATE_TURNCOATS:
		swprintf(text, szRebelCommandDirectivesText[directive * 4 + 1], rebelCommandSaveInfo.directives[directive].GetValue1(), gRebelCommandSettings.fCreateTurncoatsIntelCost);
		break;

	case RCD_DRAFT:
		swprintf(text, szRebelCommandDirectivesText[directive * 4 + 1], (rebelCommandSaveInfo.directives[directive].GetValue1() * CurrentPlayerProgressPercentage()));
		break;

	default:
		swprintf(text, L"Unrecognised directive id: %d. Do you need to add it to GetDirectiveEffect?", directive);
		break;
	}
}

INT32 GetDirectiveImprovementCost(const RebelCommandDirectives directive)
{
	return rebelCommandSaveInfo.directives[directive].GetCostToImprove();
}

INT32 GetMissionCost()
{
	const INT32 additionalCost = missionMap.size() * gRebelCommandSettings.iMissionAdditionalCost;
	return gRebelCommandSettings.iMissionBaseCost + additionalCost;
}

INT8 GetMissionSuccessChanceBonus(const MERCPROFILESTRUCT* merc)
{
	return merc ? merc->bExpLevel * 5 : 0;
}

void ImproveDirective(const RebelCommandDirectives directive)
{
	const INT32 cost = rebelCommandSaveInfo.directives[directive].GetCostToImprove();
	if (cost > LaptopSaveInfo.iCurrentBalance)
	{
		DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return;
	}

	CHAR16 text[200];
	swprintf(text, szRebelCommandText[RCT_IMPROVE_DIRECTIVE_PROMPT], cost);
	DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, text, LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, [](UINT8 exitValue) {
		if (exitValue == MSG_BOX_RETURN_YES)
		{
			const INT16 directive = REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey();
			const INT32 cost = rebelCommandSaveInfo.directives[directive].GetCostToImprove();
			rebelCommandSaveInfo.directives[directive].Improve();

			AddTransactionToPlayersBook(REBEL_COMMAND_SPENDING, 0, GetWorldTotalMin(), -cost);

			RenderWebsite();
		}
	});
}

void PurchaseAdminAction(INT32 regionId, INT32 actionIndex)
{
	const INT32 cost = GetAdminActionCostForRegion(regionId);

	if (cost > rebelCommandSaveInfo.iSupplies)
	{
		DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return;
	}

	rebelCommandSaveInfo.regions[regionId].actionLevels[actionIndex]++;

	if (actionIndex == RCAA_SUPPLY_LINE)
	{
		rebelCommandSaveInfo.regions[regionId].ubMaxLoyalty += static_cast<UINT8>(info.adminActions[RCAA_SUPPLY_LINE].fValue1);
		rebelCommandSaveInfo.regions[regionId].ubMaxLoyalty = min(rebelCommandSaveInfo.regions[regionId].ubMaxLoyalty, 100);
	}

	rebelCommandSaveInfo.iSupplies -= cost;
}

void RegionNavNext()
{
	if (websiteState == RCS_REGIONAL_OVERVIEW)
	{
		do
		{
			iCurrentRegionId++;

			if (iCurrentRegionId >= NUM_TOWNS)
				iCurrentRegionId = 1;
		} while (gfTownUsesLoyalty[iCurrentRegionId] == FALSE);

		UpdateAdminActionChangeList(iCurrentRegionId);
	}
}

void RegionNavPrev()
{
	if (websiteState == RCS_REGIONAL_OVERVIEW)
	{
		do
		{
			iCurrentRegionId--;

			if (iCurrentRegionId <= 0)
				iCurrentRegionId = NUM_TOWNS - 1;
		} while (gfTownUsesLoyalty[iCurrentRegionId] == FALSE);

		UpdateAdminActionChangeList(iCurrentRegionId);
	}
}

void SetupAdminActionBox(const UINT8 actionIndex, const UINT16 descriptionText, const UINT16 buttonText)
{
	CHAR16 text[200];
	INT16 x;
	INT16 y;
	INT16 helpTextY;

	switch (actionIndex % 3)
	{
	case 0: x = WEBSITE_LEFT + 10; break;
	case 1: x = WEBSITE_LEFT + 180; break;
	case 2: x = WEBSITE_LEFT + 350; break;
	}

	y = WEBSITE_TOP + 125 + 110 * (actionIndex / 3);
	helpTextY = y;

	if (actionIndex < 5 || adminActionChangeState == CAAS_INIT)
	{
		// show label if maxed out
		if ((actionIndex == RCAA_SUPPLY_LINE && rebelCommandSaveInfo.regions[iCurrentRegionId].ubMaxLoyalty >= MAX_LOYALTY_VALUE)
			|| (actionIndex != RCAA_SUPPLY_LINE && rebelCommandSaveInfo.regions[iCurrentRegionId].GetLevel(actionIndex) >= 2))
		{
			DrawTextToScreen(szRebelCommandAdminActionsText[buttonText], x, y + 7, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
		}
		else // show button
		{
			const UINT8 level = rebelCommandSaveInfo.regions[iCurrentRegionId].GetLevel(actionIndex);
			swprintf(text, szRebelCommandText[level == 0 ? RCT_ADMIN_ACTION_ESTABLISH : RCT_ADMIN_ACTION_IMPROVE], szRebelCommandAdminActionsText[buttonText]);
			const INT32 btnId = CreateTextButton(text, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 140, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
				{
					ButtonHelper(btn, reason, [btn]() { PurchaseAdminAction(MSYS_GetBtnUserData(btn, 0), MSYS_GetBtnUserData(btn, 1)); });
				});

			MSYS_SetBtnUserData(btnId, 0, iCurrentRegionId);
			MSYS_SetBtnUserData(btnId, 1, actionIndex);

			btnIds.push_back(btnId);
		}

		y += 22;
		swprintf(text, szRebelCommandText[RCT_ADMIN_ACTION_TIER], rebelCommandSaveInfo.regions[iCurrentRegionId].GetLevel(actionIndex));
		DrawTextToScreen(text, x, y, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// show on/off switch for toggleable actions
		if (CanAdminActionBeToggled(rebelCommandSaveInfo.regions[iCurrentRegionId].actions[actionIndex]) && rebelCommandSaveInfo.regions[iCurrentRegionId].GetLevel(actionIndex) > 0)
		{
			// draw checkbox text
			DrawTextToScreen(szRebelCommandText[RCT_ACTIVE], x+125, y, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);
			
			// draw checkbox
			const INT32 btnId = CreateCheckBoxButton(x + 128, y-3, "INTERFACE\\OptionsCheckBoxes_12x12.sti", MSYS_PRIORITY_HIGH, [](GUI_BUTTON* btn, INT32 reason) {
				const UINT8 regionIndex = (UINT8)MSYS_GetBtnUserData( btn, 0 );
				const UINT8 actionIndex = (UINT8)MSYS_GetBtnUserData( btn, 1 );

				if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
				{
					if (btn->uiFlags & BUTTON_CLICKED_ON)
					{
						// don't toggle on if we have a bad supply balance
						if (rebelCommandSaveInfo.iSupplies <= 0)
							return;

						btn->uiFlags &= ~BUTTON_CLICKED_ON;
						rebelCommandSaveInfo.regions[regionIndex].SetActive(actionIndex);
					}
					else
					{
						btn->uiFlags |= BUTTON_CLICKED_ON;
						rebelCommandSaveInfo.regions[regionIndex].SetInactive(actionIndex);
					}
				}
				RenderWebsite();
			});

			if (rebelCommandSaveInfo.iSupplies <= 0)
				DisableButton(btnId);

			MSYS_SetBtnUserData( btnId, 0, iCurrentRegionId );
			MSYS_SetBtnUserData( btnId, 1, actionIndex );

			Assert(ButtonList[btnId]);
			if (rebelCommandSaveInfo.regions[iCurrentRegionId].IsActive(actionIndex))
				ButtonList[btnId]->uiFlags |= BUTTON_CLICKED_ON;

			btnIds.push_back(btnId);

			// setup mouse target for "Active" text - setting this AFTER the checkbox so we can set the button id
			MSYS_DefineRegion(&adminActionActiveTextRegion[actionIndex-1], x+75, y-2, x+125, y+14, MSYS_PRIORITY_HIGH,
				CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, [](MOUSE_REGION* pRegion, INT32 iReason) {
					if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
					{
						const UINT8	regionIndex = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
						const UINT8 actionIndex = (UINT8)MSYS_GetRegionUserData( pRegion, 1 );
						const INT32 buttonId = MSYS_GetRegionUserData( pRegion, 2 );
						GUI_BUTTON* btn = ButtonList[buttonId];

						if (btn->uiFlags & BUTTON_CLICKED_ON)
						{
							// don't toggle on if we have a bad supply balance
							if (rebelCommandSaveInfo.iSupplies <= 0)
								return;

							btn->uiFlags &= ~BUTTON_CLICKED_ON;
							rebelCommandSaveInfo.regions[regionIndex].SetInactive(actionIndex);
						}
						else
						{
							btn->uiFlags |= BUTTON_CLICKED_ON;
							rebelCommandSaveInfo.regions[regionIndex].SetActive(actionIndex);
						}

						RenderWebsite();
					}
				});
			MSYS_AddRegion(&adminActionActiveTextRegion[actionIndex-1]);
			MSYS_SetRegionUserData(&adminActionActiveTextRegion[actionIndex-1], 0, iCurrentRegionId);
			MSYS_SetRegionUserData(&adminActionActiveTextRegion[actionIndex-1], 1, actionIndex);
			MSYS_SetRegionUserData(&adminActionActiveTextRegion[actionIndex-1], 2, btnId);

		}

		y += 13;
		const UINT8 textColor = rebelCommandSaveInfo.regions[iCurrentRegionId].IsActive(actionIndex) ? FONT_MCOLOR_BLACK : FONT_MCOLOR_DKGRAY;
		DisplayWrappedString(x, y, 140, 2, FONT10ARIAL, textColor, szRebelCommandAdminActionsText[descriptionText], FONT_MCOLOR_BLACK, FALSE, 0);

		helpTextY = y;


		// special case for index 5: show state change button
		if (actionIndex == 5)
		{
			y += 68;

			const INT32 btnId = CreateTextButton(szRebelCommandText[RCT_CHANGE_ADMIN_ACTION], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 140, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
				{
					ButtonHelper(btn, reason, [btn]() { adminActionChangeState = CAAS_CHANGING; });
				});
			btnIds.push_back(btnId);
		}
	}
	else if (actionIndex == 5 && adminActionChangeState == CAAS_CHANGING)
	{
		// let player change last action
		RebelCommandAdminActions newAction = adminActionChangeList[adminActionChangeIndex];

		DrawTextToScreen(szRebelCommandAdminActionsText[newAction*2], x, y + 7, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		y += 22;
		DisplayWrappedString(x, y, 140, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, szRebelCommandAdminActionsText[newAction*2+1], FONT_MCOLOR_BLACK, FALSE, 0);

		helpTextY = y;

		y += 81;
		INT32 btnId = CreateTextButton(szRebelCommandText[RCT_CANCEL], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 140, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { adminActionChangeState = CAAS_INIT; });
			});
		btnIds.push_back(btnId);

		y += 18;
		btnId = CreateTextButton(szRebelCommandText[RCT_PREV_ARROW], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 35, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { adminActionChangeIndex--; if (adminActionChangeIndex < 0) adminActionChangeIndex = static_cast<INT8>(adminActionChangeList.size() - 1); });
			});
		btnIds.push_back(btnId);

		btnId = CreateTextButton(szRebelCommandText[RCT_NEXT_ARROW], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x+35, y, 35, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { adminActionChangeIndex++; if (adminActionChangeIndex >= static_cast<INT8>(adminActionChangeList.size())) adminActionChangeIndex = 0; });
			});
		btnIds.push_back(btnId);

		btnId = CreateTextButton(szRebelCommandText[RCT_CONFIRM], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x+70, y, 70, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]()
				{
					if (LaptopSaveInfo.iCurrentBalance < ADMIN_ACTION_CHANGE_COST)
					{
						DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
						return;
					}

					CHAR16 text[200];
					swprintf(text, szRebelCommandText[RCT_CONFIRM_CHANGE_ADMIN_ACTION_PROMPT], ADMIN_ACTION_CHANGE_COST);
					DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, text, LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, [](UINT8 exitValue) {
						if (exitValue == MSG_BOX_RETURN_YES)
						{
							rebelCommandSaveInfo.regions[iCurrentRegionId].actionLevels[5] = 0;
							rebelCommandSaveInfo.regions[iCurrentRegionId].actions[5] = static_cast<RebelCommandAdminActions>(adminActionChangeList[adminActionChangeIndex]);

							UpdateAdminActionChangeList(iCurrentRegionId);
							AddTransactionToPlayersBook(REBEL_COMMAND_SPENDING, 0, GetWorldTotalMin(), -ADMIN_ACTION_CHANGE_COST);

							RenderWebsite();
						}
					});
				});
			});
		btnIds.push_back(btnId);
	}

	// admin action help text region
	MSYS_DefineRegion(&adminActionHelpTextRegion[actionIndex], x, helpTextY, x + 140, helpTextY + 68, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason)
		{
			const INT32 index = MSYS_GetRegionUserData(pRegion, 0);
			const RebelCommandAdminActions aa = (index == 5 && adminActionChangeState == CAAS_CHANGING) ?
				adminActionChangeList[adminActionChangeIndex] :
				rebelCommandSaveInfo.regions[iCurrentRegionId].actions[index];

			CHAR16 text[250];
			switch (aa)
			{
			case RCAA_SAFEHOUSES:
				// 1 tile range
				swprintf(text, szRebelCommandHelpText[RCHT_AA_TOWN_PLUS_ONE]);
				break;

			case RCAA_SUPPLY_DISRUPTION:
			case RCAA_PATHFINDERS:
			case RCAA_HARRIERS:
				// 1-2 tile range
				swprintf(text, szRebelCommandHelpText[RCHT_AA_TOWN_PLUS_TWO]);
				break;

			case RCAA_SCOUTS:
				// 2-3 tile range
				swprintf(text, szRebelCommandHelpText[RCHT_AA_TOWN_PLUS_THREE]);
				break;

			default:
				// apply in cities only
				swprintf(text, szRebelCommandHelpText[RCHT_AA_TOWN_ONLY]);
				break;
			}

			SetRegionFastHelpText(&adminActionHelpTextRegion[index], text);
		}, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&adminActionHelpTextRegion[actionIndex]);
	MSYS_SetRegionUserData(&adminActionHelpTextRegion[actionIndex], 0, actionIndex);
}

void SetDirectiveDescriptionHelpText(INT32 reason, MOUSE_REGION& region, RebelCommandDirectives text)
{
	if (reason == MSYS_CALLBACK_REASON_MOVE)
		SetRegionFastHelpText(&region, szRebelCommandDirectivesText[text]);
	else if (reason == MSYS_CALLBACK_REASON_LOST_MOUSE)
		SetRegionFastHelpText(&region, L"");
}

void SetRegionHelpText(INT32 reason, MOUSE_REGION& helpTextRegion, RebelCommandHelpText text)
{
	if (reason == MSYS_CALLBACK_REASON_MOVE)
		SetRegionFastHelpText(&helpTextRegion, szRebelCommandHelpText[text]);
	else if (reason == MSYS_CALLBACK_REASON_LOST_MOUSE)
		SetRegionFastHelpText(&helpTextRegion, L"");
}

void SetWebsiteView(WebsiteState newState)
{
	websiteState = newState;
}

void ToggleWebsiteView()
{
	switch (websiteState)
	{
	case RCS_NATIONAL_OVERVIEW:
		websiteState = RCS_REGIONAL_OVERVIEW;
		break;

	case RCS_REGIONAL_OVERVIEW:
		websiteState = RCS_AGENT_OVERVIEW;
		break;

	case RCS_AGENT_OVERVIEW:
		websiteState = RCS_NATIONAL_OVERVIEW;
		break;
	}
}

void UpdateAdminActionChangeList(INT16 regionId)
{
	adminActionChangeIndex = 0;
	adminActionChangeList.clear();
	adminActionChangeState = CAAS_INIT;

	for (int aa = RCAA_SUPPLY_LINE; aa < RCAA_NUM_ACTIONS; ++aa)
	{
		BOOLEAN found = FALSE;

		for (int actionId = 0; actionId < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++actionId)
		{
			if (rebelCommandSaveInfo.regions[regionId].actions[actionId] == aa)
			{
				found = TRUE;
				break;
			}
		}

		if (found == FALSE)
		{
			adminActionChangeList.push_back(static_cast<RebelCommandAdminActions>(aa));
		}
	}
}

BOOLEAN EnterWebsite()
{
	UpdateAdminActionChangeList(iCurrentRegionId);

	// make sure we have a valid directive
	if (rebelCommandSaveInfo.iActiveDirective < RCD_GATHER_SUPPLIES || rebelCommandSaveInfo.iActiveDirective >= RCD_NUM_DIRECTIVES)
	{
		rebelCommandSaveInfo.iActiveDirective = RCD_GATHER_SUPPLIES;
		rebelCommandSaveInfo.iSelectedDirective = RCD_GATHER_SUPPLIES;
	}

	ClearAllButtons();

	websiteState = RCS_NATIONAL_OVERVIEW;

	RenderWebsite();

	return(TRUE);
}

void ExitWebsite()
{
	ClearAllButtons();
	ClearAllHelpTextRegions();
	REBEL_COMMAND_DROPDOWN.Destroy();
}

void HandleWebsite()
{
	InputAtom input;
	while (DequeueSpecificEvent(&input, KEY_DOWN | KEY_UP | KEY_REPEAT))
	{
		if (input.usEvent == KEY_DOWN)
		{
			switch (input.usParam)
			{
			case TAB:
			case SPACE:
				ToggleWebsiteView();
				RenderWebsite();
				break;

			case 'a':
			case LEFTARROW:
				RegionNavPrev();
				RenderWebsite();
				break;

			case 'd':
			case RIGHTARROW:
				RegionNavNext();
				RenderWebsite();
				break;

			case '1':
				SetWebsiteView(RCS_NATIONAL_OVERVIEW);
				RenderWebsite();
				break;

			case '2':
				SetWebsiteView(RCS_REGIONAL_OVERVIEW);
				RenderWebsite();
				break;

			case '3':
				SetWebsiteView(RCS_AGENT_OVERVIEW);
				RenderWebsite();
				break;

			default:
				HandleKeyBoardShortCutsForLapTop(input.usEvent, input.usParam, input.usKeyState);
				break;
			}
		}
	}

	if (redraw)
	{
		RenderWebsite();
		REBEL_COMMAND_DROPDOWN.Display();
		redraw = FALSE;
	}

}

void RenderWebsite()
{
	REBEL_COMMAND_DROPDOWN.Destroy();

	ClearAllButtons();
	ClearAllHelpTextRegions();

	// background
	ColorFillVideoSurfaceArea(FRAME_BUFFER, WEBSITE_LEFT, WEBSITE_TOP, WEBSITE_LEFT + WEBSITE_WIDTH, WEBSITE_TOP + WEBSITE_HEIGHT, Get16BPPColor(FROMRGB(224, 224, 224)));

	SetFontShadow(FONT_GRAY1);

	// national/regional views
	switch (websiteState)
	{
	case RCS_REGIONAL_OVERVIEW:
		RenderRegionalOverview();
		break;

	case RCS_AGENT_OVERVIEW:
		RenderMissionOverview();
		break;

	case RCS_NATIONAL_OVERVIEW:
	default:
		RenderNationalOverview();
		break;
	}

	SetFontShadow(DEFAULT_SHADOW);

	MarkButtonsDirty();
	RenderWWWProgramTitleBar();
	InvalidateRegion(WEBSITE_LEFT, WEBSITE_TOP, WEBSITE_LEFT + WEBSITE_WIDTH, WEBSITE_TOP + WEBSITE_HEIGHT);
}

void RenderHeader(RebelCommandText titleText)
{
	CHAR16 sText[500];
	UINT16 usPosX, usPosY;
	INT32 btnId;

	// title
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 2;
	DrawTextToScreen(szRebelCommandText[titleText], usPosX, usPosY, 0, FONT16ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// supplies
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 23;
	DrawTextToScreen(szRebelCommandText[RCT_SUPPLIES], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// supply count
	usPosX = WEBSITE_LEFT + 55;
	usPosY = WEBSITE_TOP + 20;
	swprintf(sText, L"%d", rebelCommandSaveInfo.iSupplies);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, rebelCommandSaveInfo.iSupplies > 0 ? FONT_GREEN : FONT_MCOLOR_LTRED, FONT_MCOLOR_BLACK, FALSE, 0);

	// intel
	usPosX = WEBSITE_LEFT + 150;
	usPosY = WEBSITE_TOP + 23;
	DrawTextToScreen(szRebelCommandText[RCT_INTEL], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// intel count
	usPosX = WEBSITE_LEFT + 185;
	usPosY = WEBSITE_TOP + 20;
	const int intel = (int)GetIntel();
	swprintf(sText, L"%d", intel);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, intel > 0 ? FONT_GREEN : FONT_MCOLOR_LTRED, FONT_MCOLOR_BLACK, FALSE, 0);

	// supplies region
	MSYS_DefineRegion(&suppliesHelpTextRegion, WEBSITE_LEFT, WEBSITE_TOP + 20, WEBSITE_LEFT + 100, WEBSITE_TOP + 35, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetRegionHelpText(iReason, suppliesHelpTextRegion, RCHT_SUPPLIES); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&suppliesHelpTextRegion);
	MSYS_SetRegionUserData(&suppliesHelpTextRegion, 0, 0);

	// view select text
	usPosX = WEBSITE_LEFT + 251;
	usPosY = WEBSITE_TOP + 3;
	DrawTextToScreen(szRebelCommandText[RCT_SELECT_VIEW], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// view swap buttons
	usPosY = WEBSITE_TOP + 13;
	btnId = CreateTextButton(szRebelCommandText[RCT_SWITCH_TO_NATIONAL], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 82, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []() { SetWebsiteView(RCS_NATIONAL_OVERVIEW); });
		});
	btnIds.push_back(btnId);
	usPosX = WEBSITE_LEFT + 334;
	btnId = CreateTextButton(szRebelCommandText[RCT_SWITCH_TO_REGIONAL], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 82, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []() { SetWebsiteView(RCS_REGIONAL_OVERVIEW); });
		});
	btnIds.push_back(btnId);
	usPosX = WEBSITE_LEFT + 417;
	btnId = CreateTextButton(szRebelCommandText[RCT_SWITCH_TO_AGENT], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 82, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []() { SetWebsiteView(RCS_AGENT_OVERVIEW); });
		});
	btnIds.push_back(btnId);

	// line at the bottom of the header
	usPosX = WEBSITE_LEFT - 1;
	usPosY = WEBSITE_TOP + 35;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + WEBSITE_WIDTH, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// DEBUG
	if (CHEATER_CHEAT_LEVEL())
	{
		INT32 btnId;
		usPosX = WEBSITE_LEFT + 400;
		usPosY = WEBSITE_TOP + 380;
		btnId = CreateTextButton(L"DEBUG MAGIC!", FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 99, 14, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason) {
			ButtonHelper(btn, reason, []() { DEBUG_DAY(); });
			});
		btnIds.push_back(btnId);
	}

}

void RenderNationalOverview()
{
	DropdownSetup();

	CHAR16 sText[500];
	UINT16 usPosX, usPosY;
	INT32 btnId;

	// title
	RenderHeader(RCT_NATIONAL_OVERVIEW);

	// incoming supplies
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 40;
	DrawTextToScreen(szRebelCommandText[RCT_INCOMING_SUPPLIES], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	usPosX = WEBSITE_LEFT + 5;
	usPosY += 10;
	iIncomingSuppliesPerDay = CalcIncomingSuppliesPerDay(static_cast<RebelCommandDirectives>(rebelCommandSaveInfo.iSelectedDirective));
	swprintf(sText, L"%d", iIncomingSuppliesPerDay);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, iIncomingSuppliesPerDay > 0 ? FONT_GREEN : FONT_MCOLOR_LTRED, FONT_MCOLOR_BLACK, FALSE, 0);

	INT32 width = 0;
	INT32 value = iIncomingSuppliesPerDay;
	do {
		width += value % 10 == 1 ? 6 : 8;
		value /= 10;
	} while (value != 0);
	usPosX += width;
	usPosY += 3;
	DrawTextToScreen(szRebelCommandText[RCT_PER_DAY], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// incoming supplies help text region
	usPosX = WEBSITE_LEFT + 1;
	usPosY -= 13;
	MSYS_DefineRegion(&suppliesIncomeHelpTextRegion, usPosX, usPosY, usPosX + 100, usPosY + 35, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { 
			if (iReason == MSYS_CALLBACK_REASON_MOVE)
			{
				CHAR16 text[1000];

				// base income
				const INT32 base = static_cast<INT32>(CurrentPlayerProgressPercentage() * gRebelCommandSettings.fIncomeModifier);
				swprintf(text, szRebelCommandHelpText[RCHT_SUPPLIES_INCOME], base);

				// admin action upkeep
				const INT32 supplyUpkeep = static_cast<INT32>(gRebelCommandSettings.fIncomeModifier + 0.5f);

				for (int a = FIRST_TOWN+1; a < NUM_TOWNS; ++a)
				{
					// ignore this region if there is no active admin team
					if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_ACTIVE)
						continue;

					INT32 upkeepCount = 0;
					for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
					{
						if (rebelCommandSaveInfo.regions[a].IsActive(b) && rebelCommandSaveInfo.regions[a].GetLevel(b) > 0)
							upkeepCount++;
					}

					if (upkeepCount > 0)
					{
						const INT32 totalUpkeep = upkeepCount * supplyUpkeep;
						swprintf(text, L"%s\n-%d (%s)", text, totalUpkeep, pTownNames[a]);
					}
				}

				SetRegionFastHelpText(&suppliesIncomeHelpTextRegion, text);
			}
			else if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE)
				SetRegionFastHelpText(&suppliesIncomeHelpTextRegion, L"");
		 }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&suppliesIncomeHelpTextRegion);
	MSYS_SetRegionUserData(&suppliesIncomeHelpTextRegion, 0, 0);

	// line between incoming supplies and directive
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 43;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + WEBSITE_WIDTH, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// current directive 
	usPosX = WEBSITE_LEFT + 1;
	usPosY += 5;
	DrawTextToScreen(szRebelCommandText[RCT_CURRENT_DIRECTIVE], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// set directives dropdown coords for use later
	const INT16 dropdownX = WEBSITE_LEFT + 5;
	const INT16 dropdownY = usPosY + 10;

	// improve directive button
	usPosX = WEBSITE_LEFT + WEBSITE_WIDTH / 2 + 5;
	usPosY = dropdownY;
	const INT16 directive = REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey();
	if (rebelCommandSaveInfo.directives[directive].CanImprove())
	{
		swprintf(sText, szRebelCommandText[RCT_IMPROVE_DIRECTIVE], GetDirectiveImprovementCost(static_cast<RebelCommandDirectives>(directive)));
		btnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 200, 24, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { ImproveDirective(static_cast<RebelCommandDirectives>(MSYS_GetBtnUserData(btn, 0))); });
			});
		btnIds.push_back(btnId);

		MSYS_SetBtnUserData( btnId, 0, REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey() );
	}

	// directive effect
	usPosX = dropdownX;
	usPosY = dropdownY + 27;
	GetDirectiveEffect(static_cast<RebelCommandDirectives>(directive), sText);
	DisplayWrappedString(usPosX, usPosY, WEBSITE_WIDTH / 2 - 10, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, sText, FONT_MCOLOR_BLACK, FALSE, 0);

	// directives description help text region
	MSYS_DefineRegion(&directiveDescriptionHelpTextRegion, usPosX, usPosY, usPosX + WEBSITE_WIDTH / 2 - 5, usPosY + 40, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetDirectiveDescriptionHelpText(iReason, directiveDescriptionHelpTextRegion, static_cast<RebelCommandDirectives>(REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey() * 4 + 2)); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&directiveDescriptionHelpTextRegion);
	MSYS_SetRegionUserData(&directiveDescriptionHelpTextRegion, 0, 0);

	// directive improvement description
	usPosX = WEBSITE_LEFT + WEBSITE_WIDTH / 2 + 5;
	usPosY = dropdownY + 27;
	if (rebelCommandSaveInfo.directives[directive].CanImprove())
		DisplayWrappedString(usPosX, usPosY, WEBSITE_WIDTH / 2 - 5, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, szRebelCommandDirectivesText[directive * 4 + 3], FONT_MCOLOR_BLACK, FALSE, 0);

	// directive change notification, if applicable
	usPosX = WEBSITE_LEFT + 5;
	usPosY += 50;
	if (rebelCommandSaveInfo.iActiveDirective != rebelCommandSaveInfo.iSelectedDirective)
		DrawTextToScreen(szRebelCommandText[RCT_DIRECTIVE_EFFECT_TIME], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// line between directive and militia
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 10;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + WEBSITE_WIDTH, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// militia
	usPosX = WEBSITE_LEFT + 1;
	usPosY += 5;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_OVERVIEW], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// militia count
	UINT16 militiaGreen = 0, militiaRegular = 0, militiaElite = 0;
	for (INT16 sX = 1; sX < (MAP_WORLD_X - 1); ++sX)
	{
		for (INT16 sY = 1; sY < (MAP_WORLD_Y - 1); ++sY)
		{
			militiaGreen += MilitiaInSectorOfRank(sX, sY, GREEN_MILITIA);
			militiaRegular += MilitiaInSectorOfRank(sX, sY, REGULAR_MILITIA);
			militiaElite += MilitiaInSectorOfRank(sX, sY, ELITE_MILITIA);
		}
	}

	// headers
	usPosX = WEBSITE_LEFT + 10;
	usPosY += 15;
	const INT16 militiaY = usPosY; // cache for other militia elements
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_GREEN], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosY = militiaY + 15;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_REGULAR], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosY = militiaY + 30;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_ELITE], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// values
	usPosX += 45;
	usPosY = militiaY - 2;
	swprintf(sText, L"%d", militiaGreen);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_LTGREEN, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosY += 15;
	swprintf(sText, L"%d", militiaRegular);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_LTBLUE, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosY += 15;
	swprintf(sText, L"%d", militiaElite);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_DKBLUE, FONT_MCOLOR_BLACK, FALSE, 0);

	// militia volunteer pool, if enabled
	if (gGameExternalOptions.fMilitiaVolunteerPool)
	{
		// draw vertical line
		usPosX += 75;
		usPosY = militiaY - 3;
		{
			UINT32 uiDestPitchBYTES;
			UINT8 *pDestBuf;

			pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

			SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY + 38, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

			UnLockVideoSurface( FRAME_BUFFER );
		}

		// header
		usPosX += 20;
		DrawTextToScreen(szRebelCommandText[RCT_MILITIA_VOLUNTEERS], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// value
		usPosY += 12;
		const INT16 volunteerPool = static_cast<INT16>(LaptopSaveInfo.dMilitiaVolunteerPool);
		swprintf(sText, L"%d", GetVolunteerPool());
		DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		usPosX += 50;
	}

	// militia resources, if enabled
	if (gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory)
	{
		// draw vertical line
		usPosX += 75;
		usPosY = militiaY - 3;
		{
			UINT32 uiDestPitchBYTES;
			UINT8 *pDestBuf;

			pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

			SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

			LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY + 38, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

			UnLockVideoSurface( FRAME_BUFFER );
		}
		// headers
		usPosX += 20;
		DrawTextToScreen(szRebelCommandText[RCT_MILITIA_RESOURCES], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
		usPosY += 12;
		DrawTextToScreen(szRebelCommandText[RCT_MILITIA_RESOURCE_GUNS], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
		usPosX += 66;
		DrawTextToScreen(szRebelCommandText[RCT_MILITIA_RESOURCE_ARMOUR], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
		usPosX += 66;
		DrawTextToScreen(szRebelCommandText[RCT_MILITIA_RESOURCE_MISC], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// values
		usPosX -= 132;
		usPosY += 12;
		swprintf(sText, L"%.1f", LaptopSaveInfo.dMilitiaGunPool);
		DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_ORANGE, FONT_MCOLOR_BLACK, FALSE, 0);
		usPosX += 66;
		swprintf(sText, L"%.1f", LaptopSaveInfo.dMilitiaArmourPool);
		DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_LTKHAKI, FONT_MCOLOR_BLACK, FALSE, 0);
		usPosX += 66;
		swprintf(sText, L"%.1f", LaptopSaveInfo.dMilitiaMiscPool);
		DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_LTBLUE, FONT_MCOLOR_BLACK, FALSE, 0);
	}

	// line
	usPosX = WEBSITE_LEFT + 25;
	usPosY = militiaY + 50;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + 450, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// training cost
	usPosX = WEBSITE_LEFT + 10;
	usPosY += 15;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_TRAINING_COST], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// draw vertical line
	usPosX += 120;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY - 2, usPosX, usPosY + 38, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// upkeep cost
	usPosX += 20;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_UPKEEP_COST], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// upkeep cost per type
	usPosY += 12;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_GREEN], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosX += 66;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_REGULAR], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosX += 66;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_ELITE], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosX += 75;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_TOTAL], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// training cost value
	usPosX = WEBSITE_LEFT + 15;
	usPosY += 12;
	const INT16 cost = static_cast<const INT16>(gGameExternalOptions.iMilitiaTrainingCost * GetMilitiaTrainingCostModifier());
	swprintf(sText, L"$%d", cost);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// upkeep costs
	const INT16 costGreen = static_cast<const INT16>(gGameExternalOptions.usDailyCostTown[GREEN_MILITIA] * GetMilitiaUpkeepCostModifier());
	const INT16 costRegular = static_cast<const INT16>(gGameExternalOptions.usDailyCostTown[REGULAR_MILITIA] * GetMilitiaUpkeepCostModifier());
	const INT16 costElite = static_cast<const INT16>(gGameExternalOptions.usDailyCostTown[ELITE_MILITIA] * GetMilitiaUpkeepCostModifier());
	usPosX += 140;
	swprintf(sText, L"$%d", costGreen);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosX += 66;
	swprintf(sText, L"$%d", costRegular);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosX += 66;
	swprintf(sText, L"$%d", costElite);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	usPosX += 75;
	swprintf(sText, L"$%d", (costGreen * militiaGreen + costRegular * militiaRegular + costElite * militiaElite));
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// line
	usPosX = WEBSITE_LEFT + 25;
	usPosY += 30;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + 450, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// training speed bonus
	usPosX = WEBSITE_LEFT + 10;
	usPosY += 15;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_TRAINING_SPEED_BONUS], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// training speed bonus value
	usPosX += 5;
	usPosY += 12;
	value = GetMilitiaTrainingSpeedBonus();
	swprintf(sText, L"%d", value);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	width = 0;
	do {
		width += value % 10 == 1 ? 6 : 8;
		value /= 10;
	} while (value != 0);
	DrawTextToScreen(L"%%", usPosX + width, usPosY + 3, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// draw vertical line
	usPosX = WEBSITE_LEFT + 130;
	usPosY -= 12;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY + 38, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// militia physical stat bonus
	usPosX += 20;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_PHYSICAL_BONUS], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// militia marksmanship bonus
	usPosY += 15;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_MARKSMANSHIP_BONUS], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// physical stat value
	usPosX += 120;
	usPosY -= 18;
	swprintf(sText, L"+%d", gRebelCommandSettings.iMilitiaStatBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// marksmanship value
	usPosY += 15;
	swprintf(sText, L"+%d", gRebelCommandSettings.iMilitiaMarksmanshipBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// upgrade militia stats button
	usPosX = WEBSITE_LEFT + 325;
	usPosY -= 15;
	if (rebelCommandSaveInfo.iMilitiaStatsLevel < static_cast<UINT8>(gRebelCommandSettings.iMilitiaUpgradeCosts.size()))
	{
		swprintf(sText, szRebelCommandText[RCT_MILITIA_UPGRADE_STATS], gRebelCommandSettings.iMilitiaUpgradeCosts[rebelCommandSaveInfo.iMilitiaStatsLevel]);
		btnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 150, 25, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason) {
			ButtonHelper(btn, reason, []() { UpgradeMilitiaStats(); });
			});
		btnIds.push_back(btnId);
	}

	// dropdown - has to be last, or else things after this will be drawn twice
	REBEL_COMMAND_DROPDOWN.Display();
}

void RenderRegionalOverview()
{
	CHAR16 sText[800];
	UINT16 usPosX, usPosY;
	INT32 btnId;

	// title
	RenderHeader(RCT_REGIONAL_OVERVIEW);

	// region 
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 40;
	DrawTextToScreen(szRebelCommandText[RCT_REGION], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// next region
	usPosX = WEBSITE_LEFT + 334;
	btnId = CreateTextButton(szRebelCommandText[RCT_NEXT], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 82, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []()
				{
					RegionNavNext();
				});
		});
	btnIds.push_back(btnId);

	// prev region
	usPosX = WEBSITE_LEFT + 417;
	btnId = CreateTextButton(szRebelCommandText[RCT_PREV], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 82, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []()
				{
					RegionNavPrev();
				});
		});
	btnIds.push_back(btnId);

	// region value
	usPosX = WEBSITE_LEFT + 5;
	usPosY += 12;
	swprintf(sText, L"%s", pTownNames[iCurrentRegionId]);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// line between region info and admin info
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 15;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + WEBSITE_WIDTH, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// admin team
	usPosX = WEBSITE_LEFT + 1;
	usPosY += 5;
	DrawTextToScreen(szRebelCommandText[RCT_ADMIN_TEAM], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// admin team status
	const RegionAdminStatus adminStatus = rebelCommandSaveInfo.regions[iCurrentRegionId].adminStatus;
	usPosX += 10;
	usPosY += 12;
	switch (adminStatus)
	{
	case RAS_ACTIVE:
		DrawTextToScreen(szRebelCommandText[RCT_ACTIVE], usPosX, usPosY, 0, FONT14ARIAL, FONT_GREEN, FONT_MCOLOR_BLACK, FALSE, 0);
		break;

	case RAS_INACTIVE:
		DrawTextToScreen(szRebelCommandText[RCT_INACTIVE], usPosX, usPosY, 0, FONT14ARIAL, FONT_ORANGE, FONT_MCOLOR_BLACK, FALSE, 0);
		break;

	default:
	case RAS_NONE:
		DrawTextToScreen(szRebelCommandText[RCT_NONE], usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_RED, FONT_MCOLOR_BLACK, FALSE, 0);
		break;
	}

	// admin team help text region
	usPosX -= 10;
	usPosY -= 12;
	MSYS_DefineRegion(&adminTeamHelpTextRegion, usPosX, usPosY, usPosX + 100, usPosY + 35, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetRegionHelpText(iReason, adminTeamHelpTextRegion, RCHT_ADMIN_TEAM); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&adminTeamHelpTextRegion);
	MSYS_SetRegionUserData(&adminTeamHelpTextRegion, 0, 0);

	// vertical line between admin team and loyalty
	usPosX = WEBSITE_LEFT + 105;
	usPosY += 5;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY + 15, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// loyalty
	usPosX += 15;
	usPosY -= 5;
	DrawTextToScreen(szRebelCommandText[RCT_LOYALTY], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// loyalty value
	usPosX += 10;
	usPosY += 12;
	swprintf(sText, L"%d", GetRegionLoyalty(iCurrentRegionId));
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	INT32 width = 0;
	INT32 value = GetRegionLoyalty(iCurrentRegionId);
	do {
		width += value % 10 == 1 ? 6 : 8;
		value /= 10;
	} while (value != 0);
	DrawTextToScreen(L"%%", usPosX + width, usPosY + 3, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// loyalty region
	usPosX -= 10;
	usPosY -= 12;
	MSYS_DefineRegion(&loyaltyHelpTextRegion, usPosX, usPosY, usPosX + 100, usPosY + 35, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetRegionHelpText(iReason, loyaltyHelpTextRegion, RCHT_LOYALTY); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&loyaltyHelpTextRegion);
	MSYS_SetRegionUserData(&loyaltyHelpTextRegion, 0, 0);

	// vertical line between loyalty and max loyalty
	usPosX = WEBSITE_LEFT + 195;
	usPosY += 5;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY + 15, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// max loyalty
	usPosX += 15;
	usPosY -= 5;
	DrawTextToScreen(szRebelCommandText[RCT_LOYALTY_MAX], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// max loyalty value
	usPosX += 10;
	usPosY += 12;
	swprintf(sText, L"%d", rebelCommandSaveInfo.regions[iCurrentRegionId].ubMaxLoyalty);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	width = 0;
	value = rebelCommandSaveInfo.regions[iCurrentRegionId].ubMaxLoyalty;
	do {
		width += value % 10 == 1 ? 6 : 8;
		value /= 10;
	} while (value != 0);
	DrawTextToScreen(L"%%", usPosX + width, usPosY + 3, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// max loyalty region
	usPosX -= 10;
	usPosY -= 12;
	MSYS_DefineRegion(&maxLoyaltyHelpTextRegion, usPosX, usPosY, usPosX + 100, usPosY + 35, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetRegionHelpText(iReason, maxLoyaltyHelpTextRegion, RCHT_MAX_LOYALTY); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&maxLoyaltyHelpTextRegion);
	MSYS_SetRegionUserData(&maxLoyaltyHelpTextRegion, 0, 0);
	
	// vertical line between max loyalty
	usPosX = WEBSITE_LEFT + 325;
	usPosY += 5;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX, usPosY + 15, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// deploy/reactivate admin teams (if applicable)
	// if we're displaying any of these, early exit since everything else is locked behind the admin team's deployment
	if (iCurrentRegionId == OMERTA)
	{
		// omerta displays the tutorial about regional bonuses
		usPosX = WEBSITE_LEFT + 100;
		usPosY = WEBSITE_TOP + 150;
		INT16 height = DisplayWrappedString(usPosX, usPosY, 300, 2, FONT12ARIAL, FONT_MCOLOR_BLACK, szRebelCommandText[RCT_ADMIN_ACTIONS_TUTORIAL1], FONT_MCOLOR_BLACK, FALSE, 0);

		usPosY += height + 15;
		height = DisplayWrappedString(usPosX, usPosY, 300, 2, FONT12ARIAL, FONT_MCOLOR_BLACK, szRebelCommandText[RCT_ADMIN_ACTIONS_TUTORIAL2], FONT_MCOLOR_BLACK, FALSE, 0);

		usPosY += height + 15;
		height = DisplayWrappedString(usPosX, usPosY, 300, 2, FONT12ARIAL, FONT_MCOLOR_BLACK, szRebelCommandText[RCT_ADMIN_ACTIONS_TUTORIAL3], FONT_MCOLOR_BLACK, FALSE, 0);

		return;
	}
	else if (!gTownLoyalty[iCurrentRegionId].fStarted || GetRegionLoyalty(iCurrentRegionId) == 0)
	{
		usPosX = WEBSITE_LEFT + 150;
		usPosY = WEBSITE_TOP + 175;
		DisplayWrappedString(usPosX, usPosY, 200, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, szRebelCommandText[RCT_CANNOT_DEPLOY_ADMIN_TEAM], FONT_MCOLOR_BLACK, FALSE, 0);

		return;
	}
	else if (rebelCommandSaveInfo.regions[iCurrentRegionId].adminStatus == RAS_NONE)
	{
		INT16 numAdminTeams = 0;
		for (int a = FIRST_TOWN + 1; a < NUM_TOWNS; ++a)
			if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_NONE)
				numAdminTeams++;
		const INT32 adminDeployCost = rebelCommandSaveInfo.regions[iCurrentRegionId].GetAdminDeployCost(numAdminTeams);

		// deploy admin team button (if no team deployed)
		usPosX = WEBSITE_LEFT + 100;
		usPosY = WEBSITE_TOP + 150;
		swprintf(sText, szRebelCommandText[RCT_DEPLOY_ADMIN_TEAM], adminDeployCost);
		btnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 300, 100, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { DeployOrReactivateAdminTeam(MSYS_GetBtnUserData(btn, 0)); });
			});
		btnIds.push_back(btnId);

		MSYS_SetBtnUserData( btnId, 0, iCurrentRegionId );
		return;
	}
	else if (rebelCommandSaveInfo.regions[iCurrentRegionId].adminStatus == RAS_INACTIVE)
	{
		INT16 numAdminTeams = 0;
		for (int a = FIRST_TOWN + 1; a < NUM_TOWNS; ++a)
			if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_NONE)
				numAdminTeams++;
		const INT32 adminReactivateCost = rebelCommandSaveInfo.regions[iCurrentRegionId].GetAdminReactivateCost(numAdminTeams);

		// reactivate team button (when retaking a lost town)
		usPosX = WEBSITE_LEFT + 100;
		usPosY = WEBSITE_TOP + 150;
		swprintf(sText, szRebelCommandText[RCT_REACTIVATE_ADMIN_TEAM], adminReactivateCost / 2);
		btnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 300, 100, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { DeployOrReactivateAdminTeam(MSYS_GetBtnUserData(btn, 0)); });
			});
		btnIds.push_back(btnId);

		MSYS_SetBtnUserData( btnId, 0, iCurrentRegionId );

		return;
	}

	// line between admin info and admin actions
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 25;
	{
		UINT32 uiDestPitchBYTES;
		UINT8 *pDestBuf;

		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

		SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		LineDraw(FALSE, usPosX, usPosY, usPosX + WEBSITE_WIDTH, usPosY, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

		UnLockVideoSurface( FRAME_BUFFER );
	}

	// admin actions
	usPosX = WEBSITE_LEFT + 1;
	usPosY += 5;
	DrawTextToScreen(szRebelCommandText[RCT_ADMIN_ACTIONS], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// setup action boxes (max 6)
	UINT8 actionCount = 0;
	for (int a = 0; a < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++a)
	{
		const RebelCommandAdminActions action = static_cast<RebelCommandAdminActions>(rebelCommandSaveInfo.regions[iCurrentRegionId].actions[a]);
		if (rebelCommandSaveInfo.regions[iCurrentRegionId].actions[a] != RCAA_NONE)
		{
			SetupAdminActionBox(actionCount++, action * 2 + 1, action * 2);
		}
	}

	// admin upgrade cost
	usPosX = WEBSITE_LEFT + 5;
	usPosY = WEBSITE_TOP + WEBSITE_HEIGHT - 15;
	swprintf(sText, szRebelCommandText[RCT_ADMIN_ACTION_COST], GetAdminActionCostForRegion(iCurrentRegionId));
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
}

BOOLEAN SetupMissionAgentBox(UINT16 x, UINT16 y, INT8 index)
{
	CHAR16 sText[800];
	INT32 btnId;
	VOBJECT_DESC vObjDesc;
	HVOBJECT hvObj;
	char sTemp[100];
	UINT32 image;
	UINT32 uiDestPitchBYTES;
	UINT8 *pDestBuf;

	// temp/fixme
	std::vector<SOLDIERTYPE*> mercs;
	for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[i];

		if (pSoldier && pSoldier->bActive
			&& !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE)
			)
		{
			mercs.push_back(pSoldier);
		}
	}

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// top horizontal line
	LineDraw(FALSE, x, y, x+230, y, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);
	
	// bottom horizontal line
	LineDraw(FALSE, x, y+310, x+230, y+310, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

	// left vertical line
	LineDraw(FALSE, x, y, x, y+310, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

	// right vertical line
	LineDraw(FALSE, x+230, y, x+230, y+310, Get16BPPColor(FROMRGB(0, 0, 0)), pDestBuf);

	UnLockVideoSurface( FRAME_BUFFER );

	// clamp indices
	// we're reserving an index for the generic rebel agent, so no need to subtract 1 from size here
	if (agentIndex[index] < 0) agentIndex[index] = static_cast<INT8>(mercs.size());
	else if (agentIndex[index] > static_cast<INT8>(mercs.size())) agentIndex[index] = 0;

	if (rebelCommandSaveInfo.availableMissions[index] == RCAM_NONE)
	{
		// we shouldn't even reach this point, but leaving this here for safety
		DrawTextToScreen(szRebelCommandText[RCT_MISSION_PREP_IN_PROGRESS], x, y+155, 230, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
		return FALSE;
	}

	// draw mission title
	switch (rebelCommandSaveInfo.availableMissions[index])
	{
	case RCAM_DEEP_DEPLOYMENT:					swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_DEEP_DEPLOYMENT_TITLE]); break;
	case RCAM_DISRUPT_ASD:						swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_DISRUPT_ASD_TITLE]); break;
	case RCAM_GET_ENEMY_MOVEMENT_TARGETS:		swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_GET_ENEMY_MOVEMENT_TARGETS_TITLE]); break;
	case RCAM_IMPROVE_LOCAL_SHOPS:				swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_IMPROVE_LOCAL_SHOPS_TITLE]); break;
	case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:	swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_REDUCE_STRATEGIC_DECISION_SPEED_TITLE]); break;
	case RCAM_REDUCE_UNALERTED_ENEMY_VISION:	swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_REDUCE_UNALERTED_ENEMY_VISION_TITLE]); break;
	case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:		swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SABOTAGE_INFANTRY_EQUIPMENT_TITLE]); break;
	case RCAM_SABOTAGE_MECHANICAL_UNITS:		swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SABOTAGE_MECHANICAL_UNITS_TITLE]); break;
	case RCAM_SEND_SUPPLIES_TO_TOWN:			swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SEND_SUPPLIES_TO_TOWN_TITLE]); break;
	case RCAM_SOLDIER_BOUNTIES_KINGPIN:			swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SOLDIER_BOUNTIES_KINGPIN_TITLE]); break;
	case RCAM_TRAIN_MILITIA_ANYWHERE:			swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_TRAIN_MILITIA_ANYWHERE_TITLE]); break;

	default:									swprintf(sText, L"Mission Index: %d", rebelCommandSaveInfo.availableMissions[index]); break;
	}
	DrawTextToScreen(sText, x+5, y+5, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// draw mission base duration, in days
	UINT8 missionDurationBase = 0;
	switch (rebelCommandSaveInfo.availableMissions[index])
	{
	case RCAM_DEEP_DEPLOYMENT:					missionDurationBase = gRebelCommandSettings.iDeepDeploymentDuration; break;
	case RCAM_DISRUPT_ASD:						missionDurationBase = gRebelCommandSettings.iDisruptAsdDuration; break;
	case RCAM_GET_ENEMY_MOVEMENT_TARGETS:		missionDurationBase = gRebelCommandSettings.iGetEnemyMovementTargetsDuration; break;
	case RCAM_IMPROVE_LOCAL_SHOPS:				missionDurationBase = gRebelCommandSettings.iImproveLocalShopsDuration; break;
	case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:	missionDurationBase = gRebelCommandSettings.iReduceStrategicDecisionSpeedDuration; break;
	case RCAM_REDUCE_UNALERTED_ENEMY_VISION:	missionDurationBase = gRebelCommandSettings.iReduceUnalertedEnemyVisionDuration; break;
	case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:		missionDurationBase = gRebelCommandSettings.iSabotageInfantryEquipmentDuration; break;
	case RCAM_SABOTAGE_MECHANICAL_UNITS:		missionDurationBase = gRebelCommandSettings.iSabotageMechanicalUnitsDuration; break;
	case RCAM_SEND_SUPPLIES_TO_TOWN:			missionDurationBase = gRebelCommandSettings.iSendSuppliesToTownDuration; break;
	case RCAM_SOLDIER_BOUNTIES_KINGPIN:			missionDurationBase = gRebelCommandSettings.iSoldierBountiesKingpinDuration; break;
	case RCAM_TRAIN_MILITIA_ANYWHERE:			missionDurationBase = gRebelCommandSettings.iTrainMilitiaAnywhereDuration; break;

	default: break;
	}
	// convert from hours
	missionDurationBase /= 24;
	swprintf(sText, szRebelCommandText[RCT_MISSION_DURATION_DAYS], missionDurationBase);
	DrawTextToScreen(sText, x+5, y+21, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// draw mission base success chance
	int missionSuccessChanceBase = 50;
	switch (rebelCommandSaveInfo.availableMissions[index])
	{
	case RCAM_DEEP_DEPLOYMENT:					missionSuccessChanceBase = gRebelCommandSettings.iDeepDeploymentSuccessChance; break;
	case RCAM_DISRUPT_ASD:						missionSuccessChanceBase = gRebelCommandSettings.iDisruptAsdSuccessChance; break;
	case RCAM_GET_ENEMY_MOVEMENT_TARGETS:		missionSuccessChanceBase = gRebelCommandSettings.iGetEnemyMovementTargetsSuccessChance; break;
	case RCAM_IMPROVE_LOCAL_SHOPS:				missionSuccessChanceBase = gRebelCommandSettings.iImproveLocalShopsSuccessChance; break;
	case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:	missionSuccessChanceBase = gRebelCommandSettings.iReduceStrategicDecisionSpeedSuccessChance; break;
	case RCAM_REDUCE_UNALERTED_ENEMY_VISION:	missionSuccessChanceBase = gRebelCommandSettings.iReduceUnalertedEnemyVisionSuccessChance; break;
	case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:		missionSuccessChanceBase = gRebelCommandSettings.iSabotageInfantryEquipmentSuccessChance; break;
	case RCAM_SABOTAGE_MECHANICAL_UNITS:		missionSuccessChanceBase = gRebelCommandSettings.iSabotageMechanicalUnitsSuccessChance; break;
	case RCAM_SEND_SUPPLIES_TO_TOWN:			missionSuccessChanceBase = gRebelCommandSettings.iSendSuppliesToTownSuccessChance; break;
	case RCAM_SOLDIER_BOUNTIES_KINGPIN:			missionSuccessChanceBase = gRebelCommandSettings.iSoldierBountiesKingpinSuccessChance; break;
	case RCAM_TRAIN_MILITIA_ANYWHERE:			missionSuccessChanceBase = gRebelCommandSettings.iTrainMilitiaAnywhereSuccessChance; break;
		
	default: break;
	}
	swprintf(sText, szRebelCommandText[RCT_MISSION_SUCCESS_CHANCE], missionSuccessChanceBase, L"%%");
	DrawTextToScreen(sText, x+5, y+33, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// draw mission description
	switch (rebelCommandSaveInfo.availableMissions[index])
	{
	case RCAM_DEEP_DEPLOYMENT:					swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_DEEP_DEPLOYMENT_DESC]); break;
	case RCAM_DISRUPT_ASD:						swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_DISRUPT_ASD_DESC]); break;
	case RCAM_GET_ENEMY_MOVEMENT_TARGETS:		swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_GET_ENEMY_MOVEMENT_TARGETS_DESC]); break;
	case RCAM_IMPROVE_LOCAL_SHOPS:				swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_IMPROVE_LOCAL_SHOPS_DESC]); break;
	case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:	swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_REDUCE_STRATEGIC_DECISION_SPEED_DESC]); break;
	case RCAM_REDUCE_UNALERTED_ENEMY_VISION:	swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_REDUCE_UNALERTED_ENEMY_VISION_DESC]); break;
	case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:		swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SABOTAGE_INFANTRY_EQUIPMENT_DESC]); break;
	case RCAM_SABOTAGE_MECHANICAL_UNITS:		swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SABOTAGE_MECHANICAL_UNITS_DESC]); break;
	case RCAM_SEND_SUPPLIES_TO_TOWN:			swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SEND_SUPPLIES_TO_TOWN_DESC]); break;
	case RCAM_SOLDIER_BOUNTIES_KINGPIN:			swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_SOLDIER_BOUNTIES_KINGPIN_DESC], gRebelCommandSettings.iSoldierBountiesKingpinPayout_Limit); break;
	case RCAM_TRAIN_MILITIA_ANYWHERE:			swprintf(sText, szRebelCommandAgentMissionsText[RCAMT_TRAIN_MILITIA_ANYWHERE_DESC]); break;

	default: swprintf(sText, L"Mission description goes here. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut faucibus libero dui. Etiam facilisis posuere dictum. Etiam a velit viverra, interdum eros non, placerat lectus. Vivamus ut lorem id velit tempus auctor. Donec molestie, erat at molestie malesuada, diam purus tincidunt eros, vel hendrerit mi elit vitae leo. Suspendisse dui lectus, malesuada eu elementum at, viverra eu odio."); break;
	}
	DisplayWrappedString(x+5, y+45, 220, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, sText, FONT_MCOLOR_BLACK, FALSE, 0);

	if (agentIndex[index] == mercs.size()) // generic rebel agent
	{
		// draw black box for face
		ColorFillVideoSurfaceArea(FRAME_BUFFER, x+5, y+150+10, x+5+48, y+150+10+43, Get16BPPColor(FROMRGB(64, 64, 64)));

		// draw question mark
		SetFontShadow(NO_SHADOW);
		DrawTextToScreen(L"?", x+5+20, y+150+10+16, 0, FONT14HUMANIST, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, 0);
		SetFontShadow(FONT_GRAY1);

		// draw name
		swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_NAME], szRebelCommandText[RCT_MISSION_AGENT_REDACTED]);
		DrawTextToScreen(sText, x+55, y+150+10, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// draw location
		swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_LOCATION], szRebelCommandText[RCT_MISSION_AGENT_REDACTED]);
		DrawTextToScreen(sText, x+55, y+150+22, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// draw assignment
		swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_ASSIGNMENT], szRebelCommandText[RCT_NONE]);
		DrawTextToScreen(sText, x+55, y+150+34, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// draw contract
		DrawTextToScreen(szRebelCommandText[RCT_MISSION_AGENT_CONTRACT_NONE], x+55, y+150+46, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	}
	else // one of the player's mercs
	{
		// draw face
		vObjDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		sprintf(sTemp, "FACES\\%02d.sti", gMercProfiles[mercs[agentIndex[index]]->ubProfile].ubFaceIndex);
		FilenameForBPP(sTemp, vObjDesc.ImageFile);
		CHECKF(AddVideoObject(&vObjDesc, &image));
		GetVideoObject(&hvObj, image);
		BltVideoObject(FRAME_BUFFER, hvObj, 0, x+5, y+150+10, VO_BLT_SRCTRANSPARENCY, NULL);

		// draw name
		swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_NAME], gMercProfiles[mercs[agentIndex[index]]->ubProfile].zName);
		DrawTextToScreen(sText, x+55, y+150+10, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// draw location
		CHAR16 locationStr[128];
		GetSectorIDString(mercs[agentIndex[index]]->sSectorX, mercs[agentIndex[index]]->sSectorY, 0, locationStr, TRUE);
		swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_LOCATION], locationStr);
		DrawTextToScreen(sText, x+55, y+150+22, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// draw assignment
		swprintf( sText, szRebelCommandText[RCT_MISSION_AGENT_ASSIGNMENT], pAssignmentStrings[mercs[agentIndex[index]]->bAssignment]);
		DrawTextToScreen(sText, x+55, y+150+34, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		// draw contract
		const BOOLEAN fromAim = mercs[agentIndex[index]]->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC;
		
		if (fromAim)
		{
			const INT32 endTime = mercs[agentIndex[index]]->iEndofContractTime;
			const INT32 worldMin = GetWorldTotalMin();
			const INT32 remaining = endTime - worldMin;

			if (remaining >= 24 * 60)
			{
				swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_CONTRACT_DAYS], remaining / (24 * 60));
			}
			else
			{
				swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_CONTRACT_HOURS], remaining / 60);
			}
		}
		else
		{
			swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_CONTRACT_NONE]);
		}
		DrawTextToScreen(sText, x+55, y+150+46, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	}

	// draw btns under face
	btnId = CreateTextButton(L"<<", FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x+5, y+150+54, 24, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			const INT8 index = MSYS_GetBtnUserData(btn, 0);
			ButtonHelper(btn, reason, [btn, index]() { agentIndex[index]--; });
		});
	MSYS_SetBtnUserData(btnId, 0, index);
	btnIds.push_back(btnId);

	btnId = CreateTextButton(L">>", FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x+5+24, y+150+54, 24, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			const INT8 index = MSYS_GetBtnUserData(btn, 0);
			ButtonHelper(btn, reason, [btn, index]() { agentIndex[index]++; });
		});
	MSYS_SetBtnUserData(btnId, 0, index);
	btnIds.push_back(btnId);

	// draw agent bonus header text
	swprintf(sText, szRebelCommandText[RCT_MISSION_AGENT_BONUS]);
	DrawTextToScreen(sText, x+5, y+150+54+20+2, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	
	// draw agent bonus text
	UINT32 durationBonus = 0;
	int durationBonusSkill = 0;
	std::vector<std::wstring> agentBonusText;
	if (agentIndex[index] < static_cast<INT8>(mercs.size()))
	{
		const MERCPROFILESTRUCT merc = gMercProfiles[mercs[agentIndex[index]]->ubProfile];
		const INT8 successBonus_expLevel = GetMissionSuccessChanceBonus(&merc);
		CHAR16 successText[100];
		// stupid string hack to get the percent sign to display correctly
		swprintf(successText, szRebelCommandText[RCT_MISSION_BONUS_SUCCESS_CHANCE], successBonus_expLevel, L"%s", pShortAttributeStrings[5]); // "Lvl"
		agentBonusText.push_back(successText);

		const STR16* locSkillText = gGameOptions.fNewTraitSystem ? gzMercSkillTextNew : gzMercSkillText;
		INT16 intModifier;
		int intModifierSkill;
		FLOAT floatModifier;
		int floatModifierSkill;
		UINT16 extraBits;
		MissionHelpers::GetMissionInfo(static_cast<RebelCommandAgentMissions>(rebelCommandSaveInfo.availableMissions[index]), &merc, durationBonus, floatModifier, intModifier, durationBonusSkill, floatModifierSkill, intModifierSkill, extraBits);
		switch (rebelCommandSaveInfo.availableMissions[index])
		{
		case RCAM_DEEP_DEPLOYMENT:
		{
			intModifier = max(intModifier, gRebelCommandSettings.iDeepDeploymentRangeEW);
			CHAR16 rangeText[100];
			swprintf(rangeText, szRebelCommandText[RCT_MISSION_BONUS_DEPLOY_RANGE], intModifier, locSkillText[intModifierSkill]);
			agentBonusText.push_back(rangeText);
		}
		break;

		case RCAM_DISRUPT_ASD:
		{
			CHAR16 text[100];
			floatModifier = max(floatModifier, gRebelCommandSettings.fDisruptAsdIncomeReductionModifier);
			floatModifier *= 100.f;
			swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_ASD_INCOME_REDUCTION], floatModifier, L"%s", locSkillText[floatModifierSkill]);
			agentBonusText.push_back(text);

			if (gGameOptions.fNewTraitSystem)
			{
				switch (extraBits)
				{
				case MissionHelpers::DISRUPT_ASD_STEAL_FUEL:
				{
					const UINT8 townId = GetTownIdForSector(BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y);
					swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_STEAL_FUEL], pTownNames[townId], locSkillText[TECHNICIAN_NT]);
					agentBonusText.push_back(text);
				}
				break;

				case MissionHelpers::DISRUPT_ASD_DESTROY_RESERVES:
				{
					swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_DESTROY_RESERVES], locSkillText[DEMOLITIONS_NT]);
					agentBonusText.push_back(text);
				}
				break;
				}
			}
		}

		case RCAM_GET_ENEMY_MOVEMENT_TARGETS:
		{
			// no special modifiers. included for completeness.
		}
		break;

		case RCAM_IMPROVE_LOCAL_SHOPS:
		{
			// no special modifiers. included for completeness.
		}
		break;

		case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:
		{
			floatModifier = max(floatModifier, gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier);
			floatModifier -= 1.f;
			floatModifier *= 100.f;
			CHAR16 rangeText[100];
			swprintf(rangeText, szRebelCommandText[RCT_MISSION_BONUS_DECISION_TIME], floatModifier, L"%s", locSkillText[floatModifierSkill]);
			agentBonusText.push_back(rangeText);
		}
		break;

		case RCAM_REDUCE_UNALERTED_ENEMY_VISION:
		{
			floatModifier = max(floatModifier, gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier);
			floatModifier *= 100.f;
			CHAR16 text[100];
			swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_UNALERTED_VISION_PENALTY], floatModifier, L"%s", locSkillText[floatModifierSkill]);
			agentBonusText.push_back(text);
		}
		break;

		case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:
		{
			intModifier = max(intModifier, gRebelCommandSettings.iSabotageInfantryEquipmentModifier);
			CHAR16 text[100];
			swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_INFANTRY_GEAR_QUALITY], intModifier, locSkillText[intModifierSkill]);
			agentBonusText.push_back(text);
		}
		break;

		case RCAM_SABOTAGE_MECHANICAL_UNITS:
		{
			intModifier = max(intModifier, gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss);
			CHAR16 text[100];
			swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_MECHANICAL_STAT_LOSS], intModifier, locSkillText[intModifierSkill]);
			agentBonusText.push_back(text);
		}
		break;

		case RCAM_SEND_SUPPLIES_TO_TOWN:
		{
			// duration here is affected by lvl instead of a specific skill
			CHAR16 durationText[100];
			swprintf(durationText, szRebelCommandText[RCT_MISSION_BONUS_DURATION], SendSuppliesToTownDurationBonus(&merc), pShortAttributeStrings[5]); // "Lvl"
			agentBonusText.push_back(durationText);
		}
		break;

		case RCAM_SOLDIER_BOUNTIES_KINGPIN:
		{
			floatModifier = max(floatModifier, 1.f);
			CHAR16 text[100];
			if (floatModifier > 1.f)
			{
				floatModifier *= 100;
				floatModifier -= 100;
				swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_PAYOUT], floatModifier, L"%s", locSkillText[floatModifierSkill]);
				agentBonusText.push_back(text);
			}

			if (intModifier > 0)
			{
				swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_PAYOUT_LIMIT_INCREASE], intModifier, locSkillText[intModifierSkill]);
				agentBonusText.push_back(text);
			}

			if (extraBits == MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_OFFICER_PAYOUTS)
			{
				swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_OFFICER_PAYOUT], locSkillText[floatModifierSkill]);
				agentBonusText.push_back(text);
			}
			else if (extraBits == MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_VEHICLE_PAYOUTS)
			{
				swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_VEHICLE_PAYOUT], locSkillText[floatModifierSkill]);
				agentBonusText.push_back(text);
			}
		}
		break;

		case RCAM_TRAIN_MILITIA_ANYWHERE:
		{
			intModifier = max(intModifier, 1);
			CHAR16 text[100];
			swprintf(text, szRebelCommandText[RCT_MISSION_BONUS_MAX_TRAINERS], intModifier, locSkillText[intModifierSkill]);
			agentBonusText.push_back(text);
		}
		break;

		default: break;
		}

		if (durationBonus > 0)
		{
			CHAR16 durationText[100];
			swprintf(durationText, szRebelCommandText[RCT_MISSION_BONUS_DURATION], durationBonus, locSkillText[durationBonusSkill]);
			agentBonusText.push_back(durationText);
		}
	}

	if (agentBonusText.size() == 0)
	{
		DrawTextToScreen(szRebelCommandText[RCT_NONE], x+10, y+150+54+20+2+11, 0, FONT10ARIAL, FONT_MCOLOR_RED, FONT_MCOLOR_BLACK, FALSE, 0);
	}
	else
	{
		for (UINT8 i = 0; i < agentBonusText.size(); ++i)
		{
			// the percent sign here is a hack to get it to display properly for string that need a percent sign
			swprintf(sText, agentBonusText[i].c_str(), L"%%");
			DrawTextToScreen(sText, x+10, y+150+54+20+2+11*(i+1), 0, FONT10ARIAL, FONT_GREEN, FONT_MCOLOR_BLACK, FALSE, 0);
		}
	}

	// draw "start mission" button
	BOOLEAN canStartMission = TRUE;
	if (agentIndex[index] < static_cast<INT8>(mercs.size()))
	{
		const UINT8 townId = GetTownIdForSector(mercs[agentIndex[index]]->sSectorX, mercs[agentIndex[index]]->sSectorY);
		const UINT8 townLoyalty = GetRegionLoyalty(townId);
		const INT32 endTime = mercs[agentIndex[index]]->iEndofContractTime;
		const INT32 worldMin = GetWorldTotalMin();
		const INT32 remaining = endTime - worldMin;

		if (townId < FIRST_TOWN || townId >= NUM_TOWNS || !gfTownUsesLoyalty[townId])
		{
			canStartMission = FALSE;
			swprintf(sText, szRebelCommandText[RCT_MISSION_CANT_START_NOT_IN_TOWN]);
		}
		else if (townLoyalty < gRebelCommandSettings.iMinLoyaltyForMission && rebelCommandSaveInfo.availableMissions[index] != RCAM_SEND_SUPPLIES_TO_TOWN)
		{
			canStartMission = FALSE;
			swprintf(sText, szRebelCommandText[RCT_MISSION_CANT_START_LOW_LOYALTY]);
		}
		else if (mercs[agentIndex[index]]->bAssignment == ASSIGNMENT_POW || mercs[agentIndex[index]]->bAssignment == ASSIGNMENT_MINIEVENT || mercs[agentIndex[index]]->bAssignment == ASSIGNMENT_REBELCOMMAND)
		{
			canStartMission = FALSE;
			swprintf(sText, szRebelCommandText[RCT_MISSION_CANT_START_AGENT_UNAVAILABLE]);
		}
		else if (mercs[agentIndex[index]]->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC && remaining < 24 * 60)
		{
			canStartMission = FALSE;
			swprintf(sText, szRebelCommandText[RCT_MISSION_CANT_START_CONTRACT_EXPIRING]);
		}
	}
	else if (agentIndex[index] == mercs.size() && rebelCommandSaveInfo.availableMissions[index] == RCAM_SEND_SUPPLIES_TO_TOWN)
	{
		canStartMission = FALSE;
		swprintf(sText, szRebelCommandText[RCT_MISSION_CANT_USE_REBEL_AGENT]);
	}
	else if ((gTacticalStatus.uiFlags & INCOMBAT) || gTacticalStatus.fEnemyInSector)
	{
		canStartMission = FALSE;
		swprintf(sText, szRebelCommandText[RCT_MISSION_CANT_START_BATTLE_IN_PROGRESS]);
		DrawTextToScreen(sText, x, y+295, 231, FONT10ARIAL, FONT_RED, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	if (canStartMission)
	{
		swprintf(sText, szRebelCommandText[RCT_MISSION_START_BUTTON], GetMissionCost());
		btnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y+290, 231, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				const INT8 index = MSYS_GetBtnUserData(btn, 0);
				ButtonHelper(btn, reason, [btn, index]() {
					PrepareMission(index);
				});
			});
		MSYS_SetBtnUserData(btnId, 0, index);
		btnIds.push_back(btnId);
	}
	else
	{
		DrawTextToScreen(sText, x, y+295, 231, FONT10ARIAL, FONT_RED, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	return TRUE;
}

void RenderMissionOverview()
{
	CHAR16 sText[800];
	INT32 btnId;

	// title
	RenderHeader(RCT_AGENT_OVERVIEW);

	// display help button
	btnId = CreateTextButton(L"?", FONT12ARIAL, FONT_MCOLOR_WHITE, FONT_BLACK, BUTTON_USE_DEFAULT, WEBSITE_LEFT + 15, WEBSITE_TOP + 40, 30, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []()
			{
				missionOverviewSubview = missionOverviewSubview == MOS_HELP ? MOS_MISSION_LIST : MOS_HELP;
			});
		});

	btnIds.push_back(btnId);

	// toggle between mission picker and active mission effects
	switch (missionOverviewSubview)
	{
	case MOS_MISSION_LIST:
		swprintf(sText, szRebelCommandText[RCT_MISSION_VIEW_ACTIVE]);
		break;

	case MOS_ACTIVE_MISSION_EFFECTS:
	case MOS_HELP:
		swprintf(sText, szRebelCommandText[RCT_MISSION_VIEW_LIST]);
		break;
	}
	btnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, WEBSITE_LEFT + 50, WEBSITE_TOP + 40, 435, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []()
			{
				switch (missionOverviewSubview)
				{
				case MOS_MISSION_LIST:
					missionOverviewSubview = MOS_ACTIVE_MISSION_EFFECTS;
					break;

				case MOS_ACTIVE_MISSION_EFFECTS:
				case MOS_HELP:
					missionOverviewSubview = MOS_MISSION_LIST;
					break;
				}
			});
		});

	btnIds.push_back(btnId);

	// main body
	switch (missionOverviewSubview)
	{
	case MOS_MISSION_LIST:
		if (rebelCommandSaveInfo.availableMissions[0] == RCAM_NONE)
		{
			UINT32 nextMissionAvailableDay = GetWorldDay();
			const INT8 interval = gRebelCommandSettings.iMissionRefreshTimeDays;
			nextMissionAvailableDay += (interval - (nextMissionAvailableDay % interval));
			
			if (missionMap.size() > 0)
			{
				DrawTextToScreen(szRebelCommandText[RCT_MISSION_PREP_IN_PROGRESS], WEBSITE_LEFT + 15, WEBSITE_TOP + 155, WEBSITE_WIDTH - 30, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
			}

			swprintf(sText, szRebelCommandText[RCT_MISSION_NEXT_AVAILABILITY], nextMissionAvailableDay);
			DrawTextToScreen(sText, WEBSITE_LEFT + 15, WEBSITE_TOP + 175, WEBSITE_WIDTH - 30, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
		}
		else
		{
			for (int i = 0; i < NUM_ARC_AGENT_SLOTS; ++i)
			{
				SetupMissionAgentBox(WEBSITE_LEFT + 15 + 240 * i, WEBSITE_TOP + 65, i);
			}
		}
		break;

	case MOS_ACTIVE_MISSION_EFFECTS:
		{
			DrawTextToScreen(szRebelCommandText[RCT_MISSION_ACTIVE_MISSIONS], WEBSITE_LEFT + 25, WEBSITE_TOP + 75, 0, FONT12ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
			if (missionMap.size() == 0)
			{
				DrawTextToScreen(szRebelCommandText[RCT_NONE], WEBSITE_LEFT + 35, WEBSITE_TOP + 90, 0, FONT12ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
			}
			else
			{
				std::vector<std::wstring> evt1Strings;
				std::vector<std::wstring> evt2Strings;
				std::vector<std::pair<UINT32,UINT32>> missions = GetAllStrategicEventsOfType(EVENT_REBELCOMMAND);
				for (std::vector<std::pair<UINT32, UINT32>>::iterator it = missions.begin(); it != missions.end(); ++it)
				{
					const UINT32 eventTime = it->first;
					const UINT32 day = eventTime / 60 / 60 / 24;
					const UINT32 hour = (eventTime % (24 * 60 * 60)) / 60 / 60;
					const UINT32 minute = (eventTime % (24 * 60 * 60)) % (60 * 60);
					MissionFirstEvent evt1;
					MissionSecondEvent evt2;
					DeserialiseMissionFirstEvent(it->second, evt1);
					DeserialiseMissionSecondEvent(it->second, evt2);

					if (evt1.isFirstEvent)
					{
						CHAR16 prepText[200];
						swprintf(prepText, szRebelCommandText[RCT_MISSION_LIST_PREPARING], szRebelCommandAgentMissionsText[evt1.missionId * 2], day, hour, minute);
						evt1Strings.push_back(prepText);
					}
					else if (evt2.isSecondEvent)
					{
						CHAR16 missionText[200];
						swprintf(missionText, szRebelCommandText[RCT_MISSION_LIST_ACTIVE], szRebelCommandAgentMissionsText[evt2.missionId * 2], day, hour, minute);
						evt2Strings.push_back(missionText);
					}
				}

				UINT16 y = WEBSITE_TOP + 90;
				for (std::vector<std::wstring>::size_type i = 0; i < evt1Strings.size(); ++i)
				{
					DrawTextToScreen(const_cast<STR16>(evt1Strings[i].c_str()), WEBSITE_LEFT + 35, y, 0, FONT12ARIAL, FONT_DKYELLOW, FONT_MCOLOR_BLACK, FALSE, 0);
					y += 15;
				}

				for (std::vector<std::wstring>::size_type i = 0; i < evt2Strings.size(); ++i)
				{
					DrawTextToScreen(const_cast<STR16>(evt2Strings[i].c_str()), WEBSITE_LEFT + 35, y, 0, FONT12ARIAL, FONT_DKGREEN, FONT_MCOLOR_BLACK, FALSE, 0);
					y += 15;
				}
			}
		}
		break;

	case MOS_HELP:
	{
		UINT16 y = WEBSITE_TOP + 100;
		swprintf(sText, szRebelCommandText[RCT_MISSION_HELP_1], gRebelCommandSettings.iMissionPrepTime);
		y += DisplayWrappedString(WEBSITE_LEFT + 35, y, 400, 2, FONT12ARIAL, FONT_MCOLOR_BLACK, sText, FONT_MCOLOR_BLACK, FALSE, 0);
		y += 20;
		y += DisplayWrappedString(WEBSITE_LEFT + 35, y, 400, 2, FONT12ARIAL, FONT_MCOLOR_BLACK, szRebelCommandText[RCT_MISSION_HELP_2], FONT_MCOLOR_BLACK, FALSE, 0);
		y += 20;
		DisplayWrappedString(WEBSITE_LEFT + 35, y, 400, 2, FONT12ARIAL, FONT_MCOLOR_BLACK, szRebelCommandText[RCT_MISSION_HELP_3], FONT_MCOLOR_BLACK, FALSE, 0);
	}
	break;
	}
	
	// "new missions every X hours" text
	swprintf(sText, szRebelCommandText[RCT_NEW_MISSIONS_AVAILABLE_TIME], gRebelCommandSettings.iMissionRefreshTimeDays * 24);
	DrawTextToScreen(sText, WEBSITE_LEFT + 22, WEBSITE_TOP + WEBSITE_HEIGHT - 14, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
}

void PrepareMission(INT8 index)
{
	const INT32 cost = GetMissionCost();
	if (rebelCommandSaveInfo.iSupplies < cost)
	{
		DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return;
	}

	// confirmation popup
	std::vector<SOLDIERTYPE*> mercs;
	for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[i];

		if (pSoldier && pSoldier->bActive
			&& !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE)
			)
		{
			mercs.push_back(pSoldier);
		}
	}

	const MERCPROFILESTRUCT* merc = agentIndex[index] == mercs.size() ? nullptr : &gMercProfiles[mercs[agentIndex[index]]->ubProfile];
	CHAR16 text[400];
	RebelCommandAgentMissionsText missionTitle;
	INT8 missionSuccessChance;
	UINT8 missionDuration;
	UINT32 durationBonus;
	FLOAT floatModifier;
	INT16 intModifier;
	int durSkill;
	int floatSkill;
	int intSkill;
	UINT16 extraBits;

	MissionHelpers::GetMissionInfo(static_cast<RebelCommandAgentMissions>(rebelCommandSaveInfo.availableMissions[index]), merc, durationBonus, floatModifier, intModifier, durSkill, floatSkill, intSkill, extraBits);
	switch (rebelCommandSaveInfo.availableMissions[index])
	{
	case RCAM_DEEP_DEPLOYMENT:					
	{
		missionTitle = RCAMT_DEEP_DEPLOYMENT_TITLE;
		missionSuccessChance = gRebelCommandSettings.iDeepDeploymentSuccessChance;
		missionDuration = gRebelCommandSettings.iDeepDeploymentDuration;
	}
	break;

	case RCAM_DISRUPT_ASD:
	{
		missionTitle = RCAMT_DISRUPT_ASD_TITLE;
		missionSuccessChance = gRebelCommandSettings.iDisruptAsdSuccessChance;
		missionDuration = gRebelCommandSettings.iDisruptAsdDuration;
	}
	break;

	case RCAM_GET_ENEMY_MOVEMENT_TARGETS:
	{
		missionTitle = RCAMT_GET_ENEMY_MOVEMENT_TARGETS_TITLE;
		missionSuccessChance = gRebelCommandSettings.iGetEnemyMovementTargetsSuccessChance;
		missionDuration = gRebelCommandSettings.iGetEnemyMovementTargetsDuration;
	}
	break;

	case RCAM_IMPROVE_LOCAL_SHOPS:
	{
		missionTitle = RCAMT_IMPROVE_LOCAL_SHOPS_TITLE;
		missionSuccessChance = gRebelCommandSettings.iImproveLocalShopsSuccessChance;
		missionDuration = gRebelCommandSettings.iImproveLocalShopsDuration;
	}
	break;

	case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:
	{
		missionTitle = RCAMT_REDUCE_STRATEGIC_DECISION_SPEED_TITLE;
		missionSuccessChance = gRebelCommandSettings.iReduceStrategicDecisionSpeedSuccessChance;
		missionDuration = gRebelCommandSettings.iReduceStrategicDecisionSpeedDuration;
	}
	break;

	case RCAM_REDUCE_UNALERTED_ENEMY_VISION:
	{
		missionTitle = RCAMT_REDUCE_UNALERTED_ENEMY_VISION_TITLE;
		missionSuccessChance = gRebelCommandSettings.iReduceUnalertedEnemyVisionSuccessChance;
		missionDuration = gRebelCommandSettings.iReduceUnalertedEnemyVisionDuration;
	}
	break;

	case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:
	{
		missionTitle = RCAMT_SABOTAGE_INFANTRY_EQUIPMENT_TITLE;
		missionSuccessChance = gRebelCommandSettings.iSabotageInfantryEquipmentSuccessChance;
		missionDuration = gRebelCommandSettings.iSabotageInfantryEquipmentDuration;
	}
	break;

	case RCAM_SABOTAGE_MECHANICAL_UNITS:
	{
		missionTitle = RCAMT_SABOTAGE_MECHANICAL_UNITS_TITLE;
		missionSuccessChance = gRebelCommandSettings.iSabotageMechanicalUnitsSuccessChance;
		missionDuration = gRebelCommandSettings.iSabotageMechanicalUnitsDuration;
	}
	break;

	case RCAM_SEND_SUPPLIES_TO_TOWN:
	{
		missionTitle = RCAMT_SEND_SUPPLIES_TO_TOWN_TITLE;
		missionSuccessChance = gRebelCommandSettings.iSendSuppliesToTownSuccessChance;
		missionDuration = gRebelCommandSettings.iSendSuppliesToTownDuration + SendSuppliesToTownDurationBonus(merc);
		extraBits = SECTOR(merc->sSectorX, merc->sSectorY);
	}
	break;

	case RCAM_SOLDIER_BOUNTIES_KINGPIN:
	{
		missionTitle = RCAMT_SOLDIER_BOUNTIES_KINGPIN_TITLE;
		missionSuccessChance = gRebelCommandSettings.iSoldierBountiesKingpinSuccessChance;
		missionDuration = gRebelCommandSettings.iSoldierBountiesKingpinDuration;
	}
	break;
		
	case RCAM_TRAIN_MILITIA_ANYWHERE:
	{
		missionTitle = RCAMT_TRAIN_MILITIA_ANYWHERE_TITLE;
		missionSuccessChance = gRebelCommandSettings.iTrainMilitiaAnywhereSuccessChance;
		missionDuration = gRebelCommandSettings.iTrainMilitiaAnywhereDuration;
	}
	break;

	default: break;
	}

	missionSuccessChance += GetMissionSuccessChanceBonus(merc);

	if (Random(100) > static_cast<UINT8>(missionSuccessChance))
	{
		// mission failed!
		missionDuration = 0;
	}
	else
	{
		missionDuration += durationBonus;
	}

	swprintf(text, szRebelCommandText[RCT_MISSION_POPUP_PART1], szRebelCommandAgentMissionsText[missionTitle], cost);

	if (agentIndex[index] == mercs.size())
	{
		// sent a generic rebel
		MissionHelpers::missionParam = SerialiseMissionFirstEvent(TRUE, 0 /* no profile needed */, static_cast<RebelCommandAgentMissions>(rebelCommandSaveInfo.availableMissions[index]), missionDuration, 0 /* no extra data */);
		swprintf(text, szRebelCommandText[RCT_MISSION_POPUP_PART2_GENERIC], text);
	}
	else
	{
		MissionHelpers::missionParam = SerialiseMissionFirstEvent(FALSE, mercs[agentIndex[index]]->ubProfile, static_cast<RebelCommandAgentMissions>(rebelCommandSaveInfo.availableMissions[index]), missionDuration, static_cast<UINT8>(extraBits));
		if (merc->bSex == MALE)
			swprintf(text, szRebelCommandText[RCT_MISSION_POPUP_PART2_MALE], text, merc->zNickname, gRebelCommandSettings.iMissionPrepTime);
		else
			swprintf(text, szRebelCommandText[RCT_MISSION_POPUP_PART2_FEMALE], text, merc->zNickname, gRebelCommandSettings.iMissionPrepTime);
	}

	DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, text, LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, [](UINT8 exitValue) {
		if (exitValue == MSG_BOX_RETURN_YES)
		{
			MissionFirstEvent evt;
			DeserialiseMissionFirstEvent(MissionHelpers::missionParam, evt);

			if (!evt.sentGenericRebelAgent)
			{
				for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
				{
					SOLDIERTYPE* pSoldier = MercPtrs[i];
					if (pSoldier->ubProfile == evt.mercProfileId)
					{
						TakeSoldierOutOfVehicle(pSoldier);
						RemoveCharacterFromSquads(pSoldier);
						pSoldier->bSectorZ += REBEL_COMMAND_Z_OFFSET;
						pSoldier->bBleeding = 0;
						SetTimeOfAssignmentChangeForMerc(pSoldier);
						ChangeSoldiersAssignment(pSoldier, ASSIGNMENT_REBELCOMMAND);
						break;
					}
				}
			}

			// disable missions until next refresh
			for (INT8 i = 0; i < NUM_ARC_AGENT_SLOTS; ++i)
			{
				rebelCommandSaveInfo.availableMissions[i] = RCAM_NONE;
			}

			// queue up the mission start event. make sure we use the top of the hour because I'm lazy and we're handling the assignment here instead of Assignments.cpp
			const UINT32 time = GetWorldTotalMin();
			AddStrategicEvent(EVENT_REBELCOMMAND, time + (60 - time % 60) + 60 * gRebelCommandSettings.iMissionPrepTime, MissionHelpers::missionParam);
			missionMap.insert(std::make_pair(static_cast<RebelCommandAgentMissions>(evt.missionId), MissionHelpers::missionParam));

			rebelCommandSaveInfo.iSupplies -= GetMissionCost();
		}

		// update the mission list to show that we've started
		redraw = TRUE;
	});
}
// end website

void ApplyEnemyPenalties(SOLDIERTYPE* pSoldier)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	const auto applyPenalties = [](SOLDIERTYPE* pSoldier, UINT8 level)
	{
		pSoldier->stats.bLife -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bLifeMax -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bAgility -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bDexterity -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bStrength -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bWisdom -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bLeadership -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bMarksmanship -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bMechanical -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bExplosive -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->stats.bMedical -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->bBreath -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);
		pSoldier->bBreathMax -= static_cast<INT8>(info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level);

		pSoldier->stats.bLife = static_cast<INT8>(max(25, pSoldier->stats.bLife));
		pSoldier->stats.bLifeMax = static_cast<INT8>(max(25, pSoldier->stats.bLifeMax));
		pSoldier->stats.bAgility = static_cast<INT8>(max(25, pSoldier->stats.bAgility));
		pSoldier->stats.bDexterity = static_cast<INT8>(max(25, pSoldier->stats.bDexterity));
		pSoldier->stats.bStrength = static_cast<INT8>(max(25, pSoldier->stats.bStrength));
		pSoldier->stats.bWisdom = static_cast<INT8>(max(25, pSoldier->stats.bStrength));
		pSoldier->stats.bLeadership = static_cast<INT8>(max(25, pSoldier->stats.bLeadership));
		pSoldier->stats.bMarksmanship = static_cast<INT8>(max(25, pSoldier->stats.bMarksmanship));
		pSoldier->stats.bMechanical = static_cast<INT8>(max(25, pSoldier->stats.bMechanical));
		pSoldier->stats.bExplosive = static_cast<INT8>(max(25, pSoldier->stats.bExplosive));
		pSoldier->stats.bMedical = static_cast<INT8>(max(25, pSoldier->stats.bMedical));
		pSoldier->bBreath = static_cast<INT8>(max(25, info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level));
		pSoldier->bBreathMax = static_cast<INT8>(max(25, info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1 * level));
	};

	// need to get dist between soldier and town.
	// run through all towns, check manhattan distance
	UINT8 foundLevel = 0;
	UINT8 foundLoyalty = 0;
	for (int a = FIRST_TOWN; a < NUM_TOWNS; ++a)
	{
		// make sure town has active admins
		if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_ACTIVE)
			continue;

		// make sure action exists in this town
		const INT16 index = GetAdminActionInRegion(a, RCAA_SUPPLY_DISRUPTION);
		if (index == -1)
			continue;

		// and that it's not level 0
		const UINT8 level = rebelCommandSaveInfo.regions[a].GetLevel(index);
		if (level == 0)
			continue;

		// get all sectors with this townid
		std::vector<std::tuple<INT16, INT16, UINT8>> sectors;
		for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
			for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
				if (GetTownIdForSector(x, y) == a)
					sectors.push_back(std::tuple<INT16, INT16, UINT8>(x, y, GetRegionLoyalty(a)));

		// check if soldier is within range of the city
		for (const auto& tuple : sectors)
		{
			const INT16 x = std::get<0>(tuple);
			const INT16 y = std::get<1>(tuple);
			const UINT8 loyalty = std::get<2>(tuple);

			if (abs(x - pSoldier->sSectorX) + abs(y - pSoldier->sSectorY) <= level)
			{
				if (level > foundLevel)
				{
					foundLevel = level;
					foundLoyalty = loyalty;
				}
				else if (level == foundLevel)
				{
					foundLoyalty = max(loyalty, foundLoyalty);
				}
			}
		}
	}

	// loyalty determines success rate
	if (foundLevel > 0 && Random(100) <= foundLoyalty)
		applyPenalties(pSoldier, foundLevel);
}

void ApplyMilitiaBonuses(SOLDIERTYPE* pMilitia)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	pMilitia->stats.bLife += (gRebelCommandSettings.iMilitiaStatBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	pMilitia->stats.bLifeMax += (gRebelCommandSettings.iMilitiaStatBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	pMilitia->stats.bAgility += (gRebelCommandSettings.iMilitiaStatBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	pMilitia->stats.bDexterity += (gRebelCommandSettings.iMilitiaStatBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	pMilitia->stats.bStrength += (gRebelCommandSettings.iMilitiaStatBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);
	pMilitia->stats.bMarksmanship += (gRebelCommandSettings.iMilitiaMarksmanshipBonusPerLevel * rebelCommandSaveInfo.iMilitiaStatsLevel);

	pMilitia->stats.bLife = min(100, pMilitia->stats.bLife);
	pMilitia->stats.bLifeMax = min(100, pMilitia->stats.bLifeMax);
	pMilitia->stats.bAgility = min(100, pMilitia->stats.bAgility);
	pMilitia->stats.bDexterity = min(100, pMilitia->stats.bDexterity);
	pMilitia->stats.bStrength = min(100, pMilitia->stats.bStrength);
	pMilitia->stats.bMarksmanship = min(100, pMilitia->stats.bMarksmanship);
}

UINT8 GetApproximateEnemyLocationResolutionIndex()
{
	return static_cast<UINT8>(rebelCommandSaveInfo.directives[RCD_SPOTTERS].GetValue1());
}

FLOAT GetAssignmentBonus(INT16 x, INT16 y)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0.f;

	const UINT8 townId = GetTownIdForSector(x, y);

	// make sure town has active admins
	if (rebelCommandSaveInfo.regions[townId].adminStatus != RAS_ACTIVE)
		return 0.f;

	const INT16 index = GetAdminActionInRegion(townId, RCAA_MERC_SUPPORT);
	FLOAT value = 0.f;

	if (index >= 0)
	{
		const UINT8 level = rebelCommandSaveInfo.regions[townId].GetLevel(index);
		value += info.adminActions[RCAA_MERC_SUPPORT].fValue1 * level;
	}

	return value/100.f;
}

INT32 GetMiningPolicyBonus(INT16 townId)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0;

	// make sure town has active admins
	if (rebelCommandSaveInfo.regions[townId].adminStatus != RAS_ACTIVE)
		return 0;

	const INT16 index = GetAdminActionInRegion(townId, RCAA_MINING_POLICY);

	if (index >= 0)
	{
		const UINT8 level = rebelCommandSaveInfo.regions[townId].GetLevel(index);
		return static_cast<INT32>(info.adminActions[RCAA_MINING_POLICY].fValue1 * level);
	}

	return 0;
}

void GetBonusMilitia(INT16 sx, INT16 sy, UINT8& green, UINT8& regular, UINT8& elite, BOOLEAN createGroup)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;
	
	const auto createBonusMilitia = [sx, sy, &green, &regular, &elite, createGroup](UINT8 level)
	{
		if (level == 1)
		{
			green = static_cast<UINT8>(info.adminActions[RCAA_SAFEHOUSES].fValue1 + Random(static_cast<UINT32>(1 + info.adminActions[RCAA_SAFEHOUSES].fValue2)));
			regular = Random(2);
			elite = 0;
		}
		else if (level == 2)
		{
			green = 0;
			regular = static_cast<UINT8>(info.adminActions[RCAA_SAFEHOUSES].fValue1 + Random(static_cast<UINT32>(1 + info.adminActions[RCAA_SAFEHOUSES].fValue2)));
			elite = Random(2);
		}

		for (int a = 0; a < max(green, max(regular, elite)); ++a)
		{
			if (a < green)
				CreateNewIndividualMilitia(GREEN_MILITIA, MO_ARULCO, SECTOR(sx, sy));

			if (a < regular)
				CreateNewIndividualMilitia(REGULAR_MILITIA, MO_ARULCO, SECTOR(sx, sy));

			if (a < elite)
				CreateNewIndividualMilitia(ELITE_MILITIA, MO_ARULCO, SECTOR(sx, sy));
		}

		if (createGroup)
		{
			// randomise entry direction
			std::vector<std::pair<INT16, INT16>> vec;
			if (sx > MINIMUM_VALID_X_COORDINATE)
				vec.push_back(std::pair<INT16, INT16>(-1,0));
			if (sx < MAXIMUM_VALID_X_COORDINATE)
				vec.push_back(std::pair<INT16, INT16>(1,0));
			if (sy > MINIMUM_VALID_Y_COORDINATE)
				vec.push_back(std::pair<INT16, INT16>(0,-1));
			if (sy < MAXIMUM_VALID_Y_COORDINATE)
				vec.push_back(std::pair<INT16, INT16>(0,1));
			const UINT8 dir = Random(vec.size());
			const INT16 xmod = static_cast<const INT16>(std::get<0>(vec[dir]));
			const INT16 ymod = static_cast<const INT16>(std::get<1>(vec[dir]));

			const GROUP* group = CreateNewMilitiaGroupDepartingFromSector(SECTOR(sx+xmod, sy+ymod), green, regular, elite);

			PlaceGroupInSector(group->ubGroupID, group->ubSectorX, group->ubSectorY, sx, sy, 0, FALSE); // no need to check for a battle we're jumping into

			gTacticalStatus.uiFlags |= WANT_MILITIA_REINFORCEMENTS;
		}

		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szRebelCommandText[RCT_BONUS_MILITIA_JOINED]);
	};
	
	UINT8 foundLevel = 0;
	UINT8 foundLoyalty = 0;
	for (int a = FIRST_TOWN; a < NUM_TOWNS; ++a)
	{
		// make sure town has active admins
		if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_ACTIVE)
			continue;

		// make sure action exists in this town
		const INT16 index = GetAdminActionInRegion(a, RCAA_SAFEHOUSES);
		if (index == -1)
			continue;

		// and that it's not level 0
		const UINT8 level = rebelCommandSaveInfo.regions[a].GetLevel(index);
		if (level == 0)
			continue;

		// get all sectors with this townid
		std::vector<std::tuple<INT16, INT16, UINT8>> sectors;
		for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
			for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
				if (GetTownIdForSector(x, y) == a)
					sectors.push_back(std::tuple<INT16, INT16, UINT8>(x, y, GetRegionLoyalty(a)));

		// check if sector is within range of the city
		for (const auto& tuple : sectors)
		{
			const INT16 x = std::get<0>(tuple);
			const INT16 y = std::get<1>(tuple);
			const UINT8 loyalty = std::get<2>(tuple);

			// safehouse effect is only for sectors in or immediately adjacent to a town
			if (abs(x - sx) + abs(y - sy) <= 1)
			{
				if (level > foundLevel)
				{
					foundLevel = level;
					foundLoyalty = loyalty;
				}
				else if (level == foundLevel)
				{
					foundLoyalty = max(loyalty, foundLoyalty);
				}
			}
		}
	}

	// loyalty determines success rate
	if (foundLevel > 0 && Random(100) <= min(foundLoyalty, gRebelCommandSettings.iSafehouseReinforceChance))
		createBonusMilitia(foundLevel);
}

INT16 GetFortificationsBonus(UINT8 sector)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0;

	const UINT8 x = SECTORX(sector);
	const UINT8 y = SECTORY(sector);
	const UINT8 townId = GetTownIdForSector(x, y);

	// not a town
	if (townId < FIRST_TOWN || townId >= NUM_TOWNS)
		return 0;

	// no admin team
	if (rebelCommandSaveInfo.regions[townId].adminStatus != RAS_ACTIVE)
		return 0;

	const INT16 index = GetAdminActionInRegion(townId, RCAA_FORTIFICATIONS);

	// action doesn't exist in region
	if (index == -1)
		return 0;

	// no levels in region
	const UINT8 level = rebelCommandSaveInfo.regions[townId].GetLevel(index);
	if (level == 0)
		return 0;

	return static_cast<INT16>(info.adminActions[RCAA_FORTIFICATIONS].fValue1 * level);
}

FLOAT GetHarriersSpeedPenalty(UINT8 sector)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0.f;

	const UINT8 x = SECTORX(sector);
	const UINT8 y = SECTORY(sector);

	// get towns that have harriers
	std::vector<std::tuple<INT16, INT16, INT16>> townSectors;
	for (INT16 x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
	{
		for (INT16 y = MINIMUM_VALID_Y_COORDINATE; y < MAXIMUM_VALID_Y_COORDINATE; ++y)
		{
			const UINT8 townId = GetTownIdForSector(x, y);
			// not a town
			if (townId < FIRST_TOWN || townId >= NUM_TOWNS)
				continue;

			// no admin team
			if (rebelCommandSaveInfo.regions[townId].adminStatus != RAS_ACTIVE)
				continue;

			const INT16 index = GetAdminActionInRegion(townId, RCAA_HARRIERS);

			// action doesn't exist in region
			if (index == -1)
				continue;

			// no levels in region
			const UINT8 level = rebelCommandSaveInfo.regions[townId].GetLevel(index);
			if (level == 0)
				continue;

			townSectors.push_back(std::tuple<INT16, INT16, INT16>(x, y, level));
		}
	}

	// run through townSectors to find the biggest harriers penalty
	BOOLEAN found = FALSE;
	for (const auto& trio : townSectors)
	{
		const INT16 sx = std::get<0>(trio);
		const INT16 sy = std::get<1>(trio);
		const INT16 range = std::get<2>(trio);

		if (abs(sx - x) + abs(sy - y) <= range)
		{
			// level 2 penalty
			if (range == 2)
				return range * info.adminActions[RCAA_HARRIERS].fValue1;

			// keep searching to see if we're in range of a better penalty
			found = TRUE;
		}
	}

	// level 1 penalty
	if (found == TRUE)
		return info.adminActions[RCAA_HARRIERS].fValue1;

	// nothing found, no penalty
	return 0.f;
}

FLOAT GetLoyaltyGainModifier()
{
	if (gGameExternalOptions.fRebelCommandEnabled)
		return gRebelCommandSettings.fLoyaltyGainModifier * (rebelCommandSaveInfo.iActiveDirective == RCD_CREATE_PROPAGANDA ? rebelCommandSaveInfo.directives[RCD_CREATE_PROPAGANDA].GetValue1() : 1.f);

	return 1.f;
}

UINT8 GetMaxTownLoyalty(INT8 townId)
{
	if (gGameExternalOptions.fRebelCommandEnabled)
		return rebelCommandSaveInfo.regions[townId].ubMaxLoyalty;

	return MAX_LOYALTY_VALUE;
}

INT16 GetMilitiaTrainingSpeedBonus()
{
	if (gGameExternalOptions.fRebelCommandEnabled && rebelCommandSaveInfo.iActiveDirective == RCD_TRAIN_MILITIA)
		return static_cast<INT16>(rebelCommandSaveInfo.directives[RCD_TRAIN_MILITIA].GetValue2());

	return 0;
}

FLOAT GetMilitiaTrainingCostModifier()
{
	if (gGameExternalOptions.fRebelCommandEnabled && rebelCommandSaveInfo.iActiveDirective == RCD_TRAIN_MILITIA)
		return rebelCommandSaveInfo.directives[RCD_TRAIN_MILITIA].GetValue1();

	return 1.f;
}

FLOAT GetMilitiaUpkeepCostModifier()
{
	if (gGameExternalOptions.fRebelCommandEnabled && rebelCommandSaveInfo.iActiveDirective == RCD_SUPPORT_MILITIA)
		return rebelCommandSaveInfo.directives[RCD_SUPPORT_MILITIA].GetValue1();

	return 1.f;
}

void HandleScouting()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	// get towns that have scouting
	std::vector<std::tuple<INT16, INT16, INT16>> townSectors;
	for (INT16 x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
	{
		for (INT16 y = MINIMUM_VALID_Y_COORDINATE; y < MAXIMUM_VALID_Y_COORDINATE; ++y)
		{
			const UINT8 townId = GetTownIdForSector(x, y);
			// not a town
			if (townId < FIRST_TOWN || townId >= NUM_TOWNS)
				continue;

			// no admin team
			if (rebelCommandSaveInfo.regions[townId].adminStatus != RAS_ACTIVE)
				continue;

			const INT16 index = GetAdminActionInRegion(townId, RCAA_SCOUTS);

			// action doesn't exist in region
			if (index == -1)
				continue;

			// no levels in region
			const UINT8 level = rebelCommandSaveInfo.regions[townId].GetLevel(index);
			if (level == 0)
				continue;

			// scouting range is level+1, since by default militia can see tiles adjacent to them
			townSectors.push_back(std::tuple<INT16, INT16, INT16>(x, y, level+1));
		}
	}

	// run through all sectors, and see if they're within scouting range of a town
	for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
	{
		for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
		{
			for (const auto& trio : townSectors)
			{
				const INT16 sx = std::get<0>(trio);
				const INT16 sy = std::get<1>(trio);
				const INT16 range = std::get<2>(trio);

				if (abs(x - sx) + abs(y - sy) <= range)
				{
					if (NumNonPlayerTeamMembersInSector(x, y, ENEMY_TEAM) > 0)
					{
						SectorInfo[SECTOR(x, y)].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_HERE;
						SectorInfo[SECTOR(x, y)].uiFlags |= SF_ASSIGN_NOTICED_ENEMIES_KNOW_NUMBER;
					}
					break;
				}
			}
		}
	}
}

FLOAT GetPathfindersSpeedBonus(UINT8 sector)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0.f;

	const UINT8 x = SECTORX(sector);
	const UINT8 y = SECTORY(sector);

	// get towns that have pathfinders
	std::vector<std::tuple<INT16, INT16, INT16>> townSectors;
	for (INT16 x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
	{
		for (INT16 y = MINIMUM_VALID_Y_COORDINATE; y < MAXIMUM_VALID_Y_COORDINATE; ++y)
		{
			const UINT8 townId = GetTownIdForSector(x, y);
			// not a town
			if (townId < FIRST_TOWN || townId >= NUM_TOWNS)
				continue;

			// no admin team
			if (rebelCommandSaveInfo.regions[townId].adminStatus != RAS_ACTIVE)
				continue;

			const INT16 index = GetAdminActionInRegion(townId, RCAA_PATHFINDERS);

			// action doesn't exist in region
			if (index == -1)
				continue;

			// no levels in region
			const UINT8 level = rebelCommandSaveInfo.regions[townId].GetLevel(index);
			if (level == 0)
				continue;

			townSectors.push_back(std::tuple<INT16, INT16, INT16>(x, y, level));
		}
	}

	// run through townSectors to find the biggest pathfinders bonus
	BOOLEAN found = FALSE;
	for (const auto& trio : townSectors)
	{
		const INT16 sx = std::get<0>(trio);
		const INT16 sy = std::get<1>(trio);
		const INT16 range = std::get<2>(trio);

		if (abs(sx - x) + abs(sy - y) <= range)
		{
			// level 2 bonus
			if (range == 2)
				return range * info.adminActions[RCAA_PATHFINDERS].fValue1;

			// keep searching to see if we're in range of a better bonus
			found = TRUE;
		}
	}

	// level 1 bonus
	if (found == TRUE)
		return info.adminActions[RCAA_PATHFINDERS].fValue1;

	// nothing found, no bonus
	return 0.f;
}

BOOLEAN NeutraliseRole(const SOLDIERTYPE* pSoldier)
{
	if (!gGameExternalOptions.fRebelCommandEnabled || !gGameExternalOptions.fEnemyRoles)
		return FALSE;

	if (rebelCommandSaveInfo.iActiveDirective != RCD_HVT_STRIKES)
		return FALSE;

	if (!SOLDIER_CLASS_ENEMY(pSoldier->ubSoldierClass))
		return FALSE;

	const UINT32 chance = static_cast<const UINT32>(rebelCommandSaveInfo.directives[RCD_HVT_STRIKES].GetValue1());

	return Random(100) < chance;
}

void RaidMines(INT32 &playerIncome, INT32 &enemyIncome)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	if (rebelCommandSaveInfo.iActiveDirective != RCD_RAID_MINES)
		return;

	if (Random(100) < static_cast<UINT32>(gRebelCommandSettings.iRaidMinesFailChance))
		return;

	INT32 stolenIncome = static_cast<INT32>(enemyIncome * rebelCommandSaveInfo.directives[RCD_RAID_MINES].GetValue1() * Random(100) / 100.f);
	playerIncome += stolenIncome;
	enemyIncome -= stolenIncome*2;

	if (enemyIncome < 0) enemyIncome = 0;

	if (stolenIncome > 0)
	{
		CHAR16 text[200];
		swprintf(text, szRebelCommandText[RCT_MINE_RAID_SUCCESSFUL], stolenIncome);
		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s", text);
	}
}

BOOLEAN ShowApproximateEnemyLocations()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return FALSE;

	return rebelCommandSaveInfo.iActiveDirective == RCD_SPOTTERS;
}

void ShowWebsiteAvailableMessage()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	StopTimeCompression();
	DoMessageBox(MSG_BOX_MINIEVENT_STYLE, szRebelCommandText[RCT_WEBSITE_AVAILABLE], guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_BIGGER, NULL, NULL);
}

void DailyUpdate()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	// make sure we have a valid directive
	if (rebelCommandSaveInfo.iActiveDirective < RCD_GATHER_SUPPLIES || rebelCommandSaveInfo.iActiveDirective >= RCD_NUM_DIRECTIVES)
	{
		rebelCommandSaveInfo.iActiveDirective = RCD_GATHER_SUPPLIES;
		rebelCommandSaveInfo.iSelectedDirective = RCD_GATHER_SUPPLIES;
	}

	// apply old directive bonus before setting new one
	switch (rebelCommandSaveInfo.iActiveDirective)
	{
	case RCD_GATHER_SUPPLIES:
	case RCD_SUPPORT_MILITIA:
	case RCD_TRAIN_MILITIA:
	case RCD_CREATE_PROPAGANDA:
	case RCD_HVT_STRIKES:
	case RCD_SPOTTERS:
	case RCD_RAID_MINES:
		// effects applied elsewhere
		break;

	case RCD_ELITE_MILITIA:
		{
			const UINT8 elites = static_cast<UINT8>(rebelCommandSaveInfo.directives[RCD_ELITE_MILITIA].GetValue1());
			// get the top-right most omerta tile
			INT16 sx = 1, sy = 1;
			for (sx = MAXIMUM_VALID_X_COORDINATE; sx >= MINIMUM_VALID_X_COORDINATE; --sx)
			{
				for (sy = MINIMUM_VALID_Y_COORDINATE; sy <= MAXIMUM_VALID_Y_COORDINATE; ++sy)
				{
					if (GetTownIdForSector(sx, sy) == OMERTA)
					{
						goto foundOmerta;
					}
				}
			}

			foundOmerta:

			StrategicAddMilitiaToSector(sx, sy, ELITE_MILITIA, elites);
			for (int i = 0 ; i < elites ; ++i)
			{
				CreateNewIndividualMilitia( ELITE_MILITIA, MO_ARULCO, SECTOR(sx, sy) );
			}
		}
		break;

	case RCD_CREATE_TURNCOATS:
		{
			if (LaptopSaveInfo.dIntelPool >= gRebelCommandSettings.fCreateTurncoatsIntelCost)
			{
				UINT32 groupNum = 1;
				GROUP* groupPtr = gpGroupList;
				GROUP* selectedGroupPtr = nullptr;

				// pick a random group
				while (groupPtr != nullptr)
				{
					if (groupPtr->usGroupTeam != ENEMY_TEAM)
					{
						groupPtr = groupPtr->next;
						continue;
					}

					if (Random(groupNum) == 0)
					{
						selectedGroupPtr = groupPtr;
					}

					groupNum++;
					groupPtr = groupPtr->next;
				}

				if (selectedGroupPtr == nullptr)
					break; // couldn't find a group...
				
				// determine which enemies become turncoats
				UINT8 turncoatsToCreate = static_cast<UINT8>(rebelCommandSaveInfo.directives[RCD_CREATE_TURNCOATS].GetValue1());
				UINT8 netEnemyCount = selectedGroupPtr->pEnemyGroup->ubNumAdmins + selectedGroupPtr->pEnemyGroup->ubNumTroops + selectedGroupPtr->pEnemyGroup->ubNumElites;
				netEnemyCount -= selectedGroupPtr->pEnemyGroup->ubNumAdmins_Turncoat - selectedGroupPtr->pEnemyGroup->ubNumTroops_Turncoat - selectedGroupPtr->pEnemyGroup->ubNumElites_Turncoat;
				Assert(netEnemyCount >= 0);
				turncoatsToCreate = min(turncoatsToCreate, netEnemyCount);

				const UINT8 maxAdmins = selectedGroupPtr->pEnemyGroup->ubNumAdmins - selectedGroupPtr->pEnemyGroup->ubNumAdmins_Turncoat;
				const UINT8 maxTroops = selectedGroupPtr->pEnemyGroup->ubNumTroops - selectedGroupPtr->pEnemyGroup->ubNumTroops_Turncoat;
				const UINT8 maxElites = selectedGroupPtr->pEnemyGroup->ubNumElites - selectedGroupPtr->pEnemyGroup->ubNumElites_Turncoat;

				UINT8 admins = 0;
				UINT8 troops = 0;
				UINT8 elites = 0;

				while (turncoatsToCreate > 0)
				{
					std::vector<int> vec;
					// weighted odds
					if (admins < maxAdmins)
					{
						vec.push_back(0);
						vec.push_back(0);
						vec.push_back(0);
					}

					if (troops < maxTroops)
					{
						vec.push_back(1);
						vec.push_back(1);
					}

					if (elites < maxElites)
					{
						vec.push_back(2);
					}

					Assert(vec.size() > 0);

					const int choice = vec[Random(vec.size())];

					switch (choice)
					{
					case 0:
						admins++;
						break;

					case 1:
						troops++;
						break;

					case 2:
						elites++;
						break;
					}

					turncoatsToCreate--;
				}

				// add the turncoats
				selectedGroupPtr->pEnemyGroup->ubNumAdmins_Turncoat += admins;
				selectedGroupPtr->pEnemyGroup->ubNumTroops_Turncoat += troops;
				selectedGroupPtr->pEnemyGroup->ubNumElites_Turncoat += elites;

				LaptopSaveInfo.dIntelPool -= gRebelCommandSettings.fCreateTurncoatsIntelCost;
			}
			else
			{
				ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szRebelCommandText[RCT_INSUFFICIENT_INTEL_TO_CREATE_TURNCOATS]);
			}
		}
		break;

	case RCD_DRAFT:
		{
			AddVolunteers(static_cast<FLOAT>(rebelCommandSaveInfo.directives[RCD_DRAFT].GetValue1() * CurrentPlayerProgressPercentage()));
			for (int a = FIRST_TOWN; a < NUM_TOWNS; ++a)
			{
				DecrementTownLoyalty(a, static_cast<UINT32>(rebelCommandSaveInfo.directives[RCD_DRAFT].GetValue2()));
			}
		}
		break;
	}


	// set new directive
	const INT16 directive = rebelCommandSaveInfo.iSelectedDirective;
	rebelCommandSaveInfo.iActiveDirective = directive;

	// increment supplies
	iIncomingSuppliesPerDay = CalcIncomingSuppliesPerDay(static_cast<RebelCommandDirectives>(directive));
	rebelCommandSaveInfo.iSupplies += iIncomingSuppliesPerDay;

	// get regional bonuses
	INT16 intelGain = 0;
	INT16 supplyGain = 0;
	INT16 moneyGain = 0;
	CHAR16 text[200];
	for (int a = FIRST_TOWN+1; a < NUM_TOWNS; ++a)
	{
		// check to see if the town is lost
		if (IsTownUnderCompleteControlByEnemy(a) && rebelCommandSaveInfo.regions[a].adminStatus == RAS_ACTIVE)
			rebelCommandSaveInfo.regions[a].adminStatus = RAS_INACTIVE;

		// ignore this region if there is no active admin team
		if (rebelCommandSaveInfo.regions[a].adminStatus != RAS_ACTIVE)
			continue;

		UINT32 coolness = 0;
		// clunky... get town coolness. just get the top/left most sector
		for (int x = MINIMUM_VALID_X_COORDINATE ; x <= MAXIMUM_VALID_X_COORDINATE ; ++x)
		{
			for(int y = MINIMUM_VALID_Y_COORDINATE ; y < MAXIMUM_VALID_Y_COORDINATE ; ++y)
			{
				if (GetTownIdForSector(x, y) == a)
				{
					coolness = gCoolnessBySector[SECTOR(x, y)];
					goto foundCoolness;
				}
			}
		}
		foundCoolness:
		coolness = max(coolness, 1);

		const UINT8 loyalty = GetRegionLoyalty(a);
		for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
		{
			const INT8 level = rebelCommandSaveInfo.regions[a].GetLevel(b);
			if (level == 0) continue;

			if (!CanAdminActionBeToggled(rebelCommandSaveInfo.regions[a].actions[b]) || !rebelCommandSaveInfo.regions[a].IsActive(b)) continue;

			// toggle admin action off on a negative supply balance
			if (rebelCommandSaveInfo.iSupplies <= 0)
				rebelCommandSaveInfo.regions[a].SetInactive(b);

			if (rebelCommandSaveInfo.regions[a].IsActive(b))
			{
				switch (static_cast<RebelCommandAdminActions>(rebelCommandSaveInfo.regions[a].actions[b]))
				{
				case RCAA_SUPPLY_LINE:
				case RCAA_SUPPLY_DISRUPTION:
				case RCAA_SCOUTS:
				case RCAA_MERC_SUPPORT:
				case RCAA_PATHFINDERS:
				case RCAA_FORTIFICATIONS:
				case RCAA_HARRIERS:
				case RCAA_MINING_POLICY:
				case RCAA_SAFEHOUSES:
					// no daily bonuses
					break;

				case RCAA_REBEL_RADIO:
					IncrementTownLoyalty(a, static_cast<UINT32>(info.adminActions[RCAA_REBEL_RADIO].fValue1 * level));
					break;

				case RCAA_DEAD_DROPS:
					intelGain += Random(static_cast<UINT32>(info.adminActions[RCAA_DEAD_DROPS].fValue1 * level * loyalty / 100.f));
					break;
					
				case RCAA_SMUGGLERS:
					supplyGain += Random(static_cast<UINT32>(info.adminActions[RCAA_SMUGGLERS].fValue1 * level * loyalty / 100.f));
					break;

				case RCAA_WAREHOUSES:
						AddResources(
							static_cast<FLOAT>(info.adminActions[RCAA_WAREHOUSES].fValue1 * level * Random(100) * loyalty / 10000.f),
							static_cast<FLOAT>(info.adminActions[RCAA_WAREHOUSES].fValue2 * level * Random(100) * loyalty / 10000.f),
							static_cast<FLOAT>(info.adminActions[RCAA_WAREHOUSES].fValue3 * level * Random(100) * loyalty / 10000.f));
					break;

				case RCAA_TAXES:
					moneyGain += static_cast<INT16>(info.adminActions[RCAA_TAXES].fValue1 * coolness * level * loyalty * (75.f + Random(26))/ 10000.f);
					DecrementTownLoyalty(a, static_cast<UINT32>(info.adminActions[RCAA_TAXES].fValue2 * level));
					break;

				case RCAA_ASSIST_CIVILIANS:
					AddVolunteers(static_cast<FLOAT>(info.adminActions[RCAA_ASSIST_CIVILIANS].fValue1 * level * loyalty / 100.f));
					break;


				default:
					AssertMsg(false, "Unknown Admin Action");
					break;
				}
			}
		}
	}

	if (rebelCommandSaveInfo.iSupplies <= 0)
	{
		ScreenMsg(FONT_MCOLOR_RED, MSG_INTERFACE, szRebelCommandText[RCT_INSUFFICIENT_SUPPLIES_ADMIN_ACTIONS_DISABLED]);
	}

	if (intelGain > 0)
	{
		swprintf(text, szRebelCommandText[RCT_DEAD_DROP_INCOME], intelGain);
		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s", text);

		AddIntel(intelGain, FALSE);
	}

	if (supplyGain > 0)
	{
		swprintf(text, szRebelCommandText[RCT_SMUGGLER_INCOME], supplyGain);
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s", text );

		rebelCommandSaveInfo.iSupplies += supplyGain;
	}

	if (moneyGain > 0)
	{
		AddTransactionToPlayersBook(REBEL_COMMAND, 0, GetWorldTotalMin(), moneyGain);
	}

	// update missions
	if (rebelCommandSaveInfo.cachedBountyPayout > 0)
	{
		AddTransactionToPlayersBook(REBEL_COMMAND_BOUNTY_PAYOUT, 0, GetWorldTotalMin(), rebelCommandSaveInfo.cachedBountyPayout);
		rebelCommandSaveInfo.cachedBountyPayout = 0;
	}

	// RCAM_DISRUPT_ASD
	ApplyAdditionalASDEffects();

	if (GetWorldDay() % gRebelCommandSettings.iMissionRefreshTimeDays == 0)
	{
		std::unordered_set<RebelCommandAgentMissions> validMissions;
		for (int i = 0; i < RCAM_NUM_MISSIONS; ++i)
		{
			if (i == RCAM_SOLDIER_BOUNTIES_KINGPIN && !(CheckFact(FACT_KINGPIN_INTRODUCED_SELF, 0) == TRUE && CheckFact(FACT_KINGPIN_DEAD, 0) == FALSE && CheckFact(FACT_KINGPIN_IS_ENEMY, 0) == FALSE && CurrentPlayerProgressPercentage() >= 30)) continue;
			else if (i == RCAM_DISRUPT_ASD && gGameExternalOptions.fASDActive == FALSE) continue;

			validMissions.insert(static_cast<RebelCommandAgentMissions>(i));
		}

		for (const auto& pair : missionMap)
		{
			const RebelCommandAgentMissions mission = pair.first;
			validMissions.erase(mission);
		}

		if (validMissions.size() >= NUM_ARC_AGENT_SLOTS)
		{
			for (int i = 0; i < NUM_ARC_AGENT_SLOTS; ++i)
			{
				const INT8 missionIndex = static_cast<INT8>(Random(validMissions.size()));
				auto iter = validMissions.cbegin();
				for (INT8 j = 0; j < missionIndex; ++j) iter++;
				rebelCommandSaveInfo.availableMissions[i] = *iter;
				validMissions.erase(static_cast<RebelCommandAgentMissions>(rebelCommandSaveInfo.availableMissions[i]));
			}
		}
		else // 1 mission available
		{
			int idx = 0;
			for (auto iter = validMissions.cbegin(); iter != validMissions.cend(); ++iter)
			{
				rebelCommandSaveInfo.availableMissions[idx] = *iter;
				idx++;
			}

			while (idx < NUM_ARC_AGENT_SLOTS)
			{
				rebelCommandSaveInfo.availableMissions[idx] = RCAM_NONE;
				idx++;
			}
		}

		ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szRebelCommandText[RCT_NEW_MISSIONS_AVAILABLE_NOTIFICATION]);
	}
}

void HourlyUpdate()
{
	HandleScouting();

	// RCAM_SEND_SUPPLIES_TO_TOWN
	SendSuppliesToTownMission();

	// it's midnight! do the daily update
	if (GetWorldHour() == 0)
	{
		DailyUpdate();
	}
}

void Init()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	INT8 startingMaxLoyalty;

	switch (gGameOptions.ubDifficultyLevel)
	{
	case DIF_LEVEL_EASY:
		startingMaxLoyalty = gRebelCommandSettings.iMaxLoyaltyNovice;
		break;

	case DIF_LEVEL_HARD:
		startingMaxLoyalty = gRebelCommandSettings.iMaxLoyaltyExpert;
		break;

	case DIF_LEVEL_INSANE:
		startingMaxLoyalty = gRebelCommandSettings.iMaxLoyaltyInsane;
		break;

	case DIF_LEVEL_MEDIUM:
	default:
		startingMaxLoyalty = gRebelCommandSettings.iMaxLoyaltyExperienced;
		break;
	}

	// set base values
	iIncomingSuppliesPerDay = static_cast<INT32>(CurrentPlayerProgressPercentage() * gRebelCommandSettings.fIncomeModifier);
	rebelCommandSaveInfo.iSelectedDirective = RCD_GATHER_SUPPLIES;
	rebelCommandSaveInfo.iActiveDirective = RCD_GATHER_SUPPLIES;
	rebelCommandSaveInfo.iMilitiaStatsLevel = 0;
	// let's be nice and give some supplies to people who enable this feature partway through a campaign so they don't start from zero
	rebelCommandSaveInfo.iSupplies = static_cast<INT32>(GetWorldDay() * CurrentPlayerProgressPercentage() * gRebelCommandSettings.fIncomeModifier / 3);

	// initialise admin actions -- first one is always supply line
	std::vector<RebelCommandAdminActions> actions;
	actions.push_back(RCAA_REBEL_RADIO);
	actions.push_back(RCAA_SAFEHOUSES);
	actions.push_back(RCAA_SUPPLY_DISRUPTION);
	actions.push_back(RCAA_SCOUTS);
	actions.push_back(RCAA_SMUGGLERS);
	actions.push_back(RCAA_TAXES);
	actions.push_back(RCAA_MERC_SUPPORT);
	if (gGameExternalOptions.fMilitiaResources == TRUE)
		actions.push_back(RCAA_WAREHOUSES);
	if (gGameExternalOptions.fIntelResource == TRUE)
		actions.push_back(RCAA_DEAD_DROPS);
	if (gGameExternalOptions.fMilitiaVolunteerPool == TRUE)
		actions.push_back(RCAA_ASSIST_CIVILIANS);
	// RCAA_MINING_POLICY is added below in the region init
	actions.push_back(RCAA_PATHFINDERS);
	actions.push_back(RCAA_HARRIERS);
	actions.push_back(RCAA_FORTIFICATIONS);

	SetupInfo();

	for (int d = 0 ; d < RCD_NUM_DIRECTIVES ; ++d)
	{
		rebelCommandSaveInfo.directives[d].id = static_cast<RebelCommandDirectives>(d);
		rebelCommandSaveInfo.directives[d].iLevel = 0;
	}

	// init regions
	for (int a = FIRST_TOWN; a < NUM_TOWNS; ++a)
	{
		// init max loyalty
		if (a == OMERTA)
		{
			// as rebel hq, omerta gets 100% max loyalty and an admin team
			rebelCommandSaveInfo.regions[OMERTA].adminStatus = RAS_ACTIVE;
			rebelCommandSaveInfo.regions[OMERTA].ubMaxLoyalty = MAX_LOYALTY_VALUE;
		}
		else
		{
			// init admins
			rebelCommandSaveInfo.regions[a].adminStatus = RAS_NONE;
			rebelCommandSaveInfo.regions[a].ubMaxLoyalty = startingMaxLoyalty;
			gTownLoyalty[a].ubRating = min(gTownLoyalty[a].ubRating, startingMaxLoyalty);
		}

		// init admin actions per region. each region has supply line + 5 random actions
		std::vector<RebelCommandAdminActions> vec(actions);

		// check to see if this town has a mine - if it does, add it to this region's admin action pool
		if (GetMineSectorForTown(a) != -1)
			vec.push_back(RCAA_MINING_POLICY);

		// randomise pool
		for (size_t z = 0; z < vec.size(); ++z)
		{
			const int randomIndex = Random(vec.size());
			std::swap(vec[randomIndex], vec[z]);
		}
		vec.resize(5);
		std::sort(vec.begin(), vec.end());
		for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
		{
			if (b == 0)
				rebelCommandSaveInfo.regions[a].actions[0] = RCAA_SUPPLY_LINE; // first action is always supply line
			else
				rebelCommandSaveInfo.regions[a].actions[b] = vec[b - 1];

			rebelCommandSaveInfo.regions[a].actionLevels[b] = 0;
		}
	}

	// init missions
	for (int i = 0; i < NUM_ARC_AGENT_SLOTS; ++i)
	{
		rebelCommandSaveInfo.availableMissions[i] = RCAM_NONE;
	}
}

BOOLEAN Load(HWFILE file)
{
	if (guiCurrentSaveGameVersion >= REBELCOMMAND)
	{
		UINT32 numBytesRead = 0;

		numBytesRead = FileRead(file, &rebelCommandSaveInfo, sizeof(RebelCommand::SaveInfo), &numBytesRead);

		SetupInfo();
	}
	else
	{
		Init();
	}

	// missions update check
	if (rebelCommandSaveInfo.availableMissions[0] == rebelCommandSaveInfo.availableMissions[1] && rebelCommandSaveInfo.availableMissions[0] != RCAM_NONE)
	{
		// init missions
		for (int i = 0; i < NUM_ARC_AGENT_SLOTS; ++i)
		{
			rebelCommandSaveInfo.availableMissions[i] = RCAM_NONE;
		}
	}

	// go through every strategic event to find active agent missions
	std::vector<std::pair<UINT32,UINT32>> missions = GetAllStrategicEventsOfType(EVENT_REBELCOMMAND);
	missionMap.clear();
	for (std::vector<std::pair<UINT32,UINT32>>::iterator it = missions.begin(); it != missions.end(); ++it)
	{
		MissionFirstEvent evt1;
		DeserialiseMissionFirstEvent(it->second, evt1);
		if (evt1.isFirstEvent)
		{
			missionMap.insert(std::make_pair(static_cast<RebelCommandAgentMissions>(evt1.missionId), it->second));
		}

		MissionSecondEvent evt2;
		DeserialiseMissionSecondEvent(it->second, evt2);

		if (evt2.isSecondEvent)
		{
			missionMap.insert(std::make_pair(static_cast<RebelCommandAgentMissions>(evt2.missionId), it->second));
		}
	}
	return TRUE;
}

BOOLEAN Save(HWFILE file)
{
	UINT32 uiNumBytesWritten = 0;

	if (!FileWrite(file, &rebelCommandSaveInfo, sizeof(RebelCommand::SaveInfo), &uiNumBytesWritten))
		return FALSE;
	
	return TRUE;
}

void SetupInfo()
{
	iCurrentRegionId = 1;

	const auto toFloatVec = [](const std::vector<INT32> intVec, std::vector<FLOAT>& floatVec)
	{
		floatVec.clear();
		for (const auto val : intVec)
			floatVec.push_back(static_cast<FLOAT>(val));
	};

	// initialise directives
	Directive d;
	info.directives.clear();

	d.iCostToImprove = gRebelCommandSettings.iGatherSuppliesCosts;
	toFloatVec(gRebelCommandSettings.iGatherSuppliesIncome, d.fValue1);
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_GATHER_SUPPLIES, d);

	d.iCostToImprove = gRebelCommandSettings.iSupportMilitiaCosts;
	d.fValue1 = gRebelCommandSettings.fSupportMilitiaDiscounts;
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_SUPPORT_MILITIA, d);

	d.iCostToImprove = gRebelCommandSettings.iTrainMilitiaCosts;
	d.fValue1 = gRebelCommandSettings.fTrainMilitiaDiscount;
	toFloatVec(gRebelCommandSettings.iTrainMilitiaSpeedBonus, d.fValue2);
	info.directives.insert(info.directives.begin() + RCD_TRAIN_MILITIA, d);

	d.iCostToImprove = gRebelCommandSettings.iCreatePropagandaCosts;
	d.fValue1 = gRebelCommandSettings.fCreatePropagandaModifier;
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_CREATE_PROPAGANDA, d);

	d.iCostToImprove = gRebelCommandSettings.iEliteMilitiaCosts;
	toFloatVec(gRebelCommandSettings.iEliteMilitiaPerDay, d.fValue1);
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_ELITE_MILITIA, d);

	d.iCostToImprove = gRebelCommandSettings.iHvtStrikesCosts;
	toFloatVec(gRebelCommandSettings.iHvtStrikesChance, d.fValue1);
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_HVT_STRIKES, d);

	d.iCostToImprove = gRebelCommandSettings.iSpottersCosts;
	toFloatVec(gRebelCommandSettings.iSpottersModifier, d.fValue1);
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_SPOTTERS, d);

	d.iCostToImprove = gRebelCommandSettings.iRaidMinesCosts;
	d.fValue1 = gRebelCommandSettings.fRaidMinesPercentage;
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_RAID_MINES, d);

	d.iCostToImprove = gRebelCommandSettings.iCreateTurncoatsCosts;
	toFloatVec(gRebelCommandSettings.iCreateTurncoatsPerDay, d.fValue1);
	d.fValue2.clear();
	info.directives.insert(info.directives.begin() + RCD_CREATE_TURNCOATS, d);

	d.iCostToImprove = gRebelCommandSettings.iDraftCosts;
	toFloatVec(gRebelCommandSettings.iDraftPerDayModifier, d.fValue1);
	toFloatVec(gRebelCommandSettings.iDraftLoyaltyLossPerDay, d.fValue2);
	info.directives.insert(info.directives.begin() + RCD_DRAFT, d);

	// init admin actions
	AdminAction aa;
	info.adminActions.clear();

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iSupplyLineMaxLoyaltyIncrease);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_SUPPLY_LINE, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iRebelRadioDailyLoyaltyGain);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_REBEL_RADIO, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iSafehouseMinimumSoldiers);
	aa.fValue2 = static_cast<FLOAT>(gRebelCommandSettings.iSafehouseBonusSoldiers);
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_SAFEHOUSES, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iSupplyDisruptionEnemyStatLoss);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_SUPPLY_DISRUPTION, aa);

	aa.fValue1 = 0.f;
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_SCOUTS, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iDeadDropsDailyIntel);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_DEAD_DROPS, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iSmugglersDailySupplies);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_SMUGGLERS, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iWarehousesDailyMilitiaGuns);
	aa.fValue2 = static_cast<FLOAT>(gRebelCommandSettings.iWarehousesDailyMilitiaArmour);
	aa.fValue3 = static_cast<FLOAT>(gRebelCommandSettings.iWarehousesDailyMilitiaMisc);
	info.adminActions.insert(info.adminActions.begin() + RCAA_WAREHOUSES, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iTaxesDailyIncome);
	aa.fValue2 = static_cast<FLOAT>(gRebelCommandSettings.iTaxesDailyLoyaltyLoss);
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_TAXES, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iAssistCiviliansDailyVolunteers);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_ASSIST_CIVILIANS, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iMercSupportBonus);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_MERC_SUPPORT, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iMiningPolicyBonus);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_MINING_POLICY, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.uPathfindersSpeedBonus);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_PATHFINDERS, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.uHarriersSpeedPenalty);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_HARRIERS, aa);

	aa.fValue1 = static_cast<FLOAT>(gRebelCommandSettings.iFortificationsBonus);
	aa.fValue2 = 0.f;
	aa.fValue3 = 0.f;
	info.adminActions.insert(info.adminActions.begin() + RCAA_FORTIFICATIONS, aa);

	// rftr todo: this should really be a map... but as long as we insert mission info in the same order as the enum then we're good
	MissionHelpers::missionInfo.clear();
	// example format
	// {
	//		{ new skill traits to check },
	//		{ old skill traits to check. use -1 to not match against anything },
	//		{ duration bonus for checked trait },
	//		{ float modifier for checked trait },
	//		{ int modifier for checked trait },
	//		{ value to place in extra bits, used to determine what bonus is applied. }
	// }
	//RCAM_DEEP_DEPLOYMENT
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,														SCOUTING_NT,													STEALTHY_NT,													SURVIVAL_NT},
		{-1,															-1,																STEALTHY_OT,													CAMOUFLAGED_OT},
		{gRebelCommandSettings.iDeepDeploymentDuration_Bonus_Covert,	gRebelCommandSettings.iDeepDeploymentDuration_Bonus_Scouting,	gRebelCommandSettings.iDeepDeploymentDuration_Bonus_Stealthy,	gRebelCommandSettings.iDeepDeploymentDuration_Bonus_Survival},
		{0.f, 0.f, 0.f, 0.f},
		{gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Covert,		gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Scouting,	gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Stealthy,	gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Survival},
		{MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_COVERT,			MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_SCOUTING,			MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_STEALTHY,			MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_SURVIVAL}
	});
	//RCAM_DISRUPT_ASD
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,															TECHNICIAN_NT,															DEMOLITIONS_NT,															NIGHT_OPS_NT},
		{-1,																-1,																		-1,																		NIGHTOPS_OT},
		{gRebelCommandSettings.iDisruptAsdDuration_Bonus_Covert,			gRebelCommandSettings.iDisruptAsdDuration_Bonus_Technician,				gRebelCommandSettings.iDisruptAsdDuration_Bonus_Demolitions,			gRebelCommandSettings.iDisruptAsdDuration_Bonus_Nightops},
		{gRebelCommandSettings.fDisruptAsdIncomeReductionModifier_Covert,	gRebelCommandSettings.fDisruptAsdIncomeReductionModifier_Technician,	gRebelCommandSettings.fDisruptAsdIncomeReductionModifier_Technician,	gRebelCommandSettings.fDisruptAsdIncomeReductionModifier_Nightops},
		{0, 0, 0, 0},
		{0,																	MissionHelpers::DISRUPT_ASD_STEAL_FUEL,									MissionHelpers::DISRUPT_ASD_DESTROY_RESERVES,							0}
	});
	//RCAM_GET_ENEMY_MOVEMENT_TARGETS
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT, RADIO_OPERATOR_NT},
		{-1, -1},
		{gRebelCommandSettings.iGetEnemyMovementTargetsDuration_Bonus_Covert, gRebelCommandSettings.iGetEnemyMovementTargetsDuration_Bonus_Radio},
		{0.f, 0.f},
		{0, 0},
		{0, 0}
	});
	//RCAM_IMPROVE_LOCAL_SHOPS
	MissionHelpers::missionInfo.push_back({ }); // no entries necessary - no modifiers
	//RCAM_REDUCE_STRATEGIC_DECISION_SPEED
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,																	SQUADLEADER_NT,																SNITCH_NT},
		{-1,																		-1,																			-1},
		{gRebelCommandSettings.iReduceStrategicDecisionSpeedDuration_Bonus_Covert,	gRebelCommandSettings.iReduceStrategicDecisionSpeedDuration_Bonus_Deputy,	gRebelCommandSettings.iReduceStrategicDecisionSpeedDuration_Bonus_Snitch},
		{gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier_Covert,		gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier_Deputy,			gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier_Snitch},
		{0, 0, 0},
		{MissionHelpers::REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_COVERT,			MissionHelpers::REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_DEPUTY,			MissionHelpers::REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_SNITCH}
	});
	//RCAM_REDUCE_UNALERTED_ENEMY_VISION
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,																	RADIO_OPERATOR_NT,														STEALTHY_NT},
		{-1,																		-1,																		STEALTHY_OT},
		{gRebelCommandSettings.iReduceUnalertedEnemyVisionDuration_Bonus_Covert,	gRebelCommandSettings.iReduceUnalertedEnemyVisionDuration_Bonus_Radio,	0},
		{gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier_Covert,			gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier_Radio,		gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier_Stealthy},
		{0, 0, 0},
		{MissionHelpers::REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_COVERT,				MissionHelpers::REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_RADIO,			MissionHelpers::REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_STEALTHY}
	});
	//RCAM_SABOTAGE_INFANTRY_EQUIPMENT
	MissionHelpers::missionInfo.push_back({
		{AUTO_WEAPONS_NT,																COVERT_NT,																DEMOLITIONS_NT,																	GUNSLINGER_NT,																RANGER_NT,																SNIPER_NT },
		{AUTO_WEAPS_OT,																	-1,																		-1,																				-1,																			-1,																		PROF_SNIPER_OT },
		{gRebelCommandSettings.iSabotageInfantryEquipmentDuration_Bonus_Auto_Weapons,	gRebelCommandSettings.iSabotageInfantryEquipmentDuration_Bonus_Covert,	gRebelCommandSettings.iSabotageInfantryEquipmentDuration_Bonus_Demolitions,		gRebelCommandSettings.iSabotageInfantryEquipmentDuration_Bonus_Gunslinger,	gRebelCommandSettings.iSabotageInfantryEquipmentDuration_Bonus_Ranger,	gRebelCommandSettings.iSabotageInfantryEquipmentDuration_Bonus_Sniper},
		{0.f, 0.f, 0.f, 0.f, 0.f, 0.f},
		{gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Auto_Weapons,			gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Covert,		gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Demolitions,			gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Gunslinger,		gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Ranger,		gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Sniper},
		{MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_AUTO_WEAPONS,		MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_COVERT,		MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_DEMOLITIONS,			MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_GUNSLINGER,		MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_RANGER,		MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_SNIPER}
	});
	//RCAM_SABOTAGE_MECHANICAL_UNITS
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,																DEMOLITIONS_NT,																HEAVY_WEAPONS_NT},
		{-1,																	-1,																			HEAVY_WEAPS_OT},
		{gRebelCommandSettings.iSabotageMechanicalUnitsDuration_Bonus_Covert,	gRebelCommandSettings.iSabotageMechanicalUnitsDuration_Bonus_Demolitions,	gRebelCommandSettings.iSabotageMechanicalUnitsDuration_Bonus_Heavy_Weapons},
		{0.f, 0.f, 0.f},
		{gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss_Covert,			gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss_Demolitions,			gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss_Heavy_Weapons},
		{MissionHelpers::SABOTAGE_MECHANICAL_UNITS_COVERT,						MissionHelpers::SABOTAGE_MECHANICAL_UNITS_DEMOLITIONS,						MissionHelpers::SABOTAGE_MECHANICAL_UNITS_HEAVY_WEAPONS}
	});
	//RCAM_SEND_SUPPLIES_TO_TOWN
	MissionHelpers::missionInfo.push_back({ }); // no entries necessary - no modifiers
	//RCAM_SOLDIER_BOUNTIES_KINGPIN
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,																SQUADLEADER_NT,																	SNITCH_NT,																DEMOLITIONS_NT},
		{-1,																	-1,																				-1,																		-1},
		{gRebelCommandSettings.iSoldierBountiesKingpinDuration_Bonus_Covert,	0,																				0,																		gRebelCommandSettings.iSoldierBountiesKingpinDuration_Bonus_Demolitions},
		{gRebelCommandSettings.fSoldierBountiesKingpinPayout_Bonus_Covert,		gRebelCommandSettings.fSoldierBountiesKingpinPayout_Bonus_Deputy,				gRebelCommandSettings.fSoldierBountiesKingpinPayout_Bonus_Snitch,		1.f},
		{0,																		0,																				gRebelCommandSettings.iSoldierBountiesKingpinPayout_Limit_Snitch,		gRebelCommandSettings.iSoldierBountiesKingpinPayout_Limit_Demolitions},
		{0,																		MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_OFFICER_PAYOUTS,						0,																		MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_VEHICLE_PAYOUTS}
	});
	//RCAM_TRAIN_MILITIA_ANYWHERE
	MissionHelpers::missionInfo.push_back(
	{
		{COVERT_NT,															SURVIVAL_NT,															TEACHING_NT},
		{-1,																CAMOUFLAGED_OT,															TEACHING_OT},
		{gRebelCommandSettings.iTrainMilitiaAnywhereDuration_Bonus_Covert,	gRebelCommandSettings.iTrainMilitiaAnywhereDuration_Bonus_Survival,		gRebelCommandSettings.iTrainMilitiaAnywhereDuration_Bonus_Teaching},
		{0.f, 0.f, 0.f},
		{gRebelCommandSettings.iTrainMilitiaAnywhereMaxTrainers,			gRebelCommandSettings.iTrainMilitiaAnywhereMaxTrainers,					gRebelCommandSettings.iTrainMilitiaAnywhereMaxTrainers_Teaching},
		{0,																	0,																		MissionHelpers::TRAIN_MILITIA_ANYWHERE_TEACHING}
	});

	// cache item IDs
	ItemIdCache::Clear();
	// (Item[i].usItemClass & IC_AMMO) && (Magazine[ Item[i].ubClassIndex ].ubMagType == AMMO_BOX or AMMO_CRATE?)
	for (UINT16 i = 0; i < MAXITEMS; ++i)
	{
		if (Item[i].gascan) ItemIdCache::gasCans.push_back(i);
		else if (Item[i].firstaidkit) ItemIdCache::firstAidKits.push_back(i);
		else if (Item[i].medicalkit) ItemIdCache::medKits.push_back(i);
		else if (Item[i].toolkit) ItemIdCache::toolKits.push_back(i);
		else if (Item[i].usItemClass & IC_AMMO)
		{
			if (Magazine[Item[i].ubClassIndex].ubMagType == AMMO_BOX)
			{
				if ((gGameOptions.fGunNut || !Item[i].biggunlist)
				&& (gGameOptions.ubGameStyle == STYLE_SCIFI || !Item[i].scifi))
				{
					// coolness runs from 1-10, so apply offset
					ItemIdCache::ammo[Item[i].ubCoolness-1].push_back(i);
				}
			}
		}
		
	}
}

void UpgradeMilitiaStats()
{
	const INT32 cost = gRebelCommandSettings.iMilitiaUpgradeCosts[rebelCommandSaveInfo.iMilitiaStatsLevel];
	if (cost > LaptopSaveInfo.iCurrentBalance)
	{
		DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, szRebelCommandText[RCT_INSUFFICIENT_FUNDS], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return;
	}

	CHAR16 text[200];
	swprintf(text, szRebelCommandText[RCT_MILITIA_UPGRADE_STATS_PROMPT], cost);
	DoLapTopMessageBox(MSG_BOX_LAPTOP_DEFAULT, text, LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, [](UINT8 exitValue) {
		if (exitValue == MSG_BOX_RETURN_YES)
		{
			const INT32 cost = gRebelCommandSettings.iMilitiaUpgradeCosts[rebelCommandSaveInfo.iMilitiaStatsLevel++];

			AddTransactionToPlayersBook(REBEL_COMMAND_SPENDING, 0, GetWorldTotalMin(), -cost);

			RenderWebsite();
		}
		});
}

void ApplySoldierBounty(const SOLDIERTYPE* pSoldier)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	if (pSoldier->bTeam != ENEMY_TEAM)
		return;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_SOLDIER_BOUNTIES_KINGPIN);

	if (iter == missionMap.end())
		return;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (!evt.isSecondEvent)
		return;

	UINT16 payout = 0;

	if (TANK(pSoldier) && evt.extraBits == MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_VEHICLE_PAYOUTS)
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Tank;
	else if (COMBAT_JEEP(pSoldier) && evt.extraBits == MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_VEHICLE_PAYOUTS)
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Jeep;
	else if (ENEMYROBOT(pSoldier) && evt.extraBits == MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_VEHICLE_PAYOUTS)
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Robot;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR)
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Admin;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY)
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Troop;
	else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE)
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Elite;
	else // unknown kill, bail out!
		return;

	// payout per role
	if (pSoldier->usSoldierFlagMask & SOLDIER_ENEMY_OFFICER && (evt.extraBits == MissionHelpers::SOLDIER_BOUNTIES_KINGPIN_OFFICER_PAYOUTS))
		payout += gRebelCommandSettings.iSoldierBountiesKingpinPayout_Officer;


	// apply payout limit
	UINT32 durationBonus = 0;
	int durationBonusSkill = 0;
	INT16 intModifier = 0;
	int intModifierSkill = 0;
	FLOAT floatModifier = 0.f;
	int floatModifierSkill = 0;
	UINT16 extraBits = 0;
	MissionHelpers::GetMissionInfo(RCAM_SOLDIER_BOUNTIES_KINGPIN, &gMercProfiles[evt.mercProfileId], durationBonus, floatModifier, intModifier, durationBonusSkill, floatModifierSkill, intModifierSkill, extraBits);

	const INT32 payoutLimit = max(gRebelCommandSettings.iSoldierBountiesKingpinPayout_Limit, intModifier);
	// clamp payout like this in case the player maxes out payouts in config and we have to deal with a uint overflow
	if (payoutLimit - payout < rebelCommandSaveInfo.cachedBountyPayout)
		payout = payoutLimit - rebelCommandSaveInfo.cachedBountyPayout;

	rebelCommandSaveInfo.cachedBountyPayout += payout;
}

void ApplyEnemyMechanicalUnitPenalties(SOLDIERTYPE* pSoldier)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_SABOTAGE_MECHANICAL_UNITS);

	if (iter == missionMap.end())
		return;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (evt.isSecondEvent)
	{
		INT8 statLoss = 0;
		switch (evt.extraBits)
		{
		case MissionHelpers::SABOTAGE_MECHANICAL_UNITS_COVERT:			statLoss = gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss_Covert; break;
		case MissionHelpers::SABOTAGE_MECHANICAL_UNITS_DEMOLITIONS:		statLoss = gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss_Demolitions; break;
		case MissionHelpers::SABOTAGE_MECHANICAL_UNITS_HEAVY_WEAPONS:	statLoss = gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss_Heavy_Weapons; break;
		default: statLoss = gRebelCommandSettings.iSabotageMechanicalUnitsStatLoss; break;
		}

		pSoldier->stats.bLife -= statLoss;
		pSoldier->stats.bLifeMax = pSoldier->stats.bLife;
		pSoldier->stats.bAgility -= statLoss;
		pSoldier->stats.bDexterity -= statLoss;
		pSoldier->stats.bStrength -= statLoss;
		pSoldier->stats.bMarksmanship -= statLoss;

		pSoldier->stats.bLife = max(33, pSoldier->stats.bLife);
		pSoldier->stats.bLifeMax = max(33, pSoldier->stats.bLifeMax);
		pSoldier->stats.bAgility = max(33, pSoldier->stats.bAgility);
		pSoldier->stats.bDexterity = max(33, pSoldier->stats.bDexterity);
		pSoldier->stats.bStrength = max(33, pSoldier->stats.bStrength);
		pSoldier->stats.bMarksmanship = max(33, pSoldier->stats.bMarksmanship);
	}
}

void ApplyMilitiaTraits(SOLDIERTYPE* pSoldier)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	// rftr todo: check bitmask for specific possible traits
}

void ApplyVisionModifier(const SOLDIERTYPE* pSoldier, INT32& sight)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_REDUCE_UNALERTED_ENEMY_VISION);

	if (iter == missionMap.end())
		return;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (evt.isSecondEvent)
	{
		FLOAT modifier = 0.f;
		switch (evt.extraBits)
		{
		case MissionHelpers::REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_COVERT:		modifier = gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier_Covert; break;
		case MissionHelpers::REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_RADIO:		modifier = gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier_Radio; break;
		case MissionHelpers::REDUCE_UNALERTED_ENEMY_VISION_MODIFIER_STEALTHY:	modifier = gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier_Stealthy; break;
		default: modifier = gRebelCommandSettings.fReduceUnlaertedEnemyVisionModifier; break;
		}

		if (pSoldier->bTeam == ENEMY_TEAM && pSoldier->aiData.bAlertStatus == STATUS_GREEN)
		{
			sight = static_cast<INT32>(sight * (1.f - modifier));
		}
	}
}

BOOLEAN CanAssignTraitsToMilitia()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return FALSE;

	// rftr todo: check bitmask
	
	return TRUE;
}

BOOLEAN CanTrainMilitiaAnywhere()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return FALSE;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_TRAIN_MILITIA_ANYWHERE);

	if (iter == missionMap.end())
		return FALSE;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	return evt.isSecondEvent;
}

UINT8 GetMaxTrainersForTrainMilitiaAnywhere()
{
	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_TRAIN_MILITIA_ANYWHERE);

	if (iter == missionMap.end())
		return FALSE;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (!evt.isSecondEvent)
		return 0;

	switch (evt.extraBits)
	{
	case MissionHelpers::TRAIN_MILITIA_ANYWHERE_TEACHING:	return gRebelCommandSettings.iTrainMilitiaAnywhereMaxTrainers_Teaching;
	default:												return gRebelCommandSettings.iTrainMilitiaAnywhereMaxTrainers;
	}
}

INT16 GetAdditionalDeployRange(const UINT8 insertionCode)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_DEEP_DEPLOYMENT);

	if (iter == missionMap.end())
		return 0;

	INT16 range = 0;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	// we only get a range bonus if the mission is active!
	if (evt.isSecondEvent)
	{
		switch (insertionCode)
		{
		case INSERTION_CODE_NORTH:
		case INSERTION_CODE_SOUTH:
		{
			range = gRebelCommandSettings.iDeepDeploymentRangeNS;

			switch (evt.extraBits)
			{
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_COVERT:	range += gRebelCommandSettings.iDeepDeploymentRangeNS_Bonus_Covert; break;
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_SCOUTING:	range += gRebelCommandSettings.iDeepDeploymentRangeNS_Bonus_Scouting; break;
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_STEALTHY:	range += gRebelCommandSettings.iDeepDeploymentRangeNS_Bonus_Stealthy; break;
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_SURVIVAL:	range += gRebelCommandSettings.iDeepDeploymentRangeNS_Bonus_Survival; break;

			default: break;
			}

			return range;
		}

		case INSERTION_CODE_WEST:
		case INSERTION_CODE_EAST:
		{
			range = gRebelCommandSettings.iDeepDeploymentRangeEW;

			switch (evt.extraBits)
			{
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_COVERT:	range += gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Covert; break;
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_SCOUTING:	range += gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Scouting; break;
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_STEALTHY:	range += gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Stealthy; break;
			case MissionHelpers::DEEP_DEPLOYMENT_RANGE_BONUS_SURVIVAL:	range += gRebelCommandSettings.iDeepDeploymentRangeEW_Bonus_Survival; break;

			default: break;
			}

			return range;
		}
		}
	}

	return 0;
}

FLOAT GetASDIncomeModifier()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 1.f;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_DISRUPT_ASD);

	if (iter == missionMap.end())
		return 1.f;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (evt.isSecondEvent)
	{
		const MERCPROFILESTRUCT* merc = &gMercProfiles[evt.mercProfileId];
		UINT32 durationBonus;
		FLOAT floatModifier;
		INT16 intModifier;
		int durSkill;
		int floatSkill;
		int intSkill;
		UINT16 extraBits;
		MissionHelpers::GetMissionInfo(RCAM_DISRUPT_ASD, merc, durationBonus, floatModifier, intModifier, durSkill, floatSkill, intSkill, extraBits);

		return 1.f - floatModifier;
	}

	return 1.f;
}

void ApplyAdditionalASDEffects()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_DISRUPT_ASD);

	if (iter == missionMap.end())
		return;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (evt.isSecondEvent)
	{
		switch (evt.extraBits)
		{
		case MissionHelpers::DISRUPT_ASD_STEAL_FUEL:
		{
			// spawn a gas can
			CreateItemAtAirport(ItemIdCache::gasCans.at(ItemIdCache::gasCans.size()), 75 + Random(26));

			// say it came from the ASD's reserves
			AddStrategicAIResources(ASD_FUEL, gGameExternalOptions.gASDResource_Fuel_Jeep + Random(gGameExternalOptions.gASDResource_Fuel_Jeep));
		}
		break;

		case MissionHelpers::DISRUPT_ASD_DESTROY_RESERVES:
		{
			// priority: tank > jeep > robots
			if (GetStrategicAIResourceCount(ASD_TANK) > 0)
				AddStrategicAIResources(ASD_TANK, 1 + Random(2));
			else if (GetStrategicAIResourceCount(ASD_JEEP) > 0)
				AddStrategicAIResources(ASD_JEEP, 1 + Random(2));
			else if (GetStrategicAIResourceCount(ASD_ROBOT) > 0)
				AddStrategicAIResources(ASD_ROBOT, 2 + Random(3));

			// let's also try to destroy a good amount of fuel
			AddStrategicAIResources(ASD_FUEL, gGameExternalOptions.gASDResource_Fuel_Tank + Random(gGameExternalOptions.gASDResource_Fuel_Tank));
		}
		break;
		}
	}
}

INT8 GetEnemyEquipmentCoolnessModifier()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_SABOTAGE_INFANTRY_EQUIPMENT);

	if (iter == missionMap.end())
		return 0;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	return evt.isSecondEvent ? -1 : 0;
}

INT8 GetEnemyEquipmentStatusModifier(const INT8 initialStatus)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return initialStatus;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_SABOTAGE_INFANTRY_EQUIPMENT);

	if (iter == missionMap.end())
		return initialStatus;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (evt.isSecondEvent)
	{
		INT8 modifier = 0;
		switch (evt.extraBits)
		{
		case MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_AUTO_WEAPONS:	modifier = gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Auto_Weapons; break;
		case MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_COVERT:			modifier = gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Covert; break;
		case MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_DEMOLITIONS:	modifier = gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Demolitions; break;
		case MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_GUNSLINGER:		modifier = gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Gunslinger; break;
		case MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_RANGER:			modifier = gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Ranger; break;
		case MissionHelpers::SABOTAGE_ENEMY_INFANTRY_EQUIPMENT_MODIFIER_SNIPER:			modifier = gRebelCommandSettings.iSabotageInfantryEquipmentModifier_Sniper; break;
		}

		return max(1, min(initialStatus - modifier, 100));
	}

	return initialStatus;
}

UINT8 GetMerchantCoolnessBonus()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 0;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_IMPROVE_LOCAL_SHOPS);

	if (iter == missionMap.end())
		return 0;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	return evt.isSecondEvent ? 1 : 0;
}

FLOAT GetStrategicDecisionSpeedModifier()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return 1.f;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_REDUCE_STRATEGIC_DECISION_SPEED);

	if (iter == missionMap.end())
		return 1.f;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	FLOAT modifier = 1.f;

	if (evt.isSecondEvent)
	{
		switch (evt.extraBits)
		{
		case MissionHelpers::REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_COVERT:	modifier = gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier_Covert; break;
		case MissionHelpers::REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_DEPUTY:	modifier = gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier_Deputy; break;
		case MissionHelpers::REDUCE_STRATEGIC_DECISION_SPEED_MODIFIER_SNITCH:	modifier = gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier_Snitch; break;
		default:																modifier = gRebelCommandSettings.fReduceStrategicDecisionSpeedModifier; break;
		}
	}

	return modifier;
}

void HandleStrategicEvent(const UINT32 eventParam)
{
	// this handles the transition from "mission prep" (first event) to "mission active" (second event), which happens 24 hours after the player clicks on "start mission"
	MissionFirstEvent evt1;
	MissionSecondEvent evt2;
	DeserialiseMissionFirstEvent(eventParam, evt1);
	DeserialiseMissionSecondEvent(eventParam, evt2);
	CHAR16 msgBoxText[200];
	CHAR16 screenMsgText[200];

	if (evt1.isFirstEvent)
	{
		// mission prep is over. see if we can activate the mission
		missionMap.erase(static_cast<RebelCommandAgentMissions>(evt1.missionId));

		// make sure the merc's still on our team
		BOOLEAN foundMerc = FALSE;
		for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
		{
			const SOLDIERTYPE* pSoldier = MercPtrs[i];

			if (pSoldier->ubProfile == evt1.mercProfileId && pSoldier->bActive)
			{
				foundMerc = TRUE;
				break;
			}
		}

		if (evt1.isMissionSuccess)
		{
			const RebelCommandAgentMissions mission = static_cast<RebelCommandAgentMissions>(evt1.missionId);
			const MERCPROFILESTRUCT merc = gMercProfiles[evt1.mercProfileId];

			if (!foundMerc)
				goto MissionFailed_MercNoLongerOnTeam;

			// what mission did we do? apply bonuses here, and don't forget to check them later when checking to see if a mission bonus should be applied
			UINT32 durationBonus = 0;
			int durationBonusSkill = 0;
			INT16 intModifier = 0;
			int intModifierSkill = 0;
			FLOAT floatModifier = 0.f;
			int floatModifierSkill = 0;
			UINT16 extraBits = 0;
			MissionHelpers::GetMissionInfo(mission, &merc, durationBonus, floatModifier, intModifier, durationBonusSkill, floatModifierSkill, intModifierSkill, extraBits);

			BOOLEAN validMission = FALSE;
			switch (mission)
			{
			case RCAM_DEEP_DEPLOYMENT:
			case RCAM_DISRUPT_ASD:
			case RCAM_GET_ENEMY_MOVEMENT_TARGETS:
			case RCAM_IMPROVE_LOCAL_SHOPS:
			case RCAM_REDUCE_STRATEGIC_DECISION_SPEED:
			case RCAM_REDUCE_UNALERTED_ENEMY_VISION:
			case RCAM_SABOTAGE_INFANTRY_EQUIPMENT:
			case RCAM_SABOTAGE_MECHANICAL_UNITS:
			case RCAM_SEND_SUPPLIES_TO_TOWN:
			case RCAM_SOLDIER_BOUNTIES_KINGPIN:
			case RCAM_TRAIN_MILITIA_ANYWHERE:
			{
				validMission = TRUE;
			}
			break;

			default:
			{
				ScreenMsg(FONT_MCOLOR_RED, MSG_INTERFACE, L"Unrecognised mission ID: %d", mission);
			}
			break;
			}

			if (validMission)
			{
				const UINT32 activatedMissionParam = SerialiseMissionSecondEvent(evt1.sentGenericRebelAgent, evt1.mercProfileId, mission, extraBits);
				AddStrategicEvent(EVENT_REBELCOMMAND, GetWorldTotalMin() + 60 * evt1.missionDurationInHours, activatedMissionParam);

				if (!evt1.sentGenericRebelAgent)
				{
					for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
					{
						SOLDIERTYPE* pSoldier = MercPtrs[i];
						if (pSoldier->ubProfile == evt1.mercProfileId)
						{
							// mission successful! give some experience pts
							StatChange(pSoldier, LDRAMT, 20, FROM_SUCCESS);
							StatChange(pSoldier, WISDOMAMT, 15, FROM_SUCCESS);
							break;
						}
					}
				}

				missionMap.insert(std::make_pair(mission, activatedMissionParam));
				swprintf(msgBoxText, szRebelCommandText[RCT_MISSION_SUCCESS], szRebelCommandAgentMissionsText[evt1.missionId * 2]);
				swprintf(screenMsgText, szRebelCommandText[RCT_MISSION_SUCCESS], szRebelCommandAgentMissionsText[evt1.missionId * 2]);
				ScreenMsg(FONT_MCOLOR_LTGREEN, MSG_INTERFACE, screenMsgText);
			}
		}
		else
		{
			if (!evt1.sentGenericRebelAgent && foundMerc)
			{
				for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
				{
					SOLDIERTYPE* pSoldier = MercPtrs[i];
					if (pSoldier->ubProfile == evt1.mercProfileId)
					{
						// mission failed! we tried, give some pity exp
						StatChange(pSoldier, LDRAMT, 20, FROM_FAILURE);
						StatChange(pSoldier, WISDOMAMT, 15, FROM_FAILURE);
						break;
					}
				}
			}

			MissionFailed_MercNoLongerOnTeam:
			swprintf(msgBoxText, szRebelCommandText[RCT_MISSION_FAILURE], szRebelCommandAgentMissionsText[evt1.missionId * 2]);
			swprintf(screenMsgText, szRebelCommandText[RCT_MISSION_FAILURE], szRebelCommandAgentMissionsText[evt1.missionId * 2]);
			ScreenMsg(FONT_MCOLOR_RED, MSG_INTERFACE, screenMsgText);
		}

		if (!evt1.sentGenericRebelAgent && foundMerc)
		{
			for (UINT8 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
			{
				SOLDIERTYPE* pSoldier = MercPtrs[i];
				if (pSoldier->ubProfile == evt1.mercProfileId)
				{
					// merc ready for reassignment
					pSoldier->bSectorZ -= REBEL_COMMAND_Z_OFFSET;
					pSoldier->ubInsertionDirection = DIRECTION_IRRELEVANT;
					pSoldier->ubStrategicInsertionCode = INSERTION_CODE_CENTER;
					AssignmentDone(pSoldier, TRUE, FALSE);
					AddCharacterToAnySquad(pSoldier);
					break;
				}
			}

		}
	}
	else if (evt2.isSecondEvent)
	{
		// mission duration is over. deactivate the mission
		missionMap.erase(static_cast<RebelCommandAgentMissions>(evt2.missionId));
		swprintf(msgBoxText, szRebelCommandText[RCT_MISSION_EXPIRED], szRebelCommandAgentMissionsText[evt2.missionId * 2]);
		swprintf(screenMsgText, szRebelCommandText[RCT_MISSION_EXPIRED], szRebelCommandAgentMissionsText[evt1.missionId * 2]);
		ScreenMsg(FONT_MCOLOR_RED, MSG_INTERFACE, screenMsgText);
	}

	DoMessageBox(MSG_BOX_BASIC_STYLE, msgBoxText, guiCurrentScreen, MSG_BOX_FLAG_OK, NULL, NULL);
}

void SendSuppliesToTownMission()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_SEND_SUPPLIES_TO_TOWN);

	if (iter == missionMap.end())
		return;

	if (GetWorldHour() % gRebelCommandSettings.iSendSuppliesToTownInterval != 0)
		return;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	if (evt.isSecondEvent)
	{
		IncrementTownLoyalty(evt.extraBits, gRebelCommandSettings.iSendSuppliesToTownLoyaltyGain);
	}
}

INT16 SendSuppliesToTownDurationBonus(const MERCPROFILESTRUCT* merc)
{
	return merc ? merc->bExpLevel * 8 : 0;
}

BOOLEAN ShowEnemyMovementTargets()
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return FALSE;

	const std::unordered_map<RebelCommandAgentMissions, UINT32>::iterator iter = missionMap.find(RCAM_GET_ENEMY_MOVEMENT_TARGETS);

	if (iter == missionMap.end())
		return FALSE;

	MissionSecondEvent evt;
	DeserialiseMissionSecondEvent(iter->second, evt);

	return evt.isSecondEvent;
}

void DEBUG_DAY()
{
	DailyUpdate();
}

}

template<> void DropDownTemplate<DROPDOWN_REBEL_COMMAND_DIRECTIVE>::SetRefresh()
{
	using namespace RebelCommand;
	const INT16 newDirective = REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey();
	rebelCommandSaveInfo.iSelectedDirective = newDirective;
	iIncomingSuppliesPerDay = CalcIncomingSuppliesPerDay(static_cast<RebelCommandDirectives>(newDirective));

	redraw = TRUE;
}


