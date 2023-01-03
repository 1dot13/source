	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Text.h"
	#include "GameInitOptionsScreen.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	DIFFICULTY_SETTINGS_VALUES	curDifficultySettings;
	DIFFICULTY_SETTINGS_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef difficultySettingsParseData;

BOOLEAN DifficultySettings_TextOnly;

static void XMLCALL
DifficultySettingsParseDataStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	difficultySettingsParseData * pData = (difficultySettingsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "DIFFICULTY_SETTINGS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DIFFICULTY") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "Name") == 0 ||
				strcmp(name, "ConfirmText") == 0 ||	
				strcmp(name, "StartingCash") == 0 ||
				strcmp(name, "EnemyAPBonus") == 0 ||
				strcmp(name, "NumKillsPerProgressPoint") == 0 ||
				strcmp(name, "InitialGarrisonPercentages") == 0 ||
				strcmp(name, "MinEnemyGroupSize") == 0 ||
				strcmp(name, "CounterAttackGroupSize") == 0 ||
				strcmp(name, "UnlimitedPoolOfTroops") == 0 ||
				strcmp(name, "QueensInitialPoolOfTroops") == 0 ||
				strcmp(name, "QueenPoolMaxSizePerDifficultyLevel" ) == 0 ||
				strcmp(name, "QueenPoolIncrementDaysPerDifficultyLevel" ) == 0 ||
				strcmp(name, "QueenPoolBaseIncrementSizePerDifficultyLevel" ) == 0 ||
				strcmp(name, "QueenPoolRecruitPercentPerDifficultyLevel" ) == 0 ||
				strcmp(name, "EnemyStartingAlertLevel") == 0 ||
				strcmp(name, "EnemyAlertDecay") == 0 ||
				strcmp(name, "NumAwareBattles") == 0 ||
				strcmp(name, "BaseDelayInMinutesBetweenEvaluations") == 0 ||
				strcmp(name, "EvaluationDelayVariance") == 0 ||
				strcmp(name, "GracePeriodInHoursAfterSectorLiberation") == 0 ||
				strcmp(name, "GracePeriodInDaysAfterPatrolDestroyed") == 0 ||
				strcmp(name, "AggressiveQueenAi") == 0 ||
				strcmp(name, "MaxMercDeaths") == 0 ||
				strcmp(name, "LootStatusModifier") == 0 ||
				strcmp(name, "CreatureSpreadTime") == 0 ||
				strcmp(name, "QueenReproductionBase") == 0 ||
				strcmp(name, "QueenReproductionBonus") == 0 ||
				strcmp(name, "QueenInitBonusSpread") == 0 ||
				strcmp(name, "CreaturePopulationModifier") == 0 ||
				strcmp(name, "CreatureTownAggressiveness") == 0	||
				strcmp(name, "SectorJ9B1NumTroops") == 0	||
				strcmp(name, "SectorJ9B2NumCreatures") == 0	||
				strcmp(name, "SectorK4B1NumTroops") == 0	||
				strcmp(name, "SectorK4B2NumElites") == 0	||
				strcmp(name, "SectorO3B1NumTroops") == 0	||
				strcmp(name, "SectorO3B2NumElites") == 0	||
				strcmp(name, "SectorP3B1NumElites") == 0	||
				strcmp(name, "StrategicAiActionWakeQueen") == 0	||
				strcmp(name, "UpdateLastDayOfPlayerActivity") == 0	||
				strcmp(name, "ChanceOfEnemyAmbushes") == 0	||
				strcmp(name, "AllowReinforcements") == 0	||
				strcmp(name, "AllowReinforcementsOmerta") == 0	||
				strcmp(name, "PopulationLevel2") == 0	||
				strcmp(name, "PopulationLevel3") == 0	||
				strcmp(name, "PercentElitesBonus") == 0	||
				strcmp(name, "WeaponCacheTroops1") == 0	||
				strcmp(name, "WeaponCacheTroops2") == 0	||
				strcmp(name, "WeaponCacheTroops3") == 0	||
				strcmp(name, "WeaponCacheTroops4") == 0	||
				strcmp(name, "WeaponCacheTroops5") == 0	||
				strcmp(name, "UpgradeAdminsToTroops") == 0	||
				strcmp(name, "UpgradeGarrisonsAdminsToTroops") == 0	||
				strcmp(name, "AlwaysUpGradeAdminsToTroopsProgress" ) == 0 ||
				strcmp(name, "LevelModifierLowLimit" ) == 0 ||
				strcmp(name, "LevelModifierHighLimit" ) == 0 ||
				strcmp(name, "AllowUnrestrictedXPLevels" ) == 0 ||
				strcmp(name, "QueenAttackLosingControlOfSector") == 0	||
				strcmp(name, "BloodcatAmbushSectors") == 0	||
				strcmp(name, "AirRaidLookForDive") == 0	||
				strcmp(name, "NumberOfTurnsPowerGenFanWillBeStoppedFor") == 0 ||
				strcmp(name, "RadioSightings") == 0	||
				strcmp(name, "RadioSightings2") == 0 ||
				strcmp(name, "EnemyMoraleWorried") == 0 ||
				strcmp(name, "MaxMortarsPerTeam" ) == 0 ||
				strcmp(name, "CthConstantsAimDifficulty") == 0	||
				strcmp(name, "CthConstantsBaseDifficulty") == 0	||
				strcmp(name, "CthConstantsAimDifficultyMilitia") == 0	||
				strcmp(name, "CthConstantsBaseDifficultyMilitia") == 0	||
				strcmp(name, "CthConstantsAimDifficultyPlayer") == 0	||
				strcmp(name, "CthConstantsBaseDifficultyPlayer") == 0	
				))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
DifficultySettingsDataHandle(void *userData, const XML_Char *str, int len)
{
	difficultySettingsParseData * pData = (difficultySettingsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
difficultySettingsEndElementHandle(void *userData, const XML_Char *name)
{
	difficultySettingsParseData * pData = (difficultySettingsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "DIFFICULTY_SETTINGS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DIFFICULTY") == 0)
		{
			pData->curElement = ELEMENT_LIST;
				
			if (!DifficultySettings_TextOnly)
				{
				    wcscpy(zDiffSetting[pData->curDifficultySettings.uiIndex].szDiffName, pData->curDifficultySettings.szDiffName);
					wcscpy(zDiffSetting[pData->curDifficultySettings.uiIndex].szConfirmText, pData->curDifficultySettings.szConfirmText);					
					zDiffSetting[pData->curDifficultySettings.uiIndex].iStartingCash = pData->curDifficultySettings.iStartingCash;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iEnemyAPBonus = pData->curDifficultySettings.iEnemyAPBonus;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iNumKillsPerProgressPoint = pData->curDifficultySettings.iNumKillsPerProgressPoint;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iInitialGarrisonPercentages = pData->curDifficultySettings.iInitialGarrisonPercentages;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iMinEnemyGroupSize = pData->curDifficultySettings.iMinEnemyGroupSize;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iCounterAttackGroupSize = pData->curDifficultySettings.iCounterAttackGroupSize;
					zDiffSetting[pData->curDifficultySettings.uiIndex].bUnlimitedPoolOfTroops = pData->curDifficultySettings.bUnlimitedPoolOfTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueensInitialPoolOfTroops = pData->curDifficultySettings.iQueensInitialPoolOfTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenPoolMaxSizePerDifficultyLevel = pData->curDifficultySettings.iQueenPoolMaxSizePerDifficultyLevel;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenPoolIncrementDaysPerDifficultyLevel = pData->curDifficultySettings.iQueenPoolIncrementDaysPerDifficultyLevel;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenPoolBaseIncrementSizePerDifficultyLevel = pData->curDifficultySettings.iQueenPoolBaseIncrementSizePerDifficultyLevel;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenPoolRecruitPercentPerDifficultyLevel = pData->curDifficultySettings.iQueenPoolRecruitPercentPerDifficultyLevel;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iEnemyStartingAlertLevel = pData->curDifficultySettings.iEnemyStartingAlertLevel;				
					zDiffSetting[pData->curDifficultySettings.uiIndex].iEnemyAlertDecay = pData->curDifficultySettings.iEnemyAlertDecay;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iNumAwareBattles = pData->curDifficultySettings.iNumAwareBattles;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iBaseDelayInMinutesBetweenEvaluations = pData->curDifficultySettings.iBaseDelayInMinutesBetweenEvaluations;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iEvaluationDelayVariance = pData->curDifficultySettings.iEvaluationDelayVariance;			
					zDiffSetting[pData->curDifficultySettings.uiIndex].iGracePeriodInHoursAfterSectorLiberation = pData->curDifficultySettings.iGracePeriodInHoursAfterSectorLiberation;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iGracePeriodInDaysAfterPatrolDestroyed = pData->curDifficultySettings.iGracePeriodInDaysAfterPatrolDestroyed;					
					zDiffSetting[pData->curDifficultySettings.uiIndex].bAggressiveQueenAi = pData->curDifficultySettings.bAggressiveQueenAi;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iMaxMercDeaths = pData->curDifficultySettings.iMaxMercDeaths;
					zDiffSetting[pData->curDifficultySettings.uiIndex].usLootStatusModifier = pData->curDifficultySettings.usLootStatusModifier;

					//Creatures Settings
					zDiffSetting[pData->curDifficultySettings.uiIndex].iCreatureSpreadTime = pData->curDifficultySettings.iCreatureSpreadTime;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenReproductionBase = pData->curDifficultySettings.iQueenReproductionBase;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenReproductionBonus = pData->curDifficultySettings.iQueenReproductionBonus;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iQueenInitBonusSpread = pData->curDifficultySettings.iQueenInitBonusSpread;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iCreaturePopulationModifier = pData->curDifficultySettings.iCreaturePopulationModifier;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iCreatureTownAggressiveness = pData->curDifficultySettings.iCreatureTownAggressiveness;			

					//sectors
					zDiffSetting[pData->curDifficultySettings.uiIndex].iJ9B1NumTroops = pData->curDifficultySettings.iJ9B1NumTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iJ9B2NumCreatures = pData->curDifficultySettings.iJ9B2NumCreatures;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iK4B1NumTroops = pData->curDifficultySettings.iK4B1NumTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iK4B1NumElites = pData->curDifficultySettings.iK4B1NumElites;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iO3B1NumTroops = pData->curDifficultySettings.iO3B1NumTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iO3B1NumElites = pData->curDifficultySettings.iO3B1NumElites;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iP3B1NumElites = pData->curDifficultySettings.iP3B1NumElites;
					
					//Other
					zDiffSetting[pData->curDifficultySettings.uiIndex].bStrategicAiActionWakeQueen = pData->curDifficultySettings.bStrategicAiActionWakeQueen;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iUpdateLastDayOfPlayerActivity = pData->curDifficultySettings.iUpdateLastDayOfPlayerActivity;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iChanceOfEnemyAmbushes = pData->curDifficultySettings.iChanceOfEnemyAmbushes;					
					zDiffSetting[pData->curDifficultySettings.uiIndex].bAllowReinforcements = pData->curDifficultySettings.bAllowReinforcements;
					zDiffSetting[pData->curDifficultySettings.uiIndex].bAllowReinforcementsOmerta = pData->curDifficultySettings.bAllowReinforcementsOmerta;				
					zDiffSetting[pData->curDifficultySettings.uiIndex].iPercentElitesBonus = pData->curDifficultySettings.iPercentElitesBonus;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iDesiredPopulationL2 = pData->curDifficultySettings.iDesiredPopulationL2;				
					zDiffSetting[pData->curDifficultySettings.uiIndex].iDesiredPopulationL3 = pData->curDifficultySettings.iDesiredPopulationL3;
						
					zDiffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops1 = pData->curDifficultySettings.iWeaponCacheTroops1;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops2 = pData->curDifficultySettings.iWeaponCacheTroops2;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops3 = pData->curDifficultySettings.iWeaponCacheTroops3;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops4 = pData->curDifficultySettings.iWeaponCacheTroops4;
					zDiffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops5 = pData->curDifficultySettings.iWeaponCacheTroops5;
					
					zDiffSetting[pData->curDifficultySettings.uiIndex].bUpgradeAdminsToTroops = pData->curDifficultySettings.bUpgradeAdminsToTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].bUpgradeGarrisonsAdminsToTroops = pData->curDifficultySettings.bUpgradeGarrisonsAdminsToTroops;
					zDiffSetting[pData->curDifficultySettings.uiIndex].usAlwaysUpGradeAdminsToTroopsProgress = pData->curDifficultySettings.usAlwaysUpGradeAdminsToTroopsProgress;
					zDiffSetting[pData->curDifficultySettings.uiIndex].usLevelModifierLowLimit = pData->curDifficultySettings.usLevelModifierLowLimit;
					zDiffSetting[pData->curDifficultySettings.uiIndex].usLevelModifierHighLimit = pData->curDifficultySettings.usLevelModifierHighLimit;
					zDiffSetting[pData->curDifficultySettings.uiIndex].bAllowUnrestrictedXPLevels = pData->curDifficultySettings.bAllowUnrestrictedXPLevels;

					zDiffSetting[pData->curDifficultySettings.uiIndex].bQueenLosingControlOfSector = pData->curDifficultySettings.bQueenLosingControlOfSector;

					zDiffSetting[pData->curDifficultySettings.uiIndex].bBloodcatAmbush = pData->curDifficultySettings.bBloodcatAmbush;
					zDiffSetting[pData->curDifficultySettings.uiIndex].bAirRaidLookForDive = pData->curDifficultySettings.bAirRaidLookForDive;
					
				
					zDiffSetting[pData->curDifficultySettings.uiIndex].iGetNumberOfTurnsPowerGenFanWillBeStoppedFor = pData->curDifficultySettings.iGetNumberOfTurnsPowerGenFanWillBeStoppedFor;
					
					
					zDiffSetting[pData->curDifficultySettings.uiIndex].bRadioSightings = pData->curDifficultySettings.bRadioSightings;
					zDiffSetting[pData->curDifficultySettings.uiIndex].bRadioSightings2 = pData->curDifficultySettings.bRadioSightings2;

					zDiffSetting[pData->curDifficultySettings.uiIndex].fCTHSettingsAimDifficultyEnemy = pData->curDifficultySettings.fCTHSettingsAimDifficultyEnemy;
					zDiffSetting[pData->curDifficultySettings.uiIndex].fCTHSettingsBaseDifficultyEnemy = pData->curDifficultySettings.fCTHSettingsBaseDifficultyEnemy;
					zDiffSetting[pData->curDifficultySettings.uiIndex].fCTHSettingsAimDifficultyMilitia = pData->curDifficultySettings.fCTHSettingsAimDifficultyMilitia;
					zDiffSetting[pData->curDifficultySettings.uiIndex].fCTHSettingsBaseDifficultyMilitia = pData->curDifficultySettings.fCTHSettingsBaseDifficultyMilitia;
					zDiffSetting[pData->curDifficultySettings.uiIndex].fCTHSettingsAimDifficultyPlayer = pData->curDifficultySettings.fCTHSettingsAimDifficultyPlayer;
					zDiffSetting[pData->curDifficultySettings.uiIndex].fCTHSettingsBaseDifficultyPlayer = pData->curDifficultySettings.fCTHSettingsBaseDifficultyPlayer;
					
					zDiffSetting[pData->curDifficultySettings.uiIndex].bEnemyMoraleWorried = pData->curDifficultySettings.bEnemyMoraleWorried;
					
					zDiffSetting[pData->curDifficultySettings.uiIndex].usMaxMortarsPerTeam = pData->curDifficultySettings.usMaxMortarsPerTeam;
				}
				else
				{			
					wcscpy(zDiffSetting[pData->curDifficultySettings.uiIndex].szDiffName, pData->curDifficultySettings.szDiffName);
					wcscpy(zDiffSetting[pData->curDifficultySettings.uiIndex].szConfirmText, pData->curDifficultySettings.szConfirmText);
				}			
		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.uiIndex	= (UINT16) atol(pData->szCharData);
			MaxDifficultySettingsValues++; 
		}
		else if(strcmp(name, "Name") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curDifficultySettings.szDiffName, sizeof(pData->curDifficultySettings.szDiffName)/sizeof(pData->curDifficultySettings.szDiffName[0]) );
			pData->curDifficultySettings.szDiffName[sizeof(pData->curDifficultySettings.szDiffName)/sizeof(pData->curDifficultySettings.szDiffName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ConfirmText") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curDifficultySettings.szConfirmText, sizeof(pData->curDifficultySettings.szConfirmText)/sizeof(pData->curDifficultySettings.szConfirmText[0]) );
			pData->curDifficultySettings.szConfirmText[sizeof(pData->curDifficultySettings.szConfirmText)/sizeof(pData->curDifficultySettings.szConfirmText[0]) - 1] = '\0';
		}		
		else if(strcmp(name, "StartingCash") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iStartingCash	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "EnemyAPBonus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iEnemyAPBonus	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "NumKillsPerProgressPoint") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iNumKillsPerProgressPoint	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "InitialGarrisonPercentages") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iInitialGarrisonPercentages	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "MinEnemyGroupSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iMinEnemyGroupSize	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "CounterAttackGroupSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iCounterAttackGroupSize	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "PercentElitesBonus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iPercentElitesBonus	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "UnlimitedPoolOfTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bUnlimitedPoolOfTroops	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QueensInitialPoolOfTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueensInitialPoolOfTroops = max( 0, (INT32)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "QueenPoolIncrementDaysPerDifficultyLevel" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenPoolIncrementDaysPerDifficultyLevel = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "QueenPoolBaseIncrementSizePerDifficultyLevel" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenPoolBaseIncrementSizePerDifficultyLevel = (INT32)atol( pData->szCharData );
			pData->curDifficultySettings.iQueenPoolBaseIncrementSizePerDifficultyLevel = max( 0, pData->curDifficultySettings.iQueenPoolBaseIncrementSizePerDifficultyLevel );
		}
		else if ( strcmp( name, "QueenPoolRecruitPercentPerDifficultyLevel" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenPoolRecruitPercentPerDifficultyLevel = (UINT8)atol( pData->szCharData );
			pData->curDifficultySettings.iQueenPoolRecruitPercentPerDifficultyLevel = min( 100, pData->curDifficultySettings.iQueenPoolRecruitPercentPerDifficultyLevel );
		}
		else if ( strcmp( name, "QueenPoolMaxSizePerDifficultyLevel" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenPoolMaxSizePerDifficultyLevel = (INT32)atol( pData->szCharData );
			pData->curDifficultySettings.iQueenPoolMaxSizePerDifficultyLevel = max( 0, pData->curDifficultySettings.iQueenPoolMaxSizePerDifficultyLevel );
		}
		else if(strcmp(name, "EnemyStartingAlertLevel") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iEnemyStartingAlertLevel	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "EnemyAlertDecay") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iEnemyAlertDecay	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "NumAwareBattles") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iNumAwareBattles	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "BaseDelayInMinutesBetweenEvaluations") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iBaseDelayInMinutesBetweenEvaluations	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "EvaluationDelayVariance") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iEvaluationDelayVariance	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "GracePeriodInHoursAfterSectorLiberation") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iGracePeriodInHoursAfterSectorLiberation	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "GracePeriodInDaysAfterPatrolDestroyed") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iGracePeriodInDaysAfterPatrolDestroyed	= (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "AggressiveQueenAi") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bAggressiveQueenAi	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "MaxMercDeaths") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iMaxMercDeaths	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "LootStatusModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.usLootStatusModifier	= (UINT8) atol(pData->szCharData);
		}
		//Creatures Settings
		else if(strcmp(name, "CreatureSpreadTime") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iCreatureSpreadTime	= (UINT16) atol(pData->szCharData);
		}	
		else if(strcmp(name, "QueenReproductionBase") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenReproductionBase	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "QueenReproductionBonus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenReproductionBonus	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "QueenInitBonusSpread") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenInitBonusSpread	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "CreaturePopulationModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iCreaturePopulationModifier	= (INT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "CreatureTownAggressiveness") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iCreatureTownAggressiveness	= (INT8) atol(pData->szCharData);
		}	
		//sectors
		else if(strcmp(name, "SectorJ9B1NumTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iJ9B1NumTroops	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "SectorJ9B2NumCreatures") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iJ9B2NumCreatures	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "SectorK4B1NumTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iK4B1NumTroops	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "SectorK4B1NumElites") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iK4B1NumElites	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "SectorO3B1NumTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iO3B1NumTroops	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "SectorO3B1NumElites") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iO3B1NumElites	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "SectorP3B1NumElites") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iP3B1NumElites	= (UINT8) atol(pData->szCharData);
		}
		//Sector
		else if(strcmp(name, "StrategicAiActionWakeQueen") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bStrategicAiActionWakeQueen	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "UpdateLastDayOfPlayerActivity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iUpdateLastDayOfPlayerActivity	= (UINT16) atol(pData->szCharData);
		}				
		else if(strcmp(name, "ChanceOfEnemyAmbushes") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iChanceOfEnemyAmbushes	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "AllowReinforcements") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bAllowReinforcements	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "AllowReinforcementsOmerta") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bAllowReinforcementsOmerta	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "PopulationLevel2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iDesiredPopulationL2	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "PopulationLevel3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iDesiredPopulationL3	= (BOOLEAN) atol(pData->szCharData);
		}		
		else if(strcmp(name, "WeaponCacheTroops1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iWeaponCacheTroops1	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "WeaponCacheTroops2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iWeaponCacheTroops2	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "WeaponCacheTroops3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iWeaponCacheTroops3	= (UINT8) atol(pData->szCharData);
		}		
		else if(strcmp(name, "WeaponCacheTroops4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iWeaponCacheTroops4	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "WeaponCacheTroops5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iWeaponCacheTroops5	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "UpgradeAdminsToTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bUpgradeAdminsToTroops	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "UpgradeGarrisonsAdminsToTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bUpgradeGarrisonsAdminsToTroops = (BOOLEAN)atol( pData->szCharData );
		}
		else if ( strcmp( name, "AlwaysUpGradeAdminsToTroopsProgress" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.usAlwaysUpGradeAdminsToTroopsProgress = (UINT8)atol( pData->szCharData );
			pData->curDifficultySettings.usAlwaysUpGradeAdminsToTroopsProgress = min( pData->curDifficultySettings.usAlwaysUpGradeAdminsToTroopsProgress, 100 );
		}
		else if ( strcmp( name, "LevelModifierLowLimit" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.usLevelModifierLowLimit = (UINT8)atol( pData->szCharData );
			pData->curDifficultySettings.usLevelModifierLowLimit = min( pData->curDifficultySettings.usLevelModifierLowLimit, 5 );
		}
		else if ( strcmp( name, "LevelModifierHighLimit" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.usLevelModifierHighLimit = (UINT8)atol( pData->szCharData );
			pData->curDifficultySettings.usLevelModifierHighLimit = min( pData->curDifficultySettings.usLevelModifierHighLimit, 5 );
		}
		else if(strcmp(name, "AllowUnrestrictedXPLevels") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bAllowUnrestrictedXPLevels	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "QueenAttackLosingControlOfSector") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bQueenLosingControlOfSector	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "BloodcatAmbushSectors") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bBloodcatAmbush	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "AirRaidLookForDive") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bAirRaidLookForDive	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "NumberOfTurnsPowerGenFanWillBeStoppedFor") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iGetNumberOfTurnsPowerGenFanWillBeStoppedFor	= (UINT32) atol(pData->szCharData);
		}		
		else if(strcmp(name, "RadioSightings") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bRadioSightings	= (BOOLEAN) atol(pData->szCharData);
		}			
		else if(strcmp(name, "RadioSightings2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bRadioSightings2	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "EnemyMoraleWorried") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bEnemyMoraleWorried	= (BOOLEAN) atol(pData->szCharData);
		}
		else if ( strcmp( name, "MaxMortarsPerTeam" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.usMaxMortarsPerTeam = (UINT8)atol( pData->szCharData );
		}
		else if(strcmp(name, "CthConstantsAimDifficulty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.fCTHSettingsAimDifficultyEnemy	= (FLOAT) atol(pData->szCharData);
		}	
		else if(strcmp(name, "CthConstantsBaseDifficulty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.fCTHSettingsBaseDifficultyEnemy	= (FLOAT) atol(pData->szCharData);
		}			
		else if(strcmp(name, "CthConstantsAimDifficultyMilitia") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.fCTHSettingsAimDifficultyMilitia	= (FLOAT) atol(pData->szCharData);
		}	
		else if(strcmp(name, "CthConstantsBaseDifficultyMilitia") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.fCTHSettingsBaseDifficultyMilitia	= (FLOAT) atol(pData->szCharData);
		}			
		else if(strcmp(name, "CthConstantsAimDifficultyPlayer") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.fCTHSettingsAimDifficultyPlayer	= (FLOAT) atol(pData->szCharData);
		}	
		else if(strcmp(name, "CthConstantsBaseDifficultyPlayer") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.fCTHSettingsBaseDifficultyPlayer	= (FLOAT) atol(pData->szCharData);
		}			
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInDifficultySettings( STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	difficultySettingsParseData pData;
	
	MaxDifficultySettingsValues = 0;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading DifficultySettings.xml" );
	
	DifficultySettings_TextOnly = localizedVersion;
	
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( localizedVersion );

	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );


	XML_SetElementHandler(parser, DifficultySettingsParseDataStartElementHandle, difficultySettingsEndElementHandle);
	XML_SetCharacterDataHandler(parser, DifficultySettingsDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in DifficultySettings.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	MaxDifficultySettingsValues--; 
	
	return( TRUE );
}
