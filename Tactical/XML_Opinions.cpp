///////////////////////////////////////////////////////////////////////////////
// HEADROCK PROFEX: PROFile EXternalization
//
// This file handles all reading from Merc Profiles.XML. It offers an external
// alternative to PROEDIT. Values that have no current use in the game were
// EXCLUDED. If you wish to add them simply follow the example set here. You
// can read MERCPROFILESTRUCT in the file "soldier profile type.h" to see
// all profile data. Add the ones you want to externalize to TEMPPROFILESTRUCT 
// in "Soldier Profile.h", and then add the appropriate lines where needed,
// in this file, following my example.
///////////////////////////////////////////////////////////////////////////////

#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "gamesettings.h"
	#include "XML.h"
	#include "Soldier Profile.h"
#endif

//#define MAX_PROFILE_NAME_LENGTH 30

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	TEMPPROFILETYPE curProfile;
	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef profileParseData;

//TEMPPROFILETYPE tempProfiles[NUM_PROFILES+1];



static void XMLCALL
opinionStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	profileParseData * pData = (profileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MERCOPINIONS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "OPINION") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"MergeStartElementHandle: setting memory for curMerge");

			memset(&pData->curProfile,0,sizeof(TEMPPROFILETYPE));

			pData->maxReadDepth++; //we are not skipping this element

			//pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "zNickname") == 0 ||
				strcmp(name, "uiIndex") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT)
		{
			for(UINT8 i = 0; i < NUMBER_OF_OPINIONS; ++i)
			{
				XML_Char bla[12];
				sprintf(bla, "Opinion%d", i);
				
				if(strcmp(name, bla) == 0)
				{
					pData->curElement = ELEMENT_PROPERTY;

					pData->maxReadDepth++; //we are not skipping this element

					break;
				}
			}
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
opinionCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	profileParseData * pData = (profileParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
opinionEndElementHandle(void *userData, const XML_Char *name)
{
	profileParseData * pData = (profileParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "MERCOPINIONS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "OPINION") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curIndex < pData->maxArraySize)
			{

				// Write data into a temporary array that holds profiles. We will later copy data from that
				// temp array into the REAL profile array, one item at a time, replacing PROF.DAT data.
				memcpy( &(tempProfiles[pData->curIndex].bMercOpinion), &(pData->curProfile.bMercOpinion), NUMBER_OF_OPINIONS * sizeof(INT8) );

			}
		}

		else if(strcmp(name, "zNickname") == 0)
		{
			pData->curElement = ELEMENT;

			// Just a formality
		}

		else if(strcmp(name, "uiIndex") == 0)
		{

			pData->curElement = ELEMENT;

			// Sets new index for writing.
			pData->curIndex = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}

		else
		{
			for(UINT8 i = 0; i < NUMBER_OF_OPINIONS; ++i)
			{
				XML_Char bla[12];
				sprintf(bla, "Opinion%d", i);
				
				if(strcmp(name, bla) == 0)
				{
					pData->curElement = ELEMENT;

					pData->curProfile.bMercOpinion[i] = (INT8) atol(pData->szCharData);

					break;
				}
			}
		}
		
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInMercOpinions(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	profileParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading MercOpinions.xml" );

	// Open merges file
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


	XML_SetElementHandler(parser, opinionStartElementHandle, opinionEndElementHandle);
	XML_SetCharacterDataHandler(parser, opinionCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in MercOpinions.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteMercOpinions()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\MercOpinions out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<MERCOPINIONS>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES ;cnt++)
		{

			FilePrintf(hFile,"\t<OPINION>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",						cnt);

			//////////////////////////////
			// Write Character's Nickname

			FilePrintf(hFile,"\t\t<zNickname>");

			STR16 szRemainder = gMercProfiles[cnt].zNickname; //the remaining string to be output (for making valid XML)

			while(szRemainder[0] != '\0')
			{
				//UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\'\"\0");
				UINT32 uiCharLoc = wcscspn(szRemainder,L"&<>\0");
				CHAR16 invChar = szRemainder[uiCharLoc];

				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%S",szRemainder);
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

					//case '\'':
					//	FilePrintf(hFile,"&apos;");
					//	szRemainder++;
					//break;

					//case '\"':
					//	FilePrintf(hFile,"&quot;");
					//	szRemainder++;
					//break;
				}
			}

			FilePrintf(hFile,"</zNickname>\r\n");

			UINT8 cnt_b = 0;
			for (cnt_b = 0; cnt_b < NUMBER_OF_OPINIONS; ++cnt_b)
			{
				FilePrintf(hFile,"\t\t<Opinion%d>%d</Opinion%d>\r\n", cnt_b, gMercProfiles[ cnt ].bMercOpinion[cnt_b], cnt_b);
			}
			

			FilePrintf(hFile,"\t</OPINION>\r\n");
		}
		FilePrintf(hFile,"</MERCOPINIONS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
