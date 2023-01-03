#ifndef __STORE_INVENTORY_H_
#define __STORE_INVENTORY_H_

#include "Items.h"


typedef struct
{
	UINT16	usItemIndex;								//Index into the item table
	UINT8		ubQtyOnHand;	
	UINT8		ubQtyOnOrder;								//The number of items on order
	UINT8		ubItemQuality;							// the % damaged listed from 0 to 100
	BOOLEAN fPreviouslyEligible;				// whether or not dealer has been eligible to sell this item in days prior to today

	UINT8		filler;

} STORE_INVENTORY;


//Enums used for the access the MAX dealers array
enum
{
	BOBBY_RAY_NEW,
	BOBBY_RAY_USED,

	BOBBY_RAY_LISTS,
};

extern	UINT8 StoreInventory[MAXITEMS][BOBBY_RAY_LISTS]; 
extern	INT16 WeaponROF[ MAXITEMS ]; 



void SetupStoreInventory( STORE_INVENTORY *pInventoryArray, BOOLEAN fUsed );
BOOLEAN DoesGunOfSameClassExistInInventory( UINT8 ubItemIndex, UINT8 ubDealerID );
STORE_INVENTORY *GetPtrToStoreInventory( UINT8 ubDealerID );
//INT16	CountNumberOfItemsInStoreInventory( UINT8 ubArmsDealerID );

#endif
