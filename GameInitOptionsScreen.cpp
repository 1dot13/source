#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Intro.h"
#else
	#include "Types.h"
	#include "GameInitOptionsScreen.h"
	#include "GameSettings.h"
	#include "Utilities.h"
	#include "wCheck.h"
	#include "Font Control.h"
	#include "WordWrap.h"
	#include "Render Dirty.h"
	#include "Input.h"
	#include "Options Screen.h"
	#include "English.h"
	#include "Sysutil.h"
	#include "Fade Screen.h"
	#include "Cursor Control.h"
	#include "Music Control.h"
	#include "cursors.h"
	#include "Intro.h"
	#include "Text.h"
	#include "_Ja25EnglishText.h"
	#include "Soldier Profile.h"
#endif

#include "gameloop.h"
#include "connect.h"
#include "saveloadscreen.h"

#ifdef JA2UB
	#include "ub_config.h"
#endif

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_init.h>
#include <vfs/Tools/vfs_property_container.h>
#include <vfs/Core/vfs_os_functions.h>

#ifdef JA2UB

// *********************************
// Unfinished Business: Initial Game Screen
// *********************************

//////////////////////////////////////////////////////////////
//	SANDRO - the start-new-game screen has been changed a lot
//////////////////////////////////////////////////////////////

////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		GIO_TITLE_FONT							FONT14ARIAL
#define		GIO_TITLE_COLOR							FONT_MCOLOR_WHITE

#define		GIO_TOGGLE_TEXT_FONT					FONT10ARIAL
#define		GIO_TOGGLE_TEXT_COLOR					FONT_MCOLOR_WHITE

//buttons
#define		GIO_CANCEL_X							iScreenWidthOffset + ((320 - 115) / 2)  

#define		GIO_BTN_START_X							iScreenWidthOffset + 320 + 105
#define		GIO_BTN_START_Y							iScreenHeightOffset + 435
 

//main title
#define		GIO_MAIN_TITLE_X						0
#define		GIO_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		GIO_MAIN_TITLE_WIDTH					SCREEN_WIDTH

//radio box locations
#define		GIO_GAP_BN_SETTINGS						35
#define		GIO_OFFSET_TO_TEXT						36
#define		GIO_OFFSET_TO_TOGGLE_BOX				180
#define		GIO_OFFSET_TO_TOGGLE_BOX_Y				9

#define		GIO_TITLE_DISTANCE						30 // higher means closer

#define		GIO_DIF_SETTING_X						iScreenWidthOffset + 48
#define		GIO_DIF_SETTING_Y						iScreenHeightOffset + 55
#define		GIO_DIF_SETTING_WIDTH					GIO_OFFSET_TO_TOGGLE_BOX - GIO_OFFSET_TO_TEXT

#define		GIO_IMP_SETTING_X						GIO_DIF_SETTING_X
#define		GIO_IMP_SETTING_Y						GIO_DIF_SETTING_Y + 63
#define		GIO_IMP_SETTING_WIDTH					GIO_DIF_SETTING_WIDTH

// old/new traits
#define		GIO_TRAITS_SETTING_X					GIO_DIF_SETTING_X + 36
#define		GIO_TRAITS_SETTING_Y					GIO_IMP_SETTING_Y + 52
#define		GIO_TRAITS_SETTING_WIDTH				GIO_DIF_SETTING_WIDTH

// Madd
#define		GIO_GAME_SETTING_X						GIO_TRAITS_SETTING_X
#define		GIO_GAME_SETTING_Y						GIO_TRAITS_SETTING_Y + 67
#define		GIO_GAME_SETTING_WIDTH					GIO_DIF_SETTING_WIDTH

#define		GIO_IRON_MAN_SETTING_X					GIO_TRAITS_SETTING_X
#define		GIO_IRON_MAN_SETTING_Y					GIO_GAME_SETTING_Y + 67
#define		GIO_IRON_MAN_SETTING_WIDTH				GIO_DIF_SETTING_WIDTH

// Tex and Jon
#define		GIO_TERRORISTS_SETTING_X				GIO_TRAITS_SETTING_X
#define		GIO_TERRORISTS_SETTING_Y				GIO_IRON_MAN_SETTING_Y + 67
#define		GIO_TERRORISTS_SETTING_WIDTH			GIO_DIF_SETTING_WIDTH

#define		GIO_BR_SETTING_X						iScreenWidthOffset + 370
#define		GIO_BR_SETTING_Y						GIO_DIF_SETTING_Y
#define		GIO_BR_SETTING_WIDTH					GIO_DIF_SETTING_WIDTH

#define		GIO_SQUAD_SIZE_SETTING_X				GIO_BR_SETTING_X
#define		GIO_SQUAD_SIZE_SETTING_Y				GIO_BR_SETTING_Y + 63
#define		GIO_SQUAD_SIZE_SETTING_WIDTH			GIO_DIF_SETTING_WIDTH

#define		GIO_INV_SETTING_X						GIO_SQUAD_SIZE_SETTING_X
#define		GIO_INV_SETTING_Y						GIO_SQUAD_SIZE_SETTING_Y + 63
#define		GIO_INV_SETTING_WIDTH					GIO_DIF_SETTING_WIDTH

#define		GIO_DROPALL_SETTING_X					GIO_INV_SETTING_X + 36
#define		GIO_DROPALL_SETTING_Y					GIO_INV_SETTING_Y + 56
#define		GIO_DROPALL_SETTING_WIDTH				GIO_DIF_SETTING_WIDTH

#define		GIO_GUN_SETTING_X						GIO_DROPALL_SETTING_X
#define		GIO_GUN_SETTING_Y						GIO_DROPALL_SETTING_Y + 67
#define		GIO_GUN_SETTING_WIDTH					GIO_DIF_SETTING_WIDTH

#define		GIO_CACHES_SETTING_X					GIO_GUN_SETTING_X
#define		GIO_CACHES_SETTING_Y					GIO_GUN_SETTING_Y + 67
#define		GIO_CACHES_SETTING_WIDTH				GIO_DIF_SETTING_WIDTH

#define		GIO_TIMED_TURN_SETTING_X				GIO_GUN_SETTING_X
#define		GIO_TIMED_TURN_SETTING_Y				GIO_IRON_MAN_SETTING_Y
#define		GIO_TIMED_TURN_SETTING_WIDTH			GIO_DIF_SETTING_WIDTH

#define		GIO_PROGRESS_SETTING_X					GIO_SQUAD_SIZE_SETTING_X
#define		GIO_PROGRESS_SETTING_Y					GIO_TERRORISTS_SETTING_Y + 13
#define		GIO_PROGRESS_SETTING_WIDTH				GIO_DIF_SETTING_WIDTH


// INI File
#define		JA2SP_INI_FILENAME						"ja2_sp.ini"

// INI Section
#define		JA2SP_INI_INITIAL_SECTION				"JA2 Singleplayer Initial Settings"

// INI Properties
#define		JA2SP_DIFFICULTY_LEVEL					"DIFFICULTY_LEVEL"
#define		JA2SP_BOBBY_RAY_SELECTION				"BOBBY_RAY_SELECTION"
#define		JA2SP_MAX_IMP_CHARACTERS				"MAX_IMP_CHARACTERS"
#define		JA2SP_PROGRESS_SPEED_OF_ITEM_CHOICES	"PROGRESS_SPEED_OF_ITEM_CHOICES"
#define		JA2SP_SKILL_TRAITS						"SKILL_TRAITS"
#define		JA2SP_INVENTORY_ATTACHMENTS				"INVENTORY_ATTACHMENTS"
#define		JA2SP_GAME_STYLE						"GAME_STYLE"
#define		JA2SP_ENEMIES_DROP_ALL_ITEMS			"ENEMIES_DROP_ALL_ITEMS"
#define		JA2SP_EXTRA_DIFFICULTY					"EXTRA_DIFFICULTY"
#define		JA2SP_AVAILABLE_ARSENAL					"AVAILABLE_ARSENAL"
#define		JA2SP_NUMBER_OF_TERRORISTS				"NUMBER_OF_TERRORISTS"
#define		JA2SP_SECRET_WEAPON_CACHES				"SECRET_WEAPON_CACHES"
#define		JA2SP_SQUAD_SIZE						"SQUAD_SIZE"

// Exclusive UB
#define		JA2SP_UB_RPC_TEX_AND_JOHN				"RPC_TEX_AND_JOHN"
#define		JA2SP_UB_RANDOM_MANUEL_TEXT				"RANDOM_MANUEL_TEXT"


//Difficulty settings
enum
{
	GIO_DIFF_EASY,
	GIO_DIFF_MED,
	GIO_DIFF_HARD,
	GIO_DIFF_INSANE,

	NUM_DIFF_SETTINGS,
};

enum
{
	GIO_TRAITS_OLD,
	GIO_TRAITS_NEW,

	NUM_TRAIT_OPTIONS,
};

// Game Settings options
enum
{
	GIO_TEXT_OFF,
	GIO_TEXT_ON,	

	NUM_TEXT_STYLES,
};

enum
{
	GIO_REALISTIC,
	GIO_SCI_FI,	

	NUM_GAME_STYLES,
};


// Iron man mode
enum
{
	GIO_CAN_SAVE,
	GIO_IRON_MAN,

	NUM_SAVE_OPTIONS,
};

enum
{
	GIO_TEX_JOHN_RANDOM,
	GIO_TEX_AND_JOHN,

	NUM_RPC_UB_OPTIONS,
};

// BR options
enum
{
	GIO_BR_GOOD,
	GIO_BR_GREAT,
	GIO_BR_EXCELLENT,
	GIO_BR_AWESOME,

	NUM_BR_OPTIONS,
};

enum
{
	GIO_PROGRESS_VERY_SLOW,
	GIO_PROGRESS_SLOW,
	GIO_PROGRESS_NORMAL,
	GIO_PROGRESS_FAST,
	GIO_PROGRESS_VERY_FAST,

	NUM_PROGRESS_OPTIONS,
};

// New inventory options
enum
{
	GIO_INV_OLD,
	GIO_INV_NEW,
	GIO_INV_NEW_NAS,	// WANNE: Added NAS to the start new game screen

	NUM_INV_OPTIONS,
};

enum
{
	GIO_SQUAD_SIZE_6,
	GIO_SQUAD_SIZE_8,
	GIO_SQUAD_SIZE_10,

	NUM_SQUAD_SIZE_OPTIONS,
};

enum
{
	GIO_DROPALL_OFF,
	GIO_DROPALL_ON,

	NUM_DROPALL_OPTIONS,
};

// Gun options
enum
{
	GIO_REDUCED_GUNS,
	GIO_GUN_NUT,

	NUM_GUN_OPTIONS,
};

enum
{
	GIO_CACHES_RANDOM,
	GIO_CACHES_ALL,

	NUM_CACHES_OPTIONS,
};


// Timed turns setting (Multiplayer exclusive)
enum
{
	GIO_NO_TIMED_TURNS,
	GIO_TIMED_TURNS,

	GIO_NUM_TIMED_TURN_OPTIONS,
};


//enum for different states of game
enum
{
	GIO_NOTHING,
	GIO_CANCEL,
	GIO_EXIT,
	GIO_IRON_MAN_MODE,
	MP_LOAD
};

////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfGIOScreenEntry = TRUE;
BOOLEAN		gfGIOScreenExit	= FALSE;
BOOLEAN		gfReRenderGIOScreen=TRUE;
BOOLEAN		gfGIOButtonsAllocated = FALSE;

UINT8		gubGameOptionScreenHandler=GIO_NOTHING;

UINT32		gubGIOExitScreen = GAME_INIT_OPTIONS_SCREEN;

UINT32		guiGIOMainBackGroundImage;

INT32		giGioMessageBox = -1;

INT8 iCurrentDifficulty;
INT8 iCurrentBRSetting;
INT8 iCurrentIMPNumberSetting;
INT8 iCurrentProgressSetting;
INT8 iCurrentInventorySetting;
INT8 iCurrentSquadSize;


UINT32 guiGIOSMALLFRAME;

// Done Button
void BtnGIODoneCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiGIODoneButton;
INT32		giGIODoneBtnImage;

// Cancel Button
void BtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiGIOCancelButton;
INT32		giGIOCancelBtnImage;

// MP LOAD Button
void MPBtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	MPguiGIOCancelButton;
INT32		MPgiGIOCancelBtnImage;

UINT32 giGIODifficultyButton[ 2 ];
INT32 giGIODifficultyButtonImage[ 2 ];
void BtnGIODifficultySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIODifficultySelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOIMPNumberButton[ 2 ];
INT32 giGIOIMPNumberButtonImage[ 2 ];
void BtnGIOIMPNumberSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOIMPNumberSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOBRSettingButton[ 2 ];
INT32 giGIOBRSettingButtonImage[ 2 ];
void BtnGIOBRSettingLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOBRSettingRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOProgressSettingButton[ 2 ];
INT32 giGIOProgressSettingButtonImage[ 2 ];
void BtnGIOProgressSettingLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOProgressSettingRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOInventorySettingButton [ 2 ];
INT32 giGIOInventorySettingButtonImage [ 2 ];
void BtnGIOInventorySettingLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOInventorySettingRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32	guiTraitsOptionTogglesImage[ NUM_TRAIT_OPTIONS ];
UINT32	guiTraitsOptionToggles[ NUM_TRAIT_OPTIONS ];
void BtnGIOOldTraitsCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIONewTraitsCallback(GUI_BUTTON *btn,INT32 reason);
void NewTraitsNotPossibleMessageBoxCallBack( UINT8 bExitValue );

//checkbox to toggle Game style
UINT32	guiGameTextTogglesImage[ NUM_TEXT_STYLES ];
UINT32	guiGameTextToggles[ NUM_TEXT_STYLES ];

UINT32	guiGameStyleTogglesImage[ NUM_GAME_STYLES ];
UINT32	guiGameStyleToggles[ NUM_GAME_STYLES ];

void BtnGIOOffStyleCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOOnStyleCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Save style
UINT32	guiGameSaveTogglesImage[ NUM_SAVE_OPTIONS ];
UINT32	guiGameSaveToggles[ NUM_SAVE_OPTIONS ];
void BtnGIOIronManOffCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOIronManOnCallback(GUI_BUTTON *btn,INT32 reason);

UINT32	guiRpcOptionTogglesImage[ NUM_RPC_UB_OPTIONS ];
UINT32	guiRpcOptionToggles[ NUM_RPC_UB_OPTIONS ];

void BtnGIORpcRandomCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIORpcAllCallback(GUI_BUTTON *btn,INT32 reason);

UINT32	guiDropAllOptionTogglesImage[ NUM_DROPALL_OPTIONS ];
UINT32	guiDropAllOptionToggles[ NUM_DROPALL_OPTIONS ];
void BtnGIODropAllOffCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIODropAllOnCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Gun options
UINT32	guiGunOptionTogglesImage[ NUM_GUN_OPTIONS ];
UINT32	guiGunOptionToggles[ NUM_GUN_OPTIONS ];
void BtnGIOGunSettingReducedCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOGunSettingToGCallback(GUI_BUTTON *btn,INT32 reason);


UINT32 giGIOSquadSizeButton[ 2 ];
INT32 giGIOSquadSizeButtonImage[ 2 ];
void BtnGIOSquadSizeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOSquadSizeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );


UINT32	guiTimedTurnToggles[ GIO_NUM_TIMED_TURN_OPTIONS ];
void BtnTimedTurnsTogglesCallback(GUI_BUTTON *btn,INT32 reason);

void RenderGIOSmallSelectionFrame(INT16 sX, INT16 sY);

////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern		void ClearMainMenu();

BOOLEAN		EnterGIOScreen();
BOOLEAN		ExitGIOScreen();
void			HandleGIOScreen();
BOOLEAN		RenderGIOScreen();
void			GetGIOScreenUserInput();
UINT8			GetCurrentGunButtonSetting();
// JA2Gold: added save (iron man) button setting
UINT8			GetCurrentGameSaveButtonSetting();

UINT8			GetCurrentTextStyleButtonSetting();

// SANDRO - added following
UINT8	GetCurrentTraitsOptionButtonSetting();
UINT8	GetCurrentDropAllButtonSetting();
UINT8	GetCurrentTexAndJohnButtonSetting();

void		DoneFadeOutForExitGameInitOptionScreen( void );
void		DoneFadeInForExitGameInitOptionScreen( void );
UINT8		GetCurrentTimedTurnsButtonSetting();
BOOLEAN		DoGioMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void		DisplayMessageToUserAboutGameDifficulty();
void		ConfirmGioDifSettingMessageBoxCallBack( UINT8 bExitValue );
BOOLEAN		DisplayMessageToUserAboutIronManMode();
BOOLEAN		DisplayMessageToUserAboutOIVandNASincompatibility();
void		ConfirmGioIronManMessageBoxCallBack( UINT8 bExitValue );

BOOLEAN SpIniExists()
{
	BOOLEAN exists = TRUE;

	if(!getVFS()->fileExists(JA2SP_INI_FILENAME))	
		exists = FALSE;		
	
	return exists;
}

UINT32	GameInitOptionsScreenInit( void )
{
	memset( &gGameOptions, 0, sizeof( GAME_OPTIONS ) );

	// WANNE: Read initial game settings from ja2_sp.ini
	vfs::PropertyContainer props;

	if (SpIniExists())
		props.initFromIniFile(JA2SP_INI_FILENAME);

	// Difficulty Level (Default: Experienced = 1)
	gGameOptions.ubDifficultyLevel =  ((UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_DIFFICULTY_LEVEL, 1)) + 1;

	// Bobby Ray's Selection (Default: Great = 1)
	UINT8 ubBobbyRay = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_BOBBY_RAY_SELECTION, 1);
	switch (ubBobbyRay)
	{
		// Normal
		case 0:
			gGameOptions.ubBobbyRay =  BR_GOOD;
			break;
		// Great
		case 1:
			gGameOptions.ubBobbyRay =  BR_GREAT;
			break;
		// Excellent
		case 2:
			gGameOptions.ubBobbyRay =  BR_EXCELLENT;
			break;
		// Awesome
		case 3:
			gGameOptions.ubBobbyRay =  BR_AWESOME;
			break;
	}

	// Max. IMP Characters
	UINT8 maxIMPCharacterCount = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_MAX_IMP_CHARACTERS, 1);
	gGameOptions.ubMaxIMPCharacters = min( (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount), ( max( 1, maxIMPCharacterCount) ));

	// Progress Speed of Item Choices (Default: Normal)
	gGameOptions.ubProgressSpeedOfItemsChoices =  (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_PROGRESS_SPEED_OF_ITEM_CHOICES, ITEM_PROGRESS_NORMAL);

	// Skill Traits
	UINT8 ubTraitSystem = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_SKILL_TRAITS, 0);
	if (!gGameExternalOptions.fReadProfileDataFromXML)
		ubTraitSystem = 0;

	gGameOptions.fNewTraitSystem = ubTraitSystem;

	// Inventory Attachments (Default: New/New = 2)
	UINT8 ubInventoryAttachmentSystem = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_INVENTORY_ATTACHMENTS, 0);
	
	// NIV is not allowed
	if (!IsNIVModeValid(true))
		ubInventoryAttachmentSystem = 0;

	switch (ubInventoryAttachmentSystem)
	{
		// Old / Old
		case 0:
			gGameOptions.ubInventorySystem	= INVENTORY_OLD;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
			break;
		// New / Old
		case 1:
			gGameOptions.ubInventorySystem	= INVENTORY_NEW;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
			break;
		// New / New
		case 2:
			gGameOptions.ubInventorySystem	= INVENTORY_NEW;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_NEW;
			break;
	}

	// Game Style (Default: Sci Fi = 1)
	gGameOptions.ubGameStyle =  (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_GAME_STYLE, STYLE_SCIFI);

	// Enemies Drop All Items (Default: Off = 0)
	gGameOptions.fEnemiesDropAllItems =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_ENEMIES_DROP_ALL_ITEMS, 0);

	// Extra Difficulty (Default: Save Anytime = 0)
	gGameOptions.fIronManMode =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_EXTRA_DIFFICULTY, 0);


	// Available Arsenal (Default: Tons of Guns = 1)
	gGameOptions.fGunNut =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_AVAILABLE_ARSENAL, 1);
	
	// tex and john
	gGameUBOptions.fTexAndJohn =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_UB_RPC_TEX_AND_JOHN, 0);	
	
	// Random Manuel Text
	gGameUBOptions.fRandomManuelText =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_UB_RANDOM_MANUEL_TEXT, 0);	
	
	// Squad size (Default: 6)
	UINT8 ubSquadSize = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_SQUAD_SIZE, 6);

	if (iResolution == 0)
		ubSquadSize = 6;

	gGameOptions.ubSquadSize = ubSquadSize;
	
	// Secret Weapon Caches (Default: Random = 0)
	gGameOptions.fEnableAllWeaponCaches =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_SECRET_WEAPON_CACHES, 0);

	gGameOptions.fAirStrikes =  FALSE;
	gGameOptions.fTurnTimeLimit	= FALSE;

	return (1);
}

UINT32	GameInitOptionsScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfGIOScreenEntry )
	{
		GameInitOptionsScreenInit();

		EnterGIOScreen();
		gfGIOScreenEntry = FALSE;
		gfGIOScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	GetGIOScreenUserInput();

	HandleGIOScreen();

	// render buttons marked dirty
	MarkButtonsDirty( );
	RenderButtons( );

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	if ( HandleFadeOutCallback( ) )
	{
		ClearMainMenu();
		return( gubGIOExitScreen );
	}

	if ( HandleBeginFadeOut( gubGIOExitScreen ) )
	{
		return( gubGIOExitScreen );
	}

	if( gfGIOScreenExit )
	{
		ExitGIOScreen();
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderGIOScreen();
	}

	if ( HandleBeginFadeIn( gubGIOExitScreen ) )
	{
	}

	return( gubGIOExitScreen );
}


UINT32	GameInitOptionsScreenShutdown( void )
{
	return( 1 );
}


BOOLEAN		EnterGIOScreen()
{
	VOBJECT_DESC	VObjectDesc, VObjectDesc2;
	
	if( gfGIOButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	// load the Main trade screen backgroiund image
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution == 0)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 1)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_800x600.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 2)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_1024x768.sti", VObjectDesc.ImageFile);
	}

	CHECKF(AddVideoObject(&VObjectDesc, &guiGIOMainBackGroundImage ));

	VObjectDesc2.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\GIOSmallFrame.sti", VObjectDesc2.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc2, &guiGIOSMALLFRAME));

	//Ok button
	giGIODoneBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiGIODoneButton = CreateIconAndTextButton( giGIODoneBtnImage, gzGIOScreenText[GIO_START_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													GIO_BTN_START_X, GIO_BTN_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIODoneCallback);

	SpecifyButtonSoundScheme( guiGIODoneButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiGIODoneButton, DISABLED_STYLE_NONE );

	//Cancel button
	giGIOCancelBtnImage = UseLoadedButtonImage( giGIODoneBtnImage, -1,1,-1,3,-1 );
	guiGIOCancelButton = CreateIconAndTextButton( giGIOCancelBtnImage, gzGIOScreenText[GIO_CANCEL_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													GIO_CANCEL_X, GIO_BTN_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOCancelCallback );
	SpecifyButtonSoundScheme( guiGIOCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIFFICULTY SETTING

	giGIODifficultyButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giGIODifficultyButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giGIODifficultyButton[ 0 ] = QuickCreateButton( giGIODifficultyButtonImage[ 0 ], GIO_DIF_SETTING_X + 39, GIO_DIF_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIODifficultySelectionLeftCallback );

	// right button - increment difficulty level
	giGIODifficultyButton[ 1 ] = QuickCreateButton( giGIODifficultyButtonImage[ 1 ], GIO_DIF_SETTING_X + 158, GIO_DIF_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIODifficultySelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giGIODifficultyButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIODifficultyButton[1],0, 1 );

	iCurrentDifficulty = max( 0, gGameOptions.ubDifficultyLevel - 1);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// MAX IMP NUMBER SETTING

	giGIOIMPNumberButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ],-1,0,-1,1,-1 );
	giGIOIMPNumberButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ],-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giGIOIMPNumberButton[ 0 ] = QuickCreateButton( giGIOIMPNumberButtonImage[ 0 ], GIO_IMP_SETTING_X + 39, GIO_IMP_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOIMPNumberSelectionLeftCallback );

	// right button - increment difficulty level
	giGIOIMPNumberButton[ 1 ] = QuickCreateButton( giGIOIMPNumberButtonImage[ 1 ], GIO_IMP_SETTING_X + 158, GIO_IMP_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOIMPNumberSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giGIOIMPNumberButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIOIMPNumberButton[1],0, 1 );

	iCurrentIMPNumberSetting = gGameOptions.ubMaxIMPCharacters;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// OLD/NEW TARITS SETTING

	guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiTraitsOptionToggles[ GIO_TRAITS_OLD ] = CreateIconAndTextButton( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], gzGIOScreenText[ GIO_TRAITS_OLD_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TRAITS_SETTING_X + 74), (GIO_TRAITS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOOldTraitsCallback);

	guiTraitsOptionTogglesImage[ GIO_TRAITS_NEW ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiTraitsOptionToggles[ GIO_TRAITS_NEW ] = CreateIconAndTextButton( guiTraitsOptionTogglesImage[ GIO_TRAITS_NEW ],  gzGIOScreenText[ GIO_TRAITS_NEW_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TRAITS_SETTING_X), (GIO_TRAITS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIONewTraitsCallback );

	SpecifyButtonSoundScheme( guiTraitsOptionToggles[ GIO_TRAITS_OLD ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiTraitsOptionToggles[ GIO_TRAITS_NEW ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiTraitsOptionToggles[ GIO_TRAITS_OLD ],0, 0 );
	MSYS_SetBtnUserData(guiTraitsOptionToggles[ GIO_TRAITS_NEW ],0, 1 );

	if( gGameOptions.fNewTraitSystem )
		ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_NEW ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_OLD ] ]->uiFlags |= BUTTON_CLICKED_ON;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// GAME SETTING ( realistic, sci fi )

	
	guiGameTextTogglesImage[ GIO_TEXT_OFF ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameTextToggles[ GIO_TEXT_OFF ] = CreateIconAndTextButton( guiGameTextTogglesImage[ GIO_TEXT_OFF ], gzGIOScreenText[ GIO_REALISTIC_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GAME_SETTING_X + 74), (GIO_GAME_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOOffStyleCallback);

	guiGameTextTogglesImage[ GIO_TEXT_ON ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameTextToggles[ GIO_TEXT_ON ] = CreateIconAndTextButton( guiGameTextTogglesImage[ GIO_TEXT_ON ],  gzGIOScreenText[ GIO_SCI_FI_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GAME_SETTING_X), (GIO_GAME_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOOnStyleCallback );

	SpecifyButtonSoundScheme( guiGameTextToggles[ GIO_TEXT_OFF ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiGameTextToggles[ GIO_TEXT_ON ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiGameTextToggles[ GIO_TEXT_OFF ],0, 0 );
	MSYS_SetBtnUserData(guiGameTextToggles[ GIO_TEXT_ON ],0, 1 );

	if( gGameUBOptions.fRandomManuelText )
		ButtonList[ guiGameTextToggles[ GIO_TEXT_ON ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else 
		ButtonList[ guiGameTextToggles[ GIO_TEXT_OFF ] ]->uiFlags |= BUTTON_CLICKED_ON;		
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// IRON MAN SETTING

	guiGameSaveTogglesImage[ GIO_CAN_SAVE ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameSaveToggles[ GIO_CAN_SAVE ] = CreateIconAndTextButton( guiGameSaveTogglesImage[ GIO_CAN_SAVE ], gzGIOScreenText[ GIO_SAVE_ANYWHERE_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_IRON_MAN_SETTING_X), (GIO_IRON_MAN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOIronManOffCallback);

	guiGameSaveTogglesImage[ GIO_IRON_MAN ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameSaveToggles[ GIO_IRON_MAN ] = CreateIconAndTextButton( guiGameSaveTogglesImage[ GIO_IRON_MAN ],  gzGIOScreenText[ GIO_IRON_MAN_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_IRON_MAN_SETTING_X + 74), (GIO_IRON_MAN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOIronManOnCallback );

	SpecifyButtonSoundScheme( guiGameSaveToggles[ GIO_CAN_SAVE ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiGameSaveToggles[ GIO_IRON_MAN ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiGameSaveToggles[ GIO_CAN_SAVE ],0, 0 );
	MSYS_SetBtnUserData(guiGameSaveToggles[ GIO_IRON_MAN ],0, 1 );

	if( gGameOptions.fIronManMode )
		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags |= BUTTON_CLICKED_ON;	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// NUMBER OF TERRORISTS SETTING
		
	guiRpcOptionTogglesImage[ GIO_TEX_JOHN_RANDOM ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiRpcOptionToggles[ GIO_TEX_JOHN_RANDOM ] = CreateIconAndTextButton( guiRpcOptionTogglesImage[ GIO_TEX_JOHN_RANDOM ], gzGIOScreenText[ GIO_TERRORISTS_RANDOM_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TERRORISTS_SETTING_X), (GIO_TERRORISTS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIORpcRandomCallback);
	
	guiRpcOptionTogglesImage[ GIO_TEX_AND_JOHN ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiRpcOptionToggles[ GIO_TEX_AND_JOHN ] = CreateIconAndTextButton( guiRpcOptionTogglesImage[ GIO_TEX_AND_JOHN ],  gzGIOScreenText[ GIO_TERRORISTS_ALL_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TERRORISTS_SETTING_X + 74), (GIO_TERRORISTS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIORpcAllCallback );	
																								
	SpecifyButtonSoundScheme( guiRpcOptionToggles[ GIO_TEX_JOHN_RANDOM ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiRpcOptionToggles[ GIO_TEX_AND_JOHN ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiRpcOptionToggles[ GIO_TEX_JOHN_RANDOM ],0, 0 );
	MSYS_SetBtnUserData(guiRpcOptionToggles[ GIO_TEX_AND_JOHN ],0, 1 );

	if( gGameUBOptions.fTexAndJohn )
		ButtonList[ guiRpcOptionToggles[ GIO_TEX_AND_JOHN ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else 
		ButtonList[ guiRpcOptionToggles[ GIO_TEX_JOHN_RANDOM ] ]->uiFlags |= BUTTON_CLICKED_ON;		
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// BOBBY RAY SETTING
	
	giGIOBRSettingButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ], -1,0,-1,1,-1 );
	giGIOBRSettingButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ], -1,2,-1,3,-1 );

	// left button - decrement BR level
	giGIOBRSettingButton[ 0 ] = QuickCreateButton( giGIOBRSettingButtonImage[ 0 ], GIO_BR_SETTING_X + 39, GIO_BR_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOBRSettingLeftCallback );

	// right button - increment BR level
	giGIOBRSettingButton[ 1 ] = QuickCreateButton( giGIOBRSettingButtonImage[ 1 ], GIO_BR_SETTING_X + 158, GIO_BR_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOBRSettingRightCallback );

	// set user data
	MSYS_SetBtnUserData(giGIOBRSettingButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIOBRSettingButton[1],0, 1 );

	// set initial value
	switch ( gGameOptions.ubBobbyRay )
	{
		case BR_GOOD:
			iCurrentBRSetting = GIO_BR_GOOD;
			break;
		case BR_GREAT:
			iCurrentBRSetting = GIO_BR_GREAT;
			break;
		case BR_EXCELLENT:
			iCurrentBRSetting = GIO_BR_EXCELLENT;
			break;
		case BR_AWESOME:
			iCurrentBRSetting = GIO_BR_AWESOME;
			break;
		default:
			iCurrentBRSetting = GIO_BR_GOOD; // optimistically assume, we have selected the normal one
			break;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// ITEM PROGRESS SETTING
	
	giGIOProgressSettingButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ], -1,0,-1,1,-1 );
	giGIOProgressSettingButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ], -1,2,-1,3,-1 );

	// left button - decrement BR level
	giGIOProgressSettingButton[ 0 ] = QuickCreateButton( giGIOProgressSettingButtonImage[ 0 ], GIO_PROGRESS_SETTING_X + 39, GIO_PROGRESS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOProgressSettingLeftCallback );

	// right button - increment BR level
	giGIOProgressSettingButton[ 1 ] = QuickCreateButton( giGIOProgressSettingButtonImage[ 1 ], GIO_PROGRESS_SETTING_X + 158, GIO_PROGRESS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOProgressSettingRightCallback );

	// set user data
	MSYS_SetBtnUserData(giGIOProgressSettingButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIOProgressSettingButton[1],0, 1 );

	// set initial value
	iCurrentProgressSetting = gGameOptions.ubProgressSpeedOfItemsChoices;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// OLD/NEW INVENTORY SETTING

	if (IsNIVModeValid(true) == TRUE )
	{		
		giGIOInventorySettingButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ],-1,0,-1,1,-1 );
		giGIOInventorySettingButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ],-1,2,-1,3,-1 );

		// left button - decrement difficulty level
		giGIOInventorySettingButton[ 0 ] = QuickCreateButton( giGIOInventorySettingButtonImage[ 0 ], GIO_INV_SETTING_X + 39, GIO_INV_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOInventorySettingLeftCallback );

		// right button - increment difficulty level
		giGIOInventorySettingButton[ 1 ] = QuickCreateButton( giGIOInventorySettingButtonImage[ 1 ], GIO_INV_SETTING_X + 158, GIO_INV_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOInventorySettingRightCallback );


		// set user data
		MSYS_SetBtnUserData(giGIOInventorySettingButton[0],0, 0 );
		MSYS_SetBtnUserData(giGIOInventorySettingButton[1],0, 1 );

		if (UsingNewInventorySystem()==true &&
			UsingNewAttachmentSystem()==true)
		{
			iCurrentInventorySetting = GIO_INV_NEW_NAS;
		}
		else
		{
			// set initial value
			switch ( gGameOptions.ubInventorySystem )
			{
				case INVENTORY_OLD:
					iCurrentInventorySetting = GIO_INV_OLD;
					break;
				case INVENTORY_NEW:
					iCurrentInventorySetting = GIO_INV_NEW;
					break;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// DROP ALL ON/OFF SETTING

	guiDropAllOptionTogglesImage[ GIO_DROPALL_OFF ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiDropAllOptionToggles[ GIO_DROPALL_OFF ] = CreateIconAndTextButton( guiDropAllOptionTogglesImage[ GIO_DROPALL_OFF ], gzGIOScreenText[ GIO_DROPALL_OFF_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_DROPALL_SETTING_X), (GIO_DROPALL_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIODropAllOffCallback);

	guiDropAllOptionTogglesImage[ GIO_DROPALL_ON ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiDropAllOptionToggles[ GIO_DROPALL_ON ] = CreateIconAndTextButton( guiDropAllOptionTogglesImage[ GIO_DROPALL_ON ],  gzGIOScreenText[ GIO_DROPALL_ON_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_DROPALL_SETTING_X + 74), (GIO_DROPALL_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIODropAllOnCallback );

	SpecifyButtonSoundScheme( guiDropAllOptionToggles[ GIO_DROPALL_OFF ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiDropAllOptionToggles[ GIO_DROPALL_ON ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiDropAllOptionToggles[ GIO_DROPALL_OFF ],0, 0 );
	MSYS_SetBtnUserData(guiDropAllOptionToggles[ GIO_DROPALL_ON ],0, 1 );

	if( gGameOptions.fEnemiesDropAllItems )
		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_ON ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_OFF ] ]->uiFlags |= BUTTON_CLICKED_ON;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// GUN SETTING

	guiGunOptionTogglesImage[ GIO_REDUCED_GUNS ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGunOptionToggles[ GIO_REDUCED_GUNS ] = CreateIconAndTextButton( guiGunOptionTogglesImage[ GIO_REDUCED_GUNS ], gzGIOScreenText[ GIO_REDUCED_GUNS_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GUN_SETTING_X + 74), (GIO_GUN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOGunSettingReducedCallback);

	guiGunOptionTogglesImage[ GIO_GUN_NUT ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGunOptionToggles[ GIO_GUN_NUT ] = CreateIconAndTextButton( guiGunOptionTogglesImage[ GIO_GUN_NUT ],  gzGIOScreenText[ GIO_GUN_NUT_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GUN_SETTING_X), (GIO_GUN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOGunSettingToGCallback );

	SpecifyButtonSoundScheme( guiGunOptionToggles[ GIO_REDUCED_GUNS ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiGunOptionToggles[ GIO_GUN_NUT ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiGunOptionToggles[ GIO_REDUCED_GUNS ],0, 0 );
	MSYS_SetBtnUserData(guiGunOptionToggles[ GIO_GUN_NUT ],0, 1 );

	if( gGameOptions.fGunNut )
		ButtonList[ guiGunOptionToggles[ GIO_GUN_NUT ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiGunOptionToggles[ GIO_REDUCED_GUNS ] ]->uiFlags |= BUTTON_CLICKED_ON;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// SQUAD SIZE
	if (iResolution > 0)
	{
		giGIOSquadSizeButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
		giGIOSquadSizeButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

		// left button - decrement difficulty level
		giGIOSquadSizeButton[ 0 ] = QuickCreateButton( giGIOSquadSizeButtonImage[ 0 ], GIO_SQUAD_SIZE_SETTING_X + 39, GIO_SQUAD_SIZE_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOSquadSizeSelectionLeftCallback );

		// right button - increment difficulty level
		giGIOSquadSizeButton[ 1 ] = QuickCreateButton( giGIOSquadSizeButtonImage[ 1 ], GIO_SQUAD_SIZE_SETTING_X + 158, GIO_SQUAD_SIZE_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOSquadSizeSelectionRightCallback );
		
		// set user data
		MSYS_SetBtnUserData(giGIOSquadSizeButton[0],0, 0 );
		MSYS_SetBtnUserData(giGIOSquadSizeButton[1],0, 1 );
		
		switch (gGameOptions.ubSquadSize)
		{
			case 6:
				iCurrentSquadSize = GIO_SQUAD_SIZE_6;
				break;
			case 8:
				iCurrentSquadSize = GIO_SQUAD_SIZE_8;
				break;
			case 10:
				iCurrentSquadSize = GIO_SQUAD_SIZE_10;
				break;
		}
	}

	
	//Reset the exit screen
	gubGIOExitScreen = GAME_INIT_OPTIONS_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderGIOScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfGIOButtonsAllocated = TRUE;

	return( TRUE );
}

void BtnGIODifficultySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentDifficulty > 0 )
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentDifficulty > 0 )
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIODifficultySelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{		
		if ( iCurrentDifficulty < 2 )		
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);
		
		if ( iCurrentDifficulty < 2 )		
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOBRSettingLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentBRSetting > 0 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentBRSetting > 0 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOBRSettingRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentBRSetting < 3 )
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentBRSetting < 3 )
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOIMPNumberSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentIMPNumberSetting > 1 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentIMPNumberSetting > 1 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOIMPNumberSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentIMPNumberSetting < (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount) )
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentIMPNumberSetting < (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount) )
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOInventorySettingLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentInventorySetting > GIO_INV_OLD )
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentInventorySetting > GIO_INV_OLD )
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOInventorySettingRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentInventorySetting < GIO_INV_NEW_NAS )
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentInventorySetting < GIO_INV_NEW_NAS )
		{
			PlayButtonSound( giGIOInventorySettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOProgressSettingLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentProgressSetting > 0 )
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentProgressSetting > 0 )
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOProgressSettingRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentProgressSetting < GIO_PROGRESS_VERY_FAST )
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentProgressSetting < GIO_PROGRESS_VERY_FAST )
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOOldTraitsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_TRAITS_SETTING_X), (GIO_TRAITS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_NEW ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiTraitsOptionToggles[ GIO_TRAITS_OLD ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIONewTraitsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if (!gGameExternalOptions.fReadProfileDataFromXML)
		{
			PlayButtonSound( guiTraitsOptionToggles[ GIO_TRAITS_NEW ], BUTTON_SOUND_DISABLED_CLICK );

			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioNewTraitsImpossibleText[0], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_OK, NewTraitsNotPossibleMessageBoxCallBack );
		}
		else
		{
			RestoreExternBackgroundRect( (GIO_TRAITS_SETTING_X), (GIO_TRAITS_SETTING_Y + 10), 230, 40 );

			ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_OLD ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			btn->uiFlags|=(BUTTON_CLICKED_ON);
				
			PlayButtonSound( guiTraitsOptionToggles[ GIO_TRAITS_NEW ], BUTTON_SOUND_CLICKED_ON );
		}
	}
}

void NewTraitsNotPossibleMessageBoxCallBack( UINT8 bExitValue )
{
	/*if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_NOTHING;
	}*/
}

void BtnGIOOffStyleCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GAME_SETTING_X), (GIO_GAME_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameTextToggles[ GIO_TEXT_ON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameTextToggles[ GIO_TEXT_OFF ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOOnStyleCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GAME_SETTING_X), (GIO_GAME_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameTextToggles[ GIO_TEXT_OFF ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameTextToggles[ GIO_TEXT_ON ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOIronManOffCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_IRON_MAN_SETTING_X), (GIO_IRON_MAN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameStyleToggles[ GIO_CAN_SAVE ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOIronManOnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_IRON_MAN_SETTING_X), (GIO_IRON_MAN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameStyleToggles[ GIO_IRON_MAN ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIORpcRandomCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_TERRORISTS_SETTING_X), (GIO_TERRORISTS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiRpcOptionToggles[ GIO_TEX_AND_JOHN ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiRpcOptionToggles[ GIO_TEX_JOHN_RANDOM ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIORpcAllCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_TERRORISTS_SETTING_X), (GIO_TERRORISTS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiRpcOptionToggles[ GIO_TEX_JOHN_RANDOM ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiRpcOptionToggles[ GIO_TEX_AND_JOHN ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIODropAllOffCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_DROPALL_SETTING_X), (GIO_DROPALL_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_ON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiDropAllOptionToggles[ GIO_DROPALL_OFF ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIODropAllOnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_DROPALL_SETTING_X), (GIO_DROPALL_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_OFF ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiDropAllOptionToggles[ GIO_DROPALL_ON ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOGunSettingReducedCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GUN_SETTING_X), (GIO_GUN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGunOptionToggles[ GIO_GUN_NUT ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGunOptionToggles[ GIO_REDUCED_GUNS ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOGunSettingToGCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GUN_SETTING_X), (GIO_GUN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGunOptionToggles[ GIO_REDUCED_GUNS ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGunOptionToggles[ GIO_GUN_NUT ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOSquadSizeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{		
		if ( iCurrentSquadSize > GIO_SQUAD_SIZE_6 )
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentSquadSize > GIO_SQUAD_SIZE_6 )
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOSquadSizeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		UINT8 maxSquadSize = GIO_SQUAD_SIZE_10;
		if (iResolution == 1)
			maxSquadSize = GIO_SQUAD_SIZE_8;

		if ( iCurrentSquadSize < maxSquadSize )
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		UINT8 maxSquadSize = GIO_SQUAD_SIZE_10;
		if (iResolution == 1)
			maxSquadSize = GIO_SQUAD_SIZE_8;

		if ( iCurrentSquadSize < maxSquadSize )
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

BOOLEAN		ExitGIOScreen()
{
	UINT16	cnt;

	if( !gfGIOButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiGIOMainBackGroundImage );

	DeleteVideoObjectFromIndex( guiGIOSMALLFRAME );

	// Destroy Basic buttons
	RemoveButton( guiGIOCancelButton );
	RemoveButton( guiGIODoneButton );
	UnloadButtonImage( giGIOCancelBtnImage );
	UnloadButtonImage( giGIODoneBtnImage );

	// Destroy Difficulty setting buttons
	RemoveButton( giGIODifficultyButton[0] );
	RemoveButton( giGIODifficultyButton[1] );
	UnloadButtonImage( giGIODifficultyButtonImage[0] );
	UnloadButtonImage( giGIODifficultyButtonImage[1] );

	// Destroy IMP Number setting buttons
	RemoveButton( giGIOIMPNumberButton[0] );
	RemoveButton( giGIOIMPNumberButton[1] );
	UnloadButtonImage( giGIOIMPNumberButtonImage[0] );
	UnloadButtonImage( giGIOIMPNumberButtonImage[1] );

	// Destroy BR setting buttons
	RemoveButton( giGIOBRSettingButton[0] );
	RemoveButton( giGIOBRSettingButton[1] );
	UnloadButtonImage( giGIOBRSettingButtonImage[0] );
	UnloadButtonImage( giGIOBRSettingButtonImage[1] );

	// Destroy Progress setting buttons
	RemoveButton( giGIOProgressSettingButton[0] );
	RemoveButton( giGIOProgressSettingButton[1] );
	UnloadButtonImage( giGIOProgressSettingButtonImage[0] );
	UnloadButtonImage( giGIOProgressSettingButtonImage[1] );
	
	// Destroy Traits setting buttons
	for( cnt=0; cnt<NUM_TRAIT_OPTIONS; cnt++)
	{
		RemoveButton( guiTraitsOptionToggles[ cnt ] );
		UnloadButtonImage( guiTraitsOptionTogglesImage[ cnt ] );
	}

	// Destroy Game setting buttons	
	for( cnt=0; cnt<NUM_TEXT_STYLES; cnt++)
	{
		RemoveButton( guiGameTextToggles[ cnt ] );
		UnloadButtonImage( guiGameTextTogglesImage[ cnt ] );
	}
	
	// Destroy Iron Man setting buttons
	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		RemoveButton( guiGameSaveToggles[ cnt ] );
		UnloadButtonImage( guiGameSaveTogglesImage[ cnt ] );
	}

	// Destroy Terrorists setting buttons
	for( cnt=0; cnt<NUM_RPC_UB_OPTIONS; cnt++)
	{
		RemoveButton( guiRpcOptionToggles[ cnt ] );
		UnloadButtonImage( guiRpcOptionTogglesImage[ cnt ] );
	}		

	// Destroy Inventory setting buttons
	if(IsNIVModeValid(true) == TRUE)
	{
		// Destroy Inventory settings
		RemoveButton( giGIOInventorySettingButton[0] );
		RemoveButton( giGIOInventorySettingButton[1] );
		UnloadButtonImage( giGIOInventorySettingButtonImage[0] );
		UnloadButtonImage( giGIOInventorySettingButtonImage[1] );
	}

	// Destroy Drop All setting buttons
	for( cnt=0; cnt<NUM_DROPALL_OPTIONS; cnt++)
	{
		RemoveButton( guiDropAllOptionToggles[ cnt ] );
		UnloadButtonImage( guiDropAllOptionTogglesImage[ cnt ] );
	}

	// Destroy Gun setting buttons
	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		RemoveButton( guiGunOptionToggles[ cnt ] );
		UnloadButtonImage( guiGunOptionTogglesImage[ cnt ] );
	}

	// Destroy Squad size setting buttons
	if (iResolution > 0)
	{
		RemoveButton( giGIOSquadSizeButton[0] );
		RemoveButton( giGIOSquadSizeButton[1] );
		UnloadButtonImage( giGIOSquadSizeButtonImage[0] );
		UnloadButtonImage( giGIOSquadSizeButtonImage[1] );
	}
	
	gfGIOButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	if( gubGameOptionScreenHandler == GIO_EXIT )
		SetMusicMode( MUSIC_NONE );

	gfGIOScreenExit = FALSE;
	gfGIOScreenEntry = TRUE;

	return( TRUE );
}


void HandleGIOScreen()
{
	if( gubGameOptionScreenHandler != GIO_NOTHING )
	{
		switch( gubGameOptionScreenHandler )
		{
			case GIO_CANCEL:
				gubGIOExitScreen = MAINMENU_SCREEN;
				gfGIOScreenExit	= TRUE;
				break;
			
			case MP_LOAD:
				gubGIOExitScreen = SAVE_LOAD_SCREEN;
				gfSaveGame = FALSE;
				gfGIOScreenExit	= TRUE;
				guiPreviousOptionScreen = GAME_INIT_OPTIONS_SCREEN;
				break;

			case GIO_EXIT:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitGameInitOptionScreen )
				{
					//Disable the ok button
					DisableButton( guiGIODoneButton );

					gFadeOutDoneCallback = DoneFadeOutForExitGameInitOptionScreen;

					FadeOutNextFrame( );
				}
				break;
			}

			case GIO_IRON_MAN_MODE:
				DisplayMessageToUserAboutGameDifficulty();
				break;
		}

		gubGameOptionScreenHandler = GIO_NOTHING;
	}


	if( gfReRenderGIOScreen )
	{
		RenderGIOScreen();
		gfReRenderGIOScreen = FALSE;
	}
}


BOOLEAN		RenderGIOScreen()
{
	HVOBJECT	hPixHandle;

	RestoreExternBackgroundRect( GIO_DIF_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_DIF_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_IMP_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_BR_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_BR_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_PROGRESS_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_PROGRESS_SETTING_Y-3, 120, 20 );	
	RestoreExternBackgroundRect( GIO_INV_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_INV_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_SQUAD_SIZE_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_SQUAD_SIZE_SETTING_Y-3, 120, 20 );

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiGIOMainBackGroundImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,0,0, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	//Display the title
	DrawTextToScreen( gzGIOScreenText[ GIO_INITIAL_GAME_SETTINGS ], GIO_MAIN_TITLE_X, GIO_MAIN_TITLE_Y, GIO_MAIN_TITLE_WIDTH, GIO_TITLE_FONT, GIO_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Dif Settings Title Text
	RenderGIOSmallSelectionFrame( (GIO_DIF_SETTING_X + 36), (GIO_DIF_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_DIF_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_DIF_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_DIF_SETTING_Y+6), GIO_DIF_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentDifficulty + 9 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the IMP number Title Text
	RenderGIOSmallSelectionFrame( (GIO_IMP_SETTING_X + 36), (GIO_IMP_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_IMP_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_DIF_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_IMP_NUMBER_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	if ( iCurrentIMPNumberSetting <= 6 )
	{
		DisplayWrappedString( (UINT16)(GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_IMP_SETTING_Y+6), GIO_IMP_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentIMPNumberSetting + 32 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
	else
	{
		CHAR16 sStartLevelString[16];
		swprintf(sStartLevelString, L"%i", iCurrentIMPNumberSetting );
		DisplayWrappedString( (UINT16)(GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_IMP_SETTING_Y+6), GIO_IMP_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, sStartLevelString, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
	// Display BR Setting texts
	RenderGIOSmallSelectionFrame( (GIO_BR_SETTING_X + 36), (GIO_BR_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_BR_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_BR_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_BR_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_BR_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_BR_SETTING_Y+6), GIO_BR_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentBRSetting + 20 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Display Progress Setting texts
	RenderGIOSmallSelectionFrame( (GIO_PROGRESS_SETTING_X + 36), (GIO_PROGRESS_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_PROGRESS_SETTING_X+GIO_OFFSET_TO_TEXT - 6), (UINT16)(GIO_PROGRESS_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_PROGRESS_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_PROGRESS_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_PROGRESS_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_PROGRESS_SETTING_Y+6), GIO_PROGRESS_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentProgressSetting + 49 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Old/new traits system
	DisplayWrappedString( (GIO_TRAITS_SETTING_X - 6), (UINT16)(GIO_TRAITS_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_TRAITS_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TRAITS_TEXT], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Game Settings Title Text
	DisplayWrappedString( (GIO_GAME_SETTING_X - 6), (UINT16)(GIO_GAME_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_GAME_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GAME_STYLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// JA2Gold: Display the iron man Settings Title Text
	DisplayWrappedString( (GIO_IRON_MAN_SETTING_X - 6), (UINT16)(GIO_IRON_MAN_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_IRON_MAN_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GAME_SAVE_STYLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Terrorists Settings Title Text
	DisplayWrappedString( (GIO_TERRORISTS_SETTING_X - 6), (UINT16)(GIO_TERRORISTS_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_TERRORISTS_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TERRORISTS_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Display Inventory Settings text
	RenderGIOSmallSelectionFrame( (GIO_INV_SETTING_X + 36), (GIO_INV_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_INV_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_INV_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_INV_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_INV_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_INV_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_INV_SETTING_Y+5), GIO_INV_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentInventorySetting + 54 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );	

	//Display the Drop All Settings Title Text
	DisplayWrappedString( (GIO_DROPALL_SETTING_X - 6), (UINT16)(GIO_DROPALL_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_DROPALL_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DROPALL_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Gun Settings Title Text
	DisplayWrappedString( (GIO_GUN_SETTING_X - 6), (UINT16)(GIO_GUN_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_GUN_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GUN_OPTIONS_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Squad size Title Text	
	RenderGIOSmallSelectionFrame( (GIO_SQUAD_SIZE_SETTING_X + 36), (GIO_SQUAD_SIZE_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_SQUAD_SIZE_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_SQUAD_SIZE_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_SQUAD_SIZE_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_SQUAD_SIZE_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_SQUAD_SIZE_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_SQUAD_SIZE_SETTING_Y+6), GIO_SQUAD_SIZE_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentSquadSize + (GIO_SQUAD_SIZE_TITLE_TEXT + 1) ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
		
	return( TRUE );
}


void GetGIOScreenUserInput()
{
	InputAtom Event;

	while( DequeueEvent( &Event ) )
	{
		if( Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubGameOptionScreenHandler = GIO_CANCEL;
					break;

#ifdef JA2TESTVERSION
				case 'r':
					gfReRenderGIOScreen=TRUE;
					break;

				case 'i':
					InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
					break;
#endif

				case ENTER:
					gubGameOptionScreenHandler = GIO_EXIT;
					break;
			}
		}
	}
}


void BtnGIODoneCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//if the user doesnt have IRON MAN mode selected
		if( !DisplayMessageToUserAboutIronManMode() )
		{
			//Confirm the difficulty setting
			DisplayMessageToUserAboutGameDifficulty();
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubGameOptionScreenHandler = GIO_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void MPBtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubGameOptionScreenHandler = MP_LOAD;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

UINT8	GetCurrentTraitsOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_TRAIT_OPTIONS; cnt++)
	{
		if( ButtonList[ guiTraitsOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentTextStyleButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_TEXT_STYLES; cnt++)
	{
		if( ButtonList[ guiGameTextToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentGameSaveButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		if( ButtonList[ guiGameSaveToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentTexAndJohnButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_RPC_UB_OPTIONS; cnt++)
	{
		if( ButtonList[ guiRpcOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentDropAllButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GAME_STYLES; cnt++)
	{
		if( ButtonList[ guiDropAllOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentGunButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		if( ButtonList[ guiGunOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentTimedTurnsButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
	{
		if( ButtonList[ guiTimedTurnToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

void DoneFadeOutForExitGameInitOptionScreen( void )
{
	// loop through and get the status of all the buttons
	gGameOptions.fGunNut = GetCurrentGunButtonSetting();
		
	gGameOptions.ubGameStyle = FALSE; 
	gGameUBOptions.fRandomManuelText = GetCurrentTextStyleButtonSetting();
		
	gGameOptions.ubDifficultyLevel = min( NUM_DIFF_SETTINGS-1, ( max( 1, (iCurrentDifficulty + 1)) )); 
	
	gGameOptions.fTurnTimeLimit = FALSE;
		
	// iron man
	gGameOptions.fIronManMode = GetCurrentGameSaveButtonSetting();

	switch ( iCurrentBRSetting )
	{
		case GIO_BR_GOOD:
			gGameOptions.ubBobbyRay = BR_GOOD;
			break;
		case GIO_BR_GREAT:
			gGameOptions.ubBobbyRay = BR_GREAT;
			break;
		case GIO_BR_EXCELLENT:
			gGameOptions.ubBobbyRay = BR_EXCELLENT;
			break;
		case GIO_BR_AWESOME:
			gGameOptions.ubBobbyRay = BR_AWESOME;
			break;
	}

	// CHRISL:
	if(IsNIVModeValid(true) == TRUE)
	{
		switch ( iCurrentInventorySetting)				
		{
			case GIO_INV_OLD:
				gGameOptions.ubInventorySystem = INVENTORY_OLD;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
				break;
			case GIO_INV_NEW:
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
				break;
			case GIO_INV_NEW_NAS:
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_NEW;
		}
	}

	// Squad size
	switch (iCurrentSquadSize)
	{
		case GIO_SQUAD_SIZE_6:
			gGameOptions.ubSquadSize = 6;
			break;
		case GIO_SQUAD_SIZE_8:
			gGameOptions.ubSquadSize = 8;
			break;
		case GIO_SQUAD_SIZE_10:
			gGameOptions.ubSquadSize = 10;
			break;
	}


	// WANNE: Not needed
	/*if (gGameOptions.ubSquadSize > 6)
		gGameExternalOptions.ubGameMaximumNumberOfPlayerMercs = gGameOptions.ubSquadSize;*/

	// SANDRO - added following:
	gGameOptions.ubMaxIMPCharacters = min( (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount), ( max( 1, iCurrentIMPNumberSetting) ));
	gGameOptions.fNewTraitSystem = GetCurrentTraitsOptionButtonSetting();
		
	gGameUBOptions.fTexAndJohn = GetCurrentTexAndJohnButtonSetting();
	
	gGameOptions.fEnemiesDropAllItems = GetCurrentDropAllButtonSetting();	
	gGameOptions.fEnableAllWeaponCaches = FALSE;	
	gGameOptions.ubProgressSpeedOfItemsChoices = min( GIO_PROGRESS_VERY_FAST, iCurrentProgressSetting );
	
	gubGIOExitScreen = INIT_SCREEN;
	
	//set the fact that we should do the intro videos
//	gbIntroScreenMode = INTRO_BEGINNING;
#ifdef JA2TESTVERSION
	if( gfKeyState[ ALT ] )
	{
		if( gfKeyState[ CTRL ] )
		{
			gMercProfiles[ MIGUEL ].bMercStatus = MERC_IS_DEAD;
			gMercProfiles[ SKYRIDER ].bMercStatus = MERC_IS_DEAD;
		}

		SetIntroType( INTRO_ENDING );
	}
	else
#endif
			
	ExitGIOScreen();

	gFadeInDoneCallback = DoneFadeInForExitGameInitOptionScreen;
	FadeInNextFrame( );

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitGameInitOptionScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

BOOLEAN DoGioMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

	// do message box and return
	giGioMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giGioMessageBox != -1 ) );
}

void DisplayMessageToUserAboutGameDifficulty()
{
	UINT8 ubDiffLevel = iCurrentDifficulty;

	switch( ubDiffLevel )
	{
		case 0:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_NOVICE], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 1:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_EXPERIENCED], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 2:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_EXPERT], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 3:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_INSANE], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
	}
}


void	ConfirmGioDifSettingMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_EXIT;
	}
}

BOOLEAN DisplayMessageToUserAboutIronManMode()
{
	// Madd
	UINT8 ubIronManMode = GetCurrentGameSaveButtonSetting(); //FALSE; 

	//if the user has selected IRON MAN mode
	if( ubIronManMode )
	{
		DoGioMessageBox( MSG_BOX_BASIC_STYLE, gzIronManModeWarningText[ IMM__IRON_MAN_MODE_WARNING_TEXT ], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioIronManMessageBoxCallBack );

		return( TRUE );
	}

	return( FALSE );
}

void ConfirmGioIronManMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_IRON_MAN_MODE;
	}
	else
	{
		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void RenderGIOSmallSelectionFrame(INT16 sX, INT16 sY)
{

	HVOBJECT hHandle;

	// get the video object
	GetVideoObject(&hHandle, guiGIOSMALLFRAME);

	// blt to sX, sY relative to upper left corner
	BltVideoObject(FRAME_BUFFER, hHandle, 0, sX, sY , VO_BLT_SRCTRANSPARENCY,NULL);

	return;
}

#else

// *********************************
// JA2 - 1.13: Initial Game Screen
// *********************************

//////////////////////////////////////////////////////////////
//	SANDRO - the start-new-game screen has been changed a lot
//////////////////////////////////////////////////////////////

////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		GIO_TITLE_FONT							FONT14ARIAL
#define		GIO_TITLE_COLOR							FONT_MCOLOR_WHITE

#define		GIO_TOGGLE_TEXT_FONT					FONT10ARIAL
#define		GIO_TOGGLE_TEXT_COLOR					FONT_MCOLOR_WHITE

//buttons
#define		GIO_CANCEL_X							iScreenWidthOffset + ((320 - 115) / 2)  

#define		GIO_BTN_START_X							iScreenWidthOffset + 320 + 105
#define		GIO_BTN_START_Y							iScreenHeightOffset + 435
 

//main title
#define		GIO_MAIN_TITLE_X						0
#define		GIO_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		GIO_MAIN_TITLE_WIDTH					SCREEN_WIDTH

//radio box locations
#define		GIO_GAP_BN_SETTINGS						35
#define		GIO_OFFSET_TO_TEXT						36
#define		GIO_OFFSET_TO_TOGGLE_BOX				180
#define		GIO_OFFSET_TO_TOGGLE_BOX_Y				9

#define		GIO_TITLE_DISTANCE						30 // higher means closer

#define		GIO_OFFSET_TO_TEXT						36
#define		GIO_OFFSET_TO_TOGGLE_BOX				180

// ---------------------------------
// Y-Offset for Combo-Controls
#define		COMBO_Y_OFFSET							63
// Y-Offset for Check-Controls
#define		CHECK_Y_OFFSET							50

#define		CORRECTION_Y_OFFSET						(COMBO_Y_OFFSET - CHECK_Y_OFFSET)

// X-Offset for Checkbox-Controls
#define		CHECK_X_OFFSET							36
#define		COMBO_X_OFFSET							0

#define		CHECK_WIDTH								(GIO_OFFSET_TO_TOGGLE_BOX - GIO_OFFSET_TO_TEXT)
#define		COMBO_WIDTH								(GIO_OFFSET_TO_TOGGLE_BOX - GIO_OFFSET_TO_TEXT)


/*********************************
	FIRST COLUMN
**********************************/

#define		FIRST_COLUMN_X							iScreenWidthOffset + 10
#define		FIRST_COLUMN_Y							iScreenHeightOffset + 75

#define		GIO_DIF_SETTING_X						FIRST_COLUMN_X + COMBO_X_OFFSET
#define		GIO_DIF_SETTING_Y						FIRST_COLUMN_Y
#define		GIO_DIF_SETTING_WIDTH					COMBO_WIDTH

#define		GIO_IMP_SETTING_X						FIRST_COLUMN_X + COMBO_X_OFFSET
#define		GIO_IMP_SETTING_Y						GIO_DIF_SETTING_Y + COMBO_Y_OFFSET
#define		GIO_IMP_SETTING_WIDTH					COMBO_WIDTH

#define		GIO_TRAITS_SETTING_X					FIRST_COLUMN_X + CHECK_X_OFFSET
#define		GIO_TRAITS_SETTING_Y					GIO_IMP_SETTING_Y + CHECK_Y_OFFSET
#define		GIO_TRAITS_SETTING_WIDTH				CHECK_WIDTH

#define		GIO_GAME_SETTING_X						FIRST_COLUMN_X + CHECK_X_OFFSET
#define		GIO_GAME_SETTING_Y						GIO_TRAITS_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		GIO_GAME_SETTING_WIDTH					CHECK_WIDTH

#define		GIO_IRON_MAN_SETTING_X					FIRST_COLUMN_X + CHECK_X_OFFSET
#define		GIO_IRON_MAN_SETTING_Y					GIO_GAME_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		GIO_IRON_MAN_SETTING_WIDTH				CHECK_WIDTH

#define		GIO_TERRORISTS_SETTING_X				FIRST_COLUMN_X + CHECK_X_OFFSET
#define		GIO_TERRORISTS_SETTING_Y				GIO_IRON_MAN_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		GIO_TERRORISTS_SETTING_WIDTH			CHECK_WIDTH


/*********************************
	SECOND COLUMN
**********************************/

#define		SECOND_COLUMN_X							iScreenWidthOffset + 210
#define		SECOND_COLUMN_Y							FIRST_COLUMN_Y

#define		GIO_BR_SETTING_X						SECOND_COLUMN_X + COMBO_X_OFFSET
#define		GIO_BR_SETTING_Y						SECOND_COLUMN_Y
#define		GIO_BR_SETTING_WIDTH					COMBO_WIDTH

#define		GIO_PROGRESS_SETTING_X					SECOND_COLUMN_X + COMBO_X_OFFSET
#define		GIO_PROGRESS_SETTING_Y					GIO_BR_SETTING_Y + COMBO_Y_OFFSET
#define		GIO_PROGRESS_SETTING_WIDTH				COMBO_WIDTH

#define		GIO_INV_SETTING_X						SECOND_COLUMN_X + COMBO_X_OFFSET
#define		GIO_INV_SETTING_Y						GIO_PROGRESS_SETTING_Y + COMBO_Y_OFFSET
#define		GIO_INV_SETTING_WIDTH					COMBO_WIDTH

#define		GIO_DROPALL_SETTING_X					SECOND_COLUMN_X + CHECK_X_OFFSET
#define		GIO_DROPALL_SETTING_Y					GIO_INV_SETTING_Y + CHECK_Y_OFFSET
#define		GIO_DROPALL_SETTING_WIDTH				CHECK_WIDTH

#define		GIO_GUN_SETTING_X						SECOND_COLUMN_X + CHECK_X_OFFSET
#define		GIO_GUN_SETTING_Y						GIO_DROPALL_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		GIO_GUN_SETTING_WIDTH					CHECK_WIDTH

#define		GIO_CACHES_SETTING_X					SECOND_COLUMN_X + CHECK_X_OFFSET
#define		GIO_CACHES_SETTING_Y					GIO_GUN_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		GIO_CACHES_SETTING_WIDTH				CHECK_WIDTH

/*********************************
	THIRD COLUMN
**********************************/

#define		THIRD_COLUMN_X							iScreenWidthOffset + 413
#define		THIRD_COLUMN_Y							FIRST_COLUMN_Y

#define		GIO_SQUAD_SIZE_SETTING_X				THIRD_COLUMN_X + COMBO_X_OFFSET
#define		GIO_SQUAD_SIZE_SETTING_Y				THIRD_COLUMN_Y
#define		GIO_SQUAD_SIZE_SETTING_WIDTH			COMBO_WIDTH





// INI File
#define		JA2SP_INI_FILENAME						"ja2_sp.ini"

// INI Section
#define		JA2SP_INI_INITIAL_SECTION				"JA2 Singleplayer Initial Settings"

// INI Properties
#define		JA2SP_DIFFICULTY_LEVEL					"DIFFICULTY_LEVEL"
#define		JA2SP_BOBBY_RAY_SELECTION				"BOBBY_RAY_SELECTION"
#define		JA2SP_MAX_IMP_CHARACTERS				"MAX_IMP_CHARACTERS"
#define		JA2SP_PROGRESS_SPEED_OF_ITEM_CHOICES	"PROGRESS_SPEED_OF_ITEM_CHOICES"
#define		JA2SP_SKILL_TRAITS						"SKILL_TRAITS"
#define		JA2SP_INVENTORY_ATTACHMENTS				"INVENTORY_ATTACHMENTS"
#define		JA2SP_GAME_STYLE						"GAME_STYLE"
#define		JA2SP_ENEMIES_DROP_ALL_ITEMS			"ENEMIES_DROP_ALL_ITEMS"
#define		JA2SP_EXTRA_DIFFICULTY					"EXTRA_DIFFICULTY"
#define		JA2SP_AVAILABLE_ARSENAL					"AVAILABLE_ARSENAL"
#define		JA2SP_NUMBER_OF_TERRORISTS				"NUMBER_OF_TERRORISTS"
#define		JA2SP_SECRET_WEAPON_CACHES				"SECRET_WEAPON_CACHES"
#define		JA2SP_SQUAD_SIZE						"SQUAD_SIZE"


//Difficulty settings
enum
{
	GIO_DIFF_EASY,
	GIO_DIFF_MED,
	GIO_DIFF_HARD,
	GIO_DIFF_INSANE,

	NUM_DIFF_SETTINGS,
};

enum
{
	GIO_TRAITS_OLD,
	GIO_TRAITS_NEW,

	NUM_TRAIT_OPTIONS,
};

// Game Settings options
enum
{
	GIO_REALISTIC,
	GIO_SCI_FI,	

	NUM_GAME_STYLES,
};

// Iron man mode
enum
{
	GIO_CAN_SAVE,
	GIO_IRON_MAN,

	NUM_SAVE_OPTIONS,
};

enum
{
	GIO_TERRORISTS_RANDOM,
	GIO_TERRORISTS_ALL,

	NUM_TERRORISTS_OPTIONS,
};

// BR options
enum
{
	GIO_BR_GOOD,
	GIO_BR_GREAT,
	GIO_BR_EXCELLENT,
	GIO_BR_AWESOME,

	NUM_BR_OPTIONS,
};

enum
{
	GIO_PROGRESS_VERY_SLOW,
	GIO_PROGRESS_SLOW,
	GIO_PROGRESS_NORMAL,
	GIO_PROGRESS_FAST,
	GIO_PROGRESS_VERY_FAST,

	NUM_PROGRESS_OPTIONS,
};

// New inventory options
enum
{
	GIO_INV_OLD,
	GIO_INV_NEW,
	GIO_INV_NEW_NAS,	// WANNE: Added NAS to the start new game screen

	NUM_INV_OPTIONS,
};

enum
{
	GIO_SQUAD_SIZE_6,
	GIO_SQUAD_SIZE_8,
	GIO_SQUAD_SIZE_10,

	NUM_SQUAD_SIZE_OPTIONS,
};

enum
{
	GIO_DROPALL_OFF,
	GIO_DROPALL_ON,

	NUM_DROPALL_OPTIONS,
};

// Gun options
enum
{
	GIO_REDUCED_GUNS,
	GIO_GUN_NUT,

	NUM_GUN_OPTIONS,
};

enum
{
	GIO_CACHES_RANDOM,
	GIO_CACHES_ALL,

	NUM_CACHES_OPTIONS,
};


// Timed turns setting (Multiplayer exclusive)
enum
{
	GIO_NO_TIMED_TURNS,
	GIO_TIMED_TURNS,

	GIO_NUM_TIMED_TURN_OPTIONS,
};


//enum for different states of game
enum
{
	GIO_NOTHING,
	GIO_CANCEL,
	GIO_EXIT,
	GIO_IRON_MAN_MODE,
	MP_LOAD
};

////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfGIOScreenEntry = TRUE;
BOOLEAN		gfGIOScreenExit	= FALSE;
BOOLEAN		gfReRenderGIOScreen=TRUE;
BOOLEAN		gfGIOButtonsAllocated = FALSE;

UINT8		gubGameOptionScreenHandler=GIO_NOTHING;

UINT32		gubGIOExitScreen = GAME_INIT_OPTIONS_SCREEN;

UINT32		guiGIOMainBackGroundImage;

INT32		giGioMessageBox = -1;

INT8 iCurrentDifficulty;
INT8 iCurrentBRSetting;
INT8 iCurrentIMPNumberSetting;
INT8 iCurrentProgressSetting;
INT8 iCurrentInventorySetting;
INT8 iCurrentSquadSize;


UINT32 guiGIOSMALLFRAME;

// Done Button
void BtnGIODoneCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiGIODoneButton;
INT32		giGIODoneBtnImage;

// Cancel Button
void BtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiGIOCancelButton;
INT32		giGIOCancelBtnImage;

// MP LOAD Button
void MPBtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	MPguiGIOCancelButton;
INT32		MPgiGIOCancelBtnImage;

UINT32 giGIODifficultyButton[ 2 ];
INT32 giGIODifficultyButtonImage[ 2 ];
void BtnGIODifficultySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIODifficultySelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOIMPNumberButton[ 2 ];
INT32 giGIOIMPNumberButtonImage[ 2 ];
void BtnGIOIMPNumberSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOIMPNumberSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOBRSettingButton[ 2 ];
INT32 giGIOBRSettingButtonImage[ 2 ];
void BtnGIOBRSettingLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOBRSettingRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOProgressSettingButton[ 2 ];
INT32 giGIOProgressSettingButtonImage[ 2 ];
void BtnGIOProgressSettingLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOProgressSettingRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32 giGIOInventorySettingButton [ 2 ];
INT32 giGIOInventorySettingButtonImage [ 2 ];
void BtnGIOInventorySettingLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOInventorySettingRightCallback( GUI_BUTTON *btn,INT32 reason );

UINT32	guiTraitsOptionTogglesImage[ NUM_TRAIT_OPTIONS ];
UINT32	guiTraitsOptionToggles[ NUM_TRAIT_OPTIONS ];
void BtnGIOOldTraitsCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIONewTraitsCallback(GUI_BUTTON *btn,INT32 reason);
void NewTraitsNotPossibleMessageBoxCallBack( UINT8 bExitValue );

//checkbox to toggle Game style
UINT32	guiGameStyleTogglesImage[ NUM_GAME_STYLES ];
UINT32	guiGameStyleToggles[ NUM_GAME_STYLES ];
void BtnGIORealisticStyleCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOScifiStyleCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Save style
UINT32	guiGameSaveTogglesImage[ NUM_SAVE_OPTIONS ];
UINT32	guiGameSaveToggles[ NUM_SAVE_OPTIONS ];
void BtnGIOIronManOffCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOIronManOnCallback(GUI_BUTTON *btn,INT32 reason);

UINT32	guiTerroristsOptionTogglesImage[ NUM_TERRORISTS_OPTIONS ];
UINT32	guiTerroristsOptionToggles[ NUM_TERRORISTS_OPTIONS ];
void BtnGIOTerroristsRandomCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOTerroristsAllCallback(GUI_BUTTON *btn,INT32 reason);

UINT32	guiDropAllOptionTogglesImage[ NUM_DROPALL_OPTIONS ];
UINT32	guiDropAllOptionToggles[ NUM_DROPALL_OPTIONS ];
void BtnGIODropAllOffCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIODropAllOnCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Gun options
UINT32	guiGunOptionTogglesImage[ NUM_GUN_OPTIONS ];
UINT32	guiGunOptionToggles[ NUM_GUN_OPTIONS ];
void BtnGIOGunSettingReducedCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOGunSettingToGCallback(GUI_BUTTON *btn,INT32 reason);

UINT32	guiWeaponCachesOptionTogglesImage[ NUM_CACHES_OPTIONS ];
UINT32	guiWeaponCachesOptionToggles[ NUM_CACHES_OPTIONS ];
void BtnGIOWeaponCachesRandomCallback(GUI_BUTTON *btn,INT32 reason);
void BtnGIOWeaponCachesAllCallback(GUI_BUTTON *btn,INT32 reason);

UINT32 giGIOSquadSizeButton[ 2 ];
INT32 giGIOSquadSizeButtonImage[ 2 ];
void BtnGIOSquadSizeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnGIOSquadSizeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );



UINT32	guiTimedTurnToggles[ GIO_NUM_TIMED_TURN_OPTIONS ];
void BtnTimedTurnsTogglesCallback(GUI_BUTTON *btn,INT32 reason);

void RenderGIOSmallSelectionFrame(INT16 sX, INT16 sY);

////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern		void ClearMainMenu();

BOOLEAN		EnterGIOScreen();
BOOLEAN		ExitGIOScreen();
void			HandleGIOScreen();
BOOLEAN		RenderGIOScreen();
void			GetGIOScreenUserInput();
UINT8			GetCurrentGunButtonSetting();
// JA2Gold: added save (iron man) button setting
UINT8			GetCurrentGameSaveButtonSetting();
UINT8			GetCurrentGameStyleButtonSetting();

// SANDRO - added following
UINT8	GetCurrentTraitsOptionButtonSetting();
UINT8	GetCurrentDropAllButtonSetting();
UINT8	GetCurrentTerroristsButtonSetting();
UINT8	GetCurrentWeaponCachesButtonSetting();

void		DoneFadeOutForExitGameInitOptionScreen( void );
void		DoneFadeInForExitGameInitOptionScreen( void );
UINT8		GetCurrentTimedTurnsButtonSetting();
BOOLEAN		DoGioMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void		DisplayMessageToUserAboutGameDifficulty();
void		ConfirmGioDifSettingMessageBoxCallBack( UINT8 bExitValue );
BOOLEAN		DisplayMessageToUserAboutIronManMode();
BOOLEAN		DisplayMessageToUserAboutOIVandNASincompatibility();
void		ConfirmGioIronManMessageBoxCallBack( UINT8 bExitValue );

BOOLEAN SpIniExists()
{
	BOOLEAN exists = TRUE;

	if(!getVFS()->fileExists(JA2SP_INI_FILENAME))	
		exists = FALSE;		
	
	return exists;
}

UINT32	GameInitOptionsScreenInit( void )
{
	memset( &gGameOptions, 0, sizeof( GAME_OPTIONS ) );

	// WANNE: Read initial game settings from ja2_sp.ini
	vfs::PropertyContainer props;

	if (SpIniExists())
		props.initFromIniFile(JA2SP_INI_FILENAME);

	// Difficulty Level (Default: Experienced = 1)
	gGameOptions.ubDifficultyLevel =  ((UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_DIFFICULTY_LEVEL, 1)) + 1;

	// Bobby Ray's Selection (Default: Great = 1)
	UINT8 ubBobbyRay = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_BOBBY_RAY_SELECTION, 1);
	switch (ubBobbyRay)
	{
		// Normal
		case 0:
			gGameOptions.ubBobbyRay =  BR_GOOD;
			break;
		// Great
		case 1:
			gGameOptions.ubBobbyRay =  BR_GREAT;
			break;
		// Excellent
		case 2:
			gGameOptions.ubBobbyRay =  BR_EXCELLENT;
			break;
		// Awesome
		case 3:
			gGameOptions.ubBobbyRay =  BR_AWESOME;
			break;
	}

	// Max. IMP Characters
	UINT8 maxIMPCharacterCount = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_MAX_IMP_CHARACTERS, 1);
	gGameOptions.ubMaxIMPCharacters = min( (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount), ( max( 1, maxIMPCharacterCount) ));

	// Progress Speed of Item Choices (Default: Normal)
	gGameOptions.ubProgressSpeedOfItemsChoices =  (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_PROGRESS_SPEED_OF_ITEM_CHOICES, ITEM_PROGRESS_NORMAL);

	// Skill Traits
	UINT8 ubTraitSystem = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_SKILL_TRAITS, 1);
	if (!gGameExternalOptions.fReadProfileDataFromXML)
		ubTraitSystem = 0;

	gGameOptions.fNewTraitSystem = ubTraitSystem;

	// Inventory Attachments (Default: New/New = 2)
	UINT8 ubInventoryAttachmentSystem = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_INVENTORY_ATTACHMENTS, 2);
	
	// NIV is not allowed
	if (!IsNIVModeValid(true))
		ubInventoryAttachmentSystem = 0;

	switch (ubInventoryAttachmentSystem)
	{
		// Old / Old
		case 0:
			gGameOptions.ubInventorySystem	= INVENTORY_OLD;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
			break;
		// New / Old
		case 1:
			gGameOptions.ubInventorySystem	= INVENTORY_NEW;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
			break;
		// New / New
		case 2:
			gGameOptions.ubInventorySystem	= INVENTORY_NEW;
			gGameOptions.ubAttachmentSystem = ATTACHMENT_NEW;
			break;
	}

	// Game Style (Default: Sci Fi = 0)
	gGameOptions.ubGameStyle =  (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_GAME_STYLE, STYLE_SCIFI);

	// Enemies Drop All Items (Default: Off = 0)
	gGameOptions.fEnemiesDropAllItems =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_ENEMIES_DROP_ALL_ITEMS, 0);

	// Extra Difficulty (Default: Save Anytime = 0)
	gGameOptions.fIronManMode =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_EXTRA_DIFFICULTY, 0);

	// Available Arsenal (Default: Tons of Guns = 1)
	gGameOptions.fGunNut =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_AVAILABLE_ARSENAL, 1);

	// Number of Terrorists (Default: Random = 0)
	gGameOptions.fEnableAllTerrorists =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_NUMBER_OF_TERRORISTS, 0);

	// Secret Weapon Caches (Default: Random = 0)
	gGameOptions.fEnableAllWeaponCaches =  (BOOLEAN)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_SECRET_WEAPON_CACHES, 0);

	// Squad size (Default: 8)
	UINT8 ubSquadSize = (UINT8)props.getIntProperty(JA2SP_INI_INITIAL_SECTION, JA2SP_SQUAD_SIZE, 8);
	
	if (iResolution == 0)
		ubSquadSize = 6;

	gGameOptions.ubSquadSize = ubSquadSize;
	
	// Air strikes
	gGameOptions.fAirStrikes =  FALSE;

	// Turn limit
	gGameOptions.fTurnTimeLimit	= FALSE;

	return (1);
}

UINT32	GameInitOptionsScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfGIOScreenEntry )
	{
		GameInitOptionsScreenInit();

		EnterGIOScreen();
		gfGIOScreenEntry = FALSE;
		gfGIOScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	GetGIOScreenUserInput();

	HandleGIOScreen();

	// render buttons marked dirty
	MarkButtonsDirty( );
	RenderButtons( );

	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	if ( HandleFadeOutCallback( ) )
	{
		ClearMainMenu();
		return( gubGIOExitScreen );
	}

	if ( HandleBeginFadeOut( gubGIOExitScreen ) )
	{
		return( gubGIOExitScreen );
	}

	if( gfGIOScreenExit )
	{
		ExitGIOScreen();
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderGIOScreen();
	}

	if ( HandleBeginFadeIn( gubGIOExitScreen ) )
	{
	}

	return( gubGIOExitScreen );
}


UINT32	GameInitOptionsScreenShutdown( void )
{
	return( 1 );
}


BOOLEAN		EnterGIOScreen()
{
	VOBJECT_DESC	VObjectDesc, VObjectDesc2;
	
	if( gfGIOButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

	// load the Main trade screen backgroiund image
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution == 0)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 1)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_800x600.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 2)
	{
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_1024x768.sti", VObjectDesc.ImageFile);
	}

	CHECKF(AddVideoObject(&VObjectDesc, &guiGIOMainBackGroundImage ));

	VObjectDesc2.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\GIOSmallFrame.sti", VObjectDesc2.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc2, &guiGIOSMALLFRAME));

	//Ok button
	giGIODoneBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiGIODoneButton = CreateIconAndTextButton( giGIODoneBtnImage, gzGIOScreenText[GIO_START_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													GIO_BTN_START_X, GIO_BTN_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIODoneCallback);

	SpecifyButtonSoundScheme( guiGIODoneButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiGIODoneButton, DISABLED_STYLE_NONE );

	//Cancel button
	giGIOCancelBtnImage = UseLoadedButtonImage( giGIODoneBtnImage, -1,1,-1,3,-1 );
	guiGIOCancelButton = CreateIconAndTextButton( giGIOCancelBtnImage, gzGIOScreenText[GIO_CANCEL_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													GIO_CANCEL_X, GIO_BTN_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOCancelCallback );
	SpecifyButtonSoundScheme( guiGIOCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIFFICULTY SETTING

	giGIODifficultyButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giGIODifficultyButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giGIODifficultyButton[ 0 ] = QuickCreateButton( giGIODifficultyButtonImage[ 0 ], GIO_DIF_SETTING_X + 39, GIO_DIF_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIODifficultySelectionLeftCallback );

	// right button - increment difficulty level
	giGIODifficultyButton[ 1 ] = QuickCreateButton( giGIODifficultyButtonImage[ 1 ], GIO_DIF_SETTING_X + 158, GIO_DIF_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIODifficultySelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giGIODifficultyButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIODifficultyButton[1],0, 1 );

	iCurrentDifficulty = max( 0, gGameOptions.ubDifficultyLevel - 1);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// MAX IMP NUMBER SETTING

	giGIOIMPNumberButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ],-1,0,-1,1,-1 );
	giGIOIMPNumberButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ],-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giGIOIMPNumberButton[ 0 ] = QuickCreateButton( giGIOIMPNumberButtonImage[ 0 ], GIO_IMP_SETTING_X + 39, GIO_IMP_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOIMPNumberSelectionLeftCallback );

	// right button - increment difficulty level
	giGIOIMPNumberButton[ 1 ] = QuickCreateButton( giGIOIMPNumberButtonImage[ 1 ], GIO_IMP_SETTING_X + 158, GIO_IMP_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOIMPNumberSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giGIOIMPNumberButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIOIMPNumberButton[1],0, 1 );

	iCurrentIMPNumberSetting = gGameOptions.ubMaxIMPCharacters;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// OLD/NEW TARITS SETTING

	guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiTraitsOptionToggles[ GIO_TRAITS_OLD ] = CreateIconAndTextButton( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], gzGIOScreenText[ GIO_TRAITS_OLD_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TRAITS_SETTING_X + 74), (GIO_TRAITS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOOldTraitsCallback);

	guiTraitsOptionTogglesImage[ GIO_TRAITS_NEW ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiTraitsOptionToggles[ GIO_TRAITS_NEW ] = CreateIconAndTextButton( guiTraitsOptionTogglesImage[ GIO_TRAITS_NEW ],  gzGIOScreenText[ GIO_TRAITS_NEW_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TRAITS_SETTING_X), (GIO_TRAITS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIONewTraitsCallback );

	SpecifyButtonSoundScheme( guiTraitsOptionToggles[ GIO_TRAITS_OLD ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiTraitsOptionToggles[ GIO_TRAITS_NEW ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiTraitsOptionToggles[ GIO_TRAITS_OLD ],0, 0 );
	MSYS_SetBtnUserData(guiTraitsOptionToggles[ GIO_TRAITS_NEW ],0, 1 );

	if( gGameOptions.fNewTraitSystem )
		ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_NEW ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_OLD ] ]->uiFlags |= BUTTON_CLICKED_ON;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// GAME SETTING ( realistic, sci fi )

	guiGameStyleTogglesImage[ GIO_REALISTIC ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameStyleToggles[ GIO_REALISTIC ] = CreateIconAndTextButton( guiGameStyleTogglesImage[ GIO_REALISTIC ], gzGIOScreenText[ GIO_REALISTIC_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GAME_SETTING_X + 74), (GIO_GAME_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIORealisticStyleCallback);

	guiGameStyleTogglesImage[ GIO_SCI_FI ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameStyleToggles[ GIO_SCI_FI ] = CreateIconAndTextButton( guiGameStyleTogglesImage[ GIO_SCI_FI ],  gzGIOScreenText[ GIO_SCI_FI_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GAME_SETTING_X), (GIO_GAME_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOScifiStyleCallback );

	SpecifyButtonSoundScheme( guiGameStyleToggles[ GIO_REALISTIC ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiGameStyleToggles[ GIO_SCI_FI ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiGameStyleToggles[ GIO_REALISTIC ],0, 0 );
	MSYS_SetBtnUserData(guiGameStyleToggles[ GIO_SCI_FI ],0, 1 );

	if( gGameOptions.ubGameStyle == STYLE_SCIFI )
		ButtonList[ guiGameStyleToggles[ GIO_SCI_FI ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else 
		ButtonList[ guiGameStyleToggles[ GIO_REALISTIC ] ]->uiFlags |= BUTTON_CLICKED_ON;		

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// IRON MAN SETTING

	guiGameSaveTogglesImage[ GIO_CAN_SAVE ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameSaveToggles[ GIO_CAN_SAVE ] = CreateIconAndTextButton( guiGameSaveTogglesImage[ GIO_CAN_SAVE ], gzGIOScreenText[ GIO_SAVE_ANYWHERE_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_IRON_MAN_SETTING_X), (GIO_IRON_MAN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOIronManOffCallback);

	guiGameSaveTogglesImage[ GIO_IRON_MAN ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGameSaveToggles[ GIO_IRON_MAN ] = CreateIconAndTextButton( guiGameSaveTogglesImage[ GIO_IRON_MAN ],  gzGIOScreenText[ GIO_IRON_MAN_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_IRON_MAN_SETTING_X + 74), (GIO_IRON_MAN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOIronManOnCallback );

	SpecifyButtonSoundScheme( guiGameSaveToggles[ GIO_CAN_SAVE ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiGameSaveToggles[ GIO_IRON_MAN ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiGameSaveToggles[ GIO_CAN_SAVE ],0, 0 );
	MSYS_SetBtnUserData(guiGameSaveToggles[ GIO_IRON_MAN ],0, 1 );

	if( gGameOptions.fIronManMode )
		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags |= BUTTON_CLICKED_ON;	


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// NUMBER OF TERRORISTS SETTING

	guiTerroristsOptionTogglesImage[ GIO_TERRORISTS_RANDOM ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiTerroristsOptionToggles[ GIO_TERRORISTS_RANDOM ] = CreateIconAndTextButton( guiTerroristsOptionTogglesImage[ GIO_TERRORISTS_RANDOM ], gzGIOScreenText[ GIO_TERRORISTS_RANDOM_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TERRORISTS_SETTING_X), (GIO_TERRORISTS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOTerroristsRandomCallback);

	guiTerroristsOptionTogglesImage[ GIO_TERRORISTS_ALL ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiTerroristsOptionToggles[ GIO_TERRORISTS_ALL ] = CreateIconAndTextButton( guiTerroristsOptionTogglesImage[ GIO_TERRORISTS_ALL ],  gzGIOScreenText[ GIO_TERRORISTS_ALL_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_TERRORISTS_SETTING_X + 74), (GIO_TERRORISTS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOTerroristsAllCallback );

	SpecifyButtonSoundScheme( guiTerroristsOptionToggles[ GIO_TERRORISTS_RANDOM ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiTerroristsOptionToggles[ GIO_TERRORISTS_ALL ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiTerroristsOptionToggles[ GIO_TERRORISTS_RANDOM ],0, 0 );
	MSYS_SetBtnUserData(guiTerroristsOptionToggles[ GIO_TERRORISTS_ALL ],0, 1 );

	if( gGameOptions.fEnableAllTerrorists )
		ButtonList[ guiTerroristsOptionToggles[ GIO_TERRORISTS_ALL ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else 
		ButtonList[ guiTerroristsOptionToggles[ GIO_TERRORISTS_RANDOM ] ]->uiFlags |= BUTTON_CLICKED_ON;		

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// BOBBY RAY SETTING
	
	giGIOBRSettingButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ], -1,0,-1,1,-1 );
	giGIOBRSettingButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ], -1,2,-1,3,-1 );

	// left button - decrement BR level
	giGIOBRSettingButton[ 0 ] = QuickCreateButton( giGIOBRSettingButtonImage[ 0 ], GIO_BR_SETTING_X + 39, GIO_BR_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOBRSettingLeftCallback );

	// right button - increment BR level
	giGIOBRSettingButton[ 1 ] = QuickCreateButton( giGIOBRSettingButtonImage[ 1 ], GIO_BR_SETTING_X + 158, GIO_BR_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOBRSettingRightCallback );

	// set user data
	MSYS_SetBtnUserData(giGIOBRSettingButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIOBRSettingButton[1],0, 1 );

	// set initial value
	switch ( gGameOptions.ubBobbyRay )
	{
		case BR_GOOD:
			iCurrentBRSetting = GIO_BR_GOOD;
			break;
		case BR_GREAT:
			iCurrentBRSetting = GIO_BR_GREAT;
			break;
		case BR_EXCELLENT:
			iCurrentBRSetting = GIO_BR_EXCELLENT;
			break;
		case BR_AWESOME:
			iCurrentBRSetting = GIO_BR_AWESOME;
			break;
		default:
			iCurrentBRSetting = GIO_BR_GOOD; // optimistically assume, we have selected the normal one
			break;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// ITEM PROGRESS SETTING
	
	giGIOProgressSettingButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ], -1,0,-1,1,-1 );
	giGIOProgressSettingButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ], -1,2,-1,3,-1 );

	// left button - decrement BR level
	giGIOProgressSettingButton[ 0 ] = QuickCreateButton( giGIOProgressSettingButtonImage[ 0 ], GIO_PROGRESS_SETTING_X + 39, GIO_PROGRESS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOProgressSettingLeftCallback );

	// right button - increment BR level
	giGIOProgressSettingButton[ 1 ] = QuickCreateButton( giGIOProgressSettingButtonImage[ 1 ], GIO_PROGRESS_SETTING_X + 158, GIO_PROGRESS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOProgressSettingRightCallback );

	// set user data
	MSYS_SetBtnUserData(giGIOProgressSettingButton[0],0, 0 );
	MSYS_SetBtnUserData(giGIOProgressSettingButton[1],0, 1 );

	// set initial value
	iCurrentProgressSetting = gGameOptions.ubProgressSpeedOfItemsChoices;

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// OLD/NEW INVENTORY SETTING

	if (IsNIVModeValid(true) == TRUE )
	{		
		giGIOInventorySettingButtonImage[ 0 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 0 ],-1,0,-1,1,-1 );
		giGIOInventorySettingButtonImage[ 1 ]=	UseLoadedButtonImage( giGIODifficultyButtonImage[ 1 ],-1,2,-1,3,-1 );

		// left button - decrement difficulty level
		giGIOInventorySettingButton[ 0 ] = QuickCreateButton( giGIOInventorySettingButtonImage[ 0 ], GIO_INV_SETTING_X + 39, GIO_INV_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOInventorySettingLeftCallback );

		// right button - increment difficulty level
		giGIOInventorySettingButton[ 1 ] = QuickCreateButton( giGIOInventorySettingButtonImage[ 1 ], GIO_INV_SETTING_X + 158, GIO_INV_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOInventorySettingRightCallback );


		// set user data
		MSYS_SetBtnUserData(giGIOInventorySettingButton[0],0, 0 );
		MSYS_SetBtnUserData(giGIOInventorySettingButton[1],0, 1 );

		if (UsingNewInventorySystem()==true &&
			UsingNewAttachmentSystem()==true)
		{
			iCurrentInventorySetting = GIO_INV_NEW_NAS;
		}
		else
		{
			// set initial value
			switch ( gGameOptions.ubInventorySystem )
			{
				case INVENTORY_OLD:
					iCurrentInventorySetting = GIO_INV_OLD;
					break;
				case INVENTORY_NEW:
					iCurrentInventorySetting = GIO_INV_NEW;
					break;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// DROP ALL ON/OFF SETTING

	guiDropAllOptionTogglesImage[ GIO_DROPALL_OFF ] = 	UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiDropAllOptionToggles[ GIO_DROPALL_OFF ] = CreateIconAndTextButton( guiDropAllOptionTogglesImage[ GIO_DROPALL_OFF ], gzGIOScreenText[ GIO_DROPALL_OFF_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_DROPALL_SETTING_X), (GIO_DROPALL_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIODropAllOffCallback);

	guiDropAllOptionTogglesImage[ GIO_DROPALL_ON ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiDropAllOptionToggles[ GIO_DROPALL_ON ] = CreateIconAndTextButton( guiDropAllOptionTogglesImage[ GIO_DROPALL_ON ],  gzGIOScreenText[ GIO_DROPALL_ON_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_DROPALL_SETTING_X + 74), (GIO_DROPALL_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIODropAllOnCallback );

	SpecifyButtonSoundScheme( guiDropAllOptionToggles[ GIO_DROPALL_OFF ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiDropAllOptionToggles[ GIO_DROPALL_ON ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiDropAllOptionToggles[ GIO_DROPALL_OFF ],0, 0 );
	MSYS_SetBtnUserData(guiDropAllOptionToggles[ GIO_DROPALL_ON ],0, 1 );

	if( gGameOptions.fEnemiesDropAllItems )
		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_ON ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_OFF ] ]->uiFlags |= BUTTON_CLICKED_ON;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// GUN SETTING

	guiGunOptionTogglesImage[ GIO_REDUCED_GUNS ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGunOptionToggles[ GIO_REDUCED_GUNS ] = CreateIconAndTextButton( guiGunOptionTogglesImage[ GIO_REDUCED_GUNS ], gzGIOScreenText[ GIO_REDUCED_GUNS_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GUN_SETTING_X + 74), (GIO_GUN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOGunSettingReducedCallback);

	guiGunOptionTogglesImage[ GIO_GUN_NUT ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiGunOptionToggles[ GIO_GUN_NUT ] = CreateIconAndTextButton( guiGunOptionTogglesImage[ GIO_GUN_NUT ],  gzGIOScreenText[ GIO_GUN_NUT_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_GUN_SETTING_X), (GIO_GUN_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOGunSettingToGCallback );

	SpecifyButtonSoundScheme( guiGunOptionToggles[ GIO_REDUCED_GUNS ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiGunOptionToggles[ GIO_GUN_NUT ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiGunOptionToggles[ GIO_REDUCED_GUNS ],0, 0 );
	MSYS_SetBtnUserData(guiGunOptionToggles[ GIO_GUN_NUT ],0, 1 );

	if( gGameOptions.fGunNut )
		ButtonList[ guiGunOptionToggles[ GIO_GUN_NUT ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiGunOptionToggles[ GIO_REDUCED_GUNS ] ]->uiFlags |= BUTTON_CLICKED_ON;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// WEAPON CACHES SETTING

	guiWeaponCachesOptionTogglesImage[ GIO_CACHES_RANDOM ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiWeaponCachesOptionToggles[ GIO_CACHES_RANDOM ] = CreateIconAndTextButton( guiWeaponCachesOptionTogglesImage[ GIO_CACHES_RANDOM ], gzGIOScreenText[ GIO_CACHES_RANDOM_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_CACHES_SETTING_X), (GIO_CACHES_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOWeaponCachesRandomCallback);

	guiWeaponCachesOptionTogglesImage[ GIO_CACHES_ALL ] = UseLoadedButtonImage( guiTraitsOptionTogglesImage[ GIO_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiWeaponCachesOptionToggles[ GIO_CACHES_ALL ] = CreateIconAndTextButton( guiWeaponCachesOptionTogglesImage[ GIO_CACHES_ALL ],  gzGIOScreenText[ GIO_CACHES_ALL_TEXT ], GIO_TOGGLE_TEXT_FONT,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													GIO_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(GIO_CACHES_SETTING_X + 74), (GIO_CACHES_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnGIOWeaponCachesAllCallback );

	SpecifyButtonSoundScheme( guiWeaponCachesOptionToggles[ GIO_CACHES_RANDOM ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiWeaponCachesOptionToggles[ GIO_CACHES_ALL ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiWeaponCachesOptionToggles[ GIO_CACHES_RANDOM ],0, 0 );
	MSYS_SetBtnUserData(guiWeaponCachesOptionToggles[ GIO_CACHES_ALL ],0, 1 );

	if( gGameOptions.fEnableAllWeaponCaches )
		ButtonList[ guiWeaponCachesOptionToggles[ GIO_CACHES_ALL ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiWeaponCachesOptionToggles[ GIO_CACHES_RANDOM ] ]->uiFlags |= BUTTON_CLICKED_ON;


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// SQUAD SIZE
	if (iResolution > 0)
	{
		giGIOSquadSizeButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
		giGIOSquadSizeButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

		// left button - decrement difficulty level
		giGIOSquadSizeButton[ 0 ] = QuickCreateButton( giGIOSquadSizeButtonImage[ 0 ], GIO_SQUAD_SIZE_SETTING_X + 39, GIO_SQUAD_SIZE_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOSquadSizeSelectionLeftCallback );

		// right button - increment difficulty level
		giGIOSquadSizeButton[ 1 ] = QuickCreateButton( giGIOSquadSizeButtonImage[ 1 ], GIO_SQUAD_SIZE_SETTING_X + 158, GIO_SQUAD_SIZE_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnGIOSquadSizeSelectionRightCallback );
		
		// set user data
		MSYS_SetBtnUserData(giGIOSquadSizeButton[0],0, 0 );
		MSYS_SetBtnUserData(giGIOSquadSizeButton[1],0, 1 );
		
		switch (gGameOptions.ubSquadSize)
		{
			case 6:
				iCurrentSquadSize = GIO_SQUAD_SIZE_6;
				break;
			case 8:
				iCurrentSquadSize = GIO_SQUAD_SIZE_8;
				break;
			case 10:
				iCurrentSquadSize = GIO_SQUAD_SIZE_10;
				break;
		}
	}
	
	//Reset the exit screen
	gubGIOExitScreen = GAME_INIT_OPTIONS_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderGIOScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfGIOButtonsAllocated = TRUE;

	return( TRUE );
}

void BtnGIODifficultySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentDifficulty > 0 )
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentDifficulty > 0 )
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIODifficultySelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentDifficulty < 3 )
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentDifficulty < 3 )
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentDifficulty++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIODifficultyButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOBRSettingLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentBRSetting > 0 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentBRSetting > 0 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOBRSettingRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentBRSetting < 3 )
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentBRSetting < 3 )
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentBRSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOIMPNumberSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentIMPNumberSetting > 1 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentIMPNumberSetting > 1 )
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOBRSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOIMPNumberSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentIMPNumberSetting < (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount) )
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentIMPNumberSetting < (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount) )
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentIMPNumberSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOInventorySettingLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentInventorySetting > GIO_INV_OLD )
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentInventorySetting > GIO_INV_OLD )
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOInventorySettingRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentInventorySetting < GIO_INV_NEW_NAS )
		{
			PlayButtonSound( giGIOIMPNumberButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentInventorySetting < GIO_INV_NEW_NAS )
		{
			PlayButtonSound( giGIOInventorySettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentInventorySetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOInventorySettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOProgressSettingLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentProgressSetting > 0 )
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentProgressSetting > 0 )
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOProgressSettingRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		if ( iCurrentProgressSetting < GIO_PROGRESS_VERY_FAST )
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentProgressSetting < GIO_PROGRESS_VERY_FAST )
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentProgressSetting++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOProgressSettingButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOOldTraitsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_TRAITS_SETTING_X), (GIO_TRAITS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_NEW ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiTraitsOptionToggles[ GIO_TRAITS_OLD ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIONewTraitsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if (!gGameExternalOptions.fReadProfileDataFromXML)
		{
			PlayButtonSound( guiTraitsOptionToggles[ GIO_TRAITS_NEW ], BUTTON_SOUND_DISABLED_CLICK );

			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioNewTraitsImpossibleText[0], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_OK, NewTraitsNotPossibleMessageBoxCallBack );
		}
		else
		{
			RestoreExternBackgroundRect( (GIO_TRAITS_SETTING_X), (GIO_TRAITS_SETTING_Y + 10), 230, 40 );

			ButtonList[ guiTraitsOptionToggles[ GIO_TRAITS_OLD ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			btn->uiFlags|=(BUTTON_CLICKED_ON);
				
			PlayButtonSound( guiTraitsOptionToggles[ GIO_TRAITS_NEW ], BUTTON_SOUND_CLICKED_ON );
		}
	}
}

void NewTraitsNotPossibleMessageBoxCallBack( UINT8 bExitValue )
{
	/*if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_NOTHING;
	}*/
}

void BtnGIORealisticStyleCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GAME_SETTING_X), (GIO_GAME_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameStyleToggles[ GIO_SCI_FI ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameStyleToggles[ GIO_REALISTIC ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOScifiStyleCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GAME_SETTING_X), (GIO_GAME_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameStyleToggles[ GIO_REALISTIC ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameStyleToggles[ GIO_SCI_FI ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOIronManOffCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_IRON_MAN_SETTING_X), (GIO_IRON_MAN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameStyleToggles[ GIO_CAN_SAVE ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOIronManOnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_IRON_MAN_SETTING_X), (GIO_IRON_MAN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGameStyleToggles[ GIO_IRON_MAN ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOTerroristsRandomCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_TERRORISTS_SETTING_X), (GIO_TERRORISTS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiTerroristsOptionToggles[ GIO_TERRORISTS_ALL ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiTerroristsOptionToggles[ GIO_TERRORISTS_RANDOM ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOTerroristsAllCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_TERRORISTS_SETTING_X), (GIO_TERRORISTS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiTerroristsOptionToggles[ GIO_TERRORISTS_RANDOM ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiTerroristsOptionToggles[ GIO_TERRORISTS_ALL ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIODropAllOffCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_DROPALL_SETTING_X), (GIO_DROPALL_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_ON ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiDropAllOptionToggles[ GIO_DROPALL_OFF ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIODropAllOnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_DROPALL_SETTING_X), (GIO_DROPALL_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiDropAllOptionToggles[ GIO_DROPALL_OFF ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiDropAllOptionToggles[ GIO_DROPALL_ON ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOGunSettingReducedCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GUN_SETTING_X), (GIO_GUN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGunOptionToggles[ GIO_GUN_NUT ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGunOptionToggles[ GIO_REDUCED_GUNS ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOGunSettingToGCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_GUN_SETTING_X), (GIO_GUN_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiGunOptionToggles[ GIO_REDUCED_GUNS ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiGunOptionToggles[ GIO_GUN_NUT ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOWeaponCachesRandomCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_CACHES_SETTING_X), (GIO_CACHES_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiWeaponCachesOptionToggles[ GIO_CACHES_ALL ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiWeaponCachesOptionToggles[ GIO_CACHES_RANDOM ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnGIOWeaponCachesAllCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (GIO_CACHES_SETTING_X), (GIO_CACHES_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiWeaponCachesOptionToggles[ GIO_CACHES_RANDOM ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiWeaponCachesOptionToggles[ GIO_CACHES_ALL ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnGIOSquadSizeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{		
		if ( iCurrentSquadSize > GIO_SQUAD_SIZE_6 )
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		if ( iCurrentSquadSize > GIO_SQUAD_SIZE_6 )
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize--;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[0], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

void BtnGIOSquadSizeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT )
	{
		UINT8 maxSquadSize = GIO_SQUAD_SIZE_10;
		if (iResolution == 1)
			maxSquadSize = GIO_SQUAD_SIZE_8;

		if ( iCurrentSquadSize < maxSquadSize )
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags|=(BUTTON_CLICKED_ON);

		UINT8 maxSquadSize = GIO_SQUAD_SIZE_10;
		if (iResolution == 1)
			maxSquadSize = GIO_SQUAD_SIZE_8;

		if ( iCurrentSquadSize < maxSquadSize )
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_CLICKED_ON );

			iCurrentSquadSize++;
			gfReRenderGIOScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giGIOSquadSizeButton[1], BUTTON_SOUND_DISABLED_CLICK );
		}
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
		}
	}
}

BOOLEAN		ExitGIOScreen()
{
	UINT16	cnt;

	if( !gfGIOButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiGIOMainBackGroundImage );

	DeleteVideoObjectFromIndex( guiGIOSMALLFRAME );

	// Destroy Basic buttons
	RemoveButton( guiGIOCancelButton );
	RemoveButton( guiGIODoneButton );
	UnloadButtonImage( giGIOCancelBtnImage );
	UnloadButtonImage( giGIODoneBtnImage );

	// Destroy Difficulty setting buttons
	RemoveButton( giGIODifficultyButton[0] );
	RemoveButton( giGIODifficultyButton[1] );
	UnloadButtonImage( giGIODifficultyButtonImage[0] );
	UnloadButtonImage( giGIODifficultyButtonImage[1] );

	// Destroy IMP Number setting buttons
	RemoveButton( giGIOIMPNumberButton[0] );
	RemoveButton( giGIOIMPNumberButton[1] );
	UnloadButtonImage( giGIOIMPNumberButtonImage[0] );
	UnloadButtonImage( giGIOIMPNumberButtonImage[1] );

	// Destroy BR setting buttons
	RemoveButton( giGIOBRSettingButton[0] );
	RemoveButton( giGIOBRSettingButton[1] );
	UnloadButtonImage( giGIOBRSettingButtonImage[0] );
	UnloadButtonImage( giGIOBRSettingButtonImage[1] );

	// Destroy Progress setting buttons
	RemoveButton( giGIOProgressSettingButton[0] );
	RemoveButton( giGIOProgressSettingButton[1] );
	UnloadButtonImage( giGIOProgressSettingButtonImage[0] );
	UnloadButtonImage( giGIOProgressSettingButtonImage[1] );
	
	// Destroy Traits setting buttons
	for( cnt=0; cnt<NUM_TRAIT_OPTIONS; cnt++)
	{
		RemoveButton( guiTraitsOptionToggles[ cnt ] );
		UnloadButtonImage( guiTraitsOptionTogglesImage[ cnt ] );
	}

	// Destroy Game setting buttons
	for( cnt=0; cnt<NUM_GAME_STYLES; cnt++)
	{
		RemoveButton( guiGameStyleToggles[ cnt ] );
		UnloadButtonImage( guiGameStyleTogglesImage[ cnt ] );
	}

	// Destroy Iron Man setting buttons
	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		RemoveButton( guiGameSaveToggles[ cnt ] );
		UnloadButtonImage( guiGameSaveTogglesImage[ cnt ] );
	}

	// Destroy Terrorists setting buttons
	for( cnt=0; cnt<NUM_TERRORISTS_OPTIONS; cnt++)
	{
		RemoveButton( guiTerroristsOptionToggles[ cnt ] );
		UnloadButtonImage( guiTerroristsOptionTogglesImage[ cnt ] );
	}

	// Destroy Inventory setting buttons
	if(IsNIVModeValid(true) == TRUE)
	{
		// Destroy Inventory settings
		RemoveButton( giGIOInventorySettingButton[0] );
		RemoveButton( giGIOInventorySettingButton[1] );
		UnloadButtonImage( giGIOInventorySettingButtonImage[0] );
		UnloadButtonImage( giGIOInventorySettingButtonImage[1] );
	}

	// Destroy Drop All setting buttons
	for( cnt=0; cnt<NUM_DROPALL_OPTIONS; cnt++)
	{
		RemoveButton( guiDropAllOptionToggles[ cnt ] );
		UnloadButtonImage( guiDropAllOptionTogglesImage[ cnt ] );
	}

	// Destroy Gun setting buttons
	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		RemoveButton( guiGunOptionToggles[ cnt ] );
		UnloadButtonImage( guiGunOptionTogglesImage[ cnt ] );
	}

	// Destroy Weapon Caches setting buttons
	for( cnt=0; cnt<NUM_CACHES_OPTIONS; cnt++)
	{
		RemoveButton( guiWeaponCachesOptionToggles[ cnt ] );
		UnloadButtonImage( guiWeaponCachesOptionTogglesImage[ cnt ] );
	}

	// Destroy Squad size setting buttons
	if (iResolution > 0)
	{
		RemoveButton( giGIOSquadSizeButton[0] );
		RemoveButton( giGIOSquadSizeButton[1] );
		UnloadButtonImage( giGIOSquadSizeButtonImage[0] );
		UnloadButtonImage( giGIOSquadSizeButtonImage[1] );
	}

	gfGIOButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	if( gubGameOptionScreenHandler == GIO_EXIT )
		SetMusicMode( MUSIC_NONE );

	gfGIOScreenExit = FALSE;
	gfGIOScreenEntry = TRUE;

	return( TRUE );
}


void HandleGIOScreen()
{
	if( gubGameOptionScreenHandler != GIO_NOTHING )
	{
		switch( gubGameOptionScreenHandler )
		{
			case GIO_CANCEL:
				gubGIOExitScreen = MAINMENU_SCREEN;
				gfGIOScreenExit	= TRUE;
				break;
			
			case MP_LOAD:
				gubGIOExitScreen = SAVE_LOAD_SCREEN;
				gfSaveGame = FALSE;
				gfGIOScreenExit	= TRUE;
				guiPreviousOptionScreen = GAME_INIT_OPTIONS_SCREEN;
				break;

			case GIO_EXIT:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitGameInitOptionScreen )
				{
					//Disable the ok button
					DisableButton( guiGIODoneButton );

					gFadeOutDoneCallback = DoneFadeOutForExitGameInitOptionScreen;

					FadeOutNextFrame( );
				}
				break;
			}

			case GIO_IRON_MAN_MODE:
				DisplayMessageToUserAboutGameDifficulty();
				break;
		}

		gubGameOptionScreenHandler = GIO_NOTHING;
	}


	if( gfReRenderGIOScreen )
	{
		RenderGIOScreen();
		gfReRenderGIOScreen = FALSE;
	}
}


BOOLEAN		RenderGIOScreen()
{
	HVOBJECT	hPixHandle;

	RestoreExternBackgroundRect( GIO_DIF_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_DIF_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_IMP_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_BR_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_BR_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_PROGRESS_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_PROGRESS_SETTING_Y-3, 120, 20 );	
	RestoreExternBackgroundRect( GIO_INV_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_INV_SETTING_Y-3, 120, 20 );
	RestoreExternBackgroundRect( GIO_SQUAD_SIZE_SETTING_X+GIO_OFFSET_TO_TEXT + 20, GIO_SQUAD_SIZE_SETTING_Y-3, 120, 20 );

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiGIOMainBackGroundImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,0,0, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	//Display the title
	DrawTextToScreen( gzGIOScreenText[ GIO_INITIAL_GAME_SETTINGS ], GIO_MAIN_TITLE_X, GIO_MAIN_TITLE_Y, GIO_MAIN_TITLE_WIDTH, GIO_TITLE_FONT, GIO_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Dif Settings Title Text
	RenderGIOSmallSelectionFrame( (GIO_DIF_SETTING_X + 36), (GIO_DIF_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_DIF_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_DIF_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_DIF_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_DIF_SETTING_Y+6), GIO_DIF_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentDifficulty + 9 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the IMP number Title Text
	RenderGIOSmallSelectionFrame( (GIO_IMP_SETTING_X + 36), (GIO_IMP_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_IMP_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_DIF_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_IMP_NUMBER_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	if ( iCurrentIMPNumberSetting <= 6 )
	{
		DisplayWrappedString( (UINT16)(GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_IMP_SETTING_Y+6), GIO_IMP_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentIMPNumberSetting + 32 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
	else
	{
		CHAR16 sStartLevelString[16];
		swprintf(sStartLevelString, L"%i", iCurrentIMPNumberSetting );
		DisplayWrappedString( (UINT16)(GIO_IMP_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_IMP_SETTING_Y+6), GIO_IMP_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, sStartLevelString, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	}
	// Display BR Setting texts
	RenderGIOSmallSelectionFrame( (GIO_BR_SETTING_X + 36), (GIO_BR_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_BR_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_BR_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_BR_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_BR_QUALITY_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_BR_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_BR_SETTING_Y+6), GIO_BR_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentBRSetting + 20 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Display Progress Setting texts
	RenderGIOSmallSelectionFrame( (GIO_PROGRESS_SETTING_X + 36), (GIO_PROGRESS_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_PROGRESS_SETTING_X+GIO_OFFSET_TO_TEXT - 6), (UINT16)(GIO_PROGRESS_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_PROGRESS_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_PROGRESS_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_PROGRESS_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_PROGRESS_SETTING_Y+6), GIO_PROGRESS_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentProgressSetting + 49 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Old/new traits system
	DisplayWrappedString( (GIO_TRAITS_SETTING_X - 6), (UINT16)(GIO_TRAITS_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_TRAITS_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TRAITS_TEXT], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Game Settings Title Text
	DisplayWrappedString( (GIO_GAME_SETTING_X - 6), (UINT16)(GIO_GAME_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_GAME_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GAME_STYLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// JA2Gold: Display the iron man Settings Title Text
	DisplayWrappedString( (GIO_IRON_MAN_SETTING_X - 6), (UINT16)(GIO_IRON_MAN_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_IRON_MAN_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GAME_SAVE_STYLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Terrorists Settings Title Text
	DisplayWrappedString( (GIO_TERRORISTS_SETTING_X - 6), (UINT16)(GIO_TERRORISTS_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_TERRORISTS_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TERRORISTS_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	// Display Inventory Settings text
	RenderGIOSmallSelectionFrame( (GIO_INV_SETTING_X + 36), (GIO_INV_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_INV_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_INV_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_INV_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_INV_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_INV_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_INV_SETTING_Y+5), GIO_INV_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentInventorySetting + 54 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );	

	//Display the Drop All Settings Title Text
	DisplayWrappedString( (GIO_DROPALL_SETTING_X - 6), (UINT16)(GIO_DROPALL_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_DROPALL_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DROPALL_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Gun Settings Title Text
	DisplayWrappedString( (GIO_GUN_SETTING_X - 6), (UINT16)(GIO_GUN_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_GUN_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_GUN_OPTIONS_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Weapon Caches Settings Title Text
	DisplayWrappedString( (GIO_CACHES_SETTING_X - 6), (UINT16)(GIO_CACHES_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE), GIO_CACHES_SETTING_WIDTH + 14, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_CACHES_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	//Display the Squad size Title Text	
	RenderGIOSmallSelectionFrame( (GIO_SQUAD_SIZE_SETTING_X + 36), (GIO_SQUAD_SIZE_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(GIO_SQUAD_SIZE_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (UINT16)(GIO_SQUAD_SIZE_SETTING_Y-GIO_GAP_BN_SETTINGS + GIO_TITLE_DISTANCE - 12), GIO_SQUAD_SIZE_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_SQUAD_SIZE_TITLE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(GIO_SQUAD_SIZE_SETTING_X+GIO_OFFSET_TO_TEXT + 1), (GIO_SQUAD_SIZE_SETTING_Y+6), GIO_SQUAD_SIZE_SETTING_WIDTH, 2, GIO_TOGGLE_TEXT_FONT, GIO_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iCurrentSquadSize + (GIO_SQUAD_SIZE_TITLE_TEXT + 1) ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	return( TRUE );
}


void GetGIOScreenUserInput()
{
	InputAtom Event;

	while( DequeueEvent( &Event ) )
	{
		if( Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubGameOptionScreenHandler = GIO_CANCEL;
					break;

#ifdef JA2TESTVERSION
				case 'r':
					gfReRenderGIOScreen=TRUE;
					break;

				case 'i':
					InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
					break;
#endif

				case ENTER:
					gubGameOptionScreenHandler = GIO_EXIT;
					break;
			}
		}
	}
}


void BtnGIODoneCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//if the user doesnt have IRON MAN mode selected
		if( !DisplayMessageToUserAboutIronManMode() )
		{
			//Confirm the difficulty setting
			DisplayMessageToUserAboutGameDifficulty();
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubGameOptionScreenHandler = GIO_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void MPBtnGIOCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubGameOptionScreenHandler = MP_LOAD;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

UINT8	GetCurrentTraitsOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_TRAIT_OPTIONS; cnt++)
	{
		if( ButtonList[ guiTraitsOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentGameStyleButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GAME_STYLES; cnt++)
	{
		if( ButtonList[ guiGameStyleToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentGameSaveButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_SAVE_OPTIONS; cnt++)
	{
		if( ButtonList[ guiGameSaveToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentTerroristsButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GAME_STYLES; cnt++)
	{
		if( ButtonList[ guiTerroristsOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentDropAllButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GAME_STYLES; cnt++)
	{
		if( ButtonList[ guiDropAllOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentGunButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GUN_OPTIONS; cnt++)
	{
		if( ButtonList[ guiGunOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentWeaponCachesButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_GAME_STYLES; cnt++)
	{
		if( ButtonList[ guiWeaponCachesOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

UINT8	GetCurrentTimedTurnsButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<GIO_NUM_TIMED_TURN_OPTIONS; cnt++)
	{
		if( ButtonList[ guiTimedTurnToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

void DoneFadeOutForExitGameInitOptionScreen( void )
{
	// loop through and get the status of all the buttons
	gGameOptions.fGunNut = GetCurrentGunButtonSetting();
	gGameOptions.ubGameStyle = GetCurrentGameStyleButtonSetting();
	gGameOptions.ubDifficultyLevel = min( NUM_DIFF_SETTINGS, ( max( 1, (iCurrentDifficulty + 1)) ));

	gGameOptions.fTurnTimeLimit = FALSE;
		
	// iron man
	gGameOptions.fIronManMode = GetCurrentGameSaveButtonSetting();

	switch ( iCurrentBRSetting )
	{
		case GIO_BR_GOOD:
			gGameOptions.ubBobbyRay = BR_GOOD;
			break;
		case GIO_BR_GREAT:
			gGameOptions.ubBobbyRay = BR_GREAT;
			break;
		case GIO_BR_EXCELLENT:
			gGameOptions.ubBobbyRay = BR_EXCELLENT;
			break;
		case GIO_BR_AWESOME:
			gGameOptions.ubBobbyRay = BR_AWESOME;
			break;
	}

	// CHRISL:
	if(IsNIVModeValid(true) == TRUE)
	{
		switch ( iCurrentInventorySetting)				
		{
			case GIO_INV_OLD:
				gGameOptions.ubInventorySystem = INVENTORY_OLD;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
				break;
			case GIO_INV_NEW:
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_OLD;
				break;
			case GIO_INV_NEW_NAS:
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				gGameOptions.ubAttachmentSystem = ATTACHMENT_NEW;
		}
	}

	// Squad size
	switch (iCurrentSquadSize)
	{
		case GIO_SQUAD_SIZE_6:
			gGameOptions.ubSquadSize = 6;
			break;
		case GIO_SQUAD_SIZE_8:
			gGameOptions.ubSquadSize = 8;
			break;
		case GIO_SQUAD_SIZE_10:
			gGameOptions.ubSquadSize = 10;
			break;
	}

	
	// SANDRO - added following:
	gGameOptions.ubMaxIMPCharacters = min( (gGameExternalOptions.iIMPMaleCharacterCount + gGameExternalOptions.iIMPFemaleCharacterCount), ( max( 1, iCurrentIMPNumberSetting) ));
	gGameOptions.fNewTraitSystem = GetCurrentTraitsOptionButtonSetting();
	gGameOptions.fEnableAllTerrorists = GetCurrentTerroristsButtonSetting();
	gGameOptions.fEnemiesDropAllItems = GetCurrentDropAllButtonSetting();
	gGameOptions.fEnableAllWeaponCaches = GetCurrentWeaponCachesButtonSetting();
	gGameOptions.ubProgressSpeedOfItemsChoices = min( GIO_PROGRESS_VERY_FAST, iCurrentProgressSetting );

	//	gubGIOExitScreen = INIT_SCREEN;
	gubGIOExitScreen = INTRO_SCREEN;

#ifdef JA2TESTVERSION
	if( gfKeyState[ ALT ] )
	{
		if( gfKeyState[ CTRL ] )
		{
			gMercProfiles[ MIGUEL ].bMercStatus = MERC_IS_DEAD;
			gMercProfiles[ SKYRIDER ].bMercStatus = MERC_IS_DEAD;
		}

		SetIntroType( INTRO_ENDING );
	}
	else
#endif
		SetIntroType( INTRO_BEGINNING );

	ExitGIOScreen();

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitGameInitOptionScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

BOOLEAN DoGioMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

	// do message box and return
	giGioMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giGioMessageBox != -1 ) );
}

void DisplayMessageToUserAboutGameDifficulty()
{
	UINT8 ubDiffLevel = iCurrentDifficulty;

	switch( ubDiffLevel )
	{
		case 0:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_NOVICE], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 1:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_EXPERIENCED], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 2:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_EXPERT], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
		case 3:
			DoGioMessageBox( MSG_BOX_BASIC_STYLE, zGioDifConfirmText[GIO_CFS_INSANE], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioDifSettingMessageBoxCallBack );
			break;
	}
}


void	ConfirmGioDifSettingMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_EXIT;
	}
}

BOOLEAN DisplayMessageToUserAboutIronManMode()
{
	// Madd
	UINT8 ubIronManMode = GetCurrentGameSaveButtonSetting(); //FALSE; 

	//if the user has selected IRON MAN mode
	if( ubIronManMode )
	{
		DoGioMessageBox( MSG_BOX_BASIC_STYLE, gzIronManModeWarningText[ IMM__IRON_MAN_MODE_WARNING_TEXT ], GAME_INIT_OPTIONS_SCREEN, MSG_BOX_FLAG_YESNO, ConfirmGioIronManMessageBoxCallBack );

		return( TRUE );
	}

	return( FALSE );
}

void ConfirmGioIronManMessageBoxCallBack( UINT8 bExitValue )
{
	if( bExitValue == MSG_BOX_RETURN_YES )
	{
		gubGameOptionScreenHandler = GIO_IRON_MAN_MODE;
	}
	else
	{
		ButtonList[ guiGameSaveToggles[ GIO_IRON_MAN ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ guiGameSaveToggles[ GIO_CAN_SAVE ] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void RenderGIOSmallSelectionFrame(INT16 sX, INT16 sY)
{

	HVOBJECT hHandle;

	// get the video object
	GetVideoObject(&hHandle, guiGIOSMALLFRAME);

	// blt to sX, sY relative to upper left corner
	BltVideoObject(FRAME_BUFFER, hHandle, 0, sX, sY , VO_BLT_SRCTRANSPARENCY,NULL);

	return;
}

#endif

