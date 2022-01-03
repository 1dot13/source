#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include	"Types.h"
	#include	"Options Screen.h"
	#include	"Video.h"
	#include	"Font Control.h"
	#include	"Game Clock.h"
	#include	"Text Input.h"
	#include	"WordWrap.h"
	#include	"SaveLoadScreen.h"
	#include	"Render Dirty.h"
	#include	"WordWrap.h"
	#include	"WCheck.h"
	#include	"Utilities.h"
	#include	"Slider.h"
	#include	"Debug.h"
	#include	"Music Control.h"
	#include	"Sound Control.h"
	#include	"soundman.h"
	#include	"Ambient Control.h"
	#include	"Worlddat.h"
	#include	"Worlddef.h"
	#include	"GameSettings.h"
	#include	"Game Init.h"
	#include	"English.h"
	#include	"Overhead.h"
	#include	"Gap.h"
	#include	"Cursors.h"
	#include	"SysUtil.h"
	#include	"Exit Grids.h"
	#include	"Text.h"
	#include	"Interface Control.h"
	#include	"Message.h"
	#include	"Language Defines.h"
	#include "Multi Language Graphic Utils.h"
	#include "Map Information.h"
	#include "SmokeEffects.h"
	#include "Sys Globals.h"
#endif

#include		"Cheats.h"
#include		"connect.h"
#include		"WorldMan.h"
#include		"Init.h"
#include		"Game Events.h"
#include		"PostalService.h"
extern CPostalService gPostalService;

/////////////////////////////////
//
//	Global Variables
//
/////////////////////////////////

UINT32		guiOptionBackGroundImage;
UINT32		guiOptionsAddOnImages;

UINT32		guiSoundEffectsSliderID;
UINT32		guiSpeechSliderID;
UINT32		guiMusicSliderID;

BOOLEAN		gfOptionsScreenEntry = TRUE;
BOOLEAN		gfOptionsScreenExit	= FALSE;
BOOLEAN		gfRedrawOptionsScreen = TRUE;

//CHAR8		gzSavedGameName[ 128 ];			//old
BOOLEAN		gfEnteredFromMapScreen=FALSE;

UINT32		guiOptionsScreen = OPTIONS_SCREEN;
UINT32		guiPreviousOptionScreen = OPTIONS_SCREEN;

BOOLEAN		gfExitOptionsDueToMessageBox=FALSE;
BOOLEAN		gfExitOptionsAfterMessageBox = FALSE;


UINT32		guiSoundFxSliderMoving = 0xffffffff;
UINT32		guiSpeechSliderMoving = 0xffffffff;

INT32		giOptionsMessageBox = -1;			// Options pop up messages index value

INT8		gbHighLightedOptionText = -1;

//BOOLEAN		gfHideBloodAndGoreOption=FALSE;	//all this blood gore option enforcment is unused
												//If a germany build we are to hide the blood and gore option

//UINT8		gubFirstColOfOptions=OPT_FIRST_COLUMN_TOGGLE_CUT_OFF; // old


BOOLEAN		gfSettingOfTreeTopStatusOnEnterOfOptionScreen;	//this is temp holder for the world state prior to any changes
BOOLEAN		gfSettingOfItemGlowStatusOnEnterOfOptionScreen;	//this is temp holder for the world state prior to any changes
BOOLEAN		gfSettingOfDontAnimateSmoke;					//this is temp holder for the world state prior to any changes
BOOLEAN		gfSettingOfTacticalFaceIcon;					//this is temp holder for the world state prior to any changes

// Goto save game Button
void BtnOptGotoSaveGameCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiOptGotoSaveGameBtn;
INT32		giOptionsButtonImages;

// Goto load game button
void BtnOptGotoLoadGameCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiOptGotoLoadGameBtn;
INT32		giGotoLoadBtnImage;

// QuitButton
void BtnOptQuitCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiQuitButton;
INT32		giQuitBtnImage;

// 1.13 Features Button
void Btn113FeaturesCallback(GUI_BUTTON *btn,INT32 reason);
UINT32 gui113FeaturesButton;
INT32 gi113FeaturesBtnImage;

// arynn : need more option screen toggles : Add in button that allow for options column paging
// Options Screen globals
INT16	OptionsList_Column_Offset	= 0 ;	// the first column's -or- "half page" start
INT16	Current_First_Option_Index;			// index of the first (single) option in the first column, used in lookup option vs togglebox
INT16	Max_Number_Of_Pages	= 0 ;			// dynamic column counter per build/mode type
void Establish_Options_Screen_Rules(void);				// define the display rules
void Build_Options_List_Reinterpretation(void);			// interpret rules and build a set of which options are used for use in options screen
void Create_Toggle_Buttons(void);				// these two are blcok removals from functions, an isolation of toggle create/destroy
void Destroy_Toggle_Buttons(void);
void Handle_ButtonStyle_Options( UINT8 Button_UserData_1 );

// Next Button
void BtnOptNextCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiOptNextButton;
INT32		giOptNextBtnImage;

// Prev Button
void BtnOptPrevCallback(GUI_BUTTON *btn,INT32 reason);
UINT32		guiOptPrevButton;
INT32		giOptPrevBtnImage;

// Done Button
void BtnDoneCallback(GUI_BUTTON *btn,INT32 reason);
extern UINT32	guiDoneButton; // symbol already declared globally in AimArchives.cpp (jonathanl)
INT32		giDoneBtnImage;


//checkbox to toggle tracking mode on or off
UINT32	guiOptionsToggles[ MAX_NUMBER_OF_OPTION_TOGGLES ]; //array of ButtonID, index's for button list
BOOL Buttons_Exist_State = 0;
void BtnOptionsTogglesCallback(GUI_BUTTON *btn,INT32 reason);


//Mouse regions for the name of the option
MOUSE_REGION	gSelectedOptionTextRegion[ MAX_NUMBER_OF_OPTION_TOGGLES ];
void		SelectedOptionTextRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void		SelectedOptionTextRegionMovementCallBack(MOUSE_REGION * pRegion, INT32 reason );


//Mouse regions for the area around the toggle boxs
MOUSE_REGION	gSelectedToggleBoxAreaRegion;
void		SelectedToggleBoxAreaRegionMovementCallBack(MOUSE_REGION * pRegion, INT32 reason );

//toggle box correspondence to option tracker
//1 define the render rules, 
//2 recast real options into new array under constraints of rules, 
//3 then using a pager system, create a subset that will fit on screen
int toggle_box_content_rules[NUM_GAME_OPTIONS];		// index = game option					value = render state
int toggle_box_gGameSettings_recast[NUM_GAME_OPTIONS+OPT_FIRST_COLUMN_TOGGLE_CUT_OFF];	
													// index = reinterpreted positions		value = game_option or state
	//" + OPT_FIRST_COLUMN_TOGGLE_CUT_OFF " to account for potential empty togglebox positions up to next whole column
int toggle_box_array[MAX_NUMBER_OF_OPTION_TOGGLES];	// index = option position on screen	value = game_option or state
/////////////////////////////////
//
//	Function ProtoTypes
//
/////////////////////////////////

BOOLEAN			EnterOptionsScreen();
void			RenderOptionsScreen();
void			ExitOptionsScreen();
void			HandleOptionsScreen();
void			GetOptionsScreenUserInput();
void			NextPage();
void			PreviousPage();

void			SoundFXSliderChangeCallBack( INT32 iNewValue );
void			SpeechSliderChangeCallBack( INT32 iNewValue );
void			MusicSliderChangeCallBack( INT32 iNewValue );
//BOOLEAN			DoOptionsMessageBox( UINT8 ubStyle, STR16zString, UINT32 uiExitScreen, UINT8 ubFlags, MSGBOX_CALLBACK ReturnCallback );
void			ConfirmQuitToMainMenuMessageBoxCallBack( UINT8 bExitValue );
void			HandleSliderBarMovementSounds();
void			HandleOptionToggle( UINT8 Button_UserData_0, UINT8 Button_UserData_1, BOOLEAN fState, BOOLEAN fDown, BOOLEAN fPlaySound );
void			HandleHighLightedText( BOOLEAN fHighLight );

extern	BOOLEAN CheckIfGameCdromIsInCDromDrive();
extern	void ToggleItemGlow( BOOLEAN fOn );

/////////////////////////////////
//
//	Code
//
/////////////////////////////////

UINT32	OptionsScreenInit()
{

	//Set so next time we come in, we can set up
	gfOptionsScreenEntry = TRUE;

	//init rules and paging extent
	Establish_Options_Screen_Rules();
	Build_Options_List_Reinterpretation();	//need to calc paging max before next/prev button creation

	return( TRUE );
}

UINT32	OptionsScreenHandle()
{
	StartFrameBufferRender();

	if( gfOptionsScreenEntry )
	{
		PauseGame();
		EnterOptionsScreen();
		gfOptionsScreenEntry = FALSE;
		gfOptionsScreenExit = FALSE;
		gfRedrawOptionsScreen = TRUE;
		RenderOptionsScreen();

		//Blit the background to the save buffer
		BlitBufferToBuffer( guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	RestoreBackgroundRects();

	GetOptionsScreenUserInput();

	HandleOptionsScreen();

	if( gfRedrawOptionsScreen )
	{
		RenderOptionsScreen();
		RenderButtons();

		gfRedrawOptionsScreen = FALSE;
	}

	//Render the active slider bars
	RenderAllSliderBars();

	// render buttons marked dirty
	MarkButtonsDirty( );
	RenderButtons( );

	// ATE: Put here to save RECTS before any fast help being drawn...
	SaveBackgroundRects( );
	RenderButtonsFastHelp();


	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();


	if( gfOptionsScreenExit )
	{
		ExitOptionsScreen();
		gfOptionsScreenExit = FALSE;
		gfOptionsScreenEntry = TRUE;

		UnPauseGame();
	}

	return( guiOptionsScreen );
}

UINT32	OptionsScreenShutdown()
{
	return( TRUE );
}

void Establish_Options_Screen_Rules(void)
{
	UINT16	counter;
// ary-05/05/2009 : note : pre-emptive definition of "game options render state". Show/Hide/etc options on option screen.
//	: note : enum of states possible :
//  : note : state -2 = is a state defined later, after rules build-up, and denotes the unused spots that exceed the final page.
//	: note : state -1 = used in toggle_box_array, after rules build-up. and denotes the unused spots that are within last page.
//	: note : state  0 = skip this option, consider it to be non-existant.. do not render.. 
//	: note : state  1 = normal, default
//	: note : state  2 = text only, no box, no option to toggle, a "header/divider" type rendering
	
	//Re-init toggle_box_content_rules[]
	for( counter = 0; counter < NUM_GAME_OPTIONS; counter++)
	{
		//this loop will re-initialize toggle_box_content_rules[counter1] to "display as normally" state
		toggle_box_content_rules[counter] = 1;
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////// begining of options content rules ////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// these rules are first created in "normal" state. So change those where needed.
	//if( (is_networked) ) //establish any Multiplayer Options rules
	//{
	//	toggle_box_content_rules[TOPTION_TOGGLE_TURN_MODE] = 0; // do not consider FTM an option when in MP mode
	//}

	if( !(gGameSettings.fOptions[ TOPTION_SHOW_RESET_ALL_OPTIONS ]) )
	{
		// hide this to prevent user mistake of reseting options
		toggle_box_content_rules[TOPTION_RESET_ALL_OPTIONS] = 0;
	}
	if( JA2BETAVERSION_FLAG || gGameSettings.fOptions[TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE] ) 
	{
		toggle_box_content_rules[TOPTION_CHEAT_MODE_OPTIONS_HEADER]			= 2;	
		toggle_box_content_rules[TOPTION_CHEAT_MODE_OPTIONS_END]			= 2;	
		toggle_box_content_rules[TOPTION_DEBUG_MODE_OPTIONS_HEADER]			= 2;	//a sample options screen options header (text)
		toggle_box_content_rules[TOPTION_DEBUG_MODE_OPTIONS_END]			= 2;	//a sample options screen options divider(text)
		toggle_box_content_rules[TOPTION_LAST_OPTION]						= 2;	//this is THE LAST option that exists
		//example of "options screens option" in action :
		if( !(gGameSettings.fOptions[TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP]))
		{
			toggle_box_content_rules[TOPTION_RENDER_MOUSE_REGIONS] = 0; 
		}
	}
	else //establish any NOT in "DEBUG build mode" rules
	{
		toggle_box_content_rules[TOPTION_CHEAT_MODE_OPTIONS_HEADER]			= 0;	
		toggle_box_content_rules[TOPTION_FORCE_BOBBY_RAY_SHIPMENTS]			= 0;	
		toggle_box_content_rules[TOPTION_CHEAT_MODE_OPTIONS_END]			= 0;	
		toggle_box_content_rules[TOPTION_DEBUG_MODE_OPTIONS_HEADER]			= 0;	//a sample options screen options header (text)
		toggle_box_content_rules[TOPTION_REPORT_MISS_MARGIN]				= 0;	
		toggle_box_content_rules[TOPTION_SHOW_RESET_ALL_OPTIONS]			= 0;	//a sample option that hides/shows another option
		toggle_box_content_rules[TOPTION_RESET_ALL_OPTIONS]					= 0;	// kept inside Debug Builds (dev's quick reset)
		toggle_box_content_rules[TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE]	= 0;	
		toggle_box_content_rules[TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP]	= 0;	//a sample option that hides/shows another option(s)
		toggle_box_content_rules[TOPTION_RENDER_MOUSE_REGIONS]				= 0;	//a sample DEBUG build option
		toggle_box_content_rules[TOPTION_DEBUG_MODE_OPTIONS_END]			= 0;	//a sample options screen options divider
		toggle_box_content_rules[TOPTION_LAST_OPTION]						= 0;	//this is THE LAST option that exists

		////however if allowing some debug options to bleed thru to release. Allow these options to be managable.
		//if( gGameSettings.fOptions[TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE] )
		//{
		//	toggle_box_content_rules[TOPTION_RETAIN_DEBUG_OPTIONS_IN_RELEASE]	= 1;	
		//	toggle_box_content_rules[TOPTION_DEBUG_MODE_RENDER_OPTIONS_GROUP]	= 1;
		//	toggle_box_content_rules[TOPTION_RENDER_MOUSE_REGIONS]				= 1;	
		//}
	}
	//at this point despite whatever build rules, enable cheat mode options to render
	if( CHEATER_CHEAT_LEVEL( ) ) 
	{
		toggle_box_content_rules[TOPTION_CHEAT_MODE_OPTIONS_HEADER]			= 2;	// text only
		toggle_box_content_rules[TOPTION_FORCE_BOBBY_RAY_SHIPMENTS]			= 1;	
		toggle_box_content_rules[TOPTION_CHEAT_MODE_OPTIONS_END]			= 2;	// text only
	}
}

void Build_Options_List_Reinterpretation(void)
{
	// This function fills out a recast of gGameSettings.fOptions[], 
	// it differs by constraint foun in Establish_Options_Screen_Rules()

	INT16 counter1, counter2, index_of_last_valid_option;

	for(counter1= 0 ; counter1 < (NUM_GAME_OPTIONS+OPT_FIRST_COLUMN_TOGGLE_CUT_OFF) ; counter1++)
	{ toggle_box_gGameSettings_recast[counter1] = -2; }

	index_of_last_valid_option = 0; //track where inside the recast we ran out of actual options, 
									//we then pad the rest with -1's
									//also, this is the basis for column counting
	
// ary-05/05/2009 : counter1 represents the position inside toggle_box_gGameSettings_recast[], we dont skip these until there are no more options
//		 : counter2 represents the position inside gGameSettings.fOptions[], skip invalids until valid or NUM_GAME_OPTIONS
	counter2 = 0; // init prior to the loop
	for( counter1 = 0 ; counter1 < NUM_GAME_OPTIONS ; counter1++)
	{
		for( ; counter2<NUM_GAME_OPTIONS ; )
		{	
			if(toggle_box_content_rules[counter2]) // might be change to bit_vect, or some other mechanic, but for only 0 is not rendered
			{
				toggle_box_gGameSettings_recast[counter1] = counter2;
				index_of_last_valid_option = counter1;
				counter2++;
				break;
			}
			counter2++;
		}
	}

	// pick up final tail of toggle box up to the next whole column
	if((index_of_last_valid_option+1) % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)
	{
		Max_Number_Of_Pages = ((index_of_last_valid_option+1) / OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)+1;
		for(	counter1 = (index_of_last_valid_option+1) ;
				counter1 < (Max_Number_Of_Pages * OPT_FIRST_COLUMN_TOGGLE_CUT_OFF); 
				counter1++ 	) 
		{
			toggle_box_gGameSettings_recast[counter1] = -1;
		}
	}
	else 
	{ 
		Max_Number_Of_Pages = ((index_of_last_valid_option+1) / OPT_FIRST_COLUMN_TOGGLE_CUT_OFF);
		// no tail to fill out, index_of_last_valid_option is the last option up to a whole column
	}

}


void Create_Toggle_Buttons(void)
{
	UINT16	counter1;
	UINT16	usPosY, usBoxPosX, usTextPosX;
	UINT16	usTextWidth, usTextHeight;

//
// Toggle Boxes
//
//		Build up a set of options toggles for render and callbacks. and remember set for deletion of set.
//
// main events:
//		fill toggle_box_content_rules[] (content rules), and global state after conditions have been setup
//		fill toggle_box_array[], with options based on content rule, or global rules
//		show check_box images and text, create mouse regions, based on toggle_box_array[]
//		see Destroy_Toggle_Buttons() for handling of deletion (this is all setup to prevent deletion of uncreated regions)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	Establish_Options_Screen_Rules();
	Build_Options_List_Reinterpretation();	// based on rules, build a options_screen version of gGameSetting[], and derive max_pages

// at this point we have an array that describes how the toggle boxes will be handled
// only thing left now is to describe the array of toggle_boxes that is currently about to be displayed on screen
//		this should be easy enough, just offset into toggle_box_gGameSettings_recast, and direct copy into toggle_box_array

	Current_First_Option_Index	= OptionsList_Column_Offset * OPT_FIRST_COLUMN_TOGGLE_CUT_OFF;

	for( counter1 = 0; counter1<MAX_NUMBER_OF_OPTION_TOGGLES; counter1++)
	{
		//counter1 represents index within toggle_box_array, we have to handle each one
		toggle_box_array[counter1] = toggle_box_gGameSettings_recast[counter1+Current_First_Option_Index];
	}

	// at this point toggle_box_array has been built properly to content rules.
	// now it is time to create/render/keep track of the toggle box options

	usTextHeight = GetFontHeight( OPT_MAIN_FONT );

	for( counter1=0; counter1<MAX_NUMBER_OF_OPTION_TOGGLES; counter1++)
	{
		//	skip creating toggle box for cases : no option , option supression 		
		if (toggle_box_array[counter1] == -1) // skip every thing for -1
		{	continue;	}

		if (toggle_box_content_rules[toggle_box_array[counter1]] == 2) //skip creating the toggle box | mouse regions
		{	// this case isnt creating toggle box, but is still using positions on the list, so adjust the graphics cursor

			// init or re-init base points of where graphics/text/regions are going to be placed
			if( counter1 % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF == 0 )
			{
				//reset the vertical position
				usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y;
			}
			//skip over vertical portion
			usPosY += OPT_GAP_BETWEEN_TOGGLE_BOXES;

			//init/reset horizontal
			if( counter1 < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF )
			{
				usBoxPosX = OPT_TOGGLE_BOX_FIRST_COLUMN_X;
				usTextPosX = OPT_TOGGLE_BOX_FIRST_COL_TEXT_X;
			}
			else
			{
				usBoxPosX = OPT_TOGGLE_BOX_SECOND_COLUMN_X;
				usTextPosX = OPT_TOGGLE_BOX_SECOND_TEXT_X;
			}

			continue;	
		}

		// init or re-init base points of where graphics/text/regions are going to be placed
		if( counter1 % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF == 0 )
		{
			//reset the vertical position
			usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y;
		}
		if( counter1 < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF )
		{
			usBoxPosX = OPT_TOGGLE_BOX_FIRST_COLUMN_X;
			usTextPosX = OPT_TOGGLE_BOX_FIRST_COL_TEXT_X;
		}
		else
		{
			usBoxPosX = OPT_TOGGLE_BOX_SECOND_COLUMN_X;
			usTextPosX = OPT_TOGGLE_BOX_SECOND_TEXT_X;
		}

		// here is the point when the graphical toggle is drawn and associated with a mouse region

		//Check box ID stored to guiOptionsToggles[] by toggle option
		guiOptionsToggles[ counter1 ] = CreateCheckBoxButton( usBoxPosX, usPosY,
															"INTERFACE\\OptionsCheckBoxes_12x12.sti", 
															MSYS_PRIORITY_HIGH+10,
															BtnOptionsTogglesCallback );

		//set the toggle box's UserData[0] to the toggle_box index (which toggle box it is)
		//set the toggle box's UserData[1] to the option's index (which option the toggle box represents)
		MSYS_SetBtnUserData( guiOptionsToggles[ counter1 ], 0, counter1 );
		MSYS_SetBtnUserData( guiOptionsToggles[ counter1 ], 1, toggle_box_array[counter1] );

		usTextWidth = StringPixLength( zOptionsToggleText[ toggle_box_array[ counter1 ] ], OPT_MAIN_FONT );

		if( usTextWidth > OPT_TOGGLE_BOX_TEXT_WIDTH )
		{
			//Get how many lines will be used to display the string, without displaying the string
			UINT8	ubNumLines = (UINT8) (	DisplayWrappedString( 0, 0, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_HIGHLIGHT_COLOR, 
											zOptionsToggleText[ toggle_box_array[ counter1 ] ], FONT_MCOLOR_BLACK, TRUE, 
											LEFT_JUSTIFIED | DONT_DISPLAY_TEXT ) / GetFontHeight( OPT_MAIN_FONT ) );

			usTextWidth = OPT_TOGGLE_BOX_TEXT_WIDTH;
	
			//Create mouse regions for the option toggle text
			MSYS_DefineRegion(	&gSelectedOptionTextRegion[counter1], usBoxPosX+13, usPosY, 
								(usTextPosX + usTextWidth), (UINT16)(usPosY+usTextHeight*ubNumLines), MSYS_PRIORITY_HIGH,
								CURSOR_NORMAL, SelectedOptionTextRegionMovementCallBack, SelectedOptionTextRegionCallBack );
			MSYS_AddRegion( &gSelectedOptionTextRegion[counter1]);

			//set the text's region's UserData[0] to the toggle_box index (which toggle box it is)
			//set the text's region's UserData[1] to the option's index (which option the toggle box represents)
			MSYS_SetRegionUserData( &gSelectedOptionTextRegion[ counter1 ], 0, counter1);
			MSYS_SetRegionUserData( &gSelectedOptionTextRegion[ counter1 ], 1, toggle_box_array[counter1]);
		}
		else
		{
			//Create mouse regions for the option toggle text
			MSYS_DefineRegion(	&gSelectedOptionTextRegion[counter1], usBoxPosX+13, usPosY, 
								(usTextPosX + usTextWidth), (UINT16)(usPosY+usTextHeight), MSYS_PRIORITY_HIGH,
								CURSOR_NORMAL, SelectedOptionTextRegionMovementCallBack, SelectedOptionTextRegionCallBack );
			MSYS_AddRegion( &gSelectedOptionTextRegion[counter1]);

			//set the text's region's UserData[0] to the toggle_box index (which toggle box it is)
			//set the text's region's UserData[1] to the option's index (which option the toggle box represents)
			MSYS_SetRegionUserData( &gSelectedOptionTextRegion[ counter1 ], 0, counter1);
			MSYS_SetRegionUserData( &gSelectedOptionTextRegion[ counter1 ], 1, toggle_box_array[counter1]);
		}

		SetRegionFastHelpText( &gSelectedOptionTextRegion[ counter1 ], zOptionsScreenHelpText[ toggle_box_array[ counter1 ] ] );
		SetButtonFastHelpText( guiOptionsToggles[ counter1 ], zOptionsScreenHelpText[ toggle_box_array[ counter1 ] ] );	

		usPosY += OPT_GAP_BETWEEN_TOGGLE_BOXES;
	}

	Buttons_Exist_State = 1; // ok check boxes now exist, its safe to allow text highlights
}



void Destroy_Toggle_Buttons(void)
{
	UINT8	counter1;

	Buttons_Exist_State = 0;	// set this to off, prevents HandleOptionsScreen() from causing a CTD when tring to  
								// higlight text for non-existing checkboxes

	//Remove the toggle buttons
	for( counter1=0; counter1<MAX_NUMBER_OF_OPTION_TOGGLES; counter1++)
	{
		if( toggle_box_array[ counter1 ] == -1  )
		{
			// dont delete non existing buttons
			continue;
		}		
		if( toggle_box_content_rules[toggle_box_array[ counter1 ]] == 2  )
		{
			// dont delete non existing buttons
			continue;
		}

		RemoveButton( guiOptionsToggles[ counter1 ] );

	    MSYS_RemoveRegion( &gSelectedOptionTextRegion[counter1]);
	}

}

BOOLEAN EnterOptionsScreen()
{
	VOBJECT_DESC	VObjectDesc;

	// WANNE: Do not draw the blackground back
	//ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );

	//if we are coming from mapscreen
	if( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
	{
		guiTacticalInterfaceFlags &= ~INTERFACE_MAPSCREEN;
		gfEnteredFromMapScreen = TRUE;
	}

	// Stop ambients...
	StopAmbients( );

	guiOptionsScreen = OPTIONS_SCREEN;

	//Init the slider bar;
	InitSlider();

	if( gfExitOptionsDueToMessageBox )
	{
		gfRedrawOptionsScreen = TRUE;
		gfExitOptionsDueToMessageBox = FALSE;
		return( TRUE );
	}

	gfExitOptionsDueToMessageBox = FALSE;

	// load the options screen background graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\OptionScreenBase.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiOptionBackGroundImage));

	// load button, title graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_OPTIONHEADER );
	CHECKF(AddVideoObject(&VObjectDesc, &guiOptionsAddOnImages));

	//Save game button
	giOptionsButtonImages = LoadButtonImage("INTERFACE\\OptionScreenAddons2.sti", -1,2,-1,3,-1 );
	guiOptGotoSaveGameBtn = CreateIconAndTextButton( giOptionsButtonImages, zOptionsText[OPT_SAVE_GAME], OPT_BUTTON_FONT2,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													OPT_SAVE_BTN_X, OPT_SAVE_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnOptGotoSaveGameCallback);
	SpecifyDisabledButtonStyle( guiOptGotoSaveGameBtn, DISABLED_STYLE_HATCHED );
	if( guiPreviousOptionScreen == MAINMENU_SCREEN || !CanGameBeSaved() || guiPreviousOptionScreen == GAME_INIT_OPTIONS_SCREEN )
	{
		DisableButton( guiOptGotoSaveGameBtn );
	}

	//Load game button
	giGotoLoadBtnImage = UseLoadedButtonImage( giOptionsButtonImages, -1,2,-1,3,-1 );
	guiOptGotoLoadGameBtn = CreateIconAndTextButton( giGotoLoadBtnImage, zOptionsText[OPT_LOAD_GAME], OPT_BUTTON_FONT2,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													OPT_LOAD_BTN_X, OPT_LOAD_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnOptGotoLoadGameCallback);
//	SpecifyDisabledButtonStyle( guiBobbyRAcceptOrder, DISABLED_STYLE_SHADED );

	//Quit to main menu button
	giQuitBtnImage = UseLoadedButtonImage( giOptionsButtonImages, -1,2,-1,3,-1 );
	guiQuitButton = CreateIconAndTextButton( giQuitBtnImage, zOptionsText[OPT_MAIN_MENU], OPT_BUTTON_FONT2,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													OPT_QUIT_BTN_X, OPT_QUIT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnOptQuitCallback);
	SpecifyDisabledButtonStyle( guiQuitButton, DISABLED_STYLE_HATCHED );
//	DisableButton( guiQuitButton );

	// 1.13 Features Button
	gi113FeaturesBtnImage = UseLoadedButtonImage( giOptionsButtonImages, -1,2,-1,3,-1 );
	gui113FeaturesButton = CreateIconAndTextButton( gi113FeaturesBtnImage, zOptionsText[OPT_NEW_IN_113], OPT_BUTTON_FONT2,
													FONT_MCOLOR_LTYELLOW, DEFAULT_SHADOW,
													FONT_MCOLOR_LTYELLOW, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													OPT_SWAP_BTN_X, OPT_SWAP_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, Btn113FeaturesCallback);
	if (is_networked)
	{
		DisableButton(gui113FeaturesButton);
	}

// ary-05/05/2009 : need more option screen toggles : Add in buttons that allow for options column paging
	// Previous Column of options
 	giOptPrevBtnImage = UseLoadedButtonImage( giOptionsButtonImages, -1,2,-1,3,-1 );                         
 	guiOptPrevButton = CreateIconAndTextButton( giOptPrevBtnImage, zOptionsText[OPT_PREV], OPT_BUTTON_FONT2,
 													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,                                                      
 													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,                                                     
 													TEXT_CJUSTIFIED,                                                                          
 													OPT_PREV_BTN_X, OPT_PREV_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,                        
 													DEFAULT_MOVE_CALLBACK, BtnOptPrevCallback);                                               
 	SpecifyDisabledButtonStyle( guiOptPrevButton, DISABLED_STYLE_HATCHED );  
	if( OptionsList_Column_Offset == 0 )
	{
		DisableButton( guiOptPrevButton );
	}

	// Next Column of options
 	giOptNextBtnImage = UseLoadedButtonImage( giOptionsButtonImages, -1,2,-1,3,-1 );                         
 	guiOptNextButton = CreateIconAndTextButton( giOptNextBtnImage, zOptionsText[OPT_NEXT], OPT_BUTTON_FONT2,
 													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,                                                      
 													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,                                                     
 													TEXT_CJUSTIFIED,                                                                          
 													OPT_NEXT_BTN_X, OPT_NEXT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,                        
 													DEFAULT_MOVE_CALLBACK, BtnOptNextCallback);                                               
 	SpecifyDisabledButtonStyle( guiOptNextButton, DISABLED_STYLE_HATCHED );   
	if( OptionsList_Column_Offset >= (Max_Number_Of_Pages-2) )// ary-05/05/2009 : "Max_Number_Of_Pages-2" becaues each "page" = 2 "columns"
	{
		DisableButton( guiOptNextButton );
	}

	//Done button
	giDoneBtnImage = UseLoadedButtonImage( giOptionsButtonImages, -1,2,-1,3,-1 );
	guiDoneButton = CreateIconAndTextButton( giDoneBtnImage, zOptionsText[OPT_DONE], OPT_BUTTON_FONT2,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													OPT_DONE_BTN_X, OPT_DONE_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnDoneCallback);
//	SpecifyDisabledButtonStyle( guiBobbyRAcceptOrder, DISABLED_STYLE_SHADED );

	Create_Toggle_Buttons(); // ary-05/05/2009 : moved block out of func

	//Create a mouse region so when the user leaves a togglebox text region we can detect it then unselect the region
	MSYS_DefineRegion( &gSelectedToggleBoxAreaRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_NORMAL,
							CURSOR_NORMAL, SelectedToggleBoxAreaRegionMovementCallBack, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &gSelectedToggleBoxAreaRegion );


	//Render the scene before adding the slider boxes
	RenderOptionsScreen();

	//Add a slider bar for the Sound Effects
	guiSoundEffectsSliderID = AddSlider(	SLIDER_VERTICAL_STEEL, CURSOR_NORMAL, OPT_SOUND_EFFECTS_SLIDER_X, OPT_SOUND_EFFECTS_SLIDER_Y, 
											OPT_SLIDER_BAR_SIZE, 127, MSYS_PRIORITY_HIGH, SoundFXSliderChangeCallBack, 0 );
	AssertMsg( guiSoundEffectsSliderID, "Failed to AddSlider" );
	SetSliderValue( guiSoundEffectsSliderID, GetSoundEffectsVolume() );

	//Add a slider bar for the Speech
	guiSpeechSliderID = AddSlider(	SLIDER_VERTICAL_STEEL, CURSOR_NORMAL, OPT_SPEECH_SLIDER_X, OPT_SPEECH_SLIDER_Y, 
									OPT_SLIDER_BAR_SIZE, 127, MSYS_PRIORITY_HIGH, SpeechSliderChangeCallBack, 0 );
	AssertMsg( guiSpeechSliderID, "Failed to AddSlider" );
	SetSliderValue( guiSpeechSliderID, GetSpeechVolume() );

	//Add a slider bar for the Music
	guiMusicSliderID = AddSlider(	SLIDER_VERTICAL_STEEL, CURSOR_NORMAL, OPT_MUSIC_SLIDER_X, OPT_MUSIC_SLIDER_Y, 
									OPT_SLIDER_BAR_SIZE, 127, MSYS_PRIORITY_HIGH, MusicSliderChangeCallBack, 0 );
	AssertMsg( guiMusicSliderID, "Failed to AddSlider" );
	SetSliderValue( guiMusicSliderID, MusicGetVolume() );



	//Remove the mouse region over the clock
	RemoveMouseRegionForPauseOfClock(	);

	//Draw the screen
	gfRedrawOptionsScreen = TRUE;

	//Set the option screen toggle boxes
	SetOptionsScreenToggleBoxes();

	DisableScrollMessages();

	//reset
	gbHighLightedOptionText = -1;

	//get the status of the tree top option
	gfSettingOfTreeTopStatusOnEnterOfOptionScreen = gGameSettings.fOptions[ TOPTION_TOGGLE_TREE_TOPS ];

	//Get the status of the item glow option
	gfSettingOfItemGlowStatusOnEnterOfOptionScreen = gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ];

	gfSettingOfDontAnimateSmoke = gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ];

	gfSettingOfTacticalFaceIcon = gGameSettings.fOptions[ TOPTION_SHOW_TACTICAL_FACE_ICONS ];
	return( TRUE );
}

void ExitOptionsScreen()
{

	if( gfExitOptionsDueToMessageBox )
	{
		gfOptionsScreenExit = FALSE;

		if( !gfExitOptionsAfterMessageBox )
			return;
		gfExitOptionsAfterMessageBox = FALSE;
		gfExitOptionsDueToMessageBox = FALSE;
	}

	//Get the current status of the toggle boxes
	GetOptionsScreenToggleBoxes();	//currently empty, used to interpret button states to determine gGameSettings.fOptions[]
									//its still remains in case of future dev where any final interpertations may be needed
	//The save the current settings to disk
	SaveGameSettings(); 

	//Create the clock mouse region
//	CreateMouseRegionForPauseOfClock( CLOCK_REGION_START_X, CLOCK_REGION_START_Y );

	if( guiOptionsScreen == GAME_SCREEN )
	{
		EnterTacticalScreen( );
	}

	if (guiOptionsScreen == SAVE_LOAD_SCREEN && guiPreviousOptionScreen == MAINMENU_SCREEN)
	{
		giMAXIMUM_NUMBER_OF_PLAYER_SLOTS = CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS;
		InitDependingGameStyleOptions();
	}

	RemoveButton( guiOptGotoSaveGameBtn );
	RemoveButton( guiOptGotoLoadGameBtn );
	RemoveButton( guiQuitButton );
	RemoveButton( gui113FeaturesButton );
	RemoveButton( guiOptNextButton );// ary-05/05/2009 : more option screen toggles
	RemoveButton( guiOptPrevButton );
	RemoveButton( guiDoneButton );

	UnloadButtonImage( giOptionsButtonImages );
	UnloadButtonImage( giGotoLoadBtnImage );
	UnloadButtonImage( giQuitBtnImage );
	UnloadButtonImage( gi113FeaturesBtnImage);
	UnloadButtonImage( giOptNextBtnImage );// ary-05/05/2009 : more option screen toggles
	UnloadButtonImage( giOptPrevBtnImage );
	UnloadButtonImage( giDoneBtnImage );

	DeleteVideoObjectFromIndex( guiOptionBackGroundImage );
	DeleteVideoObjectFromIndex( guiOptionsAddOnImages );


	Destroy_Toggle_Buttons(); // ary-05/05/2009 : moved block out of func


	//REmove the slider bars
	RemoveSliderBar( guiSoundEffectsSliderID );
	RemoveSliderBar( guiSpeechSliderID );
	RemoveSliderBar( guiMusicSliderID );


	MSYS_RemoveRegion( &gSelectedToggleBoxAreaRegion );

	ShutDownSlider();

	//if we are coming from mapscreen
	if( gfEnteredFromMapScreen )
	{
		gfEnteredFromMapScreen = FALSE;
		guiTacticalInterfaceFlags |= INTERFACE_MAPSCREEN;
	}

	//if the user changed the TREE TOP option, AND a world is loaded
	// sevenfm: always update tree top state
	//if( gfSettingOfTreeTopStatusOnEnterOfOptionScreen != gGameSettings.fOptions[ TOPTION_TOGGLE_TREE_TOPS ] && gfWorldLoaded )
	if (gfWorldLoaded)
	{
		SetTreeTopStateForMap();
	}

	//if the user has changed the item glow option AND a world is loaded
	if( gfSettingOfItemGlowStatusOnEnterOfOptionScreen != gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] && gfWorldLoaded )
	{
		ToggleItemGlow( gGameSettings.fOptions[ TOPTION_GLOW_ITEMS ] );
	}

	if( gfSettingOfDontAnimateSmoke != gGameSettings.fOptions[ TOPTION_ANIMATE_SMOKE ] && gfWorldLoaded )
	{
		UpdateSmokeEffectGraphics( );
	}

	//CHRISL: Reinitialize portrait graphic
	if( gfSettingOfTacticalFaceIcon != gGameSettings.fOptions[ TOPTION_SHOW_TACTICAL_FACE_ICONS ] )
	{
		InitializeTacticalPortraits( );
	}

}

void HandleOptionsScreen()
{
	HandleSliderBarMovementSounds();

	HandleHighLightedText( TRUE );
}

void RenderOptionsScreen()
{
	HVOBJECT hPixHandle;
	UINT16	usPosY, usPosX;
	UINT16	usWidth=0;
	UINT8	count;
	CHAR16	sPage[60];

	//Get and display the background image
	GetVideoObject(&hPixHandle, guiOptionBackGroundImage);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, iScreenWidthOffset, iScreenHeightOffset, VO_BLT_SRCTRANSPARENCY,NULL);

	//Get and display the titla image
	GetVideoObject(&hPixHandle, guiOptionsAddOnImages);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, iScreenWidthOffset, iScreenHeightOffset, VO_BLT_SRCTRANSPARENCY,NULL);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 1, iScreenWidthOffset, iScreenHeightOffset + 434, VO_BLT_SRCTRANSPARENCY,NULL);

	//
	// Text for the toggle boxes
	//

	//Display the First column of toggles
	for( count=0; count<MAX_NUMBER_OF_OPTION_TOGGLES; count++)
	{
		if( toggle_box_array[ count ] == -1  )
		{	break;	}// no text for non existing buttons

		//setup base points of where graphics/text/regions are going to be placed
		if( count % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF == 0 )
		{
			//reset the vertical position
			usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y + OPT_TOGGLE_TEXT_OFFSET_Y;
		}
		if( count < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF )
		{
			usPosX = OPT_TOGGLE_BOX_FIRST_COL_TEXT_X;
		}
		else
		{
			usPosX = OPT_TOGGLE_BOX_SECOND_TEXT_X;
		}

		usWidth = StringPixLength( zOptionsToggleText[ toggle_box_array[ count ] ], OPT_MAIN_FONT );

		//if the string is going to wrap, move the string up a bit
		if( usWidth > OPT_TOGGLE_BOX_TEXT_WIDTH )
			DisplayWrappedString(	usPosX, usPosY, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_MAIN_COLOR, 
									zOptionsToggleText[ toggle_box_array[ count ] ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		else
			DrawTextToScreen(	zOptionsToggleText[ toggle_box_array[ count ] ], usPosX, usPosY, 0, 
								OPT_MAIN_FONT, OPT_MAIN_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

		usPosY += OPT_GAP_BETWEEN_TOGGLE_BOXES;
	}

	//
	// Text for the Slider Bars
	//

	//Display the Sound Fx text
	DisplayWrappedString(	OPT_SOUND_FX_TEXT_X, OPT_SOUND_FX_TEXT_Y, OPT_SLIDER_TEXT_WIDTH, 2, OPT_SLIDER_FONT, OPT_MAIN_COLOR, 
							zOptionsText[ OPT_SOUND_FX ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);

	//Display the Speech text
	DisplayWrappedString(	OPT_SPEECH_TEXT_X, OPT_SPEECH_TEXT_Y, OPT_SLIDER_TEXT_WIDTH, 2, OPT_SLIDER_FONT, OPT_MAIN_COLOR, 
							zOptionsText[ OPT_SPEECH ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED	);

	//Display the Music text
	DisplayWrappedString(	OPT_MUSIC_TEXT_X, OPT_MUSIC_TEXT_Y, OPT_SLIDER_TEXT_WIDTH, 2, OPT_SLIDER_FONT, OPT_MAIN_COLOR, 
							zOptionsText[ OPT_MUSIC ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the option page numbers
	swprintf( sPage, L"%d / %d", OptionsList_Column_Offset + 1, Max_Number_Of_Pages - 1 );
	DisplayWrappedString(	OPT_PAGE_X, OPT_PAGE_Y, OPT_SLIDER_TEXT_WIDTH, 2, OPT_BUTTON_FONT2, OPT_MAIN_COLOR, 
							sPage, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );

	InvalidateRegion( OPTIONS__TOP_LEFT_X, OPTIONS__TOP_LEFT_Y, OPTIONS__BOTTOM_RIGHT_X, OPTIONS__BOTTOM_RIGHT_Y);

}

void GetOptionsScreenUserInput()
{
	InputAtom Event;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while (DequeueSpecificEvent(&Event, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{
				case ESC:
					SetOptionsExitScreen( guiPreviousOptionScreen );
					break;

				//Enter the save game screen
				case 's':
				case 'S':
					//if the save game button isnt disabled
					if( ButtonList[ guiOptGotoSaveGameBtn ]->uiFlags & BUTTON_ENABLED )
					{
						SetOptionsExitScreen( SAVE_LOAD_SCREEN );
						gfSaveGame = TRUE;
					}
					break;

				//Enter the Load game screen
				case 'l':
				case 'L':
					SetOptionsExitScreen( SAVE_LOAD_SCREEN );
					gfSaveGame = FALSE;
					break;

				// toggle between features and options screen
				case TAB:
					SetOptionsExitScreen(FEATURES_SCREEN);
					break;

				// page left
				case 'a':
				case 'A':
				case LEFTARROW:
					PreviousPage();
					break;

				// page right
				case 'd':
				case 'D':
				case RIGHTARROW:
					NextPage();
					break;

#ifdef JA2TESTVERSION

				case 'r':
					gfRedrawOptionsScreen = TRUE;
					break;

				case 'i':
					InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
					break;

					//Test keys

				case 'y':
					{
					static	UINT32	uiTest2 = NO_SAMPLE;
					if( !SoundIsPlaying( uiTest2 ) )
						uiTest2 = PlayJA2SampleFromFile( "Sounds\\RAID Dive.wav", RATE_11025, HIGHVOLUME, 1 , MIDDLEPAN );
					}
					break;
				case 't':
				{
/*
					{
						CHAR8		zCdromRootDrive[512];
						HWFILE	hFile;

						if( !GetCdromLocationFromIniFile( zCdromRootDrive ) )
							return;

						//Check if a file exists on the cdrom
						sprintf( zCdromRootDrive, "%sData\\NPC_Speech.slf", zCdromRootDrive );

				//		if( FileExistsNoDB( zCdromRootDrive ) )
						hFile = FileOpen( zCdromRootDrive, FILE_ACCESS_READ | FILE_OPEN_EXISTING, FALSE );
						if( hFile != 0 )
						{
							FileClose( hFile );
							sprintf( zCdromRootDrive, "%sData\\NPC_Speech.slf", zCdromRootDrive );
						}
						else
						{
							sprintf( zCdromRootDrive, "%sData\\NPC_Speech.slf", zCdromRootDrive );
						}

						uiLastTimeToCheckCDromDrive = GetJA2Clock();
					}
*/
				}
				break;


				case 'z':
					SetErrorMode( SEM_FAILCRITICALERRORS );
					break;

				case 'q':
//					ShouldMercSayPrecedentToRepeatOneSelf( 11, 99 );
					break;
#endif
			}
		}
	}

	// mousewheel input
	if (OptionsList_Column_Offset > 0)
	{
		BOOL act = FALSE;

		// check general screen mouseregion
		if (gSelectedToggleBoxAreaRegion.WheelState > 0)
		{
			act = TRUE;
		}

		// check toggle box mouseregions
		for (int i = 0; i < MAX_NUMBER_OF_OPTION_TOGGLES; ++i)
		{
			if (act) break;
			if (gSelectedOptionTextRegion[i].WheelState > 0)
			{
				act = TRUE;
			}
		}

		if (act) PreviousPage();
	}

	if (OptionsList_Column_Offset < Max_Number_Of_Pages-2)
	{
		BOOL act = FALSE;

		// check general screen mouseregion
		if (gSelectedToggleBoxAreaRegion.WheelState < 0)
		{
			act = TRUE;
		}

		// check toggle box mouseregions
		for (int i = 0; i < MAX_NUMBER_OF_OPTION_TOGGLES; ++i)
		{
			if (act) break;
			if (gSelectedOptionTextRegion[i].WheelState < 0)
			{
				act = TRUE;
			}
		}

		if (act) NextPage();
	}
}

void NextPage()
{
	ExitOptionsScreen();

	OptionsList_Column_Offset++;
	if (OptionsList_Column_Offset >= Max_Number_Of_Pages - 1) // ary-05/05/2009 : Max_Number_Of_Pages
		OptionsList_Column_Offset = (Max_Number_Of_Pages - 2);

	OptionsScreenInit();	// ary-05/05/2009 : This is important to refresh the screen properly, it stores a new SAVE_BUFFER
}

void PreviousPage()
{
	ExitOptionsScreen();

	OptionsList_Column_Offset--;
	if(OptionsList_Column_Offset < 0)
		OptionsList_Column_Offset = 0;

	OptionsScreenInit();	// ary-05/05/2009 : This is important to refresh the screen properly, it stores a new SAVE_BUFFER
}

void SetOptionsExitScreen( UINT32 uiExitScreen )
{
	OptionsList_Column_Offset	= 0 ; // reset the pager till next visit
	guiOptionsScreen = uiExitScreen;
	gfOptionsScreenExit	= TRUE;
}

void SetOptionsPreviousScreen( UINT32 uiPrevScreen, BOOLEAN setFeaturesToo )
{
	guiPreviousOptionScreen = uiPrevScreen;
	if (setFeaturesToo == TRUE)
		FeaturesScreen::SetPreviousScreen(uiPrevScreen, FALSE);
}

void BtnOptGotoSaveGameCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		SetOptionsExitScreen( SAVE_LOAD_SCREEN );
		gfSaveGame = TRUE;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptGotoLoadGameCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		SetOptionsExitScreen( SAVE_LOAD_SCREEN );
		gfSaveGame = FALSE;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptQuitCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//Confirm the Exit to the main menu screen
		DoOptionsMessageBox(	MSG_BOX_BASIC_STYLE, zOptionsText[OPT_RETURN_TO_MAIN], OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, 
								ConfirmQuitToMainMenuMessageBoxCallBack );

///		SetOptionsExitScreen( MAINMENU_SCREEN );

		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
}

void Btn113FeaturesCallback(GUI_BUTTON* btn, INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		SetOptionsExitScreen(FEATURES_SCREEN);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

// ary-05/05/2009 : need more option screen toggles : Functions for button callbacks
void BtnOptPrevCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		PreviousPage();

		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
}

void BtnOptNextCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		NextPage();

		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
}


void BtnDoneCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		SetOptionsExitScreen( guiPreviousOptionScreen );

		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(	btn->Area.RegionTopLeftX,		btn->Area.RegionTopLeftY, 
							btn->Area.RegionBottomRightX,	btn->Area.RegionBottomRightY);
	}
}

void BtnOptionsTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	UINT8	Button_UserData_0 = (UINT8)MSYS_GetBtnUserData( btn, 0 );// the toggle box
	UINT8	Button_UserData_1 = (UINT8)MSYS_GetBtnUserData( btn, 1 );// the option

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			HandleOptionToggle( Button_UserData_0, Button_UserData_1, TRUE, FALSE, FALSE );
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			HandleOptionToggle( Button_UserData_0, Button_UserData_1, FALSE, FALSE, FALSE);
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			HandleOptionToggle( Button_UserData_0, Button_UserData_1, TRUE, TRUE, FALSE );
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			HandleOptionToggle( Button_UserData_0, Button_UserData_1, FALSE, TRUE, FALSE );
		}
	}
}

void Handle_ButtonStyle_Options( UINT8 Button_UserData_1 )
{
	// note : This function is where all immediately apply effects occur
	//		:	These options represent button like applications. 
	//		:	They are meant to be clicked on, then performed, then reset to off

	if( Button_UserData_1 == TOPTION_RESET_ALL_OPTIONS)
	{
		gGameSettings.fOptions[TOPTION_RESET_ALL_OPTIONS]		= FALSE; // kind of redundant,
		gGameSettings.fOptions[TOPTION_SHOW_RESET_ALL_OPTIONS]	= FALSE; // but example of what would happen after button handlement

		InitGameSettings();
	}

	if( Button_UserData_1 == TOPTION_FORCE_BOBBY_RAY_SHIPMENTS)
	{
		STRATEGICEVENT	*pEvent = gpEventList;

		gGameSettings.fOptions[TOPTION_FORCE_BOBBY_RAY_SHIPMENTS]		= FALSE; // turn it back off

		if ( guiPreviousOptionScreen == MAINMENU_SCREEN) return; //GTFO, not in game,

		if( pEvent ) 
		{
			// clean up events prior to the DeliverShipment block (below)
			// so that we accuately determine EVENT_POSTAL_SERVICE_SHIPMENT events have been removed
			DeleteEventsWithDeletionPending(); 
		}
		else
		{
			return; //nothing here, leave
		}

		while( pEvent )
		{
			if ( pEvent->ubCallbackID == EVENT_POSTAL_SERVICE_SHIPMENT)
			{
					gPostalService.DeliverShipment(pEvent->uiParam);
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Forced Bobby Ray's delivery.");
					// ok now clean up the events we just forced..
					// Use Assert () to help find any cases where these events are NOT deleted 
					//	If they are still in EventList, an empty next Shipment will crash the game
					pEvent->ubFlags |= SEF_DELETION_PENDING;
					pEvent = pEvent->next; //move to next before deleting current event
					Assert(DeleteEventsWithDeletionPending());


			}
			else
			{
					pEvent = pEvent->next; //not a shipment move to next
			}
		}
				
	}

}

void HandleOptionToggle( UINT8 Button_UserData_0, UINT8 Button_UserData_1, BOOLEAN fState, BOOLEAN fDown, BOOLEAN fPlaySound )
{
	//Button_UserData_0 = which toggle box within guiOptionsToggles[Button_UserData_0]
	//Button_UserData_1 = which option the toggle box represents

	static UINT32	uiOptionToggleSound = NO_SAMPLE;

	if( fState )
	{
		gGameSettings.fOptions[ Button_UserData_1 ] = TRUE;

		// WANNE: Some settings in a MP game cannot be enabled!
		if (is_networked)
		{
			// General (client & server) restrictions
			switch (Button_UserData_1)
			{
				case TOPTION_ALLOW_REAL_TIME_SNEAK:
				case TOPTION_ALLOW_CROWS:
				case TOPTION_ALLOW_SOLDIER_TOOLTIPS:
				case TOPTION_USE_AUTO_SAVE:
				case TOPTION_TOGGLE_TURN_MODE:
				case TOPTION_ZOMBIES:
					gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;
				break;
			}

			// "Pure" client restrictions
			if (!is_server)
			{
				switch (Button_UserData_1)
				{
					case TOPTION_AUTO_FAST_FORWARD_MODE:
						gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;
						break;
				}
			}
		}
		else
		{
			switch (Button_UserData_1)
			{
				// Disable the automatic FF, when the highspeed timer is not set in the ja2.ini
				case TOPTION_AUTO_FAST_FORWARD_MODE:
					if (!IsHiSpeedClockMode())
						gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;
					break;
			}
		}

		ButtonList[ guiOptionsToggles[ Button_UserData_0 ] ]->uiFlags |= BUTTON_CLICKED_ON;

		if( fDown )
			DrawCheckBoxButtonOn( guiOptionsToggles[ Button_UserData_0 ] );

		Handle_ButtonStyle_Options( Button_UserData_1 );

	}
	else
	{
		//first test any conditions for non-commital, and/or pre-commital behavior

		//check to see if the user is unselecting either the spech or subtitles toggle (pseudo radio style)
		if( Button_UserData_1 == TOPTION_SPEECH	)
		{
			gGameSettings.fOptions[TOPTION_SUBTITLES] = TRUE;
		}
		if( Button_UserData_1 == TOPTION_SUBTITLES )
		{
			gGameSettings.fOptions[TOPTION_SPEECH] = TRUE;
		}
			
		//ok now commit
		gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;


		// WANNE: Some settings in a MP game cannot be enabled!
		if (is_networked)
		{
			// General (client & server) restrictions
			switch (Button_UserData_1)
			{
				case TOPTION_ALLOW_REAL_TIME_SNEAK:
				case TOPTION_ALLOW_CROWS:
				case TOPTION_ALLOW_SOLDIER_TOOLTIPS:
				case TOPTION_USE_AUTO_SAVE:
				case TOPTION_TOGGLE_TURN_MODE:
				case TOPTION_ZOMBIES:
					gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;
				break;
			}

			// "Pure" client restrictions
			if (!is_server)
			{
				switch (Button_UserData_1)
				{
					case TOPTION_AUTO_FAST_FORWARD_MODE:
						gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;
						break;
				}
			}
		}
		else
		{
			switch (Button_UserData_1)
			{
				// Disable the automatic FF, when the highspeed timer is not set in the ja2.ini
				case TOPTION_AUTO_FAST_FORWARD_MODE:
					if (!IsHiSpeedClockMode())
						gGameSettings.fOptions[ Button_UserData_1 ] = FALSE;
					break;
			}
		}


		ButtonList[ guiOptionsToggles[ Button_UserData_0 ] ]->uiFlags &= ~BUTTON_CLICKED_ON;

		if( fDown )
			DrawCheckBoxButtonOff( guiOptionsToggles[ Button_UserData_0 ] );

	}

	//stop the sound if
//	if( SoundIsPlaying( uiOptionToggleSound ) && !fDown )
	{
		SoundStop( uiOptionToggleSound );
	}


	if( fPlaySound )
	{
		if( fDown )
		{
		//				case BTN_SND_CLICK_OFF:
			PlayJA2Sample( BIG_SWITCH3_IN, RATE_11025, BTNVOLUME, 1, MIDDLEPAN );
		}
		else
		{
		//		case BTN_SND_CLICK_ON:
			PlayJA2Sample( BIG_SWITCH3_OUT, RATE_11025, BTNVOLUME, 1, MIDDLEPAN );
		}
	}

	ExitOptionsScreen();// ary-05/05/2009 : refresh the screen, toggling some options now affect the rendering of the screen
	OptionsScreenInit();

}

void SoundFXSliderChangeCallBack( INT32 iNewValue )
{
	SetSoundEffectsVolume( iNewValue );

	guiSoundFxSliderMoving = GetJA2Clock();
}

void SpeechSliderChangeCallBack( INT32 iNewValue )
{
	SetSpeechVolume( iNewValue );

	guiSpeechSliderMoving = GetJA2Clock();
}

void MusicSliderChangeCallBack( INT32 iNewValue )
{
	MusicSetVolume( iNewValue );
}

BOOLEAN DoOptionsMessageBoxWithRect(	UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT32 usFlags,
										MSGBOX_CALLBACK ReturnCallback, SGPRect *pCenteringRect )
{
	// reset exit mode
	gfExitOptionsDueToMessageBox = TRUE;

	// do message box and return
	giOptionsMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	
										ReturnCallback,	pCenteringRect );

	// send back return state
	return( ( giOptionsMessageBox != -1 ) );
}

BOOLEAN DoOptionsMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

	// reset exit mode
	gfExitOptionsDueToMessageBox = TRUE;

	// do message box and return
	giOptionsMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	
										ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giOptionsMessageBox != -1 ) );
}

void ConfirmQuitToMainMenuMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, Quit to main menu
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gfEnteredFromMapScreen = FALSE;
		gfExitOptionsAfterMessageBox = TRUE;
		SetOptionsExitScreen( MAINMENU_SCREEN );

		if (is_networked)
		{
			// haydent
			server_disconnect();
			client_disconnect();

		}

		//We want to reinitialize the game
		ReStartingGame();
	}
	else
	{
		gfExitOptionsAfterMessageBox = FALSE;
		gfExitOptionsDueToMessageBox = FALSE;
	}
}

void SetOptionsScreenToggleBoxes()
{
	UINT8	counter;

	for( counter=0; counter < MAX_NUMBER_OF_OPTION_TOGGLES; counter++)
	{
		if ( toggle_box_array[ counter ] == -1) 
			{ continue; }//there are no options to set for this toggle_box
		if ( toggle_box_content_rules[ toggle_box_array[ counter ] ] == 2)
			{ continue; }//there are no options to set for this toggle_box

		if( gGameSettings.fOptions[  toggle_box_array[ counter ]  ] )
			ButtonList[ guiOptionsToggles[ counter ] ]->uiFlags |= BUTTON_CLICKED_ON;
		else
			ButtonList[ guiOptionsToggles[ counter ] ]->uiFlags &= (~BUTTON_CLICKED_ON );
	}
}

void GetOptionsScreenToggleBoxes()
{
// ary-05/05/2009 :	note : the function is now entirely defunct, it used to reset option based on button states, 
//			note : but now options states are handled more directly
//			note : I'm leaving the function here just in case some future form of 'wrap-up' code might be needed
;
}

void HandleSliderBarMovementSounds()
{
	static UINT32	uiLastSoundFxTime=0;
	static UINT32	uiLastSpeechTime=0;
	static UINT32	uiLastPlayingSoundID = NO_SAMPLE;
	static UINT32	uiLastPlayingSpeechID = NO_SAMPLE;

	if( ( uiLastSoundFxTime - OPT_MUSIC_SLIDER_PLAY_SOUND_DELAY ) > guiSoundFxSliderMoving )
	{
		guiSoundFxSliderMoving = 0xffffffff;

		//The slider has stopped moving, reset the ambient sector sounds ( so it will change the volume )
		if( !DidGameJustStart() )
			HandleNewSectorAmbience( gTilesets[ giCurrentTilesetID ].ubAmbientID );

		if( !SoundIsPlaying( uiLastPlayingSoundID ) )
			uiLastPlayingSoundID = PlayJA2SampleFromFile( "Sounds\\Weapons\\LMG Reload.wav", RATE_11025, HIGHVOLUME, 1 , MIDDLEPAN );
	}
	else
		uiLastSoundFxTime = GetJA2Clock();


	if( ( uiLastSpeechTime - OPT_MUSIC_SLIDER_PLAY_SOUND_DELAY ) > guiSpeechSliderMoving )
	{
		guiSpeechSliderMoving = 0xffffffff;

		if( !SoundIsPlaying( uiLastPlayingSpeechID ) )
			uiLastPlayingSpeechID = PlayJA2GapSample( "BattleSnds\\m_cool.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN, NULL );
	}
	else
		uiLastSpeechTime = GetJA2Clock();

}

void SelectedOptionTextRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	//Region_UserData_0 = which toggle box within guiOptionsToggles[Button_UserData_0]
	//Region_UserData_1 = which option the toggle box represents

	UINT8	Region_UserData_0 = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
	UINT8	Region_UserData_1 = (UINT8)MSYS_GetRegionUserData( pRegion, 1 );

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{


		HandleOptionToggle( Region_UserData_0, Region_UserData_1, (BOOLEAN)(!gGameSettings.fOptions[ Region_UserData_1 ]), FALSE, TRUE );

		InvalidateRegion(	pRegion->RegionTopLeftX,		pRegion->RegionTopLeftY, 
							pRegion->RegionBottomRightX,	pRegion->RegionBottomRightY);
	}


	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )//iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||
	{
		if( gGameSettings.fOptions[ Region_UserData_1 ] )
		{
			HandleOptionToggle( Region_UserData_0, Region_UserData_1, TRUE, TRUE, TRUE );
		}
		else
		{
			HandleOptionToggle( Region_UserData_0, Region_UserData_1, FALSE, TRUE, TRUE );
		}
	}

}

void SelectedOptionTextRegionMovementCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	INT8	Region_UserData_0 = (INT8)MSYS_GetRegionUserData( pRegion, 0 );

	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		HandleHighLightedText( FALSE );
		gbHighLightedOptionText = -1;
		InvalidateRegion(	pRegion->RegionTopLeftX,		pRegion->RegionTopLeftY, 
							pRegion->RegionBottomRightX,	pRegion->RegionBottomRightY);
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		gbHighLightedOptionText = Region_UserData_0;
		InvalidateRegion(	pRegion->RegionTopLeftX,		pRegion->RegionTopLeftY, 
							pRegion->RegionBottomRightX,	pRegion->RegionBottomRightY);
	}
}

void HandleHighLightedText( BOOLEAN fHighLight )
{
	UINT16		usPosX=0;
	UINT16		usPosY=0;
	UINT8		counter;
	INT8		bHighLight=-1;
	UINT16		usWidth;

	static	INT8	bLastRegion = -1;

	if(! Buttons_Exist_State)
	{
		// there are no buttons/text to highlight, time to leave before any attempt to lookup non-existing pointer data
		gbHighLightedOptionText = -1;
		return;
	}

	if( gbHighLightedOptionText == -1 )
		fHighLight = FALSE;

	//if the user has the mouse in one of the checkboxes 
	for( counter=0; counter< MAX_NUMBER_OF_OPTION_TOGGLES; counter++)
	{
		if ( toggle_box_array[ counter ] == -1) 
			{ continue; }//there are no options to set for this toggle_box
		if ( toggle_box_content_rules[ toggle_box_array[ counter ] ] == 2)
			{ continue; }//there are no options to set for this toggle_box

		if( ButtonList[ guiOptionsToggles[ counter ] ]->Area.uiFlags & MSYS_MOUSE_IN_AREA )
		{
			gbHighLightedOptionText = counter;
			fHighLight = TRUE;
		}
	}


	// If there is a valid section being highlighted
	if( gbHighLightedOptionText != -1 )
	{
		bLastRegion = gbHighLightedOptionText;
	}

	bHighLight = gbHighLightedOptionText;


	if( bLastRegion != -1 && gbHighLightedOptionText == -1 )
	{
		fHighLight = FALSE;
		bHighLight = bLastRegion;
		bLastRegion = -1;
	}

	if( bHighLight != -1 )
	{
		if( bHighLight < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF )
		{
			usPosX = OPT_TOGGLE_BOX_FIRST_COL_TEXT_X;
			usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y + OPT_TOGGLE_TEXT_OFFSET_Y + 
						( bHighLight * OPT_GAP_BETWEEN_TOGGLE_BOXES ) ;
		}
		else
		{
			usPosX = OPT_TOGGLE_BOX_SECOND_TEXT_X;
			usPosY = OPT_TOGGLE_BOX_SECOND_COLUMN_START_Y + OPT_TOGGLE_TEXT_OFFSET_Y + 
						( ( bHighLight - OPT_FIRST_COLUMN_TOGGLE_CUT_OFF ) * OPT_GAP_BETWEEN_TOGGLE_BOXES ) ;
		}

		usWidth = StringPixLength( zOptionsToggleText[ toggle_box_array[ bHighLight ] ], OPT_MAIN_FONT );
		
		//if the string is going to wrap, move the string up a bit
		if( usWidth > OPT_TOGGLE_BOX_TEXT_WIDTH )
		{
			if( fHighLight )
				DisplayWrappedString(	usPosX, usPosY, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_HIGHLIGHT_COLOR, 
										zOptionsToggleText[ toggle_box_array[ bHighLight ] ], FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED );
			else
				DisplayWrappedString(	usPosX, usPosY, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_MAIN_COLOR, 
										zOptionsToggleText[ toggle_box_array[ bHighLight ] ], FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED );
		}
		else
		{
			if( fHighLight )
				DrawTextToScreen(	zOptionsToggleText[ toggle_box_array[ bHighLight ] ], usPosX, usPosY, 0, 
									OPT_MAIN_FONT, OPT_HIGHLIGHT_COLOR, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED	);
			else
				DrawTextToScreen(	zOptionsToggleText[ toggle_box_array[ bHighLight ] ], usPosX, usPosY, 0, 
									OPT_MAIN_FONT, OPT_MAIN_COLOR, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED	);
		}
	}
}

void SelectedToggleBoxAreaRegionMovementCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		UINT8	counter;

		//loop through all the toggle box's and remove the in area flag
		for( counter=0; counter < MAX_NUMBER_OF_OPTION_TOGGLES; counter++)
		{
			if ( toggle_box_array[ counter ] == -1) 
				{ continue; }//there are no options to set for this toggle_box
			if ( toggle_box_content_rules[ toggle_box_array[ counter ] ] == 2)
				{ continue; }//there are no options to set for this toggle_box

			ButtonList[ guiOptionsToggles[ counter ] ]->Area.uiFlags &= ~MSYS_MOUSE_IN_AREA;
		}

		gbHighLightedOptionText = -1;

		InvalidateRegion(	pRegion->RegionTopLeftX,		pRegion->RegionTopLeftY, 
							pRegion->RegionBottomRightX,	pRegion->RegionBottomRightY);
	}
}
