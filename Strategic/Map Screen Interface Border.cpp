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
	#include "Queen Command.h"	// added by Flugente
#endif

#include "connect.h"

#ifdef JA2UB
#include "ub_config.h"
#endif

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
UINT16 MAP_BORDER_DISEASE_BTN_X;	// Flugente: disease
UINT16 MAP_BORDER_DISEASE_BTN_Y;

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

// scroll direction
INT32 giScrollButtonState = -1;

// flags
BOOLEAN fShowTownFlag = FALSE;
BOOLEAN fShowMineFlag = FALSE;
BOOLEAN fShowTeamFlag = FALSE;
BOOLEAN fShowMilitia = FALSE;
BOOLEAN fShowItemsFlag = FALSE;

// Flugente: unified several display modes
UINT8 gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;


//BOOLEAN fShowVehicleFlag = FALSE;

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
INT32 giMapBorderButtons[ NUM_MAP_BORDER_BTNS ] = { -1, -1, -1, -1, -1, -1, -1, -1 };
INT32 giMapBorderButtonsImage[ NUM_MAP_BORDER_BTNS ] = { -1, -1, -1, -1, -1, -1, -1, -1 };

void DeleteMapBorderButtons( void );
BOOLEAN CreateButtonsForMapBorder( void );

// set button states to match map flags
void InitializeMapBorderButtonStates( void );

// blit in the level marker
void DisplayCurrentLevelMarker( void );

extern void CancelMapUIMessage( void );

extern BOOLEAN fMapScreenBottomDirty;

// callbacks
void BtnTownCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMineCallback(GUI_BUTTON *btn,INT32 reason);
void BtnItemCallback(GUI_BUTTON *btn,INT32 reason);
void BtnAircraftCallback(GUI_BUTTON *btn,INT32 reason);
void BtnTeamCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMilitiaCallback(GUI_BUTTON *btn,INT32 reason);
// HEADROCK HAM 4: Mobile Restrictions Button callback
void BtnMobileCallback(GUI_BUTTON *btn,INT32 reason);

// Flugente: disease
void BtnDiseaseCallback(GUI_BUTTON *btn,INT32 reason);

void LevelMarkerBtnCallback(MOUSE_REGION * pRegion, INT32 iReason );

void CommonBtnCallbackBtnDownChecks( void );


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

	return ( TRUE );
}


void DeleteMapBorderGraphics( void )
{
	// procedure will delete graphics loaded for map border

	DeleteVideoObjectFromIndex( guiMapBorder );
}


void RenderMapBorder( void )
{
	// renders the actual border to the guiSAVEBUFFER
	HVOBJECT hHandle;

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
	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
	{
		BltVideoObject( guiSAVEBUFFER , hHandle, 1, xResOffset + MAP_BORDER_X, yResOffset + MAP_BORDER_Y, VO_BLT_SRCTRANSPARENCY,NULL );
	}
	else
	{
		BltVideoObject( guiSAVEBUFFER , hHandle, 0, xResOffset + MAP_BORDER_X, yResOffset + MAP_BORDER_Y, VO_BLT_SRCTRANSPARENCY,NULL );
	}

	RestoreExternBackgroundRect( xResOffset + MAP_BORDER_X, yResOffset + MAP_BORDER_Y, SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset, SCREEN_HEIGHT - 121 - 2 * yResOffset);

	// show the level marker
	DisplayCurrentLevelMarker( );
}


void RenderMapBorderEtaPopUp( void )
{
	// renders map border corner to the FRAME_BUFFER
	HVOBJECT hHandle;

	if( fShowMapInventoryPool )
	{
		return;
	}

	if( fPlotForHelicopter )
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
}

BOOLEAN CreateButtonsForMapBorder( void )
{
	// will create the buttons needed for the map screen border region

	// HEADROCK HAM 4: Call to initiate coordinates for all buttons.
	InitMapBorderButtonCoordinates();

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
	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
	{
		giMapBorderButtonsImage[ MAP_BORDER_MOBILE_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,20,-1,21,-1 );
		giMapBorderButtons[ MAP_BORDER_MOBILE_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_MOBILE_BTN ], MAP_BORDER_MOBILE_BTN_X, MAP_BORDER_MOBILE_BTN_Y,
											BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
											(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnMobileCallback);
	}
			
	// set up fast help text
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_TOWN_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_MINE_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_MINE_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_TEAMS_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_AIRSPACE_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_ITEM_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_ITEM_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_MILITIA_BTN ] );
		
	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
		SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_MOBILE_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_MOBILE_BTN ] ); // HEADROCK HAM 4: Mobile Militia button
	
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_TOWN_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_MINE_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_ITEM_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ], MSYS_NO_CURSOR );

	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
		SetButtonCursor(giMapBorderButtons[ MAP_BORDER_MOBILE_BTN ], MSYS_NO_CURSOR ); // HEADROCK HAM 4: Mobile Militia button
	
	// Flugente: disease
	if ( gGameExternalOptions.fDisease && gGameExternalOptions.fDiseaseStrategic )
	{
		giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 31, -1, 32, -1 );

		// if the button image cannot be created, this is likely to a custom sti replacement that isn't up to date to this feature - just use some other images then
		if ( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] < 0 )
			giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 2, -1, 11, -1 );

		giMapBorderButtons[MAP_BORDER_DISEASE_BTN] = QuickCreateButton( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN], MAP_BORDER_DISEASE_BTN_X, MAP_BORDER_DISEASE_BTN_Y,
																		BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
																		(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnDiseaseCallback );

		SetButtonFastHelpText( giMapBorderButtons[MAP_BORDER_DISEASE_BTN], pMapScreenBorderButtonHelpText[MAP_BORDER_DISEASE_BTN] );

		SetButtonCursor( giMapBorderButtons[MAP_BORDER_DISEASE_BTN], MSYS_NO_CURSOR );
	}

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
	   DisableButton( giMapBorderButtons[ MAP_BORDER_DISEASE_BTN ]); 
	   	   	   
	   fShowTownFlag = FALSE;
#endif

	InitializeMapBorderButtonStates( );

	return( TRUE );
}


void DeleteMapBorderButtons( void )
{
	UINT8 ubCnt;

	RemoveButton( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ]);
	RemoveButton( giMapBorderButtons[ MAP_BORDER_MINE_BTN ]);
	RemoveButton( giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ]);
	RemoveButton( giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ]);
	RemoveButton( giMapBorderButtons[ MAP_BORDER_ITEM_BTN ]);
	RemoveButton( giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ]);

	// WANNE: Only remove if we added the button
	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
		RemoveButton( giMapBorderButtons[ MAP_BORDER_MOBILE_BTN ]); // HEADROCK HAM 4

	if ( giMapBorderButtons[MAP_BORDER_DISEASE_BTN] != -1 )
		RemoveButton( giMapBorderButtons[MAP_BORDER_DISEASE_BTN] );

	// images

	UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_TOWN_BTN ] );
	UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_MINE_BTN ] );
	UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_TEAMS_BTN ] );
	UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_AIRSPACE_BTN ] );
	UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_ITEM_BTN ] );
	UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_MILITIA_BTN ] );

	// WANNE: Only unload if we added the button
	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
		UnloadButtonImage( giMapBorderButtonsImage[ MAP_BORDER_MOBILE_BTN ] ); // HEADROCK HAM 4

	if ( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] != -1 )
		UnloadButtonImage( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] );
		
	// HEADROCK HAM 4: Increased number of buttons by one.
	for ( ubCnt = 0; ubCnt < NUM_MAP_BORDER_BTNS; ++ubCnt )
	{
		giMapBorderButtons[ ubCnt ] = -1;
		giMapBorderButtonsImage[ ubCnt ] = -1;
	}
}


// callbacks
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

// Flugente: disease
void BtnDiseaseCallback( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();

		ToggleDiseaseFilter();
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
		
		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}
	}

	fMapPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
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
		
		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}
	}

	fMapPanelDirty = TRUE;
	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
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
	fTeamPanelDirty = TRUE;
	fCharacterInfoPanelDirty = TRUE;
	fMapScreenBottomDirty = TRUE;
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
		fMapScreenBottomDirty = TRUE;
	}
	else
	{	// turn show teams ON
		TurnOnShowTeamsMode();
	}
}


void ToggleAirspaceMode( void )
{
	if ( gusMapDisplayColourMode == MAP_DISPLAY_AIRSPACE )
	{
		// turn airspace OFF
		gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;

		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );

		if( fPlotForHelicopter )
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
		fMapScreenBottomDirty = TRUE;
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
		fMapScreenBottomDirty = TRUE;
	}
	else
	{
		// turn items ON
		TurnOnItemFilterMode();
	}
}

// Flugente: disease
void ToggleDiseaseFilter( void )
{
	if ( gusMapDisplayColourMode == MAP_DISPLAY_DISEASE )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;

		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
		// turn ON
		TurnOnDiseaseFilterMode( );
	}
}

// HEADROCK HAM 4: Toggle Mobile Restrictions Button
void ToggleMobileFilter( void )
{
	if ( gusMapDisplayColourMode == MAP_DISPLAY_MOBILEMILITIARESTRICTIONS )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;

		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
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


void DisplayCurrentLevelMarker( void )
{
	// display the current level marker on the map border
	HVOBJECT hHandle;


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
		
		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
}



void TurnOnAirSpaceMode( void )
{
	// if mode already on, leave, else set and redraw

	if ( gusMapDisplayColourMode != MAP_DISPLAY_AIRSPACE )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_AIRSPACE;

		MapBorderButtonOn( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );

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
		fMapScreenBottomDirty = TRUE;
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
		
		if ( (bSelectedDestChar != -1) || fPlotForHelicopter || fPlotForMilitia )
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
		fMapScreenBottomDirty = TRUE;
	}
}

void TurnOnDiseaseFilterMode( void )
{
	// if mode already on, leave, else set and redraw
	if ( gusMapDisplayColourMode != MAP_DISPLAY_DISEASE )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_DISEASE;

		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		MapBorderButtonOn( MAP_BORDER_DISEASE_BTN );

		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}
		
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
		
		if ( (bSelectedDestChar != -1) || fPlotForHelicopter || fPlotForMilitia )
		{
			AbortMovementPlottingMode( );
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode( );
		}

		if ( !gubFact[FACT_DISEASE_VIEWED] )
		{
			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, zMarksMapScreenText[27] );

			SetFactTrue( FACT_DISEASE_VIEWED );
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

	if ( gusMapDisplayColourMode != MAP_DISPLAY_MOBILEMILITIARESTRICTIONS )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_MOBILEMILITIARESTRICTIONS;

		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOn( MAP_BORDER_MOBILE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );

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
		
		// Turn off items
		if( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}
		
		if ( (bSelectedDestChar != -1) || fPlotForHelicopter || fPlotForMilitia )
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
		fMapScreenBottomDirty = TRUE;
	}
}


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
	
	if( fShowMilitia )
	{
		MapBorderButtonOn( MAP_BORDER_MILITIA_BTN );
	}
	else
	{
		MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
	}

	switch ( gusMapDisplayColourMode )
	{
	case MAP_DISPLAY_AIRSPACE:
		MapBorderButtonOn( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		break;

	case MAP_DISPLAY_MOBILEMILITIARESTRICTIONS:
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOn( MAP_BORDER_MOBILE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		break;

	case MAP_DISPLAY_DISEASE:
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_MOBILE_BTN );
		MapBorderButtonOn( MAP_BORDER_DISEASE_BTN );
		break;
	}
}



BOOLEAN DoesPlayerHaveAnyMilitia( void )
{
	// run through list of towns that might have militia..if any return TRUE..else return FALSE
	for ( INT16 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( INT16 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			if ( NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM ) > 0 )
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
	if ( !gGameExternalOptions.gfAllowMilitiaGroups || gGameExternalOptions.fMilitiaStrategicCommand )
	{
		// Mobile Militia not allowed at all.
		return (0);
	}

	// run through list of towns that might have militia..if any return TRUE..else return FALSE
	for ( INT16 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( INT16 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			// Look only in sectors where Militia Training is not allowed at all. If any militia are found there,
			// it means that they had to MOVE there, hence mobile militia.
			if (!MilitiaTrainingAllowedInSector( sX, sY, 0 ))
			{
				if ( NumNonPlayerTeamMembersInSector( sX, sY, MILITIA_TEAM ) > 0 )
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

	fShowItemsFlag = FALSE;

	gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;
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
	MAP_BORDER_TOWN_BTN_Y 		= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_BORDER_MINE_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 109;
	MAP_BORDER_MINE_BTN_Y 		= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_BORDER_TEAMS_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 66;
	MAP_BORDER_TEAMS_BTN_Y 		= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_BORDER_AIRSPACE_BTN_X	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 20;
	MAP_BORDER_AIRSPACE_BTN_Y	= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_BORDER_ITEM_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 63;
	MAP_BORDER_ITEM_BTN_Y 		= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_BORDER_MILITIA_BTN_X 	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) - 23;
	MAP_BORDER_MILITIA_BTN_Y 	= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_BORDER_MOBILE_BTN_X 	= xResSize;
	MAP_BORDER_MOBILE_BTN_Y 	= 0;
	MAP_BORDER_DISEASE_BTN_X	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 190;
	MAP_BORDER_DISEASE_BTN_Y	= (SCREEN_HEIGHT - yResOffset - buttonOffset);

	MAP_LEVEL_MARKER_X 			= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 114;
	MAP_LEVEL_MARKER_Y 			= (SCREEN_HEIGHT - yResOffset - buttonOffset);
	MAP_LEVEL_MARKER_DELTA 		= 8;
	MAP_LEVEL_MARKER_WIDTH 		= 55;

	if ( gGameExternalOptions.gfAllowMilitiaGroups && !gGameExternalOptions.fMilitiaStrategicCommand )
	{
		// Mobile button appears next to Militia button.
		MAP_BORDER_MOBILE_BTN_X 	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 16;
		MAP_BORDER_MOBILE_BTN_Y 	= (SCREEN_HEIGHT - yResOffset - buttonOffset);

		// Airspace, Items, ZLevel buttons all moved to the right (+22px, +22px, +10px).
		MAP_BORDER_AIRSPACE_BTN_X 	= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 42;
		MAP_BORDER_ITEM_BTN_X 		= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 85;

		MAP_LEVEL_MARKER_X 			= xResOffset + MAP_BORDER_X + ((SCREEN_WIDTH - MAP_BORDER_X - 2 * xResOffset) / 2) + 124;
	}
}
