#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Map Screen Helicopter.h"
	#include "LaptopSave.h"
	#include "Vehicles.h"
	#include "Finances.h"
	#include "Quests.h"
	#include "Game Clock.h"
	#include "strategic.h"
	#include "Queen Command.h"
	#include "Strategic Pathing.h"
	#include "Random.h"
	#include "Game Event Hook.h"
	#include "String.h"
	#include "Dialogue Control.h"
	#include "Message.h"
	#include "Strategic Movement.h"
	#include "Soldier Profile.h"
	#include "Assignments.h"
	#include "PreBattle Interface.h"
	#include "strategicmap.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "tiledat.h"
	#include "Map Screen Interface.h"
	#include "Text.h"
	#include "Squads.h"
	#include "Player Command.h"
	#include "Sound Control.h"
	#include "GameSettings.h"
	#include "meanwhile.h"
	#include "Map Screen Interface Border.h"
	#include "Strategic Event Handler.h"
	#include "Overhead.h"
	#include "Soldier Create.h"
	#include "renderworld.h"
	#include "soundman.h"
	#include "Isometric Utils.h"
	#include "Scheduling.h"
	// HEADROCK HAM 3.5: Added facility-based skyrider costs modifier
	#include "Facilities.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "merc entering.h"	// added by Flugente
	#include "ASD.h"			// added by Flugente
#endif

#include "Vehicles.h"
#include "NPC.h"
#include "history.h"

// the amounts of time to wait for hover stuff
// externalised, use gHelicopterSettings.ubHelicopterTimeDelayForHoverWait instead
//#define TIME_DELAY_FOR_HOVER_WAIT						10		// minutes
// externalised, use gHelicopterSettings.ubHelicopterTimeDelayForHoverWaitTooLong instead
//#define TIME_DELAY_FOR_HOVER_WAIT_TOO_LONG	20		// mintues


#define MIN_DAYS_BETWEEN_SKYRIDER_MONOLOGUES 1

// refuel delay
// externalised, use gHelicopterSettings.ubHelicopterRefuelTime instead
//#define REFUEL_HELICOPTER_DELAY							30		// minutes

// total number of sectors one can go
// externalised, use gHelicopterSettings.ubHelicopterDistanceWithoutRefuel instead
//#define MAX_HELICOPTER_DISTANCE 25 

// maximum chance out of a hundred per unsafe sector that a SAM site in decent working condition will hit Skyrider
// externalised, use gHelicopterSettings.ubHelicopterSAMSiteAccuracy instead
//#define MAX_SAM_SITE_ACCURACY		33

#define HELICOPTER_RETURN_REASON_NONE 0
#define HELICOPTER_RETURN_REASON_LOW_FUEL 1
#define HELICOPTER_RETURN_REASON_DAMAGE 2

extern FACETYPE	*gpCurrentTalkingFace;
extern UINT8			gubCurrentTalkingID;

// current temp path for dest char
extern PathStPtr pTempHelicopterPath;

// the seating capacities
//extern INT32 iSeatingCapacities[];

// the static NPC dialogue faces
//extern UINT32 uiExternalStaticNPCFaces[];

// the squad mvt groups
extern INT8 SquadMovementGroups[ ];


// whether helicopter variables have been set up
BOOLEAN fSkyRiderSetUp = FALSE;

// plotting for a helicopter
BOOLEAN fPlotForHelicopter = FALSE;

// is the helicopter available to player?
BOOLEAN fHelicopterAvailable = FALSE;

// helicopter vehicle id
INT32 iHelicopterVehicleId = -1;

// helicopter icon
UINT32 guiHelicopterIcon;
UINT32 guiEnemyHelicopterIcon;

// total distance travelled
INT32 iTotalHeliDistanceSinceRefuel = 0;

// total owed to player
INT32 iTotalAccumulatedCostByPlayer = 0;

// whether or not skyrider is alive and well? and on our side yet?
BOOLEAN fSkyRiderAvailable = FALSE;

#ifdef JA2TESTVERSION
BOOLEAN	fSAMSitesDisabledFromAttackingPlayer = FALSE;
#endif

// helicopter destroyed
BOOLEAN fHelicopterDestroyed = FALSE;

/*Buggler: externalized to HeliSites.xml
// list of sector locations where SkyRider can be refueled
UINT8 ubRefuelList[ MAX_NUMBER_OF_REFUEL_SITES ][ 2 ] =
{
	{ 13, 2 },		// Drassen airport
	{	6, 9 },		// Estoni
};

INT32 iRefuelHeliGridNo[ MAX_NUMBER_OF_REFUEL_SITES ] ={
	9001, // drassen
	13067, // estoni
};*/

// whether or not helicopter can refuel at this site
BOOLEAN fRefuelingSiteAvailable[ MAX_NUMBER_OF_REFUEL_SITES ];		//No need to externalize as all sectors are enemy controlled on gamestart

// is the heli in the air?
BOOLEAN fHelicopterIsAirBorne = FALSE;

// is the pilot returning straight to base?
BOOLEAN fHeliReturnStraightToBase = FALSE;

// heli hovering
BOOLEAN fHoveringHelicopter = FALSE;

// time started hovering
UINT32 uiStartHoverTime = 0;

// what state are skyrider's monologues in in?
UINT32 guiHelicopterSkyriderTalkState = 0;

// the flags for skyrider events
BOOLEAN fShowEstoniRefuelHighLight = FALSE;
BOOLEAN fShowOtherSAMHighLight = FALSE;
BOOLEAN fShowDrassenSAMHighLight = FALSE;
BOOLEAN fShowCambriaHospitalHighLight = FALSE;

UINT32 guiTimeOfLastSkyriderMonologue = 0;

UINT8 gubHelicopterHitsTaken = 0;

UINT8 gubHelicopterHoursToRepair = 0;
UINT8 gubHelicopterBasicRepairsSoFar = 0;
UINT8 gubHelicopterSeriousRepairsSoFar = 0;

UINT8 gubHelicopterHoverTime = 0;
UINT8 gubHelicopterTimeToFullRefuel = 0;

BOOLEAN gfSkyriderSaidCongratsOnTakingSAM = FALSE;
UINT8 gubPlayerProgressSkyriderLastCommentedOn = 0;

// skyrider placeholder
SOLDIERTYPE SoldierSkyRider;

SOLDIERTYPE *pSkyRider;



// helicopter char dialogue
BOOLEAN HeliCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum );

// does skyrider notice bad guys in sector?
BOOLEAN DoesSkyriderNoticeEnemiesInSector( UINT8 ubNumEnemies );

// are we at the end of the path for the helicopter
BOOLEAN EndOfHelicoptersPath( void );

// find the location sector of closest refuel point for heli..and the criteria if the sector must be under the players control
INT32 FindLocationOfClosestRefuelSite( BOOLEAN fMustBeAvailable, INT16 sX, INT16 sY );


// add the tactical heli graphic
void AddHelicopterToMaps( BOOLEAN fAdd, UINT8 ubSite );

void PaySkyriderBill( void );
void MakeHeliReturnToBase( UINT8 ubReturnReason );

void HandleSkyRiderMonologueAboutDrassenSAMSite( UINT32 uiSpecialCode );
void HandleSkyRiderMonologueAboutCambriaHospital( UINT32 uiSpecialCode );
void HandleSkyRiderMonologueAboutOtherSAMSites( UINT32 uiSpecialCode );
void HandleSkyRiderMonologueAboutEstoniRefuel( UINT32 uiSpecialCode );

// refueling sites externalization stuff
UINT8	NUMBER_OF_REFUEL_SITES;

// coordinates X,Y of refuel sites on strategic map
INT16 sRefuelSectorX[ MAX_NUMBER_OF_REFUEL_SITES ];
INT16 sRefuelSectorY[ MAX_NUMBER_OF_REFUEL_SITES ];

// refuel sites known on game start
BOOLEAN fRefuelingSiteKnown[ MAX_NUMBER_OF_REFUEL_SITES ];

// heli tile index no in tileset
INT32 iRefuelHeliTileIndex[ MAX_NUMBER_OF_REFUEL_SITES ];

// heli graphics grid no
INT32 iRefuelHeliGridNo[ MAX_NUMBER_OF_REFUEL_SITES ];
// skyrider standing grid no
INT32 iRefuelSkyriderGridNo[ MAX_NUMBER_OF_REFUEL_SITES ];

#define MAX_CHAR_DATA_LENGTH			500
#define INVALID_REFUELSITE_INDEX			-1


typedef enum
{
	HELISITE_ELEMENT_NONE = 0,
	HELISITE_ELEMENT_HELIINFO,
	HELISITE_ELEMENT_REFUELLIST,
	HELISITE_ELEMENT_REFUEL,
	HELISITE_ELEMENT_INDEX,
	HELISITE_ELEMENT_REFUELSECTOR,
	HELISITE_ELEMENT_REFUELSECTOR_X,
	HELISITE_ELEMENT_REFUELSECTOR_Y,
	HELISITE_ELEMENT_REFUEL_HIDDEN,
	HELISITE_ELEMENT_REFUEL_HELI_TILE_INDEX,
	HELISITE_ELEMENT_REFUEL_HELI_GRIDNO,
	HELISITE_ELEMENT_REFUEL_SKYRIDER_GRIDNO,
} HELISITE_PARSE_STAGE;

typedef struct
{
	UINT32	uiIndex;
	INT16	refuelSectorX;
	INT16	refuelSectorY;
	BOOLEAN	refuelHidden;
	INT32	refuelHeliTileIndex;
	INT32	refuelHeliGridNo;
	INT32	refuelSkyriderGridNo;
} heliInfo;

typedef struct
{
	HELISITE_PARSE_STAGE	curElement;

	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	heliInfo				curHeliInfo;
	UINT32					uiHighestIndex;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} helisiteParseData;

static void XMLCALL
helisiteStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	helisiteParseData * pData = (helisiteParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{

		if(strcmp(name, "HELI_INFO") == 0 && pData->curElement == HELISITE_ELEMENT_NONE)
		{
			pData->curElement = HELISITE_ELEMENT_HELIINFO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "REFUELLIST") == 0 && pData->curElement == HELISITE_ELEMENT_HELIINFO)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELLIST;

			memset( sRefuelSectorX,				0,	sizeof(sRefuelSectorX)			);
			memset( sRefuelSectorY,				0,	sizeof(sRefuelSectorY)			);
			memset( fRefuelingSiteKnown,		0,	sizeof(fRefuelingSiteKnown)		);
			memset( iRefuelHeliTileIndex,		0,	sizeof(iRefuelHeliTileIndex)	);
			memset( iRefuelHeliGridNo,			0,	sizeof(iRefuelHeliGridNo)		);
			memset( iRefuelSkyriderGridNo,		0,	sizeof(iRefuelSkyriderGridNo)	);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "REFUEL") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELLIST)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;

			memset( &pData->curHeliInfo, 0, sizeof(heliInfo) );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "refuelIndex") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_INDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "refuelSector") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELSECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "x") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELSECTOR)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELSECTOR_X;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "y") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELSECTOR)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELSECTOR_Y;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "refuelHidden") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL_HIDDEN;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "refuelHeliTileIndex") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL_HELI_TILE_INDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "refuelHeliGridNo") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL_HELI_GRIDNO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "refuelSkyriderGridNo") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL_SKYRIDER_GRIDNO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
helisiteCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	helisiteParseData * pData = (helisiteParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
helisiteEndElementHandle(void *userData, const XML_Char *name)
{
	helisiteParseData * pData = (helisiteParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "HELI_INFO") == 0 && pData->curElement == HELISITE_ELEMENT_HELIINFO)
		{
			pData->curElement = HELISITE_ELEMENT_NONE;
		}
		else if(strcmp(name, "REFUELLIST") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELLIST)
		{
			pData->curElement = HELISITE_ELEMENT_HELIINFO;

			NUMBER_OF_REFUEL_SITES = pData->uiHighestIndex;
		}
		else if(strcmp(name, "REFUEL") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELLIST;

			if ( pData->curHeliInfo.uiIndex != INVALID_REFUELSITE_INDEX )
			{
				pData->curHeliInfo.uiIndex--;	
				sRefuelSectorX [ pData->curHeliInfo.uiIndex ]			= pData->curHeliInfo.refuelSectorX;
				sRefuelSectorY [ pData->curHeliInfo.uiIndex ]			= pData->curHeliInfo.refuelSectorY;
				fRefuelingSiteKnown [ pData->curHeliInfo.uiIndex ]		= !pData->curHeliInfo.refuelHidden;
				iRefuelHeliTileIndex [ pData->curHeliInfo.uiIndex ]		= pData->curHeliInfo.refuelHeliTileIndex;
				iRefuelHeliGridNo [ pData->curHeliInfo.uiIndex ]		= pData->curHeliInfo.refuelHeliGridNo;
				iRefuelSkyriderGridNo [ pData->curHeliInfo.uiIndex ]	= pData->curHeliInfo.refuelSkyriderGridNo;
			}
		}
		else if(strcmp(name, "refuelIndex") == 0 && pData->curElement == HELISITE_ELEMENT_INDEX)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;

			pData->curHeliInfo.uiIndex = atol(pData->szCharData);
			if ( !pData->curHeliInfo.uiIndex || pData->curHeliInfo.uiIndex > MAX_NUMBER_OF_REFUEL_SITES )
			{
				pData->curHeliInfo.uiIndex = INVALID_REFUELSITE_INDEX;
			}
			else if ( pData->curHeliInfo.uiIndex > pData->uiHighestIndex )
			{
				pData->uiHighestIndex = pData->curHeliInfo.uiIndex;
			}
		}
		else if(strcmp(name, "refuelSector") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELSECTOR)
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;
		}
		else if(strcmp(name, "x") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELSECTOR_X)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELSECTOR;

			pData->curHeliInfo.refuelSectorX = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "y") == 0 && pData->curElement == HELISITE_ELEMENT_REFUELSECTOR_Y)
		{
			pData->curElement = HELISITE_ELEMENT_REFUELSECTOR;

			pData->curHeliInfo.refuelSectorY = (INT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "refuelHidden") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL_HIDDEN )
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;

			pData->curHeliInfo.refuelHidden = (BOOLEAN)atol(pData->szCharData);
		}
		else if(strcmp(name, "refuelHeliTileIndex") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL_HELI_TILE_INDEX )
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;

			pData->curHeliInfo.refuelHeliTileIndex = atol(pData->szCharData);
		}
		else if(strcmp(name, "refuelHeliGridNo") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL_HELI_GRIDNO )
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;

			pData->curHeliInfo.refuelHeliGridNo = atol(pData->szCharData);
			if ( pData->curHeliInfo.refuelHeliGridNo >= MAX_ALLOWED_WORLD_MAX )
			{
				pData->curHeliInfo.refuelHeliGridNo = 0;
			}
		}
		else if(strcmp(name, "refuelSkyriderGridNo") == 0 && pData->curElement == HELISITE_ELEMENT_REFUEL_SKYRIDER_GRIDNO )
		{
			pData->curElement = HELISITE_ELEMENT_REFUEL;

			pData->curHeliInfo.refuelSkyriderGridNo = atol(pData->szCharData);
			if ( pData->curHeliInfo.refuelSkyriderGridNo >= MAX_ALLOWED_WORLD_MAX )
			{
				pData->curHeliInfo.refuelSkyriderGridNo = 0;
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInHeliInfo(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);

	helisiteParseData pData;

	// Open file
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


	XML_SetElementHandler(parser, helisiteStartElementHandle, helisiteEndElementHandle);
	XML_SetCharacterDataHandler(parser, helisiteCharacterDataHandle);


	memset(&pData,0,sizeof(pData));
	NUMBER_OF_REFUEL_SITES = 0;
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in HeliSites.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return TRUE;
}

BOOLEAN WriteInInfo(STR fileName)
{
	HWFILE		hFile;

	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );

	{
		INT8 cnt;


		FilePrintf(hFile,"<HELI_INFO>\r\n");
		FilePrintf(hFile,"\t<REFUELLIST>\r\n");
		for(cnt = 0; cnt < NUMBER_OF_REFUEL_SITES; cnt++)
		{
			FilePrintf(hFile,"\t\t<REFUEL>\r\n");

			FilePrintf(hFile,"\t\t\t<refuelIndex>%d</refuelIndex>\r\n",cnt+1);

			FilePrintf(hFile,"\t\t\t<refuelSector>\r\n");
			FilePrintf(hFile,"\t\t\t\t<x>%d</x>\r\n",sRefuelSectorX[cnt]);
			FilePrintf(hFile,"\t\t\t\t<y>%d</y>\r\n",sRefuelSectorY[cnt]);
			FilePrintf(hFile,"\t\t\t</refuelSector>\r\n");

			FilePrintf(hFile,"\t\t\t<refuelHidden>%d</refuelHidden>\r\n", !fRefuelingSiteKnown[cnt] );

			FilePrintf(hFile,"\t\t\t<refuelHeliTileIndex>%d</refuelHeliTileIndex>\r\n", iRefuelHeliTileIndex[cnt] );

			FilePrintf(hFile,"\t\t\t<refuelHeliGridNo>%d</refuelHeliGridNo>\r\n", iRefuelHeliGridNo[cnt] );

			FilePrintf(hFile,"\t\t\t<refuelSkyriderGridNo>%d</refuelSkyriderGridNo>\r\n", iRefuelSkyriderGridNo[cnt] );

			FilePrintf(hFile,"\t\t</REFUEL>\r\n");
		}
		FilePrintf(hFile,"\t</REFUELLIST>\r\n");
		FilePrintf(hFile,"</HELI_INFO>\r\n");
	}
	FileClose( hFile );

	return TRUE;
}

void InitializeHelicopter( void )
{
	// must be called whenever a new game starts up!
	fHelicopterAvailable = FALSE;
	iHelicopterVehicleId = -1;

	fSkyRiderAvailable = FALSE;
	fSkyRiderSetUp = FALSE;
	pSkyRider = NULL;
	SoldierSkyRider.initialize();

	fHelicopterIsAirBorne = FALSE;
	fHeliReturnStraightToBase = FALSE;

	fHoveringHelicopter = FALSE;
	uiStartHoverTime = 0;

	fPlotForHelicopter = FALSE;
	pTempHelicopterPath = NULL;

	iTotalHeliDistanceSinceRefuel = 0;
	iTotalAccumulatedCostByPlayer = 0;

	fHelicopterDestroyed = FALSE;

	guiHelicopterSkyriderTalkState = 0;
	guiTimeOfLastSkyriderMonologue = 0;

	fShowEstoniRefuelHighLight = FALSE;
	fShowOtherSAMHighLight = FALSE;
	fShowDrassenSAMHighLight = FALSE;
	fShowCambriaHospitalHighLight = FALSE;

	gfSkyriderEmptyHelpGiven = FALSE;

	gubHelicopterHitsTaken = 0;

	gfSkyriderSaidCongratsOnTakingSAM = FALSE;
	gubPlayerProgressSkyriderLastCommentedOn = 0;
}


BOOLEAN AddSoldierToHelicopter( SOLDIERTYPE *pSoldier )
{
	// attempt to add soldier to helicopter
	if( iHelicopterVehicleId == -1 )
	{
		// no heli yet
		return( FALSE );
	}

	// check if heli is in motion or if on the ground
	if( ( fHelicopterIsAirBorne == TRUE ) && (fHoveringHelicopter == FALSE ) )
	{
		return( FALSE );
	}

	// is the heli returning to base?..he ain't waiting if so
	if( fHeliReturnStraightToBase == TRUE )
	{
		return( FALSE );
	}


	// attempt to add to vehicle
	return( PutSoldierInVehicle( pSoldier, ( INT8 )iHelicopterVehicleId ) );
}


BOOLEAN RemoveSoldierFromHelicopter( SOLDIERTYPE *pSoldier )
{
	// attempt to add soldier to helicopter
	if( iHelicopterVehicleId == -1 )
	{
		// no heli yet
		return( FALSE );
	}

	// check if heli is in motion or if on the ground
	if( ( fHelicopterIsAirBorne == TRUE ) && (fHoveringHelicopter == FALSE ) )
	{
		return( FALSE );
	}

	// is the heli returning to base?..he ain't waiting if so
	if( fHeliReturnStraightToBase == TRUE )
	{
		return( FALSE );
	}

	pSoldier->sSectorX = pVehicleList[ iHelicopterVehicleId ].sSectorX;
	pSoldier->sSectorY = pVehicleList[ iHelicopterVehicleId ].sSectorY;
	pSoldier->bSectorZ = 0;

	// reset between sectors
	pSoldier->flags.fBetweenSectors = FALSE;


	// remove from the vehicle
	return( TakeSoldierOutOfVehicle( pSoldier ) );
}



BOOLEAN HandleHeliEnteringSector( INT16 sX, INT16 sY )
{
	// check for SAM attack upon the chopper.	If it's destroyed by the attack, do nothing else here
	if( HandleSAMSiteAttackOfHelicopterInSector( sX, sY ) == TRUE )
	{
		// destroyed
		return( TRUE );
	}

	// count how many enemies are camped there or passing through
	UINT8 ubNumEnemies = NumNonPlayerTeamMembersInSector( sX, sY, ENEMY_TEAM );

	// any baddies?
	if( ubNumEnemies > 0 )
	{
		// if the player didn't know about these prior to the chopper's arrival
		if( WhatPlayerKnowsAboutEnemiesInSector( sX, sY ) == KNOWS_NOTHING )
		{
			// but Skyrider notices them
			if ( DoesSkyriderNoticeEnemiesInSector( ubNumEnemies ) == TRUE )
			{
				// if just passing through (different quotes are used below if it's his final destination)
				if( !EndOfHelicoptersPath( ) && gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
				{
					// stop time compression and inform player that there are enemies in the sector below
					StopTimeCompression();

					if( Random( 2 ) )
					{
						HeliCharacterDialogue( pSkyRider, ENEMIES_SPOTTED_EN_ROUTE_IN_FRIENDLY_SECTOR_A );
					}
					else
					{
						HeliCharacterDialogue( pSkyRider, ENEMIES_SPOTTED_EN_ROUTE_IN_FRIENDLY_SECTOR_B );
					}
				}

				// make their presence appear on the map while Skyrider remains in the sector
				SectorInfo[ SECTOR( sX, sY ) ].uiFlags |= SF_SKYRIDER_NOTICED_ENEMIES_HERE;
			}
		}
	}


	// player pays for travel if Skyrider is NOT returning to base (even if empty while scouting/going for pickup)
	if ( fHeliReturnStraightToBase == FALSE || gGameExternalOptions.fHelicopterReturnToBaseIsNotFree )
	{
		// charge cost for flying another sector
		iTotalAccumulatedCostByPlayer += GetCostOfPassageForHelicopter( sX, sY );
	}

	// accumulate distance travelled
	AddSectorToHelicopterDistanceTravelled( );


	// check if heli has any real path left
	if( EndOfHelicoptersPath( ) )
	{
		// start hovering
		StartHoverTime( );

		// if sector is safe, or Skyrider MUST land anyway (returning to base)
		if ( ( ubNumEnemies == 0 ) || fHeliReturnStraightToBase )
		{
			// if he has passengers, or he's not going straight to base, tell player he's arrived
			// (i.e. don't say anything nor stop time compression if he's empty and just returning to base)
			if ( ( GetNumberOfPassengersInHelicopter() > 0 ) /*|| !fHeliReturnStraightToBase*/ )
			{
				// arrived at destination
				if(gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE) HeliCharacterDialogue( pSkyRider, ARRIVED_IN_NON_HOSTILE_SECTOR );
				//StopTimeCompression();
			}

			// destination reached, payment due.	If player can't pay, mercs get kicked off and heli flies to base!
			if( gGameExternalOptions.fPaySkyriderInBase == FALSE || CheckForArrivalAtRefuelPoint( ) )
				PaySkyriderBill();

			// WANNE: Fix by Headrock: Always stop time!
			StopTimeCompression();
		}
		else
		{
			// Say quote: "Gonna have to abort.	Enemies below"
			//CHRISL: Whether skyrider is set to silent or not, we should still say this if we aren't allowing landing in a hot lz.
			//if(gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE) 
			if(gGameExternalOptions.ubSkyriderHotLZ == 0)
				HeliCharacterDialogue( pSkyRider, ARRIVED_IN_HOSTILE_SECTOR );
			else
				HeliCharacterDialogue( pSkyRider, HELI_HOT_DROP );
			
			if( gGameExternalOptions.fPaySkyriderInBase == FALSE || CheckForArrivalAtRefuelPoint( ) )
				PaySkyriderBill();
			StopTimeCompression();
		}

		if( CheckForArrivalAtRefuelPoint( ) )
		{
			ReFuelHelicopter( );
			OfferHelicopterRepair();
		}
	}

	CheckIfHelicopterHasEnoughFuelToReturn( sX, sY );

	return( FALSE );
}

BOOLEAN CheckIfHelicopterHasEnoughFuelToReturn( INT16 sX, INT16 sY )
{
	// anv: check if we didn't come too far
	if( !CheckForArrivalAtRefuelPoint() && fHeliReturnStraightToBase == FALSE && gHelicopterSettings.ubHelicopterDistanceWithoutRefuel - iTotalHeliDistanceSinceRefuel < DistanceToNearestRefuelPoint(sX, sY) )
	{
		// hovered too long, inform player heli is returning to base
		HeliCharacterDialogue( pSkyRider, RETURN_TO_BASE );

		// If the sector is safe
		if ( NumNonPlayerTeamMembersInSector( pVehicleList[iHelicopterVehicleId].sSectorX, pVehicleList[iHelicopterVehicleId].sSectorY, ENEMY_TEAM ) == 0 )
		{
			// kick everyone out!
			// anv: possibly show prompt, if option is on, and heli has any passengers
			if( gHelicopterSettings.fAskBeforeKickingPassengersOut == FALSE || GetNumberInVehicle( iHelicopterVehicleId ) == 0 )
			{
				MoveAllInHelicopterToFootMovementGroup( );
				MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_LOW_FUEL );
			}
			else
				DoMessageBox( MSG_BOX_BASIC_STYLE, pHelicopterEtaStrings[ STR_HELI_ETA_KICK_OUT_PASSENGERS_PROMPT ], guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, KickOutPassengersBoxCallBack, NULL );
		}
		else
			MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_LOW_FUEL );

		return ( FALSE );
	}
	return ( TRUE );
}

INT32 GetTotalDistanceHelicopterCanTravel( void )
{
	return( gHelicopterSettings.ubHelicopterDistanceWithoutRefuel );
}

INT32 HowFarHelicopterhasTravelledSinceRefueling( void )
{
	// return total distance
	return( iTotalHeliDistanceSinceRefuel );
}

INT32 HowFurtherCanHelicopterTravel( void )
{
	// how many sectors further can we go on remaining fuel?
	return( gHelicopterSettings.ubHelicopterDistanceWithoutRefuel - ( HowFarHelicopterhasTravelledSinceRefueling( ) + DistanceOfIntendedHelicopterPath( ) ) );
}

void AddSectorToHelicopterDistanceTravelled( void )
{
	// up the distance
	iTotalHeliDistanceSinceRefuel++;

	//reset hover time
	uiStartHoverTime = 0;

	return;
}


INT32 LocationOfNearestRefuelPoint( BOOLEAN fNotifyPlayerIfNoSafeLZ, UINT8 ubReturnReason, INT16 sX, INT16 sY )
{
	INT32 iClosestLocation = -1;

	// anv: update availability before check
	UpdateRefuelSiteAvailability();
	// try to find one, any one under the players control
	iClosestLocation = FindLocationOfClosestRefuelSite( TRUE, sX, sY );

	// no go?...then find
	if( iClosestLocation == -1 )
	{
		if( fNotifyPlayerIfNoSafeLZ )
		{
			switch( ubReturnReason ) 
			{
				case HELICOPTER_RETURN_REASON_LOW_FUEL:
					// no refueling sites available, might wanna warn player about this
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pHelicopterEtaStrings[ STR_HELI_ETA_LOW_ON_FUEL_HOSTILE_TERRITORY ] );
					break;
				case HELICOPTER_RETURN_REASON_DAMAGE:
					// no refueling sites available, might wanna warn player about this
					ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pHelicopterEtaStrings[ STR_HELI_ETA_HELI_DAMAGED_HOSTILE_TERRITORY ] );
					break;
			}

		}

		// find the closest location regardless
		iClosestLocation = FindLocationOfClosestRefuelSite( FALSE, sX, sY );
	}

	// always returns a valid refuel point, picking a hostile one if unavoidable
	Assert( iClosestLocation != -1 );

	return( iClosestLocation );
}

INT32 FindLocationOfClosestRefuelSite( BOOLEAN fMustBeAvailable, INT16 sX, INT16 sY )
{
	INT32 iShortestDistance = 9999;
	INT32 iCounter = 0;
	INT32 iDistance = 9999;
	INT32 iClosestLocation = -1;

	// find shortest distance to refuel site
	for( iCounter = 0; iCounter < NUMBER_OF_REFUEL_SITES; iCounter++ )
	{
		// if this refuelling site is available
		if( ( fRefuelingSiteAvailable[ iCounter ] ) || ( fMustBeAvailable == FALSE && !( ( iCounter == ESTONI_REFUELING_SITE ) && ( CheckFact( FACT_ESTONI_REFUELLING_POSSIBLE, 0 ) == FALSE ) ) ) )
		{
			// find if sector is under control, find distance from heli to it
			iDistance = ( INT32 )FindStratPath( ( INT16 )( CALCULATE_STRATEGIC_INDEX( sX , sY ) ), ( INT16 )( CALCULATE_STRATEGIC_INDEX( sRefuelSectorX[ iCounter ], sRefuelSectorY[ iCounter ] ) ) , pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE );

			if( iDistance < iShortestDistance )
			{
				// shorter, copy over
				iShortestDistance = iDistance;
				iClosestLocation = iCounter;
			}
		}
	}

	// return the location
	return( iClosestLocation );
}


INT32 DistanceToNearestRefuelPoint( INT16 sX, INT16 sY )
{
	INT32 iClosestLocation;
	INT32 iDistance;

	// don't notify player during these checks!
	iClosestLocation = LocationOfNearestRefuelPoint( FALSE, HELICOPTER_RETURN_REASON_NONE, sX, sY );

	iDistance = ( INT32 )FindStratPath( ( INT16 )( CALCULATE_STRATEGIC_INDEX( sX, sY ) ), ( INT16 )( CALCULATE_STRATEGIC_INDEX( sRefuelSectorX[ iClosestLocation ], sRefuelSectorY[ iClosestLocation ] ) ) , pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE );
	return( iDistance );
}



BOOLEAN IsSectorOutOfTheWay( INT16 sX, INT16 sY )
{
	// check distance to nearest refuel point
	if( DistanceToNearestRefuelPoint( sX, sY ) > HowFurtherCanHelicopterTravel( ) )
	{
		return( TRUE );
	}


	return( FALSE );
}



void ReFuelHelicopter( void )
{
	// land, pay the man, and refuel

	LandHelicopter( );

	if(gGameExternalOptions.fAlternativeHelicopterFuelSystem)
	{
		gubHelicopterTimeToFullRefuel = ( gHelicopterSettings.ubHelicopterRefuelTime * ( iTotalHeliDistanceSinceRefuel ) ) / gHelicopterSettings.ubHelicopterDistanceWithoutRefuel;
		//AddStrategicEvent( EVENT_HELICOPTER_DONE_REFUELING, GetWorldTotalMin() + gHelicopterSettings.ubHelicopterRefuelTime, 0 );
		AddStrategicEvent( EVENT_HELICOPTER_REFUEL_FOR_A_MINUTE, GetWorldTotalMin() + 1, 0 );
		// reset distance traveled
		//iTotalHeliDistanceSinceRefuel = 0;
		// reset hover time
		gubHelicopterHoverTime = 0;
	}
	else
	{
		// refuel instantly
		iTotalHeliDistanceSinceRefuel = 0;
		AddStrategicEvent( EVENT_HELICOPTER_REFUEL_FOR_A_MINUTE, GetWorldTotalMin(), 0 );
	}

	return;
}

void ReFuelHelicopterForAMinute( void )
{
	if( CheckForArrivalAtRefuelPoint() == FALSE )
	{
		return;
	}
	if( iTotalHeliDistanceSinceRefuel <= 0 )
	{
		gubHelicopterTimeToFullRefuel = 0;
		iTotalHeliDistanceSinceRefuel = 0;
		ScreenMsg( FONT_GRAY2, MSG_INTERFACE, L"%s", pHelicopterRepairRefuelStrings[ STR_HELI_RR_REFUEL_FINISHED ]);
		
	}
	else
	{
		gubHelicopterTimeToFullRefuel--;	
		iTotalHeliDistanceSinceRefuel =  ( gubHelicopterTimeToFullRefuel * gHelicopterSettings.ubHelicopterDistanceWithoutRefuel ) / gHelicopterSettings.ubHelicopterRefuelTime ;

		AddStrategicEvent( EVENT_HELICOPTER_REFUEL_FOR_A_MINUTE, GetWorldTotalMin() + 1, 0 );
	}

	return;
}

void OfferHelicopterRepair( void )
{
	CHAR16	sHelicopterRepairPromptText[ 320 ];
	if( !gGameExternalOptions.fAllowWaldoToOfferRepairInStrategic )
	{
		return;
	}
	if( CheckFact(FACT_HELI_DAMAGED_CAN_START_REPAIR, 0 ) == TRUE )
	{
		if( gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
		{		
			CharacterDialogue( WALDO, WALDO_REPAIR_PROPOSITION, uiExternalStaticNPCFaces[ WALDO_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
			// popup will be called from HandleDialogueEnd()
		}
		else
		{
			// no dialogue, show popup right away
			swprintf( sHelicopterRepairPromptText, pHelicopterRepairRefuelStrings[ STR_HELI_RR_REPAIR_PROMPT ], gMercProfiles[ WALDO ].zNickname, CalculateHelicopterRepairCost( FALSE ), gHelicopterSettings.ubHelicopterBasicRepairTime );
			DoMessageBox( MSG_BOX_BASIC_STYLE, sHelicopterRepairPromptText, guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, OfferHelicopterRepairBoxCallBack, NULL );
		}
	}
	else if( CheckFact(FACT_HELI_SERIOUSLY_DAMAGED_CAN_START_REPAIR, 0 ) == TRUE )
	{
		if( gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
		{		
			CharacterDialogue( WALDO, WALDO_SERIOUS_REPAIR_PROPOSITION, uiExternalStaticNPCFaces[ WALDO_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
		}
		else
		{
			swprintf( sHelicopterRepairPromptText, pHelicopterRepairRefuelStrings[ STR_HELI_RR_REPAIR_PROMPT ], gMercProfiles[ WALDO ].zNickname,  CalculateHelicopterRepairCost( TRUE ), gHelicopterSettings.ubHelicopterSeriousRepairTime );
			DoMessageBox( MSG_BOX_BASIC_STYLE, sHelicopterRepairPromptText, guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, OfferHelicopterRepairBoxCallBack, NULL );
		}
	}

	return;
}

void OfferHelicopterRepairBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		StartHelicopterRepair( TRUE, FALSE );
	}
	else
	{
		if( gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
			CharacterDialogue( WALDO, WALDO_REPAIR_REFUSED, uiExternalStaticNPCFaces[ WALDO_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
	}
	return;
}

void KickOutPassengersBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		MoveAllInHelicopterToFootMovementGroup();
	}
	else
	{
	}
	MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_LOW_FUEL );
	return;
}

UINT16 CalculateHelicopterRepairCost( BOOLEAN fSeriousRepair )
{
	if( gGameExternalOptions.fWaldoSubsequentRepairsIncreaseCosts )
	{
		if( fSeriousRepair )
		{
			return min( gHelicopterSettings.usHelicopterSeriousRepairCost + gHelicopterSettings.usHelicopterSeriousCostIncreaseAfterBasicRepair * gubHelicopterBasicRepairsSoFar +
			+ gHelicopterSettings.usHelicopterSeriousCostIncreaseAfterSeriousRepair * gubHelicopterSeriousRepairsSoFar, gHelicopterSettings.usHelicopterSeriousRepairCostMax );
		}
		else
		{
			return min( gHelicopterSettings.usHelicopterBasicRepairCost + gHelicopterSettings.usHelicopterBasicCostIncreaseAfterBasicRepair * gubHelicopterBasicRepairsSoFar +
			+ gHelicopterSettings.usHelicopterBasicCostIncreaseAfterSeriousRepair * gubHelicopterSeriousRepairsSoFar, gHelicopterSettings.usHelicopterBasicRepairCostMax );
		}

	}
	else
	{
		if( fSeriousRepair )
		{
			return min( gHelicopterSettings.usHelicopterSeriousRepairCost, gHelicopterSettings.usHelicopterSeriousRepairCostMax );
		}
		else
		{
			return min( gHelicopterSettings.usHelicopterBasicRepairCost, gHelicopterSettings.usHelicopterBasicRepairCostMax );
		}
				
	}
}

void StartHelicopterRepair( BOOLEAN fInStrategic, BOOLEAN fCalledByGivingMoney )
{
	MoveAllInHelicopterToFootMovementGroup( );	
	if( CheckFact( FACT_HELI_DAMAGED_CAN_START_REPAIR, 0 ) == TRUE )
	{
		gubHelicopterHoursToRepair = min( max( 1, ( gHelicopterSettings.ubHelicopterBasicRepairTime - gHelicopterSettings.ubHelicopterBasicRepairTimeVariation + 2*Random(gHelicopterSettings.ubHelicopterBasicRepairTimeVariation) ) ), 255 );
		AddTransactionToPlayersBook( PAYMENT_TO_NPC, WALDO, GetWorldTotalMin(), -CalculateHelicopterRepairCost(FALSE) );
		AddHistoryToPlayersLog( HISTORY_HELICOPTER_REPAIR_STARTED, gHelicopterSettings.ubHelicopterBasicRepairTime, GetWorldTotalMin(), pVehicleList[ iHelicopterVehicleId ].sSectorX, pVehicleList[ iHelicopterVehicleId ].sSectorY );
		gubHelicopterBasicRepairsSoFar++;
	}
	else if( CheckFact( FACT_HELI_SERIOUSLY_DAMAGED_CAN_START_REPAIR, 0 ) == TRUE )
	{
		gubHelicopterHoursToRepair = min( max( 1, ( gHelicopterSettings.ubHelicopterSeriousRepairTime - gHelicopterSettings.ubHelicopterSeriousRepairTimeVariation + 2*Random(gHelicopterSettings.ubHelicopterSeriousRepairTimeVariation) ) ), 255 );
		AddTransactionToPlayersBook( PAYMENT_TO_NPC, WALDO, GetWorldTotalMin(), -CalculateHelicopterRepairCost(TRUE) );
		AddHistoryToPlayersLog( HISTORY_HELICOPTER_REPAIR_STARTED, gHelicopterSettings.ubHelicopterSeriousRepairTime, GetWorldTotalMin(), pVehicleList[ iHelicopterVehicleId ].sSectorX, pVehicleList[ iHelicopterVehicleId ].sSectorY );
		gubHelicopterSeriousRepairsSoFar++;
	}

	if( gubHelicopterHoursToRepair > 18 && gubHelicopterHoursToRepair < 30 )
	{
		if( fInStrategic == TRUE )
		{
			if( gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
				CharacterDialogue( WALDO, WALDO_COME_BACK_TOMORROW, uiExternalStaticNPCFaces[ WALDO_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
		}
		else
		{
			if( !fCalledByGivingMoney )
				TriggerNPCRecord( WALDO, 15 );
		}
	}
	else
	{
		if( fInStrategic == TRUE )
		{
			if( gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
				CharacterDialogue( WALDO, WALDO_COME_BACK_IN_SOME_TIME, uiExternalStaticNPCFaces[ WALDO_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
		}
		else
		{
			if( !fCalledByGivingMoney )
				TriggerNPCRecord( WALDO, 16 );
		}
	}
	return;
}

void FinishHelicopterRepair()
{
	gubHelicopterHoursToRepair = 0;
	gubHelicopterHitsTaken = 0;
	if( gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE )
		CharacterDialogue( WALDO, WALDO_REPAIR_COMPLETED, uiExternalStaticNPCFaces[ WALDO_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
	ScreenMsg( FONT_GRAY2, MSG_INTERFACE, L"%s", pHelicopterRepairRefuelStrings[ STR_HELI_RR_REPAIR_FINISHED ]);
	return;
}

INT32 GetCostOfPassageForHelicopter( INT16 sX, INT16 sY )
{
	// check if sector is air controlled or not, if so, then normal cost, otherwise increase the cost
	INT32 iCost = 0;

	// HEADROCK HAM 3.5: Costs externalized
	// if they don't control it
	if ( !(StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].usAirType & AIRSPACE_ENEMY_ACTIVE) )
	{
		iCost = gGameExternalOptions.usHelicopterBaseCostPerGreenTile;
	}
	else
	{
		iCost = gGameExternalOptions.usHelicopterBaseCostPerRedTile;
	}

	// HEADROCK HAM 3.5: Apply facility-based modifiers from global integer recalculated hourly
	iCost = __max(0, iCost + gsSkyriderCostModifier);

	return( iCost );
}


void SkyriderDestroyed( void )
{
	// remove any arrival events for the helicopter's group
	DeleteStrategicEvent( EVENT_GROUP_ARRIVAL, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

	// kill eveyone on board
	KillAllInVehicle( iHelicopterVehicleId );

	// kill skyrider
	fSkyRiderAvailable = FALSE;
	SoldierSkyRider.stats.bLife = 0;
	gMercProfiles[ SKYRIDER ].bLife = 0;

	// heli no longer available
	fHelicopterAvailable = FALSE;

	// destroy helicopter
	fHelicopterDestroyed = TRUE;

	// zero out balance due
	gMercProfiles[ SKYRIDER ].iBalance = 0;
	iTotalHeliDistanceSinceRefuel = 0;
	iTotalAccumulatedCostByPlayer = 0;

	// remove vehicle and reset
	RemoveVehicleFromList( iHelicopterVehicleId );
	iHelicopterVehicleId = -1;

	return;
}


BOOLEAN CanHelicopterFly( void )
{
	// check if heli is available for flight?

	// is the heli available
	if( fHelicopterAvailable == FALSE )
	{
		return( FALSE );
	}

	if( VehicleIdIsValid( iHelicopterVehicleId ) == FALSE )
	{
		return( FALSE );
	}

	// travelled too far?
	if( gGameExternalOptions.fAlternativeHelicopterFuelSystem )
	{
		if( iTotalHeliDistanceSinceRefuel > gHelicopterSettings.ubHelicopterDistanceWithoutRefuel )
		{
			return( FALSE );
		}
	}

	// is the pilot alive, well, and willing to help us?
	if( IsHelicopterPilotAvailable( ) == FALSE )
	{
		return( FALSE );
	}

	if( fHeliReturnStraightToBase == TRUE )
	{
		return ( FALSE );
	}

	// grounded by enemies in sector?
	if ( CanHelicopterTakeOff() == FALSE )
	{
		return ( FALSE );
	}

	// repair in progress?
	if( gubHelicopterHoursToRepair > 0 )
	{
		return ( FALSE );
	}

	// everything A-OK!
	return( TRUE );
}

BOOLEAN IsHelicopterPilotAvailable( void )
{
	// what is state of skyrider?
	if( fSkyRiderAvailable == FALSE )
	{
		return( FALSE );
	}

	// owe any money to skyrider?
	if( gMercProfiles[ SKYRIDER ].iBalance < 0 )
	{
		return( FALSE );
	}

	// Drassen too disloyal to wanna help player?
	if ( CheckFact( FACT_LOYALTY_LOW, SKYRIDER ) )
	{
		return ( FALSE );
	}

	// anv - helicopter too damaged?
	if ( gGameExternalOptions.fSeriouslyDamagedSkyriderWontFly == TRUE && gubHelicopterHitsTaken > 1 )
	{
		return ( FALSE );
	}

	return( TRUE );
}


void LandHelicopter( void )
{
	// set the helictoper down, call arrive callback for this mvt group
	fHelicopterIsAirBorne = FALSE;

	// no longer hovering
	fHoveringHelicopter = FALSE;

	// reset fact that we might have returned straight here
	fHeliReturnStraightToBase = FALSE;

	HandleHelicopterOnGroundGraphic( );
	HandleHelicopterOnGroundSkyriderProfile( );

	// if we'll be unable to take off again (because there are enemies in the sector, or we owe pilot money)
	if ( CanHelicopterFly() == FALSE )
	{
		// kick everyone out!
		MoveAllInHelicopterToFootMovementGroup( );
	}
	else
	{
#ifdef JA2UB
		Assert( 0 );
//No meanwhiles
#else
		// play meanwhile scene if it hasn't been used yet
		HandleKillChopperMeanwhileScene();
#endif
	}

	// Flugente: once the AI 'learns' of the player using helis, it wants some for itself
	SetASDFlag( ASDFACT_HELI_UNLOCKED );
}


void TakeOffHelicopter( void )
{
	// heli in the air
	fHelicopterIsAirBorne = TRUE;

	// no longer hovering
	fHoveringHelicopter = FALSE;

	HandleHelicopterOnGroundGraphic( );
	HandleHelicopterOnGroundSkyriderProfile( );
}



void StartHoverTime( void )
{
	// start hover in this sector
	fHoveringHelicopter = TRUE;

	// post event for x mins in future, save start time, if event time - delay = start time, then hover has gone on too long
	uiStartHoverTime = GetWorldTotalMin( );

	if( gGameExternalOptions.fAlternativeHelicopterFuelSystem )
	{
		AddStrategicEvent( EVENT_HELICOPTER_HOVER_FOR_A_MINUTE, GetWorldTotalMin() + 1, 0 );
	}
	else
	{
		// post event..to call handle hover
		AddStrategicEvent( EVENT_HELICOPTER_HOVER_TOO_LONG, GetWorldTotalMin() + gHelicopterSettings.ubHelicopterTimeDelayForHoverWait, 0 );
	}

	return;
}

void HandleHeliHoverForAMinute( void )
{
	INT16 sX = pVehicleList[ iHelicopterVehicleId ].sSectorX;
	INT16 sY = pVehicleList[ iHelicopterVehicleId ].sSectorY;
	if( fHoveringHelicopter == FALSE )
	{
		return;
	}
	gubHelicopterHoverTime++;
	if( gubHelicopterHoverTime == 10 )
	{
		gubHelicopterHoverTime = 0;

		iTotalHeliDistanceSinceRefuel++;
		if ( !(StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].usAirType & AIRSPACE_ENEMY_ACTIVE) )
		{
			iTotalAccumulatedCostByPlayer += gGameExternalOptions.usHelicopterHoverCostOnGreenTile;
		}
		else
		{
			iTotalAccumulatedCostByPlayer += gGameExternalOptions.usHelicopterHoverCostOnRedTile;
		}
		
		if( CheckIfHelicopterHasEnoughFuelToReturn( sX, sY ) == TRUE )
		{
			// since now heli can stay hovering longer, reminder every 10 minutes would get irritating
			if( iTotalHeliDistanceSinceRefuel % 3 == 0 && iTotalHeliDistanceSinceRefuel < gHelicopterSettings.ubHelicopterDistanceWithoutRefuel )
				// inform player
				HeliCharacterDialogue( pSkyRider, HOVERING_A_WHILE );
		}
	
	}
	AddStrategicEvent( EVENT_HELICOPTER_HOVER_FOR_A_MINUTE, GetWorldTotalMin() + 1, 0 );
}

void HandleHeliHoverLong( void )
{
	// post message about hovering too long
	if( fHoveringHelicopter )
	{
		// proper event, post next one
		AddStrategicEvent( EVENT_HELICOPTER_HOVER_WAY_TOO_LONG, uiStartHoverTime + gHelicopterSettings.ubHelicopterTimeDelayForHoverWaitTooLong, 0 );

		// inform player
		HeliCharacterDialogue( pSkyRider, HOVERING_A_WHILE );

		// stop time compression if it's on
		StopTimeCompression( );
	}
	else
	{
		// reset
		uiStartHoverTime = 0;
	}
}


void HandleHeliHoverTooLong( void )
{
	// reset hover time
	uiStartHoverTime = 0;

	if( fHoveringHelicopter == FALSE )
	{
		return;
	}


	// hovered too long, inform player heli is returning to base
	HeliCharacterDialogue( pSkyRider, RETURN_TO_BASE );

	// If the sector is safe
	if ( NumNonPlayerTeamMembersInSector( pVehicleList[iHelicopterVehicleId].sSectorX, pVehicleList[iHelicopterVehicleId].sSectorY, ENEMY_TEAM ) == 0 )
	{
		// anv: possibly show prompt, if option is on, and heli has any passengers
		if( gHelicopterSettings.fAskBeforeKickingPassengersOut == FALSE || GetNumberInVehicle( iHelicopterVehicleId ) == 0 )
		{
			MoveAllInHelicopterToFootMovementGroup( );
			MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_LOW_FUEL );
		}
		else
			DoMessageBox( MSG_BOX_BASIC_STYLE, pHelicopterEtaStrings[ STR_HELI_ETA_KICK_OUT_PASSENGERS_PROMPT ], guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, KickOutPassengersBoxCallBack, NULL );
	}
	else
		MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_LOW_FUEL );
}


// check if anyone in the chopper sees any baddies in sector
BOOLEAN DoesSkyriderNoticeEnemiesInSector( UINT8 ubNumEnemies )
{
	UINT8 ubChance;

	// is the pilot and heli around?
	if( CanHelicopterFly( ) == FALSE )
	{
		return( FALSE );
	}

	// if there aren't any, he obviously won't see them
	if( ubNumEnemies == 0 )
	{
		return( FALSE );
	}

	// figure out what the chance is of seeing them
	// make this relatively accurate most of the time, to encourage helicopter scouting by making it useful
	ubChance = 60 + ubNumEnemies;

	if( PreRandom( 100 ) < ubChance )
	{
		return( TRUE );
	}

	return( FALSE );
}


// if the heli is on the move, what is the distance it will move..the length of the merc path, less the first node
INT32 DistanceOfIntendedHelicopterPath( void )
{
	PathStPtr pNode = NULL;
	INT32 iLength = 0;

	if( CanHelicopterFly( ) == FALSE )
	{
		// big number, no go
		return( 9999 );
	}

	pNode = pVehicleList[ iHelicopterVehicleId ].pMercPath;

	// any path yet?
	if( pNode != NULL )
	{
		while( pNode->pNext )
		{
			iLength++;
			pNode = pNode->pNext;
		}
	}

	pNode = MoveToBeginningOfPathList( pTempHelicopterPath );

	// any path yet?
	if( pNode != NULL )
	{
		while( pNode->pNext )
		{
			iLength++;
			pNode = pNode->pNext;
		}
	}

	return( iLength );
}



BOOLEAN CheckForArrivalAtRefuelPoint( void )
{
	// check if this is our final destination
	if ( GetLengthOfPath( pVehicleList[ iHelicopterVehicleId ].pMercPath ) > 0 )
	{
		return( FALSE );
	}

	// check if we're at a refuel site
	if( DistanceToNearestRefuelPoint( pVehicleList[ iHelicopterVehicleId ].sSectorX, pVehicleList[ iHelicopterVehicleId ].sSectorY ) > 0 )
	{
		// not at a refuel point
		return( FALSE );
	}

	// we are at a refuel site
	return( TRUE );
}



void SetUpHelicopterForMovement( void )
{
	// check if helicopter vehicle has a mvt group, if not, assign one in this sector
	INT32 iCounter = 0;

	// if no group, create one for vehicle
	if( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup == 0 )
	{
		// get the vehicle a mvt group
		pVehicleList[ iHelicopterVehicleId ].ubMovementGroup = CreateNewVehicleGroupDepartingFromSector( ( UINT8 )( pVehicleList[ iHelicopterVehicleId ].sSectorX ), ( UINT8 )( pVehicleList[ iHelicopterVehicleId ].sSectorY ), iHelicopterVehicleId );

		// add everyone in vehicle to this mvt group
		for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iHelicopterVehicleId ].ubVehicleType ].iNewSeatingCapacities; iCounter++ )
		{
			if( pVehicleList[ iHelicopterVehicleId ].pPassengers[ iCounter ] != NULL )
			{
				// add character
				AddPlayerToGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, pVehicleList[ iHelicopterVehicleId ].pPassengers[ iCounter ] );
			}
		}
	}
}


BOOLEAN HeliCharacterDialogue( SOLDIERTYPE *pSoldier, UINT16 usQuoteNum )
{
	// ARM: we could just return, but since various flags are often being set it's safer to honk so it gets fixed right!
	Assert( fSkyRiderAvailable );

	return( CharacterDialogue( SKYRIDER, usQuoteNum, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE ) );
}


INT32 GetNumberOfPassengersInHelicopter( void )
{
	INT32 iNumber = 0;

	if( iHelicopterVehicleId != -1 )
	{
		iNumber = GetNumberInVehicle( iHelicopterVehicleId );
	}

	return( iNumber );
}


BOOLEAN IsRefuelSiteInSector( INT16 sMapX, INT16 sMapY )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < NUMBER_OF_REFUEL_SITES; iCounter++ )
	{
		if ( ( sRefuelSectorX[ iCounter ] == sMapX ) && ( sRefuelSectorY[ iCounter ] == sMapY ) )
		{
			return(TRUE);
		}
	}

	return(FALSE);
}


void UpdateRefuelSiteAvailability( void )
{
	// Generally, only Drassen is initially available for refuelling
	// Estoni must first be captured (although player may already have it when he gets Skyrider!)
	// anv: Estoni should only available after Jake-Shank quest
	// site availability should not be dependent on SAM zones, or weird things will happen after controlling Estoni (like inability to land in base if it's in SAM zone)

	for ( INT32 iCounter = 0; iCounter < NUMBER_OF_REFUEL_SITES; ++iCounter )
	{
		// if enemy controlled sector (ground OR air, don't want to fly into enemy air territory)
		if( ( StrategicMap[ CALCULATE_STRATEGIC_INDEX( sRefuelSectorX[ iCounter ], sRefuelSectorY[ iCounter ] ) ].fEnemyControlled == TRUE ) ||
				( ( iCounter == ESTONI_REFUELING_SITE ) && ( CheckFact( FACT_ESTONI_REFUELLING_POSSIBLE, 0 ) == FALSE ) ) )
		{
			// mark refueling site as unavailable
			fRefuelingSiteAvailable[ iCounter ] = FALSE;
		}
		else
		{
			// mark refueling site as available
			fRefuelingSiteAvailable[ iCounter ] = TRUE;

			// reactivate a grounded helicopter, if here
			if ( !fHelicopterAvailable && !fHelicopterDestroyed && fSkyRiderAvailable && ( iHelicopterVehicleId != -1 ) )
			{
				if( ( pVehicleList[ iHelicopterVehicleId ].sSectorX == sRefuelSectorX[ iCounter ]) &&
					( pVehicleList[ iHelicopterVehicleId ].sSectorY == sRefuelSectorY[ iCounter ]) )
				{
					// no longer grounded
					DoScreenIndependantMessageBox( pSkyriderText[ 5 ], MSG_BOX_FLAG_OK, NULL );
				}
			}
		}
	}
}


void SetUpHelicopterForPlayer( INT16 sX, INT16 sY , UINT8 SkyDrive, UINT8 VehicleID )
{
	if( fSkyRiderSetUp == FALSE )
	{
		fHelicopterAvailable = TRUE;
		fSkyRiderAvailable = TRUE;

		iHelicopterVehicleId = AddVehicleToList( sX, sY, 0, VehicleID ); //HELICOPTER

		Assert( iHelicopterVehicleId != -1 );

		SoldierSkyRider.initialize();
		SoldierSkyRider.ubProfile = SkyDrive; //SKYRIDER;
		SoldierSkyRider.stats.bLife = 80;

		pSkyRider = &( SoldierSkyRider );

		// set up for movement
		SetUpHelicopterForMovement( );
		UpdateRefuelSiteAvailability( );

		fSkyRiderSetUp = TRUE;

		gMercProfiles[ SkyDrive ].fUseProfileInsertionInfo = FALSE; //SKYRIDER
	}

	return;
}


UINT8 MoveAllInHelicopterToFootMovementGroup( INT8 bNewSquad )
{
	// take everyone out of heli and add to movement group
	INT32 iCounter = 0;
	UINT8 ubGroupId = 0;
	SOLDIERTYPE *pSoldier;
	//INT8 bNewSquad;
	BOOLEAN fAnyoneAboard = FALSE;
	BOOLEAN fSuccess;
	UINT8	ubInsertionCode = 0;
	BOOLEAN fInsertionCodeSet = FALSE;
	UINT16	usInsertionData = 0;

	// put these guys on their own squad (we need to return their group ID, and can only return one, so they need a unique one
	// silversurfer: This function now accepts a preferred squad ID as parameter. If this squad is empty we will use it instead of the first empty squad.
	if ( !SquadIsEmpty( bNewSquad ) )
		bNewSquad = GetFirstEmptySquad();
	if ( bNewSquad == -1 )
	{
		return( 0 );
	}

	// go through list of everyone in helicopter
	for( iCounter = 0; iCounter < gNewVehicle[ pVehicleList[ iHelicopterVehicleId ].ubVehicleType ].iNewSeatingCapacities; ++iCounter )
	{
		// get passenger
		pSoldier = pVehicleList[ iHelicopterVehicleId ].pPassengers[ iCounter ];

		if( pSoldier != NULL )
		{
			// better really be in there!
			Assert ( pSoldier->bAssignment == VEHICLE );
			Assert ( pSoldier->iVehicleId == iHelicopterVehicleId );

			fAnyoneAboard = TRUE;

			fSuccess = RemoveSoldierFromHelicopter( pSoldier );
			Assert( fSuccess );

			AddCharacterToSquad( pSoldier, bNewSquad );

			// ATE: OK - the ubStrategicInsertionCode is set 'cause groupArrivesInsector has been
			// called when buddy is added to a squad. However, the insertion code onlt sets set for
			// the first merc, so the rest are going to use whatever they had previously....
			if ( !fInsertionCodeSet )
			{
				ubInsertionCode = pSoldier->ubStrategicInsertionCode;
				usInsertionData = pSoldier->usStrategicInsertionData;
				fInsertionCodeSet = TRUE;
			}
			else
			{
				pSoldier->ubStrategicInsertionCode = ubInsertionCode;
				pSoldier->usStrategicInsertionData = usInsertionData;
			}
				
			// Flugente: we are leaving the helicopter and instantly deploy into combat - this must be an airdrop
			pSoldier->usSoldierFlagMask |= (SOLDIER_AIRDROP_TURN|SOLDIER_AIRDROP);
		}
	}

	//Flugente:  we need to determine what direction th heli came from, so that we can play the correct animation
	UINT8 usDirection = NORTH;

	GROUP* pGroup = gpGroupList;
	while ( pGroup )
	{
		if ( pGroup->usGroupTeam == OUR_TEAM && IsGroupTheHelicopterGroup( pGroup ) ) // Group came from the examined source
		{
			if ( pGroup->ubSectorX == pGroup->ubPrevX - 1 && pGroup->ubSectorY == pGroup->ubPrevY )
			{
				usDirection = EAST;
			}
			else if ( pGroup->ubSectorX == pGroup->ubPrevX + 1 && pGroup->ubSectorY == pGroup->ubPrevY )
			{
				usDirection = WEST;
			}
			else if ( pGroup->ubSectorX == pGroup->ubPrevX && pGroup->ubSectorY == pGroup->ubPrevY - 1 )
			{
				usDirection = SOUTH;
			}

			SetHelicopterDropDirection( usDirection );

			break;
		}
		pGroup = pGroup->next;
	}

	SetHelicopterDropDirection( usDirection );

	if ( fAnyoneAboard )
	{
		ubGroupId = SquadMovementGroups[ bNewSquad ];
	}

	return( ubGroupId );
}



void SkyRiderTalk( UINT16 usQuoteNum )
{
	// have skyrider talk to player
	HeliCharacterDialogue( pSkyRider, usQuoteNum );

	fTeamPanelDirty = TRUE;

	return;
}


void HandleSkyRiderMonologueEvent( UINT32 uiEventCode, UINT32 uiSpecialCode )
{
	// will handle the skyrider monologue about where the SAM sites are and what not

	TurnOnAirSpaceMode();

	switch( uiEventCode )
	{
		case( SKYRIDER_MONOLOGUE_EVENT_DRASSEN_SAM_SITE ):
			SetExternMapscreenSpeechPanelXY( DEFAULT_EXTERN_PANEL_X_POS, 117 );
			HandleSkyRiderMonologueAboutDrassenSAMSite( uiSpecialCode );
			break;
		case SKYRIDER_MONOLOGUE_EVENT_CAMBRIA_HOSPITAL:
			SetExternMapscreenSpeechPanelXY( DEFAULT_EXTERN_PANEL_X_POS, 172 );
			HandleSkyRiderMonologueAboutCambriaHospital( uiSpecialCode );
			break;
		case( SKYRIDER_MONOLOGUE_EVENT_OTHER_SAM_SITES ):
			SetExternMapscreenSpeechPanelXY( 335, DEFAULT_EXTERN_PANEL_Y_POS );
			HandleSkyRiderMonologueAboutOtherSAMSites( uiSpecialCode );
			break;
		case( SKYRIDER_MONOLOGUE_EVENT_ESTONI_REFUEL ):
			SetExternMapscreenSpeechPanelXY( DEFAULT_EXTERN_PANEL_X_POS, DEFAULT_EXTERN_PANEL_Y_POS );
			HandleSkyRiderMonologueAboutEstoniRefuel( uiSpecialCode );
			break;
	}

	// update time
	guiTimeOfLastSkyriderMonologue = GetWorldTotalMin();
}


void HandleSkyRiderMonologueAboutEstoniRefuel( UINT32 uiSpecialCode )
{
	// once estoni is free tell player about refueling

	switch( uiSpecialCode )
	{
		case( 0 ):
			CharacterDialogueWithSpecialEvent( SKYRIDER, SPIEL_ABOUT_ESTONI_AIRSPACE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] , DIALOGUE_EXTERNAL_NPC_UI , FALSE , FALSE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_ESTONI_REFUEL, 1 );
			// if special event data 2 is true, then do dialogue, else this is just a trigger for an event
		CharacterDialogue( SKYRIDER, SPIEL_ABOUT_ESTONI_AIRSPACE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );

			CharacterDialogueWithSpecialEvent( SKYRIDER, SPIEL_ABOUT_ESTONI_AIRSPACE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] , DIALOGUE_EXTERNAL_NPC_UI , FALSE , FALSE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_ESTONI_REFUEL, 2 );
			break;

		case( 1 ):
			// highlight Estoni
			fShowEstoniRefuelHighLight = TRUE;
			break;

		case( 2 ):
			fShowEstoniRefuelHighLight = FALSE;
			break;
	}
	return;
}


void HandleSkyRiderMonologueAboutDrassenSAMSite( UINT32 uiSpecialCode )
{
	switch( uiSpecialCode )
	{
		case( 0 ):
			//gpCurrentTalkingFace = &gFacesData[ uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] ];
			//gubCurrentTalkingID = SKYRIDER;

			// if special event data 2 is true, then do dialogue, else this is just a trigger for an event
			CharacterDialogue( SKYRIDER, MENTION_DRASSEN_SAM_SITE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
			CharacterDialogueWithSpecialEvent( SKYRIDER, MENTION_DRASSEN_SAM_SITE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI , FALSE , TRUE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_DRASSEN_SAM_SITE, 1 );

			if( SAMSitesUnderPlayerControl( gpSamSectorX[1], gpSamSectorY[1] ) == FALSE )
			{
				CharacterDialogue( SKYRIDER, SECOND_HALF_OF_MENTION_DRASSEN_SAM_SITE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
			}
			else
			{
				// Ian says don't use the SAM site quote unless player has tried flying already
				if ( CheckFact( FACT_SKYRIDER_USED_IN_MAPSCREEN, SKYRIDER ) )
				{
					CharacterDialogue( SKYRIDER, SAM_SITE_TAKEN, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
					gfSkyriderSaidCongratsOnTakingSAM = TRUE;
				}
			}

			CharacterDialogueWithSpecialEvent( SKYRIDER, MENTION_DRASSEN_SAM_SITE, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI , FALSE , TRUE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_DRASSEN_SAM_SITE, 2 );
			break;

		case( 1 ):
			// highlight Drassen SAM site sector
			fShowDrassenSAMHighLight = TRUE;
			SetSAMSiteAsFound( SAM_SITE_TWO );
			break;

		case( 2 ):
			fShowDrassenSAMHighLight = FALSE;
			break;
	}
	return;
}


void HandleSkyRiderMonologueAboutCambriaHospital( UINT32 uiSpecialCode )
{
	switch( uiSpecialCode )
	{
		case( 0 ):
			//gpCurrentTalkingFace = &gFacesData[ uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] ];
			//gubCurrentTalkingID = SKYRIDER;

			// if special event data 2 is true, then do dialogue, else this is just a trigger for an event
			CharacterDialogue( SKYRIDER, MENTION_HOSPITAL_IN_CAMBRIA, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
			CharacterDialogueWithSpecialEvent( SKYRIDER, MENTION_HOSPITAL_IN_CAMBRIA, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI , FALSE , TRUE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_CAMBRIA_HOSPITAL, 1 );

			// highlight Drassen hospital sector
			fShowCambriaHospitalHighLight = TRUE;
			break;

		case( 1 ):
			fShowCambriaHospitalHighLight = FALSE;
			break;
	}
	return;
}


void HandleSkyRiderMonologueAboutOtherSAMSites( UINT32 uiSpecialCode )
{
	// handle skyrider telling player about other sam sites..on fifth hiring or after one near drassen is taken out

	switch( uiSpecialCode )
	{
		case( 0 ):
			// do quote 21
			gpCurrentTalkingFace = &gFacesData[ uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] ];
			gubCurrentTalkingID = SKYRIDER;

			// if special event data 2 is true, then do dialogue, else this is just a trigger for an event
			CharacterDialogue( SKYRIDER, SPIEL_ABOUT_OTHER_SAM_SITES, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
			CharacterDialogueWithSpecialEvent( SKYRIDER, SPIEL_ABOUT_OTHER_SAM_SITES, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] , DIALOGUE_EXTERNAL_NPC_UI , FALSE , FALSE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_OTHER_SAM_SITES, 1 );

			CharacterDialogue( SKYRIDER, SECOND_HALF_OF_SPIEL_ABOUT_OTHER_SAM_SITES, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ], DIALOGUE_EXTERNAL_NPC_UI, FALSE, FALSE );
			CharacterDialogueWithSpecialEvent( SKYRIDER, SPIEL_ABOUT_OTHER_SAM_SITES, uiExternalStaticNPCFaces[ SKYRIDER_EXTERNAL_FACE ] , DIALOGUE_EXTERNAL_NPC_UI , FALSE , FALSE , DIALOGUE_SPECIAL_EVENT_SKYRIDERMAPSCREENEVENT ,SKYRIDER_MONOLOGUE_EVENT_OTHER_SAM_SITES, 2 );

			break;

		case( 1 ):
			// highlight other SAMs
			fShowOtherSAMHighLight = TRUE;
			// reveal other 3 SAM sites
			SetSAMSiteAsFound( SAM_SITE_ONE );
			SetSAMSiteAsFound( SAM_SITE_THREE );
			SetSAMSiteAsFound( SAM_SITE_FOUR );
			break;

		case( 2 ):
			fShowOtherSAMHighLight = FALSE;
			break;
	}
	return;
}


void CheckAndHandleSkyriderMonologues( void )
{
	// wait at least this many days between Skyrider monologues
	if ( ( GetWorldTotalMin() - guiTimeOfLastSkyriderMonologue ) >= ( MIN_DAYS_BETWEEN_SKYRIDER_MONOLOGUES * 24 * 60 ) )
	{
		if( guiHelicopterSkyriderTalkState == 0 )
		{
			HandleSkyRiderMonologueEvent( SKYRIDER_MONOLOGUE_EVENT_DRASSEN_SAM_SITE, 0 );
			guiHelicopterSkyriderTalkState = 1;
		}
		else if( guiHelicopterSkyriderTalkState == 1 )
		{
			// if enemy still controls the Cambria hospital sector
			if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( gModSettings.ubHospitalSectorX, gModSettings.ubHospitalSectorY )].fEnemyControlled )
			{
				HandleSkyRiderMonologueEvent( SKYRIDER_MONOLOGUE_EVENT_CAMBRIA_HOSPITAL, 0 );
			}
			// advance state even if player already has Cambria's hospital sector!!!
			guiHelicopterSkyriderTalkState = 2;
		}
		else if( guiHelicopterSkyriderTalkState == 2 )
		{
			// wait until player has taken over a SAM site before saying this and advancing state
			if ( gfSkyriderSaidCongratsOnTakingSAM )
			{
				HandleSkyRiderMonologueEvent( SKYRIDER_MONOLOGUE_EVENT_OTHER_SAM_SITES, 0 );
				guiHelicopterSkyriderTalkState = 3;
			}
		}
		else if( guiHelicopterSkyriderTalkState == 3 )
		{
			// wait until Estoni refuelling site becomes available
			if ( fRefuelingSiteAvailable[ ESTONI_REFUELING_SITE ] )
			{
				HandleSkyRiderMonologueEvent( SKYRIDER_MONOLOGUE_EVENT_ESTONI_REFUEL, 0 );
				guiHelicopterSkyriderTalkState = 4;
			}
		}
	}
}


void HandleAnimationOfSectors( void )
{
	BOOLEAN fSkipSpeakersLocator = FALSE;
	// these don't need to be saved, they merely turn off the highlights after they stop flashing
	static BOOLEAN fOldShowDrassenSAMHighLight = FALSE;
	static BOOLEAN fOldShowCambriaHospitalHighLight = FALSE;
	static BOOLEAN fOldShowEstoniRefuelHighLight = FALSE;
	static BOOLEAN fOldShowOtherSAMHighLight = FALSE;

	// find out which mode we are in and animate for that mode

	// Drassen SAM site
	if( fShowDrassenSAMHighLight )
	{
		fOldShowDrassenSAMHighLight = TRUE;
		// Drassen's SAM site is #2
		HandleBlitOfSectorLocatorIcon( gpSamSectorX[1], gpSamSectorY[1], 0, LOCATOR_COLOR_RED );
		fSkipSpeakersLocator = TRUE;
	}
	else if( fOldShowDrassenSAMHighLight )
	{
		fOldShowDrassenSAMHighLight = FALSE;
		fMapPanelDirty = TRUE;
	}

	// Cambria hospital
	if( fShowCambriaHospitalHighLight )
	{
		fOldShowCambriaHospitalHighLight = TRUE;
		HandleBlitOfSectorLocatorIcon( gModSettings.ubHospitalSectorX, gModSettings.ubHospitalSectorY, 0, LOCATOR_COLOR_RED );
		fSkipSpeakersLocator = TRUE;
	}
	else if( fOldShowCambriaHospitalHighLight )
	{
		fOldShowCambriaHospitalHighLight = FALSE;
		fMapPanelDirty = TRUE;
	}

	// show other SAM sites
	if( fShowOtherSAMHighLight )
	{
		fOldShowOtherSAMHighLight = TRUE;
		HandleBlitOfSectorLocatorIcon( gpSamSectorX[0], gpSamSectorY[0], 0, LOCATOR_COLOR_RED );
		HandleBlitOfSectorLocatorIcon( gpSamSectorX[2], gpSamSectorY[2], 0, LOCATOR_COLOR_RED );
		HandleBlitOfSectorLocatorIcon( gpSamSectorX[3], gpSamSectorY[3], 0, LOCATOR_COLOR_RED );
		fSkipSpeakersLocator = TRUE;
	}
	else if( fOldShowOtherSAMHighLight )
	{
		fOldShowOtherSAMHighLight = FALSE;
		fMapPanelDirty = TRUE;
	}

	// show Estoni site
	if( fShowEstoniRefuelHighLight )
	{
		fOldShowEstoniRefuelHighLight = TRUE;
		HandleBlitOfSectorLocatorIcon( sRefuelSectorX[ ESTONI_REFUELING_SITE ], sRefuelSectorY[ ESTONI_REFUELING_SITE ], 0, LOCATOR_COLOR_RED );
		fSkipSpeakersLocator = TRUE;
	}
	else if( fOldShowEstoniRefuelHighLight )
	{
		fOldShowEstoniRefuelHighLight = FALSE;
		fMapPanelDirty = TRUE;
	}


	// don't show sector locator over the speaker's sector if he is talking about another sector - it's confusing
	if ( !fSkipSpeakersLocator )
	{
		switch( gubBlitSectorLocatorCode )
		{
			case LOCATOR_COLOR_RED:		// normal one used for mines (will now be overriden with yellow)
				HandleBlitOfSectorLocatorIcon( gsSectorLocatorX, gsSectorLocatorY, 0, LOCATOR_COLOR_RED );
				break;
			case LOCATOR_COLOR_YELLOW: // used for all other dialogues
				HandleBlitOfSectorLocatorIcon( gsSectorLocatorX, gsSectorLocatorY, 0, LOCATOR_COLOR_YELLOW );
				break;
		}
	}

	return;
}

INT16 LastSectorInHelicoptersPath( void )
{
	// get the last sector value in the helictoper's path
	PathStPtr pNode = NULL;
	UINT32 uiLocation = 0;

	// if the heli is on the move, what is the distance it will move..the length of the merc path, less the first node
	if( CanHelicopterFly( ) == FALSE )
	{
		// big number, no go
		return( 0 );
	}

	uiLocation = pVehicleList[ iHelicopterVehicleId ].sSectorX + pVehicleList[ iHelicopterVehicleId ].sSectorY * MAP_WORLD_X;

	pNode = pVehicleList[ iHelicopterVehicleId ].pMercPath;

	// any path yet?
	if( pNode != NULL )
	{
		while( pNode)
		{
			uiLocation = pNode->uiSectorId;

			pNode = pNode->pNext;
		}
	}

	pNode = MoveToBeginningOfPathList( pTempHelicopterPath );
	// any path yet?
	if( pNode != NULL )
	{
		while( pNode )
		{
			uiLocation = pNode->uiSectorId;

			pNode = pNode->pNext;
		}
	}

	return( ( INT16 )uiLocation );
}


/*
INT32 GetTotalCostOfHelicopterTrip( void )
{
	// get cost of helicopter trip

	PathStPtr pNode = NULL, pTempNode = NULL;
	UINT32 uiCost = 0;
	UINT32 uiLastTempPathSectorId = 0;
	UINT32 iClosestRefuelPoint = 0;
	UINT32 uiStartSectorNum = 0;
	UINT32 uiLength = 0;

	// if the heli is on the move, what is the distance it will move..the length of the merc path, less the first node
	if( CanHelicopterFly( ) == FALSE )
	{
		// big number, no go
		return( 0 );
	}

	pNode = pVehicleList[ iHelicopterVehicleId ].pMercPath;

	// any path yet?
	uiLastTempPathSectorId = pVehicleList[ iHelicopterVehicleId ].sSectorX + pVehicleList[ iHelicopterVehicleId ].sSectorY * MAP_WORLD_X;
	uiStartSectorNum = uiLastTempPathSectorId;

	if( pNode )
	{
		pNode = pNode->pNext;
	}

	if( pNode != NULL )
	{
		while( pNode)
		{
			if( uiLength == 0 )
			{
				if( pNode->pNext )
				{
					if( uiLastTempPathSectorId == pNode->pNext->uiSectorId )
					{
						// do nothing
					}
					else
					{
						uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
					}
				}
				else
				{
					uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
				}
			}
			else
			{
				uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
			}

			uiLength++;

			uiLastTempPathSectorId = pNode->uiSectorId;
			pNode = pNode->pNext;
		}
	}


	pNode = NULL;

	if( pTempHelicopterPath )
	{
		pNode = MoveToBeginningOfPathList( pTempHelicopterPath );
	}

	if( pNode )
	{
		pNode = pNode->pNext;
	}

	// any path yet?
	if( pNode != NULL )
	{
		while( pNode )
		{
			if( uiLength == 0 )
			{
				if( pNode->pNext )
				{
					if( uiLastTempPathSectorId == pNode->pNext->uiSectorId )
					{
						// do nothing
					}
					else
					{
						uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
					}
				}
				else
				{
					uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
				}
			}
			else
			{
				uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
			}

			uiLength++;

			//uiCost += GetCostOfPassageForHelicopter( ( UINT16 ) ( pNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pNode->uiSectorId / MAP_WORLD_X ) );
			uiLastTempPathSectorId = pNode->uiSectorId;
			pNode = pNode->pNext;
		}
	}

	iClosestRefuelPoint = ( INT16 )( CALCULATE_STRATEGIC_INDEX( ubRefuelList[ LocationOfNearestRefuelPoint( FALSE ) ][ 0 ], ubRefuelList[ LocationOfNearestRefuelPoint( FALSE ) ][ 1 ] ) );

	pNode = NULL;

	if( uiLastTempPathSectorId != iClosestRefuelPoint )
	{
		pNode = BuildAStrategicPath( pNode, ( INT16 )( uiLastTempPathSectorId ), ( INT16 )iClosestRefuelPoint, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE );
//		pNode = BuildAStrategicPath( pNode, ( INT16 )( uiLastTempPathSectorId ), ( INT16 )iClosestRefuelPoint, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE, TRUE );

		pNode = MoveToBeginningOfPathList( pNode );
	}

	pTempNode = pNode;
	uiLength = 0;

	if( pTempNode )
	{
		pTempNode = pTempNode->pNext;
	}

	while( pTempNode )
	{
		if( uiLength == 0 )
			{
				if( pTempNode->pNext )
				{
					if( uiLastTempPathSectorId == pNode->pNext->uiSectorId )
					{
						// do nothing
					}
					else
					{
						uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pTempNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pTempNode->uiSectorId / MAP_WORLD_X ) );
					}
				}
				else
				{
					uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pTempNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pTempNode->uiSectorId / MAP_WORLD_X ) );
				}
			}
			else
			{
				uiCost += GetCostOfPassageForHelicopter( ( UINT16 )( pTempNode->uiSectorId % MAP_WORLD_X ), ( UINT16 ) ( pTempNode->uiSectorId / MAP_WORLD_X ) );
			}

		uiLength++;
		pTempNode = pTempNode->pNext;
	}

	return( ( INT32 )uiCost );
}
*/


void HandleHelicopterOnGroundGraphic( void )
{
	UINT8					ubSite = 0;
	SOLDIERTYPE		*pSoldier;

	// no worries if underground
	if (gbWorldSectorZ != 0 )
	{
		return;
	}

	for( ubSite = 0; ubSite < NUMBER_OF_REFUEL_SITES; ubSite++ )
	{
		// is this refueling site sector the loaded sector ?
		if ( ( sRefuelSectorX[ ubSite ] == gWorldSectorX ) && ( sRefuelSectorY[ ubSite ] == gWorldSectorY ) )
		{
			// YES, so find out if the chopper is landed here
			if ( IsHelicopterOnGroundAtRefuelingSite( ubSite ) )
			{
				// Add....
				AddHelicopterToMaps( TRUE, ubSite );
				// ATE: Add skyridder too
				// ATE: only if hired......
				if ( fHelicopterAvailable	)
				{
					gMercProfiles[ SKYRIDER ].sSectorX = gWorldSectorX;
					gMercProfiles[ SKYRIDER ].sSectorY = gWorldSectorY;
					gMercProfiles[ SKYRIDER ].ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
					gMercProfiles[ SKYRIDER ].usStrategicInsertionData = iRefuelSkyriderGridNo[ubSite]; // iRefuelHeliGridNo[ubSite] - 639, Stand near the heli
					gMercProfiles[ SKYRIDER ].fUseProfileInsertionInfo = TRUE;
				}
			}
			else
			{
				AddHelicopterToMaps( FALSE, ubSite );
				// ATE: Remove skyridder....
				if ( fHelicopterAvailable	)
				{
					gMercProfiles[ SKYRIDER ].sSectorX = 0;
					gMercProfiles[ SKYRIDER ].sSectorY = 0;

					// see if we can find him and remove him if so....
					pSoldier = FindSoldierByProfileID( SKYRIDER, FALSE );

			// ATE: Don't do this if buddy is on our team!
					if ( pSoldier != NULL && pSoldier->bTeam != gbPlayerNum )
					{
						TacticalRemoveSoldier( pSoldier->ubID );
					}
				}
			}

			// Invalidate rendering
			InvalidateWorldRedundency( );

			// can't be 2 places at once!
			break;
		}
	}
}


void HandleHelicopterOnGroundSkyriderProfile( void )
{
	UINT8					ubSite = 0;
	SOLDIERTYPE		*pSoldier;

	// no worries if underground
	if (gbWorldSectorZ != 0 )
	{
		return;
	}

	for( ubSite = 0; ubSite < NUMBER_OF_REFUEL_SITES; ubSite++ )
	{
		// is this refueling site sector the loaded sector ?
		if ( ( sRefuelSectorX[ ubSite ] == gWorldSectorX ) && ( sRefuelSectorY[ ubSite ] == gWorldSectorY ) )
		{
			// YES, so find out if the chopper is landed here
			if ( IsHelicopterOnGroundAtRefuelingSite( ubSite ) )
			{
				// ATE: Add skyridder too
				// ATE: only if hired......
				if ( fHelicopterAvailable	)
				{
					gMercProfiles[ SKYRIDER ].sSectorX = gWorldSectorX;
					gMercProfiles[ SKYRIDER ].sSectorY = gWorldSectorY;
				}
			}
			else
			{
				// ATE: Remove skyridder....
				if ( fHelicopterAvailable	)
				{
					gMercProfiles[ SKYRIDER ].sSectorX = 0;
					gMercProfiles[ SKYRIDER ].sSectorY = 0;

					// see if we can find him and remove him if so....
					pSoldier = FindSoldierByProfileID( SKYRIDER, FALSE );

			// ATE: Don't do this if buddy is on our team!
					if ( pSoldier != NULL && pSoldier->bTeam != gbPlayerNum )
					{
						TacticalRemoveSoldier( pSoldier->ubID );
					}
				}
			}

			// can't be 2 places at once!
			break;
		}
	}
}


BOOLEAN IsHelicopterOnGroundAtRefuelingSite( UINT8 ubRefuelingSite )
{
	if ( fHelicopterDestroyed )
	{
		return(FALSE);
	}

	if ( fHelicopterIsAirBorne )
	{
		return(FALSE);
	}

	// if we haven't even met SkyRider
	if ( !fSkyRiderSetUp )
	{
		// then it's always at Drassen
		if ( ubRefuelingSite == DRASSEN_REFUELING_SITE )
		{
			return(TRUE);
		}
		else
		{
			return(FALSE);
		}
	}

	// skyrider is setup, helicopter isn't destroyed, so this ought to be a valid vehicle id
	Assert( iHelicopterVehicleId != -1 );

	// on the ground, but is it at this site or at another one?
	if ( ( sRefuelSectorX[ ubRefuelingSite ] == pVehicleList[ iHelicopterVehicleId ].sSectorX ) &&
			( sRefuelSectorY[ ubRefuelingSite ] == pVehicleList[ iHelicopterVehicleId ].sSectorY ) )
	{
		return(TRUE);
	}

	// not here
	return(FALSE);
}


BOOLEAN WillAirRaidBeStopped( INT16 sSectorX, INT16 sSectorY )
{
	UINT8 ubChance;

	if ( !StrategicMap[CALCULATE_STRATEGIC_INDEX(AIRPORT_X, AIRPORT_Y)].fEnemyControlled && !StrategicMap[CALCULATE_STRATEGIC_INDEX(AIRPORT2_X, AIRPORT2_Y)].fEnemyControlled )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "WillAirRaidBeStopped: enemy has no more airports" ) );
		return(TRUE);
	}

	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "WillAirRaidBeStopped: enemy air controlled = %d", StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].usAirType ) );

	// if enemy controls this SAM site, then it can't stop an air raid
	if ( !(StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].usAirType & AIRSPACE_PLAYER_ACTIVE) )
	{
		return( FALSE );
	}

	// Friendly airspace controlled by a working SAM site, so SAM site fires a SAM at air raid bomber

	for ( int i = 0; i < NUMBER_OF_SAMS; ++i )
	{
		BOOLEAN samworking = FALSE;
		if ( DoesSamCoverSector( i, SECTOR( sSectorX, sSectorY ), &samworking ) && samworking )
		{
			// calc chance that chopper will be shot down
			ubChance = StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( pSamList[i] )].bSAMCondition;

			// there's a fair chance of a miss even if the SAM site is in perfect working order
			if ( ubChance > gHelicopterSettings.ubHelicopterSAMSiteAccuracy * 3 ) // Madd - since this is only used for enemy air raids, we'll say that good guy SAMs can have a max of 99% to hit instead of 33%
			{
				ubChance = gHelicopterSettings.ubHelicopterSAMSiteAccuracy * 3;
			}

			if ( PreRandom( 100 ) < ubChance )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "WillAirRaidBeStopped: return true" ) );
				return(TRUE);
			}
		}
	}
	
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("WillAirRaidBeStopped: return false"));
	return( FALSE );
}

void HeliCrashSoundStopCallback( void *pData )
{
	SkyriderDestroyed( );
}


BOOLEAN HandleSAMSiteAttackOfHelicopterInSector( INT16 sSectorX, INT16 sSectorY )
{
	UINT32 ubSamNumber = 0;
	INT8 bSAMCondition;
	UINT8 ubChance;

#ifdef JA2TESTVERSION
	if ( fSAMSitesDisabledFromAttackingPlayer == TRUE )
	{
		return(FALSE);
	}
#endif

	// if this sector is in friendly airspace, we're safe
	if ( !(StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].usAirType & AIRSPACE_ENEMY_ACTIVE) )
	{
		// no problem, friendly airspace
		return( FALSE );
	}

	// Hostile airspace controlled by a working SAM site, so SAM site fires a SAM at Skyrider!!!

	for ( int i = 0; i < NUMBER_OF_SAMS; ++i )
	{
		BOOLEAN samworking = FALSE;
		if ( DoesSamCoverSector( i, SECTOR( sSectorX, sSectorY ), &samworking ) && samworking )
		{
			bSAMCondition = StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( pSamList[i] )].bSAMCondition;

			// calc chance that chopper will be shot down
			ubChance = bSAMCondition;

			// there's a fair chance of a miss even if the SAM site is in perfect working order
			if ( ubChance > gHelicopterSettings.ubHelicopterSAMSiteAccuracy )
			{
				ubChance = gHelicopterSettings.ubHelicopterSAMSiteAccuracy;
			}

			if ( PreRandom( 100 ) < ubChance )
			{
				// another hit!
				++gubHelicopterHitsTaken;

				// Took a hit!	Pause time so player can reconsider
				StopTimeCompression( );

				// first hit?
				if ( gubHelicopterHitsTaken == 1 )
				{
					HeliCharacterDialogue( pSkyRider, HELI_TOOK_MINOR_DAMAGE );
					if ( gGameExternalOptions.fHelicopterPassengersCanGetHit == TRUE )
						HurtPassengersInHelicopter( iHelicopterVehicleId );
				}
				// second hit?
				else if ( gubHelicopterHitsTaken == 2 )
				{
					// going back to base (no choice, dialogue says so)
					HeliCharacterDialogue( pSkyRider, HELI_TOOK_MAJOR_DAMAGE );
					MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_DAMAGE );
					if ( gGameExternalOptions.fHelicopterPassengersCanGetHit == TRUE )
						HurtPassengersInHelicopter( iHelicopterVehicleId );
				}
				// third hit!
				else
				{
					// Important: Skyrider must still be alive when he talks, so must do this before heli is destroyed!
					HeliCharacterDialogue( pSkyRider, HELI_GOING_DOWN );

					// everyone die die die
					// play sound
					//	if ( PlayJA2StreamingSampleFromFile( "stsounds\\blah2.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN, HeliCrashSoundStopCallback ) == SOUND_ERROR )
					//{
					//// Destroy here if we cannot play streamed sound sample....
					//		SkyriderDestroyed( );
					//}
					//else
					//{
					//// otherwise it's handled in the callback
					//// remove any arrival events for the helicopter's group
					//DeleteStrategicEvent( EVENT_GROUP_ARRIVAL, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );
					//}
					// anv - calling SkyriderDestroyed() in callback from playing sound causes sound system to crash, as SkyriderDestroyed causes new sounds (hit battlesnds) to play while blah2 is not removed yet
					// + if there's a pause while falling heli sound is played, it's possible to cheat by changing passengers assignments
					PlayJA2StreamingSampleFromFile( "stsounds\\blah2.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN, HeliCrashSoundStopCallback );
					SkyriderDestroyed( );

					// special return code indicating heli was destroyed
					return(TRUE);
				}
			}
		}
	}

	// still flying
	return( FALSE );
}


// are we at the end of the path for the heli?
BOOLEAN EndOfHelicoptersPath( void )
{
	if( pVehicleList[ iHelicopterVehicleId ].pMercPath == NULL )
	{
		return( TRUE );
	}

	if( pVehicleList[ iHelicopterVehicleId ].pMercPath->pNext == NULL )
	{
		return( TRUE );
	}

	return( FALSE );
}


// check if helicopter can take off?
BOOLEAN CanHelicopterTakeOff( void )
{
	INT16 sHelicopterSector = 0;

	// if it's already in the air
	if( fHelicopterIsAirBorne == TRUE )
	{
		return( TRUE );
	}

	// grab location
	sHelicopterSector = pVehicleList[ iHelicopterVehicleId ].sSectorX + pVehicleList[ iHelicopterVehicleId ].sSectorY * MAP_WORLD_X;
	// if it's not in enemy control, we can take off
	if( StrategicMap[ sHelicopterSector ].fEnemyControlled == FALSE )
	{
		return( TRUE );
	}

	return( FALSE );
}

void AddHeliPiece( INT32 iGridNo, UINT16 sOStruct )
{
	UINT16 usDummy;

	// ATE: Check first if already exists....
	if ( !TypeExistsInStructLayer( iGridNo, sOStruct, &usDummy ) )
	{
		// place in the world
		AddStructToTail( iGridNo, sOStruct );
	}
}


void AddHelicopterToMaps( BOOLEAN fAdd, UINT8 ubSite )
{
 	INT32 iGridNo = iRefuelHeliGridNo[ ubSite ];
	INT32 iTileIndexNo = iRefuelHeliTileIndex[ ubSite ];
	//INT16 sOStruct = 0;
	INT16	sGridX, sGridY;
	INT16	sCentreGridX, sCentreGridY;

	// find out what slot it is by which site
	//if( ubSite == 0 )
	//{
	//	// drassen
	//	sOStruct = FIRSTOSTRUCT1;
	//}
	//else
	//{
	//	// estoni
	//	sOStruct = FOURTHOSTRUCT1;
	//}


	// are we adding or taking away
	if( fAdd )
	{
		AddHeliPiece( iGridNo, ( UINT16 ) iTileIndexNo );	// AddHeliPiece( iGridNo, sOStruct );
		AddHeliPiece( iGridNo, ( UINT16 )( iTileIndexNo + 1 ) );
		AddHeliPiece( ( iGridNo - WORLD_COLS * 5 ), ( UINT16 )( iTileIndexNo + 2 ) );	// ( iGridNo - 800 )
		AddHeliPiece( iGridNo, ( UINT16 )( iTileIndexNo + 3 ) );
		AddHeliPiece( iGridNo, ( UINT16 )( iTileIndexNo + 4 ) );
		AddHeliPiece( ( iGridNo - WORLD_COLS * 5 ), ( UINT16 )( iTileIndexNo + 5 ) );	// ( iGridNo - 800 )

		InvalidateWorldRedundency();
		SetRenderFlags( RENDER_FLAG_FULL );

		// ATE: If any mercs here, bump them off!
		ConvertGridNoToXY( iGridNo, &sCentreGridX, &sCentreGridY );

		for( sGridY = sCentreGridY - 5; sGridY < sCentreGridY + 5; sGridY++ )
		{
			for( sGridX = sCentreGridX - 5; sGridX < sCentreGridX + 5; sGridX++ )
			{
				iGridNo = MAPROWCOLTOPOS( sGridY, sGridX );

		 		BumpAnyExistingMerc( iGridNo );
			}
		}
	}
	else
	{
		// remove from the world
		RemoveStruct( iGridNo, ( UINT16 )iTileIndexNo );
		RemoveStruct( iGridNo, ( UINT16 )( iTileIndexNo + 1 ) );
		RemoveStruct( ( iGridNo - WORLD_COLS * 5 ), ( UINT16 )( iTileIndexNo + 2 ) );	// ( iGridNo - 800 )
		RemoveStruct( iGridNo, ( UINT16 )( iTileIndexNo + 3 ) );
		RemoveStruct( iGridNo, ( UINT16 )( iTileIndexNo + 4 ) );
		RemoveStruct( ( iGridNo - WORLD_COLS * 5 ), ( UINT16 )( iTileIndexNo + 5 ) );	// ( iGridNo - 800 )

		InvalidateWorldRedundency();
		SetRenderFlags( RENDER_FLAG_FULL );
	}
}

void AddEnemyHelicopterToMaps( BOOLEAN fAdd, BOOLEAN fDestroyed, INT32 aGridno, INT32 aTileIndex )
{
	// for safety, remove the old one first
	{
		// remove from the world
		RemoveStruct( aGridno, (UINT16)aTileIndex );
		RemoveStruct( aGridno, (UINT16)(aTileIndex + 1) );
		RemoveStruct( (aGridno - WORLD_COLS * 5), (UINT16)(aTileIndex + 2) );
		RemoveStruct( aGridno, (UINT16)(aTileIndex + 3) );
		RemoveStruct( aGridno, (UINT16)(aTileIndex + 4) );
		RemoveStruct( (aGridno - WORLD_COLS * 5), (UINT16)(aTileIndex + 5) );

		// for safety reasons, both versions are to be removed
		RemoveStruct( aGridno, (UINT16)aTileIndex + 6 );
		RemoveStruct( aGridno, (UINT16)(aTileIndex + 6 + 1) );
		RemoveStruct( (aGridno - WORLD_COLS * 5), (UINT16)(aTileIndex + 6 + 2) );
		RemoveStruct( aGridno, (UINT16)(aTileIndex + 6 + 3) );
		RemoveStruct( aGridno, (UINT16)(aTileIndex + 6 + 4) );
		RemoveStruct( (aGridno - WORLD_COLS * 5), (UINT16)(aTileIndex + 6 + 5) );

		InvalidateWorldRedundency( );
		SetRenderFlags( RENDER_FLAG_FULL );
	}

	if ( fAdd )
	{
		INT16	sCentreGridX, sCentreGridY;

		INT16 offset = fDestroyed ? 6 : 0;

		AddHeliPiece( aGridno, (UINT16)aTileIndex + offset );
		AddHeliPiece( aGridno, (UINT16)(aTileIndex + offset + 1) );
		AddHeliPiece( (aGridno - WORLD_COLS * 5), (UINT16)(aTileIndex + offset + 2) );
		AddHeliPiece( aGridno, (UINT16)(aTileIndex + offset + 3) );
		AddHeliPiece( aGridno, (UINT16)(aTileIndex + offset + 4) );
		AddHeliPiece( (aGridno - WORLD_COLS * 5), (UINT16)(aTileIndex + offset + 5) );

		InvalidateWorldRedundency( );
		SetRenderFlags( RENDER_FLAG_FULL );

		// ATE: If any mercs here, bump them off!
		ConvertGridNoToXY( aGridno, &sCentreGridX, &sCentreGridY );

		for ( INT16 sGridY = sCentreGridY - 5; sGridY < sCentreGridY + 5; sGridY++ )
		{
			for ( INT16 sGridX = sCentreGridX - 5; sGridX < sCentreGridX + 5; sGridX++ )
			{
				aGridno = MAPROWCOLTOPOS( sGridY, sGridX );

				BumpAnyExistingMerc( aGridno );
			}
		}
	}
}


BOOLEAN IsSkyriderIsFlyingInSector( INT16 sSectorX, INT16 sSectorY )
{
	GROUP *pGroup;

	// up and about?
	if ( fHelicopterAvailable && ( iHelicopterVehicleId != -1 ) && CanHelicopterFly() && fHelicopterIsAirBorne )
	{
		pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

		// the right sector?
		if ( ( sSectorX == pGroup->ubSectorX ) && ( sSectorY == pGroup->ubSectorY ) )
		{
			return(TRUE);
		}
	}

	return( FALSE );
}



BOOLEAN IsGroupTheHelicopterGroup( GROUP *pGroup )
{
	if( ( iHelicopterVehicleId != -1 ) && VehicleIdIsValid( iHelicopterVehicleId ) &&
			( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup != 0 ) &&
			( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup == pGroup->ubGroupID ) )
	{
		return( TRUE );
	}

	return( FALSE );
}


INT16 GetNumSafeSectorsInPath( void )
{
	// get the last sector value in the helictoper's path
	PathStPtr pNode = NULL;
	UINT32 uiLocation = 0;
	UINT32	uiCount = 0;
	INT32 iHeliSector = -1;
	GROUP *pGroup;

	// if the heli is on the move, what is the distance it will move..the length of the merc path, less the first node
	if( CanHelicopterFly( ) == FALSE )
	{
		// big number, no go
		return( 0 );
	}


	// may need to skip the sector the chopper is currently in
	iHeliSector = CALCULATE_STRATEGIC_INDEX( pVehicleList[ iHelicopterVehicleId ].sSectorX, pVehicleList[ iHelicopterVehicleId ].sSectorY );

	// get chopper's group ptr
	pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

	pNode = pVehicleList[ iHelicopterVehicleId ].pMercPath;

	// any path yet?
	if( pNode != NULL )
	{
		// first node: skip it if that's the sector the chopper is currently in, AND
		// we're NOT gonna be changing directions (not actually performed until waypoints are rebuilt AFTER plotting is done)
		if ( ( ( INT32 ) pNode->uiSectorId == iHeliSector ) && ( pNode->pNext != NULL ) &&
				!GroupBetweenSectorsAndSectorXYIsInDifferentDirection( pGroup, ( UINT8 ) GET_X_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ), ( UINT8 ) GET_Y_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ) ) )
		{
			pNode = pNode->pNext;
		}

		while( pNode)
		{
			uiLocation = pNode->uiSectorId;

			if ( !(StrategicMap[uiLocation].usAirType & AIRSPACE_ENEMY_ACTIVE) )
			{
				++uiCount;
			}

			pNode = pNode->pNext;
		}
	}


	pNode = MoveToBeginningOfPathList( pTempHelicopterPath );
	// any path yet?
	if( pNode != NULL )
	{
		// first node: skip it if that's the sector the chopper is currently in, AND
		// we're NOT gonna be changing directions (not actually performed until waypoints are rebuilt AFTER plotting is done)
		// OR if the chopper has a mercpath, in which case this a continuation of it that would count the sector twice
		if ( ( ( ( INT32 ) pNode->uiSectorId == iHeliSector ) && ( pNode->pNext != NULL ) &&
 				!GroupBetweenSectorsAndSectorXYIsInDifferentDirection( pGroup, ( UINT8 ) GET_X_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ), ( UINT8 ) GET_Y_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ) ) ) ||
				( GetLengthOfPath( pVehicleList[ iHelicopterVehicleId ].pMercPath ) > 0 ) )
		{
			pNode = pNode->pNext;
		}

		while( pNode)
		{
			uiLocation = pNode->uiSectorId;

			if ( !(StrategicMap[uiLocation].usAirType & AIRSPACE_ENEMY_ACTIVE) )
			{
				++uiCount;
			}

			pNode = pNode->pNext;
		}
	}

	return( (INT16)uiCount );
}


INT16 GetNumUnSafeSectorsInPath( void )
{
	// get the last sector value in the helictoper's path
	PathStPtr pNode = NULL;
	UINT32 uiLocation = 0;
	UINT32	uiCount = 0;
	INT32 iHeliSector = -1;
	GROUP *pGroup;


	// if the heli is on the move, what is the distance it will move..the length of the merc path, less the first node
	if( CanHelicopterFly( ) == FALSE )
	{
		// big number, no go
		return( 0 );
	}


	// may need to skip the sector the chopper is currently in
	iHeliSector = CALCULATE_STRATEGIC_INDEX( pVehicleList[ iHelicopterVehicleId ].sSectorX, pVehicleList[ iHelicopterVehicleId ].sSectorY );

	// get chopper's group ptr
	pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

	pNode = pVehicleList[ iHelicopterVehicleId ].pMercPath;

	// any path yet?
	if( pNode != NULL )
	{
		// first node: skip it if that's the sector the chopper is currently in, AND
		// we're NOT gonna be changing directions (not actually performed until waypoints are rebuilt AFTER plotting is done)
		if ( ( ( INT32 ) pNode->uiSectorId == iHeliSector ) && ( pNode->pNext != NULL ) &&
				!GroupBetweenSectorsAndSectorXYIsInDifferentDirection( pGroup, ( UINT8 ) GET_X_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ), ( UINT8 ) GET_Y_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ) ) )
		{
			pNode = pNode->pNext;
		}

		while( pNode)
		{
			uiLocation = pNode->uiSectorId;

			if ( StrategicMap[uiLocation].usAirType & AIRSPACE_ENEMY_ACTIVE )
			{
				++uiCount;
			}

			pNode = pNode->pNext;
		}
	}

	pNode = MoveToBeginningOfPathList( pTempHelicopterPath );
	// any path yet?
	if( pNode != NULL )
	{
		// first node: skip it if that's the sector the chopper is currently in, AND
		// we're NOT gonna be changing directions (not actually performed until waypoints are rebuilt AFTER plotting is done)
		// OR if the chopper has a mercpath, in which case this a continuation of it that would count the sector twice
		if ( ( ( ( INT32 ) pNode->uiSectorId == iHeliSector ) && ( pNode->pNext != NULL ) &&
				!GroupBetweenSectorsAndSectorXYIsInDifferentDirection( pGroup, ( UINT8 ) GET_X_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ), ( UINT8 ) GET_Y_FROM_STRATEGIC_INDEX( pNode->pNext->uiSectorId ) ) ) ||
				( GetLengthOfPath( pVehicleList[ iHelicopterVehicleId ].pMercPath ) > 0 ) )
		{
			pNode = pNode->pNext;
		}

		while( pNode)
		{
			uiLocation = pNode->uiSectorId;

			if ( StrategicMap[uiLocation].usAirType & AIRSPACE_ENEMY_ACTIVE )
			{
				++uiCount;
			}

			pNode = pNode->pNext;
		}
	}

	return( (INT16)uiCount );
}



void PaySkyriderBill( void)
{
	// if we owe anything for the trip
	if ( iTotalAccumulatedCostByPlayer > 0 )
	{
		// if player can afford to pay the Skyrider bill
		if( LaptopSaveInfo.iCurrentBalance >= iTotalAccumulatedCostByPlayer )
		{
			// no problem, pay the man
			// add the transaction
			AddTransactionToPlayersBook( PAYMENT_TO_NPC, SKYRIDER, GetWorldTotalMin( ), -iTotalAccumulatedCostByPlayer );
			ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pSkyriderText[ 0 ], iTotalAccumulatedCostByPlayer );
		}
		else
		{
			// money owed
			if( LaptopSaveInfo.iCurrentBalance > 0 )
			{
				ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pSkyriderText[ 0 ], LaptopSaveInfo.iCurrentBalance );
				gMercProfiles[ SKYRIDER ].iBalance = LaptopSaveInfo.iCurrentBalance - iTotalAccumulatedCostByPlayer;
				// add the transaction
				AddTransactionToPlayersBook( PAYMENT_TO_NPC, SKYRIDER, GetWorldTotalMin( ), -LaptopSaveInfo.iCurrentBalance );
			}
			else
			{
				gMercProfiles[ SKYRIDER ].iBalance = - iTotalAccumulatedCostByPlayer;
			}

			HeliCharacterDialogue( pSkyRider, OWED_MONEY_TO_SKYRIDER );
			ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pSkyriderText[ 1 ], -gMercProfiles[ SKYRIDER ].iBalance );

			// kick everyone out! (we know we're in a safe sector if we're paying)
			//CHRISL: This may no longer be the case but I'm not sure of a better way to handle things.
			MoveAllInHelicopterToFootMovementGroup( );

			MakeHeliReturnToBase( HELICOPTER_RETURN_REASON_NONE );
		}

		iTotalAccumulatedCostByPlayer = 0;
	}
}


void PayOffSkyriderDebtIfAny( )
{
	INT32 iAmountOwed;
	INT32 iPayAmount;


	iAmountOwed = - gMercProfiles[ SKYRIDER ].iBalance;

	// if we owe him anything, and have any money
	if ( ( iAmountOwed > 0 ) && ( LaptopSaveInfo.iCurrentBalance > 0 ) )
	{
		iPayAmount = min( iAmountOwed, LaptopSaveInfo.iCurrentBalance );

		// pay the man what we can
		gMercProfiles[ SKYRIDER ].iBalance += iPayAmount;
		// add the transaction
		AddTransactionToPlayersBook( PAYMENT_TO_NPC, SKYRIDER, GetWorldTotalMin( ), -iPayAmount );
		// tell player
		ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pSkyriderText[ 0 ], iPayAmount );

		// now whaddawe owe?
		iAmountOwed = - gMercProfiles[ SKYRIDER ].iBalance;

		// if it wasn't enough
		if ( iAmountOwed > 0 )
		{
			ScreenMsg( FONT_MCOLOR_DKRED, MSG_INTERFACE, pSkyriderText[ 1 ], iAmountOwed );
			HeliCharacterDialogue( pSkyRider, OWED_MONEY_TO_SKYRIDER );
		}
	}
}



void MakeHeliReturnToBase( UINT8 ubReturnReason )
{
	INT32 iLocation = 0;


	// if already at a refueling point
	if ( CheckForArrivalAtRefuelPoint() )
	{
		ReFuelHelicopter( );
	}
	else
	{
		// choose destination (closest refueling sector)
		iLocation = LocationOfNearestRefuelPoint( TRUE, ubReturnReason, pVehicleList[ iHelicopterVehicleId ].sSectorX, pVehicleList[ iHelicopterVehicleId ].sSectorY );

		// null out path
		pVehicleList[ iHelicopterVehicleId ].pMercPath = ClearStrategicPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

		// plot path to that sector
		pVehicleList[ iHelicopterVehicleId ].pMercPath = AppendStrategicPath( MoveToBeginningOfPathList( BuildAStrategicPath( NULL, GetLastSectorIdInVehiclePath( iHelicopterVehicleId ) , ( INT16 )( CALCULATE_STRATEGIC_INDEX( sRefuelSectorX[ iLocation ], sRefuelSectorY[ iLocation ] ) ) , pVehicleList[ iHelicopterVehicleId ].ubMovementGroup, FALSE /*, FALSE */ ) ), pVehicleList[ iHelicopterVehicleId ].pMercPath );
		pVehicleList[ iHelicopterVehicleId ].pMercPath = MoveToBeginningOfPathList( pVehicleList[ iHelicopterVehicleId ].pMercPath );

		// rebuild the movement waypoints
		RebuildWayPointsForGroupPath( pVehicleList[ iHelicopterVehicleId ].pMercPath, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

		fHeliReturnStraightToBase = TRUE;
		fHoveringHelicopter = FALSE;
	}

	// stop time compression if it's on so player can digest this
	StopTimeCompression( );
}



BOOLEAN SoldierAboardAirborneHeli( SOLDIERTYPE *pSoldier )
{
	Assert( pSoldier );

	// if not in a vehicle, or not aboard the helicopter
	if ( ( pSoldier->bAssignment != VEHICLE ) || ( pSoldier->iVehicleId != iHelicopterVehicleId ) )
	{
		return( FALSE );
	}

	// he's in the heli - is it airborne?
	if ( !fHelicopterIsAirBorne )
	{
		// nope, it's currently on the ground
		return( FALSE );
	}

	// yes, airborne
	return( TRUE );
}
