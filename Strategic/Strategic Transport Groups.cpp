//#pragma optimize("",off)
/*
Strategic Transport Groups
by rftr

Strategic transport groups are a type of enemy strategic group in addition to ATTACK and PATROL groups. The primary purpose
of transport groups is to function as a loot pinata for the player. However, they will generally be behind enemy lines, so
the player will have to seek them out.

Behaviourally, transport groups will spawn at the AI's HQ and then travel to a friendly town. Once the group reaches its
destination, it will wait for a few hours before returning home, where it despawns. The AI will receive small bonuses
upon the group successfully reaching both its destination and HQ.

A transport group always flags its members to drop everything they have regardless of the player's DROP_ALL setting. In
addition, transport groups will also be carrying supplies that the player may find useful.

Transport group compositions will vary based on the player's progress, how many interceptions have been completed recently,
and the difficulty of the game.

*/
#include "Strategic Transport Groups.h"

#include "ASD.h"
#include "Assignments.h"
#include "Campaign.h"
#include "Game Clock.h"
#include "Game Event Hook.h"
#include "GameSettings.h"
#include "Inventory Choosing.h"
#include "Map Screen Interface Map.h"
#include "message.h"
#include "Overhead.h"
#include "Overhead Types.h"
#include "Queen Command.h"
#include "random.h"
#include "Soldier Control.h"
#include "strategic.h"
#include "Strategic AI.h"
#include "strategicmap.h"
#include "Strategic Mines.h"
#include "Strategic Movement.h"
#include "Strategic Town Loyalty.h"

#define TRANSPORT_GROUP_DEBUG(x, ...) if (gGameExternalOptions.fStrategicTransportGroupsDebug) {ScreenMsg(FONT_RED, MSG_INTERFACE, x, __VA_ARGS__);}

// how many turncoats are required to monitor a town for transport groups?
#define ELITE_TURNCOAT_MONITOR_REQUIREMENT 1
#define TROOP_TURNCOAT_MONITOR_REQUIREMENT 3
#define ADMIN_TURNCOAT_MONITOR_REQUIREMENT 5

extern ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[SOLDIER_GUN_CHOICE_SELECTIONS][ARMY_GUN_LEVELS];
extern ARMY_GUN_CHOICE_TYPE gArmyItemChoices[SOLDIER_GUN_CHOICE_SELECTIONS][MAX_ITEM_TYPES];
extern BOOLEAN gfTownUsesLoyalty[MAX_TOWNS];

std::map<UINT8, std::map<int, UINT8>> transportGroupIdToSoldierMap;
std::map<UINT8, TransportGroupSectorInfo> transportGroupSectorInfo;

void PopulateTransportGroup(UINT8& admins, UINT8& troops, UINT8& elites, UINT8& jeeps, UINT8& tanks, UINT8& robots, UINT8 progress, int difficulty, BOOLEAN trySpecialCase);

BOOLEAN DeployTransportGroup()
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
		return FALSE;

	if (giReinforcementPool <= 0)
		return FALSE;

	const UINT8 difficulty = gGameOptions.ubDifficultyLevel;

	// is there a mine here?
	std::vector<INT16> mineSectorIds;
	for (INT8 i = 0; i < NUM_TOWNS; ++i)
	{
		// skip towns that have no loyalty
		if (!gfTownUsesLoyalty[i]) continue;

		// filter by TOWN ownership - skip contested towns on expert/insane
		if (IsTownUnderCompleteControlByPlayer(i)) continue;
		if ((difficulty == DIF_LEVEL_HARD || difficulty == DIF_LEVEL_INSANE) && IsTownUnderCompleteControlByEnemy(i) == FALSE) continue;

		// skip towns with a shut down mine
		const INT8 mineIndex = GetMineIndexForTown(i);
		if (mineIndex == -1) continue;
		if (IsMineShutDown(mineIndex) == TRUE) continue;

		// filter by MINE ownership - for novice/experienced, as hard/insane would have ignored this town above
		const INT16 mineSector = (GetMineSectorForTown(i));
		if (StrategicMap[mineSector].fEnemyControlled == FALSE) continue;

		mineSectorIds.push_back(STRATEGIC_INDEX_TO_SECTOR_INFO(mineSector));
	}

	for (int a = 0; a < mineSectorIds.size(); ++a)
	{
		TRANSPORT_GROUP_DEBUG(L"DeployTransportGroup valid town destination: %d (%d/%d)", mineSectorIds[a], SECTORX(mineSectorIds[a]), SECTORY(mineSectorIds[a]));
	}

	// no valid destinations
	if (mineSectorIds.size() == 0) return FALSE;

	INT8 transportGroupCount = 0;
	GROUP* pGroup = gpGroupList;
	std::vector<std::tuple<UINT8, UINT8, UINT8>> groupIds;
	while (pGroup)
	{
		if (pGroup->usGroupTeam == ENEMY_TEAM && pGroup->pEnemyGroup->ubIntention == TRANSPORT)
		{
			groupIds.emplace_back(pGroup->ubGroupID, pGroup->ubSectorX, pGroup->ubSectorY);
			transportGroupCount++;
		}
		pGroup = pGroup->next;
	}

	for (int a = 0; a < groupIds.size(); ++a)
	{
		TRANSPORT_GROUP_DEBUG(L"DeployTransportGroup found existing transport groupid: %d at %d/%d", std::get<0>(groupIds[a]), std::get<1>(groupIds[a]), std::get<2>(groupIds[a]));
	}

	// track recent transport group interceptions
	const INT8 recentLossCount = min(5, GetAllStrategicEventsOfType(EVENT_TRANSPORT_GROUP_DEFEATED).size());

	// if there are too many active transport groups, don't deploy any more
	// maximum number of active groups is the number of valid destinations at queen decision time
	const INT8 maxGroups = gGameExternalOptions.iMaxSimultaneousTransportGroups - recentLossCount;
	if (transportGroupCount >= min(max(maxGroups, 0), mineSectorIds.size())) return FALSE;

	const UINT8 ubSectorID = (UINT8)mineSectorIds[Random(mineSectorIds.size())];

	TRANSPORT_GROUP_DEBUG(L"DeployTransportGroup sending group to sectorId: %d (%d/%d)", ubSectorID, SECTORX(ubSectorID), SECTORY(ubSectorID));

	UINT8 admins, troops, elites, robots, jeeps, tanks;
	const UINT8 progress = min(125, HighestPlayerProgressPercentage() + recentLossCount * 5);
	PopulateTransportGroup(admins, troops, elites, jeeps, tanks, robots, progress, difficulty, mineSectorIds.size() == 1);

	// varying transport group quality/compositions
	pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), admins, troops, elites, robots, tanks, jeeps );

	//Madd: unlimited reinforcements?
	if ( !gfUnlimitedTroops )
	{
		giReinforcementPool -= (admins + troops + elites + robots + jeeps + tanks);

		giReinforcementPool = max( giReinforcementPool, 0 );
	}

	MoveSAIGroupToSector( &pGroup, ubSectorID, EVASIVE, TRANSPORT );

	pGroup->uiFlags |= GROUPFLAG_TRANSPORT_ENROUTE;

	return TRUE;
}

BOOLEAN ForceDeployTransportGroup(UINT8 sectorId)
{
	UINT8 admins, troops, elites, robots, jeeps, tanks;
	const INT8 recentLossCount = min(5, GetAllStrategicEventsOfType(EVENT_TRANSPORT_GROUP_DEFEATED).size());
	const UINT8 progress = min(125, HighestPlayerProgressPercentage() + recentLossCount * 5);
	const UINT8 difficulty = gGameOptions.ubDifficultyLevel;
	PopulateTransportGroup(admins, troops, elites, jeeps, tanks, robots, progress, difficulty, FALSE);
	return TRUE;
}

BOOLEAN ReturnTransportGroup(INT32 groupId)
{
	GROUP* pGroup = gpGroupList;
	while (pGroup)
	{
		if (pGroup->ubGroupID == groupId)
		{
			MoveSAIGroupToSector( &pGroup, SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), EVASIVE, TRANSPORT );
			if (pGroup)
				pGroup->uiFlags &= ~GROUPFLAG_TRANSPORT_ENROUTE;
			break;
		}
		pGroup = pGroup->next;
	}

	if (pGroup == nullptr)
	{
		TRANSPORT_GROUP_DEBUG(L"RETURN_TRANSPORT_GROUP failed to find groupid %d", groupId);
		return FALSE;
	}

	return TRUE;
}

void FillMapColoursForTransportGroups(INT32(&colorMap)[MAXIMUM_VALID_Y_COORDINATE][MAXIMUM_VALID_X_COORDINATE])
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
		return;

	const auto debugColor = MAP_SHADE_LT_BLUE;
	const auto targetColor = MAP_SHADE_LT_YELLOW;
	const INT8 DETECTION_RANGE_SCOUT = 1;
	const INT8 DETECTION_RANGE_RADIO = 3;
	const INT8 DETECTION_RANGE_COVERT = 0;
	GROUP* pGroup = gpGroupList;
	transportGroupSectorInfo.clear();

	enum class MonitoredSectorState {
		Unmonitored,
		Monitored,
		GroupIncoming,
	} monitoredSectorState;

	// build map of detection sectors + ranges
	std::map<std::pair<INT16,INT16>, INT8> detectionMap;
	std::map<UINT8, MonitoredSectorState> monitoredTowns;
	for( INT16 i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive &&
			MercPtrs[ i ]->stats.bLife >= OKLIFE &&
			(MercPtrs[ i ]->bAssignment < ON_DUTY || MercPtrs[ i ]->bAssignment == GATHERINTEL) &&
			!MercPtrs[ i ]->flags.fMercAsleep)
		{
			if (gGameOptions.fNewTraitSystem)
			{
				if (HAS_SKILL_TRAIT(MercPtrs[i], SCOUTING_NT))
				{
					detectionMap[std::pair<INT16,INT16>(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = DETECTION_RANGE_SCOUT;
				}
				else if (HAS_SKILL_TRAIT(MercPtrs[i], RADIO_OPERATOR_NT) && MercPtrs[i]->CanUseRadio(FALSE))
				{
					detectionMap[std::pair<INT16,INT16>(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = DETECTION_RANGE_RADIO;
				}
				else if (HAS_SKILL_TRAIT(MercPtrs[i], COVERT_NT))
				{
					if (MercPtrs[i]->bAssignment == GATHERINTEL)
					{
						detectionMap[std::pair<INT16,INT16>(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = DETECTION_RANGE_COVERT;
						monitoredTowns[GetTownIdForSector(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = MonitoredSectorState::Monitored;
					}
				}
			}
		}
	}

	// turncoats in towns can detect incoming transport groups
	for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
	{
		for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
		{
			const UINT8 townId = GetTownIdForSector(x, y);
			if (townId == BLANK_SECTOR) continue;

			CorrectTurncoatCount(x, y);
			const UINT16 adminTurncoats = NumTurncoatsOfClassInSector(x, y, SOLDIER_CLASS_ADMINISTRATOR);
			const UINT16 troopTurncoats = NumTurncoatsOfClassInSector(x, y, SOLDIER_CLASS_ARMY);
			const UINT16 eliteTurncoats = NumTurncoatsOfClassInSector(x, y, SOLDIER_CLASS_ELITE);

			if (gGameExternalOptions.fStrategicTransportGroupsDebug
				|| (adminTurncoats >= ADMIN_TURNCOAT_MONITOR_REQUIREMENT)
				|| (troopTurncoats >= TROOP_TURNCOAT_MONITOR_REQUIREMENT)
				|| (eliteTurncoats >= ELITE_TURNCOAT_MONITOR_REQUIREMENT)) monitoredTowns[townId] = MonitoredSectorState::Monitored;
		}
	}

	// colour all groups
	while (pGroup)
	{
		if (pGroup->usGroupTeam == ENEMY_TEAM)
		{
			const UINT8 intention = pGroup->pEnemyGroup->ubIntention;
			if (intention == TRANSPORT )
			{
				// check if current location is known
				const INT16 gx = pGroup->ubSectorX;
				const INT16 gy = pGroup->ubSectorY;
				for (const auto key : detectionMap)
				{
					const std::pair<INT16, INT16> sector = key.first;
					const INT8 range = key.second;

					const INT8 dist = abs((gx - sector.first)) + abs((gy - sector.second));
					if (dist <= range)
					{
						colorMap[pGroup->ubSectorY-1][pGroup->ubSectorX-1] = targetColor;
						transportGroupSectorInfo[SECTOR(pGroup->ubSectorX, pGroup->ubSectorY)] = TransportGroupSectorInfo::TransportGroupSectorInfo_LocatedGroup;
					}
				}

				// turncoats reveal their group's location
				if (pGroup->pEnemyGroup->ubNumAdmins_Turncoat >= ADMIN_TURNCOAT_MONITOR_REQUIREMENT
					|| pGroup->pEnemyGroup->ubNumTroops_Turncoat >= TROOP_TURNCOAT_MONITOR_REQUIREMENT
					|| pGroup->pEnemyGroup->ubNumElites_Turncoat >= ELITE_TURNCOAT_MONITOR_REQUIREMENT)
				{
					colorMap[pGroup->ubSectorY-1][pGroup->ubSectorX-1] = targetColor;
					transportGroupSectorInfo[SECTOR(pGroup->ubSectorX, pGroup->ubSectorY)] = TransportGroupSectorInfo::TransportGroupSectorInfo_LocatedGroup;
				}

				// check if target location is monitored 
				WAYPOINT* wp = pGroup->pWaypoints;

				while (wp)
				{
					if (wp->next == nullptr)
						break;

					wp = wp->next;
				}

				if (wp == nullptr)
				{
					// ignore this group - it doesn't have a waypoint (?)
					continue;
				}

				const UINT8 townId = GetTownIdForSector(wp->x, wp->y);
				if (monitoredTowns.find(townId) != monitoredTowns.end() && monitoredTowns[townId] == MonitoredSectorState::Monitored)
				{
					monitoredTowns[townId] = MonitoredSectorState::GroupIncoming;
				}

				// debug: colour all group locations
				if (gGameExternalOptions.fStrategicTransportGroupsDebug)
				{
					colorMap[pGroup->ubSectorY-1][pGroup->ubSectorX-1] = debugColor;
				}
			}
		}

		pGroup = pGroup->next;
	}

	// color all monitored towns if there's a transport group en route
	for (int x = MINIMUM_VALID_X_COORDINATE; x <= MAXIMUM_VALID_X_COORDINATE; ++x)
	{
		for (int y = MINIMUM_VALID_Y_COORDINATE; y <= MAXIMUM_VALID_Y_COORDINATE; ++y)
		{
			const UINT8 townId = GetTownIdForSector(x, y);
			if (monitoredTowns.find(townId) != monitoredTowns.end() && monitoredTowns[townId] == MonitoredSectorState::GroupIncoming)
			{
				colorMap[y-1][x-1] = targetColor;
				transportGroupSectorInfo[SECTOR(x, y)] = TransportGroupSectorInfo::TransportGroupSectorInfo_LocatedDestination;
			}
		}
	}

}

void ProcessTransportGroupReachedDestination(GROUP* pGroup)
{
	const UINT8 difficulty = gGameOptions.ubDifficultyLevel;

	// just arrived, let's go home
	if (pGroup->ubSectorX != gModSettings.ubSAISpawnSectorX && pGroup->ubSectorY != gModSettings.ubSAISpawnSectorY)
	{
		pGroup->ubSectorIDOfLastReassignment = (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY );

		// global loyalty loss
		INT32 loyaltyLoss = 0;
		switch (difficulty)
		{
		case DIF_LEVEL_EASY:	loyaltyLoss = 0; break;
		case DIF_LEVEL_MEDIUM:	loyaltyLoss = 0; break;
		case DIF_LEVEL_HARD:	loyaltyLoss = -100; break;
		case DIF_LEVEL_INSANE:	loyaltyLoss = -250; break;
		}

		AffectAllTownsLoyaltyByDistanceFrom(loyaltyLoss, pGroup->ubSectorX, pGroup->ubSectorY, pGroup->ubSectorZ);

		// queue up return home order
		AddStrategicEvent(EVENT_RETURN_TRANSPORT_GROUP, GetWorldTotalMin() + 60 * 6, pGroup->ubGroupID);
	}
	else
	{
		// asd income injection and bonus update
		if (gGameExternalOptions.fASDActive)
		{
			INT32 moneyAmt = 0;
			INT32 fuelAmt = 0;

			switch (difficulty)
			{
			case DIF_LEVEL_EASY:
				moneyAmt = 0;
				fuelAmt = 0;
				break;

			case DIF_LEVEL_MEDIUM:
				moneyAmt = gGameExternalOptions.gASDResource_Cost[ASD_JEEP] * 0.25f;
				fuelAmt = gGameExternalOptions.gASDResource_Fuel_Jeep * 0.25f;
				break;

			case DIF_LEVEL_HARD:
				moneyAmt = gGameExternalOptions.gASDResource_Cost[ASD_JEEP] * 0.5f;
				fuelAmt = gGameExternalOptions.gASDResource_Fuel_Jeep * 0.5f;
				break;

			case DIF_LEVEL_INSANE:
				moneyAmt = gGameExternalOptions.gASDResource_Cost[ASD_TANK];
				fuelAmt = gGameExternalOptions.gASDResource_Fuel_Tank;
				break;
			}

			AddStrategicAIResources(ASD_MONEY, moneyAmt);
			AddStrategicAIResources(ASD_FUEL, fuelAmt);
			UpdateASD();
		}

		// reinforcement pool increase
		if (!gfUnlimitedTroops)
		{
			INT32 poolAmt = 0;
			switch (difficulty)
			{
			case DIF_LEVEL_EASY:	poolAmt = 0; break;
			case DIF_LEVEL_MEDIUM:	poolAmt = 10; break;
			case DIF_LEVEL_HARD:	poolAmt = 15; break;
			case DIF_LEVEL_INSANE:	poolAmt = 40; break;
			}

			giReinforcementPool += poolAmt;
		}

		// successfully returned home. give the strategic ai some rewards!
		SendGroupToPool(&pGroup);

		if (difficulty != DIF_LEVEL_EASY)
		{
			// immediately do a queen evaluation
			DeleteAllStrategicEventsOfType(EVENT_EVALUATE_QUEEN_SITUATION);
			EvaluateQueenSituation();
		}
	}
}

void UpdateTransportGroupInventory()
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
		return;

	const int firstSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
	const int lastSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bLastID;
	const UINT8 progress = CurrentPlayerProgressPercentage();

	enum ItemTypes
	{
		GAS_CANS,
		MEDICAL_FIRSTAIDKITS,
		MEDICAL_MEDKITS,
		MEDICAL_OTHER,
		TOOL_KITS,
		BACKPACKS,
		RADIOS,
		ATTACHMENTS,
		CAMO_KITS,
		MISC,
		GRENADE_THROWN,
		GUNS,
		AMMO_BOXES,
		GRENADELAUNCHERS,
		ROCKETLAUNCHERS,


		TRANSPORT_LOOT_START = GAS_CANS,
		TRANSPORT_LOOT_END = ROCKETLAUNCHERS,
	};

	std::map<ItemTypes, std::vector<UINT16>> itemMap;

	// item cache build
	{
		// let's be nice to the player and only drop ammo for guns their mercs have in inventory
		std::set<UINT8> playerCalibres;
		for (INT16 i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; i++)
		{
			if (MercPtrs[i]->bActive && !(MercPtrs[i]->flags.uiStatusFlags & SOLDIER_VEHICLE))
			{
				for (int j = 0 ; j < MercPtrs[i]->inv.size(); ++j)
				{
					OBJECTTYPE& obj = MercPtrs[i]->inv[j];
					if (obj.exists()
					&& Item[obj.usItem].usItemClass == IC_GUN)
					{
						playerCalibres.insert(Weapon[Item[obj.usItem].ubClassIndex].ubCalibre);
					}
				}
			}
		}

		for (UINT16 i = 0; i < gMAXITEMS_READ; ++i)
		{
			if (!ItemIsLegal(i, TRUE)) continue;
			if ((Item[i].usItemClass & IC_AMMO) == 0 && (Item[i].iTransportGroupMaxProgress == 0 || Item[i].iTransportGroupMinProgress > progress || progress > Item[i].iTransportGroupMaxProgress)) continue;

			if (Item[i].medical)
			{
				if (Item[i].firstaidkit) itemMap[MEDICAL_FIRSTAIDKITS].push_back(i);
				else if (Item[i].medicalkit) itemMap[MEDICAL_MEDKITS].push_back(i);
				else itemMap[MEDICAL_OTHER].push_back(i);
			}
			else if (Item[i].gascan) itemMap[GAS_CANS].push_back(i);
			else if (Item[i].toolkit) itemMap[TOOL_KITS].push_back(i);
			else if (HasItemFlag(i, RADIO_SET)) itemMap[RADIOS].push_back(i);
			else if (Item[i].usItemClass & IC_GRENADE)
			{
				if (Item[i].glgrenade == 0
				&& Item[i].attachmentclass != AC_GRENADE // not for a grenade launcher
				&& Item[i].attachmentclass != AC_ROCKET) // not for a rocket launcher
					itemMap[GRENADE_THROWN].push_back(i);
			}
			else if (Item[i].usItemClass & IC_LBEGEAR)
			{
				if (LoadBearingEquipment[Item[i].ubClassIndex].lbeClass == BACKPACK && !HasItemFlag(i, RADIO_SET)) // make sure radios don't get added here
				{
					itemMap[BACKPACKS].push_back(i);
				}
			}
			else if (Item[i].camouflagekit) itemMap[CAMO_KITS].push_back(i);
			else if (Item[i].usItemClass & IC_MISC)
			{
				switch (Item[i].attachmentclass)
				{
				case AC_BIPOD:
				case AC_MUZZLE:
				case AC_LASER:
				case AC_SIGHT:
				case AC_SCOPE:
				case AC_FOREGRIP:
					itemMap[ATTACHMENTS].push_back(i);
					break;
				default:
					itemMap[MISC].push_back(i);
					break;
				}
			}
			else if (Item[i].usItemClass & IC_AMMO)
			{
				if (Magazine[Item[i].ubClassIndex].ubMagType == AMMO_BOX && playerCalibres.find(Magazine[Item[i].ubClassIndex].ubCalibre) != playerCalibres.end())
				{
					if (Item[i].ubCoolness <= ((progress+5) / 10)+2)
					{
						itemMap[AMMO_BOXES].push_back(i);
					}
				}
			}
			else if (Item[i].usItemClass & IC_GUN)
			{
				itemMap[GUNS].push_back(i);
			}
			else if (Item[i].grenadelauncher)
			{
				itemMap[GRENADELAUNCHERS].push_back(i);
			}
			else if (Item[i].rocketlauncher)
			{
				itemMap[ROCKETLAUNCHERS].push_back(i);
			}
		}
	}

	auto addItemToInventory = [](SOLDIERTYPE* pSoldier, UINT16 itemId, UINT8 amount)
	{
		OBJECTTYPE itemToAdd;
		CreateItems(itemId, 100, amount, &itemToAdd);

		itemToAdd.fFlags &= ~OBJECT_UNDROPPABLE;

		if (FitsInSmallPocket(&itemToAdd))
		{
			for(INT8 i = SMALLPOCKSTART; i < SMALLPOCKFINAL; i++ )
			{
				if( pSoldier->inv[ i ].exists() == false && !(pSoldier->inv[ i ].fFlags & OBJECT_NO_OVERWRITE) )
				{
					pSoldier->inv[ i ] = itemToAdd;
					break;
				}
			}
		}
		else
		{
			for(INT8 i = BIGPOCKSTART; i < BIGPOCKFINAL; i++ )
			{ //no space free in small pockets, so put it into a large pocket.
				if( pSoldier->inv[ i ].exists() == false && !(pSoldier->inv[ i ].fFlags & OBJECT_NO_OVERWRITE) )
				{
					pSoldier->inv[ i ] = itemToAdd;
					break;
				}
			}
		}
	};

	// cache the initial jeep count of every group we find
	std::map<UINT8, int> cachedGroupJeepCount;
	for (int slot = firstSlot; (slot <= lastSlot); ++slot)
	{
		SOLDIERTYPE* pSoldier = &Menptr[slot];

		const std::map<UINT8, std::map<int, UINT8>>::iterator groupIter = transportGroupIdToSoldierMap.find(pSoldier->ubGroupID);
		if (groupIter != transportGroupIdToSoldierMap.end())
		{
			// let's find out if this group is coming home or still outgoing to its target destination
			GROUP* pGroup = gpGroupList;
			BOOLEAN outgoing = FALSE;
			while (pGroup)
			{
				if (pGroup->ubGroupID == groupIter->first)
				{
					outgoing = pGroup->uiFlags & GROUPFLAG_TRANSPORT_ENROUTE;
					break;
				}
				pGroup = pGroup->next;
			}

			// found a matching transport groupid
			std::map<int, UINT8>::iterator soldierClassIter = groupIter->second.find(SOLDIER_CLASS_JEEP);
			if (cachedGroupJeepCount.find(groupIter->first) == cachedGroupJeepCount.end())
			{
				cachedGroupJeepCount[groupIter->first] = soldierClassIter == groupIter->second.end() ? 0 : groupIter->second[SOLDIER_CLASS_JEEP];
			}

			if (soldierClassIter != groupIter->second.end() && cachedGroupJeepCount.find(groupIter->first) != cachedGroupJeepCount.end() && cachedGroupJeepCount[groupIter->first] > 0)
			{
				TRANSPORT_GROUP_DEBUG(L"Found groupid[%d] with admin[%d] troop[%d] elite[%d] jeep[%d]", groupIter->first, groupIter->second[SOLDIER_CLASS_ADMINISTRATOR], groupIter->second[SOLDIER_CLASS_ARMY], groupIter->second[SOLDIER_CLASS_ELITE], groupIter->second[SOLDIER_CLASS_JEEP]);
				// this group has a jeep in it!
				// only jeeps carry things
				// but give a little extra, since the jeep exploding can outright destroy things
				if (pSoldier->ubSoldierClass == SOLDIER_CLASS_JEEP)
				{
					//if (outgoing)
					{
						// en route to target destination - carrying ammo, supplies, etc
						for (int i = TRANSPORT_LOOT_START; i <= TRANSPORT_LOOT_END; ++i)
						{
							const ItemTypes itemType = static_cast<ItemTypes>(i);
							if (itemMap[itemType].size() > 0)
							{
								const UINT16 id = itemMap[itemType][Random(itemMap[itemType].size())];
								switch (itemType)
								{
								case BACKPACKS:
								case RADIOS:
								case MISC:
								case AMMO_BOXES:
									// intentionally do nothing
									break;

								case GAS_CANS:
									addItemToInventory(pSoldier, id, 1);
									break;

								case MEDICAL_MEDKITS:
								case MEDICAL_OTHER:
								case TOOL_KITS:
									addItemToInventory(pSoldier, id, 2);
									break;

								case MEDICAL_FIRSTAIDKITS:
									addItemToInventory(pSoldier, id, 10);
									break;

								case GRENADE_THROWN:
									addItemToInventory(pSoldier, id, 12);
									break;

								case GUNS:
									{
										for (int loop = 0; loop < 3; ++loop)
										{
											const UINT16 gunId = itemMap[itemType][Random(itemMap[itemType].size())];
											addItemToInventory(pSoldier, gunId, 1);

											UINT16 ammoId = RandomMagazine(gunId, 0, 100, SOLDIER_CLASS_ELITE);
											if (ammoId == 0) continue; // no ammo matches, skip

											BOOLEAN convertedToBox = FALSE;
											for (INT32 itemId = 0; itemId < (INT32)gMAXITEMS_READ; ++itemId)
											{
												if( ItemIsLegal(itemId)
												&& Item[itemId].usItemClass == IC_AMMO
												&& Magazine[Item[itemId].ubClassIndex].ubMagType == AMMO_BOX
												&& Magazine[Item[itemId].ubClassIndex].ubCalibre == Magazine[Item[ammoId].ubClassIndex].ubCalibre
												&& Magazine[Item[itemId].ubClassIndex].ubAmmoType == Magazine[Item[ammoId].ubClassIndex].ubAmmoType)
												{
													// replace mag with box
													convertedToBox = TRUE;
													ammoId = itemId;
													break;
												}
											}
											addItemToInventory(pSoldier, ammoId, convertedToBox ? 2 : 10);
										}
									}
									break;

								case GRENADELAUNCHERS:
									{
										addItemToInventory(pSoldier, id, 1);

										const UINT16 launchableId = PickARandomLaunchable(id);
										if (launchableId == 0) continue; // no launchable matches, skip

										addItemToInventory(pSoldier, launchableId, 8);
									}
									break;

								case ROCKETLAUNCHERS:
									{
										addItemToInventory(pSoldier, id, Item[id].singleshotrocketlauncher ? 3 : 1);

										const UINT16 launchableId = PickARandomLaunchable(id);
										if (launchableId == 0) continue; // no launchable matches, skip

										addItemToInventory(pSoldier, launchableId, 3);
									}
									break;

								case ATTACHMENTS:
									for (int loop = 0; loop < 5; ++loop)
									{
										const UINT16 attachmentId = itemMap[itemType][Random(itemMap[itemType].size())];
										addItemToInventory(pSoldier, attachmentId, 2);
									}
									break;

								case CAMO_KITS:
									addItemToInventory(pSoldier, id, 6);
									break;

								default:
									TRANSPORT_GROUP_DEBUG(L"Warning: ignoring unhandled transport group loot type: %d", itemType);
									// nothing!
									break;
								}
							}
						}
					}
					//else // returning home
					{
						// I can't really think of a good different loot set for returning transport groups, so we'll have the same loot
						// regardless of whether the group is outgoing or incoming. I'll keep the in/out flag in case that changes
					}

					transportGroupIdToSoldierMap[pSoldier->ubGroupID][SOLDIER_CLASS_JEEP]--;
				}
				else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR
					|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY
					|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE)
				{
					// jeep is carrying most things, so soldiers just have ammo
					if (itemMap[AMMO_BOXES].size() > 0)
					{
						const UINT16 ammoId = itemMap[AMMO_BOXES][Random(itemMap[AMMO_BOXES].size())];
						addItemToInventory(pSoldier, ammoId, 1);
					}

					if (itemMap[BACKPACKS].size() > 0 && UsingNewInventorySystem())
					{
						OBJECTTYPE obj;
						CreateItem(itemMap[BACKPACKS][Random(itemMap[BACKPACKS].size())], 100, &obj);
						obj.fFlags |= OBJECT_UNDROPPABLE;
						pSoldier->inv[BPACKPOCKPOS] = obj;
					}

					// force inventory to be dropped!
					for (int i = 0; i < pSoldier->inv.size(); ++i)
					{
						OBJECTTYPE* item = &pSoldier->inv[i];
						if (item->exists() && Item[item->usItem].defaultundroppable == FALSE)
						{
							item->fFlags &= ~OBJECT_UNDROPPABLE;
						}
					}
					transportGroupIdToSoldierMap[pSoldier->ubGroupID][pSoldier->ubSoldierClass]--;
				}
			}
			else
			{
				TRANSPORT_GROUP_DEBUG(L"Found jeepless groupid[%d] with admin[%d] troop[%d] elite[%d] jeep[%d]", groupIter->first, groupIter->second[SOLDIER_CLASS_ADMINISTRATOR], groupIter->second[SOLDIER_CLASS_ARMY], groupIter->second[SOLDIER_CLASS_ELITE], groupIter->second[SOLDIER_CLASS_JEEP]);
				// no jeep in group, add things normally
				soldierClassIter = groupIter->second.find(pSoldier->ubSoldierClass);
				if (soldierClassIter != groupIter->second.end())
				{
					// found a matching soldierclass
					if (soldierClassIter->second > 0)
					{
						//if (outgoing)
						{
							for (int i = TRANSPORT_LOOT_START; i <= TRANSPORT_LOOT_END; ++i)
							{
								const ItemTypes itemType = static_cast<ItemTypes>(i);
								if (itemMap[itemType].size() > 0)
								{
									const UINT16 id = itemMap[itemType][Random(itemMap[itemType].size())];
									switch (itemType)
									{
									case GAS_CANS:
									case MEDICAL_MEDKITS:
									case MEDICAL_OTHER:
									case TOOL_KITS:
									case GUNS:
									case GRENADELAUNCHERS:
									case ROCKETLAUNCHERS:
									case GRENADE_THROWN:
									case MISC:
										// skip for foot soldiers!
										break;

									case BACKPACKS:
										{
											if (UsingNewInventorySystem())
											{
												OBJECTTYPE obj;
												CreateItem(id, 100, &obj);
												obj.fFlags |= OBJECT_UNDROPPABLE;
												pSoldier->inv[BPACKPOCKPOS] = obj;
											}
										}
										break;

									case MEDICAL_FIRSTAIDKITS:
										addItemToInventory(pSoldier, id, 1);
										break;

									case AMMO_BOXES:
										addItemToInventory(pSoldier, id, 1);
										break;

									case CAMO_KITS:
										addItemToInventory(pSoldier, id, 1);
										break;

									case ATTACHMENTS:
										// low chance of attachments
										if (Random(100) < 25)
											addItemToInventory(pSoldier, id, 1);
										break;

									default:
										TRANSPORT_GROUP_DEBUG(L"Warning: ignoring unhandled transport group loot type: %d", itemType);
										// nothing!
										break;
									}
								}
							}
						}
						//else // returning home
						{
							// I can't really think of a good different loot set for returning transport groups, so we'll have the same loot
							// regardless of whether the group is outgoing or incoming. I'll keep the in/out flag in case that changes
						}

						// force inventory to be dropped!
						for (int i = 0; i < pSoldier->inv.size(); ++i)
						{
							OBJECTTYPE* item = &pSoldier->inv[i];
							if (item->exists() && Item[item->usItem].defaultundroppable == FALSE)
							{
								item->fFlags &= ~OBJECT_UNDROPPABLE;
							}
						}
						transportGroupIdToSoldierMap[pSoldier->ubGroupID][pSoldier->ubSoldierClass]--;
					}
				}
			}
		}
	}
}

void AddToTransportGroupMap(UINT8 groupId, int soldierClass, UINT8 amount)
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
	{
		ClearTransportGroupMap();
		return;
	}

	// only update admins/troops/elites/jeeps

	switch (soldierClass)
	{
	case SOLDIER_CLASS_ADMINISTRATOR:
	case SOLDIER_CLASS_ARMY:
	case SOLDIER_CLASS_ELITE:
	case SOLDIER_CLASS_JEEP:
		transportGroupIdToSoldierMap[groupId][soldierClass] += amount;
		break;
	default:
		// do nothing!
		break;
	}
}

void ClearTransportGroupMap()
{
	transportGroupIdToSoldierMap.clear();
}

void NotifyTransportGroupDefeated()
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
		return;

	const UINT32 hoursToRememberDefeat = 24 * 7; // 7 days

	AddStrategicEvent(EVENT_TRANSPORT_GROUP_DEFEATED, GetWorldTotalMin() + 60 * hoursToRememberDefeat, 0);
}

void PopulateTransportGroup(UINT8& admins, UINT8& troops, UINT8& elites, UINT8& jeeps, UINT8& tanks, UINT8& robots, UINT8 progress, int difficulty, BOOLEAN trySpecialCase)
{
	admins = troops = elites = robots = jeeps = tanks = 0;

	// special case for only one valid destination - expert/insane only
	if (trySpecialCase && (difficulty == DIF_LEVEL_HARD || difficulty == DIF_LEVEL_INSANE))
	{
		admins = 1;
		elites = difficulty == DIF_LEVEL_HARD ? 14 : 19;

		if (elites > 0 && gGameExternalOptions.fASDAssignsJeeps && ASDSoldierUpgradeToJeep())
		{
			jeeps++;
			elites--;
		}

		if (gGameExternalOptions.fASDAssignsTanks)
		{
			const int numTanks = difficulty == DIF_LEVEL_INSANE ? 2 : 1;
			for (int i = 0; i < numTanks; ++i)
			{
				if (elites > 0 && ASDSoldierUpgradeToTank())
				{
					tanks++;
					elites--;
				}
			}
		}

		if (gGameExternalOptions.fASDAssignsRobots)
		{
			const int numRobots = Random(5);
			for (int i = 0; i < numRobots; ++i)
			{
				if (elites > 0 && ASDSoldierUpgradeToRobot())
				{
					robots++;
					elites--;
				}
			}
		}
	}
	else // normal case
	{
		UINT8 difficultyMod = 1;
		switch (difficulty)
		{
		case DIF_LEVEL_EASY:	difficultyMod = 1; break;
		case DIF_LEVEL_MEDIUM:	difficultyMod = 2; break;
		case DIF_LEVEL_HARD:	difficultyMod = 3; break;
		case DIF_LEVEL_INSANE:	difficultyMod = 4; break;
		default: break;
		}

		// default composition
		if (progress < 25)
		{
			admins = 8 - difficultyMod;
			troops = difficultyMod;
		}
		else if (progress < 50)
		{
			admins = 10 - difficultyMod * 2;
			troops = difficultyMod;
			elites = difficultyMod;
		}
		else if (progress < 75)
		{
			admins = 2;
			troops = 10 - difficultyMod * 2;
			elites = difficultyMod * 2;
		}
		else if (progress <= 100) // intentional equality
		{
			admins = 2;
			troops = 13 - difficultyMod * 3;
			elites = difficultyMod * 3;
		}
		else // at least one recent interception at max progress
		{
			admins = 2;
			troops = 18 - difficultyMod * 4;
			elites = difficultyMod * 4;
		}

		// add some vehicles, if possible
		if (progress >= gGameExternalOptions.usJeepMinimumProgress)
		{
			if (elites > 0 && gGameExternalOptions.fASDAssignsJeeps && ASDSoldierUpgradeToJeep())
			{
				jeeps++;
				elites--;
			}
		}

		if (progress >= gGameExternalOptions.usTankMinimumProgress && Random(100) < (20 + difficultyMod * 10))
		{
			if (elites > 0 && gGameExternalOptions.fASDAssignsTanks && ASDSoldierUpgradeToTank())
			{
				tanks++;
				elites--;
			}
		}

		if (progress >= gGameExternalOptions.usRobotMinimumProgress && Random(100) < (20 + difficultyMod * 10))
		{
			const int numRobots = Random(difficultyMod + 1);
			for (int i = 0; i < numRobots; ++i)
			{
				if (elites > 0 && gGameExternalOptions.fASDAssignsRobots && ASDSoldierUpgradeToRobot())
				{
					robots++;
					elites--;
				}
			}
		}
	}
}

const std::map<UINT8, TransportGroupSectorInfo> GetTransportGroupSectorInfo()
{
	return transportGroupSectorInfo;
}

#undef TRANSPORT_GROUP_DEBUG

