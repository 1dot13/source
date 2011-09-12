#ifndef _MAP_INTERFACE_MAP_INVEN_H
#define _MAP_INTERFACE_MAP_INVEN_H

#include "types.h"
#include "World Items.h"

// this is how close one has to be in the loaded sector to pickup an item
#define MAX_DISTANCE_TO_PICKUP_ITEM 5

// number of inventory slots
//#define MAP_INVENTORY_POOL_SLOT_COUNT				84	//45	//45

// whether we are showing the inventory pool graphic
extern BOOLEAN fShowMapInventoryPool;

// load inventory pool graphic
BOOLEAN LoadInventoryPoolGraphic( void );

// remove inventory pool graphic
void RemoveInventoryPoolGraphic( void );

// blit the inventory graphic
void BlitInventoryPoolGraphic( void );

// which buttons in map invneotyr panel?
void HandleButtonStatesWhileMapInventoryActive( void );

// handle creation and destruction of map inventory pool buttons
void CreateDestroyMapInventoryPoolButtons( BOOLEAN fExitFromMapScreen );

// bail out of sector inventory mode if it is on
void CancelSectorInventoryDisplayIfOn( BOOLEAN fExitFromMapScreen );

INT32 GetSizeOfStashInSector( INT16 sMapX, INT16 sMapY, INT16 sMapZ, BOOLEAN fCountStacksAsOne );

// get total number of items in sector
INT32 GetTotalNumberOfItems( void );

// handle flash of inventory items
void HandleFlashForHighLightedItem( void );

// the list for the inventory
extern std::vector<WORLDITEM> pInventoryPoolList;

// autoplace down object
BOOLEAN AutoPlaceObjectInInventoryStash( OBJECTTYPE *pItemPtr, INT32 sGridNo=0 );

// the current inventory item
extern INT32 iCurrentlyHighLightedItem;
extern BOOLEAN fFlashHighLightInventoryItemOnradarMap;
extern INT32 sObjectSourceGridNo;
extern INT32 iCurrentInventoryPoolPage;
extern INT32 iLastInventoryPoolPage;
extern BOOLEAN fMapInventoryItemCompatable[ ];
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