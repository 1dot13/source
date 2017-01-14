#ifdef PRECOMPILEDHEADERS
#include "Tactical All.h"
#else
#include "sgp.h"
#include "Debug Control.h"
#include "expat.h"
#include "XML.h"
#include "IMP Confirm.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH + 1];
	IMP_VOICESET	curIMPVoice;
	IMP_VOICESET*	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef impVoiceParseData;

IMP_VOICESET gIMPVoice[NUM_IMP_VOICESETS];

static void XMLCALL
impVoicesStartElementHandle( void *userData, const XML_Char *name, const XML_Char **atts )
{
	impVoiceParseData * pData = (impVoiceParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //are we reading this element?
	{
		if ( strcmp( name, "IMPVOICELIST" ) == 0 && pData->curElement == ELEMENT_NONE )
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "VOICE" ) == 0 && pData->curElement == ELEMENT_LIST )
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( pData->curElement == ELEMENT &&
				  (strcmp( name, "szVoiceSetName" ) == 0 ||
				  strcmp( name, "voiceset" ) == 0 ||
				  strcmp( name, "bSex" ) == 0 ) )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
impVoicesCharacterDataHandle( void *userData, const XML_Char *str, int len )
{
	impVoiceParseData * pData = (impVoiceParseData *)userData;

	if ( (pData->currentDepth <= pData->maxReadDepth) &&
		 (strlen( pData->szCharData ) < MAX_CHAR_DATA_LENGTH) )
	{
		strncat( pData->szCharData, str, __min( (unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen( pData->szCharData ) ) );
	}
}

static void XMLCALL
impVoicesEndElementHandle( void *userData, const XML_Char *name )
{
	impVoiceParseData * pData = (impVoiceParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth )
	{
		if ( strcmp( name, "IMPVOICELIST" ) == 0 )
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if ( strcmp( name, "VOICE" ) == 0 )
		{
			pData->curElement = ELEMENT_LIST;

			wcscpy( gIMPVoice[pData->curIndex].szVoiceSetName, pData->curIMPVoice.szVoiceSetName );
			gIMPVoice[pData->curIndex].voiceset = pData->curIMPVoice.voiceset;
			gIMPVoice[pData->curIndex].bSex = pData->curIMPVoice.bSex;
			gIMPVoice[pData->curIndex].exists = TRUE;

			++pData->curIndex;
		}
		else if ( strcmp( name, "szVoiceSetName" ) == 0 )
		{
			pData->curElement = ELEMENT;

			MultiByteToWideChar( CP_UTF8, 0, pData->szCharData, -1, pData->curIMPVoice.szVoiceSetName, sizeof(pData->curIMPVoice.szVoiceSetName) / sizeof(pData->curIMPVoice.szVoiceSetName[0]) );
			pData->curIMPVoice.szVoiceSetName[sizeof(pData->curIMPVoice.szVoiceSetName) / sizeof(pData->curIMPVoice.szVoiceSetName[0]) - 1] = '\0';
		}
		else if ( strcmp( name, "voiceset" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curIMPVoice.voiceset = (UINT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "bSex" ) == 0 )
		{
			pData->curElement = ELEMENT;
			pData->curIMPVoice.bSex = (BOOLEAN)atol( pData->szCharData );
		}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}

BOOLEAN ReadInIMPVoices( STR fileName )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate( NULL );

	impVoiceParseData pData;
	pData.curIndex = 0;

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Loading IMPVoices.xml" );
	
	// Open file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return(FALSE);

	uiFSize = FileGetSize( hFile );
	lpcBuffer = (CHAR8 *)MemAlloc( uiFSize + 1 );

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree( lpcBuffer );
		return(FALSE);
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );

	XML_SetElementHandler( parser, impVoicesStartElementHandle, impVoicesEndElementHandle );
	XML_SetCharacterDataHandler( parser, impVoicesCharacterDataHandle );

	memset( &pData, 0, sizeof(pData) );
	XML_SetUserData( parser, &pData );

	if ( !XML_Parse( parser, lpcBuffer, uiFSize, TRUE ) )
	{
		CHAR8 errorBuf[511];

		sprintf( errorBuf, "XML Parser Error in IMPVoices.xml: %s at line %d", XML_ErrorString( XML_GetErrorCode( parser ) ), XML_GetCurrentLineNumber( parser ) );
		LiveMessage( errorBuf );

		MemFree( lpcBuffer );
		return FALSE;
	}

	MemFree( lpcBuffer );

	XML_ParserFree( parser );

	return(TRUE);
}
