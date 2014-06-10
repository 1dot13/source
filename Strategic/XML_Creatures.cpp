#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"

#else
	#include "expat.h"
	#include "string.h"
	#include "Campaign Types.h"
	#include "FileMan.h"
	#include "MemMan.h"
	#include "Debug Control.h"
	#include "Creature Spreading.h"
#endif

// Buggler: creature XML externalization stuff
#define MAX_CHAR_DATA_LENGTH			500
#define INVALID_CREATURESITE_INDEX			-1
#define INVALID_COMPOSITION_INDEX			-1

// This array stores all XML-read creature placement data, valid giLairID starts at 1
CREATUREPLACEMENT gCreaturePlacements[ MAX_NUMBER_OF_INFECTIBLE_SITES + 1];
CREATURECOMPOSITION gCreatureComposition[ MAX_NUMBER_OF_CREATURE_COMPOSITIONS ];


// actual infectible sites defined in XML
UINT8	NUMBER_OF_INFECTIBLE_SITES;
UINT8	NUMBER_OF_CREATURE_COMPOSITIONS;

typedef enum
{
	CREATURE_ELEMENT_NONE = 0,
	CREATURE_ELEMENT_CREATUREINFO,
	CREATURE_ELEMENT_COMPOSITIONLIST,
	CREATURE_ELEMENT_COMPOSITION,
	CREATURE_ELEMENT_COMPINDEX,
	CREATURE_ELEMENT_LARVAEPERCENT,
	CREATURE_ELEMENT_INFANTPERCENT,
	CREATURE_ELEMENT_YMALEPERCENT,
	CREATURE_ELEMENT_YFEMALEPERCENT,
	CREATURE_ELEMENT_AMALEPERCENT,
	CREATURE_ELEMENT_AFEMALEPERCENT,
	CREATURE_ELEMENT_MAXPOPULATION,
	CREATURE_ELEMENT_PLACEMENTLIST,
	CREATURE_ELEMENT_PLACEMENT,
	CREATURE_ELEMENT_INDEX,
	CREATURE_ELEMENT_ATTACKSOURCE,
	CREATURE_ELEMENT_ATTACKSOURCE_SECTOR,
	CREATURE_ELEMENT_ATTACKSOURCE_GRIDNO,
	CREATURE_ELEMENT_ALTERNATEMAP,
	CREATURE_ELEMENT_ALTERNATEMAP_SECTOR,
	CREATURE_ELEMENT_ALTERNATEMAP_Z,
	CREATURE_ELEMENT_WARPTO,
	CREATURE_ELEMENT_WARPTO_SECTOR,
	CREATURE_ELEMENT_WARPTO_Z,
	CREATURE_ELEMENT_WARPTO_GRIDNO,
	CREATURE_ELEMENT_CREATURESECTOR,
	CREATURE_ELEMENT_QUEEN,
	CREATURE_ELEMENT_QUEEN_SECTOR,
	CREATURE_ELEMENT_QUEEN_Z,
	CREATURE_ELEMENT_HABITAT,
	CREATURE_ELEMENT_HABITAT_SECTOR,
	CREATURE_ELEMENT_HABITAT_Z,
	CREATURE_ELEMENT_HABITAT_COMPOSITION,
	CREATURE_ELEMENT_HABITAT_VALID,
} CREATURE_PARSE_STAGE;

typedef struct
{
	CREATURE_PARSE_STAGE	curElement;
	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];

	CREATUREPLACEMENT		curPlacementInfo;
	CREATURECOMPOSITION		curCompositionInfo;
	UINT32					curIndex;
	UINT32					uiHighestIndex;
	UINT32					uiCompHighestIndex;
	UINT32					curHabitat;
	
	UINT32					currentDepth;
	UINT32					maxReadDepth;
} CreatureParseData;

static void XMLCALL
creatureplacementStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	CreatureParseData * pData = (CreatureParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "CREATURE_INFO") == 0 && pData->curElement == CREATURE_ELEMENT_NONE)
		{
			pData->curElement = CREATURE_ELEMENT_CREATUREINFO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "COMPOSITION_LIST") == 0 && pData->curElement == CREATURE_ELEMENT_CREATUREINFO)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITIONLIST;
			pData->maxReadDepth++; //we are not skipping this element

			// Initiate Array by setting all values to 0
			for (UINT16 x = 0; x < MAX_NUMBER_OF_CREATURE_COMPOSITIONS; x++)
			{
				gCreatureComposition[x].ubLarvaePercent			= 0;
				gCreatureComposition[x].ubInfantPercent			= 0;
				gCreatureComposition[x].ubYoungMalePercent		= 0;
				gCreatureComposition[x].ubYoungFemalePercent	= 0;
				gCreatureComposition[x].ubAdultMalePercent		= 0;
				gCreatureComposition[x].ubAdultFemalePercent	= 0;
				gCreatureComposition[x].iMaxPopulation			= 0;
			}
		}
		else if(strcmp(name, "COMPOSITION") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITIONLIST)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "CompIndex") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_COMPINDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "LarvaePercent") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_LARVAEPERCENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "InfantPercent") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_INFANTPERCENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "YoungMalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_YMALEPERCENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "YoungFemalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_YFEMALEPERCENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "AdultMalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_AMALEPERCENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "AdultFemalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_AFEMALEPERCENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "MaxPopulation") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_MAXPOPULATION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PLACEMENT_LIST") == 0 && pData->curElement == CREATURE_ELEMENT_CREATUREINFO)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENTLIST;
			pData->maxReadDepth++; //we are not skipping this element
			
			// Initiate Array by setting all values to 0
			for (UINT16 x = 0; x < MAX_NUMBER_OF_INFECTIBLE_SITES + 1; x++)
			{
				gCreaturePlacements[x].sAltMapX				= 0;
				gCreaturePlacements[x].sAltMapY				= 0;
				gCreaturePlacements[x].ubAltMapZ			= 0;
				gCreaturePlacements[x].sQueenX				= 0;
				gCreaturePlacements[x].sQueenY				= 0;
				gCreaturePlacements[x].ubQueenZ				= 0;
				gCreaturePlacements[x].sAttackSourceX			= 0;
				gCreaturePlacements[x].sAttackSourceY			= 0;
				gCreaturePlacements[x].iAttackSourceGridNo	= 0;
				gCreaturePlacements[x].sWarpToX					= 0;
				gCreaturePlacements[x].sWarpToY					= 0;
				gCreaturePlacements[x].ubWarpToZ					= 0;
				gCreaturePlacements[x].iWarpToGridNo				= 0;
				for (UINT16 y = 0; y < MAX_NUMBER_OF_CREATURE_SECTORS; y++)
				{
					gCreaturePlacements[x].Habitat[y].sX		= 0;
					gCreaturePlacements[x].Habitat[y].sY		= 0;
					gCreaturePlacements[x].Habitat[y].ubZ		= 0;
					gCreaturePlacements[x].Habitat[y].ubComposition	= 0;
					gCreaturePlacements[x].Habitat[y].fValid	= 0;
				}
			}
		}
		else if(strcmp(name, "PLACEMENT") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENTLIST)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Index") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENT)
		{
			pData->curElement = CREATURE_ELEMENT_INDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ATTACKSOURCE") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENT)
		{
			pData->curElement = CREATURE_ELEMENT_ATTACKSOURCE;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_ATTACKSOURCE)
		{
			pData->curElement = CREATURE_ELEMENT_ATTACKSOURCE_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "GridNo") == 0 && pData->curElement == CREATURE_ELEMENT_ATTACKSOURCE)
		{
			pData->curElement = CREATURE_ELEMENT_ATTACKSOURCE_GRIDNO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ALTERNATEMAP") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENT)
		{
			pData->curElement = CREATURE_ELEMENT_ALTERNATEMAP;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_ALTERNATEMAP)
		{
			pData->curElement = CREATURE_ELEMENT_ALTERNATEMAP_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_ALTERNATEMAP)
		{
			pData->curElement = CREATURE_ELEMENT_ALTERNATEMAP_Z;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "WARPTOSECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENT)
		{
			pData->curElement = CREATURE_ELEMENT_WARPTO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO)
		{
			pData->curElement = CREATURE_ELEMENT_WARPTO_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO)
		{
			pData->curElement = CREATURE_ELEMENT_WARPTO_Z;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "GridNo") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO)
		{
			pData->curElement = CREATURE_ELEMENT_WARPTO_GRIDNO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "CREATURESECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENT)
		{
			pData->curElement = CREATURE_ELEMENT_CREATURESECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "QUEENSECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_CREATURESECTOR)
		{
			pData->curElement = CREATURE_ELEMENT_QUEEN;
			pData->curHabitat = 0;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_QUEEN)
		{
			pData->curElement = CREATURE_ELEMENT_QUEEN_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_QUEEN)
		{
			pData->curElement = CREATURE_ELEMENT_QUEEN_Z;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "HABITATSECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_CREATURESECTOR)
		{
			pData->curElement = CREATURE_ELEMENT_HABITAT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT)
		{
			pData->curElement = CREATURE_ELEMENT_HABITAT_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT)
		{
			pData->curElement = CREATURE_ELEMENT_HABITAT_Z;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Composition") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT)
		{
			pData->curElement = CREATURE_ELEMENT_HABITAT_COMPOSITION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
creatureplacementCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	CreatureParseData * pData = (CreatureParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}

static void XMLCALL
creatureplacementEndElementHandle(void *userData, const XML_Char *name)
{
	CreatureParseData * pData = (CreatureParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "CREATURE_INFO") == 0 && pData->curElement == CREATURE_ELEMENT_CREATUREINFO)
		{
			pData->curElement = CREATURE_ELEMENT_NONE;
		}
		else if(strcmp(name, "COMPOSITION_LIST") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITIONLIST)
		{
			pData->curElement = CREATURE_ELEMENT_CREATUREINFO;
			
			// CompIndex starts at 0
			NUMBER_OF_CREATURE_COMPOSITIONS = pData->uiCompHighestIndex + 1;
		}
		else if(strcmp(name, "COMPOSITION") == 0 && pData->curElement == CREATURE_ELEMENT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITIONLIST;

			if ( pData->curIndex != INVALID_CREATURESITE_INDEX )
			{
				// valid creature composition index starts at 0
				gCreatureComposition[pData->curIndex].ubLarvaePercent = pData->curCompositionInfo.ubLarvaePercent;
				gCreatureComposition[pData->curIndex].ubInfantPercent = pData->curCompositionInfo.ubInfantPercent;
				gCreatureComposition[pData->curIndex].ubYoungMalePercent = pData->curCompositionInfo.ubYoungMalePercent;
				gCreatureComposition[pData->curIndex].ubYoungFemalePercent = pData->curCompositionInfo.ubYoungFemalePercent;
				gCreatureComposition[pData->curIndex].ubAdultMalePercent = pData->curCompositionInfo.ubAdultMalePercent;
				gCreatureComposition[pData->curIndex].ubAdultFemalePercent = pData->curCompositionInfo.ubAdultFemalePercent;
				gCreatureComposition[pData->curIndex].iMaxPopulation = pData->curCompositionInfo.iMaxPopulation;
			}
		}
		else if(strcmp(name, "CompIndex") == 0 && pData->curElement == CREATURE_ELEMENT_COMPINDEX)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curIndex = atol(pData->szCharData);
			// CompIndex starts at 0
			if ( pData->curIndex < 0 || pData->curIndex > MAX_NUMBER_OF_CREATURE_COMPOSITIONS )
			{
				pData->curIndex = INVALID_COMPOSITION_INDEX;
			}
			else if ( pData->curIndex > pData->uiCompHighestIndex )
			{
				pData->uiCompHighestIndex = pData->curIndex;
			}
		}
		else if(strcmp(name, "LarvaePercent") == 0 && pData->curElement == CREATURE_ELEMENT_LARVAEPERCENT)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.ubLarvaePercent = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "InfantPercent") == 0 && pData->curElement == CREATURE_ELEMENT_INFANTPERCENT)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.ubInfantPercent = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "YoungMalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_YMALEPERCENT)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.ubYoungMalePercent = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "YoungFemalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_YFEMALEPERCENT)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.ubYoungFemalePercent = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "AdultMalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_AMALEPERCENT)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.ubAdultMalePercent = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "AdultFemalePercent") == 0 && pData->curElement == CREATURE_ELEMENT_AFEMALEPERCENT)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.ubAdultFemalePercent = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "MaxPopulation") == 0 && pData->curElement == CREATURE_ELEMENT_MAXPOPULATION)
		{
			pData->curElement = CREATURE_ELEMENT_COMPOSITION;

			pData->curCompositionInfo.iMaxPopulation = (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "PLACEMENT_LIST") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENTLIST)
		{
			pData->curElement = CREATURE_ELEMENT_CREATUREINFO;

			NUMBER_OF_INFECTIBLE_SITES = pData->uiHighestIndex;
		}
		else if(strcmp(name, "PLACEMENT") == 0 && pData->curElement == CREATURE_ELEMENT_PLACEMENT)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENTLIST;

			if ( pData->curIndex != INVALID_CREATURESITE_INDEX )
			{
				// valid giLairID starts at 1
				gCreaturePlacements[pData->curIndex].sAltMapX = pData->curPlacementInfo.sAltMapX;
				gCreaturePlacements[pData->curIndex].sAltMapY = pData->curPlacementInfo.sAltMapY;
				gCreaturePlacements[pData->curIndex].ubAltMapZ = pData->curPlacementInfo.ubAltMapZ;
				gCreaturePlacements[pData->curIndex].sQueenX = pData->curPlacementInfo.sQueenX;
				gCreaturePlacements[pData->curIndex].sQueenY = pData->curPlacementInfo.sQueenY;
				gCreaturePlacements[pData->curIndex].ubQueenZ = pData->curPlacementInfo.ubQueenZ;
				gCreaturePlacements[pData->curIndex].sAttackSourceX = pData->curPlacementInfo.sAttackSourceX;
				gCreaturePlacements[pData->curIndex].sAttackSourceY = pData->curPlacementInfo.sAttackSourceY;
				gCreaturePlacements[pData->curIndex].iAttackSourceGridNo = pData->curPlacementInfo.iAttackSourceGridNo;
				gCreaturePlacements[pData->curIndex].sWarpToX = pData->curPlacementInfo.sWarpToX;
				gCreaturePlacements[pData->curIndex].sWarpToY = pData->curPlacementInfo.sWarpToY;
				gCreaturePlacements[pData->curIndex].ubWarpToZ = pData->curPlacementInfo.ubWarpToZ;
				gCreaturePlacements[pData->curIndex].iWarpToGridNo = pData->curPlacementInfo.iWarpToGridNo;

				for (UINT8 y = 0; y < MAX_NUMBER_OF_CREATURE_SECTORS; y++)
				{
					gCreaturePlacements[pData->curIndex].Habitat[y].sX = pData->curPlacementInfo.Habitat[y].sX;
					gCreaturePlacements[pData->curIndex].Habitat[y].sY = pData->curPlacementInfo.Habitat[y].sY;
					gCreaturePlacements[pData->curIndex].Habitat[y].ubZ = pData->curPlacementInfo.Habitat[y].ubZ;
					gCreaturePlacements[pData->curIndex].Habitat[y].ubComposition = pData->curPlacementInfo.Habitat[y].ubComposition;
					gCreaturePlacements[pData->curIndex].Habitat[y].fValid = pData->curPlacementInfo.Habitat[y].fValid;

					// Clear pData Habitat Array as different creature site has different number of habitats
					pData->curPlacementInfo.Habitat[y].sX		= 0;
					pData->curPlacementInfo.Habitat[y].sY		= 0;
					pData->curPlacementInfo.Habitat[y].ubZ		= 0;
					pData->curPlacementInfo.Habitat[y].ubComposition	= 0;
					pData->curPlacementInfo.Habitat[y].fValid	= 0;
				}			
			}
		}
		else if(strcmp(name, "Index") == 0 && pData->curElement == CREATURE_ELEMENT_INDEX)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENT;

			pData->curIndex = atol(pData->szCharData);
			if ( !pData->curIndex || pData->curIndex > MAX_NUMBER_OF_INFECTIBLE_SITES + 1 )
			{
				pData->curIndex = INVALID_CREATURESITE_INDEX;
			}
			else if ( pData->curIndex > pData->uiHighestIndex )
			{
				pData->uiHighestIndex = pData->curIndex;
			}
		}
		else if(strcmp(name, "ATTACKSOURCE") == 0 && pData->curElement == CREATURE_ELEMENT_ATTACKSOURCE)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENT;
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_ATTACKSOURCE_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = CREATURE_ELEMENT_ATTACKSOURCE;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curPlacementInfo.sAttackSourceX	= x;
				pData->curPlacementInfo.sAttackSourceY	= y;
			}
			SGP_THROW_IFFALSE( (SECTOR(x,y) >= 0 && SECTOR(x,y) < 256) , L"Illegal sector number in CreaturePlacements.XML" );
		}
		else if(strcmp(name, "GridNo") == 0 && pData->curElement == CREATURE_ELEMENT_ATTACKSOURCE_GRIDNO)
		{
			pData->curElement = CREATURE_ELEMENT_ATTACKSOURCE;

			pData->curPlacementInfo.iAttackSourceGridNo = (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ALTERNATEMAP") == 0 && pData->curElement == CREATURE_ELEMENT_ALTERNATEMAP)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENT;
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_ALTERNATEMAP_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = CREATURE_ELEMENT_ALTERNATEMAP;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curPlacementInfo.sAltMapX = x;
				pData->curPlacementInfo.sAltMapY = y;
			}
			SGP_THROW_IFFALSE( (SECTOR(x,y) >= 0 && SECTOR(x,y) < 256) , L"Illegal sector number in CreaturePlacements.XML" );
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_ALTERNATEMAP_Z)
		{
			pData->curElement = CREATURE_ELEMENT_ALTERNATEMAP;

			pData->curPlacementInfo.ubAltMapZ = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "WARPTOSECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENT;
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = CREATURE_ELEMENT_WARPTO;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curPlacementInfo.sWarpToX = x;
				pData->curPlacementInfo.sWarpToY = y;
			}
			SGP_THROW_IFFALSE( (SECTOR(x,y) >= 0 && SECTOR(x,y) < 256) , L"Illegal sector number in CreaturePlacements.XML" );
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO_Z)
		{
			pData->curElement = CREATURE_ELEMENT_WARPTO;

			pData->curPlacementInfo.ubWarpToZ = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "GridNo") == 0 && pData->curElement == CREATURE_ELEMENT_WARPTO_GRIDNO)
		{
			pData->curElement = CREATURE_ELEMENT_WARPTO;

			pData->curPlacementInfo.iWarpToGridNo = (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "CREATURESECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_CREATURESECTOR)
		{
			pData->curElement = CREATURE_ELEMENT_PLACEMENT;
		}
		else if(strcmp(name, "QUEENSECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_QUEEN)
		{
			pData->curElement = CREATURE_ELEMENT_CREATURESECTOR;
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_QUEEN_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = CREATURE_ELEMENT_QUEEN;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curPlacementInfo.sQueenX	= x;
				pData->curPlacementInfo.sQueenY	= y;
			}
			SGP_THROW_IFFALSE( (SECTOR(x,y) >= 0 && SECTOR(x,y) < 256) , L"Illegal sector number in CreaturePlacements.XML" );
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_QUEEN_Z)
		{
			pData->curElement = CREATURE_ELEMENT_QUEEN;

			pData->curPlacementInfo.ubQueenZ = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "HABITATSECTOR") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT)
		{
			pData->curElement = CREATURE_ELEMENT_CREATURESECTOR;
		}
		else if(strcmp(name, "SectorGrid") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = CREATURE_ELEMENT_HABITAT;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curPlacementInfo.Habitat[pData->curHabitat].sX = x;
				pData->curPlacementInfo.Habitat[pData->curHabitat].sY = y;
			}			
			SGP_THROW_IFFALSE( (SECTOR(x,y) >= 0 && SECTOR(x,y) < 256) , L"Illegal sector number in CreaturePlacements.XML" );
		}
		else if(strcmp(name, "SectorZ") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT_Z)
		{
			pData->curElement = CREATURE_ELEMENT_HABITAT;

			pData->curPlacementInfo.Habitat[pData->curHabitat].ubZ = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Composition") == 0 && pData->curElement == CREATURE_ELEMENT_HABITAT_COMPOSITION)
		{
			pData->curElement = CREATURE_ELEMENT_HABITAT;

			pData->curPlacementInfo.Habitat[pData->curHabitat].ubComposition = (UINT8) atol(pData->szCharData);
			
			// valid creature habitat if using other than queen habitat
			if ( pData->curPlacementInfo.Habitat[pData->curHabitat].ubComposition )
				pData->curPlacementInfo.Habitat[pData->curHabitat].fValid = TRUE;

			pData->curHabitat++;
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInCreaturePlacements(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	CreatureParseData pData;

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


	XML_SetElementHandler(parser, creatureplacementStartElementHandle, creatureplacementEndElementHandle);
	XML_SetCharacterDataHandler(parser, creatureplacementCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	NUMBER_OF_INFECTIBLE_SITES = 0;
	NUMBER_OF_CREATURE_COMPOSITIONS = 0;
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];
		sprintf(errorBuf, "XML Parser Error in CreaturePlacements.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}