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
	#include "input.h"
	#include "sysutil.h"
	#include "font.h"
	#include "Font Control.h"
	#include "editscreen.h"
	#include "selectwin.h"
	#include "video.h"
	#include "vobject_blitters.h"
	#include "interface panels.h"
	#include "utilities.h"
	#include "World Items.h"
	#include "EditorDefines.h"
	#include "EditorTerrain.h"
	#include "Editor Taskbar Utils.h"
	#include "Cursor Modes.h"
	#include "english.h"
#endif

BOOLEAN gfShowTerrainTileButtons;
UINT8 ubTerrainTileButtonWeight[NUM_TERRAIN_TILE_REGIONS];
UINT16 usTotalWeight;
BOOLEAN fPrevShowTerrainTileButtons = TRUE;
BOOLEAN fUseTerrainWeights = FALSE;
INT32 TerrainTileSelected = 0, TerrainForegroundTile, TerrainBackgroundTile;
INT32 TerrainTileDrawMode = TERRAIN_TILES_NODRAW;

void EntryInitEditorTerrainInfo()
{
	//ResetTerrainTileWeights();
	if( !fUseTerrainWeights )
	{
		ResetTerrainTileWeights();
	}
}

void ResetTerrainTileWeights()
{
	INT8 x;
	for( x=0; x < NUM_TERRAIN_TILE_REGIONS; x++ )
	{
		ubTerrainTileButtonWeight[x] = 0;
	}
	usTotalWeight = 0;
	fUseTerrainWeights = FALSE;
	gfRenderTaskbar = TRUE;
}

void HideTerrainTileButtons()
{
	INT8 x;
	if( gfShowTerrainTileButtons )
	{
		for( x = BASE_TERRAIN_TILE_REGION_ID; x < NUM_TERRAIN_TILE_REGIONS; x++ )
		{
			DisableEditorRegion( x );
		}
		gfShowTerrainTileButtons=FALSE;
	}
}

void ShowTerrainTileButtons()
{
	INT8 x;
	if( !gfShowTerrainTileButtons )
	{
		for( x = BASE_TERRAIN_TILE_REGION_ID; x < NUM_TERRAIN_TILE_REGIONS; x++ )
		{
			EnableEditorRegion( x );
		}
		gfShowTerrainTileButtons=TRUE;
	}
}

void RenderTerrainTileButtons()
{
	// If needed, display the ground tile images
	if(gfShowTerrainTileButtons)
	{
		UINT16 usFillColorDark, usFillColorLight, usFillColorRed;
		UINT16 x, usX, usX2, usY, usY2;

		usFillColorDark = Get16BPPColor(FROMRGB(24, 61, 81));
		usFillColorLight = Get16BPPColor(FROMRGB(136, 138, 135));
		usFillColorRed = Get16BPPColor(FROMRGB(255, 0, 0));

		usY = 2 * iScreenHeightOffset + 369;
		usY2 = 2 * iScreenHeightOffset + 391;

		SetFont( SMALLCOMPFONT );
		SetFontForeground( FONT_YELLOW );

		for( x = 0; x < NUM_TERRAIN_TILE_REGIONS; x++ )
		{
			usX = iScreenWidthOffset + 261 + (x * 42);
			usX2 = usX + 42;

			if ( x == CurrentPaste && !fUseTerrainWeights )
			{
				ColorFillVideoSurfaceArea(ButtonDestBuffer , usX, usY, usX2, usY2, usFillColorRed);
			}
			else
			{
				ColorFillVideoSurfaceArea(ButtonDestBuffer, usX, usY, usX2, usY2, usFillColorDark);
				ColorFillVideoSurfaceArea(ButtonDestBuffer, usX + 1, usY + 1, usX2, usY2, usFillColorLight);
			}
			ColorFillVideoSurfaceArea(ButtonDestBuffer, usX + 1, usY + 1, usX2 - 1, usY2 - 1, 0);

			SetObjectShade( gTileDatabase[gTileTypeStartIndex[x]].hTileSurface, DEFAULT_SHADE_LEVEL );
			BltVideoObject(ButtonDestBuffer, gTileDatabase[gTileTypeStartIndex[x]].hTileSurface, 0, (usX + 1), (usY + 1),
										VO_BLT_SRCTRANSPARENCY, NULL);

			if( fUseTerrainWeights )
			{
				mprintf( usX+2, usY+2, L"%d", ubTerrainTileButtonWeight[ x ] );
			}
		}
	}
}

//This callback is used for each of the terrain tile buttons.	The userData[0] field
//contains the terrain button's index value.
void TerrainTileButtonRegionCallback(MOUSE_REGION *reg,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gfRenderTaskbar = TRUE;
		TerrainTileSelected = MSYS_GetRegionUserData(reg,0);
		if(TerrainTileDrawMode == TERRAIN_TILES_FOREGROUND )
		{
			TerrainForegroundTile = TerrainTileSelected;
			CurrentPaste = (UINT16)TerrainForegroundTile;
			//iEditorToolbarState = TBAR_MODE_DRAW;
			if( _KeyDown( SHIFT ) )
			{
				fUseTerrainWeights = TRUE;
			}
			if( fUseTerrainWeights )
			{
				//SHIFT+LEFTCLICK adds weight to the selected terrain tile.
				if( ubTerrainTileButtonWeight[ TerrainTileSelected ] < 10 )
				{
					ubTerrainTileButtonWeight[ TerrainTileSelected ]++;
					usTotalWeight++;
				}
			}
			else
			{ //Regular LEFTCLICK selects only that terrain tile.
				//When total weight is 0, then the only selected tile is drawn.
				ResetTerrainTileWeights();
			}
		}
		else if(TerrainTileDrawMode == TERRAIN_TILES_BACKGROUND )
		{
			TerrainBackgroundTile = TerrainTileSelected;
			iEditorToolbarState = TBAR_MODE_SET_BGRND;
		}
	}
	if(reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		gfRenderTaskbar = TRUE;
		TerrainTileSelected = MSYS_GetRegionUserData( reg, 0 );
		if(TerrainTileDrawMode == TERRAIN_TILES_FOREGROUND )
		{
			TerrainForegroundTile = TerrainTileSelected;
			iEditorToolbarState = TBAR_MODE_DRAW;
			if( ubTerrainTileButtonWeight[ TerrainTileSelected ] )
			{
				ubTerrainTileButtonWeight[ TerrainTileSelected ]--;
				usTotalWeight--;
			}
		}
	}
}

void ChooseWeightedTerrainTile()
{
	UINT16 x, usWeight;
	INT16 sRandomNum;
	if(!usTotalWeight)
	{ //Not in the weighted mode.	CurrentPaste will already contain the selected tile.
		return;
	}
	sRandomNum = rand() % usTotalWeight;
	x = NUM_TERRAIN_TILE_REGIONS;
	for( x = 0; x < NUM_TERRAIN_TILE_REGIONS; x++ )
	{
		usWeight = ubTerrainTileButtonWeight[ x ];
		sRandomNum -= usWeight;
		if( sRandomNum <= 0 && usWeight )
		{
			CurrentPaste = x;
			return;
		}
	}
}


UINT32 guiSearchType;
// TODO: change this variable name if not too time consuming (jonathanl)
//dnl as jonathanl wrote count is changed to Count so this line could be deleted, extern UINT32 count; // symbol exist already in gamescreen.cpp, this is a REALLY bad global symbol name!!! (jonathanl)
//dnl Generally globals below are not necessary because is only informational for author, Count contains number of current Fill() recursions on stack, maxCount contains the maximum number of Fill() recursions that was putted on Stack during runtime, calls contains total number of Fill() calls during the runtime
UINT32 Count=0, maxCount=0, calls=0;

void Fill( INT32 x, INT32 y )
{
	INT32 iMapIndex;
	UINT32 uiCheckType;

	Count++;
	calls++;

	if( Count > maxCount )
		maxCount = Count;

	iMapIndex = y * WORLD_COLS + x;
	if( !GridNoOnVisibleWorldTile( iMapIndex ) )
	{
		Count--;
		return;
	}
	GetTileType( gpWorldLevelData[ iMapIndex ].pLandHead->usIndex , &uiCheckType );
	if( guiSearchType == uiCheckType )
	{
		PasteTextureCommon( iMapIndex );
		//dnl This condition is mandatory because PasteTextureCommon() not returning information if was successful in adding a tile, without this condition endless loop of calling recursion Fill() was occur when tile was not add. Still without checking Count Stack breach could appear in future if we decide to use very large maps!!!
		GetTileType( gpWorldLevelData[ iMapIndex ].pLandHead->usIndex , &uiCheckType );
		if( guiSearchType == uiCheckType )
		{
			Count--;
			return;
		}
	}
	else
	{
		Count--;
		return;
	}

	if( y > 0 )
		Fill( x, y-1 );
	if( y < WORLD_ROWS - 1 )
		Fill( x, y+1 );
	if( x > 0 )
		Fill( x-1, y );
	if( x < WORLD_COLS - 1 )
		Fill( x+1, y );
	Count--;
}


void TerrainFill( INT32 iMapIndex )
{
	INT16 sX, sY;
	//determine what we should be looking for to replace...
	GetTileType( gpWorldLevelData[ iMapIndex ].pLandHead->usIndex, &guiSearchType );

	//check terminating conditions
	if( guiSearchType == CurrentPaste )
		return;

	ConvertGridNoToXY( iMapIndex, &sX, &sY );

	Count = 0;

	Fill( sX, sY );

}


#endif







