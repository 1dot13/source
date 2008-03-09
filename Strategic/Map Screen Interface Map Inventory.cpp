#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
	#include "GameSettings.h"
#else
	#include "Map Screen Interface Map Inventory.h"
	#include "Render Dirty.h"
	#include "vobject.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "sysutil.h"
	#include "Map Screen Interface Border.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map.h"
	#include "Items.h"
	#include "Interface Items.h"
	#include "Cursor Control.h"
	#include "Interface Utils.h"
	#include "Text.h"
	#include "Font Control.h"
	#include "strategicmap.h"
	#include "Tactical Save.h"
	#include "Overhead.h"
	#include "english.h"
	#include "Multi Language Graphic Utils.h"
	#include "GameSettings.h"
	#include "Radar Screen.h"
	#include "message.h"
	#include "weapons.h"
	#include "finances.h"
	#include "Game Clock.h"
	#include "Sound Control.h"
	#include "interface panels.h"
	#include "wordwrap.h"
#endif

#include "ShopKeeper Interface.h"
#include "ArmsDealerInvInit.h"
#include <algorithm>


//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


extern BOOLEAN SaveWorldItemsToTempItemFile( INT16 sMapX, INT16 sMapY, INT8 bMapZ, UINT32 uiNumberOfItems, WORLDITEM* pData );
#define MAP_INV_X_OFFSET							(((SCREEN_WIDTH - 261) - 380) / 2)
//#define MAP_INV_Y_OFFSET					(((SCREEN_HEIGHT - 121) -

// status bar colors
#define DESC_STATUS_BAR FROMRGB( 201, 172,  133 )
#define DESC_STATUS_BAR_SHADOW FROMRGB( 140, 136,  119 )

// page display positions
#define MAP_INVENTORY_POOL_PAGE_X							(MAP_INV_X_OFFSET + 506)
#define MAP_INVENTORY_POOL_PAGE_Y							(SCREEN_HEIGHT - 121 - 23)	//336
#define MAP_INVENTORY_POOL_PAGE_WIDTH					46
#define MAP_INVENTORY_POOL_PAGE_HEIGHT				13

// the number of items
#define MAP_INVENTORY_POOL_NUMBER_X						(MAP_INV_X_OFFSET + 436)
#define MAP_INVENTORY_POOL_NUMBER_WIDTH				40

// location
#define MAP_INVENTORY_POOL_LOC_X							MAP_INV_X_OFFSET + 326
#define MAP_INVENTORY_POOL_LOC_WIDTH					40

// delay for flash of item
#define DELAY_FOR_HIGHLIGHT_ITEM_FLASH				200

// inventory slot font
#define MAP_IVEN_FONT						SMALLCOMPFONT

// the position of the background graphic
#define INVEN_POOL_X 261
#define INVEN_POOL_Y 0

// inventory Graphic Offset X and y
#define MAP_INVENTORY_POOL_SLOT_OFFSET_X 2
#define MAP_INVENTORY_POOL_SLOT_OFFSET_Y 5

// height of map inventory pool bar
#define ITEMDESC_ITEM_STATUS_HEIGHT_INV_POOL 20
#define ITEMDESC_ITEM_STATUS_WIDTH_INV_POOL 2

// map bar offsets
#define ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_X 5
#define ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y 22

// inventory pool slot positions and sizes
//#define MAP_INVENTORY_POOL_SLOT_START_X 271
//#define MAP_INVENTORY_POOL_SLOT_START_Y 36
#define MAP_INVEN_SLOT_WIDTH 65
#define MAP_INVEN_SPACE_BTWN_SLOTS 72
#define MAP_INVEN_SLOT_HEIGHT 32
#define MAP_INVEN_SLOT_IMAGE_HEIGHT 24

// Number of inventory slots in 1024x768
#define MAP_INVENTORY_POOL_MAX_SLOTS 170

INT32 MAP_INV_SLOT_COLS;			// Number of vertical slots
INT32 MAP_INVENTORY_POOL_SLOT_COUNT;

INT32 MAP_INVENTORY_POOL_SLOT_START_X;
INT32 MAP_INVENTORY_POOL_SLOT_START_Y;

// the current highlighted item
INT32 iCurrentlyHighLightedItem = -1;
BOOLEAN fFlashHighLightInventoryItemOnradarMap = FALSE;

// whether we are showing the inventory pool graphic
BOOLEAN fShowMapInventoryPool = FALSE;

// the v-object index value for the background
UINT32 guiMapInventoryPoolBackground;

// inventory pool list
std::vector<WORLDITEM> pInventoryPoolList;

// current page of inventory
INT32 iCurrentInventoryPoolPage = 0;
INT32 iLastInventoryPoolPage = 0;

INT16 sObjectSourceGridNo = 0;
INT8  sObjectSourseSoldierID = -1;

// number of unseen items in sector
UINT32 uiNumberOfUnSeenItems = 0;


// the inventory slots
//MOUSE_REGION MapInventoryPoolSlots[ MAP_INVENTORY_POOL_SLOT_COUNT ];
MOUSE_REGION MapInventoryPoolSlots[ MAP_INVENTORY_POOL_MAX_SLOTS ];

MOUSE_REGION MapInventoryPoolMask;
//BOOLEAN fMapInventoryItemCompatable[ MAP_INVENTORY_POOL_SLOT_COUNT ];
BOOLEAN fMapInventoryItemCompatable[ MAP_INVENTORY_POOL_MAX_SLOTS ];

BOOLEAN fChangedInventorySlots = FALSE;

// the unseen items list...have to save this
WORLDITEM *pUnSeenItems = NULL;

// save list to write to temp file
WORLDITEM *pSaveList = NULL;

INT32 giFlashHighlightedItemBaseTime = 0;
INT32 giCompatibleItemBaseTime = 0;

// the buttons and images
UINT32 guiMapInvenButtonImage[ 3 ];
UINT32 guiMapInvenButton[ 3 ];

BOOLEAN gfCheckForCursorOverMapSectorInventoryItem = FALSE;


extern BOOLEAN fShowInventoryFlag;
extern BOOLEAN fMapScreenBottomDirty;

extern BOOLEAN ReduceStringLength( STR16 pString, UINT32 uiWidthToFitIn, UINT32 uiFont );

extern OBJECTTYPE gItemPointer;

extern OBJECTTYPE * gpItemPointer;

// outside vidieo objects for cursor
extern UINT32 guiExternVo;
extern UINT16 gusExternVoSubIndex;

extern	MOUSE_REGION    gMPanelRegion;



// map inventory callback
void MapInvenPoolSlots(MOUSE_REGION * pRegion, INT32 iReason );
void MapInvenPoolSlotsMove( MOUSE_REGION * pRegion, INT32 iReason  );
void CreateMapInventoryPoolSlots( void );
void DestroyMapInventoryPoolSlots( void );
void CreateMapInventoryButtons( void );
void DestroyMapInventoryButtons( void );
void DestroyStash( void );
void BuildStashForSelectedSector( INT16 sMapX, INT16 sMapY, INT16 sMapZ );
void BeginInventoryPoolPtr( OBJECTTYPE *pInventorySlot );
BOOLEAN PlaceObjectInInventoryStash( OBJECTTYPE *pInventorySlot, OBJECTTYPE *pItemPtr );
void RenderItemsForCurrentPageOfInventoryPool( void );
BOOLEAN RenderItemInPoolSlot( INT32 iCurrentSlot, INT32 iFirstSlotOnPage );
void UpdateHelpTextForInvnentoryStashSlots( void );
void MapInventoryPoolPrevBtn( GUI_BUTTON *btn, INT32 reason );
void MapInventoryPoolNextBtn( GUI_BUTTON *btn, INT32 reason );
void DisplayPagesForMapInventoryPool( void );
void DrawNumberOfIventoryPoolItems( void );
void CreateMapInventoryPoolDoneButton( void );
void DestroyInventoryPoolDoneButton( void );
void MapInventoryPoolDoneBtn( GUI_BUTTON *btn, INT32 reason );
void DisplayCurrentSector( void );
void ResizeInventoryList( void );
void ClearUpTempUnSeenList( void );
void SaveSeenAndUnseenItems( void );
void DrawTextOnMapInventoryBackground( void );
void DrawTextOnSectorInventory( void );
INT32 GetTotalNumberOfItemsInSectorStash( void );
void HandleMapSectorInventory( void );
void ResetMapSectorInventoryPoolHighLights( void );
void ReBuildWorldItemStashForLoadedSector( INT32 iNumberSeenItems, INT32 iNumberUnSeenItems, WORLDITEM *pSeenItemsList, WORLDITEM *pUnSeenItemsList );
BOOLEAN IsMapScreenWorldItemVisibleInMapInventory( WORLDITEM *pWorldItem );
BOOLEAN IsMapScreenWorldItemInvisibleInMapInventory( WORLDITEM *pWorldItem );
void CheckGridNoOfItemsInMapScreenMapInventory();
INT32 MapScreenSectorInventoryCompare( const void *pNum1, const void *pNum2);
void SortSectorInventory( std::vector<WORLDITEM>& pInventory, UINT32 uiSizeOfArray );
BOOLEAN CanPlayerUseSectorInventory( SOLDIERTYPE *pSelectedSoldier );

extern void MAPEndItemPointer( );
extern	BOOLEAN GetCurrentBattleSectorXYZAndReturnTRUEIfThereIsABattle( INT16 *psSectorX, INT16 *psSectorY, INT16 *psSectorZ );

void DeleteAllItemsInInventoryPool();
void DeleteItemsOfType( UINT16 usItemType );
INT32 SellItem( OBJECTTYPE& object, BOOLEAN useModifier = TRUE );

// load the background panel graphics for inventory
BOOLEAN LoadInventoryPoolGraphic( void )
{
	VOBJECT_DESC    VObjectDesc;

	// load the file
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution == 0)
	{
		MAP_INV_SLOT_COLS = 8;
		MAP_INVENTORY_POOL_SLOT_COUNT = 40;
		MAP_INVENTORY_POOL_SLOT_START_X = 269;
	  MAP_INVENTORY_POOL_SLOT_START_Y = 51;
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory.sti" );
	}
	else if (iResolution == 1)
	{
		MAP_INV_SLOT_COLS = 11;
		MAP_INVENTORY_POOL_SLOT_COUNT = 77;
		MAP_INVENTORY_POOL_SLOT_START_X = 278;
	  MAP_INVENTORY_POOL_SLOT_START_Y = 62;
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory_800x600.sti" );
	}
	else if (iResolution == 2)
	{
		MAP_INV_SLOT_COLS = 17;
		MAP_INVENTORY_POOL_SLOT_COUNT = MAP_INVENTORY_POOL_MAX_SLOTS;
		MAP_INVENTORY_POOL_SLOT_START_X = 282;
		MAP_INVENTORY_POOL_SLOT_START_Y = 50;
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory_1024x768.sti" );
	}

	// add to V-object index
	CHECKF(AddVideoObject(&VObjectDesc, &guiMapInventoryPoolBackground));

	return( TRUE );
}



// remove background panel graphics for inventory
void RemoveInventoryPoolGraphic( void )
{
	// remove from v-object index
	if( guiMapInventoryPoolBackground )
	{
		DeleteVideoObjectFromIndex( guiMapInventoryPoolBackground );
		guiMapInventoryPoolBackground = 0;
	}

	return;
}

// blit the background panel for the inventory
void BlitInventoryPoolGraphic( void )
{
	HVOBJECT hHandle;

	// blit inventory pool graphic to the screen
	GetVideoObject(&hHandle, guiMapInventoryPoolBackground);
	BltVideoObject( guiSAVEBUFFER , hHandle, 0,INVEN_POOL_X, INVEN_POOL_Y , VO_BLT_SRCTRANSPARENCY,NULL );

	// resize list
	ResizeInventoryList( );


	// now the items
	RenderItemsForCurrentPageOfInventoryPool( );

	// now update help text
	UpdateHelpTextForInvnentoryStashSlots( );

	// show which page and last page
	DisplayPagesForMapInventoryPool( );

	// draw number of items in current inventory
	DrawNumberOfIventoryPoolItems( );

	// display current sector inventory pool is at
	DisplayCurrentSector( );

	DrawTextOnMapInventoryBackground( );

	// re render buttons
	MarkButtonsDirty( );

	// which buttons will be active and which ones not
	HandleButtonStatesWhileMapInventoryActive( );

	// Invalidate
	RestoreExternBackgroundRect(MAP_BORDER_X, MAP_BORDER_Y, SCREEN_WIDTH - MAP_BORDER_X, SCREEN_HEIGHT - 121);

	return;
}

void RenderItemsForCurrentPageOfInventoryPool( void )
{
	INT32 iCounter = 0;

	// go through list of items on this page and place graphics to screen
	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT ; iCounter++ )
	{
		RenderItemInPoolSlot( iCounter, ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) );
	}

	return;
}


BOOLEAN RenderItemInPoolSlot( INT32 iCurrentSlot, INT32 iFirstSlotOnPage )
{
	// render item in this slot of the list
	INT16 sCenX, sCenY, usWidth, usHeight, sX, sY;
	HVOBJECT hHandle;
	ETRLEObject		*pTrav;
	CHAR16 sString[ 64 ];
	INT16 sWidth = 0, sHeight = 0;
	INT16 sOutLine = 0;
	BOOLEAN fOutLine = FALSE;

	// check if anything there

	if( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == false )
	{
		return ( FALSE );
	}

	GetVideoObject( &hHandle, GetInterfaceGraphicForItem( &(Item[ pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ] ) ) );

	pTrav = &( hHandle->pETRLEObject[ Item[pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ].ubGraphicNum] );
	usHeight				= (UINT16)pTrav->usHeight;
	usWidth					= (UINT16)pTrav->usWidth;

	// set sx and sy
	sX = ( INT16 )( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCurrentSlot / MAP_INV_SLOT_COLS ) ) );
	sY = 	( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCurrentSlot % ( MAP_INV_SLOT_COLS ) ) ) );

	// CENTER IN SLOT!
	sCenX = sX + ( abs( MAP_INVEN_SPACE_BTWN_SLOTS - usWidth ) / 2 ) - pTrav->sOffsetX;
	sCenY = sY + ( abs( MAP_INVEN_SLOT_HEIGHT - 5 - usHeight ) / 2 ) - pTrav->sOffsetY;


	if( fMapInventoryItemCompatable[ iCurrentSlot ] )
	{
		sOutLine = Get16BPPColor( FROMRGB( 255, 255, 255 ) );
    fOutLine = TRUE;
	}
	else
	{
		sOutLine = us16BPPItemCyclePlacedItemColors[ 0 ];
    fOutLine = FALSE;
	}

	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

  INVRenderItem( guiSAVEBUFFER, NULL, &(pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object),
                 (INT16)(sX + 7), sY, 60, 25, DIRTYLEVEL2, NULL, 0, fOutLine, sOutLine );//67

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );


	// now blit this object in the box
	//BltVideoObjectOutlineFromIndex( guiSAVEBUFFER, GetInterfaceGraphicForItem( &(Item[ pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ]) ),
	//	Item[ pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ].ubGraphicNum,
	//	sCenX, sCenY,
	 //sOutLine, TRUE );




	// now draw bar for condition
	// Display ststus
	DrawItemUIBarEx( &( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object ), 0,
		(INT16)( ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCurrentSlot / MAP_INV_SLOT_COLS ) ) ),
		( INT16 )( ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y + MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCurrentSlot % ( MAP_INV_SLOT_COLS ) ) ) )
		, ITEMDESC_ITEM_STATUS_WIDTH_INV_POOL, ITEMDESC_ITEM_STATUS_HEIGHT_INV_POOL, 	Get16BPPColor( DESC_STATUS_BAR ), Get16BPPColor( DESC_STATUS_BAR_SHADOW ), TRUE, guiSAVEBUFFER );


	//
	// if the item is not reachable, or if the selected merc is not in the current sector
	//
	if( !( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].usFlags & WORLD_ITEM_REACHABLE ) ||
			!(( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX == sSelMapX ) &&
				( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY == sSelMapY ) &&
				( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ == iCurrentMapSectorZ )
			) )
	{
		//Shade the item, but only if it is an active item!
		if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == true) {
			DrawHatchOnInventory( guiSAVEBUFFER, sX, sY, MAP_INVEN_SLOT_WIDTH, MAP_INVEN_SLOT_IMAGE_HEIGHT );
		}
	}


	// the name

	wcscpy( sString, ShortItemNames[ pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ] );

	if( StringPixLength( sString, MAP_IVEN_FONT ) >= ( MAP_INVEN_SLOT_WIDTH ) )
	{
		ReduceStringLength( sString, ( INT16 )( MAP_INVEN_SLOT_WIDTH - StringPixLength( L" ...", MAP_IVEN_FONT ) ), MAP_IVEN_FONT );
	}

	FindFontCenterCoordinates( (INT16)( 4 + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCurrentSlot / MAP_INV_SLOT_COLS ) ) ),
		0, MAP_INVEN_SLOT_WIDTH, 0,
		sString, MAP_IVEN_FONT,
		&sWidth, &sHeight );

	SetFontDestBuffer( guiSAVEBUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	SetFont( MAP_IVEN_FONT );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	mprintf( sWidth,
		( INT16 )( 3 + ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y + MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCurrentSlot % ( MAP_INV_SLOT_COLS ) ) ) )
	, sString );

/*
	if( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.ubNumberOfObjects > 1 )
	{
		swprintf( sString, L"x%d",  pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.ubNumberOfObjects );

		// find font right coord
		FindFontRightCoordinates( ( INT16 )( ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X - 1 + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCurrentSlot / MAP_INV_SLOT_COLS ) ) ),0, MAP_INVEN_SPACE_BTWN_SLOTS - 10, 0, sString, MAP_IVEN_FONT, &sX, &sY );

		sY = ( INT16 )( 3 + ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y + MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCurrentSlot % ( MAP_INV_SLOT_COLS ) ) ) ) - 7;

		// print string
		mprintf( sX, sY, sString );
	}
*/

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	return( TRUE );
}


void UpdateHelpTextForInvnentoryStashSlots( void )
{
	CHAR16 pStr[ 512 ];
	INT32 iCounter = 0;
	INT32 iFirstSlotOnPage = ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT );


	// run through list of items in slots and update help text for mouse regions
	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
	{
		if( pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object.exists() == true )
		{
			GetHelpTextForItem( pStr , &( pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object ), NULL );
			SetRegionFastHelpText( &(MapInventoryPoolSlots[ iCounter ] ), pStr );

			/*
			// set text for current item
			if( pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object.usItem == MONEY )
			{
				swprintf( pStr, L"$%ld", pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object[0]->data.money.uiMoneyAmount );
				SetRegionFastHelpText( &(MapInventoryPoolSlots[ iCounter ]), pStr );
			}
			else
			{
				SetRegionFastHelpText( &(MapInventoryPoolSlots[ iCounter ]), ItemNames[ pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object.usItem ] );
			}
			*/
		}
		else
		{
			//OK, for each item, set dirty text if applicable!
			SetRegionFastHelpText( &(MapInventoryPoolSlots[ iCounter ]), L"" );
		}
	}

	return;
}



// create and remove buttons for inventory
void CreateDestroyMapInventoryPoolButtons( BOOLEAN fExitFromMapScreen )
{
	static BOOLEAN fCreated = FALSE;

/* player can leave items underground, no?
	if( iCurrentMapSectorZ )
	{
		fShowMapInventoryPool = FALSE;
	}
*/

	if( ( fShowMapInventoryPool ) && ( fCreated == FALSE ) )
	{
		if( ( gWorldSectorX == sSelMapX ) && ( gWorldSectorY == sSelMapY ) && ( gbWorldSectorZ == iCurrentMapSectorZ ) )
		{
			// handle all reachable before save
			HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}

		// destroy buttons for map border
		DeleteMapBorderButtons( );

		fCreated = TRUE;

		// also create the inventory slot
		CreateMapInventoryPoolSlots( );

		// create buttons
		CreateMapInventoryButtons( );

		// build stash
		BuildStashForSelectedSector( sSelMapX, sSelMapY, ( INT16 )( iCurrentMapSectorZ ) );

		CreateMapInventoryPoolDoneButton( );

		fMapPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
	else if( ( fShowMapInventoryPool == FALSE ) && ( fCreated == TRUE ) )
	{

		// check fi we are in fact leaving mapscreen
		if( fExitFromMapScreen == FALSE )
		{
			// recreate mapborder buttons
			CreateButtonsForMapBorder( );
		}
		fCreated = FALSE;

		// destroy the map inventory slots
		DestroyMapInventoryPoolSlots( );

		// destroy map inventory buttons
		DestroyMapInventoryButtons( );

		DestroyInventoryPoolDoneButton( );

		// clear up unseen list
		ClearUpTempUnSeenList( );

		// now save results
		SaveSeenAndUnseenItems( );

		DestroyStash( );



		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;

		//DEF: added to remove the 'item blip' from staying on the radar map
		iCurrentlyHighLightedItem = -1;

		// re render radar map
		RenderRadarScreen( );
	}

	// do our handling here
	HandleMapSectorInventory( );

}


void CancelSectorInventoryDisplayIfOn( BOOLEAN fExitFromMapScreen )
{
	if ( fShowMapInventoryPool )
	{
		// get rid of sector inventory mode & buttons
		fShowMapInventoryPool = FALSE;
		CreateDestroyMapInventoryPoolButtons( fExitFromMapScreen );
	}
}



void ClearUpTempUnSeenList( void )
{
	// save these items and all the others
	if( pUnSeenItems == NULL )
	{
		return;
	}

	// build the list based on this
	pSaveList = pUnSeenItems;
	pUnSeenItems = NULL;

	return;
}

void SaveSeenAndUnseenItems( void )
{
	WORLDITEM *pSeenItemsList = NULL;
	UINT32 iCounter = 0;
	INT32 iItemCount = 0;
	INT32 iTotalNumberItems = 0;

	// allocate space
	iTotalNumberItems = GetTotalNumberOfItems( );

	// if there are seen items, build a temp world items list of them and save them
	if( iTotalNumberItems > 0 )
	{
		pSeenItemsList = new WORLDITEM[ iTotalNumberItems ];

		// copy
		for( iCounter = 0; iCounter < pInventoryPoolList.size(); iCounter++ )
		{
			if( pInventoryPoolList[ iCounter ].object.exists() == true )
			{
				// copy object stuff
				pSeenItemsList[ iItemCount ] = pInventoryPoolList[ iCounter ];

				// check if item actually lives at a gridno
				// if not, check predicessor, iItemCount is not 0
				if( pSeenItemsList[ iItemCount ].sGridNo == 0 )
				{
					if( iItemCount > 0 )
					{
						// borrow from predicessor
						pSeenItemsList[ iItemCount ].sGridNo = pSeenItemsList[ iItemCount - 1 ].sGridNo;
					}
					else
					{
						// get entry grid location
					}
				}
				pSeenItemsList[ iItemCount ].fExists = TRUE;
				pSeenItemsList[ iItemCount ].bVisible = TRUE;
				iItemCount++;
			}
		}
	}

	// if this is the loaded sector handle here
	if( ( gWorldSectorX == sSelMapX ) && ( gWorldSectorY == sSelMapY ) && ( gbWorldSectorZ == ( INT8 ) ( iCurrentMapSectorZ ) ) )
	{
		ReBuildWorldItemStashForLoadedSector( iItemCount, uiNumberOfUnSeenItems, pSeenItemsList, pSaveList );
	}
	else
	{
		// now copy over unseen and seen
		if( uiNumberOfUnSeenItems > 0 )
		{
			// over write file and copy unseen
			AddWorldItemsToUnLoadedSector( sSelMapX, sSelMapY, ( INT8 )( iCurrentMapSectorZ ), 0, uiNumberOfUnSeenItems, pSaveList, TRUE );

			// check if seen items exist too
			if( iItemCount > 0 )
			{
				AddWorldItemsToUnLoadedSector( sSelMapX, sSelMapY, ( INT8 )( iCurrentMapSectorZ ), 0, iItemCount, pSeenItemsList, FALSE );
			}

		}
		else if( iItemCount > 0 )
		{
			// copy only seen items
			AddWorldItemsToUnLoadedSector( sSelMapX, sSelMapY, ( INT8 )( iCurrentMapSectorZ ), 0, iItemCount, pSeenItemsList, TRUE );
		}
		else
		{
			// get rid of the file
			SaveWorldItemsToTempItemFile( sSelMapX, sSelMapY, ( INT8 )( iCurrentMapSectorZ ), 0, NULL );
			return;
		}
	}

	// now clear out seen list
	if( pSeenItemsList != NULL )
	{
		delete[]( pSeenItemsList );
		pSeenItemsList = NULL;
	}

	// clear out unseen list
	if( pSaveList != NULL )
	{
		delete[]( pSaveList );
		pSaveList = NULL;
	}

	uiNumberOfUnSeenItems = 0;
	iItemCount = 0;
}



// the screen mask bttn callaback...to disable the inventory and lock out the map itself
void MapInvenPoolScreenMaskCallback(MOUSE_REGION * pRegion, INT32 iReason )
{

	if( ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		fShowMapInventoryPool = FALSE;
	}

	return;
}


void CreateMapInventoryPoolSlots( void )
{
	INT32 iCounter = 0;
	INT16 sX = 0, sY = 0;
	INT16 sXA = 0, sYA = 0;
	INT16 sULX = 0, sULY = 0;
	INT16 sBRX = 0, sBRY = 0;

	MSYS_DefineRegion( &MapInventoryPoolMask,
			MAP_INVENTORY_POOL_SLOT_START_X, 0, SCREEN_WIDTH - MAP_INVENTORY_POOL_SLOT_START_X, SCREEN_HEIGHT - 120,
			MSYS_PRIORITY_HIGH, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MapInvenPoolScreenMaskCallback);

	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
	{
		sX = ( iCounter / MAP_INV_SLOT_COLS );
		sY = ( iCounter % ( MAP_INV_SLOT_COLS ) );

		sXA = sX + 1;
		sYA = sY + 1;

		sULX = MAP_INVENTORY_POOL_SLOT_START_X + 4;
		sULY = MAP_INVENTORY_POOL_SLOT_START_Y + 1;

		sULX += ( INT16 ) ( sX * MAP_INVEN_SPACE_BTWN_SLOTS  );
		sULY += ( INT16 ) ( ( sY * MAP_INVEN_SLOT_HEIGHT ) );

		sBRX = ( INT16 ) ( MAP_INVENTORY_POOL_SLOT_START_X + ( sXA * MAP_INVEN_SPACE_BTWN_SLOTS ) );
		sBRY = ( INT16 ) ( MAP_INVENTORY_POOL_SLOT_START_Y + ( sYA * MAP_INVEN_SLOT_HEIGHT ) ) - 1;

		MSYS_DefineRegion( &MapInventoryPoolSlots[ iCounter ],
			sULX, sULY, sBRX, sBRY,
			MSYS_PRIORITY_HIGH, MSYS_NO_CURSOR, MapInvenPoolSlotsMove, MapInvenPoolSlots );

		MSYS_SetRegionUserData( &MapInventoryPoolSlots[iCounter], 0, iCounter );

	}
}


void DestroyMapInventoryPoolSlots( void )
{
	INT32 iCounter = 0;

	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
	{
		MSYS_RemoveRegion(  &MapInventoryPoolSlots[ iCounter ] );
	}

	// remove map inventory mask
	MSYS_RemoveRegion( &MapInventoryPoolMask );

}

void MapInvenPoolSlotsMove( MOUSE_REGION * pRegion, INT32 iReason  )
{
	INT32 iCounter = 0;


	iCounter = MSYS_GetRegionUserData( pRegion, 0 );

	if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		iCurrentlyHighLightedItem = iCounter;
		fChangedInventorySlots = TRUE;
		gfCheckForCursorOverMapSectorInventoryItem = TRUE;
	}
	else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		iCurrentlyHighLightedItem = -1;
		fChangedInventorySlots = TRUE;
		gfCheckForCursorOverMapSectorInventoryItem = FALSE;

		// re render radar map
		RenderRadarScreen( );
	}
}


void MapInvenPoolSlots(MOUSE_REGION * pRegion, INT32 iReason )
{
	// btn callback handler for assignment screen mask region
	INT32 iCounter = 0;
	UINT16 usOldItemIndex, usNewItemIndex;
	INT32 iOldNumberOfObjects = 0;
	INT16 sDistanceFromObject = 0;
	SOLDIERTYPE *pSoldier = NULL;
	CHAR16 sString[ 128 ];

	iCounter = MSYS_GetRegionUserData( pRegion, 0 );

	if( ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		if ( gpItemPointer == NULL )
		{
			fShowMapInventoryPool = FALSE;
		}
		// else do nothing
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// check if item in cursor, if so, then swap, and no item in curor, pick up, if item in cursor but not box, put in box

		if ( gpItemPointer == NULL )
		{
			// Return if empty
			if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.exists() == false )
				return;
		}



		// is this item reachable
		if( !(  pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_REACHABLE ) )
		{
			if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.exists() == true )
			{
				// not reachable
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, gzLateLocalizedString[38], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
				return;
			}
		}


		// check if selected merc is in this sector, if not, warn them and leave

		// valid character?
		if( gCharactersList[ bSelectedInfoChar ].fValid == FALSE )
		{
			DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapInventoryErrorString[ 1 ], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
			return;
		}



		//if( fShowInventoryFlag )
		{
			// not in sector?
			if( ( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorX != sSelMapX ) ||
					( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].sSectorY != sSelMapY ) ||
					( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].bSectorZ != iCurrentMapSectorZ ) ||
					( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].flags.fBetweenSectors ) )
			{
				if ( gpItemPointer == NULL )
				{
					swprintf( sString, pMapInventoryErrorString[ 2 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
				}
				else
				{
					swprintf( sString, pMapInventoryErrorString[ 5 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
				}
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
				return;
			}
		}


		// If we do not have an item in hand, start moving it
		if ( gpItemPointer == NULL )
		{


			// Return if empty
			if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.exists() == false )
				return;

			// if in battle inform player they will have to do this in tactical
//			if( ( ( gTacticalStatus.fEnemyInSector ) ||( ( sSelMapX == gWorldSectorX ) && ( sSelMapY == gWorldSectorY ) && ( iCurrentMapSectorZ == gbWorldSectorZ ) && ( gTacticalStatus.uiFlags & INCOMBAT ) ) ) )
			if( !CanPlayerUseSectorInventory( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] ) )
			{
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapInventoryErrorString[ 3 ], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
				return;
			}

			sObjectSourceGridNo = pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].sGridNo;
			sObjectSourseSoldierID = pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].soldierID;

			// check if this is the loaded sector, if so, then notify player, can't do anything
			if( ( sSelMapX == gWorldSectorX )&&( gWorldSectorY == sSelMapY ) &&(gbWorldSectorZ == iCurrentMapSectorZ ) )
			{
				// notify
				pSoldier = &( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] );

				sDistanceFromObject = PythSpacesAway( sObjectSourceGridNo, pSoldier->sGridNo);

			/*	if( sDistanceFromObject > MAX_DISTANCE_TO_PICKUP_ITEM )
				{
					// see for the loaded sector if the merc is cloase enough?
					swprintf( sString, pMapInventoryErrorString[ 0 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
					DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
					return;
				}
				*/
			}

			BeginInventoryPoolPtr( &( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object ) );
		}
		else
		{

			// if in battle inform player they will have to do this in tactical
//			if( ( gTacticalStatus.fEnemyInSector ) ||( ( sSelMapX == gWorldSectorX ) && ( sSelMapY == gWorldSectorY ) && ( iCurrentMapSectorZ == gbWorldSectorZ ) && ( gTacticalStatus.uiFlags & INCOMBAT ) ) )
			if( !CanPlayerUseSectorInventory( &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] ) )
			{
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, pMapInventoryErrorString[ 4 ], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
				return;
			}

			usOldItemIndex = pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.usItem;
			usNewItemIndex = gpItemPointer->usItem;
			iOldNumberOfObjects =  pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.ubNumberOfObjects;


			// Else, try to place here
			if ( PlaceObjectInInventoryStash( &( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object ), gpItemPointer ) )
			{

				// set as reachable and set gridno
				pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_REACHABLE;

				// if loaded sector, grab grid no of dropping soldier
				//if( ( sSelMapX == gWorldSectorX )&&( gWorldSectorY == sSelMapY ) &&(gbWorldSectorZ == iCurrentMapSectorZ ) )
				//{
					// nothing here before, then place here
					if( iOldNumberOfObjects == 0 )
					{
						if( sObjectSourceGridNo == NOWHERE )
						{
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].sGridNo = sObjectSourceGridNo;
						}
						else
						{
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].sGridNo = sObjectSourceGridNo;
						}
						if( sObjectSourseSoldierID != -1 )
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].soldierID = sObjectSourseSoldierID;
					}
				//}

				// Check if it's the same now!
				if ( gpItemPointer->exists() == false )
				{
					MAPEndItemPointer( );
				}
				else
				{
					// update ptr
					// now set the cursor
					guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
					gusExternVoSubIndex = Item[ gpItemPointer->usItem ].ubGraphicNum;

					fMapInventoryItem = TRUE;
					MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
					SetCurrentCursorFromDatabase( EXTERN_CURSOR );
				}

/*
				if ( fShowInventoryFlag && bSelectedInfoChar >= 0 )
				{
					ReevaluateItemHatches( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ], FALSE );
				}
				*/
			}
		}

		// dirty region, force update
		fMapPanelDirty = TRUE;

	}
}

void CreateMapInventoryButtons( void )
{
	guiMapInvenButtonImage[ 0 ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" , 10, 1, -1, 3, -1 );
  guiMapInvenButton[ 0 ] = QuickCreateButton( guiMapInvenButtonImage[ 0 ], (MAP_INV_X_OFFSET + 559), (SCREEN_HEIGHT - 144),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapInventoryPoolNextBtn );


	guiMapInvenButtonImage[ 1 ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,9, 0, -1, 2, -1 );
  guiMapInvenButton[ 1 ] = QuickCreateButton( guiMapInvenButtonImage[ 1 ], (MAP_INV_X_OFFSET + 487), (SCREEN_HEIGHT - 144),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapInventoryPoolPrevBtn );


	//reset the current inventory page to be the first page
	iCurrentInventoryPoolPage = 0;

	return;
}


void DestroyMapInventoryButtons( void )
{


	RemoveButton( guiMapInvenButton[ 0 ] );
	RemoveButton( guiMapInvenButton[ 1 ] );

	UnloadButtonImage( guiMapInvenButtonImage[ 0 ] );
	UnloadButtonImage( guiMapInvenButtonImage[ 1 ] );
	return;
}


void BuildStashForSelectedSector( INT16 sMapX, INT16 sMapY, INT16 sMapZ )
{
	INT32 iSize = 0;
	UINT32 uiItemCount = 0;
	UINT32 uiTotalNumberOfItems = 0, uiTotalNumberOfRealItems = 0;
	WORLDITEM * pTotalSectorList = NULL;
	INT32 iCounter = 0;
	UINT32	uiTotalNumberOfSeenItems=0;

//	#ifdef _DEBUG
		BOOLEAN fReturn = TRUE;
//	#endif

	// get size of the current stash in sector (count stacks as one item)
	iSize = GetSizeOfStashInSector( sMapX, sMapY, sMapZ, TRUE );

	// round off .. we want at least 1 free page of space...
	iSize = ((iSize / MAP_INVENTORY_POOL_SLOT_COUNT) + 1) * MAP_INVENTORY_POOL_SLOT_COUNT;

	// allocate space for list
	pInventoryPoolList.clear();
	pInventoryPoolList.resize( iSize );

	iLastInventoryPoolPage = ( ( iSize - 1 ) / MAP_INVENTORY_POOL_SLOT_COUNT );


	uiNumberOfUnSeenItems = 0;

	// now laod these items into memory, based on fact if sector is in fact loaded
	if( ( sMapX == gWorldSectorX )&&( gWorldSectorY == sMapY ) &&(gbWorldSectorZ == sMapZ ) )
	{
		// sector loaded, just copy from list
		for( iCounter = 0; ( UINT32 )( iCounter ) < guiNumWorldItems; iCounter++ )
		{
			// check if visible, if so, then copy over object type
			// if visible to player, then state fact

/*
			if( gWorldItems[ iCounter].bVisible == 1 &&
					gWorldItems[ iCounter ].fExists &&
					gWorldItems[ iCounter ].object.usItem != SWITCH &&
					gWorldItems[ iCounter ].object.bTrap <= 0 )
*/
			if( IsMapScreenWorldItemVisibleInMapInventory( &gWorldItems[ iCounter ] ) )
			{
				// one more item
				pInventoryPoolList[ uiItemCount ] = gWorldItems[ iCounter ];
				uiItemCount++;
			}
		}


		uiTotalNumberOfSeenItems = uiItemCount;


			// now allocate space for all the unseen items
		if( guiNumWorldItems > uiItemCount )
		{
			pUnSeenItems = new WORLDITEM[ guiNumWorldItems - uiItemCount ];

			uiItemCount = 0;

			// now copy over
			for( iCounter = 0; ( UINT32 )iCounter < guiNumWorldItems; iCounter++ )
			{
				if( IsMapScreenWorldItemInvisibleInMapInventory( &gWorldItems[ iCounter ] ) )
				{
					// one more item
					pUnSeenItems[ uiItemCount ] = gWorldItems[ iCounter ];

					uiItemCount++;
				}
			}

			// copy over number of unseen
			uiNumberOfUnSeenItems = uiItemCount;
		}
	}
	else
	{
		// not loaded, load
		// get total number, visable and invisible
		fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( uiTotalNumberOfItems ), FALSE );
		Assert( fReturn );

		fReturn = GetNumberOfActiveWorldItemsFromTempFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( uiTotalNumberOfRealItems ) );
		Assert( fReturn );

		if( uiTotalNumberOfRealItems > 0 )
		{
			// allocate space for the list
			pTotalSectorList = new WORLDITEM[ uiTotalNumberOfItems ];


			// now load into mem
			LoadWorldItemsFromTempItemFile(  sMapX,  sMapY, ( INT8 ) ( sMapZ ), pTotalSectorList );

		}


		// now run through list and
		for( iCounter = 0; ( UINT32 )( iCounter )< uiTotalNumberOfRealItems; iCounter++ )
		{
			// if visible to player, then state fact
/*
			if( pTotalSectorList[ iCounter].bVisible == 1 &&
					pTotalSectorList[ iCounter].fExists &&
					pTotalSectorList[ iCounter].object.usItem != SWITCH &&
					pTotalSectorList[ iCounter].object.bTrap <= 0 )
*/


			//TEST!!  If the item exists, and is NOT VALID, report it
			if( pTotalSectorList[ iCounter].fExists &&  pTotalSectorList[ iCounter].object.usItem > MAXITEMS )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"The %d item in the list is NOT valid. Please send save.  DF 1.", iCounter );
			}


			if( IsMapScreenWorldItemVisibleInMapInventory( &pTotalSectorList[ iCounter] ) )
			{
				// one more item
				pInventoryPoolList[ uiItemCount ] = pTotalSectorList[ iCounter ];

				uiItemCount++;
			}
		}

		uiTotalNumberOfSeenItems = uiItemCount;

		// now allocate space for all the unseen items
		if( uiTotalNumberOfRealItems > uiItemCount )
		{
			pUnSeenItems = new WORLDITEM[ uiTotalNumberOfRealItems - uiItemCount ];

			uiItemCount = 0;

			// now copy over
			for( iCounter = 0; ( UINT32 )iCounter < uiTotalNumberOfItems; iCounter++ )
			{
				if( IsMapScreenWorldItemInvisibleInMapInventory( &pTotalSectorList[ iCounter] ) )
				{
					// one more item
					pUnSeenItems[ uiItemCount ] = pTotalSectorList[ iCounter ];

					uiItemCount++;
				}
			}

			// copy over number of unseen
			uiNumberOfUnSeenItems = uiItemCount;
		}

		// if anything was alloced, then get rid of it
		if( uiTotalNumberOfRealItems > 0 )
		{
				delete[]( pTotalSectorList );
		}
	}

	//Check to see if any of the items in the list have a gridno of NOWHERE and the entry point flag NOT set
	CheckGridNoOfItemsInMapScreenMapInventory();

	//Sort the sector invenrtory
	SortSectorInventory( pInventoryPoolList, uiTotalNumberOfSeenItems );
}

void ReBuildWorldItemStashForLoadedSector( INT32 iNumberSeenItems, INT32 iNumberUnSeenItems, WORLDITEM *pSeenItemsList, WORLDITEM *pUnSeenItemsList )
{
	INT32 iTotalNumberOfItems = 0;
	INT32 iCurrentItem = 0;
	INT32 iCounter = 0;
	INT32 iRemainder = 0;
	UINT32	uiTotalNumberOfVisibleItems=0;
	WORLDITEM * pTotalList = NULL;

	// clear out the list
	TrashWorldItems( );

	// get total number of items
	iTotalNumberOfItems = iNumberUnSeenItems + iNumberSeenItems;

	iRemainder = iTotalNumberOfItems % 10;

	// if there is a remainder, then add onto end of list
	if( iRemainder )
	{
		iTotalNumberOfItems += 10 - iRemainder;
	}

	// allocate space for items
	pTotalList = new WORLDITEM[ iTotalNumberOfItems ];

	// place seen items in the world
	for( iCounter = 0; iCounter < iNumberSeenItems; iCounter++ )
	{
		pTotalList[ iCurrentItem ] = pSeenItemsList[ iCounter ];
		iCurrentItem++;
	}

	// now store the unseen item list
	for( iCounter = 0; iCounter < iNumberUnSeenItems; iCounter++ )
	{
		pTotalList[ iCurrentItem ] = pUnSeenItemsList[ iCounter ];
		iCurrentItem++;
	}

	RefreshItemPools( pTotalList, iTotalNumberOfItems );

	//Count the total number of visible items
	for( iCounter = 0; iCounter < iNumberSeenItems; iCounter++ )
	{
		uiTotalNumberOfVisibleItems += pSeenItemsList[ iCounter ].object.ubNumberOfObjects;
	}

	//reset the visible item count in the sector info struct
	SetNumberOfVisibleWorldItemsInSectorStructureForSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ , uiTotalNumberOfVisibleItems );

	// clear out allocated space for total list
	delete[]( pTotalList );

	// reset total list
	pTotalList = NULL;

	return;
}

void DestroyStash( void )
{
	// clear out stash
	pInventoryPoolList.clear();

}

INT32 GetSizeOfStashInSector( INT16 sMapX, INT16 sMapY, INT16 sMapZ, BOOLEAN fCountStacksAsOne )
{
	// get # of items in sector that are visible to the player
	UINT32 uiTotalNumberOfItems = 0, uiTotalNumberOfRealItems = 0;
	WORLDITEM * pTotalSectorList = NULL;
	UINT32 uiItemCount = 0;
	INT32 iCounter = 0;
	BOOLEAN fReturn = TRUE;

	if( ( sMapX == gWorldSectorX ) && ( sMapY == gWorldSectorY) && ( sMapZ == gbWorldSectorZ ) )
	{
		uiTotalNumberOfItems = guiNumWorldItems;

		// now run through list and
		for( iCounter = 0; ( UINT32 )( iCounter )< uiTotalNumberOfItems; iCounter++ )
		{
			// if visible to player, then state fact
//			if( gWorldItems[ iCounter ].bVisible == 1 && gWorldItems[ iCounter ].fExists )
			if( IsMapScreenWorldItemVisibleInMapInventory( &gWorldItems[ iCounter ] ) )
			{
				// add it
				if ( fCountStacksAsOne )
				{
					uiItemCount++;
				}
				else
				{
					uiItemCount += gWorldItems[ iCounter ].object.ubNumberOfObjects;
				}
			}
		}
	}
	else
	{
		// get total number, visable and invisible
		fReturn = GetNumberOfActiveWorldItemsFromTempFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( uiTotalNumberOfRealItems ) );
		Assert( fReturn );

		fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( uiTotalNumberOfItems ), FALSE );
		Assert( fReturn );

		if( uiTotalNumberOfItems > 0 )
		{
			// allocate space for the list
			pTotalSectorList = new WORLDITEM[ uiTotalNumberOfItems ];

				// now load into mem
			LoadWorldItemsFromTempItemFile(  sMapX,  sMapY, ( INT8 ) ( sMapZ ), pTotalSectorList );
		}

		// now run through list and
		for( iCounter = 0; ( UINT32 )( iCounter )< uiTotalNumberOfRealItems; iCounter++ )
		{
			// if visible to player, then state fact
//			if( pTotalSectorList[ iCounter ].bVisible == 1 && pTotalSectorList[ iCounter ].fExists )
			if( IsMapScreenWorldItemVisibleInMapInventory( &pTotalSectorList[ iCounter ] ) )
			{
				// add it
				if ( fCountStacksAsOne )
				{
					uiItemCount++;
				}
				else
				{
					uiItemCount += pTotalSectorList[ iCounter ].object.ubNumberOfObjects;
				}
			}
		}

		// if anything was alloced, then get rid of it
		if( pTotalSectorList != NULL )
		{
			delete[]( pTotalSectorList );
			pTotalSectorList = NULL;

			#ifdef JA2BETAVERSION
			if( uiTotalNumberOfItems == 0 )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"pTotalSectorList is NOT NULL when uiTotalNumberOfRealItems is %d.", uiTotalNumberOfRealItems );
			}
			#endif
		}
	}


	return( uiItemCount );
}



void BeginInventoryPoolPtr( OBJECTTYPE *pInventorySlot )
{
	BOOLEAN fOk = FALSE;
	BOOLEAN fSELLALL = gGameExternalOptions.fSellAll;

	// If not null return
	if ( gpItemPointer != NULL )
	{
		return;
	}

	// if shift key get all

	if (_KeyDown( SHIFT ))
	{
		// Remove all from soldier's slot
		fOk = (0 == pInventorySlot->MoveThisObjectTo(gItemPointer,-1,0,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT));
	}
	else
	{
		fOk = (0 == pInventorySlot->MoveThisObjectTo(gItemPointer, 1));
	}

	if (fOk)
	{
		if (pInventorySlot->exists() == false) {
			pInventorySlot->initialize();
		}
		// Dirty interface
		fMapPanelDirty = TRUE;
		gpItemPointer = &gItemPointer;

		if ( _KeyDown ( CTRL ))//Delete Item
		{
			gpItemPointer = NULL;
			fMapInventoryItem = FALSE;

			if ( _KeyDown ( 89 )) //Lalien: delete all items of this type on Ctrl+Y
			{
				DeleteItemsOfType( gItemPointer.usItem );
				ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, New113Message[MSG113_DELETE_ALL] );
			}
			else {
				ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, New113Message[MSG113_DELETED] );
			}
			if ( fShowMapInventoryPool )
				HandleButtonStatesWhileMapInventoryActive();
		}
		else if ( _KeyDown ( ALT ) && fSELLALL)//Sell Item
		{
			INT32 iPrice = SellItem( gItemPointer );
		    PlayJA2Sample( COMPUTER_BEEP2_IN, RATE_11025, 15, 1, MIDDLEPAN );
			gpItemPointer = NULL;
			fMapInventoryItem = FALSE;
			if ( _KeyDown ( 89 )) //Lalien: sell all items of this type on Alt+Y
			{
				for( UINT32 iNumber = 0 ; iNumber <  pInventoryPoolList.size() ; ++iNumber)
				{
					if ( pInventoryPoolList[ iNumber ].object.usItem == gItemPointer.usItem )
					{
						iPrice += SellItem( pInventoryPoolList[ iNumber ].object );
						DeleteObj( &pInventoryPoolList [ iNumber ].object );
					}
				}
			}
			//ADB you can sell items for 0, but that's not fair
			//and it's not easy to stop the sale so make the price 1
			if (iPrice == 0) {
				iPrice = 1;
			}

			if ( _KeyDown ( 89 )) //Lalien: sell all items of this type on Alt+Y
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SOLD_ALL] );
			}
			else {
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SOLD] );
			}

			AddTransactionToPlayersBook( SOLD_ITEMS, 0, GetWorldTotalMin(), iPrice );

			if ( fShowMapInventoryPool )
				HandleButtonStatesWhileMapInventoryActive();
		}
		else
		{
			gpItemPointerSoldier = NULL;

			// now set the cursor
			guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
			gusExternVoSubIndex = Item[ gpItemPointer->usItem ].ubGraphicNum;

			fMapInventoryItem = TRUE;
			MSYS_ChangeRegionCursor( &gMPanelRegion , EXTERN_CURSOR );
			SetCurrentCursorFromDatabase( EXTERN_CURSOR );

			if ( fShowInventoryFlag && bSelectedInfoChar >= 0 )
			{
				ReevaluateItemHatches( MercPtrs[ gCharactersList[ bSelectedInfoChar ].usSolID ], FALSE );
				fTeamPanelDirty = TRUE;
			}
		}
	}
}

BOOLEAN PlaceObjectInInventoryStash( OBJECTTYPE *pInventorySlot, OBJECTTYPE *pItemPtr )
{
	// if there is something there, swap it, if they are of the same type and stackable then add to the count
	if (pInventorySlot->exists() == false)
	{
		// placement in an empty slot
		pItemPtr->MoveThisObjectTo(*pInventorySlot);
	}
	else
	{
		if (pItemPtr->usItem == pInventorySlot->usItem && ItemSlotLimit(pItemPtr, STACK_SIZE_LIMIT) >= 2)
		{
			// stacking
			pInventorySlot->AddObjectsToStack(*pItemPtr);
		}
		else
		{
			SwapObjs( pItemPtr, pInventorySlot );
		}
	}
	return( TRUE );
}


BOOLEAN AutoPlaceObjectInInventoryStash( OBJECTTYPE *pItemPtr, INT16 sGridNo )
{
	OBJECTTYPE *pInventorySlot;
	INT32		cnt = 0;

	//CHRISL: I'm not sure why this never fails.  Maybe it just isn't used often.  But if we try and look for index
	//	pInventoryPoolList.size() we're going to crash since "size()" returns the total number of indecies in human terms (1-xxx)
	//	but we have to start with 0.  In other words, size() might return a value of 10 but those are numbers 0-9.

	while(1)
	{
		//if we run out of indecies before running out of items to place, make new indecies
		if(cnt == pInventoryPoolList.size())
		{
			ResizeInventoryList();
		}
		pInventorySlot = &pInventoryPoolList[cnt].object;
		if(pInventorySlot->usItem == pItemPtr->usItem)
		{
			pInventorySlot->AddObjectsToStack(*pItemPtr);
		}
		else if(pInventorySlot->exists() == false)
		{
			pItemPtr->MoveThisObjectTo(*pInventorySlot);
			if(sGridNo != 0)
			{
				pInventoryPoolList[cnt].sGridNo = sGridNo;
				pInventoryPoolList[cnt].usFlags = 512;
				pInventoryPoolList[cnt].bVisible = 1;
				pInventoryPoolList[cnt].fExists = TRUE;
			}
		}
		else
		{
			cnt++;
			continue;
		}
		if(pItemPtr->exists() == false)
		{
			break;
		}
		else
		{
			cnt++;
		}
	}


	// if there is something there, swap it, if they are of the same type and stackable then add to the count
//	pInventorySlot =  &( pInventoryPoolList[ pInventoryPoolList.size() ].object );

	// placement in an empty slot
	// could be wrong type of object for slot... need to check...
	// but assuming it isn't
//	pItemPtr->MoveThisObjectTo(*pInventorySlot);

	return( TRUE );
}

void MapInventoryPoolNextBtn( GUI_BUTTON *btn, INT32 reason )
{
		if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	  btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
  {
    if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// if can go to next page, go there
			if( iCurrentInventoryPoolPage < ( iLastInventoryPoolPage ) )
			{
				iCurrentInventoryPoolPage++;
				fMapPanelDirty = TRUE;
			}
		}
	}
}

void MapInventoryPoolPrevBtn( GUI_BUTTON *btn, INT32 reason )
{
		if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	  btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
  {
    if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// if can go to next page, go there
			if( iCurrentInventoryPoolPage > 0 )
			{
				iCurrentInventoryPoolPage--;
				fMapPanelDirty = TRUE;
			}
		}
	}
}


void MapInventoryPoolDoneBtn( GUI_BUTTON *btn, INT32 reason )
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
	  btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
  {
    if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
      btn->uiFlags&=~(BUTTON_CLICKED_ON);

			// done
			fShowMapInventoryPool = FALSE;
		}
	}
}



void DisplayPagesForMapInventoryPool( void )
{
	// get the current and last pages and display them
	CHAR16 sString[ 32 ];
	INT16 sX, sY;

	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	// set the buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// grab current and last pages
	swprintf( sString, L"%d / %d", iCurrentInventoryPoolPage + 1, iLastInventoryPoolPage + 1 );

	// grab centered coords
	FindFontCenterCoordinates(MAP_INVENTORY_POOL_PAGE_X, MAP_INVENTORY_POOL_PAGE_Y ,MAP_INVENTORY_POOL_PAGE_WIDTH ,MAP_INVENTORY_POOL_PAGE_HEIGHT ,sString , MAP_SCREEN_FONT, &sX, &sY);

	mprintf( sX, sY, sString );

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );


}

INT32 GetTotalNumberOfItemsInSectorStash( void )
{
	INT32 iCount = 0;

	// run through list of items and find out how many are there
	for( UINT32 iCounter = 0; iCounter < pInventoryPoolList.size(); iCounter++ )
	{
		if( pInventoryPoolList[ iCounter].object.exists() == true )
		{
			iCount += pInventoryPoolList[ iCounter].object.ubNumberOfObjects;
		}
	}

	return iCount;
}


INT32 GetTotalNumberOfItems( void )
{
	INT32 iCount = 0;

	// run through list of items and find out how many are there
	for(UINT32 iCounter = 0; iCounter < pInventoryPoolList.size(); iCounter++ )
	{
		if( pInventoryPoolList[ iCounter].object.exists() == true )
		{
			iCount++;
		}
	}

	return iCount;
}


void DrawNumberOfIventoryPoolItems( void )
{
	INT32 iNumberOfItems = 0;
	CHAR16 sString[ 32 ];
	INT16 sX, sY;


	iNumberOfItems = GetTotalNumberOfItemsInSectorStash( );

	// get number of items
	swprintf( sString, L"%d", iNumberOfItems );

	// set font stuff
	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	// set the buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// grab centered coords
	FindFontCenterCoordinates(MAP_INVENTORY_POOL_NUMBER_X, MAP_INVENTORY_POOL_PAGE_Y ,MAP_INVENTORY_POOL_NUMBER_WIDTH ,MAP_INVENTORY_POOL_PAGE_HEIGHT ,sString , MAP_SCREEN_FONT, &sX, &sY);

	mprintf( sX, sY, sString );

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );


	return;
}


void CreateMapInventoryPoolDoneButton( void )
{
	// create done button
	guiMapInvenButtonImage[ 2 ]=  LoadButtonImage( "INTERFACE\\done_button.sti" , -1, 0, -1, 1, -1 );
	guiMapInvenButton[ 2 ] = QuickCreateButton( guiMapInvenButtonImage[ 2 ], MAP_INV_X_OFFSET + 587 , (SCREEN_HEIGHT - 147),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapInventoryPoolDoneBtn );

	return;
}

void DestroyInventoryPoolDoneButton( void )
{
	// destroy ddone button

	RemoveButton( guiMapInvenButton[ 2 ] );
	UnloadButtonImage( guiMapInvenButtonImage[ 2 ] );


	return;
}


void DisplayCurrentSector( void )
{
	// grab current sector being displayed
	CHAR16 sString[ 32 ];
	INT16 sX, sY;


	swprintf( sString, L"%s%s%s", pMapVertIndex[ sSelMapY ], pMapHortIndex[ sSelMapX ], pMapDepthIndex[ iCurrentMapSectorZ ] );

	// set font stuff
	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	// set the buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	// grab centered coords
	FindFontCenterCoordinates(MAP_INVENTORY_POOL_LOC_X, MAP_INVENTORY_POOL_PAGE_Y ,MAP_INVENTORY_POOL_LOC_WIDTH ,MAP_INVENTORY_POOL_PAGE_HEIGHT ,sString , MAP_SCREEN_FONT, &sX, &sY);

	mprintf( sX, sY, sString );

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}


void ResizeInventoryList( void )
{
	if (pInventoryPoolList.empty() == true) {
		pInventoryPoolList.resize(MAP_INVENTORY_POOL_SLOT_COUNT);
	}

	int emptySlots = 0;
	//if it's not the exact size of a page, then make it so
	if (pInventoryPoolList.size() % MAP_INVENTORY_POOL_SLOT_COUNT) {
		emptySlots = MAP_INVENTORY_POOL_SLOT_COUNT - (pInventoryPoolList.size() % MAP_INVENTORY_POOL_SLOT_COUNT);
		pInventoryPoolList.resize(pInventoryPoolList.size() + emptySlots);
	}

	//This lets us determine whether to create a new page based on the contents of the last page instead of every page
	//  which lets players move things around without having to first hunt down and fill up blank slots on other pages
	iLastInventoryPoolPage = ( ( pInventoryPoolList.size()  - 1 ) / MAP_INVENTORY_POOL_SLOT_COUNT );
	int lastPageIndex = iLastInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT;
	int activeSlotsOnPage = 0;
	for (int x = 0; x < MAP_INVENTORY_POOL_SLOT_COUNT; ++x) {
		//don't test fExists because that hasn't been set yet, test object.exists
		if (pInventoryPoolList[lastPageIndex + x].object.exists() == true) {
			++activeSlotsOnPage;
		}
	}

	//if there aren't enough empty slots we need to grow it further
	emptySlots = MAP_INVENTORY_POOL_SLOT_COUNT - activeSlotsOnPage;
	if (emptySlots < 3) {
		//we want 1 blank page
		pInventoryPoolList.resize(pInventoryPoolList.size() + MAP_INVENTORY_POOL_SLOT_COUNT);
	}

	//do this again, it may have changed
	iLastInventoryPoolPage = ( ( pInventoryPoolList.size()  - 1 ) / MAP_INVENTORY_POOL_SLOT_COUNT );

	return;
}

void DrawTextOnMapInventoryBackground( void )
{
//	CHAR16 sString[ 64 ];
	UINT16 usStringHeight;

	SetFont( MAP_IVEN_FONT );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( FONT_BEIGE );

	// set the buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	////Calculate the height of the string, as it needs to be vertically centered.
	//usStringHeight = DisplayWrappedString( 268, 342, 53, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 0 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED | DONT_DISPLAY_TEXT );
	//DisplayWrappedString( 268, (UINT16)(342 - (usStringHeight / 2) ), 53, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 0 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	////Calculate the height of the string, as it needs to be vertically centered.
	//usStringHeight = DisplayWrappedString( 369, 342, 65, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 1 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED | DONT_DISPLAY_TEXT );
	//DisplayWrappedString( 369, (UINT16)(342 - (usStringHeight / 2) ), 65, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 1 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	//Calculate the height of the string, as it needs to be vertically centered.
	usStringHeight = DisplayWrappedString( MAP_INV_X_OFFSET + 268, (SCREEN_HEIGHT - 138), 53, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 0 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED | DONT_DISPLAY_TEXT );
	DisplayWrappedString( MAP_INV_X_OFFSET + 268, (UINT16)((SCREEN_HEIGHT - 138) - (usStringHeight / 2) ), 53, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 0 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	//Calculate the height of the string, as it needs to be vertically centered.
	usStringHeight = DisplayWrappedString( MAP_INV_X_OFFSET + 369, (SCREEN_HEIGHT - 138), 65, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 1 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED | DONT_DISPLAY_TEXT );
	DisplayWrappedString( MAP_INV_X_OFFSET + 369, (UINT16)((SCREEN_HEIGHT - 138) - (usStringHeight / 2) ), 65, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 1 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	DrawTextOnSectorInventory( );

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	return;
}


void HandleButtonStatesWhileMapInventoryActive( void )
{

	// are we even showing the amp inventory pool graphic?
	if( fShowMapInventoryPool == FALSE )
	{
		return;
	}

	// first page, can't go back any
	if( iCurrentInventoryPoolPage == 0 )
	{
		DisableButton( guiMapInvenButton[ 1 ] );
	}
	else
	{
		EnableButton( guiMapInvenButton[ 1 ] );
	}

	// last page, go no further
	if( iCurrentInventoryPoolPage == iLastInventoryPoolPage )
	{
		DisableButton( guiMapInvenButton[ 0 ] );
	}
	else
	{
		EnableButton( guiMapInvenButton[ 0 ] );
	}

	// item picked up ..disable button
	if( gMPanelRegion.Cursor == EXTERN_CURSOR )
	{
		DisableButton( guiMapInvenButton[ 2 ] );
	}
	else
	{
		EnableButton( guiMapInvenButton[ 2 ] );
	}
}


void DrawTextOnSectorInventory( void )
{
	INT16 sX = 0, sY = 0;
	CHAR16 sString[ 64 ];

	// parse the string
	swprintf( sString, zMarksMapScreenText[ 11 ] );

	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	//FindFontCenterCoordinates( MAP_INV_X_OFFSET + MAP_INVENTORY_POOL_SLOT_START_X, MAP_INVENTORY_POOL_SLOT_START_Y - 20,  630 - MAP_INVENTORY_POOL_SLOT_START_X, GetFontHeight( FONT14ARIAL ), sString, FONT14ARIAL, &sX, &sY );

	FindFontCenterCoordinates( 271, 18, SCREEN_WIDTH - 271, GetFontHeight( FONT14ARIAL ), sString, FONT14ARIAL, &sX, &sY );

	SetFont( FONT14ARIAL );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	mprintf( sX, sY, sString );

	SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

}


void HandleFlashForHighLightedItem( void )
{
	INT32 iCurrentTime = 0;
	INT32 iDifference = 0;


	// if there is an invalid item, reset
	if( iCurrentlyHighLightedItem == -1 )
	{
		fFlashHighLightInventoryItemOnradarMap = FALSE;
		giFlashHighlightedItemBaseTime = 0;
	}

	// get the current time
	iCurrentTime = GetJA2Clock();

	// if there basetime is uninit
	if( giFlashHighlightedItemBaseTime == 0 )
	{
		giFlashHighlightedItemBaseTime = iCurrentTime;
	}


	iDifference = iCurrentTime - giFlashHighlightedItemBaseTime;

	if( iDifference > DELAY_FOR_HIGHLIGHT_ITEM_FLASH )
	{
		// reset timer
		giFlashHighlightedItemBaseTime = iCurrentTime;

		// flip flag
		fFlashHighLightInventoryItemOnradarMap = !fFlashHighLightInventoryItemOnradarMap;

		// re render radar map
		RenderRadarScreen( );

	}
}

void HandleMouseInCompatableItemForMapSectorInventory( INT32 iCurrentSlot )
{
	SOLDIERTYPE *pSoldier = NULL;
	static BOOLEAN fItemWasHighLighted = FALSE;

	if( iCurrentSlot == -1 )
	{
		giCompatibleItemBaseTime = 0;
	}

	if( fChangedInventorySlots == TRUE )
	{
		giCompatibleItemBaseTime = 0;
		fChangedInventorySlots = FALSE;
	}

	// reset the base time to the current game clock
	if( giCompatibleItemBaseTime == 0 )
	{
		giCompatibleItemBaseTime = GetJA2Clock( );

		if( fItemWasHighLighted == TRUE )
		{
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			fItemWasHighLighted = FALSE;
		}
	}

	ResetCompatibleItemArray( );
	ResetMapSectorInventoryPoolHighLights( );

	if( iCurrentSlot == -1 )
	{
		return;
	}

	// given this slot value, check if anything in the displayed sector inventory or on the mercs inventory is compatable
	if( fShowInventoryFlag )
	{
		// check if any compatable items in the soldier inventory matches with this item
		if( gfCheckForCursorOverMapSectorInventoryItem )
		{
			pSoldier = &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ];
			if( pSoldier )
			{
				if( HandleCompatibleAmmoUIForMapScreen( pSoldier, iCurrentSlot + ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ), TRUE, FALSE ) )
				{
					if( GetJA2Clock( ) - giCompatibleItemBaseTime > 100 )
					{
						if( fItemWasHighLighted == FALSE )
						{
							fTeamPanelDirty = TRUE;
							fItemWasHighLighted = TRUE;
						}
					}
				}
			}
		}
		else
		{
			giCompatibleItemBaseTime = 0;
		}
	}


	// now handle for the sector inventory
	if( fShowMapInventoryPool )
	{
		// check if any compatable items in the soldier inventory matches with this item
		if( gfCheckForCursorOverMapSectorInventoryItem )
		{
			if( HandleCompatibleAmmoUIForMapInventory( pSoldier, iCurrentSlot, ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) , TRUE, FALSE ) )
			{
				if( GetJA2Clock( ) - giCompatibleItemBaseTime > 100 )
				{
					if( fItemWasHighLighted == FALSE )
					{
						fItemWasHighLighted = TRUE;
						fMapPanelDirty = TRUE;
					}
				}
			}
		}
		else
		{
			giCompatibleItemBaseTime = 0;
		}
	}


	return;
}

void ResetMapSectorInventoryPoolHighLights( void )
{
	INT32 iCounter = 0;

	// now reset the highlight list for the map sector inventory
	for ( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
	{
		if ( fMapInventoryItemCompatable[ iCounter ] )
		{
			fMapInventoryItemCompatable[ iCounter ] = FALSE;
		}
	}

	return;
}
void HandleMapSectorInventory( void )
{
	// handle mouse in compatable item map sectors inventory
	HandleMouseInCompatableItemForMapSectorInventory( iCurrentlyHighLightedItem );

	return;
}


//CJC look here to add/remove checks for the sector inventory
BOOLEAN IsMapScreenWorldItemVisibleInMapInventory( WORLDITEM *pWorldItem )
{
	if( pWorldItem->bVisible == 1 &&
		pWorldItem->fExists &&
		pWorldItem->object.exists() == true &&
		pWorldItem->object.usItem != SWITCH &&
		pWorldItem->object.usItem != ACTION_ITEM &&
		pWorldItem->object[0]->data.bTrap <= 0 )
	{
		return( TRUE );
	}

	return( FALSE );
}

//CJC look here to add/remove checks for the sector inventory
BOOLEAN IsMapScreenWorldItemInvisibleInMapInventory( WORLDITEM *pWorldItem )
{
	if( pWorldItem->fExists &&
		pWorldItem->object.exists() == true &&
		!IsMapScreenWorldItemVisibleInMapInventory( pWorldItem ) )
	{
		return( TRUE );
	}

	return( FALSE );
}

//Check to see if any of the items in the list have a gridno of NOWHERE and the entry point flag NOT set
void CheckGridNoOfItemsInMapScreenMapInventory()
{
	INT32 iCnt;
	UINT32 uiNumFlagsNotSet = 0;
	INT32	 iTotalNumberItems = GetTotalNumberOfItems( );


	for( iCnt=0; iCnt<iTotalNumberItems; iCnt++)
	{
		if( pInventoryPoolList[ iCnt ].sGridNo == NOWHERE && !( pInventoryPoolList[ iCnt ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )
		{
			//set the flag
			pInventoryPoolList[ iCnt ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;

			//count the number
			uiNumFlagsNotSet++;
		}
	}


	//loop through all the UNSEEN items
	for( iCnt=0; iCnt<(INT32)uiNumberOfUnSeenItems; iCnt++)
	{
		if( pUnSeenItems[ iCnt ].sGridNo == NOWHERE && !( pUnSeenItems[ iCnt ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )
		{
			//set the flag
			pUnSeenItems[ iCnt ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;

			//count the number
			uiNumFlagsNotSet++;
		}
	}

#ifdef JA2BETAVERSION
	if( uiNumFlagsNotSet > 0 )
	{
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Item with invalid gridno doesnt have flag set.", uiNumFlagsNotSet );
	}
#endif
}


void SortSectorInventory( std::vector<WORLDITEM>& pInventory, UINT32 uiSizeOfArray )
{
	//first, compress the inventory by stacking like items that are reachable, while moving empty items towards the back
	for (std::vector<WORLDITEM>::iterator iter = pInventory.begin(); iter != pInventory.end(); ++iter) {
		//if object exists, we want to try to stack it
		if (iter->fExists && iter->object.exists() == true) {

			//ADB TODO if it is active and reachable etc, alternatively if it's on the same gridNo
#if 0
			if (iter->object.ubNumberOfObjects < ItemSlotLimit( iter->object.usItem, STACK_SIZE_LIMIT )) {
				std::vector<WORLDITEM>::iterator second = iter;
				for (++second; second != pInventory.end(); ++second) {
					if (second->object.usItem == iter->object.usItem
						&& second->object.exists() == true) {
						iter->object.AddObjectsToStack(second->object, second->object.ubNumberOfObjects);
						if (iter->object.ubNumberOfObjects >= ItemSlotLimit( iter->object.usItem, STACK_SIZE_LIMIT )) {
							break;
						}
					}
				}
			}
#endif
		}
		else {
			//object does not exist, so compress the list
			std::vector<WORLDITEM>::iterator second = iter;
			for (++second; second != pInventory.end(); ++second) {
				if (second->fExists && second->object.exists() == true) {
					*iter = *second;
					second->initialize();
					break;
				}
			}
			if (second == pInventory.end()) {
				//we reached the end of the list without finding any active item, so we can break out of this loop too!
				break;
			}
		}
	}

	//once compressed, we need only sort the existing items
	//all empty items should be at the back!!!
	std::vector<WORLDITEM>::iterator endSort = pInventory.begin();
	for (unsigned int x = 1; x < pInventory.size(); ++x) {
		if (pInventory[x].fExists && pInventory[x].object.exists() == true) {
			++endSort;
		}
		else {
			break;
		}
	}
	++endSort;

	//ADB I'm not sure qsort will work with OO data, so replace it with stl sort, which is faster anyways
	std::sort(pInventory.begin(), endSort);

	//then compress it by removing the empty objects, we know they are at the back
	//we want the size to equal x * MAP_INVENTORY_POOL_SLOT_COUNT
	for (unsigned int x = 1; x <= pInventory.size() / MAP_INVENTORY_POOL_SLOT_COUNT && pInventory.size() > x * MAP_INVENTORY_POOL_SLOT_COUNT; ++x) {
		if (pInventory[x * MAP_INVENTORY_POOL_SLOT_COUNT].fExists == false
			&& pInventory[x * MAP_INVENTORY_POOL_SLOT_COUNT].object.exists() == false) {
			//we have found a page where the first item on the page does not exist, resize to this
			//we may have just cut off a blank page leaving the previous page full with no place to put
			//any new objects, but ResizeInventoryList after this will take care of that.
			pInventory.resize(x * MAP_INVENTORY_POOL_SLOT_COUNT);
		}
	}
}


INT32 MapScreenSectorInventoryCompare( const void *pNum1, const void *pNum2)
{
	WORLDITEM *pFirst = (WORLDITEM *)pNum1;
	WORLDITEM *pSecond = (WORLDITEM *)pNum2;
	UINT16	usItem1Index;
	UINT16	usItem2Index;
	UINT16		ubItem1Quality;
	UINT16		ubItem2Quality;

	usItem1Index = pFirst->object.usItem;
	usItem2Index = pSecond->object.usItem;

	ubItem1Quality = pFirst->object[0]->data.objectStatus;
	ubItem2Quality = pSecond->object[0]->data.objectStatus;

	return( CompareItemsForSorting( usItem1Index, usItem2Index, ubItem1Quality, ubItem2Quality ) );
}

BOOLEAN CanPlayerUseSectorInventory( SOLDIERTYPE *pSelectedSoldier )
{
	INT16	sSectorX, sSectorY, sSectorZ;
	BOOLEAN fInCombat;

	//Get the sector that has a battle
	fInCombat = GetCurrentBattleSectorXYZAndReturnTRUEIfThereIsABattle( &sSectorX, &sSectorY, &sSectorZ );

	//if there is a battle going on
	if( fInCombat )
	{
		//if the selected map is the one with the combat
		if( ( ( sSelMapX == sSectorX ) &&
					( sSelMapY == sSectorY ) &&
					( iCurrentMapSectorZ == sSectorZ )
				)
			)
		{
			return( FALSE );
		}
	}

	return( TRUE );
}

void DeleteAllItemsInInventoryPool()
{
	pInventoryPoolList.clear();

	fMapPanelDirty = TRUE;

	ClearUpTempUnSeenList( );
	SaveSeenAndUnseenItems();

	iCurrentInventoryPoolPage = 0;
	iLastInventoryPoolPage = 0;

	DestroyStash();
	BuildStashForSelectedSector( sSelMapX, sSelMapY, iCurrentMapSectorZ);
}


void DeleteItemsOfType( UINT16 usItemType )
{
	for( UINT32 iNumber = 0 ; iNumber <  pInventoryPoolList.size() ; ++iNumber)
	{
		if ( pInventoryPoolList [ iNumber ].object.usItem == usItemType )
		{
			DeleteObj( &pInventoryPoolList [ iNumber ].object );
		}
	}

}

extern UINT8 CurrentPlayerProgressPercentage(void);
INT32 SellItem( OBJECTTYPE& object, BOOLEAN useModifier )
{
	INT32 iPrice = 0;
	INT16 iPriceModifier = gGameExternalOptions.iPriceModifier;
	UINT16 usItemType = object.usItem;
	UINT16 itemPrice = Item[usItemType].usPrice;

	//CHRISL: make price modifier based on current game progress
	if(iPriceModifier == 0)
	{
		iPriceModifier = __max(1, (INT16)((CurrentPlayerProgressPercentage()/5)+.5));
	}
	else if(iPriceModifier == -1)
	{
		iPriceModifier = __max(1, (INT16)(((100-CurrentPlayerProgressPercentage())/5)+.5));
	}

	if( Item[ usItemType ].usItemClass == IC_AMMO )
	{
		//we are selling ammo
		UINT16 magSize = Magazine[ Item[ usItemType ].ubClassIndex ].ubMagSize;
		for (INT8 bLoop = 0; bLoop < object.ubNumberOfObjects; bLoop++)
		{
			iPrice += (INT32)( itemPrice * (float) object[bLoop]->data.ubShotsLeft / magSize );
		}
	}
	else if(Item[usItemType].usItemClass == IC_LBEGEAR && UsingNewInventorySystem() == true)
	{
		//CHRISL: If we're selling an LBE Item, we need to verify if it's an LBENODE, first.  If it is, we need to sell
		//	everything stored in the LBENODE before we sell teh LBE Item itself.
		for(INT8 bLoop = 0; bLoop < object.ubNumberOfObjects; bLoop++)
		{
			if(object.IsActiveLBE(bLoop) == true)
			{
				LBENODE* pLBE = object.GetLBEPointer(bLoop);
				if(pLBE)
				{
					for(unsigned int x = 0; x < pLBE->inv.size(); x++)
					{
						if(pLBE->inv[x].exists() == true)
						{
							iPrice += SellItem(pLBE->inv[x], FALSE);
						}
					}
				}
			}
			iPrice += ( itemPrice * object[bLoop]->data.objectStatus / 100 );
			for (attachmentList::iterator iter = object[bLoop]->attachments.begin(); iter != object[bLoop]->attachments.end(); ++iter) {
				iPrice += SellItem(*iter, FALSE);
			}
		}
	}
	else
	{
		//we are selling a gun or something - it could be stacked or single, and if single it could have attachments
		for (INT8 bLoop = 0; bLoop < object.ubNumberOfObjects; bLoop++)
		{
			iPrice += ( itemPrice * object[bLoop]->data.objectStatus / 100 );
			for (attachmentList::iterator iter = object[bLoop]->attachments.begin(); iter != object[bLoop]->attachments.end(); ++iter) {
				iPrice += SellItem(*iter, FALSE);
			}
		}

	}

	if( iPriceModifier > 1 && useModifier == TRUE) {
		iPrice /= iPriceModifier;
	}

	return iPrice;
}
