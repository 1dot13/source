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
	#include "weapons.h"
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
#endif

struct
{
	PARSE_STAGE	curElement;

	INT8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	EXPLOSIVETYPE		curExplosive;
	EXPLOSIVETYPE *	curArray;
	UINT32			maxArraySize;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef explosiveParseData;

static void XMLCALL 
explosiveStartElementHandle(void *userData, const char *name, const char **atts)
{
	explosiveParseData * pData = (explosiveParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "EXPLOSIVELIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(EXPLOSIVETYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "EXPLOSIVE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curExplosive,0,sizeof(EXPLOSIVETYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "ubType") == 0 ||
				strcmp(name, "ubDamage") == 0 ||
				strcmp(name, "ubStunDamage") == 0 ||
				strcmp(name, "ubRadius") == 0 ||
				strcmp(name, "ubVolume") == 0 ||
				strcmp(name, "ubVolatility") == 0 ||
				strcmp(name, "ubStartRadius") == 0 ||
				strcmp(name, "ubDuration") == 0 ||
				strcmp(name, "ubAnimationID") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
explosiveCharacterDataHandle(void *userData, const char *str, int len)
{
	explosiveParseData * pData = (explosiveParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
explosiveEndElementHandle(void *userData, const char *name)
{
	explosiveParseData * pData = (explosiveParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "EXPLOSIVELIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "EXPLOSIVE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curExplosive.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curExplosive.uiIndex] = pData->curExplosive; //write the explosive into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.uiIndex   = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubType  = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubDamage = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubStunDamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubStunDamage   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubRadius") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubRadius   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubVolume") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubVolume   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubVolatility") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubVolatility   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubAnimationID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubAnimationID   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubStartRadius") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubStartRadius   = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDuration") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubDuration   = (UINT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInExplosiveStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	explosiveParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Explosives.xml" );

	// Open explosive file
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

	
	XML_SetElementHandler(parser, explosiveStartElementHandle, explosiveEndElementHandle);
	XML_SetCharacterDataHandler(parser, explosiveCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = Explosive;
	pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Explosives.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteExplosiveStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeexplosivestats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Explosives out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<EXPLOSIVELIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<EXPLOSIVE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<ubType>%d</ubType>\r\n",								Explosive[cnt].ubType  );
			FilePrintf(hFile,"\t\t<ubDamage>%d</ubDamage>\r\n",								Explosive[cnt].ubDamage   );
			FilePrintf(hFile,"\t\t<ubStunDamage>%d</ubStunDamage>\r\n",								Explosive[cnt].ubStunDamage   );
			FilePrintf(hFile,"\t\t<ubRadius>%d</ubRadius>\r\n",								Explosive[cnt].ubRadius   );
			FilePrintf(hFile,"\t\t<ubVolume>%d</ubVolume>\r\n",								Explosive[cnt].ubVolume   );
			FilePrintf(hFile,"\t\t<ubVolatility>%d</ubVolatility>\r\n",								Explosive[cnt].ubVolatility   );
			FilePrintf(hFile,"\t\t<ubAnimationID>%d</ubAnimationID>\r\n",								Explosive[cnt].ubAnimationID   );
			FilePrintf(hFile,"\t\t<ubDuration>%d</ubDuration>\r\n",								Explosive[cnt].ubDuration   );
			FilePrintf(hFile,"\t\t<ubStartRadius>%d</ubStartRadius>\r\n",								Explosive[cnt].ubStartRadius   );

			FilePrintf(hFile,"\t</EXPLOSIVE>\r\n");
		}
		FilePrintf(hFile,"</EXPLOSIVELIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}