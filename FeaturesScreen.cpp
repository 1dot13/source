#include	"Types.h"
#include	"FeaturesScreen.h"
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
#include	"Debug.h"
#include	"Sound Control.h"
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
#include	"Multi Language Graphic Utils.h"
#include	"Map Information.h"
#include	"Sys Globals.h"
#include	"insurance.h"

#include		"connect.h"
#include		"WorldMan.h"
#include		"Init.h"
#include		"Game Events.h"

extern UINT32	guiDoneButton; // symbol already declared globally in AimArchives.cpp (jonathanl)

namespace FeaturesScreen
{

// this enum must be kept in sync with z113FeaturesScreenText
enum Text
{
	FST_TITLE,
	FST_DESCRIPTION,
	FST_HOVER_FOR_MORE_INFO,
};

/////////////////////////////////
//
//	Global Variables
//
/////////////////////////////////

UINT32		guiFeaturesBackGroundImage;
UINT32		guiFeaturesAddOnImages;

BOOLEAN		gfFeaturesScreenEntry = TRUE;
BOOLEAN		gfFeaturesScreenExit = FALSE;
BOOLEAN		gfRedrawFeaturesScreen = TRUE;

BOOLEAN		gfEnteredFromMapScreen = FALSE; // entered from options screen or new game screen

UINT32		guiFeaturesScreen = FEATURES_SCREEN;
UINT32		guiPreviousScreen = FEATURES_SCREEN;

BOOLEAN		gfExitFeaturesDueToMessageBox = FALSE;
BOOLEAN		gfExitFeaturesAfterMessageBox = FALSE;

INT32		giFeaturesMessageBox = -1;			// Options pop up messages index value

INT8		gbHighLightedOptionText = -1;

// Goto save game Button
void BtnOptGotoSaveGameCallback(GUI_BUTTON* btn, INT32 reason);
UINT32		guiOptGotoSaveGameBtn;
INT32		giOptionsButtonImages;

// Goto load game button
void BtnOptGotoLoadGameCallback(GUI_BUTTON* btn, INT32 reason);
UINT32		guiOptGotoLoadGameBtn;
INT32		giGotoLoadBtnImage;

// QuitButton
void BtnOptQuitCallback(GUI_BUTTON* btn, INT32 reason);
UINT32		guiQuitButton;
INT32		giQuitBtnImage;

// Options Button
void BtnOptionsCallback(GUI_BUTTON* btn, INT32 reason);
UINT32 guiOptionsButton;
INT32 giOptionsBtnImage;

// arynn : need more option screen toggles : Add in button that allow for options column paging
// Options Screen globals
INT16	OptionsList_Column_Offset = 0;	// the first column's -or- "half page" start
INT16	Current_First_Option_Index;			// index of the first (single) option in the first column, used in lookup option vs togglebox
INT16	Max_Number_Of_Pages = 0;			// dynamic column counter per build/mode type
void Establish_Features_Screen_Rules(void);				// define the display rules
void Build_Features_List_Reinterpretation(void);			// interpret rules and build a set of which options are used for use in options screen
void Create_Toggle_Buttons(void);				// these two are blcok removals from functions, an isolation of toggle create/destroy
void Destroy_Toggle_Buttons(void);

// Next Button
void BtnOptNextCallback(GUI_BUTTON* btn, INT32 reason);
UINT32		guiOptNextButton;
INT32		giOptNextBtnImage;

// Prev Button
void BtnOptPrevCallback(GUI_BUTTON* btn, INT32 reason);
UINT32		guiOptPrevButton;
INT32		giOptPrevBtnImage;

// Done Button
void BtnDoneCallback(GUI_BUTTON* btn, INT32 reason);
INT32		giDoneBtnImage;


//checkbox to toggle tracking mode on or off
UINT32	guiFeaturesToggles[MAX_NUMBER_OF_OPTION_TOGGLES]; //array of ButtonID, index's for button list
BOOL Buttons_Exist_State = 0;
void BtnOptionsTogglesCallback(GUI_BUTTON* btn, INT32 reason);


//Mouse regions for the name of the option
MOUSE_REGION	gSelectedOptionTextRegion[MAX_NUMBER_OF_OPTION_TOGGLES];
void		SelectedOptionTextRegionCallBack(MOUSE_REGION* pRegion, INT32 iReason);
void		SelectedOptionTextRegionMovementCallBack(MOUSE_REGION* pRegion, INT32 reason);


//Mouse regions for the area around the toggle boxs
MOUSE_REGION	gSelectedToggleBoxAreaRegion;
void		SelectedToggleBoxAreaRegionMovementCallBack(MOUSE_REGION* pRegion, INT32 reason);

//toggle box correspondence to option tracker
//1 define the render rules, 
//2 recast real options into new array under constraints of rules, 
//3 then using a pager system, create a subset that will fit on screen
int toggle_box_feature_rules[NUM_FEATURE_FLAGS];		// index = game option					value = render state
int toggle_box_gGameSettings_recast[NUM_FEATURE_FLAGS + OPT_FIRST_COLUMN_TOGGLE_CUT_OFF];
// index = reinterpreted positions		value = game_option or state
//" + OPT_FIRST_COLUMN_TOGGLE_CUT_OFF " to account for potential empty togglebox positions up to next whole column
int toggle_box_array[MAX_NUMBER_OF_OPTION_TOGGLES];	// index = option position on screen	value = game_option or state
/////////////////////////////////
//
//	Function ProtoTypes
//
/////////////////////////////////

BOOLEAN			EnterScreen();
void			RenderScreen();
void			ExitScreen();
void			HandleScreen();
void			GetUserInput();
void			SetToggleBoxes();
void			SetExitScreen(UINT32 uiExitScreen);
void			NextPage();
void			PreviousPage();

void			ConfirmQuitToMainMenuMessageBoxCallBack(UINT8 bExitValue);
void			HandleOptionToggle(UINT8 Button_UserData_0, UINT8 Button_UserData_1, BOOLEAN fState, BOOLEAN fDown, BOOLEAN fPlaySound);
void			HandleHighLightedText(BOOLEAN fHighLight);
void			DrawLeftPanel();

/////////////////////////////////
//
//	Code
//
/////////////////////////////////

UINT32	Init()
{
	//Set so next time we come in, we can set up
	gfFeaturesScreenEntry = TRUE;

	//init rules and paging extent
	Establish_Features_Screen_Rules();
	Build_Features_List_Reinterpretation();	//need to calc paging max before next/prev button creation

	return(TRUE);
}

UINT32	Handle()
{
	StartFrameBufferRender();

	if (gfFeaturesScreenEntry)
	{
		EnterScreen();
		gfFeaturesScreenEntry = FALSE;
		gfFeaturesScreenExit = FALSE;
		gfRedrawFeaturesScreen = TRUE;
		RenderScreen();

		//Blit the background to the save buffer
		BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		InvalidateRegion(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	RestoreBackgroundRects();

	GetUserInput();

	RenderScreen();

	HandleScreen();

	if (gfRedrawFeaturesScreen)
	{
		RenderButtons();

		gfRedrawFeaturesScreen = FALSE;
	}

	// render buttons marked dirty
	MarkButtonsDirty();
	RenderButtons();

	// ATE: Put here to save RECTS before any fast help being drawn...
	SaveBackgroundRects();
	RenderButtonsFastHelp();


	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();


	if (gfFeaturesScreenExit)
	{
		ExitScreen();
		gfFeaturesScreenExit = FALSE;
		gfFeaturesScreenEntry = TRUE;

		UnPauseGame();
	}

	return(guiFeaturesScreen);
}

UINT32	Shutdown()
{
	return(TRUE);
}

void Establish_Features_Screen_Rules(void)
{
	UINT16	counter;
	// ary-05/05/2009 : note : pre-emptive definition of "game options render state". Show/Hide/etc options on option screen.
	//	: note : enum of states possible :
	//  : note : state -2 = is a state defined later, after rules build-up, and denotes the unused spots that exceed the final page.
	//	: note : state -1 = used in toggle_box_array, after rules build-up. and denotes the unused spots that are within last page.
	//	: note : state  0 = skip this option, consider it to be non-existant.. do not render.. 
	//	: note : state  1 = normal, default
	//	: note : state  2 = text only, no box, no option to toggle, a "header/divider" type rendering

		//Re-init toggle_box_feature_rules[]
	for (counter = 0; counter < NUM_FEATURE_FLAGS; counter++)
	{
		//this loop will re-initialize toggle_box_feature_rules[counter1] to "display as normally" state
		toggle_box_feature_rules[counter] = 1;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	////// begining of features content rules ////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void Build_Features_List_Reinterpretation(void)
{
	// This function fills out a recast of gGameSettings.fOptions[], 
	// it differs by constraint foun in Establish_Features_Screen_Rules()

	INT16 counter1, counter2, index_of_last_valid_option;

	for (counter1 = 0; counter1 < (NUM_FEATURE_FLAGS + OPT_FIRST_COLUMN_TOGGLE_CUT_OFF); counter1++)
	{
		toggle_box_gGameSettings_recast[counter1] = -2;
	}

	index_of_last_valid_option = 0; //track where inside the recast we ran out of actual options, 
									//we then pad the rest with -1's
									//also, this is the basis for column counting

// ary-05/05/2009 : counter1 represents the position inside toggle_box_gGameSettings_recast[], we dont skip these until there are no more options
//		 : counter2 represents the position inside gGameSettings.fOptions[], skip invalids until valid or NUM_FEATURE_FLAGS
	counter2 = 0; // init prior to the loop
	for (counter1 = 0; counter1 < NUM_FEATURE_FLAGS; counter1++)
	{
		for (; counter2 < NUM_FEATURE_FLAGS; )
		{
			if (toggle_box_feature_rules[counter2]) // might be change to bit_vect, or some other mechanic, but for only 0 is not rendered
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
	if ((index_of_last_valid_option + 1) % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)
	{
		Max_Number_Of_Pages = ((index_of_last_valid_option + 1) / OPT_FIRST_COLUMN_TOGGLE_CUT_OFF) + 1;
		for (counter1 = (index_of_last_valid_option + 1);
			counter1 < (Max_Number_Of_Pages * OPT_FIRST_COLUMN_TOGGLE_CUT_OFF);
			counter1++)
		{
			toggle_box_gGameSettings_recast[counter1] = -1;
		}
	}
	else
	{
		Max_Number_Of_Pages = ((index_of_last_valid_option + 1) / OPT_FIRST_COLUMN_TOGGLE_CUT_OFF);
		// no tail to fill out, index_of_last_valid_option is the last option up to a whole column
	}

}


void Create_Toggle_Buttons(void)
{
	UINT16	counter1;
	UINT16	usPosY = 0, usBoxPosX, usTextPosX;
	UINT16	usTextWidth, usTextHeight;

	//
	// Toggle Boxes
	//
	//		Build up a set of options toggles for render and callbacks. and remember set for deletion of set.
	//
	// main events:
	//		fill toggle_box_feature_rules[] (content rules), and global state after conditions have been setup
	//		fill toggle_box_array[], with options based on content rule, or global rules
	//		show check_box images and text, create mouse regions, based on toggle_box_array[]
	//		see Destroy_Toggle_Buttons() for handling of deletion (this is all setup to prevent deletion of uncreated regions)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	Establish_Features_Screen_Rules();
	Build_Features_List_Reinterpretation();	// based on rules, build a options_screen version of gGameSetting[], and derive max_pages

// at this point we have an array that describes how the toggle boxes will be handled
// only thing left now is to describe the array of toggle_boxes that is currently about to be displayed on screen
//		this should be easy enough, just offset into toggle_box_gGameSettings_recast, and direct copy into toggle_box_array

	Current_First_Option_Index = OptionsList_Column_Offset * OPT_FIRST_COLUMN_TOGGLE_CUT_OFF;

	for (counter1 = 0; counter1 < MAX_NUMBER_OF_OPTION_TOGGLES; counter1++)
	{
		//counter1 represents index within toggle_box_array, we have to handle each one
		toggle_box_array[counter1] = toggle_box_gGameSettings_recast[counter1 + Current_First_Option_Index];
	}

	// at this point toggle_box_array has been built properly to content rules.
	// now it is time to create/render/keep track of the toggle box options

	usTextHeight = GetFontHeight(OPT_MAIN_FONT);

	for (counter1 = 0; counter1 < MAX_NUMBER_OF_OPTION_TOGGLES; counter1++)
	{
		//	skip creating toggle box for cases : no option , option supression 		
		if (toggle_box_array[counter1] < 0) // skip every thing for -1
		{
			continue;
		}

		if (toggle_box_feature_rules[toggle_box_array[counter1]] == 2) //skip creating the toggle box | mouse regions
		{	// this case isnt creating toggle box, but is still using positions on the list, so adjust the graphics cursor

			// init or re-init base points of where graphics/text/regions are going to be placed
			if (counter1 % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF == 0)
			{
				//reset the vertical position
				usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y;
			}
			//skip over vertical portion
			usPosY += OPT_GAP_BETWEEN_TOGGLE_BOXES;

			//init/reset horizontal
			if (counter1 < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)
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
		if (counter1 % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF == 0)
		{
			//reset the vertical position
			usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y;
		}
		if (counter1 < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)
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

		//Check box ID stored to guiFeaturesToggles[] by toggle option
		guiFeaturesToggles[counter1] = CreateCheckBoxButton(usBoxPosX, usPosY,
			"INTERFACE\\OptionsCheckBoxes_12x12.sti",
			MSYS_PRIORITY_HIGH + 10,
			BtnOptionsTogglesCallback);

		//set the toggle box's UserData[0] to the toggle_box index (which toggle box it is)
		//set the toggle box's UserData[1] to the option's index (which option the toggle box represents)
		MSYS_SetBtnUserData(guiFeaturesToggles[counter1], 0, counter1);
		MSYS_SetBtnUserData(guiFeaturesToggles[counter1], 1, toggle_box_array[counter1]);

		usTextWidth = StringPixLength(z113FeaturesToggleText[toggle_box_array[counter1]], OPT_MAIN_FONT);

		if (usTextWidth > OPT_TOGGLE_BOX_TEXT_WIDTH)
		{
			//Get how many lines will be used to display the string, without displaying the string
			UINT8	ubNumLines = (UINT8)(DisplayWrappedString(0, 0, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_HIGHLIGHT_COLOR,
				z113FeaturesToggleText[toggle_box_array[counter1]], FONT_MCOLOR_BLACK, TRUE,
				LEFT_JUSTIFIED | DONT_DISPLAY_TEXT) / GetFontHeight(OPT_MAIN_FONT));

			usTextWidth = OPT_TOGGLE_BOX_TEXT_WIDTH;

			//Create mouse regions for the option toggle text
			MSYS_DefineRegion(&gSelectedOptionTextRegion[counter1], usBoxPosX + 13, usPosY,
				(usTextPosX + usTextWidth), (UINT16)(usPosY + usTextHeight * ubNumLines), MSYS_PRIORITY_HIGH,
				CURSOR_NORMAL, SelectedOptionTextRegionMovementCallBack, SelectedOptionTextRegionCallBack);
			MSYS_AddRegion(&gSelectedOptionTextRegion[counter1]);

			//set the text's region's UserData[0] to the toggle_box index (which toggle box it is)
			//set the text's region's UserData[1] to the option's index (which option the toggle box represents)
			MSYS_SetRegionUserData(&gSelectedOptionTextRegion[counter1], 0, counter1);
			MSYS_SetRegionUserData(&gSelectedOptionTextRegion[counter1], 1, toggle_box_array[counter1]);
		}
		else
		{
			//Create mouse regions for the option toggle text
			MSYS_DefineRegion(&gSelectedOptionTextRegion[counter1], usBoxPosX + 13, usPosY,
				(usTextPosX + usTextWidth), (UINT16)(usPosY + usTextHeight), MSYS_PRIORITY_HIGH,
				CURSOR_NORMAL, SelectedOptionTextRegionMovementCallBack, SelectedOptionTextRegionCallBack);
			MSYS_AddRegion(&gSelectedOptionTextRegion[counter1]);

			//set the text's region's UserData[0] to the toggle_box index (which toggle box it is)
			//set the text's region's UserData[1] to the option's index (which option the toggle box represents)
			MSYS_SetRegionUserData(&gSelectedOptionTextRegion[counter1], 0, counter1);
			MSYS_SetRegionUserData(&gSelectedOptionTextRegion[counter1], 1, toggle_box_array[counter1]);
		}

		SetRegionFastHelpText(&gSelectedOptionTextRegion[counter1], z113FeaturesHelpText[toggle_box_array[counter1]]);
		SetButtonFastHelpText(guiFeaturesToggles[counter1], z113FeaturesHelpText[toggle_box_array[counter1]]);

		usPosY += OPT_GAP_BETWEEN_TOGGLE_BOXES;
	}

	Buttons_Exist_State = 1; // ok check boxes now exist, its safe to allow text highlights
}



void Destroy_Toggle_Buttons(void)
{
	UINT8	counter1;

	Buttons_Exist_State = 0;	// set this to off, prevents HandleScreen() from causing a CTD when tring to  
								// higlight text for non-existing checkboxes

	//Remove the toggle buttons
	for (counter1 = 0; counter1 < MAX_NUMBER_OF_OPTION_TOGGLES; counter1++)
	{
		if (toggle_box_array[counter1] == -1)
		{
			// dont delete non existing buttons
			continue;
		}
		if (toggle_box_feature_rules[toggle_box_array[counter1]] == 2)
		{
			// dont delete non existing buttons
			continue;
		}

		RemoveButton(guiFeaturesToggles[counter1]);

		MSYS_RemoveRegion(&gSelectedOptionTextRegion[counter1]);
	}

}

BOOLEAN EnterScreen()
{
	VOBJECT_DESC	VObjectDesc;

	//if we are coming from mapscreen
	if (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN)
	{
		guiTacticalInterfaceFlags &= ~INTERFACE_MAPSCREEN;
		gfEnteredFromMapScreen = TRUE;
	}

	// Stop ambients...
	StopAmbients();

	guiFeaturesScreen = FEATURES_SCREEN;

	if (gfExitFeaturesDueToMessageBox)
	{
		gfRedrawFeaturesScreen = TRUE;
		gfExitFeaturesDueToMessageBox = FALSE;
		return(TRUE);
	}

	gfExitFeaturesDueToMessageBox = FALSE;

	// load the options screen background graphic and add it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\OptionScreenBase.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiFeaturesBackGroundImage));

	// load button, title graphic and add it
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	GetMLGFilename(VObjectDesc.ImageFile, MLG_OPTIONHEADER);
	CHECKF(AddVideoObject(&VObjectDesc, &guiFeaturesAddOnImages));

	//Save game button
	giOptionsButtonImages = LoadButtonImage("INTERFACE\\OptionScreenAddons2.sti", -1, 2, -1, 3, -1);
	guiOptGotoSaveGameBtn = CreateIconAndTextButton(giOptionsButtonImages, zOptionsText[OPT_SAVE_GAME], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_SAVE_BTN_X, OPT_SAVE_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnOptGotoSaveGameCallback);
	SpecifyDisabledButtonStyle(guiOptGotoSaveGameBtn, DISABLED_STYLE_HATCHED);
	if (guiPreviousScreen == MAINMENU_SCREEN || !CanGameBeSaved() || guiPreviousScreen == GAME_INIT_OPTIONS_SCREEN)
	{
		DisableButton(guiOptGotoSaveGameBtn);
	}

	//Load game button
	giGotoLoadBtnImage = UseLoadedButtonImage(giOptionsButtonImages, -1, 2, -1, 3, -1);
	guiOptGotoLoadGameBtn = CreateIconAndTextButton(giGotoLoadBtnImage, zOptionsText[OPT_LOAD_GAME], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_LOAD_BTN_X, OPT_LOAD_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnOptGotoLoadGameCallback);

	//Quit to main menu button
	giQuitBtnImage = UseLoadedButtonImage(giOptionsButtonImages, -1, 2, -1, 3, -1);
	guiQuitButton = CreateIconAndTextButton(giQuitBtnImage, zOptionsText[OPT_MAIN_MENU], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_QUIT_BTN_X, OPT_QUIT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnOptQuitCallback);
	SpecifyDisabledButtonStyle(guiQuitButton, DISABLED_STYLE_HATCHED);

	// Options Button
	giOptionsBtnImage = UseLoadedButtonImage(giOptionsButtonImages, -1, 2, -1, 3, -1);
	guiOptionsButton = CreateIconAndTextButton(giOptionsBtnImage, zOptionsText[OPT_OPTIONS], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_SWAP_BTN_X, OPT_SWAP_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnOptionsCallback);

	// ary-05/05/2009 : need more option screen toggles : Add in buttons that allow for options column paging
	// Previous Column of options
	giOptPrevBtnImage = UseLoadedButtonImage(giOptionsButtonImages, -1, 2, -1, 3, -1);
	guiOptPrevButton = CreateIconAndTextButton(giOptPrevBtnImage, zOptionsText[OPT_PREV], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_PREV_BTN_X, OPT_PREV_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnOptPrevCallback);
	SpecifyDisabledButtonStyle(guiOptPrevButton, DISABLED_STYLE_HATCHED);
	if (OptionsList_Column_Offset == 0)
	{
		DisableButton(guiOptPrevButton);
	}

	// Next Column of options
	giOptNextBtnImage = UseLoadedButtonImage(giOptionsButtonImages, -1, 2, -1, 3, -1);
	guiOptNextButton = CreateIconAndTextButton(giOptNextBtnImage, zOptionsText[OPT_NEXT], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_NEXT_BTN_X, OPT_NEXT_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnOptNextCallback);
	SpecifyDisabledButtonStyle(guiOptNextButton, DISABLED_STYLE_HATCHED);
	if (OptionsList_Column_Offset >= (Max_Number_Of_Pages - 2))// ary-05/05/2009 : "Max_Number_Of_Pages-2" becaues each "page" = 2 "columns"
	{
		DisableButton(guiOptNextButton);
	}

	//Done button
	giDoneBtnImage = UseLoadedButtonImage(giOptionsButtonImages, -1, 2, -1, 3, -1);
	guiDoneButton = CreateIconAndTextButton(giDoneBtnImage, zOptionsText[OPT_DONE], OPT_BUTTON_FONT2,
		OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
		OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
		TEXT_CJUSTIFIED,
		OPT_DONE_BTN_X, OPT_DONE_BTN_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
		DEFAULT_MOVE_CALLBACK, BtnDoneCallback);

	Create_Toggle_Buttons(); // ary-05/05/2009 : moved block out of func

	//Create a mouse region so when the user leaves a togglebox text region we can detect it then unselect the region
	MSYS_DefineRegion(&gSelectedToggleBoxAreaRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_NORMAL,
		CURSOR_NORMAL, SelectedToggleBoxAreaRegionMovementCallBack, MSYS_NO_CALLBACK);
	MSYS_AddRegion(&gSelectedToggleBoxAreaRegion);

	//Remove the mouse region over the clock
	RemoveMouseRegionForPauseOfClock();

	//Draw the screen
	gfRedrawFeaturesScreen = TRUE;

	//Set the screen toggle boxes
	SetToggleBoxes();

	DisableScrollMessages();

	//reset
	gbHighLightedOptionText = -1;

	return(TRUE);
}

void ExitScreen()
{
	if (gfExitFeaturesDueToMessageBox)
	{
		gfFeaturesScreenExit = FALSE;

		if (!gfExitFeaturesAfterMessageBox)
			return;
		gfExitFeaturesAfterMessageBox = FALSE;
		gfExitFeaturesDueToMessageBox = FALSE;
	}

	//The save the current settings to disk
	SaveFeatureFlags();

	if (guiFeaturesScreen == GAME_SCREEN)
	{
		EnterTacticalScreen();
	}

	if (guiFeaturesScreen == SAVE_LOAD_SCREEN && guiPreviousScreen == MAINMENU_SCREEN)
	{
		giMAXIMUM_NUMBER_OF_PLAYER_SLOTS = CODE_MAXIMUM_NUMBER_OF_PLAYER_SLOTS;
		InitDependingGameStyleOptions();
	}

	RemoveButton(guiOptGotoSaveGameBtn);
	RemoveButton(guiOptGotoLoadGameBtn);
	RemoveButton(guiQuitButton);
	RemoveButton(guiOptionsButton);
	RemoveButton(guiOptNextButton);// ary-05/05/2009 : more option screen toggles
	RemoveButton(guiOptPrevButton);
	RemoveButton(guiDoneButton);

	UnloadButtonImage(giOptionsButtonImages);
	UnloadButtonImage(giGotoLoadBtnImage);
	UnloadButtonImage(giQuitBtnImage);
	UnloadButtonImage(giOptionsBtnImage);
	UnloadButtonImage(giOptNextBtnImage);// ary-05/05/2009 : more option screen toggles
	UnloadButtonImage(giOptPrevBtnImage);
	UnloadButtonImage(giDoneBtnImage);

	DeleteVideoObjectFromIndex(guiFeaturesBackGroundImage);
	DeleteVideoObjectFromIndex(guiFeaturesAddOnImages);

	Destroy_Toggle_Buttons(); // ary-05/05/2009 : moved block out of func

	MSYS_RemoveRegion(&gSelectedToggleBoxAreaRegion);

	//if we are coming from mapscreen
	if (gfEnteredFromMapScreen)
	{
		gfEnteredFromMapScreen = FALSE;
		guiTacticalInterfaceFlags |= INTERFACE_MAPSCREEN;
	}
}

void HandleScreen()
{
	HandleHighLightedText(TRUE);
}

void RenderScreen()
{
	HVOBJECT hPixHandle;
	UINT16	usPosY = 0, usPosX;
	UINT16	usWidth = 0;
	UINT8	count;
	CHAR16	sPage[60];

	//Get and display the background image
	if (GetVideoObject(&hPixHandle, guiFeaturesBackGroundImage))
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0, iScreenWidthOffset, iScreenHeightOffset, VO_BLT_SRCTRANSPARENCY, NULL);

	//Get and display the titla image
	if (GetVideoObject(&hPixHandle, guiFeaturesAddOnImages))
	{
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0, iScreenWidthOffset, iScreenHeightOffset, VO_BLT_SRCTRANSPARENCY, NULL);
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1, iScreenWidthOffset, iScreenHeightOffset + 434, VO_BLT_SRCTRANSPARENCY, NULL);
	}

	DrawLeftPanel();

	//
	// Text for the toggle boxes
	//

	//Display the First column of toggles
	for (count = 0; count < MAX_NUMBER_OF_OPTION_TOGGLES; count++)
	{
		if (toggle_box_array[count] == -1)
		{
			break;
		}// no text for non existing buttons

//setup base points of where graphics/text/regions are going to be placed
		if (count % OPT_FIRST_COLUMN_TOGGLE_CUT_OFF == 0)
		{
			//reset the vertical position
			usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y + OPT_TOGGLE_TEXT_OFFSET_Y;
		}
		if (count < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)
		{
			usPosX = OPT_TOGGLE_BOX_FIRST_COL_TEXT_X;
		}
		else
		{
			usPosX = OPT_TOGGLE_BOX_SECOND_TEXT_X;
		}

		usWidth = StringPixLength(z113FeaturesToggleText[toggle_box_array[count]], OPT_MAIN_FONT);

		// set the colour of the toggle box text
		UINT8 fontColor = (count == 0 && OptionsList_Column_Offset == 0) || gGameSettings.fFeatures[FF_FEATURES_SCREEN] ?
			OPT_MAIN_COLOR : FONT_GRAY7;

		//if the string is going to wrap, move the string up a bit
		if (usWidth > OPT_TOGGLE_BOX_TEXT_WIDTH)
			DisplayWrappedString(usPosX, usPosY, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, fontColor,
				z113FeaturesToggleText[toggle_box_array[count]], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
		else
			DrawTextToScreen(z113FeaturesToggleText[toggle_box_array[count]], usPosX, usPosY, 0,
				OPT_MAIN_FONT, fontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

		usPosY += OPT_GAP_BETWEEN_TOGGLE_BOXES;
	}

	//Display the option page numbers
	swprintf(sPage, L"%d / %d", OptionsList_Column_Offset + 1, max(1, Max_Number_Of_Pages - 1));
	DisplayWrappedString(OPT_PAGE_X, OPT_PAGE_Y, OPT_SLIDER_TEXT_WIDTH, 2, OPT_BUTTON_FONT2, OPT_MAIN_COLOR,
		sPage, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);

	//Set the screen toggle boxes
	SetToggleBoxes();

	InvalidateRegion(OPTIONS__TOP_LEFT_X, OPTIONS__TOP_LEFT_Y, OPTIONS__BOTTOM_RIGHT_X, OPTIONS__BOTTOM_RIGHT_Y);

}

void GetUserInput()
{
	InputAtom Event;
	POINT	MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(ghWindow, &MousePos); // In window coords!

	while (DequeueSpecificEvent(&Event, KEY_DOWN | KEY_UP | KEY_REPEAT))
	{
		if (!HandleTextInput(&Event) && Event.usEvent == KEY_DOWN)
		{
			switch (Event.usParam)
			{
			case 'q':
			case 'Q':
				//Confirm the Exit to the main menu screen
				DoMessageBox(MSG_BOX_BASIC_STYLE, zOptionsText[OPT_RETURN_TO_MAIN], FEATURES_SCREEN, MSG_BOX_FLAG_YESNO,
					ConfirmQuitToMainMenuMessageBoxCallBack);
			break;

			case ESC:
				SetExitScreen(guiPreviousScreen);
				break;

				//Enter the save game screen
			case 's':
			case 'S':
				//if the save game button isnt disabled
				if (ButtonList[guiOptGotoSaveGameBtn]->uiFlags & BUTTON_ENABLED)
				{
					SetExitScreen(SAVE_LOAD_SCREEN);
					gfSaveGame = TRUE;
				}
				break;

				//Enter the Load game screen
			case 'l':
			case 'L':
				SetExitScreen(SAVE_LOAD_SCREEN);
				gfSaveGame = FALSE;
				break;

			// toggle between features and options screen
			case TAB:
				SetExitScreen(OPTIONS_SCREEN);
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
				gfRedrawFeaturesScreen = TRUE;
				break;

			case 'i':
				InvalidateRegion(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
				break;

				//Test keys

			case 'y':
			{
				static	UINT32	uiTest2 = NO_SAMPLE;
				if (!SoundIsPlaying(uiTest2))
					uiTest2 = PlayJA2SampleFromFile("Sounds\\RAID Dive.wav", RATE_11025, HIGHVOLUME, 1, MIDDLEPAN);
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
				SetErrorMode(SEM_FAILCRITICALERRORS);
				break;

			//case 'q':
				//					ShouldMercSayPrecedentToRepeatOneSelf( 11, 99 );
				//break;
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
	ExitScreen();

	OptionsList_Column_Offset++;
	if (OptionsList_Column_Offset >= Max_Number_Of_Pages - 1) // ary-05/05/2009 : Max_Number_Of_Pages
		OptionsList_Column_Offset = (Max_Number_Of_Pages - 2);

	Init();
}

void PreviousPage()
{
	ExitScreen();

	OptionsList_Column_Offset--;
	if(OptionsList_Column_Offset < 0)
		OptionsList_Column_Offset = 0;

	Init();
}

void SetExitScreen(UINT32 uiExitScreen)
{
	OptionsList_Column_Offset = 0; // reset the pager till next visit
	guiFeaturesScreen = uiExitScreen;
	gfFeaturesScreenExit = TRUE;
}

void BtnOptGotoSaveGameCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		SetExitScreen(SAVE_LOAD_SCREEN);
		gfSaveGame = TRUE;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptGotoLoadGameCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		SetExitScreen(SAVE_LOAD_SCREEN);
		gfSaveGame = FALSE;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptQuitCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		//Confirm the Exit to the main menu screen
		DoMessageBox(MSG_BOX_BASIC_STYLE, zOptionsText[OPT_RETURN_TO_MAIN], FEATURES_SCREEN, MSG_BOX_FLAG_YESNO,
			ConfirmQuitToMainMenuMessageBoxCallBack);

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptionsCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		SetExitScreen(OPTIONS_SCREEN);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

// ary-05/05/2009 : need more option screen toggles : Functions for button callbacks
void BtnOptPrevCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		ExitScreen();

		OptionsList_Column_Offset--;
		if (OptionsList_Column_Offset < 0) OptionsList_Column_Offset = 0;

		Init();	// ary-05/05/2009 : This is important to refresh the screen properly, it stores a new SAVE_BUFFER

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptNextCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		ExitScreen();

		OptionsList_Column_Offset++;
		if (OptionsList_Column_Offset >= Max_Number_Of_Pages) // ary-05/05/2009 : Max_Number_Of_Pages
			OptionsList_Column_Offset = (Max_Number_Of_Pages - 1);

		Init();	// ary-05/05/2009 : This is important to refresh the screen properly, it stores a new SAVE_BUFFER

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


void BtnDoneCallback(GUI_BUTTON* btn, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);

		SetExitScreen(guiPreviousScreen);

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON);
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY,
			btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnOptionsTogglesCallback(GUI_BUTTON* btn, INT32 reason)
{
	UINT8	Button_UserData_0 = (UINT8)MSYS_GetBtnUserData(btn, 0);// the toggle box
	UINT8	Button_UserData_1 = (UINT8)MSYS_GetBtnUserData(btn, 1);// the option

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			HandleOptionToggle(Button_UserData_0, Button_UserData_1, TRUE, FALSE, FALSE);
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			HandleOptionToggle(Button_UserData_0, Button_UserData_1, FALSE, FALSE, FALSE);
		}
	}
	else if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			HandleOptionToggle(Button_UserData_0, Button_UserData_1, TRUE, TRUE, FALSE);
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			HandleOptionToggle(Button_UserData_0, Button_UserData_1, FALSE, TRUE, FALSE);
		}
	}
}

void HandleOptionToggle(UINT8 Button_UserData_0, UINT8 Button_UserData_1, BOOLEAN fState, BOOLEAN fDown, BOOLEAN fPlaySound)
{
	//Button_UserData_0 = which toggle box within guiFeaturesToggles[Button_UserData_0]
	//Button_UserData_1 = which option the toggle box represents

	static UINT32	uiOptionToggleSound = NO_SAMPLE;

	if (fState)
	{
		switch (Button_UserData_1)
		{
		// FF_DROP_ALL and FF_DROP_ALL_DAMAGED are mutually exclusive
		case FF_DROP_ALL:
			gGameSettings.fFeatures[FF_DROP_ALL_DAMAGED] = FALSE;
			break;
		case FF_DROP_ALL_DAMAGED:
			gGameSettings.fFeatures[FF_DROP_ALL] = FALSE;
			break;

		// counterattack/aggressive AI mutual exclusive toggles
		case FF_AGGRESSIVE_STRATEGIC_AI:
			gGameSettings.fFeatures[FF_AGGRESSIVE_STRATEGIC_AI_2] = FALSE;
			break;
		case FF_AGGRESSIVE_STRATEGIC_AI_2:
			gGameSettings.fFeatures[FF_AGGRESSIVE_STRATEGIC_AI] = FALSE;
			break;

		// FF_MILITIA_USE_SECTOR_EQUIPMENT and FF_MILITIA_REQUIRE_RESOURCES are mutually exclusive
		case FF_MILITIA_REQUIRE_RESOURCES:
			gGameSettings.fFeatures[FF_MILITIA_USE_SECTOR_EQUIPMENT] = FALSE;
			break;
		case FF_MILITIA_USE_SECTOR_EQUIPMENT:
			gGameSettings.fFeatures[FF_MILITIA_REQUIRE_RESOURCES] = FALSE;
			break;

		// enemy role features
		case FF_ENEMY_ROLE_MEDIC:
		case FF_ENEMY_ROLE_OFFICER:
		case FF_ENEMY_ROLE_GENERAL:
			gGameSettings.fFeatures[FF_ENEMY_ROLES] = TRUE;
			break;
		
		// dynamic opinions and dialogue
		case FF_DYNAMIC_DIALOGUE:
			gGameSettings.fFeatures[FF_DYNAMIC_OPINIONS] = TRUE;
			break;

		// arulco strategic division
		case FF_ASD_HELICOPTERS:
			gGameSettings.fFeatures[FF_ASD] = TRUE;
			break;

		// zombies
		case FF_ZOMBIE_RAIDS:
			gGameSettings.fFeatures[FF_ZOMBIES] = TRUE;
			break;
		
		// rain/lightning
		case FF_ALLOW_LIGHTNING:
			gGameSettings.fFeatures[FF_ALLOW_RAIN] = TRUE;
			break;
		}

		gGameSettings.fFeatures[Button_UserData_1] = TRUE;

		if (fDown)
			DrawCheckBoxButtonOn(guiFeaturesToggles[Button_UserData_0]);
	}
	else
	{
		switch (Button_UserData_1)
		{
		// enemy role features
		case FF_ENEMY_ROLES:
			gGameSettings.fFeatures[FF_ENEMY_ROLE_MEDIC] = FALSE;
			gGameSettings.fFeatures[FF_ENEMY_ROLE_OFFICER] = FALSE;
			gGameSettings.fFeatures[FF_ENEMY_ROLE_GENERAL] = FALSE;
			break;

		// dynamic opinions and dialogue
		case FF_DYNAMIC_OPINIONS:
			gGameSettings.fFeatures[FF_DYNAMIC_DIALOGUE] = FALSE;
			break;

		// arulco strategic division
		case FF_ASD:
			gGameSettings.fFeatures[FF_ASD_HELICOPTERS] = FALSE;
			break;


		// zombies
		case FF_ZOMBIES:
			gGameSettings.fFeatures[FF_ZOMBIE_RAIDS] = FALSE;
			break;

		// rain/lightning
		case FF_ALLOW_RAIN:
			gGameSettings.fFeatures[FF_ALLOW_LIGHTNING] = FALSE;
			break;
		}

		gGameSettings.fFeatures[Button_UserData_1] = FALSE;

		if (fDown)
			DrawCheckBoxButtonOff(guiFeaturesToggles[Button_UserData_0]);
	}

	//stop the sound if
	SoundStop(uiOptionToggleSound);


	if (fPlaySound)
	{
		if (fDown)
		{
			PlayJA2Sample(BIG_SWITCH3_IN, RATE_11025, BTNVOLUME, 1, MIDDLEPAN);
		}
		else
		{
			PlayJA2Sample(BIG_SWITCH3_OUT, RATE_11025, BTNVOLUME, 1, MIDDLEPAN);
		}
	}

	InvalidateRegion(OPTIONS__TOP_LEFT_X, OPTIONS__TOP_LEFT_Y, OPTIONS__BOTTOM_RIGHT_X, OPTIONS__BOTTOM_RIGHT_Y);

}

BOOLEAN DoMessageBoxWithRect(UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT32 usFlags,
	MSGBOX_CALLBACK ReturnCallback, SGPRect* pCenteringRect)
{
	// reset exit mode
	gfExitFeaturesDueToMessageBox = TRUE;

	// do message box and return
	giFeaturesMessageBox = DoMessageBox(ubStyle, zString, uiExitScreen, (usFlags | MSG_BOX_FLAG_USE_CENTERING_RECT),
		ReturnCallback, pCenteringRect);

	// send back return state
	return((giFeaturesMessageBox != -1));
}

BOOLEAN DoMessageBox(UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback)
{
	SGPRect CenteringRect = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	// reset exit mode
	gfExitFeaturesDueToMessageBox = TRUE;

	// do message box and return
	giFeaturesMessageBox = DoMessageBox(ubStyle, zString, uiExitScreen, (UINT16)(usFlags | MSG_BOX_FLAG_USE_CENTERING_RECT),
		ReturnCallback, &CenteringRect);

	// send back return state
	return((giFeaturesMessageBox != -1));
}

void ConfirmQuitToMainMenuMessageBoxCallBack(UINT8 bExitValue)
{
	// yes, Quit to main menu
	if (bExitValue == MSG_BOX_RETURN_YES)
	{
		gfEnteredFromMapScreen = FALSE;
		gfExitFeaturesAfterMessageBox = TRUE;
		SetExitScreen(MAINMENU_SCREEN);

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
		gfExitFeaturesAfterMessageBox = FALSE;
		gfExitFeaturesDueToMessageBox = FALSE;
	}
}

void SetToggleBoxes()
{
	UINT8	counter;

	for (counter = 0; counter < MAX_NUMBER_OF_OPTION_TOGGLES; counter++)
	{
		if (toggle_box_array[counter] == -1)
		{
			continue;
		}//there are no options to set for this toggle_box
		if (toggle_box_feature_rules[toggle_box_array[counter]] == 2)
		{
			continue;
		}//there are no options to set for this toggle_box

		if (gGameSettings.fFeatures[toggle_box_array[counter]])
		{
			if (ButtonList[guiFeaturesToggles[counter]])
			{
				ButtonList[guiFeaturesToggles[counter]]->uiFlags |= BUTTON_CLICKED_ON;
			}
		}
		else
		{
			if (ButtonList[guiFeaturesToggles[counter]])
			{
				ButtonList[guiFeaturesToggles[counter]]->uiFlags &= (~BUTTON_CLICKED_ON);
			}
		}
	}
}

void SelectedOptionTextRegionCallBack(MOUSE_REGION* pRegion, INT32 iReason)
{
	//Region_UserData_0 = which toggle box within guiFeaturesToggles[Button_UserData_0]
	//Region_UserData_1 = which option the toggle box represents

	UINT8	Region_UserData_0 = (UINT8)MSYS_GetRegionUserData(pRegion, 0);
	UINT8	Region_UserData_1 = (UINT8)MSYS_GetRegionUserData(pRegion, 1);

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{


		HandleOptionToggle(Region_UserData_0, Region_UserData_1, (BOOLEAN)(!gGameSettings.fFeatures[Region_UserData_1]), FALSE, TRUE);

		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY,
			pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}


	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)//iReason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||
	{
		if (gGameSettings.fFeatures[Region_UserData_1])
		{
			HandleOptionToggle(Region_UserData_0, Region_UserData_1, TRUE, TRUE, TRUE);
		}
		else
		{
			HandleOptionToggle(Region_UserData_0, Region_UserData_1, FALSE, TRUE, TRUE);
		}
	}

}

void SelectedOptionTextRegionMovementCallBack(MOUSE_REGION* pRegion, INT32 reason)
{
	INT8	Region_UserData_0 = (INT8)MSYS_GetRegionUserData(pRegion, 0);

	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		HandleHighLightedText(FALSE);
		gbHighLightedOptionText = -1;
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY,
			pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if (reason & MSYS_CALLBACK_REASON_GAIN_MOUSE)
	{
		gbHighLightedOptionText = Region_UserData_0;
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY,
			pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
}

void HandleHighLightedText(BOOLEAN fHighLight)
{
	UINT16		usPosX = 0;
	UINT16		usPosY = 0;
	UINT8		counter;
	INT8		bHighLight = -1;
	UINT16		usWidth;

	static	INT8	bLastRegion = -1;

	if (!Buttons_Exist_State)
	{
		// there are no buttons/text to highlight, time to leave before any attempt to lookup non-existing pointer data
		gbHighLightedOptionText = -1;
		return;
	}

	if (gbHighLightedOptionText == -1)
		fHighLight = FALSE;

	//if the user has the mouse in one of the checkboxes 
	for (counter = 0; counter < MAX_NUMBER_OF_OPTION_TOGGLES; counter++)
	{
		if (toggle_box_array[counter] == -1)
		{
			continue;
		}//there are no options to set for this toggle_box
		if (toggle_box_feature_rules[toggle_box_array[counter]] == 2)
		{
			continue;
		}//there are no options to set for this toggle_box

		if (ButtonList[guiFeaturesToggles[counter]]->Area.uiFlags & MSYS_MOUSE_IN_AREA)
		{
			gbHighLightedOptionText = counter;
			fHighLight = TRUE;
		}
	}


	// If there is a valid section being highlighted
	if (gbHighLightedOptionText != -1)
	{
		bLastRegion = gbHighLightedOptionText;
	}

	bHighLight = gbHighLightedOptionText;


	if (bLastRegion != -1 && gbHighLightedOptionText == -1)
	{
		fHighLight = FALSE;
		bHighLight = bLastRegion;
		bLastRegion = -1;
	}

	if (bHighLight != -1)
	{
		if (bHighLight < OPT_FIRST_COLUMN_TOGGLE_CUT_OFF)
		{
			usPosX = OPT_TOGGLE_BOX_FIRST_COL_TEXT_X;
			usPosY = OPT_TOGGLE_BOX_FIRST_COLUMN_START_Y + OPT_TOGGLE_TEXT_OFFSET_Y +
				(bHighLight * OPT_GAP_BETWEEN_TOGGLE_BOXES);
		}
		else
		{
			usPosX = OPT_TOGGLE_BOX_SECOND_TEXT_X;
			usPosY = OPT_TOGGLE_BOX_SECOND_COLUMN_START_Y + OPT_TOGGLE_TEXT_OFFSET_Y +
				((bHighLight - OPT_FIRST_COLUMN_TOGGLE_CUT_OFF) * OPT_GAP_BETWEEN_TOGGLE_BOXES);
		}

		usWidth = StringPixLength(z113FeaturesToggleText[toggle_box_array[bHighLight]], OPT_MAIN_FONT);

		//if the string is going to wrap, move the string up a bit
		if (usWidth > OPT_TOGGLE_BOX_TEXT_WIDTH)
		{
			if (fHighLight)
				DisplayWrappedString(usPosX, usPosY, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_HIGHLIGHT_COLOR,
					z113FeaturesToggleText[toggle_box_array[bHighLight]], FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED);
			else
				DisplayWrappedString(usPosX, usPosY, OPT_TOGGLE_BOX_TEXT_WIDTH, 2, OPT_MAIN_FONT, OPT_MAIN_COLOR,
					z113FeaturesToggleText[toggle_box_array[bHighLight]], FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED);
		}
		else
		{
			if (fHighLight)
				DrawTextToScreen(z113FeaturesToggleText[toggle_box_array[bHighLight]], usPosX, usPosY, 0,
					OPT_MAIN_FONT, OPT_HIGHLIGHT_COLOR, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED);
			else
				DrawTextToScreen(z113FeaturesToggleText[toggle_box_array[bHighLight]], usPosX, usPosY, 0,
					OPT_MAIN_FONT, OPT_MAIN_COLOR, FONT_MCOLOR_BLACK, TRUE, LEFT_JUSTIFIED);
		}
	}
}

void SelectedToggleBoxAreaRegionMovementCallBack(MOUSE_REGION* pRegion, INT32 reason)
{
	if (reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
	}
	else if (reason & MSYS_CALLBACK_REASON_GAIN_MOUSE)
	{
		UINT8	counter;

		//loop through all the toggle box's and remove the in area flag
		for (counter = 0; counter < MAX_NUMBER_OF_OPTION_TOGGLES; counter++)
		{
			if (toggle_box_array[counter] == -1)
			{
				continue;
			}//there are no options to set for this toggle_box
			if (toggle_box_feature_rules[toggle_box_array[counter]] == 2)
			{
				continue;
			}//there are no options to set for this toggle_box

			ButtonList[guiFeaturesToggles[counter]]->Area.uiFlags &= ~MSYS_MOUSE_IN_AREA;
		}

		gbHighLightedOptionText = -1;

		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY,
			pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
}

void DrawLeftPanel()
{
	UINT16 uiPosX, uiPosY;

	// need to cover a 180x327 pixel background
	uiPosX = OPT_SOUND_FX_TEXT_X - 4;
	uiPosY = OPT_SOUND_FX_TEXT_Y - 10;
	ColorFillVideoSurfaceArea(FRAME_BUFFER, uiPosX, uiPosY, uiPosX + 180, uiPosY + 327, Get16BPPColor(FROMRGB(0, 0, 0)));

	// draw the text
	SetFontShadow(FONT_MCOLOR_BLACK);
	DisplayWrappedString( iScreenWidthOffset + 36, iScreenHeightOffset + 80, 176, 0, FONT14ARIAL, FONT_MCOLOR_WHITE, z113FeaturesScreenText[ FST_TITLE ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DisplayWrappedString( iScreenWidthOffset + 36, iScreenHeightOffset + 95, 176, 0, FONT10ARIAL, FONT_MCOLOR_LTGRAY, z113FeaturesScreenText[ FST_DESCRIPTION ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	CHAR16 text[1000];
	swprintf(text, L"%s", gbHighLightedOptionText != -1 ? z113FeaturesPanelText[toggle_box_array[gbHighLightedOptionText]] : z113FeaturesScreenText[FST_HOVER_FOR_MORE_INFO]);
	DisplayWrappedString( iScreenWidthOffset + 36, iScreenHeightOffset + 125, 176, 0, FONT12ARIAL, FONT_MCOLOR_WHITE, text, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	SetFontShadow(DEFAULT_SHADOW);
}

void SetPreviousScreen(UINT32 screenId, BOOLEAN setOptionsToo)
{
	guiPreviousScreen = screenId;

	if (setOptionsToo)
		SetOptionsPreviousScreen(screenId, FALSE);
}

} // namespace
