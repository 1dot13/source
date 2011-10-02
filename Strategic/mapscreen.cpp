#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "HelpScreen.h"
#else
	#include "mapscreen.h"
	#include <stdio.h>
	#include <stdarg.h>
	#include <time.h>
	#include "gameloop.h"
	#include "sgp.h"
	#include "vobject.h"
	#include "wcheck.h"
	#include "worlddef.h"
	#include "input.h"
	#include "font.h"
	#include "screenids.h"
	#include "screens.h"
	#include "gameloop.h"
	#include "overhead.h"
	#include "sysutil.h"
	#include "input.h"
	#include "Event Pump.h"
	#include "Font Control.h"
	#include "Timer Control.h"
	#include "Interface.h"
	#include "Handle UI.h"
	#include "Interface Items.h"
	#include "interface utils.h"
	#include "lighting.h"
	#include <wchar.h>
	#include <tchar.h>
	#include "cursors.h"
	#include "Soldier Profile.h"
	#include "Interface Cursors.h"
	#include "Interface Panels.h"
	#include "Interface Control.h"
	#include "sys globals.h"
	#include "environment.h"
	#include "Radar Screen.h"
	#include "Render Dirty.h"
	#include "utilities.h"
	#include "Game Init.h"
	#include "renderworld.h"
	#include "finances.h"
	#include "message.h"
	#include "math.h"
	#include "vsurface.h"
	#include "vobject_blitters.h"
	#include "faces.h"
	#include "PopUpBox.h"
	#include "Game Clock.h"
	#include "items.h"
	#include "vobject.h"
	#include "Cursor Control.h"
	#include "text.h"
	#include "strategic.h"
	#include "strategicmap.h"
	#include "interface.h"
	#include "strategic pathing.h"
	#include "Map Screen Interface Bottom.h"
	#include "Map Screen Interface Border.h"
	#include "Map Screen Interface Map.h"
	#include "Map Screen Interface.h"
	#include "Strategic Pathing.h"
	#include "Assignments.h"
	#include "Squads.h"
	#include "Merc Contract.h"
	#include "Sound Control.h"
	#include "strategic turns.h"
	#include "Dialogue Control.h"
	#include "Map Screen Interface TownMine Info.h"
	#include "PreBattle Interface.h"
	#include "Personnel.h"
	#include "Animated ProgressBar.h"
	#include "Queen Command.h"
	#include "LaptopSave.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Vehicles.h"
	#include "Map Screen Helicopter.h"
	#include "gamescreen.h"
	#include "line.h"
	#include "english.h"
	#include "Fade Screen.h"
	#include "Strategic Mines.h"
	#include "GameVersion.h"
	#include "SaveLoadScreen.h"
	#include "Options Screen.h"
	#include "Auto Resolve.h"
	#include "meanwhile.h"
	#include "Campaign.h"
	#include "Random.h"
	#include "Air Raid.h"
	#include "quests.h"
	#include "Town Militia.h"
	#include "weapons.h"
	#include "Player Command.h"
	#include "Multi Language Graphic Utils.h"
	#include "HelpScreen.h"
	#include "Soldier macros.h"
	#include "Map Information.h"
	#include "cheats.h"
	#include "Strategic Town Loyalty.h"
	#include "GameSettings.h"
	#include "_Ja25EnglishText.h"
	#include "Militia Control.h"
	#include "Tactical Save.h"
	#include "Explosion Control.h"
	#include "Creature Spreading.h"
	#include "email.h"
	#include "Strategic Status.h"
	#include "Soldier Create.h"
	#include "Animation Control.h"
	// HEADROCK HAM 3.6: Include facilities for assignment display
	#include "Facilities.h"
	// HEADROCK HAM 4: Include Militia Squads for Manual Militia Restrictions toggle.
	#include "MilitiaSquads.h"
#endif

#include "connect.h" //hayden
#include "fresh_header.h"
#include "InterfaceItemImages.h"
// DEFINES


#define MAX_SORT_METHODS					6

// Cursors
#define SCREEN_CURSOR CURSOR_NORMAL

// Fonts
#define CHAR_FONT BLOCKFONT2 // COMPFONT

//#define ETA_FONT BLOCKFONT2

// Colors
#define CHAR_INFO_PANEL_BLOCK_COLOR 60

#define FONT_MAP_DKYELLOW 170

#define CHAR_TITLE_FONT_COLOR 6
#define CHAR_TEXT_FONT_COLOR 5

#define STARTING_COLOR_NUM	5

#define	MAP_TIME_UNDER_THIS_DISPLAY_AS_HOURS			( 3 * 24 * 60 )

#define MIN_KB_TO_DO_PRE_LOAD ( 32 * 1024 )


#define DELAY_PER_FLASH_FOR_DEPARTING_PERSONNEL 500
#define GLOW_DELAY 70
#define ASSIGNMENT_DONE_FLASH_TIME	500

#define	MINS_TO_FLASH_CONTRACT_TIME	(4 * 60)

// CHRISL: Reclassify all coordinates as int variables and declare their values in an initialization function.
int TOWN_INFO_X;
int TOWN_INFO_Y;

int PLAYER_INFO_X;
int PLAYER_INFO_Y;

// item description
int MAP_ITEMDESC_START_X;
int MAP_ITEMDESC_START_Y;

int INV_REGION_X;
int INV_REGION_Y;
int INV_REGION_WIDTH;
int INV_REGION_HEIGHT;
int INV_BTN_X;
int INV_BTN_Y;
int INV_BDROP_X;
int INV_BDROP_Y;

int MAP_ARMOR_LABEL_X;
int MAP_ARMOR_LABEL_Y;
int MAP_ARMOR_X;
int MAP_ARMOR_Y;
int MAP_ARMOR_PERCENT_X;
int MAP_ARMOR_PERCENT_Y;

int MAP_WEIGHT_LABEL_X;
int MAP_WEIGHT_LABEL_Y;
int MAP_WEIGHT_X;
int MAP_WEIGHT_Y;
int MAP_WEIGHT_PERCENT_X;
int MAP_WEIGHT_PERCENT_Y;

int MAP_CAMMO_LABEL_X;
int MAP_CAMMO_LABEL_Y;
int MAP_CAMMO_X;
int MAP_CAMMO_Y;
int MAP_CAMMO_PERCENT_X;
int MAP_CAMMO_PERCENT_Y;

int MAP_PERCENT_WIDTH;
int MAP_PERCENT_HEIGHT;

int MAP_INV_STATS_TITLE_FONT_COLOR;
int MAP_INV_STATS_TEXT_FONT_COLOR;

int PLAYER_INFO_FACE_START_X;
int PLAYER_INFO_FACE_START_Y;
int PLAYER_INFO_FACE_END_X;
int PLAYER_INFO_FACE_END_Y;

int INV_BODY_X;
int INV_BODY_Y;

int NAME_X;
int NAME_WIDTH;

int ASSIGN_X;
int ASSIGN_WIDTH;
int SLEEP_X;
int SLEEP_WIDTH;
int LOC_X;
int LOC_WIDTH;
int DEST_ETA_X;
int DEST_ETA_WIDTH;
int TIME_REMAINING_X;
int TIME_REMAINING_WIDTH;

int CLOCK_Y_START;

int DEST_PLOT_X;
int DEST_PLOT_Y;

int CLOCK_ETA_X;
int CLOCK_HOUR_X_START;
int CLOCK_MIN_X_START;

// contract
int CONTRACT_X;
int CONTRACT_Y;

// trash can
int TRASH_CAN_X;
int TRASH_CAN_Y;
int TRASH_CAN_WIDTH;
int TRASH_CAN_HEIGHT;

// keyring
int MAP_KEYRING_X;
int MAP_KEYRING_Y;

//Text offsets
int Y_OFFSET;

// The boxes defines
int TRAIN_Y_OFFSET;
int TRAIN_X_OFF;
int TRAIN_WID;
int TRAIN_HEIG;
int STRING_X_OFFSET;
int STRING_Y_OFFSET;
int POP_UP_BOX_X;
int POP_UP_BOX_Y;
int POP_UP_BOX_WIDTH;
int POP_UP_BOX_HEIGHT;
int MOUSE_PTR_Y_OFFSET;
int POP_UP_Y_OFFSET;
int TRAIN_TEXT_Y_OFFSET;


// char stat positions
int STR_X;
int STR_Y;
int DEX_X;
int DEX_Y;
int AGL_X;
int AGL_Y;
int LDR_X;
int LDR_Y;
int WIS_X;
int WIS_Y;
int LVL_X;
int LVL_Y;
int MRK_X;
int MRK_Y;
int EXP_X;
int EXP_Y;
int MEC_X;
int MEC_Y;
int MED_X;
int MED_Y;

int STAT_WID;
int STAT_HEI;

int PIC_NAME_X;
int PIC_NAME_Y;
int PIC_NAME_WID;
int PIC_NAME_HEI;
int CHAR_NAME_X;
int CHAR_NAME_Y;
int CHAR_NAME_WID;
int CHAR_NAME_HEI;
int CHAR_LOC_X;
int CHAR_LOC_Y;
int CHAR_LOC_WID;
int CHAR_LOC_HEI;
int CHAR_TIME_REMAINING_X;
int CHAR_TIME_REMAINING_Y;
int CHAR_TIME_REMAINING_WID;
int CHAR_TIME_REMAINING_HEI;
int CHAR_SALARY_X;
int CHAR_SALARY_Y;
int CHAR_SALARY_WID;
int CHAR_SALARY_HEI;
int CHAR_MEDICAL_X;
int CHAR_MEDICAL_Y;
int CHAR_MEDICAL_WID;
int CHAR_MEDICAL_HEI;
int CHAR_ASSIGN_X;
int CHAR_ASSIGN1_Y;
int CHAR_ASSIGN2_Y;
int CHAR_ASSIGN_WID;
int CHAR_ASSIGN_HEI;
int CHAR_HP_X;
int CHAR_HP_Y;
int CHAR_HP_WID;
int CHAR_HP_HEI;
int CHAR_MORALE_X;
int CHAR_MORALE_Y;
int CHAR_MORALE_WID;
int CHAR_MORALE_HEI;

int CROSS_X;
int CROSS_Y;
int CROSS_HEIGHT;
int CROSS_WIDTH;
int CHAR_PAY_X;
int CHAR_PAY_Y;
int CHAR_PAY_HEI;
int CHAR_PAY_WID;
int SOLDIER_PIC_X;
int SOLDIER_PIC_Y;
int SOLDIER_HAND_X;
int SOLDIER_HAND_Y;

// OJW: MP POSITIONS
int MP_BTN_Y;
int MP_ROWSTART_Y;
int MP_PLAYER_X;
int MP_PLAYER_W;
int MP_TEAM_X;
int MP_TEAM_W;
int MP_COMPASS_X;
int MP_COMPASS_W;
int MP_GAMEINFO_X;
int MP_GAMEINFO_W;

#define MAX_MP_BUTTONS	4

INT   gMapMPButtonsX[ MAX_MP_BUTTONS ] = {5 , 84 , 130 , 154  };

#define MP_BTN_STARTGAME	4
#define MP_BTN_READY		3


INT32 giMapMPButtonImage[ MAX_MP_BUTTONS+1 ] = { -1, -1, -1, -1 ,-1};
INT32 giMapMPButton[ MAX_MP_BUTTONS ] = { -1, -1, -1, -1};

// Compass Drowndown
INT32 ghMPCompassBox;
INT32 ghMPTeamBox;
MOUSE_REGION gMPReadyButtonRegion;
MOUSE_REGION gCompassMenuRegion[MAX_EDGES];
MOUSE_REGION gTeamMenuRegion[MAX_MP_TEAMS];
bool is_compass_box_open = false;
bool is_team_box_open = false;

#define RGB_WHITE				( FROMRGB( 255, 255, 255 ) )
#define RGB_YELLOW			( FROMRGB( 255, 255,	0 ) )
#define RGB_NEAR_BLACK	( FROMRGB(	0,	0,	1 ) )

// ENUMS
// ARM: NOTE that these map "events" are never actually saved in a player's game in any way
enum
{
	MAP_EVENT_NONE,
	MAP_EVENT_CLICK_SECTOR,
	MAP_EVENT_PLOT_PATH,
	MAP_EVENT_CANCEL_PATH,

#ifdef JA2BETAVERSION
	MAP_EVENT_VIEWAI
#endif
};

// STRUCTURES / TYPEDEFS


struct rgbcolor
{
	UINT8 ubRed;
	UINT8 ubGreen;
	UINT8 ubBlue;
};

typedef struct rgbcolor RGBCOLOR;


struct lineoftext
{
	STR16 pLineText;
	UINT32 uiFont;
	struct lineoftext *pNext;
};

typedef struct lineoftext LineText;
typedef LineText *LineTextPtr;


struct popbox
{
 UINT16 usTopX;
 UINT16 usTopY;
 UINT16 usWidth;
 UINT16 usHeight;
 LineTextPtr pBoxText;
 struct popbox *pNext;
};

typedef struct popbox PopUpBox;
typedef PopUpBox *PopUpBoxPtr;

// TABLES
RGBCOLOR GlowColorsA[]={
	{0,0,0},
	{25,0,0},
	{50,0,0},
	{75,0,0},
	{100,0,0},
	{125,0,0},
	{150,0,0},
	{175,0,0},
	{200,0,0},
	{225,0,0},
	{250,0,0},
};

SGPPoint gMapSortButtons[ MAX_SORT_METHODS ]={
	{5,113},
	{61,113},
	{117,113},
	{141,113},
	{178,113},
	{216,113},
};

// map screen's inventory panel pockets - top right corner coordinates
INV_REGION_DESC gMapScreenInvPocketXY[NUM_INV_SLOTS];	// ARRAY FOR INV PANEL INTERFACE ITEM POSITIONS
INV_REGION_DESC gSCamoXY;

void BtnMessageUpMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BeginSellAllCallBack( UINT8 bExitValue );
void BeginDeleteAllCallBack( UINT8 bExitValue );
BOOLEAN fScrollButtonsInitialized = FALSE;

BOOLEAN	fFlashAssignDone = FALSE;
BOOLEAN	fInMapMode = FALSE;
BOOLEAN fMapPanelDirty=TRUE;
BOOLEAN fTeamPanelDirty = TRUE;
BOOLEAN fCharacterInfoPanelDirty = TRUE;
BOOLEAN	gfLoadPending = FALSE;
BOOLEAN fReDrawFace=FALSE;
BOOLEAN fFirstTimeInMapScreen = TRUE;
BOOLEAN fShowInventoryFlag = FALSE;
BOOLEAN fMapInventoryItem=FALSE;
BOOLEAN fShowDescriptionFlag=FALSE;
BOOLEAN fResetMapCoords = FALSE;

// are the graphics for mapscreen preloaded?
BOOLEAN fPreLoadedMapGraphics = FALSE;

BOOLEAN gfHotKeyEnterSector = FALSE;
BOOLEAN fOneFrame = FALSE;
BOOLEAN fShowFaceHightLight = FALSE;
BOOLEAN fShowItemHighLight = FALSE;
BOOLEAN gfAllowSkyriderTooFarQuote = FALSE;
BOOLEAN fJustFinishedPlotting = FALSE;

// for the flashing of the contract departure time...for when mercs are leaving in an hour or less
BOOLEAN fFlashContractFlag = FALSE;

BOOLEAN fShowTrashCanHighLight = FALSE;

// the flags for display of pop up boxes/menus
BOOLEAN fEndPlotting = FALSE;

BOOLEAN gfInConfirmMapMoveMode = FALSE;
BOOLEAN gfInChangeArrivalSectorMode = FALSE;

// redraw character list
BOOLEAN fDrawCharacterList = TRUE;

// was the cursor set to the checkmark?
BOOLEAN fCheckCursorWasSet = FALSE;

BOOLEAN fShowingMapDisableBox = FALSE;
BOOLEAN fShowFrameRate = FALSE;
//BOOLEAN fMapExitDueToMessageBox = FALSE;
BOOLEAN fEndShowInventoryFlag = FALSE;

// draw the temp path
BOOLEAN fDrawTempPath = TRUE;

BOOLEAN gfCharacterListInited = FALSE;

BOOLEAN gfGlowTimerExpired = FALSE;

//not required to be saved.	The flag is set to allow mapscreen to render once, then transition the
//current tactical battle into autoresolve.
BOOLEAN gfTransitionMapscreenToAutoResolve = FALSE;

BOOLEAN gfSkyriderEmptyHelpGiven = FALSE;

BOOLEAN gfRequestGiveSkyriderNewDestination = FALSE;

BOOLEAN gfFirstMapscreenFrame = FALSE;

BOOLEAN gfMapPanelWasRedrawn = FALSE;


UINT8 gubMAP_HandInvDispText[ NUM_INV_SLOTS ];

// currently selected character's list index
INT8 bSelectedInfoChar = -1;

// map sort button images
INT32 giMapSortButtonImage[ MAX_SORT_METHODS ] = { -1, -1, -1, -1, -1, -1 };
INT32 giMapSortButton[ MAX_SORT_METHODS ] = { -1, -1, -1, -1, -1, -1 };

INT32 giCharInfoButtonImage[ 2 ];
INT32 giCharInfoButton[ 2 ] = { -1, -1 };

INT32 giMapInvButtonDoneImage;
INT32 giMapInvDoneButton = -1;

INT32 giMapContractButton = -1;
INT32 giMapContractButtonImage;

INT32 giSortStateForMapScreenList = 0;
BOOLEAN	giSortOrderAscending = TRUE;

INT32 giCommonGlowBaseTime = 0;
INT32 giFlashAssignBaseTime = 0;
INT32 giFlashContractBaseTime = 0;
UINT32 guiFlashCursorBaseTime = 0;
INT32 giPotCharPathBaseTime = 0;

extern UINT32 guiVObjectSize;
extern UINT32 guiVSurfaceSize;

extern UINT8 gubHandPos;
extern UINT16 gusOldItemIndex;
extern UINT16 gusNewItemIndex;
extern BOOLEAN gfDeductPoints;

extern void CleanUpStack( OBJECTTYPE * pObj, OBJECTTYPE * pCursorObj );
extern void SwapGoggles(SOLDIERTYPE *pTeamSoldier);
// HEADROCK HAM B2.8: Function to switch team's goggles uniformly
extern void SwapGogglesUniformly(SOLDIERTYPE *pTeamSoldier, BOOLEAN fToNightVision);

UINT32	guiCHARLIST;
UINT32	guiCHARINFO;
UINT32	guiSleepIcon;
UINT32	guiCROSS;
UINT32	guiMAPINV;
UINT32	guiMapInvSecondHandBlockout;
UINT32	guiULICONS;
UINT32	guiNewMailIcons;
UINT32	guiLEVELMARKER;		// the white rectangle highlighting the current level on the map border

// misc mouse regions
MOUSE_REGION gCharInfoFaceRegion;
MOUSE_REGION gCharInfoHandRegion;
MOUSE_REGION gMPanelRegion;
MOUSE_REGION gMapViewRegion;
MOUSE_REGION gMapScreenMaskRegion;
MOUSE_REGION gTrashCanRegion;

// mouse regions for team info panel
MOUSE_REGION gTeamListNameRegion[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
MOUSE_REGION gTeamListAssignmentRegion[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
MOUSE_REGION gTeamListSleepRegion[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
MOUSE_REGION gTeamListLocationRegion[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
MOUSE_REGION gTeamListDestinationRegion[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
MOUSE_REGION gTeamListContractRegion[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];

OBJECTTYPE		gItemPointer;
SOLDIERTYPE		*gpItemPointerSoldier;

PathStPtr gpCharacterPreviousMercPath[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
PathStPtr gpHelicopterPreviousMercPath = NULL;

// GLOBAL VARIABLES (EXTERNAL)
extern BOOLEAN fHoveringHelicopter;
extern BOOLEAN fDeletedNode;
extern BOOLEAN gfRenderPBInterface;
extern BOOLEAN fMapScreenBottomDirty;
extern BOOLEAN fResetTimerForFirstEntryIntoMapScreen;
extern BOOLEAN gfStartedFromMapScreen;

extern BOOLEAN gfUsePersistantPBI;

extern BOOLEAN gfOneFramePauseOnExit;

// the selected list of mercs
extern BOOLEAN fSelectedListOfMercsForMapScreen[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];

extern INT32 iDialogueBox;
extern INT32 giMapInvDescButton;

// HEADROCK HAM 4: Extern tab buttons for UDB
extern INT32 giInvDescTabButton[3];

extern UINT32 guiBrownBackgroundForTeamPanel;

// the town mine info box
extern INT32 ghTownMineBox;
// border and bottom buttons
extern INT32 giMapBorderButtons[];
extern UINT32 guiMapButtonInventory[];

// the mine icon
extern UINT32 guiMINEICON;
extern UINT32 guiSecItemHiddenVO;

extern UINT32	guiUIMessageTimeDelay;

extern PathStPtr pTempCharacterPath;
extern PathStPtr pTempHelicopterPath;

extern BOOLEAN gfAutoAIAware;
extern void HandlePreBattleInterfaceStates();

// the title for the contract button on the character info panel in the upper left portion of the mapscreen
extern STR16 pContractButtonString[];
extern STR16 pBullseyeStrings[];

extern OBJECTTYPE	*gpItemDescObject;

extern SOLDIERTYPE *pProcessingSoldier;

// faces stuff
extern FACETYPE	*gpCurrentTalkingFace;

// externs for highlighting of ammo/weapons
extern UINT32 guiMouseOverItemTime;
extern BOOLEAN gfCheckForMouseOverItem;
extern INT8 gbCheckForMouseOverItemPos;

//Autoresolve sets this variable which defaults to -1 when not needed.
extern INT16 gsEnemyGainedControlOfSectorID;
extern INT16 gsCiviliansEatenByMonsters;

extern BOOLEAN			gfFadeOutDone;

extern UINT32 guiPendingScreen;

extern CHAR16		gzUserDefinedButton1[ 128 ];
extern CHAR16		gzUserDefinedButton2[ 128 ];

extern BOOLEAN gfMilitiaPopupCreated;

#ifdef JA2TESTVERSION
	extern INT16 MSYS_CurrentMX;
	extern INT16 MSYS_CurrentMY;
#endif


// PROTOTYPES
// basic input
void GetMapKeyboardInput( UINT32 *puiNewEvent );
void PollLeftButtonInMapView( UINT32 *puiNewEvent );
void PollRightButtonInMapView( UINT32 *puiNewEvent );

// background render
void BlitBackgroundToSaveBuffer( void );

// Drawing the Map
UINT32 HandleMapUI( );
void RenderMapCursorsIndexesAnims( void );
BOOLEAN GetMapXY( INT16 sX, INT16 sY, INT16 *psMapWorldX, INT16 *psMapWorldY );
BOOLEAN GetMouseMapXY( INT16 *psMapWorldX, INT16 *psMapWorldY );

void StartConfirmMapMoveMode( INT16 sMapY );
void EndConfirmMapMoveMode( void );

void CancelMapUIMessage( void );
void MonitorMapUIMessage( void );

void RenderMapHighlight( INT16 sMapX, INT16 sMapY, UINT16 usLineColor, BOOLEAN fStationary );
void ShadeMapElem( INT16 sMapX, INT16 sMapY );
void PopupText( UINT16 *pFontString, ... );
void DrawString(STR16 pString, UINT16 uiX, UINT16 uiY, UINT32 uiFont);

// Clock
void SetClock(STR16 pString);
void SetClockMin(STR16, ...);
void SetClockHour(STR16 pStringA, ...);
void SetHourAlternate(STR16 pStringA, ...);
void SetDayAlternate(STR16 pStringA, ...);

void RenderIconsForUpperLeftCornerPiece( INT8 bCharNumber );
void RenderAttributeStringsForUpperLeftHandCorner( UINT32 uiBufferToRenderTo );

void DisplayThePotentialPathForCurrentDestinationCharacterForMapScreenInterface( INT16 sMapX, INT16 sMapY );
void HandleCursorOverRifleAmmo( );

void SetUpCursorForStrategicMap( void );
void HandleAnimatedCursorsForMapScreen( );
void CheckToSeeIfMouseHasLeftMapRegionDuringPathPlotting( );

// handle change in info char
void HandleChangeOfInfoChar( void );

// update town mine pop up
void UpdateTownMinePopUpDisplay( void );

// Map Buttons
void GlowFace( void );

void ResetMapButtons();

//Drawing Strings
void DrawName( STR16 pName, INT16 sRowIndex, INT32 iFont );
void DrawAssignment( INT16 sCharNumber, INT16 sRowIndex, INT32 iFont );
void DrawLocation( INT16 sCharNumber, INT16 sRowIndex, INT32 iFont );
void DrawDestination( INT16 sCharNumber, INT16 sRowIndex, INT32 iFont );
void DrawTimeRemaining( INT16 sCharNumber, INT32 iFont, UINT8 ubFontColor );

void HandleAssignmentsDoneAndAwaitingFurtherOrders( void );
void HandleCommonGlowTimer( void );

void CheckIfPlottingForCharacterWhileAirCraft( void );

// handle highlighting of team panel lines
void HandleChangeOfHighLightedLine( void );

// automatically pause/unpause time compression during certain events
void UpdatePausedStatesDueToTimeCompression( void );


// handle pre battle interface in relation to inventory
void HandlePreBattleInterfaceWithInventoryPanelUp( void );
void CreateDestroyMapCharacterScrollButtons( void );


// sort buttons for team list
void AddTeamPanelSortButtonsForMapScreen( void );
void RemoveTeamPanelSortButtonsForMapScreen( void );
void SortListOfMercsInTeamPanel( BOOLEAN fRetainSelectedMercs, BOOLEAN fReverse = FALSE );

// OJW - buttons and boxes for MP interface
void AddMPButtonsForMapScreen( void );
void RemoveMPButtonsForMapScreen( void );
void InitializeMPCoordinates( void );
void CreateMPCompassBox( void );
void DestroyMPCompassBox( void );
void CreateMPTeamBox( void );
void DestroyMPTeamBox( void );
void MPReadyButtonCallback( GUI_BUTTON *btn, INT32 reason );
void MPCompassChangeCallback( MOUSE_REGION * pReason, INT32 iReason);
void CreateDestroyMouseRegionsForCompassBox();
void CompassBoxMvtCallback ( MOUSE_REGION* pRegion, INT32 iReason);
void CompassBoxBtnCallback ( MOUSE_REGION* pRegion, INT32 iReason);
void MPTeamChangeCallback( MOUSE_REGION * pReason, INT32 iReason);
void CreateDestroyMouseRegionsForTeamBox();
void TeamBoxMvtCallback ( MOUSE_REGION* pRegion, INT32 iReason);
void TeamBoxBtnCallback ( MOUSE_REGION* pRegion, INT32 iReason);

// Rendering
void RenderCharacterInfoBackground( void );
void RenderTeamRegionBackground( void );
void RenderMapRegionBackground( void );
void HandleHighLightingOfLinesInTeamPanel( void );
void ClearTeamPanel();
void PlotTemporaryPaths( void );
void PlotPermanentPaths( void );
void RenderHandPosItem( void );
void ContractListRegionBoxGlow( UINT16 usCount );
void DisplayDestinationOfCurrentDestMerc( void );
void HandleCharBarRender( void );

// rebuild waypoints for selected character list
void RebuildWayPointsForAllSelectedCharsGroups( void );

extern BOOLEAN HandleNailsVestFetish( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, UINT16 usReplaceItem );

BOOLEAN CharacterIsInTransitAndHasItemPickedUp( INT8 bCharacterNumber );

void InterruptTimeForMenus( void );

// Pop Up Boxes
void CreateAttributeBox( void );
void CreateVehicleBox( void );
void CreateContractBox( SOLDIERTYPE *pCharacter );
void CreateAssignmentsBox( void );
void CreateTrainingBox( void );
void CreateMercRemoveAssignBox( void );
// HEADROCK HAM 3.6: Facility box, Sub-menu
void CreateFacilityBox( void );
void CreateFacilityAssignmentBox( void );

void DetermineWhichAssignmentMenusCanBeShown( void );
void DetermineWhichMilitiaControlMenusCanBeShown( void ); //lal

void DetermineIfContractMenuCanBeShown( void );
void ContractRegionBtnCallback( MOUSE_REGION * pRegion, INT32 iReason );
void ContractRegionMvtCallback( MOUSE_REGION * pRegion, INT32 iReason );
void HandleShadingOfLinesForContractMenu( void );

void UpdateStatusOfMapSortButtons( void );

void DisplayIconsForMercsAsleep( void );
BOOLEAN CharacterIsInLoadedSectorAndWantsToMoveInventoryButIsNotAllowed( INT8 bCharId );

void HandlePostAutoresolveMessages();

// screen mask for pop up menus
void ClearScreenMaskForMapScreenExit( void );

void ResetAllSelectedCharacterModes( void );

// The Mouse/Btn Creation/Destruction

// destroy regions/buttons
void DestroyMouseRegionsForTeamList( void );

// create new regions/buttons
void CreateMouseRegionsForTeamList( void );

void EnableDisableTeamListRegionsAndHelpText( void );

// merc about to leave, flash contract time
BOOLEAN AnyMercsLeavingRealSoon();
void HandleContractTimeFlashForMercThatIsAboutLeave( void );

// Mouse Region Callbacks

// team list
void TeamListInfoRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason );
void TeamListInfoRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason );
void TeamListAssignmentRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason );
void TeamListAssignmentRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason );
void TeamListDestinationRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason );
void TeamListDestinationRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason );
void TeamListContractRegionBtnCallBack( MOUSE_REGION *pRegion, INT32 iReason );
void TeamListContractRegionMvtCallBack( MOUSE_REGION *pRegion, INT32 iReason );
void TeamListSleepRegionBtnCallBack( MOUSE_REGION *pRegion, INT32 iReason );
void TeamListSleepRegionMvtCallBack( MOUSE_REGION *pRegion, INT32 iReason );

void FaceRegionBtnCallback( MOUSE_REGION *pRegion, INT32 iReason );
void FaceRegionMvtCallback( MOUSE_REGION *pRegion, INT32 iReason );

void ItemRegionBtnCallback( MOUSE_REGION *pRegion, INT32 iReason );
void ItemRegionMvtCallback( MOUSE_REGION *pRegion, INT32 iReason );

// mapscreen mouse region screen mask btn callback
void MapScreenMarkRegionBtnCallback(MOUSE_REGION *pRegion, INT32 iReason );

// inventory mvt and click callbacks
void MAPInvMoveCallback( MOUSE_REGION *pRegion, INT32 iReason );
void MAPInvClickCallback( MOUSE_REGION *pRegion, INT32 iReason );
void MAPInvClickCamoCallback( MOUSE_REGION *pRegion, INT32 iReason );
void MAPInvMoveCamoCallback( MOUSE_REGION *pRegion, INT32 iReason );

void InvmaskRegionBtnCallBack( MOUSE_REGION *pRegion, INT32 iReason );
void TrashCanBtnCallback( MOUSE_REGION *pRegion, INT32 iReason);

extern void KeyRingItemPanelButtonCallback( MOUSE_REGION *pRegion, INT32 iReason );

// handle talking
void HandleSpontanousTalking( void );
BOOLEAN ContinueDialogue(SOLDIERTYPE *pSoldier, BOOLEAN fDone );
extern void CreateDestroyMilitiaSectorButtons( void );

// mouse position test
BOOLEAN IsCursorWithInRegion(INT16 sLeft, INT16 sRight, INT16 sTop, INT16 sBottom );

// pop up boxes
void CreateVehicleBox( );

void TestMessageSystem( void );

BOOLEAN CheckIfClickOnLastSectorInPath( INT16 sX, INT16 sY );

//update any bad assignments..error checking
void UpdateBadAssignments( void );

// hwo many on team, if less than 2, disable prev/next merc buttons
void UpdateTheStateOfTheNextPrevMapScreenCharacterButtons( void );

// inventory
void CreateDestroyTrashCanRegion( void );
void DoneInventoryMapBtnCallback( GUI_BUTTON *btn,INT32 reason );

// handle cursor for invenetory mode..update to object selected, if needed
void HandleMapInventoryCursor( );
void MAPEndItemPointer( );
void MAPBeginItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubHandPos );

// create/destroy inventory button as needed
void CreateDestroyMapInvButton();
void PrevInventoryMapBtnCallback( GUI_BUTTON *btn, INT32 reason );
void NextInventoryMapBtnCallback( GUI_BUTTON *btn, INT32 reason );

// check if cursor needs to be set to checkmark or to the walking guy?
void UpdateCursorIfInLastSector( void );

void ContractButtonCallback(GUI_BUTTON *btn,INT32 reason);
void MapScreenDefaultOkBoxCallback( UINT8 bExitValue );

// blt inventory panel
void BltCharInvPanel();

// show character information
void DrawCharacterInfo(INT16 sCharNumber);
void DisplayCharacterInfo( void );
void UpDateStatusOfContractBox( void );
void DrawMPPlayerList (); // OJW - 20081201

// get which index in the mapscreen character list is this guy
INT32 GetIndexForthis( SOLDIERTYPE *pSoldier );

void CheckForAndRenderNewMailOverlay();

BOOLEAN MapCharacterHasAccessibleInventory( INT8 bCharNumber );
void CheckForInventoryModeCancellation();

void ChangeMapScreenMaskCursor( UINT16 usCursor );
void CancelOrShortenPlottedPath( void );

// HEADROCK HAM B2.8: Added argument to enable multi-selecting entire squads
BOOLEAN HandleCtrlOrShiftInTeamPanel( INT8 bCharNumber, BOOLEAN fFromRightClickAssignments );

INT32 GetContractExpiryTime( SOLDIERTYPE *pSoldier );

void ConvertMinTimeToETADayHourMinString( UINT32 uiTimeInMin, STR16 sString );
INT32 GetGroundTravelTimeOfCharacter( INT8 bCharNumber );

INT16 CalcLocationValueForChar( INT32 iCounter );

void CancelChangeArrivalSectorMode( );

void MakeMapModesSuitableForDestPlotting( INT8 bCharNumber );

BOOLEAN AnyMovableCharsInOrBetweenThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void SwapCharactersInList( INT32 iCharA, INT32 iCharB );

BOOLEAN CanChangeDestinationForCharSlot( INT8 bCharNumber, BOOLEAN fShowErrorMessage );

BOOLEAN RequestGiveSkyriderNewDestination( void );
void ExplainWhySkyriderCantFly( void );
UINT8 PlayerMercsInHelicopterSector( void );

void HandleNewDestConfirmation( INT16 sMapX, INT16 sMapY );
void RandomAwakeSelectedMercConfirmsStrategicMove( void );
void DestinationPlottingCompleted( void );

void HandleMilitiaRedistributionClick( void );

void StartChangeSectorArrivalMode( void );
BOOLEAN CanMoveBullseyeAndClickedOnIt( INT16 sMapX, INT16 sMapY );
void CreateBullsEyeOrChopperSelectionPopup( void );
void BullsEyeOrChopperSelectionPopupCallback( UINT8 ubExitValue );

void WakeUpAnySleepingSelectedMercsOnFootOrDriving( void );

void GetMapscreenMercAssignmentString( SOLDIERTYPE *pSoldier, CHAR16 sString[] );
void GetMapscreenMercLocationString( SOLDIERTYPE *pSoldier, CHAR16 sString[] );
void GetMapscreenMercDestinationString( SOLDIERTYPE *pSoldier, CHAR16 sString[] );
void GetMapscreenMercDepartureString( SOLDIERTYPE *pSoldier, CHAR16 sString[], UINT8 *pubFontColor );

void InitPreviousPaths( void );
void RememberPreviousPathForAllSelectedChars( void );
void RestorePreviousPaths( void );
void ClearPreviousPaths( void );

void SelectAllCharactersInSquad( INT8 bSquadNumber );

BOOLEAN CanDrawSectorCursor( void );
void RestoreMapSectorCursor( INT16 sMapX, INT16 sMapY );

void RequestToggleMercInventoryPanel( void );

void RequestContractMenu( void );
void ChangeCharacterListSortMethod( INT32 iValue );

void MapscreenMarkButtonsDirty();


extern BOOLEAN CanRedistributeMilitiaInSector( INT16 sClickedSectorX, INT16 sClickedSectorY, INT8 bClickedTownId );

extern INT32 GetNumberOfMercsInUpdateList( void );
extern INT32 SellItem( OBJECTTYPE& object, BOOLEAN useModifier = TRUE );
void DeleteAllItemsInInventoryPool();


#ifdef JA2TESTVERSION
void TestDumpStatChanges( void );
void DumpSectorDifficultyInfo( void );
void DumpItemsList( void );
#endif

BOOLEAN IsMercInActiveSector(SOLDIERTYPE * pSoldier)
{
	if(pSoldier->sSectorX != sSelMapX)
		return( FALSE );
	if(pSoldier->sSectorY != sSelMapY)
		return( FALSE );
	if(pSoldier->bSectorZ != iCurrentMapSectorZ)
		return( FALSE );

	return( TRUE );
}

void BeginSellAllCallBack( UINT8 bExitValue )
{
	INT32	iPrice = 0;
	bool	anythingSold = false;

	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		fRestoreBackgroundForMessageBox = TRUE;
		if ( gpItemPointer != NULL || InSectorStackPopup( ) || InItemStackPopup( ) || InItemDescriptionBox( ) || InKeyRingPopup( ) )
		{
			return;
		}
		for(UINT32 wItem = 0; wItem < pInventoryPoolList.size(); wItem++)	//CHRISL: It's safer to use pInventoryPoolList.size() versus guiNumWorldItems as it avoids any possibility of a vector size error
		{
			if(pInventoryPoolList[wItem].object.exists() == true && (pInventoryPoolList[wItem].usFlags & WORLD_ITEM_REACHABLE))
			{
				if(0 == pInventoryPoolList[wItem].object.MoveThisObjectTo(gItemPointer,-1,0,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT))
			{
				if (pInventoryPoolList[wItem].object.exists() == false) {
					pInventoryPoolList[wItem].object.initialize();
				}
				// Dirty interface
				fMapPanelDirty = TRUE;
				gpItemPointer = &gItemPointer;
				// Sell Item
				iPrice += SellItem( gItemPointer );
				gpItemPointer = NULL;
				fMapInventoryItem = FALSE;
				if (iPrice == 0) {
					iPrice = 1;
				}
				anythingSold = true;
				HandleButtonStatesWhileMapInventoryActive();
				}
			}
		}
		if(anythingSold == true)
			AddTransactionToPlayersBook( SOLD_ITEMS, 0, GetWorldTotalMin(), iPrice );
	}
}

void BeginDeleteAllCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		fRestoreBackgroundForMessageBox = TRUE;
		if ( gpItemPointer != NULL || InSectorStackPopup( ) || InItemStackPopup( ) || InItemDescriptionBox( ) || InKeyRingPopup( ) )
		{
			return;
		}
		for(UINT32 wItem = 0; wItem < guiNumWorldItems; wItem++) //WS TODO: is "for(UINT32 wItem = 0; wItem < pInventoryPoolList.size(); wItem++)"
		{
			if(0 == pInventoryPoolList[wItem].object.MoveThisObjectTo(gItemPointer,-1,0,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT))
			{
				if (pInventoryPoolList[wItem].object.exists() == false) {
					pInventoryPoolList[wItem].object.initialize();
				}
				// Dirty interface
				fMapPanelDirty = TRUE;
				gpItemPointer = &gItemPointer;
				// Delete Item
				gpItemPointer = NULL;
				fMapInventoryItem = FALSE;
			}
		}
	}
}

// CHRISL: New functions to handle initialization of inventory coordinates
BOOLEAN InitializeInvPanelCoordsOld()
{
	TOWN_INFO_X						= 0;
	TOWN_INFO_Y						= 1;

	PLAYER_INFO_X					= 0;
	PLAYER_INFO_Y					= 107;

	// Inventory slots
	gMapScreenInvPocketXY[HELMETPOS].sX = PLAYER_INFO_X + 204;		gMapScreenInvPocketXY[HELMETPOS].sY = PLAYER_INFO_Y + 9;	// HELMETPOS
	gMapScreenInvPocketXY[VESTPOS].sX = PLAYER_INFO_X + 204;		gMapScreenInvPocketXY[VESTPOS].sY = PLAYER_INFO_Y + 38;	// VESTPOS
	gMapScreenInvPocketXY[LEGPOS].sX = PLAYER_INFO_X + 204;			gMapScreenInvPocketXY[LEGPOS].sY = PLAYER_INFO_Y + 98;	// LEGPOS,
	gMapScreenInvPocketXY[HEAD1POS].sX = PLAYER_INFO_X + 21;		gMapScreenInvPocketXY[HEAD1POS].sY = PLAYER_INFO_Y + 9;	// HEAD1POS
	gMapScreenInvPocketXY[HEAD2POS].sX = PLAYER_INFO_X + 21;		gMapScreenInvPocketXY[HEAD2POS].sY = PLAYER_INFO_Y + 33;	// HEAD2POS
	gMapScreenInvPocketXY[HANDPOS].sX = PLAYER_INFO_X + 21;			gMapScreenInvPocketXY[HANDPOS].sY = PLAYER_INFO_Y + 87;	// HANDPOS,
	gMapScreenInvPocketXY[SECONDHANDPOS].sX = PLAYER_INFO_X + 21;	gMapScreenInvPocketXY[SECONDHANDPOS].sY = PLAYER_INFO_Y + 111;	// SECONDHANDPOS
	gMapScreenInvPocketXY[BIGPOCK1POS].sX = PLAYER_INFO_X + 98;		gMapScreenInvPocketXY[BIGPOCK1POS].sY = PLAYER_INFO_Y + 144;	// BIGPOCK1
	gMapScreenInvPocketXY[BIGPOCK2POS].sX = PLAYER_INFO_X + 98;		gMapScreenInvPocketXY[BIGPOCK2POS].sY = PLAYER_INFO_Y + 168;	// BIGPOCK2
	gMapScreenInvPocketXY[BIGPOCK3POS].sX = PLAYER_INFO_X + 98;		gMapScreenInvPocketXY[BIGPOCK3POS].sY = PLAYER_INFO_Y + 192;	// BIGPOCK3
	gMapScreenInvPocketXY[BIGPOCK4POS].sX = PLAYER_INFO_X + 98;		gMapScreenInvPocketXY[BIGPOCK4POS].sY = PLAYER_INFO_Y + 216;	// BIGPOCK4
	gMapScreenInvPocketXY[SMALLPOCK1POS].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[SMALLPOCK1POS].sY = PLAYER_INFO_Y + 144;	// SMALLPOCK1
	gMapScreenInvPocketXY[SMALLPOCK2POS].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[SMALLPOCK2POS].sY = PLAYER_INFO_Y + 168;	// SMALLPOCK2
	gMapScreenInvPocketXY[SMALLPOCK3POS].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[SMALLPOCK3POS].sY = PLAYER_INFO_Y + 192;	// SMALLPOCK3
	gMapScreenInvPocketXY[SMALLPOCK4POS].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[SMALLPOCK4POS].sY = PLAYER_INFO_Y + 216;	// SMALLPOCK4
	gMapScreenInvPocketXY[SMALLPOCK5POS].sX = PLAYER_INFO_X + 60;	gMapScreenInvPocketXY[SMALLPOCK5POS].sY = PLAYER_INFO_Y + 144;	// SMALLPOCK5
	gMapScreenInvPocketXY[SMALLPOCK6POS].sX = PLAYER_INFO_X + 60;	gMapScreenInvPocketXY[SMALLPOCK6POS].sY = PLAYER_INFO_Y + 168;	// SMALLPOCK6
	gMapScreenInvPocketXY[SMALLPOCK7POS].sX = PLAYER_INFO_X + 60;	gMapScreenInvPocketXY[SMALLPOCK7POS].sY = PLAYER_INFO_Y + 192;	// SMALLPOCK7
	gMapScreenInvPocketXY[SMALLPOCK8POS].sX = PLAYER_INFO_X + 60;	gMapScreenInvPocketXY[SMALLPOCK8POS].sY = PLAYER_INFO_Y + 216;	// SMALLPOCK8

	// item description
	MAP_ITEMDESC_START_X			= 0;
	MAP_ITEMDESC_START_Y			= PLAYER_INFO_Y;

	INV_REGION_X					= PLAYER_INFO_X;
	INV_REGION_Y					= PLAYER_INFO_Y;
	INV_REGION_WIDTH				= 261;
	INV_REGION_HEIGHT				= 359-94;
	INV_BTN_X						= PLAYER_INFO_X + 217;
	INV_BTN_Y						= PLAYER_INFO_Y + 210;
	INV_BDROP_X						= 0;	// Not used in old inventory
	INV_BDROP_Y						= 0;	// Not used in old inventory

	MAP_ARMOR_LABEL_X				= 208;
	MAP_ARMOR_LABEL_Y				= 180;
	MAP_ARMOR_X						= 209;
	MAP_ARMOR_Y						= 189;
	MAP_ARMOR_PERCENT_X				= 229;
	MAP_ARMOR_PERCENT_Y				= 190;

	MAP_WEIGHT_LABEL_X				= 173;
	MAP_WEIGHT_LABEL_Y				= 256;
	MAP_WEIGHT_X					= 176;
	MAP_WEIGHT_Y					= 266;
	MAP_WEIGHT_PERCENT_X			= 196;
	MAP_WEIGHT_PERCENT_Y			= 266;

	MAP_CAMMO_LABEL_X				= 178;
	MAP_CAMMO_LABEL_Y				= 283;
	MAP_CAMMO_X						= 176;
	MAP_CAMMO_Y						= 292;
	MAP_CAMMO_PERCENT_X				= 196;
	MAP_CAMMO_PERCENT_Y				= 293;

	MAP_PERCENT_WIDTH				= 20;
	MAP_PERCENT_HEIGHT				= 10;

	MAP_INV_STATS_TITLE_FONT_COLOR	= 6;
	MAP_INV_STATS_TEXT_FONT_COLOR	= 5;

	PLAYER_INFO_FACE_START_X		= 9;
	PLAYER_INFO_FACE_START_Y		= 17;
	PLAYER_INFO_FACE_END_X			= 60;
	PLAYER_INFO_FACE_END_Y			= 76;

	INV_BODY_X						= 71;
	INV_BODY_Y						= 116;

	NAME_X							= 4;
	NAME_WIDTH						= 55 - NAME_X;

	ASSIGN_X						= 60;
	ASSIGN_WIDTH					= 111 - ASSIGN_X;
	SLEEP_X							= 116;
	SLEEP_WIDTH						= 135 - SLEEP_X;
	LOC_X							= 140;
	LOC_WIDTH						= 172 - LOC_X;
	DEST_ETA_X						= 177;
	DEST_ETA_WIDTH					= 210 - DEST_ETA_X;
	TIME_REMAINING_X				= 215;
	TIME_REMAINING_WIDTH			= 243 - TIME_REMAINING_X;

	if (iResolution == 0)
	{
		CLOCK_Y_START					= (MAP_BORDER_Y_OFFSET + 298);

		DEST_PLOT_X						= (MAP_BORDER_X_OFFSET + 463);
		DEST_PLOT_Y						= (MAP_BORDER_Y_OFFSET + 345);

		CLOCK_ETA_X						= (MAP_BORDER_X_OFFSET + 484);
		CLOCK_HOUR_X_START				= (MAP_BORDER_X_OFFSET + 518);
		CLOCK_MIN_X_START				= (MAP_BORDER_X_OFFSET + 538);
	}
	else if (iResolution == 1)
	{
		CLOCK_Y_START					= (MAP_BORDER_Y_OFFSET + 298 + 120);

		DEST_PLOT_X						= (MAP_BORDER_X_OFFSET + 463 + 80);
		DEST_PLOT_Y						= (MAP_BORDER_Y_OFFSET + 345 + 120);

		CLOCK_ETA_X						= (MAP_BORDER_X_OFFSET + 484 + 80);
		CLOCK_HOUR_X_START				= (MAP_BORDER_X_OFFSET + 518 + 80);
		CLOCK_MIN_X_START				= (MAP_BORDER_X_OFFSET + 538 + 80);
	}
	else if (iResolution == 2)
	{
		CLOCK_Y_START					= (MAP_BORDER_Y_OFFSET + 298 + 285);

		DEST_PLOT_X						= (MAP_BORDER_X_OFFSET + 463 + 180);
		DEST_PLOT_Y						= (MAP_BORDER_Y_OFFSET + 345 + 285);

		CLOCK_ETA_X						= (MAP_BORDER_X_OFFSET + 484 + 180);
		CLOCK_HOUR_X_START				= (MAP_BORDER_X_OFFSET + 518 + 180);
		CLOCK_MIN_X_START				= (MAP_BORDER_X_OFFSET + 538 + 180);
	}

	// -----------------

	// contract
	CONTRACT_X						= 185;
	CONTRACT_Y						= 50;

	// trash can
	TRASH_CAN_X						= 176;
	TRASH_CAN_Y						= 211 + PLAYER_INFO_Y;
	TRASH_CAN_WIDTH					= 193 - 165;
	TRASH_CAN_HEIGHT				= 239 - 217;

	// keyring
	MAP_KEYRING_X					= 217;
	MAP_KEYRING_Y					= 271;

	//Text offsets
	Y_OFFSET						= 2;

	// The boxes defines
	TRAIN_Y_OFFSET					= 53;
	TRAIN_X_OFF						= 65;
	TRAIN_WID						= 80;
	TRAIN_HEIG						= 47;
	STRING_X_OFFSET					= 10;
	STRING_Y_OFFSET					= 5;
	POP_UP_BOX_X					= 120;
	POP_UP_BOX_Y					= 0;
	POP_UP_BOX_WIDTH				= 60;
	POP_UP_BOX_HEIGHT				= 100;
	MOUSE_PTR_Y_OFFSET				= 3;
	POP_UP_Y_OFFSET					= 3;
	TRAIN_TEXT_Y_OFFSET				= 4;

	// char stat positions
	STR_X							= 112;
	STR_Y							= 42;
	DEX_X							= STR_X;
	DEX_Y							= 32;
	AGL_X							= STR_X;
	AGL_Y							= 22;
	LDR_X							= STR_X;
	LDR_Y							= 52;
	WIS_X							= STR_X;
	WIS_Y							= 62;
	LVL_X							= 159;
	LVL_Y							= AGL_Y;
	MRK_X							= LVL_X;
	MRK_Y							= DEX_Y;
	EXP_X							= LVL_X;
	EXP_Y							= STR_Y;
	MEC_X							= LVL_X;
	MEC_Y							= LDR_Y;
	MED_X							= LVL_X;
	MED_Y							= WIS_Y;

	STAT_WID						= 15;
	STAT_HEI						= GetFontHeight(CHAR_FONT);

	PIC_NAME_X						= 8;
	PIC_NAME_Y						= (66 + 3);
	PIC_NAME_WID					= 60 - PIC_NAME_X;
	PIC_NAME_HEI					= 75 - PIC_NAME_Y;
	CHAR_NAME_X						= 14;
	CHAR_NAME_Y						= (2 + 3);
	CHAR_NAME_WID					= 164 - CHAR_NAME_X;
	CHAR_NAME_HEI					= 11 - CHAR_NAME_Y;
	CHAR_LOC_X						= 76;
	CHAR_LOC_Y						= 84;
	CHAR_LOC_WID					= 16;
	CHAR_LOC_HEI					= 9;
	CHAR_TIME_REMAINING_X			= 207;
	CHAR_TIME_REMAINING_Y			= 65;
	CHAR_TIME_REMAINING_WID			= 258 - CHAR_TIME_REMAINING_X;
	CHAR_TIME_REMAINING_HEI			= GetFontHeight(CHAR_FONT);
	CHAR_SALARY_X					= CHAR_TIME_REMAINING_X;
	CHAR_SALARY_Y					= 79;
	CHAR_SALARY_WID					= CHAR_TIME_REMAINING_WID - 8;
	CHAR_SALARY_HEI					= CHAR_TIME_REMAINING_HEI;
	CHAR_MEDICAL_X					= CHAR_TIME_REMAINING_X;
	CHAR_MEDICAL_Y					= 93;
	CHAR_MEDICAL_WID				= CHAR_TIME_REMAINING_WID - 8;
	CHAR_MEDICAL_HEI				= CHAR_TIME_REMAINING_HEI;
	CHAR_ASSIGN_X					= 182;
	CHAR_ASSIGN1_Y					= 18;
	CHAR_ASSIGN2_Y					= 31;
	CHAR_ASSIGN_WID					= 257 - 178;
	CHAR_ASSIGN_HEI					= 39 - 29;
	CHAR_HP_X						= 133;
	CHAR_HP_Y						= 77 + 3;
	CHAR_HP_WID						= 175 - CHAR_HP_X;
	CHAR_HP_HEI						= 90 - CHAR_HP_Y;
	CHAR_MORALE_X					= 133;
	CHAR_MORALE_Y					= 91 + 3;
	CHAR_MORALE_WID					= 175 - CHAR_MORALE_X;
	CHAR_MORALE_HEI					= 101 - CHAR_MORALE_Y;

	CROSS_X							= 195;
	CROSS_Y							= 83;
	CROSS_HEIGHT					= 20;
	CROSS_WIDTH						= 20;
	CHAR_PAY_X						= 150;
	CHAR_PAY_Y						= 80+4;
	CHAR_PAY_HEI					= GetFontHeight(CHAR_FONT);
	CHAR_PAY_WID					= CROSS_X-CHAR_PAY_X;
	SOLDIER_PIC_X					= 9;
	SOLDIER_PIC_Y					= 20;
	SOLDIER_HAND_X					= 6;
	SOLDIER_HAND_Y					= 81;

	//OJW - MP interface changes
	InitializeMPCoordinates();
	gSCamoXY.sX = INV_BODY_X;	gSCamoXY.sY = INV_BODY_Y;	// X, Y Location of Map screen's Camouflage region

	return ( TRUE );
}
BOOLEAN InitializeInvPanelCoordsNew()
{
	TOWN_INFO_X						= 0;
	TOWN_INFO_Y						= 1;

	PLAYER_INFO_X					= 0;
	PLAYER_INFO_Y					= 107;

	//InitInventoryNew();

	// Inventory slots
	if(iResolution == 0){
		gMapScreenInvPocketXY[0].sX = PLAYER_INFO_X + 131;	gMapScreenInvPocketXY[0].sY = PLAYER_INFO_Y + 9;		// HELMETPOS
		gMapScreenInvPocketXY[1].sX = PLAYER_INFO_X + 131;	gMapScreenInvPocketXY[1].sY = PLAYER_INFO_Y + 38;		// VESTPOS
		gMapScreenInvPocketXY[2].sX = PLAYER_INFO_X + 131;	gMapScreenInvPocketXY[2].sY = PLAYER_INFO_Y + 97;		// LEGPOS
		gMapScreenInvPocketXY[3].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[3].sY = PLAYER_INFO_Y + 9;		// HEAD1POS
		gMapScreenInvPocketXY[4].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[4].sY = PLAYER_INFO_Y + 32;		// HEAD2POS
		gMapScreenInvPocketXY[5].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[5].sY = PLAYER_INFO_Y + 86;		// HANDPOS
		gMapScreenInvPocketXY[6].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[6].sY = PLAYER_INFO_Y + 110;		// SECONDHANDPOS
		gMapScreenInvPocketXY[7].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[7].sY = PLAYER_INFO_Y + 134;		// VESTPOCK
		gMapScreenInvPocketXY[8].sX = PLAYER_INFO_X + 29;	gMapScreenInvPocketXY[8].sY = PLAYER_INFO_Y + 197;		// LTHIGHPOCK
		gMapScreenInvPocketXY[9].sX = PLAYER_INFO_X + 112;	gMapScreenInvPocketXY[9].sY = PLAYER_INFO_Y + 197;		// RTHIGHPOCK
		gMapScreenInvPocketXY[10].sX = PLAYER_INFO_X + 194;	gMapScreenInvPocketXY[10].sY = PLAYER_INFO_Y + 197;		// CPACKPOCK
		gMapScreenInvPocketXY[11].sX = PLAYER_INFO_X + 173;	gMapScreenInvPocketXY[11].sY = PLAYER_INFO_Y + 332;		// BPACKPOCK
		gMapScreenInvPocketXY[12].sX = PLAYER_INFO_X + 95;	gMapScreenInvPocketXY[12].sY = PLAYER_INFO_Y + 134;		// GUNSLINGPOCKPOS
		gMapScreenInvPocketXY[13].sX = PLAYER_INFO_X + 36;	gMapScreenInvPocketXY[13].sY = PLAYER_INFO_Y + 134;		// KNIFEPOCKPOS
		gMapScreenInvPocketXY[14].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[14].sY = PLAYER_INFO_Y + 260;		// BIGPOCK1
		gMapScreenInvPocketXY[15].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[15].sY = PLAYER_INFO_Y + 281;		// BIGPOCK2
		gMapScreenInvPocketXY[16].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[16].sY = PLAYER_INFO_Y + 302;		// BIGPOCK3
		gMapScreenInvPocketXY[17].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[17].sY = PLAYER_INFO_Y + 281;		// BIGPOCK4
		gMapScreenInvPocketXY[18].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[18].sY = PLAYER_INFO_Y + 302;		// BIGPOCK5
		gMapScreenInvPocketXY[19].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[19].sY = PLAYER_INFO_Y + 323;		// BIGPOCK6
		gMapScreenInvPocketXY[20].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[20].sY = PLAYER_INFO_Y + 344;		// BIGPOCK7
		gMapScreenInvPocketXY[21].sX = PLAYER_INFO_X + 202;	gMapScreenInvPocketXY[21].sY = PLAYER_INFO_Y + 155;		// MEDPOCK1
		gMapScreenInvPocketXY[22].sX = PLAYER_INFO_X + 202;	gMapScreenInvPocketXY[22].sY = PLAYER_INFO_Y + 176;		// MEDPOCK2
		gMapScreenInvPocketXY[23].sX = PLAYER_INFO_X + 29;	gMapScreenInvPocketXY[23].sY = PLAYER_INFO_Y + 260;		// MEDPOCK3
		gMapScreenInvPocketXY[24].sX = PLAYER_INFO_X + 111;	gMapScreenInvPocketXY[24].sY = PLAYER_INFO_Y + 260;		// MEDPOCK4
		gMapScreenInvPocketXY[25].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[25].sY = PLAYER_INFO_Y + 155;		// SMALLPOCK1
		gMapScreenInvPocketXY[26].sX = PLAYER_INFO_X + 58;	gMapScreenInvPocketXY[26].sY = PLAYER_INFO_Y + 155;		// SMALLPOCK2
		gMapScreenInvPocketXY[27].sX = PLAYER_INFO_X + 94;	gMapScreenInvPocketXY[27].sY = PLAYER_INFO_Y + 155;		// SMALLPOCK3
		gMapScreenInvPocketXY[28].sX = PLAYER_INFO_X + 130;	gMapScreenInvPocketXY[28].sY = PLAYER_INFO_Y + 155;		// SMALLPOCK4
		gMapScreenInvPocketXY[29].sX = PLAYER_INFO_X + 166;	gMapScreenInvPocketXY[29].sY = PLAYER_INFO_Y + 155;		// SMALLPOCK5
		gMapScreenInvPocketXY[30].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[30].sY = PLAYER_INFO_Y + 176;		// SMALLPOCK6
		gMapScreenInvPocketXY[31].sX = PLAYER_INFO_X + 58;	gMapScreenInvPocketXY[31].sY = PLAYER_INFO_Y + 176;		// SMALLPOCK7
		gMapScreenInvPocketXY[32].sX = PLAYER_INFO_X + 94;	gMapScreenInvPocketXY[32].sY = PLAYER_INFO_Y + 176;		// SMALLPOCK8	
		gMapScreenInvPocketXY[33].sX = PLAYER_INFO_X + 130;	gMapScreenInvPocketXY[33].sY = PLAYER_INFO_Y + 176;		// SMALLPOCK9
		gMapScreenInvPocketXY[34].sX = PLAYER_INFO_X + 166;	gMapScreenInvPocketXY[34].sY = PLAYER_INFO_Y + 176;		// SMALLPOCK10
		gMapScreenInvPocketXY[35].sX = PLAYER_INFO_X + 18;	gMapScreenInvPocketXY[35].sY = PLAYER_INFO_Y + 218;		// SMALLPOCK11
		gMapScreenInvPocketXY[36].sX = PLAYER_INFO_X + 54;	gMapScreenInvPocketXY[36].sY = PLAYER_INFO_Y + 218;		// SMALLPOCK12
		gMapScreenInvPocketXY[37].sX = PLAYER_INFO_X + 18;	gMapScreenInvPocketXY[37].sY = PLAYER_INFO_Y + 239;		// SMALLPOCK13
		gMapScreenInvPocketXY[38].sX = PLAYER_INFO_X + 54;	gMapScreenInvPocketXY[38].sY = PLAYER_INFO_Y + 239;		// SMALLPOCK14
		gMapScreenInvPocketXY[39].sX = PLAYER_INFO_X + 100;	gMapScreenInvPocketXY[39].sY = PLAYER_INFO_Y + 218;		// SMALLPOCK15
		gMapScreenInvPocketXY[40].sX = PLAYER_INFO_X + 136;	gMapScreenInvPocketXY[40].sY = PLAYER_INFO_Y + 218;		// SMALLPOCK16
		gMapScreenInvPocketXY[41].sX = PLAYER_INFO_X + 100;	gMapScreenInvPocketXY[41].sY = PLAYER_INFO_Y + 239;		// SMALLPOCK17
		gMapScreenInvPocketXY[42].sX = PLAYER_INFO_X + 136;	gMapScreenInvPocketXY[42].sY = PLAYER_INFO_Y + 239;		// SMALLPOCK18
		gMapScreenInvPocketXY[43].sX = PLAYER_INFO_X + 183;	gMapScreenInvPocketXY[43].sY = PLAYER_INFO_Y + 218;		// SMALLPOCK19
		gMapScreenInvPocketXY[44].sX = PLAYER_INFO_X + 219;	gMapScreenInvPocketXY[44].sY = PLAYER_INFO_Y + 218;		// SMALLPOCK20
		gMapScreenInvPocketXY[45].sX = PLAYER_INFO_X + 183;	gMapScreenInvPocketXY[45].sY = PLAYER_INFO_Y + 239;		// SMALLPOCK21
		gMapScreenInvPocketXY[46].sX = PLAYER_INFO_X + 219;	gMapScreenInvPocketXY[46].sY = PLAYER_INFO_Y + 239;		// SMALLPOCK22
		gMapScreenInvPocketXY[47].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[47].sY = PLAYER_INFO_Y + 281;		// SMALLPOCK23
		gMapScreenInvPocketXY[48].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[48].sY = PLAYER_INFO_Y + 302;		// SMALLPOCK24
		gMapScreenInvPocketXY[49].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[49].sY = PLAYER_INFO_Y + 323;		// SMALLPOCK25
		gMapScreenInvPocketXY[50].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[50].sY = PLAYER_INFO_Y + 344;		// SMALLPOCK26
		gMapScreenInvPocketXY[51].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[51].sY = PLAYER_INFO_Y + 281;		// SMALLPOCK27
		gMapScreenInvPocketXY[52].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[52].sY = PLAYER_INFO_Y + 302;		// SMALLPOCK28
		gMapScreenInvPocketXY[53].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[53].sY = PLAYER_INFO_Y + 323;		// SMALLPOCK29
		gMapScreenInvPocketXY[54].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[54].sY = PLAYER_INFO_Y + 344;		// SMALLPOCK30
	}
	else{
		gMapScreenInvPocketXY[0].sX = PLAYER_INFO_X + 131;	gMapScreenInvPocketXY[0].sY = PLAYER_INFO_Y + 9;		// HELMETPOS
		gMapScreenInvPocketXY[1].sX = PLAYER_INFO_X + 131;	gMapScreenInvPocketXY[1].sY = PLAYER_INFO_Y + 37;		// VESTPOS
		gMapScreenInvPocketXY[2].sX = PLAYER_INFO_X + 131;	gMapScreenInvPocketXY[2].sY = PLAYER_INFO_Y + 97;		// LEGPOS
		gMapScreenInvPocketXY[3].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[3].sY = PLAYER_INFO_Y + 9;		// HEAD1POS
		gMapScreenInvPocketXY[4].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[4].sY = PLAYER_INFO_Y + 32;		// HEAD2POS
		gMapScreenInvPocketXY[5].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[5].sY = PLAYER_INFO_Y + 86;		// HANDPOS
		gMapScreenInvPocketXY[6].sX = PLAYER_INFO_X + 14;	gMapScreenInvPocketXY[6].sY = PLAYER_INFO_Y + 110;		// SECONDHANDPOS
		gMapScreenInvPocketXY[7].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[7].sY = PLAYER_INFO_Y + 156;		// VESTPOCK
		gMapScreenInvPocketXY[8].sX = PLAYER_INFO_X + 29;	gMapScreenInvPocketXY[8].sY = PLAYER_INFO_Y + 256;		// LTHIGHPOCK
		gMapScreenInvPocketXY[9].sX = PLAYER_INFO_X + 112;	gMapScreenInvPocketXY[9].sY = PLAYER_INFO_Y + 256;		// RTHIGHPOCK
		gMapScreenInvPocketXY[10].sX = PLAYER_INFO_X + 194;	gMapScreenInvPocketXY[10].sY = PLAYER_INFO_Y + 256;		// CPACKPOCK
		gMapScreenInvPocketXY[11].sX = PLAYER_INFO_X + 173;	gMapScreenInvPocketXY[11].sY = PLAYER_INFO_Y + 439;		// BPACKPOCK
		gMapScreenInvPocketXY[12].sX = PLAYER_INFO_X + 95;	gMapScreenInvPocketXY[12].sY = PLAYER_INFO_Y + 156;		// GUNSLINGPOCKPOS
		gMapScreenInvPocketXY[13].sX = PLAYER_INFO_X + 36;	gMapScreenInvPocketXY[13].sY = PLAYER_INFO_Y + 156;		// KNIFEPOCKPOS
		gMapScreenInvPocketXY[14].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[14].sY = PLAYER_INFO_Y + 336;		// BIGPOCK1
		gMapScreenInvPocketXY[15].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[15].sY = PLAYER_INFO_Y + 360;		// BIGPOCK2
		gMapScreenInvPocketXY[16].sX = PLAYER_INFO_X + 186;	gMapScreenInvPocketXY[16].sY = PLAYER_INFO_Y + 384;		// BIGPOCK3
		gMapScreenInvPocketXY[17].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[17].sY = PLAYER_INFO_Y + 379;		// BIGPOCK4
		gMapScreenInvPocketXY[18].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[18].sY = PLAYER_INFO_Y + 403;		// BIGPOCK5
		gMapScreenInvPocketXY[19].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[19].sY = PLAYER_INFO_Y + 427;		// BIGPOCK6
		gMapScreenInvPocketXY[20].sX = PLAYER_INFO_X + 98;	gMapScreenInvPocketXY[20].sY = PLAYER_INFO_Y + 451;		// BIGPOCK7
		gMapScreenInvPocketXY[21].sX = PLAYER_INFO_X + 202;	gMapScreenInvPocketXY[21].sY = PLAYER_INFO_Y + 188;		// MEDPOCK1
		gMapScreenInvPocketXY[22].sX = PLAYER_INFO_X + 202;	gMapScreenInvPocketXY[22].sY = PLAYER_INFO_Y + 212;		// MEDPOCK2
		gMapScreenInvPocketXY[23].sX = PLAYER_INFO_X + 29;	gMapScreenInvPocketXY[23].sY = PLAYER_INFO_Y + 336;		// MEDPOCK3
		gMapScreenInvPocketXY[24].sX = PLAYER_INFO_X + 112;	gMapScreenInvPocketXY[24].sY = PLAYER_INFO_Y + 336;		// MEDPOCK4
		gMapScreenInvPocketXY[25].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[25].sY = PLAYER_INFO_Y + 188;		// SMALLPOCK1
		gMapScreenInvPocketXY[26].sX = PLAYER_INFO_X + 58;	gMapScreenInvPocketXY[26].sY = PLAYER_INFO_Y + 188;		// SMALLPOCK2
		gMapScreenInvPocketXY[27].sX = PLAYER_INFO_X + 94;	gMapScreenInvPocketXY[27].sY = PLAYER_INFO_Y + 188;		// SMALLPOCK3
		gMapScreenInvPocketXY[28].sX = PLAYER_INFO_X + 130;	gMapScreenInvPocketXY[28].sY = PLAYER_INFO_Y + 188;		// SMALLPOCK4
		gMapScreenInvPocketXY[29].sX = PLAYER_INFO_X + 166;	gMapScreenInvPocketXY[29].sY = PLAYER_INFO_Y + 188;		// SMALLPOCK5
		gMapScreenInvPocketXY[30].sX = PLAYER_INFO_X + 22;	gMapScreenInvPocketXY[30].sY = PLAYER_INFO_Y + 212;		// SMALLPOCK6
		gMapScreenInvPocketXY[31].sX = PLAYER_INFO_X + 58;	gMapScreenInvPocketXY[31].sY = PLAYER_INFO_Y + 212;		// SMALLPOCK7
		gMapScreenInvPocketXY[32].sX = PLAYER_INFO_X + 94;	gMapScreenInvPocketXY[32].sY = PLAYER_INFO_Y + 212;		// SMALLPOCK8	
		gMapScreenInvPocketXY[33].sX = PLAYER_INFO_X + 130;	gMapScreenInvPocketXY[33].sY = PLAYER_INFO_Y + 212;		// SMALLPOCK9
		gMapScreenInvPocketXY[34].sX = PLAYER_INFO_X + 166;	gMapScreenInvPocketXY[34].sY = PLAYER_INFO_Y + 212;		// SMALLPOCK10
		gMapScreenInvPocketXY[35].sX = PLAYER_INFO_X + 18;	gMapScreenInvPocketXY[35].sY = PLAYER_INFO_Y + 288;		// SMALLPOCK11
		gMapScreenInvPocketXY[36].sX = PLAYER_INFO_X + 54;	gMapScreenInvPocketXY[36].sY = PLAYER_INFO_Y + 288;		// SMALLPOCK12
		gMapScreenInvPocketXY[37].sX = PLAYER_INFO_X + 18;	gMapScreenInvPocketXY[37].sY = PLAYER_INFO_Y + 312;		// SMALLPOCK13
		gMapScreenInvPocketXY[38].sX = PLAYER_INFO_X + 54;	gMapScreenInvPocketXY[38].sY = PLAYER_INFO_Y + 312;		// SMALLPOCK14
		gMapScreenInvPocketXY[39].sX = PLAYER_INFO_X + 100;	gMapScreenInvPocketXY[39].sY = PLAYER_INFO_Y + 288;		// SMALLPOCK15
		gMapScreenInvPocketXY[40].sX = PLAYER_INFO_X + 136;	gMapScreenInvPocketXY[40].sY = PLAYER_INFO_Y + 288;		// SMALLPOCK16
		gMapScreenInvPocketXY[41].sX = PLAYER_INFO_X + 100;	gMapScreenInvPocketXY[41].sY = PLAYER_INFO_Y + 312;		// SMALLPOCK17
		gMapScreenInvPocketXY[42].sX = PLAYER_INFO_X + 136;	gMapScreenInvPocketXY[42].sY = PLAYER_INFO_Y + 312;		// SMALLPOCK18
		gMapScreenInvPocketXY[43].sX = PLAYER_INFO_X + 183;	gMapScreenInvPocketXY[43].sY = PLAYER_INFO_Y + 288;		// SMALLPOCK19
		gMapScreenInvPocketXY[44].sX = PLAYER_INFO_X + 219;	gMapScreenInvPocketXY[44].sY = PLAYER_INFO_Y + 288;		// SMALLPOCK20
		gMapScreenInvPocketXY[45].sX = PLAYER_INFO_X + 183;	gMapScreenInvPocketXY[45].sY = PLAYER_INFO_Y + 312;		// SMALLPOCK21
		gMapScreenInvPocketXY[46].sX = PLAYER_INFO_X + 219;	gMapScreenInvPocketXY[46].sY = PLAYER_INFO_Y + 312;		// SMALLPOCK22
		gMapScreenInvPocketXY[47].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[47].sY = PLAYER_INFO_Y + 379;		// SMALLPOCK23
		gMapScreenInvPocketXY[48].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[48].sY = PLAYER_INFO_Y + 403;		// SMALLPOCK24
		gMapScreenInvPocketXY[49].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[49].sY = PLAYER_INFO_Y + 427;		// SMALLPOCK25
		gMapScreenInvPocketXY[50].sX = PLAYER_INFO_X + 26;	gMapScreenInvPocketXY[50].sY = PLAYER_INFO_Y + 451;		// SMALLPOCK26
		gMapScreenInvPocketXY[51].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[51].sY = PLAYER_INFO_Y + 379;		// SMALLPOCK27
		gMapScreenInvPocketXY[52].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[52].sY = PLAYER_INFO_Y + 403;		// SMALLPOCK28
		gMapScreenInvPocketXY[53].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[53].sY = PLAYER_INFO_Y + 427;		// SMALLPOCK29
		gMapScreenInvPocketXY[54].sX = PLAYER_INFO_X + 62;	gMapScreenInvPocketXY[54].sY = PLAYER_INFO_Y + 451;		// SMALLPOCK30
	}

	// item description
	MAP_ITEMDESC_START_X			= 0;
	MAP_ITEMDESC_START_Y			= PLAYER_INFO_Y;

	INV_REGION_X					= PLAYER_INFO_X;
	INV_REGION_Y					= PLAYER_INFO_Y;
	INV_REGION_WIDTH				= 261;
	INV_REGION_HEIGHT				= 490;
	INV_BTN_X						= PLAYER_INFO_X + 226;
	INV_BTN_Y						= PLAYER_INFO_Y + 19;
	INV_BDROP_X						= PLAYER_INFO_X + 220;
	INV_BDROP_Y						= PLAYER_INFO_Y + 441;

	MAP_ARMOR_LABEL_X				= 136;
	MAP_ARMOR_LABEL_Y				= 177;
	MAP_ARMOR_X						= 138;
	MAP_ARMOR_Y						= 187;
	MAP_ARMOR_PERCENT_X				= 158;
	MAP_ARMOR_PERCENT_Y				= 188;

	MAP_WEIGHT_LABEL_X				= 185;
	MAP_WEIGHT_LABEL_Y				= 209;
	MAP_WEIGHT_X					= 188;
	MAP_WEIGHT_Y					= 219;
	MAP_WEIGHT_PERCENT_X			= 208;
	MAP_WEIGHT_PERCENT_Y			= 219;

	MAP_CAMMO_LABEL_X				= 226;
	MAP_CAMMO_LABEL_Y				= 209;
	MAP_CAMMO_X						= 224;
	MAP_CAMMO_Y						= 219;
	MAP_CAMMO_PERCENT_X				= 244;
	MAP_CAMMO_PERCENT_Y				= 219;

	MAP_PERCENT_WIDTH				= 20;
	MAP_PERCENT_HEIGHT				= 10;

	MAP_INV_STATS_TITLE_FONT_COLOR	= 6;
	MAP_INV_STATS_TEXT_FONT_COLOR	= 5;

	PLAYER_INFO_FACE_START_X		= 9;
	PLAYER_INFO_FACE_START_Y		= 17;
	PLAYER_INFO_FACE_END_X			= 60;
	PLAYER_INFO_FACE_END_Y			= 76;

	INV_BODY_X						= 31;
	INV_BODY_Y						= 8 + PLAYER_INFO_Y;

	NAME_X							= 4;
	NAME_WIDTH						= 55 - NAME_X;

	ASSIGN_X						= 60;
	ASSIGN_WIDTH					= 111 - ASSIGN_X;
	SLEEP_X							= 116;
	SLEEP_WIDTH						= 135 - SLEEP_X;
	LOC_X							= 140;
	LOC_WIDTH						= 172 - LOC_X;
	DEST_ETA_X						= 177;
	DEST_ETA_WIDTH					= 210 - DEST_ETA_X;
	TIME_REMAINING_X				= 215;
	TIME_REMAINING_WIDTH			= 243 - TIME_REMAINING_X;

	if (iResolution == 0)
	{
		CLOCK_Y_START					= (MAP_BORDER_Y_OFFSET + 298);

		DEST_PLOT_X						= (MAP_BORDER_X_OFFSET + 463);
		DEST_PLOT_Y						= (MAP_BORDER_Y_OFFSET + 345);

		CLOCK_ETA_X						= (MAP_BORDER_X_OFFSET + 484);
		CLOCK_HOUR_X_START				= (MAP_BORDER_X_OFFSET + 518);
		CLOCK_MIN_X_START				= (MAP_BORDER_X_OFFSET + 538);
	}
	else if (iResolution == 1)
	{
		CLOCK_Y_START					= (MAP_BORDER_Y_OFFSET + 298 + 120);

		DEST_PLOT_X						= (MAP_BORDER_X_OFFSET + 463 + 80);
		DEST_PLOT_Y						= (MAP_BORDER_Y_OFFSET + 345 + 120);

		CLOCK_ETA_X						= (MAP_BORDER_X_OFFSET + 484 + 80);
		CLOCK_HOUR_X_START				= (MAP_BORDER_X_OFFSET + 518 + 80);
		CLOCK_MIN_X_START				= (MAP_BORDER_X_OFFSET + 538 + 80);
	}
	else if (iResolution == 2)
	{
		CLOCK_Y_START					= (MAP_BORDER_Y_OFFSET + 298 + 285);

		DEST_PLOT_X						= (MAP_BORDER_X_OFFSET + 463 + 180);
		DEST_PLOT_Y						= (MAP_BORDER_Y_OFFSET + 345 + 285);

		CLOCK_ETA_X						= (MAP_BORDER_X_OFFSET + 484 + 180);
		CLOCK_HOUR_X_START				= (MAP_BORDER_X_OFFSET + 518 + 180);
		CLOCK_MIN_X_START				= (MAP_BORDER_X_OFFSET + 538 + 180);
	}

	// contract
	CONTRACT_X						= 185;
	CONTRACT_Y						= 50;

	// trash can
	TRASH_CAN_X						= 209;
	TRASH_CAN_Y						= 59 + PLAYER_INFO_Y;
	TRASH_CAN_WIDTH					= 193 - 165;
	TRASH_CAN_HEIGHT				= 239 - 217;

	// keyring
	MAP_KEYRING_X					= 180;
	MAP_KEYRING_Y					= 127;

	//Text offsets
	Y_OFFSET						= 2;

	// The boxes defines
	TRAIN_Y_OFFSET					= 53;
	TRAIN_X_OFF						= 65;
	TRAIN_WID						= 80;
	TRAIN_HEIG						= 47;
	STRING_X_OFFSET					= 10;
	STRING_Y_OFFSET					= 5;
	POP_UP_BOX_X					= 120;
	POP_UP_BOX_Y					= 0;
	POP_UP_BOX_WIDTH				= 60;
	POP_UP_BOX_HEIGHT				= 100;
	MOUSE_PTR_Y_OFFSET				= 3;
	POP_UP_Y_OFFSET					= 3;
	TRAIN_TEXT_Y_OFFSET				= 4;

	// char stat positions
	STR_X							= 112;
	STR_Y							= 42;
	DEX_X							= STR_X;
	DEX_Y							= 32;
	AGL_X							= STR_X;
	AGL_Y							= 22;
	LDR_X							= STR_X;
	LDR_Y							= 52;
	WIS_X							= STR_X;
	WIS_Y							= 62;
	LVL_X							= 159;
	LVL_Y							= AGL_Y;
	MRK_X							= LVL_X;
	MRK_Y							= DEX_Y;
	EXP_X							= LVL_X;
	EXP_Y							= STR_Y;
	MEC_X							= LVL_X;
	MEC_Y							= LDR_Y;
	MED_X							= LVL_X;
	MED_Y							= WIS_Y;

	STAT_WID						= 15;
	STAT_HEI						= GetFontHeight(CHAR_FONT);

	PIC_NAME_X						= 8;
	PIC_NAME_Y						= (66 + 3);
	PIC_NAME_WID					= 60 - PIC_NAME_X;
	PIC_NAME_HEI					= 75 - PIC_NAME_Y;
	CHAR_NAME_X						= 14;
	CHAR_NAME_Y						= (2 + 3);
	CHAR_NAME_WID					= 164 - CHAR_NAME_X;
	CHAR_NAME_HEI					= 11 - CHAR_NAME_Y;
	CHAR_LOC_X						= 76;
	CHAR_LOC_Y						= 84;
	CHAR_LOC_WID					= 16;
	CHAR_LOC_HEI					= 9;
	CHAR_TIME_REMAINING_X			= 207;
	CHAR_TIME_REMAINING_Y			= 65;
	CHAR_TIME_REMAINING_WID			= 258 - CHAR_TIME_REMAINING_X;
	CHAR_TIME_REMAINING_HEI			= GetFontHeight(CHAR_FONT);
	CHAR_SALARY_X					= CHAR_TIME_REMAINING_X;
	CHAR_SALARY_Y					= 79;
	CHAR_SALARY_WID					= CHAR_TIME_REMAINING_WID - 8;
	CHAR_SALARY_HEI					= CHAR_TIME_REMAINING_HEI;
	CHAR_MEDICAL_X					= CHAR_TIME_REMAINING_X;
	CHAR_MEDICAL_Y					= 93;
	CHAR_MEDICAL_WID				= CHAR_TIME_REMAINING_WID - 8;
	CHAR_MEDICAL_HEI				= CHAR_TIME_REMAINING_HEI;
	CHAR_ASSIGN_X					= 182;
	CHAR_ASSIGN1_Y					= 18;
	CHAR_ASSIGN2_Y					= 31;
	CHAR_ASSIGN_WID					= 257 - 178;
	CHAR_ASSIGN_HEI					= 39 - 29;
	CHAR_HP_X						= 133;
	CHAR_HP_Y						= 77 + 3;
	CHAR_HP_WID						= 175 - CHAR_HP_X;
	CHAR_HP_HEI						= 90 - CHAR_HP_Y;
	CHAR_MORALE_X					= 133;
	CHAR_MORALE_Y					= 91 + 3;
	CHAR_MORALE_WID					= 175 - CHAR_MORALE_X;
	CHAR_MORALE_HEI					= 101 - CHAR_MORALE_Y;

	CROSS_X							= 195;
	CROSS_Y							= 83;
	CROSS_HEIGHT					= 20;
	CROSS_WIDTH						= 20;
	CHAR_PAY_X						= 150;
	CHAR_PAY_Y						= 80+4;
	CHAR_PAY_HEI					= GetFontHeight(CHAR_FONT);
	CHAR_PAY_WID					= CROSS_X-CHAR_PAY_X;
	SOLDIER_PIC_X					= 9;
	SOLDIER_PIC_Y					= 20;
	SOLDIER_HAND_X					= 6;
	SOLDIER_HAND_Y					= 81;

	gSCamoXY.sX = INV_BODY_X;	gSCamoXY.sY = INV_BODY_Y;	// X, Y Location of Map screen's Camouflage region

	//OJW - MP interface changes
	InitializeMPCoordinates();

	return ( TRUE );
}

void InitializeMPCoordinates()
{
	//OJW - MP interface changes
	if (iResolution == 0)
	{
		MP_BTN_Y = 268;
	}
	else if (iResolution == 1)
	{
		MP_BTN_Y = 386;
	}
	else if (iResolution == 2)
	{
		MP_BTN_Y = 554;
	}

	MP_ROWSTART_Y = MP_BTN_Y + 21;
	MP_PLAYER_X = 5;
	MP_PLAYER_W = 75;
	MP_TEAM_X = 84;
	MP_TEAM_W = 40;
	MP_COMPASS_X = 130;
	MP_COMPASS_W = 19;
	MP_GAMEINFO_X = 154;
	MP_GAMEINFO_W = 81;

	// End of MP interface changes
}
BOOLEAN InitializeInvPanelCoordsVehicle( )
{
	InitializeInvPanelCoordsNew();
	// Inventory slots
	if(iResolution == 0){
		gMapScreenInvPocketXY[14].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[14].sY = PLAYER_INFO_Y + 89;
		gMapScreenInvPocketXY[15].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[15].sY = PLAYER_INFO_Y + 89;
		gMapScreenInvPocketXY[16].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[16].sY = PLAYER_INFO_Y + 89;
		gMapScreenInvPocketXY[17].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[17].sY = PLAYER_INFO_Y + 110;
		gMapScreenInvPocketXY[18].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[18].sY = PLAYER_INFO_Y + 110;
		gMapScreenInvPocketXY[19].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[19].sY = PLAYER_INFO_Y + 110;
		gMapScreenInvPocketXY[20].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[20].sY = PLAYER_INFO_Y + 131;
		gMapScreenInvPocketXY[21].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[21].sY = PLAYER_INFO_Y + 131;
		gMapScreenInvPocketXY[22].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[22].sY = PLAYER_INFO_Y + 131;
		gMapScreenInvPocketXY[23].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[23].sY = PLAYER_INFO_Y + 152;
		gMapScreenInvPocketXY[24].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[24].sY = PLAYER_INFO_Y + 152;
		gMapScreenInvPocketXY[25].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[25].sY = PLAYER_INFO_Y + 152;
		gMapScreenInvPocketXY[26].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[26].sY = PLAYER_INFO_Y + 173;
		gMapScreenInvPocketXY[27].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[27].sY = PLAYER_INFO_Y + 173;
		gMapScreenInvPocketXY[28].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[28].sY = PLAYER_INFO_Y + 173;
		gMapScreenInvPocketXY[29].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[29].sY = PLAYER_INFO_Y + 194;
		gMapScreenInvPocketXY[30].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[30].sY = PLAYER_INFO_Y + 194;
		gMapScreenInvPocketXY[31].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[31].sY = PLAYER_INFO_Y + 194;
		gMapScreenInvPocketXY[32].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[32].sY = PLAYER_INFO_Y + 215;
		gMapScreenInvPocketXY[33].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[33].sY = PLAYER_INFO_Y + 215;
		gMapScreenInvPocketXY[34].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[34].sY = PLAYER_INFO_Y + 215;
		gMapScreenInvPocketXY[35].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[35].sY = PLAYER_INFO_Y + 236;
		gMapScreenInvPocketXY[36].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[36].sY = PLAYER_INFO_Y + 236;
		gMapScreenInvPocketXY[37].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[37].sY = PLAYER_INFO_Y + 236;
		gMapScreenInvPocketXY[38].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[38].sY = PLAYER_INFO_Y + 257;
		gMapScreenInvPocketXY[39].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[39].sY = PLAYER_INFO_Y + 257;
		gMapScreenInvPocketXY[40].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[40].sY = PLAYER_INFO_Y + 257;
		gMapScreenInvPocketXY[41].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[41].sY = PLAYER_INFO_Y + 278;
		gMapScreenInvPocketXY[42].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[42].sY = PLAYER_INFO_Y + 278;
		gMapScreenInvPocketXY[43].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[43].sY = PLAYER_INFO_Y + 278;
		gMapScreenInvPocketXY[44].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[44].sY = PLAYER_INFO_Y + 299;
		gMapScreenInvPocketXY[45].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[45].sY = PLAYER_INFO_Y + 299;
		gMapScreenInvPocketXY[46].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[46].sY = PLAYER_INFO_Y + 299;
		gMapScreenInvPocketXY[47].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[47].sY = PLAYER_INFO_Y + 320;
		gMapScreenInvPocketXY[48].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[48].sY = PLAYER_INFO_Y + 320;
		gMapScreenInvPocketXY[49].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[49].sY = PLAYER_INFO_Y + 320;
		gMapScreenInvPocketXY[50].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[50].sY = PLAYER_INFO_Y + 341;
		gMapScreenInvPocketXY[51].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[51].sY = PLAYER_INFO_Y + 341;
		gMapScreenInvPocketXY[52].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[52].sY = PLAYER_INFO_Y + 341;
	}
	else{
		gMapScreenInvPocketXY[14].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[14].sY = PLAYER_INFO_Y + 156;
		gMapScreenInvPocketXY[15].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[15].sY = PLAYER_INFO_Y + 156;
		gMapScreenInvPocketXY[16].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[16].sY = PLAYER_INFO_Y + 156;
		gMapScreenInvPocketXY[17].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[17].sY = PLAYER_INFO_Y + 180;
		gMapScreenInvPocketXY[18].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[18].sY = PLAYER_INFO_Y + 180;
		gMapScreenInvPocketXY[19].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[19].sY = PLAYER_INFO_Y + 180;
		gMapScreenInvPocketXY[20].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[20].sY = PLAYER_INFO_Y + 204;
		gMapScreenInvPocketXY[21].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[21].sY = PLAYER_INFO_Y + 204;
		gMapScreenInvPocketXY[22].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[22].sY = PLAYER_INFO_Y + 204;
		gMapScreenInvPocketXY[23].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[23].sY = PLAYER_INFO_Y + 228;
		gMapScreenInvPocketXY[24].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[24].sY = PLAYER_INFO_Y + 228;
		gMapScreenInvPocketXY[25].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[25].sY = PLAYER_INFO_Y + 228;
		gMapScreenInvPocketXY[26].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[26].sY = PLAYER_INFO_Y + 252;
		gMapScreenInvPocketXY[27].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[27].sY = PLAYER_INFO_Y + 252;
		gMapScreenInvPocketXY[28].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[28].sY = PLAYER_INFO_Y + 252;
		gMapScreenInvPocketXY[29].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[29].sY = PLAYER_INFO_Y + 276;
		gMapScreenInvPocketXY[30].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[30].sY = PLAYER_INFO_Y + 276;
		gMapScreenInvPocketXY[31].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[31].sY = PLAYER_INFO_Y + 276;
		gMapScreenInvPocketXY[32].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[32].sY = PLAYER_INFO_Y + 300;
		gMapScreenInvPocketXY[33].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[33].sY = PLAYER_INFO_Y + 300;
		gMapScreenInvPocketXY[34].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[34].sY = PLAYER_INFO_Y + 300;
		gMapScreenInvPocketXY[35].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[35].sY = PLAYER_INFO_Y + 324;
		gMapScreenInvPocketXY[36].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[36].sY = PLAYER_INFO_Y + 324;
		gMapScreenInvPocketXY[37].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[37].sY = PLAYER_INFO_Y + 324;
		gMapScreenInvPocketXY[38].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[38].sY = PLAYER_INFO_Y + 348;
		gMapScreenInvPocketXY[39].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[39].sY = PLAYER_INFO_Y + 348;
		gMapScreenInvPocketXY[40].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[40].sY = PLAYER_INFO_Y + 348;
		gMapScreenInvPocketXY[41].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[41].sY = PLAYER_INFO_Y + 372;
		gMapScreenInvPocketXY[42].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[42].sY = PLAYER_INFO_Y + 372;
		gMapScreenInvPocketXY[43].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[43].sY = PLAYER_INFO_Y + 372;
		gMapScreenInvPocketXY[44].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[44].sY = PLAYER_INFO_Y + 396;
		gMapScreenInvPocketXY[45].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[45].sY = PLAYER_INFO_Y + 396;
		gMapScreenInvPocketXY[46].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[46].sY = PLAYER_INFO_Y + 396;
		gMapScreenInvPocketXY[47].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[47].sY = PLAYER_INFO_Y + 420;
		gMapScreenInvPocketXY[48].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[48].sY = PLAYER_INFO_Y + 420;
		gMapScreenInvPocketXY[49].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[49].sY = PLAYER_INFO_Y + 420;
		gMapScreenInvPocketXY[50].sX = PLAYER_INFO_X + 37;	gMapScreenInvPocketXY[50].sY = PLAYER_INFO_Y + 444;
		gMapScreenInvPocketXY[51].sX = PLAYER_INFO_X + 104;	gMapScreenInvPocketXY[51].sY = PLAYER_INFO_Y + 444;
		gMapScreenInvPocketXY[52].sX = PLAYER_INFO_X + 171;	gMapScreenInvPocketXY[52].sY = PLAYER_INFO_Y + 444;
	}

	return ( TRUE );
}

// the tries to select a mapscreen character by his soldier ID
BOOLEAN SetInfoChar( UINT8 ubID )
{
	INT8 bCounter;

	for ( bCounter = 0; bCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; bCounter++)
	{
		// skip invalid characters
		if ( gCharactersList[ bCounter ].fValid == TRUE )
		{
			if ( gCharactersList[ bCounter ].usSolID == (UINT16)ubID )
			{
				ChangeSelectedInfoChar( bCounter, TRUE );
				return( TRUE );
			}
		}
	}

	return ( FALSE );
}



void DisplayDestinationOfCurrentDestMerc( void )
{
	// will display the dest of the current dest merc
	CHAR16 sString[ 32 ];
	INT16 sX, sY;
	INT16 sSector;

	SetFont( MAP_SCREEN_FONT );

	sSector = GetLastSectorIdInCharactersPath( &Menptr[gCharactersList[ bSelectedDestChar ].usSolID] );

	SetBoxForeground(ghVehicleBox, FONT_LTGREEN);
	SetBoxBackground(ghVehicleBox, FONT_BLACK);

	swprintf( sString, L"%s%s", pMapVertIndex[ sSector / MAP_WORLD_X ], pMapHortIndex[ sSector % MAP_WORLD_X ] );
	FindFontCenterCoordinates(DEST_PLOT_X, DEST_PLOT_Y ,70 ,GetFontHeight( MAP_SCREEN_FONT ) ,sString , MAP_SCREEN_FONT, &sX, &sY);

	RestoreExternBackgroundRect( DEST_PLOT_X, DEST_PLOT_Y ,70 ,GetFontHeight( MAP_SCREEN_FONT ) );
	mprintf( sX, sY, sString );
}



void ContractBoxGlow( void )
{
/* Why not?
 static INT32 iColorNum=10;
 static BOOLEAN fDelta=FALSE;
 static BOOLEAN fOldContractGlow = FALSE;
 UINT16 usColor;
 UINT32 uiDestPitchBYTES;
 UINT8	*pDestBuf;


	// stopped glowing?
	if( ( fGlowContractRegion == FALSE )&&( ( fOldContractGlow == TRUE )||( fResetContractGlow== TRUE ) ) )
	{
		// restore background
		// RestoreExternBackgroundRect( CONTRACT_X, CONTRACT_Y, CONTRACT_WIDTH+1, CONTRACT_HEIGHT+1 );

		// reset old
		fOldContractGlow = FALSE;
	}

	// not glowing right now, leave
	if( ( fGlowContractRegion == FALSE )||( fResetContractGlow == TRUE ) )
	{
		// reset color rotation
		iColorNum =0;
		fDelta = TRUE;

		// reset
		fResetContractGlow = FALSE;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	// change direction of glow?
	if((iColorNum==0)||(iColorNum==10))
	{
	fDelta=!fDelta;
	}

	// increment color
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;

	usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, 640, 480);
	RectangleDraw( TRUE, CONTRACT_X, CONTRACT_Y, CONTRACT_X+CONTRACT_WIDTH, CONTRACT_Y+CONTRACT_HEIGHT, usColor, pDestBuf );
	InvalidateRegion(CONTRACT_X, CONTRACT_Y, CONTRACT_X+CONTRACT_WIDTH+1, CONTRACT_Y+CONTRACT_HEIGHT+1);
	UnLockVideoSurface( FRAME_BUFFER );

	// restore background
	if((iColorNum==0)||(iColorNum==1))
		RestoreExternBackgroundRect( CONTRACT_X, CONTRACT_Y, CONTRACT_WIDTH+1, CONTRACT_HEIGHT+1 );
*/
}


void ContractListRegionBoxGlow( UINT16 usCount )
{
 static INT32 iColorNum =10;
 static BOOLEAN fDelta=FALSE;
 UINT16 usColor;
 UINT32 uiDestPitchBYTES;
 UINT8	*pDestBuf;
 INT16 usY = 0;
// INT16 sYStart = 0;
 //INT16 usVehicleCount = 0;
 INT16 sYAdd = 0;


	// if not glowing right now, leave
	if( ( giContractHighLine == -1 )||( fResetContractGlow == TRUE ) || fShowInventoryFlag )
	{
		iColorNum = 0;
		fDelta = TRUE;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	// change direction of glow?
	if((iColorNum==0)||(iColorNum==10))
	{
		fDelta=!fDelta;
	}

	// increment color
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;

	sYAdd = 0;

	// y start position of box
	usY=(Y_OFFSET*usCount-1)+(Y_START+(usCount*Y_SIZE) + sYAdd );

	// glow contract box
	usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	RectangleDraw( TRUE, TIME_REMAINING_X, usY, TIME_REMAINING_X + TIME_REMAINING_WIDTH, usY + GetFontHeight( MAP_SCREEN_FONT ) + 2, usColor, pDestBuf );
	InvalidateRegion(TIME_REMAINING_X - 1, usY, TIME_REMAINING_X + TIME_REMAINING_WIDTH + 1, usY + GetFontHeight( MAP_SCREEN_FONT ) + 3 );
	UnLockVideoSurface( FRAME_BUFFER );
}


void GlowFace( void )
{
 static INT32 iColorNum =10;
 static BOOLEAN fDelta=FALSE;
 static BOOLEAN fOldFaceGlow = FALSE;
 UINT16 usColor;
 UINT32 uiDestPitchBYTES;
 UINT8	*pDestBuf;


	// not glowing right now, leave
	if( fShowFaceHightLight == FALSE )
	{
		iColorNum =0;
		fDelta = TRUE;

		if( fOldFaceGlow == TRUE )
		{
			RestoreExternBackgroundRect( 9, 18, ( UINT16 )( 61 - 9 ), ( UINT16 )( 64 - 18 ) );
		}

		fOldFaceGlow = FALSE;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	fOldFaceGlow = TRUE;

	// change direction of glow?
	if((iColorNum==0)||(iColorNum==10))
	{
	fDelta=!fDelta;
	}

	// increment color
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;

	// glow contract box
	usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	RectangleDraw( TRUE, 9, 18, 60, 63 , usColor, pDestBuf );
	InvalidateRegion( 9, 18, 61, 64 );
	UnLockVideoSurface( FRAME_BUFFER );

	// restore background
	if((iColorNum==0)||(iColorNum==1))
		RestoreExternBackgroundRect( 9, 18, ( UINT16 )( 61 - 9 ), ( UINT16 )( 64 - 18 ) );
}


void GlowItem( void )
{
 static INT32 iColorNum =10;
 static BOOLEAN fDelta=FALSE;
 static BOOLEAN fOldItemGlow = FALSE;
 UINT16 usColor;
 UINT32 uiDestPitchBYTES;
 UINT8	*pDestBuf;


	// not glowing right now, leave
	if( fShowItemHighLight == FALSE )
	{
		iColorNum =0;
		fDelta = TRUE;

		if( fOldItemGlow == TRUE )
		{
			// this caused the attachment/burst/grenade star to disappear behind the item image
			//RestoreExternBackgroundRect( 3, 80, ( UINT16 )( 65 - 3 ), ( UINT16 )( 105 - 80 ) );
		}

		fOldItemGlow = FALSE;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	fOldItemGlow = TRUE;

	// change direction of glow?
	if((iColorNum==0)||(iColorNum==10))
	{
	fDelta=!fDelta;
	}

	// increment color
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;

	// restore background
	if((iColorNum==0)||(iColorNum==1))
	{
		RestoreExternBackgroundRect( 3, 80, ( UINT16 )( 65 - 3 ), ( UINT16 )( 105 - 80 ) );
		RenderHandPosItem();
	}

	// glow contract box
	usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	RectangleDraw( TRUE, 3, 80, 64, 104 , usColor, pDestBuf );
	InvalidateRegion( 3, 80, 65, 105 );
	UnLockVideoSurface( FRAME_BUFFER );
}


void GlowTrashCan( void )
{
 static INT32 iColorNum =10;
 static BOOLEAN fDelta=FALSE;
 static BOOLEAN fOldTrashCanGlow = FALSE;
 UINT16 usColor;
 UINT32 uiDestPitchBYTES;
 UINT8	*pDestBuf;


	if( fShowInventoryFlag == FALSE )
	{
		fShowTrashCanHighLight = FALSE;
	}

	// not glowing right now, leave
	if( fShowTrashCanHighLight == FALSE )
	{
		iColorNum =0;
		fDelta = TRUE;

		if( fOldTrashCanGlow == TRUE )
		{
			RestoreExternBackgroundRect( TRASH_CAN_X, TRASH_CAN_Y, ( UINT16 )( TRASH_CAN_WIDTH + 2 ), ( UINT16 )( TRASH_CAN_HEIGHT + 2 ) );
		}

		fOldTrashCanGlow = FALSE;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	fOldTrashCanGlow = TRUE;

	// glow contract box
	usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	RectangleDraw( TRUE, TRASH_CAN_X, TRASH_CAN_Y , TRASH_CAN_X + TRASH_CAN_WIDTH, TRASH_CAN_Y + TRASH_CAN_HEIGHT , usColor, pDestBuf );
	InvalidateRegion( TRASH_CAN_X, TRASH_CAN_Y, TRASH_CAN_X + TRASH_CAN_WIDTH + 1, TRASH_CAN_Y + TRASH_CAN_HEIGHT + 1 );
	UnLockVideoSurface( FRAME_BUFFER );

	// restore background
	if((iColorNum==0)||(iColorNum==1))
		RestoreExternBackgroundRect( TRASH_CAN_X, TRASH_CAN_Y, ( UINT16 )( TRASH_CAN_WIDTH + 2 ), ( UINT16 )( TRASH_CAN_HEIGHT + 2 ) );
}



void DrawFace( INT16 sCharNumber )
{
	SOLDIERTYPE	*pSoldier = NULL;
	static INT16 sOldId = -1;

	// draws the face of the currently selected merc, being displayed int he upper left hand corner

	// grab the soldier
	if( bSelectedInfoChar != -1 )
	{
		if( gCharactersList[ bSelectedInfoChar ].fValid )
		{
			GetSoldier( &pSoldier, gCharactersList[ bSelectedInfoChar ].usSolID );
		}
	}

	if( pSoldier == NULL )
	{
		return;
	}


	if( ( gCharactersList[ bSelectedInfoChar ].usSolID == sOldId )&&( fReDrawFace == FALSE ) )
	{
		// are the same, return
		return;
	}

	// get old id value
	sOldId = gCharactersList[ bSelectedInfoChar ].usSolID;

	// reset redraw of face
	fReDrawFace = FALSE;

	// render their face
	RenderSoldierFace( pSoldier, SOLDIER_PIC_X, SOLDIER_PIC_Y, TRUE );

	return;
}



void RenderHandPosItem( void )
{
	SOLDIERTYPE	*pSoldier = NULL;
	// renders the inventory item in char's right hand

	// ARM: if already in the inventory panel, don't show the item again here, seeing it twice is confusing
	if ( fShowInventoryFlag )
	{
		return;
	}


	// grab the soldier
	if( bSelectedInfoChar != -1 )
	{
		if( gCharactersList[ bSelectedInfoChar ].fValid )
		{
			GetSoldier( &pSoldier, gCharactersList[ bSelectedInfoChar ].usSolID );
		}
	}

	if( pSoldier == NULL )
	{
		return;
	}

	// check if still alive?
	if( pSoldier->stats.bLife == 0 )
	{
		return;
	}

	SetFont(BLOCKFONT2);
	SetFontForeground(CHAR_INFO_PANEL_BLOCK_COLOR);
	SetFontBackground(FONT_BLACK);

	INVRenderItem( guiSAVEBUFFER, pSoldier, &(pSoldier->inv[HANDPOS]), SOLDIER_HAND_X, SOLDIER_HAND_Y,	58, 23, DIRTYLEVEL2, NULL, 0, FALSE, 0);
}



void RenderIconsForUpperLeftCornerPiece( INT8 bCharNumber )
{
	HVOBJECT hHandle;

	GetVideoObject(&hHandle, guiULICONS);

	// if merc is an AIM merc
	if( Menptr[ gCharactersList[ bCharNumber ].usSolID ].ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
	{
		// finite contract length icon
		BltVideoObject( guiSAVEBUFFER, hHandle, 0, CHAR_ICON_X, CHAR_ICON_CONTRACT_Y, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// if merc has life insurance
	if( Menptr[ gCharactersList[ bCharNumber ].usSolID ].usLifeInsurance > 0 )
	{
		// draw life insurance icon
		BltVideoObject( guiSAVEBUFFER, hHandle, 2, CHAR_ICON_X, CHAR_ICON_CONTRACT_Y + CHAR_ICON_SPACING, VO_BLT_SRCTRANSPARENCY, NULL );
	}

	// if merc has a medical deposit
	if( Menptr[ gCharactersList[ bCharNumber ].usSolID ].usMedicalDeposit > 0 )
	{
		// draw medical deposit icon
		BltVideoObject( guiSAVEBUFFER, hHandle, 1, CHAR_ICON_X, CHAR_ICON_CONTRACT_Y + ( 2 * CHAR_ICON_SPACING ), VO_BLT_SRCTRANSPARENCY, NULL );
	}
}


void DrawPay(INT16 sCharNumber)
{
	// will draw the pay
	INT32 uiSalary;
	CHAR16 sString[7];
	INT16 usX, usY;
	INT16 usMercProfileID;


	// get merc id
	usMercProfileID = MercPtrs[ gCharactersList[ sCharNumber ].usSolID ]->ubProfile;

	// grab salary
	uiSalary=( ( UINT32 ) gMercProfiles[ usMercProfileID ].sSalary );


	// font stuff
	SetFontForeground( CHAR_TITLE_FONT_COLOR );
	SetFontBackground( FONT_BLACK );

	// parse salary
	swprintf( sString, L"%d", uiSalary );

	// right justify salary
	FindFontRightCoordinates(CHAR_PAY_X,CHAR_PAY_Y , CHAR_PAY_WID,CHAR_PAY_HEI ,sString , CHAR_FONT, &usX, &usY);

	// draw salary
	DrawString(sString,usX, usY, CHAR_FONT);
}

void DrawCharBars( void )
{
	UINT16 usSoldierID;
	SOLDIERTYPE	*pSoldier;

	// will draw the heath, morale and breath bars for a character being displayed in the upper left hand corner

	if( ( bSelectedInfoChar == -1 )&&( bSelectedDestChar == -1 ) )
	{
		// error, no character to display right now
		return;
	}
	else
	{
		// valid character
		if( bSelectedInfoChar != -1 )
		{
			usSoldierID=gCharactersList[ bSelectedInfoChar ].usSolID;
		}
		else
		{
			usSoldierID=gCharactersList[ bSelectedDestChar ].usSolID;
		}

		// grab soldier's id number
		GetSoldier( &pSoldier, usSoldierID );

		if( pSoldier == NULL )
		{
			// no soldier
			return;
		}

		// skip POWs, dead guys
		if( ( pSoldier->stats.bLife == 0 ) ||
				( pSoldier->bAssignment == ASSIGNMENT_DEAD ) ||
				( pSoldier->bAssignment == ASSIGNMENT_POW ) )
		{
			return;
		}

		// current health
		DrawLifeUIBarEx( pSoldier, BAR_INFO_X, BAR_INFO_Y,3,42, TRUE, FRAME_BUFFER );

		// robot doesn't have energy/fuel
		if ( !AM_A_ROBOT( pSoldier ) )
		{
			// current energy/fuel
			DrawBreathUIBarEx( pSoldier, BAR_INFO_X + 6, BAR_INFO_Y,3,42, TRUE, FRAME_BUFFER );
		}

		// vehicles and robot don't have morale
		if ( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) )
		{
			// draw morale bar
			DrawMoraleUIBarEx( pSoldier, BAR_INFO_X + 12, BAR_INFO_Y,3,42, TRUE, FRAME_BUFFER );
		}
	}

	return;
}


void DrawCharStats( INT16 sCharNum )
{
	// will draw the characters stats, max life, strength, dex, and skills
	CHAR16 sString[9];
	INT16 usX, usY;
	//HVOBJECT hCrossHandle;
	SOLDIERTYPE *pSoldier = NULL;

	pSoldier = &Menptr[gCharactersList[sCharNum].usSolID];

	// set up font
	SetFont(CHAR_FONT);
	SetFontForeground(CHAR_TEXT_FONT_COLOR);
	SetFontBackground(FONT_BLACK);

	// strength
	swprintf( sString, L"%d", pSoldier->stats.bStrength);

	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_STRENGTH] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeStrengthTime)&& ( pSoldier->timeChanges.uiChangeStrengthTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & STRENGTH_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(STR_X, STR_Y ,STAT_WID ,STAT_HEI ,sString, CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, STR_Y,CHAR_FONT );

	// dexterity
	swprintf( sString, L"%d", pSoldier->stats.bDexterity );

	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_DEXTERITY] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeDexterityTime ) && ( pSoldier->timeChanges.uiChangeDexterityTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & DEX_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(DEX_X,DEX_Y ,STAT_WID ,STAT_HEI ,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, DEX_Y,CHAR_FONT );

	// agility
	swprintf( sString, L"%d", pSoldier->stats.bAgility );
	
	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_AGILITY] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeAgilityTime)&& ( pSoldier->timeChanges.uiChangeAgilityTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & AGIL_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(AGL_X,AGL_Y,STAT_WID ,STAT_HEI ,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, AGL_Y,CHAR_FONT );

	// wisdom
	swprintf( sString, L"%d", pSoldier->stats.bWisdom );
	
	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_WISDOM] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeWisdomTime )&&( pSoldier->timeChanges.uiChangeWisdomTime != 0 ))
	{
		if( pSoldier->usValueGoneUp & WIS_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates( WIS_X,WIS_Y,STAT_WID ,STAT_HEI,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, WIS_Y,CHAR_FONT );

	// leadership
	swprintf( sString, L"%d", pSoldier->stats.bLeadership );
	
	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_LEADERSHIP] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeLeadershipTime ) && ( pSoldier->timeChanges.uiChangeLeadershipTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & LDR_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(LDR_X,LDR_Y,STAT_WID,STAT_HEI, sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, LDR_Y,CHAR_FONT );

	// experience level
	swprintf( sString, L"%d", pSoldier->stats.bExpLevel );

	if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeLevelTime)&&( pSoldier->timeChanges.uiChangeLevelTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & LVL_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(LVL_X,LVL_Y,STAT_WID ,STAT_HEI,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, LVL_Y,CHAR_FONT );

	// marksmanship
	swprintf( sString, L"%d", pSoldier->stats.bMarksmanship );

	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_MARKSMANSHIP] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeMarksmanshipTime) && ( pSoldier->timeChanges.uiChangeMarksmanshipTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & MRK_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(MRK_X,MRK_Y,STAT_WID ,STAT_HEI,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, MRK_Y,CHAR_FONT );

	// explosives
	swprintf( sString, L"%d", pSoldier->stats.bExplosive );
	
	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_EXPLOSIVES] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeExplosivesTime)&& ( pSoldier->timeChanges.uiChangeExplosivesTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & EXP_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(EXP_X,EXP_Y,STAT_WID ,STAT_HEI,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, EXP_Y,CHAR_FONT );

	// mechanical
	swprintf( sString, L"%d", pSoldier->stats.bMechanical );
	
	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_MECHANICAL] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeMechanicalTime )&& ( pSoldier->timeChanges.uiChangeMechanicalTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & MECH_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(MEC_X,MEC_Y,STAT_WID ,STAT_HEI,sString , CHAR_FONT, &usX, &usY);
	DrawString(sString,usX, MEC_Y,CHAR_FONT );

	// medical
	swprintf( sString, L"%d", pSoldier->stats.bMedical );

	// SANDRO - if damaged stat we could regain, show in red until repaired
	if( gGameOptions.fNewTraitSystem && ( pSoldier->ubCriticalStatDamage[DAMAGED_STAT_MEDICAL] > 0 ))
	{
		SetFontForeground( FONT_RED );
	}
	else if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeMedicalTime)&& ( pSoldier->timeChanges.uiChangeMedicalTime != 0 ) )
	{
		if( pSoldier->usValueGoneUp & MED_INCREASE )
		{
			SetFontForeground( FONT_LTGREEN );
		}
		else
		{
			SetFontForeground( FONT_RED );
		}
	}
	else
	{
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
	}

	// right justify
	FindFontRightCoordinates(MED_X,MED_Y,STAT_WID ,STAT_HEI,sString , CHAR_FONT, &usX, &usY);
 	DrawString(sString,usX, MED_Y,CHAR_FONT );

	SetFontForeground(CHAR_TEXT_FONT_COLOR);

	return;
}



void DrawCharHealth( INT16 sCharNum )
{
	UINT32 uiHealthPercent = 0;
	CHAR16 sString[9];
	INT16 usX, usY;
	SOLDIERTYPE *pSoldier = NULL;


	pSoldier = &Menptr[gCharactersList[sCharNum].usSolID];

	if( pSoldier->bAssignment != ASSIGNMENT_POW )
	{
		// find starting X coordinate by centering all 3 substrings together, then print them separately (different colors)!
		swprintf( sString, L"%d/%d", pSoldier->stats.bLife, pSoldier->stats.bLifeMax );
		FindFontCenterCoordinates(CHAR_HP_X, CHAR_HP_Y, CHAR_HP_WID, CHAR_HP_HEI, sString, CHAR_FONT, &usX, &usY);


		if ( pSoldier->stats.bLifeMax > 0 )
		{
			uiHealthPercent = ( pSoldier->stats.bLife * 100 ) / pSoldier->stats.bLifeMax;
		}

		// how is characters life?
		if (uiHealthPercent == 0)
		{
			// he's dead, Jim
			SetFontForeground(FONT_METALGRAY);
		}
		else
		if (uiHealthPercent < 25)
		{
			// very bad
			SetFontForeground(FONT_RED);
		}
		else
		if (uiHealthPercent < 50)
		{
			// not good
			SetFontForeground(FONT_YELLOW);
		}
		else
		{
			// ok
			SetFontForeground(CHAR_TEXT_FONT_COLOR);
		}

		// current life
		swprintf( sString, L"%d", pSoldier->stats.bLife );
		DrawString( sString, usX, CHAR_HP_Y, CHAR_FONT );
		usX += StringPixLength( sString, CHAR_FONT );


		// slash
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
		wcscpy( sString, L"/" );
		DrawString( sString, usX, CHAR_HP_Y, CHAR_FONT );
		usX += StringPixLength( sString, CHAR_FONT );


		if( ( GetJA2Clock() < CHANGE_STAT_RECENTLY_DURATION + pSoldier->timeChanges.uiChangeHealthTime)&& ( pSoldier->timeChanges.uiChangeHealthTime != 0 ) )
		{
			if( pSoldier->usValueGoneUp & HEALTH_INCREASE )
			{
				SetFontForeground( FONT_LTGREEN );
			}
			else
			{
				SetFontForeground( FONT_RED );
			}
		}
		else
		{
			SetFontForeground(CHAR_TEXT_FONT_COLOR);
		}

		// maximum life
		swprintf( sString, L"%d", pSoldier->stats.bLifeMax );
		DrawString( sString, usX, CHAR_HP_Y, CHAR_FONT );
	}
	else
	{
		// POW - health unknown
		SetFontForeground(CHAR_TEXT_FONT_COLOR);
		swprintf( sString, pPOWStrings[ 1 ] );
		FindFontCenterCoordinates(CHAR_HP_X, CHAR_HP_Y, CHAR_HP_WID, CHAR_HP_HEI, sString, CHAR_FONT, &usX, &usY);
		DrawString(sString, usX, CHAR_HP_Y, CHAR_FONT);
	}


	SetFontForeground(CHAR_TEXT_FONT_COLOR);
}



// "character" refers to hired people AND vehicles
void DrawCharacterInfo(INT16 sCharNumber)
{
	CHAR16 sString[80];
	INT16 usX, usY;
	INT16 usMercProfileID;
	INT32 iTimeRemaining=0;
	INT32 iDailyCost = 0;
	SOLDIERTYPE *pSoldier = NULL;
	UINT32 uiArrivalTime;


	if( gCharactersList[sCharNumber].fValid == FALSE )
	{
		return;
	}

	pSoldier = MercPtrs[ gCharactersList[sCharNumber].usSolID ];

	if( pSoldier->ubProfile == NO_PROFILE )
	{
		return;
	}


	// draw particular info about a character that are neither attributes nor skills


	// get profile information
	usMercProfileID = pSoldier->ubProfile;


	// set font stuff
	SetFont(CHAR_FONT);
	SetFontForeground(CHAR_TEXT_FONT_COLOR);
	SetFontBackground(FONT_BLACK);


	// Nickname (beneath Picture)
	if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// vehicle
		wcscpy(sString, pShortVehicleStrings[ pVehicleList[ pSoldier->bVehicleID ].ubVehicleType ]);
	}
	else
	{
		// soldier
		wcscpy(sString, gMercProfiles[usMercProfileID].zNickname);
	}

	FindFontCenterCoordinates(PIC_NAME_X, PIC_NAME_Y, PIC_NAME_WID, PIC_NAME_HEI, sString, CHAR_FONT, &usX, &usY);
	DrawString(sString, usX, usY, CHAR_FONT);


	// Full name (Top Box)
	if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		// vehicle
		wcscpy(sString, pVehicleStrings[ pVehicleList[ pSoldier->bVehicleID ].ubVehicleType ]);
	}
	else
	{
		// soldier
		wcscpy(sString, gMercProfiles[usMercProfileID].zName);
	}

	FindFontCenterCoordinates(CHAR_NAME_X, CHAR_NAME_Y, CHAR_NAME_WID, CHAR_NAME_HEI, sString, CHAR_FONT, &usX, &usY);
	DrawString(sString, usX, usY, CHAR_FONT);


	// Assignment
	if( pSoldier->bAssignment == VEHICLE )
	{
		// show vehicle type
		wcscpy( sString, pShortVehicleStrings[ pVehicleList[ pSoldier->iVehicleId ].ubVehicleType ] );
	}
	else
	{
		wcscpy( sString, pAssignmentStrings[ pSoldier->bAssignment ] );
	}

	FindFontCenterCoordinates( CHAR_ASSIGN_X, CHAR_ASSIGN1_Y, CHAR_ASSIGN_WID, CHAR_ASSIGN_HEI, sString, CHAR_FONT, &usX, &usY );
	DrawString( sString, usX, usY, CHAR_FONT );


	// second assignment line

	// HEADROCK HAM 3.6: Operating a facility?
	if (pSoldier->sFacilityTypeOperated != -1)
	{
		UINT8 ubFacilityType = (UINT8)pSoldier->sFacilityTypeOperated;
		// Print sector ID string
		GetShortSectorString( pSoldier->sSectorX, pSoldier->sSectorY, sString );
		wcscat( sString, L": " );
		wcscat( sString, gFacilityTypes[ubFacilityType].szFacilityShortName );
		//wcscpy(sString, gFacilityTypes[ pSoldier->sFacilityTypeOperated ].szFacilityName);
	}

	// train self / teammate / by other ?
	else if( ( pSoldier->bAssignment == TRAIN_SELF ) || ( pSoldier->bAssignment == TRAIN_TEAMMATE ) || ( pSoldier->bAssignment == TRAIN_BY_OTHER ) )
	{
		wcscpy(sString, pAttributeMenuStrings[pSoldier->bTrainStat]);
	}
	// train town?
	// HEADROCK HAM 3.6: Draw for Mobile Trainers as well.
	else if( pSoldier->bAssignment == TRAIN_TOWN || pSoldier->bAssignment == TRAIN_MOBILE )
	{
		wcscpy(sString,pTownNames[GetTownIdForSector( pSoldier->sSectorX, pSoldier->sSectorY ) ] );
	}
	// repairing?
	else if( pSoldier->bAssignment == REPAIR )
	{
		if ( pSoldier->flags.fFixingRobot )
		{
			// robot
			wcscpy( sString, pRepairStrings[ 3 ] );
		}
/*
		else if ( pSoldier->flags.fFixingSAMSite )
		{
			// SAM site
			wcscpy( sString, pRepairStrings[ 1 ] );
		}
*/
		else if ( pSoldier->bVehicleUnderRepairID != -1 )
		{
			// vehicle
			wcscpy( sString, pShortVehicleStrings[ pVehicleList[ pSoldier->bVehicleUnderRepairID ].ubVehicleType ] );
		}
		else
		{
			// items
			wcscpy( sString, pRepairStrings[ 0 ] );
		}
	}
	// in transit?
	else if( pSoldier->bAssignment == IN_TRANSIT )
	{
		// show ETA
		ConvertMinTimeToETADayHourMinString( pSoldier->uiTimeSoldierWillArrive, sString );
	}
	// traveling ?
	else if ( PlayerIDGroupInMotion( GetSoldierGroupId( pSoldier ) ) )
	{
		// show ETA
		uiArrivalTime = GetWorldTotalMin( ) + CalculateTravelTimeOfGroupId( GetSoldierGroupId( pSoldier ) );
		ConvertMinTimeToETADayHourMinString( uiArrivalTime, sString );
	}
	else
	{
		// show location
		GetMapscreenMercLocationString( pSoldier, sString );
	}

	if ( wcslen( sString ) > 0 )
	{
		FindFontCenterCoordinates(CHAR_ASSIGN_X, CHAR_ASSIGN2_Y ,CHAR_ASSIGN_WID ,CHAR_ASSIGN_HEI ,sString , CHAR_FONT, &usX, &usY);
		DrawString(sString, usX, usY, CHAR_FONT );
	}


	// draw health/condition
	DrawCharHealth( sCharNumber );


	// if a vehicle or robot
	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) )
	{
		// we're done - the remainder applies only to people
		return;
	}


	// draw attributes & skills for currently displayed character
	DrawCharStats(sCharNumber);

	// remaining contract length

	// dead?
	if( pSoldier->stats.bLife <= 0 )
	{
		swprintf( sString, L"%s", gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
	}
	// what kind of merc
	else if(pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC || pSoldier->ubProfile == SLAY )
	{
		FLOAT dTimeLeft = 0.0;

		// amount of time left on contract
		iTimeRemaining = pSoldier->iEndofContractTime-GetWorldTotalMin();

		//if the merc is in transit
		if( pSoldier->bAssignment == IN_TRANSIT )
		{
			//and if the ttime left on the cotract is greater then the contract time
			if( iTimeRemaining > (INT32)( pSoldier->iTotalContractLength * NUM_MIN_IN_DAY ) )
			{
				iTimeRemaining = ( pSoldier->iTotalContractLength * NUM_MIN_IN_DAY );
			}
		}


		if(iTimeRemaining >= ( 24 * 60 ) )
		{
			//calculate the exact time left on the contract ( ex 1.8 days )
			dTimeLeft = (FLOAT)(iTimeRemaining / (60*24.0) );

			// more than a day, display in green
			iTimeRemaining /= (60*24);
			if( pSoldier->stats.bLife > 0 )
			{
				SetFontForeground(FONT_LTGREEN);
			}

			swprintf(sString, L"%.1f%s/%d%s", dTimeLeft, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ], pSoldier->iTotalContractLength, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ]);
		}
		else
		{
			// less than a day, display hours left in red
			if( iTimeRemaining > 5 )
			{
				BOOLEAN fNeedToIncrement = FALSE;

				if( iTimeRemaining % 60 != 0 )
					fNeedToIncrement = TRUE;

				iTimeRemaining /= 60;

				if( fNeedToIncrement )
					iTimeRemaining++;
			}
			else
			{
				iTimeRemaining /= 60;
			}

			if( pSoldier->stats.bLife > 0 )
			{
				SetFontForeground(FONT_RED);
			}

		swprintf(sString, L"%d%s/%d%s",iTimeRemaining, gpStrategicString[ STR_PB_HOURS_ABBREVIATION ], pSoldier->iTotalContractLength, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ]);
		}
	}
	else if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC )
	{
		INT32 iBeenHiredFor = ( GetWorldTotalMin( ) / NUM_MIN_IN_DAY ) - pSoldier->iStartContractTime;

		swprintf(sString, L"%d%s/%d%s",gMercProfiles[ pSoldier->ubProfile ].iMercMercContractLength, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ], iBeenHiredFor, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ] );
	}
	else
	{
		swprintf( sString, L"%s", gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
	}


	// set font stuff
	SetFontForeground(CHAR_TEXT_FONT_COLOR);
	SetFontBackground(FONT_BLACK);

	// center and draw
	FindFontCenterCoordinates(CHAR_TIME_REMAINING_X, CHAR_TIME_REMAINING_Y, CHAR_TIME_REMAINING_WID, CHAR_TIME_REMAINING_HEI, sString, CHAR_FONT, &usX, &usY);
	DrawString(sString,usX,usY, CHAR_FONT);


	// salary
	if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
	{
		// daily rate
		if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
		{
			iDailyCost = ( gMercProfiles[ pSoldier->ubProfile ].uiBiWeeklySalary / 14 );
		}
		if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK )
		{
			iDailyCost = ( gMercProfiles[ pSoldier->ubProfile ].uiWeeklySalary / 7 );
		}
		else
		{
			iDailyCost = gMercProfiles[ pSoldier->ubProfile ].sSalary;
		}
	}
	else
	{
		iDailyCost = gMercProfiles[ pSoldier->ubProfile ].sSalary;
	}

	swprintf(sString, L"%d", iDailyCost );

	// insert commas and dollar sign
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );

	FindFontRightCoordinates(CHAR_SALARY_X, CHAR_SALARY_Y, CHAR_SALARY_WID, CHAR_SALARY_HEI, sString, CHAR_FONT, &usX, &usY);
	DrawString(sString,usX,usY, CHAR_FONT);


	// medical deposit
	if( gMercProfiles[ Menptr[ gCharactersList[ sCharNumber ].usSolID ].ubProfile ].sMedicalDepositAmount > 0 )
	{
		swprintf(sString, L"%d", gMercProfiles[ Menptr[ gCharactersList[ sCharNumber ].usSolID ].ubProfile ].sMedicalDepositAmount );

		// insert commas and dollar sign
		InsertCommasForDollarFigure( sString );
		InsertDollarSignInToString( sString );

		FindFontRightCoordinates(CHAR_MEDICAL_X, CHAR_MEDICAL_Y, CHAR_MEDICAL_WID, CHAR_MEDICAL_HEI, sString, CHAR_FONT, &usX, &usY);
		DrawString(sString,usX,CHAR_MEDICAL_Y, CHAR_FONT);
	}

/*
	// life insurance
	swprintf(sString, L"%d", Menptr[ gCharactersList[ sCharNumber ].usSolID ].usLifeInsuranceAmount );
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );
	FindFontRightCoordinates(CHAR_LIFE_INSUR_X, CHAR_LIFE_INSUR_Y, CHAR_LIFE_INSUR_WID, CHAR_LIFE_INSUR_HEI, sString, CHAR_FONT, &usX, &usY);
	DrawString(sString,usX,usY, CHAR_FONT);
*/

	// morale
	if( pSoldier->bAssignment != ASSIGNMENT_POW )
	{
		if ( pSoldier->stats.bLife != 0 )
		{
			GetMoraleString( MercPtrs[gCharactersList[sCharNumber].usSolID], sString );
		}
		else
		{
			wcscpy( sString, L"" );
		}
	}
	else
	{
		// POW - morale unknown
		swprintf( sString, pPOWStrings[ 1 ] );
	}

	FindFontCenterCoordinates(CHAR_MORALE_X, CHAR_MORALE_Y, CHAR_MORALE_WID, CHAR_MORALE_HEI, sString, CHAR_FONT, &usX, &usY);
	DrawString(sString,usX,CHAR_MORALE_Y, CHAR_FONT);


	return;
}


// this character is in transit has an item picked up
BOOLEAN CharacterIsInTransitAndHasItemPickedUp( INT8 bCharacterNumber )
{

	// valid character?
	if( bCharacterNumber == -1 )
	{
		// nope
		return( FALSE );
	}

	// second validity check
	if( gCharactersList[ bCharacterNumber ].fValid == FALSE )
	{
		// nope
		return( FALSE );
	}

	// character in transit?
	if( Menptr[ gCharactersList[ bCharacterNumber ].usSolID ].bAssignment != IN_TRANSIT )
	{
		// nope
		return( FALSE );
	}

	// item picked up?
	if( gMPanelRegion.Cursor != EXTERN_CURSOR)
	{
		return( FALSE );
	}

	return( TRUE );
}



void DisplayCharacterInfo( void )
{
	Assert( bSelectedInfoChar < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS );
	Assert( gCharactersList[ bSelectedInfoChar ].fValid );

	// set font buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// HEADROCK HAM 3.6: "progress" bars showing how near the character is to "leveling up" in any stat. The bar
	// is displayed behind the current stat value, as see on the character's info panel.
	// This section draws STRATEGIC info pages. Another section is in Interface Panels.cpp and draws TACTICAL info pages.
	// The feature is toggled by Options-Menu switch, and its color is determined in the INI files.
	if ( gGameSettings.fOptions[TOPTION_STAT_PROGRESS_BARS] )
	{
		SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[bSelectedInfoChar].usSolID ];
	
		UINT8	*pDestBuf;
		UINT32 uiDestPitchBYTES = 0;
		SGPRect ClipRect;
		UINT8 ubBarWidth = 0;
		UINT16 usColor = Get16BPPColor( FROMRGB( gGameExternalOptions.ubStatProgressBarsRed, gGameExternalOptions.ubStatProgressBarsGreen, gGameExternalOptions.ubStatProgressBarsBlue ) );
		//pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		pDestBuf = LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES );

		// AGI
		if (gMercProfiles[ pSoldier->ubProfile ].sAgilityGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sAgilityGain+1)) / SubpointsPerPoint(AGILAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (AGL_Y-1);
			ClipRect.iBottom = (AGL_Y-1) + STAT_HEI;
			ClipRect.iLeft = AGL_X;
			ClipRect.iRight = AGL_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// DEX
		if (gMercProfiles[ pSoldier->ubProfile ].sDexterityGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sDexterityGain+1)) / SubpointsPerPoint(DEXTAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (DEX_Y-1);
			ClipRect.iBottom = (DEX_Y-1) + STAT_HEI;
			ClipRect.iLeft = DEX_X;
			ClipRect.iRight = DEX_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// STR
		if (gMercProfiles[ pSoldier->ubProfile ].sStrengthGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sStrengthGain+1)) / SubpointsPerPoint(STRAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (STR_Y-1);
			ClipRect.iBottom = (STR_Y-1) + STAT_HEI;
			ClipRect.iLeft = STR_X;
			ClipRect.iRight = STR_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// WIS
		if (gMercProfiles[ pSoldier->ubProfile ].sWisdomGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sWisdomGain+1)) / SubpointsPerPoint(WISDOMAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (WIS_Y-1);
			ClipRect.iBottom = (WIS_Y-1) + STAT_HEI;
			ClipRect.iLeft = WIS_X;
			ClipRect.iRight = WIS_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// MRK
		if (gMercProfiles[ pSoldier->ubProfile ].sMarksmanshipGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sMarksmanshipGain+1)) / SubpointsPerPoint(MARKAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (MRK_Y-1);
			ClipRect.iBottom = (MRK_Y-1) + STAT_HEI;
			ClipRect.iLeft = MRK_X;
			ClipRect.iRight = MRK_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// LDR
		if (gMercProfiles[ pSoldier->ubProfile ].sLeadershipGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sLeadershipGain+1)) / SubpointsPerPoint(LDRAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (LDR_Y-1);
			ClipRect.iBottom = (LDR_Y-1) + STAT_HEI;
			ClipRect.iLeft = LDR_X;
			ClipRect.iRight = LDR_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// MECH
		if (gMercProfiles[ pSoldier->ubProfile ].sMechanicGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sMechanicGain+1)) / SubpointsPerPoint(MECHANAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (MEC_Y-1);
			ClipRect.iBottom = (MEC_Y-1) + STAT_HEI;
			ClipRect.iLeft = MEC_X;
			ClipRect.iRight = MEC_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// EXPLO
		if (gMercProfiles[ pSoldier->ubProfile ].sExplosivesGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sExplosivesGain+1)) / SubpointsPerPoint(EXPLODEAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (EXP_Y-1);
			ClipRect.iBottom = (EXP_Y-1) + STAT_HEI;
			ClipRect.iLeft = EXP_X;
			ClipRect.iRight = EXP_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// MED
		if (gMercProfiles[ pSoldier->ubProfile ].sMedicalGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sMedicalGain+1)) / SubpointsPerPoint(MEDICALAMT,0);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (MED_Y-1);
			ClipRect.iBottom = (MED_Y-1) + STAT_HEI;
			ClipRect.iLeft = MED_X;
			ClipRect.iRight = MED_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		// EXPLEVEL
		if (gMercProfiles[ pSoldier->ubProfile ].sExpLevelGain)
		{
			ubBarWidth = (STAT_WID * (gMercProfiles[ pSoldier->ubProfile ].sExpLevelGain+1)) / SubpointsPerPoint(EXPERAMT, pSoldier->stats.bExpLevel);
			ubBarWidth = __max(0, __min(ubBarWidth, STAT_WID));
			ClipRect.iTop = (LVL_Y-1);
			ClipRect.iBottom = (LVL_Y-1) + STAT_HEI;
			ClipRect.iLeft = LVL_X;
			ClipRect.iRight = LVL_X + ubBarWidth;
			Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );
		}

		UnLockVideoSurface( guiSAVEBUFFER );
	}


	// draw character info and face
	DrawCharacterInfo( bSelectedInfoChar );

	RenderHandPosItem();

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	RenderIconsForUpperLeftCornerPiece( bSelectedInfoChar );

	// mark all pop ups as dirty
	MarkAllBoxesAsAltered( );
}



INT32 GetPathTravelTimeDuringPlotting( PathStPtr pPath )
{
	INT32 iTravelTime = 0;
	WAYPOINT pCurrent;
	WAYPOINT pNext;
	GROUP *pGroup;
	UINT8 ubGroupId = 0;
	BOOLEAN fSkipFirstNode = FALSE;


	if( ( bSelectedDestChar == -1 ) && ( fPlotForHelicopter == FALSE ) )
	{
		return( 0 );
	}

	if( fTempPathAlreadyDrawn == FALSE )
	{
		return( 0 );
	}

	if( pPath == NULL )
	{
		return( 0 );
	}

	pPath = MoveToBeginningOfPathList( pPath );


	if( fPlotForHelicopter == FALSE )
	{
		// plotting for a character...
		if( Menptr[gCharactersList[bSelectedDestChar].usSolID].bAssignment == VEHICLE )
		{
			ubGroupId = pVehicleList[ Menptr[gCharactersList[bSelectedDestChar].usSolID].iVehicleId ].ubMovementGroup;
			pGroup = GetGroup( ubGroupId );

			if( pGroup == NULL )
			{
				SetUpMvtGroupForVehicle( &( Menptr[gCharactersList[bSelectedDestChar].usSolID] ) );

				// get vehicle id
				ubGroupId = pVehicleList[ Menptr[gCharactersList[bSelectedDestChar].usSolID].iVehicleId ].ubMovementGroup;
				pGroup = GetGroup( ubGroupId );
				AssertNotNIL(pGroup);
			}
		}
		else if( Menptr[gCharactersList[bSelectedDestChar].usSolID].flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			ubGroupId = pVehicleList[ Menptr[gCharactersList[bSelectedDestChar].usSolID].bVehicleID ].ubMovementGroup;
			pGroup = GetGroup( ubGroupId );

			if( pGroup == NULL )
			{
				SetUpMvtGroupForVehicle( &( Menptr[gCharactersList[bSelectedDestChar].usSolID] ) );

				// get vehicle id
				ubGroupId = pVehicleList[ Menptr[gCharactersList[bSelectedDestChar].usSolID].bVehicleID ].ubMovementGroup;
				pGroup = GetGroup( ubGroupId );
				AssertNotNIL(pGroup);
			}
		}
		else
		{
			ubGroupId = Menptr[gCharactersList[bSelectedDestChar].usSolID].ubGroupID;
			pGroup = GetGroup( ( UINT8 )( ubGroupId ) );
			AssertNotNIL(pGroup);
		}
	}
	else
	{
		ubGroupId = pVehicleList[ iHelicopterVehicleId ].ubMovementGroup;
		pGroup = GetGroup( ubGroupId );
		AssertNotNIL(pGroup);
	}

	// if between sectors
	if ( pGroup->fBetweenSectors )
	{
		// arrival time should always be legal!
		Assert( pGroup->uiArrivalTime >= GetWorldTotalMin( ) );

		// start with time to finish arriving in any traversal already in progress
		iTravelTime = pGroup->uiArrivalTime - GetWorldTotalMin();
		fSkipFirstNode = TRUE;
	}
	else
	{
		iTravelTime = 0;
	}

	while( pPath->pNext )
	{
		if ( !fSkipFirstNode )
		{
			// grab the current location
			pCurrent.x = ( UINT8 )( pPath->uiSectorId % MAP_WORLD_X );
			pCurrent.y = ( UINT8 )( pPath->uiSectorId / MAP_WORLD_X );

			// grab the next location
			pNext.x = ( UINT8 )( pPath->pNext->uiSectorId % MAP_WORLD_X );
			pNext.y = ( UINT8 )( pPath->pNext->uiSectorId / MAP_WORLD_X );

			iTravelTime += FindTravelTimeBetweenWaypoints( &pCurrent, &pNext, pGroup );
		}
		else
		{
			fSkipFirstNode = FALSE;
		}

		pPath = pPath->pNext;
	}

	return( iTravelTime );
}



void DisplayGroundEta( void )
{
	UINT32 iTotalTime = 0;


	if( fPlotForHelicopter == TRUE )
	{
		return;
	}

	if( bSelectedDestChar == -1 )
	{
		return;
	}

	if( !gCharactersList[bSelectedDestChar].fValid )
	{
		return;
	}


	iTotalTime = GetGroundTravelTimeOfCharacter( bSelectedDestChar );

	// now display it
	SetFont( ETA_FONT );
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	mprintf( CLOCK_ETA_X, CLOCK_Y_START, pEtaString[ 0 ] );

	// if less than one day
	if( ( iTotalTime / ( 60 * 24 ) ) < 1 )
	{
		// show hours and minutes
		SetClockMin( L"%d", iTotalTime % 60 );
		SetClockHour(L"%d", iTotalTime / 60);
	}
	else
	{
		// show days and hours
		SetHourAlternate(L"%d", ( iTotalTime / 60 ) % 24 );
		SetDayAlternate( L"%d", iTotalTime / ( 60 * 24 ) );
	}
}



void HighLightAssignLine()
{
	UINT32 uiDestPitchBYTES;
	UINT8	*pDestBuf;
	UINT16 usColor;
	static INT32 iColorNum = STARTING_COLOR_NUM;
	static BOOLEAN fDelta=FALSE;
	static INT32 uiOldHighlight = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS + 1;
	INT16 usCount = 0;
	UINT16 usX;
	UINT16 usY;
	INT16 usVehicleCount = 0;


	// is this a valid line?
	if( ( giAssignHighLine == -1 ) || fShowInventoryFlag )
	{
		uiOldHighlight = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS + 1;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	// check if we have moved lines, if so, reset
	if( uiOldHighlight != giAssignHighLine )
	{
		iColorNum = STARTING_COLOR_NUM;
		fDelta = FALSE;

		uiOldHighlight = giAssignHighLine;
	}


	if((iColorNum==0)||(iColorNum==10))
	{
	fDelta=!fDelta;
	}
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;

	usY = ( Y_OFFSET * giAssignHighLine - 1 ) + ( Y_START + ( giAssignHighLine * Y_SIZE ) );	

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// marke strogg more mercs
	for( usCount = 0; usCount < maxNumberOfMercVisibleInStrategyList; usCount++ )
	{
		if( IsCharacterSelectedForAssignment( usCount ) == TRUE )
		{
			usX=ASSIGN_X;
			usY=(Y_OFFSET*usCount-1)+(Y_START+(usCount*Y_SIZE));

			usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );

			LineDraw(TRUE, usX, usY, usX, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			LineDraw(TRUE, usX+ASSIGN_WIDTH, usY, usX+ASSIGN_WIDTH, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			if( ( usCount == 0 ) || ( usCount != 0 ? !( IsCharacterSelectedForAssignment( ( UINT16 )( usCount - 1 ) ) ) : 0 ) )
			{
				LineDraw( TRUE, usX, usY, usX+ASSIGN_WIDTH, usY, usColor,pDestBuf);
			}

			if( ( ( usCount == giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1 ) ) || ( usCount != ( giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1 ) ? !( IsCharacterSelectedForAssignment( ( UINT16 )( usCount + 1 ) ) ) : 0) )
			{
				LineDraw(TRUE, usX, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usX+ASSIGN_WIDTH, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			}

			InvalidateRegion( usX, usY, usX+ASSIGN_WIDTH+1, usY+GetFontHeight(MAP_SCREEN_FONT)+3);
		}
	}

	UnLockVideoSurface( FRAME_BUFFER );
}


void HighLightDestLine()
{
	UINT32 uiDestPitchBYTES;
	UINT8	*pDestBuf;
	UINT16 usColor;
	static INT32 iColorNum = STARTING_COLOR_NUM;
	static BOOLEAN fDelta=FALSE;
	static INT32 uiOldHighlight = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS + 1;
	UINT16 usCount = 0;
	UINT16 usX;
	UINT16 usY;
	//UINT16 usVehicleCont = 0;


	if( ( giDestHighLine == -1 ) || fShowInventoryFlag )
	{
		uiOldHighlight = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS + 1;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	// check if we have moved lines, if so, reset
	if( uiOldHighlight != giDestHighLine )
	{
		iColorNum = STARTING_COLOR_NUM;
		fDelta = FALSE;

		uiOldHighlight = giDestHighLine;
	}


	if((iColorNum==0)||(iColorNum==10))
	{
	fDelta=!fDelta;
	}
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;


	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	// marke strogg more mercs
	for( usCount = 0; usCount < maxNumberOfMercVisibleInStrategyList; usCount++ )
	{
		if( CharacterIsGettingPathPlotted( usCount ) == TRUE )
		{
			usX=DEST_ETA_X-4;
			usY=(Y_OFFSET*usCount-1)+(Y_START+(usCount*Y_SIZE));

			usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );

			if( ( usCount == 0 ) || ( usCount != 0 ? !( CharacterIsGettingPathPlotted( ( UINT16 )( usCount - 1 ) ) ) : 0 ))
			{
				LineDraw( TRUE, usX+4, usY, usX+DEST_ETA_WIDTH+4, usY, usColor,pDestBuf);
			}
			if( ( ( usCount == giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1 ) ) || ( usCount != ( giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1 ) ? !( CharacterIsGettingPathPlotted( ( UINT16 )( usCount + 1 ) ) ) : 0) )
			{
				LineDraw(TRUE, usX+4, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usX+DEST_ETA_WIDTH+4, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			}


			LineDraw(TRUE, usX+4, usY, usX+4, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			LineDraw(TRUE, usX+DEST_ETA_WIDTH+4, usY, usX+DEST_ETA_WIDTH+4, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);

			InvalidateRegion( usX, usY, usX + DEST_ETA_WIDTH + 5, usY + GetFontHeight(MAP_SCREEN_FONT)+3 );
		}
	}
	//InvalidateRegion( usX+4, usY, DEST_ETA_WIDTH-10, usY+GetFontHeight(MAP_SCREEN_FONT)+3);
	//InvalidateRegion( usX+10, usY, usX+ASSIGN_WIDTH, usY+GetFontHeight(MAP_SCREEN_FONT)+3);
	UnLockVideoSurface( FRAME_BUFFER );
}



void HighLightSleepLine()
{
	UINT32 uiDestPitchBYTES;
	UINT8	*pDestBuf;
	UINT16 usColor;
	static INT32 iColorNum = STARTING_COLOR_NUM;
	static BOOLEAN fDelta=FALSE;
	static INT32 uiOldHighlight = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS + 1;
	UINT16 usCount = 0;
	UINT16 usX, usX2;
	UINT16 usY;
	//UINT16 usVehicleCount = 0;


	// is this a valid line?
	if( ( giSleepHighLine == -1 ) || fShowInventoryFlag )
	{
		uiOldHighlight = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS + 1;
		return;
	}

	// if not ready to change glow phase yet, leave
	if ( !gfGlowTimerExpired )
		return;


	// check if we have moved lines, if so, reset
	if( uiOldHighlight != giSleepHighLine )
	{
		iColorNum = STARTING_COLOR_NUM;
		fDelta = FALSE;

		uiOldHighlight = giSleepHighLine;
	}


	if((iColorNum==0)||(iColorNum==10))
	{
	fDelta=!fDelta;
	}
	if(!fDelta)
		iColorNum++;
	else
		iColorNum--;


	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
// marke strogg more mercs Ok, we have to watch for to use only 18 display lines insedead of MAX_CHARACTER_COUNT
	for( usCount = 0; usCount < maxNumberOfMercVisibleInStrategyList; usCount++ )
	{
		if( IsCharacterSelectedForSleep( usCount ) == TRUE )
		{
			usX=SLEEP_X-4;
			usX2 = SLEEP_X + SLEEP_WIDTH;

			usY=(Y_OFFSET*usCount-1)+(Y_START+(usCount*Y_SIZE));

			usColor=Get16BPPColor( FROMRGB( GlowColorsA[iColorNum].ubRed, GlowColorsA[iColorNum].ubGreen, GlowColorsA[iColorNum].ubBlue ) );

			if( ( usCount == 0 ) || ( usCount != 0 ? !( IsCharacterSelectedForSleep( ( UINT16 )( usCount - 1 ) ) ) : 0 ) )
			{
				LineDraw( TRUE, usX+4, usY, usX2, usY, usColor,pDestBuf);
			}
			if( ( ( usCount == giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1 ) ) || ( usCount != ( giMAXIMUM_NUMBER_OF_PLAYER_SLOTS - 1 ) ? !( IsCharacterSelectedForSleep( ( UINT16 )( usCount + 1 ) ) ) : 0) )
			{
				LineDraw(TRUE, usX+4, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usX2, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			}

			LineDraw(TRUE, usX+ 4, usY, usX + 4, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);
			LineDraw(TRUE, usX2, usY, usX2, usY+GetFontHeight(MAP_SCREEN_FONT)+2, usColor, pDestBuf);

			InvalidateRegion( usX, usY, usX2 + 5, usY + GetFontHeight(MAP_SCREEN_FONT)+3 );
		}
	}
	UnLockVideoSurface( FRAME_BUFFER );
}



void AddCharacter( SOLDIERTYPE *pCharacter )
{
	UINT16 usCount=0;
	UINT16 usVehicleCount = 0, usVehicleLoop = 0;

	// is character valid?
	if( pCharacter == NULL)
	{
		// not valid, leave
		return;
	}

	// valid character?
	if( pCharacter->bActive == FALSE )
	{
		return;
	}

	// go through character list until a blank is reached
	while( (gCharactersList[usCount].fValid)&&(usCount < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS) )
	{
		usCount++;
	}

	Assert( usCount < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS );
	if (usCount >= giMAXIMUM_NUMBER_OF_PLAYER_SLOTS )
	{
		return;
	}

	// copy over soldier id value
	gCharactersList[usCount].usSolID = ( UINT16 )pCharacter->ubID;

	// valid character
	gCharactersList[usCount].fValid = TRUE;

	return;
}


/*
void MoveCharacter(UINT16 uiInitialPosition, UINT16 uiFinalPosition)
{
	if (!gCharactersList[uiInitialPosition].fValid)
		return;
	else
		memcpy(&gCharactersList[uiFinalPosition], &gCharactersList[uiInitialPosition], sizeof(MapScreenCharacterSt));
}


void SwapCharacters(UINT16 uiInitialPosition, UINT16 uiFinalPosition)
{
	MapScreenCharacterSt pTempChar;
	memcpy(&pTempChar, &gCharactersList[uiInitialPosition], sizeof(MapScreenCharacterSt));
	memcpy(&gCharactersList[uiInitialPosition], &gCharactersList[uiFinalPosition], sizeof(MapScreenCharacterSt));
	memcpy(&gCharactersList[uiFinalPosition], &pTempChar, sizeof(MapScreenCharacterSt));
}


void RemoveCharacter(UINT16 uiCharPosition)
{
 memset(&gCharactersList[uiCharPosition], 0, sizeof( MapScreenCharacterSt ));
}
*/


void LoadCharacters( void )
{
	UINT16 uiCount=0;
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;

	pSoldier = MercPtrs[ 0 ];

	// fills array with pressence of player controlled characters
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if(pTeamSoldier->bActive)
		{
			AddCharacter( pTeamSoldier );
			uiCount++;
		}
	}

	// set info char if no selected
	if( bSelectedInfoChar == -1 )
	{
		if( DialogueActive() == FALSE )
		{
			ChangeSelectedInfoChar( 0, TRUE );
		}
	}

	// check if ANYONE was available
	if( uiCount == 0 )
	{
		// no one to show
		ChangeSelectedInfoChar( -1, TRUE );
		bSelectedDestChar = -1;
		bSelectedAssignChar = -1;
		bSelectedContractChar = -1;
		fPlotForHelicopter = FALSE;
	}
}


void DisplayCharacterList()
{
	INT16 sCount=0;
	UINT8 ubForegroundColor = 0;

	if( ( fShowAssignmentMenu == TRUE ) && ( fTeamPanelDirty == FALSE ) )
	{
		SetFontDestBuffer(FRAME_BUFFER ,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,FALSE);
		return;
	}


	// set dest buffer
	SetFontDestBuffer(guiSAVEBUFFER,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,FALSE);
	SetFont( MAP_SCREEN_FONT );
	SetFontBackground(FONT_BLACK);


	for (sCount = 0; sCount < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; sCount++)
	{
		if ( sCount == maxNumberOfMercVisibleInStrategyList ) // marke strogg more mercs
			// skip until at the VEHICLES
			// later - simply break for now
			break ; // sCount = 87 ;
		// skip invalid characters
		if ( gCharactersList[( sCount + FIRSTmercTOdisplay )].fValid == TRUE )
		{
			if( sCount == ( INT16 ) giHighLine )
			{
				ubForegroundColor = FONT_WHITE;
			}
			// check to see if character is still alive
			else if( Menptr[gCharactersList[(sCount + FIRSTmercTOdisplay)].usSolID].stats.bLife == 0 )
			{
				ubForegroundColor = FONT_METALGRAY;
			}
			else if( CharacterIsGettingPathPlotted( sCount ) == TRUE )
			{
				ubForegroundColor = FONT_LTBLUE;
			}
			// in current sector?
			else if( ( Menptr[gCharactersList[( sCount + FIRSTmercTOdisplay )].usSolID].sSectorX == sSelMapX ) &&
							 ( Menptr[gCharactersList[( sCount + FIRSTmercTOdisplay )].usSolID].sSectorY == sSelMapY ) &&
							 ( Menptr[gCharactersList[( sCount + FIRSTmercTOdisplay )].usSolID].bSectorZ == iCurrentMapSectorZ ) )
			{
				// mobile ?
				if( ( Menptr[gCharactersList[( sCount + FIRSTmercTOdisplay )].usSolID].bAssignment < ON_DUTY ) ||
						( Menptr[gCharactersList[( sCount + FIRSTmercTOdisplay )].usSolID].bAssignment == VEHICLE ) )
					ubForegroundColor = FONT_YELLOW;
				else
					ubForegroundColor = FONT_MAP_DKYELLOW;
			}
			else
			{
				// not in current sector
				ubForegroundColor = 5;
			}

			SetFontForeground( ubForegroundColor );

			DrawName( Menptr[gCharactersList[( sCount + FIRSTmercTOdisplay )].usSolID].name, sCount, MAP_SCREEN_FONT);
			DrawLocation(     sCount + FIRSTmercTOdisplay , sCount, MAP_SCREEN_FONT);
			DrawDestination(  sCount + FIRSTmercTOdisplay, sCount, MAP_SCREEN_FONT);
			DrawAssignment(   sCount + FIRSTmercTOdisplay, sCount, MAP_SCREEN_FONT);
			DrawTimeRemaining(sCount, MAP_SCREEN_FONT, ubForegroundColor );
		}
	}

	// draw multiplayer player list
	if (is_networked && is_client)
		DrawMPPlayerList();
	HandleDisplayOfSelectedMercArrows( );
	SetFontDestBuffer(FRAME_BUFFER ,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,FALSE);

	EnableDisableTeamListRegionsAndHelpText( );

	// mark all pop ups as dirty, so that any open assigment menus get reblitted over top of the team list
	MarkAllBoxesAsAltered( );

	return;
}

// OJW - Added 081201
// Draw Lan clients in the vehicle slots
// <TODO> change the graphics, add more relevant info...
// this is just a temp test
void DrawMPPlayerList ()
{
	INT16 usX=0;
	INT16 usY=0;

	static UINT8 pbar_id_player[4];

	if (is_networked && is_client && is_connected)
	{
		wchar_t szPlayerName[30];
		wchar_t szTeam[20];
		wchar_t szCompass[2];
		int row=0;
		for(int i=0; i < 4; i++)
		{
			if (client_names[i] != NULL)
			{
				// setup bouds for player dirty rect and progress bar
				int l,t,b,r;
				l = MP_PLAYER_X+1;
				r = l + MP_PLAYER_W - 1;
				t = MP_ROWSTART_Y+(row*Y_SIZE);
				b = t + Y_SIZE - 1;

				// set up or destroy progress bar if needed
				if (client_downloading[i] == 1 && pbar_id_player[i] == 0)
				{
					fTeamPanelDirty = true;
					
					CreateProgressBarNoBorder(i,l,t,r,b);
					SetProgressBarColor(i,112,154,209);
					SetProgressBarRenderBuffer(i,guiSAVEBUFFER);
					pbar_id_player[i] = 1;
				}
				else if (client_downloading[i] == 0 && pbar_id_player[i] == 1)
				{
					fTeamPanelDirty = true;
					RemoveProgressBar(i);
					pbar_id_player[i] = 0;
				}
				
				// render the progress bar if needed
				if (client_downloading[i] == 1 && pbar_id_player[i] == 1)
				{
					RenderProgressBar(i,client_progress[i]);
				}

				if (client_ready[i]==1)
				{
					SetFontForeground( FONT_GREEN );
				}
				else
				{
					SetFontForeground( FONT_WHITE );
				}

				if (strcmp(client_names[i],"")!=0)
				{
					// valid player
					memset(szPlayerName,0,30*sizeof(wchar_t));
					mbstowcs( szPlayerName,client_names[i],30);
					FindFontCenterCoordinates((short)MP_PLAYER_X + 1, (short)(MP_ROWSTART_Y+(row*Y_SIZE)), (short)MP_PLAYER_W, (short)Y_SIZE, szPlayerName, (long)MAP_SCREEN_FONT, &usX, &usY);
					// do it manually here so as DrawString overrides the dirty rects
					// we want the dirty rect to include progress bar as well as player text
					SetFont(MAP_SCREEN_FONT);
					mprintf(usX,usY,(STR16)szPlayerName);
					int iBack = RegisterBackgroundRect(BGND_FLAG_SINGLE, NULL, l, t, r, b);
					SetBackgroundRectFilled( iBack );
					//DrawString( (STR16)szPlayerName , usX,usY, MAP_SCREEN_FONT);

					// WANNE: No team selection in DM or COOP
					if (cGameType == MP_TYPE_DEATHMATCH || cGameType == MP_TYPE_COOP)
						wcscpy(szTeam, gszMPTeamNames[4]);
					else
						wcscpy(szTeam,gszMPTeamNames[client_teams[i]]);
					FindFontCenterCoordinates((short)MP_TEAM_X + 1, (short)(MP_ROWSTART_Y+(row*Y_SIZE)), (short)MP_TEAM_W, (short)Y_SIZE, szTeam, (long)MAP_SCREEN_FONT, &usX, &usY);
					DrawString( szTeam,usX,(short)usY, MAP_SCREEN_FONT);

					wcscpy(szCompass,(gRandomStartingEdge==1 ? L"?" : gszMPEdgesText[client_edges[i]]));
					FindFontCenterCoordinates((short)MP_COMPASS_X + 1, (short)(MP_ROWSTART_Y+(row*Y_SIZE)), (short)MP_COMPASS_W, (short)Y_SIZE, szCompass, (long)MAP_SCREEN_FONT, &usX, &usY);
					DrawString(szCompass , usX,(short)usY, MAP_SCREEN_FONT);

					row++;
				}

			}
		}

		// Draw the Server Info

		// Server name
		// <TODO> change this text to wrap
		/*wchar_t szServerName[30];
		memset(szServerName,0,30*sizeof(wchar_t));
		mbstowcs( szServerName,cServerName,30);

		SetFontForeground( FONT_YELLOW );
		DrawString( L"Server Name:" , MP_GAMEINFO_X ,MP_ROWSTART_Y, MAP_SCREEN_FONT);
		SetFontForeground( FONT_WHITE );
		DrawString( (STR16)szServerName , MP_GAMEINFO_X ,MP_ROWSTART_Y+(1*Y_SIZE), MAP_SCREEN_FONT);*/

		// Game Type
		SetFontForeground( FONT_YELLOW );
		DrawString( gszMPMapscreenText[0] , MP_GAMEINFO_X ,MP_ROWSTART_Y, MAP_SCREEN_FONT);
		SetFontForeground( FONT_WHITE );

		switch(cGameType)
		{
		case MP_TYPE_DEATHMATCH:
			DrawString( gzMPHScreenText[MPH_DEATHMATCH_TEXT] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(1*Y_SIZE), MAP_SCREEN_FONT);
			break;
		case MP_TYPE_TEAMDEATMATCH:
			DrawString( gzMPHScreenText[MPH_TEAMDM_TEXT] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(1*Y_SIZE), MAP_SCREEN_FONT);
			break;
		case MP_TYPE_COOP:
			DrawString( gzMPHScreenText[MPH_COOP_TEXT] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(1*Y_SIZE), MAP_SCREEN_FONT);
			break;
		}

		// Max Players
		SetFontForeground( FONT_YELLOW );
		DrawString( gszMPMapscreenText[1] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(2*Y_SIZE), MAP_SCREEN_FONT);
		SetFontForeground( FONT_WHITE );

		wchar_t szMaxPlayers[10];
		swprintf(szMaxPlayers,L"%i",cMaxClients);
		DrawString( szMaxPlayers , MP_GAMEINFO_X + StringPixLength(gszMPMapscreenText[1],MAP_SCREEN_FONT),MP_ROWSTART_Y+(2*Y_SIZE), MAP_SCREEN_FONT);

		// Number of Mercs
		SetFontForeground( FONT_YELLOW );
		DrawString( gszMPMapscreenText[2] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(3*Y_SIZE), MAP_SCREEN_FONT);
		SetFontForeground( FONT_WHITE );

		wchar_t szSquadSize[10];
		swprintf(szSquadSize,L"%i",cMaxMercs);
		DrawString( szSquadSize , MP_GAMEINFO_X + StringPixLength(gszMPMapscreenText[2],MAP_SCREEN_FONT),MP_ROWSTART_Y+(3*Y_SIZE), MAP_SCREEN_FONT);

		row = 4;
		if (gRandomMercs)
		{
			// Random Mercs
			SetFontForeground( FONT_YELLOW );
			DrawString( gszMPMapscreenText[5] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(row*Y_SIZE), MAP_SCREEN_FONT);
			SetFontForeground( FONT_WHITE );
			DrawString( gszMPMapscreenText[6] , MP_GAMEINFO_X + StringPixLength(gszMPMapscreenText[5],MAP_SCREEN_FONT),MP_ROWSTART_Y+(row*Y_SIZE), MAP_SCREEN_FONT);
			row++;
		}

		// Server Version
		SetFontForeground( FONT_YELLOW );
		DrawString( gszMPMapscreenText[8] , MP_GAMEINFO_X ,MP_ROWSTART_Y+(row*Y_SIZE), MAP_SCREEN_FONT);
		row++;

		wchar_t szServerVer[30];
		memset(szServerVer,0,30*sizeof(wchar_t));
		mbstowcs( szServerVer,MPVERSION,30);
		SetFontForeground( FONT_WHITE );
		DrawString( szServerVer , MP_GAMEINFO_X ,MP_ROWSTART_Y+(row*Y_SIZE), MAP_SCREEN_FONT);

	}
}
void DestroyMPCompassBox( void )
{
	RemoveBox(ghMPCompassBox);
	ghMPCompassBox = -1;
}
void CreateMPCompassBox( void )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;


 // will create attribute pop up menu for mapscreen assignments
	SGPPoint CompassPosition = { MP_COMPASS_X, MP_ROWSTART_Y };
	SGPRect CompassDimensions = { 0,0, 100, 95};


	// create basic box
	CreatePopUpBox(&ghMPCompassBox, CompassDimensions, CompassPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(ghMPCompassBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghMPCompassBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghMPCompassBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghMPCompassBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghMPCompassBox, 2);

	// set current box to this one
	SetCurrentBox( ghMPCompassBox );

	// add strings for box
	for(uiCounter=0; uiCounter < MAX_EDGES; uiCounter++)
	{

		AddMonoString(&hStringHandle, gszMPEdgesText[uiCounter] );

		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghMPCompassBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghMPCompassBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghMPCompassBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghMPCompassBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghMPCompassBox, FONT_GRAY7 );
	SetBoxSecondaryShade( ghMPCompassBox, FONT_YELLOW );

	// resize box to text
	ResizeBoxToText( ghMPCompassBox );

	//DetermineBoxPositions( );
}
void CreateDestroyMouseRegionsForCompassBox()
{
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	INT32 iCounter = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	if (!is_compass_box_open)
	{

		// grab height of font
		iFontHeight = GetLineSpace( ghMPCompassBox ) + GetFontHeight( GetBoxFont( ghMPCompassBox ) );

		// get x.y position of box
		GetBoxPosition( ghMPCompassBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghMPCompassBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghMPCompassBox );

		// define regions
		for( iCounter = 0; iCounter < MAX_EDGES; iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_DefineRegion( &gCompassMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghMPCompassBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghMPCompassBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, CompassBoxMvtCallback, CompassBoxBtnCallback );

			MSYS_SetRegionUserData( &gCompassMenuRegion[ iCounter ], 0, iCounter );
		}
	}
	else
	{
		// destroy regions
		for( iCounter = 0; iCounter < MAX_EDGES; iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_RemoveRegion( &gCompassMenuRegion[ iCounter ] );
		}
	}
}
void CompassBoxMvtCallback ( MOUSE_REGION* pRegion, INT32 iReason)
{
	// mvt callback handler for assignment region
	INT32 iValue = -1;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( GetBoxShadeFlag( ghMPCompassBox, iValue ) == FALSE )
		{
			HighLightBoxLine( ghMPCompassBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghMPCompassBox );
	}
}
void CompassBoxBtnCallback ( MOUSE_REGION* pRegion, INT32 iReason)
{
	INT32 iValue = -1;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UnHighLightBox( ghMPCompassBox );

		/*switch( iValue )
		{
		case MP_EDGE_NORTH:
			break;
		case MP_EDGE_SOUTH:
			break;
		case MP_EDGE_EAST:
			break;
		case MP_EDGE_WEST:
			break;
		}*/

		// send edge change to all clients, if allowed :)
		send_edgechange(iValue);


		// close the box
		CreateDestroyMouseRegionsForCompassBox();
		HideBox(ghMPCompassBox);
		is_compass_box_open = false;
		fTeamPanelDirty = true;
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		// close the box
		CreateDestroyMouseRegionsForCompassBox();
		HideBox(ghMPCompassBox);
		is_compass_box_open = false;
		fTeamPanelDirty = true;
	}
}
void DestroyMPTeamBox( void )
{
	RemoveBox(ghMPTeamBox);
	ghMPTeamBox = -1;
}
void CreateMPTeamBox( void )
{
 UINT32 hStringHandle;
 UINT32 uiCounter;


 // will create attribute pop up menu for mapscreen assignments
	SGPPoint TeamPosition = { MP_TEAM_X, MP_ROWSTART_Y };
	SGPRect TeamDimensions = { 0,0, 100, 95};


	// create basic box
	CreatePopUpBox(&ghMPTeamBox, TeamDimensions, TeamPosition, (POPUP_BOX_FLAG_CLIP_TEXT|POPUP_BOX_FLAG_CENTER_TEXT|POPUP_BOX_FLAG_RESIZE ));

	// which buffer will box render to
	SetBoxBuffer(ghMPTeamBox, FRAME_BUFFER);

	// border type?
	SetBorderType(ghMPTeamBox,guiPOPUPBORDERS);

	// background texture
	SetBackGroundSurface(ghMPTeamBox, guiPOPUPTEX);

	// margin sizes
	SetMargins(ghMPTeamBox, 6, 6, 4, 4 );

	// space between lines
	SetLineSpace(ghMPTeamBox, 2);

	// set current box to this one
	SetCurrentBox( ghMPTeamBox );

	// add strings for box
	for(uiCounter=0; uiCounter < MAX_MP_TEAMS; uiCounter++)
	{

		AddMonoString(&hStringHandle, gszMPTeamNames[uiCounter] );

		// make sure it is unhighlighted
		UnHighLightLine(hStringHandle);
	}

	// set font type
	SetBoxFont(ghMPTeamBox, MAP_SCREEN_FONT);

	// set highlight color
	SetBoxHighLight(ghMPTeamBox, FONT_WHITE);

	// unhighlighted color
	SetBoxForeground(ghMPTeamBox, FONT_LTGREEN);

	// background color
	SetBoxBackground(ghMPTeamBox, FONT_BLACK);

	// shaded color..for darkened text
	SetBoxShade( ghMPTeamBox, FONT_GRAY7 );
	SetBoxSecondaryShade( ghMPTeamBox, FONT_YELLOW );

	// resize box to text
	ResizeBoxToText( ghMPTeamBox );

	//DetermineBoxPositions( );
}
void CreateDestroyMouseRegionsForTeamBox()
{
	INT32 iFontHeight = 0;
	INT32 iBoxXPosition = 0;
	INT32 iBoxYPosition = 0;
	INT32 iCounter = 0;
	SGPPoint pPosition;
	INT32 iBoxWidth = 0;
	SGPRect pDimensions;

	if (!is_team_box_open)
	{
		// grab height of font
		iFontHeight = GetLineSpace( ghMPTeamBox ) + GetFontHeight( GetBoxFont( ghMPTeamBox ) );

		// get x.y position of box
		GetBoxPosition( ghMPTeamBox, &pPosition);

		// grab box x and y position
		iBoxXPosition = pPosition.iX;
		iBoxYPosition = pPosition.iY;

		// get dimensions..mostly for width
		GetBoxSize( ghMPTeamBox, &pDimensions );

		// get width
		iBoxWidth = pDimensions.iRight;

		SetCurrentBox( ghMPTeamBox );

		// define regions
		for( iCounter = 0; iCounter < MAX_MP_TEAMS; iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_DefineRegion( &gTeamMenuRegion[ iCounter ], 	( INT16 )( iBoxXPosition ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghMPTeamBox ) + ( iFontHeight ) * iCounter ), ( INT16 )( iBoxXPosition + iBoxWidth ), ( INT16 )( iBoxYPosition + GetTopMarginSize( ghMPTeamBox ) + ( iFontHeight ) * ( iCounter + 1 ) ), MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, TeamBoxMvtCallback, TeamBoxBtnCallback );

			MSYS_SetRegionUserData( &gTeamMenuRegion[ iCounter ], 0, iCounter );
		}
	}
	else
	{
		// destroy regions
		for( iCounter = 0; iCounter < MAX_MP_TEAMS; iCounter++ )
		{
			// add mouse region for each line of text..and set user data
			MSYS_RemoveRegion( &gTeamMenuRegion[ iCounter ] );
		}
	}
}
void TeamBoxMvtCallback ( MOUSE_REGION* pRegion, INT32 iReason)
{
	// mvt callback handler for assignment region
	INT32 iValue = -1;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( GetBoxShadeFlag( ghMPTeamBox, iValue ) == FALSE )
		{
			HighLightBoxLine( ghMPTeamBox, iValue );
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		// unhighlight all strings in box
		UnHighLightBox( ghMPTeamBox );
	}
}
void TeamBoxBtnCallback ( MOUSE_REGION* pRegion, INT32 iReason)
{
	INT32 iValue = -1;

	iValue = MSYS_GetRegionUserData( pRegion, 0 );


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UnHighLightBox( ghMPTeamBox );

		/*switch( iValue )
		{
		case MP_EDGE_NORTH:
			break;
		case MP_EDGE_SOUTH:
			break;
		case MP_EDGE_EAST:
			break;
		case MP_EDGE_WEST:
			break;
		}*/

		// send team change to all clients, if allowed :)
		send_teamchange(iValue);


		// close the box
		CreateDestroyMouseRegionsForTeamBox();
		HideBox(ghMPTeamBox);
		is_team_box_open = false;
		fTeamPanelDirty = true;
	}
	else if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		// close the box
		CreateDestroyMouseRegionsForTeamBox();
		HideBox(ghMPTeamBox);
		is_team_box_open = false;
		fTeamPanelDirty = true;
	}
}


// THIS IS STUFF THAT RUNS *ONCE* DURING APPLICATION EXECUTION, AT INITIAL STARTUP
UINT32 MapScreenInit(void)
{
	VOBJECT_DESC VObjectDesc;

	// setup message box system
	InitGlobalMessageList( );

	// init palettes for big map
	InitializePalettesForMap( );

	// set up mapscreen fast help text
	SetUpMapScreenFastHelpText( );

	// set up leave list arrays for dismissed mercs
	InitLeaveList( );
	
	InitializeMilitiaPopup();

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\group_confirm.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiUpdatePanel ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\group_confirm_tactical.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiUpdatePanelTactical ) );

	return TRUE;
}



UINT32 MapScreenShutdown(void)
{
	// destroy some popup boxes
	fShowAssignmentMenu = FALSE;
	CreateDestroyAssignmentPopUpBoxes( );
	// OJW
	if (is_networked)
	{
		DestroyMPCompassBox();
		DestroyMPTeamBox();
	}

	fShowContractMenu = FALSE;
	DetermineIfContractMenuCanBeShown( );
	RemoveBox(ghContractBox);
	ghContractBox = -1;

	if ( ghRemoveMercAssignBox != -1 )
	{
		fShowRemoveMenu = FALSE;
		RemoveBox( ghRemoveMercAssignBox );
		ghRemoveMercAssignBox = -1;
	}

	// free up alloced mapscreen messages
	FreeGlobalMessageList( );

	ShutDownPalettesForMap( );

	// free memory for leave list arrays for dismissed mercs
	ShutDownLeaveList( );

	DeleteVideoObjectFromIndex(guiUpdatePanel );
	DeleteVideoObjectFromIndex(guiUpdatePanelTactical);

	return TRUE;
}



UINT32 MapScreenHandle(void)
{
	UINT32 uiNewScreen;
	VSURFACE_DESC	vs_desc;
	VOBJECT_DESC VObjectDesc;
//	static BOOLEAN fSecondFrame = FALSE;
	INT32 iCounter = 0;



	//DO NOT MOVE THIS FUNCTION CALL!!!
	//This determines if the help screen should be active
	if( ShouldTheHelpScreenComeUp( HelpScreenDetermineWhichMapScreenHelpToShow(), FALSE ) )
	{
		// handle the help screen
		HelpScreenHandler();
		return( MAP_SCREEN );
	}

	MoveRadarScreen();

	// shaded screen, leave
	if ( gfLoadPending == 2 )
	{
		gfLoadPending = FALSE;

		// Load Sector
		// BigCheese
		if ( !SetCurrentWorldSector( sSelMapX, sSelMapY , ( INT8 )iCurrentMapSectorZ ) )
		{
			// Cannot load!
		}
		else
		{
			CreateDestroyMapInvButton();
			// define our progress bar
			//CreateProgressBar( 0, 118, 183, 522, 202 );

		}
		return( MAP_SCREEN );
	}

	if ( !fInMapMode )
	{
		gfFirstMapscreenFrame = TRUE;

		InitPreviousPaths();

		// HEADROCK HAM 3.6: Init coordinates for new variable-sized message window
		InitMapScreenInterfaceBottomCoords();

		// if arrival sector is invalid, reset to A9
		if ( ( gsMercArriveSectorX <	1 ) || ( gsMercArriveSectorY <	1 ) ||
				( gsMercArriveSectorX > 16 ) || ( gsMercArriveSectorY > 16 ) )
		{
			// HEADROCK HAM 3.5: Externalized coordinates
			gsMercArriveSectorX = gGameExternalOptions.ubDefaultArrivalSectorX;
			gsMercArriveSectorY = gGameExternalOptions.ubDefaultArrivalSectorY;
		}

		gfInConfirmMapMoveMode = FALSE;
		gfInChangeArrivalSectorMode = FALSE;

		fLeavingMapScreen = FALSE;
		fResetTimerForFirstEntryIntoMapScreen = TRUE;
		fFlashAssignDone = FALSE;
		gfEnteringMapScreen = 0;

		guiTacticalInterfaceFlags |= INTERFACE_MAPSCREEN;

//		fDisabledMapBorder = FALSE;

		// handle the sort buttons
		AddTeamPanelSortButtonsForMapScreen( );

		// OJW - 20081204
		if (is_networked)
			AddMPButtonsForMapScreen();
		// load bottom graphics
		LoadMapScreenInterfaceBottom( );

		MoveToEndOfMapScreenMessageList( );

		// if the current time compression mode is something legal in mapscreen, keep it
		if ( ( giTimeCompressMode >= TIME_COMPRESS_5MINS ) && ( giTimeCompressMode <= TIME_COMPRESS_60MINS ) )
		{
			// leave the current time compression mode set, but DO stop it
			StopTimeCompression();
		}
		else
		{
			// set compressed mode to X0 (which also stops time compression)
			SetGameTimeCompressionLevel( TIME_COMPRESS_X0 );
		}

		// disable video overlay for tactical scroll messages
		EnableDisableScrollStringVideoOverlay( FALSE );

		CreateDestroyInsuranceMouseRegionForMercs( TRUE );

		// ATE: Init tactical interface interface ( always to team panel )
		//SetCurrentInterfacePanel( TEAM_PANEL );
		// Do some things to this now that it's initialized
		//MSYS_DisableRegion( &gViewportRegion );
		//MSYS_DisableRegion( &gRadarRegion );
		//Disable all faces
		SetAllAutoFacesInactive( );


		if(fPreLoadedMapGraphics == FALSE )
		{
			vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
		
			if (iResolution == 0)
			{
				// ------------
				// 336 x 288	(size without the black area on top and left
				// Important:	The size (including black area on top and left) of the PCX file has to be an even number for width and height,
				//				otherwise the blitting method which shrinks the image by 50% does not work.
				// ------------
				
				MAP_GRID_X = 21;		// Horizontal size of a square
				MAP_GRID_Y = 18;		// Vertical size of a square

				MAP_BORDER_X_OFFSET	= (((SCREEN_WIDTH - 261) - 380) / 2);	// 380: Width of the MBS.sti file
				MAP_BORDER_Y_OFFSET = (((SCREEN_HEIGHT - 121) - 360) / 2);  // 360: Height of the MBS.sti file

				MAP_VIEW_START_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 9);	// 9: Constant, do not change it.
				MAP_VIEW_START_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 10);	// 10: Constant, do not change it.
				
				// Maus Area (Ist die reine Größe der Karte/2 (ohne dem schwarzen Rand (links und oben))
				MAP_VIEW_WIDTH = 336;			// Width of the map (without the black area on the left)
				MAP_VIEW_HEIGHT	= 288 + 10;		// Height of the map (without the black area on the top) + constant value of 10

				
				MAP_FONT = BLOCKFONT2;

				// The postion of the map numbers (1-16) above the map (horizontal)
				MAP_HORT_INDEX_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 31);
				MAP_HORT_INDEX_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 11);
				MAP_HORT_HEIGHT = GetFontHeight(MAP_FONT);

				// The position of the latters (A-P) on the left side of the map (vertical)
				MAP_VERT_INDEX_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 15);
				MAP_VERT_INDEX_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 29);
				
				MAP_VERT_WIDTH = GetFontHeight(MAP_FONT);

				ETA_FONT = BLOCKFONT2;

				// Helicopter ETA box
				MAP_HELICOPTER_ETA_POPUP_X			= (400 + iScreenWidthOffset);
				MAP_HELICOPTER_ETA_POPUP_Y			= (250 + iScreenHeightOffset);
				MAP_HELICOPTER_UPPER_ETA_POPUP_Y	= (50 + iScreenHeightOffset);
				MAP_HELICOPTER_ETA_POPUP_WIDTH		= 120;
				MAP_HELICOPTER_ETA_POPUP_HEIGHT		= 68;

				// Map Level string
				MAP_LEVEL_STRING_X	= (SCREEN_WIDTH - 208);
				MAP_LEVEL_STRING_Y	= (SCREEN_HEIGHT - 175);

				strcpy(vs_desc.ImageFile, "INTERFACE\\b_map.pcx");
			}
			else if (iResolution == 1)
			{
				// ------------
				// 480 x 400
				// ------------

				MAP_GRID_X = 30;
				MAP_GRID_Y = 25;

				MAP_BORDER_X_OFFSET	= (((SCREEN_WIDTH - 261) - 540) / 2);
				MAP_BORDER_Y_OFFSET = (((SCREEN_HEIGHT - 121) - 480) / 2);

				MAP_VIEW_START_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 9);
				MAP_VIEW_START_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 10);
				
				MAP_VIEW_WIDTH = 480;				
				MAP_VIEW_HEIGHT	= 400 + 10 + 7;

				// The numbers on the horizontal line
				MAP_FONT = BLOCKFONT2;

				MAP_HORT_INDEX_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 38);
				MAP_HORT_INDEX_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 15);
				MAP_HORT_HEIGHT = GetFontHeight(MAP_FONT);

				MAP_VERT_INDEX_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 19);
				MAP_VERT_INDEX_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 36);
				MAP_VERT_WIDTH = GetFontHeight(MAP_FONT);

				ETA_FONT = BLOCKFONT2;

				// Helicopter ETA box
				MAP_HELICOPTER_ETA_POPUP_X			= (400 + iScreenWidthOffset);
				MAP_HELICOPTER_ETA_POPUP_Y			= (250 + iScreenHeightOffset + 58);
				MAP_HELICOPTER_UPPER_ETA_POPUP_Y	= (50 + iScreenHeightOffset - 40);
				MAP_HELICOPTER_ETA_POPUP_WIDTH		= 120;
				MAP_HELICOPTER_ETA_POPUP_HEIGHT		= 68;

				// Map Level string
				MAP_LEVEL_STRING_X	= (SCREEN_WIDTH - 208 - 80);
				MAP_LEVEL_STRING_Y	= (SCREEN_HEIGHT - 175);

				strcpy(vs_desc.ImageFile, "INTERFACE\\b_map_800x600.pcx");
			}
			else if (iResolution == 2)
			{
				// ------------
				// 688 x 560
				// ------------

				MAP_GRID_X = 43;
				MAP_GRID_Y = 35;

				MAP_BORDER_X_OFFSET	= (((SCREEN_WIDTH - 261) - 764) / 2);
				MAP_BORDER_Y_OFFSET = (((SCREEN_HEIGHT - 121) - 648) / 2);

				MAP_VIEW_START_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 9);
				MAP_VIEW_START_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 10);

				MAP_VIEW_WIDTH = 688;			
				MAP_VIEW_HEIGHT = 560 + 10 + 17;

				MAP_FONT = FONT12ARIAL;

				MAP_HORT_INDEX_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 53);
				MAP_HORT_INDEX_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 19);
				MAP_HORT_HEIGHT = GetFontHeight(MAP_FONT);

				MAP_VERT_INDEX_X = (MAP_BORDER_X + MAP_BORDER_X_OFFSET + 24);
				MAP_VERT_INDEX_Y = (MAP_BORDER_Y + MAP_BORDER_Y_OFFSET + 44);
				MAP_VERT_WIDTH = GetFontHeight(MAP_FONT);

				ETA_FONT = FONT12ARIAL;

				// Helicopter ETA box
				MAP_HELICOPTER_ETA_POPUP_X			= (400 + iScreenWidthOffset);
				MAP_HELICOPTER_ETA_POPUP_Y			= (250 + iScreenHeightOffset + 120);
				MAP_HELICOPTER_UPPER_ETA_POPUP_Y	= (50 + iScreenHeightOffset - 100);
				MAP_HELICOPTER_ETA_POPUP_WIDTH		= 120;
				MAP_HELICOPTER_ETA_POPUP_HEIGHT		= 76;

				// Map Level string
				MAP_LEVEL_STRING_X	= (SCREEN_WIDTH - 208 - 187);
				MAP_LEVEL_STRING_Y	= (SCREEN_HEIGHT - 175);

				strcpy(vs_desc.ImageFile, "INTERFACE\\b_map_1024x768.pcx");
			}

			LoadMapBorderGraphics( );

		//fInterfacePanelDirty=DIRTYLEVEL2;
		//RenderTacticalInterface();
		
		// Grab the Map image

		
		CHECKF(AddVideoSurface(&vs_desc, &guiBIGMAP));

		vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
		strcpy(vs_desc.ImageFile, "INTERFACE\\popupbackground.pcx");
		CHECKF(AddVideoSurface(&vs_desc, &guiPOPUPTEX));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\SAM.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiSAMICON));

		// VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		// FilenameForBPP("INTERFACE\\s_map.sti", VObjectDesc.ImageFile);
		// CHECKF( AddVideoObject( &VObjectDesc, &guiMAP ) );
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mapcursr.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiMAPCURSORS));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\Mine_1.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiSubLevel1));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\Mine_2.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiSubLevel2));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\Mine_3.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiSubLevel3));

		//VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		//FilenameForBPP("INTERFACE\\addonslcp.sti", VObjectDesc.ImageFile);
		//CHECKF(AddVideoObject(&VObjectDesc, &guiCORNERADDONS));

		//VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		//FilenameForBPP("INTERFACE\\mapborder.sti", VObjectDesc.ImageFile);
		//CHECKF(AddVideoObject(&VObjectDesc, &guiMAPBORDER));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\sleepicon.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiSleepIcon));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\charinfo.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiCHARINFO));
		/*strcpy(vs_desc.ImageFile, "INTERFACE\\playlist3.pcx");
		CHECKF(AddVideoSurface( &vs_desc, &guiCHARLIST ));*/

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		if (!is_networked)
		{
			if (iResolution == 0)
			{
				FilenameForBPP("INTERFACE\\newgoldpiece3.sti", VObjectDesc.ImageFile );
			}
			else if (iResolution == 1)
			{
				FilenameForBPP("INTERFACE\\newgoldpiece3_800x600.sti", VObjectDesc.ImageFile );
			}
			else if (iResolution == 2)
			{
				FilenameForBPP("INTERFACE\\newgoldpiece3_1024x768.sti", VObjectDesc.ImageFile );
			}
		}
		else
		{
			// OJW - 20081204 - change mapscreen interface for MP games
			if (iResolution == 0)
			{
					FilenameForBPP("INTERFACE\\mpgoldpiece3.sti", VObjectDesc.ImageFile );
			}
			else if (iResolution == 1)
			{
					FilenameForBPP("INTERFACE\\mpgoldpiece3_800x600.sti", VObjectDesc.ImageFile );
			}
			else if (iResolution == 2)
			{
				FilenameForBPP("INTERFACE\\mpgoldpiece3_1024x768.sti", VObjectDesc.ImageFile );
			}
		}
		CHECKF(AddVideoObject(&VObjectDesc, &guiCHARLIST));

		//VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		//FilenameForBPP("INTERFACE\\mapbordercorner.sti", VObjectDesc.ImageFile);
		//CHECKF(AddVideoObject(&VObjectDesc, &guiMAPCORNER));

		// VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		// FilenameForBPP("INTERFACE\\popup.sti", VObjectDesc.ImageFile);
		// CHECKF(AddVideoObject(&VObjectDesc, &guiPOPUPBORDERS));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

		if (iResolution == 0)
			FilenameForBPP("INTERFACE\\boxes.sti", VObjectDesc.ImageFile);
		else if (iResolution == 1)
			FilenameForBPP("INTERFACE\\boxes_800x600.sti", VObjectDesc.ImageFile);
		else if (iResolution == 2)
			FilenameForBPP("INTERFACE\\boxes_1024x768.sti", VObjectDesc.ImageFile);

		CHECKF(AddVideoObject(&VObjectDesc, &guiCHARICONS));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\incross.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiCROSS));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		if (iResolution == 0)
		{
			FilenameForBPP("INTERFACE\\mapinv.sti", VObjectDesc.ImageFile);
		}
		else if (iResolution == 1)
		{
			FilenameForBPP("INTERFACE\\mapinv_800x600.sti", VObjectDesc.ImageFile);
		}
		else if (iResolution == 2)
		{
			FilenameForBPP("INTERFACE\\mapinv_1024x768.sti", VObjectDesc.ImageFile);
		}
		if(!AddVideoObject(&VObjectDesc, &guiMAPINV))
		{
			FilenameForBPP("INTERFACE\\mapinv.sti", VObjectDesc.ImageFile);
		}
		CHECKF(AddVideoObject(&VObjectDesc, &guiMAPINV));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\map_inv_2nd_gun_cover.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiMapInvSecondHandBlockout));

		// the upper left corner piece icons
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\top_left_corner_icons.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiULICONS));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\map_item.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiORTAICON));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\prison.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiTIXAICON));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\merc_between_sector_icons.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiCHARBETWEENSECTORICONS));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\merc_mvt_green_arrows.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiCHARBETWEENSECTORICONSCLOSE));


		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\GreenArr.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiLEVELMARKER));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\Helicop.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiHelicopterIcon));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\eta_pop_up.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiMapBorderEtaPopUp));

		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\pos2.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &guiMapBorderHeliSectors));


			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			FilenameForBPP("INTERFACE\\secondary_gun_hidden.sti", VObjectDesc.ImageFile);
			CHECKF( AddVideoObject( &VObjectDesc, &guiSecItemHiddenVO ) );

			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			FilenameForBPP("INTERFACE\\selectedchararrow.sti", VObjectDesc.ImageFile);
			CHECKF( AddVideoObject( &VObjectDesc, &guiSelectedCharArrow ) );


			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			FilenameForBPP("INTERFACE\\mine.sti", VObjectDesc.ImageFile);
			CHECKF( AddVideoObject( &VObjectDesc, &guiMINEICON ) );

			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			
			if (iResolution == 0)
				sprintf( VObjectDesc.ImageFile, "INTERFACE\\hilite.sti" );
			else if (iResolution == 1)
				sprintf( VObjectDesc.ImageFile, "INTERFACE\\hilite_800x600.sti" );
			else if (iResolution == 2)
				sprintf( VObjectDesc.ImageFile, "INTERFACE\\hilite_1024x768.sti" );
			
			AddVideoObject( &VObjectDesc, &guiSectorLocatorGraphicID );

			VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
			FilenameForBPP("INTERFACE\\BullsEye.sti", VObjectDesc.ImageFile);
			CHECKF(AddVideoObject(&VObjectDesc, &guiBULLSEYE));

			HandleLoadOfMapBottomGraphics( );

			// load the militia pop up box
			LoadMilitiaPopUpBox( );

				// graphic for pool inventory
				LoadInventoryPoolGraphic( );

			//Kris:	Added this because I need to blink the icons button.
			VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
			sprintf( VObjectDesc.ImageFile, "INTERFACE\\newemail.sti" );
			AddVideoObject( &VObjectDesc, &guiNewMailIcons );
		}

		// create buttons
		CreateButtonsForMapBorder( );

		// create mouse regions for level markers
		CreateMouseRegionsForLevelMarkers( );

		// change selected sector/level if necessary
		// NOTE: Must come after border buttons are created, since it may toggle them!
		if( AnyMercsHired( ) == FALSE )
		{
			// select starting sector (A9 - Omerta)
			// HEADROCK HAM 3.5: Externalized!
			ChangeSelectedMapSector( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );
		}
		else if( ( gWorldSectorX > 0 ) && ( gWorldSectorY > 0 ) && ( gbWorldSectorZ != -1 ) )
		{
			// select currently loaded sector as the map sector
			ChangeSelectedMapSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}
		else	// no loaded sector
		{
			// only select A9 - Omerta IF there is no current selection, otherwise leave it as is
			if ( ( sSelMapX == 0 ) || ( sSelMapY == 0 ) || ( iCurrentMapSectorZ == -1 ) )
			{
				// HEADROCK HAM 3.5: Externalized!
				ChangeSelectedMapSector( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );
			}
		}


		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\Bars.sti", VObjectDesc.ImageFile);
		CHECKF( AddVideoObject( &VObjectDesc, &guiBrownBackgroundForTeamPanel ) );

		// we are in fact in the map, do not repeat this sequence
		fInMapMode = TRUE;

		// dirty map
		fMapPanelDirty=TRUE;

		// dirty team region
		fTeamPanelDirty = TRUE;

		// dirty info region
		fCharacterInfoPanelDirty = TRUE;

		// direty map bottom region
		fMapScreenBottomDirty = TRUE;

		// tactical scroll of messages not allowed to beep until new message is added in tactical
		fOkToBeepNewMessage = FALSE;

		// not in laptop, not about to go there either
		fLapTop=FALSE;

		// reset show aircraft flag
		//fShowAircraftFlag = FALSE;

		// reset fact we are showing white bounding box around face
		fShowFaceHightLight = FALSE;
		fShowItemHighLight = FALSE;

		// reset all selected character flags
		ResetAllSelectedCharacterModes( );

		if( fFirstTimeInMapScreen == TRUE )
		{
			fFirstTimeInMapScreen = FALSE;
//			fShowMapScreenHelpText = TRUE;
		}

		fShowMapInventoryPool = FALSE;

		// init character list - set all values in the list to 0
		InitalizeVehicleAndCharacterList( );

		// deselect all entries
		ResetSelectedListForMapScreen( );

		LoadCharacters();


		// set up regions
		MSYS_DefineRegion( &gMapViewRegion, MAP_VIEW_START_X + MAP_GRID_X, MAP_VIEW_START_Y + MAP_GRID_Y,MAP_VIEW_START_X + MAP_VIEW_WIDTH+MAP_GRID_X-1, MAP_VIEW_START_Y + MAP_VIEW_HEIGHT-1 + 8, MSYS_PRIORITY_HIGH - 3,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
		MSYS_DefineRegion( &gCharInfoHandRegion, ((INT16)( 4 )), ((INT16)( 81 )) ,((INT16)(62)), ((INT16)(103)), MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, ItemRegionMvtCallback , ItemRegionBtnCallback );
		MSYS_DefineRegion( &gCharInfoFaceRegion, (INT16) PLAYER_INFO_FACE_START_X, (INT16) PLAYER_INFO_FACE_START_Y, (INT16) PLAYER_INFO_FACE_END_X, (INT16) PLAYER_INFO_FACE_END_Y, MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, FaceRegionBtnCallback );

		MSYS_DefineRegion(&gMPanelRegion, INV_REGION_X, INV_REGION_Y, INV_REGION_X+INV_REGION_WIDTH, INV_REGION_Y+INV_REGION_HEIGHT, MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK,InvmaskRegionBtnCallBack);
		// screen mask for animated cursors
		MSYS_DefineRegion( &gMapScreenMaskRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_LOW,
							CURSOR_NORMAL, MSYS_NO_CALLBACK, MapScreenMarkRegionBtnCallback);

		// set help text for item glow region
		SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );

		// init the timer menus
		InitTimersForMoveMenuMouseRegions( );

		giMapContractButtonImage = LoadButtonImage( "INTERFACE\\contractbutton.sti" ,-1,0,-1,1,-1 );

		// buttonmake
		giMapContractButton= QuickCreateButton( giMapContractButtonImage, CONTRACT_X + 5, CONTRACT_Y - 1,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 5,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ContractButtonCallback);

		SpecifyButtonText( giMapContractButton, pContractButtonString[ 0 ] );
		SpecifyButtonFont( giMapContractButton, MAP_SCREEN_FONT );
		SpecifyButtonUpTextColors( giMapContractButton, CHAR_TEXT_FONT_COLOR, FONT_BLACK );
		SpecifyButtonDownTextColors( giMapContractButton, CHAR_TEXT_FONT_COLOR, FONT_BLACK );

		// create mouse region for pause clock
		CreateMouseRegionForPauseOfClock( CLOCK_REGION_START_X, CLOCK_REGION_START_Y );

		// WANNE: The number of merc we can display in the list, depends on the resolution
		if (iResolution == 0)
		{
			maxNumberOfMercVisibleInStrategyList = 22;
		}
		else if (iResolution == 1)
		{
			maxNumberOfMercVisibleInStrategyList = 34;
		}
		else if (iResolution == 2)
		{
			if (giMAXIMUM_NUMBER_OF_PLAYER_SLOTS <= 51)
				maxNumberOfMercVisibleInStrategyList = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS;
			else
				maxNumberOfMercVisibleInStrategyList = 51;
		}

		// create mouse regions
		CreateMouseRegionsForTeamList( );

		ReBuildCharactersList( );

		// create status bar region
		CreateMapStatusBarsRegion( );

		// Add region
		MSYS_AddRegion( &gMapViewRegion);
		MSYS_AddRegion( &gCharInfoFaceRegion);
		MSYS_AddRegion( &gMPanelRegion);

	if ( !gfFadeOutDone && !gfFadeIn )
	{
		MSYS_SetCurrentCursor(SCREEN_CURSOR);
	}
		MSYS_DisableRegion(&gMPanelRegion);

		// create contract box
		CreateContractBox( NULL );

		// create the permanent boxes for assignment and its submenus
		fShowAssignmentMenu = TRUE;
		CreateDestroyAssignmentPopUpBoxes( );
		fShowAssignmentMenu = FALSE;

		// OJW
		if (is_networked)
		{
			CreateMPCompassBox();
			CreateMPTeamBox();
		}

		// create merc remove box
		CreateMercRemoveAssignBox( );

		// test message
	// TestMessageSystem( );

		// fill in
		ColorFillVideoSurfaceArea( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( RGB_NEAR_BLACK ) );
		ColorFillVideoSurfaceArea(	FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( RGB_NEAR_BLACK ) );


		if( ( fFirstTimeInMapScreen == TRUE ) && ( AnyMercsHired( ) == FALSE ) )
		{
			// render both panels for the restore
			RenderMapRegionBackground( );
			RenderTeamRegionBackground( );

			// now do the warning box
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapErrorString[ 4 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
		}


		fFirstTimeInMapScreen = FALSE;

		if( gpCurrentTalkingFace != NULL )
		{
			// GO FROM GAMESCREEN TO MAPSCREEN
			// REMOVE OLD UI
			// Set face inactive!
			//gpCurrentTalkingFace->fCanHandleInactiveNow = TRUE;
			//SetAutoFaceInActive( gpCurrentTalkingFace->iID );
			//gfFacePanelActive = FALSE;


			// make him continue talking
			ContinueDialogue( MercPtrs[ gpCurrentTalkingFace->ubSoldierID ], FALSE );

			// reset diabled flag
			//gpCurrentTalkingFace->fDisabled = FALSE;

			// Continue his talking!


		}

		fOneFrame = FALSE;

		if( fEnterMapDueToContract == TRUE )
		{
			if( pContractReHireSoldier )
			{
				FindAndSetThisContractSoldier( pContractReHireSoldier );
			}
			fEnterMapDueToContract = FALSE;
		}

	}


	// if not going anywhere else
	if ( guiPendingScreen == NO_PENDING_SCREEN )
	{
		if ( HandleFadeOutCallback( ) )
		{
			// force mapscreen to be reinitialized even though we're already in it
			EndMapScreen( TRUE );
			return( MAP_SCREEN );
		}

		if ( HandleBeginFadeOut( MAP_SCREEN ) )
		{
			return( MAP_SCREEN );
		}
	}


	// check to see if we need to rebuild the characterlist for map screen
	HandleRebuildingOfMapScreenCharacterList( );

	HandleStrategicTurn( );


/*
	// update cursor based on state
	if( ( bSelectedDestChar == -1 ) && ( fPlotForHelicopter == FALSE ) && ( gfInChangeArrivalSectorMode == FALSE ) )
	{
		// reset cursor
	if ( !gfFadeIn )
	{
		ChangeMapScreenMaskCursor( CURSOR_NORMAL );
	}
	}
*/

	// check if we are going to create or destroy map border graphics?
	CreateDestroyMapInventoryPoolButtons( FALSE );

	// set up buttons for mapscreen scroll
//	HandleMapScrollButtonStates( );


	// don't process any input until we've been through here once
	if( gfFirstMapscreenFrame == FALSE )
	{
		// OJW - 20081129
		// Auto-start server or join server
		if (is_networked)
		{
			// this function does nothing if
			// already connected
			NetworkAutoStart();

			// handle changing picture on start game button
			// for server
			if (is_server)
			{
				if (allowlaptop && ButtonList [ giMapMPButton[3] ]->ImageNum == giMapMPButtonImage [4])
				{
					// Change server's button to "ready"
					SpecifyButtonImage( giMapMPButton [3] , giMapMPButtonImage [3] );
				}
			}
		}
		// Handle Interface
		uiNewScreen = HandleMapUI( );
		if ( uiNewScreen != MAP_SCREEN )
		{
			return( MAP_SCREEN );
		}
	}


	// handle flashing of contract column for any mercs leaving very soon
	HandleContractTimeFlashForMercThatIsAboutLeave( );

	if( ( fShownAssignmentMenu == FALSE ) && ( fShowAssignmentMenu == TRUE ) )
	{
		// need a one frame pause
		fShownAssignmentMenu = fShowAssignmentMenu;
		fShowAssignmentMenu = FALSE;
		fOneFrame = TRUE;
	}
	else if( ( fShownContractMenu == FALSE ) && ( fShowContractMenu == TRUE ) )
	{
		fShownContractMenu = fShowContractMenu;
		fShowContractMenu = FALSE;
		fOneFrame = TRUE;
	}
	else if( fOneFrame )
	{
		// one frame passed
		fShowContractMenu = fShownContractMenu;
		fShowAssignmentMenu = fShownAssignmentMenu;
		fOneFrame = FALSE;
	}

	if( ( fShownAssignmentMenu == FALSE ) && ( fShowAssignmentMenu == FALSE ) )
	{
		bSelectedAssignChar = -1;
	}

	HandlePostAutoresolveMessages();

	//	UpdateLevelButtonStates( );

	// NOTE: This must happen *before* UpdateTheStateOfTheNextPrevMapScreenCharacterButtons()
	CreateDestroyMapCharacterScrollButtons( );

	// update the prev next merc buttons
	UpdateTheStateOfTheNextPrevMapScreenCharacterButtons( );

	// handle for inventory
	HandleCursorOverRifleAmmo( );

	// check contract times, update screen if they do change
	CheckAndUpdateBasedOnContractTimes( );

	// handle flashing of assignment strings when merc's assignment is done
	HandleAssignmentsDoneAndAwaitingFurtherOrders( );

	// handle timing for the various glowing higlights
	HandleCommonGlowTimer( );

	// are we attempting to plot a foot/vehicle path during aircraft mode..if so, stop it
	CheckIfPlottingForCharacterWhileAirCraft( );

	// check to see if helicopter is available
	//CheckIfHelicopterAvailable( );
	if( fShowMapInventoryPool )
	{
		HandleFlashForHighLightedItem( );
	}


//	CreateDestroyMovementBox( 0,0,0 );

	// Deque all game events
	DequeAllGameEvents( TRUE );

	// Handle Interface Stuff
	SetUpInterface( );

	// reset time compress has occured
	ResetTimeCompressHasOccured( );

	// handle change in info char
	HandleChangeOfInfoChar( );

	// update status of contract box
	UpDateStatusOfContractBox( );

	// error check of assignments
	UpdateBadAssignments( );

	// if cursor has left map..will need to update temp path plotting and cursor
	CheckToSeeIfMouseHasLeftMapRegionDuringPathPlotting( );

	// update assignment menus and submenus
	HandleShadingOfLinesForAssignmentMenus( );

	// check which menus can be shown right now
	DetermineWhichAssignmentMenusCanBeShown( );

	DetermineWhichMilitiaControlMenusCanBeShown( ); //lal
	// determine if contract menu can be shown
	DetermineIfContractMenuCanBeShown( );

	// if pre battle and inventory up?..get rid of inventory
	HandlePreBattleInterfaceWithInventoryPanelUp( );

	// create destroy trash can region
	CreateDestroyTrashCanRegion( );

	// update these buttons
	UpdateStatusOfMapSortButtons( );

	// if in inventory mode, make sure it's still ok
	CheckForInventoryModeCancellation();

	// restore background rects
	RestoreBackgroundRects( );

	InterruptTimeForMenus( );

	// place down background
	BlitBackgroundToSaveBuffer( );

	if( fLeavingMapScreen == TRUE )
	{
		return( MAP_SCREEN );
	}

	if( fDisableDueToBattleRoster == FALSE )
	{
/*
		// ATE: OK mark is rendering the item every frame - which isn't good
	// however, don't want to break the world here..
	// this line was added so that when the ItemGlow() is on,
	// we're not rendering also, else glow looks bad
	if ( !fShowItemHighLight )
	{
		RenderHandPosItem();
	}
*/

		if( fDrawCharacterList )
		{
			if ( !fShowInventoryFlag )
			{
				// if we are not in inventory mode, show character list
				HandleHighLightingOfLinesInTeamPanel( );

				DisplayCharacterList();
			}

			fDrawCharacterList = FALSE;
		}
	}


	if(	!fShowMapInventoryPool && !gfPauseDueToPlayerGamePause && !IsMapScreenHelpTextUp( )	/* && !fDisabledMapBorder */ )
	{
		RenderMapCursorsIndexesAnims( );
	}


	if( fDisableDueToBattleRoster == FALSE )
	{

		// render status bar
		HandleCharBarRender( );
	}

	if( fShowInventoryFlag || fDisableDueToBattleRoster )
	{
		for( iCounter = 0; iCounter < MAX_SORT_METHODS; iCounter++ )
		{
			UnMarkButtonDirty( giMapSortButton[ iCounter ] );
		}
	}

	if( fShowContractMenu || fDisableDueToBattleRoster )
	{
		UnMarkButtonDirty( giMapContractButton );
	}

	// handle any old messages
	ScrollString( );


	HandleSpontanousTalking( );

	if( ( fDisableDueToBattleRoster == FALSE ) )
	{
		// remove the move box once user leaves it
		CreateDestroyMovementBox( 0,0,0 );

		// this updates the move box contents when changes took place
		ReBuildMoveBox( );
	}

	if( ( fDisableDueToBattleRoster == FALSE )&&( ( fShowAssignmentMenu == TRUE )||( fShowContractMenu == TRUE ) ) )
	{
		// highlight lines?
		HandleHighLightingOfLinesInTeamPanel( );

		// render glow for contract region
		ContractBoxGlow( );
		GlowTrashCan( );

		// handle changing of highlighted lines
		HandleChangeOfHighLightedLine( );
	}

	if( fDisableDueToBattleRoster == FALSE )
	{
		// render face of current info char, for animation
		DrawFace( bSelectedInfoChar );

		// handle autofaces
		HandleAutoFaces( );
		HandleTalkingAutoFaces( );
/*
		GlowFace( );
		GlowItem( );
*/

	}


	// automatically turns off mapscreen ui overlay messages when appropriate
	MonitorMapUIMessage( );


	// if heli is around, show it
	if( fHelicopterAvailable && fShowAircraftFlag && ( iCurrentMapSectorZ == 0 ) && !fShowMapInventoryPool )
	{
		// this is done on EVERY frame, I guess it beats setting entire map dirty all the time while he's moving...
		DisplayPositionOfHelicopter( );
	}


	// display town info
	DisplayTownInfo( sSelMapX, sSelMapY, ( INT8 ) iCurrentMapSectorZ );

	if( fShowTownInfo == TRUE )
	{
		// force update of town mine info boxes
		ForceUpDateOfBox( ghTownMineBox );
		MapscreenMarkButtonsDirty();
	}

	// update town mine pop up display
	UpdateTownMinePopUpDisplay( );

	if( fShowAttributeMenu )
	{
		// mark all popups as dirty
		MarkAllBoxesAsAltered( );
	}


	// if plotting path
	if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
	{
		// plot out paths
		PlotPermanentPaths( );
		PlotTemporaryPaths( );

		// show ETA
		RenderMapBorderEtaPopUp( );
		DisplayGroundEta();

		//DisplayDestinationOfCurrentDestMerc( );
	}


	HandleContractRenewalSequence( );

	// handle dialog
	HandleDialogue( );


	// now the border corner piece
//	RenderMapBorderCorner( );


	// handle display of inventory pop up
	// HEADROCK HAM 3.5: Externalize!
	HandleDisplayOfItemPopUpForSector( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, startingZ );

	// Display Framerate
	DisplayFrameRate( );

	// update paused states
	UpdatePausedStatesDueToTimeCompression( );

	// is there a description to be displayed?
	// BOB : if we're displaying the attachment popup, don't redraw the IDB
	if (giActiveAttachmentPopup > -1)
		gItemDescAttachmentPopups[giActiveAttachmentPopup]->show();
	else
	RenderItemDescriptionBox( );

	// render clock
	// WANNE: Renders the clock in the strategy screen
	RenderClock(CLOCK_X, CLOCK_Y);

	#ifdef JA2TESTVERSION
	if( !gfWorldLoaded )
	{
		SetFont( FONT10ARIAL );
		if( GetJA2Clock() % 1000 < 500 )
			SetFontForeground( FONT_DKRED );
		else
			SetFontForeground( FONT_RED );
		mprintf( SCREEN_WIDTH - 110, 2, L"TESTVERSION MSG" );
		if( GetJA2Clock() % 1000 < 500 )
			SetFontForeground( FONT_DKYELLOW );
		else
			SetFontForeground( FONT_YELLOW );
		mprintf( SCREEN_WIDTH - 110, 12, L"NO WORLD LOADED" );
		InvalidateRegion( SCREEN_WIDTH - 110, 2, SCREEN_WIDTH, 23 );
	}
	#endif

	if( fEndShowInventoryFlag == TRUE )
	{
		if( InKeyRingPopup( ) == TRUE )
		{
			DeleteKeyRingPopup( );
		}
		else
		{
			fShowInventoryFlag = FALSE;
			// CHRISL: Added call to re-render the bottom panel
			RenderMapScreenInterfaceBottom ( TRUE );
			// set help text for item glow region
			SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );
		}



		fCharacterInfoPanelDirty=TRUE;
		fMapPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fEndShowInventoryFlag = FALSE;
	}

	// handle animated cursor update
	if ( !gfFadeIn )
	{
	HandleAnimatedCursorsForMapScreen( );
	}

	// if inventory is being manipulated, update cursor
	HandleMapInventoryCursor();



	if( fShowDescriptionFlag == TRUE )
	{
		// unmark done button
		if( gpItemDescObject->usItem == MONEY )
		{
			MapscreenMarkButtonsDirty();
		}

		if( Item[ gpItemDescObject->usItem ].usItemClass & IC_GUN )
		{
			MapscreenMarkButtonsDirty();
		}

		UnMarkButtonDirty( giMapInvDoneButton );
		//UnMarkButtonDirty( giCharInfoButton[ 0 ] );
		//UnMarkButtonDirty( giCharInfoButton[ 1 ] );
		MarkAButtonDirty( giMapInvDescButton );
		// HEADROCK HAM 4: Makes sure the Tab Buttons are always redrawn on top.
		for (UINT8 cnt = 0; cnt < 3; cnt++)
		{
			if (giInvDescTabButton[cnt] != -1)
			{
				MarkAButtonDirty( giInvDescTabButton[ cnt ] );
			}
		}
	}
	else
	{
		if( fShowInventoryFlag == TRUE )
		{
			MarkAButtonDirty( giMapInvDoneButton );
			MarkAButtonDirty( giCharInfoButton[ 1 ] );
			MarkAButtonDirty( giCharInfoButton[ 0 ] );
		}
	}

	DrawMilitiaPopUpBox( );

	if( fDisableDueToBattleRoster == FALSE )
	{
		CreateDestroyTheUpdateBox( );
		DisplaySoldierUpdateBox( );
	}


	// pop up display boxes
	DisplayBoxes(FRAME_BUFFER);

	// render buttons
	RenderButtons( );

	if( fShowMapScreenMovementList )
	{
		// redisplay Movement box to blit it over any border buttons, since if long enough it can overlap them
		ForceUpDateOfBox( ghMoveBox );
		DisplayOnePopupBox( ghMoveBox, FRAME_BUFFER );
	}

	if( fShowContractMenu )
	{
		// redisplay Contract box to blit it over any map sort buttons, since they overlap
		ForceUpDateOfBox( ghContractBox );
		DisplayOnePopupBox( ghContractBox, FRAME_BUFFER );
	}


	//If we have new email, blink the email icon on top of the laptop button.
	CheckForAndRenderNewMailOverlay();

	// handle video overlays
	ExecuteVideoOverlays( );

	if ( InItemStackPopup( ) || InSectorStackPopup( ) )
	{
		RenderItemStackPopup( FALSE );
	}

	if( InKeyRingPopup() )
	{
		RenderKeyRingPopup( FALSE );
	}

	CheckForMeanwhileOKStart( );

	// save background rects
	// ATE: DO this BEFORE rendering help text....
	SaveBackgroundRects( );

	if( ( fDisableDueToBattleRoster == FALSE ) && ( fShowAssignmentMenu == FALSE )&&( fShowContractMenu == FALSE ) )
	{
		// highlight lines?
		HandleHighLightingOfLinesInTeamPanel( );

		// render glow for contract region
		ContractBoxGlow( );
		GlowTrashCan( );

		// handle changing of highlighted lines
		HandleChangeOfHighLightedLine( );

		GlowFace( );
		GlowItem( );
	}

	if( fShowMapScreenHelpText )
	{
		// display map screen fast help
		DisplayMapScreenFastHelpList( );
	}
	else
	{
		// render help
		RenderButtonsFastHelp( );
	}

/*
	if( fSecondFrame )
	{
		if( gTacticalStatus.fDidGameJustStart )
		{
			// game just started, check what state player is in......
			HandlePlayerEnteringMapScreenBeforeGoingToTactical( );
		}
	}
	fSecondFrame = FALSE;
*/

	// execute dirty
	ExecuteBaseDirtyRectQueue( );

	// update cursor
	UpdateCursorIfInLastSector( );


	// about to leave for new map
	if ( gfLoadPending == 1 )
	{
		gfLoadPending++;

		// Shade this frame!
		// Remove cursor
		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

		//Shadow area
		ShadowVideoSurfaceRect( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		InvalidateScreen( );
	}

	//InvalidateRegion( 0,0, 640, 480);
	EndFrameBufferRender( );


	// if not going anywhere else
	if ( guiPendingScreen == NO_PENDING_SCREEN )
	{
		if ( HandleFadeInCallback( ) )
		{
			// force mapscreen to be reinitialized even though we're already in it
			EndMapScreen( TRUE );
		}

		if ( HandleBeginFadeIn( MAP_SCREEN ) )
		{
		}
	}

	HandlePreBattleInterfaceStates();

	if( gfHotKeyEnterSector )
	{
		gfHotKeyEnterSector = FALSE;
		ActivatePreBattleEnterSectorAction();
	}

#ifdef JA2BETAVERSION
	DebugValidateSoldierData( );
#endif

	if ( gfRequestGiveSkyriderNewDestination )
	{
		RequestGiveSkyriderNewDestination();
		gfRequestGiveSkyriderNewDestination = FALSE;
	}


	if( gfFirstMapscreenFrame )
	{
//		fSecondFrame = TRUE;
		gfFirstMapscreenFrame = FALSE;
	}
	else
	{
		// handle exiting from mapscreen due to both exit button clicks and keyboard equivalents
		HandleExitsFromMapScreen( );
	}


	return( MAP_SCREEN );
}



void DrawString(STR16 pString, UINT16 uiX, UINT16 uiY, UINT32 uiFont)
{
	// draw monochrome string
	SetFont(uiFont);
	gprintfdirty(uiX,uiY,pString);
	mprintf(uiX,uiY,pString);
}



void SetDayAlternate(STR16 pStringA, ...)
{
 // this sets the clock counter, unwind loop
 UINT16 uiX=0;
 UINT16 uiY=0;
 CHAR16 String[80];
 va_list argptr;

	va_start(argptr, pStringA);						// Set up variable argument pointer
	vswprintf(String, pStringA, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	if (String[1]==0)
	{
		String[1]=String[0];
		String[0]=L' ';
	}
	String[2]=gsTimeStrings[ 3 ][ 0 ];
	String[3]=L' ';
	String[4]=0;

	uiX=CLOCK_HOUR_X_START-9;
	uiY=CLOCK_Y_START;

	SetFont( ETA_FONT );
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	//RestoreExternBackgroundRect( uiX, uiY, 20 ,GetFontHeight( ETA_FONT ) );
	mprintf( uiX, uiY, String );

}


void SetHourAlternate(STR16 pStringA, ...)
{
 // this sets the clock counter, unwind loop
 UINT16 uiX=0;
 UINT16 uiY=0;
 CHAR16 String[80];
 va_list argptr;

	va_start(argptr, pStringA);						// Set up variable argument pointer
	vswprintf(String, pStringA, argptr);	// process gprintf string (get output str)
	va_end(argptr);

	if (String[1]==0)
	{
		String[1]=String[0];
		String[0]=L' ';
	}

	String[2]=gsTimeStrings[ 0 ][ 0 ];
	String[3]=L' ';
	String[4]= 0;
	uiX=CLOCK_MIN_X_START-5;
	uiY=CLOCK_Y_START;
	DrawString(String, uiX, uiY, ETA_FONT);

	SetFont( ETA_FONT );
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	//RestoreExternBackgroundRect( uiX, uiY, 20 ,GetFontHeight( ETA_FONT ) );
	mprintf( uiX, uiY, String );

}



void SetClockHour(STR16 pStringA, ...)
{
 // this sets the clock counter, unwind loop
 UINT16 uiX=0;
 UINT16 uiY=0;
 CHAR16 String[80];
 va_list argptr;

	va_start(argptr, pStringA);						// Set up variable argument pointer
	vswprintf(String, pStringA, argptr);	// process gprintf string (get output str)
	va_end(argptr);
	if (String[1]==0)
	{
		String[1]=String[0];
		String[0]=L' ';
	}
	String[2]=gsTimeStrings[ 0 ][ 0 ];
	String[3]=L' ';
	String[4]=0;
	uiX=CLOCK_HOUR_X_START-8;
	uiY=CLOCK_Y_START;


	SetFont( ETA_FONT );
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	//RestoreExternBackgroundRect( uiX, uiY, 20 ,GetFontHeight( ETA_FONT ) );
	mprintf( uiX, uiY, String );
}

void SetClockMin(STR16 pStringA, ...)
{
 // this sets the clock counter, unwind loop
 CHAR16 String[10];
 va_list argptr;

 va_start(argptr, pStringA);					// Set up variable argument pointer
 vswprintf(String, pStringA, argptr);	// process gprintf string (get output str)
 va_end(argptr);

	if (String[1]==0)
	{
		String[1]=String[0];
		String[0]=L' ';
	}
	String[2]=gsTimeStrings[ 1 ][ 0 ];
	String[3]=L' ';
	String[4]=0;

	SetFont( ETA_FONT );
	SetFontForeground( FONT_LTGREEN );
	SetFontBackground( FONT_BLACK );

	//RestoreExternBackgroundRect( CLOCK_MIN_X_START - 5, CLOCK_Y_START, 20 ,GetFontHeight( ETA_FONT ) );
	mprintf( CLOCK_MIN_X_START-5, CLOCK_Y_START, String );
}

void DrawName(STR16 pName, INT16 sRowIndex, INT32 iFont)
{
	INT16 usX=0;
	INT16 usY=0;

	FindFontCenterCoordinates((short)NAME_X + 1, (short)(Y_START+(sRowIndex*Y_SIZE)), (short)NAME_WIDTH, (short)Y_SIZE, pName, (long)iFont, &usX, &usY);
	
	DrawString( pName, (UINT16)usX, ((UINT16)(usY+(Y_OFFSET*sRowIndex+1))), iFont);
}


void DrawAssignment(INT16 sCharNumber, INT16 sRowIndex, INT32 iFont)
{
	INT16 usX=0;
	INT16 usY=0;
	CHAR16 sString[32];


	GetMapscreenMercAssignmentString( MercPtrs[ gCharactersList[ sCharNumber ].usSolID ], sString );

	FindFontCenterCoordinates((short)ASSIGN_X + 1, (short)(Y_START+(sRowIndex*Y_SIZE)), (short)ASSIGN_WIDTH, (short)Y_SIZE, sString, (long)iFont, &usX, &usY);
	
	SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ sCharNumber ].usSolID ];

	if( fFlashAssignDone == TRUE )
	{
		if( Menptr[gCharactersList[sCharNumber].usSolID].flags.fDoneAssignmentAndNothingToDoFlag )
		{
			SetFontForeground( FONT_RED );
		}
	}

	if ( gGameSettings.fOptions[TOPTION_STAT_PROGRESS_BARS] &&
		pSoldier->bAssignment >= TRAIN_SELF &&
		pSoldier->bAssignment <= TRAIN_BY_OTHER &&
		pSoldier->bAssignment != TRAIN_TEAMMATE )
	{
		// HEADROCK HAM 3.6: Draw militia training progress bar
		UINT32 uiDestBuffer;
		UINT8	*pDestBuf;
		UINT32 uiDestPitchBYTES = 0;
		SGPRect ClipRect;
		UINT8 ubBarWidth;
		UINT16 usColor = Get16BPPColor( FROMRGB( gGameExternalOptions.ubStatProgressBarsRed, gGameExternalOptions.ubStatProgressBarsGreen, gGameExternalOptions.ubStatProgressBarsBlue ) );

		if( ( fShowAssignmentMenu == TRUE ) && ( fTeamPanelDirty == FALSE ) )
		{
			uiDestBuffer = FRAME_BUFFER;
		}
		else
		{
			uiDestBuffer = guiSAVEBUFFER;
		}
		pDestBuf = LockVideoSurface( uiDestBuffer, &uiDestPitchBYTES );

		UINT8 ubProgress = 0;
		UINT16 usMaxProgress = 100;
		UINT8 sMapX = (UINT8)pSoldier->sSectorX;
		UINT8 sMapY = (UINT8)pSoldier->sSectorY;
		if ( pSoldier->bAssignment == TRAIN_TOWN )
		{
			ubProgress = SectorInfo[SECTOR(sMapX, sMapY)].ubMilitiaTrainingPercentDone;
			usMaxProgress = 100;
		}
		else if ( pSoldier->bAssignment == TRAIN_MOBILE )
		{
			ubProgress = SectorInfo[SECTOR(sMapX, sMapY)].ubMobileMilitiaTrainingPercentDone;
			usMaxProgress = 100;
		}
		else if ( pSoldier->bAssignment == TRAIN_SELF ||
			pSoldier->bAssignment == TRAIN_BY_OTHER )
		{
			switch (pSoldier->bTrainStat)
			{
				case STRENGTH:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sStrengthGain+1;
					usMaxProgress = gGameExternalOptions.usStrengthSubpointsToImprove;
					break;
				case AGILITY:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sAgilityGain+1;
					usMaxProgress = gGameExternalOptions.usAgilitySubpointsToImprove;
					break;
				case DEXTERITY:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sDexterityGain+1;
					usMaxProgress = gGameExternalOptions.usDexteritySubpointsToImprove;
					break;
				case HEALTH:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sLifeGain+1;
					usMaxProgress = gGameExternalOptions.usHealthSubpointsToImprove;
					break;
				case MARKSMANSHIP:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sMarksmanshipGain+1;
					usMaxProgress = gGameExternalOptions.usMarksmanshipSubpointsToImprove;
					break;
				case LEADERSHIP:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sLeadershipGain+1;
					usMaxProgress = gGameExternalOptions.usLeadershipSubpointsToImprove;
					break;
				case MECHANICAL:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sMechanicGain+1;
					usMaxProgress = gGameExternalOptions.usMechanicalSubpointsToImprove;
					break;
				case MEDICAL:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sMedicalGain+1;
					usMaxProgress = gGameExternalOptions.usMedicalSubpointsToImprove;
					break;
				case EXPLOSIVE_ASSIGN:
					ubProgress = gMercProfiles[ pSoldier->ubProfile ].sExplosivesGain+1;
					usMaxProgress = gGameExternalOptions.usExplosivesSubpointsToImprove;
					break;
			}
		}

		ubBarWidth = (ASSIGN_WIDTH * ubProgress) / usMaxProgress;
		ubBarWidth = __max(0, __min(ubBarWidth, ASSIGN_WIDTH));
		ClipRect.iTop = (usY+(Y_OFFSET*sRowIndex+1))-1;
		ClipRect.iBottom = ClipRect.iTop + Y_SIZE;
		ClipRect.iLeft = ASSIGN_X+2;
		ClipRect.iRight = ASSIGN_X + ubBarWidth;
		Blt16BPPBufferHatchRectWithColor( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect, usColor );

		UnLockVideoSurface( uiDestBuffer );
	}
	
	//RestoreExternBackgroundRect(ASSIGN_X-2, ((UINT16)(usY+(Y_OFFSET*sRowIndex+1))), ASSIGN_WIDTH+2, Y_SIZE);
	DrawString(sString, (UINT16)usX, ((UINT16)(usY+(Y_OFFSET*sRowIndex+1))), iFont);
}


void DrawLocation(INT16 sCharNumber, INT16 sRowIndex, INT32 iFont)
{
	INT16 usX=0;
	INT16 usY=0;
	CHAR16 sString[32];

	GetMapscreenMercLocationString( MercPtrs[ gCharactersList[ sCharNumber ].usSolID ], sString );

	FindFontCenterCoordinates((short)LOC_X + 1, (short)(Y_START+(sRowIndex*Y_SIZE)), (short)LOC_WIDTH, (short)Y_SIZE, sString, (long)iFont, &usX, &usY);
	
	// draw string
	DrawString( sString,((UINT16)(usX)), ((UINT16)(usY+(Y_OFFSET*sRowIndex+1))), ((UINT32)iFont));
}


void DrawDestination(INT16 sCharNumber, INT16 sRowIndex, INT32 iFont)
{
	INT16 usX=0;
	INT16 usY=0;
	CHAR16 sString[32];

	GetMapscreenMercDestinationString( MercPtrs[ gCharactersList[ sCharNumber ].usSolID ], sString );

	if ( wcslen( sString ) == 0 )
	{
		return;
	}

	FindFontCenterCoordinates((short)DEST_ETA_X + 1, (short)(Y_START+(sRowIndex*Y_SIZE)), (short)DEST_ETA_WIDTH, (short)Y_SIZE, sString, (long)iFont, &usX, &usY);
	
	DrawString( sString,((UINT16)(usX)), ((UINT16)(usY+(Y_OFFSET*sRowIndex+1))), ((UINT32)iFont));
}


void DrawTimeRemaining( INT16 sCharNumber, INT32 iFont, UINT8 ubFontColor )
{
	INT16 usX=0;
	INT16 usY=0;
	CHAR16 sString[32];

// marke strogg more mercs MUST override pointer into array by number of skipped mercs
	GetMapscreenMercDepartureString( MercPtrs[ gCharactersList[ sCharNumber + FIRSTmercTOdisplay ].usSolID ], sString, &ubFontColor );

	// if merc is highlighted, override the color decided above with bright white
	if( sCharNumber == ( INT16 ) giHighLine )
	{
		ubFontColor = FONT_WHITE;
	}

	SetFont(iFont);
	SetFontForeground( ubFontColor );

	FindFontCenterCoordinates((short)TIME_REMAINING_X + 1, (short)(Y_START+(sCharNumber*Y_SIZE)), (short)TIME_REMAINING_WIDTH, (short)Y_SIZE, sString, (long)iFont, &usX, &usY);
	
	DrawString(sString,((UINT16)(usX)), ((UINT16)(usY+(Y_OFFSET*sCharNumber+1))), ((UINT32)iFont));
}



void RenderMapCursorsIndexesAnims( )
{
	BOOLEAN fSelectedSectorHighlighted = FALSE;
	BOOLEAN fSelectedCursorIsYellow = TRUE;
	UINT16 usCursorColor;
	UINT32 uiDeltaTime;
	static INT16 sPrevHighlightedMapX = -1, sPrevHighlightedMapY = -1;
	static INT16 sPrevSelectedMapX = -1, sPrevSelectedMapY = -1;
	static BOOLEAN fFlashCursorIsYellow = FALSE;
	BOOLEAN fDrawCursors;
	BOOLEAN fHighlightChanged = FALSE;


	HandleAnimationOfSectors( );

	if( gfBlitBattleSectorLocator )
	{
		HandleBlitOfSectorLocatorIcon( gubPBSectorX, gubPBSectorY, gubPBSectorZ, LOCATOR_COLOR_RED );
	}

	fDrawCursors = CanDrawSectorCursor( );

	// if mouse cursor is over a map sector
	if ( fDrawCursors && ( GetMouseMapXY( &gsHighlightSectorX, &gsHighlightSectorY ) ) )
	{
		// handle highlighting of sector pointed at ( WHITE )

		// if we're over a different sector than when we previously blitted this
		if ( ( gsHighlightSectorX != sPrevHighlightedMapX ) || ( gsHighlightSectorY != sPrevHighlightedMapY ) || gfMapPanelWasRedrawn )
		{
			if ( sPrevHighlightedMapX != -1 && sPrevHighlightedMapY != -1 )
			{
				RestoreMapSectorCursor( sPrevHighlightedMapX, sPrevHighlightedMapY );
			}

			// draw WHITE highlight rectangle
			RenderMapHighlight( gsHighlightSectorX, gsHighlightSectorY, Get16BPPColor( RGB_WHITE ), FALSE );

			sPrevHighlightedMapX = gsHighlightSectorX;
			sPrevHighlightedMapY = gsHighlightSectorY;

			fHighlightChanged = TRUE;
		}
	}
	else
	{
		// nothing now highlighted
		gsHighlightSectorX = -1;
		gsHighlightSectorY = -1;

		if ( sPrevHighlightedMapX != -1 && sPrevHighlightedMapY != -1 )
		{
			RestoreMapSectorCursor( sPrevHighlightedMapX, sPrevHighlightedMapY );
			fHighlightChanged = TRUE;
		}

		sPrevHighlightedMapX = -1;
		sPrevHighlightedMapY = -1;
	}


	// handle highlighting of selected sector ( YELLOW ) - don't show it while plotting movement
	if ( fDrawCursors && ( bSelectedDestChar == -1 ) && ( fPlotForHelicopter == FALSE ) )
	{
		// if mouse cursor is over the currently selected sector
		if( ( gsHighlightSectorX == sSelMapX ) && ( gsHighlightSectorY == sSelMapY ) )
		{
			fSelectedSectorHighlighted = TRUE;

			// do we need to flash the cursor?	get the delta in time
			uiDeltaTime = GetJA2Clock( ) - guiFlashCursorBaseTime;

			if ( uiDeltaTime > 300 )
			{
				guiFlashCursorBaseTime = GetJA2Clock();
				fFlashCursorIsYellow = !fFlashCursorIsYellow;

				fHighlightChanged = TRUE;
			}
		}

		if ( !fSelectedSectorHighlighted || fFlashCursorIsYellow )
		{
			// draw YELLOW highlight rectangle
			usCursorColor = Get16BPPColor( RGB_YELLOW );
		}
		else
		{
			// draw WHITE highlight rectangle
			usCursorColor = Get16BPPColor( RGB_WHITE );

			// index letters will also be white instead of yellow so that they flash in synch with the cursor
			fSelectedCursorIsYellow = FALSE;
		}

		// always render this one, it's too much of a pain detecting overlaps with the white cursor otherwise
		RenderMapHighlight( sSelMapX, sSelMapY, usCursorColor, TRUE );

		if ( ( sPrevSelectedMapX != sSelMapX ) || ( sPrevSelectedMapY != sSelMapY ) )
		{
			sPrevSelectedMapX = sSelMapX;
			sPrevSelectedMapY = sSelMapY;

			fHighlightChanged = TRUE;
		}
	}
	else
	{
		// erase yellow highlight cursor
		if ( sPrevSelectedMapX != -1 && sPrevSelectedMapY != -1 )
		{
			RestoreMapSectorCursor( sPrevSelectedMapX, sPrevSelectedMapY );
			fHighlightChanged = TRUE;
		}

		sPrevSelectedMapX = -1;
		sPrevSelectedMapY = -1;
	}


	if ( fHighlightChanged || gfMapPanelWasRedrawn )
	{
		// redraw sector index letters and numbers
/*
		if( fZoomFlag )
			DrawMapIndexSmallMap( fSelectedCursorIsYellow );
		else
*/
			DrawMapIndexBigMap( fSelectedCursorIsYellow );
	}
}



UINT32 HandleMapUI( )
{
	UINT32 uiNewEvent = MAP_EVENT_NONE;
	INT16 sMapX = 0, sMapY = 0;
	INT16 sX, sY;
	POINT MousePos;
	UINT32 uiNewScreen = MAP_SCREEN;
	BOOLEAN fWasAlreadySelected;


	// Get Input from keyboard
	GetMapKeyboardInput( &uiNewEvent );


	CreateDestroyMapInvButton();

	// Get mouse
	PollLeftButtonInMapView( &uiNewEvent );
	PollRightButtonInMapView( &uiNewEvent );

	// Switch on event
	switch( uiNewEvent )
	{
		case MAP_EVENT_NONE:
			break;

		case MAP_EVENT_PLOT_PATH:
				GetMouseMapXY(&sMapX, &sMapY);

/*
						// translate screen values to map grid values for zoomed in
						if(fZoomFlag)
						{
								sMapX=(UINT16)iZoomX/MAP_GRID_X+sMapX;
								sMapX=sMapX/2;
								sMapY=(UINT16)iZoomY/MAP_GRID_Y+sMapY;
								sMapY=sMapY/2;
						}
*/


						// plotting for the chopper?
						if( fPlotForHelicopter == TRUE )
						{
/*
							if( IsSectorOutOfTheWay( sMapX, sMapY ) == TRUE )
							{
								if( gfAllowSkyriderTooFarQuote == TRUE )
								{
									SkyRiderTalk( DESTINATION_TOO_FAR );
								}

								return( MAP_SCREEN );
							}
*/

							PlotPathForHelicopter( sMapX, sMapY );
							fTeamPanelDirty = TRUE;
						}
						else
						{
							// plot for character

							// check for valid character
							Assert ( bSelectedDestChar != -1 );
							if ( bSelectedDestChar == -1 )
								break;

							// check if last sector in character's path is same as where mouse is
							if( GetLastSectorIdInCharactersPath( &Menptr[gCharactersList[bSelectedDestChar].usSolID] ) != ( sMapX + ( sMapY * MAP_WORLD_X ) ) )
							{
								sX = ( GetLastSectorIdInCharactersPath( &Menptr[gCharactersList[bSelectedDestChar].usSolID]	) % MAP_WORLD_X );
								sY = ( GetLastSectorIdInCharactersPath( &Menptr[gCharactersList[bSelectedDestChar].usSolID]	) / MAP_WORLD_X );
								GetCursorPos(&MousePos);
								ScreenToClient(ghWindow, &MousePos); // In window coords!
								RestoreBackgroundForMapGrid( sX, sY );
								// fMapPanelDirty = TRUE;
							}

							//SetFontDestBuffer( FRAME_BUFFER, 0, 0, 640, 480, FALSE );

							if( ( IsTheCursorAllowedToHighLightThisSector( sMapX, sMapY ) == TRUE ) &&
									( SectorInfo[ ( SECTOR( sMapX, sMapY ) ) ].ubTraversability[ THROUGH_STRATEGIC_MOVE ] != GROUNDBARRIER ) )
							{
								// Can we get go there?	(NULL temp character path)
								if ( GetLengthOfPath( pTempCharacterPath ) > 0 )
								{
									PlotPathForCharacter( &Menptr[gCharactersList[bSelectedDestChar].usSolID], sMapX, sMapY, FALSE );

									// copy the path to every other selected character
									CopyPathToAllSelectedCharacters( GetSoldierMercPathPtr( MercPtrs[ gCharactersList[ bSelectedDestChar ].usSolID ] ) );

									StartConfirmMapMoveMode( sMapY );
									fMapPanelDirty = TRUE;
									fTeamPanelDirty = TRUE;	// update team panel desinations
								}
								else
								{
									// means it's a vehicle and we've clicked an off-road sector
									MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapErrorString[ 40 ] );
								}
							}
						}
			break;


		case MAP_EVENT_CANCEL_PATH:
			CancelOrShortenPlottedPath( );
			break;


/*
	case MAP_EVENT_SELECT_SECTOR:
			// will select the sector the selected merc is in

			sMapX=Menptr[gCharactersList[bSelectedInfoChar].usSolID].sSectorX;
			sMapY=Menptr[gCharactersList[bSelectedInfoChar].usSolID].sSectorY;
			bMapZ=Menptr[gCharactersList[bSelectedInfoChar].usSolID].bSectorZ;

			if( ( sSelMapX != sMapX || sSelMapY != sMapY || iCurrentMapSectorZ != bMapZ ) &&
					( gTacticalStatus.fDidGameJustStart == FALSE ) && ( gfPreBattleInterfaceActive == FALSE ) )
			{
				ChangeSelectedMapSector( sMapX, sMapY, bMapZ );

				fTeamPanelDirty = TRUE;

				fMapScreenBottomDirty = TRUE;
		bSelectedDestChar=-1;
			}

			break;
*/

		case MAP_EVENT_CLICK_SECTOR:

			// Get Current mouse position
			if ( GetMouseMapXY( &sMapX, &sMapY) )
			{
/*
				if( fZoomFlag == TRUE )
				{
					// convert to zoom out coords from screen coords
			sMapX = ( INT16 )( iZoomX / MAP_GRID_X + sMapX ) / 2;
			sMapY = ( INT16 )( iZoomY / MAP_GRID_Y + sMapY ) / 2;
					//sMapX = ( INT16 ) ( ( ( iZoomX ) / ( MAP_GRID_X * 2) ) + sMapX / 2 );
					//sMapX = ( INT16 ) ( ( ( iZoomY ) / ( MAP_GRID_Y * 2) ) + sMapY / 2 );
				}
*/

				// not zoomed out, make sure this is a valid sector
		if( IsTheCursorAllowedToHighLightThisSector( sMapX, sMapY ) == FALSE )
				{
			// do nothing, return
					return( MAP_SCREEN );
				}


				// while item in hand
				if ( fMapInventoryItem )
				{
					// if not showing item counts on the map
					if ( !fShowItemsFlag )
					{
						// turn that on
						ToggleItemsFilter( );
					}

					// if item's owner is known
					if ( gpItemPointerSoldier != NULL )
					{
						// make sure it's the owner's sector that's selected
						if ( ( gpItemPointerSoldier->sSectorX != sSelMapX ) ||
								( gpItemPointerSoldier->sSectorY != sSelMapY ) ||
								( gpItemPointerSoldier->bSectorZ != iCurrentMapSectorZ ) )
						{
							ChangeSelectedMapSector( gpItemPointerSoldier->sSectorX, gpItemPointerSoldier->sSectorY, gpItemPointerSoldier->bSectorZ );
						}
					}

					// if not already in sector inventory
					if ( !fShowMapInventoryPool )
					{
						// start it up ( NOTE: for the item OWNER'S sector, regardless of which sector player clicks )
						fShowMapInventoryPool = TRUE;
						CreateDestroyMapInventoryPoolButtons( TRUE );
					}

					return( MAP_SCREEN );
				}


				// don't permit other click handling while item is in cursor (entering PBI would permit item teleports, etc.)
				Assert( !fMapInventoryItem );


				// this doesn't change selected sector
				if ( gfInChangeArrivalSectorMode )
				{
					if( SectorInfo[ ( SECTOR( sMapX, sMapY ) ) ].ubTraversability[ THROUGH_STRATEGIC_MOVE ] != GROUNDBARRIER )
					{
						// if it's not enemy air controlled
						if ( StrategicMap[ CALCULATE_STRATEGIC_INDEX( sMapX, sMapY ) ].fEnemyAirControlled == FALSE )
						{
							CHAR16 sMsgString[ 128 ], sMsgSubString[ 64 ];

							// move the landing zone over here
							gsMercArriveSectorX = sMapX;
							gsMercArriveSectorY = sMapY;

							// change arrival sector for all mercs currently in transit who are showing up at the landing zone
							UpdateAnyInTransitMercsWithGlobalArrivalSector();

							// we're done, cancel this mode
							CancelChangeArrivalSectorMode();

							// get the name of the sector
							GetSectorIDString( sMapX, sMapY, 0, sMsgSubString, FALSE );

							// now build the string
							swprintf( sMsgString, pBullseyeStrings[ 1 ], sMsgSubString );

							// confirm the change with overlay message
							MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, sMsgString );

							// update destination column for any mercs in transit
							fTeamPanelDirty = TRUE;

							// added by haydent
							if (is_networked)
							{
								ChangeSelectedMapSector( sMapX, sMapY, ( INT8 )iCurrentMapSectorZ );
								// <TODO> OJW - 20081201 - change this state away from allowlaptop to its own field
								// or something more seperate from that concept
								if (is_server && !allowlaptop)
								{
									InitializeWorldSize(sMapX, sMapY, 0);
									send_mapchange();
								}
							}
						}
						else
						{
							// message: not allowed, don't have airspace secured
							MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pBullseyeStrings[ 2 ] );
						}
					}

					return( MAP_SCREEN );
				}
				else	// not already changing arrival sector
				{
					if ( CanMoveBullseyeAndClickedOnIt( sMapX, sMapY ) )
					{
						// if the click is ALSO over the helicopter icon
						// NOTE: The helicopter icon is NOT necessarily directly over the helicopter's current sector!!!
						if( CheckForClickOverHelicopterIcon( sMapX, sMapY ) == TRUE )
						{
							CreateBullsEyeOrChopperSelectionPopup();
						}
						else
						{
							StartChangeSectorArrivalMode();
						}

						return( MAP_SCREEN );
					}
				}


				// if new map sector was clicked on
				if( ( sSelMapX != sMapX ) || ( sSelMapY != sMapY ) )
				{
					fWasAlreadySelected = FALSE;

					// select the clicked sector, retaining the same sublevel depth
					ChangeSelectedMapSector( sMapX, sMapY, ( INT8 )iCurrentMapSectorZ );
				}
				else
				{
					fWasAlreadySelected = TRUE;
				}


				// if showing item counts on the map, and not already in sector inventory
				if ( fShowItemsFlag && !fShowMapInventoryPool )
				{
					// show sector inventory for this clicked sector
					ChangeSelectedMapSector( sMapX, sMapY, ( INT8 ) iCurrentMapSectorZ );

					fShowMapInventoryPool = TRUE;
					CreateDestroyMapInventoryPoolButtons( TRUE );

					return( MAP_SCREEN );
				}


				if( gfBlitBattleSectorLocator &&
						sMapX == gubPBSectorX && sMapY == gubPBSectorY && iCurrentMapSectorZ == gubPBSectorZ )
				{ //Bring up a non-persistant version of mapscreen if the user clicks on the sector where a
					//battle is taking place.
					InitPreBattleInterface( NULL, FALSE );
					return( MAP_SCREEN );
				}


				// if we're in airspace mode
				if( fShowAircraftFlag == TRUE )
				{
					// if not moving soldiers, and not yet plotting the helicopter
					if ( ( bSelectedDestChar == -1 ) && ( fPlotForHelicopter == FALSE ) )
					{
						// if we're on the surface level, and the click is over the helicopter icon
						// NOTE: The helicopter icon is NOT necessarily directly over the helicopter's current sector!!!
						if( ( iCurrentMapSectorZ == 0 ) && CheckForClickOverHelicopterIcon( sMapX, sMapY ) == TRUE )
						{
							RequestGiveSkyriderNewDestination( );
							return( MAP_SCREEN );
						}
					}
				}
				else	// not in airspace mode
				{
					// sector must be already selected to initiate movement plotting!	This is to allow selecting sectors with
					// mercs in them without necessarily initiating movement right away.
					if( fWasAlreadySelected )
					{
						// if there are any movable characters here
						if ( AnyMovableCharsInOrBetweenThisSector( sMapX, sMapY, ( INT8 )iCurrentMapSectorZ ) )
						{
							// if showing the surface level map
							if( iCurrentMapSectorZ == 0 )
							{
								TurnOnShowTeamsMode( );

								// NOTE: must allow move box to come up, since there may be movable characters between sectors which are
								// unaffected by combat / hostiles / air raid in the sector proper itself!!
								// This also allows all strategic movement error handling to be centralized in CanCharacterMoveInStrategic()

								// start the move box menu
								SetUpMovingListsForSector( sMapX, sMapY, ( INT16 )iCurrentMapSectorZ );
							}
							else
							{
								// no strategic movement is possible from underground sectors
								DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapErrorString[ 1 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
								return( MAP_SCREEN );
							}
						}
					}
				}
			}
			break;

//Kris -- added hook so I can access AIView in non-release mode.
#ifdef JA2BETAVERSION
		case MAP_EVENT_VIEWAI:
			SetPendingNewScreen( AIVIEWER_SCREEN );
			CreateDestroyMapInvButton();
			break;
#endif
	}


	// if we pressed something that will cause a screen change
	if ( guiPendingScreen != NO_PENDING_SCREEN )
	{
		uiNewScreen = guiPendingScreen;
	}

	return( uiNewScreen );
}


// WANNE - BMP: This method is used only for multiplayer
void InitializeWorldSize(INT16 sSectorX, INT16 sSectorY , INT8 bSectorZ)
{
	CHAR8 datFilename[ 50 ];
	CHAR8 mapFilename[ 50 ];
	UINT32					uiFileSize;
	INT8					*pBuffer;
	INT8					*pBufferHead;
	UINT32					uiBytesRead;
	UINT8					ubMinorMapVersion;
	FLOAT					dMajorMapVersion;
	HWFILE					hfile;

	GetMapFileName( sSectorX, sSectorY, bSectorZ,  datFilename, TRUE, TRUE );

	//Load the placeholder map if the real map doesn't exist.
	if( !MapExists((UINT8 *) datFilename ) )
	{
		AssertMsg( 0, "Failed to load map file." );
	}

	sprintf( mapFilename, "MAPS\\%s", datFilename );

	// Open file
	hfile = FileOpen( mapFilename, FILE_ACCESS_READ, FALSE );

	if ( !hfile )
	{
		// Initialize with default "normal" map size
		SetWorldSize(OLD_WORLD_ROWS, OLD_WORLD_COLS);
	}
	else
	{
		//Get the file size and alloc one huge buffer for it.
		//We will use this buffer to transfer all of the data from.
		uiFileSize = FileGetSize( hfile );
		pBuffer = (INT8*)MemAlloc( uiFileSize );
		pBufferHead = pBuffer;
		FileRead( hfile, pBuffer, uiFileSize, &uiBytesRead );
		FileClose( hfile );

		// Read JA2 Version ID
		LOADDATA( &dMajorMapVersion, pBuffer, sizeof( FLOAT ) );
		LOADDATA( &ubMinorMapVersion, pBuffer, sizeof( UINT8 ) );
		
		if(dMajorMapVersion < 7.00)
			// old map - always 160x160
			SetWorldSize(OLD_WORLD_ROWS, OLD_WORLD_COLS);
		else
		{
			INT32 iRowSize = 0, iColSize = 0;
			LOADDATA( &iRowSize, pBuffer, sizeof( INT32 ) );
			LOADDATA( &iColSize, pBuffer, sizeof( INT32 ) );
			SetWorldSize(iRowSize, iColSize);
		}
	}
}


void GetMapKeyboardInput( UINT32 *puiNewEvent )
{
	InputAtom					InputEvent;
	POINT	MousePos;
	INT8 bSquadNumber;
	UINT8 ubGroupId = 0;
	BOOLEAN fCtrl, fAlt;

	INT16 sMapX, sMapY;

	fCtrl = _KeyDown( CTRL );
	fAlt = _KeyDown( ALT );

	while( DequeueEvent( &InputEvent ) )
//		while( DequeueSpecificEvent( &InputEvent, KEY_DOWN ) )		// doesn't work for some reason
	{
		GetCursorPos(&MousePos);
		ScreenToClient(ghWindow, &MousePos); // In window coords!

		// HOOK INTO MOUSE HOOKS
		switch(InputEvent.usEvent)
	{
			case LEFT_BUTTON_DOWN:
				MouseSystemHook(LEFT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_UP:
				MouseSystemHook(LEFT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y ,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_DOWN:
				MouseSystemHook(RIGHT_BUTTON_DOWN, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_UP:
				MouseSystemHook(RIGHT_BUTTON_UP, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case RIGHT_BUTTON_REPEAT:
				MouseSystemHook(RIGHT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
			case LEFT_BUTTON_REPEAT:
				MouseSystemHook(LEFT_BUTTON_REPEAT, (INT16)MousePos.x, (INT16)MousePos.y,_LeftButtonDown, _RightButtonDown);
				break;
	}


		if( InputEvent.usEvent == KEY_DOWN )
		{
			// if game is paused because of player, unpause with any key
			if ( gfPauseDueToPlayerGamePause )
			{
				HandlePlayerPauseUnPauseOfGame( );
				continue;
			}

			if( IsMapScreenHelpTextUp() )
			{
				// stop mapscreen text
				StopMapScreenHelpText( );
				continue;
			}

			// handle for fast help text for interface stuff
			if( IsTheInterfaceFastHelpTextActive() )
			{
				ShutDownUserDefineHelpTextRegions( );
			}


			switch( InputEvent.usParam )
			{
				case ESC:
					gfDontStartTransitionFromLaptop = TRUE;


					if( gfPreBattleInterfaceActive && !gfPersistantPBI )
					{ //Non persistant PBI.	Allow ESC to close it and return to mapscreen.
						KillPreBattleInterface();
						gpBattleGroup = NULL;
						return;
					}

					if ( gfInChangeArrivalSectorMode )
					{
						CancelChangeArrivalSectorMode( );
						MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pBullseyeStrings[ 3 ] );
					}
					// ESC cancels MAP UI messages, unless we're in confirm map move mode
					else if( ( giUIMessageOverlay != -1 ) && !gfInConfirmMapMoveMode )
					{
						CancelMapUIMessage( );
					}
					else if( IsMapScreenHelpTextUp( ) )
					{
						StopMapScreenHelpText( );
					}
					else if ( gpCurrentTalkingFace != NULL && gpCurrentTalkingFace->fTalking )
					{
						// ATE: We want to stop speech if somebody is talking...
						StopAnyCurrentlyTalkingSpeech( );
						}
					else if( fShowUpdateBox )
					{
						if( fShowUpdateBox )
						{
							EndUpdateBox( FALSE );	// stop time compression
						}
					}
					else if(fShowDescriptionFlag)
					{
						DeleteItemDescriptionBox( );
					}
					// plotting movement?
					else if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
					{
						AbortMovementPlottingMode( );
					}
					else if( fShowAssignmentMenu )
					{
						// dirty region
						fTeamPanelDirty = TRUE;
						fMapPanelDirty = TRUE;
						fCharacterInfoPanelDirty = TRUE;

						// stop showing current assignment box
						if( fShowAttributeMenu == TRUE )
						{
							fShowAttributeMenu = FALSE;
							fMapPanelDirty = TRUE;
						}
						// stop showing current assignment box
						if( fShowFacilityAssignmentMenu == TRUE )
						{
							fShowFacilityAssignmentMenu = FALSE;
							fMapPanelDirty = TRUE;
						}
						else if( fShowTrainingMenu == TRUE )
						{
							fShowTrainingMenu = FALSE;
						}
						else if( fShowSquadMenu == TRUE )
						{
							fShowSquadMenu = FALSE;
						}
						else if( fShowRepairMenu == TRUE )
						{
							fShowRepairMenu = FALSE;
						}
						// HEADROCK HAM 3.6: Facility Menu
						else if( fShowFacilityMenu == TRUE )
						{
							fShowFacilityMenu = FALSE;
						}
						else
						{
							fShowAssignmentMenu = FALSE;
						}
						giAssignHighLine = -1;
						// restore background to glow region
						RestoreBackgroundForAssignmentGlowRegionList( );
					}
					else if( fShowContractMenu == TRUE )
					{
						fShowContractMenu = FALSE;

						// restore contract glow region
						RestoreBackgroundForContractGlowRegionList( );
						fTeamPanelDirty = TRUE;
						fCharacterInfoPanelDirty = TRUE;
						giContractHighLine = -1;
					}
					// in militia popup?
					else if( ( sSelectedMilitiaTown != 0 ) && ( sGreensOnCursor == 0 ) && ( sRegularsOnCursor == 0 ) && ( sElitesOnCursor == 0 ) )
					{
						sSelectedMilitiaTown = 0;
						fMapPanelDirty = TRUE;
					}
					else if( fShowTownInfo == TRUE )
					{
						fShowTownInfo = FALSE;
						CreateDestroyScreenMaskForAssignmentAndContractMenus( );
					}
					else if(	fShowDescriptionFlag )
					{
						if( gMPanelRegion.Cursor != EXTERN_CURSOR )
						{
							DeleteItemDescriptionBox( );
						}
					}
					else if( InKeyRingPopup( ) == TRUE )
					{
						DeleteKeyRingPopup( );
						fTeamPanelDirty = TRUE;
					}
					else if( fShowMapInventoryPool == TRUE )
					{
						// no item on cursor & not in either stack popup
						if( gMPanelRegion.Cursor != EXTERN_CURSOR && !InSectorStackPopup() && !InItemStackPopup() )
						{
							fShowMapInventoryPool = FALSE;
						}
					}
					else if( fShowInventoryFlag == TRUE )
					{
						if ( gMPanelRegion.Cursor != EXTERN_CURSOR && !InItemStackPopup() )
						{
							fEndShowInventoryFlag = TRUE;
						}
					}
					else if( MultipleCharacterListEntriesSelected( ) )
					{
						ResetSelectedListForMapScreen( );
						if( bSelectedInfoChar != -1 )
						{
							SetEntryInSelectedCharacterList( bSelectedInfoChar );
						}
						fTeamPanelDirty = TRUE;
						fCharacterInfoPanelDirty = TRUE;
					}
					else
					{
						RequestTriggerExitFromMapscreen( MAP_EXIT_TO_TACTICAL );
					}
					break; // end of ESC


				case PAUSE:
					// Pause game!
					HandlePlayerPauseUnPauseOfGame( );
					break;

				case LEFTARROW:
					// previous character
					fResetMapCoords = TRUE;
					GoToPrevCharacterInList( );
					break;
				case RIGHTARROW:
					// next character
					fResetMapCoords = TRUE;
					GoToNextCharacterInList( );
					break;

				case UPARROW:
					// up a line
					MapScreenMsgScrollUp( 1 );
					break;
				case DNARROW:
					// down a line
					MapScreenMsgScrollDown( 1 );
					break;

				case PGUP:
					// WANNE: Jump to first merc in list
					fResetMapCoords = TRUE;
					GoToFirstCharacterInList( );
					break;
				case PGDN:
					// WANNE: Jump to last merc in list
					fResetMapCoords = TRUE;
					GoToLastCharacterInList( );
					break;

				case SHIFT_PGUP:
					// up a page
					MapScreenMsgScrollUp( MAX_MESSAGES_ON_MAP_BOTTOM );
					break;

				case SHIFT_PGDN:
					// down a page
					MapScreenMsgScrollDown( MAX_MESSAGES_ON_MAP_BOTTOM );
					break;

				

				case HOME:
					// jump to top of message list
					ChangeCurrentMapscreenMessageIndex( 0 );
					break;

				case END:
					// jump to bottom of message list
					MoveToEndOfMapScreenMessageList();
					break;

				case INSERT:
					// up one sublevel
					GoUpOneLevelInMap( );
					break;

				case DEL:
					// show the inventory pool?
					if( fShowMapInventoryPool && _KeyDown( CTRL ) )
					{
						DeleteAllItemsInInventoryPool();
						break;
					}

					// down one sublevel
					GoDownOneLevelInMap( );
					break;

				case ENTER:
					RequestToggleMercInventoryPanel();
					break;

				case BACKSPACE:
					StopAnyCurrentlyTalkingSpeech( );
					break;

				case F1:
				case F2:
				case F3:
				case F4:
				case F5:
				case F6:
					ChangeCharacterListSortMethod( InputEvent.usParam - F1 );
					break;

				case F7:
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							if ( bSelectedInfoChar != -1 )
							{
								SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
								if ( pSoldier->inv[ HANDPOS ].exists() == true )
								{
									pSoldier->inv[ HANDPOS ][0]->data.objectStatus = 2;
								}
							}
						}
						if( fCtrl )
						{
							if ( bSelectedInfoChar != -1 )
							{
								SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
								if ( pSoldier->inv[ HANDPOS ].exists() == true )
								{
									pSoldier->inv[ HANDPOS ].usItem = GUN_BARREL_EXTENDER;
								}
							}
						}
					#endif
					break;

				case F8:
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							// reduce balance to $500
							AddTransactionToPlayersBook( PAYMENT_TO_NPC, SKYRIDER, GetWorldTotalMin( ), - ( LaptopSaveInfo.iCurrentBalance - 500 ) );
						}
					#endif
					break;

				case F9:
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							UINT8 ubSamIndex;

							// ALT-F9: Reveal all SAM sites
							for( ubSamIndex = 0; ubSamIndex < NUMBER_OF_SAMS; ubSamIndex++ )
							{
								SetSAMSiteAsFound( ubSamIndex );
							}
						}
					#endif
					break;

				case F10:
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							if ( bSelectedInfoChar != -1 )
							{
								// ALT-F10: force selected character asleep (ignores breathmax)
								PutMercInAsleepState( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ] );
							}
						}
					#endif
					break;

/*
				case F11:
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							// ALT-F11: make all sectors player controlled
							ClearMapControlledFlags( );
							fMapPanelDirty = TRUE;
						}
					#endif
					break;
*/

				case F12:
					#ifdef JA2BETAVERSION
						*puiNewEvent = MAP_EVENT_VIEWAI;
					#endif
					break;

				case '+':
				case '=':
					if ( CommonTimeCompressionChecks() == FALSE )
						RequestIncreaseInTimeCompression();
					break;

				case '-':
				case '_':
					if ( CommonTimeCompressionChecks() == FALSE )
						RequestDecreaseInTimeCompression();
					break;

				case SPACE:
						if( fShowUpdateBox )
						{
							EndUpdateBox( TRUE );	// restart time compression
						}
						else
						{
							// toggle time compression
							if ( CommonTimeCompressionChecks() == FALSE )
								RequestToggleTimeCompression();
						}
					break;

				case '`':
					#ifdef JA2TESTVERSION
						if( fCtrl )
						{
							if( bSelectedInfoChar != -1 )
							{
								TownMilitiaTrainingCompleted( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ], sSelMapX, sSelMapY );
							}
						}
					#endif
						if (fAlt)
						{
							
							break;
						}
					break;

				case '\\':
					#ifdef JA2TESTVERSION
						if (fAlt)
						{
						}
						else if( fCtrl )
						{
							DumpItemsList();
						}
					#endif
					break;

				case '>':
					#ifdef JA2TESTVERSION
						if( fCtrl )
						{
							// free
						}
					#endif
					break;

				case '?':
					#ifdef JA2TESTVERSION
						if( fCtrl )
						MapScreenMessage( 0, MSG_DEBUG, L"JA2Clock = %d", GetJA2Clock() );
						else
						MapScreenMessage( 0, MSG_DEBUG, L"Mouse X,Y = %d,%d", MSYS_CurrentMX, MSYS_CurrentMY );
					#endif
					break;

				case ',': 
					if( fShowMapInventoryPool == TRUE )
					{
						// if can go to previous page, go there
						if( iCurrentInventoryPoolPage > 0 )
						{
							iCurrentInventoryPoolPage--;
							fMapPanelDirty = TRUE;
						}
					}
					break;

				case '.': 
					if( fShowMapInventoryPool == TRUE )
					{
						// if can go to next page, go there
						if( iCurrentInventoryPoolPage < ( iLastInventoryPoolPage ) )
						{
							iCurrentInventoryPoolPage++;
							fMapPanelDirty = TRUE;
						}
					}
					break;

				case '/':
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							if( bSelectedInfoChar != -1 )
							{
								StatChange( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ], EXPERAMT, 1000, FROM_SUCCESS );
							}
						}
					#endif
					break;


					// ROMAN:0 multiplayer: Maybe we need to change the keys, because they are bound to other functions
					// OJW - 20090210 - disabling the old method's for starting server and disconnecting
					// as they are no longer needed
				case '1':
					{
						/*if (is_networked)
						{
							// haydent
							start_server();
							break;
						}*/
					}
				case '2':
					{
						/*if (is_networked)
						{
							// haydent
							connect_client();
							break;
						}*/
					}
				case '3':
					{
						/*if (is_networked)
						{
							// haydent
							start_battle();
							break;
						}*/
					}
				case '4':
					{
						/*if (is_networked)
						{
							// haydent
							server_disconnect();
							client_disconnect();
							break;
						}*/
					}
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if(gGameExternalOptions.fEnableInventoryPoolQ && fShowMapInventoryPool == TRUE)//dnl ch51 081009
					{
						SwitchToInventoryPoolQ((UINT8)InputEvent.usParam);
						break;
					}
					// multi-selects all characters in that squad.	SHIFT key and 1-0 for squads 11-20
					bSquadNumber = ( INT8 ) ( InputEvent.usParam - '1' );	// internal squad #s start at 0
					SelectAllCharactersInSquad( bSquadNumber );
					break;

				case '0':
					if(gGameExternalOptions.fEnableInventoryPoolQ && fShowMapInventoryPool == TRUE)//dnl ch51 081009
					{
						SwitchToInventoryPoolQ('0');
						break;
					}
					SelectAllCharactersInSquad( 9 ); // internal squad #s start at 0					
					break;

				case '!':
					SelectAllCharactersInSquad( 10 ); // internal squad #s start at 0
					break;
				case '@':
					SelectAllCharactersInSquad( 11 ); // internal squad #s start at 0
					break;
				case '#':
					SelectAllCharactersInSquad( 12 ); // internal squad #s start at 0
					break;
				case '$':
					SelectAllCharactersInSquad( 13 ); // internal squad #s start at 0
					break;
				case '%':
					SelectAllCharactersInSquad( 14 ); // internal squad #s start at 0
					break;
				case '^':
					SelectAllCharactersInSquad( 15 ); // internal squad #s start at 0
					break;
				case '&':
					SelectAllCharactersInSquad( 16 ); // internal squad #s start at 0
					break;
				case '*':
					SelectAllCharactersInSquad( 17 ); // internal squad #s start at 0
					break;
				case '(':
					SelectAllCharactersInSquad( 18 ); // internal squad #s start at 0
					break;
				case ')':
					SelectAllCharactersInSquad( 19 ); // internal squad #s start at 0
					break;


				case 'a':
						if( fAlt )
						{
							if( giHighLine != -1 )
							{
								if( gCharactersList[ giHighLine ].fValid == TRUE )
								{
									bSelectedAssignChar = ( INT8 )giHighLine;
									RebuildAssignmentsBox( );
									ChangeSelectedInfoChar( ( INT8 ) giHighLine, FALSE );
									fShowAssignmentMenu = TRUE;
								}
							}
							else if( bSelectedInfoChar != -1 )
							{
								if( gCharactersList[ bSelectedInfoChar ].fValid == TRUE )
								{
									bSelectedAssignChar = ( INT8 )bSelectedInfoChar;
									RebuildAssignmentsBox( );
									fShowAssignmentMenu = TRUE;
								}
							}
						}
						else if( fCtrl )
						{
							if( CHEATER_CHEAT_LEVEL( ) )
							{
								if(	gfAutoAmbush ^= 1 )
									ScreenMsg( FONT_WHITE, MSG_TESTVERSION, L"Enemy ambush test mode enabled." );
								else
									ScreenMsg( FONT_WHITE, MSG_TESTVERSION, L"Enemy ambush test mode disabled." );
							}
						}
						else
						{
							if( gfPreBattleInterfaceActive )
							{
								//activate autoresolve in prebattle interface.
								ActivatePreBattleAutoresolveAction();
							}
							else
							{
								// only handle border button keyboard equivalents if the button is visible!
								if ( !fShowMapInventoryPool )
								{
									ToggleAirspaceMode();
								}
							}
						}
					break;


				case 'b':
/*
						// CTRL-B: make player's perception of all sectors correct!
						if( ( fCtrl )&&( CHEATER_CHEAT_LEVEL( ) ) )
						{
							for ( sMapX = 1; sMapX <= 16; sMapX++ )
							{
								for ( sMapY = 1; sMapY <= 16; sMapY++ )
								{
									MakePlayerPerceptionOfSectorControlCorrect( sMapX, sMapY, 0 );
								}
							}
						}
*/
					break;

				case 'c':
					if(gGameExternalOptions.fEnableInventoryPoolQ && fShowMapInventoryPool == TRUE)//dnl ch51 081009
					{
						CopySectorInventoryToInventoryPoolQ(0);
						break;
					}
					RequestContractMenu();
					break;

				case 'd':
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							// prints out a text file in C:\TEMP telling you how many stat change chances/successes each profile merc got
							TestDumpStatChanges();
						}
					#endif
					break;

				case 'D':
					if( fCtrl )
					{
						if ( !InSectorStackPopup( ) && !InItemStackPopup( ) && !InItemDescriptionBox( ) && !InKeyRingPopup( ) )
						{
							//CHRISL: Delete all items
							if(!fShowMapInventoryPool)
							{
								fShowMapInventoryPool = TRUE;
								CreateDestroyMapInventoryPoolButtons( TRUE );
							}
							DoMessageBox( MSG_BOX_BASIC_STYLE, NewInvMessage[NIV_DELETE_ALL], guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, BeginDeleteAllCallBack, NULL );
							if(fShowMapInventoryPool)
							{
								fShowMapInventoryPool = FALSE;
							}
						}
					}
					break;
				case 'e':
					if( gfPreBattleInterfaceActive )
					{ //activate enter sector in prebattle interface.
						gfHotKeyEnterSector = TRUE;
					}
					break;
				case 'E':
					if(bSelectedInfoChar != -1)
					{
						SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
						//CHRISL: Try to update InSector value so we don't have to "activate" a sector
						if(pSoldier->sSectorX == sSelMapX && pSoldier->sSectorY == sSelMapY && pSoldier->bSectorZ == iCurrentMapSectorZ && !pSoldier->flags.fBetweenSectors)
							pSoldier->bInSector=TRUE;
						if(OK_CONTROL_MERC( pSoldier ))
						{
							if(!fShowMapInventoryPool)
							{
								fShowMapInventoryPool = TRUE;
								CreateDestroyMapInventoryPoolButtons( TRUE );
							}
							if(!fShowInventoryFlag)
							{
								fShowInventoryFlag = TRUE;
							}
							if( fCtrl )
							{
								//CHRISL: pickup all items to vehicle
								if ( UsingNewInventorySystem() == true && fShowInventoryFlag && fShowMapInventoryPool && !(gTacticalStatus.fEnemyInSector) && gGameExternalOptions.fVehicleInventory && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
								{
									for(unsigned int i = 0; i<pInventoryPoolList.size(); i++)
									{
										if(pInventoryPoolList[i].fExists == TRUE && (pInventoryPoolList[i].usFlags & WORLD_ITEM_REACHABLE))
										{
											for(int x = 0; x<NUM_INV_SLOTS; x++)
											{
												if(vehicleInv[x] == FALSE)
													continue;
												if(pSoldier->inv[x].exists() == true)
												{
													if(pSoldier->inv[x].usItem != pInventoryPoolList[i].object.usItem)
														continue;
													else
														pInventoryPoolList[i].object.AddObjectsToStack(pSoldier->inv[x], -1, pSoldier, x);
												}
												else
													pInventoryPoolList[i].object.MoveThisObjectTo(pSoldier->inv[x], -1, pSoldier, x);
												if(pInventoryPoolList[i].object.ubNumberOfObjects < 0)
												{
													//RemoveItemFromWorld(i);
													break;
												}
											}
										}
										fTeamPanelDirty = TRUE;
										fMapPanelDirty = TRUE;
										fInterfacePanelDirty = DIRTYLEVEL2;
									}
								}
							}
							else if ( fAlt )
							{
								//CHRISL: drop all items in backpack
								if ( bSelectedInfoChar != -1 && fShowInventoryFlag && fShowMapInventoryPool && !(gTacticalStatus.fEnemyInSector) )
								{
									for(int i = BIGPOCK4POS; i<=BIGPOCK7POS; i++)
									{
										if(pSoldier->inv[i].exists() == true)
										{
											AutoPlaceObjectInInventoryStash(&pSoldier->inv[i], pSoldier->sGridNo);
											DeleteObj(&pSoldier->inv[i]);
										}
										fTeamPanelDirty = TRUE;
										fMapPanelDirty = TRUE;
										fInterfacePanelDirty = DIRTYLEVEL2;
									}
									for(int i = SMALLPOCK23POS; i<NUM_INV_SLOTS; i++)
									{
										if(pSoldier->inv[i].exists() == true)
										{
											AutoPlaceObjectInInventoryStash(&pSoldier->inv[i], pSoldier->sGridNo);
											DeleteObj(&pSoldier->inv[i]);
										}
										fTeamPanelDirty = TRUE;
										fMapPanelDirty = TRUE;
										fInterfacePanelDirty = DIRTYLEVEL2;
									}
								}
							}
							else
							{
								//CHRISL: drop all items
								if ( bSelectedInfoChar != -1 && fShowInventoryFlag && fShowMapInventoryPool && !(gTacticalStatus.fEnemyInSector) )
								{
									for(int i = BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
									{
										if(pSoldier->inv[i].exists() == true)
										{
											AutoPlaceObjectInInventoryStash(&pSoldier->inv[i], pSoldier->sGridNo);
											DeleteObj(&pSoldier->inv[i]);
										}
										fTeamPanelDirty = TRUE;
										fMapPanelDirty = TRUE;
										fInterfacePanelDirty = DIRTYLEVEL2;
									}
								}
							}
						}
					}
					break;
				case 'f':
					#ifdef JA2TESTVERSION
					// CTRL-F: Refuel vehicle
					if( ( fCtrl ) && ( bSelectedInfoChar != -1 ) )
					{
						SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];

						if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
						{
							pSoldier->sBreathRed = 10000;
							pSoldier->bBreath = 100;
							ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"Vehicle refueled" );

							fTeamPanelDirty = TRUE;
							fCharacterInfoPanelDirty = TRUE;
						}
					}
					#endif
					if( fAlt )
					{
						if ( INFORMATION_CHEAT_LEVEL( ) )
						{
						//Toggle Frame Rate Display
						gbFPSDisplay = !gbFPSDisplay;
						DisableFPSOverlay( (BOOLEAN)!gbFPSDisplay );
						}
					}
					break;
				case 'h':
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							// set up the helicopter over Omerta (if it's not already set up)
							SetUpHelicopterForPlayer( 9,1 , SKYRIDER );
							// raise Drassen loyalty to minimum that will allow Skyrider to fly
							if ( gTownLoyalty[ DRASSEN ].fStarted && ( gTownLoyalty[ DRASSEN ].ubRating < LOYALTY_LOW_THRESHOLD ) )
							{
								SetTownLoyalty( DRASSEN, LOYALTY_LOW_THRESHOLD );
							}
							TurnOnAirSpaceMode( );
						}
						else
					#endif
						{
							// ARM: Feb01/98 - Cancel out of mapscreen movement plotting if Help subscreen is coming up
							if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
							{
								AbortMovementPlottingMode( );
							}

							ShouldTheHelpScreenComeUp( HELP_SCREEN_MAPSCREEN, TRUE );
						}

//					fShowMapScreenHelpText = TRUE;
					break;

				case 'i':
					#ifdef JA2TESTVERSION
/*
						if( fAlt )
						{
							InitializeMines();
							fMapPanelDirty = TRUE;
						}
						else
*/
						if( fCtrl )
						{
							fDisableJustForIan = ! fDisableJustForIan;
						}
						else
						{
							// only handle border button keyboard equivalents if the button is visible!
							if ( !fShowMapInventoryPool )
							{
								ToggleItemsFilter();
							}
						}
					#else
						// only handle border button keyboard equivalents if the button is visible!
						if ( !fShowMapInventoryPool )
						{
							ToggleItemsFilter();
						}
					#endif

					break;

				case 'K':
					//CHRISL: Swap gunsling
					if ( bSelectedInfoChar != -1 && fShowInventoryFlag && UsingNewInventorySystem() == true )
					{
						SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
						BOOLEAN handFit = (CanItemFitInPosition(pSoldier, &pSoldier->inv[HANDPOS], GUNSLINGPOCKPOS, FALSE) || (pSoldier->inv[HANDPOS].exists() == false && pSoldier->inv[SECONDHANDPOS].exists() == false));
						BOOLEAN slingFit = (CanItemFitInPosition(pSoldier, &pSoldier->inv[GUNSLINGPOCKPOS], HANDPOS, FALSE) || pSoldier->inv[GUNSLINGPOCKPOS].exists() == false);
						if(Item[pSoldier->inv[GUNSLINGPOCKPOS].usItem].twohanded && pSoldier->inv[SECONDHANDPOS].exists() == true)
							handFit = FALSE;
						if( handFit == TRUE && slingFit == TRUE)
						{
							SwapObjs(&pSoldier->inv[HANDPOS], &pSoldier->inv[GUNSLINGPOCKPOS]);
						}
						fTeamPanelDirty = TRUE;
						fMapPanelDirty = TRUE;
						fInterfacePanelDirty = DIRTYLEVEL2;
					}
					break;

					// haydent
				case 'k':
					if (fAlt)
					{
						if (is_networked)
							kick_player();
					}
					else if(gGameExternalOptions.fEnableInventoryPoolQ && fShowMapInventoryPool == TRUE)//dnl ch51 081009
					{
						CopySectorInventoryToInventoryPoolQs(0);
						//break;
					}
					break;

				case 'l':
					if( fAlt )
					{
						// although we're not actually going anywhere, we must still be in a state where this is permitted
						if ( AllowedToExitFromMapscreenTo( MAP_EXIT_TO_LOAD ) )
						{
							DoQuickLoad();
						}
					}
					else if( fCtrl )
					{
						// go to LOAD screen
						gfSaveGame = FALSE;
						RequestTriggerExitFromMapscreen( MAP_EXIT_TO_LOAD );
					}
					else
					{
						// go to LAPTOP
						RequestTriggerExitFromMapscreen( MAP_EXIT_TO_LAPTOP );
					}
					break;
				case 'm':
					// only handle border button keyboard equivalents if the button is visible!
					if ( !fShowMapInventoryPool )
					{
						// toggle show mines flag
						ToggleShowMinesMode();
					}

					break;
				case 'n':
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							static UINT16 gQuoteNum = 0;
							// Get Soldier
							TacticalCharacterDialogue( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ], gQuoteNum );
							gQuoteNum++;
						}
						else if( fCtrl )
						{
							static UINT16 gQuoteNum = 0;
							// Get Soldier
							if ( giHighLine != -1 )
							{
								TacticalCharacterDialogue( MercPtrs[ gCharactersList[ giHighLine ].usSolID ], gQuoteNum );
								gQuoteNum++;
							}
						}
					#endif
					break;
				case 'N':
					if ( bSelectedInfoChar != -1 && fShowInventoryFlag )
					{
						SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
						SwapGoggles(pSoldier);
					}
					break;
				case 'o':
					if( fAlt )
					{	// toggle if Orta & Tixa have been found
						#ifdef JA2TESTVERSION
							fFoundOrta = !fFoundOrta;
							fFoundTixa = !fFoundTixa;
							fMapPanelDirty = TRUE;
						#endif
					}
					else
					{
						// go to OPTIONS screen
						RequestTriggerExitFromMapscreen( MAP_EXIT_TO_OPTIONS );
					}
					break;

					case 'p':
					#ifdef JA2TESTVERSION
							if( fCtrl )
							{
								// CTRL-P: Display player's highest progress percentage
								DumpSectorDifficultyInfo();
							}
					#endif

					#ifdef JA2TESTVERSION
						// ALT-P: Make the selected character a POW!
						if( ( fAlt ) && ( bSelectedInfoChar != -1 ) )
						{
							SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];

							EnemyCapturesPlayerSoldier( pSoldier );

							if ( pSoldier->bInSector )
							{
								RemoveSoldierFromTacticalSector( pSoldier, TRUE );
							}

							fTeamPanelDirty = TRUE;
							fCharacterInfoPanelDirty = TRUE;
							fMapPanelDirty = TRUE;
						}
					#endif
					break;

				case 'q':
					#ifdef JA2TESTVERSION
						if( fAlt )
						{
							// initialize miners if not already done so (fakes entering Drassen mine first)
							HandleQuestCodeOnSectorEntry( 13, 4, 0);
							// test miner quote system
							IssueHeadMinerQuote( (INT8) (1 + Random(MAX_NUMBER_OF_MINES - 1)), (UINT8) (1 + Random(2)));
						}
					#endif
					break;
				case 'r':
					if( gfPreBattleInterfaceActive )
					{ //activate autoresolve in prebattle interface.
						ActivatePreBattleRetreatAction();
					}
					// WANNE: Only allow when mobile militia is allowed!
					else if (gGameExternalOptions.gfAllowMilitiaGroups)
					{
						// only handle border button keyboard equivalents if the button is visible!
						if ( !fShowMapInventoryPool )
						{
							// toggle show mobile restrictions filter
							ToggleMobileFilter();
						}
					}
					break;
				case 's':
					if( fAlt )
					{
						// although we're not actually going anywhere, we must still be in a state where this is permitted
						if ( AllowedToExitFromMapscreenTo( MAP_EXIT_TO_SAVE ) )
						{
							//if the game CAN be saved
							if( CanGameBeSaved() )
							{
								guiPreviousOptionScreen = guiCurrentScreen;
								DoQuickSave();
							}
							else
							{
								//Display a message saying the player cant save now
								DoMapMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__IRON_MAN_CANT_SAVE_NOW ], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
							}
						}
					}
					else if( fCtrl )
					{
						// go to SAVE screen
						gfSaveGame = TRUE;
						RequestTriggerExitFromMapscreen( MAP_EXIT_TO_SAVE );
					}
					else if(gGameExternalOptions.fEnableInventoryPoolQ && fShowMapInventoryPool == TRUE)//dnl ch51 081009
					{
						SortInventoryPoolQ();
						//break;
					}
					break;
				case 'S':
					if( fCtrl )
					{
						if ( !InSectorStackPopup( ) && !InItemStackPopup( ) && !InItemDescriptionBox( ) && !InKeyRingPopup( ) )
						{
							//CHRISL: Sell all items
							if(gGameExternalOptions.fSellAll == TRUE)
							{
								if(!fShowMapInventoryPool)
								{
									fShowMapInventoryPool = TRUE;
									CreateDestroyMapInventoryPoolButtons( TRUE );
								}
								DoMessageBox( MSG_BOX_BASIC_STYLE, NewInvMessage[NIV_SELL_ALL], guiCurrentScreen, ( UINT8 )MSG_BOX_FLAG_YESNO, BeginSellAllCallBack, NULL );
								if(fShowMapInventoryPool)
								{
									fShowMapInventoryPool = FALSE;
								}
							}
						}
					}
					break;
				case 't':
					// Teleport: CTRL-T
					if( ( fCtrl )&&( CHEATER_CHEAT_LEVEL( ) ) )
					{
						// check if selected dest char,
						if( ( bSelectedDestChar != -1 ) && ( fPlotForHelicopter == FALSE ) && ( iCurrentMapSectorZ == 0 ) && ( GetMouseMapXY( &sMapX, &sMapY ) ) )
						{
							INT16 sDeltaX, sDeltaY;
							INT16 sPrevX, sPrevY;
							SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedDestChar ].usSolID ];

							// can't teleport to where we already are
							if ( ( sMapX == pSoldier->sSectorX ) && ( sMapY == pSoldier->sSectorY ) )
								break;

/*
							if( fZoomFlag == TRUE )
							{
								// convert to zoom out coords from screen coords
								sMapX = ( INT16 )( iZoomX / MAP_GRID_X + sMapX ) / 2;
								sMapY = ( INT16 )( iZoomY / MAP_GRID_Y + sMapY ) / 2;
							}
*/

							// cancel movement plotting
							AbortMovementPlottingMode( );

							// nuke the UI message generated by this
							CancelMapUIMessage( );

							// clear their strategic movement (mercpaths and waypoints)
							ClearMvtForThisSoldierAndGang( pSoldier );

							// select this sector
							ChangeSelectedMapSector( sMapX, sMapY, 0 );

							// check to see if this person is moving, if not...then assign them to mvt group
							if( pSoldier->ubGroupID	== 0 )
							{
								ubGroupId = CreateNewPlayerGroupDepartingFromSector( ( INT8 ) ( pSoldier->sSectorX ) , ( INT8 ) ( pSoldier->sSectorY ) );
								// assign to a group
								AddPlayerToGroup( ubGroupId, pSoldier );
							}

							// figure out where they would've come from
							sDeltaX = sMapX - pSoldier->sSectorX;
							sDeltaY = sMapY - pSoldier->sSectorY;

							if ( abs( sDeltaX ) >= abs( sDeltaY ) )
							{
								// use East or West
								if( sDeltaX > 0 )
								{
									// came in from the West
									sPrevX = sMapX - 1;
									sPrevY = sMapY;
								}
								else
								{
									// came in from the East
									sPrevX = sMapX + 1;
									sPrevY = sMapY;
								}
							}
							else
							{
								// use North or South
								if( sDeltaY > 0 )
								{
									// came in from the North
									sPrevX = sMapX;
									sPrevY = sMapY - 1;
								}
								else
								{
									// came in from the South
									sPrevX = sMapX;
									sPrevY = sMapY + 1;
								}
							}

							// set where they are, were/are going, then make them arrive there and check for battle
							PlaceGroupInSector( pSoldier->ubGroupID, sPrevX, sPrevY, sMapX, sMapY, 0, TRUE );

							// unload the sector they teleported out of
							CheckAndHandleUnloadingOfCurrentWorld();
						}
					}
					else
					{
						// only handle border button keyboard equivalents if the button is visible!
						if ( !fShowMapInventoryPool )
						{
							// Toggle show teams flag
							ToggleShowTeamsMode();
						}
					}
					break;
				case 'u':
					#ifdef JA2TESTVERSION
					{
						if( fAlt )
						{
							UINT32 uiCnt;
							// initialize miners if not already done so (fakes entering Drassen mine first)
							HandleQuestCodeOnSectorEntry( 13, 4, 0);
							// test running out
							for (uiCnt = 0; uiCnt < 10; uiCnt++)
							{
								HandleIncomeFromMines();
							}
						}
					}
					#endif
					break;
				case 'v':
					if( fCtrl )
					{
						#ifdef SGP_VIDEO_DEBUGGING
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"VObjects:	%d", guiVObjectSize );
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"VSurfaces:	%d", guiVSurfaceSize );
							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"SGPVideoDump.txt updated..." );
							PerformVideoInfoDumpIntoFile( "SGPVideoDump.txt", TRUE );
						#endif
					}
					else
					{
						DisplayGameSettings( );
					}
					break;
				case 'W':
					if( fAlt )
					{
						if ( CHEATER_CHEAT_LEVEL( ) )
						{
							if ( InItemDescriptionBox( ) )
							{
								CycleItemDescriptionItem( MAP_ITEMDESC_START_X, MAP_ITEMDESC_START_Y );
							}
						}
					}
					else
					{
						if(bSelectedInfoChar != -1)
						{
							SOLDIERTYPE *pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
							//CHRISL: Try to update InSector value so we don't have to "activate" a sector
							if(pSoldier->sSectorX == sSelMapX && pSoldier->sSectorY == sSelMapY && pSoldier->bSectorZ == iCurrentMapSectorZ && !pSoldier->flags.fBetweenSectors)
								pSoldier->bInSector=TRUE;
							if(OK_CONTROL_MERC( pSoldier ))
								{
							if(!fShowMapInventoryPool)
							{
								fShowMapInventoryPool = TRUE;
								CreateDestroyMapInventoryPoolButtons( TRUE );
							}
							if(!fShowInventoryFlag)
							{
								fShowInventoryFlag = TRUE;
							}
								//tais: borrowed the drop all items code from CHRISL to make it drop EVERYTHING!
								if ( bSelectedInfoChar != -1 && fShowInventoryFlag && fShowMapInventoryPool && !(gTacticalStatus.fEnemyInSector) )
								{
									for(int i = BODYPOSFINAL; i<NUM_INV_SLOTS; i++)
									{
										if(pSoldier->inv[i].exists() == true)
										{
											AutoPlaceObjectInInventoryStash(&pSoldier->inv[i], pSoldier->sGridNo);
											DeleteObj(&pSoldier->inv[i]);
										}
										fTeamPanelDirty = TRUE;
										fMapPanelDirty = TRUE;
										fInterfacePanelDirty = DIRTYLEVEL2;
									}
									for(int i = HELMETPOS; i<BODYPOSFINAL; i++)
									{
										if(pSoldier->inv[i].exists() == true)
										{
											AutoPlaceObjectInInventoryStash(&pSoldier->inv[i], pSoldier->sGridNo);
											DeleteObj(&pSoldier->inv[i]);
										}
										fTeamPanelDirty = TRUE;
										fMapPanelDirty = TRUE;
										fInterfacePanelDirty = DIRTYLEVEL2;
									}
								}
							}
						}
					}
					break;
				case 'w':
					if( fAlt )
					{
						if ( CHEATER_CHEAT_LEVEL( ) )
						{
							if ( InItemDescriptionBox( ) )
							{
								CycleItemDescriptionItem( MAP_ITEMDESC_START_X, MAP_ITEMDESC_START_Y );
							}
						}
					}
					else if( fCtrl )
					{
					}
					else
					{
						// only handle border button keyboard equivalents if the button is visible!
						if ( !fShowMapInventoryPool )
						{
							// toggle show towns filter
							ToggleShowTownsMode();
						}
					}
					break;
				case 'x':
					if( fAlt )
					{
						HandleShortCutExitState( );
					}
					break;

				case 'y':
					// ALT-Y: toggles SAM sites disable
					if( fAlt )
					{
						#ifdef JA2TESTVERSION
							fSAMSitesDisabledFromAttackingPlayer = !fSAMSitesDisabledFromAttackingPlayer;
						#endif
					}
					// OJW - 20090208
					// Adding ingame chat
					else
					{
						if (is_networked && is_client)
						{
							OpenChatMsgBox();
						}
					}
					break;

				case 'z':
					// only handle border button keyboard equivalents if the button is visible!
					if( fCtrl )
					{
						if( CHEATER_CHEAT_LEVEL( ) )
						{
							if(	gfAutoAIAware ^= 1 )
								ScreenMsg( FONT_WHITE, MSG_TESTVERSION, L"Strategic AI awareness maxed." );
							else
								ScreenMsg( FONT_WHITE, MSG_TESTVERSION, L"Strategic AI awareness normal." );
						}
					}
					else if ( !fShowMapInventoryPool )
					{
						// Toggle Show Militia ON/OFF
						ToggleShowMilitiaMode();
					}
					break;
			}
		}
		else if( InputEvent.usEvent == KEY_REPEAT )
		{
			switch( InputEvent.usParam )
			{
				case LEFTARROW:
					// previous character
					GoToPrevCharacterInList( );
					break;
				case RIGHTARROW:
					// next character
					GoToNextCharacterInList( );
					break;

				case UPARROW:
					// up a line
					MapScreenMsgScrollUp( 1 );
					break;
				case DNARROW:
					// down a line
					MapScreenMsgScrollDown( 1 );
					break;
				case SHIFT_PGUP:
					// up a page
					MapScreenMsgScrollUp( MAX_MESSAGES_ON_MAP_BOTTOM );
					break;
				case SHIFT_PGDN:
					// down a page
					MapScreenMsgScrollDown( MAX_MESSAGES_ON_MAP_BOTTOM );
					break;
			}
		}
	}
}




void EndMapScreen( BOOLEAN fDuringFade )
{
	if ( fInMapMode == FALSE )
	{
		// shouldn't be here
		return;
	}

/*
	// exit is called due to message box, leave
	if( fMapExitDueToMessageBox )
	{
		fMapExitDueToMessageBox = FALSE;
		return;
	}
*/

	fLeavingMapScreen = FALSE;

	SetRenderFlags( RENDER_FLAG_FULL );
	//MSYS_EnableRegion( &gViewportRegion );
	//MSYS_EnableRegion( &gRadarRegion );
	// ATE: Shutdown tactical interface panel
//	ShutdownCurrentPanel( );

	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );

		return;
	}


	// still plotting movement?
	if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
	{
		AbortMovementPlottingMode( );
	}

	DestroyMouseRegionsForTeamList( );

	MSYS_RemoveRegion( &gMapViewRegion );
	MSYS_RemoveRegion( &gCharInfoFaceRegion);
	MSYS_RemoveRegion( &gCharInfoHandRegion );
	MSYS_RemoveRegion( &gMPanelRegion);
	MSYS_RemoveRegion( &gMapScreenMaskRegion );
	fInMapMode = FALSE;

	// remove team panel sort button
	RemoveTeamPanelSortButtonsForMapScreen( );

	// OJW - 20081204
	if (is_networked)
		RemoveMPButtonsForMapScreen( );
	// for th merc insurance help text
	CreateDestroyInsuranceMouseRegionForMercs( FALSE );

	// gonna need to remove the screen mask regions
	CreateDestroyMouseRegionMasksForTimeCompressionButtons( );

	UnloadButtonImage( giMapContractButtonImage );
	RemoveButton( giMapContractButton );

	HandleShutDownOfMapScreenWhileExternfaceIsTalking( );

	fShowInventoryFlag = FALSE;
	CreateDestroyMapInvButton();

	// no longer can we show assignments menu
	fShowAssignmentMenu = FALSE;

	// clear out mouse regions for pop up boxes
	DetermineWhichAssignmentMenusCanBeShown( );

	DetermineWhichMilitiaControlMenusCanBeShown( ); //lal
	sSelectedMilitiaTown = 0;
	CreateDestroyMilitiaPopUPRegions( );
	CreateDestroyMilitiaSectorButtons( );

	// stop showing contract menu
	fShowContractMenu = FALSE;
	// clear out contract menu
	DetermineIfContractMenuCanBeShown( );
	// remove contract pop up box (always created upon mapscreen entry)
	RemoveBox(ghContractBox);
	ghContractBox = -1;
	// OJW
	if (is_networked)
	{
		DestroyMPCompassBox();
		DestroyMPTeamBox();
	}

	CreateDestroyAssignmentPopUpBoxes( );
	CreateDestroyMilitiaControlPopUpBoxes( ); //lal

	// shutdown movement box
	if ( fShowMapScreenMovementList )
	{
		fShowMapScreenMovementList = FALSE;
		CreateDestroyMovementBox( 0, 0, 0 );
	}

	// the remove merc from team box
	RemoveBox( ghRemoveMercAssignBox );
	ghRemoveMercAssignBox = -1;

	// clear screen mask if needed
	ClearScreenMaskForMapScreenExit( );

	// get rid of pause clock area
	RemoveMouseRegionForPauseOfClock( );

	// get rid of pop up for town info, if being shown
	fShowTownInfo = FALSE;
	CreateDestroyTownInfoBox( );

	// build squad list
	RebuildCurrentSquad( );

	DeleteMouseRegionsForLevelMarkers( );

	if( fShowMapInventoryPool == FALSE )
	{
		// delete buttons
		DeleteMapBorderButtons( );
	}

	if(fShowDescriptionFlag)
	{
	DeleteItemDescriptionBox( );
	}


	fShowInventoryFlag = FALSE;
	CreateDestroyTrashCanRegion( );

	if ( !fDuringFade )
	{
	MSYS_SetCurrentCursor(SCREEN_CURSOR);
	}

	if(fPreLoadedMapGraphics == FALSE )
	{
		DeleteMapBottomGraphics( );

		DeleteVideoObjectFromIndex(guiSubLevel1);
		DeleteVideoObjectFromIndex(guiSubLevel2);
		DeleteVideoObjectFromIndex(guiSubLevel3);
		DeleteVideoObjectFromIndex( guiSleepIcon );
		DeleteVideoObjectFromIndex(guiMAPCURSORS);
		//DeleteVideoObjectFromIndex(guiMAPBORDER);
		DeleteVideoObjectFromIndex(guiCHARLIST);
		//DeleteVideoObjectFromIndex(guiCORNERADDONS);
		//DeleteVideoObjectFromIndex(guiMAPCORNER);
		//DeleteVideoObjectFromIndex(guiPOPUPBORDERS);
		DeleteVideoObjectFromIndex(guiCHARINFO);
		DeleteVideoObjectFromIndex(guiCHARICONS);
		DeleteVideoObjectFromIndex(guiCROSS);
		DeleteVideoSurfaceFromIndex(guiBIGMAP);
	//	DeleteVideoSurfaceFromIndex(guiPOPUPTEX);
		DeleteVideoObjectFromIndex(guiSAMICON);
		DeleteVideoObjectFromIndex(guiMAPINV);
		DeleteVideoObjectFromIndex(guiMapInvSecondHandBlockout);
		DeleteVideoObjectFromIndex(guiULICONS);
		DeleteVideoObjectFromIndex(guiORTAICON);
		DeleteVideoObjectFromIndex(guiTIXAICON);
		DeleteVideoObjectFromIndex( guiCHARBETWEENSECTORICONS );
		DeleteVideoObjectFromIndex( guiCHARBETWEENSECTORICONSCLOSE );
		DeleteVideoObjectFromIndex( guiLEVELMARKER );
		DeleteVideoObjectFromIndex( guiMapBorderEtaPopUp );

		DeleteVideoObjectFromIndex( guiSecItemHiddenVO );
		DeleteVideoObjectFromIndex( guiSelectedCharArrow );
		DeleteVideoObjectFromIndex( guiMapBorderHeliSectors );
		DeleteVideoObjectFromIndex( guiHelicopterIcon );
		DeleteVideoObjectFromIndex( guiMINEICON );
		DeleteVideoObjectFromIndex( guiSectorLocatorGraphicID );

		DeleteVideoObjectFromIndex( guiBULLSEYE );

		// remove the militia pop up box
		RemoveMilitiaPopUpBox( );

		// remove inventory pool graphic
		RemoveInventoryPoolGraphic( );

		// get rid of border stuff
		DeleteMapBorderGraphics( );

		//Kris:	Remove the email icons.
		DeleteVideoObjectFromIndex( guiNewMailIcons );
	}

	DeleteVideoObjectFromIndex( guiBrownBackgroundForTeamPanel );

	RemoveMapStatusBarsRegion( );

	fShowUpdateBox = FALSE;
	CreateDestroyTheUpdateBox( );


	// get rid of mapscreen bottom
	DeleteMapScreenInterfaceBottom( );


	// shutdown any mapscreen UI overlay message
	CancelMapUIMessage( );


	CreateDestroyMapCharacterScrollButtons( );

	// if time was ever compressed while we were in mapscreen
	if ( HasTimeCompressOccured( ) )
	{
		// make sure everything tactical got cleared out
		ClearTacticalStuffDueToTimeCompression();
	}


	CancelSectorInventoryDisplayIfOn( TRUE );


	SetAllAutoFacesInactive( );
	if(fLapTop)
	{
		StopAnyCurrentlyTalkingSpeech( );
	guiCurrentScreen=LAPTOP_SCREEN;
	}
	else
	{
	guiCurrentScreen = GAME_SCREEN;

		// remove the progress bar
		RemoveProgressBar( 0 );

		// enable scroll string video overlays
		EnableDisableScrollStringVideoOverlay( TRUE );
	}

	// if going to tactical next
	if ( guiPendingScreen == GAME_SCREEN )
	{
		// set compressed mode to Normal (X1)
		SetGameTimeCompressionLevel( TIME_COMPRESS_X1 );
	}
	else	// going to another screen (options, laptop, save/load)
	{
		StopTimeCompression();
	}

	// update paused states, we are exiting...need to reset for any pathing or menus displayed
	UnLockPauseState( );
	UpdatePausedStatesDueToTimeCompression( );

	if( !gfDontStartTransitionFromLaptop )
	{
		VOBJECT_DESC	VObjectDesc;
		UINT32 uiLaptopOn;

		//Load a tiny graphic of the on screen and draw it to the buffer.
		PlayJA2SampleFromFile( "SOUNDS\\Initial Power Up (8-11).wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\LaptopOn.sti" );
		if( !AddVideoObject( &VObjectDesc, &uiLaptopOn ) )
			AssertMsg( 0, "Failed to load data\\Interface\\LaptopOn.sti" );
		BltVideoObjectFromIndex( FRAME_BUFFER, uiLaptopOn, 0, 465, 417, VO_BLT_SRCTRANSPARENCY, NULL );
		InvalidateRegion( 465, 417, 480, 427 );
		ExecuteBaseDirtyRectQueue( );
		EndFrameBufferRender( );
		DeleteVideoObjectFromIndex( uiLaptopOn );
		RefreshScreen( NULL );
	}


	//Kris:	Removes the pre battle interface, but only if it exists.
	//		It is internally considered.
	KillPreBattleInterface();

	// cancel request if we somehow leave first
	gfRequestGiveSkyriderNewDestination = FALSE;
}


BOOLEAN GetMouseMapXY( INT16 *psMapWorldX, INT16 *psMapWorldY )
{
	POINT	MousePos;

	if( IsMapScreenHelpTextUp( ) )
	{
		// don't show highlight while global help text is up
		return( FALSE );
	}


	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	if(fZoomFlag)
	{
	if(MousePos.x >MAP_GRID_X+MAP_VIEW_START_X)
	MousePos.x-=MAP_GRID_X;
	if(MousePos.x >MAP_VIEW_START_X+MAP_VIEW_WIDTH)
		MousePos.x=-1;
	if(MousePos.y > MAP_GRID_Y+MAP_VIEW_START_Y)
	MousePos.y-=MAP_GRID_Y;
	if(MousePos.y >MAP_VIEW_START_Y+MAP_VIEW_HEIGHT-11)
		MousePos.y=-11;
	if(MousePos.y < MAP_VIEW_START_Y)
		MousePos.y=-1;
	}

	return( GetMapXY( (INT16)MousePos.x, (INT16)MousePos.y, psMapWorldX, psMapWorldY ) );
}




BOOLEAN GetMapXY( INT16 sX, INT16 sY, INT16 *psMapWorldX, INT16 *psMapWorldY )
{
	INT16 sMapX, sMapY;

	// Subtract start of map view
	sMapX = sX - MAP_VIEW_START_X;//+2*MAP_GRID_X;
	sMapY = sY - MAP_VIEW_START_Y;

	if(!fZoomFlag)
	{
		if ( sMapX < MAP_GRID_X || sMapY < MAP_GRID_Y )
	{
		return( FALSE );
	}
	}
	if ( sMapX < 0 || sMapY < 0 )
	{
		return( FALSE );
	}

	if ( sMapX > MAP_VIEW_WIDTH+MAP_GRID_X-1 || sMapY > MAP_VIEW_HEIGHT+7/* +MAP_VIEW_HEIGHT */ )
	{
		return( FALSE );
	}
	if(sMapX < 1 || sMapY <1)
	{
		return (FALSE);
	}

	*psMapWorldX = ( sMapX / MAP_GRID_X );
 	*psMapWorldY = ( sMapY / MAP_GRID_Y );

	return( TRUE );
}



void RenderMapHighlight( INT16 sMapX, INT16 sMapY, UINT16 usLineColor, BOOLEAN fStationary )
{
	INT16												sScreenX, sScreenY;
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;


	Assert( ( sMapX >= 1 ) && ( sMapX <= 16 ) );
	Assert( ( sMapY >= 1 ) && ( sMapY <= 16 ) );

/*
	if((fZoomFlag)&&((sMapX > MAP_WORLD_X-1)||(sMapY> MAP_WORLD_Y-1)))
	return;
*/

	// if we are not allowed to highlight, leave
	if( ( IsTheCursorAllowedToHighLightThisSector( sMapX, sMapY ) == FALSE )&&( fZoomFlag == FALSE ) )
	{
		return;
	}
/*
	else if( ( IsTheCursorAllowedToHighLightThisSector( sMapX , sMapY ) == FALSE )&&( fZoomFlag == TRUE ) && ( fStationary == TRUE ) )
	{
		return;
	}
	else if( ( IsTheCursorAllowedToHighLightThisSector( ( INT16 ) ( ( ( iZoomX ) / ( MAP_GRID_X * 2 ) ) + sMapX / 2 ) ,( INT16 ) ( ( ( iZoomY ) / ( MAP_GRID_Y * 2 ) ) + sMapY / 2 ) ) == FALSE ) && ( fZoomFlag == TRUE ) &&( fStationary == FALSE ) )
	{
		return;
	}
*/


//	if((!fStationary)||(!fZoomFlag))
	{
	GetScreenXYFromMapXY( sMapX, sMapY, &sScreenX, &sScreenY );
	}
/*
	else
	{
	GetScreenXYFromMapXYStationary( sMapX, sMapY, &sScreenX, &sScreenY );
	}
*/

	// blit in the highlighted sector
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );

	// clip to view region
	ClipBlitsToMapViewRegionForRectangleAndABit( uiDestPitchBYTES );

	if(gbPixelDepth==16)
	{
		// DB Need to add a radar color for 8-bit
/*
		if (fZoomFlag)
		{
			// draw rectangle for zoom in
		RectangleDraw( TRUE, sScreenX-MAP_GRID_X,	 sScreenY-MAP_GRID_Y - 1, sScreenX +	MAP_GRID_ZOOM_X - MAP_GRID_X, sScreenY +	MAP_GRID_ZOOM_Y - MAP_GRID_Y - 1, usLineColor, pDestBuf );
		InvalidateRegion(	sScreenX-MAP_GRID_X - 3, sScreenY-MAP_GRID_Y - 4, sScreenX + DMAP_GRID_ZOOM_X - MAP_GRID_X, sScreenY + DMAP_GRID_ZOOM_Y - MAP_GRID_Y - 1 );
		}
	else
*/
		{
			// draw rectangle for zoom out
		RectangleDraw( TRUE, sScreenX, sScreenY - 1, sScreenX +	MAP_GRID_X, sScreenY +	MAP_GRID_Y - 1, usLineColor, pDestBuf );
		InvalidateRegion(	sScreenX, sScreenY - 2, sScreenX + DMAP_GRID_X + 1, sScreenY + DMAP_GRID_Y - 1 );
		}
	}

	RestoreClipRegionToFullScreenForRectangle( uiDestPitchBYTES );
	UnLockVideoSurface( FRAME_BUFFER );
}



void PollLeftButtonInMapView( UINT32 *puiNewEvent )
{
	static BOOLEAN	fLBBeenPressedInMapView = FALSE;
	INT16 sMapX, sMapY;


	// if the mouse is currently over the MAP area
	if ( gMapViewRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		// if L-button is down at the moment
		if ( gMapViewRegion.ButtonState & MSYS_LEFT_BUTTON )
		{
			if ( !fLBBeenPressedInMapView )
			{
				fLBBeenPressedInMapView = TRUE;
				RESETCOUNTER( LMOUSECLICK_DELAY_COUNTER );

				gfAllowSkyriderTooFarQuote = FALSE;
			}
		}
		else	// L-button is NOT down at the moment
		{
			if ( fLBBeenPressedInMapView	)
			{
				fLBBeenPressedInMapView = FALSE;
				RESETCOUNTER( LMOUSECLICK_DELAY_COUNTER );

				// if we are showing help text in mapscreen
				if( fShowMapScreenHelpText )
				{
					fShowMapScreenHelpText = FALSE;
					fCharacterInfoPanelDirty = TRUE;
					fMapPanelDirty = TRUE;
					return;
				}

				// if in militia redistribution popup
				if ( sSelectedMilitiaTown != 0 )
				{
					// ignore clicks outside the box
					return;
				}

				// left click cancels MAP UI messages, unless we're in confirm map move mode
				if( ( giUIMessageOverlay != -1 ) && !gfInConfirmMapMoveMode )
				{
					CancelMapUIMessage( );

					// return unless moving the bullseye
					if ( !gfInChangeArrivalSectorMode )
						return;
				}

				// ignore left clicks in the map screen if:
				// game just started or we're in the prebattle interface or if we are about to hit pre-battle
				
				
				if (is_networked)
				{
					if ( /*( gTacticalStatus.fDidGameJustStart == TRUE ) || */
						 // commented out to allow heli drop off change @/b4 start ;)
							( gfPreBattleInterfaceActive == TRUE ) ||
							( fDisableMapInterfaceDueToBattle	== TRUE ) )
					{
						return;
					}
				}
				else
				{
					if ( ( gTacticalStatus.fDidGameJustStart == TRUE ) ||
							( gfPreBattleInterfaceActive == TRUE ) ||
							( fDisableMapInterfaceDueToBattle	== TRUE ) )
					{
						return;
					}
				}


				// if in "plot route" mode
				if ( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
				{
					fEndPlotting = FALSE;

					GetMouseMapXY(&sMapX, &sMapY);

/*
					// translate screen values to map grid values for zoomed in
					if(fZoomFlag)
					{
						sMapX=(UINT16)iZoomX/MAP_GRID_X+sMapX;
						sMapX=sMapX/2;
						sMapY=(UINT16)iZoomY/MAP_GRID_Y+sMapY;
						sMapY=sMapY/2;
					}
*/

					// if he clicked on the last sector in his current path
					if( CheckIfClickOnLastSectorInPath( sMapX, sMapY ) )
					{
						DestinationPlottingCompleted();
					}
					else	// clicked on a new sector
					{
						gfAllowSkyriderTooFarQuote = TRUE;

						// draw new map route
						*puiNewEvent = MAP_EVENT_PLOT_PATH;
					}
				}
		else	// not plotting movement
				{
					// if not plotting a path
					if( ( fEndPlotting == FALSE ) && ( fJustFinishedPlotting == FALSE ) )
					{
						// make this sector selected / trigger movement box / start helicopter plotting / changing arrival sector
						*puiNewEvent = MAP_EVENT_CLICK_SECTOR;
					}

					fEndPlotting = FALSE;
				}

				// reset town info flag
				fShowTownInfo = FALSE;
			}
		}
	}

	fJustFinishedPlotting = FALSE;
}


void PollRightButtonInMapView( UINT32 *puiNewEvent )
{
	static BOOLEAN	fRBBeenPressedInMapView = FALSE;
	INT16 sMapX, sMapY;


	// if the mouse is currently over the MAP area
	if ( gMapViewRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		// if R-button is down at the moment
		if ( gMapViewRegion.ButtonState & MSYS_RIGHT_BUTTON )
		{
			if ( !fRBBeenPressedInMapView )
			{
				fRBBeenPressedInMapView = TRUE;
				RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );
			}
		}
		else	// R-button is NOT down at the moment
		{
			if ( fRBBeenPressedInMapView )
			{
				fRBBeenPressedInMapView = FALSE;
				RESETCOUNTER( RMOUSECLICK_DELAY_COUNTER );

				// if we are showing help text in mapscreen
				if( fShowMapScreenHelpText )
				{
					fShowMapScreenHelpText = FALSE;
					fCharacterInfoPanelDirty = TRUE;
					fMapPanelDirty = TRUE;
					return;
				}

				// if in militia redistribution popup
				if ( sSelectedMilitiaTown != 0 )
				{
					// ignore clicks outside the box
					return;
				}

				if ( gfInChangeArrivalSectorMode )
				{
					CancelChangeArrivalSectorMode( );
					MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pBullseyeStrings[ 3 ] );
					return;
				}

				// right click cancels MAP UI messages, unless we're in confirm map move mode
				if( ( giUIMessageOverlay != -1 ) && !gfInConfirmMapMoveMode )
				{
					CancelMapUIMessage( );
					return;
				}

				// ignore right clicks in the map area if:
				// game just started or we're in the prebattle interface or if we are about to hit pre-battle
				if ( ( gTacticalStatus.fDidGameJustStart == TRUE ) ||
						( gfPreBattleInterfaceActive == TRUE ) ||
						( fDisableMapInterfaceDueToBattle	== TRUE ) )
				{
					return;
				}


				if ( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
				{
					// cancel/shorten the path
					*puiNewEvent = MAP_EVENT_CANCEL_PATH;
				}
				else
				{
					if ( GetMouseMapXY( &sMapX, &sMapY ) )
					{
/*
						if(fZoomFlag)
						{
							sMapX=(UINT16)iZoomX/MAP_GRID_X+sMapX;
							sMapX=sMapX/2;
							sMapY=(UINT16)iZoomY/MAP_GRID_Y+sMapY;
							sMapY=sMapY/2;
						}
*/

						// HEADROCK HAM 4: Toggle Militia Restrictions manually. This occurs when the Mobile Restrictions
						// view is turned on. Each click advances the sector's classification by 1. If max is reached,
						// loops back to start.
						if ( fShowMobileRestrictionsFlag == TRUE )
 						{
							// Restrict more.
							if ( gubManualRestrictMilitia[ SECTOR( sMapX, sMapY ) ] > MANUAL_MOBILE_NO_ENTER )
							{
								gubManualRestrictMilitia[ SECTOR( sMapX, sMapY ) ]--;
							}
							else
							{
								// Wrap back to highest.
								gubManualRestrictMilitia[ SECTOR( sMapX, sMapY ) ] = MANUAL_MOBILE_NO_RESTRICTION;
							}
							fMapPanelDirty = TRUE;
							// Skip the rest of this function.
							return;
						}
						// don't change sectors if we've just done the militia restrictions thing.
						else if( ( sSelMapX != sMapX ) || ( sSelMapY != sMapY ) )
						{
							ChangeSelectedMapSector( sMapX, sMapY, ( INT8 )iCurrentMapSectorZ );
						}
					}

					// sector must be selected to bring up militia or town info boxes for it
					if ( ( sMapX == sSelMapX ) && ( sSelMapY == sMapY ) )
					{
						if ( fShowMilitia == TRUE )
						{
							HandleMilitiaRedistributionClick();
						}
						else // show militia is OFF
						{
							// if on the surface, or a real underground sector (we've visited it)
							if ( ( iCurrentMapSectorZ == 0 ) ||
									( GetSectorFlagStatus( sMapX, sMapY, ( UINT8 ) iCurrentMapSectorZ, SF_ALREADY_VISITED ) == TRUE ) )
							{
								// toggle sector info for this sector
								fShowTownInfo = !fShowTownInfo;
								fMapPanelDirty = TRUE;
							}
						}

//						fMapScreenBottomDirty = TRUE;

						CreateDestroyScreenMaskForAssignmentAndContractMenus( );
						if( fShowTownInfo == FALSE )
						{
							// destroy town info box
							CreateDestroyTownInfoBox( );
						}
					}
				}
			}
		}
	}
}



void PopupText( STR16 pFontString, ...	)
{
	UINT8	*pDestBuf;
	UINT32 uiDestPitchBYTES;
	va_list argptr;
	INT16 sX, sY;
	CHAR16	PopupString[512];

	va_start(argptr, pFontString);			// Set up variable argument pointer
	vswprintf(PopupString, pFontString, argptr);	// process gprintf string (get output str)
	va_end(argptr);


	FindFontCenterCoordinates( 0, 0, SCREEN_WIDTH, INTERFACE_START_Y, PopupString, LARGEFONT1, &sX, &sY );

	BltVideoSurface( FRAME_BUFFER, guiINTEXT, 0, 85, 160, VS_BLT_FAST | VS_BLT_USECOLORKEY, NULL );

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES);

	SetFont( LARGEFONT1 );
	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( FONT_MCOLOR_DKGRAY );

	mprintf_buffer( pDestBuf, uiDestPitchBYTES, LARGEFONT1, sX, sY, PopupString );

	UnLockVideoSurface( FRAME_BUFFER );

	InvalidateScreen( );

}


/*
void BtnINVCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(fMapInventoryItem)
			return;
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		fCharacterInfoPanelDirty = TRUE;
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&= ~(BUTTON_CLICKED_ON);

		if(!fMapInventoryItem)
		{
		fShowInventoryFlag = FALSE;
		}

		// set help text for item glow region
		if( fShowInventoryFlag )
		{
			SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 2 ] );
		}
		else
		{
			SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );
		}

		fTeamPanelDirty = TRUE;
		}
	}
}
*/


void CreateDestroyMapInvButton()
{
 static BOOLEAN fOldShowInventoryFlag=FALSE;

 if( fShowInventoryFlag && !fOldShowInventoryFlag )
 {
	// create inventory button
	fOldShowInventoryFlag=TRUE;
	//giMapInvButtonImage=	LoadButtonImage( "INTERFACE\\mapinv.sti" ,-1,1,-1,2,-1 );
	//giMapInvButton= QuickCreateButton( giMapInvButtonImage, INV_BTN_X-1, INV_BTN_Y,
						//				BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
						//				BUTTON_NO_CALLBACK, (GUI_CALLBACK)BtnINVCallback);
	// disable allmouse regions in this space
	fTeamPanelDirty=TRUE;

	// CHRISL: Setup default coords
	if((UsingNewInventorySystem() == true))
		InitializeInvPanelCoordsNew();
	else
		InitializeInvPanelCoordsOld();

	InitInvSlotInterface( gMapScreenInvPocketXY, &gSCamoXY, MAPInvMoveCallback, MAPInvClickCallback, MAPInvMoveCamoCallback, MAPInvClickCamoCallback, FALSE );
	MSYS_EnableRegion(&gMPanelRegion);

	// switch hand region help text to "Exit Inventory"
	SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 2 ] );

	// reset inventory item help text
	memset( gubMAP_HandInvDispText, 0, sizeof( gubMAP_HandInvDispText ) );

	// dirty character info panel	( Why? ARM )
	fCharacterInfoPanelDirty=TRUE;
	fResetMapCoords=TRUE;
 }
 else if( !fShowInventoryFlag && fOldShowInventoryFlag )
 {
	// destroy inventory button
	ShutdownInvSlotInterface( );
	fOldShowInventoryFlag=FALSE;
	//RemoveButton( giMapInvButton );
	//UnloadButtonImage( giMapInvButtonImage );
	fTeamPanelDirty=TRUE;
	MSYS_DisableRegion(&gMPanelRegion);

	// switch hand region help text to "Enter Inventory"
	SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );

	// force immediate reblit of item in HANDPOS now that it's not blitted while in inventory mode
	fCharacterInfoPanelDirty=TRUE;
 }
}



void BltCharInvPanel()
{
	UINT32 uiDestPitchBYTES;
	UINT16	*pDestBuf;
	HVOBJECT hCharListHandle;
	SOLDIERTYPE	*pSoldier;
	CHAR16 sString[ 32 ];
	INT16 usX, usY;
	UINT8 disOpt = 0;


	// make sure we're here legally
	Assert( MapCharacterHasAccessibleInventory( bSelectedInfoChar ) );

	GetSoldier( &pSoldier, gCharactersList[bSelectedInfoChar].usSolID );


	pDestBuf = (UINT16*)LockVideoSurface( guiSAVEBUFFER, &uiDestPitchBYTES);
	GetVideoObject(&hCharListHandle, guiMAPINV);
	// CHRISL: Changed last parameter so we can display graphic based on inventory system used
	if((UsingNewInventorySystem() == true) && gGameExternalOptions.fVehicleInventory && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
	{
		if(fResetMapCoords)
		{
			InitializeInvPanelCoordsVehicle();
			// This function sets the mouse regions for the vehicle inventory
			InitInventoryVehicle(gMapScreenInvPocketXY, MAPInvMoveCallback, MAPInvClickCallback, FALSE);
			fResetMapCoords=FALSE;
		}
		disOpt = 2;
	}
	else if((UsingNewInventorySystem() == true))
	{
		if(fResetMapCoords)
		{
			InitializeInvPanelCoordsNew();
			// This function sets the mouse regions for the soldier inventory
			InitInventorySoldier(gMapScreenInvPocketXY, MAPInvMoveCallback, MAPInvClickCallback, FALSE, TRUE);
			fResetMapCoords=FALSE;
		}
		disOpt = 1;
	}
	else
	{
		//InitializeInvPanelCoordsOld();
		InitInventorySoldier(gMapScreenInvPocketXY, MAPInvMoveCallback, MAPInvClickCallback, FALSE, FALSE);
	}

	Blt8BPPDataTo16BPPBufferTransparent( pDestBuf, uiDestPitchBYTES, hCharListHandle, PLAYER_INFO_X, PLAYER_INFO_Y, disOpt);
	UnLockVideoSurface( guiSAVEBUFFER ); 
	
	Assert( pSoldier );
	CreateDestroyMapInvButton();

	if( gbCheckForMouseOverItemPos != -1 )
	{
		if( HandleCompatibleAmmoUIForMapScreen( pSoldier, (INT32)gbCheckForMouseOverItemPos, TRUE, TRUE ) == TRUE )
		{
			fMapPanelDirty = TRUE;
		}
	}

	if( ( fShowMapInventoryPool ) )
	{
		if( iCurrentlyHighLightedItem != -1 )
		{
			HandleCompatibleAmmoUIForMapScreen( pSoldier, (INT32)( iCurrentlyHighLightedItem + ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) ) , TRUE, FALSE );
		}
	}

	RenderInvBodyPanel(pSoldier, INV_BODY_X, INV_BODY_Y );

	// reset font destination buffer to the save buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// render items in each of chars slots
	HandleRenderInvSlots( pSoldier, DIRTYLEVEL2 );

	// reset font destination buffer
	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

 	SetFont( BLOCKFONT2 );

	// Render Values for stats!
	// Set font drawing to saved buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( MAP_INV_STATS_TITLE_FONT_COLOR );

	// CHRISL: Only display next three values if we're a merc
//	if(!(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
	if(!(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
	{
		// print armor/weight/camo labels
		mprintf( MAP_ARMOR_LABEL_X, MAP_ARMOR_LABEL_Y, pInvPanelTitleStrings[ 0 ] );

		#ifdef CHINESE
			mprintf( MAP_ARMOR_PERCENT_X, MAP_ARMOR_PERCENT_Y, ChineseSpecString1 );
		#else
			mprintf( MAP_ARMOR_PERCENT_X, MAP_ARMOR_PERCENT_Y, L"%%" );
		#endif

		mprintf( MAP_WEIGHT_LABEL_X, MAP_WEIGHT_LABEL_Y, pInvPanelTitleStrings[ 1 ]  );
		
		#ifdef CHINESE
			mprintf( MAP_WEIGHT_PERCENT_X, MAP_WEIGHT_PERCENT_Y, ChineseSpecString1 );
		#else
			mprintf( MAP_WEIGHT_PERCENT_X, MAP_WEIGHT_PERCENT_Y, L"%%" );
		#endif

		mprintf( MAP_CAMMO_LABEL_X, MAP_CAMMO_LABEL_Y, pInvPanelTitleStrings[ 2 ]  );
		
		#ifdef CHINESE
			mprintf( MAP_CAMMO_PERCENT_X, MAP_CAMMO_PERCENT_Y, ChineseSpecString1 );
		#else
			mprintf( MAP_CAMMO_PERCENT_X, MAP_CAMMO_PERCENT_Y, L"%%" );
		#endif

		// display armor value
		swprintf( sString, L"%3d", ArmourPercent( pSoldier ) );
		FindFontRightCoordinates(MAP_ARMOR_X, MAP_ARMOR_Y, MAP_PERCENT_WIDTH, MAP_PERCENT_HEIGHT, sString, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, sString ); 

		// Display weight value
		swprintf( sString, L"%3d", CalculateCarriedWeight( pSoldier ) );
		FindFontRightCoordinates(MAP_WEIGHT_X, MAP_WEIGHT_Y, MAP_PERCENT_WIDTH, MAP_PERCENT_HEIGHT, sString, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, sString ); 

		ApplyEquipmentBonuses(pSoldier);

		// Display camo value
		swprintf( sString, L"%3d", max(0, min ((pSoldier->bCamo + pSoldier->wornCamo + pSoldier->urbanCamo+pSoldier->wornUrbanCamo+pSoldier->desertCamo+pSoldier->wornDesertCamo+pSoldier->snowCamo+pSoldier->wornSnowCamo ),100 )) );
		FindFontRightCoordinates(MAP_CAMMO_X, MAP_CAMMO_Y, MAP_PERCENT_WIDTH, MAP_PERCENT_HEIGHT, sString, BLOCKFONT2, &usX, &usY);
		mprintf( usX, usY, sString ); 
	}

	if( InKeyRingPopup( ) )
	{
		// shade the background
		ShadowVideoSurfaceRect( guiSAVEBUFFER , PLAYER_INFO_X, PLAYER_INFO_Y, PLAYER_INFO_X + 261,	PLAYER_INFO_Y + ( 359 - 107 ));
	}
	else
	{
		// blit gold key on top of key ring if key ring is not empty
	}

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}

// check for and highlight any ammo
void HandleCursorOverRifleAmmo( )
{
	if( fShowInventoryFlag == FALSE )
	{
		return;
	}

	if( gbCheckForMouseOverItemPos == -1 )
	{
		return;
	}

	if ( gfCheckForMouseOverItem )
	{
		if ( HandleCompatibleAmmoUI( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ], (INT8)gbCheckForMouseOverItemPos, TRUE ) )
		{
			if ( ( GetJA2Clock( ) - guiMouseOverItemTime ) > 100 )
			{
				fTeamPanelDirty = TRUE;
			}
		}
	}
}

void MAPInvClickCamoCallback( MOUSE_REGION *pRegion, INT32 iReason )
{


}

void MAPInvMoveCamoCallback( MOUSE_REGION *pRegion, INT32 iReason )
{


}


// this is Map Screen's version of SMInvMoveCallback()
void MAPInvMoveCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	SOLDIERTYPE	*pSoldier;
	UINT32 uiHandPos;


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	// make sure we're here legally
	Assert( MapCharacterHasAccessibleInventory( bSelectedInfoChar ) );

	GetSoldier( &pSoldier, gCharactersList[ bSelectedInfoChar ].usSolID );

	uiHandPos = MSYS_GetRegionUserData( pRegion, 0 );

	//gbCheckForMouseOverItemPos = -1;

	if ( pSoldier->inv[ uiHandPos ].exists() == false )
		return;

	if (iReason == MSYS_CALLBACK_REASON_MOVE)
	{
	}
	else if (iReason == MSYS_CALLBACK_REASON_GAIN_MOUSE )
//	if( ( iReason == MSYS_CALLBACK_REASON_MOVE ) || ( iReason == MSYS_CALLBACK_REASON_GAIN_MOUSE ) )
	{
		gubMAP_HandInvDispText[ uiHandPos ] = 2;
		guiMouseOverItemTime = GetJA2Clock( );
		gfCheckForMouseOverItem = TRUE;
		HandleCompatibleAmmoUI( pSoldier, (INT8)uiHandPos, FALSE );
		gbCheckForMouseOverItemPos = (INT8)uiHandPos;
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		gubMAP_HandInvDispText[ uiHandPos ] = 1;
		HandleCompatibleAmmoUI( pSoldier, (INT8)uiHandPos, FALSE );
		gfCheckForMouseOverItem = FALSE;
		fTeamPanelDirty = TRUE;
	gbCheckForMouseOverItemPos = -1;
	}
}

// mapscreen wrapper to init the item description box
BOOLEAN MAPInternalInitItemDescriptionBox( OBJECTTYPE *pObject, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier )
{
	BOOLEAN fRet;

	fRet = InternalInitItemDescriptionBox( pObject, MAP_ITEMDESC_START_X, MAP_ITEMDESC_START_Y, ubStatusIndex, pSoldier );

	fShowDescriptionFlag=TRUE;
	fTeamPanelDirty=TRUE;
	fInterfacePanelDirty = DIRTYLEVEL2;

	return( fRet );
}

//CHRISL: functons for LBENODE system
extern BOOLEAN ChangeZipperStatus(SOLDIERTYPE *pSoldier, BOOLEAN newStatus);

// this is Map Screen's version of SMInvClickCallback()
void MAPInvClickCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	SOLDIERTYPE	*pSoldier;
	UINT32 uiHandPos;
	UINT16	usOldItemIndex, usNewItemIndex;
	static BOOLEAN	fRightDown = FALSE;

	// OJW - 20090319 - fix merging bug on mapscreen
	// If you left click a merge item such as EBR stock, then left click it on the item to merge, such as M-14...
	// instead of right clicking (which is the correct action to merge)
	// it will open an ItemDescriptionPanel, but then after that call PlaceObject, which performs a swap
	// and overwites the gpItemDescObject pointer. Hence when the ItemDescriptionPanel is deleted
	// it no longer references the right object, and does not necessarily free up all resources
	// creating an error the next time it is called
	bool bJustOpenedItemDescPanel = false;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	// make sure we're here legally
	Assert( MapCharacterHasAccessibleInventory( bSelectedInfoChar ) );

	GetSoldier( &pSoldier, gCharactersList[ bSelectedInfoChar ].usSolID );

	uiHandPos = MSYS_GetRegionUserData( pRegion, 0 );

	// CHRISL: Are we in combat, wearing a backpack with the zipper closed?  Don't allow access to backpack items
	if((UsingNewInventorySystem() == true))
		if(icLBE[uiHandPos] == BPACKPOCKPOS && (!(pSoldier->flags.ZipperFlag) || (pSoldier->flags.ZipperFlag && gAnimControl[pSoldier->usAnimState].ubEndHeight == ANIM_STAND)) && (gTacticalStatus.uiFlags & INCOMBAT) && (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ))
			iReason = MSYS_CALLBACK_REASON_NONE;
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// If we do not have an item in hand, start moving it
		if ( gpItemPointer == NULL )
		{
			// Return if empty
			if ( pSoldier->inv[ uiHandPos ].exists() == false )
			{
				return;
			}

			//ATE: Put this here to handle Nails refusal....
			if ( HandleNailsVestFetish( pSoldier, uiHandPos, NOTHING ) )
			{
				return;
			}

			if ( _KeyDown(CTRL) )
			{
				CleanUpStack( &( pSoldier->inv[ uiHandPos ] ), NULL );
			}

			/* CHRISL: For New Inventory system.  Are we removing an existing LBE item?  If so, we need to pull
			all items in the relevant IC Group pockets out of the soldiers inventory and put them into the LBE items
			inventory. But first, find out if we already have a LBE item inventory for this item and this merc.  If we 
			do, remove the items from it and place them into the sector the LBE inventory is located in.*/
			if((UsingNewInventorySystem() == true) && !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
			{
				/*if we pick up a backpack without reactivating the drop pack button, and we have a
				dropkey, reactivate the button*/
				if(uiHandPos == BPACKPOCKPOS)
				{
					// Deal with the zipper before we do anything
					if(pSoldier->flags.ZipperFlag)
						if(!ChangeZipperStatus(pSoldier, FALSE))
							return;
					// Do we still have a linked backpack?  If so, reset droppackflag
					for(unsigned int wi = 0; wi < guiNumWorldItems; wi++)
					{
						if(gWorldItems[wi].soldierID == pSoldier->ubID && gWorldItems[wi].object.exists() == true)
						{
							pSoldier->flags.DropPackFlag = TRUE;
						}
					}
				}
			}

			// remember what it was
			usOldItemIndex = pSoldier->inv[ uiHandPos ].usItem;

			// pick it up
			MAPBeginItemPointer( pSoldier, (UINT8)uiHandPos );

			// remember which gridno the object came from
			sObjectSourceGridNo = pSoldier->sGridNo;

			HandleTacticalEffectsOfEquipmentChange( pSoldier, uiHandPos, usOldItemIndex, NOTHING );

		fInterfacePanelDirty = DIRTYLEVEL2;
			fCharacterInfoPanelDirty = TRUE;
		}
		else	// item in cursor
		{
			// can we pass this part due to booby traps
			if( ContinuePastBoobyTrapInMapScreen( gpItemPointer, pSoldier ) == FALSE )
			{
				return;
			}

			usOldItemIndex = pSoldier->inv[ uiHandPos ].usItem;
			usNewItemIndex = gpItemPointer->usItem;

			//ATE: Put this here to handle Nails refusal....
			if ( HandleNailsVestFetish( pSoldier, uiHandPos, usNewItemIndex ) )
			{
				return;
			}

			if ( _KeyDown(CTRL) )
			{
				CleanUpStack( &( pSoldier->inv[ uiHandPos ] ), gpItemPointer );
				if ( gpItemPointer->exists() == false )
				{
					MAPEndItemPointer( );
				}
				return;
			}

			// !!! ATTACHING/MERGING ITEMS IN MAP SCREEN IS NOT SUPPORTED !!!
			if ( uiHandPos == HANDPOS || uiHandPos == SECONDHANDPOS || uiHandPos == HELMETPOS || uiHandPos == VESTPOS || uiHandPos == LEGPOS )
			{
				if((UsingNewInventorySystem() == true) && !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
				{
				}
				//if ( ValidAttachmentClass( usNewItemIndex, usOldItemIndex ) )
				else if ( ValidAttachment( usNewItemIndex, usOldItemIndex ) )
				{
					// it's an attempt to attach; bring up the inventory panel
					if ( !InItemDescriptionBox( ) )
					{
						MAPInternalInitItemDescriptionBox( &(pSoldier->inv[ uiHandPos ]), 0, pSoldier );
					}
					return;
				}
				else if ( ValidMerge( usNewItemIndex, usOldItemIndex ) )
				{
					// bring up merge requestor
					// TOO PAINFUL TO DO!! --CC
					if ( !InItemDescriptionBox( ) )
					{
						bJustOpenedItemDescPanel = true; // OJW - 20090319 - fix merging on mapscreen - see top of function
						MAPInternalInitItemDescriptionBox( &(pSoldier->inv[ uiHandPos ]), 0, pSoldier );
					}

					/*
					gubHandPos = (UINT8) uiHandPos;
					gusOldItemIndex = usOldItemIndex;
					gusNewItemIndex = usNewItemIndex;
					gfDeductPoints = fDeductPoints;

					DoScreenIndependantMessageBox( Message[ STR_MERGE_ITEMS ], MSG_BOX_FLAG_YESNO, MergeMessageBoxCallBack );
					return;
					*/
				}
				// else handle normally
			}

			if((UsingNewInventorySystem() == true) && !(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))
			{
				if((uiHandPos == VESTPOCKPOS || uiHandPos == LTHIGHPOCKPOS || uiHandPos == RTHIGHPOCKPOS || uiHandPos == CPACKPOCKPOS || uiHandPos == BPACKPOCKPOS) && CanItemFitInPosition(pSoldier, gpItemPointer, uiHandPos, FALSE))
				{
					//If we put a new pack in the backpack pocket, turn off the droppack button
					if(uiHandPos == BPACKPOCKPOS)
					{
						// First, deal with the zipper
						if(pSoldier->flags.ZipperFlag)
							if(!ChangeZipperStatus(pSoldier, FALSE))
								return;
						if(pSoldier->flags.DropPackFlag)
							pSoldier->flags.DropPackFlag = FALSE;
					}
				}
			}

			// Else, try to place here
			if ( !bJustOpenedItemDescPanel ) // OJW - 20090319 - fix merging on mapscreen - see top of function
			{
			if ( PlaceObject( pSoldier, (UINT8)uiHandPos, gpItemPointer ) )
			{

				HandleTacticalEffectsOfEquipmentChange( pSoldier, uiHandPos, usOldItemIndex, usNewItemIndex );

				// Dirty
				fInterfacePanelDirty = DIRTYLEVEL2;
				fCharacterInfoPanelDirty = TRUE;
				fMapPanelDirty = TRUE;

				// Check if cursor is empty now
				if ( gpItemPointer->exists() == false )
				{
					MAPEndItemPointer( );
				}
				else	// items swapped
				{
					// Update mouse cursor
					guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
					gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

					MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
					MSYS_SetCurrentCursor( EXTERN_CURSOR );
					fMapInventoryItem=TRUE;
					fTeamPanelDirty=TRUE;

					// remember which gridno the object came from
					sObjectSourceGridNo = pSoldier->sGridNo;
					// and who owned it last
					gpItemPointerSoldier = pSoldier;

					ReevaluateItemHatches( pSoldier, FALSE );
				}

				// re-evaluate repairs
				gfReEvaluateEveryonesNothingToDo = TRUE;

				// if item came from another merc
				if ( gpItemPointerSoldier != pSoldier )
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_ITEM_PASSED_TO_MERC ], ShortItemNames[ usNewItemIndex ], pSoldier->name );
				}

				}
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		// if there is a map UI message being displayed
		if( giUIMessageOverlay != -1 )
		{
			CancelMapUIMessage( );
			return;
		}

		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;

		// Return if empty
		if (pSoldier->inv[ uiHandPos ].exists() == false )
		{
			return;
		}

		// Some global stuff here - for esc, etc
		// Check for # of slots in item
		// CHRISL: Use new ItemSlotLimit function if we're using the new inventory system
		UINT8 isLimit = ItemSlotLimit(&pSoldier->inv[uiHandPos], uiHandPos, pSoldier);
		if ( ( pSoldier->inv[ uiHandPos ].ubNumberOfObjects > 1 ) && ( isLimit > 0 ) )
		{
			if ( !InItemStackPopup( ) )
			{
				// CHRISL: Changed final parameter so that we fill the inventory screen
				InitItemStackPopup( pSoldier, (UINT8)uiHandPos, 0, INV_REGION_Y, 261, ( SCREEN_HEIGHT - PLAYER_INFO_Y ) );
				fTeamPanelDirty=TRUE;
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}
		else
		{
			if ( !InItemDescriptionBox( ) )
			{
				//CHRISL: This could return a false in which case we don't want to activate the infobox.
				if(InitItemDescriptionBox( pSoldier, (UINT8)uiHandPos, MAP_ITEMDESC_START_X, MAP_ITEMDESC_START_Y, 0 ))
				{
					fShowDescriptionFlag=TRUE;
					fTeamPanelDirty=TRUE;
					fInterfacePanelDirty = DIRTYLEVEL2;
				}
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fRightDown = FALSE;
	}

}


void InternalMAPBeginItemPointer( SOLDIERTYPE *pSoldier )
{
	// If not null return
	if ( gpItemPointer != NULL )
	{
		return;
	}

	// Set global indicator
	gpItemPointer = &gItemPointer;
	gpItemPointerSoldier = pSoldier;

	// Set mouse
	guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
	gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

	MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
	MSYS_SetCurrentCursor( EXTERN_CURSOR );
	fMapInventoryItem=TRUE;
	fTeamPanelDirty=TRUE;

	// hatch out incompatible inventory slots
	ReevaluateItemHatches( pSoldier, FALSE );

	// re-evaluate repairs
	gfReEvaluateEveryonesNothingToDo = TRUE;
}


void MAPBeginItemPointer( SOLDIERTYPE *pSoldier, UINT8 ubHandPos )
{

	// If not null return
	if ( gpItemPointer != NULL )
	{
		return;
	}

	int numToMove = 0;
	if (_KeyDown( SHIFT ))
	{
		// Remove all from soldier's slot
		numToMove = ALL_OBJECTS;
	}
	/* This needs work
	// WDS 08/17/2008 - Cntrl-Click drops all items
	else if (_KeyDown( CTRL ))
	{
		// Remove all from soldier's slot
		numToMove = ALL_OBJECTS;
		pSoldier->inv[ubHandPos].MoveThisObjectTo(gItemPointer, numToMove, pSoldier, ubHandPos);
		if ( gItemPointer.exists() == false )
		{
			//oops, the move failed.  It might have failed because the object was force placed
			//to a slot where the ItemSizeLimit is 0, try again
			//this method won't work with LBEs in LBE pockets
			pSoldier->inv[ubHandPos].MoveThisObjectTo(gItemPointer, numToMove);
		}

		if ( gItemPointer.exists() == true )
		{
			SoldierHandleDropItem (pSoldier);
//			AddItemToPool( pSoldier->sGridNo, &gItemPointer, 1, pSoldier->pathing.bLevel, WORLD_ITEM_REACHABLE, -1 );
//			InternalMAPBeginItemPointer( pSoldier );
		}
		return;
	}
*/
	else
	{
		numToMove = 1;
	}
	pSoldier->inv[ubHandPos].MoveThisObjectTo(gItemPointer, numToMove, pSoldier, ubHandPos);

	if ( gItemPointer.exists() == false )
	{
		//oops, the move failed.  It might have failed because the object was force placed
		//to a slot where the ItemSizeLimit is 0, try again
		//this method won't work with LBEs in LBE pockets
		pSoldier->inv[ubHandPos].MoveThisObjectTo(gItemPointer, numToMove);
	}

	if ( gItemPointer.exists() == true )
	{
		InternalMAPBeginItemPointer( pSoldier );
	}
}


void MAPBeginKeyRingItemPointer( SOLDIERTYPE *pSoldier, UINT8 uiKeySlot )
{
	// If not null return
	if ( gpItemPointer != NULL )
	{
		return;
	}

	// Set mouse
	guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
	gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

	MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
	MSYS_SetCurrentCursor( EXTERN_CURSOR );
	fMapInventoryItem=TRUE;
	fTeamPanelDirty=TRUE;
}

void MAPEndItemPointer( )
{
	if ( gpItemPointer != NULL )
	{
		gpItemPointer = NULL;
		MSYS_ChangeRegionCursor( &gMPanelRegion , CURSOR_NORMAL );
		MSYS_SetCurrentCursor( CURSOR_NORMAL );
		fMapInventoryItem=FALSE;
		fTeamPanelDirty=TRUE;

		if ( fShowMapInventoryPool )
		{
			HandleButtonStatesWhileMapInventoryActive();
		}

		if ( fShowInventoryFlag && bSelectedInfoChar >= 0 )
		{
			ReevaluateItemHatches( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ], FALSE );
		}
	}
}


void HandleMapInventoryCursor()
{
	if(fMapInventoryItem)
	MSYS_SetCurrentCursor( EXTERN_CURSOR );
	return;
}


// will place down the upper left hand corner attribute strings
void RenderAttributeStringsForUpperLeftHandCorner( UINT32 uiBufferToRenderTo )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	if ( ( bSelectedInfoChar != - 1) && ( gCharactersList[ bSelectedInfoChar ].fValid ) )
	{
		pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];
	}

	SetFont( CHAR_FONT );
	SetFontForeground( CHAR_TITLE_FONT_COLOR );
	SetFontBackground( FONT_BLACK );
	SetFontDestBuffer( uiBufferToRenderTo, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );


	// assignment strings
	DrawString( pUpperLeftMapScreenStrings[ 0 ], (UINT16)(220 - StringPixLength( pUpperLeftMapScreenStrings[0], CHAR_FONT)/2), 6, CHAR_FONT);

	// vehicles and robot don't have attributes, contracts, or morale
	if ( ( pSoldier == NULL ) || ( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) ) )
	{
		// health
		DrawString(pUpperLeftMapScreenStrings[ 2 ], 87, 80, CHAR_FONT);

		for( iCounter = 0; iCounter < 5; iCounter++ )
		{
			DrawString(pShortAttributeStrings[ iCounter	], 88, ( INT16 )( 22 + iCounter * 10 ) ,CHAR_FONT );
			DrawString(pShortAttributeStrings[ iCounter + 5 ] ,133, ( INT16 )( 22 + iCounter * 10 ), CHAR_FONT );
		}

		// contract
		//DrawString(pUpperLeftMapScreenStrings[ 1 ], 194, 52,	CHAR_FONT);

		// morale
		DrawString(pUpperLeftMapScreenStrings[ 3 ], 87, 94,	CHAR_FONT);
	}
	else
	{
		// condition
		DrawString(pUpperLeftMapScreenStrings[ 4 ], 87, 80, CHAR_FONT);
	}


	// restore buffer
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}


void DisplayThePotentialPathForCurrentDestinationCharacterForMapScreenInterface(INT16 sMapX, INT16 sMapY )
{
	// simply check if we want to refresh the screen to display path
	static INT8 bOldDestChar = -1;
	static INT16	sPrevMapX, sPrevMapY;
	INT32 iDifference = 0;


	if( bOldDestChar != bSelectedDestChar )
	{
		bOldDestChar = bSelectedDestChar;
	giPotCharPathBaseTime = GetJA2Clock( );

		sPrevMapX = sMapX;
		sPrevMapY = sMapY;
		fTempPathAlreadyDrawn = FALSE;
		fDrawTempPath = FALSE;

	}

	if( ( sMapX != sPrevMapX) || ( sMapY != sPrevMapY ) )
	{
		giPotCharPathBaseTime = GetJA2Clock( );

		sPrevMapX = sMapX;
		sPrevMapY = sMapY;

		// path was plotted and we moved, re draw map..to clean up mess
		if( fTempPathAlreadyDrawn == TRUE )
		{
			fMapPanelDirty = TRUE;
		}

		fTempPathAlreadyDrawn = FALSE;
		fDrawTempPath = FALSE;
	}

	iDifference = GetJA2Clock( ) - giPotCharPathBaseTime ;

	if( fTempPathAlreadyDrawn == TRUE )
	{
		return;
	}

	if( iDifference > MIN_WAIT_TIME_FOR_TEMP_PATH )
	{
		fDrawTempPath = TRUE;
		giPotCharPathBaseTime = GetJA2Clock( );
		fTempPathAlreadyDrawn = TRUE;
	}

	return;
}



void SetUpCursorForStrategicMap( void )
{
	if ( gfInChangeArrivalSectorMode == FALSE )
	{
		// check if character is in destination plotting mode
		if( fPlotForHelicopter == FALSE )
		{
			if( bSelectedDestChar == -1 )
			{
				// no plot mode, reset cursor to normal
				ChangeMapScreenMaskCursor( CURSOR_NORMAL );
			}
			else	// yes - by character
			{
				// set cursor based on foot or vehicle
				if( ( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].bAssignment != VEHICLE ) && !( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].flags.uiStatusFlags & SOLDIER_VEHICLE ) )
				{
					ChangeMapScreenMaskCursor( CURSOR_STRATEGIC_FOOT );
				}
				else
				{
					ChangeMapScreenMaskCursor( CURSOR_STRATEGIC_VEHICLE );
				}
			}
		}
		else	// yes - by helicopter
		{
			// set cursor to chopper
			ChangeMapScreenMaskCursor( CURSOR_CHOPPER );
		}
	}
	else
	{
		// set cursor to bullseye
		ChangeMapScreenMaskCursor( CURSOR_STRATEGIC_BULLSEYE );
	}
}



void HandleAnimatedCursorsForMapScreen( )
{

	if ( COUNTERDONE( CURSORCOUNTER ) )
	{
		RESETCOUNTER( CURSORCOUNTER );
	UpdateAnimatedCursorFrames( gMapScreenMaskRegion.Cursor );
		SetCurrentCursorFromDatabase(	gMapScreenMaskRegion.Cursor	);
	}

}


void AbortMovementPlottingMode( void )
{
	// invalid if we're not plotting movement
	Assert( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) );

	// make everybody go back to where they were going before this plotting session started
	RestorePreviousPaths();

	// don't need the previous paths any more
	ClearPreviousPaths();

	// clear the character's temporary path (this is the route being constantly updated on the map)
	if( pTempCharacterPath )
	{
		// make sure we're at the beginning
		pTempCharacterPath = MoveToBeginningOfPathList( pTempCharacterPath );
		pTempCharacterPath = ClearStrategicPathList( pTempCharacterPath, 0 );
	}

	// clear the helicopter's temporary path (this is the route being constantly updated on the map)
	if( pTempHelicopterPath )
	{
		// make sure we're at the beginning
		pTempHelicopterPath = MoveToBeginningOfPathList( pTempHelicopterPath );
		pTempHelicopterPath = ClearStrategicPathList( pTempHelicopterPath, 0 );
	}


	EndConfirmMapMoveMode( );

	// cancel destination line highlight
	giDestHighLine = -1;

	// cancel movement mode
	bSelectedDestChar = -1;
	fPlotForHelicopter = FALSE;

	// tell player the route was UNCHANGED
	MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 2 ] );


	// reset cursors
	ChangeMapScreenMaskCursor( CURSOR_NORMAL );
	SetUpCursorForStrategicMap( );

	// restore glow region
	RestoreBackgroundForDestinationGlowRegionList( );

	// we might be on the map, redraw to remove old path stuff
	fMapPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;

	gfRenderPBInterface = TRUE;
}



void CheckToSeeIfMouseHasLeftMapRegionDuringPathPlotting(	)
{
	static BOOLEAN fInArea = FALSE;

	if ( ( gMapViewRegion.uiFlags & MSYS_MOUSE_IN_AREA ) == 0 )
	{

		if( fInArea == TRUE )
		{
			fInArea = FALSE;

			// plotting path, clean up
			if( ( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) || ( fDrawTempHeliPath == TRUE ) ) && ( fTempPathAlreadyDrawn == TRUE ) )
			{
				fDrawTempHeliPath = FALSE;
			fMapPanelDirty = TRUE;
				gfRenderPBInterface = TRUE;

				// clear the temp path
				if( pTempCharacterPath )
				{
					pTempCharacterPath = MoveToBeginningOfPathList( pTempCharacterPath);
					pTempCharacterPath = ClearStrategicPathList( pTempCharacterPath, 0 );
				}
			}

			// reset fact temp path has been drawn
			fTempPathAlreadyDrawn = FALSE;
		}
	}
	else
	{
		fInArea = TRUE;
	}

	return;
}





void BlitBackgroundToSaveBuffer( void )
{

	// render map
  RenderMapRegionBackground( );
	
	// Headrock: Moved the following line here from the marker below, so that the Inventory Bottom panel
	// renders underneath the character info panel (and so doesn't truncate the large open inventory). We may
	// need to put TRUE here if there's any problem, but I doubt it.
	RenderMapScreenInterfaceBottom( FALSE );
 
	if( fDisableDueToBattleRoster == FALSE )
	{
		// render team
		RenderTeamRegionBackground( );

		// render character info
		RenderCharacterInfoBackground( );
	}
	else if( gfPreBattleInterfaceActive )
	{
		ForceButtonUnDirty( giMapContractButton );
		ForceButtonUnDirty( giCharInfoButton[ 0 ] );
		ForceButtonUnDirty( giCharInfoButton[ 1 ] );
		RenderPreBattleInterface();
	}

	// now render lower panel
	// HEADROCK - Moved the following line upwards, so it renders BEFORE the character info panel.
	// RenderMapScreenInterfaceBottom( );
}

void CreateMouseRegionsForTeamList( void )
{
	// will create mouse regions for assignments, path plotting, character info selection
	INT16 sCounter = 0;
	INT16 sYAdd = 0;
	// the info region...is the background for the list itself

	// OJW - MP
	int max_rows = giMAXIMUM_NUMBER_OF_PLAYER_SLOTS;
	if (is_networked)
		max_rows = 7; // <TODO> check this value is correct / unhardcode it

	for( sCounter = 0; sCounter < max_rows; sCounter++ )
	{
		sYAdd = 0;
		
		// name region
		MSYS_DefineRegion( &gTeamListNameRegion[ sCounter ] , NAME_X, ( INT16 )( Y_START + ( sCounter ) * ( Y_SIZE + 2 ) + sYAdd ), NAME_X + NAME_WIDTH, ( INT16 )( 145 + ( sCounter + 1 ) * ( Y_SIZE + 2 ) + sYAdd ), MSYS_PRIORITY_NORMAL,
							MSYS_NO_CURSOR, TeamListInfoRegionMvtCallBack, TeamListInfoRegionBtnCallBack );

		// assignment region
		MSYS_DefineRegion( &gTeamListAssignmentRegion[ sCounter ] ,ASSIGN_X , ( INT16 )( Y_START + ( sCounter ) * ( Y_SIZE + 2 ) + sYAdd), ASSIGN_X + ASSIGN_WIDTH, ( INT16 )( 145 + ( sCounter + 1 ) * ( Y_SIZE + 2 ) + sYAdd ), MSYS_PRIORITY_NORMAL + 1,
							MSYS_NO_CURSOR, TeamListAssignmentRegionMvtCallBack, TeamListAssignmentRegionBtnCallBack );

		// location region (same function as name regions, so uses the same callbacks)
		MSYS_DefineRegion( &gTeamListLocationRegion[ sCounter ] , LOC_X, ( INT16 )( Y_START + ( sCounter ) * ( Y_SIZE + 2 ) + sYAdd ), LOC_X + LOC_WIDTH, ( INT16 )( 145 + ( sCounter + 1 ) * ( Y_SIZE + 2 ) + sYAdd ), MSYS_PRIORITY_NORMAL + 1,
							MSYS_NO_CURSOR, TeamListInfoRegionMvtCallBack, TeamListInfoRegionBtnCallBack );

		// destination region
		MSYS_DefineRegion( &gTeamListDestinationRegion[ sCounter ] ,DEST_ETA_X , ( INT16 )( Y_START + ( sCounter ) * ( Y_SIZE + 2 ) + sYAdd ), DEST_ETA_X + DEST_ETA_WIDTH, ( INT16 )( 145 + ( sCounter + 1 ) * ( Y_SIZE + 2 ) + sYAdd ), MSYS_PRIORITY_NORMAL + 1,
							MSYS_NO_CURSOR, TeamListDestinationRegionMvtCallBack, TeamListDestinationRegionBtnCallBack );

		// contract region
		MSYS_DefineRegion( &gTeamListContractRegion[ sCounter ] ,TIME_REMAINING_X , ( INT16 )( Y_START + ( sCounter ) * ( Y_SIZE + 2 ) + sYAdd), TIME_REMAINING_X + TIME_REMAINING_WIDTH, ( INT16 )( 145 + ( sCounter + 1 ) * ( Y_SIZE + 2 ) + sYAdd ), MSYS_PRIORITY_NORMAL + 1,
							MSYS_NO_CURSOR, TeamListContractRegionMvtCallBack, TeamListContractRegionBtnCallBack );

		// contract region
		MSYS_DefineRegion( &gTeamListSleepRegion[ sCounter ] ,SLEEP_X, ( INT16 )( Y_START + ( sCounter ) * ( Y_SIZE + 2 ) + sYAdd), SLEEP_X + SLEEP_WIDTH, ( INT16 )( 145 + ( sCounter + 1 ) * ( Y_SIZE + 2 ) + sYAdd ), MSYS_PRIORITY_NORMAL + 1,
							MSYS_NO_CURSOR, TeamListSleepRegionMvtCallBack, TeamListSleepRegionBtnCallBack );


		MSYS_SetRegionUserData(&gTeamListNameRegion[sCounter],0,sCounter);
		MSYS_SetRegionUserData(&gTeamListAssignmentRegion[sCounter],0,sCounter);
		MSYS_SetRegionUserData(&gTeamListSleepRegion[sCounter],0,sCounter);
		MSYS_SetRegionUserData(&gTeamListLocationRegion[sCounter],0,sCounter);
		MSYS_SetRegionUserData(&gTeamListDestinationRegion[sCounter],0,sCounter);
		MSYS_SetRegionUserData(&gTeamListContractRegion[sCounter],0,sCounter);


		// set up help boxes
		SetRegionFastHelpText( &gTeamListNameRegion[sCounter], pMapScreenMouseRegionHelpText[ 0 ] );
		SetRegionFastHelpText( &gTeamListAssignmentRegion[sCounter], pMapScreenMouseRegionHelpText[ 1 ] );
		SetRegionFastHelpText( &gTeamListSleepRegion[sCounter], pMapScreenMouseRegionHelpText[ 5 ] );
		SetRegionFastHelpText( &gTeamListLocationRegion[sCounter], pMapScreenMouseRegionHelpText[ 4 ] );
		SetRegionFastHelpText( &gTeamListDestinationRegion[sCounter], pMapScreenMouseRegionHelpText[ 2 ] );
		SetRegionFastHelpText( &gTeamListContractRegion[sCounter], pMapScreenMouseRegionHelpText[ 3 ] );
	}

	return;
}


void DestroyMouseRegionsForTeamList( void )
{
	// will destroy mouse regions overlaying the team list area
	INT32 sCounter = 0;

	for( sCounter = 0; sCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; sCounter++ )
	{
	MSYS_RemoveRegion( &gTeamListNameRegion[ sCounter ]);
		MSYS_RemoveRegion( &gTeamListAssignmentRegion[ sCounter ]);
		MSYS_RemoveRegion( &gTeamListSleepRegion[ sCounter ]);
		MSYS_RemoveRegion( &gTeamListDestinationRegion[ sCounter ]);
		MSYS_RemoveRegion( &gTeamListLocationRegion[ sCounter ]);
		MSYS_RemoveRegion( &gTeamListContractRegion[ sCounter ]);
	}
}
void MPReadyButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	int iIndex = MSYS_GetBtnUserData(btn,0);
	if (iIndex >= 1 || iIndex <= 4)
	{
		if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
		{
			btn->uiFlags|=(BUTTON_CLICKED_ON);			
		}
		else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
		{
			if (btn->uiFlags & BUTTON_CLICKED_ON)
			{
				btn->uiFlags &= ~(BUTTON_CLICKED_ON);

				if (iIndex == 3 || iIndex == 4)
				{
					// ready / start button was clicked
					if (is_networked)
					{
						start_battle();
					}
				}
				else if (iIndex == 2)
				{
					// compass button clicked
					if (!is_compass_box_open)
					{
						if (can_edgechange())
						{
							// open the box
							CreateDestroyMouseRegionsForCompassBox();
							ShowBox(ghMPCompassBox);
							is_compass_box_open = true;
						}
						else if (allowlaptop)
						{
							// warn the user that cannot change once buying has commenced
							ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, gszMPMapscreenText[3]);
						}
					}
					else
					{
						// close the box
						CreateDestroyMouseRegionsForCompassBox();
						HideBox(ghMPCompassBox);
						is_compass_box_open = false;
						fTeamPanelDirty = true;
					}

				}
				else if (iIndex == 1)
				{
					// team button clicked
					if (!is_team_box_open)
					{
						if (can_teamchange())
						{
							// open the box
							CreateDestroyMouseRegionsForTeamBox();
							ShowBox(ghMPTeamBox);
							is_team_box_open = true;
						}
						else if (allowlaptop)
						{
							// warn the user that cannot change once buying has commenced
							ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, gszMPMapscreenText[4]);
						}
					}
					else
					{
						// close the box
						CreateDestroyMouseRegionsForTeamBox();
						HideBox(ghMPTeamBox);
						is_team_box_open = false;
						fTeamPanelDirty = true;
					}
				}
			}
		}
	}
	InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
}
void MPCompassChangeCallback( MOUSE_REGION * pReason, INT32 iReason)
{
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (!is_compass_box_open)
		{
			if (can_edgechange())
			{
				// open the box
				CreateDestroyMouseRegionsForCompassBox();
				ShowBox(ghMPCompassBox);
				is_compass_box_open = true;
			}
			else if (allowlaptop)
			{
				// warn the user that cannot change once buying has commenced
				ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, gszMPMapscreenText[3]);
			}
		}
		else
		{
			// close the box
			CreateDestroyMouseRegionsForCompassBox();
			HideBox(ghMPCompassBox);
			is_compass_box_open = false;
			fTeamPanelDirty = true;
		}
	}
}
void MPTeamChangeCallback( MOUSE_REGION * pReason, INT32 iReason)
{
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (!is_team_box_open)
		{
			if (can_teamchange())
			{
				// open the box
				CreateDestroyMouseRegionsForTeamBox();
				ShowBox(ghMPTeamBox);
				is_team_box_open = true;
			}
			else if (allowlaptop)
			{
				// warn the user that cannot change once buying has commenced
				ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, gszMPMapscreenText[4]);
			}
			else if (gRandomStartingEdge)
			{
				ScreenMsg( FONT_LTBLUE, MSG_MPSYSTEM, L"Cannot change edge, the game is set to random spawn");
			}
		}
		else
		{
			// close the box
			CreateDestroyMouseRegionsForTeamBox();
			HideBox(ghMPTeamBox);
			is_team_box_open = false;
			fTeamPanelDirty = true;
		}
	}
}

// OJW - End MP Mouse Regions and Callbacks


// mask for mapscreen region
void MapScreenMarkRegionBtnCallback(MOUSE_REGION *pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
}



void ContractButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( ( iDialogueBox != -1 ) )
	{
		return;
	}

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
		}

/*
		if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
		{
			AbortMovementPlottingMode( );
			return;
		}
*/

		// redraw region
		if( btn->Area.uiFlags & MSYS_HAS_BACKRECT )
		{
			fCharacterInfoPanelDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
		}

	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&= ~(BUTTON_CLICKED_ON);

			RequestContractMenu( );
	}
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
		}
	}
}



void TeamListInfoRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;
	SOLDIERTYPE *pSoldier = NULL;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}
// marke strogg more mercs must add skipped mercs FIRSTmercTOdisplay
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// set to new info character...make sure is valid
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( gCharactersList[ iValue + FIRSTmercTOdisplay].fValid == TRUE )
		{
			// HEADROCK HAM B2.8: Added argument for multi-select entire squads
			if ( HandleCtrlOrShiftInTeamPanel( ( INT8 ) iValue + FIRSTmercTOdisplay, FALSE ))
			{
				return;
			}

			ChangeSelectedInfoChar( ( INT8 ) iValue+ FIRSTmercTOdisplay, TRUE );

			pSoldier = &Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay].usSolID ];

			// highlight
			giDestHighLine = -1;

			// reset character
			bSelectedAssignChar = -1;
			bSelectedDestChar = -1;
			bSelectedContractChar = -1;
			fPlotForHelicopter = FALSE;

			// if not dead or POW, select his sector
			if( ( pSoldier->stats.bLife > 0 ) && ( pSoldier->bAssignment != ASSIGNMENT_POW ) )
			{
				ChangeSelectedMapSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			}

			// unhilight contract line
			giContractHighLine = -1;

			// can't assign highlight line
			giAssignHighLine = -1;

			// dirty team and map regions
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			//fMapScreenBottomDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		else
		{
			// reset selected characters
			ResetAllSelectedCharacterModes( );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( gCharactersList[ iValue + FIRSTmercTOdisplay].fValid == TRUE )
		{
			pSoldier = &Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay].usSolID ];

			// select this character
			ChangeSelectedInfoChar( ( INT8 ) iValue+ FIRSTmercTOdisplay, TRUE );


			RequestToggleMercInventoryPanel();


			// highlight
			giDestHighLine = -1;

			// reset character
			bSelectedAssignChar = -1;
			bSelectedDestChar = -1;
			bSelectedContractChar = -1;
			fPlotForHelicopter = FALSE;

			// if not dead or POW, select his sector
			if( ( pSoldier->stats.bLife > 0 ) && ( pSoldier->bAssignment != ASSIGNMENT_POW ) )
			{
				ChangeSelectedMapSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			}

			// unhilight contract line
			giContractHighLine = -1;

			// can't assign highlight line
			giAssignHighLine = -1;

			// dirty team and map regions
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
//			fMapScreenBottomDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
	}
}


void TeamListInfoRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
// marke strogg more mercs So, the ACTUAL merc is location + skipped FIRSTmercTOdisplay
		if( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			giHighLine = iValue;
		}
		else
		{
			giHighLine = -1;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHighLine = -1;
	}
}



void TeamListAssignmentRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;
	SOLDIERTYPE *pSoldier = NULL;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}
// marke strogg more mercs must add skipped mercs FIRSTmercTOdisplay
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// set to new info character...make sure is valid
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( gCharactersList[ iValue  + FIRSTmercTOdisplay].fValid == TRUE )
		{
			// HEADROCK HAM B2.8: Added argument for multi-select entire squads
			if ( HandleCtrlOrShiftInTeamPanel( ( INT8 ) iValue  + FIRSTmercTOdisplay, FALSE ))
			{
				return;
			}

			// reset list if the clicked character isn't also selected
			ChangeSelectedInfoChar( ( INT8 ) iValue + FIRSTmercTOdisplay, ( BOOLEAN )( IsEntryInSelectedListSet( ( INT8 ) iValue  + FIRSTmercTOdisplay) == FALSE ) );

			pSoldier = &Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay ].usSolID ];

			// if alive (dead guys keep going, use remove menu instead),
			// and it's between sectors and it can be reassigned (non-vehicles)
			if ( ( pSoldier->bAssignment != ASSIGNMENT_DEAD ) && ( pSoldier->stats.bLife > 0 ) && ( pSoldier->flags.fBetweenSectors ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				// can't reassign mercs while between sectors
				DoScreenIndependantMessageBox( pMapErrorString[ 41 ], MSG_BOX_FLAG_OK, NULL );
				return;
			}

			bSelectedAssignChar = ( INT8 ) iValue + FIRSTmercTOdisplay;
			RebuildAssignmentsBox( );

			// reset dest character
			bSelectedDestChar = -1;
			fPlotForHelicopter = FALSE;

			// reset contract char
			bSelectedContractChar = -1;
			giContractHighLine = -1;

			// can't highlight line, anymore..if we were
		giDestHighLine = -1;

			// dirty team and map regions
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			gfRenderPBInterface = TRUE;

			// if this thing can be re-assigned
			if( !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				giAssignHighLine = iValue + FIRSTmercTOdisplay;

				fShowAssignmentMenu = TRUE;

				if( ( pSoldier->stats.bLife == 0 ) || ( pSoldier->bAssignment == ASSIGNMENT_POW ) )
				{
					fShowRemoveMenu = TRUE;
				}
			}
			else
			{
				// can't highlight line
				giAssignHighLine = -1;

				// we can't highlight this line
//				giHighLine = -1;
			}
		}
		else
		{
			// reset selected characters
			ResetAllSelectedCharacterModes( );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// HEADROCK HAM B2.8: Added argument for multi-select entire squads
		if ( HandleCtrlOrShiftInTeamPanel( ( INT8 ) iValue + FIRSTmercTOdisplay, TRUE ))
		{
			return;
		}
		// reset selected characters
		ResetAllSelectedCharacterModes( );
		// HEADROCK HAM B2.8: Right mouse button selects entire squad, if clicked on a squad-assigned character.
				// set to new info character...make sure is valid
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( gCharactersList[ iValue  + FIRSTmercTOdisplay].fValid == TRUE )
		{

			ChangeSelectedInfoChar( ( INT8 ) iValue + FIRSTmercTOdisplay, TRUE );

			pSoldier = &Menptr[ gCharactersList[ iValue  + FIRSTmercTOdisplay].usSolID ];

			// highlight
			giDestHighLine = -1;

			// reset character
			bSelectedAssignChar = -1;
			bSelectedDestChar = -1;
			bSelectedContractChar = -1;
			fPlotForHelicopter = FALSE;

			// if not dead or POW, select his sector
			if( ( pSoldier->stats.bLife > 0 ) && ( pSoldier->bAssignment != ASSIGNMENT_POW ) )
			{
				ChangeSelectedMapSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );
			}

			// Select all characters in squad
			INT16 iCounter;

			for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
			{
				if( gCharactersList[ iCounter ].fValid == TRUE )
				{
					// if not already selected
					if( fSelectedListOfMercsForMapScreen[ iCounter ] == FALSE )
					{
						pSoldier = &( Menptr[ gCharactersList[ iCounter ].usSolID ] );

						// if on a squad or in a vehicle
						if ( ( pSoldier->bAssignment < ON_DUTY ) || ( pSoldier->bAssignment == VEHICLE ) )
						{
							// and a member of that squad or vehicle is selected
							if ( AnyMercInSameSquadOrVehicleIsSelected( pSoldier ) )
							{
								// then also select this guy
								SetEntryInSelectedCharacterList( ( INT8 ) iCounter );
							}
						}
					}
				}
			}

			// unhilight contract line
			giContractHighLine = -1;

			// can't assign highlight line
			giAssignHighLine = -1;

			// dirty team and map regions
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			//fMapScreenBottomDirty = TRUE;
			gfRenderPBInterface = TRUE;
		}
		else
		{
			// reset selected characters
			ResetAllSelectedCharacterModes( );
		}
	}
}


void TeamListAssignmentRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
		// marke strogg more mercs So, the ACTUAL merc is location + FIRSTmercTOdisplay
		if( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			giHighLine = iValue;

			if( !( Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay ].usSolID ].flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				giAssignHighLine = iValue;
			}
			else
			{
				giAssignHighLine = -1;
			}
		}
		else
		{
			// can't highlight line
			giHighLine = -1;
			giAssignHighLine = -1;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHighLine = -1;

		if( bSelectedAssignChar == -1 )
		{
			giAssignHighLine = -1;
		}

		// restore background
		RestoreBackgroundForAssignmentGlowRegionList( );
	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( ( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE ) && 
		!( Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay].usSolID ].flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
			// play click
		PlayGlowRegionSound( );
		}
	}
}



void TeamListDestinationRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive || fShowMapInventoryPool )
	{
		return;
	}
// marke strogg more mercs must add skipped mercs FIRSTmercTOdisplay
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( gCharactersList[ iValue + FIRSTmercTOdisplay].fValid == TRUE )
		{
			// HEADROCK HAM B2.8: Added argument for multi-select entire squads
			if ( HandleCtrlOrShiftInTeamPanel( ( INT8 ) iValue + FIRSTmercTOdisplay , FALSE ))
			{
				return;
			}

			// reset list if the clicked character isn't also selected
			ChangeSelectedInfoChar( ( INT8 ) iValue + FIRSTmercTOdisplay, ( BOOLEAN )( IsEntryInSelectedListSet( ( INT8 ) iValue + FIRSTmercTOdisplay ) == FALSE ) );

			// deselect any characters/vehicles that can't accompany the clicked merc
			DeselectSelectedListMercsWhoCantMoveWithThisGuy( &( Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay ].usSolID ] ) );

			// select all characters/vehicles that MUST accompany the clicked merc (same squad/vehicle)
			SelectUnselectedMercsWhoMustMoveWithThisGuy( );

			// Find out if this guy and everyone travelling with him is allowed to move strategically
			// NOTE: errors are reported within...
			if ( CanChangeDestinationForCharSlot( ( INT8 ) iValue + FIRSTmercTOdisplay, TRUE ) )
			{
				// turn off sector inventory, turn on show teams filter, etc.
				MakeMapModesSuitableForDestPlotting( ( INT8 ) iValue + FIRSTmercTOdisplay );

				// check if person is in a vehicle
				if( Menptr[gCharactersList[ iValue + FIRSTmercTOdisplay ].usSolID].bAssignment == VEHICLE )
				{
					// if he's in the helicopter
					if( Menptr[gCharactersList[ iValue + FIRSTmercTOdisplay ].usSolID].iVehicleId == iHelicopterVehicleId )
					{
						TurnOnAirSpaceMode( );
						if( RequestGiveSkyriderNewDestination( ) == FALSE )
						{
							// not allowed to change destination of the helicopter
							return;
						}
					}
				}


				// select this character as the one plotting strategic movement
				bSelectedDestChar = ( INT8 )iValue + FIRSTmercTOdisplay;

				// remember the current paths for all selected characters so we can restore them if need be
				RememberPreviousPathForAllSelectedChars();

				// check each person in this mvt group, if any bleeding, have them complain
				//CheckMembersOfMvtGroupAndComplainAboutBleeding( &( Menptr[ gCharactersList[ bSelectedDestChar ].usSolID ] ) );

				// highlight
				giDestHighLine = iValue + FIRSTmercTOdisplay;

				// can't assign highlight line
				giAssignHighLine = -1;

				// reset assign character
				bSelectedAssignChar = -1;

				// reset contract char
				bSelectedContractChar = -1;
				giContractHighLine = -1;

				// dirty team and map regions
				fTeamPanelDirty = TRUE;
				fMapPanelDirty = TRUE;
				gfRenderPBInterface = TRUE;


				// set cursor
				SetUpCursorForStrategicMap( );
			}
			else	// problem - this guy can't move
			{
				// cancel destination highlight
				giDestHighLine = -1;
			}
		}
		else	// empty char list slot
		{
			// reset selected characters
			ResetAllSelectedCharacterModes( );
		}
	}


	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		MakeMapModesSuitableForDestPlotting( ( INT8 ) iValue + FIRSTmercTOdisplay );

		// reset list if the clicked character isn't also selected
		ChangeSelectedInfoChar( ( INT8 ) iValue + FIRSTmercTOdisplay, ( BOOLEAN )( IsEntryInSelectedListSet( ( INT8 ) iValue + FIRSTmercTOdisplay ) == FALSE ) );

		CancelPathsOfAllSelectedCharacters();

		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
}


void TeamListDestinationRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = -1;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
		// marke strogg more mercs So, the ACTUAL merc is location + FIRSTmercTOdisplay
		if( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			giHighLine = iValue;
			giDestHighLine = iValue;
		}
		else
		{
			// can't highlight line
			giHighLine = -1;
			giDestHighLine = -1;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHighLine = -1;

		if( bSelectedDestChar == -1 )
		{
		giDestHighLine = -1;
		}

		// restore background
		RestoreBackgroundForDestinationGlowRegionList( );
	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			// play click
			PlayGlowRegionSound( );
		}
	}
}



void TeamListSleepRegionBtnCallBack( MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;
	SOLDIERTYPE *pSoldier = NULL;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}
// marke strogg more mercs must add skipped mercs FIRSTmercTOdisplay
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// set to new info character...make sure is valid.. not in transit and alive and concious

		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( ( gCharactersList[ iValue  + FIRSTmercTOdisplay].fValid == TRUE ) )
		{
			// HEADROCK HAM B2.8: Added argument for multi-select entire squads
			if ( HandleCtrlOrShiftInTeamPanel( ( INT8 ) iValue + FIRSTmercTOdisplay , FALSE ))
			{
				return;
			}

			// reset list if the clicked character isn't also selected
			ChangeSelectedInfoChar( ( INT8 ) iValue + FIRSTmercTOdisplay, ( BOOLEAN )( IsEntryInSelectedListSet( ( INT8 ) iValue + FIRSTmercTOdisplay ) == FALSE ) );

			// if this slot's sleep status can be changed
			if ( CanChangeSleepStatusForCharSlot( (INT8) iValue + FIRSTmercTOdisplay ) )
			{
				pSoldier = &Menptr[ gCharactersList[ iValue + FIRSTmercTOdisplay ].usSolID ];

				if( pSoldier->flags.fMercAsleep == TRUE )
				{
					// try to wake him up
					if( SetMercAwake( pSoldier, TRUE, FALSE ) )
					{
						// HEADROCK HAM B2.8: New feature wakes all trainees/trainers automatically
						HandleTrainingWakeSynchronize( pSoldier );
						// propagate
						HandleSelectedMercsBeingPutAsleep( TRUE, TRUE );
						return;
					}
					else
					{
						HandleSelectedMercsBeingPutAsleep( TRUE, FALSE );
					}
				}
				else	// awake
				{
					// try to put him to sleep
					if( SetMercAsleep( pSoldier, TRUE ) )
					{
						// HEADROCK HAM B2.8: New feature sends all trainees/trainers to sleep automatically
						HandleTrainingSleepSynchronize( pSoldier );
						// propagate
						HandleSelectedMercsBeingPutAsleep( FALSE, TRUE );
						return;
					}
					else
					{
						HandleSelectedMercsBeingPutAsleep( FALSE, FALSE );
					}
				}
			}
		}
		else
		{
			// reset selected characters
			ResetAllSelectedCharacterModes( );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
}


void TeamListSleepRegionMvtCallBack( MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = -1;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
		// marke strogg more mercs So, the ACTUAL merc is location + FIRSTmercTOdisplay
		if( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			giHighLine = iValue;

			if ( CanChangeSleepStatusForCharSlot( (INT8) iValue + FIRSTmercTOdisplay ) )
			{
				giSleepHighLine = iValue;
			}
			else
			{
				giSleepHighLine = -1;
			}
		}
		else
		{
			// can't highlight line
			giHighLine = -1;
			giSleepHighLine = -1;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHighLine = -1;
		giSleepHighLine = -1;

		// restore background
		RestoreBackgroundForSleepGlowRegionList( );
	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if ( CanChangeSleepStatusForCharSlot( (INT8) iValue + FIRSTmercTOdisplay ) )
		{
			// play click
			PlayGlowRegionSound( );
		}
	}
}



void TeamListContractRegionBtnCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}
// marke strogg more mercs must add skipped mercs FIRSTmercTOdisplay
	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if( gCharactersList[ iValue ].fValid == TRUE )
	{
		if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
		{
			// select ONLY this dude
			ChangeSelectedInfoChar( ( INT8 ) iValue + FIRSTmercTOdisplay, TRUE );

			// reset character
			giDestHighLine = -1;
			bSelectedAssignChar = -1;
			bSelectedDestChar = -1;
			bSelectedContractChar = -1;
			fPlotForHelicopter = FALSE;

			fTeamPanelDirty = TRUE;
		}

		ContractRegionBtnCallback( pRegion, iReason );
	}

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
}


void TeamListContractRegionMvtCallBack(MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = -1;


	if( fLockOutMapScreenInterface || gfPreBattleInterfaceActive )
	{
		return;
	}

	iValue = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_MOVE)
	{
		// marke strogg more mercs So, the ACTUAL merc is location + FIRSTmercTOdisplay
		if( gCharactersList[ iValue + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			giHighLine = iValue;

			if( CanExtendContractForCharSlot( (INT8) iValue + FIRSTmercTOdisplay ) )
			{
				giContractHighLine = iValue;
			}
			else
			{
				giContractHighLine = -1;
			}
		}
		else
		{
			// can't highlight line
			giHighLine = -1;
			giContractHighLine = -1;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		giHighLine = -1;

		// no longer valid char?...reset display of highlight boxes
		if( fShowContractMenu == FALSE )
		{
			giContractHighLine	= -1;
		}

		// restore background
		RestoreBackgroundForContractGlowRegionList( );
	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( CanExtendContractForCharSlot( (INT8) iValue + FIRSTmercTOdisplay ) )
		{
			// play click
			PlayGlowRegionSound( );
		}
	}
}



INT32 GetIndexForthis( SOLDIERTYPE *pSoldier )
{
	INT32 iLastGuy;
	INT32 iIndex = 0;
	INT32 iCounter = 0;

	// get the index into the characters list for this soldier type
	iLastGuy = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	for( iCounter = 0; iCounter < iLastGuy; iCounter++ )
	{
		if( gCharactersList[ iCounter ].fValid == TRUE )
		{
			if( ( &Menptr[ gCharactersList[ iCounter ].usSolID ] ) == pSoldier )
			{
				iIndex = iCounter;
				iCounter = iLastGuy;
			}
		}
	}
	return( iIndex );
}



BOOLEAN IsCursorWithInRegion(INT16 sLeft, INT16 sRight, INT16 sTop, INT16 sBottom )
{
	POINT MousePos;

	// get cursor position
	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	// is it within region?

	if( ( sLeft < MousePos.x ) && ( sRight > MousePos.x) && (sTop < MousePos.y ) && ( sBottom > MousePos.y ) )
	{
		return ( TRUE );
	}
	else
	{
		return ( FALSE );
	}
}


void HandleHighLightingOfLinesInTeamPanel( void )
{
	if ( fShowInventoryFlag )
	{
		return;
	}

	// will highlight or restore backgrounds to highlighted lines

	// restore backgrounds, if need be
	RestoreBackgroundForAssignmentGlowRegionList( );
	RestoreBackgroundForDestinationGlowRegionList( );
	RestoreBackgroundForContractGlowRegionList( );
	RestoreBackgroundForSleepGlowRegionList( );

	HighLightAssignLine( /*(UINT16)giAssignHighLine*/ ); // doesn't take parameters (jonathanl)
	HighLightDestLine(	/*(UINT16)giDestHighLine	*/); // doesn't take parameters (jonathanl)
	HighLightSleepLine( /*(UINT16)giSleepHighLine */); // doesn't take parameters (jonathanl)

	// contracts?
	if( giContractHighLine != -1 )
	{
	ContractListRegionBoxGlow( ( UINT16 ) giContractHighLine );
	}
}


void PlotPermanentPaths( void )
{
	if( fPlotForHelicopter == TRUE )
	{
		DisplayHelicopterPath( );
	}
	else if( bSelectedDestChar != -1 )
	{
	DisplaySoldierPath( &Menptr[ gCharactersList[ bSelectedDestChar ].usSolID ] );
	}
}


void PlotTemporaryPaths( void )
{
	INT16 sMapX, sMapY;


	// check to see if we have in fact moved are are plotting a path?
	if ( GetMouseMapXY( &sMapX, &sMapY ) )
	{
		if ( fPlotForHelicopter == TRUE )
		{
			Assert( fShowAircraftFlag == TRUE );
/*
			if( fZoomFlag )
			{
				sMapX =	( INT16 )( ( ( iZoomX ) / ( WORLD_MAP_X ) ) + sMapX );
				sMapX /= 2;

				sMapY =	( INT16 )( ( ( iZoomY ) / ( WORLD_MAP_X ) ) + sMapY );
				sMapY /= 2;
			}
*/

			// plot temp path
			PlotATemporaryPathForHelicopter( sMapX, sMapY);

			// check if potential path is allowed
			DisplayThePotentialPathForHelicopter( sMapX, sMapY );

			if( fDrawTempHeliPath == TRUE )
			{
				// clip region
				ClipBlitsToMapViewRegion( );
				// display heli temp path
				DisplayHelicopterTempPath( );
				//restore
				RestoreClipRegionToFullScreen( );
			}
		}
		else
		// dest char has been selected,
		if( bSelectedDestChar != -1 )
		{
/*
			if( fZoomFlag )
			{
				sMapX =	( INT16 )( ( ( iZoomX ) / ( MAP_GRID_X ) ) + sMapX );
				sMapX /= 2;

				sMapY =	( INT16 )( ( ( iZoomY ) / ( MAP_GRID_Y ) ) + sMapY );
				sMapY /= 2;
			}
*/

			PlotATemporaryPathForCharacter( &Menptr[ gCharactersList[ bSelectedDestChar ].usSolID ], sMapX, sMapY );

			// check to see if we are drawing path
		DisplayThePotentialPathForCurrentDestinationCharacterForMapScreenInterface( sMapX, sMapY );

			// if we need to draw path, do it
			if( fDrawTempPath == TRUE )
			{
				// clip region
				ClipBlitsToMapViewRegion( );
				// blit
				DisplaySoldierTempPath( &Menptr[ gCharactersList[ bSelectedDestChar ].usSolID ] );
				// restore
				RestoreClipRegionToFullScreen( );
			}
		}
	}
}



void RenderMapRegionBackground( void )
{
	// renders to save buffer when dirty flag set

	if( fMapPanelDirty == FALSE )
	{
		gfMapPanelWasRedrawn = FALSE;

		// not dirty, leave
		return;
	}

	// don't bother if showing sector inventory instead of the map!!!
	if( !fShowMapInventoryPool )
	{
		// draw map
		DrawMap( );
	}


	// blit in border
	RenderMapBorder( );

	if ( ghAttributeBox != - 1 )
	{
		ForceUpDateOfBox( ghAttributeBox );
	}

	// HEADROCK HAM 3.6: Force re-render due to Facility box
	if ( ghFacilityAssignmentBox != 1 )
	{
		ForceUpDateOfBox( ghFacilityAssignmentBox );
	}

	if ( ghTownMineBox != - 1 )
	{
		// force update of town mine info boxes
		ForceUpDateOfBox( ghTownMineBox );
	}


	MapscreenMarkButtonsDirty();

	//RestoreExternBackgroundRect( 261, 0, 379, 359 );

	RestoreExternBackgroundRect( 261, 0, SCREEN_WIDTH - 261, SCREEN_HEIGHT - 121 );

	// don't bother if showing sector inventory instead of the map!!!
	if( !fShowMapInventoryPool )
	{
		// if Skyrider can and wants to talk to us
		if( IsHelicopterPilotAvailable( ) )
		{
			// see if Skyrider has anything new to tell us
			CheckAndHandleSkyriderMonologues( );
		}
	}

	// reset dirty flag
	fMapPanelDirty = FALSE;

	gfMapPanelWasRedrawn = TRUE;

	return;
}


void RenderTeamRegionBackground( void )
{
	HVOBJECT hHandle;


	// renders to save buffer when dirty flag set
	if( fTeamPanelDirty == FALSE )
	{
		// not dirty, leave
		return;
	}

	// show inventory or the team list?
	if(fShowInventoryFlag == FALSE )
	{
		GetVideoObject(&hHandle, guiCHARLIST);
		BltVideoObject( guiSAVEBUFFER , hHandle, 0,PLAYER_INFO_X, PLAYER_INFO_Y , VO_BLT_SRCTRANSPARENCY,NULL );
	}
	else
	{
		BltCharInvPanel();
	}


	if ( !fShowInventoryFlag )
	{
		// if we are not in inventory mode, show character list
		HandleHighLightingOfLinesInTeamPanel( );

		DisplayCharacterList();
	}

	fDrawCharacterList = FALSE;


	// display arrows by selected people
	HandleDisplayOfSelectedMercArrows( );
	DisplayIconsForMercsAsleep(	);


	// reset dirty flag
	fTeamPanelDirty = FALSE;
	gfRenderPBInterface = TRUE;

	// mark all pop ups as dirty
	MarkAllBoxesAsAltered( );

	// restore background for area
	if(fShowInventoryFlag == TRUE && UsingNewInventorySystem() == false){
		RestoreExternBackgroundRect( 0, 107, 261, SCREEN_HEIGHT - 106 - 121 );
	}
	else if(fShowInventoryFlag == TRUE && iResolution == 0){
		RestoreExternBackgroundRect( 0, 107, 261, SCREEN_HEIGHT - 107 );
	}
	else if(fShowInventoryFlag == TRUE && iResolution == 1){
		RestoreExternBackgroundRect( 0, 107, 261, SCREEN_HEIGHT - 107 );
	}
	else if(fShowInventoryFlag == FALSE || iResolution == 2){
		RestoreExternBackgroundRect( 0, 107, 261, SCREEN_HEIGHT - 107 - 121 );
	}
	MapscreenMarkButtonsDirty();

	return;
}


void RenderCharacterInfoBackground( void )
{
	HVOBJECT hHandle;


	// will render the background for the character info panel

	if( fCharacterInfoPanelDirty == FALSE )
	{
		// not dirty, leave
		return;
	}


	// the upleft hand corner character info panel
	GetVideoObject(&hHandle, guiCHARINFO);
	BltVideoObject( guiSAVEBUFFER , hHandle, 0,TOWN_INFO_X, TOWN_INFO_Y , VO_BLT_SRCTRANSPARENCY,NULL );

	UpdateHelpTextForMapScreenMercIcons( );

	if( ( bSelectedInfoChar != -1) && ( fDisableDueToBattleRoster == FALSE ) )
	{
		// valid char to display
		DisplayCharacterInfo();
	}

	if( ( fDisableDueToBattleRoster == FALSE ) )
	{
		// draw attributes
		RenderAttributeStringsForUpperLeftHandCorner( guiSAVEBUFFER );
	}

	// reset dirty flag
	fCharacterInfoPanelDirty = FALSE;

	// redraw face
	fReDrawFace = TRUE;

	MapscreenMarkButtonsDirty();

	// mark all pop ups as dirty
	MarkAllBoxesAsAltered( );

	// restore background for area
	RestoreExternBackgroundRect( 0, 0, 261, 107 );

}

void DetermineIfContractMenuCanBeShown( void )
{
	if( fShowContractMenu == FALSE )
	{

		// destroy menus for contract region
		CreateDestroyMouseRegionsForContractMenu( );

			// hide all boxes
		HideBox( ghContractBox );

		// make sure, absolutly sure we want to hide this box
		if( fShowAssignmentMenu == FALSE )
		{
			HideBox( ghRemoveMercAssignBox );
		}


		return;
	}

	// create mask, if needed
	CreateDestroyScreenMaskForAssignmentAndContractMenus( );

	// create mouse regions for contract region
	CreateDestroyMouseRegionsForContractMenu( );

	// determine which lines selectable
	HandleShadingOfLinesForContractMenu( );

	if( Menptr[gCharactersList[ bSelectedInfoChar ].usSolID].stats.bLife == 0 )
	{
		// show basic assignment menu
		ShowBox( ghRemoveMercAssignBox );
	}
	else
	{
		// show basic contract menu
		ShowBox( ghContractBox );
	}
}


void CheckIfPlottingForCharacterWhileAirCraft( void )
{
	// if we are in aircraft mode and plotting for character, reset plotting character

	if( fShowAircraftFlag == TRUE )
	{
		// if plotting, but not for heli
		if ( ( bSelectedDestChar != -1 ) && ( fPlotForHelicopter == FALSE ) )
		{
			// abort
			AbortMovementPlottingMode();
		}
	}
	else	// not in airspace mode
	{
		if( fPlotForHelicopter == TRUE )
		{
			// abort
			AbortMovementPlottingMode();
		}
	}
}


void ContractRegionBtnCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	SOLDIERTYPE *pSoldier = NULL;


	// btn callback handler for contract region

	if( ( iDialogueBox != -1 ) )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{

		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( CanExtendContractForCharSlot( bSelectedInfoChar ) )
		{
			pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];

			// create
			RebuildContractBoxForMerc( pSoldier );

			// reset selected characters
			ResetAllSelectedCharacterModes( );

			bSelectedContractChar = bSelectedInfoChar;
			giContractHighLine = bSelectedContractChar;


			// if not triggered internally
			if ( CheckIfSalaryIncreasedAndSayQuote( pSoldier, TRUE ) == FALSE )
			{
				// show contract box
				fShowContractMenu = TRUE;

				// stop any active dialogue
				StopAnyCurrentlyTalkingSpeech( );
			}

			//fCharacterInfoPanelDirty = TRUE;
		}
		else
		{
			// reset selected characters
			ResetAllSelectedCharacterModes( );
		}
	}

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
}


void ContractRegionMvtCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	// mvt callback handler for contract region
	if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		if( fGlowContractRegion == TRUE )
		{
			// not showing box and lost mouse?..stop glowing
			if( fShowContractMenu == FALSE )
			{
		 fGlowContractRegion = FALSE;
			fCharacterInfoPanelDirty = TRUE;
			giContractHighLine = -1;

			// reset glow
			fResetContractGlow = TRUE;
			}
		}

	}
	else if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( bSelectedInfoChar != -1 )
		{
			if( gCharactersList[ bSelectedInfoChar ].fValid == TRUE )
			{
				if( fShowContractMenu == FALSE )
				{
					// glow region
					fGlowContractRegion = TRUE;

					giContractHighLine = bSelectedInfoChar;

					PlayGlowRegionSound( );
				}
			}
		}
	}
}



void HandleShadingOfLinesForContractMenu( void )
{
	SOLDIERTYPE *pSoldier;
	MERCPROFILESTRUCT *pProfile;


	if( ( fShowContractMenu == FALSE ) || ( ghContractBox == - 1 ) )
	{
		return;
	}

	// error check, return if not a valid character
	if( bSelectedContractChar == -1 )
	{
		return;
	}

	// check if this character is a valid character
	if( gCharactersList[ bSelectedContractChar ].fValid == FALSE )
	{
		return;
	}

	Assert( CanExtendContractForCharSlot( bSelectedContractChar ) );

	// grab the character
	pSoldier = &Menptr[ gCharactersList[ bSelectedContractChar ].usSolID ];


	// is guy in AIM? and well enough to talk and make such decisions?
	if( ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC ) && ( pSoldier->stats.bLife >= OKLIFE ) )
	{
		pProfile = &( gMercProfiles[ pSoldier->ubProfile ] );

		if (is_networked)
		{
			// WANNE - MP: Shade all contract renewals, because it is not needed in a MP game
			ShadeStringInBox( ghContractBox, CONTRACT_MENU_DAY );
			ShadeStringInBox( ghContractBox, CONTRACT_MENU_WEEK );
			ShadeStringInBox( ghContractBox, CONTRACT_MENU_TWO_WEEKS );
		}
		else
		{
			// one day
			if( pProfile->sSalary > LaptopSaveInfo.iCurrentBalance )
			{
				ShadeStringInBox( ghContractBox, CONTRACT_MENU_DAY );
			}
			else
			{
				UnShadeStringInBox( ghContractBox, CONTRACT_MENU_DAY );
			}

			// one week
			if( ( INT32 )( pProfile->uiWeeklySalary ) > LaptopSaveInfo.iCurrentBalance )
			{
				ShadeStringInBox( ghContractBox, CONTRACT_MENU_WEEK );
			}
			else
			{
				UnShadeStringInBox( ghContractBox, CONTRACT_MENU_WEEK );
			}

			// two weeks
			if( ( INT32 )( pProfile->uiBiWeeklySalary ) > LaptopSaveInfo.iCurrentBalance )
			{
				ShadeStringInBox( ghContractBox, CONTRACT_MENU_TWO_WEEKS );
			}
			else
			{
				UnShadeStringInBox( ghContractBox, CONTRACT_MENU_TWO_WEEKS );
			}
		}
	}
	else
	{
		// can't extend contract duration
		ShadeStringInBox( ghContractBox, CONTRACT_MENU_DAY );
		ShadeStringInBox( ghContractBox, CONTRACT_MENU_WEEK );
		ShadeStringInBox( ghContractBox, CONTRACT_MENU_TWO_WEEKS );
	}

	// if THIS soldier is involved in a fight (dismissing in a hostile sector IS ok...)
	if( ( gTacticalStatus.uiFlags & INCOMBAT ) && pSoldier->bInSector )
	{
		ShadeStringInBox( ghContractBox, CONTRACT_MENU_TERMINATE );
	}
	else
	{
		UnShadeStringInBox( ghContractBox, CONTRACT_MENU_TERMINATE );
	}

	// WANNE - MP: Disable "Dismiss" button when Random merc was selected, because we cannot hire any new merc
	if (is_networked && gRandomMercs)
	{
		ShadeStringInBox( ghContractBox, CONTRACT_MENU_TERMINATE );
	}
}



void ReBuildCharactersList( void )
{
	// rebuild character's list
	INT16 sCount = 0;


	// add in characters
	for ( sCount = 0; sCount < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; sCount++)
	{
		// clear this slot
		gCharactersList[ sCount ].fValid = FALSE;
		gCharactersList[ sCount ].usSolID = 0;
	}

	// fills array with pressence of player controlled characters
	for ( INT32 cnt=gTacticalStatus.Team[ OUR_TEAM ].bFirstID; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++)
	{
		if(Menptr[ cnt ].bActive == 1)
		{
			AddCharacter( &Menptr[ cnt ] );
		}
	}

	// sort them according to current sorting method
	SortListOfMercsInTeamPanel( FALSE );


	// if nobody is selected, or the selected merc has somehow become invalid
	if ( ( bSelectedInfoChar == -1 ) || ( !gCharactersList[ bSelectedInfoChar ].fValid ) )
	{
		// is the first character in the list valid?
		if ( gCharactersList[ 0 ].fValid )
		{
			// select him
			ChangeSelectedInfoChar( 0, TRUE );
		}
		else
		{
			// select no one
			ChangeSelectedInfoChar( -1, TRUE );
		}
	}

	// exit inventory mode
	fShowInventoryFlag = FALSE;
	// switch hand region help text to "Enter Inventory"
	SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );
}


void HandleChangeOfInfoChar( void )
{
	static INT8 bOldInfoChar = -1;

	if( bSelectedInfoChar != bOldInfoChar )
	{
		// set auto faces inactive

		// valid character?
		if( bOldInfoChar != -1 )
		{
			if( gCharactersList[ bOldInfoChar ].fValid == TRUE )
			{
				// set face in active
				SetAutoFaceInActiveFromSoldier( Menptr[ gCharactersList[ bOldInfoChar ].usSolID ].ubID );
			}
		}

		// stop showing contract box
		//fShowContractMenu = FALSE;

		// update old info char value
		bOldInfoChar = bSelectedInfoChar;
	}
}


void RebuildContractBoxForMerc( SOLDIERTYPE *pCharacter )
{
	// rebuild contractbox for this merc
	RemoveBox( ghContractBox );
	ghContractBox = -1;

	// recreate
	CreateContractBox( pCharacter );

	return;
}


void TestMessageSystem( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < 300; iCounter++ )
	{
	MapScreenMessage( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"%d", iCounter );
	}
	MapScreenMessage( FONT_MCOLOR_DKRED, MSG_INTERFACE, L"%d", iCounter );

	return;
}



void EnableDisableTeamListRegionsAndHelpText( void )
{
	// check if valid character here, if so, then do nothing..other wise set help text timer to a gazillion
	INT8 bCharNum;


	for( bCharNum = 0; bCharNum < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; bCharNum++ )
	{ // marke strogg more mercs limit regions to actual mercs + only do it for 18 slots
		if( gCharactersList[ bCharNum + FIRSTmercTOdisplay].fValid == FALSE || bCharNum >= maxNumberOfMercVisibleInStrategyList )
		{
			// disable regions in all team list columns
			MSYS_DisableRegion( &gTeamListNameRegion[ bCharNum ] );
			MSYS_DisableRegion( &gTeamListAssignmentRegion[ bCharNum ] );
			MSYS_DisableRegion( &gTeamListLocationRegion[ bCharNum ] );
			MSYS_DisableRegion( &gTeamListSleepRegion[ bCharNum ] );
			MSYS_DisableRegion( &gTeamListDestinationRegion[ bCharNum ] );
			MSYS_DisableRegion( &gTeamListContractRegion[ bCharNum ] );
		}
		else
		{
			// always enable Name and Location regions
			MSYS_EnableRegion( &gTeamListNameRegion[ bCharNum ] );
			MSYS_EnableRegion( &gTeamListLocationRegion[ bCharNum ] );

			// valid character.	If it's a vehicle, however
			if ( Menptr[ gCharactersList[ bCharNum + FIRSTmercTOdisplay].usSolID ].flags.uiStatusFlags & SOLDIER_VEHICLE )
			{
				// Can't change assignment for vehicles
				MSYS_DisableRegion( &gTeamListAssignmentRegion[ bCharNum ] );
			}
			else
			{
				MSYS_EnableRegion( &gTeamListAssignmentRegion[ bCharNum ] );

				// POW or dead ?
				if ( ( Menptr[ gCharactersList[ bCharNum + FIRSTmercTOdisplay].usSolID ].bAssignment == ASSIGNMENT_POW ) ||
						( Menptr[ gCharactersList[ bCharNum + FIRSTmercTOdisplay].usSolID ].stats.bLife == 0 ) )
				{
					// "Remove Merc"
					SetRegionFastHelpText( &gTeamListAssignmentRegion[ bCharNum ], pRemoveMercStrings[ 0 ] );

					SetRegionFastHelpText( &gTeamListDestinationRegion[ bCharNum ], L"" );
				}
				else
				{
					// "Assign Merc"
					SetRegionFastHelpText( &gTeamListAssignmentRegion[ bCharNum ], pMapScreenMouseRegionHelpText[ 1 ] );
					// "Plot Travel Route"
					SetRegionFastHelpText( &gTeamListDestinationRegion[ bCharNum ], pMapScreenMouseRegionHelpText[ 2 ] );
				}
			}


			if ( CanExtendContractForCharSlot( bCharNum + FIRSTmercTOdisplay ) ) // marke strogg more mercs
			{
				MSYS_EnableRegion( &gTeamListContractRegion[ bCharNum ] );
			}
			else
			{
				MSYS_DisableRegion( &gTeamListContractRegion[ bCharNum ] );
			}


			if ( CanChangeSleepStatusForCharSlot( bCharNum + FIRSTmercTOdisplay  ) )
			{
				MSYS_EnableRegion( &gTeamListSleepRegion[ bCharNum ] );
			}
			else
			{
				MSYS_DisableRegion( &gTeamListSleepRegion[ bCharNum ] );
			}

			// destination region is always enabled for all valid character slots.
			// if the character can't move at this time, then the region handler must be able to tell the player why not
			MSYS_EnableRegion( &gTeamListDestinationRegion[ bCharNum ] );
		}
	}
}


void ResetAllSelectedCharacterModes( void )
{
	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );
		return;
	}

	// if in militia redistribution popup
	if ( sSelectedMilitiaTown != 0 )
	{
		sSelectedMilitiaTown = 0;
	}


	// cancel destination line highlight
 	giDestHighLine = -1;

	// cancel assign line highlight
	giAssignHighLine = -1;

	// unhilight contract line
	giContractHighLine = -1;


	// if we were plotting movement
	if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
	{
		AbortMovementPlottingMode();
	}

	// reset assign character
	bSelectedAssignChar = -1;

	// reset contract character
	bSelectedContractChar = -1;


	// reset map cursor to normal
	if ( !gfFadeOutDone && !gfFadeIn )
	{
	SetUpCursorForStrategicMap( );
	}

	return;
}



void UpdatePausedStatesDueToTimeCompression( void )
{
	// this executes every frame, so keep it optimized for speed!

	// if time is being compressed
	if( IsTimeBeingCompressed() )
	{
		// but it shouldn't be
		if ( !AllowedToTimeCompress( ) )
		{
			// pause game to (temporarily) stop time compression
			PauseGame( );
		}
	}
	else	// time is NOT being compressed
	{
		// but the player would like it to be compressing
		if ( IsTimeCompressionOn() && !gfPauseDueToPlayerGamePause )
		{
			// so check if it's legal to start time compressing again
			if ( AllowedToTimeCompress( ) )
			{
				// unpause game to restart time compresssion
				UnPauseGame( );
			}
		}
	}

	return;
}


BOOLEAN ContinueDialogue(SOLDIERTYPE *pSoldier, BOOLEAN fDone )
{
	// continue this grunts dialogue, restore when done
	static INT8 bOldSelectedInfoChar = -1;
	static BOOLEAN fTalkingingGuy = FALSE;

	INT8 bCounter = 0;




	if( fDone == TRUE )
	{
		if( fTalkingingGuy == TRUE )
		{
			/*
			// done, restore
			if( bOldSelectedInfoChar != -1 )
			{
				ChangeSelectedInfoChar( bOldSelectedInfoChar, TRUE );

				SetAutoFaceInActive( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ]->iFaceIndex );
			}

	*/
			fCharacterInfoPanelDirty = TRUE;
			fTalkingingGuy = FALSE;
		}

		return( TRUE );
	}

	// check if valid character talking?
	if( pSoldier == NULL )
	{
		return FALSE;
	}

	// otherwise, find this character
	for( bCounter = 0; bCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; bCounter++ )
	{
		if( gCharactersList[bCounter].fValid == TRUE )
		{
			if( ( &Menptr[gCharactersList[bCounter].usSolID]) == pSoldier )
			{
				if( bSelectedInfoChar != bCounter )
				{
					bOldSelectedInfoChar = bSelectedInfoChar;
					ChangeSelectedInfoChar( bCounter, TRUE );
				}
				fTalkingingGuy = TRUE;
				return ( FALSE );
			}
		}
	}

	return ( FALSE );
}


void HandleSpontanousTalking(	)
{
	// simply polls if the talking guy is done, if so...send an end command to continue dialogue

	if( DialogueActive() == FALSE )
	{
		if( ( bSelectedInfoChar != -1 ) && ( bSelectedInfoChar < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS ) )
		{
			ContinueDialogue( ( &Menptr[gCharactersList[bSelectedInfoChar].usSolID] ), TRUE );
		}
	}

	return;
}


BOOLEAN CheckIfClickOnLastSectorInPath( INT16 sX, INT16 sY )
{
	PathStPtr *ppMovePath = NULL;
	BOOLEAN fLastSectorInPath = FALSE;
	INT32 iVehicleId = -1;
	PathStPtr pPreviousMercPath = NULL;


	// see if we have clicked on the last sector in the characters path

	// check if helicopter
	if( fPlotForHelicopter == TRUE )
	{
		if( sX + ( sY * MAP_WORLD_X ) == GetLastSectorOfHelicoptersPath( ) )
		{
			// helicopter route confirmed - take off
			TakeOffHelicopter( );

			// rebuild waypoints - helicopter
			ppMovePath = &( pVehicleList[ iHelicopterVehicleId ].pMercPath );
			RebuildWayPointsForGroupPath( *ppMovePath, pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );

			// pointer to previous helicopter path
			pPreviousMercPath = gpHelicopterPreviousMercPath;

			fLastSectorInPath = TRUE;
		}
	}
	else	// not doing helicopter movement
	{
		// if not doing a soldier either, we shouldn't be here!
		if( bSelectedDestChar == -1 )
		{
			return( FALSE );
		}

		// invalid soldier?	we shouldn't be here!
		if( gCharactersList[ bSelectedDestChar ].fValid == FALSE )
		{
			bSelectedDestChar = -1;
			return( FALSE );
		}

		if( sX + ( sY * MAP_WORLD_X ) == GetLastSectorIdInCharactersPath( ( &Menptr[ gCharactersList[ bSelectedDestChar ].usSolID ] ) ) )
		{
			// clicked on last sector, reset plotting mode

			// if he's IN a vehicle or IS a vehicle
			if( ( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].bAssignment == VEHICLE ) || ( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				if( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].bAssignment == VEHICLE )
				{
					// IN a vehicle
					iVehicleId = Menptr[gCharactersList[ bSelectedDestChar ].usSolID].iVehicleId;
				}
				else
				{
					// IS a vehicle
					iVehicleId = Menptr[gCharactersList[ bSelectedDestChar ].usSolID].bVehicleID;
				}

				// rebuild waypoints - vehicles
				ppMovePath = &( pVehicleList[ iVehicleId ].pMercPath );
			}
			else
			{
				// rebuild waypoints - mercs on foot
				ppMovePath = &( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].pMercPath );
			}

			RebuildWayPointsForAllSelectedCharsGroups( );

			// pointer to previous character path
			pPreviousMercPath = gpCharacterPreviousMercPath[ bSelectedDestChar ];

			fLastSectorInPath = TRUE;
		}
	}


	// if the click was over the last sector
	if ( fLastSectorInPath )
	{
		// route has been confirmed
		EndConfirmMapMoveMode( );

		// if we really did plot a path (this will skip message if left click on current sector with no path)
		if ( GetLengthOfPath( *ppMovePath ) > 0 )
		{
			// then verbally confirm this destination!
			HandleNewDestConfirmation( sX, sY );
		}
		else	// NULL path confirmed
		{
			// if previously there was a path
			if ( pPreviousMercPath != NULL )
			{
				// then this means we've CANCELED it
				MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 3 ] );
			}
			else	// no previous path
			{
				// then it means the route was UNCHANGED
				MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 2 ] );
			}
		}
	}


	return( fLastSectorInPath );
}


void RebuildWayPointsForAllSelectedCharsGroups( void )
{
	// rebuild the waypoints for everyone in the selected character list
	INT32 iCounter = 0;
	BOOLEAN fGroupIDRebuilt[ 256 ];
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iVehicleId;
	PathStPtr *ppMovePath = NULL;
	UINT8 ubGroupId;


	memset( fGroupIDRebuilt, FALSE, sizeof( fGroupIDRebuilt ) );

	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( ( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE ) )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];


			// if he's IN a vehicle or IS a vehicle
			if( ( pSoldier->bAssignment == VEHICLE ) || ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
			{
				if( pSoldier->bAssignment == VEHICLE )
				{
					// IN a vehicle
					iVehicleId = pSoldier->iVehicleId;
				}
				else
				{
					// IS a vehicle
					iVehicleId = pSoldier->bVehicleID;
				}

				// vehicles
				ppMovePath = &( pVehicleList[ iVehicleId ].pMercPath );
				ubGroupId = pVehicleList[ iVehicleId ].ubMovementGroup;
			}
			else
			{
				// mercs on foot
				/*CHRISL: I don't understand why we're using bSelectedDestChar to generate the ppMovePath but iCounter to
					generate ubGroupId.  For vehicles, we use the iVhicleId to generate both.  Shouldn't we be using the
					same system here?  Meaning, shouldn't ppMovePath be generated with the soldier indicated with iCounter?*/
				//ppMovePath = &( Menptr[gCharactersList[ bSelectedDestChar ].usSolID].pMercPath );
				ppMovePath = &( pSoldier->pMercPath );
				ubGroupId = pSoldier->ubGroupID;
			}

			// if we haven't already rebuilt this group
			if ( !fGroupIDRebuilt[ ubGroupId ] )
			{
				// rebuild it now
				RebuildWayPointsForGroupPath( *ppMovePath, ubGroupId );

				// mark it as rebuilt
				fGroupIDRebuilt[ ubGroupId ] = TRUE;
			}
		}
	}
}


void UpdateCursorIfInLastSector( void )
{

	INT16 sMapX = 0, sMapY = 0;

	// check to see if we are plotting a path, if so, see if we are highlighting the last sector int he path, if so, change the cursor
	if ( ( bSelectedDestChar != -1) || ( fPlotForHelicopter == TRUE ) )
	{
		GetMouseMapXY(&sMapX, &sMapY);

		// translate screen values to map grid values for zoomed in
		if(fZoomFlag)
		{
			sMapX=(UINT16)iZoomX/MAP_GRID_X+sMapX;
			sMapX=sMapX/2;
			sMapY=(UINT16)iZoomY/MAP_GRID_Y+sMapY;
			sMapY=sMapY/2;
		}

		if( fShowAircraftFlag == FALSE )
		{
			if( bSelectedDestChar != -1 )
			{
				//c heck if we are in the last sector of the characters path?
				if( sMapX + ( sMapY * MAP_WORLD_X ) == GetLastSectorIdInCharactersPath( ( &Menptr[ gCharactersList[ bSelectedDestChar ].usSolID ] ) ) )
				{
					// set cursor to checkmark
					ChangeMapScreenMaskCursor( CURSOR_CHECKMARK );
				}
				else if( fCheckCursorWasSet )
				{
					// reset to walking guy/vehicle
					SetUpCursorForStrategicMap( );
				}
			}
		}
		else
		{
			// check for helicopter
			if( fPlotForHelicopter )
			{
				if( sMapX + ( sMapY * MAP_WORLD_X ) == GetLastSectorOfHelicoptersPath( ) )
				{
					// set cursor to checkmark
					ChangeMapScreenMaskCursor( CURSOR_CHECKMARK );
				}
				else if( fCheckCursorWasSet )
				{
					// reset to walking guy/vehicle
					SetUpCursorForStrategicMap( );
				}
			}
			else
			{
				// reset to walking guy/vehicle
				SetUpCursorForStrategicMap( );
			}
		}
	}

	return;

}



void FaceRegionBtnCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	// error checking, make sure someone is there
	if( bSelectedInfoChar == -1 )
	{
		return;
	}
	else if( gCharactersList[ bSelectedInfoChar ].fValid == FALSE )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		if( gfPreBattleInterfaceActive == TRUE )
		{
			return;
		}

		// now stop any dialogue in progress
		StopAnyCurrentlyTalkingSpeech( );
	}

	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		RequestToggleMercInventoryPanel();
	}
}


void FaceRegionMvtCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	if( bSelectedInfoChar == -1 )
	{
		fShowFaceHightLight = FALSE;
		return;
	}
	else if( gCharactersList[ bSelectedInfoChar ].fValid == FALSE )
	{
		fShowFaceHightLight = FALSE;
		return;
	}

	if( ( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE ) )
	{
		fShowFaceHightLight = TRUE;
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fShowFaceHightLight = FALSE;
	}
}



void ItemRegionBtnCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	// left AND right button are handled the same way
	if (iReason & ( MSYS_CALLBACK_REASON_RBUTTON_UP | MSYS_CALLBACK_REASON_LBUTTON_UP ) )
	{
		RequestToggleMercInventoryPanel();
	}
}


void ItemRegionMvtCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	if ( !CanToggleSelectedCharInventory() )
	{
		fShowItemHighLight = FALSE;
		return;
	}

	if( ( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE ) )
	{
		fShowItemHighLight = TRUE;
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fShowItemHighLight = FALSE;
	}
}



void HandleChangeOfHighLightedLine( void )
{
	static INT32 iOldHighLine;

	if ( fShowInventoryFlag )
	{
		return;
	}

	// check if change in highlight line
	if( giHighLine != iOldHighLine )
	{
		iOldHighLine = giHighLine;

		if( giHighLine == -1 )
		{
			giSleepHighLine = -1;
			giAssignHighLine = -1;
			giContractHighLine = -1;
			giSleepHighLine = -1;

			// don't do during plotting, allowing selected character to remain highlighted and their destination column to glow!
			if( ( bSelectedDestChar == -1 ) && ( fPlotForHelicopter == FALSE ) )
			{
				giDestHighLine = -1;
			}
		}

		fDrawCharacterList = TRUE;
	}
}


void UpdateTownMinePopUpDisplay( void )
{
	if ( gMapViewRegion.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		ForceUpDateOfBox( ghTownMineBox );
		MapscreenMarkButtonsDirty();
	}
}


void HandleCharBarRender( void )
{
	// check if the panel is disbled, if so, do not render
	if( ( bSelectedInfoChar !=-1) && ( fDisableDueToBattleRoster == FALSE ) )
	{
		// valid character?...render
		if( gCharactersList[ bSelectedInfoChar ].fValid == TRUE )
		{
			//if( !( ( fShowContractMenu)||( fShowAssignmentMenu ) ) )
			//{
				// draw bars for them
				DrawCharBars( );

				UpdateCharRegionHelpText( );
			//}
		}
	}
}



// update the status of the contract box
void UpDateStatusOfContractBox( void )
{
	if( fShowContractMenu == TRUE )
	{
		ForceUpDateOfBox( ghContractBox );

		if( ( Menptr[gCharactersList[ bSelectedInfoChar ].usSolID].stats.bLife == 0 )||( Menptr[gCharactersList[bSelectedInfoChar].usSolID].bAssignment == ASSIGNMENT_POW ) )
		{
			ForceUpDateOfBox( ghRemoveMercAssignBox );
		}
	}

	return;
}



void DestroyTheItemInCursor( )
{
	// actually destroy this item
	// End Item pickup
	MAPEndItemPointer( );
	gpItemPointer = NULL;
}


void TrashItemMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		// find the item and get rid of it

		DestroyTheItemInCursor( );

		// reset cursor
		MSYS_ChangeRegionCursor( &gSMPanelRegion , CURSOR_NORMAL );
		SetCurrentCursorFromDatabase( CURSOR_NORMAL );

		HandleButtonStatesWhileMapInventoryActive( );
	}
}


void TrashCanBtnCallback( MOUSE_REGION *pRegion, INT32 iReason)
{

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		// check if an item is in the cursor, if so, warn player
		if( gpItemPointer != NULL )
		{
		// set up for mapscreen
			Assert(! gpItemPointer->ubMission);
			//if( gpItemPointer->ubMission )
			//{
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, pTrashItemText[ 1 ], MAP_SCREEN, MSG_BOX_FLAG_YESNO, TrashItemMessageBoxCallBack );
			//}
			//else
			//{
			//	DoMapMessageBox( MSG_BOX_BASIC_STYLE, pTrashItemText[ 0 ], MAP_SCREEN, MSG_BOX_FLAG_YESNO, TrashItemMessageBoxCallBack );
			//}
		}
	}
}


void TrashCanMoveCallback( MOUSE_REGION *pRegion, INT32 iReason)
{

	if (iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if( gMPanelRegion.Cursor == EXTERN_CURSOR )
		{
			fShowTrashCanHighLight = TRUE;
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fShowTrashCanHighLight = FALSE;
	}
}


void MapInvDoneButtonfastHelpCall( )
{
	SetPendingNewScreen( LAPTOP_SCREEN );
}


void UpdateStatusOfMapSortButtons( void )
{
	INT32 iCounter = 0;
	static BOOLEAN fShownLastTime = FALSE;


	if( ( gfPreBattleInterfaceActive ) || fShowInventoryFlag )
	{
		if ( fShownLastTime )
		{
			for( iCounter = 0; iCounter < MAX_SORT_METHODS; iCounter++ )
			{
				HideButton( giMapSortButton[ iCounter ] );
			}
			// OJW - 20090210 - hide MP buttons here
			if (is_networked)
			{
				for( iCounter = 0; iCounter < MAX_MP_BUTTONS; iCounter++ )
				{
					HideButton( giMapMPButton[ iCounter ] );
				}
			}
			if ( gfPreBattleInterfaceActive )
			{
				HideButton( giCharInfoButton[ 0 ] );
				HideButton( giCharInfoButton[ 1 ] );
			}

			fShownLastTime = FALSE;
		}
	}
	else
	{
		if ( !fShownLastTime )
		{
			for( iCounter = 0; iCounter < MAX_SORT_METHODS; iCounter++ )
			{
				ShowButton( giMapSortButton[ iCounter ] );
			}

			// OJW - 20090210 - show MP buttons here
			if (is_networked)
			{
				for( iCounter = 0; iCounter < MAX_MP_BUTTONS; iCounter++ )
				{
					ShowButton( giMapMPButton[ iCounter ] );
				}
			}
			ShowButton( giCharInfoButton[ 0 ] );
			ShowButton( giCharInfoButton[ 1 ] );

			fShownLastTime = TRUE;
		}
	}
}



INT8 GetLastValidCharacterInTeamPanelList( void )
{
	INT8 iCounter = 0, iValue = 0;

	// run through the list and find the last valid guy in the list
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( gCharactersList[ iCounter ].fValid == TRUE )
		{
			if(( Menptr[ gCharactersList[ iCounter ].usSolID ].stats.bLife >= OKLIFE ) )
			{
				if( fShowMapInventoryPool )
				{
					if( Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorX == sSelMapX && Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorY == sSelMapY && Menptr[ gCharactersList[ iCounter ].usSolID ].bSectorZ == ( INT8 )( iCurrentMapSectorZ ) )
					{
						iValue = iCounter;
					}
				}
				else
				{
					if( fShowInventoryFlag && ( gMPanelRegion.Cursor == EXTERN_CURSOR ) )
					{
						if( bSelectedInfoChar != -1 )
						{
							if( gCharactersList[ bSelectedInfoChar ].fValid == TRUE )
							{
								if( Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorX == Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX &&	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorY == Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY && Menptr[ gCharactersList[ iCounter ].usSolID ].bSectorZ ==Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ )
								{
									iValue = iCounter;
								}
							}
						}
					}
					else
					{
						iValue = iCounter;
					}
				}
			}
		}
	}

	// return the character
	return( iValue );
}

/*
// NB These functions weren't being called anywhere!	Use GoToNextCharacterInList etc instead
INT8 GetPrevValidCharacterInTeamPanelList( INT8 bCurrentIndex )
{
	INT8 iCounter = bCurrentIndex, iValue = 0;

	// run through the list and find the last valid guy in the list
	for( iCounter = bCurrentIndex; iCounter > 0; iCounter-- )
	{
		if( gCharactersList[ iCounter ].fValid == TRUE )
		{
			if( ( Menptr[ gCharactersList[ iCounter ].usSolID ].bLife >= OKLIFE ) )
			{
				if( fShowMapInventoryPool )
				{
					if(	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorX == sSelMapX &&	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorY == sSelMapY && Menptr[ gCharactersList[ iCounter ].usSolID ].bSectorZ == ( INT8 )( iCurrentMapSectorZ ) )
					{
						iValue = iCounter;
					}
				}
				else
				{
					if( fShowInventoryFlag && ( gMPanelRegion.Cursor == EXTERN_CURSOR ) )
					{
						if( bSelectedInfoChar != -1 )
						{
							if( gCharactersList[ bSelectedInfoChar ].fValid == TRUE )
							{
								if( Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorX == Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX &&	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorY == Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY && Menptr[ gCharactersList[ iCounter ].usSolID ].bSectorZ ==Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ )
								{
									iValue = iCounter;
									iCounter = 0;
								}
							}
						}
					}
					else
					{
						iValue = iCounter;
					}
				}
			}
		}
	}

	// return the character
	return( iValue );
}

INT8 GetNextValidCharacterInTeamPanelList( INT8 bCurrentIndex )
{
	INT8 iCounter = bCurrentIndex, iValue = 0;

	// run through the list and find the last valid guy in the list
	for( iCounter = bCurrentIndex; iCounter < CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( gCharactersList[ iCounter ].fValid == TRUE )
		{
			if( ( Menptr[ gCharactersList[ iCounter ].usSolID ].bLife >= OKLIFE ) )
			{
				if( fShowMapInventoryPool )
				{
					if(	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorX == sSelMapX &&	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorY == sSelMapY && Menptr[ gCharactersList[ iCounter ].usSolID ].bSectorZ == ( INT8 )( iCurrentMapSectorZ ) )
					{
						iValue = iCounter;
					}
				}
				else
				{
					if( fShowInventoryFlag && ( gMPanelRegion.Cursor == EXTERN_CURSOR ) )
					{
						if( bSelectedInfoChar != -1 )
						{
							if( gCharactersList[ bSelectedInfoChar ].fValid == TRUE )
							{
								if( Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorX == Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX &&	Menptr[ gCharactersList[ iCounter ].usSolID ].sSectorY == Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY && Menptr[ gCharactersList[ iCounter ].usSolID ].bSectorZ ==Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ )
								{
									iValue = iCounter;
								}
							}
						}
					}
					else
					{
						iValue = iCounter;
					}
				}
			}
		}
	}

	// return the character
	return( iValue );
}
*/







void CreateDestroyTrashCanRegion( void )
{
	static BOOLEAN fCreated = FALSE;

	if( fShowInventoryFlag && ( fCreated == FALSE ) )
	{

		fCreated = TRUE;

		// trash can
		MSYS_DefineRegion( &gTrashCanRegion, 	TRASH_CAN_X, TRASH_CAN_Y, TRASH_CAN_X + TRASH_CAN_WIDTH, TRASH_CAN_Y + TRASH_CAN_HEIGHT , MSYS_PRIORITY_HIGHEST - 4 ,
							MSYS_NO_CURSOR, TrashCanMoveCallback, TrashCanBtnCallback );

		// done inventory button define
		giMapInvButtonDoneImage = LoadButtonImage( "INTERFACE\\done_button2.sti" ,-1,0,-1,1,-1 );
	giMapInvDoneButton = QuickCreateButton( giMapInvButtonDoneImage, INV_BTN_X, INV_BTN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										( GUI_CALLBACK )BtnGenericMouseMoveButtonCallback, ( GUI_CALLBACK)DoneInventoryMapBtnCallback );

		SetButtonFastHelpText( giMapInvDoneButton ,pMiscMapScreenMouseRegionHelpText[ 2 ] );
		SetRegionFastHelpText( &gTrashCanRegion, pMiscMapScreenMouseRegionHelpText[ 1 ] );

		InitMapKeyRingInterface( KeyRingItemPanelButtonCallback );
/*
		giMapInvNextImage=	LoadButtonImage( "INTERFACE\\inventory_buttons.sti" ,-1,20,-1,22,-1 );
	giMapInvNext= QuickCreateButton( giMapInvNextImage, ( 2 ), ( 79 ) ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										( GUI_CALLBACK )BtnGenericMouseMoveButtonCallback, ( GUI_CALLBACK)NextInventoryMapBtnCallback );


		giMapInvPrevImage=	LoadButtonImage( "INTERFACE\\inventory_buttons.sti" ,-1,21,-1,23,-1 );
	giMapInvPrev= QuickCreateButton( giMapInvPrevImage, ( 30 ) , ( 79 ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										( GUI_CALLBACK )BtnGenericMouseMoveButtonCallback, ( GUI_CALLBACK)PrevInventoryMapBtnCallback );

	*/

			// reset the compatable item array at this point
		ResetCompatibleItemArray( );

	}
	else if( ( fShowInventoryFlag == FALSE ) && ( fCreated == TRUE ) )
	{
		// trash can region
		fCreated = FALSE;
		MSYS_RemoveRegion( &gTrashCanRegion );

		// map inv done button
		RemoveButton( giMapInvDoneButton );

		// get rid of button image
		UnloadButtonImage( giMapInvButtonDoneImage );

		ShutdownKeyRingInterface( );

		ShutdownInventoryInterface();

		if( fShowDescriptionFlag == TRUE )
		{
			// kill description
			DeleteItemDescriptionBox( );
		}

	}
}

void InvmaskRegionBtnCallBack( MOUSE_REGION *pRegion, INT32 iReason )
{
	// CJC, December 15 1998: do NOTHING for clicks here
}


void DoneInventoryMapBtnCallback( GUI_BUTTON *btn, INT32 reason )
{
	// prevent inventory from being closed while stack popup up!
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&= ~(BUTTON_CLICKED_ON);

			if ( gMPanelRegion.Cursor != EXTERN_CURSOR && !InItemStackPopup() )
			{
				fEndShowInventoryFlag = TRUE;
			}
		}
	}
}



void StartConfirmMapMoveMode( INT16 sMapY )
{
	UINT8 ubPosition = ( sMapY < 8 ) ? MSG_MAP_UI_POSITION_LOWER : MSG_MAP_UI_POSITION_UPPER;


	// tell player what to do - to click again to confirm move
	MapScreenMessage( FONT_MCOLOR_LTYELLOW, ubPosition, pMapPlotStrings[ 0 ] );

	gfInConfirmMapMoveMode = TRUE;
}


void EndConfirmMapMoveMode( void )
{
	CancelMapUIMessage( );

	gfInConfirmMapMoveMode = FALSE;
}



void CancelMapUIMessage( void )
{
	// and kill the message overlay
	EndUIMessage( );

	fMapPanelDirty = TRUE;
}


// automatically turns off mapscreen ui overlay messages when appropriate
void MonitorMapUIMessage( void )
{
	// if there is a map UI message being displayed
	if( giUIMessageOverlay != -1 )
	{
		// and if we're not in the middle of the "confirm move" sequence
//		if( !gfInConfirmMapMoveMode || bSelectedDestChar == -1 )
		{
			// and we've now exceed its period of maximum persistance (without user input)
			if ( ( GetJA2Clock( ) - guiUIMessageTime ) > guiUIMessageTimeDelay )
			{
				// then cancel the message now
				CancelMapUIMessage( );
			}
		}
	}
}


void HandlePreBattleInterfaceWithInventoryPanelUp( void )
{
	if( ( gfPreBattleInterfaceActive == TRUE ) && ( fShowInventoryFlag == TRUE ) )
	{
		if( fShowDescriptionFlag == TRUE )
		{
			// kill description
			DeleteItemDescriptionBox( );
		}

		// kill inventory panel
		fShowInventoryFlag = FALSE;
		CreateDestroyMapInvButton();
	}
}


// this puts anyone who is on NO_ASSIGNMENT onto a free squad
void UpdateBadAssignments( void )
{
	UINT32 iCounter;

	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( gCharactersList[ iCounter ].fValid == TRUE )
		{
			CheckIfSoldierUnassigned( &Menptr[ gCharactersList[ iCounter ].usSolID ] );
		}
	}

	return;
}

void InterruptTimeForMenus( void )
{
	if( ( fShowAssignmentMenu == TRUE )||( fShowContractMenu == TRUE ) )
	{
		InterruptTime( );
		PauseTimeForInterupt( );
	}
	else if( fOneFrame )
	{
		InterruptTime( );
		PauseTimeForInterupt( );
	}

	return;
}


void HandleContractTimeFlashForMercThatIsAboutLeave( void )
{
	INT32 iCurrentTime;

	// grab the current time
	iCurrentTime = GetJA2Clock();

	// only bother checking once flash interval has elapsed
	if( ( iCurrentTime - giFlashContractBaseTime ) >= DELAY_PER_FLASH_FOR_DEPARTING_PERSONNEL )
	{
		// update timer so that we only run check so often
		giFlashContractBaseTime = iCurrentTime;
		fFlashContractFlag = !fFlashContractFlag;

		// don't redraw unless we have to!
		if ( AnyMercsLeavingRealSoon() )
		{
			// redraw character list
			fDrawCharacterList = TRUE;
		}
	}
}


BOOLEAN AnyMercsLeavingRealSoon()
{
	UINT32 uiCounter = 0;
	UINT32 uiTimeInMin = GetWorldTotalMin();
	BOOLEAN fFoundOne = FALSE;

	for( uiCounter = 0; uiCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; uiCounter++ )
	{
		if( gCharactersList[ uiCounter ].fValid == TRUE )
		{
			if( ( Menptr[gCharactersList[uiCounter].usSolID].iEndofContractTime - uiTimeInMin ) <= MINS_TO_FLASH_CONTRACT_TIME )
			{
				fFoundOne = TRUE;
				break;
			}
		}
	}

	return( fFoundOne );
}


void HandleRemovalOfPreLoadedMapGraphics( void )
{

	if( fPreLoadedMapGraphics == TRUE )
	{
		DeleteMapBottomGraphics( );
		DeleteVideoObjectFromIndex( guiMAPCURSORS );
		DeleteVideoObjectFromIndex( guiSleepIcon );

		//DeleteVideoObjectFromIndex(guiMAPBORDER);
		DeleteVideoObjectFromIndex(guiCHARLIST);
		//DeleteVideoObjectFromIndex(guiCORNERADDONS);
		//DeleteVideoObjectFromIndex(guiMAPCORNER);
	//	DeleteVideoObjectFromIndex(guiPOPUPBORDERS);
		DeleteVideoObjectFromIndex(guiCHARINFO);
		DeleteVideoObjectFromIndex(guiCHARICONS);
		DeleteVideoObjectFromIndex(guiCROSS);
		DeleteVideoSurfaceFromIndex(guiBIGMAP);
		DeleteVideoObjectFromIndex(guiSubLevel1);
		DeleteVideoObjectFromIndex(guiSubLevel2);
		DeleteVideoObjectFromIndex(guiSubLevel3);

	//	DeleteVideoSurfaceFromIndex(guiPOPUPTEX);
		DeleteVideoObjectFromIndex(guiSAMICON);
		DeleteVideoObjectFromIndex(guiMAPINV);
		DeleteVideoObjectFromIndex(guiMapInvSecondHandBlockout);
		DeleteVideoObjectFromIndex(guiULICONS);
		DeleteVideoObjectFromIndex(guiORTAICON);
		DeleteVideoObjectFromIndex(guiTIXAICON);
		DeleteVideoObjectFromIndex( guiCHARBETWEENSECTORICONS );
		DeleteVideoObjectFromIndex( guiCHARBETWEENSECTORICONSCLOSE );
		DeleteVideoObjectFromIndex( guiLEVELMARKER );
		DeleteVideoObjectFromIndex( guiMapBorderEtaPopUp );
		DeleteVideoObjectFromIndex( guiSecItemHiddenVO );
		DeleteVideoObjectFromIndex( guiSelectedCharArrow );
		DeleteVideoObjectFromIndex( guiMapBorderHeliSectors );
		DeleteVideoObjectFromIndex( guiHelicopterIcon );
		DeleteVideoObjectFromIndex( guiMINEICON );
		DeleteVideoObjectFromIndex( guiSectorLocatorGraphicID );

		//Kris:	Remove the email icons.
		DeleteVideoObjectFromIndex( guiNewMailIcons );

		DeleteVideoObjectFromIndex( guiBULLSEYE );


		// remove the graphic for the militia pop up box
		RemoveMilitiaPopUpBox( );

		// remove inventory pool graphic
		RemoveInventoryPoolGraphic( );

		// get rid of border stuff
		DeleteMapBorderGraphics( );
	}

	return;
}


BOOLEAN CharacterIsInLoadedSectorAndWantsToMoveInventoryButIsNotAllowed( INT8 bCharId )
{
	UINT16 usSoldierId = 0;

	// invalid char id
	if( bCharId == -1 )
	{
		return( FALSE );
	}

	// valid char?
	if( gCharactersList[ bCharId ].fValid == FALSE )
	{
		return( FALSE );
	}

	// get the soldier id
	usSoldierId = gCharactersList[ bCharId ].usSolID;

	// char is in loaded sector
	if( Menptr[ usSoldierId ].sSectorX != gWorldSectorX || Menptr[ usSoldierId ].sSectorY != gWorldSectorY || Menptr[ usSoldierId ].bSectorZ != gbWorldSectorZ )
	{
		return( FALSE );
	}

	// not showing inventory?
	if( fShowInventoryFlag == FALSE )
	{
		// nope
		return( FALSE );
	}

	// picked something up?
	if( gMPanelRegion.Cursor != EXTERN_CURSOR )
	{
		// nope
		return( FALSE );
	}

	// only disallow when enemies in sector
	if ( !gTacticalStatus.fEnemyInSector )
	{
		return( FALSE );
	}

	return( TRUE );
}



void UpdateTheStateOfTheNextPrevMapScreenCharacterButtons( void )
{
	if( gfPreBattleInterfaceActive )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
		}
	}
	else if( bSelectedInfoChar == -1 )
	{
		DisableButton( giCharInfoButton[ 0 ] );
		DisableButton( giCharInfoButton[ 1 ] );
		DisableButton( giMapContractButton );
	}
/* ARM: Commented out at KM's request, it won't reenabled them when coming back from PBI, on Feb. 22, 99
	else if ( fShowInventoryFlag == FALSE ) // make sure that we are in fact showing the mapscreen inventory
	{
		return;
	}
*/
	else
	{
		// standard checks
		if ( ( GetNumberOfPeopleInCharacterList( ) < 2 ) ||
				( CharacterIsInLoadedSectorAndWantsToMoveInventoryButIsNotAllowed( bSelectedInfoChar ) ) ||
				( CharacterIsInTransitAndHasItemPickedUp( bSelectedInfoChar ) ) ||
				( fShowDescriptionFlag ) )
		{
			DisableButton( giCharInfoButton[ 0 ] );
			DisableButton( giCharInfoButton[ 1 ] );
		}
		else
		{
			EnableButton( giCharInfoButton[ 0 ] );
			EnableButton( giCharInfoButton[ 1 ] );
		}
	}
}


void PrevInventoryMapBtnCallback( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&= ~(BUTTON_CLICKED_ON);
			fResetMapCoords = TRUE;
			GoToPrevCharacterInList( );
		}
	}
}


void NextInventoryMapBtnCallback( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&= ~(BUTTON_CLICKED_ON);
			fResetMapCoords = TRUE;
			GoToNextCharacterInList( );
		}
	}
}

void CreateDestroyMapCharacterScrollButtons( void )
{
	static BOOLEAN fCreated = FALSE;

	if( ( fInMapMode == TRUE ) && ( fCreated == FALSE ) )
	{
		// set the button image
		giCharInfoButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,11,4,-1,6,-1 );

		// set the button value
		giCharInfoButton[ 0 ] = QuickCreateButton( giCharInfoButtonImage[ 0 ], 67, 69,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 5,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PrevInventoryMapBtnCallback );

			// set the button image
		giCharInfoButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,12,5,-1,7,-1 );

		// set the button value
		giCharInfoButton[ 1 ] = QuickCreateButton( giCharInfoButtonImage[ 1 ], 67, 87,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 5,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)NextInventoryMapBtnCallback );


		SetButtonFastHelpText( giCharInfoButton[ 0 ], pMapScreenPrevNextCharButtonHelpText[ 0 ] );
		SetButtonFastHelpText( giCharInfoButton[ 1 ], pMapScreenPrevNextCharButtonHelpText[ 1 ] );

		fCreated = TRUE;

	}
	else if( ( ( fInMapMode == FALSE ) ) && ( fCreated == TRUE ) )
	{
		UnloadButtonImage( giCharInfoButtonImage[ 0 ] );
		UnloadButtonImage( giCharInfoButtonImage[ 1 ] );
		RemoveButton( giCharInfoButton[ 0 ]);
		RemoveButton( giCharInfoButton[ 1 ]);

		fCreated = FALSE;
	}
}



void TellPlayerWhyHeCantCompressTime( void )
{
	// if we're locked into paused time compression by some event that enforces that
	if ( PauseStateLocked() )
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_RED, MSG_BETAVERSION, L"(BETA) Can't compress time, pause state locked (reason %d). OK unless permanent.", guiLockPauseStateLastReasonId );
		ScreenMsg( FONT_MCOLOR_RED, MSG_BETAVERSION, L"(BETA) If permanent, take screenshot now, send with *previous* save & describe what happened since.");
#endif
	}
	else if( gfAtLeastOneMercWasHired == FALSE )
	{
		// no mercs hired, ever
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapScreenJustStartedHelpText[ 0 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
	}
	// WANNE - MP: It is forbidden to compress time in multiplayer
	else if (is_networked)
	{
		ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[65]);
	}
	else if ( !AnyUsableRealMercenariesOnTeam() )
	{
		// no usable mercs left on team
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapErrorString[ 39 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
	}
	else if ( ActiveTimedBombExists() )
	{
		// can't time compress when a bomb is about to go off!
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, gzLateLocalizedString[ 2 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
	}
	else if ( gfContractRenewalSquenceOn )
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_RED, MSG_BETAVERSION, L"(BETA) Can't compress time while contract renewal sequence is on.");
#endif
	}
	else if( fDisableMapInterfaceDueToBattle )
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_RED, MSG_BETAVERSION, L"(BETA) Can't compress time while disabled due to battle.");
#endif
	}
	else if( fDisableDueToBattleRoster )
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_RED, MSG_BETAVERSION, L"(BETA) Can't compress time while in battle roster.");
#endif
	}
	else if ( fMapInventoryItem )
	{
#ifdef JA2BETAVERSION
		ScreenMsg( FONT_MCOLOR_RED, MSG_BETAVERSION, L"(BETA) Can't compress time while still holding an inventory item.");
#endif
	}
	else if( fShowMapInventoryPool )
	{
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, gzLateLocalizedString[ 55 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
	}
	// ARM: THIS TEST SHOULD BE THE LAST ONE, BECAUSE IT ACTUALLY RESULTS IN SOMETHING HAPPENING NOW.
	// KM:	Except if we are in a creature lair and haven't loaded the sector yet (no battle yet)
	else if( gTacticalStatus.uiFlags & INCOMBAT || gTacticalStatus.fEnemyInSector )
	{
		if( OnlyHostileCivsInSector() )
		{
			CHAR16 str[ 256 ];
			CHAR16 pSectorString[ 128 ];
			GetSectorIDString( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, pSectorString, TRUE );
			swprintf( str, gzLateLocalizedString[ 27 ], pSectorString );
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, str, MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
		}
		else
		{
			//The NEW non-persistant PBI is used instead of a dialog box explaining why we can't compress time.
			InitPreBattleInterface( NULL, FALSE );
		}
	}
	else if( PlayerGroupIsInACreatureInfestedMine() )
	{
		DoMapMessageBox( MSG_BOX_BASIC_STYLE, gzLateLocalizedString[ 28 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
	}
}



void MapScreenDefaultOkBoxCallback( UINT8 bExitValue )
{
	// yes, load the game
	if( bExitValue == MSG_BOX_RETURN_OK )
	{
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}

	return;
}


void MapSortBtnCallback( GUI_BUTTON *btn, INT32 reason )
{
	INT32 iValue = 0;

	// grab the button index value for the sort buttons
	iValue = MSYS_GetBtnUserData( btn, 0 );


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags &= ~(BUTTON_CLICKED_ON);

			ChangeCharacterListSortMethod( iValue );
		}
	}
}



void AddTeamPanelSortButtonsForMapScreen( void )
{
	INT32 iCounter = 0;
	SGPFILENAME filename;
	INT32 iImageIndex[ MAX_SORT_METHODS ] = { 0, 1, 5, 2, 3, 4 };		// sleep image is out or order (last)


	GetMLGFilename( filename, MLG_GOLDPIECEBUTTONS );

	for( iCounter = 0; iCounter < MAX_SORT_METHODS; iCounter++ )
	{
		giMapSortButtonImage[ iCounter ] = LoadButtonImage( filename, -1, iImageIndex[ iCounter ] , -1, iImageIndex[ iCounter ] + 6 , -1 );

		// buttonmake
		giMapSortButton[ iCounter ]= QuickCreateButton( giMapSortButtonImage[ iCounter ], ( INT16 )( gMapSortButtons[ iCounter ].iX ), ( INT16 )( gMapSortButtons[ iCounter ].iY ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 5,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapSortBtnCallback );

		MSYS_SetBtnUserData(giMapSortButton[ iCounter ], 0, iCounter );

		SetButtonFastHelpText( giMapSortButton[ iCounter ], wMapScreenSortButtonHelpText[ iCounter ] );
	}
	return;
}

void AddMPButtonsForMapScreen( void )
{
	INT32 iCounter = 0;
	SGPFILENAME filename;
	INT32 iImageIndex[ MAX_MP_BUTTONS+1 ] = { 0, 1, 3, 5 , 7 };		// sleep image is out or order (last)
	INT32 iPressedIndex[ MAX_MP_BUTTONS+1 ] = { 0 , 2 , 4 , 6 , 8 };

	GetMLGFilename( filename, MLG_MP_GOLDPIECEBUTTONS );

	// <TODO> add start game button and hide it...

	for( iCounter = 0; iCounter < MAX_MP_BUTTONS+1; iCounter++ )
	{
		giMapMPButtonImage[ iCounter ] = LoadButtonImage( filename, -1, iImageIndex[ iCounter ] , -1, iPressedIndex[ iCounter ] , -1 );

		if (iCounter < MAX_MP_BUTTONS)
		{
			// buttonmake
			giMapMPButton[ iCounter ]= QuickCreateButton( giMapMPButtonImage[ iCounter ], ( INT16 )( gMapMPButtonsX[ iCounter ] ), ( INT16 )( MP_BTN_Y ),
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 5,
											(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MPReadyButtonCallback );

			// so we can tell which button was clicked
			MSYS_SetBtnUserData(giMapMPButton[ iCounter ], 0, iCounter );
		}

		//SetButtonFastHelpText( giMapSortButton[ iCounter ], wMapScreenSortButtonHelpText[ iCounter ] );
	}

	if ((is_server || is_host) && !allowlaptop)
	{
		// Server has 'Start game' button showing first if the game hasnt started
		SpecifyButtonImage( giMapMPButton [3] , giMapMPButtonImage [4] );
	}

	return;
}
void SortListOfMercsInTeamPanel( BOOLEAN fRetainSelectedMercs, BOOLEAN fReverse )
{
	INT32 iCounter = 0, iCounterA = 0;
	INT16 sEndSectorA, sEndSectorB;
	INT32 iExpiryTime, iExpiryTimeA;
	UINT8 uiID, uiIDA;
	SOLDIERTYPE *pSelectedSoldier[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
	SOLDIERTYPE *pCurrentSoldier = NULL;
	SOLDIERTYPE *pPreviousSelectedInfoChar = NULL;


	if ( fRetainSelectedMercs )
	{
		// if we have anyone valid selected
		if ( ( bSelectedInfoChar != -1 ) && ( gCharactersList[ bSelectedInfoChar ].fValid ) )
		{
			pPreviousSelectedInfoChar = &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ];
		}


		for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
		{
			// set current entry to null
			pSelectedSoldier[ iCounter ] = NULL;


			// is this entry even valid
			if( gCharactersList[ iCounter ].fValid == FALSE )
			{
				continue;
			}

			// get soldier assoc. with entry
			pCurrentSoldier = &Menptr[ gCharactersList[ iCounter ].usSolID ];

			// check if soldier is active
			if( pCurrentSoldier->bActive == FALSE )
			{
				continue;
			}

			// are they selected?...
			if( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE )
			{
				// yes, store pointer to them
				pSelectedSoldier[ iCounter ] = pCurrentSoldier;
			}
		}
	}


	// do the sort
	for( iCounter = 1; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS ; iCounter++ )
	{
		// have we gone too far
		if( gCharactersList[ iCounter ].fValid == FALSE )
		{
			break;
		}

		switch( giSortStateForMapScreenList )
		{
			case( 0 ):
				// by name
				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						break;
					}

					//if( ( wcscmp( Menptr[ gCharactersList[ iCounterA ].usSolID ].name, Menptr[ gCharactersList[ iCounter ].usSolID ].name ) > 0 ) && ( iCounterA < iCounter ) )
					if( iCounterA < iCounter )
					{
						if((fReverse && ( wcscmp( Menptr[ gCharactersList[ iCounterA ].usSolID ].name, Menptr[ gCharactersList[ iCounter ].usSolID ].name ) < 0 )) ||
							(!fReverse && ( wcscmp( Menptr[ gCharactersList[ iCounterA ].usSolID ].name, Menptr[ gCharactersList[ iCounter ].usSolID ].name ) > 0 )))
						SwapCharactersInList( iCounter, iCounterA );
					}
				}
				break;

			case( 1 ):
				// by assignment
				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						break;
					}

					if( !fReverse && ( Menptr[ gCharactersList[ iCounterA ].usSolID ].bAssignment > Menptr[ gCharactersList[ iCounter ].usSolID ].bAssignment ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( fReverse && ( Menptr[ gCharactersList[ iCounterA ].usSolID ].bAssignment < Menptr[ gCharactersList[ iCounter ].usSolID ].bAssignment ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( ( Menptr[ gCharactersList[ iCounterA ].usSolID ].bAssignment == Menptr[ gCharactersList[ iCounter ].usSolID ].bAssignment ) && ( iCounterA < iCounter ) )
					{
						// same assignment

						// if it's in a vehicle
						if( Menptr[ gCharactersList[ iCounterA ].usSolID ].bAssignment == VEHICLE )
						{
							// then also compare vehicle IDs
							if( ( Menptr[ gCharactersList[ iCounterA ].usSolID ].iVehicleId > Menptr[ gCharactersList[ iCounter ].usSolID ].iVehicleId ) && ( iCounterA < iCounter ) )
							{
								SwapCharactersInList( iCounter, iCounterA );
							}
						}
					}
				}
				break;

			case( 2 ):
				// by sleep status
				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						break;
					}

					if( !fReverse && ( Menptr[ gCharactersList[ iCounterA ].usSolID ].flags.fMercAsleep == TRUE ) && ( Menptr[ gCharactersList[ iCounter ].usSolID ].flags.fMercAsleep == FALSE ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( fReverse && ( Menptr[ gCharactersList[ iCounterA ].usSolID ].flags.fMercAsleep == FALSE ) && ( Menptr[ gCharactersList[ iCounter ].usSolID ].flags.fMercAsleep == TRUE ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
				}
				break;

			case( 3 ):
				//by location

				sEndSectorA = CalcLocationValueForChar( iCounter );

				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						break;
					}

					sEndSectorB = CalcLocationValueForChar( iCounterA );

					if( !fReverse && ( sEndSectorB > sEndSectorA ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( fReverse && ( sEndSectorB < sEndSectorA ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
				}
				break;

			case( 4 ):
				// by destination sector
				if( GetLengthOfMercPath( MercPtrs[ gCharactersList[ iCounter ].usSolID ] ) == 0 )
				{
					sEndSectorA = 9999;
				}
				else
				{
					sEndSectorA = GetLastSectorIdInCharactersPath( &Menptr[ gCharactersList[ iCounter ].usSolID ] );
				}

				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						break;
					}

					if( GetLengthOfMercPath( MercPtrs[ gCharactersList[ iCounterA ].usSolID ] ) == 0 )
					{
						sEndSectorB = 9999;
					}
					else
					{
						sEndSectorB = GetLastSectorIdInCharactersPath( &Menptr[ gCharactersList[ iCounterA ].usSolID ] );
					}

					if( !fReverse && ( sEndSectorB > sEndSectorA ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( fReverse && ( sEndSectorB < sEndSectorA ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
				}
				break;

			case( 5 ):
				iExpiryTime = GetContractExpiryTime( &( Menptr[ gCharactersList[ iCounter ].usSolID ] ) );

				//by contract expiry
				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						break;
					}

					iExpiryTimeA = GetContractExpiryTime( &( Menptr[ gCharactersList[ iCounterA ].usSolID ] ) );

					if( !fReverse && ( iExpiryTimeA > iExpiryTime ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( fReverse && ( iExpiryTimeA < iExpiryTime ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
				}
				break;

			case( 6 ):
				uiID = Menptr[ gCharactersList[ iCounter ].usSolID ].ubID;
				//by ubID
				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					if( gCharactersList[ iCounterA ].fValid == FALSE)
					{
						break;
					}
					uiIDA = Menptr[ gCharactersList[ iCounterA ].usSolID ].ubID;
					if( !fReverse && ( uiIDA > uiID ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
					else if( fReverse && ( uiIDA < uiID ) && ( iCounterA < iCounter ) )
					{
						SwapCharactersInList( iCounter, iCounterA );
					}
				}
				break;

			default:
				Assert(0);
				return;
		}
	}


	if ( fRetainSelectedMercs )
	{
		// select nobody & reset the selected list
		ChangeSelectedInfoChar( -1, TRUE );


		// now select all the soldiers that were selected before
		for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
		{
			if( pSelectedSoldier[ iCounter ] )
			{
				for( iCounterA = 0; iCounterA < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounterA++ )
				{
					// is this entry even valid
					if( gCharactersList[ iCounterA ].fValid == FALSE )
					{
						continue;
					}

					// grab current soldier
					pCurrentSoldier = &Menptr[ gCharactersList[ iCounterA ].usSolID ];

					// check if soldier is active
					if( pCurrentSoldier->bActive == FALSE )
					{
						continue;
					}

					// this guy is selected
					if( pSelectedSoldier[ iCounter ] == pCurrentSoldier )
					{
						SetEntryInSelectedCharacterList( ( INT8 ) iCounterA );
					}

					// update who the currently selected info guy is
					if( pPreviousSelectedInfoChar == pCurrentSoldier )
					{
						ChangeSelectedInfoChar( ( INT8 ) iCounterA, FALSE );
					}
				}
			}
		}
	}
	else
	{
		// keep currently selected merc, but reset the selected list (which isn't saved/restored, that's why)
		ResetSelectedListForMapScreen( );
	}


	// reset blinking animations
	SetAllAutoFacesInactive( );

	// dirty the screen parts affected
	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
}



void SwapCharactersInList( INT32 iCharA, INT32 iCharB )
{
	UINT16 usTempSoldID;

	// swap
	usTempSoldID = gCharactersList[ iCharA ].usSolID;
	gCharactersList[ iCharA ].usSolID =	gCharactersList[ iCharB ].usSolID;
	gCharactersList[ iCharB ].usSolID = usTempSoldID;
}



void RemoveTeamPanelSortButtonsForMapScreen( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < MAX_SORT_METHODS; iCounter++ )
	{
		UnloadButtonImage( giMapSortButtonImage[ iCounter ] );
		RemoveButton( giMapSortButton[ iCounter ] );

		giMapSortButtonImage[ iCounter ] = -1;
		giMapSortButton[ iCounter ] = -1;
	}
	return;
}

void RemoveMPButtonsForMapScreen( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < MAX_MP_BUTTONS+1; iCounter++ )
	{
		UnloadButtonImage( giMapMPButtonImage[ iCounter ] );

		if(iCounter < MAX_MP_BUTTONS)
			RemoveButton( giMapMPButton[ iCounter ] );

		giMapMPButtonImage[ iCounter ] = -1;
		giMapMPButton[ iCounter ] = -1;
	}
	return;
}

void HandleCommonGlowTimer( )
{
	INT32 iCurrentTime = 0;

	// grab the current time
	iCurrentTime = GetJA2Clock();

	// only bother checking once flash interval has elapsed
	if( ( iCurrentTime - giCommonGlowBaseTime ) >= GLOW_DELAY )
	{
		// update timer so that we only run check so often
		giCommonGlowBaseTime = iCurrentTime;

		// set flag to trigger glow higlight updates
		gfGlowTimerExpired = TRUE;
	}
	else
	{
		gfGlowTimerExpired = FALSE;
	}
}


void HandleAssignmentsDoneAndAwaitingFurtherOrders( void )
{
	// run through list of grunts and handle awating further orders
	INT32 iCounter = 0, iCurrentTime = 0;
	SOLDIERTYPE *pSoldier = NULL;


	// update "nothing to do" flags if necessary
	if ( gfReEvaluateEveryonesNothingToDo )
	{
		ReEvaluateEveryonesNothingToDo();
	}

	// grab the current time
	iCurrentTime = GetJA2Clock();

	// only bother checking once flash interval has elapsed
	if( ( iCurrentTime - giFlashAssignBaseTime ) >= ASSIGNMENT_DONE_FLASH_TIME )
	{
		// update timer so that we only run check so often
		giFlashAssignBaseTime = iCurrentTime;

		for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
		{
			if( gCharactersList[ iCounter ].fValid == FALSE )
			{
				break;
			}

			pSoldier = &( Menptr[ gCharactersList[ iCounter ].usSolID ] );

			// toggle and redraw if flash was left ON even though the flag is OFF
			if( pSoldier->flags.fDoneAssignmentAndNothingToDoFlag || fFlashAssignDone )
			{
				fFlashAssignDone = !fFlashAssignDone;
				fDrawCharacterList = TRUE;

				// only need to find one
				break;
			}
		}
	}
}


void DisplayIconsForMercsAsleep( void )
{
	// run throught he list of grunts to see who is asleep and who isn't
	HVOBJECT hHandle;
	INT32 iCounter;
	SOLDIERTYPE *pSoldier;

	// if we are in inventory
	if( fShowInventoryFlag == TRUE )
	{
		return;
	}

	GetVideoObject(&hHandle, guiSleepIcon );
// marke strogg more mercs set max display lines to 18
	for( iCounter = 0; iCounter < maxNumberOfMercVisibleInStrategyList; iCounter++ )
	{
		if( gCharactersList[ iCounter + FIRSTmercTOdisplay ].fValid == TRUE )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter + FIRSTmercTOdisplay ].usSolID ];
			if( pSoldier->bActive && pSoldier->flags.fMercAsleep && CanChangeSleepStatusForSoldier( pSoldier ) )
			{
				//BltVideoObject( guiSAVEBUFFER , hHandle, 0, 125, ( INT16 )( Y_START+(iCounter * ( Y_SIZE + 2 ) ) ) , VO_BLT_SRCTRANSPARENCY,NULL );
				BltVideoObject( guiSAVEBUFFER , hHandle, 0, SLEEP_X + 2, ( INT16 )( Y_START+(iCounter * ( Y_SIZE + 2 ) ) ) , VO_BLT_SRCTRANSPARENCY,NULL );
			}
		}
	}
	return;
}


//Kris:	Added this function to blink the email icon on top of the laptop button whenever we are in
//		mapscreen and we have new email to read.
void CheckForAndRenderNewMailOverlay()
{
	if( fNewMailFlag )
	{
		if( GetJA2Clock() % 1000 < 667 )
		{
			if( ButtonList[ guiMapBottomExitButtons[ MAP_EXIT_TO_LAPTOP ] ]->uiFlags & BUTTON_CLICKED_ON )
			{ //button is down, so offset the icon
				//BltVideoObjectFromIndex( FRAME_BUFFER, guiNewMailIcons, 1, 465, 418, VO_BLT_SRCTRANSPARENCY, NULL );
				BltVideoObjectFromIndex( FRAME_BUFFER, guiNewMailIcons, 1, (SCREEN_WIDTH - 175), (SCREEN_HEIGHT - 62), VO_BLT_SRCTRANSPARENCY, NULL );
				//InvalidateRegion( 465, 418, 480, 428 );
				InvalidateRegion( (SCREEN_WIDTH - 175), (SCREEN_HEIGHT - 62), (SCREEN_WIDTH - 160), (SCREEN_HEIGHT - 52 ));
			}
			else
			{	//button is up, so draw the icon normally
				//BltVideoObjectFromIndex( FRAME_BUFFER, guiNewMailIcons, 0, 464, 417, VO_BLT_SRCTRANSPARENCY, NULL );
				BltVideoObjectFromIndex( FRAME_BUFFER, guiNewMailIcons, 0, (SCREEN_WIDTH - 176), (SCREEN_HEIGHT - 63), VO_BLT_SRCTRANSPARENCY, NULL );
				if( !(ButtonList[ guiMapBottomExitButtons[ MAP_EXIT_TO_LAPTOP ] ]->uiFlags & BUTTON_ENABLED ) )
				{
					UINT32 uiDestPitchBYTES;
					UINT8	*pDestBuf;
					SGPRect area = { (SCREEN_WIDTH - 177), (SCREEN_HEIGHT - 63), (SCREEN_WIDTH - 163), (SCREEN_HEIGHT - 55) };

				pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
					Blt16BPPBufferHatchRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &area );
					UnLockVideoSurface( FRAME_BUFFER );
				}
				InvalidateRegion( (SCREEN_WIDTH - 177), (SCREEN_HEIGHT - 63), (SCREEN_WIDTH - 159), (SCREEN_HEIGHT - 50) );

			}
		}
		else
		{ //The blink is now off, so mark the button dirty so that it'll render next frame.
			MarkAButtonDirty( guiMapBottomExitButtons[ MAP_EXIT_TO_LAPTOP ] );
		}
	}
}



BOOLEAN CanToggleSelectedCharInventory( void )
{
	SOLDIERTYPE *pSoldier = NULL;


	if( gfPreBattleInterfaceActive == TRUE )
	{
		return(FALSE);
	}

	// already in inventory and an item picked up?
	if( fShowInventoryFlag && ( gMPanelRegion.Cursor == EXTERN_CURSOR ) )
	{
		return(FALSE);
	}


	// nobody selected?
	if( bSelectedInfoChar == -1 )
	{
		return(FALSE);
	}

	// does the selected guy have inventory and can we get at it?
	if ( !MapCharacterHasAccessibleInventory( bSelectedInfoChar ) )
	{
		return(FALSE);
	}

	pSoldier = MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ];

	// if not in inventory, and holding an item from sector inventory
	if( !fShowInventoryFlag &&
			( ( gMPanelRegion.Cursor == EXTERN_CURSOR ) || gpItemPointer || fMapInventoryItem ) &&
			( gpItemPointerSoldier == NULL ) )
	{
		// make sure he's in that sector
		if ( ( pSoldier->sSectorX != sSelMapX ) ||
				( pSoldier->sSectorY != sSelMapY ) ||
				( pSoldier->bSectorZ != iCurrentMapSectorZ ) ||
				pSoldier->flags.fBetweenSectors )
		{
			return(FALSE);
		}
	}


	// passed!
	return(TRUE);
}



BOOLEAN MapCharacterHasAccessibleInventory( INT8 bCharNumber )
{
	SOLDIERTYPE *pSoldier = NULL;


	Assert( bCharNumber >= 0 );
	Assert( bCharNumber < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS );

	// invalid character slot selected?
	if( gCharactersList[ bCharNumber ].fValid == FALSE )
	{
		return(FALSE);
	}

	pSoldier = MercPtrs[ gCharactersList[ bCharNumber ].usSolID ];

	if( ( pSoldier->bAssignment == IN_TRANSIT ) ||
			( pSoldier->bAssignment == ASSIGNMENT_POW ) ||
				// Kaiden: Vehicle Inventory change - Commented the following line
				// ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ||
				// And added this instead:
			( (!gGameExternalOptions.fVehicleInventory) && (pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) ) ||
			( AM_A_ROBOT( pSoldier ) ) ||
			( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__EPC ) ||
			( pSoldier->stats.bLife < OKLIFE )
		)
	{
		return(FALSE);
	}

	return( TRUE );
}



void CheckForInventoryModeCancellation()
{
	if ( fShowInventoryFlag || fShowDescriptionFlag )
	{
		// can't bail while player has an item in hand...
		if( gMPanelRegion.Cursor == EXTERN_CURSOR )
		{
			return;
		}

		if ( !CanToggleSelectedCharInventory( ) )
		{
			// get out of inventory mode if it's on!	(could have just bled below OKLIFE)
			if ( fShowInventoryFlag )
			{
				fShowInventoryFlag = FALSE;
				SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );
				fTeamPanelDirty = TRUE;
			}

			// get out of inventory description if it's on!
			if ( fShowDescriptionFlag )
			{
				DeleteItemDescriptionBox( );
			}
		}
	}
}


void ChangeSelectedMapSector( INT16 sMapX, INT16 sMapY, INT8 bMapZ )
{
	// ignore while map inventory pool is showing, or else items can be replicated, since sector inventory always applies
	// only to the currently selected sector!!!
	if( fShowMapInventoryPool )
		return;

	if ( gfPreBattleInterfaceActive )
		return;

	if( !IsTheCursorAllowedToHighLightThisSector( sMapX, sMapY ) )
		return;

	// disallow going underground while plotting (surface) movement
	if ( ( bMapZ != 0 ) && ( ( bSelectedDestChar != -1 ) || fPlotForHelicopter ) )
		return;


	sSelMapX = sMapX;
	sSelMapY = sMapY;
	iCurrentMapSectorZ = bMapZ;

	// if going underground while in airspace mode
	if ( ( bMapZ > 0 ) && ( fShowAircraftFlag == TRUE ) )
	{
		// turn off airspace mode
		ToggleAirspaceMode( );
	}

	fMapPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;

	// also need this, to update the text coloring of mercs in this sector
	fTeamPanelDirty = TRUE;
}



BOOLEAN CanChangeDestinationForCharSlot( INT8 bCharNumber, BOOLEAN fShowErrorMessage )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT8 bErrorNumber = -1;


	if ( bCharNumber == -1 )
		return( FALSE );

	if ( gCharactersList[ bCharNumber ].fValid == FALSE )
		return (FALSE);


	pSoldier = MercPtrs[ gCharactersList[ bCharNumber ].usSolID ];

	// valid soldier?
	Assert( pSoldier );
	Assert( pSoldier->bActive );


	if ( CanEntireMovementGroupMercIsInMove( pSoldier, &bErrorNumber ) )
	{
		return( TRUE );
	}
	else
	{
		// function may fail without returning any specific error # (-1).
		// if it gave us the # of an error msg, and we were told to display it
		if ( ( bErrorNumber != -1 ) && fShowErrorMessage )
		{
			ReportMapScreenMovementError( bErrorNumber );
		}

		return( FALSE );
	}
}



BOOLEAN CanExtendContractForCharSlot( INT8 bCharNumber )
{
	SOLDIERTYPE *pSoldier = NULL;


	if ( bCharNumber == -1 )
		return( FALSE );

	if ( gCharactersList[ bCharNumber ].fValid == FALSE )
		return (FALSE);

	pSoldier = MercPtrs[ gCharactersList[ bCharNumber ].usSolID ];

	// valid soldier?
	Assert( pSoldier );
	Assert( pSoldier->bActive );

	// if a vehicle, in transit, or a POW
	if( /*( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ||*/
			( pSoldier->bAssignment == IN_TRANSIT ) ||
			( pSoldier->bAssignment == ASSIGNMENT_POW ) )
	{
		// can't extend contracts at this time
		return (FALSE);
	}

	// if a vehicle has passengers
	if (	(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE) &&
			(DoesVehicleHaveAnyPassengers(pSoldier->bVehicleID) ) )
	{
		// then restrict contract menu
		return (FALSE);
	}

	// mercs below OKLIFE, M.E.R.C. mercs, EPCs, and the Robot use the Contract menu so they can be DISMISSED/ABANDONED!

	// everything OK
	return( TRUE );
}


BOOLEAN CanChangeSleepStatusForCharSlot( INT8 bCharNumber )
{
	SOLDIERTYPE *pSoldier = NULL;

	if ( bCharNumber == -1 )
		return( FALSE );

	if ( gCharactersList[ bCharNumber ].fValid == FALSE )
		return (FALSE);


	pSoldier = MercPtrs[ gCharactersList[ bCharNumber ].usSolID ];

	return( CanChangeSleepStatusForSoldier( pSoldier ) );
}


BOOLEAN CanChangeSleepStatusForSoldier( SOLDIERTYPE *pSoldier )
{
	// valid soldier?
	Assert( pSoldier );
	Assert( pSoldier->bActive );

	// if a vehicle, robot, in transit, or a POW
	if( ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) || AM_A_ROBOT( pSoldier ) ||
			( pSoldier->bAssignment == IN_TRANSIT ) || ( pSoldier->bAssignment == ASSIGNMENT_POW ) )
	{
		// can't change the sleep status of such mercs
		return ( FALSE );
	}

	// if dead
	if( ( pSoldier->stats.bLife <= 0 ) || ( pSoldier->bAssignment == ASSIGNMENT_DEAD ) )
	{
		return ( FALSE );
	}

	// this merc MAY be able to sleep/wake up - we'll allow player to click and find out
	return( TRUE );
}


void ChangeMapScreenMaskCursor( UINT16 usCursor )
{
	MSYS_SetCurrentCursor( usCursor );
	MSYS_ChangeRegionCursor( &gMapScreenMaskRegion, usCursor );

	if ( usCursor == CURSOR_CHECKMARK )
		fCheckCursorWasSet = TRUE;
	else
		fCheckCursorWasSet = FALSE;

	if ( usCursor == CURSOR_NORMAL )
	{
	if ( !InItemStackPopup( ) )
	{
		// cancel mouse restriction
		FreeMouseCursor();
	}
	}
	else
	{
		// restrict mouse cursor to the map area
		RestrictMouseCursor( &MapScreenRect );
	}
}



void CancelOrShortenPlottedPath( void )
{
	INT16 sMapX, sMapY;
	UINT32 uiReturnValue;


	GetMouseMapXY(&sMapX, &sMapY);

/*
	// translate zoom in to zoom out coords
	if(fZoomFlag)
	{
		sMapX=(UINT16)iZoomX/MAP_GRID_X+sMapX;
		sMapX=sMapX/2;
		sMapY=(UINT16)iZoomY/MAP_GRID_Y+sMapY;
		sMapY=sMapY/2;
	}
*/

	// check if we are in aircraft mode
	if( fShowAircraftFlag == TRUE )
	{
		// check for helicopter path being plotted
		if( !fPlotForHelicopter )
			return;

		// if player can't redirect it
		if ( CanHelicopterFly( ) == FALSE )
		{
			// explain & ignore
			ExplainWhySkyriderCantFly();
			return;
		}


		// try to delete portion of path AFTER the current sector for the helicopter
		uiReturnValue = ClearPathAfterThisSectorForHelicopter( sMapX, sMapY );
	}
	else
	{
		// check for character path being plotted
		if( bSelectedDestChar == -1 )
			return;

		// try to delete portion of path AFTER the current sector for the helicopter
		uiReturnValue = ClearPathAfterThisSectorForCharacter( &Menptr[gCharactersList[bSelectedDestChar].usSolID], sMapX, sMapY );
	}


	switch ( uiReturnValue )
	{
		case ABORT_PLOTTING:
			AbortMovementPlottingMode( );
			break;

		case PATH_CLEARED:	// movement was canceled
			// message was already issued when path was cleared
			DestinationPlottingCompleted();
			break;

		case PATH_SHORTENED:	// route was shortened but isn't completely gone
			// display "route shortened" message
			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 4 ] );
			break;

		default:
			Assert( FALSE );
			break;
	}

	// this triggers the path node animation to reset itself back to the first node
	fDeletedNode = TRUE;

	fMapPanelDirty = TRUE;

	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;		// to update ETAs if path reversed or shortened
}


BOOLEAN HandleCtrlOrShiftInTeamPanel( INT8 bCharNumber, BOOLEAN fFromRightClickAssignments )
{
	// HEADROCK HAM B2.8: New condition based on new argument.
	if (fFromRightClickAssignments)
	{
		if ( _KeyDown(CTRL) || _KeyDown(SHIFT) )
		{
			if ( fSelectedListOfMercsForMapScreen[ bCharNumber ] == FALSE )
			{
				SetEntryInSelectedCharacterList( bCharNumber );

				INT16 iCounter;

				for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
				{
					if( gCharactersList[ iCounter ].fValid == TRUE )
					{
						// if not already selected
						if( fSelectedListOfMercsForMapScreen[ iCounter ] == FALSE )
						{
							SOLDIERTYPE * pSelected = &( Menptr[ gCharactersList[ bCharNumber ].usSolID ] );
							SOLDIERTYPE * pSoldier = &( Menptr[ gCharactersList[ iCounter ].usSolID ] );

							// if on a squad, or in a vehicle, or IS a vehicle
							if ( pSoldier->bAssignment == VEHICLE )
							{
								// and a member of that squad or vehicle is selected, or we are a vehicle
								if ( (pSelected->bAssignment == VEHICLE && pSoldier->iVehicleId == pSelected->iVehicleId) ||
									(pSelected->flags.uiStatusFlags & SOLDIER_VEHICLE && pSoldier->iVehicleId == pSelected->bVehicleID ) ||
									(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE && pSelected->iVehicleId == pSoldier->bVehicleID ) )
								{
									// then also select this guy
									SetEntryInSelectedCharacterList( ( INT8 ) iCounter );
								}
							}

							else if ( pSoldier->bAssignment == pSelected->bAssignment &&
								(pSoldier->bAssignment == TRAIN_TEAMMATE || pSoldier->bAssignment == TRAIN_BY_OTHER) &&
								pSoldier->sSectorX == pSelected->sSectorX &&
								pSoldier->sSectorY == pSelected->sSectorY &&
								pSoldier->sZLevel == pSelected->sZLevel )
							{
								// make sure only trainers/trainees of the same stat are selected together.
								if (pSoldier->bTrainStat == pSelected->bTrainStat)
								{
									SetEntryInSelectedCharacterList( ( INT8 ) iCounter );
								}
							}
								
							else if ( pSoldier->bAssignment == pSelected->bAssignment &&
								pSoldier->sSectorX == pSelected->sSectorX &&
								pSoldier->sSectorY == pSelected->sSectorY &&
								pSoldier->sZLevel == pSelected->sZLevel )
							{
								SetEntryInSelectedCharacterList( ( INT8 ) iCounter );
							}
						}
					}
				}
			}

			else
			{
				// Deselect entire group
				ResetEntryForSelectedList( bCharNumber );

				INT16 iCounter;

				for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
				{
					if( gCharactersList[ iCounter ].fValid == TRUE )
					{
						// if not already selected
						if( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE )
						{
							SOLDIERTYPE * pSelected = &( Menptr[ gCharactersList[ bCharNumber ].usSolID ] );
							SOLDIERTYPE * pSoldier = &( Menptr[ gCharactersList[ iCounter ].usSolID ] );

							// if on a squad, or in a vehicle, or IS a vehicle
							if ( pSoldier->bAssignment == VEHICLE )
							{
								// and a member of that squad or vehicle is selected, or we are a vehicle
								if ( pSoldier->iVehicleId == pSelected->iVehicleId ||
									(pSelected->flags.uiStatusFlags & SOLDIER_VEHICLE && pSoldier->iVehicleId == pSelected->bVehicleID ) ||
									(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE && pSelected->iVehicleId == pSoldier->bVehicleID ) )
								{
									// then also select this guy
									ResetEntryForSelectedList( ( INT8 ) iCounter );
								}
							}

							else if ( pSoldier->bAssignment == pSelected->bAssignment &&
								(pSoldier->bAssignment == TRAIN_TEAMMATE || pSoldier->bAssignment == TRAIN_BY_OTHER) &&
								pSoldier->sSectorX == pSelected->sSectorX &&
								pSoldier->sSectorY == pSelected->sSectorY &&
								pSoldier->sZLevel == pSelected->sZLevel )
							{
								// make sure only trainers/trainees of the same stat are selected together.
								if (pSoldier->bTrainStat == pSelected->bTrainStat)
								{
									ResetEntryForSelectedList( ( INT8 ) iCounter );
								}
							}
								
							else if ( pSoldier->bAssignment == pSelected->bAssignment &&
								pSoldier->sSectorX == pSelected->sSectorX &&
								pSoldier->sSectorY == pSelected->sSectorY &&
								pSoldier->sZLevel == pSelected->sZLevel )
							{
								ResetEntryForSelectedList( ( INT8 ) iCounter );
							}
						}
					}
				}
			}

			fTeamPanelDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;

			return(TRUE);
		}

		return (FALSE);
	}
	else
	{

		// check if shift or ctrl held down, if so, set values in list
		if (_KeyDown(CTRL))
		{
			ToggleEntryInSelectedList( bCharNumber );

			fTeamPanelDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;

			return( TRUE);
		}
		else if( _KeyDown(SHIFT) )
		{
			// build a list from the bSelectedInfoChar To here, reset everyone

			//empty the list
			ResetSelectedListForMapScreen( );
			// rebuild the list
			BuildSelectedListFromAToB( bSelectedInfoChar, bCharNumber );

			fTeamPanelDirty = TRUE;
			fCharacterInfoPanelDirty = TRUE;

			return( TRUE );
		}

		return( FALSE );
	}
}


INT32 GetContractExpiryTime( SOLDIERTYPE *pSoldier )
{
	if( ( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC ) || ( pSoldier->ubProfile == SLAY ) )
	{
		return ( pSoldier->iEndofContractTime );
	}
	else
	{
		// never - really high number
		return ( 999999 );
	}
}



void ChangeSelectedInfoChar( INT8 bCharNumber, BOOLEAN fResetSelectedList )
{
	Assert( ( bCharNumber >= -1 ) && ( bCharNumber < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS ) );

	if( ( bCharNumber != -1 ) && ( gCharactersList[ bCharNumber ].fValid == FALSE ) )
		return;


	// CHRISL: The result of this block of code is that if we're holding an item, not only does the originally selected merc stay selected,
	//	but the merc who happens to hold the originally selected mercs position in the new sort order will ALSO be selected.
	// if holding an item
	//if ( ( gMPanelRegion.Cursor == EXTERN_CURSOR ) || gpItemPointer || fMapInventoryItem )
	//{
		// make sure we can give it to this guy, otherwise don't allow the change
	//	if ( !MapscreenCanPassItemToCharNum( bCharNumber ) )
	//	{
	//		return;
	//	}
	//}


	if ( fResetSelectedList )
	{
		// reset selections of all listed characters.	Do this even if this guy is already selected.
		// NOTE: this keeps the currently selected info char selected
		ResetSelectedListForMapScreen( );
	}


	// if this is really a change
	if ( bSelectedInfoChar != bCharNumber )
	{
		// if resetting, and another guy was selected
		if ( fResetSelectedList && ( bSelectedInfoChar != -1 ) )
		{
			// deselect previously selected character
			ResetEntryForSelectedList( bSelectedInfoChar );
		}

		bSelectedInfoChar = bCharNumber;

		if ( bCharNumber != -1 )
		{
			// the selected guy must always be ON in the list of selected chars
			SetEntryInSelectedCharacterList( bCharNumber );
		}

		// if we're in the inventory panel
		if ( fShowInventoryFlag )
		{
			// and we're changing to nobody or a guy whose inventory can't be accessed
			if ( ( bCharNumber == -1 ) || !MapCharacterHasAccessibleInventory( bCharNumber ) )
			{
				// then get out of inventory mode
				fShowInventoryFlag = FALSE;
			}
		}

		fCharacterInfoPanelDirty = TRUE;

		// if showing sector inventory
		if ( fShowMapInventoryPool )
		{
			// redraw right side to update item hatches
			fMapPanelDirty = TRUE;
		}
	}

	// marke strogg more mercs
	// we have to adapt FIRSTmercTOdisplay bCharNumber is 0 to 90 any merc
	if ( bCharNumber >= 0 )
	{
		if (bCharNumber < FIRSTmercTOdisplay)
		{
			FIRSTmercTOdisplay = bCharNumber;
		}
		else if (bCharNumber > FIRSTmercTOdisplay + (maxNumberOfMercVisibleInStrategyList - 1) )
		{
			// WANNE: This check fixes the wrong offset when hiring the FIRST merc!
			if ((maxNumberOfMercVisibleInStrategyList - 1) >= 0)
				FIRSTmercTOdisplay = bCharNumber - (maxNumberOfMercVisibleInStrategyList - 1);
			else
				FIRSTmercTOdisplay = 0;
		}
	}
// now this should do the trick
// marke strogg end

	fTeamPanelDirty = TRUE;
}



void CopyPathToAllSelectedCharacters( PathStPtr pPath )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	// run through list and copy paths for each selected character
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

			// skip itself!
			if ( GetSoldierMercPathPtr( pSoldier ) != pPath )
			{
				if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
				{
					pVehicleList[ pSoldier->bVehicleID ].pMercPath = CopyPaths( pPath, pVehicleList[ pSoldier->bVehicleID ].pMercPath );
				}
				else if( pSoldier->bAssignment == VEHICLE )
				{
					pVehicleList[ pSoldier->iVehicleId ].pMercPath = CopyPaths( pPath, pVehicleList[ pSoldier->iVehicleId ].pMercPath );
				}
				else
				{
					pSoldier->pMercPath = CopyPaths( pPath, pSoldier->pMercPath );
				}

				// don't use CopyPathToCharactersSquadIfInOne(), it will whack the original pPath by replacing that merc's path!
			}
		}
	}
}



void CancelPathsOfAllSelectedCharacters()
{
	INT8 bCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	BOOLEAN fSkyriderMsgShown = FALSE;


	// cancel destination for the clicked and ALL other valid & selected characters with a route set
	for( bCounter = 0; bCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; bCounter++ )
	{
		// if we've clicked on a selected valid character
		if( ( gCharactersList[ bCounter ].fValid == TRUE ) && IsEntryInSelectedListSet( bCounter ) )
		{
			pSoldier = MercPtrs[ gCharactersList[ bCounter ].usSolID ];

			// and he has a route set
			if ( GetLengthOfMercPath( pSoldier ) > 0 )
			{
				// if he's in the chopper, but player can't redirect it
				if ( ( pSoldier->bAssignment == VEHICLE ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) && ( CanHelicopterFly( ) == FALSE ) )
				{
					if ( !fSkyriderMsgShown )
					{
						// explain
						ExplainWhySkyriderCantFly();
						fSkyriderMsgShown = TRUE;
					}

					// don't cancel, ignore
					continue;
				}


				// cancel the entire path (also clears vehicles for any passengers selected, and handles reversing directions)
				if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
				{
					CancelPathForVehicle( &( pVehicleList[ pSoldier->bVehicleID ] ), FALSE );
				}
				else
				{
					CancelPathForCharacter( pSoldier );
				}
			}
		}
	}
}



void ConvertMinTimeToETADayHourMinString( UINT32 uiTimeInMin, STR16 sString )
{
	UINT32 uiDay, uiHour, uiMin;

	uiDay	= ( uiTimeInMin / NUM_MIN_IN_DAY );
	uiHour = ( uiTimeInMin - ( uiDay * NUM_MIN_IN_DAY ) ) / NUM_MIN_IN_HOUR;
	uiMin	= uiTimeInMin - ( ( uiDay * NUM_MIN_IN_DAY ) + ( uiHour * NUM_MIN_IN_HOUR ) );

	// there ain't enough room to show both the day and ETA: and without ETA it's confused as the current time
//	swprintf( sString, L"%s %s %d, %02d:%02d", pEtaString[ 0 ], pDayStrings[ 0 ], uiDay, uiHour, uiMin );
//	swprintf( sString, L"%s %d, %02d:%02d", pDayStrings[ 0 ], uiDay, uiHour, uiMin );
	swprintf( sString, L"%s %02d:%02d", pEtaString[ 0 ], uiHour, uiMin );
}



INT32 GetGroundTravelTimeOfCharacter( INT8 bCharNumber )
{
	INT32 iTravelTime = 0;

	if( bCharNumber == -1 )
		return(0);

	if ( !gCharactersList[ bCharNumber ].fValid )
		return(0);

	// get travel time for the last path segment (stored in pTempCharacterPath)
	iTravelTime = GetPathTravelTimeDuringPlotting( pTempCharacterPath );

	// add travel time for any prior path segments (stored in the selected character's mercpath, but waypoints aren't built)
	iTravelTime += GetPathTravelTimeDuringPlotting( GetSoldierMercPathPtr( MercPtrs[ gCharactersList[ bCharNumber ].usSolID ] ) );

	return( iTravelTime );
}



INT16 CalcLocationValueForChar( INT32 iCounter )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT16 sLocValue = 0;

	Assert( iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS );

	if( gCharactersList[ iCounter ].fValid == FALSE )
		return( sLocValue );

	pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

	// don't reveal location of POWs!
	if( pSoldier->bAssignment != ASSIGNMENT_POW )
	{
		sLocValue = SECTOR( pSoldier->sSectorX, pSoldier->sSectorY );
		// underground: add 1000 per sublevel
		sLocValue += 1000 * ( pSoldier->bSectorZ );
	}

	return( sLocValue );
}



void CancelChangeArrivalSectorMode( )
{
	// "put him in change arrival sector" mode
	gfInChangeArrivalSectorMode = FALSE;

	// change the cursor to that mode
	SetUpCursorForStrategicMap();

	fMapPanelDirty = TRUE;
}




void MakeMapModesSuitableForDestPlotting( INT8 bCharNumber )
{
	SOLDIERTYPE *pSoldier = NULL;


	if( gCharactersList[ bCharNumber ].fValid == TRUE )
	{
		pSoldier = MercPtrs[ gCharactersList[ bCharNumber ].usSolID ];

		CancelSectorInventoryDisplayIfOn( FALSE );

		TurnOnShowTeamsMode( );

		if ( ( pSoldier->bAssignment == VEHICLE ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			if ( fShowAircraftFlag == FALSE )
			{
				// turn on airspace mode automatically
				ToggleAirspaceMode();
			}
		}
		else
		{
			if ( fShowAircraftFlag == TRUE )
			{
				// turn off airspace mode automatically
				ToggleAirspaceMode();
			}
		}

		// if viewing a different sublevel
		if ( iCurrentMapSectorZ != pSoldier->bSectorZ )
		{
			// switch to that merc's sublevel
			JumpToLevel( pSoldier->bSectorZ );
		}
	}
}



BOOLEAN AnyMovableCharsInOrBetweenThisSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ )
{
	INT32 iFirstId = 0, iLastId = 0;
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;


	// to speed it up a little?
	iFirstId = gTacticalStatus.Team[ OUR_TEAM ].bFirstID;
	iLastId = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	for( iCounter = iFirstId; iCounter <= iLastId; iCounter++ )
	{
		// get the soldier
		pSoldier = &Menptr[ iCounter ];

		// is the soldier active
		if( pSoldier->bActive == FALSE )
		{
			continue;
		}

		// POWs, dead guys, guys in transit can't move
		if ( ( pSoldier->bAssignment == IN_TRANSIT ) ||
				( pSoldier->bAssignment == ASSIGNMENT_POW ) ||
				( pSoldier->bAssignment == ASSIGNMENT_DEAD ) ||
				( pSoldier->stats.bLife == 0 ) )
		{
			continue;
		}

		// don't count mercs aboard Skyrider
		if ( ( pSoldier->bAssignment == VEHICLE ) && ( pSoldier->iVehicleId == iHelicopterVehicleId ) )
		{
			continue;
		}

		// don't count vehicles - in order for them to move, somebody must be in the sector with them
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
		{
			continue;
		}


		// is he here?
		if( ( pSoldier->sSectorX == sSectorX ) && ( pSoldier->sSectorY == sSectorY ) && ( pSoldier->bSectorZ == bSectorZ ) )
		{
			// NOTE that we consider mercs between sectors, mercs < OKLIFE, and sleeping mercs to be "movable".
			// This lets CanCharacterMoveInStrategic() itself report the appropriate error message when character is clicked
			return( TRUE );
		}
	}


	return( FALSE );
}



BOOLEAN RequestGiveSkyriderNewDestination( void )
{
	// should we allow it?
	if( CanHelicopterFly( ) == TRUE )
	{
		// if not warned already, and chopper empty, but mercs are in this sector
		if ( !gfSkyriderEmptyHelpGiven &&
			( GetNumberOfPassengersInHelicopter() == 0 ) &&
			( PlayerMercsInHelicopterSector() > 0 ) )
		{
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, pSkyriderText[ 6 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
			gfSkyriderEmptyHelpGiven = TRUE;
			return( FALSE );
		}

		// say Yo!
		if(gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE) SkyRiderTalk( SKYRIDER_SAYS_HI );

		// start plotting helicopter movement
		fPlotForHelicopter = TRUE;

		// change cursor to the helicopter
		SetUpCursorForStrategicMap();

		// remember the helicopter's current path so we can restore it if need be
		gpHelicopterPreviousMercPath = CopyPaths( pVehicleList[ iHelicopterVehicleId ].pMercPath, gpHelicopterPreviousMercPath );

		// affects Skyrider's dialogue
		SetFactTrue( FACT_SKYRIDER_USED_IN_MAPSCREEN );

		return( TRUE );
	}
	else // not allowed to reroute the chopper right now
	{
		// tell player why not
		ExplainWhySkyriderCantFly();

		return( FALSE );
	}
}



void ExplainWhySkyriderCantFly( void )
{
	// do we owe him money?
	if( gMercProfiles[ SKYRIDER ].iBalance < 0 )
	{
		// overdue cash
		SkyRiderTalk( OWED_MONEY_TO_SKYRIDER );
		return;
	}

	// is he returning to base?
	if( fHeliReturnStraightToBase )
	{
		// returning to base
		SkyRiderTalk( RETURN_TO_BASE );
		return;
	}

	// grounded by enemies in sector?
	if ( CanHelicopterTakeOff() == FALSE )
	{
		SkyRiderTalk( CHOPPER_NOT_ACCESSIBLE );
//		DoMapMessageBox( MSG_BOX_BASIC_STYLE, pSkyriderText[ 4 ], MAP_SCREEN, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
		return;
	}

	// Drassen too disloyal to wanna help player?
	if ( CheckFact( FACT_LOYALTY_LOW, SKYRIDER ) )
	{
		SkyRiderTalk( DOESNT_WANT_TO_FLY );
		return;
	}

	// no explainable reason
}


UINT8 PlayerMercsInHelicopterSector( void )
{
	GROUP *pGroup = NULL;

	Assert( iHelicopterVehicleId != -1);
	pGroup = GetGroup( pVehicleList[ iHelicopterVehicleId ].ubMovementGroup );
	AssertNotNIL(pGroup);

	if ( pGroup->fBetweenSectors )
	{
		return( 0 );
	}

	return( PlayerMercsInSector( pGroup->ubSectorX, pGroup->ubSectorY, 0 ) );
}



void HandleNewDestConfirmation( INT16 sMapX, INT16 sMapY )
{
	UINT8 ubCurrentProgress;


	// if moving the chopper itself, or moving a character aboard the chopper
	if( fPlotForHelicopter )
	{
		// if there are no enemies in destination sector, or we don't know
		if ( ( NumEnemiesInSector( sMapX, sMapY ) == 0 ) ||
				(	WhatPlayerKnowsAboutEnemiesInSector( sMapX, sMapY ) == KNOWS_NOTHING ) )
		{
			// no problem

			// get current player progress
			ubCurrentProgress = CurrentPlayerProgressPercentage();

			// if we're doing a lot better than last time he said anything
			if ( ( ubCurrentProgress > gubPlayerProgressSkyriderLastCommentedOn ) &&
					( ( ubCurrentProgress - gubPlayerProgressSkyriderLastCommentedOn ) >= MIN_PROGRESS_FOR_SKYRIDER_QUOTE_DOING_WELL ) )
			{
				// kicking ass!
				SkyRiderTalk( THINGS_ARE_GOING_WELL );

				gubPlayerProgressSkyriderLastCommentedOn = ubCurrentProgress;
			}
			// if we're doing noticably worse than last time he said anything
			else if ( ( ubCurrentProgress < gubPlayerProgressSkyriderLastCommentedOn ) &&
					( ( gubPlayerProgressSkyriderLastCommentedOn - ubCurrentProgress ) >= MIN_REGRESS_FOR_SKYRIDER_QUOTE_DOING_BADLY ) )
			{
				// sucking rocks!
				SkyRiderTalk( THINGS_ARE_GOING_BADLY );

				gubPlayerProgressSkyriderLastCommentedOn = ubCurrentProgress;
			}
			else
			{
				// ordinary confirmation quote
				if(gGameSettings.fOptions[ TOPTION_SILENT_SKYRIDER ] == FALSE) SkyRiderTalk( CONFIRM_DESTINATION );
			}
		}
		//CHRISL: If we're allowing Skyrider to drop mercs into a hot LZ, we don't need Skyrider telling us he isn't going to drop the mercs off.
		else if(gGameExternalOptions.ubSkyriderHotLZ == 0)
		{
			// ok, but... you know there are enemies there...
			SkyRiderTalk( BELIEVED_ENEMY_SECTOR );
		}
	}
	else
	{
		RandomAwakeSelectedMercConfirmsStrategicMove( );

		// tell player the route was CONFIRMED
		// NOTE: We don't this this for the helicopter any more, since it clashes with Skyrider's own confirmation msg
		MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pMapPlotStrings[ 1 ] );

		// wake up anybody who needs to be awake to travel
		WakeUpAnySleepingSelectedMercsOnFootOrDriving();
	}
}



void RandomAwakeSelectedMercConfirmsStrategicMove( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter;
// WDS - make number of mercenaries, etc. be configurable
	UINT8	ubSelectedMercID[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
	UINT8	ubSelectedMercIndex[ CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS ];
	UINT8	ubNumMercs = 0;
	UINT8	ubChosenMerc;


	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( ( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE ) )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

			if ( pSoldier->stats.bLife >= OKLIFE && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) &&
						!AM_A_ROBOT( pSoldier ) && !AM_AN_EPC( pSoldier ) && !pSoldier->flags.fMercAsleep )
			{
				ubSelectedMercID[ ubNumMercs ] = pSoldier->ubID;
				ubSelectedMercIndex[ ubNumMercs ] = (UINT8)iCounter;

				ubNumMercs++;
			}
		}
	}

	if ( ubNumMercs > 0 )
	{
		ubChosenMerc = (UINT8)Random( ubNumMercs );

		// select that merc so that when he speaks we're showing his portrait and not someone else
		ChangeSelectedInfoChar( ubSelectedMercIndex[ ubChosenMerc ], FALSE );

		MercPtrs[ ubSelectedMercID[ ubChosenMerc ] ]->DoMercBattleSound( ( UINT8 ) ( Random(2) ? BATTLE_SOUND_OK1 : BATTLE_SOUND_OK2 ) );
		//TacticalCharacterDialogue( MercPtrs[ ubSelectedMercID[ ubChosenMerc ] ], ubQuoteNum );
	}
}


void DestinationPlottingCompleted( void )
{
	// clear previous paths for selected characters and helicopter
	ClearPreviousPaths();

	fPlotForHelicopter = FALSE;
	bSelectedDestChar = - 1;
	giDestHighLine = -1;

	fMapPanelDirty = TRUE;

	// reset cursor
	SetUpCursorForStrategicMap( );

	fJustFinishedPlotting = TRUE;
	fEndPlotting = TRUE;
}



void HandleMilitiaRedistributionClick( void )
{
	INT8 bTownId;
	BOOLEAN fTownStillHidden;
	CHAR16 sString[ 128 ];


	// if on the surface
	if ( iCurrentMapSectorZ == 0 )
	{
		bTownId = GetTownIdForSector( sSelMapX, sSelMapY );
		fTownStillHidden = ( ( bTownId == TIXA ) && !fFoundTixa ) || ( ( bTownId == ORTA ) && !fFoundOrta );

		if( ( bTownId != BLANK_SECTOR ) && !fTownStillHidden )
		{
			if ( MilitiaTrainingAllowedInSector( sSelMapX, sSelMapY, ( INT8 )iCurrentMapSectorZ ) )
			{
				if( fShowTownInfo == TRUE )
				{
					fShowTownInfo = FALSE;
				}
				fMapPanelDirty = TRUE;

				// check if there's combat in any of the town's sectors
				if ( CanRedistributeMilitiaInSector( sSelMapX, sSelMapY, bTownId ) )
				{
					// Nope, ok, set selected militia town
					sSelectedMilitiaTown = bTownId;
				}
				else
				{
					// can't redistribute militia during combat!
					DoScreenIndependantMessageBox( pMilitiaString[ 2 ], MSG_BOX_FLAG_OK, NULL );
				}
			}
			else
			{
				// can't have militia in this town
				swprintf( sString, pMapErrorString[ 31 ], pTownNames [ bTownId ] );
				DoScreenIndependantMessageBox( sString, MSG_BOX_FLAG_OK, NULL );
			}
		}
		else if( IsThisSectorASAMSector( sSelMapX, sSelMapY, 0 ) && fSamSiteFound[ GetSAMIdFromSector( sSelMapX, sSelMapY, 0 ) ] )
		{
			// can't move militia around sam sites
			DoScreenIndependantMessageBox( pMapErrorString[ 30 ], MSG_BOX_FLAG_OK, NULL );
		}
	}
}



#ifdef JA2TESTVERSION
void DumpSectorDifficultyInfo( void )
{
	// NOTE: This operates on the selected map sector!
	CHAR16 wSectorName[ 128 ];

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Playing Difficulty: %s", gzGIOScreenText[ GIO_DIF_LEVEL_TEXT + gGameOptions.ubDifficultyLevel ] );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Highest Progress (0-100) = %d%%", HighestPlayerProgressPercentage() );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Player Kills = %d", gStrategicStatus.usPlayerKills );

	GetSectorIDString( sSelMapX, sSelMapY, ( INT8 ) iCurrentMapSectorZ, wSectorName, TRUE );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"SECTOR: %s", wSectorName );

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Pyth. Distance From Meduna (0-20) = %d", GetPythDistanceFromPalace( sSelMapX, sSelMapY ) );

	if ( ( gWorldSectorX == sSelMapX ) && ( gWorldSectorY == sSelMapY ) && ( gbWorldSectorZ == iCurrentMapSectorZ ) )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Enemy Difficulty Factor (0 to 100) = %d%%", CalcDifficultyModifier( SOLDIER_CLASS_ARMY ) );
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"Avg Regular Enemy Exp. Level (2-6) = %d", 2 + ( CalcDifficultyModifier( SOLDIER_CLASS_ARMY ) / 20 ) );
	}
	else
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"--Must load sector to calculate difficulty--" );
	}
}
#endif



void StartChangeSectorArrivalMode( void )
{
	// "put him in change arrival sector" mode
	gfInChangeArrivalSectorMode = TRUE;

	// redraw map with bullseye removed
	fMapPanelDirty = TRUE;

	// change the cursor to that mode
	SetUpCursorForStrategicMap();

	// give instructions as overlay message
	MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pBullseyeStrings[ 0 ] );
}



BOOLEAN CanMoveBullseyeAndClickedOnIt( INT16 sMapX, INT16 sMapY )
{
	// if in airspace mode, and not plotting paths
	if( ( fShowAircraftFlag == TRUE ) && ( bSelectedDestChar == -1 ) && ( fPlotForHelicopter == FALSE ) )
	{
		if (is_networked)
		{
			// haydent
			// OJW 080101 - allow map change before laptop unlock
			if ((!is_server && !is_client) || (is_server && !allowlaptop))
			{
				// if he clicked on the bullseye, and we're on the surface level
				if ( ( sMapX == gsMercArriveSectorX ) && ( sMapY == gsMercArriveSectorY ) && ( iCurrentMapSectorZ == 0 ) )
				{
					return( TRUE );
				}
			}

			// haydent
			else if(is_server)
			{
				ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPServerMessage[9] );
			}
			else if(is_client)
				ScreenMsg( FONT_LTGREEN, MSG_MPSYSTEM, MPClientMessage[38] );
		}
		else
		{
			// don't allow moving bullseye until after initial arrival
			if ( gTacticalStatus.fDidGameJustStart == FALSE )
			{
				// if he clicked on the bullseye, and we're on the surface level
				if ( ( sMapX == gsMercArriveSectorX ) && ( sMapY == gsMercArriveSectorY ) && ( iCurrentMapSectorZ == 0 ) )
				{
					return( TRUE );
				}
			}
		}
	}

	return( FALSE );
}



void CreateBullsEyeOrChopperSelectionPopup( void )
{
	wcscpy( gzUserDefinedButton1, pHelicopterEtaStrings[ 8 ] );
	wcscpy( gzUserDefinedButton2, pHelicopterEtaStrings[ 9 ] );

	// do a BULLSEYE/CHOPPER message box
	DoScreenIndependantMessageBox( pHelicopterEtaStrings[ 7 ], MSG_BOX_FLAG_GENERIC, BullsEyeOrChopperSelectionPopupCallback );
}


void BullsEyeOrChopperSelectionPopupCallback( UINT8 ubExitValue )
{
	// button 1 pressed?
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		// chose chopper
		// have to set a flag 'cause first call to RequestGiveSkyriderNewDestination() triggers another msg box & won't work
		gfRequestGiveSkyriderNewDestination = TRUE;
	}
	// button 2 pressed?
	else if( ubExitValue == MSG_BOX_RETURN_NO )
	{
		// chose bullseye
		StartChangeSectorArrivalMode();
	}
}


// wake up anybody who needs to be awake to travel
void WakeUpAnySleepingSelectedMercsOnFootOrDriving( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter;
	BOOLEAN fSuccess = FALSE;


	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( ( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE ) )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

			// if asleep
			if ( pSoldier->flags.fMercAsleep )
			{
				// and on foot or driving
				if ( ( pSoldier->bAssignment < ON_DUTY ) ||
						( ( pSoldier->bAssignment == VEHICLE ) && SoldierMustDriveVehicle( pSoldier, pSoldier->iVehicleId, FALSE ) ) )
				{
					// we should be guaranteed that he CAN wake up to get this far, so report errors, but don't force it
					fSuccess = SetMercAwake( pSoldier, TRUE, FALSE );
					Assert( fSuccess );
				}
			}
		}
	}
}

void HandlePostAutoresolveMessages()
{
	//KM: Autoresolve sets up this global sector value whenever the enemy gains control of a sector.	As soon as
	//we leave autoresolve and enter mapscreen, then this gets called and handles ownership change for the sector.
	//It also brings up a dialog stating to the player what happened, however, the internals of those functions
	//breaks autoresolve and the game crashes after autoresolve is finished.	The value doesn't need to be saved.
	//
	//An additional case is when creatures kill all opposition in the sector.	For each surviving monster, civilians
	//are "virtually" murdered and loyalty hits will be processed.
	if( gsCiviliansEatenByMonsters >= 1 )
	{
		AdjustLoyaltyForCivsEatenByMonsters( (UINT8)SECTORX( gsEnemyGainedControlOfSectorID ),
																				(UINT8)SECTORY( gsEnemyGainedControlOfSectorID ),
																				(UINT8)gsCiviliansEatenByMonsters );
		gsCiviliansEatenByMonsters = -2;
	}
	else if( gsCiviliansEatenByMonsters == -2 )
	{
		fMapPanelDirty = TRUE;
		gsCiviliansEatenByMonsters = -1;
		gsEnemyGainedControlOfSectorID = -1;
	}
	else if( gsEnemyGainedControlOfSectorID >= 0 )
	{ //bring up the dialog box
		SetThisSectorAsEnemyControlled( (UINT8)SECTORX( gsEnemyGainedControlOfSectorID ), (UINT8)SECTORY( gsEnemyGainedControlOfSectorID ), 0, TRUE );
		gsEnemyGainedControlOfSectorID = -2;
	}
	else if( gsEnemyGainedControlOfSectorID == -2 )
	{ //dirty the mapscreen after the dialog box goes away.
		fMapPanelDirty = TRUE;
		gsEnemyGainedControlOfSectorID = -1;
	}
	else if( gbMilitiaPromotions )
	{
		CHAR16 str[ 512 ];
		BuildMilitiaPromotionsString( str );
		DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, MapScreenDefaultOkBoxCallback );
	}
}


void GetMapscreenMercAssignmentString( SOLDIERTYPE *pSoldier, CHAR16 sString[] )
{
	if ( pSoldier->bAssignment == VEHICLE )
	{
		wcscpy( sString, pShortVehicleStrings[ pVehicleList[ pSoldier->iVehicleId ].ubVehicleType ] );
	}
	else
	{
		wcscpy(sString, pAssignmentStrings[ pSoldier->bAssignment ] );
	}
	// If soldier is working at a facility, add an asterisk.
	if (pSoldier->sFacilityTypeOperated != -1)
	{
		wcscat(sString, L"*");
	}
}


void GetMapscreenMercLocationString( SOLDIERTYPE *pSoldier, CHAR16 sString[] )
{
	CHAR16 pTempString[32];


	if( pSoldier->bAssignment == IN_TRANSIT )
	{
		// show blank
		wcscpy( sString, L"--" );
	}
	else
	{
		if( pSoldier->bAssignment == ASSIGNMENT_POW )
		{
			// POW - location unknown
			swprintf( sString, L"%s", pPOWStrings[ 1 ] );
		}
		else
		{
			swprintf( pTempString, L"%s%s%s",
						pMapVertIndex[ pSoldier->sSectorY ], pMapHortIndex[ pSoldier->sSectorX ], pMapDepthIndex[ pSoldier->bSectorZ ] );

			if ( pSoldier->flags.fBetweenSectors )
			{
				// put brackets around it when he's between sectors!
				swprintf( sString, L"(%s)", pTempString );
			}
			else
			{
				wcscpy( sString, pTempString );
			}
		}
	}
}


void GetMapscreenMercDestinationString( SOLDIERTYPE *pSoldier, CHAR16 sString[] )
{
	INT32 iSectorX, iSectorY;
	INT16 sSector=0;
	GROUP *pGroup = NULL;


	// by default, show nothing
	wcscpy( sString, L"" );

	// if dead or POW - has no destination (no longer part of a group, for that matter)
	if( ( pSoldier->bAssignment == ASSIGNMENT_DEAD ) ||
			( pSoldier->bAssignment == ASSIGNMENT_POW ) ||
			( pSoldier->stats.bLife == 0 ) )
	{
		return;
	}

	if( pSoldier->bAssignment == IN_TRANSIT )
	{
		// show the sector he'll be arriving in
		iSectorX = gsMercArriveSectorX;
		iSectorY = gsMercArriveSectorY;
	}
	else
	{
		// if he's going somewhere
		if ( GetLengthOfMercPath( pSoldier ) > 0 )
		{
			sSector = GetLastSectorIdInCharactersPath( pSoldier );
			// convert
			iSectorX = sSector % MAP_WORLD_X;
			iSectorY = sSector / MAP_WORLD_Y;
		}
		else // no movement path is set...
		{
			if ( pSoldier->flags.fBetweenSectors )
			{
				// he must be returning to his previous (reversed so as to be the next) sector, so show that as his destination
				// individual soldiers don't store previous/next sector coordinates, must go to his group for that
				pGroup = GetGroup( GetSoldierGroupId( pSoldier ) );
				Assert( pGroup );
				iSectorX = pGroup->ubNextX;
				iSectorY = pGroup->ubNextY;
			}
			else
			{
				// show nothing
				return;
			}
		}
	}


	swprintf( sString, L"%s%s", pMapVertIndex[ iSectorY ], pMapHortIndex[ iSectorX ] );
}


void GetMapscreenMercDepartureString( SOLDIERTYPE *pSoldier, CHAR16 sString[], UINT8 *pubFontColor )
{
	INT32 iMinsRemaining = 0;
	INT32 iDaysRemaining = 0;
	INT32 iHoursRemaining = 0;


	if( ( pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__AIM_MERC && pSoldier->ubProfile != SLAY ) || pSoldier->stats.bLife == 0 )
	{
		swprintf( sString, L"%s", gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
	}
	else
	{
		iMinsRemaining = pSoldier->iEndofContractTime - GetWorldTotalMin();

		//if the merc is in transit
		if( pSoldier->bAssignment == IN_TRANSIT )
		{
			//and if the time left on the cotract is greater then the contract time
			if( iMinsRemaining > (INT32)( pSoldier->iTotalContractLength * NUM_MIN_IN_DAY ) )
			{
				iMinsRemaining = ( pSoldier->iTotalContractLength * NUM_MIN_IN_DAY );
			}
		}

		// if 3 or more days remain
		if( iMinsRemaining >= MAP_TIME_UNDER_THIS_DISPLAY_AS_HOURS )
		{
			iDaysRemaining = iMinsRemaining / (24*60);

			*pubFontColor = FONT_LTGREEN;

			swprintf(sString, L"%d%s", iDaysRemaining, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ] );
		}
		else	// less than 3 days
		{
			if( iMinsRemaining > 5 )
			{
				iHoursRemaining = ( iMinsRemaining + 59 ) / 60;
			}
			else
			{
				iHoursRemaining = 0;
			}

		// last 3 days is Red, last 4 hours start flashing red/white!
		if( ( iMinsRemaining <= MINS_TO_FLASH_CONTRACT_TIME ) && ( fFlashContractFlag == TRUE ) )
		{
			*pubFontColor = FONT_WHITE;
		}
		else
		{
			*pubFontColor = FONT_RED;
		}

		swprintf(sString, L"%d%s", iHoursRemaining, gpStrategicString[ STR_PB_HOURS_ABBREVIATION ] );
		}
	}
}



void InitPreviousPaths( void )
{
	INT32 iCounter = 0;

	// init character previous paths
	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		gpCharacterPreviousMercPath[ iCounter ] = NULL;
	}

	// init helicopter previous path
	gpHelicopterPreviousMercPath = NULL;
}


void RememberPreviousPathForAllSelectedChars( void )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;

	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE )
		{
			pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

			// remember his previous path by copying it to his slot in the array kept for that purpose
			gpCharacterPreviousMercPath[ iCounter ] = CopyPaths( GetSoldierMercPathPtr( MercPtrs[ gCharactersList[ iCounter ].usSolID ] ), gpCharacterPreviousMercPath[ iCounter ] );
		}
	}
}


void RestorePreviousPaths( void )
{
	INT32 iCounter = 0;
	SOLDIERTYPE *pSoldier = NULL;
	PathStPtr *ppMovePath = NULL;
	UINT8 ubGroupId = 0;
	BOOLEAN fPathChanged = FALSE;


	// invalid if we're not plotting movement
	Assert( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) );


	if ( fPlotForHelicopter == TRUE )
	{
		ppMovePath = &( pVehicleList[ iHelicopterVehicleId ].pMercPath );
		ubGroupId = pVehicleList[ iHelicopterVehicleId ].ubMovementGroup;

		// if the helicopter had a previous path
		if( gpHelicopterPreviousMercPath != NULL )
		{
			gpHelicopterPreviousMercPath = MoveToBeginningOfPathList( gpHelicopterPreviousMercPath );

			// clear current path
			*ppMovePath = ClearStrategicPathList( *ppMovePath, ubGroupId );
			// replace it with the previous one
			*ppMovePath = CopyPaths( gpHelicopterPreviousMercPath, *ppMovePath );
			// will need to rebuild waypoints
			fPathChanged = TRUE;
		}
		else	// no previous path
		{
			// if he currently has a path
			if( *ppMovePath )
			{
				// wipe it out!
				*ppMovePath = MoveToBeginningOfPathList( *ppMovePath );
				*ppMovePath = ClearStrategicPathList( *ppMovePath, ubGroupId );
				// will need to rebuild waypoints
				fPathChanged = TRUE;
			}
		}

		if ( fPathChanged )
		{
			// rebuild waypoints
			RebuildWayPointsForGroupPath( *ppMovePath, ubGroupId );

			// copy his path to all selected characters
			CopyPathToAllSelectedCharacters( *ppMovePath );
		}
	}
	else	// character(s) plotting
	{
		for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
		{
			// if selected
			if( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE )
			{
				pSoldier = MercPtrs[ gCharactersList[ iCounter ].usSolID ];

				if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
				{
					ppMovePath = &( pVehicleList[ pSoldier->bVehicleID ].pMercPath );
					ubGroupId = pVehicleList[ pSoldier->bVehicleID ].ubMovementGroup;
				}
				else if( pSoldier->bAssignment == VEHICLE )
				{
					ppMovePath = &( pVehicleList[ pSoldier->iVehicleId ].pMercPath );
					ubGroupId = pVehicleList[ pSoldier->iVehicleId ].ubMovementGroup;
				}
				else if( pSoldier->bAssignment < ON_DUTY )
				{
					ppMovePath = &( pSoldier->pMercPath );
					ubGroupId = pSoldier->ubGroupID;
				}
				else
				{
					// invalid pSoldier - that guy can't possibly be moving, he's on a non-vehicle assignment!
					Assert( 0 );
					continue;
				}


				fPathChanged = FALSE;

				// if we have the previous path stored for the dest char
				if( gpCharacterPreviousMercPath[ iCounter ] )
				{
					gpCharacterPreviousMercPath[ iCounter ] = MoveToBeginningOfPathList( gpCharacterPreviousMercPath[ iCounter ] );

					// clear current path
					*ppMovePath = ClearStrategicPathList( *ppMovePath, ubGroupId );
					// replace it with the previous one
					*ppMovePath = CopyPaths( gpCharacterPreviousMercPath[ iCounter ], *ppMovePath );
					// will need to rebuild waypoints
					fPathChanged = TRUE;
				}
				else	// no previous path stored
				{
					// if he has one now, wipe it out
					if( *ppMovePath )
					{
						// wipe it out!
						*ppMovePath = MoveToBeginningOfPathList( *ppMovePath );
						*ppMovePath = ClearStrategicPathList( *ppMovePath, ubGroupId );
						// will need to rebuild waypoints
						fPathChanged = TRUE;
					}
				}


				if ( fPathChanged )
				{
					// rebuild waypoints
					RebuildWayPointsForGroupPath( *ppMovePath, ubGroupId );
				}
			}
		}
	}
}


void ClearPreviousPaths( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; iCounter++ )
	{
		if( fSelectedListOfMercsForMapScreen[ iCounter ] == TRUE )
		{
			gpCharacterPreviousMercPath[ iCounter ] = ClearStrategicPathList( gpCharacterPreviousMercPath[ iCounter ], 0 );
		}
	}
	gpHelicopterPreviousMercPath = ClearStrategicPathList( gpHelicopterPreviousMercPath, 0 );
}


void SelectAllCharactersInSquad( INT8 bSquadNumber )
{
	INT8 bCounter;
	BOOLEAN fFirstOne = TRUE;
	SOLDIERTYPE *pSoldier;

	// ignore if that squad is empty
	if( SquadIsEmpty( bSquadNumber ) == TRUE )
	{
		return;
	}

	// select nobody & reset the selected list
	ChangeSelectedInfoChar( -1, TRUE );

	// now select all the soldiers that are in this squad
	for( bCounter = 0; bCounter < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; bCounter++ )
	{
		// is this entry is valid
		if( gCharactersList[ bCounter ].fValid == TRUE )
		{
			pSoldier = MercPtrs[ gCharactersList[ bCounter ].usSolID ];

			// if this guy is on that squad or in a vehicle which is assigned to that squad
			// NOTE: There's no way to select everyone aboard Skyrider with this function...
			if ( ( pSoldier->bAssignment == bSquadNumber ) || IsSoldierInThisVehicleSquad( pSoldier, bSquadNumber ) )
			{
				if ( fFirstOne )
				{
					// make the first guy in the list who is in this squad the selected info char
					ChangeSelectedInfoChar( bCounter, FALSE );

					// select his sector
					ChangeSelectedMapSector( pSoldier->sSectorX, pSoldier->sSectorY, pSoldier->bSectorZ );

					fFirstOne = FALSE;
				}

				SetEntryInSelectedCharacterList( bCounter );
			}
		}
	}
}



BOOLEAN CanDrawSectorCursor( void )
{
	if( /*( fCursorIsOnMapScrollButtons == FALSE ) && */
			( fShowTownInfo == FALSE ) && ( ghTownMineBox == -1 ) &&
			( fShowUpdateBox == FALSE ) && ( GetNumberOfMercsInUpdateList() == 0 ) &&
			( sSelectedMilitiaTown == 0 ) && ( gfMilitiaPopupCreated == FALSE ) &&
			( gfStartedFromMapScreen == FALSE ) &&
			( fShowMapScreenMovementList == FALSE ) && ( ghMoveBox == - 1 ) &&
			( fMapInventoryItem == FALSE )
		)
	{
		return( TRUE );
	}

	return( FALSE );
}



void RestoreMapSectorCursor( INT16 sMapX, INT16 sMapY )
{
	INT16 sScreenX, sScreenY;


	Assert( ( sMapX >= 1 ) && ( sMapX <= 16 ) );
	Assert( ( sMapY >= 1 ) && ( sMapY <= 16 ) );

	GetScreenXYFromMapXY( sMapX, sMapY, &sScreenX, &sScreenY );

	sScreenY -= 1;

/*
	if(fZoomFlag)
		RestoreExternBackgroundRect( ((INT16)( sScreenX - MAP_GRID_X )), ((INT16)( sScreenY - MAP_GRID_Y )), DMAP_GRID_ZOOM_X, DMAP_GRID_ZOOM_Y);
	else
*/
	RestoreExternBackgroundRect( sScreenX, sScreenY, DMAP_GRID_X, DMAP_GRID_Y );
}



void RequestToggleMercInventoryPanel( void )
{
	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );
		return;
	}

	if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
	{
		AbortMovementPlottingMode( );
	}


	if ( !CanToggleSelectedCharInventory() )
	{
		return;
	}

	if( fShowDescriptionFlag == TRUE )
	{
		// turn off item description
		DeleteItemDescriptionBox( );
	}
	else
	{
		// toggle inventory mode
		fShowInventoryFlag = !fShowInventoryFlag;

		// set help text for item glow region
		if( fShowInventoryFlag )
		{
			SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 2 ] );
		}
		else
		{
			// Headrock. New line forces InvBottom rerender when the inventory is opened and closed.
			fEndShowInventoryFlag = TRUE;
			RenderMapScreenInterfaceBottom ( TRUE ); 
			SetRegionFastHelpText( &gCharInfoHandRegion, pMiscMapScreenMouseRegionHelpText[ 0 ] );
		}
	}

	fTeamPanelDirty = TRUE;
}



void RequestContractMenu( void )
{
	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );
		return;
	}

	if( gfPreBattleInterfaceActive == TRUE )
	{
		return;
	}

	if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
	{
		AbortMovementPlottingMode( );
	}


	// in case we have multiple guys selected, turn off everyone but the guy we're negotiating with
	ChangeSelectedInfoChar( bSelectedInfoChar, TRUE );

	if ( CanExtendContractForCharSlot( bSelectedInfoChar ) )
	{
		// create
		RebuildContractBoxForMerc( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] );

		// reset selected characters
		ResetAllSelectedCharacterModes( );

		bSelectedContractChar = bSelectedInfoChar;
		giContractHighLine = bSelectedContractChar;

		// if not triggered internally
		if ( CheckIfSalaryIncreasedAndSayQuote( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ], TRUE ) == FALSE )
		{
			// show contract box
			fShowContractMenu = TRUE;

			// stop any dialogue by character
			StopAnyCurrentlyTalkingSpeech( );
		}

		//fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
		// reset selected characters
		ResetAllSelectedCharacterModes( );
	}
}



void ChangeCharacterListSortMethod( INT32 iValue )
{
	Assert( iValue >= 0 );
	Assert( iValue < MAX_SORT_METHODS );

	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );
		return;
	}

	if( gfPreBattleInterfaceActive == TRUE )
	{
		return;
	}

	if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
	{
		AbortMovementPlottingMode( );
	}

	if(iValue == 0 && _KeyDown( ALT ) )
		iValue = 6;
	if(iValue == giSortStateForMapScreenList && giSortOrderAscending == TRUE)
		giSortOrderAscending = FALSE;
	else if(iValue == giSortStateForMapScreenList && giSortOrderAscending == FALSE)
		giSortOrderAscending = TRUE;

	giSortStateForMapScreenList = iValue;
	SortListOfMercsInTeamPanel( TRUE, giSortOrderAscending );
}



void MapscreenMarkButtonsDirty()
{
	// redraw buttons
	MarkButtonsDirty( );

	// if border buttons are created
	if( !fShowMapInventoryPool )
	{
		// if the attribute assignment menu is showing
		if ( fShowAttributeMenu )
		{
			// don't redraw the town button, it would wipe out a chunk of the attribute menu
			UnMarkButtonDirty( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] );
		}
	}
}
// HEADROCK HAM 3.6: Get a total of all merc salaries. Used for the new Daily Expenses display.
INT32 GetTotalContractExpenses ( void )
{
	if (gGameExternalOptions.ubIncludeContractsInExpenses == 0)
	{
		// Contract costs are excluded.
		return (0);
	}

	UINT8 ubCounter = 0;
	SOLDIERTYPE *pSoldier;
	INT32 iTotalCost = 0;

	while(gCharactersList[ubCounter].fValid)
	{
		pSoldier = MercPtrs[ gCharactersList[ ubCounter ].usSolID ];
		// salary
		if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC )
		{
			if (gGameExternalOptions.ubIncludeContractsInExpenses == 2)
			{
				// daily rate
				if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
				{
					iTotalCost += ( gMercProfiles[ pSoldier->ubProfile ].uiBiWeeklySalary / 14 );
				}
				else if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK )
				{
					iTotalCost += ( gMercProfiles[ pSoldier->ubProfile ].uiWeeklySalary / 7 );
				}
				else
				{
					iTotalCost += gMercProfiles[ pSoldier->ubProfile ].sSalary;
				}
			}
		}
		else
		{
			iTotalCost += gMercProfiles[ pSoldier->ubProfile ].sSalary;
		}
		ubCounter++;
	}
	return (iTotalCost);
}
