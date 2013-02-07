#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "weapons.h"
	#include "overhead.h"
	#include "GameSettings.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	ARMOURTYPE		curArmour;
	ARMOURTYPE *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef armourParseData;

static void XMLCALL
armourStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	armourParseData * pData = (armourParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ARMOURLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(ARMOURTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ARMOUR") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curArmour,0,sizeof(ARMOURTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "ubArmourClass") == 0 ||
				strcmp(name, "ubProtection") == 0 ||
				strcmp(name, "ubCoverage") == 0 ||
				strcmp(name, "ubDegradePercent") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
armourCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	armourParseData * pData = (armourParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
armourEndElementHandle(void *userData, const XML_Char *name)
{
	armourParseData * pData = (armourParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ARMOURLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ARMOUR") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curArmour.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curArmour.uiIndex] = pData->curArmour; //write the armour into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmour.uiIndex	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "ubArmourClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmour.ubArmourClass	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubProtection") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmour.ubProtection = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubCoverage") == 0)
		{
			pData->curElement = ELEMENT;
			if ( gGameExternalOptions.fEnableArmorCoverage )
				pData->curArmour.ubCoverage = (UINT8) atol(pData->szCharData);
			else
				pData->curArmour.ubCoverage = 100;
		}
		else if(strcmp(name, "ubDegradePercent") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmour.ubDegradePercent	= (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInArmourStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	armourParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Armours.xml" );

	// Open armour file
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


	XML_SetElementHandler(parser, armourStartElementHandle, armourEndElementHandle);
	XML_SetCharacterDataHandler(parser, armourCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = Armour;
	pData.maxArraySize = MAXITEMS;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Armours.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteArmourStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writearmourstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Armour out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ARMOURLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<ARMOUR>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<ubArmourClass>%d</ubArmourClass>\r\n",								Armour[cnt].ubArmourClass	);
			FilePrintf(hFile,"\t\t<ubProtection>%d</ubProtection>\r\n",								Armour[cnt].ubProtection	);
			FilePrintf(hFile,"\t\t<ubCoverage>%d</ubCoverage>\r\n",								Armour[cnt].ubCoverage	);
			FilePrintf(hFile,"\t\t<ubDegradePercent>%d</ubDegradePercent>\r\n",								Armour[cnt].ubDegradePercent	);

			FilePrintf(hFile,"\t</ARMOUR>\r\n");
		}
		FilePrintf(hFile,"</ARMOURLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
