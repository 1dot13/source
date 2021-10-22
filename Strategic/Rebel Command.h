#ifndef REBEL_COMMAND_H
#define REBEL_COMMAND_H

#include "mapscreen.h"
#include "Soldier Control.h"
#include "Types.h"

#define		REBEL_COMMAND_MAX_ACTIONS_PER_REGION		6

namespace RebelCommand
{

enum RebelCommandDirectives
{
	RCD_NONE = -1,
	RCD_GATHER_SUPPLIES = 0,
	RCD_SUPPORT_MILITIA,
	RCD_TRAIN_MILITIA,
	RCD_CREATE_PROPAGANDA,
	RCD_ELITE_MILITIA,
	RCD_HVT_STRIKES,
	RCD_SPOTTERS,
	RCD_RAID_MINES,
	RCD_CREATE_TURNCOATS,
	RCD_DRAFT,

	RCD_NUM_DIRECTIVES
};

enum RebelCommandAdminActions
{
	RCAA_NONE = -1,
	RCAA_SUPPLY_LINE = 0,
	RCAA_REBEL_RADIO,
	RCAA_SAFEHOUSES,
	RCAA_SUPPLY_DISRUPTION,
	RCAA_SCOUTS,
	RCAA_DEAD_DROPS,
	RCAA_SMUGGLERS,
	RCAA_WAREHOUSES,
	RCAA_TAXES,
	RCAA_ASSIST_CIVILIANS,
	RCAA_MERC_SUPPORT,
	RCAA_MINING_POLICY,
	RCAA_PATHFINDERS,
	RCAA_HARRIERS,
	RCAA_FORTIFICATIONS,

	RCAA_NUM_ACTIONS
};

enum RegionAdminStatus
{
	RAS_NONE,
	RAS_ACTIVE,
	RAS_INACTIVE
};

typedef struct Directive {
	std::vector<INT32> iCostToImprove;
	std::vector<FLOAT> fValue1;
	std::vector<FLOAT> fValue2;

	INT32 GetCostToImprove(INT8 currentLevel) { return iCostToImprove[currentLevel]; }
	FLOAT GetValue1(INT8 currentLevel) { return fValue1[currentLevel]; }
	FLOAT GetValue2(INT8 currentLevel) { return fValue2[currentLevel]; }
} Directive;

typedef struct AdminAction {
	FLOAT fValue1;
	FLOAT fValue2;
	FLOAT fValue3;
} AdminAction;

typedef struct Info {
	std::vector<Directive> directives;
	std::vector<AdminAction> adminActions;
} Info;

extern Info info;

typedef struct DirectiveSaveInfo {
	INT8 iLevel = 0;
	RebelCommandDirectives id = RCD_NONE;

	BOOLEAN CanImprove() { return id != RCD_NONE && (size_t)iLevel < info.directives[id].iCostToImprove.size() && info.directives[id].GetCostToImprove(iLevel); }
	void Improve() { iLevel++; }
	INT32 GetCostToImprove() { return info.directives[id].GetCostToImprove(iLevel); }
	FLOAT GetValue1() { return info.directives[id].GetValue1(iLevel); }
	FLOAT GetValue2() { return info.directives[id].GetValue2(iLevel); }
} DirectiveSaveInfo;

typedef struct RegionSaveInfo
{
	RegionAdminStatus adminStatus = RAS_NONE;
	RebelCommandAdminActions actions[REBEL_COMMAND_MAX_ACTIONS_PER_REGION];
	UINT8 actionLevels[REBEL_COMMAND_MAX_ACTIONS_PER_REGION];
	UINT8 ubMaxLoyalty = 50;

	INT32 GetAdminDeployCost(INT16 numAdminTeams) { return 10 * numAdminTeams * numAdminTeams; };
	INT32 GetAdminReactivateCost(INT16 numAdminTeams) { return GetAdminDeployCost(numAdminTeams) / 4; };
} RegionSaveInfo;

typedef struct SaveInfo
{
	RegionSaveInfo regions[MAX_TOWNS];
	DirectiveSaveInfo directives[20];

	INT32 iSupplies = 0;
	INT32 iActiveDirective = RCD_GATHER_SUPPLIES;
	INT32 iSelectedDirective = RCD_GATHER_SUPPLIES;
	INT8 iMilitiaStatsLevel = 0;
	UINT8 uSupplyDropCount = 0;

	INT8 filler[19];
} SaveInfo;

extern SaveInfo rebelCommandSaveInfo;

BOOLEAN EnterWebsite();
void ExitWebsite();
void RenderWebsite();
void HandleWebsite();

void ApplyEnemyPenalties(SOLDIERTYPE* pSoldier);
void ApplyMilitiaBonuses(SOLDIERTYPE* pMilitia);
UINT8 GetApproximateEnemyLocationResolutionIndex();
FLOAT GetAssignmentBonus(INT16 x, INT16 y);
INT32 GetMiningPolicyBonus(INT16 townId);
void GetBonusMilitia(INT16 x, INT16 y, UINT8& green, UINT8& regular, UINT8& elite, BOOLEAN createGroup);
INT16 GetFortificationsBonus(UINT8 sector);
FLOAT GetHarriersSpeedPenalty(UINT8 sector);
FLOAT GetLoyaltyGainModifier();
UINT8 GetMaxTownLoyalty(INT8 townId);
INT16 GetMilitiaTrainingSpeedBonus();
FLOAT GetMilitiaTrainingCostModifier();
FLOAT GetMilitiaUpkeepCostModifier();
FLOAT GetPathfindersSpeedBonus(UINT8 sector);
BOOLEAN NeutraliseRole(const SOLDIERTYPE* pSoldier);
void RaidMines(INT32 &playerIncome, INT32 &enemyIncome);
BOOLEAN ShowApproximateEnemyLocations();
void ShowWebsiteAvailableMessage();

void DailyUpdate();
void HourlyUpdate();
void Init();
BOOLEAN Load(HWFILE file);
BOOLEAN Save(HWFILE file);

}

//BOOLEAN LoadRebelCommand( HWFILE file ) { return RebelCommand::Load( file ); }

#endif

