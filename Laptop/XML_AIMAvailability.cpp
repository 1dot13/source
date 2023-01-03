#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "aim.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	AIM_AVAILABLE	curAimAvailability;
	AIM_AVAILABLE *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef aimAvailabilityParseData;

BOOLEAN AimAvailability_TextOnly;

static void XMLCALL
aimAvailabilityStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	aimAvailabilityParseData * pData = (aimAvailabilityParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "AIM_AVAILABLES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "AIM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "ProfilId") == 0 ||
				strcmp(name, "AimBioID") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
aimCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	aimAvailabilityParseData * pData = (aimAvailabilityParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
aimAvailabilityEndElementHandle(void *userData, const XML_Char *name)
{
	aimAvailabilityParseData * pData = (aimAvailabilityParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "AIM_AVAILABLES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "AIM") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!AimAvailability_TextOnly)
				{		
					gAimAvailability[pData->curAimAvailability.uiIndex].uiIndex = pData->curAimAvailability.uiIndex;
					gAimAvailability[pData->curAimAvailability.uiIndex].ProfilId = pData->curAimAvailability.ProfilId;
					gAimAvailability[pData->curAimAvailability.uiIndex].ubAimArrayID = pData->curAimAvailability.uiIndex;
					gAimAvailability[pData->curAimAvailability.uiIndex].AimBio = pData->curAimAvailability.AimBio;
					
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].uiIndex = pData->curAimAvailability.uiIndex;
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].ProfilId = pData->curAimAvailability.ProfilId;
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].ubAimArrayID = pData->curAimAvailability.uiIndex;
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].AimBio = pData->curAimAvailability.AimBio;
				}
				else
				{
					gAimAvailability[pData->curAimAvailability.uiIndex].uiIndex = pData->curAimAvailability.uiIndex;
					gAimAvailability[pData->curAimAvailability.uiIndex].ProfilId = pData->curAimAvailability.ProfilId;
					gAimAvailability[pData->curAimAvailability.uiIndex].ubAimArrayID = pData->curAimAvailability.uiIndex;
					gAimAvailability[pData->curAimAvailability.uiIndex].AimBio = pData->curAimAvailability.AimBio;
					
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].uiIndex = pData->curAimAvailability.uiIndex;
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].ProfilId = pData->curAimAvailability.ProfilId;
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].ubAimArrayID = pData->curAimAvailability.uiIndex;
					gAimAvailabilityTemp[pData->curAimAvailability.uiIndex].AimBio = pData->curAimAvailability.AimBio;
				}		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAimAvailability.uiIndex	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ProfilId") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAimAvailability.ProfilId	= (UINT8) atol(pData->szCharData);
		}	
		else if(strcmp(name, "AimBioID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAimAvailability.AimBio	= (UINT8) atol(pData->szCharData);
		}	
		
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInAimAvailability(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	aimAvailabilityParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AimAvailability.xml" );

	AimAvailability_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, aimAvailabilityStartElementHandle, aimAvailabilityEndElementHandle);
	XML_SetCharacterDataHandler(parser, aimCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AimAvailability.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteAimAvailability(STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<AIM_AVAILABLES>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES;cnt++)
		{

			FilePrintf(hFile,"\t<AIM>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
		//	FilePrintf(hFile,"\t\t<ProfilId>-1</ProfilId>\r\n");
		//	FilePrintf(hFile,"\t\t<AIMBioID>0</AIMBioID>\r\n");
		
			FilePrintf(hFile,"\t\t<ProfilId>%d</ProfilId>\r\n", gAimAvailability[cnt].ProfilId);
			FilePrintf(hFile,"\t\t<AIMBioID>%d</AIMBioID>\r\n", gAimAvailability[cnt].AimBio);
			FilePrintf(hFile,"\t</AIM>\r\n");
		}
		FilePrintf(hFile,"</AIM_AVAILABLES>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}