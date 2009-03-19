#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "Intro.h"
#else
	#include "Types.h"
	#include "MPHostScreen.h"
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
	#include "Text Input.h"
	#include "_Ja25EnglishText.h"
	#include "Soldier Profile.h"
#endif

#include "gameloop.h"
#include "connect.h"
#include "network.h" // for client name
#include "saveloadscreen.h"


////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

#define		MPH_TITLE_FONT							FONT14ARIAL//FONT16ARIAL
#define		MPH_TITLE_COLOR									FONT_MCOLOR_WHITE

#define		MPH_LABEL_TEXT_FONT					FONT12ARIAL//FONT16ARIAL
#define		MPH_LABEL_TEXT_COLOR							FONT_MCOLOR_WHITE
#define		MPH_TOGGLE_TEXT_FONT					FONT12ARIAL//FONT16ARIAL
#define		MPH_TOGGLE_TEXT_COLOR							FONT_MCOLOR_WHITE

//buttons
#define		MPH_BTN_START_X							iScreenWidthOffset + 320 + 143
#define		MPH_BTN_START_Y							iScreenHeightOffset + 435
#define		MPH_CANCEL_X							iScreenWidthOffset + ((320 - 115) / 2)

//textboxes
#define		MPH_TXT_SVRNAME_X						iScreenWidthOffset + 100
#define		MPH_TXT_SVRNAME_Y						iScreenHeightOffset + 167
#define		MPH_TXT_SVRNAME_WIDTH					120
#define		MPH_TXT_SVRNAME_HEIGHT					17
#define		MPH_TXT_MAXPLAYERS_X					iScreenWidthOffset + 100
#define		MPH_TXT_MAXPLAYERS_Y					MPH_TXT_SVRNAME_Y + MPH_TXT_SVRNAME_HEIGHT + 13
#define		MPH_TXT_MAXPLAYERS_WIDTH				40
#define		MPH_TXT_MAXPLAYERS_HEIGHT				17
#define		MPH_TXT_SQUAD_X							iScreenWidthOffset + 100
#define		MPH_TXT_SQUAD_Y							MPH_TXT_MAXPLAYERS_Y + MPH_TXT_MAXPLAYERS_HEIGHT + 13
#define		MPH_TXT_SQUAD_WIDTH						40
#define		MPH_TXT_SQUAD_HEIGHT					17
#define		MPH_TXT_TIME_X							iScreenWidthOffset + 100
#define		MPH_TXT_TIME_Y							MPH_TXT_SQUAD_Y + MPH_TXT_SQUAD_HEIGHT + 13
#define		MPH_TXT_TIME_WIDTH						60
#define		MPH_TXT_TIME_HEIGHT						17
#define		MPH_TXT_CASH_X							iScreenWidthOffset + 100
#define		MPH_TXT_CASH_Y							MPH_TXT_TIME_Y + MPH_TXT_TIME_HEIGHT + 13
#define		MPH_TXT_CASH_WIDTH						60
#define		MPH_TXT_CASH_HEIGHT						17
#define		MPH_TXT_DMG_X							iScreenWidthOffset + 100
#define		MPH_TXT_DMG_Y							MPH_TXT_CASH_Y + MPH_TXT_CASH_HEIGHT + 13
#define		MPH_TXT_DMG_WIDTH						60
#define		MPH_TXT_DMG_HEIGHT						17
#define		MPH_TXT_TIMER_X							iScreenWidthOffset + 100
#define		MPH_TXT_TIMER_Y							MPH_TXT_DMG_Y + MPH_TXT_DMG_HEIGHT + 13
#define		MPH_TXT_TIMER_WIDTH						60
#define		MPH_TXT_TIMER_HEIGHT					17



//main title
#define		MPH_MAIN_TITLE_X								0
#define		MPH_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		MPH_MAIN_TITLE_WIDTH					SCREEN_WIDTH

//labels
#define		MPH_LABEL_SVRNAME_X						MPH_TXT_SVRNAME_X - 80
#define		MPH_LABEL_SVRNAME_Y						MPH_TXT_SVRNAME_Y + 3
#define		MPH_LABEL_SVRNAME_WIDTH					80
#define		MPH_LABEL_SVRNAME_HEIGHT				17
#define		MPH_LABEL_MAXPLAYERS_X					MPH_TXT_MAXPLAYERS_X - 80
#define		MPH_LABEL_MAXPLAYERS_Y					MPH_TXT_MAXPLAYERS_Y + 3
#define		MPH_LABEL_MAXPLAYERS_WIDTH				80
#define		MPH_LABEL_MAXPLAYERS_HEIGHT				17
#define		MPH_LABEL_SQUAD_X						MPH_TXT_SQUAD_X - 80
#define		MPH_LABEL_SQUAD_Y						MPH_TXT_SQUAD_Y + 3
#define		MPH_LABEL_SQUAD_WIDTH					80
#define		MPH_LABEL_SQUAD_HEIGHT					17
#define		MPH_LABEL_TIME_X						MPH_TXT_TIME_X - 80
#define		MPH_LABEL_TIME_Y						MPH_TXT_TIME_Y + 3
#define		MPH_LABEL_TIME_WIDTH					80
#define		MPH_LABEL_TIME_HEIGHT					17
#define		MPH_LABEL_CASH_X						MPH_TXT_CASH_X - 80
#define		MPH_LABEL_CASH_Y						MPH_TXT_CASH_Y + 3
#define		MPH_LABEL_CASH_WIDTH					80
#define		MPH_LABEL_CASH_HEIGHT					17
#define		MPH_LABEL_DMG_X							MPH_TXT_DMG_X - 80
#define		MPH_LABEL_DMG_Y							MPH_TXT_DMG_Y + 3
#define		MPH_LABEL_DMG_WIDTH						80
#define		MPH_LABEL_DMG_HEIGHT					17
#define		MPH_LABEL_TIMER_X						MPH_TXT_TIMER_X - 80
#define		MPH_LABEL_TIMER_Y						MPH_TXT_TIMER_Y + 3
#define		MPH_LABEL_TIMER_WIDTH					80
#define		MPH_LABEL_TIMER_HEIGHT					17

//radio box locations
#define		MPH_GAP_BN_SETTINGS								35
#define		MPH_OFFSET_TO_TEXT						30
#define		MPH_OFFSET_TO_TOGGLE_BOX				170
#define		MPH_OFFSET_TO_TOGGLE_BOX_Y						9

#define		MPH_DIF_SETTINGS_X						iScreenWidthOffset + 320 
#define		MPH_DIF_SETTINGS_Y						iScreenHeightOffset + 75
#define		MPH_DIF_SETTINGS_WIDTH					MPH_OFFSET_TO_TOGGLE_BOX - MPH_OFFSET_TO_TEXT

#define		MPH_GAMETYPE_SETTINGS_X					iScreenWidthOffset + 20
#define		MPH_GAMETYPE_SETTINGS_Y					iScreenHeightOffset + 75
#define		MPH_GAMETYPE_SETTINGS_WIDTH				MPH_OFFSET_TO_TOGGLE_BOX - MPH_OFFSET_TO_TEXT

#define		MPH_RNDMERC_X							MPH_DIF_SETTINGS_X
#define		MPH_RNDMERC_Y							MPH_TXT_MAXPLAYERS_Y//iScreenHeightOffset+214
#define		MPH_RNDMERC_WIDTH						MPH_OFFSET_TO_TOGGLE_BOX

#define		MPH_SAMEMERC_X							MPH_DIF_SETTINGS_X
#define		MPH_SAMEMERC_Y							MPH_RNDMERC_Y + MPH_GAP_BN_SETTINGS - 5
#define		MPH_SAMEMERC_WIDTH						MPH_OFFSET_TO_TOGGLE_BOX

#define		MPH_REPORTMERC_X						MPH_DIF_SETTINGS_X
#define		MPH_REPORTMERC_Y						MPH_SAMEMERC_Y + MPH_GAP_BN_SETTINGS - 5
#define		MPH_REPORTMERC_WIDTH					MPH_OFFSET_TO_TOGGLE_BOX

#define		MPH_BOBBYRAY_X							MPH_DIF_SETTINGS_X
#define		MPH_BOBBYRAY_Y							MPH_REPORTMERC_Y + MPH_GAP_BN_SETTINGS - 5
#define		MPH_BOBBYRAY_WIDTH						MPH_OFFSET_TO_TOGGLE_BOX

#define		MPH_RNDSPAWN_X							MPH_DIF_SETTINGS_X
#define		MPH_RNDSPAWN_Y							MPH_BOBBYRAY_Y + MPH_GAP_BN_SETTINGS - 5
#define		MPH_RNDSPAWN_WIDTH						MPH_OFFSET_TO_TOGGLE_BOX

#define		MPH_CIVS_X								MPH_DIF_SETTINGS_X
#define		MPH_CIVS_Y								MPH_RNDSPAWN_Y + MPH_GAP_BN_SETTINGS - 5
#define		MPH_CIVS_WIDTH							MPH_OFFSET_TO_TOGGLE_BOX

#define		MPH_USENIV_X							MPH_DIF_SETTINGS_X
#define		MPH_USENIV_Y							MPH_CIVS_Y + MPH_GAP_BN_SETTINGS - 5
#define		MPH_USENIV_WIDTH						MPH_OFFSET_TO_TOGGLE_BOX

//Difficulty settings
enum
{
	MPH_DIFF_EASY,
	MPH_DIFF_MED,
	MPH_DIFF_HARD,
	MPH_DIFF_INSANE,
	NUM_DIFF_SETTINGS,
};



////////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfMPHScreenEntry = TRUE;
BOOLEAN		gfMPHScreenExit	= FALSE;
BOOLEAN		gfReRenderMPHScreen=TRUE;
BOOLEAN		gfMPHButtonsAllocated = FALSE;

//enum for different states of screen
enum
{
	MPH_NOTHING,
	MPH_CANCEL,
	MPH_START
};

UINT8		gubMPHScreenHandler=MPH_NOTHING;	// State changer for HandleMPHScreen()

UINT32		gubMPHExitScreen = MP_HOST_SCREEN;	// The screen that is in control next iteration of the game_loop

UINT32		guiMPHMainBackGroundImage;

// Wide-char strings that will hold the variables until they are transferred to the CHAR ascii fields
CHAR16		gzServerNameField[ 30 ] = {0} ;
CHAR16		gzMaxPlayersField[ 2 ] = {0} ;
CHAR16		gzSquadSizeField[ 2 ] = {0} ;
CHAR16		gzTimeOfDayField[ 5 ] = {0};
CHAR16		gzStartingBalanceField[ 10 ] = {0};
INT16		giMPHTimeHours = 0;
INT16		giMPHTimeMins = 0;
CHAR16		gzDmgMultiplierField[ 5 ] = {0};
CHAR16		gzTimerField[ 5 ] = {0};

UINT8		guiMPHGameType = MP_TYPE_DEATHMATCH; // default value

INT32		giMPHMessageBox = -1; // message box handle

INT			giMPHRandomMercs = 0;
INT			giMPHSameMercs = 0;
INT			giMPHReportMercs = 0;
INT			giMPHBobbyRays = 0;
INT			giMPHRandomSpawn = 0;
INT			giMPHEnableCivilians = 0;
INT			giMPHUseNIV = 0;


////////////////////////////////////////////
//
//	Screen Controls
//
///////////////////////////////////////////

// Start Button
void BtnMPHStartCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPHStartButton;
INT32	giMPHStartBtnImage;

// Cancel Button
void	BtnMPHCancelCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMPHCancelButton;
INT32	giMPHCancelBtnImage;

//checkbox to toggle the Diff level
UINT32	guiMPHDifficultySettingsToggles[ NUM_DIFF_SETTINGS ];
void BtnMPHDifficultyTogglesCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox to toggle Game Type
UINT32	guiMPHGameTypeToggles[ NUM_MP_GAMETYPE ];
void BtnMPHGameTypeTogglesCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for merc options
UINT32 guiMPHRandomMercsToggle;
void BtnMPHRandomMercCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for same merc
UINT32 guiMPHSameMercToggle;
void BtnMPHSameMercCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for report purchase
UINT32 guiMPHReportMercToggle;
void BtnMPHReportMercCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for bobby rays
UINT32 guiMPHBobbyRayToggle;
void BtnMPHBobbyRayCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for random spawn pos
UINT32 guiMPHRandomSpawnToggle;
void BtnMPHRandomSpawnCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for enable civilians in CO-OP
UINT32 guiMPHCivsToggle;
void BtnMPHCivsCallback(GUI_BUTTON *btn,INT32 reason);

//checkbox for use NIV
UINT32 guiMPHUseNIVToggle;
void BtnMPHUseNIVCallback(GUI_BUTTON *btn,INT32 reason);


////////////////////////////////////////////
//
//	Local Function Prototypes
//
///////////////////////////////////////////

extern void ClearMainMenu();

BOOLEAN		EnterMPHScreen();
BOOLEAN		ExitMPHScreen();
void		HandleMPHScreen();
BOOLEAN		RenderMPHScreen();
void		GetMPHScreenUserInput();
void		RestoreMPHButtonBackGrounds();
bool		ValidateMPSettings();
void		SaveMPSettings();
UINT8		GetMPHCurrentDifficultyButtonSetting();
UINT8		GetMPHGameTypeButtonSetting();
BOOLEAN DoMPHMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void			DoneFadeOutForExitMPHScreen( void );
void			DoneFadeInForExitMPHScreen( void );

////////////////////////////////////////////
//
//	Functions
//
///////////////////////////////////////////

void		SaveMPSettings()
{
	Get16BitStringFromField( 0, gzServerNameField ); // these indexes are based on the order created
	Get16BitStringFromField( 1, gzMaxPlayersField );
	Get16BitStringFromField( 2, gzSquadSizeField );

	// save settings to JA2_mp.ini
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"SERVER_NAME", gzServerNameField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"MAX_CLIENTS", gzMaxPlayersField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"MAX_MERCS", gzSquadSizeField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"STARTING_BALANCE", gzStartingBalanceField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"DAMAGE_MULTIPLIER", gzDmgMultiplierField, L"..\\Ja2_mp.ini" );
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"TIMED_TURN_SECS_PER_TICK", gzTimerField, L"..\\Ja2_mp.ini" );


	guiMPHGameType = GetMPHGameTypeButtonSetting();
	CHAR16 tmpGTStr[2];
	_itow(guiMPHGameType,tmpGTStr,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"GAME_MODE", tmpGTStr, L"..\\Ja2_mp.ini" );

	CHAR16 tmpTimeStr[6];
	swprintf(tmpTimeStr,L"%i.%i",giMPHTimeHours,giMPHTimeMins);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"TIME", tmpTimeStr, L"..\\Ja2_mp.ini" );

	CHAR16 tmpVal[2];

	giMPHRandomMercs = ( ButtonList[ guiMPHRandomMercsToggle ]->uiFlags & BUTTON_CLICKED_ON ? 1 : 0 );
	_itow(giMPHRandomMercs,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"RANDOM_MERCS", tmpVal, L"..\\Ja2_mp.ini" );

	giMPHSameMercs = ( ButtonList[ guiMPHSameMercToggle ]->uiFlags & BUTTON_CLICKED_ON ? 1 : 0 );
	_itow(giMPHSameMercs,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"SAME_MERC", tmpVal, L"..\\Ja2_mp.ini" );

	giMPHBobbyRays = ( ButtonList[ guiMPHBobbyRayToggle ]->uiFlags & BUTTON_CLICKED_ON  ? 0 : 1 );  // This setting is reversed
	_itow(giMPHBobbyRays,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"DISABLE_BOBBY_RAYS", tmpVal, L"..\\Ja2_mp.ini" );

	giMPHReportMercs = ( ButtonList[ guiMPHReportMercToggle ]->uiFlags & BUTTON_CLICKED_ON ? 1 : 0 );
	_itow(giMPHReportMercs,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"REPORT_NAME", tmpVal, L"..\\Ja2_mp.ini" );

	giMPHRandomSpawn = ( ButtonList[ guiMPHRandomSpawnToggle ]->uiFlags & BUTTON_CLICKED_ON ? 1 : 0 );
	_itow(giMPHRandomSpawn,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"RANDOM_EDGES", tmpVal, L"..\\Ja2_mp.ini" );

	giMPHEnableCivilians = ( ButtonList[ guiMPHCivsToggle ]->uiFlags & BUTTON_CLICKED_ON ? 1 : 0 );
	_itow(giMPHEnableCivilians,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"CIV_ENABLED", tmpVal, L"..\\Ja2_mp.ini" );

	giMPHUseNIV = ( ButtonList[ guiMPHUseNIVToggle ]->uiFlags & BUTTON_CLICKED_ON ? 1 : 0 );
	_itow(giMPHUseNIV,tmpVal,10);
	WritePrivateProfileStringW( L"Ja2_mp Settings",L"ALLOW_CUSTOM_NIV", tmpVal, L"..\\Ja2_mp.ini" );




	/*if ( ButtonList[ guiMPHRandomSpawnToggle ]->uiFlags & BUTTON_CLICKED_ON )
		WritePrivateProfileStringW( L"Ja2_mp Settings",L"RANDOM_EDGES", L"1", L"..\\Ja2_mp.ini" );
	else
		WritePrivateProfileStringW( L"Ja2_mp Settings",L"RANDOM_EDGES", L"0", L"..\\Ja2_mp.ini" );*/

	// save game difficulty setting
	gGameOptions.ubDifficultyLevel = GetMPHCurrentDifficultyButtonSetting();
}

bool	ValidateMPSettings()
{
	// Check a Server name is entered
	Get16BitStringFromField( 0, gzServerNameField ); // these indexes are based on the order created
	if (wcscmp(gzServerNameField,L"")<=0)
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_SERVERNAME_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	// Verify the Max Players
	Get16BitStringFromField( 1, gzMaxPlayersField );
	UINT8 numPlayers = _wtoi(gzMaxPlayersField);
	if (numPlayers < 2 || numPlayers > 4)
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_MAXPLAYERS_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}
	
	// Verify the Squad Size
	Get16BitStringFromField( 2, gzSquadSizeField );
	UINT8 squadSize = _wtoi(gzSquadSizeField);
	if (squadSize < 1 || squadSize > 6)
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_SQUADSIZE_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	// verify the Time of Day
	Get16BitStringFromField( 3, gzTimeOfDayField );
	wchar_t* tok;
	bool bTimeOK = true;
	int hours = 0;
	int mins = 0;

	// strtok is destructive, make a copy to work on
	CHAR16 tmpTODStr[5];
	memcpy(tmpTODStr,gzTimeOfDayField,sizeof(CHAR16)*5);

	tok = wcstok(tmpTODStr,L":");
	if (tok != NULL)
	{
		hours = _wtoi(tok);
		// check for invalid conversion, ie alpha chars
		// wtoi returns 0 if it cant convert, but we need this value
		// therefore if tok <> 0 then it was a bad convert.
		if (hours == 0 && wcscmp(tok,L"0") != 0)
		{
			// force error
			bTimeOK = false;
		}

		tok = wcstok(NULL,L".");
		if (tok != NULL)
		{
			mins = _wtoi(tok);
			if (mins == 0 && wcscmp(tok,L"0") != 0)
			{
				// force error
				bTimeOK = false;
			}

			// fix for single digits
			if (wcslen(tok) == 1)
				mins = mins * 10;
		}
		else
		{
			bTimeOK = false;
		}
	}
	else
	{
		bTimeOK = false;
	}


	if (bTimeOK)
	{
		giMPHTimeHours = hours;
		float ratio = (float)mins / 60.0f;
		giMPHTimeMins = (INT16)(ratio * 100);

	}
	else
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_TIME_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}


	// verify the Starting Balance
	Get16BitStringFromField( 4, gzStartingBalanceField );
	UINT32 sBalance = _wtoi(gzStartingBalanceField);
	if (sBalance < 1)
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_CASH_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	Get16BitStringFromField( 5, gzDmgMultiplierField );
	double fDmg = _wtof(gzDmgMultiplierField);
	if (fDmg <= 0.0f || fDmg >= 5.0f)
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_DMG_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	Get16BitStringFromField( 6, gzTimerField );
	UINT32 iTimer = _wtoi(gzTimerField);
	if (iTimer < 1 || iTimer > 200)
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_TIMER_INVALID], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}

	return true;
}

UINT32	MPHostScreenInit( void )
{
	// read settings from JA2_mp.ini
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"SERVER_NAME", L"Mr Server", gzServerNameField, 30, L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"MAX_CLIENTS", L"", gzMaxPlayersField, 2, L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"MAX_MERCS", L"", gzSquadSizeField, 2 , L"..\\Ja2_mp.ini" );
	guiMPHGameType = GetPrivateProfileIntW( L"Ja2_mp Settings",L"GAME_MODE", MP_TYPE_DEATHMATCH, L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"STARTING_BALANCE", L"9:30", gzStartingBalanceField, 10 , L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"DAMAGE_MULTIPLIER", L"0.7", gzDmgMultiplierField, 5 , L"..\\Ja2_mp.ini" );
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"TIMED_TURN_SECS_PER_TICK", L"100", gzTimerField, 5 , L"..\\Ja2_mp.ini" );

	giMPHRandomMercs = GetPrivateProfileIntW( L"Ja2_mp Settings",L"RANDOM_MERCS", 0, L"..\\Ja2_mp.ini" );
	giMPHSameMercs = GetPrivateProfileIntW( L"Ja2_mp Settings",L"SAME_MERC", 0, L"..\\Ja2_mp.ini" );
	giMPHReportMercs = GetPrivateProfileIntW( L"Ja2_mp Settings",L"REPORT_NAME", 0, L"..\\Ja2_mp.ini" );
	giMPHBobbyRays = GetPrivateProfileIntW( L"Ja2_mp Settings",L"DISABLE_BOBBY_RAYS", 0, L"..\\Ja2_mp.ini" );
	giMPHRandomSpawn = GetPrivateProfileIntW( L"Ja2_mp Settings",L"RANDOM_EDGES", 0, L"..\\Ja2_mp.ini" );
	giMPHEnableCivilians = GetPrivateProfileIntW( L"Ja2_mp Settings",L"CIV_ENABLED", 0, L"..\\Ja2_mp.ini" );
	giMPHUseNIV = GetPrivateProfileIntW( L"Ja2_mp Settings",L"ALLOW_CUSTOM_NIV", 0, L"..\\Ja2_mp.ini" );


	// read in time
	CHAR16 szTime[5];
	GetPrivateProfileStringW( L"Ja2_mp Settings",L"TIME", L"13.50", szTime, 5 , L"..\\Ja2_mp.ini" );
	
	wchar_t* tok;
	bool bTimeOK = true;
	int hours = 0;
	int mins = 0;

	tok = wcstok(szTime,L".");

	if (tok != NULL)
	{
		hours = _wtoi(tok);
		// check for invalid conversion, ie alpha chars
		// wtoi returns 0 if it cant convert, but we need this value
		// therefore if tok <> 0 then it was a bad convert.
		if (hours == 0 && wcscmp(tok,L"0") != 0)
		{
			// force error
			bTimeOK = false;
		}

		tok = wcstok(NULL,L".");
		if (tok != NULL)
		{
			mins = _wtoi(tok);
			if (mins == 0 && wcscmp(tok,L"0") != 0)
			{
				// force error
				bTimeOK = false;
			}

			// fix for single digits
			if (wcslen(tok) == 1)
				mins = mins * 10;
		}
		else
		{
			bTimeOK = false;
		}
	}
	else
	{
		bTimeOK = false;
	}

	

	if (bTimeOK)
	{
		giMPHTimeHours = hours;
		float ratio = (float)mins / 100.0f;
		giMPHTimeMins = (INT16)(ratio * 60);
		swprintf(gzTimeOfDayField,L"%i:%i",giMPHTimeHours,giMPHTimeMins);
	}

	return( 1 );
}


UINT32	MPHostScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfMPHScreenEntry )
	{
//		PauseGame();

		EnterMPHScreen();
		gfMPHScreenEntry = FALSE;
		gfMPHScreenExit = FALSE;
		InvalidateRegion( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	}

	GetMPHScreenUserInput();


	HandleMPHScreen();

	// render buttons marked dirty
	MarkButtonsDirty( );
	RenderButtons( );

	// render text boxes
	RenderAllTextFields(); // textbox system call

	// render help
//	RenderFastHelp( );
//	RenderButtonsFastHelp( );


	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender();

	// handle fades in and out
	if ( HandleFadeOutCallback( ) )
	{
		ClearMainMenu();
		return( gubMPHExitScreen );
	}

	if ( HandleBeginFadeOut( gubMPHExitScreen ) )
	{
		return( gubMPHExitScreen );
	}

	if ( HandleFadeInCallback( ) )
	{
		// Re-render the scene!
		RenderMPHScreen();
	}

	if ( HandleBeginFadeIn( gubMPHExitScreen ) )
	{

	}

	if( gfMPHScreenExit ) // we are exiting this screen
	{
		ExitMPHScreen(); // perform destruction
	}

	return( gubMPHExitScreen );
} // end MPHoinScreenHandle()


UINT32	MPHostScreenShutdown( void )
{
	return( 1 );
}

BOOLEAN		EnterMPHScreen()
{
	VOBJECT_DESC	VObjectDesc;
	UINT16					cnt;
	UINT16					usPosY;

	if( gfMPHButtonsAllocated )
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

	CHECKF(AddVideoObject(&VObjectDesc, &guiMPHMainBackGroundImage ));

	//Join button
	giMPHStartBtnImage = LoadButtonImage("INTERFACE\\PreferencesButtons.sti", -1,0,-1,2,-1 );
	guiMPHStartButton = CreateIconAndTextButton( giMPHStartBtnImage, gzMPHScreenText[MPH_START_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPH_BTN_START_X, MPH_BTN_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHStartCallback);

	SpecifyButtonSoundScheme( guiMPHStartButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyDisabledButtonStyle( guiMPHStartButton, DISABLED_STYLE_NONE );

	//Cancel button
	giMPHCancelBtnImage = UseLoadedButtonImage( giMPHStartBtnImage, -1,1,-1,3,-1 );
	guiMPHCancelButton = CreateIconAndTextButton( giMPHCancelBtnImage, gzMPHScreenText[MPH_CANCEL_TEXT], OPT_BUTTON_FONT,
													OPT_BUTTON_ON_COLOR, DEFAULT_SHADOW,
													OPT_BUTTON_OFF_COLOR, DEFAULT_SHADOW,
													TEXT_CJUSTIFIED,
													MPH_CANCEL_X, MPH_BTN_START_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHCancelCallback );
	SpecifyButtonSoundScheme( guiMPHCancelButton, BUTTON_SOUND_SCHEME_BIGSWITCH3 );

	// Initialise Text Boxes
	InitTextInputMode(); // API call to initialise text input mode for this screen
						 // does not mean we are inputting text right away

	// Player Name field
	SetTextInputCursor( CUROSR_IBEAM_WHITE );
	SetTextInputFont( (UINT16) FONT12ARIALFIXEDWIDTH ); //FONT12ARIAL //FONT12ARIALFIXEDWIDTH
	Set16BPPTextFieldColor( Get16BPPColor(FROMRGB( 0, 0, 0) ) );
	SetBevelColors( Get16BPPColor(FROMRGB(136, 138, 135)), Get16BPPColor(FROMRGB(24, 61, 81)) );
	SetTextInputRegularColors( FONT_WHITE, 2 );
	SetTextInputHilitedColors( 2, FONT_WHITE, FONT_WHITE	);
	SetCursorColor( Get16BPPColor(FROMRGB(255, 255, 255) ) );

	//AddUserInputField( NULL ); // API Call that sets a special input-handling routine and method for the TAB key

	//Add Player Name textbox 
	AddTextInputField(	MPH_TXT_SVRNAME_X,
						MPH_TXT_SVRNAME_Y, 
						MPH_TXT_SVRNAME_WIDTH,
						MPH_TXT_SVRNAME_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzServerNameField,
						30,
						INPUTTYPE_ASCII );//23

	//Add Num Players textbox 
	AddTextInputField(	MPH_TXT_MAXPLAYERS_X,
						MPH_TXT_MAXPLAYERS_Y, 
						MPH_TXT_MAXPLAYERS_WIDTH,
						MPH_TXT_MAXPLAYERS_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzMaxPlayersField,
						2,
						INPUTTYPE_ASCII );//23

	//Add Squad Size textbox 
	AddTextInputField(	MPH_TXT_SQUAD_X,
						MPH_TXT_SQUAD_Y, 
						MPH_TXT_SQUAD_WIDTH,
						MPH_TXT_SQUAD_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzSquadSizeField,
						2,
						INPUTTYPE_ASCII );//23

	//Add Time of day textbox 
	AddTextInputField(	MPH_TXT_TIME_X,
						MPH_TXT_TIME_Y, 
						MPH_TXT_TIME_WIDTH,
						MPH_TXT_TIME_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzTimeOfDayField,
						5,
						INPUTTYPE_ASCII );//23

	//Add Starting Cash textbox 
	AddTextInputField(	MPH_TXT_CASH_X,
						MPH_TXT_CASH_Y, 
						MPH_TXT_CASH_WIDTH,
						MPH_TXT_CASH_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzStartingBalanceField,
						10,
						INPUTTYPE_ASCII );//23

	//Add Damage Multiplyer textbox 
	AddTextInputField(	MPH_TXT_DMG_X,
						MPH_TXT_DMG_Y, 
						MPH_TXT_DMG_WIDTH,
						MPH_TXT_DMG_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzDmgMultiplierField,
						5,
						INPUTTYPE_ASCII );//23

	//Add Turn Timer textbox 
	AddTextInputField(	MPH_TXT_TIMER_X,
						MPH_TXT_TIMER_Y, 
						MPH_TXT_TIMER_WIDTH,
						MPH_TXT_TIMER_HEIGHT,
						MSYS_PRIORITY_HIGH+2,
						gzTimerField,
						5,
						INPUTTYPE_ASCII );//23


	SetActiveField( 0 ); // Playername textbox has focus


	//
	//Check box to toggle Difficulty settings
	//
	usPosY = MPH_DIF_SETTINGS_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y;

	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		// create each checkbox
		guiMPHDifficultySettingsToggles[ cnt ] = CreateCheckBoxButton(	MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TOGGLE_BOX, usPosY,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHDifficultyTogglesCallback );
		MSYS_SetBtnUserData( guiMPHDifficultySettingsToggles[ cnt ], 0, cnt );

		usPosY += MPH_GAP_BN_SETTINGS-5;
	}

	// Set default selection
	if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_EASY )
		ButtonList[ guiMPHDifficultySettingsToggles[ MPH_DIFF_EASY ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_MEDIUM )
		ButtonList[ guiMPHDifficultySettingsToggles[ MPH_DIFF_MED ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_HARD )
		ButtonList[ guiMPHDifficultySettingsToggles[ MPH_DIFF_HARD ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else if( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
		ButtonList[ guiMPHDifficultySettingsToggles[ MPH_DIFF_INSANE ] ]->uiFlags |= BUTTON_CLICKED_ON;

	else
		ButtonList[ guiMPHDifficultySettingsToggles[ MPH_DIFF_MED ] ]->uiFlags |= BUTTON_CLICKED_ON;




	//
	//Check box to toggle GameType settings
	//
	usPosY = MPH_GAMETYPE_SETTINGS_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y;

	for( cnt=0; cnt<NUM_MP_GAMETYPE; cnt++)
	{
		// create each checkbox
		guiMPHGameTypeToggles[ cnt ] = CreateCheckBoxButton(	MPH_GAMETYPE_SETTINGS_X+MPH_OFFSET_TO_TOGGLE_BOX, usPosY,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHGameTypeTogglesCallback );
		MSYS_SetBtnUserData( guiMPHGameTypeToggles[ cnt ], 0, cnt );

		usPosY += MPH_GAP_BN_SETTINGS-5;
	}
	// set default selection
	ButtonList[ guiMPHGameTypeToggles[ guiMPHGameType ] ]->uiFlags |= BUTTON_CLICKED_ON;


	// Random Mercs
	guiMPHRandomMercsToggle = CreateCheckBoxButton(	MPH_RNDMERC_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_RNDMERC_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHRandomMercCallback );
	if ( giMPHRandomMercs )
		ButtonList[ guiMPHRandomMercsToggle ]->uiFlags |= BUTTON_CLICKED_ON;

	//checkbox for same merc
	guiMPHSameMercToggle = CreateCheckBoxButton(	MPH_SAMEMERC_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_SAMEMERC_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHSameMercCallback );
	if ( giMPHSameMercs )
		ButtonList[ guiMPHSameMercToggle ]->uiFlags |= BUTTON_CLICKED_ON;

	//checkbox for report purchase
	guiMPHReportMercToggle  = CreateCheckBoxButton(	MPH_REPORTMERC_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_REPORTMERC_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHReportMercCallback );
	if (giMPHReportMercs)
		ButtonList[ guiMPHReportMercToggle ]->uiFlags |= BUTTON_CLICKED_ON;

	//checkbox for bobby rays
	guiMPHBobbyRayToggle  = CreateCheckBoxButton(	MPH_BOBBYRAY_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_BOBBYRAY_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHBobbyRayCallback );

	if (!giMPHBobbyRays)
		ButtonList[ guiMPHBobbyRayToggle ]->uiFlags |= BUTTON_CLICKED_ON; // the setting is actually DISABLE / NEGATIVE

	//checkbox for random spawn pos
	guiMPHRandomSpawnToggle = CreateCheckBoxButton(	MPH_RNDSPAWN_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_RNDSPAWN_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHRandomSpawnCallback );

	if (giMPHRandomSpawn)
		ButtonList[ guiMPHRandomSpawnToggle ]->uiFlags |= BUTTON_CLICKED_ON;

	// checkbox for civillians
	guiMPHCivsToggle = CreateCheckBoxButton(	MPH_CIVS_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_CIVS_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHCivsCallback );
	if (giMPHEnableCivilians)
		ButtonList[ guiMPHCivsToggle ]->uiFlags |= BUTTON_CLICKED_ON;

	// checkbox for use NIV
	guiMPHUseNIVToggle = CreateCheckBoxButton(	MPH_USENIV_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_USENIV_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y,
																		"INTERFACE\\OptionsCheck.sti", MSYS_PRIORITY_HIGH+10,
																		BtnMPHUseNIVCallback );
	if (giMPHUseNIV)
		ButtonList[ guiMPHUseNIVToggle ]->uiFlags |= BUTTON_CLICKED_ON;




	//Reset the exit screen - screen the main game loop will call next iteration
	gubMPHExitScreen = MP_HOST_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderMPHScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfMPHButtonsAllocated = TRUE;

	return( TRUE );

} // End of EnterMPHScreen()


BOOLEAN		ExitMPHScreen()
{
	UINT16	cnt;

	if( !gfMPHButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiMPHMainBackGroundImage );

	RemoveButton( guiMPHStartButton );
	RemoveButton( guiMPHCancelButton );


	UnloadButtonImage( giMPHCancelBtnImage );
	UnloadButtonImage( giMPHStartBtnImage );

	//Check box to toggle Difficulty settings
	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
		RemoveButton( guiMPHDifficultySettingsToggles[ cnt ] );

	//Check box to toggle Game Types
	for( cnt=0; cnt<NUM_MP_GAMETYPE; cnt++)
		RemoveButton( guiMPHGameTypeToggles[ cnt ] );

	RemoveButton( guiMPHRandomMercsToggle );
	RemoveButton( guiMPHSameMercToggle );
	RemoveButton( guiMPHReportMercToggle );
	RemoveButton( guiMPHBobbyRayToggle );
	RemoveButton( guiMPHRandomSpawnToggle );
	RemoveButton( guiMPHCivsToggle );
	RemoveButton( guiMPHUseNIVToggle );

	// exit text input mode in this screen and clean up text boxes
	KillAllTextInputModes();
	SetTextInputCursor( CURSOR_IBEAM );

	gfMPHButtonsAllocated = FALSE;

	//If we are starting the game stop playing the music
	// <TODO> review this, i think MPH_EXIT is the proceed mode...
	//if( gubMPHScreenHandler == MPH_EXIT )
	//	SetMusicMode( MUSIC_NONE );

	gfMPHScreenExit = FALSE;
	gfMPHScreenEntry = TRUE;

	return( TRUE );

} // End of ExitMPHScreen()


void			HandleMPHScreen()
{
	if( gubMPHScreenHandler != MPH_NOTHING )
	{
		switch( gubMPHScreenHandler )
		{
			case MPH_CANCEL:
				gubMPHExitScreen = MAINMENU_SCREEN;
				gfMPHScreenExit	= TRUE;
				break;


			case MPH_START:
			{
				//if we are already fading out, get out of here
				if( gFadeOutDoneCallback != DoneFadeOutForExitMPHScreen )
				{
					//Disable the ok button
					DisableButton( guiMPHStartButton );

					gFadeOutDoneCallback = DoneFadeOutForExitMPHScreen;

					FadeOutNextFrame( );
				}
				break;
			}

		}

		gubMPHScreenHandler = MPH_NOTHING;
	}


	if( gfReRenderMPHScreen )
	{
		RenderMPHScreen();
		gfReRenderMPHScreen = FALSE;
	}

	RestoreMPHButtonBackGrounds();
} // end of HandleMPHScreen


BOOLEAN		RenderMPHScreen()
{
	HVOBJECT	hPixHandle;
	UINT16		usPosY;

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiMPHMainBackGroundImage );
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,0,0, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	//Display the title
	DrawTextToScreen( gzMPHScreenText[ MPH_TITLE_TEXT ], MPH_MAIN_TITLE_X, MPH_MAIN_TITLE_Y, MPH_MAIN_TITLE_WIDTH, MPH_TITLE_FONT, MPH_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	
	// Server name text label
	DisplayWrappedString( MPH_LABEL_SVRNAME_X, MPH_LABEL_SVRNAME_Y, MPH_LABEL_SVRNAME_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_SERVERNAME_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Num Players text label
	DisplayWrappedString( MPH_LABEL_MAXPLAYERS_X, MPH_LABEL_MAXPLAYERS_Y, MPH_LABEL_MAXPLAYERS_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_NUMPLAYERS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Squad Size text label
	DisplayWrappedString( MPH_LABEL_SQUAD_X, MPH_LABEL_SQUAD_Y, MPH_LABEL_SQUAD_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_SQUADSIZE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Time Of day text label
	DisplayWrappedString( MPH_LABEL_TIME_X, MPH_LABEL_TIME_Y, MPH_LABEL_TIME_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_TIME_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Starting Cash text label
	DisplayWrappedString( MPH_LABEL_CASH_X, MPH_LABEL_CASH_Y, MPH_LABEL_CASH_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_BALANCE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Starting Cash text label
	DisplayWrappedString( MPH_LABEL_DMG_X, MPH_LABEL_DMG_Y, MPH_LABEL_DMG_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_DMG_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Starting Cash text label
	DisplayWrappedString( MPH_LABEL_TIMER_X, MPH_LABEL_TIMER_Y, MPH_LABEL_TIMER_WIDTH, 2, MPH_LABEL_TEXT_FONT, MPH_LABEL_TEXT_COLOR, gzMPHScreenText[ MPH_TIMER_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Display the Dif Settings Title Text
	DisplayWrappedString( MPH_DIF_SETTINGS_X, (UINT16)(MPH_DIF_SETTINGS_Y-MPH_GAP_BN_SETTINGS), MPH_DIF_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Novice
	usPosY = MPH_DIF_SETTINGS_Y+2;
	DisplayWrappedString( (UINT16)(MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_DIF_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_EASY_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Intermediate
	usPosY += MPH_GAP_BN_SETTINGS-5;
	DisplayWrappedString( (UINT16)(MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_DIF_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_MEDIUM_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Hard
	usPosY += MPH_GAP_BN_SETTINGS-5;
	DisplayWrappedString( (UINT16)(MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_DIF_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_HARD_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Insane
	usPosY += MPH_GAP_BN_SETTINGS-5;
	//DrawTextToScreen( gzMPHScreenText[ MPH_HARD_TEXT ], (UINT16)(MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_MAIN_TITLE_WIDTH, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );	
	DisplayWrappedString( (UINT16)(MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_DIF_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_INSANE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Display the Game Type title
	DisplayWrappedString( MPH_GAMETYPE_SETTINGS_X, (UINT16)(MPH_GAMETYPE_SETTINGS_Y-MPH_GAP_BN_SETTINGS), MPH_GAMETYPE_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_GAMETYPE_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Deathmatch
	usPosY = MPH_GAMETYPE_SETTINGS_Y+2;
	DisplayWrappedString( (UINT16)(MPH_GAMETYPE_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_GAMETYPE_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_DEATHMATCH_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Team Deathmatch
	usPosY += MPH_GAP_BN_SETTINGS-5;
	DisplayWrappedString( (UINT16)(MPH_GAMETYPE_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_GAMETYPE_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_TEAMDM_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Co-operative
	usPosY += MPH_GAP_BN_SETTINGS-5;
	DisplayWrappedString( (UINT16)(MPH_GAMETYPE_SETTINGS_X+MPH_OFFSET_TO_TEXT), usPosY, MPH_GAMETYPE_SETTINGS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_COOP_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Random Mercs
	DisplayWrappedString( (UINT16)MPH_RNDMERC_X, MPH_RNDMERC_Y, MPH_RNDMERC_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_RANDOMMERCS_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Same Mercs
	DisplayWrappedString( (UINT16)MPH_SAMEMERC_X, MPH_SAMEMERC_Y, MPH_SAMEMERC_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_SAMEMERC_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Report Mercs
	DisplayWrappedString( (UINT16)MPH_REPORTMERC_X, MPH_REPORTMERC_Y, MPH_REPORTMERC_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_RPTMERC_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Allow Bobby-Rays
	DisplayWrappedString( (UINT16)MPH_BOBBYRAY_X, MPH_BOBBYRAY_Y, MPH_BOBBYRAY_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_BOBBYRAY_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Random Spawn
	DisplayWrappedString( (UINT16)MPH_RNDSPAWN_X, MPH_RNDSPAWN_Y, MPH_RNDSPAWN_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_RNDMSTART_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Civs
	DisplayWrappedString( (UINT16)MPH_CIVS_X, MPH_CIVS_Y, MPH_CIVS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_ENABLECIV_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	// Use NIV
	DisplayWrappedString( (UINT16)MPH_USENIV_X, MPH_USENIV_Y, MPH_CIVS_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_USENIV_TEXT ], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );


	return( TRUE );
} // end of RenderMPHScreen()



void			GetMPHScreenUserInput()
{
	InputAtom Event;
//	POINT	MousePos;

//	GetCursorPos(&MousePos);

	while( DequeueEvent( &Event ) )
	{
		// check if this event is swallowed by text input, otherwise process key
		if( !HandleTextInput( &Event ) && Event.usEvent == KEY_DOWN )
		{
			switch( Event.usParam )
			{

				case ESC:
					//Exit out of the screen
					gubMPHScreenHandler = MPH_CANCEL;
					break;

				case ENTER:
					if (ValidateMPSettings())
					{
						SaveMPSettings();
						gubMPHScreenHandler = MPH_START;
					}
					break;
			}
		}
	}
} // end of GetMPHScreenUserInput()


// CALLBACKS

void BtnMPHStartCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if (ValidateMPSettings())
		{
			SaveMPSettings();
			gubMPHScreenHandler = MPH_START;
		}

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnMPHCancelCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gubMPHScreenHandler = MPH_CANCEL;

		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
}

void BtnMPHDifficultyTogglesCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		/*UINT8	ubButton = (UINT8)*/MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			UINT8	cnt;

			for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
			{
				ButtonList[ guiMPHDifficultySettingsToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			UINT8	cnt;
			BOOLEAN fAnyChecked=FALSE;

			//if none of the other boxes are checked, do not uncheck this box
			for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
			{

				if( ButtonList[ guiMPHDifficultySettingsToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}
			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHGameTypeTogglesCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		/*UINT8	ubButton = (UINT8)*/MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			UINT8	cnt;

			for( cnt=0; cnt<NUM_MP_GAMETYPE; cnt++)
			{
				ButtonList[ guiMPHGameTypeToggles[ cnt ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			}

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
		else
		{
			UINT8	cnt;
			BOOLEAN fAnyChecked=FALSE;

			//if none of the other boxes are checked, do not uncheck this box
			for( cnt=0; cnt<NUM_MP_GAMETYPE; cnt++)
			{

				if( ButtonList[ guiMPHGameTypeToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
				{
					fAnyChecked = TRUE;
				}
			}
			//if none are checked, re check this one
			if( !fAnyChecked )
				btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void RestoreMPHButtonBackGrounds()
{
	UINT8	cnt;
	UINT16 usPosY;


	usPosY = MPH_DIF_SETTINGS_Y-MPH_OFFSET_TO_TOGGLE_BOX_Y;
	//Check box to toggle Difficulty settings
	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		RestoreExternBackgroundRect( MPH_DIF_SETTINGS_X+MPH_OFFSET_TO_TOGGLE_BOX, usPosY, 34, 29 );
		usPosY += MPH_GAP_BN_SETTINGS-5;
	}

	usPosY = MPH_GAMETYPE_SETTINGS_Y-MPH_OFFSET_TO_TOGGLE_BOX_Y;
	//Check box to toggle GameType
	for( cnt=0; cnt<NUM_MP_GAMETYPE; cnt++)
	{
		RestoreExternBackgroundRect( MPH_GAMETYPE_SETTINGS_X+MPH_OFFSET_TO_TOGGLE_BOX, usPosY, 34, 29 );
		usPosY += MPH_GAP_BN_SETTINGS-5;
	}

	RestoreExternBackgroundRect( MPH_RNDMERC_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_RNDMERC_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );
	RestoreExternBackgroundRect( MPH_SAMEMERC_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_SAMEMERC_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );
	RestoreExternBackgroundRect( MPH_REPORTMERC_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_REPORTMERC_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );
	RestoreExternBackgroundRect( MPH_BOBBYRAY_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_BOBBYRAY_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );
	RestoreExternBackgroundRect( MPH_RNDSPAWN_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_RNDSPAWN_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );
	RestoreExternBackgroundRect( MPH_CIVS_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_CIVS_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );
	RestoreExternBackgroundRect( MPH_USENIV_X+MPH_OFFSET_TO_TOGGLE_BOX, MPH_USENIV_Y - MPH_OFFSET_TO_TOGGLE_BOX_Y, 34, 29 );

}

UINT8	GetMPHCurrentDifficultyButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		if( ButtonList[ guiMPHDifficultySettingsToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}

	return( 0 );
}

UINT8	GetMPHGameTypeButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<NUM_DIFF_SETTINGS; cnt++)
	{
		if( ButtonList[ guiMPHGameTypeToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}

	return( 0 );
}

BOOLEAN DoMPHMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1 };

	// reset exit mode
//	gfExitGioDueToMessageBox = TRUE;

	// do message box and return
	giMPHMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giMPHMessageBox != -1 ) );
}

void DoneFadeOutForExitMPHScreen( void )
{
	// As we bypassed the GIO screen, set up some game options for multiplayer here
	// <TODO> most things i have left as thier defaults here for testing.
	is_networked = true;
	is_host = true; // we want to be the host, not we ARE the host yet (is_server)
	auto_retry = true;
	giNumTries = 5;

	// loop through and get the status of all the buttons
	// Madd
	/*gGameOptions.fGunNut = GetCurrentGunButtonSetting();
	gGameOptions.ubGameStyle = GetCurrentGameStyleButtonSetting();
	gGameOptions.ubDifficultyLevel = GetCurrentDifficultyButtonSetting() + 1;*/
	// JA2Gold: no more timed turns setting
	//gGameOptions.fTurnTimeLimit = GetCurrentTimedTurnsButtonSetting();//hayden : re-enabled

	if (is_networked)
		gGameOptions.fTurnTimeLimit = TRUE;
	else
		gGameOptions.fTurnTimeLimit = FALSE;
	
	// JA2Gold: iron man
	//gGameOptions.fIronManMode = GetCurrentGameSaveButtonSetting();

	// Bobby Rays - why would we want anything less than the best
	gGameOptions.ubBobbyRay = BR_AWESOME;
	

	// CHRISL:
	/*if(IsNIVModeValid() == TRUE){
		switch ( GetCurrentINVOptionButtonSetting() )
		{
			case GIO_INV_OLD:
				gGameOptions.ubInventorySystem = INVENTORY_OLD;
				break;
			case GIO_INV_NEW:
				gGameOptions.ubInventorySystem = INVENTORY_NEW;
				break;
		}
	}*/

	//	gubGIOExitScreen = INIT_SCREEN;
	gubMPHExitScreen = INTRO_SCREEN;

	//set the fact that we should do the intro videos
//	gbIntroScreenMode = INTRO_BEGINING;
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
		SetIntroType( INTRO_BEGINING );

	ExitMPHScreen(); // cleanup please, if we called a fadeout then we didnt do it above

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitMPHScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}


void BtnMPHRandomMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHRandomMercsToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHSameMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHSameMercToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHReportMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHReportMercToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHBobbyRayCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHBobbyRayToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHRandomSpawnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHRandomSpawnToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHUseNIVCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHUseNIVToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void BtnMPHCivsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		MSYS_GetBtnUserData( btn, 0 );

		if( btn->uiFlags & BUTTON_CLICKED_ON )
		{
			ButtonList[ guiMPHCivsToggle ]->uiFlags &= ~BUTTON_CLICKED_ON;

			//enable the current button
			btn->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}