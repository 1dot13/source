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
	#include "interface items.h"
	#include "text.h"
	#include "utilities.h"
	#include "Action Items.h"
	#include "World Items.h"
	#include "EditorDefines.h"
	#include "EditorItems.h"
	#include "EditorMercs.h"
	#include "weapons.h"
	#include "Editor Taskbar Utils.h"
	#include "wordwrap.h"
	#include "Item Statistics.h"
	#include "Simple Render Utils.h"
	#include "worldman.h"
	#include "random.h"
	#include "Pits.h"
	#include "keys.h"
	#include "InterfaceItemImages.h"
	#include "Editor Undo.h"//dnl ch86 220214
#endif

#include <vfs/Tools/vfs_log.h>

#define NUMBER_TRIGGERS			27
#define PRESSURE_ACTION_ID	(NUMBER_TRIGGERS - 1)

extern ITEM_POOL		*gpEditingItemPool;

//Simply counts the number of items in the world.	This is used for display purposes.
UINT16 CountNumberOfEditorPlacementsInWorld( UINT16 usEInfoIndex, UINT16 *pusQuantity ); //wrapper for the next three
UINT16 CountNumberOfItemPlacementsInWorld( UINT16 usItem, UINT16 *pusQuantity );
UINT16 CountNumberOfItemsWithFrequency( UINT16 usItem, INT8 bFrequency );
UINT16 CountNumberOfPressureActionsInWorld();
UINT16 CountNumberOfKeysOfTypeInWorld( UINT8 ubKeyID );

//Finds and selects the next item when right clicking on an item type.	Only works if the
//item actually exists in the world.
void FindNextItemOfSelectedType(); //wrapper for the next four
void SelectNextTriggerWithFrequency( UINT16 usItem, INT8 bFrequency );
void SelectNextItemOfType( UINT16 usItem );
void SelectNextPressureAction();
void SelectNextKeyOfType( UINT8 ubKeyID );

INT32 giDefaultExistChance = 100;

typedef struct IPListNode
{
	INT32 sGridNo;
	struct IPListNode *next;
}IPListNode;

IPListNode *pIPHead = NULL;

IPListNode *gpCurrItemPoolNode = NULL;
ITEM_POOL *gpItemPool = NULL;

void BuildItemPoolList()
{
	ITEM_POOL *temp;
	IPListNode *tail = NULL;
	INT32 i;
	KillItemPoolList();
	for( i = 0; i < WORLD_MAX; i++ )
	{
		if( GetItemPoolFromGround( i, &temp ) )
		{
			if( !pIPHead )
			{
				pIPHead = (IPListNode*)MemAlloc( sizeof( IPListNode ) );
				Assert( pIPHead );
				tail = pIPHead;
			}
			else
			{
				tail->next = (IPListNode*)MemAlloc( sizeof( IPListNode ) );
				Assert( tail->next );
				tail = tail->next;
			}
			ShowItemCursor( i );
			tail->sGridNo = i;
			tail->next = NULL;
		}
	}
	gpCurrItemPoolNode = pIPHead;
	SpecifyItemToEdit( NULL, -1 );
}

void KillItemPoolList()
{
	IPListNode *pIPCurr;
	pIPCurr = pIPHead;
	while( pIPCurr )
	{
		if(pIPCurr->sGridNo < WORLD_MAX)//dnl ch43 280909 Prevent CTD when world size is changed
			HideItemCursor(pIPCurr->sGridNo);
		pIPHead = pIPHead->next;
		MemFree( pIPCurr );
		pIPCurr = pIPHead;
	}
	pIPHead = NULL;
}

//Contains global information about the editor items
//May be expanded to encapsulate the entire editor later.
EditorItemsInfo eInfo;

//Does some precalculations regarding the number of each item type, so that it
//isn't calculated every time a player changes categories.
void EntryInitEditorItemsInfo()
{
	INT32 i;
	INVTYPE *item;
	eInfo.uiBuffer = 0;
	eInfo.fKill = 0;
	eInfo.fActive = 0;
	eInfo.sWidth = 0;
	eInfo.sHeight = 0;
	eInfo.sScrollIndex = 0;
	eInfo.sSelItemIndex = 0;
	eInfo.sHilitedItemIndex = -1;
	eInfo.sNumItems = 0;
	eInfo.pusItemIndex = NULL;
	if( eInfo.fGameInit )
	{ //This only gets called one time in game execution.
		memset( &eInfo, 0, sizeof( EditorItemsInfo ) );
		eInfo.sHilitedItemIndex = -1;
		eInfo.uiItemType = TBAR_MODE_ITEM_WEAPONS;
		//Pre-calculate the number of each item type.
		eInfo.sNumTriggers = NUMBER_TRIGGERS;
		for( i=0; i < MAXITEMS; i++ )
		{
			if ( Item[i].usItemClass	== 0 )
				break;
			item = &Item[i];
			//if( Item[i].fFlags & ITEM_NOT_EDITOR )
			//	continue;
			if(item->notineditor)
				continue;
			if( i == SWITCH || i == ACTION_ITEM )
			{

			}
			else switch( item->usItemClass )
			{
				case IC_GUN:
				case IC_BLADE:
				case IC_THROWN:
				case IC_LAUNCHER:
				case IC_THROWING_KNIFE:
					eInfo.sNumWeapons++;
					break;
				case IC_PUNCH:
					if ( i != NOTHING )
					{
						eInfo.sNumWeapons++;
					}
					break;
				case IC_AMMO:
					eInfo.sNumAmmo++;
					break;
				case IC_ARMOUR:
					eInfo.sNumArmour++;
					break;
				case IC_LBEGEAR:
					eInfo.sLBEGear++;
					break;
				case IC_GRENADE:
				case IC_BOMB:
					eInfo.sNumExplosives++;
					break;
				case IC_MEDKIT:
				case IC_KIT:
				case IC_FACE:
				case IC_MONEY:
				case IC_BELTCLIP:
					eInfo.sNumEquipment1++;
					break;
				case IC_MISC:
					if( eInfo.sNumEquipment2 < eInfo.sNumEquipment3 )
						eInfo.sNumEquipment2++;
					else
						eInfo.sNumEquipment3++;
					break;
				case IC_RANDOMITEM:
					eInfo.sNumRandomItems++;
					break;
				//case IC_KEY:
				//	eInfo.sNumKeys++;
				//	break;
			}
		}
		eInfo.sNumKeys = NUM_KEYS;
	}
}

void InitEditorItemsInfo(UINT32 uiItemType)
{
	VSURFACE_DESC		vs_desc;
	UINT8	*pDestBuf, *pSrcBuf;
	UINT32 uiSrcPitchBYTES, uiDestPitchBYTES;
	INVTYPE *item;
	SGPRect	SaveRect, NewRect;
	HVOBJECT hVObject;
	UINT32 uiVideoObjectIndex;
	UINT16 usUselessWidth, usUselessHeight;
	INT32 sWidth, sOffset, sStart;
	INT32 i, x, y;
	UINT16 usCounter;
	CHAR16 pStr[ 100 ];//, pStr2[ 100 ];
	CHAR16 pItemName[SIZE_ITEM_NAME];
	UINT8 ubBitDepth;
	BOOLEAN fTypeMatch;
	INT32 iEquipCount = 0;

	// Check to make sure that there isn't already a valid eInfo
	if( eInfo.fActive )
	{
		if( eInfo.uiItemType == uiItemType )
		{	//User clicked on the same item classification -- ignore
			return;
		}
		else
		{	//User selected a different item classification -- delete it first.
			ClearEditorItemsInfo();
			ClearTaskbarRegion( iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 360, iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 440 );
		}
	}
	else
	{
		//Clear the menu area, so that the buffer doesn't get corrupted.
		ClearTaskbarRegion( iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 360, iScreenWidthOffset + 480, 2 * iScreenHeightOffset + 440 );
	}
	EnableEditorRegion( ITEM_REGION_ID );

	eInfo.uiItemType = uiItemType;
	eInfo.fActive = TRUE;
	//Begin initialization of data.
	switch(uiItemType)
	{
		case TBAR_MODE_ITEM_WEAPONS:
			eInfo.sNumItems = eInfo.sNumWeapons;
			eInfo.sScrollIndex = eInfo.sSaveWeaponsScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelWeaponsIndex;
			break;
		case TBAR_MODE_ITEM_AMMO:
			eInfo.sNumItems = eInfo.sNumAmmo;
			eInfo.sScrollIndex = eInfo.sSaveAmmoScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelAmmoIndex;
			break;
		case TBAR_MODE_ITEM_ARMOUR:
			eInfo.sNumItems = eInfo.sNumArmour;
			eInfo.sScrollIndex = eInfo.sSaveArmourScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelArmourIndex;
			break;
		case TBAR_MODE_ITEM_LBEGEAR:
			eInfo.sNumItems = eInfo.sLBEGear;
			eInfo.sScrollIndex = eInfo.sSaveLBEScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelLBEIndex;
			break;
		case TBAR_MODE_ITEM_EXPLOSIVES:
			eInfo.sNumItems = eInfo.sNumExplosives;
			eInfo.sScrollIndex = eInfo.sSaveExplosivesScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelExplosivesIndex;
			break;
		case TBAR_MODE_ITEM_EQUIPMENT1:
			eInfo.sNumItems = eInfo.sNumEquipment1;
			eInfo.sScrollIndex = eInfo.sSaveEquipment1ScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelEquipment1Index;
			break;
		case TBAR_MODE_ITEM_EQUIPMENT2:
			eInfo.sNumItems = eInfo.sNumEquipment2;
			eInfo.sScrollIndex = eInfo.sSaveEquipment2ScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelEquipment2Index;
			break;
		case TBAR_MODE_ITEM_EQUIPMENT3:
			eInfo.sNumItems = eInfo.sNumEquipment3;
			eInfo.sScrollIndex = eInfo.sSaveEquipment3ScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelEquipment3Index;
			break;
		case TBAR_MODE_ITEM_TRIGGERS:
			eInfo.sNumItems = eInfo.sNumTriggers;
			eInfo.sScrollIndex = eInfo.sSaveTriggersScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelTriggersIndex;
			break;
		case TBAR_MODE_ITEM_KEYS:
			eInfo.sNumItems = eInfo.sNumKeys;
			eInfo.sScrollIndex = eInfo.sSaveKeysScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelKeysIndex;
			break;
		case TBAR_MODE_ITEM_RANDOMITEM:
			eInfo.sNumItems = eInfo.sNumRandomItems;
			eInfo.sScrollIndex = eInfo.sSaveRandomItemScrollIndex;
			eInfo.sSelItemIndex = eInfo.sSaveSelRandomItemIndex;
			break;
		default:
			//error
			return;
	}
	//Allocate memory to store all the item pointers.
	if(eInfo.sNumItems)//dnl ch78 271113
		eInfo.pusItemIndex = (UINT16*)MemAlloc( sizeof(UINT16) * eInfo.sNumItems );
	//Disable the appropriate scroll buttons based on the saved scroll index if applicable
	//Left most scroll position
	DetermineItemsScrolling();
	//calculate the width of the buffer based on the number of items.
	//every pair of items (odd rounded up) requires 60 pixels for width.
	//the minimum buffer size is 420.	Height is always 80 pixels.

	eInfo.sWidth = (eInfo.sNumItems > 12) ? (((INT32) eInfo.sNumItems+1)/2)*60 : 360;//dnl ch77 251113 was (SCREEN_HEIGHT - 120) but editor crash if resolution is 1024x768
	eInfo.sHeight = 80;
	// Create item buffer
	GetCurrentVideoSettings( &usUselessWidth, &usUselessHeight, &ubBitDepth );
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = eInfo.sWidth;
	vs_desc.usHeight = eInfo.sHeight;
	vs_desc.ubBitDepth = ubBitDepth;

	//!!!Memory check.	Create the item buffer
	if(!AddVideoSurface( &vs_desc, &eInfo.uiBuffer ))
	{
		eInfo.fKill = TRUE;
		eInfo.fActive = FALSE;
		return;
	}

	pDestBuf = LockVideoSurface(eInfo.uiBuffer, &uiDestPitchBYTES);
	pSrcBuf = LockVideoSurface(FRAME_BUFFER, &uiSrcPitchBYTES);

	//copy a blank chunk of the editor interface to the new buffer.
	for( i=0; i<eInfo.sWidth; i+=60 )
	{
		Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
			(UINT16 *)pSrcBuf, uiSrcPitchBYTES, 0+i, 0, iScreenWidthOffset + 100, 2 * iScreenHeightOffset + 360, 60, 80 );
	}

	UnLockVideoSurface(eInfo.uiBuffer);
	UnLockVideoSurface(FRAME_BUFFER);

	x = 0;
	y = 0;
	usCounter = 0;
	NewRect.iTop = 0;
	NewRect.iBottom = eInfo.sHeight;
	NewRect.iLeft = 0;
	NewRect.iRight = eInfo.sWidth;
	GetClippingRect(&SaveRect);
	SetClippingRect(&NewRect);
	if( eInfo.uiItemType == TBAR_MODE_ITEM_KEYS )
	{ //Keys use a totally different method for determining
		for( i = 0; i < eInfo.sNumItems; i++ )
		{
			item = &Item[ KeyTable[ 0 ].usItem + LockTable[ i ].usKeyItem ];
			uiVideoObjectIndex = GetInterfaceGraphicForItem( item );
			GetVideoObject( &hVObject, uiVideoObjectIndex );

			//Store these item pointers for later when rendering selected items.
			eInfo.pusItemIndex[i] = KeyTable[ 0 ].usItem + LockTable[ i ].usKeyItem;

			SetFont(SMALLCOMPFONT);
			SetFontForeground( FONT_MCOLOR_WHITE );
			SetFontDestBuffer( eInfo.uiBuffer, 0, 0, eInfo.sWidth, eInfo.sHeight, FALSE );

			swprintf( pStr, L"%S", LockTable[ i ].ubEditorName );
			DisplayWrappedString(x, (UINT16)(y+25), 60, 2, SMALLCOMPFONT, FONT_WHITE,	pStr, FONT_BLACK, TRUE, CENTER_JUSTIFIED );

			UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : item->ubGraphicNum;
			//Calculate the center position of the graphic in a 60 pixel wide area.
			sWidth = hVObject->pETRLEObject[usGraphicNum].usWidth;
			sOffset = hVObject->pETRLEObject[usGraphicNum].sOffsetX;
			sStart = x + (60 - sWidth - sOffset*2) / 2;

			BltVideoObjectOutlineFromIndex( eInfo.uiBuffer, uiVideoObjectIndex, usGraphicNum, sStart, y+2, 0, FALSE );

			//cycle through the various slot positions (0,0), (0,40), (60,0), (60,40), (120,0)...
			if( y == 0 )
			{
				y = 40;
			}
			else
			{
				y = 0;
				x += 60;
			}
		}
	}
	else for( i = 0; i < eInfo.sNumItems; i++ )
	{

		fTypeMatch = FALSE;
		while( usCounter<MAXITEMS && !fTypeMatch )
		{
			if ( Item[usCounter].usItemClass	== 0 )
				break;
			item = &Item[usCounter];
			//if( Item[usCounter].fFlags & ITEM_NOT_EDITOR )
			if(item->notineditor)
			{
				usCounter++;
				continue;
			}
			if( eInfo.uiItemType == TBAR_MODE_ITEM_TRIGGERS )
			{
				if( i < PRESSURE_ACTION_ID )
					usCounter = ( i % 2 ) ? ACTION_ITEM : SWITCH;
				else
					usCounter = ACTION_ITEM;
				fTypeMatch = TRUE;
				item = &Item[usCounter];
			}
			else switch( item->usItemClass )
			{
				case IC_GUN:
				case IC_BLADE:
				case IC_LAUNCHER:
				case IC_THROWN:
				case IC_THROWING_KNIFE:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_WEAPONS;
					break;
				case IC_PUNCH:
					if ( i != NOTHING )
					{
						fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_WEAPONS;
					}
					else
					{
						fTypeMatch = FALSE;
					}
					break;
				case IC_AMMO:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_AMMO;
					break;
				case IC_ARMOUR:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_ARMOUR;
					break;
				case IC_LBEGEAR:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_LBEGEAR;
					break;
				case IC_GRENADE:
				case IC_BOMB:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_EXPLOSIVES;
					break;
				case IC_MEDKIT:
				case IC_KIT:
				case IC_FACE:
				case IC_MONEY:
				case IC_BELTCLIP:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_EQUIPMENT1;
					break;
				case IC_MISC:
					if( usCounter == ACTION_ITEM || usCounter == SWITCH )
						break;
					if( iEquipCount < eInfo.sNumEquipment2 )
						fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_EQUIPMENT2;
					else
						fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_EQUIPMENT3;
					iEquipCount++;
					break;
				case IC_RANDOMITEM:
					fTypeMatch = eInfo.uiItemType == TBAR_MODE_ITEM_RANDOMITEM;
					break;
			}
			if( fTypeMatch )
			{

				uiVideoObjectIndex = GetInterfaceGraphicForItem( item );
				GetVideoObject( &hVObject, uiVideoObjectIndex );

				//Store these item pointers for later when rendering selected items.
				eInfo.pusItemIndex[i] = usCounter;

				SetFont(SMALLCOMPFONT);
				SetFontForeground( FONT_MCOLOR_WHITE );
				SetFontDestBuffer( eInfo.uiBuffer, 0, 0, eInfo.sWidth, eInfo.sHeight, FALSE );


				if( eInfo.uiItemType != TBAR_MODE_ITEM_TRIGGERS )
				{
					LoadItemInfo( usCounter, pItemName, NULL );
					swprintf( pStr, L"%s", pItemName );
				}
				else
				{
					if( i == PRESSURE_ACTION_ID )
					{
						swprintf( pStr, pInitEditorItemsInfoText[0] );
					}
					else if( i < 2 )
					{
						if( usCounter == SWITCH )
							swprintf( pStr, pInitEditorItemsInfoText[5] );
						else
							swprintf( pStr, pInitEditorItemsInfoText[1] );
					}
					else if( i < 4 )
					{
						if( usCounter == SWITCH )
							swprintf( pStr, pInitEditorItemsInfoText[6] );
						else
							swprintf( pStr, pInitEditorItemsInfoText[2] );
					}
					else if( i < 6 )
					{
						if( usCounter == SWITCH )
							swprintf( pStr, pInitEditorItemsInfoText[7] );
						else
							swprintf( pStr, pInitEditorItemsInfoText[3] );
					}
					else
					{
						if( usCounter == SWITCH )
							swprintf( pStr, pInitEditorItemsInfoText[8], (i-4)/2 );
						else
							swprintf( pStr, pInitEditorItemsInfoText[4], (i-4)/2 );
					}
				}

				DisplayWrappedString(x, (UINT16)(y+25), 60, 2, SMALLCOMPFONT, FONT_WHITE, pStr, FONT_BLACK, TRUE, CENTER_JUSTIFIED );

				UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : item->ubGraphicNum;
				if(usGraphicNum < hVObject->usNumberOfObjects)
				{
					//Calculate the center position of the graphic in a 60 pixel wide area.
					sWidth = hVObject->pETRLEObject[usGraphicNum].usWidth;
					sOffset = hVObject->pETRLEObject[usGraphicNum].sOffsetX;
					sStart = x + (60 - sWidth - sOffset*2) / 2;

					if( sWidth && sWidth > 0 )
					{
						BltVideoObjectOutlineFromIndex( eInfo.uiBuffer, uiVideoObjectIndex, usGraphicNum, sStart, y+2, 0, FALSE );
					}

					//cycle through the various slot positions (0,0), (0,40), (60,0), (60,40), (120,0)...
					if( y == 0 )
					{
						y = 40;
					}
					else
					{
						y = 0;
						x += 60;
					}
				}
				else
				{
					static vfs::Log& editorLog = *vfs::Log::create(L"EditorItems.log");
					editorLog	<< L"Tried to access item [" 
								<< item->ubGraphicNum << L"/" << hVObject->usNumberOfObjects 
								<< L"]" << vfs::Log::endl;
				}
			}
			usCounter++;
		}
	}
	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	SetClippingRect(&SaveRect);
	gfRenderTaskbar = TRUE;
}

void DetermineItemsScrolling()
{
	if( !eInfo.sScrollIndex )
		DisableEditorButton( ITEMS_LEFTSCROLL );
	else
		EnableEditorButton( ITEMS_LEFTSCROLL );
	//Right most scroll position.	Calculated by taking every pair of numItems rounded up,
	//and subtracting 7 (because a scroll index 0 is disabled if there are <=12 items,
	//index 1 for <=14 items, index 2 for <=16 items...
	if( eInfo.sScrollIndex == max( ((eInfo.sNumItems+1)/2)-6, 0 ) )
		DisableEditorButton( ITEMS_RIGHTSCROLL );
	else
		EnableEditorButton( ITEMS_RIGHTSCROLL );
}

void RenderEditorItemsInfo()
{
	UINT8	*pDestBuf, *pSrcBuf;
	UINT32 uiSrcPitchBYTES, uiDestPitchBYTES;
	INVTYPE *item;
	HVOBJECT hVObject;
	UINT32 uiVideoObjectIndex;
	INT16 i;
	INT16 minIndex, maxIndex;
	INT16 sWidth, sOffset, sStart, x, y;
	UINT16 usNumItems;
	UINT16 usQuantity;

	if(!eInfo.fActive)
	{
		return;
	}

	if( gusMouseXPos < (iScreenWidthOffset + 110) || gusMouseXPos > (iScreenWidthOffset + 480) || gusMouseYPos < (2 * iScreenHeightOffset + 360) || gusMouseYPos > (2 * iScreenHeightOffset + 440) )
	{ //Mouse has moved out of the items display region -- so nothing can be highlighted.
		eInfo.sHilitedItemIndex = -1;
	}
	pDestBuf = LockVideoSurface(FRAME_BUFFER, &uiDestPitchBYTES);
	pSrcBuf = LockVideoSurface(eInfo.uiBuffer, &uiSrcPitchBYTES);

	Blt16BPPTo16BPP((UINT16 *)pDestBuf, uiDestPitchBYTES,
				(UINT16 *)pSrcBuf, uiSrcPitchBYTES, iScreenWidthOffset + 110, 2 * iScreenHeightOffset + 360, 60*eInfo.sScrollIndex, 0, 360, 80 );

	UnLockVideoSurface(eInfo.uiBuffer);
	UnLockVideoSurface(FRAME_BUFFER);

	//calculate the min and max index that is currently shown.	This determines
	//if the highlighted and/or selected items are drawn with the outlines.
	minIndex = eInfo.sScrollIndex * 2;
	maxIndex = minIndex + 11;

	//draw the hilighted item if applicable.
	if( eInfo.sHilitedItemIndex >= minIndex && eInfo.sHilitedItemIndex <= maxIndex )
	{
		if( eInfo.pusItemIndex )
		{
			item = &Item[eInfo.pusItemIndex[eInfo.sHilitedItemIndex]];
			uiVideoObjectIndex = GetInterfaceGraphicForItem( item );
			GetVideoObject( &hVObject, uiVideoObjectIndex );

			x = iScreenWidthOffset + (eInfo.sHilitedItemIndex/2 - eInfo.sScrollIndex)*60 + 110;
			y = 2 * iScreenHeightOffset + 360 + (eInfo.sHilitedItemIndex % 2) * 40;

			UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : item->ubGraphicNum;
			sWidth = hVObject->pETRLEObject[usGraphicNum].usWidth;
			sOffset = hVObject->pETRLEObject[usGraphicNum].sOffsetX;
			sStart = x + (60 - sWidth - sOffset*2) / 2;
			if( sWidth )
			{
				BltVideoObjectOutlineFromIndex( FRAME_BUFFER, uiVideoObjectIndex, usGraphicNum, sStart, y+2, Get16BPPColor(FROMRGB(250, 250, 0)), TRUE );
			}
		}
	}
	//draw the selected item
	if( eInfo.sSelItemIndex >= minIndex && eInfo.sSelItemIndex <= maxIndex )
	{
		if( eInfo.pusItemIndex )
		{
			item = &Item[eInfo.pusItemIndex[eInfo.sSelItemIndex]];
			uiVideoObjectIndex = GetInterfaceGraphicForItem( item );
			GetVideoObject( &hVObject, uiVideoObjectIndex );

			x = iScreenWidthOffset + (eInfo.sSelItemIndex/2 - eInfo.sScrollIndex)*60 + 110;
			y = 2 * iScreenHeightOffset + 360 + (eInfo.sSelItemIndex % 2) * 40;

			UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : item->ubGraphicNum;
			sWidth = hVObject->pETRLEObject[usGraphicNum].usWidth;
			sOffset = hVObject->pETRLEObject[usGraphicNum].sOffsetX;
			sStart = x + (60 - sWidth - sOffset*2) / 2;
			if( sWidth )
			{
				BltVideoObjectOutlineFromIndex( FRAME_BUFFER, uiVideoObjectIndex, usGraphicNum, sStart, y+2, Get16BPPColor(FROMRGB(250, 0, 0)), TRUE );
			}
		}
	}
	//draw item index no & the numbers of each visible item that currently resides in the world.
	maxIndex = min( maxIndex, eInfo.sNumItems-1 );
	for( i = minIndex; i <= maxIndex; i++ )
	{
		x = iScreenWidthOffset + (i/2 - eInfo.sScrollIndex)*60 + 110;
		y = 2 * iScreenHeightOffset + 360 + (i % 2) * 40;
		SetFont( BLOCKFONTNARROW );
		SetFontForeground( FONT_LTGREEN );
		SetFontShadow( FONT_NEARBLACK );
		
		// item index no
		mprintf( x + 12, y + 18, L"%d", eInfo.pusItemIndex[ i ] );

		// index no within usItemClass
		SetFontForeground( FONT_LTBLUE );
		mprintf( x + 40, y + 18, L"%d", i );

		// numbers of each visible item
		usNumItems = CountNumberOfEditorPlacementsInWorld( i, &usQuantity );

		if( usNumItems )
		{
			SetFont( FONT10ARIAL );
			SetFontForeground( FONT_YELLOW );
			if( usNumItems == usQuantity )
				mprintf( x + 10, y + 4, L"%d", usNumItems );
			else
				mprintf( x + 10, y + 4, L"%d(%d)", usNumItems, usQuantity );
		}
	}
}

void ClearEditorItemsInfo()
{
	if( eInfo.uiBuffer )
	{
		DeleteVideoSurfaceFromIndex( eInfo.uiBuffer );
		eInfo.uiBuffer = 0;
	}
	if( eInfo.pusItemIndex )
	{
		MemFree( eInfo.pusItemIndex );
		eInfo.pusItemIndex = NULL;
	}
	DisableEditorRegion( ITEM_REGION_ID );
	eInfo.fKill = 0;
	eInfo.fActive = 0;
	eInfo.sWidth = 0;
	eInfo.sHeight = 0;
	eInfo.sNumItems = 0;
	//save the highlighted selections
	switch( eInfo.uiItemType )
	{
		case TBAR_MODE_ITEM_WEAPONS:
			eInfo.sSaveSelWeaponsIndex = eInfo.sSelItemIndex;
			eInfo.sSaveWeaponsScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_AMMO:
			eInfo.sSaveSelAmmoIndex = eInfo.sSelItemIndex;
			eInfo.sSaveAmmoScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_ARMOUR:
			eInfo.sSaveSelArmourIndex = eInfo.sSelItemIndex;
			eInfo.sSaveArmourScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_LBEGEAR:
			eInfo.sSaveSelLBEIndex = eInfo.sSelItemIndex;
			eInfo.sSaveLBEScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_EXPLOSIVES:
			eInfo.sSaveSelExplosivesIndex = eInfo.sSelItemIndex;
			eInfo.sSaveExplosivesScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_EQUIPMENT1:
			eInfo.sSaveSelEquipment1Index = eInfo.sSelItemIndex;
			eInfo.sSaveEquipment1ScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_EQUIPMENT2:
			eInfo.sSaveSelEquipment2Index = eInfo.sSelItemIndex;
			eInfo.sSaveEquipment2ScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_EQUIPMENT3:
			eInfo.sSaveSelEquipment3Index = eInfo.sSelItemIndex;
			eInfo.sSaveEquipment3ScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_TRIGGERS:
			eInfo.sSaveSelTriggersIndex = eInfo.sSelItemIndex;
			eInfo.sSaveTriggersScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_KEYS:
			eInfo.sSaveSelKeysIndex = eInfo.sSelItemIndex;
			eInfo.sSaveKeysScrollIndex = eInfo.sScrollIndex;
			break;
		case TBAR_MODE_ITEM_RANDOMITEM:
			eInfo.sSaveSelRandomItemIndex = eInfo.sSelItemIndex;
			eInfo.sSaveRandomItemScrollIndex = eInfo.sScrollIndex;
			break;
	}
}

void HandleItemsPanel( UINT16 usScreenX, UINT16 usScreenY, INT8 bEvent )
{
	INT16 sIndex;
	UINT16 usQuantity;
	//Calc base index from scrolling index
	sIndex = eInfo.sScrollIndex * 2;
	//Determine if the index is in the first row or second row from mouse YPos.
	if( usScreenY >= (2 * iScreenHeightOffset + 400) )
		sIndex++;
	//Add the converted mouse's XPos into a relative index;
	//Calc:	starting from 110, for every 60 pixels, add 2 to the index

	sIndex += ((usScreenX-110-iScreenWidthOffset)/60) * 2;
	switch( bEvent )
	{
		case GUI_MOVE_EVENT:
			if( sIndex < eInfo.sNumItems )
			{
				if( eInfo.sHilitedItemIndex != sIndex )
					gfRenderTaskbar = TRUE;
				//this index will now highlight in yellow.
				eInfo.sHilitedItemIndex = sIndex;
			}
			break;
		case GUI_LCLICK_EVENT:
			if( sIndex < eInfo.sNumItems )
			{
				//this index will now highlight in red.
				if( eInfo.sSelItemIndex != sIndex )
					gfRenderTaskbar = TRUE;
				eInfo.sSelItemIndex = sIndex;
				if( gfMercGetItem )
				{
					gfMercGetItem = FALSE;
					gusMercsNewItemIndex = eInfo.pusItemIndex[ eInfo.sSelItemIndex ];
					SetMercEditingMode( MERC_INVENTORYMODE );
					ClearEditorItemsInfo();
				}
			}
			break;
		case GUI_RCLICK_EVENT:
			if( gfMercGetItem )
			{
				gfMercGetItem = FALSE;
				gusMercsNewItemIndex = 0xffff;
				SetMercEditingMode( MERC_INVENTORYMODE );
				ClearEditorItemsInfo();
			}
			else if( sIndex < eInfo.sNumItems )
			{
				eInfo.sSelItemIndex = sIndex;
				gfRenderTaskbar = TRUE;
				if( CountNumberOfEditorPlacementsInWorld( eInfo.sSelItemIndex, &usQuantity ) )
				{
					FindNextItemOfSelectedType();
				}
			}
			break;
	}
}

void ShowItemCursor( INT32 iMapIndex )
{
	LEVELNODE *pNode;
	pNode = gpWorldLevelData[ iMapIndex ].pTopmostHead;
	while( pNode )
	{
		if( pNode->usIndex == SELRING1 )//dnl ch86 240214
			return;
		pNode = pNode->pNext;
	}
	AddTopmostToTail( iMapIndex, SELRING1 );
}

void HideItemCursor( INT32 iMapIndex )
{
	RemoveTopmost( iMapIndex, SELRING1 );
}

BOOLEAN TriggerAtGridNo( INT32 sGridNo )
{
	ITEM_POOL *pItemPool;
	if( !GetItemPoolFromGround( sGridNo, &pItemPool ) )
	{
		return FALSE;
	}
	while( pItemPool )
	{
		if( gWorldItems[ pItemPool->iItemIndex ].object.usItem == SWITCH )
		{
			return TRUE;
		}
		pItemPool = pItemPool->pNext;
	}
	return FALSE;
}


void AddSelectedItemToWorld( INT32 sGridNo )
{
	OBJECTTYPE *pObject;
	INVTYPE		*pItem;
	ITEM_POOL *pItemPool;
	INT32 iItemIndex;
	INT8 bVisibility = INVISIBLE;
	BOOLEAN fFound = FALSE;
	IPListNode *pIPCurr, *pIPPrev;
	UINT16 usFlags;

	//Extract the currently selected item.
	SpecifyItemToEdit( NULL, -1 );

	if( eInfo.uiItemType == TBAR_MODE_ITEM_KEYS )
	{
		if ( fEditorCreateItemFromKeyboard )
		{		
			CreateKeyObject( &gTempObject, 1, (UINT8)usEditorTempItem );
			fEditorCreateItemFromKeyboard = FALSE;
		}
		else
			CreateKeyObject( &gTempObject, 1, (UINT8)eInfo.sSelItemIndex );
	}
	else
	{
		if ( fEditorCreateItemFromKeyboard )
		{		
			CreateItem( usEditorTempItem, 100, &gTempObject );
			fEditorCreateItemFromKeyboard = FALSE;
		}
		else
		{
			if(!eInfo.pusItemIndex)//dnl ch74 181013
				return;
			CreateItem( eInfo.pusItemIndex[eInfo.sSelItemIndex], 100, &gTempObject );
		}
	}
	usFlags = 0;
	switch( gTempObject.usItem )
	{
		case MINE:
			if ( bVisibility == BURIED )
			{
				usFlags |= WORLD_ITEM_ARMED_BOMB;
			}
			break;
		case MONEY:
		case SILVER:
		case GOLD:
			gTempObject[0]->data.objectStatus = 100;
			gTempObject[0]->data.money.uiMoneyAmount = 100 + Random( 19901 );
			break;
		case OWNERSHIP:
			gTempObject[0]->data.owner.ubOwnerProfile = NO_PROFILE;
			bVisibility = BURIED;
			break;
		case SWITCH:
			if( TriggerAtGridNo( sGridNo ) )
			{ //Restricted to one action per gridno.
				return;
			}
			bVisibility = BURIED;
			gTempObject[0]->data.objectStatus = 100;
			gTempObject[0]->data.misc.ubBombOwner = 1;
			if( eInfo.sSelItemIndex < 2 )
				gTempObject[0]->data.misc.bFrequency = PANIC_FREQUENCY;
			else if( eInfo.sSelItemIndex < 4 )
				gTempObject[0]->data.misc.bFrequency = PANIC_FREQUENCY_2;
			else if( eInfo.sSelItemIndex < 6 )
				gTempObject[0]->data.misc.bFrequency = PANIC_FREQUENCY_3;
			else
				gTempObject[0]->data.misc.bFrequency = (INT8)(FIRST_MAP_PLACED_FREQUENCY + (eInfo.sSelItemIndex-4) / 2);
			usFlags |= WORLD_ITEM_ARMED_BOMB;
			break;
		case ACTION_ITEM:
			bVisibility = BURIED;
			gTempObject[0]->data.objectStatus = 100;
			gTempObject[0]->data.misc.ubBombOwner = 1;
			gTempObject[0]->data.bTrap = gbDefaultBombTrapLevel;
			if( eInfo.sSelItemIndex < PRESSURE_ACTION_ID )
			{
				gTempObject[0]->data.misc.bDetonatorType = BOMB_REMOTE;
				if( eInfo.sSelItemIndex < 2 )
					gTempObject[0]->data.misc.bFrequency = PANIC_FREQUENCY;
				else if( eInfo.sSelItemIndex < 4 )
					gTempObject[0]->data.misc.bFrequency = PANIC_FREQUENCY_2;
				else if( eInfo.sSelItemIndex < 6 )
					gTempObject[0]->data.misc.bFrequency = PANIC_FREQUENCY_3;
				else
					gTempObject[0]->data.misc.bFrequency = (INT8)(FIRST_MAP_PLACED_FREQUENCY + (eInfo.sSelItemIndex-4) / 2);
			}
			else
			{
				gTempObject[0]->data.misc.bDetonatorType = BOMB_PRESSURE;
				gTempObject[0]->data.misc.bDelay = 0;
			}
			ChangeActionItem( &gTempObject, gbActionItemIndex );
			gTempObject.fFlags |= OBJECT_ARMED_BOMB;
			if( gbActionItemIndex == ACTIONITEM_SMPIT )
				Add3X3Pit( sGridNo );
			else if( gbActionItemIndex == ACTIONITEM_LGPIT )
				Add5X5Pit( sGridNo );
			usFlags |= WORLD_ITEM_ARMED_BOMB;
			break;
	}

	ITEM_POOL *pItemPoolOld;
	GetItemPoolFromGround(sGridNo, &pItemPoolOld);//dnl ch86 220214
	pObject = InternalAddItemToPool( &sGridNo, &gTempObject, bVisibility, 0, usFlags, 0, -1, &iItemIndex );
	if( gTempObject.usItem != OWNERSHIP )
	{
		gWorldItems[ iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance);
	}
	else
	{
		gWorldItems[ iItemIndex ].ubNonExistChance = 0;
	}

	pItem = &( Item[ pObject->usItem ] );
	if( pItem->usItemClass == IC_AMMO )
	{
		if (Random( 2 ))
		{
			(*pObject)[0]->data.ubShotsLeft = Magazine[ pItem->ubClassIndex ].ubMagSize;
		}
		else
		{
			(*pObject)[0]->data.ubShotsLeft = (UINT8) Random( Magazine[ pItem->ubClassIndex ].ubMagSize );
		}
	}
	else
	{
		if(gTempObject.usItem != OWNERSHIP)//dnl ch35 110909
		{
			(*pObject)[0]->data.objectStatus = (INT8)(70 + Random( 26 ));
			(*pObject)[0]->data.sRepairThreshold = max(1, min(100, (100 + (*pObject)[0]->data.objectStatus)/2 ));
		}
	}
	if( pItem->usItemClass & IC_GUN )
	{
		if ( pObject->usItem == ROCKET_LAUNCHER )
		{
			(*pObject)[0]->data.gun.ubGunShotsLeft = 1;
		}
		else
		{
			(*pObject)[0]->data.gun.ubGunShotsLeft = (UINT8)(Random( Weapon[ pObject->usItem ].ubMagSize ));
		}
	}

	if( !GetItemPoolFromGround( sGridNo, &pItemPool ) )
		Assert( 0 );
	UpdateItemPoolInUndoList(sGridNo, pItemPoolOld, pItemPool);//dnl ch86 220214
	while( pItemPool )
	{
		if( &(gWorldItems[ pItemPool->iItemIndex ].object) == pObject )
		{
			fFound = TRUE;
			//ShowSelectedItem();
			break;
		}
		pItemPool = pItemPool->pNext;
	}
	Assert( fFound );

	gpItemPool = pItemPool;

	SpecifyItemToEdit( pObject, sGridNo );

	//Get access to the itempool.
	//search for a current node in list containing same mapindex
	pIPCurr = pIPHead;
	pIPPrev = NULL;
	while( pIPCurr )
	{
		pIPPrev = pIPCurr;
		if( pIPCurr->sGridNo == sGridNo )
		{
			//found one, so we don't need to add it
			gpCurrItemPoolNode = pIPCurr;
			return;
		}
		pIPCurr = pIPCurr->next;
	}
	//there isn't one, so we will add it now.
	ShowItemCursor( sGridNo );
	if( pIPPrev )
	{
		pIPPrev->next = (IPListNode*)MemAlloc( sizeof( IPListNode ) );
		Assert( pIPPrev->next );
		pIPPrev = pIPPrev->next;
		pIPPrev->next = NULL;
		pIPPrev->sGridNo = sGridNo;
		gpCurrItemPoolNode = pIPPrev;
	}
	else
	{
		pIPHead = (IPListNode*)MemAlloc( sizeof( IPListNode ) );
		Assert( pIPHead );
		pIPHead->next = NULL;
		pIPHead->sGridNo = sGridNo;
		gpCurrItemPoolNode = pIPHead;
	}
}

void HandleRightClickOnItem( INT32 sGridNo )
{
	ITEM_POOL *pItemPool;
	IPListNode *pIPCurr;

	if( gsItemGridNo == sGridNo )
	{ //Clicked on the same gridno as the selected item.	Automatically select the next
		//item in the same pool.
		pItemPool = gpItemPool->pNext;
		if( !pItemPool )
		{ //currently selected item was last node, so select the head node even if it is the same.
			GetItemPoolFromGround( sGridNo, &pItemPool );
		}
	}
	else if( !GetItemPoolFromGround( sGridNo, &pItemPool ) )
	{
		//possibly relocate selected item to this gridno?
		return;
	}

	gpItemPool = pItemPool;

	//set up the item pool pointer to point to the same mapindex node
	pIPCurr = pIPHead;
	gpCurrItemPoolNode = NULL;
	while( pIPCurr )
	{
		if( pIPCurr->sGridNo == sGridNo )
		{
			gpCurrItemPoolNode = pIPCurr;
			break;
		}
		pIPCurr = pIPCurr->next;
	}
	Assert( gpCurrItemPoolNode );
	SpecifyItemToEdit( &gWorldItems[ gpItemPool->iItemIndex ].object, gpItemPool->sGridNo );
}

extern void DeleteSelectedMercsItem();

void DeleteSelectedItem()
{
	SpecifyItemToEdit( NULL, -1 );
	//First, check to see if there even is a currently selected item.
	if( gsSelectedMercID != -1 )//dnl ch86 220214
	{
		DeleteSelectedMercsItem();
		return;
	}
	if( gpItemPool )
	{ //Okay, we have a selected item...
		INT32 sGridNo;
		//save the mapindex
		if( gpItemPool->pNext )
		{
			SpecifyItemToEdit( &gWorldItems[ gpItemPool->pNext->iItemIndex ].object, gpItemPool->sGridNo );
		}
		sGridNo = gpItemPool->sGridNo;
		//remove the item
		if( gWorldItems[ gpItemPool->iItemIndex ].object.usItem == ACTION_ITEM )
		{
			if( gWorldItems[ gpItemPool->iItemIndex ].object[0]->data.misc.bActionValue == ACTION_ITEM_SMALL_PIT )
				Remove3X3Pit( gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
			else if( gWorldItems[ gpItemPool->iItemIndex ].object[0]->data.misc.bActionValue == ACTION_ITEM_LARGE_PIT )
				Remove5X5Pit( gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
		}
		if( gpEditingItemPool == gpItemPool )
			gpEditingItemPool = NULL;
		RemoveItemFromPool( sGridNo, gpItemPool->iItemIndex, 0 );
#if 0//dnl ch86 220214
		gpItemPool = NULL;
		//determine if there are still any items at this location
		if( GetItemPoolFromGround( sGridNo, &gpItemPool ) )
#else
		ITEM_POOL *pItemPoolOld = gpItemPool;
		GetItemPoolFromGround(sGridNo, &gpItemPool);
		UpdateItemPoolInUndoList(sGridNo, pItemPoolOld, gpItemPool);
		if(gpItemPool)
#endif
		{ //reset display for remaining items
			SpecifyItemToEdit( &gWorldItems[ gpItemPool->iItemIndex ].object, gpItemPool->sGridNo );
		}
		else
		{ //no items left, so remove the node from the list.
			IPListNode *pIPPrev, *pIPCurr;
			pIPCurr = pIPHead;
			pIPPrev = NULL;
			while( pIPCurr )
			{
				if( pIPCurr->sGridNo == sGridNo )
				{
					if( pIPPrev ) //middle of list
						pIPPrev->next = pIPCurr->next;
					else //head of list
						pIPHead = pIPHead->next;
					//move the curr item pool to the next one.
					if( pIPCurr->next )
						gpCurrItemPoolNode = pIPCurr->next;
					else
						gpCurrItemPoolNode = pIPHead;
					if( gpCurrItemPoolNode && iCurrentTaskbar == TASK_ITEMS )//dnl ch86 220214
					{
						//get the item pool at this node's gridno.
						GetItemPoolFromGround( gpCurrItemPoolNode->sGridNo, &gpItemPool );
						Assert( gpItemPool );

						MarkMapIndexDirty( gpItemPool->sGridNo );
						SpecifyItemToEdit( &gWorldItems[ gpItemPool->iItemIndex ].object, gpItemPool->sGridNo );
						if( gsItemGridNo != -1 )
						{
							CenterScreenAtMapIndex( gsItemGridNo );
						}
					}
					//remove node
					HideItemCursor( sGridNo );
					MemFree( pIPCurr );
					pIPCurr = NULL;
					return;
				}
				pIPPrev = pIPCurr;
				pIPCurr = pIPCurr->next;
			}
		}
	}
}

void ShowSelectedItem()
{
	if( gpItemPool )
	{
		gpItemPool->bVisible = INVISIBLE;
		gWorldItems[ gpItemPool->iItemIndex ].bVisible = INVISIBLE;
	}
}

void HideSelectedItem()
{
	if( gpItemPool )
	{
		gpItemPool->bVisible = HIDDEN_ITEM;
		gWorldItems[ gpItemPool->iItemIndex ].bVisible = HIDDEN_ITEM;
	}
}

void SelectNextItemPool()
{
	if( !gpCurrItemPoolNode )
		return;
//remove the current hilight.
	if( gpItemPool )
	{
		MarkMapIndexDirty( gpItemPool->sGridNo );
	}

	//go to the next node.	If at end of list, choose pIPHead
	if( gpCurrItemPoolNode->next )
		gpCurrItemPoolNode = gpCurrItemPoolNode->next;
	else
		gpCurrItemPoolNode = pIPHead;
	//get the item pool at this node's gridno.
	GetItemPoolFromGround( gpCurrItemPoolNode->sGridNo, &gpItemPool );
	MarkMapIndexDirty( gpItemPool->sGridNo );
	SpecifyItemToEdit( &gWorldItems[ gpItemPool->iItemIndex ].object, gpItemPool->sGridNo );
	if( gsItemGridNo != -1 )
	{
		CenterScreenAtMapIndex( gsItemGridNo );
	}
}

void SelectNextItemInPool()
{
	if( gpItemPool )
	{
		if( gpItemPool->pNext )
		{
			gpItemPool = gpItemPool->pNext;
		}
		else
		{
			GetItemPoolFromGround( gpItemPool->sGridNo, &gpItemPool );
		}
		SpecifyItemToEdit( &gWorldItems[ gpItemPool->iItemIndex ].object, gpItemPool->sGridNo );
		MarkWorldDirty();
	}
}

void SelectPrevItemInPool()
{
	if( gpItemPool )
	{
		if( gpItemPool->pPrev )
		{
			gpItemPool = gpItemPool->pPrev;
		}
		else
		{
			GetItemPoolFromGround( gpItemPool->sGridNo, &gpItemPool );
			while( gpItemPool->pNext )
			{
				gpItemPool = gpItemPool->pNext;
			}
		}
		SpecifyItemToEdit( &gWorldItems[ gpItemPool->iItemIndex ].object, gpItemPool->sGridNo );
		MarkWorldDirty();
	}
}


void FindNextItemOfSelectedType()
{
	UINT16 usItem;
	usItem = eInfo.pusItemIndex[ eInfo.sSelItemIndex ];
	if( usItem == ACTION_ITEM || usItem == SWITCH )
	{
		if( eInfo.sSelItemIndex < PRESSURE_ACTION_ID )
		{
			INT8 bFrequency;
			if( eInfo.sSelItemIndex < 2 )
				bFrequency = PANIC_FREQUENCY;
			else if( eInfo.sSelItemIndex < 4 )
				bFrequency = PANIC_FREQUENCY_2;
			else if( eInfo.sSelItemIndex < 6 )
				bFrequency = PANIC_FREQUENCY_3;
			else
				bFrequency = (INT8)(FIRST_MAP_PLACED_FREQUENCY + (eInfo.sSelItemIndex-4) / 2);
			SelectNextTriggerWithFrequency( usItem, bFrequency );
		}
		else
		{
			SelectNextPressureAction();
		}
	}
	else if( Item[ usItem ].usItemClass == IC_KEY )
	{
		SelectNextKeyOfType( (UINT8)eInfo.sSelItemIndex );
	}
	else
	{
		SelectNextItemOfType( usItem );
	}
}

void SelectNextItemOfType( UINT16 usItem )
{
	IPListNode *curr;
	OBJECTTYPE *pObject;
	if( gpItemPool )
	{
		curr = pIPHead;
		while( curr )
		{ //skip quickly to the same gridno as the item pool
			if( curr->sGridNo == gWorldItems[ gpItemPool->iItemIndex ].sGridNo )
			{
				gpItemPool = gpItemPool->pNext;
				while( gpItemPool )
				{
					pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
					if( pObject->usItem == usItem )
					{
						SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
						CenterScreenAtMapIndex( gsItemGridNo );
						return; //success! (another item in same itempool)
					}
					gpItemPool = gpItemPool->pNext;
				}
				curr = curr->next;
				break;
			}
			curr = curr->next;
		}
		while( curr )
		{ //search to the end of the list
			GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
			while( gpItemPool )
			{
				pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
				if( pObject->usItem == usItem )
				{
					SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
					CenterScreenAtMapIndex( gsItemGridNo );
					return; //success! (found another item before reaching the end of the list)
				}
				gpItemPool = gpItemPool->pNext;
			}
			curr = curr->next;
		}
	}
	curr = pIPHead;
	while( curr )
	{ //search to the end of the list
		GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
		while( gpItemPool )
		{
			pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
			if( pObject->usItem == usItem )
			{
				SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
				CenterScreenAtMapIndex( gsItemGridNo );
				return; //success! (found first item in the list)
			}
			gpItemPool = gpItemPool->pNext;
		}
		curr = curr->next;
	}
}

void SelectNextKeyOfType( UINT8 ubKeyID )
{
	IPListNode *curr;
	OBJECTTYPE *pObject;
	if( gpItemPool )
	{
		curr = pIPHead;
		while( curr )
		{ //skip quickly to the same gridno as the item pool
			if( curr->sGridNo == gWorldItems[ gpItemPool->iItemIndex ].sGridNo )
			{
				gpItemPool = gpItemPool->pNext;
				while( gpItemPool )
				{
					pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
					if( Item[ pObject->usItem ].usItemClass == IC_KEY && (*pObject)[0]->data.key.ubKeyID == ubKeyID )
					{
						SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
						CenterScreenAtMapIndex( gsItemGridNo );
						return; //success! (another item in same itempool)
					}
					gpItemPool = gpItemPool->pNext;
				}
				curr = curr->next;
				break;
			}
			curr = curr->next;
		}
		while( curr )
		{ //search to the end of the list
			GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
			while( gpItemPool )
			{
				pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
				if( Item[ pObject->usItem ].usItemClass == IC_KEY && (*pObject)[0]->data.key.ubKeyID == ubKeyID )
				{
					SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
					CenterScreenAtMapIndex( gsItemGridNo );
					return; //success! (found another item before reaching the end of the list)
				}
				gpItemPool = gpItemPool->pNext;
			}
			curr = curr->next;
		}
	}
	curr = pIPHead;
	while( curr )
	{ //search to the end of the list
		GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
		while( gpItemPool )
		{
			pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
			if( Item[ pObject->usItem ].usItemClass == IC_KEY && (*pObject)[0]->data.key.ubKeyID == ubKeyID )
			{
				SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
				CenterScreenAtMapIndex( gsItemGridNo );
				return; //success! (found first item in the list)
			}
			gpItemPool = gpItemPool->pNext;
		}
		curr = curr->next;
	}
}

void SelectNextTriggerWithFrequency( UINT16 usItem, INT8 bFrequency )
{
	IPListNode *curr;
	OBJECTTYPE *pObject;
	if( gpItemPool )
	{
		curr = pIPHead;
		while( curr )
		{ //skip quickly to the same gridno as the item pool
			if( curr->sGridNo == gWorldItems[ gpItemPool->iItemIndex ].sGridNo )
			{
				gpItemPool = gpItemPool->pNext;
				while( gpItemPool )
				{
					pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
					if( pObject->usItem == usItem && (*pObject)[0]->data.misc.bFrequency == bFrequency )
					{
						SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
						CenterScreenAtMapIndex( gsItemGridNo );
						return; //success! (another item in same itempool)
					}
					gpItemPool = gpItemPool->pNext;
				}
				curr = curr->next;
				break;
			}
			curr = curr->next;
		}
		while( curr )
		{ //search to the end of the list
			GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
			while( gpItemPool )
			{
				pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
				if( pObject->usItem == usItem && (*pObject)[0]->data.misc.bFrequency == bFrequency )
				{
					SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
					CenterScreenAtMapIndex( gsItemGridNo );
					return; //success! (found another item before reaching the end of the list)
				}
				gpItemPool = gpItemPool->pNext;
			}
			curr = curr->next;
		}
	}
	curr = pIPHead;
	while( curr )
	{ //search to the end of the list
		GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
		while( gpItemPool )
		{
			pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
			if( pObject->usItem == usItem && (*pObject)[0]->data.misc.bFrequency == bFrequency )
			{
				SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
				CenterScreenAtMapIndex( gsItemGridNo );
				return; //success! (found first item in the list)
			}
			gpItemPool = gpItemPool->pNext;
		}
		curr = curr->next;
	}
}

void SelectNextPressureAction()
{
	IPListNode *curr;
	OBJECTTYPE *pObject;
	if( gpItemPool )
	{
		curr = pIPHead;
		while( curr )
		{ //skip quickly to the same gridno as the item pool
			if( curr->sGridNo == gWorldItems[ gpItemPool->iItemIndex ].sGridNo )
			{
				gpItemPool = gpItemPool->pNext;
				while( gpItemPool )
				{
					pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
					if( pObject->usItem == ACTION_ITEM && (*pObject)[0]->data.misc.bDetonatorType == BOMB_PRESSURE )
					{
						SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
						CenterScreenAtMapIndex( gsItemGridNo );
						return; //success! (another item in same itempool)
					}
					gpItemPool = gpItemPool->pNext;
				}
				curr = curr->next;
				break;
			}
			curr = curr->next;
		}
		while( curr )
		{ //search to the end of the list
			GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
			while( gpItemPool )
			{
				pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
				if( pObject->usItem == ACTION_ITEM && (*pObject)[0]->data.misc.bDetonatorType == BOMB_PRESSURE )
				{
					SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
					CenterScreenAtMapIndex( gsItemGridNo );
					return; //success! (found another item before reaching the end of the list)
				}
				gpItemPool = gpItemPool->pNext;
			}
			curr = curr->next;
		}
	}
	curr = pIPHead;
	while( curr )
	{ //search to the end of the list
		GetItemPoolFromGround( curr->sGridNo, &gpItemPool );
		while( gpItemPool )
		{
			pObject = &gWorldItems[ gpItemPool->iItemIndex ].object;
			if( pObject->usItem == ACTION_ITEM && (*pObject)[0]->data.misc.bDetonatorType == BOMB_PRESSURE )
			{
				SpecifyItemToEdit( pObject, gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
				CenterScreenAtMapIndex( gsItemGridNo );
				return; //success! (found first item in the list)
			}
			gpItemPool = gpItemPool->pNext;
		}
		curr = curr->next;
	}
}

UINT16 CountNumberOfItemPlacementsInWorld( UINT16 usItem, UINT16 *pusQuantity )
{
	ITEM_POOL *pItemPool;
	IPListNode *pIPCurr;
	INT16 num = 0;
	*pusQuantity = 0;
	pIPCurr = pIPHead;
	while( pIPCurr )
	{
		GetItemPoolFromGround( pIPCurr->sGridNo, &pItemPool );
		while( pItemPool )
		{
			if( gWorldItems[ pItemPool->iItemIndex ].object.usItem == usItem )
			{
				num++;
				*pusQuantity += gWorldItems[ pItemPool->iItemIndex ].object.ubNumberOfObjects;
			}
			pItemPool = pItemPool->pNext;
		}
		pIPCurr = pIPCurr->next;
	}
	return num;
}

UINT16 CountNumberOfItemsWithFrequency( UINT16 usItem, INT8 bFrequency )
{
	ITEM_POOL *pItemPool;
	IPListNode *pIPCurr;
	UINT16 num = 0;
	pIPCurr = pIPHead;
	while( pIPCurr )
	{
		GetItemPoolFromGround( pIPCurr->sGridNo, &pItemPool );
		while( pItemPool )
		{
			if( gWorldItems[ pItemPool->iItemIndex ].object.usItem == usItem &&
				gWorldItems[ pItemPool->iItemIndex ].object[0]->data.misc.bFrequency == bFrequency )
			{
				num++;
			}
			pItemPool = pItemPool->pNext;
		}
		pIPCurr = pIPCurr->next;
	}
	return num;
}

UINT16 CountNumberOfPressureActionsInWorld()
{
	ITEM_POOL *pItemPool;
	IPListNode *pIPCurr;
	UINT16 num = 0;
	pIPCurr = pIPHead;
	while( pIPCurr )
	{
		GetItemPoolFromGround( pIPCurr->sGridNo, &pItemPool );
		while( pItemPool )
		{
			if( gWorldItems[ pItemPool->iItemIndex ].object.usItem == ACTION_ITEM &&
				gWorldItems[ pItemPool->iItemIndex ].object[0]->data.misc.bDetonatorType == BOMB_PRESSURE )
			{
				num++;
			}
			pItemPool = pItemPool->pNext;
		}
		pIPCurr = pIPCurr->next;
	}
	return num;
}

UINT16 CountNumberOfEditorPlacementsInWorld( UINT16 usEInfoIndex, UINT16 *pusQuantity )
{
	UINT16 usNumPlacements;
	if( eInfo.uiItemType == TBAR_MODE_ITEM_TRIGGERS )
	{	//find identical items with same frequency
		INT8 bFrequency;
		if( usEInfoIndex < PRESSURE_ACTION_ID )
		{
			if( usEInfoIndex < 2 )
				bFrequency = PANIC_FREQUENCY;
			else if( usEInfoIndex < 4 )
				bFrequency = PANIC_FREQUENCY_2;
			else if( usEInfoIndex < 6 )
				bFrequency = PANIC_FREQUENCY_3;
			else
				bFrequency = (INT8)(FIRST_MAP_PLACED_FREQUENCY + (usEInfoIndex-4) / 2);
			usNumPlacements = CountNumberOfItemsWithFrequency( eInfo.pusItemIndex[usEInfoIndex], bFrequency );
			*pusQuantity = usNumPlacements;
		}
		else
		{
			usNumPlacements = CountNumberOfPressureActionsInWorld();
			*pusQuantity = usNumPlacements;
		}
	}
	else if( eInfo.uiItemType == TBAR_MODE_ITEM_KEYS )
	{
		usNumPlacements = CountNumberOfKeysOfTypeInWorld( (UINT8)usEInfoIndex );
		*pusQuantity = usNumPlacements;
	}
	else
	{
		usNumPlacements = CountNumberOfItemPlacementsInWorld( eInfo.pusItemIndex[ usEInfoIndex], pusQuantity );
	}
	return usNumPlacements;
}

UINT16 CountNumberOfKeysOfTypeInWorld( UINT8 ubKeyID )
{
	ITEM_POOL *pItemPool;
	IPListNode *pIPCurr;
	INT16 num = 0;
	pIPCurr = pIPHead;
	while( pIPCurr )
	{
		GetItemPoolFromGround( pIPCurr->sGridNo, &pItemPool );
		while( pItemPool )
		{
			if( Item[ gWorldItems[ pItemPool->iItemIndex ].object.usItem ].usItemClass == IC_KEY )
			{
				if( gWorldItems[ pItemPool->iItemIndex ].object[0]->data.key.ubKeyID == ubKeyID )
				{
					num++;
				}
			}
			pItemPool = pItemPool->pNext;
		}
		pIPCurr = pIPCurr->next;
	}
	return num;
}

void DisplayItemStatistics()
{
	BOOLEAN fUseSelectedItem;
	INT16 usItemIndex;
	CHAR16 pItemName[SIZE_ITEM_NAME];
	INVTYPE *pItem;

	if( !eInfo.fActive )
	{
		return;
	}

	//If there is nothing else currently highlited by the mouse, use the selected item.
	fUseSelectedItem = eInfo.sHilitedItemIndex == -1 || eInfo.sHilitedItemIndex == eInfo.sSelItemIndex;

	SetFont( SMALLCOMPFONT );
	SetFontForeground( (UINT8)(fUseSelectedItem ? FONT_LTRED : FONT_YELLOW) );

	//Extract all of the item information.
	if( !eInfo.pusItemIndex )
		return;
	usItemIndex = eInfo.pusItemIndex[ fUseSelectedItem ? eInfo.sSelItemIndex : eInfo.sHilitedItemIndex ];
	pItem = &Item[ usItemIndex ];
	LoadItemInfo( usItemIndex, pItemName, NULL );

	mprintf( iScreenWidthOffset + 50 - StringPixLength( pItemName , SMALLCOMPFONT )/2, 2 * iScreenHeightOffset + 403, pItemName );
	mprintf( iScreenWidthOffset + 2, 2 * iScreenHeightOffset + 410, pDisplayItemStatisticsTex[0]);
	mprintf( iScreenWidthOffset + 2, 2 * iScreenHeightOffset + 420, pDisplayItemStatisticsTex[1]);
	mprintf( iScreenWidthOffset + 2, 2 * iScreenHeightOffset + 430, pDisplayItemStatisticsTex[2]);
	mprintf( iScreenWidthOffset + 2, 2 * iScreenHeightOffset + 440, pDisplayItemStatisticsTex[3]);
	mprintf( iScreenWidthOffset + 2, 2 * iScreenHeightOffset + 450, pDisplayItemStatisticsTex[4]);
}

void ScrollEditorItemsInfo(BOOLEAN fForward)//dnl ch80 011213
{
	if(fForward)
	{
		if(eInfo.sScrollIndex < max(0, (eInfo.sNumItems+1)/2-6))
		{
			if(_KeyDown(17))// CTRL
			{
				if(_KeyDown(16))// SHIFT
					eInfo.sScrollIndex = max(0, (eInfo.sNumItems+1)/2-6);
				else
					eInfo.sScrollIndex = min(eInfo.sScrollIndex+60, (eInfo.sNumItems+1)/2-6);
			}
			else if(_KeyDown(16))
				eInfo.sScrollIndex = min(eInfo.sScrollIndex+6, (eInfo.sNumItems+1)/2-6);
			else
				eInfo.sScrollIndex++;
		}
	}
	else
	{
		if(eInfo.sScrollIndex)
		{
			if(_KeyDown(17))// CTRL
			{
				if(_KeyDown(16))// SHIFT
					eInfo.sScrollIndex = 0;
				else
					eInfo.sScrollIndex = max(eInfo.sScrollIndex-60, 0);
			}
			else if(_KeyDown(16))
				eInfo.sScrollIndex = max(eInfo.sScrollIndex-6, 0);
			else
				eInfo.sScrollIndex--;
		}
	}
	DetermineItemsScrolling();
	gfRenderTaskbar = TRUE;
}

//dnl ch86 120214
BOOLEAN ItemPoolListMove(INT32 iOldGridNo, INT32 iNewGridNo, ITEM_POOL *pItemPool)
{
	while(pItemPool)
	{
		if(gWorldItems[pItemPool->iItemIndex].sGridNo == iOldGridNo)
			gWorldItems[pItemPool->iItemIndex].sGridNo = iNewGridNo;
		pItemPool->sGridNo = iNewGridNo;
		pItemPool = pItemPool->pNext;
	}
	IPListNode *curr = pIPHead;
	while(curr)
	{
		if(curr->sGridNo == iOldGridNo)
			curr->sGridNo = iNewGridNo;
		curr = curr->next;
	}
	return(TRUE);
}

BOOLEAN DeleteItemNode(INT32 iMapIndex)
{
	IPListNode *pIPPrev = NULL, *pIPCurr = pIPHead;
	while(pIPCurr)
	{
		if(pIPCurr->sGridNo == iMapIndex)// remove node from the list
		{
			if(pIPPrev)
				pIPPrev->next = pIPCurr->next;// middle of list
			else
				pIPHead = pIPHead->next;// head of list
			if(pIPCurr->next)// move the curr item pool to the next one
				gpCurrItemPoolNode = pIPCurr->next;
			else
				gpCurrItemPoolNode = pIPHead;
			MemFree(pIPCurr);
			pIPCurr = NULL;
			return(TRUE);
		}
		pIPPrev = pIPCurr, pIPCurr = pIPCurr->next;
	}
	return(FALSE);
}

#endif
