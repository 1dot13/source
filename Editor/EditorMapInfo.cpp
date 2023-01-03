#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include <windows.h>
	#include "tiledef.h"
	#include "edit_sys.h"
	#include "vsurface.h"
	#include "vobject.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "wcheck.h"
	#include "vsurface.h"
	#include "line.h"
	#include "input.h"
	#include "sysutil.h"
	#include "font.h"
	#include "Font Control.h"
	#include "editscreen.h"
	#include "selectwin.h"
	#include "video.h"
	#include "vobject_blitters.h"
	#include "interface panels.h"
	#include "interface items.h"
	#include "utilities.h"
	#include "World Items.h"
	#include "worldman.h"
	#include "overhead.h"	//GetSoldier
	#include "renderworld.h"
	#include "Animation Data.h"
	#include "Animation Control.h"
	#include "lighting.h"
	#include "EditorDefines.h"
	#include "EditorMercs.h"
	#include "EditorTerrain.h" //for access to TerrainTileDrawMode
	#include "Soldier Create.h" //The stuff that connects the editor generated information
	#include "Soldier Init List.h"
	#include "strategicmap.h"
	#include "Soldier Add.h"
	#include "Soldier Profile Type.h"
	#include "Soldier Profile.h"
	#include "Text Input.h"
	#include "Random.h"
	#include "wordwrap.h"
	#include "EditorItems.h"
	#include "Editor Taskbar Utils.h"
	#include "Exit Grids.h"
	#include "Editor Undo.h"
	#include "Item Statistics.h"
	#include "Map Information.h"
	#include "EditorMapInfo.h"
	#include "environment.h"
	#include "Simple Render Utils.h"
	#include "Text.h"
#endif

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;

INT8 gbDefaultLightType = PRIMETIME_LIGHT;

SGPPaletteEntry	gEditorLightColor;

BOOLEAN gfEditorForceShadeTableRebuild = FALSE;
//dnl ch43 290909
INT32 iNewMapWorldRows = OLD_WORLD_ROWS;
INT32 iNewMapWorldCols = OLD_WORLD_COLS;

void SetupTextInputForOptions(void)//dnl ch52 091009
{
	CHAR16 str[10];
	InitTextInputModeWithScheme(DEFAULT_SCHEME);
	AddUserInputField(NULL); // Just so we can use short cut keys while not typing.
	swprintf(str, L"%d", WORLD_ROWS);
	AddTextInputField( iScreenWidthOffset+5, 2*iScreenHeightOffset+394, 30, 18, MSYS_PRIORITY_NORMAL, str, 4, INPUTTYPE_NUMERICSTRICT);
	swprintf(str, L"%d", WORLD_COLS);
	AddTextInputField(iScreenWidthOffset+5, 2*iScreenHeightOffset+414, 30, 18, MSYS_PRIORITY_NORMAL, str, 4, INPUTTYPE_NUMERICSTRICT);
	DisableTextField(2);
}

void UpdateOptions()
{
	if (gfResizeMapOnLoading)
		EnableTextField(1);
	else
		DisableTextField(1);

	SetFont( FONT10ARIAL );
	SetFontShadow( FONT_NEARBLACK );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 399, L"Rows");
	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 419, L"Cols");

	SetFontForeground( FONT_RED );
}

void ExtractAndUpdateOptions(void)//dnl ch52 091009
{
	iNewMapWorldRows = max(min(GetNumericStrictValueFromField(1), WORLD_ROWS_MAX), OLD_WORLD_ROWS);
	iNewMapWorldRows /= 4;
	iNewMapWorldRows *= 4;
	iNewMapWorldCols = iNewMapWorldRows;
	//iNewMapWorldCols = max(min(GetNumericStrictValueFromField(2), WORLD_COLS_MAX), OLD_WORLD_COLS);
	//iNewMapWorldCols /= 4;
	//iNewMapWorldCols *= 4;
}

void SetupTextInputForMapInfo()
{
	CHAR16 str[10];

	InitTextInputModeWithScheme( DEFAULT_SCHEME );

	AddUserInputField( NULL );	//just so we can use short cut keys while not typing.

	//light rgb fields
	swprintf( str, L"%d", gEditorLightColor.peRed );
	AddTextInputField( iScreenWidthOffset + 10, 2 * iScreenHeightOffset + 394, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gEditorLightColor.peGreen );
	AddTextInputField( iScreenWidthOffset + 10, 2 * iScreenHeightOffset + 414, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gEditorLightColor.peBlue );
	AddTextInputField( iScreenWidthOffset + 10, 2 * iScreenHeightOffset + 434, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );

	swprintf( str, L"%d", gsLightRadius );
	AddTextInputField( iScreenWidthOffset + 120, 2 * iScreenHeightOffset + 394, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gfFakeLights ? gusSavedLightLevel : gusLightLevel );//dnl ch80 011213
	AddTextInputField( iScreenWidthOffset + 120, 2 * iScreenHeightOffset + 414, 25, 18, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );

	//Scroll restriction ID
	if( !gMapInformation.ubRestrictedScrollID )
		swprintf( str, L"" );
	else
		swprintf( str, L"%d", gMapInformation.ubRestrictedScrollID );
	AddTextInputField( iScreenWidthOffset + 210, 2 * iScreenHeightOffset + 420, 30, 20, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );

	//exit grid input fields
	swprintf( str, L"%c%d", gExitGrid.ubGotoSectorY + 'A' - 1, gExitGrid.ubGotoSectorX );
	AddTextInputField( iScreenWidthOffset + 338, 2 * iScreenHeightOffset + 363, 30, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_EXCLUSIVE_COORDINATE );
	swprintf( str, L"%d", gExitGrid.ubGotoSectorZ );
	AddTextInputField( iScreenWidthOffset + 338, 2 * iScreenHeightOffset + 383, 30, 18, MSYS_PRIORITY_NORMAL, str, 1, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gExitGrid.usGridNo );
	AddTextInputField( iScreenWidthOffset + 338, 2 * iScreenHeightOffset + 403, 50, 18, MSYS_PRIORITY_NORMAL, str, 7, INPUTTYPE_NUMERICSTRICT );
}

void UpdateMapInfo()
{
	SetFont( FONT10ARIAL );
	SetFontShadow( FONT_NEARBLACK );

	SetFontForeground( FONT_RED );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 399, pUpdateMapInfoText[0]);
	SetFontForeground( FONT_GREEN );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 419, pUpdateMapInfoText[1]);
	SetFontForeground( FONT_DKBLUE );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 439, pUpdateMapInfoText[2]);

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 65, 2 * iScreenHeightOffset + 369, pUpdateMapInfoText[3] );
	mprintf( iScreenWidthOffset + 65, 2 * iScreenHeightOffset + 382, pUpdateMapInfoText[4] );
	mprintf( iScreenWidthOffset + 65, 2 * iScreenHeightOffset + 397, pUpdateMapInfoText[5] );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 148, 2 * iScreenHeightOffset + 399, pUpdateMapInfoText[6] );

	if( !gfBasement && !gfCaves )
		SetFontForeground( FONT_DKYELLOW );
	mprintf( iScreenWidthOffset + 148, 2 * iScreenHeightOffset + 414, pUpdateMapInfoText[7] );
	mprintf( iScreenWidthOffset + 148, 2 * iScreenHeightOffset + 423, pUpdateMapInfoText[8] );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 230, 2 * iScreenHeightOffset + 369, pUpdateMapInfoText[9] );
	mprintf( iScreenWidthOffset + 230, 2 * iScreenHeightOffset + 384, pUpdateMapInfoText[10] );
	mprintf( iScreenWidthOffset + 230, 2 * iScreenHeightOffset + 399, pUpdateMapInfoText[11] );

	SetFontForeground( FONT_ORANGE );
	mprintf( iScreenWidthOffset + 250, 2 * iScreenHeightOffset + 420, pUpdateMapInfoText[12] );
	mprintf( iScreenWidthOffset + 250, 2 * iScreenHeightOffset + 430, pUpdateMapInfoText[13] );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 363, pUpdateMapInfoText[14] );
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 372, pUpdateMapInfoText[15] );
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 383, pUpdateMapInfoText[16] );
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 392, pUpdateMapInfoText[17] );
	mprintf( iScreenWidthOffset + 388, 2 * iScreenHeightOffset + 403, pUpdateMapInfoText[18] );
	mprintf( iScreenWidthOffset + 388, 2 * iScreenHeightOffset + 412, pUpdateMapInfoText[19] );
	SetFontForeground( FONT_RED );
}

void UpdateMapInfoFields()
{
	CHAR16 str[10];
	//Update the text fields to reflect the validated values.
	//light rgb fields
	swprintf( str, L"%d", gEditorLightColor.peRed );
	SetInputFieldStringWith16BitString( 1, str );
	swprintf( str, L"%d", gEditorLightColor.peGreen );
	SetInputFieldStringWith16BitString( 2, str );
	swprintf( str, L"%d", gEditorLightColor.peBlue );
	SetInputFieldStringWith16BitString( 3, str );

	swprintf( str, L"%d", gsLightRadius );
	SetInputFieldStringWith16BitString( 4, str );
	swprintf( str, L"%d", gusLightLevel );
	SetInputFieldStringWith16BitString( 5, str );

	if( !gMapInformation.ubRestrictedScrollID )
		swprintf( str, L"" );
	else
		swprintf( str, L"%d", gMapInformation.ubRestrictedScrollID );
	SetInputFieldStringWith16BitString( 6, str );

	ApplyNewExitGridValuesToTextFields();
}

void ExtractAndUpdateMapInfo()
{
	CHAR16 str[10];
	INT32 temp;
	BOOLEAN fUpdateLight1 = FALSE;
	//extract light1 colors
	temp = min( GetNumericStrictValueFromField( 1 ), 255 );
	if( temp != -1 && temp != gEditorLightColor.peRed )
	{
		fUpdateLight1 = TRUE;
		gEditorLightColor.peRed = (UINT8)temp;
	}
	temp = min( GetNumericStrictValueFromField( 2 ), 255 );
	if( temp != -1 && temp != gEditorLightColor.peGreen )
	{
		fUpdateLight1 = TRUE;
		gEditorLightColor.peGreen = (UINT8)temp;
	}
	temp = min( GetNumericStrictValueFromField( 3 ), 255 );
	if( temp != -1 && temp != gEditorLightColor.peBlue )
	{
		fUpdateLight1 = TRUE;
		gEditorLightColor.peBlue = (UINT8)temp;
	}
	if( fUpdateLight1 )
	{
		gfEditorForceShadeTableRebuild = TRUE;
		LightSetColors(	&gEditorLightColor, 1 );
		gfEditorForceShadeTableRebuild = FALSE;
	}

	//dnl ch80 011213 extract radius
	temp = max(min(GetNumericStrictValueFromField(4), 8), 1);
	if(temp != -1)
		gsLightRadius = (INT16)temp;
	temp = max(min(GetNumericStrictValueFromField(5), 15), 1);
	if(temp != -1 && temp != gusLightLevel)
	{
		gusLightLevel = (UINT16)temp;
		if(gfFakeLights)
			gusLightLevel = EDITOR_LIGHT_FAKE;
		LightSetBaseLevel((UINT8)(EDITOR_LIGHT_MAX - gusLightLevel));
		LightSpriteRenderAll();
		gfRenderWorld = TRUE;
	}
	temp = GetNumericStrictValueFromField(6);
	if(temp == -1)
		gMapInformation.ubRestrictedScrollID = 0;
	else
	{
		gMapInformation.ubRestrictedScrollID = (temp ? 1 : 0);
		InitRenderParams(gMapInformation.ubRestrictedScrollID);
	}

	//set up fields for exitgrid information
	Get16BitStringFromField( 7, str, 10 );
	if( str[0] >= 'a' && str[0] <= 'z' )
		str[0] -= 32; //uppercase it!
	if( str[0] >= 'A' && str[0] <= 'Z' &&
		str[1] >= '0' && str[1] <= '9' )
	{ //only update, if coordinate is valid.
		gExitGrid.ubGotoSectorY = (UINT8)(str[0] - 'A' + 1);
		gExitGrid.ubGotoSectorX = (UINT8)(str[1] - '0');
		if( str[2] >= '0' && str[2] <= '9' )
			gExitGrid.ubGotoSectorX = (UINT8)(gExitGrid.ubGotoSectorX * 10 + str[2] - '0' );
		gExitGrid.ubGotoSectorX = (UINT8)max( min( gExitGrid.ubGotoSectorX, 16 ), 1 );
		gExitGrid.ubGotoSectorY = (UINT8)max( min( gExitGrid.ubGotoSectorY, 16 ), 1 );
	}
	gExitGrid.ubGotoSectorZ	= (UINT8)max( min( GetNumericStrictValueFromField( 8 ), 3 ), 0 );
	gExitGrid.usGridNo					 = max( min( GetNumericStrictValueFromField( 9 ), (WORLD_COLS_MAX * WORLD_ROWS_MAX) ), 0 );

	UpdateMapInfoFields();
}

BOOLEAN ApplyNewExitGridValuesToTextFields()
{
	CHAR16 str[10];
	//exit grid input fields
	if( iCurrentTaskbar != TASK_MAPINFO )
		return FALSE;
	swprintf( str, L"%c%d", gExitGrid.ubGotoSectorY + 'A' - 1, gExitGrid.ubGotoSectorX );
	SetInputFieldStringWith16BitString( 7, str );
	swprintf( str, L"%d", gExitGrid.ubGotoSectorZ );
	SetInputFieldStringWith16BitString( 8, str );
	swprintf( str, L"%d", gExitGrid.usGridNo );
	SetInputFieldStringWith16BitString( 9, str );
	SetActiveField( 0 );
	return TRUE;
}

INT32 usCurrentExitGridNo = 0;
void LocateNextExitGrid()
{
	EXITGRID ExitGrid;
	INT32 i;
	for( i = usCurrentExitGridNo + 1; i < WORLD_MAX; i++ )
	{
		if( GetExitGrid( i, &ExitGrid ) )
		{
			usCurrentExitGridNo = i;
			CenterScreenAtMapIndex( i );
			return;
		}
	}
	for( i = 0; i < usCurrentExitGridNo; i++ )
	{
		if( GetExitGrid( i, &ExitGrid ) )
		{
			usCurrentExitGridNo = i;
			CenterScreenAtMapIndex( i );
			return;
		}
	}
}

void ChangeLightDefault( INT8 bLightType )
{
	UnclickEditorButton( MAPINFO_PRIMETIME_LIGHT + gbDefaultLightType );
	gbDefaultLightType = bLightType;
	ClickEditorButton( MAPINFO_PRIMETIME_LIGHT + gbDefaultLightType );
}

#endif







