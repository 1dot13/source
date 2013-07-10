#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "Editor All.h"
	#include "LuaInitNPCs.h"
#else
	#include "Editor All.h"
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"
	#include "Item Statistics.h"
	#include "LuaInitNPCs.h"
#endif

#include "Action Items.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	ACTION_ITEM_VALUES	curActionItems;
	ACTION_ITEM_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
	//CHAR16 gzEnemyNames[MAX_ENEMY_NAMES_CHARS];
}
typedef actionItemsParseData;

BOOLEAN ActionItems_TextOnly;

static void XMLCALL
actionItemsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	actionItemsParseData * pData = (actionItemsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ACTION_ITEM_LIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ACTION_ITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   (strcmp(name, "uiIndex") == 0 ||
			    strcmp(name, "Name") == 0 ||
			    strcmp(name, "ActionID") == 0 ||
				strcmp(name, "Blow_up") == 0 ||
				strcmp(name, "BombItem") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
actionItemsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	actionItemsParseData * pData = (actionItemsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
actionItemsEndElementHandle(void *userData, const XML_Char *name)
{
	actionItemsParseData * pData = (actionItemsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "ACTION_ITEM_LIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ACTION_ITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
			if (!ActionItems_TextOnly)
				{
					//if ( ActionItemsValues[pData->curActionItems.uiIndex].ActionID > 0 )
					wcscpy(ActionItemsValues[pData->curActionItems.uiIndex].szName, pData->curActionItems.szName);
					
					#ifdef JA2EDITOR
					//if ( ActionItemsValues[pData->curActionItems.uiIndex].ActionID > 0 )
					wcscpy(gszActionItemDesc[pData->curActionItems.uiIndex], pData->curActionItems.szName);
					#endif
					
					// not ACTION_ITEM_BLOW_UP
					if ( pData->curActionItems.ActionID != ACTION_ITEM_BLOW_UP )
					{
						ActionItemsValues[pData->curActionItems.uiIndex].ActionID = pData->curActionItems.ActionID;
						ActionItemsValues[pData->curActionItems.uiIndex].BlowUp = 0;
						ActionItemsValues[pData->curActionItems.uiIndex].BombItem = 0;
					}
					else
					{
						ActionItemsValues[pData->curActionItems.uiIndex].ActionID = pData->curActionItems.ActionID;
						ActionItemsValues[pData->curActionItems.uiIndex].BlowUp = pData->curActionItems.BlowUp;
						ActionItemsValues[pData->curActionItems.uiIndex].BombItem = pData->curActionItems.BombItem;	
					}
					
				}
				else
				{
					//if ( ActionItemsValues[pData->curActionItems.uiIndex].ActionID > 0 )
					wcscpy(ActionItemsValues[pData->curActionItems.uiIndex].szName, pData->curActionItems.szName);
					
					#ifdef JA2EDITOR
					//if ( ActionItemsValues[pData->curActionItems.uiIndex].ActionID > 0 )
					wcscpy(gszActionItemDesc[pData->curActionItems.uiIndex], pData->curActionItems.szName);
					#endif
				}		
		
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curActionItems.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Name") == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curActionItems.szName, sizeof(pData->curActionItems.szName)/sizeof(pData->curActionItems.szName[0]) );
			pData->curActionItems.szName[sizeof(pData->curActionItems.szName)/sizeof(pData->curActionItems.szName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "ActionID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curActionItems.ActionID	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "Blow_up") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curActionItems.BlowUp	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "BombItem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curActionItems.BombItem	= (UINT16) atol(pData->szCharData);
		}	
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}




BOOLEAN ReadInActionItems(STR fileName, BOOLEAN localizedVersion)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	actionItemsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading ActionItems.xml" );

	ActionItems_TextOnly = localizedVersion;
	
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


	XML_SetElementHandler(parser, actionItemsStartElementHandle, actionItemsEndElementHandle);
	XML_SetCharacterDataHandler(parser, actionItemsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in ActionItems.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteInActionItems(STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<ACTION_ITEM_LIST>\r\n");
		for(cnt = 0;cnt < /*501*/NUM_ACTIONITEMS;cnt++)
		{
			FilePrintf(hFile,"\t<ACTION_ITEM>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<Name>Empty action</Name>\r\n");	
			FilePrintf(hFile,"\t\t<ActionID>%d</ActionID>\r\n", ActionItemsValues[cnt].ActionID);
			FilePrintf(hFile,"\t\t<Blow_up>%d</Blow_up>\r\n", ActionItemsValues[cnt].BlowUp);	
			FilePrintf(hFile,"\t\t<BombItem>%d</BombItem>\r\n", ActionItemsValues[cnt].BombItem);				
			FilePrintf(hFile,"\t</ACTION_ITEM>\r\n");
		}
		FilePrintf(hFile,"</ACTION_ITEM_LIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
