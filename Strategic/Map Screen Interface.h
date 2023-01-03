#ifndef MAP_SCREEN_INTERFACE_H
#define MAP_SCREEN_INTERFACE_H


#include "Types.h"
#include "Soldier Control.h"
#include "MessageBoxScreen.h"
#include "Font Control.h"
// WDS - make number of mercenaries, etc. be configurable
#include "Squads.h"


typedef struct FASTHELPREGION {
	// the string
	CHAR16 FastHelpText[ 256 ];

	// the x and y position values
	INT32 iX;
	INT32 iY;
	INT32 iW;

} FASTHELPREGION;

// Flugente: externalised squad names
extern std::vector<std::wstring> gSquadNameVector;

extern UINT8 FIRSTmercTOdisplay ;
extern UINT8 maxNumberOfMercVisibleInStrategyList;

// String Lengths Defines
#define MAX_NAME_LENGTH 10
#define MAX_LOCATION_SIZE 8
#define MAX_DESTETA_SIZE 8
#define MAX_ASSIGN_SIZE 10
#define MAX_TIME_REMAINING_SIZE 8


// char breath and life position
#define BAR_INFO_X 66
#define BAR_INFO_Y 61

// merc icon position
#define CHAR_ICON_CONTRACT_Y 64
#define CHAR_ICON_X 187
#define CHAR_ICON_WIDTH 10
#define CHAR_ICON_HEIGHT 10
#define CHAR_ICON_SPACING 13

// map screen font
#define MAP_SCREEN_FONT BLOCKFONT2

// characterlist regions
#define	Y_START				yResOffset + 135	//146
#define	MAP_START_KEYRING_Y	yResOffset + 107
#define	Y_SIZE				GetFontHeight(MAP_SCREEN_FONT)

//lal
// militia control menu
enum{
	MILCON_MENU_ATTACK,
	MILCON_MENU_HOLD,
	MILCON_MENU_RETREAT,
	MILCON_MENU_COMETOME,
	MILCON_MENU_GETDOWN,
	MILCON_MENU_CROUCH,
	MILCON_MENU_TAKE_COVER,
	MILCON_MENU_MOVE_TO,
	MILCON_MENU_ALL_ATTACK,
	MILCON_MENU_ALL_HOLD,
	MILCON_MENU_ALL_RETREAT,
	MILCON_MENU_ALL_COMETOME,
	MILCON_MENU_ALL_SPREAD,
	MILCON_MENU_ALL_GETDOWN,
	MILCON_MENU_ALL_CROUCH,
	MILCON_MENU_ALL_TAKE_COVER,
	//MILCON_MENU_ALL_TAKE_ITEMS,
	MILCON_MENU_CANCEL,
	MAX_MILCON_STRING_COUNT,
};

//enum{
//	TTALL_MENU_ATTACK,
//	TTALL_MENU_HOLD,
//	TTALL_MENU_RETREAT,
//	TTALL_MENU_COMETOME,
//	TTALL_MENU_GETDOWN,
//	TTALL_MENU_CANCEL,
//	MAX_TTALL_STRING_COUNT,
//};

// attribute menu defines (must match NUM_TRAINABLE_STATS defines, and pAttributeMenuStrings )
enum {
	ATTRIB_MENU_HEA=0,
	ATTRIB_MENU_AGI,
	ATTRIB_MENU_DEX,
	ATTRIB_MENU_STR,
	ATTRIB_MENU_LEAD,
	ATTRIB_MENU_MARK,
	ATTRIB_MENU_MECH,
	ATTRIB_MENU_EXPLOS,
	ATTRIB_MENU_MED,
	ATTRIB_MENU_CANCEL,
	MAX_ATTRIBUTE_STRING_COUNT,
};

// the epc assignment menu
enum{
	EPC_MENU_ON_DUTY = 0,
	EPC_MENU_PATIENT,
	EPC_MENU_VEHICLE,
	EPC_MENU_REMOVE,
	EPC_MENU_CANCEL,
	MAX_EPC_MENU_STRING_COUNT,
};

// assignment menu defines
enum {
	ASSIGN_MENU_ON_DUTY=0,
	ASSIGN_MENU_DOCTOR,
	ASSIGN_MENU_DOCTOR_DIAGNOSIS,	// added by Flugente
	ASSIGN_MENU_PATIENT,
	ASSIGN_MENU_VEHICLE,
	ASSIGN_MENU_REPAIR,
	ASSIGN_MENU_RADIO_SCAN,	// added by Flugente
	ASSIGN_MENU_SNITCH,
	ASSIGN_MENU_TRAIN,
	ASSIGN_MENU_MILITIA,	// added by Flugente: all things militia
	ASSIGN_MENU_MOVE_ITEMS, // added by Flugente
	ASSIGN_MENU_FORTIFY, // added by Flugente
	ASSIGN_MENU_SPY,	// added by Flugente
	ASSIGN_MENU_ADMINISTRATION,	// added by Flugente
	ASSIGN_MENU_EXPLORATION,	// added by Flugente
	ASSIGN_MENU_FACILITY, // HEAROCK HAM 3.6: Facility List menu
	ASSIGN_MENU_CANCEL,
	MAX_ASSIGN_STRING_COUNT,
};

// snitch menu defines
enum{
	SNITCH_MENU_TOGGLE = 0,
	SNITCH_MENU_SECTOR,	
	SNITCH_MENU_CANCEL,
	MAX_SNITCH_MENU_STRING_COUNT,
};

enum {
	SNITCH_MENU_TOGGLE_ON = 0,
	SNITCH_MENU_TOGGLE_OFF,
	SNITCH_MENU_MISBEHAVIOUR_ON,
	SNITCH_MENU_MISBEHAVIOUR_OFF,
	SNITCH_MENU_TOGGLE_CANCEL,
	MAX_SNITCH_TOGGLE_MENU_STRING_COUNT,
};

enum {
	SNITCH_MENU_SECTOR_PROPAGANDA = 0,
	SNITCH_MENU_SECTOR_GATHER_RUMOURS,
	SNITCH_MENU_SECTOR_CANCEL,
	MAX_SNITCH_SECTOR_MENU_STRING_COUNT,
};

// Flugente: prisoner menu defines
enum {
	PRISONER_MENU_ADMIN = 0,
	PRISONER_MENU_TROOP,
	PRISONER_MENU_ELITE,
	PRISONER_MENU_OFFICER,
	PRISONER_MENU_GENERAL,
	PRISONER_MENU_CIVILIAN,
	PRISONER_MENU_CANCEL,
	MAX_PRISONER_MENU_STRING_COUNT,
};

// training assignment menu defines
enum {
	TRAIN_MENU_SELF,
	TRAIN_MENU_WORKERS,
	TRAIN_MENU_TEAMMATES,
	TRAIN_MENU_TRAIN_BY_OTHER,
	TRAIN_MENU_CANCEL,
	MAX_TRAIN_STRING_COUNT,
};


// the remove merc from team pop up box strings
enum{
	REMOVE_MERC = 0,
	REMOVE_MERC_CANCEL,
	MAX_REMOVE_MERC_COUNT,
};


/*// squad menu defines
enum{
	SQUAD_MENU_1,
	SQUAD_MENU_2,
	SQUAD_MENU_3,
	SQUAD_MENU_4,
	SQUAD_MENU_5,
	SQUAD_MENU_6,
	SQUAD_MENU_7,
	SQUAD_MENU_8,
	SQUAD_MENU_9,
	SQUAD_MENU_10,
	SQUAD_MENU_11,
	SQUAD_MENU_12,
	SQUAD_MENU_13,
	SQUAD_MENU_14,
	SQUAD_MENU_15,
	SQUAD_MENU_16,
	SQUAD_MENU_17,
	SQUAD_MENU_18,
	SQUAD_MENU_19,
	SQUAD_MENU_20,
	SQUAD_MENU_CANCEL,
	MAX_SQUAD_MENU_STRING_COUNT,
};*/

#define SQUAD_MENU_CANCEL			ON_DUTY
#define MAX_SQUAD_MENU_STRING_COUNT SQUAD_MENU_CANCEL + 1

// contract menu defines
enum{
	CONTRACT_MENU_CURRENT_FUNDS = 0,
	CONTRACT_MENU_SPACE,
	CONTRACT_MENU_DAY,
	CONTRACT_MENU_WEEK,
	CONTRACT_MENU_TWO_WEEKS,
	CONTRACT_MENU_TERMINATE,
	CONTRACT_MENU_CANCEL,
	MAX_CONTRACT_MENU_STRING_COUNT,
};

// enums for pre battle interface pop ups
enum
{
	ASSIGNMENT_POPUP,
	DESTINATION_POPUP,
	CONTRACT_POPUP
};


enum{
	NO_REASON_FOR_UPDATE = 0,
	CONTRACT_FINISHED_FOR_UPDATE,
	ASSIGNMENT_FINISHED_FOR_UPDATE,
	ASSIGNMENT_RETURNING_FOR_UPDATE,
	ASLEEP_GOING_AUTO_FOR_UPDATE,
	CONTRACT_EXPIRE_WARNING_REASON,
};


enum{
	START_RED_SECTOR_LOCATOR = 0,
	STOP_RED_SECTOR_LOCATOR,
	START_YELLOW_SECTOR_LOCATOR,
	STOP_YELLOW_SECTOR_LOCATOR,
};

// Flugente: somewhat pointless since mobile militia has been removed, but perhaps there will be other kinds of militia at some point, so keep this for now
// HEADROCK HAM 3.6: Enums for Militia Training Types
enum{
	TOWN_MILITIA = 0,
	NUM_MILITIA_TRAINING_TYPES
};

// dimensions and offset for merc update box
#define UPDATE_MERC_FACE_X_WIDTH 50
#define UPDATE_MERC_FACE_X_HEIGHT 50
#define UPDATE_MERC_FACE_X_OFFSET 2
#define UPDATE_MERC_FACE_Y_OFFSET 2
#define WIDTH_OF_UPDATE_PANEL_BLOCKS	50
#define HEIGHT_OF_UPDATE_PANEL_BLOCKS 50
#define UPDATE_MERC_Y_OFFSET 4
#define UPDATE_MERC_X_OFFSET 4


// dimensions and offset for merc update box
#define TACT_UPDATE_MERC_FACE_X_WIDTH 70
#define TACT_UPDATE_MERC_FACE_X_HEIGHT 49
#define TACT_UPDATE_MERC_FACE_X_OFFSET 8
#define TACT_UPDATE_MERC_FACE_Y_OFFSET 6
#define TACT_WIDTH_OF_UPDATE_PANEL_BLOCKS	70
#define TACT_HEIGHT_OF_UPDATE_PANEL_BLOCKS 49
#define TACT_UPDATE_MERC_Y_OFFSET 4
#define TACT_UPDATE_MERC_X_OFFSET 4

class OLD_MERC_LEAVE_ITEM_101
{
public:
	OLD_OBJECTTYPE_101 oldObject;
	class MERC_LEAVE_ITEM *pNext;
};

class MERC_LEAVE_ITEM
{
public:
	BOOLEAN Save( HWFILE hFile);
	BOOLEAN Load( HWFILE hFile);
	void initialize() {object.initialize(); pNext = NULL;};
	OBJECTTYPE object;
	class MERC_LEAVE_ITEM *pNext;
};

extern BOOLEAN fShowAssignmentMenu;
extern BOOLEAN fShowAttributeMenu;
extern BOOLEAN fShowContractMenu ;
extern BOOLEAN fShowRemoveMenu ;
extern BOOLEAN fShowMilitiaControlMenu ; //lal
//extern BOOLEAN fShowTalkToAllMenu ;
extern BOOLEAN fShowFacilityAssignmentMenu; // HEADROCK HAM 3.6
// anv: snitch menus
extern BOOLEAN fShowSnitchToggleMenu;
extern BOOLEAN fShowSnitchSectorMenu;

// Flugente: prisoner menu
extern BOOLEAN fShowPrisonerMenu;

extern BOOLEAN fFirstTimeInMapScreen;
extern BOOLEAN fLockOutMapScreenInterface;

// The character data structure
typedef struct {

	UINT16 usSolID;// soldier ID in MenPtrs 
	BOOLEAN fValid;// is the current soldier a valid soldier

} MapScreenCharacterSt;


// map screen character structure list, contrains soldier ids into menptr
extern MapScreenCharacterSt gCharactersList[ ];

extern BOOLEAN fShowMapScreenHelpText;


// map inventory pool inited
extern BOOLEAN fMapInventoryPoolInited;

// highlighted lines
extern INT32 giHighLine;
extern INT32 giAssignHighLine;
extern INT32 giDestHighLine;
extern INT32 giContractHighLine;
extern INT32 giSleepHighLine;

extern UINT32 guiUpdatePanel;
extern UINT32 guiUpdatePanelTactical;
extern BOOLEAN fShowUpdateBox;

extern SGPRect ContractDimensions;
extern SGPPoint ContractPosition;
extern SGPRect AttributeDimensions;
extern SGPPoint AttributePosition;
extern SGPRect TrainDimensions;
extern SGPPoint TrainPosition;
extern SGPRect VehicleDimensions;
extern SGPPoint VehiclePosition;
extern SGPRect AssignmentDimensions ;
extern SGPPoint AssignmentPosition ;
extern SGPPoint SquadPosition ;
extern SGPRect SquadDimensions ;
// HEADROCK HAM 3.6: Facility Menu variables
extern SGPPoint FacilityPosition;
extern SGPRect FacilityDimensions;
extern SGPPoint FacilityAssignmentPosition;
extern SGPRect FacilityAssignmentDimensions;

extern SGPPoint RepairPosition;
extern SGPRect RepairDimensions;

extern SGPPoint SnitchPosition;
extern SGPRect SnitchDimensions;
extern SGPPoint SnitchTogglePosition;
extern SGPRect SnitchToggleDimensions;
extern SGPPoint SnitchSectorPosition;
extern SGPRect SnitchSectorDimensions;

extern SGPPoint PrisonerPosition;
extern SGPRect PrisonerDimensions;

extern SGPPoint MilitiaControlPosition; //lal
extern SGPRect MilitiaControlDimensions;

//extern SGPPoint TalkToAllPosition;
//extern SGPRect TalkToAllDimensions;

extern SGPPoint OrigContractPosition;
extern SGPPoint OrigAttributePosition;
extern SGPPoint OrigSquadPosition ;
extern SGPPoint OrigAssignmentPosition ;
extern SGPPoint OrigTrainPosition;
extern SGPPoint OrigVehiclePosition;
extern SGPPoint OrigMilitiaControlPosition ; //lal
extern SGPPoint OrigFacilityPosition; // HEADROCK HAM 3.6: Facility Menu
extern SGPPoint OrigFacilityAssignmentPosition; // HEADROCK HAM 3.6
//extern SGPPoint OrigTalkToAllPosition ;
extern SGPPoint OrigSnitchPosition;
extern SGPPoint OrigSnitchTogglePosition;
extern SGPPoint OrigSnitchSectorPosition;

extern SGPPoint OrigPrisonerPosition;

// disble team info panel due to showing of battle roster
extern BOOLEAN fDisableDueToBattleRoster;

extern BOOLEAN gfAtLeastOneMercWasHired;

// curtrent map sector z that is being displayed in the mapscreen
extern INT32 iCurrentMapSectorZ;

// y position of the pop up box
extern INT32 giBoxY;

// pop up box textures
extern UINT32	guiPOPUPTEX;
extern UINT32	guiPOPUPBORDERS;

// the level-changing markers on the map border
extern UINT32		guiLEVELMARKER;

// the currently selected character arrow
extern UINT32		guiSelectedCharArrow;

// sam and mine icons
extern UINT32 guiSAMICON;

extern BOOLEAN fShowMapScreenMovementList;

// do we need to rebuild the mapscreen characterlist?
extern BOOLEAN fReBuildCharacterList;

// restore glow rotation in contract region glow boxes
extern BOOLEAN fResetContractGlow;

// init vehicle and characters list
void InitalizeVehicleAndCharacterList( void );

// set this entry to as selected
void SetEntryInSelectedCharacterList( INT8 bEntry );
// set this entry to as unselected
void ResetEntryForSelectedList( INT8 bEntry );

// reset selected list
void ResetSelectedListForMapScreen( );

// build a selected list from a to b, inclusive
void BuildSelectedListFromAToB( INT8 bA, INT8 bB );

// isa this entry int he selected character list set?
BOOLEAN IsEntryInSelectedListSet( INT8 bEntry );

// is there more than one person selected?
BOOLEAN MultipleCharacterListEntriesSelected( void );

// toggle this entry on or off
void ToggleEntryInSelectedList( INT8 bEntry );

// reset assignments for mercs on selected list who have this assignment
// HEADROCK HAM 3.6: Argument was unused... adding my own.
void ResetAssignmentsForMercsTrainingUnpaidSectorsInSelectedList( UINT8 ubMilitiaType );

/*
// plot path for selected character list
void PlotPathForSelectedCharacterList( INT16 sX, INT16 sY );
*/

void RestoreBackgroundForAssignmentGlowRegionList( void );
void RestoreBackgroundForDestinationGlowRegionList( void );
void RestoreBackgroundForContractGlowRegionList( void );
void RestoreBackgroundForSleepGlowRegionList( void );

// play click when we are entering a glow region
void PlayGlowRegionSound( void );

// is this character in the action of plotting a path?
INT16 CharacterIsGettingPathPlotted( INT16 sCharNumber );

// disable team info panels
void DisableTeamInfoPanels( void );

// enable team info panels
void EnableTeamInfoPanels( void );
 
// activate pop up for soldiers in the pre battle interface
void ActivateSoldierPopup( SOLDIERTYPE *pSoldier, UINT8 ubPopupType, INT16 xp, INT16 yp );

// do mapscreen message box
INT32 DoMapMessageBox( UINT8 ubStyle,	STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback );

// hop up one leve,l int he map screen level interface
void GoUpOneLevelInMap( void );

// go down one level in the mapscreen map interface
void GoDownOneLevelInMap( void );

// jump to this level on the map
void JumpToLevel( INT32 iLevel );

// check to see if we need to update the screen
void CheckAndUpdateBasedOnContractTimes( void );

// check if are just about to display this pop up or stopping display
void HandleDisplayOfItemPopUpForSector( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

// display red arrow by name of selected merc
void HandleDisplayOfSelectedMercArrows( void );

// check which guys can move with this guy
void DeselectSelectedListMercsWhoCantMoveWithThisGuy( SOLDIERTYPE *pSoldier );

// get morale string for this grunt given this morale level
void GetMoraleString( SOLDIERTYPE *pSoldier, STR16 sString );

// handle leaving of equipment in sector
void HandleLeavingOfEquipmentInCurrentSector( UINT32 uiMercId );

// set up a linked list of items being dropped and post an event to later drop them
void HandleMercLeavingEquipmentInDrassen( UINT32 uiMercId );
void HandleMercLeavingEquipmentInOmerta( UINT32 uiMercId );

// actually drop the stored list of items
void HandleEquipmentLeftInOmerta( UINT32 uiSlotIndex );
void HandleEquipmentLeftInDrassen( UINT32 uiSlotIndex );

// init/shutdown leave item lists
void InitLeaveList( void );
void ShutDownLeaveList( void );

// add item to leave equip index
BOOLEAN AddItemToLeaveIndex( OBJECTTYPE *object, UINT32 uiIndex );

// release memory for all items in this slot's leave item list
void FreeLeaveListSlot( UINT32 uiSlotIndex );

// first free slot in equip leave list
INT32 FindFreeSlotInLeaveList( void );

// set up drop list
INT32 SetUpDropItemListForMerc( UINT32 uiMercId );
// store owner's profile id for the items added to this leave slot index
void SetUpMercAboutToLeaveEquipment( UINT32 ubProfileId, UINT32 uiSlotIndex );

// remove item from leave index
//BOOLEAN RemoveItemFromLeaveIndex( MERC_LEAVE_ITEM *pItem, UINT32 uiIndex );

// handle a group about to arrive in a sector
void HandleGroupAboutToArrive( void );

// up arrow
void HandleMapScreenUpArrow( void );
void HandleMapScreenDownArrow( void );


// create and destroy the status bars mouse region
void CreateMapStatusBarsRegion( void );
void RemoveMapStatusBarsRegion( void );
void UpdateCharRegionHelpText( void );

// find this soldier in mapscreen character list and set as contract
void FindAndSetThisContractSoldier( SOLDIERTYPE *pSoldier );

// lose the cursor, re-render
void HandleMAPUILoseCursorFromOtherScreen( void );

void RenderMapRegionBackground( void );

// update mapscreen assignment positions
void UpdateMapScreenAssignmentPositions( void );
void UpdateMapScreenMilitiaControlPositions( void );

// get the umber of valid mercs in the mapscreen character list
INT32 GetNumberOfPeopleInCharacterList( void );

// the next and previous people in the mapscreen
void GoToPrevCharacterInList( void );
void GoToNextCharacterInList( void );

// WANNE: Go to first/last character in list (PageUp/PageDown)
void GoToFirstCharacterInList( void );
void GoToLastCharacterInList( void );

// this does the whole miner giving player info speil
//void HandleMinerEvent( UINT8 bMinerNumber, INT16 sSectorX, INT16 sSectorY, INT16 sQuoteNumber, BOOLEAN fForceMapscreen );
void HandleMinerEvent( UINT8 ubHeadMinerIndex, INT16 sSectorX, INT16 sSectorY, INT16 sQuoteNumber, BOOLEAN fForceMapscreen );

// set up the event of animating a mine sector
void SetUpAnimationOfMineSectors( INT32 iEvent );

// display map screen 
void DisplayMapScreenFastHelpList( void );

// handle display of fast help
void HandleDisplayOfExitToTacticalMessageForFirstEntryToMapScreen( void );

// is the text up?
BOOLEAN IsMapScreenHelpTextUp( void );

// stop the help text in mapscreen
void StopMapScreenHelpText( void );

// set up the help text
void SetUpMapScreenFastHelpText( void );

void TurnOnSectorLocator( UINT8 ubProfileID );
void TurnOffSectorLocator();

extern INT16 gsSectorLocatorX;
extern INT16 gsSectorLocatorY;
extern UINT8 gubBlitSectorLocatorCode;

enum
{
	LOCATOR_COLOR_NONE,
	LOCATOR_COLOR_RED,
	LOCATOR_COLOR_YELLOW
};

extern UINT32 guiSectorLocatorGraphicID;
void HandleBlitOfSectorLocatorIcon( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, UINT8 ubLocatorID );


// the tactical version

// handle the actual showingof the list
void HandleShowingOfTacticalInterfaceFastHelpText( void );

// start showing the list
void StartShowingInterfaceFastHelpText( void );

// stop showing the list
void StopShowingInterfaceFastHelpText( void );

// is the list active?
BOOLEAN IsTheInterfaceFastHelpTextActive( void );

//set up the tactical lists
BOOLEAN SetUpFastHelpListRegions( INT32 iXPosition[], INT32 iYPosition[], INT32 iWidth[], STR16 sString[], INT32 iSize );


// the alternate mapscreen movement system
void InitializeMovingLists( void );

// reset assignment for mercs trainign militia in this sector
void ResetAssignmentOfMercsThatWereTrainingMilitiaInThisSector( INT16 sSectorX, INT16 sSectorY, UINT8 ubMilitiaType );


// the sector move box
void DeselectSquadForMovement( INT32 iSquadNumber );
void SelectedSquadForMovement( INT32 iSquadNumber );
void DeselectSoldierForMovement( SOLDIERTYPE *pSoldier );
void SelectSoldierForMovement( SOLDIERTYPE *pSoldier );
void SelectVehicleForMovement( INT32 iVehicleId, BOOLEAN fAndAllOnBoard );
void DeselectVehicleForMovement( INT32 iVehicleId );
void AddVehicleToMovingLists( INT32 iVehicleId );
void AddSquadToMovingLists( INT32 iSquadNumber );
void AddSoldierToMovingLists( SOLDIERTYPE *pSoldier );
void CreateDestroyMovementBox( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );
void SetUpMovingListsForSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );
void ReBuildMoveBox( void );
BOOLEAN IsCharacterSelectedForAssignment( INT16 sCharNumber );
BOOLEAN IsCharacterSelectedForSleep( INT16 sCharNumber );

// the update box
void CreateDestroyTheUpdateBox( void );
void SetSoldierUpdateBoxReason( INT32 iReason );
void AddSoldierToUpdateBox( SOLDIERTYPE *pSoldier );
void ResetSoldierUpdateBox( void );
void DisplaySoldierUpdateBox( );
BOOLEAN IsThePopUpBoxEmpty( void );


// unmarking buttons dirty for dialogue
void UpdateButtonsDuringCharacterDialogue( void );
void UpdateButtonsDuringCharacterDialogueSubTitles( void );
void SetUpdateBoxFlag( BOOLEAN fFlag );



/// set the town of Tixa as found by the player
void SetTixaAsFound( void );

// set the town of Orta as found by the player
void SetOrtaAsFound( void );

// set this SAM site as being found by the player
void SetSAMSiteAsFound( UINT8 uiSamIndex );

// init time menus
void InitTimersForMoveMenuMouseRegions( void );

// the screen mask
void CreateScreenMaskForMoveBox( void );
void RemoveScreenMaskForMoveBox( void );


// help text to show user merc has insurance
void UpdateHelpTextForMapScreenMercIcons( void );
void CreateDestroyInsuranceMouseRegionForMercs( BOOLEAN fCreate );


// stuff to deal with player just starting the game
BOOLEAN HandleTimeCompressWithTeamJackedInAndGearedToGo( void );
//void HandlePlayerEnteringMapScreenBeforeGoingToTactical( void );

// handle sector being taken over uncontested
BOOLEAN NotifyPlayerWhenEnemyTakesControlOfImportantSector( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, BOOLEAN fContested );

// handle notifying player of invasion by enemy
void NotifyPlayerOfInvasionByEnemyForces( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ, MSGBOX_CALLBACK ReturnCallback );

void ShutDownUserDefineHelpTextRegions( void );

// shwo the update box
void ShowUpdateBox( void );

// add special events
void AddSoldierToWaitingListQueue( SOLDIERTYPE *pSoldier );
void AddReasonToWaitingListQueue( INT32 iReason );
void AddDisplayBoxToWaitingQueue( void );

// can this group move it out
BOOLEAN CanEntireMovementGroupMercIsInMove( SOLDIERTYPE *pSoldier, INT8 *pbErrorNumber );
void ReportMapScreenMovementError( INT8 bErrorNumber );

void HandleRebuildingOfMapScreenCharacterList( void );

void RequestToggleTimeCompression( void );
void RequestIncreaseInTimeCompression( void );
void RequestDecreaseInTimeCompression( void );

void SelectUnselectedMercsWhoMustMoveWithThisGuy( void );
BOOLEAN AnyMercInSameSquadOrVehicleIsSelected( SOLDIERTYPE *pSoldier );

INT16 GetRefreshHeightForMercList( void );

BOOLEAN LoadLeaveItemList( HWFILE hFile );
BOOLEAN SaveLeaveItemList( HWFILE hFile );

BOOLEAN CheckIfSalaryIncreasedAndSayQuote( SOLDIERTYPE *pSoldier, BOOLEAN fTriggerContractMenu );

void EndUpdateBox( BOOLEAN fContinueTimeCompression );

extern BOOLEAN CanCharacterMoveInStrategic( SOLDIERTYPE *pSoldier, INT8 *pbErrorNumber );
extern BOOLEAN MapscreenCanPassItemToCharNum( INT32 iNewCharSlot );


#endif