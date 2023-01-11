	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "mercs.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	CONTITION_FOR_MERC_AVAILABLE	curMercAvailability;
	CONTITION_FOR_MERC_AVAILABLE *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef mercAvailabilityParseData;

BOOLEAN MercAvailability_TextOnly;

static void XMLCALL
mercAvailabilityStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	mercAvailabilityParseData * pData = (mercAvailabilityParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MERC_AVAILABLES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "MERC") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "ProfilId") == 0 ||
				strcmp(name, "usMoneyPaid") == 0 ||
				strcmp(name, "Available") == 0 ||
				strcmp(name, "NewMercsAvailable") == 0 ||
				strcmp(name, "StartMercsAvailable") == 0 ||
				strcmp(name, "MercBioID") == 0 ||
				strcmp(name, "Drunk") == 0 ||
				strcmp(name, "uiAlternateIndex") == 0 ||
				strcmp(name, "usDay") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
mercCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	mercAvailabilityParseData * pData = (mercAvailabilityParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
mercAvailabilityEndElementHandle(void *userData, const XML_Char *name)
{
	mercAvailabilityParseData * pData = (mercAvailabilityParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "MERC_AVAILABLES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "MERC") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!MercAvailability_TextOnly)
				{		
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].uiIndex = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].ProfilId = pData->curMercAvailability.ProfilId;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].usMoneyPaid = pData->curMercAvailability.usMoneyPaid;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].usDay = pData->curMercAvailability.usDay;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].ubMercArrayID = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].NewMercsAvailable = pData->curMercAvailability.NewMercsAvailable;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].StartMercsAvailable = pData->curMercAvailability.StartMercsAvailable;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].MercBio = pData->curMercAvailability.MercBio;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].Drunk = pData->curMercAvailability.Drunk;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].uiAlternateIndex = pData->curMercAvailability.uiAlternateIndex;
					
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].uiIndex = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].ProfilId = pData->curMercAvailability.ProfilId;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].usMoneyPaid = pData->curMercAvailability.usMoneyPaid;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].usDay = pData->curMercAvailability.usDay;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].ubMercArrayID = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].NewMercsAvailable = pData->curMercAvailability.NewMercsAvailable;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].StartMercsAvailable = pData->curMercAvailability.StartMercsAvailable;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].MercBio = pData->curMercAvailability.MercBio;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].Drunk = pData->curMercAvailability.Drunk;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].uiAlternateIndex = pData->curMercAvailability.uiAlternateIndex;
				}
				else
				{
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].uiIndex = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].ProfilId = pData->curMercAvailability.ProfilId;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].usMoneyPaid = pData->curMercAvailability.usMoneyPaid;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].usDay = pData->curMercAvailability.usDay;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].ubMercArrayID = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].NewMercsAvailable = pData->curMercAvailability.NewMercsAvailable;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].StartMercsAvailable = pData->curMercAvailability.StartMercsAvailable;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].MercBio = pData->curMercAvailability.MercBio;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].Drunk = pData->curMercAvailability.Drunk;
					gConditionsForMercAvailability[pData->curMercAvailability.uiIndex].uiAlternateIndex = pData->curMercAvailability.uiAlternateIndex;
					
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].uiIndex = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].ProfilId = pData->curMercAvailability.ProfilId;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].usMoneyPaid = pData->curMercAvailability.usMoneyPaid;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].usDay = pData->curMercAvailability.usDay;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].ubMercArrayID = pData->curMercAvailability.uiIndex;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].NewMercsAvailable = pData->curMercAvailability.NewMercsAvailable;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].StartMercsAvailable = pData->curMercAvailability.StartMercsAvailable;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].MercBio = pData->curMercAvailability.MercBio;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].Drunk = pData->curMercAvailability.Drunk;
					gConditionsForMercAvailabilityTemp[pData->curMercAvailability.uiIndex].uiAlternateIndex = pData->curMercAvailability.uiAlternateIndex;

				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.uiIndex	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usMoneyPaid") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.usMoneyPaid	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usDay") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.usDay	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "ProfilId") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.ProfilId	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "NewMercsAvailable") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.NewMercsAvailable	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "StartMercsAvailable") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.StartMercsAvailable	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "MercBioID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.MercBio	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "Drunk") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.Drunk	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiAlternateIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercAvailability.uiAlternateIndex	= (UINT8) atol(pData->szCharData);
		}
		
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInMercAvailability(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	mercAvailabilityParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading MercAvailability.xml" );

	MercAvailability_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, mercAvailabilityStartElementHandle, mercAvailabilityEndElementHandle);
	XML_SetCharacterDataHandler(parser, mercCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in MercAvailability.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteMercAvailability(STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<MERC_AVAILABLES>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES;cnt++)
		{

			FilePrintf(hFile,"\t<MERC>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<usMoneyPaid>%d</usMoneyPaid>\r\n", gConditionsForMercAvailability[cnt].usMoneyPaid);
			FilePrintf(hFile,"\t\t<usDay>%d</usDay>\r\n", gConditionsForMercAvailability[cnt].usDay);
			FilePrintf(hFile,"\t\t<ProfilId>%d</ProfilId>\r\n", gConditionsForMercAvailability[cnt].ProfilId);
			FilePrintf(hFile,"\t</MERC>\r\n");
		}
		FilePrintf(hFile,"</MERC_AVAILABLES>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}