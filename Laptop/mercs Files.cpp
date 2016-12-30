// WANNE 3 <changed some lines>
#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "mercs Files.h"
	#include "mercs.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "Soldier Profile.h"
	#include "stdio.h"
	#include "Encrypted File.h"
	#include "AimMembers.h"
	#include "InterFace Items.h"
	#include "Game Clock.h"
	#include "Soldier Add.h"
	#include "OverHead.h"
	#include "Game Clock.h"
	#include "message.h"
	#include "Font.h"
	#include "LaptopSave.h"
	#include "Text.h"
	#include "Speck Quotes.h"
	#include "Multi Language Graphic Utils.h"
	#include "Quests.h"
	#include "Assignments.h"
	#include "Input.h"
	#include "english.h"
	#include "finances.h"
	#include "GameSettings.h"
	#include "Personnel.h"
	#include "Encyclopedia_new.h"	//update encyclopedia item visibility when viewing that item
#endif

#include "Cheats.h"
#include "connect.h"
#include "fresh_header.h"
#include "aim.h"

#define		MERCBIOFILE												"BINARYDATA\\MercBios.edt"

#define		MERC_BIO_FONT											FONT14ARIAL//FONT12ARIAL
#define		MERC_BIO_COLOR										FONT_MCOLOR_WHITE

#define		MERC_TITLE_FONT										FONT14ARIAL
#define		MERC_TITLE_COLOR									FONT_YELLOW

#define		MERC_NAME_FONT										FONT14ARIAL
#define		MERC_NAME_COLOR										FONT_MCOLOR_WHITE

#define		MERC_STATS_FONT										FONT12ARIAL
#define		MERC_STATIC_STATS_COLOR						FONT_YELLOW
#define		MERC_DYNAMIC_STATS_COLOR					FONT_MCOLOR_WHITE
#define		HIGH_STATS_COLOR							FONT_MCOLOR_WHITE
#define		MED_STATS_COLOR								FONT_MCOLOR_DKWHITE
#define		LOW_STATS_COLOR								FONT_MCOLOR_DKGRAY

#define		MERC_FILES_PORTRAIT_BOX_X					LAPTOP_SCREEN_UL_X + 16
#define		MERC_FILES_PORTRAIT_BOX_Y					LAPTOP_SCREEN_WEB_UL_Y + 17

#define		MERC_FACE_X												MERC_FILES_PORTRAIT_BOX_X + 2
#define		MERC_FACE_Y												MERC_FILES_PORTRAIT_BOX_Y + 2
#define		MERC_FACE_WIDTH										106
#define		MERC_FACE_HEIGHT									122

#define		MERC_FILES_STATS_BOX_X						LAPTOP_SCREEN_UL_X + 144
#define		MERC_FILES_STATS_BOX_Y						MERC_FILES_PORTRAIT_BOX_Y


#define		MERC_FILES_BIO_BOX_X							MERC_FILES_PORTRAIT_BOX_X
#define		MERC_FILES_BIO_BOX_Y							LAPTOP_SCREEN_WEB_UL_Y + 155

// OK
//JMich_MMG: Moving the buttons around a bit, and adding a new one
#define		MERC_FILES_PREV_BUTTON_X					MERC_FILES_PORTRAIT_BOX_X + 2
#define		MERC_FILES_PREV_BUTTON_Y					iScreenHeightOffset + 380

#define		MERC_FILES_HIRE_BUTTON_X					MERC_FILES_PREV_BUTTON_X + 92
#define		MERC_FILES_HIRE_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y

#define		MERC_FILES_BIO_INV_BUTTON_X					MERC_FILES_HIRE_BUTTON_X + 92
#define		MERC_FILES_BIO_INV_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y

#define		MERC_FILES_BACK_BUTTON_X					MERC_FILES_BIO_INV_BUTTON_X + 92
#define		MERC_FILES_BACK_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y

#define		MERC_FILES_NEXT_BUTTON_X					MERC_FILES_BACK_BUTTON_X + 92
#define		MERC_FILES_NEXT_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y

/*JMich_MMG: Original button positions
#define		MERC_FILES_PREV_BUTTON_X					iScreenWidthOffset + 128
#define		MERC_FILES_PREV_BUTTON_Y					iScreenHeightOffset + 380

#define		MERC_FILES_HIRE_BUTTON_X					MERC_FILES_PREV_BUTTON_X + 103 + 16		//iScreenWidthOffset + 260
#define		MERC_FILES_HIRE_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y

#define		MERC_FILES_BACK_BUTTON_X					MERC_FILES_HIRE_BUTTON_X + 103 + 16		//iScreenWidthOffset + 380
#define		MERC_FILES_BACK_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y

// OK
#define		MERC_FILES_NEXT_BUTTON_X					iScreenWidthOffset + 485			//490
#define		MERC_FILES_NEXT_BUTTON_Y					MERC_FILES_PREV_BUTTON_Y
*/

#define		MERC_PAGE_X									iScreenWidthOffset + 570
#define		MERC_PAGE_Y									iScreenHeightOffset + 425

#define		MERC_NAME_X												MERC_FILES_STATS_BOX_X + 50
#define		MERC_NAME_Y												MERC_FILES_STATS_BOX_Y + 10

#define		MERC_BIO_TEXT_X										MERC_FILES_BIO_BOX_X + 5
#define		MERC_BIO_TEXT_Y										MERC_FILES_BIO_BOX_Y + 10

#define		MERC_ADD_BIO_TITLE_X							MERC_BIO_TEXT_X
#define		MERC_ADD_BIO_TITLE_Y							MERC_BIO_TEXT_Y + 100

#define		MERC_ADD_BIO_TEXT_X								MERC_BIO_TEXT_X
#define		MERC_ADD_BIO_TEXT_Y								MERC_ADD_BIO_TITLE_Y + 15

#define		MERC_BIO_WIDTH										460 - 10

#define		MERC_BIO_INFO_TEXT_SIZE						5 * 80 * 2
#define		MERC_BIO_ADD_INFO_TEXT_SIZE				2 * 80 * 2
#define		MERC_BIO_SIZE											7 * 80 * 2

#define		MERC_STATS_FIRST_COL_X						MERC_NAME_X
#define		MERC_STATS_FIRST_NUM_COL_X				MERC_STATS_FIRST_COL_X + 115
#define		MERC_STATS_SECOND_COL_X						MERC_FILES_STATS_BOX_X + 190
#define		MERC_STATS_SECOND_NUM_COL_X				MERC_STATS_SECOND_COL_X + 115
#define		MERC_SPACE_BN_LINES								15

#define		MERC_HEALTH_Y											MERC_FILES_STATS_BOX_Y + 30

#define		MERC_PORTRAIT_TEXT_OFFSET_Y				107

//JMich_MMG: Defines for the inventory boxes
#define		MERC_MEMBER_WEAPON_NAME_X			MERC_WEAPONBOX_X
#define		MERC_MEMBER_WEAPON_NAME_Y			MERC_WEAPONBOX_Y + MERC_WEAPONBOX_SIZE_Y + 1
#define		MERC_MEMBER_WEAPON_NAME_WIDTH		MERC_WEAPONBOX_SIZE_X - 2
#define		MERC_WEAPONBOX_X					MERC_FILES_PORTRAIT_BOX_X + 3
#define		MERC_WEAPONBOX_Y					MERC_FILES_BIO_BOX_Y + 10
#define		MERC_WEAPONBOX_SIZE_X				61
#define		MERC_WEAPONBOX_SIZE_Y				31
#define		MERC_WEAPONBOX_NUMBER				8
#define		MERC_WEAPONBOX_X_NSGI				MERC_FILES_PORTRAIT_BOX_X + 6
#define		MERC_WEAPONBOX_Y_NSGI				MERC_FILES_BIO_BOX_Y + 10
#define		MERC_WEAPONBOX_SIZE_X_NSGI			64
#define		MERC_WEAPONBOX_SIZE_Y_NSGI			30
#define		MERC_WEAPONBOX_COLUMNS				7
#define		MERC_WEAPONBOX_ROWS					3
#define		MERC_WEAPONBOX_TOTAL_ITEMS			MERC_WEAPONBOX_COLUMNS * MERC_WEAPONBOX_ROWS

#define		MERC_WEAPONBOX_BUTTON_START_WIDTH	75
#define		MERC_WEAPONBOX_BUTTON_START_HEIGHT	30

#define		MERC_WEAPONBOX_LOADOUT_BUTTON_X_DISTANCE		20

#define		MERC_WEAPONBOX_LOADOUT_ONE_X					MERC_FILES_PORTRAIT_BOX_X + 3
#define		MERC_WEAPONBOX_LOADOUT_ONE_Y					iScreenHeightOffset + 330 + LAPTOP_SCREEN_WEB_DELTA_Y
#define		MERC_WEAPONBOX_LOADOUT_ONE_BOX_Y				MERC_WEAPONBOX_LOADOUT_ONE_Y - 4
#define		MERC_WEAPONBOX_LOADOUT_ONE_BR_X				MERC_WEAPONBOX_LOADOUT_ONE_X + MERC_WEAPONBOX_BUTTON_START_WIDTH
#define		MERC_WEAPONBOX_LOADOUT_ONE_BR_Y				MERC_WEAPONBOX_LOADOUT_ONE_BOX_Y + MERC_WEAPONBOX_BUTTON_START_HEIGHT
#define		MERC_WEAPONBOX_LOADOUT_TWO_X					MERC_WEAPONBOX_LOADOUT_ONE_X + MERC_WEAPONBOX_BUTTON_START_WIDTH + MERC_WEAPONBOX_LOADOUT_BUTTON_X_DISTANCE
#define		MERC_WEAPONBOX_LOADOUT_TWO_Y					MERC_WEAPONBOX_LOADOUT_ONE_Y
#define		MERC_WEAPONBOX_LOADOUT_TWO_BOX_Y				MERC_WEAPONBOX_LOADOUT_TWO_Y - 4
#define		MERC_WEAPONBOX_LOADOUT_TWO_BR_X				MERC_WEAPONBOX_LOADOUT_TWO_X + MERC_WEAPONBOX_BUTTON_START_WIDTH
#define		MERC_WEAPONBOX_LOADOUT_TWO_BR_Y				MERC_WEAPONBOX_LOADOUT_TWO_BOX_Y + MERC_WEAPONBOX_BUTTON_START_HEIGHT
#define		MERC_WEAPONBOX_LOADOUT_THREE_X				MERC_WEAPONBOX_LOADOUT_TWO_X + MERC_WEAPONBOX_BUTTON_START_WIDTH + MERC_WEAPONBOX_LOADOUT_BUTTON_X_DISTANCE
#define		MERC_WEAPONBOX_LOADOUT_THREE_Y				MERC_WEAPONBOX_LOADOUT_TWO_Y
#define		MERC_WEAPONBOX_LOADOUT_THREE_BOX_Y			MERC_WEAPONBOX_LOADOUT_THREE_Y - 4
#define		MERC_WEAPONBOX_LOADOUT_THREE_BR_X			MERC_WEAPONBOX_LOADOUT_THREE_X + MERC_WEAPONBOX_BUTTON_START_WIDTH
#define		MERC_WEAPONBOX_LOADOUT_THREE_BR_Y			MERC_WEAPONBOX_LOADOUT_THREE_BOX_Y + MERC_WEAPONBOX_BUTTON_START_HEIGHT
#define		MERC_WEAPONBOX_LOADOUT_FOUR_X				MERC_WEAPONBOX_LOADOUT_THREE_X + MERC_WEAPONBOX_BUTTON_START_WIDTH + MERC_WEAPONBOX_LOADOUT_BUTTON_X_DISTANCE
#define		MERC_WEAPONBOX_LOADOUT_FOUR_Y				MERC_WEAPONBOX_LOADOUT_THREE_Y
#define		MERC_WEAPONBOX_LOADOUT_FOUR_BOX_Y			MERC_WEAPONBOX_LOADOUT_FOUR_Y - 4
#define		MERC_WEAPONBOX_LOADOUT_FOUR_BR_X				MERC_WEAPONBOX_LOADOUT_FOUR_X + MERC_WEAPONBOX_BUTTON_START_WIDTH
#define		MERC_WEAPONBOX_LOADOUT_FOUR_BR_Y				MERC_WEAPONBOX_LOADOUT_FOUR_BOX_Y + MERC_WEAPONBOX_BUTTON_START_HEIGHT
#define		MERC_WEAPONBOX_LOADOUT_FIVE_X				MERC_WEAPONBOX_LOADOUT_FOUR_X + MERC_WEAPONBOX_BUTTON_START_WIDTH + MERC_WEAPONBOX_LOADOUT_BUTTON_X_DISTANCE
#define		MERC_WEAPONBOX_LOADOUT_FIVE_Y				MERC_WEAPONBOX_LOADOUT_FOUR_Y
#define		MERC_WEAPONBOX_LOADOUT_FIVE_BOX_Y			MERC_WEAPONBOX_LOADOUT_FIVE_Y - 4
#define		MERC_WEAPONBOX_LOADOUT_FIVE_BR_X				MERC_WEAPONBOX_LOADOUT_FIVE_X + MERC_WEAPONBOX_BUTTON_START_WIDTH
#define		MERC_WEAPONBOX_LOADOUT_FIVE_BR_Y				MERC_WEAPONBOX_LOADOUT_FIVE_BOX_Y + MERC_WEAPONBOX_BUTTON_START_HEIGHT

//JMich_MMG: defines for the inventory buttons text

#define MERC_M_FONT_DYNAMIC_TEXT					FONT10ARIAL
#define MERC_M_WEAPON_TEXT_FONT						FONT10ARIAL
#define MERC_M_WEAPON_TEXT_COLOR					FONT_MCOLOR_WHITE
#define	MERC_M_KIT_BUTTON_FONT						FONT12ARIAL
#define	MERC_M_KIT_BUTTON_UP_COLOR					FONT_MCOLOR_WHITE
#define	MERC_M_KIT_BUTTON_DOWN_COLOR				138


UINT32	guiPortraitBox;
UINT32	guiStatsBox;
UINT32	guiBioBox;
UINT32	guiMercFace;
UINT32		lguiWeaponBox;

//JMich_MMG: Image for the inventory boxes.
UINT32	guiMercWeaponBox;
UINT32	guiMercWeaponKitButtonImage;
void EnableMercWeaponKitSelectionButtons();
void CreateMercWeaponBoxBackground();

//JMich_MMG: Adding a global variable for which page to display at the MERC files page
UINT8			gubCurMercFilesTogglePage;
//JMich_MMG: Array to hold the gearkit costs, to use when displaying the gearkit price buttons
UINT16			gubCurMercGearkitPrice[NUM_MERCSTARTINGGEAR_KITS];
enum
{
	MERC_FILES_BIO_PAGE,
	MERC_FILES_INV_PAGE,
	NUM_MERC_PAGES
};

// flag for buying gear when hiring
BOOLEAN fMercBuyEquipment;

extern BOOLEAN fMercHireOverPlayerLimitMerc;
//
// Buttons
//

// The Prev button
void BtnMercPrevButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiPrevButton;
INT32		guiButtonImage;

// The Next button
void BtnMercNextButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiNextButton;

// The Hire button
void BtnMercHireButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiHireButton;

// The Back button
void BtnMercFilesBackButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMercBackButton;

// Message box to buy gear
void MercHireButtonGearYesNoCallback (UINT8 bExitValue);

void MercProcessHireAfterGear();

//JMich_MMG: The BIO/INV toggle button
void BtnMercFilesBioInvToggleButtonCallback(GUI_BUTTON *btn, INT32 reason);
UINT32 guiBioInvButton;

//JMich_MMG: Mouseregion for item tooltips
MOUSE_REGION	gMercWeaponboxFasthelpRegion[MERC_WEAPONBOX_TOTAL_ITEMS];
extern void GetHelpTextForItemInLaptop( STR16 pzStr, UINT16 usItemNumber );
void CreateMercWeaponBoxMouseRegions();
extern void HelpTextDoneCallback( void );

//JMich_MMG: Merc Kit Selection Buttons
UINT32 guiMercWeaponBoxSelectionButton[NUM_MERCSTARTINGGEAR_KITS];
void BtnMercWeaponboxSelectButtonCallback( GUI_BUTTON * btn, INT32 Reason);

//Mouse Regions
//Clicking on guys Face
MOUSE_REGION	gMercSelectedFaceRegion;
void SelectMercsFaceRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//****************************
//
//	Function Prototypes
//
//****************************

BOOLEAN DisplayMercFace( UINT8 ubMercID );
void DisplayMercMemberClickOnFaceHelpText( UINT8 ubMercID );
void LoadAndDisplayMercBio( UINT8 ubMercID );
void DisplayMercsStats( UINT8 ubMercID );
BOOLEAN MercFilesHireMerc(UINT8 ubMercID);
void EnableDisableMercFilesNextPreviousButton( );

UINT8	GetStatsColor( INT8 bStat );

//Hotkey Assignment
void HandleMercsFilesKeyBoardInput( );

//JMich_MMG: Declaration of the display inventory function
BOOLEAN DisplayMERCMercsInventory(UINT8 ubMercID);
void CreateMercKitSelectionButtons();
void DisableMercWeaponKitSelectionButtons();
void RefreshMercWeaponKitSelectionButtons();
void MercWeaponKitSelectionUpdate(UINT8 selectedInventory = 0);
UINT8	gSelectedMercKit;


void GameInitMercsFiles()
{

}

BOOLEAN EnterMercsFiles()
{
	VOBJECT_DESC	VObjectDesc;

	InitMercBackGround();

	//JMich_MMG: Initialize the page to be used
	gubCurMercFilesTogglePage = MERC_FILES_BIO_PAGE;

	// load the stats box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_STATSBOX );
	CHECKF(AddVideoObject(&VObjectDesc, &guiStatsBox));

	// load the Portrait box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\PortraitBox.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiPortraitBox));

	// load the bio box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\BioBox.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiBioBox));

	// load the WeaponBox graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\weaponbox.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &lguiWeaponBox));

	// load the WeaponBox graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\mercbox.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMercWeaponBox));

	//** Mouse Regions **
	// mouse region for skills/traits tooltip on merc portrait
	MSYS_DefineRegion( &gMercSelectedFaceRegion, MERC_FACE_X, MERC_FACE_Y , MERC_FACE_X + MERC_FACE_WIDTH , MERC_FACE_Y + MERC_FACE_HEIGHT, MSYS_PRIORITY_HIGH,
						CURSOR_WWW, MSYS_NO_CALLBACK, SelectMercsFaceRegionCallBack );
	MSYS_AddRegion(&gMercSelectedFaceRegion);

	// Prev Box button
	guiButtonImage	= LoadButtonImage("LAPTOP\\mercmediumbuttons.sti", -1,0,-1,1,-1 );
	// Weapon Kit Button
	guiMercWeaponKitButtonImage = LoadButtonImage("LAPTOP\\MERCGEARKITBUTTON.sti", -1,0,-1,1,-1 );

	guiPrevButton = CreateIconAndTextButton( guiButtonImage, MercInfo[MERC_FILES_PREVIOUS],
													FONT12ARIAL,
													MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW,
													MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MERC_FILES_PREV_BUTTON_X, MERC_FILES_PREV_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMercPrevButtonCallback);

	SetButtonCursor(guiPrevButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiPrevButton, DISABLED_STYLE_SHADED);

	//Next Button
	guiNextButton = CreateIconAndTextButton( guiButtonImage, MercInfo[MERC_FILES_NEXT],
													FONT12ARIAL,
													MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW,
													MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MERC_FILES_NEXT_BUTTON_X, MERC_FILES_NEXT_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMercNextButtonCallback);

	SetButtonCursor(guiNextButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiNextButton, DISABLED_STYLE_SHADED);

	//Hire button
	guiHireButton = CreateIconAndTextButton( guiButtonImage, MercInfo[MERC_FILES_HIRE],
													FONT12ARIAL,
													MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW,
													MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MERC_FILES_HIRE_BUTTON_X, MERC_FILES_HIRE_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMercHireButtonCallback);
	SetButtonCursor(guiHireButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiHireButton, DISABLED_STYLE_SHADED);

	//Back button
	guiMercBackButton = CreateIconAndTextButton( guiButtonImage, MercInfo[MERC_FILES_HOME],
													FONT12ARIAL,
													MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW,
													MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MERC_FILES_BACK_BUTTON_X, MERC_FILES_BACK_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMercFilesBackButtonCallback);
	SetButtonCursor(guiMercBackButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiMercBackButton, DISABLED_STYLE_SHADED);
	
	//JMich_MMG: Bio/Inv button
	if (gubCurMercFilesTogglePage + 1 == NUM_MERC_PAGES)
	{
			guiBioInvButton = CreateIconAndTextButton( guiButtonImage, MercInfo[MERC_FILES_BIO],
													FONT12ARIAL,
													MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW,
													MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MERC_FILES_BIO_INV_BUTTON_X, MERC_FILES_BIO_INV_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMercFilesBioInvToggleButtonCallback);
	SetButtonCursor(guiBioInvButton, CURSOR_LAPTOP_SCREEN);
	}
	else
	{
			guiBioInvButton = CreateIconAndTextButton( guiButtonImage, MercInfo[MERC_FILES_BIO + gubCurMercFilesTogglePage + 1],
													FONT12ARIAL,
													MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW,
													MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MERC_FILES_BIO_INV_BUTTON_X, MERC_FILES_BIO_INV_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMercFilesBioInvToggleButtonCallback);
	SetButtonCursor(guiBioInvButton, CURSOR_LAPTOP_SCREEN);
	}
	SpecifyDisabledButtonStyle( guiBioInvButton, DISABLED_STYLE_SHADED);
	CreateMercKitSelectionButtons();
	CreateMercWeaponBoxMouseRegions();
	DisableMercWeaponKitSelectionButtons();
//	RenderMercsFiles();
	return( TRUE );
}

void SelectMercsFaceRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
			guiCurrentLaptopMode = LAPTOP_MODE_MERC;
			gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		//if the players account is suspended, go back to the main screen and have Speck inform the players
		if( LaptopSaveInfo.gubPlayersMercAccountStatus == MERC_ACCOUNT_SUSPENDED )
		{
			guiCurrentLaptopMode = LAPTOP_MODE_MERC;
#ifdef JA2UB
		//	gusMercVideoSpeckSpeech = SPECK_QUOTE_ALTERNATE_OPENING_5_PLAYER_OWES_SPECK_ACCOUNT_SUSPENDED;
#else
			gusMercVideoSpeckSpeech = SPECK_QUOTE_ALTERNATE_OPENING_5_PLAYER_OWES_SPECK_ACCOUNT_SUSPENDED;
#endif
			gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;

		}
		//else try to hire the merc
		else
		{
			if( gMercProfiles[ GetAvailableMercIDFromMERCArray( gubCurMercIndex ) ].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS )
			{	
				//bought gear before
				fMercBuyEquipment = 0;
				MercProcessHireAfterGear();
			}
			else
				//prompt to buy gear
				DoLapTopMessageBox( MSG_BOX_BLUE_ON_GREY, MercInfo[ MERC_FILES_BUY_GEAR ], LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, MercHireButtonGearYesNoCallback );
		}
	}
}

void BtnMercFilesBioInvToggleButtonCallback (GUI_BUTTON *btn, INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			if (gubCurMercFilesTogglePage + 1 == NUM_MERC_PAGES)
			{
				gubCurMercFilesTogglePage = 0;
			}
			else
			{
				gubCurMercFilesTogglePage++;
			}
			RenderMercsFiles();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void ExitMercsFiles()
{
	UINT8 i;
	DeleteVideoObjectFromIndex(guiPortraitBox);
	DeleteVideoObjectFromIndex(guiStatsBox);
	DeleteVideoObjectFromIndex(guiBioBox);
	//JMich_MMG: Deleting the image for inventory boxes
	DeleteVideoObjectFromIndex(guiMercWeaponBox);

	MSYS_RemoveRegion( &gMercSelectedFaceRegion);

	UnloadButtonImage( guiButtonImage );
	RemoveButton( guiPrevButton );
	RemoveButton( guiNextButton );
	RemoveButton( guiHireButton );

	RemoveButton( guiMercBackButton );
	DeleteVideoObjectFromIndex(lguiWeaponBox);//hayden

	//JMich_MMG: removing buttons
	RemoveButton (guiBioInvButton );
	for (i = 0; i<NUM_MERCSTARTINGGEAR_KITS; i++)
	{
		RemoveButton( guiMercWeaponBoxSelectionButton[i] );
	}
	for (i = 0; i<MERC_WEAPONBOX_TOTAL_ITEMS;i++)
	{
		MSYS_RemoveRegion( &gMercWeaponboxFasthelpRegion[i] );
	}
	RemoveMercBackGround();
}

void HandleMercsFiles()
{
	HandleMercsFilesKeyBoardInput( );
}

void RenderMercsFiles()
{
	HVOBJECT hPixHandle;
	UINT8 i;

	DrawMecBackGround();

	// Portrait Box
	GetVideoObject(&hPixHandle, guiPortraitBox);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,MERC_FILES_PORTRAIT_BOX_X, MERC_FILES_PORTRAIT_BOX_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Stats Box
	GetVideoObject(&hPixHandle, guiStatsBox);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,MERC_FILES_STATS_BOX_X, MERC_FILES_STATS_BOX_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	
	UINT8 mercID = GetAvailableMercIDFromMERCArray( gubCurMercIndex );

	//JMich_MMG disabling tooltips and inventory buttons
	DisableMercWeaponKitSelectionButtons();
	for (i = 0; i<MERC_WEAPONBOX_TOTAL_ITEMS;i++)
	{
		MSYS_DisableRegion( &gMercWeaponboxFasthelpRegion[i] );
	}
	//JMich_MMG: adding a switch here, to see what we should display
	switch (gubCurMercFilesTogglePage)
	{

	case MERC_FILES_BIO_PAGE:
		// bio box
		GetVideoObject(&hPixHandle, guiBioBox);
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,MERC_FILES_BIO_BOX_X+1, MERC_FILES_BIO_BOX_Y, VO_BLT_SRCTRANSPARENCY,NULL);

		//Load and display the mercs bio


		if (gConditionsForMercAvailabilityTemp[GetAvailableMercIndex(gubCurMercIndex)].MercBio != 255)
		{
// WANNE: @anv: Disabled the alternate BIO for John Kulba, because it is to specific for 1.13 and not very good for modders.
/*
#ifdef JA2UB
			LoadAndDisplayMercBio( gConditionsForMercAvailabilityTemp[gubCurMercIndex].MercBio );
#else
			// anv: handle John Kulba's alternate bio if Mary died during escort
			if( gConditionsForMercAvailabilityTemp[GetAvailableMercIndex(gubCurMercIndex)].ProfilId == JOHN_MERC && gMercProfiles[ MARY ].bMercStatus == MERC_IS_DEAD )
				LoadAndDisplayMercBio( gConditionsForMercAvailabilityTemp[GetAvailableMercIndex(gubCurMercIndex)].MercBio + 1 );
			else
#endif
*/
			LoadAndDisplayMercBio( gConditionsForMercAvailabilityTemp[GetAvailableMercIndex(gubCurMercIndex)].MercBio );

		}
		break;
	case MERC_FILES_INV_PAGE:
		// Inv box
		GetVideoObject(&hPixHandle, guiBioBox);
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,MERC_FILES_BIO_BOX_X+1, MERC_FILES_BIO_BOX_Y, VO_BLT_SRCTRANSPARENCY,NULL);
		CreateMercWeaponBoxBackground();
		DisplayMERCMercsInventory(mercID);
		RefreshMercWeaponKitSelectionButtons();
		EnableMercWeaponKitSelectionButtons();
		break;

	}
	if (gubCurMercFilesTogglePage + 1 == NUM_MERC_PAGES)
	{
		
		SpecifyButtonText(guiBioInvButton, MercInfo[MERC_FILES_BIO]);
	}
	else
	{
		SpecifyButtonText(guiBioInvButton, MercInfo[MERC_FILES_BIO + gubCurMercFilesTogglePage + 1]);
	}
	// WANNE: Removed the background for the items
	/*
	if(is_networked)//hayden - load bgnd
	{
		GetVideoObject(&hPixHandle, lguiWeaponBox);
		UINT16 uiPosX = IMAGE_OFFSET_X + 6;
		for(int x=0; x< 8; x++)
		{
			BltVideoObject(FRAME_BUFFER, hPixHandle, 0, uiPosX, IMAGE_OFFSET_Y + 296 -30, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += 61;
		}
	}
	*/

	UINT8 mercId = GetAvailableMercIDFromMERCArray( gubCurMercIndex );

	//Display the mercs face
	DisplayMercFace( mercId );

	//Display Mercs Name
	DrawTextToScreen(gMercProfiles[ mercId ].zName, MERC_NAME_X, MERC_NAME_Y, 0, MERC_NAME_FONT, MERC_NAME_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	//Render the 'click face' text
	DisplayMercMemberClickOnFaceHelpText( mercId );

	//Display the mercs statistic
	DisplayMercsStats( mercId );

	//check to see if the merc is dead if so disable the contact button
	if( IsMercDead( mercId ) )
	{
		DisableButton( guiHireButton );
	}
	else if( ( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_VALID ) && ( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_SUSPENDED ) && ( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_VALID_FIRST_WARNING ) )
	{
		//if the players account is suspended, disable the button
		DisableButton( guiHireButton );
	}
	else
		EnableButton( guiHireButton );

	//Enable or disable the buttons
	EnableDisableMercFilesNextPreviousButton( );

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}


void BtnMercNextButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			if (gfKeyState [ CTRL ])
				gubCurMercIndex = LaptopSaveInfo.gubLastMercIndex;
			else if (gfKeyState [ SHIFT ])
			{
				if (gubCurMercIndex <= LaptopSaveInfo.gubLastMercIndex - 10)
					gubCurMercIndex += 10;
				else
					gubCurMercIndex = LaptopSaveInfo.gubLastMercIndex;
			}
			else
			{
				if( gubCurMercIndex <= LaptopSaveInfo.gubLastMercIndex-1 )
					gubCurMercIndex++;
			}
			// WANNE: If current profile has an alternate profile, skip the next one, because it is the alternate profile,
			// otherwise we have both profiles available in MERC!
			//if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
			//{
			//	gubCurMercIndex++;
			//}

			fReDrawScreenFlag = TRUE;

			//Enable or disable the buttons
			EnableDisableMercFilesNextPreviousButton( );

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


void BtnMercHireButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			//if the players account is suspended, go back to the main screen and have Speck inform the players
			if( LaptopSaveInfo.gubPlayersMercAccountStatus == MERC_ACCOUNT_SUSPENDED )
			{
				guiCurrentLaptopMode = LAPTOP_MODE_MERC;
#ifdef JA2UB
			//	gusMercVideoSpeckSpeech = SPECK_QUOTE_ALTERNATE_OPENING_5_PLAYER_OWES_SPECK_ACCOUNT_SUSPENDED;
#else
				gusMercVideoSpeckSpeech = SPECK_QUOTE_ALTERNATE_OPENING_5_PLAYER_OWES_SPECK_ACCOUNT_SUSPENDED;
#endif
				gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;

			}
			//else try to hire the merc
			else
			{
				if( gMercProfiles[ GetAvailableMercIDFromMERCArray( gubCurMercIndex ) ].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS )
				{	
					//bought gear before
					fMercBuyEquipment = 0;
					MercProcessHireAfterGear();
				}
				else
					//prompt to buy gear
					DoLapTopMessageBox( MSG_BOX_BLUE_ON_GREY, MercInfo[ MERC_FILES_BUY_GEAR ], LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, MercHireButtonGearYesNoCallback );
			}

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


void BtnMercPrevButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			if (gfKeyState [ CTRL ])
				gubCurMercIndex = 0;
			else if (gfKeyState [ SHIFT ])
			{
				if ( gubCurMercIndex > 10)
					gubCurMercIndex -= 10;
				else
					gubCurMercIndex = 0;
			}
			else
			{
				if( gubCurMercIndex > 0 )
					gubCurMercIndex--;
			}
			// WANNE: If current profile has an alternate profile, skip the previous one, because it is the alternate profile,
			// otherwise we have both profiles available in MERC!
			if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
			{
				gubCurMercIndex--;
			}

			fReDrawScreenFlag = TRUE;

			//Enable or disable the buttons
			EnableDisableMercFilesNextPreviousButton( );

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


BOOLEAN DisplayMercFace( UINT8 ubMercID)
{
	HVOBJECT hFaceHandle;
	HVOBJECT hPortraitHandle;
	STR							sFaceLoc = "FACES\\BIGFACES\\";
	char						sTemp[100];
	MERCPROFILESTRUCT	*pMerc;
	VOBJECT_DESC	VObjectDesc;
	SOLDIERTYPE			*pSoldier=NULL;

	// Portrait Frame
	GetVideoObject(&hPortraitHandle, guiPortraitBox);
	BltVideoObject(FRAME_BUFFER, hPortraitHandle, 0,MERC_FILES_PORTRAIT_BOX_X, MERC_FILES_PORTRAIT_BOX_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	pMerc = &gMercProfiles[ ubMercID ];

	//See if the merc is currently hired
	pSoldier = FindSoldierByProfileID( ubMercID, TRUE );

	// load the Face graphic and add it
	if (gGameExternalOptions.fReadProfileDataFromXML)
	{
		// HEADROCK PROFEX: Do not read direct profile number, instead, look inside the profile for a ubFaceIndex value.
		sprintf(sTemp, "%s%02d.sti", sFaceLoc, gMercProfiles[ubMercID].ubFaceIndex);
	}
	else
	{
		sprintf(sTemp, "%s%02d.sti", sFaceLoc, ubMercID);
	}
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP(sTemp, VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMercFace));

	//Blt face to screen
	GetVideoObject(&hFaceHandle, guiMercFace);
	BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,MERC_FACE_X, MERC_FACE_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//if the merc is dead, shadow the face red and put text over top saying the merc is dead
	if( IsMercDead( ubMercID ) )
	{
		//shade the face red, (to signif that he is dead)
		hFaceHandle->pShades[ 0 ]		= Create16BPPPaletteShaded( hFaceHandle->pPaletteEntry, DEAD_MERC_COLOR_RED, DEAD_MERC_COLOR_GREEN, DEAD_MERC_COLOR_BLUE, TRUE );

		//get the face object
		GetVideoObject(&hFaceHandle, guiMercFace);

		//set the red pallete to the face
		SetObjectHandleShade( guiMercFace, 0 );

		//Blt face to screen
	BltVideoObject(FRAME_BUFFER, hFaceHandle, 0,MERC_FACE_X, MERC_FACE_Y, VO_BLT_SRCTRANSPARENCY,NULL);

		DisplayWrappedString(MERC_FACE_X, MERC_FACE_Y+MERC_PORTRAIT_TEXT_OFFSET_Y, MERC_FACE_WIDTH, 2, FONT14ARIAL, 145, MercInfo[MERC_FILES_MERC_IS_DEAD], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	else if( ubMercID == FLO && gubFact[ FACT_PC_MARRYING_DARYL_IS_FLO ] )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, MERC_FACE_X, MERC_FACE_Y, MERC_FACE_X + MERC_FACE_WIDTH, MERC_FACE_Y + MERC_FACE_HEIGHT);
		DisplayWrappedString( MERC_FACE_X, MERC_FACE_Y+MERC_PORTRAIT_TEXT_OFFSET_Y, MERC_FACE_WIDTH, 2, FONT14ARIAL, 145, pPersonnelDepartedStateStrings[3], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}

	//else if the merc is currently a POW or, the merc was fired as a pow
	else if( pMerc->bMercStatus == MERC_FIRED_AS_A_POW || ( pSoldier &&	pSoldier->bAssignment == ASSIGNMENT_POW ) )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, MERC_FACE_X, MERC_FACE_Y, MERC_FACE_X + MERC_FACE_WIDTH, MERC_FACE_Y + MERC_FACE_HEIGHT);
		DisplayWrappedString(MERC_FACE_X, MERC_FACE_Y+MERC_PORTRAIT_TEXT_OFFSET_Y, MERC_FACE_WIDTH, 2, FONT14ARIAL, 145, pPOWStrings[0], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	//if the merc is hired already, say it
	else if( !IsMercHireable( ubMercID ) && pMerc->bMercStatus == MERC_HIRED_BUT_NOT_ARRIVED_YET || pMerc->bMercStatus > 0 )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, MERC_FACE_X, MERC_FACE_Y, MERC_FACE_X + MERC_FACE_WIDTH, MERC_FACE_Y + MERC_FACE_HEIGHT);
		DisplayWrappedString(MERC_FACE_X, MERC_FACE_Y+MERC_PORTRAIT_TEXT_OFFSET_Y, MERC_FACE_WIDTH, 2, FONT14ARIAL, 145, MercInfo[MERC_FILES_ALREADY_HIRED], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	//if the merc is away on another assignemnt, say the merc is unavailable
	else if( !IsMercHireable( ubMercID ) )
	{
		ShadowVideoSurfaceRect( FRAME_BUFFER, MERC_FACE_X, MERC_FACE_Y, MERC_FACE_X + MERC_FACE_WIDTH, MERC_FACE_Y + MERC_FACE_HEIGHT);
		DisplayWrappedString(MERC_FACE_X, MERC_FACE_Y+MERC_PORTRAIT_TEXT_OFFSET_Y, MERC_FACE_WIDTH, 2, FONT14ARIAL, 145, MercInfo[MERC_FILES_MERC_UNAVAILABLE], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	//if the merc not paid yet, dont hire
	else if( gMercProfiles[ ubMercID ].iMercMercContractLength != 0 )
	{
		DisplayWrappedString(MERC_FACE_X, MERC_FACE_Y+MERC_PORTRAIT_TEXT_OFFSET_Y, MERC_FACE_WIDTH, 2, FONT14ARIAL, 145, MercInfo[MERC_FILES_MERC_OUTSTANDING], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	DeleteVideoObjectFromIndex(guiMercFace);

	return( TRUE );
}

void DisplayMercMemberClickOnFaceHelpText( UINT8 ubMercID )
{
	CHAR16 sString[ 128 ], sTemp[ 20 ];

	// Buggler: skills/traits tooltip on merc portrait
	if( gGameExternalOptions.fShowSkillsInHirePage == TRUE )
	{
		// clear string value
		swprintf( sString, L"");

		if (gGameOptions.fNewTraitSystem) // SANDRO - old/new traits check
		{
			UINT8 ubTempSkillArray[30];
			INT8 bNumSkillTraits = 0;

			// lets rearrange our skills to a temp array
			// we also get the number of lines (skills) to be displayed 
			for ( UINT8 ubCnt = 1; ubCnt < NUM_SKILLTRAITS_NT; ubCnt++ )
			{
				if ( ProfileHasSkillTrait( ubMercID, ubCnt ) == 2 )
				{
					ubTempSkillArray[bNumSkillTraits] = (ubCnt + NEWTRAIT_MERCSKILL_EXPERTOFFSET);
					bNumSkillTraits++;
				}
				else if ( ProfileHasSkillTrait( ubMercID, ubCnt ) == 1 )
				{
					ubTempSkillArray[bNumSkillTraits] = ubCnt;
					bNumSkillTraits++;
				}
			}

			if ( bNumSkillTraits == 0 )
			{
				swprintf( sString, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_NOSKILLS ] );
			}
			else
			{
				for ( UINT8 ubCnt = 0; ubCnt < bNumSkillTraits; ubCnt++ )
				{
					swprintf( sTemp, L"%s\n", gzMercSkillTextNew[ ubTempSkillArray[ubCnt] ] );
					wcscat( sString, sTemp );
				}
			}
		}
		else
		{
			INT8 bSkill1 = 0, bSkill2 = 0; 	
			bSkill1 = gMercProfiles[ ubMercID ].bSkillTraits[0];
			bSkill2 = gMercProfiles[ ubMercID ].bSkillTraits[1];

			if ( bSkill1 == 0 && bSkill2 == 0 )
			{
				swprintf( sString, L"%s", pPersonnelScreenStrings[ PRSNL_TXT_NOSKILLS ] );
			}
			else
			{
				//if the 2 skills are the same, add the '(expert)' at the end
				if( bSkill1 == bSkill2 )
				{
					swprintf( sString, L"%s %s", gzMercSkillText[bSkill1], gzMercSkillText[EXPERT] );
				}
				else
				{
					//Display the first skill
					if( bSkill1 != 0 )
					{
						swprintf( sString, L"%s\n", gzMercSkillText[bSkill1] );
					}
					if( bSkill2 != 0 )
					{
						swprintf( sTemp, L"%s", gzMercSkillText[bSkill2] );
						wcscat( sString, sTemp );
					}
				}
			}
		}
		SetRegionFastHelpText( &gMercSelectedFaceRegion, sString );
	}
}

void LoadAndDisplayMercBio( UINT8 ubMercID )
{
	CHAR16	sText[400];
	UINT32	uiStartLoc = 0;

	//load and display the merc bio
//#ifdef JA2UB
//	//Ja25 UB
//	if( ubMercID < 17 )
//		uiStartLoc = MERC_BIO_SIZE * ubMercID;
//	else
//		uiStartLoc = MERC_BIO_SIZE * ( ubMercID - 7 );
//#else
	uiStartLoc = MERC_BIO_SIZE * ubMercID;
//#endif
		
	LoadEncryptedDataFromFile(MERCBIOFILE, sText, uiStartLoc, MERC_BIO_INFO_TEXT_SIZE);

	if( sText[0] != 0 )	
		DisplayWrappedString(MERC_BIO_TEXT_X, MERC_BIO_TEXT_Y, MERC_BIO_WIDTH, 2, MERC_BIO_FONT, MERC_BIO_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);	
	
	//load and display the merc's additioanl info (if any)
//#ifdef JA2UB	
	//Ja25 UB
//	if( ubMercID < 17 )
//		uiStartLoc = MERC_BIO_SIZE * ubMercID + MERC_BIO_INFO_TEXT_SIZE;
//	else
//		uiStartLoc = MERC_BIO_SIZE * ( ubMercID - 7 ) + MERC_BIO_INFO_TEXT_SIZE;	
//#else
	uiStartLoc = MERC_BIO_SIZE * ubMercID + MERC_BIO_INFO_TEXT_SIZE;
//#endif	
	LoadEncryptedDataFromFile(MERCBIOFILE, sText, uiStartLoc, MERC_BIO_ADD_INFO_TEXT_SIZE);
	
	if( sText[0] != 0 )
	{
		DrawTextToScreen( MercInfo[ MERC_FILES_ADDITIONAL_INFO ], MERC_ADD_BIO_TITLE_X, MERC_ADD_BIO_TITLE_Y, 0, MERC_TITLE_FONT, MERC_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
		DisplayWrappedString( MERC_ADD_BIO_TEXT_X, MERC_ADD_BIO_TEXT_Y, MERC_BIO_WIDTH, 2, MERC_BIO_FONT, MERC_BIO_COLOR, sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	}
}


void DisplayMercsStats( UINT8 ubMercID )
{
	UINT16 usPosY, usPosX;
	CHAR16 sPage[60];
	CHAR16 sTemp[128];
	CHAR16 sString[128];
	CHAR16 NsString[128];
	CHAR16 N2sString[128];
	UINT8	ubColor;

	usPosY = MERC_HEALTH_Y;

	//Health
	DrawTextToScreen(MercInfo[MERC_FILES_HEALTH], MERC_STATS_FIRST_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bLife );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bLife, 3, MERC_STATS_FIRST_NUM_COL_X, MERC_HEALTH_Y, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Agility
	DrawTextToScreen(MercInfo[MERC_FILES_AGILITY], MERC_STATS_FIRST_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bAgility );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bAgility, 3, MERC_STATS_FIRST_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Dexterity
	DrawTextToScreen(MercInfo[MERC_FILES_DEXTERITY], MERC_STATS_FIRST_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bDexterity );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bDexterity, 3, MERC_STATS_FIRST_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Strength
	DrawTextToScreen(MercInfo[MERC_FILES_STRENGTH], MERC_STATS_FIRST_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bStrength );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bStrength, 3, MERC_STATS_FIRST_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Leadership
	DrawTextToScreen(MercInfo[MERC_FILES_LEADERSHIP], MERC_STATS_FIRST_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bLeadership );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bLeadership, 3, MERC_STATS_FIRST_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Wisdom
	DrawTextToScreen(MercInfo[MERC_FILES_WISDOM], MERC_STATS_FIRST_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bWisdom );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bWisdom, 3, MERC_STATS_FIRST_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);

	usPosY = MERC_HEALTH_Y;

	//Experience Level
	DrawTextToScreen(MercInfo[MERC_FILES_EXPLEVEL], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	// ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bExpLevel );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bExpLevel, 3, MERC_STATS_SECOND_NUM_COL_X, usPosY, MERC_STATS_FONT, FONT_MCOLOR_WHITE);
	usPosY += MERC_SPACE_BN_LINES;

	//Marksmanship
	DrawTextToScreen(MercInfo[MERC_FILES_MARKSMANSHIP], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bMarksmanship );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bMarksmanship, 3, MERC_STATS_SECOND_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Mechanical
	DrawTextToScreen(MercInfo[MERC_FILES_MECHANICAL], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bMechanical );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bMechanical, 3, MERC_STATS_SECOND_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Explosive
	DrawTextToScreen(MercInfo[MERC_FILES_EXPLOSIVE], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bExplosive );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bExplosive, 3, MERC_STATS_SECOND_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Medical
	DrawTextToScreen(MercInfo[MERC_FILES_MEDICAL], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	ubColor = GetStatsColor( gMercProfiles[ ubMercID ].bMedical );
	DrawNumeralsToScreen(gMercProfiles[ ubMercID ].bMedical, 3, MERC_STATS_SECOND_NUM_COL_X, usPosY, MERC_STATS_FONT, ubColor);
	usPosY += MERC_SPACE_BN_LINES;

	//Daily Salary
	DrawTextToScreen( MercInfo[MERC_FILES_SALARY], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	usPosX = MERC_STATS_SECOND_COL_X + StringPixLength(MercInfo[MERC_FILES_SALARY], MERC_NAME_FONT);
	swprintf(sString, L"%d", gMercProfiles[ ubMercID ].sSalary);
	InsertCommasForDollarFigure( sString );
	InsertDollarSignInToString( sString );
	swprintf(sTemp, L" %s", MercInfo[MERC_FILES_PER_DAY]);
	wcscat( sString, sTemp );
	DrawTextToScreen( sString, usPosX, usPosY, 95, MERC_NAME_FONT, MERC_DYNAMIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);

	// Buggler: Display current MERC index & total MERC members at the bottom of the screen
	swprintf( sPage, L"%d / %d", gubCurMercIndex + 1, LaptopSaveInfo.gubLastMercIndex + 1 );
	DrawTextToScreen(sPage, MERC_PAGE_X, MERC_PAGE_Y, 0, MERC_STATS_FONT, MERC_DYNAMIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED | TEXT_SHADOWED);

	//Only show when in inventory page
	if (gubCurMercFilesTogglePage == MERC_FILES_INV_PAGE)
	{
		//Gear Cost
		usPosY = usPosY + 145;
		DrawTextToScreen( MercInfo[MERC_FILES_GEAR], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_STATS_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
		
		if( (gMercProfiles[ ubMercID ].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS ) && (!(gMercProfiles[ ubMercID ].ubMiscFlags2 & PROFILE_MISC_FLAG2_MERC_GEARKIT_UNPAID)))
			gMercProfiles[ ubMercID ].usOptionalGearCost = 0;
		swprintf(NsString, L"+ ");
		swprintf(sTemp, L"%d",gMercProfiles[ ubMercID ].usOptionalGearCost);
		InsertCommasForDollarFigure( sTemp );
		InsertDollarSignInToString( sTemp );
		wcscat( NsString, sTemp );
		DrawTextToScreen( NsString, usPosX, usPosY, 95, MERC_NAME_FONT, MERC_DYNAMIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);
		usPosY += MERC_SPACE_BN_LINES;

		//Total Cost
		DrawTextToScreen( MercInfo[MERC_FILES_TOTAL], MERC_STATS_SECOND_COL_X, usPosY, 0, MERC_NAME_FONT, MERC_STATIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

		swprintf(N2sString, L"= ");
		swprintf(sTemp, L"%d",gMercProfiles[ ubMercID ].usOptionalGearCost+gMercProfiles[ ubMercID ].sSalary);
		InsertCommasForDollarFigure( sTemp );
		InsertDollarSignInToString( sTemp );
		wcscat( N2sString, sTemp );
		DrawTextToScreen( N2sString, usPosX, usPosY, 95, MERC_NAME_FONT, MERC_DYNAMIC_STATS_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED);
	}
}

UINT8	GetStatsColor( INT8 bStat )
{
	if( bStat >= 80 )
		return( HIGH_STATS_COLOR );
	else if( bStat >= 20 )
		return( MED_STATS_COLOR );
	else
		return( LOW_STATS_COLOR );
}

BOOLEAN MercFilesHireMerc(UINT8 ubMercID)
{
	MERC_HIRE_STRUCT HireMercStruct;
	INT8	bReturnCode;
	INT32 Namount=0;

	memset(&HireMercStruct, 0, sizeof(MERC_HIRE_STRUCT));

	//if the ALT key is down
	if( gfKeyState[ ALT ] && CHEATER_CHEAT_LEVEL( ) )
	{
		//set the merc to be hireable
		gMercProfiles[ ubMercID ].bMercStatus = MERC_OK;
		gMercProfiles[ ubMercID ].uiDayBecomesAvailable = 0;
	}

	//if the merc is away, dont hire
	if( !IsMercHireable( ubMercID ) )
	{
		if( gMercProfiles[ ubMercID ].bMercStatus != MERC_IS_DEAD )
		{
			guiCurrentLaptopMode = LAPTOP_MODE_MERC;
#ifdef JA2UB
			gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_TRIES_TO_HIRE_ALREADY_HIRED_MERC;
#else
			// anv: unlocked unused Biff unavailable quote and added one for Speck
			 if( ubMercID == BIFF )
				gusMercVideoSpeckSpeech = SPECK_QUOTE_BIFF_UNAVALIABLE;
			else if( ubMercID == SPECK_PLAYABLE )
				gusMercVideoSpeckSpeech = SPECK_QUOTE_SPECK_UNAVAILABLE;
			else
				gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_TRIES_TO_HIRE_ALREADY_HIRED_MERC;

#endif
			gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;
		}

		return(FALSE);
	}

	//if the merc not paid yet, dont hire
	if( gMercProfiles[ ubMercID ].iMercMercContractLength != 0 )
	{
		guiCurrentLaptopMode = LAPTOP_MODE_MERC;
		gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_TRIES_TO_HIRE_ALREADY_HIRED_MERC;
		gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;

		return(FALSE);
	}


	HireMercStruct.ubProfileID = ubMercID;

	HireMercStruct.fCopyProfileItemsOver = fMercBuyEquipment;

	HireMercStruct.iTotalContractLength = 1;

	//Specify where the merc is to appear
	HireMercStruct.sSectorX = gsMercArriveSectorX;//13;
	HireMercStruct.sSectorY = gsMercArriveSectorY;
	HireMercStruct.fUseLandingZoneForArrival = TRUE;

	HireMercStruct.uiTimeTillMercArrives = GetMercArrivalTimeOfDay( );// + ubMercID


	//Set the time and ID of the last hired merc will arrive
//	LaptopSaveInfo.sLastHiredMerc.iIdOfMerc = HireMercStruct.ubProfileID;
//	LaptopSaveInfo.sLastHiredMerc.uiArrivalTime = HireMercStruct.uiTimeTillMercArrives;

	Namount += gMercProfiles[ ubMercID ].sSalary * (-1);
	if(cAllowMercEquipment && fMercBuyEquipment==1)
		Namount += gMercProfiles[ubMercID].usOptionalGearCost * (-1);

	if(is_networked && (Namount*(-1) > LaptopSaveInfo.iCurrentBalance))
	{
		DoLapTopMessageBox( MSG_BOX_LAPTOP_DEFAULT, sATMText[ 4 ], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		return(FALSE);//not enough big ones $$$sATMText
	}

	bReturnCode = HireMerc( &HireMercStruct );

	//already have limit of mercs on the team
	if( bReturnCode == MERC_HIRE_OVER_PLAYER_LIMIT )
	{
		fMercHireOverPlayerLimitMerc = TRUE;
		return(FALSE);
	}
	else if( bReturnCode == MERC_HIRE_FAILED )
	{
		//function failed
		return(FALSE);
	}
	else
	{
		//take off nominal fee: hayden,
		if (is_networked)
		{
			AddTransactionToPlayersBook( HIRED_MERC, ubMercID, GetWorldTotalMin(), Namount );
		}
		
		#ifdef JA2UB
		//add an entry in the finacial page for the hiring of the merc
		AddTransactionToPlayersBook(PAY_SPECK_FOR_MERC, ubMercID, GetWorldTotalMin(), -(INT32)( gMercProfiles[ubMercID].uiWeeklySalary ) );
		#endif

		//JMich_MMG: Setting the flag that we bought the gear and still haven't paid for it if we succesfully hired the merc
		if ( fMercBuyEquipment )
		{
			gMercProfiles[ ubMercID ].ubMiscFlags |= PROFILE_MISC_FLAG_ALREADY_USED_ITEMS;
			gMercProfiles[ ubMercID ].ubMiscFlags2 |= PROFILE_MISC_FLAG2_MERC_GEARKIT_UNPAID;
		}

		return(TRUE);
	}
}



void BtnMercFilesBackButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			guiCurrentLaptopMode = LAPTOP_MODE_MERC;
			gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}


void EnableDisableMercFilesNextPreviousButton( )
{
	if( gubCurMercIndex <= LaptopSaveInfo.gubLastMercIndex-1 )
		EnableButton( guiNextButton );
	else
		DisableButton( guiNextButton );

	if( gubCurMercIndex > 0 )
		EnableButton( guiPrevButton );
	else
		DisableButton( guiPrevButton );
}

void HandleMercsFilesKeyBoardInput( )
{
	InputAtom					InputEvent;

	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		if( InputEvent.usEvent == KEY_DOWN )
		{
			switch (InputEvent.usParam)
			{
				case LEFTARROW:
				case 'a':
					// previous button
					if (gfKeyState [ CTRL ])
						gubCurMercIndex = 0;
					else if ( gubCurMercIndex > 0 )
						gubCurMercIndex--;
					// WANNE: If current profile has an alternate profile, skip the previous one, because it is the alternate profile,
					// otherwise we have both profiles available in MERC!
					if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
						gubCurMercIndex--;

					RenderMercsFiles( );

					//Enable or disable the buttons
					EnableDisableMercFilesNextPreviousButton( );
					break;
				case RIGHTARROW:
				case 'd':
					// next button
					if (gfKeyState [ CTRL ])
						gubCurMercIndex = LaptopSaveInfo.gubLastMercIndex;
					else if ( gubCurMercIndex <= LaptopSaveInfo.gubLastMercIndex-1 )
						gubCurMercIndex++;
					// WANNE: If current profile has an alternate profile, skip the next one, because it is the alternate profile,
					// otherwise we have both profiles available in MERC!
					//if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
					//	gubCurMercIndex++;

					RenderMercsFiles( );

					//Enable or disable the buttons
					EnableDisableMercFilesNextPreviousButton( );
				break;
				case SHIFT_LEFTARROW:
				case 'A':
					// previous button
					if (gfKeyState [ CTRL ])
						gubCurMercIndex = 0;
					else if ( gubCurMercIndex > 10)
						gubCurMercIndex -= 10;
					else
						gubCurMercIndex = 0;
					// WANNE: If current profile has an alternate profile, skip the previous one, because it is the alternate profile,
					// otherwise we have both profiles available in MERC!
					if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
						gubCurMercIndex--;

					RenderMercsFiles( );

					//Enable or disable the buttons
					EnableDisableMercFilesNextPreviousButton( );
					break;
				case SHIFT_RIGHTARROW:
				case 'D':
					// next button
					if (gfKeyState [ CTRL ])
						gubCurMercIndex = LaptopSaveInfo.gubLastMercIndex;
					else if (gubCurMercIndex <= LaptopSaveInfo.gubLastMercIndex - 10)
						gubCurMercIndex += 10;
					else
						gubCurMercIndex = LaptopSaveInfo.gubLastMercIndex;
					// WANNE: If current profile has an alternate profile, skip the next one, because it is the alternate profile,
					// otherwise we have both profiles available in MERC!
					//if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
					//	gubCurMercIndex++;

					RenderMercsFiles( );

					//Enable or disable the buttons
					EnableDisableMercFilesNextPreviousButton( );
				break;
				case ENTER:
				case 'e':
					//for no or invalid merc account, do nothing
					if( ( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_VALID ) && ( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_SUSPENDED ) && ( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_VALID_FIRST_WARNING ) )
					{					
						//do nothing
					}
					// hire only if alive
					else if( !IsMercDead( GetAvailableMercIDFromMERCArray( gubCurMercIndex ) ) )
					{
						//if the players account is suspended, go back to the main screen and have Speck inform the players
						if( LaptopSaveInfo.gubPlayersMercAccountStatus == MERC_ACCOUNT_SUSPENDED )
						{
							guiCurrentLaptopMode = LAPTOP_MODE_MERC;
#ifdef JA2UB
							//gusMercVideoSpeckSpeech = SPECK_QUOTE_ALTERNATE_OPENING_5_PLAYER_OWES_SPECK_ACCOUNT_SUSPENDED;
#else
							gusMercVideoSpeckSpeech = SPECK_QUOTE_ALTERNATE_OPENING_5_PLAYER_OWES_SPECK_ACCOUNT_SUSPENDED;
#endif
							gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;

						}
						//else try to hire the merc
						else
						{
							//bought gear before
							if( gMercProfiles[ GetAvailableMercIDFromMERCArray( gubCurMercIndex ) ].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS )
							{	
								fMercBuyEquipment = 0;
								MercProcessHireAfterGear();
							}
							//prompt to buy gear
							else
								DoLapTopMessageBox( MSG_BOX_BLUE_ON_GREY, MercInfo[ MERC_FILES_BUY_GEAR ], LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, MercHireButtonGearYesNoCallback );
						}
					}
				break;
				case BACKSPACE:
				case 'q':
					guiCurrentLaptopMode = LAPTOP_MODE_MERC;
					gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;
				break;
				case 't':
				case 'T':
					if (gubCurMercFilesTogglePage + 1 == NUM_MERC_PAGES)
					{
						gubCurMercFilesTogglePage = 0;
					}
					else
					{
						gubCurMercFilesTogglePage++;
					}
					RenderMercsFiles();
				break;

				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
					if ((UsingNewInventorySystem() == true) && (gubCurMercFilesTogglePage == MERC_FILES_INV_PAGE))
					{
						UINT8 mercId = GetAvailableMercIDFromMERCArray( gubCurMercIndex );

						if(!(gMercProfiles[mercId].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS))
						{
							UINT8 kit = 0;
							switch(InputEvent.usParam)
							{
								case '2':
									kit = 1;
									break;
								case '3':
									kit = 2;
									break;
								case '4':
									kit = 3;
									break;
								case '5':
									kit = 4;
									break;
							}
														
							for(UINT8 i=INV_START_POS; i<NUM_INV_SLOTS; ++i)
							{
								if(gMercProfileGear[mercId][kit].inv[i] != NONE)
								{
									MercWeaponKitSelectionUpdate(kit);
									RenderMercsFiles( );
									break;
								}
							}
						}
					}
					break;
				
				default:
					HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
				break;
			}
		}
		else if( InputEvent.usEvent == KEY_REPEAT )
		{
			switch( InputEvent.usParam )
			{
				case LEFTARROW:
				case 'a':
					// previous button
					if( gubCurMercIndex > 0 )
						gubCurMercIndex--;
					// WANNE: If current profile has an alternate profile, skip the previous one, because it is the alternate profile,
					// otherwise we have both profiles available in MERC!
					if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
						gubCurMercIndex--;

					RenderMercsFiles( );

					//Enable or disable the buttons
					EnableDisableMercFilesNextPreviousButton( );
					break;
				case RIGHTARROW:
				case 'd':
					// next button
					if( gubCurMercIndex <= LaptopSaveInfo.gubLastMercIndex-1 )
						gubCurMercIndex++;
					// WANNE: If current profile has an alternate profile, skip the next one, because it is the alternate profile,
					// otherwise we have both profiles available in MERC!
					//if (gConditionsForMercAvailability[ GetAvailableMercIndex(gubCurMercIndex) ].uiAlternateIndex != 255)
					//	gubCurMercIndex++;

					RenderMercsFiles( );

					//Enable or disable the buttons
					EnableDisableMercFilesNextPreviousButton( );
				break;
			}
		}
	}
}
//JMich_MMG: Kit Selection Buttons
void CreateMercKitSelectionButtons()
{
	INT32 i;

	for (i=0;i<NUM_MERCSTARTINGGEAR_KITS;i++)
	{
		guiMercWeaponBoxSelectionButton[i]=CreateIconAndTextButton( guiMercWeaponKitButtonImage, CharacterInfo[AIM_MEMBER_GEAR_KIT_ONE+i], MERC_M_KIT_BUTTON_FONT,
																MERC_M_KIT_BUTTON_UP_COLOR, DEFAULT_SHADOW,
																MERC_M_KIT_BUTTON_DOWN_COLOR, DEFAULT_SHADOW,
																TEXT_CJUSTIFIED,
																MERC_WEAPONBOX_LOADOUT_ONE_X + i * (MERC_WEAPONBOX_BUTTON_START_WIDTH + MERC_WEAPONBOX_LOADOUT_BUTTON_X_DISTANCE),
																MERC_WEAPONBOX_LOADOUT_ONE_BOX_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
																DEFAULT_MOVE_CALLBACK, BtnMercWeaponboxSelectButtonCallback);
		SetButtonCursor(guiMercWeaponBoxSelectionButton[i], CURSOR_WWW );
		MSYS_SetBtnUserData(guiMercWeaponBoxSelectionButton[i],0, i );
	}
}
void BtnMercWeaponboxSelectButtonCallback( GUI_BUTTON * btn, INT32 Reason)
{
	if(Reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(Reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			gSelectedMercKit = MSYS_GetBtnUserData(btn,0);
			//tais: handle selected kit
			MercWeaponKitSelectionUpdate(gSelectedMercKit);

			btn->uiFlags &= (~BUTTON_CLICKED_ON );
			RenderMercsFiles();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(Reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void DisableMercWeaponKitSelectionButtons()
{
	UINT8 i;
	for (i=0;i<NUM_MERCSTARTINGGEAR_KITS;i++)
	{
		HideButton(guiMercWeaponBoxSelectionButton[i]);
		//HideButton(guiMercWeaponBoxPriceSelectionButton[i]);
	}
}
void RefreshMercWeaponKitSelectionButtons()
{
	UINT8 ubMercID = GetAvailableMercIDFromMERCArray( gubCurMercIndex );
	UINT8 i;
	STR16 KitLabel;
	if (ubMercID != -1)
	{
		for (i=0;i<NUM_MERCSTARTINGGEAR_KITS;i++)
		{
			KitLabel = CharacterInfo[AIM_MEMBER_GEAR_KIT_ONE + i];
			if (gMercProfileGear[ubMercID][i].mGearKitName[0] != '\0')
			{
				KitLabel = gMercProfileGear[ubMercID][i].mGearKitName;
			}
			SpecifyButtonText( guiMercWeaponBoxSelectionButton[i], KitLabel);
		}
	}
}

//JMich_MMG: Copying (and modifying) the DisplayMercsInventory function from AimMembers.cpp
BOOLEAN DisplayMERCMercsInventory(UINT8 ubMercID)
{
	UINT8			i;
	INT16			PosX, PosY, sCenX, sCenY;
	UINT16			usItem;
	INVTYPE			*pItem;
	HVOBJECT		hVObject;
	UINT32			usHeight, usWidth;
	ETRLEObject		*pTrav;
	CHAR16			gzItemName[ 5000 ];
	UINT8			ubItemCount=0;
	UINT8			ubColumnCount=0;

	//if the mercs inventory has already been purchased, dont display the inventory
	if( gMercProfiles[ ubMercID ].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS )
		return( TRUE );

		UINT16 wnameY = MERC_MEMBER_WEAPON_NAME_Y;
		PosX = MERC_WEAPONBOX_X_NSGI+3;		// + 3 ( 1 to take care of the shadow, +2 to get past the weapon box border )
		PosY = MERC_WEAPONBOX_Y_NSGI;

		//tais: disable Weaponbox Mouseregions to stop crashing when changing kit selection and hovering over item
		//tooltips for weaponbox
		for(i=0;i<MERC_WEAPONBOX_TOTAL_ITEMS;i++) 
		{
			MSYS_DisableRegion(&gMercWeaponboxFasthelpRegion[i]);
		}

		UINT8 invsize = gMercProfiles[ ubMercID ].inv.size();
		for(i=0; i<invsize; ++i)
		{
			usItem = gMercProfiles[ubMercID].inv[ i ];

			//if its a valid item AND we are only displaying less then 8 items
			if( usItem && ubItemCount < MERC_WEAPONBOX_TOTAL_ITEMS )
			{
				//increase the item count
				ubItemCount++;
				ubColumnCount++;

				pItem = &Item[ usItem ];
				GetVideoObject( &hVObject, GetInterfaceGraphicForItem( pItem ) );
				/*JMich_MMG: this is the original one, but g_bUsePngItemImages is set to false, so I'm skipping it
				UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : pItem->ubGraphicNum;*/
				UINT16 usGraphicNum = pItem->ubGraphicNum;
				
				if(usGraphicNum < hVObject->usNumberOfObjects)
				{
					pTrav = &(hVObject->pETRLEObject[ usGraphicNum ] );
				}
				else
				{
					SGP_THROW(_BS(L"Number of images in VObject [") << hVObject->usNumberOfObjects
						<< L"] is smaller than the requested index [" << (int)pItem->ubGraphicNum << L"]" << _BS::wget);
				}

				usHeight				= (UINT32)pTrav->usHeight;
				usWidth					= (UINT32)pTrav->usWidth;

				sCenX = PosX + ( abs( MERC_WEAPONBOX_SIZE_X_NSGI - 3 - (int)usWidth ) /	2 ) - pTrav->sOffsetX;
				sCenY = PosY + ( abs( MERC_WEAPONBOX_SIZE_Y_NSGI - (int)usHeight ) / 2 ) - pTrav->sOffsetY;

				//blt the shadow of the item
				if(gGameSettings.fOptions[ TOPTION_SHOW_ITEM_SHADOW ]) 
					BltVideoObjectOutlineShadowFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sCenX-2, sCenY+2);
				
				//blt the item
				BltVideoObjectOutlineFromIndex( FRAME_BUFFER, GetInterfaceGraphicForItem( pItem ), usGraphicNum, sCenX, sCenY, 0, FALSE );

				//if there are more then 1 piece of equipment in the current slot, display how many there are
				if( gMercProfiles[ubMercID].bInvNumber[ i ] > 1 )
				{
					CHAR16 zTempStr[ 32 ];

					swprintf( zTempStr, L"x%d", gMercProfiles[ ubMercID ].bInvNumber[ i ] );

					DrawTextToScreen( zTempStr, (UINT16)(PosX-1), (UINT16)(PosY+20), MERC_MEMBER_WEAPON_NAME_WIDTH, MERC_M_FONT_DYNAMIC_TEXT, MERC_M_WEAPON_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );
				}
				
				GetHelpTextForItemInLaptop( gzItemName, usItem );
				SetRegionFastHelpText( &(gMercWeaponboxFasthelpRegion[ubItemCount-1]), gzItemName );
				SetRegionHelpEndCallback( &(gMercWeaponboxFasthelpRegion[ubItemCount-1]), HelpTextDoneCallback );
				MSYS_EnableRegion( &gMercWeaponboxFasthelpRegion[ubItemCount-1] );


				if(ubColumnCount == MERC_WEAPONBOX_COLUMNS) 
				{
					PosX = MERC_WEAPONBOX_X_NSGI+3;
					PosY += MERC_WEAPONBOX_SIZE_Y_NSGI;
					ubColumnCount = 0;
				} 
				else 
				{
					PosX += MERC_WEAPONBOX_SIZE_X_NSGI;
				}

#ifdef ENCYCLOPEDIA_WORKS
				//Moa: update encyclopedia item visibility when item gets displayed
				EncyclopediaSetItemAsVisible( usItem, ENC_ITEM_DISCOVERED_NOT_INSPECTABLE );
#endif
			}
		}

	return(TRUE);
}
//JMich_MMG: Creating the mouseregion for tooltips
void CreateMercWeaponBoxMouseRegions()
{
	UINT16	i, x, uiPosX, uiPosY;
	UINT16	itemcounter = 0;
	//tais: Weaponbox Mouseregions
	for(i=0; i<MERC_WEAPONBOX_ROWS; i++) {
		uiPosX = MERC_WEAPONBOX_X_NSGI;
		uiPosY = MERC_WEAPONBOX_Y_NSGI + (MERC_WEAPONBOX_SIZE_Y_NSGI*i);
		for(x=0; x<MERC_WEAPONBOX_COLUMNS; x++) {
			MSYS_DefineRegion(&gMercWeaponboxFasthelpRegion[itemcounter],uiPosX,uiPosY,uiPosX+MERC_WEAPONBOX_SIZE_X_NSGI,uiPosY+MERC_WEAPONBOX_SIZE_Y_NSGI,MSYS_PRIORITY_HIGHEST, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK);
			MSYS_AddRegion(&gMercWeaponboxFasthelpRegion[itemcounter]);
			MSYS_DisableRegion(&gMercWeaponboxFasthelpRegion[itemcounter]);
			uiPosX += MERC_WEAPONBOX_SIZE_X_NSGI;
			itemcounter++;
		}
	}
}

//JMich_MMG: Enabling of kit selection buttons
void EnableMercWeaponKitSelectionButtons()
{
	UINT8 i,j;
	UINT8 buttonCount = 0;
	BOOL buttonEnabled[NUM_MERCSTARTINGGEAR_KITS];
	//tais: weaponbox gear selection buttons
	if(UsingNewInventorySystem() == true) {
		if(!(gMercProfiles[GetAvailableMercIDFromMERCArray( gubCurMercIndex )].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS)) {
			for(i=0; i<NUM_MERCSTARTINGGEAR_KITS; i++)
			{
				buttonEnabled[i] = FALSE;
				for(j=INV_START_POS; j<NUM_INV_SLOTS; j++)
				{
					if(gMercProfileGear[GetAvailableMercIDFromMERCArray( gubCurMercIndex )][i].inv[j] != NONE)
					{
						buttonEnabled[i] = TRUE;
						buttonCount++;
						break;
					}
				}
			}
			if(buttonCount > 1){
				for (i=0;i<NUM_MERCSTARTINGGEAR_KITS;i++)
				{
					if(buttonEnabled[i] == TRUE)
					{
							ShowButton(guiMercWeaponBoxSelectionButton[i]);
					}
				}
			}
		}
	}
}
//JMich_MMG: Adding the weaponbox background, currently using the AIM one
void CreateMercWeaponBoxBackground()
{
	//tais: nsgi, create background for multiline weaponbox
	HVOBJECT	hMercWeaponBoxHandle;
	UINT16	i, x, uiPosX, uiPosY;

	GetVideoObject(&hMercWeaponBoxHandle, guiMercWeaponBox);
	for(i=0; i<MERC_WEAPONBOX_ROWS; i++) {
		uiPosX = MERC_WEAPONBOX_X_NSGI;
		uiPosY = MERC_WEAPONBOX_Y_NSGI + (MERC_WEAPONBOX_SIZE_Y_NSGI*i);
		for(x=0; x<MERC_WEAPONBOX_COLUMNS; x++)
		{
			BltVideoObject(FRAME_BUFFER, hMercWeaponBoxHandle, 0, uiPosX, uiPosY, VO_BLT_SRCTRANSPARENCY,NULL);
			uiPosX += MERC_WEAPONBOX_SIZE_X_NSGI;
		}
	}
}
//JMich_MMG: Stealing tais routine (again)
void MercWeaponKitSelectionUpdate(UINT8 selectedInventory)
{
	UINT8 ubMercID = GetAvailableMercIDFromMERCArray( gubCurMercIndex );
	UINT32 uiLoop;
	INT16 usItem;
	if(!( gMercProfiles[ ubMercID ].ubMiscFlags & PROFILE_MISC_FLAG_ALREADY_USED_ITEMS ))
	{
		if(UsingNewInventorySystem() == true){
			// Start by resetting all profile inventory values to 0
			gMercProfiles[ubMercID].clearInventory();
			gMercProfiles[ubMercID].ubInvUndroppable = 0;
			// Next, go through and assign everything but lbe gear
			for(uiLoop=INV_START_POS; uiLoop<NUM_INV_SLOTS; uiLoop++)
			{
				if(gMercProfileGear[ubMercID][selectedInventory].inv[uiLoop] != NONE)
				{
					gMercProfiles[ubMercID].inv[uiLoop] = gMercProfileGear[ubMercID][selectedInventory].inv[uiLoop];
					gMercProfiles[ubMercID].bInvStatus[uiLoop] = gMercProfileGear[ubMercID][selectedInventory].iStatus[uiLoop];
					if(uiLoop > 5)
						gMercProfiles[ubMercID].bInvNumber[uiLoop] = gMercProfileGear[ubMercID][selectedInventory].iNumber[uiLoop];
					else
						gMercProfiles[ubMercID].bInvNumber[uiLoop] = 1;
				}
			}
			// Last, go through and assign LBE items.  Only needed for new inventory system
			if((UsingNewInventorySystem() == true))
			{
				for(uiLoop=0; uiLoop<5; uiLoop++)
				{
					UINT32 uiLoop2 = uiLoop + VESTPOCKPOS;
					if(gMercProfileGear[ubMercID][selectedInventory].lbe[uiLoop] != NONE){
						gMercProfiles[ubMercID].inv[uiLoop2] = gMercProfileGear[ubMercID][selectedInventory].lbe[uiLoop];
						gMercProfiles[ubMercID].bInvStatus[uiLoop2] = gMercProfileGear[ubMercID][selectedInventory].lStatus[uiLoop];
						gMercProfiles[ubMercID].bInvNumber[uiLoop2] = 1;
					}
				}
			}
		}
		gMercProfiles[ubMercID].bMainGunAttractiveness		= -1;
		gMercProfiles[ubMercID].bArmourAttractiveness			= -1;

		UINT32 invsize = gMercProfiles[ ubMercID ].inv.size();
		for ( uiLoop = 0; uiLoop < invsize; ++uiLoop )
		{
			usItem = gMercProfiles[ubMercID].inv[ uiLoop ];

			if ( usItem != NOTHING )
			{
				// Check if it's a gun
				if ( Item[ usItem ].usItemClass & IC_GUN )
				{
					gMercProfiles[ubMercID].bMainGunAttractiveness = Weapon[ usItem ].ubDeadliness;
				}

				// If it's armour
				if ( Item[ usItem ].usItemClass & IC_ARMOUR )
				{
					gMercProfiles[ubMercID].bArmourAttractiveness = min(128,Armour[ Item[ usItem ].ubClassIndex ].ubProtection);
				}
			}
		}

		//reset Gear Kit Price
		gMercProfiles[ ubMercID ].usOptionalGearCost = 0;		

		//tais: new tag in gearkit that sets an absolute price for gearkit that will override item value and price modifier if it's a sensible value between 0 and 32000
		if(gMercProfileGear[ubMercID][selectedInventory].AbsolutePrice >= 0 && gMercProfileGear[ubMercID][selectedInventory].AbsolutePrice <= 32000)
		{
			gMercProfiles[ ubMercID ].usOptionalGearCost = gMercProfileGear[ubMercID][selectedInventory].AbsolutePrice;
		}
		else
		{
			UINT16 tempGearCost = 0;
			UINT32 invsize = gMercProfiles[ ubMercID ].inv.size();
			for ( uiLoop = 0; uiLoop< invsize; ++uiLoop )
			{
				if ( gMercProfiles[ ubMercID ].inv[ uiLoop ] != NOTHING )
				{
					//get the item
					usItem = gMercProfiles[ ubMercID ].inv[ uiLoop ];

					// for an item stack, we obviously need to account fot the number of items
					// for single items, the number is not always set, so just to be sure...
					int number = 1;
					if ( gMercProfileGear[ubMercID][selectedInventory].iNumber.size( ) >= uiLoop )
						number = max( 1, gMercProfileGear[ubMercID][selectedInventory].iNumber[uiLoop] );

					//add the cost
					tempGearCost += number * Item[usItem].usPrice;
				}
			}
			//tais: added optional price modifier for gearkits, reads the xml tag mPriceMod from MercStartingGear.xml
			if(gMercProfileGear[ubMercID][selectedInventory].PriceModifier != 0 &&
				gMercProfileGear[ubMercID][selectedInventory].PriceModifier <= 200 &&
				gMercProfileGear[ubMercID][selectedInventory].PriceModifier >= -100)
			{
				FLOAT mod;
				mod = (FLOAT) (gMercProfileGear[ubMercID][selectedInventory].PriceModifier + 100) / 100;
				gMercProfiles[ ubMercID ].usOptionalGearCost = (UINT16)(tempGearCost * mod);
			}
			else
			{
				gMercProfiles[ ubMercID ].usOptionalGearCost = tempGearCost;
			}
		}
	}
}

void MercHireButtonGearYesNoCallback (UINT8 bExitValue)
{
	//yes, buy gear
	if( bExitValue == MSG_BOX_RETURN_YES )
		fMercBuyEquipment = 1;
	//no, no gear 
	else
		fMercBuyEquipment = 0;
	
	MercProcessHireAfterGear();
}

void MercProcessHireAfterGear()
{
	//process after gear check
	guiCurrentLaptopMode = LAPTOP_MODE_MERC;
	gubArrivedFromMercSubSite = MERC_CAME_FROM_HIRE_PAGE;
	
	if( MercFilesHireMerc( GetAvailableMercIDFromMERCArray( gubCurMercIndex ) ) )
	{
		//start the merc talking
		//HandlePlayerHiringMerc( GetAvailableMercIDFromMERCArray( gubCurMercIndex ) );

		//We just hired a merc
		gfJustHiredAMercMerc = TRUE;
	}
}
