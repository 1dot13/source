	#include "sgp.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
	#include "Interface.h"

#include "soldier profile type.h"
#include "Soldier Profile.h"
#include "Random.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	RANDOM_STATS_VALUES	curRandomStats;
	RANDOM_STATS_VALUES *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef RandomStatsParseData;

BOOLEAN RandomStats_TextOnly;

static void XMLCALL
RandomStatsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	RandomStatsParseData * pData = (RandomStatsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "RANDOM_STATS") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "PROFILE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
			   ( strcmp(name, "uiIndex") == 0 ||
			     strcmp(name, "Enabled") == 0 ||
				 strcmp(name, "BaseAttribute") == 0 ||
				 strcmp(name, "RandomExpLevel") == 0 ||
				 strcmp(name, "RandomLife") == 0 ||
				 strcmp(name, "RandomAgility") == 0 ||
 				 strcmp(name, "RandomDexterity") == 0 ||
 				 strcmp(name, "RandomStrength") == 0 ||
				 strcmp(name, "RandomLeadership") == 0 ||
				 strcmp(name, "RandomWisdom") == 0 ||
				 strcmp(name, "RandomMarksmanship") == 0 ||
				 strcmp(name, "RandomMechanical") == 0 ||
				 strcmp(name, "RandomExplosive") == 0 ||
 				 strcmp(name, "RandomMedical") == 0 ||
				 strcmp(name, "RandomScientific") == 0 ) )

		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
RandomStatsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	RandomStatsParseData * pData = (RandomStatsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
RandomStatsEndElementHandle(void *userData, const XML_Char *name)
{
	RandomStatsParseData * pData = (RandomStatsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) 
	{
		if(strcmp(name, "RANDOM_STATS") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "PROFILE") == 0)
		{
			pData->curElement = ELEMENT_LIST;	
			
					gRandomStatsValue[pData->curRandomStats.uiIndex].Enabled = pData->curRandomStats.Enabled;
					gRandomStatsValue[pData->curRandomStats.uiIndex].BaseAttribute = pData->curRandomStats.BaseAttribute;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomExpLevel = pData->curRandomStats.RandomExpLevel;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomLife = pData->curRandomStats.RandomLife;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomAgility = pData->curRandomStats.RandomAgility;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomDexterity = pData->curRandomStats.RandomDexterity;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomStrength = pData->curRandomStats.RandomStrength;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomLeadership = pData->curRandomStats.RandomLeadership;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomWisdom = pData->curRandomStats.RandomWisdom;	
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomMarksmanship = pData->curRandomStats.RandomMarksmanship;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomMechanical = pData->curRandomStats.RandomMechanical;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomExplosive = pData->curRandomStats.RandomExplosive;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomMedical = pData->curRandomStats.RandomMedical;
					gRandomStatsValue[pData->curRandomStats.uiIndex].RandomScientific = pData->curRandomStats.RandomScientific;
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.uiIndex	= (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "Enabled") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.Enabled	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "BaseAttribute") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.BaseAttribute	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomExpLevel") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomExpLevel	= (INT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomLife") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomLife	= (BOOLEAN) atol(pData->szCharData);
		}		
		else if(strcmp(name, "RandomAgility") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomAgility	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "RandomDexterity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomDexterity	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomStrength") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomStrength	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomLeadership") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomLeadership	= (BOOLEAN) atol(pData->szCharData);
		}	
		else if(strcmp(name, "RandomWisdom") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomWisdom	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomMarksmanship") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomMarksmanship	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomMechanical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomMechanical	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomExplosive") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomExplosive	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomMedical") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomMedical	= (BOOLEAN) atol(pData->szCharData);
		}
		else if(strcmp(name, "RandomScientific") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curRandomStats.RandomScientific	= (BOOLEAN) atol(pData->szCharData);
		}			

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInRandomStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	RandomStatsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading RandomStats.xml" );

//	RandomStats_TextOnly = localizedVersion;
	
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	//if ( !hFile )
	//	return( localizedVersion );

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


	XML_SetElementHandler(parser, RandomStatsStartElementHandle, RandomStatsEndElementHandle);
	XML_SetCharacterDataHandler(parser, RandomStatsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in RandomStats.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteRandomStats( STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	{
		UINT32 cnt;

		FilePrintf(hFile,"<RANDOM_STATS>\r\n");
		for ( cnt = 0; cnt < NUM_PROFILES; cnt++ )
		{
			FilePrintf(hFile,"\t<PROFILE>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<Enabled>%d</Enabled>\r\n",gRandomStatsValue[cnt].Enabled);
			FilePrintf(hFile,"\t\t<BaseAttribute>%d</BaseAttribute>\r\n",gRandomStatsValue[cnt].BaseAttribute);				
			FilePrintf(hFile,"\t\t<RandomExpLevel>%d</RandomExpLevel>\r\n",gRandomStatsValue[cnt].RandomExpLevel);		
			FilePrintf(hFile,"\t\t<RandomLife>%d</RandomLife>\r\n",gRandomStatsValue[cnt].RandomLife);	
			FilePrintf(hFile,"\t\t<RandomAgility>%d</RandomAgility>\r\n",gRandomStatsValue[cnt].RandomAgility);	
			FilePrintf(hFile,"\t\t<RandomDexterity>%d</RandomDexterity>\r\n",gRandomStatsValue[cnt].RandomDexterity);
			FilePrintf(hFile,"\t\t<RandomStrength>%d</RandomStrength>\r\n",gRandomStatsValue[cnt].RandomStrength);		
			FilePrintf(hFile,"\t\t<RandomLeadership>%d</RandomLeadership>\r\n",gRandomStatsValue[cnt].RandomLeadership);
			FilePrintf(hFile,"\t\t<RandomWisdom>%d</RandomWisdom>\r\n",gRandomStatsValue[cnt].RandomWisdom);
			FilePrintf(hFile,"\t\t<RandomMarksmanship>%d</RandomMarksmanship>\r\n",gRandomStatsValue[cnt].RandomMarksmanship);
			FilePrintf(hFile,"\t\t<RandomMechanical>%d</RandomMechanical>\r\n",gRandomStatsValue[cnt].RandomMechanical);
			FilePrintf(hFile,"\t\t<RandomExplosive>%d</RandomExplosive>\r\n",gRandomStatsValue[cnt].RandomExplosive);
			FilePrintf(hFile,"\t\t<RandomMedical>%d</RandomMedical>\r\n",gRandomStatsValue[cnt].RandomMedical);
			FilePrintf(hFile,"\t\t<RandomScientific>%d</RandomScientific>\r\n",gRandomStatsValue[cnt].RandomScientific);		
			FilePrintf(hFile,"\t</PROFILE>\r\n");
		}
		FilePrintf(hFile,"</RANDOM_STATS>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}