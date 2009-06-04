#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "HelpScreen.h"
	#include "Campaign.h"
	#include "Cheats.h"
	#include "INIReader.h"
#else
	#include	"Types.h"
	#include	"GameSettings.h"
	#include	"FileMan.h"
	#include	"String.h"
	#include	"Sound Control.h"
	#include	"SaveLoadScreen.h"
	#include	"Music Control.h"
	#include	"Options Screen.h"
	#include	"Overhead.h"
	#include	"GameVersion.h"
	#include	"LibraryDataBase.h"
	#include	"Debug.h"
	#include	"Language Defines.h"
	#include "HelpScreen.h"
	#include "INIReader.h"
	#include "Shade Table Util.h"
	#include "soldier profile type.h"
	#include "Random.h"
	#include "SaveLoadGame.h"
	#include "sgp.h"
	#include "screenids.h"
	#include "Font Control.h"
	#include "message.h"
	#include "Campaign.h"
	#include "meanwhile.h"
	#include "strategicmap.h"
	#include "Queen Command.h"
	#include "Game Clock.h"
	#include "Init.h"
	#include "displaycover.h"
#endif

#include "Text.h"
#include "connect.h"

#include "VFS/vfs.h"
#include "VFS/vfs_file_raii.h"
#include "VFS/File/vfs_file.h"

#define				GAME_SETTINGS_FILE				"Ja2_Settings.INI"

#define				GAME_INI_FILE					"..\\Ja2.ini"

#define				GAME_EXTERNAL_OPTIONS_FILE		"Ja2_Options.ini"

#define				AP_BP_CONSTANTS_FILE			"APBPConstants.ini"

#define				CD_ROOT_DIR						"DATA\\"

GAME_SETTINGS		gGameSettings;
GAME_OPTIONS		gGameOptions;
// Snap: Options read from an INI file in the default of custom Data directory
GAME_EXTERNAL_OPTIONS gGameExternalOptions;

extern	SGPFILENAME	gCheckFilenames[];
extern	CHAR8		gzErrorMsg[256];
extern INT16 APBPConstants[TOTAL_APBP_VALUES];

void		InitGameSettings();


BOOLEAN GetCdromLocationFromIniFile( STR pRootOfCdromDrive );

extern BOOLEAN DoJA2FilesExistsOnDrive( CHAR8 *zCdLocation );

BOOLEAN GetCDromDriveLetter( STR8	pString );
BOOLEAN	IsDriveLetterACDromDrive( STR pDriveLetter );
void		CDromEjectionErrorMessageBoxCallBack( UINT8 bExitValue );


bool UsingNewInventorySystem()
{
	return (gGameOptions.ubInventorySystem == INVENTORY_NEW);
}

std::string StringToLower(std::string strToConvert)
{//change each element of the string to lower case
   for(unsigned int i=0;i<strToConvert.length();i++)
   {
      strToConvert[i] = tolower(strToConvert[i]);
   }
   return strToConvert;//return the converted string
}

BOOLEAN IsNIVModeValid(bool checkRes)
{
	if(iResolution == 0 && checkRes == true)
		return( FALSE );
#ifndef USE_VFS
	if(gCustomDataCat.GetRootDir() == "")
		return( FALSE );
	char customDataPath[MAX_PATH];
	GetPrivateProfileString( "Ja2 Settings","CUSTOM_DATA_LOCATION", "", customDataPath, MAX_PATH, "..\\Ja2.ini" );
	if(StringToLower((std::string)customDataPath) == "data")
		return( FALSE );
	return( TRUE );
#else
	// Check if the Profile with the NAME = "v1.13" is found in the specificed vfs_config.*.ini
	if(GetVFS()->GetProfileStack()->GetProfile(L"v1.13") != NULL)
	{
		return TRUE;
	}
	return FALSE;
#endif
}


BOOLEAN LoadGameSettings()
{

	CIniReader iniReader(GAME_SETTINGS_FILE, TRUE);	// force path even for non existing files

	//memset( &gGameSettings2, 0, sizeof( GAME_SETTINGS ) ); // Blank out GameSettings

	if ( ! iniReader.Is_CIniReader_File_Found() )
	{
		// file does not exist, InitGamesettings() and then return. 
		// InitGamesettings() will also call SaveGameSettings().
		InitGameSettings();
		return(FALSE);
	}
	
	gGameSettings.bLastSavedGameSlot                                = iniReader.ReadInteger("JA2 Game Settings","bLastSavedGameSlot"                       ,  -1        , -1 , NUM_SAVE_GAMES );
	gGameSettings.ubMusicVolumeSetting                              = iniReader.ReadInteger("JA2 Game Settings","ubMusicVolumeSetting"                     ,  MIDVOLUME ,  0 , HIGHVOLUME );
	gGameSettings.ubSoundEffectsVolume                              = iniReader.ReadInteger("JA2 Game Settings","ubSoundEffectsVolume"                     ,  MIDVOLUME ,  0 , HIGHVOLUME );
	gGameSettings.ubSpeechVolume                                    = iniReader.ReadInteger("JA2 Game Settings","ubSpeechVolume"                           ,  MIDVOLUME ,  0 , HIGHVOLUME );
	gGameSettings.uiMeanwhileScenesSeenFlags                        = iniReader.ReadUINT32 ("JA2 Game Settings","uiMeanwhileScenesSeenFlags"               ,  0         ,  0 , UINT_MAX );
	gGameSettings.fHideHelpInAllScreens                             = iniReader.ReadBoolean("JA2 Game Settings","fHideHelpInAllScreens"                    ,  FALSE );
	gGameSettings.ubSizeOfDisplayCover                              = iniReader.ReadInteger("JA2 Game Settings","ubSizeOfDisplayCover"                     ,  DC__MIN_SIZE , DC__MIN_SIZE , DC__MAX_SIZE );
	gGameSettings.ubSizeOfLOS                                       = iniReader.ReadInteger("JA2 Game Settings","ubSizeOfLOS"                              ,  DC__MIN_SIZE , DC__MIN_SIZE , DC__MAX_SIZE );
	gGameSettings.fOptions[TOPTION_SPEECH]                          = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SPEECH"                           ,  TRUE  );
	gGameSettings.fOptions[TOPTION_MUTE_CONFIRMATIONS]              = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_MUTE_CONFIRMATIONS"               ,  FALSE );
	gGameSettings.fOptions[TOPTION_SUBTITLES]                       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SUBTITLES"                        ,  TRUE  );
	gGameSettings.fOptions[TOPTION_KEY_ADVANCE_SPEECH]              = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_KEY_ADVANCE_SPEECH"               ,  FALSE );
	gGameSettings.fOptions[TOPTION_ANIMATE_SMOKE]                   = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_ANIMATE_SMOKE"                    ,  TRUE  );
	gGameSettings.fOptions[TOPTION_BLOOD_N_GORE]                    = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_BLOOD_N_GORE"                     ,  TRUE  );
	gGameSettings.fOptions[TOPTION_DONT_MOVE_MOUSE]                 = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_DONT_MOVE_MOUSE"                  ,  FALSE );
	gGameSettings.fOptions[TOPTION_OLD_SELECTION_METHOD]            = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_OLD_SELECTION_METHOD"             ,  FALSE );
	gGameSettings.fOptions[TOPTION_ALWAYS_SHOW_MOVEMENT_PATH]       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_ALWAYS_SHOW_MOVEMENT_PATH"        ,  FALSE );
	gGameSettings.fOptions[TOPTION_SHOW_MISSES]                     = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SHOW_MISSES"                      ,  FALSE );
	gGameSettings.fOptions[TOPTION_RTCONFIRM]                       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_RTCONFIRM"                        ,  FALSE );
	gGameSettings.fOptions[TOPTION_SLEEPWAKE_NOTIFICATION]          = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SLEEPWAKE_NOTIFICATION"           ,  TRUE  );
	gGameSettings.fOptions[TOPTION_USE_METRIC_SYSTEM]               = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_USE_METRIC_SYSTEM"                ,  FALSE );
	gGameSettings.fOptions[TOPTION_MERC_ALWAYS_LIGHT_UP]            = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_MERC_ALWAYS_LIGHT_UP"             ,  FALSE );
	gGameSettings.fOptions[TOPTION_SMART_CURSOR]                    = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SMART_CURSOR"                     ,  FALSE );
	gGameSettings.fOptions[TOPTION_SNAP_CURSOR_TO_DOOR]             = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SNAP_CURSOR_TO_DOOR"              ,  TRUE  );
	gGameSettings.fOptions[TOPTION_GLOW_ITEMS]                      = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_GLOW_ITEMS"                       ,  TRUE  );
	gGameSettings.fOptions[TOPTION_TOGGLE_TREE_TOPS]                = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_TOGGLE_TREE_TOPS"                 ,  TRUE  );
	gGameSettings.fOptions[TOPTION_TOGGLE_WIREFRAME]                = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_TOGGLE_WIREFRAME"                 ,  TRUE  );
	gGameSettings.fOptions[TOPTION_3D_CURSOR]                       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_3D_CURSOR"                        ,  FALSE );
	gGameSettings.fOptions[TOPTION_CTH_CURSOR]                      = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_CTH_CURSOR"                       ,  TRUE  );
	gGameSettings.fOptions[TOPTION_GL_BURST_CURSOR]                 = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_GL_BURST_CURSOR"                  ,  TRUE  );
	gGameSettings.fOptions[TOPTION_DROP_ALL]                        = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_DROP_ALL"                         ,  FALSE );
	gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE]                   = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_GL_HIGH_ANGLE"                    ,  FALSE );
	gGameSettings.fOptions[TOPTION_AIM_LEVEL_RESTRICTION]           = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_AIM_LEVEL_RESTRICTION"            ,  TRUE  );
	gGameSettings.fOptions[TOPTION_SPACE_SELECTS_NEXT_SQUAD]        = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SPACE_SELECTS_NEXT_SQUAD"         ,  TRUE  );
	gGameSettings.fOptions[TOPTION_SHOW_ITEM_SHADOW]                = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SHOW_ITEM_SHADOW"                 ,  TRUE  );
	gGameSettings.fOptions[TOPTION_SHOW_WEAPON_RANGE_IN_TILES]      = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SHOW_WEAPON_RANGE_IN_TILES"       ,  TRUE  );
	gGameSettings.fOptions[TOPTION_TRACERS_FOR_SINGLE_FIRE]         = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_TRACERS_FOR_SINGLE_FIRE"          ,  FALSE );
	gGameSettings.fOptions[TOPTION_RAIN_SOUND]                      = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_RAIN_SOUND"                       ,  TRUE  );
	gGameSettings.fOptions[TOPTION_ALLOW_CROWS]                     = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_ALLOW_CROWS"                      ,  TRUE  );
	gGameSettings.fOptions[TOPTION_ALLOW_SOLDIER_TOOLTIPS]          = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_ALLOW_SOLDIER_TOOLTIPS"           ,  TRUE ); // Changed from random IMP personality - SANDRO
	gGameSettings.fOptions[TOPTION_USE_AUTO_SAVE]                   = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_USE_AUTO_SAVE"                    ,  FALSE );
	gGameSettings.fOptions[TOPTION_SILENT_SKYRIDER]                 = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SILENT_SKYRIDER"                  ,  FALSE );
	gGameSettings.fOptions[TOPTION_LOW_CPU_USAGE]                   = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_LOW_CPU_USAGE"                    ,  FALSE );
	gGameSettings.fOptions[TOPTION_ENHANCED_DESC_BOX]               = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_ENHANCED_DESC_BOX"                ,  FALSE );
	gGameSettings.fOptions[TOPTION_TOGGLE_TURN_MODE]                = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_TOGGLE_TURN_MODE"                 ,  FALSE );
	gGameSettings.fOptions[TOPTION_CHEAT_MODE_OPTIONS_HEADER]       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_CHEAT_MODE_OPTIONS_HEADER"        ,  FALSE );
	gGameSettings.fOptions[TOPTION_FORCE_BOBBY_RAY_SHIPMENTS]       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_FORCE_BOBBY_RAY_SHIPMENTS"        ,  FALSE );
	gGameSettings.fOptions[TOPTION_CHEAT_MODE_OPTIONS_END]          = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_CHEAT_MODE_OPTIONS_END"           ,  FALSE );
	gGameSettings.fOptions[TOPTION_DEBUG_MODE_OPTIONS_HEADER]       = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_DEBUG_MODE_OPTIONS_HEADER"        ,  FALSE );
	gGameSettings.fOptions[TOPTION_SHOW_RESET_ALL_OPTIONS]          = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_SHOW_RESET_ALL_OPTIONS"           ,  FALSE );
	gGameSettings.fOptions[TOPTION_RESET_ALL_OPTIONS]               = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_RESET_ALL_OPTIONS"                ,  FALSE );
	gGameSettings.fOptions[TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE] = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE"  ,  FALSE );
	gGameSettings.fOptions[TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP] = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP"  ,  FALSE );
	gGameSettings.fOptions[TOPTION_RENDER_MOUSE_REGIONS]            = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_RENDER_MOUSE_REGIONS"             ,  FALSE );
	gGameSettings.fOptions[TOPTION_DEBUG_MODE_OPTIONS_END]          = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_DEBUG_MODE_OPTIONS_END"           ,  FALSE );
	gGameSettings.fOptions[TOPTION_LAST_OPTION]                     = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_LAST_OPTION"                      ,  FALSE );
	gGameSettings.fOptions[NUM_GAME_OPTIONS]                        = iniReader.ReadBoolean("JA2 Game Settings","NUM_GAME_OPTIONS"                         ,  FALSE );
	gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT]                = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_MERC_CASTS_LIGHT"                 ,  TRUE  );
	gGameSettings.fOptions[TOPTION_HIDE_BULLETS]                    = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_HIDE_BULLETS"                     ,  FALSE );
	gGameSettings.fOptions[TOPTION_TRACKING_MODE]                   = iniReader.ReadBoolean("JA2 Game Settings","TOPTION_TRACKING_MODE"                    ,  TRUE  );
	gGameSettings.fOptions[NUM_ALL_GAME_OPTIONS]                    = iniReader.ReadBoolean("JA2 Game Settings","NUM_ALL_GAME_OPTIONS"                     ,  FALSE );


	DeleteShadeTableDir();  // ary-05/05/2009 : Might be bad idea for it to be here. But its gotta happen for some reason every now and then.
							//		 : The call to DeleteShadeTableDir() used to coincided with reseting an older settings file.
							//		 : Shade Table Dir is rebuilt when ever LoadShadeTable() cant find it. Its only around ~1.5 MB. 

	//
	//Do checking to make sure the settings are valid
	//
	if( gGameSettings.bLastSavedGameSlot < 0 || gGameSettings.bLastSavedGameSlot >= NUM_SAVE_GAMES )
		gGameSettings.bLastSavedGameSlot = -1;

	if( gGameSettings.ubMusicVolumeSetting > HIGHVOLUME )
		gGameSettings.ubMusicVolumeSetting = MIDVOLUME;

	if( gGameSettings.ubSoundEffectsVolume > HIGHVOLUME )
		gGameSettings.ubSoundEffectsVolume = MIDVOLUME;

	if( gGameSettings.ubSpeechVolume > HIGHVOLUME )
		gGameSettings.ubSpeechVolume = MIDVOLUME;


	//make sure that at least subtitles or speech is enabled
	if( !gGameSettings.fOptions[ TOPTION_SUBTITLES ] && !gGameSettings.fOptions[ TOPTION_SPEECH ] )
	{
		gGameSettings.fOptions[ TOPTION_SUBTITLES ]						= TRUE;
		gGameSettings.fOptions[ TOPTION_SPEECH ]						= TRUE;
	}


	//
	//	Set the settings
	//

	SetSoundEffectsVolume( gGameSettings.ubSoundEffectsVolume );
	SetSpeechVolume( gGameSettings.ubSpeechVolume );
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );

	//if the user doesnt want the help screens present
	if( gGameSettings.fHideHelpInAllScreens )
	{
		gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen = 0;
	}
	else
	{
		//Set it so that every screens help will come up the first time ( the 'x' will be set )
		gHelpScreen.usHasPlayerSeenHelpScreenInCurrentScreen = 0xffff;
	}

	return( TRUE );
}


BOOLEAN	SaveGameSettings()
{
	//Record the current settings into the game settins structure
	gGameSettings.ubSoundEffectsVolume = (UINT8)GetSoundEffectsVolume( );
	gGameSettings.ubSpeechVolume       = (UINT8)GetSpeechVolume( );
	gGameSettings.ubMusicVolumeSetting = (UINT8)MusicGetVolume( );

#ifndef USE_VFS
	char	gameSettingsFilePath[MAX_PATH];
	FILE	*file_pointer;

	sprintf(gameSettingsFilePath, "%s\\%s", gCustomDataCat.GetRootDir().c_str(), GAME_SETTINGS_FILE);
	fopen_s( &file_pointer, gameSettingsFilePath, "w" );
	if( !file_pointer )
	{
		//?fclose( file_pointer );
		return FALSE;
	}
#endif

	std::stringstream settings;
	const char endl[] = "\r\n";
	settings << ";******************************************************************************************************************************" << endl;
	settings << ";******************************************************************************************************************************" << endl;
	settings << ";    Jagged Alliance 2 --Settings File--                                                                                       " << endl;
	settings << ";                                                                                                                              " << endl;
	settings << ";    Please note that this file is automatically generated by the game.                                                        " << endl;
	settings << ";                                                                                                                              " << endl;
	settings << ";    While it is safe to change things from within this file, not all values are acceptable. Some may break the game,          " << endl;
	settings << ";      some may be ignored, but most likely they will be acceptable or reset to a default value.                               " << endl;
	settings << ";                                                                                                                              " << endl;
	settings << ";    Please note, This file and its contents are in a beta phase. Expect changes, however they should be minimal and automated." << endl;
	settings << ";                                                                                                                              " << endl;
	settings << ";******************************************************************************************************************************" << endl;
	settings << endl;
	settings << ";******************************************************************************************************************************" << endl;
	settings << endl;
	settings << ";    The Current Game Setting Struct is defined as : " << endl;
	settings << ";" << endl;
	settings << ";    typedef struct" << endl;
	settings << ";    {" << endl;
	settings << ";        INT8     bLastSavedGameSlot;                 // The last saved game number goes in here" << endl;
	settings << ";        UINT8    ubMusicVolumeSetting;               // Volume Setting" << endl;
	settings << ";        UINT8    ubSoundEffectsVolume;               // Volume Setting" << endl;
	settings << ";        UINT8    ubSpeechVolume;                     // Volume Setting" << endl;
	settings << ";        UINT8    fOptions[ NUM_ALL_GAME_OPTIONS ];   // Toggle Options (Speech, Subtitles, Show Tree Tops, etc.. )" << endl;
	settings << ";        UINT32   uiMeanwhileScenesSeenFlags;         // Bit Vector describing seen 'mean whiles..'" << endl;
	settings << ";        BOOLEAN  fHideHelpInAllScreens;              // Controls Help \"do not show help again\" checkbox" << endl;
	settings << ";        UINT8    ubSizeOfDisplayCover;               // The number of grids the player designates thru [Delete + ( = or - )]" << endl;
	settings << ";        UINT8    ubSizeOfLOS;                        // The number of grids the player designates thru [End    + ( = or - )]" << endl;
	settings << ";    } GAME_SETTINGS" << endl;
	settings << ";" << endl;
	settings << ";******************************************************************************************************************************" << endl;
	settings << endl << endl;
	settings << "[JA2 Game Settings]" << endl;

	settings << "bLastSavedGameSlot                       = " << (int)gGameSettings.bLastSavedGameSlot << endl;
	settings << "ubMusicVolumeSetting                     = " << (int)gGameSettings.ubMusicVolumeSetting << endl;
	settings << "ubSoundEffectsVolume                     = " << (int)gGameSettings.ubSoundEffectsVolume << endl;
	settings << "ubSpeechVolume                           = " << (int)gGameSettings.ubSpeechVolume << endl;
	settings << "uiMeanwhileScenesSeenFlags               = " << gGameSettings.uiMeanwhileScenesSeenFlags << endl;
	settings << "fHideHelpInAllScreens                    = " << (gGameSettings.fHideHelpInAllScreens								?    "TRUE" : "FALSE" ) << endl;
	settings << "ubSizeOfDisplayCover                     = " << (int)gGameSettings.ubSizeOfDisplayCover << endl;
	settings << "ubSizeOfLOS                              = " << (int)gGameSettings.ubSizeOfLOS << endl;
	settings << "TOPTION_SPEECH                           = " << (gGameSettings.fOptions[TOPTION_SPEECH]							?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_MUTE_CONFIRMATIONS               = " << (gGameSettings.fOptions[TOPTION_MUTE_CONFIRMATIONS]				?	 "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SUBTITLES                        = " << (gGameSettings.fOptions[TOPTION_SUBTITLES]							?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_KEY_ADVANCE_SPEECH               = " << (gGameSettings.fOptions[TOPTION_KEY_ADVANCE_SPEECH]				?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_ANIMATE_SMOKE                    = " << (gGameSettings.fOptions[TOPTION_ANIMATE_SMOKE]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_BLOOD_N_GORE                     = " << (gGameSettings.fOptions[TOPTION_BLOOD_N_GORE]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_DONT_MOVE_MOUSE                  = " << (gGameSettings.fOptions[TOPTION_DONT_MOVE_MOUSE]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_OLD_SELECTION_METHOD             = " << (gGameSettings.fOptions[TOPTION_OLD_SELECTION_METHOD]				?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_ALWAYS_SHOW_MOVEMENT_PATH        = " << (gGameSettings.fOptions[TOPTION_ALWAYS_SHOW_MOVEMENT_PATH]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SHOW_MISSES                      = " << (gGameSettings.fOptions[TOPTION_SHOW_MISSES]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_RTCONFIRM                        = " << (gGameSettings.fOptions[TOPTION_RTCONFIRM]							?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SLEEPWAKE_NOTIFICATION           = " << (gGameSettings.fOptions[TOPTION_SLEEPWAKE_NOTIFICATION]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_USE_METRIC_SYSTEM                = " << (gGameSettings.fOptions[TOPTION_USE_METRIC_SYSTEM]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_MERC_ALWAYS_LIGHT_UP             = " << (gGameSettings.fOptions[TOPTION_MERC_ALWAYS_LIGHT_UP]				?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SMART_CURSOR                     = " << (gGameSettings.fOptions[TOPTION_SMART_CURSOR]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SNAP_CURSOR_TO_DOOR              = " << (gGameSettings.fOptions[TOPTION_SNAP_CURSOR_TO_DOOR]				?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_GLOW_ITEMS                       = " << (gGameSettings.fOptions[TOPTION_GLOW_ITEMS]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_TOGGLE_TREE_TOPS                 = " << (gGameSettings.fOptions[TOPTION_TOGGLE_TREE_TOPS]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_TOGGLE_WIREFRAME                 = " << (gGameSettings.fOptions[TOPTION_TOGGLE_WIREFRAME]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_3D_CURSOR                        = " << (gGameSettings.fOptions[TOPTION_3D_CURSOR]							?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_CTH_CURSOR                       = " << (gGameSettings.fOptions[TOPTION_CTH_CURSOR]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_GL_BURST_CURSOR                  = " << (gGameSettings.fOptions[TOPTION_GL_BURST_CURSOR]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_DROP_ALL                         = " << (gGameSettings.fOptions[TOPTION_DROP_ALL]							?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_GL_HIGH_ANGLE                    = " << (gGameSettings.fOptions[TOPTION_GL_HIGH_ANGLE]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_AIM_LEVEL_RESTRICTION            = " << (gGameSettings.fOptions[TOPTION_AIM_LEVEL_RESTRICTION]				?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SPACE_SELECTS_NEXT_SQUAD         = " << (gGameSettings.fOptions[TOPTION_SPACE_SELECTS_NEXT_SQUAD]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SHOW_ITEM_SHADOW                 = " << (gGameSettings.fOptions[TOPTION_SHOW_ITEM_SHADOW]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SHOW_WEAPON_RANGE_IN_TILES       = " << (gGameSettings.fOptions[TOPTION_SHOW_WEAPON_RANGE_IN_TILES]		?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_TRACERS_FOR_SINGLE_FIRE          = " << (gGameSettings.fOptions[TOPTION_TRACERS_FOR_SINGLE_FIRE]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_RAIN_SOUND                       = " << (gGameSettings.fOptions[TOPTION_RAIN_SOUND]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_ALLOW_CROWS                      = " << (gGameSettings.fOptions[TOPTION_ALLOW_CROWS]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_ALLOW_SOLDIER_TOOLTIPS           = " << (gGameSettings.fOptions[TOPTION_ALLOW_SOLDIER_TOOLTIPS]			?    "TRUE" : "FALSE" ) << endl; // changed from random IMP personality - SANDRO
	settings << "TOPTION_USE_AUTO_SAVE                    = " << (gGameSettings.fOptions[TOPTION_USE_AUTO_SAVE]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SILENT_SKYRIDER                  = " << (gGameSettings.fOptions[TOPTION_SILENT_SKYRIDER]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_LOW_CPU_USAGE                    = " << (gGameSettings.fOptions[TOPTION_LOW_CPU_USAGE]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_ENHANCED_DESC_BOX                = " << (gGameSettings.fOptions[TOPTION_ENHANCED_DESC_BOX]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_TOGGLE_TURN_MODE                 = " << (gGameSettings.fOptions[TOPTION_TOGGLE_TURN_MODE]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_CHEAT_MODE_OPTIONS_HEADER        = " << (gGameSettings.fOptions[TOPTION_CHEAT_MODE_OPTIONS_HEADER]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_FORCE_BOBBY_RAY_SHIPMENTS        = " << (gGameSettings.fOptions[TOPTION_FORCE_BOBBY_RAY_SHIPMENTS]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_CHEAT_MODE_OPTIONS_END           = " << (gGameSettings.fOptions[TOPTION_CHEAT_MODE_OPTIONS_END]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_DEBUG_MODE_OPTIONS_HEADER        = " << (gGameSettings.fOptions[TOPTION_DEBUG_MODE_OPTIONS_HEADER]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_SHOW_RESET_ALL_OPTIONS           = " << (gGameSettings.fOptions[TOPTION_SHOW_RESET_ALL_OPTIONS]			?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_RESET_ALL_OPTIONS                = " << (gGameSettings.fOptions[TOPTION_RESET_ALL_OPTIONS]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE  = " << (gGameSettings.fOptions[TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE]	?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP  = " << (gGameSettings.fOptions[TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP]	?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_RENDER_MOUSE_REGIONS             = " << (gGameSettings.fOptions[TOPTION_RENDER_MOUSE_REGIONS]				?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_DEBUG_MODE_OPTIONS_END           = " << (gGameSettings.fOptions[TOPTION_DEBUG_MODE_OPTIONS_END]			?    "TRUE" : "FALSE" ) << endl;
	settings << ";******************************************************************************************************************************" << endl;
	settings << ";  Options beyond this point are Code derived options, DO NOT CHANGE THESE UNLESS YOU KNOW WHAT YOUR ARE DOING." << endl;
	settings << ";  They are only included here for complete transparency for all GameSettings content.							" << endl;
	settings << ";******************************************************************************************************************************" << endl;
	settings << "TOPTION_LAST_OPTION                      = " << (gGameSettings.fOptions[TOPTION_LAST_OPTION]						?    "TRUE" : "FALSE" ) << endl;
	settings << "NUM_GAME_OPTIONS                         = " << (gGameSettings.fOptions[NUM_GAME_OPTIONS]							?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_MERC_CASTS_LIGHT                 = " << (gGameSettings.fOptions[TOPTION_MERC_CASTS_LIGHT]					?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_HIDE_BULLETS                     = " << (gGameSettings.fOptions[TOPTION_HIDE_BULLETS]						?    "TRUE" : "FALSE" ) << endl;
	settings << "TOPTION_TRACKING_MODE                    = " << (gGameSettings.fOptions[TOPTION_TRACKING_MODE]						?    "TRUE" : "FALSE" ) << endl;
	settings << "NUM_ALL_GAME_OPTIONS                     = " << (gGameSettings.fOptions[NUM_ALL_GAME_OPTIONS]						?    "TRUE" : "FALSE" ) << endl;

#ifndef USE_VFS
	fprintf_s (file_pointer , settings.str().c_str());
	fclose( file_pointer );
#else
	vfs::UInt32 written;
	try
	{
		vfs::COpenWriteFile wfile(GAME_SETTINGS_FILE,true,true);
		wfile.file().Write(settings.str().c_str(), settings.str().length(),written);
	}
	catch(CBasicException& ex)
	{
		vfs::CFile wfile(GAME_SETTINGS_FILE);
		if(wfile.OpenWrite(true,true))
		{
			wfile.Write(settings.str().c_str(), settings.str().length(),written);
			wfile.Close();
		}
	}
#endif

	return( TRUE );

}


void InitGameSettings()
{
	// completely blank out gGameSettings
	memset( &gGameSettings, 0, sizeof( GAME_SETTINGS ) );

	//Init the Game Settings
	gGameSettings.bLastSavedGameSlot	= -1;
	gGameSettings.ubMusicVolumeSetting	= 63;
	gGameSettings.ubSoundEffectsVolume	= 63;
	gGameSettings.ubSpeechVolume		= 63;

	//Set the settings
	SetSoundEffectsVolume( gGameSettings.ubSoundEffectsVolume );
	SetSpeechVolume( gGameSettings.ubSpeechVolume );
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );

	gGameSettings.fOptions[ TOPTION_SPEECH ]							= TRUE;
	gGameSettings.fOptions[ TOPTION_MUTE_CONFIRMATIONS ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_SUBTITLES ]							= TRUE;
	gGameSettings.fOptions[ TOPTION_KEY_ADVANCE_SPEECH ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ]					= FALSE;
	gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_ALWAYS_SHOW_MOVEMENT_PATH ]			= FALSE;
	gGameSettings.fOptions[	TOPTION_SHOW_MISSES ]						= FALSE;
	gGameSettings.fOptions[ TOPTION_RTCONFIRM ]							= FALSE;
	gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ]			= TRUE;
	gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ]					= FALSE;
	gGameSettings.fOptions[ TOPTION_MERC_ALWAYS_LIGHT_UP ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_SMART_CURSOR ]						= FALSE;
	gGameSettings.fOptions[ TOPTION_SNAP_CURSOR_TO_DOOR ]				= TRUE;
	gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_TOGGLE_TREE_TOPS ]					= TRUE;
	gGameSettings.fOptions[ TOPTION_TOGGLE_WIREFRAME ]					= TRUE;
	gGameSettings.fOptions[ TOPTION_3D_CURSOR ]							= FALSE;
	gGameSettings.fOptions[ TOPTION_CTH_CURSOR ]						= TRUE;

	//Madd:
	gGameSettings.fOptions[ TOPTION_GL_BURST_CURSOR ]					= TRUE;
	gGameSettings.fOptions[ TOPTION_DROP_ALL ]							= FALSE;
	gGameSettings.fOptions[ TOPTION_GL_HIGH_ANGLE ]						= FALSE;
	gGameSettings.fOptions[	TOPTION_AIM_LEVEL_RESTRICTION ]				= TRUE;

	//lalien
	gGameSettings.fOptions[ TOPTION_SPACE_SELECTS_NEXT_SQUAD ]			= TRUE;
	gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ]					= TRUE;
	gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ]		= TRUE;
	gGameSettings.fOptions[ TOPTION_TRACERS_FOR_SINGLE_FIRE ]			= FALSE;
	gGameSettings.fOptions[ TOPTION_RAIN_SOUND ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_ALLOW_CROWS ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_ALLOW_SOLDIER_TOOLTIPS ]			= TRUE;	// changed - SANDRO
	gGameSettings.fOptions[ TOPTION_USE_AUTO_SAVE ]						= FALSE;
	gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ]					= FALSE;
	gGameSettings.fOptions[ TOPTION_LOW_CPU_USAGE ]						= FALSE;
	gGameSettings.fOptions[ TOPTION_ENHANCED_DESC_BOX ]					= FALSE;

	// arynn 
	gGameSettings.fOptions[ TOPTION_TOGGLE_TURN_MODE ]					= FALSE;
	gGameSettings.fOptions[ TOPTION_CHEAT_MODE_OPTIONS_HEADER ]			= FALSE;	
	gGameSettings.fOptions[ TOPTION_FORCE_BOBBY_RAY_SHIPMENTS ]			= FALSE;	// force all pending Bobby Ray shipments
	gGameSettings.fOptions[ TOPTION_CHEAT_MODE_OPTIONS_END ]			= FALSE;	
	gGameSettings.fOptions[ TOPTION_DEBUG_MODE_OPTIONS_HEADER ]			= FALSE;	// an example options screen options header (pure text)
	gGameSettings.fOptions[ TOPTION_SHOW_RESET_ALL_OPTIONS ]			= FALSE;	// failsafe show/hide option to reset all options
	gGameSettings.fOptions[ TOPTION_RESET_ALL_OPTIONS ]					= FALSE;	// a do once and reset self option (button like effect)
	gGameSettings.fOptions[ TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE ]	= FALSE;	// allow debug options that were set in debug.exe to continue in a rel.exe (debugging release can be beneficial)
	gGameSettings.fOptions[	TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP ]	= FALSE;	// an example option that will show/hide other options
	gGameSettings.fOptions[	TOPTION_RENDER_MOUSE_REGIONS ]				= FALSE;	// an example of a DEBUG build option
	gGameSettings.fOptions[	TOPTION_DEBUG_MODE_OPTIONS_END ]			= FALSE;	// an example options screen options divider (pure text)

	// enum control options (not real options but included here for the sake of complete control of values)

	// ary-05/05/2009 : TOPTION_LAST_OPTION is THE LAST options screen toggle option that exists. (its still an option, and its < NUM_GAME_OPTIONS)
	//		 : intended for debugging options screen final page. test to avoid last page over or under extension. 
	//		 : might be useful in future of toggle option content developement.
	gGameSettings.fOptions[	TOPTION_LAST_OPTION ]						= FALSE; // doesnt do anything except exist
	gGameSettings.fOptions[	NUM_GAME_OPTIONS ]						    = FALSE; // Toggles prior to this will be able to be toggled by the player

	// JA2Gold
	gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ]					= TRUE;

	gGameSettings.fOptions[ TOPTION_HIDE_BULLETS ]						= FALSE;
	gGameSettings.fOptions[ TOPTION_TRACKING_MODE ]						= TRUE;

	gGameSettings.fOptions[	NUM_ALL_GAME_OPTIONS ]						= FALSE; // Absolute final end of enum

	gGameSettings.ubSizeOfDisplayCover = 4;
	gGameSettings.ubSizeOfLOS = 4;

	//Since we just set the settings, save them
	SaveGameSettings();

}

void InitGameOptions()
{
	memset( &gGameOptions, 0, sizeof( GAME_OPTIONS ) );

	//Init the game options
	
	if (is_networked)
		gGameOptions.ubBobbyRay			= BR_AWESOME;// hayden, was BR_GOOD;
	else
		gGameOptions.ubBobbyRay			= BR_GOOD;

	gGameOptions.fGunNut			= TRUE;
	gGameOptions.fAirStrikes		= FALSE;

	if (is_networked)
		gGameOptions.ubGameStyle		= STYLE_REALISTIC;//hayden, was STYLE_SCIFI;
	else
		gGameOptions.ubGameStyle		= STYLE_SCIFI;

	gGameOptions.ubDifficultyLevel	= DIF_LEVEL_MEDIUM;
	//CHRISL: override default inventory mode when in low res
	if(IsNIVModeValid() == FALSE)
		gGameOptions.ubInventorySystem	= INVENTORY_OLD;
	else
		gGameOptions.ubInventorySystem	= INVENTORY_NEW;
	
	if (is_networked)
		gGameOptions.fTurnTimeLimit	= TRUE;//hayden
	else
		gGameOptions.fTurnTimeLimit	= FALSE;
	
	gGameOptions.fIronManMode		= FALSE;
}


// WDS: These HAVE to total 100% at all times!!!
//#define PROGRESS_PORTION_KILLS		25
//#define PROGRESS_PORTION_CONTROL	25
//#define PROGRESS_PORTION_INCOME		50


// The following are for fixing problems when reading the .ini file
#define COUNT_STANDARD_MALE_SLOTS 3
#define FIRST_STANDARD_MALE_SLOT 51
#define COUNT_STANDARD_FEMALE_SLOTS 3
#define FIRST_STANDARD_FEMALE_SLOT 54


extern INT32 CountFilledIMPSlots( INT8 iSex );
extern INT32 CountEmptyIMPSlots( INT8 iSex );

// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions()
{

	//Kaiden: Setting Ja2_Options.ini file to be read
	CIniReader iniReader(GAME_EXTERNAL_OPTIONS_FILE);
	

	// WANNE: FILE TRANSFER: We disable reading from ja2_options.ini until we get file transfer working
	if(is_networked)
	{
#ifndef USE_VFS
		//memset(&iniReader, 0, sizeof (CIniReader) );//disable ini in mp (taking default values)
		iniReader.Clear();
#endif
	}

	//################# Laptop Settings #################

	// WDS: Allow flexible numbers of IMPs of each sex
	gGameExternalOptions.iMaxIMPCharacters		= iniReader.ReadInteger("JA2 Laptop Settings","MAX_IMP_CHARACTERS",1, 1, NUM_PROFILES);

	gGameExternalOptions.iIMPMaleCharacterCount	= iniReader.ReadInteger("JA2 Laptop Settings","IMP_MALE_CHARACTER_COUNT", COUNT_STANDARD_MALE_SLOTS, 1, NUM_PROFILES);
	gGameExternalOptions.iIMPFemaleCharacterCount = iniReader.ReadInteger("JA2 Laptop Settings","IMP_FEMALE_CHARACTER_COUNT", COUNT_STANDARD_FEMALE_SLOTS, 1, NUM_PROFILES);
	if (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount > NUM_PROFILES)
	{
		gGameExternalOptions.iIMPMaleCharacterCount	= COUNT_STANDARD_MALE_SLOTS;
		gGameExternalOptions.iIMPFemaleCharacterCount = COUNT_STANDARD_FEMALE_SLOTS;
	}

	//
	// Note: put -1 between male/female slots and -1 at end.	This allows everything to be
	// counted dynamically quite easily.	Note that all the code assumes there is AT
	// LEAST ONE slot for each sex.	If that changes the code will have to be updated.
	//
	// Because errors in these values can really goof things up we will try to fix up bad
	// values and use the defaults instead.
	//
	int idx;
	char caMaleCountStr [] = "IMP_MALE_%d";
	char caFemaleCountStr [] = "IMP_FEMALE_%d";
	char caCountStr[20];

	gGameExternalOptions.iaIMPSlots = (INT32*)MemAlloc( (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount + 2) * sizeof( UINT32 ) );
	for (idx = 0; idx < gGameExternalOptions.iIMPMaleCharacterCount; ++idx)
	{
		sprintf( caCountStr, caMaleCountStr, idx+1);
		gGameExternalOptions.iaIMPSlots[idx] = iniReader.ReadInteger("JA2 Laptop Settings",caCountStr, -1, -1, NUM_PROFILES-1);
		if (gGameExternalOptions.iaIMPSlots[idx] < 0)
		{
			if (idx < COUNT_STANDARD_MALE_SLOTS)
			{
				gGameExternalOptions.iaIMPSlots[idx] = FIRST_STANDARD_MALE_SLOT+idx;
			}
			else
			{
				// This is bad so just use the last standard slot #
				gGameExternalOptions.iaIMPSlots[idx] = FIRST_STANDARD_MALE_SLOT+COUNT_STANDARD_MALE_SLOTS-1;
			}
		}
	}
	gGameExternalOptions.iaIMPSlots[gGameExternalOptions.iIMPMaleCharacterCount] = -1;
	for (idx = 0; idx < gGameExternalOptions.iIMPFemaleCharacterCount; ++idx)
	{
		sprintf( caCountStr, caFemaleCountStr, idx+1);
		gGameExternalOptions.iaIMPSlots[idx+gGameExternalOptions.iIMPMaleCharacterCount+1] = iniReader.ReadInteger("JA2 Laptop Settings",caCountStr, -1, -1, NUM_PROFILES-1);
		if (gGameExternalOptions.iaIMPSlots[idx+gGameExternalOptions.iIMPMaleCharacterCount+1] < 0)
		{
			if (idx < COUNT_STANDARD_FEMALE_SLOTS)
			{
				gGameExternalOptions.iaIMPSlots[idx+gGameExternalOptions.iIMPMaleCharacterCount+1] = FIRST_STANDARD_FEMALE_SLOT+idx;
			}
			else
			{
				// This is bad so just use the last standard slot #
				gGameExternalOptions.iaIMPSlots[idx+gGameExternalOptions.iIMPMaleCharacterCount+1] = FIRST_STANDARD_FEMALE_SLOT+COUNT_STANDARD_FEMALE_SLOTS-1;
			}
		}
	}
	gGameExternalOptions.iaIMPSlots[gGameExternalOptions.iIMPFemaleCharacterCount+gGameExternalOptions.iIMPMaleCharacterCount+1] = -1;

	//Character generation
	gGameExternalOptions.iMinAttribute			= iniReader.ReadInteger("JA2 Laptop Settings","MIN_ATTRIBUTE_POINT",35, 1, 99);
	gGameExternalOptions.iMaxAttribute			= iniReader.ReadInteger("JA2 Laptop Settings","MAX_ATTRIBUTE_POINT",90, gGameExternalOptions.iMinAttribute+1, 100);
	gGameExternalOptions.iImpAttributePoints	= iniReader.ReadInteger("JA2 Laptop Settings","IMPATTRIBUTEPOINTS",40, 1, 100);
	gGameExternalOptions.iMaxZeroBonus			= iniReader.ReadInteger("JA2 Laptop Settings","MAX_ZERO_BONUS",15, 0, 100);
	gGameExternalOptions.iStartAttribute		= iniReader.ReadInteger("JA2 Laptop Settings","START_ATTRIBUTE",55, 1, 100);

	//gGameExternalOptions.fPers_att				= iniReader.ReadBoolean("JA2 Laptop Settings","USE_RANDOM_PERSONALITY",FALSE);
	// These two removed - SANDRO
	//gGameExternalOptions.iCustomPersonality		= (INT8) iniReader.ReadInteger("JA2 Laptop Settings","CUSTOM_PERSONALITY",0, 0, NUM_PERSONALITYTRAIT-1);
	//gGameExternalOptions.iCustomAttitude		= (INT8) iniReader.ReadInteger("JA2 Laptop Settings","CUSTOM_ATTITUDE",0, 0, NUM_ATTITUDES-1);
	// These three added - SANDRO
	gGameExternalOptions.iIMPStartingLevelCostMultiplier = iniReader.ReadInteger("JA2 Laptop Settings","IMP_STARTING_LEVEL_COST_MULTIPLIER", 5, 0, 100);
	gGameExternalOptions.iBonusPointsForDisability		 = iniReader.ReadInteger("JA2 Laptop Settings","IMP_BONUS_POINTS_FOR_DISABILITY",20, 0, 500);
	gGameExternalOptions.iBonusPointsPerSkillNotTaken	 = iniReader.ReadInteger("JA2 Laptop Settings","IMP_BONUS_POINTS_PER_SKILL_NOT_TAKEN",25, 0, 500);

	//Merc settings
	gGameExternalOptions.fMercDayOne			= iniReader.ReadBoolean("JA2 Laptop Settings","MERC_AVAILABLE_DAY_ONE",FALSE);
	gGameExternalOptions.fAllMercsAvailable		= iniReader.ReadBoolean("JA2 Laptop Settings","ALL_MERCS_AT_MERC",FALSE);

	//Merc Death Settings:
	gGameExternalOptions.gfMercsDieOnAssignment			= iniReader.ReadBoolean("JA2 Laptop Settings","MERCS_DIE_ON_ASSIGNMENT",TRUE);
	gGameExternalOptions.giEasyMercDeaths				= iniReader.ReadInteger("JA2 Laptop Settings","EASY_MERC_DEATHS",1, 0, 10);
	gGameExternalOptions.giExperiencedMercDeaths		= iniReader.ReadInteger("JA2 Laptop Settings","EXPERIENCED_MERC_DEATHS",2, 0, 10);
	gGameExternalOptions.giExpertMercDeaths				= iniReader.ReadInteger("JA2 Laptop Settings","EXPERT_MERC_DEATHS",3, 0, 10);
	gGameExternalOptions.giInsaneMercDeaths				= iniReader.ReadInteger("JA2 Laptop Settings","INSANE_MERC_DEATHS",4, 0, 10);

	//################# System Settings #################
	gGameExternalOptions.gubDeadLockDelay = (UINT8) iniReader.ReadInteger("JA2 System Settings","DEAD_LOCK_DELAY",15, 5, 50);
	gGameExternalOptions.gfEnableEmergencyButton_SkipStrategicEvents = iniReader.ReadBoolean("JA2 System Settings","ENABLE_EMERGENCY_BUTTON_NUMLOCK_TO_SKIP_STRATEGIC_EVENTS",0);

	// WDS - Automatically try to save when an assertion failure occurs
	gGameExternalOptions.autoSaveOnAssertionFailure		= iniReader.ReadBoolean("JA2 System Settings","AUTO_SAVE_ON_ASSERTION_FAILURE", FALSE);
	gGameExternalOptions.autoSaveTime 					= iniReader.ReadInteger("JA2 System Settings","AUTO_SAVE_TIME", 6, 0, 24);

	//################# Video Settings #################
	gGameExternalOptions.gfVSync = iniReader.ReadBoolean("JA2 Video Settings","VERTICAL_SYNC",0);


	//################# Animation Settings #################
	gGameExternalOptions.gubPlayerTurnSpeedUpFactor		= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","PLAYER_TURN_SPEED_UP_FACTOR",1, 0, 100);
	gGameExternalOptions.gubEnemyTurnSpeedUpFactor		= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","ENEMY_TURN_SPEED_UP_FACTOR",1, 0, 100);
	gGameExternalOptions.gubCreatureTurnSpeedUpFactor	= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","CREATURE_TURN_SPEED_UP_FACTOR",1, 0, 100);
	gGameExternalOptions.gubMilitiaTurnSpeedUpFactor	= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","MILITIA_TURN_SPEED_UP_FACTOR",1, 0, 100);
	gGameExternalOptions.gubCivilianTurnSpeedUpFactor	= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","CIVILIAN_TURN_SPEED_UP_FACTOR",1, 0, 100);


	//################# Sound Settings #################
	gGameExternalOptions.guiWeaponSoundEffectsVolume	= iniReader.ReadInteger("JA2 Sound Settings","WEAPON_SOUND_EFFECTS_VOLUME",0, 0, 1000 /*1000 = 10x?*/);



	//################# Tactical Settings #################

	gGameExternalOptions.gfRevealItems	= iniReader.ReadBoolean("JA2 Tactical Settings","REVEAL_ITEMS_AFTER_COMBAT",TRUE);

	// Militia Settings	
	gGameExternalOptions.fAllowTacticalMilitiaCommand	= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_TACTICAL_MILITIA_COMMAND",0);

	// Enemy AP settings
	gGameExternalOptions.iEasyAPBonus					= (INT8) iniReader.ReadInteger("JA2 Tactical Settings","NOVICE_AP_BONUS",0,__max(-APBPConstants[AP_MINIMUM],-128),127);
	gGameExternalOptions.iExperiencedAPBonus			= (INT8) iniReader.ReadInteger("JA2 Tactical Settings","EXPERIENCED_AP_BONUS",0,__max(-APBPConstants[AP_MINIMUM],-128),127);
	gGameExternalOptions.iExpertAPBonus					= (INT8) iniReader.ReadInteger("JA2 Tactical Settings","EXPERT_AP_BONUS",0,__max(-APBPConstants[AP_MINIMUM],-128),127);
	gGameExternalOptions.iInsaneAPBonus					= (INT8) iniReader.ReadInteger("JA2 Tactical Settings","INSANE_AP_BONUS",0,__max(-APBPConstants[AP_MINIMUM],-128),127);
	gGameExternalOptions.iPlayerAPBonus					= (INT8) iniReader.ReadInteger("JA2 Tactical Settings","PLAYER_AP_BONUS",0,__max(-APBPConstants[AP_MINIMUM],-128),127);

	// Sight range
	gGameExternalOptions.ubStraightSightRange				= iniReader.ReadInteger("JA2 Tactical Settings","BASE_SIGHT_RANGE",13, 5, 100);
	gGameExternalOptions.ubVisDistDecreasePerRainIntensity	= iniReader.ReadInteger("JA2 Tactical Settings","VISUAL_DISTANCE_DECREASE_PER_RAIN_INTENSITY",30, 0, 100);
	gGameExternalOptions.gfAllowLimitedVision				= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_LIMITED_VISION",0);

	// Maximal search distance for grenades
	//gGameExternalOptions.guiMaxTossSearchDist				= iniReader.ReadInteger("JA2 Tactical Settings","MaxTossSearchDist",3);

	// Soldier tool tips - removed from here, placed into preferences - SANDRO
	//gGameExternalOptions.gfAllowSoldierToolTips		= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_SOLDIER_TOOL_TIPS",0);
	//SCORE: Settings for UDT
	gGameExternalOptions.gfAllowUDTRange					= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_UDT_RANGE",0);
	gGameExternalOptions.gfAllowUDTDetail					= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_UDT_DETAIL",0);
	gGameExternalOptions.ubUDTModifier						= (UINT8) iniReader.ReadInteger("JA2 Tactical Settings", "UDT_MODIFIER", 50);

	// ShadoWarrior: Tooltip changes (start)
	gGameExternalOptions.ubSoldierTooltipDetailLevel		= (UINT8) iniReader.ReadInteger("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DETAIL_LEVEL", 1, 0, 4);

	gGameExternalOptions.fEnableDynamicSoldierTooltips		= iniReader.ReadBoolean("JA2 Tactical Settings", "DYNAMIC_SOLDIER_TOOLTIPS", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipLocation		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_LOCATION", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBrightness	= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BRIGHTNESS", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipRangeToTarget	= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_RANGE_TO_TARGET", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipID			= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_ID", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipOrders		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_ORDERS", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipAttitude		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_ATTITUDE", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipActionPoints	= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_ACTIONPOINTS", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipHealth		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_HEALTH", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipHelmet		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_HELMET", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipVest			= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_VEST", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipLeggings		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_LEGGINGS", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipHeadItem1		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_HEAD_SLOT_1", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipHeadItem2		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_HEAD_SLOT_2", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipWeapon		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_WEAPON", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipSecondHand	= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_OFF_HAND", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot1		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_1", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot2		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_2", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot3		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_3", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot4		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_4", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot5		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_5", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot6		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_6", TRUE);
	gGameExternalOptions.fEnableSoldierTooltipBigSlot7		= iniReader.ReadBoolean("JA2 Tactical Settings", "SOLDIER_TOOLTIP_DISPLAY_BIG_SLOT_7", TRUE);
	// ShadoWarrior: Tooltip changes (end)

	// Unload weapons & remove attachments
	gGameExternalOptions.gfShiftFUnloadWeapons				= iniReader.ReadBoolean("JA2 Tactical Settings","SHIFT_F_UNLOAD_WEAPONS",1);
	gGameExternalOptions.gfShiftFRemoveAttachments			= iniReader.ReadBoolean("JA2 Tactical Settings","SHIFT_F_REMOVE_ATTACHMENTS",1);

	gGameExternalOptions.fEnableArmorCoverage				= iniReader.ReadBoolean("JA2 Tactical Settings", "ENABLE_ARMOR_COVERAGE", FALSE); // ShadoWarrior for Captain J's armor coverage
	// The_Bob - real time sneaking code 01/06/09
	// Suport disabling real time sneaking via external .ini file
	gGameExternalOptions.fAllowRealTimeSneak		= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_REAL_TIME_SNEAK", FALSE);
	// Silence the RT sneaking messages
	gGameExternalOptions.fQuietRealTimeSneak		= iniReader.ReadBoolean("JA2 Tactical Settings","QUIET_REAL_TIME_SNEAK", FALSE);

	//################# Rain Settings ##################

	// Rain settings
	gGameExternalOptions.gfAllowRain									= iniReader.ReadBoolean("JA2 Rain Settings","ALLOW_RAIN",0);
	gGameExternalOptions.gusRainChancePerDay							= iniReader.ReadInteger("JA2 Rain Settings","RAIN_CHANCE_PER_DAY",100, 0, 100);
	gGameExternalOptions.gusRainMinLength								= iniReader.ReadInteger("JA2 Rain Settings","RAIN_MIN_LENGTH_IN_MINUTES",60, 1, 1438 /* 24 hrs - 2 minutes */);
	gGameExternalOptions.gusRainMaxLength								= iniReader.ReadInteger("JA2 Rain Settings","RAIN_MAX_LENGTH_IN_MINUTES",300, gGameExternalOptions.gusRainMinLength+1, 1439);
	gGameExternalOptions.guiMaxRainDrops								= iniReader.ReadInteger("JA2 Rain Settings","MAX_RAIN_DROPS",80, 1, 800);
	gGameExternalOptions.ubWeaponReliabilityReductionPerRainIntensity	= iniReader.ReadInteger("JA2 Rain Settings","WEAPON_RELIABILITY_REDUCTION_PER_RAIN_INTENSITY",0, 0, 10);
	gGameExternalOptions.ubBreathGainReductionPerRainIntensity			= iniReader.ReadDouble("JA2 Rain Settings","BREATH_GAIN_REDUCTION_PER_RAIN_INTENSITY",25, 1, 100);

	// Thunder settings
	gGameExternalOptions.gfAllowLightning					= iniReader.ReadBoolean("JA2 Thunder Settings","ALLOW_LIGHTNING",TRUE);
	gGameExternalOptions.guiMinLightningInterval			= iniReader.ReadInteger("JA2 Thunder Settings","MIN_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS",5, 1, 100);
	gGameExternalOptions.guiMaxLightningInterval			= iniReader.ReadInteger("JA2 Thunder Settings","MAX_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS",15, 2, 600);
	gGameExternalOptions.guiMinDLInterval					= iniReader.ReadInteger("JA2 Thunder Settings","MIN_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS",1, 1, 100);
	gGameExternalOptions.guiMaxDLInterval					= iniReader.ReadInteger("JA2 Thunder Settings","MAX_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS",5, 2, 600);
	gGameExternalOptions.guiProlongLightningIfSeenSomeone	= iniReader.ReadInteger("JA2 Thunder Settings","PROLOGNE_DELAY_IF_SEEN_SOMEONE_DURING_LIGHTNING_IN_TURNBASED_IN_SECONDS",5, 1, 60);
	gGameExternalOptions.guiChanceToDoLightningBetweenTurns = iniReader.ReadInteger("JA2 Thunder Settings","CHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS",35, 0, 100);



	//################# Gameplay Settings ##################

	//################# Settings valid on game start only ##################
	//Money settings
	gGameExternalOptions.iStartingCashNovice		= iniReader.ReadInteger("JA2 Gameplay Settings", "NOVICE_CASH",45000, 2000, 0x0FFFFFFF);
	gGameExternalOptions.iStartingCashExperienced	= iniReader.ReadInteger("JA2 Gameplay Settings", "EXPERIENCED_CASH",35000, 2000, 0x0FFFFFFF);
	gGameExternalOptions.iStartingCashExpert		= iniReader.ReadInteger("JA2 Gameplay Settings", "EXPERT_CASH",30000, 2000, 0x0FFFFFFF);
	gGameExternalOptions.iStartingCashInsane		= iniReader.ReadInteger("JA2 Gameplay Settings", "INSANE_CASH",15000, 2000, 0x0FFFFFFF);

	
	//Lalien: Game starting time
	gGameExternalOptions.iGameStartingTime			= iniReader.ReadInteger("JA2 Gameplay Settings", "GAME_STARTING_TIME", NUM_SEC_IN_HOUR, 0, NUM_SEC_IN_DAY-1);
	gGameExternalOptions.iGameStartingTime += NUM_SEC_IN_DAY;
	
	gGameExternalOptions.iFirstArrivalDelay			= iniReader.ReadInteger("JA2 Gameplay Settings", "FIRST_ARRIVAL_DELAY", 6 * NUM_SEC_IN_HOUR, 0, NUM_SEC_IN_DAY);

	//################# Settings valid on game start only end ##################


	gGameExternalOptions.fSellAll					= iniReader.ReadBoolean("JA2 Gameplay Settings","CAN_SELL_ALT_LMB",FALSE);
	gGameExternalOptions.iPriceModifier				= (INT16) iniReader.ReadInteger("JA2 Gameplay Settings","PRICE_MODIFIER",10, -1, 100);


	// WDS - Option to turn off stealing
	gGameExternalOptions.fStealingDisabled			= iniReader.ReadBoolean("JA2 Gameplay Settings","STEALING_FROM_SHIPMENTS_DISABLED",FALSE);

	// WDS: Game progress 
	gGameExternalOptions.ubGameProgressPortionKills		= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_KILLS", 25, 0, 100);
	gGameExternalOptions.ubGameProgressPortionControl	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_CONTROL", 25, 0, 100);
	gGameExternalOptions.ubGameProgressPortionIncome	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_INCOME", 50, 0, 100);
	gGameExternalOptions.ubGameProgressPortionVisited	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_VISITED", 0, 0, 100);
	gGameExternalOptions.ubGameProgressMinimum			= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_MINIMUM", 0, 0, 100);
	gGameExternalOptions.ubGameProgressIncrement		= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_INCREMENT", 0, 0, 100);

	// Any way to warn on this?
	if (gGameExternalOptions.ubGameProgressPortionKills + 
		gGameExternalOptions.ubGameProgressPortionControl + 
		gGameExternalOptions.ubGameProgressPortionIncome + 
		gGameExternalOptions.ubGameProgressPortionVisited != 100) 
	{
		gGameExternalOptions.ubGameProgressPortionKills		= 25;
		gGameExternalOptions.ubGameProgressPortionControl	= 25;
		gGameExternalOptions.ubGameProgressPortionIncome	= 50;
		gGameExternalOptions.ubGameProgressPortionVisited	= 0;
	}

	// WDS: Advanced start - removed - SANDRO
	//gGameExternalOptions.ubIMPStartingLevel = iniReader.ReadInteger("JA2 Gameplay Settings","IMP_STARTING_LEVEL", 1, 1, 7);

	// WDS - make number of mercenaries, etc. be configurable
	gGameExternalOptions.ubGameMaximumNumberOfPlayerMercs		= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_NUMBER_PLAYER_MERCS", 24, 1, CODE_MAXIMUM_NUMBER_OF_PLAYER_MERCS);
	gGameExternalOptions.ubGameMaximumNumberOfPlayerVehicles	= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_NUMBER_PLAYER_VEHICLES", 2, 2, CODE_MAXIMUM_NUMBER_OF_PLAYER_VEHICLES);
	gGameExternalOptions.ubGameMaximumNumberOfEnemies	= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_NUMBER_ENEMIES", 32, 16, CODE_MAXIMUM_NUMBER_OF_ENEMIES);
	gGameExternalOptions.ubGameMaximumNumberOfCreatures	= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_NUMBER_CREATURES", 32, 0, CODE_MAXIMUM_NUMBER_OF_CREATURES);
	gGameExternalOptions.ubGameMaximumNumberOfRebels	= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_NUMBER_REBELS", 32, 16, CODE_MAXIMUM_NUMBER_OF_REBELS);
	gGameExternalOptions.ubGameMaximumNumberOfCivilians	= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_NUMBER_CIVS", 32, 16, CODE_MAXIMUM_NUMBER_OF_CIVS);

	//Global game events 
	gGameExternalOptions.ubGameProgressStartMadlabQuest = iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_START_MADLAB_QUEST",35, 1, 100);
	gGameExternalOptions.ubGameProgressMikeAvailable	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_MIKE_AVAILABLE",50, 1, 100);
	gGameExternalOptions.ubGameProgressIggyAvaliable	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_IGGY_AVAILABLE",70, 1, 100);
	gGameExternalOptions.ubSendTroopsToDrassen			= iniReader.ReadBoolean("JA2 Gameplay Settings","STRATEGIC_EVENT_SEND_TROOPS_TO_DRASSEN",TRUE);

	//Weapons modification
	gGameExternalOptions.ubExplosivesDamageMultiplier	= iniReader.ReadInteger("JA2 Gameplay Settings","EXPLOSIVES_DAMAGE_MULTIPLIER",0, 0, 100);
	gGameExternalOptions.ubMeleeDamageMultiplier		= iniReader.ReadInteger("JA2 Gameplay Settings","MELEE_DAMAGE_MULTIPLIER",0, 0, 100);
	gGameExternalOptions.ubGunDamageMultiplier			= iniReader.ReadInteger("JA2 Gameplay Settings","GUN_DAMAGE_MULTIPLIER",0, 0, 100);


	//Strategic AI settings
	gGameExternalOptions.iMaxEnemyGroupSize					= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_STRATEGIC_TEAM_SIZE",20, 10, 100);

	gGameExternalOptions.iReinforcementPoolNovice			= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_QUEENS_POOL_OF_TROOPS",150, 25, 1000000);
	gGameExternalOptions.iReinforcementPoolExperienced		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_QUEENS_POOL_OF_TROOPS",200, 25, 1000000);
	gGameExternalOptions.iReinforcementPoolExpert			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_QUEENS_POOL_OF_TROOPS",400, 25, 1000000);
	gGameExternalOptions.iReinforcementPoolInsane			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_QUEENS_POOL_OF_TROOPS",8000, 25, 1000000);

	gGameExternalOptions.iForcePercentageNovice				= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_INITIAL_GARRISON_PERCENTAGES",70, 1, 200);
	gGameExternalOptions.iForcePercentageExperienced		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_INITIAL_GARRISON_PERCENTAGES",100, 1, 300);
	gGameExternalOptions.iForcePercentageExpert				= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_INITIAL_GARRISON_PERCENTAGES",150, 1, 400);
	gGameExternalOptions.iForcePercentageInsane				= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_INITIAL_GARRISON_PERCENTAGES",200, 1, 600);

	gGameExternalOptions.iPercentElitesBonusExperienced		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_ELITE_BONUS",0, 0, 100);
	gGameExternalOptions.iPercentElitesBonusExpert			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_ELITE_BONUS",25, 0, 100);
	gGameExternalOptions.iPercentElitesBonusInsane			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_ELITE_BONUS",50, 0, 100);

	gGameExternalOptions.ubMinEnemyGroupSizeNovice			= (UINT8) iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_MIN_ENEMY_GROUP_SIZE",3, 1, CODE_MAXIMUM_NUMBER_OF_ENEMIES);
	gGameExternalOptions.ubMinEnemyGroupSizeExperienced		= (UINT8) iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_MIN_ENEMY_GROUP_SIZE",4, 1, CODE_MAXIMUM_NUMBER_OF_ENEMIES);
	gGameExternalOptions.ubMinEnemyGroupSizeExpert			= (UINT8) iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_MIN_ENEMY_GROUP_SIZE",6, 1, CODE_MAXIMUM_NUMBER_OF_ENEMIES);
	gGameExternalOptions.ubMinEnemyGroupSizeInsane			= (UINT8) iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_MIN_ENEMY_GROUP_SIZE",12, 1, CODE_MAXIMUM_NUMBER_OF_ENEMIES);

	gGameExternalOptions.ubEasyEnemyStartingAlertLevel		= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_ENEMY_STARTING_ALERT_LEVEL", 5, 1, 100);
	gGameExternalOptions.ubNormalEnemyStartingAlertLevel	= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_ENEMY_STARTING_ALERT_LEVEL", 20, 1, 100);
	gGameExternalOptions.ubHardEnemyStartingAlertLevel		= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_ENEMY_STARTING_ALERT_LEVEL", 60, 1, 100);
	gGameExternalOptions.ubInsaneEnemyStartingAlertLevel	= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_ENEMY_STARTING_ALERT_LEVEL", 80, 1, 100);

	gGameExternalOptions.ubEasyEnemyStartingAlertDecay		= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_ENEMY_STARTING_ALERT_DECAY", 75, 1, 100);
	gGameExternalOptions.ubNormalEnemyStartingAlertDecay	= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_ENEMY_STARTING_ALERT_DECAY", 50, 1, 100);
	gGameExternalOptions.ubHardEnemyStartingAlertDecay		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_ENEMY_STARTING_ALERT_DECAY", 25, 1, 100);
	gGameExternalOptions.ubInsaneEnemyStartingAlertDecay	= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_ENEMY_STARTING_ALERT_DECAY", 10, 1, 100);

	gGameExternalOptions.ubEasyTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_TIME_EVALUATE_IN_MINUTES", 480, 10, NUM_MIN_IN_DAY);
	gGameExternalOptions.ubNormalTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_TIME_EVALUATE_IN_MINUTES", 360, 10, NUM_MIN_IN_DAY);
	gGameExternalOptions.ubHardTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_TIME_EVALUATE_IN_MINUTES", 180, 10, NUM_MIN_IN_DAY);
	gGameExternalOptions.ubInsaneTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_TIME_EVALUATE_IN_MINUTES", 90, 10, NUM_MIN_IN_DAY);

	gGameExternalOptions.ubEasyTimeEvaluateVariance			= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_TIME_EVALUATE_VARIANCE", 240, 10, NUM_MIN_IN_DAY);
	gGameExternalOptions.ubNormalTimeEvaluateVariance		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_TIME_EVALUATE_VARIANCE", 180, 10, NUM_MIN_IN_DAY);
	gGameExternalOptions.ubHardTimeEvaluateVariance			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_TIME_EVALUATE_VARIANCE", 120, 10, NUM_MIN_IN_DAY);
	gGameExternalOptions.ubInsaneTimeEvaluateVariance		= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_TIME_EVALUATE_VARIANCE", 60, 10, NUM_MIN_IN_DAY);

	gGameExternalOptions.ubEasyGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_GRACE_PERIOD_IN_HOURS", 144, 1, 288);
	gGameExternalOptions.ubNormalGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_GRACE_PERIOD_IN_HOURS", 96, 1, 288);
	gGameExternalOptions.ubHardGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_GRACE_PERIOD_IN_HOURS", 48, 1, 288);
	gGameExternalOptions.ubInsaneGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_GRACE_PERIOD_IN_HOURS", 6, 1, 288);

	gGameExternalOptions.ubEasyPatrolGracePeriodInDays		= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_PATROL_GRACE_PERIOD_IN_DAYS", 16, 1, 100);
	gGameExternalOptions.ubNormalPatrolGracePeriodInDays	= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_PATROL_GRACE_PERIOD_IN_DAYS", 12, 1, 100);
	gGameExternalOptions.ubHardPatrolGracePeriodInDays		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_PATROL_GRACE_PERIOD_IN_DAYS", 8, 1, 100);
	gGameExternalOptions.ubInsanePatrolGracePeriodInDays	= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_PATROL_GRACE_PERIOD_IN_DAYS", 2, 1, 100);

	gGameExternalOptions.ubEasyNumAwareBattles				= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_NUM_AWARE_BATTLES", 1, 1, 10);
	gGameExternalOptions.ubNormalNumAwareBattles			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_NUM_AWARE_BATTLES", 2, 1, 10);
	gGameExternalOptions.ubHardNumAwareBattles				= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_NUM_AWARE_BATTLES", 3, 1, 10);
	gGameExternalOptions.ubInsaneNumAwareBattles			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_NUM_AWARE_BATTLES", 4, 1, 10);

	//New v1.13 settings
	gGameExternalOptions.gfEasyUnlimitedTroops				= iniReader.ReadBoolean("JA2 Gameplay Settings","NOVICE_UNLIMITED_POOL_OF_TROOPS",FALSE);
	gGameExternalOptions.gfNormalUnlimitedTroops			= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERIENCED_UNLIMITED_POOL_OF_TROOPS",FALSE);
	gGameExternalOptions.gfHardUnlimitedTroops				= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERT_UNLIMITED_POOL_OF_TROOPS",FALSE);
	gGameExternalOptions.gfInsaneUnlimitedTroops			= iniReader.ReadBoolean("JA2 Gameplay Settings","INSANE_UNLIMITED_POOL_OF_TROOPS",TRUE);

	gGameExternalOptions.gfEasyAggressiveQueen				= iniReader.ReadBoolean("JA2 Gameplay Settings","NOVICE_QUEEN_AGGRESSIVE",FALSE);
	gGameExternalOptions.gfNormalAggressiveQueen			= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERIENCED_QUEEN_AGGRESSIVE",FALSE);
	gGameExternalOptions.gfHardAggressiveQueen				= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERT_QUEEN_AGGRESSIVE",FALSE);
	gGameExternalOptions.gfInsaneAggressiveQueen			= iniReader.ReadBoolean("JA2 Gameplay Settings","INSANE_QUEEN_AGGRESSIVE",TRUE);

	// WDS - New AI
	gGameExternalOptions.useNewAI							= iniReader.ReadBoolean("JA2 Gameplay Settings","NEW_AGGRESSIVE_AI",FALSE);

	gGameExternalOptions.gfInvestigateSector				= iniReader.ReadBoolean("JA2 Gameplay Settings","ENEMY_INVESTIGATE_SECTOR",FALSE);
	gGameExternalOptions.gfReassignPendingReinforcements	= iniReader.ReadBoolean("JA2 Gameplay Settings","REASSIGN_PENDING_REINFORCEMENTS",TRUE);

	gGameExternalOptions.ubEnemiesItemDrop					= iniReader.ReadInteger("JA2 Gameplay Settings","ENEMIES_ITEM_DROP", 0, 0, 1);

	gGameExternalOptions.gfUseExternalLoadscreens			= iniReader.ReadBoolean("JA2 Gameplay Settings","USE_EXTERNALIZED_LOADSCREENS", FALSE);

	// Militia settings
	gGameExternalOptions.guiAllowMilitiaGroupsDelay			= iniReader.ReadInteger("JA2 Gameplay Settings","ALLOW_MILITIA_MOBILE_DELAY",1, 0, 30);
	gGameExternalOptions.guiTrainVeteranMilitiaDelay		= iniReader.ReadInteger("JA2 Gameplay Settings","TRAIN_VETERAN_MILITIA_DELAY",1, 0, 30);
	gGameExternalOptions.gfTrainVeteranMilitia				= iniReader.ReadBoolean("JA2 Gameplay Settings","TRAIN_VETERAN_MILITIA",FALSE);
	gGameExternalOptions.gfAllowMilitiaGroups				= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_MILITIA_MOBILE_GROUPS",FALSE);
	gGameExternalOptions.gfmusttrainroaming					= iniReader.ReadBoolean("JA2 Gameplay Settings","MUST_TRAIN_MOBILE_MILITIA",FALSE);
	gGameExternalOptions.gfAllowReinforcements				= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_REINFORCEMENTS",FALSE);
	gGameExternalOptions.gfAllowReinforcementsOnlyInCity	= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_REINFORCEMENTS_ONLY_IN_CITIES",FALSE);
	gGameExternalOptions.guiBaseQueenPoolIncrement			= iniReader.ReadInteger("JA2 Gameplay Settings","QUEEN_POOL_INCREMENT_PER_DIFFICULTY_LEVEL",60, 1, 100);

	gGameExternalOptions.guiCreateEachNHours				= iniReader.ReadInteger("JA2 Gameplay Settings","CREATE_EACH_N_HOURS",24, 1, 96);
	gGameExternalOptions.guiDivOfOriginalMilitia			= iniReader.ReadInteger("JA2 Gameplay Settings","DIV_OF_ORIGINAL_MILITIA",4, 1, 100);
	gGameExternalOptions.guiMinMilitiaSquadSize				= iniReader.ReadInteger("JA2 Gameplay Settings","MINIMUM_MILITIA_SQUAD_SIZE",5, 1, CODE_MAXIMUM_NUMBER_OF_REBELS);
	gGameExternalOptions.guiMaxMilitiaSquadSize				= iniReader.ReadInteger("JA2 Gameplay Settings","MAXIMUM_MILITIA_SQUAD_SIZE",20, gGameExternalOptions.guiMinMilitiaSquadSize, CODE_MAXIMUM_NUMBER_OF_REBELS);
	gGameExternalOptions.guiMaxMilitiaSquadSizeBattle		= gGameExternalOptions.guiMaxMilitiaSquadSize;

	gGameExternalOptions.iMaxMilitiaPerSector				= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_MILITIA_PER_SECTOR",20, 1, CODE_MAXIMUM_NUMBER_OF_REBELS);
	gGameExternalOptions.iTrainingSquadSize					= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_TRAINING_SQUAD_SIZE",10, 1, CODE_MAXIMUM_NUMBER_OF_REBELS);
	gGameExternalOptions.iMilitiaTrainingCost				= iniReader.ReadInteger("JA2 Gameplay Settings","MILITIA_TRAINING_COST",750, 10, 10000);
	gGameExternalOptions.iMilitiaCostModifier				= iniReader.ReadInteger("JA2 Gameplay Settings","MILITIA_COST_MODIFIER",3, 1, 10);
	gGameExternalOptions.iRegularCostModifier				= iniReader.ReadInteger("JA2 Gameplay Settings","REGULAR_COST_MODIFIER",1, 1, 10);
	gGameExternalOptions.iVeteranCostModifier				= iniReader.ReadInteger("JA2 Gameplay Settings","VETERAN_COST_MODIFIER",2, 1, 10);
	gGameExternalOptions.iMinLoyaltyToTrain					= iniReader.ReadInteger("JA2 Gameplay Settings","MIN_LOYALTY_TO_TRAIN",20, 0, 100);
	gGameExternalOptions.gflimitedRoaming					= iniReader.ReadBoolean("JA2 Gameplay Settings","RESTRICT_ROAMING",FALSE);

	// WDS - Improve Tony's and Devin's inventory like BR's
	gGameExternalOptions.tonyUsesBRSetting					= iniReader.ReadBoolean("JA2 Gameplay Settings","TONY_USES_BR_SETTING", FALSE);
	gGameExternalOptions.devinUsesBRSetting					= iniReader.ReadBoolean("JA2 Gameplay Settings","DEVIN_USES_BR_SETTING", FALSE);

	// WDS - Smart goggle switching
	gGameExternalOptions.smartGoggleSwitch					= iniReader.ReadBoolean("JA2 Gameplay Settings","SMART_GOGGLE_SWITCH", FALSE);

	// WDS - Automatically flag mines
	gGameExternalOptions.automaticallyFlagMines				= iniReader.ReadBoolean("JA2 Gameplay Settings","AUTOMATICALLY_FLAG_MINES", FALSE);

	//Assignment Settings (training, repairing, doctoring, etc)
	gGameExternalOptions.ubAssignmentUnitsPerDay			= iniReader.ReadInteger("JA2 Gameplay Settings","ASSIGNMENT_UNITS_PER_DAY",24, 1, 96);
	gGameExternalOptions.ubMinutesForAssignmentToCount		= iniReader.ReadInteger("JA2 Gameplay Settings","MINUTES_FOR_ASSIGNMENT_TO_COUNT",45, 1, NUM_MIN_IN_DAY);


	gGameExternalOptions.ubTrainingSkillMin					= iniReader.ReadInteger("JA2 Gameplay Settings","TRAINING_SKILL_MIN",0, 0, 100);
	gGameExternalOptions.ubTrainingSkillMax					= iniReader.ReadInteger("JA2 Gameplay Settings","TRAINING_SKILL_MAX",100, 0, 100);
	gGameExternalOptions.ubSelfTrainingDivisor				= iniReader.ReadInteger("JA2 Gameplay Settings","SELF_TRAINING_DIVISOR",1000, 1, 10000);
	gGameExternalOptions.ubInstructedTrainingDivisor		= iniReader.ReadInteger("JA2 Gameplay Settings","INSTRUCTED_TRAINING_DIVISOR",3000, 1, 10000);
	gGameExternalOptions.ubGunRangeTrainingBonus			= iniReader.ReadInteger("JA2 Gameplay Settings","GUN_RANGE_TRAINING_BONUS",25, 0, 100);
	gGameExternalOptions.ubTownMilitiaTrainingRate			= iniReader.ReadInteger("JA2 Gameplay Settings","MILITIA_TRAINING_RATE",4, 1, 10);
	gGameExternalOptions.ubMaxMilitiaTrainersPerSector		= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_MILITIA_TRAINERS_PER_SECTOR",2, 1, gGameExternalOptions.ubGameMaximumNumberOfPlayerMercs);
	gGameExternalOptions.ubTeachBonusToTrain				= iniReader.ReadInteger("JA2 Gameplay Settings","TEACH_BONUS_TO_TRAIN",30, 0, 100);
	gGameExternalOptions.ubRpcBonusToTrainMilitia			= iniReader.ReadInteger("JA2 Gameplay Settings","RPC_BONUS_TO_TRAIN_MILITIA",10, 0, 100);
	gGameExternalOptions.ubMinSkillToTeach					= iniReader.ReadInteger("JA2 Gameplay Settings","MIN_RATING_TO_TEACH",25, 0, 100);

	gGameExternalOptions.ubLowActivityLevel					= iniReader.ReadInteger("JA2 Gameplay Settings","LOW_ACTIVITY_LEVEL",1, 1, 100);
	gGameExternalOptions.ubMediumActivityLevel				= iniReader.ReadInteger("JA2 Gameplay Settings","MEDIUM_ACTIVITY_LEVEL",4, 1, 100);
	gGameExternalOptions.ubHighActivityLevel				= iniReader.ReadInteger("JA2 Gameplay Settings","HIGH_ACTIVITY_LEVEL",12, 1, 100);
	gGameExternalOptions.ubDoctoringRateDivisor				= iniReader.ReadInteger("JA2 Gameplay Settings","DOCTORING_RATE_DIVISOR",2400, 1200, 24000);
	gGameExternalOptions.ubHospitalHealingRate				= iniReader.ReadInteger("JA2 Gameplay Settings","HOSPITAL_HEALING_RATE",5, 1, 10);
	gGameExternalOptions.ubBaseMedicalSkillToDealWithEmergency				= iniReader.ReadInteger("JA2 Gameplay Settings","BASE_MEDICAL_SKILL_TO_DEAL_WITH_EMERGENCY",20, 0, 100);
	gGameExternalOptions.ubMultiplierForDifferenceInLifeValueForEmergency	= iniReader.ReadInteger("JA2 Gameplay Settings","MULTIPLIER_FOR_DIFFERENCE_IN_LIFE_VALUE_FOR_EMERGENCY",4, 1, 10);
	gGameExternalOptions.ubPointCostPerHealthBelowOkLife	= iniReader.ReadInteger("JA2 Gameplay Settings","POINT_COST_PER_HEALTH_BELOW_OKLIFE",2, 1, 5);//OKLIFE = 15

	gGameExternalOptions.ubRepairCostPerJam					= iniReader.ReadInteger("JA2 Gameplay Settings","REPAIR_COST_PER_JAM",2, 1, 10);
	gGameExternalOptions.ubRepairRateDivisor				= iniReader.ReadInteger("JA2 Gameplay Settings","REPAIR_RATE_DIVISOR",2500, 1250, 25000);


	//Misc
	gGameExternalOptions.fAmmoDynamicWeight					= iniReader.ReadBoolean("JA2 Gameplay Settings", "DYNAMIC_AMMO_WEIGHT", TRUE); //Pulmu
	gGameExternalOptions.fEnableCrepitus					= iniReader.ReadBoolean("JA2 Gameplay Settings", "ENABLE_CREPITUS", TRUE);

	gGameExternalOptions.fEnableAllTerrorists				= iniReader.ReadBoolean("JA2 Gameplay Settings", "ENABLE_ALL_TERRORISTS", FALSE);
	gGameExternalOptions.fEnableAllWeaponCaches				= iniReader.ReadBoolean("JA2 Gameplay Settings", "ENABLE_ALL_WEAPON_CACHES", FALSE);

	gGameExternalOptions.fSlowProgressForEnemyItemsChoice	= iniReader.ReadBoolean("JA2 Gameplay Settings", "SLOW_PROGRESS_FOR_ENEMY_ITEMS_CHOICE", TRUE);

	// Kaiden: Vehicle Inventory change - Added INI file Option VEHICLE_INVENTORY
	gGameExternalOptions.fVehicleInventory					= iniReader.ReadBoolean("JA2 Gameplay Settings", "VEHICLE_INVENTORY", TRUE);

	// WANNE: Added INI file Option for enemy ambushes, so we also have the option to play vanilla ja2 ambush settings
	gGameExternalOptions.fEnableChanceOfEnemyAmbushesOnInsaneDifficult = iniReader.ReadBoolean("JA2 Gameplay Settings", "ENABLE_CHANCE_OF_ENEMY_AMBUSHES_ON_INSANE_DIFFICULT", TRUE);

	// CHRISL: New setting to allow Slay to remain as a hired PC
	gGameExternalOptions.fEnableSlayForever					= iniReader.ReadBoolean("JA2 Gameplay Settings", "SLAY_FOREVER", FALSE);

	// CHRISL: Setting to turn off the description and stack popup options from the sector inventory panel
	gGameExternalOptions.fSectorDesc						= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_SECTOR_DESCRIPTION_WINDOW",TRUE);

	//afp - use bullet tracers?
	gGameExternalOptions.gbBulletTracer						= iniReader.ReadBoolean("JA2 Gameplay Settings","USE_BULLET_TRACERS",FALSE);

	gGameExternalOptions.fRestrictFemaleEnemiesExceptElite	= iniReader.ReadBoolean("JA2 Gameplay Settings","RESTRICT_FEMALE_ENEMIES_EXCEPT_ELITE",FALSE);

	// HEADROCK: Use Enhanced Item Description Box?
	gGameExternalOptions.fEnhancedDescriptionBox			= iniReader.ReadInteger("JA2 Gameplay Settings","USE_ENHANCED_DESCRIPTION_BOX",0, 0, 2);

	// WANNE: Always use prof.dat??
	gGameExternalOptions.fAlwaysUseProfDat					= iniReader.ReadBoolean("JA2 Gameplay Settings", "ALWAYS_USE_PROF_DAT", FALSE);


	// HEADROCK HAM B1: Set % of mine income, where 100% = normal
	gGameExternalOptions.iMineIncomePercentage				= iniReader.ReadInteger("JA2 HAM Settings","MINE_INCOME_PERCENTAGE",0);
	// HEADROCK HAM B2.4: Set a threshold at 1% - otherwise game crashes on asserting mine income. 
	// Can anyone have a look at circumventing the income check? I don't want to cause any damage...
	gGameExternalOptions.iMineIncomePercentage = __max(gGameExternalOptions.iMineIncomePercentage, 100);

	// HEADROCK HAM B1: Set minimum and maximum CTH
	gGameExternalOptions.iMaximumCTH						= iniReader.ReadInteger("JA2 HAM Settings","MAXIMUM_POSSIBLE_CTH",99);

	gGameExternalOptions.iMinimumCTH						= iniReader.ReadInteger("JA2 HAM Settings","MINIMUM_POSSIBLE_CTH",0);

	// HEADROCK HAM B1: Set minimum CTH at fraction between 0 and 1 ( MINCTH = 1/(100*divisor) ) Note Minimum above must be 0.
	gGameExternalOptions.iMinimumCTHDivisor					= iniReader.ReadInteger("JA2 HAM Settings","MINIMUM_CTH_DIVISOR",100);

	// HEADROCK HAM B1: Allow restricted militia to move through visited sectors?
	gGameExternalOptions.bUnrestrictVisited					= iniReader.ReadBoolean("JA2 HAM Settings","ALLOW_RESTRICTED_MILITIA_THROUGH_VISITED_SECTORS",FALSE);

	gGameExternalOptions.bDynamicRestrictRoaming			= iniReader.ReadBoolean("JA2 HAM Settings","ALLOW_DYNAMIC_RESTRICTED_ROAMING",TRUE);

	// HEADROCK HAM B2: Reset suppression counter. 0 = never (oldskool), 1 = Every turn, 2 = Every attack.
	gGameExternalOptions.iClearSuppression					= iniReader.ReadInteger("JA2 HAM Settings","CLEAR_SUPPRESSION_COUNTER",1);

	// HEADROCK HAM B2.1: This controls how effective suppression is, by increasing the number of ubSuppressionPoints accumulated by combatants (percentage);
	gGameExternalOptions.iSuppressionEffectiveness			= iniReader.ReadInteger("JA2 HAM Settings","SUPPRESSION_EFFECTIVENESS",0);

	// HEADROCK HAM B2: MAXIMUM number of APs that can be lost to suppression in a given turn (0=unlimited)
	gGameExternalOptions.fSuppressionAPLossPerTurn			= iniReader.ReadBoolean("JA2 HAM Settings","LIMITED_SUPPRESSION_AP_LOSS_PER_TURN",TRUE);

	// HEADROCK HAM B2: MAXIMUM number of APs that can be lost to suppression in a given attack (0=unlimited)
	gGameExternalOptions.fSuppressionAPLossPerAttack		= iniReader.ReadBoolean("JA2 HAM Settings","LIMITED_SUPPRESSION_AP_LOSS_PER_ATTACK",TRUE);

	gGameExternalOptions.iSuppressionToleranceMax			= iniReader.ReadInteger("JA2 HAM Settings","SUPPRESSION_TOLERANCE_MAX", 18, 1, 24);
	gGameExternalOptions.iSuppressionToleranceMin			= iniReader.ReadInteger("JA2 HAM Settings","SUPPRESSION_TOLERANCE_MIN", 1, 0, 24);

	// HEADROCK HAM B2: Suppression Shock ON/OFF
	gGameExternalOptions.fSuppressionShock					= iniReader.ReadBoolean("JA2 HAM Settings","SUPPRESSION_SHOCK",FALSE);

	// HEADROCK HAM B2: Suppression Shock effectiveness (percentage, 100 = "normal", 0 = deactivated. Range 0-65535)
	gGameExternalOptions.iSuppressionShockEffectiveness		= iniReader.ReadInteger("JA2 HAM Settings","SUPPRESSION_SHOCK_EFFECTIVENESS",0);

	// HEADROCK HAM B2.1: CTH penalty given by a "Cowering" target to any enemy shooter.
	gGameExternalOptions.iAimPenaltyPerTargetShock			= iniReader.ReadInteger("JA2 HAM Settings","AIM_PENALTY_PER_TARGET_SHOCK",0);

	// HEADROCK HAM B2.3: A "cowering" soldier is twice as susceptible to suppression.
	gGameExternalOptions.iCowerEffectOnSuppression			= iniReader.ReadInteger("JA2 HAM Settings","COWER_EFFECT_ON_SUPPRESSION",0);

	// HEADROCK HAM B2.5: Turn on Realistic Tracers. 0 = off (regular tracers). 1 = Fully realistic tracers. 2 = Tracer Bump + 1.13 Autofire Penalty Reduction
	gGameExternalOptions.iRealisticTracers					= iniReader.ReadInteger("JA2 HAM Settings","REALISTIC_TRACERS",0);

	// HEADROCK HAM B2.5: Realistic tracers - one of every X bullets in a tracer magazines will be a tracer bullet. 0 = off (JA2 normal)
	gGameExternalOptions.iNumBulletsPerTracer				= __max(iniReader.ReadInteger("JA2 HAM Settings","NUM_BULLETS_PER_TRACER",1),1);

	// HEADROCK HAM B2.5: Realistic tracers - CTH increased by this amount whenever a tracer is fired. 0 = off.
	gGameExternalOptions.iCTHBumpPerTracer					= iniReader.ReadInteger("JA2 HAM Settings","CTH_BUMP_PER_TRACER",0);

	// HEADROCK HAM B2.6: Increased aiming costs?
	gGameExternalOptions.fIncreasedAimingCost				= iniReader.ReadBoolean("JA2 HAM Settings","INCREASED_AIM_COST", FALSE);

	// HEADROCK HAM B2.6: Dynamically determine Max-Aiming based also on weapon, bipod, etc?
	gGameExternalOptions.fDynamicAimingTime					= iniReader.ReadBoolean("JA2 HAM Settings","DYNAMIC_AIM_LIMITS", FALSE);

	// HEADROCK HAM B2.6: Controls how much effect target movement has on aiming
	//gGameExternalOptions.iMovementEffectOnAiming			= iniReader.ReadFloat("JA2 HAM Settings","MOVEMENT_EFFECT_ON_AIMING", 1.5);
	gGameExternalOptions.iMovementEffectOnAiming			= (float)iniReader.ReadDouble("JA2 HAM Settings","MOVEMENT_EFFECT_ON_AIMING", 1.5, 0.0, 255.0);

	// HEADROCK HAM B2.6: Autofire Bullets/5AP modifier
	gGameExternalOptions.iAutofireBulletsPer5APModifier		= iniReader.ReadInteger("JA2 HAM Settings","AUTOFIRE_BULLETS_PER_5AP_MODIFIER", 0);

	// HEADROCK HAM B2.6/B1: Adjustable "luck" factor in Auto-Resolve
	//gGameExternalOptions.iAutoResolveLuckFactor			= (float)(__max(iniReader.ReadFloat("JA2 HAM Settings","AUTORESOLVE_LUCK_FACTOR", 2.0),1.0));
	gGameExternalOptions.iAutoResolveLuckFactor				= (float)(__max(iniReader.ReadDouble("JA2 HAM Settings","AUTORESOLVE_LUCK_FACTOR", 2.0, 0.0, 255.0),1.0));

	// HEADROCK HAM B2.6: Adjustable maximum for Suppression Shock effect. This has the added effect of reducing overall susceptibility to shock and may render some enemies unshockable.
	gGameExternalOptions.iMaxSuppressionShock				= iniReader.ReadInteger("JA2 HAM Settings","MAXIMUM_SUPPRESSION_SHOCK", 0);

	// HEADROCK HAM B2.6/2/1: Toggle new Burst/Auto CTH bars: 0=neither, 1=both, 2=Burst, 3=Auto
	gGameExternalOptions.iNewCTHBars						= iniReader.ReadInteger("JA2 HAM Settings","NEW_BURST-AUTO_CTH_BARS", 0);

	// HEADROCK HAM B2.6: Toggle whether AI checks for larger magazine when wanting to suppress at a distance
	gGameExternalOptions.fIncreaseAISuppressionFire			= iniReader.ReadBoolean("JA2 HAM Settings","INCREASE_AI_WILLINGNESS_TO_SUPPRESS", FALSE);

	// HEADROCK HAM B2.7: Change the speed of skill progression. (defaults set to JA2 normal)
	gGameExternalOptions.ubHealthSubpointsToImprove			= __max(1,iniReader.ReadInteger("JA2 HAM Settings","HEALTH_SUBPOINTS_TO_IMPROVE", 50));
	gGameExternalOptions.ubStrengthSubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","STRENGTH_SUBPOINTS_TO_IMPROVE", 50));
	gGameExternalOptions.ubDexteritySubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","DEXTERITY_SUBPOINTS_TO_IMPROVE", 50));
	gGameExternalOptions.ubAgilitySubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","AGILITY_SUBPOINTS_TO_IMPROVE", 50));
	gGameExternalOptions.ubWisdomSubpointsToImprove			= __max(1,iniReader.ReadInteger("JA2 HAM Settings","WISDOM_SUBPOINTS_TO_IMPROVE", 50));
	gGameExternalOptions.ubMarksmanshipSubpointsToImprove	= __max(1,iniReader.ReadInteger("JA2 HAM Settings","MARKSMANSHIP_SUBPOINTS_TO_IMPROVE", 25));
	gGameExternalOptions.ubMedicalSubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","MEDICAL_SUBPOINTS_TO_IMPROVE", 25));
	gGameExternalOptions.ubMechanicalSubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","MECHANICAL_SUBPOINTS_TO_IMPROVE", 25));
	gGameExternalOptions.ubExplosivesSubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","EXPLOSIVES_SUBPOINTS_TO_IMPROVE", 25));
	gGameExternalOptions.ubLeadershipSubpointsToImprove		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","LEADERSHIP_SUBPOINTS_TO_IMPROVE", 25));
	gGameExternalOptions.ubLevelSubpointsToImprove			= __max(1,__min(iniReader.ReadInteger("JA2 HAM Settings","LEVEL_SUBPOINTS_TO_IMPROVE", 350), 6500));

	// HEADROCK HAM B2.7: When turned on, this will give a CTH approximation instead of an exact value, on CTH Bars and "F" key feedback.
	gGameExternalOptions.fApproximateCTH					= iniReader.ReadBoolean("JA2 HAM Settings","APPROXIMATE_CTH", FALSE);

	// HEADROCK HAM B2.7: Augmented Roaming Militia code - turn this to TRUE to allow militia free travel through San Mona, Tixa, Orta, Omerta, and Estoni.
	gGameExternalOptions.fAllowMilitiaMoveThroughMinorCities		= iniReader.ReadBoolean("JA2 HAM Settings","ALLOW_MILITIA_MOVE_THROUGH_MINOR_CITIES", FALSE);

	// HEADROCK HAM B2.7: Smarter Roaming Militia Generator will generate Roamers in any available city perimeter square, allowing us to avoid "wasting" training sessions more easily.
	gGameExternalOptions.fSmartRoamingMilitiaGenerator		= iniReader.ReadBoolean("JA2 HAM Settings","SMART_ROAMING_MILITIA_GENERATOR", FALSE);

	// HEADROCK HAM B2.7: When "TRUE", Roaming Militia will be created on a 25% Veteran, 25% Regular, 50% green basis. If new militia is placed into a full group, it will upgrade greens/regulars in that group. 
	gGameExternalOptions.fDiverseRoamingMilitiaGroups		= iniReader.ReadBoolean("JA2 HAM Settings","DIVERSE_ROAMING_MILITIA_GROUPS", FALSE);

	// HEADROCK HAM B2.8: This setting controls whether militia will try to join up into the largest groups they can, or average out the size of the groups to cover more territory.
	gGameExternalOptions.ubRoamingMilitiaSpreadsOutChance	= iniReader.ReadInteger("JA2 HAM Settings","ROAMING_MILITIA_SPREADOUT_CHANCE", 0);

	// HEADROCK HAM B2.8: These are new cowering penalty divisors that help us determine how effective cowering is in different stances and when the shooter is targetting different bodyparts
	gGameExternalOptions.ubCoweringPenaltyDivisorProne				= __max(1,iniReader.ReadInteger("JA2 HAM Settings","CTH_PENALTY_FOR_COWERING_PRONE_TARGET_DIVISOR", 1));
	gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedHead		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","CTH_PENALTY_FOR_COWERING_CROUCHED_TARGET_HEAD_DIVISOR", 3));
	gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedTorso		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","CTH_PENALTY_FOR_COWERING_CROUCHED_TARGET_TORSO_DIVISOR", 4));
	gGameExternalOptions.ubCoweringPenaltyDivisorCrouchedLegs		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","CTH_PENALTY_FOR_COWERING_CROUCHED_TARGET_LEGS_DIVISOR", 5));

	// HEADROCK HAM B2.8: This is the maximum range at which a target gives out the full CTH penalty for cowering. At lower range, it'll give proportionally less penalty.
	gGameExternalOptions.usMinRangeForFullCoweringPenalty	= __max(10,iniReader.ReadInteger("JA2 HAM Settings","MIN_RANGE_FOR_FULL_COWERING_TARGET_PENALTY", 300));

	// HEADROCK HAM B2.8: Absolute maximum CTH penalty from target/shooter cowering
	gGameExternalOptions.usMaxShooterCoweringPenalty		= __max(1,iniReader.ReadInteger("JA2 HAM Settings","MAX_SHOOTER_COWERING_PENALTY", 0));
	gGameExternalOptions.usMaxTargetCoweringPenalty			= __max(1,iniReader.ReadInteger("JA2 HAM Settings","MAX_TARGET_COWERING_PENALTY", 0));

	// HEADROCK HAM B2.8: If this is turned on, Militia will drop their equipment similar to enemies, IF killed by non-player character.
	gGameExternalOptions.ubMilitiaDropEquipment				= iniReader.ReadInteger("JA2 HAM Settings","MILITIA_DROP_EQUIPMENT", 0);

	// HEADROCK HAM B2.8: New Trainer Relations: 2 = Trainees will go to sleep when their trainer goes to sleep. 3 = Trainer will go to sleep if all trainees are asleep. 1 = Both. 0 = Neither.
	gGameExternalOptions.ubSmartTrainingSleep				= iniReader.ReadInteger("JA2 HAM Settings","SMART_TRAINING-SLEEP_HANDLER", 0);

	// HEADROCK HAM B2.8: New Trainer Relations: 2 = Trainees will wake up when their trainer wakes up. 3 = Trainer will wake up if all trainees wake up. 1 = Both. 0 = Neither.
	gGameExternalOptions.ubSmartTrainingWake				= iniReader.ReadInteger("JA2 HAM Settings","SMART_TRAINING-WAKE_HANDLER", 0);

	// HEADROCK HAM B2.8: New Trainer Relations: 2 = Trainers will rest if no trainees available. 3 = Trainees will rest if no trainers available (not recommended). 1 = Both. 0 = Neither.
	gGameExternalOptions.ubSmartTrainingRest				= iniReader.ReadInteger("JA2 HAM Settings","SMART_TRAINING_REST", 0);
}

INT16 DynamicAdjustAPConstants(INT16 iniReadValue, INT16 iniDefaultValue, BOOLEAN reverse)
{
	//CHRISL: This function will dynamically adjust all the APBPConstant values based on the AP_MAXIMUM value.  This way
	//	if you want to use a 50AP system instead of the default, you only need to alter one value in the INI file and all
	//	the other values will automatically be updated to reflect the new max value.  The way I've coded this, however, if
	//	you set your own value, you'll override this dynamic system.
	if(iniReadValue == iniDefaultValue)
	{
		if(!reverse)
			iniReadValue = (INT16)(((FLOAT)iniReadValue*(FLOAT)APBPConstants[AP_MAXIMUM]/100)+(iniReadValue<0?-.5:.5));
		else
			iniReadValue = (INT16)(((FLOAT)iniReadValue/(FLOAT)APBPConstants[AP_MAXIMUM]*100)+(iniReadValue<0?-.5:.5));
	}

	return iniReadValue;
}

void LoadGameAPBPConstants()
{
	// WDS - Chris you should fix up the ReadIntegers below to have valid min/max values.

	CIniReader iniReader(AP_BP_CONSTANTS_FILE);
	//CHRISL: To allow for dynamic settings, we need to switch AP_MAXIMUM and AP_MINIMUM so that only the max value needs
	//	to be changed to effect the entire system.  This will require a change in the ENUM so that we can use a loop to
	//	modify the remaining values.
	APBPConstants[AP_MAXIMUM] = iniReader.ReadInteger("APConstants","AP_MAXIMUM",100);
 
	//CHRISL: Once we've loaded the AP_MAXIMUM value, we can use it to modifiy all the remaining values
	APBPConstants[AP_MINIMUM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MINIMUM",40),40);
	APBPConstants[AP_MONSTER_MAXIMUM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MONSTER_MAXIMUM",160),160);
	APBPConstants[AP_VEHICLE_MAXIMUM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_VEHICLE_MAXIMUM",200),200);
	APBPConstants[MAX_AP_CARRIED] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","MAX_AP_CARRIED",20),20);

	//CHRISL: The following five values are actually used as percentile adjustments so we don't want to dynamically alter
	//	them.  I.E. 9 actually means 90% of the calculated APs.  15 = 150%.  etc.
	APBPConstants[AP_YOUNG_MONST_FACTOR] = iniReader.ReadInteger("APConstants","AP_YOUNG_MONST_FACTOR",15);
	APBPConstants[AP_MONST_FRENZY_FACTOR] = iniReader.ReadInteger("APConstants","AP_MONST_FRENZY_FACTOR",13);
	APBPConstants[AP_CLAUSTROPHOBE] = iniReader.ReadInteger("APConstants","AP_CLAUSTROPHOBE",9);
	APBPConstants[AP_AFRAID_OF_INSECTS] = iniReader.ReadInteger("APConstants","AP_AFRAID_OF_INSECTS",8);
	APBPConstants[AP_WRONG_MAG] = iniReader.ReadInteger("APConstants","AP_WRONG_MAG",15);

	APBPConstants[AP_EXCHANGE_PLACES] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_EXCHANGE_PLACES",20),20);
	APBPConstants[AP_REVERSE_MODIFIER] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_REVERSE_MODIFIER",4),4);
	APBPConstants[AP_STEALTH_MODIFIER] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_STEALTH_MODIFIER",8),8);
	APBPConstants[AP_STEAL_ITEM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_STEAL_ITEM",56),56);
	APBPConstants[AP_TAKE_BLOOD] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_TAKE_BLOOD",40),40);
	APBPConstants[AP_TALK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_TALK",24),24);
	APBPConstants[AP_MOVEMENT_FLAT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_FLAT",12),12);
	APBPConstants[AP_MOVEMENT_GRASS] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_GRASS",14),14);
	APBPConstants[AP_MOVEMENT_BUSH] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_BUSH",18),18);
	APBPConstants[AP_MOVEMENT_RUBBLE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_RUBBLE",24),24);
	APBPConstants[AP_MOVEMENT_SHORE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_SHORE",28),28);
	APBPConstants[AP_MOVEMENT_LAKE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_LAKE",36),36);
	APBPConstants[AP_MOVEMENT_OCEAN] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MOVEMENT_OCEAN",32),32);
	APBPConstants[AP_MODIFIER_RUN] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MODIFIER_RUN",-8),-8);
	APBPConstants[AP_MODIFIER_WALK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MODIFIER_WALK",-4),-4);
	APBPConstants[AP_MODIFIER_SWAT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MODIFIER_SWAT",0),0);
	APBPConstants[AP_MODIFIER_CRAWL] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MODIFIER_CRAWL",4),4);
	APBPConstants[AP_MODIFIER_PACK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MODIFIER_PACK",4),4);
	APBPConstants[AP_CHANGE_FACING] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CHANGE_FACING",4),4);
	APBPConstants[AP_CHANGE_TARGET] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CHANGE_TARGET",2),2);
	APBPConstants[AP_TOSS_ITEM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_TOSS_ITEM",32),32);
	APBPConstants[AP_REFUEL_VEHICLE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_REFUEL_VEHICLE",40),40);
	APBPConstants[AP_RADIO] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_RADIO",20),20);
	APBPConstants[AP_CROUCH] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CROUCH",6),6);
	APBPConstants[AP_PRONE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_PRONE",8),8);
	APBPConstants[AP_LOOK_STANDING] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_LOOK_STANDING",4),4);
	APBPConstants[AP_LOOK_CROUCHED] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_LOOK_CROUCHED",6),6);
	APBPConstants[AP_LOOK_PRONE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_LOOK_PRONE",8),8);
	APBPConstants[AP_READY_KNIFE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_READY_KNIFE",0),0);
	APBPConstants[AP_READY_DUAL] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_READY_DUAL",4),4);
	APBPConstants[AP_MIN_AIM_ATTACK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MIN_AIM_ATTACK",0),0);
	APBPConstants[AP_DROP_BOMB] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_DROP_BOMB",12),12);
	APBPConstants[AP_RELOAD_GUN] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_RELOAD_GUN",20),20);
	APBPConstants[AP_START_FIRST_AID] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_START_FIRST_AID",20),20);
	APBPConstants[AP_START_REPAIR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_START_REPAIR",20),20);

	//CHRISL: This value is a divisor which should not be dynamically altered based on maxAP
	APBPConstants[AP_GET_WOUNDED_DIVISOR] = iniReader.ReadInteger("APConstants","AP_GET_WOUNDED_DIVISOR",1);

	APBPConstants[AP_FALL_DOWN] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_FALL_DOWN",16),16);
	APBPConstants[AP_OPEN_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_OPEN_DOOR",12),12);
	APBPConstants[AP_PICKLOCK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_PICKLOCK",40),40);
	APBPConstants[AP_EXAMINE_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_EXAMINE_DOOR",20),20);
	APBPConstants[AP_BOOT_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_BOOT_DOOR",32),32);
	APBPConstants[AP_USE_CROWBAR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_USE_CROWBAR",40),40);
	APBPConstants[AP_UNLOCK_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_UNLOCK_DOOR",24),24);
	APBPConstants[AP_LOCK_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_LOCK_DOOR",24),24);
	APBPConstants[AP_EXPLODE_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_EXPLODE_DOOR",40),40);
	APBPConstants[AP_UNTRAP_DOOR] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_UNTRAP_DOOR",40),40);
	APBPConstants[AP_USEWIRECUTTERS] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_USEWIRECUTTERS",40),40);
	APBPConstants[AP_CLIMBROOF] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CLIMBROOF",40),40);
	APBPConstants[AP_CLIMBOFFROOF] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CLIMBOFFROOF",24),24);
	APBPConstants[AP_JUMPFENCE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_JUMPFENCE",24),24);
	APBPConstants[AP_JUMPFENCEBPACK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_JUMPFENCEBPACK",32),32);
	APBPConstants[AP_USE_REMOTE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_USE_REMOTE",8),8);
	APBPConstants[AP_PULL_TRIGGER] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_PULL_TRIGGER",8),8);
	APBPConstants[AP_PUNCH] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_PUNCH",16),16);
	APBPConstants[AP_PICKUP_ITEM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_PICKUP_ITEM",12),12);
	APBPConstants[AP_GIVE_ITEM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_GIVE_ITEM",4),4);
	APBPConstants[AP_BURY_MINE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_BURY_MINE",30),30);
	APBPConstants[AP_DISARM_MINE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_DISARM_MINE",40),40);
	APBPConstants[AP_DRINK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_DRINK",20),20);
	APBPConstants[AP_CAMOFLAGE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CAMOFLAGE",40),40);
	APBPConstants[AP_START_RUN_COST] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_START_RUN_COST",4),4);
	APBPConstants[AP_ATTACH_CAN] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_ATTACH_CAN",20),20);
	APBPConstants[AP_JUMP_OVER] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_JUMP_OVER",20),20);
	APBPConstants[AP_BACK_PACK] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_BACK_PACK",12),12);
	APBPConstants[AP_OPEN_ZIPPER] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_OPEN_ZIPPER",24),24);
	APBPConstants[AP_CLOSE_ZIPPER] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CLOSE_ZIPPER",28),28);
	APBPConstants[AP_CLICK_AIM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_CLICK_AIM",4),4);
	APBPConstants[AUTOFIRE_SHOTS_AP_VALUE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AUTOFIRE_SHOTS_AP_VALUE",20),20);
	APBPConstants[BAD_AP_COST] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","BAD_AP_COST",36),36);
	APBPConstants[AP_RELOAD_LOOSE] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_RELOAD_LOOSE",8),8);
	APBPConstants[AP_UNJAM] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_UNJAM",2),2);
	APBPConstants[AP_MAX_SUPPRESSED] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MAX_SUPPRESSED",64),64);
	APBPConstants[AP_MAX_TURN_SUPPRESSED] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MAX_TURN_SUPPRESSED",200),200);
	APBPConstants[AP_MIN_LIMIT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_MIN_LIMIT",-100),-100);
	APBPConstants[AP_LOST_PER_MORALE_DROP] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_LOST_PER_MORALE_DROP",12),12);
	APBPConstants[AP_SUPPRESSION_MOD] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","AP_SUPPRESSION_MOD",24),24);
	APBPConstants[DEFAULT_APS] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","DEFAULT_APS",80),80);
	APBPConstants[DEFAULT_AIMSKILL] = iniReader.ReadInteger("APConstants","DEFAULT_AIMSKILL",80);

	APBPConstants[BP_RATIO_RED_PTS_TO_NORMAL] = iniReader.ReadInteger("BPConstants","BP_RATIO_RED_PTS_TO_NORMAL",100);
	APBPConstants[BP_RUN_ENERGYCOSTFACTOR] = iniReader.ReadInteger("BPConstants","BP_RUN_ENERGYCOSTFACTOR",3);
	APBPConstants[BP_WALK_ENERGYCOSTFACTOR] = iniReader.ReadInteger("BPConstants","BP_WALK_ENERGYCOSTFACTOR",1);
	APBPConstants[BP_SWAT_ENERGYCOSTFACTOR] = iniReader.ReadInteger("BPConstants","BP_SWAT_ENERGYCOSTFACTOR",2);
	APBPConstants[BP_CRAWL_ENERGYCOSTFACTOR] = iniReader.ReadInteger("BPConstants","BP_CRAWL_ENERGYCOSTFACTOR",4);
	APBPConstants[BP_RADIO] = iniReader.ReadInteger("BPConstants","BP_RADIO",0);
	APBPConstants[BP_USE_DETONATOR] = iniReader.ReadInteger("BPConstants","BP_USE_DETONATOR",0);
	APBPConstants[BP_PER_AP_NO_EFFORT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","BP_PER_AP_NO_EFFORT",-50),-50, TRUE);
	APBPConstants[BP_PER_AP_MIN_EFFORT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","BP_PER_AP_MIN_EFFORT",-25),-25, TRUE);
	APBPConstants[BP_PER_AP_LT_EFFORT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","BP_PER_AP_LT_EFFORT",-12),-12, TRUE);
	APBPConstants[BP_PER_AP_MOD_EFFORT] = DynamicAdjustAPConstants(iniReader.ReadInteger("APConstants","BP_PER_AP_MOD_EFFORT",6),6, TRUE);
	APBPConstants[BP_MOVEMENT_FLAT] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_FLAT",5);
	APBPConstants[BP_MOVEMENT_GRASS] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_GRASS",10);
	APBPConstants[BP_MOVEMENT_BUSH] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_BUSH",20);
	APBPConstants[BP_MOVEMENT_RUBBLE] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_RUBBLE",35);
	APBPConstants[BP_MOVEMENT_SHORE] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_SHORE",50);
	APBPConstants[BP_MOVEMENT_LAKE] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_LAKE",75);
	APBPConstants[BP_MOVEMENT_OCEAN] = iniReader.ReadInteger("BPConstants","BP_MOVEMENT_OCEAN",100);
	APBPConstants[BP_CROUCH] = iniReader.ReadInteger("BPConstants","BP_CROUCH",10);
	APBPConstants[BP_PRONE] = iniReader.ReadInteger("BPConstants","BP_PRONE",10);
	APBPConstants[BP_CLIMBROOF] = iniReader.ReadInteger("BPConstants","BP_CLIMBROOF",500);
	APBPConstants[BP_CLIMBOFFROOF] = iniReader.ReadInteger("BPConstants","BP_CLIMBOFFROOF",250);
	APBPConstants[BP_JUMPFENCE] = iniReader.ReadInteger("BPConstants","BP_JUMPFENCE",200);
	APBPConstants[BP_JUMPFENCEBPACK] = iniReader.ReadInteger("BPConstants","BP_JUMPFENCEBPACK",500);
	APBPConstants[BP_STEAL_ITEM] = iniReader.ReadInteger("BPConstants","BP_STEAL_ITEM",50);
	APBPConstants[BP_START_FIRST_AID] = iniReader.ReadInteger("BPConstants","BP_START_FIRST_AID",0);
	APBPConstants[BP_GET_HIT] = iniReader.ReadInteger("BPConstants","BP_GET_HIT",200);
	APBPConstants[BP_GET_WOUNDED] = iniReader.ReadInteger("BPConstants","BP_GET_WOUNDED",50);
	APBPConstants[BP_FALL_DOWN] = iniReader.ReadInteger("BPConstants","BP_FALL_DOWN",250);
	APBPConstants[BP_OPEN_DOOR] = iniReader.ReadInteger("BPConstants","BP_OPEN_DOOR",30);
	APBPConstants[BP_PICKLOCK] = iniReader.ReadInteger("BPConstants","BP_PICKLOCK",-250);
	APBPConstants[BP_EXAMINE_DOOR] = iniReader.ReadInteger("BPConstants","BP_EXAMINE_DOOR",-250);
	APBPConstants[BP_BOOT_DOOR] = iniReader.ReadInteger("BPConstants","BP_BOOT_DOOR",200);
	APBPConstants[BP_USE_CROWBAR] = iniReader.ReadInteger("BPConstants","BP_USE_CROWBAR",350);
	APBPConstants[BP_UNLOCK_DOOR] = iniReader.ReadInteger("BPConstants","BP_UNLOCK_DOOR",50);
	APBPConstants[BP_EXPLODE_DOOR] = iniReader.ReadInteger("BPConstants","BP_EXPLODE_DOOR",-250);
	APBPConstants[BP_UNTRAP_DOOR] = iniReader.ReadInteger("BPConstants","BP_UNTRAP_DOOR",150);
	APBPConstants[BP_LOCK_DOOR] = iniReader.ReadInteger("BPConstants","BP_LOCK_DOOR",50);
	APBPConstants[BP_USEWIRECUTTERS] = iniReader.ReadInteger("BPConstants","BP_USEWIRECUTTERS",200);
	APBPConstants[BP_DROP_BOMB] = iniReader.ReadInteger("BPConstants","BP_DROP_BOMB",0);
	APBPConstants[BP_BURY_MINE] = iniReader.ReadInteger("BPConstants","BP_BURY_MINE",250);
	APBPConstants[BP_DISARM_MINE] = iniReader.ReadInteger("BPConstants","BP_DISARM_MINE",0);
	APBPConstants[BP_JUMP_OVER] = iniReader.ReadInteger("BPConstants","BP_JUMP_OVER",250);
	APBPConstants[BP_BACK_PACK] = iniReader.ReadInteger("APConstants","BP_JUMP_OVER",50);
	APBPConstants[BP_WORK_ZIPPER] = iniReader.ReadInteger("APConstants","BP_WORK_ZIPPER",250);
	APBPConstants[BP_UNJAM] = iniReader.ReadInteger("APConstants","BP_WORK_ZIPPER",0);

	SetupMaxActionPointsAnimation();
#undef ReadInteger
}

void FreeGameExternalOptions()
{
	if (gGameExternalOptions.iaIMPSlots != NULL) // OJW - 20081129 - Fix memory leak when calling LoadGameExternalOptions twice
	MemFree( gGameExternalOptions.iaIMPSlots);
}

BOOLEAN GetCDLocation( )
{
	UINT32	uiStrngLength = 0;
	CHAR8		zCdLocation[ SGPFILENAME_LEN ];
	UINT32	uiRetVal=0;

	//Do a crude check to make sure the Ja2.ini file is the right on

	uiRetVal = GetPrivateProfileString( "Ja2 Settings","CD", "", zCdLocation, SGPFILENAME_LEN, GAME_INI_FILE );
	if( uiRetVal == 0 || !IsDriveLetterACDromDrive( zCdLocation ) )
	{
		// the user most likely doesnt have the file, or the user has messed with the file
		// build a new one

		//First delete the old file
		// Snap: don't clobber the INI file!
		//FileDelete( GAME_INI_FILE );

		//Get the location of the cdrom drive
		if( GetCDromDriveLetter( zCdLocation ) )
		{
			CHAR8		*pTemp;

			//if it succeeded
			pTemp = strrchr( zCdLocation, ':' );
			pTemp[0] = '\0';
		}
		else
		{
			//put in a default location
			sprintf( zCdLocation, "c" );
		}

		//Now create a new file
		WritePrivateProfileString( "Ja2 Settings", "CD", zCdLocation, GAME_INI_FILE );

		GetPrivateProfileString( "Ja2 Settings","CD", "", zCdLocation, SGPFILENAME_LEN, GAME_INI_FILE );
	}

	uiStrngLength = strlen( zCdLocation );

	//if the string length is less the 1 character, it is a drive letter
	if( uiStrngLength == 1 )
	{
		sprintf( gzCdDirectory, "%s:\\%s", zCdLocation, CD_ROOT_DIR );
	}

	//else it is most likely a network location
	else if( uiStrngLength > 1 )
	{
		sprintf( gzCdDirectory, "%s\\%s", zCdLocation, CD_ROOT_DIR );
	}
	else
	{
		//no path was entered
		gzCdDirectory[ 0 ] = '.';
	}

	return( TRUE );
}



BOOLEAN GetCDromDriveLetter( STR8	pString )
{
	UINT32	uiSize=0;
	UINT8		ubCnt=0;
	CHAR8		zDriveLetters[512];
	CHAR8		zDriveLetter[16];
	UINT32	uiDriveType;

	uiSize = GetLogicalDriveStrings( 512, zDriveLetters );

	for( ubCnt=0;ubCnt<uiSize;ubCnt++ )
	{
		//if the current char is not null
		if( zDriveLetters[ ubCnt ] != '\0' )
		{
			//get the string
			zDriveLetter[ 0 ] = zDriveLetters[ ubCnt ];
			ubCnt++;
			zDriveLetter[ 1 ] = zDriveLetters[ ubCnt ];
			ubCnt++;
			zDriveLetter[ 2 ] = zDriveLetters[ ubCnt ];

			zDriveLetter[ 3 ] = '\0';

			//Get the drive type
			uiDriveType = GetDriveType( zDriveLetter );
			switch( uiDriveType )
			{
				// The drive is a CD-ROM drive. 
				case DRIVE_CDROM:
					strcpy( pString, zDriveLetter );

					if ( DoJA2FilesExistsOnDrive( pString ) )
					{
						return( TRUE );
					}
					break;

				default:
					break;
			}
		}
	}

	return( FALSE );
}


/*


	//Determine the type of drive the CDrom is on
	uiDriveType = GetDriveType( zCdLocation );
	switch( uiDriveType )
	{
		// The root directory does not exist. 
		case DRIVE_NO_ROOT_DIR:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The root directory does not exist." ) );
			break;


		// The disk can be removed from the drive. 
		case DRIVE_REMOVABLE:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The disk can be removed from the drive." ) );
			break;


		// The disk cannot be removed from the drive. 
		case DRIVE_FIXED:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The disk cannot be removed from the drive." ) );
			break;


		// The drive is a remote (network) drive. 
		case DRIVE_REMOTE: 
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The drive is a remote (network) drive." ) );
			break;


		// The drive is a CD-ROM drive. 
		case DRIVE_CDROM:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The drive is a CD-ROM drive." ) );
			break;


		// The drive is a RAM disk. 
		case DRIVE_RAMDISK:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The drive is a RAM disk." ) );
			break;

	
		// The drive type cannot be determined. 
		case DRIVE_UNKNOWN:
		default:
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("CDrom Info '%s': %s", zCdLocation, "The drive type cannot be determined." ) );
			break;
	}


*/

BOOLEAN CheckIfGameCdromIsInCDromDrive()
{
	CHAR8		zVolumeNameBuffer[512];
	UINT32	uiVolumeSerialNumber=0;
	UINT32	uiMaxComponentLength=0;
	UINT32	uiFileSystemFlags=0;
	CHAR8		zFileSystemNameBuffer[512];
	CHAR8		zCdLocation[ SGPFILENAME_LEN ];
	CHAR8		zCdFile[ SGPFILENAME_LEN ];

	CHAR8		zCdromRootDrive[512];
	UINT32	uiVolumeReturnValue;
	UINT32	uiLastError = ERROR_SUCCESS;

	if( !GetCdromLocationFromIniFile( zCdromRootDrive ) )
		return( FALSE );

	uiVolumeReturnValue = GetVolumeInformation( zCdromRootDrive, zVolumeNameBuffer, 512, (LPDWORD) &uiVolumeSerialNumber, (LPDWORD) &uiMaxComponentLength, (LPDWORD) &uiFileSystemFlags, zFileSystemNameBuffer, 512 );

	if( !uiVolumeReturnValue )
	{
		uiLastError = GetLastError();
	}

	// OK, build filename
	sprintf( zCdFile, "%s%s", zCdLocation, gCheckFilenames[ Random( 2 ) ] );

		//If the cdrom drive is no longer in the drive
	if( uiLastError == ERROR_NOT_READY || ( !FileExists( zCdFile ) ) )
	{
			CHAR8 sString[512];

			//if a game has been started, add the msg about saving the game to a different entry
			if( gTacticalStatus.fHasAGameBeenStarted )
			{
				sprintf( sString, "%S	%S", pMessageStrings[ MSG_INTEGRITY_WARNING ], pMessageStrings[ MSG_CDROM_SAVE_GAME ] );

				SaveGame( SAVE__ERROR_NUM, pMessageStrings[ MSG_CDROM_SAVE ] );
			}
			else
			{
				sprintf( sString, "%S", pMessageStrings[ MSG_INTEGRITY_WARNING ] );
			}

		// ATE: These are ness. due to reference counting
		// in showcursor(). I'm not about to go digging in low level stuff at this
		// point in the game development, so keep these here, as this works...
//		ShowCursor(TRUE);
//		ShowCursor(TRUE);
		ShutdownWithErrorBox( sString );

		//DoTester( );
		//MessageBox(NULL, sString, "Error", MB_OK | MB_ICONERROR	);

			return( FALSE );
	}

	return( TRUE );
}


BOOLEAN GetCdromLocationFromIniFile( STR pRootOfCdromDrive )
{
	UINT32	uiRetVal=0;

	//Do a crude check to make sure the Ja2.ini file is the right on

	uiRetVal = GetPrivateProfileString( "Ja2 Settings","CD", "", pRootOfCdromDrive, SGPFILENAME_LEN, GAME_INI_FILE );
	if( uiRetVal == 0 )
	{
		pRootOfCdromDrive[0] = '\0';
		return( FALSE);
	}
	else
	{
		//add the :\ to the dir
		strcat( pRootOfCdromDrive, ":\\" );
		return( TRUE );
	}
}


void CDromEjectionErrorMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_OK )
	{
		guiPreviousOptionScreen = GAME_SCREEN;

		//if we are in a game, save the game
		if( gTacticalStatus.fHasAGameBeenStarted )
		{
			SaveGame( SAVE__ERROR_NUM, pMessageStrings[ MSG_CDROM_SAVE ] );
		}

 		//quit the game
		gfProgramIsRunning = FALSE;
	}
}


BOOLEAN IsDriveLetterACDromDrive( STR pDriveLetter )
{
	UINT32	uiDriveType;
	CHAR8		zRootName[512];

	sprintf( zRootName, "%s:\\", pDriveLetter );

	//Get the drive type
	uiDriveType = GetDriveType( zRootName );
	switch( uiDriveType )
	{
		// The drive is a CD-ROM drive. 
#ifdef JA2BETAVERSION
		case DRIVE_NO_ROOT_DIR:
		case DRIVE_REMOTE:
#endif
		case DRIVE_CDROM:
			return( TRUE );
			break;
	}

	return( FALSE );
}

void DisplayGameSettings( )
{
	//Display the version number
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s (%S)", pMessageStrings[ MSG_VERSION ], zVersionLabel, czVersionNumber );

	//Display the difficulty level
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], gzGIOScreenText[ gGameOptions.ubDifficultyLevel + GIO_EASY_TEXT - 1 ] );

	//Bobby Ray option
	switch ( gGameOptions.ubBobbyRay )
	{
		case BR_GOOD:
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], gzGIOScreenText[ GIO_BR_GOOD_TEXT ] );
			break;
		case BR_GREAT:
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], gzGIOScreenText[ GIO_BR_GREAT_TEXT ] );
			break;
		case BR_EXCELLENT:
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], gzGIOScreenText[ GIO_BR_EXCELLENT_TEXT ] );
			break;
		case BR_AWESOME:
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], gzGIOScreenText[ GIO_BR_AWESOME_TEXT ] );
			break;
	}

		// Kaiden: Following Line was commented out (Extra Bobby Rays Setting always displays Normal.
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], gzGIOScreenText[ GIO_BR_GOOD_TEXT ] );
//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_GAME_SAVE_STYLE_TEXT ], gzGIOScreenText[ GIO_SAVE_ANYWHERE_TEXT + gGameOptions.fIronManMode ] );

	// Tons of Guns Option
	if( gGameOptions.fGunNut )
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_GUN_OPTIONS_TEXT ], gzGIOScreenText[ GIO_GUN_NUT_TEXT ] );
	else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_GUN_OPTIONS_TEXT ], gzGIOScreenText[ GIO_REDUCED_GUNS_TEXT ] );

	//Sci fi option
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_GAME_STYLE_TEXT ], gzGIOScreenText[ GIO_REALISTIC_TEXT + gGameOptions.ubGameStyle ] );

	////Timed Turns option
	//// JA2Gold: no timed turns
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_TIMED_TURN_TITLE_TEXT ], gzGIOScreenText[ GIO_NO_TIMED_TURNS_TEXT + gGameOptions.fTurnTimeLimit ] );

	if (!is_networked)
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[58], CurrentPlayerProgressPercentage(), HighestPlayerProgressPercentage() );

#ifdef _DEBUG
	// WDS - Add debug output here
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[58], CurrentPlayerProgressPercentage(), HighestPlayerProgressPercentage() );
#endif

	////if( CHEATER_CHEAT_LEVEL() )
	//{
	//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[58], CurrentPlayerProgressPercentage(), HighestPlayerProgressPercentage() );
	//}
}

BOOLEAN MeanwhileSceneSeen( UINT8 ubMeanwhile )
{
	UINT32	uiCheckFlag;

	if ( ubMeanwhile > 32 || ubMeanwhile > NUM_MEANWHILES )
	{
		return( FALSE );
	}

	uiCheckFlag = 0x1 << ubMeanwhile;

	if ( gGameSettings.uiMeanwhileScenesSeenFlags & uiCheckFlag )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN SetMeanwhileSceneSeen( UINT8 ubMeanwhile )
{
	UINT32	uiCheckFlag;

	if ( ubMeanwhile > 32 || ubMeanwhile > NUM_MEANWHILES )
	{
		// can't set such a flag!
		return( FALSE );
	}
	uiCheckFlag = 0x1 << ubMeanwhile;
	gGameSettings.uiMeanwhileScenesSeenFlags |= uiCheckFlag;
	return( TRUE );
}

BOOLEAN	CanGameBeSaved()
{
	//if the iron man mode is on
	if( gGameOptions.fIronManMode )
	{
		//if we are in turn based combat
		if( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) )
		{
			//no save for you
			return( FALSE );
		}

		//if there are enemies in the current sector
		if( gWorldSectorX != -1 && gWorldSectorY != -1 &&
				gWorldSectorX != 0 && gWorldSectorY != 0 &&
				NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) > 0 )
		{
			//no save for you
			return( FALSE );
		}

		//All checks failed, so we can save
		return( TRUE );
	}
	else
	{
		return( TRUE );
	}
}
