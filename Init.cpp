#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "HelpScreen.h"
	#include "Multilingual Text Code Generator.h"
	#include "INIReader.h"

#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "sgp.h"
	#include "Gameloop.h"
	#include "Screens.h"
	#include "INIReader.h"
	#include "vobject_blitters.h"
	#include "renderworld.h"
	#include "mousesystem.h"
	#include "sysutil.h"
	#include "worlddef.h"
	#include "Animation Data.h"
	#include "overhead.h"
	#include "Font Control.h"
	#include "Button System.h"
	#include "Timer Control.h"
	#include "Utilities.h"
	#include "Radar Screen.h"
	#include "Render Dirty.h"
	#include "Sound Control.h"
	#include "Event Pump.h"
	#include "lighting.h"
	#include "Cursor Control.h"
	#include "music control.h"
	#include "video.h"
	#include "mapscreen.h"
	#include "interface items.h"
	#include "Maputility.h"
	#include "strategic.h"
	#include "dialogue control.h"
	#include "Text.h"
	#include "laptop.h"
	#include "NPC.h"
	#include "MercTextBox.h"
	#include "tile cache.h"
	#include "strategicmap.h"
	#include "Map Information.h"
	#include "laptop.h"
	#include "Shade Table Util.h"
	#include "Exit Grids.h"
	#include "Summary Info.h"
	#include "GameSettings.h"
	#include "Game Init.h"
	#include "Init.h"
	#include "jascreens.h"
	#include "XML.h"
	#include "SaveLoadGame.h"
	#include "weapons.h"
	#include "Strategic Movement.h"
	#include "Vehicles.h"
	#include "Multilingual Text Code Generator.h"
	#include "editscreen.h"
	#include "Arms Dealer Init.h"
#endif
#include "MPXmlTeams.hpp"
#include "Strategic Mines LUA.h"
#include "UndergroundInit.h"
#include "lua_state.h"

#include "Interface.h"
#include "Strategic Town Loyalty.h"
#include "Soldier Profile.h"
#include "aim.h"
#include "mainmenuscreen.h"
#include "email.h"

#ifdef JA2UB
#include "Ja25_Tactical.h"
#endif

#include "ub_config.h"


#include "Civ Quotes.h"
#include "LuaInitNPCs.h"
#include "BriefingRoom_Data.h"
#include "AimArchives.h"
#include "connect.h"

extern INT16 APBPConstants[TOTAL_APBP_VALUES] = {0};
extern INT16 gubMaxActionPoints[28];//MAXBODYTYPES = 28... JUST GETTING IT TO WORK NOW.  GOTTHARD 7/2/08
extern BOOLEAN GetCDromDriveLetter( STR8	pString );

// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game

#ifdef JA2EDITOR
#define BUILD_AS_EDITOR_ONLY
#else
#undef BUILD_AS_EDITOR_ONLY
#endif

#ifdef JA2BETAVERSION
extern	BOOLEAN	gfUseConsecutiveQuickSaveSlots;
#endif

#if defined( GERMAN ) && !defined( _DEBUG )
	//#define LASERLOCK_ENABLED
#endif


#ifdef LASERLOCK_ENABLED
	int	LASERLOK_Init( HINSTANCE hInstance );
	int	LASERLOK_Run();
	int	LASERLOK_Check();

	BOOLEAN	PrepareLaserLockSystem();
	void HandleLaserLockResult( BOOLEAN fSuccess );
//	int	TestCall( int n);
#endif

extern	HINSTANCE					ghInstance;

extern OBJECTTYPE GLOCK_17_ForUseWithLOS;

// Prepends the language prefix to the file name in a proposed path.
static void AddLanguagePrefix(STR fileName, const STR language)
{
	char *fileComponent;

	fileComponent = strrchr( fileName, '\\');
	if (fileComponent)
	{
		fileComponent++;
	}
	else
	{
		fileComponent = fileName;
	}

	// Make sure to use the overlap-safe version of memory copy
	memmove( fileComponent + strlen( language), fileComponent, strlen( fileComponent) + 1);
	memmove( fileComponent, language, strlen( language) );
}

static const STR GetLanguagePrefix()
{
#ifdef ENGLISH
	return "";
#endif
#ifdef GERMAN 
	return GERMAN_PREFIX;
#endif
#ifdef RUSSIAN 
	return RUSSIAN_PREFIX;
#endif
#ifdef DUTCH 
	return DUTCH_PREFIX;
#endif
#ifdef POLISH 
	return POLISH_PREFIX;
#endif
#ifdef FRENCH 
	return FRENCH_PREFIX;
#endif
#ifdef ITALIAN 
	return ITALIAN_PREFIX;
#endif
#ifdef TAIWANESE 
	return TAIWANESE_PREFIX;
#endif
#ifdef CHINESE
	return CHINESE_PREFIX;
#endif
}

static void AddLanguagePrefix(STR fileName)
{
	AddLanguagePrefix( fileName, GetLanguagePrefix());
}

void ResetEmptyRPCFaceSlots()
{
	//legion by jazz (load small faces from rpcs)
	// reset empty slots with "-1"
	for (UINT8 i = 0; i < 200; i++)
	{
		if (i == 0 && 
			gRPCSmallFaceValues[i].bEyesX == 0 && gRPCSmallFaceValues[i].bEyesY == 0 &&
			gRPCSmallFaceValues[i].bMouthX == 0 && gRPCSmallFaceValues[i].bMouthY == 0 &&
			gRPCSmallFaceValues[i].FaceIndex == 0 && gRPCSmallFaceValues[i].uiIndex == 0)
		{
			gRPCSmallFaceValues[i].uiIndex = -1;	// -1 -> 65535
		}
		else if (i > 0 && gRPCSmallFaceValues[i].uiIndex == 0)
		{
			gRPCSmallFaceValues[i].uiIndex = -1;	// -1 -> 65535
		}				
	}
}

BOOLEAN LoadExternalGameplayData(STR directoryName)
{
	char fileName[MAX_PATH];

	//zilpin: pellet spread patterns externalized in XML
	//If file not found, or error, then the old hard-coded defaults are used by LOS.cpp
	//This needs to be loaded before AmmoTypes and Items because SpreadPatterns can be referenced by name or index.
	strcpy(fileName, directoryName);
	strcat(fileName, SPREADPATTERNSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	ReadInSpreadPatterns(fileName);

	// WANNE: Enemy drops - begin
	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYMISCDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInEnemyMiscDropsStats(gEnemyMiscDrops, fileName), ENEMYMISCDROPSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYEXPLOSIVEDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInEnemyExplosiveDropsStats(gEnemyExplosiveDrops, fileName),ENEMYEXPLOSIVEDROPSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYWEAPONDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInEnemyWeaponDropsStats(gEnemyWeaponDrops, fileName),ENEMYWEAPONDROPSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYAMMODROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInEnemyAmmoDropsStats(gEnemyAmmoDrops, fileName),ENEMYAMMODROPSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYARMOURDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInEnemyArmourDropsStats(gEnemyArmourDrops, fileName),ENEMYARMOURDROPSFILENAME);
	// WANNE: Enemy drops - end

	// WANNE: Sector Loadscreens [2007-05-18]
	strcpy(fileName, directoryName);
	strcat(fileName, SECTORLOADSCREENSFILENAME);

	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInSectorLoadscreensStats(gSectorLoadscreens, fileName),SECTORLOADSCREENSFILENAME);
	}
	else
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("File does not exist, fileName = %s", fileName));
		// Reset ini-option
		gGameExternalOptions.gfUseExternalLoadscreens = FALSE;
	}

	strcpy(fileName, directoryName);
	strcat(fileName, AMMOTYPESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInAmmoTypeStats(fileName),AMMOTYPESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, AMMOFILENAME);

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
#ifndef ENGLISH
	AddLanguagePrefix(fileName);
		
	if ( FileExists(fileName) )
	{
		SGP_THROW_IFFALSE(ReadInAmmoStats(fileName),AMMOFILENAME);
	}
	else
	{
		//CHRISL: If we fail to load, try loading just the default english
		strcpy(fileName, directoryName);
		strcat(fileName, AMMOFILENAME);
		SGP_THROW_IFFALSE(ReadInAmmoStats(fileName),AMMOFILENAME);
	}
#else	
	SGP_THROW_IFFALSE(ReadInAmmoStats(fileName),AMMOFILENAME);
#endif

	// Lesh: added this, begin
	strcpy(fileName, directoryName);
	strcat(fileName, BURSTSOUNDSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInBurstSoundArray(fileName),BURSTSOUNDSFILENAME);
	// Lesh: end

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInItemStats(fileName,FALSE),ITEMSFILENAME);

//Madd: Simple localization
// The idea here is that we can have a separate xml file that's named differently
// but only contains the relevant tags that need to be localized
// then when the file is read in using the same xml reader code, it will only overwrite
// the tags that are contained in the localized file.	This only works for items.xml 
// since I tweaked the xml_items.cpp to make it work :p
// So for instance, the german file would be called German.Items.xml and would only contain
// the uiIndex (for reference), szItemName, szLongItemName, szItemDesc, szBRName, and szBRDesc tags


#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInItemStats(fileName,TRUE), fileName);
	}
#endif

	//if(!WriteItemStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, SOUNDSFILENAME);
	SGP_THROW_IFFALSE(ReadInSoundArray(fileName),SOUNDSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, MAGAZINESFILENAME);
	SGP_THROW_IFFALSE(ReadInMagazineStats(fileName),MAGAZINESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTSFILENAME);
	SGP_THROW_IFFALSE(ReadInAttachmentStats(fileName),ATTACHMENTSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTINFOFILENAME);
	SGP_THROW_IFFALSE(ReadInAttachmentInfoStats(fileName),ATTACHMENTINFOFILENAME);
	
	strcpy(fileName, directoryName);
	strcat(fileName, LAUNCHABLESFILENAME);
	SGP_THROW_IFFALSE(ReadInLaunchableStats(fileName),LAUNCHABLESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, COMPATIBLEFACEITEMSFILENAME);
	SGP_THROW_IFFALSE(ReadInCompatibleFaceItemStats(fileName),COMPATIBLEFACEITEMSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, MERGESFILENAME);
	SGP_THROW_IFFALSE(ReadInMergeStats(fileName),MERGESFILENAME);

	//if(!WriteMergeStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTCOMBOMERGESFILENAME);
	SGP_THROW_IFFALSE(ReadInAttachmentComboMergeStats(fileName),ATTACHMENTCOMBOMERGESFILENAME);

	// HEADROCK HAM 5: Read item transformation 
	strcpy(fileName, directoryName);
	strcat(fileName, ITEMTRANSFORMATIONSFILENAME);
	SGP_THROW_IFFALSE(ReadInTransformationStats(fileName),ITEMTRANSFORMATIONSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIVESFILENAME);
	SGP_THROW_IFFALSE(ReadInExplosiveStats(fileName),EXPLOSIVESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, DRUGSFILENAME);
	SGP_THROW_IFFALSE(ReadInDrugsStats(fileName),DRUGSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, FOODFILENAME);
	SGP_THROW_IFFALSE(ReadInFoodStats(fileName),FOODFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, MERCHANTSFILENAME);
	SGP_THROW_IFFALSE(ReadInMerchantStats(fileName),MERCHANTSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, CLOTHESFILENAME);
	SGP_THROW_IFFALSE(ReadInClothesStats(fileName),CLOTHESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, RANDOMITEMFILENAME);
	SGP_THROW_IFFALSE(ReadInRandomItemStats(fileName),RANDOMITEMFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, SQUADNAMEFILENAME);
	SGP_THROW_IFFALSE(ReadInSquadNamesStats(fileName),SQUADNAMEFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, LOADSCREENHINTSFILENAME);

#ifndef ENGLISH
	AddLanguagePrefix(fileName);
		
	if ( FileExists(fileName) )
	{
		SGP_THROW_IFFALSE(ReadInLoadScreenHints(fileName),LOADSCREENHINTSFILENAME);
	}
	else
	{
		//CHRISL: If we fail to load, try loading just the default english
		strcpy(fileName, directoryName);
		strcat(fileName, LOADSCREENHINTSFILENAME);
		SGP_THROW_IFFALSE(ReadInLoadScreenHints(fileName),LOADSCREENHINTSFILENAME);
	}
#else	
	SGP_THROW_IFFALSE(ReadInLoadScreenHints(fileName),LOADSCREENHINTSFILENAME);
#endif
	
	
	strcpy(fileName, directoryName);
	strcat(fileName, ARMOURSFILENAME);
	SGP_THROW_IFFALSE(ReadInArmourStats(fileName),ARMOURSFILENAME);

	// CHRISL:
	strcpy(fileName, directoryName);
	strcat(fileName, LOADBEARINGEQUIPMENTFILENAME);
	SGP_THROW_IFFALSE(ReadInlbeStats(fileName),LOADBEARINGEQUIPMENTFILENAME);

	// CHRISL:
	LBEPocketType.clear();
	strcpy(fileName, directoryName);
	strcat(fileName, LBEPOCKETFILENAME);
	SGP_THROW_IFFALSE(ReadInLBEPocketStats(fileName,FALSE),LBEPOCKETFILENAME);

	// THE_BOB : added for pocket popup definitions
	LBEPocketPopup.clear();
	strcpy(fileName, directoryName);
	strcat(fileName, LBEPOCKETPOPUPFILENAME);
	SGP_THROW_IFFALSE(ReadInLBEPocketPopups(fileName),LBEPOCKETPOPUPFILENAME);

//CHRISL: Simple localization
// Same setup as what Madd used for items.xml

#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInLBEPocketStats(fileName,TRUE), fileName);
	}
#endif

#ifdef JA2UB

	strcpy(fileName, directoryName);
	strcat(fileName, MERCSTARTINGGEAR25FILENAME);
	SGP_THROW_IFFALSE(ReadInMercStartingGearStats(fileName),MERCSTARTINGGEAR25FILENAME);

#else

	// CHRISL:
	strcpy(fileName, directoryName);
	strcat(fileName, MERCSTARTINGGEARFILENAME);
	SGP_THROW_IFFALSE(ReadInMercStartingGearStats(fileName),MERCSTARTINGGEARFILENAME);

#endif

	strcpy(fileName, directoryName);
	strcat(fileName, WEAPONSFILENAME);
	SGP_THROW_IFFALSE(ReadInWeaponStats(fileName),WEAPONSFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, INCOMPATIBLEATTACHMENTSFILENAME);
	SGP_THROW_IFFALSE(ReadInIncompatibleAttachmentStats(fileName),INCOMPATIBLEATTACHMENTSFILENAME);

	//WarmSteel - Attachment slots related xml's
	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTSLOTSFILENAME);
	SGP_THROW_IFFALSE(ReadInAttachmentSlotsStats(fileName),ATTACHMENTSLOTSFILENAME);

	// Flugente: created separate gun and item choices for different soldier classes - read in different xmls
	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYGUNCHOICESFILENAME);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_NONE], fileName), ENEMYGUNCHOICESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, GUNCHOICESFILENAME_ENEMY_ADMIN);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_ADMINISTRATOR], fileName), GUNCHOICESFILENAME_ENEMY_ADMIN);

	strcpy(fileName, directoryName);
	strcat(fileName, GUNCHOICESFILENAME_ENEMY_REGULAR);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_ARMY], fileName), GUNCHOICESFILENAME_ENEMY_REGULAR);

	strcpy(fileName, directoryName);
	strcat(fileName, GUNCHOICESFILENAME_ENEMY_ELITE);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_ELITE], fileName), GUNCHOICESFILENAME_ENEMY_ELITE);

	strcpy(fileName, directoryName);
	strcat(fileName, GUNCHOICESFILENAME_MILITIA_GREEN);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_GREEN_MILITIA], fileName), GUNCHOICESFILENAME_MILITIA_GREEN);

	strcpy(fileName, directoryName);
	strcat(fileName, GUNCHOICESFILENAME_MILITIA_REGULAR);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_REG_MILITIA], fileName), GUNCHOICESFILENAME_MILITIA_REGULAR);

	strcpy(fileName, directoryName);
	strcat(fileName, GUNCHOICESFILENAME_MILITIA_ELITE);
	SGP_THROW_IFFALSE(ReadInExtendedArmyGunChoicesStats(gExtendedArmyGunChoices[SOLDIER_CLASS_ELITE_MILITIA], fileName), GUNCHOICESFILENAME_MILITIA_ELITE);

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYITEMCHOICESFILENAME);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_NONE], fileName),ENEMYITEMCHOICESFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMCHOICESFILENAME_ENEMY_ADMIN);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_ADMINISTRATOR], fileName),ITEMCHOICESFILENAME_ENEMY_ADMIN);

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMCHOICESFILENAME_ENEMY_REGULAR);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_ARMY], fileName),ITEMCHOICESFILENAME_ENEMY_REGULAR);

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMCHOICESFILENAME_ENEMY_ELITE);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_ELITE], fileName),ITEMCHOICESFILENAME_ENEMY_ELITE);

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMCHOICESFILENAME_MILITIA_GREEN);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_GREEN_MILITIA], fileName),ITEMCHOICESFILENAME_MILITIA_GREEN);

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMCHOICESFILENAME_MILITIA_REGULAR);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_REG_MILITIA], fileName),ITEMCHOICESFILENAME_MILITIA_REGULAR);

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMCHOICESFILENAME_MILITIA_ELITE);
	SGP_THROW_IFFALSE(ReadInArmyItemChoicesStats(gArmyItemChoices[SOLDIER_CLASS_ELITE_MILITIA], fileName),ITEMCHOICESFILENAME_MILITIA_ELITE);

	strcpy(fileName, directoryName);
	strcat(fileName, IMPITEMCHOICESFILENAME);
	SGP_THROW_IFFALSE(ReadInIMPItemChoicesStats(fileName),IMPITEMCHOICESFILENAME);

	// Flugente 2012-12-19: merchant data has been externalised - see XML_Merchants.cpp
	/*std::copy(DefaultarmsDealerInfo, 
			  DefaultarmsDealerInfo + sizeof(DefaultarmsDealerInfo)/sizeof(*DefaultarmsDealerInfo), 
			  armsDealerInfo.begin());*/

	strcpy(fileName, directoryName);
	strcat(fileName, TONYINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gTonyInventory,fileName),TONYINVENTORYFILENAME);

#ifdef JA2UB
//ja25 ub Biggins

	strcpy(fileName, directoryName);
	strcat(fileName, BETTYINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gBettyInventory,fileName),BETTYINVENTORYFILENAME);

	//UB
	strcpy(fileName, directoryName);
	strcat(fileName, RAULINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gRaulInventory,fileName),RAULINVENTORYFILENAME);

#else
	strcpy(fileName, directoryName);
	strcat(fileName, DEVININVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gDevinInventory,fileName),DEVININVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, PERKOINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gPerkoInventory,fileName),PERKOINVENTORYFILENAME);

#endif

	strcpy(fileName, directoryName);
	strcat(fileName, FRANZINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gFranzInventory,fileName),FRANZINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, KEITHINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gKeithInventory,fileName),KEITHINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, SAMINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gSamInventory,fileName),SAMINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, JAKEINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gJakeInventory,fileName),JAKEINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, HOWARDINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gHowardInventory,fileName),HOWARDINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, GABBYINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gGabbyInventory,fileName),GABBYINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, FRANKINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gFrankInventory,fileName),FRANKINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ELGININVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gElginInventory,fileName),ELGININVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, MANNYINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gMannyInventory,fileName),MANNYINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, HERVEINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gHerveInventory,fileName),HERVEINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, PETERINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gPeterInventory,fileName),PETERINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ALBERTOINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gAlbertoInventory,fileName),ALBERTOINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, CARLOINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gCarloInventory,fileName),CARLOINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, MICKEYINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gMickyInventory,fileName),MICKEYINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ARNIEINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArnieInventory,fileName),ARNIEINVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, FREDOINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gFredoInventory,fileName),FREDOINVENTORYFILENAME);

	// Flugente: added new merchants
	strcpy(fileName, directoryName);
	strcat(fileName, TINAINVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gTinaInventory,fileName),TINAINVENTORYFILENAME);
		
	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_1_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[0],fileName),ADITIONALDEALER_1_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_2_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[1],fileName),ADITIONALDEALER_2_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_3_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[2],fileName),ADITIONALDEALER_3_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_4_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[3],fileName),ADITIONALDEALER_4_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_5_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[4],fileName),ADITIONALDEALER_5_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_6_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[5],fileName),ADITIONALDEALER_6_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_7_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[6],fileName),ADITIONALDEALER_7_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_8_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[7],fileName),ADITIONALDEALER_8_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_9_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[8],fileName),ADITIONALDEALER_9_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_10_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[9],fileName),ADITIONALDEALER_10_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_11_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[10],fileName),ADITIONALDEALER_11_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_12_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[11],fileName),ADITIONALDEALER_12_INVENTORYFILENAME);
	
	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_13_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[12],fileName),ADITIONALDEALER_13_INVENTORYFILENAME);
	
	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_14_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[13],fileName),ADITIONALDEALER_14_INVENTORYFILENAME);
	
	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_15_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[14],fileName),ADITIONALDEALER_15_INVENTORYFILENAME);
	
	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_16_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[15],fileName),ADITIONALDEALER_16_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_17_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[16],fileName),ADITIONALDEALER_17_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_18_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[17],fileName),ADITIONALDEALER_18_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_19_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[18],fileName),ADITIONALDEALER_19_INVENTORYFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, ADITIONALDEALER_20_INVENTORYFILENAME);
	SGP_THROW_IFFALSE(ReadInInventoryStats(gArmsDealerAdditional[19],fileName),ADITIONALDEALER_20_INVENTORYFILENAME);
	
	strcpy(fileName, directoryName);
	strcat(fileName, CITYTABLEFILENAME);
	SGP_THROW_IFFALSE(ReadInMapStructure(fileName, FALSE),CITYTABLEFILENAME);

#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInStrategicMapSectorTownNames(fileName,TRUE), fileName);
	}
#endif

	// Lesh: Strategic movement costs will be read in Strategic\Strategic Movement Costs.cpp,
	//		function BOOLEAN InitStrategicMovementCosts();
	//		It is called several times from various places and acts after clearing SectorInfo array

	// Lesh: load altsectors list
	strcpy(fileName, directoryName);
	strcat(fileName, ALTSECTORSFILENAME);
	SGP_THROW_IFFALSE(ReadInAltSectors(fileName),ALTSECTORSFILENAME);

	// Lesh: load samsites - must be after cities.xml
	strcpy(fileName, directoryName);
	strcat(fileName, SAMSITESFILENAME);
	SGP_THROW_IFFALSE(ReadInSAMInfo(fileName),SAMSITESFILENAME);

	// Buggler: load helisites
	strcpy(fileName, directoryName);
	strcat(fileName, HELISITESFILENAME);
	SGP_THROW_IFFALSE(ReadInHeliInfo(fileName),HELISITESFILENAME);

#ifdef JA2UB
	if ( gGameUBOptions.EnemyXML == TRUE ) 
	{
#endif
		// Lesh: army externalization
		strcpy(fileName, directoryName);
		strcat(fileName, GARRISONFILENAME);
		SGP_THROW_IFFALSE(ReadInGarrisonInfo(fileName),GARRISONFILENAME);

		strcpy(fileName, directoryName);
		strcat(fileName, PATROLFILENAME);
		SGP_THROW_IFFALSE(ReadInPatrolInfo(fileName),PATROLFILENAME);
#ifdef JA2UB
	}
#endif

	strcpy(fileName, directoryName);
	strcat(fileName, COMPOSITIONFILENAME);
	SGP_THROW_IFFALSE(ReadInArmyCompositionInfo(fileName),COMPOSITIONFILENAME);

	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIONDATAFILENAME);
	SGP_THROW_IFFALSE(ReadInExplosionDataStats(fileName),EXPLOSIONDATAFILENAME);

	// Kaiden: Read in Restricted Sectors for Mobile Militia
	strcpy(fileName, directoryName);
	strcat(fileName, ROAMINGMILITIAFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInRoamingInfo(fileName),ROAMINGMILITIAFILENAME);

	// Dealtar: Read in shipping destinations and delivery methods
	strcpy(fileName, directoryName);
	strcat(fileName, SHIPPINGDESTINATIONSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInShippingDestinations(fileName, FALSE),SHIPPINGDESTINATIONSFILENAME);

#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInShippingDestinations(fileName, TRUE),fileName);
	}
#endif
	
	strcpy(fileName, directoryName);
	strcat(fileName, DELIVERYMETHODSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInDeliveryMethods(fileName),DELIVERYMETHODSFILENAME);

	// HEADROCK HAM 3.5: Read in facility types
	strcpy(fileName, directoryName);
	strcat(fileName, FACILITYTYPESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInFacilityTypes(fileName,FALSE), FACILITYTYPESFILENAME);
	
#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInFacilityTypes(fileName,TRUE), fileName);
	}
#endif

	// HEADROCK HAM 3.4: Read in facility locations
	strcpy(fileName, directoryName);
	strcat(fileName, SECTORFACILITIESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInSectorFacilities(fileName), SECTORFACILITIESFILENAME);

	// HEADROCK HAM 3.4: Read in dynamic roaming restrictions
	strcpy(fileName, directoryName);
	strcat(fileName, DYNAMICROAMINGFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInDynamicRoamingRestrictions(fileName), DYNAMICROAMINGFILENAME);
	
	// HEADROCK HAM 3.6: Read in customized sector names
	strcpy(fileName, directoryName);
	strcat(fileName, SECTORNAMESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInSectorNames(fileName,FALSE,0), SECTORNAMESFILENAME);

#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInSectorNames(fileName,TRUE, 0), fileName);
	}
#endif	

	// HEADROCK HAM 5: Read in Coolness by Sector
	strcpy(fileName, directoryName);
	strcat(fileName, COOLNESSBYSECTORFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInCoolnessBySector(fileName), COOLNESSBYSECTORFILENAME);

	// WANNE: Why do we need it. It should also run without that file!!
	// SANDRO - always initialize those files, we need it on game start
	if (gGameExternalOptions.fReadProfileDataFromXML)
	{
#ifdef JA2UB		
		// UB25
		strcpy(fileName, directoryName);
		strcat(fileName, MERCPROFILESFILENAME25); 
		
		if ( FileExists(fileName) )
		{	
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));		
		SGP_THROW_IFFALSE(ReadInMercProfiles(fileName, FALSE), MERCPROFILESFILENAME25);
		
		#ifndef ENGLISH
			AddLanguagePrefix(fileName);
			if ( FileExists(fileName) )
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
				if(!ReadInMercProfiles(fileName,TRUE))
				return FALSE;
			}
		#endif
		}

		strcpy(fileName, directoryName);
		strcat(fileName, MERCOPINIONSFILENAME25);
		if ( FileExists(fileName) )
		{		
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInMercOpinions(fileName), MERCOPINIONSFILENAME25);
		}
#else

		// HEADROCK PROFEX: Read in Merc Profile data to replace PROF.DAT data
		strcpy(fileName, directoryName);
		strcat(fileName, MERCPROFILESFILENAME);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInMercProfiles(fileName, FALSE), MERCPROFILESFILENAME);

		#ifndef ENGLISH
			AddLanguagePrefix(fileName);
			if ( FileExists(fileName) )
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
				SGP_THROW_IFFALSE(ReadInMercProfiles(fileName,TRUE), fileName);
			}
		#endif

		// HEADROCK PROFEX: Read in Merc Opinion data to replace PROF.DAT data
		strcpy(fileName, directoryName);
		strcat(fileName, MERCOPINIONSFILENAME);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInMercOpinions(fileName), MERCOPINIONSFILENAME);

		// WANNE: Read in the MercQuotes.xml file
		strcpy(fileName, directoryName);
		strcat(fileName, MERCQUOTEFILENAME);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInMercQuotes(QuoteExp, fileName), MERCQUOTEFILENAME);

#endif
	}
		
	// HEADROCK HAM 3.6: Read in customized Bloodcat Placements
	strcpy(fileName, directoryName);
	strcat(fileName, BLOODCATPLACEMENTSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInBloodcatPlacements(fileName), BLOODCATPLACEMENTSFILENAME);

	// HEADROCK HAM 3.6: Read in customized Uniform Colors
	strcpy(fileName, directoryName);
	strcat(fileName, UNIFORMCOLORSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInUniforms(fileName), UNIFORMCOLORSFILENAME);

    strcpy(fileName, directoryName);
	strcat(fileName, MULTIPLAYERTEAMSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	mpTeams.ReadInMPTeams(fileName);

	//legion by jazz (load small faces from rpcs)	
	strcpy(fileName, directoryName);
	strcat(fileName, RPCFACESSMALLFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInSmallFacesStats(gRPCSmallFaceValues,fileName), RPCFACESSMALLFILENAME);
	ResetEmptyRPCFaceSlots();
		
	//Hidden Names by Jazz 
	strcpy(fileName, directoryName);
	strcat(fileName, HIDDENNAMESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInHiddenNamesStats(zHiddenNames,fileName), HIDDENNAMESFILENAME);	

	
	
	if (gGameExternalOptions.fEnemyNames == TRUE)
    {
		// Enemy Names Group by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, ENEMYNAMESFILENAME);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInEnemyNames(fileName,FALSE), ENEMYNAMESFILENAME);		

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInEnemyNames(fileName,TRUE), fileName);
		}
#endif
	}
	
	
	if (gGameExternalOptions.fCivGroupName == TRUE)
    {
		// Civ Names Group by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, CIVGROUPNAMESFILENAME);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInCivGroupNamesStats(fileName,FALSE), CIVGROUPNAMESFILENAME);

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInCivGroupNamesStats(fileName,TRUE), fileName);
		}
#endif
	}
	
	
		// Sender Name List by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, EMAILSENDERNAMELIST);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInSenderNameList(fileName,FALSE), EMAILSENDERNAMELIST);

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSenderNameList(fileName,TRUE), fileName);
		}
#endif
	
	if (gGameExternalOptions.fEnemyRank == TRUE)
    {
		// Enemy Rank by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, ENEMYRANKFILENAME);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInEnemyRank(fileName,FALSE), ENEMYRANKFILENAME);		

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInEnemyRank(fileName,TRUE), fileName);
		}
#endif
	}

	// Flugente: backgrounds
	strcpy(fileName, directoryName);
	strcat(fileName, BACKGROUNDSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInBackgrounds(fileName,FALSE), BACKGROUNDSFILENAME);
			
#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInBackgrounds(fileName,TRUE), BACKGROUNDSFILENAME);
	}
#endif

	// WANNE: Only in a singleplayer game...
	// Externalised taunts
	if (!is_networked)
	{
		GETFILESTRUCT FileInfo;
		char tauntFileNamePattern[MAX_PATH];
		strcpy(tauntFileNamePattern, directoryName);
		strcat(tauntFileNamePattern, TAUNTSFILENAMEBEGINNING"*"TAUNTSFILENAMEENDING);
		if( GetFileFirst(tauntFileNamePattern, &FileInfo) )
		{
			strcpy(fileName, directoryName);
			strcat(fileName, FileInfo.zFileName);
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInTaunts(fileName,FALSE), fileName);
	#ifndef ENGLISH
			AddLanguagePrefix(fileName);
			if ( FileExists(fileName) )
			{
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
				SGP_THROW_IFFALSE(ReadInTaunts(fileName,TRUE), fileName);
			}
	#endif
			while( GetFileNext(&FileInfo) )
			{
				strcpy(fileName, directoryName);
				strcat(fileName, FileInfo.zFileName);
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
				SGP_THROW_IFFALSE(ReadInTaunts(fileName,FALSE), fileName);
	#ifndef ENGLISH
				AddLanguagePrefix(fileName);
				if ( FileExists(fileName) )
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
					SGP_THROW_IFFALSE(ReadInTaunts(fileName,TRUE), fileName);
				}
	#endif
			}
			GetFileClose(&FileInfo);
		}
	}

	// IMP Portraits List by Jazz
	strcpy(fileName, directoryName);
	strcat(fileName, IMPPORTRAITS);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInIMPPortraits(fileName,FALSE), IMPPORTRAITS);

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInIMPPortraits(fileName,TRUE), fileName);
		}
#endif

	// Flugente: soldier profiles
	if (!is_networked)
	{
		strcpy(fileName, directoryName);
		strcat(fileName, ENEMY_ADMIN_PROFILE_FILENAME);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSoldierProfiles(zSoldierProfile[0], fileName), ENEMY_ADMIN_PROFILE_FILENAME);
			num_found_soldier_profiles[0] = num_found_profiles;
		}

		strcpy(fileName, directoryName);
		strcat(fileName, ENEMY_REGULAR_PROFILE_FILENAME);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSoldierProfiles(zSoldierProfile[1], fileName), ENEMY_REGULAR_PROFILE_FILENAME);
			num_found_soldier_profiles[1] = num_found_profiles;
		}

		strcpy(fileName, directoryName);
		strcat(fileName, ENEMY_ELITE_PROFILE_FILENAME);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSoldierProfiles(zSoldierProfile[2], fileName), ENEMY_ELITE_PROFILE_FILENAME);
			num_found_soldier_profiles[2] = num_found_profiles;
		}

		strcpy(fileName, directoryName);
		strcat(fileName, MILITIA_GREEN_PROFILE_FILENAME);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSoldierProfiles(zSoldierProfile[3], fileName), MILITIA_GREEN_PROFILE_FILENAME);
			num_found_soldier_profiles[3] = num_found_profiles;
		}

		strcpy(fileName, directoryName);
		strcat(fileName, MILITIA_REGULAR_PROFILE_FILENAME);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSoldierProfiles(zSoldierProfile[4], fileName), MILITIA_REGULAR_PROFILE_FILENAME);
			num_found_soldier_profiles[4] = num_found_profiles;
		}

		strcpy(fileName, directoryName);
		strcat(fileName, MILITIA_VETERAN_PROFILE_FILENAME);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInSoldierProfiles(zSoldierProfile[5], fileName), MILITIA_VETERAN_PROFILE_FILENAME);
			num_found_soldier_profiles[5] = num_found_profiles;
		}
	}

	LoadIMPPortraitsTEMP();
	
	//Sound profile by jazz 
	strcpy(fileName, directoryName);
	strcat(fileName, SOUNDPROFILE);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInSoundProfile(fileName), SOUNDPROFILE);

	//Random stats by jazz 
	strcpy(fileName, directoryName);
	strcat(fileName, RANDOMSTATS);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInRandomStats(fileName), RANDOMSTATS);
	//new profiles by Jazz	
	strcpy(fileName, directoryName);
	strcat(fileName, MERCAVAILABILITY);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInMercAvailability(fileName,FALSE), MERCAVAILABILITY);
	
	strcpy(fileName, directoryName);
	strcat(fileName, FACEGEARFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInFaceGear(zNewFaceGear, fileName), FACEGEARFILENAME);
	//WriteFaceGear();
	
#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInMercAvailability(fileName,TRUE), fileName);
		}
#endif
	UINT32 i;
		for(i=0; i<NUM_PROFILES; i++)
		{
			gAimAvailability[i].ProfilId = -1;
		}


	//new profiles by Jazz	
	strcpy(fileName, directoryName);
	strcat(fileName, AIMAVAILABILITY);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInAimAvailability(fileName,FALSE), AIMAVAILABILITY);
	
#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInAimAvailability(fileName,TRUE), fileName);
		}
#endif

	//Main Menu by Jazz	
	strcpy(fileName, directoryName);
	strcat(fileName, LAYOUTMAINMENU);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInMainMenu(gMainMenulayout,fileName), LAYOUTMAINMENU);

	strcpy(fileName, directoryName);
	strcat(fileName, ACTIONITEMSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInActionItems(fileName,FALSE), ACTIONITEMSFILENAME);	

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			if(!ReadInActionItems(fileName,TRUE))
				return FALSE;
		}
#endif

if ( ReadXMLEmail == TRUE )
{
		// EMAIL MERC AVAILABLE by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, EMAILMERCAVAILABLE);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInEmailMercAvailable(fileName,FALSE), EMAILMERCAVAILABLE);

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			if(!ReadInEmailMercAvailable(fileName,TRUE))
				return FALSE;
		}
#endif

		// EMAIL MERC LEVEL UP by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, EMAILMERCLEVELUP);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInEmailMercLevelUp(fileName,FALSE), EMAILMERCLEVELUP);

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			if(!ReadInEmailMercLevelUp(fileName,TRUE))
				return FALSE;
		}
#endif
}
/*
		// EMAIL OTHER by Jazz
		strcpy(fileName, directoryName);
		strcat(fileName, EMAILOTHER);
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInEmailOther(fileName,FALSE), EMAILOTHER);

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			if(!ReadInEmailOther(fileName,TRUE))
				return FALSE;
		}
#endif
*/
	//new vehicles by Jazz	
	
	InitNewVehicles ();
	
	strcpy(fileName, directoryName);
	strcat(fileName, VEHICLESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInNewVehicles(fileName,FALSE), VEHICLESFILENAME);
	
#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			if(!ReadInNewVehicles(fileName,TRUE))
				return FALSE;
		}
#endif

#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			if(!ReadInLanguageLocation(fileName,TRUE,zlanguageText,0))
				return FALSE;
		}
#endif

#ifdef ENABLE_BRIEFINGROOM
	strcpy(fileName, directoryName);
	strcat(fileName, BRIEFINGROOMFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInBriefingRoom(fileName,FALSE,gBriefingRoomData, 4), BRIEFINGROOMFILENAME);
	
#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInBriefingRoom(fileName,TRUE,gBriefingRoomData, 4), fileName);
		}
#endif

BackupBRandEncyclopedia ( gBriefingRoomData, gBriefingRoomDataBackup, 0);

#endif //ENABLE_BRIEFINGROOM

	// Mine Types (Minerals)
	strcpy(fileName, directoryName);
	strcat(fileName, MINERALSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInMinerals(fileName,FALSE), MINERALSFILENAME);
	
#ifndef ENGLISH
	AddLanguagePrefix(fileName);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		SGP_THROW_IFFALSE(ReadInMinerals(fileName,TRUE), fileName);
	}
#endif

	// Old AIM Archive
		UINT8 p;
		for(p=0; p<NUM_PROFILES; p++)
		{
			gAimOldArchives[p].FaceID = -1;
		}

	strcpy(fileName, directoryName);
	strcat(fileName, OLDAIMARCHIVEFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	SGP_THROW_IFFALSE(ReadInAimOldArchive(fileName,FALSE), OLDAIMARCHIVEFILENAME);
	
#ifndef ENGLISH
		AddLanguagePrefix(fileName);
		if ( FileExists(fileName) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
			SGP_THROW_IFFALSE(ReadInAimOldArchive(fileName,TRUE), fileName);
		}
#endif
		UINT8 emptyslotsinarchives = 0;
		for (p=0;p<NUM_PROFILES;p++)
		{
			if (gAimOldArchives[p].FaceID == -1)
			{
				emptyslotsinarchives++;
			}
			else
			{
				gAimOldArchives[p-emptyslotsinarchives]=gAimOldArchives[p];
			}
		}
		for (p=NUM_PROFILES;p>NUM_PROFILES-emptyslotsinarchives;p--)
		{
			gAimOldArchives[p-1].FaceID=-1;
		}
		
		#ifdef NEWMUSIC
		UINT32 iloop;
		UINT32 zloop;
		for(iloop=0; iloop<256; iloop++)
		{
			for(zloop=0; zloop<4; zloop++)
			{
				MusicSoundValues[iloop].uiIndex = iloop;
				MusicSoundValues[iloop].SoundTacticalVictory[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalBattle[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalNothing[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalTensor[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalDeath[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalBattleCreature[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalBattleCreepy[zloop] = -1;
				MusicSoundValues[iloop].SoundTacticalBattleGroup[iloop] = -1;
			}
		}
		
		CHAR8 zFileName[255];
		sprintf( zFileName, "scripts\\Music.lua" );
		if (FileExists( zFileName ) )
		{
			LetLuaMusicControl(0);
		}
		#endif
	LuaState::INIT(lua::LUA_STATE_STRATEGIC_MINES_AND_UNDERGROUND, true);
	g_luaUnderground.LoadScript(GetLanguagePrefix());
	// load Lua for Strategic Mines initialization
	g_luaMines.LoadScript();

	return TRUE;
}


UINT32 InitializeJA2(void)
{

#ifdef LASERLOCK_ENABLED
	HandleLaserLockResult( PrepareLaserLockSystem() );
#endif

	HandleJA2CDCheck( );

	gfWorldLoaded = FALSE;
	
	//Load external game mechanic data
	//if ( !LoadExternalGameplayData(TABLEDATA_DIRECTORY))
	//{
	//	return( ERROR_SCREEN );
	//}
	SGP_TRYCATCH_RETHROW(LoadExternalGameplayData(TABLEDATA_DIRECTORY),L"Loading external data failed");

	// Load external text
	LoadAllExternalText();

	// Init JA2 sounds
	InitJA2Sound( );

	//dnl ch54 111009
	//gsRenderCenterX = 805;
	//gsRenderCenterY = 805;
	
	// Init data
	InitializeSystemVideoObjects( );

	// Init animation system
	if ( !InitAnimationSystem( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init lighting system
	InitLightingSystem();

	// Init dialog queue system
	InitalizeDialogueControl();

	if ( !InitStrategicEngine( ) )
	{
		return( ERROR_SCREEN );
	}

	//needs to be called here to init the SectorInfo struct
	if ( !InitStrategicMovementCosts( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init tactical engine
	if ( !InitTacticalEngine( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init timer system
	//Moved to the splash screen code.
	//InitializeJA2Clock( );

	// INit shade tables
	BuildShadeTable( );

	// INit intensity tables
	BuildIntensityTable( );

	// Init Event Manager
	if ( !InitializeEventManager( ) )
	{
		return( ERROR_SCREEN );
	}

	// Initailize World
	if ( !InitializeWorld( ) )
	{
		return( ERROR_SCREEN );
	}

	InitTileCache( );

	InitMercPopupBox( );

	// Set global volume
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );

	DetermineRGBDistributionSettings();

	// Snap: Init save game directory
	if ( !InitSaveDir() )
	{
		return( ERROR_SCREEN );
	}

	//ADB When a merc calcs CTGT for a thrown item he uses a GLOCK temp item
	//but we don't want to recreate it every single time CTGT is called, so init the GLOCK here
	CreateItem(GLOCK_17, 100, &GLOCK_17_ForUseWithLOS);
	

#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR

	//UNCOMMENT NEXT LINE TO ALLOW FORCE UPDATES...
	//LoadGlobalSummary();
	if( gfMustForceUpdateAllMaps )
	{
		ApologizeOverrideAndForceUpdateEverything();
	}
	#endif
#endif

#ifdef JA2BETAVERSION
	if( ProcessIfMultilingualCmdLineArgDetected( gzCommandLine ) )
	{ //If the multilingual text code generator has activated, quit now.
		gfProgramIsRunning = FALSE;
		return( INIT_SCREEN );
	}
#endif

#ifdef JA2BETAVERSION
#ifdef JA2EDITOR
	// CHECK COMMANDLINE FOR SPECIAL UTILITY
	if ( strcmp( gzCommandLine, "-DOMAPS" ) == 0 )
	{
		GenerateAllMapsInit();//dnl ch49 061009
		return( MAPUTILITY_SCREEN );
	}
#endif
#endif

#ifdef JA2BETAVERSION
	//This allows the QuickSave Slots to be autoincremented, ie everytime the user saves, there will be a new quick save file
	if ( _stricmp( gzCommandLine, "-quicksave" ) == 0 )
	{
		gfUseConsecutiveQuickSaveSlots = TRUE;
	}
#endif

#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR
	
		// CHECK COMMANDLINE FOR SPECIAL UTILITY
		if( !strcmp( gzCommandLine, "-EDITORAUTO" ) )
		{
			OutputDebugString( "Beginning JA2 using -EDITORAUTO commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = FALSE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			//gGameOptions.fBobbyRayFastShipments = FALSE;
			gGameOptions.fInventoryCostsAP = FALSE;
			return( GAME_SCREEN );
		}
		if ( strcmp( gzCommandLine, "-EDITOR" ) == 0 )
		{
			OutputDebugString( "Beginning JA2 using -EDITOR commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = FALSE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			//gGameOptions.fBobbyRayFastShipments = FALSE;
			gGameOptions.fInventoryCostsAP = FALSE;
			return( GAME_SCREEN );
		}

		#ifdef BUILD_AS_EDITOR_ONLY
			//ADB We are building with JA2EDITOR, why force a commandline arguement???
			//build once, rename, change the define and build again
			//no pesky shortcuts
			OutputDebugString( "Beginning JA2EDITOR without using a commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = TRUE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			//gGameOptions.fBobbyRayFastShipments = FALSE;
			gGameOptions.fInventoryCostsAP = FALSE;
			return( GAME_SCREEN );
		#endif
	#endif
#endif

#ifdef JA2UB
	InitGridNoUB();
#endif

//Lua
IniLuaGlobal();

	return( INIT_SCREEN );
}


void ShutdownJA2(void)
{
	UINT32 uiIndex;

	// Clear screen....
	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	InvalidateScreen( );
	// Remove cursor....
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	RefreshScreen( NULL );

	ShutdownStrategicLayer();

	// remove temp files built by laptop
	ClearOutTempLaptopFiles( );

	// Shutdown queue system
	ShutdownDialogueControl();

	// Shutdown Screens
	for (uiIndex = 0; uiIndex < MAX_SCREENS; uiIndex++)
	{
	(*(GameScreens[uiIndex].ShutdownScreen))();
	}


	// Shutdown animation system
	DeInitAnimationSystem( );

	ShutdownLightingSystem();

	CursorDatabaseClear();

	ShutdownTacticalEngine( );

	// Shutdown Overhead
	ShutdownOverhead( );

	DeinitializeWorld( );

	DeleteTileCache( );

	ShutdownJA2Clock( );

	ShutdownFonts();

	ShutdownJA2Sound( );

	ShutdownEventManager( );

	ShutdownBaseDirtyRectQueue( );

	// Unload any text box images!
	RemoveTextMercPopupImages( );

	ClearOutVehicleList();
}


#ifdef LASERLOCK_ENABLED

BOOLEAN PrepareLaserLockSystem()
{
	INT32	iInitRetVal=0;
	INT32	iRunRetVal=0;
	INT32	iCheckRetVal=0;
	CHAR8 zDirectory[512];

	CHAR8		zCdLocation[ SGPFILENAME_LEN ];
	CHAR8		zCdFile[ SGPFILENAME_LEN ];

	//Get the "current" file directory
	GetFileManCurrentDirectory( zDirectory );

	if( GetCDromDriveLetter( zCdLocation ) )
	{
		// OK, build filename
		sprintf( zCdFile, "%s%s", zCdLocation, "Data" );
	}
	else
	{
		goto FAILED_LASERLOK;
	}

	//Go back to the root directory
	SetFileManCurrentDirectory( zCdFile );
	//Init the laser lock system
	iInitRetVal = LASERLOK_Init( ghInstance );
	if( iInitRetVal != 0 )
		goto FAILED_LASERLOK;

	//makes the verification of the laserlok system
	iRunRetVal = LASERLOK_Run();
	if( iRunRetVal != 0 )
		goto FAILED_LASERLOK;

	//checks the result of the laserlok run function
	iCheckRetVal = LASERLOK_Check();
	if( iCheckRetVal != 0 )
		goto FAILED_LASERLOK;

	//Restore back to the proper directory
	SetFileManCurrentDirectory( zDirectory );
	return( TRUE );

FAILED_LASERLOK:
	//Restore back to the proper directory
	SetFileManCurrentDirectory( zDirectory );
	return( FALSE );
}

void HandleLaserLockResult( BOOLEAN fSuccess )
{
	if( !fSuccess )
	{
		CHAR8	zString[512];

		sprintf( zString, "%S", gzLateLocalizedString[56] );

//		ShowCursor(TRUE);
//		ShowCursor(TRUE);
		ShutdownWithErrorBox( zString );
	}
}

#endif

void SetupMaxActionPointsAnimation()
{
	gubMaxActionPoints[0] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[1] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[2] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[3] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[4] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[5] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[6] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[7] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[8] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[9] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[10] = APBPConstants[AP_MONSTER_MAXIMUM];
	gubMaxActionPoints[11] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[12] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[13] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[14] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[15] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[16] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[17] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[18] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[19] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[20] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[21] = APBPConstants[AP_MAXIMUM];
	gubMaxActionPoints[22] = APBPConstants[AP_VEHICLE_MAXIMUM];
	gubMaxActionPoints[23] = APBPConstants[AP_VEHICLE_MAXIMUM];
	gubMaxActionPoints[24] = APBPConstants[AP_VEHICLE_MAXIMUM];
	gubMaxActionPoints[25] = APBPConstants[AP_VEHICLE_MAXIMUM];
	gubMaxActionPoints[26] = APBPConstants[AP_VEHICLE_MAXIMUM];
	gubMaxActionPoints[27] = APBPConstants[AP_VEHICLE_MAXIMUM];
}
