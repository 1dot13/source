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

// This array stores all XML-read placement data. There are four arrays for each sector, one array for each difficulty
// level. Please note that the Placement Type is only stored in array 0 (gBloodcatPlacements[x][0].PlacementType). It
// is not possible to have different placement types at different difficulty levels. Yet.
BLOODCATPLACEMENT gBloodcatPlacements[256][4];
// Location of the Bloodcat Lair. There can be only one, as there's a quest that relies on its location. This is a
// sectorID, 0-255.
UINT8 gubBloodcatLairSectorId;

typedef enum
{
	BLOODCAT_ELEMENT_NONE = 0,
	BLOODCAT_ELEMENT_PLACEMENTS,
	BLOODCAT_ELEMENT_SECTOR,
	BLOODCAT_ELEMENT,
	BLOODCAT_ELEMENT_DIFFICULTY,
	BLOODCAT_ELEMENT_DIFFICULTY_DATA,
} BLOODCAT_PARSE_STAGE;

typedef struct
{
	BLOODCAT_PARSE_STAGE	curElement;
	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT8			sCurSectorX;
	UINT8			sCurSectorY;
	BLOODCATPLACEMENT CurPlacement[4];
	UINT8			ubCurDifficulty;
	UINT32			currentDepth;
	UINT32			maxReadDepth;
} BloodcatsParseData;


static void XMLCALL
BloodcatsStartElementHandle(void *userData, const XML_Char *name, const char **atts)
{
	BloodcatsParseData * pData = (BloodcatsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "BLOODCAT_PLACEMENTS") == 0 && pData->curElement == BLOODCAT_ELEMENT_NONE)
		{
			pData->curElement = BLOODCAT_ELEMENT_PLACEMENTS;
			pData->maxReadDepth++; //we are not skipping this element
			// Initiate Array by setting first character to 0.
			for (UINT16 x = 0; x < 256; x++)
			{
				for (UINT16 y = 0; y < 4; y++)
				{
					gBloodcatPlacements[x][y].ubFactionAffiliation = 0;
					gBloodcatPlacements[x][y].fRespawn = 0;
					gBloodcatPlacements[x][y].PlacementType = 0;
					gBloodcatPlacements[x][y].ubAmbushChance = 0;
					gBloodcatPlacements[x][y].ubMaxBloodcats = 0;
					gBloodcatPlacements[x][y].ubMinBloodcats = 0;
				}				
			}
		}

		else if(strcmp(name, "SECTOR") == 0 && pData->curElement == BLOODCAT_ELEMENT_PLACEMENTS)
		{
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if(pData->curElement == BLOODCAT_ELEMENT_SECTOR &&
				(strcmp(name, "SectorGrid") == 0  ||
				strcmp(name, "ubPlacementType") == 0) )
		{
			pData->curElement = BLOODCAT_ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
		}

		else if(pData->curElement == BLOODCAT_ELEMENT_SECTOR &&
			strcmp(name, "NOVICE") == 0)
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->ubCurDifficulty = 0;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if(pData->curElement == BLOODCAT_ELEMENT_SECTOR &&
			strcmp(name, "EXPERIENCED") == 0)
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->ubCurDifficulty = 1;
			pData->maxReadDepth++; //we are not skipping this element
		}		
		else if(pData->curElement == BLOODCAT_ELEMENT_SECTOR &&
			strcmp(name, "EXPERT") == 0)
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->ubCurDifficulty = 2;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == BLOODCAT_ELEMENT_SECTOR &&
			strcmp(name, "INSANE") == 0)
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->ubCurDifficulty = 3;
			pData->maxReadDepth++; //we are not skipping this element
		}
				
		else if(pData->curElement == BLOODCAT_ELEMENT_DIFFICULTY &&
				(strcmp(name, "ubMinBloodcats") == 0  ||
				strcmp(name, "ubMaxBloodcats") == 0 ||
				strcmp(name, "ubAmbushChance") == 0 ||
				strcmp(name, "ubFactionAffiliation") == 0 ||
				strcmp(name, "fRespawn") == 0 ))
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY_DATA;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}



static void XMLCALL
BloodcatsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	BloodcatsParseData * pData = (BloodcatsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
BloodcatsEndElementHandle(void *userData, const XML_Char *name)
{
	BloodcatsParseData * pData = (BloodcatsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "BLOODCAT_PLACEMENTS") == 0)
		{
			pData->curElement = BLOODCAT_ELEMENT_NONE;
		}
		else if(strcmp(name, "SECTOR") == 0)
		{
			pData->curElement = BLOODCAT_ELEMENT_PLACEMENTS;

			UINT8 ubSectorId = SECTOR(pData->sCurSectorX, pData->sCurSectorY);
			if (ubSectorId >= 0 && ubSectorId < 256)
			{
				// Set placement type for this sector. Only first array has this set, as it is not difficulty-dependent.
				gBloodcatPlacements[ ubSectorId ][0].PlacementType = pData->CurPlacement[0].PlacementType;
				for (UINT8 x = 0; x < 4; x++)
				{
					if (pData->CurPlacement[0].PlacementType == 0)
					{
						// Ambush placement. Bloodcats will appear in this sector randomly.
						gBloodcatPlacements[ ubSectorId ][x].ubMinBloodcats = pData->CurPlacement[x].ubMinBloodcats;
						gBloodcatPlacements[ ubSectorId ][x].ubMaxBloodcats = pData->CurPlacement[x].ubMaxBloodcats;
						gBloodcatPlacements[ ubSectorId ][x].ubAmbushChance = pData->CurPlacement[x].ubAmbushChance;
						gBloodcatPlacements[ ubSectorId ][x].ubFactionAffiliation = 0;
						gBloodcatPlacements[ ubSectorId ][x].fRespawn = 0;
					}

					else if (pData->CurPlacement[0].PlacementType == 1)
					{
						// Static placement. Bloodcats always appear here, but never respawn, and may be unaggressive
						// to enemies.
						gBloodcatPlacements[ ubSectorId ][x].ubMinBloodcats = 0;
						gBloodcatPlacements[ ubSectorId ][x].ubMaxBloodcats = pData->CurPlacement[x].ubMaxBloodcats;
						gBloodcatPlacements[ ubSectorId ][x].ubAmbushChance = 0;
						gBloodcatPlacements[ ubSectorId ][x].ubFactionAffiliation = pData->CurPlacement[x].ubFactionAffiliation;
						gBloodcatPlacements[ ubSectorId ][x].fRespawn = 0;
					}

					else if (pData->CurPlacement[0].PlacementType == 2)
					{
						// Lair sector. There can be only one, and it is affected by the Quest. Bloodcats can regenerate
						// their numbers here over time.
						gubBloodcatLairSectorId = ubSectorId;

						gBloodcatPlacements[ ubSectorId ][x].ubMinBloodcats = 0;
						gBloodcatPlacements[ ubSectorId ][x].ubMaxBloodcats = pData->CurPlacement[x].ubMaxBloodcats;
						gBloodcatPlacements[ ubSectorId ][x].ubAmbushChance = 0;
						gBloodcatPlacements[ ubSectorId ][x].ubFactionAffiliation = 0;
						gBloodcatPlacements[ ubSectorId ][x].fRespawn = pData->CurPlacement[x].fRespawn;
					}
				}
			}
		}

		else if(strcmp(name, "SectorGrid") == 0 )
		{
			UINT8	x, y;
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;

			y = (UINT8)pData->szCharData[0] & 0x1F;
			x = (UINT8)atol(&pData->szCharData[1]);
			if ( x > 0 && x <= 16	&& y > 0 && y <= 16 )
			{
				pData->sCurSectorX	= x;
				pData->sCurSectorY  = y;
			}
			SGP_THROW_IFFALSE( (SECTOR(x,y) >= 0 && SECTOR(x,y) < 256) , L"Illegal sector number in BloodcatPlacements.XML" );
		}
		else if(strcmp(name, "ubPlacementType") == 0 )
		{
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;
			// Only set for first array (Difficulty Irrelevant)
			pData->CurPlacement[0].PlacementType = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "NOVICE") == 0 )
		{
			// Just return to sector data.
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;
		}

		else if(strcmp(name, "EXPERIENCED") == 0 )
		{
			// Just return to sector data.
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;
		}

		else if(strcmp(name, "EXPERT") == 0 )
		{
			// Just return to sector data.
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;
		}

		else if(strcmp(name, "INSANE") == 0 )
		{
			// Just return to sector data.
			pData->curElement = BLOODCAT_ELEMENT_SECTOR;
		}

		else if(strcmp(name, "ubMinBloodcats") == 0 )
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->CurPlacement[pData->ubCurDifficulty].ubMinBloodcats = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubMaxBloodcats") == 0 )
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->CurPlacement[pData->ubCurDifficulty].ubMaxBloodcats = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubAmbushChance") == 0 )
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->CurPlacement[pData->ubCurDifficulty].ubAmbushChance = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "ubFactionAffiliation") == 0 )
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->CurPlacement[pData->ubCurDifficulty].ubFactionAffiliation = (UINT8) atol(pData->szCharData);
		}

		else if(strcmp(name, "fRespawn") == 0 )
		{
			pData->curElement = BLOODCAT_ELEMENT_DIFFICULTY;
			pData->CurPlacement[pData->ubCurDifficulty].fRespawn = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInBloodcatPlacements(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	BloodcatsParseData pData;

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


	XML_SetElementHandler(parser, BloodcatsStartElementHandle, BloodcatsEndElementHandle);
	XML_SetCharacterDataHandler(parser, BloodcatsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];
		sprintf(errorBuf, "XML Parser Error in BloodcatPlacements.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}
