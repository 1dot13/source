#ifndef REBEL_COMMAND_H
#define REBEL_COMMAND_H

#include "mapscreen.h"
#include "Soldier Control.h"
#include "Types.h"

#define		REBEL_COMMAND_MAX_ACTIONS_PER_REGION		6
#define		NUM_ARC_AGENT_SLOTS							2

namespace RebelCommand
{
	// applies to RegionSaveInfo.actionLevels: use the MSB as the active flag since I don't expect it to be used otherwise
	constexpr UINT8 ADMIN_ACTION_ACTIVE_BIT = 1 << 7;

	// for rebelCommandSaveInfo.uBitMask
	// requirement breakdown
	// 1 bit for notification toggle
	// 5 * 4 bits for random mission selection (2off, 2def) = 32 choices each
	// need some bits to track mission area (friendly/hostile? num towns?)
	// remaining bits to track up/downgrades?
	// some should be ASD specific?
	// can we not do this and just check strategic events? GetAllStrategicEventsOfType(). could be costly. iterates over event LL on each call
	// can we cache in-flight events on load? no real need to save this, right?
	//constexpr UINT64 AGENT_NOTIFICATION_BIT = 1 << 0;

	// the FIRST and SECOND strategic events share the same type (EVENT_REBELCOMMAND - no need to have multiple types yet, I think...)
	// so the 32-bit int will have to block out information for both events
	// FIRST EVENT BREAKDOWN
	// A######B CCCCCCCC DDDDDDDD EEEEEEEE
	// A (1 bit) - always 0 to indicate that this is the FIRST event (ie, keeping a merc busy for a set duration)
	// B (1 bit) - 0 if the player sent a generic rebel agent, 1 if the player sent one of their own mercs
	// C (8 bit) - the profile number of the merc that was sent. invalid if B == 0
	// D (8 bit) - the mission ID. should match up with RebelCommandAgentMissions enum
	// E (8 bit) - the mission duration, in hours. if 0, mission failed.
	// the rest of the bits depend on the mission?
	//

	// SECOND EVENT BREAKDOWN
	// A####### #######B CCCCCCCC DDDDDDDD
	// A (1 bit) - always 1 to indicate that this is the SECOND event (ie, this event fires when the mission bonus expires)
	// B (1 bit) - 0 if the player sent a generic rebel agent, 1 if the player sent one of their own mercs
	// C (8 bit) - the profile number of the merc that was sent. invalid if B == 0
	// D (8 bit) - the mission ID. should match up with RebelCommandAgentMissions enum
	// extra bits are for ???????????

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

enum RebelCommandAgentMissions
{
	RCAM_NONE = -1,
	RCAM_DEEP_DEPLOYMENT = 0,
	RCAM_GET_ENEMY_MOVEMENT_TARGETS, // aka Strategic Intel
	RCAM_IMPROVE_LOCAL_SHOPS,
	RCAM_REDUCE_STRATEGIC_DECISION_SPEED, // aka Slower Strategic Decisions
	RCAM_REDUCE_UNALERTED_ENEMY_VISION, // aka Lower Readiness
	RCAM_SABOTAGE_INFANTRY_EQUIPMENT, // aka Sabotage Equipment
	RCAM_SABOTAGE_MECHANICAL_UNITS, // aka Sabotage Vehicles
	RCAM_TRAIN_MILITIA_ANYWHERE,

	RCAM_NUM_MISSIONS,

	// ideas/unimplemented - rename some?
	// need to have permanent and temporary boons
	RCAM_SEND_SUPPLIES_TO_TOWN,
	RCAM_BOOST_TOWN_ADMIN_ACTIONS,
	RCAM_PROCURE_ITEMS,
	RCAM_MILITIA_SKILL_TRAITS, // should override militia skill traits ini option - split into multiple (weapon spec, bodybuilding, athletic, night ops)
	RCAM_OBSERVE_SECTORS, // ??? competes with scouts?
	RCAM_PURCHASE_SUPPLIES, // increase daily supply income, decrease daily $ income
	RCAM_SABOTAGE_ASD, // see ASD.cpp for things that can be played with
	RCAM_SABOTAGE_MINE,
	RCAM_REDUCE_ENEMY_POOL, // need to make sure enemy pool is not infinite // giReinforcementPool, also gfUnlimitedTroops		= zDiffSetting[gGameOptions.ubDifficultyLevel].bUnlimitedPoolOfTroops; 
	// militia/mercs get bonus vision (???)
	// share vision with civilians?

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
	INT8 iLevel;
	RebelCommandDirectives id;

	BOOLEAN CanImprove() { return id != RCD_NONE && (size_t)iLevel < info.directives[id].iCostToImprove.size() && info.directives[id].GetCostToImprove(iLevel); }
	void Improve() { iLevel++; }
	INT32 GetCostToImprove() { return info.directives[id].GetCostToImprove(iLevel); }
	FLOAT GetValue1() { return info.directives[id].GetValue1(iLevel); }
	FLOAT GetValue2() { return info.directives[id].GetValue2(iLevel); }
} DirectiveSaveInfo;

typedef struct RegionSaveInfo
{
	RegionAdminStatus adminStatus;
	RebelCommandAdminActions actions[REBEL_COMMAND_MAX_ACTIONS_PER_REGION];
	UINT8 actionLevels[REBEL_COMMAND_MAX_ACTIONS_PER_REGION];
	UINT8 ubMaxLoyalty;

	BOOLEAN IsActive(UINT8 index) { return (actionLevels[index] & ADMIN_ACTION_ACTIVE_BIT) == 0; }
	// rftr: I know these fly in the face of convention, but I'm lazy and this preserves savegame compatibility without needing to add any additional code
	void SetActive(UINT8 index) { actionLevels[index] &= ~ADMIN_ACTION_ACTIVE_BIT; } // active bit value = 0
	void SetInactive(UINT8 index) { actionLevels[index] |= ADMIN_ACTION_ACTIVE_BIT; } // inactive bit value = 1
	INT32 GetAdminDeployCost(INT16 numAdminTeams) { return 10 * numAdminTeams * numAdminTeams; };
	INT32 GetAdminReactivateCost(INT16 numAdminTeams) { return GetAdminDeployCost(numAdminTeams) / 4; };
	UINT8 GetLevel(INT16 index) { return actionLevels[index] & ~ADMIN_ACTION_ACTIVE_BIT; }
} RegionSaveInfo;

typedef struct SaveInfo
{
	RegionSaveInfo regions[MAX_TOWNS];
	DirectiveSaveInfo directives[20];

	INT32 iSupplies;
	INT32 iActiveDirective;
	INT32 iSelectedDirective;
	INT8 iMilitiaStatsLevel;
	UINT8 uSupplyDropCount; // keeping this around for compatibility with old saves
	INT8 availableMissions[NUM_ARC_AGENT_SLOTS];

	INT8 filler[17];
} SaveInfo;

extern SaveInfo rebelCommandSaveInfo;

BOOLEAN EnterWebsite();
void ExitWebsite();
void RenderWebsite();
void HandleWebsite();

// admin actions
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

// agent missions
void ApplyEnemyMechanicalUnitPenalties(SOLDIERTYPE* pSoldier);
void ApplyMilitiaTraits(SOLDIERTYPE* pSoldier);
void ApplyVisionModifier(const SOLDIERTYPE* pSoldier, INT32& sight);
BOOLEAN CanAssignTraitsToMilitia();
BOOLEAN CanTrainMilitiaAnywhere();
UINT8 GetMaxTrainersForTrainMilitiaAnywhere();
INT16 GetAdditionalDeployRange(const UINT8 insertionCode);
INT8 GetEnemyEquipmentCoolnessModifier();
INT8 GetEnemyEquipmentStatusModifier(const INT8 initialStatus);
UINT8 GetMerchantCoolnessBonus();
FLOAT GetStrategicDecisionSpeedModifier();
void HandleStrategicEvent(const UINT32 eventParam);
BOOLEAN ShowEnemyMovementTargets();

void DailyUpdate();
void HourlyUpdate();
void Init();
void ShowWebsiteAvailableMessage();
BOOLEAN Load(HWFILE file);
BOOLEAN Save(HWFILE file);

}

#endif

