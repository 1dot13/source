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

struct UILayout_CharPanelIconRegion
{
	INT32 x;
	INT32 y;
	INT32 width;
	INT32 height;
	INT32 spacing;
};

struct UILayout_MapBorderButtons
{
	SGPPoint Town;
	SGPPoint Mine;
	SGPPoint Teams;
	SGPPoint Militia;
	SGPPoint Airspace;
	SGPPoint Inventory;
	SGPPoint Disease;
	UINT16 Weather_Y; // Dynamic x position
	UINT16 Intel_Y; // Dynamic x position
};

struct UILayout_ETA
{
	int Start_Y;
	int Clock_X;
	int Hour_X;
	int Min_X;
};

struct UILayout_HeliETA
{
	SGPRectangle PopupBox;
	int Upper_Popup_Y;
	int Alternate_Height;
};

struct UILayout_Map
{
	SGPRectangle BorderRegion;
	SGPRectangle ViewRegion;
	SGPRectangle Alphabet;
	SGPRectangle Numbers;
	SGPRectangle LevelMarkerArea;
	SGPPoint LevelString;
	SGPPoint GridSize;
	UILayout_MapBorderButtons Button;
	UILayout_ETA ETA;
	UILayout_HeliETA HeliETA;
};

struct UILayout_CharList
{
	SGPRectangle Region;
	SGPPoint Title;
	int y;
	int yOffset;
	int xName;
	int widthName;
	int xAssignment;
	int widthAssignment;
	int xSleep;
	int widthSleep;
	int xLocation;
	int widthLocation;
	int xETA;
	int widthETA;
	int xTimeRemaining;
	int widthTimeRemaining;
};

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
BOOLEAN InitializeInvPanelCoordsRobot( );
void initMapViewAndBorderCoordinates(void);

// HEADROCK HAM 3.6: Calculate daily cost for all mercs who have one.
INT32 GetTotalContractExpenses ( void );

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

bool isWidescreenUI(void);

#endif