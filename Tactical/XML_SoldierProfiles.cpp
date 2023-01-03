#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	SOLDIER_PROFILE_VALUES	curSoldierProfile;
	SOLDIER_PROFILE_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	CHAR16 gzSoldierProfile[MAX_ENEMY_NAMES_CHARS];
}
typedef enemyProfileParseData;

UINT16 num_found_profiles;		// a helper variable during reading xmls

static void XMLCALL
soldierProfilesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	enemyProfileParseData * pData = (enemyProfileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "PROFILES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(SOLDIER_PROFILE_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PROFILE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curSoldierProfile,0,sizeof(SOLDIER_PROFILE_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "szName") == 0 ||
			    strcmp(name, "uiBodyType") == 0 ||
			    strcmp(name, "uiHair") == 0 ||
			    strcmp(name, "uiSkin") == 0 ||
			    strcmp(name, "uiTrait1") == 0 ||
			    strcmp(name, "uiTrait2") == 0 ||
			    strcmp(name, "uiTrait3") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
soldierProfilesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	enemyProfileParseData * pData = (enemyProfileParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
soldierProfilesEndElementHandle(void *userData, const XML_Char *name)
{
	enemyProfileParseData * pData = (enemyProfileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "PROFILES") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "PROFILE") == 0)
		{
			pData->curElement = ELEMENT_LIST;
			
			if(pData->curSoldierProfile.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curSoldierProfile.uiIndex] = pData->curSoldierProfile; //write the profiles into the table
			}
		
			num_found_profiles = pData->curSoldierProfile.uiIndex + 1;
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "szName") == 0 )
		{
			pData->curElement = ELEMENT;
			
			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curSoldierProfile.szName, sizeof(pData->curSoldierProfile.szName)/sizeof(pData->curSoldierProfile.szName[0]) );
			pData->curSoldierProfile.szName[sizeof(pData->curSoldierProfile.szName)/sizeof(pData->curSoldierProfile.szName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "uiBodyType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiBodyType	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiHair") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiHair	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiSkin") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiSkin	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiTrait1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiTrait[0]	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiTrait2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiTrait[1]	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "uiTrait3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curSoldierProfile.uiTrait[2]	= (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInSoldierProfiles(SOLDIER_PROFILE_VALUES* pProfiles, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	enemyProfileParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading SoldierProfile.xml" );
		
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


	XML_SetElementHandler(parser, soldierProfilesStartElementHandle, soldierProfilesEndElementHandle);
	XML_SetCharacterDataHandler(parser, soldierProfilesCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pProfiles;
	pData.maxArraySize = NUM_SOLDIER_PROFILES;

	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in SoldierProfiles.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}


BOOLEAN WriteSoldierProfiles( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<PROFILES>\r\n");
		for(cnt = 0; cnt < num_found_soldier_profiles[0]; ++cnt)
		{
			FilePrintf(hFile,"\t<PROFILE>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt);
			FilePrintf(hFile,"\t\t<uiBodyType>%d</uiBodyType>\r\n",			zSoldierProfile[0][cnt].uiBodyType);	
			FilePrintf(hFile,"\t\t<uiHair>%d</uiHair>\r\n",					zSoldierProfile[0][cnt].uiHair);	
			FilePrintf(hFile,"\t\t<uiSkin>%d</uiSkin>\r\n",					zSoldierProfile[0][cnt].uiSkin);	
			FilePrintf(hFile,"\t\t<uiTrait1>%d</uiTrait1>\r\n",				zSoldierProfile[0][cnt].uiTrait[0]);	
			FilePrintf(hFile,"\t\t<uiTrait2>%d</uiTrait2>\r\n",				zSoldierProfile[0][cnt].uiTrait[1]);	
			FilePrintf(hFile,"\t\t<uiTrait3>%d</uiTrait3>\r\n",				zSoldierProfile[0][cnt].uiTrait[2]);
			
			FilePrintf(hFile,"\t</PROFILE>\r\n");
		}
		FilePrintf(hFile,"</PROFILES>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
