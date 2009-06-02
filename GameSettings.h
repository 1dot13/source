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
	TOPTION_USE_RANDOM_PERSONALITY,
	TOPTION_USE_AUTO_SAVE,
	TOPTION_SILENT_SKYRIDER,
	TOPTION_LOW_CPU_USAGE,
	TOPTION_ENHANCED_DESC_BOX,
	
	// arynn
	TOPTION_TOGGLE_TURN_MODE,					
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

	INT8 iCustomPersonality;
	INT8 iCustomAttitude;

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
	UINT32	guiMinMilitiaSquadSize;
	UINT32	guiMaxMilitiaSquadSize;
	UINT32	guiMaxMilitiaSquadSizeBattle;

	BOOLEAN gfAllowSoldierToolTips;

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
	UINT32 ubIMPStartingLevel;

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
	INT32 ubGunRangeTrainingBonus;
	INT32 ubTownMilitiaTrainingRate;
	INT32 ubMaxMilitiaTrainersPerSector;
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
	INT32 fEnhancedDescriptionBox;

	// WANNE: Always use "prof.dat".
	BOOLEAN fAlwaysUseProfDat;

	// HEADROCK HAM B1: Global modifier for mine income (0 is automatically turned to 1 to avoid divide-by-zero.)
	INT16 iMineIncomePercentage;

	// HEADROCK HAM B1: Allows changing the maximum CTH possible, for any attack 0-100
	UINT8 iMaximumCTH;
	
	// HEADROCK HAM B1: Allows changing the minimum CTH possible, for any attack 0-100
	UINT8 iMinimumCTH;

	// HEADROCK HAM B1: Allows reducing the chance of scoring a hit fractionally, if CTH = Minimum CTH
	UINT8 iMinimumCTHDivisor;

	// HEADROCK HAM B1: Allows Restricted Roaming Militia to move through previously visited sectors, even if restricted.
	BOOLEAN bUnrestrictVisited;

	// HEADROCK HAM B1: Allows the capture of towns to dynamically alter roaming restrictions
	BOOLEAN bDynamicRestrictRoaming;

	// HEADROCK HAM B2: Reset suppression counter. 0 = never (oldskool), 1 = Every turn, 2 = Every attack.
	UINT8 iClearSuppression;

	// HEADROCK HAM B2.1: This controls how effective suppression is, by increasing the number of ubSuppressionPoints accumulated by combatants (percentage);
	INT16 iSuppressionEffectiveness;

	// HEADROCK HAM B2: MAXIMUM number of APs that can be lost to suppression in a given turn (0=unlimited)
	BOOLEAN fSuppressionAPLossPerTurn;

	// HEADROCK HAM B2: MAXIMUM number of APs that can be lost to suppression in a given attack (0=unlimited)
	BOOLEAN fSuppressionAPLossPerAttack;

	UINT8 iSuppressionToleranceMax;
	UINT8 iSuppressionToleranceMin;

	// HEADROCK HAM B2: Suppression Shock ON/OFF
	BOOLEAN fSuppressionShock;

	// HEADROCK HAM B2: Suppression Shock effectiveness (percentage, 100 = "normal", 0 = deactivated. Range 0-65535)
	UINT16 iSuppressionShockEffectiveness;

	// HEADROCK HAM B2.1: CTH penalty given by a "Cowering" target to any enemy shooter.
	INT8 iAimPenaltyPerTargetShock;

	// HEADROCK HAM B2.3: It is now possible for "cowering" to increase a target's suppression.
	UINT16 iCowerEffectOnSuppression;

	// HEADROCK HAM B2.5: Turn on Realistic Tracers. 0 = off (regular tracers). 1 = Fully realistic tracers. 2 = Tracer Bump + 1.13 Autofire Penalty Reduction
	UINT8 iRealisticTracers;

	// HEADROCK HAM B2.5: Realistic tracers - one of every X bullets in a tracer magazines will be a tracer bullet. 0 = off (JA2 normal)
	UINT8 iNumBulletsPerTracer;

	// HEADROCK HAM B2.5: Realistic tracers - CTH increased by this amount whenever a tracer is fired. 0 = off.
	INT16 iCTHBumpPerTracer;

	// HEADROCK HAM B2.6: Increased aiming costs?
	BOOLEAN fIncreasedAimingCost;

	// HEADROCK HAM B2.6: Dynamically determine Max-Aiming based also on weapon, bipod, etc?
	BOOLEAN fDynamicAimingTime;

	// HEADROCK HAM B2.6: Controls how much effect target movement has on aiming
	FLOAT iMovementEffectOnAiming;

	// HEADROCK HAM B2.6: Autofire Bullets/5AP modifier
	INT16 iAutofireBulletsPer5APModifier;

	// HEADROCK HAM B2.6/B1: Adjustable "luck" factor in Auto-Resolve
	FLOAT iAutoResolveLuckFactor;

	// HEADROCK HAM B2.6: Adjustable maximum for Suppression Shock effect. This has the added effect of reducing overall susceptibility to shock and may render some enemies unshockable.
	UINT8 iMaxSuppressionShock;

	// HEADROCK HAM B2.6/2/1: Toggle new Burst/Auto CTH bars: 0=neither, 1=both, 2=Burst, 3=Auto
	UINT8 iNewCTHBars;

	// HEADROCK HAM B2.6: Toggle whether AI checks for larger magazine when wanting to suppress at a distance
	BOOLEAN fIncreaseAISuppressionFire;

	// HEADROCK HAM B2.7: Change the speed of skill progression. (defaults set to JA2 normal)
	UINT16 ubHealthSubpointsToImprove;
	UINT16 ubStrengthSubpointsToImprove;
	UINT16 ubDexteritySubpointsToImprove;
	UINT16 ubAgilitySubpointsToImprove;
	UINT16 ubWisdomSubpointsToImprove;
	UINT16 ubMarksmanshipSubpointsToImprove;
	UINT16 ubMedicalSubpointsToImprove;
	UINT16 ubMechanicalSubpointsToImprove;
	UINT16 ubExplosivesSubpointsToImprove;
	UINT16 ubLeadershipSubpointsToImprove;
	UINT16 ubLevelSubpointsToImprove;

	// HEADROCK HAM B2.7: When turned on, this will give a CTH approximation instead of an exact value, on CTH Bars and "F" key feedback.
	BOOLEAN fApproximateCTH;

	// HEADROCK HAM B2.7: Augmented Roaming Militia code - turn this to TRUE to allow militia free travel through San Mona, Tixa, Orta, Omerta, and Estoni.
	BOOLEAN fAllowMilitiaMoveThroughMinorCities;

	// HEADROCK HAM B2.7: Smarter Roaming Militia Generator will generate Roamers in any available city perimeter square, allowing us to avoid "wasting" training sessions more easily.
	BOOLEAN fSmartRoamingMilitiaGenerator;

	// HEADROCK HAM B2.7: When "TRUE", Roaming Militia will be created on a 25% Veteran, 25% Regular, 50% green basis. If new militia is placed into a full group, it will upgrade greens/regulars in that group. 
	BOOLEAN fDiverseRoamingMilitiaGroups;

	// HEADROCK HAM B2.8: This setting controls whether militia will try to join up into the largest groups they can, or average out the size of the groups to cover more territory.
	UINT8 ubRoamingMilitiaSpreadsOutChance;

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

	// The_Bob - real time sneaking code 01/06/09
	// Suport disabling/silencing real time sneaking via external .ini file
	BOOLEAN fAllowRealTimeSneak;
	BOOLEAN fQuietRealTimeSneak;
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
