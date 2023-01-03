#ifndef __LAPTOP_H
#define __LAPTOP_H

#include "types.h"
#include "sgp.h"
#include "Font Control.h"
#include "messageboxscreen.h"





INT32			EnterLaptop();
void			ExitLaptop();
void			RenderLaptop();
UINT32		ExitLaptopMode(UINT32 uiMode);
void			EnterNewLaptopMode();
UINT32		DrawLapTopIcons();
UINT32		DrawLapTopText();
void			ReDrawHighLight();
void			DrawButtonText();
void			InitBookMarkList();
void			SetBookMark(INT32 iBookId);
void			RemoveBookmark(INT32 iBookId);
BOOLEAN			IsBookMarkSet( INT32 iBookId );
BOOLEAN		LeaveLapTopScreen( );
void			SetLaptopExitScreen( UINT32 uiExitScreen );
void			SetLaptopNewGameFlag( );
UINT32		DrawLapTopIcons( );
void			LapTopScreenCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void			HandleRightButtonUpEvent( void );

BOOLEAN		DoLapTopMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback );
void			GoToWebPage(INT32 iPageId);
BOOLEAN		WebPageTileBackground(UINT8 ubNumX, UINT8 ubNumY, UINT16 usWidth, UINT16 usHeight, UINT32 uiBackGround);
void			BlitTitleBarIcons( void );
void			HandleKeyBoardShortCutsForLapTop( UINT16 usEvent, UINT32 usParam, UINT16 usKeyState );
BOOLEAN		RenderWWWProgramTitleBar( void );
void			DisplayProgramBoundingBox( BOOLEAN fMarkButtons );
BOOLEAN		DoLapTopSystemMessageBox( UINT8 ubStyle, STR16 zString, UINT32 uiExitScreen, UINT32 usFlags, MSGBOX_CALLBACK ReturnCallback );
BOOLEAN		RemoveBookMark( INT32 iBookId );
void			CreateFileAndNewEmailIconFastHelpText( UINT32 uiHelpTextID, BOOLEAN fClearHelpText );
BOOLEAN		InitLaptopAndLaptopScreens();
void HandleLapTopESCKey( void );

// clear out all tempf iles from laptop
void ClearOutTempLaptopFiles( void );

void HaventMadeImpMercEmailCallBack();

extern INT32 guiCurrentGunFilterMode;
extern INT32 guiPrevGunFilterMode;

extern INT32 guiCurrentAmmoFilterMode;
extern INT32 guiPrevAmmoFilterMode;

extern INT32 guiCurrentUsedFilterMode;
extern INT32 guiPrevUsedFilterMode;

extern INT32 guiCurrentArmourFilterMode;
extern INT32 guiPrevArmourFilterMode;

extern INT32 guiCurrentMiscFilterMode;
extern INT32 guiPrevMiscFilterMode;
extern INT32 guiCurrentMiscSubFilterMode;
extern INT32 guiPrevMiscSubFilterMode;

extern BOOLEAN gbMessageDisplayed;

extern UINT32 guiCurrentLaptopMode;
extern UINT32 guiPreviousLaptopMode;
extern INT32	giCurrentSubPage;
extern UINT32 guiCurrentLapTopCursor;
extern UINT32 guiPreviousLapTopCursor;
extern BOOLEAN fMarkButtonsDirtyFlag;
extern BOOLEAN fNotVistedImpYet;
extern BOOLEAN fReDrawScreenFlag;
extern BOOLEAN fPausedReDrawScreenFlag;		
extern BOOLEAN fIntermediateReDrawFlag;
extern BOOLEAN fLoadPendingFlag;
extern BOOLEAN fEscKeyHandled;
extern BOOLEAN fReDrawPostButtonRender;
extern BOOLEAN fCurrentlyInLaptop;
//extern BOOLEAN gfNewGameLaptop;
extern UINT32 guiLaptopBACKGROUND;
extern UINT32 guiTITLEBARICONS;
extern BOOLEAN fDoneLoadPending;
extern BOOLEAN fReConnectingFlag;
extern BOOLEAN fConnectingToSubPage;
extern BOOLEAN fFastLoadFlag;
extern BOOLEAN gfShowBookmarks;
extern BOOLEAN fShowBookmarkInfo;
extern BOOLEAN fReDrawBookMarkInfo;


enum
{
	LAPTOP_MODE_NONE = 0,
	LAPTOP_MODE_FINANCES,
	LAPTOP_MODE_PERSONNEL,
	LAPTOP_MODE_HISTORY,
	LAPTOP_MODE_FILES,
	LAPTOP_MODE_FILES_ENRICO,
	LAPTOP_MODE_FILES_PLANS,
	LAPTOP_MODE_EMAIL,
	LAPTOP_MODE_EMAIL_NEW,
	LAPTOP_MODE_EMAIL_VIEW,
	LAPTOP_MODE_WWW,
	LAPTOP_MODE_AIM,
	LAPTOP_MODE_AIM_MEMBERS,
	LAPTOP_MODE_AIM_MEMBERS_FACIAL_INDEX,
	LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES,
	LAPTOP_MODE_AIM_MEMBERS_SORTED_FILES_VIDEO,
	LAPTOP_MODE_AIM_MEMBERS_ARCHIVES,
	LAPTOP_MODE_AIM_POLICIES,
	LAPTOP_MODE_AIM_HISTORY,
	LAPTOP_MODE_AIM_LINKS,
	LAPTOP_MODE_MERC,
	LAPTOP_MODE_MERC_ACCOUNT,
	LAPTOP_MODE_MERC_NO_ACCOUNT,
	LAPTOP_MODE_MERC_FILES,
	LAPTOP_MODE_BOBBY_R,
	LAPTOP_MODE_BOBBY_R_GUNS,
	LAPTOP_MODE_BOBBY_R_AMMO,
	LAPTOP_MODE_BOBBY_R_ARMOR,
	LAPTOP_MODE_BOBBY_R_MISC,
	LAPTOP_MODE_BOBBY_R_USED,
	LAPTOP_MODE_BOBBY_R_MAILORDER,
	LAPTOP_MODE_CHAR_PROFILE,
	LAPTOP_MODE_CHAR_PROFILE_QUESTIONAIRE,
	LAPTOP_MODE_FLORIST,
	LAPTOP_MODE_FLORIST_FLOWER_GALLERY,
	LAPTOP_MODE_FLORIST_ORDERFORM,
	LAPTOP_MODE_FLORIST_CARD_GALLERY,
	LAPTOP_MODE_INSURANCE,
	LAPTOP_MODE_INSURANCE_INFO,
	LAPTOP_MODE_INSURANCE_CONTRACT,
	LAPTOP_MODE_INSURANCE_COMMENTS,
	LAPTOP_MODE_FUNERAL,
	LAPTOP_MODE_SIRTECH,
	LAPTOP_MODE_BROKEN_LINK,

	// Flugente: Campaign History
	LAPTOP_MODE_CAMPAIGNHISTORY_SUMMARY,
	LAPTOP_MODE_CAMPAIGNHISTORY_MOSTIMPORTANT,
	LAPTOP_MODE_CAMPAIGNHISTORY_LATESTNEWS,
	LAPTOP_MODE_CAMPAIGNHISTORY_ABOUTTUS,

	// Flugente: MercCompare
	LAPTOP_MODE_MERCCOMPARE_MAIN,
	LAPTOP_MODE_MERCCOMPARE_MATRIX,
	LAPTOP_MODE_MERCCOMPARE_ANALYZE,
	LAPTOP_MODE_MERCCOMPARE_CUSTOMERS,

	// Flugente: World Health Organization
	LAPTOP_MODE_WHO_MAIN,
	LAPTOP_MODE_WHO_CONTRACT,
	LAPTOP_MODE_WHO_TIPS,

	// Flugente: PMC
	LAPTOP_MODE_PMC_MAIN,
	LAPTOP_MODE_PMC_CONTRACT_MILITIA,
	LAPTOP_MODE_PMC_CONTRACT_INDIVIDUAL,

	// Flugente: militia roster
	LAPTOP_MODE_MILITIAROSTER_MAIN,

	// Flugente: intel websites
	LAPTOP_MODE_INTELMARKET_BUYINFO,
	LAPTOP_MODE_INTELMARKET_SELLINFO,
	LAPTOP_MODE_INTELMARKET_ABOUT,

	// Flugente: facility production
	LAPTOP_MODE_FACILITY_PRODUCTION,
	
	LAPTOP_MODE_BOBBYR_SHIPMENTS,
	
	//ENCYCLOPEDIA
	LAPTOP_MODE_ENCYCLOPEDIA,//main page
	LAPTOP_MODE_ENCYCLOPEDIA_DATA,//subpage for locations, items, profiles or quests

	//BRIEFING ROOM
	LAPTOP_MODE_BRIEFING_ROOM_PAGE,
	LAPTOP_MODE_BRIEFING_ROOM,
	LAPTOP_MODE_BRIEFING_ROOM_ENTER,
	
	LAPTOP_MODE_AIM_MEMBERS_ARCHIVES_NEW,

	LAPTOP_MODE_REBEL_COMMAND,

	LAPTOP_MODE_MAX,
};

// bookamrks for WWW bookmark list


enum{
	FIRST_SIDE_PANEL=1,
	SECOND_SIDE_PANEL,
};
enum{
	LAPTOP_NO_CURSOR=0,
	LAPTOP_PANEL_CURSOR,
	LAPTOP_SCREEN_CURSOR,
	LAPTOP_WWW_CURSOR,
};

//#define LAPTOP_SIDE_PANEL_X 0
//#define LAPTOP_SIDE_PANEL_Y 0
//#define LAPTOP_SIDE_PANEL_WIDTH 640
//#define LAPTOP_SIDE_PANEL_HEIGHT 480
#define LAPTOP_X						iScreenWidthOffset
#define LAPTOP_Y						iScreenHeightOffset

//Moa: added brackets around definitions below to make them work. Especialy LAPTOP_SCREEN_HEIGHT and all its references resulted in wrong regions for invalidating screen which in return forced to redraw the screen emidiatly after render as invalidate did not work. Removed those redraw flags from all render functions I found (f.i.RenderBobbyRUsed()), but most likely missed some of them.
#define LAPTOP_SCREEN_UL_X				(iScreenWidthOffset + 111)
#define LAPTOP_SCREEN_UL_Y				(iScreenHeightOffset + 27)


#define LAPTOP_SCREEN_LR_X									(iScreenWidthOffset + 613)
#define LAPTOP_SCREEN_LR_Y									(iScreenHeightOffset + 427)
#define LAPTOP_UL_X											(iScreenWidthOffset + 24)
#define LAPTOP_UL_Y											(iScreenHeightOffset + 27)

#define LAPTOP_SCREEN_WIDTH									(LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X)
#define LAPTOP_SCREEN_HEIGHT								(LAPTOP_SCREEN_LR_Y - LAPTOP_SCREEN_UL_Y)

// new positions for web browser
#define LAPTOP_SCREEN_WEB_UL_Y								(LAPTOP_SCREEN_UL_Y + 19)
#define LAPTOP_SCREEN_WEB_LR_Y								(LAPTOP_SCREEN_WEB_UL_Y + LAPTOP_SCREEN_HEIGHT)
#define LAPTOP_SCREEN_WEB_DELTA_Y							19 //LAPTOP_SCREEN_WEB_UL_Y - LAPTOP_SCREEN_UL_Y
#define LAPTOP_TITLE_Y										(LAPTOP_SCREEN_WEB_UL_Y + 12)
#define LAPTOP_TEXT_WIDTH									((LAPTOP_SCREEN_LR_X - LAPTOP_SCREEN_UL_X + 1) + (2 * 111))

// the laptop on/off button 
#define ON_X												iScreenWidthOffset + 113
#define ON_Y												iScreenHeightOffset + 445


// the bookmark values, move cancel down as bookmarks added

enum{
	AIM_BOOKMARK=0,
	BOBBYR_BOOKMARK,
	IMP_BOOKMARK,
	MERC_BOOKMARK,
	FUNERAL_BOOKMARK,
	FLORIST_BOOKMARK,
	INSURANCE_BOOKMARK,
	CANCEL_STRING,
	ENCYCLOPEDIA_BOOKMARK, //Encyclopedia
	BRIEFING_ROOM_BOOKMARK, 
	CAMPAIGNHISTORY_BOOKMARK,	// added by Flugente
	MERCCOMPARE_BOOKMARK,		// added by Flugente
	WHO_BOOKMARK,				// added by Flugente
	PMC_BOOKMARK,				// added by Flugente
	MILITIAROSTER_BOOKMARK,		// added by Flugente
	INTELMARKET_BOOKMARK,		// added by Flugente
	PRODUCTION_BOOKMARK,		// added by Flugente
	REBELCOMMAND_BOOKMARK,
	TEXT_NUM_LAPTOP_BOOKMARKS
};


extern UINT32 guiLaptopBACKGROUND;


#define	DEAD_MERC_COLOR_RED							255
#define	DEAD_MERC_COLOR_GREEN						55
#define	DEAD_MERC_COLOR_BLUE						55

#ifdef JA2UB
//ja25 UB
void			ShouldImpReminderEmailBeSentWhenLaptopBackOnline();

extern BOOLEAN gfProcessCustomMaps;
#endif

#endif



 


