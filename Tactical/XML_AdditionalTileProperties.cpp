#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
#endif

#include "tiledef.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef additionalTilePropertyParseData;

static void XMLCALL
additionalTilePropertyStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	additionalTilePropertyParseData * pData = (additionalTilePropertyParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ADDITIONALTILEPROPERTIES") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT;

			memset(&zAdditionalTileProperties,0,sizeof(ADDITIONAL_TILE_PROPERTIES_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "ubTerrainID") == 0 ||
			   strcmp(name, "bWoodCamoAffinity") == 0 ||
			   strcmp(name, "bDesertCamoAffinity") == 0 ||
			   strcmp(name, "bUrbanCamoAffinity") == 0 ||
			   strcmp(name, "bSnowCamoAffinity") == 0 ||
			   strcmp(name, "bCamoStanceModifer") == 0 ||
			   strcmp(name, "bSoundModifier") == 0 ||
			   strcmp(name, "bStealthDifficultyModifer") == 0 ||
			   strcmp(name, "bTrapBonus") == 0 ||
			   strcmp(name, "fBlockedWindow") == 0
				))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
additionalTilePropertyCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	additionalTilePropertyParseData * pData = (additionalTilePropertyParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
additionalTilePropertyEndElementHandle(void *userData, const XML_Char *name)
{
	additionalTilePropertyParseData * pData = (additionalTilePropertyParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "ADDITIONALTILEPROPERTIES") == 0)
		{
			pData->curElement = ELEMENT;	
		}
		else if(strcmp(name, "ubTerrainID") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.ubTerrainID = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bWoodCamoAffinity") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bWoodCamoAffinity = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bDesertCamoAffinity") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bDesertCamoAffinity = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bUrbanCamoAffinity") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bUrbanCamoAffinity = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bSnowCamoAffinity") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bSnowCamoAffinity = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bCamoStanceModifer") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bCamoStanceModifer = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bSoundModifier") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bSoundModifier = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bStealthDifficultyModifer") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bStealthDifficultyModifer = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "bTrapBonus") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.bTrapBonus = (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "fBlockedWindow") == 0)
		{
			pData->curElement = ELEMENT;
			zAdditionalTileProperties.uiAdditionalFlags |= (UINT16) atol(pData->szCharData) ? ADDITIONAL_TILE_FLAG_BLOCKED_WINDOW: 0;
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInAdditionalTileProperties(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	additionalTilePropertyParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AdditionalTilePropertys.xml" );
		
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

	XML_SetElementHandler(parser, additionalTilePropertyStartElementHandle, additionalTilePropertyEndElementHandle);
	XML_SetCharacterDataHandler(parser, additionalTilePropertyCharacterDataHandle);

	memset(&pData,0,sizeof(pData));

	XML_SetUserData(parser, &pData);
	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in additionalTileProperty.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}


BOOLEAN WriteAdditionalTileProperties( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<additionalTilePropertyS>\r\n");
		for(cnt = 0; cnt < NUM_TAUNT; ++cnt)
		{
			FilePrintf(hFile,"\t<TILESET>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",				cnt);
			
			FilePrintf(hFile,"\t</TILESET>\r\n");
		}
		FilePrintf(hFile,"</additionalTilePropertyS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
