// Lesh:
#include "builddefines.h"

#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include <stdio.h>
	#include <string.h>
	#include "explosion control.h"
	#include "TopicIDs.h"
	#include "TopicOps.h"
	#include "Debug.h"
	#include "FileMan.h"
	#include "Debug Control.h"
#endif

#include "expat.h"
#include "XML.h"

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	EXPLOSION_DATA	expData;
	INT32			maxArraySize;
	INT32			curIndex;	
	INT32			currentDepth;
	INT32			maxReadDepth;
}
typedef explosionDataParseData;

static void XMLCALL
explosionDataStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	explosionDataParseData * pData = (explosionDataParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "EXPDATALIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "EXPDATA") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//DebugMsg(TOPIC_JA2, DBG_LEVEL_3,"MergeStartElementHandle: setting memory for curMerge");

			memset(&(pData->expData),0,sizeof(EXPLOSION_DATA));

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "TransKeyFrame") == 0 ||
				strcmp(name, "DamageKeyFrame") == 0 ||
				strcmp(name, "ExplosionSoundID") == 0 ||
				strcmp(name, "AltExplosionSoundID") == 0 ||
				strcmp(name, "BlastFilename") == 0 ||
				strcmp(name, "BlastSpeed") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
explosionDataCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	explosionDataParseData * pData = (explosionDataParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) &&
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
explosionDataEndElementHandle(void *userData, const XML_Char *name)
{
	explosionDataParseData * pData = (explosionDataParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "EXPDATALIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "EXPDATA") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curIndex < pData->maxArraySize)
			{
				memcpy( &(gExpAniData[pData->curIndex]), &(pData->expData), sizeof(EXPLOSION_DATA) );
			}
		}
		else if(strcmp(name, "TransKeyFrame") == 0)
		{
			pData->curElement = ELEMENT;
			pData->expData.ubTransKeyFrame = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "DamageKeyFrame") == 0)
		{
			pData->curElement = ELEMENT;
			pData->expData.ubDamageKeyFrame = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ExplosionSoundID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->expData.uiExplosionSoundID = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "AltExplosionSoundID") == 0)
		{
			pData->curElement = ELEMENT;
			pData->expData.uiAltExplosionSoundID = (UINT32) strtoul(pData->szCharData, NULL, 0);
		}
		else if(strcmp(name, "BlastFilename") == 0)
		{
			pData->curElement = ELEMENT;
			// Lesh: maybe it is needed to check lenght of the pData->szCharData string
			// and warn user, otherwise game behavior is unexpected when trying to
			// play animation from clipped filename (runtime error, crash ???)
			strncpy(pData->expData.zBlastFilename, pData->szCharData, MAX_BLAST_FILENAME_LEN);
		}
		else if(strcmp(name, "BlastSpeed") == 0)
		{
			pData->curElement = ELEMENT;
			pData->expData.sBlastSpeed = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInExplosionDataStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	explosionDataParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading ExplosionData.xml" );

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


	XML_SetElementHandler(parser, explosionDataStartElementHandle, explosionDataEndElementHandle);
	XML_SetCharacterDataHandler(parser, explosionDataCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = NUM_EXP_TYPES;
	pData.curIndex = -1;

	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in ExplosionData.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}
BOOLEAN WriteExplosionDataStats()
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\ExplosionData out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		UINT32 cnt;

		FilePrintf(hFile,"<EXPDATALIST>\r\n");
		for(cnt = 0;cnt < NUM_EXP_TYPES;cnt++)
		{
			FilePrintf(hFile,"\t<EXPDATA>\r\n");

			FilePrintf(hFile,"\t\t<TransKeyFrame>%d</TransKeyFrame>\r\n",					gExpAniData[cnt].ubTransKeyFrame);
			FilePrintf(hFile,"\t\t<DamageKeyFrame>%d</DamageKeyFrame>\r\n",					gExpAniData[cnt].ubDamageKeyFrame);
			FilePrintf(hFile,"\t\t<ExplosionSoundID>%d</ExplosionSoundID>\r\n",				gExpAniData[cnt].uiExplosionSoundID);
			FilePrintf(hFile,"\t\t<AltExplosionSoundID>%d</AltExplosionSoundID>\r\n",		gExpAniData[cnt].uiAltExplosionSoundID);
			FilePrintf(hFile,"\t\t<BlastFilename>%s</BlastFilename>\r\n",					gExpAniData[cnt].zBlastFilename);
			FilePrintf(hFile,"\t\t<BlastSpeed>%d</BlastSpeed>\r\n",							gExpAniData[cnt].sBlastSpeed);

			FilePrintf(hFile,"\t</EXPDATA>\r\n");
		}
		FilePrintf(hFile,"</EXPDATALIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
