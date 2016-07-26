#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "XML.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "XML.h"
	#include "expat.h"
	#include "string.h"
	#include "Campaign Types.h"
	#include "FileMan.h"
	#include "MemMan.h"
	#include "Debug Control.h"
	#include "mapscreen.h"
#endif

#define MAX_CHAR_DATA_LENGTH			500

// Four different sector names, used at different times.
extern CHAR16 gzSectorNames[256][4][MAX_SECTOR_NAME_LENGTH];

// Flugente: external sector data
extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// moved to lua
//extern CHAR16 gzSectorUndergroundNames1[256][4][MAX_SECTOR_NAME_LENGTH]; 
//extern CHAR16 gzSectorUndergroundNames2[256][4][MAX_SECTOR_NAME_LENGTH]; 
//extern CHAR16 gzSectorUndergroundNames3[256][4][MAX_SECTOR_NAME_LENGTH]; 

typedef struct
{
	PARSE_STAGE		curElement;
	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT8			sCurSectorX;
	UINT8			sCurSectorY;
	CHAR16			szCurUnexploredName[MAX_SECTOR_NAME_LENGTH];
	CHAR16			szCurDetailedUnexploredName[MAX_SECTOR_NAME_LENGTH];
	CHAR16			szCurExploredName[MAX_SECTOR_NAME_LENGTH];
	CHAR16			szCurDetailedExploredName[MAX_SECTOR_NAME_LENGTH];
	SECTOR_EXT_DATA sectordata;
	UINT8			curSupplyType;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
} SectorNameParseData;

// for compliation reasons, this may not be a part of SectorNameParseData
std::vector<UINT16> prisonroomvector;

BOOLEAN SectorName_TextOnly;

INT8 Sector_Level;


static void XMLCALL
SectorNameStartElementHandle(void *userData, const XML_Char *name, const char **atts)
{
	SectorNameParseData * pData = (SectorNameParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "SECTOR_NAMES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;
			pData->maxReadDepth++; //we are not skipping this element
			// Initiate gzSectorNames Array by setting first character to 0.
			for (UINT16 x = 0; x < 256; x++)
			{
				if (Sector_Level == 0 )
				{
					gzSectorNames[x][0][0]=0;
					gzSectorNames[x][1][0]=0;
					gzSectorNames[x][2][0]=0;
					gzSectorNames[x][3][0]=0;
				}
			}
		}

		else if(strcmp(name, "SECTOR") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "SectorGrid") == 0  ||
				strcmp(name, "szUnexploredName") == 0 ||
				strcmp(name, "szDetailedUnexploredName") == 0 ||
				strcmp(name, "szExploredName") == 0 ||
				strcmp(name, "szDetailedExploredName") == 0 ||
				strcmp(name, "sWaterType") == 0 ||
				strcmp(name, "usNaturalDirt") == 0 ||
				strcmp(name, "usCurfewValue") == 0 ||
				strcmp(name, "sRadioScanModifier") == 0 ||
				strcmp(name, "usPrisonRoomNumber" ) == 0 ||
				strcmp(name, "usCivilianPopulation" ) == 0 ||
				strcmp(name, "rainchance" ) == 0 ||
				strcmp(name, "sandstormchance") == 0 ||
				strcmp(name, "snowchance" ) == 0 ||
				strcmp(name, "snakechance" ) == 0 ||
				strcmp(name, "numsnakes" ) == 0 ||
				strcmp(name, "maxworkers" ) == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}



static void XMLCALL
SectorNameCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	SectorNameParseData * pData = (SectorNameParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
SectorNameEndElementHandle(void *userData, const XML_Char *name)
{
	SectorNameParseData * pData = (SectorNameParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "SECTOR_NAMES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "SECTOR") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			UINT8 ubSectorId = SECTOR(pData->sCurSectorX, pData->sCurSectorY);
			if (ubSectorId >= 0 && ubSectorId < 256)
			{
				if (!SectorName_TextOnly)
				{
					if (Sector_Level == 0 )
					{
						wcscpy(gzSectorNames[ubSectorId][0], pData->szCurUnexploredName);
						wcscpy(gzSectorNames[ubSectorId][1], pData->szCurDetailedUnexploredName);
						wcscpy(gzSectorNames[ubSectorId][2], pData->szCurExploredName);
						wcscpy(gzSectorNames[ubSectorId][3], pData->szCurDetailedExploredName);
					}
					// moved to lua
					//else if (Sector_Level == 1 )
					//{
					//	wcscpy(gzSectorUndergroundNames1[ubSectorId][0], pData->szCurUnexploredName);
					//	wcscpy(gzSectorUndergroundNames1[ubSectorId][1], pData->szCurDetailedUnexploredName);
					//	wcscpy(gzSectorUndergroundNames1[ubSectorId][2], pData->szCurExploredName);
					//	wcscpy(gzSectorUndergroundNames1[ubSectorId][3], pData->szCurDetailedExploredName);					
					//}
					//else if (Sector_Level == 2 )
					//{
					//	wcscpy(gzSectorUndergroundNames2[ubSectorId][0], pData->szCurUnexploredName);
					//	wcscpy(gzSectorUndergroundNames2[ubSectorId][1], pData->szCurDetailedUnexploredName);
					//	wcscpy(gzSectorUndergroundNames2[ubSectorId][2], pData->szCurExploredName);
					//	wcscpy(gzSectorUndergroundNames2[ubSectorId][3], pData->szCurDetailedExploredName);					
					//}
					//else if (Sector_Level == 3 )
					//{
					//	wcscpy(gzSectorUndergroundNames3[ubSectorId][0], pData->szCurUnexploredName);
					//	wcscpy(gzSectorUndergroundNames3[ubSectorId][1], pData->szCurDetailedUnexploredName);
					//	wcscpy(gzSectorUndergroundNames3[ubSectorId][2], pData->szCurExploredName);
					//	wcscpy(gzSectorUndergroundNames3[ubSectorId][3], pData->szCurDetailedExploredName);
					//}

					SectorExternalData[ubSectorId][0].usWaterType = pData->sectordata.usWaterType;
					SectorExternalData[ubSectorId][1].usWaterType = pData->sectordata.usWaterType;
					SectorExternalData[ubSectorId][2].usWaterType = pData->sectordata.usWaterType;
					SectorExternalData[ubSectorId][3].usWaterType = pData->sectordata.usWaterType;

					SectorExternalData[ubSectorId][0].usNaturalDirt = pData->sectordata.usNaturalDirt;
					SectorExternalData[ubSectorId][1].usNaturalDirt = pData->sectordata.usNaturalDirt;
					SectorExternalData[ubSectorId][2].usNaturalDirt = pData->sectordata.usNaturalDirt;
					SectorExternalData[ubSectorId][3].usNaturalDirt = pData->sectordata.usNaturalDirt;

					SectorExternalData[ubSectorId][0].usCurfewValue = pData->sectordata.usCurfewValue;
					SectorExternalData[ubSectorId][1].usCurfewValue = pData->sectordata.usCurfewValue;
					SectorExternalData[ubSectorId][2].usCurfewValue = pData->sectordata.usCurfewValue;
					SectorExternalData[ubSectorId][3].usCurfewValue = pData->sectordata.usCurfewValue;

					INT8 radioscanmod = max( -3, pData->sectordata.sRadioScanModifier );
					radioscanmod = min( 3, pData->sectordata.sRadioScanModifier );
					SectorExternalData[ubSectorId][0].sRadioScanModifier = radioscanmod;
					SectorExternalData[ubSectorId][1].sRadioScanModifier = radioscanmod;
					SectorExternalData[ubSectorId][2].sRadioScanModifier = radioscanmod;
					SectorExternalData[ubSectorId][3].sRadioScanModifier = radioscanmod;

					SectorExternalData[ubSectorId][0].usCivilianPopulation = pData->sectordata.usCivilianPopulation;
					SectorExternalData[ubSectorId][1].usCivilianPopulation = pData->sectordata.usCivilianPopulation;
					SectorExternalData[ubSectorId][2].usCivilianPopulation = pData->sectordata.usCivilianPopulation;
					SectorExternalData[ubSectorId][3].usCivilianPopulation = pData->sectordata.usCivilianPopulation;

					SectorExternalData[ubSectorId][0].rainchance = pData->sectordata.rainchance;
					SectorExternalData[ubSectorId][1].rainchance = 0;
					SectorExternalData[ubSectorId][2].rainchance = 0;
					SectorExternalData[ubSectorId][3].rainchance = 0;

					SectorExternalData[ubSectorId][0].sandstormchance = pData->sectordata.sandstormchance;
					SectorExternalData[ubSectorId][1].sandstormchance = 0;
					SectorExternalData[ubSectorId][2].sandstormchance = 0;
					SectorExternalData[ubSectorId][3].sandstormchance = 0;

					SectorExternalData[ubSectorId][0].snowchance = pData->sectordata.snowchance;
					SectorExternalData[ubSectorId][1].snowchance = 0;
					SectorExternalData[ubSectorId][2].snowchance = 0;
					SectorExternalData[ubSectorId][3].snowchance = 0;

					SectorExternalData[ubSectorId][0].snakechance = pData->sectordata.snakechance;
					SectorExternalData[ubSectorId][1].snakechance = 0;
					SectorExternalData[ubSectorId][2].snakechance = 0;
					SectorExternalData[ubSectorId][3].snakechance = 0;

					SectorExternalData[ubSectorId][0].numsnakes = pData->sectordata.numsnakes;
					SectorExternalData[ubSectorId][1].numsnakes = 0;
					SectorExternalData[ubSectorId][2].numsnakes = 0;
					SectorExternalData[ubSectorId][3].numsnakes = 0;
					
					SectorExternalData[ubSectorId][0].maxworkers = pData->sectordata.maxworkers;
					SectorExternalData[ubSectorId][1].maxworkers = 0;
					SectorExternalData[ubSectorId][2].maxworkers = 0;
					SectorExternalData[ubSectorId][3].maxworkers = 0;
																				
					if ( !prisonroomvector.empty( ) )
					{
						std::vector<UINT16>::iterator itend = prisonroomvector.end( );
						std::vector<UINT16>::iterator it = prisonroomvector.begin( );
						for ( ; it != itend; ++it )
						{
							SectorExternalData[ubSectorId][0].prisonroomvector.push_back( (*it) );
						}
					}

					prisonroomvector.clear( );

					// clean up values afterwards
					pData->sectordata.usWaterType = 0;
					pData->sectordata.usNaturalDirt = 100;
					pData->sectordata.usCurfewValue = 0;
					pData->sectordata.sRadioScanModifier = 0;
					pData->sectordata.usCivilianPopulation = 0;
					pData->sectordata.rainchance = 0;
					pData->sectordata.sandstormchance = 0;
					pData->sectordata.snowchance = 0;
					pData->sectordata.snakechance = 0;
					pData->sectordata.numsnakes = 0;
					pData->sectordata.maxworkers = 0;
				}
				else
				{
					if (Sector_Level == 0 )
					{
						wcscpy(gzSectorNames[ubSectorId][0], pData->szCurUnexploredName);
						wcscpy(gzSectorNames[ubSectorId][1], pData->szCurDetailedUnexploredName);
						wcscpy(gzSectorNames[ubSectorId][2], pData->szCurExploredName);
						wcscpy(gzSectorNames[ubSectorId][3], pData->szCurDetailedExploredName);												
					}
				}
			}	
		}

		else if(strcmp(name, "SectorGrid") == 0 )
		{
			UINT8	x, y;
			pData->curElement = ELEMENT;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->sCurSectorX	= x;
				pData->sCurSectorY  = y;
			}
		}
		else if(strcmp(name, "szUnexploredName") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->szCurUnexploredName, sizeof(pData->szCurUnexploredName)/sizeof(pData->szCurUnexploredName[0]) );
			pData->szCurUnexploredName[sizeof(pData->szCurUnexploredName)/sizeof(pData->szCurUnexploredName[0]) - 1] = '\0';
		}

		else if(strcmp(name, "szDetailedUnexploredName") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->szCurDetailedUnexploredName, sizeof(pData->szCurDetailedUnexploredName)/sizeof(pData->szCurDetailedUnexploredName[0]) );
			pData->szCurDetailedUnexploredName[sizeof(pData->szCurDetailedUnexploredName)/sizeof(pData->szCurDetailedUnexploredName[0]) - 1] = '\0';
		}

		else if(strcmp(name, "szExploredName") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->szCurExploredName, sizeof(pData->szCurExploredName)/sizeof(pData->szCurExploredName[0]) );
			pData->szCurExploredName[sizeof(pData->szCurExploredName)/sizeof(pData->szCurExploredName[0]) - 1] = '\0';
		}

		else if(strcmp(name, "szDetailedExploredName") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->szCurDetailedExploredName, sizeof(pData->szCurDetailedExploredName)/sizeof(pData->szCurDetailedExploredName[0]) );
			pData->szCurDetailedExploredName[sizeof(pData->szCurDetailedExploredName)/sizeof(pData->szCurDetailedExploredName[0]) - 1] = '\0';
		}

		else if(strcmp(name, "sWaterType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->sectordata.usWaterType = (UINT8)atol( pData->szCharData );
		}
		else if(strcmp(name, "usNaturalDirt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->sectordata.usNaturalDirt = (UINT16)atoi( pData->szCharData );
		}
		else if(strcmp(name, "usCurfewValue") == 0)
		{
			pData->curElement = ELEMENT;
			pData->sectordata.usCurfewValue = (UINT8)atoi( pData->szCharData );
		}
		else if(strcmp(name, "sRadioScanModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->sectordata.sRadioScanModifier = (INT8)atoi( pData->szCharData );
		}
		else if ( strcmp( name, "usPrisonRoomNumber" ) == 0 )
		{
			pData->curElement = ELEMENT;

			UINT16 room = (UINT16)atoi( pData->szCharData );

			if ( room )
				prisonroomvector.push_back( room );
		}		
		else if(strcmp(name, "usCivilianPopulation") == 0)
		{
			pData->curElement = ELEMENT;
			pData->sectordata.usCivilianPopulation = (UINT16)atoi( pData->szCharData );
		}
		else if ( strcmp( name, "rainchance" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->sectordata.rainchance = min( 100, (UINT8)atoi( pData->szCharData ) );
		}
		else if ( strcmp( name, "sandstormchance" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->sectordata.sandstormchance = min( 100, (UINT8)atoi( pData->szCharData ) );
		}
		else if ( strcmp( name, "snowchance" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->sectordata.snowchance = min( 100, (UINT8)atoi( pData->szCharData ) );
		}
		else if ( strcmp( name, "snakechance" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->sectordata.snakechance = min( 100, (UINT8)atoi( pData->szCharData ) );
		}
		else if ( strcmp( name, "numsnakes" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->sectordata.numsnakes = min( 20, (UINT8)atoi( pData->szCharData ) );
		}
		else if ( strcmp( name, "maxworkers" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->sectordata.maxworkers = (UINT8)atoi( pData->szCharData );
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInSectorNames(STR fileName, BOOLEAN localizedVersion, INT8 Level )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	SectorNameParseData pData;
	
	Sector_Level = Level;

	SectorName_TextOnly = localizedVersion;
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


	XML_SetElementHandler(parser, SectorNameStartElementHandle, SectorNameEndElementHandle);
	XML_SetCharacterDataHandler(parser, SectorNameCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];
		sprintf(errorBuf, "XML Parser Error in SectorNames.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}
