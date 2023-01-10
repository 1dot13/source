///////////////////////////////////////////////////////////////////////////////
// HEADROCK HAM 3.5: Facility Type Externalization
//
// This file reads XML data from FacilityTypes.XML and stores it in an array.
// It tells the game which bonuses or penalties are given for performing
// specific actions or even just being present in a sector. Later in the game,
// the appropriate functions will read this data to change strategic factors,
// like the speed of training or the ability to train militia.
//
///////////////////////////////////////////////////////////////////////////////

	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "gamesettings.h"
	#include "XML.h"
	#include "FileMan.h"
	#include "Campaign Types.h"
	#include "Facilities.h"

FACILITYTYPE gFacilityTypes[ MAX_NUM_FACILITY_TYPES ];
UINT16 NUM_FACILITY_TYPES = 0;

typedef enum
{
	FACILITYTYPE_NONE = 0,
	FACILITYTYPE_LIST,
	FACILITYTYPE_TYPE,
	FACILITYTYPE_ELEMENT,
	FACILITYTYPE_ASSIGNMENT,
	FACILITYTYPE_ASSIGNMENT_ELEMENT,
	FACILITYTYPE_CONDITIONS,
	FACILITYTYPE_CONDITIONS_ELEMENT,
	FACILITYTYPE_RISK,
	FACILITYTYPE_RISK_ELEMENT,
	FACILITYTYPE_PRODUCTION,
	FACILITYTYPE_PRODUCTION_ELEMENT,

} FACILITYTYPE_PARSE_STAGE;

typedef struct
{
	FACILITYTYPE_PARSE_STAGE	curElement;
	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	INT16			curAssignmentType;
	INT16			curRisk;
	INT16			curProduction;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	FACILITYTYPE	curFacilityTypeData;
	FACILITYASSIGNMENTTYPE curAssignmentData;
	PRODUCTION_LINE curProductionData;
} facilitytypeParseData;
#define FACILITYTYPEPARSEDATA_SIZE_OF_POD offsetof(facilitytypeParseData, curFacilityTypeData)

BOOLEAN FacilityTypes_TextOnly;

void InitFacilityTypeEntry( facilitytypeParseData *pData )
{
	// This does base setup on a single type entry, making sure that all values are set to DEFAULT. This is run
	// immediately before writing XML data into game memory.
	// Default values set here are only those that differ from 0. Normally this means maximums and performace data
	// which should equal 100 unless defined otherwise in the XML.

	pData->curFacilityTypeData.usMilitiaTraining = 100;

	for (UINT16 cnt = 0; cnt < NUM_FACILITY_ASSIGNMENTS; ++cnt )
	{
		pData->curFacilityTypeData.AssignmentData[ cnt ].ubMaximumBreath = 100;
		pData->curFacilityTypeData.AssignmentData[ cnt ].ubMaximumMorale = 100;
		pData->curFacilityTypeData.AssignmentData[ cnt ].usKitDegrade = 100;
		pData->curFacilityTypeData.AssignmentData[ cnt ].usFatigue = 100;
		pData->curFacilityTypeData.AssignmentData[ cnt ].usPerformance = 100;
		pData->curFacilityTypeData.AssignmentData[ cnt ].usSleep = 100;
	}
}

void InitAssignmentDataArray( facilitytypeParseData *pData )
{

	// Set assignment data from memory
	pData->curAssignmentData.usPerformance = 100;				
	pData->curAssignmentData.ubStaffLimit = 0;
	swprintf(pData->curAssignmentData.szTooltipText, L"");
	pData->curAssignmentData.sCostPerHour = 0;

	pData->curAssignmentData.usFatigue = 100;
	pData->curAssignmentData.usSleep = 100;
	pData->curAssignmentData.usKitDegrade = 100;

	pData->curAssignmentData.sSkyriderCostModifier = 0;
	pData->curAssignmentData.usMineIncomeModifier = 100;
	pData->curAssignmentData.fOnlyLocalMineAffected = 0;

	pData->curAssignmentData.sCantinaFoodModifier = 100;
	pData->curAssignmentData.usPrisonBaseLimit = 0;

	pData->curAssignmentData.ubMaximumBreath = 100;
	pData->curAssignmentData.ubMaximumMorale = 100;

	pData->curAssignmentData.fCountEnemiesInCities = 0;
	pData->curAssignmentData.fCountEnemiesInWild = 0;
	pData->curAssignmentData.fDetectEnemiesAnywhere = 0;
	pData->curAssignmentData.fDetectEnemiesDynamic = 0;
	pData->curAssignmentData.ubDetectEnemiesImmediate = 0;
	pData->curAssignmentData.fDetectEnemiesLongrange = 0;

	pData->curAssignmentData.ubMinimumAgility = 0;
	pData->curAssignmentData.ubMinimumBreath = 0;
	pData->curAssignmentData.ubMinimumDexterity = 0;
	pData->curAssignmentData.ubMinimumExplosives = 0;
	pData->curAssignmentData.ubMinimumHealth = 0;
	pData->curAssignmentData.ubMinimumLeadership = 0;
	pData->curAssignmentData.ubMinimumLevel = 0;
	pData->curAssignmentData.ubMinimumLoyaltyHere = 0;
	pData->curAssignmentData.ubMinimumMarksmanship = 0;
	pData->curAssignmentData.ubMinimumMechanical = 0;
	pData->curAssignmentData.ubMinimumMedical = 0;
	pData->curAssignmentData.ubMinimumMorale = 0;
	pData->curAssignmentData.ubMinimumStrength = 0;
	pData->curAssignmentData.ubMinimumWisdom = 0;

	// Set RISK data
	for (UINT16 cnt = 0; cnt < NUM_RISKS; ++cnt)
	{
		pData->curAssignmentData.Risk[cnt].usChance = 0;
		pData->curAssignmentData.Risk[cnt].bBaseEffect = 0;
		pData->curAssignmentData.Risk[cnt].ubRange = 0;
	}
}

void InitProductionDataArray( facilitytypeParseData *pData )
{
	// Set assignment data from memory
	swprintf( pData->curProductionData.szProductionName, L"" );
	swprintf( pData->curProductionData.szAdditionalRequirementTips, L"" );
	pData->curProductionData.usItemToCreate = NOTHING;
	pData->curProductionData.sMinutesRequired = 60;
	pData->curProductionData.sGridNo_Creation = -1;

	pData->curProductionData.usProductionFlags = 0;

	pData->curProductionData.sHourlyCost = 0;
	pData->curProductionData.usOptional_LoyaltyRequired = 0;
	pData->curProductionData.usOptional_PreProducts.clear();
}

static void XMLCALL
facilitytypeStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	facilitytypeParseData * pData = (facilitytypeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		// Just starting
		if(strcmp(name, "FACILITYTYPES") == 0 && pData->curElement == FACILITYTYPE_NONE)
		{
			pData->curElement = FACILITYTYPE_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}

		// Encountered type entry
		else if(strcmp(name, "FACILITYTYPE") == 0 && pData->curElement == FACILITYTYPE_LIST)
		{
			pData->curElement = FACILITYTYPE_TYPE;

			// Set all values to default before applying XML data
			memset(&pData->curFacilityTypeData, 0, FACILITYTYPE_SIZEOF_POD);
			InitFacilityTypeEntry( pData );

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"MergeStartElementHandle: setting memory for curMerge");

			pData->maxReadDepth++; //we are not skipping this element

			pData->curProduction = -1;

			//pData->curIndex++;
		}

		// Encountered basic type data
		else if(pData->curElement == FACILITYTYPE_TYPE &&
				(strcmp(name, "ubIndex") == 0 ||
				strcmp(name, "szFacilityName") == 0 ||
				strcmp(name, "szFacilityShortName") == 0 ||
				strcmp(name, "ubTotalStaffLimit") == 0 ||

				strcmp(name, "ubMilitiaTrainersAllowed") == 0 ||
				strcmp(name, "usMilitiaTraining") == 0 ||

				strcmp( name, "pmcentrypoint" ) == 0 ))
		{
			pData->curElement = FACILITYTYPE_ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}

		// Encountered Assignment entry
		else if(pData->curElement == FACILITYTYPE_TYPE &&
				strcmp(name, "ASSIGNMENT") == 0)
		{
			// Set value to -1, denoting that we have not yet read which assignment is being described.
			pData->curAssignmentType = -1;
			// Reset the array
			InitAssignmentDataArray (pData);

			pData->curElement = FACILITYTYPE_ASSIGNMENT;

			pData->maxReadDepth++;
		}
		// Encountered Production entry
		else if ( pData->curElement == FACILITYTYPE_TYPE &&
			strcmp( name, "PRODUCTIONLINE" ) == 0 )
		{
			// Set value to -1, denoting that we have not yet read which production is being described.
			pData->curProduction++;

			// Reset the array
			InitProductionDataArray ( pData );

			pData->curElement = FACILITYTYPE_PRODUCTION;

			pData->maxReadDepth++;
		}
		// Encountered production data
		else if ( pData->curElement == FACILITYTYPE_PRODUCTION &&
			( strcmp( name, "szProductionName" ) == 0 ||
				strcmp( name, "szAdditionalRequirementTips" ) == 0 ||
				strcmp( name, "usItemToCreate" ) == 0 ||
				strcmp( name, "sMinutesRequired" ) == 0 ||
				strcmp( name, "sGridNo_Creation" ) == 0 ||
				strcmp( name, "requires_staff" ) == 0 ||
				strcmp( name, "sHourlyCost" ) == 0 ||
				strcmp( name, "usOptional_LoyaltyRequired" ) == 0 ||
				strcmp( name, "usOptional_ItemUsedUp" ) == 0  ) )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION_ELEMENT;
			pData->maxReadDepth++;
		}

		// Encountered assignment data
		else if(pData->curElement == FACILITYTYPE_ASSIGNMENT &&
				(strcmp(name, "ubAssignmentType") == 0 ||
				strcmp(name, "szTooltipText") == 0 ||
				strcmp(name, "ubStaffLimit") == 0 ||
				strcmp(name, "sCostPerHour") == 0 ||
				strcmp(name, "usPerformance") == 0 ||
				strcmp(name, "usFatigue") == 0 ||
				strcmp(name, "usSleep") == 0 ||
				strcmp(name, "usKitDegrade") == 0 ||
				strcmp(name, "ubMaximumMorale") == 0 ||
				strcmp(name, "ubMaximumFatigue") == 0 ||
				strcmp(name, "ubDetectEnemiesImmediate") == 0 ||
				strcmp(name, "fDetectEnemiesDynamic") == 0 ||
				strcmp(name, "fDetectEnemiesLongrange") == 0 ||
				strcmp(name, "fDetectEnemiesAnywhere") == 0 ||
				strcmp(name, "fCountEnemiesInWild") == 0 ||
				strcmp(name, "fCountEnemiesInCities") == 0 ||
				strcmp(name, "sSkyriderCostModifier") == 0 ||
				strcmp(name, "usMineIncomeModifier") == 0 ||
				strcmp(name, "fOnlyLocalMineAffected") == 0 ||
				strcmp(name, "sCantinaFoodModifier") == 0 ||
				strcmp(name, "usPrisonBaseLimit") == 0 ))
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT_ELEMENT;

			pData->maxReadDepth++;
		}
		else if(pData->curElement == FACILITYTYPE_ASSIGNMENT &&
				strcmp(name, "CONDITIONS") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;

			pData->maxReadDepth++;
		}
		// Check for entering into a risk tag. Assign value to current risk.
		else if(pData->curElement == FACILITYTYPE_ASSIGNMENT)
		{
			if (strcmp(name, "STRENGTH") == 0)
			{
				pData->curRisk = RISK_STRENGTH;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "WISDOM") == 0)
			{
				pData->curRisk = RISK_WISDOM;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "AGILITY") == 0)
			{
				pData->curRisk = RISK_AGILITY;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "DEXTERITY") == 0)
			{
				pData->curRisk = RISK_DEXTERITY;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "HEALTH") == 0)
			{
				pData->curRisk = RISK_HEALTH;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "MARKSMANSHIP") == 0)
			{
				pData->curRisk = RISK_MARKSMANSHIP;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "LEADERSHIP") == 0)
			{
				pData->curRisk = RISK_LEADERSHIP;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "EXPLOSIVES") == 0)
			{
				pData->curRisk = RISK_EXPLOSIVES;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "MECHANICAL") == 0)
			{
				pData->curRisk = RISK_MECHANICAL;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "MEDICAL") == 0)
			{
				pData->curRisk = RISK_MEDICAL;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "INJURY") == 0)
			{
				pData->curRisk = RISK_INJURY;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "MORALE") == 0)
			{
				pData->curRisk = RISK_MORALE;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "FATIGUE") == 0)
			{
				pData->curRisk = RISK_FATIGUE;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "LOYALTY_LOCAL") == 0)
			{
				pData->curRisk = RISK_LOYALTY_LOCAL;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "LOYALTY_GLOBAL") == 0)
			{
				pData->curRisk = RISK_LOYALTY_GLOBAL;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
			if (strcmp(name, "DRUNK") == 0)
			{
				pData->curRisk = RISK_DRUNK;
				pData->curElement = FACILITYTYPE_RISK;
				pData->maxReadDepth++;
			}
		}

		else if(pData->curElement == FACILITYTYPE_CONDITIONS &&
				(strcmp(name, "ubMinimumStrength") == 0 ||
				strcmp(name, "ubMinimumHealth") == 0 ||
				strcmp(name, "ubMinimumWisdom") == 0 ||
				strcmp(name, "ubMinimumAgility") == 0 ||
				strcmp(name, "ubMinimumDexterity") == 0 ||
				strcmp(name, "ubMinimumMarksmanship") == 0 ||
				strcmp(name, "ubMinimumLeadership") == 0 ||
				strcmp(name, "ubMinimumExplosives") == 0 ||
				strcmp(name, "ubMinimumMechanical") == 0 ||
				strcmp(name, "ubMinimumMedical") == 0 ||
				strcmp(name, "ubMinimumLevel") == 0 ||
				strcmp(name, "ubMinimumLoyaltyHere") == 0 ||
				strcmp(name, "ubMinimumBreath") == 0 ||
				strcmp(name, "ubMinimumMorale") == 0 ) )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS_ELEMENT;

			pData->maxReadDepth++;
		}
		else if(pData->curElement == FACILITYTYPE_RISK &&
				(strcmp(name, "ubChance") == 0 ||
				strcmp(name, "bBaseEffect") == 0 ||
				strcmp(name, "ubRange") == 0 ) )
		{
			pData->curElement = FACILITYTYPE_RISK_ELEMENT;

			pData->maxReadDepth++;
		}
		else if ( pData->curElement == FACILITYTYPE_PRODUCTION &&
			( strcmp( name, "szProductionName" ) == 0 ||
			strcmp( name, "szAdditionalRequirementTips" ) == 0 ||
			strcmp( name, "usItemToCreate" ) == 0 ||
			strcmp( name, "sMinutesRequired" ) == 0 ||
			strcmp( name, "sGridNo_Creation" ) == 0 ||
			strcmp( name, "requires_staff" ) == 0 ||
			strcmp( name, "sHourlyCost" ) == 0 ||
			strcmp( name, "usOptional_LoyaltyRequired" ) == 0 ||
			strcmp( name, "usOptional_ItemUsedUp" ) == 0 ) )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION_ELEMENT;
			pData->maxReadDepth++;
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
facilitytypeCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	facilitytypeParseData * pData = (facilitytypeParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
facilitytypeEndElementHandle(void *userData, const XML_Char *name)
{
	facilitytypeParseData * pData = (facilitytypeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FACILITYTYPES") == 0)
		{
			if (pData->curElement == FACILITYTYPE_LIST)
			{
				pData->curElement = FACILITYTYPE_NONE;
				NUM_FACILITY_TYPES++;
			}
			else
			{
				SGP_THROW(L"FacilityTypes.XML Error: Tag <FACILITYTYPES> encountered too early! Please make sure to close all opened tags first.");
			}
		}
		else if(strcmp(name, "FACILITYTYPE") == 0)
		{
			if (pData->curElement == FACILITYTYPE_TYPE)
			{
				pData->curElement = FACILITYTYPE_LIST;

				if (pData->curIndex >= 1)
				{
					if (!FacilityTypes_TextOnly)
					{
						// Begin with basic facility data
						wcscpy(gFacilityTypes[pData->curIndex].szFacilityName, pData->curFacilityTypeData.szFacilityName);
						wcscpy(gFacilityTypes[pData->curIndex].szFacilityShortName, pData->curFacilityTypeData.szFacilityShortName);
						gFacilityTypes[pData->curIndex].ubTotalStaffLimit = pData->curFacilityTypeData.ubTotalStaffLimit;

						gFacilityTypes[pData->curIndex].ubMilitiaTrainersAllowed = pData->curFacilityTypeData.ubMilitiaTrainersAllowed;
						gFacilityTypes[pData->curIndex].usMilitiaTraining = pData->curFacilityTypeData.usMilitiaTraining;

						gFacilityTypes[pData->curIndex].usFacilityFlags = pData->curFacilityTypeData.usFacilityFlags;

						// Set assignment-specific data
						for (UINT16 cnt = 0; cnt < NUM_FACILITY_ASSIGNMENTS; ++cnt)
						{
							// Performance and limits
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].usPerformance = pData->curFacilityTypeData.AssignmentData[cnt].usPerformance;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubStaffLimit = pData->curFacilityTypeData.AssignmentData[cnt].ubStaffLimit;
							wcscpy(gFacilityTypes[pData->curIndex].AssignmentData[cnt].szTooltipText, pData->curFacilityTypeData.AssignmentData[cnt].szTooltipText);
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].sCostPerHour = pData->curFacilityTypeData.AssignmentData[cnt].sCostPerHour;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].usFatigue = pData->curFacilityTypeData.AssignmentData[cnt].usFatigue;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].usKitDegrade = pData->curFacilityTypeData.AssignmentData[cnt].usKitDegrade;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].usSleep = pData->curFacilityTypeData.AssignmentData[cnt].usSleep;

							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMaximumBreath = pData->curFacilityTypeData.AssignmentData[cnt].ubMaximumBreath;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMaximumMorale = pData->curFacilityTypeData.AssignmentData[cnt].ubMaximumMorale;

							// Detection abilities
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubDetectEnemiesImmediate = pData->curFacilityTypeData.AssignmentData[cnt].ubDetectEnemiesImmediate;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].fDetectEnemiesDynamic = pData->curFacilityTypeData.AssignmentData[cnt].fDetectEnemiesDynamic;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].fDetectEnemiesLongrange = pData->curFacilityTypeData.AssignmentData[cnt].fDetectEnemiesLongrange;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].fDetectEnemiesAnywhere = pData->curFacilityTypeData.AssignmentData[cnt].fDetectEnemiesAnywhere;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].fCountEnemiesInWild = pData->curFacilityTypeData.AssignmentData[cnt].fCountEnemiesInWild;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].fCountEnemiesInCities = pData->curFacilityTypeData.AssignmentData[cnt].fCountEnemiesInCities;

							// Other strategic abilities
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].sSkyriderCostModifier = pData->curFacilityTypeData.AssignmentData[cnt].sSkyriderCostModifier;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].usMineIncomeModifier = pData->curFacilityTypeData.AssignmentData[cnt].usMineIncomeModifier;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].fOnlyLocalMineAffected = pData->curFacilityTypeData.AssignmentData[cnt].fOnlyLocalMineAffected;

							gFacilityTypes[pData->curIndex].AssignmentData[cnt].sCantinaFoodModifier = pData->curFacilityTypeData.AssignmentData[cnt].sCantinaFoodModifier;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].usPrisonBaseLimit = pData->curFacilityTypeData.AssignmentData[cnt].usPrisonBaseLimit;
							
							// Conditions
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumStrength = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumStrength;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumAgility = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumAgility;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumDexterity = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumDexterity;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumHealth = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumHealth;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumWisdom = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumWisdom;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumMarksmanship = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumMarksmanship;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumLeadership = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumLeadership;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumExplosives = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumExplosives;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumMechanical = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumMechanical;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumMedical = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumMedical;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumLevel = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumLevel;
							gFacilityTypes[pData->curIndex].AssignmentData[cnt].ubMinimumLoyaltyHere = pData->curFacilityTypeData.AssignmentData[cnt].ubMinimumLoyaltyHere;

							// Set risks associated with this assignment
							for (UINT16 cntB = 0; cntB < NUM_RISKS; ++cntB)
							{
								gFacilityTypes[pData->curIndex].AssignmentData[cnt].Risk[cntB].usChance = pData->curFacilityTypeData.AssignmentData[cnt].Risk[cntB].usChance;
								gFacilityTypes[pData->curIndex].AssignmentData[cnt].Risk[cntB].bBaseEffect = pData->curFacilityTypeData.AssignmentData[cnt].Risk[cntB].bBaseEffect;
								gFacilityTypes[pData->curIndex].AssignmentData[cnt].Risk[cntB].ubRange = pData->curFacilityTypeData.AssignmentData[cnt].Risk[cntB].ubRange;
							}
						}

						// Set production-specific data
						gFacilityTypes[pData->curIndex].ProductionData = pData->curFacilityTypeData.ProductionData;
					}
					else
					{
						wcscpy(gFacilityTypes[pData->curIndex].szFacilityName, pData->curFacilityTypeData.szFacilityName);
						wcscpy(gFacilityTypes[pData->curIndex].szFacilityShortName, pData->curFacilityTypeData.szFacilityShortName);
					
						for (UINT16 cnt = 0; cnt < NUM_FACILITY_ASSIGNMENTS; ++cnt)
						{
							wcscpy(gFacilityTypes[pData->curIndex].AssignmentData[cnt].szTooltipText, pData->curFacilityTypeData.AssignmentData[cnt].szTooltipText);
						}
					}

					pData->curIndex = 0; // Done with this index.
					NUM_FACILITY_TYPES++; // Count number of facility types successfully loaded into the game
				}
			}
			else
			{
				SGP_THROW(L"FacilityTypes.XML Error: A </FACILITYTYPE> tag was encountered too early! Please make sure all tags are closed properly.");
			}
		}
		// Set current Index. As long as it equals 0, the program is not writing any data to memory.
		else if(strcmp(name, "ubIndex") == 0)
		{
			pData->curElement = FACILITYTYPE_TYPE;
			pData->curIndex = (UINT8) atol(pData->szCharData);
		}

		//////////////////////////////////////////////
		// General facility data and militia effects

		else if(strcmp(name, "szFacilityName") == 0)
		{
			pData->curElement = FACILITYTYPE_TYPE;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curFacilityTypeData.szFacilityName, sizeof(pData->curFacilityTypeData.szFacilityName)/sizeof(pData->curFacilityTypeData.szFacilityName[0]) );
			pData->curFacilityTypeData.szFacilityName[sizeof(pData->curFacilityTypeData.szFacilityName)/sizeof(pData->curFacilityTypeData.szFacilityName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szFacilityShortName") == 0)
		{
			pData->curElement = FACILITYTYPE_TYPE;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curFacilityTypeData.szFacilityShortName, sizeof(pData->curFacilityTypeData.szFacilityShortName)/sizeof(pData->curFacilityTypeData.szFacilityShortName[0]) );
			pData->curFacilityTypeData.szFacilityShortName[sizeof(pData->curFacilityTypeData.szFacilityShortName)/sizeof(pData->curFacilityTypeData.szFacilityShortName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ubTotalStaffLimit") == 0 )
		{
			pData->curElement = FACILITYTYPE_TYPE;
			pData->curFacilityTypeData.ubTotalStaffLimit = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMilitiaTrainersAllowed") == 0 )
		{
			pData->curElement = FACILITYTYPE_TYPE;
			pData->curFacilityTypeData.ubMilitiaTrainersAllowed = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usMilitiaTraining") == 0 )
		{
			pData->curElement = FACILITYTYPE_TYPE;
			pData->curFacilityTypeData.usMilitiaTraining = (UINT8) atol(pData->szCharData);
		}
		else if ( strcmp( name, "pmcentrypoint" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_TYPE;
			if ( (UINT8)atol( pData->szCharData ) )
				pData->curFacilityTypeData.usFacilityFlags |= FACILITYTYPE_PMCENTRYPOINT;
		}

		///////////////////////////////////////////////////////
		// Assignments

		else if(strcmp(name, "ASSIGNMENT") == 0 )
		{
			pData->curElement = FACILITYTYPE_TYPE;
			// Make sure an assignment is selected for data dump
			if (pData->curAssignmentType == -1)
			{
				CHAR16 sErrorString[256];
				swprintf(sErrorString, L"FacilityTypes.XML Error: Assignment data for facility type %d is missing a <ubAssignmentType> tag", pData->curIndex);
				SGP_THROW(sErrorString);
			}
			else if (pData->curAssignmentData.ubStaffLimit <= 0 && pData->curAssignmentType != FAC_AMBIENT && !FacilityTypes_TextOnly)
			{
				CHAR16 sErrorString[256];
				swprintf(sErrorString, L"FacilityTypes.XML Error: Assignment data for facility type %d is missing a <ubStaffLimit> tag, or the value of the tag is 0.", pData->curIndex);
				SGP_THROW(sErrorString);
			}
			else
			{
				// Set assignment data from memory
				if (!FacilityTypes_TextOnly)
				{
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].usPerformance = pData->curAssignmentData.usPerformance;
					wcscpy(pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].szTooltipText, pData->curAssignmentData.szTooltipText);
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubStaffLimit = pData->curAssignmentData.ubStaffLimit;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].sCostPerHour = pData->curAssignmentData.sCostPerHour;

					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].usFatigue = pData->curAssignmentData.usFatigue;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].usSleep = pData->curAssignmentData.usSleep;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].usKitDegrade = pData->curAssignmentData.usKitDegrade;

					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].sSkyriderCostModifier = pData->curAssignmentData.sSkyriderCostModifier;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].usMineIncomeModifier = pData->curAssignmentData.usMineIncomeModifier;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].fOnlyLocalMineAffected = pData->curAssignmentData.fOnlyLocalMineAffected;

					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].sCantinaFoodModifier = pData->curAssignmentData.sCantinaFoodModifier;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].usPrisonBaseLimit = pData->curAssignmentData.usPrisonBaseLimit;

					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMaximumBreath = pData->curAssignmentData.ubMaximumBreath;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMaximumMorale = pData->curAssignmentData.ubMaximumMorale;

					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].fCountEnemiesInCities = pData->curAssignmentData.fCountEnemiesInCities;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].fCountEnemiesInWild = pData->curAssignmentData.fCountEnemiesInWild;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].fDetectEnemiesAnywhere = pData->curAssignmentData.fDetectEnemiesAnywhere;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].fDetectEnemiesDynamic = pData->curAssignmentData.fDetectEnemiesDynamic;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubDetectEnemiesImmediate = pData->curAssignmentData.ubDetectEnemiesImmediate;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].fDetectEnemiesLongrange = pData->curAssignmentData.fDetectEnemiesLongrange;

					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumAgility = pData->curAssignmentData.ubMinimumAgility;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumBreath = pData->curAssignmentData.ubMinimumBreath;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumDexterity = pData->curAssignmentData.ubMinimumDexterity;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumExplosives = pData->curAssignmentData.ubMinimumExplosives;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumHealth = pData->curAssignmentData.ubMinimumHealth;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumLeadership = pData->curAssignmentData.ubMinimumLeadership;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumLevel = pData->curAssignmentData.ubMinimumLevel;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumLoyaltyHere = pData->curAssignmentData.ubMinimumLoyaltyHere;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumMarksmanship = pData->curAssignmentData.ubMinimumMarksmanship;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumMechanical = pData->curAssignmentData.ubMinimumMechanical;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumMedical = pData->curAssignmentData.ubMinimumMedical;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumMorale = pData->curAssignmentData.ubMinimumMorale;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumStrength = pData->curAssignmentData.ubMinimumStrength;
					pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].ubMinimumWisdom = pData->curAssignmentData.ubMinimumWisdom;

					// Set RISK data
					for (UINT16 cnt = 0; cnt < NUM_RISKS; cnt++)
					{
						pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].Risk[cnt].usChance = pData->curAssignmentData.Risk[cnt].usChance;
						pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].Risk[cnt].bBaseEffect = pData->curAssignmentData.Risk[cnt].bBaseEffect;
						pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].Risk[cnt].ubRange = pData->curAssignmentData.Risk[cnt].ubRange;
					}
				}
				else
				{
					wcscpy(pData->curFacilityTypeData.AssignmentData[pData->curAssignmentType].szTooltipText, pData->curAssignmentData.szTooltipText);
				}
			
			}
			
		}
		else if ( strcmp( name, "PRODUCTIONLINE" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_TYPE;

			// Make sure an production is selected for data dump
			if ( pData->curProduction == -1 )
			{
				CHAR16 sErrorString[256];
				swprintf( sErrorString, L"FacilityTypes.XML Error: Production data for facility type %d is missing a <ubAssignmentType> tag", pData->curIndex );
				SGP_THROW( sErrorString );
			}
			else
			{
				// Set data from memory
				pData->curFacilityTypeData.ProductionData.push_back( pData->curProductionData );
			}
		}

		////////////////////////////////////////////////
		// Specific Assignment Data

		else if(strcmp(name, "ubAssignmentType") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			// Set the assignment type we're currently reading. This is important, as if this does not occur,
			// the program doesn't know where to put the rest of the assignment data, and will throw an on-screen error
			// as a result.
			if (strcmp(pData->szCharData, "AMBIENT") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_AMBIENT;
			}
			else if (strcmp(pData->szCharData, "STAFF") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STAFF;
			}
			else if (strcmp(pData->szCharData, "FACILITY_EAT") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_FOOD;
			}
			else if (strcmp(pData->szCharData, "REST") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_REST;
			}
			else if (strcmp(pData->szCharData, "DOCTOR") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_DOCTOR;
			}
			else if (strcmp(pData->szCharData, "PATIENT") == 0)
			{
				pData->curAssignmentType = FAC_PATIENT;
			}
			else if (strcmp(pData->szCharData, "REPAIR_ITEMS") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_REPAIR_ITEMS;
			}
			else if (strcmp(pData->szCharData, "REPAIR_VEHICLE") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_REPAIR_VEHICLE;
			}
			else if (strcmp(pData->szCharData, "REPAIR_ROBOT") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_REPAIR_ROBOT;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_STRENGTH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_STRENGTH;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_DEXTERITY") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_DEXTERITY;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_AGILITY") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_AGILITY;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_HEALTH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_HEALTH;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_MARKSMANSHIP") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_MARKSMANSHIP;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_LEADERSHIP") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_LEADERSHIP;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_MEDICAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_MEDICAL;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_MECHANICAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_MECHANICAL;
			}
			else if (strcmp(pData->szCharData, "PRACTICE_EXPLOSIVES") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRACTICE_EXPLOSIVES;
			}
			else if (strcmp(pData->szCharData, "TRAINER_STRENGTH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_STRENGTH;
			}
			else if (strcmp(pData->szCharData, "TRAINER_DEXTERITY") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_DEXTERITY;
			}
			else if (strcmp(pData->szCharData, "TRAINER_AGILITY") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_AGILITY;
			}
			else if (strcmp(pData->szCharData, "TRAINER_HEALTH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_HEALTH;
			}
			else if (strcmp(pData->szCharData, "TRAINER_MARKSMANSHIP") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_MARKSMANSHIP;
			}
			else if (strcmp(pData->szCharData, "TRAINER_LEADERSHIP") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_LEADERSHIP;
			}
			else if (strcmp(pData->szCharData, "TRAINER_MECHANICAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_MECHANICAL;
			}
			else if (strcmp(pData->szCharData, "TRAINER_MEDICAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_MEDICAL;
			}
			else if (strcmp(pData->szCharData, "TRAINER_EXPLOSIVES") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_TRAINER_EXPLOSIVES;
			}
			else if (strcmp(pData->szCharData, "STUDENT_STRENGTH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_STRENGTH;
			}
			else if (strcmp(pData->szCharData, "STUDENT_DEXTERITY") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_DEXTERITY;
			}
			else if (strcmp(pData->szCharData, "STUDENT_AGILITY") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_AGILITY;
			}
			else if (strcmp(pData->szCharData, "STUDENT_HEALTH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_HEALTH;
			}
			else if (strcmp(pData->szCharData, "STUDENT_MARKSMANSHIP") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_MARKSMANSHIP;
			}
			else if (strcmp(pData->szCharData, "STUDENT_LEADERSHIP") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_LEADERSHIP;
			}
			else if (strcmp(pData->szCharData, "STUDENT_MECHANICAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_MECHANICAL;
			}
			else if (strcmp(pData->szCharData, "STUDENT_MEDICAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_MEDICAL;
			}
			else if (strcmp(pData->szCharData, "STUDENT_EXPLOSIVES") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_STUDENT_EXPLOSIVES;
			}
			else if (strcmp(pData->szCharData, "INTERROGATE_PRISONERS") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_INTERROGATE_PRISONERS;
			}
			else if (strcmp(pData->szCharData, "PRISON_SNITCH") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_PRISON_SNITCH;
			}
			else if (strcmp(pData->szCharData, "SPREAD_PROPAGANDA") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_SPREAD_PROPAGANDA;
			}	
			else if (strcmp(pData->szCharData, "SPREAD_PROPAGANDA_GLOBAL") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_SPREAD_PROPAGANDA_GLOBAL;
			}
			else if (strcmp(pData->szCharData, "GATHER_RUMOURS") == 0)
			{
				//pData->curAssignmentType = (INT16) atol(pData->szCharData);
				pData->curAssignmentType = FAC_GATHER_RUMOURS;
			}
			else if (strcmp(pData->szCharData, "STRATEGIC_MILITIA_MOVEMENT") == 0)
			{
				pData->curAssignmentType = FAC_STRATEGIC_MILITIA_MOVEMENT;
			}	
			else
			{
				CHAR16 sErrorString[256];
				swprintf(sErrorString, L"FacilityTypes.XML Error: Assignment type for facility %d is unrecognized!", pData->curIndex);
				SGP_THROW(sErrorString);
			}
		}

		else if(strcmp(name, "ubStaffLimit") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.ubStaffLimit = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "szTooltipText") == 0)
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curAssignmentData.szTooltipText, sizeof(pData->curAssignmentData.szTooltipText)/sizeof(pData->curAssignmentData.szTooltipText[0]) );
			pData->curAssignmentData.szTooltipText[sizeof(pData->curAssignmentData.szTooltipText)/sizeof(pData->curAssignmentData.szTooltipText[0]) - 1] = '\0';
		}

		else if(strcmp(name, "sCostPerHour") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.sCostPerHour = (INT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "usPerformance") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.usPerformance = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "usFatigue") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.usFatigue = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "usSleep") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.usSleep = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "usKitDegrade") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.usKitDegrade = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMaximumMorale") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.ubMaximumMorale = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMaximumBreath") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.ubMaximumBreath = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubDetectEnemiesImmediate") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.ubDetectEnemiesImmediate = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "fDetectEnemiesDynamic") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.fDetectEnemiesDynamic = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "fDetectEnemiesLongrange") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.fDetectEnemiesLongrange = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "fDetectEnemiesAnywhere") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.fDetectEnemiesAnywhere = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "fCountEnemiesInWild") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.fCountEnemiesInWild = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "fCountEnemiesInCities") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.fCountEnemiesInCities = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "sSkyriderCostModifier") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.sSkyriderCostModifier = (INT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "usMineIncomeModifier") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.usMineIncomeModifier = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "fOnlyLocalMineAffected") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.fOnlyLocalMineAffected = (BOOLEAN) atol(pData->szCharData);
		}

		else if(strcmp(name, "sCantinaFoodModifier") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.sCantinaFoodModifier = (INT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "usPrisonBaseLimit") == 0 )
		{
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
			pData->curAssignmentData.usPrisonBaseLimit = (UINT16) atol(pData->szCharData);
		}

		//////////////////////////////////////////////
		// Conditions for assignment

		else if(strcmp(name, "CONDITIONS") == 0)
		{
			// No data write, just downgrade parse stage.
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
		}

		else if(strcmp(name, "ubMinimumStrength") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumStrength = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumHealth") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumHealth = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumWisdom") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumWisdom = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumAgility") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumAgility = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumDexterity") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumDexterity = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumMarksmanship") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumMarksmanship = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumLeadership") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumLeadership = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumExplosives") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumExplosives = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumMechanical") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumMechanical = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumMedical") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumMedical = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumLevel") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumLevel = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumLoyaltyHere") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumLoyaltyHere = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumBreath") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumBreath = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMinimumMorale") == 0 )
		{
			pData->curElement = FACILITYTYPE_CONDITIONS;
			pData->curAssignmentData.ubMinimumMorale = (UINT8) atol(pData->szCharData);
		}

		////////////////////////////////////////
		// Risk data for assignment

		else if(strcmp(name, "STRENGTH") == 0 ||
			strcmp(name, "AGILITY") == 0 ||
			strcmp(name, "DEXTERITY") == 0 ||
			strcmp(name, "WISDOM") == 0 ||
			strcmp(name, "HEALTH") == 0 ||
			strcmp(name, "MARKSMANSHIP") == 0 ||
			strcmp(name, "LEADERSHIP") == 0 ||
			strcmp(name, "MECHANICAL") == 0 ||
			strcmp(name, "EXPLOSIVES") == 0 ||
			strcmp(name, "MEDICAL") == 0 ||
			strcmp(name, "INJURY") == 0 ||
			strcmp(name, "MORALE") == 0 ||
			strcmp(name, "FATIGUE") == 0 ||
			strcmp(name, "LOYALTY_LOCAL") == 0 ||
			strcmp(name, "LOYALTY_GLOBAL") == 0 ||
			strcmp(name, "DRUNK") == 0 )
		{
			// No data write, simply downgrade the parse stage
			pData->curElement = FACILITYTYPE_ASSIGNMENT;
		}
		
		else if(strcmp(name, "ubChance") == 0 )
		{
			pData->curElement = FACILITYTYPE_RISK;
			pData->curAssignmentData.Risk[pData->curRisk].usChance = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "bBaseEffect") == 0 )
		{
			pData->curElement = FACILITYTYPE_RISK;
			pData->curAssignmentData.Risk[pData->curRisk].bBaseEffect = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubRange") == 0 )
		{
			pData->curElement = FACILITYTYPE_RISK;
			pData->curAssignmentData.Risk[pData->curRisk].ubRange = (UINT8) atol(pData->szCharData);
		}
		
		else if ( strcmp( name, "szProductionName" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curProductionData.szProductionName, sizeof( pData->curProductionData.szProductionName ) / sizeof( pData->curProductionData.szProductionName[0] ) );
			pData->curProductionData.szProductionName[sizeof( pData->curProductionData.szProductionName ) / sizeof( pData->curProductionData.szProductionName[0] ) - 1] = '\0';
		}
		else if ( strcmp( name, "szAdditionalRequirementTips" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curProductionData.szAdditionalRequirementTips, sizeof( pData->curProductionData.szAdditionalRequirementTips ) / sizeof( pData->curProductionData.szAdditionalRequirementTips[0] ) );
			pData->curProductionData.szAdditionalRequirementTips[sizeof( pData->curProductionData.szAdditionalRequirementTips ) / sizeof( pData->curProductionData.szAdditionalRequirementTips[0] ) - 1] = '\0';
		}
		else if ( strcmp( name, "usItemToCreate" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			pData->curProductionData.usItemToCreate = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "sMinutesRequired" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			pData->curProductionData.sMinutesRequired = max(1, (INT32)atol( pData->szCharData ) );
		}
		else if ( strcmp( name, "sGridNo_Creation" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			pData->curProductionData.sGridNo_Creation = (INT32)atol( pData->szCharData );
		}
		else if ( strcmp( name, "requires_staff" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			if ( (UINT32)atol( pData->szCharData ) )
				pData->curProductionData.usProductionFlags |= REQUIRES_STAFFING;
		}
		else if ( strcmp( name, "sHourlyCost" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			pData->curProductionData.sHourlyCost = (INT32)atol( pData->szCharData );
		}
		else if ( strcmp( name, "usOptional_LoyaltyRequired" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			pData->curProductionData.usOptional_LoyaltyRequired = (UINT8)atol( pData->szCharData );
		}
		else if ( strcmp( name, "usOptional_ItemUsedUp" ) == 0 )
		{
			pData->curElement = FACILITYTYPE_PRODUCTION;
			UINT16 requireditem = (UINT16)atol( pData->szCharData );

			bool found = false;
			for ( std::vector<PRODUCTION_LINE_PREPRODUCT>::size_type i = 0; i < pData->curProductionData.usOptional_PreProducts.size(); ++i )
			{
				if ( pData->curProductionData.usOptional_PreProducts[i].item == requireditem )
				{
					pData->curProductionData.usOptional_PreProducts[i].requiredforonecreation++;
					found = true;
					break;
				}
			}

			if ( !found )
			{
				PRODUCTION_LINE_PREPRODUCT data;
				data.item = requireditem;
				data.requiredforonecreation = 1;
				pData->curProductionData.usOptional_PreProducts.push_back( data );
			}
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}



BOOLEAN ReadInFacilityTypes(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	facilitytypeParseData pData;
	
	FacilityTypes_TextOnly = localizedVersion;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading FacilityTypes.xml" );

	// Open merges file
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


	XML_SetElementHandler(parser, facilitytypeStartElementHandle, facilitytypeEndElementHandle);
	XML_SetCharacterDataHandler(parser, facilitytypeCharacterDataHandle);


	memset(&pData, 0, FACILITYTYPEPARSEDATA_SIZE_OF_POD);
	pData.maxArraySize = MAXITEMS;
	pData.curIndex = 0;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in FacilityTypes.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}

