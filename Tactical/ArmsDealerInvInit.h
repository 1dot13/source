#ifndef _ARMS_DEALER_INV_INIT__H_
#define	_ARMS_DEALER_INV_INIT__H_

#include "Item Types.h"
#include "Arms Dealer Init.h"

#define		LAST_DEALER_ITEM					0//-1
#define		NO_DEALER_ITEM						0

// item suitability categories for dealer inventory initialization, virtual customer sales, and re-ordering
#define	ITEM_SUITABILITY_NONE			0
#define ITEM_SUITABILITY_LOW			1
#define ITEM_SUITABILITY_MEDIUM		2
#define ITEM_SUITABILITY_HIGH			3
#define ITEM_SUITABILITY_ALWAYS		4

#define DEALER_BUYING			0
#define DEALER_SELLING		1



typedef struct
{
	INT16		sItemIndex;
	UINT8		ubOptimalNumber;
	UINT32		uiIndex;

} DEALER_POSSIBLE_INV;


typedef struct
{
	UINT32		uiItemClass;
	UINT8			ubWeaponClass;
	BOOLEAN		fAllowUsed;
} ITEM_SORT_ENTRY;



INT8	GetDealersMaxItemAmount( UINT8 ubDealerID, UINT16 usItemIndex );

DEALER_POSSIBLE_INV *GetPointerToDealersPossibleInventory( UINT16 ubArmsDealerID );

UINT8 GetCurrentSuitabilityForItem( INT8 bArmsDealer, UINT16 usItemIndex );
UINT8 ChanceOfItemTransaction( INT8 bArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerSelling, BOOLEAN fUsed );
BOOLEAN ItemTransactionOccurs( INT8 bArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerSelling, BOOLEAN fUsed );
UINT8 DetermineInitialInvItems( INT8 bArmsDealerID, UINT16 usItemIndex, UINT8 ubChances, BOOLEAN fUsed);
UINT8 HowManyItemsAreSold( INT8 bArmsDealerID, UINT16 usItemIndex, UINT8 ubNumInStock, BOOLEAN fUsed);
UINT8 HowManyItemsToReorder(UINT8 ubWanted, UINT8 ubStillHave);

int BobbyRayItemQsortCompare(const void *pArg1, const void *pArg2);
int CompareItemsForSorting( UINT16 usItem1Index, UINT16 usItem2Index, UINT16 ubItem1Quality, UINT16 ubItem2Quality );
UINT8 GetDealerItemCategoryNumber( UINT16 usItemIndex );
BOOLEAN CanDealerItemBeSoldUsed( UINT16 usItemIndex );

extern DEALER_POSSIBLE_INV gTonyInventory[MAXITEMS+1]; 

#ifdef JA2UB
extern DEALER_POSSIBLE_INV gBettyInventory[MAXITEMS+1];
#else  //ja25 ub Biggins
extern DEALER_POSSIBLE_INV gDevinInventory[MAXITEMS+1];
#endif

extern DEALER_POSSIBLE_INV gFranzInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gKeithInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gSamInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gJakeInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gHowardInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gGabbyInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gFrankInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gElginInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gMannyInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gHerveInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gPeterInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gAlbertoInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gCarloInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gMickyInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gArnieInventory[MAXITEMS+1];
#ifdef JA2UB
extern DEALER_POSSIBLE_INV gRaulInventory[MAXITEMS+1];
#else
extern DEALER_POSSIBLE_INV gPerkoInventory[MAXITEMS+1];
#endif
extern DEALER_POSSIBLE_INV gFredoInventory[MAXITEMS+1];

// added by Flugente 2012-12-19
extern DEALER_POSSIBLE_INV gTinaInventory[MAXITEMS+1];
extern DEALER_POSSIBLE_INV gArmsDealerAdditional[ADDITIONAL_ARMS_DEALERS][MAXITEMS+1];

#endif
