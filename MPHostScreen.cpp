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
	#include "cursors.h"
	#include "Intro.h"
	#include "Text.h"
	#include "Text Input.h"
	#include "_Ja25EnglishText.h"
	#include "Soldier Profile.h"
#endif

#include "gameloop.h"
#include "connect.h"
#include "network.h"
#include "saveloadscreen.h"

#include "GameInitOptionsScreen.h"

#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_init.h>
#include <vfs/Tools/vfs_property_container.h>
#include <vfs/Core/vfs_os_functions.h>
#include "MPJoinScreen.h"
#include "MainMenuScreen.h"
#include "Init.h"
#include "xml.h"

////////////////////////////////////////////
//
//	Global Defines
//
///////////////////////////////////////////

INT32		giMPHMessageBox = -1; // message box handle

#define		MPH_TITLE_FONT							FONT14ARIAL
#define		MPH_TITLE_COLOR							FONT_MCOLOR_WHITE

#define		MPH_LABEL_TEXT_FONT						FONT10ARIAL
#define		MPH_LABEL_TEXT_COLOR					FONT_MCOLOR_WHITE

#define		MPH_TOGGLE_TEXT_FONT					FONT10ARIAL
#define		MPH_TOGGLE_TEXT_COLOR					FONT_MCOLOR_WHITE

//buttons
#define		MPH_CANCEL_X							iScreenWidthOffset + ((320 - 115) / 2)

#define		MPH_BTN_START_X							iScreenWidthOffset + 320 + 105
#define		MPH_BTN_START_Y							iScreenHeightOffset + 435

//main title
#define		MPH_MAIN_TITLE_X						0
#define		MPH_MAIN_TITLE_Y						iScreenHeightOffset + 10
#define		MPH_MAIN_TITLE_WIDTH					SCREEN_WIDTH

#define		MPH_TITLE_DISTANCE						30 // higher means closer

#define		MPH_GAP_BN_SETTINGS						35
#define		MPH_OFFSET_TO_TEXT						36
#define		MPH_OFFSET_TO_TOGGLE_BOX				180
#define		MPH_OFFSET_TO_TOGGLE_BOX_Y				9

// ---------------------------------
// Y-Offset for Combo-Controls
#define		COMBO_Y_OFFSET							63
// Y-Offset for Check-Controls
#define		CHECK_Y_OFFSET							50

#define		CORRECTION_Y_OFFSET						(COMBO_Y_OFFSET - CHECK_Y_OFFSET)

// X-Offset for Checkbox-Controls
#define		CHECK_X_OFFSET							36
#define		COMBO_X_OFFSET							0

#define		CHECK_WIDTH								(MPH_OFFSET_TO_TOGGLE_BOX - MPH_OFFSET_TO_TEXT)
#define		COMBO_WIDTH								(MPH_OFFSET_TO_TOGGLE_BOX - MPH_OFFSET_TO_TEXT)
// ---------------------------------

/*********************************
	FIRST COLUMN
**********************************/

#define		FIRST_COLUMN_X							iScreenWidthOffset + 10
#define		FIRST_COLUMN_Y							iScreenHeightOffset + 75

#define		MPH_SYNC_SETTING_X						FIRST_COLUMN_X + CHECK_X_OFFSET
#define		MPH_SYNC_SETTING_Y						FIRST_COLUMN_Y - CORRECTION_Y_OFFSET
#define		MPH_SYNC_SETTING_WIDTH					CHECK_WIDTH

#define		MPH_SQUADSIZE_SETTING_X					FIRST_COLUMN_X + COMBO_X_OFFSET
#define		MPH_SQUADSIZE_SETTING_Y					MPH_SYNC_SETTING_Y + COMBO_Y_OFFSET + CORRECTION_Y_OFFSET
#define		MPH_SQUADSIZE_SETTING_WIDTH				COMBO_WIDTH

#define		MPH_TIMETURNS_SETTING_X					FIRST_COLUMN_X + COMBO_X_OFFSET
#define		MPH_TIMETURNS_SETTING_Y					MPH_SQUADSIZE_SETTING_Y + COMBO_Y_OFFSET
#define		MPH_TIMETURNS_SETTING_WIDTH				COMBO_WIDTH

#define		MPH_INVENTORY_SETTING_X					FIRST_COLUMN_X + COMBO_X_OFFSET
#define		MPH_INVENTORY_SETTING_Y					MPH_TIMETURNS_SETTING_Y + COMBO_Y_OFFSET
#define		MPH_INVENTORY_SETTING_WIDTH				COMBO_WIDTH

#define		MPH_BOBBYRAY_SETTING_X					FIRST_COLUMN_X + CHECK_X_OFFSET
#define		MPH_BOBBYRAY_SETTING_Y					MPH_INVENTORY_SETTING_Y + CHECK_Y_OFFSET
#define		MPH_BOBBYRAY_SETTING_WIDTH				CHECK_WIDTH

#define		MPH_REPORTMERC_SETTING_X				FIRST_COLUMN_X + CHECK_X_OFFSET
#define		MPH_REPORTMERC_SETTING_Y				MPH_BOBBYRAY_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		MPH_REPORTMERC_SETTING_WIDTH			CHECK_WIDTH


/*********************************
	SECOND COLUMN
**********************************/

#define		SECOND_COLUMN_X							iScreenWidthOffset + 210
#define		SECOND_COLUMN_Y							FIRST_COLUMN_Y

#define		MPH_MAXPLAYERS_SETTING_X				SECOND_COLUMN_X + COMBO_X_OFFSET
#define		MPH_MAXPLAYERS_SETTING_Y				SECOND_COLUMN_Y
#define		MPH_MAXPLAYERS_SETTING_WIDTH			COMBO_WIDTH

#define		MPH_STARTINGTIME_SETTING_X				SECOND_COLUMN_X + COMBO_X_OFFSET
#define		MPH_STARTINGTIME_SETTING_Y				MPH_MAXPLAYERS_SETTING_Y + COMBO_Y_OFFSET
#define		MPH_STARTINGTIME_SETTING_WIDTH			COMBO_WIDTH

#define		MPH_WEAPONDAMAGE_SETTING_X				SECOND_COLUMN_X + COMBO_X_OFFSET
#define		MPH_WEAPONDAMAGE_SETTING_Y				MPH_STARTINGTIME_SETTING_Y + COMBO_Y_OFFSET
#define		MPH_WEAPONDAMAGE_SETTING_WIDTH			COMBO_WIDTH

#define		MPH_TRAITS_SETTING_X					SECOND_COLUMN_X + CHECK_X_OFFSET
#define		MPH_TRAITS_SETTING_Y					MPH_WEAPONDAMAGE_SETTING_Y + CHECK_Y_OFFSET
#define		MPH_TRAITS_SETTING_WIDTH				CHECK_WIDTH

#define		MPH_HIREMERC_SETTING_X					SECOND_COLUMN_X + CHECK_X_OFFSET
#define		MPH_HIREMERC_SETTING_Y					MPH_TRAITS_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		MPH_HIREMERC_SETTING_WIDTH				CHECK_WIDTH

#define		MPH_CIVS_SETTING_X						SECOND_COLUMN_X + CHECK_X_OFFSET
#define		MPH_CIVS_SETTING_Y						MPH_HIREMERC_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		MPH_CIVS_SETTING_WIDTH					CHECK_WIDTH

/*********************************
	THIRD COLUMN
**********************************/

#define		THIRD_COLUMN_X							iScreenWidthOffset + 413
#define		THIRD_COLUMN_Y							FIRST_COLUMN_Y

#define		MPH_GAMETYPE_SETTING_X					THIRD_COLUMN_X + COMBO_X_OFFSET
#define		MPH_GAMETYPE_SETTING_Y					THIRD_COLUMN_Y
#define		MPH_GAMETYPE_SETTING_WIDTH				COMBO_WIDTH

#define		MPH_STARTINGCASH_SETTING_X				THIRD_COLUMN_X + COMBO_X_OFFSET
#define		MPH_STARTINGCASH_SETTING_Y				MPH_GAMETYPE_SETTING_Y + COMBO_Y_OFFSET
#define		MPH_STARTINGCASH_SETTING_WIDTH			COMBO_WIDTH

#define		MPH_SECTOREDGE_SETTING_X				THIRD_COLUMN_X + CHECK_X_OFFSET
#define		MPH_SECTOREDGE_SETTING_Y				MPH_STARTINGCASH_SETTING_Y + CHECK_Y_OFFSET
#define		MPH_SECTOREDGE_SETTING_WIDTH			CHECK_WIDTH

#define		MPH_DIFFICULTY_SETTING_X				THIRD_COLUMN_X + COMBO_X_OFFSET
#define		MPH_DIFFICULTY_SETTING_Y				MPH_SECTOREDGE_SETTING_Y + COMBO_Y_OFFSET + CORRECTION_Y_OFFSET
#define		MPH_DIFFICULTY_SETTING_WIDTH			COMBO_WIDTH

#define		MPH_MAXENEMIES_SETTING_X				THIRD_COLUMN_X + CHECK_X_OFFSET
#define		MPH_MAXENEMIES_SETTING_Y				MPH_DIFFICULTY_SETTING_Y + CHECK_Y_OFFSET
#define		MPH_MAXENEMIES_SETTING_WIDTH			CHECK_WIDTH

#define		MPH_SAMEMERC_SETTING_X					THIRD_COLUMN_X + CHECK_X_OFFSET
#define		MPH_SAMEMERC_SETTING_Y					MPH_MAXENEMIES_SETTING_Y + CHECK_Y_OFFSET + CORRECTION_Y_OFFSET
#define		MPH_SAMEMERC_SETTING_WIDTH				CHECK_WIDTH

///////////////////////////////////////////
//
//	Global Variables
//
///////////////////////////////////////////

BOOLEAN		gfMPHScreenEntry = TRUE;
BOOLEAN		gfMPHScreenExit	= FALSE;
BOOLEAN		gfReRenderMPHScreen=TRUE;
BOOLEAN		gfMPHButtonsAllocated = FALSE;

// RW: 
UINT32		guiMPHSMALLFRAME;

void RenderMPHSmallSelectionFrame(INT16 sX, INT16 sY);

// ----------------
// Traits
enum
{
	MPH_TRAITS_OLD = 0,
	MPH_TRAITS_NEW = 1,

	MPH_NUM_TRAITS_OPTIONS,
};

UINT32	guiMPHTraitsOptionTogglesImage[ MPH_NUM_TRAITS_OPTIONS ];
UINT32	guiMPHTraitsOptionToggles[ MPH_NUM_TRAITS_OPTIONS ];
void BtnMPHOldTraitsCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHNewTraitsCallback(GUI_BUTTON *btn,INT32 reason);
void MTHNewTraitsNotPossibleMessageBoxCallBack( UINT8 bExitValue );

void BtnMPHOldTraitsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_TRAITS_SETTING_X), (MPH_TRAITS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHTraitsOptionToggles[ MPH_TRAITS_OLD ], BUTTON_SOUND_CLICKED_ON );
	}
}

BOOLEAN DoMPHMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback )
{
	SGPRect CenteringRect= {0 + xResOffset, 0, SCREEN_WIDTH - xResOffset, SCREEN_HEIGHT };

	// do message box and return
	giMPHMessageBox = DoMessageBox(	ubStyle,	zString,	uiExitScreen, ( UINT16 ) ( usFlags| MSG_BOX_FLAG_USE_CENTERING_RECT ),	ReturnCallback,	&CenteringRect );

	// send back return state
	return( ( giMPHMessageBox != -1 ) );
}

UINT8	GetMPHCurrentTraitsOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_TRAITS_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHTraitsOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

void BtnMPHNewTraitsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if (!gGameExternalOptions.fReadProfileDataFromXML)
		{
			PlayButtonSound( guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ], BUTTON_SOUND_DISABLED_CLICK );

			DoMPHMessageBox( MSG_BOX_BASIC_STYLE, zGioNewTraitsImpossibleText[0], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, MTHNewTraitsNotPossibleMessageBoxCallBack );
		}
		else
		{
			RestoreExternBackgroundRect( (MPH_TRAITS_SETTING_X), (MPH_TRAITS_SETTING_Y + 10), 230, 40 );

			ButtonList[ guiMPHTraitsOptionToggles[ MPH_TRAITS_OLD ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
			btn->uiFlags|=(BUTTON_CLICKED_ON);
				
			PlayButtonSound( guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ], BUTTON_SOUND_CLICKED_ON );
		}
	}
}

void MTHNewTraitsNotPossibleMessageBoxCallBack( UINT8 bExitValue )
{
}

// ----------------

// -------------
// Sync
enum
{
	MPH_SYNC_NO = 0,
	MPH_SYNC_YES = 1,

	MPH_NUM_SYNC_OPTIONS,
};

UINT32	guiMPHSyncOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHSyncOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHNoSyncCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHYesSyncCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHNoSyncCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_SYNC_SETTING_X), (MPH_SYNC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHSyncOptionToggles[ MPH_SYNC_YES ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHSyncOptionToggles[ MPH_SYNC_NO ], BUTTON_SOUND_CLICKED_ON );
	}
}
void BtnMPHYesSyncCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_SYNC_SETTING_X), (MPH_SYNC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHSyncOptionToggles[ MPH_SYNC_NO ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHSyncOptionToggles[ MPH_SYNC_YES ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentSyncOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_SYNC_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHSyncOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// MaxEnemies
enum
{
	MPH_MAXENEMIES_NO = 0,
	MPH_MAXENEMIES_YES = 1,

	MPH_NUM_MAXENEMIES_OPTIONS,
};

UINT32	guiMPHMaxEnemiesOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHMaxEnemiesOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHNoMaxEnemiesCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHYesMaxEnemiesCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHNoMaxEnemiesCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_MAXENEMIES_SETTING_X), (MPH_MAXENEMIES_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_YES ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_NO ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHYesMaxEnemiesCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_MAXENEMIES_SETTING_X), (MPH_MAXENEMIES_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_NO ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_YES ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentMaxEnemiesOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_MAXENEMIES_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHMaxEnemiesOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// Civs
enum
{
	MPH_CIVS_NO = 0,
	MPH_CIVS_YES = 1,

	MPH_NUM_CIVS_OPTIONS,
};

UINT32	guiMPHCivsOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHCivsOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHNoCivsCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHYesCivsCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHNoCivsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_CIVS_SETTING_X), (MPH_CIVS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHCivsOptionToggles[ MPH_CIVS_YES ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHCivsOptionToggles[ MPH_CIVS_NO ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHYesCivsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_CIVS_SETTING_X), (MPH_CIVS_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHCivsOptionToggles[ MPH_CIVS_NO ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHCivsOptionToggles[ MPH_CIVS_YES ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentCivsOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_CIVS_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHCivsOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// ReportMerc
enum
{
	MPH_REPORTMERC_NO = 0,
	MPH_REPORTMERC_YES = 1,

	MPH_NUM_REPORTMERC_OPTIONS,
};

UINT32	guiMPHReportMercOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHReportMercOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHNoReportMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHYesReportMercCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHNoReportMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_REPORTMERC_SETTING_X), (MPH_REPORTMERC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHReportMercOptionToggles[ MPH_REPORTMERC_YES ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHReportMercOptionToggles[ MPH_REPORTMERC_NO ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHYesReportMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_REPORTMERC_SETTING_X), (MPH_REPORTMERC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHReportMercOptionToggles[ MPH_REPORTMERC_NO ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHReportMercOptionToggles[ MPH_REPORTMERC_YES ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentReportMercOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_REPORTMERC_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHReportMercOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// HireMerc
enum
{
	MPH_HIREMERC_NORMAL = 0,
	MPH_HIREMERC_RANDOM = 1,	

	MPH_NUM_HIREMERC_OPTIONS,
};

UINT32	guiMPHHireMercOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHHireMercOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHRandomHireMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHNormalHireMercCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHRandomHireMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_HIREMERC_SETTING_X), (MPH_HIREMERC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHHireMercOptionToggles[ MPH_HIREMERC_NORMAL ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHHireMercOptionToggles[ MPH_HIREMERC_RANDOM ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHNormalHireMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_HIREMERC_SETTING_X), (MPH_HIREMERC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHHireMercOptionToggles[ MPH_HIREMERC_RANDOM ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHHireMercOptionToggles[ MPH_HIREMERC_NORMAL ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentHireMercOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_HIREMERC_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHHireMercOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// SectorEdge
enum
{	
	MPH_SECTOREDGE_SELECTABLE = 0,
	MPH_SECTOREDGE_RANDOM = 1,

	MPH_NUM_SECTOREDGE_OPTIONS,
};

UINT32	guiMPHSectorEdgeOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHSectorEdgeOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHRandomSectorEdgeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHSelectableSectorEdgeCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHRandomSectorEdgeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_SECTOREDGE_SETTING_X), (MPH_SECTOREDGE_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_SELECTABLE ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_RANDOM ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHSelectableSectorEdgeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_SECTOREDGE_SETTING_X), (MPH_SECTOREDGE_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_RANDOM ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_SELECTABLE ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentSectorEdgeOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_SECTOREDGE_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHSectorEdgeOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// BobbyRay
enum
{
	MPH_BOBBYRAY_ALLOW = 0,
	MPH_BOBBYRAY_DISABLE = 1,	

	MPH_NUM_BOBBYRAY_OPTIONS,
};

UINT32	guiMPHBobbyRayOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHBobbyRayOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHDisableBobbyRayCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHAllowBobbyRayCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHDisableBobbyRayCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_BOBBYRAY_SETTING_X), (MPH_BOBBYRAY_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_ALLOW ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_DISABLE ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHAllowBobbyRayCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_BOBBYRAY_SETTING_X), (MPH_BOBBYRAY_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_DISABLE ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_ALLOW ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentBobbyRayOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_BOBBYRAY_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHBobbyRayOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// SameMerc
enum
{
	MPH_SAMEMERC_DISABLE = 0,
	MPH_SAMEMERC_ALLOW = 1,

	MPH_NUM_SAMEMERC_OPTIONS,
};

UINT32	guiMPHSameMercOptionTogglesImage[ MPH_NUM_SYNC_OPTIONS ];
UINT32	guiMPHSameMercOptionToggles[ MPH_NUM_SYNC_OPTIONS ];
void BtnMPHDisableSameMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMPHAllowSameMercCallback(GUI_BUTTON *btn,INT32 reason);

void BtnMPHDisableSameMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_SAMEMERC_SETTING_X), (MPH_SAMEMERC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHSameMercOptionToggles[ MPH_SAMEMERC_ALLOW ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHSameMercOptionToggles[ MPH_SAMEMERC_DISABLE ], BUTTON_SOUND_CLICKED_ON );
	}
}

void BtnMPHAllowSameMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		RestoreExternBackgroundRect( (MPH_SAMEMERC_SETTING_X), (MPH_SAMEMERC_SETTING_Y + 10), 230, 40 );

		ButtonList[ guiMPHSameMercOptionToggles[ MPH_SAMEMERC_DISABLE ] ]->uiFlags &= ~BUTTON_CLICKED_ON;
		btn->uiFlags|=(BUTTON_CLICKED_ON);
			
		PlayButtonSound( guiMPHSameMercOptionToggles[ MPH_SAMEMERC_ALLOW ], BUTTON_SOUND_CLICKED_ON );
	}
}

UINT8	GetCurrentSameMercOptionButtonSetting()
{
	UINT8	cnt;

	for( cnt=0; cnt<MPH_NUM_SAMEMERC_OPTIONS; cnt++)
	{
		if( ButtonList[ guiMPHSameMercOptionToggles[ cnt ] ]->uiFlags & BUTTON_CLICKED_ON )
		{
			return( cnt );
		}
	}
	return( 0 );
}

// -------------

// -------------
// Difficulty
enum
{
	MPH_DIFF_EASY = 0,
	MPH_DIFF_MED = 1,
	MPH_DIFF_HARD = 2,
	MPH_DIFF_INSANE = 3,

	MPH_NUM_DIFF_SETTINGS,
};

INT8 iMPHDifficulty;

UINT32 giMPHDifficultyButton[ 2 ];
INT32 giMPHDifficultyButtonImage[ 2 ];
void BtnMPHDifficultySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHDifficultySelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHDifficultySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHDifficulty > 0 )
		{
			PlayButtonSound( giMPHDifficultyButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHDifficulty--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHDifficultyButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHDifficultySelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if ( iMPHDifficulty < MaxDifficultySettingsValues - 1)
		//if ( iMPHDifficulty < 3 )
		{
			PlayButtonSound( giMPHDifficultyButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHDifficulty++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHDifficultyButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// GameType
enum
{
	MPH_GT_DEATHMATCH = 0,
	MPH_GT_TEAM_DEATHMATCH = 1,
	MPH_GT_COOP = 2,	

	MPH_NUM_GAMETYPE_SETTINGS,
};

INT8 iMPHGameType;

UINT32 giMPHGameTypeButton[ 2 ];
INT32 giMPHGameTypeButtonImage[ 2 ];
void BtnMPHGameTypeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHGameTypeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHGameTypeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHGameType > MPH_GT_DEATHMATCH )
		{
			PlayButtonSound( giMPHGameTypeButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHGameType--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHGameTypeButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHGameTypeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHGameType < MPH_GT_COOP )
		{
			PlayButtonSound( giMPHGameTypeButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHGameType++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHGameTypeButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// MaxPlayers
enum
{
	MPH_MAXPLAYERS_2 = 2,
	MPH_MAXPLAYERS_3 = 3,
	MPH_MAXPLAYERS_4 = 4,
	
	MPH_NUM_MAXPLAYERS_SETTINGS,
};

INT8 iMPHMaxPlayers;

UINT32 giMPHMaxPlayersButton[ 2 ];
INT32 giMPHMaxPlayersButtonImage[ 2 ];
void BtnMPHMaxPlayersSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHMaxPlayersSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHMaxPlayersSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHMaxPlayers > MPH_MAXPLAYERS_2 )
		{
			PlayButtonSound( giMPHMaxPlayersButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHMaxPlayers--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHMaxPlayersButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHMaxPlayersSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHMaxPlayers < MPH_MAXPLAYERS_4 )
		{
			PlayButtonSound( giMPHMaxPlayersButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHMaxPlayers++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHMaxPlayersButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// Inventory
enum
{
	MPH_INV_OLD = 0,
	MPH_INV_NEW = 1,
	MPH_INV_NEW_NAS = 2,

	MPH_NUM_INV_OPTIONS,
};

INT8 iMPHInventory;

UINT32 giMPHInventoryButton[ 2 ];
INT32 giMPHInventoryButtonImage[ 2 ];
void BtnMPHInventorySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHInventorySelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHInventorySelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHInventory > MPH_INV_OLD )
		{
			PlayButtonSound( giMPHInventoryButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHInventory--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHInventoryButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHInventorySelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHInventory < MPH_INV_NEW_NAS )
		{
			PlayButtonSound( giMPHInventoryButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHInventory++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHInventoryButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// StartingTime
enum
{
	MPH_STARTINGTIME_MORNING = 0,
	MPH_STARTINGTIME_AFTERNOON = 1,
	MPH_STARTINGTIME_NIGHT = 2,
	
	MPH_NUM_STARTINGTIME_SETTINGS,
};

INT8 iMPHStartingTime;

UINT32 giMPHStartingTimeButton[ 2 ];
INT32 giMPHStartingTimeButtonImage[ 2 ];
void BtnMPHStartingTimeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHStartingTimeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHStartingTimeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHStartingTime > MPH_STARTINGTIME_MORNING )
		{
			PlayButtonSound( giMPHStartingTimeButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHStartingTime--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHStartingTimeButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHStartingTimeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHStartingTime < MPH_STARTINGTIME_NIGHT )
		{
			PlayButtonSound( giMPHStartingTimeButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHStartingTime++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHStartingTimeButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// StartingCash
enum
{
	MPH_STARTINGCASH_LOW = 0,
	MPH_STARTINGCASH_MEDIUM = 1,
	MPH_STARTINGCASH_HIGH = 2,
	MPH_STARTINGCASH_UNLIMITED = 3,
	
	MPH_NUM_STARTINGCASH_SETTINGS,
};

INT8 iMPHStartingCash;

UINT32 giMPHStartingCashButton[ 2 ];
INT32 giMPHStartingCashButtonImage[ 2 ];
void BtnMPHStartingCashSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHStartingCashSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHStartingCashSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHStartingCash > MPH_STARTINGCASH_LOW )
		{
			PlayButtonSound( giMPHStartingCashButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHStartingCash--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHStartingCashButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHStartingCashSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHStartingCash < MPH_STARTINGCASH_UNLIMITED )
		{
			PlayButtonSound( giMPHStartingCashButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHStartingCash++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHStartingCashButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// WeaponDamage
enum
{
	MPH_WEAPONDAMAGE_VERYLOW = 0,
	MPH_WEAPONDAMAGE_LOW = 1,
	MPH_WEAPONDAMAGE_NORMAL = 2,
	
	MPH_NUM_WEAPONDAMAGE_SETTINGS,
};

INT8 iMPHWeaponDamage;

UINT32 giMPHWeaponDamageButton[ 2 ];
INT32 giMPHWeaponDamageButtonImage[ 2 ];
void BtnMPHWeaponDamageSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHWeaponDamageSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHWeaponDamageSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHWeaponDamage > MPH_WEAPONDAMAGE_VERYLOW )
		{
			PlayButtonSound( giMPHWeaponDamageButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHWeaponDamage--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHWeaponDamageButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHWeaponDamageSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHWeaponDamage < MPH_WEAPONDAMAGE_NORMAL )
		{
			PlayButtonSound( giMPHWeaponDamageButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHWeaponDamage++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHWeaponDamageButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// TimeTurns
enum
{
	MPH_TIMETURNS_NEVER = 0,
	MPH_TIMETURNS_SLOW = 1,
	MPH_TIMETURNS_MEDIUM = 2,
	MPH_TIMETURNS_FAST = 3,
	
	MPH_NUM_TIMETURNS_SETTINGS,
};

INT8 iMPHTimeTurns;

UINT32 giMPHTimeTurnsButton[ 2 ];
INT32 giMPHTimeTurnsButtonImage[ 2 ];
void BtnMPHTimeTurnsSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHTimeTurnsSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHTimeTurnsSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHTimeTurns > MPH_TIMETURNS_NEVER )
		{
			PlayButtonSound( giMPHTimeTurnsButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHTimeTurns--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHTimeTurnsButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHTimeTurnsSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHTimeTurns < MPH_TIMETURNS_FAST )
		{
			PlayButtonSound( giMPHTimeTurnsButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHTimeTurns++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHTimeTurnsButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

// -------------
// SquadSize
enum
{
	MPH_SQUADSIZE_1 = 1,
	MPH_SQUADSIZE_2 = 2,
	MPH_SQUADSIZE_3 = 3,
	MPH_SQUADSIZE_4 = 4,
	MPH_SQUADSIZE_5 = 5,
	MPH_SQUADSIZE_6 = 6,
	
	MPH_NUM_SQUADSIZE_SETTINGS,
};

INT8 iMPHSquadSize;

UINT32 giMPHSquadSizeButton[ 2 ];
INT32 giMPHSquadSizeButtonImage[ 2 ];
void BtnMPHSquadSizeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason );
void BtnMPHSquadSizeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason );

void BtnMPHSquadSizeSelectionLeftCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT || reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHSquadSize > MPH_SQUADSIZE_1 )
		{
			PlayButtonSound( giMPHSquadSizeButton[0], BUTTON_SOUND_CLICKED_ON );

			iMPHSquadSize--;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHSquadSizeButton[0], BUTTON_SOUND_DISABLED_CLICK );
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

void BtnMPHSquadSizeSelectionRightCallback( GUI_BUTTON *btn,INT32 reason )
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if( reason & MSYS_CALLBACK_REASON_LBUTTON_REPEAT ||  reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( iMPHSquadSize < MPH_SQUADSIZE_6 )
		{
			PlayButtonSound( giMPHSquadSizeButton[1], BUTTON_SOUND_CLICKED_ON );

			iMPHSquadSize++;
			gfReRenderMPHScreen =TRUE;
		}
		else
		{
			PlayButtonSound( giMPHSquadSizeButton[1], BUTTON_SOUND_DISABLED_CLICK );
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

// -------------

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

// ------------
// String fields
// Wide-char strings that will hold the variables until they are transferred to the CHAR ascii fields
CHAR16		gzServerNameField[ 30 ] = {0} ;
CHAR16		gzFileTransferDirectory[ 100 ] = {0} ;
CHAR16		gzKitBag[ 100 ] = {0};

// ------------
// Controls with Multi Selection!
UINT8		guiMPHGameType;
UINT8		guiMPHDifficultLevel;
UINT8		guiMPHMaxPlayers;
UINT8		guiMPHSquadSize;
UINT8		guiMPHStartingTime;
UINT8		guiMPHStartingCash;
UINT8		guiMPHTimeTurns;
UINT8		guiMPHWeaponDamage;
UINT8		guiMPHInventory;
// ------------

// ------------
// Controls with YES/NO
UINT8		guiMPHMaxEnemies;
UINT8		guiMPHSendFiles;
UINT8		guiMPHHireMerc;
UINT8		guiMPHSectorEdge;
UINT8		guiMPHBobbyRay;
UINT8		guiMPHSameMerc;
UINT8		guiMPHReportMerc;
UINT8		guiMPHCivs;
UINT8		guiMPHNewTraits;
// ------------

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
bool		ValidateMPSettings();
void		SaveMPSettings();
UINT8		GetMPHGameTypeButtonSetting();
BOOLEAN		DoMPHMessageBox( UINT8 ubStyle, const STR16 zString, UINT32 uiExitScreen, UINT16 usFlags, MSGBOX_CALLBACK ReturnCallback );
void		DoneFadeOutForExitMPHScreen( void );
void		DoneFadeInForExitMPHScreen( void );

////////////////////////////////////////////
//
//	Functions
//
///////////////////////////////////////////

void		SaveMPSettings()
{	
	MpIniExists();
	vfs::PropertyContainer props;
	props.initFromIniFile(JA2MP_INI_FILENAME);
	
	guiMPHMaxPlayers = iMPHMaxPlayers;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_MAX_CLIENTS, guiMPHMaxPlayers);
	
	guiMPHSquadSize = iMPHSquadSize;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_MAX_MERCS, guiMPHSquadSize);
	
	guiMPHStartingCash = iMPHStartingCash;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_STARTING_BALANCE, guiMPHStartingCash);
	
	guiMPHWeaponDamage = iMPHWeaponDamage;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_DAMAGE_MULTIPLIER, guiMPHWeaponDamage);
	
	guiMPHTimeTurns = iMPHTimeTurns;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_TIMED_TURN_SECS_PER_TICK, guiMPHTimeTurns);
	
	guiMPHGameType = iMPHGameType;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_GAME_MODE, guiMPHGameType);
	
	guiMPHStartingTime = iMPHStartingTime;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_TIME, guiMPHStartingTime);
	
	guiMPHMaxEnemies = GetCurrentMaxEnemiesOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_OVERRIDE_MAX_AI, guiMPHMaxEnemies);
	
	guiMPHHireMerc = GetCurrentHireMercOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_RANDOM_MERCS, guiMPHHireMerc);
	
	guiMPHSameMerc = GetCurrentSameMercOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_SAME_MERC, guiMPHSameMerc);
	
	guiMPHBobbyRay = GetCurrentBobbyRayOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_DISABLE_BOBBY_RAYS, guiMPHBobbyRay);
	
	guiMPHReportMerc = GetCurrentReportMercOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_REPORT_NAME, guiMPHReportMerc);
	
	guiMPHSectorEdge = GetCurrentSectorEdgeOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_RANDOM_EDGES, guiMPHSectorEdge);
	
	guiMPHCivs = GetCurrentCivsOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_CIV_ENABLED, guiMPHCivs);
	
	guiMPHInventory = iMPHInventory;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_ALLOW_CUSTOM_NIV, guiMPHInventory);
	
	guiMPHSendFiles = GetCurrentSyncOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_SYNC_CLIENTS_MP_DIR, guiMPHSendFiles);

	guiMPHNewTraits = GetMPHCurrentTraitsOptionButtonSetting();
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_NEW_TRAITS, guiMPHNewTraits);

	guiMPHDifficultLevel = iMPHDifficulty + 1;
	props.setIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_DIFFICULT_LEVEL, guiMPHDifficultLevel - 1);

	// This writes the data back (from the GUI) to the Profiles/UserProfile/ja2_mp.ini
	props.writeToIniFile(JA2MP_INI_FILENAME, false);	
}

bool	ValidateMPSettings()
{	
	vfs::Path sUserDir(gzFileTransferDirectory);
	if(!vfs::OS::checkRealDirectory(sUserDir))
	{
		DoMPHMessageBox( MSG_BOX_BASIC_STYLE, gzMPHScreenText[MPH_FILE_TRANSFER_DIR_NOT_EXIST], MP_HOST_SCREEN, MSG_BOX_FLAG_OK, NULL );
		return false;
	}
	
	return true;
}

UINT32	MPHostScreenInit( void )
{
	memset( &gGameOptions, 0, sizeof( GAME_OPTIONS ) );

	// this is wrong as ScreenInit is called at game start, but the settigs can change anytime
	// has be called in ScreenEnter for example
	std::vector<CHAR16> szTime(6,0);

	// read settings from JA2_mp.ini
	MpIniExists();
	vfs::PropertyContainer props;
	props.initFromIniFile(JA2MP_INI_FILENAME);
			
	// ------------
	// Get the data from ja2_mp.ini
	// ------------

	props.getStringProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_SERVER_NAME, gzServerNameField, 30, L"My JA2 Server");
	
	guiMPHMaxPlayers =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_MAX_CLIENTS, 4);
	
	guiMPHSquadSize =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_MAX_MERCS, 6);
	
	guiMPHStartingCash =	(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_STARTING_BALANCE, 1);
	
	guiMPHWeaponDamage =	(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_DAMAGE_MULTIPLIER, 1);
	
	guiMPHTimeTurns =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_TIMED_TURN_SECS_PER_TICK, 2);
	
	props.getStringProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_FILE_TRANSFER_DIRECTORY, gzFileTransferDirectory, 100, L"MULTIPLAYER/Servers/My Server");
			
	props.getStringProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_KIT_BAG, gzKitBag, 100, L"");
		
	guiMPHStartingTime =	(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_TIME, 1);
	
	guiMPHMaxEnemies =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_OVERRIDE_MAX_AI, 0);
	
	guiMPHNewTraits =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_NEW_TRAITS, 0);
	
	// It is not allowed to play with new traits
	if (!gGameExternalOptions.fReadProfileDataFromXML)
		guiMPHNewTraits = 0;

	
	guiMPHHireMerc =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_RANDOM_MERCS, 0);
	
	guiMPHSameMerc =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_SAME_MERC, 1);
	
	guiMPHReportMerc =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_REPORT_NAME, 1);
	
	guiMPHBobbyRay =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_DISABLE_BOBBY_RAYS, 0);
	
	guiMPHSectorEdge =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_RANDOM_EDGES, 0);
	
	guiMPHCivs =			(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_CIV_ENABLED, 0);
	
	guiMPHInventory =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION, JA2MP_ALLOW_CUSTOM_NIV, 0);
	
	// It is not allowed to play with NIV
	if (!IsNIVModeValid(true))
		guiMPHInventory = 0;

	guiMPHSendFiles =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_SYNC_CLIENTS_MP_DIR, 1);

	guiMPHGameType =		(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_GAME_MODE, MP_TYPE_DEATHMATCH);

	guiMPHDifficultLevel =	(UINT8)props.getIntProperty(JA2MP_INI_INITIAL_SECTION,JA2MP_DIFFICULT_LEVEL, 3) + 1;
		
	return( 1 );
}


UINT32	MPHostScreenHandle( void )
{
	StartFrameBufferRender();

	if( gfMPHScreenEntry )
	{
		// need to reload ja2_mp.ini
		MPHostScreenInit();

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
}


UINT32	MPHostScreenShutdown( void )
{
	return( 1 );
}

BOOLEAN		EnterMPHScreen()
{
	VOBJECT_DESC	VObjectDesc, VObjectDesc2;

	if( gfMPHButtonsAllocated )
		return( TRUE );

	SetCurrentCursorFromDatabase( CURSOR_NORMAL );

		// load the Main trade screen backgroiund image
	//ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround.sti", VObjectDesc.ImageFile);
	else if (iResolution < _1024x768)
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_800x600.sti", VObjectDesc.ImageFile);
	else
		FilenameForBPP("INTERFACE\\OptionsScreenBackGround_1024x768.sti", VObjectDesc.ImageFile);


	CHECKF(AddVideoObject(&VObjectDesc, &guiMPHMainBackGroundImage ));

	VObjectDesc2.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\GIOSmallFrame.sti", VObjectDesc2.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc2, &guiMPHSMALLFRAME));

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
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// MAX PLAYERS

	giMPHMaxPlayersButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHMaxPlayersButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHMaxPlayersButton[ 0 ] = QuickCreateButton( giMPHMaxPlayersButtonImage[ 0 ], MPH_MAXPLAYERS_SETTING_X + 39, MPH_MAXPLAYERS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHMaxPlayersSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHMaxPlayersButton[ 1 ] = QuickCreateButton( giMPHMaxPlayersButtonImage[ 1 ], MPH_MAXPLAYERS_SETTING_X + 158, MPH_MAXPLAYERS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHMaxPlayersSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHMaxPlayersButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHMaxPlayersButton[1],0, 1 );

	iMPHMaxPlayers = guiMPHMaxPlayers;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// SQUAD SIZE

	giMPHSquadSizeButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHSquadSizeButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHSquadSizeButton[ 0 ] = QuickCreateButton( giMPHSquadSizeButtonImage[ 0 ], MPH_SQUADSIZE_SETTING_X + 39, MPH_SQUADSIZE_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHSquadSizeSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHSquadSizeButton[ 1 ] = QuickCreateButton( giMPHSquadSizeButtonImage[ 1 ], MPH_SQUADSIZE_SETTING_X + 158, MPH_SQUADSIZE_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHSquadSizeSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHSquadSizeButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHSquadSizeButton[1],0, 1 );

	iMPHSquadSize= guiMPHSquadSize;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// STARTING TIME

	giMPHStartingTimeButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHStartingTimeButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHStartingTimeButton[ 0 ] = QuickCreateButton( giMPHStartingTimeButtonImage[ 0 ], MPH_STARTINGTIME_SETTING_X + 39, MPH_STARTINGTIME_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHStartingTimeSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHStartingTimeButton[ 1 ] = QuickCreateButton( giMPHStartingTimeButtonImage[ 1 ], MPH_STARTINGTIME_SETTING_X + 158, MPH_STARTINGTIME_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHStartingTimeSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHStartingTimeButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHStartingTimeButton[1],0, 1 );

	iMPHStartingTime = guiMPHStartingTime;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// STARTING CASH

	giMPHStartingCashButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHStartingCashButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHStartingCashButton[ 0 ] = QuickCreateButton( giMPHStartingCashButtonImage[ 0 ], MPH_STARTINGCASH_SETTING_X + 39, MPH_STARTINGCASH_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHStartingCashSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHStartingCashButton[ 1 ] = QuickCreateButton( giMPHStartingCashButtonImage[ 1 ], MPH_STARTINGCASH_SETTING_X + 158, MPH_STARTINGCASH_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHStartingCashSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHStartingCashButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHStartingCashButton[1],0, 1 );

	iMPHStartingCash = guiMPHStartingCash;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// WEAPON DAMAGE

	giMPHWeaponDamageButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHWeaponDamageButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHWeaponDamageButton[ 0 ] = QuickCreateButton( giMPHWeaponDamageButtonImage[ 0 ], MPH_WEAPONDAMAGE_SETTING_X + 39, MPH_WEAPONDAMAGE_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHWeaponDamageSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHWeaponDamageButton[ 1 ] = QuickCreateButton( giMPHWeaponDamageButtonImage[ 1 ], MPH_WEAPONDAMAGE_SETTING_X + 158, MPH_WEAPONDAMAGE_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHWeaponDamageSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHWeaponDamageButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHWeaponDamageButton[1],0, 1 );

	iMPHWeaponDamage = guiMPHWeaponDamage;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// TIME TURNS

	giMPHTimeTurnsButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHTimeTurnsButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHTimeTurnsButton[ 0 ] = QuickCreateButton( giMPHTimeTurnsButtonImage[ 0 ], MPH_TIMETURNS_SETTING_X + 39, MPH_TIMETURNS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHTimeTurnsSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHTimeTurnsButton[ 1 ] = QuickCreateButton( giMPHTimeTurnsButtonImage[ 1 ], MPH_TIMETURNS_SETTING_X + 158, MPH_TIMETURNS_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHTimeTurnsSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHTimeTurnsButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHTimeTurnsButton[1],0, 1 );

	iMPHTimeTurns = guiMPHTimeTurns;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// DIFFICULTY SETTING

	giMPHDifficultyButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHDifficultyButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHDifficultyButton[ 0 ] = QuickCreateButton( giMPHDifficultyButtonImage[ 0 ], MPH_DIFFICULTY_SETTING_X + 39, MPH_DIFFICULTY_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHDifficultySelectionLeftCallback );

	// right button - increment difficulty level
	giMPHDifficultyButton[ 1 ] = QuickCreateButton( giMPHDifficultyButtonImage[ 1 ], MPH_DIFFICULTY_SETTING_X + 158, MPH_DIFFICULTY_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHDifficultySelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHDifficultyButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHDifficultyButton[1],0, 1 );

	iMPHDifficulty = guiMPHDifficultLevel - 1;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// GAME TYPE

	giMPHGameTypeButtonImage[ 0 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,0,-1,1,-1 );
	giMPHGameTypeButtonImage[ 1 ]=	LoadButtonImage( "INTERFACE\\GIO_SELECTION_ARROWS.STI" ,-1,2,-1,3,-1 );

	// left button - decrement difficulty level
	giMPHGameTypeButton[ 0 ] = QuickCreateButton( giMPHGameTypeButtonImage[ 0 ], MPH_GAMETYPE_SETTING_X + 39, MPH_GAMETYPE_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHGameTypeSelectionLeftCallback );

	// right button - increment difficulty level
	giMPHGameTypeButton[ 1 ] = QuickCreateButton( giMPHGameTypeButtonImage[ 1 ], MPH_GAMETYPE_SETTING_X + 158, MPH_GAMETYPE_SETTING_Y ,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHGameTypeSelectionRightCallback );
	
	// set user data
	MSYS_SetBtnUserData(giMPHGameTypeButton[0],0, 0 );
	MSYS_SetBtnUserData(giMPHGameTypeButton[1],0, 1 );

	iMPHGameType = guiMPHGameType;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// MAX ENEMIES

	guiMPHMaxEnemiesOptionTogglesImage[ MPH_MAXENEMIES_NO ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_NO ] = CreateIconAndTextButton( guiMPHMaxEnemiesOptionTogglesImage[ MPH_MAXENEMIES_NO ], gzMPHScreenText[ MPH_NO ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_MAXENEMIES_SETTING_X + 74), (MPH_MAXENEMIES_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHNoMaxEnemiesCallback);

	guiMPHMaxEnemiesOptionTogglesImage[ MPH_MAXENEMIES_YES ] = UseLoadedButtonImage( guiMPHMaxEnemiesOptionTogglesImage[ MPH_MAXENEMIES_NO ], -1,1,-1,3,-1 );
	guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_YES ] = CreateIconAndTextButton( guiMPHMaxEnemiesOptionTogglesImage[ MPH_MAXENEMIES_YES ],  gzMPHScreenText[ MPH_YES ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_MAXENEMIES_SETTING_X), (MPH_MAXENEMIES_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHYesMaxEnemiesCallback );

	SpecifyButtonSoundScheme( guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_NO ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_YES ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_NO ],0, 0 );
	MSYS_SetBtnUserData(guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_YES ],0, 1 );

	if( guiMPHMaxEnemies )
		ButtonList[ guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_YES ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiMPHMaxEnemiesOptionToggles[ MPH_MAXENEMIES_NO ] ]->uiFlags |= BUTTON_CLICKED_ON;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// HIRE MERC

	guiMPHHireMercOptionTogglesImage[ MPH_HIREMERC_RANDOM ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHHireMercOptionToggles[ MPH_HIREMERC_RANDOM ] = CreateIconAndTextButton( guiMPHHireMercOptionTogglesImage[ MPH_HIREMERC_RANDOM ], gzMPHScreenText[ MPH_HIRE_RANDOM ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_HIREMERC_SETTING_X + 74), (MPH_HIREMERC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHRandomHireMercCallback);

	guiMPHHireMercOptionTogglesImage[ MPH_HIREMERC_NORMAL ] = UseLoadedButtonImage( guiMPHHireMercOptionTogglesImage[ MPH_HIREMERC_RANDOM ], -1,1,-1,3,-1 );
	guiMPHHireMercOptionToggles[ MPH_HIREMERC_NORMAL ] = CreateIconAndTextButton( guiMPHHireMercOptionTogglesImage[ MPH_HIREMERC_NORMAL ],  gzMPHScreenText[ MPH_HIRE_NORMAL ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_HIREMERC_SETTING_X), (MPH_HIREMERC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHNormalHireMercCallback );

	SpecifyButtonSoundScheme( guiMPHHireMercOptionToggles[ MPH_HIREMERC_RANDOM ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHHireMercOptionToggles[ MPH_HIREMERC_NORMAL ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHHireMercOptionToggles[ MPH_HIREMERC_RANDOM ],0, 0 );
	MSYS_SetBtnUserData(guiMPHHireMercOptionToggles[ MPH_HIREMERC_NORMAL ],0, 1 );

	if( guiMPHHireMerc )
		ButtonList[ guiMPHHireMercOptionToggles[ MPH_HIREMERC_RANDOM ] ]->uiFlags |= BUTTON_CLICKED_ON;		
	else
		ButtonList[ guiMPHHireMercOptionToggles[ MPH_HIREMERC_NORMAL ] ]->uiFlags |= BUTTON_CLICKED_ON;
				
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// SAME MERC

	guiMPHSameMercOptionTogglesImage[ MPH_SAMEMERC_DISABLE ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHSameMercOptionToggles[ MPH_SAMEMERC_DISABLE ] = CreateIconAndTextButton( guiMPHSameMercOptionTogglesImage[ MPH_SAMEMERC_DISABLE ], gzMPHScreenText[ MPH_DISABLE ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_SAMEMERC_SETTING_X + 74), (MPH_SAMEMERC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHDisableSameMercCallback);

	guiMPHSameMercOptionTogglesImage[ MPH_SAMEMERC_ALLOW ] = UseLoadedButtonImage( guiMPHSameMercOptionTogglesImage[ MPH_SAMEMERC_DISABLE ], -1,1,-1,3,-1 );
	guiMPHSameMercOptionToggles[ MPH_SAMEMERC_ALLOW ] = CreateIconAndTextButton( guiMPHSameMercOptionTogglesImage[ MPH_SAMEMERC_ALLOW ],  gzMPHScreenText[ MPH_ALLOW ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_SAMEMERC_SETTING_X), (MPH_SAMEMERC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHAllowSameMercCallback );

	SpecifyButtonSoundScheme( guiMPHSameMercOptionToggles[ MPH_SAMEMERC_DISABLE ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHSameMercOptionToggles[ MPH_SAMEMERC_ALLOW ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHSameMercOptionToggles[ MPH_SAMEMERC_DISABLE ],0, 0 );
	MSYS_SetBtnUserData(guiMPHSameMercOptionToggles[ MPH_SAMEMERC_ALLOW ],0, 1 );

	if( guiMPHSameMerc )
		ButtonList[ guiMPHSameMercOptionToggles[ MPH_SAMEMERC_ALLOW ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiMPHSameMercOptionToggles[ MPH_SAMEMERC_DISABLE ] ]->uiFlags |= BUTTON_CLICKED_ON;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// REPORT MERC

	guiMPHReportMercOptionTogglesImage[ MPH_REPORTMERC_NO ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHReportMercOptionToggles[ MPH_REPORTMERC_NO ] = CreateIconAndTextButton( guiMPHReportMercOptionTogglesImage[ MPH_REPORTMERC_NO ], gzMPHScreenText[ MPH_NO ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_REPORTMERC_SETTING_X + 74), (MPH_REPORTMERC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHNoReportMercCallback);

	guiMPHReportMercOptionTogglesImage[ MPH_REPORTMERC_YES ] = UseLoadedButtonImage( guiMPHReportMercOptionTogglesImage[ MPH_REPORTMERC_NO ], -1,1,-1,3,-1 );
	guiMPHReportMercOptionToggles[ MPH_REPORTMERC_YES ] = CreateIconAndTextButton( guiMPHReportMercOptionTogglesImage[ MPH_REPORTMERC_YES ],  gzMPHScreenText[ MPH_YES ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_REPORTMERC_SETTING_X), (MPH_REPORTMERC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHYesReportMercCallback );

	SpecifyButtonSoundScheme( guiMPHReportMercOptionToggles[ MPH_REPORTMERC_NO ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHReportMercOptionToggles[ MPH_REPORTMERC_YES ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHReportMercOptionToggles[ MPH_REPORTMERC_NO ],0, 0 );
	MSYS_SetBtnUserData(guiMPHReportMercOptionToggles[ MPH_REPORTMERC_YES ],0, 1 );

	if( guiMPHReportMerc )
		ButtonList[ guiMPHReportMercOptionToggles[ MPH_REPORTMERC_YES ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiMPHReportMercOptionToggles[ MPH_REPORTMERC_NO ] ]->uiFlags |= BUTTON_CLICKED_ON;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// BOBBY RAY

	guiMPHBobbyRayOptionTogglesImage[ MPH_BOBBYRAY_DISABLE ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_DISABLE ] = CreateIconAndTextButton( guiMPHBobbyRayOptionTogglesImage[ MPH_BOBBYRAY_DISABLE ], gzMPHScreenText[ MPH_DISABLE ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_BOBBYRAY_SETTING_X + 74), (MPH_BOBBYRAY_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHDisableBobbyRayCallback);

	guiMPHBobbyRayOptionTogglesImage[ MPH_BOBBYRAY_ALLOW ] = UseLoadedButtonImage( guiMPHBobbyRayOptionTogglesImage[ MPH_BOBBYRAY_DISABLE ], -1,1,-1,3,-1 );
	guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_ALLOW ] = CreateIconAndTextButton( guiMPHBobbyRayOptionTogglesImage[ MPH_BOBBYRAY_ALLOW ],  gzMPHScreenText[ MPH_ALLOW ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_BOBBYRAY_SETTING_X), (MPH_BOBBYRAY_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHAllowBobbyRayCallback );

	SpecifyButtonSoundScheme( guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_DISABLE ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_ALLOW ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_DISABLE ],0, 0 );
	MSYS_SetBtnUserData(guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_ALLOW ],0, 1 );
	
	if( guiMPHBobbyRay )
		ButtonList[ guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_DISABLE ] ]->uiFlags |= BUTTON_CLICKED_ON;		
	else
		ButtonList[ guiMPHBobbyRayOptionToggles[ MPH_BOBBYRAY_ALLOW ] ]->uiFlags |= BUTTON_CLICKED_ON;
			
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// SECTOR EDGE

	guiMPHSectorEdgeOptionTogglesImage[ MPH_SECTOREDGE_RANDOM ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_RANDOM ] = CreateIconAndTextButton( guiMPHSectorEdgeOptionTogglesImage[ MPH_SECTOREDGE_RANDOM ], gzMPHScreenText[ MPH_EDGE_RANDOM ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_SECTOREDGE_SETTING_X + 74), (MPH_SECTOREDGE_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHRandomSectorEdgeCallback);

	guiMPHSectorEdgeOptionTogglesImage[ MPH_SECTOREDGE_SELECTABLE ] = UseLoadedButtonImage( guiMPHSectorEdgeOptionTogglesImage[ MPH_SECTOREDGE_RANDOM ], -1,1,-1,3,-1 );
	guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_SELECTABLE ] = CreateIconAndTextButton( guiMPHSectorEdgeOptionTogglesImage[ MPH_SECTOREDGE_SELECTABLE ],  gzMPHScreenText[ MPH_EDGE_SELECTABLE ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_SECTOREDGE_SETTING_X), (MPH_SECTOREDGE_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHSelectableSectorEdgeCallback );

	SpecifyButtonSoundScheme( guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_RANDOM ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_SELECTABLE ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_RANDOM ],0, 0 );
	MSYS_SetBtnUserData(guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_SELECTABLE ],0, 1 );

	if( guiMPHSectorEdge )
		ButtonList[ guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_RANDOM ] ]->uiFlags |= BUTTON_CLICKED_ON;		
	else
		ButtonList[ guiMPHSectorEdgeOptionToggles[ MPH_SECTOREDGE_SELECTABLE ] ]->uiFlags |= BUTTON_CLICKED_ON;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// CIVS

	guiMPHCivsOptionTogglesImage[ MPH_CIVS_NO ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHCivsOptionToggles[ MPH_CIVS_NO ] = CreateIconAndTextButton( guiMPHCivsOptionTogglesImage[ MPH_CIVS_NO ], gzMPHScreenText[ MPH_NO ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_CIVS_SETTING_X + 74), (MPH_CIVS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHNoCivsCallback);

	guiMPHCivsOptionTogglesImage[ MPH_CIVS_YES ] = UseLoadedButtonImage( guiMPHCivsOptionTogglesImage[ MPH_CIVS_NO ], -1,1,-1,3,-1 );
	guiMPHCivsOptionToggles[ MPH_CIVS_YES ] = CreateIconAndTextButton( guiMPHCivsOptionTogglesImage[ MPH_CIVS_YES ],  gzMPHScreenText[ MPH_YES ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_CIVS_SETTING_X), (MPH_CIVS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHYesCivsCallback );

	SpecifyButtonSoundScheme( guiMPHCivsOptionToggles[ MPH_CIVS_NO ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHCivsOptionToggles[ MPH_CIVS_YES ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHCivsOptionToggles[ MPH_CIVS_NO ],0, 0 );
	MSYS_SetBtnUserData(guiMPHCivsOptionToggles[ MPH_CIVS_YES ],0, 1 );

	if( guiMPHCivs )
		ButtonList[ guiMPHCivsOptionToggles[ MPH_CIVS_YES ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiMPHCivsOptionToggles[ MPH_CIVS_NO ] ]->uiFlags |= BUTTON_CLICKED_ON;
	// -------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// INVENTORY / ATTACHMENT

	if (IsNIVModeValid(true))
	{		
		giMPHInventoryButtonImage[ 0 ]=	UseLoadedButtonImage( giMPHDifficultyButtonImage[ 0 ],-1,0,-1,1,-1 );
		giMPHInventoryButtonImage[ 1 ]=	UseLoadedButtonImage( giMPHDifficultyButtonImage[ 1 ],-1,2,-1,3,-1 );

		// left button - decrement difficulty level
		giMPHInventoryButton[ 0 ] = QuickCreateButton( giMPHInventoryButtonImage[ 0 ], MPH_INVENTORY_SETTING_X + 39, MPH_INVENTORY_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHInventorySelectionLeftCallback );

		// right button - increment difficulty level
		giMPHInventoryButton[ 1 ] = QuickCreateButton( giMPHInventoryButtonImage[ 1 ], MPH_INVENTORY_SETTING_X + 158, MPH_INVENTORY_SETTING_Y ,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
											BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMPHInventorySelectionRightCallback );


		// set user data
		MSYS_SetBtnUserData(giMPHInventoryButton[0],0, 0 );
		MSYS_SetBtnUserData(giMPHInventoryButton[1],0, 1 );

		// set initial value
		switch ( guiMPHInventory )
		{
			case INVENTORY_OLD:
				iMPHInventory = MPH_INV_OLD;
				break;
			case INVENTORY_NEW:
				iMPHInventory = MPH_INV_NEW;
				break;
			case 2:	// New/New
				iMPHInventory = 2;
				break;
		}
	}	

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// OLD/NEW TARITS SETTING

	guiMPHTraitsOptionTogglesImage[ MPH_TRAITS_OLD ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHTraitsOptionToggles[ MPH_TRAITS_OLD ] = CreateIconAndTextButton( guiMPHTraitsOptionTogglesImage[ MPH_TRAITS_OLD ], gzGIOScreenText[ GIO_TRAITS_OLD_TEXT ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_TRAITS_SETTING_X + 74), (MPH_TRAITS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHOldTraitsCallback);

	guiMPHTraitsOptionTogglesImage[ MPH_TRAITS_NEW ] = UseLoadedButtonImage( guiMPHTraitsOptionTogglesImage[ MPH_TRAITS_OLD ], -1,1,-1,3,-1 );
	guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ] = CreateIconAndTextButton( guiMPHTraitsOptionTogglesImage[ MPH_TRAITS_NEW ],  gzGIOScreenText[ GIO_TRAITS_NEW_TEXT ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_TRAITS_SETTING_X), (MPH_TRAITS_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHNewTraitsCallback );

	SpecifyButtonSoundScheme( guiMPHTraitsOptionToggles[ MPH_TRAITS_OLD ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHTraitsOptionToggles[ MPH_TRAITS_OLD ],0, 0 );
	MSYS_SetBtnUserData(guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ],0, 1 );

	if( guiMPHNewTraits )
		ButtonList[ guiMPHTraitsOptionToggles[ MPH_TRAITS_NEW ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiMPHTraitsOptionToggles[ MPH_TRAITS_OLD ] ]->uiFlags |= BUTTON_CLICKED_ON;
		
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// SYNC GAME DIR

	guiMPHSyncOptionTogglesImage[ MPH_SYNC_NO ] = LoadButtonImage( "INTERFACE\\GIOCheckButton.sti" ,-1,0,-1,2,-1 );
	guiMPHSyncOptionToggles[ MPH_SYNC_NO ] = CreateIconAndTextButton( guiMPHSyncOptionTogglesImage[ MPH_SYNC_NO ], gzMPHScreenText[ MPH_NO ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_SYNC_SETTING_X + 74), (MPH_SYNC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHNoSyncCallback);

	guiMPHSyncOptionTogglesImage[ MPH_SYNC_YES ] = UseLoadedButtonImage( guiMPHSyncOptionTogglesImage[ MPH_SYNC_NO ], -1,1,-1,3,-1 );
	guiMPHSyncOptionToggles[ MPH_SYNC_YES ] = CreateIconAndTextButton( guiMPHSyncOptionTogglesImage[ MPH_SYNC_YES ],  gzMPHScreenText[ MPH_YES ], MPH_TOGGLE_TEXT_FONT,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													MPH_TOGGLE_TEXT_COLOR, NO_SHADOW,
													TEXT_CJUSTIFIED,
													(MPH_SYNC_SETTING_X), (MPH_SYNC_SETTING_Y + 10), BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													DEFAULT_MOVE_CALLBACK, BtnMPHYesSyncCallback );

	SpecifyButtonSoundScheme( guiMPHSyncOptionToggles[ MPH_SYNC_NO ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	SpecifyButtonSoundScheme( guiMPHSyncOptionToggles[ MPH_SYNC_YES ], BUTTON_SOUND_SCHEME_BIGSWITCH3 );
	MSYS_SetBtnUserData(guiMPHSyncOptionToggles[ MPH_SYNC_NO ],0, 0 );
	MSYS_SetBtnUserData(guiMPHSyncOptionToggles[ MPH_SYNC_YES ],0, 1 );

	if( guiMPHSendFiles )
		ButtonList[ guiMPHSyncOptionToggles[ MPH_SYNC_YES ] ]->uiFlags |= BUTTON_CLICKED_ON;
	else
		ButtonList[ guiMPHSyncOptionToggles[ MPH_SYNC_NO ] ]->uiFlags |= BUTTON_CLICKED_ON;
	

	//Reset the exit screen - screen the main game loop will call next iteration
	gubMPHExitScreen = MP_HOST_SCREEN;

	//REnder the screen once so we can blt ot to ths save buffer
	RenderMPHScreen();

	BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	gfMPHButtonsAllocated = TRUE;

	return( TRUE );

}

BOOLEAN		ExitMPHScreen()
{
	UINT16	cnt;

	if( !gfMPHButtonsAllocated )
		return( TRUE );

	//Delete the main options screen background
	DeleteVideoObjectFromIndex( guiMPHMainBackGroundImage );

	DeleteVideoObjectFromIndex( guiMPHSMALLFRAME );

	RemoveButton( guiMPHStartButton );
	RemoveButton( guiMPHCancelButton );

	UnloadButtonImage( giMPHCancelBtnImage );
	UnloadButtonImage( giMPHStartBtnImage );

	// --------------
	// MaxPlayers
	RemoveButton( giMPHMaxPlayersButton[0] );
	RemoveButton( giMPHMaxPlayersButton[1] );
	UnloadButtonImage( giMPHMaxPlayersButtonImage[0] );
	UnloadButtonImage( giMPHMaxPlayersButtonImage[1] );
	// ----------

	// ----------
	// SquadSize
	RemoveButton( giMPHSquadSizeButton[0] );
	RemoveButton( giMPHSquadSizeButton[1] );
	UnloadButtonImage( giMPHSquadSizeButtonImage[0] );
	UnloadButtonImage( giMPHSquadSizeButtonImage[1] );
	// ---------------

	// --------------
	// StartingTime
	RemoveButton( giMPHStartingTimeButton[0] );
	RemoveButton( giMPHStartingTimeButton[1] );
	UnloadButtonImage( giMPHStartingTimeButtonImage[0] );
	UnloadButtonImage( giMPHStartingTimeButtonImage[1] );
	// ----------

	// --------------
	// StartingCash
	RemoveButton( giMPHStartingCashButton[0] );
	RemoveButton( giMPHStartingCashButton[1] );
	UnloadButtonImage( giMPHStartingCashButtonImage[0] );
	UnloadButtonImage( giMPHStartingCashButtonImage[1] );
	// ----------

	// --------------
	// WeaponDamage
	RemoveButton( giMPHWeaponDamageButton[0] );
	RemoveButton( giMPHWeaponDamageButton[1] );
	UnloadButtonImage( giMPHWeaponDamageButtonImage[0] );
	UnloadButtonImage( giMPHWeaponDamageButtonImage[1] );
	// ----------

	// --------------
	// TimeTurns
	RemoveButton( giMPHTimeTurnsButton[0] );
	RemoveButton( giMPHTimeTurnsButton[1] );
	UnloadButtonImage( giMPHTimeTurnsButtonImage[0] );
	UnloadButtonImage( giMPHTimeTurnsButtonImage[1] );
	// ----------

	// --------------
	// Difficulty
	RemoveButton( giMPHDifficultyButton[0] );
	RemoveButton( giMPHDifficultyButton[1] );
	UnloadButtonImage( giMPHDifficultyButtonImage[0] );
	UnloadButtonImage( giMPHDifficultyButtonImage[1] );
	// --------------

	// --------------
	// GameType
	RemoveButton( giMPHGameTypeButton[0] );
	RemoveButton( giMPHGameTypeButton[1] );
	UnloadButtonImage( giMPHGameTypeButtonImage[0] );
	UnloadButtonImage( giMPHGameTypeButtonImage[1] );
	// --------------

	// ---------------
	// MaxEnemies
	for (cnt = 0; cnt < MPH_NUM_MAXENEMIES_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHMaxEnemiesOptionToggles[cnt] );
		UnloadButtonImage(guiMPHMaxEnemiesOptionTogglesImage[cnt] );
	}
	// ------------------
	
	// -----------
	// HireMerc
	for (cnt = 0; cnt < MPH_NUM_HIREMERC_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHHireMercOptionToggles[cnt] );
		UnloadButtonImage(guiMPHHireMercOptionTogglesImage[cnt] );
	}
	// ---------

	// -----------
	// SameMerc
	for (cnt = 0; cnt < MPH_NUM_SAMEMERC_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHSameMercOptionToggles[cnt] );
		UnloadButtonImage(guiMPHSameMercOptionTogglesImage[cnt] );
	}
	// ---------

	// -----------
	// ReportMerc
	for (cnt = 0; cnt < MPH_NUM_REPORTMERC_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHReportMercOptionToggles[cnt] );
		UnloadButtonImage(guiMPHReportMercOptionTogglesImage[cnt] );
	}
	// ------------------
	
	// -------------------
	// BobbyRay
	for (cnt = 0; cnt < MPH_NUM_BOBBYRAY_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHBobbyRayOptionToggles[cnt] );
		UnloadButtonImage(guiMPHBobbyRayOptionTogglesImage[cnt] );
	}
	// ---------
	
	// ------------
	// SectorEdge
	for (cnt = 0; cnt < MPH_NUM_SECTOREDGE_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHSectorEdgeOptionToggles[cnt] );
		UnloadButtonImage(guiMPHSectorEdgeOptionTogglesImage[cnt] );
	}
	// ---------
	
	// ----------
	// Civs
	for (cnt = 0; cnt < MPH_NUM_CIVS_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHCivsOptionToggles[cnt] );
		UnloadButtonImage(guiMPHCivsOptionTogglesImage[cnt] );
	}
	// ------------------
	
	// -----------------
	// Inventory / Attachments
	if (IsNIVModeValid(true))
	{
		RemoveButton( giMPHInventoryButton[0] );
		RemoveButton( giMPHInventoryButton[1] );
		UnloadButtonImage( giMPHInventoryButtonImage[0] );
		UnloadButtonImage( giMPHInventoryButtonImage[1] );
	}
	// ------------
	
	// ------------------
	// Sync
	for (cnt = 0; cnt < MPH_NUM_SYNC_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHSyncOptionToggles[cnt] );
		UnloadButtonImage(guiMPHSyncOptionTogglesImage[cnt] );
	}
	// ------------------

	// ------------------
	// Traits
	for (cnt = 0; cnt < MPH_NUM_TRAITS_OPTIONS; cnt++)
	{
		RemoveButton(guiMPHTraitsOptionToggles[cnt] );
		UnloadButtonImage(guiMPHTraitsOptionTogglesImage[cnt] );
	}
	// ------------------

	// exit text input mode in this screen and clean up text boxes
	KillAllTextInputModes();
	SetTextInputCursor( CURSOR_IBEAM );

	gfMPHButtonsAllocated = FALSE;

	gfMPHScreenExit = FALSE;
	gfMPHScreenEntry = TRUE;

	return( TRUE );

}


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
}

BOOLEAN		RenderMPHScreen()
{
	HVOBJECT	hPixHandle;

	// --------------------------
	// Difficulty	
	RestoreExternBackgroundRect( MPH_DIFFICULTY_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_DIFFICULTY_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// MaxPlayers	
	RestoreExternBackgroundRect( MPH_MAXPLAYERS_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_MAXPLAYERS_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// StartingTime	
	RestoreExternBackgroundRect( MPH_STARTINGTIME_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_STARTINGTIME_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// StartingCash
	RestoreExternBackgroundRect( MPH_STARTINGCASH_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_STARTINGCASH_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// WeaponDamage
	RestoreExternBackgroundRect( MPH_WEAPONDAMAGE_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_WEAPONDAMAGE_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// TimeTurns
	RestoreExternBackgroundRect( MPH_TIMETURNS_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_TIMETURNS_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// SquadSize
	RestoreExternBackgroundRect( MPH_SQUADSIZE_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_SQUADSIZE_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// GameType	
	RestoreExternBackgroundRect( MPH_GAMETYPE_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_GAMETYPE_SETTING_Y-3, 120, 20 );
	// --------------------------

	// --------------------------
	// Inventory	
	RestoreExternBackgroundRect( MPH_INVENTORY_SETTING_X+MPH_OFFSET_TO_TEXT + 20, MPH_INVENTORY_SETTING_Y-3, 120, 20 );
	// --------------------------

	//Get the main background screen graphic and blt it
	GetVideoObject(&hPixHandle, guiMPHMainBackGroundImage );

		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,(SCREEN_WIDTH - xResSize)/2,(SCREEN_HEIGHT - yResSize)/2, VO_BLT_SRCTRANSPARENCY,NULL);

	//Shade the background
	ShadowVideoSurfaceRect( FRAME_BUFFER, iScreenWidthOffset, iScreenHeightOffset, iScreenWidthOffset + 640, iScreenHeightOffset + 480 );

	//Display the title
	DrawTextToScreen( gzMPHScreenText[ MPH_TITLE_TEXT ], MPH_MAIN_TITLE_X, MPH_MAIN_TITLE_Y, MPH_MAIN_TITLE_WIDTH, MPH_TITLE_FONT, MPH_TITLE_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	
	// ---------	
	// MaxPlayers	
	RenderMPHSmallSelectionFrame( (MPH_MAXPLAYERS_SETTING_X + 36), (MPH_MAXPLAYERS_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_MAXPLAYERS_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_MAXPLAYERS_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_MAXPLAYERS_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_NUMPLAYERS_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_MAXPLAYERS_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_MAXPLAYERS_SETTING_Y+6), MPH_MAXPLAYERS_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ (MPH_1 - MPH_MAXPLAYERS_2 + 1) + iMPHMaxPlayers ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -----------------

	// -----------------
	// SquadSize		
	RenderMPHSmallSelectionFrame( (MPH_SQUADSIZE_SETTING_X + 36), (MPH_SQUADSIZE_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_SQUADSIZE_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_SQUADSIZE_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_SQUADSIZE_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_SQUADSIZE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_SQUADSIZE_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_SQUADSIZE_SETTING_Y+6), MPH_SQUADSIZE_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ (MPH_1 - MPH_SQUADSIZE_1) + iMPHSquadSize ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -----------------

	// ----------
	// StartingTime	
	RenderMPHSmallSelectionFrame( (MPH_STARTINGTIME_SETTING_X + 36), (MPH_STARTINGTIME_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_STARTINGTIME_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_STARTINGTIME_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_STARTINGTIME_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_TIME_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_STARTINGTIME_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_STARTINGTIME_SETTING_Y+6), MPH_STARTINGTIME_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_MORNING + iMPHStartingTime ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -----------------

	// ------------------
	// StartingCash	
	RenderMPHSmallSelectionFrame( (MPH_STARTINGCASH_SETTING_X + 36), (MPH_STARTINGCASH_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_STARTINGCASH_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_STARTINGCASH_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_STARTINGCASH_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_BALANCE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_STARTINGCASH_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_STARTINGCASH_SETTING_Y+6), MPH_STARTINGCASH_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_CASH_LOW + iMPHStartingCash ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -----------------

	// ----------
	// WeaponDamage	
	RenderMPHSmallSelectionFrame( (MPH_WEAPONDAMAGE_SETTING_X + 36), (MPH_WEAPONDAMAGE_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_WEAPONDAMAGE_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_WEAPONDAMAGE_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_WEAPONDAMAGE_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_DMG_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_WEAPONDAMAGE_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_WEAPONDAMAGE_SETTING_Y+6), MPH_STARTINGCASH_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_DAMAGE_VERYLOW + iMPHWeaponDamage ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );	
	// --------------

	// -----------
	// TurnTimes	
	RenderMPHSmallSelectionFrame( (MPH_TIMETURNS_SETTING_X + 36), (MPH_TIMETURNS_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_TIMETURNS_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_TIMETURNS_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_TIMETURNS_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_TIMER_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_TIMETURNS_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_TIMETURNS_SETTING_Y+6), MPH_TIMETURNS_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_TIME_NEVER + iMPHTimeTurns ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -----------------
	
	// ---------------
	// Difficulty		
	RenderMPHSmallSelectionFrame( (MPH_DIFFICULTY_SETTING_X + 36), (MPH_DIFFICULTY_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_DIFFICULTY_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_DIFFICULTY_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_DIFFICULTY_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_DIF_LEVEL_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	
	DisplayWrappedString( (UINT16)(MPH_DIFFICULTY_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_DIFFICULTY_SETTING_Y+6), MPH_DIFFICULTY_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, zDiffSetting[ iMPHDifficulty + 1].szDiffName, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ---------------

	// ---------------
	// GameType		
	RenderMPHSmallSelectionFrame( (MPH_GAMETYPE_SETTING_X + 36), (MPH_GAMETYPE_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_GAMETYPE_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_GAMETYPE_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_GAMETYPE_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_GAMETYPE_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_GAMETYPE_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_GAMETYPE_SETTING_Y+6), MPH_GAMETYPE_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_DEATHMATCH_TEXT + iMPHGameType], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ---------------
	
	// ---------------
	// MaxEnemies	
	DisplayWrappedString( (MPH_MAXENEMIES_SETTING_X - 6), (UINT16)(MPH_MAXENEMIES_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_MAXENEMIES_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_OVERRIDEMAXAI_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -------------

	// ---------------
	// Traits
	DisplayWrappedString( (MPH_TRAITS_SETTING_X - 6), (UINT16)(MPH_TRAITS_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_TRAITS_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_TRAITS_TEXT], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -------------

	// -----------
	// HireMerc	
	DisplayWrappedString( (MPH_HIREMERC_SETTING_X - 6), (UINT16)(MPH_HIREMERC_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_HIREMERC_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_RANDOMMERCS_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ----------

	// ------------
	// SameMerc	
	DisplayWrappedString( (MPH_SAMEMERC_SETTING_X - 6), (UINT16)(MPH_SAMEMERC_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_SAMEMERC_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_SAMEMERC_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ----------

	// -------------
	// ReportMerc	
	DisplayWrappedString( (MPH_REPORTMERC_SETTING_X - 6), (UINT16)(MPH_REPORTMERC_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_REPORTMERC_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_RPTMERC_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -------------

	// -----------------
	// BobbyRay	
	DisplayWrappedString( (MPH_BOBBYRAY_SETTING_X - 6), (UINT16)(MPH_BOBBYRAY_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_BOBBYRAY_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_BOBBYRAY_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ----------

	// ------------------
	// SectorEdge	
	DisplayWrappedString( (MPH_SECTOREDGE_SETTING_X - 6), (UINT16)(MPH_SECTOREDGE_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_SECTOREDGE_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_RNDMSTART_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ----------

	// ------------
	// Civs	
	DisplayWrappedString( (MPH_CIVS_SETTING_X - 6), (UINT16)(MPH_CIVS_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_CIVS_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_ENABLECIV_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// -------------

	// --------------
	// Inventory	
	RenderMPHSmallSelectionFrame( (MPH_INVENTORY_SETTING_X + 36), (MPH_INVENTORY_SETTING_Y - 3) );
	DisplayWrappedString( (UINT16)(MPH_INVENTORY_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (UINT16)(MPH_INVENTORY_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE - 12), MPH_INVENTORY_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ GIO_INV_TEXT ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	DisplayWrappedString( (UINT16)(MPH_INVENTORY_SETTING_X+MPH_OFFSET_TO_TEXT + 1), (MPH_INVENTORY_SETTING_Y+6), MPH_INVENTORY_SETTING_WIDTH, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzGIOScreenText[ iMPHInventory + 54 ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// ----------------

	// --------------------
	// Sync	
	DisplayWrappedString( (MPH_SYNC_SETTING_X - 6), (UINT16)(MPH_SYNC_SETTING_Y-MPH_GAP_BN_SETTINGS + MPH_TITLE_DISTANCE), MPH_SYNC_SETTING_WIDTH + 14, 2, MPH_TOGGLE_TEXT_FONT, MPH_TOGGLE_TEXT_COLOR, gzMPHScreenText[ MPH_SYNC_GAME_DIRECTORY ], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );
	// --------------------

	return( TRUE );
}



void			GetMPHScreenUserInput()
{
	InputAtom Event;
//	POINT	MousePos;

//	GetCursorPos(&MousePos);

	while (DequeueSpecificEvent(&Event, KEY_DOWN|KEY_UP|KEY_REPEAT))
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
						SaveMPSettings(); // Update Profiles/UserProfile/ja2_mp.ini
						SGP_TRYCATCH_RETHROW( ja2::mp::InitializeMultiplayerProfile(vfs::Path(gzFileTransferDirectory)), L"" );
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
			gubMPHScreenHandler = MPH_START;
			SaveMPSettings(); // Update the Profiles/UserProfile/ja2_mp.ini
			SGP_TRYCATCH_RETHROW( ja2::mp::InitializeMultiplayerProfile(vfs::Path(gzFileTransferDirectory)), L"" );

			// The difficult level has to be set there. This is the only value so far, because it is used for initialization!
			//gGameOptions.ubDifficultyLevel =   GetMPHCurrentDifficultyButtonSetting();
			gGameOptions.ubDifficultyLevel = iMPHDifficulty + 1;

			// WANNE - MP: Reinit TableData folder and INIs
			LoadExternalGameplayData(TABLEDATA_DIRECTORY, true);
			InitDependingGameStyleOptions();
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

void DoneFadeOutForExitMPHScreen( void )
{
	// As we bypassed the GIO screen, set up some game options for multiplayer here
	// <TODO> most things i have left as thier defaults here for testing.
	is_networked = true;
	is_host = true; // we want to be the host, not we ARE the host yet (is_server)
	auto_retry = true;
	giNumTries = 5;
	
	if (is_networked)
		gGameOptions.fTurnTimeLimit = TRUE;
	else
		gGameOptions.fTurnTimeLimit = FALSE;
	
	// Bobby Rays - why would we want anything less than the best
	gGameOptions.ubBobbyRayQuality = BR_AWESOME;
	gGameOptions.ubBobbyRayQuantity = BR_AWESOME;
			
	gubMPHExitScreen = INTRO_SCREEN;

#ifdef JA2UB
	//SetIntroType( INTRO_BEGINNING );
#else
	SetIntroType( INTRO_BEGINNING );
#endif

	ExitMPHScreen(); // cleanup please, if we called a fadeout then we didnt do it above

	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void DoneFadeInForExitMPHScreen( void )
{
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );
}

void RenderMPHSmallSelectionFrame(INT16 sX, INT16 sY)
{

	HVOBJECT hHandle;

	// get the video object
	GetVideoObject(&hHandle, guiMPHSMALLFRAME);

	// blt to sX, sY relative to upper left corner
	BltVideoObject(FRAME_BUFFER, hHandle, 0, sX, sY , VO_BLT_SRCTRANSPARENCY,NULL);

	return;
}
