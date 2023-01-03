	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "mainmenuscreen.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	MAIN_MENU_VALUES	curMainMenu;
	MAIN_MENU_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef mainMenuParseData;

static void XMLCALL
mainMenuStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	mainMenuParseData * pData = (mainMenuParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MAINMENU") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(MAIN_MENU_VALUES)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "IMAGE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curMainMenu,0,sizeof(MAIN_MENU_VALUES));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "Visible") == 0 ||
				strcmp(name, "FileName") == 0 ||
				strcmp(name, "FitToScreen") == 0 ||
				strcmp(name, "StretchImage") == 0 ||
				strcmp(name, "CenterImage") == 0 ||
				strcmp(name, "ImagePositionRelative") == 0 ||
				strcmp(name, "ImagePositionX") == 0 ||
				strcmp(name, "ImagePositionY") == 0 ||
				strcmp(name, "MainMenuY") == 0 ||
				strcmp(name, "MainMenuX") == 0 ||
				strcmp(name, "MainMenuYSpace") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
mainMenuCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	mainMenuParseData * pData = (mainMenuParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
mainMenuEndElementHandle(void *userData, const XML_Char *name)
{
	char temp;
	mainMenuParseData * pData = (mainMenuParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "MAINMENU") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "IMAGE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curMainMenu.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curMainMenu.uiIndex] = pData->curMainMenu;
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMainMenu.uiIndex	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "Visible") == 0)
		{
			pData->curElement = ELEMENT;
			if (pData->curMainMenu.uiIndex == 0 ) pData->curMainMenu.Visible	= 0;
			 
			else pData->curMainMenu.Visible	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "FileName") == 0 && pData->curMainMenu.uiIndex > 0)
		{	
		
			pData->curElement = ELEMENT;
			
			if(MAX_MAIN_MENU_CHARS >= strlen(pData->szCharData))
				strcpy(pData->curMainMenu.FileName,pData->szCharData);
			else
			{
				strncpy(pData->curMainMenu.FileName,pData->szCharData,MAX_MAIN_MENU_CHARS);
				pData->curMainMenu.FileName[MAX_MAIN_MENU_CHARS-1] = '\0';
			}

			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_MAIN_MENU_CHARS);i++)			{
				temp = pData->szCharData[i];
				pData->curMainMenu.FileName[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: szLocation[%d] = %s, temp = %s",i,&pData->curSectorLoadscreens.szLocation[i],&temp));
			}				
		}
		else if(strcmp(name, "FitToScreen") == 0)
		{
			pData->curElement = ELEMENT;
			if (pData->curMainMenu.uiIndex == 0 ) pData->curMainMenu.FitToScreen	= 0;
			 
			else pData->curMainMenu.FitToScreen	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "StretchImage") == 0)
		{
			pData->curElement = ELEMENT;
			if (pData->curMainMenu.uiIndex == 0 ) pData->curMainMenu.StretchImage	= 0;
			 
			else pData->curMainMenu.StretchImage = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "CenterImage") == 0)
		{
			pData->curElement = ELEMENT;
			if (pData->curMainMenu.uiIndex == 0 ) pData->curMainMenu.CenterImage	= 0;
			 
			else pData->curMainMenu.CenterImage = (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePositionRelative") == 0)
		{
			pData->curElement = ELEMENT;
			if (pData->curMainMenu.uiIndex == 0 ) pData->curMainMenu.ImagePositionRelative	= 0;
			 
			else pData->curMainMenu.ImagePositionRelative	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "ImagePositionX") == 0 && pData->curMainMenu.uiIndex > 0)
		{
			pData->curElement = ELEMENT;
			pData->curMainMenu.ImagePositionX	= (INT32) atol(pData->szCharData);
		}	
		else if(strcmp(name, "ImagePositionY") == 0 && pData->curMainMenu.uiIndex > 0)
		{
			pData->curElement = ELEMENT;
			pData->curMainMenu.ImagePositionY	= (INT32) atol(pData->szCharData);
		}		
		else if(strcmp(name, "MainMenuY") == 0 && pData->curMainMenu.uiIndex == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMainMenu.MAINMENU_Y	=  (UINT32) strtoul(pData->szCharData, NULL, 0);
		}		
		else if(strcmp(name, "MainMenuX") == 0 && pData->curMainMenu.uiIndex == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMainMenu.MAINMENU_X	=  (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "MainMenuYSpace") == 0 && pData->curMainMenu.uiIndex == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMainMenu.MAINMENU_Y_SPACE	=  (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInMainMenu(MAIN_MENU_VALUES *pMainMenu, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	mainMenuParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Layout:LayoutMainMenu.xml" );

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


	XML_SetElementHandler(parser, mainMenuStartElementHandle, mainMenuEndElementHandle);
	XML_SetCharacterDataHandler(parser, mainMenuCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = pMainMenu;
	pData.maxArraySize = MAX_MAIN_MENU_IMAGE;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in MainMenu.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);
	

	return( TRUE );
}
