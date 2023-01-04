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
#include "Game Clock.h"
#include "Game Event Hook.h"
#include "message.h"
#include "Overhead.h"
#include "Overhead Types.h"
#include "random.h"
#include "strategic.h"
#include "Strategic AI.h"
#include "strategicmap.h"
#include "Strategic Mines.h"
#include "Strategic Movement.h"
#include "Strategic Town Loyalty.h"


extern BOOLEAN gfTownUsesLoyalty[MAX_TOWNS];

std::map<UINT8, std::map<int, UINT8>> transportGroupIdToSoldierMap;

BOOLEAN DeployTransportGroup()
{
	// rftr todo: do nothing if feature disabled
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
	if (transportGroupCount >= 5) return FALSE;
	
	// rftr todo: create a new group in the capital (same as attack/patrol groups) and send it to a friendly town with a mine!
	// limitations: max number of transport groups at any given time
	// track recent transport group interceptions
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
	// rftr todo: do nothing if feature disabled

	const int firstSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID;
	const int lastSlot = gTacticalStatus.Team[ ENEMY_TEAM ].bLastID;

	// rftr todo: detect whether this group is going to/from HQ,
	// and update loot accordingly

	// rftr todo: do this on init/load somewhere
	// do some prep
	std::vector<UINT16> gasCans;
	std::vector<UINT16> firstAidKits;
	std::vector<UINT16> medKits;
	std::vector<UINT16> toolKits;
	std::vector<UINT16> backpacks;
	std::map<INT8, std::vector<UINT16>> ammoBoxes; // map coolness to ammo vector
	std::map<INT8, std::vector<UINT16>> ammoCrates; // map coolness to ammo vector

	// one-time item cache build
	{
		// rftr todo: see if we can replace this with random items.
		// requirement: probably a new flag in Items.xml, <Loot_TransportGroup> or something
		// add new groups in RandomItem.xml
		// the new items will reference the new group in randomitem, eg <randomitem>23</randomitem> in Items.xml matches uiIndex 23 in RandomItem.xml
		// fallback if no random items found? for mods and stuff (thinking sdo)
		for (UINT16 i = 0; i < MAXITEMS; ++i)
		{
			if (Item[i].gascan) gasCans.push_back(i);
			else if (Item[i].firstaidkit) firstAidKits.push_back(i);
			else if (Item[i].medicalkit) medKits.push_back(i);
			else if (Item[i].toolkit) toolKits.push_back(i);
			else if (Item[i].usItemClass & IC_LBEGEAR)
			{
				if (LoadBearingEquipment[Item[i].ubClassIndex].lbeClass == BACKPACK)
				{
					// todo get actual backpacks, not covert ones, tactical slings, golf clubs, etc...
					backpacks.push_back(i);
				}
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
		}
	}

	auto addItemToInventory = [](SOLDIERTYPE* pSoldier, OBJECTTYPE& itemToAdd)
	{
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
					for (int i = 0; i < 3; ++i)
					{
						CreateItem(ammoBoxes[5][0], 100, &itemToAdd);
						addItemToInventory(pSoldier, itemToAdd);
					}

					for (int i = 0; i < 3; ++i)
					{
						CreateItem(medKits[Random(medKits.size())], 100, &itemToAdd);
						addItemToInventory(pSoldier, itemToAdd);
					}
					transportGroupIdToSoldierMap[pSoldier->ubGroupID][SOLDIER_CLASS_JEEP]--;
				}
				else if (pSoldier->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR
					|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ARMY
					|| pSoldier->ubSoldierClass == SOLDIER_CLASS_ELITE)
				{
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
						if (backpacks.size() > 0)
						{
							CreateItem(backpacks[0], 75 + Random(25), &itemToAdd);
							pSoldier->inv[BPACKPOCKPOS] = itemToAdd;
						}

						// add ammo to the soldier's inventory!
						CreateItem(ammoBoxes[10][0], (INT8)(90+Random(10)), &itemToAdd);
						addItemToInventory(pSoldier, itemToAdd);

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

