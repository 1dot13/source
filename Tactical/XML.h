#ifndef __XML_H
#define __XML_H

#include "armsdealerinvinit.h"
#include "EnemyItemDrops.h"

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

// WANNE: Removed the prefix, because it does not work correct.
#define GERMAN_PREFIX							"German."
#define RUSSIAN_PREFIX							"Russian."


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

#define	TONYINVENTORYFILENAME					"TonyInventory.xml"
#define	DEVININVENTORYFILENAME					"DevinInventory.xml"
#define	FRANZINVENTORYFILENAME					"FranzInventory.xml"
#define	KEITHINVENTORYFILENAME					"KeithInventory.xml"
#define	SAMINVENTORYFILENAME					"SamInventory.xml"
#define	JAKEINVENTORYFILENAME					"JakeInventory.xml"
#define	HOWARDINVENTORYFILENAME					"HowardInventory.xml"
#define	GABBYINVENTORYFILENAME					"GabbyInventory.xml"
#define	FRANKINVENTORYFILENAME					"FrankInventory.xml"
#define	ELGININVENTORYFILENAME					"ElginInventory.xml"
#define	MANNYINVENTORYFILENAME					"MannyInventory.xml"
#define	HERVEINVENTORYFILENAME					"HerveInventory.xml"
#define	PETERINVENTORYFILENAME					"PeterInventory.xml"
#define	ALBERTOINVENTORYFILENAME				"AlbertoInventory.xml"
#define	CARLOINVENTORYFILENAME					"CarloInventory.xml"
#define	MICKEYINVENTORYFILENAME					"MickeyInventory.xml"
#define	ARNIEINVENTORYFILENAME					"ArnieInventory.xml"
#define	PERKOINVENTORYFILENAME					"PerkoInventory.xml"
#define	FREDOINVENTORYFILENAME					"FredoInventory.xml"

#define	BOBBYRAYSTRINGSFILENAME					"BobbyRayStrings.xml"
#define AMMOCALIBERSTRINGSFILENAME				"AmmoCaliberStrings.xml"
#define SOUNDSFILENAME							"Sounds.xml"
#define BURSTSOUNDSFILENAME						"BurstSounds.xml"

// Lesh: filename definition
#define EXPLOSIONDATAFILENAME                   "ExplosionData.xml"
#define CITYTABLEFILENAME "Cities.xml"

// WANNE: drops filename
#define ENEMYWEAPONDROPSFILENAME				"EnemyWeaponDrops.xml"
#define ENEMYAMMODROPSFILENAME					"EnemyAmmoDrops.xml"
#define ENEMYEXPLOSIVEDROPSFILENAME				"EnemyExplosiveDrops.xml"
#define ENEMYARMOURDROPSFILENAME				"EnemyArmourDrops.xml"
#define ENEMYMISCDROPSFILENAME					"EnemyMiscDrops.xml"


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

// Lesh: 2 lines added
extern BOOLEAN ReadInBurstSoundArray(STR fileName);
extern BOOLEAN WriteBurstSoundArray();
// Lesh: over
// Lesh: 2 new lines added for explosion data
extern BOOLEAN ReadInExplosionDataStats(STR fileName);
extern BOOLEAN WriteExplosionDataStats();
// Lesh: over
#endif