#ifndef __XML_H
#define __XML_H

#include "armsdealerinvinit.h"
#include "EnemyItemDrops.h"
#include "Loading Screen.h"
#include "faces.h"
#include "Interface.h"
#include "XML_SenderNameList.h"
#include "mainmenuscreen.h"

#include "ub_config.h"
#include "Strategic Town Loyalty.h"
#include "qarray.h"
#include "XML_Language.h"
#include "Encyclopedia_Data.h"
#include "Encyclopedia.h"
//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


enum
{
	ELEMENT_NONE = 0,
	ELEMENT_LIST,
	ELEMENT,
	ELEMENT_PROPERTY,
	ELEMENT_SUBLIST,
	ELEMENT_SUBLIST_PROPERTY,

	// For new merchant inventory
	MERCHANT_CONTROL,
	MERCHANT_CASH,
	MERCHANT_CASH_DAILY,
	MERCHANT_COOLNESS,
	MERCHANT_FLAGS,
	MERCHANT_REORDER
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
#define TAIWANESE_PREFIX						"Taiwanese."
#define CHINESE_PREFIX							"Chinese."

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
#define	ATTACHMENTSLOTSFILENAME					"AttachmentSlots.xml"
#define	ENEMYGUNCHOICESFILENAME					"EnemyGunChoices.xml"
#define	ENEMYITEMCHOICESFILENAME				"EnemyItemChoices.xml"
#define	IMPITEMCHOICESFILENAME					"IMPItemChoices.xml"

// CHRISL:
#define LOADBEARINGEQUIPMENTFILENAME			"LoadBearingEquipment.xml"
#define LBEPOCKETFILENAME						"Pockets.xml"
#define MERCSTARTINGGEARFILENAME				"MercStartingGear.xml"

#ifdef JA2UB
#define MERCSTARTINGGEAR25FILENAME				"MercStartingGear25.xml"  //ja25 ub
#endif

#define TONYINVENTORYFILENAME					"NPCInventory\\TonyInventory.xml"
#define DEVININVENTORYFILENAME					"NPCInventory\\DevinInventory.xml"
#define FRANZINVENTORYFILENAME					"NPCInventory\\FranzInventory.xml"
#define KEITHINVENTORYFILENAME					"NPCInventory\\KeithInventory.xml"
#define SAMINVENTORYFILENAME					"NPCInventory\\SamInventory.xml"
#define JAKEINVENTORYFILENAME					"NPCInventory\\JakeInventory.xml"
#define HOWARDINVENTORYFILENAME					"NPCInventory\\HowardInventory.xml"
#define GABBYINVENTORYFILENAME					"NPCInventory\\GabbyInventory.xml"
#define FRANKINVENTORYFILENAME					"NPCInventory\\FrankInventory.xml"
#define ELGININVENTORYFILENAME					"NPCInventory\\ElginInventory.xml"
#define MANNYINVENTORYFILENAME					"NPCInventory\\MannyInventory.xml"
#define HERVEINVENTORYFILENAME					"NPCInventory\\HerveInventory.xml"
#define PETERINVENTORYFILENAME					"NPCInventory\\PeterInventory.xml"
#define ALBERTOINVENTORYFILENAME				"NPCInventory\\AlbertoInventory.xml"
#define CARLOINVENTORYFILENAME					"NPCInventory\\CarloInventory.xml"
#define MICKEYINVENTORYFILENAME					"NPCInventory\\MickeyInventory.xml"
#define ARNIEINVENTORYFILENAME					"NPCInventory\\ArnieInventory.xml"
#define PERKOINVENTORYFILENAME					"NPCInventory\\PerkoInventory.xml"
#define FREDOINVENTORYFILENAME					"NPCInventory\\FredoInventory.xml"

#ifdef JA2UB
#define BETTYINVENTORYFILENAME					"NPCInventory\\BettyInventory.xml"
#define RAULINVENTORYFILENAME					"NPCInventory\\RaulInventory.xml"
#endif

#define BOBBYRAYSTRINGSFILENAME					"BobbyRayStrings.xml"
#define AMMOCALIBERSTRINGSFILENAME				"AmmoCaliberStrings.xml"

#define SOUNDSFILENAME							"Sounds\\Sounds.xml"
#define BURSTSOUNDSFILENAME						"Sounds\\BurstSounds.xml"

#define EXPLOSIONDATAFILENAME					"ExplosionData.xml"

#define CITYTABLEFILENAME						"Map\\Cities.xml"
#define MOVEMENTCOSTFILENAME					"Map\\MovementCosts.xml"
#define ALTSECTORSFILENAME						"Map\\AltSectors.xml"
#define SAMSITESFILENAME						"Map\\SamSites.xml"
#define ROAMINGMILITIAFILENAME					"Map\\RestrictedRoamingMilitia.xml"
#define EXTRAITEMSFILENAME						"Map\\A9_0_ExtraItems"	// ".xml" will be added @runtime
#define EXTRAITEMSFILENAME2						"Map\\A11_0_ExtraItems" // ".xml" will be added @runtime

#define GARRISONFILENAME						"Army\\GarrisonGroups.xml"
#define PATROLFILENAME							"Army\\PatrolGroups.xml"
#define COMPOSITIONFILENAME						"Army\\ArmyComposition.xml"

// WANNE: drops filename
#define ENEMYWEAPONDROPSFILENAME				"EnemyWeaponDrops.xml"
#define ENEMYAMMODROPSFILENAME					"EnemyAmmoDrops.xml"
#define ENEMYEXPLOSIVEDROPSFILENAME				"EnemyExplosiveDrops.xml"
#define ENEMYARMOURDROPSFILENAME				"EnemyArmourDrops.xml"
#define ENEMYMISCDROPSFILENAME					"EnemyMiscDrops.xml"

// WANNE: Sector loadscreens [2007-05-18]
#define SECTORLOADSCREENSFILENAME				"Map\\SectorLoadscreens.xml"

//zilpin: pellet spread patterns externalized in XML
#define SPREADPATTERNSFILENAME					"SpreadPatterns.xml"
#define DELIVERYMETHODSFILENAME					"Map\\DeliveryMethods.xml"

// Dealtar: Shipping destinations and delivery methods
#define SHIPPINGDESTINATIONSFILENAME			"Map\\ShippingDestinations.xml"
#define DELIVERYMETHODSFILENAME					"Map\\DeliveryMethods.xml"
// Gotthard: Laptop Text files [2007-10-16]
#define LAPTOPFLORISTTEXTFILENAME				"Laptop\\Florist.xml"
#define LAPTOPFUNERALTEXTFILENAME				"Laptop\\Funeral.xml"
#define LAPTOPBOBBYRAYSTEXTFILENAME				"Laptop\\BobbyRays.xml"

// Gotthard: Website Picture/Text Locations [2007-11-04]
#define LAPTOPFLORISTLOCATIONFILENAME			"Laptop\\FloristPositions.xml"
#define LAPTOPFUNERALLOCATIONFILENAME			"Laptop\\FuneralPositions.xml"

#define MULTIPLAYERTEAMSFILENAME				"Multiplayer\\RandomTeams.xml"

// HEADROCK HAM 3.4: Facility Locations [2009-05-19]
#define SECTORFACILITIESFILENAME				"Map\\Facilities.xml"
// HEADROCK HAM 3.4: Dynamic Roaming Restrictions [2009-05-19]
#define DYNAMICROAMINGFILENAME					"Map\\DynamicRestrictions.xml"
// HEADROCK HAM 3.5: Facility Types [2009-06-14]
#define FACILITYTYPESFILENAME					"Map\\FacilityTypes.xml"
// HEADROCK HAM 3.6: Sector Names [2009-07-27]
#define SECTORNAMESFILENAME						"Map\\SectorNames.xml"
// HEADROCK PROFEX: Merc Profiles [2009-07-27]
#define MERCPROFILESFILENAME					"MercProfiles.xml"
// HEADROCK PROFEX: Merc Opinions [2009-07-27]
#define MERCOPINIONSFILENAME					"MercOpinions.xml"
// HEADROCK HAM 3.6: Bloodcat Placements [2009-07-31]
#define BLOODCATPLACEMENTSFILENAME				"Map\\BloodcatPlacements.xml"
// HEADROCK HAM 3.6: Uniform Colors [2009-09-29]
#define UNIFORMCOLORSFILENAME					"Army\\UniformColors.xml"

//SMALL FACES
#define RPCFACESSMALLFILENAME					"RPCFacesSmall.xml"

//Hidden Names
#define HIDDENNAMESFILENAME						"HiddenNames.xml"

//Enemy Names
#define ENEMYNAMESFILENAME						"EnemyNames.xml"

//Enemy RANK
#define ENEMYRANKFILENAME						"EnemyRank.xml"

//CIV Names
#define CIVGROUPNAMESFILENAME					"CivGroupNames.xml"

//Sender Name List
#define SENDERNAMELISTFILENAME					"SenderNameList.xml"

#define ACTIONITEMSFILENAME					"MapAction\\Actionitems.xml"

#define IMPPORTRAITS					"IMPPortraits.xml"

#ifdef JA2UB
// UB
#define MERCPROFILESFILENAME25					"MercProfiles25.xml"
// UB
#define MERCOPINIONSFILENAME25					"MercOpinions25.xml"
#endif

#define SOUNDPROFILE				"SoundsProfiles.xml"

#define RANDOMSTATS					"RandomStats.xml"

#define SECTORLEVEL1NAMESFILENAME						"Map\\SectorNamesLevel_1.xml"
#define SECTORLEVEL2NAMESFILENAME						"Map\\SectorNamesLevel_2.xml"
#define SECTORLEVEL3NAMESFILENAME						"Map\\SectorNamesLevel_3.xml"

#define LAYOUTMAINMENU	"Layout\\LayoutMainMenu.xml"
#define INTROFILESFILENAME						"IntroFiles.xml"
#define MERCAVAILABILITY	"MercAvailability.xml"

#define AIMAVAILABILITY	"AimAvailability.xml"

#define QUOTEARRAYFILENAME					"MercQuote.xml"

#define FACEGEARFILENAME					"FaceGear.xml"

//Sender Name List
#define EMAILSENDERNAMELIST					"Email\\EmailSenderNameList.xml"
#define EMAILMERCAVAILABLE					"Email\\EmailMercAvailable.xml"
#define EMAILMERCLEVELUP					"Email\\EmailMercLevelUp.xml"
#define EMAILOTHER							"Email\\EmailOther.xml"
#define EMAILINSURANCE 						"Email\\EmailInsurance.xml"


#define VEHICLESFILENAME					"Vehicles.xml"

#define TEXTFILENAME					"Text\\TacticalMessages.xml"

#define ENCYCLOPEDIALOCATIONFILENAME				"Encyclopedia\\EncyclopediaLocations.xml"
#define ENCYCLOPEDIAPROFILEFILENAME					"Encyclopedia\\EncyclopediaProfiles.xml"
#define ENCYCLOPEDIAINVENTORYFILENAME				"Encyclopedia\\EncyclopediaInventory.xml"
#define ENCYCLOPEDIAOTHERFILENAME					"Encyclopedia\\EncyclopediaOthers.xml"
#define ENCYCLOPEDIAQUESTSFILENAME					"Encyclopedia\\EncyclopediaQuests.xml"
#define BRIEFINGROOMFILENAME						"BriefingRoom\\BriefingRoom.xml"

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
//WarmSteel - for New Attachment System
extern BOOLEAN WriteAttachmentSlotsStats();
extern BOOLEAN ReadInAttachmentSlotsStats(STR fileName);

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

// Dealtar: New shipping system XMLs
extern BOOLEAN ReadInShippingDestinations(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInDeliveryMethods(STR fileName);

//zilpin: pellet spread patterns externalized in XML
extern BOOLEAN ReadInSpreadPatterns(STR fileName);
extern BOOLEAN WriteSpreadPatterns();
extern int FindSpreadPatternIndex( const STR strName );
//Gotthard: Laptop Florist Text
extern BOOLEAN ReadInFloristText(STR fileName);

//Gotthard: Laptop Funeral Text
extern BOOLEAN ReadInFuneralText(STR fileName);

//Gotthard: Delivery Locations Text (For Florist, Bobby Ray's)
extern BOOLEAN ReadInBobbyRaysText(STR fileName);

//Gotthard: Special Sector Names Test (Cambria Hospital, etc.)
extern BOOLEAN ReadInSectorNamesText(STR fileName);

//Gotthard: Laptop Florist Locations
extern BOOLEAN ReadInFloristLocations(STR fileName);

//Gotthard: Laptop Funeral Locations
extern BOOLEAN ReadInFuneralLocations(STR fileName);

// HEADROCK HAM 3.4: Sector Facility Locations
extern BOOLEAN ReadInSectorFacilities(STR fileName);

// HEADROCK HAM 3.4: Dynamic Roaming Restrictions
extern BOOLEAN ReadInDynamicRoamingRestrictions(STR fileName);

// HEADROCK HAM 3.5: Facility Types and bonuses
extern BOOLEAN ReadInFacilityTypes(STR fileName, BOOLEAN localizedVersion);

// HEADROCK HAM 3.6: Customized Sector Names
extern BOOLEAN ReadInSectorNames(STR fileName, BOOLEAN localizedVersion, INT8 Level );

// HEADROCK PROFEX: Merc Profiles
extern BOOLEAN ReadInMercProfiles(STR fileName, BOOLEAN localizedVersion);

// HEADROCK PROFEX: Merc Opinions
extern BOOLEAN ReadInMercOpinions(STR fileName);

// HEADROCK HAM 3.6: Customized Bloodcat Placements
extern BOOLEAN ReadInBloodcatPlacements(STR fileName);

// HEADROCK HAM 3.6: Customized Uniform Colors
extern BOOLEAN ReadInUniforms(STR fileName);

//Jazz Small Faces
extern BOOLEAN ReadInSmallFacesStats(RPC_SMALL_FACE_VALUES *pSmallFaces, STR fileName);
extern BOOLEAN WriteSmallFacesStats(RPC_SMALL_FACE_VALUES *pSmallFaces, STR fileName);

//Hidden Names
extern BOOLEAN ReadInHiddenNamesStats(HIDDEN_NAMES_VALUES *pHiddenNames, STR fileName);
extern BOOLEAN WriteHiddenNamesStats(HIDDEN_NAMES_VALUES *pHiddenNames, STR fileName);
extern void LoadHiddenNames();

extern BOOLEAN ReadInEnemyNames(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInCivGroupNamesStats(STR fileName, BOOLEAN localizedVersion);

// the list of email sender names by Jazz
extern BOOLEAN ReadInSenderNameList(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInEnemyRank(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteEnemyRank( STR fileName);
extern BOOLEAN ReadInIMPPortraits(STR fileName, BOOLEAN localizedVersion);
extern void LoadIMPPortraitsTEMP();

//Enabled\Disabled profile sound by Jazz
extern BOOLEAN ReadInSoundProfile( STR fileName );
extern BOOLEAN WriteSoundProfile( STR fileName );

//Random stats by Jazz
extern BOOLEAN WriteRandomStats( STR fileName );
extern BOOLEAN ReadInRandomStats( STR fileName );

//new profiles by Jazz	
extern BOOLEAN ReadInMercAvailability(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteMercAvailability(STR fileName);

extern BOOLEAN ReadInAimAvailability(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteAimAvailability(STR fileName);

extern BOOLEAN WriteQarray(QARRAY_VALUES *pQarray, STR fileName);
extern BOOLEAN ReadInQarray(QARRAY_VALUES *pQarray, STR fileName);

extern BOOLEAN WriteFaceGear();
extern BOOLEAN ReadInFaceGear( FACE_GEAR_VALUES *pFaceGear, STR fileName );
extern BOOLEAN ReadInIntroNames(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInMainMenu(MAIN_MENU_VALUES *pMainMenu, STR fileName);

extern BOOLEAN ReadInActionItems(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteInActionItems( STR fileName);

extern BOOLEAN ReadInEmailMercAvailable(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInEmailMercLevelUp(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInEmailOther(STR fileName, BOOLEAN localizedVersion);
//extern BOOLEAN ReadInInsurance(STR fileName, BOOLEAN localizedVersion);

extern BOOLEAN ReadInEncyclopediaLocation(STR fileName, BOOLEAN localizedVersion, ENCYCLOPEDIA_LOCATION *Ency, UINT32 FileType2 );

extern BOOLEAN ReadInNewVehicles(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteNewVehicles(STR fileName);
extern void InitNewVehicles ();

extern BOOLEAN ReadInLanguageLocation(STR fileName, BOOLEAN localizedVersion, LANGUAGE_LOCATION *Lang, UINT32 FileType2 );

#endif
