#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "qarray.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "qarray.h"
#endif


struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	QARRAY_VALUES	curQarray;
	QARRAY_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef qarrayParseData;

static void XMLCALL
qarrayStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	qarrayParseData * pData = (qarrayParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "QARRAY") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(QARRAY_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PROFILE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curQarray,0,sizeof(QARRAY_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "QuoteExpHeadShotOnly") == 0 ||
				strcmp(name, "QuoteExpTeamSpecific") == 0 ||
				strcmp(name, "QuoteExpGenderCode") == 0 ||
				strcmp(name, "QuoteExpGotGunOrUsedGun") == 0 ||
				strcmp(name, "QuoteExpPassingDislike") == 0 ||
				strcmp(name, "QuoteExpWitnessDeidrannaDeath") == 0 ||
				strcmp(name, "QuoteExpWitnessQueenBugDeath") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';

	}
	pData->currentDepth++;
	}

	

static void XMLCALL
qarrayCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	qarrayParseData * pData = (qarrayParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
qarrayEndElementHandle(void *userData, const XML_Char *name)
{
	qarrayParseData * pData = (qarrayParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "QARRAY") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "PROFILE") == 0)
		{
			pData->curElement = ELEMENT_LIST;	

			if(pData->curQarray.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curQarray.uiIndex] = pData->curQarray; 
			}
			
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpHeadShotOnly") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpHeadShotOnly	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpTeamSpecific") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpTeamSpecific	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpGenderCode") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpGenderCode	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpGotGunOrUsedGun") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpGotGunOrUsedGun	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpPassingDislike") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpPassingDislike	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpWitnessDeidrannaDeath") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpWitnessDeidrannaDeath	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "QuoteExpWitnessQueenBugDeath") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curQarray.QuoteExpWitnessQueenBugDeath	= (BOOLEAN) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInMercQuotes(QARRAY_VALUES *pQarray, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	qarrayParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading MercQuote.xml" );

	
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


	XML_SetElementHandler(parser, qarrayStartElementHandle, qarrayEndElementHandle);
	XML_SetCharacterDataHandler(parser, qarrayCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pQarray;
	pData.maxArraySize = NUM_PROFILES;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Quotearray.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}



BOOLEAN WriteMercQuotes(QARRAY_VALUES *pQarray, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<QARRAY>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES;cnt++)
		{

			FilePrintf(hFile,"\t<PROFILE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<QuoteExpHeadShotOnly>%d</QuoteExpHeadShotOnly>\r\n", pQarray[cnt].QuoteExpHeadShotOnly);
			FilePrintf(hFile,"\t\t<QuoteExpTeamSpecific>%d</QuoteExpTeamSpecific>\r\n", pQarray[cnt].QuoteExpTeamSpecific);
			FilePrintf(hFile,"\t\t<QuoteExpGenderCode>%d</QuoteExpGenderCode>\r\n", pQarray[cnt].QuoteExpGenderCode);
			FilePrintf(hFile,"\t\t<QuoteExpGotGunOrUsedGun>%d</QuoteExpGotGunOrUsedGun>\r\n", pQarray[cnt].QuoteExpGotGunOrUsedGun);
			FilePrintf(hFile,"\t\t<QuoteExpPassingDislike>%d</QuoteExpPassingDislike>\r\n", pQarray[cnt].QuoteExpPassingDislike);
			FilePrintf(hFile,"\t\t<QuoteExpWitnessQueenBugDeath>%d</QuoteExpWitnessQueenBugDeath>\r\n", pQarray[cnt].QuoteExpWitnessQueenBugDeath);
			FilePrintf(hFile,"\t\t<QuoteExpWitnessDeidrannaDeath>%d</QuoteExpWitnessDeidrannaDeath>\r\n", pQarray[cnt].QuoteExpWitnessDeidrannaDeath);


			FilePrintf(hFile,"\t</PROFILE>\r\n");
		}
		FilePrintf(hFile,"</QARRAY>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
