#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "Drugs And Alcohol.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	DRUGTYPE		curDrugs;
	DRUGTYPE *		curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef drugsParseData;

static void XMLCALL
drugsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	drugsParseData * pData = (drugsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "DRUGSLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(DRUGTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DRUG") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curDrugs,0,sizeof(DRUGTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "ubType") == 0 ||
				strcmp(name, "name") == 0 ||
				strcmp(name, "ubDrugTravelRate") == 0 ||
				strcmp(name, "ubDrugWearoffRate") == 0 ||
				strcmp(name, "ubDrugEffect") == 0 ||
				strcmp(name, "ubDrugSideEffect") == 0 ||
				strcmp(name, "ubDrugSideEffectRate") == 0 || 
				strcmp(name, "ubMoralBacklash" ) == 0 ||
				strcmp(name, "ubMoralBacklash" ) == 0 ||
				strcmp(name, "ubDiseaseCure" ) == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
drugsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	drugsParseData * pData = (drugsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
drugsEndElementHandle(void *userData, const XML_Char *name)
{
	drugsParseData * pData = (drugsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "DRUGSLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DRUG") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			// we do NOT want to read the first entry -> move stuff by 1
			if(pData->curDrugs.ubType < pData->maxArraySize + 1 && pData->curDrugs.ubType > 0)	// do not write the first item into our array
			{
				pData->curArray[pData->curDrugs.ubType - 1] = pData->curDrugs; //write the drugs into the table
			}
		}
		else if(strcmp(name, "ubType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubType	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "name") == 0)
		{
			pData->curElement = ELEMENT;
			// not needed, but its there for informational purposes
		}
		else if(strcmp(name, "ubDrugTravelRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubDrugTravelRate	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDrugWearoffRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubDrugWearoffRate = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDrugEffect") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubDrugEffect	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDrugSideEffect") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubDrugSideEffect	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDrugSideEffectRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubDrugSideEffectRate	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMoralBacklash") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubMoralBacklash	= (UINT8) atol(pData->szCharData);
		}
		else if ( strcmp( name, "ubDiseaseCure" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.ubDiseaseCure = (UINT8)atol( pData->szCharData );
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInDrugsStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	drugsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Drugs.xml" );

	// Open drugs file
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


	XML_SetElementHandler(parser, drugsStartElementHandle, drugsEndElementHandle);
	XML_SetCharacterDataHandler(parser, drugsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = Drug;
	pData.maxArraySize = DRUG_TYPE_MAX;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Drugs.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteDrugsStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writedrugsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Drugs out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<DRUGSLIST>\r\n");
		for(cnt = 0;cnt < DRUG_TYPE_MAX;cnt++)
		{
			FilePrintf(hFile,"\t<DRUG>\r\n");

			FilePrintf(hFile,"\t\t<ubType>%d</ubType>\r\n",									cnt );
			FilePrintf(hFile,"\t\t<ubDrugTravelRate>%d</ubDrugTravelRate>\r\n",				Drug[cnt].ubDrugTravelRate	);
			FilePrintf(hFile,"\t\t<ubDrugWearoffRate>%d</ubDrugWearoffRate>\r\n",			Drug[cnt].ubDrugWearoffRate	);
			FilePrintf(hFile,"\t\t<ubDrugEffect>%d</ubDrugEffect>\r\n",						Drug[cnt].ubDrugEffect	);
			FilePrintf(hFile,"\t\t<ubDrugSideEffect>%d</ubDrugSideEffect>\r\n",				Drug[cnt].ubDrugSideEffect	);
			FilePrintf(hFile,"\t\t<ubDrugSideEffectRate>%d</ubDrugSideEffectRate>\r\n",		Drug[cnt].ubDrugSideEffectRate	);
			FilePrintf(hFile,"\t\t<ubMoralBacklash>%d</ubMoralBacklash>\r\n",				Drug[cnt].ubMoralBacklash	);
			FilePrintf(hFile,"\t\t<ubDiseaseCure>%d</ubDiseaseCure>\r\n",					Drug[cnt].ubDiseaseCure );

			FilePrintf(hFile,"\t</DRUG>\r\n");
		}
		FilePrintf(hFile,"</DRUGSLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
