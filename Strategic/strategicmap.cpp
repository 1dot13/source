#include "builddefines.h"

#include "strategicmap.h"
#include "strategic.h"
#include "Strategic Mines.h"
#include "types.h"
#include "memory.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "jascreens.h"
#include "worlddef.h"
#include "Soldier Control.h"
#include "overhead.h"
#include "interface panels.h"
#include "isometric utils.h"
#include "Ambient Control.h"
#include "worlddef.h"
#include "worlddat.h"
#include "text.h"
#include "Soldier add.h"
#include "soldier macros.h"
#include "Strategic Pathing.h"
#include "soldier create.h"
#include "handle UI.h"
#include "faces.h"
#include "renderworld.h"
#include "gamescreen.h"
#include "game clock.h"
#include "Soldier Init List.h"
#include "strategic turns.h"
#include "merc entering.h"
#include "Map Information.h"
#include "Assignments.h"
#include "message.h"
#include "Font Control.h"
#include "environment.h"
#include "Game Events.h"
#include "quests.h"
#include "FileMan.h"
#include "animated progressbar.h"
#include "music control.h"
#include "fade screen.h"
#include "Strategic Movement.h"
#include "Campaign Types.h"
#include "sys globals.h"
#include "Exit Grids.h"
#include "Tactical Save.h"
#include "pathai.h"
#include "animation control.h"
#include "squads.h"
#include "worldman.h"
#include "Overhead.h"
#include "Strategic Town Loyalty.h"
#include "Queen Command.h"
#include "cursor control.h"
#include "PreBattle Interface.h"
#include "Shade Table Util.h"
#include "Scheduling.h"
#include "gameloop.h"
#include "Random.h"
#include "ai.h"
#include "opplist.h"
#include "keys.h"
#include "Tactical Placement GUI.h"
#include "Map Screen Helicopter.h"
#include "map edgepoints.h"
#include "Player Command.h"
#include "Event Pump.h"
#include "meanwhile.h"
#include "air raid.h"
#include "Strategic Mines.h"
#include "Creature Spreading.h"
#include "SaveLoadMap.h"
#include "Militia Control.h"
#include "gamesettings.h"
#include "dialogue control.h"
#include "Town Militia.h"
#include "sysutil.h"
#include "Sound Control.h"
#include "points.h"
#include "render dirty.h"
#include "Debug Control.h"
#include "expat.h"
#include "Loading Screen.h"
#include "Queen Command.h"
#include "Enemy Soldier Save.h"
#include "NPC.h"
#include "Strategic Event Handler.h"
#include "MessageBoxScreen.h"
#include "interface dialogue.h"
#include "Map Screen Interface.h"
#include "history.h"
#include "Bullets.h"
#include "physics.h"
#include "Explosion Control.h"
#include "Auto Resolve.h"
#include "cursors.h"
#include "GameVersion.h"

#include "LuaInitNPCs.h"
#include "Luaglobal.h"

#include "GameInitOptionsScreen.h"

#ifdef JA2UB
#include "SaveLoadGame.h"
#include "email.h"
#include "Ja25 Strategic Ai.h"
#include "Merc Hiring.h"
#include "Ja25_Tactical.h"
#include "Timer Control.h"
#include "Soldier Control.h"
#include "Ja25Update.h"
#include "Map Screen Interface Bottom.h"
#include "Strategic AI.h"
#include "ub_config.h"
#include "Luaglobal.h"
#endif

#include "connect.h" //hayden added alot ""'s to get around client spawing random/different placed AI
#include "SaveLoadGame.h"
#include "Strategic Mines.h"
#include "Strategic Mines LUA.h"
#include "UndergroundInit.h"

#include "LuaInitNPCs.h"
#include "Luaglobal.h"

#include "sgp_logger.h"

#include "Map Screen Interface Map Inventory.h"	// added by Flugente
#include "ASD.h"	// added by Flugente

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


//Used by PickGridNoToWalkIn
#define MAX_ATTEMPTS	200

#define		QUEST_CHECK_EVENT_TIME				( 8 * 60 )
#define		BOBBYRAY_UPDATE_TIME				( 9 * 60 )
#define		INSURANCE_UPDATE_TIME				0
#define		EARLY_MORNING_TIME					( 4 * 60 )
#define		ENRICO_MAIL_TIME                    ( 7 * 60 )

enum
{
	ABOUT_TO_LOAD_NEW_MAP,
	ABOUT_TO_TRASH_WORLD,
};
BOOLEAN HandleDefiniteUnloadingOfWorld( UINT8 ubUnloadCode );

extern INT32		gsRobotGridNo;
extern BOOLEAN	gfUndergroundTacticalTraversal;

//extern MINE_LOCATION_TYPE gMineLocation[MAX_NUMBER_OF_MINES];
//extern MINE_STATUS_TYPE gMineStatus[MAX_NUMBER_OF_MINES];


extern void CalculateNonPersistantPBIInfo( );

extern void MapScreenDefaultOkBoxCallback( UINT8 bExitValue );

extern BOOLEAN		gfGettingNameFromSaveLoadScreen; // symbol already declared globally in SaveLoadScreen.cpp (jonathanl)

INT16			gWorldSectorX = 0;
INT16			gWorldSectorY = 0;
INT8			gbWorldSectorZ = -1;

INT16			gsAdjacentSectorX, gsAdjacentSectorY;
INT8			gbAdjacentSectorZ;
GROUP			*gpAdjacentGroup = NULL;
UINT8			gubAdjacentJumpCode;
UINT32		guiAdjacentTraverseTime;
UINT8			gubTacticalDirection;
INT32			gsAdditionalData;//dnl ch56 151009
INT32		gusDestExitGridNo;

BOOLEAN		fUsingEdgePointsForStrategicEntry = FALSE;
BOOLEAN		gfInvalidTraversal = FALSE;
BOOLEAN		gfLoneEPCAttemptingTraversal = FALSE;
BOOLEAN		gfRobotWithoutControllerAttemptingTraversal = FALSE;
BOOLEAN   gubLoneMercAttemptingToAbandonEPCs = 0;
INT8			gbPotentiallyAbandonedEPCSlotID = -1;

INT8 gbGreenToElitePromotions = 0;
INT8 gbGreenToRegPromotions = 0;
INT8 gbRegToElitePromotions = 0;
INT8 gbMilitiaPromotions = 0;


extern BOOLEAN gfUsePersistantPBI;

BOOLEAN		gfUseAlternateMap = FALSE;
// whether or not we have found Orta yet
BOOLEAN fFoundOrta = FALSE;

UINT8	NUMBER_OF_SAMS;

// have any of the sam sites been found (actual values, saved to savegames)
BOOLEAN fSamSiteFound[MAX_NUMBER_OF_SAMS];

// original of previous for proper initialization of a new game
BOOLEAN fSamSiteFoundOrig[MAX_NUMBER_OF_SAMS];

// sectors with sam sites
INT16 pSamList[MAX_NUMBER_OF_SAMS];

// girdno's of control terminal of sam site
INT32 pSamGridNoAList[MAX_NUMBER_OF_SAMS];
INT32 pSamGridNoBList[MAX_NUMBER_OF_SAMS];

// coordinates X,Y of sam sites on strategic map
INT16 gpSamSectorX[MAX_NUMBER_OF_SAMS];
INT16 gpSamSectorY[MAX_NUMBER_OF_SAMS];

// ATE: Update this w/ graphic used
// Use 3 if / orientation, 4 if \ orientation
INT8 gbSAMGraphicList[MAX_NUMBER_OF_SAMS];

INT8 gbMercIsNewInThisSector[MAX_NUM_SOLDIERS];


// the amount of time that a soldier will wait to return to desired/old squad
#define DESIRE_SQUAD_RESET_DELAY 12 * 60

BOOLEAN localizedMapTextOnly;


UINT32 ubSAMControlledSectors[MAP_WORLD_Y][MAP_WORLD_X];// = {
//       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
//    0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,
//
//    0,   1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2,   0,		// A
//    0,   1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,   0,		// B
//    0,   1, 1, 1, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2,   0,		// C
//    0,   1,01, 1, 1, 1, 1, 1, 3, 3, 2, 2, 2, 2, 2,02, 2,   0,		// D
//    0,   1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2,   0,		// E
//    0,   1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2,   0,		// F
//    0,   1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2,   0,		// G
//    0,   1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2,   0,		// H
//    0,   1, 1, 3, 3, 3, 3, 3,03, 3, 3, 3, 3, 3, 2, 2, 2,   0,		// I
//    0,   1, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2,   0,		// J
//    0,   4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2,   0,		// K
//    0,   4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2,   0,		// L
//    0,   4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2,   0,		// M
//    0,   4, 4, 4,04, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2,   0,		// N
//    0,   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2,   0,		// O
//    0,   4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2,   0,		// P
//
//    0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,
//};

INT16 DirXIncrementer[8] =
{
	0,        //N
	1,        //NE
	1,        //E
	1,        //SE
	0,         //S
	-1,       //SW
	-1,       //W
	-1       //NW
};

INT16 DirYIncrementer[8] =
{
	-1,        //N
	-1,        //NE
	0,        //E
	1,        //SE
	1,         //S
	1,       //SW
	0,       //W
	-1       //NW
};


STR8 pVertStrings[] = {
	"X",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
};

STR8 pHortStrings[] = {
	"X",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	"17",
};

// HEADROCK HAM 3.6: Array to hold user-defined sector names
CHAR16 gzSectorNames[256][4][MAX_SECTOR_NAME_LENGTH];

// moved to lua
//CHAR16 gzSectorUndergroundNames1[256][4][MAX_SECTOR_NAME_LENGTH]; 
//CHAR16 gzSectorUndergroundNames2[256][4][MAX_SECTOR_NAME_LENGTH]; 
//CHAR16 gzSectorUndergroundNames3[256][4][MAX_SECTOR_NAME_LENGTH]; 

BOOLEAN ReadInStrategicMapSectorTownNames( STR fileName, BOOLEAN localizedVersion );

void DoneFadeOutAdjacentSector( void );
void DoneFadeOutExitGridSector( void );

INT32 PickGridNoNearestEdge( SOLDIERTYPE *pSoldier, UINT8 ubTacticalDirection );
INT32 PickGridNoToWalkIn( SOLDIERTYPE *pSoldier, UINT8 ubInsertionDirection, UINT32 *puiNumAttempts );


//JA25UB
#ifdef JA2UB
void HandleQuestCodeOnSectorExit( INT16 sOldSectorX, INT16 sOldSectorY, INT8 bOldSectorZ );
void HandlePotentialMoraleHitForSkimmingSectors( GROUP *pGroup );
void HandlePlayerTeamQuotesWhenEnteringSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );
void ShouldNpcBeAddedToSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ );
void HandlePlayerQuotesWhenEnteringFirstTunnelSector( );
void AddExitGridForFanToPowerGenSector( );
void HandleSectorSpecificUnLoadingOfMap( INT16 sMapX, INT16 sMapY, INT8 bMapZ );
//void MakeAllTeamMembersCrouchedThenStand();
void HandleMovingTheEnemiesToBeNearPlayerWhenEnteringComplexMap( );
void HandleFortifiedDoor( );
void CreateAndAddMoneyObjectToGround( UINT32 sGridNo, INT32 iEasyAmount, INT32 iNormalAmount, INT32 iHardAmount );
void HandleGoingUpOrDownStairsForLoadScreensPurposes( INT16 sCurrentlyInSectorZ, INT16 sGoingToSectorZ );
void HandleMovingEnemiesCloseToEntranceInFirstTunnelMap( );
void HandleMovingEnemiesCloseToEntranceInSecondTunnelMap( );
void HandleFirstPartOfTunnelFanSound( );
void HandlePowerGenFanSoundModification( );
BOOLEAN MoveEnemyFromGridNoToRoofGridNo( UINT32 sSourceGridNo, UINT32 sDestGridNo );
void		HandleMovingEnemiesOntoRoofs( );
#endif

void HandleQuestCodeOnSectorExit( INT16 sOldSectorX, INT16 sOldSectorY, INT8 bOldSectorZ );
void HandlePotentialMoraleHitForSkimmingSectors( GROUP *pGroup );


extern void InitializeTacticalStatusAtBattleStart( );

extern HVSURFACE ghFrameBuffer;
extern BOOLEAN gfOverrideSector;

extern BOOLEAN sBadSectorsList[WORLD_MAP_X][WORLD_MAP_X];
extern STR16 pBullseyeStrings[];

extern void HandleRPCDescription( );

#ifdef CRIPPLED_VERSION
void CrippledVersionFailureToLoadMapCallBack( UINT8 bExitValue );
void CrippledVersionFailureToLoadMapCheck( );
#endif


//temp timer stuff -- to measure the time it takes to load a map.
#ifdef JA2TESTVERSION
extern INT16	 gsAINumAdmins;
extern INT16	 gsAINumTroops;
extern INT16	 gsAINumElites;
extern INT16	 gsAINumTanks;
extern INT16	 gsAINumCreatures;
//The wrapper time for EnterSector
BOOLEAN fStartNewFile = TRUE;
UINT32 uiEnterSectorStartTime;
UINT32 uiEnterSectorEndTime;
//The grand total time for loading a map
UINT32 uiLoadWorldStartTime;
UINT32 uiLoadWorldEndTime;
//The time spent in FileRead
UINT32 uiTotalFileReadTime;
UINT32 uiTotalFileReadCalls;
//LoadWorld and parts
UINT32 uiLoadWorldTime;
UINT32 uiTrashWorldTime;
UINT32 uiLoadMapTilesetTime;
UINT32 uiLoadMapLightsTime;
UINT32 uiBuildShadeTableTime;
UINT32 uiNumTablesSaved;
UINT32 uiNumTablesLoaded;
UINT32 uiNumImagesReloaded;
#include "render dirty.h"
#include "tiledat.h"
#endif

// SAM externalization stuff

#define MAX_CHAR_DATA_LENGTH			500
#define INVALID_SAMSITE_INDEX			-1


typedef enum
{
	SAMSITE_ELEMENT_NONE = 0,
	SAMSITE_ELEMENT_SAMINFO,
	SAMSITE_ELEMENT_SAMLIST,
	SAMSITE_ELEMENT_SAM_CONTROLLED_SECTORS,
	SAMSITE_ELEMENT_MAP_ROW,
	SAMSITE_ELEMENT_SAM,
	SAMSITE_ELEMENT_INDEX,
	SAMSITE_ELEMENT_SAMSECTOR,
	SAMSITE_ELEMENT_SAMSECTOR_X,
	SAMSITE_ELEMENT_SAMSECTOR_Y,
	SAMSITE_ELEMENT_SAM_HIDDEN,
	SAMSITE_ELEMENT_SAM_ORIENTATION,
	SAMSITE_ELEMENT_SAM_GRIDNO_A,
	SAMSITE_ELEMENT_SAM_GRIDNO_B
} SAMSITE_PARSE_STAGE;

typedef struct
{
	UINT32	uiIndex;
	INT16	samSectorX;
	INT16	samSectorY;
	BOOLEAN samHidden;
	INT8	samOrientation;
	INT32	samGridNoA;
	INT32	samGridNoB;
} samInfo;

typedef struct
{
	SAMSITE_PARSE_STAGE	curElement;

	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH + 1];
	samInfo					curSamInfo;
	UINT32					uiRowNumber;
	UINT32					uiHighestIndex;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} samsiteParseData;

static void XMLCALL
samsiteStartElementHandle( void *userData, const XML_Char *name, const XML_Char **atts )
{
	samsiteParseData * pData = (samsiteParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //are we reading this element?
	{

		if ( strcmp( name, "SAM_INFO" ) == 0 && pData->curElement == SAMSITE_ELEMENT_NONE )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMINFO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "SAM_CONTROLLED_SECTORS" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMINFO )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM_CONTROLLED_SECTORS;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "MAP_ROW" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM_CONTROLLED_SECTORS )
		{
			UINT32 uiAttrIndex;
			pData->curElement = SAMSITE_ELEMENT_MAP_ROW;

			for ( uiAttrIndex = 0; atts[uiAttrIndex] != NULL; uiAttrIndex += 2 )
			if ( strcmp( atts[uiAttrIndex], "row" ) == 0 )
			{
				pData->uiRowNumber = atol( atts[uiAttrIndex + 1] );
				break;
			}

			if ( atts[uiAttrIndex] != NULL && pData->uiRowNumber < MAP_WORLD_Y )
				pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "SAMLIST" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMINFO )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMLIST;

			memset( fSamSiteFoundOrig, 0, sizeof(fSamSiteFoundOrig) );
			memset( pSamList, 0, sizeof(pSamList) );
			memset( pSamGridNoAList, 0, sizeof(pSamGridNoAList) );
			memset( pSamGridNoBList, 0, sizeof(pSamGridNoBList) );
			memset( gbSAMGraphicList, 0, sizeof(gbSAMGraphicList) );
			memset( gpSamSectorX, 0, sizeof(gpSamSectorX) );
			memset( gpSamSectorY, 0, sizeof(gpSamSectorY) );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "SAM" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMLIST )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;

			memset( &pData->curSamInfo, 0, sizeof(samInfo) );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "samIndex" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_INDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "samSector" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMSECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMSECTOR )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMSECTOR_X;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMSECTOR )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMSECTOR_Y;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "samHidden" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM_HIDDEN;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "samOrientation" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM_ORIENTATION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "samGridNoA" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM_GRIDNO_A;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "samGridNoB" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM_GRIDNO_B;
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
samsiteCharacterDataHandle( void *userData, const XML_Char *str, int len )
{
	samsiteParseData * pData = (samsiteParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth && strlen( pData->szCharData ) < MAX_CHAR_DATA_LENGTH )
		strncat( pData->szCharData, str, __min( (unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen( pData->szCharData ) ) );
}


static void XMLCALL
samsiteEndElementHandle( void *userData, const XML_Char *name )
{
	samsiteParseData * pData = (samsiteParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //we're at the end of an element that we've been reading
	{
		if ( strcmp( name, "SAM_INFO" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMINFO )
		{
			pData->curElement = SAMSITE_ELEMENT_NONE;
		}
		else if ( strcmp( name, "SAM_CONTROLLED_SECTORS" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM_CONTROLLED_SECTORS )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMINFO;
		}
		else if ( strcmp( name, "MAP_ROW" ) == 0 && pData->curElement == SAMSITE_ELEMENT_MAP_ROW )
		{
			STR8 curBuffer = pData->szCharData + strspn( pData->szCharData, " \t\n\r" );
			UINT32 curCellIndex = 0;
			UINT32 curNumber;

			pData->curElement = SAMSITE_ELEMENT_SAM_CONTROLLED_SECTORS;

			while ( curBuffer[0] != '\0' )
			{
				sscanf( curBuffer, "%d", &curNumber );

				ubSAMControlledSectors[pData->uiRowNumber][curCellIndex] = curNumber;

				curCellIndex++;
				curBuffer += strcspn( curBuffer, " \t\n\r\0" );
				curBuffer += strspn( curBuffer, " \t\n\r" );
			}
		}
		else if ( strcmp( name, "SAMLIST" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMLIST )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMINFO;

			NUMBER_OF_SAMS = pData->uiHighestIndex;
		}
		else if ( strcmp( name, "SAM" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMLIST;

			if ( pData->curSamInfo.uiIndex != INVALID_SAMSITE_INDEX )
			{
				pData->curSamInfo.uiIndex--;
				fSamSiteFoundOrig[pData->curSamInfo.uiIndex] = !pData->curSamInfo.samHidden;
				gpSamSectorX[pData->curSamInfo.uiIndex] = pData->curSamInfo.samSectorX;
				gpSamSectorY[pData->curSamInfo.uiIndex] = pData->curSamInfo.samSectorY;
				pSamList[pData->curSamInfo.uiIndex] = SECTOR( pData->curSamInfo.samSectorX, pData->curSamInfo.samSectorY );
				gbSAMGraphicList[pData->curSamInfo.uiIndex] = pData->curSamInfo.samOrientation;
				pSamGridNoAList[pData->curSamInfo.uiIndex] = pData->curSamInfo.samGridNoA;
				pSamGridNoBList[pData->curSamInfo.uiIndex] = pData->curSamInfo.samGridNoB;
			}
		}
		else if ( strcmp( name, "samIndex" ) == 0 && pData->curElement == SAMSITE_ELEMENT_INDEX )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;

			pData->curSamInfo.uiIndex = atol( pData->szCharData );
			if ( !pData->curSamInfo.uiIndex || pData->curSamInfo.uiIndex > MAX_NUMBER_OF_SAMS )
			{
				pData->curSamInfo.uiIndex = INVALID_SAMSITE_INDEX;
			}
			else if ( pData->curSamInfo.uiIndex > pData->uiHighestIndex )
			{
				pData->uiHighestIndex = pData->curSamInfo.uiIndex;
			}
		}
		else if ( strcmp( name, "samSector" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMSECTOR )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMSECTOR_X )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMSECTOR;

			pData->curSamInfo.samSectorX = (INT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAMSECTOR_Y )
		{
			pData->curElement = SAMSITE_ELEMENT_SAMSECTOR;

			pData->curSamInfo.samSectorY = (INT16)atol( pData->szCharData );
		}
		else if ( strcmp( name, "samHidden" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM_HIDDEN )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;

			pData->curSamInfo.samHidden = (BOOLEAN)atol( pData->szCharData );
		}
		else if ( strcmp( name, "samOrientation" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM_ORIENTATION )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;

			pData->curSamInfo.samOrientation = (INT8)atol( pData->szCharData );
			if ( pData->curSamInfo.samOrientation != 3 && pData->curSamInfo.samOrientation != 4 )
			{
				pData->curSamInfo.samOrientation = 3;
			}
		}
		else if ( strcmp( name, "samGridNoA" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM_GRIDNO_A )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;

			pData->curSamInfo.samGridNoA = atol( pData->szCharData );
			if ( pData->curSamInfo.samGridNoA >= MAX_ALLOWED_WORLD_MAX )
			{
				pData->curSamInfo.samGridNoA = 0;
			}
		}
		else if ( strcmp( name, "samGridNoB" ) == 0 && pData->curElement == SAMSITE_ELEMENT_SAM_GRIDNO_B )
		{
			pData->curElement = SAMSITE_ELEMENT_SAM;

			pData->curSamInfo.samGridNoB = atol( pData->szCharData );
			if ( pData->curSamInfo.samGridNoB >= MAX_ALLOWED_WORLD_MAX )
			{
				pData->curSamInfo.samGridNoB = 1;
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN ReadInSAMInfo( STR fileName )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate( NULL );

	samsiteParseData pData;



	// Open weapons file
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


	XML_SetElementHandler( parser, samsiteStartElementHandle, samsiteEndElementHandle );
	XML_SetCharacterDataHandler( parser, samsiteCharacterDataHandle );


	memset( &pData, 0, sizeof(pData) );
	NUMBER_OF_SAMS = 0;
	XML_SetUserData( parser, &pData );


	if ( !XML_Parse( parser, lpcBuffer, uiFSize, TRUE ) )
	{
		CHAR8 errorBuf[511];

		sprintf( errorBuf, "XML Parser Error in SamSites.xml: %s at line %d", XML_ErrorString( XML_GetErrorCode( parser ) ), XML_GetCurrentLineNumber( parser ) );
		LiveMessage( errorBuf );

		MemFree( lpcBuffer );
		return FALSE;
	}

	MemFree( lpcBuffer );

	XML_ParserFree( parser );

	return TRUE;
}

BOOLEAN WriteInSAMInfo( STR fileName )
{
	HWFILE		hFile;

	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return(FALSE);

	{
		UINT32 x, y;
		INT8 cnt;


		FilePrintf( hFile, "<SAM_INFO>\r\n" );
		FilePrintf( hFile, "\t<SAMLIST>\r\n" );
		for ( cnt = 0; cnt < NUMBER_OF_SAMS; cnt++ )
		{
			FilePrintf( hFile, "\t\t<SAM>\r\n" );

			FilePrintf( hFile, "\t\t\t<samIndex>%d</samIndex>\r\n", cnt + 1 );

			FilePrintf( hFile, "\t\t\t<samSector>\r\n" );
			FilePrintf( hFile, "\t\t\t\t<x>%d</x>\r\n", (pSamList[cnt] % 16) + 1 );
			FilePrintf( hFile, "\t\t\t\t<y>%d</y>\r\n", (pSamList[cnt] / 16) + 1 );
			FilePrintf( hFile, "\t\t\t</samSector>\r\n" );

			FilePrintf( hFile, "\t\t\t<samHidden>%d</samHidden>\r\n", !fSamSiteFound[cnt] );

			FilePrintf( hFile, "\t\t\t<samOrientation>%d</samOrientation>\r\n", gbSAMGraphicList[cnt] );

			FilePrintf( hFile, "\t\t\t<samGridNoA>%d</samGridNoA>\r\n", pSamGridNoAList[cnt] );

			FilePrintf( hFile, "\t\t\t<samGridNoB>%d</samGridNoB>\r\n", pSamGridNoBList[cnt] );

			FilePrintf( hFile, "\t\t</SAM>\r\n" );
		}
		FilePrintf( hFile, "\t</SAMLIST>\r\n" );

		FilePrintf( hFile, "\t<SAM_CONTROLLED_SECTORS>\r\n" );
		for ( y = 0; y < MAP_WORLD_Y; y++ )
		{
			FilePrintf( hFile, "\t\t<MAP_ROW row=\"%02d\">", y );
			for ( x = 0; x < MAP_WORLD_X; x++ )
			{
				if ( x < MAP_WORLD_X - 1 )
				{
					FilePrintf( hFile, "%2d ", ubSAMControlledSectors[y][x] );
				}
				else
					FilePrintf( hFile, "%2d </MAP_ROW>\r\n", ubSAMControlledSectors[y][x] );
			}
		}
		FilePrintf( hFile, "\t</SAM_CONTROLLED_SECTORS>\r\n" );
		FilePrintf( hFile, "</SAM_INFO>\r\n" );
	}
	FileClose( hFile );

	return TRUE;
}


// town externalization stuff
INT8 NUM_TOWNS;

extern UINT8	gubTownRebelSentiment[MAX_TOWNS];
extern BOOLEAN	gfTownUsesLoyalty[MAX_TOWNS];
extern BOOLEAN	gfMilitiaAllowedInTown[MAX_TOWNS];
BOOLEAN	gfHiddenTown[MAX_TOWNS];				// Info: Visible town are TRUE, hidden towns are FALSE
BOOLEAN	gfDrawHiddenTown[MAX_TOWNS];
BOOLEAN	gfDrawHiddenTownTemp[MAX_TOWNS];
BOOLEAN	gfHiddenTownTemp[MAX_TOWNS];
BOOLEAN	gfIconTown[MAX_TOWNS];
BOOLEAN	gfIconTownTemp[MAX_TOWNS];

#define INVALID_TOWN_INDEX				-1

typedef enum
{
	CITYTABLE_ELEMENT_NONE = 0,
	CITYTABLE_ELEMENT_CITYINFO,
	CITYTABLE_ELEMENT_CITYTABLE,
	CITYTABLE_ELEMENT_CITYTABLEROW,
	CITYTABLE_ELEMENT_CITYLIST,
	CITYTABLE_ELEMENT_CITY,
	CITYTABLE_ELEMENT_INDEX,
	CITYTABLE_ELEMENT_NAME,
	CITYTABLE_ELEMENT_BASESECTOR,
	CITYTABLE_ELEMENT_BASESECTOR_X,
	CITYTABLE_ELEMENT_BASESECTOR_Y,
	CITYTABLE_ELEMENT_TOWNPOINT,
	CITYTABLE_ELEMENT_TOWNPOINT_X,
	CITYTABLE_ELEMENT_TOWNPOINT_Y,
	CITYTABLE_ELEMENT_USES_LOYALTY,
	CITYTABLE_ELEMENT_REBEL_SENTIMENT,
	CITYTABLE_ELEMENT_MILITIA,
	CITYTABLE_ELEMENT_HIDDENTOWN,
	CITYTABLE_ELEMENT_ICONTOWN,
	CITYTABLE_ELEMENT_ICONFILE,
	CITYTABLE_ELEMENT_ICON_POSITION,
	CITYTABLE_ELEMENT_ICON_POSITION_X,
	CITYTABLE_ELEMENT_ICON_POSITION_Y,
	CITYTABLE_ELEMENT_COUNTRYINFO,
	CITYTABLE_ELEMENT_COUNTRYNAME,
	CITYTABLE_ELEMENT_COUNTRYNOUN

} CITYTABLE_PARSE_STAGE;

typedef struct
{
	UINT32	uiIndex;
	UINT8	ubBaseX;
	UINT8	ubBaseY;
	POINT	townPoint;
	BOOLEAN townUsesLoyalty;
	UINT8	townRebelSentiment;
	BOOLEAN	townMilitiaAllowed;
	CHAR8	cityName[MAX_TOWN_NAME_LENGHT];
	BOOLEAN HiddenTown;
	BOOLEAN	TownIcon;
	CHAR8	IconSTI[MAX_ICON_CHARS];
	UINT8	ubPosIconX;
	UINT8	ubPosIconY;
} cityInfo;

typedef struct
{
	CITYTABLE_PARSE_STAGE	curElement;

	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH + 1];
	CHAR8					countryName[MAX_TOWN_NAME_LENGHT];
	CHAR8					countryNoun[MAX_TOWN_NAME_LENGHT];
	cityInfo				curCityInfo;

	//cityInfo *				curArray;	// ROMAN: added
	//UINT32					maxArraySize;	// ROMAN: added

	UINT32					uiRowNumber;
	UINT32					uiHighestIndex;

	UINT32					currentDepth;
	UINT32					maxReadDepth;
} citytableParseData;

static void XMLCALL
citytableStartElementHandle( void *userData, const XML_Char *name, const XML_Char **atts )
{
	citytableParseData * pData = (citytableParseData *)userData;

	memset( pData->szCharData, 0, sizeof(pData->szCharData) );

	if ( pData->currentDepth <= pData->maxReadDepth ) //are we reading this element?
	{
		if ( strcmp( name, "CITY_INFO" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_NONE )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYINFO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "CITY_TABLE" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYTABLE;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "CITY_TABLE_ROW" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYTABLE )
		{
			UINT32 uiAttrIndex;
			pData->curElement = CITYTABLE_ELEMENT_CITYTABLEROW;

			for ( uiAttrIndex = 0; atts[uiAttrIndex] != NULL; uiAttrIndex += 2 )
			if ( strcmp( atts[uiAttrIndex], "row" ) == 0 )
			{
				pData->uiRowNumber = atol( atts[uiAttrIndex + 1] );
				break;
			}

			if ( atts[uiAttrIndex] != NULL && pData->uiRowNumber < MAP_WORLD_Y )
				pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "CITYLIST" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYLIST;

			if ( !localizedMapTextOnly )
			{
				memset( sBaseSectorList, 0, sizeof(sBaseSectorList) );
				memset( pTownPoints, 0, sizeof(pTownPoints) );
				memset( gfTownUsesLoyalty, 0, sizeof(gfTownUsesLoyalty) );
				memset( gubTownRebelSentiment, 0, sizeof(gubTownRebelSentiment) );
				memset( gfMilitiaAllowedInTown, 0, sizeof(gfMilitiaAllowedInTown) );
				memset( gfHiddenTown, 0, sizeof(gfHiddenTown) );
				memset( gfHiddenTownTemp, 0, sizeof(gfHiddenTownTemp) );
				memset( gfIconTown, 0, sizeof(gfIconTown) );
				memset( gfIconTownTemp, 0, sizeof(gfIconTownTemp) );
				memset( gfDrawHiddenTown, 0, sizeof(gfDrawHiddenTown) );
				memset( gfDrawHiddenTownTemp, 0, sizeof(gfDrawHiddenTownTemp) );

				strncpy( gHiddenIcon[pData->curCityInfo.uiIndex].IconSti, pData->curCityInfo.IconSTI, MAX_ICON_CHARS );

				//if ( pData->curCityInfo.ubPosIconX < 0 )
				//	gHiddenIcon[pData->curCityInfo.uiIndex].IconX = 0;
				//else
				gHiddenIcon[pData->curCityInfo.uiIndex].IconX = pData->curCityInfo.ubPosIconX;

				//if ( pData->curCityInfo.ubPosIconY < 0 )
				//	gHiddenIcon[pData->curCityInfo.uiIndex].IconY = 0;
				//else
				gHiddenIcon[pData->curCityInfo.uiIndex].IconY = pData->curCityInfo.ubPosIconY;

			}

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "CITY" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYLIST )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			if ( !localizedMapTextOnly )
				memset( &pData->curCityInfo, 0, sizeof(cityInfo) );

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "townName" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_NAME;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "uiIndex" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_INDEX;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "townUsesLoyalty" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_USES_LOYALTY;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "townRebelSentiment" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_REBEL_SENTIMENT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "townMilitiaAllowed" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_MILITIA;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "hiddenTown" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_HIDDENTOWN;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "townIcon" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICONTOWN;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "szIconFile" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICONFILE;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if ( strcmp( name, "iconPosition" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICON_POSITION;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICON_POSITION )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICON_POSITION_X;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICON_POSITION )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICON_POSITION_Y;
			pData->maxReadDepth++; //we are not skipping this element
		}

		else if ( strcmp( name, "baseSector" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_BASESECTOR;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_BASESECTOR )
		{
			pData->curElement = CITYTABLE_ELEMENT_BASESECTOR_X;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_BASESECTOR )
		{
			pData->curElement = CITYTABLE_ELEMENT_BASESECTOR_Y;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "townPoint" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_TOWNPOINT;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_TOWNPOINT )
		{
			pData->curElement = CITYTABLE_ELEMENT_TOWNPOINT_X;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_TOWNPOINT )
		{
			pData->curElement = CITYTABLE_ELEMENT_TOWNPOINT_Y;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "COUNTRY" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_COUNTRYINFO;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "countryName" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_COUNTRYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_COUNTRYNAME;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if ( strcmp( name, "countryNoun" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_COUNTRYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_COUNTRYNOUN;
			pData->maxReadDepth++; //we are not skipping this element
		}

		//pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
citytableCharacterDataHandle( void *userData, const XML_Char *str, int len )
{
	citytableParseData * pData = (citytableParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth && strlen( pData->szCharData ) < MAX_CHAR_DATA_LENGTH )
		strncat( pData->szCharData, str, __min( (unsigned int)len, MAX_CHAR_DATA_LENGTH - strlen( pData->szCharData ) ) );
}


static void XMLCALL
citytableEndElementHandle( void *userData, const XML_Char *name )
{

	char temp;

	citytableParseData * pData = (citytableParseData *)userData;

	if ( pData->currentDepth <= pData->maxReadDepth ) //we're at the end of an element that we've been reading
	{
		if ( strcmp( name, "CITY_INFO" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_NONE;
		}
		else if ( strcmp( name, "CITY_TABLE" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYTABLE )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYINFO;
		}
		else if ( strcmp( name, "CITY_TABLE_ROW" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYTABLEROW )
		{
			STR8 curBuffer = pData->szCharData + strspn( pData->szCharData, " \t\n\r" );
			UINT32 curCellIndex = 0;
			UINT32 curNumber;

			pData->curElement = CITYTABLE_ELEMENT_CITYTABLE;

			while ( curBuffer[0] != '\0' )
			{
				sscanf( curBuffer, "%d", &curNumber );

				StrategicMap[curCellIndex + MAP_WORLD_X*pData->uiRowNumber].bNameId = curNumber;

				curCellIndex++;
				curBuffer += strcspn( curBuffer, " \t\n\r\0" );
				curBuffer += strspn( curBuffer, " \t\n\r" );
			}
		}
		else if ( strcmp( name, "CITYLIST" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITYLIST )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYINFO;

			NUM_TOWNS = pData->uiHighestIndex + 1;
		}
		else if ( strcmp( name, "CITY" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_CITY )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYLIST;

			if ( pData->curCityInfo.uiIndex != INVALID_TOWN_INDEX && !localizedMapTextOnly )
			{
				sBaseSectorList[pData->curCityInfo.uiIndex - 1] = SECTOR( pData->curCityInfo.ubBaseX, pData->curCityInfo.ubBaseY );
				pTownPoints[pData->curCityInfo.uiIndex] = pData->curCityInfo.townPoint;
				gfTownUsesLoyalty[pData->curCityInfo.uiIndex] = pData->curCityInfo.townUsesLoyalty;
				gubTownRebelSentiment[pData->curCityInfo.uiIndex] = pData->curCityInfo.townRebelSentiment;
				gfMilitiaAllowedInTown[pData->curCityInfo.uiIndex] = pData->curCityInfo.townMilitiaAllowed;
				//mbstowcs( pTownNames[pData->curCityInfo.uiIndex], pData->curCityInfo.cityName, MAX_TOWN_NAME_LENGHT);
				MultiByteToWideChar( CP_UTF8, 0, pData->curCityInfo.cityName, -1, pTownNames[pData->curCityInfo.uiIndex], MAX_TOWN_NAME_LENGHT );

				strncpy( gHiddenIcon[pData->curCityInfo.uiIndex].IconSti, pData->curCityInfo.IconSTI, MAX_ICON_CHARS );

				//if ( pData->curCityInfo.ubPosIconX < 0 )
				//	gHiddenIcon[pData->curCityInfo.uiIndex].IconX = 0;
				//else
				gHiddenIcon[pData->curCityInfo.uiIndex].IconX = pData->curCityInfo.ubPosIconX;

				//if ( pData->curCityInfo.ubPosIconY < 0 )
				//	gHiddenIcon[pData->curCityInfo.uiIndex].IconY = 0;
				//else
				gHiddenIcon[pData->curCityInfo.uiIndex].IconY = pData->curCityInfo.ubPosIconY;

				if ( pData->curCityInfo.HiddenTown == FALSE )
				{
					gfHiddenTown[pData->curCityInfo.uiIndex] = TRUE;
					gfHiddenTownTemp[pData->curCityInfo.uiIndex] = gfHiddenTown[pData->curCityInfo.uiIndex];
					gfDrawHiddenTown[pData->curCityInfo.uiIndex] = FALSE;
				}
				else
				{
					gfHiddenTown[pData->curCityInfo.uiIndex] = FALSE;
					gfHiddenTownTemp[pData->curCityInfo.uiIndex] = gfHiddenTown[pData->curCityInfo.uiIndex];
					gfDrawHiddenTown[pData->curCityInfo.uiIndex] = FALSE;
				}

				gfIconTown[pData->curCityInfo.uiIndex] = pData->curCityInfo.TownIcon;
				gfIconTownTemp[pData->curCityInfo.uiIndex] = gfIconTown[pData->curCityInfo.uiIndex];
			}
			else if ( pData->curCityInfo.uiIndex != INVALID_TOWN_INDEX && localizedMapTextOnly )
			{
				MultiByteToWideChar( CP_UTF8, 0, pData->curCityInfo.cityName, -1, pTownNames[pData->curCityInfo.uiIndex], MAX_TOWN_NAME_LENGHT );
			}
		}
		else if ( strcmp( name, "uiIndex" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_INDEX )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			pData->curCityInfo.uiIndex = atol( pData->szCharData );
			if ( !pData->curCityInfo.uiIndex || pData->curCityInfo.uiIndex >= MAX_TOWNS )
			{
				pData->curCityInfo.uiIndex = INVALID_TOWN_INDEX;
			}
			else if ( pData->curCityInfo.uiIndex > pData->uiHighestIndex )
			{
				pData->uiHighestIndex = pData->curCityInfo.uiIndex;
			}
		}
		else if ( strcmp( name, "townName" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_NAME )
		{
			strncpy( pData->curCityInfo.cityName, pData->szCharData, MAX_TOWN_NAME_LENGHT - 1 );
			pData->curCityInfo.cityName[MAX_TOWN_NAME_LENGHT - 1] = 0;

			pData->curElement = CITYTABLE_ELEMENT_CITY;
		}
		else if ( strcmp( name, "townUsesLoyalty" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_USES_LOYALTY )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			pData->curCityInfo.townUsesLoyalty = (BOOLEAN)atol( pData->szCharData );
		}
		else if ( strcmp( name, "townRebelSentiment" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_REBEL_SENTIMENT )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			pData->curCityInfo.townRebelSentiment = (UINT8)atol( pData->szCharData );
			if ( pData->curCityInfo.townRebelSentiment > 100 )
			{
				pData->curCityInfo.townRebelSentiment = 100;
			}
			else if ( !pData->curCityInfo.townRebelSentiment )
				pData->curCityInfo.townRebelSentiment = 1;
		}
		else if ( strcmp( name, "townMilitiaAllowed" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_MILITIA )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			pData->curCityInfo.townMilitiaAllowed = (BOOLEAN)atol( pData->szCharData );
		}
		else if ( strcmp( name, "hiddenTown" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_HIDDENTOWN )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			pData->curCityInfo.HiddenTown = (BOOLEAN)atol( pData->szCharData );
		}
		else if ( strcmp( name, "townIcon" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICONTOWN )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;

			pData->curCityInfo.TownIcon = (INT32)atol( pData->szCharData );
		}
		else if ( strcmp( name, "szIconFile" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICONFILE )
		{

			pData->curElement = CITYTABLE_ELEMENT_CITY;

			if ( MAX_ICON_CHARS >= strlen( pData->szCharData ) )
				strcpy( pData->curCityInfo.IconSTI, pData->szCharData );
			else
			{
				strncpy( pData->curCityInfo.IconSTI, pData->szCharData, MAX_ICON_CHARS );
				pData->curCityInfo.IconSTI[MAX_ICON_CHARS - 1] = '\0';
			}

			for ( int i = 0; i<min( (int)strlen( pData->szCharData ), MAX_ICON_CHARS ); i++ )
			{
				temp = pData->szCharData[i];
				pData->curCityInfo.IconSTI[i] = temp;
			}
		}

		else if ( strcmp( name, "iconPosition" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICON_POSITION )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICON_POSITION_X )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICON_POSITION;

			pData->curCityInfo.ubPosIconX = (INT32)atol( pData->szCharData );
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_ICON_POSITION_Y )
		{
			pData->curElement = CITYTABLE_ELEMENT_ICON_POSITION;

			pData->curCityInfo.ubPosIconY = (INT32)atol( pData->szCharData );
		}

		else if ( strcmp( name, "baseSector" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_BASESECTOR )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_BASESECTOR_X )
		{
			pData->curElement = CITYTABLE_ELEMENT_BASESECTOR;

			pData->curCityInfo.ubBaseX = (UINT8)atol( pData->szCharData );
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_BASESECTOR_Y )
		{
			pData->curElement = CITYTABLE_ELEMENT_BASESECTOR;

			pData->curCityInfo.ubBaseY = (UINT8)atol( pData->szCharData );
		}
		else if ( strcmp( name, "townPoint" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_TOWNPOINT )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITY;
		}
		else if ( strcmp( name, "x" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_TOWNPOINT_X )
		{
			pData->curElement = CITYTABLE_ELEMENT_TOWNPOINT;

			pData->curCityInfo.townPoint.x = atol( pData->szCharData );
		}
		else if ( strcmp( name, "y" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_TOWNPOINT_Y )
		{
			pData->curElement = CITYTABLE_ELEMENT_TOWNPOINT;

			pData->curCityInfo.townPoint.y = atol( pData->szCharData );
		}
		else if ( strcmp( name, "COUNTRY" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_COUNTRYINFO )
		{
			pData->curElement = CITYTABLE_ELEMENT_CITYINFO;

			if ( !localizedMapTextOnly )
			{
				MultiByteToWideChar( CP_UTF8, 0, pData->countryName, -1, pCountryNames[COUNTRY_NAME], MAX_TOWN_NAME_LENGHT );
				MultiByteToWideChar( CP_UTF8, 0, pData->countryNoun, -1, pCountryNames[COUNTRY_NOUN], MAX_TOWN_NAME_LENGHT );

			}
			else if ( localizedMapTextOnly )
			{
				MultiByteToWideChar( CP_UTF8, 0, pData->countryName, -1, pCountryNames[COUNTRY_NAME], MAX_TOWN_NAME_LENGHT );
				MultiByteToWideChar( CP_UTF8, 0, pData->countryNoun, -1, pCountryNames[COUNTRY_NOUN], MAX_TOWN_NAME_LENGHT );
			}
		}
		else if ( strcmp( name, "countryName" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_COUNTRYNAME )
		{
			strncpy( pData->countryName, pData->szCharData, MAX_TOWN_NAME_LENGHT - 1 );
			pData->countryName[MAX_TOWN_NAME_LENGHT - 1] = 0;

			pData->curElement = CITYTABLE_ELEMENT_COUNTRYINFO;
		}
		else if ( strcmp( name, "countryNoun" ) == 0 && pData->curElement == CITYTABLE_ELEMENT_COUNTRYNOUN )
		{
			strncpy( pData->countryNoun, pData->szCharData, MAX_TOWN_NAME_LENGHT - 1 );
			pData->countryNoun[MAX_TOWN_NAME_LENGHT - 1] = 0;

			pData->curElement = CITYTABLE_ELEMENT_COUNTRYINFO;
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}

BOOLEAN WriteInStrategicMapSectorTownNames( STR fileName )
{
	HWFILE		hFile;

	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return(FALSE);

	{
		UINT32 x, y;
		INT8 cnt;


		FilePrintf( hFile, "<CITY_INFO>\r\n" );
		FilePrintf( hFile, "\t<CITYLIST>\r\n" );
		for ( cnt = 1; cnt < NUM_TOWNS; cnt++ )
		{
			FilePrintf( hFile, "\t\t<CITY>\r\n" );

			FilePrintf( hFile, "\t\t\t<uiIndex>%d</uiIndex>\r\n", cnt );

			FilePrintf( hFile, "\t\t\t<townName>%S</townName>\r\n", pTownNames[cnt] );

			FilePrintf( hFile, "\t\t\t<townUsesLoyalty>%d</townUsesLoyalty>\r\n", gfTownUsesLoyalty[cnt] );

			FilePrintf( hFile, "\t\t\t<townRebelSentiment>%d</townRebelSentiment>\r\n", gubTownRebelSentiment[cnt] );

			FilePrintf( hFile, "\t\t\t<townMilitiaAllowed>%d</townMilitiaAllowed>\r\n", gfMilitiaAllowedInTown[cnt] );

			if ( gfHiddenTown[cnt] == TRUE )
				FilePrintf( hFile, "\t\t\t<hiddenTown>%d</hiddenTown>\r\n", 0 );
			else
				FilePrintf( hFile, "\t\t\t<hiddenTown>%d</hiddenTown>\r\n", 1 );

			FilePrintf( hFile, "\t\t\t<townIcon>%d</townIcon>\r\n", gfIconTown[cnt] );

			FilePrintf( hFile, "\t\t\t<baseSector>\r\n" );
			FilePrintf( hFile, "\t\t\t\t<x>%d</x>\r\n", (sBaseSectorList[cnt - 1] % 16) + 1 );
			FilePrintf( hFile, "\t\t\t\t<y>%d</y>\r\n", (sBaseSectorList[cnt - 1] / 16) + 1 );
			FilePrintf( hFile, "\t\t\t</baseSector>\r\n" );


			FilePrintf( hFile, "\t\t\t<townPoint>\r\n" );
			FilePrintf( hFile, "\t\t\t\t<x>%d</x>\r\n", pTownPoints[cnt].x );
			FilePrintf( hFile, "\t\t\t\t<y>%d</y>\r\n", pTownPoints[cnt].y );
			FilePrintf( hFile, "\t\t\t</townPoint>\r\n" );


			FilePrintf( hFile, "\t\t</CITY>\r\n" );
		}
		FilePrintf( hFile, "\t</CITYLIST>\r\n" );

		FilePrintf( hFile, "\t<CITY_TABLE>\r\n" );
		for ( y = 0; y < MAP_WORLD_Y; y++ )
		{
			FilePrintf( hFile, "\t\t<CITY_TABLE_ROW row=\"%02d\">", y );
			for ( x = 0; x < MAP_WORLD_X; x++ )
			{
				if ( x < MAP_WORLD_X - 1 )
				{
					FilePrintf( hFile, "%3d", StrategicMap[x + y*MAP_WORLD_X].bNameId );
				}
				else
					FilePrintf( hFile, "%3d </CITY_TABLE_ROW>\r\n", StrategicMap[x + y*MAP_WORLD_X].bNameId );
			}
		}
		FilePrintf( hFile, "\t</CITY_TABLE>\r\n" );
		FilePrintf( hFile, "</CITY_INFO>\r\n" );
	}
	FileClose( hFile );

	return TRUE;
}

BOOLEAN ReadInStrategicMapSectorTownNames( STR fileName, BOOLEAN localizedVersion )
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate( NULL );

	citytableParseData pData;

	localizedMapTextOnly = localizedVersion;

	// Open weapons file
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


	XML_SetElementHandler( parser, citytableStartElementHandle, citytableEndElementHandle );
	XML_SetCharacterDataHandler( parser, citytableCharacterDataHandle );


	// TODO: ROMAN
	//pData.curCityInfo


	memset( &pData, 0, sizeof(pData) );
	NUM_TOWNS = 0;
	XML_SetUserData( parser, &pData );


	if ( !XML_Parse( parser, lpcBuffer, uiFSize, TRUE ) )
	{
		CHAR8 errorBuf[511];

		sprintf( errorBuf, "XML Parser Error in Cities.xml: %s at line %d", XML_ErrorString( XML_GetErrorCode( parser ) ), XML_GetCurrentLineNumber( parser ) );
		LiveMessage( errorBuf );

		MemFree( lpcBuffer );
		return FALSE;
	}

	MemFree( lpcBuffer );

	XML_ParserFree( parser );

	return TRUE;
}

BOOLEAN ReadInMapStructure( STR fileName, BOOLEAN localizedVersion )
{
	memset( StrategicMap, 0, sizeof(StrategicMap) );

	return ReadInStrategicMapSectorTownNames( fileName, localizedVersion );
}

UINT32 UndergroundTacticalTraversalTime( INT8 bExitDirection )
{
	//We are attempting to traverse in an underground environment.  We need to use a complete different
	//method.  When underground, all sectors are instantly adjacent.
	switch ( bExitDirection )
	{
	case NORTH_STRATEGIC_MOVE:
		if ( gMapInformation.sNorthGridNo != -1 )
			return 0;
		break;
	case EAST_STRATEGIC_MOVE:
		if ( gMapInformation.sEastGridNo != -1 )
			return 0;
		break;
	case SOUTH_STRATEGIC_MOVE:
		if ( gMapInformation.sSouthGridNo != -1 )
			return 0;
		break;
	case WEST_STRATEGIC_MOVE:
		if ( gMapInformation.sWestGridNo != -1 )
			return 0;
		break;
	}
	return 0xffffffff;
}

// WANNE: Zooming Animation of the laptop in strategic screen
void BeginLoadScreen( void )
{
	SGPRect SrcRect, DstRect;
	UINT32 uiStartTime, uiCurrTime;
	INT32 iPercentage, iFactor;
	UINT32 uiTimeRange;
	INT32 iLastShadePercentage;
	UINT8 ubLoadScreenID;

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

#ifdef JA2UB
	if ( guiCurrentScreen == MAP_SCREEN && !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
#else
	//Ja25: No meanwhiles
	if ( guiCurrentScreen == MAP_SCREEN && !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) && !AreInMeanwhile( ) )
#endif
	{
		if ( !gGameExternalOptions.fDisableStrategicTransition )
		{
			DstRect.iLeft = 0;
			DstRect.iTop = 0;
			DstRect.iRight = SCREEN_WIDTH;
			DstRect.iBottom = SCREEN_HEIGHT;
			uiTimeRange = 2000;
			iPercentage = 0;
			iLastShadePercentage = 0;
			uiStartTime = GetJA2Clock( );

			BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
			PlayJA2SampleFromFile( "SOUNDS\\Final Psionic Blast 01 (16-44).wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
			while ( iPercentage < 100 )
			{
				uiCurrTime = GetJA2Clock( );
				iPercentage = (uiCurrTime - uiStartTime) * 100 / uiTimeRange;
				iPercentage = min( iPercentage, 100 );

				//Factor the percentage so that it is modified by a gravity falling acceleration effect.
				iFactor = (iPercentage - 50) * 2;
				if ( iPercentage < 50 )
					iPercentage = (UINT32)(iPercentage + iPercentage * iFactor * 0.01 + 0.5);
				else
					iPercentage = (UINT32)(iPercentage + (100 - iPercentage) * iFactor * 0.01 + 0.05);

				if ( iPercentage > 50 )
				{
					ShadowVideoSurfaceRectUsingLowPercentTable( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
				}

				SrcRect.iLeft = 536 * iPercentage / 100;
				SrcRect.iRight = SCREEN_WIDTH - iPercentage / 20;
				SrcRect.iTop = 367 * iPercentage / 100;
				SrcRect.iBottom = SCREEN_HEIGHT - 39 * iPercentage / 100;

				BltStretchVideoSurface( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, 0, &SrcRect, &DstRect );
				InvalidateScreen( );
				RefreshScreen( NULL );
			}
		}
	}

	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	InvalidateScreen( );
	RefreshScreen( NULL );

#ifdef JA2UB	
	//if we are going to the intro screen, return before putting up a loadscreen
	if ( gbExitingMapScreenToWhere == MAP_EXIT_TO_INTRO_SCREEN )
	{
		SetPendingNewScreen( INTRO_SCREEN );
		return;
	}
#endif

	//If we are loading a saved game, use the Loading screen we saved into the SavedGameHeader file
	// ( which gets reloaded into gubLastLoadingScreenID )
	if ( !gfGotoSectorTransition )
	{
		if ( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			DisplayLoadScreenWithID( gubLastLoadingScreenID );
		}
		else
		{
			ubLoadScreenID = GetLoadScreenID( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			DisplayLoadScreenWithID( ubLoadScreenID );
		}
	}

#ifdef JA2TESTVERSION
	uiEnterSectorStartTime = 0;
	uiEnterSectorEndTime = 0;
	//The grand total time for loading a map
	uiLoadWorldStartTime = 0;
	uiLoadWorldEndTime = 0;
	//The time spent in FileRead
	uiTotalFileReadTime = 0;
	uiTotalFileReadCalls = 0;
	//Sections of LoadWorld
	uiLoadWorldTime = 0;
	uiTrashWorldTime = 0;
	uiLoadMapTilesetTime = 0;
	uiLoadMapLightsTime = 0;
	uiBuildShadeTableTime = 0;
	uiEnterSectorStartTime = GetJA2Clock( );
#endif

}

void EndLoadScreen( )
{
#ifdef JA2TESTVERSION
	//Report the time it took to load the map.  This is temporary until we are satisfied with the time
	//it takes to load the map.
	CHAR16 str[60];
	UINT32 uiSeconds;
	UINT32 uiHundreths;
	UINT32 uiUnaccounted;
	UINT32 uiPercentage;
	uiEnterSectorEndTime = GetJA2Clock( );
	uiSeconds = (uiEnterSectorEndTime - uiEnterSectorStartTime) / 1000;
	uiHundreths = ((uiEnterSectorEndTime - uiEnterSectorStartTime) / 10) % 100;
	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_YELLOW );
	SetFontBackground( FONT_NEARBLACK );
	if ( !gbWorldSectorZ )
	{
		swprintf( str, L"%c%d ENTER SECTOR TIME:  %d.%02d seconds.",
				  'A' + gWorldSectorY - 1, gWorldSectorX, uiSeconds, uiHundreths );
	}
	else
	{
		swprintf( str, L"%c%d_b%d ENTER SECTOR TIME:  %d.%02d seconds.",
				  'A' + gWorldSectorY - 1, gWorldSectorX, gbWorldSectorZ, uiSeconds, uiHundreths );
	}
	ScreenMsg( FONT_YELLOW, MSG_TESTVERSION, str );
	sgp::Logger_ID time_log_id = sgp::Logger::instance( ).createLogger( );
	sgp::Logger::instance( ).connectFile( time_log_id, L"TimeResults.txt", true, sgp::Logger::FLUSH_ON_ENDL );
	sgp::Logger::LogInstance timeResults = SGP_LOG( time_log_id );

	ScreenMsg( FONT_YELLOW, MSG_TESTVERSION, L"See JA2\\Data\\TimeResults.txt for more detailed timings." );

	//Record all of the timings.
	timeResults << str << sgp::endl;
	timeResults << "EnterSector() supersets LoadWorld().  This includes other external sections." << vfs::Log::endl;
	//FileRead()
	timeResults << sgp::endl << vfs::Log::endl << "VARIOUS FUNCTION TIMINGS (exclusive of actual function timings in second heading)" << sgp::endl;
	uiSeconds = uiTotalFileReadTime / 1000;
	uiHundreths = (uiTotalFileReadTime / 10) % 100;
	//fprintf( fp, "FileRead:  %d.%02d (called %d times)\n", uiSeconds, uiHundreths, uiTotalFileReadCalls );
	timeResults << "FileRead:  " << uiSeconds << "." << uiHundreths << " (called " << uiTotalFileReadCalls << " times)" << sgp::endl;

	timeResults << sgp::endl << sgp::endl << "SECTIONS OF LOADWORLD (all parts should add up to 100%)" << sgp::endl;

	//TrashWorld()
	uiSeconds = uiTrashWorldTime / 1000;
	uiHundreths = (uiTrashWorldTime / 10) % 100;
	timeResults << "TrashWorld: " << uiSeconds << "." << uiHundreths << sgp::endl;
	//LoadMapTilesets()
	uiSeconds = uiLoadMapTilesetTime / 1000;
	uiHundreths = (uiLoadMapTilesetTime / 10) % 100;
	timeResults << "LoadMapTileset: " << uiSeconds << "." << uiHundreths << sgp::endl;
	//LoadMapLights()
	uiSeconds = uiLoadMapLightsTime / 1000;
	uiHundreths = (uiLoadMapLightsTime / 10) % 100;
	timeResults << "LoadMapLights: " << uiSeconds << "." << uiHundreths << sgp::endl;

	uiSeconds = uiBuildShadeTableTime / 1000;
	uiHundreths = (uiBuildShadeTableTime / 10) % 100;
	timeResults << "  1)  BuildShadeTables: " << uiSeconds << "." << uiHundreths << sgp::endl;

	uiPercentage = uiNumImagesReloaded * 100 / giNumberOfTileTypes;
	timeResults << "  2)  " << uiPercentage << "% of the tileset images were actually reloaded." << sgp::endl;
	if ( (uiNumTablesSaved + uiNumTablesLoaded) != 0 )
	{
		uiPercentage = uiNumTablesSaved * 100 / (uiNumTablesSaved + uiNumTablesLoaded);
	}
	else
	{
		uiPercentage = 0;
	}
	timeResults << "  3)  Of that, " << uiPercentage << "% of the shade tables were generated (not loaded)." << sgp::endl;
	if ( gfForceBuildShadeTables )
	{
		timeResults << "  NOTE:  Force building of shadetables enabled on this local computer." << sgp::endl;
	}

	//Unaccounted
	uiUnaccounted = uiLoadWorldTime - uiTrashWorldTime - uiLoadMapTilesetTime - uiLoadMapLightsTime;
	uiSeconds = uiUnaccounted / 1000;
	uiHundreths = (uiUnaccounted / 10) % 100;
	timeResults << "Unaccounted: " << uiSeconds << "." << uiHundreths << sgp::endl;
	//LoadWorld()
	uiSeconds = uiLoadWorldTime / 1000;
	uiHundreths = (uiLoadWorldTime / 10) % 100;
	timeResults << vfs::Log::endl << "Total: " << uiSeconds << "." << uiHundreths << sgp::endl;
#endif
}


BOOLEAN InitStrategicEngine( )
{
	// this runs every time we start the application, so don't put anything in here that's only supposed to run when a new
	// *game* is started!  Those belong in InitStrategicLayer() instead.

	// set up town stuff
	BuildListOfTownSectors( );

	// town distances are pre-calculated and read in from a data file
	// since it takes quite a while to plot strategic paths between all pairs of town sectors...


	//#define RECALC_TOWN_DISTANCES
	//#ifdef RECALC_TOWN_DISTANCES
	CalcDistancesBetweenTowns( );
	//	WriteOutDistancesBetweenTowns( );
	//	DumpDistancesBetweenTowns( );
	//#endif

	//	ReadInDistancesBetweenTowns( );

	return(TRUE);
}


UINT8 GetTownIdForSector( INT16 sMapX, INT16 sMapY )
{
	// return the name value of the town in this sector

	return(StrategicMap[CALCULATE_STRATEGIC_INDEX( sMapX, sMapY )].bNameId);
}


// return number of sectors this town takes up
UINT8 GetTownSectorSize( INT8 bTownId )
{
	UINT8 ubSectorSize = 0;
	INT32 iCounterA = 0, iCounterB = 0;

	for ( iCounterA = 0; iCounterA < (INT32)(MAP_WORLD_X - 1); ++iCounterA )
	{
		for ( iCounterB = 0; iCounterB < (INT32)(MAP_WORLD_Y - 1); ++iCounterB )
		{
			if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( iCounterA, iCounterB )].bNameId == bTownId )
			{
				++ubSectorSize;
			}
		}
	}

	return(ubSectorSize);
}


// return number of sectors under player control for this town
UINT8 GetTownSectorsUnderControl( INT8 bTownId )
{
	INT8 ubSectorsControlled = 0;
	UINT16 usSector = 0;

	for ( INT32 iCounterA = 0; iCounterA < (INT32)(MAP_WORLD_X - 1); ++iCounterA )
	{
		for ( INT32 iCounterB = 0; iCounterB < (INT32)(MAP_WORLD_Y - 1); ++iCounterB )
		{
			usSector = (UINT16)CALCULATE_STRATEGIC_INDEX( iCounterA, iCounterB );

			if ( (StrategicMap[usSector].bNameId == bTownId) && !StrategicMap[usSector].fEnemyControlled &&
				 (NumNonPlayerTeamMembersInSector( (INT16)iCounterA, (INT16)iCounterB, ENEMY_TEAM ) == 0) )
			{
				++ubSectorsControlled;
			}
		}
	}

	return(ubSectorsControlled);
}

// Flugente: external sector data
extern SECTOR_EXT_DATA	SectorExternalData[256][4];


UINT16 GetTownWorkers(INT8 bTownId, UINT16& arusMax)
{
	arusMax = 0;

	UINT16 workerspresent = 0;

	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			UINT16 strategicsector = CALCULATE_STRATEGIC_INDEX( sX, sY );

			if ( StrategicMap[strategicsector].bNameId == bTownId )
			{
				// if we check for the player team, we look for sectors free of the enemy
				if ( StrategicMap[strategicsector].fEnemyControlled )
					continue;

				UINT8 sector = SECTOR( sX, sY );
				SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

				if ( !pSectorInfo )
					continue;

				workerspresent += pSectorInfo->usWorkers;

				arusMax += SectorExternalData[sector][0].maxworkers;
			}
		}
	}

	return workerspresent;
}

void AddTownWorkers( INT8 bTownId, INT16 asAdd )
{
	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			UINT16 strategicsector = CALCULATE_STRATEGIC_INDEX( sX, sY );

			if ( StrategicMap[strategicsector].bNameId == bTownId )
			{
				// if we check for the player team, we look for sectors free of the enemy
				if ( StrategicMap[strategicsector].fEnemyControlled )
					continue;

				UINT8 sector = SECTOR( sX, sY );
				SECTORINFO *pSectorInfo = &(SectorInfo[sector]);

				if ( !pSectorInfo )
					continue;

				if ( asAdd > 0 )
				{
					UINT16 add = min( asAdd, SectorExternalData[sector][0].maxworkers - pSectorInfo->usWorkers );

					asAdd -= add;
					pSectorInfo->usWorkers += add;
				}
				else if ( asAdd < 0 )
				{
					UINT16 remove = min( -asAdd, pSectorInfo->usWorkers );

					asAdd += remove;
					pSectorInfo->usWorkers -= remove;
				}

				if ( !asAdd )
					return;
			}
		}
	}
}

void InitializeSAMSites( void )
{
	// move the landing zone over to Omerta
	// HEADROCK HAM 3.5: Externalized coordinates
	gsMercArriveSectorX = gGameExternalOptions.ubDefaultArrivalSectorX;
	gsMercArriveSectorY = gGameExternalOptions.ubDefaultArrivalSectorY;

	// all SAM sites start game in perfect working condition
	for ( UINT32 cnt = 0; cnt < NUMBER_OF_SAMS; ++cnt )
	{
		StrategicMap[CALCULATE_STRATEGIC_INDEX( gpSamSectorX[cnt], gpSamSectorY[cnt] )].bSAMCondition = 100;
		StrategicMap[CALCULATE_STRATEGIC_INDEX( gpSamSectorX[cnt], gpSamSectorY[cnt] )].sSamHackStatus = 100;
	}

	// Flugente, bizarrely enough, this structure is not NULLED when starting a new campaign. For now, we NULL the flagmask to clear at least that
	for ( INT32 cnt = 0; cnt < MAP_WORLD_X*MAP_WORLD_Y; ++cnt )
	{
		StrategicMap[cnt].usFlags = 0;
	}

	UpdateAirspaceControl( );
}


// get short sector name without town name
void GetShortSectorString( INT16 sMapX, INT16 sMapY, STR16 sString )
{
	// OK, build string id like J11
	swprintf( sString, L"%S%S", pVertStrings[sMapY], pHortStrings[sMapX] );
}


void GetMapFileName( INT16 sMapX, INT16 sMapY, INT8 bSectorZ, STR8 bString, BOOLEAN fUsePlaceholder, BOOLEAN fAddAlternateMapLetter )
{
	CHAR8	 bTestString[150];
	CHAR8	 bExtensionString[15];

	if ( bSectorZ != 0 )
	{
		sprintf( bExtensionString, "_b%d", bSectorZ );
	}
	else
	{
		strcpy( bExtensionString, "" );
	}

	// the gfUseAlternateMap flag is set in the loading saved games.  When starting a new game the underground sector
	//info has not been initialized, so we need the flag to load an alternate sector.
	if ( gfUseAlternateMap | GetSectorFlagStatus( sMapX, sMapY, bSectorZ, SF_USE_ALTERNATE_MAP ) )
	{
		gfUseAlternateMap = FALSE;

		//if we ARE to use the a map, or if we are saving AND the save game version is before 80, add the a
		if ( fAddAlternateMapLetter )
		{
			strcat( bExtensionString, "_a" );
		}
	}

#ifdef JA2UB
	/* Ja25: No meanwhiles */
#else
	// If we are in a meanwhile...
	if ( AreInMeanwhile( ) && sMapX == gModSettings.ubMeanwhilePalaceSectorX && sMapY == gModSettings.ubMeanwhilePalaceSectorY && !bSectorZ ) //GetMeanwhileID() != INTERROGATION ), (3, 16)
	{
		if ( fAddAlternateMapLetter )
		{
			strcat( bExtensionString, "_m" );
		}
	}
#endif

	// This is the string to return, but...
	sprintf( bString, "%s%s%s.DAT", pVertStrings[sMapY], pHortStrings[sMapX], bExtensionString );

	// We will test against this string
	sprintf( bTestString, "MAPS\\%s", bString );

	if ( fUsePlaceholder && !FileExists( bTestString ) )
	{
		// Debug str
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Map does not exist for %s, using default.", bTestString ) );
		// Set to a string we know!
		sprintf( bString, "H10.DAT");
		ScreenMsg( FONT_YELLOW, MSG_DEBUG, L"Using PLACEHOLDER map!" );
	}
	return;
}

void GetCurrentWorldSector( INT16 *psMapX, INT16 *psMapY )
{
	*psMapX = gWorldSectorX;
	*psMapY = gWorldSectorY;
}

//not in overhead.h!
extern UINT8 NumEnemyInSector( );

void HandleRPCDescriptionOfSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT32	cnt;
	UINT8		ubSectorDescription[33][3] =
	{
		// row (letter), column, quote #
		{2, 13, 0},	// b13	Drassen
		{3, 13, 1},	// c13	Drassen
		{4, 13, 2},	// d13	Drassen
		{8, 13, 3},	// h13	Alma
		{8, 14, 4},	// h14	Alma
		{9, 13, 5},	// i13	Alma * (extra quote 6 if Sci-fi ) *
		{9, 14, 7},	// i14	Alma
		{6, 8, 8},	// f8	Cambria
		{6, 9, 9}, 	// f9	Cambria
		{7, 8, 10},	// g8	Cambria

		{7, 9, 11},	// g9	Cambria
		{3, 6, 12},	// c6	San Mona
		{3, 5, 13},	// c5	San Mona
		{4, 5, 14},	// d5	San Mona
		{2, 2, 15},	// b2	Chitzena
		{1, 2, 16},	// a2	Chitzena
		{7, 1, 17},	// g1	Grumm
		{8, 1, 18},	// h1	Grumm
		{7, 2, 19},	// g2 	Grumm
		{8, 2, 20},	// h2	Grumm

		{9, 6, 21},	// i6	Estoni
		{11, 4, 22},	// k4	Orta
		{12, 11, 23},	// l11	Balime
		{12, 12, 24},	// l12	Balime
		{15, 3, 25},	// o3	Meduna
		{16, 3, 26},	// p3	Meduna
		{14, 4, 27},	// n4	Meduna
		{14, 3, 28},	// n3	Meduna
		{15, 4, 30},	// o4	Meduna
		{10, 9, 31},	// j9	Tixa

		{4, 15, 32},	// d15	NE SAM
		{4, 2, 33},	// d2	NW SAM
		{9, 8, 34}	// i8	CENTRAL SAM
	};

	// Default to false
	gTacticalStatus.fCountingDownForGuideDescription = FALSE;
	
	// OK, if the first time in...
	if ( GetSectorFlagStatus( sSectorX, sSectorY, (UINT8)sSectorZ, SF_HAVE_USED_GUIDE_QUOTE ) != TRUE )
	{
		// guide quotes only happen on the surface
		if ( sSectorZ == 0 )
		{
			// OK, check if we are in a good sector....
			for ( cnt = 0; cnt < 33; ++cnt )
			{
				if ( sSectorX == ubSectorDescription[cnt][1] && sSectorY == ubSectorDescription[cnt][0] )
				{
					if ( cnt == 3 && ( gGameOptions.ubGameStyle == STYLE_REALISTIC || !gGameExternalOptions.fEnableCrepitus ) )
						continue;

					/*
					// If we're not scifi, skip some
					if ( !(gGameOptions.ubGameStyle == STYLE_SCIFI && gGameExternalOptions.fEnableCrepitus) && cnt == 3 )
					{
					continue;
					}
					*/

					gTacticalStatus.fCountingDownForGuideDescription = TRUE;
					gTacticalStatus.bGuideDescriptionCountDown = (INT8)( 4 + Random( 5 ) ); // 4 to 8 tactical turns...
					gTacticalStatus.ubGuideDescriptionToUse = ubSectorDescription[cnt][2];
					gTacticalStatus.bGuideDescriptionSectorX = (INT8)sSectorX;
					gTacticalStatus.bGuideDescriptionSectorY = (INT8)sSectorY;

					break;
				}
			}
		}

		// Flugente: We abuse this mechanic to also set up lua-based comments. We only play those if there are no normal comments
		if ( !gTacticalStatus.fCountingDownForGuideDescription )
		{
			gTacticalStatus.fCountingDownForGuideDescription = TRUE;
			gTacticalStatus.bGuideDescriptionCountDown = (INT8)( 2 + Random( 3 ) );

			// this value isn't used for the normal guide quotes - this way we know this is meant for lua only
			gTacticalStatus.ubGuideDescriptionToUse = 100;
			
			gTacticalStatus.bGuideDescriptionSectorX = (INT8)sSectorX;
			gTacticalStatus.bGuideDescriptionSectorY = (INT8)sSectorY;
		}
	}

	// Flugente: set this flag in any case
	SetSectorFlag( sSectorX, sSectorY, (UINT8)sSectorZ, SF_HAVE_USED_GUIDE_QUOTE );
		
	// Handle guide description ( will be needed if a SAM one )
#ifdef JA2UB
	//UB
#else
	HandleRPCDescription( );
#endif
}


BOOLEAN	SetCurrentWorldSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	BOOLEAN									fChangeMusic = TRUE;

#ifdef CRIPPLED_VERSION
	if ( sMapY >= 5 && sMapY != 16 )
	{
		CrippledVersionFailureToLoadMapCheck( );
		return FALSE;
	}
#endif

	// ATE: Zero out accounting functions
	memset( gbMercIsNewInThisSector, 0, sizeof(gbMercIsNewInThisSector) );

	SyncStrategicTurnTimes( );

#ifdef JA2BETAVERSION
	if ( gfOverrideSector )
	{
		//skip the cancel, and force load the sector.  This is used by the AIViewer to "reset" a level with
		//different numbers of various types of enemies.
	}
	else
#endif
		// reset goggle warning
		gogglewarning = FALSE;
	// is the sector already loaded?
	if ( (gWorldSectorX == sMapX) && (sMapY == gWorldSectorY) && (bMapZ == gbWorldSectorZ) )
	{
		//Inserts the enemies into the already loaded map based on the strategic information.
		//Note, the flag will return TRUE only if enemies were added.  The game may wish to
		//do something else in a case where no enemies are present.

		PrepareMilitiaForTactical( FALSE );

		SetPendingNewScreen( GAME_SCREEN );
		if ( !NumEnemyInSector( ) )
		{
			if ( is_networked )
			{
				if ( is_server && gEnemyEnabled == 1 )
					PrepareEnemyForSectorBattle( );
			}
			else
			{
				PrepareEnemyForSectorBattle( );
			}
		}

		for ( int i = 0; i<TOTAL_SOLDIERS; ++i )
		{
			//CHRISL: We should only bother with this assertion if the soldier is alive.  Dead soliders are moved to
			//	GridNo = NOWHERE, which causes this assertion to fail
			//CHRISL: There's also an issue with vehicles.  Soldiers in any vehicle are considered to be in sGridNo = NOWHERE
			//	This will result in an assertion error, so let's skip the assertion if the merc is assigned to a vehicle
			if (!(MercPtrs[i]->flags.uiStatusFlags & SOLDIER_DEAD) && MercPtrs[i]->bAssignment != VEHICLE && !SPY_LOCATION(MercPtrs[i]->bAssignment) && MercPtrs[i]->bAssignment != ASSIGNMENT_POW)
			{
				//Assert( !MercPtrs[i]->bActive || !MercPtrs[i]->bInSector || MercPtrs[i]->sGridNo != NOWHERE || MercPtrs[i]->bVehicleID == iHelicopterVehicleId );
				Assert( !MercPtrs[i]->bActive || !MercPtrs[i]->bInSector || !TileIsOutOfBounds( MercPtrs[i]->sGridNo ) || MercPtrs[i]->bVehicleID == iHelicopterVehicleId );
			}
		}

		if ( gubNumCreaturesAttackingTown && !gbWorldSectorZ &&
			 gubSectorIDOfCreatureAttack == SECTOR( gWorldSectorX, gWorldSectorY ) )
		{
			if ( is_networked )
			{
				if ( is_server && gCreatureEnabled == 1 )
					PrepareCreaturesForBattle( );
			}
			else
			{
				PrepareCreaturesForBattle( );
			}
		}

		if ( gfGotoSectorTransition )
		{
			BeginLoadScreen( );
			gfGotoSectorTransition = FALSE;
		}

		// Check for helicopter being on the ground in this sector...
		HandleHelicopterOnGroundGraphic( );

		HandleEnemyHelicopterOnGroundGraphic( );

		// 0verhaul:  Okay, it is apparent that the enemies are not reset correctly.  So I will now try to add symmetry
		// between enemy placement and militia placement.  The enemies do have one advantage here, though:  If a sector
		// is in enemy hands, then their sector is not actually loaded.  At least not normally.  Perhaps it would be useful
		// to lose a battle with one group of mercs, then bring in another group without changing to another sector to see
		// if enemy integrity holds.  But this would require the enemies to come up with reinforcements inbetween the battles
		// to really test out.
		//
		// Anyway, for now I will remove this call from here.  The objective is to add militia any time we could add enemies
		// except for the case of training new militia.  But that case can be handled each time militia training finishes.
		//			ResetMilitia();
		AllTeamsLookForAll( TRUE );

		for ( int i = 0; i<TOTAL_SOLDIERS; i++ )
		{
			//CHRISL: We should only bother with this assertion if the soldier is alive.  Dead soliders are moved to
			//	GridNo = NOWHERE, which causes this assertion to fail
			//CHRISL: There's also an issue with vehicles.  Soldiers in any vehicle are considered to be in sGridNo = NOWHERE
			//	This will result in an assertion error, so let's skip the assertion if the merc is assigned to a vehicle
			if (!(MercPtrs[i]->flags.uiStatusFlags & SOLDIER_DEAD) && MercPtrs[i]->bAssignment != VEHICLE && MercPtrs[i]->bAssignment != ASSIGNMENT_POW)
			{
				//Assert( !MercPtrs[i]->bActive || !MercPtrs[i]->bInSector || MercPtrs[i]->sGridNo != NOWHERE || MercPtrs[i]->bVehicleID == iHelicopterVehicleId );
				Assert( !MercPtrs[i]->bActive || !MercPtrs[i]->bInSector || !TileIsOutOfBounds( MercPtrs[i]->sGridNo ) || MercPtrs[i]->bVehicleID == iHelicopterVehicleId );
			}
		}

		return(TRUE);
	}

	if ( gWorldSectorX && gWorldSectorY && gbWorldSectorZ != -1 )
	{
		HandleDefiniteUnloadingOfWorld( ABOUT_TO_LOAD_NEW_MAP );
	}

	// make this the currently loaded sector
	gWorldSectorX = sMapX;
	gWorldSectorY = sMapY;
	gbWorldSectorZ = bMapZ;

	// update currently selected map sector to match
	ChangeSelectedMapSector( sMapX, sMapY, bMapZ );


	//Check to see if the sector we are loading is the cave sector under Tixa.  If so
	//then we will set up the meanwhile scene to start the creature quest.
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		StopAnyCurrentlyTalkingSpeech( );

#ifdef JA2UB
		/*  Ja25 no creatures  */
#else			
		if ( gWorldSectorX == gCreaturesSettings.ubCrepitusFeedingSectorX && gWorldSectorY == gCreaturesSettings.ubCrepitusFeedingSectorY
			 && gbWorldSectorZ == gCreaturesSettings.ubCrepitusFeedingSectorZ ) // (9, 10, 2)
		{
			InitCreatureQuest( ); //Ignored if already active.
		}
#endif
	}

	//Stop playing any music -- will fade out.
	// SetMusicMode( MUSIC_NONE );

	// ATE: Determine if we should set the default music...

	// Are we already in 'tense' music...

	// ATE: Change music only if not loading....
	/*-
	if ( gubMusicMode == MUSIC_TACTICAL_ENEMYPRESENT  )
	{
	fChangeMusic = FALSE;
	}

	// Did we 'tactically traverse' over....
	if ( gfTacticalTraversal )
	{
	fChangeMusic = FALSE;
	}

	// If we have no music playing at all....
	if ( gubMusicMode == MUSIC_NONE  )
	{
	fChangeMusic = TRUE;
	}
	-*/

	if ( (gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		fChangeMusic = TRUE;
	}
	else
	{
		fChangeMusic = FALSE;
	}


	if ( fChangeMusic )
	{
		SetMusicMode( MUSIC_MAIN_MENU );
	}

	// ATE: Do this stuff earlier!
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		// Update the last time we were in tactical...
		gTacticalStatus.uiTimeSinceLastInTactical = GetWorldTotalMin( );

		// init some AI stuff
		InitializeTacticalStatusAtBattleStart( );

		// CJC: delay this until after entering the sector!
		//InitAI();

		// Check for helicopter being on the ground in this sector...
		HandleHelicopterOnGroundSkyriderProfile( );
#ifdef JA2UB			
		//Check to see if we should add Manuel to this sector, if so add him
		ShouldNpcBeAddedToSector( gWorldSectorX, gWorldSectorY, bMapZ );
#endif		
	}

	//Load and enter the new sector
	if ( EnterSector( gWorldSectorX, gWorldSectorY, bMapZ ) )
	{
		// CJC: moved this here Feb 17
		if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
		{
			InitAI( );
		}

		//If there are any people with schedules, now is the time to process them.
		//CJC: doesn't work here if we're going through the tactical placement GUI; moving
		// this call to PrepareLoadedSector()
		//PostSchedules();

		// ATE: OK, add code here to update the states of doors if they should
		// be closed......
		if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
		{
			ExamineDoorsOnEnteringSector( );
		}

		// Update all the doors in the sector according to the temp file previously
		// loaded, and any changes made by the schedules
		UpdateDoorGraphicsFromStatus( TRUE, FALSE );

		//Set the fact we have visited the  sector
		SetSectorFlag( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, SF_ALREADY_LOADED );

		// Check for helicopter being on the ground in this sector...
		HandleHelicopterOnGroundGraphic( );

		HandleEnemyHelicopterOnGroundGraphic( );
	}
	else
		return(FALSE);

	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		if ( (GetMusicMode( ) != MUSIC_TACTICAL_ENEMYPRESENT && GetMusicMode( ) != MUSIC_TACTICAL_BATTLE) ||
			 (!NumHostilesInSector( sMapX, sMapY, bMapZ ) && GetMusicMode( ) == MUSIC_TACTICAL_ENEMYPRESENT) )
		{
			// ATE; Fade FA.T....
			SetMusicFadeSpeed( 5 );

#ifdef NEWMUSIC
			GlobalSoundID = MusicSoundValues[SECTOR( gWorldSectorX, gWorldSectorY )].SoundTacticalNothing[gbWorldSectorZ];
			if ( MusicSoundValues[SECTOR( gWorldSectorX, gWorldSectorY )].SoundTacticalNothing[gbWorldSectorZ] != -1 )
				SetMusicModeID( MUSIC_TACTICAL_NOTHING, MusicSoundValues[SECTOR( gWorldSectorX, gWorldSectorY )].SoundTacticalNothing[gbWorldSectorZ] );
			else
#endif
				SetMusicMode( MUSIC_TACTICAL_NOTHING );
		}

#ifdef JA2UB			
		// Say a quote from a merc who has new lines for the sector ( if any )
		HandlePlayerTeamQuotesWhenEnteringSector( sMapX, sMapY, bMapZ );
#else
		// ATE: Check what sector we are in, to show description if we have an RPC.....
		HandleRPCDescriptionOfSector( sMapX, sMapY, bMapZ );
#endif


		// ATE: Set Flag for being visited...
		SetSectorFlag( sMapX, sMapY, bMapZ, SF_HAS_ENTERED_TACTICAL );
#ifdef JA2UB			
		//ja2ub
		// If any emails should be sent from this sector
		HandleEmailBeingSentWhenEnteringSector( sMapX, sMapY, bMapZ, FALSE );
#endif

		// ATE; Reset some flags for creature sayings....
		gTacticalStatus.fSaidCreatureFlavourQuote = FALSE;
		gTacticalStatus.fHaveSeenCreature = FALSE;
		gTacticalStatus.fBeenInCombatOnce = FALSE;
		gTacticalStatus.fSaidCreatureSmellQuote = FALSE;
		ResetMultiSelection( );

		// ATE: Decide if we can have crows here....
		gTacticalStatus.fGoodToAllowCrows = FALSE;
		gTacticalStatus.fHasEnteredCombatModeSinceEntering = FALSE;
		gTacticalStatus.fDontAddNewCrows = FALSE;
#ifdef JA2UB			
		//Call this function, if Jerry doesnt need to be added, it will return
		UpdateJerryMiloInInitialSector( );
#endif
		// Adjust delay for tense quote
		gTacticalStatus.sCreatureTenseQuoteDelay = (INT16)(10 + Random( 20 ));

		{
			INT16 sWarpWorldX;
			INT16 sWarpWorldY;
			INT8  bWarpWorldZ;
			INT32 sWarpGridNo;

			if ( GetWarpOutOfMineCodes( &sWarpWorldX, &sWarpWorldY, &bWarpWorldZ, &sWarpGridNo ) && gbWorldSectorZ >= 2 )
			{
				gTacticalStatus.uiFlags |= IN_CREATURE_LAIR;
			}
			else
			{
				gTacticalStatus.uiFlags &= (~IN_CREATURE_LAIR);
			}
		}

		// Every third turn
		//if ( Random( 3 ) == 0  )
		{
			if ( gGameSettings.fOptions[TOPTION_ALLOW_CROWS] == TRUE )
			{
				gTacticalStatus.fGoodToAllowCrows = TRUE;
				gTacticalStatus.ubNumCrowsPossible = (UINT8)(5 + Random( 5 ));
			}
		}

	}

	return(TRUE);
}

BOOLEAN MapExists( UINT8 * szFilename )
{
	CHAR8 str[50];
	HWFILE fp;
	sprintf( str, "MAPS\\%s", szFilename );
	fp = FileOpen( str, FILE_ACCESS_READ, FALSE );
	if ( !fp )
		return FALSE;
	FileClose( fp );
	return TRUE;
}

void RemoveMercsInSector( )
{
	INT32					cnt;
	SOLDIERTYPE		*pSoldier;

	// IF IT'S THE SELECTED GUY, MAKE ANOTHER SELECTED!
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	// ATE: only for OUR guys.. the rest is taken care of in TrashWorld() when a new sector is added...
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; cnt++, pSoldier++ )
	{
		if ( pSoldier->bActive )
		{
			pSoldier->RemoveSoldierFromGridNo( );
		}
	}

}

void PrepareLoadedSector( )
{
	BOOLEAN fEnemyPresenceInThisSector = FALSE;
	BOOLEAN fAddCivs = TRUE;
	INT8 bMineIndex = -1;
#ifdef JA2UB
	//Ja25 No meanwhiles
#else
	if ( AreInMeanwhile( ) == FALSE )
#endif
	{
		if ( gbWorldSectorZ == 0 )
		{
			//			MakePlayerPerceptionOfSectorControlCorrect( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}
		else
		{
			// we always think we control underground sectors once we've visited them
			SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].fPlayer[gbWorldSectorZ] = TRUE;
		}
	}

	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		UpdateMercsInSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
	}

	// Reset ambients!
	HandleNewSectorAmbience( gTilesets[giCurrentTilesetID].ubAmbientID );

	// Flugente: fortification
	LoadSectorFortificationPlan( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
	HandleFortificationUpdate( );

	//if we are loading a 'pristine' map ( ie, not loading a saved game )
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{

#ifdef JA2UB
		//Ja25 No meanwhiles
#else
		if ( !AreReloadingFromMeanwhile( ) )
#endif
		{
			SetPendingNewScreen( GAME_SCREEN );

			// Make interface the team panel always...
			SetCurrentInterfacePanel( (UINT8)TEAM_PANEL );
		}


		//Check to see if civilians should be added.  Always add civs to maps unless they are
		//in a mine that is shutdown.
		if ( gbWorldSectorZ )
		{
			bMineIndex = GetIdOfMineForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			if ( bMineIndex != -1 )
			{
				if ( !AreThereMinersInsideThisMine( (UINT8)bMineIndex ) )
				{
					fAddCivs = FALSE;
				}
			}
		}

		if ( is_networked )
		{
			if ( is_server && fAddCivs && gCivEnabled == 1 )//hayden its around here we apply .ini choices for Ai
			{
				AddSoldierInitListTeamToWorld( CIV_TEAM, 255 );
			}
		}
		else if ( fAddCivs )
		{
			AddSoldierInitListTeamToWorld( CIV_TEAM, 255 );
		}

		if ( is_networked )
		{
			if ( is_server && gMilitiaEnabled == 1 )
				AddSoldierInitListTeamToWorld( MILITIA_TEAM, 255 );
		}
		else
		{
			AddSoldierInitListTeamToWorld( MILITIA_TEAM, 255 );
		}

		if ( is_networked )
		{
			if ( is_server && gCreatureEnabled == 1 )
				AddSoldierInitListBloodcats( );
		}
		else
		{
			AddSoldierInitListBloodcats( );
		}


		//Creatures are only added if there are actually some of them.  It has to go through some
		//additional checking.

#ifdef JA2TESTVERSION
		//Override the sector with the populations specified in the AIViewer
		if ( gfOverrideSector )
		{
			if ( gbWorldSectorZ > 0 )
			{
				UNDERGROUND_SECTORINFO *pSector;
				pSector = FindUnderGroundSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
				pSector->ubNumAdmins = (UINT8)(gsAINumAdmins > 0 ? gsAINumAdmins : 0);
				pSector->ubNumTroops = (UINT8)(gsAINumTroops > 0 ? gsAINumTroops : 0);
				pSector->ubNumElites = (UINT8)(gsAINumElites > 0 ? gsAINumElites : 0);
				pSector->ubNumTanks = (UINT8)(gsAINumTanks > 0 ? gsAINumTanks : 0);
				pSector->ubNumCreatures = (UINT8)(gsAINumCreatures > 0 ? gsAINumCreatures : 0);
			}
			else if ( !gbWorldSectorZ )
			{
				SECTORINFO *pSector;
				pSector = &SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )];
				pSector->ubNumAdmins = (UINT8)(gsAINumAdmins > 0 ? gsAINumAdmins : 0);
				pSector->ubNumTroops = (UINT8)(gsAINumTroops > 0 ? gsAINumTroops : 0);
				pSector->ubNumElites = (UINT8)(gsAINumElites > 0 ? gsAINumElites : 0);
				pSector->ubNumTanks = (UINT8)(gsAINumTanks > 0 ? gsAINumTanks : 0);
				pSector->ubNumCreatures = (UINT8)(gsAINumCreatures > 0 ? gsAINumCreatures : 0);
			}
		}
#endif

		// Haydent
		if ( is_networked )
		{
			if ( is_server && gCreatureEnabled == 1 )
				PrepareCreaturesForBattle( );

			// Haydent
			if ( is_server && gMilitiaEnabled == 1 )
				PrepareMilitiaForTactical( FALSE );
		}
		else
		{
			PrepareCreaturesForBattle( );
			PrepareMilitiaForTactical( TRUE );
		}

		// OK, set varibles for entring this new sector...
		gTacticalStatus.fVirginSector = TRUE;

		//Inserts the enemies into the newly loaded map based on the strategic information.
		//Note, the flag will return TRUE only if enemies were added.  The game may wish to
		//do something else in a case where no enemies are present.
		if ( !gfRestoringEnemySoldiersFromTempFile )
		{
			//AddSoldierInitListTeamToWorld( CIV_TEAM, 255 );
			//			fEnemyPresenceInThisSector = PrepareEnemyForSectorBattle();
		}

		if ( is_networked )
		{
			if ( is_server && gCivEnabled == 1 )
				AddProfilesNotUsingProfileInsertionData( ); //hayden: is just for civ's
		}
		else
		{
			AddProfilesNotUsingProfileInsertionData( );
		}
#ifdef JA2UB
		//Ja25 No meanwhiles
#else
		if ( !AreInMeanwhile( ) || GetMeanwhileID( ) == INTERROGATION )
#endif
		{
			if ( is_networked )
			{
				if ( is_server && gEnemyEnabled == 1 )
					fEnemyPresenceInThisSector = PrepareEnemyForSectorBattle( );
			}
			else
			{
				fEnemyPresenceInThisSector = PrepareEnemyForSectorBattle( );
			}
		}


		//Regardless whether or not this was set, clear it now.
		gfRestoringEnemySoldiersFromTempFile = FALSE;

		//KM:  FEB 8, 99 -- This call is no longer required!  Done already when group arrives in sector.
		//if( ( gbWorldSectorZ == 0 ) && ( fEnemyPresenceInThisSector == FALSE ) )
		//{
		//	SetThisSectorAsPlayerControlled( gWorldSectorX, gWorldSectorY, 0 );
		//}

		if ( gbWorldSectorZ > 0 )
		{
			// we always think we control underground sectors once we've visited them
			SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].fPlayer[gbWorldSectorZ] = TRUE;
		}

		//@@@Evaluate
		//Add profiles to world using strategic info, not editor placements.
		AddProfilesUsingProfileInsertionData( );

		PostSchedules( );
	}

	if ( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE || GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE || GetEnemyEncounterCode() == ENEMY_AMBUSH_DEPLOYMENT_CODE )
	{
		if ( gMapInformation.sCenterGridNo != NOWHERE )
		{
			CallAvailableEnemiesTo( gMapInformation.sCenterGridNo );
		}
		else
		{
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_RED, MSG_ERROR, L"Ambush aborted in sector %c%d -- no center point in map.  LC:1", gWorldSectorY + 'A' - 1, gWorldSectorX );
#endif
		}
	}

	EndLoadScreen( );

	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		// unpause game
		UnPauseGame( );
	}

	gpBattleGroup = NULL;

	if ( gfTacticalTraversal )
	{
		CalculateNonPersistantPBIInfo( );
	}

	ScreenMsg( FONT_YELLOW, MSG_DEBUG, L"Current Time is: %d", GetWorldTotalMin( ) );

#ifdef JA2UB	
	HandleSectorSpecificModificatioToMap( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, FALSE );

	//If there needs to be modifications done to the enemy exp levels...
	HandleJa25EnemyExpLevelModifier( );
#endif

	AllTeamsLookForAll( TRUE );
}

#define RANDOM_HEAD_MINERS 4
void HandleQuestCodeOnSectorEntry( INT16 sNewSectorX, INT16 sNewSectorY, INT8 bNewSectorZ )
{
	//UINT8		ubRandomMiner[RANDOM_HEAD_MINERS] = { 106, 156, 157, 158 };
	//UINT8		ubMiner, ubMinersPlaced, ubMine;
	UINT8		ubThisMine;
	UINT8		cnt;
	SOLDIERTYPE * pSoldier;

	// are we in a mine sector, on the surface?
	if ( IsThereAMineInThisSector( sNewSectorX, sNewSectorY ) && (bNewSectorZ == 0) )
	{
		if ( CheckFact( FACT_MINERS_PLACED, 0 ) == FALSE )
		{
			// SET HEAD MINER LOCATIONS

			ubThisMine = GetMineIndexForSector( sNewSectorX, sNewSectorY );
			// hardcoded logic has been moved to Lua script
			if ( g_luaMines.InitializeHeadMiners( ubThisMine ) )
				SetFactTrue( FACT_MINERS_PLACED );
#if 0
			//if (ubThisMine != MINE_SAN_MONA) // San Mona is abandoned
			if ( !gMineStatus[ubThisMine].fEmpty )
			{
				ubMinersPlaced = 0;

				if ( ubThisMine != MINE_ALMA )
				{
					// Fred Morris is always in the first mine sector we enter, unless that's Alma (then he's randomized, too)
					gMercProfiles[106].sSectorX = sNewSectorX;
					gMercProfiles[106].sSectorY = sNewSectorY;
					gMercProfiles[106].bSectorZ = 0;
					//gMercProfiles[ 106 ].bTown = gMineLocation[ ubThisMine ].bAssociatedTown;
					gMercProfiles[106].bTown = gMineStatus[ubThisMine].bAssociatedTown;

					// mark miner as placed
					ubRandomMiner[0] = 0;
					ubMinersPlaced++;
				}

				// assign the remaining (3) miners randomly
				for ( ubMine = 0; ubMine < MAX_NUMBER_OF_MINES; ubMine++ )
				{
					if ( ubMine == ubThisMine || ubMine == MINE_ALMA || ubMine == MINE_SAN_MONA )
					{
						// Alma always has Matt as a miner, and we have assigned Fred to the current mine
						// and San Mona is abandoned
						continue;
					}

					do
					{
						ubMiner = (UINT8)Random( RANDOM_HEAD_MINERS );
					}
					while( ubRandomMiner[ ubMiner ] == 0 );

					GetMineSector( ubMine, (INT16 *)&(gMercProfiles[ubRandomMiner[ubMiner]].sSectorX),
								   (INT16 *)&(gMercProfiles[ubRandomMiner[ubMiner]].sSectorY) );
					gMercProfiles[ubRandomMiner[ubMiner]].bSectorZ = 0;
					//gMercProfiles[ ubRandomMiner[ ubMiner ] ].bTown = gMineLocation[ ubMine ].bAssociatedTown;
					gMercProfiles[ubRandomMiner[ubMiner]].bTown = gMineStatus[ubMine].bAssociatedTown;

					// mark miner as placed
					ubRandomMiner[ubMiner] = 0;
					ubMinersPlaced++;

					if ( ubMinersPlaced == RANDOM_HEAD_MINERS )
					{
						break;
					}
				}

				SetFactTrue( FACT_MINERS_PLACED );
			}
#endif
		}
	}

	if ( CheckFact( FACT_ALL_TERRORISTS_KILLED, 0 ) )
	{
		// end terrorist quest
		// SANDRO - removed ending quest from here, placed to Interface Dialogue
		//EndQuest( QUEST_KILL_TERRORISTS, gMercProfiles[ CARMEN ].sSectorX, gMercProfiles[ CARMEN ].sSectorY );
		// remove Carmen
		gMercProfiles[CARMEN].sSectorX = 0;
		gMercProfiles[CARMEN].sSectorY = 0;
		gMercProfiles[CARMEN].bSectorZ = 0;
	}

	// Check to see if any player merc has the Chalice; if so,
	// note it as stolen
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++cnt, pSoldier++ )
	{
		if ( pSoldier->bActive )
		{
			if ( FindObj( pSoldier, CHALICE ) != ITEM_NOT_FOUND )
			{
				SetFactTrue( FACT_CHALICE_STOLEN );
			}
		}
	}

	if ( (gubQuest[QUEST_KINGPIN_MONEY] == QUESTINPROGRESS) && CheckFact( FACT_KINGPIN_CAN_SEND_ASSASSINS, 0 ) && (GetTownIdForSector( sNewSectorX, sNewSectorY ) != BLANK_SECTOR) && !bNewSectorZ && Random( 10 + NumNonPlayerTeamMembersInSector( sNewSectorX, sNewSectorY, MILITIA_TEAM ) ) < 3 )
	{
		DecideOnAssassin( );
	}

	if ( CheckFact( FACT_ROBOT_RECRUITED_AND_MOVED, 0 ) == FALSE )
	{
		SOLDIERTYPE * pRobot;
		pRobot = FindSoldierByProfileID( ROBOT, TRUE );
		if ( pRobot )
		{
			// robot is on our team and we have changed sectors, so we can
			// replace the robot-under-construction in Madlab's sector
			RemoveGraphicFromTempFile( gsRobotGridNo, SEVENTHISTRUCT1, gMercProfiles[MADLAB].sSectorX, gMercProfiles[MADLAB].sSectorY, gMercProfiles[MADLAB].bSectorZ );
			SetFactTrue( FACT_ROBOT_RECRUITED_AND_MOVED );
		}
	}

#ifdef LUA_HANDLE_QUEST_CODE_ON_SECTOR

	LuaHandleQuestCodeOnSector( sNewSectorX, sNewSectorY, bNewSectorZ, 1 );

#else

	/*
	if ( sNewSectorX == 5 && sNewSectorY == MAP_ROW_C )
	{
	// reset Madame Layla counters
	gMercProfiles[ MADAME ].bNPCData = 0;
	gMercProfiles[ MADAME ].bNPCData2 = 0;
	}
	*/

	if ( sNewSectorX == 6 && sNewSectorY == MAP_ROW_C && gubQuest[QUEST_RESCUE_MARIA] == QUESTDONE )
	{
		// make sure Maria and Angel are gone
		gMercProfiles[MARIA].sSectorX = 0;
		gMercProfiles[ANGEL].sSectorY = 0;
		gMercProfiles[MARIA].sSectorX = 0;
		gMercProfiles[ANGEL].sSectorY = 0;
	}

	if ( sNewSectorX == 5 && sNewSectorY == MAP_ROW_D )
	{
		gubBoxerID[0] = NOBODY;
		gubBoxerID[1] = NOBODY;
		gubBoxerID[2] = NOBODY;
	}

	if ( sNewSectorX == 3 && sNewSectorY == MAP_ROW_P )
	{
		// heal up Elliot if he's been hurt
		if ( gMercProfiles[ELLIOT].bLife < gMercProfiles[ELLIOT].bLifeMax )
		{
			gMercProfiles[ELLIOT].bLife = gMercProfiles[ELLIOT].bLifeMax;
		}
	}

#endif

	ResetOncePerConvoRecordsForAllNPCsInLoadedSector( );
}

void HandleQuestCodeOnSectorExit( INT16 sOldSectorX, INT16 sOldSectorY, INT8 bOldSectorZ )
{

#ifdef JA2UB
	SOLDIERTYPE *pSoldier = NULL;
#endif

#ifdef LUA_HANDLE_QUEST_CODE_ON_SECTOR

	LuaHandleQuestCodeOnSector( sOldSectorX, sOldSectorY, bOldSectorZ, 0 );

#else

	if ( sOldSectorX == KINGPIN_MONEY_SECTOR_X && sOldSectorY == KINGPIN_MONEY_SECTOR_Y && bOldSectorZ == KINGPIN_MONEY_SECTOR_Z )
	{
		CheckForKingpinsMoneyMissing( TRUE );
	}

	if ( sOldSectorX == 13 && sOldSectorY == MAP_ROW_H && bOldSectorZ == 0 && CheckFact( FACT_CONRAD_SHOULD_GO, 0 ) )
	{
		// remove Conrad from the map
		gMercProfiles[CONRAD].sSectorX = 0;
		gMercProfiles[CONRAD].sSectorY = 0;
	}
#ifdef JA2UB	
	//JA25 UB
	if ( sOldSectorX == 7 && sOldSectorY == MAP_ROW_H && bOldSectorZ == 0 )
	{
		// remove Jerry from the map
		gMercProfiles[JERRY_MILO_UB].sSectorX = 0;
		gMercProfiles[JERRY_MILO_UB].sSectorY = 0;
	}

	//if the player is leaving a sector with  Tex in it
	if ( sOldSectorX == gMercProfiles[TEX_UB].sSectorX && sOldSectorY == gMercProfiles[TEX_UB].sSectorY && bOldSectorZ == 0 && gMercProfiles[TEX_UB].ubLastDateSpokenTo != 0 )
	{
		pSoldier = FindSoldierByProfileID( TEX_UB, TRUE );

		//if the npc isnt on the players team AND the player has never spoken to them
		if ( pSoldier == NULL && gMercProfiles[TEX_UB].ubLastDateSpokenTo != 0 )
		{
			// remove Tex from the map
			gMercProfiles[TEX_UB].sSectorX = 0;
			gMercProfiles[TEX_UB].sSectorY = 0;
		}
	}

	//if the player is leaving a sector with  John kulba in it
	if ( sOldSectorX == gMercProfiles[JOHN_K_UB].sSectorX && sOldSectorY == gMercProfiles[JOHN_K_UB].sSectorY && bOldSectorZ == 0 && gMercProfiles[JOHN_K_UB].ubLastDateSpokenTo != 0 )
	{
		pSoldier = FindSoldierByProfileID( JOHN_K_UB, TRUE );

		//if the npc isnt on the players team AND the player has never spoken to them
		if ( pSoldier == NULL && gMercProfiles[JOHN_K_UB].ubLastDateSpokenTo != 0 )
		{
			// remove Tex from the map
			gMercProfiles[JOHN_K_UB].sSectorX = 0;
			gMercProfiles[JOHN_K_UB].sSectorY = 0;
		}
	}

	//if the player is leaving a sector with  Manuel in it
	if ( sOldSectorX == gMercProfiles[MANUEL_UB].sSectorX && sOldSectorY == gMercProfiles[MANUEL_UB].sSectorY && bOldSectorZ == 0 )
	{
		pSoldier = FindSoldierByProfileID( MANUEL_UB, TRUE );

		//if the npc isnt on the players team AND the player has never spoken to them
		if ( pSoldier == NULL && gMercProfiles[MANUEL_UB].ubLastDateSpokenTo != 0 )
		{
			// remove Manuel from the map
			gMercProfiles[MANUEL_UB].sSectorX = 0;
			gMercProfiles[MANUEL_UB].sSectorY = 0;
		}
	}
#endif
	if ( sOldSectorX == gModSettings.ubHospitalSectorX && sOldSectorY == gModSettings.ubHospitalSectorY && bOldSectorZ == gModSettings.ubHospitalSectorZ )
	{
		CheckForMissingHospitalSupplies( );
	}

	// reset the state of the museum alarm for Eldin's quotes
	SetFactFalse( FACT_MUSEUM_ALARM_WENT_OFF );

#endif
}

extern void SetLastTimePlayerWasInSector( );
//Moa: 09/14/2013 this function modifies uiTimeCurrentSectorWasLastLoaded and the various decay of items in that sector as well
// @calls HandleSectorCooldownFunctions
// @calls SetLastTimePlayerWasInSector
BOOLEAN EnterSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT32 i;
	UNDERGROUND_SECTORINFO *pNode = NULL;
	CHAR8 bFilename[50];

	// Reset
	//ResetLoadScreenHint();

	// pause game
	PauseGame( );

	// stop time for this frame
	InterruptTime( );

	//Setup the tactical existance of RPCs and CIVs in the last sector before moving on to a new sector.
	//@@@Evaluate
	if ( gfWorldLoaded )
	{
		for ( i = gTacticalStatus.Team[CIV_TEAM].bFirstID; i <= gTacticalStatus.Team[CIV_TEAM].bLastID; i++ )
		{
			if ( MercPtrs[i]->bActive && MercPtrs[i]->bInSector )
			{
				SetupProfileInsertionDataForSoldier( MercPtrs[i] );
			}
		}
	}

	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		// Handle NPC stuff related to changing sectors
		HandleQuestCodeOnSectorEntry( sSectorX, sSectorY, bSectorZ );
	}

	// Begin Load
	BeginLoadScreen( );

	// This has tobe done before loadworld, as it will remmove old gridnos if present
	RemoveMercsInSector( );

	// SANDRO - reset number of enemies here
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		memset( &(gTacticalStatus.bNumFoughtInBattle), 0, MAXTEAMS );
	}

#ifdef JA2UB
	//Ja25 No meanwhiles
#else
	if ( AreInMeanwhile( ) == FALSE )
#endif
	{
		SetSectorFlag( sSectorX, sSectorY, bSectorZ, SF_ALREADY_VISITED );
	}

	GetMapFileName( sSectorX, sSectorY, bSectorZ, bFilename, TRUE, TRUE );

	//Load the placeholder map if the real map doesn't exist.
	if ( !MapExists( (UINT8 *)bFilename ) )
	{
		if ( !MapExists( (UINT8 *)"Placeholder1.dat" ) )
			AssertMsg( 0, "Failed to find Placeholder1.dat (placeholder map)." );
	}

	CreateLoadingScreenProgressBar( TRUE );
#ifdef JA2BETAVERSION
	//set the font
	SetProgressBarMsgAttributes( 0, FONT12ARIAL, FONT_MCOLOR_WHITE, 0 );

	//Set the tile so we don see the text come up
	SetProgressBarTextDisplayFlag( 0, TRUE, TRUE, TRUE );
#endif

	//CreateProgressBar( 0, 160, 380, 480, 400 );
	//#ifdef JA2TESTVERSION
	//	//add more detailed progress bar
	//	DefineProgressBarPanel( 0, 65, 79, 94, 130, 350, 510, 430 );
	//	swprintf( str, L"Loading map:  %S", bFilename );
	//	SetProgressBarTitle( 0, str, FONT12POINT1, FONT_BLACK, FONT_BLACK );
	//#endif
	if ( !LoadWorld( bFilename ) )
	{
		return(FALSE);
	}

	// underground?
	if ( bSectorZ )
	{
		pNode = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );

		// is there a sector?..if so set flag
		if ( pNode )
		{
			pNode->fVisited = TRUE;
		}
	}

	// if we arent loading a saved game
	// ATE: Moved this form above, so that we can have the benefit of
	// changing the world BEFORE adding guys to it...
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		// Load the current sectors Information From the temporary files
		if ( !LoadCurrentSectorsInformationFromTempItemsFile( ) )
		{ //The integrity of the temp files have been compromised.  Boot out of the game after warning message.
			InitExitGameDialogBecauseFileHackDetected( );
			return TRUE;
		}
	}

	RemoveLoadingScreenProgressBar( );
	//RemoveProgressBar( 0 );

	if ( gfEnterTacticalPlacementGUI )
	{
		SetPendingNewScreen( GAME_SCREEN );
		InitTacticalPlacementGUI( );
	}
	else
	{
		PrepareLoadedSector( );
	}

	//	UnPauseGame( );

	// This function will either hide or display the tree tops, depending on the game setting
	SetTreeTopStateForMap( );

	// Flugente: certain features need to alter an item's temperature value depending on the time passed
	// if we do these functions here and adjust for the time passed since this sector was loaded last, it will seem to the player
	// as if these checks are always performed in any sector
	//Moa: removed this function and replaced by the handling function.
	//SectorInventoryCooldownFunctions(sSectorX, sSectorY, bSectorZ);
	//moved from SectorInventoryCooldownFunctions. Invisible items are handled as well.
	//Since we have already loaded the items previously we can use the globals here.
	HandleSectorCooldownFunctions( sSectorX, sSectorY, (INT8)bSectorZ, gWorldItems, guiNumWorldItems, TRUE );
	//Update LastTimePlayerWasInSector
	SetLastTimePlayerWasInSector( );

	UpdateWorldItems(sSectorX, sSectorY, (INT8)bSectorZ, guiNumWorldItems, gWorldItems);

	DebugQuestInfo(String("Enter Sector %s%s Level %d", pVertStrings[sSectorY], pHortStrings[sSectorX], bSectorZ));

	return TRUE; //because the map was loaded.
}


void UpdateMercsInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT32					cnt;
	SOLDIERTYPE		*pSoldier;
	BOOLEAN				fPOWSquadSet = FALSE;
	UINT8					ubPOWSquad = 0;
	CHAR16		zTemp[256];

	// Remove from interface slot
	RemoveAllPlayersFromSlot( );

	// Remove tactical interface stuff
	guiPendingOverrideEvent = I_CHANGE_TO_IDLE;

	//If we are in this function during the loading of a sector
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		//DONT set these values
		gusSelectedSoldier = NOBODY;
		gfGameScreenLocateToSoldier = TRUE;
	}

	// Set all faces intactive
	SetAllAutoFacesInactive( );

	if ( fUsingEdgePointsForStrategicEntry )
	{
		BeginMapEdgepointSearch( );
	}

	//if( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
	{
		for ( cnt = 0, pSoldier = MercPtrs[cnt]; cnt < MAX_NUM_SOLDIERS; ++cnt, ++pSoldier )
		{
			if ( gfRestoringEnemySoldiersFromTempFile &&
				 cnt >= gTacticalStatus.Team[ENEMY_TEAM].bFirstID &&
				 cnt <= gTacticalStatus.Team[CREATURE_TEAM].bLastID )
			{ //Don't update enemies/creatures (consec. teams) if they were
				//just restored via the temp map files...
				continue;
			}
			// Remove old merc, if exists
			RemoveMercSlot( pSoldier );

			pSoldier->bInSector = FALSE;

			if ( pSoldier->bActive )
			{
				if ( pSoldier->sSectorX == sSectorX && pSoldier->sSectorY == sSectorY && pSoldier->bSectorZ == bSectorZ && !pSoldier->flags.fBetweenSectors )
				{
					if ( !( gTacticalStatus.uiFlags & LOADING_SAVED_GAME ) )
					{
						if ( gMapInformation.sCenterGridNo != NOWHERE && gfBlitBattleSectorLocator &&
							( GetEnemyEncounterCode() == ENEMY_AMBUSH_CODE || GetEnemyEncounterCode() == BLOODCAT_AMBUSH_CODE ) && pSoldier->bTeam != CIV_TEAM )
						{
							// Flugente: improved ambush
							if ( gGameExternalOptions.fAmbushSpreadMercs )
							{
								UINT8 ubDirection;

								pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
								pSoldier->usStrategicInsertionData = FindRandomGridNoFromSweetSpotExcludingSweetSpot( pSoldier, gMapInformation.sCenterGridNo, gGameExternalOptions.usAmbushSpreadRadiusMercs, &ubDirection );

								// have the merc look outward. We add + 100 because later on we use this to signify that we want really enforce this direction
								pSoldier->ubInsertionDirection = (UINT8)GetDirectionToGridNoFromGridNo( gMapInformation.sCenterGridNo, pSoldier->usStrategicInsertionData ) + 100;
							}
							else
							{
								pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
								pSoldier->usStrategicInsertionData = gMapInformation.sCenterGridNo;
							}
						}
						else if ( gfOverrideInsertionWithExitGrid )
						{
							pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
							pSoldier->usStrategicInsertionData = gExitGrid.usGridNo;
						}

						// Flugente: override if entering from concealment
						if ( pSoldier->usSoldierFlagMask2 & SOLDIER_CONCEALINSERTION )
						{
							pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
							pSoldier->usStrategicInsertionData = pSoldier->sMTActionGridNo;
						}
					}

					gbMercIsNewInThisSector[pSoldier->ubID] = 1;

					UpdateMercInSector( pSoldier, sSectorX, sSectorY, bSectorZ );

					if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
					{
						if ( pSoldier->bAssignment == ASSIGNMENT_POW )
						{
							if ( !fPOWSquadSet )
							{
								fPOWSquadSet = TRUE;

								// ATE: If we are in i13 - pop up message!
								if ((sSectorX == gModSettings.ubInitialPOWSectorX && sSectorY == gModSettings.ubInitialPOWSectorY) || //(13, 9)
								(sSectorX == gModSettings.ubTixaPrisonSectorX && sSectorY == gModSettings.ubTixaPrisonSectorY))
								{
									swprintf( zTemp, TacticalStr[POW_MERCS_ARE_HERE], gMercProfiles[QUEEN].zNickname );
									DoMessageBox( MSG_BOX_BASIC_STYLE, zTemp, GAME_SCREEN, (UINT8)MSG_BOX_FLAG_OK, NULL, NULL );
								}
								else
								{
									AddCharacterToUniqueSquad( pSoldier );
									ubPOWSquad = pSoldier->bAssignment;
									pSoldier->aiData.bNeutral = FALSE;
								}
							}
							else
							{
								if ((sSectorX != gModSettings.ubInitialPOWSectorX && sSectorY != gModSettings.ubInitialPOWSectorY) && //(13, 9)
									(sSectorX != gModSettings.ubTixaPrisonSectorX && sSectorY != gModSettings.ubTixaPrisonSectorY))
								{
									AddCharacterToSquad( pSoldier, ubPOWSquad );
								}
							}

							// ATE: Call actions based on what POW we are on...
							#ifndef JA2UB
							HandlePOWQuestState(Q_END, QUEST_HELD_IN_ALMA, sSectorX, sSectorY, bSectorZ);
							HandlePOWQuestState(Q_END, QUEST_HELD_IN_TIXA, sSectorX, sSectorY, bSectorZ);
							#endif
						}
					}
				}
				/*else
				{
					pSoldier->bInSector = FALSE;
				}*/
			}
		}
	}

	if ( fUsingEdgePointsForStrategicEntry )
	{
		EndMapEdgepointSearch( );

		// Set to false
		fUsingEdgePointsForStrategicEntry = FALSE;
	}
}

void UpdateMercInSector( SOLDIERTYPE *pSoldier, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	BOOLEAN fError = FALSE;
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_IS_TACTICALLY_VALID )
	{
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
	}
	// OK, determine entrence direction and get sweetspot
	// Only if we are an OK guy to control....
	// SOME CHECKS HERE MUST BE FLESHED OUT......
	if ( pSoldier->bActive )		// This was in the if, removed by DEF:  pSoldier->stats.bLife >= OKLIFE &&
	{
		// If we are not in transit...
		if ( pSoldier->bAssignment != IN_TRANSIT )
		{
			// CHECK UBINSERTION CODE..
			if ( pSoldier->ubStrategicInsertionCode == INSERTION_CODE_PRIMARY_EDGEINDEX ||
				 pSoldier->ubStrategicInsertionCode == INSERTION_CODE_SECONDARY_EDGEINDEX )
			{
				if ( !fUsingEdgePointsForStrategicEntry )
				{
					// If we are not supposed to use this now, pick something better...
					pSoldier->ubStrategicInsertionCode = (UINT8)pSoldier->usStrategicInsertionData;
				}
			}

		MAPEDGEPOINT_SEARCH_FAILED:

			if ( pSoldier->ubProfile != NO_PROFILE && gMercProfiles[pSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE )
			{
				// override orders
				pSoldier->aiData.bOrders = STATIONARY;
			}

			if (pSoldier->ubStrategicInsertionCode == INSERTION_CODE_SECONDARY_EDGEINDEX && (
				   (gubTacticalDirection == NORTH && gus2ndNorthEdgepointArraySize == 0)
				|| (gubTacticalDirection == EAST && gus2ndEastEdgepointArraySize == 0)
				|| (gubTacticalDirection == SOUTH && gus2ndSouthEdgepointArraySize == 0)
				|| (gubTacticalDirection == WEST && gus2ndWestEdgepointArraySize == 0)
				)) 
			{
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_PRIMARY_EDGEINDEX;
			}

			// Use insertion direction from loaded map!
			switch (pSoldier->ubStrategicInsertionCode)
			{
			case INSERTION_CODE_NORTH:
				pSoldier->sInsertionGridNo = gMapInformation.sNorthGridNo;
				if ( !gfEditMode && gMapInformation.sNorthGridNo == -1 )
					fError = TRUE;
				break;
			case INSERTION_CODE_SOUTH:
				pSoldier->sInsertionGridNo = gMapInformation.sSouthGridNo;
				if ( !gfEditMode && gMapInformation.sSouthGridNo == -1 )
					fError = TRUE;
				break;
			case INSERTION_CODE_EAST:
				pSoldier->sInsertionGridNo = gMapInformation.sEastGridNo;
				if ( !gfEditMode && gMapInformation.sEastGridNo == -1 )
					fError = TRUE;
				break;
			case INSERTION_CODE_WEST:
				pSoldier->sInsertionGridNo = gMapInformation.sWestGridNo;
				if ( !gfEditMode && gMapInformation.sWestGridNo == -1 )
					fError = TRUE;
				break;
			case INSERTION_CODE_CENTER:
				pSoldier->sInsertionGridNo = gMapInformation.sCenterGridNo;
				if ( !gfEditMode && gMapInformation.sCenterGridNo == -1 )
					fError = TRUE;
				break;
			case INSERTION_CODE_GRIDNO:
				pSoldier->sInsertionGridNo = pSoldier->usStrategicInsertionData;
				break;

			case INSERTION_CODE_PRIMARY_EDGEINDEX:
				//dnl ch48 041009 Risky translation, must never occur again on same element
				gMapTrn.ResizeTrnCnt( pSoldier->aiData.sPendingActionData2 );
				if ( fUsingEdgePointsForStrategicEntry )
				{
					INT32 sWorldX, sWorldY;
					INT16 sScreenX, sScreenY, sX, sY;
					// Determine 'mirror' gridno...
					ConvertGridNoToXY( pSoldier->aiData.sPendingActionData2, &sX, &sY );
					// Convert to absolute xy
					GetWorldXYAbsoluteScreenXY( sX, sY, &sScreenX, &sScreenY );
					// Get 'mirror', depending on what direction...
					switch ( gubTacticalDirection )
					{
					case NORTH:
						sScreenY = MAPHEIGHT;
						break;
					case SOUTH:
						sScreenY = 0;
						break;
					case EAST:
						sScreenX = 0;
						break;
					case WEST:
						sScreenX = MAPWIDTH;
						break;
					}
					// Convert into a gridno again.....
					GetFromAbsoluteScreenXYWorldXY( &sWorldX, &sWorldY, sScreenX, sScreenY );
					// Save this gridNo....
					pSoldier->aiData.sPendingActionData2 = GETWORLDINDEXFROMWORLDCOORDS( sWorldY, sWorldX );
				}
				pSoldier->sInsertionGridNo = SearchForClosestPrimaryMapEdgepoint( pSoldier->aiData.sPendingActionData2, (UINT8)pSoldier->usStrategicInsertionData );
#ifdef JA2BETAVERSION
				{
					CHAR8 str[256];
					sprintf( str, "%S's primary insertion gridno is %d using %d as initial search gridno and %d insertion code.",
							 pSoldier->name, pSoldier->sInsertionGridNo, pSoldier->aiData.sPendingActionData2, pSoldier->usStrategicInsertionData );
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, str );
				}
#endif					
				if ( TileIsOutOfBounds( pSoldier->sInsertionGridNo ) )
				{
					ScreenMsg( FONT_RED, MSG_ERROR, L"Main edgepoint search failed for %s -- substituting entrypoint.", pSoldier->name );
					pSoldier->ubStrategicInsertionCode = (UINT8)pSoldier->usStrategicInsertionData;
					goto MAPEDGEPOINT_SEARCH_FAILED;
				}
				break;
			case INSERTION_CODE_SECONDARY_EDGEINDEX:
				pSoldier->sInsertionGridNo = SearchForClosestSecondaryMapEdgepoint( pSoldier->aiData.sPendingActionData2, (UINT8)pSoldier->usStrategicInsertionData );
#ifdef JA2BETAVERSION
				{
					CHAR8 str[256];
					sprintf( str, "%S's isolated insertion gridno is %d using %d as initial search gridno and %d insertion code.",
							 pSoldier->name, pSoldier->sInsertionGridNo, pSoldier->aiData.sPendingActionData2, pSoldier->usStrategicInsertionData );
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, str );
				}
#endif					
				if ( TileIsOutOfBounds( pSoldier->sInsertionGridNo ) )
				{
					ScreenMsg( FONT_RED, MSG_ERROR, L"Isolated edgepont search failed for %s -- substituting entrypoint.", pSoldier->name );
					pSoldier->ubStrategicInsertionCode = (UINT8)pSoldier->usStrategicInsertionData;
					goto MAPEDGEPOINT_SEARCH_FAILED;
				}
				break;

			case INSERTION_CODE_ARRIVING_GAME:
#ifdef JA2UB
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->sInsertionGridNo = gGameUBOptions.LOCATEGRIDNO;
#else
				extern BOOLEAN gfFirstHeliRun;
				if (gfFirstHeliRun)
				{
					pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
					pSoldier->sInsertionGridNo = gGameExternalOptions.iInitialMercArrivalLocation;
				}
				else
				{
					pSoldier->ubStrategicInsertionCode = INSERTION_CODE_CENTER;
					pSoldier->sInsertionGridNo = gMapInformation.sCenterGridNo;
				}
#endif
				break;
			case INSERTION_CODE_CHOPPER:
				// Try another location and walk into map
				// Add merc to chopper....
				//pSoldier->sInsertionGridNo = 4058;
				AddMercToHeli( pSoldier->ubID );
				return;
				break;
			default:
				//Moa: Hardcoded grid number set to CenterGrid
				pSoldier->sInsertionGridNo = (WORLD_ROWS * WORLD_COLS + WORLD_COLS) / 2;//12880
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Improper insertion code %d given to UpdateMercsInSector", pSoldier->ubStrategicInsertionCode ) );
				break;
			}

			if ( fError )
			{ //strategic insertion failed because it expected to find an entry point.  This is likely
				//a missing part of the map or possible fault in strategic movement costs, traversal logic, etc.
				CHAR16 szEntry[10];
				CHAR16 szSector[10];
				INT32 sGridNo;
				GetLoadedSectorString( szSector );
				if ( gMapInformation.sNorthGridNo != -1 )
				{
					swprintf( szEntry, L"north" );
					sGridNo = gMapInformation.sNorthGridNo;
				}
				else if ( gMapInformation.sEastGridNo != -1 )
				{
					swprintf( szEntry, L"east" );
					sGridNo = gMapInformation.sEastGridNo;
				}
				else if ( gMapInformation.sSouthGridNo != -1 )
				{
					swprintf( szEntry, L"south" );
					sGridNo = gMapInformation.sSouthGridNo;
				}
				else if ( gMapInformation.sWestGridNo != -1 )
				{
					swprintf( szEntry, L"west" );
					sGridNo = gMapInformation.sWestGridNo;
				}
				else if ( gMapInformation.sCenterGridNo != -1 )
				{
					swprintf( szEntry, L"center" );
					sGridNo = gMapInformation.sCenterGridNo;
				}
				else
				{
					ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Sector %s has NO entrypoints -- using precise center of map for %s.", szSector, pSoldier->name );
					pSoldier->sInsertionGridNo = (WORLD_ROWS * WORLD_COLS + WORLD_COLS) / 2;//12880
					AddSoldierToSector( pSoldier->ubID );
					return;
				}
				pSoldier->sInsertionGridNo = sGridNo;
				switch ( pSoldier->ubStrategicInsertionCode )
				{
				case INSERTION_CODE_NORTH:
					ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Sector %s doesn't have a north entrypoint -- substituting  %s entrypoint for %s.", szSector, szEntry, pSoldier->name );
					break;
				case INSERTION_CODE_EAST:
					ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Sector %s doesn't have a east entrypoint -- substituting  %s entrypoint for %s.", szSector, szEntry, pSoldier->name );
					break;
				case INSERTION_CODE_SOUTH:
					ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Sector %s doesn't have a south entrypoint -- substituting  %s entrypoint for %s.", szSector, szEntry, pSoldier->name );
					break;
				case INSERTION_CODE_WEST:
					ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Sector %s doesn't have a west entrypoint -- substituting  %s entrypoint for %s.", szSector, szEntry, pSoldier->name );
					break;
				case INSERTION_CODE_CENTER:
					ScreenMsg( FONT_RED, MSG_BETAVERSION, L"Sector %s doesn't have a center entrypoint -- substituting  %s entrypoint for %s.", szSector, szEntry, pSoldier->name );
					break;
				}
			}
			// If no insertion direction exists, this is bad!
			if ( pSoldier->sInsertionGridNo == -1 )
			{
				DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Insertion gridno for direction %d not added to map sector %d %d", pSoldier->ubStrategicInsertionCode, sSectorX, sSectorY ) );
				pSoldier->sInsertionGridNo = (WORLD_ROWS * WORLD_COLS + WORLD_COLS) / 2;//12880
			}

			AddSoldierToSector( pSoldier->ubID );
		}
	}
}

// Get sector ID string makes a string like 'A9 - OMERTA', or just J11 if no town....
void GetSectorIDString( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, STR16 zString, BOOLEAN fDetailed )
{
	SECTORINFO *pSector = NULL;
	UNDERGROUND_SECTORINFO *pUnderground;
	INT8		bTownNameID;
	UINT8 ubSectorID = 0;
	UINT8 ubLandType = 0;

	if ( sSectorX <= 0 || sSectorY <= 0 || bSectorZ < 0 )
	{
		//swprintf( zString, L"%s", pErrorStrings[0] );
	}
	else if ( bSectorZ != 0 ) // UNDERGROUND SECTORS
	{
		pUnderground = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );

#if 0 // reading sector names from lua now
		////////////////////////////////////
		// Read and verify XML sector names

		INT8		bMineIndex;
		ubSectorID = (UINT8)SECTOR( sSectorX, sSectorY );
		AssertGE( ubSectorID, 0 );
		AssertLT( ubSectorID, 256 );
		pSector = &SectorInfo[ubSectorID];
		ubLandType = pSector->ubTraversability[4];
		swprintf( zString, L"%c%d: ", 'A' + sSectorY - 1, sSectorX );

		BOOLEAN fSectorHasXMLNames = TRUE;
		CHAR16 zUnexploredUnderground[MAX_SECTOR_NAME_LENGTH];
		CHAR16 zDetailedUnexploredUnderground[MAX_SECTOR_NAME_LENGTH];
		CHAR16 zExploredUnderground[MAX_SECTOR_NAME_LENGTH];
		CHAR16 zDetailedExploredUnderground[MAX_SECTOR_NAME_LENGTH];

		if ( bSectorZ == 1 )
		{
			wcscpy( zUnexploredUnderground, gzSectorUndergroundNames1[ubSectorID][0] );
			wcscpy( zDetailedUnexploredUnderground, gzSectorUndergroundNames1[ubSectorID][1] );
			wcscpy( zExploredUnderground, gzSectorUndergroundNames1[ubSectorID][2] );
			wcscpy( zDetailedExploredUnderground, gzSectorUndergroundNames1[ubSectorID][3] );
		}
		else if ( bSectorZ == 2 )
		{
			wcscpy( zUnexploredUnderground, gzSectorUndergroundNames2[ubSectorID][0] );
			wcscpy( zDetailedUnexploredUnderground, gzSectorUndergroundNames2[ubSectorID][1] );
			wcscpy( zExploredUnderground, gzSectorUndergroundNames2[ubSectorID][2] );
			wcscpy( zDetailedExploredUnderground, gzSectorUndergroundNames2[ubSectorID][3] );
		}
		else if ( bSectorZ == 3 )
		{
			wcscpy( zUnexploredUnderground, gzSectorUndergroundNames3[ubSectorID][0] );
			wcscpy( zDetailedUnexploredUnderground, gzSectorUndergroundNames3[ubSectorID][1] );
			wcscpy( zExploredUnderground, gzSectorUndergroundNames3[ubSectorID][2] );
			wcscpy( zDetailedExploredUnderground, gzSectorUndergroundNames3[ubSectorID][3] );
		}

		if ( zUnexploredUnderground[0] == 0 || zDetailedUnexploredUnderground[0] == 0 || zExploredUnderground[0] == 0 || zDetailedExploredUnderground[0] == 0 )
		{
			fSectorHasXMLNames = FALSE;
		}

		if ( fSectorHasXMLNames ) // UNDERGROUND XML
		{
			if ( pUnderground )
			{
				bMineIndex = GetIdOfMineForSector( sSectorX, sSectorY, bSectorZ );
				if ( bMineIndex != -1 )
				{
					swprintf( zString, L"%c%d: %s %s", 'A' + sSectorY - 1, sSectorX, pTownNames[GetTownAssociatedWithMine( bMineIndex )], pwMineStrings[0] );
				}
				else if ( pUnderground->fVisited )
				{
					if ( fDetailed )
					{
						wcscat( zString, zDetailedExploredUnderground );
					}
					else
					{
						wcscat( zString, zExploredUnderground );
					}
				}
				else
				{
					if ( fDetailed )
					{
						wcscat( zString, zDetailedUnexploredUnderground );
					}
					else
					{
						wcscat( zString, zUnexploredUnderground );
					}
				}

				if ( pUnderground->ubNumCreatures )
					swprintf( zString, L"%c%d: %s", 'A' + sSectorY - 1, sSectorX, pLandTypeStrings[CREATURE_LAIR] );
				//else
				//	swprintf( zString, L"%c%d-%d", 'A' + sSectorY - 1, sSectorX, bSectorZ );
			}
		}
		else // UNDERGROUND HARDCODED
		{
			pUnderground = FindUnderGroundSector( sSectorX, sSectorY, bSectorZ );
			if ( pUnderground && (pUnderground->fVisited || gfGettingNameFromSaveLoadScreen) )
			{
				bMineIndex = GetIdOfMineForSector( sSectorX, sSectorY, bSectorZ );
				if ( bMineIndex != -1 )
				{
					swprintf( zString, L"%c%d: %s %s", 'A' + sSectorY - 1, sSectorX, pTownNames[GetTownAssociatedWithMine( bMineIndex )], pwMineStrings[0] );
				}
				else switch ( SECTOR( sSectorX, sSectorY ) )
				{
				case SEC_A10:
					swprintf( zString, L"A10: %s", pLandTypeStrings[REBEL_HIDEOUT] );
					break;
				case SEC_J9:
					swprintf( zString, L"J9: %s", pLandTypeStrings[TIXA_DUNGEON] );
					break;
				case SEC_K4:
					swprintf( zString, L"K4: %s", pLandTypeStrings[ORTA_BASEMENT] );
					break;
				case SEC_O3:
					swprintf( zString, L"O3: %s", pLandTypeStrings[TUNNEL] );
					break;
				case SEC_P3:
					swprintf( zString, L"P3: %s", pLandTypeStrings[SHELTER] );
					break;
				default:
					if ( pUnderground->ubNumCreatures )
						swprintf( zString, L"%c%d: %s", 'A' + sSectorY - 1, sSectorX, pLandTypeStrings[CREATURE_LAIR] );
					else
						swprintf( zString, L"%c%d-%d", 'A' + sSectorY - 1, sSectorX, bSectorZ );
					break;
				}
			}
			else
			{ //Display nothing
				wcscpy( zString, L"" );
			}
		}
#else

		// TODO: the code is riddled with (potential) buffer overruns
		// this totally needs to be fixed sometime
		// anyway, only trying to not create another one here
		// found calls to this function with buffers as small as 50 elements
		g_luaUnderground.GetSectorName( sSectorX, sSectorY, bSectorZ, pUnderground, zString, 50, fDetailed );

#endif
	}
	else // SECTORS ABOVE GROUND
	{

		bTownNameID = StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].bNameId;
		ubSectorID = (UINT8)SECTOR( sSectorX, sSectorY );
		AssertGE( ubSectorID, 0 );
		AssertLT( ubSectorID, 256 );
		pSector = &SectorInfo[ubSectorID];
		ubLandType = pSector->ubTraversability[4];
		swprintf( zString, L"%c%d: ", 'A' + sSectorY - 1, sSectorX );

		////////////////////////////////////
		// Read and verify XML sector names
		BOOLEAN fSectorHasXMLNames = TRUE;
		CHAR16 zUnexplored[MAX_SECTOR_NAME_LENGTH];
		CHAR16 zDetailedUnexplored[MAX_SECTOR_NAME_LENGTH];
		CHAR16 zExplored[MAX_SECTOR_NAME_LENGTH];
		CHAR16 zDetailedExplored[MAX_SECTOR_NAME_LENGTH];

		wcscpy( zUnexplored, gzSectorNames[ubSectorID][0] );
		wcscpy( zDetailedUnexplored, gzSectorNames[ubSectorID][1] );
		wcscpy( zExplored, gzSectorNames[ubSectorID][2] );
		wcscpy( zDetailedExplored, gzSectorNames[ubSectorID][3] );

		if ( zUnexplored[0] == 0 || zDetailedUnexplored[0] == 0 || zExplored[0] == 0 || zDetailedExplored[0] == 0 )
		{
			fSectorHasXMLNames = FALSE;
		}

		if ( fSectorHasXMLNames ) // ABOVE GROUND XML
		{
			// HEADROCK HAM 3.6: The program can now read custom names from XML for all above-ground sectors.
			// In the event that a specific name or set of names is missing, the program generates a default
			// name as it always has.
			// I've also updated the SAM Site sectors to rely on SamSite.XML data.

			BOOLEAN fVisited = (SectorInfo[ubSectorID].uiFlags & SF_ALREADY_VISITED);
			BOOLEAN fSAMSiteKnown = FALSE;

			// Test for known SAM Site at this location
			for ( UINT16 x = 0; x < MAX_NUMBER_OF_SAMS; x++ )
			{
				if ( pSamList[x] == ubSectorID )
				{
					if ( fSamSiteFound[x] )
					{
						fSAMSiteKnown = TRUE;
						break;
					}
				}
			}

			if ( fVisited || fSAMSiteKnown )
			{
				if ( fDetailed )
				{
					wcscat( zString, zDetailedExplored );
				}
				else
				{
					wcscat( zString, zExplored );
				}
			}
			else
			{
				if ( fDetailed )
				{
					wcscat( zString, zDetailedUnexplored );
				}
				else
				{
					wcscat( zString, zUnexplored );
				}
			}
		}
		else // ABOVE GROUND HARDCODED
		{
			// Default JA2 Name selection
			if ( bTownNameID == BLANK_SECTOR )
			{
				// OK, build string id like J11
				// are we dealing with the unfound towns?
				switch ( ubSectorID )
				{
				case SEC_D2: //Chitzena SAM
					if ( !fSamSiteFound[SAM_SITE_ONE] )
						wcscat( zString, pLandTypeStrings[TROPICS] );
					else if ( fDetailed )
						wcscat( zString, pLandTypeStrings[TROPICS_SAM_SITE] );
					else
						wcscat( zString, pLandTypeStrings[SAM_SITE] );
					break;
				case SEC_D15: //Drassen SAM
					if ( !fSamSiteFound[SAM_SITE_TWO] )
						wcscat( zString, pLandTypeStrings[SPARSE] );
					else if ( fDetailed )
						wcscat( zString, pLandTypeStrings[SPARSE_SAM_SITE] );
					else
						wcscat( zString, pLandTypeStrings[SAM_SITE] );
					break;
				case SEC_I8: //Cambria SAM
					if ( !fSamSiteFound[SAM_SITE_THREE] )
						wcscat( zString, pLandTypeStrings[SAND] );
					else if ( fDetailed )
						wcscat( zString, pLandTypeStrings[SAND_SAM_SITE] );
					else
						wcscat( zString, pLandTypeStrings[SAM_SITE] );
					break;
				default:
					wcscat( zString, pLandTypeStrings[ubLandType] );
					break;
				}
			}
			else
			{
				switch ( ubSectorID )
				{
				case SEC_B13:
					if ( fDetailed )
						wcscat( zString, pLandTypeStrings[DRASSEN_AIRPORT_SITE] );
					else
						wcscat( zString, pTownNames[DRASSEN] );
					break;
				case SEC_F8:
					if ( fDetailed )
						wcscat( zString, pLandTypeStrings[CAMBRIA_HOSPITAL_SITE] );
					else
						wcscat( zString, pTownNames[CAMBRIA] );
					break;
				case SEC_J9: //Tixa
					//if( !fFoundTixa )
					if ( gfHiddenTown[TIXA] == FALSE )
						wcscat( zString, pLandTypeStrings[SAND] );
					else
						wcscat( zString, pTownNames[TIXA] );
					break;
				case SEC_K4: //Orta
					//if( !fFoundOrta )
					if ( gfHiddenTown[ORTA] == FALSE )
						wcscat( zString, pLandTypeStrings[SWAMP] );
					else
						wcscat( zString, pTownNames[ORTA] );
					break;
				case SEC_N3:
					if ( fDetailed )
						wcscat( zString, pLandTypeStrings[MEDUNA_AIRPORT_SITE] );
					else
						wcscat( zString, pTownNames[MEDUNA] );
					break;
				default:
					if ( ubSectorID == SEC_N4 && fSamSiteFound[SAM_SITE_FOUR] )
					{	//Meduna's SAM site
						if ( fDetailed )
							wcscat( zString, pLandTypeStrings[MEDUNA_SAM_SITE] );
						else
							wcscat( zString, pLandTypeStrings[SAM_SITE] );
					}
					else
					{	//All other towns that are known since beginning of the game.
						wcscat( zString, pTownNames[bTownNameID] );
						if ( fDetailed )
						{
							/*
							UINT8 ubMineIndex = GetMineIndexForSector( sSectorX, sSectorY );
							if ( gMineStatus[ubMineIndex].sSectorX == sSectorX && gMineStatus[ubMineIndex].sSectorY == sSectorY )
							{
							swprintf( zString, L" %s", MineralsName[gMineStatus[ubMineIndex].ubMineType].sType );
							}
							*/
							switch ( ubSectorID )
							{ //Append the word, "mine" for town sectors containing a mine.
							case SEC_B2:
							case SEC_D4:
							case SEC_D13:
							case SEC_H3:
							case SEC_H8:
							case SEC_I14:
								wcscat( zString, L" " ); //space
								wcscat( zString, pwMineStrings[0] ); //then "Mine"
								break;
							}
						}
					}
					break;
				}
			}
		}
	}
}


UINT8 SetInsertionDataFromAdjacentMoveDirection( SOLDIERTYPE *pSoldier, UINT8 ubTacticalDirection, INT32 sAdditionalData )//dnl ch56 151009
{
	UINT8				ubDirection;
	EXITGRID		ExitGrid;

	// Set insertion code
	switch ( ubTacticalDirection )
	{
		// OK, we are using an exit grid - set insertion values...

	case 255:
		if ( !GetExitGrid( sAdditionalData, &ExitGrid ) )
		{
			AssertMsg( 0, String( "No valid Exit grid can be found when one was expected: SetInsertionDataFromAdjacentMoveDirection." ) );
		}
		ubDirection = 255;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
		pSoldier->usStrategicInsertionData = ExitGrid.usGridNo;
		pSoldier->bUseExitGridForReentryDirection = TRUE;
		break;

	case NORTH:
		ubDirection = NORTH_STRATEGIC_MOVE;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_SOUTH;
		break;
	case SOUTH:
		ubDirection = SOUTH_STRATEGIC_MOVE;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_NORTH;
		break;
	case EAST:
		ubDirection = EAST_STRATEGIC_MOVE;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_WEST;
		break;
	case WEST:
		ubDirection = WEST_STRATEGIC_MOVE;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_EAST;
		break;
	default:
		// Wrong direction given!
#ifdef JA2BETAVERSION
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Improper insertion direction %d given to SetInsertionDataFromAdjacentMoveDirection", ubTacticalDirection ) );
		ScreenMsg( FONT_RED, MSG_ERROR, L"Improper insertion direction %d given to SetInsertionDataFromAdjacentMoveDirection", ubTacticalDirection );
#endif
		ubDirection = EAST_STRATEGIC_MOVE;
		pSoldier->ubStrategicInsertionCode = INSERTION_CODE_WEST;
	}

	return(ubDirection);
}

UINT8 GetInsertionDataFromAdjacentMoveDirection( UINT8 ubTacticalDirection, INT32 sAdditionalData )//dnl ch56 151009
{
	UINT8				ubDirection;

	// Set insertion code
	switch ( ubTacticalDirection )
	{
		// OK, we are using an exit grid - set insertion values...

	case 255:
		ubDirection = 255;
		break;
	case NORTH:
		ubDirection = NORTH_STRATEGIC_MOVE;
		break;
	case SOUTH:
		ubDirection = SOUTH_STRATEGIC_MOVE;
		break;
	case EAST:
		ubDirection = EAST_STRATEGIC_MOVE;
		break;
	case WEST:
		ubDirection = WEST_STRATEGIC_MOVE;
		break;
	default:
		// Wrong direction given!
#ifdef JA2BETAVERSION
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Improper insertion direction %d given to GetInsertionDataFromAdjacentMoveDirection", ubTacticalDirection ) );
		ScreenMsg( FONT_RED, MSG_ERROR, L"Improper insertion direction %d given to GetInsertionDataFromAdjacentMoveDirection", ubTacticalDirection );
#endif
		ubDirection = EAST_STRATEGIC_MOVE;
	}

	return(ubDirection);
}

UINT8 GetStrategicInsertionDataFromAdjacentMoveDirection( UINT8 ubTacticalDirection, INT32 sAdditionalData )//dnl ch56 151009
{
	UINT8				ubDirection;

	// Set insertion code
	switch ( ubTacticalDirection )
	{
		// OK, we are using an exit grid - set insertion values...

	case 255:
		ubDirection = 255;
		break;
	case NORTH:
		ubDirection = INSERTION_CODE_SOUTH;
		break;
	case SOUTH:
		ubDirection = INSERTION_CODE_NORTH;
		break;
	case EAST:
		ubDirection = INSERTION_CODE_WEST;
		break;
	case WEST:
		ubDirection = INSERTION_CODE_EAST;
		break;
	default:
		// Wrong direction given!
#ifdef JA2BETAVERSION
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Improper insertion direction %d given to SetInsertionDataFromAdjacentMoveDirection", ubTacticalDirection ) );
		ScreenMsg( FONT_RED, MSG_ERROR, L"Improper insertion direction %d given to GetStrategicInsertionDataFromAdjacentMoveDirection", ubTacticalDirection );
#endif
		ubDirection = EAST_STRATEGIC_MOVE;
	}

	return(ubDirection);
}


void JumpIntoAdjacentSector( UINT8 ubTacticalDirection, UINT8 ubJumpCode, INT32 sAdditionalData )//dnl ch56 151009
{
	INT32 cnt;
	SOLDIERTYPE		*pSoldier;
	SOLDIERTYPE *pValidSoldier = NULL;
	GROUP *pGroup;
	UINT32 uiTraverseTime = 0;
	UINT8 ubDirection = 0xff;
	EXITGRID ExitGrid;
	INT8 bPrevAssignment;
	UINT8 ubPrevGroupID;

	// Set initial selected
	// ATE: moved this towards top...
	gubPreferredInitialSelectedGuy = (UINT8)gusSelectedSoldier;

	if ( ubJumpCode == JUMP_ALL_LOAD_NEW || ubJumpCode == JUMP_ALL_NO_LOAD )
	{
		// TODO: Check flags to see if we can jump!
		// Move controllable mercs!
		cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

		// look for all mercs on the same team,
		for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; cnt++, pSoldier++ )
		{
			// If we are controllable
			if ( OK_CONTROLLABLE_MERC( pSoldier ) && pSoldier->bAssignment == CurrentSquad( ) )
			{
				pValidSoldier = pSoldier;
				//This now gets handled by strategic movement.  It is possible that the
				//group won't move instantaneously.
				//pSoldier->sSectorX = sNewX;
				//pSoldier->sSectorY = sNewY;

				ubDirection = GetInsertionDataFromAdjacentMoveDirection( ubTacticalDirection, sAdditionalData );
				break;
			}
		}
	}
	else if ( (ubJumpCode == JUMP_SINGLE_LOAD_NEW || ubJumpCode == JUMP_SINGLE_NO_LOAD) )
	{
		// Use selected soldier...
		// This guy should always be 1 ) selected and 2 ) close enough to exit sector to leave
		if ( gusSelectedSoldier != NOBODY )
		{
			pValidSoldier = MercPtrs[gusSelectedSoldier];
			ubDirection = GetInsertionDataFromAdjacentMoveDirection( ubTacticalDirection, sAdditionalData );
		}

		// save info for desired squad and and time for all single mercs leaving their squad.
		Assert( pValidSoldier );
		bPrevAssignment = pValidSoldier->bAssignment;
		ubPrevGroupID = pValidSoldier->ubGroupID;

		if ( ubJumpCode == JUMP_SINGLE_NO_LOAD )
		{ // handle soldier moving by themselves
			HandleSoldierLeavingSectorByThemSelf( pValidSoldier );
		}
		else
		{ // now add char to a squad all their own
			AddCharacterToUniqueSquad( pValidSoldier );
		}
		if ( !pValidSoldier->ubNumTraversalsAllowedToMerge && bPrevAssignment < ON_DUTY )
		{
			PLAYERGROUP *pPlayer;
			pValidSoldier->ubDesiredSquadAssignment = bPrevAssignment;
			pValidSoldier->ubNumTraversalsAllowedToMerge = 2;
			pGroup = GetGroup( ubPrevGroupID );
			Assert( pGroup );
			Assert( pGroup->usGroupTeam == OUR_TEAM );
			Assert( pGroup->ubGroupSize );
			pPlayer = pGroup->pPlayerList;
			while ( pPlayer )
			{
				if ( pPlayer->pSoldier != pValidSoldier )
				{
					pPlayer->pSoldier->ubNumTraversalsAllowedToMerge = 100;
					pPlayer->pSoldier->ubDesiredSquadAssignment = NO_ASSIGNMENT;
				}
				pPlayer = pPlayer->next;
			}
		}
	}
	else
	{
		// OK, no jump code here given...
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "Improper jump code %d given to JumpIntoAdjacentSector", ubJumpCode ) );
	}

	Assert( pValidSoldier );


	//Now, determine the traversal time.
	pGroup = GetGroup( pValidSoldier->ubGroupID );
	AssertMsg( pGroup, String( "%S is not in a valid group (pSoldier->ubGroupID is %d)", pValidSoldier->name, pValidSoldier->ubGroupID ) );

	// If we are going through an exit grid, don't get traversal direction!
	if ( ubTacticalDirection != 255 )
	{
		if ( !gbWorldSectorZ )
		{
			uiTraverseTime = GetSectorMvtTimeForGroup( (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), ubDirection, pGroup );
		}
		else if ( gbWorldSectorZ > 0 )
		{ //We are attempting to traverse in an underground environment.  We need to use a complete different
			//method.  When underground, all sectors are instantly adjacent.
			uiTraverseTime = UndergroundTacticalTraversalTime( ubDirection );
		}
		if ( uiTraverseTime == 0xffffffff )
			AssertMsg( 0, "Attempting to tactically traverse to adjacent sector, despite being unable to do so." );
	}

	// Alrighty, we want to do whatever our omnipotent player asked us to do
	// this is what the ubJumpCode is for.
	// Regardless of that we were asked to do, we MUST walk OFF ( Ian loves this... )
	// So..... let's setup our people to walk off...
	// We deal with a pGroup here... if an all move or a group...

	// Setup some globals so our callback that deals when guys go off screen is handled....
	// Look in the handler function AllMercsHaveWalkedOffSector() below...
	gpAdjacentGroup = pGroup;
	gubAdjacentJumpCode = ubJumpCode;
	guiAdjacentTraverseTime = uiTraverseTime;
	gubTacticalDirection = ubTacticalDirection;
	gsAdditionalData = sAdditionalData;

	// If normal direction, use it!
	if ( ubTacticalDirection != 255 )
	{
		gsAdjacentSectorX = (INT16)(gWorldSectorX + DirXIncrementer[ubTacticalDirection]);
		gsAdjacentSectorY = (INT16)(gWorldSectorY + DirYIncrementer[ubTacticalDirection]);
		gbAdjacentSectorZ = pValidSoldier->bSectorZ;
	}
	else
	{
		// Take directions from exit grid info!
		if ( !GetExitGrid( sAdditionalData, &ExitGrid ) )
		{
			AssertMsg( 0, String( "Told to use exit grid at %d but one does not exist", sAdditionalData ) );
		}

		gsAdjacentSectorX = ExitGrid.ubGotoSectorX;
		gsAdjacentSectorY = ExitGrid.ubGotoSectorY;
		gbAdjacentSectorZ = ExitGrid.ubGotoSectorZ;
		gusDestExitGridNo = ExitGrid.usGridNo;
	}

	// Give guy(s) orders to walk off sector...
	if ( pGroup->usGroupTeam == OUR_TEAM )
	{
		//For player groups, update the soldier information
		PLAYERGROUP *curr;
		INT32 sGridNo;
		UINT8				ubNum = 0;

		curr = pGroup->pPlayerList;
		while ( curr )
		{
			// anv: passengers can't move anyway
			if ( curr->pSoldier->bAssignment == VEHICLE )
			{
				curr->pSoldier->ubWaitActionToDo = 0;
			}
			else if ( OK_CONTROLLABLE_MERC( curr->pSoldier ) )
			{
				if ( ubTacticalDirection != 255 )
				{
					sGridNo = PickGridNoNearestEdge( curr->pSoldier, ubTacticalDirection );

					curr->pSoldier->sPreTraversalGridNo = curr->pSoldier->sGridNo;

					if ( !TileIsOutOfBounds( sGridNo ) )
					{
						// Save wait code - this will make buddy walk off screen into oblivion
						curr->pSoldier->ubWaitActionToDo = 2;
						// This will set the direction so we know now to move into oblivion
						curr->pSoldier->aiData.uiPendingActionData1 = ubTacticalDirection;
					}
					else
					{
						AssertMsg( 0, String( "Failed to get good exit location for adjacentmove" ) );
					}

					curr->pSoldier->EVENT_GetNewSoldierPath( sGridNo, WALKING );
				}
				else
				{
					// Here, get closest location for exit grid....
					sGridNo = FindGridNoFromSweetSpotCloseToExitGrid( curr->pSoldier, sAdditionalData, 10, &ubDirection );

					if ( !TileIsOutOfBounds( sGridNo ) )
					{
						// Save wait code - this will make buddy walk off screen into oblivion
						//	curr->pSoldier->ubWaitActionToDo = 2;
					}
					else
					{
						AssertMsg( 0, String( "Failed to get good exit location for adjacentmove" ) );
					}

					// Don't worry about walk off screen, just stay at gridno...
					curr->pSoldier->ubWaitActionToDo = 1;

					// Set buddy go!
					gfPlotPathToExitGrid = TRUE;
					curr->pSoldier->EVENT_GetNewSoldierPath( sGridNo, WALKING );
					gfPlotPathToExitGrid = FALSE;

				}

				++ubNum;
			}
			else
			{
				// We will remove them later....
			}
			curr = curr->next;
		}

		// ATE: Do another round, removing guys from group that can't go on...
	BEGINNING_LOOP:

		curr = pGroup->pPlayerList;
		while ( curr )
		{
			if ( !OK_CONTROLLABLE_MERC( curr->pSoldier ) )
			{
				if ( OK_CONTROL_MERC( curr->pSoldier ) && curr->pSoldier->bAssignment == VEHICLE && pGroup->fVehicle )
				{
					//CHRISL: passengers in a vehicle movement group will not pass the OK_CONTROLLABLE_MERC check because their assignment is not "ON_DUTY".
					//	The above conditions should allow passengers in a vehicle movement group to remain in the group.
				}
				else
				{
					RemoveCharacterFromSquads( curr->pSoldier );
					goto BEGINNING_LOOP;
				}
			}
			curr = curr->next;
		}

		// OK, setup TacticalOverhead polling system that will notify us once everybody
		// has made it to our destination.
		if ( ubTacticalDirection != 255 )
		{
			SetActionToDoOnceMercsGetToLocation( WAIT_FOR_MERCS_TO_WALKOFF_SCREEN, ubNum, ubJumpCode, 0, 0 );
		}
		else
		{
			// Add new wait action here...
			SetActionToDoOnceMercsGetToLocation( WAIT_FOR_MERCS_TO_WALK_TO_GRIDNO, ubNum, ubJumpCode, 0, 0 );
		}

		// Lock UI!
		guiPendingOverrideEvent = LU_BEGINUILOCK;
		HandleTacticalUI( );
	}
}

void JumpIntoEscapedSector(UINT8 ubTacticalDirection)
{
	// Remove any incapacitated mercs from current squads and assign them to new squad
	UINT32 i = gTacticalStatus.Team[gbPlayerNum].bFirstID;
	UINT32 const lastID = gTacticalStatus.Team[gbPlayerNum].bLastID;
	INT8 currentSquad = -1;

	for (SOLDIERTYPE* pSoldier = MercPtrs[i]; i <= lastID; ++i, ++pSoldier)
	{
		// Are we not active in sector
		if (!pSoldier->bActive || !pSoldier->bInSector || pSoldier->stats.bLife >= OKLIFE)
		{
			continue;
		}
		if (currentSquad == -1)
		{
			currentSquad = AddCharacterToUniqueSquad(pSoldier);
			continue;
		}
		if (!AddCharacterToSquad(pSoldier, currentSquad))
		{
			currentSquad = AddCharacterToUniqueSquad(pSoldier);
		}
	}

	// Retreat squads that are capable of it
	for (size_t i = 0; i < NUMBER_OF_SQUADS; i++)
	{
		for (size_t j = 0; j < NUMBER_OF_SOLDIERS_PER_SQUAD; j++)
		{
			SOLDIERTYPE* pSoldier = Squad[i][j];
			if (pSoldier && OK_CONTROLLABLE_MERC(pSoldier))
			{
				GROUP* pGroup = GetGroup(pSoldier->ubGroupID);
				switch (ubTacticalDirection)
				{
				case NORTH:
					pGroup->ubPrevX = pGroup->ubSectorX;
					pGroup->ubPrevY = pGroup->ubSectorY - 1;
					break;
				case EAST:
					pGroup->ubPrevX = pGroup->ubSectorX + 1;
					pGroup->ubPrevY = pGroup->ubSectorY;
					break;
				case SOUTH:
					pGroup->ubPrevX = pGroup->ubSectorX;
					pGroup->ubPrevY = pGroup->ubSectorY + 1;
					break;
				case WEST:
					pGroup->ubPrevX = pGroup->ubSectorX - 1;
					pGroup->ubPrevY = pGroup->ubSectorY;
					break;
				default:
					break;
				}

				RetreatGroupToPreviousSector(pGroup);
				break;
			}
		}
	}
}

void HandleSoldierLeavingSectorByThemSelf( SOLDIERTYPE *pSoldier )
{
	// soldier leaving thier squad behind, will rejoin later
	// if soldier in a squad, set the fact they want to return here

	if ( pSoldier->bAssignment < ON_DUTY )
	{
		RemoveCharacterFromSquads( pSoldier );

		// are they in a group?..remove from group
		if ( pSoldier->ubGroupID != 0 )
		{
			// remove from group
			RemovePlayerFromGroup( pSoldier->ubGroupID, pSoldier );
			pSoldier->ubGroupID = 0;
		}
	}
	else
	{
		// otherwise, they are on thier own, not in a squad, simply remove mvt group
		if ( pSoldier->ubGroupID && pSoldier->bAssignment != VEHICLE )
		{ //Can only remove groups if they aren't persistant (not in a squad or vehicle)
			// delete group
			RemoveGroup( pSoldier->ubGroupID );
			pSoldier->ubGroupID = 0;
		}
	}

	// set to guard
	AddCharacterToUniqueSquad( pSoldier );

	if ( pSoldier->ubGroupID == 0 )
	{
		// create independant group
		UINT8 ubGroupId = CreateNewPlayerGroupDepartingFromSector( (UINT8)pSoldier->sSectorX, (UINT8)pSoldier->sSectorY );
		AddPlayerToGroup( ubGroupId, pSoldier );
	}
}

void AllMercsWalkedToExitGrid( )
{
	PLAYERGROUP *pPlayer;
	BOOLEAN fDone;
	// SANDRO was here.. made some tweaks, fixed some stuff
	BOOLEAN fEnemiesInLoadedSector = FALSE;
	if ( NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fEnemiesInLoadedSector = TRUE;
		////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - if enemies are in sector, handle morale hit for fleeing
		// Minor bug fix - check certain circumstances and don't reduce the morale if the fight is not over yet
		// if all mercs leaving, morale gets hit
		// if one merc only leaves, but there are others fighting on, don't reduce the morale
		// if sector is about to be loaded and there are enemies in the current, then all mercs are leaving, so the morale gets hit
		if ( (gubAdjacentJumpCode == JUMP_ALL_NO_LOAD && gpAdjacentGroup->ubGroupSize >= PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, (UINT8)gbWorldSectorZ )) ||
			 (gubAdjacentJumpCode == JUMP_SINGLE_NO_LOAD && PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, (UINT8)gbWorldSectorZ ) <= 1) ||
			 (gubAdjacentJumpCode == JUMP_ALL_LOAD_NEW || gubAdjacentJumpCode == JUMP_SINGLE_LOAD_NEW) )
		{
			HandleLoyaltyImplicationsOfMercRetreat( RETREAT_TACTICAL_TRAVERSAL, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			HandlePOWQuestState(Q_END, QUEST_INTERROGATION, gWorldSectorX, gWorldSectorY, gbWorldSectorZ);
		}
		////////////////////////////////////////////////////////////////////////////////////////

		HandlePotentialMoraleHitForSkimmingSectors( gpAdjacentGroup );
	}

	if ( gubAdjacentJumpCode == JUMP_ALL_NO_LOAD || gubAdjacentJumpCode == JUMP_SINGLE_NO_LOAD )
	{
		Assert( gpAdjacentGroup );
		//pPlayer = gpAdjacentGroup->pPlayerList; // SANDRO - why was this here twice?
		pPlayer = gpAdjacentGroup->pPlayerList;
		while ( pPlayer )
		{
			/////////////////////////////////////////////////////////////////////////////////
			// SANDRO - merc records - times retreated counter
			if ( fEnemiesInLoadedSector && pPlayer->pSoldier->ubProfile != NO_PROFILE )
				gMercProfiles[pPlayer->pSoldier->ubProfile].records.usBattlesRetreated++;
			/////////////////////////////////////////////////////////////////////////////////

			SetInsertionDataFromAdjacentMoveDirection( pPlayer->pSoldier, gubTacticalDirection, gsAdditionalData );

			RemoveSoldierFromTacticalSector( pPlayer->pSoldier, TRUE );

			pPlayer = pPlayer->next;
		}

		SetGroupSectorValue( gsAdjacentSectorX, gsAdjacentSectorY, (INT16)gbAdjacentSectorZ, gpAdjacentGroup->ubGroupID );
		AttemptToMergeSeparatedGroups( gpAdjacentGroup, FALSE );

		SetDefaultSquadOnSectorEntry( TRUE );

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - reset num enemies here if the sector was unloaded
		if ( gWorldSectorX == 0 && gWorldSectorY == 0 && gbWorldSectorZ == -1 )
		{
			memset( &(gTacticalStatus.bNumFoughtInBattle), 0, MAXTEAMS );
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		// SANDRO - reset number of enemies here, we are about to load a new sector
		memset( &(gTacticalStatus.bNumFoughtInBattle), 0, MAXTEAMS );

		//Because we are actually loading the new map, and we are physically traversing, we don't want
		//to bring up the prebattle interface when we arrive if there are enemies there.  This flag
		//ignores the initialization of the prebattle interface and clears the flag.
		gfTacticalTraversal = TRUE;
		gpTacticalTraversalGroup = gpAdjacentGroup;

		//Check for any unconcious and/or dead merc and remove them from the current squad, so that they
		//don't get moved to the new sector.
		fDone = FALSE;
		while ( !fDone )
		{
			fDone = FALSE;
			pPlayer = gpAdjacentGroup->pPlayerList;
			while ( pPlayer )
			{
				if ( pPlayer->pSoldier->stats.bLife < OKLIFE )
				{
					AddCharacterToUniqueSquad( pPlayer->pSoldier );
					break;
				}
				InitSoldierOppList( pPlayer->pSoldier );
				pPlayer = pPlayer->next;
			}
			if ( !pPlayer )
			{
				fDone = TRUE;
			}
		}

		// OK, Set insertion direction for all these guys....
		Assert( gpAdjacentGroup );
		pPlayer = gpAdjacentGroup->pPlayerList;
		while ( pPlayer )
		{
			/////////////////////////////////////////////////////////////////////////////////
			// SANDRO - merc records - times retreated counter
			if ( fEnemiesInLoadedSector && pPlayer->pSoldier->ubProfile != NO_PROFILE )
				gMercProfiles[pPlayer->pSoldier->ubProfile].records.usBattlesRetreated++;
			/////////////////////////////////////////////////////////////////////////////////

			SetInsertionDataFromAdjacentMoveDirection( pPlayer->pSoldier, gubTacticalDirection, gsAdditionalData );

			pPlayer = pPlayer->next;
		}
		SetGroupSectorValue( gsAdjacentSectorX, gsAdjacentSectorY, (INT16)gbAdjacentSectorZ, gpAdjacentGroup->ubGroupID );
		AttemptToMergeSeparatedGroups( gpAdjacentGroup, FALSE );

		gFadeOutDoneCallback = DoneFadeOutExitGridSector;
		FadeOutGameScreen( );
	}
	// SANDRO - This actually never happeneds, moved up
	//if( !PlayerMercsInSector( (UINT8)gsAdjacentSectorX, (UINT8)gsAdjacentSectorY, (UINT8)gbAdjacentSectorZ ) )
	//{
	//	HandleLoyaltyImplicationsOfMercRetreat( RETREAT_TACTICAL_TRAVERSAL, gsAdjacentSectorX, gsAdjacentSectorY, gbAdjacentSectorZ );
	//}
	if ( gubAdjacentJumpCode == JUMP_ALL_NO_LOAD || gubAdjacentJumpCode == JUMP_SINGLE_NO_LOAD )
	{
		gfTacticalTraversal = FALSE;
		gpTacticalTraversalGroup = NULL;
		gpTacticalTraversalChosenSoldier = NULL;
	}
}

void SetupTacticalTraversalInformation( )
{
	SOLDIERTYPE *pSoldier;
	PLAYERGROUP *pPlayer;

	Assert( gpAdjacentGroup );
	pPlayer = gpAdjacentGroup->pPlayerList;
	while ( pPlayer )
	{
		pSoldier = pPlayer->pSoldier;

		SetInsertionDataFromAdjacentMoveDirection( pSoldier, gubTacticalDirection, gsAdditionalData );

		// pass flag that this is a tactical traversal, the path built MUST go in the traversed direction even if longer!
		PlotPathForCharacter( pSoldier, gsAdjacentSectorX, gsAdjacentSectorY, TRUE );

		//dnl ch48 041009
		if ( guiAdjacentTraverseTime <= 5 )
		{
			// Save this gridNo....
			pSoldier->aiData.sPendingActionData2 = GETWORLDINDEXFROMWORLDCOORDS( pSoldier->sY, pSoldier->sX );
			// Copy CODe computed earlier into data
			pSoldier->usStrategicInsertionData = pSoldier->ubStrategicInsertionCode;
			// Now use NEW code....
			pSoldier->ubStrategicInsertionCode = CalcMapEdgepointClassInsertionCode( pSoldier->sPreTraversalGridNo );
			if ( gubAdjacentJumpCode == JUMP_SINGLE_LOAD_NEW || gubAdjacentJumpCode == JUMP_ALL_LOAD_NEW )
				fUsingEdgePointsForStrategicEntry = TRUE;
		}

		pPlayer = pPlayer->next;
	}
	if ( gubAdjacentJumpCode == JUMP_ALL_NO_LOAD || gubAdjacentJumpCode == JUMP_SINGLE_NO_LOAD )
	{
		gfTacticalTraversal = FALSE;
		gpTacticalTraversalGroup = NULL;
		gpTacticalTraversalChosenSoldier = NULL;
	}
}

void AllMercsHaveWalkedOffSector( )
{
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "AllMercsHaveWalkedOffSector" );

	PLAYERGROUP *pPlayer;
	BOOLEAN fEnemiesInLoadedSector = FALSE;

	if ( NumEnemiesInAnySector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ ) )
	{
		fEnemiesInLoadedSector = TRUE;
		////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - if enemies are in sector, handle morale hit for fleeing
		// Minor bug fix - check certain circumstances and don't reduce the morale if the fight is not over yet
		// if all mercs leaving, morale gets hit
		// if one merc only leaves, but there are others fighting on, don't reduce the morale
		// if sector is about to be loaded and there are enemies in the current, then all mercs are leaving alway
		if ( (gubAdjacentJumpCode == JUMP_ALL_NO_LOAD && gpAdjacentGroup->ubGroupSize >= PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, (UINT8)gbWorldSectorZ )) ||
			 (gubAdjacentJumpCode == JUMP_SINGLE_NO_LOAD && PlayerMercsInSector( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY, (UINT8)gbWorldSectorZ ) <= 1) ||
			 (gubAdjacentJumpCode == JUMP_ALL_LOAD_NEW || gubAdjacentJumpCode == JUMP_SINGLE_LOAD_NEW) )
		{
			HandleLoyaltyImplicationsOfMercRetreat( RETREAT_TACTICAL_TRAVERSAL, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			HandlePOWQuestState(Q_END, QUEST_INTERROGATION, gWorldSectorX, gWorldSectorY, gbWorldSectorZ);
		}
		////////////////////////////////////////////////////////////////////////////////////////
	}

	//Setup strategic traversal information
	if ( guiAdjacentTraverseTime <= 5 )
	{
		gfTacticalTraversal = TRUE;
		gpTacticalTraversalGroup = gpAdjacentGroup;

		if ( gbAdjacentSectorZ > 0 && guiAdjacentTraverseTime <= 5 )
		{	//Nasty strategic movement logic doesn't like underground sectors!
			gfUndergroundTacticalTraversal = TRUE;
		}
	}
	ClearMercPathsAndWaypointsForAllInGroup( gpAdjacentGroup );
	AddWaypointToPGroup( gpAdjacentGroup, (UINT8)gsAdjacentSectorX, (UINT8)gsAdjacentSectorY );
	if ( gbAdjacentSectorZ > 0 && guiAdjacentTraverseTime <= 5 )
	{	//Nasty strategic movement logic doesn't like underground sectors!
		gfUndergroundTacticalTraversal = TRUE;
	}

	SetupTacticalTraversalInformation( );

	// ATE: Added here: donot load another screen if we were told not to....
	if ( (gubAdjacentJumpCode == JUMP_ALL_NO_LOAD || gubAdjacentJumpCode == JUMP_SINGLE_NO_LOAD) )
	{ //Case 1:  Group is leaving sector, but there are other mercs in sector and player wants to stay, or
		//         there are other mercs in sector while a battle is in progress.
		pPlayer = gpAdjacentGroup->pPlayerList;
		while ( pPlayer )
		{
			/////////////////////////////////////////////////////////////////////////////////
			// SANDRO - merc records - times retreated counter
			if ( fEnemiesInLoadedSector && pPlayer->pSoldier->ubProfile != NO_PROFILE )
				gMercProfiles[pPlayer->pSoldier->ubProfile].records.usBattlesRetreated++;
			/////////////////////////////////////////////////////////////////////////////////
			RemoveSoldierFromTacticalSector( pPlayer->pSoldier, TRUE );
			pPlayer = pPlayer->next;
		}
		SetDefaultSquadOnSectorEntry( TRUE );
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - reset num enemies here if the sector was unloaded
		if ( !fEnemiesInLoadedSector || (gWorldSectorX == 0 && gWorldSectorY == 0 && gbWorldSectorZ == -1) )
		{
			memset( &(gTacticalStatus.bNumFoughtInBattle), 0, MAXTEAMS );
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		// SANDRO - reset number of enemies here, we are about to load a new sector
		memset( &(gTacticalStatus.bNumFoughtInBattle), 0, MAXTEAMS );

		if ( fEnemiesInLoadedSector )
		{
			/////////////////////////////////////////////////////////////////////////////////
			// SANDRO - merc records - times retreated counter
			pPlayer = gpAdjacentGroup->pPlayerList;
			while ( pPlayer )
			{
				if ( fEnemiesInLoadedSector && pPlayer->pSoldier->ubProfile != NO_PROFILE )
					gMercProfiles[pPlayer->pSoldier->ubProfile].records.usBattlesRetreated++;
				pPlayer = pPlayer->next;
			}
			/////////////////////////////////////////////////////////////////////////////////
			//We are retreating from a sector with enemies in it and there are no mercs left  so
			//warp the game time by 5 minutes to simulate the actual retreat.  This restricts the
			//player from immediately coming back to the same sector they left to perhaps take advantage
			//of the tactical placement gui to get into better position.  Additionally, if there are any
			//enemies in this sector that are part of a movement group, reset that movement group so that they
			//are "in" the sector rather than 75% of the way to the next sector if that is the case.
			ResetMovementForNonPlayerGroupsInLocation( (UINT8)gWorldSectorX, (UINT8)gWorldSectorY );

			if ( guiAdjacentTraverseTime > 5 )
			{
				//Because this final group is retreating, simulate extra time to retreat, so they can't immediately come back.
				WarpGameTime( 300, WARPTIME_NO_PROCESSING_OF_EVENTS );
			}
		}
		if ( guiAdjacentTraverseTime <= 5 )
		{
			//Case 2:  Immediatly loading the next sector
			if ( !gbAdjacentSectorZ )
			{
				UINT32 uiWarpTime;
				uiWarpTime = (GetWorldTotalMin( ) + 5) * 60 - GetWorldTotalSeconds( );
				WarpGameTime( uiWarpTime, WARPTIME_PROCESS_TARGET_TIME_FIRST );
			}
			else if ( gbAdjacentSectorZ > 0 )
			{
				UINT32 uiWarpTime;
				uiWarpTime = (GetWorldTotalMin( ) + 1) * 60 - GetWorldTotalSeconds( );
				WarpGameTime( uiWarpTime, WARPTIME_PROCESS_TARGET_TIME_FIRST );
			}

			//Because we are actually loading the new map, and we are physically traversing, we don't want
			//to bring up the prebattle interface when we arrive if there are enemies there.  This flag
			//ignores the initialization of the prebattle interface and clears the flag.
			gFadeOutDoneCallback = DoneFadeOutAdjacentSector;
			FadeOutGameScreen( );
		}
		else
		{ //Case 3:  Going directly to mapscreen

			//Lock game into mapscreen mode, but after the fade is done.
			gfEnteringMapScreen = TRUE;

			// ATE; Fade FAST....
			SetMusicFadeSpeed( 5 );

#ifdef NEWMUSIC
			GlobalSoundID = MusicSoundValues[SECTOR( gWorldSectorX, gWorldSectorY )].SoundTacticalNothing[gbWorldSectorZ];
			if ( MusicSoundValues[SECTOR( gWorldSectorX, gWorldSectorY )].SoundTacticalNothing[gbWorldSectorZ] != -1 )
				SetMusicModeID( MUSIC_TACTICAL_NOTHING, MusicSoundValues[SECTOR( gWorldSectorX, gWorldSectorY )].SoundTacticalNothing[gbWorldSectorZ] );
			else
#endif
				SetMusicMode( MUSIC_TACTICAL_NOTHING );
		}
	}
	DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "AllMercsHaveWalkedOffSector done" );
}

void DoneFadeOutExitGridSector( )
{
	SetCurrentWorldSector( gsAdjacentSectorX, gsAdjacentSectorY, gbAdjacentSectorZ );
	if ( gfTacticalTraversal && gpTacticalTraversalGroup && gpTacticalTraversalChosenSoldier )
	{
		if ( gTacticalStatus.fEnemyInSector )
		{
			TacticalCharacterDialogueWithSpecialEvent( gpTacticalTraversalChosenSoldier, QUOTE_ENEMY_PRESENCE, 0, 0, 0 );
		}
	}
	gfTacticalTraversal = FALSE;
	gpTacticalTraversalGroup = NULL;
	gpTacticalTraversalChosenSoldier = NULL;
	FadeInGameScreen( );
}

void DoneFadeOutAdjacentSector( )
{
	UINT8 ubDirection;
	SetCurrentWorldSector( gsAdjacentSectorX, gsAdjacentSectorY, gbAdjacentSectorZ );

	ubDirection = GetStrategicInsertionDataFromAdjacentMoveDirection( gubTacticalDirection, gsAdditionalData );
	if ( gfTacticalTraversal && gpTacticalTraversalGroup && gpTacticalTraversalChosenSoldier )
	{
		if ( gTacticalStatus.fEnemyInSector )
		{
			TacticalCharacterDialogueWithSpecialEvent( gpTacticalTraversalChosenSoldier, QUOTE_ENEMY_PRESENCE, 0, 0, 0 );
		}
	}
	gfTacticalTraversal = FALSE;
	gpTacticalTraversalGroup = NULL;
	gpTacticalTraversalChosenSoldier = NULL;

	if ( gfCaves )
	{
		// ATE; Set tactical status flag...
		gTacticalStatus.uiFlags |= IGNORE_ALL_OBSTACLES;
		// Set pathing flag to path through anything....
		gfPathAroundObstacles = FALSE;
	}

	// OK, give our guys new orders...
	if ( gpAdjacentGroup->usGroupTeam == OUR_TEAM )
	{
		//For player groups, update the soldier information
		PLAYERGROUP *curr;
		UINT32 uiAttempts;
		INT32 sGridNo, sOldGridNo;
		UINT8				ubNum = 0;
		INT16 sWorldX, sWorldY;
		curr = gpAdjacentGroup->pPlayerList;
		while ( curr )
		{
			if ( !(curr->pSoldier->flags.uiStatusFlags & SOLDIER_IS_TACTICALLY_VALID) )
			{
				if ( !TileIsOutOfBounds( curr->pSoldier->sGridNo ) )
				{
					sGridNo = PickGridNoToWalkIn( curr->pSoldier, ubDirection, &uiAttempts );

					//If the search algorithm failed due to too many attempts, simply reset the
					//the gridno as the destination is a reserved gridno and we will place the
					//merc there without walking into the sector.

					if ( TileIsOutOfBounds( sGridNo ) && uiAttempts == MAX_ATTEMPTS )
					{
						sGridNo = curr->pSoldier->sGridNo;
					}

					if ( !TileIsOutOfBounds( sGridNo ) )
					{
						curr->pSoldier->ubWaitActionToDo = 1;
						// OK, here we have been given a position, a gridno has been given to use as well....
						sOldGridNo = curr->pSoldier->sGridNo;
						ConvertGridNoToCenterCellXY(sGridNo, &sWorldX, &sWorldY);

						curr->pSoldier->EVENT_SetSoldierPosition( sWorldX, sWorldY );
						if ( sGridNo != sOldGridNo )
						{
							curr->pSoldier->EVENT_GetNewSoldierPath( sOldGridNo, WALKING );
						}
						ubNum++;
					}
				}
				else
				{
#ifdef JA2BETAVERSION
					CHAR8 str[256];
					sprintf( str, "%S's gridno is NOWHERE, and is attempting to walk into sector.", curr->pSoldier->name );
					DebugMsg( TOPIC_JA2, DBG_LEVEL_3, str );
#endif
				}
			}
			curr = curr->next;
		}
		SetActionToDoOnceMercsGetToLocation( WAIT_FOR_MERCS_TO_WALKON_SCREEN, ubNum, 0, 0, 0 );
		guiPendingOverrideEvent = LU_BEGINUILOCK;
		HandleTacticalUI( );

		// Unset flag here.....
		gfPathAroundObstacles = TRUE;

	}
	FadeInGameScreen( );
}



BOOLEAN SoldierOKForSectorExit( SOLDIERTYPE * pSoldier, INT8 bExitDirection, INT32 usAdditionalData )//dnl ch56 151009
{
	INT16 sXMapPos;
	INT16 sYMapPos;
	INT16 sWorldX;
	INT16 sWorldY;
	UINT8	ubDirection;
	INT32 sGridNo;
	INT16	sAPs;

	// if the soldiers gridno is not NOWHERE	
	if ( TileIsOutOfBounds( pSoldier->sGridNo ) )
		return(FALSE);

	// OK, anyone on roofs cannot!
	if ( pSoldier->pathing.bLevel > 0 )
		return(FALSE);

	// get world absolute XY
	ConvertGridNoToXY( pSoldier->sGridNo, &sXMapPos, &sYMapPos );

	// Get screen coordinates for current position of soldier
	GetWorldXYAbsoluteScreenXY( sXMapPos, sYMapPos, &sWorldX, &sWorldY );

	// Check direction
	switch ( bExitDirection )
	{
	case EAST_STRATEGIC_MOVE:

		if ( sWorldX < ((gsTRX - gsTLX) - CHECK_DIR_X_DELTA) )
		{
			// NOT OK, return FALSE
			return(FALSE);
		}
		break;

	case WEST_STRATEGIC_MOVE:

		if ( sWorldX > CHECK_DIR_X_DELTA )
		{
			// NOT OK, return FALSE
			return(FALSE);
		}
		break;

	case SOUTH_STRATEGIC_MOVE:

		if ( sWorldY < ((gsBLY - gsTRY) - CHECK_DIR_Y_DELTA) )
		{
			// NOT OK, return FALSE
			return(FALSE);
		}
		break;

	case NORTH_STRATEGIC_MOVE:

		if ( sWorldY > CHECK_DIR_Y_DELTA )
		{
			// NOT OK, return FALSE
			return(FALSE);
		}
		break;

		// This case is for an exit grid....
		// check if we are close enough.....

	case -1:


		// FOR REALTIME - DO MOVEMENT BASED ON STANCE!
		if ( (gTacticalStatus.uiFlags & REALTIME) || !(gTacticalStatus.uiFlags & INCOMBAT) )
		{
			pSoldier->usUIMovementMode = pSoldier->GetMoveStateBasedOnStance( gAnimControl[pSoldier->usAnimState].ubEndHeight );
		}

		sGridNo = FindGridNoFromSweetSpotCloseToExitGrid( pSoldier, usAdditionalData, 10, &ubDirection );

		if ( TileIsOutOfBounds( sGridNo ) )
		{
			return(FALSE);
		}

		// ATE: if we are in combat, get cost to move here....
		if ( gTacticalStatus.uiFlags & INCOMBAT )
		{
			// Turn off at end of function...
			sAPs = PlotPath( pSoldier, sGridNo, NO_COPYROUTE, NO_PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );

			if ( !EnoughPoints( pSoldier, sAPs, 0, FALSE ) )
			{
				return(FALSE);
			}
		}
		break;

	}
	return(TRUE);
}

//ATE: Returns FALSE if NOBODY is close enough, 1 if ONLY selected guy is and 2 if all on squad are...
BOOLEAN OKForSectorExit( INT8 bExitDirection, INT32 usAdditionalData, UINT32 *puiTraverseTimeInMinutes )//dnl ch56 151009
{
	INT32 cnt;
	SOLDIERTYPE		*pSoldier;
	BOOLEAN		fAtLeastOneMercControllable = FALSE;
	BOOLEAN		fOnlySelectedGuy = FALSE;
	SOLDIERTYPE *pValidSoldier = NULL;
	UINT8			ubReturnVal = FALSE;
	UINT8			ubNumControllableMercs = 0;
	UINT8		  ubNumMercs = 0, ubNumEPCs = 0;
	UINT8     ubPlayerControllableMercsInSquad = 0;

	if ( gusSelectedSoldier == NOBODY )
	{ //must have a selected soldier to be allowed to tactically traverse.
		return FALSE;
	}

	// anv: vehicles can't use inner exit grids
	if ( bExitDirection == (-1) && MercPtrs[gusSelectedSoldier]->bAssignment == VEHICLE )
	{
		return FALSE;
	}

	/*
	//Exception code for the two sectors in San Mona that are separated by a cliff.  We want to allow strategic
	//traversal, but NOT tactical traversal.  The only way to tactically go from D4 to D5 (or viceversa) is to enter
	//the cave entrance.
	if( gWorldSectorX == 4 && gWorldSectorY == 4 && !gbWorldSectorZ && bExitDirection == EAST_STRATEGIC_MOVE )
	{
	gfInvalidTraversal = TRUE;
	return FALSE;
	}
	if( gWorldSectorX == 5 && gWorldSectorY == 4 && !gbWorldSectorZ && bExitDirection == WEST_STRATEGIC_MOVE )
	{
	gfInvalidTraversal = TRUE;
	return FALSE;
	}
	*/

	gfInvalidTraversal = FALSE;
	gfLoneEPCAttemptingTraversal = FALSE;
	gubLoneMercAttemptingToAbandonEPCs = 0;
	gbPotentiallyAbandonedEPCSlotID = -1;

	// Look through all mercs and check if they are within range of east end....
	cnt = gTacticalStatus.Team[gbPlayerNum].bFirstID;

	// look for all mercs on the same team,
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[gbPlayerNum].bLastID; ++cnt, ++pSoldier )
	{
		// If we are controllable
		if ( OK_CONTROLLABLE_MERC( pSoldier ) && (pSoldier->bAssignment == CurrentSquad( ) ||
			(pSoldier->bAssignment == VEHICLE && pSoldier->iVehicleId != iHelicopterVehicleId && GetSoldierStructureForVehicle( pSoldier->iVehicleId )->bAssignment == CurrentSquad( ))) )
		{
			//Need to keep a copy of a good soldier, so we can access it later, and
			//not more than once.
			pValidSoldier = pSoldier;

			ubNumControllableMercs++;

			//We need to keep track of the number of EPCs and mercs in this squad.  If we have
			//only one merc and one or more EPCs, then we can't allow the merc to tactically traverse,
			//if he is the only merc near enough to traverse.
			if ( AM_AN_EPC( pSoldier ) )
			{
				ubNumEPCs++;
				//Also record the EPC's slot ID incase we later build a string using the EPC's name.
				gbPotentiallyAbandonedEPCSlotID = (INT8)cnt;
				if ( AM_A_ROBOT( pSoldier ) && !pSoldier->CanRobotBeControlled( ) )
				{
					gfRobotWithoutControllerAttemptingTraversal = TRUE;
					ubNumControllableMercs--;
					continue;
				}
			}
			else
			{
				ubNumMercs++;
			}

			if ( SoldierOKForSectorExit( pSoldier, bExitDirection, usAdditionalData ) )
			{
				fAtLeastOneMercControllable++;

				if ( cnt == gusSelectedSoldier )
				{
					fOnlySelectedGuy = TRUE;
				}
			}
			else
			{
				GROUP *pGroup;

				// ATE: Dont's assume exit grids here...
				if ( bExitDirection != -1 )
				{
					// Bob: make sure we have a valid movement group
					UINT8 soldierGroupId = GetSoldierGroupId(pValidSoldier);
					if (soldierGroupId == 0) {
						soldierGroupId = tryToRecoverSquadsAndMovementGroups(pValidSoldier);
					}

					//Now, determine if this is a valid path.
					pGroup = GetGroup(soldierGroupId);
					AssertMsg( pGroup, String( "%S is not in a valid group (pSoldier->ubGroupID is %d)", pValidSoldier->name, pValidSoldier->ubGroupID ) );
					
					if ( !gbWorldSectorZ )
					{
						*puiTraverseTimeInMinutes = GetSectorMvtTimeForGroup( (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), bExitDirection, pGroup );
					}
					else if ( gbWorldSectorZ > 1 )
					{ //We are attempting to traverse in an underground environment.  We need to use a complete different
						//method.  When underground, all sectors are instantly adjacent.
						*puiTraverseTimeInMinutes = UndergroundTacticalTraversalTime( bExitDirection );
					}
					if ( *puiTraverseTimeInMinutes == 0xffffffff )
					{
						gfInvalidTraversal = TRUE;
						return FALSE;
					}
				}
				else
				{
					*puiTraverseTimeInMinutes = 0; //exit grid travel is instantaneous
				}
			}
		}
	}

	// If we are here, at least one guy is controllable in this sector, at least he can go!
	if ( fAtLeastOneMercControllable )
	{
		ubPlayerControllableMercsInSquad = (UINT8)NumberOfPlayerControllableMercsInSquad( MercPtrs[gusSelectedSoldier]->bAssignment );
		if ( fAtLeastOneMercControllable <= ubPlayerControllableMercsInSquad )
		{ //if the selected merc is an EPC and we can only leave with that merc, then prevent it
			//as EPCs aren't allowed to leave by themselves.  Instead of restricting this in the
			//exiting sector gui, we restrict it by explaining it with a message box.
			if ( AM_AN_EPC( MercPtrs[gusSelectedSoldier] ) )
			{
				if ( AM_A_ROBOT( pSoldier ) && !pSoldier->CanRobotBeControlled( ) )
				{
					//gfRobotWithoutControllerAttemptingTraversal = TRUE;
					return FALSE;
				}
				else if ( fAtLeastOneMercControllable < ubPlayerControllableMercsInSquad || fAtLeastOneMercControllable == 1 )
				{
					gfLoneEPCAttemptingTraversal = TRUE;
					return FALSE;
				}
			}
			else
			{	//We previously counted the number of EPCs and mercs, and if the selected merc is not an EPC and there are no
				//other mercs in the squad able to escort the EPCs, we will prohibit this merc from tactically traversing.
				if ( ubNumEPCs && ubNumMercs == 1 && fAtLeastOneMercControllable < ubPlayerControllableMercsInSquad )
				{
					gubLoneMercAttemptingToAbandonEPCs = ubNumEPCs;
					return FALSE;
				}
			}
		}
		if ( bExitDirection != -1 )
		{
			GROUP *pGroup;
			//Now, determine if this is a valid path.
			pGroup = GetGroup( pValidSoldier->ubGroupID );
			AssertMsg( pGroup, String( "%S is not in a valid group (pSoldier->ubGroupID is %d)", pValidSoldier->name, pValidSoldier->ubGroupID ) );
			if ( !gbWorldSectorZ )
			{
				*puiTraverseTimeInMinutes = GetSectorMvtTimeForGroup( (UINT8)SECTOR( pGroup->ubSectorX, pGroup->ubSectorY ), bExitDirection, pGroup );
			}
			else if ( gbWorldSectorZ > 0 )
			{ //We are attempting to traverse in an underground environment.  We need to use a complete different
				//method.  When underground, all sectors are instantly adjacent.
				*puiTraverseTimeInMinutes = UndergroundTacticalTraversalTime( bExitDirection );
			}
			if ( *puiTraverseTimeInMinutes == 0xffffffff )
			{
				gfInvalidTraversal = TRUE;
				ubReturnVal = FALSE;
			}
			else
			{
				ubReturnVal = TRUE;
			}
		}
		else
		{
			ubReturnVal = TRUE;
			*puiTraverseTimeInMinutes = 0; //exit grid travel is instantaneous
		}
	}

	if ( ubReturnVal )
	{
		// Default to FALSE again, until we see that we have
		ubReturnVal = FALSE;

		if ( fAtLeastOneMercControllable )
		{
			// Do we contain the selected guy?
			if ( fOnlySelectedGuy )
			{
				ubReturnVal = 1;
			}
			// Is the whole squad able to go here?
			if ( fAtLeastOneMercControllable == ubPlayerControllableMercsInSquad )
			{
				ubReturnVal = 2;
			}
		}
	}

	return(ubReturnVal);
}


void SetupNewStrategicGame( )
{
	// Set all sectors as enemy controlled
	for ( INT16 sSectorX = 0; sSectorX < MAP_WORLD_X; ++sSectorX )
	{
		for ( INT16 sSectorY = 0; sSectorY < MAP_WORLD_Y; ++sSectorY )
		{
			StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].fEnemyControlled = TRUE;
		}
	}

#ifdef JA2UB	
	//Ja25
	// Make the initial sector free of enemies
	StrategicMap[CALCULATE_STRATEGIC_INDEX( JA2_5_START_SECTOR_X, JA2_5_START_SECTOR_Y )].fEnemyControlled = FALSE;

#endif

	//Initialize the game time
	InitNewGameClock( );
	//Remove all events
	DeleteAllStrategicEvents( );

	//Set up all events that get processed daily...
	//.............................................
	BuildDayLightLevels( );
	// Check for quests each morning
	AddEveryDayStrategicEvent( EVENT_CHECKFORQUESTS, QUEST_CHECK_EVENT_TIME, 0 );
	// Some things get updated in the very early morning
	AddEveryDayStrategicEvent( EVENT_DAILY_EARLY_MORNING_EVENTS, EARLY_MORNING_TIME, 0 );
	//Daily Update BobbyRay Inventory
	AddEveryDayStrategicEvent( EVENT_DAILY_UPDATE_BOBBY_RAY_INVENTORY, BOBBYRAY_UPDATE_TIME, 0 );
	//Daily Update of the M.E.R.C. site.
	AddEveryDayStrategicEvent( EVENT_DAILY_UPDATE_OF_MERC_SITE, 0, 0 );
	// Flugente: plan when to conduct raids on player sectors
	AddEveryDayStrategicEvent( EVENT_DAILY_RAID_EVENTS, ENRICO_MAIL_TIME, 0 );

#ifdef JA2UB
	//Ja25:  No insurance for mercs
	//JA25: There is no mines
	//Ja25 no town opinions
#else	
	//Daily update of insured mercs
	AddEveryDayStrategicEvent( EVENT_HANDLE_INSURED_MERCS, INSURANCE_UPDATE_TIME, 0 );
	//Daily update of mercs
	AddEveryDayStrategicEvent( EVENT_MERC_DAILY_UPDATE, 0, 0 );
	// Daily mine production processing events
	AddEveryDayStrategicEvent( EVENT_SETUP_MINE_INCOME, 0, 0 );
	// Daily merc reputation processing events
	AddEveryDayStrategicEvent( EVENT_SETUP_TOWN_OPINION, 0, 0 );
	// Daily checks for E-mail from Enrico
	AddEveryDayStrategicEvent( EVENT_ENRICO_MAIL, ENRICO_MAIL_TIME, 0 );
#endif

	//	if ( gGameOptions.fAirStrikes )
	//	{
	//		//Daily check for an air raid
	//		AddEveryDayStrategicEvent( EVENT_CHECK_FOR_AIR_RAID, 360, 0 );
	//	}
	//
	// Hourly update of all sorts of things
	AddPeriodStrategicEvent( EVENT_HOURLY_UPDATE, 60, 0 );
	AddPeriodStrategicEvent( EVENT_QUARTER_HOUR_UPDATE, 15, 0 );

	// Flugente: ASD
	AddPeriodStrategicEvent( EVENT_ASD_UPDATE, 90, 0 );

	//Clear any possible battle locator
	gfBlitBattleSectorLocator = FALSE;

	StrategicTurnsNewGame( );
}


// a -1 will be returned upon failure
INT8 GetSAMIdFromSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	// check if valid sector
	if ( bSectorZ != 0 )
	{
		return(-1);
	}

	// get the sector value
	INT16 sSectorValue = SECTOR( sSectorX, sSectorY );

	// run through list of sam sites
	for ( INT8 bCounter = 0; bCounter < NUMBER_OF_SAMS; ++bCounter )
	{
		if ( pSamList[bCounter] == sSectorValue )
		{
			return(bCounter);
		}
	}

	return(-1);
}

void SetSamHackStatus( INT16 sSectorX, INT16 sSectorY, INT8 sStatus )
{
	if ( IsThisSectorASAMSector( sSectorX, sSectorY, 0 ) )
	{
		StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].sSamHackStatus = sStatus;

		UpdateAirspaceControl();
	}
}

BOOLEAN CanGoToTacticalInSector( INT16 sX, INT16 sY, UINT8 ubZ )
{
	INT32 cnt;
	SOLDIERTYPE *pSoldier;

	// if not a valid sector
	if ( (sX < 1) || (sX > 16) || (sY < 1) || (sY > 16) || (ubZ > 3) )
	{
		return(FALSE);
	}


	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;

	// look for all living, fighting mercs on player's team.  Robot and EPCs qualify!
	for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++, pSoldier++)
	{
		// ARM: now allows loading of sector with all mercs below OKLIFE as long as they're alive
		if( ( pSoldier->bActive && pSoldier->stats.bLife ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
			( pSoldier->bAssignment != IN_TRANSIT ) && ( pSoldier->bAssignment != ASSIGNMENT_POW ) && ( pSoldier->bAssignment != ASSIGNMENT_MINIEVENT ) && ( pSoldier->bAssignment != ASSIGNMENT_REBELCOMMAND ) &&
			( pSoldier->bAssignment != ASSIGNMENT_DEAD ) && !SoldierAboardAirborneHeli( pSoldier )
			)

		//if( (SectorInfo[ SECTOR( gWorldSectorX,gWorldSectorY) ].uiFlags & SF_ALREADY_VISITED) )
		if ( GetSectorFlagStatus( sSelMapX, sSelMapY, (UINT8)iCurrentMapSectorZ, SF_ALREADY_VISITED ) == TRUE )
		{
			if ( !pSoldier->flags.fBetweenSectors && pSoldier->sSectorX == sX && pSoldier->sSectorY == sY && pSoldier->bSectorZ == ubZ )
			{
				return(TRUE);
			}
		}
	}

	return(FALSE);
}


INT32 GetNumberOfSAMSitesUnderPlayerControl( void )
{
	INT32 iNumber = 0;

	// if the sam site is under player control, up the number
	for ( INT32 iCounter = 0; iCounter < NUMBER_OF_SAMS; ++iCounter )
	{
		if ( StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( pSamList[iCounter] )].fEnemyControlled == FALSE )
		{
			++iNumber;
		}
	}

	return(iNumber);
}

INT32 SAMSitesUnderPlayerControl( INT16 sX, INT16 sY )
{
	BOOLEAN fSamSiteUnderControl = FALSE;

	// is this sector a SAM sector?
	if ( IsThisSectorASAMSector( sX, sY, 0 ) == TRUE )
	{
		// is it under control by the player
		if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].fEnemyControlled == FALSE )
		{
			// yes
			fSamSiteUnderControl = TRUE;
		}
	}

	return(fSamSiteUnderControl);
}


void UpdateAirspaceControl( void )
{
	StrategicMapElement *pSAMStrategicMap = NULL;

	for ( INT32 iCounterA = 1; iCounterA < (INT32)(MAP_WORLD_X - 1); ++iCounterA )
	{
		for ( INT32 iCounterB = 1; iCounterB < (INT32)(MAP_WORLD_Y - 1); ++iCounterB )
		{
			StrategicMap[CALCULATE_STRATEGIC_INDEX( iCounterA, iCounterB )].usAirType = 0;

			// if networked, disable SAM airspace restrictions...
			if ( is_networked )
			{
				continue;
			}
			
			for ( int samcnt = 0; samcnt < NUMBER_OF_SAMS; ++samcnt )
			{
				BOOLEAN samworking = FALSE;
				if ( DoesSamCoverSector( samcnt, SECTOR( iCounterA, iCounterB ), &samworking ) )
				{					
					if ( samworking )
					{
						pSAMStrategicMap = &(StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( pSamList[samcnt] )]);

						// different status depending on who controls the SAM sector, and whether the SAM is operational
						if ( pSAMStrategicMap->fEnemyControlled )
						{
							StrategicMap[CALCULATE_STRATEGIC_INDEX( iCounterA, iCounterB )].usAirType |= AIRSPACE_ENEMY_ACTIVE;
						}
						else
						{
							StrategicMap[CALCULATE_STRATEGIC_INDEX( iCounterA, iCounterB )].usAirType |= AIRSPACE_PLAYER_ACTIVE;
						}
					}
				}
			}
		}
	}

	// check if currently selected arrival sector still has secure airspace

	// if it's not enemy air controlled
	if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( gsMercArriveSectorX, gsMercArriveSectorY )].usAirType & AIRSPACE_ENEMY_ACTIVE )
	{
		// NOPE!
		CHAR16 sMsgString[256], sMsgSubString1[64], sMsgSubString2[64];

		// get the name of the old sector
		GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, sMsgSubString1, FALSE );

		// move the landing zone over to Omerta
		// HEADROCK HAM 3.5: Externalized coordinates
		if ( !(StrategicMap[CALCULATE_STRATEGIC_INDEX( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY )].usAirType & AIRSPACE_ENEMY_ACTIVE) )
		{
			gsMercArriveSectorX = gGameExternalOptions.ubDefaultArrivalSectorX;
			gsMercArriveSectorY = gGameExternalOptions.ubDefaultArrivalSectorY;
		}
		else
		{
			for ( UINT8 ubSectorX = 1; ubSectorX <= 16; ++ubSectorX )
			{
				for ( UINT8 ubSectorY = 1; ubSectorY <= 16; ++ubSectorY )
				{
					if ( !(StrategicMap[CALCULATE_STRATEGIC_INDEX( ubSectorX, ubSectorY )].usAirType & AIRSPACE_ENEMY_ACTIVE) && !sBadSectorsList[ubSectorX][ubSectorY] )
					{
						gsMercArriveSectorX = ubSectorX;
						gsMercArriveSectorY = ubSectorY;
					}
				}
			}
		}

		// get the name of the new sector
		GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, sMsgSubString2, FALSE );

		// now build the string
		swprintf( sMsgString, pBullseyeStrings[4], sMsgSubString1, sMsgSubString2 );

		// confirm the change with overlay message
		// HEADROCK HAM 3.5: If we do this during SaveLoad, the message box prevents loading the game at all!
		if ( guiCurrentScreen != SAVE_LOAD_SCREEN )
		{
			DoScreenIndependantMessageBox( sMsgString, MSG_BOX_FLAG_OK, NULL );
		}

		// update position of bullseye
		fMapPanelDirty = TRUE;

		// update destination column for any mercs in transit
		fTeamPanelDirty = TRUE;
	}

	// ARM: airspace control now affects refueling site availability, so update that too with every change!
	UpdateRefuelSiteAvailability( );
}

// get the maximum distance from a SAM to its covered sectors (in principle, not taking into account status)
FLOAT GetSAMMaxDistanceToCoveredSector( UINT8 usSam )
{
	FLOAT maxdist = 0.0f;

	if ( usSam < NUMBER_OF_SAMS )
	{
		UINT8 samsector = pSamList[usSam];
		UINT8 sam_x = SECTORX( samsector );
		UINT8 sam_y = SECTORY( samsector );

		for ( int x = 1; x < MAP_WORLD_X - 1; ++x )
		{
			for ( int y = 1; y < MAP_WORLD_Y - 1; ++y )
			{
				if ( ubSAMControlledSectors[y][x] & (1 << usSam) )
				{
					FLOAT dist = sqrt( FLOAT((sam_x - x)*(sam_x - x) + (sam_y - y)*(sam_y - y)) );

					if ( maxdist < dist )
						maxdist = dist;
				}
			}
		}
	}

	return maxdist;
}


BOOLEAN DoesSamCoverSector( UINT8 usSam, UINT8 usSector, BOOLEAN* apSamIsWorking )
{
	if ( apSamIsWorking )
		*apSamIsWorking = FALSE;
	
	if ( usSam < NUMBER_OF_SAMS )
	{
		// can this sam control this sector in principle?
		if ( ubSAMControlledSectors[SECTORY( usSector )][SECTORX( usSector )] & (1 << usSam) )
		{
			UINT8 samsector = pSamList[usSam];

			if ( StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( samsector )].bSAMCondition >= MIN_CONDITION_FOR_SAM_SITE_TO_WORK && StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( samsector )].sSamHackStatus > 0 )
			{
				if ( apSamIsWorking )
					*apSamIsWorking = TRUE;
			}

			FLOAT dist = sqrt( FLOAT((SECTORX( samsector ) - SECTORX( usSector ))*(SECTORX( samsector ) - SECTORX( usSector )) + (SECTORY( samsector ) - SECTORY( usSector ))*(SECTORY( samsector ) - SECTORY( usSector ))) );

			// determine max distance SAM has to cover
			FLOAT sammaxdistance = GetSAMMaxDistanceToCoveredSector( usSam );

			// allowed range ratio according to status and hacking
			FLOAT samrangeratio = StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( samsector )].bSAMCondition * StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( samsector )].sSamHackStatus / 10000.0f;

			// can this SAM currently cover this sector?
			if ( dist <= sammaxdistance * samrangeratio )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOLEAN IsThereAFunctionalSAMSiteInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	if ( IsThisSectorASAMSector( sSectorX, sSectorY, bSectorZ ) == FALSE )
	{
		return(FALSE);
	}

	if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].bSAMCondition < MIN_CONDITION_FOR_SAM_SITE_TO_WORK )
		return(FALSE);

	if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].sSamHackStatus <= 0 )
		return(FALSE);

	return(TRUE);
}

BOOLEAN IsThisSectorASAMSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	// is the sector above ground?
	if ( bSectorZ != 0 )
	{
		return(FALSE);
	}

	for ( INT32 cnt = 0; cnt < NUMBER_OF_SAMS; ++cnt )
	{
		if ( (sSectorX == gpSamSectorX[cnt]) && (sSectorY == gpSamSectorY[cnt]) )
			return(TRUE);
	}

	return (FALSE);
}

void RepairSamSite( UINT16 strategicIndex )
{
	INT8 repairAmount = 0;
	switch (gGameOptions.ubDifficultyLevel)
	{
	case DIF_LEVEL_EASY:	repairAmount = AI_SAMSITE_REPAIR_EASY; break;
	case DIF_LEVEL_MEDIUM:	repairAmount = AI_SAMSITE_REPAIR_MEDIUM; break;
	case DIF_LEVEL_HARD:	repairAmount = AI_SAMSITE_REPAIR_HARD; break;
	case DIF_LEVEL_INSANE:	repairAmount = AI_SAMSITE_REPAIR_INSANE; break;
	}

	if ( StrategicMap[strategicIndex].fEnemyControlled && (StrategicMap[strategicIndex].usFlags & SAMSITE_REPAIR_ORDERED) && IsThisSectorASAMSector( GET_X_FROM_STRATEGIC_INDEX( strategicIndex ), GET_Y_FROM_STRATEGIC_INDEX( strategicIndex ), 0 ) )
	{
		if (StrategicMap[strategicIndex].bSAMCondition > 100 - repairAmount)
			StrategicMap[strategicIndex].bSAMCondition = 100;
		else
			StrategicMap[strategicIndex].bSAMCondition += repairAmount;

		if (StrategicMap[strategicIndex].sSamHackStatus > 100 - repairAmount)
			StrategicMap[strategicIndex].sSamHackStatus = 100;
		else
			StrategicMap[strategicIndex].sSamHackStatus += repairAmount;

		if (StrategicMap[strategicIndex].bSAMCondition < 100 || StrategicMap[strategicIndex].sSamHackStatus < 100)
			AddStrategicEvent(EVENT_SAMSITE_REPAIRED, GetWorldTotalMin() + 24 * 60, strategicIndex);
		else
			StrategicMap[strategicIndex].usFlags &= ~SAMSITE_REPAIR_ORDERED;

		if (StrategicMap[strategicIndex].bSAMCondition >= MIN_CONDITION_FOR_SAM_SITE_TO_WORK)
			UpdateSAMDoneRepair( GET_X_FROM_STRATEGIC_INDEX( strategicIndex ), GET_Y_FROM_STRATEGIC_INDEX( strategicIndex ), 0 );
	}
}


// is this sector part of the town?
BOOLEAN SectorIsPartOfTown( INT8 bTownId, INT16 sSectorX, INT16 sSectorY )
{
	if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sSectorX, sSectorY )].bNameId == bTownId )
	{
		// is in the town
		return (TRUE);
	}

	// not in the town
	return (FALSE);
}


BOOLEAN SaveStrategicInfoToSavedFile( HWFILE hFile )
{
	UINT32		uiNumBytesWritten = 0;
	UINT32		uiSize = sizeof(StrategicMapElement)* (MAP_WORLD_X * MAP_WORLD_Y);


	// Save the strategic map information
	FileWrite( hFile, StrategicMap, uiSize, &uiNumBytesWritten );
	if ( uiNumBytesWritten != uiSize )
	{
		return(FALSE);
	}

	// Save the Sector Info
	for ( int sectorID = 0; sectorID <= 255; ++sectorID ) {
		FileWrite( hFile, &SectorInfo[sectorID], sizeof(SECTORINFO), &uiNumBytesWritten );
		if ( uiNumBytesWritten != sizeof(SECTORINFO) ) {
			return(FALSE);
		}
	}
	//	uiSize = sizeof( SECTORINFO ) * 256;
	//	FileWrite( hFile, SectorInfo, uiSize, &uiNumBytesWritten );
	//	if( uiNumBytesWritten != uiSize)
	//	{
	//		return(FALSE);
	//	}

	// Save the SAM Controlled Sector Information
	uiSize = MAP_WORLD_X * MAP_WORLD_Y;
	/*
	FileWrite( hFile, ubSAMControlledSectors, uiSize, &uiNumBytesWritten );
	if( uiNumBytesWritten != uiSize)
	{
	return(FALSE);
	}
	*/
	FileSeek( hFile, uiSize, FILE_SEEK_FROM_CURRENT );

	// Save the fFoundOrta
	FileWrite( hFile, &fFoundOrta, sizeof(BOOLEAN), &uiNumBytesWritten );
	if ( uiNumBytesWritten != sizeof(BOOLEAN) )
	{
		return(FALSE);
	}



	return(TRUE);
}



BOOLEAN LoadStrategicInfoFromSavedFile( HWFILE hFile )
{
	UINT32		uiNumBytesRead = 0;

	UINT32		uiSize = sizeof(StrategicMapElement);
	if ( guiCurrentSaveGameVersion < MILITIA_MOVEMENT )
		uiSize = 41;

	for ( UINT16 i = 0; i < MAP_WORLD_X * MAP_WORLD_Y; ++i )
	{
		// Load the strategic map information
		FileRead( hFile, &StrategicMap[i], uiSize, &uiNumBytesRead );
		if ( uiNumBytesRead != uiSize )
		{
			return(FALSE);
		}

		if ( guiCurrentSaveGameVersion < MILITIA_MOVEMENT )
			StrategicMap[i].usFlags = 0;
	}

	if ( guiCurrentSaveGameVersion < HACKABLE_SAMS )
	{
		// all SAM sites start game in perfect working condition
		for ( UINT32 cnt = 0; cnt < NUMBER_OF_SAMS; ++cnt )
		{
			StrategicMap[CALCULATE_STRATEGIC_INDEX( gpSamSectorX[cnt], gpSamSectorY[cnt] )].sSamHackStatus = 100;
		}
	}
	
	// Load the Sector Info
	uiSize = sizeof(SECTORINFO);
	if ( guiCurrentSaveGameVersion < ENEMY_JEEPS )
		uiSize = 116;

	for ( int sectorID = 0; sectorID <= 255; ++sectorID )
	{
		FileRead( hFile, &SectorInfo[sectorID], uiSize, &uiNumBytesRead );
		if ( uiNumBytesRead != uiSize )
		{
			return(FALSE);
		}

		// Flugente: changes in SECTORINFO require a weird-looking remapping
		if ( guiCurrentSaveGameVersion < PRISONER_EXPANSION )
		{
			SectorInfo[sectorID].uiNumberOfPrisonersOfWar[PRISONER_GENERAL] = 0;
			SectorInfo[sectorID].uiNumberOfPrisonersOfWar[PRISONER_CIVILIAN] = 0;
			SectorInfo[sectorID].uiNumberOfPrisonersOfWar[PRISONER_SECRET1] = 0;
			SectorInfo[sectorID].uiNumberOfPrisonersOfWar[PRISONER_SECRET2] = 0;
			SectorInfo[sectorID].ubNumTanks = SectorInfo[sectorID].uiInterrogationHundredsLeft[PRISONER_GENERAL];
			SectorInfo[sectorID].ubTanksInBattle = SectorInfo[sectorID].uiInterrogationHundredsLeft[PRISONER_CIVILIAN];
		}

		if ( guiCurrentSaveGameVersion < WORKERS )
		{
			SectorInfo[sectorID].usWorkers = SectorExternalData[sectorID][0].maxworkers * gGameExternalOptions.dInitialWorkerRate;
			SectorInfo[sectorID].ubWorkerTrainingHundredths = 0;
		}

		if ( guiCurrentSaveGameVersion < CORPSE_DISPOSAL )
		{
			SectorInfo[sectorID].usNumCorpses = 0;		// this will get updated on the next full hour anyway
			SectorInfo[sectorID].fDiseasePoints = 0.0f;
			SectorInfo[sectorID].usInfectionFlag &= ~3;
			SectorInfo[sectorID].dBurial_UnappliedProgress = 0.0f;
		}
	}

	//	uiSize = sizeof( SECTORINFO ) * 256;
	//	FileRead( hFile, SectorInfo, uiSize, &uiNumBytesRead );
	//	if( uiNumBytesRead != uiSize)
	//	{
	//		return(FALSE);
	//	}

	// Load the SAM Controlled Sector Information
	uiSize = MAP_WORLD_X * MAP_WORLD_Y;
	/*
	FileRead( hFile, ubSAMControlledSectors, uiSize, &uiNumBytesRead );
	if( uiNumBytesRead != uiSize)
	{
	return(FALSE);
	}
	*/
	FileSeek( hFile, uiSize, FILE_SEEK_FROM_CURRENT );

	// Load the fFoundOrta
	FileRead( hFile, &fFoundOrta, sizeof(BOOLEAN), &uiNumBytesRead );
	if ( uiNumBytesRead != sizeof(BOOLEAN) )
	{
		return(FALSE);
	}

	return(TRUE);
}


INT32 PickGridNoNearestEdge( SOLDIERTYPE *pSoldier, UINT8 ubTacticalDirection )
{
	INT32 sGridNo, sStartGridNo, sOldGridNo;
	INT8			bOdd = 1, bOdd2 = 1;
	UINT8			bAdjustedDist = 0;
	UINT32		cnt;

	switch ( ubTacticalDirection )
	{

	case EAST:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the right!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo -= WORLD_COLS;
			}
			else
			{
				sGridNo++;
			}

			bOdd = (INT8)!bOdd;
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		do
		{
			// OK, here we go back one, check for OK destination...
			if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE ) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate up/down )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS - 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS + 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		} while ( TRUE );

		break;

	case WEST:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the left!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo += WORLD_COLS;
			}
			else
			{
				sGridNo--;
			}

			bOdd = (INT8)!bOdd;
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		do
		{
			// OK, here we go back one, check for OK destination...
			if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE ) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate up/down )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS - 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS + 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		} while ( TRUE );

		break;

	case NORTH:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the left!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo -= WORLD_COLS;
			}
			else
			{
				sGridNo--;
			}

			bOdd = (INT8)(!bOdd);
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		do
		{
			// OK, here we go back one, check for OK destination...
			if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE ) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate left/right )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS - 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS + 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		} while ( TRUE );

		break;

	case SOUTH:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the left!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo += WORLD_COLS;
			}
			else
			{
				sGridNo++;
			}

			bOdd = (INT8)(!bOdd);
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		do
		{
			// OK, here we go back one, check for OK destination...
			if ( NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE ) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate left/right )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS - 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS + 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		} while ( TRUE );

		break;

	}

	return(NOWHERE);
}


void AdjustSoldierPathToGoOffEdge( SOLDIERTYPE *pSoldier, INT32 sEndGridNo, UINT8 ubTacticalDirection )
{
	INT32 sNewGridNo, sTempGridNo;
	INT32	iLoop;

	// will this path segment actually take us to our desired destination in the first place?
	if ( pSoldier->pathing.usPathDataSize + 2 > MAX_PATH_LIST_SIZE )
	{

		sTempGridNo = pSoldier->sGridNo;

		for ( iLoop = 0; iLoop < pSoldier->pathing.usPathDataSize; iLoop++ )
		{
			sTempGridNo += DirectionInc( (UINT8)pSoldier->pathing.usPathingData[iLoop] );
		}

		if ( sTempGridNo == sEndGridNo )
		{
			// we can make it, but there isn't enough path room for the two steps required.
			// truncate our path so there's guaranteed the merc will have to generate another
			// path later on...
			pSoldier->pathing.usPathDataSize -= 4;
			return;
		}
		else
		{
			// can't even make it there with these 30 tiles of path, abort...
			return;
		}
	}

	switch ( ubTacticalDirection )
	{
	case EAST:

		sNewGridNo = NewGridNo( sEndGridNo, (UINT16)DirectionInc( (UINT8)NORTHEAST ) );

		if ( OutOfBounds( sEndGridNo, sNewGridNo ) )
		{
			return;
		}

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = NORTHEAST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;

		sTempGridNo = NewGridNo( sNewGridNo, (UINT16)DirectionInc( (UINT8)NORTHEAST ) );

		if ( OutOfBounds( sNewGridNo, sTempGridNo ) )
		{
			return;
		}
		sNewGridNo = sTempGridNo;

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = NORTHEAST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;

		break;

	case WEST:

		sNewGridNo = NewGridNo( sEndGridNo, (UINT16)DirectionInc( (UINT8)SOUTHWEST ) );

		if ( OutOfBounds( sEndGridNo, sNewGridNo ) )
		{
			return;
		}

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = SOUTHWEST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;

		sTempGridNo = NewGridNo( sNewGridNo, (UINT16)DirectionInc( (UINT8)SOUTHWEST ) );

		if ( OutOfBounds( sNewGridNo, sTempGridNo ) )
		{
			return;
		}
		sNewGridNo = sTempGridNo;

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = SOUTHWEST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;
		break;

	case NORTH:

		sNewGridNo = NewGridNo( sEndGridNo, (UINT16)DirectionInc( (UINT8)NORTHWEST ) );

		if ( OutOfBounds( sEndGridNo, sNewGridNo ) )
		{
			return;
		}

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = NORTHWEST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;

		sTempGridNo = NewGridNo( sNewGridNo, (UINT16)DirectionInc( (UINT8)NORTHWEST ) );

		if ( OutOfBounds( sNewGridNo, sTempGridNo ) )
		{
			return;
		}
		sNewGridNo = sTempGridNo;

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = NORTHWEST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;

		break;

	case SOUTH:

		sNewGridNo = NewGridNo( sEndGridNo, (UINT16)DirectionInc( (UINT8)SOUTHEAST ) );

		if ( OutOfBounds( sEndGridNo, sNewGridNo ) )
		{
			return;
		}

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = SOUTHEAST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;

		sTempGridNo = NewGridNo( sNewGridNo, (UINT16)DirectionInc( (UINT8)SOUTHEAST ) );

		if ( OutOfBounds( sNewGridNo, sTempGridNo ) )
		{
			return;
		}
		sNewGridNo = sTempGridNo;

		pSoldier->pathing.usPathingData[pSoldier->pathing.usPathDataSize] = SOUTHEAST;
		pSoldier->pathing.usPathDataSize++;
		pSoldier->pathing.sFinalDestination = sNewGridNo;
		pSoldier->aiData.usActionData = sNewGridNo;
		break;

	}
}

INT32 PickGridNoToWalkIn( SOLDIERTYPE *pSoldier, UINT8 ubInsertionDirection, UINT32 *puiNumAttempts )
{
	INT32 sGridNo, sStartGridNo, sOldGridNo;
	INT8			bOdd = 1, bOdd2 = 1;
	UINT8			bAdjustedDist = 0;
	UINT32		cnt;

	*puiNumAttempts = 0;

	switch ( ubInsertionDirection )
	{
		// OK, we're given a direction on visible map, let's look for the first oone
		// we find that is just on the start of visible map...
	case INSERTION_CODE_WEST:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the left!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo += WORLD_COLS;
			}
			else
			{
				sGridNo--;
			}

			bOdd = (INT8)(!bOdd);
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		while ( *puiNumAttempts < MAX_ATTEMPTS )
		{
			(*puiNumAttempts)++;
			// OK, here we go back one, check for OK destination...
			if ( (gTacticalStatus.uiFlags & IGNORE_ALL_OBSTACLES) || (NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE )) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate up/down )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS - 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS + 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		}
		return NOWHERE;

	case INSERTION_CODE_EAST:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the right!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo -= WORLD_COLS;
			}
			else
			{
				sGridNo++;
			}

			bOdd = (INT8)(!bOdd);
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		while ( *puiNumAttempts < MAX_ATTEMPTS )
		{
			(*puiNumAttempts)++;
			// OK, here we go back one, check for OK destination...
			if ( (gTacticalStatus.uiFlags & IGNORE_ALL_OBSTACLES) || (NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE )) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate up/down )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS - 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS + 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		}
		return NOWHERE;

	case INSERTION_CODE_NORTH:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the up!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo -= WORLD_COLS;
			}
			else
			{
				sGridNo--;
			}

			bOdd = (INT8)(!bOdd);
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		while ( *puiNumAttempts < MAX_ATTEMPTS )
		{
			(*puiNumAttempts)++;
			// OK, here we go back one, check for OK destination...
			if ( (gTacticalStatus.uiFlags & IGNORE_ALL_OBSTACLES) || (NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE )) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate left/right )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS + 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS - 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		}
		return NOWHERE;

	case INSERTION_CODE_SOUTH:

		sGridNo = pSoldier->sGridNo;
		sStartGridNo = pSoldier->sGridNo;
		sOldGridNo = pSoldier->sGridNo;

		// Move directly to the down!
		while ( GridNoOnVisibleWorldTile( sGridNo ) )
		{
			sOldGridNo = sGridNo;

			if ( bOdd )
			{
				sGridNo += WORLD_COLS;
			}
			else
			{
				sGridNo++;
			}

			bOdd = (INT8)(!bOdd);
		}

		sGridNo = sOldGridNo;
		sStartGridNo = sOldGridNo;

		while ( *puiNumAttempts < MAX_ATTEMPTS )
		{
			(*puiNumAttempts)++;
			// OK, here we go back one, check for OK destination...
			if ( (gTacticalStatus.uiFlags & IGNORE_ALL_OBSTACLES) || (NewOKDestination( pSoldier, sGridNo, TRUE, pSoldier->pathing.bLevel ) && FindBestPath( pSoldier, sGridNo, pSoldier->pathing.bLevel, WALKING, NO_COPYROUTE, PATH_THROUGH_PEOPLE )) )
			{
				return(sGridNo);
			}

			// If here, try another place!
			// ( alternate left/right )
			if ( bOdd2 )
			{
				bAdjustedDist++;

				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo - WORLD_COLS + 1);
				}
			}
			else
			{
				sGridNo = sStartGridNo;

				for ( cnt = 0; cnt < bAdjustedDist; cnt++ )
				{
					sGridNo = (sGridNo + WORLD_COLS - 1);
				}
			}

			bOdd2 = (INT8)(!bOdd2);

		}
		return NOWHERE;

	}

	//Unhandled exit
	*puiNumAttempts = 0;

	return(NOWHERE);
}


void GetLoadedSectorString( STR16 pString )
{
	if ( !gfWorldLoaded )
	{
		swprintf( pString, L"" );
		return;
	}
	if ( gbWorldSectorZ )
	{
		swprintf( pString, L"%c%d_b%d", gWorldSectorY + 'A' - 1, gWorldSectorX, gbWorldSectorZ );
	}
	else if ( !gbWorldSectorZ )
	{
		swprintf( pString, L"%c%d", gWorldSectorY + 'A' - 1, gWorldSectorX );
	}
}

#ifdef JA2UB
// no UB
#else
void HandleSlayDailyEvent( void )
{
	SOLDIERTYPE *pSoldier = NULL;

	// grab slay
	pSoldier = FindSoldierByProfileID( 64, TRUE );

	if ( pSoldier == NULL )
	{
		return;
	}

	// valid soldier?
	if ( (pSoldier->bActive == FALSE) || (pSoldier->stats.bLife == 0) || (pSoldier->bAssignment == IN_TRANSIT) || (pSoldier->bAssignment == ASSIGNMENT_POW) || (pSoldier->bAssignment == ASSIGNMENT_MINIEVENT) || (pSoldier->bAssignment == ASSIGNMENT_REBELCOMMAND) )
	{
		// no
		return;
	}

	// ATE: This function is used to check for the ultimate last day SLAY can stay for
	// he may decide to leave randomly while asleep...
	//if the user hasnt renewed yet, and is still leaving today
	if ( (pSoldier->iEndofContractTime / 1440) <= (INT32)GetWorldDay( ) )
	{
		pSoldier->ubLeaveHistoryCode = HISTORY_SLAY_MYSTERIOUSLY_LEFT;
		TacticalCharacterDialogueWithSpecialEvent( pSoldier, 0, DIALOGUE_SPECIAL_EVENT_CONTRACT_ENDING_NO_ASK_EQUIP, 0, 0 );
	}
}
#endif

BOOLEAN IsSectorDesert( INT16 sSectorX, INT16 sSectorY )
{
	// HEADROCK HAM 3.5: Fix for assertion errors associated with rain checks.
	if ( sSectorX <= 0 || sSectorY <= 0 || sSectorX > 16 || sSectorY > 16 )
	{
		return(FALSE);
	}

	// SANDRO - added more terrain types for heat intolerant feature
	if ( SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == SAND ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == SAND_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == SAND_SAM_SITE )
	{
		// desert
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

// SANDRO - added function
BOOLEAN IsSectorTropical( INT16 sSectorX, INT16 sSectorY )
{
	if ( SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == TROPICS ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == TROPICS_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == TROPICS_SAM_SITE )
	{
		return (TRUE);
	}

	return (FALSE);
}

BOOLEAN IsSectorFarm( INT16 sSectorX, INT16 sSectorY )
{
	if ( SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == FARMLAND ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == FARMLAND_ROAD )
	{
		return (TRUE);
	}

	return (FALSE);
}

BOOLEAN IsSectorRoad( INT16 sSectorX, INT16 sSectorY )
{
	if ( SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == PLAINS_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == SPARSE_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == FARMLAND_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == TROPICS_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == DENSE_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == HILLS_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == COASTAL_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == SAND_ROAD ||
		 SectorInfo[SECTOR( sSectorX, sSectorY )].ubTraversability[THROUGH_STRATEGIC_MOVE] == SWAMP_ROAD )
	{
		return (TRUE);
	}

	return (FALSE);
}

BOOLEAN HandleDefiniteUnloadingOfWorld( UINT8 ubUnloadCode )
{
	INT32 i;

	// clear tactical queue
	ClearEventQueue( );

	// ATE: End all bullets....
	DeleteAllBullets( );

	// End all physics objects...
	RemoveAllPhysicsObjects( );

	RemoveAllActiveTimedBombs( );

	// Flugente: remove any marker that an explosion is active but has not yet occured (to be used when leaving a sector)
	RemoveActiveExplosionMarkers( );

	// handle any quest stuff here so world items can be affected
	HandleQuestCodeOnSectorExit( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );

	//if we arent loading a saved game
	if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
	{
		//Clear any potential battle flags.  They will be set if necessary.
		gTacticalStatus.fEnemyInSector = FALSE;
		gTacticalStatus.uiFlags &= ~INCOMBAT;
	}

	// Flugente: update possible fortification potential in current sector
	UpdateFortificationPossibleAmount( );

	if ( ubUnloadCode == ABOUT_TO_LOAD_NEW_MAP )
	{
		//if we arent loading a saved game
		if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
		{
			// Flugente: cause all militia whose equipment is from this sector to drop it
			TeamDropAll( MILITIA_TEAM );

			// Save the current sectors Item list to a temporary file, if its not the first time in
			SaveCurrentSectorsInformationToTempItemFile( );

			// Update any mercs currently in sector, their profile info...
			UpdateSoldierPointerDataIntoProfile( FALSE );
		}
	}
	else if ( ubUnloadCode == ABOUT_TO_TRASH_WORLD )
	{
		//if we arent loading a saved game
		if ( !(gTacticalStatus.uiFlags & LOADING_SAVED_GAME) )
		{
			// Flugente: cause all militia whose equipment is from this sector to drop it
			TeamDropAll( MILITIA_TEAM );
		}

		//Save the current sectors open temp files to the disk
		if ( !SaveCurrentSectorsInformationToTempItemFile( ) )
		{
			ScreenMsg( FONT_MCOLOR_WHITE, MSG_TESTVERSION, L"ERROR in SaveCurrentSectorsInformationToTempItemFile()" );
			return FALSE;
		}

		//Setup the tactical existance of the current soldier.
		//@@@Evaluate
		for ( i = gTacticalStatus.Team[CIV_TEAM].bFirstID; i <= gTacticalStatus.Team[CIV_TEAM].bLastID; i++ )
		{
			if ( MercPtrs[i]->bActive && MercPtrs[i]->bInSector )
			{
				SetupProfileInsertionDataForSoldier( MercPtrs[i] );
			}
		}

		gfBlitBattleSectorLocator = FALSE;
	}

	//Handle cases for both types of unloading
	HandleMilitiaStatusInCurrentMapBeforeLoadingNewMap( );
	return TRUE;
}

BOOLEAN HandlePotentialBringUpAutoresolveToFinishBattle( int pSectorX, int pSectorY, int pSectorZ )
{
	//We don't have mercs in the sector.  Now, we check to see if there are BOTH enemies and militia.  If both
	//co-exist in the sector, then make them fight for control of the sector via autoresolve.
	for ( int i = gTacticalStatus.Team[ENEMY_TEAM].bFirstID; i <= gTacticalStatus.Team[CREATURE_TEAM].bLastID; i++ )
	{
		if ( MercPtrs[i]->bActive && MercPtrs[i]->stats.bLife )
		{
			if ( MercPtrs[i]->sSectorX == pSectorX &&
				 MercPtrs[i]->sSectorY == pSectorY &&
				 MercPtrs[i]->bSectorZ == pSectorZ )
			{ //We have enemies, now look for militia!
				for ( i = gTacticalStatus.Team[MILITIA_TEAM].bFirstID; i <= gTacticalStatus.Team[MILITIA_TEAM].bLastID; i++ )
				{
					if ( MercPtrs[i]->bActive && MercPtrs[i]->stats.bLife && MercPtrs[i]->bSide == OUR_TEAM )
					{
						if ( MercPtrs[i]->sSectorX == pSectorX &&
							 MercPtrs[i]->sSectorY == pSectorY &&
							 MercPtrs[i]->bSectorZ == pSectorZ )
						{ //We have militia and enemies and no mercs!  Let's finish this battle in autoresolve.
							gfEnteringMapScreen = TRUE;
							gfEnteringMapScreenToEnterPreBattleInterface = TRUE;
							gfAutomaticallyStartAutoResolve = TRUE;
							gfUsePersistantPBI = FALSE;
							gubPBSectorX = (UINT8)pSectorX;
							gubPBSectorY = (UINT8)pSectorY;
							gubPBSectorZ = (UINT8)pSectorZ;
							gfBlitBattleSectorLocator = TRUE;
							gfTransferTacticalOppositionToAutoResolve = TRUE;
							if ( GetEnemyEncounterCode() != CREATURE_ATTACK_CODE && 
								GetEnemyEncounterCode() != BLOODCAT_ATTACK_CODE && 
								GetEnemyEncounterCode() != ZOMBIE_ATTACK_CODE &&
								GetEnemyEncounterCode() != BANDIT_ATTACK_CODE )
							{
								SetEnemyEncounterCode( ENEMY_INVASION_CODE ); //has to be, if militia are here.
							}
							else
							{
								//DoScreenIndependantMessageBox( gzLateLocalizedString[ 39 ], MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
							}

							return(TRUE);
						}
					}
				}
			}
		}
	}

	return(FALSE);
}


BOOLEAN CheckAndHandleUnloadingOfCurrentWorld( )
{
	INT32 i;
	INT16 sBattleSectorX, sBattleSectorY, sBattleSectorZ;

	//Don't bother checking this if we don't have a world loaded.
	if ( !gfWorldLoaded )
	{
		return FALSE;
	}

	if ( is_client )//hayden - if multiplayer dont kick from level, allow spectate.
	{
		return FALSE;
	}

	if ( gTacticalStatus.fDidGameJustStart && gWorldSectorX == gGameExternalOptions.ubDefaultArrivalSectorX && gWorldSectorY == gGameExternalOptions.ubDefaultArrivalSectorY && !gbWorldSectorZ )
	{
		return FALSE;
	}

	GetCurrentBattleSectorXYZ( &sBattleSectorX, &sBattleSectorY, &sBattleSectorZ );

	if ( guiCurrentScreen == AUTORESOLVE_SCREEN )
	{ //The user has decided to let the game autoresolve the current battle.
		if ( gWorldSectorX == sBattleSectorX && gWorldSectorY == sBattleSectorY && gbWorldSectorZ == sBattleSectorZ )
		{
			for ( i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; i++ )
			{ //If we have a live and valid soldier
				if ( MercPtrs[i]->bActive && MercPtrs[i]->stats.bLife && !MercPtrs[i]->flags.fBetweenSectors && !(MercPtrs[i]->flags.uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( MercPtrs[i] ) && !AM_AN_EPC( MercPtrs[i] ) )
				{
					if ( MercPtrs[i]->sSectorX == gWorldSectorX &&
						 MercPtrs[i]->sSectorY == gWorldSectorY &&
						 MercPtrs[i]->bSectorZ == gbWorldSectorZ )
					{
						MercPtrs[i]->RemoveSoldierFromGridNo( );
						InitSoldierOppList( MercPtrs[i] );
					}
				}
			}
		}
	}
	else
	{	//Check and see if we have any live mercs in the sector.
		for ( i = gTacticalStatus.Team[OUR_TEAM].bFirstID; i <= gTacticalStatus.Team[OUR_TEAM].bLastID; i++ )
		{ //If we have a live and valid soldier
			if ( MercPtrs[i]->bActive && MercPtrs[i]->stats.bLife && !MercPtrs[i]->flags.fBetweenSectors && !(MercPtrs[i]->flags.uiStatusFlags & SOLDIER_VEHICLE) && !AM_A_ROBOT( MercPtrs[i] ) && !AM_AN_EPC( MercPtrs[i] ) )
			{
				if ( MercPtrs[i]->sSectorX == gWorldSectorX &&
					 MercPtrs[i]->sSectorY == gWorldSectorY &&
					 MercPtrs[i]->bSectorZ == gbWorldSectorZ )
				{
					return FALSE;
				}
			}
		}
		//KM : August 6, 1999 Patch fix
		//     Added logic to prevent a crash when player mercs would retreat from a battle involving militia and enemies.
		//		 Without the return here, it would proceed to trash the world, and then when autoresolve would come up to
		//     finish the tactical battle, it would fail to find the existing soldier information (because it was trashed).
		if ( HandlePotentialBringUpAutoresolveToFinishBattle( sBattleSectorX, sBattleSectorY, sBattleSectorZ ) )
		{
			// Flugente: cause all militia whose equipment is from this sector to drop it - otherwise it can get lost
			TeamDropAll( MILITIA_TEAM, TRUE );

			// Save the current sectors Item list to a temporary file, if its not the first time in
			SaveCurrentSectorsInformationToTempItemFile( );

			return FALSE;
		}
		//end

		//HandlePotentialBringUpAutoresolveToFinishBattle( ); //prior patch logic
	}


	CheckForEndOfCombatMode( FALSE );
	EndTacticalBattleForEnemy( );

	// ATE: Change cursor to wait cursor for duration of frame.....
	// save old cursor ID....
	SetCurrentCursorFromDatabase( CURSOR_WAIT_NODELAY );
	RefreshScreen( NULL );

	// JA2Gold: Leaving sector, so get rid of ambients!
	DeleteAllAmbients( );

	if ( guiCurrentScreen == GAME_SCREEN )
	{
		if ( !gfTacticalTraversal )
		{ //if we are in tactical and don't intend on going to another sector immediately, then
			gfEnteringMapScreen = TRUE;
		}
		else
		{ //The trashing of the world will be handled automatically.
			return FALSE;
		}
	}

	//We have passed all the checks and can Trash the world.
	if ( !HandleDefiniteUnloadingOfWorld( ABOUT_TO_TRASH_WORLD ) )
	{
		return FALSE;
	}

	if ( guiCurrentScreen == AUTORESOLVE_SCREEN )
	{
		if ( gWorldSectorX == sBattleSectorX && gWorldSectorY == sBattleSectorY && gbWorldSectorZ == sBattleSectorZ )
		{	//Yes, this is and looks like a hack.  The conditions of this if statement doesn't work inside
			//TrashWorld() or more specifically, TacticalRemoveSoldier() from within TrashWorld().  Because
			//we are in the autoresolve screen, soldiers are internally created different (from pointers instead of
			//the MercPtrs[]).  It keys on the fact that we are in the autoresolve screen.  So, by switching the
			//screen, it'll delete the soldiers in the loaded world properly, then later on, once autoresolve is
			//complete, it'll delete the autoresolve soldiers properly.  As you can now see, the above if conditions
			//don't change throughout this whole process which makes it necessary to do it this way.
			guiCurrentScreen = MAP_SCREEN;
			TrashWorld( );
			guiCurrentScreen = AUTORESOLVE_SCREEN;
		}
	}
	else
	{
		TrashWorld( );
	}

	//Clear all combat related flags.
	gTacticalStatus.fEnemyInSector = FALSE;
	gTacticalStatus.uiFlags &= ~INCOMBAT;
	EndTopMessage( );


	//Clear the world sector values.
	gWorldSectorX = gWorldSectorY = 0;
	gbWorldSectorZ = -1;

	//Clear the flags regarding.
	gfCaves = FALSE;
	gfBasement = FALSE;

	//CHRISL: If we're unloading the world, shouldn't be reset the gfWorldLoaded flag?
	gfWorldLoaded = FALSE;

	return TRUE;
}



//This is called just before the world is unloaded to preserve location information for RPCs and NPCs either in
//the sector or strategically in the sector (such as firing an NPC in a sector that isn't yet loaded.)  When loading that
//sector, the RPC would be added.
//@@@Evaluate
void SetupProfileInsertionDataForSoldier( SOLDIERTYPE *pSoldier )
{
	if ( !pSoldier || pSoldier->ubProfile == NO_PROFILE )
	{ //Doesn't have profile information.
		return;
	}

	if ( gMercProfiles[pSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_PERMANENT_INSERTION_CODE )
		//if ( gMercProfiles[ pSoldier->ubProfile ].ubStrategicInsertionCode == INSERTION_CODE_PERMANENT_GRIDNO )
	{
		// can't be changed!
		return;
	}

	if ( gfWorldLoaded && pSoldier->bActive && pSoldier->bInSector )
	{ //This soldier is currently in the sector

		//@@@Evaluate -- insert code here
		//SAMPLE CODE:  There are multiple situations that I didn't code.  The gridno should be the final destination
		//or reset???

		if ( pSoldier->ubQuoteRecord && pSoldier->ubQuoteActionID )
		{
			// if moving to traverse
			if ( pSoldier->ubQuoteActionID >= QUOTE_ACTION_ID_TRAVERSE_EAST && pSoldier->ubQuoteActionID <= QUOTE_ACTION_ID_TRAVERSE_NORTH )
			{
				// Handle traversal.  This NPC's sector will NOT already be set correctly, so we have to call for that too
				HandleNPCChangesForTacticalTraversal( pSoldier );
				gMercProfiles[pSoldier->ubProfile].fUseProfileInsertionInfo = FALSE;
				if ( pSoldier->ubProfile != NO_PROFILE && NPCHasUnusedRecordWithGivenApproach( pSoldier->ubProfile, APPROACH_DONE_TRAVERSAL ) )
				{
					gMercProfiles[pSoldier->ubProfile].ubMiscFlags3 |= PROFILE_MISC_FLAG3_HANDLE_DONE_TRAVERSAL;
				}

			}
			else
			{
				if ( pSoldier->pathing.sFinalDestination == pSoldier->sGridNo )
				{
					gMercProfiles[pSoldier->ubProfile].usStrategicInsertionData = pSoldier->sGridNo;
				}
				else if ( !TileIsOutOfBounds( pSoldier->sAbsoluteFinalDestination ) )
				{
					gMercProfiles[pSoldier->ubProfile].usStrategicInsertionData = pSoldier->sAbsoluteFinalDestination;
				}
				else
				{
					gMercProfiles[pSoldier->ubProfile].usStrategicInsertionData = pSoldier->pathing.sFinalDestination;
				}

				gMercProfiles[pSoldier->ubProfile].fUseProfileInsertionInfo = TRUE;
				gMercProfiles[pSoldier->ubProfile].ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				gMercProfiles[pSoldier->ubProfile].ubQuoteActionID = pSoldier->ubQuoteActionID;
				gMercProfiles[pSoldier->ubProfile].ubQuoteRecord = pSoldier->ubQuoteActionID;
			}
		}
		else
		{
			gMercProfiles[pSoldier->ubProfile].fUseProfileInsertionInfo = FALSE;
		}

	}
	else
	{ //use strategic information

		//It appears to set the soldier's strategic insertion code everytime a group arrives in a new sector.  The insertion data
		//isn't needed for these cases as the code is a direction only.
		gMercProfiles[pSoldier->ubProfile].ubStrategicInsertionCode = pSoldier->ubStrategicInsertionCode;
		gMercProfiles[pSoldier->ubProfile].usStrategicInsertionData = 0;

		//Strategic system should now work.
		gMercProfiles[pSoldier->ubProfile].fUseProfileInsertionInfo = TRUE;
	}
}


void HandlePotentialMoraleHitForSkimmingSectors( GROUP *pGroup )
{
	//PLAYERGROUP *pPlayer;

	if ( !gTacticalStatus.fHasEnteredCombatModeSinceEntering && gTacticalStatus.fEnemyInSector )
	{
		//Flag is set so if "wilderness" enemies are in the adjacent sector of this group, the group has
		//a 90% chance of ambush.  Because this typically doesn't happen very often, the chance is high.
		//This reflects the enemies radioing ahead to other enemies of the group's arrival, so they have
		//time to setup a good ambush!
		pGroup->uiFlags |= GROUPFLAG_HIGH_POTENTIAL_FOR_AMBUSH;

		//SANDRO - WTF?!?
		//pPlayer = pGroup->pPlayerList;

		//while( pPlayer )
		//{
		//   // Do morale hit...
		//   // CC look here!
		//   // pPlayer->pSoldier

		// pPlayer = pPlayer->next;
		//}
	}
}



#ifdef CRIPPLED_VERSION
void CrippledVersionFailureToLoadMapCheck( )
{
	CHAR16	zString[512];

	swprintf( zString, L"Error! Sorry, you must stay between sectors A and E in this limited press version." );

	DoScreenIndependantMessageBox( zString, MSG_BOX_FLAG_OK, CrippledVersionFailureToLoadMapCallBack );
}

void CrippledVersionFailureToLoadMapCallBack( UINT8 bExitValue )
{
	//clean up the code
	ReStartingGame( );

	//go to the main menu
	if ( guiCurrentScreen == MAP_SCREEN )
	{
		SetPendingNewScreen( MAINMENU_SCREEN );
	}
	else
	{
		InternalLeaveTacticalScreen( MAINMENU_SCREEN );
	}
}
#endif
BOOLEAN EscapeDirectionIsValid( INT8 * pbDirection )
{
	UINT8 ubSectorID = SECTOR( gWorldSectorX, gWorldSectorY );

	switch ( *pbDirection )
	{
	case NORTHEAST: // east
		if ( gWorldSectorX + 1 > MAXIMUM_VALID_X_COORDINATE || gMapInformation.sEastGridNo == NOWHERE ||
			 SectorInfo[ubSectorID].ubTraversability[EAST_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[EAST_STRATEGIC_MOVE] == EDGEOFWORLD )
			 *pbDirection = -1;
		break;
	case SOUTHEAST: // south
		if ( gWorldSectorY + 1 > MAXIMUM_VALID_Y_COORDINATE || gMapInformation.sSouthGridNo == NOWHERE ||
			 SectorInfo[ubSectorID].ubTraversability[SOUTH_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[SOUTH_STRATEGIC_MOVE] == EDGEOFWORLD )
			 *pbDirection = -1;
		break;
	case SOUTHWEST: // west
		if ( gWorldSectorX - 1 < MINIMUM_VALID_X_COORDINATE || gMapInformation.sWestGridNo == NOWHERE ||
			 SectorInfo[ubSectorID].ubTraversability[WEST_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[WEST_STRATEGIC_MOVE] == EDGEOFWORLD )
			 *pbDirection = -1;
		break;
	case NORTHWEST: // north
		if ( gWorldSectorY - 1 < MINIMUM_VALID_Y_COORDINATE || gMapInformation.sNorthGridNo == NOWHERE ||
			 SectorInfo[ubSectorID].ubTraversability[NORTH_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[NORTH_STRATEGIC_MOVE] == EDGEOFWORLD )
			 *pbDirection = -1;
		break;
	default: *pbDirection = -1;
	}
	return(*pbDirection != -1);
}

bool IsEscapeDirectionValid(WorldDirections pbDirection)
{
	bool isValid = false;
	UINT8 const ubSectorID = SECTOR(gWorldSectorX, gWorldSectorY);

	switch (pbDirection)
	{
	case NORTH:
		if (!(gWorldSectorY - 1 < MINIMUM_VALID_Y_COORDINATE || gMapInformation.sNorthGridNo == NOWHERE ||
			SectorInfo[ubSectorID].ubTraversability[NORTH_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[NORTH_STRATEGIC_MOVE] == EDGEOFWORLD))
		{
			isValid = true;
		}
		break;
	case EAST:
		if (!(gWorldSectorX + 1 > MAXIMUM_VALID_X_COORDINATE || gMapInformation.sEastGridNo == NOWHERE ||
			SectorInfo[ubSectorID].ubTraversability[EAST_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[EAST_STRATEGIC_MOVE] == EDGEOFWORLD))
		{
			isValid = true;
		}
		break;
	case SOUTH:
		if (!(gWorldSectorY + 1 > MAXIMUM_VALID_Y_COORDINATE || gMapInformation.sSouthGridNo == NOWHERE ||
			SectorInfo[ubSectorID].ubTraversability[SOUTH_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[SOUTH_STRATEGIC_MOVE] == EDGEOFWORLD))
		{
			isValid = true;
		}
		break;
	case WEST:
		if (!(gWorldSectorX - 1 < MINIMUM_VALID_X_COORDINATE || gMapInformation.sWestGridNo == NOWHERE ||
			SectorInfo[ubSectorID].ubTraversability[WEST_STRATEGIC_MOVE] == GROUNDBARRIER || SectorInfo[ubSectorID].ubTraversability[WEST_STRATEGIC_MOVE] == EDGEOFWORLD))
		{
			isValid = true;
		}
		break;
	}

	return isValid;
}

#ifdef JA2UB


//------------ub

typedef struct
{
	INT16	sSectorID;
	INT8	bSectorZ;
	UINT8	ubQuoteNum;
} ENTER_SECTOR_PLAYER_QUOTE;

void HandlePlayerTeamQuotesWhenEnteringSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ )
{
	UINT32		uiCnt;
	UINT8		usNumValidMercs = 0;
	INT32		iSectorID = SECTOR( sSectorX, sSectorY );
	UINT8		ubValidMercProfileIDArray[NUM_MERCS_WITH_NEW_QUOTES];
	SOLDIERTYPE *pSoldier = NULL;
	
#define	NUM_VALID_SECTORS									6
#define	DELAY_FOR_PLAYER_DESC_OF_SECTOR		2
	
	ENTER_SECTOR_PLAYER_QUOTE	PlayerSectorDescQuote[ NUM_VALID_SECTORS ] = 
	{
	    //SEC_H9-0
		{ SECTOR( gGameUBOptions.SectorGuardPostX, gGameUBOptions.SectorGuardPostY ),		gGameUBOptions.SectorGuardPostZ,	QUOTE_HATED_1_ON_TEAM },
		
		//SEC_I9-0
		{ SECTOR( gGameUBOptions.I9SectorPlayerQuoteX, gGameUBOptions.I9SectorPlayerQuoteY ),		gGameUBOptions.I9SectorPlayerQuoteZ,	QUOTE_LEARNED_TO_HATE_MERC_ON_TEAM },
		
		//SEC_H10-0
		{ SECTOR( gGameUBOptions.H10SectorPlayerQuoteX, gGameUBOptions.H10SectorPlayerQuoteY ),	gGameUBOptions.H10SectorPlayerQuoteZ,	QUOTE_LEARNED_TO_HATE_MERC_ON_TEAM },
		
		//SEC_I10-0
		{ SECTOR( gGameUBOptions.FristSectorTownX, gGameUBOptions.FristSectorTownY ),	gGameUBOptions.FristSectorTownZ,	QUOTE_HATED_2_ON_TEAM },
		
		//SEC_J13-0
		{ SECTOR( gGameUBOptions.SectorFanX, gGameUBOptions.SectorFanY ),	gGameUBOptions.SectorFanZ,	QUOTE_ENTER_SECTOR_WITH_FAN_1 },
		
		//SEC_J14-1
		{ SECTOR( gGameUBOptions.SectorGuardPostX, gGameUBOptions.SectorGuardPostY ),	gGameUBOptions.ExitForFanToPowerGenSectorZ,	0 },
	};

	//loop through all the sectors that have the quotes
	for ( uiCnt = 0; uiCnt<NUM_VALID_SECTORS; ++uiCnt )
	{
		//if this sector is the right x & y
		if ( PlayerSectorDescQuote[uiCnt].sSectorID == iSectorID )
		{
			//if is the right level
			if ( PlayerSectorDescQuote[uiCnt].bSectorZ == sSectorZ )
			{
				// If the player has never said the "enter sector" desc before
				if ( GetSectorFlagStatus( sSectorX, sSectorY, (UINT8)sSectorZ, SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR ) != TRUE )
				{
					//Get # of valid mercs
					usNumValidMercs = GetNumSoldierIdAndProfileIdOfTheNewMercsOnPlayerTeam( NULL, ubValidMercProfileIDArray );

					//if there isnt any valid mercs
					if ( usNumValidMercs == 0 )
					{
						//exit
						return;
					}

					//Switch on the town ID
					//I10
					if ( iSectorID == SECTOR( gGameUBOptions.FristSectorTownX, gGameUBOptions.FristSectorTownY ) )
					{
						UINT8	cnt;
	
						for( cnt=0; cnt<usNumValidMercs; ++cnt )
						{
							//Say the qoute in a couple of seconds
							DelayedMercQuote( ubValidMercProfileIDArray[ cnt ], PlayerSectorDescQuote[ uiCnt ].ubQuoteNum, GetWorldTotalSeconds() + DELAY_FOR_PLAYER_DESC_OF_SECTOR + cnt );
						}
					}
					//J13
					else if ( iSectorID == SECTOR( gGameUBOptions.SectorFanX, gGameUBOptions.SectorFanY ) )
					{
						if( PlayerSectorDescQuote[ uiCnt ].bSectorZ == gGameUBOptions.SectorFanZ )
						{
							//See if Manuel is on the team
							pSoldier = FindSoldierByProfileID( MANUEL_UB, TRUE );

							//if he is ON the team
							if( pSoldier != NULL )
							{
								//get manuel to say the quote
								DelayedMercQuote( MANUEL_UB, PlayerSectorDescQuote[ uiCnt ].ubQuoteNum, GetWorldTotalSeconds() + DELAY_FOR_PLAYER_DESC_OF_SECTOR );

								//get manuel to say his additional quote
								DelayedMercQuote( MANUEL_UB, QUOTE_ENTER_SECTOR_WITH_FAN_2, GetWorldTotalSeconds() + DELAY_FOR_PLAYER_DESC_OF_SECTOR );
							}
							else
							{
								//else if Biggens is not on the team, check for biggens
								pSoldier = FindSoldierByProfileID( BIGGENS_UB, TRUE );

								//if he is ON the team
								if( pSoldier != NULL )
								{
									//get manuel to say the quote
									DelayedMercQuote( BIGGENS_UB, PlayerSectorDescQuote[ uiCnt ].ubQuoteNum, GetWorldTotalSeconds() + DELAY_FOR_PLAYER_DESC_OF_SECTOR );

									//Remember he said
									SetJa25GeneralFlag( JA_GF__BIGGENS_SAID_QUOTE_117 );
								}
							}

							//Remeber the quote was said
							SetFactTrue( FACT_PLAYER_KNOWS_ABOUT_FAN_STOPPING );
						}	
					}
					//J14
					else if ( iSectorID == SECTOR( gGameUBOptions.ExitForFanToPowerGenSectorX, gGameUBOptions.ExitForFanToPowerGenSectorY ) )
					{
					  //first underground sector
					  HandlePlayerQuotesWhenEnteringFirstTunnelSector();
					}
					else
					{
						//Get a random merc from the list
						INT16	sMercToSayQuote = RandomProfileIdFromNewMercsOnPlayerTeam();

						//Say the qoute in a couple of seconds
						if ( sMercToSayQuote > -1 )
							DelayedMercQuote( (UINT16)sMercToSayQuote, PlayerSectorDescQuote[ uiCnt ].ubQuoteNum, GetWorldTotalSeconds() + DELAY_FOR_PLAYER_DESC_OF_SECTOR );							
					}
/*
					switch ( iSectorID )
					{
					case SEC_I10:
					{
									UINT8	cnt;

									for ( cnt = 0; cnt<bNumValidMercs; cnt++ )
									{
										//Say the qoute in a couple of seconds
										DelayedMercQuote( ubValidMercProfileIDArray[cnt], PlayerSectorDescQuote[uiCnt].ubQuoteNum, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR + cnt );
									}
					}
						break;

					case SEC_J13:
						if ( PlayerSectorDescQuote[uiCnt].bSectorZ == 0 )
						{
							//See if Manuel is on the team
							pSoldier = FindSoldierByProfileID( MANUEL_UB, TRUE );

							//if he is ON the team
							if ( pSoldier != NULL )
							{
								//get manuel to say the quote
								DelayedMercQuote( MANUEL_UB, PlayerSectorDescQuote[uiCnt].ubQuoteNum, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR );

								//get manuel to say his additional quote
								DelayedMercQuote( MANUEL_UB, QUOTE_ENTER_SECTOR_WITH_FAN_2, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR );
							}
							else
							{
								//else if Biggens is not on the team, check for biggens
								pSoldier = FindSoldierByProfileID( BIGGENS_UB, TRUE );

								//if he is ON the team
								if ( pSoldier != NULL )
								{
									//get manuel to say the quote
									DelayedMercQuote( BIGGENS_UB, PlayerSectorDescQuote[uiCnt].ubQuoteNum, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR );

									//Remember he said
									SetJa25GeneralFlag( JA_GF__BIGGENS_SAID_QUOTE_117 );
								}
							}

							//Remeber the quote was said
							SetFactTrue( FACT_PLAYER_KNOWS_ABOUT_FAN_STOPPING );
						}
						break;
					case SEC_J14:
					{
									//first underground sector
									HandlePlayerQuotesWhenEnteringFirstTunnelSector( );
					}
						break;
					default:
					{
							   //Get a random merc from the list
							   UINT8	ubMercToSayQuote = RandomProfileIdFromNewMercsOnPlayerTeam( );

							   //Say the qoute in a couple of seconds
							   DelayedMercQuote( ubMercToSayQuote, PlayerSectorDescQuote[uiCnt].ubQuoteNum, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR );
					}
						break;
					}

*/
					//if this sector is one of the fields before the town, only say the quote once for both sectors
					//I9 or H10
					if( ( iSectorID ==  SECTOR( gGameUBOptions.I9SectorPlayerQuoteX, gGameUBOptions.I9SectorPlayerQuoteY ) || iSectorID ==  SECTOR( gGameUBOptions.H10SectorPlayerQuoteX, gGameUBOptions.H10SectorPlayerQuoteY ) ) && sSectorZ == 0 )
					{
						//Remeber that we said the quote
						//SetSectorFlag( 9, 9, 0, SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR );
						//SetSectorFlag( 10, 8, 0, SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR );
						SetSectorFlag( gGameUBOptions.I9SectorPlayerQuoteX, gGameUBOptions.I9SectorPlayerQuoteY, gGameUBOptions.I9SectorPlayerQuoteZ, SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR );
						SetSectorFlag( gGameUBOptions.H10SectorPlayerQuoteX, gGameUBOptions.H10SectorPlayerQuoteY, gGameUBOptions.H10SectorPlayerQuoteZ, SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR );	
					}

					//default
					else
					{
						//Remeber that we said the quote
						SetSectorFlag( sSectorX, sSectorY, (UINT8)sSectorZ, SF_HAVE_SAID_PLAYER_QUOTE_NEW_SECTOR );
					}
				}

				//we are done here, leave
				return;
			}
		}
	}
}

void HandlePlayerQuotesWhenEnteringFirstTunnelSector( )
{
	//if the player got through using the timed method
	if ( gJa25SaveStruct.ubHowPlayerGotThroughFan == PG__PLAYER_STOPPED_FAN_TO_GET_THROUGH )
	{
		UINT8 ubNumPlayersInSector = NumActiveAndConsciousTeamMembers( OUR_TEAM );
		UINT8 ubNumPlayersOnTeam = NumberOfMercsOnPlayerTeam( );

		//Get a random qualified merc to say the quote
		INT16 bID = RandomSoldierIdFromNewMercsOnPlayerTeam( );

		//if there is no valid merc
		if ( bID == -1 )
			return;

		//if not all the players made it through
		if ( ubNumPlayersInSector < ubNumPlayersOnTeam )
		{
			//			TacticalCharacterDialogue( &Menptr[ ubID ], QUOTE_CONTRACTS_OVER );
			DelayedMercQuote( Menptr[bID].ubProfile, QUOTE_CONTRACTS_OVER, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR );
		}

		//if ALL mercs made it through
		else if ( ubNumPlayersInSector == ubNumPlayersOnTeam )
		{
			DelayedMercQuote( Menptr[bID].ubProfile, QUOTE_CONTRACT_ACCEPTANCE, GetWorldTotalSeconds( ) + DELAY_FOR_PLAYER_DESC_OF_SECTOR );
			//			TacticalCharacterDialogue( &Menptr[ ubID ], QUOTE_CONTRACT_ACCEPTANCE );
		}
	}
	/*else if ( gJa25SaveStruct.ubHowPlayerGotThroughFan == PG__PLAYER_BLEW_UP_FAN_TO_GET_THROUGH )
	{
		Chnaged to be played when fan blows up
		//Get a random qualified merc to say the quote
		ubID = RandomSoldierIdFromNewMercsOnPlayerTeam();

		DelayedMercQuote( Menptr[ ubID ].ubProfile, QUOTE_ACCEPT_CONTRACT_RENEWAL, GetWorldTotalSeconds() + DELAY_FOR_PLAYER_DESC_OF_SECTOR );
	}*/
	else
	{
		Assert( 0 );
	}
}

void HandleEmailBeingSentWhenEnteringSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fLaptopJustGotFixed )
{
	SOLDIERTYPE *pSoldier = NULL;

	//
	// if this sector is a sector we are to send an email to the player from
	//

	//if the laptop transmiter is not working yet
	if ( gubQuest[QUEST_FIX_LAPTOP] != QUESTDONE && !fLaptopJustGotFixed && gGameUBOptions.LaptopQuestEnabled == TRUE )
	{
		//we will send these emails later
		return;
	}

	//if miguel is alive
	if ( gubFact[FACT_PLAYER_IMPORTED_SAVE_MIGUEL_DEAD] == FALSE )
	{
		//if its either J11 or I12 ( or we just got the email back up and we have been to the sector
		if( ( ( sMapY == gGameUBOptions.SectorTown2Y && sMapX == gGameUBOptions.SectorTown2X ) || ( sMapY == 9 && sMapX == 12 ) && bMapZ == 0 ) ||
			fLaptopJustGotFixed && 
			( GetSectorFlagStatus( gGameUBOptions.SectorTown2X, gGameUBOptions.SectorTown2Y, gGameUBOptions.SectorTown2Z, SF_HAS_ENTERED_TACTICAL ) == TRUE || GetSectorFlagStatus( 12, 9, 0, SF_HAS_ENTERED_TACTICAL ) == TRUE ) )
		{
			//and we havent sent it before
			if ( !(gJa25SaveStruct.ubEmailFromSectorFlag & SECTOR_EMAIL__J11_J12) )
			{
				pSoldier = FindSoldierByProfileID( MANUEL_UB, TRUE ); //MANUEL

				//if Manuel isnt on the team
				if ( pSoldier == NULL || gMercProfiles[MANUEL_UB].bMercStatus == MERC_IS_DEAD ) //MANUEL
				{
					//email 8a
					AddEmail( EMAIL_MIGUELSORRY, EMAIL_MIGUELSORRY_LENGTH, MAIL_MIGUEL, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
				}
				else
				{
					//email 8b
					AddEmail( EMAIL_MIGUELMANUEL, EMAIL_MIGUELMANUEL_LENGTH, MAIL_MIGUEL, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
				}

				//Remeber we sent it
				gJa25SaveStruct.ubEmailFromSectorFlag |= SECTOR_EMAIL__J11_J12;
			}
		}

		//if its the power generator sector J13
		if( sMapY == gGameUBOptions.SectorFanY && sMapX == gGameUBOptions.SectorFanX && bMapZ == gGameUBOptions.SectorFanZ ||
			fLaptopJustGotFixed && GetSectorFlagStatus( gGameUBOptions.SectorFanX, gGameUBOptions.SectorFanY, gGameUBOptions.SectorFanZ, SF_HAS_ENTERED_TACTICAL ) == TRUE )
		{
			//and we havent sent it before
			if ( !(gJa25SaveStruct.ubEmailFromSectorFlag & SECTOR_EMAIL__POWER_GEN) )
			{
				AddEmail( EMAIL_MIGUELSICK, EMAIL_MIGUELSICK_LENGTH, MAIL_MIGUEL, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );

				//Remeber we sent it
				gJa25SaveStruct.ubEmailFromSectorFlag |= SECTOR_EMAIL__POWER_GEN;
			}
		}
	}


	//if its the tunnel sector J14-1
	if( sMapY == gGameUBOptions.ExitForFanToPowerGenSectorY && sMapX == gGameUBOptions.ExitForFanToPowerGenSectorX && bMapZ == gGameUBOptions.ExitForFanToPowerGenSectorZ ||
		fLaptopJustGotFixed && GetSectorFlagStatus( gGameUBOptions.ExitForFanToPowerGenSectorX, gGameUBOptions.ExitForFanToPowerGenSectorY, gGameUBOptions.ExitForFanToPowerGenSectorZ, SF_HAS_ENTERED_TACTICAL ) == TRUE	)
	{
		//and we havent sent it before
		if ( !(gJa25SaveStruct.ubEmailFromSectorFlag & SECTOR_EMAIL__TUNNEL) )
		{
			//If Jerry isnt dead
			if ( gMercProfiles[JERRY_MILO_UB].bMercStatus != MERC_IS_DEAD ) //JERRY
			{
				AddEmail( EMAIL_PILOTFOUND, EMAIL_PILOTFOUND_LENGTH, MAIL_ENRICO, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
			}

			//Remeber we sent it
			gJa25SaveStruct.ubEmailFromSectorFlag |= SECTOR_EMAIL__TUNNEL;
		}
	}
}



void ShouldNpcBeAddedToSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	//if Manuel has never been added before
	if ( !(gJa25SaveStruct.fNpcHasBeenAdded & SECTOR_ADDED_NPC__MANUEL) )
	{
		//if it is the right sector H10 or I9
		if( ( sMapY == gGameUBOptions.H10SectorPlayerQuoteY && sMapX == gGameUBOptions.H10SectorPlayerQuoteX && bMapZ == gGameUBOptions.H10SectorPlayerQuoteZ ) ||
				( sMapY == gGameUBOptions.I9SectorPlayerQuoteY && sMapX == gGameUBOptions.I9SectorPlayerQuoteX && bMapZ == gGameUBOptions.I9SectorPlayerQuoteZ ) )
		{
			//Change his sector values to 
			gMercProfiles[MANUEL_UB].sSectorX = sMapX;
			gMercProfiles[MANUEL_UB].sSectorY = sMapY;
			gMercProfiles[MANUEL_UB].bSectorZ = bMapZ;

			//remember that we have added him
			gJa25SaveStruct.fNpcHasBeenAdded |= SECTOR_ADDED_NPC__MANUEL;
		}
	}

	//if Tex has never been added before 
	if ( !(gJa25SaveStruct.fNpcHasBeenAdded & SECTOR_ADDED_NPC__TEX) )
	{
		//and tex is TO be added
		if ( gubFact[FACT_TEX_IS_IN_GAME_AND_ALIVE_IN_STORE] )
		{
			//if it is the right sector
			if ( sMapY == MAP_ROW_I && sMapX == 10 && bMapZ == 0 )
			{
				//Change his sector values to 
				gMercProfiles[TEX_UB].sSectorX = sMapX;
				gMercProfiles[TEX_UB].sSectorY = sMapY;
				gMercProfiles[TEX_UB].bSectorZ = bMapZ;

				//remember that we have added him
				gJa25SaveStruct.fNpcHasBeenAdded |= SECTOR_ADDED_NPC__TEX;
			}
		}
	}

	//if John has never been added before 
	if ( !(gJa25SaveStruct.fNpcHasBeenAdded & SECTOR_ADDED_NPC__JOHN_K) )
	{
		//and John is TO be added ( Tex not in game )
		//if( !gubFact[ FACT_TEX_IS_IN_GAME_AND_ALIVE_IN_STORE ] )
		//	{
		//if it is the right sector
		if ( sMapX == gJa25SaveStruct.ubJohnKulbaInitialSectorX &&
			 sMapY == gJa25SaveStruct.ubJohnKulbaInitialSectorY &&
			 bMapZ == 0 )
		{
			//Change his sector values to 
			gMercProfiles[JOHN_K_UB].sSectorX = sMapX;
			gMercProfiles[JOHN_K_UB].sSectorY = sMapY;
			gMercProfiles[JOHN_K_UB].bSectorZ = bMapZ;

			//remember that we have added him
			gJa25SaveStruct.fNpcHasBeenAdded |= SECTOR_ADDED_NPC__JOHN_K;
		}
		//	}
	}
}

void HandleSectorSpecificUnLoadingOfMap( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	//if this is the power gen map J13
	if( sMapX == gGameUBOptions.SectorFanX && sMapY == gGameUBOptions.SectorFanY && bMapZ == gGameUBOptions.SectorFanZ )
	{
		BOOLEAN fGoingToTunnelSector = FALSE;

		//if we are going to the first sector if the tunnel J14-1
		if( gWorldSectorX == gGameUBOptions.ExitForFanToPowerGenSectorX &&
				gWorldSectorY == gGameUBOptions.ExitForFanToPowerGenSectorY &&
				gbWorldSectorZ == gGameUBOptions.ExitForFanToPowerGenSectorZ )
		{
			fGoingToTunnelSector = TRUE;
		}

		switch ( gJa25SaveStruct.ubStateOfFanInPowerGenSector )
		{
		case PGF__RUNNING_NORMALLY:
			HandleRemovingPowerGenFanSound( );
			break;
		}

		//Remeber how the player got through
		HandleHowPlayerGotThroughFan( );
	}
	//else if this is the 1st level of tunne;l J14-1
	else	if( sMapX == gGameUBOptions.ExitForFanToPowerGenSectorX && sMapY == gGameUBOptions.ExitForFanToPowerGenSectorY && bMapZ == gGameUBOptions.ExitForFanToPowerGenSectorZ )
	{
		switch ( gJa25SaveStruct.ubStateOfFanInPowerGenSector )
		{
		case PGF__RUNNING_NORMALLY:
		case PGF__STOPPED:

			//remove the sound to the world
			HandleRemovingPowerGenFanSound( );
			break;
		}
	}
}


void HandleSectorSpecificModificatioToMap( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fLoadingSavedGame )
{
	//	SOLDIERTYPE *pSoldier=NULL;
	//	INT32				iCash=0;

	//if we are loading a game, dont do this yet ( it will be done after everything is loaded
	if ( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
	{
		return;
	}

	SetTileAnimCounter( TILE_ANIM__NORMAL_SPEED );

	//if we are being called from LoadSavedGame()
	if ( fLoadingSavedGame )
	{
		//
		// only do certain modifications
		//

		//if this is the power gen map J13-0
		if( sMapX == gGameUBOptions.SectorFanX && sMapY == gGameUBOptions.SectorFanY && bMapZ == gGameUBOptions.SectorFanZ )
		{
			HandlePowerGenFanSoundModification( );
		}
		else	if( sMapX == gGameUBOptions.ExitForFanToPowerGenSectorX && sMapY == gGameUBOptions.ExitForFanToPowerGenSectorY && bMapZ == gGameUBOptions.ExitForFanToPowerGenSectorZ ) //J14-1
		{
			HandleFirstPartOfTunnelFanSound( );
		}

		//if this is the Final Sector of the complex L15-3
		else if( sMapX == gGameUBOptions.SectorLaunchMisslesX && sMapY == gGameUBOptions.SectorLaunchMisslesY && bMapZ == gGameUBOptions.SectorLaunchMisslesZ )
		{
			HandleOpenControlPanelToRevealSwitchInMorrisArea( );
		}
	}
	else
	{
		//if this is the first map H7
		if( sMapX == gGameExternalOptions.ubDefaultArrivalSectorX && sMapY == gGameExternalOptions.ubDefaultArrivalSectorY && bMapZ == 0 )
		{
		}

		//if this is the guardpost H9
		else if( sMapX == gGameUBOptions.SectorGuardPostX && sMapY == gGameUBOptions.SectorGuardPostY && bMapZ == gGameUBOptions.SectorGuardPostZ )
		{
			//if we havent added the money to the sector before
			if ( !IsJa25GeneralFlagSet( JA_GF__PICKED_UP_MONEY_IN_GUARD_POST ) )
			{
				SetJa25GeneralFlag( JA_GF__PICKED_UP_MONEY_IN_GUARD_POST );

				// Add some money to the location
				//CreateAndAddMoneyObjectToGround( 9026, 15000, 10000, 7000 );
				  CreateAndAddMoneyObjectToGround( gGameUBOptions.H9MoneyGridNo, gGameUBOptions.H9MoneyEasy, gGameUBOptions.H9MoneyMedium, gGameUBOptions.H9MoneyHard);
			}
		}

		//if this is the First sector of the town I10
		else if( sMapX == gGameUBOptions.FristSectorTownX && sMapY == gGameUBOptions.FristSectorTownY && bMapZ == gGameUBOptions.FristSectorTownX )
		{
			//if we havent added the money to the sector before
			if ( !IsJa25GeneralFlagSet( JA_GF__PICKED_UP_MONEY_IN_FIRST_TOWN ) )
			{
				SetJa25GeneralFlag( JA_GF__PICKED_UP_MONEY_IN_FIRST_TOWN );

				// Add some money to the location
				//CreateAndAddMoneyObjectToGround( 11894, 8000, 4000, 3000 );
				//CreateAndAddMoneyObjectToGround( 7906,  12000, 6000, 5000 );
				CreateAndAddMoneyObjectToGround( gGameUBOptions.I10MoneyGridNo1, gGameUBOptions.I10MoneyEasy1, gGameUBOptions.I10MoneyMedium1, gGameUBOptions.I10MoneyHard1 );
				CreateAndAddMoneyObjectToGround( gGameUBOptions.I10MoneyGridNo2, gGameUBOptions.I10MoneyEasy2, gGameUBOptions.I10MoneyMedium2, gGameUBOptions.I10MoneyHard2 );
				
			}
		}

		//if this is the power gen map J13
		else if( sMapX == gGameUBOptions.SectorFanX && sMapY == gGameUBOptions.SectorFanY && bMapZ == gGameUBOptions.SectorFanZ )
		{
			HandlePowerGenFanSoundModification( );
		}

		//else if this is the 1st part of tunnel J14-1
		else	if( sMapX == gGameUBOptions.ExitForFanToPowerGenSectorX && sMapY == gGameUBOptions.ExitForFanToPowerGenSectorY && bMapZ == gGameUBOptions.ExitForFanToPowerGenSectorZ )
		{
			HandleFirstPartOfTunnelFanSound( );

			if ( IsJa25GeneralFlagSet( JA_GF__MOVE_ENEMIES_TO_EDGE_IN_TUNNEL_1 ) )
			{
				HandleMovingEnemiesCloseToEntranceInFirstTunnelMap( );
			}
		}

		//else if this is the 2nd part of tunnel K14-1
		else	if( sMapX == gGameUBOptions.SectorOpenGateInTunnelX && sMapY == gGameUBOptions.SectorOpenGateInTunnelY && bMapZ == gGameUBOptions.SectorOpenGateInTunnelZ )
		{
			if ( IsJa25GeneralFlagSet( JA_GF__MOVE_ENEMIES_TO_EDGE_IN_TUNNEL_1 ) )
			{
				HandleMovingEnemiesCloseToEntranceInSecondTunnelMap( );
			}
		}

		//else if this is the 1st level in the complex K15-1
		else	if( sMapX == gGameUBOptions.SectorDoorInTunnelX && sMapY == gGameUBOptions.SectorDoorInTunnelY && bMapZ == gGameUBOptions.SectorDoorInTunnelZ )
		{
			//Make all the team members look like they dropped from a high place
			//		MakeAllTeamMembersCrouchedThenStand();

			//If the player made noise in the tunnel, enemies should be placed near them
			//HandleMovingTheEnemiesToBeNearPlayerWhenEnteringComplexMap(); //is Disabled. The problem of moving from K14_1 to K15_1.

			//if the big door should be opened
			HandleFortifiedDoor( );
		}

		//if this is the Final Sector of the complex L15-3
		else if( sMapX == gGameUBOptions.SectorLaunchMisslesX && sMapY == gGameUBOptions.SectorLaunchMisslesY && bMapZ == gGameUBOptions.SectorLaunchMisslesZ )
		{
			HandleOpenControlPanelToRevealSwitchInMorrisArea( );
		}




		//if the enemies should go and find the player mercs
		if ( GetSectorEnemyIsToImmediatelySeekEnemyIn( ) != -1 )
		{
			//Make the enemies go find the player mercs
			SetEnemiesToFindThePlayerMercs( );
		}

		//if this is a sector we feel can be made harder for players ( on hard difficulty levels ), then move some
		//enemies onto roofs.
		HandleMovingEnemiesOntoRoofs( );
	}
}
/*
void MakeAllTeamMembersCrouchedThenStand()
{
UINT8	cnt;
SOLDIERTYPE *pSoldier=NULL;

//Move some of the enemies to be 'near' them player when the enter the room
cnt = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;

// Loop through the list and move some of the enemies
for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++, pSoldier++)
{
//if the soldier is active,
if( pSoldier->bActive && pSoldier->bInSector )
{
pSoldier->EVENT_InitNewSoldierAnim( CRAWLING, 1, TRUE );
pSoldier->ChangeSoldierStance( PRONE );
}
}
}
*/
void HandleMovingTheEnemiesToBeNearPlayerWhenEnteringComplexMap( )
{
	SOLDIERTYPE *pSoldier = NULL;
	UINT8				ubNumEnemiesMoved = 0;

	//if we are loading a saved game, or we have already moved the enemies, get out.
	if ( gTacticalStatus.uiFlags & LOADING_SAVED_GAME ||
		 gJa25SaveStruct.uiJa25GeneralFlags & JA_GF__ALREADY_MOVED_ENEMIES_IN_COMPLEX )
	{
		return;
	}

	//if the player made a 'noise' going through the gate at the end of the tunnel sector
	if ( gJa25SaveStruct.uiJa25GeneralFlags & JA_GF__DID_PLAYER_MAKE_SOUND_GOING_THROUGH_TUNNEL_GATE )
	{
		UINT8	cnt;

		//
		//Move some of the enemies to be 'near' them player when the enter the room
		//

		// Loop through the list and move some of the enemies
		cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;
		for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; cnt++, pSoldier++ )
		{
			//if the soldier is active,
			if ( pSoldier->bActive )
			{
				//
				// move the soldier to the modified location
				//

				if ( pSoldier->sGridNo == 13959 )
				{
					pSoldier->SetSoldierGridNo( 15705, TRUE );
					ubNumEnemiesMoved++;
				}

				if ( pSoldier->sGridNo == 13983 )
				{
					pSoldier->SetSoldierGridNo( 15712, TRUE );
					ubNumEnemiesMoved++;
				}

				if ( pSoldier->sGridNo == 12543 )
				{
					pSoldier->SetSoldierGridNo( 15233, TRUE );
					ubNumEnemiesMoved++;
				}
			}
		}

		while ( ubNumEnemiesMoved < 3 )
		{
			cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;
			for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; cnt++, pSoldier++ )
			{
				//if the soldier is active,
				if ( pSoldier->bActive  && pSoldier->sGridNo != 15705 && pSoldier->sGridNo != 15712 && pSoldier->sGridNo != 15233 )
				{
					//
					// move the soldier to the modified location
					//

					if ( ubNumEnemiesMoved == 0 )
					{
						pSoldier->SetSoldierGridNo( 15706, TRUE );
						ubNumEnemiesMoved++;
					}

					if ( ubNumEnemiesMoved == 1 )
					{
						pSoldier->SetSoldierGridNo( 15713, TRUE );
						ubNumEnemiesMoved++;
					}

					if ( ubNumEnemiesMoved == 2 )
					{
						pSoldier->SetSoldierGridNo( 15234, TRUE );
						ubNumEnemiesMoved++;
					}
				}
			}
		}

		//Remeber we have moved the enemies
		gJa25SaveStruct.uiJa25GeneralFlags |= JA_GF__ALREADY_MOVED_ENEMIES_IN_COMPLEX;
	}
}

void HandleFortifiedDoor( )
{
	//if the fortified door should be open
	if ( gJa25SaveStruct.ubStatusOfFortifiedDoor == FD__OPEN )
	{
		ModifyDoorStatus( gGameUBOptions.SectorDoorInTunnelGridNo, TRUE, DONTSETDOORSTATUS ); //11419
	}
}

void CreateAndAddMoneyObjectToGround( UINT32 sGridNo, INT32 iEasyAmount, INT32 iNormalAmount, INT32 iHardAmount )
{
	OBJECTTYPE	Object;
	INT32				iCash = 0;
	//	INT8				bAmountToAdd=0;

	switch ( gGameOptions.ubDifficultyLevel )
	{
	case DIF_LEVEL_EASY:
		iCash = iEasyAmount;
		break;

	case DIF_LEVEL_MEDIUM:
		iCash = iNormalAmount;
		break;

	case DIF_LEVEL_HARD:
		iCash = iHardAmount;
		break;

	default:
		iCash = iNormalAmount;
		//Assert(0);
		break;
	}

	CreateMoney( iCash, &Object );

	//add the item to the world
	AddItemToPool( sGridNo, &Object, FALSE, 0, 0, 0 );
}


void HandleGoingUpOrDownStairsForLoadScreensPurposes( INT16 sCurrentlyInSectorZ, INT16 sGoingToSectorZ )
{
	if ( sCurrentlyInSectorZ == sGoingToSectorZ )
	{
		gJa25SaveStruct.ubLoadScreenStairTraversal = LS__NOT_GOING_UP_STAIRS;
	}
	else if ( sCurrentlyInSectorZ < sGoingToSectorZ )
	{
		gJa25SaveStruct.ubLoadScreenStairTraversal = LS__GOING_DOWN_STAIRS;
	}
	else
	{
		gJa25SaveStruct.ubLoadScreenStairTraversal = LS__GOING_UP_STAIRS;
	}
}

void HandleMovingEnemiesCloseToEntranceInFirstTunnelMap( )
{
	SOLDIERTYPE *pSoldier = NULL;
	UINT8	ubIndex = 0;
	UINT32 cnt;
	BOOLEAN	fDone = FALSE;
	INT16							sXPos, sYPos;
	UINT32 sGridNos[27] = {18200, 18360, 18520,
		18199, 18359, 18519,
		18198, 18358, 18518,
		18197, 18357, 18517,
		18196, 18356, 18516,
		18195, 18355, 18515,
		18194, 18354, 18514,
		18193, 18353, 18513,
		18035, 18034, 18033,
	};


	//
	//Move some of the enemies to be 'near' them player when the enter the room
	//

	// Loop through the list and move some of the enemies
	cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; cnt++, pSoldier++ )
	{
		//if the soldier is active,
		if ( pSoldier->bActive )
		{
			fDone = FALSE;
			while ( !fDone )
			{
				//if there is no one in the gridno
				if ( WhoIsThere2( sGridNos[ubIndex], 0 ) == NOBODY )
				{
					// move the soldier to the modified location
					ConvertGridNoToCenterCellXY( sGridNos[ubIndex], &sXPos, &sYPos );
					pSoldier->EVENT_SetSoldierPosition( sXPos, sYPos );
					//					SetSoldierGridNo( pSoldier, sGridNos[ ubIndex ], TRUE );
					ubIndex++;
					fDone = TRUE;
				}
				else
				{
					ubIndex++;
				}

				if ( ubIndex >= 27 )
				{
					Assert( 0 );
					return;
				}
			}
		}
	}
}

void HandleMovingEnemiesCloseToEntranceInSecondTunnelMap( )
{
	SOLDIERTYPE *pSoldier = NULL;
	UINT8	ubIndex = 0;
	BOOLEAN	fDone = FALSE;
	UINT32 cnt;
	INT16							sXPos, sYPos;
	UINT32 sGridNos[30] = {4900, 4901, 4902, 4903, 4904,
		5060, 5061, 5062, 5063, 5064,
		5220, 5221, 5222, 5223, 5224,
		5380, 5381, 5382, 5383, 5384,
		5540, 5541, 5542, 5543, 5544,
		5700, 5701, 5702, 5703, 5704};



	//
	//Move some of the enemies to be 'near' them player when the enter the room
	//

	// Loop through the list and move some of the enemies
	cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; cnt++, pSoldier++ )
	{
		//if the soldier is active,
		if ( pSoldier->bActive )
		{
			fDone = FALSE;
			while ( !fDone )
			{
				//if there is no one in the gridno
				if ( WhoIsThere2( sGridNos[ubIndex], 0 ) == NOBODY )
				{
					// move the soldier to the modified location
					ConvertGridNoToCenterCellXY( sGridNos[ubIndex], &sXPos, &sYPos );
					pSoldier->EVENT_SetSoldierPosition( sXPos, sYPos );
					//					SetSoldierGridNo( pSoldier, sGridNos[ ubIndex ], TRUE );
					ubIndex++;
					fDone = TRUE;
				}
				else
				{
					ubIndex++;
				}

				if ( ubIndex >= 26 )
				{
					Assert( 0 );
					return;
				}
			}
		}
	}
}

void HandlePowerGenFanSoundModification( )
{
	SetTileAnimCounter( TILE_ANIM__FAST_SPEED );

	switch ( gJa25SaveStruct.ubStateOfFanInPowerGenSector )
	{
	case PGF__RUNNING_NORMALLY:
		HandleAddingPowerGenFanSound( );

		//MAKE SURE the fan does not have an exit grid
		RemoveExitGridFromWorld( PGF__FAN_EXIT_GRID_GRIDNO );
		break;

	case PGF__STOPPED:
		//Add an exit grid to the map
		AddExitGridForFanToPowerGenSector( );
		break;

	case PGF__BLOWN_UP:
		break;

	default:
		Assert( 0 );
	}
}

void HandleFirstPartOfTunnelFanSound( )
{
	switch ( gJa25SaveStruct.ubStateOfFanInPowerGenSector )
	{
	case PGF__RUNNING_NORMALLY:
	case PGF__STOPPED:

		//add the sound to the world
		HandleAddingPowerGenFanSound( );
		break;
	}
}

void HandleMovingEnemiesOntoRoofs( )
{
	if ( gWorldSectorX <= 0 || gWorldSectorY <= 0 || gbWorldSectorZ < 0 )
	{
		return;
	}

	//if this is the sector south of the town J11
	if( gWorldSectorX == gGameUBOptions.SectorTown2X && gWorldSectorY ==gGameUBOptions.SectorTown2Y && gbWorldSectorZ == gGameUBOptions.SectorTown2Z )
	{
		switch ( gGameOptions.ubDifficultyLevel )
		{
		case DIF_LEVEL_EASY:
		case DIF_LEVEL_MEDIUM:
			break;
		case DIF_LEVEL_HARD:
				//MoveEnemyFromGridNoToRoofGridNo( 15446, 13993 );
				//MoveEnemyFromGridNoToRoofGridNo( 15436, 14006 );
				MoveEnemyFromGridNoToRoofGridNo( gGameUBOptions.SectorTownGridNo1a, gGameUBOptions.SectorTownGridNo1b );
				MoveEnemyFromGridNoToRoofGridNo( gGameUBOptions.SectorTownGridNo2a, gGameUBOptions.SectorTownGridNo2a );
			break;
		default:
				MoveEnemyFromGridNoToRoofGridNo( gGameUBOptions.SectorTownGridNo1a, gGameUBOptions.SectorTownGridNo1b );
				MoveEnemyFromGridNoToRoofGridNo( gGameUBOptions.SectorTownGridNo2a, gGameUBOptions.SectorTownGridNo2a );
			break;
		}
	}

	//else if this is the sector south of the town H11
	else if( gWorldSectorX == gGameUBOptions.SectorTown3X && gWorldSectorY == gGameUBOptions.SectorTown3Y && gbWorldSectorZ == gGameUBOptions.SectorTown3Z )
	{
		switch ( gGameOptions.ubDifficultyLevel )
		{
		case DIF_LEVEL_EASY:
		case DIF_LEVEL_MEDIUM:
			break;
		case DIF_LEVEL_HARD:
				//MoveEnemyFromGridNoToRoofGridNo( 8711, 5521 );
				MoveEnemyFromGridNoToRoofGridNo( gGameUBOptions.SectorTownGridNo3a, gGameUBOptions.SectorTownGridNo3a );
			break;
		default:
				MoveEnemyFromGridNoToRoofGridNo( gGameUBOptions.SectorTownGridNo3a, gGameUBOptions.SectorTownGridNo3a );
			break;
		}
	}
}



BOOLEAN MoveEnemyFromGridNoToRoofGridNo( UINT32 sSourceGridNo, UINT32 sDestGridNo )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32				cnt;
	INT16				sXPos, sYPos;

	cnt = gTacticalStatus.Team[ENEMY_TEAM].bFirstID;
	for ( pSoldier = MercPtrs[cnt]; cnt <= gTacticalStatus.Team[ENEMY_TEAM].bLastID; cnt++, pSoldier++ )
	{
		if ( pSoldier->stats.bLife >= OKLIFE && pSoldier->bActive && pSoldier->bInSector &&
			 pSoldier->sGridNo == sSourceGridNo )
		{
			pSoldier->SetSoldierHeight( 50.0 );

			// move soldier
			ConvertGridNoToCenterCellXY( sDestGridNo, &sXPos, &sYPos );
			pSoldier->EVENT_SetSoldierPosition( sXPos, sYPos );

			return(TRUE);
			//			pSoldier->bOrders = SEEKENEMY;
		}
	}

	return(FALSE);
}
#endif

// Flugente: militia movement: can we order militia reinforcements from( sSrcMapX, sSrcMapY ) to( sMapX, sMapY ) ?
BOOLEAN CanRequestMilitiaReinforcements( INT16 sMapX, INT16 sMapY, INT16 sSrcMapX, INT16 sSrcMapY )
{
	if ( !gGameExternalOptions.gfAllowReinforcements || !gSkillTraitValues.usVOReinforcementSetting )
		return FALSE;

	// not possible of nobody is there
	if ( !NumNonPlayerTeamMembersInSector( sSrcMapX, sSrcMapY, MILITIA_TEAM ) )
		return FALSE;

	UINT8 townid_A = GetTownIdForSector( sMapX, sMapY );
	UINT8 townid_B = GetTownIdForSector( sSrcMapX, sSrcMapY );

	switch ( gSkillTraitValues.usVOReinforcementSetting )
	{
		// reinforcements allowed inside town
	case 1:
		if ( townid_A != BLANK_SECTOR && townid_A == townid_B )
			return TRUE;
		break;

		// reinforcements allowed to and from town
	case 2:
		if ( townid_A != BLANK_SECTOR || townid_B != BLANK_SECTOR )
			return TRUE;
		break;

		// reinforcements allowed everywhere
	case 3:
		return TRUE;
		break;

	default:
		break;
	}

	return FALSE;
}

// Bob: this function will check if the merc is in a valid squad and if there are other mercs assigned to that squad who lack a mvt group.
// Will attempt to fix this by assigning everyone to a proper squad and forcing squads to reevaluate movement groups.
// returns the movement group the character was reassigned to or zero if we failed.
UINT8 tryToRecoverSquadsAndMovementGroups(SOLDIERTYPE* pCharacter) {
	// check if the char we're trying to move is in a valid squad!
	if (SquadCharacterIsIn(pCharacter) == -1) {

		if (pCharacter->bAssignment < ON_DUTY && SquadIsEmpty(pCharacter->bAssignment)) 
		{ // assignment says we're in a squad but that squad is empty!			
			int squadWeThinkWeAreIn = pCharacter->bAssignment;

			for (int i = 0; i < MAXMERCS; i++) {
				if (Menptr[i].bActive && Menptr[i].bTeam == pCharacter->bTeam && Menptr[i].bAssignment == squadWeThinkWeAreIn) 
				{ // reassign everyone who's supposed to be in the bogus squad
					AddCharacterToAnySquad(&Menptr[i]); 
				}
			}
		}
		else 
		{ // that squad seems fine, it's just the merc that got confused
			AddCharacterToAnySquad(pCharacter); 
		}
	}
	CheckSquadMovementGroups();
	return GetSoldierGroupId(pCharacter);
}
