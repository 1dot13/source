#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Text.h"
	#include "GameInitOptionsScreen.h"
#endif

#ifdef DIFFICULTY_SETTING

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
				strcmp(name, "UnlimitedPoolOfTroops") == 0 ||
				strcmp(name, "QueensInitialPoolOfTroops") == 0 ||
				strcmp(name, "QueenPoolIncrementPerDifficultyLevel") == 0 ||
				strcmp(name, "EnemyStartingAlertLevel") == 0 ||
				strcmp(name, "EnemyAlertDecay") == 0 ||
				strcmp(name, "NumAwareBattles") == 0 ||
				strcmp(name, "BaseDelayInMinutesBetweenEvaluations") == 0 ||
				strcmp(name, "EvaluationDelayVariance") == 0 ||
				strcmp(name, "GracePeriodInHoursAfterSectorLiberation") == 0 ||
				strcmp(name, "GracePeriodInDaysAfterPatrolDestroyed") == 0 ||
				strcmp(name, "AggressiveQueenAi") == 0 ||
				strcmp(name, "MaxMercDeaths") == 0 ||
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
				strcmp(name, "QueenAttackLosingControlOfSector") == 0	||
				strcmp(name, "BloodcatAmbushSectors") == 0	||
				strcmp(name, "AirRaidLookForDive") == 0	||
				strcmp(name, "NumberOfTurnsPowerGenFanWillBeStoppedFor") == 0 ||
				strcmp(name, "RadioSightings") == 0	||
				strcmp(name, "RadioSightings2") == 0 ||
				strcmp(name, "EnemyMoraleWorried") == 0 ||
				strcmp(name, "CthConstantsAimDifficulty") == 0	||
				strcmp(name, "CthConstantsBaseDifficulty") == 0	
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
				    wcscpy(zDeffSetting[pData->curDifficultySettings.uiIndex].szDeffName, pData->curDifficultySettings.szDeffName);
					wcscpy(zDeffSetting[pData->curDifficultySettings.uiIndex].szConfirmText, pData->curDifficultySettings.szConfirmText);					
					zDeffSetting[pData->curDifficultySettings.uiIndex].iStartingCash = pData->curDifficultySettings.iStartingCash;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iEnemyAPBonus = pData->curDifficultySettings.iEnemyAPBonus;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iNumKillsPerProgressPoint = pData->curDifficultySettings.iNumKillsPerProgressPoint;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iInitialGarrisonPercentages = pData->curDifficultySettings.iInitialGarrisonPercentages;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iMinEnemyGroupSize = pData->curDifficultySettings.iMinEnemyGroupSize;
					zDeffSetting[pData->curDifficultySettings.uiIndex].bUnlimitedPoolOfTroops = pData->curDifficultySettings.bUnlimitedPoolOfTroops;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iQueensInitialPoolOfTroops = pData->curDifficultySettings.iQueensInitialPoolOfTroops;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iQueenPoolIncrementPerDifficultyLevel = pData->curDifficultySettings.iQueenPoolIncrementPerDifficultyLevel;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iEnemyStartingAlertLevel = pData->curDifficultySettings.iEnemyStartingAlertLevel;				
					zDeffSetting[pData->curDifficultySettings.uiIndex].iEnemyAlertDecay = pData->curDifficultySettings.iEnemyAlertDecay;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iNumAwareBattles = pData->curDifficultySettings.iNumAwareBattles;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iBaseDelayInMinutesBetweenEvaluations = pData->curDifficultySettings.iBaseDelayInMinutesBetweenEvaluations;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iEvaluationDelayVariance = pData->curDifficultySettings.iEvaluationDelayVariance;			
					zDeffSetting[pData->curDifficultySettings.uiIndex].iGracePeriodInHoursAfterSectorLiberation = pData->curDifficultySettings.iGracePeriodInHoursAfterSectorLiberation;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iGracePeriodInDaysAfterPatrolDestroyed = pData->curDifficultySettings.iGracePeriodInDaysAfterPatrolDestroyed;					
					zDeffSetting[pData->curDifficultySettings.uiIndex].bAggressiveQueenAi = pData->curDifficultySettings.bAggressiveQueenAi;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iMaxMercDeaths = pData->curDifficultySettings.iMaxMercDeaths;

					//Creatures Settings
					zDeffSetting[pData->curDifficultySettings.uiIndex].iCreatureSpreadTime = pData->curDifficultySettings.iCreatureSpreadTime;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iQueenReproductionBase = pData->curDifficultySettings.iQueenReproductionBase;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iQueenReproductionBonus = pData->curDifficultySettings.iQueenReproductionBonus;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iQueenInitBonusSpread = pData->curDifficultySettings.iQueenInitBonusSpread;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iCreaturePopulationModifier = pData->curDifficultySettings.iCreaturePopulationModifier;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iCreatureTownAggressiveness = pData->curDifficultySettings.iCreatureTownAggressiveness;			

					//sectors
					zDeffSetting[pData->curDifficultySettings.uiIndex].iJ9B1NumTroops = pData->curDifficultySettings.iJ9B1NumTroops;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iJ9B2NumCreatures = pData->curDifficultySettings.iJ9B2NumCreatures;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iK4B1NumTroops = pData->curDifficultySettings.iK4B1NumTroops;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iK4B1NumElites = pData->curDifficultySettings.iK4B1NumElites;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iO3B1NumTroops = pData->curDifficultySettings.iO3B1NumTroops;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iO3B1NumElites = pData->curDifficultySettings.iO3B1NumElites;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iP3B1NumElites = pData->curDifficultySettings.iP3B1NumElites;
					
					//Other
					zDeffSetting[pData->curDifficultySettings.uiIndex].bStrategicAiActionWakeQueen = pData->curDifficultySettings.bStrategicAiActionWakeQueen;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iUpdateLastDayOfPlayerActivity = pData->curDifficultySettings.iUpdateLastDayOfPlayerActivity;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iChanceOfEnemyAmbushes = pData->curDifficultySettings.iChanceOfEnemyAmbushes;					
					zDeffSetting[pData->curDifficultySettings.uiIndex].bAllowReinforcements = pData->curDifficultySettings.bAllowReinforcements;
					zDeffSetting[pData->curDifficultySettings.uiIndex].bAllowReinforcementsOmerta = pData->curDifficultySettings.bAllowReinforcementsOmerta;				
					zDeffSetting[pData->curDifficultySettings.uiIndex].iPercentElitesBonus = pData->curDifficultySettings.iPercentElitesBonus;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iDesiredPopulationL2 = pData->curDifficultySettings.iDesiredPopulationL2;				
					zDeffSetting[pData->curDifficultySettings.uiIndex].iDesiredPopulationL3 = pData->curDifficultySettings.iDesiredPopulationL3;
						
					zDeffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops1 = pData->curDifficultySettings.iWeaponCacheTroops1;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops2 = pData->curDifficultySettings.iWeaponCacheTroops2;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops3 = pData->curDifficultySettings.iWeaponCacheTroops3;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops4 = pData->curDifficultySettings.iWeaponCacheTroops4;
					zDeffSetting[pData->curDifficultySettings.uiIndex].iWeaponCacheTroops5 = pData->curDifficultySettings.iWeaponCacheTroops5;
					
					zDeffSetting[pData->curDifficultySettings.uiIndex].bUpgradeAdminsToTroops = pData->curDifficultySettings.bUpgradeAdminsToTroops;
					zDeffSetting[pData->curDifficultySettings.uiIndex].bUpgradeAdminsToTroops2 = pData->curDifficultySettings.bUpgradeAdminsToTroops2;

					zDeffSetting[pData->curDifficultySettings.uiIndex].bQueenLosingControlOfSector = pData->curDifficultySettings.bQueenLosingControlOfSector;

					zDeffSetting[pData->curDifficultySettings.uiIndex].bBloodcatAmbush = pData->curDifficultySettings.bBloodcatAmbush;
					zDeffSetting[pData->curDifficultySettings.uiIndex].bAirRaidLookForDive = pData->curDifficultySettings.bAirRaidLookForDive;
					
				
					zDeffSetting[pData->curDifficultySettings.uiIndex].iGetNumberOfTurnsPowerGenFanWillBeStoppedFor = pData->curDifficultySettings.iGetNumberOfTurnsPowerGenFanWillBeStoppedFor;
					
					
					zDeffSetting[pData->curDifficultySettings.uiIndex].bRadioSightings = pData->curDifficultySettings.bRadioSightings;
					zDeffSetting[pData->curDifficultySettings.uiIndex].bRadioSightings2 = pData->curDifficultySettings.bRadioSightings2;

					zDeffSetting[pData->curDifficultySettings.uiIndex].NewDifficualtySetingsAIM_DIFFICULTY = pData->curDifficultySettings.NewDifficualtySetingsAIM_DIFFICULTY;
					zDeffSetting[pData->curDifficultySettings.uiIndex].NewDifficualtySetingsBASE_DIFFICULTY = pData->curDifficultySettings.NewDifficualtySetingsBASE_DIFFICULTY;
					
					zDeffSetting[pData->curDifficultySettings.uiIndex].bEnemyMoraleWorried = pData->curDifficultySettings.bEnemyMoraleWorried;
								
				}
				else
				{			
					wcscpy(zDeffSetting[pData->curDifficultySettings.uiIndex].szDeffName, pData->curDifficultySettings.szDeffName);
					wcscpy(zDeffSetting[pData->curDifficultySettings.uiIndex].szConfirmText, pData->curDifficultySettings.szConfirmText);
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

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curDifficultySettings.szDeffName, sizeof(pData->curDifficultySettings.szDeffName)/sizeof(pData->curDifficultySettings.szDeffName[0]) );
			pData->curDifficultySettings.szDeffName[sizeof(pData->curDifficultySettings.szDeffName)/sizeof(pData->curDifficultySettings.szDeffName[0]) - 1] = '\0';
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
			pData->curDifficultySettings.iQueensInitialPoolOfTroops	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "QueenPoolIncrementPerDifficultyLevel") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.iQueenPoolIncrementPerDifficultyLevel	= (UINT32) atol(pData->szCharData);
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
			pData->curDifficultySettings.bUpgradeAdminsToTroops2	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "UpgradeGarrisonsAdminsToTroops") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.bUpgradeAdminsToTroops	= (BOOLEAN) atol(pData->szCharData);
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
		else if(strcmp(name, "CthConstantsAimDifficulty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.NewDifficualtySetingsAIM_DIFFICULTY	= (FLOAT) atol(pData->szCharData);
		}	
		else if(strcmp(name, "CthConstantsBaseDifficulty") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDifficultySettings.NewDifficualtySetingsBASE_DIFFICULTY	= (FLOAT) atol(pData->szCharData);
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
#endif