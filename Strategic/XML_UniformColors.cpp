////////////////////////////////////////////////////////////
//
//	HEADROCK HAM 3.6:
//
//  This code reads an XML called "UniformColors.XML", which tells the program which colors to use for
//  "standard" uniforms, including randomly-generated enemies and militia (for now).
//
//////////////////////////////////////////////////////////////


#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "XML.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "XML.h"
	#include "expat.h"
	#include "string.h"
	#include "Campaign Types.h"
	#include "FileMan.h"
	#include "MemMan.h"
	#include "Debug Control.h"
	#include "mapscreen.h"
#endif

#define MAX_CHAR_DATA_LENGTH			500

// Error strings for this XML reader
STR16 szUniformErrorStrings[]=
{
	L"ENEMY_ADMIN",
	L"ENEMY_TROOP",
	L"ENEMY_ELITE",
	L"MILITIA_ROOKIE",
	L"MILITIA_REGULAR",
	L"MILITIA_ELITE",
};

// Array to hold uniform data
UNIFORMCOLORS gUniformColors[NUM_UNIFORMS];

typedef enum
{
	UNIFORM_ELEMENT_NONE = 0,
	UNIFORM_ELEMENT_LIST,
	UNIFORM_ELEMENT,
	UNIFORM_ELEMENT_DATA,
} UNIFORM_PARSE_STAGE;

typedef struct
{
	UNIFORM_PARSE_STAGE	curElement;
	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT8			sCurUniform;
	PaletteRepID	szCurVest;
	PaletteRepID	szCurPants;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
} UniformParseData;

// Array for flags making sure that we have ALL uniform data defined.
BOOLEAN UniformDataFound[NUM_UNIFORMS];

static void XMLCALL
UniformStartElementHandle(void *userData, const XML_Char *name, const char **atts)
{
	UniformParseData * pData = (UniformParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "UNIFORMS") == 0 && pData->curElement == UNIFORM_ELEMENT_NONE)
		{
			pData->curElement = UNIFORM_ELEMENT_LIST;
			pData->maxReadDepth++; //we are not skipping this element
			for (UINT8 cnt = 0; cnt < NUM_UNIFORMS; cnt++)
			{
				// reset array
				UniformDataFound[cnt] = FALSE;
			}
		}

		else if(pData->curElement == UNIFORM_ELEMENT_LIST &&
			(strcmp(name, "ENEMY_ADMIN") == 0 || 
			strcmp(name, "ENEMY_TROOP") == 0 ||
			strcmp(name, "ENEMY_ELITE") == 0 ||
			strcmp(name, "MILITIA_ROOKIE") == 0 ||
			strcmp(name, "MILITIA_REGULAR") == 0 ||
			strcmp(name, "MILITIA_ELITE") == 0 ) )
		{
			if (strcmp(name, "ENEMY_ADMIN") == 0)
			{
				pData->sCurUniform = UNIFORM_ENEMY_ADMIN;
			}
			else if (strcmp(name, "ENEMY_TROOP") == 0)
			{
				pData->sCurUniform = UNIFORM_ENEMY_TROOP;
			}
			else if (strcmp(name, "ENEMY_ELITE") == 0)
			{
				pData->sCurUniform = UNIFORM_ENEMY_ELITE;
			}
			else if (strcmp(name, "MILITIA_ROOKIE") == 0)
			{
				pData->sCurUniform = UNIFORM_MILITIA_ROOKIE;
			}
			else if (strcmp(name, "MILITIA_REGULAR") == 0)
			{
				pData->sCurUniform = UNIFORM_MILITIA_REGULAR;
			}
			else if (strcmp(name, "MILITIA_ELITE") == 0)
			{
				pData->sCurUniform = UNIFORM_MILITIA_ELITE;
			}
			pData->curElement = UNIFORM_ELEMENT;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if(pData->curElement == UNIFORM_ELEMENT &&
				(strcmp(name, "Vest") == 0  ||
				strcmp(name, "Pants") == 0 ))
		{
			pData->curElement = UNIFORM_ELEMENT_DATA;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}



static void XMLCALL
UniformCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	UniformParseData * pData = (UniformParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
UniformEndElementHandle(void *userData, const XML_Char *name)
{
	UniformParseData * pData = (UniformParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "UNIFORMS") == 0)
		{
			// All entries are mandatory. Exception thrown if an entry was not found
			for (UINT8 cnt = 0; cnt < NUM_UNIFORMS; cnt++)
			{
				if (!UniformDataFound[cnt])
				{
					CHAR16 sString[256];
					swprintf( sString, L"Error in UniformColors.XML: Uniform color for %s not found or not properly defined!", szUniformErrorStrings[ cnt ] );
					THROWEXCEPTION( sString );
				}
			}

			pData->curElement = UNIFORM_ELEMENT_NONE;
		}

		else if(strcmp(name, "ENEMY_ADMIN") == 0 || 
			strcmp(name, "ENEMY_TROOP") == 0 ||
			strcmp(name, "ENEMY_ELITE") == 0 ||
			strcmp(name, "MILITIA_ROOKIE") == 0 ||
			strcmp(name, "MILITIA_REGULAR") == 0 ||
			strcmp(name, "MILITIA_ELITE") == 0 )
		{
			pData->curElement = UNIFORM_ELEMENT_LIST;

			if (pData->sCurUniform >= UNIFORM_ENEMY_ADMIN && pData->sCurUniform < NUM_UNIFORMS )
			{
				memcpy( &(gUniformColors[pData->sCurUniform].pants), &(pData->szCurPants), sizeof(PaletteRepID) );
				memcpy( &(gUniformColors[pData->sCurUniform].vest), &(pData->szCurVest), sizeof(PaletteRepID) );
				UniformDataFound[pData->sCurUniform] = TRUE;
			}
		}

		else if(strcmp(name, "Vest") == 0 )
		{
			pData->curElement = UNIFORM_ELEMENT;

			strcpy(pData->szCurVest, pData->szCharData);
		}

		else if(strcmp(name, "Pants") == 0 )
		{
			pData->curElement = UNIFORM_ELEMENT;

			strcpy(pData->szCurPants, pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInUniforms(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	UniformParseData pData;

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


	XML_SetElementHandler(parser, UniformStartElementHandle, UniformEndElementHandle);
	XML_SetCharacterDataHandler(parser, UniformCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];
		sprintf(errorBuf, "XML Parser Error in UniformColors.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}
