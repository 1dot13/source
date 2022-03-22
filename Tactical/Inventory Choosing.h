#ifndef __INVENTORY_CHOOSING_H
#define __INVENTORY_CHOOSING_H

#include "types.h"
#include "Soldier Create.h"
#include "Strategic Status.h"

#define MORTAR_GRENADE_CLASS	100
#define RPG_GRENADE_CLASS	101


typedef struct ARMY_GUN_CHOICE_TYPE
{
	UINT8		ubChoices;						// how many valid choices there are in this category
	INT16		bItemNo[50];						// room for up to 50 choices of gun in each category

	UINT32		uiIndex;						
} ARMY_GUN_CHOICE_TYPE;


enum
{
	//administrator ratings (1-5)
	BAD_ADMINISTRATOR_EQUIPMENT_RATING = 1,
	POOR_ADMINISTRATOR_EQUIPMENT_RATING,
	AVERAGE_ADMINISTRATOR_EQUIPMENT_RATING,
	GOOD_ADMINISTRATOR_EQUIPMENT_RATING,
	GREAT_ADMINISTRATOR_EQUIPMENT_RATING,
	//army ratings (3-7)
	BAD_ARMY_EQUIPMENT_RATING = AVERAGE_ADMINISTRATOR_EQUIPMENT_RATING,
	POOR_ARMY_EQUIPMENT_RATING,
	AVERAGE_ARMY_EQUIPMENT_RATING,
	GOOD_ARMY_EQUIPMENT_RATING,
	GREAT_ARMY_EQUIPMENT_RATING,
	//elite ratings (5-9)
	BAD_ELITE_EQUIPMENT_RATING = AVERAGE_ARMY_EQUIPMENT_RATING,
	POOR_ELITE_EQUIPMENT_RATING,
	AVERAGE_ELITE_EQUIPMENT_RATING,
	GOOD_ELITE_EQUIPMENT_RATING,
	GREAT_ELITE_EQUIPMENT_RATING,

	// militia ratings (1-5)
	BAD_MILITIA_EQUIPMENT_RATING = BAD_ADMINISTRATOR_EQUIPMENT_RATING,
	POOR_MILITIA_EQUIPMENT_RATING,
	AVERAGE_MILITIA_EQUIPMENT_RATING,
	GOOD_MILITIA_EQUIPMENT_RATING,
	GREAT_MILITIA_EQUIPMENT_RATING,
};

#define MIN_EQUIPMENT_CLASS				1
#define MAX_EQUIPMENT_CLASS				11


// Selects at the start of the game the set of guns the Queen's army will use during this game
void InitArmyGunTypes(void);

//Chooses equipment based on the equipment level (1-11) with best being 11.	It allocates a range 
//of equipment to choose from.
void GenerateRandomEquipment( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass, INT8 bEquipmentRating);

void ChooseWeaponForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bWeaponClass, INT8 bAmmoClips, INT8 bAttachClass, BOOLEAN fAttachment );

void ChooseGrenadesForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bGrenades, INT8 bGrenadeClass, BOOLEAN fGrenadeLauncher );

void ChooseArmourForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bHelmetClass, INT8 bVestClass, INT8 bLeggingsClass );

void ChooseSpecialWeaponsForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, INT8 bKnifeClass, BOOLEAN fGrenadeLauncher, BOOLEAN fLAW, BOOLEAN fMortar, BOOLEAN fRPG );

void ChooseFaceGearForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp );

void ChooseLocationSpecificGearForSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp );

BOOLEAN PlaceObjectInSoldierCreateStruct( SOLDIERCREATE_STRUCT *pp, OBJECTTYPE *pObject );

void AssignCreatureInventory( SOLDIERTYPE *pSoldier );

void ReplaceExtendedGuns( SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass );

UINT16 SelectStandardArmyGun( UINT8 uiGunLevel, INT8 bSoldierClass );

INT8 GetWeaponClass( UINT16 usGun );
void MarkAllWeaponsOfSameGunClassAsDropped( UINT16 usWeapon );

void ResetMortarsOnTeamCount( void );

// Flugente: created separate gun choices for different soldier classes
extern ARMY_GUN_CHOICE_TYPE gExtendedArmyGunChoices[SOLDIER_GUN_CHOICE_SELECTIONS][ARMY_GUN_LEVELS];

// Headrock: Added LBE enumeration
enum
{
	HELMET = 0,
	VEST,
	LEGS,
	GRENADE,
	KIT,
	BOMB,
	MISCITEMS,
	GRENADELAUNCHER,
	SINGLESHOTROCKETLAUNCHER,
	ROCKETLAUNCHER,
	MORTARLAUNCHER,
	ARMOURATTACHMENT,
	GASMASKS,
	NVGLOW,
	NVGHIGH,
	HEARINGAIDS,
	SCOPE,
	ATTACHMENTS,
	KNIVES,
	ENEMYAMMOTYPES,
	LBE,
	MAX_ITEM_TYPES
};

// Flugente: created separate gun choices for different soldier classes
extern ARMY_GUN_CHOICE_TYPE gArmyItemChoices[SOLDIER_GUN_CHOICE_SELECTIONS][MAX_ITEM_TYPES];

////////////////// Flugente: militia equipment feature ///////////////////////////////////
// move one set of militia equipment between sectors
void MoveOneMilitiaEquipmentSet(INT16 sSourceX, INT16 sSourceY, INT16 sTargetX, INT16 sTargetY, INT8 bSoldierClass);

// move a number of militia equipment sets between sectors
void MoveMilitiaEquipment(INT16 sSourceX, INT16 sSourceY, INT16 sTargetX, INT16 sTargetY, UINT8 usElites, UINT8 usRegulars, UINT8 usGreens);

// take militia items from sector
void TakeMilitiaEquipmentfromSector( INT16 sMapX, INT16 sMapY, INT8 sMapZ, SOLDIERCREATE_STRUCT *pp, INT8 bSoldierClass );

#endif