#ifndef _GAME_SETTINGS__H_
#define _GAME_SETTINGS__H_


//If you add any options, MAKE sure you add the corresponding string to the Options Screen string array
enum
{
	TOPTION_SPEECH,
	TOPTION_MUTE_CONFIRMATIONS,
	TOPTION_SUBTITLES,
	TOPTION_KEY_ADVANCE_SPEECH,
	TOPTION_ANIMATE_SMOKE,
//	TOPTION_HIDE_BULLETS,
//	TOPTION_CONFIRM_MOVE,
	TOPTION_BLOOD_N_GORE,
	TOPTION_DONT_MOVE_MOUSE,
	TOPTION_OLD_SELECTION_METHOD,
	TOPTION_ALWAYS_SHOW_MOVEMENT_PATH,


//	TOPTION_TIME_LIMIT_TURNS,			//moved to the game init screen

	TOPTION_SHOW_MISSES,

	TOPTION_RTCONFIRM,


//	TOPTION_DISPLAY_ENEMY_INDICATOR,		//Displays the number of enemies seen by the merc, ontop of their portrait
	TOPTION_SLEEPWAKE_NOTIFICATION,

	TOPTION_USE_METRIC_SYSTEM,		//If set, uses the metric system

	TOPTION_MERC_ALWAYS_LIGHT_UP,

	TOPTION_SMART_CURSOR,

	TOPTION_SNAP_CURSOR_TO_DOOR,

	TOPTION_GLOW_ITEMS,
	TOPTION_TOGGLE_TREE_TOPS,
	TOPTION_TOGGLE_WIREFRAME,
	TOPTION_3D_CURSOR,
	TOPTION_CTH_CURSOR,
	TOPTION_GL_BURST_CURSOR,
	TOPTION_DROP_ALL,
	TOPTION_GL_HIGH_ANGLE,
	TOPTION_AIM_LEVEL_RESTRICTION,
	TOPTION_SPACE_SELECTS_NEXT_SQUAD,
	NUM_GAME_OPTIONS,				//Toggle up this will be able to be Toggled by the player

	//These options will NOT be toggable by the Player
	TOPTION_MERC_CASTS_LIGHT = NUM_GAME_OPTIONS,
	TOPTION_HIDE_BULLETS,
	TOPTION_TRACKING_MODE,

	NUM_ALL_GAME_OPTIONS,
};	


typedef struct
{
	INT8				bLastSavedGameSlot;							// The last saved game number goes in here	

	UINT8				ubMusicVolumeSetting;
	UINT8				ubSoundEffectsVolume;
	UINT8				ubSpeechVolume;

	//The following are set from the status of the toggle boxes in the Options Screen
	UINT8				fOptions[ NUM_ALL_GAME_OPTIONS ];

	CHAR8				zVersionNumber[14];

	UINT32			uiSettingsVersionNumber;
	UINT32			uiMeanwhileScenesSeenFlags;

	BOOLEAN			fHideHelpInAllScreens;

	BOOLEAN				fUNUSEDPlayerFinishedTheGame; // JA2Gold: for UB compatibility
	UINT8				ubSizeOfDisplayCover;
	UINT8				ubSizeOfLOS;

	UINT8		ubFiller[17];

}	GAME_SETTINGS;



//Enums for the difficulty levels
enum
{
	DIF_LEVEL_ZERO,
	DIF_LEVEL_EASY,
	DIF_LEVEL_MEDIUM,
	DIF_LEVEL_HARD,
	DIF_LEVEL_INSANE,
	DIF_LEVEL_FIVE,
};

//Enums for the difficulty levels
enum
{
	STYLE_REALISTIC = 0, //the specific indices are there to keep compartibility wiht old saves (back when there were only 2 styles)
	STYLE_SCIFI		= 1,
	STYLE_PLATINUM	= 2,
};

enum
{
	BR_GOOD = 1, 
	BR_GREAT = 2,
	BR_EXCELLENT = 4,
	BR_AWESOME = 10,
};

typedef struct
{
	BOOLEAN fGunNut;
	BOOLEAN fAirStrikes;//Madd
	UINT8	ubGameStyle;
	UINT8	ubDifficultyLevel;
	BOOLEAN	fTurnTimeLimit;
	BOOLEAN	fIronManMode;
	UINT8	ubBobbyRay;
	UINT8	ubFiller[7];

} GAME_OPTIONS;

// Snap: Options read from an INI file in the default of custom Data directory
typedef struct
{
	INT32	iStartingCashNovice;
	INT32	iStartingCashExperienced;
	INT32	iStartingCashExpert;
	INT32	iStartingCashInsane;

	BOOLEAN fSellAll;
	INT16 iPriceModifier;

	INT32 iMinAttribute;
	INT32 iMaxAttribute;
	INT32 iImpAttributePoints;
	INT32 iMaxZeroBonus;
	INT32 iStartAttribute;

	INT32 iMaxMilitiaPerSector;
	INT32 iTrainingSquadSize;
	INT32 iMilitiaTrainingCost;
	INT32 iMinLoyaltyToTrain;

	INT32 iMaxEnemyGroupSize;

	INT32 iReinforcementPoolNovice;
	INT32 iReinforcementPoolExperienced;
	INT32 iReinforcementPoolExpert;
	INT32 iReinforcementPoolInsane;

	INT32 iForcePercentageNovice;
	INT32 iForcePercentageExperienced;
	INT32 iForcePercentageExpert;
	INT32 iForcePercentageInsane;

	INT32 iPercentElitesBonusExpert;
	INT32 iPercentElitesBonusInsane;

	UINT8 ubMinEnemyGroupSizeNovice;
	UINT8 ubMinEnemyGroupSizeExperienced;
	UINT8 ubMinEnemyGroupSizeExpert;
	UINT8 ubMinEnemyGroupSizeInsane;

	BOOLEAN fMercDayOne;
	BOOLEAN fAllMercsAvailable;

	BOOLEAN fPers_att;

	INT8 iCustomPersonality;
	INT8 iCustomAttitude;

	INT8 iEasyAPBonus;
	INT8 iExperiencedAPBonus;
	INT8 iExpertAPBonus;
	INT8 iInsaneAPBonus;


	// System settings
	UINT8 gubDeadLockDelay;
	BOOLEAN gfEnableEmergencyButton_SkipStrategicEvents;


	//Video settings	
	BOOLEAN gfVSync;

	//Animation settings
	UINT8 gubPlayerTurnSpeedUpFactor;
	UINT8 gubEnemyTurnSpeedUpFactor;
	UINT8 gubCreatureTurnSpeedUpFactor;
	UINT8 gubMilitiaTurnSpeedUpFactor;
	UINT8 gubCivilianTurnSpeedUpFactor;

	//Sound settings
	UINT32 guiWeaponSoundEffectsVolume;

	// Militia Settings	
	BOOLEAN fAllowTacticalMilitiaCommand;

	BOOLEAN gfAllowMilitiaGroups;
	BOOLEAN gfAllowReinforcements;
	BOOLEAN gfAllowReinforcementsOnlyInCity;
	UINT32	guiBaseQueenPoolIncrement;
	
	UINT32	guiCreateEachNHours;
	UINT32	guiDivOfOriginalMilitia;
	UINT32	guiMinMilitiaSquadSize;
	UINT32	guiMaxMilitiaSquadSize;
	UINT32	guiMaxMilitiaSquadSizeBattle;



	
	//Sight range
	UINT8  ubStraightSightRange;
	UINT32 ubVisDistDecreasePerRainIntensity;
	BOOLEAN gfAllowLimitedVision;

	// Rain settings
	BOOLEAN gfAllowRain;
	UINT16 gusRainChancePerDay;
	UINT16 gusRainMinLength;
	UINT16 gusRainMaxLength;
	UINT32 guiMaxRainDrops;
	UINT32 ubWeaponReliabilityReductionPerRainIntensity;
	double ubBreathGainReductionPerRainIntensity;

	// Thunder settings
	UINT32 guiMinLightningInterval;
	UINT32 guiMaxLightningInterval;
	UINT32 guiMinDLInterval;
	UINT32 guiMaxDLInterval;
	UINT32 guiProlongLightningIfSeenSomeone;
	UINT32 guiChanceToDoLightningBetweenTurns;

	//Gameplay settings
	UINT8 ubGameProgressStartMadlabQuest;
	UINT8 ubGameProgressMikeAvailable;
	UINT8 ubGameProgressIggyAvaliable;
	UINT8 ubExplosivesDamageMultiplier;
	UINT8 ubGunDamageMultiplier;
	UINT8 ubMeleeDamageMultiplier;

	UINT32 ubEasyEnemyStartingAlertLevel;
	UINT32 ubNormalEnemyStartingAlertLevel;
	UINT32 ubHardEnemyStartingAlertLevel;
	UINT32 ubInsaneEnemyStartingAlertLevel;

	UINT32 ubEasyEnemyStartingAlertDecay;
	UINT32 ubNormalEnemyStartingAlertDecay;
	UINT32 ubHardEnemyStartingAlertDecay;
	UINT32 ubInsaneEnemyStartingAlertDecay;

	UINT32 ubEasyTimeEvaluateInMinutes;
	UINT32 ubNormalTimeEvaluateInMinutes;
	UINT32 ubHardTimeEvaluateInMinutes;
	UINT32 ubInsaneTimeEvaluateInMinutes;

	UINT32 ubEasyTimeEvaluateVariance;
	UINT32 ubNormalTimeEvaluateVariance;
	UINT32 ubHardTimeEvaluateVariance;
	UINT32 ubInsaneTimeEvaluateVariance;

	UINT32 ubEasyGracePeriodInHours;
	UINT32 ubNormalGracePeriodInHours;
	UINT32 ubHardGracePeriodInHours;
	UINT32 ubInsaneGracePeriodInHours;

	UINT32 ubEasyPatrolGracePeriodInDays;
	UINT32 ubNormalPatrolGracePeriodInDays;
	UINT32 ubHardPatrolGracePeriodInDays;
	UINT32 ubInsanePatrolGracePeriodInDays;

	UINT32 ubEasyNumAwareBattles;
	UINT32 ubNormalNumAwareBattles;
	UINT32 ubHardNumAwareBattles;
	UINT32 ubInsaneNumAwareBattles;

} GAME_EXTERNAL_OPTIONS;

//This structure will contain general Ja2 settings  NOT individual game settings.
extern GAME_SETTINGS		gGameSettings;

//This structure will contain the Game options set at the beginning of the game.
extern GAME_OPTIONS		gGameOptions;

// Snap: Options read from an INI file in the default of custom Data directory
extern GAME_EXTERNAL_OPTIONS gGameExternalOptions; 

BOOLEAN	SaveGameSettings();
BOOLEAN LoadGameSettings();
// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions();

void InitGameOptions();


BOOLEAN GetCDLocation( );

void DisplayGameSettings( );

BOOLEAN MeanwhileSceneSeen( UINT8 ubMeanwhile );

BOOLEAN SetMeanwhileSceneSeen( UINT8 ubMeanwhile );

BOOLEAN	CanGameBeSaved();

#endif