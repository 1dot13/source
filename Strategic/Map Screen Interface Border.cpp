#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Map Screen Interface Border.h"
	#include "Types.h"
	#include "vsurface.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "sgp.h"
	#include "WCheck.h"
	#include "Utilities.h"
	#include "message.h"
	#include "sysutil.h"
	#include "mapscreen.h"
	#include "Render Dirty.h"
	#include "Map Screen Interface Map.h"
	#include "Map Screen Interface Border.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map Inventory.h"
	#include "Map Screen Helicopter.h"
	#include "Text.h"
	#include "Campaign Types.h"
	#include "Strategic Town Loyalty.h"
	#include "strategicmap.h"
	#include "Assignments.h"
	// HEADROCK HAM 4: Now accepts INI settings
	#include "GameSettings.h"
	// Also include Town Militia for checks regarding Mobile Militia Restrictions
	#include "Town Militia.h"
	// Also include Quests, for checking whether a fact is true.
	#include "Quests.h"	
	// HEADROCK HAM 5: Required for inventory filter popup
	#include "popup_callback.h"
	#include "popup_class.h"
#endif

#include "connect.h"

#ifdef JA2UB
#include "ub_config.h"
#endif

//#define MAP_BORDER_CORNER_X 584
//#define MAP_BORDER_CORNER_Y 279

// HEADROCK HAM 4: Now defining X/Y coordinates for Map Bottom buttons. Some day perhaps variable coordinates?
UINT16 MAP_BORDER_TOWN_BTN_X;
UINT16 MAP_BORDER_TOWN_BTN_Y;
UINT16	MAP_BORDER_MINE_BTN_X;
UINT16 MAP_BORDER_MINE_BTN_Y;
UINT16 MAP_BORDER_TEAMS_BTN_X;
UINT16	MAP_BORDER_TEAMS_BTN_Y;
UINT16	MAP_BORDER_AIRSPACE_BTN_X;
UINT16 MAP_BORDER_AIRSPACE_BTN_Y;
UINT16	MAP_BORDER_ITEM_BTN_X;
UINT16 MAP_BORDER_ITEM_BTN_Y;
UINT16	MAP_BORDER_MILITIA_BTN_X;
UINT16 MAP_BORDER_MILITIA_BTN_Y;
UINT16 MAP_BORDER_MOBILE_BTN_X;
UINT16 MAP_BORDER_MOBILE_BTN_Y;

UINT16 MAP_LEVEL_MARKER_X;
UINT16 MAP_LEVEL_MARKER_Y;
UINT16 MAP_LEVEL_MARKER_DELTA;
UINT16 MAP_LEVEL_MARKER_WIDTH;

// extern to anchored button in winbart97
extern GUI_BUTTON *gpAnchoredButton;
extern BOOLEAN	gfAnchoredState;

// mouse levels
MOUSE_REGION LevelMouseRegions[ 4 ];

// graphics
UINT32 guiMapBorder;
//UINT32 guiMapBorderCorner;


// scroll direction
INT32 giScrollButtonState = -1;

// flags
BOOLEAN fShowTownFlag = FALSE;
BOOLEAN fShowMineFlag = FALSE;
BOOLEAN fShowTeamFlag = FALSE;
BOOLEAN fShowMilitia = FALSE;
BOOLEAN fShowAircraftFlag = FALSE;
BOOLEAN fShowItemsFlag = FALSE;
BOOLEAN fShowMobileRestrictionsFlag = FALSE; // HEADROCK HAM 4

BOOLEAN fZoomFlag = FALSE;
//BOOLEAN fShowVehicleFlag = FALSE;

//BOOLEAN fMapScrollDueToPanelButton = FALSE;
//BOOLEAN fCursorIsOnMapScrollButtons = FALSE;
//BOOLEAN fDisabledMapBorder = FALSE;

// HEADROCK HAM 5: Externed here to be able to forgo redrawing the map inventory.
extern POPUP* gMapInventoryFilterPopup;
extern BOOLEAN gfMapInventoryFilterPopupVisible;
extern BOOLEAN gfQueueRecreateMapInventoryFilterMenu;
extern UINT32 guiMapInvenFilterButton[ MAP_INVENTORY_FILTER_BUTTONS ];
extern void CreateMapInventoryFilterMenu( );

// used for ETA border drawing
extern int CLOCK_ETA_X;
extern int CLOCK_Y_START;

// buttons & button images
// HEADROCK HAM 4: Increase both arrays by one to accomodate new Mobile Restrictions button
INT32 giMapBorderButtons[ 7 ] = { -1, -1, -1, -1, -1, -1, -1 };
INT32 giMapBorderButtonsImage[ 7 ] = { -1, -1, -1, -1, -1, -1, -1 };

//UINT32 guiMapBorderScrollButtons[ 4 ] = { -1, -1, -1, -1 };
//UINT32 guiMapBorderScrollButtonsImage[ 4 ];

// raise/lower land buttons
//UINT32 guiMapBorderLandRaiseButtons[ 2 ] = { -1, -1 };
//UINT32 guiMapBorderLandRaiseButtonsImage[ 2 ];



void DeleteMapBorderButtons( void );
BOOLEAN CreateButtonsForMapBorder( void );


//void MapScrollButtonMvtCheck( void );
//BOOLEAN ScrollButtonsDisplayingHelpMessage( void );
//void UpdateScrollButtonStatesWhileScrolling( void );

// set button states to match map flags
void InitializeMapBorderButtonStates( void );

// blit in the level marker
void DisplayCurrentLevelMarker( void );

extern void CancelMapUIMessage( void );



// callbacks
void BtnTownCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMineCallback(GUI_BUTTON *btn,INT32 reason);
void BtnItemCallback(GUI_BUTTON *btn,INT32 reason);
void BtnAircraftCallback(GUI_BUTTON *btn,INT32 reason);
void BtnTeamCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMilitiaCallback(GUI_BUTTON *btn,INT32 reason);
// HEADROCK HAM 4: Mobile Restrictions Button callback
void BtnMobileCallback(GUI_BUTTON *btn,INT32 reason);
//void BtnZoomCallback(GUI_BUTTON *btn,INT32 reason);

void BtnGenericMouseMoveButtonCallbackForMapBorder(GUI_BUTTON *btn,INT32 reason);
void LevelMarkerBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );

void CommonBtnCallbackBtnDownChecks( void );

/*
void BtnScrollNorthMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BtnScrollSouthMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BtnScrollWestMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BtnScrollEastMapScreenCallback( GUI_BUTTON *btn,INT32 reason );
void BtnLowerLevelBtnCallback(GUI_BUTTON *btn,INT32 reason);
void BtnRaiseLevelBtnCallback(GUI_BUTTON *btn,INT32 reason);
*/

BOOLEAN LoadMapBorderGraphics( void )
{
	// this procedure will load the graphics needed for the map border
	VOBJECT_DESC	VObjectDesc;

	// will load map border
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		FilenameForBPP( "INTERFACE\\MBS.sti", VObjectDesc.ImageFile );
	}
	else if (iResolution < _1024x768)
	{
		FilenameForBPP( "INTERFACE\\MBS_800x600.sti", VObjectDesc.ImageFile );
	}
	else
	{
		FilenameForBPP( "INTERFACE\\MBS_1024x768.sti", VObjectDesc.ImageFile );
	}

	CHECKF( AddVideoObject( &VObjectDesc, &guiMapBorder ) );

/* corner was removed along with the Zoom feature
	// will load map border corner
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "INTERFACE\\map_screen_cutout.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiMapBorderCorner ) );

	fCursorIsOnMapScrollButtons = FALSE;
*/


	return ( TRUE );
}


void DeleteMapBorderGraphics( void )
{
	// procedure will delete graphics loaded for map border

	DeleteVideoObjectFromIndex( guiMapBorder );
//	DeleteVideoObjectFromIndex( guiMapBorderCorner );

	return;
}



void RenderMapBorder( void )
{
	// renders the actual border to the guiSAVEBUFFER
	HVOBJECT hHandle;

/*
	if( fDisabledMapBorder )
	{
		return;
	}
*/

	// HEADROCK HAM 5: Do not redraw if the filter menu is open.
	if( fShowMapInventoryPool )
	{
		if (gfMapInventoryFilterPopupVisible)
		{
			if (gfQueueRecreateMapInventoryFilterMenu)
			{
				ButtonList[guiMapInvenFilterButton[ 0 ]]->uiFlags |= (BUTTON_CLICKED_ON);
				CreateMapInventoryFilterMenu();
			}
			gMapInventoryFilterPopup->show();
		}
		else
		{
			// render background, then leave
			BlitInventoryPoolGraphic( );
		}
		return;
	}

	// get and blt border
	GetVideoObject(&hHandle, guiMapBorder );
	// HEADROCK HAM 4: Load different map border depending on whether we want to display the mobile militia button or not.
	if (gGameExternalOptions.gfAllowMilitiaGroups)
	{
		BltVideoObject( guiSAVEBUFFER , hHandle, 1, xResOffset + MAP_BORDER_X, MAP_BORDER_Y, VO_BLT_SRCTRANSPARENCY,NULL );
	}
	else
	{
		BltVideoObject( guiSAVEBUFFER , hHandle, 0, xResOffset + MAP_BORDER_X, MAP_BORDER_Y, VO_BLT_SRCTRANSPARENCY,NULL );
	}

	RestoreExternBackgroundRect( xResOffset + MAP_BORDER_X, MAP_BORDER_Y, SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset, SCREEN_HEIGHT );

	// show the level marker
	DisplayCurrentLevelMarker( );

	return;
}


/*
void RenderMapBorderCorner( void )
{
	// renders map border corner to the FRAME_BUFFER
	HVOBJECT hHandle;

	if( fDisabledMapBorder )
	{
		return;
	}

	if( fShowMapInventoryPool )
	{
		return;
	}


	// get and blt corner
	GetVideoObject(&hHandle, guiMapBorderCorner );
	BltVideoObject( FRAME_BUFFER , hHandle, 0,MAP_BORDER_CORNER_X, MAP_BORDER_CORNER_Y, VO_BLT_SRCTRANSPARENCY,NULL );

	InvalidateRegion( MAP_BORDER_CORNER_X, MAP_BORDER_CORNER_Y, 635, 315);

	return;
}
*/


void RenderMapBorderEtaPopUp( void )
{
	// renders map border corner to the FRAME_BUFFER
	HVOBJECT hHandle;

/*
	if( fDisabledMapBorder )
	{
		return;
	}
*/

	if( fShowMapInventoryPool )
	{
		return;
	}

	if( fPlotForHelicopter == TRUE )
	{
		DisplayDistancesForHelicopter( );
		return;
	}

	// get and blt ETA box
	GetVideoObject(&hHandle, guiMapBorderEtaPopUp );

	// coordinates should depend on the actual ETA display
	UINT16 xVal = (CLOCK_ETA_X - 10) ;
	UINT16 yVal = (CLOCK_Y_START - 7);

	if (iResolution >= _640x480 && iResolution < _800x600)
		BltVideoObject( FRAME_BUFFER , hHandle, 0, xVal, yVal, VO_BLT_SRCTRANSPARENCY,NULL );
	else if (iResolution < _1024x768)
		BltVideoObject( FRAME_BUFFER , hHandle, 1, xVal, yVal, VO_BLT_SRCTRANSPARENCY,NULL );
	else
		BltVideoObject( FRAME_BUFFER , hHandle, 2, xVal, yVal, VO_BLT_SRCTRANSPARENCY,NULL );

	InvalidateRegion( xVal, yVal, xVal + 100 , yVal + 19);

	return;
}

BOOLEAN CreateButtonsForMapBorder( void )
{
	// will create the buttons needed for the map screen border region

	// HEADROCK HAM 4: Call to initiate coordinates for all buttons.
	InitMapBorderButtonCoordinates();
/*
	// up button
	guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_UP ] = LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,11,4,-1,6,-1 );
	guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_UP ] = QuickCreateButton( guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_UP ], 602, 303,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnScrollNorthMapScreenCallback);

	// dwn button
	guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_DWN ] = LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,12,5,-1,7,-1 );
	guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_DWN ] = QuickCreateButton( guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_DWN ], 602, 338,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnScrollSouthMapScreenCallback);

	// left button
	guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_LEFT ] = LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,9,0,-1,2,-1 );
	guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_LEFT ] = QuickCreateButton( guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_LEFT ], 584, 322,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnScrollWestMapScreenCallback);

	// right button
	guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_RIGHT ] = LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,10,1,-1,3,-1 );
	guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_RIGHT ] = QuickCreateButton( guiMapBorderScrollButtonsImage[ ZOOM_MAP_SCROLL_RIGHT ], 619, 322,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnScrollEastMapScreenCallback);

	// set up fast help text
	SetButtonFastHelpText( guiMapBorderScrollButtons[ 0 ], pMapScreenBorderButtonHelpText[ 6 ] );
	SetButtonFastHelpText( guiMapBorderScrollButtons[ 1 ], pMapScreenBorderButtonHelpText[ 7 ] );
	SetButtonFastHelpText( guiMapBorderScrollButtons[ 2 ], pMapScreenBorderButtonHelpText[ 8 ] );
	SetButtonFastHelpText( guiMapBorderScrollButtons[ 3 ], pMapScreenBorderButtonHelpText[ 9 ] );
*/

	// HEADROCK HAM 4: Button X/Y coordinates are now INIT'ed separately.

	// towns
	giMapBorderButtonsImage[ MAP_BORDER_TOWN_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,5,-1,14,-1 );
	giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_TOWN_BTN ], MAP_BORDER_TOWN_BTN_X, MAP_BORDER_TOWN_BTN_Y,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnTownCallback);


	// mines
	giMapBorderButtonsImage[ MAP_BORDER_MINE_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,4,-1,13,-1 );
	giMapBorderButtons[ MAP_BORDER_MINE_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_MINE_BTN ], MAP_BORDER_MINE_BTN_X, MAP_BORDER_MINE_BTN_Y,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnMineCallback);

	// people
	giMapBorderButtonsImage[ MAP_BORDER_TEAMS_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,3,-1,12,-1 );
	giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_TEAMS_BTN ], MAP_BORDER_TEAMS_BTN_X, MAP_BORDER_TEAMS_BTN_Y,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnTeamCallback);

	// militia
	giMapBorderButtonsImage[ MAP_BORDER_MILITIA_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,8,-1,17,-1 );
	giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_MILITIA_BTN ], MAP_BORDER_MILITIA_BTN_X, MAP_BORDER_MILITIA_BTN_Y,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnMilitiaCallback);

	// airspace
	giMapBorderButtonsImage[ MAP_BORDER_AIRSPACE_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,2,-1,11,-1 );
	giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_AIRSPACE_BTN ], MAP_BORDER_AIRSPACE_BTN_X, MAP_BORDER_AIRSPACE_BTN_Y,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnAircraftCallback);

	// items
	giMapBorderButtonsImage[ MAP_BORDER_ITEM_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,1,-1,10,-1 );
	giMapBorderButtons[ MAP_BORDER_ITEM_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_ITEM_BTN ], MAP_BORDER_ITEM_BTN_X, MAP_BORDER_ITEM_BTN_Y,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnItemCallback);

	// WANNE: Only display the buton when mobile militia is allowed!
	// HEADROCK HAM 4: Mobile Restrictions Button
	if (gGameExternalOptions.gfAllowMilitiaGroups)
	{
		giMapBorderButtonsImage[ MAP_BORDER_MOBILE_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,20,-1,21,-1 );
		giMapBorderButtons[ MAP_BORDER_MOBILE_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_MOBILE_BTN ], MAP_BORDER_MOBILE_BTN_X, MAP_BORDER_MOBILE_BTN_Y,
											BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
											(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnMobileCallback);
	}



	// raise and lower view level

	// raise
	/*
	guiMapBorderLandRaiseButtonsImage[ MAP_BORDER_RAISE_LEVEL ] = LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,11,4,-1,6,-1 );
	guiMapBorderLandRaiseButtons[ MAP_BORDER_RAISE_LEVEL ] = QuickCreateButton( guiMapBorderLandRaiseButtonsImage[ MAP_BORDER_RAISE_LEVEL ], MAP_BORDER_X + 264, 322,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnRaiseLevelBtnCallback);

	// lower
	guiMapBorderLandRaiseButtonsImage[ MAP_BORDER_LOWER_LEVEL ] = LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,12,5,-1,7,-1	);
	guiMapBorderLandRaiseButtons[ MAP_BORDER_LOWER_LEVEL ] = QuickCreateButton( guiMapBorderLandRaiseButtonsImage[ MAP_BORDER_LOWER_LEVEL ], MAP_BORDER_X + 264, 340,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnLowerLevelBtnCallback);

*/
	// set up fast help text
	SetButtonFastHelpText( giMapBorderButtons[ 0 ], pMapScreenBorderButtonHelpText[ 0 ] );
	SetButtonFastHelpText( giMapBorderButtons[ 1 ], pMapScreenBorderButtonHelpText[ 1 ] );
	SetButtonFastHelpText( giMapBorderButtons[ 2 ], pMapScreenBorderButtonHelpText[ 2 ] );
	SetButtonFastHelpText( giMapBorderButtons[ 3 ], pMapScreenBorderButtonHelpText[ 3 ] );
	SetButtonFastHelpText( giMapBorderButtons[ 4 ], pMapScreenBorderButtonHelpText[ 4 ] );
	SetButtonFastHelpText( giMapBorderButtons[ 5 ], pMapScreenBorderButtonHelpText[ 5 ] );

	if (gGameExternalOptions.gfAllowMilitiaGroups)
		SetButtonFastHelpText( giMapBorderButtons[ 6 ], pMapScreenBorderButtonHelpText[ 6 ] ); // HEADROCK HAM 4: Mobile Militia button

	//SetButtonFastHelpText( guiMapBorderLandRaiseButtons[ 0 ], pMapScreenBorderButtonHelpText[ 10 ] );
	//SetButtonFastHelpText( guiMapBorderLandRaiseButtons[ 1 ], pMapScreenBorderButtonHelpText[ 11 ] );

	SetButtonCursor(giMapBorderButtons[ 0 ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ 1 ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ 2 ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ 3 ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ 4 ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ 5 ], MSYS_NO_CURSOR );

	if (gGameExternalOptions.gfAllowMilitiaGroups)
		SetButtonCursor(giMapBorderButtons[ 6 ], MSYS_NO_CURSOR ); // HEADROCK HAM 4: Mobile Militia button

//	SetButtonCursor(guiMapBorderLandRaiseButtons[ 0 ], MSYS_NO_CURSOR );
//	SetButtonCursor(guiMapBorderLandRaiseButtons[ 1 ], MSYS_NO_CURSOR );


#ifdef JA2UB
    //EnableButton
      
	if (gGameUBOptions.BorderTown == TRUE)  
	   { 
	   EnableButton( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ]); 
	   }  
	   else
	   {
	   DisableButton( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ]); 
	   }
	   
	if (gGameUBOptions.BorderMine == TRUE)  
	   { 
	   EnableButton( giMapBorderButtons[ MAP_BORDER_MINE_BTN ]); 
	   }  
	   else
	   {
	   DisableButton( giMapBorderButtons[ MAP_BORDER_MINE_BTN ]); 
	   }
	
	if (gGameUBOptions.BorderTeams == TRUE)  
	   { 
	   EnableButton( giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ]); 
	   }  
	   else
	   {
	   DisableButton( giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ]); 
	   }
	
	if (gGameUBOptions.BorderMilitia == TRUE)  
	   { 
	   EnableButton( giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ]); 
	   }  
	   else
	   {
	   DisableButton( giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ]); 
	   }      
	   
	if (gGameUBOptions.BorderAirspace == TRUE)  
	   { 
	   EnableButton( giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ]); 
	   }  
	   else
	   {
	   DisableButton( giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ]); 
	   }      

	if (gGameUBOptions.BorderItem == TRUE)  
	   { 
	   EnableButton( giMapBorderButtons[ MAP_BORDER_ITEM_BTN ]); 
	   }  
	   else
	   {
	   DisableButton( giMapBorderButtons[ MAP_BORDER_ITEM_BTN ]); 
	   }  
#endif

#ifdef JA113DEMO
	   DisableButton( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ]); 
	   DisableButton( giMapBorderButtons[ MAP_BORDER_MINE_BTN ]); 
	   DisableButton( giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ]); 
	   DisableButton( giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ]); 
	   DisableButton( giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ]); 
	   DisableButton( giMapBorderButtons[ MAP_BORDER_ITEM_BTN ]); 
	   DisableButton( giMapBorderButtons[ MAP_BORDER_MOBILE_BTN ]); 
	   
	   fShowTownFlag = FALSE;
#endif

	InitializeMapBorderButtonStates( );

	return( TRUE );
}


void DeleteMapBorderButtons( void )
{
	UINT8 ubCnt;

/*
	RemoveButton( guiMapBorderScrollButtons[ 0 ]);
	RemoveButton( guiMapBorderScrollButtons[ 1 ]);
	RemoveButton( guiMapBorderScrollButtons[ 2 ]);
	RemoveButton( guiMapBorderScrollButtons[ 3 ]);
*/

	RemoveButton( giMapBorderButtons[ 0 ]);
	RemoveButton( giMapBorderButtons[ 1 ]);
	RemoveButton( giMapBorderButtons[ 2 ]);
	RemoveButton( giMapBorderButtons[ 3 ]);
	RemoveButton( giMapBorderButtons[ 4 ]);
	RemoveButton( giMapBorderButtons[ 5 ]);

	// WANNE: Only remove if we added the button
	if (gGameExternalOptions.gfAllowMilitiaGroups)
		RemoveButton( giMapBorderButtons[ 6 ]); // HEADROCK HAM 4

	//RemoveButton( guiMapBorderLandRaiseButtons[ 0 ]);
	//RemoveButton( guiMapBorderLandRaiseButtons[ 1 ]);

	// images

/*
	UnloadButtonImage( guiMapBorderScrollButtonsImage[ 0 ] );
	UnloadButtonImage( guiMapBorderScrollButtonsImage[ 1 ] );
	UnloadButtonImage( guiMapBorderScrollButtonsImage[ 2 ] );
	UnloadButtonImage( guiMapBorderScrollButtonsImage[ 3 ] );
*/

	UnloadButtonImage( giMapBorderButtonsImage[ 0 ] );
	UnloadButtonImage( giMapBorderButtonsImage[ 1 ] );
	UnloadButtonImage( giMapBorderButtonsImage[ 2 ] );
	UnloadButtonImage( giMapBorderButtonsImage[ 3 ] );
	UnloadButtonImage( giMapBorderButtonsImage[ 4 ] );
	UnloadButtonImage( giMapBorderButtonsImage[ 5 ] );

	// WANNE: Only unload if we added the button
	if (gGameExternalOptions.gfAllowMilitiaGroups)
		UnloadButtonImage( giMapBorderButtonsImage[ 6 ] ); // HEADROCK HAM 4


	//UnloadButtonImage( guiMapBorderLandRaiseButtonsImage[ 0 ] );
	//UnloadButtonImage( guiMapBorderLandRaiseButtonsImage[ 1 ] );


	// HEADROCK HAM 4: Increased number of buttons by one.
	for ( ubCnt = 0; ubCnt < NUM_MAP_BORDER_BTNS; ubCnt++ )
	{
		giMapBorderButtons[ ubCnt ] = -1;
		giMapBorderButtonsImage[ ubCnt ] = -1;
	}
}


// callbacks

/*
void BtnLowerLevelBtnCallback(GUI_BUTTON *btn,INT32 reason)
{


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		// are help messages being displayed?..redraw
		if( ScrollButtonsDisplayingHelpMessage( ) )
		{
			fMapPanelDirty = TRUE;
		}

		MarkButtonsDirty( );

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

		// go down one level
		GoDownOneLevelInMap( );
		}
	}
}


void BtnRaiseLevelBtnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		// are help messages being displayed?..redraw
		if( ScrollButtonsDisplayingHelpMessage( ) )
		{
			fMapPanelDirty = TRUE;
		}


		MarkButtonsDirty( );

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
			// go up one level
		GoUpOneLevelInMap( );
		}
	}
}
*/


void BtnMilitiaCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
		ToggleShowMilitiaMode( );
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}


void BtnTeamCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
		ToggleShowTeamsMode();
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}


void BtnTownCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
		ToggleShowTownsMode();
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}


void BtnMineCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
		ToggleShowMinesMode();
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}


void BtnAircraftCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();

		ToggleAirspaceMode();
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}


void BtnItemCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();

		ToggleItemsFilter();
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}

// HEADROCK HAM 4: Callback for Mobile Restrictions Button
void BtnMobileCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();

		ToggleMobileFilter();
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}


/*
void BtnZoomCallback(GUI_BUTTON *btn,INT32 reason)
{
	UINT16 sTempXOff=0;
	UINT16 sTempYOff=0;


	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();

		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		fZoomFlag=FALSE;
		}
		else
		{
		btn->uiFlags|=(BUTTON_CLICKED_ON);
		fZoomFlag=TRUE;
		if( sSelMapX > 14 )
		{
			iZoomX = ( ( sSelMapX + 2 ) / 2 ) * ( MAP_GRID_X * 2 );
		}
		else
		{
			iZoomX=sSelMapX/2*MAP_GRID_X*2;
		}

		if( sOldSelMapY > 14 )
		{
			iZoomY = ( ( sSelMapY + 2 ) / 2 ) * ( MAP_GRID_Y * 2 );
		}
		else
		{
			iZoomY=sSelMapY/2*MAP_GRID_Y*2;
		}

		}

		fMapPanelDirty=TRUE;
	}
	else if(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();
	}
}
*/


void ToggleShowTownsMode( void )
{
	if( fShowTownFlag == TRUE )
	{
		fShowTownFlag = FALSE;
		MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
	}
	else
	{
		fShowTownFlag = TRUE;
		MapBorderButtonOn( MAP_BORDER_TOWN_BTN );

		if( fShowMineFlag == TRUE )
		{
			fShowMineFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MINE_BTN );
		}

		if( fShowAircraftFlag == TRUE )
		{
			fShowAircraftFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		}

		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}
	}

	fMapPanelDirty = TRUE;
}


void ToggleShowMinesMode( void )
{
	if( fShowMineFlag == TRUE )
	{
		fShowMineFlag = FALSE;
		MapBorderButtonOff( MAP_BORDER_MINE_BTN );
	}
	else
	{
		fShowMineFlag = TRUE;
		MapBorderButtonOn( MAP_BORDER_MINE_BTN );

		if( fShowTownFlag == TRUE )
		{
			fShowTownFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
		}

		if( fShowAircraftFlag == TRUE )
		{
			fShowAircraftFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		}

		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}
		// HEADROCK HAM 4: Mobile Militia Restrictions
		if( fShowMobileRestrictionsFlag == TRUE )
		{
			fShowMobileRestrictionsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		}
	}

	fMapPanelDirty = TRUE;
}


void ToggleShowMilitiaMode( void )
{
	if( fShowMilitia == TRUE )
	{
		fShowMilitia = FALSE;
		MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
	}
	else
	{
		// toggle militia ON
		fShowMilitia = TRUE;
		MapBorderButtonOn( MAP_BORDER_MILITIA_BTN );

		// if Team is ON, turn it OFF
		if( fShowTeamFlag == TRUE )
		{
			fShowTeamFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
		}

/*
		// if Airspace is ON, turn it OFF
		if( fShowAircraftFlag == TRUE )
		{
			fShowAircraftFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		}
*/

		if ( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}


		// check if player has any militia
		if ( DoesPlayerHaveAnyMilitia( ) == FALSE )
		{
			STR16 pwString = NULL;

			// no - so put up a message explaining how it works

			// if he's already training some
			// HEADROCK HAM 3.6: Also for Mobile trainers.
			if( IsAnyOneOnPlayersTeamOnThisAssignment( TRAIN_TOWN ) ||
				IsAnyOneOnPlayersTeamOnThisAssignment( TRAIN_MOBILE ) )
			{
				// say they'll show up when training is completed
				pwString = pMapErrorString[ 28 ];
			}
			else
			{
				// say you need to train them first
				pwString = zMarksMapScreenText[ 1 ];
			}

			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pwString );
		}
	}

	fMapPanelDirty = TRUE;
}


void ToggleShowTeamsMode( void )
{
	if( fShowTeamFlag == TRUE )
	{
		// turn show teams OFF
		fShowTeamFlag = FALSE;
		MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{	// turn show teams ON
		TurnOnShowTeamsMode();
	}
}


void ToggleAirspaceMode( void )
{
	if( fShowAircraftFlag == TRUE )
	{
		// turn airspace OFF
		fShowAircraftFlag = FALSE;
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );

		if( fPlotForHelicopter == TRUE )
		{
			AbortMovementPlottingMode( );
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode( );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{	// turn airspace ON
		TurnOnAirSpaceMode();
	}
}


void ToggleItemsFilter( void )
{
	if( fShowItemsFlag == TRUE )
	{
		// turn items OFF
		fShowItemsFlag = FALSE;
		MapBorderButtonOff( MAP_BORDER_ITEM_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
		// turn items ON
		TurnOnItemFilterMode();
	}
}

// HEADROCK HAM 4: Toggle Mobile Restrictions Button
void ToggleMobileFilter( void )
{
	if( fShowMobileRestrictionsFlag == TRUE )
	{
		// turn items OFF
		fShowMobileRestrictionsFlag = FALSE;
		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
		// turn items ON
		TurnOnMobileFilterMode();
	}
}

/*
void ShowDestinationOfPlottedPath( STR16 pLoc )
{
	INT16 sFontX, sFontY;

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, 640, 480, FALSE );

	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	VarFindFontCenterCoordinates( 461, 344,	70, 12,	COMPFONT, &sFontX, &sFontY, pLoc );
	gprintfdirty(sFontX, sFontY, pLoc );
	mprintf(sFontX, sFontY, pLoc	);

	return;
}
*/

/*
void BtnScrollNorthMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

		// not zoomed in?...don't push down
	if( fZoomFlag == FALSE )
		{
			return;
		}

		// are help messages being displayed?..redraw
		if( ScrollButtonsDisplayingHelpMessage( ) )
		{
			fMapPanelDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
	 btn->uiFlags&=~(BUTTON_CLICKED_ON);
		giScrollButtonState = NORTH_DIR;
		fMapScrollDueToPanelButton = TRUE;
	}
	}
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		giScrollButtonState = NORTH_DIR;
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
}

void BtnScrollSouthMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

		// not zoomed in?...don't push down
	if( fZoomFlag == FALSE )
		{
			return;
		}

		// are help messages being displayed?..redraw
		if( ScrollButtonsDisplayingHelpMessage( ) )
		{
			fMapPanelDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		giScrollButtonState = SOUTH_DIR;
		fMapScrollDueToPanelButton = TRUE;

	}
	}
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		giScrollButtonState = SOUTH_DIR;
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
}

void BtnScrollEastMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	// not zoomed in?...don't push down
	if( fZoomFlag == FALSE )
		{
			return;
		}

		// are help messages being displayed?..redraw
		if( ScrollButtonsDisplayingHelpMessage( ) )
		{
			fMapPanelDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		giScrollButtonState = EAST_DIR;
		fMapScrollDueToPanelButton = TRUE;

	}
	}
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		giScrollButtonState = EAST_DIR;
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
}

void BtnScrollWestMapScreenCallback( GUI_BUTTON *btn,INT32 reason )
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{

	// not zoomed in?...don't push down
	if( fZoomFlag == FALSE )
		{
			return;
		}

		// are help messages being displayed?..redraw
		if( ScrollButtonsDisplayingHelpMessage( ) )
		{
			fMapPanelDirty = TRUE;
		}

	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		giScrollButtonState = WEST_DIR;
		fMapScrollDueToPanelButton = TRUE;

	}
	}
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		giScrollButtonState = WEST_DIR;
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
}


void MapScrollButtonMvtCheck( void	)
{
	// run through each button's mouse region, if mouse cursor there...don't show map white sector highlight
	fCursorIsOnMapScrollButtons = FALSE;

	if( ButtonList[ guiMapBorderScrollButtons[ 0 ] ]->Area.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		fCursorIsOnMapScrollButtons = TRUE;
	}

	if( ButtonList[ guiMapBorderScrollButtons[ 1 ] ]->Area.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		fCursorIsOnMapScrollButtons = TRUE;
	}

	if( ButtonList[ guiMapBorderScrollButtons[ 2 ] ]->Area.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		fCursorIsOnMapScrollButtons = TRUE;
	}

	if( ButtonList[ guiMapBorderScrollButtons[ 3 ] ]->Area.uiFlags & MSYS_MOUSE_IN_AREA )
	{
		fCursorIsOnMapScrollButtons = TRUE;
	}
}
*/


/*
void HandleMapScrollButtonStates( void )
{
	// will enable/disable map scroll buttons based on zoom mode

	if( fDisabledMapBorder || fShowMapInventoryPool )
	{
		return;
	}

	// if underground, don't want zoom in
	if( iCurrentMapSectorZ )
	{
		if( fZoomFlag == TRUE )
		{
			fZoomFlag = FALSE;
			fMapPanelDirty = TRUE;
		}

		MapBorderButtonOff( MAP_BORDER_ZOOM_BTN );
		DisableButton( giMapBorderButtons[ MAP_BORDER_ZOOM_BTN ]);
	}
	else
	{
		EnableButton( giMapBorderButtons[ MAP_BORDER_ZOOM_BTN ]);
	}

	if( fZoomFlag )
	{
		EnableButton( guiMapBorderScrollButtons[ 0 ]);
	EnableButton( guiMapBorderScrollButtons[ 1 ]);
	EnableButton( guiMapBorderScrollButtons[ 2 ]);
	EnableButton( guiMapBorderScrollButtons[ 3 ]);

		UpdateScrollButtonStatesWhileScrolling(	);

	}
	else
	{

		DisableButton( guiMapBorderScrollButtons[ 0 ]);
	DisableButton( guiMapBorderScrollButtons[ 1 ]);
	DisableButton( guiMapBorderScrollButtons[ 2 ]);
	DisableButton( guiMapBorderScrollButtons[ 3 ]);

	}

	// check mvt too
	MapScrollButtonMvtCheck( );


	return;
}
*/


// generic button mvt callback for mapscreen map border
void BtnGenericMouseMoveButtonCallbackForMapBorder(GUI_BUTTON *btn,INT32 reason)
{
	//If the button isn't the anchored button, then we don't want to modify the button state.

	if( btn != gpAnchoredButton )
	{
		if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
		{
			if( btn->Area.uiFlags & MSYS_FASTHELP )
			{
			// redraw area
			fMapPanelDirty = TRUE;
			}
		}
		return;
	}

	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		if( !gfAnchoredState )
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON ;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


/*
BOOLEAN ScrollButtonsDisplayingHelpMessage( void )
{
	// return if any help messages are being displayed for the scroll buttons

	if( ( ButtonList[ guiMapBorderScrollButtons[ 0 ] ]->Area.uiFlags & MSYS_HAS_BACKRECT )||
	( ButtonList[ guiMapBorderScrollButtons[ 1 ] ]->Area.uiFlags & MSYS_HAS_BACKRECT )||
	( ButtonList[ guiMapBorderScrollButtons[ 2 ] ]->Area.uiFlags & MSYS_HAS_BACKRECT )||
	( ButtonList[ guiMapBorderScrollButtons[ 3 ] ]->Area.uiFlags & MSYS_HAS_BACKRECT ) )
	{
		return( TRUE );
	}

	return( FALSE );
}
*/


void DisplayCurrentLevelMarker( void )
{
	// display the current level marker on the map border
	HVOBJECT hHandle;

/*
	if( fDisabledMapBorder )
	{
		return;
	}
*/

	// it's actually a white rectangle, not a green arrow!
	GetVideoObject(&hHandle, guiLEVELMARKER );
	BltVideoObject( guiSAVEBUFFER , hHandle, 0,	MAP_LEVEL_MARKER_X + 1, MAP_LEVEL_MARKER_Y + ( MAP_LEVEL_MARKER_DELTA * ( INT16 )iCurrentMapSectorZ ), VO_BLT_SRCTRANSPARENCY,NULL );

	RestoreExternBackgroundRect(MAP_LEVEL_MARKER_X + 1, MAP_LEVEL_MARKER_Y + ( MAP_LEVEL_MARKER_DELTA * ( INT16 )iCurrentMapSectorZ ), 55, 9);


	return;
}


void CreateMouseRegionsForLevelMarkers( void )
{
	INT16 sCounter = 0;
	CHAR16 sString[ 64 ];

	for( sCounter = 0; sCounter	< 4 ; sCounter++ )
	{
		MSYS_DefineRegion(&LevelMouseRegions[ sCounter ], MAP_LEVEL_MARKER_X, ( INT16 )( MAP_LEVEL_MARKER_Y + ( MAP_LEVEL_MARKER_DELTA * sCounter ) ),	MAP_LEVEL_MARKER_X + MAP_LEVEL_MARKER_WIDTH, ( INT16 )( MAP_LEVEL_MARKER_Y + ( MAP_LEVEL_MARKER_DELTA * ( sCounter + 1 ) ) ), MSYS_PRIORITY_HIGH, MSYS_NO_CURSOR,
			MSYS_NO_CALLBACK, LevelMarkerBtnCallback );

		MSYS_SetRegionUserData(&LevelMouseRegions[sCounter],0,sCounter);

		swprintf( sString, L"%s %d", zMarksMapScreenText[ 0 ], sCounter + 1 );
		SetRegionFastHelpText( &LevelMouseRegions[ sCounter ], sString );
	}


}


void DeleteMouseRegionsForLevelMarkers( void )
{
	INT16 sCounter = 0;


	for( sCounter = 0; sCounter < 4; sCounter++ )
	{
		MSYS_RemoveRegion(	&LevelMouseRegions[ sCounter ] );
	}
}

void LevelMarkerBtnCallback(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment screen mask region
	INT32 iCounter = 0;

	iCounter = MSYS_GetRegionUserData( pRegion, 0 );

	if( ( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP ) )
	{
		JumpToLevel( iCounter );
	}
}


/*
void DisableMapBorderRegion( void )
{
	// will shutdown map border region

	if( fDisabledMapBorder )
	{
		// checked, failed
		return;
	}

	// get rid of graphics and mouse regions
	DeleteMapBorderGraphics( );


	fDisabledMapBorder = TRUE;
}

void EnableMapBorderRegion( void )
{
	// will re-enable mapborder region

	if( fDisabledMapBorder == FALSE )
	{
		// checked, failed
		return;
	}

	// re load graphics and buttons
	LoadMapBorderGraphics( );

	fDisabledMapBorder = FALSE;

}
*/


void TurnOnShowTeamsMode( void )
{
	// if mode already on, leave, else set and redraw

	if( fShowTeamFlag == FALSE )
	{
		fShowTeamFlag = TRUE;
		MapBorderButtonOn( MAP_BORDER_TEAMS_BTN );

		if( fShowMilitia == TRUE )
		{
			fShowMilitia = FALSE;
			MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
		}
		// HEADROCK HAM 4: Turn off Show Militia Restrictions
		if (fShowMobileRestrictionsFlag == TRUE)
		{
			fShowMobileRestrictionsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		}

/*
		if( fShowAircraftFlag == TRUE )
		{
			fShowAircraftFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		}
*/

		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
}



void TurnOnAirSpaceMode( void )
{
	// if mode already on, leave, else set and redraw

	if( fShowAircraftFlag == FALSE )
	{
		fShowAircraftFlag = TRUE;
		MapBorderButtonOn( MAP_BORDER_AIRSPACE_BTN );


		// Turn off towns & mines (mostly because town/mine names overlap SAM site names)
		if( fShowTownFlag == TRUE )
		{
			fShowTownFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
		}

		if( fShowMineFlag == TRUE )
		{
			fShowMineFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MINE_BTN );
		}

/*
		// Turn off teams and militia
		if( fShowTeamFlag == TRUE )
		{
			fShowTeamFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
		}

		if( fShowMilitia == TRUE )
		{
			fShowMilitia = FALSE;
			MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
		}
*/
		// HEADROCK HAM 4: Turn off Militia Restrictions
		if (fShowMobileRestrictionsFlag == TRUE)
		{
			fShowMobileRestrictionsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		}

		// Turn off items
		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		if ( bSelectedDestChar != -1 )
		{
			AbortMovementPlottingMode( );
		}


		// if showing underground
		if ( iCurrentMapSectorZ != 0 )
		{
			// switch to the surface
			JumpToLevel( 0 );
		}

		// WANNE: Output info text on the airspace for changing maps
		if (is_server )
		{			
			STR16 pwString = MPServerMessage[ 13 ];

			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pwString );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
}


void TurnOnItemFilterMode( void )
{
	// if mode already on, leave, else set and redraw

	if( fShowItemsFlag == FALSE )
	{
		fShowItemsFlag = TRUE;
		MapBorderButtonOn( MAP_BORDER_ITEM_BTN );


		// Turn off towns, mines, teams, militia & airspace if any are on
		if( fShowTownFlag == TRUE )
		{
			fShowTownFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
		}

		if( fShowMineFlag == TRUE )
		{
			fShowMineFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MINE_BTN );
		}

		if( fShowTeamFlag == TRUE )
		{
			fShowTeamFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
		}

		if( fShowMilitia == TRUE )
		{
			fShowMilitia = FALSE;
			MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
		}
		// HEADROCK HAM 4: Turn off Militia Restrictions
		if (fShowMobileRestrictionsFlag == TRUE)
		{
			fShowMobileRestrictionsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		}

		if( fShowAircraftFlag == TRUE )
		{
			fShowAircraftFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		}

		if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
		{
			AbortMovementPlottingMode( );
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode( );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
}

// HEADROCK HAM 4: Activate "View Mobile Restrictions" mode.
void TurnOnMobileFilterMode( void )
{
	// if mode already on, leave, else set and redraw

	if( fShowMobileRestrictionsFlag == FALSE )
	{
		fShowMobileRestrictionsFlag = TRUE;
		MapBorderButtonOn( MAP_BORDER_MOBILE_BTN );
		// Also turn on Militia mode
		fShowMilitia = FALSE; // Fool the function so that it always turns militia ON.
		ToggleShowMilitiaMode();


		if( fShowMineFlag == TRUE )
		{
			fShowMineFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MINE_BTN );
		}

		if( fShowTeamFlag == TRUE )
		{
			fShowTeamFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
		}

		if( fShowAircraftFlag == TRUE )
		{
			fShowAircraftFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		}

		// Turn off items
		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		if( ( bSelectedDestChar != -1 ) || ( fPlotForHelicopter == TRUE ) )
		{
			AbortMovementPlottingMode( );
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode( );
		}

		STR16 pwString = NULL;

		// check if player has any Mobile militia
		if ( DoesPlayerHaveAnyMobileMilitia( ) == 1 )
		{
			// say you need to train mobiles first
			pwString = zMarksMapScreenText[ 25 ];

			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pwString );
		}

		if ( !gubFact[ FACT_MOBILE_RESTRICTIONS_VIEWED ] )
		{
			// say you need to train mobiles first
			pwString = zMarksMapScreenText[ 26 ];

			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, pwString );

			SetFactTrue( FACT_MOBILE_RESTRICTIONS_VIEWED );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
}

/*
void UpdateLevelButtonStates( void )
{

	if( iCurrentMapSectorZ == 0 )
	{
		DisableButton( guiMapBorderLandRaiseButtons[ MAP_BORDER_RAISE_LEVEL ] );
	}
	else
	{
		EnableButton( guiMapBorderLandRaiseButtons[ MAP_BORDER_RAISE_LEVEL ] );
	}

	if( iCurrentMapSectorZ == 3 )
	{
		DisableButton( guiMapBorderLandRaiseButtons[ MAP_BORDER_LOWER_LEVEL ] );
	}
	else
	{
		EnableButton( guiMapBorderLandRaiseButtons[ MAP_BORDER_LOWER_LEVEL ] );
	}

	return;
}
*/


/*
void UpdateScrollButtonStatesWhileScrolling( void )
{
	// too far west, disable
	if ( iZoomY == NORTH_ZOOM_BOUND )
	{
		ButtonList[ guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_UP ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
		DisableButton( guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_UP ] );
	}
	else if(iZoomY == SOUTH_ZOOM_BOUND )
	{
		ButtonList[ guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_DWN ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
		DisableButton( guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_DWN ] );
	}

	// too far west, disable
	if ( iZoomX == WEST_ZOOM_BOUND )
	{
		ButtonList[ guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_LEFT ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
		DisableButton( guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_LEFT ] );
	}
	else if(iZoomX == EAST_ZOOM_BOUND )
	{
		ButtonList[ guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_RIGHT ] ]->uiFlags&=~(BUTTON_CLICKED_ON);
		DisableButton( guiMapBorderScrollButtons[ ZOOM_MAP_SCROLL_RIGHT ] );
	}

}
*/


void InitializeMapBorderButtonStates( void )
{
	if( fShowItemsFlag )
	{
		MapBorderButtonOn( MAP_BORDER_ITEM_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
	}

	if( fShowTownFlag )
	{
		MapBorderButtonOn( MAP_BORDER_TOWN_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
	}

	if( fShowMineFlag )
	{
		MapBorderButtonOn( MAP_BORDER_MINE_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_MINE_BTN );
	}

	if( fShowTeamFlag )
	{
		MapBorderButtonOn( MAP_BORDER_TEAMS_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
	}

	if( fShowAircraftFlag )
	{
		MapBorderButtonOn( MAP_BORDER_AIRSPACE_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
	}

	if( fShowMilitia )
	{
		MapBorderButtonOn( MAP_BORDER_MILITIA_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
	}

	if( fShowMobileRestrictionsFlag )
	{
		MapBorderButtonOn( MAP_BORDER_MOBILE_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
	}
}



BOOLEAN DoesPlayerHaveAnyMilitia( void )
{
	INT16 sX, sY;

	// run through list of towns that might have militia..if any return TRUE..else return FALSE
	for( sX = 1; sX < MAP_WORLD_X - 1; sX++ )
	{
		for( sY = 1; sY < MAP_WORLD_Y - 1; sY++ )
		{
			if( ( SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ GREEN_MILITIA ] +	SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ]
					+ SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ ELITE_MILITIA ] ) > 0 )
			{
				// found at least one
				return( TRUE );
			}
		}
	}

	// no one found
	return( FALSE );
}

// HEADROCK HAM 4: Check for Mobile Militia
UINT8 DoesPlayerHaveAnyMobileMilitia( void )
{
	INT16 sX, sY;

	if (!gGameExternalOptions.gfAllowMilitiaGroups)
	{
		// Mobile Militia not allowed at all.
		return (0);
	}

	// run through list of towns that might have militia..if any return TRUE..else return FALSE
	for( sX = 1; sX < MAP_WORLD_X - 1; sX++ )
	{
		for( sY = 1; sY < MAP_WORLD_Y - 1; sY++ )
		{
			// Look only in sectors where Militia Training is not allowed at all. If any militia are found there,
			// it means that they had to MOVE there, hence mobile militia.
			if (!MilitiaTrainingAllowedInSector( sX, sY, 0 ))
			{
				if( ( SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ GREEN_MILITIA ] +	SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ REGULAR_MILITIA ]
						+ SectorInfo[ SECTOR( sX, sY )].ubNumberOfCivsAtLevel[ ELITE_MILITIA ] ) > 0 )
				{
					// found at least one
					return( 2 );
				}
			}
		}
	}

	// Militia are allowed, but none have been found.
	return( 1 );
}


void CommonBtnCallbackBtnDownChecks( void )
{
	if( IsMapScreenHelpTextUp() )
	{
		// stop mapscreen text
		StopMapScreenHelpText( );
	}

	// any click cancels MAP UI messages, unless we're in confirm map move mode
	if( ( giUIMessageOverlay != -1 ) && !gfInConfirmMapMoveMode )
	{
		CancelMapUIMessage( );
	}
}



void InitMapScreenFlags( void )
{
	fShowTownFlag = TRUE;
	fShowMineFlag = FALSE;

	fShowTeamFlag = TRUE;
	fShowMilitia = FALSE;

	fShowAircraftFlag = FALSE;
	fShowItemsFlag = FALSE;

	// HEADROCK HAM 4: Militia Restrictions
	fShowMobileRestrictionsFlag = FALSE;
}



void MapBorderButtonOff( UINT8 ubBorderButtonIndex )
{
	Assert( ubBorderButtonIndex < NUM_MAP_BORDER_BTNS );

	if( fShowMapInventoryPool )
	{
		return;
	}

	// if button doesn't exist, return
	if ( giMapBorderButtons[ ubBorderButtonIndex ] == -1 )
	{
		return;
	}

	Assert( giMapBorderButtons[ ubBorderButtonIndex ] < MAX_BUTTONS );

	ButtonList[ giMapBorderButtons[ ubBorderButtonIndex ] ]->uiFlags &= ~(BUTTON_CLICKED_ON);
}


void MapBorderButtonOn( UINT8 ubBorderButtonIndex )
{
	Assert( ubBorderButtonIndex < NUM_MAP_BORDER_BTNS );

	if( fShowMapInventoryPool )
	{
		return;
	}

	// if button doesn't exist, return
	if ( giMapBorderButtons[ ubBorderButtonIndex ] == -1 )
	{
		return;
	}

	Assert( giMapBorderButtons[ ubBorderButtonIndex ] < MAX_BUTTONS );

	ButtonList[ giMapBorderButtons[ ubBorderButtonIndex ] ]->uiFlags |= BUTTON_CLICKED_ON;
}

// HEADROCK HAM 4: Init the coordinates for all Map Border buttons
void InitMapBorderButtonCoordinates()
{
	UINT32 buttonOffset = 155;	// 160

	MAP_BORDER_TOWN_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 152;
	MAP_BORDER_TOWN_BTN_Y 		= (yResSize - buttonOffset);
	MAP_BORDER_MINE_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 109;
	MAP_BORDER_MINE_BTN_Y 		= (yResSize - buttonOffset);
	MAP_BORDER_TEAMS_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 66;
	MAP_BORDER_TEAMS_BTN_Y 		= (yResSize - buttonOffset);
	MAP_BORDER_AIRSPACE_BTN_X	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 20;
	MAP_BORDER_AIRSPACE_BTN_Y	= (yResSize - buttonOffset);
	MAP_BORDER_ITEM_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 63;
	MAP_BORDER_ITEM_BTN_Y 		= (yResSize - buttonOffset);
	MAP_BORDER_MILITIA_BTN_X 	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 23;
	MAP_BORDER_MILITIA_BTN_Y 	= (yResSize - buttonOffset);
	MAP_BORDER_MOBILE_BTN_X 	= xResSize;
	MAP_BORDER_MOBILE_BTN_Y 	= 0;

	MAP_LEVEL_MARKER_X 			= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 114;
	MAP_LEVEL_MARKER_Y 			= (yResSize - buttonOffset);
	MAP_LEVEL_MARKER_DELTA 		= 8;
	MAP_LEVEL_MARKER_WIDTH 		= 55;

	if (gGameExternalOptions.gfAllowMilitiaGroups)
	{
		// Mobile button appears next to Militia button.
		MAP_BORDER_MOBILE_BTN_X 	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 16;
		MAP_BORDER_MOBILE_BTN_Y 	= (yResSize - buttonOffset);

		// Airspace, Items, ZLevel buttons all moved to the right (+22px, +22px, +10px).
		MAP_BORDER_AIRSPACE_BTN_X 	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 42;
		MAP_BORDER_ITEM_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 85;

		MAP_LEVEL_MARKER_X 			= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 124;
	}
}
