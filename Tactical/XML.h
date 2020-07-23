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
#include "BriefingRoom_Data.h"
#include "Inventory Choosing.h"		// added by Flugente
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
	MERCHANT_REORDER,

	ELEMENT_DRUG_EFFECT,
	ELEMENT_DRUG_EFFECT_PROPERTY,
	ELEMENT_DISEASE_EFFECT,
	ELEMENT_DISEASE_EFFECT_PROPERTY,
	ELEMENT_DISABILITY_EFFECT,
	ELEMENT_DISABILITY_EFFECT_PROPERTY,
	ELEMENT_PERSONALITY_EFFECT,
	ELEMENT_PERSONALITY_EFFECT_PROPERTY
}
typedef PARSE_STAGE;

#define MAX_CHAR_DATA_LENGTH			500
#define MAX_CHAR_1000_DATA_LENGTH		1000

#define TABLEDATA_DIRECTORY						"TableData\\"

#define GERMAN_PREFIX							"German."
#define RUSSIAN_PREFIX							"Russian."
#define DUTCH_PREFIX							"Dutch."
#define POLISH_PREFIX							"Polish."
#define FRENCH_PREFIX							"French."
#define ITALIAN_PREFIX							"Italian."
#define CHINESE_PREFIX							"Chinese."

#define	ATTACHMENTSFILENAME						"Items\\Attachments.xml"
#define	ATTACHMENTINFOFILENAME					"Items\\AttachmentInfo.xml"
#define	ITEMSFILENAME							"Items\\Items.xml"
#define	WEAPONSFILENAME							"Items\\Weapons.xml"
#define	LAUNCHABLESFILENAME						"Items\\Launchables.xml"
#define	COMPATIBLEFACEITEMSFILENAME				"Items\\CompatibleFaceItems.xml"
#define	MERGESFILENAME							"Items\\Merges.xml"
#define	ATTACHMENTCOMBOMERGESFILENAME			"Items\\AttachmentComboMerges.xml"
#define ITEMTRANSFORMATIONSFILENAME				"Items\\Item_Transformations.xml"
#define	MAGAZINESFILENAME						"Items\\Magazines.xml"
#define	ARMOURSFILENAME							"Items\\Armours.xml"
#define	EXPLOSIVESFILENAME						"Items\\Explosives.xml"
#define DRUGSFILENAME							"Items\\Drugs.xml"
#define FOODFILENAME							"Items\\Food.xml"
#define FOODOPINIONFILENAME						"Items\\FoodOpinion.xml"
#define STRUCTUREDECONSTRUCTFILENAME			"Items\\StructureDeconstruct.xml"
#define STRUCTURECONSTRUCTFILENAME				"Items\\StructureConstruct.xml"
#define STRUCTUREMOVEFILENAME					"Items\\StructureMove.xml"
#define CLOTHESFILENAME							"Items\\Clothes.xml"
#define RANDOMITEMFILENAME						"Items\\RandomItem.xml"
#define	AMMOFILENAME							"Items\\AmmoStrings.xml"
#define	AMMOTYPESFILENAME						"Items\\AmmoTypes.xml"
#define	INCOMPATIBLEATTACHMENTSFILENAME			"Items\\IncompatibleAttachments.xml"
#define	ATTACHMENTSLOTSFILENAME					"Items\\AttachmentSlots.xml"
#define LOADBEARINGEQUIPMENTFILENAME			"Items\\LoadBearingEquipment.xml"
#define EXPLOSIONDATAFILENAME					"Items\\ExplosionData.xml"
#define LBEPOCKETFILENAME						"Items\\Pockets.xml"
#define LBEPOCKETPOPUPFILENAME					"Items\\PocketPopups.xml"

#define	ENEMYGUNCHOICESFILENAME					"Inventory\\EnemyGunChoices.xml"	// default selection
#define	GUNCHOICESFILENAME_ENEMY_ADMIN			"Inventory\\GunChoices_Enemy_Admin.xml"
#define	GUNCHOICESFILENAME_ENEMY_REGULAR		"Inventory\\GunChoices_Enemy_Regular.xml"
#define	GUNCHOICESFILENAME_ENEMY_ELITE			"Inventory\\GunChoices_Enemy_Elite.xml"
#define	GUNCHOICESFILENAME_MILITIA_GREEN		"Inventory\\GunChoices_Militia_Green.xml"
#define	GUNCHOICESFILENAME_MILITIA_REGULAR		"Inventory\\GunChoices_Militia_Regular.xml"
#define	GUNCHOICESFILENAME_MILITIA_ELITE		"Inventory\\GunChoices_Militia_Elite.xml"
#define	ENEMYITEMCHOICESFILENAME				"Inventory\\EnemyItemChoices.xml"	// default selection
#define ITEMCHOICESFILENAME_ENEMY_ADMIN			"Inventory\\ItemChoices_Enemy_Admin.xml"
#define ITEMCHOICESFILENAME_ENEMY_REGULAR		"Inventory\\ItemChoices_Enemy_Regular.xml"
#define ITEMCHOICESFILENAME_ENEMY_ELITE			"Inventory\\ItemChoices_Enemy_Elite.xml"
#define ITEMCHOICESFILENAME_MILITIA_GREEN		"Inventory\\ItemChoices_Militia_Green.xml"
#define ITEMCHOICESFILENAME_MILITIA_REGULAR		"Inventory\\ItemChoices_Militia_Regular.xml"
#define ITEMCHOICESFILENAME_MILITIA_ELITE		"Inventory\\ItemChoices_Militia_Elite.xml"
#define	IMPITEMCHOICESFILENAME					"Inventory\\IMPItemChoices.xml"
#define MERCSTARTINGGEARFILENAME				"Inventory\\MercStartingGear.xml"
#define ENEMYWEAPONDROPSFILENAME				"Inventory\\EnemyWeaponDrops.xml"
#define ENEMYAMMODROPSFILENAME					"Inventory\\EnemyAmmoDrops.xml"
#define ENEMYEXPLOSIVEDROPSFILENAME				"Inventory\\EnemyExplosiveDrops.xml"
#define ENEMYARMOURDROPSFILENAME				"Inventory\\EnemyArmourDrops.xml"
#define ENEMYMISCDROPSFILENAME					"Inventory\\EnemyMiscDrops.xml"

#ifdef JA2UB
	#define MERCSTARTINGGEAR25FILENAME			"Inventory\\MercStartingGear25.xml"
#endif

#define MERCHANTSFILENAME						"NPCInventory\\Merchants.xml"
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
#define TINAINVENTORYFILENAME					"NPCInventory\\TinaInventory.xml"

// Flugente: create additional filenames by putting them together
#define ADITIONALDEALER_INVENTORYFILENAME_A	"NPCInventory\\AdditionalDealer_"
#define ADITIONALDEALER_INVENTORYFILENAME_B	"_Inventory.xml"

#ifdef JA2UB
	#define BETTYINVENTORYFILENAME				"NPCInventory\\BettyInventory.xml"
	#define RAULINVENTORYFILENAME				"NPCInventory\\RaulInventory.xml"
#endif

#define SOUNDSFILENAME							"Sounds\\Sounds.xml"
#define BURSTSOUNDSFILENAME						"Sounds\\BurstSounds.xml"

#define SECTORLOADSCREENSFILENAME				"Map\\SectorLoadscreens.xml"
#define CITYTABLEFILENAME						"Map\\Cities.xml"
#define MOVEMENTCOSTFILENAME					"Map\\MovementCosts.xml"
#define ALTSECTORSFILENAME						"Map\\AltSectors.xml"
#define SAMSITESFILENAME						"Map\\SamSites.xml"
#define HELISITESFILENAME						"Map\\HeliSites.xml"
#define EXTRAITEMSFILENAME						"Map\\A9_0_ExtraItems"	// ".xml" will be added @runtime
#define EXTRAITEMSFILENAME2						"Map\\A11_0_ExtraItems" // ".xml" will be added @runtime
#define SHIPPINGDESTINATIONSFILENAME			"Map\\ShippingDestinations.xml"
#define DELIVERYMETHODSFILENAME					"Map\\DeliveryMethods.xml"
#define DELIVERYMETHODSFILENAME					"Map\\DeliveryMethods.xml"
#define SECTORFACILITIESFILENAME				"Map\\Facilities.xml"
#define FACILITYTYPESFILENAME					"Map\\FacilityTypes.xml"
#define SECTORNAMESFILENAME						"Map\\SectorNames.xml"
#define COOLNESSBYSECTORFILENAME				"Map\\CoolnessBySector.xml"
#define BLOODCATPLACEMENTSFILENAME				"Map\\BloodcatPlacements.xml"
#define CREATUREPLACEMENTSFILENAME				"Map\\CreaturePlacements.xml"
#define SECTORLEVEL1NAMESFILENAME				"Map\\SectorNamesLevel_1.xml"
#define SECTORLEVEL2NAMESFILENAME				"Map\\SectorNamesLevel_2.xml"
#define SECTORLEVEL3NAMESFILENAME				"Map\\SectorNamesLevel_3.xml"
#define INTERACTIVEACTIONSFILENAME				"Map\\InteractiveActions.xml"

#define GARRISONFILENAME						"Army\\GarrisonGroups.xml"
#define PATROLFILENAME							"Army\\PatrolGroups.xml"
#define COMPOSITIONFILENAME						"Army\\ArmyComposition.xml"
#define UNIFORMCOLORSFILENAME					"Army\\UniformColors.xml"

#define LAPTOPFLORISTTEXTFILENAME				"Laptop\\Florist.xml"
#define LAPTOPFUNERALTEXTFILENAME				"Laptop\\Funeral.xml"
#define LAPTOPBOBBYRAYSTEXTFILENAME				"Laptop\\BobbyRays.xml"
#define LAPTOPFLORISTLOCATIONFILENAME			"Laptop\\FloristPositions.xml"
#define LAPTOPFUNERALLOCATIONFILENAME			"Laptop\\FuneralPositions.xml"

#define EMAILSENDERNAMELIST						"Email\\EmailSenderNameList.xml"
#define EMAILMERCAVAILABLE						"Email\\EmailMercAvailable.xml"
#define EMAILMERCLEVELUP						"Email\\EmailMercLevelUp.xml"
#define EMAILOTHER								"Email\\EmailOther.xml"
#define EMAILINSURANCE 							"Email\\EmailInsurance.xml"

// WANNE: Old encyclopedia. Not used anymore ..
#ifdef ENABLE_ENCYCLOPEDIA
	#define ENCYCLOPEDIALOCATIONFILENAME			"Encyclopedia\\EncyclopediaLocations.xml"
	#define ENCYCLOPEDIAPROFILEFILENAME				"Encyclopedia\\EncyclopediaProfiles.xml"
	#define ENCYCLOPEDIAINVENTORYFILENAME			"Encyclopedia\\EncyclopediaInventory.xml"
	#define ENCYCLOPEDIAOTHERFILENAME				"Encyclopedia\\EncyclopediaOthers.xml"
	#define ENCYCLOPEDIAQUESTSFILENAME				"Encyclopedia\\EncyclopediaQuests.xml"
#endif

#define BRIEFINGROOMFILENAME					"BriefingRoom\\BriefingRoom.xml"

#define MULTIPLAYERTEAMSFILENAME				"Multiplayer\\RandomTeams.xml"

#define ACTIONITEMSFILENAME						"MapAction\\Actionitems.xml"

#define LAYOUTMAINMENU							"Layout\\LayoutMainMenu.xml"

#define MINERALSFILENAME						"Map\\Minerals.xml"

#define AIMAVAILABILITY							"AimAvailability.xml"
#define CIVGROUPNAMESFILENAME					"CivGroupNames.xml"
#define ENEMYNAMESFILENAME						"EnemyNames.xml"
#define ENEMYRANKFILENAME						"EnemyRank.xml"
#define FACEGEARFILENAME						"FaceGear.xml"
#define HIDDENNAMESFILENAME						"HiddenNames.xml"
#define IMPPORTRAITS							"IMPPortraits.xml"
#define IMPVOICES								"IMPVoices.xml"
#define MERCAVAILABILITY						"MercAvailability.xml"
#define MERCOPINIONSFILENAME					"MercOpinions.xml"
#define MERCPROFILESFILENAME					"MercProfiles.xml"
#define LOADSCREENHINTSFILENAME					"LoadScreenHints.xml"

#ifdef JA2UB
	#define MERCPROFILESFILENAME25				"MercProfiles25.xml"
	#define MERCOPINIONSFILENAME25				"MercOpinions25.xml"
#endif

#define MERCQUOTEFILENAME						"MercQuote.xml"
#define OLDAIMARCHIVEFILENAME					"OldAIMArchive.xml"
#define RANDOMSTATS								"RandomStats.xml"
#define RPCFACESSMALLFILENAME					"RPCFacesSmall.xml"
#define SPREADPATTERNSFILENAME					"SpreadPatterns.xml"
#define VEHICLESFILENAME						"Vehicles.xml"
#define SQUADNAMEFILENAME						"SquadNames.xml"
#define BACKGROUNDSFILENAME						"Backgrounds.xml"
#define INDIVIDUALMILITIAFILENAME				"MilitiaIndividual.xml"
#define CAMPAIGNSTATSEVENTSFILENAME				"CampaignStatsEvents.xml"
#define DISEASEFILENAME							"Disease.xml"

#define TAUNTSFOLDERNAME						"EnemyTaunts\\"
#define TAUNTSFILENAME							"EnemyTaunts.xml"
#define TAUNTSFILENAMEBEGINNING					"EnemyTaunts"		// You can add additional taunt files next to the "EnemyTaunts.xml" file. They have to start with "EnemyTaunts*.xml" (e.g: EnemyTauntsKill.xml)
#define TAUNTSFILENAMEENDING					".xml"

#define LOCKSFILENAME							"Items\\Locks.xml"
#define KEYSFILENAME							"Items\\Keys.xml"

#define ENEMY_ADMIN_PROFILE_FILENAME			"Profiles\\SoldierProfileEnemyAdmin.xml"
#define ENEMY_REGULAR_PROFILE_FILENAME			"Profiles\\SoldierProfileEnemyRegular.xml"
#define ENEMY_ELITE_PROFILE_FILENAME			"Profiles\\SoldierProfileEnemyElite.xml"
#define MILITIA_GREEN_PROFILE_FILENAME			"Profiles\\SoldierProfileMilitiaGreen.xml"
#define MILITIA_REGULAR_PROFILE_FILENAME		"Profiles\\SoldierProfileMilitiaRegular.xml"
#define MILITIA_VETERAN_PROFILE_FILENAME		"Profiles\\SoldierProfileMilitiaVeteran.xml"

#define HISTORYNAMEFILENAME						"History.xml"

#define DIFFICULTYFILENAME							"DifficultySettings.xml"

#define LBT_LAYERSFILENAME						"LogicalBodyTypes/Layers.xml"
#define LBT_PALETTESFILENAME					"LogicalBodyTypes/Palettes.xml"
#define LBT_ANIMSURFACESFILENAME				"LogicalBodyTypes/AnimationSurfaces.xml"
#define LBT_FILTERSFILENAME						"LogicalBodyTypes/Filters.xml"
#define LBT_BODYTYPESFILENAME					"LogicalBodyTypes/LogicalBodyTypes.xml"

extern BOOLEAN ReadInItemStats(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteItemStats();

extern BOOLEAN ReadInAttachmentStats(STR fileName);
extern BOOLEAN WriteAttachmentStats();

extern BOOLEAN ReadInAttachmentInfoStats(STR fileName);
extern BOOLEAN WriteAttachmentInfoStats();

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

// HEADROCK HAM 5: Item Transformation XML reader
extern BOOLEAN ReadInTransformationStats(STR fileName);

extern BOOLEAN ReadInArmourStats(STR fileName);
extern BOOLEAN WriteArmourStats();

// CHRISL:
extern BOOLEAN ReadInLBEStats(STR fileName);
extern BOOLEAN WriteLBEEquipmentStats();

extern BOOLEAN ReadInLBEPocketStats(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteLBEPocketEquipmentStats();

// THE_BOB : added for pocket popup definitions
extern BOOLEAN ReadInLBEPocketPopups(STR fileName);

extern BOOLEAN ReadInMercStartingGearStats(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteMercStartingGearStats();

extern BOOLEAN ReadInExplosiveStats(STR fileName);
extern BOOLEAN WriteExplosiveStats();

// Flugente: drugs
extern BOOLEAN ReadInDrugsStats(STR fileName);
extern BOOLEAN WriteDrugsStats();

// Flugente: food
extern BOOLEAN ReadInFoodStats(STR fileName);
extern BOOLEAN WriteFoodStats();

// Flugente: merc opinions on food
extern BOOLEAN ReadInFoodOpinionStats(STR fileName);
extern BOOLEAN WriteFoodOpinionStats();

// Flugente: disease
BOOLEAN ReadInDiseaseStats( STR fileName, BOOLEAN localizedVersion );
extern BOOLEAN WriteDiseaseStats( );

// Flugente: structure (de-)construct
extern BOOLEAN ReadInStructureDeconstructStats(STR fileName);
extern BOOLEAN WriteStructureDeconstructStats();

extern BOOLEAN ReadInStructureConstructStats(STR fileName);
extern BOOLEAN WriteStructureConstructStats();

// Flugente: interactive actions
extern BOOLEAN ReadInInteractiveActionsStats( STR fileName );
extern BOOLEAN WriteInteractiveActionsStats( );

// Flugente: structure move
extern BOOLEAN ReadInStructureMoveStats( STR fileName );
extern BOOLEAN WriteStructureMoveStats();


// Flugente: merchants
extern BOOLEAN ReadInMerchantStats(STR fileName);
extern BOOLEAN WriteMerchantStats();

// Flugente: clothes
extern BOOLEAN ReadInClothesStats(STR fileName);
extern BOOLEAN WriteClothesStats();

// Flugente: random items
extern BOOLEAN ReadInRandomItemStats(STR fileName);
extern BOOLEAN WriteRandomItemStats();

// Flugente: squad names
extern BOOLEAN ReadInSquadNamesStats(STR fileName);
extern BOOLEAN WriteSquadNamesStats();

// Flugente: loadscreen hints
extern BOOLEAN ReadInLoadScreenHints(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteLoadScreenHints();

// Flugente: soldier profiles
extern BOOLEAN ReadInSoldierProfiles(SOLDIER_PROFILE_VALUES* pProfiles, STR fileName);
extern BOOLEAN WriteSoldierProfiles();

extern BOOLEAN ReadInAmmoStats(STR fileName);
extern BOOLEAN WriteAmmoStats();

extern BOOLEAN WriteIncompatibleAttachmentStats();
extern BOOLEAN ReadInIncompatibleAttachmentStats(STR fileName);
//WarmSteel - for New Attachment System
extern BOOLEAN WriteAttachmentSlotsStats();
extern BOOLEAN ReadInAttachmentSlotsStats(STR fileName, BOOLEAN localizedVersion);

extern BOOLEAN WriteExtendedArmyGunChoicesStats();
extern BOOLEAN ReadInExtendedArmyGunChoicesStats(ARMY_GUN_CHOICE_TYPE* pGunChoice, STR fileName);

extern BOOLEAN WriteArmyItemChoicesStats();
extern BOOLEAN ReadInArmyItemChoicesStats(ARMY_GUN_CHOICE_TYPE* pGunChoice, STR fileName);

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

// Buggler: helisite externalization
extern BOOLEAN ReadInHeliInfo(STR fileName);
extern BOOLEAN WriteInHeliInfo(STR fileName);

// Lesh: army externalization
extern BOOLEAN ReadInGarrisonInfo(STR fileName);
extern BOOLEAN WriteInGarrisonInfo(STR fileName);
extern BOOLEAN ReadInPatrolInfo(STR fileName);
extern BOOLEAN WriteInPatrolInfo(STR fileName);
extern BOOLEAN ReadInArmyCompositionInfo(STR fileName);
extern BOOLEAN WriteInArmyCompositionInfo(STR fileName);

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

// HEADROCK HAM 3.5: Facility Types and bonuses
extern BOOLEAN ReadInFacilityTypes(STR fileName, BOOLEAN localizedVersion);

// HEADROCK HAM 3.6: Customized Sector Names
extern BOOLEAN ReadInSectorNames(STR fileName, BOOLEAN localizedVersion, INT8 Level );

// HEADROCK HAM 5: Coolness by Sectors
extern BOOLEAN ReadInCoolnessBySector(STR fileName );

// HEADROCK PROFEX: Merc Profiles
extern BOOLEAN ReadInMercProfiles(STR fileName, BOOLEAN localizedVersion);

// HEADROCK PROFEX: Merc Opinions
extern BOOLEAN ReadInMercOpinions(STR fileName);

// HEADROCK HAM 3.6: Customized Bloodcat Placements
extern BOOLEAN ReadInBloodcatPlacements(STR fileName);

// Buggler: Customized Creature Placements
extern BOOLEAN ReadInCreaturePlacements(STR fileName);

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
extern BOOLEAN ReadInIMPVoices( STR fileName );
extern void LoadIMPPortraitsTEMP();

// Flugente: backgrounds
extern BOOLEAN ReadInBackgrounds(STR fileName, BOOLEAN localizedVersion );
extern BOOLEAN WriteBackgrounds( STR fileName);

// Flugente: individual militia
extern BOOLEAN ReadInMilitiaIndividual( STR fileName, BOOLEAN localizedVersion );
extern BOOLEAN WriteMilitiaIndividual( STR fileName );

// Flugente: campaign stats events
extern BOOLEAN ReadInCampaignStatsEvents(STR fileName, BOOLEAN localizedVersion );
extern BOOLEAN WriteCampaignStatsEvents( STR fileName);

// anv: externalised taunts
extern BOOLEAN ReadInTaunts(STR fileName, BOOLEAN localizedVersion );
extern BOOLEAN WriteTaunts( STR fileName );

// anv: detailed tileset terrain types
extern BOOLEAN ReadInAdditionalTileProperties(STR fileName );
extern BOOLEAN WriteAdditionalTileProperties( STR fileName );

// anv: externalised locks and keys
extern BOOLEAN ReadInLocks(STR fileName );
extern BOOLEAN WriteLocks( STR fileName );

extern BOOLEAN ReadInKeys( STR fileName );
extern BOOLEAN WriteKeys( STR fileName );

//Random stats by Jazz
extern BOOLEAN WriteRandomStats( STR fileName );
extern BOOLEAN ReadInRandomStats( STR fileName );

//new profiles by Jazz	
extern BOOLEAN ReadInMercAvailability(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteMercAvailability(STR fileName);

extern BOOLEAN ReadInAimAvailability(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteAimAvailability(STR fileName);

extern BOOLEAN WriteMercQuotes(QARRAY_VALUES *pQarray, STR fileName);
extern BOOLEAN ReadInMercQuotes(QARRAY_VALUES *pQarray, STR fileName);

extern BOOLEAN WriteFaceGear();
extern BOOLEAN ReadInFaceGear( FACE_GEAR_VALUES *pFaceGear, STR fileName );
extern BOOLEAN ReadInIntroNames(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInMainMenu(MAIN_MENU_VALUES *pMainMenu, STR fileName);

extern BOOLEAN ReadInActionItems(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteInActionItems( STR fileName);

extern BOOLEAN ReadInEmailMercAvailable(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInEmailMercLevelUp(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN ReadInEmailOther(STR fileName, BOOLEAN localizedVersion);

extern BOOLEAN ReadInBriefingRoom(STR fileName, BOOLEAN localizedVersion, BRIEFINGROOM_M_DATA *Ency, UINT32 FileType2 );
extern BOOLEAN ReadInMinerals(STR fileName, BOOLEAN localizedVersion);

extern BOOLEAN ReadInNewVehicles(STR fileName, BOOLEAN localizedVersion);
extern BOOLEAN WriteNewVehicles(STR fileName);
extern void InitNewVehicles ();

extern BOOLEAN ReadInLanguageLocation(STR fileName, BOOLEAN localizedVersion, LANGUAGE_LOCATION *Lang, UINT32 FileType2 );

extern BOOLEAN ReadInAimOldArchive(STR fileName, BOOLEAN localizedVersion);

extern BOOLEAN ReadInHistorys(STR fileName, BOOLEAN localizedVersion );
extern BOOLEAN ReadInDifficultySettings(STR fileName, BOOLEAN localizedVersion);

#endif
