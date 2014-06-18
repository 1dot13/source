#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "XML.h"
#else
	#include "types.h"
	#include "Campaign Init.h"
	#include "Random.h"
	#include "Campaign Types.h"
	#include "Queen Command.h"
	#include "overhead.h"
	#include "Strategic Movement.h"
	#include "Strategic Status.h"
	#include "GameSettings.h"
	#include "Strategic AI.h"
	#include "XML.h"
	#include "expat.h"
	#include "Debug Control.h"
	#include "Tactical Save.h"
#endif

#include "connect.h"
#include <vfs/Core/vfs.h>
#include "XMLWriter.h"

#include "UndergroundInit.h"

extern BOOLEAN InitStrategicMovementCosts();
void InitKnowFacilitiesFlags( );

UNDERGROUND_SECTORINFO* gpUndergroundSectorInfoTail = NULL;

// Lesh: this array controls randomization of sectors
// Note that some sectors in game already using alternative map
// It is Skyrider quest: B15, E14, D12, C16
//		weapon caches: E11, H5, H10, J12, M9
//		Madlab quest: H7, H16, I11, E4
// Do not enable randomization of this sectors until you are know what you're doing
BOOLEAN RandomSector[256] =
{
	//		1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
	/* A */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* B */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* C */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* D */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* E */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* F */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* G */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* H */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* I */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* J */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* K */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* L */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* M */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* N */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* O */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* P */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};
typedef enum
{
	ALTSECTORS_ELEMENT_NONE = 0,
	ALTSECTORS_ELEMENT_LIST,
	ALTSECTORS_ELEMENT_ROW,
} ALTSECTORS_PARSE_STAGE;

typedef struct
{
	ALTSECTORS_PARSE_STAGE	curElement;
	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT32					uiRowNumber;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} altSectorsParseData;

static void XMLCALL
altSectorsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	altSectorsParseData * pData = (altSectorsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ALT_SECTORS_LIST") == 0 && pData->curElement == ALTSECTORS_ELEMENT_NONE)
		{
			pData->curElement = ALTSECTORS_ELEMENT_LIST;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ROW") == 0 && pData->curElement == ALTSECTORS_ELEMENT_LIST)
		{
			pData->curElement = ALTSECTORS_ELEMENT_ROW;

			if(strcmp(atts[0], "y") == 0)
			{
				if ( isalpha(atts[1][0]) )
					pData->uiRowNumber = (UINT32)(atts[1][0] - 0x40);
				else
					pData->uiRowNumber = atol(atts[1]);
			}
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
altSectorsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	altSectorsParseData * pData = (altSectorsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
altSectorsEndElementHandle(void *userData, const XML_Char *name)
{
	altSectorsParseData * pData = (altSectorsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ALT_SECTORS_LIST") == 0 && pData->curElement == ALTSECTORS_ELEMENT_LIST)
		{
			pData->curElement = ALTSECTORS_ELEMENT_NONE;
		}
		else if(strcmp(name, "ROW") == 0 && pData->curElement == ALTSECTORS_ELEMENT_ROW)
		{
			STR8 curBuffer = pData->szCharData + strspn(pData->szCharData," \t\n\r");
			UINT32 curCellIndex = 0;
			UINT32 curNumber;

			pData->curElement = ALTSECTORS_ELEMENT_LIST;

			while(curBuffer[0] != '\0')
			{
				sscanf( curBuffer,"%d",&curNumber);

				RandomSector[curCellIndex+16*(pData->uiRowNumber - 1)] = curNumber;

				curCellIndex++;
				curBuffer += strcspn(curBuffer," \t\n\r\0");
				curBuffer += strspn(curBuffer," \t\n\r");
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInAltSectors(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	altSectorsParseData pData;

	// Open xml file
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


	XML_SetElementHandler(parser, altSectorsStartElementHandle, altSectorsEndElementHandle);
	XML_SetCharacterDataHandler(parser, altSectorsCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AltSectors.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteInAltSectors(STR fileName)
{
#ifndef USE_VFS
	// Lets output the current Strategic map format using the XML structure I've devised.
	FILE *outfile = fopen(fileName, "wt");

	UINT32 x, y;

	fprintf (outfile, "<ALT_SECTORS_LIST>\n");
	for(y = 1;y <= 16;y++)
	{
		fprintf (outfile, "\t<ROW y=\"%c\">",(y+0x40));
		for(x = 1;x <= 16; x++)
		{
			fprintf (outfile, " %d",RandomSector[ ((y - 1) * 16) + (x - 1) ] );
		}
		fprintf (outfile, " </ROW>\n");
	}
	fprintf (outfile, "</ALT_SECTORS_LIST>\n");

	fclose(outfile);
#else
	XMLWriter xmlw;
	xmlw.openNode(L"ALT_SECTORS_LIST");
	UINT32 x, y;
	for(y = 1;y <= 16;y++)
	{
		xmlw.addAttributeToNextValue(L"y",(char)(y+0x40));
		std::stringstream ss;
		for(x = 1;x <= 16; x++)
		{
			ss << " " << RandomSector[ ((y - 1) * 16) + (x - 1) ];
		}
		xmlw.addValue(L"ROW", ss.str());
	}
	xmlw.closeNode();
	xmlw.writeToFile(fileName);
#endif
	return (TRUE);
}

UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ )
{
	UNDERGROUND_SECTORINFO *curr;
	curr = (UNDERGROUND_SECTORINFO*)MemAlloc( sizeof( UNDERGROUND_SECTORINFO ) );
	AssertMsg( curr, "Failed to create an underground sector info node." );
	memset( curr, 0, sizeof( UNDERGROUND_SECTORINFO ) );

	curr->ubSectorX = ubSectorX;
	curr->ubSectorY = ubSectorY;
	curr->ubSectorZ = ubSectorZ;

	if( gpUndergroundSectorInfoTail )
	{
		gpUndergroundSectorInfoTail->next = curr;
		gpUndergroundSectorInfoTail = gpUndergroundSectorInfoTail->next;
	}
	else
	{
		gpUndergroundSectorInfoHead = curr;
		gpUndergroundSectorInfoTail = gpUndergroundSectorInfoHead;
	}
	return curr;
}

// setup which know facilities are in which cities
void InitKnowFacilitiesFlags( )
{
	/////////////////////////////////////////////////////////////////
	// HEADROCK HAM 3.5: No longer required. Function disconnected.
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//SECTORINFO *pSector;

	// HEADROCK HAM 3.4: Externalized. All "known" facilities are read from gFacilityLocations. The
	// ones that have (fHidden=0) will be added to the sector info now, and thus visible immediately.
	// The others are added only once the player visits those sectors.

	/*for(UINT32 cnt=0; (gFacilityLocations[cnt].uiSectorID >= 0 && gFacilityLocations[cnt].uiSectorID <= 256); cnt++ )
	{
		// Is the current sector valid? Is the facility set to be revealed on game-start?
		if (gFacilityLocations[cnt].uiSectorID > 0 && gFacilityLocations[cnt].uiSectorID < 257 &&
			gFacilityLocations[cnt].fHidden == 0)
		{
			// Reveal the facility.
			pSector = &SectorInfo[gFacilityLocations[cnt].uiSectorID];
			pSector->uiFacilitiesFlags |= (1 << (gFacilityLocations[cnt].uiFacilityType-1));
		}
	}*/

	/* HEADROCK HAM 3.4: Externalized.
	// Cambria hospital
	pSector = &SectorInfo[SEC_G8];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
	pSector = &SectorInfo[SEC_F8];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
	pSector = &SectorInfo[SEC_G9];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
	pSector = &SectorInfo[SEC_F9];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;

	// Drassen airport
	pSector = &SectorInfo[SEC_B13];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_C13];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_D13];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;

	// Meduna airport & military complex
	pSector = &SectorInfo[SEC_N3];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_N4];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_N5];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_O3];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_O4];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	*/

	return;
}


//Mobile groups are handled separately from sectors, because they are on the move.
void GeneratePatrolGroups()
{
	GROUP *pGroup;
	UINT8 ubNumTroops;
	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_C7, 0, ubNumTroops, 0, 0 );
	pGroup->ubTransportationMask = CAR;
	AddWaypointToPGroup( pGroup, 8, 3 ); //C8
	AddWaypointToPGroup( pGroup, 7, 3 ); //C7

	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_D9, 0, ubNumTroops, 0, 0 );
	AddWaypointToPGroup( pGroup, 9, 5 ); //E9
	AddWaypointToPGroup( pGroup, 9, 4 ); //D9
	pGroup->ubTransportationMask = TRUCK;

	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_B9, 0, ubNumTroops, 0, 0 );
	AddWaypointToPGroup( pGroup, 12, 2 ); //B12
	AddWaypointToPGroup( pGroup, 9, 2 ); //B9
	pGroup->ubTransportationMask = FOOT;

	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_A14, 0, ubNumTroops, 0, 0 );
	pGroup->ubMoveType = ENDTOEND_FORWARDS;
	AddWaypointToPGroup( pGroup, 13, 1 ); //A13
	AddWaypointToPGroup( pGroup, 15, 1 ); //A15
	AddWaypointToPGroup( pGroup, 15, 5 ); //E15
	AddWaypointToPGroup( pGroup, 13, 5 ); //E13
	AddWaypointToPGroup( pGroup, 12, 5 ); //E12
	AddWaypointToPGroup( pGroup, 12, 3 ); //C12
	pGroup->ubTransportationMask = TRACKED;

	ubNumTroops = (UINT8)(5 + gGameOptions.ubDifficultyLevel * 2 + Random( 4 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_N6, 0, ubNumTroops, 0, 0 );
	AddWaypointToPGroup( pGroup, 9, 14 ); //N9
	AddWaypointToPGroup( pGroup, 6, 14 ); //N6
	pGroup->ubTransportationMask = CAR;

	ubNumTroops = (UINT8)(5 + gGameOptions.ubDifficultyLevel * 2 + Random( 4 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_N10, 0, ubNumTroops, 0, 0 );
	AddWaypointToPGroup( pGroup, 10, 11 ); //K10
	AddWaypointToPGroup( pGroup, 10, 14 ); //N10
	pGroup->ubTransportationMask = CAR;
}

void TrashUndergroundSectorInfo()
{
	UNDERGROUND_SECTORINFO *curr;
	while( gpUndergroundSectorInfoHead )
	{
		curr = gpUndergroundSectorInfoHead;
		gpUndergroundSectorInfoHead = gpUndergroundSectorInfoHead->next;
		MemFree( curr );
	}
	gpUndergroundSectorInfoHead = NULL;
	gpUndergroundSectorInfoTail = NULL;
}

//Defines the sectors that can be occupied by enemies, creatures, etc.	It also
//contains the network of cave connections critical for strategic creature spreading, as we can't
//know how the levels connect without loading the maps.	This is completely hardcoded, and any
//changes to the maps, require changes accordingly.
void BuildUndergroundSectorInfoList()
{
	UNDERGROUND_SECTORINFO *curr;
	SECTORINFO			   *pSector = NULL;

	TrashUndergroundSectorInfo();

	//if (LetLuaBuildUndergroundSectorInfoList())
	g_luaUnderground.InitializeSectorList();
		// lua script succeeded
		return;

	// If lua script had any errors
	// reset whatever the script may have done
	// and use hardcoded underground initialization.
	TrashUndergroundSectorInfo();

	//********************
	//* BASEMENT LEVEL 1 *
	//********************

	//Miguel's basement.	Nothing here.
	curr = NewUndergroundNode( 10, 1, 1 );

	//Chitzena mine.	Nothing here.
	curr = NewUndergroundNode( 2, 2, 1 );

	//San mona mine.	Nothing here.
	curr = NewUndergroundNode( 4, 4, 1 );
	curr = NewUndergroundNode( 5, 4, 1 );

	//J9
	curr = NewUndergroundNode( 9, 10, 1 );
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			curr->ubNumTroops = 8;
			break;
		case DIF_LEVEL_MEDIUM:
			curr->ubNumTroops = 11;
			break;
		case DIF_LEVEL_HARD:
			curr->ubNumTroops = 15;
			break;
		case DIF_LEVEL_INSANE:
			curr->ubNumTroops = 20;
			break;
	}
	//J9 feeding zone
	curr = NewUndergroundNode( 9, 10, 2 );
	curr->ubNumCreatures = (UINT8)(2 + gGameOptions.ubDifficultyLevel*2 + Random( 2 ));

	//K4
	curr = NewUndergroundNode( 4, 11, 1 );
	curr->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel*2 + Random( 3 ));
	curr->ubNumElites = (UINT8)(4 + gGameOptions.ubDifficultyLevel + Random( 2 ));

	//O3
	curr = NewUndergroundNode( 3, 15, 1 );
	curr->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel*2 + Random( 3 ));
	curr->ubNumElites = (UINT8)(4 + gGameOptions.ubDifficultyLevel + Random( 2 ));

	//P3
	curr = NewUndergroundNode( 3, 16, 1 );
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			curr->ubNumElites = (UINT8)(8 + Random( 3 ));
			break;
		case DIF_LEVEL_MEDIUM:
			curr->ubNumElites = (UINT8)(10 + Random( 5 ));
			break;
		case DIF_LEVEL_HARD:
			curr->ubNumElites = (UINT8)(14 + Random( 6 ));
			break;
		case DIF_LEVEL_INSANE:
			curr->ubNumElites = 20;
			break;
	}

	//Do all of the mandatory underground mine sectors

	//Drassen's mine
	//D13_B1
	curr = NewUndergroundNode( 13, 4, 1 );

	//E13_B1
	curr = NewUndergroundNode( 13, 5, 1 );

	//E13_B2
	curr = NewUndergroundNode( 13, 5, 2 );

	//F13_B2
	curr = NewUndergroundNode( 13, 6, 2 );

	//G13_B2
	curr = NewUndergroundNode( 13, 7, 2 );

	//G13_B3
	curr = NewUndergroundNode( 13, 7, 3 );

	//F13_B3
	curr = NewUndergroundNode( 13, 6, 3 );


	//Cambria's mine
	//H8_B1
	curr = NewUndergroundNode( 8, 8, 1 );

	//H9_B1
	curr = NewUndergroundNode( 9, 8, 1 );

	//H9_B2
	curr = NewUndergroundNode( 9, 8, 2 );

	//H8_B2
	curr = NewUndergroundNode( 8, 8, 2 );

	//H8_B3
	curr = NewUndergroundNode( 8, 8, 3 );

	//I8_B3
	curr = NewUndergroundNode( 8, 9, 3 );

	//J8_B3
	curr = NewUndergroundNode( 8, 10, 3 );


	//Alma's mine
	//I14_B1
	curr = NewUndergroundNode( 14, 9, 1 );

	//J14_B1
	curr = NewUndergroundNode( 14, 10, 1 );

	//J14_B2
	curr = NewUndergroundNode( 14, 10, 2 );

	//J13_B2
	curr = NewUndergroundNode( 13, 10, 2 );

	//J13_B3
	curr = NewUndergroundNode( 13, 10, 3 );

	//K13_B3
	curr = NewUndergroundNode( 13, 11, 3 );


	//Grumm's mine
	//H3_B1
	curr = NewUndergroundNode( 3, 8, 1 );

	//I3_B1
	curr = NewUndergroundNode( 3, 9, 1 );

	//I3_B2
	curr = NewUndergroundNode( 3, 9, 2 );

	//H3_B2
	curr = NewUndergroundNode( 3, 8, 2 );

	//H4_B2
	curr = NewUndergroundNode( 4, 8, 2 );
	curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;

	//H4_B3
	curr = NewUndergroundNode( 4, 8, 3 );

	//G4_B3
	curr = NewUndergroundNode( 4, 7, 3 );
}

// Lesh: this function creates randomized world
//		every sector can be randomized between common and alternative, chances 50/50
//		randomization of individual sectors can be switched off via array RandomSector[]
void InitWorld()
{
	INT16	sSectorCounter;

	for (sSectorCounter = 0; sSectorCounter < 256; sSectorCounter++)
	{
		// WANNE - MP: Disable "random sectors" in a multiplayer game
		if (!is_networked)
		{
			if ( RandomSector[ sSectorCounter ] )
			{
				if ( Random(100) >= 50 )
					SectorInfo[ sSectorCounter ].uiFlags |= SF_USE_ALTERNATE_MAP;
			}
		}
	}
}

//This is the function that is called only once, when the player begins a new game.	This will calculate
//starting numbers of the queen's army in various parts of the map, which will vary from campaign to campaign.
//This is also highly effected by the game's difficulty setting.
void InitNewCampaign()
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewCampaign");
	//First clear all the sector information of all enemy existance.	Conveniently, the
	//ubGroupType is also cleared, which is perceived to be an empty group.
	//memset( &SectorInfo, 0, sizeof( SECTORINFO ) * 256 );
	for (std::vector<SECTORINFO>::iterator secIter = SectorInfo.begin(); secIter != SectorInfo.end(); ++secIter) {
		memset( &(*secIter), 0, sizeof( SECTORINFO ) );
	}
	InitStrategicMovementCosts();
	RemoveAllGroups();

	InitWorld();	// Lesh: generate different world each time using alternative maps

	// HEADROCK HAM 3.5: This is no longer required.
	//InitKnowFacilitiesFlags( );

	BuildUndergroundSectorInfoList();
#ifdef JA2UB
// no UB
#else
	if (!is_networked)
		// allow overhead view of omerta A9 on game onset
		// HEADROCK HAM 3.5: Externalized.
		SetSectorFlag( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ, SF_ALREADY_VISITED ); //hayden
#endif

	//Generates the initial forces in a new campaign.	The idea is to randomize numbers and sectors
	//so that no two games are the same.
	InitStrategicAI();

	InitStrategicStatus();

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewCampaign done");
}
