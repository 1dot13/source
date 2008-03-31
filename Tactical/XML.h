#ifndef __XML_H
#define __XML_H

#include "armsdealerinvinit.h"
#include "EnemyItemDrops.h"
#include "Loading Screen.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


enum
{
	ELEMENT_NONE = 0,
	ELEMENT_LIST,
	ELEMENT,
	ELEMENT_PROPERTY,
}
typedef PARSE_STAGE;

#define MAX_CHAR_DATA_LENGTH			500

#define TABLEDATA_DIRECTORY						"TableData\\"

#define GERMAN_PREFIX							"German."
#define RUSSIAN_PREFIX							"Russian."
#define DUTCH_PREFIX							"Dutch."
#define POLISH_PREFIX							"Polish."
#define FRENCH_PREFIX							"French."
#define ITALIAN_PREFIX							"Italian."
#define TAIWANESE_PREFIX						"Tiawanese."

#define	ATTACHMENTSFILENAME						"Attachments.xml"
#define	ATTACHMENTINFOFILENAME					"AttachmentInfo.xml"
#define	ITEMSFILENAME							"Items.xml"
#define	WEAPONSFILENAME							"Weapons.xml"
#define	LAUNCHABLESFILENAME						"Launchables.xml"
#define	COMPATIBLEFACEITEMSFILENAME				"CompatibleFaceItems.xml"
#define	MERGESFILENAME							"Merges.xml"
#define	ATTACHMENTCOMBOMERGESFILENAME			"AttachmentComboMerges.xml"
#define	MAGAZINESFILENAME						"Magazines.xml"
#define	ARMOURSFILENAME							"Armours.xml"
#define	EXPLOSIVESFILENAME						"Explosives.xml"
#define	AMMOFILENAME							"AmmoStrings.xml"
#define	AMMOTYPESFILENAME						"AmmoTypes.xml"
#define	INCOMPATIBLEATTACHMENTSFILENAME			"IncompatibleAttachments.xml"
#define	ENEMYGUNCHOICESFILENAME					"EnemyGunChoices.xml"
#define	ENEMYITEMCHOICESFILENAME				"EnemyItemChoices.xml"
#define	IMPITEMCHOICESFILENAME					"IMPItemChoices.xml"

// CHRISL:
#define LOADBEARINGEQUIPMENTFILENAME			"LoadBearingEquipment.xml"
#define LBEPOCKETFILENAME						"Pockets.xml"
#define MERCSTARTINGGEARFILENAME				"MercStartingGear.xml"

#define TONYINVENTORYFILENAME					"NPCInventory\\TonyInventory.xml"
#define DEVININVENTORYFILENAME					"NPCInventory\\DevinInventory.xml"
#define FRANZINVENTORYFILENAME					"NPCInventory\\FranzInventory.xml"
#define KEITHINVENTORYFILENAME					"NPCInventory\\KeithInventory.xml"
#define SAMINVENTORYFILENAME					"NPCInventory\\SamInventory.xml"
#define JAKEINVENTORYFILENAME					"NPCInventory\\JakeInventory.xml"
#define HOWARDINVENTORYFILENAME				 "NPCInventory\\HowardInventory.xml"
#define GABBYINVENTORYFILENAME					"NPCInventory\\GabbyInventory.xml"
#define FRANKINVENTORYFILENAME					"NPCInventory\\FrankInventory.xml"
#define ELGININVENTORYFILENAME					"NPCInventory\\ElginInventory.xml"
#define MANNYINVENTORYFILENAME					"NPCInventory\\MannyInventory.xml"
#define HERVEINVENTORYFILENAME					"NPCInventory\\HerveInventory.xml"
#define PETERINVENTORYFILENAME					"NPCInventory\\PeterInventory.xml"
#define ALBERTOINVENTORYFILENAME				"NPCInventory\\AlbertoInventory.xml"
#define CARLOINVENTORYFILENAME					"NPCInventory\\CarloInventory.xml"
#define MICKEYINVENTORYFILENAME				 "NPCInventory\\MickeyInventory.xml"
#define ARNIEINVENTORYFILENAME					"NPCInventory\\ArnieInventory.xml"
#define PERKOINVENTORYFILENAME					"NPCInventory\\PerkoInventory.xml"
#define FREDOINVENTORYFILENAME					"NPCInventory\\FredoInventory.xml"

#define BOBBYRAYSTRINGSFILENAME				 "BobbyRayStrings.xml"
#define AMMOCALIBERSTRINGSFILENAME				"AmmoCaliberStrings.xml"

#define SOUNDSFILENAME							"Sounds\\Sounds.xml"
#define BURSTSOUNDSFILENAME					 "Sounds\\BurstSounds.xml"

#define EXPLOSIONDATAFILENAME					"ExplosionData.xml"

#define CITYTABLEFILENAME						"Map\\Cities.xml"
#define MOVEMENTCOSTFILENAME					"Map\\MovementCosts.xml"
#define ALTSECTORSFILENAME						"Map\\AltSectors.xml"
#define SAMSITESFILENAME						"Map\\SamSites.xml"
#define ROAMINGMILITIAFILENAME					"Map\\RestrictedRoamingMilitia.xml"


#define GARRISONFILENAME						"Army\\GarrisonGroups.xml"
#define PATROLFILENAME							"Army\\PatrolGroups.xml"
#define COMPOSITIONFILENAME					 "Army\\ArmyComposition.xml"

// WANNE: drops filename
#define ENEMYWEAPONDROPSFILENAME				"EnemyWeaponDrops.xml"
#define ENEMYAMMODROPSFILENAME					"EnemyAmmoDrops.xml"
#define ENEMYEXPLOSIVEDROPSFILENAME			 "EnemyExplosiveDrops.xml"
#define ENEMYARMOURDROPSFILENAME				"EnemyArmourDrops.xml"
#define ENEMYMISCDROPSFILENAME					"EnemyMiscDrops.xml"

// WANNE: Sector loadscreens [2007-05-18]
#define SECTORLOADSCREENSFILENAME				"Map\\SectorLoadscreens.xml"


extern BOOLEAN ReadInItemStats(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteItemStats();

extern BOOLEAN ReadInAttachmentStats(STR fileName);
extern BOOLEAN WriteAttachmentStats();

extern BOOLEAN ReadInAttachmentInfoStats(STR fileName);
extern BOOLEAN WriteAttachmentInfoStats();

extern BOOLEAN ReadInStringArray(STR fileName);
extern BOOLEAN WriteStringArray();

extern BOOLEAN ReadInMagazineStats(STR fileName);
extern BOOLEAN WriteMagazineStats();

extern BOOLEAN ReadInLaunchableStats(STR fileName);
extern BOOLEAN WriteLaunchableStats();

extern BOOLEAN ReadInCompatibleFaceItemStats(STR fileName);
extern BOOLEAN WriteCompatibleFaceItemStats();

extern BOOLEAN ReadInMergeStats(STR fileName);
extern BOOLEAN WriteMergeStats();

extern BOOLEAN ReadInAttachmentComboMergeStats(STR fileName);
extern BOOLEAN WriteAttachmentComboMergeStats();

extern BOOLEAN ReadInArmourStats(STR fileName);
extern BOOLEAN WriteArmourStats();

// CHRISL:
extern BOOLEAN ReadInlbeStats(STR fileName);
extern BOOLEAN WritelbeEquipmentStats();

extern BOOLEAN ReadInLBEPocketStats(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteLBEPocketEquipmentStats();

extern BOOLEAN ReadInMercStartingGearStats(STR fileName);
extern BOOLEAN WriteMercStartingGearStats();

extern BOOLEAN ReadInExplosiveStats(STR fileName);
extern BOOLEAN WriteExplosiveStats();

extern BOOLEAN ReadInAmmoStats(STR fileName);
extern BOOLEAN WriteAmmoStats();

extern BOOLEAN WriteIncompatibleAttachmentStats();
extern BOOLEAN ReadInIncompatibleAttachmentStats(STR fileName);

extern BOOLEAN WriteExtendedArmyGunChoicesStats();
extern BOOLEAN ReadInExtendedArmyGunChoicesStats(STR fileName);

extern BOOLEAN WriteArmyItemChoicesStats();
extern BOOLEAN ReadInArmyItemChoicesStats(STR fileName);

extern BOOLEAN WriteIMPItemChoicesStats();
extern BOOLEAN ReadInIMPItemChoicesStats(STR fileName);

extern BOOLEAN WriteInventoryStats (DEALER_POSSIBLE_INV *pInv, STR fileName);
extern BOOLEAN ReadInInventoryStats (DEALER_POSSIBLE_INV *pInv, STR fileName);

extern BOOLEAN WriteSoundArray();
extern BOOLEAN ReadInSoundArray(STR fileName);

extern BOOLEAN ReadInAmmoTypeStats(STR fileName);
extern BOOLEAN WriteAmmoTypeStats();

// WANNE: Enemy drops
extern BOOLEAN ReadInEnemyWeaponDropsStats(WEAPON_DROPS *pEnemyWeaponDrops, STR fileName);
extern BOOLEAN WriteEnemyWeaponDropsStats(WEAPON_DROPS *pEnemyWeaponDrops, STR fileName);
extern BOOLEAN ReadInEnemyAmmoDropsStats(AMMO_DROPS *pEnemyAmmoDrops, STR fileName);
extern BOOLEAN WriteEnemyAmmoDropsStats(AMMO_DROPS *pEnemyAmmoDrops, STR fileName);
extern BOOLEAN ReadInEnemyExplosiveDropsStats(EXPLOSIVE_DROPS *pEnemyExplosiveDrops, STR fileName);
extern BOOLEAN WriteEnemyExplosiveDropsStats(EXPLOSIVE_DROPS *pEnemyExplosiveDrops, STR fileName);
extern BOOLEAN ReadInEnemyArmourDropsStats(ARMOUR_DROPS *pEnemyArmourDrops, STR fileName);
extern BOOLEAN WriteEnemyArmourDropsStats(ARMOUR_DROPS *pEnemyArmourDrops, STR fileName);
extern BOOLEAN ReadInEnemyMiscDropsStats(MISC_DROPS *pEnemyMiscDrops, STR fileName);
extern BOOLEAN WriteEnemyMiscDropsStats(MISC_DROPS *pEnemyMiscDrops, STR fileName);

// WANNE: Sector Loadscreens [2007-05-18]
extern BOOLEAN ReadInSectorLoadscreensStats(SECTOR_LOADSCREENS *pSectorLoadscreens, STR fileName);
extern BOOLEAN WriteSectorLoadscreensStats(SECTOR_LOADSCREENS *pSectorLoadscreens, STR fileName);

// Lesh: burst sounds and explosion info
extern BOOLEAN ReadInBurstSoundArray(STR fileName);
extern BOOLEAN WriteBurstSoundArray();
extern BOOLEAN ReadInExplosionDataStats(STR fileName);
extern BOOLEAN WriteExplosionDataStats();

//Lesh: strategic movement costs and alternative sectors table
extern BOOLEAN ReadInStrategicMovementCosts(STR fileName);
extern BOOLEAN WriteInStrategicMovementCosts(STR fileName);
extern BOOLEAN ReadInAltSectors(STR fileName);
extern BOOLEAN WriteInAltSectors(STR fileName);

//Lesh: samsite info
extern BOOLEAN ReadInSAMInfo(STR fileName);
extern BOOLEAN WriteInSAMInfo(STR fileName);

// Lesh: army externalization
extern BOOLEAN ReadInGarrisonInfo(STR fileName);
extern BOOLEAN WriteInGarrisonInfo(STR fileName);
extern BOOLEAN ReadInPatrolInfo(STR fileName);
extern BOOLEAN WriteInPatrolInfo(STR fileName);
extern BOOLEAN ReadInArmyCompositionInfo(STR fileName);
extern BOOLEAN WriteInArmyCompositionInfo(STR fileName);

//Kaiden: Roaming Militia Restricted Sectors List
extern BOOLEAN ReadInRoamingInfo(STR filename);
#endif
