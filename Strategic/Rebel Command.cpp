//#pragma optimize("", off)
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
#include "GameSettings.h"
#include "GameVersion.h"
#include "input.h"
#include "Line.h"
#include "insurance.h"
#include "laptop.h"
#include "message.h"
#include "MessageBoxScreen.h"
#include "MilitiaIndividual.h"
#include "mousesystem.h"
#include "Queen Command.h"
#include "random.h"
#include "SaveLoadGame.h"
#include "strategicmap.h"
#include "Strategic Mines.h"
#include "Strategic Movement.h"
#include "Strategic Town Loyalty.h"
#include "Text.h"
#include "Town Militia.h"
#include "Utilities.h"
#include "WCheck.h"
#include "WordWrap.h"
#endif

#define		DIRECTIVE_TEXT(id)		RCDT_##id##, RCDT_##id##_EFFECT, RCDT_##id##_DESC, RCDT_##id##_IMPROVE,

#define		ADMIN_ACTION_CHANGE_COST	15000

#define		REBEL_COMMAND_DROPDOWN		DropDownTemplate<DROPDOWN_REBEL_COMMAND_DIRECTIVE>::getInstance()

#define		WEBSITE_LEFT	LAPTOP_SCREEN_UL_X
#define		WEBSITE_TOP		LAPTOP_SCREEN_WEB_UL_Y + 3
#define		WEBSITE_WIDTH	500
#define		WEBSITE_HEIGHT	395

extern UINT32 gCoolnessBySector[256];
extern UINT32 guiInsuranceBackGround;
extern BOOLEAN gfTownUsesLoyalty[MAX_TOWNS];
extern GROUP *gpGroupList;

namespace RebelCommand
{
void DEBUG_DAY();
void DEBUG_PRINT();

enum WebsiteState
{
	RCS_NATIONAL_OVERVIEW,
	RCS_REGIONAL_OVERVIEW,
};

enum RebelCommandText // keep this synced with szRebelCommandText in the text files
{
	RCT_NATIONAL_OVERVIEW = 0,
	RCT_REGIONAL_OVERVIEW,
	RCT_SWITCH_TO_REGIONAL,
	RCT_SWITCH_TO_NATIONAL,
	RCT_SUPPLIES,
	RCT_INCOMING_SUPPLIES,
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
};

enum RebelCommandHelpText // keep this synced with szRebelCommandHelpText in the text files
{
	RCHT_SUPPLIES = 0,
	RCHT_SUPPLIES_INCOME,
	RCHT_DIRECTIVES,
	RCHT_ADMIN_TEAM,
	RCHT_MAX_LOYALTY,
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

enum ChangeAdminActionState
{
	CAAS_INIT,
	CAAS_CHANGING,
};

// website functions
template<typename voidFunc>
void ButtonHelper(GUI_BUTTON* btn, INT32 reason, voidFunc onClick);
void ClearAllButtons();
void ClearAllHelpTextRegions();
void DeployOrReactivateAdminTeam(INT16 regionId);
void GetDirectiveEffect(const RebelCommandDirectives directive, STR16 text);
INT32 GetDirectiveImprovementCost(const RebelCommandDirectives directive);
void ImproveDirective(const RebelCommandDirectives directiveId);
void PurchaseAdminAction(INT32 regionId, INT32 actionIndex);
void RegionNavNext();
void RegionNavPrev();
void RenderHeader(RebelCommandText titleText);
void RenderNationalOverview();
void RenderRegionalOverview();
void SetDirectiveDescriptionHelpText(INT32 reason, MOUSE_REGION& region, RebelCommandDirectives text);
void SetRegionHelpText(INT32 reason, MOUSE_REGION& helpTextRegion, RebelCommandHelpText text);
void SetupAdminActionBox(const UINT8 actionIndex, const UINT16 descriptionText, const UINT16 buttonText);
void ToggleWebsiteView();
void UpdateAdminActionChangeList(INT16 regionId);

INT32 GetAdminActionCostForRegion(INT16 regionId);
INT16 GetAdminActionInRegion(INT16 regionId, RebelCommandAdminActions adminAction);
void HandleScouting();
void SetupInfo();
void UpgradeMilitiaStats();

// buttons
INT32 dbgAdvanceDayBtnId = -1;
INT32 dbgPrintBtnId = -1;
std::vector<INT32> adminActionBtnIds;
std::vector<INT32> adminActionChangeBtnIds;
ChangeAdminActionState adminActionChangeState;
INT32 adminTeamBtnId = -1;
INT32 improveDirectiveBtnId = -1;
INT32 regionNextBtnId = -1;
INT32 regionPrevBtnId = -1;
INT32 upgradeMilitiaStatsBtnId = -1;
INT32 viewSwapBtnId = -1;

// help text regions
MOUSE_REGION adminTeamHelpTextRegion;
MOUSE_REGION directiveDescriptionHelpTextRegion;
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

void ClearAllButtons()
{
	for (const auto btnId : adminActionBtnIds)
	{
		RemoveButton(btnId);
	}
	adminActionBtnIds.clear();

	for (const auto btnId : adminActionChangeBtnIds)
	{
		RemoveButton(btnId);
	}
	adminActionChangeBtnIds.clear();

	if (adminTeamBtnId != -1)
	{
		RemoveButton(adminTeamBtnId);
		adminTeamBtnId = -1;
	}

	if (improveDirectiveBtnId != -1)
	{
		RemoveButton(improveDirectiveBtnId);
		improveDirectiveBtnId = -1;
	}

	if (regionNextBtnId != -1)
	{
		RemoveButton(regionNextBtnId);
		regionNextBtnId = -1;
	}

	if (regionPrevBtnId != -1)
	{
		RemoveButton(regionPrevBtnId);
		regionPrevBtnId = -1;
	}

	if (viewSwapBtnId != -1)
	{
		RemoveButton(viewSwapBtnId);
		viewSwapBtnId = -1;
	}

	if (upgradeMilitiaStatsBtnId != -1)
	{
		RemoveButton(upgradeMilitiaStatsBtnId);
		upgradeMilitiaStatsBtnId = -1;
	}

	if (dbgAdvanceDayBtnId != -1)
	{
		RemoveButton(dbgAdvanceDayBtnId);
		dbgAdvanceDayBtnId = -1;
	}

	if (dbgPrintBtnId != -1)
	{
		RemoveButton(dbgPrintBtnId);
		dbgPrintBtnId = -1;
	}
}

void ClearAllHelpTextRegions()
{
	MSYS_RemoveRegion(&adminTeamHelpTextRegion);
	MSYS_RemoveRegion(&directiveDescriptionHelpTextRegion);
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

INT32 GetAdminActionCostForRegion(INT16 regionId)
{
	INT16 totalLocalActions = 0;
	INT16 totalNationalActions = 0;

	// ignore omerta when calculating action costs
	for (int a = OMERTA+1; a < NUM_TOWNS; ++a)
	{
		for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
		{
			totalNationalActions += rebelCommandSaveInfo.regions[a].actionLevels[b];

			if (a == regionId)
				totalLocalActions += rebelCommandSaveInfo.regions[a].actionLevels[b];
		}
	}

	return totalNationalActions * gRebelCommandSettings.iAdminActionCostIncreaseNational + totalLocalActions * gRebelCommandSettings.iAdminActionCostIncreaseRegional;
}

INT16 GetAdminActionInRegion(INT16 regionId, RebelCommandAdminActions adminAction)
{
	if (regionId >= FIRST_TOWN && regionId < NUM_TOWNS)
	{
		for (int idx = 0; idx < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++idx)
		{
			if (rebelCommandSaveInfo.regions[regionId].actions[idx] == adminAction)
			{
				return idx;
			}
		}
	}

	return -1;
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

			LaptopSaveInfo.iCurrentBalance -= cost;

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

	switch (actionIndex % 3)
	{
	case 0: x = WEBSITE_LEFT + 10; break;
	case 1: x = WEBSITE_LEFT + 180; break;
	case 2: x = WEBSITE_LEFT + 350; break;
	}

	y = WEBSITE_TOP + 125 + 110 * (actionIndex / 3);

	if (actionIndex < 5 || adminActionChangeState == CAAS_INIT)
	{
		// show label if maxed out
		if ((actionIndex == RCAA_SUPPLY_LINE && rebelCommandSaveInfo.regions[iCurrentRegionId].ubMaxLoyalty >= MAX_LOYALTY_VALUE)
			|| (actionIndex != RCAA_SUPPLY_LINE && rebelCommandSaveInfo.regions[iCurrentRegionId].actionLevels[actionIndex] >= 2))
		{
			DrawTextToScreen(szRebelCommandAdminActionsText[buttonText], x, y + 7, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
		}
		else // show button
		{
			const UINT8 level = rebelCommandSaveInfo.regions[iCurrentRegionId].actionLevels[actionIndex];
			swprintf(text, szRebelCommandText[level == 0 ? RCT_ADMIN_ACTION_ESTABLISH : RCT_ADMIN_ACTION_IMPROVE], szRebelCommandAdminActionsText[buttonText]);
			const INT32 btnId = CreateTextButton(text, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 140, 20, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
				{
					ButtonHelper(btn, reason, [btn]() { PurchaseAdminAction(btn->UserData[0], btn->UserData[1]); });
				});

			Assert(ButtonList[btnId]);
			ButtonList[btnId]->UserData[0] = iCurrentRegionId;
			ButtonList[btnId]->UserData[1] = actionIndex;

			adminActionBtnIds.push_back(btnId);
		}

		y += 22;
		swprintf(text, szRebelCommandText[RCT_ADMIN_ACTION_TIER], rebelCommandSaveInfo.regions[iCurrentRegionId].actionLevels[actionIndex]);
		DrawTextToScreen(text, x, y, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		y += 13;
		DisplayWrappedString(x, y, 140, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, szRebelCommandAdminActionsText[descriptionText], FONT_MCOLOR_BLACK, FALSE, 0);

		// special case for index 5: show state change button
		if (actionIndex == 5)
		{
			y += 68;

			const INT32 btnId = CreateTextButton(szRebelCommandText[RCT_CHANGE_ADMIN_ACTION], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 140, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
				{
					ButtonHelper(btn, reason, [btn]() { adminActionChangeState = CAAS_CHANGING; });
				});
			adminActionChangeBtnIds.push_back(btnId);
		}
	}
	else if (actionIndex == 5 && adminActionChangeState == CAAS_CHANGING)
	{
		// let player change last action
		RebelCommandAdminActions newAction = adminActionChangeList[adminActionChangeIndex];

		DrawTextToScreen(szRebelCommandAdminActionsText[newAction*2], x, y + 7, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

		y += 22;
		DisplayWrappedString(x, y, 140, 2, FONT10ARIAL, FONT_MCOLOR_BLACK, szRebelCommandAdminActionsText[newAction*2+1], FONT_MCOLOR_BLACK, FALSE, 0);

		y += 81;
		INT32 btnId = CreateTextButton(szRebelCommandText[RCT_CANCEL], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 140, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { adminActionChangeState = CAAS_INIT; });
			});
		adminActionChangeBtnIds.push_back(btnId);

		y += 18;
		btnId = CreateTextButton(szRebelCommandText[RCT_PREV_ARROW], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x, y, 35, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { adminActionChangeIndex--; if (adminActionChangeIndex < 0) adminActionChangeIndex = static_cast<INT8>(adminActionChangeList.size() - 1); });
			});
		adminActionChangeBtnIds.push_back(btnId);

		btnId = CreateTextButton(szRebelCommandText[RCT_NEXT_ARROW], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, x+35, y, 35, 18, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { adminActionChangeIndex++; if (adminActionChangeIndex >= static_cast<INT8>(adminActionChangeList.size())) adminActionChangeIndex = 0; });
			});
		adminActionChangeBtnIds.push_back(btnId);

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
							LaptopSaveInfo.iCurrentBalance -= ADMIN_ACTION_CHANGE_COST;

							RenderWebsite();
						}
					});
				});
			});
		adminActionChangeBtnIds.push_back(btnId);
	}
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

void ToggleWebsiteView()
{
	if (websiteState == RCS_REGIONAL_OVERVIEW)
		websiteState = RCS_NATIONAL_OVERVIEW;
	else
		websiteState = RCS_REGIONAL_OVERVIEW;
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

	VOBJECT_DESC VObjectDesc;

	// load the background (white tile)
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BackGroundTile.sti", VObjectDesc.ImageFile);
	AddVideoObject(&VObjectDesc, &guiInsuranceBackGround);

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
	if (HighestPlayerProgressPercentage() >= gRebelCommandSettings.uHvtStrikesProgressRequirement && gGameExternalOptions.fEnemyRoles == TRUE && gGameExternalOptions.fAssignTraitsToEnemy == TRUE)
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

	RenderWebsite();

	return(TRUE);
}

void ExitWebsite()
{
	ClearAllButtons();
	ClearAllHelpTextRegions();
	REBEL_COMMAND_DROPDOWN.Destroy();

	DeleteVideoObjectFromIndex(guiInsuranceBackGround);
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
	ClearAllButtons();
	ClearAllHelpTextRegions();

	// background
	WebPageTileBackground(4, 4, 125, 100, guiInsuranceBackGround);

	SetFontShadow(FONT_MCOLOR_WHITE);

	// national/regional views
	switch (websiteState)
	{
	case RCS_REGIONAL_OVERVIEW:
		RenderRegionalOverview();
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

	// title
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 3;
	DrawTextToScreen(szRebelCommandText[titleText], usPosX, usPosY, 0, FONT16ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// supplies
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 23;
	DrawTextToScreen(szRebelCommandText[RCT_SUPPLIES], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// supply count
	usPosX = WEBSITE_LEFT + 50;
	usPosY = WEBSITE_TOP + 20;
	swprintf(sText, L"%d", rebelCommandSaveInfo.iSupplies);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, rebelCommandSaveInfo.iSupplies > 0 ? FONT_GREEN : FONT_MCOLOR_LTRED, FONT_MCOLOR_BLACK, FALSE, 0);

	// supplies region
	MSYS_DefineRegion(&suppliesHelpTextRegion, WEBSITE_LEFT, WEBSITE_TOP + 20, WEBSITE_LEFT + 100, WEBSITE_TOP + 35, MSYS_PRIORITY_HIGH,
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetRegionHelpText(iReason, suppliesHelpTextRegion, RCHT_SUPPLIES); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&suppliesHelpTextRegion);
	MSYS_SetRegionUserData(&suppliesHelpTextRegion, 0, 0);

	// line at the bottom of the header
	usPosX = WEBSITE_LEFT - 1;
	usPosY = WEBSITE_TOP + 35;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 500, usPosY, FROMRGB(240, 240, 240));

	// DEBUG
	if (CHEATER_CHEAT_LEVEL())
	{
		usPosX = WEBSITE_LEFT + 400;
		usPosY = WEBSITE_TOP + 380;
		dbgAdvanceDayBtnId = CreateTextButton(L"DEBUG MAGIC!", FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 99, 14, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason) {
			ButtonHelper(btn, reason, []() { DEBUG_DAY(); });
			});

		usPosY = WEBSITE_TOP + 365;
		dbgPrintBtnId = CreateTextButton(L"DEBUG PRINT!", FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 99, 14, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason) {
			ButtonHelper(btn, reason, []() { DEBUG_PRINT(); });
			});
	}

}

void RenderNationalOverview()
{
	CHAR16 sText[500];
	UINT16 usPosX, usPosY;

	// title
	RenderHeader(RCT_NATIONAL_OVERVIEW);

	// view swap button
	usPosX = WEBSITE_LEFT + 350;
	usPosY = WEBSITE_TOP + 1;
	viewSwapBtnId = CreateTextButton(szRebelCommandText[RCT_SWITCH_TO_REGIONAL], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 149, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []() { ToggleWebsiteView(); });
		});

	// incoming supplies
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 40;
	DrawTextToScreen(szRebelCommandText[RCT_INCOMING_SUPPLIES], usPosX, usPosY, 0, FONT10ARIALBOLD, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	usPosX = WEBSITE_LEFT + 5;
	usPosY += 10;
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
		CURSOR_LAPTOP_SCREEN, [](MOUSE_REGION* pRegion, INT32 iReason) { SetRegionHelpText(iReason, suppliesIncomeHelpTextRegion, RCHT_SUPPLIES_INCOME); }, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&suppliesIncomeHelpTextRegion);
	MSYS_SetRegionUserData(&suppliesIncomeHelpTextRegion, 0, 0);

	// line between incoming supplies and directive
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 43;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 500, usPosY, FROMRGB(240, 240, 240));

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
		improveDirectiveBtnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 200, 24, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { ImproveDirective(static_cast<RebelCommandDirectives>(btn->UserData[0])); });
			});

		Assert(ButtonList[improveDirectiveBtnId]);
		ButtonList[improveDirectiveBtnId]->UserData[0] = REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey();
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
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 500, usPosY, FROMRGB(240, 240, 240));

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
		DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX, usPosY + 38, FROMRGB(240, 240, 240));

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
		DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX, usPosY + 38, FROMRGB(240, 240, 240));

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
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 450, usPosY, FROMRGB(240, 240, 240));

	// training cost
	usPosX = WEBSITE_LEFT + 10;
	usPosY += 15;
	DrawTextToScreen(szRebelCommandText[RCT_MILITIA_TRAINING_COST], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// draw vertical line
	usPosX += 120;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY - 2, usPosX, usPosY + 38, FROMRGB(240, 240, 240));

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
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 450, usPosY, FROMRGB(240, 240, 240));

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
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX, usPosY + 38, FROMRGB(240, 240, 240));

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
		upgradeMilitiaStatsBtnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 150, 25, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason) {
			ButtonHelper(btn, reason, []() { UpgradeMilitiaStats(); });
			});
	}

	// dropdown - has to be last, or else things after this will be drawn twice
	REBEL_COMMAND_DROPDOWN.Display();
}

void RenderRegionalOverview()
{
	CHAR16 sText[800];
	UINT16 usPosX, usPosY;

	// title
	RenderHeader(RCT_REGIONAL_OVERVIEW);

	// view swap button
	usPosX = WEBSITE_LEFT + 350;
	usPosY = WEBSITE_TOP + 1;
	viewSwapBtnId = CreateTextButton(szRebelCommandText[RCT_SWITCH_TO_NATIONAL], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 149, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []() { ToggleWebsiteView(); });
		});

	// region 
	usPosX = WEBSITE_LEFT + 1;
	usPosY = WEBSITE_TOP + 40;
	DrawTextToScreen(szRebelCommandText[RCT_REGION], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// next region
	usPosX = WEBSITE_LEFT + 300;
	regionNextBtnId = CreateTextButton(szRebelCommandText[RCT_NEXT], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 99, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []()
				{
					RegionNavNext();
				});
		});

	// prev region
	usPosX = WEBSITE_LEFT + 400;
	regionPrevBtnId = CreateTextButton(szRebelCommandText[RCT_PREV], FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 99, 16, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
		{
			ButtonHelper(btn, reason, []()
				{
					RegionNavPrev();
				});
		});

	// region value
	usPosX = WEBSITE_LEFT + 5;
	usPosY += 12;
	swprintf(sText, L"%s", pTownNames[iCurrentRegionId]);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// line between region info and admin info
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 20;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 500, usPosY, FROMRGB(240, 240, 240));

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
	usPosX = WEBSITE_LEFT + 164;
	usPosY += 5;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX, usPosY + 15, FROMRGB(240, 240, 240));

	// loyalty
	usPosX += 30;
	usPosY -= 5;
	DrawTextToScreen(szRebelCommandText[RCT_LOYALTY], usPosX, usPosY, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// loyalty value
	usPosX += 10;
	usPosY += 12;
	swprintf(sText, L"%d", gTownLoyalty[iCurrentRegionId].ubRating);
	DrawTextToScreen(sText, usPosX, usPosY, 0, FONT14ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);
	INT32 width = 0;
	INT32 value = gTownLoyalty[iCurrentRegionId].ubRating;
	do {
		width += value % 10 == 1 ? 6 : 8;
		value /= 10;
	} while (value != 0);
	DrawTextToScreen(L"%%", usPosX + width, usPosY + 3, 0, FONT10ARIAL, FONT_MCOLOR_BLACK, FONT_MCOLOR_BLACK, FALSE, 0);

	// vertical line between loyalty and max loyalty
	usPosX = WEBSITE_LEFT + 334;
	usPosY -= 7;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX, usPosY + 15, FROMRGB(240, 240, 240));

	// max loyalty
	usPosX += 30;
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
	else if (!gTownLoyalty[iCurrentRegionId].fStarted || gTownLoyalty[iCurrentRegionId].ubRating == 0)
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
		adminTeamBtnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 300, 100, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { DeployOrReactivateAdminTeam(btn->UserData[0]); });
			});

		Assert(ButtonList[adminTeamBtnId]);
		ButtonList[adminTeamBtnId]->UserData[0] = iCurrentRegionId;

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
		adminTeamBtnId = CreateTextButton(sText, FONT10ARIAL, FONT_MCOLOR_LTYELLOW, FONT_BLACK, BUTTON_USE_DEFAULT, usPosX, usPosY, 300, 100, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK, [](GUI_BUTTON* btn, INT32 reason)
			{
				ButtonHelper(btn, reason, [btn]() { DeployOrReactivateAdminTeam(btn->UserData[0]); });
			});

		Assert(ButtonList[adminTeamBtnId]);
		ButtonList[adminTeamBtnId]->UserData[0] = iCurrentRegionId;

		return;
	}

	// line between region info and admin info
	usPosX = WEBSITE_LEFT - 1;
	usPosY += 30;
	DisplaySmallColouredLineWithShadow(usPosX, usPosY, usPosX + 500, usPosY, FROMRGB(240, 240, 240));

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
		const UINT8 level = rebelCommandSaveInfo.regions[a].actionLevels[index];
		if (level == 0)
			continue;

		// get all sectors with this townid
		std::vector<std::pair<INT16, INT16>> sectors;
		for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
			for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
				if (GetTownIdForSector(x, y) == a)
					sectors.push_back(std::pair<INT16, INT16>(x, y));

		// check if soldier is within range of the city
		for (const auto pair : sectors)
		{
			const INT16 x = std::get<0>(pair);
			const INT16 y = std::get<1>(pair);

			if (abs(x - pSoldier->sSectorX) + abs(y - pSoldier->sSectorY) <= level)
			{
				applyPenalties(pSoldier, level);
				return;
			}
		}
	}
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
		const UINT8 level = rebelCommandSaveInfo.regions[townId].actionLevels[index];
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
		const UINT8 level = rebelCommandSaveInfo.regions[townId].actionLevels[index];
		return static_cast<INT32>(info.adminActions[RCAA_MINING_POLICY].fValue1 * level);
	}

	return 0;
}

void GetBonusMilitia(INT16 sx, INT16 sy, UINT8& green, UINT8& regular, UINT8& elite, BOOLEAN createGroup)
{
	if (!gGameExternalOptions.fRebelCommandEnabled)
		return;
	
	// chance for militia to show up
	if (Random(100) >= gRebelCommandSettings.iSafehouseReinforceChance)
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
	
	BOOLEAN found = FALSE;
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
		const UINT8 level = rebelCommandSaveInfo.regions[a].actionLevels[index];
		if (level == 0)
			continue;

		// get all sectors with this townid
		std::vector<std::pair<INT16, INT16>> sectors;
		for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
			for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
				if (GetTownIdForSector(x, y) == a)
					sectors.push_back(std::pair<INT16, INT16>(x, y));

		// check if sector is within range of the city
		for (const auto pair : sectors)
		{
			const INT16 x = std::get<0>(pair);
			const INT16 y = std::get<1>(pair);

			if (abs(x - sx) + abs(y - sy) <= level)
			{
				if (level == 2)
				{
					createBonusMilitia(level);
					return;
				}

				// let's keep searching to see if we are in range of a level 2 bonus
				found = TRUE;
			}
		}
	}

	if (found)
		createBonusMilitia(1);
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
	const UINT8 level = rebelCommandSaveInfo.regions[townId].actionLevels[index];
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
			const UINT8 level = rebelCommandSaveInfo.regions[townId].actionLevels[index];
			if (level == 0)
				continue;

			townSectors.push_back(std::tuple<INT16, INT16, INT16>(x, y, level));
		}
	}

	// run through townSectors to find the biggest harriers penalty
	BOOLEAN found = FALSE;
	for (const auto trio : townSectors)
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
			const UINT8 level = rebelCommandSaveInfo.regions[townId].actionLevels[index];
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
			for (const auto trio : townSectors)
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
			const UINT8 level = rebelCommandSaveInfo.regions[townId].actionLevels[index];
			if (level == 0)
				continue;

			townSectors.push_back(std::tuple<INT16, INT16, INT16>(x, y, level));
		}
	}

	// run through townSectors to find the biggest pathfinders bonus
	BOOLEAN found = FALSE;
	for (const auto trio : townSectors)
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
	if (!gGameExternalOptions.fRebelCommandEnabled || !gGameExternalOptions.fEnemyRoles || !gGameExternalOptions.fAssignTraitsToEnemy)
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
	enemyIncome -= stolenIncome;

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
	const UINT8 progress = CurrentPlayerProgressPercentage();
	iIncomingSuppliesPerDay = static_cast<INT32>(progress * gRebelCommandSettings.fIncomeModifier + (directive == RCD_GATHER_SUPPLIES ? rebelCommandSaveInfo.directives[RCD_GATHER_SUPPLIES].GetValue1() : 0));
	rebelCommandSaveInfo.iSupplies += iIncomingSuppliesPerDay;

	// get regional bonuses
	INT16 intelGain = 0;
	INT16 supplyGain = 0;
	INT16 moneyGain = 0;
	for (int a = FIRST_TOWN; a < NUM_TOWNS; ++a)
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

		for (int b = 0; b < REBEL_COMMAND_MAX_ACTIONS_PER_REGION; ++b)
		{
			const INT8 level = rebelCommandSaveInfo.regions[a].actionLevels[b];
			switch (static_cast<RebelCommandAdminActions>(rebelCommandSaveInfo.regions[a].actions[b]))
			{
			case RCAA_SUPPLY_LINE:
			case RCAA_SAFEHOUSES:
			case RCAA_SUPPLY_DISRUPTION:
			case RCAA_SCOUTS:
			case RCAA_MERC_SUPPORT:
			case RCAA_MINING_POLICY:
			case RCAA_PATHFINDERS:
			case RCAA_HARRIERS:
			case RCAA_FORTIFICATIONS:
				// no daily bonuses
				break;

			case RCAA_REBEL_RADIO:
				IncrementTownLoyalty(a, static_cast<UINT32>(info.adminActions[RCAA_REBEL_RADIO].fValue1 * level));
				break;

			case RCAA_DEAD_DROPS:
				intelGain += (Random(static_cast<UINT32>(info.adminActions[RCAA_DEAD_DROPS].fValue1)) * level);
				break;
				
			case RCAA_SMUGGLERS:
				supplyGain += static_cast<INT16>((Random(static_cast<UINT32>(info.adminActions[RCAA_SMUGGLERS].fValue1)) * level));
				break;

			case RCAA_WAREHOUSES:
				AddResources(
					static_cast<FLOAT>(info.adminActions[RCAA_WAREHOUSES].fValue1 * level * Random(100) / 100.f),
					static_cast<FLOAT>(info.adminActions[RCAA_WAREHOUSES].fValue2 * level * Random(100) / 100.f),
					static_cast<FLOAT>(info.adminActions[RCAA_WAREHOUSES].fValue3 * level * Random(100) / 100.f));
				break;

			case RCAA_TAXES:
				moneyGain += static_cast<INT16>(info.adminActions[RCAA_TAXES].fValue1 * coolness * level * level);
				DecrementTownLoyalty(a, static_cast<UINT32>(info.adminActions[RCAA_TAXES].fValue2 * level));
				break;

			case RCAA_ASSIST_CIVILIANS:
				AddVolunteers(static_cast<FLOAT>(info.adminActions[RCAA_ASSIST_CIVILIANS].fValue1 * level * Random(100) / 100.f));
				break;


			default:
				AssertMsg(false, "Unknown Admin Action");
				break;
			}
		}
	}

	CHAR16 text[200];
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
}

void HourlyUpdate()
{
	HandleScouting();

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

			LaptopSaveInfo.iCurrentBalance -= cost;

			RenderWebsite();
		}
		});
}

void DEBUG_DAY()
{
	DailyUpdate();
}

void DEBUG_PRINT()
{
	CHAR16 text[500];
	swprintf(text, L"radio_loyalty[%.0f] safehouse_chance/min/bon[%d, %.0f, %.0f] ", info.adminActions[RCAA_REBEL_RADIO].fValue1, gRebelCommandSettings.iSafehouseReinforceChance, info.adminActions[RCAA_SAFEHOUSES].fValue1, info.adminActions[RCAA_SAFEHOUSES].fValue2);
	swprintf(text, L"%s supply_dis[%.0f] deaddrops[%.0f] smugglers[%.0f]", text, info.adminActions[RCAA_SUPPLY_DISRUPTION].fValue1, info.adminActions[RCAA_DEAD_DROPS].fValue1, info.adminActions[RCAA_SMUGGLERS].fValue1);
	swprintf(text, L"%s warehouse[%.2f/%.2f/%.2f]", text, info.adminActions[RCAA_WAREHOUSES].fValue1, info.adminActions[RCAA_WAREHOUSES].fValue2, info.adminActions[RCAA_WAREHOUSES].fValue3);
	swprintf(text, L"%s tax_inc/loy[%.0f/%.0f]", text, info.adminActions[RCAA_TAXES].fValue1, info.adminActions[RCAA_TAXES].fValue2);
	swprintf(text, L"%s volunteers[%.0f] support[%.0f]", text, info.adminActions[RCAA_ASSIST_CIVILIANS].fValue1, info.adminActions[RCAA_MERC_SUPPORT].fValue1);
	swprintf(text, L"%s minebonus[%0.1f]", text, info.adminActions[RCAA_MINING_POLICY].fValue1);
	DoMessageBox(MSG_BOX_MINIEVENT_STYLE, text, guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_BIGGER, NULL, NULL);
}

}

template<> void DropDownTemplate<DROPDOWN_REBEL_COMMAND_DIRECTIVE>::SetRefresh()
{
	using namespace RebelCommand;
	const INT16 newDirective = REBEL_COMMAND_DROPDOWN.GetSelectedEntryKey();
	rebelCommandSaveInfo.iSelectedDirective = newDirective;
	iIncomingSuppliesPerDay = static_cast<INT32>(CurrentPlayerProgressPercentage() * gRebelCommandSettings.fIncomeModifier + (newDirective == RCD_GATHER_SUPPLIES ? rebelCommandSaveInfo.directives[RCD_GATHER_SUPPLIES].GetValue1() : 0));

	redraw = TRUE;
}


