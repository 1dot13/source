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
				strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "Opinion0") == 0 ||
				strcmp(name, "Opinion1") == 0 ||
				strcmp(name, "Opinion2") == 0 ||
				strcmp(name, "Opinion3") == 0 ||
				strcmp(name, "Opinion4") == 0 ||
				strcmp(name, "Opinion5") == 0 ||
				strcmp(name, "Opinion6") == 0 ||
				strcmp(name, "Opinion7") == 0 ||
				strcmp(name, "Opinion8") == 0 ||
				strcmp(name, "Opinion9") == 0 ||
				strcmp(name, "Opinion10") == 0 ||
				strcmp(name, "Opinion11") == 0 ||
				strcmp(name, "Opinion12") == 0 ||
				strcmp(name, "Opinion13") == 0 ||
				strcmp(name, "Opinion14") == 0 ||
				strcmp(name, "Opinion15") == 0 ||
				strcmp(name, "Opinion16") == 0 ||
				strcmp(name, "Opinion17") == 0 ||
				strcmp(name, "Opinion18") == 0 ||
				strcmp(name, "Opinion19") == 0 ||
				strcmp(name, "Opinion20") == 0 ||
				strcmp(name, "Opinion21") == 0 ||
				strcmp(name, "Opinion22") == 0 ||
				strcmp(name, "Opinion23") == 0 ||
				strcmp(name, "Opinion24") == 0 ||
				strcmp(name, "Opinion25") == 0 ||
				strcmp(name, "Opinion26") == 0 ||
				strcmp(name, "Opinion27") == 0 ||
				strcmp(name, "Opinion28") == 0 ||
				strcmp(name, "Opinion29") == 0 ||
				strcmp(name, "Opinion30") == 0 ||
				strcmp(name, "Opinion31") == 0 ||
				strcmp(name, "Opinion32") == 0 ||
				strcmp(name, "Opinion33") == 0 ||
				strcmp(name, "Opinion34") == 0 ||
				strcmp(name, "Opinion35") == 0 ||
				strcmp(name, "Opinion36") == 0 ||
				strcmp(name, "Opinion37") == 0 ||
				strcmp(name, "Opinion38") == 0 ||
				strcmp(name, "Opinion39") == 0 ||
				strcmp(name, "Opinion40") == 0 ||
				strcmp(name, "Opinion41") == 0 ||
				strcmp(name, "Opinion42") == 0 ||
				strcmp(name, "Opinion43") == 0 ||
				strcmp(name, "Opinion44") == 0 ||
				strcmp(name, "Opinion45") == 0 ||
				strcmp(name, "Opinion46") == 0 ||
				strcmp(name, "Opinion47") == 0 ||
				strcmp(name, "Opinion48") == 0 ||
				strcmp(name, "Opinion49") == 0 ||
				strcmp(name, "Opinion50") == 0 ||
				strcmp(name, "Opinion51") == 0 ||
				strcmp(name, "Opinion52") == 0 ||
				strcmp(name, "Opinion53") == 0 ||
				strcmp(name, "Opinion54") == 0 ||
				strcmp(name, "Opinion55") == 0 ||
				strcmp(name, "Opinion56") == 0 ||
				strcmp(name, "Opinion57") == 0 ||
				strcmp(name, "Opinion58") == 0 ||
				strcmp(name, "Opinion59") == 0 ||
				strcmp(name, "Opinion60") == 0 ||
				strcmp(name, "Opinion61") == 0 ||
				strcmp(name, "Opinion62") == 0 ||
				strcmp(name, "Opinion63") == 0 ||
				strcmp(name, "Opinion64") == 0 ||
				strcmp(name, "Opinion65") == 0 ||
				strcmp(name, "Opinion66") == 0 ||
				strcmp(name, "Opinion67") == 0 ||
				strcmp(name, "Opinion68") == 0 ||
				strcmp(name, "Opinion69") == 0 ||
				strcmp(name, "Opinion70") == 0 ||
				strcmp(name, "Opinion71") == 0 ||
				strcmp(name, "Opinion72") == 0 ||
				strcmp(name, "Opinion73") == 0 ||
				strcmp(name, "Opinion74") == 0
				))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
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
				memcpy( &(tempProfiles[pData->curIndex].bMercOpinion), &(pData->curProfile.bMercOpinion), 75 * sizeof(INT8) );

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

		else if(strcmp(name, "Opinion0") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[0] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion1") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[1] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion2") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[2] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion3") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[3] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion4") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[4] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion5") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[5] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion6") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[6] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion7") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[7] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion8") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[8] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion9") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[9] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion10") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[10] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion11") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[11] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion12") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[12] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion13") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[13] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion14") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[14] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion15") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[15] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion16") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[16] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion17") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[17] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion18") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[18] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion19") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[19] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion20") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[20] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion21") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[21] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion22") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[22] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion23") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[23] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion24") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[24] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion25") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[25] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion26") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[26] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion27") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[27] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion28") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[28] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion29") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[29] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion30") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[30] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion31") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[31] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion32") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[32] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion33") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[33] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion34") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[34] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion35") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[35] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion36") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[36] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion37") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[37] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion38") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[38] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion39") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[39] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion40") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[40] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion41") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[41] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion42") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[42] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion43") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[43] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion44") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[44] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion45") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[45] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion46") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[46] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion47") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[47] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion48") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[48] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion49") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[49] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion50") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[50] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion51") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[51] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion52") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[52] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion53") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[53] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion54") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[54] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion55") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[55] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion56") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[56] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion57") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[57] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion58") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[58] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion59") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[59] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion60") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[60] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion61") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[61] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion62") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[62] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion63") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[63] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion64") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[64] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion65") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[65] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion66") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[66] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion67") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[67] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion68") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[68] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion69") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[69] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion70") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[70] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion71") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[71] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion72") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[72] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion73") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[73] = (INT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "Opinion74") == 0)
		{
			pData->curElement = ELEMENT;

			pData->curProfile.bMercOpinion[74] = (INT8) atol(pData->szCharData);
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

		sprintf(errorBuf, "XML Parser Error in MercProfiles.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
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
			for (cnt_b = 0; cnt_b < 75; cnt_b++)
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
