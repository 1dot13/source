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
#endif

struct
{
	PARSE_STAGE	curElement;

	INT8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef ammoParseData;

static void XMLCALL 
ammoStartElementHandle(void *userData, const char *name, const char **atts)
{
	ammoParseData * pData = (ammoParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "AMMOLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "AMMO") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "AmmoCaliber") == 0 ||
				strcmp(name, "BRCaliber") == 0 ||
				strcmp(name, "BurstSound") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
ammoCharacterDataHandle(void *userData, const char *str, int len)
{
	ammoParseData * pData = (ammoParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
ammoEndElementHandle(void *userData, const char *name)
{	
	char temp;
	ammoParseData * pData = (ammoParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "AMMOLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "AMMO") == 0)
		{
			pData->curElement = ELEMENT_LIST;

		}
		else if(strcmp(name, "AmmoCaliber") == 0)
		{
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)
			{
				for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
				{
					temp = pData->szCharData[i];
					AmmoCaliber[pData->curIndex][i] = temp;
				}
			}

		}
		else if(strcmp(name, "BRCaliber") == 0)
		{
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)
			{
				for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
				{
					temp = pData->szCharData[i];
					BobbyRayAmmoCaliber[pData->curIndex][i] = temp;
				}
			}

		}
		else if(strcmp(name, "BurstSound") == 0)
		{
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)
			{
				for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
				{
					temp = pData->szCharData[i];
					gzBurstSndStrings[pData->curIndex][i] = temp;
				}
			}

		}


		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInAmmoStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	ammoParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AmmoStrings.xml" );

	// Open ammos file
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

	
	XML_SetElementHandler(parser, ammoStartElementHandle, ammoEndElementHandle);
	XML_SetCharacterDataHandler(parser, ammoCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS; 
	pData.curIndex = -1;
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AmmoStrings.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}
BOOLEAN WriteAmmoStats()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Ammos out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<AMMOLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			FilePrintf(hFile,"\t<AMMO>\r\n");

			UINT16 * szRemainder = AmmoCaliber[cnt]; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t\t<AmmoCaliber>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
				UINT16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",szRemainder);
					szRemainder[uiCharLoc] = invChar;
				}

				szRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						szRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						szRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						szRemainder++;
					break;

					case '\'':
						FilePrintf(hFile,"&apos;");
						szRemainder++;
					break;

					case '\"':
						FilePrintf(hFile,"&quot;");
						szRemainder++;
					break;
				}
			}
			FilePrintf(hFile,"</AmmoCaliber>\r\n");


			szRemainder = BobbyRayAmmoCaliber[cnt]; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t\t<BRCaliber>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
				UINT16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",szRemainder);
					szRemainder[uiCharLoc] = invChar;
				}

				szRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						szRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						szRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						szRemainder++;
					break;

					case '\'':
						FilePrintf(hFile,"&apos;");
						szRemainder++;
					break;

					case '\"':
						FilePrintf(hFile,"&quot;");
						szRemainder++;
					break;
				}
			}
			FilePrintf(hFile,"</BRCaliber>\r\n");


			INT8 * szRemainder2 = gzBurstSndStrings[cnt]; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t\t<BurstSound>");
			while(szRemainder2[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder2,"&<>\'\"\0");
				INT8 invChar = szRemainder2[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder2[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",szRemainder2);
					szRemainder2[uiCharLoc] = invChar;
				}

				szRemainder2 += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						szRemainder2++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						szRemainder2++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						szRemainder2++;
					break;

					case '\'':
						FilePrintf(hFile,"&apos;");
						szRemainder2++;
					break;

					case '\"':
						FilePrintf(hFile,"&quot;");
						szRemainder2++;
					break;
				}
			}
			FilePrintf(hFile,"</BurstSound>\r\n");

			FilePrintf(hFile,"\t</AMMO>\r\n");
		}
		FilePrintf(hFile,"</AMMOLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}