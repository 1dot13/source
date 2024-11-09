	#include "sgp.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "Render Dirty.h"
	#include "sysutil.h"
	#include "screens.h"
	#include "cursors.h"
	#include "Event Pump.h"
	#include "laptop.h"
	#include "aim.h"
	#include "AimMembers.h"
	#include "AimFacialIndex.h"
	#include "AimSort.h"
	#include "AimArchives.h"
	#include "AimHistory.h"
	#include "AimLinks.h"
	#include "AimPolicies.h"
	#include "mercs.h"
	#include "mercs Files.h"
	#include "mercs Account.h"
	#include "mercs No Account.h"
	#include "BobbyR.h"
	#include "BobbyRGuns.h"
	#include "BobbyRAmmo.h"
	#include "BobbyRArmour.h"
	#include "BobbyRMisc.h"
	#include "BobbyRUsed.h"
	#include "BobbyRMailOrder.h"
	#include "CharProfile.h"
	#include "florist.h"
	#include "florist Cards.h"
	#include "florist Gallery.h"
	#include "florist Order Form.h"
	#include "insurance.h"
	#include "insurance Contract.h"
	#include "insurance Info.h"
	#include "insurance Comments.h"
	#include "funeral.h"
	#include "sirtech.h"
	#include "finances.h"
	#include "personnel.h"
	#include "history.h"
	#include "files.h"
	#include "email.h"
	#include "Interface Control.h"
	#include "Game Event Hook.h"
	#include "WordWrap.h"
	#include "Game init.h"
	#include "Game Clock.h"
	#include "vobject_blitters.h"
	#include "Soldier Profile.h"
	#include "Overhead.h"
	#include "environment.h"
	#include "LibraryDataBase.h"
	#include "music control.h"
	#include "SaveLoadGame.h"
	#include "LaptopSave.h"
	#include "RenderWorld.h"
	#include "gameloop.h"
	#include "english.h"
	#include "random.h"
	#include "Merc Hiring.h"
	#include "Map Screen Interface.h"
	#include "ambient control.h"
	#include "Sound Control.h"
	#include "text.h"
	#include "Message.h"
	#include "Map Screen Interface Bottom.h"
	#include "Cursor Control.h"
	#include "Quests.h"
	#include "Multi Language Graphic Utils.h"
	#include "BrokenLink.h"
	#include "BobbyRShipments.h"
	#include "Dialogue Control.h"
	#include "helpscreen.h"
	#include "cheats.h"
	#include "Strategic Status.h"
	#include "Arms Dealer Init.h"
	#include "GameSettings.h"
	#include "Encyclopedia_new.h"
	#include "Encyclopedia_Data_new.h"
	#include "CampaignHistoryMain.h"		// added by Flugente
	#include "CampaignHistory_Summary.h"	// added by Flugente
	#include "MercCompare.h"				// added by Flugente
	#include "WHO.h"						// added by Flugente
	#include "PMC.h"						// added by Flugente
	#include "MilitiaWebsite.h"				// added by Flugente
	#include "Intelmarket.h"				// added by Flugente
	#include "FacilityProduction.h"			// added by Flugente
	#include "Rebel Command.h"

#include "connect.h"
#include "BriefingRoom_Data.h"
#include "BriefingRoom.h"
#include "BriefingRoomM.h"


#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "End Game.h"
#include "ub_config.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

#ifdef JA2UB
BOOLEAN gfProcessCustomMaps  = FALSE; //ja25 UB
#endif

// icons text id's
enum{
	MAIL=0,
	WWW,
	FINANCIAL,
	PERSONNEL,
	HISTORY,
	FILES,
	MAX_ICON_COUNT,
};

enum{

};
enum{
	NO_REGION=0,
	EMAIL_REGION,
	WWW_REGION,
	FINANCIAL_REGION,
	PERSONNEL_REGION,
	HISTORY_REGION,
	FILES_REGION,
};





struct rgbcolor
{
	UINT8 ubRed;
	UINT8 ubGreen;
	UINT8 ubBlue;
};

typedef struct rgbcolor RGBCOLOR;

RGBCOLOR GlowColors[]={
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


// laptop programs
enum{
	LAPTOP_PROGRAM_MAILER,
	LAPTOP_PROGRAM_WEB_BROWSER,
	LAPTOP_PROGRAM_FILES,
	LAPTOP_PROGRAM_PERSONNEL,
	LAPTOP_PROGRAM_FINANCES,
	LAPTOP_PROGRAM_HISTORY,

};

// laptop program states
enum{
	LAPTOP_PROGRAM_MINIMIZED,
	LAPTOP_PROGRAM_OPEN,
};
#define LAPTOP_ICONS_X										iScreenWidthOffset + 33
#define LAPTOP_ICONS_MAIL_Y									iScreenHeightOffset + 30
#define LAPTOP_ICONS_WWW_Y									iScreenHeightOffset + 87
#define LAPTOP_ICONS_FINANCIAL_Y							iScreenHeightOffset + 157
#define LAPTOP_ICONS_PERSONNEL_Y							iScreenHeightOffset + 238
#define LAPTOP_ICONS_HISTORY_Y								iScreenHeightOffset + 305
#define LAPTOP_ICONS_FILES_Y								iScreenHeightOffset + 355
#define LAPTOP_ICON_TEXT_X									iScreenWidthOffset + 24

#define LAPTOP_ICON_TEXT_WIDTH								79
#define LAPTOP_ICON_TEXT_HEIGHT								6

#define LAPTOP_ICON_TEXT_MAIL_Y								iScreenHeightOffset + 77
#define LAPTOP_ICON_TEXT_WWW_Y								iScreenHeightOffset + 142
#define LAPTOP_ICON_TEXT_FINANCIAL_Y						iScreenHeightOffset + 214
#define LAPTOP_ICON_TEXT_PERSONNEL_Y						iScreenHeightOffset + 296
#define LAPTOP_ICON_TEXT_HISTORY_Y							iScreenHeightOffset + 356
#define LAPTOP_ICON_TEXT_FILES_Y							iScreenHeightOffset + 415

#define LAPTOPICONFONT FONT10ARIAL
#define BOOK_FONT FONT10ARIAL
#define DOWNLOAD_FONT FONT12ARIAL
#define ERROR_TITLE_FONT	FONT14ARIAL
#define ERROR_FONT	FONT12ARIAL


#define HISTORY_ICON_OFFSET_X								0
#define FILES_ICON_OFFSET_X									3
#define FINANCIAL_ICON_OFFSET_X								0
#define LAPTOP_ICON_WIDTH									80
#define MAX_BUTTON_COUNT									1
#define ON_BUTTON											0
#define GLOW_DELAY											70
#define WWW_COUNT											6
#define ICON_INTERVAL										150
#define BOOK_X												iScreenWidthOffset + 111
#define BOOK_TOP_Y											iScreenHeightOffset + 79
#define BOOK_HEIGHT											12
#define DOWN_HEIGHT											19
#define BOOK_WIDTH											100
#define SCROLL_MIN											-100
#define SCROLL_DIFFERENCE									10

#define LONG_UNIT_TIME										120
#define UNIT_TIME											40
#define LOAD_TIME											UNIT_TIME * 30
#define FAST_UNIT_TIME										3
#define FASTEST_UNIT_TIME									2
#define ALMOST_FAST_UNIT_TIME								25
#define ALMOST_FAST_LOAD_TIME								ALMOST_FAST_UNIT_TIME * 30
#define FAST_LOAD_TIME										FAST_UNIT_TIME * 30
#define LONG_LOAD_TIME										LONG_UNIT_TIME * 30
#define FASTEST_LOAD_TIME									FASTEST_UNIT_TIME * 30
#define DOWNLOAD_X											iScreenWidthOffset + 300
#define DOWNLOAD_Y											iScreenHeightOffset +	200
#define LAPTOP_WINDOW_X										DOWNLOAD_X + 12
#define LAPTOP_WINDOW_Y										DOWNLOAD_Y + 25
#define LAPTOP_BAR_Y										LAPTOP_WINDOW_Y + 2
#define LAPTOP_BAR_X										LAPTOP_WINDOW_X + 1
#define UNIT_WIDTH											4
#define LAPTOP_WINDOW_WIDTH									150
#define LAPTOP_WINDOW_HEIGHT								50
#define DOWN_STRING_X										DOWNLOAD_X + 47
#define DOWN_STRING_Y										DOWNLOAD_Y + 5
#define ERROR_X												iScreenWidthOffset + 300
#define ERROR_Y												iScreenHeightOffset + 200
#define ERROR_BTN_X											94
#define ERROR_BTN_Y											88
#define ERROR_TITLE_X										3
#define ERROR_TITLE_Y										1
#define ERROR_BTN_TEXT_X									15
#define ERROR_BTN_TEXT_Y									9
#define ERROR_TEXT_X										20
#define ERROR_TEXT_Y										15
#define LAPTOP_TITLE_ICONS_X								iScreenWidthOffset + 113
#define LAPTOP_TITLE_ICONS_Y								iScreenHeightOffset + 27

// HD flicker times
#define HD_FLICKER_TIME										3000
#define FLICKER_TIME										50


#define	NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS				9 // 18
#define	LAPTOP_TITLE_BAR_WIDTH								500
#define	LAPTOP_TITLE_BAR_HEIGHT								24

#define	LAPTOP_TITLE_BAR_TOP_LEFT_X							iScreenWidthOffset + 111
#define	LAPTOP_TITLE_BAR_TOP_LEFT_Y							iScreenHeightOffset + 25
#define	LAPTOP_TITLE_BAR_TOP_RIGHT_X						iScreenWidthOffset + 610
#define	LAPTOP_TITLE_BAR_TOP_RIGHT_Y						LAPTOP_TITLE_BAR_TOP_LEFT_Y

#define	LAPTOP_TITLE_BAR_ICON_OFFSET_X						5
#define	LAPTOP_TITLE_BAR_ICON_OFFSET_Y						2
#define	LAPTOP_TITLE_BAR_TEXT_OFFSET_X						29
#define	LAPTOP_TITLE_BAR_TEXT_OFFSET_Y						8

#define	LAPTOP_PROGRAM_ICON_X								LAPTOP_TITLE_BAR_TOP_LEFT_X
#define	LAPTOP_PROGRAM_ICON_Y								LAPTOP_TITLE_BAR_TOP_LEFT_Y
#define	LAPTOP_PROGRAM_ICON_WIDTH							20
#define	LAPTOP_PROGRAM_ICON_HEIGHT							20
#define	DISPLAY_TIME_FOR_WEB_BOOKMARK_NOTIFY				2000

// the wait time for closing of laptop animation/delay
#define EXIT_LAPTOP_DELAY_TIME								100

UINT32		guiTitleBarSurface;
BOOLEAN		gfTitleBarSurfaceAlreadyActive=FALSE;
extern BOOLEAN fFirstTimeInGameScreen;

#define		LAPTOP__NEW_FILE_ICON_X							iScreenWidthOffset + 83
#define		LAPTOP__NEW_FILE_ICON_Y							iScreenHeightOffset + 412

#define		LAPTOP__NEW_EMAIL_ICON_X						iScreenWidthOffset + 67
#define		LAPTOP__NEW_EMAIL_ICON_Y						LAPTOP__NEW_FILE_ICON_Y

INT32 guiCurrentGunFilterMode;
INT32 guiPrevGunFilterMode;

INT32 guiCurrentAmmoFilterMode;
INT32 guiPrevAmmoFilterMode;

INT32 guiCurrentUsedFilterMode;
INT32 guiPrevUsedFilterMode;

INT32 guiCurrentArmourFilterMode;
INT32 guiPrevArmourFilterMode;

INT32 guiCurrentMiscFilterMode;
INT32 guiPrevMiscFilterMode;
INT32 guiCurrentMiscSubFilterMode; // Madd: new BR filter options
INT32 guiPrevMiscSubFilterMode;

BOOLEAN	gbMessageDisplayed;


// Mode values
UINT32 guiCurrentLaptopMode;
UINT32 guiPreviousLaptopMode;
UINT32 guiCurrentWWWMode = LAPTOP_MODE_NONE;
INT32	giCurrentSubPage;
UINT32 guiCurrentLapTopCursor;
UINT32 guiPreviousLapTopCursor;
UINT32 guiCurrentSidePanel; // the current navagation panel on the leftside of the laptop screen
UINT32 guiPreviousSidePanel;

extern UINT32 guiVObjectSize;
extern UINT32 guiVSurfaceSize;


INT32 iHighLightBookLine=-1;
BOOLEAN fFastLoadFlag = FALSE;
BOOLEAN gfSideBarFlag;
BOOLEAN gfEnterLapTop=TRUE;
BOOLEAN gfShowBookmarks=FALSE;

// in progress of loading a page?
BOOLEAN fLoadPendingFlag=FALSE;
BOOLEAN fErrorFlag=FALSE;

// mark buttons dirty?
BOOLEAN fMarkButtonsDirtyFlag = TRUE;

// redraw afer rendering buttons?
BOOLEAN fReDrawPostButtonRender = FALSE;

// intermediate refresh flag
BOOLEAN fIntermediateReDrawFlag = FALSE;

// in laptop right now?
BOOLEAN fCurrentlyInLaptop = FALSE;

// exit due to a message box pop up?..don't really leave LAPTOP
BOOLEAN fExitDueToMessageBox = FALSE;

// have we visited IMP yety?
BOOLEAN fNotVistedImpYet = TRUE;

// exit laptop during a load?
BOOLEAN fExitDuringLoad = FALSE;

// done loading?
BOOLEAN fDoneLoadPending = FALSE;

// re connecting to a web page?
BOOLEAN fReConnectingFlag = FALSE;

// going a subpage of a web page?..faster access time
BOOLEAN fConnectingToSubPage = FALSE;

// is this our first time in laptop?
BOOLEAN fFirstTimeInLaptop = TRUE;

// redraw the book mark info panel .. for blitting on top of animations
BOOLEAN fReDrawBookMarkInfo = FALSE;

// show the 2 second info about bookmarks being accessed by clicking on web
BOOLEAN fShowBookmarkInfo = FALSE;

// show start button for ATM panel?
extern BOOLEAN fShowAtmPanelStartButton;


//TEMP!	Disables the loadpending delay when switching b/n www pages
BOOLEAN	gfTemporaryDisablingOfLoadPendingFlag=FALSE;


//GLOBAL FOR WHICH SCREEN TO EXIT TO FOR LAPTOP
UINT32		guiExitScreen = MAP_SCREEN;

MOUSE_REGION gLaptopRegion;
// Laptop screen graphic handle
UINT32	guiLAPTOP;
BOOLEAN fNewWWWDisplay=TRUE;

static BOOLEAN fNewWWW=TRUE;

//Used to store the site to go to after the 'rain delay' message
extern	UINT32										guiRainLoop;

INT32	giRainDelayInternetSite=-1;



// have we visitied this site already?
//BOOLEAN fVisitedBookmarkAlready[20];


// the laptop icons
UINT32 guiFILESICON;
UINT32 guiFINANCIALICON;
UINT32 guiHISTORYICON;
UINT32 guiMAILICON;
UINT32 guiPERSICON;
UINT32 guiWWWICON;
UINT32 guiBOOKTOP;
UINT32 guiBOOKHIGH;
UINT32 guiBOOKMID;
UINT32 guiBOOKBOT;
UINT32 guiBOOKMARK;
UINT32 guiGRAPHWINDOW;
UINT32 guiGRAPHBAR;
UINT32 guiLaptopBACKGROUND;
UINT32 guiDOWNLOADTOP;
UINT32 guiDOWNLOADMID;
UINT32 guiDOWNLOADBOT;
UINT32 guiTITLEBARLAPTOP;
UINT32 guiLIGHTS;
UINT32 guiTITLEBARICONS;
UINT32 guiDESKTOP;

// email notification
UINT32 guiUNREAD;
UINT32 guiNEWMAIL;

//laptop button
UINT32 guiLAPTOPBUTTON;
// the sidepanel handle
UINT32	guiLAPTOPSIDEPANEL;

//BOOLEAN		gfNewGameLaptop = TRUE;

// enter new laptop mode due to sliding bars
BOOLEAN fEnteredNewLapTopDueToHandleSlidingBars = FALSE;


// laptop pop up messages index value
INT32 iLaptopMessageBox = -1;

// whether or not we are initing the slide in title bar
BOOLEAN fInitTitle = TRUE;

// tab handled
BOOLEAN fTabHandled = FALSE;

// are we maxing or mining?
BOOLEAN fForward = TRUE;

// BUTTON IMAGES
INT32 giLapTopButton[MAX_BUTTON_COUNT];
INT32 giLapTopButtonImage[MAX_BUTTON_COUNT];
INT32 giErrorButton[1];
INT32 giErrorButtonImage[1];

INT32 gLaptopButton[ 7 ];
INT32 gLaptopButtonImage[ 7 ];

// minimize button
INT32 gLaptopMinButton[ 1 ];
INT32 gLaptopMinButtonImage[ 1 ];



INT32 gLaptopProgramStates[ LAPTOP_PROGRAM_HISTORY + 1 ];

// process of mazimizing
BOOLEAN fMaximizingProgram = FALSE;

// program we are maximizing
INT8 bProgramBeingMaximized = -1;

// are we minimizing
BOOLEAN fMinizingProgram = FALSE;


// process openned queue
INT32	gLaptopProgramQueueList[ 6 ];



// state of createion of minimize button
BOOLEAN fCreateMinimizeButton = FALSE;

BOOLEAN fExitingLaptopFlag = FALSE;

// HD and power lights on
BOOLEAN fPowerLightOn = TRUE;
BOOLEAN fHardDriveLightOn = FALSE;

// HD flicker
BOOLEAN fFlickerHD = FALSE;

// the screens limiting rect
SGPRect LaptopScreenRect={ LAPTOP_UL_X,LAPTOP_UL_Y - 5,LAPTOP_SCREEN_LR_X + 2,LAPTOP_SCREEN_LR_Y + 5 + 19};

// the sub pages visited or not status within the web browser
BOOLEAN gfWWWaitSubSitesVisitedFlags[LAPTOP_MODE_MAX];

// mouse regions
MOUSE_REGION gEmailRegion;
MOUSE_REGION gWWWRegion;
MOUSE_REGION gFinancialRegion;
MOUSE_REGION gPersonnelRegion;
MOUSE_REGION gHistoryRegion;
MOUSE_REGION gFilesRegion;
MOUSE_REGION gLapTopScreenRegion;
MOUSE_REGION gBookmarkMouseRegions[MAX_BOOKMARKS];
MOUSE_REGION pScreenMask;
MOUSE_REGION gLapTopProgramMinIcon;
MOUSE_REGION gNewMailIconRegion;
MOUSE_REGION gNewFileIconRegion;

// highlighted mouse region
INT32 giHighLightRegion=NO_REGION;


// highlighted regions
INT32 giCurrentRegion=NO_REGION;
INT32 giOldRegion=NO_REGION;


//used for global variables that need to be saved
LaptopSaveInfoStruct LaptopSaveInfo;


// function calls
UINT32 RenderLaptopPanel();
void RenderLapTopImage();
void GetLaptopKeyboardInput();
UINT32 ExitLaptopMode(UINT32 uiMode);

UINT32 DrawLapTopText();
void	BtnOnCallback(GUI_BUTTON *btn,INT32 reason);
UINT32 CreateLaptopButtons();
void DeleteLapTopButtons();
BOOLEAN DeleteLapTopMouseRegions();
BOOLEAN CreateLapTopMouseRegions();
BOOLEAN fReDrawScreenFlag=FALSE;
BOOLEAN fPausedReDrawScreenFlag=FALSE;		//used in the handler functions to redraw the screen, after the current frame
void HandleLapTopScreenMouseUi();
void DrawHighLightRegionBox();
void HandleLapTopCursorUpDate();
void PrintBalance( void );


// callbacks
void FinancialRegionButtonCallback(GUI_BUTTON *btn,INT32 reason);
void PersonnelRegionButtonCallback(GUI_BUTTON *btn,INT32 reason );
void WWWRegionButtonCallback(GUI_BUTTON *btn,INT32 reason);
void EmailRegionButtonCallback(GUI_BUTTON *btn,INT32 reason );
void FilesRegionButtonCallback(GUI_BUTTON *btn,INT32 reason);
void HistoryRegionButtonCallback(GUI_BUTTON *btn,INT32 reason);
void LaptopProgramIconMinimizeCallback( MOUSE_REGION * pRegion, INT32 iReason );



void WWWRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void EmailRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void RestoreOldRegion(INT32 iOldRegion);
void HighLightRegion(INT32 iCurrentRegion);
void FinancialRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void HistoryRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void FilesRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void PersonnelRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );
void ScreenRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason );


// minimize callback
void LaptopMinimizeProgramButtonCallback(GUI_BUTTON *btn,INT32 reason);


void NewFileIconCallback( MOUSE_REGION * pRegion, INT32 iReason );


void DisplayBookMarks();
void InitBookMarkList();
BOOLEAN LoadBookmark();
void DeleteBookmark();
void ScrollDisplayText(INT32 iY);
void BookmarkCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void CreateBookMarkMouseRegions();
void BookmarkMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void DeleteBookmarkRegions();
void DeleteLoadPending( void );
BOOLEAN LoadLoadPending( void );
BOOLEAN DisplayLoadPending( void );
void DisplayErrorBox( void );
void CreateDestroyErrorButton( void );
void DrawTextOnErrorButton();
BOOLEAN LeaveLapTopScreen( );
void HandleAnimatedButtons( void );
void AnimateButton( UINT32 uiIconID, UINT16 usX, UINT16 usY );
void CreateDestoryBookMarkRegions( void );
void EnterLaptopInitLaptopPages();
void CheckMarkButtonsDirtyFlag( void );
void PostButtonRendering( void );
void ShouldNewMailBeDisplayed( void );
void DisplayPlayersBalanceToDate( void );
void CheckIfNewWWWW( void );
void HandleLapTopESCKey( void );
void HandleLapTopEnterKey( void );
BOOLEAN InitTitleBarMaximizeGraphics( UINT32 uiBackgroundGraphic, STR16 pTitle, UINT32 uiIconGraphic, UINT16 usIconGraphicIndex );
void RemoveTitleBarMaximizeGraphics();
BOOLEAN DisplayTitleBarMaximizeGraphic(BOOLEAN fForward, BOOLEAN fInit, UINT16 usTopLeftX, UINT16 usTopLeftY, UINT16 usTopRightX );
void HandleSlidingTitleBar( void );
void ShowLights( void );
void FlickerHDLight( void );
BOOLEAN ExitLaptopDone( void );
void CreateDestroyMinimizeButtonForCurrentMode( void );
void CreateMinimizeButtonForCurrentMode( void );
void DestroyMinimizeButtonForCurrentMode( void );
void InitLaptopOpenQueue( void );
void UpdateListToReflectNewProgramOpened( INT32 iOpenedProgram );
INT32 FindLastProgramStillOpen( void );
void SetCurrentToLastProgramOpened( void );
BOOLEAN HandleExit( void );
void DeleteDesktopBackground( void );
BOOLEAN LoadDesktopBackground( void );
BOOLEAN DrawDeskTopBackground( void );
void PrintDate( void );
void DisplayTaskBarIcons();
void PrintNumberOnTeam( void );
void HandleDefaultWebpageForLaptop( void );
void CreateMinimizeRegionsForLaptopProgramIcons( void );
void DestroyMinimizeRegionsForLaptopProgramIcons( void );
void CreateDestroyMouseRegionForNewMailIcon( void );
void NewEmailIconCallback( MOUSE_REGION * pRegion, INT32 iReason );
void HandleWWWSubSites( void );
void UpdateStatusOfDisplayingBookMarks( void );
void InitalizeSubSitesList( void );
void SetSubSiteAsVisted( void );
void HandleAltTabKeyInLaptop( void );
void HandleShiftAltTabKeyInLaptop( void );

// display bookmark notify
void DisplayWebBookMarkNotify( void );

// handle timer for bookmark notify
void HandleWebBookMarkNotifyTimer( void );

void CreateBookMarkHelpText( MOUSE_REGION *pRegion, UINT32 uiBookMarkID );

void CreateFileAndNewEmailIconFastHelpText( UINT32 uiHelpTextID, BOOLEAN fClearHelpText );
void CreateLaptopButtonHelpText( INT32 iButtonIndex, UINT32 uiButtonHelpTextID );
//ppp

extern void ClearHistoryList( void );


//TEMP CHEAT
#ifdef JA2TESTVERSION
extern	void CheatToGetAll5Merc();
#endif
#if defined ( JA2TESTVERSION )
extern	void DemoHiringOfMercs( );
#endif

#ifdef JA2UB
//JA25 UB
#define		LAPTOP__HAVENT_CREATED_IMP_REMINDER_EMAIL_ARRIVE_TIME				( (8 + Random(4) ) * 60 )
#endif

void	SetLaptopExitScreen( UINT32 uiExitScreen )
{
	guiExitScreen = uiExitScreen;
}

void	SetLaptopNewGameFlag( )
{
	LaptopSaveInfo.gfNewGameLaptop = TRUE;
}


void HandleLapTopCursorUpDate()
{
	if(guiPreviousLapTopCursor==guiCurrentLapTopCursor)
	return;
	switch(guiCurrentLapTopCursor)
	{
		case LAPTOP_PANEL_CURSOR:
			MSYS_SetCurrentCursor(CURSOR_NORMAL);
		break;
		case LAPTOP_SCREEN_CURSOR:
			MSYS_SetCurrentCursor(CURSOR_LAPTOP_SCREEN);
		break;
		case LAPTOP_WWW_CURSOR:
			MSYS_SetCurrentCursor(CURSOR_WWW);
		break;
	}
	guiPreviousLapTopCursor=guiCurrentLapTopCursor;

}
void GetLaptopKeyboardInput()
{
	InputAtom					InputEvent;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	fTabHandled = FALSE;

	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
	}
}

//This is called only once at game initialization.
UINT32 LaptopScreenInit()
{
	//Memset the whole structure, to make sure of no 'JUNK'
	memset( &LaptopSaveInfo, 0, sizeof( LaptopSaveInfoStruct ) );

	LaptopSaveInfo.gfNewGameLaptop = TRUE;

	InitializeNumDaysMercArrive();

	//reset the id of the last hired merc
	LaptopSaveInfo.sLastHiredMerc.iIdOfMerc = -1;

	//reset the flag that enables the 'just hired merc' popup
	LaptopSaveInfo.sLastHiredMerc.fHaveDisplayedPopUpInLaptop = FALSE;

#ifdef JA2UB	
	//JA25 UB
	//Set the internet as WORKING
	if ( gGameUBOptions.LaptopQuestEnabled == TRUE )
	gubQuest[ QUEST_FIX_LAPTOP ] = QUESTNOTSTARTED;
#endif

	//Initialize all vars
	guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
	guiPreviousLaptopMode = LAPTOP_MODE_NONE;
	guiCurrentWWWMode = LAPTOP_MODE_NONE;
	guiCurrentSidePanel = FIRST_SIDE_PANEL;
	guiPreviousSidePanel = FIRST_SIDE_PANEL;


	gfSideBarFlag=FALSE;
	gfShowBookmarks=FALSE;
	InitBookMarkList();
	GameInitAIM();
	GameInitAIMMembers();
	GameInitAimFacialIndex();
	GameInitAimSort();
	GameInitAimArchives();
	GameInitAimPolicies();
	GameInitAimLinks();
	GameInitAimHistory();
	GameInitMercs();
	GameInitBobbyR();
	GameInitBobbyRAmmo();
	GameInitBobbyRArmour();
	GameInitBobbyRGuns();
	GameInitBobbyRMailOrder();
	GameInitBobbyRMisc();
	GameInitBobbyRUsed();
	GameInitEmail();
	GameInitCharProfile();
	GameInitFlorist();
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
	{
	GameInitInsurance();
	GameInitInsuranceContract();
	}
//JA25:	
#else
	GameInitInsurance();
	GameInitInsuranceContract();
#endif
	GameInitFuneral();
	GameInitSirTech();
	GameInitFiles();
	GameInitPersonnel();
	
#ifdef ENCYCLOPEDIA_WORKS
	//legion
/*	GameInitEncyclopedia();
	GameInitEncyclopediaLocation();*/
	GameInitEncyclopedia_NEW();
	GameInitEncyclopediaData_NEW();
#endif

	GameInitBriefingRoom();
	GameInitBriefingRoomEnter();

	// Flugente: campaign history
	GameInitCampaignHistory();

	// init program states
	memset( &gLaptopProgramStates, LAPTOP_PROGRAM_MINIMIZED, sizeof( gLaptopProgramStates ) );

	gfAtLeastOneMercWasHired = FALSE;

	//No longer inits the laptop screens, now InitLaptopAndLaptopScreens() does
#ifdef JA2UB	
	InitJa25SaveStruct();
#endif
	return( 1 );
}


BOOLEAN InitLaptopAndLaptopScreens()
{

	GameInitFinances();
	GameInitHistory();

	//Reset the flag so we can create a new IMP character
	LaptopSaveInfo.fIMPCompletedFlag = FALSE;

	//Reset the flag so that BOBBYR's isnt available at the begining of the game
	#ifdef JA2UB
	if ( gGameUBOptions.fBobbyRSite == TRUE )
	LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;
	else
	#endif
	LaptopSaveInfo.fBobbyRSiteCanBeAccessed = FALSE;

	//Reset Kulbas' saving date and possible missed flights
	LaptopSaveInfo.bJohnEscorted = FALSE;
	LaptopSaveInfo.uiJohnEscortedDate = 0;
	LaptopSaveInfo.ubJohnPossibleMissedFlights = 3;

	return( TRUE );
}


UINT32
DrawLapTopIcons()
{

	return (TRUE);
}

UINT32
DrawLapTopText()
{

	// show balance
	DisplayPlayersBalanceToDate( );


	return (TRUE);
}

//This is only called once at game shutdown.
UINT32 LaptopScreenShutdown()
{
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		InsuranceContractEndGameShutDown();
//JA25:	
#else
	InsuranceContractEndGameShutDown();
#endif
	BobbyRayMailOrderEndGameShutDown();
	ShutDownEmailList();

	ClearHistoryList( );

	return TRUE;
}

INT32 EnterLaptop()
{
	//Create, load, initialize data -- just entered the laptop.

	VOBJECT_DESC	VObjectDesc;

	static BOOLEAN fEnteredFromGameStartup = TRUE;
	// we are re entering due to message box, leave NOW!
	if( fExitDueToMessageBox	== TRUE )
	{

		return ( TRUE );
	}

	//if the radar map mouse region is still active, disable it.
	if( gRadarRegion.uiFlags & MSYS_REGION_ENABLED )
	{
		MSYS_DisableRegion( &gRadarRegion );
/*
		#ifdef JA2BETAVERSION
			DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, L"Mapscreen's radar region is still active, please tell Dave how you entered Laptop.", LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL );
		#endif
*/
	}

	gfDontStartTransitionFromLaptop = FALSE;

	//Since we are coming in from MapScreen, uncheck the flag
	guiTacticalInterfaceFlags &= ~INTERFACE_MAPSCREEN;

	// ATE: Disable messages....
	DisableScrollMessages( );

	// Stop any person from saying anything
	StopAnyCurrentlyTalkingSpeech( );

	// Don't play music....
	SetMusicMode( MUSIC_LAPTOP );

	// Stop ambients...
	StopAmbients( );

	// WANNE: disabled rain sound when laptop is displayed
	////if its raining, start the rain showers
	//if( IsItRaining() )
	//{
	//	//Enable the rain delay warning
	//	giRainDelayInternetSite = -1;

	//	//lower the volume
	//	guiRainLoop	= PlayJA2Ambient( RAIN_1, LOWVOLUME, 0 );
	//}


	//open the laptop library
//	OpenLibrary( LIBRARY_LAPTOP );

	//pause the game because we dont want time to advance in the laptop
	PauseGame();

	// set the fact we are currently in laptop, for rendering purposes
	fCurrentlyInLaptop = TRUE;



	// clear guiSAVEBUFFER
	//ColorFillVideoSurfaceArea(guiSAVEBUFFER,	0, 0, 640, 480, Get16BPPColor(FROMRGB(0, 0, 0)) );
	// disable characters panel buttons

	// reset redraw flag and redraw new mail
	fReDrawScreenFlag = FALSE;
	fReDrawNewMailFlag = TRUE;

	// setup basic cursors
	guiCurrentLapTopCursor=LAPTOP_PANEL_CURSOR;
	guiPreviousLapTopCursor=LAPTOP_NO_CURSOR;

	// sub page
	giCurrentSubPage = 0;
	giCurrentRegion=EMAIL_REGION;

	// load the laptop graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\laptop3.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiLAPTOP));

	// background for panel
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\taskbar.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiLaptopBACKGROUND));


	// background for panel
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\programtitlebar.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiTITLEBARLAPTOP));

	// lights for power and HD
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\lights.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiLIGHTS));

	// icons for title bars
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\ICONS.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiTITLEBARICONS));

	// load, blt and delete graphics
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "LAPTOP\\NewMailWarning.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiEmailWarning ) );
	// load background
	LoadDesktopBackground( );


	guiCurrentLaptopMode = LAPTOP_MODE_NONE;
	//MSYS_SetCurrentCursor(CURSOR_NORMAL);

	guiCurrentLaptopMode = LAPTOP_MODE_NONE;
	guiPreviousLaptopMode = LAPTOP_MODE_NONE;
	guiCurrentWWWMode = LAPTOP_MODE_NONE;
	guiCurrentSidePanel = FIRST_SIDE_PANEL;
	guiPreviousSidePanel = FIRST_SIDE_PANEL;
	gfSideBarFlag=FALSE;
	CreateLapTopMouseRegions();
	RenderLapTopImage();
	HighLightRegion(giCurrentRegion);
	//AddEmailMessage(L"Entered LapTop",L"Entered", 0, 0);
	//for(iCounter=0; iCounter <10; iCounter++)
	//{
	//AddEmail(3,5,0,0);
	//}
	// the laptop mouse region


	// reset bookmarks flags
	fFirstTimeInLaptop = TRUE;

	// reset all bookmark visits
	memset( &LaptopSaveInfo.fVisitedBookmarkAlready, 0, sizeof( LaptopSaveInfo.fVisitedBookmarkAlready ) );

	// init program states
	memset( &gLaptopProgramStates, LAPTOP_PROGRAM_MINIMIZED, sizeof( gLaptopProgramStates ) );

	// turn the power on
	fPowerLightOn = TRUE;

	// we are not exiting laptop right now, we just got here
	fExitingLaptopFlag = FALSE;

	// reset program we are maximizing
	bProgramBeingMaximized = -1;

	// reset fact we are maximizing/ mining
	fMaximizingProgram = FALSE;
	fMinizingProgram = FALSE;

	// initialize open queue
	InitLaptopOpenQueue( );

	gfShowBookmarks=FALSE;
	LoadBookmark( );
	
#ifdef JA2UB		
	//JA25 UB
	SetBookMark(MERC_BOOKMARK);
#endif

	if ( !is_networked )
	{
		SetBookMark( AIM_BOOKMARK );

#ifdef ENCYCLOPEDIA_WORKS
		if ( gGameExternalOptions.gEncyclopedia )
			SetBookMark( ENCYCLOPEDIA_BOOKMARK );
#endif

		if ( gGameExternalOptions.gBriefingRoom )
			SetBookMark( BRIEFING_ROOM_BOOKMARK );

		if ( gGameExternalOptions.fCampaignHistoryWebSite )
			SetBookMark( CAMPAIGNHISTORY_BOOKMARK );
		else
			RemoveBookmark( CAMPAIGNHISTORY_BOOKMARK );

		if ( gGameExternalOptions.fDynamicOpinions )
			SetBookMark( MERCCOMPARE_BOOKMARK );
		else
			RemoveBookmark( MERCCOMPARE_BOOKMARK );

		if ( gGameExternalOptions.fDisease && gGameExternalOptions.fDiseaseStrategic )
			SetBookMark( WHO_BOOKMARK );
		else
			RemoveBookmark( WHO_BOOKMARK );

		if ( !gGameExternalOptions.fIntelResource )
			RemoveBookmark( INTELMARKET_BOOKMARK );

		if ( gGameExternalOptions.fFactories )
			SetBookMark( PRODUCTION_BOOKMARK );
		else
			RemoveBookmark( PRODUCTION_BOOKMARK );

		if (gGameExternalOptions.fRebelCommandEnabled && gubQuest[QUEST_FOOD_ROUTE] == QUESTDONE)
			SetBookMark( REBELCOMMAND_BOOKMARK );
		else
			RemoveBookmark( REBELCOMMAND_BOOKMARK );
	}

	LoadLoadPending( );

	DrawDeskTopBackground( );

	// create region for new mail icon
	CreateDestroyMouseRegionForNewMailIcon( );

	//DEF: Added to Init things in various laptop pages
	EnterLaptopInitLaptopPages();
	InitalizeSubSitesList( );

	fShowAtmPanelStartButton = TRUE;

	// lock cursor to screen
	if ( gGameExternalOptions.fLaptopMouseCaptured == TRUE )
	{
		RestrictMouseCursor( &LaptopScreenRect );
	}

	InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

	return( TRUE );
}

void ExitLaptop()
{
	// exit is called due to message box, leave
	if( fExitDueToMessageBox )
	{
		fExitDueToMessageBox = FALSE;
		return;
	}

	if (gfEnterLapTop)
	{
		// we have already exited the laptop, its not currently initialised
		return;
	}
	if ( DidGameJustStart() )
	{
	SetMusicMode( MUSIC_LAPTOP );
	}
	else
	{
	// Restore to old stuff...
	SetMusicMode( MUSIC_RESTORE);
	}

	// Start ambients...
	BuildDayAmbientSounds( );

	// WANNE: disabled rain sound when laptop is displayed
	//if its raining, start the rain showers
	//if( IsItRaining() )
	//{
	//	//Raise the volume to where it was
	//	guiRainLoop	= PlayJA2Ambient( RAIN_1, MIDVOLUME, 0 );
	//}

	// release cursor
	FreeMouseCursor( FALSE );

	// set the fact we are currently not in laptop, for rendering purposes
	fCurrentlyInLaptop = FALSE;


	//Deallocate, save data -- leaving laptop.
	SetRenderFlags( RENDER_FLAG_FULL );

	if( fExitDuringLoad == FALSE )
	{
		ExitLaptopMode(guiCurrentLaptopMode);
	}
	else
	{
		ExitLaptopMode(guiPreviousLaptopMode);
	}

	fExitDuringLoad = FALSE;
	fLoadPendingFlag = FALSE;


	DeleteVideoObjectFromIndex(guiLAPTOP);
	DeleteVideoObjectFromIndex(guiLaptopBACKGROUND);
	DeleteVideoObjectFromIndex( guiTITLEBARLAPTOP );
	DeleteVideoObjectFromIndex( guiLIGHTS );
	DeleteVideoObjectFromIndex( guiTITLEBARICONS );
	DeleteVideoObjectFromIndex( guiEmailWarning );

	// destroy region for new mail icon
	CreateDestroyMouseRegionForNewMailIcon( );

	// get rid of desktop
	DeleteDesktopBackground(	);

	if(fErrorFlag)
	{
	fErrorFlag=FALSE;
	CreateDestroyErrorButton();
	}
	if(fDeleteMailFlag)
	{
	fDeleteMailFlag=FALSE;
	CreateDestroyDeleteNoticeMailButton();
	}
	if(fNewMailFlag)
	{
	fNewMailFlag=FALSE;
	CreateDestroyNewMailButton();
	}


	// get rid of minize button
	CreateDestroyMinimizeButtonForCurrentMode( );



	//MSYS_SetCurrentCursor(CURSOR_NORMAL);
	gfEnterLapTop=TRUE;
	DeleteLapTopButtons();
	DeleteLapTopMouseRegions();
	// restore tactical buttons
	//CreateCurrentTacticalPanelButtons();
	gfShowBookmarks=FALSE;
	CreateDestoryBookMarkRegions( );

	fNewWWW=TRUE;
	RemoveBookmark(-2);
	DeleteBookmark( );
	//DeleteBookmarkRegions();
	DeleteLoadPending( );
	fReDrawNewMailFlag = FALSE;

	//Since we are going to MapScreen, check the flag
	guiTacticalInterfaceFlags |= INTERFACE_MAPSCREEN;

	//close the laptop library
//	CloseLibrary( LIBRARY_LAPTOP );
	//pause the game because we dont want time to advance in the laptop
	UnPauseGame();
#ifdef JA2UB
//ja25 UB
	if( gTacticalStatus.uiFlags & IN_ENDGAME_SEQUENCE )
	{
		HandleJa25EndGameAndGoToCreditsScreen( FALSE );
	}
#endif
}

void
RenderLapTopImage()
{
	HVOBJECT hLapTopHandle;


	if( ( fMaximizingProgram == TRUE ) ||( fMinizingProgram == TRUE) )
	{
		return;
	}

	GetVideoObject(&hLapTopHandle, guiLAPTOP);
	BltVideoObject(FRAME_BUFFER, hLapTopHandle, 0,LAPTOP_X, LAPTOP_Y, VO_BLT_SRCTRANSPARENCY,NULL);


	GetVideoObject(&hLapTopHandle, guiLaptopBACKGROUND);
	BltVideoObject(FRAME_BUFFER, hLapTopHandle, 1,iScreenWidthOffset + 25, iScreenHeightOffset + 23, VO_BLT_SRCTRANSPARENCY,NULL);


	MarkButtonsDirty( );

}
void RenderLaptop()
{
	UINT32 uiTempMode = 0;

	if( ( fMaximizingProgram == TRUE ) ||( fMinizingProgram == TRUE) )
	{
		gfShowBookmarks = FALSE;
		return;
	}

	if(fLoadPendingFlag || fExitDuringLoad)
	{
		uiTempMode = guiCurrentLaptopMode;
		guiCurrentLaptopMode = guiPreviousLaptopMode;
	}

	switch( guiCurrentLaptopMode )
	{
		case( LAPTOP_MODE_NONE ):
			DrawDeskTopBackground( );
			break;
		
#ifdef ENCYCLOPEDIA_WORKS
		case LAPTOP_MODE_ENCYCLOPEDIA: //LEGION
//			RenderEncyclopedia();
			RenderEncyclopedia_NEW();
			break;
			
		case LAPTOP_MODE_ENCYCLOPEDIA_DATA:
//			RenderEncyclopediaLocation(FALSE);
			RenderEncyclopediaData_NEW();
			break;
#endif
			
		case LAPTOP_MODE_BRIEFING_ROOM_PAGE:
			RenderBriefingRoom();
			break;
			
		case LAPTOP_MODE_BRIEFING_ROOM_ENTER:
			RenderBriefingRoomEnter();
			 break;
		
		case LAPTOP_MODE_BRIEFING_ROOM:
			RenderEncyclopediaLocation(FALSE);
			break;
			
		case LAPTOP_MODE_AIM:
		RenderAIM();
			break;
		case LAPTOP_MODE_AIM_MEMBERS:
			RenderAIMMembers();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX:
			RenderAimFacialIndex();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES:
			RenderAimSort();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_ARCHIVES:
			RenderAimArchives();
			break;
		case LAPTOP_MODE_AIM_POLICIES:
			RenderAimPolicies();
			break;
		case LAPTOP_MODE_AIM_LINKS:
			RenderAimLinks();
			break;
		case LAPTOP_MODE_AIM_HISTORY:
			RenderAimHistory();
			break;
		case LAPTOP_MODE_MERC:
			RenderMercs();
			break;
		case LAPTOP_MODE_MERC_FILES:
			RenderMercsFiles();
			break;
		case LAPTOP_MODE_MERC_ACCOUNT:
#ifdef JA2UB
//Ja25
#else
			RenderMercsAccount();
#endif
			break;
		case LAPTOP_MODE_MERC_NO_ACCOUNT:
#ifdef JA2UB
//Ja25
#else
			RenderMercsNoAccount();
#endif
			break;

		case LAPTOP_MODE_BOBBY_R:
			RenderBobbyR();
			break;

		case LAPTOP_MODE_BOBBY_R_GUNS:
			RenderBobbyRGuns();
			break;
		case LAPTOP_MODE_BOBBY_R_AMMO:
			RenderBobbyRAmmo();
			break;
		case LAPTOP_MODE_BOBBY_R_ARMOR:
			RenderBobbyRArmour();
			break;
		case LAPTOP_MODE_BOBBY_R_MISC:
			RenderBobbyRMisc();
			break;
		case LAPTOP_MODE_BOBBY_R_USED:
			RenderBobbyRUsed();
			break;
		case LAPTOP_MODE_BOBBY_R_MAILORDER:
			RenderBobbyRMailOrder();
			break;
		case LAPTOP_MODE_CHAR_PROFILE:
			RenderCharProfile();
			break;
		case LAPTOP_MODE_FLORIST:
			RenderFlorist();
			break;
		case LAPTOP_MODE_FLORIST_FLOWER_GALLERY:
			RenderFloristGallery();
			break;
		case LAPTOP_MODE_FLORIST_ORDERFORM:
			RenderFloristOrderForm();
			break;
		case LAPTOP_MODE_FLORIST_CARD_GALLERY:
			RenderFloristCards();
			break;

		case LAPTOP_MODE_INSURANCE:
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		RenderInsurance();
//JA25:		//	Assert( 0 );
#else
			RenderInsurance();
#endif
			break;

		case LAPTOP_MODE_INSURANCE_INFO:
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		RenderInsuranceInfo();
//JA25:			//	Assert( 0 );
#else
			RenderInsuranceInfo();
#endif
			break;

		case LAPTOP_MODE_INSURANCE_CONTRACT:
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		RenderInsuranceContract();
//JA25:
#else
			RenderInsuranceContract();
#endif
			break;

		case LAPTOP_MODE_INSURANCE_COMMENTS:
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		RenderInsuranceComments();
//JA25:			//	Assert( 0 );
#else
			RenderInsuranceComments();
#endif
			break;

		case LAPTOP_MODE_FUNERAL:
			RenderFuneral();
			break;
		case LAPTOP_MODE_SIRTECH:
			RenderSirTech();
			break;
		case LAPTOP_MODE_FINANCES:
			RenderFinances();
			break;
		case LAPTOP_MODE_PERSONNEL:
			RenderPersonnel();
			break;
		case LAPTOP_MODE_HISTORY:
			RenderHistory();
			break;
		case LAPTOP_MODE_FILES:
			RenderFiles();
			break;
		case LAPTOP_MODE_EMAIL:
			RenderEmail();
			break;
		case( LAPTOP_MODE_WWW ):
			DrawDeskTopBackground( );
			RenderWWWProgramTitleBar( );
		break;
		case( LAPTOP_MODE_BROKEN_LINK ):
			RenderBrokenLink();
		break;

		case LAPTOP_MODE_BOBBYR_SHIPMENTS:
			RenderBobbyRShipments();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_SUMMARY:
			RenderCampaignHistorySummary();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT:
			RenderCampaignHistory_MostImportant();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_LATESTNEWS:
			RenderCampaignHistory_News();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS:
			RenderCampaignHistory();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MATRIX:
			RenderMercCompareMatrix( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_ANALYZE:
			RenderMercCompareAnalyze( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_CUSTOMERS:
			RenderMercCompareCustomers();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MAIN:
			RenderMercCompareMain();
			break;

		case LAPTOP_MODE_WHO_MAIN:
			RenderWHOMain( );
			break;

		case LAPTOP_MODE_WHO_CONTRACT:
			RenderWHOContract( );
			break;

		case LAPTOP_MODE_WHO_TIPS:
			RenderWHOTips( );
			break;

		case LAPTOP_MODE_PMC_MAIN:
			RenderPMCMain( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_MILITIA:
			RenderPMCContract( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_INDIVIDUAL:
			RenderPMCContract( );
			break;

		case LAPTOP_MODE_MILITIAROSTER_MAIN:
			RenderMilitiaWebsiteMain( );
			break;

		case LAPTOP_MODE_INTELMARKET_BUYINFO:
			RenderIntelmarket();
			break;

		case LAPTOP_MODE_INTELMARKET_SELLINFO:
			RenderIntelmarket_Sell();
			break;

		case LAPTOP_MODE_INTELMARKET_ABOUT:
			RenderIntelmarket_About();
			break;

		case LAPTOP_MODE_FACILITY_PRODUCTION:
			RenderFacilityProduction();
			break;

		case LAPTOP_MODE_REBEL_COMMAND:
			RebelCommand::RenderWebsite();
			break;
	}

	if( guiCurrentLaptopMode >= LAPTOP_MODE_WWW )
	{
		// render program bar for www program
		RenderWWWProgramTitleBar( );
	}

	if(fLoadPendingFlag)
	{
		guiCurrentLaptopMode	=	uiTempMode;
		return;
	}

	DisplayProgramBoundingBox( FALSE );

	// mark the buttons dirty at this point
	MarkButtonsDirty( );
}

void EnterNewLaptopMode()
{
	static BOOLEAN fOldLoadFlag=FALSE;


	if( fExitingLaptopFlag )
	{
		return;
	}
	// cause flicker, as we are going to a new program/WEB page
	fFlickerHD = TRUE;

	// handle maximizing of programs
	switch( guiCurrentLaptopMode )
	{
		case ( LAPTOP_MODE_EMAIL ):
		if( gLaptopProgramStates[ LAPTOP_PROGRAM_MAILER ] == LAPTOP_PROGRAM_MINIMIZED )
		{
			// minized, maximized
			if(	fMaximizingProgram == FALSE )
			{
			fInitTitle = TRUE;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopTitles[ 0 ], guiTITLEBARICONS, 0 );
			ExitLaptopMode(guiPreviousLaptopMode);

			}
			fMaximizingProgram = TRUE;
			bProgramBeingMaximized = LAPTOP_PROGRAM_MAILER;
			gLaptopProgramStates[ LAPTOP_PROGRAM_MAILER ] = LAPTOP_PROGRAM_OPEN;

		return;
		}
		break;
		case ( LAPTOP_MODE_FILES ):
		if( gLaptopProgramStates[ LAPTOP_PROGRAM_FILES ] == LAPTOP_PROGRAM_MINIMIZED )
		{
			// minized, maximized
			if(	fMaximizingProgram == FALSE )
			{
			fInitTitle = TRUE;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopTitles[ 1 ], guiTITLEBARICONS, 2 );
			ExitLaptopMode(guiPreviousLaptopMode);

			}

			// minized, maximized
			fMaximizingProgram = TRUE;
			bProgramBeingMaximized = LAPTOP_PROGRAM_FILES;
			gLaptopProgramStates[ LAPTOP_PROGRAM_FILES ] = LAPTOP_PROGRAM_OPEN;
			return;
		}
		break;
		case ( LAPTOP_MODE_PERSONNEL ):
		if( gLaptopProgramStates[ LAPTOP_PROGRAM_PERSONNEL ] == LAPTOP_PROGRAM_MINIMIZED )
		{

			// minized, maximized
			if(	fMaximizingProgram == FALSE )
			{
			fInitTitle = TRUE;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopTitles[ 2 ], guiTITLEBARICONS, 3 );
			ExitLaptopMode(guiPreviousLaptopMode);

			}

			// minized, maximized
			fMaximizingProgram = TRUE;
			bProgramBeingMaximized = LAPTOP_PROGRAM_PERSONNEL;
			gLaptopProgramStates[ LAPTOP_PROGRAM_PERSONNEL ] = LAPTOP_PROGRAM_OPEN;
			return;
		}
		break;
		case ( LAPTOP_MODE_FINANCES ):
		if( gLaptopProgramStates[ LAPTOP_PROGRAM_FINANCES ] == LAPTOP_PROGRAM_MINIMIZED )
		{

			// minized, maximized
			if(	fMaximizingProgram == FALSE )
			{
			fInitTitle = TRUE;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopTitles[ 3 ], guiTITLEBARICONS, 5 );
			ExitLaptopMode(guiPreviousLaptopMode);


			}

			// minized, maximized
			fMaximizingProgram = TRUE;
			bProgramBeingMaximized = LAPTOP_PROGRAM_FINANCES;
			gLaptopProgramStates[ LAPTOP_PROGRAM_FINANCES ] = LAPTOP_PROGRAM_OPEN;
			return;
		}
		break;
		case ( LAPTOP_MODE_HISTORY ):
		if( gLaptopProgramStates[ LAPTOP_PROGRAM_HISTORY ] == LAPTOP_PROGRAM_MINIMIZED )
		{
			// minized, maximized
			if(	fMaximizingProgram == FALSE )
			{
			fInitTitle = TRUE;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopTitles[ 4 ], guiTITLEBARICONS, 4 );
			ExitLaptopMode(guiPreviousLaptopMode);


			}
			// minized, maximized
			fMaximizingProgram = TRUE;
			bProgramBeingMaximized = LAPTOP_PROGRAM_HISTORY;
			gLaptopProgramStates[ LAPTOP_PROGRAM_HISTORY ] = LAPTOP_PROGRAM_OPEN;
			return;
		}
		break;
		case( LAPTOP_MODE_NONE ):
		// do nothing
		break;
		default:
		if( gLaptopProgramStates[ LAPTOP_PROGRAM_WEB_BROWSER ] == LAPTOP_PROGRAM_MINIMIZED )
		{
		
			if ( guiCurrentLaptopMode == LAPTOP_MODE_BRIEFING_ROOM_PAGE || guiCurrentLaptopMode == LAPTOP_MODE_BRIEFING_ROOM || guiCurrentLaptopMode == LAPTOP_MODE_BRIEFING_ROOM_ENTER )
			{
				guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM_ENTER;
			}
#ifdef ENCYCLOPEDIA_WORKS
			else if ( guiCurrentLaptopMode == LAPTOP_MODE_ENCYCLOPEDIA_DATA || guiCurrentLaptopMode == LAPTOP_MODE_ENCYCLOPEDIA )
			{
				guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA;
			}
#endif
			
			// minized, maximized
			if(	fMaximizingProgram == FALSE )
			{
			fInitTitle = TRUE;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pWebTitle[ 0 ], guiTITLEBARICONS, 1 );
			ExitLaptopMode(guiPreviousLaptopMode);
			}
			// minized, maximized
			fMaximizingProgram = TRUE;
			bProgramBeingMaximized = LAPTOP_PROGRAM_WEB_BROWSER;
			gLaptopProgramStates[ LAPTOP_PROGRAM_WEB_BROWSER ] = LAPTOP_PROGRAM_OPEN;
			return;
		}
		break;

	}

	if( ( fMaximizingProgram == TRUE ) ||( fMinizingProgram == TRUE) )
	{
		return;
	}

	if((fOldLoadFlag)&&(!fLoadPendingFlag))
	{
	fOldLoadFlag=FALSE;
	}
	else if((fLoadPendingFlag)&&(!fOldLoadFlag))
	{
	ExitLaptopMode(guiPreviousLaptopMode);
		fOldLoadFlag=TRUE;
		return;
	}
	else if((fOldLoadFlag)&&(fLoadPendingFlag))
	{
		return;
	}
	else
	{
		// do not exit previous mode if coming from sliding bar handler
		if( ( fEnteredNewLapTopDueToHandleSlidingBars == FALSE ) )
		{
		ExitLaptopMode(guiPreviousLaptopMode);
		}
	}



	if((guiCurrentWWWMode==LAPTOP_MODE_NONE)&&(guiCurrentLaptopMode >=LAPTOP_MODE_WWW))
	{
	RenderLapTopImage();
	giCurrentRegion=WWW_REGION;
	RestoreOldRegion(giOldRegion);
	guiCurrentLaptopMode =LAPTOP_MODE_WWW;
	HighLightRegion(giCurrentRegion);
	}
	else
	{
	if( guiCurrentLaptopMode > LAPTOP_MODE_WWW)
	{
		if(guiPreviousLaptopMode < LAPTOP_MODE_WWW )
			guiCurrentLaptopMode = guiCurrentWWWMode;
		else
		{
			guiCurrentWWWMode = guiCurrentLaptopMode;
			giCurrentSubPage =0;
		}
	}
	}

	if( guiCurrentLaptopMode >= LAPTOP_MODE_WWW )
	{
		RenderWWWProgramTitleBar( );
	}



	if( ( guiCurrentLaptopMode >= LAPTOP_MODE_WWW ) &&( guiPreviousLaptopMode >= LAPTOP_MODE_WWW ) )
	{
	gfShowBookmarks = FALSE;
	}



	//Initialize the new mode.
	switch( guiCurrentLaptopMode )
	{
#ifdef ENCYCLOPEDIA_WORKS
		//legion
		case LAPTOP_MODE_ENCYCLOPEDIA:
//			EnterEncyclopedia();
			EnterEncyclopedia_NEW();
			break;
			
		case LAPTOP_MODE_ENCYCLOPEDIA_DATA:
//			EnterEncyclopediaLocation();
			EnterEncyclopediaData_NEW();
			break;
#endif
			
		case LAPTOP_MODE_BRIEFING_ROOM_PAGE:
			EnterBriefingRoom();
			break;
			
		case LAPTOP_MODE_BRIEFING_ROOM_ENTER:
			EnterBriefingRoomEnter();
			break;
		
		case LAPTOP_MODE_BRIEFING_ROOM:
			EnterEncyclopediaLocation();
			break;

		case LAPTOP_MODE_AIM:
			EnterAIM();
			break;
		case LAPTOP_MODE_AIM_MEMBERS:
			EnterAIMMembers();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX:
			EnterAimFacialIndex();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES:
			EnterAimSort();
			break;

		case LAPTOP_MODE_AIM_MEMBERS_ARCHIVES:
			EnterAimArchives();
			break;
		case LAPTOP_MODE_AIM_POLICIES:
			EnterAimPolicies();
			break;
		case LAPTOP_MODE_AIM_LINKS:
			EnterAimLinks();
			break;
		case LAPTOP_MODE_AIM_HISTORY:
			EnterAimHistory();
			break;

		case LAPTOP_MODE_MERC:
			EnterMercs();
			break;
		case LAPTOP_MODE_MERC_FILES:
			EnterMercsFiles();
			break;
		case LAPTOP_MODE_MERC_ACCOUNT:
#ifdef JA2UB
//Ja25
#else
			EnterMercsAccount();
#endif
			break;
		case LAPTOP_MODE_MERC_NO_ACCOUNT:
#ifdef JA2UB
//Ja25
#else
			EnterMercsNoAccount();
#endif
			break;

		case LAPTOP_MODE_BOBBY_R:
			EnterBobbyR();
			break;
		case LAPTOP_MODE_BOBBY_R_GUNS:
			EnterBobbyRGuns();
			break;
		case LAPTOP_MODE_BOBBY_R_AMMO:
			EnterBobbyRAmmo();
			break;
		case LAPTOP_MODE_BOBBY_R_ARMOR:
			EnterBobbyRArmour();
			break;
		case LAPTOP_MODE_BOBBY_R_MISC:
			EnterBobbyRMisc();
			break;
		case LAPTOP_MODE_BOBBY_R_USED:
			EnterBobbyRUsed();
			break;
		case LAPTOP_MODE_BOBBY_R_MAILORDER:
			EnterBobbyRMailOrder();
			break;
		case LAPTOP_MODE_CHAR_PROFILE:
			EnterCharProfile();
			break;

		case LAPTOP_MODE_FLORIST:
			EnterFlorist();
			break;
		case LAPTOP_MODE_FLORIST_FLOWER_GALLERY:
			EnterFloristGallery();
			break;
		case LAPTOP_MODE_FLORIST_ORDERFORM:
			EnterFloristOrderForm();
			break;
		case LAPTOP_MODE_FLORIST_CARD_GALLERY:
			EnterFloristCards();
			break;
#ifdef JA2UB
		case LAPTOP_MODE_INSURANCE:
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				EnterInsurance();
			break;
		case LAPTOP_MODE_INSURANCE_INFO:
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				EnterInsuranceInfo();
			break;
		case LAPTOP_MODE_INSURANCE_CONTRACT:
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				EnterInsuranceContract();
			break;
		case LAPTOP_MODE_INSURANCE_COMMENTS:
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				EnterInsuranceComments();
			break;
#else
		case LAPTOP_MODE_INSURANCE:
			EnterInsurance();
			break;
		case LAPTOP_MODE_INSURANCE_INFO:
			EnterInsuranceInfo();
			break;
		case LAPTOP_MODE_INSURANCE_CONTRACT:
			EnterInsuranceContract();
			break;
		case LAPTOP_MODE_INSURANCE_COMMENTS:
			EnterInsuranceComments();
			break;
#endif
		case LAPTOP_MODE_FUNERAL:
			EnterFuneral();
			break;
		case LAPTOP_MODE_SIRTECH:
			EnterSirTech();
			break;
		case LAPTOP_MODE_FINANCES:
			EnterFinances();
			break;
		case LAPTOP_MODE_PERSONNEL:
			EnterPersonnel();
			break;
		case LAPTOP_MODE_HISTORY:
			EnterHistory();
			break;
		case LAPTOP_MODE_FILES:
			EnterFiles();
			break;
		case LAPTOP_MODE_EMAIL:
			EnterEmail();
			break;
		case LAPTOP_MODE_BROKEN_LINK:
			EnterBrokenLink();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_SUMMARY:
			EnterCampaignHistorySummary();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT:
			EnterCampaignHistory_MostImportant();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_LATESTNEWS:
			EnterCampaignHistory_News();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS:
			EnterCampaignHistory();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MATRIX:
			EnterMercCompareMatrix( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_ANALYZE:
			EnterMercCompareAnalyze( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_CUSTOMERS:
			EnterMercCompareCustomers();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MAIN:
			EnterMercCompareMain( );
			break;

		case LAPTOP_MODE_BOBBYR_SHIPMENTS:
			EnterBobbyRShipments();
			break;

		case LAPTOP_MODE_WHO_MAIN:
			EnterWHOMain( );
			break;

		case LAPTOP_MODE_WHO_CONTRACT:
			EnterWHOContract( );
			break;

		case LAPTOP_MODE_WHO_TIPS:
			EnterWHOTips( );
			break;

		case LAPTOP_MODE_PMC_MAIN:
			EnterPMCMain( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_MILITIA:
			EnterPMCContract( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_INDIVIDUAL:
			EnterPMCContract( );
			break;

		case LAPTOP_MODE_MILITIAROSTER_MAIN:
			EnterMilitiaWebsiteMain( );
			break;

		case LAPTOP_MODE_INTELMARKET_BUYINFO:
			EnterIntelmarket();
			break;

		case LAPTOP_MODE_INTELMARKET_SELLINFO:
			EnterIntelmarket_Sell();
			break;

		case LAPTOP_MODE_INTELMARKET_ABOUT:
			EnterIntelmarket_About();
			break;

		case LAPTOP_MODE_FACILITY_PRODUCTION:
			EnterFacilityProduction();
			break;

		case LAPTOP_MODE_REBEL_COMMAND:
			RebelCommand::EnterWebsite();
			break;
	}

	// first time using webbrowser in this laptop session
	if( ( fFirstTimeInLaptop == TRUE ) && ( guiCurrentLaptopMode >=LAPTOP_MODE_WWW ) )
	{
		// show bookmarks
		gfShowBookmarks = TRUE;

		// reset flag
		fFirstTimeInLaptop = FALSE;
	}

	if( ( !fLoadPendingFlag) )
	{
		CreateDestroyMinimizeButtonForCurrentMode( );
		guiPreviousLaptopMode = guiCurrentLaptopMode;
		SetSubSiteAsVisted( );
	}

	DisplayProgramBoundingBox( TRUE );


	// check to see if we need to go to there default web page of not
	//HandleDefaultWebpageForLaptop( );
}

void HandleLapTopHandles()
{
	if(fLoadPendingFlag)
		return;

	if( ( fMaximizingProgram == TRUE ) || ( fMinizingProgram == TRUE ) )
	{
		return;
	}


 	switch( guiCurrentLaptopMode )
	{
#ifdef ENCYCLOPEDIA_WORKS
		//legion
		case LAPTOP_MODE_ENCYCLOPEDIA:
//			HandleEncyclopedia();
			HandleEncyclopedia_NEW();
			break; 
			
		case LAPTOP_MODE_ENCYCLOPEDIA_DATA:
//			HandleEncyclopediaLocation();
			HandleEncyclopediaData_NEW();
			break; 				
#endif

		case LAPTOP_MODE_BRIEFING_ROOM_PAGE:
			HandleBriefingRoom();
			break;
			
		case LAPTOP_MODE_BRIEFING_ROOM_ENTER:
			HandleBriefingRoomEnter();
			break;
		
		case LAPTOP_MODE_BRIEFING_ROOM:
			HandleEncyclopediaLocation();
			break; 				

		case LAPTOP_MODE_AIM:

			HandleAIM();
			break;
		case LAPTOP_MODE_AIM_MEMBERS:
			HandleAIMMembers();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX:
			HandleAimFacialIndex();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES:
			HandleAimSort();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_ARCHIVES:
			HandleAimArchives();
			break;
		case LAPTOP_MODE_AIM_POLICIES:
			HandleAimPolicies();
			break;
		case LAPTOP_MODE_AIM_LINKS:
			HandleAimLinks();
			break;
		case LAPTOP_MODE_AIM_HISTORY:
			HandleAimHistory();
			break;

		case LAPTOP_MODE_MERC:
			HandleMercs();
			break;
		case LAPTOP_MODE_MERC_FILES:
			HandleMercsFiles();
			break;
#ifdef JA2UB
		case LAPTOP_MODE_MERC_ACCOUNT:
//Ja25						HandleMercsAccount();
			break;
		case LAPTOP_MODE_MERC_NO_ACCOUNT:
//Ja25						HandleMercsNoAccount();
			break;
#else
		case LAPTOP_MODE_MERC_ACCOUNT:
			HandleMercsAccount();
			break;
		case LAPTOP_MODE_MERC_NO_ACCOUNT:
			HandleMercsNoAccount();
			break;
#endif

		case LAPTOP_MODE_BOBBY_R:
			HandleBobbyR();
			break;
		case LAPTOP_MODE_BOBBY_R_GUNS:
			HandleBobbyRGuns();
			break;
		case LAPTOP_MODE_BOBBY_R_AMMO:
			HandleBobbyRAmmo();
			break;
		case LAPTOP_MODE_BOBBY_R_ARMOR:
			HandleBobbyRArmour();
			break;
		case LAPTOP_MODE_BOBBY_R_MISC:
			HandleBobbyRMisc();
			break;
		case LAPTOP_MODE_BOBBY_R_USED:
			HandleBobbyRUsed();
			break;
		case LAPTOP_MODE_BOBBY_R_MAILORDER:
			HandleBobbyRMailOrder();
			break;


		case LAPTOP_MODE_CHAR_PROFILE:
			HandleCharProfile();
			break;
		case LAPTOP_MODE_FLORIST:
			HandleFlorist();
			break;
		case LAPTOP_MODE_FLORIST_FLOWER_GALLERY:
			HandleFloristGallery();
			break;
		case LAPTOP_MODE_FLORIST_ORDERFORM:
			HandleFloristOrderForm();
			break;
		case LAPTOP_MODE_FLORIST_CARD_GALLERY:
			HandleFloristCards();
			break;
#ifdef JA2UB
		case LAPTOP_MODE_INSURANCE:
		//	Assert( 0 );
		if (gGameUBOptions.LaptopLinkInsurance == TRUE )
			HandleInsurance();
			break;

		case LAPTOP_MODE_INSURANCE_INFO:
		//	Assert( 0 );
		if (gGameUBOptions.LaptopLinkInsurance == TRUE )
			HandleInsuranceInfo();
			break;

		case LAPTOP_MODE_INSURANCE_CONTRACT:
		if (gGameUBOptions.LaptopLinkInsurance == TRUE )
			HandleInsuranceContract();
			break;
			
		case LAPTOP_MODE_INSURANCE_COMMENTS:
		//	Assert( 0 );
		if (gGameUBOptions.LaptopLinkInsurance == TRUE )
			HandleInsuranceComments();
			break;
#else
		case LAPTOP_MODE_INSURANCE:
			HandleInsurance();
			break;

		case LAPTOP_MODE_INSURANCE_INFO:
			HandleInsuranceInfo();
			break;

		case LAPTOP_MODE_INSURANCE_CONTRACT:
			HandleInsuranceContract();
			break;
		case LAPTOP_MODE_INSURANCE_COMMENTS:
			HandleInsuranceComments();
			break;

#endif
		case LAPTOP_MODE_FUNERAL:
			HandleFuneral();
			break;
		case LAPTOP_MODE_SIRTECH:
			HandleSirTech();
			break;
		case LAPTOP_MODE_FINANCES:
			HandleFinances();
			break;
		case LAPTOP_MODE_PERSONNEL:
			HandlePersonnel();
			break;
		case LAPTOP_MODE_HISTORY:
			HandleHistory();
			break;
		case LAPTOP_MODE_FILES:
			HandleFiles();
			break;
		case LAPTOP_MODE_EMAIL:
			HandleEmail();
			break;

		case LAPTOP_MODE_BROKEN_LINK:
			HandleBrokenLink();
			break;

		case LAPTOP_MODE_BOBBYR_SHIPMENTS:
			HandleBobbyRShipments();
			break;
			
		case LAPTOP_MODE_CAMPAIGNHISTORY_SUMMARY:
			HandleCampaignHistorySummary();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT:
			HandleCampaignHistory_MostImportant();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_LATESTNEWS:
			HandleCampaignHistory_News();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS:
			HandleCampaignHistory();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MATRIX:
			HandleMercCompareMatrix( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_ANALYZE:
			HandleMercCompareAnalyze( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_CUSTOMERS:
			HandleMercCompareCustomers();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MAIN:
			HandleMercCompareMain();
			break;

		case LAPTOP_MODE_WHO_MAIN:
			HandleWHOMain( );
			break;

		case LAPTOP_MODE_WHO_CONTRACT:
			HandleWHOContract( );
			break;

		case LAPTOP_MODE_WHO_TIPS:
			HandleWHOTips( );
			break;

		case LAPTOP_MODE_PMC_MAIN:
			HandlePMCMain( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_MILITIA:
			HandlePMCContract( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_INDIVIDUAL:
			HandlePMCContract( );
			break;

		case LAPTOP_MODE_MILITIAROSTER_MAIN:
			HandleMilitiaWebsiteMain( );
			break;

		case LAPTOP_MODE_INTELMARKET_BUYINFO:
			HandleIntelmarket();
			break;

		case LAPTOP_MODE_INTELMARKET_SELLINFO:
			HandleIntelmarket_Sell();
			break;

		case LAPTOP_MODE_INTELMARKET_ABOUT:
			HandleIntelmarket_About();
			break;

		case LAPTOP_MODE_FACILITY_PRODUCTION:
			HandleFacilityProduction();
			break;

		case LAPTOP_MODE_REBEL_COMMAND:
			RebelCommand::HandleWebsite();
			break;
	}
}

extern BOOLEAN gfPrintFrameBuffer;

UINT32 LaptopScreenHandle()
{
	INT16 sYOffset = 0;
	INT16 sXOffset = 0;

	//User just changed modes.	This is determined by the button callbacks
	//created in LaptopScreenInit()

	// Correct the minor cosmetic bug (laptop zooming start not correct)
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		sXOffset = -2;
		sYOffset = -2;
	}
	else if (iResolution < _1024x768)
	{
		sYOffset = -1;
	}
	else
	{
		sXOffset = 2;
		sYOffset = 1;
	}

	// Set mouse region values
	LaptopScreenRect.iLeft = LAPTOP_UL_X;
	LaptopScreenRect.iTop = LAPTOP_UL_Y - 5;
	LaptopScreenRect.iRight = LAPTOP_SCREEN_LR_X + 2;
	LaptopScreenRect.iBottom = LAPTOP_SCREEN_LR_Y + 5 + 19;

	// just entered
	if(gfEnterLapTop)
	{
	EnterLaptop();
	CreateLaptopButtons();
	gfEnterLapTop=FALSE;
	}

	if( gfStartMapScreenToLaptopTransition )
	{
		SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
		
		gfStartMapScreenToLaptopTransition = FALSE;

		//Step 1:	Build the laptop image into the save buffer.		
		RestoreBackgroundRects();
		RenderLapTopImage();
		HighLightRegion(giCurrentRegion);
		RenderLaptop();
		RenderButtons();
		PrintDate( );
		PrintBalance( );
		PrintNumberOnTeam( );
		ShowLights();

		if (!gGameExternalOptions.fDisableLaptopTransition)
		{
			//Everything is set up to start the transition animation.
			SGPRect SrcRect2, DstRect;
			INT32 iPercentage, iScalePercentage, iFactor;
			UINT32 uiStartTime, uiTimeRange, uiCurrTime;
			INT32 iX, iY, iWidth, iHeight;
			INT32 iLaptopMonitorCenterX, iLaptopMonitorCenterY;

			INT32 iRealPercentage;

			//Step 2:	The mapscreen image is in the EXTRABUFFER, and laptop is in the SAVEBUFFER
			//		 Start transitioning the screen.
			DstRect.iLeft =	iScreenWidthOffset;						//0
			DstRect.iTop =	iScreenHeightOffset;						//0
			DstRect.iRight = iScreenWidthOffset + 640;				//640
			DstRect.iBottom = iScreenHeightOffset + 480;				//480

			iLaptopMonitorCenterX = SCREEN_WIDTH - 184 + 19 + sXOffset - xResOffset;
			iLaptopMonitorCenterY = SCREEN_HEIGHT - 70 + 16 + sYOffset;

			uiTimeRange = 1000;
			iPercentage = iRealPercentage = 0;
			uiStartTime = GetJA2Clock();

			BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, iScreenWidthOffset, iScreenHeightOffset,
				640, 480 );

			PlayJA2SampleFromFile( "SOUNDS\\Laptop power up (8-11).wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

			while( iRealPercentage < 100	)
			{
				// Lesh: restore mapscreen so laptop zooming won't leave "graphical trail
				BlitBufferToBuffer( guiEXTRABUFFER, FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset,
					SCREEN_WIDTH - iScreenWidthOffset, SCREEN_HEIGHT - iScreenHeightOffset );

				uiCurrTime = GetJA2Clock();
				iPercentage = (uiCurrTime-uiStartTime) * 100 / uiTimeRange;
				iPercentage = min( iPercentage, 100 );

				iRealPercentage = iPercentage;

				//Factor the percentage so that it is modified by a gravity falling acceleration effect.
				iFactor = (iPercentage - 50) * 2;
				if( iPercentage < 50 )
					iPercentage = (UINT32)(iPercentage + iPercentage * iFactor * 0.01 + 0.5);
				else
					iPercentage = (UINT32)(iPercentage + (100-iPercentage) * iFactor * 0.01 + 0.5);

				//Laptop source rect
				if( iPercentage < 99 )
					iScalePercentage = 10000 / (100-iPercentage);
				else
					iScalePercentage = 5333;
				iWidth = 12 * iScalePercentage / 100;
				iHeight = 9 * iScalePercentage / 100;

				iX = iLaptopMonitorCenterX - (iLaptopMonitorCenterX - (iScreenWidthOffset + 320)) * iScalePercentage / 5333;
				iY = iLaptopMonitorCenterY - (iLaptopMonitorCenterY - (iScreenHeightOffset + 240)) * iScalePercentage / 5333;

				SrcRect2.iLeft = iX - iWidth / 2;
				SrcRect2.iRight = SrcRect2.iLeft + iWidth;
				SrcRect2.iTop = iY - iHeight / 2;
				SrcRect2.iBottom = SrcRect2.iTop + iHeight;

				BltStretchVideoSurface( FRAME_BUFFER, guiSAVEBUFFER, iScreenWidthOffset, iScreenHeightOffset, 0, &DstRect, &SrcRect2 );

				InvalidateScreen();

				RefreshScreen( NULL );
			}

			fReDrawScreenFlag = TRUE;
		}
	}

	//DO NOT MOVE THIS FUNCTION CALL!!!

	//This determines if the help screen should be active
	if( ShouldTheHelpScreenComeUp( HELP_SCREEN_LAPTOP, FALSE ) )
	{
		// handle the help screen
		HelpScreenHandler();
		return( LAPTOP_SCREEN );
	}

	RestoreBackgroundRects();

	// handle animated cursors
	HandleAnimatedCursors( );
	// Deque all game events
	DequeAllGameEvents( TRUE );

	// handle sub sites..like BR Guns, BR Ammo, Armour, Misc...for WW Wait..since they are not true sub pages
	// and are not individual sites
	HandleWWWSubSites( );
	UpdateStatusOfDisplayingBookMarks( );

	// check if we need to reset new WWW mode
	CheckIfNewWWWW( );

	if( guiCurrentLaptopMode != guiPreviousLaptopMode )
	{
		if( guiCurrentLaptopMode <=LAPTOP_MODE_WWW )
		{
			fLoadPendingFlag=FALSE;
		}

		if( ( fMaximizingProgram == FALSE ) && ( fMinizingProgram == FALSE ) )
		{
			if( guiCurrentLaptopMode <= LAPTOP_MODE_WWW )
			{
				EnterNewLaptopMode();
				if( ( fMaximizingProgram == FALSE ) && ( fMinizingProgram == FALSE ) )
				{
					guiPreviousLaptopMode = guiCurrentLaptopMode;
				}
			}
			else
			{
				if(!fLoadPendingFlag)
				{
					EnterNewLaptopMode();
					guiPreviousLaptopMode = guiCurrentLaptopMode;
				}
			}
		}
	}

	if( fPausedReDrawScreenFlag )
	{
		fReDrawScreenFlag = TRUE;
		fPausedReDrawScreenFlag = FALSE;
	}

	if( fReDrawScreenFlag )
	{
		RenderLapTopImage();
		HighLightRegion(giCurrentRegion);
		RenderLaptop();
	}


	// are we about to leave laptop
	if( fExitingLaptopFlag )
	{
		if( fLoadPendingFlag == TRUE )
		{
		fLoadPendingFlag = FALSE;
		fExitDuringLoad = TRUE;
		}
		LeaveLapTopScreen( );
	}

	if( fExitingLaptopFlag == FALSE )
	{
		// handle handles for laptop input stream
		HandleLapTopHandles();
	}

	// get keyboard input, handle it
	GetLaptopKeyboardInput();
		
	// check to see if new mail box needs to be displayed
	DisplayNewMailBox( );
	CreateDestroyNewMailButton( );

	// create various mouse regions that are global to laptop system
	CreateDestoryBookMarkRegions( );
	CreateDestroyErrorButton( );

	// check to see if error box needs to be displayed
	DisplayErrorBox();

	// check to see if buttons marked dirty
	CheckMarkButtonsDirtyFlag( );

	// check to see if new mail box needs to be displayed
	ShouldNewMailBeDisplayed();

	// check to see if new mail box needs to be displayed
	ReDrawNewMailBox( );

	// look for unread email
	LookForUnread();
	//Handle keyboard shortcuts...

	// mouse regions
	//HandleLapTopScreenMouseUi();
	//RenderButtons();
	//RenderButtonsFastHelp( );



	if( ( fLoadPendingFlag == FALSE )||( fNewMailFlag	) )
	{
	// render buttons marked dirty
	RenderButtons( );

	// render fast help 'quick created' buttons
//		RenderFastHelp( );
//	RenderButtonsFastHelp( );
	}

	// show text on top of buttons
	if( ( fMaximizingProgram == FALSE ) && ( fMinizingProgram == FALSE ) )
	{
		DrawButtonText();
	}

	// check to see if bookmarks need to be displayed
	if(gfShowBookmarks)
	{
		if( fExitingLaptopFlag )
			gfShowBookmarks = FALSE;
		else
			DisplayBookMarks();
	}

	// check to see if laod pending flag is set
	DisplayLoadPending( );

	// check if we are showing message?
	DisplayWebBookMarkNotify( );

	if( ( fIntermediateReDrawFlag )||( fReDrawPostButtonRender ) )
	{
	// rendering AFTER buttons and button text
		if( ( fMaximizingProgram == FALSE ) && ( fMinizingProgram == FALSE ) )
		{
			PostButtonRendering( );
		}
	}
	//PrintBalance( );

	PrintDate( );

	PrintBalance( );

	PrintNumberOnTeam( );
	DisplayTaskBarIcons();

	// handle if we are maximizing a program from a minimized state or vice versa
	HandleSlidingTitleBar( );

	// flicker HD light as nessacary
	FlickerHDLight( );

	// display power and HD lights
	ShowLights( );

	// render frame rate
	DisplayFrameRate( );

	// invalidate screen if redrawn
	if( fReDrawScreenFlag == TRUE )
	{
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
		fReDrawScreenFlag = FALSE;
	}

	ExecuteVideoOverlays();

	SaveBackgroundRects( );
//	RenderButtonsFastHelp();
	RenderFastHelp( );

	// ex SAVEBUFFER queue
	ExecuteBaseDirtyRectQueue();
	ResetInterface();
	EndFrameBufferRender( );
	return (LAPTOP_SCREEN);
}



UINT32 RenderLaptopPanel()
{
	return 0;
}


UINT32 ExitLaptopMode(UINT32 uiMode)
{
		//Deallocate the previous mode that you were in.

	switch( uiMode )
	{
#ifdef ENCYCLOPEDIA_WORKS
		case LAPTOP_MODE_ENCYCLOPEDIA:
			ExitEncyclopedia_NEW();
//			ExitEncyclopedia();
			//InitEncyklopediaBool();
			break;			
			
		case LAPTOP_MODE_ENCYCLOPEDIA_DATA:
//			ExitEncyclopediaLocation();
			ExitEncyclopediaData_NEW();
			break;
#endif

		case LAPTOP_MODE_BRIEFING_ROOM_PAGE:
			ExitBriefingRoom();
			break;
			
		case LAPTOP_MODE_BRIEFING_ROOM_ENTER:
			ExitBriefingRoomEnter();
			break;
		
		case LAPTOP_MODE_BRIEFING_ROOM:
			ExitEncyclopediaLocation();
			//InitEncyklopediaBool();
			break;	
			
		case LAPTOP_MODE_AIM:
			ExitAIM();
			break;
		case LAPTOP_MODE_AIM_MEMBERS:
			ExitAIMMembers();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX:
			ExitAimFacialIndex();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES:
			ExitAimSort();
			break;
		case LAPTOP_MODE_AIM_MEMBERS_ARCHIVES:
			ExitAimArchives();
			break;
		case LAPTOP_MODE_AIM_POLICIES:
			ExitAimPolicies();
			break;
		case LAPTOP_MODE_AIM_LINKS:
			ExitAimLinks();
			break;
		case LAPTOP_MODE_AIM_HISTORY:
			ExitAimHistory();
			break;

		case LAPTOP_MODE_MERC:
			ExitMercs();
			break;
		case LAPTOP_MODE_MERC_FILES:
			ExitMercsFiles();
			break;
#ifdef JA2UB
		case LAPTOP_MODE_MERC_ACCOUNT:
//Ja25						ExitMercsAccount();
			break;
		case LAPTOP_MODE_MERC_NO_ACCOUNT:
//Ja25			ExitMercsNoAccount();
			break;
#else
		case LAPTOP_MODE_MERC_ACCOUNT:
			ExitMercsAccount();
			break;
		case LAPTOP_MODE_MERC_NO_ACCOUNT:
			ExitMercsNoAccount();
			break;
#endif

		case LAPTOP_MODE_BOBBY_R:
			ExitBobbyR();
			break;
		case LAPTOP_MODE_BOBBY_R_GUNS:
			ExitBobbyRGuns();
			break;
		case LAPTOP_MODE_BOBBY_R_AMMO:
			ExitBobbyRAmmo();
			break;
		case LAPTOP_MODE_BOBBY_R_ARMOR:
			ExitBobbyRArmour();
			break;
		case LAPTOP_MODE_BOBBY_R_MISC:
			ExitBobbyRMisc();
			break;
		case LAPTOP_MODE_BOBBY_R_USED:
			ExitBobbyRUsed();
			break;
		case LAPTOP_MODE_BOBBY_R_MAILORDER:
			ExitBobbyRMailOrder();
			break;


		case LAPTOP_MODE_CHAR_PROFILE:
			ExitCharProfile();
			break;
		case LAPTOP_MODE_FLORIST:
			ExitFlorist();
			break;
		case LAPTOP_MODE_FLORIST_FLOWER_GALLERY:
			ExitFloristGallery();
			break;
		case LAPTOP_MODE_FLORIST_ORDERFORM:
			ExitFloristOrderForm();
			break;
		case LAPTOP_MODE_FLORIST_CARD_GALLERY:
			ExitFloristCards();
			break;
#ifdef JA2UB			
		case LAPTOP_MODE_INSURANCE:
		//	Assert( 0 );
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				ExitInsurance();
			break;

		case LAPTOP_MODE_INSURANCE_INFO:
		//	Assert( 0 );
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				ExitInsuranceInfo();
			break;

		case LAPTOP_MODE_INSURANCE_CONTRACT:
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				ExitInsuranceContract();
			break;
			
		case LAPTOP_MODE_INSURANCE_COMMENTS:
		//	Assert( 0 );
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				ExitInsuranceComments();
			break;
#else
		case LAPTOP_MODE_INSURANCE:
			ExitInsurance();
			break;

		case LAPTOP_MODE_INSURANCE_INFO:
			ExitInsuranceInfo();
			break;

		case LAPTOP_MODE_INSURANCE_CONTRACT:
			ExitInsuranceContract();
			break;
		case LAPTOP_MODE_INSURANCE_COMMENTS:
			ExitInsuranceComments();
			break;
#endif
		case LAPTOP_MODE_FUNERAL:
			ExitFuneral();
			break;
		case LAPTOP_MODE_SIRTECH:
			ExitSirTech();
			break;
		case LAPTOP_MODE_FINANCES:
			ExitFinances();
			break;
		case LAPTOP_MODE_PERSONNEL:
			ExitPersonnel();
			break;
		case LAPTOP_MODE_HISTORY:
			ExitHistory();
			break;
		case LAPTOP_MODE_FILES:
			ExitFiles();
			break;
		case LAPTOP_MODE_EMAIL:
			ExitEmail();
			break;
		case LAPTOP_MODE_BROKEN_LINK:
			ExitBrokenLink();
			break;

		case LAPTOP_MODE_BOBBYR_SHIPMENTS:
			ExitBobbyRShipments();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_SUMMARY:
			ExitCampaignHistorySummary();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT:
			ExitCampaignHistory_MostImportant();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_LATESTNEWS:
			ExitCampaignHistory_News();
			break;

		case LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS:
			ExitCampaignHistory();
			break;

		case LAPTOP_MODE_MERCCOMPARE_MATRIX:
			ExitMercCompareMatrix( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_ANALYZE:
			ExitMercCompareAnalyze( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_CUSTOMERS:
			ExitMercCompareCustomers( );
			break;

		case LAPTOP_MODE_MERCCOMPARE_MAIN:
			ExitMercCompareMain();
			break;

		case LAPTOP_MODE_WHO_MAIN:
			ExitWHOMain();
			break;

		case LAPTOP_MODE_WHO_CONTRACT:
			ExitWHOContract( );
			break;

		case LAPTOP_MODE_WHO_TIPS:
			ExitWHOTips( );
			break;

		case LAPTOP_MODE_PMC_MAIN:
			ExitPMCMain( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_MILITIA:
			ExitPMCContract( );
			break;

		case LAPTOP_MODE_PMC_CONTRACT_INDIVIDUAL:
			ExitPMCContract( );
			break;

		case LAPTOP_MODE_MILITIAROSTER_MAIN:
			ExitMilitiaWebsiteMain();
			break;

		case LAPTOP_MODE_INTELMARKET_BUYINFO:
			ExitIntelmarket();
			break;

		case LAPTOP_MODE_INTELMARKET_SELLINFO:
			ExitIntelmarket_Sell();
			break;

		case LAPTOP_MODE_INTELMARKET_ABOUT:
			ExitIntelmarket_About();
			break;

		case LAPTOP_MODE_FACILITY_PRODUCTION:
			ExitFacilityProduction();
			break;

		case LAPTOP_MODE_REBEL_COMMAND:
			RebelCommand::ExitWebsite();
			break;
	}

	if( ( uiMode != LAPTOP_MODE_NONE )&&( uiMode < LAPTOP_MODE_WWW ) )
	{
	CreateDestroyMinimizeButtonForCurrentMode( );
	}
	return(TRUE);
}


UINT32
CreateLaptopButtons()
{
 memset( giLapTopButton, -1, sizeof( giLapTopButton ) );

 // the program buttons


 gLaptopButtonImage[0]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,0,-1,8,-1 );
 gLaptopButton[0] = QuickCreateButton( gLaptopButtonImage[0], iScreenWidthOffset + 29,
										iScreenHeightOffset + 66,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)EmailRegionButtonCallback);
	CreateLaptopButtonHelpText( gLaptopButton[0], LAPTOP_BN_HLP_TXT_VIEW_EMAIL );

 SpecifyButtonText(	gLaptopButton[0], pLaptopIcons[ 0 ] );
 SpecifyButtonFont( gLaptopButton[0], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[0], 30, 11, TRUE );
 SpecifyButtonDownTextColors( gLaptopButton[0], 2, 0 );
 SpecifyButtonUpTextColors( gLaptopButton[0], 2, 0 );

 gLaptopButtonImage[1]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,1,-1,9,-1 );
 gLaptopButton[1] = QuickCreateButton( gLaptopButtonImage[1], iScreenWidthOffset + 29,
										iScreenHeightOffset + 98,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)WWWRegionButtonCallback);
	CreateLaptopButtonHelpText( gLaptopButton[1], LAPTOP_BN_HLP_TXT_BROWSE_VARIOUS_WEB_SITES );

 SpecifyButtonText(	gLaptopButton[1], pLaptopIcons[1 ] );
 SpecifyButtonFont( gLaptopButton[1], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[1], 30, 11, TRUE );
 SpecifyButtonUpTextColors( gLaptopButton[1], 2, 0 );
 SpecifyButtonDownTextColors( gLaptopButton[1], 2, 0 );

 gLaptopButtonImage[2]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,2,-1,10,-1 );
 gLaptopButton[2] = QuickCreateButton( gLaptopButtonImage[2], iScreenWidthOffset + 29,
										iScreenHeightOffset + 130,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)FilesRegionButtonCallback);
	CreateLaptopButtonHelpText( gLaptopButton[2], LAPTOP_BN_HLP_TXT_VIEW_FILES_AND_EMAIL_ATTACHMENTS );

 SpecifyButtonText(	gLaptopButton[2], pLaptopIcons[ 5 ] );
 SpecifyButtonFont( gLaptopButton[2], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[2], 30, 11, TRUE );
 SpecifyButtonUpTextColors( gLaptopButton[2], 2, 0 );
 SpecifyButtonDownTextColors( gLaptopButton[2], 2, 0 );


 gLaptopButtonImage[3]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,3,-1,11,-1 );
 gLaptopButton[3] = QuickCreateButton( gLaptopButtonImage[3], iScreenWidthOffset + 29,
										iScreenHeightOffset + 194,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PersonnelRegionButtonCallback);
	CreateLaptopButtonHelpText( gLaptopButton[3], LAPTOP_BN_HLP_TXT_VIEW_TEAM_INFO );

 SpecifyButtonText(	gLaptopButton[3], pLaptopIcons[ 3] );
 SpecifyButtonFont( gLaptopButton[3], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[3], 30, 11, TRUE );
 SpecifyButtonUpTextColors( gLaptopButton[3], 2, 0 );
 SpecifyButtonDownTextColors( gLaptopButton[3], 2, 0 );


 gLaptopButtonImage[4]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,4,-1,12,-1 );
 gLaptopButton[4] = QuickCreateButton( gLaptopButtonImage[4], iScreenWidthOffset + 29,
										iScreenHeightOffset + 162,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)HistoryRegionButtonCallback);
	CreateLaptopButtonHelpText( gLaptopButton[4], LAPTOP_BN_HLP_TXT_READ_LOG_OF_EVENTS );

 SpecifyButtonText(	gLaptopButton[4], pLaptopIcons[ 4 ] );
 SpecifyButtonFont( gLaptopButton[4], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[4], 30, 11, TRUE );
 SpecifyButtonUpTextColors( gLaptopButton[4], 2, 0 );
 SpecifyButtonDownTextColors( gLaptopButton[4], 2, 0 );


 gLaptopButtonImage[5]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,5,-1,13,-1 );
 gLaptopButton[5] = QuickCreateButton( gLaptopButtonImage[5], iScreenWidthOffset + 29,
										iScreenHeightOffset + 241,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)FinancialRegionButtonCallback);
	CreateLaptopButtonHelpText( gLaptopButton[5], LAPTOP_BN_HLP_TXT_VIEW_FINANCIAL_SUMMARY_AND_HISTORY );

 SpecifyButtonText(	gLaptopButton[5], pLaptopIcons[ 2 ] );
 SpecifyButtonFont( gLaptopButton[5], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[5], 30, 11, TRUE );
 SpecifyButtonUpTextColors( gLaptopButton[5], 2, 0 );
 SpecifyButtonDownTextColors( gLaptopButton[5], 2, 0 );


 gLaptopButtonImage[6]=	LoadButtonImage( "LAPTOP\\buttonsforlaptop.sti" ,-1,6,-1,14,-1 );
 gLaptopButton[6] = QuickCreateButton( gLaptopButtonImage[6], iScreenWidthOffset + 29,
										iScreenHeightOffset + 378,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnOnCallback);
	CreateLaptopButtonHelpText( gLaptopButton[6], LAPTOP_BN_HLP_TXT_CLOSE_LAPTOP );

 SpecifyButtonText(	gLaptopButton[6], pLaptopIcons[ 6 ] );
 SpecifyButtonFont( gLaptopButton[6], FONT10ARIAL );
 SpecifyButtonTextOffsets( gLaptopButton[6], 25, 11, TRUE );
 SpecifyButtonUpTextColors( gLaptopButton[6], 2, 0 );
 SpecifyButtonDownTextColors( gLaptopButton[6], 2, 0 );


 // define the cursor
	SetButtonCursor(gLaptopButton[0], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(gLaptopButton[1], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(gLaptopButton[2], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(gLaptopButton[3], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(gLaptopButton[4], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(gLaptopButton[5], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(gLaptopButton[6], CURSOR_LAPTOP_SCREEN);



 return (TRUE);
}

void
DeleteLapTopButtons()
{
	UINT32 cnt;

	for( cnt = 0; cnt < 7; cnt++ )
	{
		RemoveButton( gLaptopButton[ cnt ] );
		UnloadButtonImage( gLaptopButtonImage[ cnt ] );
	}
}


void
BtnOnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	if(!(btn->uiFlags & BUTTON_CLICKED_ON))
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);

	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		if ( HandleExit( ) )
		{
//			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fExitingLaptopFlag = TRUE;
			InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
	btn->uiFlags&=~(BUTTON_CLICKED_ON);
	}

}

BOOLEAN LeaveLapTopScreen( void )
{
	INT16 sYOffset = 0;
	INT16 sXOffset = 0;

	// Correct the minor cosmetic bug (laptop zooming start not correct)
	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		sXOffset = -2;
		sYOffset = -2;
	}
	else if (iResolution < _1024x768)
	{
		sYOffset = -1;
	}
	else
	{
		sXOffset = 2;
		sYOffset = 1;
	}

	if( ExitLaptopDone( ) )
	{
		SetLaptopExitScreen( MAP_SCREEN );

		if( gfAtLeastOneMercWasHired == TRUE )
		{
			if( LaptopSaveInfo.gfNewGameLaptop )
			{
				LaptopSaveInfo.gfNewGameLaptop = FALSE;
				fExitingLaptopFlag = TRUE;
				InitNewGame( FALSE );
				gfDontStartTransitionFromLaptop = TRUE;
#ifdef JA2UB				
				//Ja25 UB
				// ATE: Set flag to look for custom maps
				gfProcessCustomMaps = TRUE;
#endif
				return( TRUE );
			}
		}
		else
		{
			gfDontStartTransitionFromLaptop = TRUE;
		}

		SetPendingNewScreen(guiExitScreen);

		if( !gfDontStartTransitionFromLaptop )
		{
			gfDontStartTransitionFromLaptop = TRUE;

			SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

			//Step 1:	Build the laptop image into the save buffer.
			RestoreBackgroundRects();
			RenderLapTopImage();
			HighLightRegion(giCurrentRegion);
			RenderLaptop();
			RenderButtons();
			PrintDate( );
			PrintBalance( );
			PrintNumberOnTeam( );
			ShowLights();

			if (!gGameExternalOptions.fDisableLaptopTransition)
			{
				SGPRect SrcRect1, SrcRect2, DstRect;
				INT32 iPercentage, iScalePercentage, iFactor;
				UINT32 uiStartTime, uiTimeRange, uiCurrTime;
				INT32 iX, iY, iWidth, iHeight;
				INT32 iRealPercentage;
				INT32 iLaptopMonitorCenterX, iLaptopMonitorCenterY;

				//Step 2:	The mapscreen image is in the EXTRABUFFER, and laptop is in the SAVEBUFFER
				//		 Start transitioning the screen.
				DstRect.iLeft = iScreenWidthOffset + 0;			// 0
				DstRect.iTop = iScreenHeightOffset + 0;			// 0
				DstRect.iRight = iScreenWidthOffset + 640;		// 640
				DstRect.iBottom = iScreenHeightOffset + 480;		// 480

				iLaptopMonitorCenterX = SCREEN_WIDTH - 184 + 19 + sXOffset - xResOffset;
				iLaptopMonitorCenterY = SCREEN_HEIGHT - 70 + 16 + sYOffset;

				uiTimeRange = 1000;
				iPercentage = iRealPercentage = 100;
				uiStartTime = GetJA2Clock();

				BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, iScreenWidthOffset, iScreenHeightOffset,
					640, 480 );

				PlayJA2SampleFromFile( "SOUNDS\\Laptop power down (8-11).wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

				while( iRealPercentage > 0	)
				{
					BlitBufferToBuffer( guiEXTRABUFFER, FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset,
						SCREEN_WIDTH - iScreenWidthOffset, SCREEN_HEIGHT - iScreenHeightOffset );

					uiCurrTime = GetJA2Clock();
					iPercentage = (uiCurrTime-uiStartTime) * 100 / uiTimeRange;
					iPercentage = min( iPercentage, 100 );
					iPercentage = 100 - iPercentage;

					iRealPercentage = iPercentage;

					//Factor the percentage so that it is modified by a gravity falling acceleration effect.
					iFactor = (iPercentage - 50) * 2;
					if( iPercentage < 50 )
						iPercentage = (UINT32)(iPercentage + iPercentage * iFactor * 0.01 + 0.5);
					else
						iPercentage = (UINT32)(iPercentage + (100-iPercentage) * iFactor * 0.01 + 0.5);

					//Mapscreen source rect
					SrcRect1.iLeft = iScreenWidthOffset + 464 * iPercentage / 100;
					SrcRect1.iRight = iScreenWidthOffset + 640 - 163 * iPercentage / 100;
					SrcRect1.iTop = iScreenHeightOffset + 417 * iPercentage / 100;
					SrcRect1.iBottom = iScreenHeightOffset + 480 - 55 * iPercentage / 100;
					//Laptop source rect
					if( iPercentage < 99 )
						iScalePercentage = 10000 / (100-iPercentage);
					else
						iScalePercentage = 5333;
					iWidth = 12 * iScalePercentage / 100;
					iHeight = 9 * iScalePercentage / 100;

					iX = iLaptopMonitorCenterX - (iLaptopMonitorCenterX - (iScreenWidthOffset + 320)) * iScalePercentage / 5333;
					iY = iLaptopMonitorCenterY - (iLaptopMonitorCenterY - (iScreenHeightOffset + 240)) * iScalePercentage / 5333;

					SrcRect2.iLeft = iX - iWidth / 2;
					SrcRect2.iRight = SrcRect2.iLeft + iWidth;
					SrcRect2.iTop = iY - iHeight / 2;
					SrcRect2.iBottom = SrcRect2.iTop + iHeight;

					BltStretchVideoSurface( FRAME_BUFFER, guiSAVEBUFFER, 0, iScreenWidthOffset, iScreenHeightOffset, &DstRect, &SrcRect2 );

					InvalidateScreen();
					//gfPrintFrameBuffer = TRUE;
					RefreshScreen( NULL );
				}
			}
		}
	}

	return( TRUE );
}

BOOLEAN HandleExit( void )
{
//	static BOOLEAN fSentImpWarningAlready = FALSE;

	// remind player about IMP
	//if ( LaptopSaveInfo.gfNewGameLaptop != 0 )
	//{
		//if ( !AnyMercsHired( ) )
		//{
			//AddEmail(0,1, GAME_HELP, GetWorldTotalMin( ) );
			//fExitingLaptopFlag = FALSE;
			//return( FALSE );
		//}
	//}

	// new game, send email
	if ( LaptopSaveInfo.gfNewGameLaptop != 0 )
	{
		// Set an event to send this email ( day 2 8:00-12:00 )
		if( ( LaptopSaveInfo.fIMPCompletedFlag == FALSE ) && ( LaptopSaveInfo.fSentImpWarningAlready == FALSE ) )
		{
			AddFutureDayStrategicEvent( EVENT_HAVENT_MADE_IMP_CHARACTER_EMAIL, (8 + Random(4) ) * 60, 0, 1 );

/*
 Moved to an event that gets triggered the next day: HaventMadeImpMercEmailCallBack()

			LaptopSaveInfo.fSentImpWarningAlready = TRUE;
			AddEmail(IMP_EMAIL_AGAIN,IMP_EMAIL_AGAIN_LENGTH,1, GetWorldTotalMin( ) );
*/
			fExitingLaptopFlag = TRUE;

			return( FALSE );
		}
	}
	
	return ( TRUE );
}

void HaventMadeImpMercEmailCallBack()
{
#ifdef JA2UB
	//if the Laptop is NOT broken
	if( gubQuest[ QUEST_FIX_LAPTOP ] != QUESTINPROGRESS && gGameUBOptions.LaptopQuestEnabled == TRUE )
	{
		//if the player STILL hasnt made an imp merc yet
		if( ( LaptopSaveInfo.fIMPCompletedFlag == FALSE ) && ( LaptopSaveInfo.fSentImpWarningAlready == FALSE ) )
		{
			//if the player DIDNT import the save
			//if( !gubFact[ FACT_PLAYER_IMPORTED_SAVE ] )
			//{
				//send a follow up email to the player
				LaptopSaveInfo.fSentImpWarningAlready = TRUE;
				AddEmail( IMP_EMAIL_AGAIN, IMP_EMAIL_AGAIN_LENGTH, CHAR_PROFILE_SITE, GetWorldTotalMin( ), -1 ,-1, TYPE_EMAIL_EMAIL_EDT);
			//}
		}
	}
#else

	//if the player STILL hasnt made an imp merc yet
	if( ( LaptopSaveInfo.fIMPCompletedFlag == FALSE ) && ( LaptopSaveInfo.fSentImpWarningAlready == FALSE ) )
	{
		LaptopSaveInfo.fSentImpWarningAlready = TRUE;
		AddEmail(IMP_EMAIL_AGAIN,IMP_EMAIL_AGAIN_LENGTH,1, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	}
#endif
}


BOOLEAN
CreateLapTopMouseRegions()
{
	// define regions
	
	// the entire laptop display region
	MSYS_DefineRegion( &gLapTopScreenRegion, ( UINT16 )( LaptopScreenRect.iLeft ),( UINT16 )( LaptopScreenRect.iTop ),( UINT16 ) ( LaptopScreenRect.iRight ),( UINT16 )( LaptopScreenRect.iBottom ), MSYS_PRIORITY_NORMAL+1,
							CURSOR_LAPTOP_SCREEN, ScreenRegionMvtCallback, LapTopScreenCallBack );
	
	//MSYS_AddRegion(&gLapTopScreenRegion);

	return (TRUE);
}

BOOLEAN
DeleteLapTopMouseRegions()
{
	MSYS_RemoveRegion( &gLapTopScreenRegion);

	return (TRUE);
}

void FinancialRegionButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			if(giCurrentRegion!=FINANCIAL_REGION)
				giOldRegion=giCurrentRegion;

			giCurrentRegion=FINANCIAL_REGION;
			
			if(gfShowBookmarks)
			{
				gfShowBookmarks=FALSE;
				fReDrawScreenFlag=TRUE;
			}

			guiCurrentLaptopMode=LAPTOP_MODE_FINANCES;

			UpdateListToReflectNewProgramOpened( LAPTOP_PROGRAM_FINANCES );
		}
	}
}



void PersonnelRegionButtonCallback(	GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			if(giCurrentRegion!=PERSONNEL_REGION)
				giOldRegion=giCurrentRegion;

			giCurrentRegion=PERSONNEL_REGION;
			guiCurrentLaptopMode=LAPTOP_MODE_PERSONNEL;

			if(gfShowBookmarks)
			{
				gfShowBookmarks=FALSE;
				fReDrawScreenFlag=TRUE;
			}

			RestoreOldRegion(giOldRegion);
			HighLightRegion(giCurrentRegion);
			gfShowBookmarks=FALSE;

			UpdateListToReflectNewProgramOpened( LAPTOP_PROGRAM_PERSONNEL );
		}
	}
}


void EmailRegionButtonCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// set old region
			if(giCurrentRegion!=EMAIL_REGION)
				giOldRegion=giCurrentRegion;

			// stop showing WWW bookmarks
			if(gfShowBookmarks)
			{
				gfShowBookmarks=FALSE;
			}

			// set current highlight region
			giCurrentRegion=EMAIL_REGION;

			// restore old region
			RestoreOldRegion(giOldRegion);

			// set up current mode
			guiCurrentLaptopMode =LAPTOP_MODE_EMAIL;

			UpdateListToReflectNewProgramOpened( LAPTOP_PROGRAM_MAILER );

			// highlight current region
			HighLightRegion(giCurrentRegion);

			//redraw screen
			fReDrawScreenFlag=TRUE;
		}
	}
}


void WWWRegionButtonCallback(GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);	
			
			if(giCurrentRegion!=WWW_REGION)
				giOldRegion=giCurrentRegion;
			if(!fNewWWW)
				fNewWWWDisplay=FALSE;

			// reset show bookmarks
			if( guiCurrentLaptopMode < LAPTOP_MODE_WWW )
			{
				gfShowBookmarks = FALSE;
				fShowBookmarkInfo = TRUE;
			}
			else
			{
				gfShowBookmarks = !gfShowBookmarks;
			}


			if((gfShowBookmarks)&&(!fNewWWW))
			{
				fReDrawScreenFlag=TRUE;
				fNewWWWDisplay=FALSE;
			}
			else if( fNewWWW )
			{
				// no longer a new WWW mode
				fNewWWW=FALSE;

				// new WWW to display
				fNewWWWDisplay=TRUE;

				// make sure program is maximized
				if( gLaptopProgramStates[ LAPTOP_PROGRAM_WEB_BROWSER ] == LAPTOP_PROGRAM_OPEN )
				{
					// re render laptop region
					RenderLapTopImage();

					// re render background
					DrawDeskTopBackground( );
				}
			}
			giCurrentRegion=WWW_REGION;
			RestoreOldRegion(giOldRegion);	
			
			if ( guiCurrentWWWMode >= LAPTOP_MODE_FINANCES && guiCurrentWWWMode  <= LAPTOP_MODE_BOBBYR_SHIPMENTS )
			{ 
			    IDPageEncyData = PAGENONE;
				UnLoadMenuButtons ();
				bBriefingRoom  = FALSE;
				bBriefingRoomSpecialMission = FALSE;	
			}
	
			if ( IDPageEncyData == PAGEBRIEFINGROOM )
				guiCurrentWWWMode = LAPTOP_MODE_BRIEFING_ROOM_ENTER;//{ bBriefingRoom = TRUE; InitData (); }
			else if ( IDPageEncyData == PAGEBRIEFINGROOMSPECIALMISSION )
				guiCurrentWWWMode = LAPTOP_MODE_BRIEFING_ROOM_ENTER;//{ bBriefingRoomSpecialMission = TRUE; InitData (); } 
			
			if(guiCurrentWWWMode!=LAPTOP_MODE_NONE)
				guiCurrentLaptopMode = guiCurrentWWWMode;
			else
				guiCurrentLaptopMode=LAPTOP_MODE_WWW;

			UpdateListToReflectNewProgramOpened( LAPTOP_PROGRAM_WEB_BROWSER );
			HighLightRegion(giCurrentRegion);
			fReDrawScreenFlag=TRUE;
		}
	}
	else if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// nothing yet
			if(giCurrentRegion!=WWW_REGION)
				giOldRegion=giCurrentRegion;

			giCurrentRegion=WWW_REGION;

			RestoreOldRegion(giOldRegion);

			if(guiCurrentWWWMode!=LAPTOP_MODE_NONE)
				guiCurrentLaptopMode = guiCurrentWWWMode;
			else
				guiCurrentLaptopMode=LAPTOP_MODE_WWW;

			HighLightRegion(giCurrentRegion);

			fReDrawScreenFlag=TRUE;
		}
	}
}


void HistoryRegionButtonCallback(GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			
			// if not in history, update to the fact
			if(giCurrentRegion!=HISTORY_REGION)
				giOldRegion=giCurrentRegion;
			
			if(gfShowBookmarks)
			{
				// stop showing WWW bookmarks
				gfShowBookmarks=FALSE;
			}

			// current region is history
			giCurrentRegion=HISTORY_REGION;

			// restore old region area
			RestoreOldRegion(giOldRegion);

			// set mode to history
			guiCurrentLaptopMode=LAPTOP_MODE_HISTORY;

			// hightlight current icon
			HighLightRegion(giCurrentRegion);

			UpdateListToReflectNewProgramOpened( LAPTOP_PROGRAM_HISTORY );

			gfShowBookmarks=FALSE;

			//redraw screen
			fReDrawScreenFlag=TRUE;
		}
	}
}

void FilesRegionButtonCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
			btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			
			// reset old region
			if(giCurrentRegion!=FILES_REGION)
				giOldRegion=giCurrentRegion;

			// stop showing WWW bookmarks
			if(gfShowBookmarks)
			{
				gfShowBookmarks=FALSE;
				fReDrawScreenFlag=TRUE;
			}

			// set new region
			giCurrentRegion=FILES_REGION;

			// restore old highlight region
			RestoreOldRegion(giOldRegion);

			// highlight new region
			HighLightRegion(giCurrentRegion);

			guiCurrentLaptopMode=LAPTOP_MODE_FILES;

			UpdateListToReflectNewProgramOpened( LAPTOP_PROGRAM_FILES );

			//redraw screen
			fReDrawScreenFlag=TRUE;
		}
	}
}


void HandleLapTopScreenMouseUi()
{
 if ( gEmailRegion.uiFlags & MSYS_MOUSE_IN_AREA )
 {
	giHighLightRegion=EMAIL_REGION;
 }
 else if ( gPersonnelRegion.uiFlags & MSYS_MOUSE_IN_AREA )
 {
	giHighLightRegion=PERSONNEL_REGION;
 }
 else if ( gFinancialRegion.uiFlags & MSYS_MOUSE_IN_AREA )
 {
	giHighLightRegion=FINANCIAL_REGION;
 }
 else if ( gWWWRegion.uiFlags & MSYS_MOUSE_IN_AREA )
 {
	giHighLightRegion=WWW_REGION;
 }
 else if ( gFilesRegion.uiFlags & MSYS_MOUSE_IN_AREA )
 {
	giHighLightRegion=FILES_REGION;
 }
 else if ( gHistoryRegion.uiFlags & MSYS_MOUSE_IN_AREA )
 {
	giHighLightRegion=HISTORY_REGION;
 }
 else
	giHighLightRegion=NO_REGION;

 DrawHighLightRegionBox();
}

void
DrawHighLightRegionBox()
{
	return;
}

void
RestoreOldRegion(INT32 iOldRegion)
{
	return;
}

void HighLightRegion(INT32 iCurrentRegion)
{
	return;
}

void HandleAnimatedButtons()
{
	return;
}
void AnimateButton( UINT32 uiIconID, UINT16 usX, UINT16 usY )
{
	return;
}

void WWWRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	static INT32 iBaseTime=0;
	static INT32 iFrame=0;
	HVOBJECT hLapTopIconHandle;
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iBaseTime=0;
		iFrame=0;
		GetVideoObject(&hLapTopIconHandle, guiWWWICON);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, (UINT16)iFrame,LAPTOP_ICONS_X, LAPTOP_ICONS_WWW_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		DrawLapTopText();
		HighLightRegion(giCurrentRegion);
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}


void EmailRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	static INT32 iBaseTime=0;
	static INT32 iFrame=0;
	HVOBJECT hLapTopIconHandle;
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iBaseTime=0;
		iFrame=0;
		DrawLapTopText();
		GetVideoObject(&hLapTopIconHandle, guiMAILICON);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, (UINT16)iFrame,LAPTOP_ICONS_X, LAPTOP_ICONS_MAIL_Y, VO_BLT_SRCTRANSPARENCY,NULL);

		if(fUnReadMailFlag)
		{
			GetVideoObject(&hLapTopIconHandle, guiUNREAD);
			BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,LAPTOP_ICONS_X+CHECK_X, LAPTOP_ICONS_MAIL_Y+CHECK_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		}

		DrawLapTopText();
		HighLightRegion(giCurrentRegion);
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}


void FinancialRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	static INT32 iBaseTime=0;
	static INT32 iFrame=0;
	HVOBJECT hLapTopIconHandle;
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iBaseTime=0;
		iFrame=0;
		GetVideoObject(&hLapTopIconHandle, guiFINANCIALICON);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, (UINT16)iFrame,LAPTOP_ICONS_X-4, LAPTOP_ICONS_FINANCIAL_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		DrawLapTopText();
		HighLightRegion(giCurrentRegion);
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

void HistoryRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	static INT32 iBaseTime=0;
	static INT32 iFrame=0;
	HVOBJECT hLapTopIconHandle;
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iBaseTime=0;
		iFrame=0;

		GetVideoObject(&hLapTopIconHandle, guiHISTORYICON);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, (UINT16)iFrame,LAPTOP_ICONS_X, LAPTOP_ICONS_HISTORY_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		DrawLapTopText();
		HighLightRegion(giCurrentRegion);
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}



void FilesRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	static INT32 iBaseTime=0;
	static INT32 iFrame=0;
	HVOBJECT hLapTopIconHandle;
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iBaseTime=0;
		iFrame=0;
		GetVideoObject(&hLapTopIconHandle, guiFILESICON);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, (UINT16)iFrame,LAPTOP_ICONS_X, LAPTOP_ICONS_FILES_Y+7, VO_BLT_SRCTRANSPARENCY,NULL);
		DrawLapTopText();
		HighLightRegion(giCurrentRegion);
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}

void PersonnelRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	static INT32 iBaseTime=0;
	static INT32 iFrame=0;
	HVOBJECT hLapTopIconHandle;
	if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iBaseTime=0;
		iFrame=0;

		GetVideoObject(&hLapTopIconHandle, guiPERSICON);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, (UINT16)iFrame,LAPTOP_ICONS_X, LAPTOP_ICONS_PERSONNEL_Y, VO_BLT_SRCTRANSPARENCY,NULL);
 		DrawLapTopText();
		HighLightRegion(giCurrentRegion);
		InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	}
}



void
CheckIfMouseLeaveScreen()
{
 	POINT	MousePos;
	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!
	if((MousePos.x >LAPTOP_SCREEN_LR_X )||(MousePos.x<LAPTOP_UL_X)||(MousePos.y<LAPTOP_UL_Y )||(MousePos.y >LAPTOP_SCREEN_LR_Y))
	{
		guiCurrentLapTopCursor=LAPTOP_PANEL_CURSOR;
	}

}
void ScreenRegionMvtCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	/*if (iReason == MSYS_CALLBACK_REASON_MOVE)
	{
		guiCurrentLapTopCursor=LAPTOP_SCREEN_CURSOR;
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
	CheckIfMouseLeaveScreen();
	}
	*/
}

void ReDrawHighLight()
{
	HighLightRegion(giCurrentRegion);
}


void DrawButtonText()
{
	if(fErrorFlag)
		DrawTextOnErrorButton();

	switch(guiCurrentLaptopMode)
	{
		case LAPTOP_MODE_EMAIL:
			DisplayEmailHeaders( );
			break;
	}
}

void InitBookMarkList()
{
	// sets bookmark list to -1
	memset( LaptopSaveInfo.iBookMarkList, -1, sizeof( LaptopSaveInfo.iBookMarkList ) );
}

void SetBookMark(INT32 iBookId)
{
	// find first empty spot, set to iBookId
	INT32 iCounter=0;
	if(iBookId!=-2)
	{
		while(LaptopSaveInfo.iBookMarkList[iCounter]!=-1)
		{
			// move through list until empty
			if( LaptopSaveInfo.iBookMarkList[iCounter]==iBookId)
			{
				// found it, return
				return;
			}

			++iCounter;
		}

		LaptopSaveInfo.iBookMarkList[iCounter]=iBookId;
	}
}

BOOLEAN	IsBookMarkSet( INT32 iBookId )
{
	INT32 iCounter = 0;
	while ( LaptopSaveInfo.iBookMarkList[iCounter] != -1 )
	{
		// move through list until empty
		if ( LaptopSaveInfo.iBookMarkList[iCounter] == iBookId )
		{
			// found it, return
			return TRUE;
		}

		++iCounter;
	}

	return FALSE;
}


BOOLEAN RemoveBookMark( INT32 iBookId )
{
	INT32 iCounter=0;

	//Loop through the bookmarks to get to the desired bookmark
	while( LaptopSaveInfo.iBookMarkList[iCounter] != iBookId )
	{
		++iCounter;
	}

	//Did we find the right one?
	if( LaptopSaveInfo.iBookMarkList[iCounter] == iBookId )
	{
		//Reset it
		LaptopSaveInfo.iBookMarkList[iCounter] = -1;

		//return true signifing that we found it
		return( TRUE );
	}

	//nope, we didnt find it.
	return( FALSE );
}



BOOLEAN LoadBookmark()
{
	VOBJECT_DESC	VObjectDesc;

	// grab download bars too
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\downloadtop.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiDOWNLOADTOP));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\downloadmid.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiDOWNLOADMID));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\downloadbot.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiDOWNLOADBOT));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\bookmarktop.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBOOKTOP));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\bookmarkmiddle.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBOOKMID));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\webpages.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBOOKMARK));

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\hilite.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBOOKHIGH));
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\Bookmarkbottom.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBOOKBOT));
	
	return( TRUE );
}



void DisplayBookMarks( void )
{
	// will look at bookmarklist and set accordingly
	INT32 iCounter=1;
	// load images
	HVOBJECT hLapTopIconHandle;
	// laptop icons
	INT16 sX, sY;

	// check if we are maximizing or minimizing.. if so, do not display
	if( ( fMaximizingProgram == TRUE ) || ( fMinizingProgram == TRUE ) )
		return;

	// font stuff
	SetFont(BOOK_FONT);
	SetFontForeground(FONT_WHITE);
	SetFontBackground(FONT_BLACK);
	SetFontShadow(NO_SHADOW);

	// set buffer
	SetFontDestBuffer(FRAME_BUFFER ,BOOK_X, BOOK_TOP_Y,BOOK_X+BOOK_WIDTH-10,SCREEN_HEIGHT, FALSE);

	// blt in book mark background
	while( LaptopSaveInfo.iBookMarkList[iCounter-1]!=-1)
	{
		if(iHighLightBookLine==iCounter-1)
		{
			GetVideoObject(&hLapTopIconHandle, guiBOOKHIGH);
			BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,BOOK_X, BOOK_TOP_Y+ ( iCounter*( BOOK_HEIGHT + 6 ) )+ 6, VO_BLT_SRCTRANSPARENCY,NULL);

			// blit in text
			SetFontForeground( FONT_WHITE );
			SetFontBackground( FONT_BLACK );
		}
		else
		{
			GetVideoObject(&hLapTopIconHandle, guiBOOKMARK);
			BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,BOOK_X, BOOK_TOP_Y+ ( iCounter*( BOOK_HEIGHT + 6 ) )+ 6, VO_BLT_SRCTRANSPARENCY,NULL);

			// blit in text
			SetFontForeground( FONT_BLACK );
			SetFontBackground( FONT_BLACK );
		}
		
		FindFontCenterCoordinates(BOOK_X + 3, (UINT16)(BOOK_TOP_Y+2+(iCounter*( BOOK_HEIGHT + 6 ) ) + 6),BOOK_WIDTH - 3,BOOK_HEIGHT+6, pBookMarkStrings[ LaptopSaveInfo.iBookMarkList[iCounter-1] ] , BOOK_FONT, &sX, &sY );

		mprintf(sX, sY,pBookMarkStrings[ LaptopSaveInfo.iBookMarkList[iCounter-1] ] );
		++iCounter;
	}

	// blit one more
	if(iHighLightBookLine==iCounter-1)
	{
		GetVideoObject(&hLapTopIconHandle, guiBOOKHIGH);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,BOOK_X, BOOK_TOP_Y+ ( iCounter*( BOOK_HEIGHT + 6 ) )+ 6, VO_BLT_SRCTRANSPARENCY,NULL);

		// blit in text
		SetFontForeground( FONT_WHITE );
		SetFontBackground( FONT_BLACK );
	}
	else
	{
		GetVideoObject(&hLapTopIconHandle, guiBOOKMARK);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,BOOK_X, BOOK_TOP_Y+ ( iCounter*( BOOK_HEIGHT + 6 ) )+ 6, VO_BLT_SRCTRANSPARENCY,NULL);

		// blit in text
		SetFontForeground( FONT_BLACK );
		SetFontBackground( FONT_BLACK );
	}

	FindFontCenterCoordinates(BOOK_X + 3, (UINT16)(BOOK_TOP_Y+2+(iCounter*( BOOK_HEIGHT + 6 ) ) + 6),BOOK_WIDTH - 3,BOOK_HEIGHT+6,pBookMarkStrings[ CANCEL_STRING ] , BOOK_FONT, &sX, &sY );
	mprintf(sX, sY,pBookMarkStrings[CANCEL_STRING] );
	++iCounter;

	SetFontDestBuffer(FRAME_BUFFER,0,0,SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	//GetVideoObject(&hLapTopIconHandle, guiBOOKBOT);
	//BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,BOOK_X, 6+BOOK_TOP_Y+(iCounter)*BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	/*if(fNewWWWDisplay)
	ScrollDisplayText(BOOK_TOP_Y+2+((iCounter)*BOOK_HEIGHT)+6);
	else
 */	InvalidateRegion(BOOK_X,BOOK_TOP_Y+((iCounter)*BOOK_HEIGHT)+12, BOOK_X+BOOK_WIDTH, BOOK_TOP_Y+((iCounter+1)*BOOK_HEIGHT)+16);
	SetFontShadow(DEFAULT_SHADOW);

	InvalidateRegion(BOOK_X, BOOK_TOP_Y, BOOK_X+BOOK_WIDTH, BOOK_TOP_Y+(iCounter + 6 )*BOOK_HEIGHT+16);
}

void RemoveBookmark(INT32 iBookId)
{
	INT32 iCounter=0;
	if(iBookId==-2)
		return;

	while( LaptopSaveInfo.iBookMarkList[iCounter]!=-1)
	{
		if( LaptopSaveInfo.iBookMarkList[iCounter]==iBookId)
		{
			// found, move everyone back
			for(iCounter=iCounter+1; iCounter <MAX_BOOKMARKS; ++iCounter)
			{
				LaptopSaveInfo.iBookMarkList[iCounter-1] = LaptopSaveInfo.iBookMarkList[iCounter];
			}

			return;
		}

		++iCounter;
	}
}

void DeleteBookmark()
{
	DeleteVideoObjectFromIndex(guiBOOKTOP);
	DeleteVideoObjectFromIndex(guiBOOKMID);
	DeleteVideoObjectFromIndex(guiBOOKHIGH);
	DeleteVideoObjectFromIndex(guiBOOKBOT);
	DeleteVideoObjectFromIndex(guiBOOKMARK);

	DeleteVideoObjectFromIndex(guiDOWNLOADTOP);
	DeleteVideoObjectFromIndex(guiDOWNLOADMID);
	DeleteVideoObjectFromIndex(guiDOWNLOADBOT);


}

void ScrollDisplayText(INT32 iY)
{
	static INT32 iBaseTime=0;
	static INT16 sCurX;

	// if we are just enetering, set basetime to current clock value
	if(iBaseTime==0)
		iBaseTime=GetJA2Clock();

	// long enough time has passed, shift string
	if(GetJA2Clock()-iBaseTime >SCROLL_DIFFERENCE)
	{

	// reset postion, if scrolled too far
	if(sCurX <SCROLL_MIN)
		sCurX=BOOK_X+BOOK_WIDTH;
	else
		sCurX--;

	// reset base time
	iBaseTime=GetJA2Clock();
	}

	// font stuff
	SetFontDestBuffer(FRAME_BUFFER, BOOK_X,0,BOOK_X+BOOK_WIDTH,SCREEN_HEIGHT, FALSE);

	SetFontForeground(FONT_BLACK);
	SetFontBackground(FONT_BLACK);

	// print the scrolling string for bookmarks
	mprintf(sCurX, iY, pBookmarkTitle[1]);

	// reset buffer
	SetFontDestBuffer(FRAME_BUFFER, 0,0,SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	// invalidate region
	InvalidateRegion(BOOK_X,iY, BOOK_X+BOOK_WIDTH, iY+BOOK_HEIGHT);

}
void CreateBookMarkMouseRegions()
{
	INT32 iCounter=0;
	// creates regions based on number of entries
	while( LaptopSaveInfo.iBookMarkList[iCounter] != -1 )
	{
		MSYS_DefineRegion(&gBookmarkMouseRegions[iCounter],(INT16)BOOK_X, (UINT16)(BOOK_TOP_Y+((iCounter+1)*(BOOK_HEIGHT+6))+6), BOOK_X+BOOK_WIDTH ,(INT16)(BOOK_TOP_Y+((iCounter+2)*(BOOK_HEIGHT+6))+6),
			MSYS_PRIORITY_HIGHEST - 2,CURSOR_LAPTOP_SCREEN, BookmarkMvtCallBack, BookmarkCallBack );
		//MSYS_AddRegion(&gBookmarkMouseRegions[iCounter]);
		MSYS_SetRegionUserData(&gBookmarkMouseRegions[iCounter],0,iCounter);
		MSYS_SetRegionUserData(&gBookmarkMouseRegions[iCounter],1,0);

		//Create the regions help text
		CreateBookMarkHelpText( &gBookmarkMouseRegions[iCounter], LaptopSaveInfo.iBookMarkList[iCounter] );

		++iCounter;
	}

	// now add one more
	// for the cancel button
	MSYS_DefineRegion(&gBookmarkMouseRegions[iCounter],(INT16)BOOK_X, (UINT16)(BOOK_TOP_Y+((iCounter+1)*(BOOK_HEIGHT+6))+6), BOOK_X+BOOK_WIDTH ,(INT16)(BOOK_TOP_Y+((iCounter+2)*(BOOK_HEIGHT+6))+6),
			MSYS_PRIORITY_HIGHEST - 2,CURSOR_LAPTOP_SCREEN, BookmarkMvtCallBack, BookmarkCallBack );
	//MSYS_AddRegion(&gBookmarkMouseRegions[iCounter]);
	MSYS_SetRegionUserData(&gBookmarkMouseRegions[iCounter],0,iCounter);
	MSYS_SetRegionUserData(&gBookmarkMouseRegions[iCounter],1,CANCEL_STRING );
}

void DeleteBookmarkRegions()
{
	INT32 iCounter=0;
	//deletes bookmark regions
	while( LaptopSaveInfo.iBookMarkList[iCounter] != -1 )
	{
		MSYS_RemoveRegion(&gBookmarkMouseRegions[iCounter]);
		++iCounter;
	}

	// now one for the cancel
	MSYS_RemoveRegion(&gBookmarkMouseRegions[iCounter]);
}

void CreateDestoryBookMarkRegions( void )
{
	// checks to see if a bookmark needs to be created or destroyed
	static BOOLEAN fOldShowBookmarks=FALSE;

	if((gfShowBookmarks)&&(!fOldShowBookmarks))
	{
		// create regions
		CreateBookMarkMouseRegions( );
		fOldShowBookmarks=TRUE;
	}
	else if((!gfShowBookmarks)&&(fOldShowBookmarks))
	{
		// destroy bookmarks
		DeleteBookmarkRegions( );
		fOldShowBookmarks=FALSE;
	}
}


void BookmarkCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	INT32 iCount;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	// we are in process of loading
	if( fLoadPendingFlag == TRUE )
	{
		return;
	}

	if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		iCount=MSYS_GetRegionUserData(pRegion, 0);
		if( MSYS_GetRegionUserData(pRegion, 1) == CANCEL_STRING )
		{
			gfShowBookmarks = FALSE;
			fReDrawScreenFlag = TRUE;
		}

		if( LaptopSaveInfo.iBookMarkList[iCount] != -1 )
		{
			GoToWebPage( LaptopSaveInfo.iBookMarkList[iCount] );
		}
		else
		{
			return;
		}
	}
	else if(iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		iCount=MSYS_GetRegionUserData(pRegion, 0);
	}
}



void GoToWebPage(INT32 iPageId )
{
#ifdef JA2UB
	//if the laptop is broken
	if( (gubQuest[ QUEST_FIX_LAPTOP ] != QUESTINPROGRESS) || (gGameUBOptions.LaptopQuestEnabled != TRUE) )
{
#endif
	switch(iPageId)
	{
		case AIM_BOOKMARK:
			guiCurrentWWWMode=LAPTOP_MODE_AIM;
		guiCurrentLaptopMode=LAPTOP_MODE_AIM;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ AIM_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ AIM_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
		break;
		
		//LEGION
		case ENCYCLOPEDIA_BOOKMARK:
#ifdef ENCYCLOPEDIA_WORKS
		  guiCurrentWWWMode=LAPTOP_MODE_ENCYCLOPEDIA;
		  guiCurrentLaptopMode=LAPTOP_MODE_ENCYCLOPEDIA;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ ENCYCLOPEDIA_BOOKMARK ] == FALSE )
			{
        // reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ ENCYCLOPEDIA_BOOKMARK ] = TRUE;
				fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
				fLoadPendingFlag = TRUE;
				fFastLoadFlag =  TRUE;
			}
#endif
		break;
		
		case BRIEFING_ROOM_BOOKMARK:
		  guiCurrentWWWMode=LAPTOP_MODE_BRIEFING_ROOM_PAGE;
		  guiCurrentLaptopMode=LAPTOP_MODE_BRIEFING_ROOM_PAGE;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ BRIEFING_ROOM_BOOKMARK ] == FALSE )
			{
        // reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ BRIEFING_ROOM_BOOKMARK ] = TRUE;
				fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
				fLoadPendingFlag = TRUE;
				fFastLoadFlag =  TRUE;
			}
		break;
		
		case BOBBYR_BOOKMARK:
			guiCurrentWWWMode=LAPTOP_MODE_BOBBY_R;
		guiCurrentLaptopMode=LAPTOP_MODE_BOBBY_R;

				// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ BOBBYR_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ BOBBYR_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
		break;
		case( IMP_BOOKMARK ):
		guiCurrentWWWMode=LAPTOP_MODE_CHAR_PROFILE;
		guiCurrentLaptopMode=LAPTOP_MODE_CHAR_PROFILE;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ IMP_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ IMP_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
			iCurrentImpPage = IMP_HOME_PAGE;
		break;
		case( MERC_BOOKMARK ):

			//if the mercs server has gone down, but hasnt come up yet
			if( LaptopSaveInfo.fMercSiteHasGoneDownYet == TRUE && LaptopSaveInfo.fFirstVisitSinceServerWentDown == FALSE )
			{
				guiCurrentWWWMode = LAPTOP_MODE_BROKEN_LINK;
				guiCurrentLaptopMode = LAPTOP_MODE_BROKEN_LINK;
			}
			else
			{
				guiCurrentWWWMode = LAPTOP_MODE_MERC;
				guiCurrentLaptopMode = LAPTOP_MODE_MERC;
			}


			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ MERC_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ MERC_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
		break;
		case( FUNERAL_BOOKMARK ):
		guiCurrentWWWMode=LAPTOP_MODE_FUNERAL;
		guiCurrentLaptopMode=LAPTOP_MODE_FUNERAL;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ FUNERAL_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ FUNERAL_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
		break;
		case( FLORIST_BOOKMARK ):
		guiCurrentWWWMode=LAPTOP_MODE_FLORIST;
		guiCurrentLaptopMode=LAPTOP_MODE_FLORIST;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ FLORIST_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ FLORIST_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
		break;

		case( INSURANCE_BOOKMARK ):
		guiCurrentWWWMode=LAPTOP_MODE_INSURANCE;
		guiCurrentLaptopMode=LAPTOP_MODE_INSURANCE;

			// do we have to have a World Wide Wait
			if( LaptopSaveInfo.fVisitedBookmarkAlready[ INSURANCE_BOOKMARK ] == FALSE )
			{
		// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[ INSURANCE_BOOKMARK ] = TRUE;
		fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
		fLoadPendingFlag = TRUE;
				fFastLoadFlag =	TRUE;
			}
		break;

		case CAMPAIGNHISTORY_BOOKMARK:
			{
				// if the option is off, we instead link to a 'broken' website
				if ( !gGameExternalOptions.fCampaignHistoryWebSite )
				{
					guiCurrentWWWMode=LAPTOP_MODE_BROKEN_LINK;
					guiCurrentLaptopMode=LAPTOP_MODE_BROKEN_LINK;

					return GoToWebPage(LAPTOP_MODE_BROKEN_LINK);
				}

				guiCurrentWWWMode=LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS;
				guiCurrentLaptopMode=LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS;

				// do we have to have a World Wide Wait
				if( LaptopSaveInfo.fVisitedBookmarkAlready[ CAMPAIGNHISTORY_BOOKMARK ] == FALSE )
				{
					// reset flag and set load pending flag
					LaptopSaveInfo.fVisitedBookmarkAlready[ CAMPAIGNHISTORY_BOOKMARK ] = TRUE;
					fLoadPendingFlag = TRUE;
				}
				else
				{
					// fast reload
					fLoadPendingFlag = TRUE;
					fFastLoadFlag =	TRUE;
				}
			}
			break;

		case MERCCOMPARE_BOOKMARK:
			{		
				guiCurrentWWWMode = LAPTOP_MODE_MERCCOMPARE_MAIN;
				guiCurrentLaptopMode = LAPTOP_MODE_MERCCOMPARE_MAIN;

				// do we have to have a World Wide Wait
				if ( LaptopSaveInfo.fVisitedBookmarkAlready[MERCCOMPARE_BOOKMARK] == FALSE )
				{
					// reset flag and set load pending flag
					LaptopSaveInfo.fVisitedBookmarkAlready[MERCCOMPARE_BOOKMARK] = TRUE;
					fLoadPendingFlag = TRUE;
				}
				else
				{
					// fast reload
					fLoadPendingFlag = TRUE;
					fFastLoadFlag = TRUE;
				}
			}
			break;

		case WHO_BOOKMARK:
			{
				guiCurrentWWWMode = LAPTOP_MODE_WHO_MAIN;
				guiCurrentLaptopMode = LAPTOP_MODE_WHO_MAIN;

				// do we have to have a World Wide Wait
				if ( LaptopSaveInfo.fVisitedBookmarkAlready[WHO_BOOKMARK] == FALSE )
				{
					// reset flag and set load pending flag
					LaptopSaveInfo.fVisitedBookmarkAlready[WHO_BOOKMARK] = TRUE;
					fLoadPendingFlag = TRUE;
				}
				else
				{
					// fast reload
					fLoadPendingFlag = TRUE;
					fFastLoadFlag = TRUE;
				}
			}
			break;

		case PMC_BOOKMARK:
			{
				guiCurrentWWWMode = LAPTOP_MODE_PMC_MAIN;
				guiCurrentLaptopMode = LAPTOP_MODE_PMC_MAIN;

				// do we have to have a World Wide Wait
				if ( LaptopSaveInfo.fVisitedBookmarkAlready[PMC_BOOKMARK] == FALSE )
				{
					// reset flag and set load pending flag
					LaptopSaveInfo.fVisitedBookmarkAlready[PMC_BOOKMARK] = TRUE;
					fLoadPendingFlag = TRUE;
				}
				else
				{
					// fast reload
					fLoadPendingFlag = TRUE;
					fFastLoadFlag = TRUE;
				}
			}
			break;

		case MILITIAROSTER_BOOKMARK:
		{
			guiCurrentWWWMode = LAPTOP_MODE_MILITIAROSTER_MAIN;
			guiCurrentLaptopMode = LAPTOP_MODE_MILITIAROSTER_MAIN;

			// do we have to have a World Wide Wait
			if ( LaptopSaveInfo.fVisitedBookmarkAlready[MILITIAROSTER_BOOKMARK] == FALSE )
			{
				// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[MILITIAROSTER_BOOKMARK] = TRUE;
				fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
				fLoadPendingFlag = TRUE;
				fFastLoadFlag = TRUE;
			}
		}
		break;

		case INTELMARKET_BOOKMARK:
		{
			guiCurrentWWWMode = LAPTOP_MODE_INTELMARKET_BUYINFO;
			guiCurrentLaptopMode = LAPTOP_MODE_INTELMARKET_BUYINFO;

			// do we have to have a World Wide Wait
			if ( LaptopSaveInfo.fVisitedBookmarkAlready[INTELMARKET_BOOKMARK] == FALSE )
			{
				// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[INTELMARKET_BOOKMARK] = TRUE;
				fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
				fLoadPendingFlag = TRUE;
				fFastLoadFlag = TRUE;
			}
		}
		break;

		case PRODUCTION_BOOKMARK:
		{
			guiCurrentWWWMode = LAPTOP_MODE_FACILITY_PRODUCTION;
			guiCurrentLaptopMode = LAPTOP_MODE_FACILITY_PRODUCTION;

			// do we have to have a World Wide Wait
			if ( LaptopSaveInfo.fVisitedBookmarkAlready[PRODUCTION_BOOKMARK] == FALSE )
			{
				// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[PRODUCTION_BOOKMARK] = TRUE;
				fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
				fLoadPendingFlag = TRUE;
				fFastLoadFlag = TRUE;
			}
		}
		break;

		case REBELCOMMAND_BOOKMARK:
		{
			guiCurrentWWWMode = LAPTOP_MODE_REBEL_COMMAND;
			guiCurrentLaptopMode = LAPTOP_MODE_REBEL_COMMAND;

			// do we have to have a World Wide Wait
			if ( LaptopSaveInfo.fVisitedBookmarkAlready[REBELCOMMAND_BOOKMARK] == FALSE )
			{
				// reset flag and set load pending flag
				LaptopSaveInfo.fVisitedBookmarkAlready[REBELCOMMAND_BOOKMARK] = TRUE;
				fLoadPendingFlag = TRUE;
			}
			else
			{
				// fast reload
				fLoadPendingFlag = TRUE;
				fFastLoadFlag = TRUE;
			}
		}
		break;
	}

#ifdef JA2UB	
	}
	//the web is not working
	else
	{
		guiCurrentWWWMode = LAPTOP_MODE_BROKEN_LINK;
		guiCurrentLaptopMode = LAPTOP_MODE_BROKEN_LINK;

		// slow load
		fLoadPendingFlag = TRUE;
		fFastLoadFlag =  FALSE;
	}
#endif

	gfShowBookmarks=FALSE;
	fReDrawScreenFlag=TRUE;
	return;
}

void BookmarkMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason == MSYS_CALLBACK_REASON_MOVE)
	{
	iHighLightBookLine=MSYS_GetRegionUserData(pRegion, 0);
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
	iHighLightBookLine=-1;
	}
}


BOOLEAN LoadLoadPending( void )
{
	// function will load the load pending graphics
	// reuse bookmark
	// load graph window and bar
	VOBJECT_DESC	VObjectDesc;

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\graphwindow.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiGRAPHWINDOW));
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\graphsegment.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiGRAPHBAR));


	return (TRUE);
}

BOOLEAN DisplayLoadPending( void )
{
	// this function will display the load pending and return if the load is done
	static INT32 iBaseTime=0;
	static INT32 iTotalTime=0;
	INT32 iTempTime=0;
	INT32 iCounter=0;
	INT32 iDifference=0;
	HVOBJECT hLapTopIconHandle;
	INT32 iLoadTime;
	INT32 iUnitTime;
	INT16 sXPosition = 0, sYPosition = 0;

	if (gGameExternalOptions.fFastWWWSitesLoading)	
		gfTemporaryDisablingOfLoadPendingFlag = TRUE;

	// if merc webpage, make it longer
		//TEMP disables the loadpending
	if( gfTemporaryDisablingOfLoadPendingFlag )
	{
		iLoadTime=1;
		iUnitTime=1;
	}
	else
	{
		if( ( fFastLoadFlag == TRUE )&&( fConnectingToSubPage == TRUE ) )
		{
			iUnitTime=FASTEST_UNIT_TIME;
		}
		else if( fFastLoadFlag == TRUE )
		{
			iUnitTime=FAST_UNIT_TIME;
		}
		else if( fConnectingToSubPage == TRUE )
		{
			iUnitTime=ALMOST_FAST_UNIT_TIME;
		}

		//if we are connecting the MERC site, and the MERC site hasnt yet moved to their new site, have the sloooww wait
		else if(guiCurrentLaptopMode == LAPTOP_MODE_MERC && !LaptopSaveInfo.fMercSiteHasGoneDownYet )
		{
			iUnitTime=LONG_UNIT_TIME;
		}
		else
		{
			iUnitTime=UNIT_TIME;
		}
		
		iLoadTime = iUnitTime * 30;
#ifdef JA2UB		
		//if the site we are going to is the web poage not found page
		if( guiCurrentLaptopMode == LAPTOP_MODE_BROKEN_LINK )
		{
			iLoadTime=1;
			iUnitTime=1;
		}
#endif
	}


	// we are now waiting on a web page to download, reset counter
	if(!fLoadPendingFlag)
	{
	fDoneLoadPending = FALSE;
	fFastLoadFlag = FALSE;
	fConnectingToSubPage = FALSE;
	iBaseTime=0;
	iTotalTime=0;
	return (FALSE);
	}
	// if total time is exceeded, return (TRUE)
	if(iBaseTime==0)
	{
	iBaseTime=GetJA2Clock();
	}



	if(iTotalTime >= iLoadTime)
	{
		// done loading, redraw screen
		fLoadPendingFlag	= FALSE;
		fFastLoadFlag = FALSE;
		iTotalTime=0;
		iBaseTime=0;
		fDoneLoadPending = TRUE;
		fConnectingToSubPage = FALSE;
		fPausedReDrawScreenFlag = TRUE;

		return(TRUE);
	}


	iDifference=GetJA2Clock()-iBaseTime;



	// difference has been long enough or we are redrawing the screen
	if( (iDifference) > iUnitTime)
	{
		// LONG ENOUGH TIME PASSED
		iCounter=0;
		iBaseTime=GetJA2Clock();
		iTotalTime+=iDifference;
	iTempTime=iTotalTime;

	}

	// new mail, don't redraw
	if( fNewMailFlag == TRUE )
	{
		return( FALSE );
	}

	RenderButtons( );

//	RenderFastHelp( );
//	RenderButtonsFastHelp( );


	// display top middle and bottom of box
	GetVideoObject(&hLapTopIconHandle, guiDOWNLOADTOP);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,DOWNLOAD_X, DOWNLOAD_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	GetVideoObject(&hLapTopIconHandle, guiDOWNLOADMID);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,DOWNLOAD_X, DOWNLOAD_Y+DOWN_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);
	GetVideoObject(&hLapTopIconHandle, guiDOWNLOADBOT);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,DOWNLOAD_X, DOWNLOAD_Y+2*DOWN_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);
	GetVideoObject(&hLapTopIconHandle, guiTITLEBARICONS);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 1,DOWNLOAD_X + 4, DOWNLOAD_Y + 1, VO_BLT_SRCTRANSPARENCY,NULL);


	// font stuff
	SetFont(DOWNLOAD_FONT);
	SetFontForeground(FONT_WHITE);
	SetFontBackground(FONT_BLACK);
	SetFontShadow(NO_SHADOW);

	// reload or download?
	if( fFastLoadFlag == TRUE )
	{
		FindFontCenterCoordinates( iScreenWidthOffset + 328,
			iScreenHeightOffset, 118, 0, pDownloadString[1], DOWNLOAD_FONT, &sXPosition, &sYPosition );

	// display download string
		mprintf( sXPosition, DOWN_STRING_Y,pDownloadString[1]);
	}
	else
	{
		FindFontCenterCoordinates( iScreenWidthOffset + 328,
			iScreenHeightOffset, /*iScreenWidthOffset + */118, 0, pDownloadString[ 0 ], DOWNLOAD_FONT, &sXPosition, &sYPosition );

		// display download string
		mprintf( sXPosition, DOWN_STRING_Y,pDownloadString[0]);
	}

	// get and blt the window video object
	GetVideoObject(&hLapTopIconHandle, guiGRAPHWINDOW);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,LAPTOP_WINDOW_X, LAPTOP_WINDOW_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// check to see if we are only updating screen, but not passed a new element in the load pending display


	iTempTime = iTotalTime;
	// decide how many time units are to be displayed, based on amount of time passed
	while(iTempTime >0)
	{
	GetVideoObject(&hLapTopIconHandle, guiGRAPHBAR);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,LAPTOP_BAR_X+(UNIT_WIDTH*iCounter), LAPTOP_BAR_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		iTempTime-=iUnitTime;
		iCounter++;

		// have we gone too far?
		if( iCounter > 30 )
		{
			iTempTime = 0;
		}
	}

	InvalidateRegion(DOWNLOAD_X, DOWNLOAD_Y, DOWNLOAD_X + 150, DOWNLOAD_Y + 100);

	// re draw screen and new mail warning box
	SetFontShadow(DEFAULT_SHADOW);

	MarkButtonsDirty( );

	DisableMercSiteButton();

	return (FALSE);
}

void DeleteLoadPending( void )
{
	// this funtion will delete the load pending graphics
	// reuse bookmark
	DeleteVideoObjectFromIndex(guiGRAPHBAR);
	DeleteVideoObjectFromIndex(guiGRAPHWINDOW);
	return;
}

void BtnErrorCallback(GUI_BUTTON *btn,INT32 reason)
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
		fErrorFlag=FALSE;

		}
	}
}
void CreateDestroyErrorButton( void )
{
 static BOOLEAN fOldErrorFlag=FALSE;
 if((fErrorFlag)&&(!fOldErrorFlag))
 {
	// create inventory button
	fOldErrorFlag=TRUE;

	// load image and create error confirm button
	giErrorButtonImage[0]=LoadButtonImage( "LAPTOP\\errorbutton.sti" ,-1,0,-1,1,-1 );
	giErrorButton[0]= QuickCreateButton( giErrorButtonImage[0], ERROR_X+ERROR_BTN_X, ERROR_Y + ERROR_BTN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnErrorCallback);

	// define the cursor
	SetButtonCursor(giErrorButton[0], CURSOR_LAPTOP_SCREEN);

	// define the screen mask
	MSYS_DefineRegion(&pScreenMask,0, 0,SCREEN_WIDTH, SCREEN_HEIGHT,
		MSYS_PRIORITY_HIGHEST-3,CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, LapTopScreenCallBack);

	// add region
	MSYS_AddRegion(&pScreenMask);
 }
 else if((!fErrorFlag)&&(fOldErrorFlag))
 {
	// done dsiplaying, get rid of button and screen mask
	fOldErrorFlag=FALSE;

	RemoveButton( giErrorButton[0] );
	UnloadButtonImage( giErrorButtonImage[0] );

	MSYS_RemoveRegion(&pScreenMask);

	// re draw screen
	fReDrawScreenFlag=TRUE;

 }
	return;
}


void DisplayErrorBox( void )
{
	// this function will display the error graphic
	HVOBJECT hLapTopIconHandle;
	if(!fErrorFlag)
		return;

	// get and blt top portion
	GetVideoObject(&hLapTopIconHandle, guiBOOKTOP);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// middle * 5
	GetVideoObject(&hLapTopIconHandle, guiBOOKMID);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y+BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	GetVideoObject(&hLapTopIconHandle, guiBOOKMID);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y+2*BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	GetVideoObject(&hLapTopIconHandle, guiBOOKMID);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y+3*BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	GetVideoObject(&hLapTopIconHandle, guiBOOKMID);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y+4*BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	GetVideoObject(&hLapTopIconHandle, guiBOOKMID);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y+5*BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	// the bottom
	GetVideoObject(&hLapTopIconHandle, guiBOOKBOT);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,ERROR_X, ERROR_Y+6*BOOK_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);

	// font stuff
	SetFont(ERROR_TITLE_FONT);
	SetFontForeground(FONT_WHITE);
	SetFontBackground(FONT_BLACK);
	SetFontShadow(NO_SHADOW);

	// print title
	mprintf(ERROR_X + ERROR_TITLE_X, ERROR_Y + ERROR_TITLE_Y, pErrorStrings[0]);
	SetFontForeground(FONT_BLACK);
	SetFont(ERROR_FONT);

	// display error string
	DisplayWrappedString(ERROR_X+ERROR_TEXT_X,(UINT16)(ERROR_Y+ERROR_TEXT_Y+DisplayWrappedString(ERROR_X+ERROR_TEXT_X, ERROR_Y+ERROR_TEXT_Y, BOOK_WIDTH, 2,ERROR_FONT,FONT_BLACK, pErrorStrings[1],FONT_BLACK,FALSE,CENTER_JUSTIFIED)), BOOK_WIDTH, 2,ERROR_FONT,FONT_BLACK, pErrorStrings[2],FONT_BLACK,FALSE,CENTER_JUSTIFIED);

	SetFontShadow(DEFAULT_SHADOW);

	return;
}


void DrawTextOnErrorButton()
{
	// draws text on error button
	SetFont(ERROR_TITLE_FONT);
	SetFontForeground(FONT_BLACK);
	SetFontBackground(FONT_BLACK);
	SetFontShadow(NO_SHADOW);
	mprintf(ERROR_X+ERROR_BTN_X+ERROR_BTN_TEXT_X, ERROR_Y + ERROR_BTN_Y+ERROR_BTN_TEXT_Y, pErrorStrings[3]);
	SetFontShadow(DEFAULT_SHADOW);

	InvalidateRegion(ERROR_X, ERROR_Y, ERROR_X+BOOK_WIDTH, ERROR_Y+6*BOOK_HEIGHT);
	return;
}

// This function is called every time the laptop is FIRST initialized, ie whenever the laptop is loaded.	It calls
// various init function in the laptop pages that need to be inited when the laptop is just loaded.
void EnterLaptopInitLaptopPages()
{
	EnterInitAimMembers();
	EnterInitAimArchives();
	EnterInitAimPolicies();
	EnterInitAimHistory();
	EnterInitFloristGallery();
#ifdef JA2UB
	if (gGameUBOptions.LaptopLinkInsurance == TRUE )
		EnterInitInsuranceInfo();
#else
	EnterInitInsuranceInfo();
#endif
	EnterInitBobbyRayOrder();
	EnterInitMercSite();


	// init sub pages for WW Wait
	InitIMPSubPageList( );
}


void CheckMarkButtonsDirtyFlag( void )
{
	// this function checks the fMarkButtonsDirtyFlag, if true, mark buttons dirty
	if( fMarkButtonsDirtyFlag )
	{
	// flag set, mark buttons and reset
		MarkButtonsDirty( );
		fMarkButtonsDirtyFlag = FALSE;
	}

	return;
}


void PostButtonRendering( void )
{
	// this function is in place to allow for post button rendering

	switch( guiCurrentLaptopMode )
	{
	case LAPTOP_MODE_AIM:
//	 RenderCharProfilePostButton( );
		break;

	case 	LAPTOP_MODE_AIM_MEMBERS:
			RenderAIMMembersTopLevel();
		break;

	}
	return;
}

void ShouldNewMailBeDisplayed()
{

	BOOLEAN fReDraw=FALSE;
	switch( guiCurrentLaptopMode )
	{
		case LAPTOP_MODE_AIM_MEMBERS:
			fReDraw = DisableNewMailMessage();
			break;
	}
/*
	if(fReDraw)
	{
		RenderLapTopImage();
		HighLightRegion(giCurrentRegion);
		RenderLaptop();
	}
 */
}


void DisplayPlayersBalanceToDate( void )
{
	// print players balance to date
	CHAR16 sString[ 100 ];
	INT16 sX, sY;

	// initialize string
	memset( sString, 0, sizeof( sString ) );

	// font stuff
	SetFont( FONT10ARIAL);
	SetFontForeground( 142 );
	SetFontShadow(NO_SHADOW);

	// parse straigth number
	swprintf( sString, L"%d", LaptopSaveInfo.iCurrentBalance );

	// put in commas, then dollar sign
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );

	// get center
	FindFontCenterCoordinates( (INT16)LAPTOP_ICON_TEXT_X, iScreenHeightOffset, (INT16)(LAPTOP_ICON_TEXT_WIDTH) ,(INT16)(LAPTOP_ICON_TEXT_HEIGHT), sString, LAPTOPICONFONT, &sX, &sY );

//	gprintfdirty( sX , LAPTOP_ICON_TEXT_FINANCIAL_Y + 10, sString );
	// printf it!
	if( ButtonList[ gLaptopButton[ 5 ] ]->uiFlags & BUTTON_CLICKED_ON )
	{
		mprintf( sX + 5, LAPTOP_ICON_TEXT_FINANCIAL_Y + 10 + 5, sString);
	}
	else
	{
		mprintf( sX, LAPTOP_ICON_TEXT_FINANCIAL_Y + 10, sString);
	}

	// reset shadow
	SetFontShadow(DEFAULT_SHADOW);

	return;
}


void CheckIfNewWWWW( void )
{
	// if no www mode, set new www flag..until new www mode that is not 0

    if( guiCurrentWWWMode == LAPTOP_MODE_NONE )
	{
		fNewWWW = TRUE;
	}
	else
	{
		fNewWWW = FALSE;
	}

	return;
}


void HandleLapTopESCKey( void )
{
	// will handle esc key events, since handling depends on state of laptop

	if( fNewMailFlag )
	{
		// get rid of new mail warning box
		fNewMailFlag=FALSE;
		CreateDestroyNewMailButton();

		// force redraw
		fReDrawScreenFlag = TRUE;
		RenderLaptop( );
	}
	else if(fDeleteMailFlag)
	{
		// get rid of delete mail box
		fDeleteMailFlag = FALSE;
		CreateDestroyDeleteNoticeMailButton();

		// force redraw
		fReDrawScreenFlag = TRUE;
		RenderLaptop( );
	}
	else if( fErrorFlag )
	{
		// get rid of error warning box
		fErrorFlag=FALSE;
		CreateDestroyErrorButton();

		// force redraw
		fReDrawScreenFlag = TRUE;
		RenderLaptop( );
	}

	else if( gfShowBookmarks )
	{
		// get rid of bookmarks
		gfShowBookmarks = FALSE;

		// force redraw
		fReDrawScreenFlag = TRUE;
		RenderLapTopImage( );
		RenderLaptop( );
	}
	else
	{
		// leave
		fExitingLaptopFlag = TRUE;
		HandleExit( );
	}

	return;
}


void HandleLapTopEnterKey( void )
{
	// will handle esc key events, since handling depends on state of laptop

	if( fNewMailFlag )
	{
		// get rid of new mail warning box
		fNewMailFlag=FALSE;
		CreateDestroyNewMailButton();

		// force redraw
		fReDrawScreenFlag = TRUE;
		RenderLaptop( );
	}
	
	return;
}


void HandleRightButtonUpEvent( void )
{

	// will handle the right button up event
	if( fNewMailFlag )
	{
		// get rid of new mail warning box
	fNewMailFlag=FALSE;
	CreateDestroyNewMailButton();

		// force redraw
	fReDrawScreenFlag = TRUE;
	RenderLaptop( );
	}
	else if(fDeleteMailFlag)
	{
	// get rid of delete mail box
	fDeleteMailFlag=FALSE;
	CreateDestroyDeleteNoticeMailButton();

		// force redraw
	fReDrawScreenFlag = TRUE;
	RenderLaptop( );
	}
	else if( fErrorFlag )
	{
		// get rid of error warning box
	fErrorFlag=FALSE;
	CreateDestroyErrorButton();

		// force redraw
	fReDrawScreenFlag = TRUE;
	RenderLaptop( );
	}

	else if( gfShowBookmarks )
	{
		// get rid of bookmarks
		gfShowBookmarks = FALSE;

		// force redraw
	fReDrawScreenFlag = TRUE;
		RenderLapTopImage( );
	RenderLaptop( );
	}
	/* // Buggler: bugged mouse region render behavior on returning to view another mail message after right click in other laptop modes
	else if( fDisplayMessageFlag )
	{
		fDisplayMessageFlag = FALSE;

		// force redraw
		fReDrawScreenFlag = TRUE;
		RenderLapTopImage( );
		RenderLaptop( );

	}*/
	else if( fShowBookmarkInfo )
	{
		fShowBookmarkInfo = FALSE;
	}

}


void HandleLeftButtonUpEvent( void )
{

	// will handle the left button up event

	if( gfShowBookmarks )
	{
		// get rid of bookmarks
		gfShowBookmarks = FALSE;

		// force redraw
	fReDrawScreenFlag = TRUE;
		RenderLapTopImage( );
	RenderLaptop( );
	}
	else if( fShowBookmarkInfo )
	{
		fShowBookmarkInfo = FALSE;
	}
}

void LapTopScreenCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{

	return;
	}

	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		HandleLeftButtonUpEvent( );
		return;
	}
	if(iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	HandleRightButtonUpEvent( );
		return;
	}

	return;
}

BOOLEAN		DoLapTopMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect pCenteringRect= {LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_LR_Y };


	// reset exit mode
	fExitDueToMessageBox = TRUE;



	// do message box and return
	iLaptopMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT8 ) ( ubFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	&pCenteringRect );



	// send back return state
	return( ( iLaptopMessageBox != -1 ) );
}


BOOLEAN DoLapTopSystemMessageBoxWithRect( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect )
{
	// reset exit mode
	fExitDueToMessageBox = TRUE;

	// do message box and return
	iLaptopMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	pCenteringRect );

	// send back return state
	return( ( iLaptopMessageBox != -1 ) );
}

BOOLEAN DoLapTopSystemMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {iScreenWidthOffset, iScreenHeightOffset,
		iScreenWidthOffset + 640, INV_INTERFACE_START_Y };


	// reset exit mode
	fExitDueToMessageBox = TRUE;

	// do message box and return
	iLaptopMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback, &CenteringRect );

	// send back return state
	return( ( iLaptopMessageBox != -1 ) );
}



//places a tileable pattern down
BOOLEAN WebPageTileBackground(UINT8 ubNumX, UINT8 ubNumY, UINT16 usWidth, UINT16 usHeight, UINT32 uiBackgroundIdentifier)
{
	HVOBJECT hBackGroundHandle;
	UINT16	x,y, uiPosX, uiPosY;

	// Blt the Wood background
	GetVideoObject(&hBackGroundHandle, uiBackgroundIdentifier);

	uiPosY = LAPTOP_SCREEN_WEB_UL_Y;
	for(y=0; y<ubNumY; y++)
	{
		uiPosX = LAPTOP_SCREEN_UL_X;
		for(x=0; x<ubNumX; x++)
		{
		BltVideoObject(FRAME_BUFFER, hBackGroundHandle, 0,uiPosX, uiPosY, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += usWidth;
		}
		uiPosY += usHeight;
	}
	return(TRUE);
}


BOOLEAN InitTitleBarMaximizeGraphics( UINT32 uiBackgroundGraphic, STR16 pTitle, UINT32 uiIconGraphic, UINT16 usIconGraphicIndex )
{
	VSURFACE_DESC		vs_desc;
	HVOBJECT	hImageHandle;

	Assert( uiBackgroundGraphic );

	// Create a background video surface to blt the title bar onto
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = LAPTOP_TITLE_BAR_WIDTH;
	vs_desc.usHeight = LAPTOP_TITLE_BAR_HEIGHT;
	vs_desc.ubBitDepth = 16;
	CHECKF( AddVideoSurface( &vs_desc, &guiTitleBarSurface) );

	//blit the toolbar grapgucs onto the surface
	GetVideoObject(&hImageHandle, uiBackgroundGraphic);
	BltVideoObject(guiTitleBarSurface, hImageHandle, 0,0,0, VO_BLT_SRCTRANSPARENCY,NULL);

	//blit th icon onto the tool bar
	GetVideoObject(&hImageHandle, uiIconGraphic);
	BltVideoObject(guiTitleBarSurface, hImageHandle, usIconGraphicIndex, LAPTOP_TITLE_BAR_ICON_OFFSET_X, LAPTOP_TITLE_BAR_ICON_OFFSET_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	SetFontDestBuffer( guiTitleBarSurface, 0, 0, vs_desc.usWidth, vs_desc.usHeight , FALSE);
	DrawTextToScreen(pTitle, LAPTOP_TITLE_BAR_TEXT_OFFSET_X, LAPTOP_TITLE_BAR_TEXT_OFFSET_Y, 0, FONT14ARIAL, FONT_MCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE);

	return( TRUE );
}

BOOLEAN DisplayTitleBarMaximizeGraphic(BOOLEAN fForward, BOOLEAN fInit, UINT16 usTopLeftX, UINT16 usTopLeftY, UINT16 usTopRightX )
{
	static 	INT8			ubCount;
	INT16		sPosX, sPosY, sPosRightX, sPosBottomY, sWidth, sHeight;
	SGPRect		SrcRect;
	SGPRect		DestRect;
	static SGPRect		LastRect;
	FLOAT dTemp;


	if( fInit )
	{
		if( gfTitleBarSurfaceAlreadyActive )
					return(FALSE);

		gfTitleBarSurfaceAlreadyActive=TRUE;
		if( fForward )
		{
			ubCount = 1;
		}
		else
		{
			ubCount = NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS - 1;
		}
	}

		dTemp = (LAPTOP_TITLE_BAR_TOP_LEFT_X - usTopLeftX) / (FLOAT)NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS;
		sPosX = (INT16)(usTopLeftX + dTemp * ubCount);

		dTemp = (LAPTOP_TITLE_BAR_TOP_RIGHT_X - usTopRightX) / (FLOAT)NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS;
		sPosRightX = (INT16)(usTopRightX + dTemp * ubCount);

		dTemp = ( LAPTOP_TITLE_BAR_TOP_LEFT_Y - usTopLeftY ) / (FLOAT)NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS;
		sPosY =	(INT16)(usTopLeftY + dTemp * ubCount);


	sPosBottomY = LAPTOP_TITLE_BAR_HEIGHT;

	SrcRect.iLeft = 0;
	SrcRect.iTop = 0;
	SrcRect.iRight = LAPTOP_TITLE_BAR_WIDTH;
	SrcRect.iBottom = LAPTOP_TITLE_BAR_HEIGHT;

	//if its the last fram, bit the tittle bar to the final position
	if( ubCount == NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS)
	{
		DestRect.iLeft = LAPTOP_TITLE_BAR_TOP_LEFT_X;
		DestRect.iTop = LAPTOP_TITLE_BAR_TOP_LEFT_Y;
		DestRect.iRight = LAPTOP_TITLE_BAR_TOP_RIGHT_X;
		DestRect.iBottom = DestRect.iTop + sPosBottomY;
	}
	else
	{
		DestRect.iLeft = sPosX;
		DestRect.iTop = sPosY;
		DestRect.iRight = sPosRightX;
		DestRect.iBottom = DestRect.iTop + sPosBottomY;
	}



	if( fForward )
	{
		//Restore the old rect
		if( ubCount > 1 )
		{
			sWidth = (UINT16)(LastRect.iRight - LastRect.iLeft);
			sHeight = (UINT16)(LastRect.iBottom - LastRect.iTop);
			BlitBufferToBuffer(guiSAVEBUFFER, guiRENDERBUFFER, (UINT16)LastRect.iLeft, (UINT16)LastRect.iTop, sWidth, sHeight);
		}

		//Save rectangle
		if( ubCount > 0 )
		{
			sWidth = (UINT16)(DestRect.iRight - DestRect.iLeft);
			sHeight = (UINT16)(DestRect.iBottom - DestRect.iTop);
			BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, (UINT16)DestRect.iLeft, (UINT16)DestRect.iTop, sWidth, sHeight);
		}
	}
	else
	{
		//Restore the old rect
		if( ubCount < NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS - 1 )
		{
			sWidth = (UINT16)(LastRect.iRight - LastRect.iLeft);
			sHeight = (UINT16)(LastRect.iBottom - LastRect.iTop);
			BlitBufferToBuffer(guiSAVEBUFFER, guiRENDERBUFFER, (UINT16)LastRect.iLeft, (UINT16)LastRect.iTop, sWidth, sHeight);
		}

		//Save rectangle
		if( ubCount < NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS )
		{
			sWidth = (UINT16)(DestRect.iRight - DestRect.iLeft);
			sHeight = (UINT16)(DestRect.iBottom - DestRect.iTop);
			BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, (UINT16)DestRect.iLeft, (UINT16)DestRect.iTop, sWidth, sHeight);
		}
	}


	BltStretchVideoSurface(FRAME_BUFFER, guiTitleBarSurface, 0, 0, VO_BLT_SRCTRANSPARENCY, &SrcRect, &DestRect );

	InvalidateRegion(DestRect.iLeft,DestRect.iTop, DestRect.iRight, DestRect.iBottom);
	InvalidateRegion(LastRect.iLeft,LastRect.iTop, LastRect.iRight, LastRect.iBottom);

	LastRect = DestRect;

	if( fForward )
	{
		if( ubCount == NUMBER_OF_LAPTOP_TITLEBAR_ITERATIONS)
		{
			gfTitleBarSurfaceAlreadyActive=FALSE;
			return(TRUE);
		}
		else
		{
			ubCount++;
			return(FALSE);
		}
	}
	else
	{
		if( ubCount == 0)
		{
			gfTitleBarSurfaceAlreadyActive=FALSE;
			return(TRUE);
		}
		else
		{
			ubCount--;
			return(FALSE);
		}
	}

	return(TRUE);
}

void RemoveTitleBarMaximizeGraphics()
{
	DeleteVideoSurfaceFromIndex( guiTitleBarSurface );
}


void HandleSlidingTitleBar( void )
{
	if( ( fMaximizingProgram == FALSE ) && ( fMinizingProgram == FALSE ) )
	{
		return;
	}

	if( fExitingLaptopFlag )
	{
		return;
	}

	if( fMaximizingProgram )
	{
		switch( bProgramBeingMaximized )
		{
			case( LAPTOP_PROGRAM_MAILER ):

			fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 66, iScreenWidthOffset + 49 );

			//fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, 29, 66, 29 + 20 );

			if( fMaximizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				fEnteredNewLapTopDueToHandleSlidingBars = TRUE;
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;

			case( LAPTOP_PROGRAM_FILES ):
			fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 120, iScreenWidthOffset + 29 + 20 );
			if( fMaximizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				fEnteredNewLapTopDueToHandleSlidingBars = TRUE;
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_FINANCES ):
			fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 226, iScreenWidthOffset + 29 + 20 );
			if( fMaximizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				fEnteredNewLapTopDueToHandleSlidingBars = TRUE;
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_PERSONNEL ):
			fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 194, iScreenWidthOffset + 29 + 20 );
			if( fMaximizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				fEnteredNewLapTopDueToHandleSlidingBars = TRUE;
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_HISTORY ):
			fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 162, iScreenWidthOffset + 29 + 20 );
			if( fMaximizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				fEnteredNewLapTopDueToHandleSlidingBars = TRUE;
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_WEB_BROWSER ):
			fMaximizingProgram = !DisplayTitleBarMaximizeGraphic( TRUE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 99, iScreenWidthOffset + 29 + 20 );
			if( fMaximizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				fEnteredNewLapTopDueToHandleSlidingBars = TRUE;
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
		}

		MarkButtonsDirty( );
	}
	else
	{
		// minimizing
		switch( bProgramBeingMaximized )
		{
			case( LAPTOP_PROGRAM_MAILER ):
			fMinizingProgram = !DisplayTitleBarMaximizeGraphic( FALSE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 66, iScreenWidthOffset + 29 + 20 );
			if( fMinizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_FILES ):
			fMinizingProgram = !DisplayTitleBarMaximizeGraphic( FALSE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 130, iScreenWidthOffset + 29 + 20 );
			if( fMinizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_FINANCES ):
			fMinizingProgram = !DisplayTitleBarMaximizeGraphic( FALSE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 226, iScreenWidthOffset + 29 + 20 );
			if( fMinizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_PERSONNEL ):
			fMinizingProgram = !DisplayTitleBarMaximizeGraphic( FALSE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 194, iScreenWidthOffset + 29 + 20 );
			if( fMinizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_HISTORY ):
			fMinizingProgram = !DisplayTitleBarMaximizeGraphic( FALSE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset + 162 , iScreenWidthOffset + 29 + 20 );
			if( fMinizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
			case( LAPTOP_PROGRAM_WEB_BROWSER ):
			fMinizingProgram = !DisplayTitleBarMaximizeGraphic( FALSE, fInitTitle, iScreenWidthOffset + 29,
				iScreenHeightOffset +	99 , iScreenWidthOffset + 29 + 20 );
			if( fMinizingProgram	== FALSE )
			{
				RemoveTitleBarMaximizeGraphics();
				EnterNewLaptopMode();
				fEnteredNewLapTopDueToHandleSlidingBars = FALSE;
				fPausedReDrawScreenFlag = TRUE;
			}
			break;
		}
	}


	// reset init
	fInitTitle = FALSE;
}


void ShowLights( void )
{
	// will show lights depending on state
	HVOBJECT hHandle;
	GetVideoObject( &hHandle, guiLIGHTS);

	if( fPowerLightOn == TRUE )
	{
		BltVideoObject(FRAME_BUFFER, hHandle, 0, iScreenWidthOffset + 44, iScreenHeightOffset + 466, VO_BLT_SRCTRANSPARENCY,NULL);
	}
	else
	{
		BltVideoObject(FRAME_BUFFER, hHandle, 1, iScreenWidthOffset + 44, iScreenHeightOffset + 466, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	if( fHardDriveLightOn == TRUE )
	{
		BltVideoObject(FRAME_BUFFER, hHandle, 0, iScreenWidthOffset + 88, iScreenHeightOffset + 466, VO_BLT_SRCTRANSPARENCY,NULL);
	}
	else
	{
		BltVideoObject(FRAME_BUFFER, hHandle, 1, iScreenWidthOffset + 88, iScreenHeightOffset + 466, VO_BLT_SRCTRANSPARENCY,NULL);
	}
}


void FlickerHDLight( void )
{
	static INT32 iBaseTime = 0;
	static INT32 iTotalDifference = 0;
	INT32 iDifference = 0;

	if( fLoadPendingFlag == TRUE )
	{
		fFlickerHD = TRUE;
	}

	if( fFlickerHD == FALSE )
	{
		return;
	}

	if( iBaseTime == 0 )
	{
		iBaseTime = GetJA2Clock( );
	}

	iDifference = GetJA2Clock( ) - iBaseTime;

	if( ( iTotalDifference > HD_FLICKER_TIME ) && ( fLoadPendingFlag == FALSE ) )
	{
		iBaseTime = GetJA2Clock( );
		fHardDriveLightOn = FALSE;
		iBaseTime = 0;
		iTotalDifference = 0;
		fFlickerHD = FALSE;
		InvalidateRegion(iScreenWidthOffset + 88, iScreenHeightOffset + 466, iScreenWidthOffset + 102, iScreenHeightOffset + 477 );
		return;
	}

	if( iDifference > FLICKER_TIME )
	{
		iTotalDifference += iDifference;

		if( fLoadPendingFlag == TRUE )
		{
			iTotalDifference = 0;
		}

		if( ( Random( 2 ) ) == 0 )
		{
			fHardDriveLightOn = TRUE;
		}
		else
		{
			fHardDriveLightOn = FALSE;
		}

		InvalidateRegion(iScreenWidthOffset + 88, iScreenHeightOffset + 466, iScreenWidthOffset + 102, iScreenHeightOffset + 477 );
	}
}


BOOLEAN ExitLaptopDone( void )
{
	// check if this is the first time, to reset counter

	static BOOLEAN fOldLeaveLaptopState = FALSE;
	static INT32 iBaseTime = 0;
	INT32 iDifference = 0;

	if( fOldLeaveLaptopState == FALSE )
	{
		fOldLeaveLaptopState = TRUE;
		iBaseTime = GetJA2Clock( );
	}

	fPowerLightOn = FALSE;

	InvalidateRegion(iScreenWidthOffset + 44, iScreenHeightOffset + 466,
	iScreenWidthOffset + 58, iScreenHeightOffset + 477 );
	// get the current difference
	iDifference = GetJA2Clock( ) - iBaseTime;

	// did we wait long enough?
	if( iDifference > EXIT_LAPTOP_DELAY_TIME )
	{
		iBaseTime = 0;
		fOldLeaveLaptopState = FALSE;
		return TRUE;
	}
	
	return FALSE;
}


void CreateDestroyMinimizeButtonForCurrentMode( void )
{
	// will create the minimize button

	static BOOLEAN fAlreadyCreated = FALSE;
	// check to see if created, if so, do nothing

	// check current mode
	if( ( guiCurrentLaptopMode == LAPTOP_MODE_NONE )&&( guiPreviousLaptopMode != LAPTOP_MODE_NONE ) )
	{
		fCreateMinimizeButton = FALSE;
	}
	else if( ( guiCurrentLaptopMode != LAPTOP_MODE_NONE ) )
	{
		fCreateMinimizeButton = TRUE;
	}
	else if( ( guiPreviousLaptopMode != LAPTOP_MODE_NONE ) )
	{
		fCreateMinimizeButton = FALSE;
	}

	// leaving laptop, get rid of the button
	if( fExitingLaptopFlag == TRUE )
	{
		fCreateMinimizeButton = FALSE;
	}

	if( ( fAlreadyCreated == FALSE ) && ( fCreateMinimizeButton == TRUE ) )
	{
		// not created, create
		fAlreadyCreated = TRUE;
		CreateMinimizeButtonForCurrentMode( );
		CreateMinimizeRegionsForLaptopProgramIcons( );
	}
	else if(	( fAlreadyCreated == TRUE ) && ( fCreateMinimizeButton == FALSE )	)
	{
		// created and must be destroyed
		fAlreadyCreated = FALSE;
		DestroyMinimizeButtonForCurrentMode( );
		DestroyMinimizeRegionsForLaptopProgramIcons( );

	}
	else
	{
		// do nothing
	}
}


void CreateMinimizeButtonForCurrentMode( void )
{
	// create minimize button
	gLaptopMinButtonImage[0]=	LoadButtonImage( "LAPTOP\\x.sti" ,-1,0,-1,1,-1 );
	gLaptopMinButton[0] = QuickCreateButton( gLaptopMinButtonImage[0], iScreenWidthOffset + 590, iScreenHeightOffset + 30,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)LaptopMinimizeProgramButtonCallback);

	SetButtonCursor(gLaptopMinButton[0], CURSOR_LAPTOP_SCREEN);
}

void DestroyMinimizeButtonForCurrentMode( void )
{
	// destroy minimize button
	RemoveButton( gLaptopMinButton[ 0 ] );
	UnloadButtonImage( gLaptopMinButtonImage[ 0 ] );
}


void LaptopMinimizeProgramButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
	 btn->uiFlags|=(BUTTON_CLICKED_ON);
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			switch( guiCurrentLaptopMode )
			{
			case( LAPTOP_MODE_EMAIL ):
					gLaptopProgramStates[ LAPTOP_PROGRAM_MAILER ] = LAPTOP_PROGRAM_MINIMIZED;
				InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[ 0 ], guiTITLEBARICONS, 0 );
				SetCurrentToLastProgramOpened( );
					fMinizingProgram = TRUE;
					fInitTitle = TRUE;
				break;
				case( LAPTOP_MODE_FILES ):
					gLaptopProgramStates[ LAPTOP_PROGRAM_FILES ] = LAPTOP_PROGRAM_MINIMIZED;
				InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[5], guiTITLEBARICONS, 2 );
				SetCurrentToLastProgramOpened( );
					fMinizingProgram = TRUE;
					fInitTitle = TRUE;
				break;
				case( LAPTOP_MODE_FINANCES ):
					gLaptopProgramStates[ LAPTOP_PROGRAM_FINANCES ] = LAPTOP_PROGRAM_MINIMIZED;
				InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[ 2 ], guiTITLEBARICONS, 5 );
					SetCurrentToLastProgramOpened( );
					fMinizingProgram = TRUE;
					fInitTitle = TRUE;
				break;
				case( LAPTOP_MODE_HISTORY ):
					gLaptopProgramStates[ LAPTOP_PROGRAM_HISTORY ] = LAPTOP_PROGRAM_MINIMIZED;
				InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[4], guiTITLEBARICONS, 4 );
					SetCurrentToLastProgramOpened( );
					fMinizingProgram = TRUE;
					fInitTitle = TRUE;
				break;
				case( LAPTOP_MODE_PERSONNEL ):
					gLaptopProgramStates[ LAPTOP_PROGRAM_PERSONNEL ] = LAPTOP_PROGRAM_MINIMIZED;
				InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[3], guiTITLEBARICONS, 3 );
					SetCurrentToLastProgramOpened( );
					fMinizingProgram = TRUE;
					fInitTitle = TRUE;
				break;
				case( LAPTOP_MODE_NONE ):
				// nothing
				break;
				default:
					gLaptopProgramStates[ LAPTOP_PROGRAM_WEB_BROWSER ] = LAPTOP_PROGRAM_MINIMIZED;
				InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[ 7 ], guiTITLEBARICONS, 1 );
					SetCurrentToLastProgramOpened( );
					gfShowBookmarks = FALSE;
					fMinizingProgram = TRUE;
					fInitTitle = TRUE;
				break;
			}
		}
	}
}

INT32 FindLastProgramStillOpen( void )
{
	INT32 iLowestValue = 6;
	INT32 iLowestValueProgram = 6;
	INT32 iCounter =0;

	// returns ID of last program open and not minimized
	for( iCounter = 0; iCounter < 6; iCounter++ )
	{
		if( gLaptopProgramStates[ iCounter ] != LAPTOP_PROGRAM_MINIMIZED )
		{
			if( gLaptopProgramQueueList[ iCounter ] < iLowestValue )
			{
				iLowestValue = gLaptopProgramQueueList[ iCounter ];
				iLowestValueProgram = iCounter;
			}
		}
	}

	return ( iLowestValueProgram );
}

void UpdateListToReflectNewProgramOpened( INT32 iOpenedProgram )
{
	INT32 iCounter = 0;

	// will update queue of opened programs to show thier states
	// set iOpenedProgram to 1, and update others

	// increment everyone
	for( iCounter = 0; iCounter < 6; iCounter++ )
	{
			gLaptopProgramQueueList[ iCounter ]++;
	}

	gLaptopProgramQueueList[ iOpenedProgram ] = 1;

	return;
}


void InitLaptopOpenQueue( void )
{

	INT32 iCounter = 0;

	// set evereyone to 1
	for( iCounter = 0; iCounter < 6; iCounter++ )
	{
			gLaptopProgramQueueList[ iCounter ] = 1;
	}

	return;
}

void SetCurrentToLastProgramOpened( void )
{
	guiCurrentLaptopMode = LAPTOP_MODE_NONE;

	switch( FindLastProgramStillOpen( ) )
	{
		case(LAPTOP_PROGRAM_HISTORY ):
				guiCurrentLaptopMode = LAPTOP_MODE_HISTORY;
		break;
		case( LAPTOP_PROGRAM_MAILER ):
		guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
		break;
		case( LAPTOP_PROGRAM_PERSONNEL ):
			guiCurrentLaptopMode = LAPTOP_MODE_PERSONNEL;
		break;
		case( LAPTOP_PROGRAM_FINANCES ):
			guiCurrentLaptopMode = LAPTOP_MODE_FINANCES;
		break;
		case( LAPTOP_PROGRAM_FILES ):
			guiCurrentLaptopMode = LAPTOP_MODE_FILES;
		break;
		case( LAPTOP_PROGRAM_WEB_BROWSER ):
		// last www mode
			if ( guiCurrentLaptopMode == LAPTOP_MODE_BRIEFING_ROOM_PAGE || guiCurrentLaptopMode == LAPTOP_MODE_BRIEFING_ROOM || guiCurrentLaptopMode == LAPTOP_MODE_BRIEFING_ROOM_ENTER )
			{
				guiCurrentLaptopMode = LAPTOP_MODE_BRIEFING_ROOM_ENTER;
			}
#ifdef ENCYCLOPEDIA_WORKS
			else if ( guiCurrentLaptopMode == LAPTOP_MODE_ENCYCLOPEDIA_DATA || guiCurrentLaptopMode == LAPTOP_MODE_ENCYCLOPEDIA )
			{
				guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA;
			}
#endif
			else if( guiCurrentWWWMode >= LAPTOP_MODE_FINANCES && guiCurrentWWWMode  <= LAPTOP_MODE_BOBBYR_SHIPMENTS  )
			{
				guiCurrentLaptopMode = guiCurrentWWWMode;
			}
			else
			{
				guiCurrentLaptopMode = LAPTOP_MODE_WWW;
			}
			
				guiCurrentLaptopMode = LAPTOP_MODE_WWW;
			//gfShowBookmarks = TRUE;
			fShowBookmarkInfo = TRUE;
		break;
	}



}


void BlitTitleBarIcons( void )
{

	HVOBJECT hHandle;
	// will blit the icons for the title bar of the program we are in
	switch( guiCurrentLaptopMode )
	{
		case( LAPTOP_MODE_HISTORY ):
				GetVideoObject(&hHandle, guiTITLEBARICONS);
		BltVideoObject(FRAME_BUFFER, hHandle, 4,LAPTOP_TITLE_ICONS_X, LAPTOP_TITLE_ICONS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		break;
		case( LAPTOP_MODE_EMAIL ):
			GetVideoObject(&hHandle, guiTITLEBARICONS);
		BltVideoObject(FRAME_BUFFER, hHandle, 0,LAPTOP_TITLE_ICONS_X, LAPTOP_TITLE_ICONS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		break;
		case( LAPTOP_MODE_PERSONNEL ):
			GetVideoObject(&hHandle, guiTITLEBARICONS);
		BltVideoObject(FRAME_BUFFER, hHandle, 3,LAPTOP_TITLE_ICONS_X, LAPTOP_TITLE_ICONS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		break;
		case( LAPTOP_MODE_FINANCES ):
			GetVideoObject(&hHandle, guiTITLEBARICONS);
		BltVideoObject(FRAME_BUFFER, hHandle, 5,LAPTOP_TITLE_ICONS_X, LAPTOP_TITLE_ICONS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		break;
		case( LAPTOP_MODE_FILES ):
			GetVideoObject(&hHandle, guiTITLEBARICONS);
		BltVideoObject(FRAME_BUFFER, hHandle, 2,LAPTOP_TITLE_ICONS_X, LAPTOP_TITLE_ICONS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		break;
		case( LAPTOP_MODE_NONE ):
		// do nothing
		break;
		default:
			// www pages
			GetVideoObject(&hHandle, guiTITLEBARICONS);
	 BltVideoObject(FRAME_BUFFER, hHandle, 1,LAPTOP_TITLE_ICONS_X, LAPTOP_TITLE_ICONS_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		break;

	}
}


BOOLEAN DrawDeskTopBackground( void )
{
	HVSURFACE hSrcVSurface;
	UINT32 uiDestPitchBYTES;
	UINT32 uiSrcPitchBYTES;
	UINT16	*pDestBuf;
	UINT8	*pSrcBuf;
	SGPRect clip;

	// set clipping region
	// WANNE: If i change this values to SCREEN_WIDTH, ... than an exception is thrown in the vobject_blitters.cpp)
	//->no changes to clip structure
	clip.iLeft = 0;
	clip.iRight = 506;
	clip.iTop = 0;
	clip.iBottom = 427;


	// get surfaces
	pDestBuf = (UINT16*)LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES);
	CHECKF( GetVideoSurface( &hSrcVSurface, guiDESKTOP) );
	pSrcBuf = LockVideoSurface( guiDESKTOP, &uiSrcPitchBYTES);


	// blit .pcx for the background onto desktop
	Blt8BPPDataSubTo16BPPBuffer( pDestBuf,	uiDestPitchBYTES, hSrcVSurface, pSrcBuf,uiSrcPitchBYTES, LAPTOP_SCREEN_UL_X - 2, LAPTOP_SCREEN_UL_Y - 3, &clip);


	// release surfaces
	UnLockVideoSurface( guiDESKTOP );
	UnLockVideoSurface( FRAME_BUFFER );

	return ( TRUE );
}


BOOLEAN LoadDesktopBackground( void )
{
	// load desktop background
	VSURFACE_DESC		vs_desc;

	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;
	GetMLGFilename( vs_desc.ImageFile, MLG_DESKTOP );
	CHECKF(AddVideoSurface(&vs_desc, &guiDESKTOP));


	return( TRUE );
}

void DeleteDesktopBackground( void )
{
	// delete desktop

	DeleteVideoSurfaceFromIndex(guiDESKTOP);
	return;
}


void PrintBalance( void )
{
	CHAR16 pString[ 32 ];
//	UINT16 usX, usY;

	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );
	SetFontShadow( NO_SHADOW );

	swprintf(pString, L"%d", LaptopSaveInfo.iCurrentBalance);
	InsertCommasForDollarFigure( pString );
	InsertDollarSignInToString( pString );

	if( ButtonList[ gLaptopButton[ 5 ] ]->uiFlags & BUTTON_CLICKED_ON )
	{
		mprintf(iScreenWidthOffset + 48, iScreenHeightOffset + 273,pString);
	}
	else
	{
		mprintf(iScreenWidthOffset + 47, iScreenHeightOffset + 272,pString);
	}


	SetFontShadow( DEFAULT_SHADOW );
}


void PrintNumberOnTeam( void )
{
	CHAR16 pString[ 32 ];
	SOLDIERTYPE *pSoldier, *pTeamSoldier;
	INT32 cnt=0;
	INT32 iCounter=0;
	UINT16 usPosX, usPosY, usFontHeight, usStrLength;


	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );
	SetFontShadow( NO_SHADOW );

	// grab number on team
	pSoldier = MercPtrs[0];

	for ( pTeamSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ pSoldier->bTeam ].bLastID; cnt++, pTeamSoldier++)
	{
		pTeamSoldier = MercPtrs[ cnt ];

		if( ( pTeamSoldier->bActive)&&( !( pTeamSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) ) )
		{
			iCounter++;
		}
	}


	swprintf( pString, L"%s %d",pPersonnelString[ 0 ], iCounter );

	usFontHeight = GetFontHeight( FONT10ARIAL );
	usStrLength = StringPixLength( pString, FONT10ARIAL );

	if( ButtonList[ gLaptopButton[ 3 ] ]->uiFlags & BUTTON_CLICKED_ON )
	{
		usPosX = iScreenWidthOffset + 48;
		usPosY = iScreenHeightOffset + 225;
	}
	else
	{
		usPosX = iScreenWidthOffset + 47;
		usPosY = iScreenHeightOffset + 224;
	}

	mprintf( usPosX, usPosY, pString);

	SetFontShadow( DEFAULT_SHADOW );
}

void PrintDate( void )
{
	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );

	SetFontShadow( NO_SHADOW );

	mprintf( iScreenWidthOffset + 30 + (70 - StringPixLength( WORLDTIMESTR, FONT10ARIAL ))/2, iScreenHeightOffset + 433, WORLDTIMESTR );

	SetFontShadow( DEFAULT_SHADOW );

	return;
}


void DisplayTaskBarIcons()
{
	HVOBJECT hPixHandle;

	GetVideoObject(&hPixHandle, guiTITLEBARICONS);

	if( fNewFilesInFileViewer )
	{
		//display the files icon, if there is any
	BltVideoObject(FRAME_BUFFER, hPixHandle, 7,LAPTOP__NEW_FILE_ICON_X, LAPTOP__NEW_FILE_ICON_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	}

	//display the email icon, if there is email
	if( fUnReadMailFlag )
	{
		BltVideoObject(FRAME_BUFFER, hPixHandle, 6, LAPTOP__NEW_EMAIL_ICON_X, LAPTOP__NEW_EMAIL_ICON_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	}
}


void HandleKeyBoardShortCutsForLapTop( UINT16 usEvent, UINT32 usParam, UINT16 usKeyState )
{

	// will handle keyboard shortcuts for the laptop ... to be added to later

	if( ( fExitingLaptopFlag == TRUE )||( fTabHandled ) )
	{
		return;
	}

	if ( (usEvent == KEY_DOWN ) && (usParam == ESC ) )
	{
		// handle various functions of ESC key
		HandleLapTopESCKey( );
	}
	else if ( (usEvent == KEY_DOWN ) && (usParam == ENTER ) )
	{
		// handle various functions of Enter key
		HandleLapTopEnterKey( );
	}
	else if( (usEvent == KEY_DOWN ) && ( usParam == TAB ) )
	{
		if(usKeyState & CTRL_DOWN )
		{
			HandleShiftAltTabKeyInLaptop( );
		}
		else
		{
			HandleAltTabKeyInLaptop( );
		}

		fTabHandled = TRUE;
	}

#ifdef JA2TESTVERSION

	else if ((usEvent == KEY_DOWN )&& ( usParam == 'm' ) )
	{
		if( ( usKeyState & ALT_DOWN ) )
		{
				CheatToGetAll5Merc();
		}
	}
#endif

	else if( (usEvent == KEY_DOWN )&& ( usParam == 'b' ) )
	{
		if( CHEATER_CHEAT_LEVEL( ) )
		{
			if( ( usKeyState & ALT_DOWN ) )
				LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;
			else if( usKeyState & CTRL_DOWN )
			{
				guiCurrentLaptopMode = LAPTOP_MODE_BROKEN_LINK;
			}
		}
	}

	else if ((usEvent == KEY_DOWN )&& ( usParam == 'x' ) )
	{
		if( ( usKeyState & ALT_DOWN ) )
		{
		HandleShortCutExitState( );
		}
		//LeaveLapTopScreen( );
	}
#ifdef JA2TESTVERSION
	else if ((usEvent == KEY_DOWN )&& ( usParam == 'q' ))
	{
			//if we dont currently have mercs on the team, hire some
			if( NumberOfMercsOnPlayerTeam() == 0 )
			{
				UINT8 ubRand = (UINT8) Random( 2 ) + 2;
				TempHiringOfMercs( ubRand, FALSE );
			//	QuickStartGame( );
			}
			MarkButtonsDirty( );
			fExitingLaptopFlag = TRUE;
	}
	else if ((usEvent == KEY_DOWN )&& ( usParam == 's' ))
	{
		if( ( usKeyState & ALT_DOWN ) )
		{
			SetBookMark( AIM_BOOKMARK );
			SetBookMark( BOBBYR_BOOKMARK );
			SetBookMark( IMP_BOOKMARK );
			SetBookMark( MERC_BOOKMARK );
			SetBookMark( FUNERAL_BOOKMARK );
			SetBookMark( FLORIST_BOOKMARK );
#ifdef JA2UB
			if (gGameUBOptions.LaptopLinkInsurance == TRUE )
				SetBookMark( INSURANCE_BOOKMARK );
#else
			SetBookMark( INSURANCE_BOOKMARK );
#endif
			SetBookMark( CAMPAIGNHISTORY_BOOKMARK );
			SetBookMark( MERCCOMPARE_BOOKMARK );
			SetBookMark( WHO_BOOKMARK );
			SetBookMark( PMC_BOOKMARK );
			SetBookMark( MILITIAROSTER_BOOKMARK );
			SetBookMark( INTELMARKET_BOOKMARK );
			SetBookMark( PRODUCTION_BOOKMARK );
		}
	}

	//help screen stuff
	else
#endif
	if( (usEvent == KEY_DOWN ) && ( ( usParam == 'h' ) || ( usParam == 'H' ) ) )
	{
		ShouldTheHelpScreenComeUp( HELP_SCREEN_LAPTOP, TRUE );
	}


#if defined ( JA2TESTVERSION )
	else if ((usEvent == KEY_DOWN )&& ( usParam == 'w' ))
	{
		DemoHiringOfMercs( );
	}
#endif

#ifdef JA2BETAVERSION
	//adding all emails
	else if ((usEvent == KEY_DOWN )&& ( usParam == 'e' ))
	{
		if( CHEATER_CHEAT_LEVEL( ) )
		{
			if( ( usKeyState & ALT_DOWN ) )
			{
				AddAllEmails();
			}
		}
	}
#endif

	//adding money
	else if ((usEvent == KEY_DOWN )&& ( usParam == '=' ))
	{
		if( CHEATER_CHEAT_LEVEL( ) )
		{
			AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), 10000 );
			MarkButtonsDirty( );
		}
	}

	//adding lots of money
	else if ((usEvent == KEY_DOWN )&& ( usParam == '+' ))
	{
		if( CHEATER_CHEAT_LEVEL( ) )
		{
			AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), 100000 );
			MarkButtonsDirty( );
		}
	}

	//subtracting money
	else if( (usEvent == KEY_DOWN ) && ( usParam == '-' ) )
	{
		if( CHEATER_CHEAT_LEVEL( ) )
		{
			AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), -10000 );
			MarkButtonsDirty( );
		}
	}
	
	//subtracting lots of money
	else if( (usEvent == KEY_DOWN ) && ( usParam == '_' ) )
	{
		if( CHEATER_CHEAT_LEVEL( ) )
		{
			AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), -100000 );
			MarkButtonsDirty( );
		}
	}
	else if( ( ( usEvent == KEY_DOWN ) || ( usEvent == KEY_REPEAT ) ) && ( usParam == 'z' ) )
	{
		if ( usKeyState & CTRL_DOWN )
		{
			if( IsCursorRestricted( ) )
			{
				FreeMouseCursor( FALSE );
			}
			else
			{
				RestrictMouseCursor( &LaptopScreenRect );
			}

		}
	}

#ifdef JA2TESTVERSION
	else if ((usEvent == KEY_DOWN )&& ( usParam == 'd' ))
	{
		if( gfTemporaryDisablingOfLoadPendingFlag )
			gfTemporaryDisablingOfLoadPendingFlag = FALSE;
		else
			gfTemporaryDisablingOfLoadPendingFlag = TRUE;
	}
	else if ((usEvent == KEY_DOWN ) && ( usParam == '+' ))
	{
		if( usKeyState & ALT_DOWN )
		{
			gStrategicStatus.ubHighestProgress += 10;
			if (gStrategicStatus.ubHighestProgress > 100)
				gStrategicStatus.ubHighestProgress = 100;

			InitAllArmsDealers();
			InitBobbyRayInventory();
		}
	}
	else if ((usEvent == KEY_DOWN ) && ( usParam == '-' ))
	{
		if( usKeyState & ALT_DOWN )
		{
			if (gStrategicStatus.ubHighestProgress >= 10)
				gStrategicStatus.ubHighestProgress -= 10;
			else
				gStrategicStatus.ubHighestProgress = 0;

			InitAllArmsDealers();
			InitBobbyRayInventory();
		}
	}
	else if ((usEvent == KEY_DOWN ) && ( usParam == '*' ))
	{
		if( usKeyState & ALT_DOWN )
		{
			DeleteAllStrategicEventsOfType( EVENT_EVALUATE_QUEEN_SITUATION );
			AdvanceToNextDay();
		}
	}
	else if( usEvent == KEY_DOWN && usParam == 'v' && usKeyState & CTRL_DOWN )
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
		if ((usEvent == KEY_DOWN )&& ( usParam == '1' ))
		{
			TempHiringOfMercs( 1, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '2' ))
		{
			TempHiringOfMercs( 2, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '3' ))
		{
			TempHiringOfMercs( 3, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '4' ))
		{
			TempHiringOfMercs( 4, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '5' ))
		{
			TempHiringOfMercs( 5, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '6' ))
		{
			TempHiringOfMercs( 6, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '7' ))
		{
			TempHiringOfMercs( 7, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '8' ))
		{
			TempHiringOfMercs( 8, FALSE);
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '9' ))
		{
			TempHiringOfMercs( 9, FALSE );
		}

		if ((usEvent == KEY_DOWN )&& ( usParam == '0' ))
		{
			TempHiringOfMercs( 10, FALSE );
		}
	}
#endif

		return;
}


BOOLEAN RenderWWWProgramTitleBar( void )
{
	// will render the title bar for the www program
	UINT32 uiTITLEFORWWW;
	HVOBJECT hHandle;
	VOBJECT_DESC VObjectDesc;
	INT32 iIndex = 0;
	CHAR16 sString[256], sTemp[256];

	// title bar - load
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\programtitlebar.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &uiTITLEFORWWW));

	// blit title
	GetVideoObject(&hHandle, uiTITLEFORWWW);
	BltVideoObject( FRAME_BUFFER, hHandle, 0,LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_UL_Y - 2, VO_BLT_SRCTRANSPARENCY,NULL );
	
	// now delete
	DeleteVideoObjectFromIndex( uiTITLEFORWWW );

	// now slapdown text
	SetFont( FONT14ARIAL );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	// display title

	// no page loaded yet, do not handle yet

	if( guiCurrentLaptopMode == LAPTOP_MODE_WWW )
	{
		mprintf(iScreenWidthOffset + 140 , iScreenHeightOffset + 33 ,pWebTitle[0]);
	}
	else
	{
		iIndex = guiCurrentLaptopMode - LAPTOP_MODE_WWW-1;

		if( iIndex >= 33 && iIndex <= 36 )
		{
			swprintf( sTemp, pWebPagesTitles[ iIndex ], pCountryNames[COUNTRY_NAME] );
			swprintf( sString, L"%s - %s", pWebTitle[0], sTemp );
		}
		else 
			swprintf( sString, L"%s - %s", pWebTitle[0], pWebPagesTitles[ iIndex ] );
		mprintf(iScreenWidthOffset + 140 ,iScreenHeightOffset + 33 ,sString);
	}
	
	BlitTitleBarIcons( );

	DisplayProgramBoundingBox( FALSE );

	//InvalidateRegion( 0, 0, 640, 480 );
	return( TRUE );
}


void HandleDefaultWebpageForLaptop( void )
{
	// go to first page in bookmark list
	if( guiCurrentLaptopMode == LAPTOP_MODE_WWW )
	{
		// if valid entry go there
		if( LaptopSaveInfo.iBookMarkList[ 0 ] != -1 )
		{
			GoToWebPage( LaptopSaveInfo.iBookMarkList[ 0 ] );
		}
	}
}


void CreateMinimizeRegionsForLaptopProgramIcons( void )
{
	// will create the minizing region to lie over the icon for this particular laptop program

	MSYS_DefineRegion( &gLapTopProgramMinIcon,LAPTOP_PROGRAM_ICON_X, LAPTOP_PROGRAM_ICON_Y ,LAPTOP_PROGRAM_ICON_X + LAPTOP_PROGRAM_ICON_WIDTH ,LAPTOP_PROGRAM_ICON_Y + LAPTOP_PROGRAM_ICON_HEIGHT, MSYS_PRIORITY_NORMAL+1,
							CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, LaptopProgramIconMinimizeCallback );
}


void DestroyMinimizeRegionsForLaptopProgramIcons( void )
{
	// will destroy the minizmize regions to be placed over the laptop icons that will be
	// displayed on the top of the laptop program bar

	MSYS_RemoveRegion( &gLapTopProgramMinIcon);
}


void LaptopProgramIconMinimizeCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	// callback handler for the minize region that is attatched to the laptop program icon
	if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		switch( guiCurrentLaptopMode )
		{
		case( LAPTOP_MODE_EMAIL ):
			gLaptopProgramStates[ LAPTOP_PROGRAM_MAILER ] = LAPTOP_PROGRAM_MINIMIZED;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[0], guiTITLEBARICONS, 0 );
			SetCurrentToLastProgramOpened( );
			fMinizingProgram = TRUE;
			fInitTitle = TRUE;
			break;

		case( LAPTOP_MODE_FILES ):
			gLaptopProgramStates[ LAPTOP_PROGRAM_FILES ] = LAPTOP_PROGRAM_MINIMIZED;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[5], guiTITLEBARICONS, 2 );
			SetCurrentToLastProgramOpened( );
			fMinizingProgram = TRUE;
			fInitTitle = TRUE;
			break;

		case( LAPTOP_MODE_FINANCES ):
			gLaptopProgramStates[ LAPTOP_PROGRAM_FINANCES ] = LAPTOP_PROGRAM_MINIMIZED;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[2], guiTITLEBARICONS, 5 );
			SetCurrentToLastProgramOpened( );
			fMinizingProgram = TRUE;
			fInitTitle = TRUE;
			break;

		case( LAPTOP_MODE_HISTORY ):
			gLaptopProgramStates[ LAPTOP_PROGRAM_HISTORY ] = LAPTOP_PROGRAM_MINIMIZED;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[4], guiTITLEBARICONS, 4 );
			SetCurrentToLastProgramOpened( );
			fMinizingProgram = TRUE;
			fInitTitle = TRUE;
			break;

		case( LAPTOP_MODE_PERSONNEL ):
			gLaptopProgramStates[ LAPTOP_PROGRAM_PERSONNEL ] = LAPTOP_PROGRAM_MINIMIZED;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pLaptopIcons[3], guiTITLEBARICONS, 3 );
			SetCurrentToLastProgramOpened( );
			fMinizingProgram = TRUE;
			fInitTitle = TRUE;
			break;

		case( LAPTOP_MODE_NONE ):
			// nothing
			break;

		default:
			gLaptopProgramStates[ LAPTOP_PROGRAM_WEB_BROWSER ] = LAPTOP_PROGRAM_MINIMIZED;
			InitTitleBarMaximizeGraphics(guiTITLEBARLAPTOP, pWebTitle[ 0 ], guiTITLEBARICONS, 1 );
			SetCurrentToLastProgramOpened( );
			gfShowBookmarks = FALSE;
			fShowBookmarkInfo = FALSE;
			fMinizingProgram = TRUE;
			fInitTitle = TRUE;
			break;
		}
	}
}


void DisplayProgramBoundingBox( BOOLEAN fMarkButtons )
{
	// the border fot eh program
	HVOBJECT hHandle;

	GetVideoObject(&hHandle, guiLaptopBACKGROUND);
	BltVideoObject(FRAME_BUFFER, hHandle, 1, iScreenWidthOffset + 25, iScreenHeightOffset + 23, VO_BLT_SRCTRANSPARENCY,NULL);

	// no laptop mode, no border around the program
	if( guiCurrentLaptopMode != LAPTOP_MODE_NONE )
	{
		GetVideoObject(&hHandle, guiLaptopBACKGROUND);
		BltVideoObject(FRAME_BUFFER, hHandle, 0, iScreenWidthOffset + 108, iScreenHeightOffset + 23, VO_BLT_SRCTRANSPARENCY,NULL);
 	}

	if( fMarkButtons || fLoadPendingFlag )
	{
		MarkButtonsDirty( );
		RenderButtons( );
	}
	
	PrintDate( );

	PrintBalance( );

	PrintNumberOnTeam( );

	// new files or email?
	DisplayTaskBarIcons( );
}


void CreateDestroyMouseRegionForNewMailIcon( void )
{
	static BOOLEAN fCreated = FALSE;

	//. will toggle creation/destruction of the mouse regions used by the new mail icon

	if( fCreated == FALSE )
	{
		fCreated = TRUE;
		MSYS_DefineRegion(&gNewMailIconRegion, LAPTOP__NEW_EMAIL_ICON_X, LAPTOP__NEW_EMAIL_ICON_Y + 5,	LAPTOP__NEW_EMAIL_ICON_X+16,	LAPTOP__NEW_EMAIL_ICON_Y + 16,
		 MSYS_PRIORITY_HIGHEST - 3 , CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, NewEmailIconCallback);
		CreateFileAndNewEmailIconFastHelpText( LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_MAIL, (BOOLEAN)(fUnReadMailFlag == 0 ) );

		MSYS_DefineRegion(&gNewFileIconRegion, LAPTOP__NEW_FILE_ICON_X, LAPTOP__NEW_FILE_ICON_Y + 5,	LAPTOP__NEW_FILE_ICON_X + 16,	LAPTOP__NEW_FILE_ICON_Y + 16,
		 MSYS_PRIORITY_HIGHEST - 3 , CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, NewFileIconCallback);
		CreateFileAndNewEmailIconFastHelpText( LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_FILE, (BOOLEAN)( fNewFilesInFileViewer == 0 ) );
	}
	else
	{
		fCreated = FALSE;
		MSYS_RemoveRegion( &gNewMailIconRegion );
		MSYS_RemoveRegion( &gNewFileIconRegion );
	}
}


void NewEmailIconCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( fUnReadMailFlag )
		{
			fOpenMostRecentUnReadFlag = TRUE;
			guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
		}
	}
}


void NewFileIconCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( fNewFilesInFileViewer )
		{
			fEnteredFileViewerFromNewFileIcon = TRUE;
			guiCurrentLaptopMode = LAPTOP_MODE_FILES;
		}
	}
}

void HandleWWWSubSites( void )
{
	// check to see if WW Wait is needed for a sub site within the Web Browser

	if( (guiCurrentLaptopMode == guiPreviousLaptopMode )||( guiCurrentLaptopMode < LAPTOP_MODE_WWW) ||( fLoadPendingFlag == TRUE ) ||( fDoneLoadPending == TRUE ) || ( guiPreviousLaptopMode < LAPTOP_MODE_WWW ) )
	{
		// no go, leave
		return;
	}

	fLoadPendingFlag = TRUE;
	fConnectingToSubPage = TRUE;

	// fast or slow load?
	if( gfWWWaitSubSitesVisitedFlags[ guiCurrentLaptopMode ] == TRUE )
	{
		fFastLoadFlag = TRUE;
	}

	// set fact we were here
	gfWWWaitSubSitesVisitedFlags[ guiCurrentLaptopMode ] = TRUE;

	//Dont show the dlownload screen when switching between these pages
	if( ( guiCurrentLaptopMode == LAPTOP_MODE_AIM_MEMBERS ) && ( guiPreviousLaptopMode == LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX ) ||
			( guiCurrentLaptopMode == LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX ) && ( guiPreviousLaptopMode == LAPTOP_MODE_AIM_MEMBERS ) )
	{
		fFastLoadFlag = FALSE;
		fLoadPendingFlag = FALSE;

		// set fact we were here
		gfWWWaitSubSitesVisitedFlags[LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX] = TRUE;
		gfWWWaitSubSitesVisitedFlags[LAPTOP_MODE_AIM_MEMBERS] = TRUE;
	}
}


void UpdateStatusOfDisplayingBookMarks( void )
{
	// this function will disable showing of bookmarks if in process of download or if we miniming web browser
	if ( (fLoadPendingFlag == TRUE) || (guiCurrentLaptopMode < LAPTOP_MODE_WWW) )
	{
		gfShowBookmarks = FALSE;
	}
}


void InitalizeSubSitesList( void )
{
	// init all subsites list to not visited
	for ( INT32 iCounter = LAPTOP_MODE_WWW + 1; iCounter < LAPTOP_MODE_MAX; ++iCounter )
	{
		gfWWWaitSubSitesVisitedFlags[iCounter] = FALSE;
	}
}


void SetSubSiteAsVisted( void )
{
	// sets a www sub site as visited
	if( guiCurrentLaptopMode <= LAPTOP_MODE_WWW )
	{
		// not at a web page yet
	}
	else
	{
		gfWWWaitSubSitesVisitedFlags[ guiCurrentLaptopMode ] = TRUE;
	}
}


void HandleShiftAltTabKeyInLaptop( void )
{
	// will handle the alt tab keying in laptop

	// move to next program
	if( fMaximizingProgram == TRUE )
	{
		return;
	}

	switch( guiCurrentLaptopMode )
	{
		case( LAPTOP_MODE_FINANCES ):
			guiCurrentLaptopMode = LAPTOP_MODE_PERSONNEL;
		break;
		case( LAPTOP_MODE_PERSONNEL ):
			guiCurrentLaptopMode = LAPTOP_MODE_HISTORY;
		break;
		case( LAPTOP_MODE_HISTORY ):
			guiCurrentLaptopMode = LAPTOP_MODE_FILES;
		break;
		case( LAPTOP_MODE_EMAIL ):
			guiCurrentLaptopMode = LAPTOP_MODE_FINANCES;
		break;
		case( LAPTOP_MODE_FILES ):
			guiCurrentLaptopMode = LAPTOP_MODE_WWW;
		break;
		case( LAPTOP_MODE_NONE ):
			guiCurrentLaptopMode = LAPTOP_MODE_FINANCES;
		break;
		case( LAPTOP_MODE_WWW ):
			guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
		break;
		default:
			guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
		break;
	}

	fPausedReDrawScreenFlag = TRUE;
}


void HandleAltTabKeyInLaptop( void )
{
	// will handle the alt tab keying in laptop

	// move to next program
	// move to next program
	if( fMaximizingProgram == TRUE )
	{
		return;
	}

	switch( guiCurrentLaptopMode )
	{
		case( LAPTOP_MODE_FINANCES ):
			guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
		break;
		case( LAPTOP_MODE_PERSONNEL ):
			guiCurrentLaptopMode = LAPTOP_MODE_FINANCES;
		break;

		case( LAPTOP_MODE_HISTORY ):
			guiCurrentLaptopMode = LAPTOP_MODE_PERSONNEL;
		break;
		case( LAPTOP_MODE_EMAIL ):
			guiCurrentLaptopMode = LAPTOP_MODE_WWW;
		break;
		case( LAPTOP_MODE_FILES ):
			guiCurrentLaptopMode = LAPTOP_MODE_HISTORY;
		break;
		case( LAPTOP_MODE_NONE ):
			guiCurrentLaptopMode = LAPTOP_MODE_EMAIL;
		break;
		default:
			guiCurrentLaptopMode = LAPTOP_MODE_FILES;
		break;
	}

	fPausedReDrawScreenFlag = TRUE;
}



// display the 2 second book mark instruction
void DisplayWebBookMarkNotify( void )
{
	static BOOLEAN fOldShow = FALSE;
	HVOBJECT hLapTopIconHandle;


	// handle the timer for this thing
	HandleWebBookMarkNotifyTimer( );

	// are we about to start showing box? or did we just stop?
	if( ( ( fOldShow == FALSE )||( fReDrawBookMarkInfo ) ) && ( fShowBookmarkInfo == TRUE ) )
	{
		fOldShow = TRUE;
		fReDrawBookMarkInfo = FALSE;

		// show background objects
		GetVideoObject(&hLapTopIconHandle, guiDOWNLOADTOP);
	BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,DOWNLOAD_X, DOWNLOAD_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	GetVideoObject(&hLapTopIconHandle, guiDOWNLOADMID);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,DOWNLOAD_X, DOWNLOAD_Y+DOWN_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);
		GetVideoObject(&hLapTopIconHandle, guiDOWNLOADBOT);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 0,DOWNLOAD_X, DOWNLOAD_Y+2*DOWN_HEIGHT, VO_BLT_SRCTRANSPARENCY,NULL);
		GetVideoObject(&hLapTopIconHandle, guiTITLEBARICONS);
		BltVideoObject(FRAME_BUFFER, hLapTopIconHandle, 1,DOWNLOAD_X + 4, DOWNLOAD_Y + 1, VO_BLT_SRCTRANSPARENCY,NULL);

		// font stuff
		SetFont(DOWNLOAD_FONT);
		SetFontForeground(FONT_WHITE);
		SetFontBackground(FONT_BLACK);
		SetFontShadow(NO_SHADOW);

		// display download string
		mprintf( DOWN_STRING_X, DOWN_STRING_Y, pShowBookmarkString[ 0 ]);

		SetFont(BOOK_FONT);
		SetFontForeground(FONT_BLACK);
		SetFontBackground(FONT_BLACK);
		SetFontShadow(NO_SHADOW);

		// now draw the message
		DisplayWrappedString(( INT16 )( DOWN_STRING_X - 42 ),(UINT16)(DOWN_STRING_Y + 20), BOOK_WIDTH + 45, 2,BOOK_FONT,FONT_BLACK, pShowBookmarkString[ 1 ],FONT_BLACK,FALSE,CENTER_JUSTIFIED);

		// invalidate region
		InvalidateRegion(DOWNLOAD_X, DOWNLOAD_Y, DOWNLOAD_X + 150, DOWNLOAD_Y + 100);

	}
	else if( ( fOldShow == TRUE ) && ( fShowBookmarkInfo == FALSE ) )
	{
		//MSYS_RemoveRegion( &gLapTopScreenRegion );
		fOldShow = FALSE;
		fPausedReDrawScreenFlag = TRUE;
	}

	SetFontShadow(DEFAULT_SHADOW);
}

void HandleWebBookMarkNotifyTimer( void )
{
	static INT32 iBaseTime = 0;
	INT32 iDifference = 0;
	static BOOLEAN fOldShowBookMarkInfo = FALSE;

	// check if maxing or mining?
	if( ( fMaximizingProgram == TRUE ) || ( fMinizingProgram == TRUE ) )
	{
		fOldShowBookMarkInfo |= fShowBookmarkInfo;
		fShowBookmarkInfo = FALSE;
		return;
	}

		// if we were going to show this pop up, but were delayed, then do so now
	fShowBookmarkInfo |= fOldShowBookMarkInfo;

	// reset old flag
	fOldShowBookMarkInfo = FALSE;

	// if current mode is too low, then reset
	if( guiCurrentLaptopMode < LAPTOP_MODE_WWW )
	{
		fShowBookmarkInfo = FALSE;
	}

	// if showing bookmarks, don't show help
	if( gfShowBookmarks == TRUE )
	{
		fShowBookmarkInfo = FALSE;
	}

	// check if flag false, is so, leave
	if( fShowBookmarkInfo == FALSE )
	{
		iBaseTime = 0;
		return;
	}

	// check if this is the first time in here
	if( iBaseTime == 0 )
	{
		iBaseTime = GetJA2Clock( );
		return;
	}

	iDifference = GetJA2Clock( ) - iBaseTime;

	fReDrawBookMarkInfo = TRUE;

	if( iDifference > DISPLAY_TIME_FOR_WEB_BOOKMARK_NOTIFY )
	{
		// waited long enough, stop showing
		iBaseTime = 0;
		fShowBookmarkInfo = FALSE;
	}
}


void ClearOutTempLaptopFiles( void )
{
	// clear out all temp files from laptop

	// file file
	if (	(FileExists( "files.dat" ) == TRUE ) )
	{
		FileDelete( "files.dat" );
	}

	// finances
	if (	(FileExists( "finances.dat" ) == TRUE ) )
	{
		FileDelete( "finances.dat" );
	}

	// email
	if (	(FileExists( "email.dat" ) == TRUE ) )
	{
		FileDelete( "email.dat" );
	}

	// history
	if (	(FileExists( "history.dat" ) == TRUE ) )
	{
		FileDelete( "history.dat" );
	}
}


BOOLEAN SaveLaptopInfoToSavedGame( HWFILE hFile )
{
	UINT32	uiNumBytesWritten=0;
	UINT32	uiSize;

	// Save The laptop information
	FileWrite( hFile, &LaptopSaveInfo, sizeof( LaptopSaveInfoStruct ), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( LaptopSaveInfoStruct ) )
	{
		return(FALSE);
	}

	//If there is anything in the Bobby Ray Orders on Delivery
	if( LaptopSaveInfo.usNumberOfBobbyRayOrderUsed )
	{
		//Allocate memory for the information
		uiSize = sizeof( BobbyRayOrderStruct ) * LaptopSaveInfo.usNumberOfBobbyRayOrderItems;

		// Load The laptop information
		FileWrite( hFile, LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray, uiSize, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiSize)
		{
			return(FALSE);
		}
	}

	//If there is any Insurance Payouts in progress
	if( LaptopSaveInfo.ubNumberLifeInsurancePayoutUsed )
	{
		//Allocate memory for the information
		uiSize = sizeof( LIFE_INSURANCE_PAYOUT ) * LaptopSaveInfo.ubNumberLifeInsurancePayouts;

		// Load The laptop information
		FileWrite( hFile, LaptopSaveInfo.pLifeInsurancePayouts, uiSize, &uiNumBytesWritten );
		if( uiNumBytesWritten != uiSize )
		{
			return(FALSE);
		}
	}

	return( TRUE );
}




BOOLEAN LoadLaptopInfoFromSavedGame( HWFILE hFile )
{
	UINT32	uiNumBytesRead=0;
	UINT32	uiSize;

	//if there is memory allocated for the BobbyR orders
	if( LaptopSaveInfo.usNumberOfBobbyRayOrderItems )
	{
		//Free the memory
		if( LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray )
			MemFree( LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray );
		LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray = NULL;
	}

	//if there is memory allocated for life insurance payouts
	if( LaptopSaveInfo.ubNumberLifeInsurancePayouts )
	{
		if( !LaptopSaveInfo.pLifeInsurancePayouts )
			Assert( 0 );	//Should never happen

		//Free the memory
		MemFree( LaptopSaveInfo.pLifeInsurancePayouts );
		LaptopSaveInfo.pLifeInsurancePayouts = NULL;
	}

	// Load The laptop information
	FileRead( hFile, &LaptopSaveInfo, sizeof( LaptopSaveInfoStruct ), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( LaptopSaveInfoStruct ) )
	{
		return(FALSE);
	}

	//If there is anything in the Bobby Ray Orders on Delivery
	if( LaptopSaveInfo.usNumberOfBobbyRayOrderUsed )
	{
		//Allocate memory for the information
		uiSize = sizeof( BobbyRayOrderStruct ) * LaptopSaveInfo.usNumberOfBobbyRayOrderItems;

		LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray = (BobbyRayOrderStruct *) MemAlloc( uiSize );
		Assert( LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray );

		// Load The laptop information
		FileRead( hFile, LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray, uiSize, &uiNumBytesRead );
		if( uiNumBytesRead != uiSize )
		{
			return(FALSE);
		}
	}
	else
	{
		LaptopSaveInfo.usNumberOfBobbyRayOrderItems = 0;
		LaptopSaveInfo.BobbyRayOrdersOnDeliveryArray = NULL;
	}
	
	//If there is any Insurance Payouts in progress
	if( LaptopSaveInfo.ubNumberLifeInsurancePayoutUsed )
	{
		//Allocate memory for the information
		uiSize = sizeof( LIFE_INSURANCE_PAYOUT ) * LaptopSaveInfo.ubNumberLifeInsurancePayouts;

		LaptopSaveInfo.pLifeInsurancePayouts = (LIFE_INSURANCE_PAYOUT *) MemAlloc( uiSize );
		Assert( LaptopSaveInfo.pLifeInsurancePayouts );

		// Load The laptop information
		FileRead( hFile, LaptopSaveInfo.pLifeInsurancePayouts, uiSize, &uiNumBytesRead );
		if( uiNumBytesRead != uiSize )
		{
			return(FALSE);
		}
	}
	else
	{
		LaptopSaveInfo.ubNumberLifeInsurancePayouts = 0;
		LaptopSaveInfo.pLifeInsurancePayouts = NULL;
	}
	
	return( TRUE );
}

void LaptopSaveVariablesInit()
{
}

/*BOOLEAN IsItRaining()
{
	if ( (SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_RAIN || SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_THUNDERSHOWERS 
		|| SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_SANDSTORM || SectorInfo[SECTOR( gWorldSectorX, gWorldSectorY )].usWeather == WEATHER_FORECAST_SNOW) )
		return( TRUE );
	
	return( FALSE );
}*/


void		InternetRainDelayMessageBoxCallBack( UINT8 bExitValue )
{
	//GoToWebPage(	giRainDelayInternetSite );

	//Set to -2 so we dont due the message for this occurence of laptop
	//giRainDelayInternetSite = -2;
}


void CreateBookMarkHelpText( MOUSE_REGION *pRegion, UINT32 uiBookMarkID )
{
	SetRegionFastHelpText( pRegion, gzLaptopHelpText[ BOOKMARK_TEXT_ASSOCIATION_OF_INTERNATION_MERCENARIES + uiBookMarkID ] );
}

void CreateFileAndNewEmailIconFastHelpText( UINT32 uiHelpTextID, BOOLEAN fClearHelpText )
{
	MOUSE_REGION *pRegion;

	switch( uiHelpTextID )
	{
		case LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_MAIL:
			pRegion = &gNewMailIconRegion;
			break;

		case LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_FILE:
			pRegion = &gNewFileIconRegion;
			break;

		default:
			Assert(0);
			return;
	}

	if( fClearHelpText )
		SetRegionFastHelpText( pRegion, L"" );
	else
		SetRegionFastHelpText( pRegion, gzLaptopHelpText[ uiHelpTextID ] );

	//fUnReadMailFlag
	//fNewFilesInFileViewer
}


void CreateLaptopButtonHelpText( INT32 iButtonIndex, UINT32 uiButtonHelpTextID )
{
	SetButtonFastHelpText( iButtonIndex, gzLaptopHelpText[ uiButtonHelpTextID ] );
}
#ifdef JA2UB
//ja25 ub
void ShouldImpReminderEmailBeSentWhenLaptopBackOnline()
{
	//if this is past the point of when the IMP email should have been sent
	if( GetWorldTotalMin() > LAPTOP__HAVENT_CREATED_IMP_REMINDER_EMAIL_ARRIVE_TIME )
	{
		//and the email hasnt been sent
		HaventMadeImpMercEmailCallBack();
	}
}
#endif
