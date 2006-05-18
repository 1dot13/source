#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "HelpScreen.h"
	#include "Multilingual Text Code Generator.h"
	#include "INIReader.h"
	
#else
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
#endif

extern BOOLEAN GetCDromDriveLetter( STR8	pString );

// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game

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


BOOLEAN LoadExternalGameplayData(STR directoryName)
{
	char fileName[MAX_PATH];

	strcpy(fileName, directoryName);
	strcat(fileName, AMMOTYPESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));

	if(!ReadInAmmoTypeStats(fileName))
		return FALSE;

	//if(!WriteAmmoTypeStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, AMMOFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInAmmoStats(fileName))
		return FALSE;

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
	if(!ReadInItemStats(fileName))
		return FALSE;

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
	if(!ReadInMapStructure(fileName))
		return FALSE;
		
	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIONDATAFILENAME);
    if(!ReadInExplosionDataStats(fileName))
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
	InitStrategicMovementCosts( );

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
	if( ProcessIfMultilingualCmdLineArgDetected( (UINT8 *)gzCommandLine ) )
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
			sprintf( (char *)gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = TRUE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			return( GAME_SCREEN );
		}
		if ( strcmp( gzCommandLine, "-EDITOR" ) == 0 )
		{
			OutputDebugString( "Beginning JA2 using -EDITOR commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( (char *)gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = FALSE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			return( GAME_SCREEN );
		}
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

    ShowCursor(TRUE);
    ShowCursor(TRUE);
		ShutdownWithErrorBox( zString );
	}
}

#endif
