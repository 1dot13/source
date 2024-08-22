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
	#include "GameSettings.h"
	#include "Radar Screen.h"
	#include "message.h"
	#include "weapons.h"
	#include "finances.h"
	#include "Game Clock.h"
	#include "Sound Control.h"
	#include "interface panels.h"
	#include "wordwrap.h"
	#include "Soldier macros.h"
	#include "rt time defines.h"
	#include "Encyclopedia_new.h" //Moa: item visibility
	#include "Town Militia.h"	// added by Flugente

#include "ShopKeeper Interface.h"
#include "ArmsDealerInvInit.h"
#include <algorithm>
#include "InterfaceItemImages.h"
#include "SaveLoadGame.h"//dnl ch51 081009
#include "Map Information.h"//dnl ch51 091009
#include "Interface Items.h"
#include "Food.h"	// added by Flugente
#include "Campaign Types.h"	// added by Flugente
#include "mapscreen.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
extern UILayout_Map UI_MAP;
extern WorldItems gAllWorldItems;

// Flugente: external sector data
extern SECTOR_EXT_DATA	SectorExternalData[256][4];

extern void	SetLastTimePlayerWasInSector(INT16 sMapX, INT16 sMapY, INT8 sMapZ);	// Flugente: set last time sector was visited

UINT16 MAPINV_NEXT_X;
UINT16 MAPINV_PREV_X;
UINT16 MAPINV_ARROW_Y;
UINT16 MAPINV_ZOOM_X;
UINT16 MAPINV_ZOOM_Y;
UINT16 MAPINV_STACK_X;
UINT16 MAPINV_STACK_Y;
UINT16 MAPINV_SORT_AMMO_X;
UINT16 MAPINV_SORT_AMMO_Y;
UINT16 MAPINV_SORT_ATT_X;
UINT16 MAPINV_SORT_ATT_Y;
UINT16 MAPINV_EJECT_AMMO_X;
UINT16 MAPINV_EJECT_AMMO_Y;
UINT16 MAPINV_FILTER_X;
UINT16 MAPINV_FILTER_Y;
UINT16 MAPINV_FILTER_AMMO_X;
UINT16 MAPINV_FILTER_AMMO_Y;
UINT16 MAPINV_FILTER_GUN_X;
UINT16 MAPINV_FILTER_GUN_Y;
UINT16 MAPINV_FILTER_EXPLOSIVE_X;
UINT16 MAPINV_FILTER_EXPLOSIVE_Y;
UINT16 MAPINV_FILTER_MELEE_X;
UINT16 MAPINV_FILTER_MELEE_Y;
UINT16 MAPINV_FILTER_ARMOR_X;
UINT16 MAPINV_FILTER_ARMOR_Y;
UINT16 MAPINV_FILTER_LBE_X;
UINT16 MAPINV_FILTER_LBE_Y;
UINT16 MAPINV_FILTER_KIT_X;
UINT16 MAPINV_FILTER_KIT_Y;
UINT16 MAPINV_FILTER_MISC1_X;
UINT16 MAPINV_FILTER_MISC1_Y;
UINT16 MAPINV_FILTER_MISC2_X;
UINT16 MAPINV_FILTER_MISC2_Y;
UINT16 MAPINV_FILTER_SAVE_TEMPLATE_X;
UINT16 MAPINV_FILTER_SAVE_TEMPLATE_Y;
UINT16 MAPINV_FILTER_LOAD_TEMPLATE_X;
UINT16 MAPINV_FILTER_LOAD_TEMPLATE_Y;
UINT16 MAPINV_DONE_X;
UINT16 MAPINV_DONE_y;
UINT16 MAPINV_ITEMPOOL_X;
UINT16 MAPINV_ITEMPOOL_Y;
UINT16 MAPINV_PAGE_X;
UINT16 MAPINV_PAGE_Y;
UINT16 MAPINV_SECTOR_X;
UINT16 MAPINV_SECTOR_Y;
UINT16 MAPINV_LOC_STR_X;
UINT16 MAPINV_LOC_STR_Y;
UINT16 MAPINV_ITEMPOOL_STR_X;
UINT16 MAPINV_ITEMPOOL_STR_Y;

//dnl ch51 081009
UINT8 gInventoryPoolIndex = '0';
std::vector<WORLDITEM> pInventoryPoolListQ[INVPOOLLISTNUM];
INT32 iCurrentInventoryPoolPageQ[INVPOOLLISTNUM]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
INT32 iLastInventoryPoolPageQ[INVPOOLLISTNUM]={ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#define MAP_INV_X_OFFSET						(((SCREEN_WIDTH - 261) - 380) / 2)

// status bar colors
#define DESC_STATUS_BAR FROMRGB( 201, 172,  133 )
#define DESC_STATUS_BAR_SHADOW FROMRGB( 140, 136,  119 )

// page display positions
#define MAP_INVENTORY_POOL_PAGE_X				(MAP_INV_X_OFFSET + 506)
#define MAP_INVENTORY_POOL_PAGE_Y				(SCREEN_HEIGHT - 121 - 23)	//336
#define MAP_INVENTORY_POOL_PAGE_WIDTH			46
#define MAP_INVENTORY_POOL_PAGE_HEIGHT			13

// the number of items
#define MAP_INVENTORY_POOL_NUMBER_X				(MAP_INV_X_OFFSET + 436)
#define MAP_INVENTORY_POOL_NUMBER_WIDTH			40

// location
#define MAP_INVENTORY_POOL_LOC_X				(MAP_INV_X_OFFSET + 326)
#define MAP_INVENTORY_POOL_LOC_WIDTH			40

// delay for flash of item
#define DELAY_FOR_HIGHLIGHT_ITEM_FLASH			200

// inventory slot font
#define MAP_IVEN_FONT							SMALLCOMPFONT
// HEADROCK HAM 5: Item names use variable fonts.
INT32	MAP_INVEN_NAME_FONT	= SMALLCOMPFONT;

// the position of the background graphic
#define INVEN_POOL_X 261
#define INVEN_POOL_Y 0

// inventory Graphic Offset X and y
#define MAP_INVENTORY_POOL_SLOT_OFFSET_X 2
#define MAP_INVENTORY_POOL_SLOT_OFFSET_Y 5

// height of map inventory pool bar
// HEADROCK HAM 5: Changed to variables
UINT16 ITEMDESC_ITEM_STATUS_HEIGHT_INV_POOL = 20;
UINT16 ITEMDESC_ITEM_STATUS_WIDTH_INV_POOL = 2;

// map bar offsets
// HEADROCK HAM 5: Changed to variables
INT16 ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_X = 5;
INT16 ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y = 22;

// HEADROCK HAM 5: For now I just need a Y.
INT16 ITEMDESC_ITEM_NAME_POOL_OFFSET_Y = 3;

// inventory pool slot positions and sizes
//#define MAP_INVENTORY_POOL_SLOT_START_X 271
//#define MAP_INVENTORY_POOL_SLOT_START_Y 36
// HEADROCK HAM 5: Changed to variables
UINT16 MAP_INVEN_SLOT_WIDTH = 65;
UINT16 MAP_INVEN_SPACE_BTWN_SLOTS = 72;
UINT16 MAP_INVEN_SLOT_HEIGHT = 32;
UINT16 MAP_INVEN_SLOT_IMAGE_HEIGHT = 24;

// Number of inventory slots in 1024x768
#define MAP_INVENTORY_POOL_MAX_SLOTS 170

INT32 MAP_INV_SLOT_COLS;			// Number of vertical slots
INT32 MAP_INVENTORY_POOL_SLOT_COUNT;

INT32 MAP_INVENTORY_POOL_SLOT_START_X;
INT32 MAP_INVENTORY_POOL_SLOT_START_Y;

// the current highlighted item
INT32 iCurrentlyHighLightedItem = -1;
BOOLEAN fFlashHighLightInventoryItemOnradarMap = FALSE;

INT32 iCurrentlyPickedUpItem = -1;

// whether we are showing the inventory pool graphic
BOOLEAN fShowMapInventoryPool = FALSE;
// HEADROCK HAM 5: Flag telling us whether we've already redrawn the screen to show
// sector inventory sale prices.
UINT8 gubRenderedMapInventorySalePrices = FALSE;

// the v-object index value for the background
UINT32 guiMapInventoryPoolBackground;
// HEADROCK HAM 5: Graphic for map inventory slots
UINT32 guiMapInventoryPoolSlot;

// inventory pool list
std::vector<WORLDITEM> pInventoryPoolList;

// current page of inventory
INT32 iCurrentInventoryPoolPage = 0;
INT32 iLastInventoryPoolPage = 0;

INT32 sObjectSourceGridNo = -1;//shadooow: I don't see much of a sense in this, we have gpItemPointerSoldier and we can use gpItemPointerSoldier->sGridNo to do this, this is actually unused
INT8  sObjectSourseSoldierID = -1;

// number of unseen items in sector
UINT32 uiNumberOfUnSeenItems = 0;

// the inventory slots
//MOUSE_REGION MapInventoryPoolSlots[ MAP_INVENTORY_POOL_SLOT_COUNT ];
MOUSE_REGION MapInventoryPoolSlots[ MAP_INVENTORY_POOL_MAX_SLOTS ];
MOUSE_REGION MapInventoryPoolMask;
// HEADROCK HAM 5: This rectangle describes the inventory. It is used to limit cursor movement while waiting for zoom input.
SGPRect MapInventoryRect;

//BOOLEAN fMapInventoryItemCompatable[ MAP_INVENTORY_POOL_SLOT_COUNT ];
BOOLEAN fMapInventoryItemCompatable[ MAP_INVENTORY_POOL_MAX_SLOTS ];
// HEADROCK HAM 5: Same idea as above, this flags items as being candidates for appearing in Zoomed mode.
BOOLEAN gfMapInventoryItemToZoom[ MAP_INVENTORY_POOL_MAX_SLOTS ];

BOOLEAN fChangedInventorySlots = FALSE;

// the unseen items list...have to save this
std::vector<WORLDITEM> pUnSeenItems;//dnl ch75 271013
#ifdef INVFIX_Moa//dnl ch75 311013
// save list to write to temp file
std::vector<WORLDITEM> pSaveList;
#else
std::vector<WORLDITEM>& pSaveList = pUnSeenItems;
#endif
INT32 giFlashHighlightedItemBaseTime = 0;
//INT32 giCompatibleItemBaseTime = 0;//Moa: removed (see HandleMouseInCompatableItemForMapSectorInventory)

// the buttons and images
// HEADROCK HAM 5: Increased for new inventory buttons
UINT32 guiMapInvenArrowButtonImage[ 2 ];
UINT32 guiMapInvenArrowButton[ 2 ];

UINT32 guiMapInvenDoneButtonImage;
UINT32 guiMapInvenDoneButton;

UINT32 guiMapInvenZoomButtonImage;
UINT32 guiMapInvenZoomButton;

UINT32 guiMapInvenSortButtonImage[4];
UINT32 guiMapInvenSortButton[4];

UINT32 guiMapInvenFilterButtonImage[MAP_INVENTORY_FILTER_BUTTONS];
UINT32 guiMapInvenFilterButton[MAP_INVENTORY_FILTER_BUTTONS];
BOOLEAN guiMapInvenFilterButtonDefined[MAP_INVENTORY_FILTER_BUTTONS];


BOOLEAN gfCheckForCursorOverMapSectorInventoryItem = FALSE;

// HEADROCK HAM 5: Map Inventory ZOOM
BOOLEAN fMapInventoryZoom = FALSE;
// HEADROCK HAM 5: This flag tells us that we're waiting for player zoom region selection. It affects the
// behavior of inventory slot regions.
BOOLEAN fWaitingForZoomInput = FALSE;

// HEADROCK HAM 5: This is the minimum desired number of inventory items. Unless we reset this to -1, there will be
// exactly as many item pages as we had previously when checking inventory resize.
INT32 giDesiredNumMapInventorySlots = -1;

// HEADROCK HAM 5: This is an inventory filter. It is a bit array, reacting to an item's Class.
UINT32 guiMapInventoryFilter = IC_MAPFILTER_ALL;

// HEADROCK HAM 5: Constants and variables for the filter popup menu.
BOOLEAN gfMapInventoryFilterPopupInitialized = FALSE;
POPUP*  gMapInventoryFilterPopup;
BOOLEAN gfMapInventoryFilterPopupVisible = FALSE;
BOOLEAN gfQueueRecreateMapInventoryFilterMenu = FALSE;

extern int PLAYER_INFO_Y;
extern int INV_REGION_Y;

extern BOOLEAN fShowInventoryFlag;
extern BOOLEAN fMapScreenBottomDirty;

extern BOOLEAN ReduceStringLength( STR16 pString, UINT32 uiWidthToFitIn, UINT32 uiFont );

extern OBJECTTYPE gItemPointer;

extern OBJECTTYPE * gpItemPointer;

// outside vidieo objects for cursor
extern UINT32 guiExternVo;
extern UINT16 gusExternVoSubIndex;

extern	MOUSE_REGION    gMPanelRegion;

extern void InternalMAPBeginItemPointer( SOLDIERTYPE *pSoldier );

extern UINT32 GetLastTimePlayerWasInSector(INT16 sMapX, INT16 sMapY, INT8 sMapZ);	// Flugente: get time for another sector

// HEADROCK HAM 5: Because BigItem graphics are not loaded into memory by default, we need to load them to
// display the large map inventory. We save their indexes in this array:
typedef struct
{
	UINT16 usItem;
	INT32 iGraphicNum;
} BIGITEMSLOTGRAPHICS;

BIGITEMSLOTGRAPHICS giMapInventoryBigItemGraphics[ MAP_INVENTORY_POOL_MAX_SLOTS ];

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
BOOLEAN PlaceObjectInInventoryStash( OBJECTTYPE *pInventorySlot, OBJECTTYPE *pItemPtr, INT32 iDestSlot = (-1), INT32 iSrcSlot = (-1) );
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
// HEADROCK HAM 5: Callback for inventory zoom
void MapInventoryPoolZoomBtn( GUI_BUTTON *btn, INT32 reason );
// HEADROCK HAM 5: Sort ammo button callback
void MapInventoryPoolStackAndMergeBtn( GUI_BUTTON *btn, INT32 reason );
void MapInventoryPoolSortAmmoBtn( GUI_BUTTON *btn, INT32 reason );
void MapInventoryPoolSortAttachmentsBtn( GUI_BUTTON *btn, INT32 reason );
void MapInventoryPoolEjectAmmoBtn( GUI_BUTTON *btn, INT32 reason );
// HEADROCK HAM 5: Preliminary Filter Button
void MapInventoryPoolFilterBtn( GUI_BUTTON *btn, INT32 reason );
void MapInventoryPoolFilterBtnMoveItemDisplay( GUI_BUTTON *btn, INT32 reason );	// Flugente

// Flugente: gear templates
void MapInventoryWriteEquipmentTemplate(GUI_BUTTON *btn, INT32 reason);
void MapInventoryReadEquipmentTemplate(GUI_BUTTON *btn, INT32 reason);

void DisplayCurrentSector( void );
void ResizeInventoryList( void );
#ifdef INVFIX_Moa//dnl ch85 050214
void ClearUpTempUnSeenList( void );
#endif
void SaveSeenAndUnseenItems( void );
void DrawTextOnMapInventoryBackground( void );
void DrawTextOnSectorInventory( void );
INT32 GetTotalNumberOfItemsInSectorStash( void );
void HandleMapSectorInventory( void );
void ResetMapSectorInventoryPoolHighLights( void );
void ReBuildWorldItemStashForLoadedSector( INT32 iNumberSeenItems, INT32 iNumberUnSeenItems, std::vector<WORLDITEM>& pSeenItemsList, std::vector<WORLDITEM>& pUnSeenItemsList );//dnl ch75 271013
BOOLEAN IsMapScreenWorldItemVisibleInMapInventory( WORLDITEM *pWorldItem );
BOOLEAN IsMapScreenWorldItemInvisibleInMapInventory( WORLDITEM *pWorldItem );
void CheckGridNoOfItemsInMapScreenMapInventory();
INT32 MapScreenSectorInventoryCompare( const void *pNum1, const void *pNum2);
void SortSectorInventory( std::vector<WORLDITEM>& pInventory, UINT32 uiSizeOfArray );
BOOLEAN CanPlayerUseSectorInventory( SOLDIERTYPE *pSelectedSoldier );

extern void MAPEndItemPointer( );
extern	BOOLEAN GetCurrentBattleSectorXYZAndReturnTRUEIfThereIsABattle( INT16 *psSectorX, INT16 *psSectorY, INT16 *psSectorZ );
extern BOOLEAN MAPInternalInitItemDescriptionBox( OBJECTTYPE *pObject, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier );

#ifdef INVFIX_Moa//dnl ch85 050214
void DeleteAllItemsInInventoryPool();
#endif
void DeleteItemsOfType( UINT16 usItemType );
// HEADROCK HAM 5: flag to indicate that all items in the stack will be sold. This is primarily used to
// figure out the price of the top item in the stack rather than just all of them.
INT32 SellItem( OBJECTTYPE& object, BOOLEAN fAll, BOOLEAN useModifier = TRUE );

// HEADROCK HAM 5: Resets the coordinates and sizes of inventory slots.
void ResetMapInventoryOffsets(void);
// HEADROCK HAM 5: Handles loading and unloading Big Item images.
void LoadAllMapInventoryBigItemGraphics();
void LoadMapInventoryBigItemGraphic( INT32 iCounter );
void UnloadAllMapInventoryBigItemGraphics();
void UnloadMapInventoryBigItemGraphic( INT32 iCounter );
void ResetAllMapInventoryBigItemGraphics();
// HEADROCK HAM 5: Handles map inventory zoom/unzoom to specific page.
void HandleMapInventoryZoom( UINT32 iPage, INT32 iCounter );
void HandleMapInventoryUnzoom( UINT32 iPage );
// HEADROCK HAM 5: Function to get the number of slots when zoomed.
UINT16 GetInventorySlotCount( BOOLEAN fZoomed );
void CancelInventoryZoomInput( BOOLEAN fButtonOff );

void AnimateZoomInventory ( UINT16 iLocationInPool, UINT16 iCounter, INT32 iStartX, INT32 iStartY, UINT32 uiOrigWidth, UINT32 uiOrigHeight );
// HEADROCK HAM 5: This does the same thing as CTRL-SHIFT-A in the tactical view, except it sorts
// the currently-opened sector inventory ammo.
void SortSectorInventoryAmmo(bool useBoxes);
void SortSectorInventoryEjectAmmo();
void SortSectorInventoryEmptyLBE(); // Bob: added option to empty LBE items in sector
void SortSectorInventorySeparateAttachments();
void SortSectorInventoryStackAndMerge(bool ammoOnly);
// HEADROCK HAM 5: A quick function to rebuilt the Seen and Unseen item pools without saving the inventory.
void RefreshSeenAndUnseenPools();
void CreateMapInventoryFilterMenu( );
void MapInventoryFilterMenuPopup_FilterToggle( UINT32 uiFlags );
void MapInventoryFilterMenuPopup_FilterSet(UINT32 uiFlags );
void MapInventoryFilterMenuPopup_Hide( void );
BOOLEAN MapInventoryFilterMenuPopup_OptionOff( void );
void MapInventoryFilterToggle( UINT32 uiFlags );
void MapInventoryFilterSet( UINT32 uiFlags );
void HandleSetFilterButtons();
void InitMapInventoryCoordinates(void);
void HandleMousePageScroll(void);
void MapInvNextPage(void);
void MapInvPreviousPage(void);


//dnl ch75 271013 ClearAllItemPools and RefreshItemPools are relocated from "Handle Items.cpp"
void ClearAllItemPools()
{
	for(INT32 cnt=0; cnt<WORLD_MAX; cnt++)
	{
		RemoveItemPool(cnt, 0);
		RemoveItemPool(cnt, 1);
	}
}

void RefreshItemPools(std::vector<WORLDITEM>& pItemList, INT32 iNumberOfItems)
{
	ClearAllItemPools();
	RefreshWorldItemsIntoItemPools(pItemList, iNumberOfItems);
}

// Flugente:  show whether an item is set to be ignored by the 'move item' assignment. This can be toggled in strategic inventory.
static BOOLEAN fShowMoveItem = TRUE;
BOOLEAN IsShowMoveItem()
{
	return (fShowMoveItem && guiCurrentScreen == MAP_SCREEN);
}

void ToggleShowMoveItem()
{
	fShowMoveItem = !fShowMoveItem;
}

// load the background panel graphics for inventory
BOOLEAN LoadInventoryPoolGraphic( void )
{
	VOBJECT_DESC    VObjectDesc;

	// load the file
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;

	if (iResolution >= _640x480 && iResolution < _800x600)
	{
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory.sti" );
	}
	else if (isWidescreenUI())
	{
		sprintf(VObjectDesc.ImageFile, "INTERFACE\\sector_inventory_1280x720.sti");
	}
	else if (iResolution < _1024x768)
	{
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory_800x600.sti" );
	}
	else
	{
		sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory_1024x768.sti" );
	}


	// add to V-object index
	CHECKF(AddVideoObject(&VObjectDesc, &guiMapInventoryPoolBackground));

	// HEADROCK HAM 5: Load individual slot pics.
	sprintf( VObjectDesc.ImageFile, "INTERFACE\\sector_inventory_slot.sti" );
	CHECKF(AddVideoObject(&VObjectDesc, &guiMapInventoryPoolSlot));

	// HEADROCK HAM 5: Take this opportunity to load BigItem asterisks.
	sprintf( VObjectDesc.ImageFile, "INTERFACE\\Attachment_Asterisks.sti" );
	CHECKF(AddVideoObject(&VObjectDesc, &guiAttachmentAsterisks));


	ResetMapInventoryOffsets();
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

	// HEADROCK HAM 5: Remove slot image
	if (guiMapInventoryPoolSlot)
	{
		DeleteVideoObjectFromIndex( guiMapInventoryPoolSlot );
		guiMapInventoryPoolSlot = 0;
	}
	
	// HEADROCK HAM 5: Remove asterisks
	if (guiAttachmentAsterisks)
	{
		DeleteVideoObjectFromIndex( guiAttachmentAsterisks );
		guiAttachmentAsterisks = 0;
	}

	return;
}

// blit the background panel for the inventory
void BlitInventoryPoolGraphic( void )
{
	HVOBJECT hHandle;

	// blit inventory pool graphic to the screen
	GetVideoObject(&hHandle, guiMapInventoryPoolBackground);
	BltVideoObject( guiSAVEBUFFER , hHandle, 0, UI_MAP.BorderRegion.x, UI_MAP.BorderRegion.y, VO_BLT_SRCTRANSPARENCY,NULL );

	// HEADROCK HAM 5: Draw inventory slots
	BlitInventoryPoolSlotGraphics();

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
	RestoreExternBackgroundRect(UI_MAP.BorderRegion.x, UI_MAP.BorderRegion.y, SCREEN_WIDTH, SCREEN_HEIGHT - 121);

	return;
}

// HEADROCK HAM 5: Blit all map inventory item slots
void BlitInventoryPoolSlotGraphics( void )
{
	INT16 sX, sY;
	HVOBJECT hHandle;

	// blit inventory pool graphic to the screen
	GetVideoObject(&hHandle, guiMapInventoryPoolSlot);
		
	for( INT32 iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT ; iCounter++ )
	{
		sX = ( INT16 )( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCounter / MAP_INV_SLOT_COLS ) ) );
		sY = ( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCounter % ( MAP_INV_SLOT_COLS ) ) ) );

		BltVideoObject( guiSAVEBUFFER , hHandle, fMapInventoryZoom, sX, sY , VO_BLT_SRCTRANSPARENCY,NULL );
	}
	
	return;
}

void RenderItemsForCurrentPageOfInventoryPool( void )
{
	// go through list of items on this page and place graphics to screen
	for ( INT32 iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; ++iCounter )
	{
		RenderItemInPoolSlot( iCounter, ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) );
	}
}

BOOLEAN RenderItemInPoolSlot( INT32 iCurrentSlot, INT32 iFirstSlotOnPage )
{
	// render item in this slot of the list
	INT16 sCenX, sCenY, usWidth, usHeight, sX, sY;
	HVOBJECT hHandle;
	ETRLEObject		*pTrav;
	CHAR16 sString[ 80 ];
	INT16 sWidth = 0, sHeight = 0;
	INT16 sOutLine = 0;
	BOOLEAN fOutLine = FALSE;

	// check if anything there

	if( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == false )
	{
		// HEADROCK HAM 5: Have to return false.
		return ( FALSE );
	}

	GetVideoObject( &hHandle, GetInterfaceGraphicForItem( &(Item[ pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ] ) ) );

	UINT16 usGraphicNum = g_bUsePngItemImages ? 0 : Item[pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ].ubGraphicNum;
	pTrav = &( hHandle->pETRLEObject[ usGraphicNum ] );
	usHeight				= (UINT16)pTrav->usHeight;
	usWidth					= (UINT16)pTrav->usWidth;

	// set sx and sy
	sX = ( INT16 )( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCurrentSlot / MAP_INV_SLOT_COLS ) ) );
	sY = 	( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCurrentSlot % ( MAP_INV_SLOT_COLS ) ) ) );

	// CENTER IN SLOT!
	sCenX = sX + ( abs( MAP_INVEN_SPACE_BTWN_SLOTS - usWidth ) / 2 ) - pTrav->sOffsetX;
	sCenY = sY + ( abs( MAP_INVEN_SLOT_HEIGHT - 5 - usHeight ) / 2 ) - pTrav->sOffsetY;

	if( fMapInventoryItemCompatable[ iCurrentSlot ] || gfMapInventoryItemToZoom[ iCurrentSlot ] )
	{
		fOutLine = TRUE;
		// Determine Color
		if ( fMapInventoryItemCompatable[ iCurrentSlot ] )
		{
			sOutLine = Get16BPPColor( FROMRGB( 210, 210, 210 ) );
		}
		else if ( gfMapInventoryItemToZoom[ iCurrentSlot ] )
		{
			sOutLine = Get16BPPColor( FROMRGB( 235, 235, 30 ) );
		}
	}
	else
	{
		sOutLine = us16BPPItemCyclePlacedItemColors[ 0 ];
		fOutLine = FALSE;
	}

	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	if(UsingNewInventorySystem() == true && gpItemPointer != NULL)
	{
		int itemSlotLimit = ItemSlotLimit(gpItemPointer, STACK_SIZE_LIMIT);
		RenderPocketItemCapacity( guiSAVEBUFFER, itemSlotLimit, STACK_SIZE_LIMIT, 0, &pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object, (sX + 7), sY );
	}

	if (fMapInventoryZoom)
	{
		// Check whether the correct graphic exists.
		if (giMapInventoryBigItemGraphics[ iCurrentSlot ].iGraphicNum == -1 || 
			giMapInventoryBigItemGraphics[ iCurrentSlot ].usItem != pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem )
		{
			UnloadMapInventoryBigItemGraphic( iCurrentSlot );
			LoadMapInventoryBigItemGraphic( iCurrentSlot );
		}
		// Retest
		if (giMapInventoryBigItemGraphics[ iCurrentSlot ].usItem > 0 && giMapInventoryBigItemGraphics[ iCurrentSlot ].iGraphicNum > -1)
		{
			MAPINVRenderItem( guiSAVEBUFFER, NULL, &(pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object), (UINT32)giMapInventoryBigItemGraphics[ iCurrentSlot ].iGraphicNum, 
				sX + 5, sY, MAP_INVEN_SLOT_WIDTH, MAP_INVEN_SLOT_IMAGE_HEIGHT, fOutLine, sOutLine );
		}
	}
	else
	{
		INVRenderItem( guiSAVEBUFFER, NULL, &(pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object),
			(INT16)(sX + 7), sY, 60, 25, DIRTYLEVEL2, NULL, 0, fOutLine, sOutLine );//67
	}

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	if( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == false )
	{
		return ( FALSE );
	}

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
		if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == true)
		{
			DrawHatchOnInventory( guiSAVEBUFFER, sX, sY, MAP_INVEN_SLOT_WIDTH, MAP_INVEN_SLOT_IMAGE_HEIGHT );

#ifdef ENCYCLOPEDIA_WORKS
			//Moa: set encyclopedia item visibility (not reachable)
			EncyclopediaSetItemAsVisible( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem, ENC_ITEM_DISCOVERED_NOT_REACHABLE );
#endif
		}
	}
#ifdef ENCYCLOPEDIA_WORKS
	//Moa: set encyclopedia item visibility (reachable)
	else if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == true )
		EncyclopediaSetItemAsVisible( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem, ENC_ITEM_DISCOVERED_NOT_INSPECTABLE );
#endif

	// Flugente: militia equipment
	if( gGameExternalOptions.fMilitiaUseSectorInventory && ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ALL ) )
	{
		//Shade the item, but only if it is an active item!
		if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == true)
		{
			// colour depends on flag
			UINT16 usMilitia_EQColor = Get16BPPColor( FROMRGB( 0, 150, 215 ) );
			if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE )
				usMilitia_EQColor = Get16BPPColor( FROMRGB( 0, 110, 245 ) );
			if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ELITE )
				usMilitia_EQColor = Get16BPPColor( FROMRGB( 156, 37, 3 ) );

			DrawHatchOnInventory_MilitiaAccess( guiSAVEBUFFER, sX, sY, MAP_INVEN_SLOT_WIDTH, MAP_INVEN_SLOT_IMAGE_HEIGHT , usMilitia_EQColor);
		}
	}

	// Flugente: move item assignment ignore marker
	if( ( IsShowMoveItem() && pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].usFlags & WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE ) )
	{
		//Shade the item, but only if it is an active item!
		if ( pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.exists() == true)
		{
			// colour depends on flag
			UINT16 usColor = Get16BPPColor( FROMRGB( 254, 190, 133 ) );

			DrawHatchOnInventory_MilitiaAccess( guiSAVEBUFFER, sX, sY, MAP_INVEN_SLOT_WIDTH, MAP_INVEN_SLOT_IMAGE_HEIGHT , usColor);
		}
	}

	// the name
	wcscpy( sString, ShortItemNames[ pInventoryPoolList[ iCurrentSlot + iFirstSlotOnPage ].object.usItem ] );

	if( StringPixLength( sString, MAP_INVEN_NAME_FONT ) >= ( MAP_INVEN_SLOT_WIDTH ) )
	{
		ReduceStringLength( sString, ( INT16 )( MAP_INVEN_SLOT_WIDTH - StringPixLength( L" ...", MAP_INVEN_NAME_FONT ) ), MAP_INVEN_NAME_FONT );
	}

	FindFontCenterCoordinates( (INT16)( 4 + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCurrentSlot / MAP_INV_SLOT_COLS ) ) ),
		0, MAP_INVEN_SLOT_WIDTH, 0,
		sString, MAP_INVEN_NAME_FONT,
		&sWidth, &sHeight );

	SetFontDestBuffer( guiSAVEBUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	SetFont( MAP_INVEN_NAME_FONT );
	SetFontForeground( FONT_GRAY2 );
	SetFontBackground( FONT_BLACK );

	// HEADROCK HAM 5: Offset is now changeable.
	mprintf( sWidth,
		( INT16 )( ITEMDESC_ITEM_NAME_POOL_OFFSET_Y + ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y + MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCurrentSlot % ( MAP_INV_SLOT_COLS ) ) ) )
	, sString );

	// HEADROCK HAM 5: If ALT is pressed, draw sale price.
	if( _KeyDown( ALT ) )
	{
		UINT32 uiSalePriceFont = (fMapInventoryZoom ? FONT12ARIAL : FONT10ARIAL);
		SetFont( uiSalePriceFont );

		UINT16 usFontHeight = GetFontHeight( uiSalePriceFont );

		SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

		INT16 sOffsetX = (fMapInventoryZoom ? 12 : 10);
		INT16 sOffsetY = (fMapInventoryZoom ? 5 : 3);

		sX = (INT16)(sOffsetX + MAP_INVENTORY_POOL_SLOT_START_X + ((MAP_INVEN_SPACE_BTWN_SLOTS)* (iCurrentSlot / MAP_INV_SLOT_COLS)));
		sY = (INT16)(sOffsetY + MAP_INVENTORY_POOL_SLOT_START_Y + ((MAP_INVEN_SLOT_HEIGHT)* (iCurrentSlot % MAP_INV_SLOT_COLS)));

		if ( gGameExternalOptions.fSellAll )
		{
			SetFontForeground( FONT_LTRED );
			SetFontBackground( FONT_BLACK );

			INT32 iPrice = SellItem( pInventoryPoolList[iCurrentSlot + iFirstSlotOnPage].object, _KeyDown( SHIFT ) );

			swprintf( sString, L"$%d", iPrice );

			mprintf( sX, sY, sString );
			sY += usFontHeight + 2;
		}

		if ( gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory )
		{
			FLOAT dvalue_Gun = 0;
			FLOAT dvalue_Armour = 0;
			FLOAT dvalue_Misc = 0;

			if ( ConvertItemToResources( pInventoryPoolList[iCurrentSlot + iFirstSlotOnPage].object, _KeyDown( SHIFT ), dvalue_Gun, dvalue_Armour, dvalue_Misc ) )
			{
				if ( dvalue_Gun > 0.0001f )
				{
					SetFontForeground( FONT_ORANGE );
					SetFontBackground( FONT_BLACK );

					swprintf( sString, L"%-5.2f", dvalue_Gun );
					mprintf( sX, sY, sString );
					sY += usFontHeight + 2;
				}

				if ( dvalue_Armour > 0.0001f )
				{
					SetFontForeground( FONT_LTKHAKI );
					SetFontBackground( FONT_BLACK );

					swprintf( sString, L"%-5.2f", dvalue_Armour );
					mprintf( sX, sY, sString );
					sY += usFontHeight + 2;
				}

				if ( dvalue_Misc > 0.0001f )
				{
					SetFontForeground( FONT_LTBLUE );
					SetFontBackground( FONT_BLACK );

					swprintf( sString, L"%-5.2f", dvalue_Misc );
					mprintf( sX, sY, sString );
					sY += usFontHeight + 2;
				}
			}
		}
	}

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	return( TRUE );
}


void UpdateHelpTextForInvnentoryStashSlots( void )
{
	CHAR16 pTemp[ 512 ], pStr[ 512 ];
	INT32 iCounter = 0;
	INT32 iFirstSlotOnPage = ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT );


	// run through list of items in slots and update help text for mouse regions
	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
	{
		if( pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object.exists() == true )
		{
			GetHelpTextForItem( pTemp , &( pInventoryPoolList[ iCounter + iFirstSlotOnPage ].object ), NULL );
			swprintf( pStr, L"%s\n \n%s", pTemp, gzMiscItemStatsFasthelp[ 34 ] );
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

// Flugente: 
extern void EquipmentListMenuCancel( );

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
		// HEADROCK HAM 5: Reset the offsets!
		ResetMapInventoryOffsets();

		if( ( gWorldSectorX == sSelMapX ) && ( gWorldSectorY == sSelMapY ) && ( gbWorldSectorZ == iCurrentMapSectorZ ) )
		{
			// handle all reachable before save
			HandleAllReachAbleItemsInTheSector( gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
		}

		// destroy buttons for map border
		if (isWidescreenUI())
		{
			DisableMapBorderButtons();
		}
		else
		{
			DeleteMapBorderButtons( );
		}

		// delete map level markers regions
		DeleteMouseRegionsForLevelMarkers( );

		fCreated = TRUE;

		// also create the inventory slot
		CreateMapInventoryPoolSlots( );

		// create buttons
		CreateMapInventoryButtons( );

		// Flugente: certain features need to alter an item's temperature value depending on the time passed
		// if we do these functions here and adjust for the time passed since this sector was loaded last, it will seem to the player
		// as if these checks are always performed in any sector
		//Moa: removed this function and replaced by HandleSectorCooldownFunctions() below
		//SectorInventoryCooldownFunctions(sSelMapX, sSelMapY, ( INT16 )( iCurrentMapSectorZ ));

		// build stash
		BuildStashForSelectedSector( sSelMapX, sSelMapY, ( INT16 )( iCurrentMapSectorZ ) );

		//Moa: added the handling function instead of SectorInventoryCooldown, which has loaded the items from the tempfile (loading also done in BuildStashForSelectedSector)
		HandleSectorCooldownFunctions( sSelMapX, sSelMapY, (INT8)iCurrentMapSectorZ, pInventoryPoolList, pInventoryPoolList.size(), TRUE );//dnl ch75 271013
		//SetLastTimePlayerWasInSector( sSelMapX, sSelMapY, (INT8)iCurrentMapSectorZ );
		//SaveWorldItemsToTempItemFile( sSelMapX, sSelMapY, (INT8)iCurrentMapSectorZ, pInventoryPoolList.size(), (WORLDITEM*) &(*pInventoryPoolList.begin()) );

		// HEADROCK HAM 5: Reset all BigItem image numbers, just in case.
		ResetAllMapInventoryBigItemGraphics();

		CreateMapInventoryPoolDoneButton( );

		fMapPanelDirty = TRUE;
		fMapScreenBottomDirty = TRUE;
	}
	else if( ( fShowMapInventoryPool == FALSE ) && ( fCreated == TRUE ) )
	{
		if(gGameExternalOptions.fEnableInventoryPoolQ)//dnl ch51 081009
			SwitchToInventoryPoolQ(0);

		// check if we are in fact leaving mapscreen
		if( fExitFromMapScreen == FALSE )
		{
			// recreate mapborder buttons
			if (isWidescreenUI())
			{
				EnableMapBorderButtons();
			}
			else
			{
				CreateButtonsForMapBorder();
			}
			// recreate map level markers regions
			CreateMouseRegionsForLevelMarkers( );
		}
		fCreated = FALSE;

		// destroy the map inventory slots
		DestroyMapInventoryPoolSlots( );

		// destroy map inventory buttons
		DestroyMapInventoryButtons( );

		DestroyInventoryPoolDoneButton( );

		// HEADROCK HAM 5: Destroy big item graphics.
		if (fMapInventoryZoom)
		{
			UnloadAllMapInventoryBigItemGraphics();
		}
		// HEADROCK HAM 5: Cancel zoom input request, if active.
		if (fWaitingForZoomInput)
		{
			CancelInventoryZoomInput( FALSE ); // Must be false, because the button doesn't exist anymore!
		}

		// Reset number of desired slots so we recalculate page size entirely on the next open.
		giDesiredNumMapInventorySlots = -1;
#ifdef INVFIX_Moa//dnl ch75 311013
		// clear up unseen list
		ClearUpTempUnSeenList( );
#endif
		// undo item decay (will be saved once and only when entering sector)
//		HandleSectorCooldownFunctions( sSelMapX, sSelMapY, (INT8)iCurrentMapSectorZ, (WORLDITEM*) &(*pInventoryPoolList.begin()) , pInventoryPoolList.size(), TRUE , TRUE);

		// silversurfer: temporary bugfix for superheated weapons in loaded sector inventory after saving the game
		// save time we have viewed those items to make the changes by HandleSectorCooldownFunctions() permanent
		SetLastTimePlayerWasInSector( sSelMapX, sSelMapY, (INT8)iCurrentMapSectorZ );

		// now save results
		SaveSeenAndUnseenItems( );
#ifdef INVFIX_Moa//dnl ch75 311013
		DestroyStash( );
#endif
		// HEADROCK HAM 5: Filters!
		guiMapInventoryFilter = IC_MAPFILTER_ALL;

		fMapPanelDirty = TRUE;
		fTeamPanelDirty = TRUE;
		fCharacterInfoPanelDirty = TRUE;
		// HEADROCK HAM 3.6: Yet again, make sure to refresh the bottom panel on closing.
		fMapScreenBottomDirty = TRUE;

		//CHRISL: close stack popup and item description windows
		DeleteItemStackPopup();
		if ( gfInItemDescBox )
		{
			DeleteItemDescriptionBox();
		}
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

#ifdef INVFIX_Moa//dnl ch85 050214
void ClearUpTempUnSeenList( void )//dnl ch75 271013
{
	// save these items and all the others
	if(pUnSeenItems.empty())
		return;
	// build the list based on this
	pSaveList = pUnSeenItems;
	pUnSeenItems.clear();
}
#endif

//////////////////////////////////////
//@brief Saves pSaveList and pInventoryPoolList either into file or into memmory.
// Takes any existing items from pInventoryPoolList and all items from pSaveList to save same into
// file if map is not loaded or memmory if the map is loaded. Make sure to remove all filters before
// calling this function!
//
// Globals which get modified:
//	WORLDITEM* pSaveList						...If successfull pSaveList gets cleared
//	UINT32 uiNumberOfUnSeenItems				...and uiNumberOfUnSeenItems set to 0.
//
// Globals used to save the list:
//	INT16 gWorldSectorX,gWorldSectorY
//	INT8 gWorldSectorZ							...Current loaded sector.
//	INT32 sSelMapX, sSelMapY
//	INT32 iCurrentMapSectorZ					...Current selected sector.
//	std::vector<WORLDITEM> pInventoryPoolList	...The stash of unfiltered Items.
// Notes:
// The original function had set the visible and exist flag for every worlditem which had existing objects, 
// also it has set the WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT using TileIsOutOfBounds() but this is not
// possible since we dont know the boundaries of a unloaded sector, occationally some data got lost. For
// loaded maps this check was not performed...
// In case this function should mimic the old behavier activate the preprocessor instruction(s), in this case
// those checks ARE performed for loaded maps.
//@auth SirTech
//@auth (merged into single loop by Moa)
//////////////////////////////////////
void SaveSeenAndUnseenItems( void )
{
#ifdef INVFIX_Moa//dnl ch75 291013
	std::vector<WORLDITEM> worldItemsSaveList;	//exists() on front and exists() && visible != 0 on back.
	UINT32 iExistingItems = 0;	//fExists

	//build save list and add flag WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT if required.
	worldItemsSaveList.reserve( pInventoryPoolList.size() + uiNumberOfUnSeenItems );

	if ( worldItemsSaveList.capacity() == 0 ) //nothing to do anyways so get out of here
		return;

	//make list of seen items
	for ( UINT32 i = 0; i < pInventoryPoolList.size(); i++ )
	{
		if ( pInventoryPoolList[i].fExists )
		{
			worldItemsSaveList.push_back(pInventoryPoolList[i]);
			iExistingItems++;
		}
	}

	// no items to save but we need an iterator
	if ( worldItemsSaveList.size() == 0 )
		worldItemsSaveList.resize(1);

	//check whether memory (loaded map) or file is used to save the items
	if( ( gWorldSectorX == sSelMapX ) && ( gWorldSectorY == sSelMapY ) && ( gbWorldSectorZ == ( INT8 ) ( iCurrentMapSectorZ ) ) )
	{
		//handle in existing function
		ReBuildWorldItemStashForLoadedSector( iExistingItems, uiNumberOfUnSeenItems, worldItemsSaveList, pSaveList );//dnl ch75 271013
	}
	else
	{
		//copy remaining unseen items into savelist
		if ( uiNumberOfUnSeenItems > 0 && !pSaveList.empty() )//dnl ch75 271013
			worldItemsSaveList.insert( worldItemsSaveList.end(), pSaveList.begin(), pSaveList.end() );

		//save the items to file (skipping all checks of AddWorldItemsToUnLoadedSector(), allready done here!, also we do not delete the file before we overwrite)
		if ( !SaveWorldItemsToTempItemFile( sSelMapX, sSelMapY, ( INT8 ) iCurrentMapSectorZ, uiNumberOfUnSeenItems + iExistingItems, worldItemsSaveList ) )//dnl ch75 271013
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Error: Could not save %d seen and %d unseen items to disc! Please Report.", iExistingItems, uiNumberOfUnSeenItems );
			return;
		}
	}
	uiNumberOfUnSeenItems = 0;
	pSaveList.clear();//dnl ch75 271013
#else
	UINT32 i, uiTotalNumberOfVisibleItems, uiNumOfSlots, uiNumberOfSeenItems;
	WORLDITEM *pipl;

	// Idea of this change is avoiding resize and clear of pInventoryPool and pUnSeenItems and twice loading tempfile to increase inventory closing time
	uiNumOfSlots = min(MAP_INVENTORY_POOL_SLOT_COUNT * ((UINT32)iLastInventoryPoolPage + 1), pInventoryPoolList.size());// Best guess without going into loop
	uiNumberOfSeenItems = 0;
	uiTotalNumberOfVisibleItems = 0;
	for(i=0; i<uiNumOfSlots; i++)// Calculate total number of objects and throw out empty item slots
	{
		pipl = &pInventoryPoolList[i];
		if(pipl->fExists && pipl->object.ubNumberOfObjects)
		{
			uiTotalNumberOfVisibleItems += pipl->object.ubNumberOfObjects;
			if(i > uiNumberOfSeenItems)
				pInventoryPoolList[uiNumberOfSeenItems] = *pipl;
			uiNumberOfSeenItems++;
		}
	}
	uiNumOfSlots = ((uiNumberOfSeenItems + uiNumberOfUnSeenItems) / MAP_INVENTORY_POOL_SLOT_COUNT + 1) * MAP_INVENTORY_POOL_SLOT_COUNT;
	if(pInventoryPoolList.size() < uiNumOfSlots)
		pInventoryPoolList.resize(uiNumOfSlots);
	for(i=0; i<uiNumberOfUnSeenItems; i++)
		pInventoryPoolList[uiNumberOfSeenItems+i] = pUnSeenItems[i];
	if(gWorldSectorX == sSelMapX && gWorldSectorY == sSelMapY && gbWorldSectorZ == iCurrentMapSectorZ)
	{
		TrashWorldItems();
		SetNumberOfVisibleWorldItemsInSectorStructureForSector(gWorldSectorX, gWorldSectorY, gbWorldSectorZ, uiTotalNumberOfVisibleItems);
		RefreshItemPools(pInventoryPoolList, uiNumberOfSeenItems + uiNumberOfUnSeenItems);
	}
	else
	{
		UpdateWorldItems(sSelMapX, sSelMapY, iCurrentMapSectorZ, uiNumberOfSeenItems + uiNumberOfUnSeenItems, pInventoryPoolList);
	}
#endif
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

void CreateMapInventoryPoolSlots( )
{
	INT32 iCounter = 0;
	INT16 sX = 0, sY = 0;
	INT16 sXA = 0, sYA = 0;
	INT16 sULX = 0, sULY = 0;
	INT16 sBRX = 0, sBRY = 0;
	extern MOUSE_REGION gMapViewRegion;
	//Moa: removed MapInventoryPoolMask, instead we disable map mouseregion and enable again when deleting stash regions
	MSYS_DisableRegion( &gMapViewRegion );
	//MSYS_DefineRegion( &MapInventoryPoolMask,
	//		MAP_INVENTORY_POOL_SLOT_START_X, 0, SCREEN_WIDTH - MAP_INVENTORY_POOL_SLOT_START_X, SCREEN_HEIGHT - 120,
	//		MSYS_PRIORITY_HIGH, MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MapInvenPoolScreenMaskCallback);

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
	extern MOUSE_REGION gMapViewRegion;

	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; iCounter++ )
	{
		MSYS_RemoveRegion(  &MapInventoryPoolSlots[ iCounter ] );
	}

	// remove map inventory mask
	//MSYS_RemoveRegion( &MapInventoryPoolMask );
	MSYS_EnableRegion( &gMapViewRegion );
}

void MapInvenPoolSlotsMove( MOUSE_REGION * pRegion, INT32 iReason  )
{
	INT32 iCounter = 0;


	iCounter = MSYS_GetRegionUserData( pRegion, 0 );

	// HEADROCK HAM 5: While in inventory zoom input mode, we do not draw item compatibility. Instead, mousing
	// over the inventory shows which items from the current page will appear on the zoomed page. This information
	// is calculated below.
	if (!fMapInventoryZoom && fWaitingForZoomInput)
	{
		//iCounter indicates the position of our item in the current (unzoomed) pool page.

		// Get the total number of slots shown on each Zoomed Page.
		UINT16 usSlotsOnZoomedPage = GetInventorySlotCount( TRUE );

		// Find our item's location in the entire sector pool.
		UINT16 usItemLocationInPool = iCounter + (iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT);
		
		// Which page would it be on when zoomed?
		UINT16 usZoomedPage = usItemLocationInPool / usSlotsOnZoomedPage;
		// Which position would it be in on that page?
		UINT16 usPlaceOnZoomedPage = usItemLocationInPool % usSlotsOnZoomedPage;

		// Alright, now lets figure out what the first item on that page will be.
		INT16 sFirstItemToHilite = (usZoomedPage * usSlotsOnZoomedPage);
		// Where is it on the page we're looking at currently?
		sFirstItemToHilite = sFirstItemToHilite - (iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT);
		// We're not interested in items that are not currently on the screen...
		sFirstItemToHilite = __max(0, sFirstItemToHilite );

		// And which is the last item that will be on the zoomed page?
		INT16 sLastItemToHilite = ((usZoomedPage+1) * usSlotsOnZoomedPage) - 1;
		// Where is it on the page we're looking at currently?
		sLastItemToHilite = sLastItemToHilite - (iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT);
		// We're not interested in items that are not currently on the screen...
		sLastItemToHilite = __min(((iCurrentInventoryPoolPage+1) * MAP_INVENTORY_POOL_SLOT_COUNT), sLastItemToHilite );

		// Run from the first to the last item, setting them to have an outline displayed.
		BOOLEAN fAnyObjectsExist = FALSE;
		for (UINT16 x = 0; x < MAP_INVENTORY_POOL_SLOT_COUNT; x++)
		{
			if (x >= sFirstItemToHilite && x <= sLastItemToHilite)
			{
				gfMapInventoryItemToZoom[x] = TRUE;
			}
			else
			{
				gfMapInventoryItemToZoom[x] = FALSE;
			}
		}

		/*
		for (UINT8 x = ubColumnOfFirstItem; x <= ubColumnOfLastItem; x++)
		{
			INT16 top = 0;
			INT16 bottom = 0;
			INT16 left = 0;
			INT16 right = 0;

			if (x == ubColumnOfFirstItem)
			{
				top = ( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y + ( MAP_INVEN_SLOT_HEIGHT * ubPositionFirstItem ) );
			}
			else
			{
				top = ( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y );
			}

			if (x == ubColumnOfLastItem)
			{
				bottom = ( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y + ( MAP_INVEN_SLOT_HEIGHT * (ubPositionLastItem+1) ) );
			}
			else
			{
				bottom = ( INT16 )( MAP_INVENTORY_POOL_SLOT_START_Y + ( MAP_INVEN_SLOT_HEIGHT * (MAP_INV_SLOT_COLS+1) ) );
			}
				
			left = ( INT16 )( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( MAP_INVEN_SPACE_BTWN_SLOTS * x ) );
			right = ( INT16 )( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( MAP_INVEN_SPACE_BTWN_SLOTS * (x+1) ) );

			INT16 sColor = Get16BPPColor( FROMRGB( 255, 255, 255 ) );
			DrawItemOutlineZoomedInventory( left, top, right, bottom, sColor, guiSAVEBUFFER );
		}
		*/

		fMapPanelDirty = TRUE;
	}
	else
	{
		if( iReason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
		{
			if (gpItemPointer == NULL)
			{
				iCurrentlyHighLightedItem = iCounter;
				fChangedInventorySlots = TRUE;
				gfCheckForCursorOverMapSectorInventoryItem = TRUE;
			}
		}
		else if( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
		{
			if (gpItemPointer == NULL)
			{
				iCurrentlyHighLightedItem = NO_SLOT;
				fChangedInventorySlots = TRUE;
				gfCheckForCursorOverMapSectorInventoryItem = FALSE;
			}
			// re render radar map
			RenderRadarScreen( );
		}
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
	extern OBJECTTYPE	*gpItemDescObject;

	iCounter = MSYS_GetRegionUserData( pRegion, 0 );

	if( ( iReason & MSYS_CALLBACK_REASON_RBUTTON_UP ) )
	{
		// HEADROCK HAM 5: If in Zoom Input mode, cancel it
		if (fWaitingForZoomInput)
		{
			CancelInventoryZoomInput( TRUE );
			
			return;
		}
		if(gGameExternalOptions.fSectorDesc == TRUE)
		{
			//CHRISL: Make it possible to right click and pull up stack popup and/or item description boxes
			WORLDITEM	* twItem = &(pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ]);
			bool	fValidPointer = false;
			//CHRISL: Try to update InSector value so we don't have to "activate" a sector
			if(MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->sSectorX == sSelMapX && MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->sSectorY == sSelMapY && MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->bSectorZ == iCurrentMapSectorZ && !MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->flags.fBetweenSectors)
			{
				MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->bInSector=TRUE;
			}
			else
			{
				// HEADROCK HAM 5: Unreachable items will never show an infobox.
				return;
			}
			// HEADROCK HAM 5: Unreachable items will never show an infobox.
			if (!(twItem->usFlags & WORLD_ITEM_REACHABLE))
			{
				return;
			}
			if ( !InSectorStackPopup( ) && !InItemStackPopup( ) /*&& !InItemDescriptionBox( ) */ && !InKeyRingPopup( ) && twItem->object.exists() == true && (bSelectedInfoChar != -1 && gCharactersList[bSelectedInfoChar].fValid) )
			{
				if(OK_CONTROL_MERC( MercPtrs[gCharactersList[bSelectedInfoChar].usSolID] ))
				{
					//CHRISL: The old setup had a flaw I didn't consider.  if, for some reason, the ItemSlotLimit = 0
					//	nothing might happen.  Now we setup a flag to determine if our cursor is value for the item we're
					//	clicking on.  If it's not valid, we can't do anything so all conditions rely on it being true.
					//if(ItemSlotLimit( &twItem->object, STACK_SIZE_LIMIT ) == 1)
					if(gpItemPointer == NULL)
						fValidPointer = true;
					else
					{
						if(gpItemPointer->usItem == twItem->object.usItem)
							fValidPointer = true;
						if(ValidAttachment(gpItemPointer->usItem, &(twItem->object)) == TRUE)
							fValidPointer = true;
						if(ValidAmmoType(twItem->object.usItem, gpItemPointer->usItem) == TRUE)
							fValidPointer = true;
					}

					// Flugente: convert gear to resources
					if ( _KeyDown( ALT ) && gGameExternalOptions.fMilitiaResources && !gGameExternalOptions.fMilitiaUseSectorInventory )
					{
						FLOAT dvalue_Gun = 0;
						FLOAT dvalue_Armour = 0;
						FLOAT dvalue_Misc = 0;

						if (_KeyDown( SHIFT ))
						{
							// convert all items in slot
							twItem->object.MoveThisObjectTo(gItemPointer,-1,0,NUM_INV_SLOTS,MAX_OBJECTS_PER_SLOT);
						}
						else
						{
							twItem->object.MoveThisObjectTo(gItemPointer, 1);
						}

						fMapPanelDirty = TRUE;
						UINT16 usItem = gItemPointer.usItem;

						if ( ConvertItemToResources( gItemPointer, _KeyDown( SHIFT ), dvalue_Gun, dvalue_Armour, dvalue_Misc ) )
						{
							AddResources( dvalue_Gun, dvalue_Armour, dvalue_Misc );

							dvalue_Gun = dvalue_Armour = dvalue_Misc = 0;

							PlayJA2Sample( COMPUTER_BEEP2_IN, RATE_11025, 15, 1, MIDDLEPAN );
							gpItemPointer = NULL;
							fMapInventoryItem = FALSE;

							ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, szSMilitiaResourceText[0], Item[usItem].szItemName );
						}
			
						if ( _KeyDown( 89 ) )
						{
							for ( UINT32 iNumber = 0; iNumber < pInventoryPoolList.size( ); ++iNumber )
							{
								if ( pInventoryPoolList[iNumber].object.usItem == usItem && pInventoryPoolList[iNumber].usFlags & WORLD_ITEM_REACHABLE )
								{
									if ( ConvertItemToResources( pInventoryPoolList[iNumber].object, TRUE, dvalue_Gun, dvalue_Armour, dvalue_Misc ) )
									{
										AddResources( dvalue_Gun, dvalue_Armour, dvalue_Misc );

										dvalue_Gun = dvalue_Armour = dvalue_Misc = 0;

										DeleteObj( &pInventoryPoolList[iNumber].object );
									}
								}
							}
						}
			
						if ( fShowMapInventoryPool )
							HandleButtonStatesWhileMapInventoryActive( );
					}
					// access description box directly if CTRL is pressed for stack items
					else if((twItem->object.ubNumberOfObjects == 1 || _KeyDown( CTRL )) && fValidPointer)
					{
						if (!isWidescreenUI())
						{
							fShowInventoryFlag = TRUE;
						}

						if (InItemDescriptionBox( ))
						{
							DeleteItemDescriptionBox();
						}
						
						// HEADROCK HAM 5: Sector Inventory Item Desc Box no longer accessible during combat.
						if( !CanPlayerUseSectorInventory( &(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ]) ) )
						{
							DoScreenIndependantMessageBox( New113HAMMessage[ 22 ], MSG_BOX_FLAG_OK, NULL );
							return;
						}
						else
						{
							if ( _KeyDown(SHIFT) && gpItemPointer == NULL && Item[twItem->object.usItem].usItemClass == IC_GUN && (twItem->object)[0]->data.gun.ubGunShotsLeft && !ItemIsSingleShotRocketLauncher(twItem->object.usItem))
							{
								EmptyWeaponMagazine( &twItem->object, &gItemPointer );
								InternalMAPBeginItemPointer( MercPtrs[gCharactersList[bSelectedInfoChar].usSolID] );
							}
							else
								MAPInternalInitItemDescriptionBox( &twItem->object, 0, MercPtrs[gCharactersList[bSelectedInfoChar].usSolID] );
						}
					}
					else if(fValidPointer)
					{
						// Sector Inventory Stack Popup no longer accessible during combat.
						if( !CanPlayerUseSectorInventory( &(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ]) ) )
						{
							DoScreenIndependantMessageBox( New113HAMMessage[ 22 ], MSG_BOX_FLAG_OK, NULL );
							return;
						}
						else
						{
							InitSectorStackPopup( MercPtrs[gCharactersList[bSelectedInfoChar].usSolID], twItem, iCounter, xResOffset, yResOffset - 10, 261, ( SCREEN_HEIGHT - PLAYER_INFO_Y ) );
							fTeamPanelDirty=TRUE;
							fInterfacePanelDirty = DIRTYLEVEL2;
						}
					}
				}
			}
		}
		else
		{
			if ( gpItemPointer == NULL )
			{
				fShowMapInventoryPool = FALSE;
			}
			// else do nothing
		}
	}
	else if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// HEADROCK HAM 5: If in Zoom Input mode, enter zoom mode using this item's location to determine
		// which page we want to view.
		if (fWaitingForZoomInput)
		{
			// Figure out which page we need to display.
			UINT16 usSlotsOnZoomedPage = GetInventorySlotCount( TRUE );
			UINT16 usItemLocationInPool = iCounter + (iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT);
			UINT16 usTargetPage = usItemLocationInPool / usSlotsOnZoomedPage;

			giDesiredNumMapInventorySlots = (__max(usTargetPage * usSlotsOnZoomedPage, (UINT16)(pInventoryPoolList.size() / usSlotsOnZoomedPage) + usSlotsOnZoomedPage));

			CancelInventoryZoomInput( FALSE );
			HandleMapInventoryZoom( usTargetPage, iCounter );

			return;	
		}

		// check if item in cursor, if so, then swap, and no item in curor, pick up, if item in cursor but not box, put in box

		if ( gpItemPointer == NULL )
		{
			// Return if empty
			if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.exists() == false )
				return;
			// CHRISL: Also return if the item we've clicked on is currently displayed in the item description box
			if(InItemDescriptionBox( ) && gpItemDescObject == &pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object)
				return;
			// Buggler: sector inventory item deletion before item reachable & in-sector checks to allow deletion of any item
			if ( _KeyDown ( DEL ) )
			{
				if ( _KeyDown ( 89 )) //Lalien: delete all items of this type (Y)
				{
					DeleteItemsOfType( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.usItem );
					ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, New113Message[MSG113_DELETE_ALL] );
				}
				else if ( _KeyDown( SHIFT ) ) // delete stack
				{
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.ubNumberOfObjects = 0;
					ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, New113Message[MSG113_DELETED] );
				}
				else
				{
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object.ubNumberOfObjects--;
					ScreenMsg( FONT_MCOLOR_LTRED, MSG_INTERFACE, New113Message[MSG113_DELETED] );
				}

				fMapPanelDirty = TRUE;
				return;
			}
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
					if ( _KeyDown ( TAB ) && _KeyDown ( CTRL ) && fShowMoveItem )
					{
						if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE )
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags &= ~WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE;
						else
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE;
					}
					else if ( _KeyDown ( TAB ) && gGameExternalOptions.fMilitiaUseSectorInventory )
					{
						pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object[0]->data.sObjectFlag &= ~(TAKEN_BY_MILITIA_TABOO_GREEN|TAKEN_BY_MILITIA_TABOO_BLUE);

						if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ELITE )
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags &= ~WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ALL;
						else if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE )
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ELITE;
						else if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN )
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE;
						else
						{
							pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN;

							// if we do not use class specific taboos, set all flags at once
							if ( !gGameExternalOptions.fMilitiaUseSectorClassSpecificTaboos )
								pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ALL;
						}
					}
					else
					{
						swprintf( sString, pMapInventoryErrorString[ 2 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
						DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
					}
				}
				else
				{
					swprintf( sString, pMapInventoryErrorString[ 5 ], Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ].name );
					DoMapMessageBox( MSG_BOX_BASIC_STYLE, sString, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
				}				
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

			iCurrentlyPickedUpItem = ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter;

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

			if ( _KeyDown ( TAB ) && _KeyDown ( CTRL ) && fShowMoveItem )
			{
				if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE )
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags &= ~WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE;
				else
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_MOVE_ASSIGNMENT_IGNORE;
			}
			else if ( _KeyDown ( TAB ) && gGameExternalOptions.fMilitiaUseSectorInventory )
			{
				pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object[0]->data.sObjectFlag &= ~(TAKEN_BY_MILITIA_TABOO_GREEN|TAKEN_BY_MILITIA_TABOO_BLUE);

				if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ELITE )
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags &= ~WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ALL;
				else if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE )
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ELITE;
				else if ( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags & WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN )
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_BLUE;
				else
				{
					pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_GREEN;

					// if we do not use class specific taboos, set all flags at once
					if ( !gGameExternalOptions.fMilitiaUseSectorClassSpecificTaboos )
						pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].usFlags |= WORLD_ITEM_TABOO_FOR_MILITIA_EQ_ALL;
				}
			}
			else
				BeginInventoryPoolPtr( &( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object ) );
		}
		else
		{//we have an item on cursor

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

			//CHRISL: Allow CTRL+LMB to clean up stacks just like we do with personal inventory
			//Curretly, screen isn't automatically refreshing.  You have to move your cursor after CTRL+LMB to see the results.
			if ( _KeyDown(CTRL) )
			{
				CleanUpStack( &( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object ), gpItemPointer );
				if ( gpItemPointer->exists() == false )
				{
					MAPEndItemPointer( );
				}
				return;
			}

			// Else, try to place here
			if ( PlaceObjectInInventoryStash( &( pInventoryPoolList[ ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter ].object ), gpItemPointer,
				( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) + iCounter, iCurrentlyPickedUpItem ) )
			{
				// nothing here before, then place here
				if (iOldNumberOfObjects == 0)
				{			
					INT32 sGridNo = -1;
					INT8 bLevel = 0;

					// is this sector loaded?
					if (gpItemPointerSoldier && (gpItemPointerSoldier->sSectorX == gWorldSectorX) && (gpItemPointerSoldier->sSectorY == gWorldSectorY) && (gpItemPointerSoldier->bSectorZ == gbWorldSectorZ))
					{
						sGridNo = gpItemPointerSoldier->sGridNo;
						bLevel = gpItemPointerSoldier->pathing.bLevel;
					}
					// set a grid no for item from mercs with invalid grid no in sector inventory, e.g. merc arriving in sector with a different tactical map loaded
					if (sGridNo != 0)
					{
						// set as reachable and set gridno
						pInventoryPoolList[(iCurrentInventoryPoolPage*MAP_INVENTORY_POOL_SLOT_COUNT) + iCounter].sGridNo = sGridNo;
						pInventoryPoolList[(iCurrentInventoryPoolPage*MAP_INVENTORY_POOL_SLOT_COUNT) + iCounter].usFlags |= WORLD_ITEM_REACHABLE;
						pInventoryPoolList[(iCurrentInventoryPoolPage*MAP_INVENTORY_POOL_SLOT_COUNT) + iCounter].ubLevel = bLevel;
						pInventoryPoolList[(iCurrentInventoryPoolPage*MAP_INVENTORY_POOL_SLOT_COUNT) + iCounter].bVisible = 1;
						pInventoryPoolList[(iCurrentInventoryPoolPage*MAP_INVENTORY_POOL_SLOT_COUNT) + iCounter].fExists = TRUE;
					}
					if (sGridNo == -1)
					{
						pInventoryPoolList[(iCurrentInventoryPoolPage*MAP_INVENTORY_POOL_SLOT_COUNT) + iCounter].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
					}
				}

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
					gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

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
	InitMapInventoryCoordinates();

	guiMapInvenArrowButtonImage[ 0 ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" , 10, 1, -1, 3, -1 );
	guiMapInvenArrowButton[ 0 ] = QuickCreateButton( guiMapInvenArrowButtonImage[ 0 ], MAPINV_NEXT_X, MAPINV_ARROW_Y,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
									(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapInventoryPoolNextBtn );


	guiMapInvenArrowButtonImage[ 1 ]=  LoadButtonImage( "INTERFACE\\map_screen_bottom_arrows.sti" ,9, 0, -1, 2, -1 );
	guiMapInvenArrowButton[ 1 ] = QuickCreateButton( guiMapInvenArrowButtonImage[ 1 ], MAPINV_PREV_X, MAPINV_ARROW_Y,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
									(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapInventoryPoolPrevBtn );

	// set up fast help text
	SetButtonFastHelpText( guiMapInvenArrowButton[ 0 ], pMapScreenInvenButtonHelpText[ 0 ] );
	SetButtonFastHelpText( guiMapInvenArrowButton[ 1 ], pMapScreenInvenButtonHelpText[ 1 ] );

	// HEADROCK HAM 5: Zoom Button
	// Currently available only at 800x600 resolution or higher. There is simply not enough space for more than 6
	// bigitem slots on 640x480.
	if (iResolution >= _800x600 )
	{
		guiMapInvenZoomButtonImage =  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 0, 0, -1, 1, -1 );
		guiMapInvenZoomButton = QuickCreateButton( guiMapInvenZoomButtonImage, MAPINV_ZOOM_X, MAPINV_ZOOM_Y,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
											NULL, (GUI_CALLBACK)MapInventoryPoolZoomBtn );

		SetButtonFastHelpText( guiMapInvenZoomButton, pMapScreenInvenButtonHelpText[ 3 ] );
		// Set to current selected state.
		if (fMapInventoryZoom)
		{
			ButtonList[ guiMapInvenZoomButton ]->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}

	// HEADROCK HAM 5: Stack and Merge button
	guiMapInvenSortButtonImage[ 0 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 16, 14, -1, 15, -1 );
	guiMapInvenSortButton[ 0 ] = QuickCreateButton( guiMapInvenSortButtonImage[ 0 ], MAPINV_STACK_X, MAPINV_STACK_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolStackAndMergeBtn );

	SetButtonFastHelpText( guiMapInvenSortButton[ 0 ], pMapScreenInvenButtonHelpText[ 4 ] );

	// HEADROCK HAM 5: Sort Ammo Button
	guiMapInvenSortButtonImage[ 1 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 4, 2, -1, 3, -1 );
	guiMapInvenSortButton[ 1 ] = QuickCreateButton( guiMapInvenSortButtonImage[ 1 ], MAPINV_SORT_AMMO_X, MAPINV_SORT_AMMO_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolSortAmmoBtn );

	SetButtonFastHelpText( guiMapInvenSortButton[ 1 ], pMapScreenInvenButtonHelpText[ 5 ] );

	// HEADROCK HAM 5: Sort Attachments Button
	guiMapInvenSortButtonImage[ 2 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 10, 8, -1, 9, -1 );
	guiMapInvenSortButton[ 2 ] = QuickCreateButton( guiMapInvenSortButtonImage[ 2 ], MAPINV_SORT_ATT_X, MAPINV_SORT_ATT_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolSortAttachmentsBtn );

	SetButtonFastHelpText( guiMapInvenSortButton[ 2 ], pMapScreenInvenButtonHelpText[ 6 ] );

	// HEADROCK HAM 5: Eject Ammo Button
	guiMapInvenSortButtonImage[ 3 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 13, 11, -1, 12, -1 );
	guiMapInvenSortButton[ 3 ] = QuickCreateButton( guiMapInvenSortButtonImage[ 3 ], MAPINV_EJECT_AMMO_X, MAPINV_EJECT_AMMO_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolEjectAmmoBtn );

	SetButtonFastHelpText( guiMapInvenSortButton[ 3 ], pMapScreenInvenButtonHelpText[ 7 ] );

	// HEADROCK HAM 5: Filter Toggle Button
	guiMapInvenFilterButtonImage[ 0 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 7, 5, -1, 6, -1 );
	guiMapInvenFilterButton[ 0 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 0 ], MAPINV_FILTER_X, MAPINV_FILTER_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 0 ], pMapScreenInvenButtonHelpText[ 8 ] );
	ButtonList[ guiMapInvenFilterButton[ 0 ] ]->UserData[0] = 0;
	ButtonList[ guiMapInvenFilterButton[ 0 ] ]->UserData[1] = IC_MAPFILTER_ALL;
	ButtonList[ guiMapInvenFilterButton[ 0 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 0 ] ]->UserData[3] = 0;
	guiMapInvenFilterButtonDefined[0] = TRUE;

	guiMapInvenFilterButtonImage[ 1 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 20, 20, -1, 21, -1 );
	guiMapInvenFilterButton[ 1 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 1 ], MAPINV_FILTER_GUN_X, MAPINV_FILTER_GUN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 1 ], pMapScreenInvenButtonHelpText[ 9 ] );
	ButtonList[ guiMapInvenFilterButton[ 1 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 1 ] ]->UserData[1] = IC_MAPFILTER_GUN;
	ButtonList[ guiMapInvenFilterButton[ 1 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 1 ] ]->UserData[3] = IC_MAPFILTER_GUN;
	guiMapInvenFilterButtonDefined[1] = TRUE;

	guiMapInvenFilterButtonImage[ 2 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 22, 22, -1, 23, -1 );
	guiMapInvenFilterButton[ 2 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 2 ], MAPINV_FILTER_AMMO_X, MAPINV_FILTER_AMMO_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 2 ], pMapScreenInvenButtonHelpText[ 10 ] );
	ButtonList[ guiMapInvenFilterButton[ 2 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 2 ] ]->UserData[1] = IC_MAPFILTER_AMMO;
	ButtonList[ guiMapInvenFilterButton[ 2 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 2 ] ]->UserData[3] = IC_MAPFILTER_AMMO;
	guiMapInvenFilterButtonDefined[2] = TRUE;

	guiMapInvenFilterButtonImage[ 3 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 24, 24, -1, 25, -1 );
	guiMapInvenFilterButton[ 3 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 3 ], MAPINV_FILTER_EXPLOSIVE_X, MAPINV_FILTER_EXPLOSIVE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 3 ], pMapScreenInvenButtonHelpText[ 11 ] );
	ButtonList[ guiMapInvenFilterButton[ 3 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 3 ] ]->UserData[1] = IC_MAPFILTER_EXPLOSV;
	ButtonList[ guiMapInvenFilterButton[ 3 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 3 ] ]->UserData[3] = IC_MAPFILTER_EXPLOSV;
	guiMapInvenFilterButtonDefined[3] = TRUE;

	guiMapInvenFilterButtonImage[ 4 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 26, 26, -1, 27, -1 );
	guiMapInvenFilterButton[ 4 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 4 ], MAPINV_FILTER_MELEE_X, MAPINV_FILTER_MELEE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 4 ], pMapScreenInvenButtonHelpText[ 12 ] );
	ButtonList[ guiMapInvenFilterButton[ 4 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 4 ] ]->UserData[1] = IC_MAPFILTER_MELEE;
	ButtonList[ guiMapInvenFilterButton[ 4 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 4 ] ]->UserData[3] = IC_MAPFILTER_MELEE;
	guiMapInvenFilterButtonDefined[4] = TRUE;

	guiMapInvenFilterButtonImage[ 5 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 28, 28, -1, 29, -1 );
	guiMapInvenFilterButton[ 5 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 5 ], MAPINV_FILTER_ARMOR_X, MAPINV_FILTER_ARMOR_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 5 ], pMapScreenInvenButtonHelpText[ 13 ] );
	ButtonList[ guiMapInvenFilterButton[ 5 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 5 ] ]->UserData[1] = IC_MAPFILTER_ARMOR;
	ButtonList[ guiMapInvenFilterButton[ 5 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 5 ] ]->UserData[3] = IC_MAPFILTER_ARMOR;
	guiMapInvenFilterButtonDefined[5] = TRUE;

	guiMapInvenFilterButtonImage[ 6 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 30, 30, -1, 31, -1 );
	guiMapInvenFilterButton[ 6 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 6 ], MAPINV_FILTER_LBE_X, MAPINV_FILTER_LBE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 6 ], pMapScreenInvenButtonHelpText[ 14 ] );
	ButtonList[ guiMapInvenFilterButton[ 6 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 6 ] ]->UserData[1] = IC_MAPFILTER_LBE;
	ButtonList[ guiMapInvenFilterButton[ 6 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 6 ] ]->UserData[3] = IC_MAPFILTER_LBE;
	guiMapInvenFilterButtonDefined[6] = TRUE;

	guiMapInvenFilterButtonImage[ 7 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 32, 32, -1, 33, -1 );
	guiMapInvenFilterButton[ 7 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 7 ], MAPINV_FILTER_KIT_X, MAPINV_FILTER_KIT_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 7 ], pMapScreenInvenButtonHelpText[ 15 ] );
	ButtonList[ guiMapInvenFilterButton[ 7 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 7 ] ]->UserData[1] = IC_MAPFILTER_KIT;
	ButtonList[ guiMapInvenFilterButton[ 7 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 7 ] ]->UserData[3] = IC_MAPFILTER_KIT;
	guiMapInvenFilterButtonDefined[7] = TRUE;

	guiMapInvenFilterButtonImage[ 8 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 34, 34, -1, 35, -1 );
	guiMapInvenFilterButton[ 8 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 8 ], MAPINV_FILTER_MISC1_X, MAPINV_FILTER_MISC1_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtn );

	SetButtonFastHelpText( guiMapInvenFilterButton[ 8 ], pMapScreenInvenButtonHelpText[ 16 ] );
	ButtonList[ guiMapInvenFilterButton[ 8 ] ]->UserData[0] = 1;
	ButtonList[ guiMapInvenFilterButton[ 8 ] ]->UserData[1] = IC_MAPFILTER_MISC;
	ButtonList[ guiMapInvenFilterButton[ 8 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 8 ] ]->UserData[3] = IC_MAPFILTER_MISC;
	guiMapInvenFilterButtonDefined[8] = TRUE;

	// Flugente: toggle button for move item display
	guiMapInvenFilterButtonImage[ 9 ]=  LoadButtonImage( "INTERFACE\\sector_inventory_buttons.sti" , 36, 36, -1, 37, -1 );
	guiMapInvenFilterButton[ 9 ] = QuickCreateButton( guiMapInvenFilterButtonImage[ 9 ], MAPINV_FILTER_MISC2_X, MAPINV_FILTER_MISC2_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
										NULL, (GUI_CALLBACK)MapInventoryPoolFilterBtnMoveItemDisplay );

	// INVEN_POOL_X+336 is last button that can be accomodated in 640x480 resolution

	SetButtonFastHelpText( guiMapInvenFilterButton[ 9 ], pMapScreenInvenButtonHelpText[ 17 ] );
	ButtonList[ guiMapInvenFilterButton[ 9 ] ]->UserData[0] = 0;
	ButtonList[ guiMapInvenFilterButton[ 9 ] ]->UserData[1] = IC_MAPFILTER_MISC;
	ButtonList[ guiMapInvenFilterButton[ 9 ] ]->UserData[2] = 0;
	ButtonList[ guiMapInvenFilterButton[ 9 ] ]->UserData[3] = IC_MAPFILTER_MISC;
	guiMapInvenFilterButtonDefined[9] = TRUE;

	// as no space is left, further buttons require better resolutions
	if ( iResolution > _640x480 )
	{
		// Flugente: gear templates
		guiMapInvenFilterButtonImage[10] = LoadButtonImage("INTERFACE\\sector_inventory_buttons.sti", 40, 40, -1, 40, -1);
		guiMapInvenFilterButton[10] = QuickCreateButton(guiMapInvenFilterButtonImage[10], MAPINV_FILTER_SAVE_TEMPLATE_X, MAPINV_FILTER_SAVE_TEMPLATE_Y,
			BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
			NULL, (GUI_CALLBACK)MapInventoryWriteEquipmentTemplate);
	
		SetButtonFastHelpText(guiMapInvenFilterButton[10], pMapScreenInvenButtonHelpText[18]);
		guiMapInvenFilterButtonDefined[10] = TRUE;

		guiMapInvenFilterButtonImage[11] = LoadButtonImage("INTERFACE\\sector_inventory_buttons.sti", 42, 42, -1, 42, -1);
		guiMapInvenFilterButton[11] = QuickCreateButton(guiMapInvenFilterButtonImage[11], MAPINV_FILTER_LOAD_TEMPLATE_X, MAPINV_FILTER_LOAD_TEMPLATE_Y,
			BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
			NULL, (GUI_CALLBACK)MapInventoryReadEquipmentTemplate);

		SetButtonFastHelpText(guiMapInvenFilterButton[11], pMapScreenInvenButtonHelpText[19]);
		guiMapInvenFilterButtonDefined[11] = TRUE;
	}

	//reset the current inventory page to be the first page
	iCurrentInventoryPoolPage = 0;

	HandleSetFilterButtons();
}


void DestroyMapInventoryButtons( void )
{
	// Page Arrow Buttons
	RemoveButton( guiMapInvenArrowButton[ 0 ] );
	RemoveButton( guiMapInvenArrowButton[ 1 ] );
	UnloadButtonImage( guiMapInvenArrowButtonImage[ 0 ] );
	UnloadButtonImage( guiMapInvenArrowButtonImage[ 1 ] );

	// HEADROCK HAM 5: Destroy Inventory Zoom Button
	if (iResolution >= _800x600 )
	{
		RemoveButton( guiMapInvenZoomButton );
		UnloadButtonImage( guiMapInvenZoomButtonImage );
	}

	// HEADROCK HAM 5: Auto-Sort buttons
	RemoveButton( guiMapInvenSortButton[ 0 ] );
	UnloadButtonImage( guiMapInvenSortButtonImage[ 0 ] );
	RemoveButton( guiMapInvenSortButton[ 1 ] );
	UnloadButtonImage( guiMapInvenSortButtonImage[ 1 ] );
	RemoveButton( guiMapInvenSortButton[ 2 ] );
	UnloadButtonImage( guiMapInvenSortButtonImage[ 2 ] );
	RemoveButton( guiMapInvenSortButton[ 3 ] );
	UnloadButtonImage( guiMapInvenSortButtonImage[ 3 ] );

	// HEADROCK HAM 5: Filter button
	for (INT32 iCounter = 0; iCounter < MAP_INVENTORY_FILTER_BUTTONS; ++iCounter)
	{
		if ( guiMapInvenFilterButtonDefined[ iCounter ] )
		{
			RemoveButton( guiMapInvenFilterButton[ iCounter ] );
			UnloadButtonImage( guiMapInvenFilterButtonImage[ iCounter ] );
			guiMapInvenFilterButtonDefined[ iCounter ] = FALSE;
		}
	}

	// Flugente: destroy gear template selection, in case that thing is still open
	EquipmentListMenuCancel( );

	/*
	// Destroy the popup.
	delete(gMapInventoryFilterPopup);
	gMapInventoryFilterPopup = NULL;
	gfMapInventoryFilterPopupInitialized = FALSE;
	gfMapInventoryFilterPopupVisible = FALSE;
	*/
}

////////////////////////////
// @brief: Creates filtered and unfiltered lists for stash and stores them in global variables.
//
// If loaded sector matches the parameter sector globals are used to build the stash, otherwise the items are read from temp file (if any)
// Globals used to build the stash
//	INT32 MAP_INVENTORY_POOL_SLOT_COUNT			...Number of Slots per page.
//	INT16 gWorldSectorX,gWorldSectorY
//	INT8 gWorldSectorZ							...Current loaded sector.
//	UINT32 guiNumWorldItems						...Number of itemsstacks in the loaded sector.
//	WORLDITEM * gWorldItems						...Global dynamic array of itemstacks in the loaded sector.
//
// Globals used to save the lists:
//	INT32 iLastInventoryPoolPage				...page index where still one item is displayed (0..)
//	std::vector<WORLDITEM> pInventoryPoolList	...Stores unfiltered items and itemstacks of the sector (only items that return TRUE for @see IsMapScreenWorldItemVisibleInMapInventory() ) vector.size() is 1 + a multiple of MAP_INVENTORY_POOL_SLOT_COUNT.
//	UINT32 uiNumberOfUnSeenItems				...Number of filtered items in that sector (only items that exist and return TRUE for @see IsMapScreenWorldItemInvisibleInMapInventory() ).
//	WORLDITEM *pUnSeenItems						...Stores filtered items in that sector
//
// @calls CheckGridNoOfItemsInMapScreenMapInventory(), SortSectorInventory()
// @Todo: remove CheckGridNoOfItemsInMapScreenMapInventory() by identifying the root cause of the missing flag
//@auth SirTech, ChrisL
//@auth (merged into single loop by Moa)
////////////////////////////
void BuildStashForSelectedSector( INT16 sMapX, INT16 sMapY, INT16 sMapZ )
{
#ifdef INVFIX_Moa//dnl ch75 301013
	UINT32 fNumTotal = 0, fNumShown = 0, fNumFiltered = 0, fNumFlagsSet = 0; //itemstack count
	std::vector<WORLDITEM> fFilteredItems;	//store filtered items to be converted later to an array.
	std::vector<WORLDITEM> fWorldItems;		//store everything here what we can get (WORLDITEM.fExists == TRUE)
	// pInventoryPoolList //store unfiltered items directly into the global


	// if map is allready loaded use gWorldItems[] and guiNumWorldItems to build lists
	if( ( sMapX == gWorldSectorX )&&( gWorldSectorY == sMapY ) &&(gbWorldSectorZ == sMapZ ) )
	{
		// approximate capacitiy
		fFilteredItems.reserve( 50 ); //since we are building the stash from blank, there is probably no filter set and only few items get into fFilteredItems
		fWorldItems.reserve(guiNumWorldItems);
		// start with empty list (global)
		pInventoryPoolList.clear( );
		pInventoryPoolList.reserve( MAP_INVENTORY_POOL_SLOT_COUNT * ( 1 + (guiNumWorldItems/MAP_INVENTORY_POOL_SLOT_COUNT) ) );


		// sector loaded, just copy from list
		for( UINT32 i = 0;  i < guiNumWorldItems; i++ )
		{
			if( IsMapScreenWorldItemVisibleInMapInventory( &gWorldItems[ i ] ) )
			{
				if( ( gWorldItems[i].object.exists() == true ) && ( TileIsOutOfBounds(gWorldItems[ i ].sGridNo) ) && !( gWorldItems[ i ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )		
				{
					//set the flag
#ifndef _DEBUG
					pInventoryPoolList[ i ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
#endif
					fNumFlagsSet++;
				}
				// unfiltered? add to visible and total list
				pInventoryPoolList.push_back( gWorldItems[ i ] );
				fWorldItems.push_back( gWorldItems[ i ] );

				fNumShown++;
			}
			else if ( IsMapScreenWorldItemInvisibleInMapInventory( &gWorldItems[ i ] ) )
			{ 
				if( ( gWorldItems[i].object.exists() == true ) && ( TileIsOutOfBounds(gWorldItems[ i ].sGridNo) ) && !( gWorldItems[ i ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )		
				{
					//set the flag
#ifndef _DEBUG
					gWorldItems[ i ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
#endif
					fNumFlagsSet++;
				}				
				// filtered? add to invisible and total list
				fFilteredItems.push_back( gWorldItems[ i ] );
				fWorldItems.push_back( gWorldItems[ i ] );
			}
			else if ( gWorldItems[ i ].fExists )
			{
				// dont skip it, we reuse this loop to create a complete list of all items
				fWorldItems.push_back( gWorldItems[ i ] );
#ifdef _DEBUG
				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"The %d item in the list is hidden or does not exist.", i );
#endif
			}
		}

		// update countings
		fNumTotal = fWorldItems.size();
		fNumFiltered = fFilteredItems.size();
		fNumShown = pInventoryPoolList.size();

		// double check if counting was correct
#ifdef JA2BETAVERSION
		AssertGE( guiNumWorldItems, fNumTotal );
		AssertGE( fNumTotal, fNumFiltered + fNumShown );
#endif
	}
	else
	{ // if map not loaded, use tempfile to build lists
		
		
		BOOLEAN error;
		// get total number of itemstacks from temp file...
		error = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( fNumTotal ), FALSE );
		Assert( error );

		// ...and use this number to allocate space
		fFilteredItems.reserve( 50 ); //since we are building the stash from blank, there is probably no filter set and only few items get into fFilteredItems, was 8 changed to 50 because of bobby ray
		
		//initialize at least one element to avoid null dereference of iterator
		fWorldItems.resize(fNumTotal+1);

		// start with empty list (global)
		pInventoryPoolList.reserve( MAP_INVENTORY_POOL_SLOT_COUNT * ( 1 + (fNumTotal/MAP_INVENTORY_POOL_SLOT_COUNT) ) );
		pInventoryPoolList.clear( );

		// load items into fWorldItems vector
		if ( !LoadWorldItemsFromTempItemFile( sMapX, sMapY, (INT8) sMapZ, fWorldItems ) )//dnl ch75 271013
		{
			DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String("LoadWorldItemsFromTempItemFile:  failed in BuildStashForSelectedSector()" ) );
			return;
		}

		// was temporay used to get #items in file, now count what actually is added
		fNumTotal = 0;
		// now run through list and copy reference
		for(UINT32 i = 0; i < fWorldItems.size(); ++i )
		{
			// TEST!!  If the item exists, and is NOT VALID, report it
			if( fWorldItems[i].fExists &&  fWorldItems[ i ].object.usItem > gMAXITEMS_READ )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"The %d item in the list is NOT valid (ID%d). Please send save.  DF 1.", i, fWorldItems[i].object.usItem );
			}

			if( IsMapScreenWorldItemVisibleInMapInventory( &fWorldItems[ i ] ) )
			{
				if( ( fWorldItems[i].object.exists() == true ) && ( TileIsOutOfBounds(fWorldItems[ i ].sGridNo) ) && !( fWorldItems[ i ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )		
				{
					//set the flag
#ifndef _DEBUG
					fWorldItems[ i ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
#endif
					fNumFlagsSet++;
				}
				// unfiltered? add to visible
				pInventoryPoolList.push_back( fWorldItems[i] );
				fNumTotal++;
			}
			else if ( IsMapScreenWorldItemInvisibleInMapInventory( &fWorldItems[ i ] ) )
			{
				if( ( fWorldItems[i].object.exists() == true ) && ( TileIsOutOfBounds(fWorldItems[ i ].sGridNo) ) && !( fWorldItems[ i ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )		
				{
					//set the flag
#ifndef _DEBUG
					fWorldItems[ i ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
#endif
					fNumFlagsSet++;
				}
				// filtered? add to invisible and total list
				fFilteredItems.push_back( fWorldItems[ i ] );
				fNumTotal++;
			}
			else if ( fWorldItems[ i ].fExists )
			{
				// dont skip it, we reuse this loop to create a complete list of existing items
				fNumTotal++;
#ifdef _DEBUG
				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"The %d item in the list does not exist.", i );
#endif
			}
		}
		// update countings
		//fNumTotal = fWorldItems.size();
		fNumFiltered = fFilteredItems.size();
		fNumShown = pInventoryPoolList.size();

		// double check if counting was correct
#ifdef JA2BETAVERSION
		AssertGE( fNumTotal, fNumFiltered + fNumShown );
		AssertGE( fWorldItems.size(), fNumTotal );
#endif
	}// done with lists...


	// ... set globals: set index for last page ...
	iLastInventoryPoolPage = (INT32)(fNumShown / MAP_INVENTORY_POOL_SLOT_COUNT);

	uiNumberOfUnSeenItems = fNumFiltered;

	// to copy fFilteredItems (vector) to global (array)
	if (uiNumberOfUnSeenItems > 0)
	{
		//dnl ch75 271013
		pUnSeenItems.resize(uiNumberOfUnSeenItems);//gets passed to pSaveList in ClearUpTempUnSeenList() and deleted by SaveSeenAndUnseenItems()
		std::copy(fFilteredItems.begin(), fFilteredItems.end(), pUnSeenItems.begin());
	}
	//resize globallist to a multiple of MAP_INVENTORY_POOL_SLOT_COUNT + one element (some other code does expect that)
	pInventoryPoolList.resize(MAP_INVENTORY_POOL_SLOT_COUNT * (iLastInventoryPoolPage+1) );

	// Report corrected flags / missing flags
	if( fNumFlagsSet > 0 )
	{
#ifndef _DEBUG
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Item with invalid gridno doesnt have flag set:# %d, this was corrected.", fNumFlagsSet );
#else
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Item with invalid gridno doesnt have flag set:# %d, this was not corrected! Figure out why.", fNumFlagsSet );
#endif
	}

	//Sort the sector inventory
	SortSectorInventory( pInventoryPoolList, fNumShown );
#else
	// Decide to use pointers instead of iterators which slightly decrease inventory loading time when have huge number of items, also pInventoryPoolList and pUnSeenItems will resize only if needed
	UINT32 i, uiTotalNumberOfItems, uiNumberOfSeenItems, uiNumOfSlots;
	WORLDITEM *pwi, *pipl, *pusi;
	if(sMapX == gWorldSectorX && gWorldSectorY == sMapY && gbWorldSectorZ == sMapZ)// if map is already loaded use gWorldItems[] and guiNumWorldItems to build lists
	{
		uiTotalNumberOfItems = guiNumWorldItems;
		uiNumOfSlots = (uiTotalNumberOfItems / MAP_INVENTORY_POOL_SLOT_COUNT + 1) * MAP_INVENTORY_POOL_SLOT_COUNT;
		
		if (pInventoryPoolList.size() < uiNumOfSlots) {
			pInventoryPoolList.resize(uiNumOfSlots);
		}
		
		if (pUnSeenItems.size() < uiNumOfSlots) {
			pUnSeenItems.resize(uiNumOfSlots);
		}
		
		uiNumberOfSeenItems = 0;
		uiNumberOfUnSeenItems = 0;
		
		pwi = &gWorldItems.front();
		pipl = &pInventoryPoolList.front();
		pusi = &pUnSeenItems.front();

		for(i=0; i<uiTotalNumberOfItems; i++, pwi++)
		{
			if(IsMapScreenWorldItemVisibleInMapInventory(pwi))
			{
				*pipl++ = *pwi;
				uiNumberOfSeenItems++;
			}
			else if(pwi->fExists)
			{
				*pusi++ = *pwi;
				uiNumberOfUnSeenItems++;
			}
		}
	}
	else// if map not loaded, use tempfile to build lists
	{
		const auto ii = FindWorldItemSector(sMapX, sMapY, (INT8)sMapZ);
		if (ii == -1)
		{
			uiTotalNumberOfItems = 0;
		}
		else
		{
			uiTotalNumberOfItems = gAllWorldItems.NumItems[ii];
			pInventoryPoolList = gAllWorldItems.Items[ii];
		}
		
		uiNumOfSlots = (uiTotalNumberOfItems / MAP_INVENTORY_POOL_SLOT_COUNT + 1) * MAP_INVENTORY_POOL_SLOT_COUNT;
		if(pInventoryPoolList.size() < uiNumOfSlots)
			pInventoryPoolList.resize(uiNumOfSlots);
		if(pUnSeenItems.size() < uiNumOfSlots)
			pUnSeenItems.resize(uiNumOfSlots);

		uiNumberOfSeenItems = uiTotalNumberOfItems;
		uiNumberOfUnSeenItems = 0;
		pipl = &pInventoryPoolList.front();
		pusi = &pUnSeenItems.front();
		for(i=0; i<uiTotalNumberOfItems; i++, pipl++)
		{
			if(!IsMapScreenWorldItemVisibleInMapInventory(pipl))
			{
				if(pipl->fExists)
				{
					*pusi++ = *pipl;
					uiNumberOfUnSeenItems++;
				}
				pipl->fExists = FALSE;
				pipl->usFlags = 0;
				pipl->object.usItem = NONE;
				pipl->object.ubNumberOfObjects = 0;
			}
		}
	}
	// Clear out rest of the pInventoryPoolList slots
	uiNumOfSlots = pInventoryPoolList.size();
	pipl = &pInventoryPoolList[uiNumberOfSeenItems];
	for(i=uiNumberOfSeenItems; i<uiNumOfSlots; i++, pipl++)
	{
		pipl->fExists = FALSE;
		pipl->usFlags = 0;
		pipl->object.usItem = NONE;
		pipl->object.ubNumberOfObjects = 0;
	}
	SortSectorInventory(pInventoryPoolList, uiNumberOfSeenItems);
#endif
}

void ReBuildWorldItemStashForLoadedSector( INT32 iNumberSeenItems, INT32 iNumberUnSeenItems, std::vector<WORLDITEM>& pSeenItemsList, std::vector<WORLDITEM>& pUnSeenItemsList )//dnl ch75 271013
{
	INT32 iTotalNumberOfItems = 0;
	INT32 iCurrentItem = 0;
	INT32 iCounter = 0;
	INT32 iRemainder = 0;
	UINT32	uiTotalNumberOfVisibleItems=0;
	std::vector<WORLDITEM> pTotalList;//dnl ch75 271013

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
	pTotalList.resize(iTotalNumberOfItems);//dnl ch75 271013

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
}

#ifdef INVFIX_Moa//dnl ch85 050214
void DestroyStash( void )
{
	// clear out stash
	pInventoryPoolList.clear();
}
#endif

void BeginInventoryPoolPtr( OBJECTTYPE *pInventorySlot )
{
	BOOLEAN fOk = FALSE;
	BOOLEAN fShift = FALSE;

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
		fShift = TRUE;
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

		if ( _KeyDown ( CTRL ))//MM: Pass item to selected merc.  Delete if none selected.
		{
			SOLDIERTYPE *pSoldier = &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ];
			bool placedAllObjects = false;

			if(pSoldier->exists() == true)
			{
				UINT8 ubInitialNumberOfObjects = gpItemPointer->ubNumberOfObjects;

				/* // commented for item compare feature for easy autoplace of multiple items
				if (InItemDescriptionBox( ))
					DeleteItemDescriptionBox();*/

				if (!fShowInventoryFlag)
					fShowInventoryFlag = TRUE;
				
				if(pSoldier->flags.uiStatusFlags & SOLDIER_VEHICLE)
				{
					// try to STACK the item in all existing slots
					for(int x = 0; x<NUM_INV_SLOTS; x++)
					{
						if(vehicleInv[x] == FALSE)
							continue;

						if(pSoldier->inv[x].usItem == gpItemPointer->usItem)
						{
							if(TryToStackInSlot(pSoldier, gpItemPointer, x) == true)
							{
									placedAllObjects = true;
									break;
							}
							else
									continue;
						}
					}

					// else place item in an empty slot
					if(gpItemPointer->ubNumberOfObjects == ubInitialNumberOfObjects)
					{
						for(int x = 0; x<NUM_INV_SLOTS; x++)
						{
							if(vehicleInv[x] == FALSE)
								continue;

							if(pSoldier->inv[x].exists() == false)
							{
								gpItemPointer->MoveThisObjectTo(pSoldier->inv[x], gpItemPointer->ubNumberOfObjects, pSoldier, x);
								
								if(gpItemPointer->ubNumberOfObjects == 0)
								{								
									placedAllObjects = true;
									break;
								}
								// vehicle slots holds less items than sector inventory slots
								else if(gpItemPointer->ubNumberOfObjects < ubInitialNumberOfObjects)
									break;
							}
						}
					}
				}
				else if (AM_A_ROBOT(pSoldier))
				{
					// intentionally do nothing for madlab's robot
				}
				else
				{
					if (AutoPlaceObject(pSoldier,gpItemPointer,FALSE,NO_SLOT,TRUE)) //doesn't work for vehicles :p
						if(gpItemPointer->ubNumberOfObjects == 0)
							placedAllObjects = true;
				}

				if (placedAllObjects)
				{
					fShowInventoryFlag = TRUE;
					fTeamPanelDirty = TRUE;
					fInterfacePanelDirty = DIRTYLEVEL2;
					fMapInventoryItem = FALSE;
					gpItemPointer = NULL;
				}
				else 
				{
					// return leftover items back to original slot after autoplace
					if (fShift && gpItemPointer->ubNumberOfObjects < ubInitialNumberOfObjects)
					{
						gpItemPointer->MoveThisObjectTo(*pInventorySlot);

						fShowInventoryFlag = TRUE;
						fTeamPanelDirty = TRUE;
						fInterfacePanelDirty = DIRTYLEVEL2;
						fMapInventoryItem = FALSE;
						gpItemPointer = NULL;
					}
					else // pick item up to indicate no more room for autoplace
					{
						gpItemPointerSoldier = NULL;

						// now set the cursor
						guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
						gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

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
			if ( fShowMapInventoryPool )
				HandleButtonStatesWhileMapInventoryActive();
		}
		else if ( _KeyDown( ALT ) && gGameExternalOptions.fSellAll )//Sell Item
		{
			// HEADROCK HAM 5: Added argument
			INT32 iPrice = SellItem( gItemPointer, TRUE );
			PlayJA2Sample( COMPUTER_BEEP2_IN, RATE_11025, 15, 1, MIDDLEPAN );
			gpItemPointer = NULL;
			fMapInventoryItem = FALSE;
			if ( _KeyDown( 89 ) ) //Lalien: sell all items of this type on Alt+Y
			{
				for ( UINT32 iNumber = 0; iNumber < pInventoryPoolList.size( ); ++iNumber )
				{
					// WANNE: Fix by Headrock
					//if ( pInventoryPoolList[ iNumber ].object.usItem == gItemPointer.usItem )
					if ( pInventoryPoolList[iNumber].object.usItem == gItemPointer.usItem && pInventoryPoolList[iNumber].usFlags & WORLD_ITEM_REACHABLE )
					{
						// HEADROCK HAM 5: Added argument
						iPrice += SellItem( pInventoryPoolList[iNumber].object, TRUE );
						DeleteObj( &pInventoryPoolList[iNumber].object );
					}
				}
			}
			//ADB you can sell items for 0, but that's not fair
			//and it's not easy to stop the sale so make the price 1
			// HEADROCK HAM 5: Fair Schmair. They can now see that the sale value is $0 anyway, it's up to them.
			//if (iPrice == 0) {
			//	iPrice = 1;
			//}

			if ( _KeyDown( 89 ) ) //Lalien: sell all items of this type on Alt+Y
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SOLD_ALL] );
			}
			else {
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, New113Message[MSG113_SOLD] );
			}

			// HEADROCK HAM 5: No transaction if no money.
			if ( iPrice > 0 )
			{
				AddTransactionToPlayersBook( SOLD_ITEMS, 0, GetWorldTotalMin( ), iPrice );
			}

			if ( fShowMapInventoryPool )
				HandleButtonStatesWhileMapInventoryActive( );
		}
		else
		{
			gpItemPointerSoldier = NULL;

			// now set the cursor
			guiExternVo = GetInterfaceGraphicForItem( &(Item[ gpItemPointer->usItem ]) );
			gusExternVoSubIndex = g_bUsePngItemImages ? 0 : Item[ gpItemPointer->usItem ].ubGraphicNum;

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

BOOLEAN PlaceObjectInInventoryStash( OBJECTTYPE *pInventorySlot, OBJECTTYPE *pItemPtr, INT32 iDestSlot, INT32 iSrcSlot )
{
	// if there is something there, swap it, if they are of the same type and stackable then add to the count
	if (pInventorySlot->exists() == false)
	{
		if( iSrcSlot != (-1) )
		{
			// anv: swap ownerships too
			if( pInventoryPoolList[ iSrcSlot ].soldierID == sObjectSourseSoldierID )
			{
				pInventoryPoolList[ iSrcSlot ].soldierID = (-1);
			}
			pInventoryPoolList[ iDestSlot ].soldierID = sObjectSourseSoldierID;		
		}
		// placement in an empty slot
		pItemPtr->MoveThisObjectTo(*pInventorySlot);
	}
	else
	{
		if (pItemPtr->usItem == pInventorySlot->usItem && ItemSlotLimit(pItemPtr, STACK_SIZE_LIMIT) >= 2)
		{
			// anv: disable soldier's ownership, as otherwise stacked backpacks would share it
			if( iDestSlot != (-1) && iSrcSlot != (-1) )
			{
				pInventoryPoolList[ iDestSlot ].soldierID = (-1);
				pInventoryPoolList[ iSrcSlot ].soldierID = (-1);
				iCurrentlyPickedUpItem = iDestSlot;
				sObjectSourseSoldierID = (-1);
			}
			// stacking
			pInventorySlot->AddObjectsToStack(*pItemPtr);
		}
		else
		{
			if( iDestSlot != (-1) && iSrcSlot != (-1) )
			{
				// anv: swap ownerships too
				if( pInventoryPoolList[ iSrcSlot ].soldierID == sObjectSourseSoldierID )
				{
					pInventoryPoolList[ iSrcSlot ].soldierID = (-1);
				}
				INT32 iTempSoldierID = pInventoryPoolList[ iDestSlot ].soldierID;
				pInventoryPoolList[ iDestSlot ].soldierID = sObjectSourseSoldierID;
				sObjectSourseSoldierID = iTempSoldierID;
				iCurrentlyPickedUpItem = iDestSlot;
			}
			SwapObjs( pItemPtr, pInventorySlot );
		}
	}
	return( TRUE );
}


BOOLEAN AutoPlaceObjectInInventoryStash( OBJECTTYPE *pItemPtr, INT32 sGridNo, INT8 ubLevel )
{
	OBJECTTYPE *pInventorySlot;
	INT32		cnt = 0;

	//CHRISL: I'm not sure why this never fails.  Maybe it just isn't used often.  But if we try and look for index
	//	pInventoryPoolList.size() we're going to crash since "size()" returns the total number of indecies in human terms (1-xxx)
	//	but we have to start with 0.  In other words, size() might return a value of 10 but those are numbers 0-9.

	while(1)
	{
		//if we run out of indices before running out of items to place, make new indices
		if(cnt == pInventoryPoolList.size())
		{
			ResizeInventoryList();
		}
		pInventorySlot = &pInventoryPoolList[cnt].object;

		if(pInventorySlot->usItem == pItemPtr->usItem && pInventoryPoolList[cnt].usFlags & WORLD_ITEM_REACHABLE)
		{
			pInventorySlot->AddObjectsToStack(*pItemPtr);
		}
		else if(pInventorySlot->exists() == false)
		{
			pItemPtr->MoveThisObjectTo(*pInventorySlot);
			if(sGridNo != 0)
			{
				pInventoryPoolList[cnt].sGridNo = sGridNo;
				pInventoryPoolList[cnt].usFlags |= WORLD_ITEM_REACHABLE;
				pInventoryPoolList[cnt].ubLevel = ubLevel;
				pInventoryPoolList[cnt].bVisible = 1;
				pInventoryPoolList[cnt].fExists = TRUE;
			}
			if(sGridNo == -1)
				pInventoryPoolList[cnt].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;
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
			MapInvNextPage();
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
			MapInvPreviousPage();
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

			if (gfInItemDescBox)
			{
				DoMapMessageBox( MSG_BOX_BASIC_STYLE, gzLateLocalizedString[64], MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
			}
			else
			{
				// done
				fShowMapInventoryPool = FALSE;
			}
		}
	}
}

// HEADROCK HAM 5: Sort ammo button callback
void MapInventoryPoolSortAmmoBtn( GUI_BUTTON *btn, INT32 reason )
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN || reason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		if (!(btn->uiFlags & (BUTTON_CLICKED_ON)))
		{
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			SortSectorInventoryAmmo(false);
			btn->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			SortSectorInventoryAmmo(true);
			btn->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}
}

// HEADROCK HAM 5: Stack and merge button callback
void MapInventoryPoolStackAndMergeBtn( GUI_BUTTON *btn, INT32 reason )
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if (!(btn->uiFlags & (BUTTON_CLICKED_ON)))
		{
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			SortSectorInventoryStackAndMerge(false);
			btn->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}
}

// HEADROCK HAM 5: Sort attachments button callback
void MapInventoryPoolSortAttachmentsBtn( GUI_BUTTON *btn, INT32 reason )
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN || reason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		if (!(btn->uiFlags & (BUTTON_CLICKED_ON)))
		{
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			SortSectorInventoryEmptyLBE();
			btn->uiFlags &= ~(BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			SortSectorInventorySeparateAttachments();
			btn->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

}

// HEADROCK HAM 5: Eject Ammo button callback
void MapInventoryPoolEjectAmmoBtn( GUI_BUTTON *btn, INT32 reason )
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if (!(btn->uiFlags & (BUTTON_CLICKED_ON)))
		{
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			SortSectorInventoryEjectAmmo();
			btn->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}
}

// HEADROCK HAM 5: Preliminary Filter Button
void MapInventoryPoolFilterBtn( GUI_BUTTON *btn, INT32 reason )
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN ||
		reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if (!(btn->uiFlags & (BUTTON_CLICKED_ON)))
		{
			// Set as "clicked on", but do nothing until the mouse is released.
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			if (btn->UserData[0] == 0)
			{
				MapInventoryFilterSet( btn->UserData[1] );
			}
			else
			{
				MapInventoryFilterToggle( btn->UserData[1] );
			}
			//shadooow: this will re-render item compatibility highlighting when changing filters for currently held item
			if (gpItemPointer != NULL)
			{
				fChangedInventorySlots = TRUE;
				gfCheckForCursorOverMapSectorInventoryItem = TRUE;
			}
			// HEADROCK HAM 5: Disabled for now, as we've got buttons for this.
			//CreateMapInventoryFilterMenu( );
		}
	}
	if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			if (btn->UserData[2] == 0)
			{
				MapInventoryFilterSet( btn->UserData[3] );
			}
			else
			{
				MapInventoryFilterToggle( btn->UserData[3] );
			}
			//shadooow: this will re-render item compatibility highlighting when changing filters for currently held item
			if (gpItemPointer != NULL)
			{
				fChangedInventorySlots = TRUE;
				gfCheckForCursorOverMapSectorInventoryItem = TRUE;
			}
			// HEADROCK HAM 5: Disabled for now, as we've got buttons for this.
			//CreateMapInventoryFilterMenu( );
		}
	}
}

// Flugente: display whether items that shoul be ignored for 'move item' assignment should be marked
void MapInventoryPoolFilterBtnMoveItemDisplay( GUI_BUTTON *btn, INT32 reason )
{
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_DWN ||
		reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if (!(btn->uiFlags & (BUTTON_CLICKED_ON)))
		{
			// Set as "clicked on", but do nothing until the mouse is released.
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if (btn->uiFlags & (BUTTON_CLICKED_ON))
		{
			ToggleShowMoveItem();

			// The refresh function moves the necessary items from the Seen to the Unseen inventories, and vice versa.
			RefreshSeenAndUnseenPools();

			BlitInventoryPoolGraphic();

			HandleSetFilterButtons();
		}
	}
}

// Flugente: gear templates
extern void WriteEquipmentTemplate(SOLDIERTYPE* pSoldier, STR16 name);
extern void EquipmentListMenu( );

void TemplateNameInputCallBack(UINT8 ubResult)
{
	if (ubResult == MSG_BOX_RETURN_OK && wcscmp(gszMsgBoxInputString, L"") > 0)
	{
		SOLDIERTYPE* pSoldier = &Menptr[gCharactersList[bSelectedInfoChar].usSolID];
		if (pSoldier)
		{
			WriteEquipmentTemplate(pSoldier, gszMsgBoxInputString);

			// The refresh function moves the necessary items from the Seen to the Unseen inventories, and vice versa.
			RefreshSeenAndUnseenPools();

			BlitInventoryPoolGraphic();

			HandleSetFilterButtons();
		}
	}
	memset(gszMsgBoxInputString, 0, sizeof(gszMsgBoxInputString));
}

void MapInventoryWriteEquipmentTemplate(GUI_BUTTON *btn, INT32 reason)
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN ||
		 reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if ( !(btn->uiFlags & (BUTTON_CLICKED_ON)) )
		{
			// Set as "clicked on", but do nothing until the mouse is released.
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( btn->uiFlags & (BUTTON_CLICKED_ON) )
		{
			SOLDIERTYPE* pSoldier = &Menptr[gCharactersList[bSelectedInfoChar].usSolID];
			if ( pSoldier )
			{
				DoMessageBox( MSG_BOX_BASIC_SMALL_BUTTONS, szGearTemplateText[0], guiCurrentScreen, MSG_BOX_FLAG_INPUTBOX, TemplateNameInputCallBack, NULL );
			}
		}
	}
}

void MapInventoryReadEquipmentTemplate(GUI_BUTTON *btn, INT32 reason)
{
	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN ||
		 reason & MSYS_CALLBACK_REASON_RBUTTON_DWN )
	{
		if ( !(btn->uiFlags & (BUTTON_CLICKED_ON)) )
		{
			// Set as "clicked on", but do nothing until the mouse is released.
			btn->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}

	if ( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( btn->uiFlags & (BUTTON_CLICKED_ON) )
		{
			SOLDIERTYPE* pSoldier = &Menptr[gCharactersList[bSelectedInfoChar].usSolID];
			if ( pSoldier &&
				 pSoldier->sSectorX == sSelMapX && pSoldier->sSectorY == sSelMapY && pSoldier->bSectorZ == iCurrentMapSectorZ &&
				 !pSoldier->flags.fBetweenSectors )
			{
				if ( (gTacticalStatus.uiFlags & INCOMBAT || gTacticalStatus.fEnemyInSector) )
				{
					DoScreenIndependantMessageBox( szGearTemplateText[1], MSG_BOX_FLAG_OK, NULL );
				}
				else
				{
					// open the menu
					EquipmentListMenu( );
				}
			}
			else
			{
				DoScreenIndependantMessageBox( szGearTemplateText[2], MSG_BOX_FLAG_OK, NULL );
			}
		}
	}
}

// HEADROCK HAM 5: Zoom button.
// This button handles toggling between the small-scale inventory (items shown at normal size, similar to merc's inventory)
// and the big-item inventory (which shows BigItem pics and is therefore much easier to examine in detail).
// A click on the button initiates "zoom input" mode, allowing the player to decide which part of the small-scale
// inventory he would like to examine more closely. While in this mode, you may flip inventory pages or close the inventory
// altogether, but may not move the mouse outside the map inventory region. Right-click will cancel this mode 
// automatically.
void MapInventoryPoolZoomBtn( GUI_BUTTON *btn, INT32 reason )
{
	// Only left-click interacts with this button.
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// Is the button already pressed?
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			// Are we still waiting for zoom input? If so, the button is depressed, but we are still not
			// in zoomed mode.
			if (fWaitingForZoomInput)
			{
				// Turn off Zoom Input mode.
				CancelInventoryZoomInput(TRUE);
			}
			// If zoomed, unzoom.
			if (fMapInventoryZoom)
			{
				// What is the number of the first item on the current inventory page?
				UINT16 usFirstItem = iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT;
				UINT16 usNumSlotsOnUnzoomedPage = GetInventorySlotCount( FALSE );
				UINT16 usNewPage = (UINT16)(usFirstItem / usNumSlotsOnUnzoomedPage);
				HandleMapInventoryUnzoom( usNewPage );
			}
			
			// Turn the button off so it can be clicked again.
			btn->uiFlags &=~(BUTTON_CLICKED_ON);
		}
		else
		{
			// This will initiate Zoom Input mode.
			if (!fWaitingForZoomInput)
			{
				// Turn the button on do indicate that it has been pressed.
				btn->uiFlags |= (BUTTON_CLICKED_ON);

				// Calculate the number of inventory rows and columns we have on-screen currently.
				INT16 sRows = ( MAP_INVENTORY_POOL_SLOT_COUNT / MAP_INV_SLOT_COLS );
				INT16 sCols = ( MAP_INV_SLOT_COLS );

				// Calculate the size of the inventory portion. Unfortunately there are still no constants
				// for this.
				MapInventoryRect.iLeft = MAP_INVENTORY_POOL_SLOT_START_X+4;
				MapInventoryRect.iRight = MAP_INVENTORY_POOL_SLOT_START_X + (sRows * MAP_INVEN_SPACE_BTWN_SLOTS);
				MapInventoryRect.iTop = 5;
				MapInventoryRect.iBottom = MAP_INVENTORY_POOL_SLOT_START_Y + (sCols * MAP_INVEN_SLOT_HEIGHT) + 54;
				// Restrict mouse movement to this area only. This will avoid interacting with unexpected portions of
				// the screen, allowing us to better control confirming or exiting Zoom Input mode.
				RestrictMouseCursor( &MapInventoryRect );

				// Notify to the rest of the program that we are inside zoom input mode. In particular, inventory
				// slots will behave differently when clicked or right-clicked while this flag is set.
				fWaitingForZoomInput = 1;
			}
			else
			{
				// Zoom Input mode was active, which isn't supposed to happen because the button was off. Still, make
				// sure the zoom input is canceled so we can go on without errors.
				CancelInventoryZoomInput( TRUE );
			}
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
	FindFontCenterCoordinates(MAPINV_PAGE_X, MAPINV_PAGE_Y, MAP_INVENTORY_POOL_PAGE_WIDTH ,MAP_INVENTORY_POOL_PAGE_HEIGHT ,sString , MAP_SCREEN_FONT, &sX, &sY);

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
	FindFontCenterCoordinates(MAPINV_ITEMPOOL_X, MAPINV_ITEMPOOL_Y, MAP_INVENTORY_POOL_NUMBER_WIDTH ,MAP_INVENTORY_POOL_PAGE_HEIGHT ,sString , MAP_SCREEN_FONT, &sX, &sY);

	mprintf( sX, sY, sString );

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	return;
}


void CreateMapInventoryPoolDoneButton( void )
{
	// create done button
	guiMapInvenDoneButtonImage=  LoadButtonImage( "INTERFACE\\done_button.sti" , -1, 0, -1, 1, -1 );
	guiMapInvenDoneButton = QuickCreateButton( guiMapInvenDoneButtonImage, MAPINV_DONE_X , MAPINV_DONE_y,
								BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST,
								(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)MapInventoryPoolDoneBtn );

	// set up fast help text
	SetButtonFastHelpText( guiMapInvenDoneButton, pMapScreenInvenButtonHelpText[ 2 ] );

	return;
}

void DestroyInventoryPoolDoneButton( void )
{
	// destroy ddone button

	RemoveButton( guiMapInvenDoneButton );
	UnloadButtonImage( guiMapInvenDoneButtonImage );


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
	FindFontCenterCoordinates(MAPINV_SECTOR_X, MAPINV_SECTOR_Y, MAP_INVENTORY_POOL_LOC_WIDTH ,MAP_INVENTORY_POOL_PAGE_HEIGHT ,sString , MAP_SCREEN_FONT, &sX, &sY);

	mprintf( sX, sY, sString );

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
}

// HEADROCK HAM 5: Following some issues with the resizing of the map inventory - especially due to switching
// between zoomed and unzoomed move, but possibly some pre-existing issues as well. Also, this function is now
// significantly more readable.
void ResizeInventoryList( void )
{
	// If we've got no items in this sector...
	if (pInventoryPoolList.empty() == true ) 
	{
		// Resize the list to at least one page in size, containing no items.
		giDesiredNumMapInventorySlots = __max(giDesiredNumMapInventorySlots, MAP_INVENTORY_POOL_SLOT_COUNT);
		pInventoryPoolList.resize(giDesiredNumMapInventorySlots);
		iLastInventoryPoolPage = (giDesiredNumMapInventorySlots / MAP_INVENTORY_POOL_SLOT_COUNT) - 1;

		// Done!
		return;
	}

	// NEVER create an inventory that's SMALLER than the one we're looking at.
	giDesiredNumMapInventorySlots = __max(giDesiredNumMapInventorySlots, ((iCurrentInventoryPoolPage+1) * MAP_INVENTORY_POOL_SLOT_COUNT));

	// Lets see how many item slots we're going to need
	int activeSlotsOnPage = 0;
	int activeSlotsTotal = 0;
	INT32 x = 1;
	INT32 ListSize = pInventoryPoolList.size(); // Current size, so we can run through it all.
	UINT32 iNumEmptySlots = 0;
	UINT32 iNumEmptySlotsAtEnd = 0;
	// Run through the entire sector inventory, collecting data.
	for (x = 0; x < ListSize; x++) 
	{
		//don't test fExists because that hasn't been set yet, test object.exists
		if (pInventoryPoolList[x].object.exists() == true) 
		{
			// Item exists. Increase the count of valid items.
			activeSlotsTotal++;
			// Since the last item we (just now) checked exists, lets reset the count of how many 
			// slots are empty at the end of the inventory array.
			iNumEmptySlotsAtEnd = 0;
			
			// Now count the number of items found since the end of the "previous" page.
			// We constantly assume that we're looking at the end of the last page. If we find more
			// items beyond this, that means there's yet another page to check, and so on until
			// we actually reach the end of the array.
			if (x % MAP_INVENTORY_POOL_SLOT_COUNT == 0 )
			{
				// Reset the counter, this is the first item on a new page.
				activeSlotsOnPage = 1;
			}
			else
			{
				// Increase the counter, building up towards the end of the page.
				activeSlotsOnPage++;
			}

		}
		else
		{
			// Found an empty slot. 
			// Increase the number of empty slots we've encountered...
			iNumEmptySlots++;
			// Increase the number of slots encountered since the last valid item.
			iNumEmptySlotsAtEnd++;
		}
	}

	// Truncate. This makes sure we don't have a page full of empty slots. If we need one, we'll add it later.
	// 1. Find out how many redundant empty slots we have at the end.
	INT32 iNumSlotsToRemove = iNumEmptySlotsAtEnd - (iNumEmptySlotsAtEnd % MAP_INVENTORY_POOL_SLOT_COUNT);
	// 2. Reduce the number of required slots at the end to make the last page.
	iNumEmptySlotsAtEnd -= iNumSlotsToRemove;
	// 3. Reduce the total number of empty slots encountered by the same amount.
	iNumEmptySlots -= iNumSlotsToRemove;
	// 4. The difference between these two values is the number of empty slots strewn between valid items.
	// We need to count them separately because for all intents and purposes, we count them as valid items.
	INT32 iNumEmptySlotsWithin = iNumEmptySlots - iNumEmptySlotsAtEnd;

	// Calculate the minimum size of the inventory, including both valid items and empty slots between them,
	// but not the extra slots required to make a round number of pages.
	ListSize = activeSlotsTotal + iNumEmptySlotsWithin;
	
	// Now find out the minimal number of slots required to make a round number of pages.
	INT32 iOptimalSizeWithExtraEmptySlots = ((ListSize / MAP_INVENTORY_POOL_SLOT_COUNT) + 1) * MAP_INVENTORY_POOL_SLOT_COUNT;

	// This simply ensures we don't make more extra empty slots at the end than is absolutely necessary.
	iNumEmptySlotsAtEnd = __min(MAP_INVENTORY_POOL_SLOT_COUNT - 1, iOptimalSizeWithExtraEmptySlots - ListSize);

	////////////////////////////////////////////////////////////////////////////
	// Now we resize the inventory to its full intended size.
	// 
	// Note that below we use __max to make sure that we're not decreasing the number of pages while the inventory
	// screen is open. In other words, the number of pages will not shrink unless we explicitly say it can. This
	// can lead to having 0 items on 14 pages, for example if we delete all items from a huge sector inventory. However,
	// this is fine because the extra pages will disappear when we reopen this inventory - and until then we avoid
	// any issues that may arise from checking invalid inventory slots. A more experienced programmer might be able
	// to reduce the size of the inventory while we work, but that would require more wizardry - and is not actually
	// necessary to begin with. It's actually better and safer this way!
	BOOLEAN fExtraPage = FALSE;
#ifdef INVFIX_Moa//dnl ch75 311013
	if (iNumEmptySlotsAtEnd >= 3 )
	{
		// Lots of empty slots at the end, so we don't need to increase the number of pages.
		pInventoryPoolList.resize( __max(iOptimalSizeWithExtraEmptySlots, giDesiredNumMapInventorySlots) );
	}
	else 
	{
		// We want 1 extra blank page at the end, so we add the number of slots required to make that page.
		pInventoryPoolList.resize( __max(iOptimalSizeWithExtraEmptySlots + MAP_INVENTORY_POOL_SLOT_COUNT, giDesiredNumMapInventorySlots ) );
		fExtraPage = TRUE;
	}
#else
	if(iNumEmptySlotsAtEnd >= 3)
	{
		// Lots of empty slots at the end, so we don't need to increase the number of pages.
		giDesiredNumMapInventorySlots = max(iOptimalSizeWithExtraEmptySlots, giDesiredNumMapInventorySlots);
	}
	else 
	{
		// We want 1 extra blank page at the end, so we add the number of slots required to make that page.
		giDesiredNumMapInventorySlots = max(iOptimalSizeWithExtraEmptySlots + MAP_INVENTORY_POOL_SLOT_COUNT, giDesiredNumMapInventorySlots);
		fExtraPage = TRUE;
	}
	// Only resize if need to extend slot number, also never clear pInventoryPoolList and pUnSeenItems to avoid performance decrease, and both of them must be same size
	if(giDesiredNumMapInventorySlots > (INT32)pInventoryPoolList.size())
		pInventoryPoolList.resize(giDesiredNumMapInventorySlots);
	if(giDesiredNumMapInventorySlots > (INT32)pUnSeenItems.size())
		pUnSeenItems.resize(giDesiredNumMapInventorySlots);
#endif

	// Set this value so that the inventory does not shrink later. If we zoom or close the inventory, this will be
	// automatically reset to force a complete recalculation - which is ok because we're recalculating everything in
	// such a case anyway.
	giDesiredNumMapInventorySlots = pInventoryPoolList.size();

	// Calculate the number of pages we've got.
	//iLastInventoryPoolPage = ( ( pInventoryPoolList.size()  - 1 ) / MAP_INVENTORY_POOL_SLOT_COUNT );
	iLastInventoryPoolPage = ( ( iOptimalSizeWithExtraEmptySlots - 1 ) / MAP_INVENTORY_POOL_SLOT_COUNT ) + fExtraPage;
	iCurrentInventoryPoolPage = __min(iCurrentInventoryPoolPage, iLastInventoryPoolPage);

	return;
}

void DrawTextOnMapInventoryBackground( void )
{
	UINT16 usStringHeight;

	SetFont( MAP_IVEN_FONT );
	SetFontBackground( FONT_BLACK );
	SetFontForeground( FONT_BEIGE );

	// set the buffer
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	//Calculate the height of the string, as it needs to be vertically centered.
	usStringHeight = DisplayWrappedString(MAPINV_LOC_STR_X, MAPINV_LOC_STR_Y, 53, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 0 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED | DONT_DISPLAY_TEXT );
	DisplayWrappedString(MAPINV_LOC_STR_X, MAPINV_LOC_STR_Y - (usStringHeight / 2) , 53, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 0 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	//Calculate the height of the string, as it needs to be vertically centered.
	usStringHeight = DisplayWrappedString(MAPINV_ITEMPOOL_STR_X, MAPINV_ITEMPOOL_STR_Y, 65, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 1 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED | DONT_DISPLAY_TEXT );
	DisplayWrappedString(MAPINV_ITEMPOOL_STR_X, MAPINV_ITEMPOOL_STR_Y - (usStringHeight / 2), 65, 1, MAP_IVEN_FONT, FONT_BEIGE, pMapInventoryStrings[ 1 ], FONT_BLACK, FALSE, RIGHT_JUSTIFIED );

	// item filter buttons included in 640x480
	/*if (iResolution < _800x600)
	{
		DrawTextOnSectorInventory( );
	}*/

	SetFontDestBuffer( FRAME_BUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	return;
}


void HandleButtonStatesWhileMapInventoryActive( void )
{

	// are we even showing the map inventory pool graphic?
	if( fShowMapInventoryPool == FALSE )
	{
		return;
	}

	// last page, go no further
	if( iCurrentInventoryPoolPage == iLastInventoryPoolPage )
	{
		DisableButton( guiMapInvenArrowButton[ 0 ] );
	}
	else
	{
		EnableButton( guiMapInvenArrowButton[ 0 ] );
	}

	// first page, can't go back any
	if( iCurrentInventoryPoolPage == 0 )
	{
		DisableButton( guiMapInvenArrowButton[ 1 ] );
	}
	else
	{
		EnableButton( guiMapInvenArrowButton[ 1 ] );
	}

	// item picked up ..disable button
	if( gMPanelRegion.Cursor == EXTERN_CURSOR )
	{
		DisableButton( guiMapInvenDoneButton );
		DisableButton( guiMapInvenSortButton[ 0 ] );
		DisableButton( guiMapInvenSortButton[ 1 ] );
		DisableButton( guiMapInvenSortButton[ 2 ] );
		DisableButton( guiMapInvenSortButton[ 3 ] );
	}
	else
	{
		EnableButton( guiMapInvenDoneButton );
		EnableButton( guiMapInvenSortButton[ 0 ] );
		EnableButton( guiMapInvenSortButton[ 1 ] );
		EnableButton( guiMapInvenSortButton[ 2 ] );
		EnableButton( guiMapInvenSortButton[ 3 ] );
	}

	// Selected Merc is in sector? Or is in combat?
	if(MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->sSectorX != sSelMapX ||
		MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->sSectorY != sSelMapY || 
		MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->bSectorZ != iCurrentMapSectorZ ||
		MercPtrs[gCharactersList[bSelectedInfoChar].usSolID]->flags.fBetweenSectors ||
		!CanPlayerUseSectorInventory( &(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ]) ) ) 
	{
		DisableButton( guiMapInvenSortButton[ 0 ] );
		DisableButton( guiMapInvenSortButton[ 1 ] );
		DisableButton( guiMapInvenSortButton[ 2 ] );
		DisableButton( guiMapInvenSortButton[ 3 ] );
	}
}


void DrawTextOnSectorInventory( void )
{
	INT16 sX = 0, sY = 0;
	CHAR16 sString[ 64 ];

	// parse the string
	swprintf( sString, zMarksMapScreenText[ 11 ] );

	if(gGameExternalOptions.fEnableInventoryPoolQ && gInventoryPoolIndex != '0')//dnl ch51 081009
		swprintf(sString, L"Inventory Pool %c", gInventoryPoolIndex);

	
	SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	FindFontCenterCoordinates( (SCREEN_WIDTH - INTERFACE_WIDTH)/2 + 271, 18, xResSize - 271, GetFontHeight( FONT14ARIAL ), sString, FONT14ARIAL, &sX, &sY );
	
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


//Moa 09/26/2013: now only processed if mouse over different slot then before (using fChangedInventorySlots)
// change preprocessor directive to 1 and uncomment any giCompatibleItemBaseTime in TimerControl.cpp and in this file if original Sirtech code should be used.
void HandleMouseInCompatableItemForMapSectorInventory( INT32 iCurrentSlot )
{
	SOLDIERTYPE *pSoldier = NULL;
	static BOOLEAN fItemWasHighLighted = FALSE;

	//if same slot then before dont recalculate
	if ( !fChangedInventorySlots ) return;

	//reset highlightings for soldier inventory and stash if it was highlighted before
	if ( fItemWasHighLighted )
	{
		ResetCompatibleItemArray( );
		ResetMapSectorInventoryPoolHighLights( );
	}

	//Nothing selected or out of bounds or empty slot selected; highlightings reseted already, return
	if ( gpItemPointer == NULL && (iCurrentSlot == NO_SLOT || 
		(pInventoryPoolList.size() < (UINT32)(iCurrentSlot + ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) )) ||
		pInventoryPoolList[ iCurrentSlot + iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ].fExists == FALSE ))
	{
		fChangedInventorySlots = FALSE;
		if (fItemWasHighLighted)
		{
			fTeamPanelDirty = TRUE;
			fMapPanelDirty = TRUE;
			fItemWasHighLighted = FALSE;
		}
		return;
	}

	// given this slot value, check if anything in the displayed sector inventory or on the mercs inventory is compatable
	if( fShowInventoryFlag )
	{
		//Soldier inventory is shown, highlight those items
		pSoldier = &Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ];
		if( pSoldier )
		{
			if( HandleCompatibleAmmoUIForMapScreen( pSoldier, iCurrentSlot + ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ), TRUE, FALSE ) )
			{
				fItemWasHighLighted = TRUE;//remember that something is highlighted
			}
			fTeamPanelDirty = TRUE;
		}
	}


	// now handle for the sector inventory
	if( fShowMapInventoryPool )
	{
		if( HandleCompatibleAmmoUIForMapInventory( pSoldier, iCurrentSlot, ( iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT ) , TRUE, FALSE ) )
		{
			fItemWasHighLighted = TRUE;//remember that something is highlighted
		}
		fMapPanelDirty = TRUE;
	}

	fChangedInventorySlots = FALSE;
}

void ResetMapSectorInventoryPoolHighLights( void )
{
	INT32 iCounter = 0;

	// now reset the highlight list for the map sector inventory
	for ( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT; ++iCounter )
	{
		if ( fMapInventoryItemCompatable[ iCounter ] )
		{
			fMapInventoryItemCompatable[ iCounter ] = FALSE;
		}
	}
}
void HandleMapSectorInventory( void )
{
	// handle mouse in compatable item map sectors inventory
	// HEADROCK HAM 5: This shouldn't fire when doing Inventory Zoom
	if (!fWaitingForZoomInput)
	{
		HandleMouseInCompatableItemForMapSectorInventory( iCurrentlyHighLightedItem );
	}
	HandleMousePageScroll();
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
		// HEADROCK HAM 5: Map Inventory Filters are applied here. The item in question
		// has to match one of the Item Classes defined in this global Flags variable.
		if (Item[pWorldItem->object.usItem].usItemClass & guiMapInventoryFilter)
		{
			return ( TRUE );
		}
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

	for( iCnt=0; iCnt<iTotalNumberItems; ++iCnt)
	{
		if( ( TileIsOutOfBounds(pInventoryPoolList[ iCnt ].sGridNo) )&& !( pInventoryPoolList[ iCnt ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )		
		{
			//set the flag
			pInventoryPoolList[ iCnt ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;

			//count the number
			++uiNumFlagsNotSet;
		}
	}


	//loop through all the UNSEEN items
	for( iCnt=0; iCnt<(INT32)uiNumberOfUnSeenItems; ++iCnt)
	{		
		if (TileIsOutOfBounds(pUnSeenItems[ iCnt ].sGridNo) && !( pUnSeenItems[ iCnt ].usFlags & WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT ) )
		{
			//set the flag
			pUnSeenItems[ iCnt ].usFlags |= WORLD_ITEM_GRIDNO_NOT_SET_USE_ENTRY_POINT;

			//count the number
			++uiNumFlagsNotSet;
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
#if _ITERATOR_DEBUG_LEVEL > 1//dnl ch75 061113 under debug VS2010 throws exceptions after qsort but not under VS2005 and VS2008, all release version seems to work fine
	std::sort(pInventory.begin(), pInventory.begin() + uiSizeOfArray);
#else
	qsort((LPVOID)&pInventory.front(), (size_t)uiSizeOfArray, sizeof(WORLDITEM), MapScreenSectorInventoryCompare);
#endif
}

INT32 MapScreenSectorInventoryCompare( const void *pNum1, const void *pNum2)
{
	WORLDITEM *pFirst = (WORLDITEM *)pNum1;
	WORLDITEM *pSecond = (WORLDITEM *)pNum2;
	UINT16	usItem1Index;
	UINT16	usItem2Index;
	UINT16		ubItem1Quality;
	UINT16		ubItem2Quality;

	//dnl ch75 071113 without below fix sort will create mess when use empty slots because fExists remain TRUE after item is removed from inventory so decide to rather check ubNumberOfObjects
	if(!pFirst->object.ubNumberOfObjects)
		return(1);
	if(!pSecond->object.ubNumberOfObjects)
		return(-1);

	usItem1Index = pFirst->object.usItem;
	usItem2Index = pSecond->object.usItem;

	ubItem1Quality = pFirst->object[0]->data.objectStatus;
	ubItem2Quality = pSecond->object[0]->data.objectStatus;

	return( CompareItemsForSorting( usItem1Index, usItem2Index, ubItem1Quality, ubItem2Quality ) );
}

BOOLEAN CanPlayerUseSectorInventory( SOLDIERTYPE *pSelectedSoldier )
{
#ifdef _DEBUG//dnl ch75 021113
	if(gGameExternalOptions.fEnableInventoryPoolQ)
		return(TRUE);
#endif
	INT16	sX, sY, sZ;

	//Get the sector that has a battle
	BOOLEAN fInCombat = GetCurrentBattleSectorXYZAndReturnTRUEIfThereIsABattle( &sX, &sY, &sZ );

	//if there is a battle going on
	if( fInCombat )
	{
		//if the selected soldier is in combat sector
		if( ( ( pSelectedSoldier->sSectorX == sX ) &&
					( pSelectedSoldier->sSectorY == sY ) &&
					( pSelectedSoldier->bSectorZ == sZ )
				)
			)
		{
			return( FALSE );
		}
	}

	return( TRUE );
}

//dnl ch51 081009 start
BOOLEAN	LoadInventoryPoolQ (UINT8 ubSaveGameID)
{
	BOOLEAN ret;
	INT32 i, j, iTotNumSlots;
	CHAR8 tmpbuf[1024];
	UINT32 uiNumBytesRead;
	HWFILE hFile = 0;

	//if(MAP_INVENTORY_POOL_SLOT_COUNT <= 0)
	//{
	//	if (iResolution >= _640x480 && iResolution < _800x600)
	//		MAP_INVENTORY_POOL_SLOT_COUNT = 40;
	//	else if (iResolution == _1280x720)
	//	{
	//		MAP_INVENTORY_POOL_SLOT_COUNT = MAP_INVENTORY_POOL_MAX_SLOTS;
	//	}
	//	else if (iResolution < _1024x768)
	//		MAP_INVENTORY_POOL_SLOT_COUNT = 77;
	//	else
	//		MAP_INVENTORY_POOL_SLOT_COUNT = 170;
	//}
	ResetMapInventoryOffsets();

	ret = FALSE;
	CreateSavedGameFileNameFromNumber(ubSaveGameID, tmpbuf);
	strcat(tmpbuf, ".IPQ");
	
	//MM:  This check is sometimes required while debugging a save game from release mode
	if (FileExists(tmpbuf))
		hFile = FileOpen(tmpbuf, FILE_ACCESS_READ|FILE_OPEN_EXISTING, FALSE);

	if(hFile == 0)
		return(TRUE);
	
	MemFreeInventoryPoolQ();
	FileRead(hFile, iCurrentInventoryPoolPageQ+1, (INVPOOLLISTNUM-1)*sizeof(INT32), &uiNumBytesRead);
	if(uiNumBytesRead != (INVPOOLLISTNUM-1)*sizeof(INT32))
		goto ERR;
	FileRead(hFile, iLastInventoryPoolPageQ+1, (INVPOOLLISTNUM-1)*sizeof(INT32), &uiNumBytesRead);
	if(uiNumBytesRead != (INVPOOLLISTNUM-1)*sizeof(INT32))
		goto ERR;
	for(i=1; i<INVPOOLLISTNUM; i++)
	{
		FileRead(hFile, &j, sizeof(INT32), &uiNumBytesRead);
		if(uiNumBytesRead != sizeof(INT32))
			goto ERR;
		if(j == 0)
			continue;
		iTotNumSlots = j;
		if(MAP_INVENTORY_POOL_SLOT_COUNT != (j/(iLastInventoryPoolPageQ[i]+1)))
		{
			iLastInventoryPoolPageQ[i] = j / MAP_INVENTORY_POOL_SLOT_COUNT + 1;
			j = iLastInventoryPoolPageQ[i] * MAP_INVENTORY_POOL_SLOT_COUNT;
			iCurrentInventoryPoolPageQ[i] = 0;
		}
		pInventoryPoolListQ[i].resize(j);
		for(j=0; j<iTotNumSlots; j++)
			if(!pInventoryPoolListQ[i][j].Load(hFile))
				goto ERR;
	}
	ret = TRUE;
ERR:
	if(ret == FALSE)
		MemFreeInventoryPoolQ();
	FileClose(hFile);
	return(ret);
}

BOOLEAN MemFreeInventoryPoolQ(void)
{
	INT32 i;

	for(i=1; i<INVPOOLLISTNUM; i++)
	{
		pInventoryPoolListQ[i].clear();
		iCurrentInventoryPoolPageQ[i] = 0;
		iLastInventoryPoolPageQ[i] = 0;
	}
	return(TRUE);
}

BOOLEAN SaveInventoryPoolQ(UINT8 ubSaveGameID)
{
	BOOLEAN ret;
	INT32 i, j;
	CHAR8 tmpbuf[1024];
	UINT32 uiNumBytesWritten;
	HWFILE hFile;

	//if(MAP_INVENTORY_POOL_SLOT_COUNT <= 0)
	//{
	//	if (iResolution >= _640x480 && iResolution < _800x600)
	//		MAP_INVENTORY_POOL_SLOT_COUNT = 40;
	//	else if (iResolution == _1280x720)
	//	{
	//		MAP_INVENTORY_POOL_SLOT_COUNT = MAP_INVENTORY_POOL_MAX_SLOTS;
	//	}
	//	else if (iResolution < _1024x768)
	//		MAP_INVENTORY_POOL_SLOT_COUNT = 77;
	//	else
	//		MAP_INVENTORY_POOL_SLOT_COUNT = 170;
	//}
	ResetMapInventoryOffsets();

	ret = FALSE;
	CreateSavedGameFileNameFromNumber(ubSaveGameID, tmpbuf);
	strcat(tmpbuf, ".IPQ");
	if(FileExists(tmpbuf))//dnl ch75 021113
		FileDelete(tmpbuf);
	hFile = FileOpen(tmpbuf, FILE_ACCESS_WRITE|FILE_OPEN_ALWAYS, FALSE);
	if(hFile == 0)
		goto ERR;
	FileWrite(hFile, iCurrentInventoryPoolPageQ+1, (INVPOOLLISTNUM-1)*sizeof(INT32), &uiNumBytesWritten);
	if(uiNumBytesWritten != (INVPOOLLISTNUM-1)*sizeof(INT32))
		goto ERR;
	FileWrite(hFile, iLastInventoryPoolPageQ+1, (INVPOOLLISTNUM-1)*sizeof(INT32), &uiNumBytesWritten);
	if(uiNumBytesWritten != (INVPOOLLISTNUM-1)*sizeof(INT32))
		goto ERR;
	for(i=1; i<INVPOOLLISTNUM; i++)
	{
		j = pInventoryPoolListQ[i].size();
		FileWrite(hFile, &j, sizeof(INT32), &uiNumBytesWritten);
		if(uiNumBytesWritten != sizeof(INT32))
			goto ERR;
		for(j=0; j<(INT32)pInventoryPoolListQ[i].size(); j++)
			if(!pInventoryPoolListQ[i][j].Save(hFile, TRUE))
				goto ERR;
	}
	ret = TRUE;
ERR:
	FileClose(hFile);
	return(ret);
}

BOOLEAN SortInventoryPoolQ(void)
{
	if(pInventoryPoolList.size() > 0)
	{
		SortSectorInventory(pInventoryPoolList, pInventoryPoolList.size());
	}
	fMapPanelDirty = TRUE;
	return(TRUE);
}

BOOLEAN SwitchToInventoryPoolQ(UINT8 newidx)//dnl ch75 021113
{
	UINT8 curidx = gInventoryPoolIndex & 0x0F;
	if(curidx >= INVPOOLLISTNUM)
		return(FALSE);

	newidx &= 0x0F;

	if(curidx == newidx)
		return(FALSE);

	MapInventoryFilterSet(IC_MAPFILTER_ALL);

	UINT32 uiNumOfSlots = (iLastInventoryPoolPage + 1) * MAP_INVENTORY_POOL_SLOT_COUNT;
	
	if(pInventoryPoolListQ[curidx].size() != uiNumOfSlots)
		pInventoryPoolListQ[curidx].resize(uiNumOfSlots);

	WORLDITEM *pwiQ1 = &pInventoryPoolListQ[curidx].front();
	WORLDITEM *pwiQ2 = &pInventoryPoolListQ[curidx].back();
	WORLDITEM *pwi1 = &pInventoryPoolList.front();
	WORLDITEM *pwi2 = &pInventoryPoolList.back();
	int cnt = 0;

	while(pwi1 <= pwi2)
	{
		if(pwi1->fExists)
		{
			*pwiQ1 = *pwi1;
			pwiQ1++;
		}
		pwi1++;
		cnt++;
	}

	while(pwiQ1 <= pwiQ2)
	{
		pwiQ1->fExists = FALSE;
		pwiQ1->object.ubNumberOfObjects = 0;
		pwiQ1++;
	}

	iCurrentInventoryPoolPageQ[curidx] = iCurrentInventoryPoolPage;
	iLastInventoryPoolPageQ[curidx] = iLastInventoryPoolPage;
	
	if(pInventoryPoolListQ[newidx].empty() == true)
		pInventoryPoolListQ[newidx].resize(MAP_INVENTORY_POOL_SLOT_COUNT);

	if(pInventoryPoolList.size() < pInventoryPoolListQ[newidx].size())
		pInventoryPoolList.resize(pInventoryPoolListQ[newidx].size());

	pwiQ1 = &pInventoryPoolListQ[newidx].front();
	pwiQ2 = &pInventoryPoolListQ[newidx].back();
	pwi1 = &pInventoryPoolList.front();
	pwi2 = &pInventoryPoolList.back();
	cnt = 0;
	while(pwiQ1 <= pwiQ2)
	{
		if(pwiQ1->fExists)
		{
			*pwi1 = *pwiQ1;
			pwi1++;
		}
		pwiQ1++;
		cnt++;
	}
	while(pwi1 <= pwi2)
	{
		pwi1->fExists = FALSE;
		pwi1->object.ubNumberOfObjects = 0;
		pwi1++;
	}

	iCurrentInventoryPoolPage = iCurrentInventoryPoolPageQ[newidx];
	iLastInventoryPoolPage = iLastInventoryPoolPageQ[newidx];
	gInventoryPoolIndex = newidx | 0x30;
	fMapPanelDirty = TRUE;

	return(TRUE);
}

BOOLEAN CopySectorInventoryToInventoryPoolQ(UINT8 idx)
{
	INT32 iSlotCounter, iSlotCounterQ;

	idx = gInventoryPoolIndex & 0x0F;
	if(idx <= 0 || idx >= INVPOOLLISTNUM || !(gWorldSectorX == sSelMapX && gWorldSectorY == sSelMapY && gbWorldSectorZ == iCurrentMapSectorZ))//dnl ch75 021113
		return(FALSE);
	SwitchToInventoryPoolQ('0');
	iSlotCounterQ = 0;
	for(iSlotCounter=0; iSlotCounter<(INT32)pInventoryPoolList.size(); iSlotCounter++)
	{
		if(pInventoryPoolList[iSlotCounter].bVisible==1 && pInventoryPoolList[iSlotCounter].fExists && pInventoryPoolList[iSlotCounter].usFlags&WORLD_ITEM_REACHABLE && pInventoryPoolList[iSlotCounter].object.usItem != NOTHING)
		{
			while(1)
			{
				if(iSlotCounterQ >= (INT32)pInventoryPoolListQ[idx].size())
				{
					pInventoryPoolListQ[idx].resize(pInventoryPoolListQ[idx].size() + MAP_INVENTORY_POOL_SLOT_COUNT);
					iLastInventoryPoolPageQ[idx] = (pInventoryPoolListQ[idx].size() - 1) / MAP_INVENTORY_POOL_SLOT_COUNT;
				}
				if(pInventoryPoolListQ[idx][iSlotCounterQ].object.usItem == NOTHING)
				{
					pInventoryPoolListQ[idx][iSlotCounterQ] = pInventoryPoolList[iSlotCounter];
					DeleteObj(&pInventoryPoolList[iSlotCounter].object);
					iSlotCounterQ++;
					break;
				}
				iSlotCounterQ++;
			}
		}
	}
	SwitchToInventoryPoolQ(idx|'0');
	return(TRUE);
}

BOOLEAN CopySectorInventoryToInventoryPoolQs(UINT8 idx)//dnl ch75 021113
{
	UINT32 iSlotCounter, iSlotCounterQ[INVPOOLLISTNUM], usItemClass;
	WORLDITEM *pipl;

	if(!(gWorldSectorX == sSelMapX && gWorldSectorY == sSelMapY && gbWorldSectorZ == iCurrentMapSectorZ))
		return(FALSE);
	memset(iSlotCounterQ, 0, sizeof(iSlotCounterQ));
	for(iSlotCounter=0; iSlotCounter<(INT32)pInventoryPoolList.size(); iSlotCounter++)
	{
		pipl = &pInventoryPoolList[iSlotCounter];
		if(pipl->fExists)
		{
			usItemClass = Item[pipl->object.usItem].usItemClass;
			if(usItemClass & IC_MAPFILTER_GUN)
				idx = 1;
			else if(usItemClass & IC_MAPFILTER_AMMO)
				idx = 2;
			else if(usItemClass & IC_MAPFILTER_EXPLOSV)
				idx = 3;
			else if(usItemClass & IC_MAPFILTER_MELEE)
				idx = 4;
			else if(usItemClass & IC_MAPFILTER_ARMOR)
				idx = 5;
			else if(usItemClass & IC_MAPFILTER_LBE)
				idx = 6;
			else if(usItemClass & IC_MAPFILTER_KIT)
				idx = 7;
			else
				idx = 8;
			if((gInventoryPoolIndex & 0x0F) == idx)
				continue;
			while(1)
			{
				if(iSlotCounterQ[idx] >= (INT32)pInventoryPoolListQ[idx].size())
				{
					pInventoryPoolListQ[idx].resize(pInventoryPoolListQ[idx].size() + MAP_INVENTORY_POOL_SLOT_COUNT);
					iLastInventoryPoolPageQ[idx] = (pInventoryPoolListQ[idx].size() - 1) / MAP_INVENTORY_POOL_SLOT_COUNT;
				}
				if(pInventoryPoolListQ[idx][iSlotCounterQ[idx]].object.usItem == NONE)
				{
					pInventoryPoolListQ[idx][iSlotCounterQ[idx]] = *pipl;
					pipl->object.ubNumberOfObjects = 0;
					pipl->object.usItem = NONE;
					pipl->fExists = FALSE;
					pipl->bVisible = FALSE;
					iSlotCounterQ[idx]++;
					break;
				}
				iSlotCounterQ[idx]++;
			}
		}
	}
	fMapPanelDirty = TRUE;
	return(TRUE);
}

BOOLEAN	DisplaySectorItemsInfo(void)//dnl ch51 090913 //dnl ch75 021113
{
	UINT32 iCounter, iItemCount=0, iItemTraps=0, iItemBombs=0;
	WORLDITEM *pWorldItem = gWorldItems.size()==0 ? NULL : &gWorldItems.front();
	for(iCounter=0; iCounter<guiNumWorldItems; iCounter++, pWorldItem++)
	{
		if(!pWorldItem->fExists)
			continue;
		pWorldItem = &gWorldItems[iCounter];
		if(pWorldItem->bVisible<=0 && pWorldItem->fExists && !(pWorldItem->object.usItem==NOTHING || pWorldItem->object.usItem==SWITCH || pWorldItem->object.usItem==ACTION_ITEM || pWorldItem->object.usItem==OWNERSHIP/* || pWorldItem->object.usItem==SYRINGE_3 || pWorldItem->object.usItem==SYRINGE_4 || pWorldItem->object.usItem==SYRINGE_5*/))
			iItemCount++;
		if(pWorldItem->object[0]->data.bTrap > 0)
		{
			iItemTraps++;
			if(pWorldItem->object[0]->data.misc.usBombItem)
				iItemBombs++;
		}
//SendFmtMsg("Item%.4d:%4d %d %.4X %.4X %.2X %.2X %.2X %d", iCounter, pWorldItem->bVisible, pWorldItem->fExists, pWorldItem->sGridNo, pWorldItem->usFlags, pWorldItem->object[0]->data.bTrap, pWorldItem->object[0]->data.fUsed, pWorldItem->object[0]->data.misc.usBombItem, pWorldItem->object.usItem);
	}
	ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"%d unseen items and %d boobytraps left with %d/%d bombs.", iItemCount, iItemTraps, iItemBombs, guiNumWorldBombs);
	return(TRUE);
}
//dnl ch51 081009 finish

#ifdef INVFIX_Moa//dnl ch85 050214
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
#endif

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

// HEADROCK HAM 5: Added argument
INT32 SellItem( OBJECTTYPE& object, BOOLEAN fAll, BOOLEAN useModifier )
{
	INT32 iPrice = 0;
	INT16 iPriceModifier = gGameExternalOptions.iPriceModifier;
	UINT16 usItemType = object.usItem;
	UINT16 itemPrice = Item[usItemType].usPrice;

	// HEADROCK HAM 5: Controls whether we price the top object or all objects in the stack.
	UINT8 ubNumberOfObjects;
	if (!fAll)
	{
		ubNumberOfObjects = 1;
	}
	else
	{
		ubNumberOfObjects = object.ubNumberOfObjects;
	}

	//CHRISL: make price modifier based on current game progress
	if(iPriceModifier == 0)
	{
		iPriceModifier = max(1, (INT16)(((FLOAT)CurrentPlayerProgressPercentage() / 5.0f) + 0.5f));
	}
	else if(iPriceModifier == -1)
	{
		iPriceModifier = max(1, (INT16)(((100.0f - (FLOAT)CurrentPlayerProgressPercentage()) / 5.0f) + 0.5f));
	}

	if( Item[ usItemType ].usItemClass == IC_AMMO )
	{
		//we are selling ammo
		UINT16 magSize = Magazine[ Item[ usItemType ].ubClassIndex ].ubMagSize;
		for (UINT8 ubLoop = 0; ubLoop < ubNumberOfObjects; ubLoop++)
		{
			iPrice += (INT32)( itemPrice * (float) object[ubLoop]->data.ubShotsLeft / magSize );
		}
	}
	//CHRISL: If we're dealing with money, we want to use the money's amount and just return that value with no modification
	else if(Item[usItemType].usItemClass == IC_MONEY)
	{
		for (UINT8 ubLoop = 0; ubLoop < ubNumberOfObjects; ubLoop++)
		{
			iPrice += (INT32)(object[ubLoop]->data.money.uiMoneyAmount);
		}
		return iPrice;
	}
	else if(Item[usItemType].usItemClass == IC_LBEGEAR && UsingNewInventorySystem() == true)
	{
		//CHRISL: If we're selling an LBE Item, we need to verify if it's an LBENODE, first.  If it is, we need to sell
		//	everything stored in the LBENODE before we sell teh LBE Item itself.
		for(UINT8 ubLoop = 0; ubLoop < ubNumberOfObjects; ++ubLoop)
		{
			if(object.IsActiveLBE(ubLoop) == true)
			{
				LBENODE* pLBE = object.GetLBEPointer(ubLoop);
				if(pLBE)
				{
					UINT8 invsize = pLBE->inv.size();
					for(UINT8 x = 0; x < invsize; ++x)
					{
						if(pLBE->inv[x].exists() == true)
						{
							// HEADROCK HAM 5: Added argument
							iPrice += SellItem(pLBE->inv[x], TRUE, FALSE);
						}
					}
				}
			}
			iPrice += ( itemPrice * object[ubLoop]->data.objectStatus / 100 );
			for (attachmentList::iterator iter = object[ubLoop]->attachments.begin(); iter != object[ubLoop]->attachments.end(); ++iter) {
				if(iter->exists())
				{
					// HEADROCK HAM 5: Added argument
					iPrice += SellItem(*iter, TRUE, FALSE);
				}
			}
		}
	}
	else
	{
		//we are selling a gun or something - it could be stacked or single, and if single it could have attachments
		for (UINT8 ubLoop = 0; ubLoop < ubNumberOfObjects; ubLoop++)
		{
			iPrice += ( itemPrice * object[ubLoop]->data.objectStatus / 100 );
			for (attachmentList::iterator iter = object[ubLoop]->attachments.begin(); iter != object[ubLoop]->attachments.end(); ++iter) {
				if(iter->exists())
				{
					// HEADROCK HAM 5: Added argument
					iPrice += SellItem(*iter, TRUE, FALSE);
				}
			}
		}

	}

	if( iPriceModifier > 1 && useModifier == TRUE) {
		iPrice /= iPriceModifier;
	}

	return iPrice;
}

// HEADROCK HAM 5: This function handles altering the coordinates and sizes of all map inventory slots. This is used
// for a switch between Small and Large inventory item display ("zoom").
void ResetMapInventoryOffsets( void )
{
	if ( !fMapInventoryZoom )
	{
		//////////////////////////////////////////////
		// ZOOM OFF
		if (isWidescreenUI() ||iResolution >= _1024x768)
		{
			MAP_INV_SLOT_COLS = 17;
			MAP_INVENTORY_POOL_SLOT_START_X = UI_MAP.BorderRegion.x + 25;
			MAP_INVENTORY_POOL_SLOT_START_Y = UI_MAP.BorderRegion.y + 51;
		}
		else if (iResolution >= _800x600 && iResolution < _1024x768)
		{
			MAP_INV_SLOT_COLS = 11;
			MAP_INVENTORY_POOL_SLOT_START_X = 278 + xResOffset;
			MAP_INVENTORY_POOL_SLOT_START_Y = 62 + yResOffset;
		}
		else
		{
			MAP_INV_SLOT_COLS = 8;
			MAP_INVENTORY_POOL_SLOT_START_X = 269 + xResOffset;
			MAP_INVENTORY_POOL_SLOT_START_Y = 51 + yResOffset;
		}
		//else if (iResolution >= _1024x768)
		//{
		//	MAP_INV_SLOT_COLS = 17;
		//	MAP_INVENTORY_POOL_SLOT_START_X = 282 + xResOffset;
		//	MAP_INVENTORY_POOL_SLOT_START_Y = 50 + yResOffset;
		//}
		MAP_INVENTORY_POOL_SLOT_COUNT = GetInventorySlotCount( FALSE );

		ITEMDESC_ITEM_STATUS_HEIGHT_INV_POOL = 20;
		ITEMDESC_ITEM_STATUS_WIDTH_INV_POOL = 2;
		ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_X = 5;
		ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y = 22;

		ITEMDESC_ITEM_NAME_POOL_OFFSET_Y = 3;

		MAP_INVEN_SLOT_WIDTH = 65;
		MAP_INVEN_SPACE_BTWN_SLOTS = 72;
		MAP_INVEN_SLOT_HEIGHT = 32;
		MAP_INVEN_SLOT_IMAGE_HEIGHT = 24;

		MAP_INVEN_NAME_FONT = BLOCKFONTNARROW;

	}
	else
	{
		////////////////////////////////////
		// ZOOM ON

		if (isWidescreenUI() || iResolution >= _1024x768)
		{
			MAP_INV_SLOT_COLS = 8;
			MAP_INVENTORY_POOL_SLOT_START_X = UI_MAP.BorderRegion.x + 25;
			MAP_INVENTORY_POOL_SLOT_START_Y = UI_MAP.BorderRegion.y + 51;
		}
		else if (iResolution >= _800x600 && iResolution < _1024x768)
		{
			MAP_INV_SLOT_COLS = 5;
			MAP_INVENTORY_POOL_SLOT_START_X = 278 + xResOffset;
			MAP_INVENTORY_POOL_SLOT_START_Y = 62 + yResOffset;
		}
		else
		{
			MAP_INV_SLOT_COLS = 3;
			MAP_INVENTORY_POOL_SLOT_START_X = 269 + xResOffset;
			MAP_INVENTORY_POOL_SLOT_START_Y = 51 + yResOffset;
		}
		//else if (iResolution >= _1024x768)
		//{
		//	MAP_INV_SLOT_COLS = 8;
		//	MAP_INVENTORY_POOL_SLOT_START_X = 282 + xResOffset;
		//	MAP_INVENTORY_POOL_SLOT_START_Y = 50 + yResOffset;
		//}

		MAP_INVENTORY_POOL_SLOT_COUNT = GetInventorySlotCount( TRUE );
		ITEMDESC_ITEM_STATUS_HEIGHT_INV_POOL = 40;
		ITEMDESC_ITEM_STATUS_WIDTH_INV_POOL = 3;
		ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_X = 5;
		ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y = 44;

		ITEMDESC_ITEM_NAME_POOL_OFFSET_Y = 10;

		MAP_INVEN_SLOT_WIDTH = 129;
		MAP_INVEN_SPACE_BTWN_SLOTS = 135;
		MAP_INVEN_SLOT_HEIGHT = 67;
		MAP_INVEN_SLOT_IMAGE_HEIGHT = 49;

		MAP_INVEN_NAME_FONT = FONT10ARIAL;
	}
}

// HEADROCK HAM 5: Load the BigItem graphics for all items on this page of the sector inventory.
// Because BigItem graphics are not kept in memory by default, we need to load them now in order to display all items
// on the screen. Note that this function will run every time the contents of the inventory are changed, in order to
// ensure that new items now appearing on the screen have their BigItem pics accessible.
void LoadAllMapInventoryBigItemGraphics()
{
	INT32 iCounter = 0;

	// Go through each and every slot that exists on the screen.
	for( iCounter = 0; iCounter < MAP_INVENTORY_POOL_SLOT_COUNT ; iCounter++ )
	{
		LoadMapInventoryBigItemGraphic( iCounter );
	}
}

// HEADROCK HAM 5: This function loads a specific BigItem graphic for one of the items in the Sector Inventory.
// This is done because BigItem graphics are not stored in memory - this function allows us to access one of them,
// using only its position in the inventory.
void LoadMapInventoryBigItemGraphic( INT32 iCounter )
{
	// Get the item's absolute position in the sector inventory
	INT32 iItemNum = iCounter + (iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT);
	UINT32 uiItemGraphic;
	// Does the item exist? (If not, it's an empty slot and requires no graphics)
	if (pInventoryPoolList[ iItemNum ].object.exists())
	{
		// Load the graphic from the STI
		LoadTileGraphicForItem( &(Item[ pInventoryPoolList[ iItemNum ].object.usItem ]), &uiItemGraphic );
		// Record the number of graphic used, so we can easily access it later. The array contains only images
		// for the items currently on the screen, so [0] is always the item in the top-left corner.
		giMapInventoryBigItemGraphics[ iCounter ].iGraphicNum = (INT32)uiItemGraphic;
		giMapInventoryBigItemGraphics[ iCounter ].usItem = pInventoryPoolList[ iItemNum ].object.usItem;
	}
	else
	{
		// Empty slot. Reset the number so we don't try to unload the picture here.
		giMapInventoryBigItemGraphics[ iCounter ].iGraphicNum = -1;
		giMapInventoryBigItemGraphics[ iCounter ].usItem = 0;
	}
}

// HEADROCK HAM 5: Unload all BigItem graphics from memory.
// We run this every time the zoomed inventory (or the entire inventory) is closed.
void UnloadAllMapInventoryBigItemGraphics()
{
	for (INT32 iCounter = 0; iCounter < MAP_INVENTORY_POOL_MAX_SLOTS; iCounter++)
	{
		UnloadMapInventoryBigItemGraphic( iCounter );
	}
}

void UnloadMapInventoryBigItemGraphic( INT32 iCounter )
{
	// Skip slots that have no item graphic assigned to them.
	if (giMapInventoryBigItemGraphics [ iCounter ].iGraphicNum > -1)
	{
		DeleteVideoObjectFromIndex( (UINT32)giMapInventoryBigItemGraphics[ iCounter ].iGraphicNum );
		giMapInventoryBigItemGraphics[ iCounter ].iGraphicNum = -1;
		giMapInventoryBigItemGraphics[ iCounter ].usItem = 0;
	}
}

void ResetAllMapInventoryBigItemGraphics()
{
	for (INT32 iCounter = 0; iCounter < MAP_INVENTORY_POOL_MAX_SLOTS; iCounter++)
	{
		giMapInventoryBigItemGraphics[ iCounter ].iGraphicNum = -1;
		giMapInventoryBigItemGraphics[ iCounter ].usItem = 0;
	}
}

// HEADROCK HAM 5: Handle switch to Zoomed inventory on given page.
void HandleMapInventoryZoom( UINT32 iPage, INT32 iCounter )
{
	// Set the zoom flag. This tells the program to draw all items using their large images, and also ensures
	// that all coordinates are set to show large slots.
	fMapInventoryZoom = 1;

	// Erase all of these flags to stop items from being outlined.
	for (UINT16 x = 0; x < MAP_INVENTORY_POOL_MAX_SLOTS; x++)
	{
		gfMapInventoryItemToZoom[x] = FALSE;
	}

	// Before resetting the pool slots, lets determine the position and size of the slot we're about to
	// zoom into. We'll use this for animations.
	INT32 iStartX = (INT32)( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCounter / MAP_INV_SLOT_COLS ) ) );
	INT32 iStartY = (INT32)( MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCounter % ( MAP_INV_SLOT_COLS ) ) ) );
	UINT32 iOrigWidth = MAP_INVEN_SLOT_WIDTH;
	UINT32 iOrigHeight = MAP_INVEN_SLOT_HEIGHT;
	UINT16 iLocationInPool = iCounter + (iCurrentInventoryPoolPage * MAP_INVENTORY_POOL_SLOT_COUNT);

	// Destroy all pool slots...
	DestroyMapInventoryPoolSlots();
	// Reinit the coordinates and sizes for the larger inventory...
	ResetMapInventoryOffsets();
	// Recreate all pool slots...
	CreateMapInventoryPoolSlots( );

	// Set current page
	iCurrentInventoryPoolPage = iPage;

	// Load all BigItem graphics for the current page.
	ResetAllMapInventoryBigItemGraphics();

	AnimateZoomInventory ( iLocationInPool, iCounter, iStartX, iStartY, iOrigWidth, iOrigHeight );

	ResizeInventoryList();

	// Flag as dirty for re-render.
	fMapPanelDirty = TRUE;
}

// HEADROCK HAM 5: Handle switch to Unzoomed inventory on given page.
void HandleMapInventoryUnzoom( UINT32 iPage )
{
	// Reset zoom flag
	fMapInventoryZoom = 0;

	giDesiredNumMapInventorySlots = -1;

	// Load all BigItem graphics for the current page.
	UnloadAllMapInventoryBigItemGraphics();

	// Destroy all pool slots, reinit all inventory coordinates and variables, then recreate slots.
	DestroyMapInventoryPoolSlots();
	ResetMapInventoryOffsets();
	CreateMapInventoryPoolSlots( );

	// Set current page
	iCurrentInventoryPoolPage = iPage;

	// Flag as dirty for re-render.
	fMapPanelDirty = TRUE;
}

// HEADROCK HAM 5: This function tells us how many slots there will be in the zoomed inventory. We run this
// both when creating the zoomed inventory, as well as when trying to calculate which items from unzoomed mode
// will appear in zoomed mode.
UINT16 GetInventorySlotCount( BOOLEAN fZoomed )
{
	if (!fZoomed)
	{
		if (isWidescreenUI() || iResolution >= _1024x768)
		{
			return MAP_INVENTORY_POOL_MAX_SLOTS;
		}
		else if (iResolution >= _800x600 && iResolution < _1024x768)
		{
			return 77;
		}
		else
		{
			return 40;
		}
	}
	else
	{
		if (isWidescreenUI() || iResolution >= _1024x768)
		{
			return 40;
		}
		else if (iResolution >= _800x600 && iResolution < _1024x768)
		{
			return 15;
		}
		else
		{
			return 6;
		}
	}

	// Error!
	AssertMsg( 0, "Zoomed Inventory error: Encountered unknown resolution setting." );
	return 0;
}

// HEADROCK HAM 5: This is a callback function that will cancel inventory zoom input.
void CancelInventoryZoomInput( BOOLEAN fButtonOff )
{
	// Are we being asked to turn the Zoom Button off?
	if (fButtonOff)
	{
		// Do it by referencing the button and toggling its flag manually.
		ButtonList[ guiMapInvenZoomButton ]->uiFlags &=~ (BUTTON_CLICKED_ON);
	}

	// Erase all of these flags to stop items from being outlined.
	for (UINT16 x = 0; x < MAP_INVENTORY_POOL_MAX_SLOTS; x++)
	{
		gfMapInventoryItemToZoom[x] = FALSE;
	}

	// Reset flag to inform the program that we are no longer expecting zoom input. Slot Mouse-Regions will now
	// behave normally.
	fWaitingForZoomInput = 0;
	// Free up the mouse!
	FreeMouseCursor( FALSE );
}

// HEADROCK HAM 5: This function handles animation from Unzoomed inventory mode to Zoomed inventory mode.
// The animation takes an entire "slot" from the inventory view, and moves and stretches it to the exact
// location and dimensions it'll have in "zoomed" mode. This is similar to the zooming of the Laptop transition.
void AnimateZoomInventory ( UINT16 iLocationInPool, UINT16 iCounter, INT32 iStartX, INT32 iStartY, UINT32 uiOrigWidth, UINT32 uiOrigHeight )
{
	// We get here after already having set the offsets for the zoomed mode. We use one now to determine
	// the position of our slot within the zoomed page - which will open as soon as this animation is done.
	// Therefore, we're getting the animation's destination coordinates.
	UINT16 iCounterZoomed = iLocationInPool % MAP_INVENTORY_POOL_SLOT_COUNT;

	// Declarations
	SGPRect SrcRect, CurRect, StartRect, DstRect;
	INT32 iPercentage;
	UINT32 uiStartTime, uiTimeRange, uiCurrTime;
	INT32 iX, iY, iWidth, iHeight;
	INT16 sCenX, sCenY, usWidth, usHeight, sX, sY;
	INT16 sWidth = 0, sHeight = 0;
	ETRLEObject	*pTrav;
	HVOBJECT hHandle;
	INT32 iRealPercentage;

	// Make the mouse cursor disappear for now. It'll automatically restore itself when the animation is done.
	// Disabled for now...
	//SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	// First lets copy the entire screen into the SAVEBUFFER. By doing this we can ensure that the background
	// gets redrawn under our moving sprite.
	BlitBufferToBuffer( FRAME_BUFFER, guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
	// For extra "cool" effect, we'll be making one slot "disappear", by blitting a part of the inventory
	// pool graphic on top of it. This'll give the impression that our slot is actually moving away from its
	// original location, leaving a hole behind it!
	// Get the pool graphic...
	GetVideoObject(&hHandle, guiMapInventoryPoolBackground);
	// Blit to the EXTRABUFFER
	BltVideoObject( guiEXTRABUFFER , hHandle, 0,INVEN_POOL_X, INVEN_POOL_Y , VO_BLT_SRCTRANSPARENCY,NULL );
	// Copy a piece of it back into the frame.
	BlitBufferToBuffer( guiEXTRABUFFER, guiSAVEBUFFER, iStartX, iStartY, uiOrigWidth, uiOrigHeight );

	// Not really necessary, but I'm not sure if I should remove it.
	RestoreBackgroundRects();

	// STEP 1: We make our sprite. This is done by drawing a slot to the top-left corner of the EXTRABUFFER,
	// then drawing our item on it. We'll be copying it back with every frame, at a new destination and
	// size.
	
	// Draw a big-item slot to 0,0.
	GetVideoObject(&hHandle, guiMapInventoryPoolSlot);
	sX = 0;
	sY = 0;
	BltVideoObject( guiEXTRABUFFER , hHandle, 1, sX, sY , VO_BLT_SRCTRANSPARENCY,NULL );

	// Does the object exist?
	if (pInventoryPoolList[ iLocationInPool ].object.exists() )
	{
		// We draw the BigItem graphic on top of the slot.

		// Get the BigItem Graphic for the item.
		LoadMapInventoryBigItemGraphic( iCounterZoomed );
		GetVideoObject( &hHandle, (UINT32)giMapInventoryBigItemGraphics[ iCounterZoomed ].iGraphicNum );
		// Calculate size
		pTrav = &( hHandle->pETRLEObject[ 0 ] );
		usHeight				= (UINT16)pTrav->usHeight;
		usWidth					= (UINT16)pTrav->usWidth;
		// Mandatory offsets...
		sX = 5;
		sY = 1;
		// Find the X,Y for the BigItem Image, so that it would center on the slot.
		sCenX = sX + ( abs(MAP_INVEN_SLOT_WIDTH - usWidth) / 2 ) - pTrav->sOffsetX;
		sCenY = sY + ( abs(MAP_INVEN_SLOT_IMAGE_HEIGHT - usHeight) / 2 ) - pTrav->sOffsetY;
		// Blit to the EXTRABUFFER
		BltVideoObject( guiEXTRABUFFER , hHandle, 0, sCenX, sCenY , VO_BLT_SRCTRANSPARENCY,NULL );

		// We print the item name below the item graphic
		CHAR16 sString[ 64 ];
		wcscpy( sString, ShortItemNames[ pInventoryPoolList[ iLocationInPool ].object.usItem ] );
		if( StringPixLength( sString, MAP_INVEN_NAME_FONT ) >= ( MAP_INVEN_SLOT_WIDTH ) )
		{
			ReduceStringLength( sString, ( INT16 )( MAP_INVEN_SLOT_WIDTH - StringPixLength( L" ...", MAP_INVEN_NAME_FONT ) ), MAP_INVEN_NAME_FONT );
		}
		INT16 iFontWidth = 0;
		INT16 iFontHeight = 0;
		FindFontCenterCoordinates( 2,
			0, MAP_INVEN_SLOT_WIDTH, 0,
			sString, MAP_INVEN_NAME_FONT,
			&iFontWidth, &iFontHeight );

		// Blit to the EXTRABUFFER
		SetFontDestBuffer( guiEXTRABUFFER, 0,0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
		SetFont( MAP_INVEN_NAME_FONT );
		SetFontForeground( FONT_WHITE );
		SetFontBackground( FONT_BLACK );

		mprintf( iFontWidth, ITEMDESC_ITEM_NAME_POOL_OFFSET_Y + ITEMDESC_ITEM_STATUS_INV_POOL_OFFSET_Y, sString );
	}

	// Set the size of the EXTRABUFFER region we want to copy and use as a sprite.
	SrcRect.iLeft =	0;
	SrcRect.iTop = 0;
	SrcRect.iRight = MAP_INVEN_SLOT_WIDTH;
	SrcRect.iBottom = MAP_INVEN_SLOT_HEIGHT;

	// STEP 2: The inventory is now stored in the SAVEBUFFER, and our sprite is in the EXTRABUUFFER.
	// Lets calculate the position and size of the sprite at the start of the animation and at its end.

	// Set the size of the sprite when it starts moving. Topleft should be the top left corner of the slot in
	// the unzoomed inventory, width and height correspond to the size of an unzoomed slot.
	// We get this info from outside the function, because we can't calculate it now that the offsets have
	// already been changed.
	StartRect.iLeft =	iStartX;
	StartRect.iTop =	iStartY;
	StartRect.iRight = iStartX + uiOrigWidth;
	StartRect.iBottom = iStartY + uiOrigHeight;

	// The destination is the top left corner of the slot in zoomed view. Its height and width are defined by
	// the size of a zoomed slot. Again, since we've already altered all the offsets, we can use them now to calculate
	// that position and size.
	DstRect.iLeft = (INT32)( MAP_INVENTORY_POOL_SLOT_OFFSET_X + MAP_INVENTORY_POOL_SLOT_START_X + ( ( MAP_INVEN_SPACE_BTWN_SLOTS ) * ( iCounterZoomed / MAP_INV_SLOT_COLS ) ) );
	DstRect.iTop = (INT32)( MAP_INVENTORY_POOL_SLOT_START_Y + ( ( MAP_INVEN_SLOT_HEIGHT ) * ( iCounterZoomed % ( MAP_INV_SLOT_COLS ) ) ) );
	DstRect.iRight = DstRect.iLeft + MAP_INVEN_SLOT_WIDTH;
	DstRect.iBottom = DstRect.iTop + MAP_INVEN_SLOT_HEIGHT;

	// Calculate Deltas
	
	// Movement of the sprite's top-left corner
	INT32 iDeltaX = DstRect.iLeft - StartRect.iLeft;
	INT32 iDeltaY = DstRect.iTop - StartRect.iTop;
	// Change in width and height
	INT32 iDeltaWidth = (DstRect.iRight - DstRect.iLeft) - (StartRect.iRight - StartRect.iLeft);
	INT32 iDeltaHeight = (DstRect.iBottom - DstRect.iTop) - (StartRect.iBottom - StartRect.iTop);

	// Set transition time and other variables.
	uiTimeRange = 400;
	iPercentage = iRealPercentage = 0;
	uiStartTime = GetJA2Clock();

	// Loud click!
	PlayJA2Sample( 202, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// ANIMATE
	
	while( iPercentage < 100	)
	{
		// The animation process is quite simple, and is done in a more simple fashion than the Laptop transition
		// as well - though the principle is the same.
		// We first blit the background into the frame buffer to draw it on the screen. Then we calculate the current
		// size and position of the sprite, and blit it to the frame buffer as well.
		// By tracking the time on the clock we can figure out where the sprite should be at any given time,
		// how large it should be, and also use effects to make it look cooler.
		
		// First of all, blit the Map Inventory back from the SAVEBUFFER into the FRAMEBUFFER, so that it gets
		// drawn on the screen as a background. We don't need to draw the entire screen - at least not yet...
		BlitBufferToBuffer( guiSAVEBUFFER, FRAME_BUFFER, INVEN_POOL_X, INVEN_POOL_Y, SCREEN_WIDTH-INVEN_POOL_X, SCREEN_HEIGHT-INVEN_POOL_Y);

		// Get the current time
		uiCurrTime = GetJA2Clock();
		// Compare it to the current time to get a percentage. This signifies how much of the animation has been
		// done.
		iPercentage = (uiCurrTime-uiStartTime) * 100 / uiTimeRange;
		// Can't be more than 100%, duh.
		iPercentage = min( iPercentage, 100 );

		// To make a "falling" effect, we bias the percentage. I'm not sure how the maths work, but they do.
		INT32 iScalePercentage = iPercentage;
		INT32 iFactor = (iScalePercentage - 50) * 2;
		if( iScalePercentage < 50 )
			iScalePercentage = (UINT32)(iScalePercentage + iScalePercentage * iFactor * 0.01 + 0.5);
		else
			iScalePercentage = (UINT32)(iScalePercentage + (100-iScalePercentage) * iFactor * 0.01 + 0.5);

		// Find the width and height the sprite at this time.
		iWidth = (INT32)(uiOrigWidth + (INT32)(( iDeltaWidth * iScalePercentage ) / 100));
		iHeight = (INT32)(uiOrigHeight + (INT32)(( iDeltaHeight * iScalePercentage ) / 100));
		// Find the sprite's current X,Y coordinates
		iX = (INT32)(iStartX + (INT32)(( iDeltaX * iScalePercentage ) / 100));
		iY = (INT32)(iStartY + (INT32)(( iDeltaY * iScalePercentage ) / 100));

		// Set the position and size of the rectangle on which we'll draw this sprite.
		CurRect.iLeft = iX;
		CurRect.iRight = iX + iWidth;
		CurRect.iTop = iY;
		CurRect.iBottom = iY + iHeight;

		// Stretch the image of the sprite, still stored in the EXTRABUFFER, to its correct
		// size and position on the FRAMEBUFFER, essentially drawing it on the screen on top of
		// the background.
		BltStretchVideoSurface( FRAME_BUFFER, guiEXTRABUFFER, 0, 0, 0, &SrcRect, &CurRect );

		// Something....
		InvalidateScreen();

		// Some other thing...
		RefreshScreen( NULL );
	}
	// Unload the image of the item we just animated. Yeah, we'll be using it in two seconds for drawing
	// the zoomed inventory page, but it's safer if it doesn't exist in memory anymore. I'm not good enough
	// to figure out whether this is necessary :)
	UnloadMapInventoryBigItemGraphic( iCounterZoomed );

	// Dirty!
	fMapPanelDirty=TRUE;
}

// HEADROCK HAM 5: This function sorts all ammo items in the inventory into crates. It is the same operation
// performed when pressing CTRL+SHIFT+A in the Tactical screen, except it works directly with the sector
// inventory. Most of the function was ripped from that function - with several significant changes. For one,
// the program never looks at any mercs' crates-in-hand.
void SortSectorInventoryAmmo(bool useBoxes)
{
	
	// Declarations
	INT32	crateItem;
	bool	mergeSuccessful = false;
	OBJECTTYPE	newCrate;
	int loopCount = 0;

	SOLDIERTYPE * pSoldier = &(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ]);
	
	AssertMsg( pSoldier != NULL, "Sector Inventory: Attempting ammo sort without valid selected soldier?" );

	//MM: added for ammo boxes
	bool ammoPresent = true;
	UINT8 magType = AMMO_CRATE;
	if (useBoxes)
		magType = AMMO_BOX;

	// Get the original size of the inventory.
	UINT32 uiOrigInvSize = pInventoryPoolList.size();

	while (ammoPresent && loopCount <= 10 )
	{
		// Start scanning through the entire Sector Inventory pool.
		for(UINT32 iInvCounter = 0; iInvCounter < uiOrigInvSize; ++iInvCounter)
		{
			// Set values
			crateItem = 0;
			mergeSuccessful = false;

			// Skip objects that don't exist (empty slots)
			if(pInventoryPoolList[iInvCounter].object.exists() == false)
			{
				continue;
			}
		
			// Store a pointer to this object and record its item number
			OBJECTTYPE *pCurObject = &(pInventoryPoolList[iInvCounter].object);
			UINT16 usCurItem = pCurObject->usItem;

			// If the object....
			if(Item[usCurItem].usItemClass & IC_AMMO && // Is an ammo item
				Magazine[Item[usCurItem].ubClassIndex].ubMagType != magType && // MM: Is not current crate/box selection
				pInventoryPoolList[iInvCounter].usFlags & WORLD_ITEM_REACHABLE && // Is reachable
				!(pInventoryPoolList[iInvCounter].usFlags & WORLD_ITEM_ARMED_BOMB) ) // Is not boobytrapped!
			{
				// We have a valid, ammo item - one or more magazines. We'll want to dump as much ammo from them
				// as possible into crates. 

				// Look through all items in the game to try and find an ammocrate that can contain this kind of ammo.
				for (INT32 iCrateLoop = 0; iCrateLoop < (INT32)gMAXITEMS_READ; ++iCrateLoop)
				{
					// Is it the right ammo crate?
					if( Item[iCrateLoop].usItemClass == IC_AMMO &&
						Magazine[Item[iCrateLoop].ubClassIndex].ubMagType == magType && // An ammo crate or box
						Magazine[Item[iCrateLoop].ubClassIndex].ubCalibre == Magazine[Item[usCurItem].ubClassIndex].ubCalibre && //Same caliber
						Magazine[Item[iCrateLoop].ubClassIndex].ubAmmoType == Magazine[Item[usCurItem].ubClassIndex].ubAmmoType ) // Same ammotype
					{
						// Found a crate for this ammo.
						crateItem = iCrateLoop;
						break;
					}
				}

				// Have we found a crate?
				if(crateItem != 0)
				{
					// Excellent. Let see if a crate like this already exists in the Sector Inventory.
					for(UINT32 iInvCounter2=0; iInvCounter2 < pInventoryPoolList.size(); ++iInvCounter2)
					{
						// Non-empty slots please...
						if(pInventoryPoolList[iInvCounter2].object.exists() == true)
						{
							// Is it our crate item?
							if (pInventoryPoolList[iInvCounter2].object.usItem == crateItem)
							{
								// Try pouring our ammo into this crate.
								DistributeStatus(pCurObject, &(pInventoryPoolList[iInvCounter2].object), Magazine[Item[crateItem].ubClassIndex].ubMagSize);
								// If the ammo magazine disappeared, that means we were successful in passing all
								// ammo from it to the crate.
								if(pCurObject->ubNumberOfObjects < 1)
								{
									mergeSuccessful = true;
									// Delete the magazine, it's empty anyway.
									DeleteObj( pCurObject );
									break;
								}
								// If we've reched this point, then we've still got ammo to distribute. Keep
								// looking through the inventory for additional crates to dump it into.
							}
						}
					}
					// Did we find enough crates and dumped all our ammo into them?
					if(mergeSuccessful == false)
					{
						// Hmm, so we need to create a new crate.
						CreateAmmo(crateItem, &newCrate, 0);
						// Dump all the ammo into it.
						DistributeStatus(pCurObject, &newCrate, Magazine[Item[crateItem].ubClassIndex].ubMagSize);
						// Place it in the sector inventory.
						AutoPlaceObjectToWorld(pSoldier, &newCrate, TRUE);
						// Ran out of magazines?
						if(pCurObject->ubNumberOfObjects < 1)
						{
							// Excellent, we're done.
							mergeSuccessful = true;
							// Delete this magazine item.
							DeleteObj( pCurObject );
						}
					}
				}
			}
		}

		// if we added to / created a box/crate, then we're fine to reset this
		if ( mergeSuccessful )
			loopCount = 0;

		//MM: loop through ammo multiple times, as boxes and crates may take a few passes to fill
		ammoPresent = false;
		for(unsigned int i = 0; i < pInventoryPoolList.size(); i++)
		{
			if(Item[pInventoryPoolList[i].object.usItem].usItemClass == IC_AMMO && pInventoryPoolList[i].bVisible == TRUE && pInventoryPoolList[i].fExists && (pInventoryPoolList[i].usFlags & WORLD_ITEM_REACHABLE) && !(pInventoryPoolList[i].usFlags & WORLD_ITEM_ARMED_BOMB))
			{
				if(Magazine[Item[pInventoryPoolList[i].object.usItem].ubClassIndex].ubMagType == magType)
					continue;

				loopCount++;
				ammoPresent = true;
				break;
			}
		}

		SortSectorInventoryStackAndMerge(true);
	}
	// FINISHED
	CHAR16 pStr[500];
	swprintf( pStr, gzMapInventorySortingMessage[ 0 ], 'A' + sSelMapY - 1, sSelMapX );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pStr );

	// Dirty!
	fMapPanelDirty=TRUE;
}

// HEADROCK HAM 5: This function removed all ammunition from each item in the sector inventory. It is the same
// as the SHIFT+F function, except it deals directly with the sector inventory pool rather than the WorldItems list.
void SortSectorInventoryEjectAmmo()
{
	OBJECTTYPE gTempObject;
	
	SOLDIERTYPE * pSoldier = &(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ]);

	for ( UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size(); uiLoop++ ) //for all items in sector
	{
		if ( pInventoryPoolList[uiLoop].bVisible == TRUE && // Visible
			pInventoryPoolList[uiLoop].object.exists() == true && // Exists
			pInventoryPoolList[uiLoop].usFlags & WORLD_ITEM_REACHABLE && // Reachable
			!(pInventoryPoolList[uiLoop].usFlags & WORLD_ITEM_ARMED_BOMB) ) // Not booby-trapped!
		{
			if (Item[ pInventoryPoolList[uiLoop].object.usItem ].usItemClass & IC_GUN) // Is a gun?
			{
				// Iterate through stacks
				for (int x = 0; x < pInventoryPoolList[uiLoop].object.ubNumberOfObjects; ++x) 
				{
					//Remove magazine
					if ( (pInventoryPoolList[uiLoop].object[x]->data.gun.usGunAmmoItem != NONE) && (pInventoryPoolList[uiLoop].object[x]->data.gun.ubGunShotsLeft > 0) )
					{
						CreateAmmo(pInventoryPoolList[uiLoop].object[x]->data.gun.usGunAmmoItem, &gTempObject, pInventoryPoolList[uiLoop].object[x]->data.gun.ubGunShotsLeft);
						pInventoryPoolList[uiLoop].object[x]->data.gun.ubGunShotsLeft = 0;
						pInventoryPoolList[uiLoop].object[x]->data.gun.usGunAmmoItem = NONE;
						// HEADROCK HAM 3.5: Clear ammo type
						pInventoryPoolList[uiLoop].object[x]->data.gun.ubGunAmmoType = NONE;

						// put it on the ground
						AutoPlaceObjectToWorld(pSoldier, &gTempObject, TRUE);

						// Check...
						if (&gTempObject != NULL)
						{
							DeleteObj( &gTempObject );
						}
					}
				}
			}
		}
	}
	// "FINISHED"
	CHAR16 pStr[500];
	swprintf( pStr, gzMapInventorySortingMessage[ 2 ], 'A' + sSelMapY - 1, sSelMapX );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pStr );

	// Dirty!
	fMapPanelDirty=TRUE;
}

void SortSectorInventoryEmptyLBE() {
	// current item
	WORLDITEM * pInventoryItem = NULL;

	SOLDIERTYPE * pSoldier = &(Menptr[gCharactersList[bSelectedInfoChar].usSolID]);

	for (UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size(); uiLoop++) //for all items in sector
	{
		pInventoryItem = &(pInventoryPoolList[uiLoop]);

		if (pInventoryItem->bVisible == TRUE && // Visible
			pInventoryItem->object.exists() == true && // Exists
			pInventoryItem->usFlags & WORLD_ITEM_REACHABLE && // Reachable
			!(pInventoryItem->usFlags & WORLD_ITEM_ARMED_BOMB)) // Not booby-trapped!
		{
			// Iterate through stacks
			for (int x = 0; x < pInventoryItem->object.ubNumberOfObjects; ++x)
			{
				UINT8 size = 0, cnt = 0, uiLoopCnt = 0;
				// Bob: empty LBE items in sector inventory
				if (pInventoryItem->object.IsActiveLBE(x)) {
					LBENODE * lbePtr = pInventoryItem->object.GetLBEPointer(x);
					INVTYPE & LBEType = Item[pInventoryItem->object.usItem];

					if (lbePtr != NULL) {
						int dropCnt = 0;

						for (auto lbeInvIter = lbePtr->inv.begin(); lbeInvIter != lbePtr->inv.end(); lbeInvIter++) {
							OBJECTTYPE * LBEStack = lbeInvIter._Ptr;
							dropCnt += LBEStack->ubNumberOfObjects;

							AutoPlaceObjectToWorld(pSoldier, LBEStack, TRUE);
							if (LBEStack != NULL)
							{
								DeleteObj(lbeInvIter._Ptr);
							}
						}

						if (dropCnt > 0) {
							ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, gzMapInventorySortingMessage[5], dropCnt, LBEType.szItemName);
						}
						else {
							ScreenMsg(FONT_MCOLOR_LTRED, MSG_INTERFACE, gzMapInventorySortingMessage[6], LBEType.szItemName);
						}

						if (DestroyLBEIfEmpty(&(pInventoryItem->object), x)) {
							ScreenMsg(FONT_MCOLOR_LTGREEN, MSG_INTERFACE, gzMapInventorySortingMessage[7], LBEType.szItemName);
						}
						else {
							ScreenMsg(FONT_MCOLOR_LTRED, MSG_INTERFACE, gzMapInventorySortingMessage[8], LBEType.szItemName);
						}
					}
					else { // can't get LBE pointer, something is wrong
						ScreenMsg(FONT_MCOLOR_LTRED, MSG_INTERFACE, gzMapInventorySortingMessage[9], LBEType.szItemName);
					}
				} // IsActiveLBE
			} // Iterate through stacks
		} // pInventoryPoolList loop
	}

	// "FINISHED"
	CHAR16 pStr[500];
	swprintf(pStr, gzMapInventorySortingMessage[4], 'A' + sSelMapY - 1, sSelMapX);
	ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pStr);

	// Dirty!
	fMapPanelDirty = TRUE;
}

void SortSectorInventorySeparateAttachments()
{
	// attachment ref and pointer
	//OBJECTTYPE gTempObject;
	OBJECTTYPE * gpTempObject = NULL;

	// current item
	WORLDITEM * pInventoryItem = NULL;


	SOLDIERTYPE * pSoldier = &(Menptr[gCharactersList[bSelectedInfoChar].usSolID]);

	for (UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size(); uiLoop++) //for all items in sector
	{
		pInventoryItem = &(pInventoryPoolList[uiLoop]);

		if (pInventoryItem->bVisible == TRUE && // Visible
			pInventoryItem->object.exists() == true && // Exists
			pInventoryItem->usFlags & WORLD_ITEM_REACHABLE && // Reachable
			!(pInventoryItem->usFlags & WORLD_ITEM_ARMED_BOMB)) // Not booby-trapped!
		{
			// Iterate through stacks
			for (int x = 0; x < pInventoryItem->object.ubNumberOfObjects; ++x)
			{
				UINT8 size = 0, cnt = 0, uiLoopCnt = 0;

				// Iterate backwards through attachments in order to detach grenades from underbarrel
				// launchers before detaching the launchers (and their grenade slots) themselves.
				// Since the size of the attachmentList remains constant under NAS but decrements by one
				// under OAS, recheck the list size every iteration in order to calculate an rindex.
				while ((size = pInventoryItem->object[x]->attachments.size()) != cnt)
				{
					OBJECTTYPE * pNewObj = new OBJECTTYPE();

					// Flugente: prevent an underflow
					UINT8 attindex = size > cnt ? size - 1 - cnt : 0;

					gpTempObject = pInventoryItem->object[x]->GetAttachmentAtIndex( attindex );

					//WarmSteel - This actually still works with NAS, be it by accident
					if (gpTempObject != NULL && pInventoryItem->object.RemoveAttachment(gpTempObject, pNewObj, x))
					{
						gpTempObject = pNewObj;

						AutoPlaceObjectToWorld(pSoldier, gpTempObject, TRUE);
						if (gpTempObject != NULL)
						{
							DeleteObj(gpTempObject);
						}
					}
					else
					{
						++cnt;
					}

					++uiLoopCnt;

					// Failsafe
					if (uiLoopCnt > 50)
					{
						break;
					}
				}
			} // Iterate through stacks		
		} // pInventoryPoolList loop
	}

	// "FINISHED"
	CHAR16 pStr[500];
	swprintf(pStr, gzMapInventorySortingMessage[1], 'A' + sSelMapY - 1, sSelMapX);
	ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pStr);

	// Dirty!
	fMapPanelDirty = TRUE;
}

// HEADROCK HAM 5: This function sorts all inventory items into stacks. It is similar to a hotkey-driven
// function that normally runs only in tactical mode, except it has been altered to work directly with the
// sector inventory pool.
void SortSectorInventoryStackAndMerge(bool ammoOnly )
{
	OBJECTTYPE * StackObject;
	
	SOLDIERTYPE * pSoldier = &(Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ]);

	for ( UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size(); uiLoop++ )
	{
		// anv: disable soldier's ownership, as otherwise stacked backpacks would share it
		pInventoryPoolList[ uiLoop ].soldierID = (-1);
	}

	// Run through sector inventory.
	for ( UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size(); uiLoop++ )
	{
		if ( pInventoryPoolList[ uiLoop ].bVisible == TRUE && // Visible
			pInventoryPoolList[ uiLoop ].object.exists() == true && // Exists
			pInventoryPoolList[ uiLoop ].usFlags & WORLD_ITEM_REACHABLE &&  // Reachable
			!(pInventoryPoolList[ uiLoop ].usFlags & WORLD_ITEM_ARMED_BOMB) ) // Not boobytrapped!
		{
			if ( ammoOnly && Item[pInventoryPoolList[uiLoop].object.usItem].usItemClass != IC_AMMO )
				continue;

			// Set this as the stack into which we'll try to pool all other items of this type.
			StackObject = &(pInventoryPoolList[uiLoop].object);
			UINT16 usStackItem = StackObject->usItem;

			// Check whether there are any other objects of the same type that we can add to this stack.
			// We run only forward through the inventory, thus skipping any stacks that have already been checked.
			for(UINT32 i = uiLoop+1; i < pInventoryPoolList.size(); i++)
			{
				// Did we encounter a valid item of the same type?
				if(pInventoryPoolList[i].object.usItem == usStackItem && // Same type
					pInventoryPoolList[i].bVisible == TRUE && // Visible
					pInventoryPoolList[i].object.exists() == true && // Exists
					pInventoryPoolList[i].usFlags & WORLD_ITEM_REACHABLE && // Reachable
					!(pInventoryPoolList[i].usFlags & WORLD_ITEM_ARMED_BOMB) ) // Not boobytrapped!
				{
					// Add it to the stack!
					StackObject->AddObjectsToStack(pInventoryPoolList[i].object, -1, NULL, NUM_INV_SLOTS, MAX_OBJECTS_PER_SLOT, false);

					// Have we removed all items from the secondary stack?
					if( pInventoryPoolList[i].object.exists() == false )
					{
						// Destroy it. 
						DeleteObj( &(pInventoryPoolList[i].object) );
					}
				}
			}
			//merge items in stack
			CleanUpStack(StackObject, NULL);

			// Pick up the stack, and place it back into the inventory. That will sort items back into any empty
			// slots.

			// Create a new object, same as this stack.
			OBJECTTYPE TempStack;
			TempStack.initialize();
			TempStack = pInventoryPoolList[ uiLoop ].object;

			// Erase the original from its position in the inventory.
			pInventoryPoolList[ uiLoop ].bVisible = FALSE;
			pInventoryPoolList[ uiLoop ].fExists = FALSE;
			DeleteObj( &(pInventoryPoolList[ uiLoop ].object) );

			// Place the duplicate into the inventory, filling any gaps.
			AutoPlaceObjectToWorld( pSoldier, &TempStack, TRUE );
			if (TempStack.exists() == true )
			{
				DeleteObj( &TempStack );
			}
		}
	}
	// "FINISHED"
	CHAR16 pStr[500];
	swprintf( pStr, gzMapInventorySortingMessage[ 3 ], 'A' + sSelMapY - 1, sSelMapX );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pStr );

	// Dirty!
	fMapPanelDirty = TRUE;
}

// Flugente: on a key press, we loop over each team members' inventory and exchange it with world items that have a higher status
// however, we ignore items that have inseparable attachments
// the goal is to simulate the player manually changing items, as that is rather tedious
BOOL GetBetterObject_InventoryPool(UINT16 usItem, INT16 status, UINT32& arLoop, UINT8& arIndex)
{
	OBJECTTYPE* pBestObj = NULL;

	INT16	beststatus = status;
	BOOLEAN	found = FALSE;

	for (UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size(); ++uiLoop)				// ... for all items in the world ...
	{
		if (pInventoryPoolList[uiLoop].object.exists() &&
			pInventoryPoolList[uiLoop].usFlags & WORLD_ITEM_REACHABLE &&
			!(pInventoryPoolList[uiLoop].usFlags & WORLD_ITEM_ARMED_BOMB) &&
			pInventoryPoolList[uiLoop].bVisible == VISIBLE &&
			pInventoryPoolList[uiLoop].object.usItem == usItem)
		{
			OBJECTTYPE* pObj = &(pInventoryPoolList[uiLoop].object);

			if (pObj != NULL )
			{
				for (UINT8 i = 0; i < pObj->ubNumberOfObjects; ++i)
				{
					if ((*pObj)[i]->data.objectStatus > beststatus)
					{
						// ignore if there are inseparable attachments - we wouldn't be able to change those by hand
						attachmentList::iterator iterend = (*pObj)[i]->attachments.end();
						for (attachmentList::iterator iter = (*pObj)[i]->attachments.begin(); iter != iterend; ++iter)
						{
							if (iter->exists() && Item[iter->usItem].inseparable)
							{
								continue;
							}
						}

						beststatus = (*pObj)[i]->data.objectStatus;
						arLoop = uiLoop;
						arIndex = i;
						found = TRUE;
					}
				}
			}
		}
	}

	return found;
}

// Flugente: tell us the position of the first inventory item with a fitting calibre and ammotype
BOOL GetFittingAmmo_InventoryPool( UINT8 usCalibre, UINT8 usAmmoType, UINT32& arLoop )
{
	for ( UINT32 uiLoop = 0; uiLoop < pInventoryPoolList.size( ); ++uiLoop )
	{
		if ( pInventoryPoolList[uiLoop].object.exists( ) &&
			 pInventoryPoolList[uiLoop].usFlags & WORLD_ITEM_REACHABLE &&
			 !(pInventoryPoolList[uiLoop].usFlags & WORLD_ITEM_ARMED_BOMB) &&
			 pInventoryPoolList[uiLoop].bVisible == VISIBLE &&
			 (Item[pInventoryPoolList[uiLoop].object.usItem].usItemClass & IC_AMMO) )
		{
			UINT16 usMagIndex = Item[pInventoryPoolList[uiLoop].object.usItem].ubClassIndex;

			if ( usCalibre == Magazine[usMagIndex].ubCalibre )
			{
				if ( usAmmoType == Magazine[usMagIndex].ubAmmoType )
				{						
					arLoop = uiLoop;

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

// HEADROCK HAM 5: This is a helper function that puts together all seen and unseen items from the two
// sector-inventory pools, and then rebuilds the pool. By changing the flags used by the function that tests
// whether an item should appear in the "SEEN" list, we can create filters to show or hide specific items.
//
// NOTE: Never run this function when the pool is closed or not yet created!
void RefreshSeenAndUnseenPools()
{
#ifdef INVFIX_Moa//dnl ch75 011113
	INT32 iItemCount = 0;

	// Create a new itemlist to contain all items in the sector. This is a CPU-cheap alternative to saving them
	// into the tempfile WorldItems list.
	std::vector<WORLDITEM> pItemsList;//dnl ch75 271013
	// What is the number of items currently seen in the Sector Inventory?
	INT32 iTotalNumberSeenItems = GetTotalNumberOfItems( );
	// The number of Unseen items is stored in a global, so lets use it to figure out how many items are in
	// this sector inventory IN TOTAL.
	INT32 iTotalNumItems = iTotalNumberSeenItems + uiNumberOfUnSeenItems;

	// Any items?
	if( iTotalNumItems > 0 )
	{
		// Point to a new array that will hold all these items.
		pItemsList.resize(iTotalNumItems);//dnl ch75 271013

		///////////////////
		// Copy all items from the SEEN inventory
		//
		// These are basically the items seen in the sector inventory view at the moment. The are stored in the
		// main inventory list, the pInventoryPoolList. Lets go through each item on this list.
		for( UINT32 iCounter = 0; iCounter < pInventoryPoolList.size(); iCounter++ )
		{
			// Does it exist? (skip empty slots)
			if( pInventoryPoolList[ iCounter ].object.exists() == true )
			{
				// Copy it into the master pool.
				pItemsList[ iItemCount ] = pInventoryPoolList[ iCounter ];

				// Set it as true and existing... though I'm pretty sure this is unnecessary.
				pItemsList[ iItemCount ].fExists = TRUE;
				pItemsList[ iItemCount ].bVisible = TRUE;
				// Count the number of items we've copied over.
				iItemCount++;
			}
		}
		/////////////////
		// Copy all items from the UNSEEN inventory
		//
		// Unseen items are stored in memory, and their quantity is also stored. This is a
		// separate list from the main inventory.
		// Originally this list only contained items which have not yet been spotted by any
		// merc, including buried items etc. With filters, they now include all items that
		// have been filtered out during the previous Stash Build.
		for( UINT32 iCounter = 0; iCounter < uiNumberOfUnSeenItems; iCounter++ )
		{
			// Skip any empty slots... though there shouldn't be any.
			if (pUnSeenItems[ iCounter ].object.exists() == true )
			{
				// Copy into the master list.
				pItemsList[ iItemCount ] = pUnSeenItems[ iCounter ];

				// Count the number of items we've copied over.
				iItemCount++;
			}
		}
	}

	// Clear both the SEEN and UNSEEN lists.
	pInventoryPoolList.clear();

	INT32 iCurItem = 0;
	INT32 iNumUnseenItems = 0;

	// Do we have any items in the inventory?
	if (iItemCount > 0)
	{
		//////////////////////////////////////////////
		// SPLIT APART
		//
		// We now run through our master item list, splitting the items between the new SEEN
		// and UNSEEN inventories. This is where filters come into play.

		// Start with the SEEN list (the primary inventory). Make it large enough to contain
		// all the items, just in case.
		pInventoryPoolList.resize( iItemCount );

		// Run through the master pool...
		for (INT32 iCounter = 0; iCounter < iItemCount; iCounter++)
		{
			// Is the item visible? This takes into account FILTERS, which make items "invisible"
			// by class.
			if (IsMapScreenWorldItemVisibleInMapInventory( &pItemsList[ iCounter ] ))
			{
				// Add it to the SEEN list - the Sector Inventory Pool.
				pInventoryPoolList[ iCurItem ] = pItemsList[ iCounter ];
				iCurItem++;
			}
		}

		// If we have not yet processed all the items, it means that some are invisible.
		// How many UNSEEN items are there?
		iNumUnseenItems = iItemCount - iCurItem;

		if( iNumUnseenItems > 0 )
		{
			// Put them in the UNSEEN list.

			// Create a new UNSEEN pool of this size.
			pUnSeenItems.resize(iNumUnseenItems);//dnl ch75 271013

			iCurItem = 0;

			// Run through the master inventory.
			for( INT32 iCounter = 0; iCounter < iItemCount; iCounter++ )
			{
				// If item is NOT visible (including filters)
				if( IsMapScreenWorldItemInvisibleInMapInventory( &pItemsList[ iCounter ] ) )
				{
					// Add it to the UNSEEN pool.
					pUnSeenItems[ iCurItem ] = pItemsList[ iCounter ];

					iCurItem++;
				}
			}
		}
	}

	// Resize properly. This function makes sure that we have exactly as many pages as we need:
	// to contain all the items, to have empty enough space at the end, AND to avoid crashes
	// due to the sudden change in inventory size.
	ResizeInventoryList();

	// Store the number of unseen items in memory!
	uiNumberOfUnSeenItems = iNumUnseenItems;

	// Finally, resort the inventory.
	SortSectorInventory( pInventoryPoolList, pInventoryPoolList.size() );
#else
	//!!! pInventoryPoolList and pUnSeenItems must be the same size so never clear neither of them as entering here will throw exception risk CTD if not same
	UINT32 i, j, uiNumOfSlots, uiUnSeenSlots;
	WORLDITEM *pipl, *pusi, *pwi;
	// Move previously unseen items from pUnSeenItems to pInventoryPoolList from the end as there are probably most free slots
	uiNumOfSlots = pInventoryPoolList.size();
	pipl = &pInventoryPoolList.back();
	j = uiNumberOfUnSeenItems;
	pusi = &pUnSeenItems.front();
	while(uiNumOfSlots && j)
	{
		if(pipl->fExists)
		{
			uiNumOfSlots--, pipl--;
			continue;
		}
		while(j)
		{
			if(IsMapScreenWorldItemVisibleInMapInventory(pusi))
			{
				*pipl = *pusi;
				pusi->fExists = FALSE;
				pusi->object.usItem = NONE;
				pusi->object.ubNumberOfObjects = 0;
				uiNumOfSlots--, pipl--;
				j--, pusi++;
				break;
			}
			j--, pusi++;
		}
	}
	// Move previously seen items from pInventoryPoolList to pUnSeenItems 
	i = 0;
	pipl = &pInventoryPoolList.front();
	j = 0;
	pusi = &pUnSeenItems.front();
	uiUnSeenSlots = pUnSeenItems.size();
	while(j < uiUnSeenSlots && i < uiNumOfSlots)
	{
		if(pusi->fExists && j < uiNumberOfUnSeenItems)
		{
			j++, pusi++;
			continue;
		}
		while(i < uiNumOfSlots)
		{
			if(IsMapScreenWorldItemInvisibleInMapInventory(pipl))
			{
				*pusi = *pipl;
				pipl->fExists = FALSE;
				pipl->object.usItem = NONE;
				pipl->object.ubNumberOfObjects = 0;
				if(j >= uiNumberOfUnSeenItems)
					uiNumberOfUnSeenItems++;
				j++, pusi++;
				i++, pipl++;
				break;
			}
			i++, pipl++;
		}
	}
	// Move empty slots to the end of pUnSeenItems and recalculate number of uiNumberOfUnSeenItems
	while(j < uiNumberOfUnSeenItems)
	{
		if(pusi->fExists)
		{
			j++, pusi++;
			continue;
		}
		pwi = &pUnSeenItems[uiNumberOfUnSeenItems-1];
		if(pwi->fExists)
		{
			*pusi = *pwi;
			j++, pusi++;
		}
		uiNumberOfUnSeenItems--;
	}
	SortSectorInventory(pInventoryPoolList, pInventoryPoolList.size());
	fMapPanelDirty = TRUE;
#endif
}

void CreateMapInventoryFilterMenu( )
{
	///////////////////////////
	// CONSTRUCT POPUP MENU
	///////////////////////////
	// If we already have a popup, destroy it first. This ensures we get a fresh menu each time.
	if ( gfMapInventoryFilterPopupInitialized )
	{
		delete(gMapInventoryFilterPopup);
		gMapInventoryFilterPopup = NULL;
		gfMapInventoryFilterPopupInitialized = FALSE;
		gfMapInventoryFilterPopupVisible = FALSE;
	}

	POPUP_OPTION *pOption;
	UINT32 uiFlags;

	// create a popup
	gMapInventoryFilterPopup = new POPUP("MAP INVENTORY FILTER MENU POPUP");	// at this point the name is used mainly for debug output
	
	// add a callback that lets the keyboard handler know we're done (and ready to pop up again)
	gMapInventoryFilterPopup->setCallback(POPUP_CALLBACK_HIDE, new popupCallbackFunction<void,void>( &MapInventoryFilterMenuPopup_Hide ) );
	
	CHAR16 pStr[300];

	// Create menu to toggle groups of item classes on and off individually.

	swprintf( pStr, gzMapInventoryFilterOptions[ 0 ] );
	// Add option: "SHOW ALL"
	uiFlags = IC_MAPFILTER_ALL;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterSet, uiFlags ) );
	if (guiMapInventoryFilter == IC_MAPFILTER_ALL)
	{
		// Set this option off.
		pOption->setAvail(new popupCallbackFunction<bool,TransformInfoStruct*>( &MapInventoryFilterMenuPopup_OptionOff, NULL ));
	}
	// Add the option to the menu.
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_GUN)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 1 ]  );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 1 ]  );
	}
	uiFlags = IC_MAPFILTER_GUN;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_AMMO)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 2 ]  );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 2 ] );
	}
	uiFlags = IC_MAPFILTER_AMMO;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_EXPLOSV)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 3 ] );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 3 ] );
	}
	uiFlags = IC_MAPFILTER_EXPLOSV;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_MELEE)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 4 ] );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 4 ] );
	}
	uiFlags = IC_MAPFILTER_MELEE;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_ARMOR)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 5 ] );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 5 ] );
	}
	uiFlags = IC_MAPFILTER_ARMOR;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_LBE)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 6 ] );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 6 ] );
	}
	uiFlags = IC_MAPFILTER_LBE;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_KIT)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 7 ] );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 7 ] );
	}
	uiFlags = IC_MAPFILTER_KIT;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	if (guiMapInventoryFilter & IC_MAPFILTER_MISC)
	{
		// Hide guns
		swprintf( pStr, L"%s *", gzMapInventoryFilterOptions[ 8 ] );
	}
	else
	{
		// Show guns
		swprintf( pStr, gzMapInventoryFilterOptions[ 8 ] );
	}
	uiFlags = IC_MAPFILTER_MISC;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterToggle, uiFlags ) );
	gMapInventoryFilterPopup->addOption( *pOption );

	swprintf( pStr, gzMapInventoryFilterOptions[ 9 ] );
	// Add option: "HIDE ALL"
	uiFlags = 0;
	pOption = new POPUP_OPTION(&std::wstring( pStr ), new popupCallbackFunction<void,UINT32>( &MapInventoryFilterMenuPopup_FilterSet, uiFlags ) );
	if (guiMapInventoryFilter == 0)
	{
		// Set this option off.
		pOption->setAvail(new popupCallbackFunction<bool,TransformInfoStruct*>( &MapInventoryFilterMenuPopup_OptionOff, NULL ));
	}
	// Add the option to the menu.
	gMapInventoryFilterPopup->addOption( *pOption );


	UINT16 usPosX;
	UINT16 usPosY;

	usPosX = ButtonList[guiMapInvenFilterButton[ 0 ]]->XLoc;
	usPosY = ButtonList[guiMapInvenFilterButton[ 0 ]]->Area.RegionBottomRightY;

	gMapInventoryFilterPopup->setPosition( usPosX, usPosY );

	gfMapInventoryFilterPopupInitialized = TRUE;
	gfMapInventoryFilterPopupVisible = TRUE;
	gfQueueRecreateMapInventoryFilterMenu = FALSE;
	gMapInventoryFilterPopup->show();
}

// HEADROCK HAM 5: This function alters the Map Inventory Filter, by applying the argument as a flag toggle
// (XOR) to the current filter set.
// As a result of the alteration, some items will disappear from view - though they are still kept in 
// memory as "Unseen" items.
void MapInventoryFilterMenuPopup_FilterToggle( UINT32 uiFlags )
{
	// Hide the Filter Menu pop-up.
	if (gMapInventoryFilterPopup != NULL && gfMapInventoryFilterPopupInitialized == TRUE)
	{
		gMapInventoryFilterPopup->hide();
	}
	
	// Alter the filter based on the flags we want. This is a XOR operation, so we're basically
	// toggling the correct bits in the filter on-and-off. The bits correspond to the various
	// item classes - though they are often toggled in groups (i.e. Guns + Launchers, Kits + Medkits + Camokits, etc.)
	guiMapInventoryFilter ^= uiFlags;
	// The refresh function moves the necessary items from the Seen to the Unseen inventories, and vice versa.
	RefreshSeenAndUnseenPools();

	BlitInventoryPoolGraphic( );

	// Toggling these flags back on serves to cause the menu to stay open - yet refresh its options. As a result,
	// we can keep clicking on the various options to see the items appear/disappear, and the options change to indicate
	// this as well.
	gfMapInventoryFilterPopupVisible = TRUE;
	gfQueueRecreateMapInventoryFilterMenu = TRUE;
}

// HEADROCK HAM 5: This function alters the Map Inventory Filter, by applying the argument as a flag set
// (=) to the current filter set.
// As a result of the alteration, some items will disappear from view - though they are still kept in 
// memory as "Unseen" items.
void MapInventoryFilterMenuPopup_FilterSet( UINT32 uiFlags )
{
	// Hide the Filter Menu pop-up.
	if (gMapInventoryFilterPopup != NULL && gfMapInventoryFilterPopupInitialized == TRUE)
	{
		gMapInventoryFilterPopup->hide();
	}
	
	// Alter the filter based on the flags we want. This is a XOR operation, so we're basically
	// toggling the correct bits in the filter on-and-off. The bits correspond to the various
	// item classes - though they are often toggled in groups (i.e. Guns + Launchers, Kits + Medkits + Camokits, etc.)
	guiMapInventoryFilter = uiFlags;
	// The refresh function moves the necessary items from the Seen to the Unseen inventories, and vice versa.
	RefreshSeenAndUnseenPools();

	BlitInventoryPoolGraphic( );

	// Toggling these flags back on serves to cause the menu to stay open - yet refresh its options. As a result,
	// we can keep clicking on the various options to see the items appear/disappear, and the options change to indicate
	// this as well.
	gfMapInventoryFilterPopupVisible = TRUE;
	gfQueueRecreateMapInventoryFilterMenu = TRUE;
}

void MapInventoryFilterMenuPopup_Hide( void )
{
	// When the pop-up goes away, the button that spawned it is popped back to the OFF position.
	ButtonList[ guiMapInvenFilterButton[ 0 ] ]->uiFlags &=~ (BUTTON_CLICKED_ON);

	// Signal the renderer to stop drawing this menu.
	gfMapInventoryFilterPopupVisible = FALSE;
	// Dirty!
	fMapPanelDirty = TRUE;
}

BOOLEAN MapInventoryFilterMenuPopup_OptionOff( void )
{
	// Turns an option off by always returning FALSE
	return FALSE;
}

// HEADROCK HAM 5: Alternative functions that alter the inventory filter without the need for a popup.
void MapInventoryFilterToggle( UINT32 uiFlags )
{
	// Alter the filter based on the flags we want. This is a XOR operation, so we're basically
	// toggling the correct bits in the filter on-and-off. The bits correspond to the various
	// item classes - though they are often toggled in groups (i.e. Guns + Launchers, Kits + Medkits + Camokits, etc.)
	guiMapInventoryFilter ^= uiFlags;
	// The refresh function moves the necessary items from the Seen to the Unseen inventories, and vice versa.
	RefreshSeenAndUnseenPools();

	BlitInventoryPoolGraphic( );

	HandleSetFilterButtons();
}

void MapInventoryFilterSet( UINT32 uiFlags )
{
	if(guiMapInventoryFilter != uiFlags)//dnl ch75 021113
	{
	// Alter the filter based on the flags we want. This is a XOR operation, so we're basically
	// toggling the correct bits in the filter on-and-off. The bits correspond to the various
	// item classes - though they are often toggled in groups (i.e. Guns + Launchers, Kits + Medkits + Camokits, etc.)
	guiMapInventoryFilter = uiFlags;
	// The refresh function moves the necessary items from the Seen to the Unseen inventories, and vice versa.
	RefreshSeenAndUnseenPools();
	}
	BlitInventoryPoolGraphic( );

	HandleSetFilterButtons();
}

// HEADROCK HAM 5: Handle toggling the buttons on and off depending on which filters are shown.
void HandleSetFilterButtons()
{
	// Show/Hide All button always off.
	if ( guiMapInvenFilterButtonDefined[0] )
	{
		ButtonList[guiMapInvenFilterButton[ 0 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
	}

	// Guns
	if ( guiMapInvenFilterButtonDefined[1] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_GUN)
		{
			ButtonList[guiMapInvenFilterButton[ 1 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 1 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Ammo
	if ( guiMapInvenFilterButtonDefined[2] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_AMMO)
		{
			ButtonList[guiMapInvenFilterButton[ 2 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 2 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Explosives
	if ( guiMapInvenFilterButtonDefined[3] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_EXPLOSV)
		{
			ButtonList[guiMapInvenFilterButton[ 3 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 3 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Melee Weapons
	if ( guiMapInvenFilterButtonDefined[4] )
		{
		if (guiMapInventoryFilter & IC_MAPFILTER_MELEE)
		{
			ButtonList[guiMapInvenFilterButton[ 4 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 4 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Armor
	if ( guiMapInvenFilterButtonDefined[5] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_ARMOR)
		{
			ButtonList[guiMapInvenFilterButton[ 5 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 5 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// LBEs
	if ( guiMapInvenFilterButtonDefined[6] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_LBE)
		{
			ButtonList[guiMapInvenFilterButton[ 6 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 6 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Kits
	if ( guiMapInvenFilterButtonDefined[7] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_KIT)
		{
			ButtonList[guiMapInvenFilterButton[ 7 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 7 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Misc. Items
	if ( guiMapInvenFilterButtonDefined[8] )
	{
		if (guiMapInventoryFilter & IC_MAPFILTER_MISC)
		{
			ButtonList[guiMapInvenFilterButton[ 8 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 8 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}

	// Flugente: move item display
	if ( guiMapInvenFilterButtonDefined[9] )
	{
		if ( IsShowMoveItem() )
		{
			ButtonList[guiMapInvenFilterButton[ 9 ]]->uiFlags |= (BUTTON_CLICKED_ON);
		}
		else
		{
			ButtonList[guiMapInvenFilterButton[ 9 ]]->uiFlags &=~ (BUTTON_CLICKED_ON);
		}
	}
}

//Moa:moved code to CreateDestroyMapInventoryPoolButtons() and EnterSector() to decrease loading time
// Flugente: handle various cooldown functions in a sector
/*void SectorInventoryCooldownFunctions( INT16 sMapX, INT16 sMapY, INT16 sMapZ )
{
	UINT32 uiTotalNumberOfRealItems = 0;
	WORLDITEM * pTotalSectorList = NULL;

//	#ifdef _DEBUG
	BOOLEAN fReturn = TRUE;
//	#endif
			
	// now load these items into memory, based on fact if sector is in fact loaded
	if( ( sMapX == gWorldSectorX )&&( gWorldSectorY == sMapY ) && (gbWorldSectorZ == sMapZ ) && guiNumWorldItems)
	{
		HandleSectorCooldownFunctions( sMapX, sMapY, (INT8)sMapZ, gWorldItems, guiNumWorldItems, TRUE );
	}
	else
	{
		// not loaded, load
		// get total number, visable and invisible
		fReturn = GetNumberOfWorldItemsFromTempItemFile( sMapX, sMapY, ( INT8 )( sMapZ ), &( uiTotalNumberOfRealItems ), FALSE );
		Assert( fReturn );

		if( uiTotalNumberOfRealItems > 0 )
		{
			// allocate space for the list
			pTotalSectorList = new WORLDITEM[ uiTotalNumberOfRealItems ];

			if ( !uiTotalNumberOfRealItems )
				return;

			// now load into mem
			LoadWorldItemsFromTempItemFile(  sMapX,  sMapY, ( INT8 ) ( sMapZ ), pTotalSectorList );
		}

		HandleSectorCooldownFunctions( sMapX, sMapY, (INT8)sMapZ, pTotalSectorList, uiTotalNumberOfRealItems, TRUE );

		//Save the Items to the the file
		SaveWorldItemsToTempItemFile( sMapX, sMapY, (INT8)sMapZ, uiTotalNumberOfRealItems, pTotalSectorList );
	}
		
	HandleSectorCooldownFunctions( sMapX, sMapY, (INT8)sMapZ, pTotalSectorList, uiTotalNumberOfRealItems, TRUE );

	//Save the time the player was last in the sector
	SetLastTimePlayerWasInSector( sMapX, sMapY, (INT8)sMapZ );

	//Save the Items to the the file
	SaveWorldItemsToTempItemFile( sMapX, sMapY, (INT8)sMapZ, uiTotalNumberOfRealItems, pTotalSectorList );
}*/

// @brief modifies data of items including, but not limited to: bTemperature (food/weapon overheat)
// Uses various external parameters set in ini file(s) labeled under [Tactical Weapon Overheating Settings], [Tactical Food Settings], [Strategic Gameplay Settings](advanced repair/dirt system)
// to calculate a delta decay and adds it to the itemStack.data
// It is assumed that the itemStack consists of the same usItem.
// Items can not have negative dirt and temperature (0 is used instead).
// @param itemstack: A single item or a itemsstack which will be decayed (dirt, temperature) depending on ini settings.
// @param seconds: Time passed in seconds for which the delta decay is calculated and added to the items in itemstack. Can handle up to +-2147483647/(60*60*24*365) ~ 68 years. This means you can pass in positive seconds to decay the items and negative seconds to undecay them.
// @param naturalDirt: Dirt of the sector, if omitted default = 100 is used.
// @param isUnderground: flag to reduce food decay in underground sectors to 80%, if omitted default = FALSE is used.
// @auth origninal code by flugente
// @auth Moa
void HandleItemCooldownFunctions( OBJECTTYPE* itemStack, INT32 deltaSeconds, BOOLEAN isUnderground )
{
	INT32 tickspassed = deltaSeconds / NUM_SEC_PER_TACTICAL_TURN;//1 tick is 5 seconds

	if ( tickspassed == 0 || !itemStack->exists() || 
		!( gGameExternalOptions.fWeaponOverheating || UsingFoodSystem() ) )
		return;
	

//original code by flugente, renamed variables to fit here, removed "min (OVERHEATING_MAX_TEMPERATURE, newValue)" for dirt to allow to go beyond maximum and deduct later the same amount if neccessary.
	// ... if we use overheating and item is a gun, a launcher or a barrel ...
	if ( gGameExternalOptions.fWeaponOverheating && ( Item[itemStack->usItem].usItemClass & (IC_GUN|IC_LAUNCHER) || ItemIsBarrel(itemStack->usItem) ) )
	{
		for(INT16 i = 0; i < itemStack->ubNumberOfObjects; ++i)			// ... there might be multiple items here (item stack), so for each one ...
		{
			FLOAT guntemperature = (*itemStack)[i]->data.bTemperature;	// ... get temperature ...

			FLOAT cooldownfactor = GetItemCooldownFactor(itemStack);		// ... get item cooldown factor provided of attachments ...

			if (ItemIsBarrel(itemStack->usItem))	// ... a barrel lying around cools down a bit faster ...
				cooldownfactor *= gGameExternalOptions.iCooldownModificatorLonelyBarrel;

			FLOAT newguntemperature = max(0.0f, guntemperature - tickspassed * cooldownfactor );	// ... calculate new temperature ...

#if JA2TESTVERSION
			ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"World: Item temperature lowered from %4.2f to %4.2f", guntemperature, newguntemperature);
#endif

			(*itemStack)[i]->data.bTemperature = newguntemperature;			// ... set new temperature

			// for every objects, we also have to check whether there are weapon attachments (eg. underbarrel weapons), and cool them down too
			attachmentList::iterator iterend = (*itemStack)[i]->attachments.end();
			for (attachmentList::iterator iter = (*itemStack)[i]->attachments.begin(); iter != iterend; ++iter) 
			{
				if ( iter->exists() && Item[ iter->usItem ].usItemClass & (IC_GUN|IC_LAUNCHER) )
				{
					FLOAT temperature =  (*iter)[i]->data.bTemperature;			// ... get temperature of item ...

					FLOAT cooldownfactor = GetItemCooldownFactor( &(*iter) );	// ... get cooldown factor ...

					FLOAT newtemperature = max(0.0f, temperature - tickspassed * cooldownfactor );	// ... calculate new temperature ...

					(*iter)[i]->data.bTemperature = newtemperature;				// ... set new temperature

#if JA2TESTVERSION
					ScreenMsg(FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"World: Item temperature lowered from %4.2f to %4.2f", temperature, newtemperature);
#endif

					// we assume that there can exist only 1 underbarrel weapon per gun
					break;
				}
			}
		}
	}//end overheating

	// ... if it is food and the food system is active ...
	if ( UsingFoodSystem() && Item[ itemStack->usItem ].foodtype > 0 )
	{
		if ( Food[ Item[ itemStack->usItem ].foodtype ].usDecayRate > 0.0f )		// ... if the food can decay...
		{
			// if in undergound sector the food will decay less
			FLOAT sectorModifier = gGameExternalOptions.sFoodDecayModificator * ( isUnderground? 0.8f : 1.0f );

			for( INT16 i = 0; i < itemStack->ubNumberOfObjects; ++i )			// ... there might be multiple items here (item stack), so for each one ...
			{						
				(*itemStack)[i]->data.bTemperature = max( 0.0f, (*itemStack)[i]->data.bTemperature - tickspassed * sectorModifier * Food[ Item[ itemStack->usItem ].foodtype ].usDecayRate );	// set new temperature
			}
		}
	}//end food
}

// Flugente: handle various cooldown functions over an array of items in a specific sector. 
// if fWithMinutes = true, adjust cooldown for time since sector was last entered
// otherwise its used for a turn-precise cooldown
//Moa: code inside pWorldItem loop moved to HandleItemCooldownFunctions, added optional fUndo flag to undecay items (default = FALSE)
void HandleSectorCooldownFunctions( INT16 sMapX, INT16 sMapY, INT8 sMapZ, std::vector<WORLDITEM>& pWorldItem, UINT32 size, BOOLEAN fWithMinutes, BOOLEAN fUndo )//dnl ch75 271013
{
	// if not using overheating or food system, no point in all this
	if ( !gGameExternalOptions.fWeaponOverheating && !UsingFoodSystem() )
		return;

	UINT32 tickspassed = 1;

	if ( fWithMinutes )
	{
		UINT32 usLastVisited = GetLastTimePlayerWasInSector(sMapX, sMapY, sMapZ);

		// if usLastVisited is 0, we have not yet visited this sector. We don't want to cooldown anything in this case (preplaced food could become rotten jsut because we visit this sector late in our campaign)
		if ( usLastVisited == 0 )
			return;

		UINT32 sMinutesPassed = __max(0, GetWorldTotalMin() - usLastVisited );

		if ( sMinutesPassed == 0 )
			return;

		// it is assumed that one turn equals to 5 seconds
		tickspassed = 12*sMinutesPassed;
	}

	FLOAT foofdecaymod = tickspassed * gGameExternalOptions.sFoodDecayModificator;

	// food decays slower if underground
	if ( sMapZ > 0 )
		foofdecaymod *= 0.8f;

	for( UINT32 uiCount = 0; uiCount < size; ++uiCount )				// ... for all items in the world ...
	{
		HandleItemCooldownFunctions( &(pWorldItem[ uiCount ].object), tickspassed * ( fUndo ? -NUM_SEC_PER_TACTICAL_TURN : NUM_SEC_PER_TACTICAL_TURN ), (sMapZ > 0) );
	}
}



void HandleMousePageScroll(void)
{
	SGPPoint	MousePos;
	GetMousePos(&MousePos);
	const auto x = MousePos.iX;
	const auto y = MousePos.iY;
	if ( (UI_MAP.BorderRegion.x < x && x < (UI_MAP.BorderRegion.x + UI_MAP.BorderRegion.width)) && (UI_MAP.BorderRegion.y < y && y < (UI_MAP.BorderRegion.y + UI_MAP.BorderRegion.height)) )
	{
		const auto Wheelstate = _WheelValue * (gGameSettings.fOptions[TOPTION_INVERT_WHEEL] ? -1 : 1);
		if (Wheelstate < 0)
		{
			MapInvNextPage();
		}
		else if (Wheelstate > 0)
		{
			MapInvPreviousPage();
		}
		_WheelValue = 0;
	}
}

void MapInvNextPage(void)
{
	// if can go to next page, go there
	if (iCurrentInventoryPoolPage < iLastInventoryPoolPage)
	{
		iCurrentInventoryPoolPage++;
		//shadooow: this will re-render item compatibility highlighting when changing pages for currently held item
		if (gpItemPointer != NULL)
		{
			fChangedInventorySlots = TRUE;
			gfCheckForCursorOverMapSectorInventoryItem = TRUE;
		}
		fMapPanelDirty = TRUE;
	}
}

void MapInvPreviousPage(void)
{
	// if can go to next page, go there
	if (iCurrentInventoryPoolPage > 0)
	{
		iCurrentInventoryPoolPage--;
		//shadooow: this will re-render item compatibility highlighting when changing pages for currently held item
		if (gpItemPointer != NULL)
		{
			fChangedInventorySlots = TRUE;
			gfCheckForCursorOverMapSectorInventoryItem = TRUE;
		}
		fMapPanelDirty = TRUE;
	}
}


void InitMapInventoryCoordinates(void)
{
	if (isWidescreenUI())
	{
		MAPINV_NEXT_X = UI_MAP.BorderRegion.x + 679;
		MAPINV_PREV_X = UI_MAP.BorderRegion.x + 607;
		MAPINV_ARROW_Y = UI_MAP.BorderRegion.y + 19;

		MAPINV_ZOOM_X = UI_MAP.BorderRegion.x + 10;
		MAPINV_ZOOM_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_STACK_X = UI_MAP.BorderRegion.x + 50;
		MAPINV_STACK_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_SORT_AMMO_X = UI_MAP.BorderRegion.x + 80;
		MAPINV_SORT_AMMO_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_SORT_ATT_X = UI_MAP.BorderRegion.x + 110;
		MAPINV_SORT_ATT_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_EJECT_AMMO_X = UI_MAP.BorderRegion.x + 140;
		MAPINV_EJECT_AMMO_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_X = UI_MAP.BorderRegion.x + 185;
		MAPINV_FILTER_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_GUN_X = UI_MAP.BorderRegion.x + 214;
		MAPINV_FILTER_GUN_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_AMMO_X = UI_MAP.BorderRegion.x + 214;
		MAPINV_FILTER_AMMO_Y = UI_MAP.BorderRegion.y + 24;

		MAPINV_FILTER_EXPLOSIVE_X = UI_MAP.BorderRegion.x + 243;
		MAPINV_FILTER_EXPLOSIVE_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_MELEE_X = UI_MAP.BorderRegion.x + 243;
		MAPINV_FILTER_MELEE_Y = UI_MAP.BorderRegion.y + 24;

		MAPINV_FILTER_ARMOR_X = UI_MAP.BorderRegion.x + 272;
		MAPINV_FILTER_ARMOR_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_LBE_X = UI_MAP.BorderRegion.x + 272;
		MAPINV_FILTER_LBE_Y = UI_MAP.BorderRegion.y + 24;

		MAPINV_FILTER_KIT_X = UI_MAP.BorderRegion.x + 301;
		MAPINV_FILTER_KIT_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_MISC1_X = UI_MAP.BorderRegion.x + 301;
		MAPINV_FILTER_MISC1_Y = UI_MAP.BorderRegion.y + 24;

		MAPINV_FILTER_MISC2_X = UI_MAP.BorderRegion.x + 336;
		MAPINV_FILTER_MISC2_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_SAVE_TEMPLATE_X = UI_MAP.BorderRegion.x + 371;
		MAPINV_FILTER_SAVE_TEMPLATE_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_FILTER_LOAD_TEMPLATE_X = UI_MAP.BorderRegion.x + 406;
		MAPINV_FILTER_LOAD_TEMPLATE_Y = UI_MAP.BorderRegion.y + 10;

		MAPINV_DONE_X = UI_MAP.BorderRegion.x + 706;
		MAPINV_DONE_y = UI_MAP.BorderRegion.y + 15;

		MAPINV_ITEMPOOL_X = UI_MAP.BorderRegion.x + 558;
		MAPINV_ITEMPOOL_Y = UI_MAP.BorderRegion.y + 19;

		MAPINV_PAGE_X = UI_MAP.BorderRegion.x + 626;
		MAPINV_PAGE_Y = UI_MAP.BorderRegion.y + 19;

		MAPINV_SECTOR_X = UI_MAP.BorderRegion.x + 447;
		MAPINV_SECTOR_Y = UI_MAP.BorderRegion.y + 19;

		MAPINV_LOC_STR_X = MAPINV_SECTOR_X - 19;
		MAPINV_LOC_STR_Y = UI_MAP.BorderRegion.y + 15;

		MAPINV_ITEMPOOL_STR_X = MAPINV_ITEMPOOL_X - 19;
		MAPINV_ITEMPOOL_STR_Y = UI_MAP.BorderRegion.y + 15;
	}
	else
	{
		MAPINV_NEXT_X = MAP_INV_X_OFFSET + 559;
		MAPINV_PREV_X = MAP_INV_X_OFFSET + 487;
		MAPINV_ARROW_Y = SCREEN_HEIGHT - 144 - yResOffset;

		MAPINV_ZOOM_X = INVEN_POOL_X + 10 + xResOffset;
		MAPINV_ZOOM_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_STACK_X = INVEN_POOL_X + 50 + xResOffset;
		MAPINV_STACK_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_SORT_AMMO_X = INVEN_POOL_X + 80 + xResOffset;
		MAPINV_SORT_AMMO_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_SORT_ATT_X = INVEN_POOL_X + 110 + xResOffset;
		MAPINV_SORT_ATT_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_EJECT_AMMO_X = INVEN_POOL_X + 140 + xResOffset;
		MAPINV_EJECT_AMMO_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_X = INVEN_POOL_X + 185 + xResOffset;
		MAPINV_FILTER_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_GUN_X = INVEN_POOL_X + 214 + xResOffset;
		MAPINV_FILTER_GUN_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_AMMO_X = INVEN_POOL_X + 214 + xResOffset;
		MAPINV_FILTER_AMMO_Y = INVEN_POOL_Y + 24 + yResOffset;

		MAPINV_FILTER_EXPLOSIVE_X = INVEN_POOL_X + 243 + xResOffset;
		MAPINV_FILTER_EXPLOSIVE_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_MELEE_X = INVEN_POOL_X + 243 + xResOffset;
		MAPINV_FILTER_MELEE_Y = INVEN_POOL_Y + 24 + yResOffset;

		MAPINV_FILTER_ARMOR_X = INVEN_POOL_X + 272 + xResOffset;
		MAPINV_FILTER_ARMOR_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_LBE_X = INVEN_POOL_X + 272 + xResOffset;
		MAPINV_FILTER_LBE_Y = INVEN_POOL_Y + 24 + yResOffset;

		MAPINV_FILTER_KIT_X = INVEN_POOL_X + 301 + xResOffset;
		MAPINV_FILTER_KIT_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_MISC1_X = INVEN_POOL_X + 301 + xResOffset;
		MAPINV_FILTER_MISC1_Y = INVEN_POOL_Y + 24 + yResOffset;

		MAPINV_FILTER_MISC2_X = INVEN_POOL_X + 336 + xResOffset;
		MAPINV_FILTER_MISC2_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_SAVE_TEMPLATE_X = INVEN_POOL_X + 371 + xResOffset;
		MAPINV_FILTER_SAVE_TEMPLATE_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_FILTER_LOAD_TEMPLATE_X = INVEN_POOL_X + 406 + xResOffset;
		MAPINV_FILTER_LOAD_TEMPLATE_Y = INVEN_POOL_Y + 10 + yResOffset;

		MAPINV_DONE_X = MAP_INV_X_OFFSET + 587;
		MAPINV_DONE_y = yResSize - 147 + yResOffset;

		MAPINV_ITEMPOOL_X = MAP_INVENTORY_POOL_NUMBER_X;
		MAPINV_ITEMPOOL_Y = MAP_INVENTORY_POOL_PAGE_Y - yResOffset;

		MAPINV_PAGE_X = MAP_INVENTORY_POOL_PAGE_X;
		MAPINV_PAGE_Y = MAP_INVENTORY_POOL_PAGE_Y - yResOffset;

		MAPINV_SECTOR_X = MAP_INVENTORY_POOL_LOC_X;
		MAPINV_SECTOR_Y = MAP_INVENTORY_POOL_PAGE_Y - yResOffset;

		MAPINV_LOC_STR_X = MAP_INV_X_OFFSET + 268;
		MAPINV_LOC_STR_Y = (SCREEN_HEIGHT - 138 - yResOffset);

		MAPINV_ITEMPOOL_STR_X = MAP_INV_X_OFFSET + 369;
		MAPINV_ITEMPOOL_STR_Y = (SCREEN_HEIGHT - 138 - yResOffset);
	}
}
