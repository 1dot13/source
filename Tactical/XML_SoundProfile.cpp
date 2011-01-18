#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

#include "Dialogue Control.h"
#include "soldier profile type.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	SOUND_PROFILE_VALUES	curSoundProfile;
	SOUND_PROFILE_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef SoundProfileParseData;

static void XMLCALL
SoundProfileStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	SoundProfileParseData * pData = (SoundProfileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "SOUNDPROFILE") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PROFILE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "EnabledSound") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
SoundProfileCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	SoundProfileParseData * pData = (SoundProfileParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
SoundProfileEndElementHandle(void *userData, const XML_Char *name)
{
	SoundProfileParseData * pData = (SoundProfileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "SOUNDPROFILE") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "PROFILE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			gSoundProfileValue[pData->curSoundProfile.uiIndex].EnabledSound = pData->curSoundProfile.EnabledSound;
		}	
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoundProfile.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "EnabledSound") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoundProfile.EnabledSound	= (BOOLEAN) atol(pData->szCharData);
		}	
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInSoundProfile(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	SoundProfileParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading SoundsProfiles.xml" );

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


	XML_SetElementHandler(parser, SoundProfileStartElementHandle, SoundProfileEndElementHandle);
	XML_SetCharacterDataHandler(parser, SoundProfileCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in SoundsProfiles.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteSoundProfile(STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<SOUNDPROFILE>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES;cnt++)
		{

			FilePrintf(hFile,"\t<PROFILE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<EnabledSound>1</EnabledSound>\r\n");
			FilePrintf(hFile,"\t</PROFILE>\r\n");
		}
		FilePrintf(hFile,"</SOUNDPROFILE>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}