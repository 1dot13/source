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
#endif

#include	"Text.h"

#define				GAME_SETTINGS_FILE		"..\\Ja2.set"

#define				GAME_INI_FILE					"..\\Ja2.ini"

#define				GAME_EXTERNAL_OPTIONS_FILE	"Ja2_Options.ini"

#define				CD_ROOT_DIR						"DATA\\"

GAME_SETTINGS		gGameSettings;
GAME_OPTIONS		gGameOptions;
// Snap: Options read from an INI file in the default of custom Data directory
GAME_EXTERNAL_OPTIONS gGameExternalOptions; 

extern	SGPFILENAME	gCheckFilenames[];
extern	CHAR8		gzErrorMsg[256];

void		InitGameSettings();

BOOLEAN GetCdromLocationFromIniFile( STR pRootOfCdromDrive );

extern BOOLEAN DoJA2FilesExistsOnDrive( CHAR8 *zCdLocation );

BOOLEAN GetCDromDriveLetter( STR8	pString );
BOOLEAN	IsDriveLetterACDromDrive( STR pDriveLetter );
void		CDromEjectionErrorMessageBoxCallBack( UINT8 bExitValue );


//Change this number when we want any who gets the new build to reset the options
#define				GAME_SETTING_CURRENT_VERSION		522



BOOLEAN LoadGameSettings()
{
	HWFILE	hFile;
	UINT32	uiNumBytesRead;


	//if the game settings file does NOT exist, or if it is smaller then what it should be
	if( !FileExists( GAME_SETTINGS_FILE ) || FileSize( GAME_SETTINGS_FILE ) != sizeof( GAME_SETTINGS ) )
	{
		//Initialize the settings
		InitGameSettings();

		//delete the shade tables aswell
		DeleteShadeTableDir( );
	}
	else
	{
		hFile = FileOpen( GAME_SETTINGS_FILE, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
		if( !hFile )
		{
			FileClose( hFile );
			InitGameSettings();
			return(FALSE);
		}

		FileRead( hFile, &gGameSettings, sizeof( GAME_SETTINGS ), &uiNumBytesRead );
		if( uiNumBytesRead != sizeof( GAME_SETTINGS ) )
		{
			FileClose( hFile );
			InitGameSettings();
			return(FALSE);
		}

		FileClose( hFile );
	}


	//if the version in the game setting file is older then the we want, init the game settings
	if( gGameSettings.uiSettingsVersionNumber < GAME_SETTING_CURRENT_VERSION )
	{
		//Initialize the settings
		InitGameSettings();

		//delete the shade tables aswell
		DeleteShadeTableDir( );

		return( TRUE );
	}


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
		gGameSettings.fOptions[ TOPTION_SPEECH ]							= TRUE;
	}
		

	//
	//	Set the settings
	//

	SetSoundEffectsVolume( gGameSettings.ubSoundEffectsVolume );
	SetSpeechVolume( gGameSettings.ubSpeechVolume );
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );	

#ifndef BLOOD_N_GORE_ENABLED
	gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ]				= FALSE;
#endif

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
	HWFILE		hFile;
	UINT32	uiNumBytesWritten;



	//create the file
	hFile = FileOpen( GAME_SETTINGS_FILE, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if( !hFile )
	{
		FileClose( hFile );
		return(FALSE);
	}



	//Record the current settings into the game settins structure

	gGameSettings.ubSoundEffectsVolume = (UINT8)GetSoundEffectsVolume( );
	gGameSettings.ubSpeechVolume = (UINT8)GetSpeechVolume( );
	gGameSettings.ubMusicVolumeSetting = (UINT8)MusicGetVolume( );	

	strcpy( gGameSettings.zVersionNumber, czVersionNumber );

	gGameSettings.uiSettingsVersionNumber = GAME_SETTING_CURRENT_VERSION;

	//Write the game settings to disk
	FileWrite( hFile, &gGameSettings, sizeof( GAME_SETTINGS ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( GAME_SETTINGS ) )
	{
		FileClose( hFile );
		return(FALSE);
	}

	FileClose( hFile );

	return( TRUE );
}

void InitGameSettings()
{
	memset( &gGameSettings, 0, sizeof( GAME_SETTINGS ) );

	//Init the Game Settings
	gGameSettings.bLastSavedGameSlot		= -1;
	gGameSettings.ubMusicVolumeSetting	= 63;
	gGameSettings.ubSoundEffectsVolume	= 63;
	gGameSettings.ubSpeechVolume				= 63;

	//Set the settings
	SetSoundEffectsVolume( gGameSettings.ubSoundEffectsVolume );
	SetSpeechVolume( gGameSettings.ubSpeechVolume );
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );	

	gGameSettings.fOptions[ TOPTION_SUBTITLES ]							= TRUE;
	gGameSettings.fOptions[ TOPTION_SPEECH ]							= TRUE;
	gGameSettings.fOptions[ TOPTION_KEY_ADVANCE_SPEECH ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_RTCONFIRM ]							= FALSE;
	gGameSettings.fOptions[ TOPTION_HIDE_BULLETS ]						= FALSE;
	gGameSettings.fOptions[ TOPTION_TRACKING_MODE ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_MUTE_CONFIRMATIONS ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_DONT_MOVE_MOUSE ]					= FALSE;
	gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ]				= FALSE;
	gGameSettings.fOptions[ TOPTION_ALWAYS_SHOW_MOVEMENT_PATH ]			= FALSE;

	gGameSettings.fOptions[ TOPTION_SLEEPWAKE_NOTIFICATION ]			= TRUE;

	gGameSettings.fOptions[ TOPTION_USE_METRIC_SYSTEM ]					= FALSE;

	#ifndef BLOOD_N_GORE_ENABLED
		gGameSettings.fOptions[ TOPTION_BLOOD_N_GORE ]					= FALSE;
	#endif

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
	// JA2Gold
	gGameSettings.fOptions[ TOPTION_MERC_CASTS_LIGHT ]					= TRUE;

	//lalien
	gGameSettings.fOptions[ TOPTION_SPACE_SELECTS_NEXT_SQUAD ]			= TRUE;
	gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ]					= TRUE;
	gGameSettings.fOptions[ TOPTION_SHOW_WEAPON_RANGE_IN_TILES ]		= TRUE;
	gGameSettings.fOptions[ TOPTION_TRACERS_FOR_SINGLE_FIRE ]			= FALSE;
	gGameSettings.fOptions[ TOPTION_RAIN_SOUND ]						= TRUE;
	gGameSettings.fOptions[ TOPTION_ALLOW_CROWS ]						= TRUE;

	gGameSettings.ubSizeOfDisplayCover = 4;
	gGameSettings.ubSizeOfLOS = 4;

	//Since we just set the settings, save them
	SaveGameSettings();
}

void InitGameOptions()
{
	memset( &gGameOptions, 0, sizeof( GAME_OPTIONS ) );

	//Init the game options
	gGameOptions.ubBobbyRay			= BR_GOOD;
	gGameOptions.fGunNut			= TRUE;
	gGameOptions.fAirStrikes		= FALSE;
	gGameOptions.ubGameStyle		= STYLE_SCIFI;
	gGameOptions.ubDifficultyLevel	= DIF_LEVEL_MEDIUM;
	//gGameOptions.fTurnTimeLimit	= FALSE;
	
	gGameOptions.fIronManMode		= FALSE;
}


// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions()
{
	//Kaiden: Setting Ja2_Options.ini file to be read
	CIniReader iniReader(GAME_EXTERNAL_OPTIONS_FILE);


	//################# Laptop Settings #################

	//Character generation
	gGameExternalOptions.iMinAttribute			= iniReader.ReadInteger("JA2 Laptop Settings","MIN_ATTRIBUTE_POINT",35);
	gGameExternalOptions.iMaxAttribute			= iniReader.ReadInteger("JA2 Laptop Settings","MAX_ATTRIBUTE_POINT",90);
	gGameExternalOptions.iImpAttributePoints	= iniReader.ReadInteger("JA2 Laptop Settings","IMPATTRIBUTEPOINTS",40);
	gGameExternalOptions.iMaxZeroBonus			= iniReader.ReadInteger("JA2 Laptop Settings","MAX_ZERO_BONUS",15);
	gGameExternalOptions.iStartAttribute		= iniReader.ReadInteger("JA2 Laptop Settings","START_ATTRIBUTE",55);

	gGameExternalOptions.fPers_att				= iniReader.ReadBoolean("JA2 Laptop Settings","USE_RANDOM_PERSONALITY",FALSE);
	gGameExternalOptions.iCustomPersonality		= iniReader.ReadInteger("JA2 Laptop Settings","CUSTOM_PERSONALITY",0);
	gGameExternalOptions.iCustomAttitude		= iniReader.ReadInteger("JA2 Laptop Settings","CUSTOM_ATTITUDE",0);

	//Merc settings
	gGameExternalOptions.fMercDayOne			= iniReader.ReadBoolean("JA2 Laptop Settings","MERC_AVAILABLE_DAY_ONE",FALSE);
	gGameExternalOptions.fAllMercsAvailable		= iniReader.ReadBoolean("JA2 Laptop Settings","ALL_MERCS_AT_MERC",FALSE);

	
	//################# System Settings #################
	gGameExternalOptions.gubDeadLockDelay = iniReader.ReadInteger("JA2 System Settings","DEAD_LOCK_DELAY",15);
	gGameExternalOptions.gfEnableEmergencyButton_SkipStrategicEvents = iniReader.ReadBoolean("JA2 System Settings","ENABLE_EMERGENCY_BUTTON_NUMLOCK_TO_SKIP_STRATEGIC_EVENTS",0);

	
	//################# Video Settings #################
	gGameExternalOptions.gfVSync = iniReader.ReadBoolean("JA2 Video Settings","VERTICAL_SYNC",0);
	

	//################# Animation Settings #################
	gGameExternalOptions.gubPlayerTurnSpeedUpFactor		= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","PLAYER_TURN_SPEED_UP_FACTOR",1);
	gGameExternalOptions.gubEnemyTurnSpeedUpFactor		= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","ENEMY_TURN_SPEED_UP_FACTOR",1);
	gGameExternalOptions.gubCreatureTurnSpeedUpFactor	= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","CREATURE_TURN_SPEED_UP_FACTOR",1);
	gGameExternalOptions.gubMilitiaTurnSpeedUpFactor	= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","MILITIA_TURN_SPEED_UP_FACTOR",1);
	gGameExternalOptions.gubCivilianTurnSpeedUpFactor	= iniReader.ReadInteger("JA2 Turnbased Animation Speed Settings","CIVILIAN_TURN_SPEED_UP_FACTOR",1);

	
	//################# Sound Settings #################
	gGameExternalOptions.guiWeaponSoundEffectsVolume = iniReader.ReadInteger("JA2 Sound Settings","WEAPON_SOUND_EFFECTS_VOLUME",0);



	//################# Tactical Settings #################

	// Militia Settings	
	gGameExternalOptions.fAllowTacticalMilitiaCommand	= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_TACTICAL_MILITIA_COMMAND",0);

	// Enemy AP settings
	gGameExternalOptions.iEasyAPBonus					= iniReader.ReadInteger("JA2 Tactical Settings","NOVICE_AP_BONUS",0);
	gGameExternalOptions.iExperiencedAPBonus			= iniReader.ReadInteger("JA2 Tactical Settings","EXPERIENCED_AP_BONUS",0);
	gGameExternalOptions.iExpertAPBonus					= iniReader.ReadInteger("JA2 Tactical Settings","EXPERT_AP_BONUS",0);
	gGameExternalOptions.iInsaneAPBonus					= iniReader.ReadInteger("JA2 Tactical Settings","INSANE_AP_BONUS",0);

	// Sight range
	gGameExternalOptions.ubStraightSightRange				= iniReader.ReadInteger("JA2 Tactical Settings","BASE_SIGHT_RANGE",13);
	gGameExternalOptions.ubVisDistDecreasePerRainIntensity	= iniReader.ReadInteger("JA2 Tactical Settings","VISUAL_DISTANCE_DECREASE_PER_RAIN_INTENSITY",30);
	gGameExternalOptions.gfAllowLimitedVision				= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_LIMITED_VISION",0);

	// Maximal search distance for grenades
	//gGameExternalOptions.guiMaxTossSearchDist = iniReader.ReadInteger("JA2 Tactical Settings","MaxTossSearchDist",3);

	// Soldier tool tips
	gGameExternalOptions.gfAllowSoldierToolTips		= iniReader.ReadBoolean("JA2 Tactical Settings","ALLOW_SOLDIER_TOOL_TIPS",0);

	//################# Rain Settings ##################
	  
	// Rain settings
	gGameExternalOptions.gfAllowRain									= iniReader.ReadBoolean("JA2 Rain Settings","ALLOW_RAIN",0);
	gGameExternalOptions.gusRainChancePerDay							= iniReader.ReadInteger("JA2 Rain Settings","RAIN_CHANCE_PER_DAY",100);
	gGameExternalOptions.gusRainMinLength								= iniReader.ReadInteger("JA2 Rain Settings","RAIN_MIN_LENGTH_IN_MINUTES",60);
	gGameExternalOptions.gusRainMaxLength								= iniReader.ReadInteger("JA2 Rain Settings","RAIN_MAX_LENGTH_IN_MINUTES",300);
	gGameExternalOptions.guiMaxRainDrops								= iniReader.ReadInteger("JA2 Rain Settings","MAX_RAIN_DROPS",80);
	gGameExternalOptions.ubWeaponReliabilityReductionPerRainIntensity	= iniReader.ReadInteger("JA2 Rain Settings","WEAPON_RELIABILITY_REDUCTION_PER_RAIN_INTENSITY",0);
	gGameExternalOptions.ubBreathGainReductionPerRainIntensity			= iniReader.ReadFloat("JA2 Rain Settings","BREATH_GAIN_REDUCTION_PER_RAIN_INTENSITY",25);

	// Thunder settings
	gGameExternalOptions.guiMinLightningInterval			= iniReader.ReadInteger("JA2 Thunder Settings","MIN_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS",5);
	gGameExternalOptions.guiMaxLightningInterval			= iniReader.ReadInteger("JA2 Thunder Settings","MAX_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS",15);
	gGameExternalOptions.guiMinDLInterval					= iniReader.ReadInteger("JA2 Thunder Settings","MIN_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS",1);
	gGameExternalOptions.guiMaxDLInterval					= iniReader.ReadInteger("JA2 Thunder Settings","MAX_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS",5);
	gGameExternalOptions.guiProlongLightningIfSeenSomeone	= iniReader.ReadInteger("JA2 Thunder Settings","PROLOGNE_DELAY_IF_SEEN_SOMEONE_DURING_LIGHTNING_IN_TURNBASED_IN_SECONDS",5);
	gGameExternalOptions.guiChanceToDoLightningBetweenTurns = iniReader.ReadInteger("JA2 Thunder Settings","CHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS",35);



	//################# Gameplay Settings ##################

	//Money settings
	gGameExternalOptions.iStartingCashNovice		= iniReader.ReadInteger("JA2 Gameplay Settings", "NOVICE_CASH",45000);
	gGameExternalOptions.iStartingCashExperienced	= iniReader.ReadInteger("JA2 Gameplay Settings", "EXPERIENCED_CASH",35000);
	gGameExternalOptions.iStartingCashExpert		= iniReader.ReadInteger("JA2 Gameplay Settings", "EXPERT_CASH",30000);
	gGameExternalOptions.iStartingCashInsane		= iniReader.ReadInteger("JA2 Gameplay Settings", "INSANE_CASH",15000);

	gGameExternalOptions.fSellAll					= iniReader.ReadBoolean("JA2 Gameplay Settings","CAN_SELL_ALT_LMB",FALSE);
	gGameExternalOptions.iPriceModifier				= iniReader.ReadInteger("JA2 Gameplay Settings","PRICE_MODIFIER",10);


	//Global game events 
	gGameExternalOptions.ubGameProgressStartMadlabQuest = iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_START_MADLAB_QUEST",35);
	gGameExternalOptions.ubGameProgressMikeAvailable	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_MIKE_AVALIABLE",50);
	gGameExternalOptions.ubGameProgressIggyAvaliable	= iniReader.ReadInteger("JA2 Gameplay Settings","GAME_PROGRESS_IGGY_AVALIABLE",70);
	gGameExternalOptions.ubSendTroopsToDrassen			= iniReader.ReadBoolean("JA2 Gameplay Settings","STRATEGIC_EVENT_SEND_TROOPS_TO_DRASSEN",TRUE);
	

	//Weapons modification
	gGameExternalOptions.ubExplosivesDamageMultiplier	= iniReader.ReadInteger("JA2 Gameplay Settings","EXPLOSIVES_DAMAGE_MULTIPLIER",0);
	gGameExternalOptions.ubMeleeDamageMultiplier		= iniReader.ReadInteger("JA2 Gameplay Settings","MELEE_DAMAGE_MULTIPLIER",0);
	gGameExternalOptions.ubGunDamageMultiplier			= iniReader.ReadInteger("JA2 Gameplay Settings","GUN_DAMAGE_MULTIPLIER",0);


	//Strategic AI settings
	gGameExternalOptions.iMaxEnemyGroupSize					= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_STRATEGIC_TEAM_SIZE",20);

	gGameExternalOptions.iReinforcementPoolNovice			= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_QUEENS_POOL_OF_TROOPS",150);
	gGameExternalOptions.iReinforcementPoolExperienced		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_QUEENS_POOL_OF_TROOPS",200);
	gGameExternalOptions.iReinforcementPoolExpert			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_QUEENS_POOL_OF_TROOPS",400);
	gGameExternalOptions.iReinforcementPoolInsane			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_QUEENS_POOL_OF_TROOPS",8000);

	gGameExternalOptions.iForcePercentageNovice				= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_INITIAL_GARRISON_PERCENTAGES",70);
	gGameExternalOptions.iForcePercentageExperienced		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_INITIAL_GARRISON_PERCENTAGES",100);
	gGameExternalOptions.iForcePercentageExpert				= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_INITIAL_GARRISON_PERCENTAGES",150);
	gGameExternalOptions.iForcePercentageInsane				= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_INITIAL_GARRISON_PERCENTAGES",200);

	gGameExternalOptions.iPercentElitesBonusExpert			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_ELITE_BONUS",25);
	gGameExternalOptions.iPercentElitesBonusInsane			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_ELITE_BONUS",25);

	gGameExternalOptions.ubMinEnemyGroupSizeNovice			= iniReader.ReadInteger("JA2 Gameplay Settings","NOVICE_MIN_ENEMY_GROUP_SIZE",3);
	gGameExternalOptions.ubMinEnemyGroupSizeExperienced		= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERIENCED_MIN_ENEMY_GROUP_SIZE",4);
	gGameExternalOptions.ubMinEnemyGroupSizeExpert			= iniReader.ReadInteger("JA2 Gameplay Settings","EXPERT_MIN_ENEMY_GROUP_SIZE",6);
	gGameExternalOptions.ubMinEnemyGroupSizeInsane			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_MIN_ENEMY_GROUP_SIZE",12);

	gGameExternalOptions.ubEasyEnemyStartingAlertLevel		= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_ENEMY_STARTING_ALERT_LEVEL", 5);
	gGameExternalOptions.ubNormalEnemyStartingAlertLevel	= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_ENEMY_STARTING_ALERT_LEVEL", 20);
	gGameExternalOptions.ubHardEnemyStartingAlertLevel		= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_ENEMY_STARTING_ALERT_LEVEL", 60);
	gGameExternalOptions.ubInsaneEnemyStartingAlertLevel	= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_ENEMY_STARTING_ALERT_LEVEL", 80);

	gGameExternalOptions.ubEasyEnemyStartingAlertDecay		= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_ENEMY_STARTING_ALERT_DECAY", 75);
	gGameExternalOptions.ubNormalEnemyStartingAlertDecay	= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_ENEMY_STARTING_ALERT_DECAY", 50);
	gGameExternalOptions.ubHardEnemyStartingAlertDecay		= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_ENEMY_STARTING_ALERT_DECAY", 25);
	gGameExternalOptions.ubInsaneEnemyStartingAlertDecay	= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_ENEMY_STARTING_ALERT_DECAY", 10);

	gGameExternalOptions.ubEasyTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_TIME_EVALUATE_IN_MINUTES", 480);
	gGameExternalOptions.ubNormalTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_TIME_EVALUATE_IN_MINUTES", 360);
	gGameExternalOptions.ubHardTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_TIME_EVALUATE_IN_MINUTES", 180);
	gGameExternalOptions.ubInsaneTimeEvaluateInMinutes		= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_TIME_EVALUATE_IN_MINUTES", 90);

	gGameExternalOptions.ubEasyTimeEvaluateVariance			= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_TIME_EVALUATE_VARIANCE", 240);
	gGameExternalOptions.ubNormalTimeEvaluateVariance		= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_TIME_EVALUATE_VARIANCE", 180);
	gGameExternalOptions.ubHardTimeEvaluateVariance			= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_TIME_EVALUATE_VARIANCE", 120);
	gGameExternalOptions.ubInsaneTimeEvaluateVariance		= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_TIME_EVALUATE_VARIANCE", 60);

	gGameExternalOptions.ubEasyGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_GRACE_PERIOD_IN_HOURS", 144);
	gGameExternalOptions.ubNormalGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_GRACE_PERIOD_IN_HOURS", 96);
	gGameExternalOptions.ubHardGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_GRACE_PERIOD_IN_HOURS", 48);
	gGameExternalOptions.ubInsaneGracePeriodInHours			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_GRACE_PERIOD_IN_HOURS", 6);

	gGameExternalOptions.ubEasyPatrolGracePeriodInDays		= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_PATROL_GRACE_PERIOD_IN_DAYS", 16);
	gGameExternalOptions.ubNormalPatrolGracePeriodInDays	= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_PATROL_GRACE_PERIOD_IN_DAYS", 12);
	gGameExternalOptions.ubHardPatrolGracePeriodInDays		= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_PATROL_GRACE_PERIOD_IN_DAYS", 8);
	gGameExternalOptions.ubInsanePatrolGracePeriodInDays	= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_PATROL_GRACE_PERIOD_IN_DAYS", 2);

	gGameExternalOptions.ubEasyNumAwareBattles				= iniReader.ReadInteger("JA2 Gameplay Settings","EASY_NUM_AWARE_BATTLES", 1);
	gGameExternalOptions.ubNormalNumAwareBattles			= iniReader.ReadInteger("JA2 Gameplay Settings","NORMAL_NUM_AWARE_BATTLES", 2);
	gGameExternalOptions.ubHardNumAwareBattles				= iniReader.ReadInteger("JA2 Gameplay Settings","HARD_NUM_AWARE_BATTLES", 3);
	gGameExternalOptions.ubInsaneNumAwareBattles			= iniReader.ReadInteger("JA2 Gameplay Settings","INSANE_NUM_AWARE_BATTLES", 4);

	//New v1.13 settings
	gGameExternalOptions.gfEasyUnlimitedTroops				= iniReader.ReadBoolean("JA2 Gameplay Settings","NOVICE_UNLIMITED_POOL_OF_TROOPS",FALSE);
	gGameExternalOptions.gfNormalUnlimitedTroops			= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERIENCED_UNLIMITED_POOL_OF_TROOPS",FALSE);
	gGameExternalOptions.gfHardUnlimitedTroops				= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERT_UNLIMITED_POOL_OF_TROOPS",FALSE);
	gGameExternalOptions.gfInsaneUnlimitedTroops			= iniReader.ReadBoolean("JA2 Gameplay Settings","INSANE_UNLIMITED_POOL_OF_TROOPS",TRUE);

	gGameExternalOptions.gfEasyAggressiveQueen				= iniReader.ReadBoolean("JA2 Gameplay Settings","NOVICE_QUEEN_AGGRESSIVE",FALSE);
	gGameExternalOptions.gfNormalAggressiveQueen			= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERIENCED_QUEEN_AGGRESSIVE",FALSE);
	gGameExternalOptions.gfHardAggressiveQueen				= iniReader.ReadBoolean("JA2 Gameplay Settings","EXPERT_QUEEN_AGGRESSIVE",FALSE);
	gGameExternalOptions.gfInsaneAggressiveQueen			= iniReader.ReadBoolean("JA2 Gameplay Settings","INSANE_QUEEN_AGGRESSIVE",TRUE);


	// Militia settings
	gGameExternalOptions.gfAllowMilitiaGroups				= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_MILITIA_MOBILE_GROUPS",0);
	gGameExternalOptions.gfAllowReinforcements				= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_REINFORCEMENTS",0);
	gGameExternalOptions.gfAllowReinforcementsOnlyInCity	= iniReader.ReadBoolean("JA2 Gameplay Settings","ALLOW_REINFORCEMENTS_ONLY_IN_CITIES",0);
	gGameExternalOptions.guiBaseQueenPoolIncrement			= iniReader.ReadInteger("JA2 Gameplay Settings","QUEEN_POOL_INCREMENT_PER_DIFFICULTY_LEVEL",60);
	
	gGameExternalOptions.guiCreateEachNHours				= iniReader.ReadInteger("JA2 Gameplay Settings","CREATE_EACH_N_HOURS",24);
	gGameExternalOptions.guiDivOfOriginalMilitia			= iniReader.ReadInteger("JA2 Gameplay Settings","DIV_OF_ORIGINAL_MILITIA",4);
	gGameExternalOptions.guiMinMilitiaSquadSize				= iniReader.ReadInteger("JA2 Gameplay Settings","MINIMUM_MILITIA_SQUAD_SIZE",5);
	gGameExternalOptions.guiMaxMilitiaSquadSize				= iniReader.ReadInteger("JA2 Gameplay Settings","MAXIMUM_MILITIA_SQUAD_SIZE",20);
	gGameExternalOptions.guiMaxMilitiaSquadSizeBattle		= iniReader.ReadInteger("JA2 Gameplay Settings","MAXIMUM_MILITIA_SQUAD_SIZE",20); //lal: this have to be the same value as MAXIMUM_MILITIA_SQUAD_SIZE!! do not change!!

	gGameExternalOptions.iMaxMilitiaPerSector				= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_MILITIA_PER_SECTOR",20);
	gGameExternalOptions.iTrainingSquadSize					= iniReader.ReadInteger("JA2 Gameplay Settings","MAX_TRAINING_SQUAD_SIZE",10);
	gGameExternalOptions.iMilitiaTrainingCost				= iniReader.ReadInteger("JA2 Gameplay Settings","MILITIA_TRAINING_COST",750);
	gGameExternalOptions.iMinLoyaltyToTrain					= iniReader.ReadInteger("JA2 Gameplay Settings","MIN_LOYALTY_TO_TRAIN",20);

	//Misc
	gGameExternalOptions.fAmmoDynamicWeight			= iniReader.ReadBoolean("JA2 Gameplay Settings", "DYNAMIC_AMMO_WEIGHT", TRUE); //Pulmu

}


BOOLEAN GetCDLocation( )
{
	UINT32	uiStrngLength = 0;
	CHAR8		zCdLocation[ SGPFILENAME_LEN ];
	UINT32	uiDriveType=0;
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
	BOOLEAN	fFailed = FALSE;
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
				sprintf( sString, "%S  %S", pMessageStrings[ MSG_INTEGRITY_WARNING ], pMessageStrings[ MSG_CDROM_SAVE_GAME ] );

				SaveGame( SAVE__ERROR_NUM, pMessageStrings[ MSG_CDROM_SAVE ] );
			}
			else
			{
				sprintf( sString, "%S", pMessageStrings[ MSG_INTEGRITY_WARNING ] );
			}

      // ATE: These are ness. due to reference counting
      // in showcursor(). I'm not about to go digging in low level stuff at this
      // point in the game development, so keep these here, as this works...
      ShowCursor(TRUE);
      ShowCursor(TRUE);
      ShutdownWithErrorBox( sString );

      //DoTester( );
      //MessageBox(NULL, sString, "Error", MB_OK | MB_ICONERROR  );

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

	//Timed Turns option
	// JA2Gold: no timed turns
	//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%s: %s", gzGIOScreenText[ GIO_TIMED_TURN_TITLE_TEXT ], gzGIOScreenText[ GIO_NO_TIMED_TURNS_TEXT + gGameOptions.fTurnTimeLimit ] );

	//if( CHEATER_CHEAT_LEVEL() )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzLateLocalizedString[58], CurrentPlayerProgressPercentage(), HighestPlayerProgressPercentage() );
	}
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
