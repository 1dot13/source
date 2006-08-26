#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "sys globals.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "worldman.h"
	#include "math.h"
	#include "points.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "opplist.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "tile animation.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "explosion control.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "EnemyItemDrops.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	INT8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	ARMOUR_DROPS	curArmourDrop;
	ARMOUR_DROPS *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef armourDropParseData;

static void XMLCALL 
armourDropStartElementHandle(void *userData, const char *name, const char **atts)
{
	armourDropParseData * pData = (armourDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ARMOURDROPLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(ARMOUR_DROPS)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DROPITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curArmourDrop,0,sizeof(ARMOUR_DROPS));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 || 
				strcmp(name, "ubArmourClass") == 0 ||
				strcmp(name, "ubEnemyDropRate") == 0 ||
				strcmp(name, "ubMilitiaDropRate") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
armourDropCharacterDataHandle(void *userData, const char *str, int len)
{
	armourDropParseData * pData = (armourDropParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
armourDropEndElementHandle(void *userData, const char *name)
{	
	armourDropParseData * pData = (armourDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ARMOURDROPLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DROPITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curArmourDrop.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curArmourDrop.uiIndex] = pData->curArmourDrop; //write the inventory into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmourDrop.uiIndex   = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubArmourClass") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmourDrop.ubArmourClass   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubEnemyDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmourDrop.ubEnemyDropRate  = (UINT8) atol(pData->szCharData);
		}
		else if (strcmp(name, "ubMilitiaDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curArmourDrop.ubMilitiaDropRate  = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInEnemyArmourDropsStats(ARMOUR_DROPS *pEnemyArmourDrops, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	armourDropParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyArmourDrops.xml" );

	// Open inventory file
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

	
	XML_SetElementHandler(parser, armourDropStartElementHandle, armourDropEndElementHandle);
	XML_SetCharacterDataHandler(parser, armourDropCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = pEnemyArmourDrops;
	pData.maxArraySize = MAX_DROP_ITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyArmourDrops.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteEnemyArmourDropsStats(ARMOUR_DROPS *pEnemyArmourDrops, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<ARMOURDROPLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<DROPITEM>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<ubArmourClass>%d</ubArmourClass>\r\n", pEnemyArmourDrops[cnt].ubArmourClass);
			FilePrintf(hFile,"\t\t<ubEnemyDropRate>%d</ubEnemyDropRate>\r\n", pEnemyArmourDrops[cnt].ubEnemyDropRate);
			FilePrintf(hFile,"\t\t<ubMilitiaDropRate>%d</ubMilitiaDropRate>\r\n", pEnemyArmourDrops[cnt].ubMilitiaDropRate);

			FilePrintf(hFile,"\t</DROPITEM>\r\n");
		}
		FilePrintf(hFile,"</ARMOURDROPLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}