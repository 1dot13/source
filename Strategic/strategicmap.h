#ifndef __STRATMAP_H
#define __STRATMAP_H
// this file is for manipulation of the strategic map structure
#include "types.h"
#include "soldier control.h"
#include "FileMan.h"

#include "mapscreen.h"

//The maximum size for any team strategically speaking.	For example, we can't have more than 20 enemies, militia, or creatures at a time.
//#define MAX_STRATEGIC_TEAM_SIZE	20

// Codes for jumoing into adjacent sectors..
enum
{
	JUMP_ALL_LOAD_NEW,
	JUMP_ALL_NO_LOAD,
	JUMP_SINGLE_LOAD_NEW,
	JUMP_SINGLE_NO_LOAD
};


enum{
	CONTROLLED=0,
	UNCONTROLLED,
};

// For speed, etc lets make these globals, forget the functions if you want
extern INT16	gWorldSectorX;
extern INT16	gWorldSectorY;
extern INT8		gbWorldSectorZ;

extern BOOLEAN	gfHiddenTown			[ MAX_TOWNS ];
extern BOOLEAN	gfDrawHiddenTown		[ MAX_TOWNS ];
extern BOOLEAN	gfDrawHiddenTownTemp	[ MAX_TOWNS ];
extern BOOLEAN	gfHiddenTownTemp		[ MAX_TOWNS ];
extern BOOLEAN	gfIconTown				[ MAX_TOWNS ];
extern BOOLEAN	gfIconTownTemp			[ MAX_TOWNS ];

// SAM definitions
extern UINT8	NUMBER_OF_SAMS;
#define MAX_NUMBER_OF_SAMS	50 //4 //50

extern INT16 pSamList[ MAX_NUMBER_OF_SAMS ];
extern INT32 pSamGridNoAList[ MAX_NUMBER_OF_SAMS ];
extern INT32 pSamGridNoBList[ MAX_NUMBER_OF_SAMS ];
extern INT16 gpSamSectorX[ MAX_NUMBER_OF_SAMS ];
extern INT16 gpSamSectorY[ MAX_NUMBER_OF_SAMS ];

extern BOOLEAN fFoundOrta;
extern BOOLEAN fSamSiteFound[ MAX_NUMBER_OF_SAMS ];

extern	BOOLEAN		gfUseAlternateMap;

/* Buggler: used externalized gpSamSectorX[ MAX_NUMBER_OF_SAMS ] values instead of hardcoded ones
// SAM sites
#define SAM_1_X 2
#define SAM_2_X 15
#define SAM_3_X 8
#define SAM_4_X 4

#define SAM_1_Y 4
#define SAM_2_Y 4
#define SAM_3_Y 9
#define SAM_4_Y 14
*/

// min condition for sam site to be functional
#define MIN_CONDITION_FOR_SAM_SITE_TO_WORK 50	// Flugente: changed from 80 to 50


// FUNCTIONS FOR DERTERMINING GOOD SECTOR EXIT DATA
#define			CHECK_DIR_X_DELTA							( WORLD_TILE_X * 4 )
#define			CHECK_DIR_Y_DELTA							( WORLD_TILE_Y * 10 )

// get index into aray
#define	CALCULATE_STRATEGIC_INDEX( x, y )			( x + ( y * MAP_WORLD_X ) )
#define	GET_X_FROM_STRATEGIC_INDEX( i )				( i % MAP_WORLD_X )
#define	GET_Y_FROM_STRATEGIC_INDEX( i )				( i / MAP_WORLD_X )
#define	INVALID_STRATEGIC_INDEX                     ( 0 )

// macros to convert between the 2 different sector numbering systems
#define		SECTOR_INFO_TO_STRATEGIC_INDEX( i )		( CALCULATE_STRATEGIC_INDEX ( SECTORX( i ), SECTORY( i ) ) )
#define		STRATEGIC_INDEX_TO_SECTOR_INFO( i )		( SECTOR(	GET_X_FROM_STRATEGIC_INDEX( i ), GET_Y_FROM_STRATEGIC_INDEX( i ) ) )

#define AI_SAMSITE_REPAIR_EASY		20
#define AI_SAMSITE_REPAIR_MEDIUM	25
#define AI_SAMSITE_REPAIR_HARD		34
#define AI_SAMSITE_REPAIR_INSANE	50

// grab the town id value
UINT8 GetTownIdForSector( INT16 sMapX, INT16 sMapY );

void GetCurrentWorldSector( INT16 *psMapX, INT16 *psMapY );
BOOLEAN	SetCurrentWorldSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ );

BOOLEAN EnterSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
void UpdateMercsInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
void UpdateMercInSector( SOLDIERTYPE *pSoldier, INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

BOOLEAN ReadInMapStructure(STR fileName, BOOLEAN localizedVersion);
BOOLEAN ReadInStrategicMapSectorTownNames(STR fileName, BOOLEAN localizedVersion);
BOOLEAN WriteInStrategicMapSectorTownNames(STR fileName);

// get short sector name without town name
void GetShortSectorString( INT16 sMapX,INT16 sMapY, STR16 sString );

//NEW!
//Calculates the name of the sector based on the loaded sector values.
//Examples:		A9		
//						A10_B1		
//						J9_B2_A ( >= BETAVERSION ) else J9_B2 (release equivalent)
void GetLoadedSectorString( STR16 pString );

// This will get an ID string like A9- OMERTA...
void GetSectorIDString( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ , STR16 zString, BOOLEAN fDetailed );

void GetMapFileName(INT16 sMapX,INT16 sMapY, INT8 bSectorZ, STR8 bString, BOOLEAN fUsePlaceholder, BOOLEAN fAddAlternateMapLetter );

// Called from within tactical.....
void JumpIntoAdjacentSector( UINT8 ubDirection, UINT8 ubJumpCode, INT32 sAdditionalData );//dnl ch56 151009



BOOLEAN CanGoToTacticalInSector( INT16 sX, INT16 sY, UINT8 ubZ );

extern UINT32 ubSAMControlledSectors[MAP_WORLD_Y][MAP_WORLD_X];

void UpdateAirspaceControl( void );

BOOLEAN DoesSamCoverSector( UINT8 usSam, UINT8 usSector, BOOLEAN* apSamIsWorking = NULL );

BOOLEAN IsThisSectorASAMSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void RepairSamSite( UINT16 aSector );

// init sam sites
void InitializeSAMSites( void );

// get town sector size
UINT8 GetTownSectorSize( INT8 bTownId );

// get town sector size, that is under play control
UINT8 GetTownSectorsUnderControl( INT8 bTownId );

UINT16 GetTownWorkers(INT8 bTownId, UINT16& arusMax);
void AddTownWorkers( INT8 bTownId, INT16 asAdd );

// is this sector part of this town
BOOLEAN SectorIsPartOfTown( INT8 bTownId, INT16 sSectorX, INT16 sSectorY ); 

//BOOLEAN IsThereAnyOneInThisTown( UINT8 ubTownId );


BOOLEAN SoldierOKForSectorExit( SOLDIERTYPE * pSoldier, INT8 bExitDirection, INT32 usAdditionalData );//dnl ch56 151009
BOOLEAN OKForSectorExit( INT8 bExitDirection, INT32 usAdditionalData, UINT32 *puiTraverseTimeInMinutes );//dnl ch56 151009
void SetupNewStrategicGame( );

BOOLEAN LoadStrategicInfoFromSavedFile( HWFILE hFile );
BOOLEAN SaveStrategicInfoToSavedFile( HWFILE hFile );

void AllMercsHaveWalkedOffSector( );

void AdjustSoldierPathToGoOffEdge( SOLDIERTYPE *pSoldier, INT32 sEndGridNo, UINT8 ubTacticalDirection );

void AllMercsWalkedToExitGrid();

void PrepareLoadedSector();


#ifdef JA2UB
// no JA2 UB
#else
// handle for slay...no better place to really put this stuff
void HandleSlayDailyEvent( void );
#endif

void HandleQuestCodeOnSectorEntry( INT16 sNewSectorX, INT16 sNewSectorY, INT8 bNewSectorZ );

// handle a soldier leaving thier squad behind, this sets them up for mvt and potential rejoining of group
void HandleSoldierLeavingSectorByThemSelf( SOLDIERTYPE *pSoldier );

BOOLEAN CheckAndHandleUnloadingOfCurrentWorld();

// number of SAM sites under player control
INT32 GetNumberOfSAMSitesUnderPlayerControl( void );

// is there a FUNCTIONAL SAM site in this sector?
BOOLEAN IsThereAFunctionalSAMSiteInSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

BOOLEAN IsSectorDesert( INT16 sSectorX, INT16 sSectorY );
BOOLEAN IsSectorTropical( INT16 sSectorX, INT16 sSectorY ); // added by SANDRO
BOOLEAN IsSectorFarm( INT16 sSectorX, INT16 sSectorY ); // added by Flugente
BOOLEAN IsSectorRoad( INT16 sSectorX, INT16 sSectorY ); // added by Flugente

// sam site under players control?
INT32 SAMSitesUnderPlayerControl( INT16 sX, INT16 sY );

INT8 GetSAMIdFromSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );
void SetSamHackStatus( INT16 sSectorX, INT16 sSectorY, INT8 sStatus );

void SetupProfileInsertionDataForSoldier( SOLDIERTYPE *pSoldier );

BOOLEAN HandlePotentialBringUpAutoresolveToFinishBattle( int pSectorX, int pSectorY, int pSectorZ );

// WANNE - BMP;
BOOLEAN MapExists( UINT8 * szFilename );

BOOLEAN EscapeDirectionIsValid( INT8 * pbDirection );
bool IsEscapeDirectionValid(WorldDirections pbDirection);
//Used for determining the type of error message that comes up when you can't traverse to
//an adjacent sector.	THESE VALUES DO NOT NEED TO BE SAVED!
extern BOOLEAN	gfInvalidTraversal;
extern BOOLEAN	gfLoneEPCAttemptingTraversal;
extern BOOLEAN	gfRobotWithoutControllerAttemptingTraversal;
extern UINT8		gubLoneMercAttemptingToAbandonEPCs;
extern INT8			gbPotentiallyAbandonedEPCSlotID;

extern INT8 gbGreenToElitePromotions;
extern INT8 gbGreenToRegPromotions;
extern INT8 gbRegToElitePromotions;
extern INT8 gbMilitiaPromotions;

#ifdef JA2UB
void 	BeginLoadScreen();

//ja25 UB
void HandleSectorSpecificModificatioToMap( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fLoadingSavedGame );

void HandleEmailBeingSentWhenEnteringSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ, BOOLEAN fLaptopJustGotFixed );
#endif

// Flugente: militia movement: can we order militia reinforcements from( sSrcMapX, sSrcMapY ) to( sMapX, sMapY ) ?
BOOLEAN CanRequestMilitiaReinforcements( INT16 sMapX, INT16 sMapY, INT16 sSrcMapX, INT16 sSrcMapY );

// Bob: check and try to fix issues with squad and group assignment
UINT8 tryToRecoverSquadsAndMovementGroups(SOLDIERTYPE* pCharacter);

#endif
