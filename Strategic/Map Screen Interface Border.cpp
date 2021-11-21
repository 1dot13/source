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

extern UINT16 UI_BOTTOM_X;
extern UINT16 UI_BOTTOM_Y;
extern UILayout_Map UI_MAP;

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
INT32 giMapBorderButtons[ NUM_MAP_BORDER_BTNS ] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
INT32 giMapBorderButtonsImage[ NUM_MAP_BORDER_BTNS ] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

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

// Flugente: more callbacks
void BtnDiseaseCallback(GUI_BUTTON *btn,INT32 reason);
void BtnWeatherCallback( GUI_BUTTON *btn, INT32 reason );
void BtnIntelCallback( GUI_BUTTON *btn, INT32 reason );

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
	else if (iResolution == _1280x720)
	{
		FilenameForBPP("INTERFACE\\MBS_1280x720.sti", VObjectDesc.ImageFile);
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
	
	BltVideoObject( guiSAVEBUFFER, hHandle, 0, UI_MAP.BorderRegion.x, UI_MAP.BorderRegion.y, VO_BLT_SRCTRANSPARENCY, NULL );

	RestoreExternBackgroundRect(UI_MAP.BorderRegion.x, UI_MAP.BorderRegion.y, SCREEN_WIDTH, SCREEN_HEIGHT - 121);

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
	UINT16 xVal = (UI_MAP.ETA.Clock_X - 10) ;
	UINT16 yVal = (UI_MAP.ETA.Start_Y - 7);

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
	giMapBorderButtons[ MAP_BORDER_TOWN_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_TOWN_BTN ], UI_MAP.Button.Town.iX, UI_MAP.Button.Town.iY,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnTownCallback);
	
	// mines
	giMapBorderButtonsImage[ MAP_BORDER_MINE_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,4,-1,13,-1 );
	giMapBorderButtons[ MAP_BORDER_MINE_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_MINE_BTN ], UI_MAP.Button.Mine.iX, UI_MAP.Button.Mine.iY,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnMineCallback);

	// people
	giMapBorderButtonsImage[ MAP_BORDER_TEAMS_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,3,-1,12,-1 );
	giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_TEAMS_BTN ], UI_MAP.Button.Teams.iX, UI_MAP.Button.Teams.iY,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnTeamCallback);

	// militia
	giMapBorderButtonsImage[ MAP_BORDER_MILITIA_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,8,-1,17,-1 );
	giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_MILITIA_BTN ], UI_MAP.Button.Militia.iX, UI_MAP.Button.Militia.iY,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnMilitiaCallback);

	// airspace
	giMapBorderButtonsImage[ MAP_BORDER_AIRSPACE_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,2,-1,11,-1 );
	giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_AIRSPACE_BTN ], UI_MAP.Button.Airspace.iX, UI_MAP.Button.Airspace.iY,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnAircraftCallback);

	// items
	giMapBorderButtonsImage[ MAP_BORDER_ITEM_BTN ] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti" ,-1,1,-1,10,-1 );
	giMapBorderButtons[ MAP_BORDER_ITEM_BTN ] = QuickCreateButton( giMapBorderButtonsImage[ MAP_BORDER_ITEM_BTN ], UI_MAP.Button.Inventory.iX, UI_MAP.Button.Inventory.iY,
										BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
										(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnItemCallback);
				
	// set up fast help text
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_TOWN_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_TOWN_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_MINE_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_MINE_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_TEAMS_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_AIRSPACE_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_ITEM_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_ITEM_BTN ] );
	SetButtonFastHelpText( giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ], pMapScreenBorderButtonHelpText[ MAP_BORDER_MILITIA_BTN ] );
	
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_TOWN_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_MINE_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_TEAMS_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_AIRSPACE_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_ITEM_BTN ], MSYS_NO_CURSOR );
	SetButtonCursor(giMapBorderButtons[ MAP_BORDER_MILITIA_BTN ], MSYS_NO_CURSOR );

	UINT16 nextButtonX = UI_MAP.Button.Disease.iX - 43;
		
	// Flugente: disease
	if ( gGameExternalOptions.fDisease && gGameExternalOptions.fDiseaseStrategic )
	{
		nextButtonX += 43;

		giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 31, -1, 32, -1 );

		// if the button image cannot be created, this is likely to a custom sti replacement that isn't up to date to this feature - just use some other images then
		if ( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] < 0 )
			giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 2, -1, 11, -1 );

		giMapBorderButtons[MAP_BORDER_DISEASE_BTN] = QuickCreateButton( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN], nextButtonX, UI_MAP.Button.Disease.iY,
																		BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
																		(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnDiseaseCallback );

		SetButtonFastHelpText( giMapBorderButtons[MAP_BORDER_DISEASE_BTN], pMapScreenBorderButtonHelpText[MAP_BORDER_DISEASE_BTN] );

		SetButtonCursor( giMapBorderButtons[MAP_BORDER_DISEASE_BTN], MSYS_NO_CURSOR );
	}

	// Flugente: weather
	if ( gGameExternalOptions.gfAllowRain || gGameExternalOptions.gfAllowSandStorms || gGameExternalOptions.gfAllowSnow )
	{
		nextButtonX += 43;

		giMapBorderButtonsImage[MAP_BORDER_WEATHER_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 37, -1, 38, -1 );

		// if the button image cannot be created, this is likely to a custom sti replacement that isn't up to date to this feature - just use some other images then
		if ( giMapBorderButtonsImage[MAP_BORDER_WEATHER_BTN] < 0 )
			giMapBorderButtonsImage[MAP_BORDER_WEATHER_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 2, -1, 11, -1 );

		/*
		INT16 x = MAP_BORDER_WEATHER_BTN_X;
		if ( giMapBorderButtonsImage[MAP_BORDER_DISEASE_BTN] < 0 )
			x = MAP_BORDER_DISEASE_BTN_X;
		*/

		giMapBorderButtons[MAP_BORDER_WEATHER_BTN] = QuickCreateButton( giMapBorderButtonsImage[MAP_BORDER_WEATHER_BTN], nextButtonX, UI_MAP.Button.Weather_Y,
																		BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
																		(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnWeatherCallback );

		SetButtonFastHelpText( giMapBorderButtons[MAP_BORDER_WEATHER_BTN], pMapScreenBorderButtonHelpText[MAP_BORDER_WEATHER_BTN] );

		SetButtonCursor( giMapBorderButtons[MAP_BORDER_WEATHER_BTN], MSYS_NO_CURSOR );
	}

	// Flugente: intel
	if ( TRUE )
	{
		nextButtonX += 43;

		giMapBorderButtonsImage[MAP_BORDER_INTEL_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 39, -1, 40, -1 );

		// if the button image cannot be created, this is likely to a custom sti replacement that isn't up to date to this feature - just use some other images then
		if ( giMapBorderButtonsImage[MAP_BORDER_INTEL_BTN] < 0 )
			giMapBorderButtonsImage[MAP_BORDER_INTEL_BTN] = LoadButtonImage( "INTERFACE\\map_border_buttons.sti", -1, 2, -1, 11, -1 );

		/*
		INT16 x = MAP_BORDER_INTEL_BTN_X;
		if ( giMapBorderButtonsImage[MAP_BORDER_WEATHER_BTN] < 0 )
			x = MAP_BORDER_WEATHER_BTN_X;
		*/

		giMapBorderButtons[MAP_BORDER_INTEL_BTN] = QuickCreateButton( giMapBorderButtonsImage[MAP_BORDER_INTEL_BTN], nextButtonX, UI_MAP.Button.Intel_Y,
																		BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH,
																		(GUI_CALLBACK)MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnIntelCallback );

		SetButtonFastHelpText( giMapBorderButtons[MAP_BORDER_INTEL_BTN], pMapScreenBorderButtonHelpText[MAP_BORDER_INTEL_BTN] );

		SetButtonCursor( giMapBorderButtons[MAP_BORDER_INTEL_BTN], MSYS_NO_CURSOR );
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
	DisableButton( giMapBorderButtons[ MAP_BORDER_DISEASE_BTN ]); 
	DisableButton( giMapBorderButtons[ MAP_BORDER_WEATHER_BTN ]);
	DisableButton( giMapBorderButtons[ MAP_BORDER_INTEL_BTN ] );
	
	fShowTownFlag = FALSE;
#endif

	InitializeMapBorderButtonStates( );

	return( TRUE );
}

void DeleteMapBorderButtons( void )
{
	for ( int i = 0; i < NUM_MAP_BORDER_BTNS; ++i )
	{
		if ( giMapBorderButtons[i] != -1 )
			RemoveButton( giMapBorderButtons[i] );

		giMapBorderButtons[i] = -1;

		if ( giMapBorderButtonsImage[i] != -1 )
			UnloadButtonImage( giMapBorderButtonsImage[i] );

		giMapBorderButtonsImage[i] = -1;
	}
}

void DisableMapBorderButtons(void)
{
	for (int i = 0; i < NUM_MAP_BORDER_BTNS; ++i)
	{
		if (giMapBorderButtons[i] != -1)
			DisableButton( giMapBorderButtons[i] );
	}
}

void EnableMapBorderButtons(void)
{
	for (int i = 0; i < NUM_MAP_BORDER_BTNS; ++i)
	{
		if (giMapBorderButtons[i] != -1)
			EnableButton(giMapBorderButtons[i]);
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

// Flugente: weather
void BtnWeatherCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks( );

		ToggleWeatherFilter( );
	}
	else if ( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks( );
	}
}

// Flugente: intel
void BtnIntelCallback( GUI_BUTTON *btn, INT32 reason )
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		CommonBtnCallbackBtnDownChecks();

		ToggleIntelFilter();
	}
	else if ( reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
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
			if( IsAnyOneOnPlayersTeamOnThisAssignment( TRAIN_TOWN ) )
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
	if ( gusMapDisplayColourMode == MAP_DISPLAY_AIRSPACE_COLOURED_SAMS )
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

// Flugente: weather
void ToggleWeatherFilter( void )
{
	if ( gusMapDisplayColourMode == MAP_DISPLAY_WEATHER )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;

		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
		// turn ON
		TurnOnWeatherFilterMode( );
	}
}

// Flugente: intel
void ToggleIntelFilter()
{
	if ( gusMapDisplayColourMode == MAP_DISPLAY_INTEL )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_NORMAL;

		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
	else
	{
		// turn ON
		TurnOnIntelFilterMode();
	}
}

void DisplayCurrentLevelMarker( void )
{
	// display the current level marker on the map border
	HVOBJECT hHandle;


	// it's actually a white rectangle, not a green arrow!
	GetVideoObject(&hHandle, guiLEVELMARKER );
	BltVideoObject( guiSAVEBUFFER , hHandle, 0, UI_MAP.LevelMarkerArea.x + 1, UI_MAP.LevelMarkerArea.y + (UI_MAP.LevelMarkerArea.height * ( INT16 )iCurrentMapSectorZ ), VO_BLT_SRCTRANSPARENCY,NULL );

	RestoreExternBackgroundRect(UI_MAP.LevelMarkerArea.x + 1, UI_MAP.LevelMarkerArea.y + (UI_MAP.LevelMarkerArea.height * ( INT16 )iCurrentMapSectorZ ), 55, 9);
}

void CreateMouseRegionsForLevelMarkers( void )
{
	INT16 sCounter = 0;
	CHAR16 sString[ 64 ];

	for( sCounter = 0; sCounter	< 4 ; ++sCounter )
	{
		const auto tlx = UI_MAP.LevelMarkerArea.x;
		const auto tly = UI_MAP.LevelMarkerArea.y + UI_MAP.LevelMarkerArea.height * sCounter;
		const auto brx = tlx + UI_MAP.LevelMarkerArea.width;
		const auto bry = tly + UI_MAP.LevelMarkerArea.height;
		MSYS_DefineRegion(&LevelMouseRegions[ sCounter ], tlx, tly, brx, bry, MSYS_PRIORITY_HIGH, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, LevelMarkerBtnCallback );

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

	if ( gusMapDisplayColourMode != MAP_DISPLAY_AIRSPACE && gusMapDisplayColourMode != MAP_DISPLAY_AIRSPACE_COLOURED_SAMS )
		gusMapDisplayColourMode = MAP_DISPLAY_AIRSPACE;
	else if ( gusMapDisplayColourMode == MAP_DISPLAY_AIRSPACE )
		gusMapDisplayColourMode = MAP_DISPLAY_AIRSPACE_COLOURED_SAMS;

	if ( gusMapDisplayColourMode == MAP_DISPLAY_AIRSPACE || gusMapDisplayColourMode == MAP_DISPLAY_AIRSPACE_COLOURED_SAMS )
	{
		MapBorderButtonOn( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );

		// Turn off towns & mines (mostly because town/mine names overlap SAM site names)
		if( fShowTownFlag )
		{
			fShowTownFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
		}

		if( fShowMineFlag )
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
		if( fShowItemsFlag )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		if ( GetSelectedDestChar() != -1 )
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
		
		if ( ( GetSelectedDestChar() != -1) || fPlotForHelicopter || fPlotForMilitia )
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
		MapBorderButtonOn( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );

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
		
		if ( ( GetSelectedDestChar() != -1) || fPlotForHelicopter || fPlotForMilitia )
		{
			AbortMovementPlottingMode( );
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode( );
		}

		if ( !gubFact[FACT_DISEASE_VIEWED] )
		{
			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, zMarksMapScreenText[25] );

			SetFactTrue( FACT_DISEASE_VIEWED );
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
}

void TurnOnWeatherFilterMode()
{
	// if mode already on, leave, else set and redraw
	if ( gusMapDisplayColourMode != MAP_DISPLAY_WEATHER )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_WEATHER;

		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOn( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );

		if ( fShowItemsFlag == TRUE )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		// Turn off towns, mines, teams, militia & airspace if any are on
		if ( fShowTownFlag == TRUE )
		{
			fShowTownFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
		}

		if ( fShowMineFlag == TRUE )
		{
			fShowMineFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MINE_BTN );
		}

		if ( fShowTeamFlag == TRUE )
		{
			fShowTeamFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
		}

		if ( fShowMilitia == TRUE )
		{
			fShowMilitia = FALSE;
			MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
		}

		if ( ( GetSelectedDestChar() != -1) || fPlotForHelicopter || fPlotForMilitia )
		{
			AbortMovementPlottingMode( );
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode( );
		}

		// Flugente: basically only show this message the first time every session
		static BOOLEAN sWeatherviewed = FALSE;
		if ( !sWeatherviewed )
		{
			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, zMarksMapScreenText[26] );

			sWeatherviewed = TRUE;
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
	}
}

void TurnOnIntelFilterMode()
{
	// if mode already on, leave, else set and redraw
	if ( gusMapDisplayColourMode != MAP_DISPLAY_INTEL )
	{
		gusMapDisplayColourMode = MAP_DISPLAY_INTEL;

		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOn( MAP_BORDER_INTEL_BTN );

		if ( fShowItemsFlag )
		{
			fShowItemsFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_ITEM_BTN );
		}

		// Turn off towns, mines, teams, militia & airspace if any are on
		if ( fShowTownFlag )
		{
			fShowTownFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TOWN_BTN );
		}

		if ( fShowMineFlag )
		{
			fShowMineFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_MINE_BTN );
		}

		if ( fShowTeamFlag )
		{
			fShowTeamFlag = FALSE;
			MapBorderButtonOff( MAP_BORDER_TEAMS_BTN );
		}

		if ( fShowMilitia )
		{
			fShowMilitia = FALSE;
			MapBorderButtonOff( MAP_BORDER_MILITIA_BTN );
		}

		if ( ( GetSelectedDestChar() != -1 ) || fPlotForHelicopter || fPlotForMilitia )
		{
			AbortMovementPlottingMode();
		}
		else if ( gfInChangeArrivalSectorMode )
		{
			CancelChangeArrivalSectorMode();
		}

		// Flugente: basically only show this message the first time every session
		static BOOLEAN sIntelViewed = FALSE;
		if ( !sIntelViewed )
		{
			MapScreenMessage( FONT_MCOLOR_LTYELLOW, MSG_MAP_UI_POSITION_MIDDLE, zMarksMapScreenText[27] );

			sIntelViewed = TRUE;
		}

		// dirty regions
		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
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
	case MAP_DISPLAY_AIRSPACE_COLOURED_SAMS:
		MapBorderButtonOn( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );
		break;
		
	case MAP_DISPLAY_DISEASE:
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOn( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );
		break;

	case MAP_DISPLAY_WEATHER:
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOn( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOff( MAP_BORDER_INTEL_BTN );
		break;

	case MAP_DISPLAY_INTEL:
		MapBorderButtonOff( MAP_BORDER_AIRSPACE_BTN );
		MapBorderButtonOff( MAP_BORDER_DISEASE_BTN );
		MapBorderButtonOff( MAP_BORDER_WEATHER_BTN );
		MapBorderButtonOn( MAP_BORDER_INTEL_BTN );
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
	if (isWidescreenUI())
	{
		// Map border buttons are located in the map bottom ui bar for widescreen UI
		const UINT16 firstColumn = UI_BOTTOM_X + 746;
		const UINT16 secondColumn = UI_BOTTOM_X + 788;
		const UINT16 thirdColumn = UI_BOTTOM_X + 831;
		const UINT16 fourthColumn = UI_BOTTOM_X + 874;
		const UINT16 fifthColumn = UI_BOTTOM_X + 917;
		const UINT16 sixthColumn = UI_BOTTOM_X + 960;
		const UINT16 firstRow = UI_BOTTOM_Y + 1; //SCREEN_HEIGHT - 115;
		const UINT16 SecondRow = UI_BOTTOM_Y + 38; //SCREEN_HEIGHT - 78;
		
		
		UI_MAP.Button.Town = { firstColumn, firstRow };
		UI_MAP.Button.Mine = { secondColumn, firstRow };
		UI_MAP.Button.Teams = { fifthColumn, firstRow };
		UI_MAP.Button.Militia = { sixthColumn, firstRow };
		UI_MAP.Button.Airspace = { firstColumn, SecondRow};
		UI_MAP.Button.Inventory = { secondColumn, SecondRow };
		UI_MAP.Button.Disease = { thirdColumn, SecondRow };
		UI_MAP.Button.Weather_Y = SecondRow;
		UI_MAP.Button.Intel_Y = SecondRow;
		UI_MAP.LevelMarkerArea = { UI_BOTTOM_X + 840, UI_BOTTOM_Y, 55, 8 };
	}
	else if (iResolution < _800x600)
	{
		const UINT16 xOffset = 43;
		UI_MAP.Button.Town = { UI_MAP.BorderRegion.x + 35, UI_MAP.BorderRegion.y + 325 };
		UI_MAP.Button.Mine = { UI_MAP.Button.Town.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Teams = { UI_MAP.Button.Mine.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Militia = { UI_MAP.Button.Teams.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Airspace = { UI_MAP.Button.Militia.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Inventory = { UI_MAP.Button.Airspace.iX + xOffset, UI_MAP.Button.Town.iY };

		UI_MAP.Button.Disease = { UI_MAP.BorderRegion.x + 361, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Weather_Y = UI_MAP.Button.Town.iY;
		UI_MAP.Button.Intel_Y = UI_MAP.Button.Town.iY;
		UI_MAP.LevelMarkerArea = { UI_MAP.BorderRegion.x + 303, UI_MAP.Button.Town.iY, 55, 8 };

	}
	else if (iResolution < _1024x768)
	{
		const UINT16 xOffset = 43;
		UI_MAP.Button.Town = { UI_MAP.BorderRegion.x + 115, UI_MAP.BorderRegion.y + 444 };
		UI_MAP.Button.Mine = { UI_MAP.Button.Town.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Teams = { UI_MAP.Button.Mine.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Militia = { UI_MAP.Button.Teams.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Airspace = { UI_MAP.Button.Militia.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Inventory = { UI_MAP.Button.Airspace.iX + xOffset, UI_MAP.Button.Town.iY };

		UI_MAP.Button.Disease = { UI_MAP.BorderRegion.x + 450, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Weather_Y = UI_MAP.Button.Town.iY; 
		UI_MAP.Button.Intel_Y = UI_MAP.Button.Town.iY;
		UI_MAP.LevelMarkerArea = { UI_MAP.BorderRegion.x + 383, UI_MAP.Button.Town.iY, 55, 8 };
	}
	else
	{
		const UINT16 xOffset = 43;
		UI_MAP.Button.Town = { UI_MAP.BorderRegion.x + 228, UI_MAP.BorderRegion.y + 612 };
		UI_MAP.Button.Mine = { UI_MAP.Button.Town.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Teams = { UI_MAP.Button.Mine.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Militia = { UI_MAP.Button.Teams.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Airspace = { UI_MAP.Button.Militia.iX + xOffset, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Inventory = { UI_MAP.Button.Airspace.iX + xOffset, UI_MAP.Button.Town.iY };

		UI_MAP.Button.Disease = { UI_MAP.BorderRegion.x + 560, UI_MAP.Button.Town.iY };
		UI_MAP.Button.Weather_Y = UI_MAP.Button.Town.iY;
		UI_MAP.Button.Intel_Y = UI_MAP.Button.Town.iY;
		UI_MAP.LevelMarkerArea = { UI_MAP.BorderRegion.x + 495, UI_MAP.Button.Town.iY, 55, 8 };
	}
}
