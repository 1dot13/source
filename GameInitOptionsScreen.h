#ifndef _GAME_INIT_OPTIONS_SCREEN_H_
#define _GAME_INIT_OPTIONS_SCREEN_H_


UINT32	GameInitOptionsScreenInit( void );
UINT32	GameInitOptionsScreenHandle( void );
UINT32	GameInitOptionsScreenShutdown( void );

BOOLEAN	SpIniExists();

// Flugente: moved enums
//Enums for the difficulty levels
enum
{
	DIF_LEVEL_EASY = 1,
	DIF_LEVEL_MEDIUM,
	DIF_LEVEL_HARD,
	DIF_LEVEL_INSANE,
	MAX_DIF_LEVEL = 15,
};

typedef struct
{
	UINT16		uiIndex;
	CHAR16	szDiffName[128];
	CHAR16 szConfirmText[255];
	CHAR16	szPrefixXmlName[128];
	
	INT32 iStartingCash;
	INT8 iEnemyAPBonus;
	UINT16 iNumKillsPerProgressPoint;
	INT32 iInitialGarrisonPercentages;
	UINT8 iMinEnemyGroupSize;
	BOOLEAN bUnlimitedPoolOfTroops;
	INT32 iQueensInitialPoolOfTroops;
	UINT32 iQueenPoolIncrementPerDifficultyLevel;
	UINT32 iEnemyStartingAlertLevel;
	UINT32 iEnemyAlertDecay;
	UINT32 iNumAwareBattles;
	UINT32 iBaseDelayInMinutesBetweenEvaluations;
	UINT32 iEvaluationDelayVariance;
	UINT32 iGracePeriodInHoursAfterSectorLiberation;
	UINT32 iGracePeriodInDaysAfterPatrolDestroyed;
	BOOLEAN bAggressiveQueenAi;
	INT32 iMaxMercDeaths;
	
	//Creatures Settings
	UINT16 iCreatureSpreadTime;
	UINT8 iQueenReproductionBase;
	UINT8 iQueenReproductionBonus;
	UINT8 iQueenInitBonusSpread;
	INT8 iCreaturePopulationModifier;
	INT8 iCreatureTownAggressiveness;
	
	//sectors
	UINT8 iJ9B1NumTroops;
	UINT8 iJ9B2NumCreatures;
	UINT8 iK4B1NumTroops;
	UINT8 iK4B1NumElites;
	UINT8 iO3B1NumTroops;
	UINT8 iO3B1NumElites;
	UINT8 iP3B1NumElites;
	
	BOOLEAN bStrategicAiActionWakeQueen;
	UINT16 iUpdateLastDayOfPlayerActivity;
	INT32 iChanceOfEnemyAmbushes;
	BOOLEAN bAllowReinforcements;
	BOOLEAN bAllowReinforcementsOmerta;
	INT32 iPercentElitesBonus;
	BOOLEAN iDesiredPopulationL3;
	BOOLEAN iDesiredPopulationL2;
	UINT8 iWeaponCacheTroops1;
	UINT8 iWeaponCacheTroops2;
	UINT8 iWeaponCacheTroops3;
	UINT8 iWeaponCacheTroops4;
	UINT8 iWeaponCacheTroops5;
	BOOLEAN bUpgradeAdminsToTroops;
	BOOLEAN bUpgradeAdminsToTroops2;
	BOOLEAN bQueenLosingControlOfSector;
	BOOLEAN bBloodcatAmbush;
	BOOLEAN bAirRaidLookForDive;
	UINT32 iGetNumberOfTurnsPowerGenFanWillBeStoppedFor; //UB
	BOOLEAN bRadioSightings;
	BOOLEAN bRadioSightings2;
	
	FLOAT NewDifficultySettingsBASE_DIFFICULTY;
	FLOAT NewDifficultySettingsAIM_DIFFICULTY;
	
	BOOLEAN bEnemyMoraleWorried;
	
} DIFFICULTY_SETTINGS_VALUES;
extern UINT8 MaxDifficultySettingsValues; 
extern DIFFICULTY_SETTINGS_VALUES zDiffSetting[MAX_DIF_LEVEL];

#endif
