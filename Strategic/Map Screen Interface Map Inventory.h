#ifndef _MAP_INTERFACE_MAP_INVEN_H
#define _MAP_INTERFACE_MAP_INVEN_H

#include "types.h"
#include "World Items.h"

// this is how close one has to be in the loaded sector to pickup an item
#define MAX_DISTANCE_TO_PICKUP_ITEM 5

// Flugente: number of map inventory filter buttons
#define	MAP_INVENTORY_FILTER_BUTTONS	12	// Flugente: 9 -> 12

// number of inventory slots
//#define MAP_INVENTORY_POOL_SLOT_COUNT				84	//45	//45

// whether we are showing the inventory pool graphic
extern BOOLEAN fShowMapInventoryPool;
// HEADROCK HAM 5: Flag telling us whether we've already redrawn the screen to show
// sector inventory sale prices.
extern UINT8 gubRenderedMapInventorySalePrices;

// load inventory pool graphic
BOOLEAN LoadInventoryPoolGraphic( void );

// remove inventory pool graphic
void RemoveInventoryPoolGraphic( void );

// blit the inventory graphic
void BlitInventoryPoolGraphic( void );
// HEADROCK HAM 5: Blit map inventory slots
void BlitInventoryPoolSlotGraphics( void );

// which buttons in map invneotyr panel?
void HandleButtonStatesWhileMapInventoryActive( void );

// handle creation and destruction of map inventory pool buttons
void CreateDestroyMapInventoryPoolButtons( BOOLEAN fExitFromMapScreen );

// bail out of sector inventory mode if it is on
void CancelSectorInventoryDisplayIfOn( BOOLEAN fExitFromMapScreen );

// get total number of items in sector
INT32 GetTotalNumberOfItems( void );

// handle flash of inventory items
void HandleFlashForHighLightedItem( void );

// the list for the inventory
extern std::vector<WORLDITEM> pInventoryPoolList;

// autoplace down object
BOOLEAN AutoPlaceObjectInInventoryStash( OBJECTTYPE *pItemPtr, INT32 sGridNo=0, INT8 ubLevel=0 );

// the current inventory item
extern INT32 iCurrentlyHighLightedItem;
extern BOOLEAN fFlashHighLightInventoryItemOnradarMap;
extern INT32 sObjectSourceGridNo;
extern INT32 iCurrentInventoryPoolPage;
extern INT32 iLastInventoryPoolPage;
extern BOOLEAN fMapInventoryItemCompatable[ ];
// anv: extern'd this for use in Interface Enhanced.cpp
extern BOOLEAN gfCheckForCursorOverMapSectorInventoryItem;
// HEADROCK HAM 5: Same idea as above, this flags items as being candidates for appearing in Zoomed mode.
extern BOOLEAN gfMapInventoryItemToZoom[ ];
extern INT32 MAP_INVENTORY_POOL_SLOT_COUNT; 

BOOLEAN IsMapScreenWorldItemInvisibleInMapInventory( WORLDITEM *pWorldItem );
BOOLEAN IsMapScreenWorldItemVisibleInMapInventory( WORLDITEM *pWorldItem );

//dnl ch51 081009
#define INVPOOLLISTNUM 10
BOOLEAN	CopySectorInventoryToInventoryPoolQ(UINT8 idx);
BOOLEAN CopySectorInventoryToInventoryPoolQs(UINT8 idx);
BOOLEAN	DisplaySectorItemsInfo(void);
BOOLEAN	LoadInventoryPoolQ(UINT8 ubSaveGameID);
BOOLEAN	MemFreeInventoryPoolQ(void);
BOOLEAN	SaveInventoryPoolQ(UINT8 ubSaveGameID);
BOOLEAN	SortInventoryPoolQ(void);
BOOLEAN	SwitchToInventoryPoolQ(UINT8 newidx);

#endif

// Flugente: handle various cooldown functions in a sector
//void SectorInventoryCooldownFunctions( INT16 sMapX, INT16 sMapY, INT16 sMapZ );

// Flugente: handle various cooldown functions over an array of items in a specific sector. 
// if fWithMinutes = true, adjust cooldown for time since sector was last entered
// otherwise its used for a turn-precise cooldown
void HandleSectorCooldownFunctions( INT16 sMapX, INT16 sMapY, INT8 sMapZ, std::vector<WORLDITEM>& pWorldItem, UINT32 size, BOOLEAN fWithMinutes, BOOLEAN fUndo = FALSE);//dnl ch75 271013
void HandleItemCooldownFunctions( OBJECTTYPE* itemStack, INT32 deltaSeconds,  BOOLEAN isUnderground = FALSE);
