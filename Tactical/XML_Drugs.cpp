#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "Drugs And Alcohol.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	DRUG		curDrugs;
	DRUG *		curArray;
	UINT32		maxArraySize;

	UINT32		currentDepth;
	UINT32		maxReadDepth;

	DRUG_EFFECT			drug_effects;
	DISEASE_EFFECT		disease_effects;
	DISABILITY_EFFECT	disability_effects;
	PERSONALITY_EFFECT	personality_effects;
}
typedef drugsParseData;

static void XMLCALL
drugsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	drugsParseData * pData = (drugsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "DRUGSLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(DRUG)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DRUG") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset( &pData->curDrugs, 0, sizeof(DRUG) );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szName") == 0 ||
				strcmp(name, "opinionevent" ) == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "DRUG_EFFECT" ) == 0) )
		{
			pData->curElement = ELEMENT_DRUG_EFFECT;

			// start of a new element: set default values
			pData->drug_effects.effect = 0;
			pData->drug_effects.duration = 0;
			pData->drug_effects.size = 0;
			pData->drug_effects.chance = 100;
				
			pData->maxReadDepth++;
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "DISEASE_EFFECT" ) == 0) )
		{
			pData->curElement = ELEMENT_DISEASE_EFFECT;

			// start of a new element: set default values
			pData->disease_effects.disease = 0;
			pData->disease_effects.size = 0;
			pData->disease_effects.chance = 100;

			pData->maxReadDepth++;
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "DISABILITY_EFFECT" ) == 0) )
		{
			pData->curElement = ELEMENT_DISABILITY_EFFECT;

			// start of a new element: set default values
			pData->disability_effects.disability = 0;
			pData->disability_effects.duration = 0;
			pData->disability_effects.chance = 100;

			pData->maxReadDepth++;
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "PERSONALITY_EFFECT" ) == 0) )
		{
			pData->curElement = ELEMENT_PERSONALITY_EFFECT;

			// start of a new element: set default values
			pData->personality_effects.personality = 0;
			pData->personality_effects.duration = 0;
			pData->personality_effects.chance = 100;

			pData->maxReadDepth++;
		}
		else if ( pData->curElement == ELEMENT_DRUG_EFFECT &&
				  (strcmp( name, "effect" ) == 0 ||
				  strcmp( name, "duration" ) == 0 ||
				  strcmp( name, "size" ) == 0 ||
				  strcmp( name, "chance" ) == 0 ) )
		{
			pData->curElement = ELEMENT_DRUG_EFFECT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT_DISEASE_EFFECT &&
				  (strcmp( name, "disease" ) == 0 ||
				  strcmp( name, "size" ) == 0 ||
				  strcmp( name, "chance" ) == 0) )
		{
			pData->curElement = ELEMENT_DISEASE_EFFECT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT_DISABILITY_EFFECT &&
				  (strcmp( name, "disability" ) == 0 ||
				  strcmp( name, "duration" ) == 0 ||
				  strcmp( name, "chance" ) == 0) )
		{
			pData->curElement = ELEMENT_DISABILITY_EFFECT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT_PERSONALITY_EFFECT &&
				  (strcmp( name, "personality" ) == 0 ||
				  strcmp( name, "duration" ) == 0 ||
				  strcmp( name, "chance" ) == 0) )
		{
			pData->curElement = ELEMENT_PERSONALITY_EFFECT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}
			
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
drugsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	drugsParseData * pData = (drugsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
drugsEndElementHandle(void *userData, const XML_Char *name)
{
	drugsParseData * pData = (drugsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "DRUGSLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DRUG") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			pData->curArray[pData->curDrugs.uiIndex] = pData->curDrugs; //write the drugs into the table
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.uiIndex = (UINT8)atol( pData->szCharData );
		}
		else if(strcmp(name, "szName") == 0)
		{
			pData->curElement = ELEMENT;
			// not needed, but its there for informational purposes

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curDrugs.szName, sizeof(pData->curDrugs.szName) / sizeof(pData->curDrugs.szName[0]) );
			pData->curDrugs.szName[sizeof(pData->curDrugs.szName) / sizeof(pData->curDrugs.szName[0]) - 1] = '\0';
		}
		else if(strcmp(name, "opinionevent") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curDrugs.opinionevent = (BOOLEAN)atol( pData->szCharData );
		}

		// Close opened Tags.
		else if ( strcmp( name, "DRUG_EFFECT" ) == 0  )
		{
			pData->curElement = ELEMENT;

			pData->curDrugs.drug_effects.push_back( pData->drug_effects );
		}
		else if ( strcmp( name, "DISEASE_EFFECT" ) == 0 )
		{
			pData->curElement = ELEMENT;

			pData->curDrugs.disease_effects.push_back( pData->disease_effects );
		}
		else if ( strcmp( name, "DISABILITY_EFFECT" ) == 0 )
		{
			pData->curElement = ELEMENT;

			pData->curDrugs.disability_effects.push_back( pData->disability_effects );
		}
		else if ( strcmp( name, "PERSONALITY_EFFECT" ) == 0 )
		{
			pData->curElement = ELEMENT;

			pData->curDrugs.personality_effects.push_back( pData->personality_effects );
		}

		else if ( pData->curElement == ELEMENT_DRUG_EFFECT_PROPERTY )
		{
			if ( strcmp( name, "effect" ) == 0 )
			{
				pData->drug_effects.effect = (UINT8)atol( pData->szCharData );
			}
			else if ( strcmp( name, "duration" ) == 0 )
			{
				pData->drug_effects.duration = (UINT16)atol( pData->szCharData );
			}
			else if ( strcmp( name, "size" ) == 0 )
			{
				pData->drug_effects.size = (INT16)atol( pData->szCharData );
			}
			else if ( strcmp( name, "chance" ) == 0 )
			{
				pData->drug_effects.chance = (UINT8)atol( pData->szCharData );
			}

			pData->curElement = ELEMENT_DRUG_EFFECT;
		}

		else if ( pData->curElement == ELEMENT_DISEASE_EFFECT_PROPERTY )
		{
			if ( strcmp( name, "disease" ) == 0 )
			{
				pData->disease_effects.disease = (UINT8)atol( pData->szCharData );
			}
			else if ( strcmp( name, "size" ) == 0 )
			{
				pData->disease_effects.size = (INT32)atol( pData->szCharData );
			}
			else if ( strcmp( name, "chance" ) == 0 )
			{
				pData->disease_effects.chance = (UINT8)atol( pData->szCharData );
			}

			pData->curElement = ELEMENT_DISEASE_EFFECT;
		}

		else if ( pData->curElement == ELEMENT_DISABILITY_EFFECT_PROPERTY )
		{
			if ( strcmp( name, "disability" ) == 0 )
			{
				pData->disability_effects.disability = (UINT8)atol( pData->szCharData );
			}
			else if ( strcmp( name, "duration" ) == 0 )
			{
				pData->disability_effects.duration = (UINT16)atol( pData->szCharData );
			}
			else if ( strcmp( name, "chance" ) == 0 )
			{
				pData->disability_effects.chance = (UINT8)atol( pData->szCharData );
			}

			pData->curElement = ELEMENT_DISABILITY_EFFECT;
		}

		else if ( pData->curElement == ELEMENT_PERSONALITY_EFFECT_PROPERTY )
		{
			if ( strcmp( name, "personality" ) == 0 )
			{
				pData->personality_effects.personality = (UINT8)atol( pData->szCharData );
			}
			else if ( strcmp( name, "duration" ) == 0 )
			{
				pData->personality_effects.duration = (UINT16)atol( pData->szCharData );
			}
			else if ( strcmp( name, "chance" ) == 0 )
			{
				pData->personality_effects.chance = (UINT8)atol( pData->szCharData );
			}

			pData->curElement = ELEMENT_PERSONALITY_EFFECT;
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInDrugsStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	drugsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Drugs.xml" );

	// Open drugs file
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


	XML_SetElementHandler(parser, drugsStartElementHandle, drugsEndElementHandle);
	XML_SetCharacterDataHandler(parser, drugsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.curArray = NewDrug;
	pData.maxArraySize = NEW_DRUGS_MAX;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Drugs.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteDrugsStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writedrugsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Drugs out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		FilePrintf(hFile,"<DRUGSLIST>\r\n");
		for ( UINT32 cnt = 0; cnt < NEW_DRUGS_MAX; ++cnt )
		{
			FilePrintf(hFile,"\t<DRUG>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									cnt );
			FilePrintf(hFile,"\t\t<opinionevent>%d</opinionevent>\r\n",							NewDrug[cnt].opinionevent );

			for ( std::vector<DRUG_EFFECT>::iterator it = NewDrug[cnt].drug_effects.begin( ); it != NewDrug[cnt].drug_effects.end( ); ++it )
			{
				FilePrintf( hFile, "\t\t<effect>%d</effect>\r\n",			(*it).effect );
				FilePrintf( hFile, "\t\t<duration>%d</duration>\r\n",		(*it).duration );
				FilePrintf( hFile, "\t\t<size>%d</size>\r\n",				(*it).size );
				FilePrintf( hFile, "\t\t<chance>%d</chance>\r\n",			(*it).chance );
			}

			for ( std::vector<DISEASE_EFFECT>::iterator it = NewDrug[cnt].disease_effects.begin( ); it != NewDrug[cnt].disease_effects.end( ); ++it )
			{
				FilePrintf( hFile, "\t\t<disease>%d</disease>\r\n",			(*it).disease );
				FilePrintf( hFile, "\t\t<size>%d</size>\r\n",				(*it).size );
				FilePrintf( hFile, "\t\t<chance>%d</chance>\r\n",			(*it).chance );
			}

			for ( std::vector<DISABILITY_EFFECT>::iterator it = NewDrug[cnt].disability_effects.begin( ); it != NewDrug[cnt].disability_effects.end( ); ++it )
			{
				FilePrintf( hFile, "\t\t<disability>%d</disability>\r\n",	(*it).disability );
				FilePrintf( hFile, "\t\t<duration>%d</duration>\r\n",		(*it).duration );
				FilePrintf( hFile, "\t\t<chance>%d</chance>\r\n",			(*it).chance );
			}

			for ( std::vector<PERSONALITY_EFFECT>::iterator it = NewDrug[cnt].personality_effects.begin( ); it != NewDrug[cnt].personality_effects.end( ); ++it )
			{
				FilePrintf( hFile, "\t\t<personality>%d</personality>\r\n", (*it).personality );
				FilePrintf( hFile, "\t\t<duration>%d</duration>\r\n",		(*it).duration );
				FilePrintf( hFile, "\t\t<chance>%d</chance>\r\n",			(*it).chance );
			}

			FilePrintf(hFile,"\t</DRUG>\r\n");
		}
		FilePrintf(hFile,"</DRUGSLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
