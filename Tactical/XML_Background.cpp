#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	BACKGROUND_VALUES	curBackground;
	BACKGROUND_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzBackground[MAX_ENEMY_NAMES_CHARS];
}
typedef enemyRankParseData;

BOOLEAN localizedTextOnly_BG;

UINT16 num_found_background = 0;	// the correct number is set on reading the xml

#define XML_BACKGROUND_AP_MAX		 8
#define XML_BACKGROUND_STAT_MAX		10
#define XML_BACKGROUND_TRAVEL_MAX	20
#define XML_BACKGROUND_RESI_MAX		20
#define XML_VARIOUS_MAX			   300

static void XMLCALL
backgroundStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "BACKGROUNDS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			if ( !localizedTextOnly_BG )
				memset(pData->curArray,0,sizeof(BACKGROUND_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "BACKGROUND") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			if ( !localizedTextOnly_BG )
				memset(&pData->curBackground,0,sizeof(BACKGROUND_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "szName") == 0 ||
				strcmp(name, "szShortName") == 0 ||				
				strcmp(name, "szDescription") == 0 ||				
				strcmp(name, "ap_polar") == 0 ||
				strcmp(name, "ap_desert") == 0 ||
				strcmp(name, "ap_swamp") == 0 ||
				strcmp(name, "ap_urban") == 0 ||
				strcmp(name, "ap_river") == 0 ||
				strcmp(name, "ap_coastal") == 0 ||
				strcmp(name, "ap_tropical") == 0 ||
				strcmp(name, "ap_mountain") == 0 ||				
				strcmp(name, "ap_height") == 0 ||
				strcmp(name, "ap_swimming") == 0 ||
				strcmp(name, "ap_fortify") == 0 ||
				strcmp(name, "ap_artillery") == 0 ||
				strcmp(name, "ap_inventory") == 0 ||
				strcmp(name, "ap_airdrop") == 0 ||
				strcmp(name, "ap_assault") == 0 ||
				strcmp(name, "agility") == 0 ||
				strcmp(name, "strength") == 0 ||
				strcmp(name, "dexterity") == 0 ||
				strcmp(name, "mechanical") == 0 ||
				strcmp(name, "medical") == 0 ||
				strcmp(name, "wisdom") == 0 ||
				strcmp(name, "explosives") == 0 ||
				strcmp(name, "leadership") == 0 ||
				strcmp(name, "marksmanship") == 0 ||
				strcmp(name, "travel_foot") == 0 ||
				strcmp(name, "travel_car") == 0 ||
				strcmp(name, "travel_air") == 0 ||
				strcmp(name, "travel_boat") == 0 ||
				strcmp(name, "resistance_fear") == 0 ||
				strcmp(name, "resistance_suppression") == 0 ||
				strcmp(name, "resistance_physical") == 0 ||
				strcmp(name, "resistance_alcohol") == 0 ||
				strcmp(name, "resistance_disease" ) == 0 ||
				strcmp(name, "interrogation") == 0 ||
				strcmp(name, "prisonguard") == 0 ||
				strcmp(name, "betterprices_guns") == 0 ||
				strcmp(name, "betterprices") == 0 ||
				strcmp(name, "capitulation") == 0 ||
				strcmp(name, "speed_run") == 0 ||
				strcmp(name, "speed_bandaging") == 0 ||
				strcmp(name, "drink_energyregen") == 0 ||
				strcmp(name, "carrystrength") == 0 ||
				strcmp(name, "food") == 0 ||
				strcmp(name, "water") == 0 ||
				strcmp(name, "sleep") == 0 ||
				strcmp(name, "meleedamage") == 0 ||
				strcmp(name, "cth_blades") == 0 ||
				strcmp(name, "camo") == 0 ||
				strcmp(name, "stealth") == 0 ||
				strcmp(name, "increased_maxcth") == 0 ||
				strcmp(name, "hearing_night") == 0 ||
				strcmp(name, "hearing_day") == 0 ||
				strcmp(name, "disarm_trap") == 0 ||
				strcmp(name, "SAM_cth" ) == 0 ||
				strcmp(name, "approach_friendly") == 0 ||
				strcmp(name, "approach_direct") == 0 ||
				strcmp(name, "approach_threaten") == 0 ||
				strcmp(name, "approach_recruit") == 0 ||
				strcmp(name, "breachingcharge") == 0 ||
				strcmp(name, "cth_vs_creatures") == 0 ||
				strcmp(name, "insurance") == 0 ||
				strcmp(name, "spotter") == 0 ||
				strcmp(name, "disease_diagnose" ) == 0 ||
				strcmp(name, "disease_treatment" ) == 0 ||
				strcmp(name, "tracker_ability" ) == 0 ||
				strcmp(name, "ambush_radius" ) == 0 ||
				strcmp(name, "dislikebackground" ) == 0 ||
				strcmp(name, "smoker" ) == 0 ||
				strcmp(name, "croucheddefense" ) == 0 ||
				strcmp(name, "fortify_assignment" ) == 0 ||
				strcmp(name, "druguse") == 0 ||
				strcmp(name, "xenophobic") == 0 ||				
				strcmp(name, "corruptionspread") == 0 ||
				strcmp(name, "level_underground") == 0 ||
				strcmp(name, "scrounging") == 0 ||
				strcmp(name, "traplevel") == 0 ||
				strcmp(name, "no_male") == 0 ||
				strcmp(name, "no_female") == 0 ||
				strcmp(name, "loyalitylossondeath" ) == 0 ))

		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
backgroundCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
backgroundEndElementHandle(void *userData, const XML_Char *name)
{
	enemyRankParseData * pData = (enemyRankParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "BACKGROUNDS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "BACKGROUND") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if(pData->curBackground.uiIndex < pData->maxArraySize)
			{
				if ( localizedTextOnly_BG )
				{
					wcscpy(pData->curArray[pData->curBackground.uiIndex].szName,pData->curBackground.szName);
					wcscpy(pData->curArray[pData->curBackground.uiIndex].szShortName,pData->curBackground.szShortName);
					wcscpy(pData->curArray[pData->curBackground.uiIndex].szDescription,pData->curBackground.szDescription);
				}
				else
					pData->curArray[pData->curBackground.uiIndex] = pData->curBackground;
			}
		
			num_found_background = pData->curBackground.uiIndex;	
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curBackground.szName, sizeof(pData->curBackground.szName)/sizeof(pData->curBackground.szName[0]) );
			pData->curBackground.szName[sizeof(pData->curBackground.szName)/sizeof(pData->curBackground.szName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szShortName") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curBackground.szShortName, sizeof(pData->curBackground.szShortName)/sizeof(pData->curBackground.szShortName[0]) );
			pData->curBackground.szShortName[sizeof(pData->curBackground.szShortName)/sizeof(pData->curBackground.szShortName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "szDescription") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curBackground.szDescription, sizeof(pData->curBackground.szDescription)/sizeof(pData->curBackground.szDescription[0]) );
			pData->curBackground.szDescription[sizeof(pData->curBackground.szDescription)/sizeof(pData->curBackground.szDescription[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ap_polar") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_POLAR] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_desert") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_DESERT] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_swamp") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_SWAMP] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_urban") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_URBAN] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_river") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_RIVER] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_coastal") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_COASTAL] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_tropical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_TROPICAL] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_mountain") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_MOUNTAIN] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_height") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_HEIGHT] = min(XML_BACKGROUND_AP_MAX, max(-XML_BACKGROUND_AP_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_swimming") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_SWIMMING] = min(40, max(-40, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_fortify") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_FORTIFY] = min(40, max(-40, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_artillery") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_ARTILLERY] = min(40, max(-40, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_inventory") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_INVENTORY] = min(40, max(-40, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_airdrop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_AIRDROP] = min(40, max(-40, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "ap_assault") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_ASSAULT] = min(10, max(-10, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "agility") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_AGILITY] = min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "strength") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_STRENGTH]	= min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "dexterity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_DEXTERITY] = min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "mechanical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_MECHANICAL] = min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "medical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_MEDICAL] = min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "wisdom") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_WISDOM] = min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "explosives") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_EXPLOSIVE_ASSIGN]	= min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "leadership") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_LEADERSHIP]	= min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "marksmanship") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_MARKSMANSHIP]	= min(XML_BACKGROUND_STAT_MAX, max(-XML_BACKGROUND_STAT_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "travel_foot") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_TRAVEL_FOOT] = min(XML_BACKGROUND_TRAVEL_MAX, max(-XML_BACKGROUND_TRAVEL_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "travel_car") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_TRAVEL_CAR] = min(XML_BACKGROUND_TRAVEL_MAX, max(-XML_BACKGROUND_TRAVEL_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "travel_air") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_TRAVEL_AIR] = min(XML_BACKGROUND_TRAVEL_MAX, max(-XML_BACKGROUND_TRAVEL_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "travel_boat") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_TRAVEL_BOAT] = min(XML_BACKGROUND_TRAVEL_MAX, max(-XML_BACKGROUND_TRAVEL_MAX, (INT8) atol(pData->szCharData) ));
		}		
		else if(strcmp(name, "resistance_fear") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_RESI_FEAR] = min(XML_BACKGROUND_RESI_MAX, max(-XML_BACKGROUND_RESI_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "resistance_suppression") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_RESI_SUPPRESSION] = min(XML_BACKGROUND_RESI_MAX, max(-XML_BACKGROUND_RESI_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "resistance_physical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_RESI_PHYSICAL] = min(10, max(-10, (INT8) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "resistance_alcohol") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_RESI_ALCOHOL] = min(XML_BACKGROUND_RESI_MAX, max(-XML_BACKGROUND_RESI_MAX, (INT8) atol(pData->szCharData) ));
		}
		else if ( strcmp( name, "resistance_disease" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_RESI_DISEASE] = min( XML_BACKGROUND_RESI_MAX, max( -XML_BACKGROUND_RESI_MAX, (INT8)atol( pData->szCharData ) ) );
		}
		else if(strcmp(name, "interrogation") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_INTERROGATION] = min(XML_VARIOUS_MAX, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "prisonguard") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_GUARD] = min(XML_VARIOUS_MAX, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "betterprices_guns") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_PRICES_GUNS] = min(10, max(-10, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "betterprices") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_PRICES] = min(10, max(-10, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "capitulation") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_CAPITULATION] = min(100, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "speed_run") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_SPEED_RUNNING] = min(30, max(-30, (INT16) atol(pData->szCharData) ));
		}	
		else if(strcmp(name, "speed_bandaging") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_BANDAGING] = min(50, max(-50, (INT16) atol(pData->szCharData) ));
		}		
		else if(strcmp(name, "drink_energyregen") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_REGEN_ENERGY] = min(XML_VARIOUS_MAX, max(-80, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "carrystrength") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_CARRYSTRENGTH] = min(20, max(-20, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "food") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_FOOD] = min(100, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "water") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_WATER] = min(100, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "sleep") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_SLEEP] = min(1, max(-1, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "meleedamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_DAMAGE_MELEE] = min(10, max(-10, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "cth_blades") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_CTH_BLADE] = min(10, max(-10, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "camo") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_CAMO] = min(10, max(-20, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "stealth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_STEALTH] = min(10, max(-20, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "increased_maxcth") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_CTH_MAX] = min(5, max(-5, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "hearing_night") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_HEARING_NIGHT] = min(2, max(-5, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "hearing_day") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_HEARING_DAY] = min(2, max(-5, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "disarm_trap") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_DISARM] = min( 50, max( -50, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "SAM_cth" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_SAM_CTH] = min( 100, max( -50, (INT16)atol( pData->szCharData ) ) );
		}
		else if(strcmp(name, "approach_friendly") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_APPROACH_FRIENDLY] = min(20, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "approach_direct") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_APPROACH_DIRECT] = min(20, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "approach_threaten") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_APPROACH_THREATEN] = min(20, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "approach_recruit") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_APPROACH_RECRUIT] = min(20, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "breachingcharge") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_BONUS_BREACHINGCHARGE] = min(100, max(-100, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "cth_vs_creatures") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_CTH_CREATURE] = min(10, max(-10, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "insurance") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_INSURANCE] = min(200, max(-50, (INT16) atol(pData->szCharData) ));
		}
		else if(strcmp(name, "spotter") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_SPOTTER] = min(30, max(-30, (INT16) atol(pData->szCharData) ));
		}
		else if ( strcmp( name, "disease_diagnose" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_DISEASE_DIAGNOSE] = min( 50, max( -50, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "disease_treatment" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_PERC_DISEASE_TREAT] = min( 50, max( -50, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "tracker_ability" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_TRACKER_ABILITY] = min( 40, max( 0, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "ambush_radius" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_AMBUSH_RADIUS] = min( 50, max( 0, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "dislikebackground" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_DISLIKEBG] = (INT16)atol( pData->szCharData );
		}				
		else if ( strcmp( name, "smoker" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_SMOKERTYPE] = min( 2, max( 0, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "croucheddefense" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_CROUCHEDDEFENSE] = min( 30, max( -30, (INT16)atol( pData->szCharData ) ) );
		}
		else if ( strcmp( name, "fortify_assignment" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.value[BG_FORTIFY_ASSIGNMENT] = min( 200, max( -50, (INT16)atol( pData->szCharData ) ) );
		}
		else if(strcmp(name, "druguse") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_DRUGUSE : 0;
		}
		else if(strcmp(name, "xenophobic") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_XENOPHOBIC : 0;
		}
		else if(strcmp(name, "corruptionspread") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_CORRUPTIONSPREAD : 0;
		}		
		else if(strcmp(name, "level_underground") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_EXP_UNDERGROUND : 0;
		}
		else if(strcmp(name, "scrounging") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_SCROUNGING : 0;
		}
		else if(strcmp(name, "traplevel") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_TRAPLEVEL : 0;
		}
		else if(strcmp(name, "no_male") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_NO_MALE : 0;
		}
		else if(strcmp(name, "no_female") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags	|= (UINT16) atol(pData->szCharData) ? BACKGROUND_NO_FEMALE : 0;
		}
		else if ( strcmp( name, "loyalitylossondeath" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curBackground.uiFlags |= (UINT16)atol( pData->szCharData ) ? BACKGROUND_GLOBALOYALITYLOSSONDEATH : 0;
		}
																						
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInBackgrounds(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	enemyRankParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Backgrounds.xml" );

	localizedTextOnly_BG = localizedVersion;
		
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );

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


	XML_SetElementHandler(parser, backgroundStartElementHandle, backgroundEndElementHandle);
	XML_SetCharacterDataHandler(parser, backgroundCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = zBackground;
	pData.maxArraySize = NUM_BACKGROUND;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Backgrounds.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteBackgrounds( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<BACKGROUNDS>\r\n");
		for(cnt = 0; cnt < NUM_BACKGROUND; ++cnt)
		{
			FilePrintf(hFile,"\t<BACKGROUND>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt);
			
			FilePrintf(hFile,"\t</BACKGROUND>\r\n");
		}
		FilePrintf(hFile,"</BACKGROUNDS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
