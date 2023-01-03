/*
Mini events are a feature where the player can receive short text blurbs, and pick one of two outcomes.
Based on game implementations like Gloomhaven (road/city events) or Battletech (travel events).

The general idea is that every so often, the game checks to see if an event should be triggered. These shouldn't be very
frequent so that the usual gameplay isn't interrupted that often.

Mini events are set up in MiniEvents.lua. This file handles mini event triggers and defines functions for the lua file
to call into.
*/

#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
#include "MiniEvents.h"

#include "Assignments.h"
#include "Campaign.h"
#include "Campaign Types.h"
#include "connect.h"
#include "finances.h"
#include "Game Clock.h"
#include "Game Event Hook.h"
#include "GameSettings.h"
#include "LaptopSave.h"
#include "mapscreen.h"
#include "Map Screen Helicopter.h"
#include "message.h"
#include "MessageBoxScreen.h"
#include "MilitiaIndividual.h"
#include "Morale.h"
#include "Overhead.h"
#include "Overhead Types.h"
#include "random.h"
#include "Soldier Control.h"
#include "Soldier macros.h"
#include "Soldier Profile.h"
#include "Squads.h"
#include "strategic.h"
#include "strategicmap.h"
#include "Strategic Movement.h"
#include "Strategic Town Loyalty.h"
#include "Text.h"
#include "Town Militia.h"
#include "Vehicles.h"
#endif

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <lua.h>
#include <lua_function.h>
#include <lua_state.h>
#include <lua_table.h>

extern CHAR16 gzUserDefinedButton1[ 128 ];
extern CHAR16 gzUserDefinedButton2[ 128 ];
extern CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT];

static size_t MAX_BUTTON_LENGTH = 60;
static size_t MAX_BODY_LENGTH = 450;

static UINT32 guiMiniEventsCachedScreen;
static LuaState gLS;
static std::vector<SOLDIERTYPE*> gAllMercs;

static void QueueNextMiniEvent(UINT32 nextEventId, UINT32 hoursToNextMiniEvent);

// LUA STUFF
static void MiniEventsLua(UINT32 eventId);
static int MiniEventsLua_MessageBox(lua_State* LS);
static void MiniEventsLua_MessageBoxCallback(UINT8 ubExitValue);
static int MiniEventsLua_ResolveEvent(lua_State* LS);
static int MiniEventsLua_ScreenMsg(lua_State* LS);

namespace MiniEventHelpers
{
	enum Skills
	{
		SKILL_START = 0,
		SKILL_AUTO_WEAPONS,
		SKILL_HEAVY_WEAPONS,
		SKILL_MARKSMAN,
		SKILL_HUNTER,
		SKILL_GUNSLINGER,
		SKILL_HAND_TO_HAND,
		SKILL_DEPUTY,
		SKILL_TECHNICIAN,
		SKILL_PARAMEDIC,
		SKILL_AMBIDEXTROUS,
		SKILL_MELEE,
		SKILL_THROWING,
		SKILL_NIGHT_OPS,
		SKILL_STEALTHY,
		SKILL_ATHLETICS,
		SKILL_BODYBUILDING,
		SKILL_DEMOLITIONS,
		SKILL_TEACHING,
		SKILL_SCOUTING,
		SKILL_COVERT_OPS,
		SKILL_RADIO_OPERATOR,
		SKILL_SNITCH,
		SKILL_SURVIVAL,
		SKILL_END
	};

	enum Stats
	{
		STAT_START = -1,
		STAT_LIFE = 0,
		STAT_STRENGTH,
		STAT_AGILITY,
		STAT_DEXTERITY,
		STAT_WISDOM,
		STAT_LEADERSHIP,
		STAT_MARKSMANSHIP,
		STAT_MECHANICAL,
		STAT_EXPLOSIVE,
		STAT_MEDICAL,
		STAT_EXPLEVEL,
		STAT_MAX
	};

	static BOOLEAN IsMajorSkill(const INT8 skillId)
	{
		return skillId >= Skills::SKILL_AUTO_WEAPONS && skillId <= Skills::SKILL_PARAMEDIC || skillId == Skills::SKILL_COVERT_OPS;
	}

	static int l_AddMoneyToPlayerAccount(lua_State* LS)
	{
		const INT32 currentBalance = LaptopSaveInfo.iCurrentBalance;
		const INT32 amount = lua_tointeger(LS, 1);
		bool forceToZero;

		if (lua_gettop(LS) == 2)
			forceToZero = lua_toboolean(LS, 2);

		if (currentBalance + amount < 0)
		{
			if (forceToZero)
			{
				AddTransactionToPlayersBook(MINI_EVENT, 0, GetWorldTotalMin(), -currentBalance);
				lua_pushboolean(LS, true);
			}
			else
			{
				lua_pushboolean(LS, false);
			}
		}
		else
		{
			AddTransactionToPlayersBook(MINI_EVENT, 0, GetWorldTotalMin(), amount);
			lua_pushboolean(LS, true);
		}

		return 1;
	}

	static int l_AddIntel(lua_State* LS)
	{
		INT32 amount = lua_tointeger(LS, 1);

		if (!gGameExternalOptions.fIntelResource)
		{
			// intel is disabled. give some cash instead
			amount *= 500;
			const INT32 currentBalance = LaptopSaveInfo.iCurrentBalance;
			if (currentBalance + amount < 0)
			{
				lua_pushboolean(LS, false);
			}
			else
			{
				AddTransactionToPlayersBook(MINI_EVENT, 0, GetWorldTotalMin(), amount);
				lua_pushboolean(LS, true);
			}

			return 1;
		}

		const float currentIntel = LaptopSaveInfo.dIntelPool;

		if (currentIntel + amount < 0)
		{
			lua_pushboolean(LS, false);
		}
		else
		{
			AddIntel(static_cast<FLOAT>(amount), TRUE);
			lua_pushboolean(LS, true);
		}

		lua_pushboolean(LS, true);
		return 1;
	}

	static int l_AddSkill(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));
		const INT8 skillId = static_cast<INT8>(lua_tointeger(LS, 2));

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [profileId, skillId](SOLDIERTYPE* merc) {
			if (merc->ubProfile != profileId)
				return;

			int firstAvailableIndex = sizeof(gMercProfiles[merc->ubProfile].bSkillTraits) / sizeof(gMercProfiles[merc->ubProfile].bSkillTraits[0]);
			int skillCount = 0;
			INT8* skillTraits = gMercProfiles[merc->ubProfile].bSkillTraits;

			for (int i = 0; i < sizeof(gMercProfiles[merc->ubProfile].bSkillTraits) / sizeof(gMercProfiles[merc->ubProfile].bSkillTraits[0]); ++i)
			{
				if (*(skillTraits + i) == 0 && firstAvailableIndex > i)
					firstAvailableIndex = i;

				if (*(skillTraits + i) == skillId)
					skillCount++;
			}

			if (skillCount == 0 || (IsMajorSkill(skillId) && skillCount < 2))
				*(skillTraits + firstAvailableIndex) = skillId;
		});


		return 0;
	}

	static int l_AddTownLoyalty(lua_State* LS)
	{
		const INT8 townId = static_cast<INT8>(lua_tointeger(LS, 1));
		const int points = lua_tointeger(LS, 2);

		if (points > 0)
		{
			IncrementTownLoyalty(townId, static_cast<UINT32>(points));
		}
		else if (points < 0)
		{
			DecrementTownLoyalty(townId, static_cast<UINT32>(std::abs(points)));
		}

		return 0;
	}

	static int l_AdjustBreathMax(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));
		const int val = lua_tointeger(LS, 2);

		if (val == 0)
			return 0;

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [profileId, val](SOLDIERTYPE* merc) {
			if (merc->ubProfile == profileId)
			{
				int newBreathValue = merc->bBreathMax + val;
				newBreathValue = max(min(newBreathValue, 100), 0);

				merc->bBreathMax = newBreathValue;
				merc->bBreath = max(min(merc->bBreath, merc->bBreathMax), 0);
			}
		});

		return 0;
	}

	static int l_AdjustEnemyStrengthInSector(lua_State* LS)
	{
		const INT16 x = lua_tointeger(LS, 1);
		const INT16 y = lua_tointeger(LS, 2);
		const INT8 adminAdjustment = static_cast<INT8>(lua_tointeger(LS, 3));
		const INT8 troopAdjustment = static_cast<INT8>(lua_tointeger(LS, 4));
		const INT8 eliteAdjustment = static_cast<INT8>(lua_tointeger(LS, 5));
		const INT8 robotAdjustment = gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsRobots ? static_cast<INT8>(lua_tointeger(LS, 6)) : 0;
		const INT8 jeepAdjustment = gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsJeeps ? static_cast<INT8>(lua_tointeger(LS, 7)) : 0;
		const INT8 tankAdjustment = gGameExternalOptions.fASDActive && gGameExternalOptions.fASDAssignsTanks ? static_cast<INT8>(lua_tointeger(LS, 8)) : 0;

		SECTORINFO *sector = &(SectorInfo[SECTOR(x,y)]);
		if (!sector)
			return 0;

		sector->ubNumAdmins = min(max(0, sector->ubNumAdmins + adminAdjustment), 30);
		sector->ubNumTroops = min(max(0, sector->ubNumTroops + troopAdjustment), 30);
		sector->ubNumElites = min(max(0, sector->ubNumElites + eliteAdjustment), 30);
		sector->ubNumRobots = min(max(0, sector->ubNumRobots + robotAdjustment), 30);
		sector->ubNumJeeps = min(max(0, sector->ubNumJeeps + jeepAdjustment), 30);
		sector->ubNumTanks = min(max(0, sector->ubNumTanks + tankAdjustment), 30);

		return 0;
	}

	static int l_AdjustMorale(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));;
		const INT8 val = static_cast<INT8>(lua_tointeger(LS, 2));

		if (val == 0)
			return 0;

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [val, profileId](SOLDIERTYPE* merc) {
			if (merc->ubProfile == profileId)
			{
				merc->aiData.bStrategicMoraleMod += val;
				merc->aiData.bStrategicMoraleMod = min(merc->aiData.bStrategicMoraleMod, gMoraleSettings.bModifiers[MORALE_MOD_MAX]);
				merc->aiData.bStrategicMoraleMod = max(merc->aiData.bStrategicMoraleMod, -gMoraleSettings.bModifiers[MORALE_MOD_MAX]);
				RefreshSoldierMorale(merc);
			}
		});

		return 0;
	}

	static int l_AdjustStat(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));;
		const UINT16 stat = lua_tointeger(LS, 2);
		const INT16 val = lua_tointeger(LS, 3);

		if (stat <= STAT_START || stat >= STAT_EXPLEVEL || val == 0)
			return 0;

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [stat, val, profileId](SOLDIERTYPE* merc) {
			if (merc->ubProfile != profileId)
				return;

			INT16 amount = val;
			CHAR16 wTempString[ 128 ];
			int statId = -1;

			switch (stat)
			{
			case STAT_LIFE:
				amount = max(min(100 - merc->stats.bLifeMax, amount), -merc->stats.bLifeMax);
				merc->stats.bLifeMax += amount;
				merc->stats.bLife += amount;
				statId = HEALTHAMT;
				merc->timeChanges.uiChangeHealthTime = GetJA2Clock();
				
				if (amount < 0)
				{
					merc->ubCriticalStatDamage[DAMAGED_STAT_HEALTH] -= amount;
					gMercProfiles[merc->ubProfile].bLifeMax = merc->stats.bLifeMax;
					gMercProfiles[merc->ubProfile].bLife = min(gMercProfiles[merc->ubProfile].bLife, gMercProfiles[merc->ubProfile].bLifeMax);
					merc->usValueGoneUp &= ~( HEALTH_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bLifeDelta += amount;
					merc->usValueGoneUp |= HEALTH_INCREASE;
				}
				break;
			case STAT_STRENGTH:
				amount = max(min(100 - merc->stats.bStrength, amount), -merc->stats.bStrength);
				merc->stats.bStrength += amount;
				statId = STRAMT;
				merc->timeChanges.uiChangeStrengthTime = GetJA2Clock();

				if (amount < 0)
				{
					merc->ubCriticalStatDamage[DAMAGED_STAT_STRENGTH] -= amount;
					gMercProfiles[merc->ubProfile].bStrength = merc->stats.bStrength;
					merc->usValueGoneUp &= ~( STRENGTH_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bStrengthDelta += amount;
					merc->usValueGoneUp |= STRENGTH_INCREASE;
				}
				break;
			case STAT_AGILITY:
				amount = max(min(100 - merc->stats.bAgility, amount), -merc->stats.bAgility);
				merc->stats.bAgility += amount;
				statId = AGILAMT;
				merc->timeChanges.uiChangeAgilityTime = GetJA2Clock();

				if (amount < 0)
				{
					merc->ubCriticalStatDamage[DAMAGED_STAT_AGILITY] -= amount;
					gMercProfiles[merc->ubProfile].bAgility = merc->stats.bAgility;
					merc->usValueGoneUp &= ~( AGIL_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bAgilityDelta += amount;
					merc->usValueGoneUp |= AGIL_INCREASE;
				}
				break;
			case STAT_DEXTERITY:
				amount = max(min(100 - merc->stats.bDexterity, amount), -merc->stats.bDexterity);
				merc->stats.bDexterity += amount;
				statId = DEXTAMT;
				merc->timeChanges.uiChangeDexterityTime = GetJA2Clock();

				if (amount < 0)
				{
					merc->ubCriticalStatDamage[DAMAGED_STAT_DEXTERITY] -= amount;
					gMercProfiles[merc->ubProfile].bDexterity = merc->stats.bDexterity;
					merc->usValueGoneUp &= ~( DEX_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bDexterityDelta += amount;
					merc->usValueGoneUp |= DEX_INCREASE;
				}
				break;
			case STAT_WISDOM:
				amount = max(min(100 - merc->stats.bWisdom, amount), -merc->stats.bWisdom);
				merc->stats.bWisdom += amount;
				statId = WISDOMAMT;
				merc->timeChanges.uiChangeWisdomTime = GetJA2Clock();

				if (amount < 0)
				{
					merc->ubCriticalStatDamage[DAMAGED_STAT_WISDOM] -= amount;
					gMercProfiles[merc->ubProfile].bWisdom = merc->stats.bWisdom;
					merc->usValueGoneUp &= ~( WIS_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bWisdomDelta += amount;
					merc->usValueGoneUp |= WIS_INCREASE;
				}
				break;
			case STAT_LEADERSHIP:
				amount = max(min(100 - merc->stats.bLeadership, amount), -merc->stats.bLeadership);
				merc->stats.bLeadership += amount;
				statId = LDRAMT;
				merc->timeChanges.uiChangeLeadershipTime = GetJA2Clock();

				if (amount < 0)
				{
					gMercProfiles[merc->ubProfile].bLeadership = merc->stats.bLeadership;
					merc->usValueGoneUp &= ~( LDR_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bLeadershipDelta += amount;
					merc->usValueGoneUp |= LDR_INCREASE;
				}
				break;
			case STAT_MARKSMANSHIP:
				amount = max(min(100 - merc->stats.bMarksmanship, amount), -merc->stats.bMarksmanship);
				merc->stats.bMarksmanship += amount;
				statId = MARKAMT;
				merc->timeChanges.uiChangeMarksmanshipTime = GetJA2Clock();

				if (amount < 0)
				{
					gMercProfiles[merc->ubProfile].bMarksmanship = merc->stats.bMarksmanship;
					merc->usValueGoneUp &= ~( MRK_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bMarksmanshipDelta += amount;
					merc->usValueGoneUp |= MRK_INCREASE;
				}
				break;
			case STAT_MECHANICAL:
				amount = max(min(100 - merc->stats.bMechanical, amount), -merc->stats.bMechanical);
				merc->stats.bMechanical += amount;
				statId = MECHANAMT;
				merc->timeChanges.uiChangeMechanicalTime = GetJA2Clock();

				if (amount < 0)
				{
					gMercProfiles[merc->ubProfile].bMechanical = merc->stats.bMechanical;
					merc->usValueGoneUp &= ~( MECH_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bMechanicDelta += amount;
					merc->usValueGoneUp |= MECH_INCREASE;
				}
				break;
			case STAT_EXPLOSIVE:
				amount = max(min(100 - merc->stats.bExplosive, amount), -merc->stats.bExplosive);
				merc->stats.bExplosive += amount;
				statId = EXPLODEAMT;
				merc->timeChanges.uiChangeExplosivesTime = GetJA2Clock();

				if (amount < 0)
				{
					gMercProfiles[merc->ubProfile].bExplosive = merc->stats.bExplosive;
					merc->usValueGoneUp &= ~( EXP_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bExplosivesDelta += amount;
					merc->usValueGoneUp |= EXP_INCREASE;
				}
				break;
			case STAT_MEDICAL:
				amount = max(min(100 - merc->stats.bMedical, amount), -merc->stats.bMedical);
				merc->stats.bMedical += amount;
				statId = MEDICALAMT;
				merc->timeChanges.uiChangeMedicalTime = GetJA2Clock();

				if (amount < 0)
				{
					gMercProfiles[merc->ubProfile].bMedical = merc->stats.bMedical;
					merc->usValueGoneUp &= ~( MED_INCREASE );
				}
				else if (amount > 0)
				{
					gMercProfiles[merc->ubProfile].bMedicalDelta += amount;
					merc->usValueGoneUp |= MED_INCREASE;
				}
				break;
			}

			if (amount != 0)
			{
				BuildStatChangeString(wTempString, merc->GetName(), amount > 0, amount > 0 ? amount : -amount, statId);
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, wTempString );
			}
		});

		return 0;
	}

	static int l_AdjustVehicleFuel(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));;
		const INT16 val = lua_tointeger(LS, 2);
		INT32 vehicleId = -1;
		
		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId)
			{
				vehicleId = (*iter)->iVehicleId;
				break;

			}
		}

		if (vehicleId == -1)
		{
			// invalid vehicle id
			lua_pushboolean(LS, false);
			lua_pushstring(LS, "");
			return 2;
		}

		for (int index = 0; index < ubNumberOfVehicles; ++index)
		{
			if (pVehicleList[index].fValid == TRUE)
			{
				SOLDIERTYPE* vehicle = GetSoldierStructureForVehicle(index);

				if (vehicle)
				{
					SpendVehicleFuel(vehicle, -(100*val));

					lua_pushboolean(LS, true);
					MERCPROFILESTRUCT mps = gMercProfiles[vehicle->ubProfile];
					CHAR8 nickname[50];
					sprintf(nickname, "%ls", mps.zNickname);
					lua_pushstring(LS, nickname);
					return 2;
				}
			}
		}

		lua_pushboolean(LS, false);
		lua_pushstring(LS, "");
		return 2;
	}

	static int l_AdjustVehicleHealth(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));;
		const INT16 val = lua_tointeger(LS, 2);
		INT32 vehicleId = -1;
		
		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId)
			{
				vehicleId = (*iter)->iVehicleId;
				break;

			}
		}

		if (vehicleId == -1)
		{
			// invalid vehicle id
			lua_pushboolean(LS, false);
			lua_pushstring(LS, "");
			return 2;
		}

		for (int index = 0; index < ubNumberOfVehicles; ++index)
		{
			if (pVehicleList[index].fValid == TRUE)
			{
				SOLDIERTYPE* vehicle = GetSoldierStructureForVehicle(index);

				if (vehicle)
				{
					vehicle->stats.bLife += val;
					vehicle->stats.bLife = max(min(vehicle->stats.bLife, 100), 0);

					lua_pushboolean(LS, true);
					MERCPROFILESTRUCT mps = gMercProfiles[vehicle->ubProfile];
					CHAR8 nickname[50];
					sprintf(nickname, "%ls", mps.zNickname);
					lua_pushstring(LS, nickname);
					return 2;
				}
			}
		}

		lua_pushboolean(LS, false);
		lua_pushstring(LS, "");
		return 2;
	}

	static int l_ApplyPermanentStatDamage(lua_State* LS)
	{
		// note that we're intentionally NOT incrementing the damaged stat array (ubCriticalStatDamage) as we do not want these penalties to be doctorable!

		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));
		const UINT16 stat = static_cast<UINT16>(lua_tointeger(LS, 2));
		const int amount = lua_tointeger(LS, 3);

		if (amount <= 0)
			return 0;

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [profileId, stat, amount](SOLDIERTYPE* merc) {
			if (merc->ubProfile != profileId)
				return;

			int loss = amount;

			switch (stat)
			{
			case STAT_LIFE:
				if (loss >= merc->stats.bLifeMax)
				{
					loss = merc->stats.bLifeMax - 1;
				}
				merc->stats.bLifeMax -= loss;
				merc->stats.bLife = min(merc->stats.bLife, merc->stats.bLifeMax);

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[merc->ubProfile].bLifeMax = merc->stats.bLifeMax;
					gMercProfiles[merc->ubProfile].bLife = min(gMercProfiles[merc->ubProfile].bLife, gMercProfiles[merc->ubProfile].bLifeMax);
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeHealthTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( HEALTH_INCREASE );
				}
				break;
			case STAT_STRENGTH:
				if (loss >= merc->stats.bStrength)
				{
					loss = merc->stats.bStrength - 1;
				}
				merc->stats.bStrength -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bStrength = merc->stats.bStrength;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeStrengthTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( STRENGTH_INCREASE );
				}
				break;
			case STAT_AGILITY:
				if (loss >= merc->stats.bAgility)
				{
					loss = merc->stats.bAgility - 1;
				}
				merc->stats.bAgility -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bAgility = merc->stats.bAgility;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeAgilityTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( AGIL_INCREASE );
				}
				break;
			case STAT_DEXTERITY:
				if (loss >= merc->stats.bDexterity)
				{
					loss = merc->stats.bDexterity - 1;
				}
				merc->stats.bDexterity -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bDexterity = merc->stats.bDexterity;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeDexterityTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( DEX_INCREASE );
				}
				break;
			case STAT_WISDOM:
				if (loss >= merc->stats.bWisdom)
				{
					loss = merc->stats.bWisdom - 1;
				}
				merc->stats.bWisdom -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bWisdom = merc->stats.bWisdom;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeWisdomTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( WIS_INCREASE );
				}
				break;
			case STAT_LEADERSHIP:
				if (loss >= merc->stats.bLeadership)
				{
					loss = merc->stats.bLeadership - 1;
				}
				merc->stats.bLeadership -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bLeadership = merc->stats.bLeadership;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeLeadershipTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( LDR_INCREASE );
				}
				break;
			case STAT_MARKSMANSHIP:
				if (loss >= merc->stats.bMarksmanship)
				{
					loss = merc->stats.bMarksmanship - 1;
				}
				merc->stats.bMarksmanship -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bMarksmanship = merc->stats.bMarksmanship;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeMarksmanshipTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( MRK_INCREASE );
				}
				break;
			case STAT_MECHANICAL:
				if (loss >= merc->stats.bMechanical)
				{
					loss = merc->stats.bMechanical - 1;
				}
				merc->stats.bMechanical -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bMechanical = merc->stats.bMechanical;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeMechanicalTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( MECH_INCREASE );
				}
				break;
			case STAT_EXPLOSIVE:
				if (loss >= merc->stats.bExplosive)
				{
					loss = merc->stats.bExplosive - 1;
				}
				merc->stats.bExplosive -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bExplosive = merc->stats.bExplosive;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeExplosivesTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( EXP_INCREASE );
				}
				break;
			case STAT_MEDICAL:
				if (loss >= merc->stats.bMedical)
				{
					loss = merc->stats.bMedical - 1;
				}
				merc->stats.bMedical -= loss;

				if (merc->ubProfile != NO_PROFILE)
				{
					gMercProfiles[ merc->ubProfile ].bMedical = merc->stats.bMedical;
				}

				if (merc->name[0] && merc->bVisible == TRUE)
				{
					merc->timeChanges.uiChangeMedicalTime = GetJA2Clock();
					merc->usValueGoneUp &= ~( MED_INCREASE );
				}
				break;
			}
		});

		return 0;
	}

	static int l_ApplyDamage(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));
		const int amount = lua_tointeger(LS, 2);

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [profileId, amount](SOLDIERTYPE* merc) {
			if (merc->ubProfile != profileId)
				return;

			int newLifeValue = merc->stats.bLife - amount;
			newLifeValue = max(min(merc->stats.bLifeMax, newLifeValue), 0);

			merc->stats.bLife = newLifeValue;

			if (merc->ubProfile != NO_PROFILE)
			{
				gMercProfiles[merc->ubProfile].bLife = merc->stats.bLife;
			}

			if (merc->stats.bLife <= 0)
			{
				HandleStrategicDeath(merc);
			}
			else if (merc->stats.bLife < 15)
			{
				merc->stats.bLife = 15;
			}
		});

		return 0;
	}

	static int l_CheckForAssignment(lua_State* LS)
	{
		const INT8 assignment = lua_tointeger(LS, 1);
		INT16 sectorX = 0;
		INT16 sectorY = 0;
		INT8 sectorZ = 0;
		bool globalSearch = true;

		if (lua_gettop(LS) == 4)
		{
			globalSearch = false;
			sectorX = lua_tointeger(LS, 2);
			sectorY = lua_tointeger(LS, 3);
			sectorZ = lua_tointeger(LS, 4);
		}

		std::vector<SOLDIERTYPE*> foundMercs;

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if (globalSearch || ((*iter)->sSectorX == sectorX && (*iter)->sSectorY == sectorY && (*iter)->bSectorZ == sectorZ))
			{
				if ((*iter)->bAssignment == assignment)
				{
					foundMercs.push_back(*iter);
				}
				else if (assignment == ON_DUTY && (*iter)->bAssignment < ON_DUTY)
				{
					foundMercs.push_back(*iter);
				}
			}
		}

		if (foundMercs.size() == 0)
		{
			lua_pushboolean(LS, false);
			lua_pushstring(LS, "");
			lua_pushinteger(LS, 0);
		}
		else
		{
			const UINT8 index = Random(foundMercs.size());

			MERCPROFILESTRUCT merc = gMercProfiles[foundMercs[index]->ubProfile];
			CHAR8 nickname[50];
			sprintf(nickname, "%ls", merc.zNickname);
			lua_pushboolean(LS, true);
			lua_pushstring(LS, nickname);
			lua_pushinteger(LS, foundMercs[index]->ubProfile);
		}

		return 3;
	}

	static int l_CheckForSkill(lua_State* LS)
	{
		const INT8 skill = lua_tointeger(LS, 1);
		INT16 sectorX = 0;
		INT16 sectorY = 0;
		INT8 sectorZ = 0;
		INT8 profileId = 0;
		bool globalSearch = true;
		bool searchAllMercs = true;

		if (lua_gettop(LS) == 4)
		{
			globalSearch = false;
			sectorX = lua_tointeger(LS, 2);
			sectorY = lua_tointeger(LS, 3);
			sectorZ = lua_tointeger(LS, 4);
		}

		if (lua_gettop(LS) == 2)
		{
			searchAllMercs = false;
			profileId = static_cast<UINT8>(lua_tointeger(LS, 2));
		}

		std::vector<SOLDIERTYPE*> foundMercs;

		if (gGameOptions.fNewTraitSystem)
		{
			for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
			{
				MERCPROFILESTRUCT merc = gMercProfiles[(*iter)->ubProfile];
				if (searchAllMercs || (*iter)->ubProfile == profileId)
				{
					for (int i = 0; i < sizeof(merc.bSkillTraits) / sizeof(merc.bSkillTraits[0]); ++i)
					{
						if (globalSearch || ((*iter)->sSectorX == sectorX && (*iter)->sSectorY == sectorY && (*iter)->bSectorZ == sectorZ))
						{
							if(merc.bSkillTraits[i] == skill)
							{
								foundMercs.push_back(*iter);
							}
						}
					}
				}
			}
		}

		if (foundMercs.size() == 0)
		{
			lua_pushboolean(LS, false);
			lua_pushstring(LS, "");
			lua_pushinteger(LS, 0);
		}
		else
		{
			const UINT8 index = Random(foundMercs.size());

			MERCPROFILESTRUCT merc = gMercProfiles[foundMercs[index]->ubProfile];
			CHAR8 nickname[50];
			sprintf(nickname, "%ls", merc.zNickname);
			lua_pushboolean(LS, true);
			lua_pushstring(LS, nickname);
			lua_pushinteger(LS, foundMercs[index]->ubProfile);
		}

		return 3;
	}

	static int l_CheckForSleep(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId && (*iter)->flags.fMercAsleep)
			{
				lua_pushboolean(LS, true);
				return 1;
			}
		}

		lua_pushboolean(LS, false);
		return 1;
	}

	static int l_CheckForTravel(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId && (*iter)->flags.fBetweenSectors)
			{
				lua_pushboolean(LS, true);
				return 1;
			}
		}

		lua_pushboolean(LS, false);
		return 1;
	}

	static int l_CheckForTravelOnFoot(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId && (*iter)->flags.fBetweenSectors && (*iter)->bAssignment != VEHICLE)
			{
				lua_pushboolean(LS, true);
				return 1;
			}
		}

		lua_pushboolean(LS, false);
		return 1;
	}

	static int l_CheckForTravelInHelicopter(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId && SoldierAboardAirborneHeli(*iter))
			{
				lua_pushboolean(LS, true);
				return 1;
			}
		}

		lua_pushboolean(LS, false);
		return 1;
	}

	static int l_CreateMilitia(lua_State* LS)
	{
		const int greenMilitia = lua_tointeger(LS, 1);
		const int regularMilitia = lua_tointeger(LS, 2);
		const int eliteMilitia = lua_tointeger(LS, 3);
		const INT16 sectorX = lua_tointeger(LS, 4);
		const INT16 sectorY = lua_tointeger(LS, 5);

		if (greenMilitia > 0)
		{
			StrategicAddMilitiaToSector(sectorX, sectorY, GREEN_MILITIA, greenMilitia);
			for (int i = 0; i < greenMilitia; ++i)
			{
				CreateNewIndividualMilitia( GREEN_MILITIA, MO_ARULCO, SECTOR(sectorX, sectorY) );
			}
		}

		if (regularMilitia > 0)
		{
			StrategicAddMilitiaToSector(sectorX, sectorY, REGULAR_MILITIA, regularMilitia);
			for (int i = 0; i < regularMilitia; ++i)
			{
				CreateNewIndividualMilitia( REGULAR_MILITIA, MO_ARULCO, SECTOR(sectorX, sectorY) );
			}
		}

		if (eliteMilitia > 0)
		{
			StrategicAddMilitiaToSector(sectorX, sectorY, ELITE_MILITIA, eliteMilitia);
			for (int i = 0; i < eliteMilitia; ++i)
			{
				CreateNewIndividualMilitia( ELITE_MILITIA, MO_ARULCO, SECTOR(sectorX, sectorY) );
			}
		}

		return 0;
	}

	static int l_GetCoordinates(lua_State* LS)
	{
		const UINT8 profileId = lua_tointeger(LS, 1);

		INT16 x = 0;
		INT16 y = 0;
		INT8 z = 0;
		for (UINT32 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
		{
			const SOLDIERTYPE* merc = MercPtrs[i];
			if (merc && merc->ubProfile == profileId)
			{
				x = merc->sSectorX;
				y = merc->sSectorY;
				z = merc->bSectorZ;
				break;
			}
		}

		// invalid profileid
		lua_pushinteger(LS, x);
		lua_pushinteger(LS, y);
		lua_pushinteger(LS, z);
		return 3;
	}

	static int l_GetHealth(lua_State* LS)
	{
		const UINT8 profileId = lua_tointeger(LS, 1);

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId)
			{
				lua_pushinteger(LS, (*iter)->stats.bLife);
				lua_pushinteger(LS, (*iter)->stats.bLifeMax);
				return 2;
			}
		}

		lua_pushinteger(LS, 0);
		lua_pushinteger(LS, 0);
		return 2;
	}

	static int l_GetHoursRemainingOnMiniEvent(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));

		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId)
			{
				lua_pushinteger(LS, (*iter)->ubHoursRemainingOnMiniEvent);
				return 1;
			}
		}

		lua_pushinteger(LS, -1);
		return 1;
	}

	static int l_GetSectorIDString(lua_State* LS)
	{
		const INT16 x = lua_tointeger(LS, 1);
		const INT16 y = lua_tointeger(LS, 2);
		const INT8 z = lua_tointeger(LS, 3);
		CHAR16 sectorName[512];
		GetSectorIDString(x, y, z, sectorName, FALSE);
		std::wstring wsSectorName(sectorName);
		std::string strSectorName(wsSectorName.begin(), wsSectorName.end());

		lua_pushstring(LS, strSectorName.c_str());
		return 1;
	}

	static int l_GetSkills(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));

		lua_newtable(LS);
		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if ((*iter)->ubProfile == profileId)
			{
				const INT8* skillTraits = gMercProfiles[(*iter)->ubProfile].bSkillTraits;
				for (int i = 0; i < sizeof(gMercProfiles[(*iter)->ubProfile].bSkillTraits) / sizeof(gMercProfiles[(*iter)->ubProfile].bSkillTraits[0]) ;++i)
				{
					const int skillId = *(skillTraits + i);
					if (skillId > 0)
					{
						lua_pushinteger(LS, i + 1);
						lua_pushinteger(LS, skillId);
						lua_settable(LS, -3);
					}
					else
					{
						break;
					}
				}
			}
		}
		
		return 1;
	}

	static int l_GetStat(lua_State* LS)
	{
		const UINT16 stat = lua_tointeger(LS, 1);
		INT16 sectorX = 0;
		INT16 sectorY = 0;
		INT8 sectorZ = 0;
		UINT8 profileId = 0;
		bool globalSearch = true;
		bool lookAtAllMercs = true;

		if (lua_gettop(LS) == 4)
		{
			globalSearch = false;
			sectorX = lua_tointeger(LS, 2);
			sectorY = lua_tointeger(LS, 3);
			sectorZ = lua_tointeger(LS, 4);
		}

		if (lua_gettop(LS) == 2)
		{
			lookAtAllMercs = false;
			profileId = static_cast<UINT8>(lua_tointeger(LS, 2));
		}

		INT8 bestStat = 0;
		SOLDIERTYPE* bestSoldier = gAllMercs[0];
		for (auto iter = gAllMercs.begin(); iter != gAllMercs.end(); ++iter)
		{
			if (lookAtAllMercs || ((*iter)->ubProfile == profileId))
			{
				if (globalSearch || ((*iter)->sSectorX == sectorX && (*iter)->sSectorY == sectorY && (*iter)->bSectorZ == sectorZ))
				{
					switch (stat)
					{
						case STAT_LIFE:
							if ((*iter)->stats.bLifeMax > bestStat)
							{
								bestStat = (*iter)->stats.bLifeMax;
								bestSoldier = *iter;
							}
							break;
						case STAT_STRENGTH:
							if ((*iter)->stats.bStrength > bestStat)
							{
								bestStat = (*iter)->stats.bStrength;
								bestSoldier = *iter;
							}
							break;
						case STAT_AGILITY:
							if ((*iter)->stats.bAgility > bestStat)
							{
								bestStat = (*iter)->stats.bAgility;
								bestSoldier = *iter;
							}
							break;
						case STAT_DEXTERITY:
							if ((*iter)->stats.bDexterity > bestStat)
							{
								bestStat = (*iter)->stats.bDexterity;
								bestSoldier = *iter;
							}
							break;
						case STAT_WISDOM:
							if ((*iter)->stats.bWisdom > bestStat)
							{
								bestStat = (*iter)->stats.bWisdom;
								bestSoldier = *iter;
							}
							break;
						case STAT_LEADERSHIP:
							if ((*iter)->stats.bLeadership > bestStat)
							{
								bestStat = (*iter)->stats.bLeadership;
								bestSoldier = *iter;
							}
							break;
						case STAT_MARKSMANSHIP:
							if ((*iter)->stats.bMarksmanship > bestStat)
							{
								bestStat = (*iter)->stats.bMarksmanship;
								bestSoldier = *iter;
							}
							break;
						case STAT_MECHANICAL:
							if ((*iter)->stats.bMechanical > bestStat)
							{
								bestStat = (*iter)->stats.bMechanical;
								bestSoldier = *iter;
							}
							break;
						case STAT_EXPLOSIVE:
							if ((*iter)->stats.bExplosive > bestStat)
							{
								bestStat = (*iter)->stats.bExplosive;
								bestSoldier = *iter;
							}
							break;
						case STAT_MEDICAL:
							if ((*iter)->stats.bMedical > bestStat)
							{
								bestStat = (*iter)->stats.bMedical;
								bestSoldier = *iter;
							}
							break;
						case STAT_EXPLEVEL:
							if ((*iter)->stats.bExpLevel > bestStat)
							{
								bestStat = (*iter)->stats.bExpLevel;
								bestSoldier = *iter;
							}
							break;
					}
				}
			}
		}

		CHAR8 nickname[50];
		sprintf(nickname, "%ls", gMercProfiles[bestSoldier->ubProfile].zNickname);
		lua_pushinteger(LS, bestStat);
		lua_pushstring(LS, nickname);
		lua_pushinteger(LS, bestSoldier->ubProfile);
		return 3;
	}

	static int l_GetProgress(lua_State* LS)
	{
		lua_pushinteger(LS, HighestPlayerProgressPercentage());
		return 1;
	}

	static int l_GetTownId(lua_State* LS)
	{
		const INT16 x = lua_tointeger(LS, 1);
		const INT16 y = lua_tointeger(LS, 2);

		const UINT8 townId = GetTownIdForSector(x, y);
		CHAR8 townName[MAX_TOWN_NAME_LENGHT];
		sprintf( townName, "%ls", pTownNames[townId] );

		lua_pushinteger(LS, townId);
		lua_pushstring( LS, townName );
		return 2;
	}

	static int l_SendMercOnMiniEvent(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));
		const UINT16 hoursOnMiniEvent = static_cast<UINT16>(lua_tointeger(LS, 2));

		if (hoursOnMiniEvent == 0)
			return 0;

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [profileId, hoursOnMiniEvent](SOLDIERTYPE* merc) {
			if (merc->ubProfile != profileId)
				return;

			TakeSoldierOutOfVehicle(merc);
			RemoveCharacterFromSquads(merc);
			merc->ubHoursRemainingOnMiniEvent = hoursOnMiniEvent;
			merc->bSectorZ += MINI_EVENT_Z_OFFSET;
			merc->bBleeding = 0;
			SetTimeOfAssignmentChangeForMerc(merc);
			ChangeSoldiersAssignment(merc, ASSIGNMENT_MINIEVENT);
			
			// see HandleMiniEventAssignments() in Assignments.cpp for returning merc to normal
		});

		return 0;
	}

	static int l_SetEnemyGroupVisibility(lua_State* LS)
	{
		const bool visible = lua_toboolean(LS, 1);
		INT16 x = 0;
		INT16 y = 0;

		if (lua_gettop(LS) == 3)
		{
			x = lua_tointeger(LS, 2);
			y = lua_tointeger(LS, 3);
		}

		if (x == 0 && y == 0)
		{
			for ( INT16 sX = 1; sX < (MAP_WORLD_X - 1); ++sX )
			{
				for ( INT16 sY = 1; sY < (MAP_WORLD_Y - 1); ++sY )
				{
					if (visible)
					{
						SectorInfo[SECTOR(sX, sY)].ubDetectionLevel |= (1 << 2);
					}
					else
					{
						SectorInfo[SECTOR(sX, sY)].ubDetectionLevel &= ~(1 << 2);
					}
				}
			}
		}
		else
		{
			if (visible)
			{
				SectorInfo[SECTOR(x,y)].ubDetectionLevel |= (1 << 2);
			}
			else
			{
				SectorInfo[SECTOR(x,y)].ubDetectionLevel &= ~(1 << 2);
			}
		}

		fMapPanelDirty = true;

		return 0;
	}

	static int l_SetMercCoordinates(lua_State* LS)
	{
		const UINT8 profileId = static_cast<UINT8>(lua_tointeger(LS, 1));
		const INT16 sectorX = lua_tointeger(LS, 2);
		const INT16 sectorY = lua_tointeger(LS, 3);
		const INT8 sectorZ = lua_tointeger(LS, 4);

		std::for_each(gAllMercs.begin(), gAllMercs.end(), [profileId, sectorX, sectorY, sectorZ](SOLDIERTYPE* merc) {
			if (merc->ubProfile != profileId)
				return;

			merc->sSectorX = sectorX;
			merc->sSectorY = sectorY;
			merc->bSectorZ = sectorZ;

			TakeSoldierOutOfVehicle(merc);
			RemoveCharacterFromSquads(merc);
			merc->ubInsertionDirection = DIRECTION_IRRELEVANT;
			merc->ubStrategicInsertionCode = INSERTION_CODE_CENTER;
			AddCharacterToAnySquad(merc);
			});

		return 0;
	}
}

void InitMiniEvents()
{
	// if a user really wants to reschedule an event by disabling this flag, saving their game, re-enabling the flag, then loading their game, then fine. it's a single player game, be my guest
	if (gGameExternalOptions.fMiniEventsEnabled == false || is_networked)
	{
		DeleteAllStrategicEventsOfType(EVENT_MINIEVENT);
	}
	else
	{
		if (GetAllStrategicEventsOfType(EVENT_MINIEVENT).size() == 0)
			QueueNextMiniEvent(0, 0);

		const char* filename = "scripts\\MiniEvents.lua";

		if (gLS() != nullptr)
		{
			// clear old lua state (on load or new game)
			LuaState::CLOSE(gLS);
		}
		gLS = LuaState::INIT(true);

		SGP_THROW_IFFALSE( gLS.EvalFile(filename), _BS("Cannot open file: ") << filename << _BS::cget );

		lua_register(gLS(), "CScreenMsg", MiniEventsLua_ScreenMsg);
		lua_register(gLS(), "CMsgBox", MiniEventsLua_MessageBox);
		lua_register(gLS(), "CResolveEvent", MiniEventsLua_ResolveEvent);

		using namespace MiniEventHelpers;
		{
			lua_register(gLS(), "CAddIntel", l_AddIntel);
			lua_register(gLS(), "CAddMoneyToPlayerAccount", l_AddMoneyToPlayerAccount);
			lua_register(gLS(), "CAddTownLoyalty", l_AddTownLoyalty);
			lua_register(gLS(), "CAddSkill", l_AddSkill);
			lua_register(gLS(), "CAdjustBreathMax", l_AdjustBreathMax);
			lua_register(gLS(), "CAdjustMorale", l_AdjustMorale);
			lua_register(gLS(), "CAdjustEnemyStrengthInSector", l_AdjustEnemyStrengthInSector);
			lua_register(gLS(), "CAdjustStat", l_AdjustStat);
			lua_register(gLS(), "CAdjustVehicleFuel", l_AdjustVehicleFuel);
			lua_register(gLS(), "CAdjustVehicleHealth", l_AdjustVehicleHealth);
			lua_register(gLS(), "CApplyDamage", l_ApplyDamage);
			lua_register(gLS(), "CApplyPermanentStatDamage", l_ApplyPermanentStatDamage);
			lua_register(gLS(), "CCheckForAssignment", l_CheckForAssignment);
			lua_register(gLS(), "CCheckForSkill", l_CheckForSkill);
			lua_register(gLS(), "CCheckForSleep", l_CheckForSleep);
			lua_register(gLS(), "CCheckForTravel", l_CheckForTravel);
			lua_register(gLS(), "CCheckForTravelOnFoot", l_CheckForTravelOnFoot);
			lua_register(gLS(), "CCheckForTravelInHelicopter", l_CheckForTravelInHelicopter);
			lua_register(gLS(), "CCreateMilitia", l_CreateMilitia);
			lua_register(gLS(), "CGetCoordinates", l_GetCoordinates);
			lua_register(gLS(), "CGetHealth", l_GetHealth);
			lua_register(gLS(), "CGetHoursRemainingOnMiniEvent", l_GetHoursRemainingOnMiniEvent);
			lua_register(gLS(), "CGetSectorIDString", l_GetSectorIDString);
			lua_register(gLS(), "CGetSkills", l_GetSkills);
			lua_register(gLS(), "CGetStat", l_GetStat);
			lua_register(gLS(), "CGetProgress", l_GetProgress);
			lua_register(gLS(), "CGetTownId", l_GetTownId);
			lua_register(gLS(), "CSendMercOnMiniEvent", l_SendMercOnMiniEvent);
			lua_register(gLS(), "CSetEnemyGroupVisibility", l_SetEnemyGroupVisibility);
			lua_register(gLS(), "CSetMercCoordinates", l_SetMercCoordinates);
		}
	}
}

void CheckMiniEvents(UINT32 nextEventId)
{
	if (gGameExternalOptions.fMiniEventsEnabled == false || is_networked)
		return;

	// no events if we're in combat or a hostile sector
	if ((gTacticalStatus.uiFlags & INCOMBAT) || gTacticalStatus.fEnemyInSector)
		return;

	StopTimeCompression();
	MiniEventsLua(nextEventId);
}

static void QueueNextMiniEvent(UINT32 nextEventId, UINT32 hoursToNextMiniEvent)
{
	const UINT32 timestamp = GetWorldTotalMin() + gGameExternalOptions.fMiniEventsMinHoursBetweenEvents * 60 + Random((gGameExternalOptions.fMiniEventsMaxHoursBetweenEvents - gGameExternalOptions.fMiniEventsMinHoursBetweenEvents) * 60);
	AddStrategicEvent(EVENT_MINIEVENT, hoursToNextMiniEvent > 0 ? GetWorldTotalMin() + 60 * hoursToNextMiniEvent : timestamp, nextEventId);
}

// LUA STUFF FOLLOWS

void MiniEventsLua(UINT32 eventId)
{
	gAllMercs.clear();

	// get all mercs eligible to get a mini event
	for (UINT32 cnt = gTacticalStatus.Team[OUR_TEAM].bFirstID; cnt <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++cnt)
	{
		SOLDIERTYPE* pSoldier = MercPtrs[ cnt ];

		if (pSoldier && pSoldier->bActive
			&& pSoldier->stats.bLife > 0
			&& pSoldier->bAssignment != IN_TRANSIT
			&& pSoldier->bAssignment != ASSIGNMENT_POW
			&& pSoldier->bAssignment != ASSIGNMENT_REBELCOMMAND
			&& !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
		{
			gAllMercs.push_back(pSoldier);
		}
	}

	if (eventId > 0)
	{
		LuaFunction f = LuaFunction(gLS, "BeginSpecificEvent");
		// first param: the event to trigger
		f.Param<int>(eventId);

		// second param: a table containing basic info about all of the player's mercs ({ nickname = profileid })
		f.TableOpen();
		for (UINT32 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
		{
			const SOLDIERTYPE* merc = MercPtrs[i];
			if (merc && merc->bActive && merc->bAssignment != IN_TRANSIT && !(merc->flags.uiStatusFlags & SOLDIER_VEHICLE) && !(AM_A_ROBOT(merc)))
			{
				std::wstring ws(gMercProfiles[merc->ubProfile].zNickname);
				std::string str(ws.begin(), ws.end());
				f.TParam(str.c_str(), (int)merc->ubProfile);
			}
		}
		f.TableClose();

		SGP_THROW_IFFALSE(f.Call(2), "call to lua function BeginSpecificEvent failed");
	}
	else
	{
		LuaFunction f = LuaFunction(gLS, "BeginRandomEvent");
		// first param: a table containing basic info about all of the player's mercs ({ nickname = profileid })
		f.TableOpen();
		for (UINT32 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; ++i)
		{
			const SOLDIERTYPE* merc = MercPtrs[i];
			if (merc && merc->bActive && merc->bAssignment != IN_TRANSIT && !(merc->flags.uiStatusFlags & SOLDIER_VEHICLE) && !(AM_A_ROBOT(merc)))
			{
				std::wstring ws(gMercProfiles[merc->ubProfile].zNickname);
				std::string str(ws.begin(), ws.end());
				f.TParam(str.c_str(), (int)merc->ubProfile);
			}
		}
		f.TableClose();

		SGP_THROW_IFFALSE(f.Call(1), "call to lua function BeginRandomEvent failed");
	}
}

static int MiniEventsLua_MessageBox(lua_State* LS)
{
	size_t len = 0;
	CHAR16 w_str[500];

	std::string str = lua_tolstring(LS, 1, &len);
	MultiByteToWideChar( CP_UTF8, 0, len > MAX_BUTTON_LENGTH ? str.substr(0, MAX_BUTTON_LENGTH).c_str() : str.c_str(), -1, w_str, sizeof(w_str) / sizeof(w_str[0]) );
	w_str[sizeof(w_str) / sizeof(w_str[0]) - 1] = '\0';
	wcscpy( gzUserDefinedButton1, w_str );

	str = lua_tolstring(LS, 2, &len);
	MultiByteToWideChar( CP_UTF8, 0, len > MAX_BUTTON_LENGTH ? str.substr(0, MAX_BUTTON_LENGTH).c_str() : str.c_str(), -1, w_str, sizeof(w_str) / sizeof(w_str[0]) );
	w_str[sizeof(w_str) / sizeof(w_str[0]) - 1] = '\0';
	wcscpy( gzUserDefinedButton2, w_str );

	str = lua_tolstring(LS, 3, &len);
	MultiByteToWideChar( CP_UTF8, 0, len > MAX_BODY_LENGTH ? str.substr(0, MAX_BODY_LENGTH).c_str() : str.c_str(), -1, w_str, sizeof(w_str) / sizeof(w_str[0]) );
	w_str[sizeof(w_str) / sizeof(w_str[0]) - 1] = '\0';

	// we need to cache the screen here so that the second msgbox doesn't keep the global screen state in MSG_BOX_SCREEN (causes infinite recursion)
	guiMiniEventsCachedScreen = guiCurrentScreen;
	DoMessageBox(MSG_BOX_MINIEVENT_STYLE, w_str,
		guiCurrentScreen, MSG_BOX_FLAG_WIDE_BUTTONS | MSG_BOX_FLAG_BIGGER, MiniEventsLua_MessageBoxCallback , NULL);

	return 0;
}

static void MiniEventsLua_MessageBoxCallback(UINT8 ubExitValue)
{
	// appears to be first button = 2, second button = 3
	gfDontOverRideSaveBuffer = FALSE;
	LuaFunction(gLS, "ResolveMsgBox" ).Param<bool>(ubExitValue == 2).Call(1);
}

static int MiniEventsLua_ResolveEvent(lua_State* LS)
{
	size_t len = 0;
	std::string str = lua_tolstring(LS, 1, &len);

	UINT32 nextEventId = 0;
	if (lua_gettop(LS) >= 2)
		nextEventId = static_cast<UINT32>(lua_tointeger(LS, 2));

	UINT32 hoursToNextMiniEvent = 0;
	if (lua_gettop(LS) == 3)
		hoursToNextMiniEvent = static_cast<UINT32>(lua_tointeger(LS, 3));

	CHAR16 w_str[500];
	MultiByteToWideChar( CP_UTF8, 0, len > MAX_BODY_LENGTH ? str.substr(0, MAX_BODY_LENGTH).c_str() : str.c_str(), -1, w_str, sizeof(w_str) / sizeof(w_str[0]) );
	w_str[sizeof(w_str) / sizeof(w_str[0]) - 1] = '\0';

	MSYS_RemoveRegion(&(gMsgBox.BackRegion));
	DoMessageBox(MSG_BOX_MINIEVENT_STYLE, w_str,
		guiMiniEventsCachedScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_BIGGER, [](UINT8 ubExitValue) { gfDontOverRideSaveBuffer = FALSE; }, NULL);

	QueueNextMiniEvent(nextEventId, hoursToNextMiniEvent);

	return 0;
}

static int MiniEventsLua_ScreenMsg(lua_State* LS)
{
	size_t len = 0;
	std::string str = lua_tolstring(LS, 1, &len);

	CHAR16 w_str[250];
	MultiByteToWideChar( CP_UTF8, 0, len > 250 ? str.substr(0, 250).c_str() : str.c_str(), -1, w_str, sizeof(w_str) / sizeof(w_str[0]) );
	w_str[sizeof(w_str) / sizeof(w_str[0]) - 1] = '\0';

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s", w_str );
	return 0;
}

