#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "XML.h"
#else
	#include "Campaign Types.h"
	#include "Queen Command.h"
	#include "Strategic Movement.h"
	#include "XML.h"
	#include "expat.h"
	#include "Debug.h"
	#include "Debug Control.h"
	#include "Tactical Save.h"
#endif
#include "Map Screen Interface Map.h"

#ifdef JA2UB
#include "Ja25Update.h"
#include "message.h"
#include "strategicmap.h"
#include "Map Screen Interface Map.h"
#include "Strategic Movement.h"
//#include "Summary Info.h"
#include "ub_config.h"
#endif

extern BOOLEAN sBadSectorsList[ WORLD_MAP_X ][ WORLD_MAP_X ];

#ifdef JA2UB
extern void UpdateCustomMapMovementCosts(); // ja25 UB
extern void MakeBadSectorListFromMapsOnHardDrive( BOOLEAN fDisplayMessages ); // ja25 UB
extern void AddCustomMap( INT32 iRow, INT32 iCol, BOOLEAN fDisplayMessages, BOOLEAN fMessageIfNotExist ); //ja25 UB
extern UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ );
#endif

typedef enum
{
	SMCTABLE_ELEMENT_NONE = 0,
	SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS,
	SMCTABLE_ELEMENT_SECTORTRAVERSAL,
	SMCTABLE_ELEMENT_SECTOR,
	SMCTABLE_ELEMENT_TRAVELRATING,
	SMCTABLE_ELEMENT_TRAVELTYPE,
	SMCTABLE_ELEMENT_SHORTNAME,
	SMCTABLE_ELEMENT_BADSECTOR,
} SMCTABLE_PARSE_STAGE;

typedef struct
{
	SMCTABLE_PARSE_STAGE	curElement;
	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT32					currentDepth;
	UINT32					maxReadDepth;

	//CHAR8					szCustomShortname[MAX_SECTORNAME_LENGTH+1];
	UINT32					uiRowNumber;
	UINT32					uiColNumber;
	UINT32					travRating;
	UINT32					travNorth;
	UINT32					travEast;
	UINT32					travSouth;
	UINT32					travWest;
	UINT32					travHere;

} smctableParseData;

/** Process the opening tag in this expat callback.
 */
static void XMLCALL
smctableStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	smctableParseData * pData = (smctableParseData *) userData;
	//FILE *outfile = fopen("smc.log", "at");

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "StrategicMovementCosts") == 0 && pData->curElement == SMCTABLE_ELEMENT_NONE)
		{
			// Enter
			pData->curElement = SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Strategic Movement Costs\n");
		}
		else if(strcmp(name, "SectorTraversal") == 0 && pData->curElement == SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS)
		{
			pData->curElement = SMCTABLE_ELEMENT_SECTORTRAVERSAL;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Exiting Sector Traversal\n");
		}
		else if(strcmp(name, "Sector") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTORTRAVERSAL)
		{
			UINT32 uiAttrIndex;
			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
			// Extract the x and y attributes to use as the sector co-ordinate
			for(uiAttrIndex = 0;atts[uiAttrIndex] != NULL;uiAttrIndex += 2)
			{
				if(strcmp(atts[uiAttrIndex], "y") == 0)
				{
					if ( isalpha(atts[uiAttrIndex+1][0]) )
					{
						pData->uiRowNumber = (UINT32)(atts[uiAttrIndex+1][0] & 0x1F);
					}
					else
					{
						pData->uiRowNumber = atol(atts[uiAttrIndex+1]);
					}
				}
				else if(strcmp(atts[uiAttrIndex], "x") == 0)
				{
					pData->uiColNumber = atol(atts[uiAttrIndex+1]);
				}
			}
			//fprintf(outfile, "Extracted sector co-ordinates: [x=%d, y=%d]\n", pData->uiColNumber, pData->uiRowNumber);
			// A simple test to try and stop bogus entries
			if(pData->uiRowNumber <= 16 && pData->uiColNumber <= 16)
			{
				pData->maxReadDepth++; //we are not skipping this element
				//fprintf (outfile, "Entering Sector\n");
			}
		}
		else if(strcmp(name, "TravelRating") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_TRAVELRATING;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Traversal Rating\n");
		}
		else if((strcmp(name, "North") == 0 || strcmp(name, "East") == 0 || strcmp(name, "South") == 0 || strcmp(name, "West") == 0 || strcmp(name, "Here") == 0)
				&& pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_TRAVELTYPE;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Traversal Type\n");
		}
		else if(strcmp(name, "Shortname") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_SHORTNAME;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Traversal Rating\n");
		}
		else if(strcmp(name, "BadSector") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_BADSECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}
	pData->currentDepth++;
	//fclose(outfile);
}

/** Process any text content in this callback.
 */
static void XMLCALL
smctableCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	smctableParseData * pData = (smctableParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}

/** Process the closing tag in this expat callback.
 */
static void XMLCALL
smctableEndElementHandle(void *userData, const XML_Char *name)
{
	smctableParseData * pData = (smctableParseData *) userData;

	//FILE *outfile = fopen("C:/JA2/debug.log", "a");
	//fprintf (outfile, "TOWN INDEX: %d\n", TOWN);
	//fprintf (outfile, "Encountered Close Tag: %s\n", name);

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "StrategicMovementCosts") == 0 && pData->curElement == SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS)
		{
			//fprintf (outfile, "Exiting Strategic Movement Costs\n");
			pData->curElement = SMCTABLE_ELEMENT_NONE;
		}
		else if(strcmp(name, "SectorTraversal") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTORTRAVERSAL)
		{
			//fprintf (outfile, "Exiting Sector Traversal\n");
			pData->curElement = SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS;
		}
		else if(strcmp(name, "Sector") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			// We've read in a complete section. Process the contents of pData into the
			// SectionInfo array
			SECTORINFO *pSector;
			int sectorID = ((pData->uiRowNumber - 1) * 16) + (pData->uiColNumber - 1);
			pSector = &SectorInfo[ sectorID ];
			pSector->ubTravelRating = pData->travRating;
			pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ] = pData->travNorth;
			pSector->ubTraversability[ EAST_STRATEGIC_MOVE ] = pData->travEast;
			pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ] = pData->travSouth;
			pSector->ubTraversability[ WEST_STRATEGIC_MOVE ] = pData->travWest;
			pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = pData->travHere;
			//strncpy(pSector->szCustomShortname, pData->szCustomShortname, MAX_SECTORNAME_LENGTH);
			//fprintf (outfile, "Added SectorInfo #%d: y %d, x %d, name: %s, tr %d, n %d, e %d, s %d, w %d, h %d\n",
			//	sec_index, pData->uiRowNumber, pData->uiColNumber, pData->szCustomShortname,
			//	pSector->ubTravelRating, pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ],
			//	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ],
			//	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ],
			//	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ],
			//	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ]);

			// And finish the sector.
			//fprintf (outfile, "Exiting Sector\n");
			//pData->szCustomShortname[0] = '\0';
			pData->curElement = SMCTABLE_ELEMENT_SECTORTRAVERSAL;
		}
		else if(strcmp(name, "TravelRating") == 0 && pData->curElement == SMCTABLE_ELEMENT_TRAVELRATING)
		{
			// Convert the travel rating to an integer
			pData->travRating = atol(pData->szCharData);
			// Done, done, and I'm on to the next one.
			//fprintf (outfile, "Extracted traversal rating: %d\n", pData->travRating);
			//fprintf (outfile, "Exiting Traversal Rating\n");
			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		}
		else if(pData->curElement == SMCTABLE_ELEMENT_TRAVELTYPE)
		{
			// Determine the traversal type from the string in pData
			UINT32 trav_type;
			//fprintf (outfile, "Extracted terrain type: %s\n", pData->szCharData);
			if(strcmp(pData->szCharData, "TOWN") == 0)
			{
				trav_type = TOWN;
			}
			else if(strcmp(pData->szCharData, "ROAD") == 0)
			{
				trav_type = ROAD;
			}
			else if(strcmp(pData->szCharData, "PLAINS") == 0)
			{
				trav_type = PLAINS;
			}
			else if(strcmp(pData->szCharData, "SAND") == 0)
			{
				trav_type = SAND;
			}
			else if(strcmp(pData->szCharData, "SPARSE") == 0)
			{
				trav_type = SPARSE;
			}
			else if(strcmp(pData->szCharData, "DENSE") == 0)
			{
				trav_type = DENSE;
			}
			else if(strcmp(pData->szCharData, "SWAMP") == 0)
			{
				trav_type = SWAMP;
			}
			else if(strcmp(pData->szCharData, "WATER") == 0)
			{
				trav_type = WATER;
			}
			else if(strcmp(pData->szCharData, "HILLS") == 0)
			{
				trav_type = HILLS;
			}
			else if(strcmp(pData->szCharData, "GROUNDBARRIER") == 0)
			{
				trav_type = GROUNDBARRIER;
			}
			else if(strcmp(pData->szCharData, "NS_RIVER") == 0)
			{
				trav_type = NS_RIVER;
			}
			else if(strcmp(pData->szCharData, "EW_RIVER") == 0)
			{
				trav_type = EW_RIVER;
			}
			else if(strcmp(pData->szCharData, "EDGEOFWORLD") == 0)
			{
				trav_type = EDGEOFWORLD;
			}
			else if(strcmp(pData->szCharData, "TROPICS") == 0)
			{
				trav_type = TROPICS;
			}
			else if(strcmp(pData->szCharData, "FARMLAND") == 0)
			{
				trav_type = FARMLAND;
			}
			else if(strcmp(pData->szCharData, "PLAINS_ROAD") == 0)
			{
				trav_type = PLAINS_ROAD;
			}
			else if(strcmp(pData->szCharData, "SPARSE_ROAD") == 0)
			{
				trav_type = SPARSE;
			}
			else if(strcmp(pData->szCharData, "FARMLAND_ROAD") == 0)
			{
				trav_type = FARMLAND_ROAD;
			}
			else if(strcmp(pData->szCharData, "TROPICS_ROAD") == 0)
			{
				trav_type = TROPICS_ROAD;
			}
			else if(strcmp(pData->szCharData, "DENSE_ROAD") == 0)
			{
				trav_type = DENSE_ROAD;
			}
			else if(strcmp(pData->szCharData, "COASTAL") == 0)
			{
				trav_type = COASTAL;
			}
			else if(strcmp(pData->szCharData, "HILLS_ROAD") == 0)
			{
				trav_type = HILLS_ROAD;
			}
			else if(strcmp(pData->szCharData, "COASTAL_ROAD") == 0)
			{
				trav_type = COASTAL_ROAD;
			}
			else if(strcmp(pData->szCharData, "SAND_ROAD") == 0)
			{
				trav_type = SAND_ROAD;
			}
			else if(strcmp(pData->szCharData, "SWAMP_ROAD") == 0)
			{
				trav_type = SWAMP_ROAD;
			}
			else
			{
				// silversurfer: don't just assign type GROUNDBARRIER. Let modders know that traversal data is incorrect!
				AssertMsg(0, String( "Incorrect traversal data '%s' for sector row=%d column=%d in MovementCosts.xml!", pData->szCharData, pData->uiRowNumber, pData->uiColNumber) );
				//trav_type = GROUNDBARRIER;
			}
			// Now assign it to the correct directory using the close tag as a guide
			if(strcmp(name, "North") == 0)
			{
				pData->travNorth = trav_type;
			}
			else if(strcmp(name, "East") == 0)
			{
				pData->travEast = trav_type;
			}
			else if(strcmp(name, "South") == 0)
			{
				pData->travSouth = trav_type;
			}
			else if(strcmp(name, "West") == 0)
			{
				pData->travWest = trav_type;
			}
			else if(strcmp(name, "Here") == 0)
			{
				pData->travHere = trav_type;
			}
			else
			{
				// Bogus Traversal Reference
				Assert(FALSE);
			}
			//fprintf (outfile, "Extracted %s traversal: %d\n", name, trav_type);
			//fprintf (outfile, "Exiting Traversal Type\n");
			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		}
		else if(pData->curElement == SMCTABLE_ELEMENT_BADSECTOR)
		{
			bool fBadSector = atoi(pData->szCharData) > 0;
			sBadSectorsList[pData->uiColNumber][pData->uiRowNumber] = fBadSector;

			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		}
		//else if(strcmp(name, "Shortname") == 0 && pData->curElement == SMCTABLE_ELEMENT_SHORTNAME)
		//{
		//	// Copy this string into the pTownNames array
		//	//fprintf(outfile, "Element data: %s\n", pData->szCharData);
		//	strncpy(pData->szCustomShortname, pData->szCharData, MAX_SECTORNAME_LENGTH);
		//	//fprintf(outfile, "Shortname extracted: %s\n", pData->szCustomShortname);
		//	pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		//	//fprintf(outfile, "Exiting Shortname\n");
		//}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;

	//fclose (outfile);
}


BOOLEAN ReadInStrategicMovementCosts(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	smctableParseData pData;

	// Open weapons file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
	{
		return( FALSE );
	}

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

	// Setup expat callbacks
	XML_SetElementHandler(parser, smctableStartElementHandle, smctableEndElementHandle);
	XML_SetCharacterDataHandler(parser, smctableCharacterDataHandle);

	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

	if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in StrategicMovementCosts.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);
	return TRUE;
}


BOOLEAN WriteInStrategicMovementCosts(STR fileName)
{
	// Lets output the current Strategic map format using the XML structure I've devised.
	FILE *outfile = fopen(fileName, "wt");

	UINT32 x, y, dir_type, trav_type;
	SECTORINFO *pSector;

	fprintf (outfile, "<StrategicMovementCosts>\n");
	fprintf (outfile, "\t<SectorTraversal>\n");
	for(y = 1;y <= 16;y++)
	{
		for(x = 1;x <= 16; x++)
		{
			pSector = &SectorInfo[ ((y - 1) * 16) + (x - 1) ];
			fprintf (outfile, "\t\t<Sector y=\"%c\" x=\"%d\">\n",(y+0x40),x);
			//if(pSector->szCustomShortname != NULL)
			//{
			//	fprintf (outfile, "\t\t\t<Shortname>%s</Shortname>\n",pSector->szCustomShortname);
			//}
			fprintf (outfile, "\t\t\t<TravelRating>%d</TravelRating>\n",pSector->ubTravelRating);
			// Direction Types:
			// 0. NORTH
			// 1. EAST
			// 2. SOUTH
			// 3. WEST
			// 4. THROUGH
			for(dir_type = 0; dir_type < 5; dir_type++)
			{
				trav_type = pSector->ubTraversability[ dir_type ];

				// Output the traverse direction as the XML tag
				switch(dir_type)
				{
				case NORTH_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<North>");
					break;
				case EAST_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<East>");
					break;
				case SOUTH_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<South>");
					break;
				case WEST_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<West>");
					break;
				default:
					fprintf (outfile, "\t\t\t<Here>");
				}
				// Output the traversal type
				switch(trav_type)
				{
				case TOWN:
					fprintf (outfile, "TOWN");
					break;
				case ROAD:
					fprintf (outfile, "ROAD");
					break;
				case PLAINS:
					fprintf (outfile, "PLAINS");
					break;
				case SAND:
					fprintf (outfile, "SAND");
					break;
				case SPARSE:
					fprintf (outfile, "SPARSE");
					break;
				case DENSE:
					fprintf (outfile, "DENSE");
					break;
				case SWAMP:
					fprintf (outfile, "SWAMP");
					break;
				case WATER:
					fprintf (outfile, "WATER");
					break;
				case HILLS:
					fprintf (outfile, "HILLS");
					break;
				case GROUNDBARRIER:
					fprintf (outfile, "GROUNDBARRIER");
					break;
				case NS_RIVER:
					fprintf (outfile, "NS_RIVER");
					break;
				case EW_RIVER:
					fprintf (outfile, "EW_RIVER");
					break;
				case EDGEOFWORLD:
					fprintf (outfile, "EDGEOFWORLD");
					break;
				case TROPICS:
					fprintf (outfile, "TROPICS");
					break;
				case FARMLAND:
					fprintf (outfile, "FARMLAND");
					break;
				case PLAINS_ROAD:
					fprintf (outfile, "PLAINS_ROAD");
					break;
				case SPARSE_ROAD:
					fprintf (outfile, "SPARSE_ROAD");
					break;
				case FARMLAND_ROAD:
					fprintf (outfile, "FARMLAND_ROAD");
					break;
				case TROPICS_ROAD:
					fprintf (outfile, "TROPICS_ROAD");
					break;
				case DENSE_ROAD:
					fprintf (outfile, "DENSE_ROAD");
					break;
				case COASTAL:
					fprintf (outfile, "COASTAL");
					break;
				case HILLS_ROAD:
					fprintf (outfile, "HILLS_ROAD");
					break;
				case COASTAL_ROAD:
					fprintf (outfile, "COASTAL_ROAD");
					break;
				case SAND_ROAD:
					fprintf (outfile, "SAND_ROAD");
					break;
				case SWAMP_ROAD:
					fprintf (outfile, "SWAMP_ROAD");
					break;
				default:
					fprintf (outfile, "%d", trav_type);
				}
				// Output the traverse direction as the closing tag
				switch(dir_type)
				{
				case NORTH_STRATEGIC_MOVE:
					fprintf (outfile, "</North>\n");
					break;
				case EAST_STRATEGIC_MOVE:
					fprintf (outfile, "</East>\n");
					break;
				case SOUTH_STRATEGIC_MOVE:
					fprintf (outfile, "</South>\n");
					break;
				case WEST_STRATEGIC_MOVE:
					fprintf (outfile, "</West>\n");
					break;
				default:
					fprintf (outfile, "</Here>\n");
				}
			}
			fprintf (outfile, "\t\t</Sector>\n");
			}
	}
	fprintf (outfile, "\t</SectorTraversal>\n");
	fprintf (outfile, "</StrategicMovementCosts>\n");

	fclose(outfile);

	return (TRUE);
}

//void InitStrategicMovementCosts()
//{
//	// Init Traversal Data
//
//	// Testing Externalization
//	//#ifdef JA2TESTVERSION
//	//FILE *outfile = fopen("C:/JA2/debug.log", "a");
//	{ //Simply make sure all shared borders between sectors match.
//		INT32 x,y;
//		OutputDebugString( "STRATEGIC MOVE COSTS:	Testing validity of data (okay if no following debug msgs)...\n");
//		for( y = 1; y <= 16; y++ ) for( x = 1; x <= 16; x++ )
//		{
//			if( y > 1 )
//			{
//				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] !=
//					SectorInfo[ SECTOR(x,y-1) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] )
//				{	//north
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	North mismatch for sector %c%d\n", y+'A'-1, x ) );
//					//fprintf(outfile, "STRATEGIC MOVE COSTS:	North mismatch for sector %c%d\n", y+'A'-1, x);
//				}
//				if( y == 16 && SectorInfo[ SECTOR(x,y) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] != EDGEOFWORLD )
//				{
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	South should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
//					//fprintf(outfile, "STRATEGIC MOVE COSTS:	South should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x );
//				}
//			}
//			else
//			{
//				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] !=
//				 SectorInfo[ SECTOR(x,y+1) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] )
//				{ //south
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	South mismatch for sector %c%d\n", y+'A'-1, x ) );
//					//fprintf(outfile, "STRATEGIC MOVE COSTS:	South mismatch for sector %c%d\n", y+'A'-1, x);
//				}
//				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] != EDGEOFWORLD )
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	North should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
//			}
//			if( x < 16 )
//			{
//				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ EAST_STRATEGIC_MOVE ] !=
//					SectorInfo[ SECTOR(x+1,y) ].ubTraversability[ WEST_STRATEGIC_MOVE ] )
//				{ //east
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	East mismatch for sector %c%d\n", y+'A'-1, x ) );
//					//fprintf(outfile, "STRATEGIC MOVE COSTS:	East mismatch for sector %c%d\n", y+'A'-1, x);
//				}
//				if( x == 1 && SectorInfo[ SECTOR(x,y) ].ubTraversability[ WEST_STRATEGIC_MOVE ] != EDGEOFWORLD )
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	West should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
//			}
//			else
//			{
//				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ WEST_STRATEGIC_MOVE ] !=
//				 SectorInfo[ SECTOR(x-1,y) ].ubTraversability[ EAST_STRATEGIC_MOVE ] )
//				{ //west
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	West mismatch for sector %c%d\n", y+'A'-1, x ) );
//					//fprintf(outfile, "STRATEGIC MOVE COSTS:	West mismatch for sector %c%d\n", y+'A'-1, x);
//				}
//				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ EAST_STRATEGIC_MOVE ] != EDGEOFWORLD )
//					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	East should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
//			}
//		}
//	}
//	//fclose(outfile);
//	//#endif
//}
//
UINT8 GetTraversability( INT16 sStartSector, INT16 sEndSector )
{
	UINT8 ubDirection = 0;
	INT16 sDifference = 0;

	// given start and end sectors
	sDifference = sEndSector - sStartSector;


	if( sDifference == -1 )
	{
		ubDirection = WEST_STRATEGIC_MOVE;
	}
	else if( sDifference == 1 )
	{
		ubDirection = EAST_STRATEGIC_MOVE;
	}
	else if( sDifference == 16 )
	{
		ubDirection = SOUTH_STRATEGIC_MOVE;
	}
	else
	{
		ubDirection = NORTH_STRATEGIC_MOVE;
	}


	return( SectorInfo[ sStartSector ].ubTraversability[ ubDirection ] );
}

BOOLEAN SectorIsImpassable( INT16 sSector, UINT8 aDirection )
{
	// returns true if the sector is impassable in direction
	return( SectorInfo[ sSector ].ubTraversability[aDirection] == GROUNDBARRIER ||
		SectorInfo[ sSector ].ubTraversability[aDirection] == EDGEOFWORLD );
}

UINT8 gubEncryptionArray3[ BASE_NUMBER_OF_ROTATION_ARRAYS * 3 ][ NEW_ROTATION_ARRAY_SIZE ] =
{
	{
	250,224,3,197,156,209,110,
	159,75,119,221,42,212,180,
	223,115,13,246,173,221,211,
	148,3,78,214,195,102,155,
	5,128,5,204,42,72,240,
	65,177,242,226,81,255,139,
	70,150,95,124,203,83,248
	},

	{
	234,33,49,205,144,43,212,
	44,249,86,116,150,112,80,
	244,150,120,207,182,110,50,
	179,160,41,114,31,130,253,
	243,221,106,120,118,181,252,
	103,30,238,119,10,242,187,
	99,99,210,197,153,71,176
	},

	{
	137,180,252,121,200,124,8,
	111,186,110,245,102,71,247,
	195,157,232,115,191,169,136,
	138,98,54,253,14,34,248,
	106,226,167,185,48,19,112,
	183,175,155,66,76,150,34,
	114,38,225,8,126,236,96
	},

	{
	16,220,169,218,40,146,208,
	171,96,114,57,235,189,141,
	227,252,238,194,231,160,128,
	231,91,85,175,137,143,46,
	106,176,119,234,149,173,154,
	114,52,93,90,126,142,222,
	243,157,223,56,7,82,175
	},

	{
	214,11,122,112,113,118,195,
	111,55,4,85,186,203,217,
	125,14,7,2,128,91,236,
	239,85,23,213,142,125,198,
	74,130,186,4,118,41,195,
	123,188,1,212,1,94,239,
	40,89,169,57,55,203,169
	},

	{
	107,224,17,213,57,56,188,
	177,120,49,183,211,64,230,
	226,84,84,171,122,18,226,
	165,77,205,198,31,112,139,
	65,93,107,58,110,22,144,
	19,97,87,140,177,42,4,
	192,72,174,177,138,11,166
	},

	{
	9,11,33,144,120,134,56,
	4,91,241,26,37,93,204,
	71,167,75,221,9,34,10,
	219,253,35,235,183,134,199,
	28,130,28,63,91,151,1,
	3,187,36,59,41,166,150,
	104,162,205,29,72,186,83
	},

	{
	196,130,182,208,71,167,231,
	133,59,210,10,82,151,243,
	72,199,77,48,187,1,229,
	90,194,112,224,238,252,108,
	206,154,86,168,215,178,231,
	84,179,166,10,22,174,219,
	127,163,226,226,243,31,38
	},

	{
	151,198,6,214,2,99,210,
	26,142,255,31,154,133,47,
	111,116,53,2,99,76,220,
	32,138,65,181,182,139,192,
	37,121,215,223,133,181,173,
	70,135,166,142,83,146,1,
	243,133,54,3,113,189,13
	},

	{
	218,245,135,17,201,119,232,
	222,199,170,217,219,28,16,
	3,178,162,3,204,38,47,
	12,104,170,218,33,215,196,
	149,34,158,166,210,45,34,
	243,172,26,99,60,84,204,
	28,18,35,143,222,62,46
	},

	{
	60,75,3,168,92,248,239,
	242,4,39,47,181,156,203,
	212,206,79,31,30,121,87,
	53,27,131,225,189,185,224,
	197,139,173,133,179,233,43,
	197,57,111,229,53,35,75,
	91,56,162,191,210,60,204
	},

	{
	204,16,230,187,172,49,5,
	6,62,173,174,199,231,242,
	88,238,27,145,67,3,252,
	116,22,44,104,24,248,161,
	191,68,19,63,190,51,179,
	124,223,155,19,121,99,175,
	236,86,157,100,225,151,149
	},

	{
	20,225,193,156,236,144,244,
	233,27,222,169,213,53,207,
	99,209,213,167,118,171,224,
	107,166,60,107,5,215,26,
	193,227,130,90,118,110,40,
	15,9,41,122,128,4,213,
	119,214,25,121,36,43,50
	},

	{
	145,47,181,236,88,31,32,
	115,104,90,150,49,168,172,
	179,101,188,142,221,234,236,
	228,41,88,211,109,94,201,
	158,144,56,104,73,210,109,
	23,168,157,173,64,144,150,
	18,68,3,56,48,116,165
	},

	{
	244,90,27,112,128,36,134,
	214,150,207,139,84,223,171,
	128,173,54,7,27,180,4,
	201,54,253,233,84,240,76,
	115,170,33,14,5,159,140,
	205,195,253,229,225,165,86,
	11,58,114,131,107,165,215
	},

	{
	127,30,93,91,165,158,58,
	91,236,151,103,207,65,207,
	224,16,142,150,170,76,137,
	179,3,245,230,90,117,207,
	4,1,32,217,158,175,10,
	214,182,171,214,154,51,253,
	189,234,95,204,17,14,207
	},

	{
	251,51,223,223,24,80,138,
	60,244,179,168,186,1,21,
	12,239,194,171,206,186,121,
	108,254,72,86,66,135,179,
	75,154,160,214,228,28,109,
	100,31,230,13,217,190,45,
	212,123,22,131,225,202,182
	},

	{
	185,198,186,9,155,133,18,
	53,111,146,55,105,127,17,
	220,228,159,10,193,193,233,
	209,13,3,157,84,98,206,
	113,120,76,80,52,103,3,
	69,15,214,66,155,70,31,
	44,43,203,79,226,242,132
	},

	{
	243,234,219,137,211,230,117,
	77,78,213,164,239,148,89,
	188,164,131,43,255,119,66,
	78,239,81,106,25,124,145,
	243,179,114,20,144,27,54,
	248,181,69,49,9,19,129,
	246,21,163,160,145,26,21
	},

	{
	19,244,140,188,119,3,162,
	214,207,50,237,66,223,44,
	37,110,211,126,117,193,202,
	185,39,26,89,15,255,186,
	152,204,45,61,223,196,18,
	230,196,12,213,241,104,9,
	2,33,192,82,18,67,223
	},

	{
	74,68,234,227,249,134,5,
	155,29,216,149,124,210,253,
	70,1,251,206,7,6,169,
	11,110,69,164,249,34,121,
	124,192,237,83,24,179,204,
	195,70,140,154,203,57,204,
	154,84,113,52,162,44,11
	},

	{
	149,12,210,227,237,40,13,
	145,9,125,242,172,155,114,
	134,79,24,170,101,90,40,
	201,183,100,21,213,235,222,
	1,235,97,78,63,140,139,
	41,175,36,176,69,106,21,
	222,78,151,1,31,62,206
	},

	{
	111,142,87,207,172,114,135,
	240,251,218,183,28,227,230,
	7,172,200,86,82,11,141,
	106,27,97,114,183,48,49,
	236,5,27,61,172,200,203,
	128,129,90,113,165,107,124,
	2,196,116,74,95,198,166
	},

	{
	36,157,67,183,185,88,56,
	196,189,140,108,182,108,4,
	207,158,104,168,192,176,19,
	219,132,39,248,42,196,176,
	100,106,126,180,172,179,32,
	32,102,40,67,229,250,6,
	212,3,207,255,251,39,137
	},

	{
	75,159,202,137,103,226,221,
	61,6,107,208,82,34,206,
	43,111,163,245,105,131,160,
	221,86,66,164,127,159,241,
	252,63,209,15,117,177,134,
	241,155,33,226,253,211,145,
	55,122,105,182,231,179,227
	},

	{
	157,96,103,188,105,64,44,
	218,9,130,220,208,31,209,
	165,84,23,196,202,232,165,
	52,185,56,150,110,141,11,
	65,114,137,84,121,247,180,
	97,83,114,27,129,147,201,
	227,59,40,2,192,121,117
	},

	{
	141,213,168,224,119,181,65,
	98,40,127,183,126,248,200,
	61,116,77,83,91,13,104,
	56,217,205,187,161,226,238,
	229,156,224,248,17,35,26,
	72,247,255,100,102,62,145,
	12,135,83,17,77,255,163
	},

	{
	114,95,19,65,117,142,233,
	198,248,84,19,166,59,238,
	91,165,4,102,92,171,109,
	125,153,177,72,137,125,255,
	201,156,23,103,141,9,230,
	198,139,174,164,127,20,8,
	55,25,105,110,215,204,24
	},

	{
	158,164,46,157,212,125,174,
	116,154,138,38,34,169,58,
	43,99,220,22,105,253,182,
	66,163,101,91,9,182,186,
	147,53,45,66,185,174,198,
	244,21,25,133,42,145,223,
	147,19,91,117,172,252,72
	},

	{
	49,66,21,133,143,27,168,
	148,62,162,138,247,194,151,
	175,153,19,96,160,84,252,
	176,202,168,181,193,91,4,
	91,206,171,158,213,18,227,
	101,224,241,223,225,148,168,
	252,160,86,4,213,6,111
	},

	{
	144,151,17,65,208,251,3,
	77,204,130,87,4,157,7,
	28,165,66,66,8,17,95,
	85,91,208,59,252,247,77,
	146,111,174,109,148,149,48,
	134,177,171,170,239,125,216,
	120,18,77,240,230,76,226
	},

	{
	210,134,132,192,156,253,190,
	117,63,210,141,138,131,45,
	185,81,35,254,244,69,17,
	145,239,66,118,235,177,58,
	145,10,125,173,254,99,41,
	155,144,176,54,26,63,107,
	135,92,92,2,13,83,139
	},

	{
	51,60,163,170,147,164,49,
	58,161,146,230,89,121,242,
	4,248,134,113,158,82,65,
	18,148,65,101,47,159,144,
	148,39,206,229,233,148,16,
	64,113,112,11,203,242,240,
	255,1,19,113,237,186,66
	},

	{
	89,159,78,103,56,246,78,
	204,4,21,252,53,204,162,
	14,168,189,244,222,214,188,
	53,154,156,141,90,137,154,
	195,28,5,79,102,155,54,
	192,149,251,61,20,11,162,
	196,30,206,82,172,93,1
	},

	{
	226,222,85,249,190,223,200,
	178,240,60,187,187,232,97,
	207,164,185,5,211,32,8,
	168,23,210,90,85,110,5,
	12,44,92,46,148,220,104,
	161,95,153,5,51,231,168,
	13,54,84,34,77,166,72
	},

	{
	252,15,213,37,242,26,114,
	115,99,46,77,163,196,100,
	157,235,193,113,53,117,144,
	72,105,138,167,8,22,7,
	97,184,138,186,169,200,185,
	7,73,199,135,77,234,79,
	143,149,114,153,47,242,186
	},

	{
	187,60,9,83,243,54,78,
	90,20,70,81,255,107,243,
	177,221,63,217,7,159,51,
	56,113,50,168,185,8,252,
	138,74,218,63,120,74,198,
	59,206,5,205,40,123,185,
	46,167,40,14,241,178,153
	},

	{
	75,41,175,215,50,141,196,
	250,196,198,238,44,224,253,
	14,195,247,8,102,7,200,
	205,196,115,107,61,202,22,
	142,105,139,229,44,24,255,
	154,171,123,119,239,174,72,
	160,219,106,222,45,158,228
	},

	{
	201,188,54,248,57,37,25,
	96,199,162,200,176,46,20,
	27,160,39,217,196,100,58,
	103,23,127,168,47,95,229,
	39,234,244,187,179,238,89,
	154,37,140,111,160,190,49,
	56,56,126,62,22,213,80
	},

	{
	81,12,160,241,248,231,70,
	171,127,226,220,168,223,151,
	45,22,115,217,54,204,131,
	100,66,186,63,198,114,191,
	69,158,2,56,67,137,48,
	242,216,196,25,192,64,253,
	95,93,232,65,242,229,139
	},

	{
	134,221,148,217,202,95,252,
	95,61,51,127,170,99,97,
	40,82,194,103,179,250,244,
	25,250,229,172,5,102,45,
	149,205,194,61,150,45,7,
	167,96,27,110,234,204,213,
	117,58,248,57,20,234,161
	},

	{
	38,213,157,169,107,23,175,
	84,238,15,28,30,134,243,
	88,168,69,218,79,201,159,
	159,4,16,64,125,5,223,
	214,149,64,121,210,33,68,
	249,64,123,162,195,195,200,
	107,77,238,103,118,198,207
	},

	{
	232,120,145,34,201,147,8,
	220,158,104,126,144,240,77,
	8,89,132,187,230,206,52,
	139,46,181,45,26,125,223,
	181,244,93,1,55,20,46,
	220,205,75,29,161,7,5,
	34,193,17,215,109,50,25
	},

	{
	25,89,86,245,1,51,123,
	253,111,240,58,28,252,69,
	144,241,90,250,19,53,165,
	34,9,11,197,1,207,136,
	105,56,90,29,184,34,29,
	30,96,214,85,38,248,211,
	231,131,125,190,194,106,204
	},

	{
	61,15,48,227,80,24,43,
	221,58,41,146,86,89,88,
	250,64,248,115,177,207,134,
	12,182,142,54,217,120,46,
	111,96,32,51,32,37,151,
	15,72,90,11,200,212,66,
	17,187,46,58,64,154,125
	},

	{
	176,94,60,25,239,233,78,
	19,10,51,143,104,187,179,
	159,185,176,236,250,20,228,
	122,71,189,152,144,122,121,
	149,165,253,58,50,118,92,
	202,216,34,158,78,119,147,
	232,32,175,242,105,5,20
	},

	{
	88,62,37,83,109,101,204,
	176,66,65,101,138,12,229,
	157,97,249,172,65,38,232,
	47,177,45,30,73,118,158,
	209,49,230,186,172,61,84,
	202,3,116,192,24,3,129,
	135,189,122,24,1,172,139
	},

	{
	115,137,193,238,244,237,60,
	4,136,178,113,108,224,44,
	23,96,32,227,245,129,17,
	62,100,83,120,217,93,33,
	161,164,138,122,190,26,26,
	17,48,159,188,27,71,132,
	155,5,167,136,166,149,216
	},

	{
	124,10,86,29,212,50,96,
	40,191,32,87,212,177,122,
	184,100,207,41,78,103,73,
	208,226,235,2,23,9,255,
	153,233,21,34,48,194,23,
	194,249,39,252,94,6,68,
	157,81,56,5,229,1,239
	},

	{
	109,209,104,83,161,130,167,
	172,101,12,168,226,109,80,
	124,120,101,130,117,14,239,
	162,172,222,143,156,249,47,
	182,69,250,40,239,237,75,
	18,96,198,112,106,145,201,
	171,208,196,95,49,54,187
	},

	{
	125,238,86,66,116,112,229,
	80,35,251,120,41,196,128,
	141,64,28,109,190,69,41,
	7,139,44,39,89,183,137,
	4,83,178,29,23,51,255,
	218,62,204,31,93,41,202,
	220,250,247,133,158,120,253
	},

	{
	117,124,147,199,242,198,81,
	46,74,212,97,166,187,160,
	98,132,139,36,127,115,172,
	244,19,206,38,12,210,29,
	201,63,54,94,83,86,145,
	105,132,61,162,21,95,76,
	244,88,13,24,242,35,139
	},

	{
	191,252,45,196,59,89,93,
	15,158,95,25,209,189,162,
	46,60,61,146,124,209,115,
	74,54,193,42,248,209,175,
	155,184,122,14,184,40,48,
	143,46,158,66,212,21,89,
	120,234,207,110,136,175,12
	},

	{
	239,25,187,91,249,22,224,
	99,40,115,213,19,41,56,
	53,221,222,229,82,112,215,
	23,12,215,126,112,44,146,
	209,173,116,133,9,253,233,
	75,235,96,117,211,69,72,
	120,209,63,49,107,230,5
	},

	{
	98,24,14,131,155,143,55,
	150,221,114,139,140,10,153,
	84,73,144,203,3,226,232,
	129,64,28,254,91,143,128,
	99,100,112,138,96,179,122,
	168,183,133,108,113,69,98,
	167,230,45,116,11,32,225
	},

	{
	153,169,41,171,77,85,127,
	241,6,111,247,245,26,2,
	97,66,194,143,211,123,90,
	150,228,211,108,60,176,209,
	165,35,7,167,82,207,143,
	205,104,166,75,33,202,249,
	58,54,206,10,136,19,166
	},

	{
	146,29,30,194,190,208,94,
	195,8,67,217,18,255,127,
	64,188,106,114,153,172,177,
	98,54,195,32,99,153,14,
	221,5,133,140,46,33,255,
	187,212,29,98,102,47,125,
	80,232,235,19,180,106,219
	},
};

/*
void InitStrategicRowA()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_A1 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS;

	pSector = &SectorInfo[ SEC_A2 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_A3 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_A4 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A5 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A6 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_A7 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_A8 ];
	pSector->ubTravelRating = 14;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_A9 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_A10 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_A11 ];
	pSector->ubTravelRating = 18;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_A12 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_A13 ];
	pSector->ubTravelRating = 14;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_A14 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_A15 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_A16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowB()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_B1 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS;

	pSector = &SectorInfo[ SEC_B2 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_B3 ];
	pSector->ubTravelRating = 6;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_B4 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_B5 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_B6 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_B7 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_B8 ];
	pSector->ubTravelRating = 20;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_B9 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_B10 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_B11 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_B12 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND_ROAD;

	pSector = &SectorInfo[ SEC_B13 ];
	pSector->ubTravelRating = 85;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_B14 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = WATER;

	pSector = &SectorInfo[ SEC_B15 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_B16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;
}

void InitStrategicRowC()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_C1 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS;

	pSector = &SectorInfo[ SEC_C2 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS_ROAD;

	pSector = &SectorInfo[ SEC_C3 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_C4 ];
	pSector->ubTravelRating = 20;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_C5 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_C6 ];
	pSector->ubTravelRating = 75;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_C7 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_C8 ];
	pSector->ubTravelRating = 48;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_C9 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_C10 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_C11 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_C12 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_C13 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_C14 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = WATER;

	pSector = &SectorInfo[ SEC_C15 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_C16 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;
}

void InitStrategicRowD()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_D1 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_D2 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;//TROPICS_SAMSITE;

	pSector = &SectorInfo[ SEC_D3 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_D4 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_D5 ];
	pSector->ubTravelRating = 49;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_D6 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_D7 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE_ROAD;

	pSector = &SectorInfo[ SEC_D8 ];
	pSector->ubTravelRating = 16;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_D9 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_D10 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_D11 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_D12 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_D13 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_D14 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = WATER;

	pSector = &SectorInfo[ SEC_D15 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_D16 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;
}

void InitStrategicRowE()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_E1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_E2 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS;

	pSector = &SectorInfo[ SEC_E3 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_E4 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_E5 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_E6 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_E7 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND_ROAD;

	pSector = &SectorInfo[ SEC_E8 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_E9 ];
	pSector->ubTravelRating = 56;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND_ROAD;

	pSector = &SectorInfo[ SEC_E10 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_E11 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_E12 ];
	pSector->ubTravelRating = 35;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_E13 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = WATER;

	pSector = &SectorInfo[ SEC_E14 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_E15 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE_ROAD;

	pSector = &SectorInfo[ SEC_E16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowF()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_F1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_F2 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= NS_RIVER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL;

	pSector = &SectorInfo[ SEC_F3 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= NS_RIVER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_F4 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_F5 ];
	pSector->ubTravelRating = 6;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_F6 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_F7 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS_ROAD;

	pSector = &SectorInfo[ SEC_F8 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_F9 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_F10 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_F11 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_F12 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_F13 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_F14 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_F15 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE_ROAD;

	pSector = &SectorInfo[ SEC_F16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowG()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_G1 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_G2 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_G3 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_G4 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS_ROAD;

	pSector = &SectorInfo[ SEC_G5 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS_ROAD;

	pSector = &SectorInfo[ SEC_G6 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS_ROAD;

	pSector = &SectorInfo[ SEC_G7 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND_ROAD;

	pSector = &SectorInfo[ SEC_G8 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_G9 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_G10 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND_ROAD;

	pSector = &SectorInfo[ SEC_G11 ];
	pSector->ubTravelRating = 25;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND_ROAD;

	pSector = &SectorInfo[ SEC_G12 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_G13 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_G14 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_G15 ];
	pSector->ubTravelRating = 16;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_G16 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;
}

void InitStrategicRowH()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_H1 ];
	pSector->ubTravelRating = 67;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_H2 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_H3 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_H4 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_H5 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS;

	pSector = &SectorInfo[ SEC_H6 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = HILLS_ROAD;

	pSector = &SectorInfo[ SEC_H7 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= HILLS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_H8 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_H9 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_H10 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_H11 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_H12 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_H13 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_H14 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_H15 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_H16 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;
}

void InitStrategicRowI()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_I1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_I2 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_I3 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_I4 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_I5 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_I6 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_I7 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_I8 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_I9 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_I10 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_I11 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_I12 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_I13 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_I14 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_I15 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_I16 ];
	pSector->ubTravelRating = 2;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;
}

void InitStrategicRowJ()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_J1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_J2 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_J3 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_J4 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_J5 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_J6 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_J7 ];
	pSector->ubTravelRating = 6;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_J8 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_J9 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_J10 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_J11 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_J12 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_J13 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_J14 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SPARSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND_ROAD;

	pSector = &SectorInfo[ SEC_J15 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_J16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowK()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_K1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_K2 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_K3 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_K4 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_K5 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_K6 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_K7 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND_ROAD;

	pSector = &SectorInfo[ SEC_K8 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND_ROAD;

	pSector = &SectorInfo[ SEC_K9 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND_ROAD;

	pSector = &SectorInfo[ SEC_K10 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_K11 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_K12 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_K13 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_K14 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_K15 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_K16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowL()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_L1 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL;

	pSector = &SectorInfo[ SEC_L2 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_L3 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_L4 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_L5 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_L6 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS_ROAD;

	pSector = &SectorInfo[ SEC_L7 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_L8 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SAND;

	pSector = &SectorInfo[ SEC_L9 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_L10 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE_ROAD;

	pSector = &SectorInfo[ SEC_L11 ];
	pSector->ubTravelRating = 17;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_L12 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_L13 ];
	pSector->ubTravelRating = 18;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL;

	pSector = &SectorInfo[ SEC_L14 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_L15 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_L16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowM()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_M1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_M2 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_M3 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SWAMP;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP_ROAD;

	pSector = &SectorInfo[ SEC_M4 ];
	pSector->ubTravelRating = 38;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FARMLAND;

	pSector = &SectorInfo[ SEC_M5 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE_ROAD;

	pSector = &SectorInfo[ SEC_M6 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE_ROAD;

	pSector = &SectorInfo[ SEC_M7 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_M8 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= SAND;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_M9 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_M10 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS_ROAD;

	pSector = &SectorInfo[ SEC_M11 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_M12 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_M13 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_M14 ];
	pSector->ubTravelRating = 2;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SWAMP;

	pSector = &SectorInfo[ SEC_M15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowN()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_N1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_N2 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_N3 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_N4 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_N5 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_N6 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS_ROAD;

	pSector = &SectorInfo[ SEC_N7 ];
	pSector->ubTravelRating = 20;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_N8 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = COASTAL_ROAD;

	pSector = &SectorInfo[ SEC_N9 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS_ROAD;

	pSector = &SectorInfo[ SEC_N10 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= ROAD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS_ROAD;

	pSector = &SectorInfo[ SEC_N11 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_N12 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_N13 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N14 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowO()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_O1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O2 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O3 ];
	pSector->ubTravelRating = 90;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_O4 ];
	pSector->ubTravelRating = 90;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_O5 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O6 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O7 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O8 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS;

	pSector = &SectorInfo[ SEC_O9 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TROPICS;

	pSector = &SectorInfo[ SEC_O10 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O11 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O12 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O13 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_O14 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowP()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_P1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;
//	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = WATER; //keep as water so we can teleport to demo maps.

	pSector = &SectorInfo[ SEC_P2 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P3 ];
	pSector->ubTravelRating = 100;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_P4 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P5 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P6 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P7 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P8 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P9 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P10 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P11 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P12 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P13 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_P14 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}
*/
#ifdef JA2UB
void InitStrategicRowA()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_A1 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A2 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A3 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A4 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A5 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A6 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A7 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A8 ];
	pSector->ubTravelRating = 14;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A9 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A10 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A11 ];
	pSector->ubTravelRating = 18;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A12 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A13 ];
	pSector->ubTravelRating = 14;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A14 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A15 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_A16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowB()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_B1 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B2 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B3 ];
	pSector->ubTravelRating = 6;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B4 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B5 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B6 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B7 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B8 ];
	pSector->ubTravelRating = 20;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B9 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B10 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B11 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B12 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B13 ];
	pSector->ubTravelRating = 85;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B14 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B15 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_B16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowC()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_C1 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C2 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C3 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C4 ];
	pSector->ubTravelRating = 20;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C5 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C6 ];
	pSector->ubTravelRating = 75;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C7 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C8 ];
	pSector->ubTravelRating = 48;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C9 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C10 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C11 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C12 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C13 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C14 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C15 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_C16 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowD()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_D1 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D2 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;//EDGEOFWORLD_SAMSITE;

	pSector = &SectorInfo[ SEC_D3 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D4 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D5 ];
	pSector->ubTravelRating = 49;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D6 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D7 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D8 ];
	pSector->ubTravelRating = 16;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D9 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D10 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D11 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D12 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D13 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D14 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_D15 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;	

	pSector = &SectorInfo[ SEC_D16 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowE()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_E1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E2 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E3 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E4 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E5 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E6 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E7 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E8 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E9 ];
	pSector->ubTravelRating = 56;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E10 ];
	pSector->ubTravelRating = 11;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E11 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E12 ];
	pSector->ubTravelRating = 35;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E13 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E14 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E15 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_E16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowF()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_F1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F2 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F3 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F4 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F5 ];
	pSector->ubTravelRating = 6;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F6 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F7 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F8 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F9 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F10 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F11 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F12 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F13 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F14 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F15 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_F16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowG()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_G1 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G2 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G3 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G4 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G5 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G6 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G7 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G8 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G9 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G10 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G11 ];
	pSector->ubTravelRating = 25;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G12 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G13 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G14 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G15 ];
	pSector->ubTravelRating = 16;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_G16 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowH()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_H1 ];
	pSector->ubTravelRating = 67;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H2 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H3 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H4 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H5 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H6 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H7 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = MOUNTAINS_TYPE;

	pSector = &SectorInfo[ SEC_H8 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = MOUNTAINS_TYPE;

	pSector = &SectorInfo[ SEC_H9 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = MOUNTAINS_TYPE;

	pSector = &SectorInfo[ SEC_H10 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= DENSE;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_H11 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;


	pSector = &SectorInfo[ SEC_H12 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H13 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H14 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H15 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_H16 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowI()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_I1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I2 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I3 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I4 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I5 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I6 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I7 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I8 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I9 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;


#ifdef JA2UB_PRESS_PREVIEW

	pSector = &SectorInfo[ SEC_I10 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

#else

	pSector = &SectorInfo[ SEC_I10 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

#endif

	pSector = &SectorInfo[ SEC_I11 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= TOWN;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = TOWN;

	pSector = &SectorInfo[ SEC_I12 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_I13 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;

	pSector = &SectorInfo[ SEC_I14 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I15 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_I16 ];
	pSector->ubTravelRating = 2;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowJ()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_J1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J2 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J3 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J4 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J5 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J6 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J7 ];
	pSector->ubTravelRating = 6;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J8 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J9 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J10 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J11 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = SPARSE;

	pSector = &SectorInfo[ SEC_J12 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = DENSE;

	pSector = &SectorInfo[ SEC_J13 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= PLAINS;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = POWER_PLANT_TYPE;

	pSector = &SectorInfo[ SEC_J14 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_J15 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_J16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowK()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_K1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K2 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K3 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K4 ];
	pSector->ubTravelRating = 45;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K5 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K6 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K7 ];
	pSector->ubTravelRating = 60;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K8 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K9 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K10 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K11 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K12 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_K13 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_K14 ];
	pSector->ubTravelRating = 50;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_K15 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= GROUNDBARRIER;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = FINAL_COMPLEX;

	pSector = &SectorInfo[ SEC_K16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;
}

void InitStrategicRowL()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_L1 ];
	pSector->ubTravelRating = 4;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L2 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L3 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L4 ];
	pSector->ubTravelRating = 15;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L5 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L6 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L7 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L8 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L9 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L10 ];
	pSector->ubTravelRating = 9;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L11 ];
	pSector->ubTravelRating = 17;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L12 ];
	pSector->ubTravelRating = 55;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L13 ];
	pSector->ubTravelRating = 18;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L14 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_L15 ];
	pSector->ubTravelRating = 3;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = GROUNDBARRIER;

	pSector = &SectorInfo[ SEC_L16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowM()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_M1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M2 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M3 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M4 ];
	pSector->ubTravelRating = 38;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M5 ];
	pSector->ubTravelRating = 70;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M6 ];
	pSector->ubTravelRating = 65;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M7 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M8 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M9 ];
	pSector->ubTravelRating = 8;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M10 ];
	pSector->ubTravelRating = 7;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M11 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M12 ];
	pSector->ubTravelRating = 12;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M13 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M14 ];
	pSector->ubTravelRating = 2;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_M16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowN()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_N1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N2 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N3 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N4 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N5 ];
	pSector->ubTravelRating = 80;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N6 ];
	pSector->ubTravelRating = 40;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N7 ];
	pSector->ubTravelRating = 20;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N8 ];
	pSector->ubTravelRating = 10;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N9 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N10 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N11 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N12 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N13 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N14 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_N16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowO()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_O1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O2 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O3 ];
	pSector->ubTravelRating = 90;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O4 ];
	pSector->ubTravelRating = 90;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O5 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O6 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O7 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O8 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O9 ];
	pSector->ubTravelRating = 5;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O10 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O11 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O12 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O13 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O14 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_O16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}

void InitStrategicRowP()
{
	SECTORINFO *pSector;

	pSector = &SectorInfo[ SEC_P1 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
//	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD; //keep as EDGEOFWORLD so we can teleport to demo maps.

	pSector = &SectorInfo[ SEC_P2 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P3 ];
	pSector->ubTravelRating = 100;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P4 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P5 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P6 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P7 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P8 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P9 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P10 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P11 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P12 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P13 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P14 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P15 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

	pSector = &SectorInfo[ SEC_P16 ];
	pSector->ubTravelRating = 0;
	pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ]		= EDGEOFWORLD;
	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;
}
#endif

BOOLEAN InitStrategicMovementCosts()
{
	char fileName[MAX_PATH];
#ifdef JA2UB
if ( gGameUBOptions.StrategicMovementCostsXML == TRUE )
	{
#endif
	// need to be here
	strcpy(fileName, TABLEDATA_DIRECTORY);
	strcat(fileName, MOVEMENTCOSTFILENAME);
	if(!ReadInStrategicMovementCosts(fileName))
		return FALSE;
#ifdef JA2UB
	}
	else
	{
	InitStrategicRowA();
	InitStrategicRowB();
	InitStrategicRowC();
	InitStrategicRowD();
	InitStrategicRowE();
	InitStrategicRowF();
	InitStrategicRowG();
	InitStrategicRowH();
	InitStrategicRowI();
	InitStrategicRowJ();
	InitStrategicRowK();
	InitStrategicRowL();
	InitStrategicRowM();
	InitStrategicRowN();
	InitStrategicRowO();
	InitStrategicRowP();
	}
#endif	
	//WriteInStrategicMovementCosts("TABLEDATA\\~MovementCosts.xml");


	#ifdef JA2TESTVERSION
	{ //Simply make sure all shared borders between sectors match.
		INT32 x,y;
		OutputDebugString( "STRATEGIC MOVE COSTS:	Testing validity of data (okay if no following debug msgs)...\n");
		for( y = 1; y <= 16; y++ ) for( x = 1; x <= 16; x++ )
		{
			if( y > 1 )
			{
				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] !=
					SectorInfo[ SECTOR(x,y-1) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] )
				{	//north
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	North mismatch for sector %c%d\n", y+'A'-1, x ) );
				}
				if( y == 16 && SectorInfo[ SECTOR(x,y) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] != EDGEOFWORLD )
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	South should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
			}
			else
			{
				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] !=
				 SectorInfo[ SECTOR(x,y+1) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] )
				{ //south
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	South mismatch for sector %c%d\n", y+'A'-1, x ) );
				}
				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] != EDGEOFWORLD )
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	North should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
			}
			if( x < 16 )
			{
				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ EAST_STRATEGIC_MOVE ] !=
					SectorInfo[ SECTOR(x+1,y) ].ubTraversability[ WEST_STRATEGIC_MOVE ] )
				{ //east
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	East mismatch for sector %c%d\n", y+'A'-1, x ) );
				}
				if( x == 1 && SectorInfo[ SECTOR(x,y) ].ubTraversability[ WEST_STRATEGIC_MOVE ] != EDGEOFWORLD )
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	West should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
			}
			else
			{
				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ WEST_STRATEGIC_MOVE ] !=
				 SectorInfo[ SECTOR(x-1,y) ].ubTraversability[ EAST_STRATEGIC_MOVE ] )
				{ //west
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	West mismatch for sector %c%d\n", y+'A'-1, x ) );
				}
				if( SectorInfo[ SECTOR(x,y) ].ubTraversability[ EAST_STRATEGIC_MOVE ] != EDGEOFWORLD )
					OutputDebugString( (LPCSTR)String( "STRATEGIC MOVE COSTS:	East should be EDGEOFWORLD for sector %c%d\n", y+'A'-1, x ) );
			}
		}
	}
	#endif
	
	#ifdef JA2UB
	if ( gGameUBOptions.MakeStrategicMovementCosts == TRUE )
		UpdateCustomMapMovementCosts(); //Ja25 UB
	//MakeBadSectorListFromMapsOnHardDrive( TRUE ); //ja25 UB
	#endif
	


	return( TRUE );
}

#ifdef JA2UB
void AddCustomMap( INT32 iRow, INT32 iCol, BOOLEAN fDisplayMessages, BOOLEAN fMessageIfNotExist ) //ja25 UB
{
	CHAR8			zMapName[200];
	//SUMMARYFILE *pSummary;
	//SUMMARYFILE *pSummary
	INT8			bLevel;
	UNDERGROUND_SECTORINFO *pSector;
	//UNDERGROUND_SECTORINFO *curr;
	//SECTORINFO			   *pSector = NULL;

	// if we are the first secotr, ignore!
	//if ( iRow == JA2_5_START_SECTOR_Y && iCol == JA2_5_START_SECTOR_X )
	//{
	//	return;
	//}


	for ( bLevel = 0; bLevel < 4; bLevel++ )
	{
		
		// ATE: Check for existance of 'remove' file - if so, remove this sector from list, adjust
		// movement costs accordingly...
		if ( bLevel == 0 )
		{
			sprintf( zMapName, "Maps\\%c%d.nomap", iRow + 'A' -1, iCol );

			if ( FileExistsNoDB( zMapName ) )
			{
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector = FALSE;
				sBadSectorsList[iCol][iRow] = 1;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubTravelRating = 50;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubTraversability[ NORTH_STRATEGIC_MOVE ]   = EDGEOFWORLD;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubTraversability[ EAST_STRATEGIC_MOVE ]	   = EDGEOFWORLD;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ]   = EDGEOFWORLD;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubTraversability[ WEST_STRATEGIC_MOVE ]	   = EDGEOFWORLD;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubTraversability[ THROUGH_STRATEGIC_MOVE ] = EDGEOFWORLD;

				// update adjacent movement costs
				if( iRow > 1 )
				{
					SectorInfo[ ( SECTOR( iCol, iRow-1 ) ) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] = EDGEOFWORLD;
				}
					
				if( iCol > 1 )
				{
					SectorInfo[ ( SECTOR( iCol-1, iRow ) ) ].ubTraversability[ EAST_STRATEGIC_MOVE ] = EDGEOFWORLD;
				}
					
				if( iRow < 16 )
				{
					SectorInfo[ ( SECTOR( iCol, iRow+1 ) ) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] = EDGEOFWORLD;
				}
					
				if( iCol < 16 )
				{
					SectorInfo[ ( SECTOR( iCol+1, iRow ) ) ].ubTraversability[ WEST_STRATEGIC_MOVE ] = EDGEOFWORLD;
				}
				continue;
			}
		}

		if ( bLevel == 0 )
		{
			sprintf( zMapName, "Maps\\%c%d.dat", iRow + 'A' -1, iCol );
		}
		else
		{
			sprintf( zMapName, "Maps\\%c%d_b%d.dat", iRow + 'A' -1, iCol, bLevel );
		}

		//check to see if there is a map here
		if ( !FileExistsNoDB( zMapName ) )
		{
			if ( fMessageIfNotExist && bLevel == 0 )
			{
				if ( fDisplayMessages )
				{
					sprintf( zMapName, "Maps\\%c%d", iRow + 'A' -1, iCol );
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map file does not exist." );
				}
			}
			//return;
			continue;
		}

		if ( bLevel == 0 )
		{
			sprintf( zMapName, "Maps\\%c%d", iRow + 'A' -1, iCol );
		}
		else
		{
			sprintf( zMapName, "Maps\\%c%d_b%d", iRow + 'A' -1, iCol, bLevel );
		}

		// OK, now check a bunch of conditions...
		
		// Is this level a virgin?
		if ( bLevel == 0 )
		{
			if ( GetSectorFlagStatus( (INT16)iCol, (INT16)iRow, 0, SF_ALREADY_VISITED ) )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map has already been visited." );
					
					SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector = TRUE;
					sBadSectorsList[iCol][iRow] = 0;					
				}
				continue;
			}
			
		}
		else
		{
			pSector = FindUnderGroundSector( (INT16)iCol, (INT16)iRow, bLevel );

			if ( pSector )
			{
				if ( pSector->fVisited )
				{
					if ( fDisplayMessages )
					{
						ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
						ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map has already been visited." );
						sBadSectorsList[iCol][iRow] = 0;
					}
					continue;
				}
			}
		}
		
		// Is this the current sector?
		if ( gWorldSectorX == iCol && gWorldSectorY == iRow && gbWorldSectorZ == bLevel )
		{
			if ( fDisplayMessages )
			{
				ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
				ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map is the current sector." );
				sBadSectorsList[iCol][iRow] = 0;
			}
			continue;
		}
		
		
			//We're good!
			if ( fDisplayMessages )
			{
				ScreenMsg( FONT_MCOLOR_WHITE, MSG_CHAT, L"Added Custom map: %S.", zMapName );
			}
			
			if ( bLevel == 0 )
			{
				if ( SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCampaignSector )
				{
					// ATE: This has been changed, if a campaign sector, just remove flag...
					SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCampaignSector = FALSE;
					sBadSectorsList[iCol][iRow] = 0;
					continue;
				}
			}
			
			else
			{
				// See if this sector exists, and if so is it a campagin sectior?
				pSector = FindUnderGroundSector( (INT16)iCol, (INT16)iRow, bLevel );

				if ( pSector )
				{
					if ( pSector->fCampaignSector )
					{
						// ATE: This has been changed, if a campaign sector, just remove flag...
						pSector->fCampaignSector = FALSE;
						sBadSectorsList[iCol][iRow] = 0;
						continue;
					}
				}
			}
					
			if ( bLevel == 0 )
			{
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector = TRUE;
				sBadSectorsList[iCol][iRow] = 0;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCustomSector = TRUE;
				
								
				if ( gGameUBOptions.AddRandomEnemyToSector == TRUE )
					{
						if ( SECTOR( iCol , iRow ) == SEC_H7 || SECTOR( iCol , iRow )  == SEC_H8  || SECTOR( iCol , iRow ) == SEC_H9  ||
							 SECTOR( iCol , iRow ) == SEC_H10 || SECTOR( iCol , iRow ) == SEC_H11 || SECTOR( iCol , iRow ) == SEC_I9  ||
							 SECTOR( iCol , iRow ) == SEC_I10 || SECTOR( iCol , iRow ) == SEC_I11 || SECTOR( iCol , iRow ) == SEC_I12 ||
							 SECTOR( iCol , iRow ) == SEC_I13 || SECTOR( iCol , iRow ) == SEC_J11 || SECTOR( iCol , iRow ) == SEC_J12 ||
							 SECTOR( iCol , iRow ) == SEC_J13 || SECTOR( iCol , iRow ) == SEC_K14 || SECTOR( iCol , iRow ) == SEC_K15 ||
							 SECTOR( iCol , iRow ) == SEC_K16 || SECTOR( iCol , iRow ) == SEC_L15 || 
							 ( SECTOR( iCol , iRow ) == SEC_I13 && bLevel == 1) || ( SECTOR( iCol , iRow ) == SEC_J13 && bLevel == 1) ||
							 ( SECTOR( iCol , iRow ) == SEC_J14 && bLevel == 1) || ( SECTOR( iCol , iRow ) == SEC_K15 && bLevel == 1) ||
							 ( SECTOR( iCol , iRow ) == SEC_K15 && bLevel == 2) || ( SECTOR( iCol , iRow ) == SEC_L15 && bLevel == 2) ||
							 ( SECTOR( iCol , iRow ) == SEC_L15 && bLevel == 3) )
							 {
							 }
							 else
							 {
								ScreenMsg( FONT_MCOLOR_WHITE, MSG_CHAT, L"Added Random Enemy to sector : %S.", zMapName );
								RandomAddEnemy( iCol, iRow, bLevel );
							 }
					}
			
			
			}
			else
			{
			
				pSector = FindUnderGroundSector( (UINT8)iCol, (UINT8)iRow, bLevel );
				if ( pSector )
				{
					//ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_WHITE, MSG_CHAT, L"Added New Underground : %S.", zMapName );
					pSector = NewUndergroundNode( (UINT8)iCol, (UINT8)iRow, bLevel );
					sBadSectorsList[iCol][iRow] = 0;
					if ( gGameUBOptions.AddRandomEnemyToSector == TRUE )
						{
						if ( SECTOR( iCol , iRow ) == SEC_H7 || SECTOR( iCol , iRow )  == SEC_H8  || SECTOR( iCol , iRow ) == SEC_H9  ||
							 SECTOR( iCol , iRow ) == SEC_H10 || SECTOR( iCol , iRow ) == SEC_H11 || SECTOR( iCol , iRow ) == SEC_I9  ||
							 SECTOR( iCol , iRow ) == SEC_I10 || SECTOR( iCol , iRow ) == SEC_I11 || SECTOR( iCol , iRow ) == SEC_I12 ||
							 SECTOR( iCol , iRow ) == SEC_I13 || SECTOR( iCol , iRow ) == SEC_J11 || SECTOR( iCol , iRow ) == SEC_J12 ||
							 SECTOR( iCol , iRow ) == SEC_J13 || SECTOR( iCol , iRow ) == SEC_K14 || SECTOR( iCol , iRow ) == SEC_K15 ||
							 SECTOR( iCol , iRow ) == SEC_K16 || SECTOR( iCol , iRow ) == SEC_L15 || 
							 ( SECTOR( iCol , iRow ) == SEC_I13 && bLevel == 1) || ( SECTOR( iCol , iRow ) == SEC_J13 && bLevel == 1) ||
							 ( SECTOR( iCol , iRow ) == SEC_J14 && bLevel == 1) || ( SECTOR( iCol , iRow ) == SEC_K15 && bLevel == 1) ||
							 ( SECTOR( iCol , iRow ) == SEC_K15 && bLevel == 2) || ( SECTOR( iCol , iRow ) == SEC_L15 && bLevel == 2) ||
							 ( SECTOR( iCol , iRow ) == SEC_L15 && bLevel == 3) )
							 {
							 }
							 else
							 {
								ScreenMsg( FONT_MCOLOR_WHITE, MSG_CHAT, L"Added Random Enemy to sector : %S.", zMapName );
								RandomAddEnemy( iCol, iRow, bLevel );
							 }
			
						}
				
				}
			}
	
		// I guess we need to load level to get the map information....?
		// we don't want to destroy the level we're in, however....
		//pSummary = (SUMMARYFILE*)MemAlloc( sizeof( SUMMARYFILE ) );
		//memset( pSummary, 0, sizeof( SUMMARYFILE ) );
/*
		if ( !EvaluateWorldEx( zMapName, 0, pSummary, FALSE, FALSE, NULL ) )
		{
			if ( fDisplayMessages )
			{
				// Spew message
				ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
				
				if ( gfTriedToLoadOldMapVersion )
				{
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Incompatible map." );
				}
			}
		}
		else
		{
			//JA25_SECTOR_AI		SectorAIInfo;
			UINT8							ubNumAdmins, ubNumTroops, ubNumElites, ubNumBloodcats;
			
			//Test some stuff here, like the total # of placements, enemies, etc
			switch( gGameOptions.ubDifficultyLevel )
			{
				case DIF_LEVEL_EASY:
					ubNumAdmins = pSummary->MapInfo.ubNumAdminsE;
					ubNumTroops = pSummary->MapInfo.ubNumTroopsE;
					ubNumElites = pSummary->MapInfo.ubNumElitesE;
					ubNumBloodcats = pSummary->MapInfo.ubNumBloodcatsE;
					break;
				case DIF_LEVEL_MEDIUM:
					ubNumAdmins = pSummary->MapInfo.ubNumAdminsN;
					ubNumTroops = pSummary->MapInfo.ubNumTroopsN;
					ubNumElites = pSummary->MapInfo.ubNumElitesN;
					ubNumBloodcats = pSummary->MapInfo.ubNumBloodcatsN;
					break;
				case DIF_LEVEL_HARD:
					ubNumAdmins = pSummary->MapInfo.ubNumAdminsD;
					ubNumTroops = pSummary->MapInfo.ubNumTroopsD;
					ubNumElites = pSummary->MapInfo.ubNumElitesD;
					ubNumBloodcats = pSummary->MapInfo.ubNumBloodcatsD;
					break;
			}
			
			/*if( pSummary->MapInfo.sNorthGridNo	== -1 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map needs NORTH entry point." );
				}
				MemFree( pSummary );
				continue;
			}

			if( pSummary->MapInfo.sEastGridNo	== -1 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map needs EAST entry point." );
				}
				MemFree( pSummary );
				continue;
			}

			if( pSummary->MapInfo.sWestGridNo	== -1 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map needs WEST entry point." );
				}
				MemFree( pSummary );
				continue;
			}

			if( pSummary->MapInfo.sSouthGridNo	== -1 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map needs SOUTH entry point." );
				}
				MemFree( pSummary );
				continue;
			}

			if( pSummary->MapInfo.sCenterGridNo	== -1 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map needs CENTER entry point." );
				}
				MemFree( pSummary );
				continue;
			}

			
			if ( ( pSummary->ubAdminDetailed + pSummary->ubTroopDetailed + pSummary->ubEliteDetailed ) != 32 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map requires 32 enemy placements." );
				}
				MemFree( pSummary );
				continue;
			}
			
			// Check total populations
			if ( ( ubNumTroops + ubNumAdmins + ubNumElites ) > 32 )
			{
				if ( fDisplayMessages )
				{
					ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Map population total must be less than 32." );
				}
				MemFree( pSummary );
				continue;
			}
			
			// Check insertion gridno if baaenment
			if ( bLevel > 0 )
			{
				if ( pSummary->MapInfo.sInsertionGridNo == -1 )
				{
					if ( fDisplayMessages )
					{
						ScreenMsg( MSG_FONT_RED, MSG_CHAT, L"Failed to add map: %S.", zMapName );
						ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"Underground level needs invasion insertion gridno value." );
					}
					MemFree( pSummary );
					continue;					
				}
			}
			
			//We're good!
			if ( fDisplayMessages )
			{
				ScreenMsg( FONT_MCOLOR_WHITE, MSG_CHAT, L"Added Custom map: %S.", zMapName );
			}
			
			//if the sector shoulnd have a map there
			if ( bLevel == 0 )
			{
				if ( SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCampaignSector )
				{
					// ATE: This has been changed, if a campaign sector, just remove flag...
					SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCampaignSector = FALSE;
					//continue;
				}
			}
			else
			{
				// See if this sector exists, and if so is it a campagin sectior?
				pSector = FindUnderGroundSector( (INT16)iCol, (INT16)iRow, bLevel );

				if ( pSector )
				{
					if ( pSector->fCampaignSector )
					{
						// ATE: This has been changed, if a campaign sector, just remove flag...
						pSector->fCampaignSector = FALSE;
						//continue;
					}
				}
			}
			
			if ( bLevel == 0 )
			{
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector = TRUE;
				SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCustomSector = TRUE;

				// Seed level
				//SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubNumAdmins = ubNumAdmins;
				//SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubNumTroops = ubNumTroops;
				//SectorInfo[ ( SECTOR( iCol , iRow ) ) ].ubNumElites = ubNumElites;
				//SectorInfo[ ( SECTOR( iCol , iRow ) ) ].bBloodCats  = ubNumBloodcats;

			//	wcscpy( SectorInfo[ ( SECTOR( iCol , iRow ) ) ].zCustomLevelName, pSummary->MapInfo.zLevelName );
			}
			else
			{
				pSector = NewUndergroundNode( (UINT8)iCol, (UINT8)iRow, bLevel, FALSE );

				// Seed
				pSector->ubNumAdmins = ubNumAdmins;
				pSector->ubNumTroops = ubNumTroops;
				pSector->ubNumElites = ubNumElites;

				wcscpy( pSector->zCustomLevelName, pSummary->MapInfo.zLevelName );
			}
			
			// Seed AI
			memset( &SectorAIInfo, 0, sizeof( SectorAIInfo ) );
			
			SectorAIInfo.iSectorID = SECTOR( iCol , iRow ) ;
			SectorAIInfo.bSectorZ = bLevel;
			SectorAIInfo.bBaseNumEnemies = pSummary->MapInfo.ubBaseNumEnemies;
			SectorAIInfo.bRandomNumEnemies = 4;
			SectorAIInfo.bProbabilityOfAttacking = pSummary->MapInfo.ubAttackProbabilityRate;
			SectorAIInfo.bMaxProbabilityForAttackingSector = 60;
			SectorAIInfo.ubMinimumProbabiltyBeforeAttack = pSummary->MapInfo.ubGracePeriod;
			SectorAIInfo.fAutoDirection = pSummary->MapInfo.fAutoDirection;
			SectorAIInfo.ubInsertionDirection = pSummary->MapInfo.ubInsertionDirection;
			SectorAIInfo.sInsertionGridNo = pSummary->MapInfo.sInsertionGridNo;
			SectorAIInfo.fCustomSector = TRUE;
			SectorAIInfo.ubLoadingScreenID =  pSummary->MapInfo.ubLoadingScreenID;

			AddJA25AIDataToSector( &SectorAIInfo );

			if ( bLevel == 0 )
			{
				SetNumberOfJa25BloodCatsInSector( SectorAIInfo.iSectorID, ubNumBloodcats, pSummary->CreatureTeam.ubNumAnimals );
			}
			
		}

		MemFree( pSummary );
		*/
		
	}	

}

void MakeBadSectorListFromMapsOnHardDrive( BOOLEAN fDisplayMessages )
{
	INT32			iRow, iCol;
	
	
	for( iRow=1; iRow<=16; iRow++ )
	{
		for( iCol=1; iCol<=16; iCol++ )
		{
			sBadSectorsList[iCol][iRow] = 1;
		}
	}

	SetUpValidCampaignSectors( );

	//loop through all the sectors
	for( iRow=1; iRow<=16; iRow++ )
	{
		for( iCol=1; iCol<=16; iCol++ )
		{
			AddCustomMap( iRow, iCol, fDisplayMessages, FALSE );
		}
	}
	
if ( gGameUBOptions.MakeStrategicMovementCosts == TRUE )
	UpdateCustomMapMovementCosts();
}

void UpdateCustomMapMovementCosts()
{
	INT32			iRow, iCol;

	//loop through all the sectors
	for( iRow=1; iRow<=16; iRow++ )
	{
		for( iCol=1; iCol<=16; iCol++ )
		{
		
		
			if( SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fValidSector && SectorInfo[ ( SECTOR( iCol , iRow ) ) ].fCustomSector )
			{
				
				//if ( SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ THROUGH_STRATEGIC_MOVE ] == EDGEOFWORLD )
					SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ THROUGH_STRATEGIC_MOVE ] = PLAINS;
				
				//sector above is clear
				if( iRow > 1 && SectorInfo[ ( SECTOR( iCol , iRow - 1 ) ) ].fValidSector )
				{
				
					//if ( SectorInfo[ ( SECTOR( iCol, iRow-1 ) ) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol, iRow-1 ) ) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] = PLAINS;
					//if ( SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] = PLAINS;
					
					
				}
					
				//sector west is clear
				if( iCol > 1 && SectorInfo[ ( SECTOR( iCol - 1 , iRow ) ) ].fValidSector )
				{
					//if ( SectorInfo[ ( SECTOR( iCol-1, iRow ) ) ].ubTraversability[ EAST_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol-1, iRow ) ) ].ubTraversability[ EAST_STRATEGIC_MOVE ] = PLAINS;
					//if ( SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ WEST_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ WEST_STRATEGIC_MOVE ] = PLAINS;
				}
					
				//sector south is clear
				if( iRow < 16 && SectorInfo[ ( SECTOR( iCol , iRow + 1 ) ) ].fValidSector )
				{
					//if ( SectorInfo[ ( SECTOR( iCol, iRow+1 ) ) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol, iRow+1 ) ) ].ubTraversability[ NORTH_STRATEGIC_MOVE ] = PLAINS;
					//if ( SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ SOUTH_STRATEGIC_MOVE ] = PLAINS;
				}
					
				//sector east is clear
				if( iCol < 16 && SectorInfo[ ( SECTOR( iCol + 1 , iRow ) ) ].fValidSector )
				{
					//if ( SectorInfo[ ( SECTOR( iCol+1, iRow ) ) ].ubTraversability[ WEST_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol+1, iRow ) ) ].ubTraversability[ WEST_STRATEGIC_MOVE ] = PLAINS;
					//if ( SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ EAST_STRATEGIC_MOVE ] == EDGEOFWORLD )
						SectorInfo[ ( SECTOR( iCol, iRow ) ) ].ubTraversability[ EAST_STRATEGIC_MOVE ] = PLAINS;
				}
			}
		}
	}
}
#endif
