#include "builddefines.h"
#include <stdio.h>
#include "XML.h"
#include "expat.h"
#include "DEBUG.h"
#include "string.h"
#include "Tactical Save.h"
#include "FileMan.h"
#include "MemMan.h"
#include "Debug Control.h"
#include "GameSettings.h"
#include "GameInitOptionsScreen.h"

typedef enum
{
	EXTRAITEMS_ELEMENT_NONE = 0,
	EXTRAITEMS_ELEMENT_EXTRAITEMS,
	EXTRAITEMS_ELEMENT_ITEM,
	EXTRAITEMS_ELEMENT,
} EXTRAITEMS_PARSE_STAGE;

typedef struct
{
	EXTRAITEMS_PARSE_STAGE	curElement;
	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT32					currentDepth;
	UINT32					maxReadDepth;

	UINT32					item;
	UINT32					quantity;
	UINT32					condition;
	UINT32					gridno;
	BOOLEAN					visible;
} ExtraItemsParseData;


static bool gSectorIsLoaded = false;
static UINT8 gX = 0;
static UINT8 gY = 0;
static UINT8 gZ = 0;

/** Process the opening tag in this expat callback.
 */
static void XMLCALL
ExtraItemsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	ExtraItemsParseData * pData = (ExtraItemsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ExtraItems") == 0 && pData->curElement == EXTRAITEMS_ELEMENT_NONE)
		{
			pData->curElement = EXTRAITEMS_ELEMENT_EXTRAITEMS;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "Item") == 0 && pData->curElement == EXTRAITEMS_ELEMENT_EXTRAITEMS)
		{
			pData->curElement = EXTRAITEMS_ELEMENT_ITEM;
			pData->maxReadDepth++; //we are not skipping this element

			// set defaults
			pData->quantity = 1;
			pData->condition = 100;
			pData->gridno = 0;
			pData->visible = FALSE;
		}
		else if(pData->curElement == EXTRAITEMS_ELEMENT_ITEM &&
			(strcmp(name, "uiIndex") == 0 ||
			strcmp(name, "quantity") == 0 ||
			strcmp(name, "condition") == 0 ||
			strcmp(name, "gridno") == 0 ||
			strcmp(name, "visible") == 0 ))
		{
			pData->curElement = EXTRAITEMS_ELEMENT;
			pData->maxReadDepth++;
		}
		pData->szCharData[0] = '\0';
	}
	pData->currentDepth++;
}

/** Process any text content in this callback.
 */
static void XMLCALL
ExtraItemsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	ExtraItemsParseData * pData = (ExtraItemsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}

/** Process the closing tag in this expat callback.
 */
static void XMLCALL
ExtraItemsEndElementHandle(void *userData, const XML_Char *name)
{
	ExtraItemsParseData * pData = (ExtraItemsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(pData->curElement == EXTRAITEMS_ELEMENT_EXTRAITEMS &&
			strcmp(name, "ExtraItems") == 0)
		{
			pData->curElement = EXTRAITEMS_ELEMENT_NONE;
		}
		else if(pData->curElement == EXTRAITEMS_ELEMENT_ITEM &&
				strcmp(name, "Item") == 0 &&
				pData->item > 0 && // Make sure not to create 0-index objects!
				pData->quantity > 0 && // Quantity should be positive. Not sure what the LIMIT is, if any.
				pData->condition > 0 && pData->condition <= 100 ) // Condition between 1 and 100!
		{
			OBJECTTYPE object;
			CreateItem(pData->item, pData->condition, &object);
			for (unsigned cnt=1; cnt <= pData->quantity; ++cnt) {
				if (gSectorIsLoaded) {
					AddItemToPool( pData->gridno, &object, pData->visible, 0, WORLD_ITEM_REACHABLE, 0 );
				} else {
					AddItemsToUnLoadedSector( gX, gY, gZ, pData->gridno, pData->visible, &object, 0, WORLD_ITEM_REACHABLE, 0, 1, 0 );
				}
			}
			pData->curElement = EXTRAITEMS_ELEMENT_EXTRAITEMS;
		}

		else if( pData->curElement == EXTRAITEMS_ELEMENT &&
				strcmp( name, "uiIndex" ) == 0)
		{
			pData->item = (UINT32)atol( pData->szCharData );
			pData->curElement = EXTRAITEMS_ELEMENT_ITEM;
		}
		else if( pData->curElement == EXTRAITEMS_ELEMENT &&
				strcmp( name, "quantity" ) == 0)
		{
			pData->quantity = (UINT32)atol( pData->szCharData );
			pData->curElement = EXTRAITEMS_ELEMENT_ITEM;
		}
		else if( pData->curElement == EXTRAITEMS_ELEMENT &&
				strcmp( name, "condition" ) == 0)
		{
			pData->condition = (UINT32)atol( pData->szCharData );
			pData->curElement = EXTRAITEMS_ELEMENT_ITEM;
		}
		else if( pData->curElement == EXTRAITEMS_ELEMENT &&
				strcmp( name, "gridno" ) == 0)
		{
			pData->gridno = (UINT32)atol( pData->szCharData );
			pData->curElement = EXTRAITEMS_ELEMENT_ITEM;
		}
		else if( pData->curElement == EXTRAITEMS_ELEMENT &&
				strcmp( name, "visible" ) == 0)
		{
			pData->visible = (UINT32)atol( pData->szCharData );
			pData->curElement = EXTRAITEMS_ELEMENT_ITEM;
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}


void AddExtraItems(UINT8 x, UINT8 y, UINT8 z, bool sectorIsLoaded)
{
	char fileName[MAX_PATH];
	strcpy(fileName, TABLEDATA_DIRECTORY);
	if (x < 10) {
		strcat(fileName, EXTRAITEMSFILENAME);
		char* spot = strstr(fileName, "A9_0");
		Assert(spot);
		if (!spot)
			return;
		*spot = 'A' + y-1;
		*(spot+1) = '0' + x;
		*(spot+3) = '0' + z;
	} else {
		strcat(fileName, EXTRAITEMSFILENAME2);
		char* spot = strstr(fileName, "A11_0");
		Assert(spot);
		if (!spot)
			return;
		*spot = 'A' + y-1;
		*(spot+1) = '0' + x / 10;
		*(spot+2) = '0' + x % 10;
		*(spot+4) = '0' + z;
	}

	// Remember the base file name length
	int baseFNLength = strlen(fileName);

	// Append a suffix based on the difficulty
	switch (gGameOptions.ubDifficultyLevel) {
		case DIF_LEVEL_ZERO:
			break;
		case DIF_LEVEL_EASY:
			strcat(fileName, "_Novice");
			break;
		case DIF_LEVEL_MEDIUM:
			strcat(fileName, "_Experienced");
			break;
		case DIF_LEVEL_HARD:
			strcat(fileName, "_Expert");
			break;
		case DIF_LEVEL_INSANE:
			strcat(fileName, "_Insane");
			break;
		case DIF_LEVEL_FIVE:
			strcat(fileName, "_Diff_5");
			break;
		case 6:
			strcat(fileName, "_Diff_6");
			break;
		case 7:
			strcat(fileName, "_Diff_7");
			break;
		case 8:
			strcat(fileName, "_Diff_8");
			break;
		case 9:
			strcat(fileName, "_Diff_9");
			break;
		case 10:
			strcat(fileName, "_Diff_10");
			break;
		case 11:
			strcat(fileName, "_Diff_11");
			break;
		case 12:
			strcat(fileName, "_Diff_12");
			break;
		case 13:
			strcat(fileName, "_Diff_13");
			break;
		case 14:
			strcat(fileName, "_Diff_14");
			break;
		case 15:
			strcat(fileName, "_Diff_15");
			break;
		default:
			break;
	}
	/*
	switch (gGameOptions.ubDifficultyLevel) {
		case DIF_LEVEL_ZERO:
			break;
		case DIF_LEVEL_EASY:
			strcat(fileName, "_Novice");
			break;
		case DIF_LEVEL_MEDIUM:
			strcat(fileName, "_Experienced");
			break;
		case DIF_LEVEL_HARD:
			strcat(fileName, "_Expert");
			break;
		case DIF_LEVEL_INSANE:
			strcat(fileName, "_Insane");
			break;
		case DIF_LEVEL_FIVE:
			break;
		default:
			break;
	}
	*/

	strcat(fileName, ".xml");
	if(!FileExists(fileName))//dnl ch75 261013 just to avoid sdd::exception under debug from VFS when file not exist
		return;

	// Open extra items file
	HWFILE hFile;
	// Try "special" difficulty level file first
	if (!(hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE ))) {
		// OK, that didn't work so try the "plain" file
		fileName[baseFNLength] = '\0';
		if (!(hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE ))) {
			// There isn't a file for this sector, just return
			return;
		}
	}

	gSectorIsLoaded = sectorIsLoaded;
	gX = x;
	gY = y;
	gZ = z;

	UINT32		uiBytesRead;
	XML_Parser	parser = XML_ParserCreate(NULL);
	ExtraItemsParseData pData;

	UINT32 uiFSize = FileGetSize(hFile);
	CHAR8 * lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) ) {
		MemFree(lpcBuffer);
		XML_ParserFree(parser);
		return;
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );

	// Setup expat callbacks
	XML_SetElementHandler(parser, ExtraItemsStartElementHandle, ExtraItemsEndElementHandle);
	XML_SetCharacterDataHandler(parser, ExtraItemsCharacterDataHandle);

	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in ExtraItems.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		XML_ParserFree(parser);
		return;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);
	return;
}
