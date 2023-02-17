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

TODO LIST:
- determine how/when/what groups are deployed
- use proper loyalty degradation (Strategic Loyalty lua) (maybe...)
- use enemygunchoices and enemyitemchoices to populate bonus loot, depending on jeep or no jeep
- track previous failed transports? transport group alertness level? partially degrades over time?
	- we can use strategic status (gStrategicStatus. there's tons of unused padding). that way we don't need to muck with savegames,
	  as the bytes are already there.
	- what do we actually want to track?
		- alertness level (affects group compositions)
		- recent losses (strategic events?) (affects group compositions)
		- groups should track their own "readiness level" (ie, strategic stats when they were spawned) (could use the padding in GROUP->ENEMYGROUP)

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
#include "random.h"
#include "Soldier Control.h"
#include "strategic.h"
#include "Strategic AI.h"
#include "strategicmap.h"
#include "Strategic Mines.h"
#include "Strategic Movement.h"
#include "Strategic Town Loyalty.h"

extern ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[SOLDIER_GUN_CHOICE_SELECTIONS][ARMY_GUN_LEVELS];
extern ARMY_GUN_CHOICE_TYPE gArmyItemChoices[SOLDIER_GUN_CHOICE_SELECTIONS][MAX_ITEM_TYPES];
extern BOOLEAN gfTownUsesLoyalty[MAX_TOWNS];
//extern STRATEGIC_STATUS gStrategicStatus;

std::map<UINT8, std::map<int, UINT8>> transportGroupIdToSoldierMap;

BOOLEAN DeployTransportGroup()
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
		return FALSE;

	if (giReinforcementPool <= 0)
		return FALSE;

	// is there a mine here?
	// rftr todo: valid destination towns depend on difficulty
	std::vector<INT16> mineSectorIds;
	for (INT8 i = 0; i < NUM_TOWNS; ++i)
	{
		// skip towns that have no loyalty
		if (!gfTownUsesLoyalty[i]) continue;
		// filter by TOWN ownership
		if (IsTownUnderCompleteControlByEnemy(i) == FALSE) continue;
		// skip towns with a shut down mine
		const INT8 mineIndex = GetMineIndexForTown(i);
		if (mineIndex == -1) continue;
		if (IsMineShutDown(mineIndex) == TRUE) continue;
		// filter by MINE ownership
		const INT16 mineSector = GetMineSectorForTown(i);
		if (StrategicMap[mineSector].fEnemyControlled == FALSE) continue;

		mineSectorIds.push_back(mineSector);
	}
	ScreenMsg(FONT_RED, MSG_INTERFACE, L"DeployTransportGroup valid town destinations: %d", mineSectorIds.size());

	// rftr todo: special case when only one town left?
	if (mineSectorIds.size() < 1) return FALSE;

	INT8 transportGroupCount = 0;
	GROUP* pGroup = gpGroupList;
	while (pGroup)
	{
		if (pGroup->usGroupTeam == ENEMY_TEAM && pGroup->pEnemyGroup->ubIntention == TRANSPORT)
		{
			transportGroupCount++;
		}
		pGroup = pGroup->next;
	}
	ScreenMsg(FONT_RED, MSG_INTERFACE, L"DeployTransportGroup found existing transport groups: %d", transportGroupCount);

	// if there are too many active transport groups, don't deploy any more
	// rftr todo: based on difficulty?
	if (transportGroupCount >= gGameExternalOptions.iMaxSimultaneousTransportGroups) return FALSE;
	
	// rftr todo: create a new group in the capital (same as attack/patrol groups) and send it to a friendly town with a mine!
	// limitations: max number of transport groups at any given time
	// track recent transport group interceptions
	const INT8 recentLossCount = GetAllStrategicEventsOfType(EVENT_TRANSPORT_GROUP_DEFEATED).size();
	// varying transport group quality/compositions
	// copied from NPC_ACTION_SEND_SOLDIERS_TO_BATTLE_LOCATION, which happens after the first non-welcome wagon battle
	// rftr todo: replace this with townid
	// rftr todo: only pick towns that 1) have mines, and 2) are uncontested
	const UINT8 ubSectorID = (UINT8)mineSectorIds[Random(mineSectorIds.size())];
	const SECTORINFO* pSector = &SectorInfo[ ubSectorID ];

	// rftr: adjust group size and composition based on recent interceptions, game progress, etc
	const INT32 ubNumSoldiers = 17;

	//InitializeGroup(GROUP_TYPE_TRANSPORT, ubNumSoldiers, grouptroops[0], groupelites[0], grouprobots[0], groupjeeps[0], grouptanks[0], Random(10) < difficultyMod);
	//totalusedsoldiers += grouptroops[0] + groupelites[0] + grouprobots[0] + grouptanks[0] + groupjeeps[0];

	//pGroup = CreateNewEnemyGroupDepartingFromSector( SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), 0, grouptroops[0], groupelites[0], grouprobots[0], grouptanks[0], groupjeeps[0] );
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_D5, 10, 5, 1, 0, 0, 1 );
	pGroup->pEnemyGroup->ubReadiness = HighestPlayerProgressPercentage();

	//Madd: unlimited reinforcements?
	if ( !gfUnlimitedTroops )
	{
		giReinforcementPool -= ubNumSoldiers;

		giReinforcementPool = max( giReinforcementPool, 0 );
	}

	MoveSAIGroupToSector( &pGroup, ubSectorID, EVASIVE, TRANSPORT );

	pGroup->uiFlags |= GROUPFLAG_TRANSPORT_ENROUTE;

	return TRUE;
}

BOOLEAN ReturnTransportGroup(INT32 option1)
{
	GROUP* pGroup = gpGroupList;
	while (pGroup)
	{
		if (pGroup->ubGroupID == option1)
		{
			MoveSAIGroupToSector( &pGroup, SECTOR( gModSettings.ubSAISpawnSectorX, gModSettings.ubSAISpawnSectorY ), EVASIVE, TRANSPORT );
			pGroup->uiFlags &= ~GROUPFLAG_TRANSPORT_ENROUTE;
			break;
		}
		pGroup = pGroup->next;
	}

	if (pGroup == nullptr)
	{
		ScreenMsg( FONT_YELLOW, MSG_INTERFACE, L"RETURN_TRANSPORT_GROUP failed to find groupid %d", option1);
		return FALSE;
	}

	return TRUE;
}

void FillMapColoursForTransportGroups(INT32(&colorMap)[MAXIMUM_VALID_Y_COORDINATE][MAXIMUM_VALID_X_COORDINATE])
{
	if (gGameExternalOptions.fStrategicTransportGroupsEnabled == FALSE)
		return;

				// spies identify incoming transport groups
				// RIS identifies ALL transport groups in monitored areas? in all areas?

	const auto debugColor = MAP_SHADE_LT_BLUE;
	const auto targetColor = MAP_SHADE_LT_YELLOW;
	const INT8 DETECTION_RANGE_SCOUT = 1;
	const INT8 DETECTION_RANGE_RADIO = 3;
	const INT8 DETECTION_RANGE_COVERT = 0;
	GROUP* pGroup = gpGroupList;

	// build map of detection sectors + ranges
	std::map<std::pair<INT16,INT16>, INT8> detectionMap;
	std::map<UINT8, BOOLEAN> monitoredTowns;
	for( INT16 i = gTacticalStatus.Team[ OUR_TEAM ].bFirstID; i <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; i++ )
	{
		if( MercPtrs[ i ]->bActive &&
			MercPtrs[ i ]->stats.bLife >= OKLIFE &&
			MercPtrs[ i ]->bAssignment < ON_DUTY &&
			!MercPtrs[ i ]->flags.fMercAsleep)
		{
			if (gGameOptions.fNewTraitSystem)
			{
				if (HAS_SKILL_TRAIT(MercPtrs[i], SCOUTING_NT))
				{
					detectionMap[std::pair<INT16,INT16>(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = DETECTION_RANGE_SCOUT;
				}
				else if (HAS_SKILL_TRAIT(MercPtrs[i], RADIO_OPERATOR_NT))
				{
					detectionMap[std::pair<INT16,INT16>(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = DETECTION_RANGE_RADIO;
				}
				else if (HAS_SKILL_TRAIT(MercPtrs[i], COVERT_NT))
				{
					if (MercPtrs[i]->bAssignment == GATHERINTEL)
					{
						detectionMap[std::pair<INT16,INT16>(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = DETECTION_RANGE_COVERT;
						monitoredTowns[GetTownIdForSector(MercPtrs[i]->sSectorX, MercPtrs[i]->sSectorY)] = FALSE;
					}
				}
			}
		}
	}

	while (pGroup)
	{
		if (pGroup->usGroupTeam == ENEMY_TEAM)
		{
			const UINT8 intention = pGroup->pEnemyGroup->ubIntention;
			if (intention == TRANSPORT )
			{
				// rftr todo: delete me!
				colorMap[pGroup->ubSectorY-1][pGroup->ubSectorX-1] = debugColor;

				// check if current location is known
				const INT16 gx = pGroup->ubSectorX;
				const INT16 gy = pGroup->ubSectorY;
				for (const auto key : detectionMap)
				{
					const std::pair<INT16, INT16> sector = key.first;
					const INT8 range = key.second;

					const INT8 dist = abs((gx - sector.first) + (gy - sector.second));
					if (dist <= range)
					{
						colorMap[pGroup->ubSectorY-1][pGroup->ubSectorX-1] = targetColor;
					}
				}

				// check if target location is monitored 
				WAYPOINT* wp = pGroup->pWaypoints;

				while (wp)
				{
					if (wp->next == nullptr)
						break;

					wp = wp->next;
				}
				const UINT8 townId = GetTownIdForSector(wp->x, wp->y);
				if (monitoredTowns.find(townId) != monitoredTowns.end())
				{
					monitoredTowns[townId] = TRUE;
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
			if (monitoredTowns.find(townId) != monitoredTowns.end() && monitoredTowns[townId])
			{
				colorMap[x-1][y-1] = targetColor;
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
		MISC,
		GRENADE_THROWN,
		GUNS,
		GRENADELAUNCHERS,
		ROCKETLAUNCHERS,
		AMMO_BOXES,
		AMMO_CRATES,

		TRANSPORT_LOOT_START = GAS_CANS,
		TRANSPORT_LOOT_END = ROCKETLAUNCHERS,
	};

	std::map<ItemTypes, std::vector<UINT16>> itemMap;

	std::map<INT8, std::vector<UINT16>> ammoBoxes; // map coolness to ammo vector
	std::map<INT8, std::vector<UINT16>> ammoCrates; // map coolness to ammo vector
	
	// rftr todo: instead of building ammo caches, perhaps we could examine ChooseWeaponForSoldierCreateStruct(). excerpt:
	// usAmmoIndex = RandomMagazine( &pp->Inv[HANDPOS], ubChanceStandardAmmo, max(Item[usGunIndex].ubCoolness, HighestPlayerProgressPercentage() / 10 + 3 ), pp->ubSoldierClass);
	// however, we'd still need to convert the magazine into an ammo box
	// for conversion, see the following (ammo conversion in strategic inventory)
	// void SortSectorInventoryAmmo(bool useBoxes)

	// one-time item cache build
	{
		//gExtendedArmyGunChoices[SOLDIER_CLASS_ELITE][gunLevel];
		//gArmyItemChoices[SOLDIER_CLASS_ELITE][typeIndex];

		for (UINT16 i = 0; i < gMAXITEMS_READ; ++i)
		{
			if (!ItemIsLegal(i, TRUE)) continue;
			if (Item[i].iTransportGroupMaxProgress == 0 || Item[i].iTransportGroupMinProgress > progress || progress > Item[i].iTransportGroupMaxProgress) continue;

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
			else if (Item[i].usItemClass & IC_MISC)
			{
				itemMap[MISC].push_back(i);
			}
			else if (Item[i].usItemClass & IC_AMMO)
			{
				if (Magazine[Item[i].ubClassIndex].ubMagType == AMMO_BOX
				|| Magazine[Item[i].ubClassIndex].ubMagType == AMMO_CRATE)
				{
					if ((gGameOptions.fGunNut || !Item[i].biggunlist)
					&& (gGameOptions.ubGameStyle == STYLE_SCIFI || !Item[i].scifi))
					{
						if (Magazine[Item[i].ubClassIndex].ubMagType == AMMO_BOX)
							ammoBoxes[Item[i].ubCoolness].push_back(i);
						else
							ammoCrates[Item[i].ubCoolness].push_back(i);
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

			const UINT8 groupReadiness = pGroup->pEnemyGroup->ubReadiness;

			// found a matching transport groupid
			std::map<int, UINT8>::iterator soldierClassIter = groupIter->second.find(SOLDIER_CLASS_JEEP);
			if (soldierClassIter != groupIter->second.end())
			{
				// this group has a jeep in it!
				// only jeeps carry things
				// but give a little extra, since the jeep exploding can outright destroy things
				if (pSoldier->ubSoldierClass == SOLDIER_CLASS_JEEP)
				{
					OBJECTTYPE itemToAdd;
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

								default:
									ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Warning: ignoring unhandled transport group loot type: %d", itemType);
									// nothing!
									break;
								}
							}
						}
					}
					//else // returning home
					{
						// coming back home - carrying money/loot/???
					}

					transportGroupIdToSoldierMap[pSoldier->ubGroupID][SOLDIER_CLASS_JEEP]--;
				}
				else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR
					|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY
					|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE)
				{
					// jeep is carrying everything, so just force inventory to be dropped!
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
				// no jeep in group, add things normally
				soldierClassIter = groupIter->second.find(pSoldier->ubSoldierClass);
				if (soldierClassIter != groupIter->second.end())
				{
					// found a matching soldierclass
					if (soldierClassIter->second > 0)
					{
						// there are still un-updated soldiers! begin the update!
						soldierClassIter->second--;

						// rftr todo: move this into its own function
						// rftr todo: don't forget to call this for reinforcing troops!
						// adjust soldier inventory for transport groups

						// ideas:
						// soldiers have backpacks + kits + ammo box (BONUS: make sure the backpack goes in the backpack slot for lobot compatibility. that might be a fix outside of this feature tho)
						// jeeps have ammo crates and/or lots of boxes. reduce bullet count in crate?

						// add backpack to soldier's inventory!
						OBJECTTYPE itemToAdd;
						if (itemMap[BACKPACKS].size() > 0)
						{
							CreateItem(itemMap[BACKPACKS][0], 100, &itemToAdd);
							pSoldier->inv[BPACKPOCKPOS] = itemToAdd;
						}

						//if (outgoing)
						{
							// add ammo to the soldier's inventory!
							addItemToInventory(pSoldier, ammoBoxes[10][0], 1);
						}
						//else // returning home
						{
							// coming back home - carrying money/loot/???
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

