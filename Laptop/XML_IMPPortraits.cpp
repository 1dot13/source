#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "IMP Confirm.h"
	#include "Soldier Profile.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	IMP_FACE_VALUES	curIMPPortraits;
	IMP_FACE_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	//CHAR16 gzIMPPortraits[MAX_IMP_NAMES_CHARS];
}
typedef impPortraitsParseData;

BOOLEAN IMPPortraits_TextOnly;

IMP_FACE_VALUES gIMPFaceValues[NUM_PROFILES];

static void XMLCALL
impPortraitsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	impPortraitsParseData * pData = (impPortraitsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "IMPPORTRAITSLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "IMP") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "PortraitId") == 0 ||
				strcmp(name, "bSex") == 0 ||
				strcmp(name, "usEyesX") == 0 ||
				strcmp(name, "usEyesY") == 0 ||
				strcmp(name, "usMouthY") == 0 ||
				strcmp(name, "usMouthX") == 0 ||
				strcmp(name, "DefaultSkin") == 0 ||
				strcmp(name, "DefaultHair") == 0 ||
				strcmp(name, "DefaultShirt") == 0 ||
				strcmp(name, "DefaultPants") == 0 ||
				strcmp(name, "DefaultBigBody") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
impPortraitsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	impPortraitsParseData * pData = (impPortraitsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH) )
	{
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}

static void XMLCALL
impPortraitsEndElementHandle(void *userData, const XML_Char *name)
{
	impPortraitsParseData * pData = (impPortraitsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "IMPPORTRAITSLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "IMP") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].uiEyeXPositions = pData->curIMPPortraits.uiEyeXPositions;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].uiEyeYPositions = pData->curIMPPortraits.uiEyeYPositions;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].uiMouthXPositions = pData->curIMPPortraits.uiMouthXPositions;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].uiMouthYPositions = pData->curIMPPortraits.uiMouthYPositions;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].PortraitId = pData->curIMPPortraits.PortraitId;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].bSex = pData->curIMPPortraits.bSex;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].iCurrentSkin = pData->curIMPPortraits.iCurrentSkin;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].iCurrentShirt = pData->curIMPPortraits.iCurrentShirt;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].iCurrentHair = pData->curIMPPortraits.iCurrentHair;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].iCurrentPants = pData->curIMPPortraits.iCurrentPants;
			gIMPFaceValues[pData->curIMPPortraits.uiIndex].bBigBody = pData->curIMPPortraits.bBigBody;
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "PortraitId") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.PortraitId	= (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "bSex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.bSex	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "usEyesX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.uiEyeXPositions	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usEyesY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.uiEyeYPositions	=  (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usMouthX") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.uiMouthXPositions	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "usMouthY") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.uiMouthYPositions	=  (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "DefaultSkin") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.iCurrentSkin	=  (INT32) atol(pData->szCharData);
		}		
		else if(strcmp(name, "DefaultHair") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.iCurrentHair	=  (INT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "DefaultShirt") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.iCurrentShirt	=  (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "DefaultPants") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.iCurrentPants	=  (INT32) atol(pData->szCharData);
		}		
		else if(strcmp(name, "DefaultBigBody") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curIMPPortraits.bBigBody	=  (INT32) atol(pData->szCharData);
		}	
	
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInIMPPortraits(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	impPortraitsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading IMPPortraits.xml" );

	IMPPortraits_TextOnly = localizedVersion;
	
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
	
	XML_SetElementHandler(parser, impPortraitsStartElementHandle, impPortraitsEndElementHandle);
	XML_SetCharacterDataHandler(parser, impPortraitsCharacterDataHandle);
	
	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in IMPPortraits.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

void LoadIMPPortraitsTEMP()
{
	UINT32 cnt2 = 0;

	for ( UINT32 cnt = 0; cnt < NUM_PROFILES; ++cnt )
	{
		if ( gIMPFaceValues[cnt].PortraitId !=0)
		{
			gIMPValues[cnt2].uiIndex = cnt2;					
			gIMPValues[cnt2].Enabled = 1;					
			gIMPValues[cnt2].uiEyeXPositions = gIMPFaceValues[cnt].uiEyeXPositions;
			gIMPValues[cnt2].uiEyeYPositions = gIMPFaceValues[cnt].uiEyeYPositions;
			gIMPValues[cnt2].uiMouthXPositions = gIMPFaceValues[cnt].uiMouthXPositions;
			gIMPValues[cnt2].uiMouthYPositions = gIMPFaceValues[cnt].uiMouthYPositions;
			gIMPValues[cnt2].PortraitId = gIMPFaceValues[cnt].PortraitId;
			gIMPValues[cnt2].bSex = gIMPFaceValues[cnt].bSex;
			gIMPValues[cnt2].iCurrentSkin = gIMPFaceValues[cnt].iCurrentSkin;
			gIMPValues[cnt2].iCurrentShirt = gIMPFaceValues[cnt].iCurrentShirt;
			gIMPValues[cnt2].iCurrentHair = gIMPFaceValues[cnt].iCurrentHair;
			gIMPValues[cnt2].iCurrentPants = gIMPFaceValues[cnt].iCurrentPants;
			gIMPValues[cnt2].bBigBody = gIMPFaceValues[cnt].bBigBody;
			++cnt2;
		}
	}
}
