	#include "builddefines.h"

#ifdef JA2EDITOR

	//sgp
	#include "Button System.h"
	#include "Font Control.h"
	#include "debug.h"
	//editor
	#include "EditorDefines.h"
	#include "Editor Callback Prototypes.h"
	#include "Editor Taskbar Utils.h"
	#include "EditorMercs.h"
	#include "EditorMapInfo.h"
	//tactical
	#include "Soldier Create.h"
	#include "overhead types.h"
	#include "local.h"
	#include "Text.h"

//Category tabs of the editor buttons
void InitEditorTerrainToolbar();
void InitEditorBuildingsToolbar();
void InitEditorItemsToolbar();
void InitEditorMercsToolbar();
void InitEditorMapInfoToolbar();
void InitEditorOptionsToolbar();
void InitEditorItemStatsButtons();

void InitEditorItemStatsButtons()
{
	iEditorButton[ ITEMSTATS_PANEL ] =
		CreateTextButton( 0, 0, 0, 0, BUTTON_USE_DEFAULT, iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 361, 160, 99, BUTTON_TOGGLE,
		MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	SpecifyDisabledButtonStyle( iEditorButton[ ITEMSTATS_PANEL ], DISABLED_STYLE_NONE );
	DisableButton( iEditorButton[ ITEMSTATS_PANEL ] );
	iEditorButton[ ITEMSTATS_HIDDEN_BTN ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 365, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_NORMAL, ItemStatsToggleHideCallback );
	iEditorButton[ ITEMSTATS_DELETE_BTN ] =
		CreateTextButton( iEditorItemStatsButtonsText[0], FONT10ARIAL, FONT_RED, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 600, 2 * iScreenHeightOffset + 441, 36, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL+1, DEFAULT_MOVE_CALLBACK, ItemStatsDeleteCallback );
	SetButtonFastHelpText( iEditorButton[ ITEMSTATS_DELETE_BTN ], iEditorItemStatsButtonsText[1]);
}

void InitEditorMercsToolbar()
{
	CHAR16 TempString[40];
//	STR16 FaceDirs[8] = {L"north",L"northeast",L"east",L"southeast",L"south",L"southwest",L"west",L"northwest"};
	INT32 x;

	iEditorButton[ MERCS_PLAYERTOGGLE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 4, 2 * iScreenHeightOffset + 362, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsTogglePlayers );
	if( gfShowPlayers )
		ClickEditorButton( MERCS_PLAYERTOGGLE );
	SetButtonFastHelpText( iEditorButton[ MERCS_PLAYERTOGGLE ], iEditorMercsToolbarText[0]);
	DisableButton( iEditorButton[ MERCS_PLAYERTOGGLE ] );
	iEditorButton[ MERCS_ENEMYTOGGLE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 4, 2 * iScreenHeightOffset + 382, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleEnemies );
	if( gfShowEnemies )
		ClickEditorButton( MERCS_ENEMYTOGGLE );
	SetButtonFastHelpText( iEditorButton[ MERCS_ENEMYTOGGLE ], iEditorMercsToolbarText[1]);
	iEditorButton[ MERCS_CREATURETOGGLE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 4, 2 * iScreenHeightOffset + 402, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleCreatures );
	if( gfShowCreatures )
		ClickEditorButton( MERCS_CREATURETOGGLE );
	SetButtonFastHelpText( iEditorButton[ MERCS_CREATURETOGGLE ], iEditorMercsToolbarText[2]);
	iEditorButton[ MERCS_REBELTOGGLE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 4, 2 * iScreenHeightOffset + 422, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleRebels );
	if( gfShowRebels )
		ClickEditorButton( MERCS_REBELTOGGLE );
	SetButtonFastHelpText( iEditorButton[ MERCS_REBELTOGGLE ], iEditorMercsToolbarText[3]);
	iEditorButton[ MERCS_CIVILIANTOGGLE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 4, 2 * iScreenHeightOffset + 442, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleCivilians );
	if( gfShowCivilians )
		ClickEditorButton( MERCS_CIVILIANTOGGLE );
	SetButtonFastHelpText( iEditorButton[ MERCS_CIVILIANTOGGLE ], iEditorMercsToolbarText[4]);
	iEditorButton[MERCS_PLAYER] =
		CreateTextButton( iEditorMercsToolbarText[5],(UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 20, 2 * iScreenHeightOffset + 362, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsPlayerTeamCallback );
	SpecifyButtonDownTextColors( iEditorButton[MERCS_PLAYER], FONT_YELLOW, FONT_BLACK );
	DisableButton( iEditorButton[MERCS_PLAYER] );
	iEditorButton[MERCS_ENEMY] =
		CreateTextButton( iEditorMercsToolbarText[6],(UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 20, 2 * iScreenHeightOffset + 382, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsEnemyTeamCallback );
	SpecifyButtonDownTextColors( iEditorButton[MERCS_ENEMY], FONT_YELLOW, FONT_BLACK );
	iEditorButton[MERCS_CREATURE] =
		CreateTextButton( iEditorMercsToolbarText[7],(UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 20, 2 * iScreenHeightOffset + 402, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsCreatureTeamCallback );
	SpecifyButtonDownTextColors( iEditorButton[MERCS_CREATURE], FONT_YELLOW, FONT_BLACK );
	iEditorButton[MERCS_REBEL] =
		CreateTextButton( iEditorMercsToolbarText[8],(UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 20, 2 * iScreenHeightOffset + 422, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsRebelTeamCallback );
	SpecifyButtonDownTextColors( iEditorButton[MERCS_REBEL], FONT_YELLOW, FONT_BLACK );
	iEditorButton[MERCS_CIVILIAN] =
		CreateTextButton( iEditorMercsToolbarText[9],(UINT16)BLOCKFONT, 165, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 20, 2 * iScreenHeightOffset + 442, 78, 19, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsCivilianTeamCallback );
	SpecifyButtonDownTextColors( iEditorButton[MERCS_CIVILIAN], FONT_YELLOW, FONT_BLACK );

	iEditorButton[ MERCS_1 ] =
		CreateTextButton( iEditorMercsToolbarText[10], SMALLCOMPFONT, FONT_ORANGE, 60, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 362, 68, 20, BUTTON_TOGGLE,
		MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	DisableButton( iEditorButton[ MERCS_1 ] );
	SpecifyDisabledButtonStyle( iEditorButton[ MERCS_1 ], DISABLED_STYLE_NONE );
	SpecifyButtonTextOffsets( iEditorButton[ MERCS_1 ], 20, 4, FALSE );
	SpecifyButtonTextWrappedWidth( iEditorButton[ MERCS_1 ], 46 );
	iEditorButton[ MERCS_DETAILEDCHECKBOX ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 103, 2 * iScreenHeightOffset + 365, "EDITOR//checkbox.sti", MSYS_PRIORITY_NORMAL, MercsDetailedPlacementCallback );

	iEditorButton[ MERCS_GENERAL ] =
		CreateEasyToggleButton( iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 382, "EDITOR//MercGeneral.sti", MercsGeneralModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_GENERAL ], iEditorMercsToolbarText[11]);
	iEditorButton[ MERCS_APPEARANCE ] =
		CreateEasyToggleButton( iScreenWidthOffset + 134, 2 * iScreenHeightOffset + 382, "EDITOR//MercAppearance.sti", MercsAppearanceModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_APPEARANCE ], iEditorMercsToolbarText[12]);
	iEditorButton[ MERCS_ATTRIBUTES ] =
		CreateEasyToggleButton( iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 408, "EDITOR//MercAttributes.sti", MercsAttributesModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_ATTRIBUTES ], iEditorMercsToolbarText[13]);
	iEditorButton[ MERCS_INVENTORY ] =
		CreateEasyToggleButton( iScreenWidthOffset + 134, 2 * iScreenHeightOffset + 408, "EDITOR//MercInventory.sti", MercsInventoryModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_INVENTORY ], iEditorMercsToolbarText[14]);
	iEditorButton[ MERCS_PROFILE ] =
		CreateEasyToggleButton( iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 434, "EDITOR//MercProfile.sti", MercsProfileModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_PROFILE ], iEditorMercsToolbarText[15]);
	iEditorButton[ MERCS_SCHEDULE ] =
		CreateEasyToggleButton( iScreenWidthOffset + 134, 2 * iScreenHeightOffset + 434, "EDITOR//MercSchedule.sti", MercsScheduleModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_SCHEDULE ], iEditorMercsToolbarText[16]);
	//Workaround for identical buttons.
	MSYS_SetBtnUserData( iEditorButton[ MERCS_SCHEDULE ], 3, 0xffffffff );
	iEditorButton[ MERCS_GLOWSCHEDULE ] =
		CreateEasyToggleButton( iScreenWidthOffset + 134, 2 * iScreenHeightOffset + 434, "EDITOR//MercGlowSchedule.sti", MercsScheduleModeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_GLOWSCHEDULE ], iEditorMercsToolbarText[17]);
	HideEditorButton( MERCS_GLOWSCHEDULE );

	iEditorButton[ MERCS_DELETE ] =
		CreateTextButton( iEditorMercsToolbarText[18], (UINT16)SMALLCOMPFONT, FONT_DKBLUE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 600, 2 * iScreenHeightOffset + 362, 40, 20, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsDeleteCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_DELETE ], iEditorMercsToolbarText[19]);
	iEditorButton[ MERCS_NEXT ] =
		CreateTextButton( iEditorMercsToolbarText[20], (UINT16)SMALLCOMPFONT, FONT_DKBLUE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 600, 2 * iScreenHeightOffset + 382, 40, 20, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsNextCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_NEXT ], iEditorMercsToolbarText[21]);

	//Priority Existance
	iEditorButton[ MERCS_PRIORITYEXISTANCE_CHECKBOX ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 170, 2 * iScreenHeightOffset + 365, "EDITOR//checkbox.sti", MSYS_PRIORITY_NORMAL, MercsPriorityExistanceCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_PRIORITYEXISTANCE_CHECKBOX ], iEditorMercsToolbarText[22] );

	//If merc has keys
	iEditorButton[ MERCS_HASKEYS_CHECKBOX ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 170, 2 * iScreenHeightOffset + 390, "EDITOR//checkbox.sti", MSYS_PRIORITY_NORMAL, MercsHasKeysCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_HASKEYS_CHECKBOX ], iEditorMercsToolbarText[23] );

	//Orders
	iEditorButton[ MERCS_ORDERS_STATIONARY ] =
		CreateTextButton( iEditorMercsToolbarText[24], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 368, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_ONGUARD] =
		CreateTextButton( iEditorMercsToolbarText[25], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 380, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_ONCALL ] =
		CreateTextButton( iEditorMercsToolbarText[26], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 392, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_SEEKENEMY ] =
		CreateTextButton( iEditorMercsToolbarText[27], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 404, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_CLOSEPATROL ] =
		CreateTextButton( iEditorMercsToolbarText[28], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 368, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_FARPATROL ] =
		CreateTextButton( iEditorMercsToolbarText[29], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 380, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_POINTPATROL ] =
		CreateTextButton( iEditorMercsToolbarText[30], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 392, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	iEditorButton[ MERCS_ORDERS_RNDPTPATROL ] =
		CreateTextButton( iEditorMercsToolbarText[31], (UINT16)SMALLCOMPFONT, FONT_GRAY2, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 404, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetOrdersCallback );
	for ( x = 0; x < 8; x++ )
	{
		MSYS_SetBtnUserData( iEditorButton[ FIRST_MERCS_ORDERS_BUTTON + x ], 0, x);
	}

	//Attitudes
	iEditorButton[ MERCS_ATTITUDE_DEFENSIVE ] =
		CreateTextButton( iEditorMercsToolbarText[32], (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 424, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetAttitudeCallback );
	iEditorButton[ MERCS_ATTITUDE_BRAVESOLO ] =
		CreateTextButton( iEditorMercsToolbarText[33], (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 436, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetAttitudeCallback );
	iEditorButton[ MERCS_ATTITUDE_BRAVEAID ] =
		CreateTextButton( iEditorMercsToolbarText[34], (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 200, 2 * iScreenHeightOffset + 448, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetAttitudeCallback );
	iEditorButton[ MERCS_ATTITUDE_AGGRESSIVE ] =
		CreateTextButton( iEditorMercsToolbarText[35], (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 424, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetAttitudeCallback );
	iEditorButton[ MERCS_ATTITUDE_CUNNINGSOLO ] =
		CreateTextButton( iEditorMercsToolbarText[36], (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 436, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetAttitudeCallback );
	iEditorButton[ MERCS_ATTITUDE_CUNNINGAID ] =
		CreateTextButton( iEditorMercsToolbarText[37], (UINT16)SMALLCOMPFONT, FONT_GRAY4, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 270, 2 * iScreenHeightOffset + 448, 70, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK,
		MercsSetAttitudeCallback );
	for ( x = 0; x < 6; x++ )
	{
		MSYS_SetBtnUserData( iEditorButton[ FIRST_MERCS_ATTITUDE_BUTTON + x ], 0, x);
	}

	iEditorButton[ MERCS_DIRECTION_W ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 7, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 360, 2 * iScreenHeightOffset + 365, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_NW ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 0, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 390, 2 * iScreenHeightOffset + 365, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_N ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 1, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 420, 2 * iScreenHeightOffset + 365, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_NE] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 2, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 420, 2 * iScreenHeightOffset + 395, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_E ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 3, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 420, 2 * iScreenHeightOffset + 425, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_SE ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 4, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 390, 2 * iScreenHeightOffset + 425, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_S ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 5, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 360, 2 * iScreenHeightOffset + 425, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	iEditorButton[ MERCS_DIRECTION_SW ] =
		CreateIconButton((INT16)giEditMercDirectionIcons[0], 6, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 360, 2 * iScreenHeightOffset + 395, 30, 30, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsDirectionSetCallback );
	for ( x = 0; x < 8; x++ )
	{
		swprintf( TempString, iEditorMercsToolbarText[38], FaceDirs[x] );
		SetButtonFastHelpText( iEditorButton[ FIRST_MERCS_DIRECTION_BUTTON + x ], TempString );
		MSYS_SetBtnUserData( iEditorButton[ FIRST_MERCS_DIRECTION_BUTTON + x ], 0, x);
	}

	iEditorButton[ MERCS_DIRECTION_FIND ] =
		CreateTextButton( iEditorMercsToolbarText[39], (INT16)FONT12POINT1, FONT_MCOLOR_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 390, 2 * iScreenHeightOffset + 395, 30, 30, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL + 1, DEFAULT_MOVE_CALLBACK, MercsFindSelectedMercCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_DIRECTION_FIND] , iEditorMercsToolbarText[63] );

	iEditorButton[ MERCS_EQUIPMENT_BAD ] =
		CreateTextButton( iEditorMercsToolbarText[40], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 385, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_EQUIPMENT_BAD ], FONT_LTRED, FONT_BLACK );
	iEditorButton[ MERCS_EQUIPMENT_POOR ] =
		CreateTextButton( iEditorMercsToolbarText[41], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 400, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_EQUIPMENT_POOR ], FONT_ORANGE, FONT_BLACK );
	iEditorButton[ MERCS_EQUIPMENT_AVERAGE ] =
		CreateTextButton( iEditorMercsToolbarText[42], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 415, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_EQUIPMENT_AVERAGE ], FONT_YELLOW, FONT_BLACK );
	iEditorButton[ MERCS_EQUIPMENT_GOOD ] =
		CreateTextButton( iEditorMercsToolbarText[43], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 430, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_EQUIPMENT_GOOD ], FONT_LTGREEN, FONT_BLACK );
	iEditorButton[ MERCS_EQUIPMENT_GREAT ] =
		CreateTextButton( iEditorMercsToolbarText[44], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 445, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeEquipmentCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_EQUIPMENT_GREAT ], FONT_LTBLUE, FONT_BLACK );
	for ( x = 0; x < 5; x++ )
	{
		MSYS_SetBtnUserData( iEditorButton[ FIRST_MERCS_REL_EQUIPMENT_BUTTON + x ], 0, x);
	}

	iEditorButton[ MERCS_ATTRIBUTES_BAD ] =
		CreateTextButton( iEditorMercsToolbarText[45], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 530, 2 * iScreenHeightOffset + 385, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_ATTRIBUTES_BAD ], FONT_LTRED, FONT_BLACK );
	iEditorButton[ MERCS_ATTRIBUTES_POOR ] =
		CreateTextButton( iEditorMercsToolbarText[46], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 530, 2 * iScreenHeightOffset + 400, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_ATTRIBUTES_POOR ], FONT_ORANGE, FONT_BLACK );
	iEditorButton[ MERCS_ATTRIBUTES_AVERAGE ] =
		CreateTextButton( iEditorMercsToolbarText[47], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 530, 2 * iScreenHeightOffset + 415, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_ATTRIBUTES_AVERAGE ], FONT_YELLOW, FONT_BLACK );
	iEditorButton[ MERCS_ATTRIBUTES_GOOD ] =
		CreateTextButton( iEditorMercsToolbarText[48], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 530, 2 * iScreenHeightOffset + 430, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_ATTRIBUTES_GOOD ], FONT_LTGREEN, FONT_BLACK );
	iEditorButton[ MERCS_ATTRIBUTES_GREAT ] =
		CreateTextButton( iEditorMercsToolbarText[49], (INT16)SMALLCOMPFONT, FONT_GRAY1, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 530, 2 * iScreenHeightOffset + 445, 40, 15, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsSetRelativeAttributesCallback );
	SpecifyButtonDownTextColors( iEditorButton[ MERCS_ATTRIBUTES_GREAT ], FONT_LTBLUE, FONT_BLACK );
	for( x = 0; x < 5; x++ )
		MSYS_SetBtnUserData( iEditorButton[ FIRST_MERCS_REL_ATTRIBUTE_BUTTON + x ], 0, x);

	iEditorButton[ MERCS_ARMY_CODE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 575, 2 * iScreenHeightOffset + 410, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MercsSetEnemyColorCodeCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_ARMY_CODE ], 0, SOLDIER_CLASS_ARMY );
	iEditorButton[ MERCS_ADMIN_CODE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 575, 2 * iScreenHeightOffset + 424, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MercsSetEnemyColorCodeCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_ADMIN_CODE ], 0, SOLDIER_CLASS_ADMINISTRATOR );
	iEditorButton[ MERCS_ELITE_CODE ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 575, 2 * iScreenHeightOffset + 438, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MercsSetEnemyColorCodeCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_ELITE_CODE ], 0, SOLDIER_CLASS_ELITE );

	iEditorButton[ MERCS_CIVILIAN_GROUP ] =
		CreateTextButton( gszCivGroupNames[0], (INT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 574, 2 * iScreenHeightOffset + 410, 60, 25, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsCivilianGroupCallback );

	iEditorButton[ MERCS_TOGGLECOLOR_BUTTON ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 180, 2 * iScreenHeightOffset +364, "EDITOR//checkbox.sti", MSYS_PRIORITY_NORMAL, MercsToggleColorModeCallback );
	iEditorButton[MERCS_HAIRCOLOR_DOWN] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 200,2 * iScreenHeightOffset +364,"EDITOR//leftarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_HAIRCOLOR_UP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 360,2 * iScreenHeightOffset +364,"EDITOR//rightarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_SKINCOLOR_DOWN] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 200,2 * iScreenHeightOffset +388,"EDITOR//leftarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_SKINCOLOR_UP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 360,2 * iScreenHeightOffset +388,"EDITOR//rightarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_VESTCOLOR_DOWN] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 200,2 * iScreenHeightOffset +412,"EDITOR//leftarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_VESTCOLOR_UP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 360,2 * iScreenHeightOffset +412,"EDITOR//rightarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_PANTCOLOR_DOWN] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 200,2 * iScreenHeightOffset +436,"EDITOR//leftarrow.sti", MercsSetColorsCallback );
	iEditorButton[MERCS_PANTCOLOR_UP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 360,2 * iScreenHeightOffset +436,"EDITOR//rightarrow.sti", MercsSetColorsCallback );
	for ( x = FIRST_MERCS_COLOR_BUTTON; x < LAST_MERCS_COLOR_BUTTON; x+=2 )
	{
		SetButtonFastHelpText( iEditorButton[x], iEditorMercsToolbarText[50] );
		SetButtonFastHelpText( iEditorButton[x + 1], iEditorMercsToolbarText[51] );
		DisableButton( iEditorButton[ x ] );
		DisableButton( iEditorButton[ x + 1 ] );
	}

	iEditorButton[MERCS_BODYTYPE_DOWN] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 460, 2 * iScreenHeightOffset +364,"EDITOR//leftarrow.sti", MercsSetBodyTypeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_BODYTYPE_DOWN ], iEditorMercsToolbarText[52]);
	iEditorButton[MERCS_BODYTYPE_UP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 560, 2 * iScreenHeightOffset +364,"EDITOR//rightarrow.sti", MercsSetBodyTypeCallback );
	SetButtonFastHelpText( iEditorButton[ MERCS_BODYTYPE_UP ], iEditorMercsToolbarText[53]);

	iEditorButton[ MERCS_SCHEDULE_VARIANCE1 ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 309, 2 * iScreenHeightOffset +375, "EDITOR//SmCheckBox.sti", MSYS_PRIORITY_NORMAL, MercsScheduleToggleVariance1Callback );
	SetButtonFastHelpText( iEditorButton[ MERCS_SCHEDULE_VARIANCE1 ], iEditorMercsToolbarText[54]);
	iEditorButton[ MERCS_SCHEDULE_VARIANCE2 ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 309, 2 * iScreenHeightOffset +396, "EDITOR//SmCheckBox.sti", MSYS_PRIORITY_NORMAL, MercsScheduleToggleVariance2Callback );
	SetButtonFastHelpText( iEditorButton[ MERCS_SCHEDULE_VARIANCE2 ], iEditorMercsToolbarText[55]);
	iEditorButton[ MERCS_SCHEDULE_VARIANCE3 ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 309, 2 * iScreenHeightOffset +417, "EDITOR//SmCheckBox.sti", MSYS_PRIORITY_NORMAL, MercsScheduleToggleVariance3Callback );
	SetButtonFastHelpText( iEditorButton[ MERCS_SCHEDULE_VARIANCE3 ], iEditorMercsToolbarText[56]);
	iEditorButton[ MERCS_SCHEDULE_VARIANCE4 ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 309, 2 * iScreenHeightOffset +438, "EDITOR//SmCheckBox.sti", MSYS_PRIORITY_NORMAL, MercsScheduleToggleVariance4Callback );
	SetButtonFastHelpText( iEditorButton[ MERCS_SCHEDULE_VARIANCE4 ], iEditorMercsToolbarText[57]);

	iEditorButton[ MERCS_SCHEDULE_ACTION1 ] =
		CreateTextButton( iEditorMercsToolbarText[58], FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 186, 2 * iScreenHeightOffset + 373, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleAction1Callback );
	iEditorButton[ MERCS_SCHEDULE_ACTION2 ] =
		CreateTextButton( iEditorMercsToolbarText[59], FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 186, 2 * iScreenHeightOffset + 394, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleAction2Callback );
	iEditorButton[ MERCS_SCHEDULE_ACTION3 ] =
		CreateTextButton( iEditorMercsToolbarText[60], FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 186, 2 * iScreenHeightOffset + 415, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleAction3Callback );
	iEditorButton[ MERCS_SCHEDULE_ACTION4 ] =
		CreateTextButton( iEditorMercsToolbarText[61], FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 186, 2 * iScreenHeightOffset + 436, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleAction4Callback );

	iEditorButton[ MERCS_SCHEDULE_DATA1A ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 331, 2 * iScreenHeightOffset + 373, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData1ACallback );
	iEditorButton[ MERCS_SCHEDULE_DATA1B ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 381, 2 * iScreenHeightOffset + 373, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData1BCallback );
	iEditorButton[ MERCS_SCHEDULE_DATA2A ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 331, 2 * iScreenHeightOffset + 394, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData2ACallback );
	iEditorButton[ MERCS_SCHEDULE_DATA2B ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 381, 2 * iScreenHeightOffset + 394, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData2BCallback );
	iEditorButton[ MERCS_SCHEDULE_DATA3A ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 331, 2 * iScreenHeightOffset + 415, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData3ACallback );
	iEditorButton[ MERCS_SCHEDULE_DATA3B ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 381, 2 * iScreenHeightOffset + 415, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData3BCallback );
	iEditorButton[ MERCS_SCHEDULE_DATA4A ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 331, 2 * iScreenHeightOffset + 436, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData4ACallback );
	iEditorButton[ MERCS_SCHEDULE_DATA4B ] =
		CreateTextButton( L"", FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 381, 2 * iScreenHeightOffset + 436, 40, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleData4BCallback );
	iEditorButton[ MERCS_SCHEDULE_CLEAR ] =
		CreateTextButton( iEditorMercsToolbarText[62], FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 516, 2 * iScreenHeightOffset + 362, 77, 16, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, MercsScheduleClearCallback );
	HideEditorButtons( MERCS_SCHEDULE_DATA1A, MERCS_SCHEDULE_DATA4B );

	iEditorButton[ MERCS_HEAD_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+61, MERCPANEL_Y+0, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_HEAD_SLOT ], 0, HELMETPOS );
	iEditorButton[ MERCS_BODY_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+61, MERCPANEL_Y+22, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_BODY_SLOT ], 0, VESTPOS );
	iEditorButton[ MERCS_LEGS_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+62, MERCPANEL_Y+73, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_LEGS_SLOT ], 0, LEGPOS );
	iEditorButton[ MERCS_LEFTHAND_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+12, MERCPANEL_Y+43, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_LEFTHAND_SLOT ], 0, HANDPOS );
	iEditorButton[ MERCS_RIGHTHAND_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+90, MERCPANEL_Y+42, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_RIGHTHAND_SLOT ], 0, SECONDHANDPOS );
	iEditorButton[ MERCS_PACK1_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+166, MERCPANEL_Y+6, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_PACK1_SLOT ], 0, BIGPOCK1POS );
	iEditorButton[ MERCS_PACK2_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+166, MERCPANEL_Y+29, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_PACK2_SLOT ], 0, BIGPOCK2POS );
	iEditorButton[ MERCS_PACK3_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+166, MERCPANEL_Y+52, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_PACK3_SLOT ], 0, BIGPOCK3POS );
	iEditorButton[ MERCS_PACK4_SLOT ] =
		CreateCheckBoxButton(	MERCPANEL_X+166, MERCPANEL_Y+75, "EDITOR//smCheckbox.sti", MSYS_PRIORITY_NORMAL+1, MercsInventorySlotCallback );
	MSYS_SetBtnUserData( iEditorButton[ MERCS_PACK4_SLOT ], 0, BIGPOCK4POS );
}

void InitEditorBuildingsToolbar()
{
	iEditorButton[BUILDING_TOGGLE_ROOF_VIEW] =
		CreateTextButton(iEditorBuildingsToolbarText[0],(UINT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 110, 2 * iScreenHeightOffset + 400, 50, 15, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,BUTTON_NO_CALLBACK,
		BuildingToggleRoofViewCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_TOGGLE_ROOF_VIEW],iEditorBuildingsToolbarText[29]);
	iEditorButton[BUILDING_TOGGLE_WALL_VIEW] =
		CreateTextButton(iEditorBuildingsToolbarText[1],(UINT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 110, 2 * iScreenHeightOffset + 415, 50, 15, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,BUTTON_NO_CALLBACK,
		BuildingToggleWallViewCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_TOGGLE_WALL_VIEW],iEditorBuildingsToolbarText[30]);
	iEditorButton[BUILDING_TOGGLE_INFO_VIEW] =
		CreateTextButton(iEditorBuildingsToolbarText[2],(UINT16)SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 110, 2 * iScreenHeightOffset + 430, 50, 15, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,BUTTON_NO_CALLBACK,
		BuildingToggleInfoViewCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_TOGGLE_INFO_VIEW],iEditorBuildingsToolbarText[31]);
	//Selection method buttons
	iEditorButton[BUILDING_PLACE_WALLS] =
		CreateEasyToggleButton( iScreenWidthOffset + 180,2 * iScreenHeightOffset + 370,"EDITOR//wall.sti", BuildingWallCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_WALLS],iEditorBuildingsToolbarText[3]);//dnl fix for to many tooltip message in place walls
	iEditorButton[BUILDING_PLACE_DOORS] =
		CreateEasyToggleButton( iScreenWidthOffset + 210,2 * iScreenHeightOffset + 370,"EDITOR//door.sti",	BuildingDoorCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_DOORS],iEditorBuildingsToolbarText[4]);
	iEditorButton[BUILDING_PLACE_ROOFS] =
		CreateEasyToggleButton( iScreenWidthOffset + 240,2 * iScreenHeightOffset + 370,"EDITOR//roof.sti", BuildingRoofCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_ROOFS],iEditorBuildingsToolbarText[5]);
	iEditorButton[BUILDING_PLACE_WINDOWS] =
		CreateEasyToggleButton( iScreenWidthOffset + 180,2 * iScreenHeightOffset + 400,"EDITOR//window.sti", BuildingWindowCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_WINDOWS],iEditorBuildingsToolbarText[6]);
	iEditorButton[BUILDING_PLACE_BROKEN_WALLS] =
		CreateEasyToggleButton( iScreenWidthOffset + 210, 2 * iScreenHeightOffset + 400,"EDITOR//crackwall.sti", BuildingCrackWallCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_BROKEN_WALLS],iEditorBuildingsToolbarText[7]);
	iEditorButton[BUILDING_PLACE_FURNITURE] =
		CreateEasyToggleButton( iScreenWidthOffset + 240, 2 * iScreenHeightOffset + 400,"EDITOR//decor.sti", BuildingFurnitureCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_FURNITURE],iEditorBuildingsToolbarText[8]);
	iEditorButton[BUILDING_PLACE_DECALS] =
		CreateEasyToggleButton( iScreenWidthOffset + 180, 2 * iScreenHeightOffset + 430,"EDITOR//decal.sti", BuildingDecalCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_DECALS],iEditorBuildingsToolbarText[9]);
	iEditorButton[BUILDING_PLACE_FLOORS] =
		CreateEasyToggleButton( iScreenWidthOffset + 210, 2 * iScreenHeightOffset + 430,"EDITOR//floor.sti", BuildingFloorCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_FLOORS],iEditorBuildingsToolbarText[10]);
	iEditorButton[BUILDING_PLACE_TOILETS] =
		CreateEasyToggleButton( iScreenWidthOffset + 240, 2 * iScreenHeightOffset + 430,"EDITOR//toilet.sti", BuildingToiletCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_PLACE_TOILETS],iEditorBuildingsToolbarText[11]);

	//Smart method buttons
	iEditorButton[BUILDING_SMART_WALLS] =
		CreateEasyToggleButton( iScreenWidthOffset + 290, 2 * iScreenHeightOffset + 370,"EDITOR//wall.sti", BuildingSmartWallCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_SMART_WALLS],iEditorBuildingsToolbarText[12]);
	iEditorButton[BUILDING_SMART_DOORS] =
		CreateEasyToggleButton( iScreenWidthOffset + 320, 2 * iScreenHeightOffset + 370,"EDITOR//door.sti", BuildingSmartDoorCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_SMART_DOORS],iEditorBuildingsToolbarText[13]);
	iEditorButton[BUILDING_SMART_WINDOWS] =
		CreateEasyToggleButton( iScreenWidthOffset + 290, 2 * iScreenHeightOffset + 400,"EDITOR//window.sti", BuildingSmartWindowCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_SMART_WINDOWS],iEditorBuildingsToolbarText[14]);
	iEditorButton[BUILDING_SMART_BROKEN_WALLS] =
		CreateEasyToggleButton( iScreenWidthOffset + 320, 2 * iScreenHeightOffset + 400,"EDITOR//crackwall.sti", BuildingSmartCrackWallCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_SMART_BROKEN_WALLS],iEditorBuildingsToolbarText[15]);
	iEditorButton[BUILDING_DOORKEY] =
		CreateEasyToggleButton( iScreenWidthOffset + 290, 2 * iScreenHeightOffset + 430,"EDITOR//key.sti", BuildingDoorKeyCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_DOORKEY], iEditorBuildingsToolbarText[16] );

	iEditorButton[BUILDING_NEW_ROOM] =
		CreateEasyToggleButton( iScreenWidthOffset + 370, 2 * iScreenHeightOffset + 370,"EDITOR//newroom.sti", BuildingNewRoomCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_NEW_ROOM],iEditorBuildingsToolbarText[17]);
	iEditorButton[BUILDING_CAVE_DRAWING] =
		CreateEasyToggleButton( iScreenWidthOffset + 370, 2 * iScreenHeightOffset + 370,"EDITOR//caves.sti", BuildingCaveDrawingCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_CAVE_DRAWING],iEditorBuildingsToolbarText[18]);
	iEditorButton[BUILDING_SAW_ROOM] =
		CreateEasyToggleButton( iScreenWidthOffset + 370, 2 * iScreenHeightOffset + 400,"EDITOR//sawroom.sti", BuildingSawRoomCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_SAW_ROOM],iEditorBuildingsToolbarText[19]);
	iEditorButton[BUILDING_KILL_BUILDING] =
		CreateEasyToggleButton( iScreenWidthOffset + 370, 2 * iScreenHeightOffset + 430,"EDITOR//delroom.sti", BuildingKillBuildingCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_KILL_BUILDING],iEditorBuildingsToolbarText[20]);
	iEditorButton[BUILDING_NEW_ROOF] =
		CreateEasyToggleButton( iScreenWidthOffset + 400, 2 * iScreenHeightOffset + 430,"EDITOR//newroof.sti", BuildingNewRoofCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_NEW_ROOF],iEditorBuildingsToolbarText[21]);
	iEditorButton[BUILDING_COPY_BUILDING] =
		CreateEasyToggleButton( iScreenWidthOffset + 430, 2 * iScreenHeightOffset + 430,"EDITOR//copyroom.sti", BuildingCopyBuildingCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_COPY_BUILDING],iEditorBuildingsToolbarText[22]);
	iEditorButton[BUILDING_MOVE_BUILDING] =
		CreateEasyToggleButton( iScreenWidthOffset + 460, 2 * iScreenHeightOffset + 430,"EDITOR//moveroom.sti", BuildingMoveBuildingCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_MOVE_BUILDING],iEditorBuildingsToolbarText[23]);
	iEditorButton[BUILDING_DRAW_ROOMNUM] =
		CreateEasyToggleButton( iScreenWidthOffset + 410, 2 * iScreenHeightOffset + 370,"EDITOR//addTileRoom.sti", BuildingDrawRoomNumCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_DRAW_ROOMNUM],iEditorBuildingsToolbarText[24]);
	iEditorButton[BUILDING_ERASE_ROOMNUM] =
		CreateEasyToggleButton( iScreenWidthOffset + 440, 2 * iScreenHeightOffset + 370,"EDITOR//killTileRoom.sti", BuildingEraseRoomNumCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_ERASE_ROOMNUM],iEditorBuildingsToolbarText[25]);

	iEditorButton[BUILDING_TOGGLE_ERASEMODE] =
		CreateEasyToggleButton( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 400,"EDITOR//eraser.sti", BtnEraseCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_TOGGLE_ERASEMODE],iEditorBuildingsToolbarText[26]);
	iEditorButton[BUILDING_UNDO] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 530, 2 * iScreenHeightOffset + 400,"EDITOR//undo.sti", BtnUndoCallback);
	SetButtonFastHelpText(iEditorButton[BUILDING_UNDO],iEditorBuildingsToolbarText[27]);
	iEditorButton[BUILDING_CYCLE_BRUSHSIZE] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 430,"EDITOR//paint.sti",BtnBrushCallback);
	SetButtonFastHelpText( iEditorButton[ BUILDING_CYCLE_BRUSHSIZE ], iEditorBuildingsToolbarText[28]);
}

void InitEditorItemsToolbar()
{
	SetFontForeground( FONT_MCOLOR_LTRED );
	iEditorButton[ITEMS_WEAPONS] =
		CreateTextButton(iEditorItemsToolbarText[0],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 440, 39, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsWeaponsCallback);
	iEditorButton[ITEMS_AMMO] =
		CreateTextButton(iEditorItemsToolbarText[1],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 139, 2 * iScreenHeightOffset + 440, 40, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsAmmoCallback);
	iEditorButton[ITEMS_ARMOUR] =
		CreateTextButton(iEditorItemsToolbarText[2],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 179, 2 * iScreenHeightOffset + 440, 52, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsArmourCallback);
	iEditorButton[ITEMS_LBEGEAR] =
		CreateTextButton(iEditorItemsToolbarText[3],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 231, 2 * iScreenHeightOffset + 440, 30, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsLBECallback);
	iEditorButton[ITEMS_EXPLOSIVES] =
		CreateTextButton(iEditorItemsToolbarText[4],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 261, 2 * iScreenHeightOffset + 440, 30, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsExplosivesCallback);
	iEditorButton[ITEMS_EQUIPMENT1] =
		CreateTextButton(iEditorItemsToolbarText[5],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 291, 2 * iScreenHeightOffset + 440, 21, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsEquipment1Callback);
	iEditorButton[ITEMS_EQUIPMENT2] =
		CreateTextButton(iEditorItemsToolbarText[6],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 312, 2 * iScreenHeightOffset + 440, 21, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsEquipment2Callback);
	iEditorButton[ITEMS_EQUIPMENT3] =
		CreateTextButton(iEditorItemsToolbarText[7],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 333, 2 * iScreenHeightOffset + 440, 21, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsEquipment3Callback);
	iEditorButton[ITEMS_TRIGGERS] =
		CreateTextButton(iEditorItemsToolbarText[8],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 354, 2 * iScreenHeightOffset + 440, 59, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsTriggersCallback );
	iEditorButton[ITEMS_KEYS] =
		CreateTextButton(iEditorItemsToolbarText[9],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 413, 2 * iScreenHeightOffset + 440, 38, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsKeysCallback );
	iEditorButton[ITEMS_RANDOMITEM] =
		CreateTextButton(iEditorItemsToolbarText[10],(UINT16)BLOCKFONT, FONT_MCOLOR_DKWHITE, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 451, 2 * iScreenHeightOffset + 440, 29, 20, BUTTON_TOGGLE,MSYS_PRIORITY_NORMAL,DEFAULT_MOVE_CALLBACK,
		ItemsRandomItemCallback);

	iEditorButton[ITEMS_LEFTSCROLL] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 1, 2 * iScreenHeightOffset + 361,"EDITOR//leftscroll.sti", ItemsLeftScrollCallback);
	SetButtonFastHelpText(iEditorButton[ITEMS_LEFTSCROLL], iEditorItemsToolbarText[11]);
	iEditorButton[ITEMS_RIGHTSCROLL] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 50, 2 * iScreenHeightOffset + 361,"EDITOR//rightscroll.sti", ItemsRightScrollCallback);
	SetButtonFastHelpText(iEditorButton[ITEMS_RIGHTSCROLL], iEditorItemsToolbarText[12]);

}

void InitEditorMapInfoToolbar()
{
	iEditorButton[MAPINFO_ADD_LIGHT1_SOURCE] =
		CreateEasyToggleButton( iScreenWidthOffset + 10, SCREEN_HEIGHT - 118, "EDITOR//light.sti", BtnDrawLightsCallback );
	SetButtonFastHelpText(iEditorButton[MAPINFO_ADD_LIGHT1_SOURCE],iEditorMapInfoToolbarText[0]);

	iEditorButton[ MAPINFO_LIGHT_PANEL ] =
		CreateTextButton( 0, 0, 0, 0, BUTTON_USE_DEFAULT, iScreenWidthOffset + 45, SCREEN_HEIGHT - 118, 60, 50, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	SpecifyDisabledButtonStyle( iEditorButton[ MAPINFO_LIGHT_PANEL ], DISABLED_STYLE_NONE );
	DisableButton( iEditorButton[ MAPINFO_LIGHT_PANEL ] );
	iEditorButton[ MAPINFO_PRIMETIME_LIGHT ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 48, SCREEN_HEIGHT - 115, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MapInfoPrimeTimeRadioCallback );
	iEditorButton[ MAPINFO_NIGHTTIME_LIGHT ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 48, SCREEN_HEIGHT - 100, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MapInfoNightTimeRadioCallback );
	iEditorButton[ MAPINFO_24HOUR_LIGHT ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 48, SCREEN_HEIGHT - 85, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MapInfo24HourTimeRadioCallback );
	ClickEditorButton( gbDefaultLightType + MAPINFO_PRIMETIME_LIGHT );

	iEditorButton[MAPINFO_TOGGLE_FAKE_LIGHTS] =
		CreateEasyToggleButton( iScreenWidthOffset + 120, SCREEN_HEIGHT - 118, "EDITOR//fakelight.sti", BtnFakeLightCallback );
	SetButtonFastHelpText(iEditorButton[MAPINFO_TOGGLE_FAKE_LIGHTS], iEditorMapInfoToolbarText[1]);

	iEditorButton[ MAPINFO_RADIO_PANEL ] =
		CreateTextButton( 0, 0, 0, 0, BUTTON_USE_DEFAULT, iScreenWidthOffset + 207, SCREEN_HEIGHT - 118, 70, 50, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	SpecifyDisabledButtonStyle( iEditorButton[ MAPINFO_RADIO_PANEL ], DISABLED_STYLE_NONE );
	DisableButton( iEditorButton[ MAPINFO_RADIO_PANEL ] );
	iEditorButton[ MAPINFO_RADIO_NORMAL ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 210, SCREEN_HEIGHT - 115, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MapInfoNormalRadioCallback );
	iEditorButton[ MAPINFO_RADIO_BASEMENT ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 210, SCREEN_HEIGHT - 100, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MapInfoBasementRadioCallback );
	iEditorButton[ MAPINFO_RADIO_CAVES ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 210, SCREEN_HEIGHT - 85, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, MapInfoCavesRadioCallback );

	iEditorButton[MAPINFO_DRAW_EXITGRIDS] =
		CreateEasyToggleButton( iScreenWidthOffset + 305, SCREEN_HEIGHT - 108, "EDITOR//exitgridbut.sti", MapInfoDrawExitGridCallback );
	SetButtonFastHelpText(iEditorButton[MAPINFO_DRAW_EXITGRIDS],iEditorMapInfoToolbarText[2]);
	iEditorButton[MAPINFO_CYCLE_BRUSHSIZE] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 420, SCREEN_HEIGHT - 50, "EDITOR//paint.sti", BtnBrushCallback );
	SetButtonFastHelpText(iEditorButton[MAPINFO_CYCLE_BRUSHSIZE],iEditorMapInfoToolbarText[3]);
	iEditorButton[MAPINFO_UNDO] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 510, SCREEN_HEIGHT - 50, "EDITOR//undo.sti", BtnUndoCallback );
	SetButtonFastHelpText(iEditorButton[MAPINFO_UNDO],iEditorMapInfoToolbarText[4]);
	iEditorButton[MAPINFO_TOGGLE_ERASEMODE] =
		CreateEasyToggleButton( iScreenWidthOffset + 540, SCREEN_HEIGHT - 50, "EDITOR//eraser.sti", BtnEraseCallback);
	SetButtonFastHelpText(iEditorButton[MAPINFO_TOGGLE_ERASEMODE],iEditorMapInfoToolbarText[5]);

	iEditorButton[ MAPINFO_NORTH_POINT ] =
		CreateEasyToggleButton( iScreenWidthOffset + 540, SCREEN_HEIGHT - 115, "EDITOR//north.sti", MapInfoEntryPointsCallback);
	SetButtonFastHelpText( iEditorButton[ MAPINFO_NORTH_POINT ], iEditorMapInfoToolbarText[6]);

	iEditorButton[ MAPINFO_WEST_POINT ] =
		CreateEasyToggleButton( iScreenWidthOffset + 525, SCREEN_HEIGHT - 94, "EDITOR//west.sti", MapInfoEntryPointsCallback);
	SetButtonFastHelpText( iEditorButton[ MAPINFO_WEST_POINT ], iEditorMapInfoToolbarText[7]);

	iEditorButton[ MAPINFO_EAST_POINT ] =
		CreateEasyToggleButton( iScreenWidthOffset + 555, SCREEN_HEIGHT - 94, "EDITOR//east.sti", MapInfoEntryPointsCallback);
	SetButtonFastHelpText( iEditorButton[ MAPINFO_EAST_POINT ], iEditorMapInfoToolbarText[8]);

	iEditorButton[ MAPINFO_SOUTH_POINT ] =
		CreateEasyToggleButton( iScreenWidthOffset + 540, SCREEN_HEIGHT - 73, "EDITOR//south.sti", MapInfoEntryPointsCallback);
	SetButtonFastHelpText( iEditorButton[ MAPINFO_SOUTH_POINT ], iEditorMapInfoToolbarText[9]);

	iEditorButton[ MAPINFO_CENTER_POINT ] =
		CreateEasyToggleButton( iScreenWidthOffset + 590, SCREEN_HEIGHT - 105, "EDITOR//center.sti", MapInfoEntryPointsCallback);
	SetButtonFastHelpText( iEditorButton[ MAPINFO_CENTER_POINT ], iEditorMapInfoToolbarText[10]);

	iEditorButton[ MAPINFO_ISOLATED_POINT ] =
		CreateEasyToggleButton( iScreenWidthOffset + 590, SCREEN_HEIGHT - 84, "EDITOR//isolated.sti", MapInfoEntryPointsCallback);
	SetButtonFastHelpText( iEditorButton[ MAPINFO_ISOLATED_POINT ], iEditorMapInfoToolbarText[11]);
}

void InitEditorOptionsToolbar()
{
	iEditorButton[OPTIONS_NEW_MAP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 71,SCREEN_HEIGHT - 79,"EDITOR//new.sti",	BtnNewMapCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_NEW_MAP],iEditorOptionsToolbarText[0]);

	iEditorButton[OPTIONS_NEW_BASEMENT] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 101,SCREEN_HEIGHT - 79,"EDITOR//new.sti",	BtnNewBasementCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_NEW_BASEMENT],iEditorOptionsToolbarText[1]);

	iEditorButton[OPTIONS_NEW_CAVES] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 131,SCREEN_HEIGHT - 79,"EDITOR//new.sti", BtnNewCavesCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_NEW_CAVES],iEditorOptionsToolbarText[2]);

	iEditorButton[OPTIONS_SAVE_MAP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 161,SCREEN_HEIGHT - 79,"EDITOR//save.sti",	BtnSaveCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_SAVE_MAP],iEditorOptionsToolbarText[3]);

	iEditorButton[OPTIONS_LOAD_MAP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 191,SCREEN_HEIGHT - 79,"EDITOR//load.sti",	BtnLoadCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_LOAD_MAP],iEditorOptionsToolbarText[4]);

	iEditorButton[OPTIONS_CHANGE_TILESET] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 221,SCREEN_HEIGHT - 79,"EDITOR//tileset.sti", BtnChangeTilesetCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_CHANGE_TILESET],iEditorOptionsToolbarText[5]);

	iEditorButton[OPTIONS_LEAVE_EDITOR] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 251,SCREEN_HEIGHT - 79,"EDITOR//cancel.sti", BtnCancelCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_LEAVE_EDITOR],iEditorOptionsToolbarText[6]);

	iEditorButton[OPTIONS_QUIT_GAME] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 281,SCREEN_HEIGHT - 79,"EDITOR//cancel.sti", BtnQuitCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_QUIT_GAME],iEditorOptionsToolbarText[7]);

	//dnl ch9 071009
	iEditorButton[OPTIONS_RADAR_MAP] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 311,SCREEN_HEIGHT - 79,"EDITOR//tileset.sti", BtnRadarMapCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_RADAR_MAP],iEditorOptionsToolbarText[8]);

	//dnl ch33 160909
	iEditorButton[OPTIONS_VANILLA_MODE] =
		CreateCheckBoxButton(iScreenWidthOffset+71, SCREEN_HEIGHT-119, "EDITOR//smcheckbox.sti", MSYS_PRIORITY_NORMAL, VanillaModeCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_VANILLA_MODE],iEditorOptionsToolbarText[9]);

	// TODO.MAP
	iEditorButton[OPTIONS_RESIZE_MAP_ON_LOADING] =
		CreateCheckBoxButton(iScreenWidthOffset+5, SCREEN_HEIGHT-42, "EDITOR//smcheckbox.sti", MSYS_PRIORITY_NORMAL, ResizeMapOnLoadingCallback);
	SetButtonFastHelpText(iEditorButton[OPTIONS_RESIZE_MAP_ON_LOADING],iEditorOptionsToolbarText[10]);
}

void InitEditorTerrainToolbar()
{
	iEditorButton[TERRAIN_FGROUND_TEXTURES] =
		CreateEasyToggleButton( iScreenWidthOffset + 100,SCREEN_HEIGHT - 80,"EDITOR//downgrid.sti", BtnFgGrndCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_FGROUND_TEXTURES],iEditorTerrainToolbarText[0]);

	iEditorButton[TERRAIN_BGROUND_TEXTURES] =
		CreateEasyToggleButton( iScreenWidthOffset + 130,SCREEN_HEIGHT - 80,"EDITOR//upgrid.sti", BtnBkGrndCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_BGROUND_TEXTURES],iEditorTerrainToolbarText[1]);

	iEditorButton[TERRAIN_PLACE_CLIFFS] =
		CreateEasyToggleButton( iScreenWidthOffset + 160,SCREEN_HEIGHT - 80,"EDITOR//banks.sti", BtnBanksCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_CLIFFS],iEditorTerrainToolbarText[2]);

	iEditorButton[TERRAIN_PLACE_ROADS] =
		CreateEasyToggleButton( iScreenWidthOffset + 190,SCREEN_HEIGHT - 80,"EDITOR//road.sti", BtnRoadsCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_ROADS],iEditorTerrainToolbarText[3]);

	iEditorButton[TERRAIN_PLACE_DEBRIS] =
		CreateEasyToggleButton( iScreenWidthOffset + 220,SCREEN_HEIGHT - 80,"EDITOR//debris.sti", BtnDebrisCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_DEBRIS],iEditorTerrainToolbarText[4]);

	iEditorButton[TERRAIN_PLACE_TREES] =
		CreateEasyToggleButton( iScreenWidthOffset + 250,SCREEN_HEIGHT - 80,"EDITOR//tree.sti", BtnObjectCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_TREES],iEditorTerrainToolbarText[5]);

	iEditorButton[TERRAIN_PLACE_ROCKS] =
		CreateEasyToggleButton( iScreenWidthOffset + 280,SCREEN_HEIGHT - 80,"EDITOR//num1.sti", BtnObject1Callback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_ROCKS],iEditorTerrainToolbarText[6]);

	iEditorButton[TERRAIN_PLACE_MISC] =
		CreateEasyToggleButton( iScreenWidthOffset + 310,SCREEN_HEIGHT - 80,"EDITOR//num2.sti", BtnObject2Callback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_PLACE_MISC],iEditorTerrainToolbarText[7]);

	iEditorButton[TERRAIN_FILL_AREA] =
		CreateEasyToggleButton( iScreenWidthOffset + 100,SCREEN_HEIGHT - 50,"EDITOR//fill.sti", BtnFillCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_FILL_AREA],iEditorTerrainToolbarText[8]);

	iEditorButton[TERRAIN_UNDO] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 130,SCREEN_HEIGHT - 50,"EDITOR//undo.sti", BtnUndoCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_UNDO],iEditorTerrainToolbarText[9]);

	iEditorButton[TERRAIN_TOGGLE_ERASEMODE] =
		CreateEasyToggleButton( iScreenWidthOffset + 160,SCREEN_HEIGHT - 50,"EDITOR//eraser.sti", BtnEraseCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_TOGGLE_ERASEMODE],iEditorTerrainToolbarText[10]);

	iEditorButton[TERRAIN_CYCLE_BRUSHSIZE] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 190,SCREEN_HEIGHT - 50,"EDITOR//paint.sti", BtnBrushCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_CYCLE_BRUSHSIZE],iEditorTerrainToolbarText[11]);

	iEditorButton[TERRAIN_RAISE_DENSITY] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 350,SCREEN_HEIGHT - 50,"EDITOR//uparrow.sti", BtnIncBrushDensityCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_RAISE_DENSITY],iEditorTerrainToolbarText[12]);

	iEditorButton[TERRAIN_LOWER_DENSITY] =
		CreateEasyNoToggleButton( iScreenWidthOffset + 280,SCREEN_HEIGHT - 50,"EDITOR//downarrow.sti", BtnDecBrushDensityCallback);
	SetButtonFastHelpText(iEditorButton[TERRAIN_LOWER_DENSITY],iEditorTerrainToolbarText[13]);
}

void CreateEditorTaskbarInternal()
{
	//Create the tabs for the editor taskbar
	iEditorButton[ TAB_TERRAIN ] =
		CreateTextButton(iEditorTaskbarInternalText[0],  (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 100, SCREEN_HEIGHT - 20, 90, 20, BUTTON_TOGGLE,MSYS_PRIORITY_HIGH,BUTTON_NO_CALLBACK,
		TaskTerrainCallback);
	SpecifyButtonDownTextColors( iEditorButton[TAB_TERRAIN], FONT_YELLOW, FONT_ORANGE );
	SetButtonFastHelpText(iEditorButton[ TAB_TERRAIN ],iEditorTaskbarInternalText[6]);
	iEditorButton[ TAB_BUILDINGS ] =
		CreateTextButton(iEditorTaskbarInternalText[1], (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 190, SCREEN_HEIGHT - 20, 90, 20, BUTTON_TOGGLE,MSYS_PRIORITY_HIGH,BUTTON_NO_CALLBACK,
		TaskBuildingCallback);
	SpecifyButtonDownTextColors( iEditorButton[TAB_BUILDINGS], FONT_YELLOW, FONT_ORANGE );
	SetButtonFastHelpText(iEditorButton[ TAB_BUILDINGS ],iEditorTaskbarInternalText[7]);
	iEditorButton[ TAB_ITEMS ] =
		CreateTextButton(iEditorTaskbarInternalText[2], (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 280, SCREEN_HEIGHT - 20, 90, 20, BUTTON_TOGGLE,MSYS_PRIORITY_HIGH,BUTTON_NO_CALLBACK,
		TaskItemsCallback);
	SpecifyButtonDownTextColors( iEditorButton[TAB_ITEMS], FONT_YELLOW, FONT_ORANGE );
	SetButtonFastHelpText(iEditorButton[ TAB_ITEMS ],iEditorTaskbarInternalText[8]);
	iEditorButton[ TAB_MERCS ] =
		CreateTextButton(iEditorTaskbarInternalText[3], (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 370, SCREEN_HEIGHT - 20, 90, 20, BUTTON_TOGGLE,MSYS_PRIORITY_HIGH,BUTTON_NO_CALLBACK,
		TaskMercsCallback);
	SpecifyButtonDownTextColors( iEditorButton[TAB_MERCS], FONT_YELLOW, FONT_ORANGE );
	SetButtonFastHelpText(iEditorButton[ TAB_MERCS ],iEditorTaskbarInternalText[9]);
	iEditorButton[ TAB_MAPINFO ] =
		CreateTextButton(iEditorTaskbarInternalText[4], (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 460, SCREEN_HEIGHT - 20, 90, 20, BUTTON_TOGGLE,MSYS_PRIORITY_HIGH,BUTTON_NO_CALLBACK,
		TaskMapInfoCallback);
	SpecifyButtonDownTextColors( iEditorButton[TAB_MAPINFO], FONT_YELLOW, FONT_ORANGE );
	SetButtonFastHelpText(iEditorButton[ TAB_MAPINFO ],iEditorTaskbarInternalText[10]);
	iEditorButton[ TAB_OPTIONS ] =
		CreateTextButton(iEditorTaskbarInternalText[5], (UINT16)SMALLFONT1, FONT_LTKHAKI, FONT_DKKHAKI, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 550, SCREEN_HEIGHT - 20, 90, 20, BUTTON_TOGGLE,MSYS_PRIORITY_HIGH,BUTTON_NO_CALLBACK,
		TaskOptionsCallback);
	SpecifyButtonDownTextColors( iEditorButton[TAB_OPTIONS], FONT_YELLOW, FONT_ORANGE );
	SetButtonFastHelpText(iEditorButton[ TAB_OPTIONS ],iEditorTaskbarInternalText[11]);

	//Create the buttons within each tab.
	InitEditorTerrainToolbar();
	InitEditorBuildingsToolbar();
	InitEditorItemsToolbar();
	InitEditorMercsToolbar();
	InitEditorMapInfoToolbar();
	InitEditorOptionsToolbar();
	InitEditorItemStatsButtons();
}


#endif
