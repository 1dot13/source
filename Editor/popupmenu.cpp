// WANNE: EDITOR: todo

//FUNCTIONALITY:
//This popup menu functions IDENTICALLY to Windows95's methodology.  You can use the arrow keys, or the
//mouse in two methods.  The first method is where you release the mouse button before
//moving the cursor inside the popup menu, which makes it persistant (it won't go away until you click again),
//or non-persistant, where you release the button inside the menu after your initial click (non-persistant).
//IMPORTANT NOTE:
//Linda Currie, the main user of the editor dislikes the non-persistant functionality, however, it is still
//supported.  Just remove the commented line of code (search for UNCOMMENT), and it's fixed -- it is
//currently disabled.

#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include "tiledef.h"
	#include "sysutil.h"
	#include "font.h"
	#include "Font Control.h"
	#include "line.h"
	#include "worlddat.h"
	#include "selectwin.h"
	#include "popupmenu.h"
	#include "EditorDefines.h"
	#include "render dirty.h"
	#include "debug.h"
	#include "mousesystem.h"
	#include "cursors.h"
	#include "overhead types.h"
	#include "EditorMercs.h"
	#include "Scheduling.h"
	#include "english.h"
	#include "Item Statistics.h"
#endif

CurrentPopupMenuInformation gPopupData;

MOUSE_REGION popupRegion;

UINT16 gusEntryHeight;
BOOLEAN fWaitingForLButtonRelease = FALSE;

extern CHAR16 gszScheduleActions[ NUM_SCHEDULE_ACTIONS ][20];

//Finds the string for any popup menu in JA2 -- the strings are stored
//in different ways in each instance.
STR16 GetPopupMenuString( UINT8 ubIndex )
{
	switch( gPopupData.ubPopupMenuID )
	{
		case CHANGETSET_POPUP: //tile sets
			return gTilesets[ ubIndex ].zName;
		case OWNERSHIPGROUP_POPUP: //civilian groups
		case CHANGECIVGROUP_POPUP: //civilian groups
			return gszCivGroupNames[ ubIndex ];
		case SCHEDULEACTION_POPUP: //setting scheduling actions
			return gszScheduleActions[ ubIndex ];
		case ACTIONITEM_POPUP:
			return gszActionItemDesc[ ubIndex ];
		default:
			return 0;
	}
}
/*
InitPopUpMenu
---------------
Creates the popup menu, then takes control of the i/o
effectively turning the popup into a modal system (forcing the
user to make a selection or cancel before doing anything else.
There are two modes of operation as soon as the user presses the mouse
on a popup menu activation button.
	1)  The user releases the mouse before moving the cursor onto
			the popup region.  The popup menu will end as soon as the
			user presses the mouse button.  If the cursor is out of the
			popup region, then it returns 0.
	2)  The user holds the mouse while moving the cursor onto the
			popup region.  The popup menu will end as soon as the user
			releases the mouse button.  If the cursor is out of the popup
			region, then it returns 0.


Returns 0 if nothing in menu was selected, else
returns the menu entry number starting from 1.
*/

void InitPopupMenu( INT32 iButtonID, UINT8 ubPopupMenuID, UINT8 ubDirection )
{
	UINT16 usX, usY;
	UINT16 usMenuHeight;
	UINT16 usMenuWidth = 0;
	UINT16 usCurrStrWidth;
	UINT8 ubColumn, ubEntry;
	UINT8 ubCounter;
	GUI_BUTTON *button;
	//calculate the location of the menu based on the button position.
	//This also calculates the menu's direction based on position.

	gPopupData.usFont = (UINT16)SMALLFONT1;
	gusEntryHeight = GetFontHeight( gPopupData.usFont );

	button = ButtonList[ iButtonID ];
	MSYS_DisableRegion( &gBottomPanalRegion );

	switch( ubDirection )
	{
		case DIR_UPRIGHT:
			usX = button->Area.RegionTopLeftX;
			usY = button->Area.RegionTopLeftY;
			break;
		case DIR_UPLEFT:
			usX = button->Area.RegionBottomRightX;
			usY = button->Area.RegionTopLeftY;
			break;
		case DIR_DOWNRIGHT:
			usX = button->Area.RegionTopLeftX;
			usY = button->Area.RegionBottomRightY;
			break;
		case DIR_DOWNLEFT:
			usX = button->Area.RegionBottomRightX;
			usY = button->Area.RegionBottomRightY;
			break;
		default:
			AssertMsg(0, "Invalid direction given");
			return;
	}

	//Decipher the popupMenuID
	switch( ubPopupMenuID )
	{
		case CHANGETSET_POPUP:	//change tileset
			gPopupData.ubNumEntries = gubNumSets;
			break;
		case OWNERSHIPGROUP_POPUP:
		case CHANGECIVGROUP_POPUP:
			gPopupData.ubNumEntries = NUM_CIV_GROUPS;
			break;
		case SCHEDULEACTION_POPUP:
			gPopupData.ubNumEntries = NUM_SCHEDULE_ACTIONS;
			break;
		case ACTIONITEM_POPUP:
			gPopupData.ubNumEntries = NUM_ACTIONITEMS;
			break;
		default:
			return;
	}

	gPopupData.usFont = (UINT16)SMALLFONT1;
	gusEntryHeight = GetFontHeight( gPopupData.usFont );

	button = ButtonList[ iButtonID ];
	MSYS_DisableRegion( &gBottomPanalRegion );

	gPopupData.ubPopupMenuID = ubPopupMenuID;
	gPopupData.ubSelectedIndex = 0;
	gPopupData.ubActiveType = POPUP_ACTIVETYPE_NOT_YET_DETERMINED;
	gPopupData.fActive = TRUE;
	fWaitingForLButtonRelease = FALSE;
	gPopupData.fUseKeyboardInfoUntilMouseMoves = FALSE;
	//Initialize the last mouse position to be out of bounds.
	gPopupData.usLastMouseX = 1000;
	gPopupData.usLastMouseY = 1000;
	//clear the column widths.
	for( ubColumn = 0; ubColumn < MAX_COLUMNS; ubColumn++ )
		gPopupData.ubColumnWidth[ ubColumn ] = 0;

	//1) Calc total entry height of the popup region.
	gPopupData.ubColumns = 1;
	gPopupData.ubMaxEntriesPerColumn = gPopupData.ubNumEntries;
	usMenuHeight = gPopupData.ubNumEntries * gusEntryHeight + 3;
	while( usMenuHeight >= usY && ( ubDirection == DIR_UPLEFT || ubDirection == DIR_UPRIGHT ) ||
				 480-usMenuHeight >= usY && ( ubDirection == DIR_DOWNLEFT || ubDirection == DIR_DOWNRIGHT ) )
	{ //menu has too many entries.  Increase the number of columns until the height is
		//less than the max height.
		gPopupData.ubMaxEntriesPerColumn = (gPopupData.ubNumEntries+gPopupData.ubColumns)/(gPopupData.ubColumns+1);
		usMenuHeight = gPopupData.ubMaxEntriesPerColumn * gusEntryHeight + 3;
		gPopupData.ubColumns++;
	}
	//now we have the number of columns as well as the max number of entries per column, and
	//the total menu height.

	//We now calculate the total width of the menu as well as the max width of each column.
	ubCounter = 0;
	usMenuWidth = 0;
	for( ubColumn = 0; ubColumn < gPopupData.ubColumns; ubColumn++ )
	{
		for( ubEntry = 0; ubEntry < gPopupData.ubMaxEntriesPerColumn; ubEntry++ )
		{
			if( ubCounter >= gPopupData.ubNumEntries )
				break; //done (don't want to process undefined entries...)
			usCurrStrWidth = 16 + StringPixLength( GetPopupMenuString( ubCounter ) , gPopupData.usFont );
			if( usCurrStrWidth > gPopupData.ubColumnWidth[ ubColumn ] )
			{
				gPopupData.ubColumnWidth[ ubColumn ] = (UINT8)usCurrStrWidth;
			}
			ubCounter++;
		}
		usMenuWidth += gPopupData.ubColumnWidth[ ubColumn ];
	}

	//Calculate popup menu boundaries based on direction from argument point.
	switch( ubDirection )
	{
		case DIR_UPRIGHT:
			gPopupData.usLeft = usX;
			gPopupData.usTop = usY - usMenuHeight - 1;
			gPopupData.usRight = usX + usMenuWidth;
			gPopupData.usBottom = usY - 1;
			break;
		case DIR_UPLEFT:
			gPopupData.usLeft = usX - usMenuWidth;
			gPopupData.usTop = usY - usMenuHeight - 1;
			gPopupData.usRight = usX;
			gPopupData.usBottom = usY - 1;
			break;
		case DIR_DOWNRIGHT:
			gPopupData.usLeft = usX;
			gPopupData.usTop = usY + 1;
			gPopupData.usRight = usX + usMenuWidth;
			gPopupData.usBottom = usY + usMenuHeight + 1;
			break;
		case DIR_DOWNLEFT:
			gPopupData.usLeft = usX - usMenuWidth;
			gPopupData.usTop = usY + 1;
			gPopupData.usRight = usX;
			gPopupData.usBottom = usY + usMenuHeight + 1;
			break;
	}
	MSYS_DefineRegion(&popupRegion, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST, CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK);//dnl ch34 090909
	RenderPopupMenu();
}

void RenderPopupMenu()
{
	UINT16 usX, usY;
	UINT8 ubColumn, ubEntry, ubCounter;
	UINT8 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	UINT16 usLineColor;
	UINT16 usStringWidth;
	UINT16 usStart;

	//Draw the menu
	ColorFillVideoSurfaceArea(FRAME_BUFFER,
		gPopupData.usLeft, gPopupData.usTop, gPopupData.usRight, gPopupData.usBottom,
		Get16BPPColor(FROMRGB(128, 128, 128) ) );
	pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, 0, 640, 480 );
	usLineColor = Get16BPPColor( FROMRGB( 64, 64, 64 ) );
	RectangleDraw( TRUE, gPopupData.usLeft, gPopupData.usTop, gPopupData.usRight, gPopupData.usBottom,
		usLineColor, pDestBuf );
	if( gPopupData.ubColumns > 1 )
	{ //draw a vertical line between each column
		usStart = gPopupData.usLeft + gPopupData.ubColumnWidth[ 0 ];
		for( ubColumn = 1; ubColumn < gPopupData.ubColumns; ubColumn++ )
		{
			LineDraw( TRUE, usStart, gPopupData.usTop, usStart, gPopupData.usBottom, usLineColor, pDestBuf );
		}
		usStart += (UINT16)gPopupData.ubColumnWidth[ ubColumn ];
	}
	UnLockVideoSurface( FRAME_BUFFER );

	//Set up the text attributes.
	SetFont( gPopupData.usFont);
	SetFontBackground( FONT_MCOLOR_BLACK );
	SetFontForeground( FONT_MCOLOR_WHITE );

	usX = gPopupData.usLeft + 1;
	ubCounter = 0;
	usStart = gPopupData.usLeft;
	for( ubColumn = 0; ubColumn < gPopupData.ubColumns; ubColumn++ )
	{
		for( ubEntry = 0; ubEntry < gPopupData.ubMaxEntriesPerColumn; ubEntry++ )
		{
			if( ubCounter >= gPopupData.ubNumEntries )
				return; //done
			//Calc current string's width in pixels.  Adding 14 pixels which is the width of
			//two padded gPopupData.usFont spaces not stored in the string.
			usStringWidth = 14 + StringPixLength( GetPopupMenuString( ubCounter ), gPopupData.usFont );
			//Horizontally center the string inside the popup menu
			usX = usStart + ( gPopupData.ubColumnWidth[ ubColumn ] - usStringWidth ) / 2;
			usY = gPopupData.usTop + 1 + ubEntry * gusEntryHeight;
			if( ubCounter == gPopupData.ubSelectedIndex - 1 )
			{
				//This is the highlighted menu entry.
				SetFontForeground( FONT_MCOLOR_LTBLUE );
				mprintf( usX, usY, L" %s ", GetPopupMenuString( ubCounter ) );
				SetFontForeground( FONT_MCOLOR_WHITE );
			}
			else
			{
				mprintf( usX, usY, L" %s ", GetPopupMenuString( ubCounter ) );
			}
			ubCounter++;
		}
		usStart += gPopupData.ubColumnWidth[ ubColumn ];
	}
}

//This private function of PopupMenuHandle determines which menu entry
//is highlighted based on the mouse cursor position.  Returns 0 if the
//mouse is out of the menu region.
UINT8 GetPopupIndexFromMousePosition()
{
	UINT8 ubNumEntriesDown;
	UINT16 usRelX;
	UINT8 ubCount;
	if( gusMouseXPos >= gPopupData.usLeft
		&& gusMouseXPos <= gPopupData.usRight
		&& gusMouseYPos > gPopupData.usTop						//one pixel gap on top ignored
		&& gusMouseYPos < gPopupData.usBottom - 2 )		//two pixel gap on bottom ignored
	{
		//subtract the top y coord of the popup region from the mouse's yPos as well
		//as an extra pixel at the top of the region which is ignored in menu selection,
		//divide this number by the height of a menu entry, then add one.  This will
		//return the menu index from 1 (at the top) to n (at the bottom).
		ubNumEntriesDown = (gusMouseYPos - gPopupData.usTop - 1) / gusEntryHeight + 1;
		usRelX = gusMouseXPos - gPopupData.usLeft;
		ubCount=0;
		while( usRelX > gPopupData.ubColumnWidth[ ubCount ] )
		{
			usRelX -= gPopupData.ubColumnWidth[ ubCount ];
			ubCount++;
			ubNumEntriesDown += gPopupData.ubMaxEntriesPerColumn;
		}
		if( ubNumEntriesDown >= gPopupData.ubNumEntries )
			ubNumEntriesDown = gPopupData.ubNumEntries;
		return ubNumEntriesDown;
	}
	return 0; //mouse not in valid region.
}

void PopupMenuHandle()
{
	InputAtom InputEvent;

	if( gPopupData.ubActiveType == POPUP_ACTIVETYPE_NOT_YET_DETERMINED )
	{
		//Attempt to determine if the menu will be persistant or not.
		//Determination is made when the mouse's left button is released or if
		//the mouse cursor enters the menu region.
		if( gusMouseXPos >= gPopupData.usLeft
			&& gusMouseXPos <= gPopupData.usRight
			&& gusMouseYPos > gPopupData.usTop						//one pixel gap on top ignored
			&& gusMouseYPos < gPopupData.usBottom - 1 )		//two pixel gap on bottom ignored
		{
			//mouse cursor has just entered the menu region -- nonpersistant.

			//KRIS:  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//Disabled this because Linda doesn't like it... (I like it though, and it works)
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//UNCOMMENT IF NONPERSISTANT IS ALLOWED
			//gPopupData.ubActiveType = POPUP_ACTIVETYPE_NONPERSISTANT;

			return;
		}
		else if( !gfLeftButtonState )
		{	//left button has been released before entering region -- persistant
			gPopupData.ubActiveType = POPUP_ACTIVETYPE_PERSISTANT;
			return;
		}
	}
	if( !gPopupData.fUseKeyboardInfoUntilMouseMoves )
	{
		//check menu entry based on mouse position
		gPopupData.ubSelectedIndex = GetPopupIndexFromMousePosition();
	}
	else if( gusMouseXPos != gPopupData.usLastMouseX || gusMouseYPos != gPopupData.usLastMouseY )
	{
		//The keyboard determined the last entry, but the mouse has moved,
		//so use the mouse to determine the new entry.
		gPopupData.fUseKeyboardInfoUntilMouseMoves = FALSE;
		gPopupData.ubSelectedIndex = GetPopupIndexFromMousePosition();
	}
	//Check terminating conditions for persistant states.
	if( gfLeftButtonState && gPopupData.ubActiveType == POPUP_ACTIVETYPE_PERSISTANT )
		fWaitingForLButtonRelease = TRUE;
	if( gfLeftButtonState && gPopupData.ubActiveType == POPUP_ACTIVETYPE_PERSISTANT
		|| !gfLeftButtonState && gPopupData.ubActiveType == POPUP_ACTIVETYPE_NONPERSISTANT )
	{
		//Selection conditions via mouse have been met whether the mouse is in the
		//menu region or not.
		gPopupData.ubSelectedIndex = GetPopupIndexFromMousePosition();
		if( gPopupData.ubSelectedIndex )
		{
			ProcessPopupMenuSelection();
		}
		gPopupData.fActive = FALSE;
		MSYS_RemoveRegion( &popupRegion );
		gfRenderWorld = TRUE;
		gfRenderTaskbar = TRUE;
		return;
	}
	//Use keyboard input as well.

	while (DequeueSpecificEvent(&InputEvent, KEY_DOWN|KEY_UP|KEY_REPEAT))
	{
		switch(InputEvent.usEvent)
		{
			case KEY_DOWN:
				switch( InputEvent.usParam )
				{
					case DNARROW:
						gPopupData.fUseKeyboardInfoUntilMouseMoves = TRUE;
						gPopupData.usLastMouseX = gusMouseXPos;
						gPopupData.usLastMouseY = gusMouseYPos;
						gPopupData.ubSelectedIndex++;
						if( gPopupData.ubSelectedIndex > gPopupData.ubNumEntries )
						{
							gPopupData.ubSelectedIndex = 1;
						}
						break;
					case UPARROW:
						gPopupData.fUseKeyboardInfoUntilMouseMoves = TRUE;
						gPopupData.usLastMouseX = gusMouseXPos;
						gPopupData.usLastMouseY = gusMouseYPos;
						if( gPopupData.ubSelectedIndex < 2 )
						{
							gPopupData.ubSelectedIndex = gPopupData.ubNumEntries;
						}
						else
						{
							gPopupData.ubSelectedIndex--;
						}
						break;
					case ESC:
						gPopupData.fActive = FALSE;
						MSYS_RemoveRegion( &popupRegion );
						gfRenderWorld = TRUE;
						gfRenderTaskbar = TRUE;
						break;
					case ENTER:
						ProcessPopupMenuSelection();
						gPopupData.fActive = FALSE;
						MSYS_RemoveRegion( &popupRegion );
						gfRenderWorld = TRUE;
						gfRenderTaskbar = TRUE;
						break;
				}
				break;
		}
  }
}

void ProcessPopupMenuSelection()
{
	switch( gPopupData.ubPopupMenuID )
	{
		case CHANGETSET_POPUP:
			//change the tileset here.
			ReloadTileset( (UINT8)(gPopupData.ubSelectedIndex - 1 ) );
			InitJA2SelectionWindow( );
			break;
		case CHANGECIVGROUP_POPUP:
			ChangeCivGroup( (UINT8)(gPopupData.ubSelectedIndex - 1) );
			break;
		case SCHEDULEACTION_POPUP:
			UpdateScheduleAction( (UINT8)(gPopupData.ubSelectedIndex - 1) );
			break;
		case ACTIONITEM_POPUP:
			UpdateActionItem( (UINT8)(gPopupData.ubSelectedIndex - 1) );
			break;
		case OWNERSHIPGROUP_POPUP:
			SetOwnershipGroup( (UINT8)(gPopupData.ubSelectedIndex - 1) );
			break;
	}
}

BOOLEAN ProcessPopupMenuIfActive( )
{
	if( !gPopupData.fActive && !fWaitingForLButtonRelease )
		return FALSE;
	if( fWaitingForLButtonRelease )
	{
		if( !gfLeftButtonState )
		{
			fWaitingForLButtonRelease = FALSE;
			return FALSE;
		}
		return TRUE;
	}
	PopupMenuHandle();
	RenderPopupMenu();
	InvalidateRegion( gPopupData.usLeft, gPopupData.usTop, gPopupData.usRight, gPopupData.usBottom );
	ExecuteBaseDirtyRectQueue();
	EndFrameBufferRender( );
	return TRUE;
}

#endif







