#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdio.h>
	#include <string.h>
	#include "stdlib.h"
	#include "debug.h"
	#include "math.h"
	#include "worlddef.h"
	#include "renderworld.h"

	#include "Random.h"
	#include "Campaign.h"
#endif

#include "XML.h"

//Jenilee: full randomization of mercs, including traits, gear kits and all statistics.
//to use, set MERCS_RANDOM_STATS to 4 in JA2_Options.ini.

//the randomizer uses a set of 1-3 roles per merc that define their general character.
enum rolesets_t
{
	ROLE_GENERIC = 0,
	ROLE_MEDIC = 1,
	ROLE_MECHANIC,
	ROLE_EXPLOSIVES_EXPERT,
	ROLE_LEADER,
	ROLE_COMBAT_SNIPER,
	ROLE_COMBAT_HUNTER,
	ROLE_COMBAT_RIFLEMAN,
	ROLE_COMBAT_MARTIAL_ARTIST,
	ROLE_COMBAT_GUNSLINGER,
	ROLE_COMBAT_HEAVY_WEAPONS,
	ROLE_COMBAT_GRENADIER
};

//types of camo for armor generation
enum camotypes_t
{
	CAMO_ARMOR = 1,
	CAMO_URBAN,
	CAMO_WOODLAND,
	CAMO_DESERT,
	CAMO_STEALTH
};

//types of face items
enum random_faceitems_t
{
	FACEITEM_GAS_MASK = 1,
	FACEITEM_NVG,
	FACEITEM_SPECTACLES,
	FACEITEM_HEADSET
};

UINT8* roles[15]; //roles are saved in this array

//these vectors store suitable items for randomization
std::vector<INVTYPE*> gear_helmets;
std::vector<INVTYPE*> gear_vests;
std::vector<INVTYPE*> gear_leggings;
std::vector<INVTYPE*> gear_plates;
std::vector<INVTYPE*> gear_faceitems;
std::vector<INVTYPE*> gear_lbe;
std::vector<INVTYPE*> gear_weapons;
std::vector<INVTYPE*> gear_magazines;
std::vector<INVTYPE*> gear_attachments;
std::vector<INVTYPE*> gear_misc;

//prototypes
bool GenerateRoleSet(UINT8 role_name, UINT8 hlt, UINT8 dex, UINT8 agi, UINT8 str, UINT8 wis, UINT8 mrk, UINT8 ldr, UINT8 mec, UINT8 exp, UINT8 med);

//generate the stat distributions per role, to be used in the roll tables, and read in pointers to items
void InitRandomMercs()
{
												//	HLT		DEX		AGI		STR		WIS		MRK		LDR		MEC		EXP		MED
	GenerateRoleSet(ROLE_GENERIC,					14,		13,		13,		14,		13,		13,		5,		5,		5,		5);

	GenerateRoleSet(ROLE_MEDIC,						8,		18,		16,		5,		14,		15,		3,		2,		2,		17);
	GenerateRoleSet(ROLE_MECHANIC,					9,		17,		15,		5,		15,		17,		1,		18,		1,		2);
	GenerateRoleSet(ROLE_EXPLOSIVES_EXPERT,			8,		19,		14,		6,		13,		16,		2,		2,		19,		1);
	GenerateRoleSet(ROLE_LEADER,					10,		6,		15,		10,		18,		18,		18,		1,		1,		3);

	GenerateRoleSet(ROLE_COMBAT_GRENADIER,			12,		8,		21,		19,		11,		21,		1,		1,		5,		1);
	GenerateRoleSet(ROLE_COMBAT_SNIPER,				12,		11,		20,		11,		12,		28,		1,		2,		1,		2);
	GenerateRoleSet(ROLE_COMBAT_HUNTER,				11,		11,		23,		10,		13,		25,		1,		1,		1,		4);
	GenerateRoleSet(ROLE_COMBAT_RIFLEMAN,			15,		10,		22,		14,		9,		23,		2,		1,		1,		3);
	GenerateRoleSet(ROLE_COMBAT_MARTIAL_ARTIST,		12,		23,		22,		12,		13,		14,		1,		1,		1,		1);
	GenerateRoleSet(ROLE_COMBAT_GUNSLINGER,			14,		12,		24,		11,		12,		22,		1,		1,		1,		2);
	GenerateRoleSet(ROLE_COMBAT_HEAVY_WEAPONS,		14,		8,		20,		18,		10,		22,		1,		1,		5,		1);
												//	HLT		DEX		AGI		STR		WIS		MRK		LDR		MEC		EXP		MED

	//populate random gear list
	for ( int i = 0; i < gMAXITEMS_READ; ++i )
	{
		INVTYPE* item = &Item[i];

		if (item->usItemClass == IC_AMMO) //ammo is not affected by all the other filters
		{
			gear_magazines.push_back(item);
		}

		if (item->usPrice < 1 || item->ubCoolness < 1) //if it has no price or coolness we don't want it
			continue;

		if (StoreInventory[i][0] < 1) //if BobbyRay doesn't sell it then we won't use it either
			continue;

		if (item->usItemClass == IC_ARMOUR)
		{
			ARMOURTYPE* armor = &Armour[item->ubClassIndex];
			
			if (armor->ubArmourClass == ARMOURCLASS_HELMET)
				gear_helmets.push_back(item);
			else if (armor->ubArmourClass == ARMOURCLASS_VEST)
				gear_vests.push_back(item);
			else if (armor->ubArmourClass == ARMOURCLASS_LEGGINGS)
				gear_leggings.push_back(item);
			else if (armor->ubArmourClass == ARMOURCLASS_PLATE)
				gear_plates.push_back(item);
		}
		else if (item->usItemClass == IC_FACE)
		{
			gear_faceitems.push_back(item);
		}
		else if (item->usItemClass == IC_LBEGEAR)
		{
			gear_lbe.push_back(item);
		}
		else if (item->usItemClass == IC_GUN || item->usItemClass == IC_BLADE || item->usItemClass == IC_THROWING_KNIFE || item->usItemClass == IC_LAUNCHER)
		{
			gear_weapons.push_back(item);
		}
		else if (item->usItemClass == IC_MISC)
		{
			gear_attachments.push_back(item);
		}
		else if (item->usItemClass == IC_BLADE || item->usItemClass == IC_THROWING_KNIFE || item->usItemClass == IC_THROWN || item->usItemClass == IC_PUNCH || item->usItemClass == IC_APPLIABLE || 
			     item->usItemClass == IC_MONEY || item->usItemClass == IC_GRENADE || item->usItemClass == IC_BOMB || item->usItemClass == IC_MEDKIT || item->usItemClass == IC_KIT)
		{
			gear_misc.push_back(item);
		}
	}
}

//clean up
void ExitRandomMercs()
{
	for (UINT8 i = 0;i < 15; i++)
	{
		if (roles[i])
			delete[] roles[i];
	}
}

//generate roll tables for roles
bool GenerateRoleSet(UINT8 role_name, UINT8 hlt, UINT8 dex, UINT8 agi, UINT8 str, UINT8 wis, UINT8 mrk, UINT8 ldr, UINT8 mec, UINT8 exp, UINT8 med)
{
	if ((hlt + dex + agi + str + wis + mrk + ldr+ mec + exp + med) > 100)
		return false; //total roll table sum must be 100

	UINT8* set = new UINT8[100];
	UINT8 counter = 0;

	for (; med > 0; med--) { set[counter++] = MEDICALAMT; }
	for (; dex > 0; dex--) { set[counter++] = DEXTAMT; }
	for (; agi > 0; agi--) { set[counter++] = AGILAMT; }
	for (; str > 0; str--) { set[counter++] = STRAMT; }
	for (; wis > 0; wis--) { set[counter++] = WISDOMAMT; }
	for (; mrk > 0; mrk--) { set[counter++] = MARKAMT; }
	for (; ldr > 0; ldr--) { set[counter++] = LDRAMT; }
	for (; mec > 0; mec--) { set[counter++] = MECHANAMT; }
	for (; exp > 0; exp--) { set[counter++] = EXPLODEAMT; }
	while (counter < 100) { set[counter++] = HEALTHAMT; } //health is filler stat

	roles[role_name] = set;

	return true;
}

//this function will increase the specified stat and checks for boundaries
UINT8 IncreaseStat(MERCPROFILESTRUCT* merc, UINT8 stat)
{
	if (stat == HEALTHAMT)			{ if (merc->bLifeMax < 100)			{ merc->bLifeMax++; merc->bLife = merc->bLifeMax; } }
	else if (stat == DEXTAMT)		{ if (merc->bDexterity < 100)		{ merc->bDexterity++; } }
	else if (stat == AGILAMT)		{ if (merc->bAgility < 100)			{ merc->bAgility++; } }
	else if (stat == STRAMT)		{ if (merc->bStrength < 100)		{ merc->bStrength++; } }
	else if (stat == WISDOMAMT)		{ if (merc->bWisdom < 100)			{ merc->bWisdom++; } }
	else if (stat == MARKAMT)		{ if (merc->bMarksmanship < 100)	{ merc->bMarksmanship++; } }
	else if (stat == LDRAMT)		{ if (merc->bLeadership < 100)		{ merc->bLeadership++; } }
	else if (stat == MECHANAMT)		{ if (merc->bMechanical < 100)		{ merc->bMechanical++; } }
	else if (stat == EXPLODEAMT)	{ if (merc->bExplosive < 100)		{ merc->bExplosive++; } }
	else if (stat == MEDICALAMT)	{ if (merc->bMedical < 100)			{ merc->bMedical++; } }

	return 1;
}

//get a random number from the specified roll table
UINT8 GetRandomNumberForRole(UINT8 role_name)
{
	UINT8 rnd = Random(100);
	return roles[role_name][rnd];
}

//returns the amount of combat traits we have
FLOAT CountCombatTraits(std::vector<UINT8>* traits)
{
	FLOAT amount = 0;

	for (std::vector<UINT8>::iterator it = traits->begin(); it != traits->end(); it++)
	{
		if (*it == AUTO_WEAPONS_NT ||
			*it == HEAVY_WEAPONS_NT ||
			*it == SNIPER_NT ||
			*it == RANGER_NT ||
			*it == GUNSLINGER_NT ||
			*it == DEMOLITIONS_NT)
		{
			amount += 1.0f;
		}

		if (*it == MARTIAL_ARTS_NT ||
			*it == MELEE_NT ||
			*it == THROWING_NT)
		{
			amount += 0.5f;
		}
	}
	
	return amount;
}

//returns the amount of times we have trait x (since some have 2 tiers)
UINT8 HasTrait(std::vector<UINT8>* traits, UINT8 trait_to_check)
{
	UINT8 amount = 0;

	for (std::vector<UINT8>::iterator it = traits->begin(); it != traits->end(); it++)
	{
		if (*it == trait_to_check)
			amount++;
	}
	
	return amount;
}

//returns whether we have role x
BOOLEAN HasRole(std::vector<UINT8>* roles, UINT8 role_to_check)
{
	bool exists = false;

	for (std::vector<UINT8>::iterator it = roles->begin(); it != roles->end(); it++)
	{
		if (*it == role_to_check)
			exists = true;
	}
	
	return exists;
}

//decide which roles this merc will have
std::vector<UINT8> GrantRoles(UINT8 num_roles_to_add)
{
	std::vector<UINT8> roles;

	while (num_roles_to_add > 0)
	{
		UINT8 combat_role = Random(2);
		UINT8 new_role = 0;

		if (combat_role == 1)
			new_role = Random(7) + 5;
		else
			new_role = Random(4) + 1;

		if (!HasRole(&roles, new_role))
		{
			num_roles_to_add--;
			roles.push_back(new_role);
		}
	}

	return roles;
}

//check which roles we qualify for. you are far more likely to obtain traits for which you have the associated role. skill checks are higher for the second level of a trait
UINT8 TestForTrait(UINT8 trait_to_attempt, MERCPROFILESTRUCT* merc, std::vector<UINT8>* roles, std::vector<UINT8>* traits)
{
	UINT16 success_rate = 0; // 1/1000th! NOT percentage!

	if (trait_to_attempt < 1 || trait_to_attempt > 19) // don't bother
		return 0;

	if (trait_to_attempt > 9 && HasTrait(traits, trait_to_attempt) > 0) //can only have a minor trait once!
		return 0;

	if (trait_to_attempt < 10 && HasTrait(traits, trait_to_attempt) > 1) //can only have a major trait twice!
		return 0;

	switch (trait_to_attempt)
	{
		case AUTO_WEAPONS_NT:
			if (HasTrait(traits, AUTO_WEAPONS_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_RIFLEMAN) * 200) + ((merc->bMarksmanship > 50) ? 100 : 0) + ((merc->bMarksmanship > 70) ? 100 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_RIFLEMAN) * 800) + ((merc->bMarksmanship > 30) ? 100 : 0) + ((merc->bMarksmanship > 40) ? 100 : 0);
			break;
		case HEAVY_WEAPONS_NT:
			if (HasTrait(traits, HEAVY_WEAPONS_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_HEAVY_WEAPONS) * 200) + ((merc->bStrength > 70) ? 100 : 0) + ((merc->bStrength > 80) ? 100 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_HEAVY_WEAPONS) * 800) + ((merc->bStrength > 40) ? 100 : 0) + ((merc->bStrength > 60) ? 100 : 0);
			break;
		case SNIPER_NT:
			if (HasTrait(traits, SNIPER_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_SNIPER) * 200) + ((merc->bMarksmanship > 85) ? 100 : 0) + ((merc->bMarksmanship > 90) ? 100 : 0) + ((merc->bMarksmanship > 95) ? 200 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_SNIPER) * 800) + ((merc->bMarksmanship > 70) ? 100 : 0) + ((merc->bMarksmanship > 80) ? 100 : 0);
			break;
		case RANGER_NT:
			if (HasTrait(traits, RANGER_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_HUNTER) * 200) + ((merc->bMarksmanship > 70) ? 100 : 0) + ((merc->bAgility > 70) ? 100 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_HUNTER) * 800) + ((merc->bMarksmanship > 40) ? 100 : 0) + ((merc->bAgility > 40) ? 100 : 0);
			break;
		case GUNSLINGER_NT:
			if (HasTrait(traits, GUNSLINGER_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_GUNSLINGER) * 200) + ((merc->bMarksmanship > 70) ? 100 : 0) + ((merc->bMarksmanship > 80) ? 100 : 0) + ((merc->bMarksmanship > 90) ? 100 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_GUNSLINGER) * 800) + ((merc->bMarksmanship > 40) ? 100 : 0) + ((merc->bMarksmanship > 60) ? 100 : 0);
			break;
		case MARTIAL_ARTS_NT:
			if (HasTrait(traits, MARTIAL_ARTS_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_MARTIAL_ARTIST) * 200) + ((merc->bDexterity > 70) ? 150 : 0) + ((merc->bDexterity > 80) ? 150 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_COMBAT_MARTIAL_ARTIST) * 800) + ((merc->bDexterity > 40) ? 150 : 0) + ((merc->bDexterity > 60) ? 100 : 0);
			break;
		case SQUADLEADER_NT:
			if (HasTrait(traits, SQUADLEADER_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_LEADER) * 200) + ((merc->bLeadership > 70) ? 150 : 0) + ((merc->bLeadership > 80) ? 150 : 0) + ((merc->bLeadership > 90) ? 150 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_LEADER) * 800) + ((merc->bLeadership > 40) ? 200 : 0) + ((merc->bLeadership > 50) ? 150 : 0);
			break;
		case TECHNICIAN_NT:
			if (HasTrait(traits, TECHNICIAN_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_MECHANIC) * 200) + ((merc->bMechanical > 70) ? 100 : 0) + ((merc->bDexterity > 60) ? 100 : 0) + ((merc->bDexterity > 75) ? 150 : 0) + ((merc->bMechanical > 85) ? 150 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_MECHANIC) * 800) + ((merc->bMechanical > 35) ? 100 : 0);
			break;
		case DOCTOR_NT:
			if (HasTrait(traits, DOCTOR_NT) > 0)
				success_rate = 0 + (HasRole(roles, ROLE_MEDIC) * 200) + ((merc->bMedical > 70) ? 150 : 0) + ((merc->bDexterity > 60) ? 150 : 0) + ((merc->bMedical > 85) ? 150 : 0) + ((merc->bDexterity > 75) ? 150 : 0);
			else
				success_rate = 0 + (HasRole(roles, ROLE_MEDIC) * 800) + ((merc->bMedical > 35) ? 100 : 0);
			break;
		case AMBIDEXTROUS_NT:
			success_rate = 50 + (HasRole(roles, ROLE_COMBAT_GUNSLINGER) * 100) + ((merc->bDexterity > 60) ? 100 : 0) + ((merc->bDexterity > 80) ? 100 : 0) + ((merc->bDexterity > 90) ? 100 : 0);
			break;
		case MELEE_NT:
			success_rate = 50 + ((merc->bDexterity > 30) ? 50 : 0) + ((merc->bDexterity > 60) ? 200 : 0);
			break;
		case THROWING_NT:
			success_rate = 50 + ((merc->bDexterity > 30) ? 100 : 0) + ((merc->bDexterity > 60) ? 200 : 0);
			break;
		case NIGHT_OPS_NT:
			success_rate = 200;
			break;
		case STEALTHY_NT:
			success_rate = 200;
			break;
		case ATHLETICS_NT:
			success_rate = 70 + ((merc->bAgility > 50) ? 100 : 0) + ((merc->bAgility > 70) ? 200 : 0) + ((merc->bAgility > 90) ? 200 : 0);
			break;
		case BODYBUILDING_NT:
			success_rate = 70 + ((merc->bStrength > 30) ? 100 : 0) + ((merc->bStrength > 60) ? 200 : 0) + ((merc->bStrength > 90) ? 200 : 0);
			break;
		case DEMOLITIONS_NT:
			success_rate = 0 + (HasRole(roles, ROLE_EXPLOSIVES_EXPERT) * 800) + (HasRole(roles, ROLE_COMBAT_GRENADIER) * 800) + ((merc->bExplosive > 30) ? 200 : 0) + ((merc->bStrength > 35) ? 200 : 0);
			break;
		case TEACHING_NT:
			success_rate = 10 + (HasRole(roles, ROLE_LEADER) * 300) + ((merc->bLeadership > 40) ? 200 : 0) + ((merc->bLeadership > 70) ? 200 : 0);
			break;
		case SCOUTING_NT:
			success_rate = 70 + (HasRole(roles, ROLE_COMBAT_HUNTER) * 150) + (HasRole(roles, ROLE_COMBAT_SNIPER) * 80);
			break;
	}

	if (success_rate > Random(1000))
		return trait_to_attempt;
	else
		return 0;
}

//clear this merc's kit
void GearKitClear(UINT8 profid, UINT8 kit_number)
{
	gMercProfileGear[profid][kit_number].clearInventory();
}

//set price for this kit
void GearKitSetPrice(UINT8 profid, UINT8 kit_number, INT16 price)
{
	gMercProfileGear[profid][kit_number].AbsolutePrice = price;
}

//get the specified slot 
INVTYPE* GearGetSlot(UINT8 profid, UINT8 kit_number, UINT8 slot)
{
	return &Item[ gMercProfileGear[profid][kit_number].inv[slot] ];
}

//set the specified slot
UINT16 GearSetSlot(UINT8 profid, UINT8 kit_number, UINT8 slot, INVTYPE* item, UINT8 quantity = 1, UINT8 status = 100)
{
	if (item == NULL)
		return 0;

	gMercProfileGear[profid][kit_number].inv[slot] = item->uiIndex;
	gMercProfileGear[profid][kit_number].iStatus[slot] = status;
	gMercProfileGear[profid][kit_number].iNumber[slot] = quantity;
	gMercProfileGear[profid][kit_number].iDrop[slot] = 0;

	return item->usPrice;
}

//generate a random LBE item (this is mostly used as a parachute function)
INVTYPE* GearGetRandomLBE(INT8 min_coolness = -1, INT8 max_coolness = -1, UINT8 lbe_type = -1)
{
	std::vector<INVTYPE*> candidates;

	for (std::vector<INVTYPE*>::iterator it = gear_lbe.begin(); it != gear_lbe.end(); it++)
	{
		LBETYPE* lbe = &LoadBearingEquipment[(*it)->ubClassIndex];
		if (lbe == NULL)
			return NULL;

		if (HasItemFlag((*it)->uiIndex, SCUBA_BOTTLE))
			continue;

		if (min_coolness > -1 && (*it)->ubCoolness < min_coolness) continue;
		if (max_coolness > -1 && (*it)->ubCoolness > max_coolness) continue;
		if (lbe_type != -1 && lbe->lbeClass	!= lbe_type) continue;

		candidates.push_back(*it);
	}

	if (candidates.size() < 1)
		return NULL;

	return candidates[Random(candidates.size())];
}

//generate a random face item for our merc
INVTYPE* GearGetRandomFaceItem(INT8 min_coolness = -1, INT8 max_coolness = -1, UINT8 item_type = -1)
{
	std::vector<INVTYPE*> candidates;

	for (std::vector<INVTYPE*>::iterator it = gear_faceitems.begin(); it != gear_faceitems.end(); it++)
	{
		if (min_coolness > -1 && (*it)->ubCoolness < min_coolness) continue;
		if (max_coolness > -1 && (*it)->ubCoolness > max_coolness) continue;

		if (HasItemFlag((*it)->uiIndex, SCUBA_MASK))
			continue;

		if (item_type == FACEITEM_GAS_MASK && (*it)->gasmask < 1) continue;
		if (item_type == FACEITEM_SPECTACLES && (*it)->brightlightvisionrangebonus < 1) continue;
		if (item_type == FACEITEM_NVG && (*it)->nightvisionrangebonus < 1) continue;
		if (item_type == FACEITEM_HEADSET && (*it)->hearingrangebonus < 1) continue;

		candidates.push_back(*it);
	}

	if (candidates.size() < 1)
		return NULL;

	return candidates[Random(candidates.size())];
}

//generate a random armor item based on slot and camo type selected (the idea is all 3 pieces of armor should have similar camo)
INVTYPE* GearGetRandomArmor(std::vector<INVTYPE*>* pointer, INT8 min_coolness = -1, INT8 max_coolness = -1, INT8 preferred_camo = -1, INT8 min_camo = -1)
{
	if (pointer == NULL)
		return NULL;

	//safety net for low LBE counts (including 1.13 vanilla!)
	if (pointer->size() < 20)
		return pointer->at(Random(pointer->size()));

	std::vector<INVTYPE*> candidates;

	for (std::vector<INVTYPE*>::iterator it = pointer->begin(); it != pointer->end(); it++)
	{
		if (min_coolness > -1 && (*it)->ubCoolness < min_coolness) continue;
		if (max_coolness > -1 && (*it)->ubCoolness > max_coolness) continue;

		if (HasItemFlag((*it)->uiIndex, SCUBA_BOTTLE))
			continue;

		if (HasItemFlag((*it)->uiIndex, SCUBA_MASK))
			continue;

		if (HasItemFlag((*it)->uiIndex, SCUBA_FINS))
			continue;

		ARMOURTYPE* armor = &Armour[(*it)->ubClassIndex];
		if (armor == NULL)
			break;

		if (preferred_camo == CAMO_ARMOR && armor->ubProtection < min_camo) continue;
		if (preferred_camo == CAMO_URBAN && (*it)->urbanCamobonus < min_camo) continue;
		if (preferred_camo == CAMO_WOODLAND && (*it)->camobonus < min_camo) continue;
		if (preferred_camo == CAMO_DESERT && (*it)->desertCamobonus < min_camo) continue;
		if (preferred_camo == CAMO_STEALTH && (*it)->stealthbonus < min_camo) continue;
		
		candidates.push_back(*it);
	}

	if (candidates.size() < 1)
		return NULL;

	return candidates[ Random( candidates.size() ) ];
}

//generate a random weapon of requested type, class and BR coolness
INVTYPE* GearGetRandomWeapon(INT8 min_coolness = -1, INT8 max_coolness = -1, INT8 weapon_type = -1, INT8 weapon_class = -1)
{
	std::vector<INVTYPE*> candidates;

	for (std::vector<INVTYPE*>::iterator it = gear_weapons.begin(); it != gear_weapons.end(); it++)
	{
		WEAPONTYPE* weapon = &Weapon[(*it)->ubClassIndex];
		if (weapon == NULL)
			return NULL;

		if (min_coolness > -1 && (*it)->ubCoolness < min_coolness) continue;
		if (max_coolness > -1 && (*it)->ubCoolness > max_coolness) continue;
		if (weapon_type != -1 && weapon->ubWeaponType != weapon_type) continue;
		if (weapon_class != -1 && weapon->ubWeaponClass	!= weapon_class) continue;

		candidates.push_back(*it);
	}

	if (candidates.size() < 1)
		return NULL;

	return candidates[Random(candidates.size())];
}

//generate random weapon, but take the merc's role into account. snipers get sniper rifles, etc.
INVTYPE* GearGetRandomWeaponForRole(std::vector<UINT8>* roles, std::vector<UINT8>* traits, INT8 min_coolness = -1, INT8 max_coolness = -1)
{
	if (!traits || traits->size() < 1) //guy has NO traits. just give him a pistol or something
		return GearGetRandomWeapon(min_coolness, max_coolness, GUN_PISTOL, -1);

	UINT8 tries_left = 5;

	while (tries_left-- > 0)
	{
		UINT8 random_trait = traits->at( Random(traits->size()) );

		switch (random_trait)
		{
			case AUTO_WEAPONS_NT:
				if (HasTrait(traits, AUTO_WEAPONS_NT) > 1 && Random(10) < 4)
					return GearGetRandomWeapon(min_coolness, max_coolness, GUN_LMG, -1);
				else
					return GearGetRandomWeapon(min_coolness, max_coolness, GUN_AS_RIFLE, -1);
				break;

			case SNIPER_NT:
				return GearGetRandomWeapon(min_coolness, max_coolness, GUN_SN_RIFLE, -1);
				break;

			case RANGER_NT:
				if (Random(2) < 1)
					return GearGetRandomWeapon(min_coolness, max_coolness, GUN_SHOTGUN, -1);
				else
					return GearGetRandomWeapon(min_coolness, max_coolness, GUN_RIFLE, -1);
				break;

			case GUNSLINGER_NT:
				return GearGetRandomWeapon(min_coolness + 1, max_coolness + 1, GUN_PISTOL, -1);
				break;
		}
	}

	//fall through to random selection
	UINT random_weapon = Random(10);

	if (random_weapon < 4)
		return GearGetRandomWeapon(min_coolness, max_coolness, GUN_AS_RIFLE, -1);
	else if (random_weapon < 7)
		return GearGetRandomWeapon(min_coolness, max_coolness, GUN_SMG, -1);
	else
		return GearGetRandomWeapon(min_coolness, max_coolness, GUN_SHOTGUN, -1);
}

//find magazine type that suits our weapon
INVTYPE* GearFindAmmoForWeapon(WEAPONTYPE* weapon)
{
	if (!weapon)
		return NULL;

	std::vector<INVTYPE*> candidates;

	for (std::vector<INVTYPE*>::iterator it = gear_magazines.begin(); it != gear_magazines.end(); it++)
	{
		MAGTYPE* mag = &Magazine[(*it)->ubClassIndex];

		if (mag->ubMagType != AMMO_MAGAZINE)
			continue;

		AMMOTYPE* at = &AmmoTypes[mag->ubAmmoType];
		if (at)
		{
			if (at->standardIssue < 1 || at->highExplosive > 0)
				continue;
		}

		if (mag->ubCalibre == weapon->ubCalibre)
		{
			if (mag->ubMagSize == weapon->ubMagSize)
			{
				candidates.push_back(*it);
			}
		}
	}

	if (candidates.size() < 1)
		return NULL;

	return candidates[Random(candidates.size())];
}

//find attachments that match our weapon, that we are allowed to select
std::vector<INVTYPE*> GearFindAttachmentsForWeapon(INVTYPE* weapon, BOOL attach_grip, BOOL attach_muzzle, BOOL attach_laser, BOOL attach_sight, BOOL attach_underbarrel)
{
	std::vector<INVTYPE*> result;
	std::vector<INVTYPE*> candidates[5];

	if (!weapon)
		return result;

	for (std::vector<INVTYPE*>::iterator it = gear_attachments.begin(); it != gear_attachments.end(); it++)
	{
		INVTYPE* item = *it;

		if (item->attachment < 1)
			continue;

		if (!ValidAttachment(item->uiIndex, weapon->uiIndex))
			continue;

		if (attach_grip			 && item->attachmentclass == AC_BIPOD)			candidates[0].push_back(item);			
		if (attach_grip			 && item->attachmentclass == AC_FOREGRIP)		candidates[0].push_back(item);		
		if (attach_muzzle		 && item->attachmentclass == AC_MUZZLE)			candidates[1].push_back(item);
		if (attach_laser		 && item->attachmentclass == AC_LASER)			candidates[2].push_back(item);
		if (attach_sight		 && item->attachmentclass == AC_SIGHT)			candidates[3].push_back(item);
		if (attach_sight		 && item->attachmentclass == AC_SCOPE)			candidates[3].push_back(item);
		if (attach_underbarrel   && item->attachmentclass == AC_UNDERBARREL)	candidates[4].push_back(item);
	}

	for (int i = 0; i < 5; i++)
	{
		if (candidates[i].size() > 0)
		{
			result.push_back(candidates[i][ Random(candidates[i].size()) ]);
		}
	}

	return result;
}

//find items that this role could use (medkits for doctors, etc)
std::vector<INVTYPE*> GearFindItemsForRole(std::vector<UINT8>* roles, std::vector<UINT8>* traits)
{
	std::vector<INVTYPE*> result;
	INT8 tracker[9] = {0};

	for (std::vector<INVTYPE*>::iterator it = gear_misc.begin(); it != gear_misc.end(); it++)
	{
		INVTYPE* item = *it;
		INT8 chance = 0;

		if (item->glgrenade > 0)
			continue;

		if (item->attachmentclass == AC_GRENADE || item->attachmentclass == AC_ROCKET)
			continue;

		chance = Random(100) - (HasTrait(traits, MARTIAL_ARTS_NT) * 25) - 2;
		if (item->usItemClass == IC_PUNCH && chance < 0 && tracker[0] < 1) { result.push_back(item); tracker[0]++; }

		chance = Random(100) - (HasTrait(traits, MELEE_NT) * 25) - 2;
		if (item->usItemClass == IC_BLADE && chance < 0 && tracker[1] < 1) { result.push_back(item); tracker[1]++; }

		chance = Random(100) - (HasTrait(traits, THROWING_NT) * 25) - 2;
		if (item->usItemClass == IC_THROWING_KNIFE && chance < 0 && tracker[2] < 2) { result.push_back(item); tracker[2]++; }

		chance = Random(100) - (HasTrait(traits, DEMOLITIONS_NT) * 45) - 1;
		if (item->usItemClass == IC_BOMB && chance < 0 && tracker[3] < 1) { result.push_back(item); tracker[3]++; }

		chance = Random(100) - (HasTrait(traits, DOCTOR_NT) * 40) - 1;
		if (item->usItemClass == IC_MEDKIT && chance < 0 && tracker[4] < 2) { result.push_back(item); tracker[4]++; }

		chance = Random(100) - (HasTrait(traits, TECHNICIAN_NT) * 40) - 1;
		if (item->usItemClass == IC_KIT && chance < 0 && tracker[5] < 2) { result.push_back(item); tracker[5]++; }
			
		chance = Random(100) - 3;
		if (item->usItemClass == IC_APPLIABLE && chance < 0 && tracker[6] < 2) { result.push_back(item); tracker[6]++; }

		chance = Random(100) - 3;
		if (item->usItemClass == IC_MONEY && chance < 0 && tracker[7] < 1) { result.push_back(item); tracker[7]++; }

		chance = Random(100) - (HasTrait(traits, DEMOLITIONS_NT) * 30) - 5;
		if (item->usItemClass == IC_GRENADE && chance < 0 && tracker[8] < 3) { result.push_back(item); tracker[8]++; }
	}

	return result;
}

//LBE helper stuff
UINT16 LBECountPocketsOfSize(LBETYPE* lbe, UINT16 size)
{
	UINT16 count = 0;

	for (std::vector<UINT8>::iterator it = lbe->lbePocketIndex.begin(); it != lbe->lbePocketIndex.end(); it++)
	{
		count += LBEPocketType[*it].ItemCapacityPerSize[size];
	}

	return count;
}

//LBE helper stuff
UINT16 LBEGetItemSize(INVTYPE* item)
{
	if (!item)
		return 0;

	return item->ItemSize;
}

//attempt to find an LBE item that can fit all the specified items at once
INVTYPE* LBEFindSuitableLBE(std::map<UINT16, UINT16> storage, UINT16 lbe_type)
{
	std::vector<INVTYPE*> candidates;

	for (std::vector<INVTYPE*>::iterator it = gear_lbe.begin(); it != gear_lbe.end(); it++)
	{
		LBETYPE* lbe = &LoadBearingEquipment[(*it)->ubClassIndex];

		if (lbe->lbeClass != lbe_type)
			continue;

		if (lbe)
		{
			bool valid = true;

			for (std::map<UINT16, UINT16>::iterator sub_it = storage.begin(); sub_it != storage.end(); sub_it++)
			{
				if (LBECountPocketsOfSize(lbe, sub_it->first) < sub_it->second)
					valid = false;
			}

			if (valid)
			{
				candidates.push_back(*it);
			}
		}
	}

	if (candidates.size() < 1)
		return NULL;

	return candidates[Random(candidates.size())];
}

//this is the main randomization function that calls almost all the other functions.
void RandomizeMerc(UINT8 profile_id, MERCPROFILESTRUCT* merc, BOOL random_gear_kits)
{
	//reset merc's stats to basic values so we can increase them
	merc->bExpLevel = 1;
	merc->bAgility = 1;
	merc->bLifeMax = 1; merc->bLife = merc->bLifeMax;
	merc->bDexterity = 1;
	merc->bStrength = 1;
	merc->bWisdom = 1;
	merc->bMarksmanship = 1;
	merc->bLeadership = 0;
	merc->bMechanical = 0;
	merc->bExplosive = 0;
	merc->bMedical = 0;
	memset(merc->bSkillTraits, 0, sizeof(merc->bSkillTraits));

	//determine character level and points (absolute maximum = 666 points)
	merc->bExpLevel = Random(9) + 1;
	INT16 points = 300 + (merc->bExpLevel * 10) + Random(20);

	//determine roles and traits
	UINT8 roles_to_grant = 1 + Random((UINT16)((float)merc->bExpLevel / 2.0f));
	if (roles_to_grant > 3) roles_to_grant = 3;
	UINT8 max_major_traits = 2 + Random((UINT16)((float)merc->bExpLevel / 2.0f)) + Random(2); if (max_major_traits > 5) max_major_traits = 5;
	UINT8 max_minor_traits = 2 + Random((UINT16)((float)merc->bExpLevel / 2.0f)) + Random(2); if (max_minor_traits > 3) max_minor_traits = 3;
	FLOAT max_combat_traits = 4.5f;

	std::vector<UINT8> roles = GrantRoles(roles_to_grant);
	std::vector<UINT8> traits;
	
	//spend our points budget
	while (points > 0)
	{
		UINT8 decider = Random(100);

		if (decider < 30) //30% chance to assign a totally random stat
		{
			UINT8 result = IncreaseStat(merc, GetRandomNumberForRole(roles[ROLE_GENERIC]));
			if (result > 0)
				points -= result;
		}
		else //business as usual, find a random role we have and assign a point to one of its attributes
		{
			UINT8 role_to_increase = Random(roles.size());
			UINT8 result = IncreaseStat(merc, GetRandomNumberForRole(roles[role_to_increase]));
			if (result > 0)
				points -= result;
		}
	}

	//test for major traits
	for (int i = 1; i < 10; i++)
	{
		UINT8 trait = TestForTrait(i, merc, &roles, &traits);
		if (trait > 0 && traits.size() < max_major_traits && CountCombatTraits(&traits) < max_combat_traits)
		{
			traits.push_back(trait);

			trait = TestForTrait(i, merc, &roles, &traits); //test for second level
			if (trait > 0 && traits.size() < max_major_traits && CountCombatTraits(&traits) < max_combat_traits)
			{
				traits.push_back(trait);
			}
		}
	}

	//test for minor traits
	for (int i = 10; i < 20; i++) { UINT8 trait = TestForTrait(i, merc, &roles, &traits);	if (trait > 0 && traits.size() < max_minor_traits && CountCombatTraits(&traits) < max_combat_traits) { traits.push_back(trait); } }

	//assign us the traits we earned
	UINT8 num_traits = 0;
	for (std::vector<UINT8>::iterator it = traits.begin(); it != traits.end(); it++)
	{
		merc->bSkillTraits[num_traits++] = *it;
	}

	//make sure some stats are at a BARE minimum
	if (merc->bLifeMax < 60) { merc->bLifeMax = 60 + Random(20); merc->bLife = merc->bLifeMax; }
	if (merc->bAgility < 60) merc->bAgility = 60 + Random(20);
	if (merc->bStrength < 40) merc->bStrength = 40 + Random(30);
	if (merc->bMarksmanship < 60) merc->bMarksmanship = 60 + Random(20);
	if (merc->bDexterity < 40) merc->bDexterity = 40 + Random(20);
	if (merc->bWisdom < 50) merc->bWisdom = 50 + Random(20);

	//calculate how much this soldier will make
	FLOAT salary = -100;

	salary += (merc->bExpLevel - 1) * 150;
	salary += merc->bAgility * 1.2f;
	salary += merc->bLifeMax * 1.0f;
	salary += merc->bDexterity * 1.0f;
	salary += merc->bStrength * 1.0f;
	salary += merc->bWisdom * 1.0f;
	salary += merc->bMarksmanship * 1.3f;
	salary += merc->bLeadership * 1.0f;
	salary += merc->bMechanical * 1.0f;
	salary += merc->bExplosive * 1.0f;
	salary += merc->bMedical * 1.0f;
	salary += traits.size() * 40.0f;
	salary += CountCombatTraits(&traits) * 50.0f; //combat traits count double.

	merc->sSalary = (INT16)salary;
	merc->sTrueSalary = merc->sSalary;
	merc->uiWeeklySalary = (UINT32)(salary * 6.0f);
	merc->uiBiWeeklySalary = (UINT32)(salary * 11.0f);
	merc->sMedicalDepositAmount = 0; //disabled now.
	merc->bMedicalDeposit = false;

	if (random_gear_kits)
	{
		//generate gear kits for the merc
		for (UINT8 kit_id = 0;kit_id < 5; kit_id++)
		{
			std::map<UINT16, UINT16> storage[3]; //this will hold all the items we need to store in LBE.

			GearKitClear(profile_id, kit_id);

			UINT16 kit_cost = 0;
			INT8 min_kit_level = (kit_id / 2) + (merc->bExpLevel / 4); if (min_kit_level < 1) min_kit_level = 1;
			INT8 max_kit_level = 1 + (kit_id) + (merc->bExpLevel / 4); if (max_kit_level > 7) max_kit_level = 7;

			//assign armor
			UINT8 preferred_camo = Random(5) + 1;
			kit_cost += GearSetSlot(profile_id, kit_id, HELMETPOS, GearGetRandomArmor(&gear_helmets, min_kit_level, max_kit_level, preferred_camo, (preferred_camo == CAMO_ARMOR) ? 10 : 5));
			kit_cost += GearSetSlot(profile_id, kit_id, VESTPOS, GearGetRandomArmor(&gear_vests, min_kit_level, max_kit_level, preferred_camo, (preferred_camo == CAMO_ARMOR) ? 25 : 5));
			kit_cost += GearSetSlot(profile_id, kit_id, LEGPOS, GearGetRandomArmor(&gear_leggings, min_kit_level, max_kit_level, preferred_camo, (preferred_camo == CAMO_ARMOR) ? 10 : 5));

			UINT8 head1 = Random(24) - (merc->bExpLevel / 3) - max_kit_level;
			if (head1 < 1)
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD1POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_GAS_MASK));
			else if (head1 < 4)
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD1POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_HEADSET));
			else if (HasRole(&roles, ROLE_LEADER) && head1 < 15) //leaders get an extra chance to get a headset
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD1POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_HEADSET));

			//assign face gear
			UINT8 head2 = Random(24) - (merc->bExpLevel / 3) - max_kit_level;
			if (head2 < 1)
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD2POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_NVG));
			else if (HasTrait(&traits, STEALTHY_NT) && head2 < 3)
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD2POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_NVG));
			else if (HasTrait(&traits, NIGHT_OPS_NT) && head2 < 5)
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD2POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_NVG));
			else if (head2 < 5)
				kit_cost += GearSetSlot(profile_id, kit_id, HEAD2POS, GearGetRandomFaceItem(min_kit_level, max_kit_level, FACEITEM_SPECTACLES));

			if (kit_id == 0) //kit 1 is always a pistol
			{
				kit_cost += GearSetSlot(profile_id, kit_id, HANDPOS, GearGetRandomWeapon(min_kit_level, max_kit_level, GUN_PISTOL, -1));
			}
			else if (kit_id == 1 || kit_id == 2) //kit 2 and 3 are randomly a shotgun, machinepistol or smg
			{
				UINT8 rnd = Random(2);

				if (rnd == 1)
					kit_cost += GearSetSlot(profile_id, kit_id, HANDPOS, GearGetRandomWeapon(min_kit_level, max_kit_level - 1, GUN_SHOTGUN, -1));
				else if (rnd == 2)
					kit_cost += GearSetSlot(profile_id, kit_id, HANDPOS, GearGetRandomWeapon(min_kit_level, max_kit_level - 1, GUN_M_PISTOL, -1));
				else
					kit_cost += GearSetSlot(profile_id, kit_id, HANDPOS, GearGetRandomWeapon(min_kit_level, max_kit_level - 1, GUN_SMG, -1));
			}
			else //last 2 kits hold role specific weapons if applicable.
			{
				kit_cost += GearSetSlot(profile_id, kit_id, HANDPOS, GearGetRandomWeaponForRole(&roles, &traits, min_kit_level, max_kit_level));
			}

			//deal out potential sidearm, ammo and attachments to the main weapon
			INVTYPE* hand_item = GearGetSlot(profile_id, kit_id, HANDPOS);

			if (hand_item)
			{
				WEAPONTYPE* weapon = &Weapon[hand_item->ubClassIndex];
				BOOL got_second_pistol = false;

				//ambidexterity has a chance of giving them a second pistol, if they have one.
				if (weapon->ubWeaponType == GUN_PISTOL && Random(5) < 3 && HasTrait(&traits, AMBIDEXTROUS_NT))
				{
					kit_cost += GearSetSlot(profile_id, kit_id, SECONDHANDPOS, hand_item);
					got_second_pistol = true;
				}

				//ammo
				INVTYPE* ammo = GearFindAmmoForWeapon(weapon);
				if (ammo)
				{
					MAGTYPE* mag = &Magazine[ammo->ubClassIndex];
					UINT8 bullets = (UINT8)(Random(50) + 50) + ((got_second_pistol) ? 30 : 0);
					UINT8 mags = (UINT8)(bullets / mag->ubMagSize); if (mags < 1) mags = 1; if (mags > 8) mags = 8;
					UINT8 mags_per_slot = (UINT8)(60 / mag->ubMagSize); if (mags_per_slot < 1) mags_per_slot = 1; if (mags_per_slot > 4) mags_per_slot = 4;
					UINT8 pos = 0;
					UINT8 runs = 0;

					while (mags > 0)
					{
						if (mags < mags_per_slot)
							mags_per_slot = mags;

						kit_cost += GearSetSlot(profile_id, kit_id, SMALLPOCK17POS + pos, ammo, mags_per_slot); 
						for (int i = 0; i < mags_per_slot; i++) { storage[0][ LBEGetItemSize(ammo) ]++; }

						mags -= mags_per_slot;
						pos++;
						runs++;

						if (runs > 2)
							break;
					}
				}

				//sidearm for snipers/rifle users (50% chance), and sometimes to just someone randomly (10% chance).
				if (((weapon->ubWeaponType == GUN_SN_RIFLE || weapon->ubWeaponType == GUN_SN_RIFLE) && Random(2) < 1) || (Random(10) < 1 && weapon->ubWeaponType != GUN_PISTOL))
				{
					INVTYPE* pistol = GearGetRandomWeapon(min_kit_level, max_kit_level - 1, GUN_PISTOL, -1);

					if (pistol)
					{
						kit_cost += GearSetSlot(profile_id, kit_id, SECONDHANDPOS, pistol);
						storage[1][ LBEGetItemSize(pistol) ]++;

						weapon = &Weapon[pistol->ubClassIndex];
						ammo = GearFindAmmoForWeapon(weapon);

						if (ammo)
						{
							kit_cost += GearSetSlot(profile_id, kit_id, MEDPOCK2POS, ammo, 2); 
							for (int i = 0; i < 2; i++) { storage[0][ LBEGetItemSize(ammo) ]++; }
						}
					}
				}

				//attachments
				INT8 attach_grip = (BOOL)(Random(8) - Random((merc->bExpLevel / 2)) - Random(kit_id) - 1);
				INT8 attach_muzzle = (BOOL)(Random(20) - Random((merc->bExpLevel / 2)) - Random(kit_id) - 0);
				INT8 attach_laser = (BOOL)(Random(15) - Random((merc->bExpLevel / 2)) - Random(kit_id) - 1);
				INT8 attach_sight = (BOOL)(Random(10) - Random((merc->bExpLevel / 2)) - Random(kit_id) - 1);
				INT8 attach_underbarrel = (BOOL)(Random(15) - Random((merc->bExpLevel / 2)) - Random(kit_id) - 0);
				std::vector<INVTYPE*> attachments = GearFindAttachmentsForWeapon(hand_item, (BOOL)(attach_grip < 0), (BOOL)(attach_muzzle < 0), (BOOL)(attach_laser < 0), (BOOL)(attach_sight < 0), (BOOL)(attach_underbarrel < 0));

				UINT8 count = 0;
				for (std::vector<INVTYPE*>::iterator it = attachments.begin(); it != attachments.end(); it++)
				{
					kit_cost += GearSetSlot(profile_id, kit_id, SMALLPOCK23POS + count++, (*it));
					storage[2][ LBEGetItemSize(*it) ]++;
				}
			}

			//assign random stuff based on role etc
			std::vector<INVTYPE*> misc_items = GearFindItemsForRole(&roles, &traits);

			UINT8 count = 0;
			for (std::vector<INVTYPE*>::iterator it = misc_items.begin(); it != misc_items.end(); it++)
			{
				kit_cost += GearSetSlot(profile_id, kit_id, SMALLPOCK1POS + count++, (*it));
				storage[1][ LBEGetItemSize(*it) ] += 1;
			}

			//LBE:
			//storage[0] holds the sizes and counts for ammo. => vest
			//storage[1] holds the sizes and counts for potential sidearm and "kits" => combat pack
			//storage[2] holds the sizes and counts for attachments => thigh packs

			INVTYPE* lbe_vest = LBEFindSuitableLBE(storage[0], VEST_PACK);
			INVTYPE* lbe_cpack = LBEFindSuitableLBE(storage[1], COMBAT_PACK);
			INVTYPE* lbe_thigh = LBEFindSuitableLBE(storage[2], THIGH_PACK);

			if (lbe_vest)
				kit_cost += GearSetSlot(profile_id, kit_id, VESTPOCKPOS, lbe_vest);

			if (lbe_cpack)
				kit_cost += GearSetSlot(profile_id, kit_id, CPACKPOCKPOS, lbe_cpack);

			if (lbe_thigh)
				kit_cost += GearSetSlot(profile_id, kit_id, LTHIGHPOCKPOS, lbe_thigh);

			//sometimes we get the second thigh slot
			if ((Random(10) - max_kit_level) < 5) kit_cost += GearSetSlot(profile_id, kit_id, RTHIGHPOCKPOS, GearGetRandomLBE(min_kit_level, max_kit_level, THIGH_PACK));

			//tally up costs of kit 
			kit_cost = kit_cost * (1 + (max_kit_level / 10));
			GearKitSetPrice(profile_id, kit_id, kit_cost);

			//make sure this is set to the price of the FIRST gear set and you'll be fine.
			if (kit_id == 0)
				merc->usOptionalGearCost = kit_cost; 
		}
	}
}