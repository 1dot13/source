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
	#include "Wcheck.h"
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
	#include "cursors.h"
	#include "Sound Control.h"
	#include "Event Pump.h"
	#include "lighting.h"
	#include "Cursor Control.h"
	#include "music control.h"
	#include "video.h"
	#include "sys globals.h"
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
#endif

#include "Sector Summary.h"
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


BOOLEAN LoadExternalGameplayData(STR directoryName)
{
	char fileName[MAX_PATH];

	// WANNE: Enemy drops - begin
	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYMISCDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyMiscDropsStats(gEnemyMiscDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYEXPLOSIVEDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyExplosiveDropsStats(gEnemyExplosiveDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYWEAPONDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyWeaponDropsStats(gEnemyWeaponDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYAMMODROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyAmmoDropsStats(gEnemyAmmoDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYARMOURDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyArmourDropsStats(gEnemyArmourDrops, fileName))
		return FALSE;
	// WANNE: Enemy drops - end

	// WANNE: Sector Loadscreens [2007-05-18]
	strcpy(fileName, directoryName);
	strcat(fileName, SECTORLOADSCREENSFILENAME);

	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInSectorLoadscreensStats(gSectorLoadscreens, fileName))
			return FALSE;
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
	if(!ReadInAmmoTypeStats(fileName))
		return FALSE;

//Madd: Simple Localization
//Read in the correct ammostring file for the given language
	strcpy(fileName, directoryName);
#ifdef GERMAN
	strcat(fileName, GERMAN_PREFIX); // add German. prefix to filename
#endif
#ifdef RUSSIAN
	strcat(fileName, RUSSIAN_PREFIX); // add Russian. prefix to filename
#endif
#ifdef DUTCH
	strcat(fileName, DUTCH_PREFIX); // add Polish. prefix to filename
#endif
#ifdef POLISH
	strcat(fileName, POLISH_PREFIX); // add Polish. prefix to filename
#endif
#ifdef FRENCH
	strcat(fileName, FRENCH_PREFIX); // add Polish. prefix to filename
#endif
#ifdef ITALIAN
	strcat(fileName, ITALIAN_PREFIX); // add Polish. prefix to filename
#endif
#ifdef TAIWANESE
	strcat(fileName, TAIWANESE_PREFIX); // add Polish. prefix to filename
#endif
#ifdef CHINESE
	strcat(fileName, CHINESE_PREFIX); // add Polish. prefix to filename
#endif
	strcat(fileName, AMMOFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInAmmoStats(fileName))
	{
		//CHRISL: If we fail to load, try loading just the default english
		strcpy(fileName, directoryName);
		strcat(fileName, AMMOFILENAME);
		if(!ReadInAmmoStats(fileName))
			return FALSE;
	}


	// Lesh: added this, begin
	strcpy(fileName, directoryName);
	strcat(fileName, BURSTSOUNDSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInBurstSoundArray(fileName))
		return FALSE;
	// Lesh: end

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInItemStats(fileName,FALSE))
		return FALSE;

//Madd: Simple localization
// The idea here is that we can have a separate xml file that's named differently
// but only contains the relevant tags that need to be localized
// then when the file is read in using the same xml reader code, it will only overwrite
// the tags that are contained in the localized file.	This only works for items.xml 
// since I tweaked the xml_items.cpp to make it work :p
// So for instance, the german file would be called German.Items.xml and would only contain
// the uiIndex (for reference), szItemName, szLongItemName, szItemDesc, szBRName, and szBRDesc tags

#ifdef GERMAN
	AddLanguagePrefix( fileName, GERMAN_PREFIX);
#endif
#ifdef RUSSIAN
	AddLanguagePrefix( fileName, RUSSIAN_PREFIX);
#endif
#ifdef DUTCH
	AddLanguagePrefix( fileName, DUTCH_PREFIX);
#endif
#ifdef POLISH
	AddLanguagePrefix( fileName, POLISH_PREFIX);
#endif
#ifdef FRENCH
	AddLanguagePrefix( fileName, FRENCH_PREFIX);
#endif
#ifdef ITALIAN
	AddLanguagePrefix( fileName, ITALIAN_PREFIX);
#endif
#ifdef TAIWANESE
	AddLanguagePrefix( fileName, TAIWANESE_PREFIX);
#endif
#ifdef CHINESE
	AddLanguagePrefix( fileName, CHINESE_PREFIX);
#endif
#ifndef ENGLISH
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInItemStats(fileName,TRUE))
			return FALSE;
	}
#endif

	//if(!WriteItemStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, SOUNDSFILENAME);
	if(!ReadInSoundArray(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, MAGAZINESFILENAME);
	if(!ReadInMagazineStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTSFILENAME);
	if(!ReadInAttachmentStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTINFOFILENAME);
	if(!ReadInAttachmentInfoStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, LAUNCHABLESFILENAME);
	if(!ReadInLaunchableStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, COMPATIBLEFACEITEMSFILENAME);
	if(!ReadInCompatibleFaceItemStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, MERGESFILENAME);
	if(!ReadInMergeStats(fileName))
		return FALSE;

	//if(!WriteMergeStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTCOMBOMERGESFILENAME);
	if(!ReadInAttachmentComboMergeStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIVESFILENAME);
	if(!ReadInExplosiveStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ARMOURSFILENAME);
	if(!ReadInArmourStats(fileName))
		return FALSE;

	// CHRISL:
	strcpy(fileName, directoryName);
	strcat(fileName, LOADBEARINGEQUIPMENTFILENAME);
	if(!ReadInlbeStats(fileName))
		return FALSE;

	// CHRISL:
	strcpy(fileName, directoryName);
	strcat(fileName, LBEPOCKETFILENAME);
	if(!ReadInLBEPocketStats(fileName,FALSE))
		return FALSE;

//CHRISL: Simple localization
// Same setup as what Madd used for items.xml

#ifdef GERMAN
	AddLanguagePrefix( fileName, GERMAN_PREFIX);
#endif
#ifdef RUSSIAN
	AddLanguagePrefix( fileName, RUSSIAN_PREFIX);
#endif
#ifdef DUTCH
	AddLanguagePrefix( fileName, DUTCH_PREFIX);
#endif
#ifdef POLISH
	AddLanguagePrefix( fileName, POLISH_PREFIX);
#endif
#ifdef FRENCH
	AddLanguagePrefix( fileName, FRENCH_PREFIX);
#endif
#ifdef ITALIAN
	AddLanguagePrefix( fileName, ITALIAN_PREFIX);
#endif
#ifdef TAIWANESE
	AddLanguagePrefix( fileName, TAIWANESE_PREFIX);
#endif
#ifdef CHINESE
	AddLanguagePrefix( fileName, CHINESE_PREFIX);
#endif
#ifndef ENGLISH
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInLBEPocketStats(fileName,TRUE))
			return FALSE;
	}
#endif

	// CHRISL:
	strcpy(fileName, directoryName);
	strcat(fileName, MERCSTARTINGGEARFILENAME);
	if(!ReadInMercStartingGearStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, WEAPONSFILENAME);
	if(!ReadInWeaponStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, INCOMPATIBLEATTACHMENTSFILENAME);
	if(!ReadInIncompatibleAttachmentStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYGUNCHOICESFILENAME);
	if(!ReadInExtendedArmyGunChoicesStats (fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYITEMCHOICESFILENAME);
	if(!ReadInArmyItemChoicesStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, IMPITEMCHOICESFILENAME);
	if(!ReadInIMPItemChoicesStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, TONYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gTonyInventory,fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, DEVININVENTORYFILENAME);
	if(!ReadInInventoryStats(gDevinInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, FRANZINVENTORYFILENAME);
	if(!ReadInInventoryStats(gFranzInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, KEITHINVENTORYFILENAME);
	if(!ReadInInventoryStats(gKeithInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, SAMINVENTORYFILENAME);
	if(!ReadInInventoryStats(gSamInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, JAKEINVENTORYFILENAME);
	if(!ReadInInventoryStats(gJakeInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, HOWARDINVENTORYFILENAME);
	if(!ReadInInventoryStats(gHowardInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, GABBYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gGabbyInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, FRANKINVENTORYFILENAME);
	if(!ReadInInventoryStats(gFrankInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, ELGININVENTORYFILENAME);
	if(!ReadInInventoryStats(gElginInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, MANNYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gMannyInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, HERVEINVENTORYFILENAME);
	if(!ReadInInventoryStats(gHerveInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, PETERINVENTORYFILENAME);
	if(!ReadInInventoryStats(gPeterInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, ALBERTOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gAlbertoInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, CARLOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gCarloInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, MICKEYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gMickyInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, ARNIEINVENTORYFILENAME);
	if(!ReadInInventoryStats(gArnieInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, PERKOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gPerkoInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, FREDOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gFredoInventory,fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, CITYTABLEFILENAME);
	if(!ReadInMapStructure(fileName, FALSE))
		return FALSE;

#ifdef GERMAN 
	AddLanguagePrefix( fileName, GERMAN_PREFIX);
#endif
#ifdef RUSSIAN 
	AddLanguagePrefix( fileName, RUSSIAN_PREFIX);
#endif
#ifdef DUTCH 
	AddLanguagePrefix( fileName, DUTCH_PREFIX);
#endif
#ifdef POLISH 
	AddLanguagePrefix( fileName, POLISH_PREFIX);
#endif
#ifdef FRENCH 
	AddLanguagePrefix( fileName, FRENCH_PREFIX);
#endif
#ifdef ITALIAN 
	AddLanguagePrefix( fileName, ITALIAN_PREFIX);
#endif
#ifdef TAIWANESE 
	AddLanguagePrefix( fileName, TAIWANESE_PREFIX);
#endif
#ifdef CHINESE
	AddLanguagePrefix( fileName, TAIWANESE_PREFIX);
#endif
#ifndef ENGLISH
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInStrategicMapSectorTownNames(fileName, TRUE))
			return FALSE;
	}
#endif

	// Lesh: Strategic movement costs will be read in Strategic\Strategic Movement Costs.cpp,
	//		function BOOLEAN InitStrategicMovementCosts();
	//		It is called several times from various places and acts after clearing SectorInfo array

	// Lesh: load altsectors list
	strcpy(fileName, directoryName);
	strcat(fileName, ALTSECTORSFILENAME);
	if ( !ReadInAltSectors(fileName) )
		return FALSE;

	// Lesh: load samsites - must be after cities.xml
	strcpy(fileName, directoryName);
	strcat(fileName, SAMSITESFILENAME);
	if ( !ReadInSAMInfo(fileName) )
		return FALSE;

	// Lesh: army externalization
	strcpy(fileName, directoryName);
	strcat(fileName, GARRISONFILENAME);
	if ( !ReadInGarrisonInfo(fileName) )
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, PATROLFILENAME);
	if ( !ReadInPatrolInfo(fileName) )
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, COMPOSITIONFILENAME);
	if ( !ReadInArmyCompositionInfo(fileName) )
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIONDATAFILENAME);
	if(!ReadInExplosionDataStats(fileName))
		return FALSE;

	// Kaiden: Read in Restricted Sectors for Mobile Militia
	strcpy(fileName, directoryName);
	strcat(fileName, ROAMINGMILITIAFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInRoamingInfo(fileName))
		return FALSE;

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
	if ( !LoadExternalGameplayData(TABLEDATA_DIRECTORY))
	{
		return( ERROR_SCREEN );
	}

	// Load external text
	LoadAllExternalText();

	// Init JA2 sounds
	InitJA2Sound( );

	gsRenderCenterX = 805;
	gsRenderCenterY = 805;


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
	// CHECK COMMANDLINE FOR SPECIAL UTILITY
	if ( strcmp( gzCommandLine, "-DOMAPS" ) == 0 )
	{
		return( MAPUTILITY_SCREEN );
	}
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
			return( GAME_SCREEN );
		#endif
	#endif
#endif

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
