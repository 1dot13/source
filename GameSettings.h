#ifndef _GAME_SETTINGS__H_
#define _GAME_SETTINGS__H_

#include "Types.h"
#include "Strategic Status.h"
#include "Morale.h"
#include "GameInitOptionsScreen.h"
#include "Campaign Types.h"
#include "environment.h"

#define				GAME_INI_FILE					"Ja2.ini"

//If you add any options, MAKE sure you add the corresponding string to the Options Screen string array.
//	 look up : zOptionsScreenHelpText , zOptionsToggleText
//Also, define its initialization and add its load/save to INI lines in : InitGameSettings() , SaveGameSettings() , LoadGameSettings()
//Also, to establish non typical display rules (options screen list) : Establish_Options_Screen_Rules()
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
//	TOPTION_DISPLAY_ENEMY_INDICATOR,	//Displays the number of enemies seen by the merc, ontop of their portrait
	TOPTION_SLEEPWAKE_NOTIFICATION,
	TOPTION_USE_METRIC_SYSTEM,			//If set, uses the metric system
	TOPTION_MERC_ALWAYS_LIGHT_UP,
	TOPTION_SMART_CURSOR,
	TOPTION_SNAP_CURSOR_TO_DOOR,
	TOPTION_GLOW_ITEMS,
	TOPTION_TOGGLE_TREE_TOPS,
	TOPTION_TOGGLE_WIREFRAME,
	TOPTION_3D_CURSOR,
	TOPTION_CTH_CURSOR,

	//Madd:
	TOPTION_GL_BURST_CURSOR,
	TOPTION_ALLOW_TAUNTS, // changed from drop all - SANDRO
	TOPTION_GL_HIGH_ANGLE,
	TOPTION_ALLOW_REAL_TIME_SNEAK, // changed from aim levels restriction - SANDRO

	//lalien
	TOPTION_SPACE_SELECTS_NEXT_SQUAD,
	TOPTION_SHOW_ITEM_SHADOW,
	TOPTION_SHOW_WEAPON_RANGE_IN_TILES,
	TOPTION_TRACERS_FOR_SINGLE_FIRE,
	TOPTION_RAIN_SOUND,
	TOPTION_ALLOW_CROWS,
	TOPTION_ALLOW_SOLDIER_TOOLTIPS,
	TOPTION_USE_AUTO_SAVE,
	TOPTION_SILENT_SKYRIDER,
	//TOPTION_LOW_CPU_USAGE,
	TOPTION_ENHANCED_DESC_BOX,

	// arynn
	TOPTION_TOGGLE_TURN_MODE,

	// HEADROCK HAM 3.6:
	TOPTION_STAT_PROGRESS_BARS,

	

	// HEADROCK HAM 4:
	TOPTION_ALT_MAP_COLOR,

	// WANNE: Moved alternate bullets graphics (tracers) to options
	TOPTION_ALTERNATE_BULLET_GRAPHICS,

	// CHRISL: HAM 4: Activate/Deactivate NCTH mode
	//TOPTION_USE_NCTH,
	//Jenilee's Merc Ranks
	TOPTION_SHOW_MERC_RANKS,
	
	// WANNE:	
	TOPTION_SHOW_TACTICAL_FACE_GEAR,
	TOPTION_SHOW_TACTICAL_FACE_ICONS,

	TOPTION_DISABLE_CURSOR_SWAP,				// Disable cursor swapping every second between talk and quick exchange
	TOPTION_QUIET_TRAINING,						//Madd: mercs don't say gained experience quote while training
	TOPTION_QUIET_REPAIRING,					//Madd: mercs don't say gained experience quote while repairing items
	TOPTION_QUIET_DOCTORING,					//Madd: mercs don't say gained experience quote while doctoring
	
	TOPTION_AUTO_FAST_FORWARD_MODE,				// automatically fast forward through AI turns

	TOPTION_ZOMBIES,							// Flugente Zombies: allow zombies	

	TOPTION_ENABLE_INVENTORY_POPUPS,			// the_bob : enable popups for picking items from sector inv

	TOPTION_SHOW_LAST_ENEMY,					//DBrot: show approximate locations for the last enemies
	TOPTION_SHOW_LBE_CONTENT,					//DBrot: toggle between the content of an lbe and its attachments
	TOPTION_INVERT_WHEEL,						//jikuja: invert mouse wheel

	// Flugente: mercenary formations
	TOPTION_MERCENARY_FORMATIONS,

	// arynn: Debug/Cheat
	TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	TOPTION_FORCE_BOBBY_RAY_SHIPMENTS,			// force all pending Bobby Ray shipments
	TOPTION_CHEAT_MODE_OPTIONS_END,
	TOPTION_DEBUG_MODE_OPTIONS_HEADER,			// an example options screen options header (pure text)        
	
	// HEADROCK HAM 4:
	TOPTION_REPORT_MISS_MARGIN,
	
	TOPTION_SHOW_RESET_ALL_OPTIONS,				// failsafe show/hide option to reset all options 
	TOPTION_RESET_ALL_OPTIONS,					// a do once and reset self option (button like effect)                                                         
	TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE,	// allow debug options that were set in debug.exe to continue in a rel.exe (debugging release can be beneficial)
	TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP,	// an example option that will show/hide other options                                                          
	TOPTION_RENDER_MOUSE_REGIONS,				// an example of a DEBUG build option                                                                           
	TOPTION_DEBUG_MODE_OPTIONS_END,				// an example options screen options divider (pure text)

	// this is THE LAST option that exists (intended for debugging the options screen, doesnt do anything, except exist)
	TOPTION_LAST_OPTION, 
	NUM_GAME_OPTIONS,							// Toggles prior to this will be able to be toggled by the player

	//These options will NOT be toggable by the Player

	// JA2Gold
	TOPTION_MERC_CASTS_LIGHT,

	TOPTION_HIDE_BULLETS,
	TOPTION_TRACKING_MODE,

	NUM_ALL_GAME_OPTIONS,
};	


typedef struct
{
	INT32				bLastSavedGameSlot;					// The last saved game number goes in here	

	UINT8				ubMusicVolumeSetting;               // Volume Setting
	UINT8				ubSoundEffectsVolume;               // Volume Setting
	UINT8				ubSpeechVolume;                     // Volume Setting

	//The following are set from the status of the toggle boxes in the Options Screen
	BOOLEAN				fOptions[ NUM_ALL_GAME_OPTIONS + 1 ];	// Toggle Options (Speech, Subtitles, Show Tree Tops, etc.. )

	UINT32				uiMeanwhileScenesSeenFlags;         // Bit Vector describing seen 'mean whiles..' (not sure why this is in Game Settings )

	BOOLEAN				fHideHelpInAllScreens;              // Controls Help "do not show help again" checkbox

#ifdef JA2UB	
	//JA25UB
	BOOLEAN			fPlayerFinishedTheGame;
#endif

	UINT8				ubSizeOfDisplayCover;               // The number of grids the player designates thru "Delete + '=' or '-'"
	UINT8				ubSizeOfLOS;                        // The number of grids the player designates thru "End    + '=' or '-'"
                     // The number of grids the player designates thru "End    + '=' or '-'"
#ifdef JA2UB
	UINT8		ubFiller[17];
#endif	
}	GAME_SETTINGS;

// CHRISL: New Enums to track inventory system
enum
{
	INVENTORY_OLD = 0,
	INVENTORY_NEW = 1,
};

enum
{
	ATTACHMENT_OLD = 0,
	ATTACHMENT_NEW = 1,
};

//Enums for game styles
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

// Added by SANDRO
enum
{
	ITEM_PROGRESS_VERY_SLOW = 0,
	ITEM_PROGRESS_SLOW,
	ITEM_PROGRESS_NORMAL,
	ITEM_PROGRESS_FAST,
	ITEM_PROGRESS_VERY_FAST,
};

// Flugente: prisoner interrogation results
enum
{
	PRISONER_INTERROGATION_DEFECT = 0,
	PRISONER_INTERROGATION_INTEL,
	PRISONER_INTERROGATION_RANSOM,

	PRISONER_INTERROGATION_MAX
};

// Flugente: ASD resource types
enum
{
	ASD_MONEY,
	ASD_FUEL,
	ASD_HELI,
	ASD_JEEP,
	ASD_TANK,

	ASD_RESOURCE_MAX
};

// exact gun types
enum
{
	NOT_GUN = 0,
	GUN_PISTOL,
	GUN_M_PISTOL,
	GUN_SMG,
	GUN_RIFLE,
	GUN_SN_RIFLE,
	GUN_AS_RIFLE,
	GUN_LMG,
	GUN_SHOTGUN,

	GUN_TYPES_MAX
};

typedef struct
{
	BOOLEAN fGunNut;
	BOOLEAN fAirStrikes;//Madd
	UINT8	ubGameStyle;
	UINT8	ubDifficultyLevel;
	BOOLEAN	fTurnTimeLimit;
	BOOLEAN	fIronManMode;
	UINT8	ubBobbyRayQuality;
	UINT8	ubBobbyRayQuantity;
	UINT8	ubInventorySystem;
	UINT8	ubAttachmentSystem;
	UINT8	ubSquadSize;
	// SANDRO - added variables
	UINT8	ubFiller1;					// Flugente: used to be UINT8 ubMaxIMPCharacters;
	BOOLEAN	fNewTraitSystem;
	UINT8	ubFiller2;					// Flugente: used to be BOOLEAN	fEnemiesDropAllItems;
	UINT8   ubProgressSpeedOfItemsChoices;

	UINT8	ubFiller3;					// Flugente: used to be BOOLEAN fInventoryCostsAP;			// ubFiller:	From 500 to 499

	UINT8	ubFiller7;					// Flugente: used to be BOOLEAN fUseNCTH;					// ubFiller:	From 499 to 498
	UINT8	ubFiller4;					// Flugente: used to be BOOLEAN fImprovedInterruptSystem;	// ubFiller:	From 498 to 497
	UINT8	ubFiller5;					// Flugente: used to be BOOLEAN fBackGround;				// ubFiller:	From 497 to 496
	UINT8	ubFiller6;					// Flugente: used to be BOOLEAN fFoodSystem;				// ubFiler:		From 496 to 495

	UINT8	ubIronManMode;				// ubFiler:		From 495 to 494
	
	// WANNE: Decrease this filler by 1, for each new UINT8 / BOOLEAN variable, so we can maintain savegame compatibility!!
	UINT8	ubFiller[494];		

} GAME_OPTIONS;

bool UsingNewInventorySystem();
bool UsingNewAttachmentSystem();
bool UsingNewCTHSystem();
BOOLEAN UsingFoodSystem();
BOOLEAN UsingBackGroundSystem();
BOOLEAN UsingImprovedInterruptSystem();
BOOLEAN UsingInventoryCostsAPSystem();

BOOLEAN IsNIVModeValid(bool checkRes = true);

// Snap: Options read from an INI file in the default of custom Data directory
typedef struct
{
	UINT32 iGameStartingTime; //Lalien: game starting time	
	UINT32 iFirstArrivalDelay; 

	BOOLEAN fSellAll;
	INT16 iPriceModifier;
		
	// SANDRO was here - some changes done
	INT32 iIMPProfileCost;
	BOOLEAN fDynamicIMPProfileCost;
	INT32 iMinAttribute;
	INT32 iMaxAttribute;
	INT32 iImpAttributePoints;
	INT32 iMaxZeroBonus;

	INT32 iIMPStartingLevelCostMultiplier;
	INT32 iBonusPointsForDisability;
	INT32 iBonusPointsPerSkillNotTaken;
	INT32 iMaxMilitiaPerSector;
	INT32 iTrainingSquadSize;
	INT32 iMilitiaTrainingCost;
	INT32 iRegularCostModifier;
	INT32 iVeteranCostModifier;
	INT32 iMinLoyaltyToTrain;

	// Flugente: militia volunteer pool
	BOOLEAN fMilitiaVolunteerPool;
	FLOAT dMilitiaVolunteerGainFactorHourly;
	FLOAT dMilitiaVolunteerGainFactorLiberation;
	FLOAT dMilitiaVolunteerMultiplierFarm;

	INT32 iMaxEnemyGroupSize;

	BOOLEAN fMercDayOne;
	BOOLEAN fAllMercsAvailable;
	
	// tais: Any AIM merc on assignment?
	UINT8 fMercsOnAssignment;
	// tais: Soldiers wear any armour
	UINT8 fSoldiersWearAnyArmour;

	INT8 iPlayerAPBonus;

	////////////////////////////////////
	// added by SANDRO
	INT16 sEnemyAdminCtHBonusPercent;
	INT16 sEnemyRegularCtHBonusPercent;
	INT16 sEnemyEliteCtHBonusPercent;

	INT8 sEnemyAdminEquipmentQualityModifier;
	INT8 sEnemyRegularEquipmentQualityModifier;
	INT8 sEnemyEliteEquipmentQualityModifier;

	INT8 sEnemyAdminDamageResistance;	
	INT8 sEnemyRegularDamageResistance;
	INT8 sEnemyEliteDamageResistance;

	BOOLEAN fAssignTraitsToEnemy;
	BOOLEAN fAssignTraitsToMilitia;
	INT8 bAssignedTraitsRarity;

	BOOLEAN fCamoRemoving;
	INT8	bCamoKitArea;	// silversurfer added this. It defines how much of the body can be painted with camo kits (usually face and hands).
	BOOLEAN fEnhancedCloseCombatSystem;

	BOOLEAN fImprovedInterruptSystem;
	UINT8 ubBasicPercentRegisterValueIIS;
	UINT8 ubPercentRegisterValuePerLevelIIS;
	UINT8 ubBasicReactionTimeLengthIIS;
	BOOLEAN fAllowCollectiveInterrupts;
	BOOLEAN fAllowInstantInterruptsOnSight;

	BOOLEAN fNoEnemyAutoReadyWeapon;
	BOOLEAN fReducedInstantDeath;
	BOOLEAN fAllNamedNpcsDecideAction;

	UINT16 usAwardSpecialExpForQuests;

	BOOLEAN fAllowWalkingWithWeaponRaised;

	// Flugente: moved this to the ini from the option screen, as players might want to change it later on
	BOOLEAN fUseNCTH;
	////////////////////////////////////

	// Kaiden: Vehicle Inventory change - Added for INI Option
	BOOLEAN fVehicleInventory;

	///////////////////////////////////////
	// changed/added these - SANDRO
	UINT8 sMercsAutoresolveOffenseBonus;
	UINT8 sMercsAutoresolveDeffenseBonus;

	// Flugente: more ambush options
	BOOLEAN fEnableChanceOfEnemyAmbushes; 
	INT8 bChanceModifierEnemyAmbushes;
	BOOLEAN fAmbushSpreadMercs;
	UINT16 usAmbushSpreadRadiusMercs;
	UINT8 uAmbushEnemyEncircle;
	UINT16 usAmbushEnemyEncircleRadius1;
	UINT16 usAmbushEnemyEncircleRadius2;

	UINT8 usSpecialNPCStronger;

	// Flugente: should kingpin's hitmen be disguised? This will make them have random clothes among other stuff
	BOOLEAN fAssassinsAreDisguised;

	// Flugente: does the queen send out assassins that mix among your militia?
	BOOLEAN fEnemyAssassins;
	UINT8	usAssassinMinimumProgress;
	UINT8	usAssassinMinimumMilitia;
	UINT32  usAssassinPropabilityModifier;
	///////////////////////////////////////

	// System settings
	UINT8 gubDeadLockDelay;
	BOOLEAN gfEnableEmergencyButton_SkipStrategicEvents;


	//Video settings	
	BOOLEAN gfVSync;

	// Flugente: zombie settings
	INT8	sZombieRiseBehaviour;
	BOOLEAN fZombieSpawnWaves;
	INT8	sZombieRiseWaveFrequency;	
	BOOLEAN fZombieCanClimb;
	BOOLEAN fZombieCanJumpWindows;
	BOOLEAN fZombieExplodingCivs;
	INT8	sEnemyZombieDamageResistance;
	INT8	sEnemyZombieBreathDamageResistance;
	BOOLEAN fZombieOnlyHeadshotsWork;
	INT8	sZombieDifficultyLevel;
	BOOLEAN fZombieRiseWithArmour;
	BOOLEAN fZombieOnlyHeadShotsPermanentlyKill;

	// Flugente: corpse settings
	UINT32	usCorpseDelayUntilRotting;
	UINT32	usCorpseDelayUntilDoneRotting;
		
	// Flugente: fortification settings
	BOOLEAN fFortificationAllowInHostileSector;

	// Flugente: can roofs collapse?
	BOOLEAN fRoofCollapse;

	// Flugente: option to print out tilesets of structure if we press 'f'
	BOOLEAN fPrintStructureTileset;

	// Flugente: food settings
	BOOLEAN fFoodSystem;
	UINT16	usFoodDigestionHourlyBaseFood;
	UINT16	usFoodDigestionHourlyBaseDrink;
	FLOAT	sFoodDigestionSleep;
	FLOAT	sFoodDigestionTravelVehicle;
	FLOAT	sFoodDigestionTravel;
	FLOAT	sFoodDigestionAssignment;
	FLOAT	sFoodDigestionOnDuty;
	FLOAT	sFoodDigestionCombat;
		
	BOOLEAN fFoodDecayInSectors;
	FLOAT	sFoodDecayModificator;
	
	BOOLEAN fFoodEatingSounds;

	// Flugente: disease settings
	BOOLEAN fDisease;
	BOOLEAN fDiseaseStrategic;
	INT32	sDiseaseWHOSubscriptionCost;
	BOOLEAN fDiseaseContaminatesItems;
	
	//Animation settings
	FLOAT giPlayerTurnSpeedUpFactor;
	FLOAT giEnemyTurnSpeedUpFactor;
	FLOAT giCreatureTurnSpeedUpFactor;
	FLOAT giMilitiaTurnSpeedUpFactor;
	FLOAT giCivilianTurnSpeedUpFactor;
	FLOAT fScrollSpeedFactor;

	//Sound settings
	UINT32 guiWeaponSoundEffectsVolume;
	UINT8  gubMaxPercentNoiseSilencedSound;

	// WDS - Option to turn off stealing
	BOOLEAN fStealingDisabled;

	// WANNE: Externalize chance of shipment package lost
	UINT32	gubChanceOfShipmentLost;

	// Militia Settings	
	BOOLEAN fAllowTacticalMilitiaCommand;
	BOOLEAN gfTrainVeteranMilitia;

	// Flugente: militia movement
	BOOLEAN fMilitiaStrategicCommand;
	BOOLEAN fMilitiaStrategicCommand_MercRequired;
	
	BOOLEAN gfAllowReinforcements;
	BOOLEAN gfAllowReinforcementsOnlyInCity;
	
	UINT32	guiTrainVeteranMilitiaDelay;

	// SANDRO - added some variables
	INT16	sGreenMilitiaAutoresolveStrength;
	INT16	sRegularMilitiaAutoresolveStrength;
	INT16	sVeteranMilitiaAutoresolveStrength;
	INT8	bGreenMilitiaAPsBonus;
	INT8	bRegularMilitiaAPsBonus;
	INT8	bVeteranMilitiaAPsBonus;
	INT16	sGreenMilitiaCtHBonusPercent;	
	INT16	sRegularMilitiaCtHBonusPercent;
	INT16	sVeteranMilitiaCtHBonusPercent;
	INT8	bGreenMilitiaDamageResistance;
	INT8	bRegularMilitiaDamageResistance;
	INT8	bVeteranMilitiaDamageResistance;
	INT8	bGreenMilitiaEquipmentQualityModifier;
	INT8	bRegularMilitiaEquipmentQualityModifier;
	INT8	bVeteranMilitiaEquipmentQualityModifier;

	// Flugente - militia equipment
	BOOLEAN	fMilitiaUseSectorInventory;
	BOOLEAN fMilitiaUseSectorInventory_Armour;
	BOOLEAN fMilitiaUseSectorInventory_Face;
	BOOLEAN fMilitiaUseSectorInventory_Melee;
	BOOLEAN fMilitiaUseSectorInventory_Gun;
	BOOLEAN fMilitiaUseSectorInventory_Ammo;
	BOOLEAN fMilitiaUseSectorInventory_GunAttachments;
	BOOLEAN fMilitiaUseSectorInventory_Grenade;
	BOOLEAN fMilitiaUseSectorInventory_Launcher;
	UINT16 usMilitiaAmmo_Min;
	UINT16 usMilitiaAmmo_Max;
	UINT16 usMilitiaAmmo_OptimalMagCount;
	BOOLEAN	fMilitiaUseSectorClassSpecificTaboos;

	// Flugente: militia resources
	BOOLEAN fMilitiaResources;
	FLOAT fMilitiaResources_ProgressFactor;

	FLOAT fMilitiaResources_ItemClassMod_Ammo_Bullet;
	FLOAT fMilitiaResources_ItemClassMod_Gun;
	FLOAT fMilitiaResources_ItemClassMod_Armour;
	FLOAT fMilitiaResources_ItemClassMod_Melee;
	FLOAT fMilitiaResources_ItemClassMod_Bomb;
	FLOAT fMilitiaResources_ItemClassMod_Grenade;
	FLOAT fMilitiaResources_ItemClassMod_Face;
	FLOAT fMilitiaResources_ItemClassMod_LBE;
	FLOAT fMilitiaResources_ItemClassMod_Attachment_Low;
	FLOAT fMilitiaResources_ItemClassMod_Attachment_Medium;
	FLOAT fMilitiaResources_ItemClassMod_Attachment_High;
	
	FLOAT fMilitiaResources_WeaponMod[GUN_TYPES_MAX];

	// Flugente - allow accessing other mercs inventory via 'stealing'
	BOOLEAN fAccessOtherMercInventories;

	// Moa - weight of filled backpack lowers our AP
	BOOLEAN	fBackPackWeightLowersAP;

	// sevenfm: enemy gun jams
	BOOLEAN fEnemyJams;

	// WDS - Improve Tony's and Devin's inventory like BR's
	// silversurfer: not used anymore, see "Tactical\XML_Merchants.cpp" for "useBRSetting"
	// BOOLEAN tonyUsesBRSetting;
	// BOOLEAN devinUsesBRSetting;

	// WDS - Smart goggle switching
	BOOLEAN smartGoggleSwitch;
	
	// WDS - Automatically flag mines
	BOOLEAN automaticallyFlagMines;

	// Flugente: display an animation on any active timed bomb
	BOOLEAN fTimeBombWarnAnimations;

	// WDS - Automatically try to save when an assertion failure occurs
	BOOLEAN autoSaveOnAssertionFailure;
	UINT32  autoSaveTime;

	//JMich
	UINT16 guiMaxWeaponSize;
	UINT16 guiMaxItemSize;
	UINT16 guiOIVSizeNumber;

	// Flugente: this change allows to target head/torso/legs of prone targets. This option will be removed once accepted
	BOOLEAN fAllowTargetHeadAndLegIfProne;

	//Sight range
	UINT32	ubStraightSightRange;

	INT8 ubBrightnessVisionMod[16];

	BOOLEAN gfAllowLimitedVision;
	UINT8 usLowerVisionWhileRunning;

	BOOLEAN gfShiftFUnloadWeapons;
	BOOLEAN gfShiftFRemoveAttachments;

	// Rain settings
	BOOLEAN gfAllowRain;
	UINT16 gusWeatherPerDayRain;
	UINT32 gusRainChancePerDay;
	UINT32 gusRainMinLength;
	UINT32 gusRainMaxLength;
	UINT32 guiMaxRainDrops;
		
	// Thunder settings
	BOOLEAN gfAllowLightning;
	UINT32 guiMinLightningInterval;
	UINT32 guiMaxLightningInterval;
	UINT32 guiMinDLInterval;
	UINT32 guiMaxDLInterval;
	UINT32 guiProlongLightningIfSeenSomeone;
	UINT32 guiChanceToDoLightningBetweenTurns;

	// Sandstorm settings
	BOOLEAN gfAllowSandStorms;
	UINT16 gusWeatherPerDaySandstorm;
	UINT32 gusSandStormsChancePerDay;
	UINT32 gusSandStormsMinLength;
	UINT32 gusSandStormsMaxLength;

	// Snow settings
	BOOLEAN gfAllowSnow;
	UINT16 gusWeatherPerDaySnow;
	UINT32 gusSnowChancePerDay;
	UINT32 gusSnowMinLength;
	UINT32 gusSnowMaxLength;

	// weather penalties
	UINT32 ubWeaponReliabilityReduction[WEATHER_FORECAST_MAX];
	FLOAT dBreathGainReduction[WEATHER_FORECAST_MAX];
	FLOAT dVisDistDecrease[WEATHER_FORECAST_MAX];
	FLOAT dHearingReduction[WEATHER_FORECAST_MAX];

	// Flugente: this controls what dangers we face in different sectors
	// Snakes can attack mercs in water
	BOOLEAN gfAllowSnakes;

	// WDS - Progress settings
	UINT32 ubGameProgressPortionKills;
	UINT32 ubGameProgressPortionControl;
	UINT32 ubGameProgressPortionIncome;
	UINT32 ubGameProgressPortionVisited;
	UINT32 ubGameProgressMinimum;
	INT32 bGameProgressModifier;

	// Event settings
	UINT32 ubGameProgressStartMadlabQuest;
	UINT32 ubGameProgressMikeAvailable;
	UINT32 ubGameProgressIggyAvaliable;
	BOOLEAN ubSendTroopsToDrassen;

	// Flugente: new counterattacks and other new AI tactics
	UINT8  ubAgressiveStrategicAI;
	UINT32 ubGameProgressOffensiveStage1;
	UINT32 ubGameProgressOffensiveStage2;

	// WDS - make number of mercenaries, etc. be configurable
	// group sizes
	UINT32 ubGameMaximumNumberOfPlayerMercs;
	UINT32 ubGameMaximumNumberOfPlayerVehicles;
	UINT32 ubGameMaximumNumberOfEnemies;
	UINT32 ubGameMaximumNumberOfCreatures;
	UINT32 ubGameMaximumNumberOfRebels;
	UINT32 ubGameMaximumNumberOfCivilians;

	//Gameplay settings

	INT32 iExplosivesDamageModifier;
	INT32 iGunDamageModifier;
	INT32 iGunRangeModifier;
	INT32 iMeleeDamageModifier;

	// WDS - New AI
	BOOLEAN useNewAI;

	BOOLEAN gfInvestigateSector;
	BOOLEAN gfReassignPendingReinforcements;

	// Flugente: ASD
	BOOLEAN fASDActive;

	INT32 gASDResource_Cost[ASD_RESOURCE_MAX];
	INT32 gASDResource_BuyTime[ASD_RESOURCE_MAX];

	BOOLEAN fASDAssignsTanks;
	BOOLEAN fASDAssignsJeeps;

	INT32 gASDResource_Fuel_Tank;
	INT32 gASDResource_Fuel_Jeep;

	// Flugente: enemy heli
	BOOLEAN fEnemyHeliActive;
	UINT8 usEnemyHeliMinimumProgress;
	
	UINT8 gEnemyHeliMaxHP;
	UINT16 gEnemyHeliTimePerHPRepair;
	INT32  gEnemyHeliCostPerRepair1HP;
	UINT16 gEnemyHeliMaxFuel;
	UINT16 gEnemyHeliTimePerFuelRefuel;

	UINT8 gEnemyHeliAllowedSAMContacts;
	UINT16 gEnemyHeliSAMDamage_Base;
	UINT16 gEnemyHeliSAMDamage_Var;
	UINT16 gEnemyHeliMANPADSDamage_Base;
	UINT16 gEnemyHeliMANPADSDamage_Var;

	// Flugente: raids
	BOOLEAN gRaid_Bloodcats;
	BOOLEAN gRaid_Zombies;
	BOOLEAN gRaid_Bandits;

	UINT8 gRaidReplenish_BaseValue;

	UINT16 gRaidMaxSize_Bloodcats;
	UINT16 gRaidMaxSize_Zombies;
	UINT16 gRaidMaxSize_Bandits;

	UINT8 gRaid_MaxAttackPerNight_Bloodcats;
	UINT8 gRaid_MaxAttackPerNight_Zombies;
	UINT8 gRaid_MaxAttackPerNight_Bandits;
	
	INT32 ubEnemiesItemDrop;

	BOOLEAN	gfUseExternalLoadscreens;
	BOOLEAN gfUseLoadScreenHints;		// added by Flugente
	UINT32	ubAdditionalDelayUntilLoadScreenDisposal;		// added by WANNE to have time to read the load screen hints

	//tais: nsgi
	BOOLEAN gfUseNewStartingGearInterface;
	// DBrot: Seperate Starting items for Experts
	BOOLEAN fExpertsGetDifferentChoices;
	BOOLEAN gfUseAutoSave;

	//Merc Assignment settings
	INT32 ubAssignmentUnitsPerDay;
	INT32 ubMinutesForAssignmentToCount;
	BOOLEAN fSleepDisplayFailNotification;

	FLOAT fAdministrationPointsPerPercent;
	UINT16 fAdministrationMaxPercent;

	INT32 ubTrainingSkillMin;
	INT32 ubTrainingSkillMax;
	INT32 ubSelfTrainingDivisor;
	INT32 ubInstructedTrainingDivisor;
	// HEADROCK HAM 3.5: No longer necessary.
	//INT32 ubGunRangeTrainingBonus;
	INT32 ubTownMilitiaTrainingRate;

	BOOLEAN gfMilitiaTrainingCarryOver;	// added by Flugente

	// HEADROCK HAM 3.5: No longer necessary.
	//INT32 ubMaxMilitiaTrainersPerSector;
	INT32 ubTeachBonusToTrain;
	INT32 ubRpcBonusToTrainMilitia;
	INT32 ubMinSkillToTeach;

	// Flugente: disable evolution setting in MercProfiles.xml
	BOOLEAN bDisableEvolution;

	INT32 ubLowActivityLevel;
	INT32 ubMediumActivityLevel;
	INT32 ubHighActivityLevel;
	INT32 ubDoctoringRateDivisor;
	INT32 ubHospitalHealingRate;
	INT32 ubBaseMedicalSkillToDealWithEmergency;
	INT32 ubMultiplierForDifferenceInLifeValueForEmergency;
	INT32 ubPointCostPerHealthBelowOkLife;//OKLIFE = 15
	BOOLEAN fAllowBandagingDuringTravel;

	INT32 ubRepairCostPerJam;
	INT32 ubRepairRateDivisor;
	INT32 ubCleaningRateDivisor;

	// Flugente: intel
	BOOLEAN fIntelResource;

	//Misc settings
	BOOLEAN fAmmoDynamicWeight; //Pulmu
	BOOLEAN fEnableCrepitus;
	BOOLEAN fCrepitusAttackAllTowns;
	BOOLEAN fEnableAllWeaponCaches;
	BOOLEAN fEnableAllTerrorists;
	BOOLEAN gfRevealItems;
	BOOLEAN fEnableArmorCoverage; // ShadoWarrior for Captain J's armor coverage

	// ShadoWarrior: Tooltip changes (start)
	UINT8 ubSoldierTooltipDetailLevel;

	BOOLEAN fEnableDynamicSoldierTooltips;
	BOOLEAN fEnableSoldierTooltipLocation;
	BOOLEAN fEnableSoldierTooltipBrightness;
	BOOLEAN fEnableSoldierTooltipRangeToTarget;
	BOOLEAN fEnableSoldierTooltipID;
	BOOLEAN fEnableSoldierTooltipOrders;
	BOOLEAN fEnableSoldierTooltipAttitude;
	BOOLEAN fEnableSoldierTooltipActionPoints;
	BOOLEAN fEnableSoldierTooltipHealth;
	BOOLEAN fEnableSoldierTooltipEnergy;
	BOOLEAN fEnableSoldierTooltipMorale;
	BOOLEAN fEnableSoldierTooltipShock;	//Moa: debug tooltip only
	BOOLEAN fEnableSoldierTooltipSuppressionPoints; //Moa: debug tooltip only
	BOOLEAN fEnableSoldierTooltipSuppressionInfo; //sevenfm: additional suppression info
	BOOLEAN fEnableSoldierTooltipTraits; // added by SANDRO
	BOOLEAN fEnableSoldierTooltipHelmet;
	BOOLEAN fEnableSoldierTooltipVest;
	BOOLEAN fEnableSoldierTooltipLeggings;
	BOOLEAN fEnableSoldierTooltipHeadItem1;
	BOOLEAN fEnableSoldierTooltipHeadItem2;
	BOOLEAN fEnableSoldierTooltipWeapon;
	BOOLEAN fEnableSoldierTooltipSecondHand;
	BOOLEAN fEnableSoldierTooltipBigSlot1;
	BOOLEAN fEnableSoldierTooltipBigSlot2;
	BOOLEAN fEnableSoldierTooltipBigSlot3;
	BOOLEAN fEnableSoldierTooltipBigSlot4;
	BOOLEAN fEnableSoldierTooltipBigSlot5;
	BOOLEAN fEnableSoldierTooltipBigSlot6;
	BOOLEAN fEnableSoldierTooltipBigSlot7;
	// ShadoWarrior: Tooltip changes (end)
	//SCORE: UDT related
	BOOLEAN gfAllowUDTRange;
	BOOLEAN gfAllowUDTDetail;
	INT8	ubUDTModifier;

	//Kaiden MERC Deaths Externalized:
	BOOLEAN gfMercsDieOnAssignment;

	// Lesh: slow enemy items choice progress
	BOOLEAN fSlowProgressForEnemyItemsChoice;

	//afp - use bullet tracers
	BOOLEAN gbBulletTracer;

	// CHRISL: option to allow Slay to remain as a hired PC
	BOOLEAN fEnableSlayForever;
	// Bob: % chance Slay will leave every hour if left alone in sector
	UINT16  ubHourlyChanceSlayWillLeave;

	// anv: playable Speck
	BOOLEAN fEnableRecruitableSpeck;
	// anv: John Kulba becomes recruitable as a merc after finishing escort quest
	BOOLEAN fEnableRecruitableJohnKulba;
	UINT16  ubRecruitableJohnKulbaDelay;
	// anv: enable JA1 natives as MERC mercs
	BOOLEAN fEnableRecruitableJA1Natives;
	UINT32	usMERCBankruptWarning;	// Flugente: if outstanding debt exceeds this, Speck will complain about player's outstanding debts

	// CHRISL: Setting to turn off the description and stack popup options from the sector inventory panel
	BOOLEAN fSectorDesc;

	// WANNE: Restrict female enemies from beeing in the queens army (except Female Elite enemies)?
	BOOLEAN fRestrictFemaleEnemiesExceptElite;

	// HEADROCK: Enhanced Item Description Box ON/OFF
	// WANNE: Changed from BOOLEAN to INT32!
	INT32 iEnhancedDescriptionBox;
	
	//WarmSteel - New Attachment System?	
	BOOLEAN fUseDefaultSlots;
	UINT32	usAttachmentDropRate;
	INT16   iMaxEnemyAttachments;

	// Flugente: class specific gun/item choice
	BOOLEAN	fSoldierClassSpecificItemTables;

	//** ddd
	//enable ext mouse key
	BOOLEAN fExtMouseKeyEnabled;
	// sevenfm: new mouse commands
	BOOLEAN bAlternateMouseCommands;
	INT32 iQuickItem1;
	INT32 iQuickItem2;
	INT32 iQuickItem3;
	INT32 iQuickItem4;
	INT32 iQuickItem5;
	INT32 iQuickItem6;
	INT32 iQuickItem7;
	INT32 iQuickItem8;
	INT32 iQuickItem9;
	INT32 iQuickItem0;

	// for small progress bar
	BOOLEAN fSmallSizeProgressbar;
	// anv: hide stuff on roof in explored rooms at ground level view (sandbags and other crap)
	BOOLEAN fHideExploredRoomRoofStructures;
	//enable ext mouse key
	BOOLEAN bAltAimEnabled;	
	BOOLEAN bAimedBurstEnabled;
	INT16 uAimedBurstPenalty;
	BOOLEAN	bWeSeeWhatMilitiaSeesAndViceVersa;
	BOOLEAN	bAllowWearSuppressor;
	BOOLEAN	bLazyCivilians;
	BOOLEAN	bNeutralCiviliansAvoidPlayerMines;		//sevenfm:  Neutral civilians can detect mines with MAPELEMENT_PLAYER_MINE_PRESENT flag set
	BOOLEAN bExtraCivilians;						// Flugente: add civilians via LUA
	BOOLEAN bExtraMerchants;						// Flugente: add non-profile-based merchants via LUA
	BOOLEAN	bAddSmokeAfterExplosion;
	BOOLEAN	bAllowExplosiveAttachments;
	BOOLEAN	bAllowSpecialExplosiveAttachments;
	BOOLEAN fDelayedGrenadeExplosion;
	INT16 iChanceSayAnnoyingPhrase;
	BOOLEAN	bNewTacticalAIBehavior;
	BOOLEAN fAIYellowFlanking;
	BOOLEAN fAIExtraSuppression;
	BOOLEAN fAINewMorale;
	BOOLEAN fAIBetterCover;
	FLOAT uShotHeadPenalty;
	FLOAT fShotHeadMultiplier;
	INT16 iPenaltyShootUnSeen;
	BOOLEAN fNoStandingAnimAdjustInCombat;	// Flugente: in turnbased combat, do not adjust animation after arriving at target location

	BOOLEAN fInventoryCostsAP;				// manipulating the inventory in tactical costs AP

	//Inventory AP Weight Divisor
	FLOAT uWeightDivisor;

	FLOAT fOutOfGunRangeOrSight;

	// anv: automatically return to team panel on turn end (better situation overview during enemy turn)
	BOOLEAN fAutoCollapseInventoryOnTurnEnd;

	// anv: vehicle interface options
	BOOLEAN fAddPassengerToAnySquad;
	BOOLEAN fPassengerLeavingSwitchToNewSquad;

	// anv: tanks can move!
	BOOLEAN fEnemyTanksCanMoveInTactical;
	BOOLEAN fEnemyTanksDontSpareShells;
	BOOLEAN fEnemyTanksBlowObstaclesUp;
	BOOLEAN fEnemyTanksAnyPartVisible;

	BOOLEAN fEnemiesDontSpareLaunchables;
	BOOLEAN fEnemiesBlowObstaclesUp;

	BOOLEAN fArmyUsesTanksInAttacks;
	BOOLEAN fArmyUsesTanksInPatrols;
	UINT8 usTankMinimumProgress;

	BOOLEAN fArmyUsesJeepsInAttacks;
	BOOLEAN fArmyUsesJeepsInPatrols;
	UINT8 usJeepMinimumProgress;

	// WANNE: Always use "prof.dat".
	BOOLEAN fUseDifficultyBasedProfDat;

	// 2Points: Use new repair algorithm
	BOOLEAN fAdditionalRepairMode;

	// CHRISL: Skyrider and enemy occupied sectors
	UINT8 ubSkyriderHotLZ;

	// Laptop mouse capturing
	BOOLEAN fLaptopMouseCaptured;

	// WANNE: Fast loading settings
	BOOLEAN fDisableLaptopTransition;
	BOOLEAN fFastWWWSitesLoading;
	BOOLEAN fDisableStrategicTransition;

	// Flugente: campaign history
	BOOLEAN	fCampaignHistoryWebSite;	// if set to FALSE, the website won't be displayed, and thus the data cannot be seen
	INT16	usReportsToLoad;			// number of reports to read when loading a game, older reports will be lost. This is irreversible. -1: read all (default)

	// CPT: Cover System Settings
	UINT8 ubStealthTraitCoverValue;
	UINT8 ubStealthEffectiveness;
	UINT8 ubTreeCoverEffectiveness;
	UINT8 ubCamouflageEffectiveness;
	UINT8 ubStanceEffectiveness;
	UINT8 ubLBEEffectiveness;
	UINT8 ubMovementEffectiveness;
	UINT16 ubCoverDisplayUpdateWait;//Moa: changed to UINT16 to allow values 500ms to 10000ms, see ini file and DisplayCover( BOOLEAN ), DisplayMines( BOOLEAN )
	
	// anv: additional tile properties
	BOOLEAN fAdditionalTileProperties;
	// static shadows decrease brightness
	BOOLEAN fStaticShadowsDecreaseBrightness;
	// camo affinities will be used as caps not multipliers (suggested by mmm) - it will be possible to achieve perfect camo on mixed terrain using mixed camo types
	BOOLEAN fAlternateMultiTerrainCamoCalculation;
	// cover tooltip ("f") will display detailed properties and their values
	BOOLEAN fCoverTooltipDetailedTileProperties;

	// HEADROCK HAM B1: Global modifier for mine income (0 is automatically turned to 1 to avoid divide-by-zero.)
	INT16 usMineIncomePercentage;

	// HEADROCK HAM B1: Allows changing the maximum CTH possible, for any attack 0-100
	UINT8 ubMaximumCTH;
	
	// HEADROCK HAM B1: Allows changing the minimum CTH possible, for any attack 0-100
	UINT8 ubMinimumCTH;

	// HEADROCK HAM B1: Allows reducing the chance of scoring a hit fractionally, if CTH = Minimum CTH
	UINT16 usMinimumCTHDivisor;
	
	// HEADROCK HAM B2.1: This controls how effective suppression is, by increasing the number of ubSuppressionPoints accumulated by combatants (percentage);
	INT16 sSuppressionEffectiveness;

	UINT8 ubSuppressionToleranceMax;
	UINT8 ubSuppressionToleranceMin;

	// Flugente: suppression modifiers for player and other teams
	UINT16 usSuppressionEffectivenessPlayer;
	UINT16 usSuppressionEffectivenessAI;

	// HEADROCK HAM B2: Suppression Shock effectiveness (percentage, 100 = "normal", 0 = deactivated. Range 0-65535)
	UINT16 usSuppressionShockEffect;

	// HEADROCK HAM B2.1: CTH penalty given by a "Cowering" target to any enemy shooter.
	UINT8 ubAimPenaltyPerTargetShock;

	// HEADROCK HAM B2.3: It is now possible for "cowering" to increase a target's suppression.
	UINT8 ubCowerEffectOnSuppression;

	// HEADROCK HAM B2.5: Turn on Realistic Tracers. 0 = off (regular tracers). 1 = Fully realistic tracers. 2 = Tracer Bump + 1.13 Autofire Penalty Reduction
	UINT8 ubRealisticTracers;

	// HEADROCK HAM B2.5: Realistic tracers - one of every X bullets in a tracer magazines will be a tracer bullet. 0 = off (JA2 normal)
	UINT8 ubNumBulletsPerTracer;

	// HEADROCK HAM B2.5: Realistic tracers - CTH increased by this amount whenever a tracer is fired. 0 = off.
	UINT8 ubCTHBumpPerTracer;

	// CHRISL: Exeternalize the minimum range at which tracers can improve autofire hit chance
	UINT16 ubMinRangeTracerEffect;

	// HEADROCK HAM B2.6: Increased aiming costs?
	BOOLEAN fIncreasedAimingCost;

	// added by SANDRO
	BOOLEAN fAimLevelRestriction;

	// HEADROCK HAM B2.6: Dynamically determine Max-Aiming based also on weapon, bipod, etc?
	BOOLEAN fDynamicAimingTime;

	// allow old behaviour
	BOOLEAN fAimLevelsDependOnDistance; // feel free to dump this option
	INT32 iAimLevelsCompatibilityOption; // but not this one

	//WarmSteel - These determine in which group each scope belongs. Needed for dynamic aiming limits.
	INT16 sVeryHighPowerScope;
	INT16 sHighPowerScope;
	INT16 sMediumPowerScope;

	//CHRISL: AI Sniper fields
	BOOLEAN fAISniperElite;
	UINT16 fAISniperRange;
	UINT16 fAISniperChance;
	UINT16 fAISniperChanceWithSR;

	// HEADROCK HAM B2.6: Controls how much effect target movement has on aiming
	FLOAT iMovementEffectOnAiming;

	// HEADROCK HAM B2.6: Autofire Bullets/5AP modifier
	INT8 bAutofireBulletsPer5APModifier;

	// HEADROCK HAM B2.6/B1: Adjustable "luck" factor in Auto-Resolve
	FLOAT iAutoResolveLuckFactor;

	// HEADROCK HAM B2.6: Adjustable maximum for Suppression Shock effect. This has the added effect of reducing overall susceptibility to shock and may render some enemies unshockable.
	UINT8 ubMaxSuppressionShock;

	// HEADROCK HAM B2.6/2/1: Toggle new Burst/Auto CTH bars: 0=neither, 1=both, 2=Burst, 3=Auto
	UINT8 ubNewCTHBars;

	// sevenfm: default autofire bullets
	UINT8 ubSetDefaultAutofireBulletsSMG;
	UINT8 ubSetDefaultAutofireBulletsAR;
	UINT8 ubSetDefaultAutofireBulletsMG;

	// CHRISL: Changed from a simple flag to two externalized values for more modder control over AI suppression
	UINT16 ubAISuppressionMinimumMagSize;
	UINT16 ubAISuppressionMinimumAmmo;

	// HEADROCK HAM B2.7: Change the speed of skill progression. (defaults set to JA2 normal)
	UINT16 usHealthSubpointsToImprove;
	UINT16 usStrengthSubpointsToImprove;
	UINT16 usDexteritySubpointsToImprove;
	UINT16 usAgilitySubpointsToImprove;
	UINT16 usWisdomSubpointsToImprove;
	UINT16 usMarksmanshipSubpointsToImprove;
	UINT16 usMedicalSubpointsToImprove;
	UINT16 usMechanicalSubpointsToImprove;
	UINT16 usExplosivesSubpointsToImprove;
	UINT16 usLeadershipSubpointsToImprove;
	UINT16 usLevelSubpointsToImprove;

	// HEADROCK HAM B2.7: When turned on, this will give a CTH approximation instead of an exact value, on CTH Bars and "F" key feedback.
	BOOLEAN fApproximateCTH;

	// HEADROCK HAM B2.8: These are new cowering penalty divisors that help us determine how effective cowering is in different stances and when the shooter is targetting different bodyparts
	UINT8 ubCoweringPenaltyDivisorProne;
	UINT8 ubCoweringPenaltyDivisorCrouchedHead;
	UINT8 ubCoweringPenaltyDivisorCrouchedTorso;
	UINT8 ubCoweringPenaltyDivisorCrouchedLegs;

	// HEADROCK HAM B2.8: This is the maximum range at which a target gives out the full CTH penalty for cowering. At lower range, it'll give proportionally less penalty.
	UINT16 usMinRangeForFullCoweringPenalty;

	// HEADROCK HAM B2.8: Absolute maximum CTH penalty from target/shooter cowering
	UINT16 usMaxShooterCoweringPenalty;
	UINT16 usMaxTargetCoweringPenalty;

	// Flugente: drop all is now set in the ini instead of the starting screen
	UINT8 fEnemiesDropAllItems;

	// HEADROCK HAM B2.8: If this is turned on, Militia will drop their equipment similar to enemies, IF killed by non-player character.
	UINT8 ubMilitiaDropEquipment;

	// HEADROCK HAM B2.8: New Trainer Relations: 2 = Trainees will go to sleep when their trainer goes to sleep. 3 = Trainer will go to sleep if all trainees are asleep. 1 = Both. 0 = Neither.
	UINT8 ubSmartTrainingSleep;

	// HEADROCK HAM B2.8: New Trainer Relations: 2 = Trainees will wake up when their trainer wakes up. 3 = Trainer will wake up if all trainees wake up. 1 = Both. 0 = Neither.
	UINT8 ubSmartTrainingWake;

	// HEADROCK HAM B2.8: New Trainer Relations: 2 = Trainers will rest if no trainees available. 3 = Trainees will rest if no trainers available (not recommended). 1 = Both. 0 = Neither.
	UINT8 ubSmartTrainingRest;

	// HEADROCK HAM 3: Externalized ratio between Weight and Strength. This value determines how many strength points we need to lift 0.5kg with no encumberance.
	FLOAT iStrengthToLiftHalfKilo;

	// HEADROCK HAM 3: Define minimum leadership required for training militia. Set to 0 for "no limit".
	UINT8 ubMinimumLeadershipToTrainMilitia;

	// HEADROCK HAM 3: Define effect of "TEACHER" trait in increasing effective leadership, for purposes of eligibility for training militia
	UINT16 usTeacherTraitEffectOnLeadership;

	// HEADROCK HAM 3: If enabled, the trainer's "effective" leadership skill determines HOW MANY militia he/she creates per session (only works in town militia training).
	BOOLEAN fLeadershipAffectsMilitiaQuantity;

	// HEADROCK HAM 3: If "LEADERSHIP_AFFECTS_MILITIA_QUANTITY" is true, this value determines the lowest leadership required to train a full (default size 10) squad of town militia in one training session.
	UINT8 ubReqLeadershipForFullTraining;

	// HEADROCK HAM 3: If enabled, changes the way the game calculates progress. It will look at each progress control separately (Kills/Income/Control/Visited) and set the progress to the HIGHEST scoring control.
	BOOLEAN fAlternateProgressCalculation;

	// HEADROCK HAM 3: If enabled, tooltipping over Bobby Ray's weapons will show a list of possible attachments to those weapons.
	BOOLEAN fBobbyRayTooltipsShowAttachments;

	//JMich Externalized gGameExternalOptions.ubBobbyRayMaxPurchaseAmount for BobbyRay
	UINT8 ubBobbyRayMaxPurchaseAmount;

	// CHRISL: Converts the AutoFireToHitBonus value to a percentage for CTH calculations
	UINT8 ubFlatAFTHBtoPrecentMultiplier;

	// HEADROCK HAM 3.1: Divisor for the AP-to-Ready cost charge on first aiming click, when extra aiming costs are enabled. 0 = No ready-time-based charge.
	UINT8 ubFirstAimReadyCostDivisor;

	// HEADROCK HAM 3.1: This is a suppression tool that not everyone will like. It gives an on-screen message when any character has been suppressed so much he's lost his next turn completely!
	BOOLEAN fShowSuppressionShutdown;

	// HEADROCK HAM 3.1: Select which mine will run out. -1 = Random selection. 0 = no mine. 1 = San Mona (unused), 2 = Drassen, 3 = Alma, 4 = Cambria, 5 = Chitzena, 6 = Grumm.
	INT8 bWhichMineRunsOut;

	// HEADROCK HAM 3.1: Can the Humvee go off-road?
	BOOLEAN fHumveeOffroad;

	// HEADROCK HAM 3.2: If activated, reinforcements (militia/enemy) arrive in the battle with 0 APs. This makes them less of a diablo-ex-machina.
	UINT8 ubReinforcementsFirstTurnFreeze;

	// HEADROCK HAM 3.2: This feature allows the status, leadership and experience of nearby friendlies help/hinder a character's tolerance, based on their distance from him.
	BOOLEAN fFriendliesAffectTolerance;

	// HEADROCK HAM 3.2: Set a divisor for the CtH of Mortar weapons.
	UINT8 ubMortarCTHDivisor;

	// HEADROCK HAM 3.2: This enabled reduced sight for cowering characters. 0 = disabled. 2 = Reduced Sightrange. 3 = Tunnel-vision. 1 = Both.
	UINT8 ubCoweringReducesSightRange;

	// HEADROCK HAM 3.2: Critical Headshots may cause blindness. Rolls 1 to X change of being blinded. 0 = disabled.
	UINT8 ubChanceBlindedByHeadshot;

	// HEADROCK HAM 3.2: Critical Legshots cause additional AP loss
	BOOLEAN fCriticalLegshotCausesAPLoss;

	// HEADROCK HAM 3.2: When enabled, this setting removes the player's omniscience inside his own sectors. The player will no longer see the movement of enemy groups unless someone is nearby to scout them.
	BOOLEAN fNoEnemyDetectionWithoutRecon;

	// HEADROCK HAM 3.2: Determines the training bonus for the Medical skill, when training in "hospital" sectors
	UINT8 ubHospitalTrainingBonus;

	// HEADROCK HAM 3.2: Determines the repaid bonus when training in factory ("industrial") sectors
	UINT8 ubFactoryRepairBonus;

	// HEADROCK HAM 3.2: How much MECHANICAL skill do we need to gain a repair bonus from a factory?
	UINT8 ubMinMechanicalForFactoryRepairBonus;

	// HEADROCK HAM 3.3: Externalized maximum possible penalty for hitting a moving target. JA2 Default = 30.
	UINT16 usMaxCTHPenaltyForMovingTarget;

	// HEADROCK HAM 3.3: Increases tolerance while moving.
	UINT8 ubTilesMovedPerBonusTolerancePoint;
	
	// HEADROCK HAM 3.3: Minimum distance (in METERS) at which character suffer from friendly suppression.
	UINT16 usMinDistanceFriendlySuppression;
	
	// HEADROCK HAM 3.4: This controls the intensity of Hiding the Bullet Count during combat. The higher it is, the more intense the effect. Negative values reduce the effect.
	UINT16 usBulletHideIntensity;
		
	// HEADROCK HAM 3.5: Explosive Suppression Effectiveness alters the amount of Suppression Points you get from nearby blasts.
	UINT16 usExplosionSuppressionEffect;

	// HEADROCK HAM 3.5: When enabled, goggle-switching affects every merc in the sector, not just those belonging to the current squad.
	BOOLEAN fGoggleSwapAffectsAllMercsInSector;

	// HEADROCK HAM 3.5: Helicopter - Base cost per tile in "GREEN" airspace
	UINT16 usHelicopterBaseCostPerGreenTile;

	// HEADROCK HAM 3.5: Helicopter - Base cost per tile in "RED" airspace
	UINT16 usHelicopterBaseCostPerRedTile;

	// HEADROCK HAM 3.5: Experimental, arrival sectorX/Y.
	UINT8 ubDefaultArrivalSectorX;
	UINT8 ubDefaultArrivalSectorY;

	// HEADROCK HAM 3.5: Limit bonus from tracers based on range to target. This is a divisor for the effect.
	UINT8 ubRangeDifficultyAimingWithTracers;

	// HEADROCK HAM 3.6: Militia can now place flags on discovered mines.
	BOOLEAN fMilitiaPlaceBlueFlags;

	// HEADROCK PROFEX: Activate this to read Profile data from Profiles.XML and Opinions.XML
	BOOLEAN fReadProfileDataFromXML;

	// HEADROCK PROFEX: Activate this to read Profile data from Profiles.XML and Opinions.XML
	BOOLEAN fWriteProfileDataToXML;

	// HEADROCK HAM 3.6: Progress bars for each stat, displayed behind the stat value on the merc panel.
	UINT8 ubStatProgressBarsRed;
	UINT8 ubStatProgressBarsGreen;
	UINT8 ubStatProgressBarsBlue;

	// HEADROCK HAM 3.6: Determines how likely a character is, every hour, to trigger a facility event. The actual chance is X in every Y attempts, where X is facility-specific, and Y is the value adjusted here.
	UINT16 usFacilityEventRarity;
	// HEADROCK HAM 3.6: Controls how important the character's stats are, in insuring he/she gets better results (or smaller damage) from using facilities. The higher this is, the more dangerous all facilities are.
	UINT8 ubFacilityDangerRate;

	// HEADROCK HAM 3.6: Determines whether the extrapolated daily costs of mercs are figured into the "Daily Expenses" display. 0 = No, just facility costs. 1 = Only mercs with a fixed daily rate. 2 = All mercs, including AIM contracts.
	UINT8 ubIncludeContractsInExpenses;

	// HEADROCK HAM 3.6: Maximum number of messages displayed in Tactical view
	UINT8 ubMaxMessagesTactical;

	// HEADROCK HAM 3.6: Daily upkeep costs for militia
	UINT16 usDailyCostTown[MAX_MILITIA_LEVELS];

	// HEADROCK HAM 3.6: Non-Combat Bodytypes can't become hostile
	BOOLEAN fCanTrueCiviliansBecomeHostile;

	// HEADROCK HAM 3.6: Militia become hostile when attacked. 0 = No. 1 = If killed. 2 = If attacked (JA2 Default)
	UINT8 ubCanMilitiaBecomeHostile;

	// HEADROCK HAM 3.6: If activated, the game does not switch focus to a merc who spots an enemy in real-time mode. This fixes issues with Real-Time Sneak.
	BOOLEAN fNoAutoFocusChangeInRealtimeSneak;

	UINT8 ubEarlyRebelsRecruitment[4];	// early recruitment of Miguel and Carlos
	// silversurfer: don't play quote when merc spots a mine? TRUE = shut up! FALSE = tell me that you found a mine!
	BOOLEAN fMineSpottedNoTalk;
	// enables item drops for civilians
	BOOLEAN fCiviliansDropAll;

	// Flugente: can NPCs drop items in autoresolve?
	BOOLEAN fNPCAutoresolveItemDrop;

	// WANNE: Don't stop and talk when spotting a new item in turn based mode
	BOOLEAN fItemSpottedNoTalk;

	// The_Bob - real time sneaking code 01/06/09
	// Suport disabling/silencing real time sneaking via external .ini file
	//BOOLEAN fAllowRealTimeSneak; // SANDRO - moved to preferences
	BOOLEAN fQuietRealTimeSneak;

	//legion by Jazz
	BOOLEAN fCanJumpThroughWindows;
	BOOLEAN fCanJumpThroughClosedWindows;	
	BOOLEAN fCanClimbOnWalls;

	//JMich.BackpackClimb
	INT16 sBackpackWeightToClimb;
	BOOLEAN fUseGlobalBackpackSettings;

	// sevenfm: show enemy weapon above soldier in tactical
	BOOLEAN fShowEnemyWeapon;
	BOOLEAN fShowEnemyExtendedInfo;

	//legion by Jazz
	BOOLEAN fIndividualHiddenPersonNames;

	// Flugente: display NPCs we know of in a different colour on the sector and radar map
	BOOLEAN fKnownNPCsUseDifferentColour;
	
	//SAveGame Slot by Jazz
	BOOLEAN fSaveGameSlot;

	//dnl ch51 081009 JA2 Debug Settings
	BOOLEAN fEnableInventoryPoolQ;
	
	//legion by Jazz
	//BOOLEAN fShowTacticalFaceGear; //legion 2
	//BOOLEAN fShowTacticalFaceIcons; //legion 2
	INT8 bTacticalFaceIconStyle;
	
	//Enemy Names Group Legion 2 by Jazz
	BOOLEAN fEnemyNames;
	
	//Civ Names Group Legion 2 by Jazz
	BOOLEAN fCivGroupName;
	
	//Enemy Rank Legion 2 by Jazz
	BOOLEAN fEnemyRank;

	// sevenfm: show enemy rank as icon
	BOOLEAN fImprovedTacticalUI;

	// Flugente: soldier profiles
	BOOLEAN	fSoldierProfiles_Enemy;
	BOOLEAN	fSoldierProfiles_Militia;
	
	BOOLEAN fShowCamouflageFaces;

	//BOOLEAN fHideEnemyHealthText;
	UINT8 ubShowHealthBarsOnHead;
	UINT8 ubShowEnemyHealth;

	UINT8 ubEnemyHitCount;
	UINT8 ubPlayerHitCount;
	BOOLEAN fShowHitInfo;
	UINT8 ubAdditionalNCTHCursorInfo;
	UINT8 ubImprovedNCTHCursor;

	// sevenfm: new additional info above soldier
	UINT8 ubShowSuppressionCount;
	UINT8 ubShowShockCount;
	UINT8 ubShowAPCount;
	UINT8 ubShowMoraleCount;
	BOOLEAN ubShowSuppressionUseAsterisks;
	BOOLEAN ubShowSuppressionCountAlt;
	BOOLEAN ubShowSuppressionScaleAsterisk;

	FLOAT gMercLevelUpSalaryIncreasePercentage;

	// Flugente: mine income can depend on the number or workers we have
	BOOLEAN fMineRequiresWorkers;
	FLOAT dInitialWorkerRate;
	UINT32 usWorkerTrainingCost;
	UINT8 usWorkerTrainingPoints;

	UINT8 ubChanceTonyAvailable; // silversurfer/SANDRO

	BOOLEAN fShowSkillsInHirePage;

	UINT8 ubMercRandomStats;
	
	BOOLEAN fMercRandomBellDistribution;
	
	UINT8 ubMercRandomStatsRange;

	BOOLEAN fMercRandomGearKits;

	UINT8 ubMercRandomExpRange;

	BOOLEAN fMercRandomStartSalary;

	UINT8 ubMercRandomStartSalaryPercentMod;

	BOOLEAN fBobbyRayFastShipments;

	BOOLEAN fGridExitInTurnBased;

	BOOLEAN fStandUpAfterBattle;
	
	INT32 iInitialMercArrivalLocation;

	// Keyboard shortcut (as VK) for fastforward key.  See Utils/KeyMap
	INT32 iFastForwardKey;
	// Turn on fast forward whenever ui is disabled.
	BOOLEAN fEnableAutoFastForwardMode;
	// When fast forward is enabled what is the period to update clock in milliseconds.
	FLOAT iFastForwardPeriod;
	// Frequency that the screen is updated
	INT32 iNotifyFrequency;
	// Frequency that the screen is updated
	FLOAT fClockSpeedPercent;

	// sevenfm: improved auto fast forwarding
	BOOLEAN fImprovedAutoFastForward;

	// Flugente: Weapon Overheating
	BOOLEAN	fWeaponOverheating;						// Should weapons overheat, thus increasing the chance of malfunction?
	BOOLEAN fDisplayOverheatJamPercentage;			// Should we display temperature/jam threshold (true) or temperature/damage threshold (false)?
	UINT8	ubOverheatThermometerRedOffset;			// amount of red colour while temperature is below threshold
	FLOAT   iCooldownModificatorLonelyBarrel;		// Cooldown modificator for barrels left alone in the landscape ;-)
	
	// Flugente: Weapon Mounting
	BOOLEAN	fWeaponResting;							// Should it be possible to rest your weapon on structures in crouched position?
	UINT8	ubProneModifierPercentage;				// for boni, use ubProneModifierPercentage*boni[PRONE] + (100 - ubProneModifierPercentage)*boni[CROUCHED]

	// Flugente: Scope Modes
	BOOLEAN fScopeModes;							// allow the player to toggle between existing scopes/sights

	// Flugente: Spotter skill
	UINT8	usSpotterPreparationTurns;
	UINT16	usSpotterRange;
	UINT16	usSpotterMaxCTHBoost;
	
	// Flugente: External Feeding
	UINT8	ubExternalFeeding;						// allow external feeding of guns (0 = no, 1 = only for others, 2 = we can also fed ourselves)

	// Flugente: advanced repair/dirt system
	BOOLEAN	fAdvRepairSystem;						// allow thresholds to repairing
	BOOLEAN fOnlyRepairGunsArmour;					// we can only repair guns and armour
	FLOAT	iDirtGlobalModifier;					// a global modifier to dirt generation, if one feels that all values should be lower/higher

	// Flugente: prisoner related settings
	BOOLEAN fAllowPrisonerSystem;
	BOOLEAN	fEnemyCanSurrender;
	BOOLEAN fDisplaySurrenderSValues;
	FLOAT	fSurrenderMultiplier;
	BOOLEAN	fPlayerCanAsktoSurrender;
	UINT8	ubPrisonerReturntoQueenChance;
	UINT8	ubPrisonerProcessChance[PRISONER_INTERROGATION_MAX];
	UINT16	ubPrisonerInterrogationPoints[8];		// points needed to interrogate a prisoner of a specific type
	
	// Flugente: sexism/racism/etc.
	INT8	sMoraleModAppearance;
	INT8	sMoraleModRefinement;
	INT8	sMoraleModHatedNationality;
	INT8	sMoraleModRacism;
	INT8	sMoraleModSexism;
	INT8	sMoraleModXenophobicBackGround;

	// Flugente: dynamic opinions
	BOOLEAN fDynamicOpinions;
	BOOLEAN fDynamicOpinionsShowChange;
	FLOAT	fDynamicWageFactor;

	// Flugente: dynamic dialogue
	BOOLEAN fDynamicDialogue;
	INT32	sDynamicDialogueTimeOffset;

	// Flugente: PMC
	BOOLEAN fPMC;
	UINT8	usPMCMaxRegulars;
	UINT8	usPMCMaxVeterans;

	// Flugente: enemy roles
	BOOLEAN fEnemyRoles;
	UINT8	usTurnsToUncover;
	BOOLEAN fEnemyMedics;
	FLOAT	dEnemyMedicMedKitDrainFactor;
	INT16   sEnemyMedicsSearchRadius;
	INT32	sEnemyMedicsWoundMinAmount;
	BOOLEAN fEnemyMedicsHealSelf;
	BOOLEAN fEnemyOfficers;
	UINT16  usEnemyOfficersPerTeamSize;
	UINT16  usEnemyOfficersMax;
	INT8	sEnemyOfficerSuppressionResistanceBonus;
	FLOAT   dEnemyOfficerMoraleModifier;
	FLOAT   dEnemyOfficerSurrenderStrengthBonus;
	BOOLEAN fEnemyGenerals;
	UINT8	usEnemyGeneralsNumber;
	UINT16	usEnemyGeneralsBodyGuardsNumber;
	FLOAT	fEnemyGeneralStrategicDecisionSpeedBonus;
	FLOAT	fEnemyGeneralStrategicMovementSpeedBonus;

	// Flugente: individual militia
	BOOLEAN fIndividualMilitia;
	UINT16 usIndividualMilitia_PromotionPoints_To_Regular;
	UINT16 usIndividualMilitia_PromotionPoints_To_Elite;
	BOOLEAN fIndividualMilitia_ManageHealth;
	FLOAT dIndividualMilitiaHourlyHealthPercentageGain;
	FLOAT dIndividualMilitiaDoctorHealModifier;
	
	// Flugente: backgrounds
	BOOLEAN fBackGround;
	
	// Sandro: Alternative weapon holding (rifles fired from hip / pistols fired one-handed)
	UINT8 ubAllowAlternativeWeaponHolding;
	UINT8 ubToAltWeaponHoldReadyAPsPerc;
	UINT8 ubFromAltWeaponHoldReadyAPsPerc;
	UINT8 ubAltWeaponHoldingFireSpeedBonus;
	UINT8 ubAltWeaponHoldingCtHPenaly;
	UINT8 ubAltWeaponHoldingAimingPenaly;
	UINT8 ubAltWeaponHoldingAimLevelsReduced;

	// Sandro: Energy cost on weapon manipulation
	UINT8 ubEnergyCostForWeaponWeight;
	UINT8 ubEnergyCostForWeaponRecoilKick;

	BOOLEAN gBriefingRoom;
	BOOLEAN gEncyclopedia;
	UINT8 ubEncyclopediaItemMask;					//Moa: visibility of items in encyclopedia, see ENC_ITEM_VISIBILITY_T

	UINT8	ubMapItemChanceOverride;				//Madd: special map override, mostly for debugging
	UINT8	ubNumPItems;							//Madd: set number of PItem files to be used - default 3
	BOOLEAN	fUseXmlTileSets;						//Madd: move this variable here, it should be mod dependent

	UINT8	ubMarkerMode;							//DBrot: how should we mark the last hostile area?
	UINT8	ubSoldiersLeft;							//DBrot: how many soldiers may still be standing for this to become active
	UINT8	ubGridResolutionDay;					//DBrot: how precise we want to show their location
	UINT8	ubGridResolutionNight;					//DBrot: how precise we want to show their location - adjust for shorter night time ranges ... or don't

	BOOLEAN fRobotNoReadytime;						//DBrot: should the robot need to ready his gun?

	// Flugente: externalised squad names
	BOOLEAN fUseXMLSquadNames;

	//dnl ch68 090913 Reinforcements Setttings
	UINT16 sMinDelayEnemyReinforcements;
	UINT16 sRndDelayEnemyReinforcements;
	UINT16 sMinEnterEnemyReinforcements;
	UINT16 sRndEnterEnemyReinforcements;
	UINT16 sMinDelayMilitiaReinforcements;
	UINT16 sRndDelayMilitiaReinforcements;
	UINT16 sMinEnterMilitiaReinforcements;
	UINT16 sRndEnterMilitiaReinforcements;

	BOOLEAN fNoRemoveRandomSectorItems;//dnl ch68 090913

	// anv: helicopter repair settings
	BOOLEAN fWaldoCanRepairHelicopter;
	BOOLEAN fAllowWaldoToOfferRepairInStrategic;
	BOOLEAN fWaldoSubsequentRepairsIncreaseCosts;
	BOOLEAN fSeriouslyDamagedSkyriderWontFly;

	BOOLEAN fAlternativeHelicopterFuelSystem;
	BOOLEAN fHelicopterPassengersCanGetHit;

	UINT16 usHelicopterHoverCostOnGreenTile;
	UINT16 usHelicopterHoverCostOnRedTile;

	BOOLEAN fHelicopterReturnToBaseIsNotFree;
	BOOLEAN fPaySkyriderInBase;

	// anv: new iron man modes
	UINT8 ubExtremeIronManSavingTimeNotification;
	UINT8 ubExtremeIronManSavingHour;

	BOOLEAN fAllowDrivingVehiclesInTactical;

	BOOLEAN fAllowCarsDrivingOverPeople;
	BOOLEAN fAllowTanksDrivingOverPeople;

	UINT8 ubCarsRammingMaxStructureArmour;
	UINT8 ubEnemyJeepsRammingMaxStructureArmour;
	UINT8 ubTanksRammingMaxStructureArmour;

	UINT8 ubAPSharedAmongPassengersAndVehicleMode;
	UINT8 ubAPSharedAmongPassengersAndVehicleScale;

} GAME_EXTERNAL_OPTIONS;

typedef struct
{
	BOOLEAN fTauntCensoredMode;
	BOOLEAN fTauntShowPopupBox;
	BOOLEAN fTauntShowInLog;
	BOOLEAN fTauntMakeNoise;
	BOOLEAN fTauntOnlyVisibleEnemies;
	BOOLEAN fTauntShowPopupBoxIfHeard;
	BOOLEAN fTauntShowInLogIfHeard;
	BOOLEAN fTauntAlwaysShowPopupBox;
	BOOLEAN fTauntAlwaysShowInLog;
	UINT16 sVolume;
	INT16 sModDelay;
	UINT16 sMinDelay;
	UINT16 sMaxDelay;
	UINT8 ubTauntFireGunChance;
	UINT8 ubTauntFireLauncherChance;
	UINT8 ubTauntAttackBladeChance;
	UINT8 ubTauntAttackHTHChance;
	UINT8 ubTauntThrowKnifeChance;
	UINT8 ubTauntThrowGrenadeChance;
	UINT8 ubTauntChargeKnifeChance;
	UINT8 ubTauntChargeFistsChance;
	UINT8 ubTauntStealChance;
	UINT8 ubTauntRunAwayChance;
	UINT8 ubTauntSeekNoiseChance;
	UINT8 ubTauntAlertChance;
	UINT8 ubTauntSuspiciousChance;
	UINT8 ubTauntGotHitChance;
	UINT8 ubTauntGotDeafenedBlindedChance;
	UINT8 ubTauntGotRobbedChance;
	UINT8 ubTauntGotMissedChance;
	UINT8 ubTauntKillChance;
	UINT8 ubTauntHeadPopChance;
	UINT8 ubTauntHitChance;
	UINT8 ubTauntMissChance;
	UINT8 ubTauntOutOfAmmoChance;
	UINT8 ubTauntReloadChance;
	UINT8 ubTauntNoticedUnseenChance;
	UINT8 ubTauntSayHiChance;
	UINT8 ubTauntInformAboutChance;
	UINT8 ubRiposteChance;

} TAUNTS_SETTINGS;

typedef struct
{
	UINT16 usHelicopterBasicRepairCost;
	UINT16 usHelicopterSeriousRepairCost;
	UINT8 ubHelicopterBasicRepairTime;
	UINT8 ubHelicopterBasicRepairTimeVariation;
	UINT8 ubHelicopterSeriousRepairTime;
	UINT8 ubHelicopterSeriousRepairTimeVariation;
	UINT16 usHelicopterBasicCostIncreaseAfterBasicRepair;
	UINT16 usHelicopterBasicCostIncreaseAfterSeriousRepair;
	UINT16 usHelicopterSeriousCostIncreaseAfterBasicRepair;
	UINT16 usHelicopterSeriousCostIncreaseAfterSeriousRepair;
	UINT16 usHelicopterBasicRepairCostMax;
	UINT16 usHelicopterSeriousRepairCostMax;

	UINT8 ubHelicopterDistanceWithoutRefuel;
	UINT8 ubHelicopterHoverTimePerFuelUnit;
	UINT8 ubHelicopterRefuelTime;
	UINT8 ubHelicopterTimeDelayForHoverWait;
	UINT8 ubHelicopterTimeDelayForHoverWaitTooLong;
	BOOLEAN fAskBeforeKickingPassengersOut;

	UINT8 ubHelicopterSAMSiteAccuracy;
	UINT8 ubHelicopterPassengerHitChance;
	UINT8 ubHelicopterPassengerHitMinDamage;
	UINT8 ubHelicopterPassengerHitMaxDamage;

	BOOLEAN fHelicopterTownLoyaltyCheck;

} HELICOPTER_SETTINGS;

typedef struct
{
	UINT8 ubDefaultMorale;
	UINT8 ubHoursBetweenStrategicDelay;
	INT8 bValues[64];
	INT8 bModifiers[32];
} MORALE_SETTINGS;

typedef struct
{
	INT8 bValues[32];
} REPUTATION_SETTINGS;

typedef struct
{
	//Crepitus Feeding Sector
	UINT8 ubCrepitusFeedingSectorX;
	UINT8 ubCrepitusFeedingSectorY;
	UINT8 ubCrepitusFeedingSectorZ;
} CREATURES_SETTINGS;

typedef struct
{
	UINT8 ubMaxNumberOfTraits;
	UINT8 ubNumberOfMajorTraitsAllowed;
	
	UINT8 ubMaxNumberOfTraitsForIMP;
	UINT8 ubNumberOfMajorTraitsAllowedForIMP;

	//BOOLEAN fAllowSpecialMercTraitsException;
	//UINT8 ubSpecialMercID;

	BOOLEAN fAllowAttributePrereq;

	// GENERIC SETTINGS
	INT8 bCtHModifierAssaultRifles;
	INT8 bCtHModifierSniperRifles;
	INT8 bCtHModifierRifles;
	INT8 bCtHModifierSMGs;
	INT8 bCtHModifierLMGs;
	INT8 bCtHModifierShotguns;
	INT8 bCtHModifierPistols;
	INT8 bCtHModifierMachinePistols;
	INT8 bCtHModifierRocketLaunchers;
	INT8 bCtHModifierGrenadeLaunchers;
	INT16 sCtHModifierMortar;
	INT8 bCtHModifierThrowingKnives;
	INT8 bCtHModifierThrowingGrenades;
	INT8 bCtHModifierHtHAttack;
	INT8 bModifierDodgeHtHChance;
	INT8 bCtHModifierKnifeAttack;
	INT8 bModifierDodgeKnifeChance;
	INT8 bCtHModifierRobot;
	UINT8 ubCtHPenaltyDualShot;
	INT8 bPercentModifierHtHBreathLoss;
	INT8 bPercentModifierBladesBreathLoss;
	INT8 bPercentModifierBluntBreathLoss;
	UINT8 ubModifierForAPsAddedOnAimedPunches;
	UINT8 ubModifierForAPsAddedOnAimedBladedAttackes;
	INT8 bSpeedModifierDoctoring;
	INT8 bSpeedModifierBandaging;
	INT8 bSpeedModifierRepairing;
	INT8 bSpeedModifierTrainingMilitia;
	INT8 bSpeedModifierTeachingOthers;
	INT8 bCheckModifierLockpicking;
	INT8 bCheckModifierAttachDetonators;
	INT8 bCheckModifierSetExplosives;
	INT8 bCheckModifierDisarmExplosiveTraps;
	INT8 bCheckModifierDisarmElectronicTraps;
	INT8 bCheckModifierAttachSpecialItems;
	INT8 bTanksDamageResistanceModifier;
	UINT8 ubDamageNeededToLoseStats;

	// AUTO WEAPONS
	UINT8 ubAWBonusCtHAssaultRifles;
	UINT8 ubAWBonusCtHSMGs;
	UINT8 ubAWBonusCtHLMGs;
	UINT8 ubAWAutoFirePenaltyReduction;
	UINT8 ubAWUnwantedBulletsReduction;
	UINT8 ubAWFiringSpeedBonusLMGs;
	UINT8 ubAWPercentReadyLMGReduction;

	// HEAVY WEAPONS
	UINT8 ubHWGrenadeLaunchersAPsReduction;
	UINT8 ubHWRocketLaunchersAPsReduction;
	UINT8 ubHWBonusCtHGrenadeLaunchers;
	UINT8 ubHWBonusCtHRocketLaunchers;
	UINT8 ubHWMortarAPsReduction;
	UINT8 ubHWMortarCtHPenaltyReduction;
	UINT8 ubHWDamageTanksBonusPercent;
	UINT8 ubHWDamageBonusPercentForHW;

	// SNIPER
	UINT8 ubSNBonusCtHRifles;
	UINT8 ubSNBonusCtHSniperRifles;
	UINT8 ubSNEffRangeToTargetReduction;
	UINT8 ubSNAimingBonusPerClick;
	UINT8 ubSNDamageBonusPerClick;
	UINT8 ubSNDamageBonusFromNumClicks;
	UINT8 ubSNChamberRoundAPsReduction;
	UINT8 ubSNAimClicksAdded;
	UINT8 ubSNFocusRadius;
	INT8 sSNFocusInterruptBonus;
	
	// RANGER
	UINT8 ubRABonusCtHRifles;
	UINT8 ubRABonusCtHShotguns;
	UINT8 ubRAPumpShotgunsAPsReduction;
	UINT8 ubRAFiringSpeedBonusShotguns;
	UINT8 ubRAAimClicksAdded;
	UINT8 ubRAEffectiveRangeBonusShotguns;
	UINT8 ubRAReloadSpeedShotgunsManual;

	// GUNSLINGER
	UINT8 ubGSFiringSpeedBonusPistols;
	UINT8 ubGSEffectiveRangeBonusPistols;
	UINT8 ubGSBonusCtHPistols;
	UINT8 ubGSBonusCtHMachinePistols;
	UINT8 ubGSCtHMPExcludeAuto;
	UINT8 ubGSAimingBonusPerClick;
	UINT8 ubGSPercentReadyPistolsReduction;
	UINT8 ubGSRealoadSpeedHandgunsBonus;
	UINT8 ubGSAimClicksAdded;
	BOOLEAN fCanFanTheHammer;

	// MARTIAL ARTS
	UINT8 ubMABonusCtHBareHands;
	UINT8 ubMABonusCtHBrassKnuckles;
	UINT8 ubMAPunchAPsReduction;
	UINT8 ubMABonusDamageHandToHand;
	UINT8 ubMABonusBreathDamageHandToHand;
	UINT16 usMALostBreathRegainPenalty;
	UINT16 usMAAimedPunchDamageBonus;
	INT8 bMAAimedPunchCtHModifier;
	UINT8 ubMAChanceToDodgeHtH;
	UINT8 ubMAOnTopCTDHtHBareHanded;
	UINT8 ubMAOnTopCTDHtHBrassKnuckles;
	UINT8 ubMAChanceToDodgeMelee;
	UINT8 ubMAReducedAPsToSteal;
	UINT8 ubMAAPsChangeStanceReduction;
	UINT8 ubMAApsTurnAroundReduction;
	UINT8 ubMAAPsClimbOrJumpReduction;
	UINT8 ubMAReducedAPsRegisteredWhenMoving;
	UINT8 ubMAChanceToCkickDoors;
	BOOLEAN fPermitExtraAnimationsOnlyToMA;

	// SQUADLEADER
	UINT16 usSLRadiusNormal;
	UINT16 usSLRadiusExtendedEar;
	UINT8 ubSLMaxBonuses;
	UINT8 ubSLBonusAPsPercent;
	UINT8 ubSLEffectiveLevelInRadius;
	UINT8 ubSLEffectiveLevelAsStandby;
	UINT8 ubSLCollectiveInterruptsBonus;
	UINT8 ubSLOverallSuppresionBonusPercent;
	UINT8 ubSLMoraleGainBonus;
	UINT8 ubSLMoraleLossReduction;
	UINT8 ubSLFearResistance;
	UINT8 ubSLDeathMoralelossMultiplier;

	// TECHNICIAN
	UINT16 usTERepairSpeedBonus;
	UINT16 usTELockpickingBonus;
	UINT16 usTEDisarmElTrapBonus;
	UINT16 usTEAttachingItemsBonus;
	UINT8 ubTEUnjamGunBonus;
	UINT8 ubTERepairElectronicsPenaltyReduction;
	UINT8 ubTEChanceToDetectTrapsBonus;
	UINT8 ubTECtHControlledRobotBonus;
	UINT8 ubTERepairRobotPenaltyReduction;
	UINT8 ubTETraitsNumToRepairRobot;
	BOOLEAN fTETraitsCanRestoreItemThreshold;

	// DOCTOR
	UINT8 ubDONumberTraitsNeededForSurgery;
	UINT8 ubDOSurgeryHealPercentBase;
	UINT8 ubDOSurgeryHealPercentOnTop;
	UINT8 ubDOSurgeryHealPercentBloodbag;
	UINT16 usDOSurgeryMedBagConsumption;
	UINT16 usDOSurgeryMaxBreathLoss;
	UINT16 usDORepairStatsRateBasic;
	UINT16 usDORepairStatsRateOnTop;
	UINT8 ubDORepStPenaltyIfAlsoHealing;
	UINT8 ubDOHealingPenaltyIfAlsoStatRepair;
	BOOLEAN fDORepStShouldThrowMessage;
	UINT8 ubDOBandagingSpeedPercent;
	UINT16 usDODoctorAssignmentBonus;
	UINT8 ubDONaturalRegenBonus;
	UINT8 ubDOMaxRegenBonuses;

	// AMBIDEXTROUS
	UINT8 ubAMPenaltyDoubleReduction;
	UINT8 ubAMReloadSpeedMagazines;
	UINT8 ubAMReloadSpeedLoose;
	UINT8 ubAMPickItemsAPsReduction;
	UINT8 ubAMWorkBackpackAPsReduction;
	UINT8 ubAMHandleDoorsAPsReduction;
	UINT8 ubAMHandleBombsAPsReduction;
	UINT8 ubAMAttachingItemsAPsReduction;

	// MELEE
	UINT8 ubMEBladesAPsReduction;
	UINT8 ubMECtHBladesBonus;
	UINT8 ubMECtHBluntBonus;
	UINT8 ubMEDamageBonusBlades;
	UINT8 ubMEDamageBonusBlunt;
	UINT16 usMEAimedMeleeAttackDamageBonus;
	UINT8 ubMEDodgeBladesBonus;
	UINT8 ubMECtDBladesOnTopWithBladeInHands;
	UINT8 ubMEDodgeBluntBonus;
	UINT8 ubMECtDBluntOnTopWithBladeInHands;

	// THROWING
	UINT8 ubTHBladesAPsReduction;
	UINT8 ubTHBladesMaxRange;
	UINT8 ubTHBladesCtHBonus;
	UINT8 ubTHBladesCtHBonusPerClick;
	UINT8 ubTHBladesDamageBonus;
	UINT8 ubTHBladesDamageBonusPerClick;
	UINT8 ubTHBladesSilentCriticalHitChance;
	UINT8 ubTHBladesCriticalHitMultiplierBonus;
	UINT8 ubTHBladesAimClicksAdded;

	// NIGHT OPS
	UINT8 ubNOeSightRangeBonusInDark;
	UINT8 ubNOHearingRangeBonus;
	UINT8 ubNOHearingRangeBonusInDark;
	UINT8 ubNOIterruptsBonusInDark;
	UINT8 ubNONeedForSleepReduction;

	// STEALTHY
	UINT8 ubSTStealthModeSpeedBonus;
	UINT8 ubSTBonusToMoveQuietly;
	UINT8 ubSTStealthBonus;
	UINT8 ubSTReducedAPsRegistered;
	UINT8 ubSTStealthPenaltyForMovingReduction;

	// ATHLETICS
	UINT8 ubATAPsMovementReduction;
	UINT8 ubATBPsMovementReduction;

	// BODYBUILDING
	UINT8 ubBBDamageResistance;
	UINT8 ubBBCarryWeightBonus;
	UINT8 ubBBBreathLossForHtHImpactReduction;
	UINT16 usBBIncreasedNeededDamageToFallDown;

	// DEMOLITIONS
	UINT8 ubDEAPsNeededToThrowGrenadesReduction;
	UINT8 ubDEMaxRangeToThrowGrenades;
	UINT8 ubDECtHWhenThrowingGrenades;
	UINT8 ubDEDamageOfBombsAndMines;
	UINT8 ubDEAttachDetonatorCheckBonus;
	UINT8 ubDEPlantAndRemoveBombCheckBonus;
	UINT8 ubDEPlacedBombLevelBonus;
	UINT8 ubDEShapedChargeDamageMultiplier;

	// TEACHING
	UINT8 ubTGBonusToTrainMilitia;
	UINT8 ubTGEffectiveLDRToTrainMilitia;
	UINT8 ubTGBonusToTeachOtherMercs;
	UINT8 ubTGEffectiveSkillValueForTeaching;
	UINT8 ubTGBonusOnPractising;

	// SCOUTING
	UINT8 ubSCSightRangebonusWithScopes;
	UINT16 usSCSightRangebonusWithBinoculars;
	UINT8 ubSCTunnelVisionReducedWithBinoculars;
	BOOLEAN fSCCanDetectEnemyPresenseAround;
	BOOLEAN fSCCanDetermineEnemyNumbersAround;
	BOOLEAN fSCDetectionInDiagonalSectors;
	BOOLEAN fSCPreventsTheEnemyToAmbushMercs;
	BOOLEAN fSCPreventsBloodcatsAmbushes;
	BOOLEAN fSCThrowMessageIfAmbushPrevented;

	// COVERT OPS
	INT16	sCOMeleeCTHBonus;
	INT16	sCoMeleeInstakillBonus;
	INT16	sCODisguiseAPReduction;
	INT16	sCOCloseDetectionRange;
	INT16	sCOCloseDetectionRangeSoldierCorpse;
	UINT16	usCOEliteUncoverRadius;
	BOOLEAN fCODetectIfBleeding;					// will we be discovered if we are bleeding?
	BOOLEAN fCOStripIfUncovered;

	// RADIO OPERATOR
	BOOLEAN fROAllowArtillery;						// artillery can be forbidden for all teams with this setting
	BOOLEAN fROArtilleryDistributedOverTurns;		// shells don't come in at once, but over multiple turns
	INT16 bVOArtillerySectorFrequency;				// ability to call an a artillery strike from a sector every '_' minutes. Only applies if ordered from enemy/militia
	UINT8 usVOMortarCountDivisor;
	UINT8 usVOMortarShellDivisor;
	UINT8 usVOMortarPointsAdmin;
	UINT8 usVOMortarPointsTroop;
	UINT8 usVOMortarPointsElite;
	UINT8 usVOMortarRadius;
	UINT8 usVOMortarSignalShellRadius;
	UINT8 usVOReinforcementSetting;
	INT8  sVOScanAssignmentBaseRange;
	INT8  sVOListeningHearingBonus;
	BOOLEAN fVOJammingBlocksRemoteBombs;
	BOOLEAN fVOEnemyVOSetsOffRemoteBombs;
	
	// SNITCH
	UINT8 ubSNTBaseChance;						// Base chance that snitch will inform about certain event (in percents)
	FLOAT fSNTMercOpinionAboutSnitchBonusModifier;	// How much chance is influenced by relation between snitch and target
	FLOAT fSNTSnitchOpinionAboutMercBonusModifier;	// How much chance is influenced by relation between snitch and target
	FLOAT fSNTSnitchLeadershipBonusModifer;	// How much chance is influenced by snitch's leadership
	INT8 bSNTSociableMercBonus;				// Bonus to chance if merc is sociable
	INT8 bSNTLonerMercBonus;					// Bonus to chance if merc is loner
	INT8 bSNTSameAssignmentBonus;				// Bonus to chance if snitch and merc are on the same assignment
	INT8 bSNTMercOpinionAboutMercTreshold;		// For reports "merc hates merc" final chance is proportional to dislike in range between treshold (0%) and -25(100%)
	UINT8 ubSNTPassiveReputationGain;			// Reputation gain per day if snitch is happy
	UINT8 ubSNTHearingRangeBonus;				// Hearing range bonus
	FLOAT fSNTPrisonSnitchInterrogationMultiplier;	// Undercover snitch interrogation effectivness multiplier (compared to normal interrogation)
	FLOAT fSNTPrisonSnitchGuardStrengthMultiplier;	// Undercover snitch guarding effectivness multiplier (compared to normal guards)

	// SURVIVAL
	UINT8 ubSVGroupTimeSpentForTravellingFoot;
	UINT8 ubSVGroupTimeSpentForTravellingVehicle;
	UINT8 ubSVMaxBonusesToTravelSpeed;
	UINT8 ubSVBreathForTravellingReduction;
	FLOAT dSVWeatherPenaltiesReduction;
	UINT8 ubSVCamoWornountSpeedReduction;
	UINT16 usSVTrackerMaxRange;
	UINT8 usSVTrackerAbility;
	UINT8 usSVDiseaseResistance;
	INT8 sSVFoodConsumption;					// alters food consumption rate
	INT8 sSVDrinkConsumption;
	UINT8 usSVSnakeDefense;						// increases chance to evade a snake attack
} SKILL_TRAIT_VALUES;

// HEADROCK HAM 4: Constants used as coefficients by the various parts of the new CTH system.
typedef struct
{
	UINT32 NORMAL_SHOOTING_DISTANCE;		// Distance at which 1x magnification is 100% effective. This is a major component of the entire shooting mechanism.
	FLOAT IRON_SIGHT_PERFORMANCE_BONUS;		// percentage bonus to base aperture size for iron sights
	FLOAT LASER_PERFORMANCE_BONUS_HIP;		// percentage bonus to base aperture size for laser pointers when shooting from hip
	FLOAT LASER_PERFORMANCE_BONUS_IRON;		// percentage bonus to base aperture size for laser pointers when using iron sights
	FLOAT LASER_PERFORMANCE_BONUS_SCOPE;	// percentage bonus to base aperture size for laser pointers when using scopes
	FLOAT DEGREES_MAXIMUM_APERTURE;			// Maximum possible aperture for a 100% muzzle sway shot. Decrease to make all shots more accurate.
	BOOLEAN IRON_SIGHTS_MAX_APERTURE_USE_GRADIENT;	// Use new curve instead of linear function to get the max aperture at target range for iron sights.
	FLOAT IRON_SIGHTS_MAX_APERTURE_MODIFIER;	// Modifier that is used to make aperture grow slower/faster when IRON_SIGHTS_MAX_APERTURE_USE_GRADIENT is TRUE.
	FLOAT RANGE_COEFFICIENT;				// Determines maximum range which decides when gravity forces bullets to drop
	FLOAT GRAVITY_COEFFICIENT;				// Changes the way gravity works in the game. Higher values mean bullets don't drop as quickly after reaching max range.
	FLOAT VERTICAL_BIAS;					// This float can be used to reduce the chance of missing too far upwards or downwards (compared to left/right).
	FLOAT SCOPE_RANGE_MULTIPLIER;			// Adjusts the minimum effective range of scopes
	FLOAT SCOPE_EFFECTIVENESS_MULTIPLIER;	// This modifies the maximum effective magnification that is applied to the shooting aperture.
	UINT16 SCOPE_EFFECTIVENESS_MINIMUM;		// Defines a minimum effectiveness level that every shooter gets. 0 = fully dynamic, no minimum, 100 = no skills required to use scopes
	UINT16 SCOPE_EFFECTIVENESS_MINIMUM_RANGER;		// the minimum effectiveness for the ranger. He can reach more using the normal formula.
	UINT16 SCOPE_EFFECTIVENESS_MINIMUM_MARKSMAN;	// the minimum effectiveness for the marksman. He can reach more using the normal formula.
	UINT16 SCOPE_EFFECTIVENESS_MINIMUM_SNIPER;		// the minimum effectiveness for the sniper. He can reach more using the normal formula.
	FLOAT SIDE_FACING_DIVISOR;				// Deals with a visual error in NCTH relating to shooting at a target who is facing directly perpendicular to the shooters facing.
	UINT16 BASIC_RELIABILITY_ODDS;			// Determines the base chance to lose one condition point when firing a gun.

	FLOAT BASE_EXP;				// Importance of Experience for BASE CTH
	FLOAT BASE_MARKS;				// Importance of Marksmanship for BASE CTH
	FLOAT BASE_WIS;				// Importance of Wisdom for BASE CTH
	FLOAT BASE_DEX;				// Importance of Dexterity for BASE CTH

	FLOAT BASE_LOW_MORALE;		// Applied gradually when morale is below 50.
	FLOAT BASE_HIGH_MORALE;		// Applied gradually when morale is above 50.
	FLOAT BASE_PSYCHO;			// Applied for each level of PSYCHO trait
	FLOAT BASE_SHOOTING_UPWARDS;	// Applied gradually for shooting at a higher target. Decreases with distance.
	FLOAT BASE_INJURY;			// Applied gradually for injuries
	FLOAT BASE_DRUNK[4];			// Applied for drunkness levels
	FLOAT BASE_FATIGUE;			// Applied gradually for fatigue
	FLOAT BASE_SAME_TARGET;		// Applied for shooting at the same target again
	FLOAT BASE_GASSED;			// Applied for shooting while breathing gas
	FLOAT BASE_BEING_BANDAGED;	// Applied for shooting while being bandaged
	FLOAT BASE_SHOCK;				// Applied gradually for shock points
	FLOAT BASE_AGILE_TARGET;		// Applied gradually for agile or experienced target
	FLOAT BASE_TARGET_INVISIBLE;	// Applied for shooting at a target you can't see
	FLOAT BASE_DRAW_COST;			// Applied per 1 AP of the weapon's Ready Cost, under 100AP.
	FLOAT BASE_TWO_GUNS;			// Gun Difficulty Multiplier for shooting two guns
	FLOAT BASE_ONE_HANDED;		// Gun Difficulty Multiplier for shooting a pistol with one hand.
	FLOAT BASE_STANDING_STANCE;	// Gun Difficulty Multiplier for shooting from a standing stance
	FLOAT BASE_CROUCHING_STANCE;	// Gun Difficulty Multiplier for shooting from a crouched stance
	FLOAT BASE_PRONE_STANCE;		// Gun Difficulty Multiplier for shooting from a prone stance
	FLOAT BASE_HEAVY_WEAPON;		// Gun Difficulty Multiplier for shooting a launcher

	FLOAT AIM_EXP;				// Importance of Experience for AIMING CTH
	FLOAT AIM_MARKS;				// Importance of Marksmanship for AIMING CTH
	FLOAT AIM_WIS;				// Importance of Wisdom for AIMING CTH
	FLOAT AIM_DEX;				// Importance of Dexterity for AIMING CTH

	FLOAT AIM_TOO_CLOSE_SCOPE;	// Applied per tile closer than the scope's minimum range.
	FLOAT AIM_TOO_CLOSE_THRESHOLD;	// Affects at what point AIM_TOO_CLOSE_SCOPE kicks in (0.0 to 1.0).
	FLOAT AIM_GUN_CONDITION;		// Applied per point of condition below 50.
	FLOAT AIM_LOW_MORALE;			// Applied gradually when morale is below 50.
	FLOAT AIM_HIGH_MORALE;		// Applied gradually when morale is above 50.
	FLOAT AIM_PSYCHO;				// Applied for each level of PSYCHO trait
	FLOAT AIM_VISIBILITY;			// Applied for bad visibility
	FLOAT AIM_SHOOTING_UPWARDS;	// Applied gradually for shooting at a higher target. Decreases with distance.
	FLOAT AIM_INJURY;				// Applied gradually for injuries
	FLOAT AIM_DRUNK[4];			// Applied for drunkness levels
	FLOAT AIM_FATIGUE;			// Applied gradually for fatigue
	FLOAT AIM_GASSED;				// Applied for shooting while breathing gas
	FLOAT AIM_BEING_BANDAGED;		// Applied for shooting while being bandaged
	FLOAT AIM_SHOCK;				// Applied gradually for shock points
	FLOAT AIM_TARGET_INVISIBLE;	// Applied for shooting at a target you can't see
	FLOAT AIM_SNIPER_SKILL;		// Applied as percentage bonus to CTH cap, once per each level.
	FLOAT AIM_DRAW_COST;			// Applied per 1 AP of the weapon's Ready Cost, under 100AP.
	FLOAT AIM_STANDING_STANCE;	// Gun Difficulty multiplier for shooting a heavy gun when standing
	FLOAT AIM_CROUCHING_STANCE;	// Gun Difficulty multiplier for shooting a heavy gun from a crouched stance
	FLOAT AIM_PRONE_STANCE;		// Gun Difficulty multiplier for shooting a heavy gun when prone, before bipod bonuses
	FLOAT AIM_TWO_GUNS;			// Gun Difficulty multiplier for shooting two guns (halved for each Ambidextrous level)
	FLOAT AIM_ONE_HANDED;			// Gun Difficulty multiplier for firing a pistol with one hand.
	FLOAT AIM_HEAVY_WEAPON;		// Gun Difficulty multiplier for shooting a launcher (halved for each Heavy Weapons level)

	FLOAT MOVEMENT_MRK;
	FLOAT MOVEMENT_WIS;
	FLOAT MOVEMENT_DEX;
	FLOAT MOVEMENT_EXP_LEVEL;
	UINT32 MOVEMENT_TRACKING_DIFFICULTY;
	FLOAT MOVEMENT_PENALTY_PER_TILE;

	FLOAT PRE_RECOIL_WIS;
	FLOAT PRE_RECOIL_EXP_LEVEL;
	FLOAT PRE_RECOIL_AUTO_WEAPONS_SKILL;

	FLOAT RECOIL_MAX_COUNTER_STR;
	FLOAT RECOIL_MAX_COUNTER_AGI;
	FLOAT RECOIL_MAX_COUNTER_EXP_LEVEL;
	FLOAT RECOIL_MAX_COUNTER_FORCE;
	FLOAT RECOIL_MAX_COUNTER_CROUCH;
	FLOAT RECOIL_MAX_COUNTER_PRONE;
	FLOAT RECOIL_COUNTER_ACCURACY_MIN_ERROR;
	FLOAT RECOIL_COUNTER_ACCURACY_DEX;
	FLOAT RECOIL_COUNTER_ACCURACY_WIS;
	FLOAT RECOIL_COUNTER_ACCURACY_AGI;
	FLOAT RECOIL_COUNTER_ACCURACY_EXP_LEVEL;
	FLOAT RECOIL_COUNTER_ACCURACY_AUTO_WEAPONS_DIVISOR;
	FLOAT RECOIL_COUNTER_ACCURACY_TRACER_BONUS;
	FLOAT RECOIL_COUNTER_ACCURACY_ANTICIPATION;
	FLOAT RECOIL_COUNTER_ACCURACY_COMPENSATION;
	FLOAT RECOIL_COUNTER_FREQUENCY_AGI;
	FLOAT RECOIL_COUNTER_FREQUENCY_EXP_LEVEL;
	FLOAT RECOIL_COUNTER_FREQUENCY_AUTO_WEAPONS_DIVISOR;

	UINT8 RECOIL_COUNTER_INCREMENT;
	UINT8 RECOIL_COUNTER_INCREMENT_TRACER;
	UINT32 NORMAL_RECOIL_DISTANCE;
	FLOAT MAX_BULLET_DEV;
	BOOLEAN RANGE_EFFECTS_DEV;
	FLOAT MAX_EFFECTIVE_RANGE_MULTIPLIER;
	FLOAT MAX_EFFECTIVE_RANGE_REDUCTION;
	BOOLEAN MAX_EFFECTIVE_USE_GRADIENT;

} CTH_CONSTANTS;
//DBrot: Grids
typedef struct
{
	//AI
	UINT8 ubSAISpawnSectorX;
	UINT8 ubSAISpawnSectorY;
	
	//defines the basement and the bottom of the stairs
	UINT8 ubHideoutSectorX;
	UINT8 ubHideoutSectorY;
	UINT8 ubHideoutSectorZ;
	INT32 iHideoutExitGrid;
	//defines the surface and the top of the stairs
	UINT8 ubHideoutSurfaceX;
	UINT8 ubHideoutSurfaceY;
	UINT8 ubHideoutSurfaceZ;
	INT32 iHideoutEntryGrid;
	//where your mercs land when entering the basement, added some for 10 man squads
	INT32 iBasementEntry[10];
	
	//where your mercs land when leaving the basement, again added some
	INT32 iBasementExit[12];
	
	//this moves the crate to reveal the entrance
	INT32 iFinalCrateGrid;
	UINT16 usCrateTileDef;
	UINT16 usTrapdoorTileDef;
	
	//Leave Stuff
	UINT8 ubOmertaDropOffX;
	UINT8 ubOmertaDropOffY;
	UINT8 ubOmertaDropOffZ;
	INT32 iOmertaDropOff;
	INT32 iAirportDropOff;

	//Airport Stuff
	UINT8 ubAirportX;
	UINT8 ubAirportY;
	INT32 iHeliSquadDropOff;
	
	UINT8 ubAirport2X;
	UINT8 ubAirport2Y;

	//BobbyR stuff
	UINT8 ubBobbyRShipDestSectorX;
	UINT8 ubBobbyRShipDestSectorY;
	UINT8 ubBobbyRShipDestSectorZ;
	INT32 iBobbyRShipDestGridNo;
	INT32 iPablosStolenDestGridNo;
	INT32 iLostShipmentGridNo;

	//[Omerta]
	INT32 iInitialMercArrivalLocation;

	//[Drassen]
	//meet Carmen in sector for reward collection
	UINT8 ubCarmenGiveRewardSectorX;
	UINT8 ubCarmenGiveRewardSectorY;
	UINT8 ubCarmenGiveRewardSectorZ;

	//[Alma]
	INT32 sRocketRifleGridNo;
	
	//POW stuff for QUEST_HELD_IN_ALMA
	UINT8 ubInitialPOWSectorX;
	UINT8 ubInitialPOWSectorY;
	INT32 iInitialPOWGridNo[3];
	INT32 iInitialPOWItemGridNo[3];

	//[Grumm]

	//[Tixa]
	UINT8 ubTixaPrisonSectorX;
	UINT8 ubTixaPrisonSectorY;

	//Dynamo Captive Sector
	UINT8 ubDyanmoCaptiveSectorX;
	UINT8 ubDyanmoCaptiveSectorY;
	UINT8 ubDyanmoCaptiveSectorZ;

	//[Cambria]
	//Hospital Sector
	UINT8 ubHospitalSectorX;
	UINT8 ubHospitalSectorY;
	UINT8 ubHospitalSectorZ;

	//add Rat to location after creature meanwhile scene
	UINT8 ubAddRatSectorX;
	UINT8 ubAddRatSectorY;
	UINT8 ubAddRatSectorZ;

	//[San Mona]
	//Porn Quest
	UINT16 usPornShopRoomHans;
	INT32  iHansGridNo;
	UINT16 usPornShopRoomBrenda;
	UINT16 usPornShopRoomTony;
	UINT8 ubPornShopTonySectorX;
	UINT8 ubPornShopTonySectorY;
	UINT8 ubPornShopTonySectorZ;
	INT32 iPornShopEntranceGridNo;

	// Kyle Lemmons Leather Shop Deed Quest
	INT32  iKyleMoneyGridNo;

	//Brothel Quests
	UINT16 usLeatherShop;
	
	UINT8 ubBrothelSectorX;
	UINT8 ubBrothelSectorY;
	INT32  iCarlaDoorGridNo;
	INT32  iCindyDoorGridNo;
	INT32  iBambiDoorGridNo;
	INT32  iMariaDoorGridNo;

	UINT16 usBrothelRoomRangeStart;
	UINT16 usBrothelRoomRangeEnd;
	UINT16 usBrothelGuardRoom;

	INT32 iBrothelDoor1;
	INT32 iBrothelDoor2;
	INT32 iBrothelDoor3;

	UINT8 ubKingpinHouseSectorX;
	UINT8 ubKingpinHouseSectorY;
	UINT8 ubKingpinHouseSectorZ;
	UINT16 usKingpinRoomRangeStart;
	UINT16 usKingpinRoomRangeEnd;
	INT32 iKingpinRingTile;
	
	//Kingpin Private Stash
	UINT8 ubKingpinMoneySectorX;
	UINT8 ubKingpinMoneySectorY;
	UINT8 ubKingpinMoneySectorZ;

	//Joey primary and alternate Location to be found
	UINT8 ubJoeyPrimarySectorX;
	UINT8 ubJoeyPrimarySectorY;
	UINT8 ubJoeyPrimarySectorZ;
	UINT8 ubJoeyAlternateSectorX;
	UINT8 ubJoeyAlternateSectorY;
	UINT8 ubJoeyAlternateSectorZ;

	//Spike new location when assassins are sent
	UINT8 ubSpikeNewSectorX;
	UINT8 ubSpikeNewSectorY;
	UINT8 ubSpikeNewSectorZ;

	//add Iggy to location when game progress condition met
	UINT8 ubAddIggySectorX;
	UINT8 ubAddIggySectorY;
	UINT8 ubAddIggySectorZ;

	//[Estoni]

	//[Orta]

	//[Balime]

	//[Meduna]
	//Queen's new gridno instead of the map pre-placed gridno if basement sector is cleared
	UINT8 ubQueenBasementSectorX;
	UINT8 ubQueenBasementSectorY;
	UINT8 ubQueenBasementSectorZ;
	INT32 iQueenAlternateGridNo;

	//[Chitzena]


	//Weapon Cache Locations
	UINT8 ubWeaponCache1X;
	UINT8 ubWeaponCache1Y;
	UINT8 ubWeaponCache2X;
	UINT8 ubWeaponCache2Y;
	UINT8 ubWeaponCache3X;
	UINT8 ubWeaponCache3Y;
	UINT8 ubWeaponCache4X;
	UINT8 ubWeaponCache4Y;
	UINT8 ubWeaponCache5X;
	UINT8 ubWeaponCache5Y;

	//Meanwhile scenes
	//palace sector
	UINT8 ubMeanwhilePalaceSectorX;
	UINT8 ubMeanwhilePalaceSectorY;

	//interrogate and second POW stuff
	UINT8 ubMeanwhileInterrogatePOWSectorX;
	UINT8 ubMeanwhileInterrogatePOWSectorY;
	INT32 iMeanwhileInterrogatePOWGridNo[3];
	INT32 iMeanwhileInterrogatePOWItemGridNo[3];

	//row sectors to trigger meduna outskirts meanwhile
	UINT8 ubMeanwhileMedunaOutskirtsRowMinX;
	UINT8 ubMeanwhileMedunaOutskirtsRowMaxX;
	UINT8 ubMeanwhileMedunaOutskirtsRowY;
	//column sectors to trigger meduna outskirts meanwhile
	UINT8 ubMeanwhileMedunaOutskirtsColX;
	UINT8 ubMeanwhileMedunaOutskirtsColMinY;
	UINT8 ubMeanwhileMedunaOutskirtsColMaxY;

	//add Madlab to location after scientist AWOL meanwhile scene
	UINT8 ubMeanwhileAddMadlabSector1X;
	UINT8 ubMeanwhileAddMadlabSector1Y;
	UINT8 ubMeanwhileAddMadlabSector2X;
	UINT8 ubMeanwhileAddMadlabSector2Y;
	UINT8 ubMeanwhileAddMadlabSector3X;
	UINT8 ubMeanwhileAddMadlabSector3Y;
	UINT8 ubMeanwhileAddMadlabSector4X;
	UINT8 ubMeanwhileAddMadlabSector4Y;
	
	//[End Game]
	//victory celebration stuff
	UINT8 ubEndGameVictorySectorX;
	UINT8 ubEndGameVictorySectorY;
	INT32 iEndGameVictoryGridNo;
	
	//enable meanwhile cutscene
	BOOLEAN AllMeanwhileCutscene;
	BOOLEAN CreatureMeanwhileCutscene;

	// [ASD]
	UINT8 usASDSupplyArrivalSectorX;
	UINT8 usASDSupplyArrivalSectorY;

	// [Enemy Heli]
	UINT8 usEnemyHeliBaseSectorX;
	UINT8 usEnemyHeliBaseSectorY;
	INT32 sEnemyHeliBaseParkGridno[2];
	INT32 sEnemyHeliBaseParkTileIndex;

}MOD_SETTINGS;

// silversurfer: item property modifiers
typedef struct
{
	// -------------- COMMON MODIFIERS ----------------
	FLOAT fBestLaserRangeModifier;
	UINT16 usBasicDeprecateChance;

	// -------------- WEAPON MODIFIERS ----------------
	// weapon overheating modifiers
	FLOAT fOverheatJamThresholdModifierGun[9];
	FLOAT fOverheatDamageThresholdModifierGun[9];
	FLOAT fOverheatTemperatureModifierGun[9];
	FLOAT fOverheatCooldownModifierGun[9];
	FLOAT fOverheatJamThresholdModifierLauncher;
	FLOAT fOverheatDamageThresholdModifierLauncher;
	FLOAT fOverheatTemperatureModifierLauncher;
	FLOAT fOverheatCooldownModifierLauncher;
	// range modifiers
	FLOAT fRangeModifierGun[9];
	FLOAT fRangeModifierLauncher;
	FLOAT fRangeModifierThrowingKnife;
	FLOAT fRangeModifierGrenade;

	// damage modifiers
	FLOAT fDamageModifierGun[GUN_TYPES_MAX];
	FLOAT fDamageModifierBlade;
	FLOAT fDamageModifierPunch;
	FLOAT fDamageModifierTentacle;
	// AP modifiers for single attacks (ubShotsPer4Turns)
	FLOAT fShotsPer4TurnsModifierGun[9];
	FLOAT fShotsPer4TurnsModifierLauncher;
	FLOAT fShotsPer4TurnsModifierBlade;
	FLOAT fShotsPer4TurnsModifierPunch;
	FLOAT fShotsPer4TurnsModifierTentacle;
	FLOAT fShotsPer4TurnsModifierThrowKnife;
	// Burst AP modifiers
	FLOAT fBurstAPModifierGun[GUN_TYPES_MAX];
	// AP modifiers for Auto Fire Shots per 5 AP (bAutofireShotsPerFiveAP)
	FLOAT fAFShotsPer5APModifierGun[GUN_TYPES_MAX];
	// AP to Reload modifiers
	FLOAT fAPtoReloadModifierGun[GUN_TYPES_MAX];
	FLOAT fAPtoReloadModifierLauncher;
	// AP to Reload manually modifiers
	FLOAT fAPtoReloadManuallyModifierGun[GUN_TYPES_MAX];
	FLOAT fAPtoReloadManuallyModifierLauncher;
	// Max Distance for messy shot modifiers
	FLOAT fDistMessyModifierGun[GUN_TYPES_MAX];
	// Handling modifiers
	FLOAT fHandlingModifierGun[GUN_TYPES_MAX];
	FLOAT fHandlingModifierLauncher;
	// Recoil modifiers
	FLOAT fRecoilXModifierGun[GUN_TYPES_MAX];
	FLOAT fRecoilYModifierGun[GUN_TYPES_MAX];
	// Jam damage thresholds
	UINT16 usJamDamageThresholdGun[GUN_TYPES_MAX];

	// -------------- ARMOR MODIFIERS ----------------
	FLOAT fCamoLBEoverVestModifier;
	FLOAT fCamoLBEoverPantsModifier;

	// ------------ EXPLOSIVE MODIFIERS --------------
	// Grenade Damage Modifiers
	FLOAT fDamageHealthModifierExplosive;
	FLOAT fDamageBreathModifierExplosive;
	FLOAT fDamageHealthMoveModifierExplosive;	// applies when character moves through gas cloud without a gas mask
	FLOAT fDamageBreathMoveModifierExplosive;	// applies when character moves through gas cloud without a gas mask
	UINT8 usMissMaxRadiusGrenade;
	UINT8 usMissMaxRadiusMortar;

	// ------------ MISC MODIFIERS --------------
	// Flugente
	FLOAT fShieldMovementAPCostModifier;		// movement AP costs are multiplied with this while a shield is equipped
	FLOAT fDragAPCostModifier;

}ITEM_SETTINGS;

extern MOD_SETTINGS gModSettings;

extern ITEM_SETTINGS gItemSettings;

//This structure will contain general Ja2 settings	NOT individual game settings.
extern GAME_SETTINGS		gGameSettings;

//This structure will contain the Game options set at the beginning of the game.
extern GAME_OPTIONS		gGameOptions;

// Snap: Options read from an INI file in the default of custom Data directory
extern GAME_EXTERNAL_OPTIONS gGameExternalOptions; 

extern SKILL_TRAIT_VALUES gSkillTraitValues;  // SANDRO - added this one

extern TAUNTS_SETTINGS gTauntsSettings;

extern HELICOPTER_SETTINGS gHelicopterSettings;

extern MORALE_SETTINGS gMoraleSettings;

extern REPUTATION_SETTINGS gReputationSettings;

extern CREATURES_SETTINGS gCreaturesSettings;

// HEADROCK HAM 4: CTH constants read from a separate INI file
extern CTH_CONSTANTS gGameCTHConstants;

// WDS - Automatically try to save when an assertion failure occurs
extern bool alreadySaving;

BOOLEAN	SaveGameSettings();
BOOLEAN LoadGameSettings();
// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions();
void LoadSkillTraitsExternalSettings(); // SANDRO - added this one
void LoadModSettings();
void LoadItemSettings(); // silversurfer: reads item property modifiers
void LoadGameAPBPConstants();
// HEADROCK HAM 4: Read CTH/Shooting coefficients from file
void LoadCTHConstants();
void LoadTauntsSettings();
void LoadHelicopterRepairRefuelSettings();
void LoadMoraleSettings();
void LoadReputationSettings();
void LoadCreaturesSettings();
void FreeGameExternalOptions();

void InitGameOptions();

void InitGameSettings();

BOOLEAN GetCDLocation( );

void DisplayGameSettings( );

BOOLEAN MeanwhileSceneSeen( UINT8 ubMeanwhile );

BOOLEAN SetMeanwhileSceneSeen( UINT8 ubMeanwhile );

BOOLEAN	CanGameBeSaved();

INT16 DynamicAdjustAPConstants(INT16 iniReadValue, INT16 iniDefaultValue, BOOLEAN reverse = FALSE);

#endif
