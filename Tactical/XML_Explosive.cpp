#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	EXPLOSIVETYPE		curExplosive;
	EXPLOSIVETYPE *	curArray;
	UINT32			maxArraySize;

	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef explosiveParseData;

static void XMLCALL
explosiveStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
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
				strcmp(name, "ubMagSize") == 0 ||
				strcmp(name, "ubDuration") == 0 ||
				strcmp(name, "ubAnimationID") == 0 ||
				strcmp(name, "fExplodeOnImpact") == 0 ||// HEADROCK HAM 5: Explode on impact flag
				strcmp(name, "usNumFragments") == 0 || // HEADROCK HAM 5.1: Fragmenting explosive data
				strcmp(name, "ubFragType") == 0 ||
				strcmp(name, "ubFragDamage") == 0 ||
				strcmp(name, "ubFragRange") == 0 ||
				strcmp(name, "ubHorizontalDegree") == 0 ||
				strcmp(name, "ubVerticalDegree") == 0 ||
				strcmp(name, "bIndoorModifier") == 0 ))  
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
explosiveCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	explosiveParseData * pData = (explosiveParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
explosiveEndElementHandle(void *userData, const XML_Char *name)
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
			pData->curExplosive.uiIndex	= (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "ubType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubType	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubDamage = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubStunDamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubStunDamage	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubRadius") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubRadius	= (UINT8) atol(pData->szCharData);

			// Flugente: variable is UINT16, but is always used as UINT8, so better keep it in line
			pData->curExplosive.ubRadius	= max( 0, min(255, pData->curExplosive.ubRadius));
		}
		else if(strcmp(name, "ubVolume") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubVolume	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubVolatility") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubVolatility	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubAnimationID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubAnimationID	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubStartRadius") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubStartRadius	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubMagSize") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubMagSize	= (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ubDuration") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubDuration	= (UINT8) atol(pData->szCharData);
		}
		// HEADROCK HAM 5: Flag for "Explosion on Impact"
		else if(strcmp(name, "fExplodeOnImpact") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.fExplodeOnImpact = (BOOLEAN) atol(pData->szCharData);
		}

		// HEADROCK HAM 5.1: Four tags for Fragmenting Explosives
		else if(strcmp(name, "usNumFragments") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.usNumFragments = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubFragType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubFragType = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubFragDamage") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubFragDamage = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubFragRange") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubFragRange = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubHorizontalDegree") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubHorizontalDegree = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubVerticalDegree") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.ubVerticalDegree = (UINT16) atol(pData->szCharData);
		}

		else if(strcmp(name, "bIndoorModifier") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curExplosive.bIndoorModifier = (FLOAT) atof(pData->szCharData);
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

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									cnt );
			FilePrintf(hFile,"\t\t<ubType>%d</ubType>\r\n",										Explosive[cnt].ubType	);
			FilePrintf(hFile,"\t\t<ubDamage>%d</ubDamage>\r\n",									Explosive[cnt].ubDamage	);
			FilePrintf(hFile,"\t\t<ubStunDamage>%d</ubStunDamage>\r\n",							Explosive[cnt].ubStunDamage	);
			FilePrintf(hFile,"\t\t<ubRadius>%d</ubRadius>\r\n",									Explosive[cnt].ubRadius	);
			FilePrintf(hFile,"\t\t<ubVolume>%d</ubVolume>\r\n",									Explosive[cnt].ubVolume	);
			FilePrintf(hFile,"\t\t<ubVolatility>%d</ubVolatility>\r\n",							Explosive[cnt].ubVolatility	);
			FilePrintf(hFile,"\t\t<ubAnimationID>%d</ubAnimationID>\r\n",						Explosive[cnt].ubAnimationID	);
			FilePrintf(hFile,"\t\t<ubDuration>%d</ubDuration>\r\n",								Explosive[cnt].ubDuration	);
			FilePrintf(hFile,"\t\t<ubStartRadius>%d</ubStartRadius>\r\n",						Explosive[cnt].ubStartRadius	);
			FilePrintf(hFile,"\t\t<ubMagSize>%d</ubMagSize>\r\n",								Explosive[cnt].ubMagSize	);
			FilePrintf(hFile,"\t\t<fExplodeOnImpact>%d</fExplodeOnImpact>\r\n",					(UINT8)Explosive[cnt].fExplodeOnImpact	);

			FilePrintf(hFile,"\t\t<usNumFragments>%d</usNumFragments>\r\n",						Explosive[cnt].usNumFragments	);
			FilePrintf(hFile,"\t\t<ubFragType>%d</ubFragType>\r\n",								Explosive[cnt].ubFragType	);
			FilePrintf(hFile,"\t\t<ubFragDamage>%d</ubFragDamage>\r\n",							Explosive[cnt].ubFragDamage	);
			FilePrintf(hFile,"\t\t<ubFragRange>%d</ubFragRange>\r\n",							Explosive[cnt].fExplodeOnImpact	);
			FilePrintf(hFile,"\t\t<ubHorizontalDegree>%d</ubHorizontalDegree>\r\n",				Explosive[cnt].ubHorizontalDegree	);
			FilePrintf(hFile,"\t\t<ubVerticalDegree>%d</ubVerticalDegree>\r\n",					Explosive[cnt].ubVerticalDegree	);
			FilePrintf(hFile,"\t\t<bIndoorModifier>%d</bIndoorModifier>\r\n",					Explosive[cnt].bIndoorModifier	);

			FilePrintf(hFile,"\t</EXPLOSIVE>\r\n");
		}
		FilePrintf(hFile,"</EXPLOSIVELIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
