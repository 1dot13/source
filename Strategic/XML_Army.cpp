#include "builddefines.h"

	#include <stdio.h>
	#include "XML.h"
	#include "Strategic AI.h"
	#include "expat.h"
	#include "Campaign Types.h"
	#include "Debug Control.h"

#define MAX_CHAR_DATA_LENGTH			500

extern GARRISON_GROUP	gOrigGarrisonGroup[ MAX_GARRISON_GROUPS ];
extern PATROL_GROUP		gOrigPatrolGroup[ MAX_PATROL_GROUPS ];
extern ARMY_COMPOSITION	gOrigArmyComp[ MAX_ARMY_COMPOSITIONS ];
extern INT32	iOrigGarrisonArraySize;
extern INT32	iOrigPatrolArraySize;


// Garrison externalization
typedef enum
{
	GARRISON_ELEMENT_NONE = 0,
	GARRISON_ELEMENT_GARRISON_INFO,
	GARRISON_ELEMENT_GARRISON,
	GARRISON_ELEMENT_SECTOR,
	GARRISON_ELEMENT_COMPOSITION
} GARRISON_PARSE_STAGE;

typedef struct
{
	UINT8	ubSectorID;
	UINT8	ubComposition;
	BOOLEAN fValidSector;
} garrisonInfo;

typedef struct
{
	GARRISON_PARSE_STAGE	curElement;

	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	garrisonInfo			curGarrisonInfo;
	UINT32					uiGarrisonCount;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} garrisonParseData;

static void XMLCALL
garrisonStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	garrisonParseData * pData = (garrisonParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "GARRISON_INFO") == 0 && pData->curElement == GARRISON_ELEMENT_NONE)
		{
			pData->curElement = GARRISON_ELEMENT_GARRISON_INFO;
			memset( gOrigGarrisonGroup,	0, sizeof(gOrigGarrisonGroup) );
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "GARRISON") == 0 && pData->curElement == GARRISON_ELEMENT_GARRISON_INFO)
		{
			pData->curElement = GARRISON_ELEMENT_GARRISON;
			memset( &pData->curGarrisonInfo, 0, sizeof(garrisonInfo) );
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Sector") == 0 && pData->curElement == GARRISON_ELEMENT_GARRISON)
		{
			pData->curElement = GARRISON_ELEMENT_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Composition") == 0 && pData->curElement == GARRISON_ELEMENT_GARRISON)
		{
			pData->curElement = GARRISON_ELEMENT_COMPOSITION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
garrisonCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	garrisonParseData * pData = (garrisonParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
garrisonEndElementHandle(void *userData, const XML_Char *name)
{
	garrisonParseData * pData = (garrisonParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "GARRISON_INFO") == 0 && pData->curElement == GARRISON_ELEMENT_GARRISON_INFO)
		{
			pData->curElement = GARRISON_ELEMENT_NONE;
			iOrigGarrisonArraySize = pData->uiGarrisonCount;
		}
		else if(strcmp(name, "GARRISON") == 0 && pData->curElement == GARRISON_ELEMENT_GARRISON)
		{
			pData->curElement = GARRISON_ELEMENT_GARRISON_INFO;

			if ( pData->curGarrisonInfo.fValidSector && pData->uiGarrisonCount < MAX_GARRISON_GROUPS )
			{
				gOrigGarrisonGroup[ pData->uiGarrisonCount ].ubSectorID	= pData->curGarrisonInfo.ubSectorID;
				gOrigGarrisonGroup[ pData->uiGarrisonCount ].ubComposition = pData->curGarrisonInfo.ubComposition;
				pData->uiGarrisonCount++;
			}
		}
		else if(strcmp(name, "Sector") == 0 && pData->curElement == GARRISON_ELEMENT_SECTOR)
		{
			UINT8	x, y;
			pData->curElement = GARRISON_ELEMENT_GARRISON;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->curGarrisonInfo.fValidSector = TRUE;
				pData->curGarrisonInfo.ubSectorID	= SECTOR(x,y);
			}
			else
			{
				pData->curGarrisonInfo.fValidSector = FALSE;
			}
		}
		else if(strcmp(name, "Composition") == 0 && pData->curElement == GARRISON_ELEMENT_COMPOSITION)
		{
			pData->curElement = GARRISON_ELEMENT_GARRISON;
			pData->curGarrisonInfo.ubComposition = (UINT8) atol(pData->szCharData);
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInGarrisonInfo(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	garrisonParseData pData;


	// Open weapons file
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


	XML_SetElementHandler(parser, garrisonStartElementHandle, garrisonEndElementHandle);
	XML_SetCharacterDataHandler(parser, garrisonCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);
	iOrigGarrisonArraySize = 0;

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in GarrisonGroups.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}


BOOLEAN WriteInGarrisonInfo(STR fileName)
{
	HWFILE		hFile;

	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		INT8 cnt;


		FilePrintf(hFile,"<GARRISON_INFO>\r\n");
		for(cnt = 0; cnt < 57; cnt++)
		{
			FilePrintf(hFile,"\t<GARRISON>\r\n");

			FilePrintf(hFile,"\t\t<Sector>%c%d</Sector>\r\n",
				(gOrigGarrisonGroup[cnt].ubSectorID / 16 + 0x41),
				(gOrigGarrisonGroup[cnt].ubSectorID % 16 + 1));

			FilePrintf(hFile,"\t\t<Composition>%d</Composition>\r\n",
				gOrigGarrisonGroup[cnt].ubComposition);

			FilePrintf(hFile,"\t</GARRISON>\r\n");
		}
		FilePrintf(hFile,"</GARRISON_INFO>\r\n");
	}
	FileClose( hFile );

	return TRUE;
}


// Patrol externalization
typedef enum
{
	PATROL_ELEMENT_NONE = 0,
	PATROL_ELEMENT_PATROL_INFO,
	PATROL_ELEMENT_PATROL,
	PATROL_ELEMENT_SIZE,
	PATROL_ELEMENT_PRIORITY,
	PATROL_ELEMENT_WAYPOINTS,
	PATROL_ELEMENT_SECTOR1,
	PATROL_ELEMENT_SECTOR2,
	PATROL_ELEMENT_SECTOR3,
	PATROL_ELEMENT_SECTOR4
} PATROL_PARSE_STAGE;

typedef struct
{
	BOOLEAN fErrorState;
	INT8	bSize;
	INT8	bPriority;
	UINT8	ubSectorID[4];
} patrolInfo;

typedef struct
{
	PATROL_PARSE_STAGE		curElement;

	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	patrolInfo				curPatrolInfo;
	UINT32					uiPatrolCount;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} patrolParseData;

static void XMLCALL
patrolStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	patrolParseData * pData = (patrolParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "PATROL_INFO") == 0 && pData->curElement == PATROL_ELEMENT_NONE)
		{
			pData->curElement = PATROL_ELEMENT_PATROL_INFO;
			memset( gOrigPatrolGroup,	0, sizeof(gOrigPatrolGroup) );
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PATROL") == 0 && pData->curElement == PATROL_ELEMENT_PATROL_INFO)
		{
			pData->curElement = PATROL_ELEMENT_PATROL;
			memset( &pData->curPatrolInfo, 0, sizeof(patrolInfo) );
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Size") == 0 && pData->curElement == PATROL_ELEMENT_PATROL)
		{
			pData->curElement = PATROL_ELEMENT_SIZE;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Priority") == 0 && pData->curElement == PATROL_ELEMENT_PATROL)
		{
			pData->curElement = PATROL_ELEMENT_PRIORITY;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Waypoints") == 0 && pData->curElement == PATROL_ELEMENT_PATROL)
		{
			pData->curElement = PATROL_ELEMENT_WAYPOINTS;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Sector1") == 0 && pData->curElement == PATROL_ELEMENT_WAYPOINTS)
		{
			pData->curElement = PATROL_ELEMENT_SECTOR1;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Sector2") == 0 && pData->curElement == PATROL_ELEMENT_WAYPOINTS)
		{
			pData->curElement = PATROL_ELEMENT_SECTOR2;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Sector3") == 0 && pData->curElement == PATROL_ELEMENT_WAYPOINTS)
		{
			pData->curElement = PATROL_ELEMENT_SECTOR3;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Sector4") == 0 && pData->curElement == PATROL_ELEMENT_WAYPOINTS)
		{
			pData->curElement = PATROL_ELEMENT_SECTOR4;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
patrolCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	patrolParseData * pData = (patrolParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
patrolEndElementHandle(void *userData, const XML_Char *name)
{
	patrolParseData * pData = (patrolParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "PATROL_INFO") == 0 && pData->curElement == PATROL_ELEMENT_PATROL_INFO)
		{
			pData->curElement = PATROL_ELEMENT_NONE;
			iOrigPatrolArraySize = pData->uiPatrolCount;
		}
		else if(strcmp(name, "PATROL") == 0 && pData->curElement == PATROL_ELEMENT_PATROL)
		{
			pData->curElement = PATROL_ELEMENT_PATROL_INFO;

			if ( !pData->curPatrolInfo.fErrorState && pData->uiPatrolCount < MAX_PATROL_GROUPS )
			{
				gOrigPatrolGroup[ pData->uiPatrolCount ].bSize		 = pData->curPatrolInfo.bSize;
				gOrigPatrolGroup[ pData->uiPatrolCount ].bPriority	 = pData->curPatrolInfo.bPriority;
				gOrigPatrolGroup[ pData->uiPatrolCount ].ubSectorID[0] = pData->curPatrolInfo.ubSectorID[0];
				gOrigPatrolGroup[ pData->uiPatrolCount ].ubSectorID[1] = pData->curPatrolInfo.ubSectorID[1];
				gOrigPatrolGroup[ pData->uiPatrolCount ].ubSectorID[2] = pData->curPatrolInfo.ubSectorID[2];
				gOrigPatrolGroup[ pData->uiPatrolCount ].ubSectorID[3] = pData->curPatrolInfo.ubSectorID[3];
				pData->uiPatrolCount++;
			}
		}
		else if(strcmp(name, "Size") == 0 && pData->curElement == PATROL_ELEMENT_SIZE)
		{
			pData->curElement = PATROL_ELEMENT_PATROL;
			pData->curPatrolInfo.bSize = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Priority") == 0 && pData->curElement == PATROL_ELEMENT_PRIORITY)
		{
			pData->curElement = PATROL_ELEMENT_PATROL;
			pData->curPatrolInfo.bPriority = (INT8) atol(pData->szCharData);

			if ( pData->curPatrolInfo.bPriority < 0 )
				pData->curPatrolInfo.bPriority = 0;
			else if ( pData->curPatrolInfo.bPriority > 100 )
				pData->curPatrolInfo.bPriority = 100;
		}
		else if(strcmp(name, "Waypoints") == 0 && pData->curElement == PATROL_ELEMENT_WAYPOINTS)
		{
			pData->curElement = PATROL_ELEMENT_PATROL;
		}
		else if(strcmp(name, "Sector1") == 0 && pData->curElement == PATROL_ELEMENT_SECTOR1)
		{
			pData->curElement = PATROL_ELEMENT_WAYPOINTS;

			if ( strcmp(pData->szCharData, "0") == 0 )
			{
				pData->curPatrolInfo.ubSectorID[0] = 0;
			}
			else
			{
				UINT8	x, y;
				y = (UINT8)pData->szCharData[0] & 0x1F;
				x = (UINT8)atol(&pData->szCharData[1]);
				if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
				{
					pData->curPatrolInfo.ubSectorID[0] = SECTOR(x,y);
				}
				else
				{
					pData->curPatrolInfo.fErrorState++;
				}
			}
		}
		else if(strcmp(name, "Sector2") == 0 && pData->curElement == PATROL_ELEMENT_SECTOR2)
		{
			pData->curElement = PATROL_ELEMENT_WAYPOINTS;

			if ( strcmp(pData->szCharData, "0") == 0 )
			{
				pData->curPatrolInfo.ubSectorID[1] = 0;
			}
			else
			{
				UINT8	x, y;
				y = (UINT8)pData->szCharData[0] & 0x1F;
				x = (UINT8)atol(&pData->szCharData[1]);
				if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
				{
					pData->curPatrolInfo.ubSectorID[1] = SECTOR(x,y);
				}
				else
				{
					pData->curPatrolInfo.fErrorState++;
				}
			}
		}
		else if(strcmp(name, "Sector3") == 0 && pData->curElement == PATROL_ELEMENT_SECTOR3)
		{
			pData->curElement = PATROL_ELEMENT_WAYPOINTS;

			if ( strcmp(pData->szCharData, "0") == 0 )
			{
				pData->curPatrolInfo.ubSectorID[2] = 0;
			}
			else
			{
				UINT8	x, y;
				y = (UINT8)pData->szCharData[0] & 0x1F;
				x = (UINT8)atol(&pData->szCharData[1]);
				if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
				{
					pData->curPatrolInfo.ubSectorID[2] = SECTOR(x,y);
				}
				else
				{
					pData->curPatrolInfo.fErrorState++;
				}
			}
		}
		else if(strcmp(name, "Sector4") == 0 && pData->curElement == PATROL_ELEMENT_SECTOR4)
		{
			pData->curElement = PATROL_ELEMENT_WAYPOINTS;

			if ( strcmp(pData->szCharData, "0") == 0 )
			{
				pData->curPatrolInfo.ubSectorID[3] = 0;
			}
			else
			{
				UINT8	x, y;
				y = (UINT8)pData->szCharData[0] & 0x1F;
				x = (UINT8)atol(&pData->szCharData[1]);
				if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
				{
					pData->curPatrolInfo.ubSectorID[3] = SECTOR(x,y);
				}
				else
				{
					pData->curPatrolInfo.fErrorState++;
				}
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInPatrolInfo(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	patrolParseData pData;


	// Open weapons file
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


	XML_SetElementHandler(parser, patrolStartElementHandle, patrolEndElementHandle);
	XML_SetCharacterDataHandler(parser, patrolCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);
	iOrigPatrolArraySize = 0;

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in PatrolGroups.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}


BOOLEAN WriteInPatrolInfo(STR fileName)
{
	HWFILE		hFile;

	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		INT8 cnt, i;


		FilePrintf(hFile,"<PATROL_INFO>\r\n");
		for(cnt = 0; cnt < 29; cnt++)
		{
			FilePrintf(hFile,"\t<PATROL>\r\n");

			FilePrintf(hFile,"\t\t<Size>%d</Size>\r\n", gOrigPatrolGroup[cnt].bSize);

			FilePrintf(hFile,"\t\t<Priority>%d</Priority>\r\n",	gOrigPatrolGroup[cnt].bPriority);

			FilePrintf(hFile,"\t\t<Waypoints>\r\n");

			for (i=0; i<4; i++)
			{
				if ( gOrigPatrolGroup[cnt].ubSectorID[i] )
				{
					FilePrintf(hFile,"\t\t\t<Sector%d>%c%d</Sector%d>\r\n",
						i+1,
						(gOrigPatrolGroup[cnt].ubSectorID[i] / 16 + 0x41),
						(gOrigPatrolGroup[cnt].ubSectorID[i] % 16 + 1),
						i+1);
				}
				else
				{
					FilePrintf(hFile,"\t\t\t<Sector%d>0</Sector%d>\r\n", i+1, i+1);
				}
			}

			FilePrintf(hFile,"\t\t</Waypoints>\r\n");
			FilePrintf(hFile,"\t</PATROL>\r\n");
		}
		FilePrintf(hFile,"</PATROL_INFO>\r\n");
	}
	FileClose( hFile );

	return TRUE;
}




// Army composition externalization
typedef enum
{
	COMPOSITION_ELEMENT_NONE = 0,
	COMPOSITION_ELEMENT_COMPOSITION_INFO,
	COMPOSITION_ELEMENT_COMPOSITION,
	COMPOSITION_ELEMENT_INDEX,
	COMPOSITION_ELEMENT_PRIORITY,
	COMPOSITION_ELEMENT_ELITE_PERCENTAGE,
	COMPOSITION_ELEMENT_TROOP_PERCENTAGE,
	COMPOSITION_ELEMENT_ADMIN_PERCENTAGE,
	COMPOSITION_ELEMENT_DESIRED_POPULATION,
	COMPOSITION_ELEMENT_START_POPULATION
} COMPOSITION_PARSE_STAGE;

typedef struct
{
	INT32 iIndex;
	INT8	bPriority;
	INT8	bElitePercentage;
	INT8	bTroopPercentage;
	INT8	bAdminPercentage;
	INT8	bDesiredPopulation;
	INT8	bStartPopulation;
} compositionInfo;

typedef struct
{
	COMPOSITION_PARSE_STAGE	curElement;

	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	compositionInfo			curCompositionInfo;
	INT32					iHighestIndex;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} compositionParseData;

static void XMLCALL
compositionStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	compositionParseData * pData = (compositionParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "COMPOSITION_INFO") == 0 && pData->curElement == COMPOSITION_ELEMENT_NONE)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION_INFO;
			memset( gOrigArmyComp,	0, sizeof(gOrigArmyComp) );
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "COMPOSITION") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION_INFO)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			memset( &pData->curCompositionInfo, 0, sizeof(compositionInfo) );
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Index") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_INDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Priority") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_PRIORITY;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ElitePercentage") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_ELITE_PERCENTAGE;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "TroopPercentage") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_TROOP_PERCENTAGE;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "AdminPercentage") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_ADMIN_PERCENTAGE;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DesiredPopulation") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_DESIRED_POPULATION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "StartPopulation") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_START_POPULATION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
compositionCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	compositionParseData * pData = (compositionParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
compositionEndElementHandle(void *userData, const XML_Char *name)
{
	compositionParseData * pData = (compositionParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "COMPOSITION_INFO") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION_INFO)
		{
			pData->curElement = COMPOSITION_ELEMENT_NONE;
			NUM_ARMY_COMPOSITIONS = (UINT8)(pData->iHighestIndex + 1);
		}
		else if(strcmp(name, "COMPOSITION") == 0 && pData->curElement == COMPOSITION_ELEMENT_COMPOSITION)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION_INFO;

			if ( pData->curCompositionInfo.iIndex < MAX_ARMY_COMPOSITIONS )
			{
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].iReadability		= pData->curCompositionInfo.iIndex;
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].bPriority			= pData->curCompositionInfo.bPriority;
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].bElitePercentage	= pData->curCompositionInfo.bElitePercentage;
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].bTroopPercentage	= pData->curCompositionInfo.bTroopPercentage;
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].bAdminPercentage	= pData->curCompositionInfo.bAdminPercentage;
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].bDesiredPopulation = pData->curCompositionInfo.bDesiredPopulation;
				gOrigArmyComp[ pData->curCompositionInfo.iIndex ].bStartPopulation	= pData->curCompositionInfo.bStartPopulation;
			}
		}
		else if(strcmp(name, "Index") == 0 && pData->curElement == COMPOSITION_ELEMENT_INDEX)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.iIndex = (INT32) atol(pData->szCharData);
			if ( pData->curCompositionInfo.iIndex < MAX_ARMY_COMPOSITIONS )
			{
				if ( pData->curCompositionInfo.iIndex > pData->iHighestIndex )
				{
					pData->iHighestIndex = pData->curCompositionInfo.iIndex;
				}
			}
		}
		else if(strcmp(name, "Priority") == 0 && pData->curElement == COMPOSITION_ELEMENT_PRIORITY)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.bPriority = (INT8) atol(pData->szCharData);
			if ( pData->curCompositionInfo.bPriority < 0 )
				pData->curCompositionInfo.bPriority = 0;
			else if ( pData->curCompositionInfo.bPriority > 100 )
				pData->curCompositionInfo.bPriority = 100;
		}
		else if(strcmp(name, "ElitePercentage") == 0 && pData->curElement == COMPOSITION_ELEMENT_ELITE_PERCENTAGE)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.bElitePercentage = (INT8) atol(pData->szCharData);
			if ( pData->curCompositionInfo.bElitePercentage < 0 )
				pData->curCompositionInfo.bElitePercentage = 0;
			else if ( pData->curCompositionInfo.bElitePercentage > 100 )
				pData->curCompositionInfo.bElitePercentage = 100;
		}
		else if(strcmp(name, "TroopPercentage") == 0 && pData->curElement == COMPOSITION_ELEMENT_TROOP_PERCENTAGE)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.bTroopPercentage = (INT8) atol(pData->szCharData);
			if ( pData->curCompositionInfo.bTroopPercentage < 0 )
				pData->curCompositionInfo.bTroopPercentage = 0;
			else if ( pData->curCompositionInfo.bTroopPercentage > 100 )
				pData->curCompositionInfo.bTroopPercentage = 100;
		}
		else if(strcmp(name, "AdminPercentage") == 0 && pData->curElement == COMPOSITION_ELEMENT_ADMIN_PERCENTAGE)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.bAdminPercentage = (INT8) atol(pData->szCharData);
			if ( pData->curCompositionInfo.bAdminPercentage < 0 )
				pData->curCompositionInfo.bAdminPercentage = 0;
			else if ( pData->curCompositionInfo.bAdminPercentage > 100 )
				pData->curCompositionInfo.bAdminPercentage = 100;
		}
		else if(strcmp(name, "DesiredPopulation") == 0 && pData->curElement == COMPOSITION_ELEMENT_DESIRED_POPULATION)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.bDesiredPopulation = (INT8) atol(pData->szCharData);
			if ( pData->curCompositionInfo.bDesiredPopulation < 0 )
				pData->curCompositionInfo.bDesiredPopulation = 1;
		}
		else if(strcmp(name, "StartPopulation") == 0 && pData->curElement == COMPOSITION_ELEMENT_START_POPULATION)
		{
			pData->curElement = COMPOSITION_ELEMENT_COMPOSITION;
			pData->curCompositionInfo.bStartPopulation = (INT8) atol(pData->szCharData);
			if ( pData->curCompositionInfo.bStartPopulation < 0 )
				pData->curCompositionInfo.bStartPopulation = 1;
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInArmyCompositionInfo(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	compositionParseData pData;

	// Open weapons file
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


	XML_SetElementHandler(parser, compositionStartElementHandle, compositionEndElementHandle);
	XML_SetCharacterDataHandler(parser, compositionCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);
	NUM_ARMY_COMPOSITIONS = 0;

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in ArmyComposition.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}


BOOLEAN WriteInArmyCompositionInfo(STR fileName)
{
	HWFILE		hFile;

	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		INT8 cnt;


		FilePrintf(hFile,"<COMPOSITION_INFO>\r\n");
		for(cnt = 0; cnt < NUM_ARMY_COMPOSITIONS; cnt++)
		{
			FilePrintf(hFile,"\t<COMPOSITION>\r\n");

			FilePrintf(hFile,"\t\t<Index>%d</Index>\r\n", gOrigArmyComp[cnt].iReadability );

			FilePrintf(hFile,"\t\t<Priority>%d</Priority>\r\n", gOrigArmyComp[cnt].bPriority);

			FilePrintf(hFile,"\t\t<ElitePercentage>%d</ElitePercentage>\r\n", gOrigArmyComp[cnt].bElitePercentage);

			FilePrintf(hFile,"\t\t<TroopPercentage>%d</TroopPercentage>\r\n", gOrigArmyComp[cnt].bTroopPercentage);

			FilePrintf(hFile,"\t\t<AdminPercentage>%d</AdminPercentage>\r\n", gOrigArmyComp[cnt].bAdminPercentage);

			FilePrintf(hFile,"\t\t<DesiredPopulation>%d</DesiredPopulation>\r\n", gOrigArmyComp[cnt].bDesiredPopulation);

			FilePrintf(hFile,"\t\t<StartPopulation>%d</StartPopulation>\r\n", gOrigArmyComp[cnt].bStartPopulation);

			FilePrintf(hFile,"\t</COMPOSITION>\r\n");
		}
		FilePrintf(hFile,"</COMPOSITION_INFO>\r\n");
	}
	FileClose( hFile );

	return TRUE;
}



