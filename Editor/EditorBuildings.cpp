#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include "tiledef.h"
	#include "edit_sys.h"
	#include "sysutil.h"
	#include "Font Control.h"
	#include "renderworld.h"	//ClearRenderFlags();
	#include "worldman.h"			//HideStructOfGivenType()
	#include "EditorDefines.h"
	#include "EditorBuildings.h"
	#include "EditorTerrain.h" //for access to TerrainTileDrawMode
	#include "Render Fun.h"
	#include "newsmooth.h"
	#include "Editor Undo.h"
	#include "Editor Taskbar Utils.h"
	#include "Editor Modes.h"
	#include "Smoothing Utils.h"
	#include "Text Input.h"
	#include "keys.h"
	#include "environment.h"
	#include "selectwin.h"
	#include "Simple Render Utils.h"
	#include "Text.h"
	//dnl ch86 100214
	#include "lighting.h"
	#include "Exit Grids.h"
	#include "editscreen.h"
	#include "EditorItems.h"
	#include "EditorMapInfo.h"
#endif

BOOLEAN fBuildingShowRoofs, fBuildingShowWalls, fBuildingShowRoomInfo;
UINT16 usCurrentMode;
//DBrot: More Rooms
//UINT8 gubCurrRoomNumber;
//UINT8 gubMaxRoomNumber;
UINT16 gusCurrRoomNumber;
UINT16 gusMaxRoomNumber;
BOOLEAN	gfEditingDoor;

//BEGINNNING OF BUILDING INITIALIZATION FUNCTIONS
void GameInitEditorBuildingInfo()
{
	fBuildingShowRoofs = TRUE;
	fBuildingShowWalls = TRUE;
	fBuildingShowRoomInfo = FALSE;
	usCurrentMode = BUILDING_PLACE_WALLS;
	gusCurrRoomNumber = gusMaxRoomNumber = 1;
}

//BEGINNING OF BUILDING UTILITY FUNCTIONS
void UpdateRoofsView()//dnl ch80 011213
{
	INT32 x, cnt, tiletypecnt;
	/* // hide roof structures only
	UINT16 usType[12] = {FIRSTROOF, SECONDROOF, THIRDROOF, FOURTHROOF, FIRSTSLANTROOF, SECONDSLANTROOF, FIRSTONROOF, SECONDONROOF, FIRSTWALL, SECONDWALL, THIRDWALL, FOURTHWALL};
	for(cnt=0; cnt<WORLD_MAX; cnt++)
	{
		x = 12;
		while(x--)
			HideStructOfGivenType(cnt, usType[x], !fBuildingShowRoofs);
	}
	*/
	
	// Buggler: hide all tiles on roof
	x = FIRSTSWITCHES + 1;
	for(cnt=0; cnt<WORLD_MAX; cnt++)
	{
		for(tiletypecnt=0; tiletypecnt<x; tiletypecnt++)
			HideStructOfGivenType(cnt, tiletypecnt, !fBuildingShowRoofs);
	}
	gfRenderWorld = TRUE;
}

void UpdateWallsView()
{
	INT32 cnt;
	for ( cnt = 0; cnt < WORLD_MAX; cnt++ )
	{
		if ( fBuildingShowWalls )
		{
			RemoveWallLevelnodeFlags( cnt, LEVELNODE_HIDDEN );
		}
		else
		{
			SetWallLevelnodeFlags( cnt, LEVELNODE_HIDDEN );
		}
	}
	gfRenderWorld = TRUE;
}

void UpdateBuildingsInfo()
{
	//print the headers on top of the columns
	SetFont( SMALLCOMPFONT );
	SetFontForeground( FONT_RED );
	mprintfEditor( iScreenWidthOffset + 112, 2 * iScreenHeightOffset + 362, iUpdateBuildingsInfoText[0]);
	mprintfEditor( iScreenWidthOffset + 114, 2 * iScreenHeightOffset + 372, iUpdateBuildingsInfoText[1]);
	SetFontForeground( FONT_YELLOW );
	mprintfEditor( iScreenWidthOffset + 185, 2 * iScreenHeightOffset + 362, iUpdateBuildingsInfoText[2]);
	SetFontForeground( FONT_LTGREEN );
	mprintfEditor( iScreenWidthOffset + 290, 2 * iScreenHeightOffset + 362, iUpdateBuildingsInfoText[3]);
	SetFontForeground( FONT_LTBLUE );
	mprintfEditor( iScreenWidthOffset + 390, 2 * iScreenHeightOffset + 362, iUpdateBuildingsInfoText[4]);
	SetFontForeground( FONT_GRAY2 );
	mprintfEditor( iScreenWidthOffset + 452, 2 * iScreenHeightOffset + 404, iUpdateBuildingsInfoText[5] );
}

//Uses a recursive method to elimate adjacent tiles of structure information.
//The code will attempt to delete the current mapindex, then search using this method:
//1) if there isn't structure info here, return.
//2) if there is structure info here, delete it now.
//3) KillBuilding at x-1, y.
//4) KillBuilding at x	, y-1.
//5) KillBuilding at x+1, y.
//6) KillBuilding at x	, y+1.
void KillBuilding( INT32 iMapIndex )
{
	BOOLEAN fFound = FALSE;

	if( !gfBasement )
		fFound |= RemoveAllRoofsOfTypeRange( iMapIndex, FIRSTTEXTURE, LASTITEM );
	fFound |= RemoveAllLandsOfTypeRange( iMapIndex, FIRSTFLOOR, LASTFLOOR );

	EraseBuilding( iMapIndex );
	gusWorldRoomInfo[ iMapIndex ] = 0;

	if( !fFound )
	{
		if( gfBasement )
			RebuildRoof( iMapIndex, 0 );
		return;
	}

	if( GridNoOnVisibleWorldTile( iMapIndex - WORLD_COLS  ) )
		KillBuilding( iMapIndex - WORLD_COLS );
	if( GridNoOnVisibleWorldTile( iMapIndex + WORLD_COLS  ) )
		KillBuilding( iMapIndex + WORLD_COLS );
	if( GridNoOnVisibleWorldTile( iMapIndex + 1  ) )
		KillBuilding( iMapIndex + 1 );
	if( GridNoOnVisibleWorldTile( iMapIndex - 1  ) )
		KillBuilding( iMapIndex - 1 );

	if( gfBasement )
		RebuildRoof( iMapIndex, 0 );
}

BUILDINGLAYOUTNODE *gpBuildingLayoutList = NULL;
INT32 gsBuildingLayoutAnchorGridNo = -1;
extern void RemoveBuildingLayout();

void DeleteBuildingLayout()
{
	BUILDINGLAYOUTNODE *curr;
	//Erases the cursors associated with them.
	RemoveBuildingLayout();
	while( gpBuildingLayoutList )
	{
		curr = gpBuildingLayoutList;
		gpBuildingLayoutList = gpBuildingLayoutList->next;
		MemFree( curr );
	}
	gpBuildingLayoutList = NULL;
	gsBuildingLayoutAnchorGridNo = -1;
}

void BuildLayout( INT32 iMapIndex, INT32 iOffset )
{
	BUILDINGLAYOUTNODE *curr;
	//First, validate the gridno
	iMapIndex += iOffset;
	if( iMapIndex < 0 && iMapIndex >= WORLD_COLS*WORLD_ROWS )
		return;
	//Now, check if there is a building here
	if( !BuildingAtGridNo( iMapIndex ) )
	{
		if( iOffset == 1 && !BuildingAtGridNo( iMapIndex - 1 ) )
			return;
		if( iOffset == WORLD_COLS && !BuildingAtGridNo( iMapIndex - WORLD_COLS ) )
			return;
		if( iOffset == -1 && !GetVerticalWall( iMapIndex ) )
			return;
		if( iOffset == -WORLD_COLS && !GetHorizontalWall( iMapIndex ) )
			return;
	}
	//Now, check to make sure this gridno hasn't already been processed.
	curr = gpBuildingLayoutList;
	while( curr )
	{
		if( iMapIndex == curr->sGridNo )
			return;
		curr = curr->next;
	}
	//Good, it hasn't, so process it and add it to the head of the list.
	curr = (BUILDINGLAYOUTNODE*)MemAlloc( sizeof( BUILDINGLAYOUTNODE ) );
	Assert( curr );
	curr->sGridNo = iMapIndex;
	curr->next = gpBuildingLayoutList;
	gpBuildingLayoutList = curr;

	//Use recursion to process the remainder.
	BuildLayout( iMapIndex, -WORLD_COLS );
	BuildLayout( iMapIndex, -1 );
	BuildLayout( iMapIndex, 1 );
	BuildLayout( iMapIndex, WORLD_COLS );
}

//The first step is copying a building.	After that, it either must be pasted or moved.
void CopyBuilding( INT32 iMapIndex )
{
	AssertMsg( !gpBuildingLayoutList, "Error:	Attempting to copy building multiple times." );

	//First step is to determine if we have a building in the area that we click.	If not, do nothing.
	if( !BuildingAtGridNo( iMapIndex ) )
		return;
	//Okay, a building does exist here to some undetermined capacity.
	//Allocate the basic structure, then calculate the layout.	The head node is
	gpBuildingLayoutList = (BUILDINGLAYOUTNODE*)MemAlloc( sizeof( BUILDINGLAYOUTNODE ) );
	Assert( gpBuildingLayoutList );
	gpBuildingLayoutList->sGridNo = iMapIndex;
	gpBuildingLayoutList->next = NULL;

	//Set the anchor point for this building -- this is where the user clicked.
	gsBuildingLayoutAnchorGridNo = iMapIndex;

	//Now, recursively expand out while adding unique gridnos to our list.	The recursion will
	//terminate when complete.
	BuildLayout( iMapIndex, -WORLD_COLS );
	BuildLayout( iMapIndex, -1 );
	BuildLayout( iMapIndex, 1 );
	BuildLayout( iMapIndex, WORLD_COLS );

	//We have our layout.	Now depending on the mode, we will either move the building or
	//copy it.	The layout automatically gets deleted as soon as the user releases the mouse
	//button.
}

//depending on the offset, we will either sort in increasing order, or decreasing order.
//This will prevent overlapping problems.
void SortBuildingLayout( INT32 iMapIndex )
{
	BUILDINGLAYOUTNODE *head, *curr, *prev, *prevBest = NULL, *best = NULL;
	INT32 iBestIndex;
	head = NULL;
	if( iMapIndex < gsBuildingLayoutAnchorGridNo )
	{	//Forward sort (in increasing order)
		while( gpBuildingLayoutList )
		{
			iBestIndex = -1;
			curr = gpBuildingLayoutList;
			prev = NULL;
			while( curr )
			{
				if( iBestIndex < curr->sGridNo )
				{
					iBestIndex = curr->sGridNo;
					prevBest = prev;
					best = curr;
				}
				prev = curr;
				curr = curr->next;
			}
			//detach node from real list
			if( prevBest )
				prevBest->next = best->next;
			if( best == gpBuildingLayoutList )
				gpBuildingLayoutList = gpBuildingLayoutList->next;
			//insert node into temp sorted list
			best->next = head;
			head = best;
		}
	}
	else
	{ //Reverse sort (in decreasing order)
		while( gpBuildingLayoutList )
		{
			iBestIndex = 100000;
			curr = gpBuildingLayoutList;
			prev = NULL;
			while( curr )
			{
				if( iBestIndex > curr->sGridNo )
				{
					iBestIndex = curr->sGridNo;
					prevBest = prev;
					best = curr;
				}
				prev = curr;
				curr = curr->next;
			}
			//detach node from real list
			if( prevBest )
				prevBest->next = best->next;
			if( best == gpBuildingLayoutList )
				gpBuildingLayoutList = gpBuildingLayoutList->next;
			//insert node into temp sorted list
			best->next = head;
			head = best;
		}
	}
	//Now assign the newly sorted list back to the real list.
	gpBuildingLayoutList = head;
}

void PasteMapElementToNewMapElement( INT32 iSrcGridNo, INT32 iDstGridNo )
{
	MAP_ELEMENT			*pSrcMapElement;
	LEVELNODE				*pNode;
	//UINT16					usType;//dnl ch86 110214

	DeleteStuffFromMapTile( iDstGridNo );
	DeleteAllLandLayers( iDstGridNo );

	//Get a pointer to the src mapelement
	pSrcMapElement = &gpWorldLevelData[ iSrcGridNo ];

	//Go through each levelnode, and paste the info into the new gridno
	pNode = pSrcMapElement->pLandHead;
	while( pNode )
	{
		if( pNode == pSrcMapElement->pLandStart )
			gpWorldLevelData[ iDstGridNo ].pLandStart = AddLandToTail( iDstGridNo, pNode->usIndex );
		else
			AddLandToTail( iDstGridNo, pNode->usIndex );
		pNode = pNode->pNext;
	}
	pNode = pSrcMapElement->pObjectHead;
	while( pNode )
	{
		if(pNode->usIndex != GOODRING1)//dnl ch86 130214 skip light circle marker
			AddObjectToTail(iDstGridNo, pNode->usIndex);
		pNode = pNode->pNext;
	}
	pNode = pSrcMapElement->pStructHead;
	while( pNode )
	{
		if(!(pNode->uiFlags & LEVELNODE_ITEM))//dnl ch86 120214 skip items
			AddStructToTail(iDstGridNo, pNode->usIndex);
		pNode = pNode->pNext;
	}
	pNode = pSrcMapElement->pShadowHead;
	while( pNode )
	{
		if(!(pNode->uiFlags & LEVELNODE_EXITGRID))//dnl ch86 120214 skip exit grids
			AddShadowToTail(iDstGridNo, pNode->usIndex);
		pNode = pNode->pNext;
	}
	pNode = pSrcMapElement->pRoofHead;
	while( pNode )
	{
		AddRoofToTail( iDstGridNo, pNode->usIndex );
		pNode = pNode->pNext;
	}
	pNode = pSrcMapElement->pOnRoofHead;
	while( pNode )
	{
		if(!(pNode->uiFlags & LEVELNODE_ITEM))//dnl ch86 120214 skip items
			AddOnRoofToTail(iDstGridNo, pNode->usIndex);
		pNode = pNode->pNext;
	}
	pNode = pSrcMapElement->pTopmostHead;
	while( pNode )
	{
		if( !(pNode->usIndex == FIRSTPOINTERS1 || pNode->usIndex == ROTATINGKEY1 || pNode->usIndex == SELRING1) )//dnl ch86 130214
			AddTopmostToTail( iDstGridNo, pNode->usIndex );
		pNode = pNode->pNext;
	}
#if 0//dnl ch86 110214
	for ( usType = FIRSTROOF; usType <= LASTSLANTROOF; usType++ )
	{
		HideStructOfGivenType( iDstGridNo, usType, (BOOLEAN)(!fBuildingShowRoofs) );
	}
#endif
}

#if 0//dnl ch86 220214
void MoveBuilding( INT32 iMapIndex )
{
	BUILDINGLAYOUTNODE *curr;
	INT32 iOffset;
	if( !gpBuildingLayoutList )
		return;
	SortBuildingLayout( iMapIndex );
	iOffset = iMapIndex - gsBuildingLayoutAnchorGridNo;
	if(iOffset == 0)//dnl ch32 080909
		return;
	//First time, set the undo gridnos to everything effected.
	curr = gpBuildingLayoutList;
	while( curr )
	{
		AddToUndoList( curr->sGridNo );
		AddToUndoList( curr->sGridNo + iOffset );
		curr = curr->next;
	}
	//Now, move the building
	curr = gpBuildingLayoutList;
	while( curr )
	{
		PasteMapElementToNewMapElement( curr->sGridNo, curr->sGridNo + iOffset );
		DeleteStuffFromMapTile( curr->sGridNo );
		curr = curr->next;
	}
	MarkWorldDirty();
}
#else
void MoveBuilding( INT32 iMapIndex )
{
	INT8 bLightType;
	UINT8 ubLightRadius, ubLightId;
	INT16 sX, sY;
	INT32 iOffset, iNewGridNo;
	EXITGRID ExitGrid;
	DOOR Door, *pDoor;
	BUILDINGLAYOUTNODE *curr;
	if(!gpBuildingLayoutList)
		return;
	SortBuildingLayout(iMapIndex);
	iOffset = iMapIndex - gsBuildingLayoutAnchorGridNo;
	if(iOffset == 0)//dnl ch32 080909
		return;
	// First time, set the undo gridnos to everything effected.
	curr = gpBuildingLayoutList;
	while(curr)
	{
		AddToUndoList(curr->sGridNo);
		AddToUndoList(curr->sGridNo + iOffset);
		curr = curr->next;
	}
	// Now, move the building
	curr = gpBuildingLayoutList;
	while(curr)
	{
		iNewGridNo = curr->sGridNo + iOffset;
		PasteMapElementToNewMapElement(curr->sGridNo, iNewGridNo);
		PasteRoomNumber(iNewGridNo, gusWorldRoomInfo[curr->sGridNo]);
		if(GetExitGrid(curr->sGridNo, &ExitGrid))
			AddExitGridToWorld(iNewGridNo, &ExitGrid);
		ConvertGridNoToXY(iNewGridNo, &sX, &sY);
		for(bLightType=PRIMETIME_LIGHT; bLightType<ANY_LIGHT; bLightType++)
			if(FindLight(curr->sGridNo, bLightType, &ubLightRadius, &ubLightId))
				PlaceLight(ubLightRadius, sX, sY, ubLightId, bLightType);
		if((pDoor=FindDoorInfoAtGridNo(curr->sGridNo)) != NULL)
		{
			Door = *pDoor;
			Door.sGridNo = iNewGridNo;
			AddDoorInfoToTable(&Door);
			AddTopmostToHead(Door.sGridNo, ROTATINGKEY1);
		}
		ITEM_POOL *pItemPool;
		if(GetItemPoolFromGround(curr->sGridNo, &pItemPool))
		{
			MergeItemPoolInUndoList(iNewGridNo, pItemPool);
			UpdateItemPoolMoveInUndoList(curr->sGridNo, iNewGridNo);
			ItemPoolListMove(curr->sGridNo, iNewGridNo, pItemPool);
			AddItemPoolGraphic(pItemPool);
			ShowItemCursor(iNewGridNo);
		}
		DeleteStuffFromMapTile(curr->sGridNo);
		curr = curr->next;
	}
	UndoItemPoolGraphicInUndoList();// Reconstruct item pool graphic if buliding was paste over them
	UpdateRoofsView();
	UpdateWallsView();
	MarkWorldDirty();
	LightSpriteRenderAll();
}
#endif

void PasteBuilding( INT32 iMapIndex )
{
	BUILDINGLAYOUTNODE *curr;
	INT32 iOffset;
	if( !gpBuildingLayoutList )
		return;
	SortBuildingLayout( iMapIndex );
	iOffset = iMapIndex - gsBuildingLayoutAnchorGridNo;
	if(iOffset == 0)//dnl ch32 080909
		return;
	curr = gpBuildingLayoutList;
	//First time, set the undo gridnos to everything effected.
	while( curr )
	{
		AddToUndoList( curr->sGridNo );
		AddToUndoList( curr->sGridNo + iOffset );
		curr = curr->next;
	}
	//Now, paste the building (no smoothing)
	curr = gpBuildingLayoutList;
	while( curr )
	{
		PasteMapElementToNewMapElement( curr->sGridNo, curr->sGridNo + iOffset );
		curr = curr->next;
	}
	//dnl ch86 220214
	UndoItemPoolGraphicInUndoList();
	UpdateRoofsView();
	UpdateWallsView();
	MarkWorldDirty();
}

typedef struct ROOFNODE
{
	INT32 iMapIndex;
	struct ROOFNODE *next;
}ROOFNODE;

ROOFNODE *gpRoofList = NULL;

void ReplaceRoof( INT32 iMapIndex, UINT16 usRoofType )
{
	ROOFNODE *curr;
	//First, validate the gridno
	if( iMapIndex < 0 && iMapIndex >= WORLD_COLS*WORLD_ROWS )
		return;
	//Now, check if there is a floor here
	if( !FloorAtGridNo( iMapIndex ) )
		return;
	//Now, check to make sure this gridno hasn't already been processed.
	curr = gpRoofList;
	while( curr )
	{
		if( iMapIndex == curr->iMapIndex )
			return;
		curr = curr->next;
	}
	//Good, it hasn't, so process it and add it to the head of the list.
	curr = (ROOFNODE*)MemAlloc( sizeof( ROOFNODE ) );
	Assert( curr );
	curr->iMapIndex = iMapIndex;
	curr->next = gpRoofList;
	gpRoofList = curr;

	RebuildRoofUsingFloorInfo( iMapIndex, usRoofType );

	//Use recursion to process the remainder.
	ReplaceRoof( iMapIndex - WORLD_COLS, usRoofType );
	ReplaceRoof( iMapIndex + WORLD_COLS, usRoofType );
	ReplaceRoof( iMapIndex - 1, usRoofType );
	ReplaceRoof( iMapIndex + 1, usRoofType );
}

void ReplaceBuildingWithNewRoof( INT32 iMapIndex )
{
	UINT16 usRoofType;
	ROOFNODE *curr;
	//Not in normal editor mode, then can't do it.
	if( gfBasement || gfCaves )
		return;
	//if we don't have a floor here, then we can't replace the roof!
	if( !FloorAtGridNo( iMapIndex ) )
		return;
	//Extract the selected roof type.
	usRoofType = (UINT16)SelSingleNewRoof[ iCurBank ].uiObject;

	//now start building a linked list of all nodes visited -- start the first node.
	gpRoofList = (ROOFNODE*)MemAlloc( sizeof( ROOFNODE ) );
	Assert( gpRoofList );
	gpRoofList->iMapIndex = iMapIndex;
	gpRoofList->next = 0;
	RebuildRoofUsingFloorInfo( iMapIndex, usRoofType );

	//Use recursion to process the remainder.
	ReplaceRoof( iMapIndex - WORLD_COLS, usRoofType );
	ReplaceRoof( iMapIndex + WORLD_COLS, usRoofType );
	ReplaceRoof( iMapIndex - 1, usRoofType );
	ReplaceRoof( iMapIndex + 1, usRoofType );

	//Done, so delete the list.
	while( gpRoofList )
	{
		curr = gpRoofList;
		gpRoofList = gpRoofList->next;
		MemFree( curr );
	}
	gpRoofList = NULL;
}

//internal door editing vars.
INT32 iDoorMapIndex = 0;
enum{
	DOOR_BACKGROUND,
	DOOR_OKAY,
	DOOR_CANCEL,
	DOOR_LOCKED,
	NUM_DOOR_BUTTONS
};
INT32 iDoorButton[ NUM_DOOR_BUTTONS ];
MOUSE_REGION DoorRegion;
void DoorOkayCallback( GUI_BUTTON *btn, INT32 reason );
void DoorCancelCallback( GUI_BUTTON *btn, INT32 reason );
void DoorToggleLockedCallback( GUI_BUTTON *btn, INT32 reason );

extern BOOLEAN OpenableAtGridNo( INT32 iMapIndex );

void InitDoorEditing( INT32 iMapIndex )
{
	DOOR *pDoor;
	if( !DoorAtGridNo( iMapIndex ) && !OpenableAtGridNo( iMapIndex ) )
		return;
	gfEditingDoor = TRUE;
	iDoorMapIndex = iMapIndex;
	DisableEditorTaskbar();
	MSYS_DefineRegion( &DoorRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGH-2, 0, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	iDoorButton[ DOOR_BACKGROUND ] =
		CreateTextButton( 0, 0, 0, 0, BUTTON_USE_DEFAULT, iScreenWidthOffset + 200, iScreenHeightOffset + 130, 240, 100, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
		BUTTON_NO_CALLBACK, BUTTON_NO_CALLBACK );
	DisableButton( iDoorButton[ DOOR_BACKGROUND ] );
	SpecifyDisabledButtonStyle( iDoorButton[ DOOR_BACKGROUND ], DISABLED_STYLE_NONE );
	iDoorButton[ DOOR_OKAY ] =
		CreateTextButton(L"Okay", FONT12POINT1, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 330, iScreenHeightOffset + 195, 50, 30, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		DoorOkayCallback );
	iDoorButton[ DOOR_CANCEL ] =
		CreateTextButton(L"Cancel", FONT12POINT1, FONT_BLACK, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 385, iScreenHeightOffset + 195, 50, 30, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH, DEFAULT_MOVE_CALLBACK,
		DoorCancelCallback );
	InitTextInputModeWithScheme( DEFAULT_SCHEME );
	AddTextInputField( iScreenWidthOffset + 210, iScreenHeightOffset + 155, 25, 16, MSYS_PRIORITY_HIGH, L"0", 3, INPUTTYPE_NUMERICSTRICT );
	AddTextInputField( iScreenWidthOffset + 210, iScreenHeightOffset + 175, 25, 16, MSYS_PRIORITY_HIGH, L"0", 2, INPUTTYPE_NUMERICSTRICT );
	AddTextInputField( iScreenWidthOffset + 210, iScreenHeightOffset + 195, 25, 16, MSYS_PRIORITY_HIGH, L"0", 2, INPUTTYPE_NUMERICSTRICT );
	iDoorButton[ DOOR_LOCKED ] =
		CreateCheckBoxButton(	iScreenWidthOffset + 210, iScreenHeightOffset + 215, "EDITOR//SmCheckbox.sti", MSYS_PRIORITY_HIGH, DoorToggleLockedCallback );

	pDoor = FindDoorInfoAtGridNo( iDoorMapIndex );
	if( pDoor )
	{
		if( pDoor->fLocked )
		{
			ButtonList[ iDoorButton[ DOOR_LOCKED ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
		SetInputFieldStringWithNumericStrictValue( 0, pDoor->ubLockID );
		SetInputFieldStringWithNumericStrictValue( 1, pDoor->ubTrapID );
		SetInputFieldStringWithNumericStrictValue( 2, pDoor->ubTrapLevel );
	}
	else
	{
		ButtonList[ iDoorButton[ DOOR_LOCKED ] ]->uiFlags |= BUTTON_CLICKED_ON;
	}
}

void ExtractAndUpdateDoorInfo()
{
	LEVELNODE* pNode;
	INT32 num;
	DOOR door;
	BOOLEAN fCursor = FALSE;
	BOOLEAN fCursorExists = FALSE;

	memset( &door, 0, sizeof( DOOR ) );

	door.sGridNo = iDoorMapIndex;

	num = min( GetNumericStrictValueFromField( 0 ), NUM_LOCKS-1 );
	door.ubLockID = (UINT8)num;
	SetInputFieldStringWithNumericStrictValue( 0, num );
	if( num >= 0 )
		fCursor = TRUE;

	num = min( max( GetNumericStrictValueFromField( 1 ), 0 ), 10 );
	door.ubTrapID = (UINT8)num;
	SetInputFieldStringWithNumericStrictValue( 1, num );
	if( num )
		fCursor = TRUE;

	num = min( max( GetNumericStrictValueFromField( 2 ), 0 ), 20 );
	if( door.ubTrapID && !num )
		num = 1;	//Can't have a trap without a traplevel!
	door.ubTrapLevel = (UINT8)num;
	SetInputFieldStringWithNumericStrictValue( 2, num );
	if( num )
		fCursor = TRUE;

	if( ButtonList[ iDoorButton[ DOOR_LOCKED ] ]->uiFlags & BUTTON_CLICKED_ON )
	{
		door.fLocked = TRUE;
	}
	else
	{
		door.fLocked = FALSE;
	}

	//Find out if we have a rotating key cursor (we will either add one or remove one)
	pNode = gpWorldLevelData[ iDoorMapIndex ].pTopmostHead;
	while( pNode )
	{
		if( pNode->usIndex == ROTATINGKEY1 )
		{
			fCursorExists = TRUE;
			break;
		}
		pNode = pNode->pNext;
	}
	if( fCursor )
	{ //we have a valid door, so add it (or replace existing)
		if( !fCursorExists )
			AddTopmostToHead( iDoorMapIndex, ROTATINGKEY1 );
		//If the door already exists, the new information will replace it.
		AddDoorInfoToTable( &door );
	}
	else
	{ //if a door exists here, remove it.
		if( fCursorExists )
			RemoveAllTopmostsOfTypeRange( iDoorMapIndex, ROTATINGKEY, ROTATINGKEY );
		RemoveDoorInfoFromTable( iDoorMapIndex );
	}

}

void FindNextLockedDoor()
{
	DOOR *pDoor;
	INT32 i;
	for( i = iDoorMapIndex + 1; i < WORLD_MAX; i++ )
	{
		pDoor = FindDoorInfoAtGridNo( i );
		if( pDoor )
		{
			CenterScreenAtMapIndex( i );
			iDoorMapIndex = i;
			return;
		}
	}
	for( i = 0; i <= iDoorMapIndex; i++ )
	{
		pDoor = FindDoorInfoAtGridNo( i );
		if( pDoor )
		{
			CenterScreenAtMapIndex( i );
			iDoorMapIndex = i;
			return;
		}
	}
}

void RenderDoorEditingWindow()
{
	InvalidateRegion( iScreenWidthOffset + 200, iScreenHeightOffset + 130, iScreenWidthOffset + 440, iScreenHeightOffset + 230 );
	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_YELLOW );
	SetFontShadow( FONT_NEARBLACK );
	SetFontBackground( 0 );
	mprintf( iScreenWidthOffset + 210, iScreenHeightOffset + 140, iRenderDoorEditingWindowText[0], iDoorMapIndex );

	SetFontForeground( FONT_GRAY2 );
	mprintf( iScreenWidthOffset + 238, iScreenHeightOffset + 160, iRenderDoorEditingWindowText[1] );
	mprintf( iScreenWidthOffset + 238, iScreenHeightOffset + 180, iRenderDoorEditingWindowText[2] );
	mprintf( iScreenWidthOffset + 238, iScreenHeightOffset + 200, iRenderDoorEditingWindowText[3] );
	mprintf( iScreenWidthOffset + 238, iScreenHeightOffset + 218, iRenderDoorEditingWindowText[4] );
}

void KillDoorEditing()
{
	INT32 i;
	EnableEditorTaskbar();
	MSYS_RemoveRegion( &DoorRegion );
	for( i = 0; i < NUM_DOOR_BUTTONS; i++ )
		RemoveButton( iDoorButton[ i ] );
	gfEditingDoor = FALSE;
	KillTextInputMode();
}

void DoorOkayCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ExtractAndUpdateDoorInfo();
		KillDoorEditing();
	}
}

void DoorCancelCallback( GUI_BUTTON *btn, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		KillDoorEditing();
	}
}

void DoorToggleLockedCallback( GUI_BUTTON *btn, INT32 reason )
{
	//handled in ExtractAndUpdateDoorInfo();
}

void AddLockedDoorCursors()
{
	DOOR *pDoor;
	INT i;
	for( i = 0; i < gubNumDoors; i++ )
	{
		pDoor = &DoorTable[ i ];
		AddTopmostToHead( pDoor->sGridNo, ROTATINGKEY1 );
	}
}

void RemoveLockedDoorCursors()
{
	DOOR *pDoor;
	INT i;
	LEVELNODE* pNode;
	LEVELNODE* pTemp;
	for( i = 0; i < gubNumDoors; i++ )
	{
		pDoor = &DoorTable[ i ];
		pNode = gpWorldLevelData[ pDoor->sGridNo ].pTopmostHead;
		while( pNode )
		{
			if( pNode->usIndex == ROTATINGKEY1 )
			{
				pTemp = pNode;
				pNode = pNode->pNext;
				RemoveTopmost( pDoor->sGridNo, pTemp->usIndex );
			}
			else
				pNode = pNode->pNext;
		}
	}
}

void SetupTextInputForBuildings()
{
	CHAR16 str[4];
	InitTextInputModeWithScheme( DEFAULT_SCHEME );
	AddUserInputField( NULL );	//just so we can use short cut keys while not typing.
	swprintf( str, L"%d", gusMaxRoomNumber );
	AddTextInputField( iScreenWidthOffset + 410, 2 * iScreenHeightOffset + 400, 40, 15, MSYS_PRIORITY_NORMAL, str, 5, INPUTTYPE_NUMERICSTRICT );
}

void ExtractAndUpdateBuildingInfo()
{
	CHAR16 str[5];
	INT32 temp;
	//extract light1 colors
	temp = min( GetNumericStrictValueFromField( 1 ), 65535 );
	if( temp != -1 )
	{
		gusCurrRoomNumber = (UINT16)temp;
	}
	else
	{
		gusCurrRoomNumber = 0;
	}
	swprintf( str, L"%d", gusCurrRoomNumber );
	SetInputFieldStringWith16BitString( 1, str );
	SetActiveField( 0 );
}

#endif
