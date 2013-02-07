#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
//	#include "SectorLoadscreens.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	SECTOR_LOADSCREENS	curSectorLoadscreens;
	SECTOR_LOADSCREENS *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef sectorLoadscreensParseData;

static void XMLCALL
sectorLoadscreensStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	sectorLoadscreensParseData * pData = (sectorLoadscreensParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "LOADSCREENS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(SECTOR_LOADSCREENS)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "SECTOR") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curSectorLoadscreens,0,sizeof(SECTOR_LOADSCREENS));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if (pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szLocation") == 0 ||
				strcmp(name, "RandomAltSector") == 0 ||
				strcmp(name, "szImageFormat") == 0 ||
				strcmp(name, "szDay") == 0 ||
				strcmp(name, "szNight") == 0 ||
				strcmp(name, "szDayAlt") == 0 ||
				strcmp(name, "szNightAlt") == 0))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
sectorLoadscreensCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	sectorLoadscreensParseData * pData = (sectorLoadscreensParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
sectorLoadscreensEndElementHandle(void *userData, const XML_Char *name)
{
	char temp;
	sectorLoadscreensParseData * pData = (sectorLoadscreensParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "LOADSCREENS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "SECTOR") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curSectorLoadscreens.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curSectorLoadscreens.uiIndex] = pData->curSectorLoadscreens; //write the inventory into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSectorLoadscreens.uiIndex	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "szLocation") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_LOCATION_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curSectorLoadscreens.szLocation,pData->szCharData);
			else
			{
				strncpy(pData->curSectorLoadscreens.szLocation,pData->szCharData,MAX_LOCATION_CHARS);
				pData->curSectorLoadscreens.szLocation[MAX_LOCATION_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_LOCATION_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curSectorLoadscreens.szLocation[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szLocation[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szLocation[i],&temp));
			}
		}
		else if(strcmp(name, "RandomAltSector") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSectorLoadscreens.RandomAltSector	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "szImageFormat") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_IMAGE_FORMAT_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curSectorLoadscreens.szImageFormat,pData->szCharData);
			else
			{
				strncpy(pData->curSectorLoadscreens.szImageFormat,pData->szCharData,MAX_IMAGE_FORMAT_CHARS);
				pData->curSectorLoadscreens.szImageFormat[MAX_IMAGE_FORMAT_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_IMAGE_FORMAT_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curSectorLoadscreens.szImageFormat[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szImageFormat[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szImageFormat[i],&temp));
			}
		}
		else if(strcmp(name, "szDay") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_IMAGE_PATH_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curSectorLoadscreens.szDay,pData->szCharData);
			else
			{
				strncpy(pData->curSectorLoadscreens.szDay,pData->szCharData,MAX_IMAGE_PATH_CHARS);
				pData->curSectorLoadscreens.szDay[MAX_IMAGE_PATH_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_IMAGE_PATH_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curSectorLoadscreens.szDay[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szDay[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szDay[i],&temp));
			}
		}
		else if(strcmp(name, "szNight") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_IMAGE_PATH_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curSectorLoadscreens.szNight,pData->szCharData);
			else
			{
				strncpy(pData->curSectorLoadscreens.szNight,pData->szCharData,MAX_IMAGE_PATH_CHARS);
				pData->curSectorLoadscreens.szNight[MAX_IMAGE_PATH_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_IMAGE_PATH_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curSectorLoadscreens.szNight[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szNight[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szNight[i],&temp));
			}
		}
		else if(strcmp(name, "szDayAlt") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_IMAGE_PATH_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curSectorLoadscreens.szDayAlt,pData->szCharData);
			else
			{
				strncpy(pData->curSectorLoadscreens.szDayAlt,pData->szCharData,MAX_IMAGE_PATH_CHARS);
				pData->curSectorLoadscreens.szDayAlt[MAX_IMAGE_PATH_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_IMAGE_PATH_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curSectorLoadscreens.szDayAlt[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szDayAlt[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szDayAlt[i],&temp));
			}
		}
		else if(strcmp(name, "szNightAlt") == 0)
		{
			pData->curElement = ELEMENT;

			if(MAX_IMAGE_PATH_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curSectorLoadscreens.szNightAlt,pData->szCharData);
			else
			{
				strncpy(pData->curSectorLoadscreens.szNightAlt,pData->szCharData,MAX_IMAGE_PATH_CHARS);
				pData->curSectorLoadscreens.szNightAlt[MAX_IMAGE_PATH_CHARS] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_IMAGE_PATH_CHARS);i++)
			{
				temp = pData->szCharData[i];
				pData->curSectorLoadscreens.szNightAlt[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szNightAlt[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szNightAlt[i],&temp));
			}
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInSectorLoadscreensStats(SECTOR_LOADSCREENS *pSectorLoadscreens, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	sectorLoadscreensParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading SectorLoadscreens.xml" );

	// Open inventory file
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


	XML_SetElementHandler(parser, sectorLoadscreensStartElementHandle, sectorLoadscreensEndElementHandle);
	XML_SetCharacterDataHandler(parser, sectorLoadscreensCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pSectorLoadscreens;
	pData.maxArraySize = MAX_SECTOR_LOADSCREENS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in SectorLoadscreens.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteSectorLoadscreensStats(SECTOR_LOADSCREENS *pSectorLoadscreens, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<LOADSCREENS>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<SECTOR>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<szLocation>%d</szLocation>\r\n", pSectorLoadscreens[cnt].szLocation);
			FilePrintf(hFile,"\t\t<RandomAltSector>%d</RandomAltSector>\r\n", pSectorLoadscreens[cnt].RandomAltSector);
			FilePrintf(hFile,"\t\t<szImageFormat>%d</szImageFormat>\r\n", pSectorLoadscreens[cnt].szImageFormat);
			FilePrintf(hFile,"\t\t<szDay>%d</szDay>\r\n", pSectorLoadscreens[cnt].szDay);
			FilePrintf(hFile,"\t\t<szNight>%d</szNight>\r\n", pSectorLoadscreens[cnt].szNight);
			FilePrintf(hFile,"\t\t<szDayAlt>%d</szDayAlt>\r\n", pSectorLoadscreens[cnt].szDayAlt);
			FilePrintf(hFile,"\t\t<szNightAlt>%d</szNightAlt>\r\n", pSectorLoadscreens[cnt].szNightAlt);

			FilePrintf(hFile,"\t</SECTOR>\r\n");
		}
		FilePrintf(hFile,"</LOADSCREENS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
