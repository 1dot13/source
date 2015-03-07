#ifndef __MAPSCREEN_H
#define __MAPSCREEN_H


#include "Types.h"
#include "Soldier Control.h"

#define			MAP_WORLD_X						18
#define			MAP_WORLD_Y						18

extern int MAP_ITEMDESC_START_Y;
extern int MAP_ITEMDESC_START_X;
#define MAX_TOWN_NAME_LENGHT	32
#define MAX_SECTOR_NAME_LENGTH	128

#define MAX_TOWNS			50	//13 //50
#define MAX_TOWN_SECTORS	260	//40 //260

// HEADROCK HAM B2.7: Moved here from Strategicmap.cpp to allow use by MilitiaSquads.cpp

extern BOOLEAN	gfMilitiaAllowedInTown	[ MAX_TOWNS ];

// Country name identifiers
enum
{
	COUNTRY_NAME,
	COUNTRY_NOUN,
} ;

// Sector name identifiers
enum Towns
{
	BLANK_SECTOR=0,
	OMERTA,
	DRASSEN,
	ALMA,
	GRUMM,
	TIXA,
	CAMBRIA,
	SAN_MONA,
	ESTONI,
	ORTA,
	BALIME,
	MEDUNA,
	CHITZENA,

	//NUM_TOWNS
} ;

extern INT8 NUM_TOWNS;

#define FIRST_TOWN	OMERTA
//#define PALACE			NUM_TOWNS


extern BOOLEAN fCharacterInfoPanelDirty;
extern BOOLEAN fTeamPanelDirty;
extern BOOLEAN fMapPanelDirty;
extern BOOLEAN fDrawCharacterList; // OJW - 20081204

extern BOOLEAN fMapInventoryItem;
extern BOOLEAN gfInConfirmMapMoveMode;
extern BOOLEAN gfInChangeArrivalSectorMode;

extern BOOLEAN gfSkyriderEmptyHelpGiven;


BOOLEAN SetInfoChar(UINT8 ubSolId);
void EndMapScreen( BOOLEAN fDuringFade );
void ReBuildCharactersList( void );

// WANNE - BMP: Initialize world size for a multiplayer game
void InitializeWorldSize( INT16 sSectorX, INT16 sSectorY , INT8 bSectorZ );

void HandleRemovalOfPreLoadedMapGraphics( void );

void ChangeSelectedMapSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ );

BOOLEAN CanToggleSelectedCharInventory( void );

BOOLEAN CanExtendContractForCharSlot( INT8 bCharNumber );

void TellPlayerWhyHeCantCompressTime( void );

void ChangeSelectedInfoChar( INT8 bCharNumber, BOOLEAN fResetSelectedList );

void MAPEndItemPointer();

void CopyPathToAllSelectedCharacters( PathStPtr pPath );
void CancelPathsOfAllSelectedCharacters();

INT32 GetPathTravelTimeDuringPlotting( PathStPtr pPath );

void AbortMovementPlottingMode( void );
void CancelChangeArrivalSectorMode( void );

void ExplainWhySkyriderCantFly( void );

BOOLEAN CanChangeSleepStatusForCharSlot( INT8 bCharNumber );
BOOLEAN CanChangeSleepStatusForSoldier( SOLDIERTYPE *pSoldier );

BOOLEAN MapCharacterHasAccessibleInventory( INT8 bCharNumber );

// CHRISL: New functions to handle initialization of inventory coordinates
BOOLEAN InitializeInvPanelCoordsOld( );
BOOLEAN InitializeInvPanelCoordsNew( );
BOOLEAN InitializeInvPanelCoordsVehicle( );

// HEADROCK HAM 3.6: Calculate daily cost for all mercs who have one.
INT32 GetTotalContractExpenses ( void );

// HEADROCK HAM 4: enumarate Manual Militia Restriction states
enum
{
	MANUAL_MOBILE_RESTRICTED,
	MANUAL_MOBILE_NO_ENTER,
	MANUAL_MOBILE_NO_LEAVE,
	MANUAL_MOBILE_NO_RESTRICTION,
	NUM_MANUAL_MOBILE_STATES,
};

// Flugente: militia movement
extern BOOLEAN fPlotForMilitia;
extern UINT32 gMilitiaPlotStartSector;

BOOLEAN RequestGiveMilitiaNewDestination( void );

void SetUpMilitiaForMovement();

INT32 GetGroundTravelTimeOfMilitia( );

typedef struct
{
	INT16		sGroupid;		// group ids are normally UINT8 - we use -1 to indicate that it hasn't been used
	PathStPtr	path;			// the path of a militia group that travels
} MILITIA_PATH;

#define MILITIA_PATROLS_MAX	30

extern MILITIA_PATH gMilitiaPath[MILITIA_PATROLS_MAX];

// upon starting a game/loading from an old version, set up the data
void MilitiaPlotInit();

// when starting to plot, set up things
BOOLEAN MilitiaPlotStart();

// when finished plotting, set up other things
void MilitiaplotFinish();

// if this group is a militia travel group, return its slot, othewise return -1
INT16 GetMilitiaPathSlot(UINT8 uGroupId);

void DeleteAllMilitiaPaths();

// return the last militia group in this sector, group id will be stored in arId. return FALSE if none found
BOOLEAN GetLastMilitiaGroupInSector( INT16 sMapX, INT16 sMapY, UINT8& arId );

BOOLEAN CanGiveStrategicMilitiaMoveOrder( INT16 sMapX, INT16 sMapY );

void ConvertMinTimeToDayHourMinString( UINT32 uiTimeInMin, STR16 sString );
void ConvertMinTimeToETADayHourMinString( UINT32 uiTimeInMin, STR16 sString );

// Flugente: bandaging during retreat
void RetreatBandageCallback( UINT8 ubResult );

#endif