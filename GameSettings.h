#ifndef _GAME_SETTINGS__H_
#define _GAME_SETTINGS__H_

#include "Types.h"


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
	TOPTION_DROP_ALL,
	TOPTION_GL_HIGH_ANGLE,
	TOPTION_AIM_LEVEL_RESTRICTION,

	//lalien
	TOPTION_SPACE_SELECTS_NEXT_SQUAD,
	TOPTION_SHOW_ITEM_SHADOW,
	TOPTION_SHOW_WEAPON_RANGE_IN_TILES,
	TOPTION_TRACERS_FOR_SINGLE_FIRE,
	TOPTION_RAIN_SOUND,
	TOPTION_ALLOW_CROWS,
	TOPTION_ALLOW_SOLDIER_TOOLTIPS, // Changed from TOPTION_USE_RANDOM_PERSONALITY - SANDRO
	TOPTION_USE_AUTO_SAVE,
	TOPTION_SILENT_SKYRIDER,
	TOPTION_LOW_CPU_USAGE,
	TOPTION_ENHANCED_DESC_BOX,
	
	// arynn
	TOPTION_TOGGLE_TURN_MODE,					

	// HEADROCK HAM 3.6:
	TOPTION_STAT_PROGRESS_BARS,

	// arynn: Debug/Cheat
	TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	TOPTION_FORCE_BOBBY_RAY_SHIPMENTS,			// force all pending Bobby Ray shipments
	TOPTION_CHEAT_MODE_OPTIONS_END,
	TOPTION_DEBUG_MODE_OPTIONS_HEADER,			// an example options screen options header (pure text)                                                         
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
	INT8				bLastSavedGameSlot;					// The last saved game number goes in here	

	UINT8				ubMusicVolumeSetting;               // Volume Setting
	UINT8				ubSoundEffectsVolume;               // Volume Setting
	UINT8				ubSpeechVolume;                     // Volume Setting

	//The following are set from the status of the toggle boxes in the Options Screen
	BOOLEAN				fOptions[ NUM_ALL_GAME_OPTIONS + 1 ];	// Toggle Options (Speech, Subtitles, Show Tree Tops, etc.. )

	UINT32				uiMeanwhileScenesSeenFlags;         // Bit Vector describing seen 'mean whiles..' (not sure why this is in Game Settings )

	BOOLEAN				fHideHelpInAllScreens;              // Controls Help "do not show help again" checkbox

	UINT8				ubSizeOfDisplayCover;               // The number of grids the player designates thru "Delete + '=' or '-'"
	UINT8				ubSizeOfLOS;                        // The number of grids the player designates thru "End    + '=' or '-'"

}	GAME_SETTINGS;

// CHRISL: New Enums to track inventory system
enum
{
	INVENTORY_OLD = 0,
	INVENTORY_NEW = 1
};


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
	UINT8	ubInventorySystem;
	UINT8	ubFiller[6];

} GAME_OPTIONS;

bool UsingNewInventorySystem();
BOOLEAN IsNIVModeValid(bool checkRes = true);

// Snap: Options read from an INI file in the default of custom Data directory
typedef struct
{
	INT32	iStartingCashNovice;
	INT32	iStartingCashExperienced;
	INT32	iStartingCashExpert;
	INT32	iStartingCashInsane;

	UINT32 iGameStartingTime; //Lalien: game starting time	
	UINT32 iFirstArrivalDelay; 

	BOOLEAN fSellAll;
	INT16 iPriceModifier;

	// WDS: Allow flexible numbers of IMPs of each sex
	INT32 iIMPMaleCharacterCount;	// Count of how many there are
	INT32 iIMPFemaleCharacterCount;
	INT32 iMaxIMPCharacters;		// Limit of how many to allow
	//
	// iaIMPSlots is an array of the slots (in prof.dat) to use for IMPs.
	//
	INT32 *iaIMPSlots;

	INT32 iMinAttribute;
	INT32 iMaxAttribute;
	INT32 iImpAttributePoints;
	INT32 iMaxZeroBonus;
	INT32 iStartAttribute;

	// These 3 added - SANDRO
	INT32 iIMPStartingLevelCostMultiplier;
	INT32 iBonusPointsForDisability;
	INT32 iBonusPointsPerSkillNotTaken;
	INT32 iMaxMilitiaPerSector;
	INT32 iTrainingSquadSize;
	INT32 iMilitiaTrainingCost;
	INT32 iMilitiaCostModifier;
	INT32 iRegularCostModifier;
	INT32 iVeteranCostModifier;
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
	INT32 iPercentElitesBonusExperienced;

	UINT8 ubMinEnemyGroupSizeNovice;
	UINT8 ubMinEnemyGroupSizeExperienced;
	UINT8 ubMinEnemyGroupSizeExpert;
	UINT8 ubMinEnemyGroupSizeInsane;

	BOOLEAN fMercDayOne;
	BOOLEAN fAllMercsAvailable;

	//BOOLEAN fPers_att;

	// These 2 removed - SANDRO
	//INT8 iCustomPersonality;
	//INT8 iCustomAttitude;

	INT8 iEasyAPBonus;
	INT8 iExperiencedAPBonus;
	INT8 iExpertAPBonus;
	INT8 iInsaneAPBonus;
	INT8 iPlayerAPBonus;
	// Kaiden: Vehicle Inventory change - Added for INI Option
	BOOLEAN fVehicleInventory;

	BOOLEAN fEnableChanceOfEnemyAmbushesOnInsaneDifficult;

	// System settings
	UINT8 gubDeadLockDelay;
	BOOLEAN gfEnableEmergencyButton_SkipStrategicEvents;


	//Video settings	
	BOOLEAN gfVSync;

	//Animation settings
	UINT32 gubPlayerTurnSpeedUpFactor;
	UINT32 gubEnemyTurnSpeedUpFactor;
	UINT32 gubCreatureTurnSpeedUpFactor;
	UINT32 gubMilitiaTurnSpeedUpFactor;
	UINT32 gubCivilianTurnSpeedUpFactor;

	//Sound settings
	UINT32 guiWeaponSoundEffectsVolume;

	// WDS - Option to turn off stealing
	BOOLEAN fStealingDisabled;

	// WANNE: Externalize chance of shipment package lost
	UINT32	gubChanceOfShipmentLost;

	// Militia Settings	
	BOOLEAN fAllowTacticalMilitiaCommand;
	BOOLEAN gfTrainVeteranMilitia;
	BOOLEAN gfAllowMilitiaGroups;
	BOOLEAN gfmusttrainroaming;
	BOOLEAN gflimitedRoaming;
	BOOLEAN gfAllowReinforcements;
	BOOLEAN gfAllowReinforcementsOnlyInCity;
	UINT32	guiBaseQueenPoolIncrement;
	
	UINT32	guiAllowMilitiaGroupsDelay;
	UINT32	guiTrainVeteranMilitiaDelay;
	UINT32	guiCreateEachNHours;
	UINT32	guiDivOfOriginalMilitia;
	// HEADROCK HAM 3.6: These settings are REDUNDANT.
	//UINT32	guiMinMilitiaSquadSize;
	//UINT32	guiMaxMilitiaSquadSize;
	//UINT32	guiMaxMilitiaSquadSizeBattle;

	//BOOLEAN gfAllowSoldierToolTips; // moved to prefereces - SANDRO

	// WDS - Improve Tony's and Devin's inventory like BR's
	BOOLEAN tonyUsesBRSetting;
	BOOLEAN devinUsesBRSetting;

	// WDS - Smart goggle switching
	BOOLEAN smartGoggleSwitch;
	
	// WDS - Automatically flag mines
	BOOLEAN automaticallyFlagMines;

	// WDS - Automatically try to save when an assertion failure occurs
	BOOLEAN autoSaveOnAssertionFailure;
	UINT32  autoSaveTime;

	//Sight range
	UINT32	ubStraightSightRange;
	UINT32 ubVisDistDecreasePerRainIntensity;
	BOOLEAN gfAllowLimitedVision;

	BOOLEAN gfShiftFUnloadWeapons;
	BOOLEAN gfShiftFRemoveAttachments;

	// Rain settings
	BOOLEAN gfAllowRain;
	UINT32 gusRainChancePerDay;
	UINT32 gusRainMinLength;
	UINT32 gusRainMaxLength;
	UINT32 guiMaxRainDrops;
	UINT32 ubWeaponReliabilityReductionPerRainIntensity;
	double ubBreathGainReductionPerRainIntensity;

	// Thunder settings
	BOOLEAN gfAllowLightning;
	UINT32 guiMinLightningInterval;
	UINT32 guiMaxLightningInterval;
	UINT32 guiMinDLInterval;
	UINT32 guiMaxDLInterval;
	UINT32 guiProlongLightningIfSeenSomeone;
	UINT32 guiChanceToDoLightningBetweenTurns;

	// WDS - Progress settings
	UINT32 ubGameProgressPortionKills;
	UINT32 ubGameProgressPortionControl;
	UINT32 ubGameProgressPortionIncome;
	UINT32 ubGameProgressPortionVisited;
	UINT32 ubGameProgressMinimum;
	UINT32 ubGameProgressIncrement;

	// WDS - Advanced start 
	//UINT32 ubIMPStartingLevel; // removed - SANDRO

	// Event settings
	UINT32 ubGameProgressStartMadlabQuest;
	UINT32 ubGameProgressMikeAvailable;
	UINT32 ubGameProgressIggyAvaliable;
	BOOLEAN ubSendTroopsToDrassen;

	// WDS - make number of mercenaries, etc. be configurable
	// group sizes
	UINT32 ubGameMaximumNumberOfPlayerMercs;
	UINT32 ubGameMaximumNumberOfPlayerVehicles;
	UINT32 ubGameMaximumNumberOfEnemies;
	UINT32 ubGameMaximumNumberOfCreatures;
	UINT32 ubGameMaximumNumberOfRebels;
	UINT32 ubGameMaximumNumberOfCivilians;

	//Gameplay settings

	UINT32 ubExplosivesDamageMultiplier;
	UINT32 ubGunDamageMultiplier;
	UINT32 ubMeleeDamageMultiplier;

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

	// WDS - New AI
	BOOLEAN useNewAI;

	BOOLEAN gfEasyUnlimitedTroops;
	BOOLEAN gfNormalUnlimitedTroops;
	BOOLEAN gfHardUnlimitedTroops;
	BOOLEAN gfInsaneUnlimitedTroops;

	BOOLEAN gfEasyAggressiveQueen;
	BOOLEAN gfNormalAggressiveQueen;
	BOOLEAN gfHardAggressiveQueen;
	BOOLEAN gfInsaneAggressiveQueen;

	BOOLEAN gfInvestigateSector;
	BOOLEAN gfReassignPendingReinforcements;

	INT32 ubEnemiesItemDrop;

	BOOLEAN	gfUseExternalLoadscreens;

	BOOLEAN gfUseAutoSave;

	//Merc Assignment settings
	INT32 ubAssignmentUnitsPerDay;
	INT32 ubMinutesForAssignmentToCount;

	INT32 ubTrainingSkillMin;
	INT32 ubTrainingSkillMax;
	INT32 ubSelfTrainingDivisor;
	INT32 ubInstructedTrainingDivisor;
	// HEADROCK HAM 3.5: No longer necessary.
	//INT32 ubGunRangeTrainingBonus;
	INT32 ubTownMilitiaTrainingRate;
	// HEADROCK HAM 3.5: No longer necessary.
	//INT32 ubMaxMilitiaTrainersPerSector;
	INT32 ubTeachBonusToTrain;
	INT32 ubRpcBonusToTrainMilitia;
	INT32 ubMinSkillToTeach;

	INT32 ubLowActivityLevel;
	INT32 ubMediumActivityLevel;
	INT32 ubHighActivityLevel;
	INT32 ubDoctoringRateDivisor;
	INT32 ubHospitalHealingRate;
	INT32 ubBaseMedicalSkillToDealWithEmergency;
	INT32 ubMultiplierForDifferenceInLifeValueForEmergency;
	INT32 ubPointCostPerHealthBelowOkLife;//OKLIFE = 15

	INT32 ubRepairCostPerJam;
	INT32 ubRepairRateDivisor;

	//Misc settings
	BOOLEAN fAmmoDynamicWeight; //Pulmu
	BOOLEAN fEnableCrepitus;
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
	INT32 giEasyMercDeaths;
	INT32 giExperiencedMercDeaths;
	INT32 giExpertMercDeaths;
	INT32 giInsaneMercDeaths;

	// Lesh: slow enemy items choice progress
	BOOLEAN fSlowProgressForEnemyItemsChoice;

//afp - use bullet tracers
BOOLEAN gbBulletTracer;

	// CHRISL: option to allow Slay to remain as a hired PC
	BOOLEAN fEnableSlayForever;

	// CHRISL: Setting to turn off the description and stack popup options from the sector inventory panel
	BOOLEAN fSectorDesc;

	// WANNE: Restrict female enemies from beeing in the queens army (except Female Elite enemies)?
	BOOLEAN fRestrictFemaleEnemiesExceptElite;

	// HEADROCK: Enhanced Item Description Box ON/OFF
	// WANNE: Changed from BOOLEAN to INT32!
	INT32 iEnhancedDescriptionBox;

	// WANNE: Always use "prof.dat".
	BOOLEAN fAlwaysUseProfDat;

	// CPT: Cover System Settings
	UINT8 ubStealthTraitCoverValue;
	UINT8 ubStealthEffectiveness;
	UINT8 ubTreeCoverEffectiveness;
	UINT8 ubCamouflageEffectiveness;
	UINT8 ubStanceEffectiveness;
	UINT8 ubLBEEffectiveness;
	UINT8 ubMovementEffectiveness;
	UINT8 ubCoverDisplayUpdateWait;

	// HEADROCK HAM B1: Global modifier for mine income (0 is automatically turned to 1 to avoid divide-by-zero.)
	INT16 usMineIncomePercentage;

	// HEADROCK HAM B1: Allows changing the maximum CTH possible, for any attack 0-100
	UINT8 ubMaximumCTH;
	
	// HEADROCK HAM B1: Allows changing the minimum CTH possible, for any attack 0-100
	UINT8 ubMinimumCTH;

	// HEADROCK HAM B1: Allows reducing the chance of scoring a hit fractionally, if CTH = Minimum CTH
	UINT16 usMinimumCTHDivisor;

	// HEADROCK HAM B1: Allows Restricted Roaming Militia to move through previously visited sectors, even if restricted.
	BOOLEAN fUnrestrictVisited;

	// HEADROCK HAM B1: Allows the capture of towns to dynamically alter roaming restrictions
	BOOLEAN fDynamicRestrictRoaming;

	// HEADROCK HAM B2.1: This controls how effective suppression is, by increasing the number of ubSuppressionPoints accumulated by combatants (percentage);
	INT16 sSuppressionEffectiveness;

	UINT8 ubSuppressionToleranceMax;
	UINT8 ubSuppressionToleranceMin;

	// HEADROCK HAM B2: Suppression Shock effectiveness (percentage, 100 = "normal", 0 = deactivated. Range 0-65535)
	UINT16 usSuppressionShockEffect;

	// HEADROCK HAM B2.1: CTH penalty given by a "Cowering" target to any enemy shooter.
	UINT8 ubAimPenaltyPerTargetShock;

	// HEADROCK HAM B2.3: It is now possible for "cowering" to increase a target's suppression.
	UINT16 usCowerEffectOnSuppression;

	// HEADROCK HAM B2.5: Turn on Realistic Tracers. 0 = off (regular tracers). 1 = Fully realistic tracers. 2 = Tracer Bump + 1.13 Autofire Penalty Reduction
	UINT8 ubRealisticTracers;

	// HEADROCK HAM B2.5: Realistic tracers - one of every X bullets in a tracer magazines will be a tracer bullet. 0 = off (JA2 normal)
	UINT8 ubNumBulletsPerTracer;

	// HEADROCK HAM B2.5: Realistic tracers - CTH increased by this amount whenever a tracer is fired. 0 = off.
	UINT8 ubCTHBumpPerTracer;

	// HEADROCK HAM B2.6: Increased aiming costs?
	BOOLEAN fIncreasedAimingCost;

	// HEADROCK HAM B2.6: Dynamically determine Max-Aiming based also on weapon, bipod, etc?
	BOOLEAN fDynamicAimingTime;

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

	// HEADROCK HAM B2.6: Toggle whether AI checks for larger magazine when wanting to suppress at a distance
	BOOLEAN fIncreaseAISuppressionFire;

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

	// HEADROCK HAM B2.7: Augmented Roaming Militia code - turn this to TRUE to allow militia free travel through San Mona, Tixa, Orta, Omerta, and Estoni.
	BOOLEAN fAllowMilitiaMoveThroughMinorCities;

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

	// HEADROCK HAM 3: Four INI settings to control the number of kills you must accumulate to win one progress point, one setting per difficulty level
	UINT16 usNumKillsPerProgressPointNovice;
	UINT16 usNumKillsPerProgressPointExperienced;
	UINT16 usNumKillsPerProgressPointExpert;
	UINT16 usNumKillsPerProgressPointInsane;

	// HEADROCK HAM 3: If enabled, changes the way the game calculates progress. It will look at each progress control separately (Kills/Income/Control/Visited) and set the progress to the HIGHEST scoring control.
	BOOLEAN fAlternateProgressCalculation;

	// HEADROCK HAM 3: If enabled, tooltipping over Bobby Ray's weapons will show a list of possible attachments to those weapons.
	BOOLEAN fBobbyRayTooltipsShowAttachments;

	// HEADROCK HAM 3.1: Divisor for the AP-to-Ready cost charge on first aiming click, when extra aiming costs are enabled. 0 = No ready-time-based charge.
	UINT8 ubFirstAimReadyCostDivisor;

	// HEADROCK HAM 3.1: This is a suppression tool that not everyone will like. It gives an on-screen message when any character has been suppressed so much he's lost his next turn completely!
	BOOLEAN fShowSuppressionShutdown;

	// HEADROCK HAM 3.1: Set whether you'd like to determine yourself which mine (if any) will shut down during the campaign.
	BOOLEAN fManuallySelectMineShutdown;

	// HEADROCK HAM 3.1: Select which mine will run out. 0 = no mine. 1 = San Mona (unused), 2 = Drassen, 3 = Alma, 4 = Cambria, 5 = Chitzena, 6 = Grumm.
	UINT8 ubWhichMineRunsOut;

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

	// HEADROCK HAM 3.3: New militia feature, Minimum Leadership required to train Roaming Militia
	UINT8 ubMinimumLeadershipToTrainMobileMilitia;

	// HEADROCK HAM 3.3: If enabled, the trainer's "effective" leadership skill determines HOW MANY militia he/she creates per session.
	BOOLEAN fLeadershipAffectsMobileMilitiaQuantity;

	// HEADROCK HAM 3.3: If "LEADERSHIP_AFFECTS_MILITIA_QUANTITY" is true, this value determines the lowest leadership required to train a full (default size 10) squad of town militia in one training session.
	UINT8 ubReqLeadershipForFullMobileTraining;

	// HEADROCK HAM 3.3: Minimum distance (in METERS) at which character suffer from friendly suppression.
	UINT16 usMinDistanceFriendlySuppression;

	// HEADROCK HAM 3.3: If enabled, Roaming Militia can automatically reinforce city garrisons. 
	BOOLEAN fAllowMobileReinforceCities;

	// HEADROCK HAM 3.3: If enabled, Roaming Militia can automatically reinforce city garrisons. 
	BOOLEAN fAllowMobileReinforceSAM;

	// HEADROCK HAM 3.4: This controls the intensity of Hiding the Bullet Count during combat. The higher it is, the more intense the effect. Negative values reduce the effect.
	UINT16 usBulletHideIntensity;

	// HEADROCK HAM 3.4: What percentage of a new Mobile Militia group will be made of Elites? If >0, then at least one of every group will be an Elite. 100 = All Elites.
	UINT8 ubPercentRoamingMilitiaElites;

	// HEADROCK HAM 3.4: What percentage of a new Mobile Militia group will be made of Elites? If >0, then at least one of every group will be an Elite. 100 = All Elites.
	UINT8 ubPercentRoamingMilitiaRegulars;

	// HEADROCK HAM 3.5: Does leadership affect the Quality of new Mobile Militia groups?
	BOOLEAN fLeadershipAffectsMobileMilitiaQuality;

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
	UINT16 usDailyCostTownGreen;
	UINT16 usDailyCostTownRegular;
	UINT16 usDailyCostTownElite;
	UINT16 usDailyCostMobileGreen;
	UINT16 usDailyCostMobileRegular;
	UINT16 usDailyCostMobileElite;

	// HEADROCK HAM 3.6: Non-Combat Bodytypes can't become hostile
	BOOLEAN fCanTrueCiviliansBecomeHostile;

	// HEADROCK HAM 3.6: Militia become hostile when attacked. 0 = No. 1 = If killed. 2 = If attacked (JA2 Default)
	UINT8 ubCanMilitiaBecomeHostile;

	// HEADROCK HAM 3.6: If activated, the game does not switch focus to a merc who spots an enemy in real-time mode. This fixes issues with Real-Time Sneak.
	BOOLEAN fNoAutoFocusChangeInRealtimeSneak;

	// The_Bob - real time sneaking code 01/06/09
	// Suport disabling/silencing real time sneaking via external .ini file
	BOOLEAN fAllowRealTimeSneak;
	BOOLEAN fQuietRealTimeSneak;

//dnl ch51 081009 JA2 Debug Settings
	BOOLEAN fEnableInventoryPoolQ;
} GAME_EXTERNAL_OPTIONS;

//This structure will contain general Ja2 settings	NOT individual game settings.
extern GAME_SETTINGS		gGameSettings;

//This structure will contain the Game options set at the beginning of the game.
extern GAME_OPTIONS		gGameOptions;

// Snap: Options read from an INI file in the default of custom Data directory
extern GAME_EXTERNAL_OPTIONS gGameExternalOptions; 

// WDS - Automatically try to save when an assertion failure occurs
extern bool alreadySaving;

BOOLEAN	SaveGameSettings();
BOOLEAN LoadGameSettings();
// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions();
void LoadGameAPBPConstants();
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
