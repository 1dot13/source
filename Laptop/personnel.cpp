	#include "laptop.h"
	#include "personnel.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "Overhead.h"
	#include "Soldier Profile.h"
	#include "Text.h"
	#include "mapscreen.h"
	#include "Game Clock.h"
	#include "finances.h"
	#include "LaptopSave.h"
	#include "Map Screen Interface Map.h"
	#include "input.h"
	#include "english.h"
	#include "random.h"
	#include "line.h"
	#include "Assignments.h"
	#include "gameloop.h"
	#include "Soldier Add.h"
	#include "Interface Items.h"
	#include "Weapons.h"
	#include "strategicmap.h"
	#include "GameSettings.h"
	#include "Merc Contract.h"
	#include "_Ja25Englishtext.h" // added by SANDRO

#include "Soldier Macros.h"
#include "InterfaceItemImages.h"

#include "IMP Skill Trait.h"		// added by Flugente
#include "Map Screen Interface.h"	// added by Flugente
#include "Interface.h"				// added by Flugente
#include "IMP Background.h"			// added by Flugente for AssignBackgroundHelpText()
#include "NPC.h"					// added by Flugente for GetEffectiveApproachValue(...)
#include "Drugs And Alcohol.h"		// added by Flugente for DoesMercHaveDisability(...)


// WDS - make number of mercenaries, etc. be configurable
#define MAX_MERCS_ON_SCREEN 20

#define NUM_BACKGROUND_REPS					40
#define BACKGROUND_HEIGHT						10
#define BACKGROUND_WIDTH						125
#define IMAGE_BOX_X										iScreenWidthOffset + 395
#define IMAGE_BOX_Y									LAPTOP_SCREEN_UL_Y+24
#define IMAGE_BOX_WIDTH							112
#define	IMAGE_BOX_WITH_NO_BORDERS		106
#define IMAGE_BOX_COUNT							4
#define	IMAGE_NAME_WIDTH						106
#define	IMAGE_FULL_NAME_OFFSET_Y		111
#define TEXT_BOX_WIDTH							160
#define	TEXT_DELTA_OFFSET						9
#define TEXT_BOX_Y LAPTOP_SCREEN_UL_Y+188
#define PAGE_BOX_X LAPTOP_SCREEN_UL_X+250 - 10
#define PAGE_BOX_Y LAPTOP_SCREEN_UL_Y+3
#define PAGE_BOX_WIDTH	58
#define PAGE_BOX_HEIGHT 24
#define MAX_SLOTS 4
#define PERS_CURR_TEAM_X LAPTOP_SCREEN_UL_X + 39 - 15
#define PERS_CURR_TEAM_Y LAPTOP_SCREEN_UL_Y + 218
#define PERS_DEPART_TEAM_Y LAPTOP_SCREEN_UL_Y + 247

#define BUTTON_Y LAPTOP_SCREEN_UL_Y+34
#define LEFT_BUTTON_X	LAPTOP_SCREEN_UL_X+3 - 10
#define RIGHT_BUTTON_X LAPTOP_SCREEN_UL_X+476 - 10
#define PERS_COUNT 15
#define MAX_STATS 20
#define PERS_FONT						FONT10ARIAL
#define PERS_HEADER_FONT		FONT14ARIAL
#define CHAR_NAME_FONT			FONT12ARIAL
#define CHAR_NAME_Y										iScreenHeightOffset + 177
#define CHAR_LOC_Y										iScreenHeightOffset + 189
#define PERS_TEXT_FONT_COLOR							FONT_WHITE
#define PERS_TEXT_FONT_ALTERNATE_COLOR FONT_YELLOW
#define PERS_FONT_COLOR FONT_WHITE
#define PAGE_X PAGE_BOX_X+2 - 10
#define PAGE_Y PAGE_BOX_Y+2

#define	FACES_DIR "FACES\\BIGFACES\\"
#define	SMALL_FACES_DIR "FACES\\"

#define	IMP_FACES_DIR "IMPFACES\\BIGFACES\\"
#define	IMP_SMALL_FACES_DIR "IMPFACES\\"

#define NEXT_MERC_FACE_X	LAPTOP_SCREEN_UL_X + 448
#define MERC_FACE_SCROLL_Y LAPTOP_SCREEN_UL_Y + 150
#define PREV_MERC_FACE_X	LAPTOP_SCREEN_UL_X + 285

#define DEPARTED_X LAPTOP_SCREEN_UL_X + 29 - 10
#define DEPARTED_Y LAPTOP_SCREEN_UL_Y + 207

#define PERSONNEL_PORTRAIT_NUMBER 20
#define PERSONNEL_PORTRAIT_NUMBER_WIDTH 5

#define SMALL_PORTRAIT_WIDTH 46
#define SMALL_PORTRAIT_HEIGHT 42

#define SMALL_PORT_WIDTH 52
#define SMALL_PORT_HEIGHT 45

#define	SMALL_PORTRAIT_WIDTH_NO_BORDERS		48

#define SMALL_PORTRAIT_START_X							iScreenWidthOffset + 141 - 10
#define SMALL_PORTRAIT_START_Y							iScreenHeightOffset + 53

#define PERS_CURR_TEAM_COST_X LAPTOP_SCREEN_UL_X + 150 - 10
#define PERS_CURR_TEAM_COST_Y LAPTOP_SCREEN_UL_Y + 218

#define PERS_CURR_TEAM_HIGHEST_Y PERS_CURR_TEAM_COST_Y + 15
#define PERS_CURR_TEAM_LOWEST_Y PERS_CURR_TEAM_HIGHEST_Y + 15

#define PERS_CURR_TEAM_WIDTH 286 - 160

#define PERS_DEPART_TEAM_WIDTH PERS_CURR_TEAM_WIDTH - 20

#define PERS_STAT_AVG_X LAPTOP_SCREEN_UL_X + 157 - 10
#define PERS_STAT_AVG_Y LAPTOP_SCREEN_UL_Y + 274
#define PERS_STAT_AVG_WIDTH 202 - 159
#define PERS_STAT_LOWEST_X LAPTOP_SCREEN_UL_X + 72 - 10
#define PERS_STAT_LOWEST_WIDTH 155 - 75
#define PERS_STAT_HIGHEST_X LAPTOP_SCREEN_UL_X + 205 - 10
#define PERS_STAT_LIST_X LAPTOP_SCREEN_UL_X + 33 - 10

#define PERS_TOGGLE_CUR_DEPART_WIDTH 106 - 35
#define PERS_TOGGLE_CUR_DEPART_HEIGHT 236 - 212

#define PERS_TOGGLE_CUR_DEPART_X LAPTOP_SCREEN_UL_X + 35 - 10
#define PERS_TOGGLE_CUR_Y LAPTOP_SCREEN_UL_Y + 208
#define PERS_TOGGLE_DEPART_Y LAPTOP_SCREEN_UL_Y + 238

#define PERS_DEPARTED_UP_X	LAPTOP_SCREEN_UL_X + 265 - 10
#define PERS_DEPARTED_UP_Y	LAPTOP_SCREEN_UL_Y + 210
#define PERS_DEPARTED_DOWN_Y LAPTOP_SCREEN_UL_Y + 237

#define PERS_TITLE_X									iScreenWidthOffset + 140
#define PERS_TITLE_Y									iScreenHeightOffset + 33

#define ATM_UL_X LAPTOP_SCREEN_UL_X + 397
#define ATM_UL_Y LAPTOP_SCREEN_UL_Y + 27

/// atm font
#define ATM_FONT PERS_FONT

// atm button positions
#define ATM_BUTTONS_START_Y								iScreenHeightOffset + 110
#define ATM_BUTTONS_START_X								iScreenWidthOffset + 510
#define ATM_BUTTON_WIDTH 15
#define ATM_BUTTON_HEIGHT 15
#define ATM_DISPLAY_X									iScreenWidthOffset + 509
#define ATM_DISPLAY_Y									iScreenHeightOffset + 58
#define ATM_DISPLAY_HEIGHT 10
#define ATM_DISPLAY_WIDTH	81


// the number of inventory items per personnel page
#define NUMBER_OF_INVENTORY_PERSONNEL 8
#define Y_SIZE_OF_PERSONNEL_SCROLL_REGION ( 422 - 219 )
#define X_SIZE_OF_PERSONNEL_SCROLL_REGION ( 589 - 573 )
#define Y_OF_PERSONNEL_SCROLL_REGION					(iScreenHeightOffset + 219)
#define X_OF_PERSONNEL_SCROLL_REGION					(iScreenWidthOffset + 573)
#define SIZE_OF_PERSONNEL_CURSOR 19

// number buttons
enum{
	OK_ATM = 0,
	DEPOSIT_ATM,
	WIDTHDRAWL_ATM,
	CANCEL_ATM,
	CLEAR_ATM,
	NUMBER_ATM_BUTTONS,
};

// enums for the buttons in the information side bar ( used with giPersonnelATMStartButton[] )
enum
{
	PERSONNEL_STAT_BTN,
	PERSONNEL_PERSONALITY_BTN,
	PERSONNEL_EMPLOYMENT_BTN,	
	PERSONNEL_INV_BTN,

	PERSONNEL_NUM_BTN,
};

UINT8	gubPersonnelInfoState = PERSONNEL_STAT_BTN;

extern BOOLEAN gfTemporaryDisablingOfLoadPendingFlag;
extern BOOLEAN fExitingLaptopFlag;
extern void HandleLapTopESCKey( void );
extern void HandleAltTabKeyInLaptop( void );
extern void HandleShiftAltTabKeyInLaptop( void );

UINT8 uiCurrentInventoryIndex = 0;

UINT32 guiSliderPosition;

// the transfer funds string
CHAR16 sTransferString[ 32 ];

INT32 giPersonnelATMSideButton[ NUMBER_ATM_BUTTONS ];
INT32 giPersonnelATMSideButtonImage[ NUMBER_ATM_BUTTONS ];

INT32 iNumberPadButtons[ 10 ];
INT32 iNumberPadButtonsImages[ 10 ];

#define PrsnlOffSetX	(-15) //-20
#define Prsnl_DATA_OffSetX	(36)
#define PrsnlOffSetY	10

POINT pPersonnelScreenPoints[]=
{
	{422+PrsnlOffSetX, 205+PrsnlOffSetY},
	{422+PrsnlOffSetX, 215+PrsnlOffSetY},
	{422+PrsnlOffSetX, 225+PrsnlOffSetY},
	{422+PrsnlOffSetX, 235+PrsnlOffSetY},
	{422+PrsnlOffSetX, 245+PrsnlOffSetY},
	{422+PrsnlOffSetX, 255+PrsnlOffSetY},
	{422+PrsnlOffSetX, 315+PrsnlOffSetY},
	{422+PrsnlOffSetX, 270+PrsnlOffSetY},
	{422+PrsnlOffSetX, 280+PrsnlOffSetY},
	{422+PrsnlOffSetX, 290+PrsnlOffSetY},
	{422+PrsnlOffSetX, 300+PrsnlOffSetY},			//10
	{422+PrsnlOffSetX, 395+PrsnlOffSetY},
	{422+PrsnlOffSetX, 385+PrsnlOffSetY},
	{422+PrsnlOffSetX, 415+PrsnlOffSetY},
	{422+PrsnlOffSetX, 425+PrsnlOffSetY},
	{422+PrsnlOffSetX, 445+PrsnlOffSetY},
	{422+PrsnlOffSetX, 380+PrsnlOffSetY}, // for contract price
	{422+PrsnlOffSetX, 435+PrsnlOffSetY},
	{140,33},	// Personnel Header
	{422+PrsnlOffSetX, 318+PrsnlOffSetY},
	{422+PrsnlOffSetX, 340+PrsnlOffSetY},	//20
	{422+PrsnlOffSetX, 355+PrsnlOffSetY},
	{422+PrsnlOffSetX, 365+PrsnlOffSetY},
	{422+PrsnlOffSetX, 375+PrsnlOffSetY},
	{422+PrsnlOffSetX, 385+PrsnlOffSetY},
	{422+PrsnlOffSetX, 395+PrsnlOffSetY},
};




UINT32 guiSCREEN;
extern UINT32 guiTITLE; // symbol already defined in laptop.cpp (jonathanl)
UINT32 guiFACE;
UINT32 guiDEPARTEDTEAM;
UINT32 guiCURRENTTEAM;
UINT32 guiPersonnelInventory;
UINT32 guiQMark;

INT32 giPersonnelButton[6];
INT32 giPersonnelButtonImage[6];
INT32 giPersonnelInventoryButtons[ 2 ];
INT32 giPersonnelInventoryButtonsImages[ 2 ];
INT32 giDepartedButtonImage[ 2 ];
INT32 giDepartedButton[ 2 ];

// buttons for ATM
INT32 giPersonnelATMStartButton[PERSONNEL_NUM_BTN];
INT32 giPersonnelATMStartButtonImage[PERSONNEL_NUM_BTN];
INT32 giPersonnelATMButton;
INT32 giPersonnelATMButtonImage;

BOOLEAN fATMFlags = 0;
BOOLEAN fOldATMFlags = 0;

// WDS - make number of mercenaries, etc. be configurable
// For the active mercenary screen...
// the id of currently displayed merc in right half of screen
INT32 iCurrentPersonSelectedId = -1;

// For the departed screen...
INT32 iCurPortraitId = 0;
INT32 giCurrentUpperLeftPortraitNumber = 0;

// which mode are we showing?..current team?...or deadly departed?
BOOLEAN fCurrentTeamMode = TRUE;

// show the atm panel?
BOOLEAN fShowAtmPanel = FALSE;
BOOLEAN fShowAtmPanelStartButton = TRUE;

// create buttons for scrolling departures
BOOLEAN fCreatePeronnelDepartureButton = FALSE;

// waitr one frame
BOOLEAN fOneFrameDelayInPersonnel = FALSE;

// whther or not we are creating mouse regions to place over portraits
BOOLEAN fCreatePersonnelPortraitMouseRegions = FALSE;

// WDS - make number of mercenaries, etc. be configurable
// mouse regions
MOUSE_REGION gPortraitMouseRegions[ MAX_MERCS_ON_SCREEN ];

MOUSE_REGION gTogglePastCurrentTeam[ 2 ];

MOUSE_REGION gMouseScrollPersonnelINV;


// create mouse regions for past/current toggles
BOOLEAN fCreateRegionsForPastCurrentToggle = FALSE;

// WDS - make number of mercenaries, etc. be configurable
// List of ids of current team members, dea or alive
std::vector<INT32>	currentTeamList (CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS, -1);
// The index of the last current team members
int maxCurrentTeamIndex = -1;
// The id of currently displayed merc
INT32 currentTeamIndex = -1;
// The id of the first merc on the screen (upper left hand corner)
INT32 currentTeamFirstIndex = 0;


void LoadPersonnelGraphics( void );
void RemovePersonnelGraphics( void );
void RenderPersonnel( void );
void RenderPersonnelStats(INT32 iId, INT32 iSlot);
void RenderPersonnelFace(INT32 iId, INT32 iSlot, BOOLEAN fDead, BOOLEAN fFired, BOOLEAN fOther );
void LeftButtonCallBack(GUI_BUTTON *btn,INT32 reason);
void RightButtonCallBack(GUI_BUTTON *btn,INT32 reason);
void PersonnelPortraitCallback( MOUSE_REGION * pRegion, INT32 iReason );
void CreatePersonnelButtons( void );
void DeletePersonnelButtons( void );
void DisplayHeader( void );
void DisplayCharName( INT32 iId, INT32 iSlot );
void DisplayCharStats(INT32 iId, INT32 iSlot);
void DisplayCharPersonality( INT32 iId, INT32 iSlot );
void SetPersonnelButtonStates( void );
void CreateDestroyButtonsForPersonnelDepartures( void );
void LoadPersonnelScreenBackgroundGraphics( void );
void DeletePersonnelScreenBackgroundGraphics( void );
void RenderPersonnelScreenBackground( void );
INT32 GetNumberOfMercsOnPlayersTeam( void );
void CreateDestroyMouseRegionsForPersonnelPortraits( void );
void DisplayPicturesOfCurrentTeam( void );
void DisplayFaceOfDisplayedMerc( );
void DisplayNumberOnCurrentTeam( void );
void DisplayNumberDeparted( void );
INT32 GetTotalDailyCostOfCurrentTeam( void );
void DisplayCostOfCurrentTeam( void );
INT32 GetLowestDailyCostOfCurrentTeam( void );
INT32 GetHighestDailyCostOfCurrentTeam( void );
INT32 GetAvgStatOfCurrentTeamStat( INT32 iStat );
void DisplayAverageStatValuesForCurrentTeam( void );
void DisplayLowestStatValuesForCurrentTeam( void );
void DisplayHighestStatValuesForCurrentTeam( void );
void DisplayPersonnelTeamStats( void );
void InitPastCharactersList( void );
INT32 GetNumberOfPastMercsOnPlayersTeam( void );
INT32 GetNumberOfOtherOnPastTeam( void );
INT32 GetNumberOfLeftOnPastTeam( void );
INT32 GetNumberOfDeadOnPastTeam( void );
void DisplayStateOfPastTeamMembers( void );
void CreateDestroyCurrentDepartedMouseRegions( void );
void PersonnelCurrentTeamCallback( MOUSE_REGION * pRegion, INT32 iReason );
void PersonnelDepartedTeamCallback( MOUSE_REGION * pRegion, INT32 iReason );
void CreateDestroyButtonsForDepartedTeamList( void );
void DepartedDownCallBack(GUI_BUTTON *btn,INT32 reason);
void DepartedUpCallBack(GUI_BUTTON *btn,INT32 reason);
void DisplayPastMercsPortraits( void );
void DisplayPortraitOfPastMerc( INT32 iId , INT32 iCounter, BOOLEAN fDead, BOOLEAN fFired, BOOLEAN fOther );
INT32 GetIdOfPastMercInSlot( INT32 iSlot );
void DisplayDepartedCharStats(INT32 iId, INT32 iSlot, INT32 iState);
void EnableDisableDeparturesButtons( void );
void DisplayDepartedCharName( INT32 iId, INT32 iSlot, INT32 iState );
INT32 GetTheStateOfDepartedMerc( INT32 iId );
void DisplayPersonnelTextOnTitleBar( void );
INT32 GetIdOfDepartedMercWithHighestStat( INT32 iStat );
INT32 GetIdOfDepartedMercWithLowestStat( INT32 iStat );
void RenderInventoryForCharacter( INT32 iId, INT32 iSlot );
void DisplayInventoryForSelectedChar( void );
INT32 GetNumberOfInventoryItemsOnCurrentMerc( void );
void CreateDestroyPersonnelInventoryScrollButtons( void );
void EnableDisableInventoryScrollButtons( void );
void PersonnelDataButtonCallback(GUI_BUTTON *btn,INT32 reason);
void HandleSliderBarClickCallback( MOUSE_REGION *pRegion, INT32 iReason );

void RenderSliderBarForPersonnelInventory( void );
void FindPositionOfPersInvSlider( void );


// check if current guy can have atm
void UpDateStateOfStartButton( void );
void HandlePersonnelKeyboard( void );


void DisplayEmploymentinformation( INT32 iId, INT32 iSlot );



// AIM merc:	Returns the amount of time left on mercs contract
// MERC merc: Returns the amount of time the merc has worked
// IMP merc:	Returns the amount of time the merc has worked
// else:			returns -1
INT32 CalcTimeLeftOnMercContract( SOLDIERTYPE *pSoldier );


// what state is the past merc in?
BOOLEAN IsPastMercDead( INT32 iId );
BOOLEAN IsPastMercFired( INT32 iId );
BOOLEAN IsPastMercOther( INT32 iId );


// display box around currently selected merc
BOOLEAN DisplayHighLightBox( void );


// grab appropriate id of soldier first being displayed
INT32 GetIdOfFirstDisplayedMerc( );

// get avg for this stat
INT32 GetAvgStatOfPastTeamStat( INT32 iStat );

// render atm panel
BOOLEAN RenderAtmPanel( void );
void DisplayATMAmount( void );

// create destroy ATM button
void CreateDestroyStartATMButton( void );
void CreateDestroyATMButton( void );
void ATMStartButtonCallback(GUI_BUTTON *btn,INT32 reason);
void ATMNumberButtonCallback(GUI_BUTTON *btn,INT32 reason);
void HandleStateOfATMButtons( void );
void ATMOtherButtonCallback(GUI_BUTTON *btn,INT32 reason);


// atm misc functions

void ATMOther2ButtonCallback(GUI_BUTTON *btn,INT32 reason);
void DisplayATMStrings( void );
void DisplayAmountOnCurrentMerc( void );
void RenderRectangleForPersonnelTransactionAmount( void );
void HandleTimedAtmModes( void );

// SANDRO - added variables for popup help text windows
MOUSE_REGION	gSkillTraitHelpTextRegion[13];
BOOLEAN fAddedTraitRegion[13] = { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE };

void AssignPersonnelCharacterTraitHelpText( UINT8 ubCharacterNumber );
void AssignPersonnelDisabilityHelpText( UINT8 ubDisabilityNumber );
void AssignPersonnelMultipleDisabilityHelpText( SOLDIERTYPE* pSoldier );
void AssignPersonnelKillsHelpText( INT32 ubProfile );
void AssignPersonnelAssistsHelpText( INT32 ubProfile );
void AssignPersonnelHitPercentageHelpText( INT32 ubProfile );
void AssignPersonnelBattlesHelpText( INT32 ubProfile );
void AssignPersonnelAchievementsHelpText( INT32 ubProfile );
void AssignPersonnelWoundsHelpText( INT32 ubProfile );
INT8 CalculateMercsAchievementPercentage( INT32 ubProfile );

// Flugente: personality info
void AssignPersonalityHelpText( SOLDIERTYPE* pSoldier, MOUSE_REGION* pMouseregion );

BOOLEAN fShowRecordsIfZero = TRUE;

void GameInitPersonnel( void )
{
	// init past characters lists
	InitPastCharactersList( );
}

void InitVariables(void)
{
	pPersonnelScreenPoints[0].x = iScreenWidthOffset + 422+PrsnlOffSetX;		//0
	pPersonnelScreenPoints[0].y = iScreenHeightOffset + 205+PrsnlOffSetY;

	pPersonnelScreenPoints[1].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[1].y = iScreenHeightOffset + 215+PrsnlOffSetY;

	pPersonnelScreenPoints[2].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[2].y = iScreenHeightOffset + 225+PrsnlOffSetY;

	pPersonnelScreenPoints[3].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[3].y = iScreenHeightOffset + 235+PrsnlOffSetY;

	pPersonnelScreenPoints[4].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[4].y = iScreenHeightOffset + 245+PrsnlOffSetY;

	pPersonnelScreenPoints[5].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[5].y = iScreenHeightOffset + 255+PrsnlOffSetY;

	pPersonnelScreenPoints[6].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[6].y = iScreenHeightOffset + 315+PrsnlOffSetY;

	pPersonnelScreenPoints[7].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[7].y = iScreenHeightOffset + 270+PrsnlOffSetY;

	pPersonnelScreenPoints[8].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[8].y = iScreenHeightOffset + 280+PrsnlOffSetY;

	pPersonnelScreenPoints[9].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[9].y = iScreenHeightOffset + 290+PrsnlOffSetY;

	pPersonnelScreenPoints[10].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[10].y = iScreenHeightOffset + 300+PrsnlOffSetY; // 10

	pPersonnelScreenPoints[11].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[11].y = iScreenHeightOffset + 395+PrsnlOffSetY;

	pPersonnelScreenPoints[12].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[12].y = iScreenHeightOffset + 385+PrsnlOffSetY;

	pPersonnelScreenPoints[13].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[13].y = iScreenHeightOffset + 415+PrsnlOffSetY;

	pPersonnelScreenPoints[14].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[14].y = iScreenHeightOffset + 425+PrsnlOffSetY;

	pPersonnelScreenPoints[15].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[15].y = iScreenHeightOffset + 445+PrsnlOffSetY;

	pPersonnelScreenPoints[16].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[16].y = iScreenHeightOffset + 380+PrsnlOffSetY;

	pPersonnelScreenPoints[17].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[17].y = iScreenHeightOffset + 435+PrsnlOffSetY;

	pPersonnelScreenPoints[18].x = iScreenWidthOffset + 140;		// Personal Header // 18
	pPersonnelScreenPoints[18].y = iScreenHeightOffset + 33;

	pPersonnelScreenPoints[18].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[18].y = iScreenHeightOffset + 330+PrsnlOffSetY;

	pPersonnelScreenPoints[19].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[19].y = iScreenHeightOffset + 333+PrsnlOffSetY;

	pPersonnelScreenPoints[20].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[20].y = iScreenHeightOffset + 355+PrsnlOffSetY;

	pPersonnelScreenPoints[21].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[21].y = iScreenHeightOffset + 365+PrsnlOffSetY;

	pPersonnelScreenPoints[22].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[22].y = iScreenHeightOffset + 375+PrsnlOffSetY;

	pPersonnelScreenPoints[23].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[23].y = iScreenHeightOffset + 385+PrsnlOffSetY;

	pPersonnelScreenPoints[24].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[24].y = iScreenHeightOffset + 395+PrsnlOffSetY;

	pPersonnelScreenPoints[25].x = iScreenWidthOffset + 422+PrsnlOffSetX;
	pPersonnelScreenPoints[25].y = iScreenHeightOffset + 405+PrsnlOffSetY;
}

void EnterPersonnel( void )
{
	InitVariables();

	// Clear out the current team list
	for (unsigned idx=0; idx < giMAXIMUM_NUMBER_OF_PLAYER_SLOTS; idx++) {
		currentTeamList[idx] = -1;
	}

	// Fill in the current team list
	maxCurrentTeamIndex = -1;
    SOLDIERTYPE *pTeamSoldier = MercPtrs[ 0 ];
	for (int idx = gTacticalStatus.Team[ pTeamSoldier->bTeam ].bFirstID; 
	     idx <= gTacticalStatus.Team[ pTeamSoldier->bTeam ].bLastID; 
		 ++idx) {
        pTeamSoldier = MercPtrs[ idx ]; 
				
		// WANNE: Bugfix: Also show the roboter in ther personnel screen. This bug was introduced in revision 2498, when Many Mercenary was included.
		//if ((pTeamSoldier->bActive) && 
		//	!(pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE)  && 
		//	/*(pTeamSoldier->stats.bLife > 0 ) && */  !AM_A_ROBOT(pTeamSoldier)  )		

		if ((pTeamSoldier->bActive) && !(pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE))				
		{		
			currentTeamList[++maxCurrentTeamIndex] = idx;
		}
	}

	if (currentTeamList[0] >= 0)
		currentTeamIndex = 0;
	else
		currentTeamIndex = -1;
	currentTeamFirstIndex = 0;

	fReDrawScreenFlag=TRUE;

	uiCurrentInventoryIndex = 0;
	guiSliderPosition = 0;

	iCurrentPersonSelectedId = -1;
	iCurPortraitId = 0;

	// load graphics for screen
	LoadPersonnelGraphics( );

	// show atm panel
	fShowAtmPanelStartButton = TRUE;

	// create buttons needed
	CreateDestroyButtonsForPersonnelDepartures( );

	// load personnel
	LoadPersonnelScreenBackgroundGraphics( );

	// render screen
	RenderPersonnel( );

	fCreatePersonnelPortraitMouseRegions = TRUE;

	CreateDestroyMouseRegionsForPersonnelPortraits( );
	// set states of en- dis able buttons
	//SetPersonnelButtonStates( );

	fCreateRegionsForPastCurrentToggle = TRUE;

	CreateDestroyCurrentDepartedMouseRegions( );

	// create buttons for screen
	CreatePersonnelButtons( );

	// set states of en- dis able buttons
	SetPersonnelButtonStates( );

	return;
}

void ExitPersonnel( void )
{
	if (!fCurrentTeamMode) {
		fCurrentTeamMode = TRUE;
		CreateDestroyButtonsForDepartedTeamList( );
		fCurrentTeamMode = FALSE;
	}

	// get rid of atm panel buttons
	fShowAtmPanelStartButton = FALSE;
	fShowAtmPanel = FALSE;
	fATMFlags = 0;
	CreateDestroyStartATMButton( );
	CreateDestroyATMButton( );

	gubPersonnelInfoState = PERSONNEL_STAT_BTN;

	CreateDestroyPersonnelInventoryScrollButtons( );

	// get rid of graphics
	RemovePersonnelGraphics( );

	DeletePersonnelScreenBackgroundGraphics(	);

	CreateDestroyButtonsForPersonnelDepartures( );

	// delete buttons
	DeletePersonnelButtons( );

	fCreatePersonnelPortraitMouseRegions = FALSE;

	// delete mouse regions
	CreateDestroyMouseRegionsForPersonnelPortraits( );

	fCreateRegionsForPastCurrentToggle = FALSE;

	CreateDestroyCurrentDepartedMouseRegions( );
	
	// SANDRO - remove the regions
	for( INT8 i = 0; i < 13; i++ )
	{
		if( fAddedTraitRegion[i] )
		{
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[i] );
			fAddedTraitRegion[i] = FALSE;
		}
	}
}

void HandlePersonnel( void )
{
	//RenderButtonsFastHelp( );
	CreateDestroyButtonsForPersonnelDepartures( );

	// create / destroy buttons for scrolling departed list
	CreateDestroyButtonsForDepartedTeamList( );

	// enable / disable departures buttons
	EnableDisableDeparturesButtons( );

	// create destroy inv buttons as needed
	CreateDestroyPersonnelInventoryScrollButtons( );

	// enable disable buttons as needed
	EnableDisableInventoryScrollButtons( );

	HandlePersonnelKeyboard( );

	// handle timed modes for ATM
	HandleTimedAtmModes( );
}

void LoadPersonnelGraphics( void )
{
	// load graphics needed for personnel screen
	VOBJECT_DESC	VObjectDesc;

	// load graphics

	// title bar
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\programtitlebar.sti", VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiTITLE));

	// the background grpahics
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\personnelwindow.sti", VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiSCREEN));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\personnel_inventory.sti", VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiPersonnelInventory));

	// load ? marks for tooltips
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\PERSONNEL_TOOLTIP_MARK.sti", VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiQMark));
}

void RemovePersonnelGraphics( void )
{
	// delete graphics needed for personnel screen

	DeleteVideoObjectFromIndex(guiSCREEN);
	DeleteVideoObjectFromIndex(guiTITLE);
	DeleteVideoObjectFromIndex(guiPersonnelInventory);
	DeleteVideoObjectFromIndex(guiQMark);
}

void RenderPersonnel( void )
{
	HVOBJECT hHandle;
	// re-renders personnel screen
	// render main background

	// blit title
	GetVideoObject(&hHandle, guiTITLE);
	BltVideoObject( FRAME_BUFFER, hHandle, 0,LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_UL_Y - 2, VO_BLT_SRCTRANSPARENCY,NULL );

	// blit screen
	GetVideoObject(&hHandle, guiSCREEN);
	BltVideoObject( FRAME_BUFFER, hHandle, 0,LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_UL_Y + 22, VO_BLT_SRCTRANSPARENCY,NULL );

	// render personnel screen background
	RenderPersonnelScreenBackground( );

	// show team
	DisplayPicturesOfCurrentTeam( );

	DisplayPastMercsPortraits( );

	// show selected merc
	DisplayFaceOfDisplayedMerc( );

	// show current team size
	DisplayNumberOnCurrentTeam( );

	// show departed team size
	DisplayNumberDeparted( );

	// list stats row headers for team stats list
	DisplayPersonnelTeamStats( );

	// showinventory of selected guy if applicable
	DisplayInventoryForSelectedChar( );

	// the average stats for the current team
	DisplayAverageStatValuesForCurrentTeam( );

	// lowest stat values
	DisplayLowestStatValuesForCurrentTeam( );

	// past team
	DisplayStateOfPastTeamMembers( );

	// title bar
	BlitTitleBarIcons(	);

	// show text on titlebar
	DisplayPersonnelTextOnTitleBar( );

	// the highest stats
	DisplayHighestStatValuesForCurrentTeam( );

	// render the atm panel
	RenderAtmPanel( );

	DisplayAmountOnCurrentMerc( );

	// en-dis-able start button
	UpDateStateOfStartButton( );
}

void RenderPersonnelStats( INT32 iId, INT32 iSlot )
{
	// will render the stats of person iId in slot iSlot
	SetFont(PERS_FONT);
	SetFontForeground(PERS_TEXT_FONT_COLOR);
	SetFontBackground(FONT_BLACK);

	if( gubPersonnelInfoState == PERSONNEL_STAT_BTN )
		DisplayCharStats(iId, iSlot);
	else if( gubPersonnelInfoState == PERSONNEL_EMPLOYMENT_BTN )
		DisplayEmploymentinformation( iId, iSlot );
	else if ( gubPersonnelInfoState == PERSONNEL_PERSONALITY_BTN )
		DisplayCharPersonality( iId, iSlot );
}

// ID -> fortlaufende ID, und nicht die mercID
// -> bei aktuellen Merc passt es
// -> bei departed Merc wird die MercId anstatt der fortlaufenden ID übergeben!!
void RenderPersonnelFace(INT32 iId, INT32 iSlot, BOOLEAN fDead, BOOLEAN fFired, BOOLEAN fOther )
{
	// Get the profile id (from profileId or slotId)
	INT32 profileId = iId;
	if (profileId == -1)
	{
		profileId = MercPtrs[iSlot]->ubProfile;
	}

	if ( profileId < 0 )
		return;

	char sTemp[100];
	HVOBJECT hFaceHandle;
	VOBJECT_DESC	VObjectDesc;
	// draw face to soldier iId in slot iSlot

	// Currently active merc!
	// special case?..player generated merc
	if (fCurrentTeamMode) 
	{
		if ( gMercProfiles[profileId].Type == PROFILETYPE_IMP )
		{
			sprintf( sTemp, "%s%02d.sti", IMP_FACES_DIR, gMercProfiles[profileId].ubFaceIndex );
		}
		else
		{
			sprintf( sTemp, "%s%02d.sti", FACES_DIR, gMercProfiles[profileId].ubFaceIndex );
		}
		
		// TODO: Check if needed!
		if( MercPtrs[iSlot]->flags.uiStatusFlags & SOLDIER_VEHICLE ) 
		{
			return;
		}
	} 
	// departed mercs
	else 
	{
		//if this is not a valid merc
		if( !fDead && !fFired && !fOther ) 
		{
			return;
		}

		if ( gMercProfiles[profileId].Type == PROFILETYPE_IMP )
		{
			sprintf( sTemp, "%s%02d.sti", IMP_FACES_DIR, gMercProfiles[profileId].ubFaceIndex );
		}
		else
		{
			sprintf( sTemp, "%s%02d.sti", FACES_DIR, gMercProfiles[profileId].ubFaceIndex );
		}
	}

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP(sTemp, VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiFACE));

	//Blt face to screen to
	GetVideoObject(&hFaceHandle, guiFACE);

	if (fCurrentTeamMode) 
	{
		if( MercPtrs[iSlot]->stats.bLife <= 0 ) 
		{
			hFaceHandle->pShades[ 0 ]		= Create16BPPPaletteShaded( hFaceHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );
			//set the red pallete to the face
			SetObjectHandleShade( guiFACE, 0 );
		}
	} 
	else 
	{
		if (fDead == TRUE)
		{
			hFaceHandle->pShades[ 0 ]		= Create16BPPPaletteShaded( hFaceHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );
			//set the red pallete to the face
			SetObjectHandleShade( guiFACE, 0 );
		}
	}

	// TODO:Check
	BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,IMAGE_BOX_X, IMAGE_BOX_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//if the merc is dead, display it
	if (!fCurrentTeamMode)
	{
		INT32 iHeightOfText = DisplayWrappedString(IMAGE_BOX_X, (UINT16)(IMAGE_BOX_Y+IMAGE_FULL_NAME_OFFSET_Y), IMAGE_NAME_WIDTH, 1, PERS_FONT, PERS_FONT_COLOR, gMercProfiles[profileId].zName, 0, FALSE, CENTER_JUSTIFIED | DONT_DISPLAY_TEXT );

		//if the string will rap
		if( ( iHeightOfText - 2 ) > GetFontHeight( PERS_FONT ) ) 
		{
			//raise where we display it, and rap it
			DisplayWrappedString(IMAGE_BOX_X, (UINT16)(IMAGE_BOX_Y+IMAGE_FULL_NAME_OFFSET_Y - GetFontHeight( PERS_FONT )), IMAGE_NAME_WIDTH, 1, PERS_FONT, PERS_FONT_COLOR, gMercProfiles[profileId].zName, 0, FALSE, CENTER_JUSTIFIED);
		} 
		else 
		{
			DrawTextToScreen( gMercProfiles[profileId].zName, IMAGE_BOX_X, (UINT16)(IMAGE_BOX_Y+IMAGE_FULL_NAME_OFFSET_Y), IMAGE_NAME_WIDTH, PERS_FONT, PERS_FONT_COLOR, 0, FALSE, CENTER_JUSTIFIED );
		}
	}

	DeleteVideoObjectFromIndex(guiFACE);
}


// WDS - make number of mercenaries, etc. be configurable
static BOOLEAN NextPersonnelFace( void )
{
	if (fCurrentTeamMode)
	{
		// Anyone to display?
		if (currentTeamIndex == -1) {
			return TRUE;
		}

		fReDrawScreenFlag = TRUE;
		// wrap around?
		if (currentTeamIndex == maxCurrentTeamIndex) {
			currentTeamIndex = 0;
			currentTeamFirstIndex = 0;
			return FALSE;
		} else if ((currentTeamIndex+1) % MAX_MERCS_ON_SCREEN == 0) {
			currentTeamFirstIndex += MAX_MERCS_ON_SCREEN;
		}
		++currentTeamIndex;
	} else {
		// Anyone to display?
		if (iCurPortraitId == -1) {
			return TRUE;
		}

		if ( ( ( iCurPortraitId + 1) == ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) - giCurrentUpperLeftPortraitNumber ) ) {
			// about to go off the end
			giCurrentUpperLeftPortraitNumber = 0;
			iCurPortraitId = 0;
		} else if(	iCurPortraitId == MAX_MERCS_ON_SCREEN-1 ) {
			giCurrentUpperLeftPortraitNumber += MAX_MERCS_ON_SCREEN;
			iCurPortraitId = 0;
		} else {
			iCurPortraitId++;
		}
		// get of this merc in this slot

		iCurrentPersonSelectedId =	iCurPortraitId;
		fReDrawScreenFlag = TRUE;
	}

	return TRUE;
}

static BOOLEAN PrevPersonnelFace( void )
{
	if (fCurrentTeamMode) {
		// Anyone to display?
		if (currentTeamIndex == -1) {
			return TRUE;
		}

		fReDrawScreenFlag = TRUE;
		// wrap around?
		if (currentTeamIndex == 0 ) {
			currentTeamIndex = maxCurrentTeamIndex;
			currentTeamFirstIndex = (currentTeamIndex / MAX_MERCS_ON_SCREEN) * MAX_MERCS_ON_SCREEN;
			if (currentTeamIndex == 0) {
				return FALSE;
			}
		} else {
			if ((currentTeamIndex) % MAX_MERCS_ON_SCREEN == 0) {
				currentTeamFirstIndex -= MAX_MERCS_ON_SCREEN;
			}
			currentTeamIndex--;
		}
	}
	else
	{
		// Anyone to display?
		if (iCurPortraitId == -1) {
			return TRUE;
		}

		if	( ( iCurPortraitId == 0 ) && ( giCurrentUpperLeftPortraitNumber == 0 ) )
		{
			// about to go off the end
			giCurrentUpperLeftPortraitNumber = ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) - ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) % MAX_MERCS_ON_SCREEN;
			iCurPortraitId = ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) % MAX_MERCS_ON_SCREEN ;
			iCurPortraitId--;

		}
		else if(	iCurPortraitId == 0 )
		{
			giCurrentUpperLeftPortraitNumber -= MAX_MERCS_ON_SCREEN;
			iCurPortraitId = MAX_MERCS_ON_SCREEN-1;
		}
		else
		{
			iCurPortraitId--;
		}
		// get of this merc in this slot

		iCurrentPersonSelectedId =	iCurPortraitId;
		fReDrawScreenFlag = TRUE;
	}

	return TRUE;
}

static BOOLEAN NextPersonnelFacePage(void)
{
	if (fCurrentTeamMode)
	{
		// Anyone to display?
		if (currentTeamIndex == -1) {
			return TRUE;
		}

		fReDrawScreenFlag = TRUE;
		// wrap around?
		currentTeamIndex += MAX_MERCS_ON_SCREEN;
		if (currentTeamIndex >= maxCurrentTeamIndex)
		{
			currentTeamIndex = 0;
			currentTeamFirstIndex = 0;
			return FALSE;
		}
		else
		{
			currentTeamFirstIndex += MAX_MERCS_ON_SCREEN;
		}
	}
	else {
		// Anyone to display?
		if (iCurPortraitId == -1) {
			return TRUE;
		}

		iCurPortraitId += MAX_MERCS_ON_SCREEN;
		if (iCurPortraitId >= (GetNumberOfDeadOnPastTeam() + GetNumberOfLeftOnPastTeam() + GetNumberOfOtherOnPastTeam()) - giCurrentUpperLeftPortraitNumber)
		{
			// about to go off the end
			giCurrentUpperLeftPortraitNumber = 0;
			iCurPortraitId = 0;
		}
		else
		{
			giCurrentUpperLeftPortraitNumber += MAX_MERCS_ON_SCREEN;
			iCurPortraitId = 0;
		}

		// get of this merc in this slot
		iCurrentPersonSelectedId = iCurPortraitId;
		fReDrawScreenFlag = TRUE;
	}

	return TRUE;
}

static BOOLEAN PrevPersonnelFacePage(void)
{
	if (fCurrentTeamMode) {
		// Anyone to display?
		if (currentTeamIndex == -1) {
			return TRUE;
		}

		fReDrawScreenFlag = TRUE;
		currentTeamIndex -= MAX_MERCS_ON_SCREEN;
		// wrap around?
		if (currentTeamIndex <= 0) {
			currentTeamIndex = maxCurrentTeamIndex;
			currentTeamFirstIndex = (currentTeamIndex / MAX_MERCS_ON_SCREEN) * MAX_MERCS_ON_SCREEN;
			if (currentTeamIndex == 0) {
				return FALSE;
			}
		}
		else {
			currentTeamFirstIndex -= MAX_MERCS_ON_SCREEN;
		}
	}
	else
	{
		// Anyone to display?
		if (iCurPortraitId == -1) {
			return TRUE;
		}

		iCurPortraitId -= MAX_MERCS_ON_SCREEN;

		if ((iCurPortraitId <= 0) && (giCurrentUpperLeftPortraitNumber == 0))
		{
			// about to go off the end
			giCurrentUpperLeftPortraitNumber = (GetNumberOfDeadOnPastTeam() + GetNumberOfLeftOnPastTeam() + GetNumberOfOtherOnPastTeam()) - (GetNumberOfDeadOnPastTeam() + GetNumberOfLeftOnPastTeam() + GetNumberOfOtherOnPastTeam()) % MAX_MERCS_ON_SCREEN;
			iCurPortraitId = (GetNumberOfDeadOnPastTeam() + GetNumberOfLeftOnPastTeam() + GetNumberOfOtherOnPastTeam()) % MAX_MERCS_ON_SCREEN;
		}
		else
		{
			giCurrentUpperLeftPortraitNumber -= MAX_MERCS_ON_SCREEN;
			iCurPortraitId = MAX_MERCS_ON_SCREEN - 1;
		}
		// get of this merc in this slot

		iCurrentPersonSelectedId = iCurPortraitId;
		fReDrawScreenFlag = TRUE;
	}

	return TRUE;
}


void CreatePersonnelButtons( void )
{

	// left button
	giPersonnelButtonImage[0]=	LoadButtonImage( "LAPTOP\\personnelbuttons.sti" ,-1,0,-1,1,-1 );
	giPersonnelButton[0] = QuickCreateButton( giPersonnelButtonImage[0], PREV_MERC_FACE_X, MERC_FACE_SCROLL_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)LeftButtonCallBack);

	// right button
	giPersonnelButtonImage[1]=	LoadButtonImage( "LAPTOP\\personnelbuttons.sti" ,-1,2,-1,3,-1 );
	giPersonnelButton[1] = QuickCreateButton( giPersonnelButtonImage[1], NEXT_MERC_FACE_X, MERC_FACE_SCROLL_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)RightButtonCallBack);



	/*
	// left button
	giPersonnelButtonImage[0]=	LoadButtonImage( "LAPTOP\\arrows.sti" ,-1,0,-1,1,-1 );
	giPersonnelButton[0] = QuickCreateButton( giPersonnelButtonImage[0], LEFT_BUTTON_X, BUTTON_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)LeftButtonCallBack);

	// right button
	giPersonnelButtonImage[1]=	LoadButtonImage( "LAPTOP\\arrows.sti" ,-1,6,-1,7,-1 );
	giPersonnelButton[1] = QuickCreateButton( giPersonnelButtonImage[1], RIGHT_BUTTON_X, BUTTON_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)RightButtonCallBack);

	// left FF button
	giPersonnelButtonImage[2]=	LoadButtonImage( "LAPTOP\\arrows.sti" ,-1,3,-1,4,-1 );
	giPersonnelButton[2] = QuickCreateButton( giPersonnelButtonImage[2], LEFT_BUTTON_X, BUTTON_Y + 22,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)LeftFFButtonCallBack);

	// right ff button
	giPersonnelButtonImage[3]=	LoadButtonImage( "LAPTOP\\arrows.sti" ,-1,9,-1,10,-1 );
	giPersonnelButton[3] = QuickCreateButton( giPersonnelButtonImage[3], RIGHT_BUTTON_X, BUTTON_Y + 22,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)RightFFButtonCallBack);
	*/
	// set up cursors
	SetButtonCursor(giPersonnelButton[0], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(giPersonnelButton[1], CURSOR_LAPTOP_SCREEN);
	//SetButtonCursor(giPersonnelButton[2], CURSOR_LAPTOP_SCREEN);
	//SetButtonCursor(giPersonnelButton[3], CURSOR_LAPTOP_SCREEN);


	return;
}

void DeletePersonnelButtons( void )
{

	RemoveButton(giPersonnelButton[0] );
	UnloadButtonImage(giPersonnelButtonImage[0] );
	RemoveButton(giPersonnelButton[1] );
	UnloadButtonImage( giPersonnelButtonImage[1] );
	/*RemoveButton(giPersonnelButton[2] );
	UnloadButtonImage( giPersonnelButtonImage[2] );
	RemoveButton(giPersonnelButton[3] );
	UnloadButtonImage( giPersonnelButtonImage[3] );
	*/
	return;
}


void LeftButtonCallBack(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			fReDrawScreenFlag=TRUE;
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		fReDrawScreenFlag=TRUE;
		if (_KeyDown(SHIFT))
		{
			PrevPersonnelFacePage();
		}
		else
		{
			PrevPersonnelFace( );
		}
		uiCurrentInventoryIndex = 0;
		guiSliderPosition = 0;

		}
	}
}

void RightButtonCallBack(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			fReDrawScreenFlag=TRUE;
		}
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fReDrawScreenFlag=TRUE;
			if (_KeyDown(SHIFT))
			{
				NextPersonnelFacePage();
			}
			else
			{
				NextPersonnelFace( );
			}
			uiCurrentInventoryIndex = 0;
			guiSliderPosition = 0;
		}
	}
}

void DisplayHeader( void )
{
	SetFont(PERS_HEADER_FONT);
	SetFontForeground(PERS_FONT_COLOR);
	SetFontBackground( 0 );

	mprintf(pPersonnelScreenPoints[18].x,pPersonnelScreenPoints[18].y,pPersonnelTitle[0]);

	return;
}


void DisplayCharName( INT32 iId, INT32 iSlot )
{
	// get merc's nickName, assignment, and sector location info
	INT16 sX, sY;
	SOLDIERTYPE *pSoldier;
	CHAR16 sString[ 64 ];
	CHAR16 sTownName[ 256 ];
	INT8 bTownId =	-1;
	INT32	iHeightOfText;

	sTownName[0] = L'\0';

	pSoldier=MercPtrs[iId];

	SetFont(CHAR_NAME_FONT);
	SetFontForeground(PERS_TEXT_FONT_COLOR);
	SetFontBackground(FONT_BLACK);

	if( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	if( Menptr[iId].bAssignment == ASSIGNMENT_POW )
	{
	}
	else if( Menptr[iId].bAssignment == IN_TRANSIT )
	{
	}
	else
	{
		// name of town, if any
		bTownId = GetTownIdForSector( Menptr[iId].sSectorX, Menptr[iId].sSectorY );

		if( bTownId != BLANK_SECTOR )
		{
			swprintf( sTownName, L"%s", pTownNames[ bTownId ] );
		}
	}



	if( sTownName[0] != L'\0' )
	{
		//nick name - town name
		swprintf( sString, L"%s - %s", gMercProfiles[Menptr[iId].ubProfile].zNickname, sTownName );
	}
	else
	{
		//nick name
		swprintf( sString, L"%s", gMercProfiles[Menptr[iId].ubProfile].zNickname );
	}


	// nick name - assignment
	FindFontCenterCoordinates(IMAGE_BOX_X-5,0,IMAGE_BOX_WIDTH + 90 , 0,sString,CHAR_NAME_FONT, &sX, &sY );

	// check to see if we are going to go off the left edge
	if( sX < pPersonnelScreenPoints[ 0 ].x )
	{
		sX = ( INT16 )pPersonnelScreenPoints[ 0 ].x;
	}

	//Display the mercs name
	mprintf( sX + iSlot*IMAGE_BOX_WIDTH, CHAR_NAME_Y, sString );

	if ( gGameExternalOptions.fUseXMLSquadNames && Menptr[iId].bAssignment < min(ON_DUTY, gSquadNameVector.size() ) )
		swprintf( sString, L"%s", gSquadNameVector[Menptr[iId].bAssignment].c_str() );
	else
		swprintf( sString, L"%s", pPersonnelAssignmentStrings[Menptr[iId].bAssignment]);

	// nick name - assignment
	FindFontCenterCoordinates(IMAGE_BOX_X-5,0,IMAGE_BOX_WIDTH + 90 , 0,sString,CHAR_NAME_FONT, &sX, &sY );

	// check to see if we are going to go off the left edge
	if( sX < pPersonnelScreenPoints[ 0 ].x )
	{
		sX = ( INT16 )pPersonnelScreenPoints[ 0 ].x;
	}

	mprintf(sX+iSlot*IMAGE_BOX_WIDTH, CHAR_LOC_Y, sString );


	//
	// Display the mercs FULL name over top of their portrait
	//

	//first get height of text to be displayed
	iHeightOfText = DisplayWrappedString(IMAGE_BOX_X, (UINT16)(IMAGE_BOX_Y+IMAGE_FULL_NAME_OFFSET_Y), IMAGE_NAME_WIDTH, 1, PERS_FONT, PERS_FONT_COLOR, gMercProfiles[Menptr[iId].ubProfile].zName, 0, FALSE, CENTER_JUSTIFIED | DONT_DISPLAY_TEXT );

	//if the string will rap
	if( ( iHeightOfText - 2 ) > GetFontHeight( PERS_FONT ) )
	{
		//raise where we display it, and rap it
		DisplayWrappedString(IMAGE_BOX_X, (UINT16)(IMAGE_BOX_Y+IMAGE_FULL_NAME_OFFSET_Y - GetFontHeight( PERS_FONT )), IMAGE_NAME_WIDTH, 1, PERS_FONT, PERS_FONT_COLOR, gMercProfiles[Menptr[iId].ubProfile].zName, 0, FALSE, CENTER_JUSTIFIED);
	}
	else
	{
		DrawTextToScreen( gMercProfiles[Menptr[iId].ubProfile].zName, IMAGE_BOX_X, (UINT16)(IMAGE_BOX_Y+IMAGE_FULL_NAME_OFFSET_Y), IMAGE_NAME_WIDTH, PERS_FONT, PERS_FONT_COLOR, 0, FALSE, CENTER_JUSTIFIED );
	}

/*
Moved so the name of the town will be in the same line as the name


	if( Menptr[iId].bAssignment == ASSIGNMENT_POW )
	{
//		FindFontCenterCoordinates(IMAGE_BOX_X-5,0,IMAGE_BOX_WIDTH, 0,pPOWStrings[ 1 ],CHAR_NAME_FONT, &sX, &sY );
//	mprintf(sX+iSlot*IMAGE_BOX_WIDTH, CHAR_NAME_Y+20,pPOWStrings[ 1 ] );
	}
	else if( Menptr[iId].bAssignment == IN_TRANSIT )
	{
		return;
	}
	else
	{
		// name of town, if any
		bTownId = GetTownIdForSector( Menptr[iId].sSectorX, Menptr[iId].sSectorY );

		if( bTownId != BLANK_SECTOR )
		{
			FindFontCenterCoordinates( IMAGE_BOX_X - 5, 0, IMAGE_BOX_WIDTH, 0, pTownNames[ bTownId ], CHAR_NAME_FONT, &sX, &sY );
			mprintf( sX + ( iSlot * IMAGE_BOX_WIDTH ), CHAR_NAME_Y + 20, pTownNames[ bTownId ]);
		}
	}
*/

	return;
}


void DisplayCharStats( INT32 iId, INT32 iSlot )
{
	INT32 iCounter = 0;
	CHAR16 sString[50];
	//	CHAR16 sStringA[ 50 ];
	INT16 sX, sY;
	UINT32 uiHits = 0;
	SOLDIERTYPE *pSoldier = &Menptr[iId];
	BOOLEAN	fAmIaRobot = AM_A_ROBOT( pSoldier );

	// SANDRO - remove the regions
	for ( INT8 i = 0; i < 13; i++ )
	{
		if ( fAddedTraitRegion[i] )
		{
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[i] );
			fAddedTraitRegion[i] = FALSE;
		}
	}

	if ( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	// display the stats for a char
	for ( iCounter = 0; iCounter <MAX_STATS; iCounter++ )
	{
		switch ( iCounter )
		{
		case 0:
			// health
			if ( Menptr[iId].bAssignment != ASSIGNMENT_POW )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bLifeDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_HEALTH]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d/%d", Menptr[iId].stats.bLife, Menptr[iId].stats.bLifeMax );
			}
			else
			{
				swprintf( sString, pPOWStrings[1] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[PRSNL_TXT_HEALTH] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 1:
			// agility
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bAgilityDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_AGILITY]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bAgility );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 2:
			// dexterity
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bDexterityDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_DEXTERITY]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bDexterity );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 3:
			// strength
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bStrengthDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_STRENGTH]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bStrength );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 4:
			// leadership
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bLeadershipDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_LEADERSHIP]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bLeadership );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 5:
			// wisdom
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bWisdomDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_WISDOM]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bWisdom );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 6:
			// exper
			if ( !fAmIaRobot )
			{

				if ( gMercProfiles[Menptr[iId].ubProfile].bExpLevelDelta > 0 )
				{
					swprintf( sString, L"( %+d )", gMercProfiles[Menptr[iId].ubProfile].bExpLevelDelta );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}
				//else
				//{
				swprintf( sString, L"%d", Menptr[iId].stats.bExpLevel );
				//}
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}


			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 7:
			//mrkmanship
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bMarksmanshipDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_MARKSMANSHIP]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bMarksmanship );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 8:
			// mech
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bMechanicDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_MECHANICAL]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bMechanical );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 9:
			// exp
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bExplosivesDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_EXPLOSIVES]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bExplosive );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;
		case 10:
			// med
			if ( !fAmIaRobot )
			{
				// Flugente: stats can have gone up or down, find out which 
				INT16 change = gMercProfiles[Menptr[iId].ubProfile].bMedicalDelta - (INT16)(pSoldier->ubCriticalStatDamage[DAMAGED_STAT_MEDICAL]);

				if ( change )
				{
					swprintf( sString, change > 0 ? L"( +%d )" : L"( %d )", change );
					FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
					mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
				}

				swprintf( sString, L"%d", Menptr[iId].stats.bMedical );
			}
			else
			{
				swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
			}

			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), pPersonnelScreenPoints[iCounter].y, pPersonnelScreenStrings[iCounter] );
			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, pPersonnelScreenPoints[iCounter].y, sString );
			break;

			// Added by Flugente
		case 16:
			// Background

			// display background
			if (UsingBackGroundSystem())
			{
				UINT8 loc = 21;
				UINT8 regionnr = 12;

				mprintf( (INT16)(pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + 15), pPersonnelRecordsHelpTexts[47] ); //L"Background:"

				if ( !gMercProfiles[pSoldier->ubProfile].usBackground )
					swprintf( sString, pwMiscSectorStrings[3] ); //L"unknown"
				else
					swprintf( sString, zBackground[gMercProfiles[pSoldier->ubProfile].usBackground].szShortName );

				FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
				mprintf( sX, (pPersonnelScreenPoints[loc].y + 15), sString );

				// Add specific region for fast help window
				if ( fAddedTraitRegion[regionnr] )
				{
					MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[regionnr] );
				}
				MSYS_DefineRegion( &gSkillTraitHelpTextRegion[regionnr], (sX - 3), (UINT16)(pPersonnelScreenPoints[loc].y + 15),
								   (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[loc].y + 23), MSYS_PRIORITY_HIGH,
								   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );

				MSYS_AddRegion( &gSkillTraitHelpTextRegion[regionnr] );
				fAddedTraitRegion[regionnr] = TRUE;

				// Info about our background
				AssignBackgroundHelpText( gMercProfiles[pSoldier->ubProfile].usBackground, &(gSkillTraitHelpTextRegion[12]) );
			}

			break;

			// Added by Flugente
		case 17:
			// sexism, racism, nationalities etc.
		{
				   UINT8 loc = 22;
				   UINT8 regionnr = 8;

				   mprintf( (INT16)(pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + 15), pPersonnelRecordsHelpTexts[48] ); //L"Personality:"

				   swprintf( sString, L"->" );

				   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
				   mprintf( sX, (pPersonnelScreenPoints[loc].y + 15), sString );

				   // Add specific region for fast help window
				   if ( fAddedTraitRegion[regionnr] )
				   {
					   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[regionnr] );
				   }
				   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[regionnr], (sX - 3), (UINT16)(pPersonnelScreenPoints[loc].y + 15),
									  (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[loc].y + 23), MSYS_PRIORITY_HIGH,
									  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );

				   MSYS_AddRegion( &gSkillTraitHelpTextRegion[regionnr] );
				   fAddedTraitRegion[regionnr] = TRUE;

				   // assign bubblehelp text
				   AssignPersonalityHelpText( pSoldier, &(gSkillTraitHelpTextRegion[regionnr]) );
		}

			break;

			/////////////////////////////////////////////////////////////////////////
			// Added by SANDRO
		case 15:
			// Character Trait

			// WANNE: With old trait system, display "Attitudes" instead of "Character"
			if ( gGameOptions.fNewTraitSystem )
				mprintf( (INT16)(pPersonnelScreenPoints[23].x + (iSlot*TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[23].y + 15), pPersonnelRecordsHelpTexts[43] ); //L"Character:"
			else
				mprintf( (INT16)(pPersonnelScreenPoints[23].x + (iSlot*TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[23].y + 15), pPersonnelRecordsHelpTexts[45] ); //L"Attitudes:"

			if ( gGameOptions.fNewTraitSystem )
				swprintf( sString, gzIMPCharacterTraitText[gMercProfiles[Menptr[iId].ubProfile].bCharacterTrait] );
			else
				swprintf( sString, gzIMPAttitudesText[gMercProfiles[Menptr[iId].ubProfile].bAttitude] );

			FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[23].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
			mprintf( sX, (pPersonnelScreenPoints[23].y + 15), sString );

			//GetVideoObject(&hHandle, guiQMark);
			//BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[23].x + 148), ( pPersonnelScreenPoints[23].y + 5), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if ( fAddedTraitRegion[5] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[5] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[5], (sX - 3), (UINT16)(pPersonnelScreenPoints[23].y + 15),	// 10
							   (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[23].y + 23), MSYS_PRIORITY_HIGH,	// 17
							   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			//MSYS_DefineRegion( &gSkillTraitHelpTextRegion[3], (UINT16)( pPersonnelScreenPoints[23].x + 147 ), (UINT16)(pPersonnelScreenPoints[23].y + 4),
			//				(UINT16)( pPersonnelScreenPoints[23].x + 166 ), (UINT16)(pPersonnelScreenPoints[23].y + 15), MSYS_PRIORITY_HIGH,
			//					MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[5] );
			fAddedTraitRegion[5] = TRUE;
			// Assign the text

			// Only new traits have help text
			if ( gGameOptions.fNewTraitSystem )
				AssignPersonnelCharacterTraitHelpText( gMercProfiles[Menptr[iId].ubProfile].bCharacterTrait );

			break;

		case 18:
			// Disability
			{
				UINT8 loc = 25;
				UINT8 regionnr = 6;

				mprintf( (INT16)( pPersonnelScreenPoints[loc].x + ( iSlot*TEXT_BOX_WIDTH ) ), ( pPersonnelScreenPoints[loc].y + 10 ), pPersonnelRecordsHelpTexts[44] ); //L"Disability:"

				int numdisabilities = 0;
				UINT8 disabilityfound = 0;
				for ( UINT8 i = NO_DISABILITY + 1; i < NUM_DISABILITIES; ++i )
				{
					if ( DoesMercHaveDisability( pSoldier, i ) )
					{
						disabilityfound = i;
						++numdisabilities;
					}
				}

				if ( numdisabilities <= 1 )
					swprintf( sString, gzIMPDisabilityTraitText[disabilityfound] );
				else
					swprintf( sString, L"%s, ...", gzIMPDisabilityTraitText[disabilityfound] );
				
				FindFontRightCoordinates( (INT16)( pPersonnelScreenPoints[loc].x + ( iSlot*TEXT_BOX_WIDTH ) ), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
				mprintf( sX, ( pPersonnelScreenPoints[loc].y + 10 ), sString );

				// Add specific region for fast help window
				if ( fAddedTraitRegion[regionnr] )
				{
					MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[regionnr] );
				}
				MSYS_DefineRegion( &gSkillTraitHelpTextRegion[regionnr], ( sX - 3 ), (UINT16)( pPersonnelScreenPoints[loc].y + 10 ),
					( sX + StringPixLength( sString, PERS_FONT ) + 3 ), (UINT16)( pPersonnelScreenPoints[loc].y + 17 ), MSYS_PRIORITY_HIGH,
					MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );

				MSYS_AddRegion( &gSkillTraitHelpTextRegion[regionnr] );
				fAddedTraitRegion[regionnr] = TRUE;

				// Assign the text
				if ( numdisabilities <= 1 )
					AssignPersonnelDisabilityHelpText( gMercProfiles[Menptr[iId].ubProfile].bDisability );
				else
					AssignPersonnelMultipleDisabilityHelpText( pSoldier );
			}
			break;
			/////////////////////////////////////////////////////////////////////////

			// The Mercs Skills
		case 19:
		{
				   INT32 iWidth;
				   INT32 iMinimumX;
				   INT8	bScreenLocIndex = 19;	//if you change the '19', change it below in the if statement

				   //Display the 'Skills' text
				   mprintf( (INT16)(pPersonnelScreenPoints[bScreenLocIndex].x + (iSlot*TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[bScreenLocIndex].y), pPersonnelScreenStrings[PRSNL_TXT_SKILLS] );

				   //KM: April 16, 1999
				   //Added support for the German version, which has potential string overrun problems.	For example, the text "Skills:" can
				   //overlap "NightOps (Expert)" because the German strings are much longer.	In these cases, I ensure that the right
				   //justification of the traits don't overlap.	If it would, I move it over to the right.
				   iWidth = StringPixLength( pPersonnelScreenStrings[PRSNL_TXT_SKILLS], PERS_FONT );
				   iMinimumX = iWidth + pPersonnelScreenPoints[bScreenLocIndex].x + iSlot * TEXT_BOX_WIDTH + 2;

				   if ( !fAmIaRobot )
				   {
					   if ( gGameOptions.fNewTraitSystem ) // SANDRO - old/new traits check
					   {
						   UINT8 ubTempSkillArray[30];
						   INT8 bNumSkillTraits = 0;

						   // lets rearrange our skills to a temp array
						   // we also get the number of lines (skills) to be displayed 
						   for ( UINT8 ubCnt = 1; ubCnt < NUM_SKILLTRAITS_NT; ubCnt++ )
						   {
							   if ( ProfileHasSkillTrait( Menptr[iId].ubProfile, ubCnt ) == 2 )
							   {
								   ubTempSkillArray[bNumSkillTraits] = (ubCnt + NEWTRAIT_MERCSKILL_EXPERTOFFSET);
								   bNumSkillTraits++;
							   }
							   else if ( ProfileHasSkillTrait( Menptr[iId].ubProfile, ubCnt ) == 1 )
							   {
								   ubTempSkillArray[bNumSkillTraits] = ubCnt;
								   bNumSkillTraits++;
							   }
						   }

						   if ( bNumSkillTraits == 0 )
						   {
							   swprintf( sString, L"%s", pPersonnelScreenStrings[PRSNL_TXT_NOSKILLS] );

							   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[bScreenLocIndex].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
							   mprintf( sX, pPersonnelScreenPoints[bScreenLocIndex].y, sString );

							   // Add specific region for fast help window
							   if ( fAddedTraitRegion[0] )
							   {
								   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[0] );
							   }
							   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[0], (sX - 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y),
												  (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y + 7), MSYS_PRIORITY_HIGH,
												  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
							   MSYS_AddRegion( &gSkillTraitHelpTextRegion[0] );
							   fAddedTraitRegion[0] = TRUE;

							   // Assign the text
							   CHAR16	apStr[5000];
							   swprintf( apStr, L"" );
							   AssignPersonnelSkillTraitHelpText( 0, FALSE, (gMercProfiles[iId].ubBodyType == REGMALE), apStr );

							   // Set region help text
							   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[0]), apStr );
							   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[0], MSYS_NO_CALLBACK );
						   }
						   else
						   {
							   CHAR16 sString2[500];
							   swprintf( sString2, L"" );
							   BOOLEAN fDisplayMoreTraits = FALSE;

							   for ( UINT8 ubCnt = 0; ubCnt < bNumSkillTraits; ubCnt++ )
							   {
								   if ( ubCnt >= 3 && bNumSkillTraits > 4 )
								   {
									   fDisplayMoreTraits = TRUE;
									   swprintf( sString, L"%s\n", gzMercSkillTextNew[ubTempSkillArray[ubCnt]] );
									   wcscat( sString2, sString );
								   }
								   else
								   {
									   swprintf( sString, L"%s", gzMercSkillTextNew[ubTempSkillArray[ubCnt]] );

									   if ( ubTempSkillArray[ubCnt] > NEWTRAIT_MERCSKILL_EXPERTOFFSET )
									   {
										   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[19].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, FONT10ARIALBOLD, &sX, &sY );

										   if ( sX <= iMinimumX )
										   {
											   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[19].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, FONT10ARIALBOLD, &sX, &sY );
											   sX = (INT16)max( sX, iMinimumX );
										   }
										   sY = (INT16)(pPersonnelScreenPoints[19].y + (ubCnt * 12));

										   SetFont( FONT10ARIALBOLD );
										   mprintf( sX, sY, sString );
										   SetFont( PERS_FONT );
									   }
									   else
									   {
										   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[19].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
										   if ( sX <= iMinimumX )
										   {
											   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[19].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
											   sX = (INT16)max( sX, iMinimumX );
										   }
										   sY = (INT16)(pPersonnelScreenPoints[19].y + (ubCnt * 12));

										   mprintf( sX, sY, sString );

									   }

									   // Add specific region for fast help window
									   if ( fAddedTraitRegion[ubCnt] )
									   {
										   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[ubCnt] );
									   }
									   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[ubCnt], (sX), (sY),
														  (sX + StringPixLength( sString, PERS_FONT )), (sY + 7), MSYS_PRIORITY_HIGH,
														  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
									   MSYS_AddRegion( &gSkillTraitHelpTextRegion[ubCnt] );
									   fAddedTraitRegion[ubCnt] = TRUE;

									   // Assign the text
									   BOOLEAN fExpert = (ubTempSkillArray[ubCnt] > NEWTRAIT_MERCSKILL_EXPERTOFFSET);

									   UINT8 traitnr = ubTempSkillArray[ubCnt];
									   if ( fExpert )
										   traitnr -= NEWTRAIT_MERCSKILL_EXPERTOFFSET;

									   CHAR16	apStr[5000];
									   swprintf( apStr, L"" );
									   AssignPersonnelSkillTraitHelpText( traitnr, fExpert, (gMercProfiles[Menptr[iId].ubProfile].ubBodyType == REGMALE), apStr );

									   // Set region help text
									   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[ubCnt]), apStr );
									   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[ubCnt], MSYS_NO_CALLBACK );
								   }
							   }

							   // if we have more skills than we can display, show "more" and create a tooltip box with the rest of them
							   if ( fDisplayMoreTraits )
							   {
								   swprintf( sString, L"%s", gzMercSkillTextNew[2 * NEWTRAIT_MERCSKILL_EXPERTOFFSET + 1] ); // display "More..."
								   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[19].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
								   if ( sX <= iMinimumX )
								   {
									   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[19].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
									   sX = (INT16)max( sX, iMinimumX );
								   }
								   sY = (INT16)(pPersonnelScreenPoints[19].y + 36);

								   mprintf( sX, sY, sString );

								   // Add specific region for fast help window
								   if ( fAddedTraitRegion[4] )
								   {
									   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[4] );
								   }
								   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[4], (sX), (sY),
													  (sX + StringPixLength( sString, PERS_FONT )), (sY + 7), MSYS_PRIORITY_HIGH,
													  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
								   MSYS_AddRegion( &gSkillTraitHelpTextRegion[4] );
								   fAddedTraitRegion[4] = TRUE;
								   // Set region help text
								   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[4]), sString2 );
								   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[4], MSYS_NO_CALLBACK );
							   }
						   }
					   }
					   else
					   {
						   INT8 bSkill1 = 0, bSkill2 = 0;
						   bSkill1 = gMercProfiles[Menptr[iId].ubProfile].bSkillTraits[0];
						   bSkill2 = gMercProfiles[Menptr[iId].ubProfile].bSkillTraits[1];

						   //if the 2 skills are the same, add the '(expert)' at the end
						   if ( bSkill1 == bSkill2 && bSkill1 != 0 )
						   {
							   swprintf( sString, L"%s %s", gzMercSkillText[bSkill1], gzMercSkillText[EXPERT] );

							   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[bScreenLocIndex].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );

							   //KM: April 16, 1999
							   //Perform the potential overrun check
							   if ( sX <= iMinimumX )
							   {
								   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[iCounter].x + (iSlot*TEXT_BOX_WIDTH) + TEXT_BOX_WIDTH - 20 + TEXT_DELTA_OFFSET), 0, 30, 0, sString, PERS_FONT, &sX, &sY );
								   sX = (INT16)max( sX, iMinimumX );
							   }

							   mprintf( sX, pPersonnelScreenPoints[bScreenLocIndex].y, sString );

							   // Add specific region for fast help window
							   if ( fAddedTraitRegion[0] )
							   {
								   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[0] );
							   }
							   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[0], (sX - 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y),
												  (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y + 7), MSYS_PRIORITY_HIGH,
												  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
							   MSYS_AddRegion( &gSkillTraitHelpTextRegion[0] );
							   fAddedTraitRegion[0] = TRUE;

							   // Assign the text
							   CHAR16	apStr[5000];
							   swprintf( apStr, L"" );
							   AssignPersonnelSkillTraitHelpText( bSkill1, TRUE, (gMercProfiles[iId].ubBodyType == REGMALE), apStr );

							   // Set region help text
							   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[0]), apStr );
							   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[0], MSYS_NO_CALLBACK );
						   }
						   else
						   {
							   //Display the first skill
							   if ( bSkill1 != 0 )
							   {
								   swprintf( sString, L"%s", gzMercSkillText[bSkill1] );

								   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[bScreenLocIndex].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );

								   //KM: April 16, 1999
								   //Perform the potential overrun check
								   sX = (INT16)max( sX, iMinimumX );
								   mprintf( sX, pPersonnelScreenPoints[bScreenLocIndex].y, sString );

								   // Add specific region for fast help window
								   if ( fAddedTraitRegion[0] )
								   {
									   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[0] );
								   }
								   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[0], (sX - 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y),
													  (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y + 7), MSYS_PRIORITY_HIGH,
													  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
								   MSYS_AddRegion( &gSkillTraitHelpTextRegion[0] );
								   fAddedTraitRegion[0] = TRUE;

								   // Assign the text
								   CHAR16	apStr[5000];
								   swprintf( apStr, L"" );
								   AssignPersonnelSkillTraitHelpText( bSkill1, FALSE, (gMercProfiles[iId].ubBodyType == REGMALE), apStr );

								   // Set region help text
								   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[0]), apStr );
								   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[0], MSYS_NO_CALLBACK );

								   ++bScreenLocIndex;
							   }

							   //Display the second skill
							   if ( bSkill2 != 0 )
							   {
								   swprintf( sString, L"%s", gzMercSkillText[bSkill2] );

								   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[bScreenLocIndex].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );

								   //KM: April 16, 1999
								   //Perform the potential overrun check
								   sX = (INT16)max( sX, iMinimumX );
								   mprintf( sX, pPersonnelScreenPoints[bScreenLocIndex].y, sString );

								   // Add specific region for fast help window
								   if ( fAddedTraitRegion[1] )
								   {
									   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[1] );
								   }
								   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[1], (sX - 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y),
													  (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y + 7), MSYS_PRIORITY_HIGH,
													  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
								   MSYS_AddRegion( &gSkillTraitHelpTextRegion[1] );
								   fAddedTraitRegion[1] = TRUE;

								   // Assign the text
								   CHAR16	apStr[5000];
								   swprintf( apStr, L"" );
								   AssignPersonnelSkillTraitHelpText( bSkill2, FALSE, (gMercProfiles[iId].ubBodyType == REGMALE), apStr );

								   // Set region help text
								   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[1]), apStr );
								   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[1], MSYS_NO_CALLBACK );

								   ++bScreenLocIndex;
							   }

							   //if no skill was displayed
							   if ( bScreenLocIndex == 19 )
							   {
								   swprintf( sString, L"%s", pPersonnelScreenStrings[PRSNL_TXT_NOSKILLS] );

								   FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[bScreenLocIndex].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH - 20, 0, sString, PERS_FONT, &sX, &sY );
								   mprintf( sX, pPersonnelScreenPoints[bScreenLocIndex].y, sString );

								   // Add specific region for fast help window
								   if ( fAddedTraitRegion[0] )
								   {
									   MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[0] );
								   }
								   MSYS_DefineRegion( &gSkillTraitHelpTextRegion[0], (sX - 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y),
													  (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[bScreenLocIndex].y + 7), MSYS_PRIORITY_HIGH,
													  MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
								   MSYS_AddRegion( &gSkillTraitHelpTextRegion[0] );
								   fAddedTraitRegion[0] = TRUE;

								   // Assign the text
								   CHAR16	apStr[5000];
								   swprintf( apStr, L"" );
								   AssignPersonnelSkillTraitHelpText( bSkill1, FALSE, (gMercProfiles[iId].ubBodyType == REGMALE), apStr );

								   // Set region help text
								   SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[0]), apStr );
								   SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[0], MSYS_NO_CALLBACK );
							   }
						   }
					   }
				   }
				   else
				   {
					   swprintf( sString, L"%s", gpStrategicString[STR_PB_NOTAPPLICABLE_ABBREVIATION] );
				   }
		}
			break;
		}
	}
}

void DisplayCharPersonality(INT32 iId, INT32 iSlot)
{
	INT32 iCounter=0;
	INT16 sX, sY;
	SOLDIERTYPE *pSoldier = &Menptr[iId];

	// SANDRO - remove the regions
	for( INT8 i = 0; i < 13; ++i )
	{
		if( fAddedTraitRegion[i] )
		{
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[i] );
			fAddedTraitRegion[i] = FALSE;
		}
	}

	if ( !pSoldier || pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE || AM_A_ROBOT( pSoldier ) || pSoldier->ubProfile == NO_PROFILE )
	{
		return;
	}

	int loc = 0;
	int region = 0;

	for ( int i = APPROACH_FRIENDLY; i <= APPROACH_RECRUIT; ++i )
	{
		mprintf( (INT16)(pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + 15), szLaptopStatText[LAPTOP_STAT_TEXT_FRIENDLY_APPROACH-1 + i] ); // APPROACH_FRIENDLY is 1 so fix the offset

		CHAR16 sStr[200];
		swprintf( sStr, L"" );
		CHAR16 sString[200];
		swprintf( sString, L"" );
		INT32 val = GetEffectiveApproachValue( pSoldier->ubProfile, i, sString );
	
		swprintf( sStr, L"%d", val );

		FindFontRightCoordinates( (INT16)(pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, sStr, PERS_FONT, &sX, &sY );
		mprintf( sX, (pPersonnelScreenPoints[loc].y + 15), sStr );

		// Add specific region for fast help window
		if ( fAddedTraitRegion[region] )
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[region] );

		MSYS_DefineRegion( &gSkillTraitHelpTextRegion[region], (sX - 3), (UINT16)(pPersonnelScreenPoints[loc].y + 15),
						   (sX + StringPixLength( sString, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[loc].y + 23), MSYS_PRIORITY_HIGH,
						   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );

		MSYS_AddRegion( &gSkillTraitHelpTextRegion[region] );
		fAddedTraitRegion[region] = TRUE;
	
		// Set region help text
		SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[region]), sString );
		SetRegionHelpEndCallback( &(gSkillTraitHelpTextRegion[region]), MSYS_NO_CALLBACK );

		++loc;
		++region;
	}

	if ( (gMercProfiles[pSoldier->ubProfile].ubMiscFlags3 & PROFILE_MISC_FLAG3_GOODGUY) )
	{
		CHAR16 sStr1[200];
		swprintf( sStr1, szLaptopStatText[LAPTOP_STAT_TEXT_GOOD_GUY] );

		CHAR16 sStr2[200];
		swprintf( sStr2, szLaptopStatText[LAPTOP_STAT_TEXT_REFUSES_TO_ATTACK_NON_HOSTILES], pSoldier->GetName( ) );

		sX = pPersonnelScreenPoints[loc].x + (iSlot*TEXT_BOX_WIDTH);

		mprintf( (INT16)(sX), (pPersonnelScreenPoints[loc].y + 15), sStr1 );

		// Add specific region for fast help window
		if ( fAddedTraitRegion[region] )
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[region] );

		MSYS_DefineRegion( &gSkillTraitHelpTextRegion[region], (sX - 3), (UINT16)(pPersonnelScreenPoints[loc].y + 15),
						   (sX + StringPixLength( sStr2, PERS_FONT ) + 3), (UINT16)(pPersonnelScreenPoints[loc].y + 23), MSYS_PRIORITY_HIGH,
						   MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );

		MSYS_AddRegion( &gSkillTraitHelpTextRegion[region] );
		fAddedTraitRegion[region] = TRUE;

		// Set region help text
		SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[region]), sStr2 );
		SetRegionHelpEndCallback( &(gSkillTraitHelpTextRegion[region]), MSYS_NO_CALLBACK );

		++loc;
		++region;
	}

	if (gGameExternalOptions.fMercGrowthModifiersEnabled)
	{
		if (gMercProfiles[pSoldier->ubProfile].fRegresses)
		{
			CHAR16 sStr2[200];
			swprintf(sStr2, szLaptopStatText[LAPTOP_STAT_TEXT_MERC_REGRESSES]);

			mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + 15), sStr2);

			++loc;
			++region;
		}
		else
		{
			const int THRESHOLD_FAST = -3; // this value and below: "fast"
			const int THRESHOLD_SLOW = 3; // this value and above: "slow"
			int yOffset = 15;
			CHAR16 statTxt[200];
			// health
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_HEALTH_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierLife <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierLife >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// strength
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_STRENGTH_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierStrength <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierStrength >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// agility
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_AGILITY_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierAgility <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierAgility >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// dexterity
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_DEXTERITY_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierDexterity <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierDexterity >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// wisdom
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_WISDOM_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierWisdom <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierWisdom >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// marksmanship
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_MARKSMANSHIP_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierMarksmanship <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierMarksmanship >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// explosives
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_EXPLOSIVES_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierExplosive <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierExplosive >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// leadership
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_LEADERSHIP_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierLeadership <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierLeadership >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// medical
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_MEDICAL_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierMedical <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierMedical >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// mechanical
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_MECHANICAL_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierMechanical <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierMechanical >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
				yOffset += 10;
			}
			// exp level
			{
				swprintf(statTxt, szLaptopStatText[LAPTOP_STAT_TEXT_EXPERIENCE_SPEED]);
				mprintf((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), (pPersonnelScreenPoints[loc].y + yOffset), statTxt);

				if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierExpLevel <= THRESHOLD_FAST)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_FAST]);
				else if (gMercProfiles[pSoldier->ubProfile].bGrowthModifierExpLevel >= THRESHOLD_SLOW)
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_SLOW]);
				else
					swprintf(statTxt, L"%s", szLaptopStatText[LAPTOP_STAT_TEXT_AVERAGE]);
				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[loc].x + (iSlot * TEXT_BOX_WIDTH)), 0, TEXT_BOX_WIDTH, 0, statTxt, PERS_FONT, &sX, &sY);
				mprintf(sX, (pPersonnelScreenPoints[loc].y + yOffset), statTxt);
			}
		}
	}
}


void SetPersonnelButtonStates( void )
{
	// this function will look at what page we are viewing, enable and disable buttons as needed

	if( ! PrevPersonnelFace( ) )
	{
		// first page, disable left buttons

//		DisableButton( 	giPersonnelButton[ 2 ] );
		DisableButton( 	giPersonnelButton[ 0 ] );
	}
	else
	{
		// enable buttons
	NextPersonnelFace( );

		// enable buttons
//		EnableButton( giPersonnelButton[ 2 ] );
		EnableButton( giPersonnelButton[ 0 ] );
	}

	if( ! NextPersonnelFace( ) )
	{

//		DisableButton( 	giPersonnelButton[ 3 ] );
		DisableButton( 	giPersonnelButton[ 1 ] );
	}
	else
	{
	// decrement page
	PrevPersonnelFace( );
			// enable buttons
//		EnableButton( giPersonnelButton[ 3 ] );
		EnableButton( giPersonnelButton[ 1 ] );
	}

	return;
}


void RenderPersonnelScreenBackground( void )
{
	HVOBJECT hHandle;
	// this fucntion will render the background for the personnel screen
	if (fCurrentTeamMode) {
		// blit title
		GetVideoObject(&hHandle, guiCURRENTTEAM);
	} else {
		// blit title
		GetVideoObject(&hHandle, guiDEPARTEDTEAM);
	}
	BltVideoObject( FRAME_BUFFER, hHandle, 0,DEPARTED_X, DEPARTED_Y, VO_BLT_SRCTRANSPARENCY,NULL );
}


void LoadPersonnelScreenBackgroundGraphics( void )
{
	// will load the graphics for the personeel screen background
	VOBJECT_DESC	VObjectDesc;

	// departed bar
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\departed.sti", VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiDEPARTEDTEAM));

	// current bar
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\CurrentTeam.sti", VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiCURRENTTEAM));
}

void DeletePersonnelScreenBackgroundGraphics( void )
{
	// delete background V/O's

	DeleteVideoObjectFromIndex( guiCURRENTTEAM );
	DeleteVideoObjectFromIndex( guiDEPARTEDTEAM );

}

void CreateDestroyButtonsForPersonnelDepartures( void )
{
	static BOOLEAN fCreated = FALSE;

	// create/ destroy personnel departures buttons as needed

	// create button?..if not created
	if( ( fCreatePeronnelDepartureButton == TRUE)&&( fCreated == FALSE ) )
	{
		fCreated = TRUE;
	}
	else if( ( fCreatePeronnelDepartureButton == FALSE ) && ( fCreated == TRUE ) )
	{
		fCreated = FALSE;
	}


	return;
}


INT32 GetNumberOfMercsOnPlayersTeam( void )
{
	SOLDIERTYPE *pTeamSoldier, *pSoldier;
	INT32 cnt=0;
	INT32 iCounter = 0;

	// grab number on team
	pSoldier = MercPtrs[0];

	// no soldiers

	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++, pTeamSoldier++)
	{
		if( ( pTeamSoldier->bActive) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && ( pTeamSoldier->stats.bLife > 0 ) )
			iCounter++;
	}

	return ( iCounter );
}


void	CreateDestroyMouseRegionsForPersonnelPortraits( void )
{
	// creates/ destroys mouse regions for portraits

	static BOOLEAN fCreated = FALSE;
	INT16 sCounter = 0;

	if( ( fCreated == FALSE )&&( fCreatePersonnelPortraitMouseRegions == TRUE ) )
	{
		// create regions
		for( sCounter = 0; sCounter < PERSONNEL_PORTRAIT_NUMBER; sCounter++ )
		{
			MSYS_DefineRegion(&gPortraitMouseRegions[ sCounter ], ( INT16 ) ( SMALL_PORTRAIT_START_X + ( sCounter % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH ), ( INT16 ) ( SMALL_PORTRAIT_START_Y +	( sCounter / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT ) , ( INT16 ) ( ( SMALL_PORTRAIT_START_X ) + ( ( sCounter % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH )	+	SMALL_PORTRAIT_WIDTH ) , ( INT16 )( SMALL_PORTRAIT_START_Y + ( sCounter / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT + SMALL_PORTRAIT_HEIGHT ),
		 MSYS_PRIORITY_HIGHEST,CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, PersonnelPortraitCallback);
			MSYS_SetRegionUserData( &gPortraitMouseRegions[ sCounter ] ,0 , sCounter );
			MSYS_AddRegion( &gPortraitMouseRegions[ sCounter ] );
		}

		fCreated = TRUE;

	}
	else if( ( fCreated == TRUE ) && ( fCreatePersonnelPortraitMouseRegions == FALSE ) )
	{
		// destroy regions
		for( sCounter = 0; sCounter < PERSONNEL_PORTRAIT_NUMBER; sCounter++ )
		{
		MSYS_RemoveRegion( &gPortraitMouseRegions[ sCounter ] );
		}

		fCreated = FALSE;
	}
	return;
}


// WDS - make number of mercenaries, etc. be configurable
// Will display the MAX_MERCS_ON_SCREEN small portraits of the current team
void DisplayPicturesOfCurrentTeam( void )
{
	if (!fCurrentTeamMode)
		return;

	// No one on team, just leave
	if (maxCurrentTeamIndex == -1)
		return;

	char sTemp[100];
	HVOBJECT hFaceHandle;
	VOBJECT_DESC	VObjectDesc;

	int countOnScreen = 0;
	for (int currentOnSreenIndex = currentTeamFirstIndex; 
		 currentOnSreenIndex <= maxCurrentTeamIndex;
		 ++currentOnSreenIndex) {
		if (++countOnScreen > MAX_MERCS_ON_SCREEN) {
			// Ran out of room...
			return;
		} // if

		SOLDIERTYPE *pSoldier = MercPtrs[currentTeamList[currentOnSreenIndex]];
		
		if ( pSoldier->ubProfile >= 0 )
		{
			if ( gMercProfiles[pSoldier->ubProfile].Type == PROFILETYPE_IMP )
			{
				sprintf( sTemp, "%s%02d.sti", IMP_SMALL_FACES_DIR, gMercProfiles[pSoldier->ubProfile].ubFaceIndex );
			}
			else
			{
				sprintf( sTemp, "%s%02d.sti", SMALL_FACES_DIR, gMercProfiles[pSoldier->ubProfile].ubFaceIndex );
			}
		}
		
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP(sTemp, VObjectDesc.ImageFile);
		CHECKV(AddVideoObject(&VObjectDesc, &guiFACE));

		//Blt face to screen to
		GetVideoObject(&hFaceHandle, guiFACE);

		if (pSoldier->stats.bLife <= 0) {
			hFaceHandle->pShades[ 0 ] = Create16BPPPaletteShaded( hFaceHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );
			//set the red pallete to the face
			SetObjectHandleShade( guiFACE, 0 );
		} // if

		BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,( INT16 ) ( SMALL_PORTRAIT_START_X+ ( (countOnScreen-1) % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH ), ( INT16 ) ( SMALL_PORTRAIT_START_Y + ( (countOnScreen-1) / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT ), VO_BLT_SRCTRANSPARENCY,NULL);

		if (pSoldier->stats.bLife <= 0)	{
			//if the merc is dead, display it
			DrawTextToScreen(AimPopUpText[AIM_MEMBER_DEAD], ( INT16 ) ( SMALL_PORTRAIT_START_X+ ( (countOnScreen-1) % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH ), ( INT16 ) ( SMALL_PORTRAIT_START_Y + ( (countOnScreen-1) / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT + SMALL_PORT_HEIGHT / 2 ), SMALL_PORTRAIT_WIDTH_NO_BORDERS, FONT10ARIAL, 145, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);
		} // if

		DeleteVideoObjectFromIndex(guiFACE);
	}
}



void PersonnelPortraitCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iPortraitId = 0;
	INT32 iOldPortraitId;

	iPortraitId = MSYS_GetRegionUserData(pRegion, 0);

	// callback handler for the minize region that is attatched to the laptop program icon
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP) {
		if (fCurrentTeamMode) {
			if (iPortraitId + currentTeamFirstIndex > maxCurrentTeamIndex) {
				return;
			}
			iOldPortraitId = currentTeamIndex - currentTeamFirstIndex;
			currentTeamIndex = iPortraitId + currentTeamFirstIndex;
			fReDrawScreenFlag = TRUE;
			// if the selected merc is valid, and they are a POW, change to the inventory display
			if( currentTeamIndex != -1 && Menptr[currentTeamList[currentTeamIndex]].bAssignment == ASSIGNMENT_POW && gubPersonnelInfoState == PERSONNEL_INV_BTN ) {
				gubPersonnelInfoState = PERSONNEL_STAT_BTN;
			}
		} else {
			if (iPortraitId >= GetNumberOfPastMercsOnPlayersTeam()) {
				return;
			}
			iOldPortraitId = iCurrentPersonSelectedId;
			iCurrentPersonSelectedId = iPortraitId;
			fReDrawScreenFlag = TRUE;
			iCurPortraitId = iPortraitId;
		}

		if( iOldPortraitId != iPortraitId ) {
			uiCurrentInventoryIndex = 0;
			guiSliderPosition = 0;
		}
	}

	if( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if (fCurrentTeamMode) {
			// valid portrait, set up id
			if (iPortraitId + currentTeamFirstIndex > maxCurrentTeamIndex) {
				// not a valid id, leave
				return;
			}

			//if the user is rigt clicking on the same face
			if (currentTeamIndex == iPortraitId + currentTeamFirstIndex) {
				//increment the info page when the user right clicks
				if( gubPersonnelInfoState < PERSONNEL_NUM_BTN-1 )
					gubPersonnelInfoState++;
				else
					gubPersonnelInfoState = PERSONNEL_STAT_BTN;
			}

			currentTeamIndex = iPortraitId + currentTeamFirstIndex;
			fReDrawScreenFlag = TRUE;

			uiCurrentInventoryIndex = 0;
			guiSliderPosition = 0;

			//if the selected merc is valid, and they are a POW, change to the inventory display
			if( currentTeamIndex != -1 && Menptr[currentTeamList[currentTeamIndex]].bAssignment == ASSIGNMENT_POW && gubPersonnelInfoState == PERSONNEL_INV_BTN) {
				gubPersonnelInfoState = PERSONNEL_STAT_BTN;
			}
		}
	}
}


void DisplayFaceOfDisplayedMerc( )
{
	// if showing inventory, leave
	if (fCurrentTeamMode) 
	{
		if (currentTeamIndex == -1) 
		{
			return;
		}
		DisplayHighLightBox();
		RenderPersonnelFace(-1, currentTeamList[currentTeamIndex], FALSE, FALSE, FALSE );
		DisplayCharName(currentTeamList[currentTeamIndex], 0 );
		if ( gubPersonnelInfoState == PERSONNEL_INV_BTN )
		{
			return;
		}

		RenderPersonnelStats(currentTeamList[currentTeamIndex], 0 );
	} 
	else 
	{
		if (iCurrentPersonSelectedId == -1) 
		{
			return;
		}

		DisplayHighLightBox();

		// Hier dürfte der Aufruf falsch sein

		RenderPersonnelFace(	GetIdOfPastMercInSlot( iCurrentPersonSelectedId ), 0, IsPastMercDead( iCurrentPersonSelectedId ), IsPastMercFired( iCurrentPersonSelectedId ), IsPastMercOther( iCurrentPersonSelectedId ) );
		DisplayDepartedCharName(	GetIdOfPastMercInSlot( iCurrentPersonSelectedId ), 0, GetTheStateOfDepartedMerc( GetIdOfPastMercInSlot( iCurrentPersonSelectedId	) ) );
		
		if ( gubPersonnelInfoState == PERSONNEL_INV_BTN )
		{
			return;
		}
		
		DisplayDepartedCharStats( GetIdOfPastMercInSlot( iCurrentPersonSelectedId ), 0, GetTheStateOfDepartedMerc( GetIdOfPastMercInSlot( iCurrentPersonSelectedId	) ) );
	}
}

void DisplayInventoryForSelectedChar( void )
{
	// display the inventory for this merc
	if ( gubPersonnelInfoState != PERSONNEL_INV_BTN ) {
		return;
	}

	CreateDestroyPersonnelInventoryScrollButtons( );

	if (fCurrentTeamMode) {
		RenderInventoryForCharacter (currentTeamList[currentTeamIndex], 0 );
	} else {
		RenderInventoryForCharacter (GetIdOfPastMercInSlot(iCurrentPersonSelectedId), 0 );
	}
}

void RenderInventoryForCharacter( INT32 iId, INT32 iSlot )
{
	UINT8 ubCounter = 0;
	SOLDIERTYPE *pSoldier;
	INT16 sIndex;
	HVOBJECT hHandle;
	ETRLEObject	*pTrav;
	INVTYPE			*pItem;
	INT16				PosX, PosY, sCenX, sCenY;
	UINT32			usHeight, usWidth;
	UINT8 ubItemCount = 0;
	UINT8 ubUpToCount = 0;
	INT16 sX, sY;
	CHAR16 sString[ 128 ];
	INT32 cnt = 0;
	INT32 iTotalAmmo = 0;

	GetVideoObject(&hHandle, guiPersonnelInventory);
	BltVideoObject(FRAME_BUFFER, hHandle, 0,( INT16 ) ( iScreenWidthOffset + 397 ), ( INT16 ) ( iScreenHeightOffset + 200 ), VO_BLT_SRCTRANSPARENCY,NULL);

	// SANDRO - remove the regions
	for( INT8 i = 0; i < 13; i++ )
	{
		if( fAddedTraitRegion[i] )
		{
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[i] );
			fAddedTraitRegion[i] = FALSE;
		}
	}

	if (!fCurrentTeamMode) {
		return;
	}

	// render the bar for the character
	RenderSliderBarForPersonnelInventory( );

	pSoldier = &Menptr[ iId ];

	//if this is a robot, dont display any inventory
	if( AM_A_ROBOT( pSoldier ) )
	{
		return;
	}

	UINT8 invsize = pSoldier->inv.size();
	for( ubCounter = 0; ubCounter < invsize; ++ubCounter )
	{
		PosX = iScreenWidthOffset + 397 + 3;
		PosY = iScreenHeightOffset + 200 + 8 +( ubItemCount * ( 29 ) );

		//if the character is a robot, only display the inv for the hand pos
		if( pSoldier->ubProfile == ROBOT && ubCounter != HANDPOS )
		{
			continue;
		}

		if( pSoldier->inv[ ubCounter ].exists() == true )
		{
			if( uiCurrentInventoryIndex > ubUpToCount )
			{
				++ubUpToCount;
			}
			else
			{
				sIndex = ( pSoldier->inv[ ubCounter ].usItem );
				pItem = &Item[ sIndex ];

				GetVideoObject( &hHandle, GetInterfaceGraphicForItem( pItem ) );
				UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;
				pTrav = &(hHandle->pETRLEObject[ usGraphicNum ] );

				usHeight				= (UINT32)pTrav->usHeight;
				usWidth					= (UINT32)pTrav->usWidth;

				sCenX = PosX + ( abs( 57 - (int)usWidth ) /	2 ) - pTrav->sOffsetX;
				sCenY = PosY + ( abs( 22 - (int)usHeight ) / 2 ) - pTrav->sOffsetY;

				// shadow
				if(gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ]) BltVideoObjectOutlineShadowFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sCenX-2, sCenY+2);

				//blt the item
				BltVideoObjectOutlineFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sCenX, sCenY, 0, FALSE );

				SetFont( FONT10ARIAL );
				SetFontForeground( FONT_WHITE );
				SetFontBackground( FONT_BLACK );
				SetFontDestBuffer( FRAME_BUFFER, 0,0,SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

				// grab item name
				LoadItemInfo(sIndex, sString, NULL );

				// shorten if needed
				if( StringPixLength( sString, FONT10ARIAL) > ( 171 - 75 ) )
				{
					ReduceStringLength( sString, ( 171 - 75 ), FONT10ARIAL );
				}

				// print name
				mprintf( PosX + 65, PosY + 3, sString );

				// condition
				if ( Item[pSoldier->inv[ ubCounter ].usItem ].usItemClass & IC_AMMO )
				{
					// Ammo
					iTotalAmmo = 0;
					for( cnt = 0; cnt < pSoldier->inv[ ubCounter ].ubNumberOfObjects; cnt++ )
					{
						// get total ammo
						iTotalAmmo += pSoldier->inv[ ubCounter ][cnt]->data.ubShotsLeft;
					}
					swprintf( sString, L"%d/%d", iTotalAmmo, ( pSoldier->inv[ ubCounter ].ubNumberOfObjects * Magazine[ Item[pSoldier->inv[ ubCounter ].usItem ].ubClassIndex ].ubMagSize ) );
					FindFontRightCoordinates( ( INT16 )( PosX + 65 ), ( INT16 ) ( PosY + 15 ), ( INT16 ) ( 171 - 75 ),
					( INT16 )( GetFontHeight( FONT10ARIAL ) ), sString, FONT10ARIAL, &sX, &sY );
				}
				else
				{
						swprintf( sString, L"%2d%%%%", pSoldier->inv[ ubCounter ][0]->data.objectStatus );
						FindFontRightCoordinates( ( INT16 )( PosX + 65 ), ( INT16 ) ( PosY + 15 ), ( INT16 ) ( 171 - 75 ),
							( INT16 )( GetFontHeight( FONT10ARIAL ) ), sString, FONT10ARIAL, &sX, &sY );

						sX += StringPixLength( sSpecialCharacters[0], FONT10ARIAL );
				}



				mprintf( sX, sY, sString );

				if ( Item[pSoldier->inv[ ubCounter ].usItem ].usItemClass & IC_GUN )
				{
					swprintf( sString, L"%s", AmmoCaliber[ Weapon[ Item[	pSoldier->inv[ ubCounter ].usItem ].ubClassIndex ].ubCalibre ]);

					// shorten if needed
					if( StringPixLength( sString, FONT10ARIAL) > ( 171 - 75 ) )
					{
						ReduceStringLength( sString, ( 171 - 75 ), FONT10ARIAL );
					}

					// print name
					mprintf( PosX + 65, PosY + 15 , sString );


				}

				// if more than 1?
				if( pSoldier->inv[ ubCounter ].ubNumberOfObjects > 1 )
				{
					swprintf( sString, L"x%d",	pSoldier->inv[ ubCounter ].ubNumberOfObjects );
					FindFontRightCoordinates( ( INT16 )( PosX ), ( INT16 ) ( PosY + 15 ), ( INT16 ) ( 58 ),
						( INT16 )( GetFontHeight( FONT10ARIAL ) ), sString, FONT10ARIAL, &sX, &sY );
					mprintf( sX, sY, sString );
				}

				// display info about it

				ubItemCount++;
			}
		}

		if( ubItemCount == NUMBER_OF_INVENTORY_PERSONNEL )
		{
			break;
		}
	}



	return;
}


void InventoryUpButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & (BUTTON_CLICKED_ON) )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);

			if( uiCurrentInventoryIndex == 0 )
			{
				return;
			}

			// up one element
			uiCurrentInventoryIndex--;
			fReDrawScreenFlag = TRUE;

			FindPositionOfPersInvSlider( );
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if( uiCurrentInventoryIndex == 0 )
		{
			return;
		}

		// up one element
		uiCurrentInventoryIndex--;
		fReDrawScreenFlag = TRUE;
		FindPositionOfPersInvSlider( );
	}
}


void InventoryDownButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if( ( INT32 )uiCurrentInventoryIndex >= ( INT32 )( GetNumberOfInventoryItemsOnCurrentMerc( ) - NUMBER_OF_INVENTORY_PERSONNEL ) )
		{
			return;
		}

		// up one element
		uiCurrentInventoryIndex++;
		fReDrawScreenFlag = TRUE;
		FindPositionOfPersInvSlider( );

	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & (BUTTON_CLICKED_ON) )
		{
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);

			if( ( INT32 )uiCurrentInventoryIndex >= ( INT32 )( GetNumberOfInventoryItemsOnCurrentMerc( ) - NUMBER_OF_INVENTORY_PERSONNEL ) )
			{
				return;
			}

			// up one element
			uiCurrentInventoryIndex++;
			fReDrawScreenFlag = TRUE;

			FindPositionOfPersInvSlider( );

		}
	}

}


// decide which buttons can and can't be accessed based on what the current item is
void EnableDisableInventoryScrollButtons( void )
{
//	if( fShowInventory == FALSE )
	if ( gubPersonnelInfoState != PERSONNEL_INV_BTN )
	{
		return;
	}

	if( uiCurrentInventoryIndex == 0 )
	{
		ButtonList[ giPersonnelInventoryButtons[ 0 ] ]->uiFlags &= ~( BUTTON_CLICKED_ON );
		DisableButton( giPersonnelInventoryButtons[ 0 ] );
	}
	else
	{
		EnableButton( giPersonnelInventoryButtons[ 0 ] );
	}


	if( ( INT32 )uiCurrentInventoryIndex >= ( INT32 )( GetNumberOfInventoryItemsOnCurrentMerc( ) - NUMBER_OF_INVENTORY_PERSONNEL ) )
	{
		ButtonList[ giPersonnelInventoryButtons[ 1 ] ]->uiFlags &= ~( BUTTON_CLICKED_ON );
		DisableButton( giPersonnelInventoryButtons[ 1 ] );
	}
	else
	{
		EnableButton( giPersonnelInventoryButtons[ 1 ] );
	}


	return;
}

INT32 GetNumberOfInventoryItemsOnCurrentMerc( void )
{
	// in current team mode?..nope...move on
	if (!fCurrentTeamMode)
		return( 0 );

	SOLDIERTYPE *pSoldier = &Menptr[currentTeamList[currentTeamIndex]];

	INT32 ubCount = 0;
	UINT8 invsize = pSoldier->inv.size();
	for (UINT8 ubCounter = 0; ubCounter < invsize; ++ubCounter)
	{
		if( ( pSoldier->inv[ ubCounter ].exists() == true) )
			++ubCount;
	}

	return ubCount;
}

void CreateDestroyPersonnelInventoryScrollButtons( void )
{
	static BOOLEAN fCreated = FALSE;

//	if( ( fShowInventory == TRUE ) && ( fCreated == FALSE ) )
	if ( (gubPersonnelInfoState == PERSONNEL_INV_BTN) && (fCreated == FALSE) )
	{
		// create buttons
		giPersonnelInventoryButtonsImages[ 0 ]=	LoadButtonImage( "LAPTOP\\personnel_inventory.sti" ,-1,1,-1,2,-1 );
	giPersonnelInventoryButtons[ 0 ] = QuickCreateButton( giPersonnelInventoryButtonsImages[0], iScreenWidthOffset + 176 + 397, iScreenHeightOffset + 2 + 200,
					 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)InventoryUpButtonCallback);

		giPersonnelInventoryButtonsImages[ 1 ]=	LoadButtonImage( "LAPTOP\\personnel_inventory.sti" ,-1,3,-1,4,-1 );
	giPersonnelInventoryButtons[ 1 ] = QuickCreateButton( giPersonnelInventoryButtonsImages[1], iScreenWidthOffset + 397 + 176, iScreenHeightOffset + 200 + 223,
					 BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)InventoryDownButtonCallback);

			// set up cursors for these buttons
		SetButtonCursor( giPersonnelInventoryButtons[ 0 ], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor( giPersonnelInventoryButtons[ 1 ], CURSOR_LAPTOP_SCREEN);

		MSYS_DefineRegion( &gMouseScrollPersonnelINV, X_OF_PERSONNEL_SCROLL_REGION, Y_OF_PERSONNEL_SCROLL_REGION, X_OF_PERSONNEL_SCROLL_REGION + X_SIZE_OF_PERSONNEL_SCROLL_REGION, Y_OF_PERSONNEL_SCROLL_REGION + Y_SIZE_OF_PERSONNEL_SCROLL_REGION,
			MSYS_PRIORITY_HIGHEST - 3, CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, HandleSliderBarClickCallback );

		fCreated = TRUE;
	}
//	else if( ( fCreated == TRUE ) && ( fShowInventory == FALSE ) )
	else if( ( fCreated == TRUE ) && ( gubPersonnelInfoState != PERSONNEL_INV_BTN ) )
	{
		// destroy buttons
		RemoveButton(giPersonnelInventoryButtons[0] );
	UnloadButtonImage(giPersonnelInventoryButtonsImages[0] );
	RemoveButton(giPersonnelInventoryButtons[1] );
	UnloadButtonImage(giPersonnelInventoryButtonsImages[1] );

		MSYS_RemoveRegion( &gMouseScrollPersonnelINV );

		fCreated = FALSE;
	}
}

void DisplayNumberOnCurrentTeam( void )
{
	// display number on team
	CHAR16 sString[ 32 ];
	INT16 sX = 0, sY = 0;

	// font stuff
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	if (fCurrentTeamMode) {
		swprintf( sString, L"%s ( %d )", pPersonelTeamStrings[ 0 ], maxCurrentTeamIndex+1 );
		sX = PERS_CURR_TEAM_X;
	} else {
		swprintf( sString, L"%s", pPersonelTeamStrings[ 0 ] );

		FindFontCenterCoordinates( PERS_CURR_TEAM_X, 0, 65, 0,sString, FONT10ARIAL, &sX, &sY );
	}

	mprintf( sX, PERS_CURR_TEAM_Y, sString );

	// now the cost of the current team, if applicable
	DisplayCostOfCurrentTeam( );
}

void DisplayNumberDeparted( void )
{
	// display number departed from team
	CHAR16 sString[ 32 ];
	INT16 sX = 0, sY = 0;


	// font stuff
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	if (!fCurrentTeamMode) {
		swprintf( sString, L"%s ( %d )", pPersonelTeamStrings[ 1 ], GetNumberOfPastMercsOnPlayersTeam( ) );
		sX = PERS_CURR_TEAM_X;
	} else {
		swprintf( sString, L"%s", pPersonelTeamStrings[ 1 ] );
		FindFontCenterCoordinates( PERS_CURR_TEAM_X, 0, 65, 0,sString, FONT10ARIAL, &sX, &sY );
	}

	mprintf( sX, PERS_DEPART_TEAM_Y, sString );
}


INT32 GetTotalDailyCostOfCurrentTeam( void )
{
	// will return the total daily cost of the current team

	SOLDIERTYPE *pSoldier;
	INT32 cnt=0;
	INT32 iCostOfTeam = 0;

	// first grunt
	pSoldier = MercPtrs[0];

	// not active?..return cost of zero

	// run through active soldiers
	for ( pSoldier = MercPtrs[ 0 ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++ )
	{
		pSoldier = MercPtrs[cnt];

		if( ( pSoldier->bActive) && ( pSoldier->stats.bLife > 0 ) )
		{
			// valid soldier, get cost
			if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC)
			{
				// daily rate
				if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
				{
					// 2 week contract
				iCostOfTeam += gMercProfiles[pSoldier->ubProfile].uiBiWeeklySalary / 14;
				}
				else if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK )
				{
					// 1 week contract
				iCostOfTeam += gMercProfiles[ pSoldier->ubProfile ].uiWeeklySalary / 7 ;
				}
				else
				{

					iCostOfTeam += gMercProfiles[ pSoldier->ubProfile ].sSalary;
				}
			}
			else if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC)
			{
				// MERC Merc
				iCostOfTeam += gMercProfiles[ pSoldier->ubProfile ].sSalary;
			}
			else
			{
					// no cost
				iCostOfTeam += 0;
			}
		}
	}
	return iCostOfTeam;
}

INT32 GetLowestDailyCostOfCurrentTeam( void )
{
	// will return the lowest daily cost of the current team

	SOLDIERTYPE *pSoldier;
	INT32 cnt=0;
	INT32 iLowest = 999999;
//	INT32 iId =0;
	INT32 iCost = 0;

	// first grunt
	pSoldier = MercPtrs[0];

	// not active?..return cost of zero

	// run through active soldiers
	for ( pSoldier = MercPtrs[ 0 ]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++ )
	{
		pSoldier = MercPtrs[cnt];

		if( ( pSoldier->bActive ) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && ( pSoldier->stats.bLife > 0 ) )
		{
			// valid soldier, get cost
			if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC)
			{
				// daily rate
				if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
				{
					// 2 week contract
				iCost = gMercProfiles[ pSoldier->ubProfile ].uiBiWeeklySalary / 14;
				}
				else if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK )
				{
					// 1 week contract
				iCost = gMercProfiles[ pSoldier->ubProfile ].uiWeeklySalary / 7 ;
				}
				else
				{

					iCost = gMercProfiles[ pSoldier->ubProfile ].sSalary;
				}
			}
			else if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC)
			{
				// MERC Merc
				iCost = gMercProfiles[ pSoldier->ubProfile ].sSalary;
			}
			else
			{
					// no cost
				iCost = 0;
			}

			if( iCost <= iLowest )
			{
				iLowest = iCost;
			}
		}

		}

	// if no mercs, send 0
	if( iLowest == 999999 )
	{
		iLowest = 0;
	}
	
	return iLowest;
}

INT32 GetHighestDailyCostOfCurrentTeam( void )
{
	// will return the lowest daily cost of the current team

	SOLDIERTYPE *pSoldier;
	INT32 cnt=0;
	INT32 iHighest = 0;
//	INT32 iId =0;
	INT32 iCost = 0;

	// first grunt
	pSoldier = MercPtrs[0];

	// not active?..return cost of zero

	// run through active soldiers
	for ( pSoldier = MercPtrs[0]; cnt <= gTacticalStatus.Team[ OUR_TEAM ].bLastID; cnt++)
	{
		pSoldier = MercPtrs[cnt];

		if( ( pSoldier->bActive) && !( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && ( pSoldier->stats.bLife > 0 ) )
		{
			// valid soldier, get cost
			if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC)
			{
				// daily rate
				if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
				{
					// 2 week contract
				iCost = gMercProfiles[ pSoldier->ubProfile ].uiBiWeeklySalary / 14;
				}
				else if( pSoldier->bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK	)
				{
					// 1 week contract
				iCost = gMercProfiles[ pSoldier->ubProfile ].uiWeeklySalary / 7 ;
				}
				else
				{

					iCost = gMercProfiles[ pSoldier->ubProfile ].sSalary;
				}
			}
			else if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC)
			{
				// MERC Merc
				iCost = gMercProfiles[ pSoldier->ubProfile ].sSalary;
			}
			else
			{
					// no cost
				iCost = 0;
			}
			
			if( iCost >= iHighest )
			{
				iHighest = iCost;
			}
		}

		}
	return iHighest;
}

void DisplayCostOfCurrentTeam( void )
{
	// display number on team
	CHAR16 sString[ 32 ];
	INT16 sX, sY;
	
	// font stuff
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	if (fCurrentTeamMode) {
		// daily cost
		mprintf(	PERS_CURR_TEAM_COST_X, PERS_CURR_TEAM_COST_Y, pPersonelTeamStrings[ 2 ] );

		swprintf( sString, L"%d", GetTotalDailyCostOfCurrentTeam( ) );
		InsertCommasForDollarFigure( sString );
		InsertDollarSignInToString( sString );

		FindFontRightCoordinates((INT16)(PERS_CURR_TEAM_COST_X),0,PERS_CURR_TEAM_WIDTH,0,sString, PERS_FONT,	&sX, &sY);

		mprintf( sX ,PERS_CURR_TEAM_COST_Y, sString );

		// highest cost
		mprintf(	PERS_CURR_TEAM_COST_X, PERS_CURR_TEAM_HIGHEST_Y, pPersonelTeamStrings[ 3 ] );

		swprintf( sString, L"%d", GetHighestDailyCostOfCurrentTeam( ) );
		InsertCommasForDollarFigure( sString );
		InsertDollarSignInToString( sString );

		FindFontRightCoordinates((INT16)(PERS_CURR_TEAM_COST_X),0,PERS_CURR_TEAM_WIDTH,0,sString, PERS_FONT,	&sX, &sY);

		mprintf( sX ,PERS_CURR_TEAM_HIGHEST_Y, sString );

		// the lowest cost
		mprintf(	PERS_CURR_TEAM_COST_X, PERS_CURR_TEAM_LOWEST_Y, pPersonelTeamStrings[ 4 ] );

		swprintf( sString, L"%d", GetLowestDailyCostOfCurrentTeam( ) );
		InsertCommasForDollarFigure( sString );
		InsertDollarSignInToString( sString );

		FindFontRightCoordinates((INT16)(PERS_CURR_TEAM_COST_X),0,PERS_CURR_TEAM_WIDTH,0,sString, PERS_FONT,	&sX, &sY);

		mprintf( sX ,PERS_CURR_TEAM_LOWEST_Y, sString );
	}
}

INT32 GetIdOfDepartedMercWithHighestStat( INT32 iStat )
{
	// will return the id value of the merc on the players team with highest in this stat
	// -1 means error
	INT32 iId = -1;
	INT32 iValue =0;
	MERCPROFILESTRUCT *pTeamSoldier;
	INT32 cnt=0;
	INT8 bCurrentList = 0;
	INT16 *bCurrentListValue = LaptopSaveInfo.ubDeadCharactersList;
	BOOLEAN fNotDone = TRUE;
	SOLDIERTYPE *pSoldier;
	UINT32 uiLoopCounter;

	// run through active soldiers
	for( uiLoopCounter=0; fNotDone ; uiLoopCounter++ )
	{
		//if we are at the end of
		if( uiLoopCounter == NUM_PROFILES && bCurrentList == 2 )
		{
			fNotDone = FALSE;
			continue;
		}

		// check if we need to move to the next list
		if( uiLoopCounter == NUM_PROFILES )
		{
			if( bCurrentList == 0 )
			{
				bCurrentList = 1;
			bCurrentListValue = LaptopSaveInfo.ubLeftCharactersList;
			}
			else if( bCurrentList == 1 )
			{
				bCurrentList = 2;
				bCurrentListValue = LaptopSaveInfo.ubOtherCharactersList;
			}

			//reset the loop counter
			uiLoopCounter = 0;
		}

		// get the id of the grunt
		cnt = *bCurrentListValue;

		// do we need to reset the count?
		if( cnt == -1 )
		{
			bCurrentListValue++;
			continue;
		}

		pTeamSoldier = &( gMercProfiles[ cnt ] );

		switch( iStat )
		{
			case 0:
			// health

				//if the soldier is a pow, dont use the health cause it aint known
			pSoldier = FindSoldierByProfileID( (UINT8)cnt, FALSE );
			if( pSoldier && pSoldier->bAssignment == ASSIGNMENT_POW )
			{
				continue;
			}

			if( pTeamSoldier->bLife >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bLife;
			}
			break;
		case 1:
		// agility
			if( pTeamSoldier->bAgility >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bAgility;
			}
			break;
			case 2:
			// dexterity
			if( pTeamSoldier->bDexterity >= iValue )
			{
					iId = cnt;
					iValue = pTeamSoldier->bDexterity;
			}
			break;
			case 3:
			// strength
		if(	pTeamSoldier->bStrength >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bStrength;
			}
			break;
			case 4:
			// leadership
		if(	pTeamSoldier->bLeadership >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bLeadership;
			}
			break;
			case 5:
		// wisdom
			if(	pTeamSoldier->bWisdom >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bWisdom;
			}
			break;
			case 6:
			// exper
		if( pTeamSoldier->bExpLevel >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bExpLevel;
			}

			break;
			case 7:
			//mrkmanship
			if(	pTeamSoldier->bMarksmanship >= iValue )
			{
					iId = cnt;
					iValue = pTeamSoldier->bMarksmanship;
			}

		break;
			case 8:
			// mech
			if(	pTeamSoldier->bMechanical >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bMechanical;
			}
		break;
			case 9:
			// exp
			if(pTeamSoldier->bExplosive >= iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bExplosive;
			}
			break;
			case 10:
			// med
			if(	pTeamSoldier->bMedical >= iValue )
			{
					iId = cnt;
					iValue = pTeamSoldier->bMedical;
			}
			break;
		}

		bCurrentListValue++;
	}
	
	return( iId );
}

INT32 GetIdOfDepartedMercWithLowestStat( INT32 iStat )
{
	// will return the id value of the merc on the players team with highest in this stat
	// -1 means error
	INT32 iId = -1;
	INT32 iValue =9999999;
	MERCPROFILESTRUCT *pTeamSoldier;
	INT32 cnt=0;
	INT8 bCurrentList = 0;
	INT16 *bCurrentListValue = LaptopSaveInfo.ubDeadCharactersList;
	BOOLEAN fNotDone = TRUE;
	SOLDIERTYPE		*pSoldier;
	UINT32 uiLoopCounter;

	// run through active soldiers
//	while( fNotDone )
	for( uiLoopCounter=0; fNotDone ; uiLoopCounter++ )
	{
/*
		// check if we are in fact not done
		if( ( bCurrentList == 2 ) && ( *bCurrentListValue == -1 ) )
		{
			fNotDone = FALSE;
			continue;
		}
*/
		//if we are at the end of
		if( uiLoopCounter == NUM_PROFILES && bCurrentList == 2 )
		{
			fNotDone = FALSE;
			continue;
		}

		// check if we need to move to the next list
//		if( *bCurrentListValue == -1 )
		if( uiLoopCounter == NUM_PROFILES )
		{
			if( bCurrentList == 0 )
			{
				bCurrentList = 1;
			bCurrentListValue = LaptopSaveInfo.ubLeftCharactersList;
			}
			else if( bCurrentList == 1 )
			{
				bCurrentList = 2;
				bCurrentListValue = LaptopSaveInfo.ubOtherCharactersList;
			}

			//reset the loop counter
			uiLoopCounter = 0;
		}

		// get the id of the grunt
		cnt = *bCurrentListValue;

		// do we need to reset the count?
		if( cnt == -1 )
		{
			bCurrentListValue++;
			continue;
		}

		pTeamSoldier = &( gMercProfiles[ cnt ] );


		switch( iStat )
		{
			case 0:
			// health

			pSoldier = FindSoldierByProfileID( (UINT8)cnt, FALSE );
			if( pSoldier && pSoldier->bAssignment == ASSIGNMENT_POW )
			{
				continue;
			}

			if( pTeamSoldier->bLife < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bLife;
			}
			break;
		case 1:
		// agility
			if( pTeamSoldier->bAgility < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bAgility;
			}
			break;
			case 2:
			// dexterity
			if( pTeamSoldier->bDexterity < iValue )
			{
					iId = cnt;
					iValue = pTeamSoldier->bDexterity;
			}
			break;
			case 3:
			// strength
		if(	pTeamSoldier->bStrength < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bStrength;
			}
			break;
			case 4:
			// leadership
		if(	pTeamSoldier->bLeadership < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bLeadership;
			}
			break;
			case 5:
		// wisdom
			if(	pTeamSoldier->bWisdom < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bWisdom;
			}
			break;
			case 6:
			// exper
		if( pTeamSoldier->bExpLevel < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bExpLevel;
			}

			break;
			case 7:
			//mrkmanship
			if(	pTeamSoldier->bMarksmanship < iValue )
			{
					iId = cnt;
					iValue = pTeamSoldier->bMarksmanship;
			}

		break;
			case 8:
			// mech
			if(	pTeamSoldier->bMechanical < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bMechanical;
			}
		break;
			case 9:
			// exp
			if(pTeamSoldier->bExplosive < iValue )
			{
				iId = cnt;
				iValue = pTeamSoldier->bExplosive;
			}
			break;
			case 10:
			// med
			if(	pTeamSoldier->bMedical < iValue )
			{
					iId = cnt;
					iValue = pTeamSoldier->bMedical;
			}
			break;
		}

		bCurrentListValue++;
	}


	return( iId );
}


INT32 GetIdOfMercWithHighestStat( INT32 iStat )
{
	// will return the id value of the merc on the players team with highest in this stat
	// -1 means error
	INT32 iId = -1;
	INT32 iValue =0;
	SOLDIERTYPE *pTeamSoldier, *pSoldier;
	INT32 cnt=0;

	// first grunt
	pSoldier = MercPtrs[0];


	// run through active soldiers
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if( ( pTeamSoldier->bActive) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && ( pTeamSoldier->stats.bLife > 0 ) && !AM_A_ROBOT( pTeamSoldier ) )
		{
			switch( iStat )
			{
				case 0:
			// health
						if( pTeamSoldier->bAssignment == ASSIGNMENT_POW )
						{
							continue;
						}

					if( pTeamSoldier->stats.bLifeMax >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bLifeMax;
					}
				break;
				case 1:
					// agility
					if( pTeamSoldier->stats.bAgility >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bAgility;
					}
					break;
				case 2:
					// dexterity
					if( pTeamSoldier->stats.bDexterity >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bDexterity;
					}
					break;
				case 3:
				// strength
				if(	pTeamSoldier->stats.bStrength >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bStrength;
					}
				break;
				case 4:
					// leadership
				if(	pTeamSoldier->stats.bLeadership >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bLeadership;
					}
				break;
				case 5:
					// wisdom
					if(	pTeamSoldier->stats.bWisdom >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bWisdom;
					}
				break;
				case 6:
					// exper
			if( pTeamSoldier->stats.bExpLevel >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bExpLevel;
					}

				break;
				case 7:
					//mrkmanship
					if(	pTeamSoldier->stats.bMarksmanship >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bMarksmanship;
					}

				break;
				case 8:
					// mech
					if(	pTeamSoldier->stats.bMechanical >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bMechanical;
					}
				break;
				case 9:
					// exp
					if(pTeamSoldier->stats.bExplosive >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bExplosive;
					}
				break;
				case 10:
					// med
					if(	pTeamSoldier->stats.bMedical >= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bMedical;
					}
				break;
				}
		}
	}


	return( iId );
}

INT32 GetIdOfMercWithLowestStat( INT32 iStat )
{
	// will return the id value of the merc on the players team with highest in this stat
	// -1 means error
	INT32 iId = -1;
	INT32 iValue =999999;
	SOLDIERTYPE *pTeamSoldier, *pSoldier;
	INT32 cnt=0;

	// first grunt
	pSoldier = MercPtrs[0];


	// run through active soldiers
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		if(( pTeamSoldier->bActive) && !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && ( pTeamSoldier->stats.bLife > 0 ) && !AM_A_ROBOT( pTeamSoldier ) )
		{

			switch( iStat )
			{
				case 0:
			// health

					if( pTeamSoldier->bAssignment == ASSIGNMENT_POW )
					{
						continue;
					}

					if( pTeamSoldier->stats.bLifeMax <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bLifeMax;
					}
				break;
				case 1:
					// agility
					if( pTeamSoldier->stats.bAgility <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bAgility;
					}
					break;
				case 2:
					// dexterity
					if(	pTeamSoldier->stats.bDexterity <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bDexterity;
					}
					break;
				case 3:
				// strength
				if(	pTeamSoldier->stats.bStrength <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bStrength;
					}
				break;
				case 4:
					// leadership
				if( pTeamSoldier->stats.bLeadership <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bLeadership;
					}
				break;
				case 5:
					// wisdom
					if( pTeamSoldier->stats.bWisdom <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bWisdom;
					}
				break;
				case 6:
					// exper
			if(	pTeamSoldier->stats.bExpLevel <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bExpLevel;
					}

				break;
				case 7:
					//mrkmanship
					if(	pTeamSoldier->stats.bMarksmanship <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bMarksmanship;
					}

				break;
				case 8:
					// mech
					if(	pTeamSoldier->stats.bMechanical <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bMechanical;
					}
				break;
				case 9:
					// exp
					if(	pTeamSoldier->stats.bExplosive <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bExplosive;
					}
				break;
				case 10:
					// med
					if(	pTeamSoldier->stats.bMedical <= iValue )
					{
						iId = cnt;
						iValue = pTeamSoldier->stats.bMedical;
					}
				break;
				}
		}
	}


	return( iId );
}


INT32 GetAvgStatOfCurrentTeamStat( INT32 iStat )
{
	// will return the id value of the merc on the players team with highest in this stat
	// -1 means error
	SOLDIERTYPE *pTeamSoldier, *pSoldier;
	INT32 cnt=0;
	INT32 iTotalStatValue = 0;
	INT8	bNumberOfPows = 0;
	UINT8	ubNumberOfMercsInCalculation = 0;


	// first grunt
	pSoldier = MercPtrs[0];

	// run through active soldiers
	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++,pTeamSoldier++)
	{
		// Only count stats of merc (not vehicles)
		if ( !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) )
		{
		if(( pTeamSoldier->bActive)&&( pTeamSoldier->stats.bLife > 0 ) && !AM_A_ROBOT( pTeamSoldier ) )
		{
			switch( iStat )
			{
				case 0:
			// health

						//if this is a pow, dont count his stats
						if( pTeamSoldier->bAssignment == ASSIGNMENT_POW )
						{
							bNumberOfPows++;
							continue;
						}

					iTotalStatValue += pTeamSoldier->stats.bLifeMax;

				break;
				case 1:
					// agility
					iTotalStatValue +=pTeamSoldier->stats.bAgility;

					break;
				case 2:
					// dexterity
					iTotalStatValue +=	pTeamSoldier->stats.bDexterity;

					break;
				case 3:
				// strength
				iTotalStatValue +=	pTeamSoldier->stats.bStrength;

				break;
				case 4:
					// leadership
					iTotalStatValue +=	pTeamSoldier->stats.bLeadership;

				break;
				case 5:
					// wisdom

					iTotalStatValue += pTeamSoldier->stats.bWisdom;
				break;
				case 6:
					// exper

					iTotalStatValue +=	pTeamSoldier->stats.bExpLevel;

				break;
				case 7:
					//mrkmanship

					iTotalStatValue +=	pTeamSoldier->stats.bMarksmanship;

				break;
				case 8:
					// mech

					iTotalStatValue +=	pTeamSoldier->stats.bMechanical;
				break;
				case 9:
					// exp

					iTotalStatValue +=	pTeamSoldier->stats.bExplosive;
				break;
				case 10:
					// med

					iTotalStatValue +=	pTeamSoldier->stats.bMedical;
				break;
			}

			ubNumberOfMercsInCalculation++;
		}
	}
	}


	//if the stat is health, and there are only pow's
	if( GetNumberOfMercsOnPlayersTeam( ) != 0 && GetNumberOfMercsOnPlayersTeam( ) == bNumberOfPows && iStat == 0 )
	{
		return( - 1 );
	}
	else if( ( ubNumberOfMercsInCalculation - bNumberOfPows ) > 0 )
	{
		return( iTotalStatValue / ( ubNumberOfMercsInCalculation - bNumberOfPows ) );
	}
	else
	{
		return( 0 );
	}
}


INT32 GetAvgStatOfPastTeamStat( INT32 iStat )
{
	// will return the id value of the merc on the players team with highest in this stat
	// -1 means error
	INT32 cnt=0;
	INT32 iTotalStatValue = 0;
	INT32 iId = -1;
	MERCPROFILESTRUCT *pTeamSoldier;
	INT8 bCurrentList = 0;
	INT16 *bCurrentListValue = LaptopSaveInfo.ubDeadCharactersList;
	BOOLEAN fNotDone = TRUE;
	UINT32 uiLoopCounter;

	// run through active soldiers

	//while( fNotDone )
	for( uiLoopCounter=0; fNotDone ; uiLoopCounter++ )
	{
/*
		// check if we are in fact not done
		if( ( bCurrentList == 2 ) && ( *bCurrentListValue == -1 ) )
		{
			fNotDone = FALSE;
			continue;
		}
*/

		//if we are at the end of
		if( uiLoopCounter == NUM_PROFILES && bCurrentList == 2 )
		{
			fNotDone = FALSE;
			continue;
		}

		// check if we need to move to the next list
//		if( *bCurrentListValue == -1 )
		if( uiLoopCounter == NUM_PROFILES )
		{
			if( bCurrentList == 0 )
			{
				bCurrentList = 1;
			bCurrentListValue = LaptopSaveInfo.ubLeftCharactersList;
			}
			else if( bCurrentList == 1 )
			{
				bCurrentList = 2;
				bCurrentListValue = LaptopSaveInfo.ubOtherCharactersList;
			}

			//reset the loop counter
			uiLoopCounter = 0;
		}

		// get the id of the grunt
		cnt = *bCurrentListValue;

		// do we need to reset the count?
		if( cnt == -1 )
		{
			bCurrentListValue++;
			continue;
		}

		pTeamSoldier = &( gMercProfiles[ cnt ] );



		switch( iStat )
		{
			case 0:
			// health

				iTotalStatValue += pTeamSoldier->bLife;

			break;
		case 1:
		// agility

				iTotalStatValue += pTeamSoldier->bAgility;

			break;
			case 2:
			// dexterity

					iTotalStatValue += pTeamSoldier->bDexterity;

			break;
			case 3:
			// strength

				iTotalStatValue += pTeamSoldier->bStrength;

			break;
			case 4:
			// leadership

				iTotalStatValue += pTeamSoldier->bLeadership;

			break;
			case 5:
		// wisdom

				iTotalStatValue += pTeamSoldier->bWisdom;

			break;
			case 6:
			// exper

				iTotalStatValue += pTeamSoldier->bExpLevel;


			break;
			case 7:
			//mrkmanship

					iId = cnt;
					iTotalStatValue += pTeamSoldier->bMarksmanship;


		break;
			case 8:
			// mech

				iTotalStatValue += pTeamSoldier->bMechanical;

		break;
			case 9:
			// exp

				iTotalStatValue += pTeamSoldier->bExplosive;

			break;
			case 10:
			// med

				iTotalStatValue += pTeamSoldier->bMedical;
			break;
		}

		bCurrentListValue++;
	}

	if( GetNumberOfPastMercsOnPlayersTeam( ) > 0 )
	{
	return( iTotalStatValue / GetNumberOfPastMercsOnPlayersTeam( ) );
	}
	else
	{
		return( 0 );
	}
}

void DisplayAverageStatValuesForCurrentTeam( void )
{
	// will display the average values for stats for the current team
	INT16 sX, sY;
	INT32 iCounter = 0;
	CHAR16 sString[ 32 ];

	// set up font
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	// display header

	// center
	FindFontCenterCoordinates( PERS_STAT_AVG_X, 0 ,PERS_STAT_AVG_WIDTH, 0 , pPersonnelCurrentTeamStatsStrings[ 1 ], FONT10ARIAL , &sX, &sY );

	mprintf( sX, PERS_STAT_AVG_Y, pPersonnelCurrentTeamStatsStrings[ 1 ] );

	// nobody on team leave
	if (fCurrentTeamMode) {
		if (maxCurrentTeamIndex == -1) {
			return;
		}
	} else  {
	    if (GetNumberOfPastMercsOnPlayersTeam() == 0) {
			return;
		}
	}

	for( iCounter = 0; iCounter < 11; iCounter++ )
	{
		// even or odd?..color black or yellow?
		if( iCounter % 2 == 0 )
		{
			SetFontForeground( PERS_TEXT_FONT_ALTERNATE_COLOR );
		}
		else
		{
			SetFontForeground( PERS_TEXT_FONT_COLOR );
		}

		if (fCurrentTeamMode)
		{
			INT32	iValue = GetAvgStatOfCurrentTeamStat( iCounter );

			//if there are no values
			if( iValue == -1 )
				swprintf( sString, L"%s", pPOWStrings[ 1 ] );
			else
				swprintf( sString, L"%d", iValue );

		}
		else
		{
			swprintf( sString, L"%d", GetAvgStatOfPastTeamStat( iCounter ) );
		}
		// center
		FindFontCenterCoordinates( PERS_STAT_AVG_X, 0 ,PERS_STAT_AVG_WIDTH, 0 , sString, FONT10ARIAL , &sX, &sY );
		mprintf( sX, PERS_STAT_AVG_Y + ( iCounter + 1 ) * ( GetFontHeight( FONT10ARIAL ) + 3 ), sString );
	}
}

void DisplayLowestStatValuesForCurrentTeam( void )
{
	// will display the average values for stats for the current team
	INT16 sX, sY;
	INT32 iCounter = 0;
	CHAR16 sString[ 32 ];
	INT32 iStat = 0;
	INT32	iDepartedId=0;
	INT32	iId = 0;

	// set up font
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	// display header

	// center
	FindFontCenterCoordinates( PERS_STAT_LOWEST_X, 0 ,PERS_STAT_LOWEST_WIDTH, 0 , pPersonnelCurrentTeamStatsStrings[ 0 ], FONT10ARIAL , &sX, &sY );

	mprintf( sX, PERS_STAT_AVG_Y, pPersonnelCurrentTeamStatsStrings[ 0 ] );

	// nobody on team leave
	if (fCurrentTeamMode) {
		if (maxCurrentTeamIndex == -1) {
			return;
		}
	} else  {
	    if (GetNumberOfPastMercsOnPlayersTeam() == 0) {
			return;
		}
	}

	for( iCounter = 0; iCounter < 11; iCounter++ )
	{
		if (fCurrentTeamMode) {
			iId = GetIdOfMercWithLowestStat( iCounter );
			if( iId == -1 )
				continue;
		} else {
			iDepartedId = GetIdOfDepartedMercWithLowestStat( iCounter );
			if( iDepartedId == -1 )
				continue;
		}

		// even or odd?..color black or yellow?
		if( iCounter % 2 == 0 ) {
			SetFontForeground( PERS_TEXT_FONT_ALTERNATE_COLOR );
		} else {
			SetFontForeground( PERS_TEXT_FONT_COLOR );
		}

		if (fCurrentTeamMode) {
			// get name
			/*if( iId == -1 )
				swprintf( sString, L"%s", pPOWStrings[1] );
			else*/
				swprintf( sString, L"%s", MercPtrs[ iId ]->GetName() );
		} else {
			// get name
			swprintf( sString, L"%s", gMercProfiles[ iDepartedId ].zNickname );
		}
		// print name
		mprintf( PERS_STAT_LOWEST_X, PERS_STAT_AVG_Y + ( iCounter + 1 ) * ( GetFontHeight( FONT10ARIAL ) + 3 ), sString );

		switch (iCounter) {
			case 0:
				// health
				if (fCurrentTeamMode) {
					/*if( iId == -1 )
						iStat = -1;
					else*/
						iStat = MercPtrs[ iId ]->stats.bLifeMax;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bLife;
				}
				break;
			case 1:
				// agility
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bAgility;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bAgility;
				}
				break;
			case 2:
				// dexterity
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bDexterity;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bDexterity;
				}
				break;
			case 3:
				// strength
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bStrength;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bStrength;
				}
				break;
			case 4:
				// leadership
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bLeadership;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bLeadership;
				}
				break;
			case 5:
				// wisdom
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bWisdom;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bWisdom;
				}
				break;
			case 6:
				// exper
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bExpLevel;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bExpLevel;
				}
				break;
			case 7:
				//mrkmanship
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bMarksmanship;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bMarksmanship;
				}
				break;
			case 8:
				// mech
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bMechanical;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bMechanical;
				}
				break;
			case 9:
				// exp
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bExplosive;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bExplosive;
				}
				break;
			case 10:
				// med
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bMedical;
				} else {
					iStat =	gMercProfiles[ iDepartedId ] . bMedical;
				}
				break;
			}

			/*if( iStat == -1 )
				swprintf( sString, L"%s", pPOWStrings[1] );
			else*/
				swprintf( sString, L"%d", iStat );

		// right justify
		FindFontRightCoordinates(	PERS_STAT_LOWEST_X, 0 ,PERS_STAT_LOWEST_WIDTH, 0 , sString, FONT10ARIAL , &sX, &sY );
		mprintf( sX, PERS_STAT_AVG_Y + ( iCounter + 1 ) * ( GetFontHeight( FONT10ARIAL ) + 3 ), sString );
	}
}


void DisplayHighestStatValuesForCurrentTeam( void )
{
	// will display the average values for stats for the current team
	INT16 sX, sY;
	INT32 iCounter = 0;
	CHAR16 sString[ 32 ];
	INT32 iStat = 0;
	INT32 iId=0;

	// set up font
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	// display header

	// center
	FindFontCenterCoordinates( PERS_STAT_HIGHEST_X, 0 ,PERS_STAT_LOWEST_WIDTH, 0 , pPersonnelCurrentTeamStatsStrings[ 2 ], FONT10ARIAL , &sX, &sY );

	mprintf( sX, PERS_STAT_AVG_Y, pPersonnelCurrentTeamStatsStrings[ 2 ] );

	// nobody on team leave
	if (fCurrentTeamMode) {
		if (maxCurrentTeamIndex == -1) {
			return;
		}
	} else  {
	    if (GetNumberOfPastMercsOnPlayersTeam() == 0) {
			return;
		}
	}

	for( iCounter = 0; iCounter < 11; iCounter++ )
	{
		if (fCurrentTeamMode)
			iId = GetIdOfMercWithHighestStat( iCounter );
		else
			iId = GetIdOfDepartedMercWithHighestStat( iCounter );

		if( iId == -1 )
			continue;

		// even or odd?..color black or yellow?
		if( iCounter % 2 == 0 )
		{
			SetFontForeground( PERS_TEXT_FONT_ALTERNATE_COLOR );
		}
		else
		{
			SetFontForeground( PERS_TEXT_FONT_COLOR );
		}

		if (fCurrentTeamMode)
		{
			// get name
			/*if( iId == -1 )
			{
				swprintf( sString, L"%s", pPOWStrings[1] );
			}
			else*/
				swprintf( sString, L"%s", MercPtrs[ iId ]->GetName() );
		}
		else
		{
			// get name
			swprintf( sString, L"%s", gMercProfiles[ iId ].zNickname );
		}
		// print name
		mprintf( PERS_STAT_HIGHEST_X, PERS_STAT_AVG_Y + ( iCounter + 1 ) * ( GetFontHeight( FONT10ARIAL ) + 3 ), sString );

		switch (iCounter) {
			case 0:
				// health
				if (fCurrentTeamMode) {
					/*if( iId == -1 )
						iStat = -1;
					else*/
						iStat = MercPtrs[ iId ]->stats.bLifeMax;
				} else {
					iStat =	gMercProfiles[ iId ] . bLife;
				}
				break;
			case 1:
				// agility
				if (fCurrentTeamMode) {
						iStat = MercPtrs[ iId ]->stats.bAgility;
				} else {
					iStat =	gMercProfiles[ iId ] . bAgility;
				}
				break;
			case 2:
				// dexterity
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bDexterity;
				} else {
					iStat =	gMercProfiles[ iId ] . bDexterity;
				}
				break;
			case 3:
				// strength
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bStrength;
				} else {
					iStat =	gMercProfiles[ iId ] . bStrength;
				}
				break;
			case 4:
				// leadership
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bLeadership;
				} else {
					iStat =	gMercProfiles[ iId ] . bLeadership;
				}
				break;
			case 5:
				// wisdom
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bWisdom;
				} else {
					iStat =	gMercProfiles[ iId ] . bWisdom;
				}
				break;
			case 6:
				// exper
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bExpLevel;
				} else {
					iStat =	gMercProfiles[ iId ] . bExpLevel;
				}
				break;
			case 7:
				//mrkmanship
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bMarksmanship;
				} else {
					iStat =	gMercProfiles[ iId ] . bMarksmanship;
				}
				break;
			case 8:
				// mech
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bMechanical;
				} else {
					iStat =	gMercProfiles[ iId ] . bMechanical;
				}
				break;
			case 9:
				// exp
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bExplosive;
				} else {
					iStat =	gMercProfiles[ iId ] . bExplosive;
				}
				break;
			case 10:
				// med
				if (fCurrentTeamMode) {
					iStat = MercPtrs[ iId ]->stats.bMedical;
				} else {
					iStat =	gMercProfiles[ iId ] . bMedical;
				}
				break;
			}

			/*if( iStat == -1 )
				swprintf( sString, L"%s", pPOWStrings[1] );
			else*/
				swprintf( sString, L"%d", iStat );

		// right justify
		FindFontRightCoordinates(	PERS_STAT_HIGHEST_X, 0 ,PERS_STAT_LOWEST_WIDTH, 0 , sString, FONT10ARIAL , &sX, &sY );
		mprintf( sX, PERS_STAT_AVG_Y + ( iCounter + 1 ) * ( GetFontHeight( FONT10ARIAL ) + 3 ), sString );
	}
}



void DisplayPersonnelTeamStats( void )
{
	// displays the stat title for each row in the team stat list
	INT32 iCounter =0;


	// set up font
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( FONT_WHITE );


	// display titles for each row
	for( iCounter = 0; iCounter < 11; iCounter++ )
	{
			// even or odd?..color black or yellow?
		if( iCounter % 2 == 0 )
		{
			SetFontForeground( PERS_TEXT_FONT_ALTERNATE_COLOR );
		}
		else
		{
			SetFontForeground( PERS_TEXT_FONT_COLOR );
		}

		mprintf( PERS_STAT_LIST_X, PERS_STAT_AVG_Y + ( iCounter + 1 ) * ( GetFontHeight( FONT10ARIAL ) + 3 ), pPersonnelTeamStatsStrings[ iCounter ] );
	}


	return;
}


INT32 GetNumberOfPastMercsOnPlayersTeam( void )
{
	INT32 iPastNumberOfMercs = 0;
	// will run through the alist of past mercs on the players team and return thier number

	// dead
	iPastNumberOfMercs += GetNumberOfDeadOnPastTeam( );

	// left
	iPastNumberOfMercs += GetNumberOfLeftOnPastTeam( );

	// other
	iPastNumberOfMercs += GetNumberOfOtherOnPastTeam( );


	return iPastNumberOfMercs;
}


void InitPastCharactersList( void )
{
	// inits the past characters list
	memset( &LaptopSaveInfo.ubDeadCharactersList, -1, sizeof( LaptopSaveInfo.ubDeadCharactersList ) );
	memset( &LaptopSaveInfo.ubLeftCharactersList, -1, sizeof( LaptopSaveInfo.ubLeftCharactersList ) );
	memset( &LaptopSaveInfo.ubOtherCharactersList, -1, sizeof( LaptopSaveInfo.ubOtherCharactersList ) );

	return;
}


INT32 GetNumberOfDeadOnPastTeam( void )
{

	INT32 iNumberDead = 0;
	INT32 iCounter = 0;

//	for( iCounter = 0; ( ( iCounter < 256) && ( LaptopSaveInfo.ubDeadCharactersList[ iCounter ] != -1 ) ) ; iCounter ++ )
	for( iCounter = 0; iCounter < 256 ; iCounter ++ )
	{
		if( LaptopSaveInfo.ubDeadCharactersList[ iCounter ] != -1 )
			iNumberDead++;
	}


	return( iNumberDead );

}


INT32 GetNumberOfLeftOnPastTeam( void )
{

	INT32 iNumberLeft = 0;
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < 256 ; iCounter ++ )
	{
		if( LaptopSaveInfo.ubLeftCharactersList[ iCounter ] != -1 )
			iNumberLeft++;
	}


	return( iNumberLeft );

}


INT32 GetNumberOfOtherOnPastTeam( void )
{

	INT32 iNumberOther = 0;
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < 256 ; iCounter ++ )
	{
		if( LaptopSaveInfo.ubOtherCharactersList[ iCounter ] != -1 )
			iNumberOther++;
	}


	return( iNumberOther );

}


void DisplayStateOfPastTeamMembers( void )
{
	INT16 sX, sY;
	CHAR16 sString[ 32 ];

	// font stuff
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );


	// display numbers fired, dead and othered
	if (!fCurrentTeamMode) {
		// dead
		mprintf(	PERS_CURR_TEAM_COST_X, PERS_CURR_TEAM_COST_Y, pPersonelTeamStrings[ 5 ] );
		swprintf( sString, L"%d", GetNumberOfDeadOnPastTeam( ) );

		FindFontRightCoordinates((INT16)(PERS_CURR_TEAM_COST_X),0,PERS_DEPART_TEAM_WIDTH,0,sString, PERS_FONT,	&sX, &sY);

		mprintf( sX ,PERS_CURR_TEAM_COST_Y, sString );

		// fired
		mprintf(	PERS_CURR_TEAM_COST_X, PERS_CURR_TEAM_HIGHEST_Y, pPersonelTeamStrings[ 6 ] );
		swprintf( sString, L"%d", GetNumberOfLeftOnPastTeam( ) );

		FindFontRightCoordinates((INT16)(PERS_CURR_TEAM_COST_X),0,PERS_DEPART_TEAM_WIDTH,0,sString, PERS_FONT,	&sX, &sY);

		mprintf( sX ,PERS_CURR_TEAM_HIGHEST_Y, sString );

		// other
		mprintf(	PERS_CURR_TEAM_COST_X, PERS_CURR_TEAM_LOWEST_Y, pPersonelTeamStrings[ 7 ] );
		swprintf( sString, L"%d", GetNumberOfOtherOnPastTeam( ) );

		FindFontRightCoordinates((INT16)(PERS_CURR_TEAM_COST_X),0,PERS_DEPART_TEAM_WIDTH,0,sString, PERS_FONT,	&sX, &sY);

		mprintf( sX ,PERS_CURR_TEAM_LOWEST_Y, sString );
	}
}



void CreateDestroyCurrentDepartedMouseRegions( void )
{

	static BOOLEAN fCreated = FALSE;

	// will arbitrate the creation/deletion of mouse regions for current/past team toggles


	if( ( fCreateRegionsForPastCurrentToggle == TRUE ) && ( fCreated == FALSE ) )
	{
		// not created, create
		MSYS_DefineRegion(&gTogglePastCurrentTeam[ 0 ], PERS_TOGGLE_CUR_DEPART_X, PERS_TOGGLE_CUR_Y, PERS_TOGGLE_CUR_DEPART_X + PERS_TOGGLE_CUR_DEPART_WIDTH, PERS_TOGGLE_CUR_Y + PERS_TOGGLE_CUR_DEPART_HEIGHT,
		 MSYS_PRIORITY_HIGHEST - 3 ,CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, PersonnelCurrentTeamCallback);

		MSYS_AddRegion( &gTogglePastCurrentTeam[ 0 ] );

		MSYS_DefineRegion(&gTogglePastCurrentTeam[ 1 ], PERS_TOGGLE_CUR_DEPART_X, PERS_TOGGLE_DEPART_Y , PERS_TOGGLE_CUR_DEPART_X + PERS_TOGGLE_CUR_DEPART_WIDTH, PERS_TOGGLE_DEPART_Y + PERS_TOGGLE_CUR_DEPART_HEIGHT,
		 MSYS_PRIORITY_HIGHEST - 3,CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, PersonnelDepartedTeamCallback);

		MSYS_AddRegion( &gTogglePastCurrentTeam[ 1 ] );

		fCreated = TRUE;

	}
	else if( ( fCreateRegionsForPastCurrentToggle == FALSE ) && ( fCreated == TRUE ) )
	{
		// created, get rid of

		MSYS_RemoveRegion( &gTogglePastCurrentTeam[ 0 ] );
		MSYS_RemoveRegion( &gTogglePastCurrentTeam[ 1 ] );
		fCreated = FALSE;
	}

	return;
}



void PersonnelCurrentTeamCallback( MOUSE_REGION * pRegion, INT32 iReason ) {
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP) {
		fCurrentTeamMode = TRUE;
		// how many people do we have?..if you have someone set default to 0
		if( maxCurrentTeamIndex >= 0 ) {
			// get id of first merc in list
			currentTeamIndex = 0;
		} else {
			currentTeamIndex = -1;
		} // else
		fReDrawScreenFlag = TRUE;
	} // if
}


void PersonnelDepartedTeamCallback( MOUSE_REGION * pRegion, INT32 iReason ) {
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP) {
		fCurrentTeamMode = FALSE;
		// how many departed people?
		if (GetNumberOfPastMercsOnPlayersTeam() > 0) {
			iCurrentPersonSelectedId = 0;
		} else {
			iCurrentPersonSelectedId = -1;
		}
		//Switch the panel on the right to be the stat panel
		gubPersonnelInfoState = PERSONNEL_STAT_BTN;
		fReDrawScreenFlag = TRUE;
	}
}



void CreateDestroyButtonsForDepartedTeamList( void )
{
	// creates/ destroys the buttons for cdeparted team list
	static BOOLEAN fCreated = FALSE;

	if ((!fCurrentTeamMode) && (!fCreated)) {
		// not created. create
		giPersonnelButtonImage[ 4 ]=	LoadButtonImage( "LAPTOP\\departuresbuttons.sti" ,-1,0,-1,2,-1 );
		giPersonnelButton[ 4 ] = QuickCreateButton( giPersonnelButtonImage[4], PERS_DEPARTED_UP_X, PERS_DEPARTED_UP_Y,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)DepartedUpCallBack);

		// right button
		giPersonnelButtonImage[ 5 ]=	LoadButtonImage( "LAPTOP\\departuresbuttons.sti" ,-1,1,-1,3,-1 );
		giPersonnelButton[ 5 ] = QuickCreateButton( giPersonnelButtonImage[5], PERS_DEPARTED_UP_X, PERS_DEPARTED_DOWN_Y,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)DepartedDownCallBack);

		// set up cursors for these buttons
		SetButtonCursor( giPersonnelButton[ 4 ], CURSOR_LAPTOP_SCREEN);
		SetButtonCursor( giPersonnelButton[ 5 ], CURSOR_LAPTOP_SCREEN);

		fCreated = TRUE;
	}
	else if ((fCurrentTeamMode) && (fCreated)) {
		// created. destroy
		RemoveButton(giPersonnelButton[4] );
		UnloadButtonImage(giPersonnelButtonImage[4] );
		RemoveButton(giPersonnelButton[5] );
		UnloadButtonImage(giPersonnelButtonImage[5] );
		fCreated = FALSE;
		fReDrawScreenFlag = TRUE;
	}
}


void DepartedUpCallBack(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{

		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON) {
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			if(	giCurrentUpperLeftPortraitNumber - MAX_MERCS_ON_SCREEN >= 0 ) {
				giCurrentUpperLeftPortraitNumber -= MAX_MERCS_ON_SCREEN;
				fReDrawScreenFlag = TRUE;
			}
		}
	}
}


void DepartedDownCallBack(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{

		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			if( ( giCurrentUpperLeftPortraitNumber + MAX_MERCS_ON_SCREEN ) < ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) )
			{
				giCurrentUpperLeftPortraitNumber+=MAX_MERCS_ON_SCREEN;
				fReDrawScreenFlag = TRUE;
			}
		}
	}
}


// display past mercs portraits, starting at giCurrentUpperLeftPortraitNumber and going up MAX_MERCS_ON_SCREEN mercs
// start at dead mercs, then fired, then other
void DisplayPastMercsPortraits( void )
{
	// not time to display
	if (fCurrentTeamMode) {
		return;
	}

	INT32 iCounter = 0;
	INT32 iStartArray = 0; // 0 = dead list, 1 = fired list, 2 = other list

	// go through dead list
	for( INT32 iCounterA = 0; ( iCounter < giCurrentUpperLeftPortraitNumber ); iCounterA++ ) {
		if( LaptopSaveInfo.ubDeadCharactersList[ iCounterA ] != -1 )
			iCounter++;
	}

	if( iCounter < giCurrentUpperLeftPortraitNumber) {
		// now the fired list
		for( INT32 iCounterA = 0; ( ( iCounter < giCurrentUpperLeftPortraitNumber ) ); iCounterA++ ) {
			if( LaptopSaveInfo.ubLeftCharactersList[ iCounterA ] != -1 ) {
				iCounter++;
			}
		}

		if( iCounter < MAX_MERCS_ON_SCREEN ) {
			iStartArray = 0;
		} else {
			iStartArray = 1;
		}
	} else {
		iStartArray = 0;
	}

	if( ( iCounter < giCurrentUpperLeftPortraitNumber ) && ( iStartArray != 0 ) ) {
		// now the fired list
		for( INT32 iCounterA = 0; ( iCounter < giCurrentUpperLeftPortraitNumber ); iCounterA++ ) {
			if( LaptopSaveInfo.ubOtherCharactersList[ iCounterA ] != -1 )
				iCounter++;
		}

		if( iCounter < MAX_MERCS_ON_SCREEN ) {
			iStartArray = 1;
		} else {
			iStartArray = 2;
		}
	} else if( iStartArray != 0 ) {
		iStartArray = 1;
	}

	//; we now have the array to start in, the position

	iCounter = 0;
	INT32 iCounterA = 0;

	if( iStartArray == 0 ) {
	// run through list and display
		for( iCounterA ; iCounter < MAX_MERCS_ON_SCREEN && iCounterA < 256; iCounterA++ ) {
			// show dead pictures
			if( LaptopSaveInfo.ubDeadCharactersList[ iCounterA ] != -1 ) {
				DisplayPortraitOfPastMerc( LaptopSaveInfo.ubDeadCharactersList[ iCounterA ], iCounter, TRUE, FALSE, FALSE );
				iCounter++;
			}
		}

		// reset counter A for the next array, if applicable
		iCounterA = 0;
	}
	if( iStartArray <= 1 ) {
		for( iCounterA ; ( iCounter < MAX_MERCS_ON_SCREEN	&& iCounterA < 256 ); iCounterA++) {
			// show fired pics
			if( LaptopSaveInfo.ubLeftCharactersList[ iCounterA ] != -1 ) {
				DisplayPortraitOfPastMerc( LaptopSaveInfo.ubLeftCharactersList[ iCounterA ], iCounter, FALSE, TRUE, FALSE );
				iCounter++;
			}
		}
		// reset counter A for the next array, if applicable
		iCounterA = 0;
	}

	for( iCounterA ; ( iCounter < MAX_MERCS_ON_SCREEN	&& iCounterA < 256 ); iCounterA++ ) {
		// show other pics
		if( LaptopSaveInfo.ubOtherCharactersList[ iCounterA ] != -1 ) {
			DisplayPortraitOfPastMerc( LaptopSaveInfo.ubOtherCharactersList[ iCounterA ], iCounter, FALSE, FALSE, TRUE );
			iCounter++;
		}
	}
}


// returns ID of Merc in this slot
INT32 GetIdOfPastMercInSlot( INT32 iSlot )
{
	// not time to display
	if (fCurrentTeamMode) {
		return -1;
	}

	INT32 iCounter =-1;
	INT32 iCounterA =0;
	if( iSlot > ( ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) - giCurrentUpperLeftPortraitNumber ) )
	{
		// invalid slot
		return iCurrentPersonSelectedId;
	}
	// go through dead list
	for( iCounterA = 0; ( ( iCounter ) < iSlot + giCurrentUpperLeftPortraitNumber);	iCounterA++ )
	{
		if( LaptopSaveInfo.ubDeadCharactersList[ iCounterA ] != -1 )
			iCounter++;
	}

	if( iSlot + giCurrentUpperLeftPortraitNumber == iCounter)
	{
		return	( LaptopSaveInfo.ubDeadCharactersList[ iCounterA - 1] );
	}

	// now the fired list
	iCounterA =0;
	for( iCounterA = 0; ( ( ( iCounter	)< iSlot + giCurrentUpperLeftPortraitNumber) ); iCounterA++ )
	{
		if( LaptopSaveInfo.ubLeftCharactersList[ iCounterA ] != -1 )
			iCounter++;
	}

	if( iSlot + giCurrentUpperLeftPortraitNumber == iCounter)
	{
		return	( LaptopSaveInfo.ubLeftCharactersList[ iCounterA	- 1 ] );
	}


	// now the fired list
	iCounterA =0;
	for( iCounterA = 0; ( ( ( iCounter ) < ( iSlot + giCurrentUpperLeftPortraitNumber ) ) ); iCounterA++ )
	{
		if( LaptopSaveInfo.ubOtherCharactersList[ iCounterA ] != -1 )
			++iCounter;
	}

	return( LaptopSaveInfo.ubOtherCharactersList[ iCounterA	- 1] );
}


void DisplayPortraitOfPastMerc( INT32 iId , INT32 iCounter, BOOLEAN fDead, BOOLEAN fFired, BOOLEAN fOther )
{
	char sTemp[100];
	HVOBJECT hFaceHandle;
	VOBJECT_DESC	VObjectDesc;
	
	if ( gMercProfiles[iId].Type == PROFILETYPE_IMP )
	{
		sprintf( sTemp, "%s%02d.sti", IMP_SMALL_FACES_DIR, gMercProfiles[iId].ubFaceIndex );
	}
	else
	{
		sprintf( sTemp, "%s%02d.sti", SMALL_FACES_DIR, gMercProfiles[iId].ubFaceIndex );
	}
	
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP(sTemp, VObjectDesc.ImageFile);
	CHECKV(AddVideoObject(&VObjectDesc, &guiFACE));

	//Blt face to screen to
	GetVideoObject(&hFaceHandle, guiFACE);

	if( fDead ) {
		hFaceHandle->pShades[ 0 ]		= Create16BPPPaletteShaded( hFaceHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );
		//set the red pallete to the face
		SetObjectHandleShade( guiFACE, 0 );
	}

	BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,( INT16 ) ( SMALL_PORTRAIT_START_X+ ( iCounter % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH ), ( INT16 ) ( SMALL_PORTRAIT_START_Y + ( iCounter / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT ), VO_BLT_SRCTRANSPARENCY,NULL);

	DeleteVideoObjectFromIndex(guiFACE);
}




void DisplayDepartedCharStats(INT32 iId, INT32 iSlot, INT32 iState)
{
	INT32 iCounter=0;
	CHAR16 sString[50];
	INT16 sX, sY;
	UINT32 uiHits = 0;
	HVOBJECT hHandle;

		// font stuff
	SetFont( FONT10ARIAL );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( PERS_TEXT_FONT_COLOR );

	// SANDRO - remove the regions
	for( INT8 i = 0; i < 13; i++ )
	{
		if( fAddedTraitRegion[i] )
		{
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[i] );
			fAddedTraitRegion[i] = FALSE;
		}
	}

	// display the stats for a char
	for(iCounter=0;iCounter <MAX_STATS; iCounter++)
	{
		switch(iCounter)
		{
		case 0:
			// health

			// dead?
			if( iState == 0 )
			{
			swprintf(sString, L"%d/%d",0,gMercProfiles[iId].bLife);
			}
			else
			{
				swprintf(sString, L"%d/%d",gMercProfiles[iId].bLife,gMercProfiles[iId].bLife);
			}

			mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
			break;
		case 1:
			// agility
		swprintf(sString, L"%d",gMercProfiles[iId].bAgility);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
			break;
		case 2:
		// dexterity
		swprintf(sString, L"%d",gMercProfiles[iId].bDexterity);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
			break;
		case 3:
		// strength
		swprintf(sString, L"%d",gMercProfiles[iId].bStrength);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
		case 4:
		// leadership
		swprintf(sString, L"%d",gMercProfiles[iId].bLeadership);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
		case 5:
		// wisdom
		swprintf(sString, L"%d",gMercProfiles[iId].bWisdom);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
		case 6:
		// exper
		swprintf(sString, L"%d",gMercProfiles[iId].bExpLevel);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
		case 7:
			//mrkmanship
		swprintf(sString, L"%d",gMercProfiles[iId].bMarksmanship);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
	 case 8:
		// mech
		swprintf(sString, L"%d",gMercProfiles[iId].bMechanical);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
	 case 9:
		// exp
		swprintf(sString, L"%d",gMercProfiles[iId].bExplosive);
		mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;
		case 10:
		// med
			mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[iCounter]);

		swprintf(sString, L"%d",gMercProfiles[iId].bMedical);


			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;


		case 14:
		// kills
			mprintf((INT16)(pPersonnelScreenPoints[20].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[20].y - 12),pPersonnelScreenStrings[PRSNL_TXT_KILLS]);

			swprintf(sString, L"%d",(gMercProfiles[iId].records.usKillsElites + gMercProfiles[iId].records.usKillsRegulars + gMercProfiles[iId].records.usKillsAdmins + gMercProfiles[iId].records.usKillsHostiles + gMercProfiles[iId].records.usKillsCreatures + gMercProfiles[iId].records.usKillsZombies + gMercProfiles[iId].records.usKillsTanks + gMercProfiles[iId].records.usKillsOthers));

			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[20].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[20].y - 12),sString);

			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[20].x + 148), ( pPersonnelScreenPoints[20].y - 13 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[7] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[7] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[7], (UINT16)( pPersonnelScreenPoints[20].x + 147 ), (UINT16)( pPersonnelScreenPoints[20].y - 14 ),
							(UINT16)( pPersonnelScreenPoints[20].x + 166 ), (UINT16)(pPersonnelScreenPoints[20].y - 3), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[7] );
			fAddedTraitRegion[7] = TRUE;
			// Assign the text
			AssignPersonnelKillsHelpText( iId );

		break;
		case 15:
			// assists
			mprintf((INT16)(pPersonnelScreenPoints[21].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[21].y - 10),pPersonnelScreenStrings[PRSNL_TXT_ASSISTS]);
			swprintf(sString, L"%d",(gMercProfiles[iId].records.usAssistsMercs + gMercProfiles[iId].records.usAssistsMilitia + gMercProfiles[iId].records.usAssistsOthers));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[21].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[21].y - 10),sString);
		
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[21].x + 148), ( pPersonnelScreenPoints[21].y - 11 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[8] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[8] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[8], (UINT16)( pPersonnelScreenPoints[21].x + 147 ), (UINT16)( pPersonnelScreenPoints[21].y - 12 ),
							(UINT16)( pPersonnelScreenPoints[21].x + 166 ), (UINT16)(pPersonnelScreenPoints[21].y - 1), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[8] );
			fAddedTraitRegion[8] = TRUE;
			// Assign the text
			AssignPersonnelAssistsHelpText( iId );

		break;
		case 16:
			// shots/hits
			mprintf((INT16)(pPersonnelScreenPoints[22].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[22].y - 8),pPersonnelScreenStrings[PRSNL_TXT_HIT_PERCENTAGE]);
			uiHits = ( UINT32 )gMercProfiles[iId].records.usShotsHit;
			uiHits *= 100;

			// check we have shot at least once
			if( (gMercProfiles[iId].records.usShotsFired + gMercProfiles[iId].records.usMissilesLaunched + gMercProfiles[iId].records.usGrenadesThrown + gMercProfiles[iId].records.usKnivesThrown + gMercProfiles[iId].records.usBladeAttacks + gMercProfiles[iId].records.usHtHAttacks) > 0 )
			{
				uiHits /= ( UINT32 )(gMercProfiles[iId].records.usShotsFired + gMercProfiles[iId].records.usMissilesLaunched + gMercProfiles[iId].records.usGrenadesThrown + gMercProfiles[iId].records.usKnivesThrown + gMercProfiles[iId].records.usBladeAttacks + gMercProfiles[iId].records.usHtHAttacks);
				if ( uiHits > 100 )
					uiHits = 100;
			}
			else
			{
				// no, set hit % to 0
				uiHits = 0;
			}


			swprintf(sString, L"%d %%%%",uiHits);
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[22].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			sX += StringPixLength( sSpecialCharacters[0],	PERS_FONT );
			mprintf(sX,(pPersonnelScreenPoints[22].y - 8),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[22].x + 148), ( pPersonnelScreenPoints[22].y - 9 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[9] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[9] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[9], (UINT16)( pPersonnelScreenPoints[22].x + 147 ), (UINT16)( pPersonnelScreenPoints[22].y - 10 ),
							(UINT16)( pPersonnelScreenPoints[22].x + 166 ), (UINT16)(pPersonnelScreenPoints[22].y + 1), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[9] );
			fAddedTraitRegion[9] = TRUE;
			// Assign the text
			AssignPersonnelHitPercentageHelpText( iId );

		break;
		case 17:
			// Achievements
			mprintf((INT16)(pPersonnelScreenPoints[23].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[23].y - 6),pPersonnelScreenStrings[PRSNL_TXT_ACHIEVEMNTS]);
			swprintf(sString, L"%d %%%%",CalculateMercsAchievementPercentage( iId ));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[23].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			sX += StringPixLength( sSpecialCharacters[0],	PERS_FONT );
			mprintf(sX,(pPersonnelScreenPoints[23].y - 6),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[23].x + 148), ( pPersonnelScreenPoints[23].y - 7 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[10] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[10] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[10], (UINT16)( pPersonnelScreenPoints[23].x + 147 ), (UINT16)( pPersonnelScreenPoints[23].y - 8 ),
							(UINT16)( pPersonnelScreenPoints[23].x + 166 ), (UINT16)(pPersonnelScreenPoints[23].y + 3), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[10] );
			fAddedTraitRegion[10] = TRUE;
			// Assign the text
			AssignPersonnelAchievementsHelpText( iId );

		break;
		case 18:
			// battles
			mprintf((INT16)(pPersonnelScreenPoints[24].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[24].y - 4),pPersonnelScreenStrings[PRSNL_TXT_BATTLES]);
			swprintf(sString, L"%d",(gMercProfiles[iId].records.usBattlesTactical + gMercProfiles[iId].records.usBattlesAutoresolve));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[24].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[24].y - 4),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[24].x + 148), ( pPersonnelScreenPoints[24].y - 5 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[11] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[11] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[11], (UINT16)( pPersonnelScreenPoints[24].x + 147 ), (UINT16)( pPersonnelScreenPoints[24].y - 6 ),
							(UINT16)( pPersonnelScreenPoints[24].x + 166 ), (UINT16)(pPersonnelScreenPoints[24].y + 5), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[11] );
			fAddedTraitRegion[11] = TRUE;
			// Assign the text
			AssignPersonnelBattlesHelpText( iId );

		break;
		case 19:
			// wounds
			mprintf((INT16)(pPersonnelScreenPoints[25].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[25].y - 2),pPersonnelScreenStrings[PRSNL_TXT_TIMES_WOUNDED]);
			swprintf(sString, L"%d",(gMercProfiles[iId].records.usTimesWoundedShot + gMercProfiles[iId].records.usTimesWoundedStabbed + (gMercProfiles[iId].records.usTimesWoundedPunched/2) + gMercProfiles[iId].records.usTimesWoundedBlasted));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[25].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[25].y - 2),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[25].x + 148), ( pPersonnelScreenPoints[25].y - 3 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[12] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[12] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[12], (UINT16)( pPersonnelScreenPoints[25].x + 147 ), (UINT16)( pPersonnelScreenPoints[25].y - 4 ),
							(UINT16)( pPersonnelScreenPoints[25].x + 166 ), (UINT16)(pPersonnelScreenPoints[25].y + 7), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[12] );
			fAddedTraitRegion[12] = TRUE;
			// Assign the text
			AssignPersonnelWoundsHelpText( iId );

		break;
		}
	}

	return;
}


void EnableDisableDeparturesButtons( void )
{
	// will enable or disable departures buttons based on upperleft picutre index value
	if (fCurrentTeamMode || fNewMailFlag) {
		return;
	}

	// disable both buttons
	DisableButton( giPersonnelButton[ 4 ] );
	DisableButton( giPersonnelButton[ 5 ] );

	if( giCurrentUpperLeftPortraitNumber != 0 ) {
		// enable up button
		EnableButton( giPersonnelButton[ 4 ] );
	}
	if( ( GetNumberOfDeadOnPastTeam( ) + GetNumberOfLeftOnPastTeam( ) + GetNumberOfOtherOnPastTeam( ) ) - giCurrentUpperLeftPortraitNumber	>= MAX_MERCS_ON_SCREEN ) {
		// enable down button
		EnableButton( giPersonnelButton[ 5 ] );
	}
}


void DisplayDepartedCharName( INT32 iId, INT32 iSlot, INT32 iState )
{
	// get merc's nickName, assignment, and sector location info
	INT16 sX, sY;
	CHAR16 sString[ 32 ];

	SetFont(CHAR_NAME_FONT);
	SetFontForeground(PERS_TEXT_FONT_COLOR);
	SetFontBackground(FONT_BLACK);

	if( ( iState == -1 )||( iId == -1 ) )
	{
		return;
	}

	swprintf( sString, L"%s", gMercProfiles[ iId ].zNickname );

		// nick name - assignment
	FindFontCenterCoordinates(IMAGE_BOX_X-5,0,IMAGE_BOX_WIDTH + 90 , 0,sString,CHAR_NAME_FONT, &sX, &sY );

	// cehck to se eif we are going to go off the left edge
	if( sX < pPersonnelScreenPoints[ 0 ].x )
	{
		sX = ( INT16 )pPersonnelScreenPoints[ 0 ].x;
	}

	mprintf( sX + iSlot * IMAGE_BOX_WIDTH, CHAR_NAME_Y, sString );


	// state
	if( gMercProfiles[ iId ].ubMiscFlags2 & PROFILE_MISC_FLAG2_MARRIED_TO_HICKS )
	{
		//displaye 'married'
		swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_MARRIED ] );
	}
	else if(	iState == DEPARTED_DEAD )
	{
		swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_DEAD ] );
	}
	//if the merc is an AIM merc
	//else if( iId < BIFF )
	else if ( gMercProfiles[iId].Type == PROFILETYPE_AIM )
	{
		//if dismissed
		if( iState == DEPARTED_FIRED )
			swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_FIRED ] );
		else
			swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_CONTRACT_EXPIRED ] );
	}

	//else if its a MERC merc
	//else if( iId >= BIFF && iId <= BUBBA )
	else if ( gMercProfiles[iId].Type == PROFILETYPE_MERC )
	{
		if( iState == DEPARTED_FIRED )
			swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_FIRED ] );
		else
			swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_QUIT ] );
	}
	//must be a RPC
	//else
	else if ( gMercProfiles[iId].Type == PROFILETYPE_RPC )
	{
		if( iState == DEPARTED_FIRED )
			swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_FIRED ] );
		else
			swprintf( sString, L"%s", pPersonnelDepartedStateStrings[ DEPARTED_QUIT ] );
	}

	// nick name - assignment
	FindFontCenterCoordinates(IMAGE_BOX_X-5,0,IMAGE_BOX_WIDTH + 90 , 0,sString,CHAR_NAME_FONT, &sX, &sY );

	// cehck to se eif we are going to go off the left edge
	if( sX < pPersonnelScreenPoints[ 0 ].x )
	{
		sX = ( INT16 )pPersonnelScreenPoints[ 0 ].x;
	}

	mprintf( sX + iSlot * IMAGE_BOX_WIDTH, CHAR_NAME_Y + 10 , sString );
}


INT32 GetTheStateOfDepartedMerc( INT32 iId )
{
	INT32 iCounter =0;
	// will runt hrough each list until merc is found, if not a -1 is returned

	for( iCounter = 0; iCounter < 256; iCounter++ )
	{
		if ( LaptopSaveInfo.ubDeadCharactersList[ iCounter ] == iId )
		{
			return( DEPARTED_DEAD );
		}
	}

	for( iCounter = 0; iCounter < 256; iCounter++ )
	{
		if ( LaptopSaveInfo.ubLeftCharactersList[ iCounter ] == iId )
		{
			return( DEPARTED_FIRED );
		}
	}

	for( iCounter = 0; iCounter < 256; iCounter++ )
	{
		if ( LaptopSaveInfo.ubOtherCharactersList[ iCounter ] == iId )
		{
			return( DEPARTED_OTHER );
		}
	}

	return( -1 );
}


void DisplayPersonnelTextOnTitleBar( void )
{
	// draw email screen title text

	// font stuff
	SetFont( FONT14ARIAL );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	// printf the title
	mprintf( PERS_TITLE_X, PERS_TITLE_Y, pPersTitleText[0] );

	// reset the shadow

}

BOOLEAN DisplayHighLightBox( void )
{
	// is the current selected face valid?
	if (fCurrentTeamMode) {
		if( currentTeamIndex == -1 ) {
			return FALSE;
		}
	} else {
		if( iCurrentPersonSelectedId == -1 ) {
			return FALSE;
		}
	}

	// bounding
	VOBJECT_DESC VObjectDesc;
	UINT32 uiBox = 0;
	HVOBJECT hHandle;
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\PicBorde.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &uiBox));

	// blit it
	GetVideoObject(&hHandle, uiBox);
	if (fCurrentTeamMode)
		BltVideoObject(FRAME_BUFFER, hHandle, 0,( INT16 ) ( SMALL_PORTRAIT_START_X+ ( (currentTeamIndex-currentTeamFirstIndex) % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH - 2 ), ( INT16 ) ( SMALL_PORTRAIT_START_Y + ( (currentTeamIndex-currentTeamFirstIndex) / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT - 3 ), VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hHandle, 0,( INT16 ) ( SMALL_PORTRAIT_START_X+ ( iCurrentPersonSelectedId % PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_WIDTH - 2 ), ( INT16 ) ( SMALL_PORTRAIT_START_Y + ( iCurrentPersonSelectedId / PERSONNEL_PORTRAIT_NUMBER_WIDTH ) * SMALL_PORT_HEIGHT - 3 ), VO_BLT_SRCTRANSPARENCY,NULL);

	// deleteit
	DeleteVideoObjectFromIndex( uiBox );

	return ( TRUE );
}

// add to dead list
void AddCharacterToDeadList( SOLDIERTYPE *pSoldier )
{
	for ( INT32 iCounter = 0; iCounter < 256; ++iCounter )
	{
		if( LaptopSaveInfo.ubDeadCharactersList[ iCounter ] == -1 )
		{
			// valid slot, merc not found yet, insert here
			LaptopSaveInfo.ubDeadCharactersList[ iCounter ] = pSoldier->ubProfile;

			// leave
			return;
		}

		// are they already in the list?
		if( LaptopSaveInfo.ubDeadCharactersList[ iCounter ] == pSoldier->ubProfile )
		{
			return;
		}
	}
}


void AddCharacterToFiredList( SOLDIERTYPE *pSoldier )
{
	for ( INT32 iCounter = 0; iCounter < 256; ++iCounter )
	{
		if( LaptopSaveInfo.ubLeftCharactersList[ iCounter ] == -1 )
		{
			// valid slot, merc not found yet, inset here
			LaptopSaveInfo.ubLeftCharactersList[ iCounter ] = pSoldier->ubProfile;

			// leave
			return;
		}

		// are they already in the list?
		if( LaptopSaveInfo.ubLeftCharactersList[ iCounter ] == pSoldier->ubProfile )
		{
			return;
		}
	}
}

void AddCharacterToOtherList( SOLDIERTYPE *pSoldier )
{
	for ( INT32 iCounter = 0; iCounter < 256; ++iCounter )
	{
		if( LaptopSaveInfo.ubOtherCharactersList[ iCounter ] == -1 )
		{
			// valid slot, merc not found yet, inset here
			LaptopSaveInfo.ubOtherCharactersList[ iCounter ] = pSoldier->ubProfile;

			// leave
			return;
		}

		// are they already in the list?
		if( LaptopSaveInfo.ubOtherCharactersList[ iCounter ] == pSoldier->ubProfile )
		{
			return;
		}
	}
}


// If you have hired a merc before, then the they left for whatever reason, and now you are hiring them again,
// we must get rid of them from the departed section in the personnel screen.	( wouldnt make sense for them
//to be on your team list, and departed list )
BOOLEAN RemoveNewlyHiredMercFromPersonnelDepartedList( UINT8 ubProfile )
{
	for ( INT32 iCounter = 0; iCounter < 256; ++iCounter )
	{
		// are they already in the Dead list?
		if( LaptopSaveInfo.ubDeadCharactersList[ iCounter ] == ubProfile )
		{
			//Reset the fact that they were once hired
			LaptopSaveInfo.ubDeadCharactersList[ iCounter ] = -1;
			return( TRUE );
		}

		// are they already in the other list?
		if( LaptopSaveInfo.ubLeftCharactersList[ iCounter ] == ubProfile )
		{
			//Reset the fact that they were once hired
			LaptopSaveInfo.ubLeftCharactersList[ iCounter ] = -1;
			return( TRUE );
		}

		// are they already in the list?
		if( LaptopSaveInfo.ubOtherCharactersList[ iCounter ] == ubProfile )
		{
			//Reset the fact that they were once hired
			LaptopSaveInfo.ubOtherCharactersList[ iCounter ] = -1;
			return( TRUE );
		}
	}

	return( FALSE );
}

// grab the id of the first merc being displayed
INT32 GetIdOfFirstDisplayedMerc( )
{
	if (fCurrentTeamMode) {
		return currentTeamList[currentTeamFirstIndex];
	} else {
		// run through list of soldier on players old team...the slot id will be translated
		return 0;
	}
}


BOOLEAN RenderAtmPanel( void )
{

	VOBJECT_DESC VObjectDesc;
	UINT32 uiBox = 0;
	HVOBJECT hHandle;


	// render the ATM panel
	if( fShowAtmPanel )
	{
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("LAPTOP\\AtmButtons.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &uiBox));

		// blit it
		GetVideoObject(&hHandle, uiBox);
		BltVideoObject(FRAME_BUFFER, hHandle, 0,( INT16 ) ( ATM_UL_X ), ( INT16 ) ( ATM_UL_Y ), VO_BLT_SRCTRANSPARENCY,NULL);

		DeleteVideoObjectFromIndex( uiBox );

		// show amount
		DisplayATMAmount( );
		RenderRectangleForPersonnelTransactionAmount( );

		// create destroy
		CreateDestroyStartATMButton( );
		CreateDestroyATMButton( );

		// display strings for ATM
		DisplayATMStrings( );

		// handle states
		HandleStateOfATMButtons( );

		//DisplayAmountOnCurrentMerc( );

	}
	else
	{
		// just show basic panel
		// bounding
		VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("LAPTOP\\AtmButtons.sti", VObjectDesc.ImageFile);
		CHECKF(AddVideoObject(&VObjectDesc, &uiBox));

		GetVideoObject(&hHandle, uiBox);
		BltVideoObject(FRAME_BUFFER, hHandle, 0,( INT16 ) ( ATM_UL_X ), ( INT16 ) ( ATM_UL_Y ), VO_BLT_SRCTRANSPARENCY,NULL);

		// blit it
		GetVideoObject(&hHandle, uiBox);
		BltVideoObject(FRAME_BUFFER, hHandle, 1,( INT16 ) ( ATM_UL_X + 1 ), ( INT16 ) ( ATM_UL_Y + 18), VO_BLT_SRCTRANSPARENCY,NULL);


		DeleteVideoObjectFromIndex( uiBox );

		// display strings for ATM
		DisplayATMStrings( );

		//DisplayAmountOnCurrentMerc( );

		// create destroy
		CreateDestroyStartATMButton( );
		CreateDestroyATMButton( );
	}
	return( TRUE );
}

void CreateDestroyStartATMButton( void )
{
	static BOOLEAN fCreated = FALSE;
	// create/destroy atm start button as needed
	
	if( ( fCreated == FALSE ) && ( fShowAtmPanelStartButton == TRUE ) )
	{
		// not created, must create

		/*
		// the ATM start button
		giPersonnelATMStartButtonImage[ 0 ]=	LoadButtonImage( "LAPTOP\\AtmButtons.sti" ,-1,2,-1,3,-1 );
		giPersonnelATMStartButton[ 0 ] = QuickCreateButton( giPersonnelATMStartButtonImage[ 0 ] , 519,87,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ATMStartButtonCallback );

		// set text and what not
		SpecifyButtonText( giPersonnelATMStartButton[ 0 ] ,gsAtmStartButtonText[ 0 ] );
		SpecifyButtonUpTextColors( giPersonnelATMStartButton[ 0 ], FONT_BLACK, FONT_BLACK );
		SpecifyButtonFont( giPersonnelATMStartButton[ 0 ], PERS_FONT );
		SetButtonCursor(giPersonnelATMStartButton[ 0 ], CURSOR_LAPTOP_SCREEN);
*/

		INT16 x = iScreenWidthOffset + 519;
		INT16 y = iScreenHeightOffset + 74;

		for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
		{
			giPersonnelATMStartButtonImage[i] = LoadButtonImage( "LAPTOP\\AtmButtons.sti", -1, 2, -1, 3, -1 );
			giPersonnelATMStartButton[i] = QuickCreateButton( giPersonnelATMStartButtonImage[i], x, y,
															  BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
															  MSYS_NO_CALLBACK, (GUI_CALLBACK)PersonnelDataButtonCallback );

			// set text and what not
			SpecifyButtonText( giPersonnelATMStartButton[i], gsAtmStartButtonText[i] );
			SpecifyButtonUpTextColors( giPersonnelATMStartButton[i], FONT_BLACK, FONT_BLACK );
			SpecifyButtonFont( giPersonnelATMStartButton[i], PERS_FONT );
			SetButtonCursor( giPersonnelATMStartButton[i], CURSOR_LAPTOP_SCREEN );

			y += 24;
		}
		
		fCreated = TRUE;
	}
	else if( ( fCreated == TRUE ) && ( fShowAtmPanelStartButton == FALSE ) )
	{
		// stop showing
		for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
		{
			RemoveButton( giPersonnelATMStartButton[i] );
			UnloadButtonImage( giPersonnelATMStartButtonImage[i] );
		}

		fCreated = FALSE;
	}
}

void FindPositionOfPersInvSlider( void )
{
	INT32 iValue = 0;
	INT32 iNumberOfItems = 0;
	INT16 sSizeOfEachSubRegion = 0;

	// find out how many there are
	iValue = ( INT32 )( GetNumberOfInventoryItemsOnCurrentMerc( ) );

	// otherwise there are more than one item
	iNumberOfItems = iValue - NUMBER_OF_INVENTORY_PERSONNEL;

	if( iValue <= 0 )
	{
		iValue = 1;
	}

	// get the subregion sizes
	sSizeOfEachSubRegion = ( INT16 )( ( INT32 )( Y_SIZE_OF_PERSONNEL_SCROLL_REGION - SIZE_OF_PERSONNEL_CURSOR ) / ( INT32 )( iNumberOfItems	) );

	// get slider position
	guiSliderPosition = uiCurrentInventoryIndex * sSizeOfEachSubRegion;
}


void HandleSliderBarClickCallback( MOUSE_REGION *pRegion, INT32 iReason )
{
	INT32 iValue = 0;
	INT32 iNumberOfItems = 0;
	POINT MousePos;
	INT16 sSizeOfEachSubRegion = 0;
	INT16 sYPositionOnBar = 0;
	INT16 iCurrentItemValue = 0;

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN ) || ( iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ) )
	{
		// find out how many there are
		iValue = ( INT32 )( GetNumberOfInventoryItemsOnCurrentMerc( ) );

		// make sure there are more than one page
		if( ( INT32 )uiCurrentInventoryIndex >=	iValue - NUMBER_OF_INVENTORY_PERSONNEL + 1 )
		{
			return;
		}

		// otherwise there are more than one item
		iNumberOfItems = iValue - NUMBER_OF_INVENTORY_PERSONNEL;

		// number of items is 0
		if( iNumberOfItems == 0 )
		{
			return;
		}

		// find the x,y on the slider bar
		GetCursorPos(&MousePos);
		ScreenToClient(ghWindow, &MousePos); // In window coords!

		// get the subregion sizes
		sSizeOfEachSubRegion = ( INT16 )( ( INT32 )( Y_SIZE_OF_PERSONNEL_SCROLL_REGION - SIZE_OF_PERSONNEL_CURSOR ) / ( INT32 )( iNumberOfItems	) );

		// get the cursor placement
		sYPositionOnBar = (INT16) MousePos.y - Y_OF_PERSONNEL_SCROLL_REGION;

		if( sSizeOfEachSubRegion == 0 )
		{
			return;
		}

		// get the actual item position
		iCurrentItemValue = sYPositionOnBar / sSizeOfEachSubRegion;

		if( uiCurrentInventoryIndex != iCurrentItemValue )
		{
			// get slider position
			guiSliderPosition = iCurrentItemValue * sSizeOfEachSubRegion;

			// set current inventory value
			uiCurrentInventoryIndex = ( UINT8 )iCurrentItemValue;

			// force update
			fReDrawScreenFlag = TRUE;
		}
	}
}

void RenderSliderBarForPersonnelInventory( void )
{
	HVOBJECT hHandle;

	// render slider bar for personnel
	GetVideoObject(&hHandle, guiPersonnelInventory );
	BltVideoObject( FRAME_BUFFER, hHandle, 5,( INT16 ) ( X_OF_PERSONNEL_SCROLL_REGION ), ( INT16 ) ( guiSliderPosition + Y_OF_PERSONNEL_SCROLL_REGION), VO_BLT_SRCTRANSPARENCY,NULL);
}

void CreateDestroyATMButton( void )
{
	/*
	static BOOLEAN fCreated = FALSE;
	CHAR16 sString[ 32 ];



	// create/destroy atm start button as needed
	INT32 iCounter = 0;

	if( ( fCreated == FALSE ) && ( fShowAtmPanel == TRUE ) )
	{

		for( iCounter = 0; iCounter < 10; iCounter++ )
		{
			if( iCounter != 9 )
			{
				iNumberPadButtonsImages[ iCounter ]=LoadButtonImage( "LAPTOP\\AtmButtons.sti" ,-1,4,-1,6,-1 );
				swprintf( sString, L"%d", iCounter+1 );
			}
			else
			{
				iNumberPadButtonsImages[ iCounter ]=LoadButtonImage( "LAPTOP\\AtmButtons.sti" ,-1,7,-1,9,-1 );
				swprintf( sString, L"%d", iCounter - 9 );
			}

			iNumberPadButtons[ iCounter ] = QuickCreateButton( iNumberPadButtonsImages[ iCounter ], ( INT16 )( ATM_BUTTONS_START_X + ( ATM_BUTTON_WIDTH * ( INT16 )( iCounter % 3 )) ), ( INT16 )( ATM_BUTTONS_START_Y + ( INT16 )( ATM_BUTTON_HEIGHT * ( iCounter / 3 ))) ,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
									BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ATMNumberButtonCallback );

			if( iCounter != 9)
			{
				MSYS_SetBtnUserData(iNumberPadButtons[iCounter],0,iCounter + 1 );
			}
			else
			{
				MSYS_SetBtnUserData(iNumberPadButtons[iCounter],0, 0 );
			}
			SetButtonCursor(iNumberPadButtons[iCounter], CURSOR_LAPTOP_SCREEN);
			SpecifyButtonFont( iNumberPadButtons[iCounter], PERS_FONT );
			SpecifyButtonText( iNumberPadButtons[iCounter], sString );
			SpecifyButtonUpTextColors( iNumberPadButtons[iCounter], FONT_BLACK, FONT_BLACK );

		}


		// now slap down done, cancel, dep, withdraw
		for( iCounter = OK_ATM; iCounter < NUMBER_ATM_BUTTONS ;iCounter++ )
		{
			if( iCounter == OK_ATM )
			{
				giPersonnelATMSideButtonImage[ iCounter ]=	LoadButtonImage( "LAPTOP\\AtmButtons.sti" ,-1,7,-1,9,-1 );
			}
			else
			{
				giPersonnelATMSideButtonImage[ iCounter ]=	LoadButtonImage( "LAPTOP\\AtmButtons.sti" ,-1,10,-1,12,-1 );
			}

			if( ( iCounter != DEPOSIT_ATM ) && ( iCounter != WIDTHDRAWL_ATM ) )
			{
				giPersonnelATMSideButton[ iCounter ] = QuickCreateButton( giPersonnelATMSideButtonImage[ iCounter ], ( INT16 )( pAtmSideButtonPts[ iCounter ].x ), ( INT16 )( pAtmSideButtonPts[ iCounter ].y ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ATMOtherButtonCallback );
			}
			else
			{
				giPersonnelATMSideButton[ iCounter ] = QuickCreateButton( giPersonnelATMSideButtonImage[ iCounter ], ( INT16 )( pAtmSideButtonPts[ iCounter ].x ), ( INT16 )( pAtmSideButtonPts[ iCounter ].y ),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ATMOther2ButtonCallback );
			}
			MSYS_SetBtnUserData(giPersonnelATMSideButton[iCounter],0,iCounter );
			SpecifyButtonFont( giPersonnelATMSideButton[iCounter], PERS_FONT );
			SetButtonCursor(giPersonnelATMSideButton[iCounter], CURSOR_LAPTOP_SCREEN);
			SpecifyButtonUpTextColors( giPersonnelATMSideButton[iCounter], FONT_BLACK, FONT_BLACK );
			SpecifyButtonText( giPersonnelATMSideButton[iCounter], gsAtmSideButtonText[ iCounter ] );
		}


		//SetButtonCursor(giPersonnelATMStartButton, CURSOR_LAPTOP_SCREEN);
		fCreated = TRUE;
	}
	else if( ( fCreated == TRUE ) && ( fShowAtmPanel == FALSE ) )
	{
		// stop showing
		//RemoveButton( giPersonnelATMButton );
		//UnloadButtonImage( giPersonnelATMButtonImage );

		for( iCounter = 0; iCounter < 10; iCounter++ )
		{
			UnloadButtonImage( iNumberPadButtonsImages[ iCounter ] );
			RemoveButton( iNumberPadButtons[ iCounter ] );
		}

		for( iCounter = OK_ATM; iCounter < NUMBER_ATM_BUTTONS ;iCounter++ )
		{
			RemoveButton( giPersonnelATMSideButton[ iCounter ]	);
			UnloadButtonImage( giPersonnelATMSideButtonImage[ iCounter ] );
		}

		fCreated = FALSE;
	}

	*/
}


void ATMStartButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			fReDrawScreenFlag=TRUE;
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fReDrawScreenFlag=TRUE;
			fShowAtmPanel = TRUE;
			fShowAtmPanelStartButton = FALSE;
			fATMFlags = 0;

		}
	}
}

void PersonnelDataButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		UINT32	uiButton = btn->IDNum;

		fReDrawScreenFlag=TRUE;

		for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
		{
			ButtonList[giPersonnelATMStartButton[i]]->uiFlags &= ~(BUTTON_CLICKED_ON);

			if ( uiButton == giPersonnelATMStartButton[i] )
				gubPersonnelInfoState = i;
		}
		
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void ATMOther2ButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT32 iValue = 0;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	iValue = MSYS_GetBtnUserData( btn, 0 );

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			fReDrawScreenFlag=TRUE;
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);

		switch( iValue )
		{
			case( DEPOSIT_ATM ):
				fATMFlags = 2;
				fReDrawScreenFlag=TRUE;
				ButtonList[ giPersonnelATMSideButton[ WIDTHDRAWL_ATM ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
			break;
			case( WIDTHDRAWL_ATM ):
				fATMFlags = 3;
				fReDrawScreenFlag=TRUE;
				ButtonList[ giPersonnelATMSideButton[ DEPOSIT_ATM ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
			break;
		}
	}
}

void ATMOtherButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT32 iValue = 0;
	SOLDIERTYPE *pSoldier = MercPtrs[ 0 ];

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	iValue = MSYS_GetBtnUserData( btn, 0 );

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			fReDrawScreenFlag=TRUE;
		}
		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			if (fCurrentTeamMode && 
				currentTeamIndex != -1) {
				// set soldier
				pSoldier = MercPtrs[ currentTeamList[currentTeamIndex] ];

				switch( iValue ) {
					case( OK_ATM ):
						if( fATMFlags == 0 ) {
							fATMFlags = 1;
							fReDrawScreenFlag=TRUE;
							fOneFrameDelayInPersonnel = TRUE;
						} else if( fATMFlags == 2 ) {
							// deposit from merc to account
							if( GetFundsOnMerc( pSoldier ) >= wcstol( sTransferString, NULL, 10 ) ) {
								if( ( wcstol( sTransferString, NULL, 10 ) % 10 ) != 0 ) {
									fOldATMFlags = fATMFlags;
									fATMFlags = 5;
									iValue = ( wcstol( sTransferString, NULL, 10 ) - ( wcstol( sTransferString, NULL, 10 ) % 10 ) );
									swprintf( sTransferString, L"%d", iValue );
									fReDrawScreenFlag=TRUE;
								} else {
									// transfer
									TransferFundsFromMercToBank( pSoldier, wcstol( sTransferString, NULL, 10 ) );
									sTransferString[ 0 ] = 0;
									fReDrawScreenFlag=TRUE;
								}
							} else {
								fOldATMFlags = fATMFlags;
								fATMFlags = 4;
								iValue = GetFundsOnMerc( pSoldier );
								swprintf( sTransferString, L"%d", iValue );
								fReDrawScreenFlag=TRUE;
							}
						} else if( fATMFlags == 3 ) {
							// deposit from merc to account
							if( LaptopSaveInfo.iCurrentBalance >= wcstol( sTransferString, NULL, 10 ) ) {
								if( ( wcstol( sTransferString, NULL, 10 ) % 10 ) != 0 ) {
									fOldATMFlags = fATMFlags;
									fATMFlags = 5;
									iValue = ( wcstol( sTransferString, NULL, 10 ) - ( wcstol( sTransferString, NULL, 10 ) % 10 ) );
									swprintf( sTransferString, L"%d", iValue );
									fReDrawScreenFlag=TRUE;
								} else {
									// transfer
									TransferFundsFromBankToMerc( pSoldier, wcstol( sTransferString, NULL, 10 ) );
									sTransferString[ 0 ] = 0;
									fReDrawScreenFlag=TRUE;
								}
							} else {
								fOldATMFlags = fATMFlags;
								fATMFlags = 4;
								iValue = LaptopSaveInfo.iCurrentBalance;
								swprintf( sTransferString, L"%d", iValue );
								fReDrawScreenFlag=TRUE;
							}
						} else if( fATMFlags == 4 ) {
							fATMFlags = fOldATMFlags;
							fReDrawScreenFlag=TRUE;
						}
						break;
					case( DEPOSIT_ATM ):
						fATMFlags = 2;
						fReDrawScreenFlag=TRUE;
						break;
					case( WIDTHDRAWL_ATM ):
						fATMFlags = 3;
						fReDrawScreenFlag=TRUE;
						break;
					case( CANCEL_ATM ):
						if( sTransferString[ 0 ] != 0 ) {
							sTransferString[ 0 ] = 0;
						} else if( fATMFlags != 0 ) {
							fATMFlags = 0;
							ButtonList[ giPersonnelATMSideButton[ WIDTHDRAWL_ATM ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
							ButtonList[ giPersonnelATMSideButton[ DEPOSIT_ATM ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
						} else {
							fShowAtmPanel = FALSE;
							fShowAtmPanelStartButton = TRUE;
						}
						fReDrawScreenFlag=TRUE;
						break;
					case( CLEAR_ATM ):
						sTransferString[ 0 ] = 0;
							fReDrawScreenFlag=TRUE;
						break;
				} // switch
			}
		}
	}
}

void ATMNumberButtonCallback(GUI_BUTTON *btn,INT32 reason)
{

	INT32 iValue = 0;
	INT32 iCounter = 0;
	CHAR16 sZero[ 2 ] = L"0";

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	iValue = MSYS_GetBtnUserData( btn, 0 );

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
			fReDrawScreenFlag=TRUE;
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			// find position in value string, append character at end
			for( iCounter = 0; iCounter < ( INT32 )wcslen( sTransferString ) ; iCounter++ );
			sTransferString[ iCounter ] = ( sZero[ 0 ] + ( UINT16 )iValue );
			sTransferString[ iCounter + 1 ] = 0;
			fReDrawScreenFlag=TRUE;

			// gone too far
			if( StringPixLength( sTransferString, ATM_FONT ) >= ATM_DISPLAY_WIDTH - 10 )
			{
				sTransferString[ iCounter ] = 0;
			}
		}
	}
}

void DisplayATMAmount( void )
{

	INT16 sX = 0, sY = 0;
	CHAR16 sTempString[ 32 ];
	INT32 iCounter = 0;

	if( fShowAtmPanel == FALSE )
	{
		return;
	}

	wcscpy( sTempString, sTransferString );

	if( ( sTempString[ 0 ] == 48 ) && ( sTempString[ 1 ] != 0 ) )
	{
		// strip the zero from the beginning
		for(iCounter = 1; iCounter < ( INT32 )wcslen( sTempString ); iCounter++ )
		{
			sTempString[ iCounter - 1 ] = sTempString[ iCounter ];
		}
	}

	// insert commas and dollar sign
	InsertCommasForDollarFigure( sTempString );
	InsertDollarSignInToString( sTempString );

	// set font
	SetFont( ATM_FONT );

	// set back and foreground
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );


	// right justify
	FindFontRightCoordinates( ATM_DISPLAY_X, ATM_DISPLAY_Y + 37, ATM_DISPLAY_WIDTH, ATM_DISPLAY_HEIGHT, sTempString, ATM_FONT, &sX, &sY );

	// print string
	mprintf( sX, sY, sTempString );

	return;
}


void HandleStateOfATMButtons( void )
{
	INT32 iCounter = 0;

	// disable buttons based on state
	if( ( fATMFlags == 0 ) )
	{
		for( iCounter = 0; iCounter < 10 ; iCounter++ )
		{
			DisableButton( iNumberPadButtons[ iCounter ] );
		}

		for( iCounter = 0; iCounter < NUMBER_ATM_BUTTONS; iCounter++ )
		{
			if( ( iCounter != DEPOSIT_ATM) && ( iCounter != WIDTHDRAWL_ATM ) && ( iCounter != CANCEL_ATM )	)
			{
				DisableButton( giPersonnelATMSideButton[ iCounter ] );
			}
		}
	}
	else
	{
		for( iCounter = 0; iCounter < 10; iCounter++ )
		{
			EnableButton( iNumberPadButtons[ iCounter ] );
		}

		for( iCounter = 0; iCounter < NUMBER_ATM_BUTTONS; iCounter++ )
		{
			EnableButton( giPersonnelATMSideButton[ iCounter ] );
		}
	}
}


INT32 GetFundsOnMerc( SOLDIERTYPE *pSoldier )
{
	INT32 iCurrentAmount = 0;
	UINT32 iCurrentPocket = 0;
	// run through mercs pockets, if any money in them, add to total

	// error check
	if( pSoldier == NULL )
	{
		return 0;
	}

	// run through grunts pockets and count all the spare change
	UINT32 invsize = pSoldier->inv.size();
	for( iCurrentPocket = 0; iCurrentPocket < invsize; ++iCurrentPocket )
	{
		if ( Item[ pSoldier->inv[ iCurrentPocket ] .usItem ].usItemClass == IC_MONEY )
		{
			iCurrentAmount += pSoldier->inv[ iCurrentPocket ][0]->data.money.uiMoneyAmount;
		}
	}

	return iCurrentAmount;
}


BOOLEAN TransferFundsFromMercToBank( SOLDIERTYPE *pSoldier, INT32 iCurrentBalance )
{
	// move this amount of money from the grunt to the bank
	// error check
	if( pSoldier == NULL )
	{
		return FALSE;
	}

	UINT32 iCurrentPocket = 0;
	INT32 iAmountLeftToTake = iCurrentBalance;
	// run through grunts pockets and count all the spare change
	UINT32 invsize = pSoldier->inv.size();
	for( iCurrentPocket = 0; iCurrentPocket < invsize; ++iCurrentPocket )
	{
		if ( Item[ pSoldier->inv[ iCurrentPocket ] .usItem ].usItemClass == IC_MONEY )
		{

			// is there more left to go, or does this pocket finish it off?
			if( pSoldier->inv[ iCurrentPocket ][0]->data.money.uiMoneyAmount > ( UINT32 )iAmountLeftToTake )
			{
				pSoldier->inv[ iCurrentPocket ][0]->data.money.uiMoneyAmount -= iAmountLeftToTake;
				iAmountLeftToTake = 0;
			}
			else
			{
				iAmountLeftToTake	-= pSoldier->inv[ iCurrentPocket ][0]->data.money.uiMoneyAmount;
				pSoldier->inv[ iCurrentPocket ][0]->data.money.uiMoneyAmount = 0;

				//Remove the item out off the merc
				DeleteObj(&pSoldier->inv[iCurrentPocket]);
			}
		}
	}

	if( iAmountLeftToTake != 0 )
	{
		// something wrong
		AddTransactionToPlayersBook ( TRANSFER_FUNDS_FROM_MERC, 	pSoldier->ubProfile, GetWorldTotalMin() , ( iCurrentBalance - iAmountLeftToTake ) );
		return ( FALSE );
	}
	else
	{
		// everything ok
		AddTransactionToPlayersBook ( TRANSFER_FUNDS_FROM_MERC, 	pSoldier->ubProfile, GetWorldTotalMin() , ( iCurrentBalance ) );
		return ( TRUE );
	}
}


BOOLEAN TransferFundsFromBankToMerc( SOLDIERTYPE *pSoldier, INT32 iCurrentBalance )
{
	// move this amount of money from the grunt to the bank
	// error check
	if( pSoldier == NULL )
	{
		return FALSE;
	}

	// make sure we are giving them some money
	if( iCurrentBalance <= 0 )
	{
		return ( FALSE );
	}

	// current balance
	if( iCurrentBalance > LaptopSaveInfo.iCurrentBalance )
	{
		iCurrentBalance = LaptopSaveInfo.iCurrentBalance;
	}


	// set up money object
	CreateMoney(iCurrentBalance, &gTempObject);

	// now auto place money object
	if( AutoPlaceObject( pSoldier, &( gTempObject ), TRUE ) == TRUE )
	{
		// now place transaction
		AddTransactionToPlayersBook ( TRANSFER_FUNDS_TO_MERC, pSoldier->ubProfile, GetWorldTotalMin() , -( iCurrentBalance ) );
	}
	else
	{
		// error, notify player that merc doesn't have the spce for this much cash
	}

	return( TRUE );
}

void DisplayATMStrings( void )
{
	// display strings for ATM
	switch( fATMFlags )
	{
		case( 0 ):
			if( fShowAtmPanelStartButton == FALSE )
			{
				DisplayWrappedString(iScreenWidthOffset + 509, ( INT16 )( iScreenHeightOffset + 80 ), 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 3 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
			}
		break;
		case( 2 ):
			if( sTransferString[ 0 ] != 0 )
			{
				DisplayWrappedString(iScreenWidthOffset + 509, iScreenHeightOffset + 80, 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 0 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
			}
			else
			{
				DisplayWrappedString(iScreenWidthOffset + 509, iScreenHeightOffset + 80, 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 2 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
			}
		break;
		case( 3 ):
			if( sTransferString[ 0 ] != 0 )
			{
				DisplayWrappedString(iScreenWidthOffset + 509, iScreenHeightOffset + 80, 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 0 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
				//DisplayWrappedString(509, ( INT16 )( 80 + GetFontHeight( ATM_FONT ) ), 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 1 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
			}
			else
			{
				DisplayWrappedString(iScreenWidthOffset + 509, iScreenHeightOffset + 80, 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 2 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
			}
		break;
		case( 4 ):
			// not enough money
			DisplayWrappedString(iScreenWidthOffset + 509, iScreenHeightOffset + 80, 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 4 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
		break;
		case( 5 ):
			// not enough money
			DisplayWrappedString(iScreenWidthOffset + 509, iScreenHeightOffset + 73, 81, 2, ATM_FONT, FONT_WHITE, sATMText[ 5 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
		break;

	}
}

void UpDateStateOfStartButton( void )
{
	// start button being shown?
	if (!fShowAtmPanelStartButton)
		return;

	for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
		ButtonList[giPersonnelATMStartButton[i]]->uiFlags &= ~(BUTTON_CLICKED_ON);

	ButtonList[giPersonnelATMStartButton[gubPersonnelInfoState]]->uiFlags |= BUTTON_CLICKED_ON;

	// if in current mercs and the currently selected guy is valid, enable button, else disable it
	if (fCurrentTeamMode)
	{
		// is the current guy valid
		if (currentTeamIndex != -1)
		{
			for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
				EnableButton( giPersonnelATMStartButton[ i ] );

			INT32 iId = currentTeamList[currentTeamIndex];

			if (iId != -1)
			{
				if ( Menptr[ iId ].bAssignment == ASSIGNMENT_POW )
				{
					DisableButton( giPersonnelATMStartButton[ PERSONNEL_INV_BTN ] );

					if ( gubPersonnelInfoState == PERSONNEL_INV_BTN )
					{
						gubPersonnelInfoState = PERSONNEL_STAT_BTN;
						fPausedReDrawScreenFlag = TRUE;
					}

					if( fATMFlags )
					{
						fATMFlags = 0;
						fPausedReDrawScreenFlag = TRUE;
					}
				}
			}
		}
		else
		{
			// not valid, disable
			for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
				DisableButton( giPersonnelATMStartButton[ i ] );
		}
	}
	else
	{
		// disable button
		for ( int i = 0; i < PERSONNEL_NUM_BTN; ++i )
			DisableButton( giPersonnelATMStartButton[i] );
	}
}

void DisplayAmountOnCurrentMerc( void )
{
	// will display the amount that the merc is carrying on him or herself
	SOLDIERTYPE *pSoldier = NULL;
	CHAR16 sString[ 64 ];
	INT16 sX, sY;

	if (currentTeamIndex == -1) {
		pSoldier = NULL;
	} else {
		// set soldier
		pSoldier = MercPtrs[ currentTeamList[currentTeamIndex] ];
	}

	INT32 iFunds = GetFundsOnMerc( pSoldier );

	swprintf( sString, L"%d", iFunds );

	// insert commas and dollar sign
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );

	// set font
	SetFont( ATM_FONT );

	// set back and foreground
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	// right justify
	FindFontRightCoordinates( ATM_DISPLAY_X, ATM_DISPLAY_Y, ATM_DISPLAY_WIDTH, ATM_DISPLAY_HEIGHT, sString, ATM_FONT, &sX, &sY );

	// print string
	mprintf( sX, sY, sString );
}

void HandlePersonnelKeyboard( void )
{
	INT32 iCounter = 0;
	INT32 iValue = 0;
	CHAR16 sZero[ 2 ] = L"0";

	InputAtom					InputEvent;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	//while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	while (DequeueEvent(&InputEvent) == TRUE)
	{
		if ( InputEvent.usEvent == KEY_DOWN )
		{
			switch (InputEvent.usParam)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if( ( fShowAtmPanel ) && ( fATMFlags != 0 ) )
					{
						iValue = ( INT32 )( InputEvent.usParam - '0' );

						for( iCounter = 0; iCounter < ( INT32 )wcslen( sTransferString ) ; iCounter++ );
						sTransferString[ iCounter ] = ( sZero[ 0 ] + ( UINT16 )iValue );
						sTransferString[ iCounter + 1 ] = 0;
						fPausedReDrawScreenFlag=TRUE;

						// gone too far
						if( StringPixLength( sTransferString, ATM_FONT ) >= ATM_DISPLAY_WIDTH - 10 )
						{
							sTransferString[ iCounter ] = 0;
						}
					}
				break;
				case UPARROW:
				case 'w':
					if ( fCurrentTeamMode )
					{
						if( gubPersonnelInfoState > PERSONNEL_STAT_BTN )
							gubPersonnelInfoState--;
						else
							gubPersonnelInfoState = PERSONNEL_NUM_BTN - 1;

						fReDrawScreenFlag = TRUE;

						uiCurrentInventoryIndex = 0;
						guiSliderPosition = 0;

						//if the selected merc is valid, and they are a POW, change to the inventory display
						if( currentTeamIndex != -1 && Menptr[currentTeamList[currentTeamIndex]].bAssignment == ASSIGNMENT_POW && gubPersonnelInfoState == PERSONNEL_INV_BTN)
							gubPersonnelInfoState = PERSONNEL_EMPLOYMENT_BTN;

						fPausedReDrawScreenFlag = TRUE;
					}
				break;
				case DNARROW:
				case 's':
					if ( fCurrentTeamMode )
					{
						if( gubPersonnelInfoState < PERSONNEL_NUM_BTN-1 )
							gubPersonnelInfoState++;
						else
							gubPersonnelInfoState = PERSONNEL_STAT_BTN;

						fReDrawScreenFlag = TRUE;

						uiCurrentInventoryIndex = 0;
						guiSliderPosition = 0;

						//if the selected merc is valid, and they are a POW, change to the inventory display
						if( currentTeamIndex != -1 && Menptr[currentTeamList[currentTeamIndex]].bAssignment == ASSIGNMENT_POW && gubPersonnelInfoState == PERSONNEL_INV_BTN)
							gubPersonnelInfoState = PERSONNEL_STAT_BTN;
						
						fPausedReDrawScreenFlag = TRUE;
					}
				break;
				case LEFTARROW:
				case 'a':
					fReDrawScreenFlag = TRUE;
					PrevPersonnelFace( );
					uiCurrentInventoryIndex = 0;
					guiSliderPosition = 0;
					fPausedReDrawScreenFlag = TRUE;
				break;
				case RIGHTARROW:
				case 'd':
					fReDrawScreenFlag = TRUE;
					NextPersonnelFace( );
					uiCurrentInventoryIndex = 0;
					guiSliderPosition = 0;
					fPausedReDrawScreenFlag = TRUE;
				break;
				case SHIFT_LEFTARROW:
				case 'A':
					fReDrawScreenFlag = TRUE;
					PrevPersonnelFacePage();
					uiCurrentInventoryIndex = 0;
					guiSliderPosition = 0;
					fPausedReDrawScreenFlag = TRUE;
				break;
				case SHIFT_RIGHTARROW:
				case 'D':
					fReDrawScreenFlag = TRUE;
					NextPersonnelFacePage();
					uiCurrentInventoryIndex = 0;
					guiSliderPosition = 0;
					fPausedReDrawScreenFlag = TRUE;
				break;
				case SHIFT_TAB:
					if ( !fCurrentTeamMode )
					{
						fCurrentTeamMode = TRUE;
						// how many people do we have?..if you have someone set default to 0
						if( maxCurrentTeamIndex >= 0 )
							// get id of first merc in list
							currentTeamIndex = 0;
						else
							currentTeamIndex = -1;
					}
					else
					{
						fCurrentTeamMode = FALSE;
						// how many departed people?
						if (GetNumberOfPastMercsOnPlayersTeam() > 0) {
							iCurrentPersonSelectedId = 0;
						} else {
							iCurrentPersonSelectedId = -1;
						}
						//Switch the panel on the right to be the stat panel
						gubPersonnelInfoState = PERSONNEL_STAT_BTN;
					}
					fReDrawScreenFlag = TRUE;
					fPausedReDrawScreenFlag = TRUE;
				break;
				default:
					HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
			}
		}
		else if( InputEvent.usEvent == KEY_REPEAT )
		{
			switch (InputEvent.usParam)
			{
				case LEFTARROW:
				case 'a':
					fReDrawScreenFlag = TRUE;
					PrevPersonnelFace( );
					uiCurrentInventoryIndex = 0;
					guiSliderPosition = 0;
					fPausedReDrawScreenFlag = TRUE;
				break;
				case RIGHTARROW:
				case 'd':
					fReDrawScreenFlag = TRUE;
					NextPersonnelFace( );
					uiCurrentInventoryIndex = 0;
					guiSliderPosition = 0;
					fPausedReDrawScreenFlag = TRUE;
				break;
			}
		}
	}
}

void RenderRectangleForPersonnelTransactionAmount( void )
{
	INT32 iLength = 0;
	INT32 iHeight = GetFontHeight( ATM_FONT );
	UINT32										uiDestPitchBYTES;
	UINT8											*pDestBuf;
	CHAR16 sTempString[ 32 ];
	INT32	iCounter = 0;
	
	wcscpy( sTempString, sTransferString );

	if( ( sTempString[ 0 ] == 48 ) && ( sTempString[ 1 ] != 0 ) )
	{
		// strip the zero from the beginning
		for(iCounter = 1; iCounter < ( INT32 )wcslen( sTempString ); iCounter++ )
		{
			sTempString[ iCounter - 1 ] = sTempString[ iCounter ];
		}
	}

	// insert commas and dollar sign
	InsertCommasForDollarFigure( sTempString );
	InsertDollarSignInToString( sTempString );

	// string not worth worrying about?
	if( wcslen( sTempString ) < 2 )
	{
		return;
	}

	// grab total length
	iLength = StringPixLength( sTempString, ATM_FONT );

	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	RestoreClipRegionToFullScreenForRectangle( uiDestPitchBYTES );
	RectangleDraw( TRUE, ( ATM_DISPLAY_X + ATM_DISPLAY_WIDTH ) - iLength - 2,	ATM_DISPLAY_Y + 35, ATM_DISPLAY_X + ATM_DISPLAY_WIDTH + 1, ATM_DISPLAY_Y + iHeight + 36, Get16BPPColor( FROMRGB( 255, 255, 255 ) ), pDestBuf );
	UnLockVideoSurface( FRAME_BUFFER );
}

void HandleTimedAtmModes( void )
{
	static BOOLEAN fOldAtmMode = 0;
	static UINT32 uiBaseTime = 0;

	if( fShowAtmPanel == FALSE )
	{
		return;
	}

	// update based on modes
	if( fATMFlags != fOldAtmMode )
	{
		uiBaseTime = GetJA2Clock();
		fOldAtmMode = fATMFlags;
		fPausedReDrawScreenFlag = TRUE;
 	}
	
	if( ( GetJA2Clock() - uiBaseTime ) > DELAY_PER_MODE_CHANGE_IN_ATM )
	{
		switch( fATMFlags )
		{
			case( 4 ):
			case( 5 ):
				// insufficient funds ended
				fATMFlags = fOldATMFlags;
				fPausedReDrawScreenFlag = TRUE;
			break;
		}
	}
}

BOOLEAN IsPastMercDead( INT32 iId )
{
	if( GetTheStateOfDepartedMerc( GetIdOfPastMercInSlot( iId ) ) == DEPARTED_DEAD )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN IsPastMercFired( INT32 iId )
{
	if( GetTheStateOfDepartedMerc( GetIdOfPastMercInSlot( iId )	) == DEPARTED_FIRED )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

BOOLEAN IsPastMercOther( INT32 iId )
{
	if( GetTheStateOfDepartedMerc( GetIdOfPastMercInSlot( iId )	) == DEPARTED_OTHER )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

void DisplayEmploymentinformation( INT32 iId, INT32 iSlot )
{
	INT32 iCounter=0;
	CHAR16 sString[50];
	CHAR16 sStringA[ 50 ];
	INT16 sX, sY;
	UINT32 uiHits = 0;
	HVOBJECT hHandle;

	// SANDRO - remove the regions
	for( INT8 i = 0; i < 13; i++ )
	{
		if( fAddedTraitRegion[i] )
		{
			MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[i] );
			fAddedTraitRegion[i] = FALSE;
		}
	}

	if( Menptr[iId].flags.uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	// display the stats for a char
	for(iCounter=0;iCounter <MAX_STATS; ++iCounter)
	{
		switch(iCounter)
		{

//		case 12:

		//Remaining Contract:
		case 0:
		{
			//UINT32 uiTimeUnderThisDisplayAsHours = 24*60;
#ifdef JA2UB
			wcscpy( sString, gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
			mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[PRSNL_TXT_CURRENT_CONTRACT]);
#else
			static const UINT32 uiMinutesInDay = 24 * 60;

				if(Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC || Menptr[iId].ubProfile == SLAY )
				{
					INT32 iTimeLeftOnContract = CalcTimeLeftOnMercContract( &Menptr[iId] );

					//if the merc is in transit
					if( Menptr[iId ].bAssignment == IN_TRANSIT )
					{
						//and if the ttime left on the cotract is greater then the contract time
						if( iTimeLeftOnContract > (INT32)( Menptr[iId].iTotalContractLength * uiMinutesInDay ) )
						{
							iTimeLeftOnContract = ( Menptr[iId].iTotalContractLength * uiMinutesInDay );
						}
					}
					// if there is going to be a both days and hours left on the contract
					if( iTimeLeftOnContract / uiMinutesInDay )
					{
						swprintf(sString, L"%d%s %d%s / %d%s",( iTimeLeftOnContract / uiMinutesInDay ), gpStrategicString[ STR_PB_DAYS_ABBREVIATION ], (iTimeLeftOnContract % uiMinutesInDay)/60, gpStrategicString[ STR_PB_HOURS_ABBREVIATION ], Menptr[iId].iTotalContractLength, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ]);
						mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[PRSNL_TXT_CURRENT_CONTRACT]);
					}

					//else there is under a day left
					else
					{
						//DEF: removed 2/7/99
						swprintf(sString, L"%d%s / %d%s", (iTimeLeftOnContract % uiMinutesInDay)/60, gpStrategicString[ STR_PB_HOURS_ABBREVIATION ], Menptr[iId].iTotalContractLength, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ]);
						mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[PRSNL_TXT_CURRENT_CONTRACT]);
					}

				}
				else if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__MERC)
				{
//					swprintf(sString, L"%d%s / %d%s",Menptr[iId].iTotalContractLength, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ], ( GetWorldTotalMin( ) -Menptr[iId].iStartContractTime ) / ( 24 * 60 ), gpStrategicString[ STR_PB_DAYS_ABBREVIATION ] );

					wcscpy( sString, gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
					mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[PRSNL_TXT_CURRENT_CONTRACT]);
				}
				else
				{
					wcscpy( sString, gpStrategicString[ STR_PB_NOTAPPLICABLE_ABBREVIATION ] );
					mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[PRSNL_TXT_CURRENT_CONTRACT]);
				}
#endif
		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)+Prsnl_DATA_OffSetX),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		}
		break;

//		case 11:
//		case 19:
		case 1:

			// total contract time served
			mprintf((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter].y,pPersonnelScreenStrings[PRSNL_TXT_TOTAL_SERVICE]);

			//./DEF 2/4/99: total service days used to be calced as 'days -1'

			swprintf(sString, L"%d %s",gMercProfiles[Menptr[iId].ubProfile].usTotalDaysServed, gpStrategicString[ STR_PB_DAYS_ABBREVIATION ] );

		FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)+Prsnl_DATA_OffSetX),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
		mprintf(sX,pPersonnelScreenPoints[iCounter].y,sString);
		break;

//		case 13:
		case 3:
		// cost (PRSNL_TXT_TOTAL_COST)

/*
			if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC)
			{
				UINT32 uiDailyCost = 0;

				if( Menptr[iId].bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
				{
					// 2 week contract
					uiDailyCost = gMercProfiles[ Menptr[ iId ].ubProfile ].uiBiWeeklySalary / 14;
				}
				else if( Menptr[iId].bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK )
				{
					// 1 week contract
					uiDailyCost = gMercProfiles[ Menptr[ iId ].ubProfile ].uiWeeklySalary / 7;
				}
				else
				{
					uiDailyCost = gMercProfiles[ Menptr[ iId ].ubProfile ].sSalary;
				}

//				swprintf( sString, L"%d",uiDailyCost * Menptr[ iId ].iTotalContractLength );
				swprintf( sString, L"%d", gMercProfiles[ Menptr[ iId ].ubProfile ].uiTotalCostToDate );
			}
			else if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__MERC)
			{
//					swprintf( sString, L"%d",gMercProfiles[ Menptr[ iId ].ubProfile ].sSalary * gMercProfiles[ Menptr[ iId ].ubProfile ].iMercMercContractLength );
					swprintf( sString, L"%d", gMercProfiles[ Menptr[ iId ].ubProfile ].uiTotalCostToDate );
			}
			else
			{
				//Display a $0 amount
//				swprintf( sString, L"0" );

				swprintf( sString, L"%d", gMercProfiles[ Menptr[ iId ].ubProfile ].uiTotalCostToDate );
			}
*/
				swprintf( sString, L"%d", gMercProfiles[ Menptr[ iId ].ubProfile ].uiTotalCostToDate );

				// insert commas and dollar sign
				InsertCommasForDollarFigure( sString );
				InsertDollarSignInToString( sString );

/*
DEF:3/19/99:
			if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__MERC )
			{
			swprintf( sStringA, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_UNPAID_AMOUNT ] );
			}
			else
*/
			{
				swprintf( sStringA, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_TOTAL_COST ]	);
			}

			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)+Prsnl_DATA_OffSetX),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf( (INT16)(pPersonnelScreenPoints[iCounter].x +(iSlot*TEXT_BOX_WIDTH) ),pPersonnelScreenPoints[ iCounter ].y,sStringA);

			// print contract cost
			mprintf( ( INT16 ) ( sX ) , pPersonnelScreenPoints[iCounter].y,sString);

			if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC)
			{
				// daily rate
				if( Menptr[iId].bTypeOfLastContract == CONTRACT_EXTEND_2_WEEK )
				{
					// 2 week contract
					swprintf( sStringA, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].uiBiWeeklySalary / 14 );
				InsertCommasForDollarFigure( sStringA );
					InsertDollarSignInToString( sStringA );
					swprintf( sString, L"%s", sStringA );
				}
				else if( Menptr[iId].bTypeOfLastContract == CONTRACT_EXTEND_1_WEEK )
				{
					// 1 week contract
					swprintf( sStringA, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].uiWeeklySalary / 7 );
				InsertCommasForDollarFigure( sStringA );
					InsertDollarSignInToString( sStringA );
					swprintf( sString, L"%s",	sStringA );
				}
				else
				{

					swprintf( sStringA, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].sSalary );
					InsertCommasForDollarFigure( sStringA );
				InsertDollarSignInToString( sStringA );
					swprintf( sString,	L"%s", sStringA );
				}
			}
			else if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__MERC)
			{
//DEF: 99/2/7
//				swprintf( sStringA, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].sSalary * Menptr[ iId ].iTotalContractLength);
				swprintf( sStringA, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].sSalary );
				InsertCommasForDollarFigure( sStringA );
				InsertDollarSignInToString( sStringA );
				swprintf( sString,	L"%s", sStringA );
			}

			else
			{
				//Display a $0 amount
//				swprintf( sString, L"0" );
//				InsertDollarSignInToString( sString );
				swprintf( sStringA, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].sSalary );
				InsertCommasForDollarFigure( sStringA );
				InsertDollarSignInToString( sStringA );
				swprintf( sString,	L"%s", sStringA );
			}

			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter].x+(iSlot*TEXT_BOX_WIDTH)+Prsnl_DATA_OffSetX),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);

//			iCounter++;
			iCounter++;

		// now print daily rate
			mprintf( ( INT16 )( sX ),pPersonnelScreenPoints[ iCounter+1 ].y,sString);
			mprintf( (INT16)(pPersonnelScreenPoints[iCounter+1].x +(iSlot*TEXT_BOX_WIDTH) ),pPersonnelScreenPoints[ iCounter +1].y, pPersonnelScreenStrings[PRSNL_TXT_DAILY_COST]);

			break;

		case 5:
		// medical deposit

			//if its a merc merc, display the salary oweing
			if( Menptr[iId].ubWhatKindOfMercAmI == MERC_TYPE__MERC )
			{
				mprintf((INT16)(pPersonnelScreenPoints[iCounter-1].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter-1].y,pPersonnelScreenStrings[PRSNL_TXT_UNPAID_AMOUNT]);

				swprintf( sString, L"%d", gMercProfiles[Menptr[ iId ].ubProfile].sSalary * gMercProfiles[Menptr[ iId ].ubProfile ].iMercMercContractLength );
				InsertCommasForDollarFigure( sString );
				InsertDollarSignInToString( sString );

				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter-1].x+(iSlot*TEXT_BOX_WIDTH)+Prsnl_DATA_OffSetX),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
				mprintf(sX,pPersonnelScreenPoints[iCounter-1].y,sString);
			}
			else
			{
				mprintf((INT16)(pPersonnelScreenPoints[iCounter-1].x+(iSlot*TEXT_BOX_WIDTH)),pPersonnelScreenPoints[iCounter-1].y,pPersonnelScreenStrings[PRSNL_TXT_MED_DEPOSIT]);

				swprintf(sString, L"%d",gMercProfiles[Menptr[iId].ubProfile].sMedicalDepositAmount);

				// insert commas and dollar sign
				InsertCommasForDollarFigure( sString );
				InsertDollarSignInToString( sString );

				FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[iCounter-1].x+(iSlot*TEXT_BOX_WIDTH)+Prsnl_DATA_OffSetX),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
				mprintf(sX,pPersonnelScreenPoints[iCounter-1].y,sString);
			}


		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// SANDRO - show num kills, hit percentage, times wounded etc. in here instead of stats table

		case 14:
		// kills
			mprintf((INT16)(pPersonnelScreenPoints[20].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[20].y - 12),pPersonnelScreenStrings[PRSNL_TXT_KILLS]);

			swprintf(sString, L"%d",(gMercProfiles[Menptr[iId].ubProfile].records.usKillsElites + gMercProfiles[Menptr[iId].ubProfile].records.usKillsRegulars + gMercProfiles[Menptr[iId].ubProfile].records.usKillsAdmins + gMercProfiles[Menptr[iId].ubProfile].records.usKillsHostiles + gMercProfiles[Menptr[iId].ubProfile].records.usKillsCreatures + gMercProfiles[Menptr[iId].ubProfile].records.usKillsZombies + gMercProfiles[Menptr[iId].ubProfile].records.usKillsTanks + gMercProfiles[Menptr[iId].ubProfile].records.usKillsOthers));

			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[20].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[20].y - 12),sString);

			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[20].x + 148), ( pPersonnelScreenPoints[20].y - 13 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[7] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[7] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[7], (UINT16)( pPersonnelScreenPoints[20].x + 147 ), (UINT16)( pPersonnelScreenPoints[20].y - 14 ),
							(UINT16)( pPersonnelScreenPoints[20].x + 166 ), (UINT16)(pPersonnelScreenPoints[20].y - 3), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[7] );
			fAddedTraitRegion[7] = TRUE;
			// Assign the text
			AssignPersonnelKillsHelpText( Menptr[iId].ubProfile );

		break;
		case 15:
			// assists
			mprintf((INT16)(pPersonnelScreenPoints[21].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[21].y - 10),pPersonnelScreenStrings[PRSNL_TXT_ASSISTS]);
			swprintf(sString, L"%d",(gMercProfiles[Menptr[iId].ubProfile].records.usAssistsMercs + gMercProfiles[Menptr[iId].ubProfile].records.usAssistsMilitia + gMercProfiles[Menptr[iId].ubProfile].records.usAssistsOthers));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[21].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[21].y - 10),sString);
		
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[21].x + 148), ( pPersonnelScreenPoints[21].y - 11 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[8] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[8] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[8], (UINT16)( pPersonnelScreenPoints[21].x + 147 ), (UINT16)( pPersonnelScreenPoints[21].y - 12 ),
							(UINT16)( pPersonnelScreenPoints[21].x + 166 ), (UINT16)(pPersonnelScreenPoints[21].y - 1), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[8] );
			fAddedTraitRegion[8] = TRUE;
			// Assign the text
			AssignPersonnelAssistsHelpText( Menptr[iId].ubProfile );

		break;
		case 16:
			// shots/hits
			mprintf((INT16)(pPersonnelScreenPoints[22].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[22].y - 8),pPersonnelScreenStrings[PRSNL_TXT_HIT_PERCENTAGE]);
			uiHits = ( UINT32 )gMercProfiles[Menptr[iId].ubProfile].records.usShotsHit;
			uiHits *= 100;

			// check we have shot at least once
			if( (gMercProfiles[Menptr[iId].ubProfile].records.usShotsFired + gMercProfiles[Menptr[iId].ubProfile].records.usMissilesLaunched + gMercProfiles[Menptr[iId].ubProfile].records.usGrenadesThrown + gMercProfiles[Menptr[iId].ubProfile].records.usKnivesThrown + gMercProfiles[Menptr[iId].ubProfile].records.usBladeAttacks + gMercProfiles[Menptr[iId].ubProfile].records.usHtHAttacks) > 0 )
			{
				uiHits /= ( UINT32 )(gMercProfiles[Menptr[iId].ubProfile].records.usShotsFired + gMercProfiles[Menptr[iId].ubProfile].records.usMissilesLaunched + gMercProfiles[Menptr[iId].ubProfile].records.usGrenadesThrown + gMercProfiles[Menptr[iId].ubProfile].records.usKnivesThrown + gMercProfiles[Menptr[iId].ubProfile].records.usBladeAttacks + gMercProfiles[Menptr[iId].ubProfile].records.usHtHAttacks);
				if ( uiHits > 100 )
					uiHits = 100;
			}
			else
			{
				// no, set hit % to 0
				uiHits = 0;
			}


			swprintf(sString, L"%d %%%%",uiHits);
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[22].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			sX += StringPixLength( sSpecialCharacters[0],	PERS_FONT );
			mprintf(sX,(pPersonnelScreenPoints[22].y - 8),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[22].x + 148), ( pPersonnelScreenPoints[22].y - 9 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[9] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[9] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[9], (UINT16)( pPersonnelScreenPoints[22].x + 147 ), (UINT16)( pPersonnelScreenPoints[22].y - 10 ),
							(UINT16)( pPersonnelScreenPoints[22].x + 166 ), (UINT16)(pPersonnelScreenPoints[22].y + 1), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[9] );
			fAddedTraitRegion[9] = TRUE;
			// Assign the text
			AssignPersonnelHitPercentageHelpText( Menptr[iId].ubProfile );

		break;
		case 17:
			// Achievements
			mprintf((INT16)(pPersonnelScreenPoints[23].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[23].y - 6),pPersonnelScreenStrings[PRSNL_TXT_ACHIEVEMNTS]);
			swprintf(sString, L"%d %%%%", CalculateMercsAchievementPercentage( Menptr[iId].ubProfile ));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[23].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			sX += StringPixLength( sSpecialCharacters[0],	PERS_FONT );
			mprintf(sX,(pPersonnelScreenPoints[23].y - 6),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[23].x + 148), ( pPersonnelScreenPoints[23].y - 7 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[10] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[10] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[10], (UINT16)( pPersonnelScreenPoints[23].x + 147 ), (UINT16)( pPersonnelScreenPoints[23].y - 8 ),
							(UINT16)( pPersonnelScreenPoints[23].x + 166 ), (UINT16)(pPersonnelScreenPoints[23].y + 3), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[10] );
			fAddedTraitRegion[10] = TRUE;
			// Assign the text
			AssignPersonnelAchievementsHelpText( Menptr[iId].ubProfile );

		break;
		case 18:
			// battles
			mprintf((INT16)(pPersonnelScreenPoints[24].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[24].y - 4),pPersonnelScreenStrings[PRSNL_TXT_BATTLES]);
			swprintf(sString, L"%d",(gMercProfiles[Menptr[iId].ubProfile].records.usBattlesTactical + gMercProfiles[Menptr[iId].ubProfile].records.usBattlesAutoresolve));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[24].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[24].y - 4),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[24].x + 148), ( pPersonnelScreenPoints[24].y - 5 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[11] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[11] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[11], (UINT16)( pPersonnelScreenPoints[24].x + 147 ), (UINT16)( pPersonnelScreenPoints[24].y - 6 ),
							(UINT16)( pPersonnelScreenPoints[24].x + 166 ), (UINT16)(pPersonnelScreenPoints[24].y + 5), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[11] );
			fAddedTraitRegion[11] = TRUE;
			// Assign the text
			AssignPersonnelBattlesHelpText( Menptr[iId].ubProfile );

		break;
		case 19:
			// wounds
			mprintf((INT16)(pPersonnelScreenPoints[25].x+(iSlot*TEXT_BOX_WIDTH)),(pPersonnelScreenPoints[25].y - 2),pPersonnelScreenStrings[PRSNL_TXT_TIMES_WOUNDED]);
			swprintf(sString, L"%d",(gMercProfiles[Menptr[iId].ubProfile].records.usTimesWoundedShot + gMercProfiles[Menptr[iId].ubProfile].records.usTimesWoundedStabbed + (gMercProfiles[Menptr[iId].ubProfile].records.usTimesWoundedPunched/2) + gMercProfiles[Menptr[iId].ubProfile].records.usTimesWoundedBlasted));
			FindFontRightCoordinates((INT16)(pPersonnelScreenPoints[25].x+(iSlot*TEXT_BOX_WIDTH)),0,TEXT_BOX_WIDTH-20,0,sString, PERS_FONT,	&sX, &sY);
			mprintf(sX,(pPersonnelScreenPoints[25].y - 2),sString);
			
			GetVideoObject(&hHandle, guiQMark);
			BltVideoObject( FRAME_BUFFER, hHandle, 0,(pPersonnelScreenPoints[25].x + 148), ( pPersonnelScreenPoints[25].y - 3 ), VO_BLT_SRCTRANSPARENCY,NULL );

			// Add specific region for fast help window
			if( fAddedTraitRegion[12] )
			{
				MSYS_RemoveRegion( &gSkillTraitHelpTextRegion[12] );
			}
			MSYS_DefineRegion( &gSkillTraitHelpTextRegion[12], (UINT16)( pPersonnelScreenPoints[25].x + 147 ), (UINT16)( pPersonnelScreenPoints[25].y - 4 ),
							(UINT16)( pPersonnelScreenPoints[25].x + 166 ), (UINT16)(pPersonnelScreenPoints[25].y + 7), MSYS_PRIORITY_HIGH,
								MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NULL );
			MSYS_AddRegion( &gSkillTraitHelpTextRegion[12] );
			fAddedTraitRegion[12] = TRUE;
			// Assign the text
			AssignPersonnelWoundsHelpText( Menptr[iId].ubProfile );

		break;

		/////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}

// AIM merc:	Returns the amount of time left on mercs contract
// MERC merc: Returns the amount of time the merc has worked
// IMP merc:	Returns the amount of time the merc has worked
// else:			returns -1
INT32 CalcTimeLeftOnMercContract( SOLDIERTYPE *pSoldier )
{
	INT32 iTimeLeftOnContract = -1;

	if(pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__AIM_MERC)
	{
		iTimeLeftOnContract = pSoldier->iEndofContractTime-GetWorldTotalMin();

		if( iTimeLeftOnContract < 0 )
			iTimeLeftOnContract = 0;
	}
	else if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__MERC)
	{
		iTimeLeftOnContract = gMercProfiles[ pSoldier->ubProfile ].iMercMercContractLength;
	}

	else if( pSoldier->ubWhatKindOfMercAmI == MERC_TYPE__PLAYER_CHARACTER )
	{
		iTimeLeftOnContract = pSoldier->iTotalContractLength;
	}

	else
	{
		iTimeLeftOnContract = -1;
	}

	return( iTimeLeftOnContract );
}

// SANDRO - Popup text windows for traits
void AssignPersonnelSkillTraitHelpText( UINT8 ubTraitNumber, BOOLEAN fExpertLevel, BOOLEAN fRegMale, STR16 apStr )
{
	//CHAR16	apStr[ 5000 ];
	CHAR16	atStr[ 1500 ];

	if( gGameOptions.fNewTraitSystem )
	{
		switch( ubTraitNumber )
		{
			case AUTO_WEAPONS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubAWBonusCtHAssaultRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[0], ( gSkillTraitValues.ubAWBonusCtHAssaultRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWBonusCtHSMGs != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[1], ( gSkillTraitValues.ubAWBonusCtHSMGs * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWBonusCtHLMGs != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[2], ( gSkillTraitValues.ubAWBonusCtHLMGs * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWFiringSpeedBonusLMGs != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[3], ( gSkillTraitValues.ubAWFiringSpeedBonusLMGs * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWPercentReadyLMGReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[4], ( gSkillTraitValues.ubAWPercentReadyLMGReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWAutoFirePenaltyReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[5], ( gSkillTraitValues.ubAWAutoFirePenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAWUnwantedBulletsReduction > 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsAutoWeapons[6]);
					wcscat( apStr, atStr );
				}
				// Flugente: focus is a skill that can be used by multiple traits. For simplicity, the ini values are in the sniper trait section
				if ( gSkillTraitValues.ubSNFocusRadius != 0 && gSkillTraitValues.sSNFocusInterruptBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[19], gSkillTraitValues.sSNFocusInterruptBonus );
					wcscat( apStr, atStr );
				}
				break;
			}
			case HEAVY_WEAPONS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[0], ( gSkillTraitValues.ubHWGrenadeLaunchersAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWRocketLaunchersAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[1], ( gSkillTraitValues.ubHWRocketLaunchersAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWBonusCtHGrenadeLaunchers != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[2], ( gSkillTraitValues.ubHWBonusCtHGrenadeLaunchers * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWBonusCtHRocketLaunchers != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[3], ( gSkillTraitValues.ubHWBonusCtHRocketLaunchers * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWMortarAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[4], ( gSkillTraitValues.ubHWMortarAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWMortarCtHPenaltyReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[5], ( gSkillTraitValues.ubHWMortarCtHPenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWDamageTanksBonusPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[6], ( gSkillTraitValues.ubHWDamageTanksBonusPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubHWDamageBonusPercentForHW != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsHeavyWeapons[7], ( gSkillTraitValues.ubHWDamageBonusPercentForHW * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				// Flugente: focus is a skill that can be used by multiple traits. For simplicity, the ini values are in the sniper trait section
				if ( gSkillTraitValues.ubSNFocusRadius != 0 && gSkillTraitValues.sSNFocusInterruptBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[19], gSkillTraitValues.sSNFocusInterruptBonus );
					wcscat( apStr, atStr );
				}
				break;
			}
			case SNIPER_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubSNBonusCtHRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[0], ( gSkillTraitValues.ubSNBonusCtHRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNBonusCtHSniperRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[1], ( gSkillTraitValues.ubSNBonusCtHSniperRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNEffRangeToTargetReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[2], ( gSkillTraitValues.ubSNEffRangeToTargetReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNAimingBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[3], ( gSkillTraitValues.ubSNAimingBonusPerClick * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNDamageBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[4], ( gSkillTraitValues.ubSNDamageBonusPerClick * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					if( gSkillTraitValues.ubSNDamageBonusFromNumClicks == 0)
					{
						wcscat( apStr, gzIMPMajorTraitsHelpTextsSniper[5] );
						wcscat( apStr, gzIMPMajorTraitsHelpTextsSniper[6] );
					}
					else if( gSkillTraitValues.ubSNDamageBonusFromNumClicks == 1 )
					{
						wcscat( atStr, gzIMPMajorTraitsHelpTextsSniper[6] );
					}
					else
					{
						wcscat( atStr, gzIMPMajorTraitsHelpTextsSniper[6] );
						wcscat( atStr, gzIMPMajorTraitsHelpTextsSniper[gSkillTraitValues.ubSNDamageBonusFromNumClicks + 4] );
					}
					wcscat( atStr, L"\n" );
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNChamberRoundAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[14], ( gSkillTraitValues.ubSNChamberRoundAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNAimClicksAdded != 0 )
				{
					if( UsingNewCTHSystem() == true )
					{
						if( gSkillTraitValues.ubSNAimClicksAdded == 1 && !fExpertLevel )
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[17]);
						else
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[18], ( gSkillTraitValues.ubSNAimClicksAdded * (fExpertLevel ? 2 : 1)));
					}
					else
					{
						if( gSkillTraitValues.ubSNAimClicksAdded == 1 && !fExpertLevel )
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[15]);
						else
							swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[16], ( gSkillTraitValues.ubSNAimClicksAdded * (fExpertLevel ? 2 : 1)));
					}
					wcscat( apStr, atStr );
				}

				if ( gSkillTraitValues.ubSNFocusRadius != 0 && gSkillTraitValues.sSNFocusInterruptBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[19], gSkillTraitValues.sSNFocusInterruptBonus );
					wcscat( apStr, atStr );
				}

				break;
			}
			case RANGER_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubRABonusCtHRifles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[0], ( gSkillTraitValues.ubRABonusCtHRifles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRABonusCtHShotguns != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[1], ( gSkillTraitValues.ubRABonusCtHShotguns * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAPumpShotgunsAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[2], ( gSkillTraitValues.ubRAPumpShotgunsAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAFiringSpeedBonusShotguns != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[3], ( gSkillTraitValues.ubRAFiringSpeedBonusShotguns * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAReloadSpeedShotgunsManual != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[7], ( gSkillTraitValues.ubRAReloadSpeedShotgunsManual * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAAimClicksAdded != 0 )
				{
					if( gSkillTraitValues.ubRAAimClicksAdded == 1 && !fExpertLevel )
						swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[4], (UINT8)( gSkillTraitValues.ubRAAimClicksAdded * (fExpertLevel ? 2 : 1) ) );
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[5], (UINT8)( gSkillTraitValues.ubRAAimClicksAdded * (fExpertLevel ? 2 : 1) ) );

					wcscat( apStr, atStr );
					// half of the above bonus also applies to rifles
					if( (gSkillTraitValues.ubRAAimClicksAdded >= 2 && !fExpertLevel) || (gSkillTraitValues.ubRAAimClicksAdded == 1 && fExpertLevel) )
						swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[8], (UINT8)( gSkillTraitValues.ubRAAimClicksAdded * (fExpertLevel ? 2 : 1) / 2.0f));
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[9], (UINT8)( gSkillTraitValues.ubRAAimClicksAdded * (fExpertLevel ? 2 : 1) / 2.0f));

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubRAEffectiveRangeBonusShotguns != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsRanger[6], ( gSkillTraitValues.ubRAEffectiveRangeBonusShotguns * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				// Flugente: focus is a skill that can be used by multiple traits. For simplicity, the ini values are in the sniper trait section
				if ( gSkillTraitValues.ubSNFocusRadius != 0 && gSkillTraitValues.sSNFocusInterruptBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[19], gSkillTraitValues.sSNFocusInterruptBonus );
					wcscat( apStr, atStr );
				}

				break;
			}
			case GUNSLINGER_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubGSFiringSpeedBonusPistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[0], ( gSkillTraitValues.ubGSFiringSpeedBonusPistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSEffectiveRangeBonusPistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[1], ( gSkillTraitValues.ubGSEffectiveRangeBonusPistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSBonusCtHPistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[2], ( gSkillTraitValues.ubGSBonusCtHPistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSBonusCtHMachinePistols != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[3], ( gSkillTraitValues.ubGSBonusCtHMachinePistols * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					if( gSkillTraitValues.ubGSCtHMPExcludeAuto )
						wcscat( atStr, gzIMPMajorTraitsHelpTextsGunslinger[4] );
					wcscat( atStr, L"\n");
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSAimingBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[5], ( gSkillTraitValues.ubGSAimingBonusPerClick * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSPercentReadyPistolsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[6], ( gSkillTraitValues.ubGSPercentReadyPistolsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[7], ( gSkillTraitValues.ubGSRealoadSpeedHandgunsBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubGSAimClicksAdded != 0 )
				{
					if( gSkillTraitValues.ubGSAimClicksAdded == 1 && !fExpertLevel )
						swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[8], ( gSkillTraitValues.ubGSAimClicksAdded * (fExpertLevel ? 2 : 1)));
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[9], ( gSkillTraitValues.ubGSAimClicksAdded * (fExpertLevel ? 2 : 1)));

					wcscat( apStr, atStr );
				}
				// Flugente: focus is a skill that can be used by multiple traits. For simplicity, the ini values are in the sniper trait section
				if ( gSkillTraitValues.ubSNFocusRadius != 0 && gSkillTraitValues.sSNFocusInterruptBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSniper[19], gSkillTraitValues.sSNFocusInterruptBonus );
					wcscat( apStr, atStr );
				}

				// Flugente: can we fan the hammer on certain guns (it's effectively a hidden mode of revolvers)?
				if ( gSkillTraitValues.fCanFanTheHammer )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsGunslinger[10] );
					wcscat( apStr, atStr );
				}
				break;
			}
			case MARTIAL_ARTS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubMAPunchAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[0], ( gSkillTraitValues.ubMAPunchAPsReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusCtHBareHands != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[1], ( gSkillTraitValues.ubMABonusCtHBareHands * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusCtHBrassKnuckles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[2], ( gSkillTraitValues.ubMABonusCtHBrassKnuckles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusDamageHandToHand != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[3], ( gSkillTraitValues.ubMABonusDamageHandToHand * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMABonusBreathDamageHandToHand != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[4], ( gSkillTraitValues.ubMABonusBreathDamageHandToHand * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usMALostBreathRegainPenalty != 0 )
				{
					if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 25)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[5]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 50)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[6]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 100)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[7]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 200)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[8]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 400)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[9]);
					else if( (gSkillTraitValues.usMALostBreathRegainPenalty * (fExpertLevel ? 2 : 1)) <= 700)
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[10]);
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[11]);

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usMAAimedPunchDamageBonus != 0 )
				{
					if ( !fRegMale || (gSkillTraitValues.fPermitExtraAnimationsOnlyToMA && !fExpertLevel) )
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[12], ( gSkillTraitValues.usMAAimedPunchDamageBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					else
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[13], ( gSkillTraitValues.usMAAimedPunchDamageBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAChanceToDodgeHtH != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[14], ( gSkillTraitValues.ubMAChanceToDodgeHtH * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAOnTopCTDHtHBareHanded != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[15], ( gSkillTraitValues.ubMAOnTopCTDHtHBareHanded * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					if( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles == gSkillTraitValues.ubMAOnTopCTDHtHBareHanded ) 
						wcscat( apStr, gzIMPMajorTraitsHelpTextsMartialArts[16] );
					else if( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles > 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[17], ( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
					wcscat( apStr, L"\n" );
				}
				else if( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[18], ( gSkillTraitValues.ubMAOnTopCTDHtHBrassKnuckles * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAChanceToDodgeMelee != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[19], ( gSkillTraitValues.ubMAChanceToDodgeMelee * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAReducedAPsToSteal != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[20], ( gSkillTraitValues.ubMAReducedAPsToSteal * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAAPsChangeStanceReduction != 0 && ( gSkillTraitValues.ubMAAPsChangeStanceReduction == gSkillTraitValues.ubMAApsTurnAroundReduction == gSkillTraitValues.ubMAAPsClimbOrJumpReduction ))
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[21], ( gSkillTraitValues.ubMAAPsChangeStanceReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				else 
				{
					if( gSkillTraitValues.ubMAAPsChangeStanceReduction != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[22], ( gSkillTraitValues.ubMAAPsChangeStanceReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
					if( gSkillTraitValues.ubMAApsTurnAroundReduction != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[23], ( gSkillTraitValues.ubMAApsTurnAroundReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
					if( gSkillTraitValues.ubMAAPsClimbOrJumpReduction != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[24], ( gSkillTraitValues.ubMAAPsClimbOrJumpReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
				}
				if( gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving != 0 && UsingImprovedInterruptSystem() )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[27], ( gSkillTraitValues.ubMAReducedAPsRegisteredWhenMoving * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMAChanceToCkickDoors != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[25], ( gSkillTraitValues.ubMAChanceToCkickDoors * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}

				if ( fRegMale &&
					((gSkillTraitValues.fPermitExtraAnimationsOnlyToMA && fExpertLevel) ||
					!gSkillTraitValues.fPermitExtraAnimationsOnlyToMA ))
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsMartialArts[26]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case SQUADLEADER_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubSLBonusAPsPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[0], ( gSkillTraitValues.ubSLBonusAPsPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLEffectiveLevelInRadius != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[1], ( gSkillTraitValues.ubSLEffectiveLevelInRadius * (fExpertLevel ? 2 : 1)), (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLEffectiveLevelAsStandby != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[2], ( gSkillTraitValues.ubSLEffectiveLevelAsStandby * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLCollectiveInterruptsBonus != 0 && UsingImprovedInterruptSystem() )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[11], ( gSkillTraitValues.ubSLCollectiveInterruptsBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLOverallSuppresionBonusPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[3], ( gSkillTraitValues.ubSLOverallSuppresionBonusPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0], (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLMoraleGainBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[4], ( gSkillTraitValues.ubSLMoraleGainBonus * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLMoraleLossReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[5], ( gSkillTraitValues.ubSLMoraleLossReduction * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
							
				swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[6], gSkillTraitValues.usSLRadiusNormal);
				wcscat( apStr, atStr );
				if( gSkillTraitValues.usSLRadiusExtendedEar > gSkillTraitValues.usSLRadiusNormal )
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[7], gSkillTraitValues.usSLRadiusExtendedEar);

				wcscat( apStr, atStr );
				wcscat( apStr, L"\n" );

				if( gSkillTraitValues.ubSLMaxBonuses > 1 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[8], gSkillTraitValues.ubSLMaxBonuses ) ;
					wcscat( apStr, atStr );
				}
				
				if( gSkillTraitValues.ubSLFearResistance != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[9], ( gSkillTraitValues.ubSLFearResistance * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0], (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSLDeathMoralelossMultiplier != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSquadleader[10], (1 + ( gSkillTraitValues.ubSLDeathMoralelossMultiplier * (fExpertLevel ? 2 : 1))), (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				break;
			}
			case TECHNICIAN_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.usTERepairSpeedBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[0], ( gSkillTraitValues.usTERepairSpeedBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usTELockpickingBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[1], ( gSkillTraitValues.usTELockpickingBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usTEDisarmElTrapBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[2], ( gSkillTraitValues.usTEDisarmElTrapBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usTEAttachingItemsBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[3], ( gSkillTraitValues.usTEAttachingItemsBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTEUnjamGunBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[4], ( gSkillTraitValues.ubTEUnjamGunBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTERepairElectronicsPenaltyReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[5], ( gSkillTraitValues.ubTERepairElectronicsPenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTEChanceToDetectTrapsBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[6], ( gSkillTraitValues.ubTEChanceToDetectTrapsBonus * (fExpertLevel ? 2 : 1)));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTECtHControlledRobotBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[7], ( gSkillTraitValues.ubTECtHControlledRobotBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0], (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTETraitsNumToRepairRobot == 2 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[8], (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );
				}
				else if( gSkillTraitValues.ubTETraitsNumToRepairRobot == 1 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[8], (fExpertLevel ? gzMercSkillTextNew[ubTraitNumber + NEWTRAIT_MERCSKILL_EXPERTOFFSET] : gzMercSkillTextNew[ubTraitNumber]));
					wcscat( apStr, atStr );

					if( gSkillTraitValues.ubTERepairRobotPenaltyReduction != 0 && fExpertLevel)
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[9], ( gSkillTraitValues.ubTERepairRobotPenaltyReduction * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
					}
				}
				if ( gGameExternalOptions.fAdvRepairSystem && gSkillTraitValues.fTETraitsCanRestoreItemThreshold )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsTechnician[10] );
					wcscat( apStr, atStr );
				}
				break;
			}
			case DOCTOR_NT:
			{
				BOOLEAN fCanSurgery = FALSE;
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubDONumberTraitsNeededForSurgery != 0 && ((gSkillTraitValues.ubDOSurgeryHealPercentBase + gSkillTraitValues.ubDOSurgeryHealPercentOnTop) > 0))
				{
					if( gSkillTraitValues.ubDONumberTraitsNeededForSurgery <= (fExpertLevel ? 2 : 1))
					{
						fCanSurgery = TRUE;
						swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[0]);
						wcscat( apStr, atStr );
						swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[1], (gSkillTraitValues.ubDOSurgeryHealPercentBase + ( gSkillTraitValues.ubDOSurgeryHealPercentOnTop * (fExpertLevel ? 2 : 1))), sSpecialCharacters[0]);
						wcscat( apStr, atStr );
						if( gSkillTraitValues.usDOSurgeryMedBagConsumption >= 60 )
							wcscat( apStr, gzIMPMajorTraitsHelpTextsDoctor[2] );

						wcscat( apStr, L"\n" );

						if ( gSkillTraitValues.ubDOSurgeryHealPercentBloodbag )
						{
							swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[10], gSkillTraitValues.ubDOSurgeryHealPercentBloodbag, sSpecialCharacters[0] );
							wcscat( apStr, atStr );
						}
					}
				}
				if( (gSkillTraitValues.usDORepairStatsRateBasic + gSkillTraitValues.usDORepairStatsRateOnTop) > 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[3]);
					wcscat( apStr, atStr );
					if( fCanSurgery )
						wcscat( apStr, gzIMPMajorTraitsHelpTextsDoctor[4] );

					wcscat( apStr, gzIMPMajorTraitsHelpTextsDoctor[5] );
				}
				if( gSkillTraitValues.usDODoctorAssignmentBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[6], ( gSkillTraitValues.usDODoctorAssignmentBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDOBandagingSpeedPercent != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[7], ( gSkillTraitValues.ubDOBandagingSpeedPercent * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDONaturalRegenBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[8], ( gSkillTraitValues.ubDONaturalRegenBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					if( gSkillTraitValues.ubDOMaxRegenBonuses > 1 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsDoctor[9], gSkillTraitValues.ubDOMaxRegenBonuses ) ;
						wcscat( apStr, atStr );
					}
					wcscat( apStr, L"\n" );
				}
				break;
			}
			case AMBIDEXTROUS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubAMPenaltyDoubleReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[0], gSkillTraitValues.ubAMPenaltyDoubleReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMReloadSpeedMagazines != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[1], gSkillTraitValues.ubAMReloadSpeedMagazines, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMReloadSpeedLoose != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[2], gSkillTraitValues.ubAMReloadSpeedLoose, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMPickItemsAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[3], gSkillTraitValues.ubAMPickItemsAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMWorkBackpackAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[4], gSkillTraitValues.ubAMWorkBackpackAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMHandleDoorsAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[5], gSkillTraitValues.ubAMHandleDoorsAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMHandleBombsAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[6], gSkillTraitValues.ubAMHandleBombsAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubAMAttachingItemsAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAmbidextrous[7], gSkillTraitValues.ubAMAttachingItemsAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case MELEE_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubMEBladesAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[0], gSkillTraitValues.ubMEBladesAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMECtHBladesBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[1], gSkillTraitValues.ubMECtHBladesBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMECtHBluntBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[2], gSkillTraitValues.ubMECtHBluntBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMEDamageBonusBlades != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[3], gSkillTraitValues.ubMEDamageBonusBlades, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMEDamageBonusBlunt != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[4], gSkillTraitValues.ubMEDamageBonusBlunt, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usMEAimedMeleeAttackDamageBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[5], gSkillTraitValues.usMEAimedMeleeAttackDamageBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMEDodgeBladesBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[6], gSkillTraitValues.ubMEDodgeBladesBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMECtDBladesOnTopWithBladeInHands != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[7], gSkillTraitValues.ubMECtDBladesOnTopWithBladeInHands, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMEDodgeBluntBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[8], gSkillTraitValues.ubMEDodgeBluntBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubMECtDBluntOnTopWithBladeInHands != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsMelee[9], gSkillTraitValues.ubMECtDBluntOnTopWithBladeInHands, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case THROWING_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubTHBladesAPsReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[0], gSkillTraitValues.ubTHBladesAPsReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesMaxRange != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[1], gSkillTraitValues.ubTHBladesMaxRange, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesCtHBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[2], gSkillTraitValues.ubTHBladesCtHBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesCtHBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[3], gSkillTraitValues.ubTHBladesCtHBonusPerClick, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesDamageBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[4], gSkillTraitValues.ubTHBladesDamageBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesDamageBonusPerClick != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[5], gSkillTraitValues.ubTHBladesDamageBonusPerClick, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesSilentCriticalHitChance != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[6], gSkillTraitValues.ubTHBladesSilentCriticalHitChance, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesCriticalHitMultiplierBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[7], gSkillTraitValues.ubTHBladesCriticalHitMultiplierBonus);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHBladesAimClicksAdded != 0 )
				{
					if( gSkillTraitValues.ubTHBladesAimClicksAdded == 1 )
						swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[8], gSkillTraitValues.ubTHBladesAimClicksAdded );
					else
						swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[9], gSkillTraitValues.ubTHBladesAimClicksAdded );

					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHAPsNeededToThrowGrenadesReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[10], gSkillTraitValues.ubTHAPsNeededToThrowGrenadesReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHMaxRangeToThrowGrenades != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[11], gSkillTraitValues.ubTHMaxRangeToThrowGrenades, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTHCtHWhenThrowingGrenades != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsThrowing[12], gSkillTraitValues.ubTHCtHWhenThrowingGrenades, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case NIGHT_OPS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubNOeSightRangeBonusInDark != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[0], gSkillTraitValues.ubNOeSightRangeBonusInDark, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubNOHearingRangeBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[1], gSkillTraitValues.ubNOHearingRangeBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubNOHearingRangeBonusInDark != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[2], gSkillTraitValues.ubNOHearingRangeBonusInDark, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubNOIterruptsBonusInDark != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[3], gSkillTraitValues.ubNOIterruptsBonusInDark, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubNONeedForSleepReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsNightOps[4], gSkillTraitValues.ubNONeedForSleepReduction);
					wcscat( apStr, atStr );
				}
				break;
			}
			case STEALTHY_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubSTStealthModeSpeedBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[0], gSkillTraitValues.ubSTStealthModeSpeedBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSTBonusToMoveQuietly != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[1], gSkillTraitValues.ubSTBonusToMoveQuietly, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSTStealthBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[2], gSkillTraitValues.ubSTStealthBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSTReducedAPsRegistered != 0 && UsingImprovedInterruptSystem() )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[4], gSkillTraitValues.ubSTReducedAPsRegistered, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSTStealthPenaltyForMovingReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsStealthy[3], gSkillTraitValues.ubSTStealthPenaltyForMovingReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case ATHLETICS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubATAPsMovementReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAthletics[0], gSkillTraitValues.ubATAPsMovementReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubATBPsMovementReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsAthletics[1], gSkillTraitValues.ubATBPsMovementReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case BODYBUILDING_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubBBDamageResistance != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[0], gSkillTraitValues.ubBBDamageResistance, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubBBCarryWeightBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[1], gSkillTraitValues.ubBBCarryWeightBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubBBBreathLossForHtHImpactReduction != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[2], gSkillTraitValues.ubBBBreathLossForHtHImpactReduction, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usBBIncreasedNeededDamageToFallDown != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsBodybuilding[3], gSkillTraitValues.usBBIncreasedNeededDamageToFallDown, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case DEMOLITIONS_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubDEDamageOfBombsAndMines != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[0], gSkillTraitValues.ubDEDamageOfBombsAndMines, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDEAttachDetonatorCheckBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[1], gSkillTraitValues.ubDEAttachDetonatorCheckBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[2], gSkillTraitValues.ubDEPlantAndRemoveBombCheckBonus, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDEPlacedBombLevelBonus != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[3], gSkillTraitValues.ubDEPlacedBombLevelBonus);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubDEShapedChargeDamageMultiplier != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsDemolitions[4], gSkillTraitValues.ubDEShapedChargeDamageMultiplier);
					wcscat( apStr, atStr );
				}
				break;
			}
			case TEACHING_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubTGBonusToTrainMilitia != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[0], gSkillTraitValues.ubTGBonusToTrainMilitia, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[1], gSkillTraitValues.ubTGEffectiveLDRToTrainMilitia, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTGBonusToTeachOtherMercs != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[2], gSkillTraitValues.ubTGBonusToTeachOtherMercs, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTGEffectiveSkillValueForTeaching != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[3], gSkillTraitValues.ubTGEffectiveSkillValueForTeaching);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubTGBonusOnPractising != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsTeaching[4], gSkillTraitValues.ubTGBonusOnPractising, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case SCOUTING_NT:
			{
				swprintf( apStr, L"" );
				if( gSkillTraitValues.ubSCSightRangebonusWithScopes != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[0], gSkillTraitValues.ubSCSightRangebonusWithScopes, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.usSCSightRangebonusWithBinoculars != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[1], gSkillTraitValues.usSCSightRangebonusWithBinoculars, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSCTunnelVisionReducedWithBinoculars != 0 )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[2], gSkillTraitValues.ubSCTunnelVisionReducedWithBinoculars, sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.fSCCanDetectEnemyPresenseAround  )
				{
					if( gSkillTraitValues.fSCCanDetermineEnemyNumbersAround )
					{
						swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[3]);
						wcscat( apStr, atStr );
					}
					else
					{
						swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[4]);
						wcscat( apStr, atStr );
					}
				}
				if( gSkillTraitValues.fSCPreventsTheEnemyToAmbushMercs )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[5]);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.fSCPreventsBloodcatsAmbushes )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsScouting[6]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case COVERT_NT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[0]);
				wcscat( apStr, atStr );

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[1]);
				wcscat( apStr, atStr );

				if ( !fExpertLevel )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[2], gSkillTraitValues.sCOCloseDetectionRange);
					wcscat( apStr, atStr );
				}

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[3], gSkillTraitValues.sCOCloseDetectionRangeSoldierCorpse);
				wcscat( apStr, atStr );

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[4], ( gSkillTraitValues.sCOMeleeCTHBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );

				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[5], ( gSkillTraitValues.sCoMeleeInstakillBonus * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				
				INT16 apreduction =  ( gSkillTraitValues.sCODisguiseAPReduction * (fExpertLevel ? 2 : 1));
				swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[6], apreduction, sSpecialCharacters[0]);
				wcscat( apStr, atStr );

				if ( gSkillTraitValues.fCOTurncoats )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsCovertOps[7] );
					wcscat( apStr, atStr );
				}

				break;
			}

			// Flugente: Radio Operator
			case RADIO_OPERATOR_NT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[0]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[1]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[2]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[3]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[4]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsRadioOperator[5]);
				wcscat( apStr, atStr );
				break;
			}
			case SNITCH_NT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[0]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[1]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[2]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[3]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[4]);
				wcscat( apStr, atStr );
				if( gSkillTraitValues.ubSNTPassiveReputationGain )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[5],gSkillTraitValues.ubSNTPassiveReputationGain);
					wcscat( apStr, atStr );
				}
				if( gSkillTraitValues.ubSNTHearingRangeBonus )
				{
					swprintf( atStr, gzIMPMinorTraitsHelpTextsSnitch[6],gSkillTraitValues.ubSNTHearingRangeBonus);
					wcscat( apStr, atStr );
				}
				break;
			}
			case SURVIVAL_NT:
			{
				swprintf( apStr, L"" );

				if ( gSkillTraitValues.ubSVGroupTimeSpentForTravellingFoot != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[0], gSkillTraitValues.ubSVGroupTimeSpentForTravellingFoot, sSpecialCharacters[0] );
					wcscat( apStr, atStr );
				}
				if ( gSkillTraitValues.ubSVGroupTimeSpentForTravellingVehicle != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[1], gSkillTraitValues.ubSVGroupTimeSpentForTravellingVehicle, sSpecialCharacters[0] );
					wcscat( apStr, atStr );
				}
				if ( gSkillTraitValues.ubSVBreathForTravellingReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[2], gSkillTraitValues.ubSVBreathForTravellingReduction, sSpecialCharacters[0] );
					wcscat( apStr, atStr );
				}
				if ( gSkillTraitValues.dSVWeatherPenaltiesReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[3], (UINT8)(100 * gSkillTraitValues.dSVWeatherPenaltiesReduction), sSpecialCharacters[0] );
					wcscat( apStr, atStr );
				}

				if ( gSkillTraitValues.ubSVCamoWornountSpeedReduction != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[4], gSkillTraitValues.ubSVCamoWornountSpeedReduction, sSpecialCharacters[0] );
					wcscat( apStr, atStr );
				}

				if ( gSkillTraitValues.usSVTrackerMaxRange && gSkillTraitValues.usSVTrackerAbility )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[5], (gSkillTraitValues.usSVTrackerAbility * gSkillTraitValues.usSVTrackerMaxRange / 100) );
					wcscat( apStr, atStr );
				}

				if ( gGameExternalOptions.fDisease )
				{
					if ( gSkillTraitValues.usSVDiseaseResistance != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[6], gSkillTraitValues.usSVDiseaseResistance > 0 ? L"+" : L"", gSkillTraitValues.usSVDiseaseResistance );
						wcscat( apStr, atStr );
					}
				}

				if ( UsingFoodSystem() )
				{
					if ( gSkillTraitValues.sSVFoodConsumption != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[7], gSkillTraitValues.sSVFoodConsumption > 0 ? L"+" : L"", gSkillTraitValues.sSVFoodConsumption );
						wcscat( apStr, atStr );
					}

					if ( gSkillTraitValues.sSVDrinkConsumption != 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[8], gSkillTraitValues.sSVDrinkConsumption > 0 ? L"+" : L"", gSkillTraitValues.sSVDrinkConsumption );
						wcscat( apStr, atStr );
					}
				}

				if ( gGameExternalOptions.gfAllowSnakes )
				{
					if ( gSkillTraitValues.usSVSnakeDefense > 0 )
					{
						swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[9], gSkillTraitValues.usSVSnakeDefense );
						wcscat( apStr, atStr );
					}
				}

				if ( gSkillTraitValues.ubSVCamoEffectivenessBonus != 0 )
				{
					swprintf( atStr, gzIMPMajorTraitsHelpTextsSurvival[10], gSkillTraitValues.ubSVCamoEffectivenessBonus );
					wcscat( apStr, atStr );
				}

				break;
			}
			case NO_SKILLTRAIT_NT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsNone[0] );
				wcscat( apStr, atStr );
				break;
			}
		}
	}
	else
	{
		switch( ubTraitNumber )
		{
			case LOCKPICKING_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[LOCKPICKING_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[0], ( gbSkillTraitBonus[LOCKPICKING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case HANDTOHAND_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[HANDTOHAND_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[1], (  gbSkillTraitBonus[HANDTOHAND_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[2], (  3 * gbSkillTraitBonus[HANDTOHAND_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[3], (  gbSkillTraitBonus[HANDTOHAND_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case ELECTRONICS_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[4] );
				wcscat( apStr, atStr );
				break;
			}
			case NIGHTOPS_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[5], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[6], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[7], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[8], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[9], (fExpertLevel ? 2 : 1));
				wcscat( apStr, atStr );
				break;
			}
			case THROWING_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[THROWING_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[10], (  gbSkillTraitBonus[THROWING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[11], (  gbSkillTraitBonus[THROWING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[12], (  10 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case TEACHING_OT:
			{
				swprintf( apStr, L"" );
				if( gGameExternalOptions.ubTeachBonusToTrain != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[13], (  gGameExternalOptions.ubTeachBonusToTrain * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				if( (gGameExternalOptions.usTeacherTraitEffectOnLeadership - 100) > 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[14], (  (gGameExternalOptions.usTeacherTraitEffectOnLeadership - 100) * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case HEAVY_WEAPS_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[HEAVY_WEAPS_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[15], (  gbSkillTraitBonus[HEAVY_WEAPS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case AUTO_WEAPS_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[16], (  2 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[17] );
				wcscat( apStr, atStr );
				break;
			}
			case STEALTHY_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[18], (  25 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				if( gGameExternalOptions.ubStealthTraitCoverValue != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[19], (  gGameExternalOptions.ubStealthTraitCoverValue * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case AMBIDEXT_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[20] );
				wcscat( apStr, atStr );
				break;
			}
			case MARTIALARTS_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[MARTIALARTS_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[28], (  gbSkillTraitBonus[MARTIALARTS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[29], (  gbSkillTraitBonus[MARTIALARTS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[30], (  gbSkillTraitBonus[MARTIALARTS_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[31], (  gbSkillTraitBonus[MARTIALARTS_OT] * 2 / 3 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[32], (  gbSkillTraitBonus[MARTIALARTS_OT] / 2 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[33]);
				wcscat( apStr, atStr );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[34]);
				wcscat( apStr, atStr );
				break;
			}
			case KNIFING_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[KNIFING_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[21], (  gbSkillTraitBonus[KNIFING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[22], (  gbSkillTraitBonus[KNIFING_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[23], (  gbSkillTraitBonus[KNIFING_OT] / 3 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[24], (  gbSkillTraitBonus[KNIFING_OT] / 2 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				break;
			}
			case PROF_SNIPER_OT:
			{
				swprintf( apStr, L"" );
				if( gbSkillTraitBonus[PROF_SNIPER_OT] != 0 )
				{
					swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[25], (  gbSkillTraitBonus[PROF_SNIPER_OT] * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
					wcscat( apStr, atStr );
				}
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[26], (  10 * (fExpertLevel ? 2 : 1)), sSpecialCharacters[0]);
				wcscat( apStr, atStr );
				break;
			}
			case CAMOUFLAGED_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPOldSkillTraitsHelpTexts[27] );
				wcscat( apStr, atStr );
				break;
			}
			case NO_SKILLTRAIT_OT:
			{
				swprintf( apStr, L"" );
				swprintf( atStr, gzIMPMajorTraitsHelpTextsNone[0] );
				wcscat( apStr, atStr );
				break;
			}
		}
	}
}

// SANDRO - Popup text windows for character 
void AssignPersonnelCharacterTraitHelpText( UINT8 ubCharacterNumber )
{
	CHAR16	apStr[ 1000 ];

	swprintf( apStr, L"" );
	swprintf( apStr, gzIMPNewCharacterTraitsHelpTexts[ubCharacterNumber] );

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[5]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[5], MSYS_NO_CALLBACK );
}


// SANDRO - Popup text windows for disability 
void AssignPersonnelDisabilityHelpText( UINT8 ubDisabilityNumber )
{
	CHAR16	apStr[ 500 ];

	swprintf( apStr, L"" );
	swprintf( apStr, gzIMPDisabilitiesHelpTexts[ubDisabilityNumber] );

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[6]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[6], MSYS_NO_CALLBACK );
}

void AssignPersonnelMultipleDisabilityHelpText( SOLDIERTYPE* pSoldier )
{
	CHAR16	apStr[1000];

	int numdisabilities = 0;
	UINT8 disabilityfound = 0;
	swprintf( apStr, L"" );
	for ( UINT8 i = NO_DISABILITY + 1; i < NUM_DISABILITIES; ++i )
	{
		if ( DoesMercHaveDisability( pSoldier, i ) )
		{
			swprintf( apStr, L"%s%s\n", apStr, gzIMPDisabilityTraitText[i] );
			swprintf( apStr, L"%s%s\n\n", apStr, gzIMPDisabilitiesHelpTexts[i] );
		}
	}

	// Set region help text
	SetRegionFastHelpText( &( gSkillTraitHelpTextRegion[6] ), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[6], MSYS_NO_CALLBACK );
}

void AssignPersonnelKillsHelpText( INT32 ubProfile )
{
	CHAR16	apStr[ 1000 ];
	CHAR16	atStr[ 150 ];

	swprintf( apStr, L"" );
	if (gMercProfiles[ubProfile].records.usKillsElites > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 0 ], gMercProfiles[ubProfile].records.usKillsElites );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usKillsRegulars > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 1 ], gMercProfiles[ubProfile].records.usKillsRegulars );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usKillsAdmins > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 2 ], gMercProfiles[ubProfile].records.usKillsAdmins );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usKillsHostiles > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 3 ], gMercProfiles[ubProfile].records.usKillsHostiles );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usKillsCreatures > 0 || fShowRecordsIfZero)
	{
		// WANNE: Only display the monster info, when we play with monsters!
		if (gGameOptions.ubGameStyle == STYLE_SCIFI && gGameExternalOptions.fEnableCrepitus)
		{
			swprintf(atStr, pPersonnelRecordsHelpTexts[ 4 ], gMercProfiles[ubProfile].records.usKillsCreatures );
			wcscat( apStr, atStr );
		}
	}
	if (gMercProfiles[ubProfile].records.usKillsTanks > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 5 ], gMercProfiles[ubProfile].records.usKillsTanks );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usKillsOthers > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 6 ], gMercProfiles[ubProfile].records.usKillsOthers );
		wcscat( apStr, atStr );
	}

	if (gGameSettings.fOptions[TOPTION_ZOMBIES] )
	{
		if (gMercProfiles[ubProfile].records.usKillsZombies > 0 || fShowRecordsIfZero)
		{
			swprintf(atStr, pPersonnelRecordsHelpTexts[ 46 ], gMercProfiles[ubProfile].records.usKillsZombies );
			wcscat( apStr, atStr );
		}
	}

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[7]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[7], MSYS_NO_CALLBACK );
}

void AssignPersonnelAssistsHelpText( INT32 ubProfile )
{
	CHAR16	apStr[ 350 ];
	CHAR16	atStr[ 80 ];

	swprintf( apStr, L"" );
	if (gMercProfiles[ubProfile].records.usAssistsMercs > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 7 ], gMercProfiles[ubProfile].records.usAssistsMercs );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usAssistsMilitia > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 8 ], gMercProfiles[ubProfile].records.usAssistsMilitia );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usAssistsOthers > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 9 ], gMercProfiles[ubProfile].records.usAssistsOthers );
		wcscat( apStr, atStr );
	}

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[8]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[8], MSYS_NO_CALLBACK );
}

void AssignPersonnelHitPercentageHelpText( INT32 ubProfile )
{
	CHAR16	apStr[ 1000 ];
	CHAR16	atStr[ 150 ];

	swprintf( apStr, L"" );
	if (gMercProfiles[ubProfile].records.usShotsFired > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 10 ], gMercProfiles[ubProfile].records.usShotsFired );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usMissilesLaunched > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 11 ], gMercProfiles[ubProfile].records.usMissilesLaunched );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usGrenadesThrown > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 12 ], gMercProfiles[ubProfile].records.usGrenadesThrown );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usKnivesThrown > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 13 ], gMercProfiles[ubProfile].records.usKnivesThrown );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usBladeAttacks > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 14 ], gMercProfiles[ubProfile].records.usBladeAttacks );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usHtHAttacks > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 15 ], gMercProfiles[ubProfile].records.usHtHAttacks );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usShotsHit > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 16 ], gMercProfiles[ubProfile].records.usShotsHit );
		wcscat( apStr, atStr );
	}
	if ( gMercProfiles[ubProfile].records.usDamageDealt > 0 || fShowRecordsIfZero )
	{
		swprintf( atStr, pPersonnelRecordsHelpTexts[52], gMercProfiles[ubProfile].records.usDamageDealt );
		wcscat( apStr, atStr );
	}

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[9]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[9], MSYS_NO_CALLBACK );
}

void AssignPersonnelAchievementsHelpText( INT32 ubProfile )
{
	CHAR16	apStr[ 1000 ];
	CHAR16	atStr[ 80 ];

	swprintf( apStr, L"" );
	if (gMercProfiles[ubProfile].records.usLocksPicked > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 17 ], gMercProfiles[ubProfile].records.usLocksPicked );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usLocksBreached > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 18 ], gMercProfiles[ubProfile].records.usLocksBreached );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usTrapsRemoved > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 19 ], gMercProfiles[ubProfile].records.usTrapsRemoved );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usExpDetonated > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 20 ], gMercProfiles[ubProfile].records.usExpDetonated );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usItemsRepaired > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 21 ], gMercProfiles[ubProfile].records.usItemsRepaired );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usItemsCombined > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 22 ], gMercProfiles[ubProfile].records.usItemsCombined );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usItemsStolen > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 23 ], gMercProfiles[ubProfile].records.usItemsStolen );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usMilitiaTrained > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 24 ], gMercProfiles[ubProfile].records.usMilitiaTrained );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usMercsBandaged > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 25 ], gMercProfiles[ubProfile].records.usMercsBandaged );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usSurgeriesMade > 0 || fShowRecordsIfZero)
	{
		if ( gGameOptions.fNewTraitSystem )
		{
			switch( gSkillTraitValues.ubDONumberTraitsNeededForSurgery )
			{
				case 0: 
					swprintf(atStr, pPersonnelRecordsHelpTexts[ 26 ], gMercProfiles[ubProfile].records.usSurgeriesMade );
					wcscat( apStr, atStr );
					break;
				case 1: 
					if ( ProfileHasSkillTrait( ubProfile, DOCTOR_NT ) > 0 )
					{
						swprintf(atStr, pPersonnelRecordsHelpTexts[ 26 ], gMercProfiles[ubProfile].records.usSurgeriesMade );
						wcscat( apStr, atStr );
					}
					break;
				case 2: 
					if ( ProfileHasSkillTrait( ubProfile, DOCTOR_NT ) > 1 )
					{
						swprintf(atStr, pPersonnelRecordsHelpTexts[ 26 ], gMercProfiles[ubProfile].records.usSurgeriesMade );
						wcscat( apStr, atStr );
					}
					break;
				default:
					break;
			}
		}
	}
	if ( gMercProfiles[ubProfile].records.usPointsHealed /100 > 0 || fShowRecordsIfZero )
	{
		swprintf( atStr, pPersonnelRecordsHelpTexts[53], gMercProfiles[ubProfile].records.usPointsHealed / 100 );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usNPCsDiscovered > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 27 ], gMercProfiles[ubProfile].records.usNPCsDiscovered );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usSectorsDiscovered > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 28 ], gMercProfiles[ubProfile].records.usSectorsDiscovered );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usAmbushesExperienced > 0 || fShowRecordsIfZero)
	{
		if ( gGameOptions.fNewTraitSystem && ( ProfileHasSkillTrait( ubProfile, SCOUTING_NT ) > 0 ) )
		{
			swprintf(atStr, pPersonnelRecordsHelpTexts[ 29 ], gMercProfiles[ubProfile].records.usAmbushesExperienced );
			wcscat( apStr, atStr );
		}
	}
	if (gMercProfiles[ubProfile].records.ubQuestsHandled > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 30 ], gMercProfiles[ubProfile].records.ubQuestsHandled );
		wcscat( apStr, atStr );
	}
	if ( gMercProfiles[ubProfile].records.usInterrogations > 0 || fShowRecordsIfZero )
	{
		swprintf( atStr, pPersonnelRecordsHelpTexts[49], gMercProfiles[ubProfile].records.usInterrogations );
		wcscat( apStr, atStr );
	}

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[10]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[10], MSYS_NO_CALLBACK );
}

void AssignPersonnelBattlesHelpText( INT32 ubProfile )
{
	CHAR16	apStr[ 400 ];
	CHAR16	atStr[ 80 ];

	swprintf( apStr, L"" );
	if (gMercProfiles[ubProfile].records.usBattlesTactical > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 31 ], gMercProfiles[ubProfile].records.usBattlesTactical );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usBattlesAutoresolve > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 32 ], gMercProfiles[ubProfile].records.usBattlesAutoresolve );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usBattlesRetreated > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 33 ], gMercProfiles[ubProfile].records.usBattlesRetreated );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usAmbushesExperienced > 0 || fShowRecordsIfZero)
	{		
		if (!( gGameOptions.fNewTraitSystem && ( ProfileHasSkillTrait( ubProfile, SCOUTING_NT ) > 0 ) ))
		{
			swprintf(atStr, pPersonnelRecordsHelpTexts[ 34 ], gMercProfiles[ubProfile].records.usAmbushesExperienced );
			wcscat( apStr, atStr );
		}
	}
	if (gMercProfiles[ubProfile].records.usLargestBattleFought > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 35 ], gMercProfiles[ubProfile].records.usLargestBattleFought );
		wcscat( apStr, atStr );
	}

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[11]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[11], MSYS_NO_CALLBACK );
	
	return;
}

void AssignPersonnelWoundsHelpText( INT32 ubProfile )
{
	CHAR16	apStr[ 500 ];
	CHAR16	atStr[ 80 ];

	swprintf( apStr, L"" );
	if (gMercProfiles[ubProfile].records.usTimesWoundedShot > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 36 ], gMercProfiles[ubProfile].records.usTimesWoundedShot );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usTimesWoundedStabbed > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 37 ], gMercProfiles[ubProfile].records.usTimesWoundedStabbed );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usTimesWoundedPunched > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 38 ], gMercProfiles[ubProfile].records.usTimesWoundedPunched );
		wcscat( apStr, atStr );
	}
	if (gMercProfiles[ubProfile].records.usTimesWoundedBlasted > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 39 ], gMercProfiles[ubProfile].records.usTimesWoundedBlasted );
		wcscat( apStr, atStr );
	}
	
	if (gMercProfiles[ubProfile].records.usTimesSurgeryUndergoed > 0 || fShowRecordsIfZero)
	{
		if ( gGameOptions.fNewTraitSystem )
		{
			swprintf(atStr, pPersonnelRecordsHelpTexts[ 41 ], gMercProfiles[ubProfile].records.usTimesSurgeryUndergoed );
			wcscat( apStr, atStr );
		}
	}

	if (gMercProfiles[ubProfile].records.usFacilityAccidents > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 42 ], gMercProfiles[ubProfile].records.usFacilityAccidents );
		wcscat( apStr, atStr );
	}

	// WANNE: Moved to the end
	if (gMercProfiles[ubProfile].records.usTimesStatDamaged > 0 || fShowRecordsIfZero)
	{
		swprintf(atStr, pPersonnelRecordsHelpTexts[ 40 ], gMercProfiles[ubProfile].records.usTimesStatDamaged );
		wcscat( apStr, atStr );
	}

	if ( gMercProfiles[ubProfile].records.usTimesInfected > 0 || fShowRecordsIfZero )
	{
		swprintf( atStr, pPersonnelRecordsHelpTexts[50], gMercProfiles[ubProfile].records.usTimesInfected );
		wcscat( apStr, atStr );
	}

	if ( gMercProfiles[ubProfile].records.usDamageTaken > 0 || fShowRecordsIfZero )
	{
		swprintf( atStr, pPersonnelRecordsHelpTexts[51], gMercProfiles[ubProfile].records.usDamageTaken );
		wcscat( apStr, atStr );
	}

	// Set region help text
	SetRegionFastHelpText( &(gSkillTraitHelpTextRegion[12]), apStr );
	SetRegionHelpEndCallback( &gSkillTraitHelpTextRegion[12], MSYS_NO_CALLBACK );
}

INT8 CalculateMercsAchievementPercentage( INT32 ubProfile )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32 cnt=0;
	UINT32 uiMercPoints, uiMercPercentage; 
	unsigned long ulTotalMercPoints = 0;

	// run through active soldiers
	for ( cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++)
	{
		pTeamSoldier = MercPtrs[cnt];
		// Only count stats of merc (not vehicles)
		if ( !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pTeamSoldier ) )
		{
			if( pTeamSoldier->bActive && pTeamSoldier->stats.bLife > 0 && pTeamSoldier->ubProfile != 0 )
			{
				// get total value of all mercs, adjust by importance
				ulTotalMercPoints += 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usLocksPicked )
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usLocksBreached )
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usTrapsRemoved *3/2)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usExpDetonated *3/2)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usItemsRepaired /2)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usItemsCombined *2)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usItemsStolen )
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usMercsBandaged *3/4)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usSurgeriesMade *3/2)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usNPCsDiscovered *4/3)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usSectorsDiscovered )
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.usMilitiaTrained /4)
					+ 
					( gMercProfiles[ pTeamSoldier->ubProfile ].records.ubQuestsHandled *2)
					+
					(gMercProfiles[pTeamSoldier->ubProfile].records.usInterrogations);
			}
		}
	}

	// Now get points of our mercs
	uiMercPoints = 
		( gMercProfiles[ ubProfile ].records.usLocksPicked )
		+ 
		( gMercProfiles[ ubProfile ].records.usLocksBreached )
		+ 
		( gMercProfiles[ ubProfile ].records.usTrapsRemoved *3/2)
		+ 
		( gMercProfiles[ ubProfile ].records.usExpDetonated *3/2)
		+ 
		( gMercProfiles[ ubProfile ].records.usItemsRepaired /2)
		+ 
		( gMercProfiles[ ubProfile ].records.usItemsCombined *2)
		+ 
		( gMercProfiles[ ubProfile ].records.usItemsStolen )
		+ 
		( gMercProfiles[ ubProfile ].records.usMercsBandaged *3/4)
		+ 
		( gMercProfiles[ ubProfile ].records.usSurgeriesMade *3/2)
		+ 
		( gMercProfiles[ ubProfile ].records.usNPCsDiscovered *4/3)
		+ 
		( gMercProfiles[ ubProfile ].records.usSectorsDiscovered )
		+ 
		( gMercProfiles[ ubProfile ].records.usMilitiaTrained /4)
		+ 
		( gMercProfiles[ ubProfile ].records.ubQuestsHandled *2)
		+
		( gMercProfiles[ubProfile].records.usInterrogations );

	// Calculate percentage
	if( ulTotalMercPoints != 0 )
	{
		uiMercPercentage = (UINT32)(100.0f * (FLOAT)uiMercPoints / (FLOAT)ulTotalMercPoints + 0.5f);

		uiMercPercentage = min( 100, uiMercPercentage );

		return( (UINT8)(uiMercPercentage) );
	}
	else
		return( 0 );
}

// Flugente: personality info
void AssignPersonalityHelpText( SOLDIERTYPE* pSoldier, MOUSE_REGION* pMouseregion )
{
	CHAR16	apStr[ 4500 ];
	CHAR16	atStr[  260 ];

	swprintf( apStr, L"" );
	
	if ( pSoldier )
	{
		swprintf(atStr, L"- %s %s %s %s %s\n", szPersonalityDisplayText[0], szAppearanceText[gMercProfiles[ pSoldier->ubProfile ].bAppearance], szPersonalityDisplayText[1], szCareLevelText[gMercProfiles[ pSoldier->ubProfile ].bAppearanceCareLevel], szPersonalityDisplayText[2] );
		wcscat( apStr, atStr );

		swprintf(atStr, L"- %s %s %s %s %s\n", szPersonalityDisplayText[3], szRefinementText[gMercProfiles[ pSoldier->ubProfile ].bRefinement], szPersonalityDisplayText[4], szCareLevelText[gMercProfiles[ pSoldier->ubProfile ].bRefinementCareLevel], szPersonalityDisplayText[5] );
		wcscat( apStr, atStr );

		if ( gMercProfiles[ pSoldier->ubProfile ].bHatedNationality < 0 )
			swprintf(atStr, L"- %s %s %s\n", szPersonalityDisplayText[6], szNationalityText[gMercProfiles[ pSoldier->ubProfile ].bNationality], szNationalityText_Special[0] );
		else
			swprintf(atStr, L"- %s %s %s %s %s.\n", szPersonalityDisplayText[6], szNationalityText[gMercProfiles[ pSoldier->ubProfile ].bNationality], szPersonalityDisplayText[7], szNationalityText[gMercProfiles[ pSoldier->ubProfile ].bHatedNationality], szCareLevelText[gMercProfiles[ pSoldier->ubProfile ].bHatedNationalityCareLevel] );
		wcscat( apStr, atStr );

		swprintf(atStr, L"- %s %s %s-%s %s\n", szPersonalityDisplayText[6], szRacistText[gMercProfiles[ pSoldier->ubProfile ].bRacist], szPersonalityDisplayText[9], szRaceText[gMercProfiles[ pSoldier->ubProfile ].bRace], szPersonalityDisplayText[10] );
		wcscat( apStr, atStr );

		swprintf(atStr, L"- %s %s.\n", szPersonalityDisplayText[6], szSexistText[gMercProfiles[ pSoldier->ubProfile ].bSexist] );
		wcscat( apStr, atStr );
	}

	if ( pMouseregion )
	{
		// Set region help text
		SetRegionFastHelpText( pMouseregion, apStr );
		SetRegionHelpEndCallback( pMouseregion, MSYS_NO_CALLBACK );
	}
}
