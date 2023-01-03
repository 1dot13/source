/*TODOs:
*remove every unneeded data (temp arrays, save arrays, backup arrays, data arrays, ec.)
*Items:		- needs three state ini option to either be saved into file (any new item in inventory or sector stash will be added) 
*				UINT8 array [MAX_ITEMS]: 0 not yet discovered (invisible), 1 in sector stash (unreachable) or on enemy, 2 seen at bobby ray, 3 seen in dealer inventory, 4 in sector stash (reachable) or solder inventory
*			- or to gather data from all sector inventories (mark all as reachable, as there is no way to figure out in unloaded sectors), bobby rays current inventory, known dealers, soldier inventory
*			- or to reveal all items from game (thats the old encyclopedia behavier)
*			- make sure that this data gets updated only when accessing the laptop encyclopedia to avoid any impact on gameplay performance memory/speed (not possible for option 1!!)
*Profiles:	- gather data from existing structs
*Quests:	- gather data from existing structs
*Locations	- gather data from exisitng structs
*
*only current DATA object is stored (graphic, names, description text, type ec..) and replaced when [next] or [previous] button was pressed. Stored in order to rerender dirty area if needed.
* DONE: filters are handled on button press (gathering data in sequence and skip filtered items)
*if possible do not read/write from files to get some data
* DONE: reuse all graphics and layout from old Enc.
*
* PARTIALLY DONE: on button press: check if next/previous item exists to hide or disable (TBD) buttons and play bad beep sound (applys to clicks on filter buttons as well!)
*
*minimize usage of functions: default laptop www site has following functions:
*	void GameInitXXX()											inits coords, validates data ec. everything that does not change (called when starting JA2/loading game/starting new game).
*	void EnterXXX()												creates video objects, buttons, fasthelp texts, mousregions, ec. DOES NOT CALL RENDER!!
*	void ExitXXX()												clean up everything, memory usage before enter and after exit should be equal
*	void HandleXXX()											helper function gets called AFTER rendering by game loop. Can be used for animations, blinking and stuff (showing position of person on a map by blinking, simulate loading of site, animated zoom ec.)
*	void RenderXXX()											obviously renders the site
*	void ChangingXXXSubPage(UINT8)								prepares a change to a different subpage (called by BtnCallback)
*	void BtnXXXButtonCallback(GUI_BUTTON *btn,INT32 reason)		callback for buttons / regions. If possible reuse these by adding some local swiches for readability of code (f.i.one for filters, one for next/previous).
*/

/// uncomment to use just graphic + mouseregion instead of real buttons. No sounds, no button states, just plain 'hyperlinks'.
#define ENC_USE_BUTTONSYSTEM

#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "Types.h"
	#include "WCheck.h"
	#include "DEBUG.H"
	#include "GameSettings.h"
	#include "laptop.h"//UI dimensions, mouse regions
	#include "Utilities.h"//file names
	#include "vobject.h"//video objects
	#include "Utils/Cursors.h"
	#include "Text.h"//button text
#ifdef ENC_USE_BUTTONSYSTEM
	#include "Button System.h"
#else
	#include "WordWrap.h"//centered text
#endif
	#include "Encyclopedia_new.h"
	//#include "Encrypted File.h"
	//#include "Soldier Profile.h"
	//#include "Sound Control.h"
	//#include "Campaign Types.h"
	//#include "Quests.h"
	//#include "Tactical Save.h"
	#include "Encyclopedia_Data_new.h"
#endif	

#ifdef ENCYCLOPEDIA_WORKS
/** @defgroup ENCYCLOPEDIA Encyclopedia
* Encyclopedia shows discovered places, characters, known items and quests.
*/

/** @ingroup ENCYCLOPEDIA
* @file
* Main page of Encyclopedia.
*/

///@{ background images
UINT32 guiEncyclopediaBG;
UINT32 guiEncyclopediaAimLogo;
///@}

///@{ buttons, graphics and regions for main page
#ifdef ENC_USE_BUTTONSYSTEM
INT32 giEncyclopediaBtn[ ENC_NUM_SUBPAGES ];
INT32 giEncyclopediaBtnImage;
#else
MOUSE_REGION gEncyclopediaBtnRegions[ ENC_NUM_SUBPAGES ];
UINT32 guiEncyclopediaBtnImage;
#endif
#define ENC_BTN_GAP 6
#define ENC_AIMLOGO_GAP_TOP 20
#define ENC_AIMLOGO_GAP_BOTTOM 40
///@}

ENC_SUBPAGE_T geENC_SubPage;	///< Current sub page

///////
//prototypes

#ifdef ENC_USE_BUTTONSYSTEM
void BtnEncyclopedia_newSelectDataPageBtnCallBack ( GUI_BUTTON *btn, INT32 reason );
#else
void BtnEncyclopedia_newSelectDataPageRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason );
#endif
///////
//functions

/** @defgroup ENC_MAIN Main page
* Encyclopedia main page global functions.
* @ingroup ENCYCLOPEDIA
* @{*/
/**
* Checks for all required graphic files.
*
* Called when a new game is started (guiCurrentScreen == 0), entering new game screen/loading game from main menu (guiCurrentScreen == MAINMENU_SCREEN) and quitting game (guiCurrentScreen == MSG_BOX_SCREEN).
* Checks for all needed graphic files. Initializes button and image handles.
* Shows error message and deactivates Encyclopedia if a file is missing.
* @todo show a msg to user. MsgBox is crashing JA2 because of multithread. Maybe pause game?
*/
void GameInitEncyclopedia_NEW()
{
	// initialize gui handles
#ifdef ENC_USE_BUTTONSYSTEM
	memset( giEncyclopediaBtn, BUTTON_NO_SLOT, sizeof(giEncyclopediaBtn) );
	giEncyclopediaBtnImage = BUTTON_NO_IMAGE;
#else
#endif
	// check for files only on start of JA2
	CHECKV( guiCurrentScreen == 0 && gGameExternalOptions.gEncyclopedia );

	if ( !FileExists( "ENCYCLOPEDIA\\encyclopediabackground.sti" ) )
	{
		goto error;
	}
	if ( !FileExists( "ENCYCLOPEDIA\\encyclopedialogoaim.sti" ) )
	{
		goto error;
	}
	if ( !FileExists( "ENCYCLOPEDIA\\CONTENTBUTTON.STI" ) )
	{
		goto error;
	}
	return;
error:
	gGameExternalOptions.gEncyclopedia = FALSE;
	DebugMsg( TOPIC_JA2, DBG_LEVEL_1, "Ja2 Error: GRAPHIC FILE MISSING. Encyclopedia has been deactivated." );
	//MessageBox( NULL, "GRAPHIC FILE MISSING.Encyclopedia has been deactivated.", "Ja2 Error!", MB_OK );
	//SGP_THROW( "Ja2 Error: GRAPHIC FILE MISSING. Encyclopedia has been deactivated." );
}

/**
* Helper function gets called AFTER rendering by game loop. 
* Can be used to poll keyboard or mouse events.
* Can also be used for animations (e.g. showing blinking position of person on a map, simulate loading of commercials, animated zoom ec.).
*/
void HandleEncyclopedia_NEW()
{
}

/**
* Creates video objects, buttons, fasthelp texts, mousregions.
* Called when entering encyclopedia web page in laptop. Prepares main page of Encyclopedia.
* @return FALSE on error. TRUE on success.
* @see ExitEncyclopedia_NEW()
*/
BOOLEAN EnterEncyclopedia_NEW(  )
{
	VOBJECT_DESC	VObjectDesc;
	HVOBJECT hVObject;
	UINT16 buttonSizeX, buttonSizeY, logoBottomY;

	//error if not enabled
	CHECKF( gGameExternalOptions.gEncyclopedia );
	//error if the laptop mode does not match encyclopedia
	CHECKF( LAPTOP_MODE_ENCYCLOPEDIA == guiCurrentLaptopMode );

	SetBookMark( ENCYCLOPEDIA_BOOKMARK );//bookmark should be allways set, just in case it is not.
	//////
	// load the background graphic
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "ENCYCLOPEDIA\\encyclopediabackground.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiEncyclopediaBG ) );
	//////
	// load AIM Logo
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "ENCYCLOPEDIA\\encyclopedialogoaim.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiEncyclopediaAimLogo ) );
	// get bottom Y of logo from image
	GetVideoObject( &hVObject, guiEncyclopediaAimLogo );
	CHECKF(hVObject);CHECKF(hVObject->pETRLEObject);
	logoBottomY = hVObject->pETRLEObject->usHeight + LAPTOP_SCREEN_WEB_UL_Y + ENC_AIMLOGO_GAP_TOP;

#ifdef ENC_USE_BUTTONSYSTEM//use button system
	//////
	// load button graphic for the data pages
	giEncyclopediaBtnImage = LoadButtonImage( "ENCYCLOPEDIA\\CONTENTBUTTON.STI", BUTTON_NO_IMAGE, 0, BUTTON_NO_IMAGE , 0, BUTTON_NO_IMAGE );
	if ( giEncyclopediaBtnImage == BUTTON_NO_IMAGE )
		return FALSE;
	buttonSizeX = GetWidthOfButtonPic( giEncyclopediaBtnImage, 0 );
	buttonSizeY = GetHeightOfButtonPic( giEncyclopediaBtnImage, 0 );
	//////
	// create buttons and set user data
	for ( UINT8 i = 0; i < ENC_NUM_SUBPAGES; i++ )
	{
		giEncyclopediaBtn[ i ] = CreateIconAndTextButton( giEncyclopediaBtnImage,
			pMenuStrings[ i ], FONT12ARIAL, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, FONT_FCOLOR_WHITE, DEFAULT_SHADOW, TEXT_CJUSTIFIED,
			LAPTOP_SCREEN_UL_X + (LAPTOP_SCREEN_WIDTH)/2 - buttonSizeX/2,//upper left x: center of laptop screen - 1/2 buttonsize
			logoBottomY + ENC_AIMLOGO_GAP_BOTTOM + i * (ENC_BTN_GAP + buttonSizeY),//upper left y: below logo + logo gap + previous buttons and button gaps
			BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
			DEFAULT_MOVE_CALLBACK, BtnEncyclopedia_newSelectDataPageBtnCallBack );
		CHECKF( giEncyclopediaBtn[ i ] >= 0 );
		SetButtonCursor( giEncyclopediaBtn[ i ], CURSOR_LAPTOP_SCREEN );
		MSYS_SetBtnUserData( giEncyclopediaBtn[ i ], 1, i + 1 );
		GetButtonPtr( giEncyclopediaBtn[ i ] )->fShiftImage = TRUE;
		//SpecifyButtonSoundScheme( giEncyclopediaDataBtn[ i ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	}
#else
	//////
	// load button graphic for the data pages
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "ENCYCLOPEDIA\\CONTENTBUTTON.STI", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiEncyclopediaBtnImage ) );
	//////
	// create mouse regions for data buttons and set user data
	GetVideoObject( &hVObject, guiEncyclopediaBtnImage );
	CHECKF(hVObject);CHECKF(hVObject->pETRLEObject);
	buttonSizeX = hVObject->pETRLEObject->usWidth;//get width of buttons from image
	buttonSizeY = hVObject->pETRLEObject->usHeight;//get heigth of buttons from image

	for ( UINT8 i = 0; i < ENC_NUM_SUBPAGES; i++ )
	{
		MSYS_DefineRegion( &gEncyclopediaBtnRegions[i],
			LAPTOP_SCREEN_UL_X + (LAPTOP_SCREEN_WIDTH)/2 - buttonSizeX/2,//upper left x: center of laptop screen - 1/2 buttonsize
			logoBottomY + ENC_AIMLOGO_GAP_BOTTOM + i * (ENC_BTN_GAP + buttonSizeY),//upper left y: below logo + logo gap + previous buttons and button gaps
			LAPTOP_SCREEN_UL_X + (LAPTOP_SCREEN_WIDTH)/2 + buttonSizeX/2,//lower right x: center of laptop screen + 1/2 buttonsize
			logoBottomY + ENC_AIMLOGO_GAP_BOTTOM + buttonSizeY + i * (ENC_BTN_GAP + buttonSizeY),//lower right y: below logo + logo gap + button height + previous buttons and button gaps
			MSYS_PRIORITY_HIGH,//priority
			CURSOR_WWW,//cursor
			MSYS_NO_CALLBACK,//moveCB
			BtnEncyclopedia_newSelectDataPageRegionCallBack);
		MSYS_SetRegionUserData( &gEncyclopediaBtnRegions[i], 0, i + 1 );
		CHECKF( MSYS_AddRegion( &gEncyclopediaBtnRegions[i] ) );
	}
#endif
	return TRUE;
}

/**
* Destroys video objects, buttons, fasthelp texts, mousregions.
* Called on exit of encyclopedia main page in laptop.
* @return FALSE on error. TRUE on success.
* @see EnterEncyclopedia_NEW()
*/
BOOLEAN ExitEncyclopedia_NEW(  )
{
	BOOLEAN success = TRUE;
	
	//error if not enabled
	success &= !gGameExternalOptions.gEncyclopedia;
	//error if the laptop mode does not match encyclopedia
	success &= !( LAPTOP_MODE_ENCYCLOPEDIA == guiPreviousLaptopMode );
	// destroy background graphic
	success &= DeleteVideoObjectFromIndex( guiEncyclopediaBG );
	// destroy AIM logo
	success &= DeleteVideoObjectFromIndex( guiEncyclopediaAimLogo );

#ifdef ENC_USE_BUTTONSYSTEM//use button system
	// destroy buttons
	for ( UINT8 i = 0; i < ENC_NUM_SUBPAGES; i++ )
		if ( giEncyclopediaBtn[ i ] != BUTTON_NO_SLOT )
		{
			RemoveButton( giEncyclopediaBtn[ i ] );
			giEncyclopediaBtn[ i ] = BUTTON_NO_SLOT;
		}
		else
			success = FALSE;
	// destroy button graphic
	if ( giEncyclopediaBtnImage != BUTTON_NO_IMAGE )
	{
		UnloadButtonImage( giEncyclopediaBtnImage );
		giEncyclopediaBtnImage = BUTTON_NO_IMAGE;
	}
	else
		success = FALSE;
#else
	// destroy button graphic
	success &= DeleteVideoObjectFromIndex( guiEncyclopediaBtnImage );
	// destroy mouseregions for buttons
	for (UINT8 i = 0; i < ENC_NUM_SUBPAGES; i++)
		MSYS_RemoveRegion( &gEncyclopediaBtnRegions[i] );
#endif
	return success;
}

/**
* Renders main page of encyclopedia.
*/
void RenderEncyclopedia_NEW(  )
{
	HVOBJECT hVObject;
	UINT16 x,y;

	CHECKV( gGameExternalOptions.gEncyclopedia );

	// render background
	x = LAPTOP_SCREEN_UL_X;
	y = LAPTOP_SCREEN_WEB_UL_Y;
	CHECKV( BltVideoObjectFromIndex ( FRAME_BUFFER, guiEncyclopediaBG, 0, x, y, VO_BLT_SRCTRANSPARENCY, NULL ) );

	// render AIM logo
	GetVideoObject( &hVObject, guiEncyclopediaAimLogo );
	CHECKV(hVObject);CHECKV(hVObject->pETRLEObject);
	x = LAPTOP_SCREEN_UL_X + (LAPTOP_SCREEN_WIDTH - hVObject->pETRLEObject->usWidth)/2;	//Center of Laptop screen
	y = LAPTOP_SCREEN_WEB_UL_Y + ENC_AIMLOGO_GAP_TOP;									//20 pixel below title bar of www
	CHECKV( BltVideoObjectFromIndex( FRAME_BUFFER, guiEncyclopediaAimLogo, 0, x, y, VO_BLT_SRCTRANSPARENCY, NULL ) );

	// render Buttons for Data pages
#ifdef ENC_USE_BUTTONSYSTEM
	RenderButtons();
#else
	for ( UINT8 i = 0; i < ENC_NUM_SUBPAGES; i++ )
	{ 
		x = gEncyclopediaBtnRegions[ i ].RegionTopLeftX;
		y = gEncyclopediaBtnRegions[ i ].RegionTopLeftY;
		//Btn graphic
		CHECKV( BltVideoObjectFromIndex( FRAME_BUFFER, guiEncyclopediaBtnImage, 0, x, y, VO_BLT_SRCTRANSPARENCY, NULL ) );
		//Btn text
		y += (gEncyclopediaBtnRegions[ i ].RegionBottomRightY - y)/2 - GetFontHeight( FONT12ARIAL )/2;
		DrawTextToScreen( pMenuStrings[ i ], x, y, (UINT16)gEncyclopediaBtnRegions[ i ].RegionBottomRightX - x, FONT12ARIAL, FONT_FCOLOR_WHITE, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
#endif
	// finish render
	CHECKV ( RenderWWWProgramTitleBar() );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);	
}

/**
* @brief Sets an item as visible in encyclopedia.
* Unsetting single items is not allowed by design. However you can reset all items as not discovered by using \ref EncyclopediaInitItemsVisibility().
* If the visibility parameter is less strict then the value assigned to gbEncyclopediaData_ItemVisible no action is taken.
* Usage:
* Whenever an item is viewed in stash, on enemies (steal menu, enemy tooltip), dealers inventory ec. call this function.
* @param itemIndex Index of INVTYPE Item[MAXITEMS]
* @param visibility see \ref ENC_ITEM_VISIBILITY_T. Do not take external options into account when choosing that value, those are handled when displaying the items in encyclopedia.
*/
void EncyclopediaSetItemAsVisible( UINT16 itemIndex, ENC_ITEM_VISIBILITY_T visibility )
{
	CHECKV(gGameExternalOptions.gEncyclopedia);
	CHECKV(itemIndex < MAXITEMS);

	if( gbEncyclopediaData_ItemVisible[ itemIndex ] < visibility )
		gbEncyclopediaData_ItemVisible[ itemIndex ] = visibility;
}

/**
* @brief Sets all items as not discovered.
* Called during loading of old saveGame.
*/
void EncyclopediaInitItemsVisibility()
{
	memset( gbEncyclopediaData_ItemVisible, ENC_ITEM_NOT_DISCOVERED, MAXITEMS*sizeof(UINT8) );
}

#endif

/**
* @brief Saves Item visibility array to file.
*
* @return TRUE if data is written.
*/
BOOLEAN SaveEncyclopediaItemVisibility( HWFILE hFile )
{
	UINT32 bytesWritten;

	CHECKF( FileWrite( hFile, gbEncyclopediaData_ItemVisible, MAXITEMS, &bytesWritten ) );

	return (TRUE);
}

/**
* @brief Loads item visibility array from file.
*
* @return TRUE if data is read.
*/
BOOLEAN LoadEncyclopediaItemVisibility( HWFILE hFile )
{
	UINT32 bytesRead;

	CHECKF( FileRead( hFile, gbEncyclopediaData_ItemVisible, MAXITEMS, &bytesRead) );

	return (TRUE);
}
///@}

#ifdef ENCYCLOPEDIA_WORKS
/**
* @brief Prepares a change to a different subpage.
* Subpages currently available: Loactions, Characters, Items, Quests.
* @param ubSubPageNumber 1 to ENC_NUM_SUBPAGES (4)
* @see ENC_SUBPAGE_T
*/
void ChangingEncyclopediaSubPage( UINT8 ubSubPageNumber )
{
	CHECKV( gGameExternalOptions.gEncyclopedia );
	AssertGT(ubSubPageNumber,0);
	AssertLE(ubSubPageNumber, ENC_NUM_SUBPAGES);

	fLoadPendingFlag = TRUE;//tell laptop it should load another page
	fConnectingToSubPage = TRUE;//faster loading on same web site
	fFastLoadFlag = FALSE;//accessing some data subpage, so do not load too fast
	geENC_SubPage = static_cast<ENC_SUBPAGE_T>( ubSubPageNumber );
}

//////////////
//Callback functions
#ifdef ENC_USE_BUTTONSYSTEM//use button system
void BtnEncyclopedia_newSelectDataPageBtnCallBack( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			if ( MSYS_GetBtnUserData( btn, 1 ) == ENC_MAINPAGE )
				guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA;
			else
			{
				ChangingEncyclopediaSubPage( (UINT8)MSYS_GetBtnUserData( btn, 1 ) );
				guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA_DATA;
			}
			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}
#else
/**
* @brief Callback for data page buttons.
* Userdata at index 0 is used to determine which button is pressed.
*/
void BtnEncyclopedia_newSelectDataPageRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	CHECKV( gGameExternalOptions.gEncyclopedia );
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT8 selectedButton = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );

		if( selectedButton == 0 )
		{
			guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA;
		}
		else if( selectedButton > 0 && selectedButton <= ENC_NUM_SUBPAGES )
		{
			ChangingEncyclopediaSubPage ( selectedButton );
			guiCurrentLaptopMode = LAPTOP_MODE_ENCYCLOPEDIA_DATA;
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	}
}
#endif

#endif
