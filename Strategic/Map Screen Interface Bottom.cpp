#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Map Screen Interface Bottom.h"
	#include "Map Screen Interface Border.h"
	#include "Types.h"
	#include "vsurface.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "sgp.h"
	#include "Utilities.h"
	#include "message.h"
	#include "mapscreen.h"
	#include "strategicmap.h"
	#include "font control.h"
	#include "Radar Screen.h"
	#include "game clock.h"
	#include "sysutil.h"
	#include "Render Dirty.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map.h"
	#include "Text.h"
	#include "Overhead.h"
	#include "Prebattle Interface.h"
	#include "Options Screen.h"
	#include "Cursor Control.h"
	#include "gameloop.h"
	#include "ai.h"
	#include "Tactical Save.h"
	#include "Campaign Types.h"
	#include "Air Raid.h"
	#include "Finances.h"
	#include "LaptopSave.h"
	#include "Interface Items.h"
	#include "wordwrap.h"
	#include "meanwhile.h"
	#include "Dialogue Control.h"
	#include "Map Screen Helicopter.h"
	#include "Map Screen Interface TownMine Info.h"
	#include "Merc Contract.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Explosion Control.h"
	#include "Creature Spreading.h"
	#include "Assignments.h"
	#include "Soldier macros.h"
	#include "GameSettings.h"
	#include "SaveLoadScreen.h"
	#include "Interface Control.h"
	#include "Sys Globals.h"
#include "game init.h"
#endif

#ifdef JA2UB
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "SaveLoadGame.h"
#include "strategicmap.h"
#endif

#include "connect.h"

/* CHRISL: Adjusted settings to allow new Map_Screen_Bottom_800x600.sti to work.  This is needed if we
want to have the new inventory panel not overlap the message text area. */
#define MAP_BOTTOM_X							0
#define MAP_BOTTOM_Y							(SCREEN_HEIGHT - 121)	//359

//#define MESSAGE_SCROLL_AREA_START_X				(SCREEN_WIDTH - 534)	//330
//#define MESSAGE_SCROLL_AREA_END_X				(SCREEN_WIDTH - 522)	//344
#define MESSAGE_SCROLL_AREA_WIDTH				( MESSAGE_SCROLL_AREA_END_X - MESSAGE_SCROLL_AREA_START_X + 1 )

#define MESSAGE_SCROLL_AREA_START_Y				(SCREEN_HEIGHT - 90)	//390
#define MESSAGE_SCROLL_AREA_END_Y				(SCREEN_HEIGHT - 32)		//448
#define MESSAGE_SCROLL_AREA_HEIGHT				( MESSAGE_SCROLL_AREA_END_Y - MESSAGE_SCROLL_AREA_START_Y + 1 )

// CHRISL: Use these if we want scroll bar based on left edge of screen
// HEADROCK HAM 3.6: Now global variables to be changed when initializing map mode.
// See InitMapScreenInterfaceBottomCoords()
//#define MESSAGE_SCROLL_AREA_START_X				330
//#define MESSAGE_SCROLL_AREA_END_X				344
UINT16 MESSAGE_SCROLL_AREA_START_X;
UINT16 MESSAGE_SCROLL_AREA_END_X;

#define SLIDER_HEIGHT							11
#define SLIDER_WIDTH							11

#define SLIDER_BAR_RANGE						( MESSAGE_SCROLL_AREA_HEIGHT - SLIDER_HEIGHT )



#define MESSAGE_BTN_SCROLL_TIME					100

// delay for paused flash
#define PAUSE_GAME_TIMER						500

#define MAP_BOTTOM_FONT_COLOR					( 32 * 4 - 9 )

/*
// delay to start auto message scroll
#define DELAY_TO_START_MESSAGE_SCROLL 3000
// delay per auto message scroll
#define DELAY_PER_MESSAGE_SCROLL 300
*/

// button enums
enum{
	MAP_SCROLL_MESSAGE_UP =0,
	MAP_SCROLL_MESSAGE_DOWN,
};

enum{
	MAP_TIME_COMPRESS_MORE = 0,
	MAP_TIME_COMPRESS_LESS,
};



// GLOBALS


// the dirty state of the mapscreen interface bottom
BOOLEAN fMapScreenBottomDirty = TRUE;

BOOLEAN fMapBottomDirtied = FALSE;

//Used to flag the transition animation from mapscreen to laptop.
BOOLEAN gfStartMapScreenToLaptopTransition = FALSE;

// leaving map screen
BOOLEAN fLeavingMapScreen = FALSE;

// don't start transition from laptop to tactical stuff
BOOLEAN gfDontStartTransitionFromLaptop = FALSE;

// exiting to laptop?
BOOLEAN fLapTop = FALSE;

BOOLEAN gfOneFramePauseOnExit = FALSE;

// we've just scrolled to a new message (for autoscrolling only)
//BOOLEAN gfNewScrollMessage = FALSE;

// exit states
INT8 gbExitingMapScreenToWhere = -1;

UINT8 gubFirstMapscreenMessageIndex = 0;

UINT32 guiCompressionStringBaseTime = 0;

// graphics
UINT32 guiMAPBOTTOMPANEL;
UINT32 guiSliderBar;

// buttons
UINT32 guiMapMessageScrollButtons[ 2 ];
UINT32 guiMapBottomExitButtons[ 3 ];
UINT32 guiMapBottomTimeButtons[ 2 ];

// buttons images
UINT32 guiMapMessageScrollButtonsImage[ 2 ];
UINT32 guiMapBottomExitButtonsImage[ 3 ];
UINT32 guiMapBottomTimeButtonsImage[ 2 ];

// mouse regions
MOUSE_REGION gMapMessageScrollBarRegion;
MOUSE_REGION gMapMessageRegion;
MOUSE_REGION gMapPauseRegion;

MOUSE_REGION gTimeCompressionMask[ 3 ];

// EXTERNS

extern UINT8 gubStartOfMapScreenMessageList;

extern INT32 giMapInvDoneButton;

extern BOOLEAN fInMapMode;
extern BOOLEAN fShowInventoryFlag;
extern BOOLEAN fShowDescriptionFlag;

extern MOUSE_REGION gMPanelRegion;

extern BOOLEAN gfDisableTacticalPanelButtons;

// PROTOTYPES


void LoadMessageSliderBar( void );
void DeleteMessageSliderBar( void );
void DisplayScrollBarSlider( );

void CreateMapScreenBottomMessageScrollBarRegion( void );
void DeleteMapScreenBottomMessageScrollRegion( void );

void DisplayCurrentBalanceForMapBottom( void );
void DisplayCurrentBalanceTitleForMapBottom( void );
void DisplayProjectedDailyMineIncome( void );
void DisplayProjectedDailyExpenses( void ); // HEADROCK HAM 3.6: New expenses window shows running costs today.
void DrawNameOfLoadedSector( void );

void EnableDisableBottomButtonsAndRegions( void );
void EnableDisableTimeCompressButtons( void );
void EnableDisableMessageScrollButtonsAndRegions( void );

void DisplayCompressMode( void );
void RemoveCompressModePause( void );
void CreateCompressModePause( void );

void BtnLaptopCallback(GUI_BUTTON *btn,INT32 reason);
void BtnTacticalCallback( GUI_BUTTON *btn,INT32 reason );
void BtnOptionsFromMapScreenCallback( GUI_BUTTON *btn, INT32 reason );

void CompressModeClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
void CompressMaskClickCallback( MOUSE_REGION * pRegion, INT32 iReason );

void BtnTimeCompressMoreMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BtnTimeCompressLessMapScreenCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMessageDownMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMessageUpMapScreenCallback( GUI_BUTTON *btn,INT32 reason );

void MapScreenMessageScrollBarCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//void CheckForAndHandleAutoMessageScroll( void );



// FUNCTIONS



void HandleLoadOfMapBottomGraphics( void )
{
	// will load the graphics needed for the mapscreen interface bottom
	VOBJECT_DESC	VObjectDesc;

	// will create buttons for interface bottom
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		FilenameForBPP( "INTERFACE\\map_screen_bottom.sti", VObjectDesc.ImageFile );
	}
	else if (iResolution < _1024x768)
	{
		FilenameForBPP( "INTERFACE\\map_screen_bottom_800x600.sti", VObjectDesc.ImageFile );
	}
	else
	{
		FilenameForBPP( "INTERFACE\\map_screen_bottom_1024x768.sti", VObjectDesc.ImageFile );
	}

	if( !AddVideoObject( &VObjectDesc, &guiMAPBOTTOMPANEL ) )
		return;


	// load slider bar icon
	LoadMessageSliderBar( );

	return;
}

BOOLEAN LoadMapScreenInterfaceBottom( void )
{
	CreateButtonsForMapScreenInterfaceBottom( );
	CreateMapScreenBottomMessageScrollBarRegion( );

	// create pause region
	CreateCompressModePause( );

	return( TRUE );
}

void DeleteMapBottomGraphics( void )
{
	DeleteVideoObjectFromIndex( guiMAPBOTTOMPANEL );

	// delete slider bar icon
	DeleteMessageSliderBar( );

	return;
}

void DeleteMapScreenInterfaceBottom( void )
{
	// will delete graphics loaded for the mapscreen interface bottom

	DestroyButtonsForMapScreenInterfaceBottom( );
	DeleteMapScreenBottomMessageScrollRegion( );

	// remove comrpess mode pause
	RemoveCompressModePause( );

	return;
}


// Headrock: this function used to accept no arguments. It now accepts a Boolean so it can be forced to run
// the whole process when we call it from other files. See also .h file!!!
void RenderMapScreenInterfaceBottom( BOOLEAN fForceMapscreenFullRender )
{
	// will render the map screen bottom interface
	HVOBJECT hHandle;
	CHAR8 bFilename[ 32 ];

	fDisplayOverheadMap = FALSE;

	// render whole panel
	// HEADROCK Changed this line to accept outside influence through the new boolean:
	//if ( fForceMapscreenFullRender == TRUE || fMapScreenBottomDirty == TRUE)
	// HEADROCK HAM 3.6: OK, let's always render this panel, as long as the team inventory screen isn't open.
	// sevenfm: improved r8524 fix to work with 1280x720 resolution
	//if (fMapScreenBottomDirty || ((!fShowInventoryFlag || iResolution > _1024x600) && fForceMapscreenFullRender))
	if ((!fShowInventoryFlag || iResolution > _1280x720) && (fMapScreenBottomDirty || fForceMapscreenFullRender))
	{
		// get and blt panel
		GetVideoObject(&hHandle, guiMAPBOTTOMPANEL );

		BltVideoObject( guiSAVEBUFFER , hHandle, 0, xResOffset + MAP_BOTTOM_X, MAP_BOTTOM_Y, VO_BLT_SRCTRANSPARENCY,NULL );
	
		// WANNE - MP: Radarmap image should be displayed on every sector in multiplayer game
		if( GetSectorFlagStatus( sSelMapX, sSelMapY, ( UINT8 )iCurrentMapSectorZ, SF_ALREADY_VISITED ) == TRUE 
			|| is_networked)
		{
			GetMapFileName( sSelMapX, sSelMapY, ( UINT8 )iCurrentMapSectorZ, bFilename, TRUE, TRUE );
			LoadRadarScreenBitmap( bFilename );
		}
		else
		{
			ClearOutRadarMapImage();
		}

		fInterfacePanelDirty = DIRTYLEVEL2;

		// display title
		DisplayCurrentBalanceTitleForMapBottom( );

		// dirty buttons
		MarkButtonsDirty( );

		// TODO.RW.ARSP: Check: Is the height 121 OK?
		RestoreExternBackgroundRect( xResOffset + MAP_BOTTOM_X, MAP_BOTTOM_Y, SCREEN_WIDTH - MAP_BOTTOM_X - 2 * xResOffset, 121 );

		// re render radar map
		RenderRadarScreen( );

		// reset dirty flag
		fMapScreenBottomDirty = FALSE;
		fMapBottomDirtied = TRUE;

		// Headrock: Moved this next line from the above marker ^
		fMapScreenBottomDirty = FALSE;

		// CHRISL: Don't display messagelist if inventory panel is open
		// CHRISL: Only run this condition if we're drawing the message list on the left
		// display messages that can be scrolled through
		// HEADROCK HAM 3.6: Hmmmm, why not draw them? LEt's just move this whole segment here, shall we?
		//if(!fShowInventoryFlag)
		DisplayStringsInMapScreenMessageList( );
	}

	DisplayCompressMode( );

	DisplayCurrentBalanceForMapBottom( );
	DisplayProjectedDailyMineIncome( );
	DisplayProjectedDailyExpenses( );

	// draw the name of the loaded sector
	DrawNameOfLoadedSector( );

	// display slider on the scroll bar
	DisplayScrollBarSlider( );

		
	// handle auto scroll
	//CheckForAndHandleAutoMessageScroll( );

	EnableDisableMessageScrollButtonsAndRegions( );

	EnableDisableBottomButtonsAndRegions( );

	fMapBottomDirtied = FALSE;
	
	#ifdef JA113DEMO
	DisableButton (guiMapBottomExitButtons[MAP_EXIT_TO_LAPTOP]);
	#endif
	
	return;
}


BOOLEAN CreateButtonsForMapScreenInterfaceBottom( void )
{
	// laptop
	guiMapBottomExitButtonsImage[ MAP_EXIT_TO_LAPTOP ]=	LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,6,-1,15,-1 );
	guiMapBottomExitButtons[ MAP_EXIT_TO_LAPTOP ] = QuickCreateButton( guiMapBottomExitButtonsImage[ MAP_EXIT_TO_LAPTOP ], xResOffset + (xResSize - 184), (SCREEN_HEIGHT - 70),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnLaptopCallback);

	// tactical
	guiMapBottomExitButtonsImage[ MAP_EXIT_TO_TACTICAL ]=	LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,7,-1,16,-1 );

	guiMapBottomExitButtons[ MAP_EXIT_TO_TACTICAL ] = QuickCreateButton( guiMapBottomExitButtonsImage[ MAP_EXIT_TO_TACTICAL ], xResOffset + (xResSize - 144), (SCREEN_HEIGHT - 70),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnTacticalCallback);

	// options
	guiMapBottomExitButtonsImage[ MAP_EXIT_TO_OPTIONS ]=	LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,18,-1,19,-1 );
	guiMapBottomExitButtons[ MAP_EXIT_TO_OPTIONS ] = QuickCreateButton( guiMapBottomExitButtonsImage[ MAP_EXIT_TO_OPTIONS ], xResOffset + (xResSize - 182), (SCREEN_HEIGHT - 108),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnOptionsFromMapScreenCallback);


	SetButtonFastHelpText( guiMapBottomExitButtons[ 0 ], pMapScreenBottomFastHelp[ 0 ] );
	SetButtonFastHelpText( guiMapBottomExitButtons[ 1 ], pMapScreenBottomFastHelp[ 1 ] );
	SetButtonFastHelpText( guiMapBottomExitButtons[ 2 ], pMapScreenBottomFastHelp[ 2 ] );

	SetButtonCursor(guiMapBottomExitButtons[ 0 ], MSYS_NO_CURSOR );
	SetButtonCursor(guiMapBottomExitButtons[ 1 ], MSYS_NO_CURSOR );
	SetButtonCursor(guiMapBottomExitButtons[ 2 ], MSYS_NO_CURSOR );


	// time compression buttons
	guiMapBottomTimeButtonsImage[ MAP_TIME_COMPRESS_MORE ]=	LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,10,1,-1,3,-1 );
	guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_MORE ] = QuickCreateButton( guiMapBottomTimeButtonsImage[ MAP_TIME_COMPRESS_MORE ], xResOffset + (xResSize - 112), (SCREEN_HEIGHT - 24),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 2 ,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnTimeCompressMoreMapScreenCallback);

	guiMapBottomTimeButtonsImage[ MAP_TIME_COMPRESS_LESS ]=	LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,9,0,-1,2,-1 );
	guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_LESS ] = QuickCreateButton( guiMapBottomTimeButtonsImage[ MAP_TIME_COMPRESS_LESS ], xResOffset + (xResSize - 174), (SCREEN_HEIGHT - 24),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 2,
					(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnTimeCompressLessMapScreenCallback);


	SetButtonFastHelpText( guiMapBottomTimeButtons[ 0 ], pMapScreenBottomFastHelp[ 3 ] );
	SetButtonFastHelpText( guiMapBottomTimeButtons[ 1 ], pMapScreenBottomFastHelp[ 4 ] );

	SetButtonCursor(guiMapBottomTimeButtons[ 0 ], MSYS_NO_CURSOR );
	SetButtonCursor(guiMapBottomTimeButtons[ 1 ], MSYS_NO_CURSOR );


 // scroll buttons
// CHRISL: Changed coordinates to dynamically place scroll buttons from the right edge of screen.
  guiMapMessageScrollButtonsImage[ MAP_SCROLL_MESSAGE_UP ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,11,4,-1,6,-1 );
	guiMapMessageScrollButtonsImage[ MAP_SCROLL_MESSAGE_DOWN ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,12,5,-1,7,-1 );

	guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_UP ] = QuickCreateButton( guiMapMessageScrollButtonsImage[ MAP_SCROLL_MESSAGE_UP ], xResOffset + (xResSize - 306), (SCREEN_HEIGHT - 109),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMessageUpMapScreenCallback);
	guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_DOWN ] = QuickCreateButton( guiMapMessageScrollButtonsImage[ MAP_SCROLL_MESSAGE_DOWN ], xResOffset + (xResSize - 306), (SCREEN_HEIGHT - 28),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMessageDownMapScreenCallback);

	SetButtonFastHelpText( guiMapMessageScrollButtons[ 0 ], pMapScreenBottomFastHelp[ 5 ] );
	SetButtonFastHelpText( guiMapMessageScrollButtons[ 1 ], pMapScreenBottomFastHelp[ 6 ] );
	SetButtonCursor(guiMapMessageScrollButtons[ 0 ], MSYS_NO_CURSOR );
	SetButtonCursor(guiMapMessageScrollButtons[ 1 ], MSYS_NO_CURSOR );

	return( TRUE );
}


void DestroyButtonsForMapScreenInterfaceBottom( void )
{
	// will destroy the buttons for the mapscreen bottom interface

	RemoveButton( guiMapBottomExitButtons[ 0 ] );
	RemoveButton( guiMapBottomExitButtons[ 1 ] );
	RemoveButton( guiMapBottomExitButtons[ 2 ] );
	RemoveButton( guiMapMessageScrollButtons[ 0 ] );
	RemoveButton( guiMapMessageScrollButtons[ 1 ] );
	RemoveButton( guiMapBottomTimeButtons[ 0 ] );
	RemoveButton( guiMapBottomTimeButtons[ 1 ] );


	UnloadButtonImage( guiMapBottomExitButtonsImage[ 0 ] );
	UnloadButtonImage( guiMapBottomExitButtonsImage[ 1 ] );
	UnloadButtonImage( guiMapBottomExitButtonsImage[ 2 ] );
	UnloadButtonImage( guiMapMessageScrollButtonsImage[ 0 ] );
	UnloadButtonImage( guiMapMessageScrollButtonsImage[ 1 ] );
	UnloadButtonImage( guiMapBottomTimeButtonsImage[ 0 ] );
	UnloadButtonImage( guiMapBottomTimeButtonsImage[ 1 ] );

	// reset dirty flag
	fMapScreenBottomDirty = TRUE;

	return;
}


void BtnLaptopCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
		}

		// redraw region
		if( btn->Area.uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
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
			btn->uiFlags&=~(BUTTON_CLICKED_ON | BUTTON_DIRTY);
			DrawButton( btn->IDNum );

			RequestTriggerExitFromMapscreen( MAP_EXIT_TO_LAPTOP );
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


void BtnTacticalCallback( GUI_BUTTON *btn,INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		// redraw region
		if( btn->Area.uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			RequestTriggerExitFromMapscreen( MAP_EXIT_TO_TACTICAL );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}
	}
}

void BtnOptionsFromMapScreenCallback( GUI_BUTTON *btn, INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fMapScreenBottomDirty = TRUE;

			RequestTriggerExitFromMapscreen( MAP_EXIT_TO_OPTIONS );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}
	}
}

void DrawNameOfLoadedSector( void )
{
	CHAR16 sString[ 128 ];
	INT16 sFontX, sFontY;


	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );


	GetSectorIDString( sSelMapX, sSelMapY, ( INT8 )( iCurrentMapSectorZ ),sString, TRUE );
	ReduceStringLength( sString, 80, COMPFONT );

	VarFindFontCenterCoordinates( xResOffset + (xResSize - 92), (SCREEN_HEIGHT - 55), 80, 16, COMPFONT, &sFontX, &sFontY, sString );

	mprintf( sFontX, sFontY, L"%s", sString );
}


void CompressModeClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if( iReason & ( MSYS_CALLBACK_REASON_RBUTTON_UP | MSYS_CALLBACK_REASON_LBUTTON_UP ) )
	{
		if ( CommonTimeCompressionChecks() == TRUE )
			return;

		RequestToggleTimeCompression();
	}
}


void BtnTimeCompressMoreMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if ( CommonTimeCompressionChecks() == TRUE )
			return;

		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fMapScreenBottomDirty = TRUE;

			RequestIncreaseInTimeCompression();
	}
	}
	else 	if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if ( CommonTimeCompressionChecks() == TRUE )
			return;
	}
}



void BtnTimeCompressLessMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if ( CommonTimeCompressionChecks() == TRUE )
			return;

		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fMapScreenBottomDirty = TRUE;

			RequestDecreaseInTimeCompression();
	}
	}
	else 	if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if ( CommonTimeCompressionChecks() == TRUE )
			return;
	}
}



void BtnMessageDownMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{
	static INT32 iLastRepeatScrollTime = 0;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	iLastRepeatScrollTime = 0;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				fMapScreenBottomDirty = TRUE;
			}

			// down a line
			MapScreenMsgScrollDown( 1 );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= MESSAGE_BTN_SCROLL_TIME )
		{
			// down a line
			MapScreenMsgScrollDown( 1 );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	iLastRepeatScrollTime = 0;
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				fMapScreenBottomDirty = TRUE;
			}

			// down a page
			MapScreenMsgScrollDown( MAX_MESSAGES_ON_MAP_BOTTOM );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= MESSAGE_BTN_SCROLL_TIME )
		{
			// down a page
			MapScreenMsgScrollDown( MAX_MESSAGES_ON_MAP_BOTTOM );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
}


void BtnMessageUpMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{
	static INT32 iLastRepeatScrollTime = 0;


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	// redraw region
		if( btn->Area.uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	iLastRepeatScrollTime = 0;
	}

	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				fMapScreenBottomDirty = TRUE;
			}

			// up a line
			MapScreenMsgScrollUp( 1 );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= MESSAGE_BTN_SCROLL_TIME )
		{
			// up a line
			MapScreenMsgScrollUp( 1 );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if( IsMapScreenHelpTextUp() )
		{
			// stop mapscreen text
			StopMapScreenHelpText( );
			return;
		}

		// redraw region
		if( btn->uiFlags & MSYS_HAS_BACKRECT )
		{
			fMapScreenBottomDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);

	iLastRepeatScrollTime = 0;
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// redraw region
			if( btn->uiFlags & MSYS_HAS_BACKRECT )
			{
				fMapScreenBottomDirty = TRUE;
			}

			// up a page
			MapScreenMsgScrollUp( MAX_MESSAGES_ON_MAP_BOTTOM );
	}
	}
	else if( reason & MSYS_CALLBACK_REASON_RBUTTON_REPEAT )
	{
		if( GetJA2Clock() - iLastRepeatScrollTime >= MESSAGE_BTN_SCROLL_TIME )
		{
			// up a page
			MapScreenMsgScrollUp( MAX_MESSAGES_ON_MAP_BOTTOM );

		iLastRepeatScrollTime = GetJA2Clock( );
		}
	}
}



void EnableDisableMessageScrollButtonsAndRegions( void )
{
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfMapScreenMessages();

	// if no scrolling required, or already showing the topmost message
	if( ( ubNumMessages <= MAX_MESSAGES_ON_MAP_BOTTOM ) || ( gubFirstMapscreenMessageIndex == 0 ) )
	{
		DisableButton( guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_UP ] );
		ButtonList[ guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_UP ] ]->uiFlags &= ~( BUTTON_CLICKED_ON );
	}
	else
	{
		EnableButton( guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_UP ] );
	}

	// if no scrolling required, or already showing the last message
	if( ( ubNumMessages <= MAX_MESSAGES_ON_MAP_BOTTOM ) ||
			( ( gubFirstMapscreenMessageIndex + MAX_MESSAGES_ON_MAP_BOTTOM ) >= ubNumMessages ) )
	{
		DisableButton( guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_DOWN ] );
		ButtonList[ guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_DOWN ] ]->uiFlags &= ~( BUTTON_CLICKED_ON );
	}
	else
	{
		EnableButton( guiMapMessageScrollButtons[ MAP_SCROLL_MESSAGE_DOWN ] );
	}

	if( ubNumMessages <= MAX_MESSAGES_ON_MAP_BOTTOM )
	{
		MSYS_DisableRegion( &gMapMessageScrollBarRegion );
	}
	else
	{
		MSYS_EnableRegion( &gMapMessageScrollBarRegion );
	}
}


void DisplayCompressMode( void )
{
	INT16 sX, sY;
	CHAR16 sString[ 128 ];
	static UINT8 usColor = FONT_LTGREEN;

	// get compress speed
	if( giTimeCompressMode != NOT_USING_TIME_COMPRESSION )
	{
		if( IsTimeBeingCompressed() )
		{
			swprintf( sString, L"%s", sTimeStrings[ giTimeCompressMode ] );
		}
		else
		{
			swprintf( sString, L"%s", sTimeStrings[ 0 ] );
		}
	}

	// ABCDEFG
	RestoreExternBackgroundRect( xResOffset + (xResSize - 151), (SCREEN_HEIGHT - 24), 63, 13 );
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	SetFont( COMPFONT );

	if( GetJA2Clock() - guiCompressionStringBaseTime >= PAUSE_GAME_TIMER )
	{
		if( usColor == FONT_LTGREEN )
		{
			usColor = FONT_WHITE;
		}
		else
		{
			usColor = FONT_LTGREEN;
		}

		guiCompressionStringBaseTime = GetJA2Clock();
	}

	if( ( giTimeCompressMode != 0 ) && ( GamePaused( ) == FALSE ) )
	{
		usColor = FONT_LTGREEN;
	}

	SetFontForeground( usColor );
	SetFontBackground( FONT_BLACK );

	FindFontCenterCoordinates( xResOffset + (xResSize - 151), (SCREEN_HEIGHT - 24), 33, 13, sString, COMPFONT, &sX, &sY );
		
	mprintf( sX, sY, sString );

	return;
}


void CreateCompressModePause( void )
{
	MSYS_DefineRegion( &gMapPauseRegion, xResOffset + (xResSize - 153), (SCREEN_HEIGHT - 24), xResOffset + (xResSize - 118), (SCREEN_HEIGHT - 13), MSYS_PRIORITY_HIGH,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, CompressModeClickCallback );

	SetRegionFastHelpText( &gMapPauseRegion, pMapScreenBottomFastHelp[ 7 ] );
}

void RemoveCompressModePause( void )
{
	MSYS_RemoveRegion( &gMapPauseRegion );
}


void LoadMessageSliderBar( void )
{
	// this function will load the message slider bar
	VOBJECT_DESC	VObjectDesc;

	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "INTERFACE\\map_screen_bottom_arrows.sti", VObjectDesc.ImageFile );
	if( !AddVideoObject( &VObjectDesc, &guiSliderBar ) )
		return;
}

void DeleteMessageSliderBar( void )
{
	// this function will delete message slider bar
	DeleteVideoObjectFromIndex( guiSliderBar );
}


void CreateMapScreenBottomMessageScrollBarRegion( void )
{
		MSYS_DefineRegion( &gMapMessageScrollBarRegion, MESSAGE_SCROLL_AREA_START_X, MESSAGE_SCROLL_AREA_START_Y,
								MESSAGE_SCROLL_AREA_END_X, MESSAGE_SCROLL_AREA_END_Y,
								MSYS_PRIORITY_NORMAL, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MapScreenMessageScrollBarCallBack );

		MSYS_DefineRegion(&gMapMessageRegion, (INT16)(xResOffset + 4), MESSAGE_SCROLL_AREA_START_Y, MESSAGE_SCROLL_AREA_END_X, MESSAGE_SCROLL_AREA_END_Y,
			MSYS_PRIORITY_NORMAL, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK);
}


void DeleteMapScreenBottomMessageScrollRegion( void )
{
	MSYS_RemoveRegion( &gMapMessageScrollBarRegion );
	MSYS_RemoveRegion(&gMapMessageRegion);
}



void MapScreenMessageScrollBarCallBack( MOUSE_REGION *pRegion, INT32 iReason )
{
	POINT MousePos;
	UINT8 ubMouseYOffset;
	UINT8	ubDesiredSliderOffset;
	UINT8 ubDesiredMessageIndex;
	UINT8 ubNumMessages;


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	return;
	}


	if ( iReason & ( MSYS_CALLBACK_REASON_LBUTTON_DWN | MSYS_CALLBACK_REASON_LBUTTON_REPEAT ) )
	{
		// how many messages are there?
	ubNumMessages = GetRangeOfMapScreenMessages();

		// region is supposed to be disabled if there aren't enough messages to scroll.	Formulas assume this
		if ( ubNumMessages > MAX_MESSAGES_ON_MAP_BOTTOM )
		{
			// where is the mouse?
			GetCursorPos( &MousePos );
			ScreenToClient(ghWindow, &MousePos); // In window coords!

			ubMouseYOffset = (UINT8) MousePos.y - MESSAGE_SCROLL_AREA_START_Y;

			// if clicking in the top 5 pixels of the slider bar
			if ( ubMouseYOffset < ( SLIDER_HEIGHT / 2 ) )
			{
				// scroll all the way to the top
				ubDesiredMessageIndex = 0;
			}
			// if clicking in the bottom 6 pixels of the slider bar
			else if ( ubMouseYOffset >= ( MESSAGE_SCROLL_AREA_HEIGHT - ( SLIDER_HEIGHT / 2 ) ) )
			{
				// scroll all the way to the bottom
				ubDesiredMessageIndex = ubNumMessages - MAX_MESSAGES_ON_MAP_BOTTOM;
			}
			else
			{
				// somewhere in between
				ubDesiredSliderOffset = ubMouseYOffset - ( SLIDER_HEIGHT / 2 );

				Assert( ubDesiredSliderOffset <= SLIDER_BAR_RANGE );

				// calculate what the index should be to place the slider at this offset (round fractions of .5+ up)
				ubDesiredMessageIndex = ( ( ubDesiredSliderOffset * ( ubNumMessages - MAX_MESSAGES_ON_MAP_BOTTOM ) ) + ( SLIDER_BAR_RANGE / 2 ) ) / SLIDER_BAR_RANGE;
			}

			// if it's a change
			if ( ubDesiredMessageIndex != gubFirstMapscreenMessageIndex )
			{
				ChangeCurrentMapscreenMessageIndex( ubDesiredMessageIndex );
			}
		}
	}
}


void DisplayScrollBarSlider( )
{
	// will display the scroll bar icon
	UINT8 ubNumMessages;
	UINT8 ubSliderOffset;
	HVOBJECT hHandle;


	ubNumMessages = GetRangeOfMapScreenMessages();

	// only show the slider if there are more messages than will fit on screen
	if ( ubNumMessages > MAX_MESSAGES_ON_MAP_BOTTOM )
	{
		// calculate where slider should be positioned
		ubSliderOffset = ( SLIDER_BAR_RANGE * gubFirstMapscreenMessageIndex ) / ( ubNumMessages - MAX_MESSAGES_ON_MAP_BOTTOM );

		GetVideoObject( &hHandle, guiSliderBar );
		BltVideoObject( FRAME_BUFFER, hHandle, 8, MESSAGE_SCROLL_AREA_START_X + 2, MESSAGE_SCROLL_AREA_START_Y + ubSliderOffset, VO_BLT_SRCTRANSPARENCY, NULL );
	}
}


/*
void CheckForAndHandleAutoMessageScroll( void )
{
	// will check if we are not at the most recent message, if not, scroll to it
	static INT32 iBaseScrollTime =0;
	static INT32 iBaseScrollDelay =0;
	static BOOLEAN fScrollMessage = FALSE;

	// check if we are at the last message, if so, leave
	if( IsThisTheLastMessageInTheList( ) )
	{
		// leave
		// reset flag
		fScrollMessage = FALSE;
		return;
	}

	// we are not, check how long we have been here?
	if( gfNewScrollMessage == TRUE )
	{
		// we just scrolled to a new message, reset timer
		iBaseScrollTime = GetJA2Clock( );

		// reset flag
		gfNewScrollMessage = FALSE;
		fScrollMessage = FALSE;
	}

	// check timer

	if( GetJA2Clock( ) - iBaseScrollTime > DELAY_TO_START_MESSAGE_SCROLL )
	{

		if( fScrollMessage == FALSE )
		{
		// set up scroll delay
		iBaseScrollDelay = GetJA2Clock( );

		// start scroll
		fScrollMessage = TRUE;

		}

		iBaseScrollTime = GetJA2Clock( );
	}

	if( fScrollMessage == TRUE )
	{
		if( GetJA2Clock( ) - iBaseScrollDelay > DELAY_PER_MESSAGE_SCROLL )
		{
			// scroll to next message
			MoveCurrentMessagePointerDownList( );

			// dirty region
			fMapScreenBottomDirty = TRUE;

			// reset delay timer
			iBaseScrollDelay = GetJA2Clock( );
		}
	}


	return;
}
*/



void EnableDisableBottomButtonsAndRegions( void )
{
	INT8 iExitButtonIndex;

	// this enables and disables the buttons MAP_EXIT_TO_LAPTOP, MAP_EXIT_TO_TACTICAL, and MAP_EXIT_TO_OPTIONS
	for ( iExitButtonIndex = 0; iExitButtonIndex < 3; iExitButtonIndex++ )
	{
		if ( AllowedToExitFromMapscreenTo( iExitButtonIndex ) )
		{
			EnableButton( guiMapBottomExitButtons[ iExitButtonIndex ]);
		}
		else
		{
			DisableButton( guiMapBottomExitButtons[ iExitButtonIndex ]);
		}
	}

	// enable/disable time compress buttons and region masks
	EnableDisableTimeCompressButtons( );
	CreateDestroyMouseRegionMasksForTimeCompressionButtons( );


	// Enable/Disable map inventory panel buttons

	// if in merc inventory panel
	if( fShowInventoryFlag )
	{
		// and an item is in the cursor
		if( ( gMPanelRegion.Cursor == EXTERN_CURSOR ) || ( InKeyRingPopup( ) == TRUE ) || InItemStackPopup() )
		{
			DisableButton( giMapInvDoneButton );
		}
		else
		{
			EnableButton( giMapInvDoneButton );
		}

		if( fShowDescriptionFlag )
		{
			ForceButtonUnDirty( giMapInvDoneButton );
		}
	}
}


void EnableDisableTimeCompressButtons( void )
{
	if( AllowedToTimeCompress( ) == FALSE )
	{
		DisableButton( guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_MORE ] );
		DisableButton( guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_LESS ] );
	}
	else
	{
		// disable LESS if time compression is at minimum or OFF
		if ( !IsTimeCompressionOn() || giTimeCompressMode == TIME_COMPRESS_X0 )
		{
			DisableButton( guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_LESS ] );
		}
		else
		{
			EnableButton( guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_LESS] );
		}

		// disable MORE if we're not paused and time compression is at maximum
		// only disable MORE if we're not paused and time compression is at maximum
		if ( IsTimeCompressionOn() && ( giTimeCompressMode == TIME_COMPRESS_60MINS ) )
		{
			DisableButton( guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_MORE ] );
		}
		else
		{
			EnableButton( guiMapBottomTimeButtons[ MAP_TIME_COMPRESS_MORE ] );
		}
	}
}


void EnableDisAbleMapScreenOptionsButton( BOOLEAN fEnable )
{
	if( fEnable )
	{
		EnableButton( guiMapBottomExitButtons[ MAP_EXIT_TO_OPTIONS ] );
	}
	else
	{
		DisableButton( guiMapBottomExitButtons[ MAP_EXIT_TO_OPTIONS ] );
	}
}


BOOLEAN AllowedToTimeCompress( void )
{
	// WANNE - MP: In multiplayer mode we do not allow compress time, for now.
	if (is_networked)
	{
		return ( FALSE );
	}

	// if already leaving, disallow any other attempts to exit
	if ( fLeavingMapScreen )
	{
		return( FALSE );
	}

	// if already going someplace
	if ( gbExitingMapScreenToWhere != -1 )
	{
		return( FALSE );
	}

	// if we're locked into paused time compression by some event that enforces that
	if ( PauseStateLocked() )
	{
		return( FALSE );
	}

#ifdef JA2UB
//Ja25 no meanwhiles
#else
	// meanwhile coming up
	if ( gfMeanwhileTryingToStart )
	{
		return( FALSE );
	}
#endif

	// someone has something to say
	if ( !DialogueQueueIsEmpty() )
	{
		return( FALSE );
	}

	// moving / confirming movement
	if ( ( GetSelectedDestChar() != -1) || fPlotForHelicopter || fPlotForMilitia || gfInConfirmMapMoveMode || fShowMapScreenMovementList )
	{
		return( FALSE );
	}

	// HEADROCK HAM 3.6: Added Facility Menus
	if( fShowAssignmentMenu || gAssignMenuState > ASMENU_NONE || fShowAttributeMenu || fShowContractMenu || fShowRemoveMenu || fShowFacilityAssignmentMenu )
	{
		return( FALSE );
	}

	if( fShowUpdateBox || fShowTownInfo || ( sSelectedMilitiaTown != 0 ) )
	{
		return( FALSE );
	}

	// renewing contracts
	if ( gfContractRenewalSquenceOn )
	{
		return( FALSE );
	}

	// disabled due to battle?
	if( ( fDisableMapInterfaceDueToBattle ) || ( fDisableDueToBattleRoster ) )
	{
		return( FALSE );
	}

	// if holding an inventory item
	if ( fMapInventoryItem )
	{
		return( FALSE );
	}

	// show the inventory pool?
	if( fShowMapInventoryPool )
	{
		// prevent time compress (items get stolen over time, etc.)
		return( FALSE );
	}

	// no mercs have ever been hired
	if( gfAtLeastOneMercWasHired == FALSE )
	{
		return( FALSE );
	}

/*
	//in air raid
	if( InAirRaid( ) == TRUE )
	{
		return( FALSE );
	}
*/

	// no usable mercs on team!
	if ( !AnyUsableRealMercenariesOnTeam() )
	{
		return( FALSE );
	}

		// must wait till bombs go off
	if ( ActiveTimedBombExists() )
	{
		return( FALSE );
	}

	// hostile sector / in battle
	if( (gTacticalStatus.uiFlags & INCOMBAT ) || ( gTacticalStatus.fEnemyInSector ) )
	{
		return( FALSE );
	}

	if( PlayerGroupIsInACreatureInfestedMine() )
	{
		return FALSE;
	}

	// Flugente: no time compression if hostile civilians bloodcats are in the current sector. 
	// Otherwise time progresses, but squad arrivals will be delayed as long as they still exist
	if ( HostileCiviliansPresent() || HostileBloodcatsPresent() )
	{
		return FALSE;
	}

#ifdef JA2UB  
		//if the player hasnt been to the initial sector yet
	if( !GetSectorFlagStatus( gGameExternalOptions.ubDefaultArrivalSectorX, gGameExternalOptions.ubDefaultArrivalSectorY, 0, SF_HAS_ENTERED_TACTICAL ) ) //7, 8
	{
		//if there is something that jerry wants to say
		if( !WillJerryMiloAllowThePlayerToCompressTimeAtBeginingOfGame() )
		{
			return( FALSE );
		}
	}
#endif
	
	return( TRUE );
}


void DisplayCurrentBalanceTitleForMapBottom( void )
{
	CHAR16 sString[ 128 ];
	INT16 sFontX, sFontY;

	// ste the font buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	SetFont( COMPFONT );
	SetFontForeground( MAP_BOTTOM_FONT_COLOR );
	SetFontBackground( FONT_BLACK );

	swprintf( sString, L"%s", pMapScreenBottomText[ 0 ] );

	// center it
	// CHRISL: Replaced X coordinate with dynamic coordinate set from right edge of screen
	//VarFindFontCenterCoordinates( (1024 - 637), (SCREEN_HEIGHT - 107),  78, 10,  COMPFONT, &sFontX, &sFontY, sString );
	// CHRISL: Use this if we want to display from the left edge
	//VarFindFontCenterCoordinates( 359, (SCREEN_HEIGHT - 107),  78, 10,  COMPFONT, &sFontX, &sFontY, sString );
	// HEADROCK HAM 3.6: The balance/income box has been moved to the right side, near the laptop button.
	VarFindFontCenterCoordinates( xResOffset + (xResSize - 278), (SCREEN_HEIGHT - 111), 78, 10, COMPFONT, &sFontX, &sFontY, sString );
	
	// print it
	mprintf( sFontX, sFontY, L"%s", sString );

	swprintf( sString, L"%s", zMarksMapScreenText[ 2 ] );

	// center it
	// CHRISL: Replaced X coordinate with dynamic coordinate set from right edge of screen
	//VarFindFontCenterCoordinates( (1024 - 637), (SCREEN_HEIGHT - 61),  78, 10,  COMPFONT, &sFontX, &sFontY, sString );
	// CHRISL: Use this if we want to display from the left edge
	//VarFindFontCenterCoordinates( 359, (SCREEN_HEIGHT - 61),  78, 10,  COMPFONT, &sFontX, &sFontY, sString );
	// HEADROCK HAM 3.6: The balance/income box has been moved to the right side, near the laptop button.	
	VarFindFontCenterCoordinates( xResOffset + (xResSize - 278), (SCREEN_HEIGHT - 74), 78, 10, COMPFONT, &sFontX, &sFontY, sString );

	// print it
	mprintf( sFontX, sFontY, L"%s", sString );

	swprintf( sString, L"%s", New113HAMMessage[ 20 ] );

	// HEADROCK HAM 3.6: Projected expenses for today, with facilities (and in the future, merc contracts) taken into
	// account.
	VarFindFontCenterCoordinates( xResOffset + (xResSize - 278), (SCREEN_HEIGHT - 37), 78, 10, COMPFONT, &sFontX, &sFontY, sString );

	// print it
	mprintf( sFontX, sFontY, L"%s", sString );

	// ste the font buffer
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	
	return;
}

void DisplayCurrentBalanceForMapBottom( void )
{
	// show the current balance for the player on the map panel bottom
	CHAR16 sString[ 128 ];
	INT16 sFontX, sFontY;

	// ste the font buffer
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// set up the font
	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	swprintf( sString, L"%d", LaptopSaveInfo.iCurrentBalance );

	// insert
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );

	VarFindFontCenterCoordinates( xResOffset + (xResSize - 278), (SCREEN_HEIGHT - 95), 78, 10, COMPFONT, &sFontX, &sFontY, sString );
	
	// print it
	mprintf( sFontX, sFontY, L"%s", sString );

	return;
}

void CreateDestroyMouseRegionMasksForTimeCompressionButtons( void )
{
	BOOLEAN fDisabled = FALSE;
	static BOOLEAN fCreated = FALSE;

	// allowed to time compress?
	if( AllowedToTimeCompress( )== FALSE )
	{
		// no, disable buttons
		fDisabled = TRUE;
	}

	if( fInMapMode == FALSE )
	{
		fDisabled = FALSE;
	}

	// check if disabled and not created, create
	if( ( fDisabled ) && ( fCreated == FALSE ) )
	{
		// mask over compress more button
		MSYS_DefineRegion( &gTimeCompressionMask[ 0 ], xResOffset + (xResSize - 112), (SCREEN_HEIGHT - 24), xResOffset + (xResSize - 112) + 13, (SCREEN_HEIGHT - 24) + 16, MSYS_PRIORITY_HIGHEST - 1,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, CompressMaskClickCallback );

		// mask over compress less button
		MSYS_DefineRegion( &gTimeCompressionMask[ 1 ], xResOffset + (xResSize - 174), (SCREEN_HEIGHT - 24), xResOffset + (xResSize - 174) + 13, (SCREEN_HEIGHT - 24) + 16, MSYS_PRIORITY_HIGHEST - 1,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, CompressMaskClickCallback );

		// mask over pause game button
		MSYS_DefineRegion( &gTimeCompressionMask[ 2 ], xResOffset + (xResSize - 153), (SCREEN_HEIGHT - 24), xResOffset + (xResSize - 153) + 118, (SCREEN_HEIGHT - 13), MSYS_PRIORITY_HIGHEST - 1,
							MSYS_NO_CURSOR, MSYS_NO_CALLBACK, CompressMaskClickCallback );
		RemoveMouseRegionForPauseOfClock();
		fCreated = TRUE;
	}
	else if( ( fDisabled == FALSE ) && ( fCreated ) )
	{
		// created and no longer need to disable
		MSYS_RemoveRegion( &gTimeCompressionMask[ 0 ] );
		MSYS_RemoveRegion( &gTimeCompressionMask[ 1 ] );
		MSYS_RemoveRegion( &gTimeCompressionMask[ 2 ] );
		CreateMouseRegionForPauseOfClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
		fCreated = FALSE;
	}
}


void CompressMaskClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		TellPlayerWhyHeCantCompressTime( );
	}
}


void DisplayProjectedDailyMineIncome( void )
{
	INT32 iRate = 0;
	static INT32 iOldRate = -1;
	CHAR16 sString[ 128 ];
	INT16 sFontX, sFontY;

	// grab the rate from the financial system
	iRate = GetProjectedTotalDailyIncome( );

	if( iRate != iOldRate )
	{
		iOldRate = iRate;
		fMapScreenBottomDirty = TRUE;

		// if screen was not dirtied, leave
		if( fMapBottomDirtied == FALSE )
		{
			return;
		}
	}
		// ste the font buffer
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// set up the font
	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	swprintf( sString, L"%d", iRate );

	// insert
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );

	VarFindFontCenterCoordinates( xResOffset + (xResSize - 278), (SCREEN_HEIGHT - 58), 78, 10, COMPFONT, &sFontX, &sFontY, sString );

	// print it
	mprintf( sFontX, sFontY, L"%s", sString );

	return;
}

void DisplayProjectedDailyExpenses( void )
{
	INT32 iRate = 0;
	static INT32 iOldExpensesRate = -1;
	CHAR16 sString[ 128 ];
	INT16 sFontX, sFontY;

	// grab the rate from the financial system
	iRate = GetProjectedExpenses( );

	if( iRate != iOldExpensesRate )
	{
		iOldExpensesRate = iRate;
		fMapScreenBottomDirty = TRUE;

		// if screen was not dirtied, leave
		if( fMapBottomDirtied == FALSE )
		{
			return;
		}
	}

	// set the font buffer
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// set up the font
	SetFont( COMPFONT );
	if (iRate == 0)
	{
		SetFontForeground( 183 );
	}
	else
	{
		SetFontForeground( FONT_MCOLOR_LTRED );
	}
	SetFontBackground( FONT_BLACK );

	swprintf( sString, L"%d", iRate );

	// insert extra characters
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );

	VarFindFontCenterCoordinates( xResOffset + (xResSize - 278), (SCREEN_HEIGHT - 21), 78, 10, COMPFONT, &sFontX, &sFontY, sString );

	// print it
	mprintf( sFontX, sFontY, L"%s", sString );

	return;
}


BOOLEAN CommonTimeCompressionChecks( void )
{
	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );
		return( TRUE );
	}

	if ( ( GetSelectedDestChar() != -1) || fPlotForHelicopter || fPlotForMilitia )
	{
		// abort plotting movement
		AbortMovementPlottingMode( );
		return( TRUE );
	}

//	if ( gfInAirRaid )
//		return TRUE;

	return( FALSE );
}



BOOLEAN AnyUsableRealMercenariesOnTeam( void )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT32 iCounter = 0, iNumberOnTeam = 0;

	// this is for speed, this runs once/frame
	iNumberOnTeam = gTacticalStatus.Team[ OUR_TEAM ].bLastID;

	// get number of mercs on team who are not vehicles or robot, POWs or EPCs
	for( iCounter = 0; iCounter < iNumberOnTeam; iCounter++ )
	{
		pSoldier = &Menptr[ iCounter ];

		if( ( pSoldier->bActive ) && ( pSoldier->stats.bLife > 0 ) &&
				!( pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) &&
				( pSoldier->bAssignment != ASSIGNMENT_POW ) &&
				( pSoldier->bAssignment != ASSIGNMENT_DEAD ) &&
				( pSoldier->bAssignment != ASSIGNMENT_MINIEVENT ) &&
				( pSoldier->ubWhatKindOfMercAmI != MERC_TYPE__EPC ) )
		{
			return( TRUE );
		}
	}

	return( FALSE );
}


void RequestTriggerExitFromMapscreen( INT8 bExitToWhere )
{
#ifdef JA2UB
	Assert( ( bExitToWhere >= MAP_EXIT_TO_LAPTOP ) && ( bExitToWhere <= MAP_EXIT_TO_INTRO_SCREEN ));//MAP_EXIT_TO_MAINMENU ) );
#else
	Assert( ( bExitToWhere >= MAP_EXIT_TO_LAPTOP ) && ( bExitToWhere <= MAP_EXIT_TO_MAINMENU ));
#endif
	// if allowed to do so
	if ( AllowedToExitFromMapscreenTo( bExitToWhere ) )
	{
		//if the screen to exit to is the SAVE screen
		if( bExitToWhere == MAP_EXIT_TO_SAVE )
		{
			//if the game CAN NOT be saved
			if( !CanGameBeSaved() )
			{
				//Display a message saying the player cant save now
				switch( gGameOptions.ubIronManMode )
				{
					case 2:
						DoMapMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__SOFT_IRON_MAN_CANT_SAVE_NOW ], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
						break;
					case 3:
						CHAR16 zTemp[320];
						swprintf( zTemp, zNewTacticalMessages[ TCTL_MSG__EXTREME_IRON_MAN_CANT_SAVE_NOW ], gGameExternalOptions.ubExtremeIronManSavingHour);
						DoMapMessageBox( MSG_BOX_BASIC_STYLE, zTemp, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
						break;
					default:
						DoMapMessageBox( MSG_BOX_BASIC_STYLE, zNewTacticalMessages[ TCTL_MSG__IRON_MAN_CANT_SAVE_NOW ], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
						break;
				}
				return;
			}
		}
		else if (!(bExitToWhere == MAP_EXIT_TO_TACTICAL || bExitToWhere == MAP_EXIT_TO_MAINMENU))
		{
			// OJW - 20090301
			if (is_networked && is_client)
			{
				if (client_ready[CLIENT_NUM-1]==1 && is_game_started == false)
				{
					// un-ready if we are not on map screen
					start_battle();
				}
			}
		}

		// permit it, and get the ball rolling
		gbExitingMapScreenToWhere = bExitToWhere;

		// delay until mapscreen has had a chance to render at least one full frame
		gfOneFramePauseOnExit = TRUE;

		//CHRISL: To force activation of tactical team panel buttons when switching to tactical
		gfDisableTacticalPanelButtons = FALSE;
	}
}


BOOLEAN AllowedToExitFromMapscreenTo( INT8 bExitToWhere )
{
#ifdef JA2UB
	Assert( ( bExitToWhere >= MAP_EXIT_TO_LAPTOP ) && ( bExitToWhere <= MAP_EXIT_TO_INTRO_SCREEN)); //MAP_EXIT_TO_MAINMENU ) );
#else
	Assert( ( bExitToWhere >= MAP_EXIT_TO_LAPTOP ) && ( bExitToWhere <= MAP_EXIT_TO_MAINMENU ) );
#endif
	// if already leaving, disallow any other attempts to exit
	if ( fLeavingMapScreen )
	{
		return( FALSE );
	}

	// OJW - 20090210 - clean resources on disconnect
	if (bExitToWhere == MAP_EXIT_TO_MAINMENU)
	{
		// always allow this
		return( TRUE );
	}

	// WANNE: At least one merc must be hired so we can go to tactical.
	// This was an exploit. If you go to tactical in Omerta and have not hired any merc
	// then go back to laptop and hire a merc. After the merc arrives the heli landing was not
	// shown and the sector was free of enemies.
	if (bExitToWhere == MAP_EXIT_TO_TACTICAL && gfAtLeastOneMercWasHired == FALSE)
	{
		return ( FALSE );
	}

	// if already going someplace else
	if ( ( gbExitingMapScreenToWhere != -1 ) && ( gbExitingMapScreenToWhere != bExitToWhere ) )
	{
		return( FALSE );
	}

	// someone has something to say
	if ( !DialogueQueueIsEmpty() )
	{
		return( FALSE );
	}

#ifdef JA2UB
//Ja25 No meanwhiles
#else
	// meanwhile coming up
	if ( gfMeanwhileTryingToStart )
	{
		return( FALSE );
	}
#endif

	// if we're locked into paused time compression by some event that enforces that
	if ( PauseStateLocked() )
	{
		return( FALSE );
	}

	// if holding an inventory item
	if ( fMapInventoryItem || ( gMPanelRegion.Cursor == EXTERN_CURSOR ) )
	{
		return( FALSE );
	}

	if( fShowUpdateBox || fShowTownInfo || ( sSelectedMilitiaTown != 0 ) )
	{
		return( FALSE );
	}

	// renewing contracts
	if( gfContractRenewalSquenceOn )
	{
		return( FALSE );
	}

	// battle about to occur?
	if( ( fDisableDueToBattleRoster ) || ( fDisableMapInterfaceDueToBattle ) )
	{
		return( FALSE );
	}

/*
	// air raid starting
	if( gubAirRaidMode == AIR_RAID_START )
	{
		// nope
		return( FALSE );
	}
*/


	// the following tests apply to going tactical screen only
	if ( bExitToWhere == MAP_EXIT_TO_TACTICAL )
	{
		// if in battle or air raid, the ONLY sector we can go tactical in is the one that's loaded
		if ( ( ( gTacticalStatus.uiFlags & INCOMBAT ) || ( gTacticalStatus.fEnemyInSector ) /*|| InAirRaid( )*/ ) &&
				( ( sSelMapX != gWorldSectorX ) || ( sSelMapY != gWorldSectorY ) || ( ( UINT8 )iCurrentMapSectorZ ) != gbWorldSectorZ ) )
		{
			return( FALSE );
		}

		// must have some mercs there
		if( !CanGoToTacticalInSector( sSelMapX, sSelMapY, ( UINT8 )iCurrentMapSectorZ ) )
		{
			return( FALSE );
		}
		if ( (( sSelMapX != gWorldSectorX ) || ( sSelMapY != gWorldSectorY ) || (( UINT8 )iCurrentMapSectorZ ) != gbWorldSectorZ  ) && is_client) //hayden (PBI only)
		{
			return (FALSE);
		}
	}

	//if we are map screen sector inventory
	if( fShowMapInventoryPool )
	{
		//dont allow it
		return( FALSE );
	}

	//hayden
	if( bExitToWhere == MAP_EXIT_TO_LAPTOP && !allowlaptop && is_networked /*&& (is_client || is_server)*/)
	{
		//dont allow it
		return( FALSE );
	}

	// OK to go there, passed all the checks
	return( TRUE );
}


void HandleExitsFromMapScreen( void )
{
	// if going somewhere
	if ( gbExitingMapScreenToWhere != -1 )
	{
		// delay all exits by one frame...
		if( gfOneFramePauseOnExit == TRUE )
		{
			gfOneFramePauseOnExit = FALSE;
			return;
		}

		// make sure it's still legal to do this!
		if ( AllowedToExitFromMapscreenTo( gbExitingMapScreenToWhere ) )
		{
			// see where we're trying to go
			switch ( gbExitingMapScreenToWhere )
			{
				case MAP_EXIT_TO_LAPTOP:
					fLapTop = TRUE;
					SetPendingNewScreen(LAPTOP_SCREEN);

					if( gfExtraBuffer )
					{ //Then initiate the transition animation from the mapscreen to laptop...

						BlitBufferToBuffer( FRAME_BUFFER, guiEXTRABUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
						gfStartMapScreenToLaptopTransition = TRUE;
					}
					break;

				case MAP_EXIT_TO_TACTICAL:
					SetCurrentWorldSector( sSelMapX, sSelMapY, ( INT8 )iCurrentMapSectorZ );
					// silversurfer: bugfix for panel redraw issue after quickload from tactical screen
					// this will make sure that the tactical screen gets refreshed and guiTacticalInterfaceFlags is reset
					fFirstTimeInGameScreen = TRUE;
					guiTacticalInterfaceFlags &= (~INTERFACE_MAPSCREEN );
					break;

				case MAP_EXIT_TO_OPTIONS:
					guiPreviousOptionScreen = guiCurrentScreen;
					SetPendingNewScreen( OPTIONS_SCREEN );
					break;

				case MAP_EXIT_TO_SAVE:
				case MAP_EXIT_TO_LOAD:
					gfCameDirectlyFromGame = TRUE;
					guiPreviousOptionScreen = guiCurrentScreen;
					SetPendingNewScreen( SAVE_LOAD_SCREEN );
					break;
				// OJW - 20090210 - clean resources on disconnect
				case MAP_EXIT_TO_MAINMENU:
					// Re-initialise the game
					ReStartingGame();
					SetPendingNewScreen( MAINMENU_SCREEN );
					break;
#ifdef JA2UB
				//JA25 ub
				case MAP_EXIT_TO_INTRO_SCREEN:
				//	SetPendingNewScreen( INTRO_SCREEN );
					BeginLoadScreen();
					break;
#endif
				default:
					// invalid exit type
					Assert( FALSE );
			}

			// time compression during mapscreen exit doesn't seem to cause any problems, but turn it off as early as we can
			StopTimeCompression();

			// now leaving mapscreen
			fLeavingMapScreen = TRUE;
		}

		// cancel exit, either we're on our way, or we're not allowed to go
		gbExitingMapScreenToWhere = -1;
	}
}



void MapScreenMsgScrollDown( UINT8 ubLinesDown )
{
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfMapScreenMessages();

	// check if we can go that far, only go as far as we can
	if ( ( gubFirstMapscreenMessageIndex + MAX_MESSAGES_ON_MAP_BOTTOM + ubLinesDown ) > ubNumMessages )
	{
		ubLinesDown = ubNumMessages - gubFirstMapscreenMessageIndex - min( ubNumMessages, MAX_MESSAGES_ON_MAP_BOTTOM );
	}

	if ( ubLinesDown > 0 )
	{
		ChangeCurrentMapscreenMessageIndex( ( UINT8 ) ( gubFirstMapscreenMessageIndex + ubLinesDown ) );
	}
}


void MapScreenMsgScrollUp( UINT8 ubLinesUp )
{
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfMapScreenMessages();

	// check if we can go that far, only go as far as we can
	if ( gubFirstMapscreenMessageIndex < ubLinesUp )
	{
		ubLinesUp = gubFirstMapscreenMessageIndex;
	}

	if ( ubLinesUp > 0 )
	{
		ChangeCurrentMapscreenMessageIndex( ( UINT8 ) ( gubFirstMapscreenMessageIndex - ubLinesUp ) );
	}
}



void MoveToEndOfMapScreenMessageList( void )
{
	UINT8 ubDesiredMessageIndex;
	UINT8 ubNumMessages;

	ubNumMessages = GetRangeOfMapScreenMessages();

	ubDesiredMessageIndex = ubNumMessages - min( ubNumMessages, MAX_MESSAGES_ON_MAP_BOTTOM );
	ChangeCurrentMapscreenMessageIndex( ubDesiredMessageIndex );
}



void ChangeCurrentMapscreenMessageIndex( UINT8 ubNewMessageIndex )
{
	Assert( ubNewMessageIndex + MAX_MESSAGES_ON_MAP_BOTTOM <= max( MAX_MESSAGES_ON_MAP_BOTTOM, GetRangeOfMapScreenMessages() ) );

	gubFirstMapscreenMessageIndex = ubNewMessageIndex;
	gubCurrentMapMessageString = ( gubStartOfMapScreenMessageList + gubFirstMapscreenMessageIndex ) % 256;

	// set fact we just went to a new message
//	gfNewScrollMessage = TRUE;

	// refresh screen
	fMapScreenBottomDirty = TRUE;
}

void InitMapScreenInterfaceBottomCoords( void )
{
	// HEADROCK HAM 3.6: Message window is now as wide as possible.
	MESSAGE_SCROLL_AREA_START_X = xResOffset + xResSize - 307;
	MESSAGE_SCROLL_AREA_END_X = xResOffset + xResSize - 293;
}

