	#include "Types.h"
	#include "stdlib.h"
	#include "Arms Dealer Init.h"
	#include "String.h"
	#include "Debug.h"
	#include "Random.h"
	#include "Weapons.h"
	#include "FileMan.h"
	#include "Game Clock.h"
	#include "ArmsDealerInvInit.h"
	#include "Message.h"
	#include "soldier profile.h"
	#include "Handle Items.h"
	#include "Quests.h"
	#include "Scheduling.h"
	#include "GameSettings.h"
	#include "Overhead.h"	// added by Flugente for MercPtrs[]
	#include "LuaInitNPCs.h"	// added by Flugente

#ifdef JA2UB
#include "Explosion Control.h"
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "MapScreen Quotes.h"
#include "email.h"
#include "interface Dialogue.h"
#include "ub_config.h"
#endif

#include <list>

#define	MIN_REPAIR_TIME_IN_MINUTES				15		// minutes
#define MIN_REPAIR_COST										10		// dollars

// price classes
#define PRICE_CLASS_JUNK									0
#define PRICE_CLASS_CHEAP									1
#define PRICE_CLASS_EXPENSIVE							2

void ConvertCreatureBloodToElixir( void );

// Flugente: if we are a lua-based merchant, stop selling items we didn't explicitly want to be sold
void RemoveNonIntelItems();

UINT8 gubLastSpecialItemAddedAtElement = 255;

// Flugente 2012-12-19: merchant data has been externalised - see XML_Merchants.cpp

std::vector<ARMS_DEALER_INFO>	armsDealerInfo (NUM_ARMS_DEALERS);


// THESE GET SAVED/RESTORED/RESET
ARMS_DEALER_STATUS	gArmsDealerStatus[ NUM_ARMS_DEALERS ];
std::vector<DealerItemList>	gArmsDealersInventory;




void		AddAmmoToArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT16 ubShotsLeft );
void		AddItemToArmsDealerInventory( UINT8 ubArmsDealer, OBJECTTYPE& object );

void		RemoveRandomItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany );

void		DailyCheckOnItemQuantities();
void		SimulateArmsDealerCustomer();

BOOLEAN AdjustCertainDealersInventory();
void		LimitArmsDealersInventory( UINT8 ubArmsDealer, UINT32 uDealerItemType, UINT8 ubMaxNumberOfItemType );
void		GuaranteeAtLeastOneItemOfType( UINT8 ubArmsDealer, UINT32 uiDealerItemType );
void		GuaranteeAtLeastXItemsOfIndex( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany );
void		GuaranteeAtMostNumOfItemsForItem( UINT8 ubArmsDealer, INT16 sItemIndex, UINT8 ubAtMostNumItems );

void		ArmsDealerGetsFreshStock( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubNumItems );
BOOLEAN ItemContainsLiquid( UINT16 usItemIndex );
UINT8		DetermineDealerItemCondition( UINT8 ubArmsDealer, UINT16 usItemIndex );

BOOLEAN DoesItemAppearInDealerInventoryList( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer );

#ifdef JA2UB
UINT8	GetFirstValidSpecialItemFromDealer( UINT8 ubArmsDealer, INT16 sItemIndex );

void AddTexsVideosToBettysInventory();
BOOLEAN CanThisItemBeSoldToSimulatedCustomer( UINT8 ubArmsDealerID, UINT16 usItemIndex );
#endif

void GuaranteeMinimumAlcohol( UINT8 ubArmsDealer );

BOOLEAN ItemIsARocketRifle( INT16 sItemIndex );

BOOLEAN GetArmsDealerShopHours( UINT8 ubArmsDealer, UINT32 *puiOpeningTime, UINT32 *puiClosingTime );

ARMS_DEALER_STATUS& ARMS_DEALER_STATUS::operator=(const OLD_ARMS_DEALER_STATUS_101& status)
{
	this->initialize();
	this->fOutOfBusiness = status.fOutOfBusiness;
	this->fRepairDelayBeenUsed = status.fRepairDelayBeenUsed;
	this->fUnusedKnowsPlayer = status.fUnusedKnowsPlayer;
	this->ubSpecificDealerFlags = status.ubSpecificDealerFlags;
	this->uiArmsDealersCash = status.uiArmsDealersCash;
	this->uiTimePlayerLastInSKI = status.uiTimePlayerLastInSKI;
	return *this;
}

void ARMS_DEALER_STATUS::initialize()
{
	memset(this, 0, sizeof(ARMS_DEALER_STATUS));
}

void DEALER_SPECIAL_ITEM::initialize()
{
	memset(this, 0, SIZEOF_DEALER_SPECIAL_ITEM_POD);
	object.initialize();
}

bool DEALER_SPECIAL_ITEM::operator ==(const DEALER_SPECIAL_ITEM& compare)
{
	if (this->bItemCondition == compare.bItemCondition &&
		this->ubOwnerProfileId == compare.ubOwnerProfileId &&
		this->ubQtyOnOrder == compare.ubQtyOnOrder &&
		this->uiOrderArrivalTime == compare.uiOrderArrivalTime &&
		this->uiRepairDoneTime == compare.uiRepairDoneTime) {

		if (this->object == compare.object) {
			return true;
		}
	}
	return false;
}

void InitAllArmsDealers()
{
	ShutDownArmsDealers();

	//Initialize the initial status & inventory for each of the arms dealers
	for( UINT8 ubArmsDealer = 0; ubArmsDealer < NUM_ARMS_DEALERS; ++ubArmsDealer )
	{
		InitializeOneArmsDealer( ubArmsDealer );
	}

	//make sure certain items are in stock and certain limits are respected
	AdjustCertainDealersInventory( );
}


void InitializeOneArmsDealer( UINT8 ubArmsDealer )
{
	UINT16	usItemIndex;
	UINT8	ubNumItems=0;

	memset( &( gArmsDealerStatus[ ubArmsDealer ] ), 0, sizeof( ARMS_DEALER_STATUS ) );
	gArmsDealersInventory.resize(gArmsDealersInventory.size() + 1);

	//Reset the arms dealers cash on hand to the default initial value
	gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash = armsDealerInfo[ ubArmsDealer ].iInitialCash;

	//if the arms dealer isn't supposed to have any items (includes all repairmen)
	if( armsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_HAS_NO_INVENTORY )
	{
		return;
	}

	//loop through all the item types
	for ( usItemIndex = 1; usItemIndex < gMAXITEMS_READ; ++usItemIndex )
	{
		if ( Item[usItemIndex].usItemClass == 0 )
			break;

		//Can the item be sold by the arms dealer
		if( CanDealerTransactItem( ubArmsDealer, usItemIndex, FALSE ) )
		{
			//Setup an initial amount for the items (treat items as new, how many are used isn't known yet)
			ubNumItems = DetermineInitialInvItems( ubArmsDealer, usItemIndex, GetDealersMaxItemAmount( ubArmsDealer, usItemIndex ), FALSE );

			//if there are any initial items
			if( ubNumItems > 0 )
			{
				ArmsDealerGetsFreshStock( ubArmsDealer, usItemIndex, ubNumItems );
			}
		}
	}
}

void ShutDownArmsDealers()
{
	gArmsDealersInventory.clear();
}

typedef struct
{
	UINT16		usItem;
	INT16		sIntelPrice;
	INT16		sOptimalNumber;

} ARMS_DEALER_ITEM_INTEL;

std::map<UINT16, std::vector<ARMS_DEALER_ITEM_INTEL> >	gArmsDealerAdditionalIntelData;

void AddArmsDealerAdditionalIntelData( UINT16 ausDealer, UINT16 usItem, INT16 sIntelPrice, INT16 sOptimalNumber )
{
	ARMS_DEALER_ITEM_INTEL data;
	data.usItem = usItem;
	data.sIntelPrice = sIntelPrice;
	data.sOptimalNumber = sOptimalNumber;
	
	gArmsDealerAdditionalIntelData[ausDealer].push_back( data );
}

void ArmsDealers_ReadIntelData()
{
	gArmsDealerAdditionalIntelData.clear();

	// ask lua on what to use
	LuaAddArmsDealerAdditionalIntelData();
}

void HandlePossibleArmsDealerIntelRefresh( BOOLEAN aForceReread )
{
	if ( gArmsDealerAdditionalIntelData.empty() || aForceReread )
	{
		ArmsDealers_ReadIntelData();

		// replace xml-based possible inventory with what lua gave us
		for ( std::map<UINT16, std::vector<ARMS_DEALER_ITEM_INTEL> >::iterator it = gArmsDealerAdditionalIntelData.begin(); it != gArmsDealerAdditionalIntelData.end(); ++it)
		{
			UINT16 dealerid = ( *it ).first;

			DEALER_POSSIBLE_INV* pDealerInv = GetPointerToDealersPossibleInventory( dealerid );

			memset( pDealerInv, 0, sizeof( DEALER_POSSIBLE_INV )*MAXITEMS );

			int cnt = 0;
			for ( std::vector<ARMS_DEALER_ITEM_INTEL>::iterator it2 = (*it).second.begin();  it2 != ( *it ).second.end(); ++it2 )
			{
				ARMS_DEALER_ITEM_INTEL data = ( *it2 );

				pDealerInv[cnt].uiIndex = cnt;
				pDealerInv[cnt].sItemIndex = data.usItem;
				pDealerInv[cnt].ubOptimalNumber = (UINT8)data.sOptimalNumber;

				++cnt;
			}
		}
	}
}

BOOLEAN SaveArmsDealerInventoryToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;
	UINT8		ubArmsDealer;

	//save the number of dealers to load
	int dealers = NUM_ARMS_DEALERS;
	if (!FileWrite( hFile, &dealers, sizeof( int ), &uiNumBytesWritten ))
	{
		return( FALSE );
	}

	//Save the arms dealers status
	if (!FileWrite( hFile, gArmsDealerStatus, sizeof( gArmsDealerStatus ), &uiNumBytesWritten ))
	{
		return( FALSE );
	}

	//loop through all the dealers inventories
	for( ubArmsDealer=0; ubArmsDealer<NUM_ARMS_DEALERS; ++ubArmsDealer )
	{
		int size = gArmsDealersInventory.empty() ? 0 : gArmsDealersInventory[ubArmsDealer].size();
		if (!FileWrite( hFile, &size, sizeof( int ), &uiNumBytesWritten ))
		{
			return( FALSE );
		}

		if(gArmsDealersInventory.empty())
			continue;

		//loop through this dealer's individual items
		for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin(); iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter)
		{
			if (! iter->Save(hFile) )
			{
				return FALSE;
			}
		}
	}

	return( TRUE );
}

void DEALER_SPECIAL_ITEM::ConvertFrom101(OLD_DEALER_ITEM_HEADER_101& header, OLD_DEALER_SPECIAL_ITEM_101& special, int ubArmsDealer, int usItemIndex)
{
	this->bItemCondition = special.oldInfo.bItemCondition;
	if (this->bItemCondition == 0) {
		return;
	}

	this->uiRepairDoneTime = special.uiRepairDoneTime;
	this->ubOwnerProfileId = special.ubOwnerProfileId;

	this->uiOrderArrivalTime = 0;
	this->ubQtyOnOrder = 0;

	int condition = this->bItemCondition;
	if (this->IsUnderRepair()) {
		condition *= -1;
	}

	CreateObjectForDealer(usItemIndex, condition, 1, &(this->object));
	this->object[0]->data.ubImprintID = special.oldInfo.ubImprintID;
	for (int x = 0; x < OLD_MAX_ATTACHMENTS_101; ++x) {
		if (special.oldInfo.usAttachment[x] && special.oldInfo.bAttachmentStatus[x] ) {
			CreateObjectForDealer(special.oldInfo.usAttachment[x], special.oldInfo.bAttachmentStatus[x], 1, &gTempObject);
			this->object.AttachObject(0, &gTempObject, FALSE);
		}
	}
	gArmsDealersInventory[ubArmsDealer].push_back(*this);
};

void DailyUpdateOfArmsDealersInventory()
{
	// if Gabby has creature blood, start turning it into extra elixir
	ConvertCreatureBloodToElixir();
	
	//Simulate other customers buying inventory from the dealer
	SimulateArmsDealerCustomer();

	//if there are some items that are out of stock, order some more
#ifdef JA2UB
	DailyCheckOnItemQuantities( FALSE );
#else
	DailyCheckOnItemQuantities();
#endif

	// Flugente: if we are a lua-based merchant, stop selling items we didn't explicitly want to be sold
	RemoveNonIntelItems();

	//make sure certain items are in stock and certain limits are respected
	AdjustCertainDealersInventory( );
}

// Once a day, loop through each dealer's inventory items and possibly sell some
void SimulateArmsDealerCustomer()
{
	UINT8	ubItemsSold=0;

	static int numPerfectItems[MAXITEMS];

	//loop through all the arms dealers
	for( UINT8 ubArmsDealer=0;ubArmsDealer<NUM_ARMS_DEALERS; ++ubArmsDealer )
	{
		if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
			continue;

		//if the arms dealer isn't supposed to have any items (includes all repairmen)
		if( armsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_HAS_NO_INVENTORY )
			continue;

		memset (&numPerfectItems, 0, sizeof(numPerfectItems));

		DealerItemList::iterator iterend = gArmsDealersInventory[ubArmsDealer].end( );
		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin(); iter != iterend; ++iter )
		{
			if (iter->ItemIsInInventory() == true && iter->bItemCondition == 100)
			{
				numPerfectItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
			}
		}

		//loop through all items of the same type
		for ( UINT16 usItemIndex = 1; usItemIndex < gMAXITEMS_READ; ++usItemIndex )
		{
			if ( Item[usItemIndex].usItemClass	== 0 )
				break;
#ifdef JA2UB			
			//JA25 UB//			
			if( !CanThisItemBeSoldToSimulatedCustomer( ubArmsDealer, usItemIndex ) )
			{
				continue;
			}
#endif			
			//if there are some of these in stock
			if( numPerfectItems[usItemIndex] > 0)
			{
				// first, try to sell all the new (perfect) ones
				if ( usItemIndex == JAR_ELIXIR )
				{
					// only allow selling of standard # of items so those converted from blood given by player will be available
					ubItemsSold = HowManyItemsAreSold( ubArmsDealer, usItemIndex, (UINT8) __min( 3, numPerfectItems[usItemIndex]), FALSE);
				}
				else
				{
					ubItemsSold = HowManyItemsAreSold( ubArmsDealer, usItemIndex, numPerfectItems[usItemIndex], FALSE);
				}

				if ( ubItemsSold > 0)
				{
					RemoveItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, ubItemsSold);
				}
			}
		}

		iterend = gArmsDealersInventory[ubArmsDealer].end();
		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin(); iter != iterend; ++iter )
		{
			// next, try to sell all the used ones, gotta do these one at a time so we can remove them by element
			// don't worry about negative condition, repairmen can't come this far, they don't sell!
			if (iter->bItemCondition < 100 && iter->bItemCondition > 0)
			{
				if ( iter->ItemIsInInventory() == true)
				{
					// try selling just this one
					if (HowManyItemsAreSold( ubArmsDealer, iter->object.usItem, 1, TRUE) > 0)
					{
						iter = gArmsDealersInventory[ ubArmsDealer ].erase(iter);
						if (iter == gArmsDealersInventory[ ubArmsDealer ].end() )
							break;
					}
				}
			}
		}
	}
}

#ifdef JA2UB
void DailyCheckOnItemQuantities( BOOLEAN fInstallyHaveItemsAppear )
#else
void DailyCheckOnItemQuantities()
#endif
{
	UINT8		ubArmsDealer;
	UINT16	usItemIndex;
	UINT8		ubMaxSupply;
	UINT8		ubNumItems;
	UINT32	uiArrivalDay;
	UINT8		ubReorderDays;
	
	HandlePossibleArmsDealerIntelRefresh(TRUE);
	
	//loop through all the arms dealers
	for( ubArmsDealer=0;ubArmsDealer<NUM_ARMS_DEALERS; ++ubArmsDealer )
	{
		if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
			continue;

		//Reset the arms dealers cash on hand to the default initial value
		gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash = gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash * armsDealerInfo[ ubArmsDealer ].dailyRetained / 100 +
															  armsDealerInfo[ ubArmsDealer ].dailyIncrement;
		if (gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash > armsDealerInfo[ ubArmsDealer ].dailyMaximum)
			gArmsDealerStatus[ ubArmsDealer ].uiArmsDealersCash = armsDealerInfo[ ubArmsDealer ].dailyMaximum;

		//if the arms dealer isn't supposed to have any items (includes all repairmen)
		if( armsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_HAS_NO_INVENTORY )
			continue;

		std::map< UINT16, UINT16> numTotalItems;
		std::set<UINT16> itemsAreOnOrder;
		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();	iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter)
		{
			if (iter->object.exists() == true)
			{
				if (iter->IsBeingOrdered() == false)
				{
					numTotalItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
				}
				else
				{
					itemsAreOnOrder.insert(iter->object.usItem);

					//and today is the day the items come in
					if( iter->uiOrderArrivalTime >= GetWorldDay() )
					{
						iter->ubQtyOnOrder = 0;
						iter->uiOrderArrivalTime = 0;
#ifdef JA2UB						
						//JA25 UB if the dealer is RAUL
						if( ubArmsDealer == ARMS_DEALER_RAUL )
						{
							//set the fact the raul refreshed his inventory
							SetFactTrue( FACT_RAULS_INVENTORY_CHANGED_SINCE_LAST_VISIT );
						}
#endif
					}
				}
			}
		}

		//loop through all items of the same type
		for ( usItemIndex = 1; usItemIndex < gMAXITEMS_READ; ++usItemIndex )
		{
			//if the dealer can sell the item type
			if( CanDealerTransactItem( ubArmsDealer, usItemIndex, FALSE ) )
			{
				//if there are no items on order
				if ( itemsAreOnOrder.count( usItemIndex ) == 0 )
				{
					ubMaxSupply = GetDealersMaxItemAmount( ubArmsDealer, usItemIndex );
					UINT16 halfSupply = (UINT16)(ubMaxSupply / 2);
					UINT16 itemsInStock = 0;
					if ( numTotalItems.count(usItemIndex) > 0 )
					{
						itemsInStock = numTotalItems[usItemIndex];
					}
					//if the qty on hand is half the desired amount or fewer
					if( itemsInStock <= halfSupply )
					{
						//determine if the item can be restocked (assume new, use items aren't checked for until the stuff arrives)
						if (ItemTransactionOccurs( ubArmsDealer, usItemIndex, DEALER_BUYING, FALSE ))
						{
							// figure out how many items to reorder (items are reordered an entire batch at a time)
							ubNumItems = HowManyItemsToReorder( ubMaxSupply, itemsInStock);
#ifdef JA2UB							
							//if the dealer is betty, and we are to ADD the stuff instantly
							if( ubArmsDealer == ARMS_DEALER_BETTY && fInstallyHaveItemsAppear &&
									( usItemIndex == MEDICKIT || usItemIndex == FIRSTAIDKIT ) )
							{
								ubNumItems = ubMaxSupply + ubMaxSupply/2;
							}
#endif
							// if this is the first day the player is eligible to have access to this thing
							if ( gArmsDealerStatus[ubArmsDealer].fPreviouslyEligible[ usItemIndex ] == false )
							{
								// eliminate the ordering delay and stock the items instantly!
								// This is just a way to reward the player right away for making progress without the reordering lag...
								ArmsDealerGetsFreshStock( ubArmsDealer, usItemIndex, ubNumItems );
							}
							else
							{
#ifdef JA2UB							
								if( fInstallyHaveItemsAppear )
								{
									ubReorderDays = 0;
								}
								else
								{
									ubReorderDays = ( UINT8) ( armsDealerInfo[ ubArmsDealer ].daysDelayMin + Random( armsDealerInfo[ ubArmsDealer ].daysDelayMax - armsDealerInfo[ ubArmsDealer ].daysDelayMin ) );
								}
								
#else
								ubReorderDays = ( UINT8) ( armsDealerInfo[ ubArmsDealer ].daysDelayMin + Random( armsDealerInfo[ ubArmsDealer ].daysDelayMax - armsDealerInfo[ ubArmsDealer ].daysDelayMin ) );
#endif
								//Determine when the inventory should arrive
								uiArrivalDay = GetWorldDay() + ubReorderDays;	// consider changing this to minutes
								// post new order
								OrderDealerItems(ubArmsDealer, usItemIndex, ubNumItems, uiArrivalDay);
							}
						}
					}
				}
			}
		}
	}
}

void OrderDealerItems(int armsDealer, int usItem, int numItems, int arrivalDay)
{
	int perfectItems = 0;
	for (int x = 0; x < numItems; ++x)
	{
		int ubItemCondition = DetermineDealerItemCondition( armsDealer, usItem);

		// if the item is brand new
		if ( ubItemCondition == 100)
		{
			++perfectItems;
		}
		else
		{
			// add a used item with that condition to his inventory
			gArmsDealersInventory[ armsDealer ].push_back(DEALER_SPECIAL_ITEM());
			DEALER_SPECIAL_ITEM* pOrder = &(gArmsDealersInventory[ armsDealer ].back());

			pOrder->ubQtyOnOrder = 1;
			pOrder->uiOrderArrivalTime = arrivalDay;
			pOrder->bItemCondition = ubItemCondition;
			CreateObjectForDealer(usItem, ubItemCondition, 1, &(pOrder->object));
		}
	}

	// now add all the perfect ones, in one shot
	if ( perfectItems > 0)
	{
		gArmsDealersInventory[ armsDealer ].push_back(DEALER_SPECIAL_ITEM());
		DEALER_SPECIAL_ITEM* pOrder = &(gArmsDealersInventory[ armsDealer ].back());

		pOrder->ubQtyOnOrder = perfectItems;
		pOrder->uiOrderArrivalTime = arrivalDay;
		/*CHRISL: I can't say for absolute, but I think this is where we are causing the "100rnd" bug.  We're setting
			a bItemConditon 100 regardless of what the item is.  But if we're working with ammo, we don't want
			bItemCondition to be 100 even if the item is "perfect" (i.e., a fully loaded mag).  We need a condition
			so we set this value to magSize when dealing with ammo*/
		if(Item[usItem].usItemClass == IC_AMMO)
			pOrder->bItemCondition = Magazine[Item[usItem].ubClassIndex].ubMagSize;
		else
			pOrder->bItemCondition = 100;
		CreateObjectForDealer(usItem, 100, perfectItems, &(pOrder->object));
	}
}

void ConvertCreatureBloodToElixir( void )
{
	UINT8	ubBloodAvailable = 0;
	UINT8 ubAmountToConvert;

	for (DealerItemList::iterator iter = gArmsDealersInventory[ ARMS_DEALER_GABBY ].begin(); iter != gArmsDealersInventory[ ARMS_DEALER_GABBY ].end(); ++iter)
	{
		if (iter->ItemIsInInventory() == true && iter->object.usItem == JAR_CREATURE_BLOOD)
		{
			ubBloodAvailable += iter->object.ubNumberOfObjects;
		}
	}

	if ( ubBloodAvailable )
	{
		// start converting blood into elixir!
		//ubAmountToConvert = (UINT8) __min( 5 + Random( 3 ), ubBloodAvailable );
		ubAmountToConvert = ubBloodAvailable;

		RemoveItemFromArmsDealerInventory( ARMS_DEALER_GABBY, JAR_CREATURE_BLOOD, ubAmountToConvert );

		ArmsDealerGetsFreshStock( ARMS_DEALER_GABBY, JAR_ELIXIR, ubAmountToConvert );
	}
}

// Flugente: if we are a lua-based merchant, stop selling items we didn't explicitly want to be sold
void RemoveNonIntelItems()
{
	for ( UINT8 ubArmsDealer = 0; ubArmsDealer<NUM_ARMS_DEALERS; ++ubArmsDealer )
	{
		if ( gArmsDealerStatus[ubArmsDealer].fOutOfBusiness )
			continue;

		if ( armsDealerInfo[ubArmsDealer].uiFlags & ARMS_DEALER_DEALWITHINTEL )
		{
			std::vector<UINT16> baditemsvector;

			for ( DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin(); iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter )
			{
				if ( iter->ItemIsInInventory() == true && CalcValueOfItemToDealer( ubArmsDealer, iter->object.usItem, TRUE ) < 1 )
					baditemsvector.push_back( iter->object.usItem );
			}

			for ( std::vector<UINT16>::iterator it2 = baditemsvector.begin(); it2 != baditemsvector.end(); ++it2 )
			{
				RemoveItemFromArmsDealerInventory( ubArmsDealer, (*it2), 100 );
			}
		}
	}
}

BOOLEAN AdjustCertainDealersInventory( )
{
	//Adjust Tony's items (this restocks *instantly* 1/day, doesn't use the reorder system)
	GuaranteeAtLeastOneItemOfType( ARMS_DEALER_TONY, ARMS_DEALER_BIG_GUNS );
	LimitArmsDealersInventory( ARMS_DEALER_TONY, ARMS_DEALER_BIG_GUNS, 2 );
	LimitArmsDealersInventory( ARMS_DEALER_TONY, ARMS_DEALER_HANDGUNCLASS, 3 );
	LimitArmsDealersInventory( ARMS_DEALER_TONY, ARMS_DEALER_AMMO, 8 );

	//Adjust all bartenders' alcohol levels to a minimum
	GuaranteeMinimumAlcohol( ARMS_DEALER_FRANK );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_1 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_2 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_3 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_BAR_BRO_4 );
	GuaranteeMinimumAlcohol( ARMS_DEALER_ELGIN );
	GuaranteeMinimumAlcohol( ARMS_DEALER_MANNY );

	//make sure Sam (hardware guy) has at least one empty jar
	GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_SAM, JAR, 1 );

	if ( CheckFact( FACT_ESTONI_REFUELLING_POSSIBLE, 0 ) )
	{
		// gas is restocked regularly, unlike most items
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_JAKE, GAS_CAN, ( UINT8 ) ( 4 + Random( 3 ) ) );
	}

	//If the player hasn't bought a video camera from Franz yet, make sure Franz has one to sell
	if( !( gArmsDealerStatus[ ARMS_DEALER_FRANZ ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__FRANZ_HAS_SOLD_VIDEO_CAMERA_TO_PLAYER ) )
	{
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_FRANZ, VIDEO_CAMERA, 1 );
	}
#ifdef JA2UB
	//------------UB---------------------
		
	//if Raul hasnt yet sold the barret
	if( !( gArmsDealerStatus[ ARMS_DEALER_RAUL ].ubSpecificDealerFlags & ARMS_DEALER_FLAG__RAUL_HAS_SOLD_BARRETT_TO_PLAYER ) )
	{
		//Guarentee at least 1 Barrett
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_RAUL, BARRETT_UB, 1 );
	}

	/*
		moved to Quest.lua

	//if the player hasnt done the "killed the annoying bloodcats" quest for betty, 
	if( gubQuest[ QUEST_FIX_LAPTOP ] != QUESTDONE && gGameUBOptions.LaptopQuestEnabled == TRUE )
	{
		GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_BETTY, LAPTOP_TRANSMITTER , 1 ); //4500
	}
	else
	{
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, LAPTOP_TRANSMITTER, 0 ); //4500
	}
	*/
	
	if( gGameUBOptions.LaptopQuestEnabled == FALSE )
	{
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, LAPTOP_TRANSMITTER, 0 ); //4500
	}
	
	if( gubQuest[ QUEST_GET_RID_BLOODCATS_AT_BETTYS ] != QUESTDONE )
	{
		//make sure she doesnt sell these items just yet
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, FIRSTAIDKIT, 0 );
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, MEDICKIT, 0 );
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, COMPOUND18, 0 );
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, CERAMIC_PLATES, 0 );
		GuaranteeAtMostNumOfItemsForItem( ARMS_DEALER_BETTY, LAME_BOY, 0 );
	}

	//Guarntee 1 laptop transmitter to be at betty's
	GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_BETTY, PORNOS, 1 );
#endif
	return( TRUE );
}

void LimitArmsDealersInventory( UINT8 ubArmsDealer, UINT32 uiDealerItemType, UINT8 ubMaxNumberOfItemType )
{
	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;

	std::map< UINT16, UINT16> numTotalItems;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin(); iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
		if (iter->ItemIsInInventory() == true && iter->IsUnderRepair() == false) {
			numTotalItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
		}
	}

	UINT32	uiTotalNumberOfItems = 0, uiRandomChoice;
	UINT32	uiIndex;

	UINT16	usItemIndex=0;
	std::vector<int> usAvailableItems;
	std::vector<UINT32> ubNumberOfAvailableItem;
	//loop through all items of the same class and count the number in stock
	for ( usItemIndex = 1; usItemIndex < gMAXITEMS_READ; ++usItemIndex )
	{
		//if there is some items in stock
		if (numTotalItems.count(usItemIndex) > 0)
		{
			//if the item is of the same dealer item type
			if( uiDealerItemType & GetArmsDealerItemTypeFromItemNumber( usItemIndex ) )
			{
				usAvailableItems.push_back(usItemIndex);
				//if the dealer item type is ammo
				if( uiDealerItemType == ARMS_DEALER_AMMO )
				{
					// all ammo of same type counts as only one item
					ubNumberOfAvailableItem.push_back(1);
					++uiTotalNumberOfItems;
				}
				else
				{
					// items being repaired don't count against the limit
					ubNumberOfAvailableItem.push_back(numTotalItems[usItemIndex]);
					uiTotalNumberOfItems += ubNumberOfAvailableItem.back();
				}
			}
		}
	}

	//if there is more of the given type than we want
	INT32 uiItemsToRemove = usAvailableItems.size() - ubMaxNumberOfItemType;

	while (uiItemsToRemove > 0)
	{
		uiRandomChoice = Random( uiTotalNumberOfItems );

		for ( uiIndex = 0; uiIndex < usAvailableItems.size(); ++uiIndex )
		{
			if ( uiRandomChoice <= ubNumberOfAvailableItem[ uiIndex ] )
			{
				usItemIndex = usAvailableItems[ uiIndex ];
				if ( uiDealerItemType == ARMS_DEALER_AMMO )
				{
					// remove all of them, since each ammo item counts as only one "item" here
					// ammo will always be only condition 100, there's never any in special slots
					RemoveItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, ubNumberOfAvailableItem[uiIndex] );
				}
				else
				{
					// pick 1 random one, don't care about its condition
					RemoveRandomItemFromArmsDealerInventory( ubArmsDealer, usItemIndex, 1 );
				}
				// now remove entry from the array by replacing it with the last and decrementing
				// the size of the array
				usAvailableItems[ uiIndex ] = usAvailableItems.back();
				usAvailableItems.pop_back();
				ubNumberOfAvailableItem[ uiIndex ] = ubNumberOfAvailableItem.back();
				ubNumberOfAvailableItem.pop_back();

				// decrement count of # of items to remove
				--uiItemsToRemove;
				break; // and out of 'for' loop
			}
			else
			{
				// next item!
				uiRandomChoice -= ubNumberOfAvailableItem[ uiIndex ];
			}
		}

	}
}

void GuaranteeAtLeastOneItemOfType( UINT8 ubArmsDealer, UINT32 uiDealerItemType )
{
	UINT16 usItemIndex;
	UINT8 ubChance;
	UINT32	uiTotalChances = 0;
	UINT32	uiIndex, uiRandomChoice;

	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;


	std::map< UINT16, UINT16> numTotalItems;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();	iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter)
	{
		if (iter->ItemIsInInventory() == true && iter->IsUnderRepair() == false)
		{
			numTotalItems[iter->object.usItem] += iter->object.ubNumberOfObjects;
		}
	}


	std::vector<UINT16> usAvailableItems;
	std::vector<UINT8> ubChanceForAvailableItem;
	//loop through all items of the same type
	for ( usItemIndex = 1; usItemIndex < gMAXITEMS_READ; ++usItemIndex )
	{
		//if the item is of the same dealer item type
		if( uiDealerItemType & GetArmsDealerItemTypeFromItemNumber( usItemIndex ) )
		{
			//if there are any of these in stock
			if( numTotalItems.count(usItemIndex) > 0 )
			{
				//there is already at least 1 item of that type, return
				return;
			}

			// if he can stock it (it appears in his inventory list)
			if( GetDealersMaxItemAmount( ubArmsDealer, usItemIndex ) > 0)
			{
				// and the stage of the game gives him a chance to have it (assume new)
				ubChance = ChanceOfItemTransaction( ubArmsDealer, usItemIndex, DEALER_BUYING, FALSE );
				if ( ubChance > 0 )
				{
					usAvailableItems.push_back(usItemIndex);
					ubChanceForAvailableItem.push_back(ubChance);
					uiTotalChances += ubChance;
				}
			}
		}
	}

	// if there aren't any such items, the following loop would never finish, so quit before trying it!
	if (usAvailableItems.empty() == true)
	{
		return;
	}

	// CJC: randomly pick one of available items by weighted random selection.

	// randomize number within uiTotalChances and then loop forwards till we find that item
	uiRandomChoice = Random( uiTotalChances );

	for ( uiIndex = 0; uiIndex < usAvailableItems.size(); ++uiIndex )
	{
		if ( uiRandomChoice <= ubChanceForAvailableItem[ uiIndex ] )
		{
			ArmsDealerGetsFreshStock( ubArmsDealer, usAvailableItems[ uiIndex ], 1 );
			return;
		}
		else
		{
			// next item!
			uiRandomChoice -= ubChanceForAvailableItem[ uiIndex ];
		}
	}

	Assert(false);
	// internal logic failure!
}

void GuaranteeAtMostNumOfItemsForItem( UINT8 ubArmsDealer, INT16 sItemIndex, UINT8 ubAtMostNumItems )
{
	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;

	//ADB, ya, a whole 1 line of extra code!
	// not permitted for repair dealers - would take extra code to avoid counting items under repair!
	//Assert( !DoesDealerDoRepairs( ubArmsDealer ) );
	int itemsIHave = 0;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();	iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter)
	{
		if (iter->ItemIsInInventory() == true && 
			iter->object.usItem == sItemIndex && 
			iter->IsUnderRepair() == false)
		{
			itemsIHave -= iter->object.ubNumberOfObjects;
			//if there are any of these in stock
		}
	}
}

void GuaranteeAtLeastXItemsOfIndex( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany )
{
	if( gArmsDealerStatus[ ubArmsDealer ].fOutOfBusiness )
		return;

	//ADB, ya, a whole 1 line of extra code!
	// not permitted for repair dealers - would take extra code to avoid counting items under repair!
	//Assert( !DoesDealerDoRepairs( ubArmsDealer ) );
	int itemsIHave = 0;
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin();	iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter)
	{
		if (iter->ItemIsInInventory() == true
			&& iter->object.usItem == usItemIndex
			&& iter->IsUnderRepair() == false)
		{
			itemsIHave += iter->object.ubNumberOfObjects;
			//if there are any of these in stock
			if( itemsIHave >= ubHowMany )
			{
				// have what we need...
				return;
			}
		}
	}
	
	// if he can stock it (it appears in his inventory list)
// RESTRICTION REMOVED: Jake must be able to guarantee GAS even though it's not in his list, it's presence is conditional
//	if( GetDealersMaxItemAmount( ubArmsDealer, usItemIndex ) > 0)
	{
		//add the item
		ArmsDealerGetsFreshStock( ubArmsDealer, usItemIndex, (UINT8)( ubHowMany - itemsIHave ) );
	}
}

UINT32 GetArmsDealerItemTypeFromItemNumber( UINT16 usItem )
{
	switch( Item[ usItem ].usItemClass )
	{
		case IC_NONE:
			return( 0 );
			break;

		case IC_GUN:
			switch(	Weapon[ Item[ usItem ].ubClassIndex ].ubWeaponClass )
			{
				case HANDGUNCLASS:
					return( ARMS_DEALER_HANDGUNCLASS );
					break;
				case RIFLECLASS:
					if ( ItemIsARocketRifle( usItem ) )
						return( ARMS_DEALER_ROCKET_RIFLE );
					else
						return( ARMS_DEALER_RIFLECLASS );
					break;
				case SHOTGUNCLASS:
					return( ARMS_DEALER_SHOTGUNCLASS );
					break;
				case SMGCLASS:
					return( ARMS_DEALER_SMGCLASS );
					break;
				case MGCLASS:
					return( ARMS_DEALER_MGCLASS );
					break;
				case MONSTERCLASS:
					return( 0 );
					break;
				case KNIFECLASS:
					return( ARMS_DEALER_KNIFECLASS );
					break;
			}
			break;

		case IC_PUNCH:
			if (usItem == NOTHING)
			{
				return( 0 );
			}
			// else treat as blade
		case IC_BLADE:
		case IC_THROWING_KNIFE:
			return( ARMS_DEALER_BLADE );
			break;
		case IC_LAUNCHER:
			return( ARMS_DEALER_LAUNCHER );
			break;
		case IC_ARMOUR:
			return( ARMS_DEALER_ARMOUR );
			break;
		case IC_MEDKIT:
			return( ARMS_DEALER_MEDKIT );
			break;
		case IC_KIT:
			return( ARMS_DEALER_KIT );
			break;
		case IC_MISC:
		{
			if ( Item [usItem].alcohol > 0.0f )
				return( ARMS_DEALER_ALCOHOL );
			else if ( Item [usItem].electronic )
				return( ARMS_DEALER_ELECTRONICS );
			else if ( Item [usItem].hardware )
				return( ARMS_DEALER_HARDWARE );
			else if ( Item [usItem].medical )
				return( ARMS_DEALER_MEDICAL );
			else if ( Item [usItem].attachment )
				return( ARMS_DEALER_ATTACHMENTS );
			else if ( Item [usItem].detonator || Item [usItem].remotedetonator || Item [usItem].remotetrigger )
				return( ARMS_DEALER_DETONATORS );
			else
				return( ARMS_DEALER_MISC );
			//switch on the type of item
//			switch( usItem )
//			{
//				case BEER:
//				case WINE:
//				case ALCOHOL:
//					return( ARMS_DEALER_ALCOHOL );
//					break;
//
//				case METALDETECTOR:
//				case LASERSCOPE:
////				case REMDETONATOR:
//					return( ARMS_DEALER_ELECTRONICS );
//					break;
//
//				case CANTEEN:
//				case CROWBAR:
//				case WIRECUTTERS:
//					return( ARMS_DEALER_HARDWARE );
//					break;
//
//				case ADRENALINE_BOOSTER:
//				case REGEN_BOOSTER:
//				case SYRINGE_3:
//				case SYRINGE_4:
//				case SYRINGE_5:
//					return( ARMS_DEALER_MEDICAL );
//					break;
//
//				case SILENCER:
//				case SNIPERSCOPE:
//				case BIPOD:
//				case DUCKBILL:
//					return( ARMS_DEALER_ATTACHMENTS );
//					break;
//
//				case DETONATOR:
//				case REMDETONATOR:
//				case REMOTEBOMBTRIGGER:
//					return( ARMS_DEALER_DETONATORS );
//					break;
//
//				default:
//					return( ARMS_DEALER_MISC );
//
//					break;
//		}

		}
		break;
		case IC_AMMO:
			return( ARMS_DEALER_AMMO );
			break;
		case IC_FACE:
			if (Item[usItem].electronic )
				return ARMS_DEALER_ELECTRONICS;
			else
				return ARMS_DEALER_FACE;
			//switch( usItem )
			//{
			//	case EXTENDEDEAR:
			//	case NIGHTGOGGLES:
			//	case ROBOT_REMOTE_CONTROL:
			//		return( ARMS_DEALER_ELECTRONICS );
			//	break;

			//	default:
			//		return( ARMS_DEALER_FACE );
			//}
			break;
		case IC_THROWN:
			return( 0 );
//			return( ARMS_DEALER_THROWN );

			break;
		case IC_KEY:
			return( 0 );
//			return( ARMS_DEALER_KEY );
			break;
		case IC_GRENADE:
			return( ARMS_DEALER_GRENADE );
			break;
		case IC_BOMB:
			return( ARMS_DEALER_BOMB );
			break;
		case IC_EXPLOSV:
			return( ARMS_DEALER_EXPLOSV );
			break;
		case IC_TENTACLES:
		case IC_MONEY:
			return( 0 );
			break;

	//	case IC_APPLIABLE:
			break;

	// CHRISL: Itendify new class for inventory system
		case IC_LBEGEAR:
		case IC_BELTCLIP:
		case IC_RANDOMITEM:
			return( 0 );
			break;
		default:
			AssertMsg( FALSE, String( "GetArmsDealerItemTypeFromItemNumber(), invalid class %d for item %d. gMAXITEMS_READ = %d.", Item[ usItem ].usItemClass, usItem, gMAXITEMS_READ ) );
			break;
	}
	return( 0 );
}

BOOLEAN IsMercADealer( UINT8 ubMercID )
{
	// Manny is not actually a valid dealer unless a particular event sets that fact
	if( ( ubMercID == MANNY ) && !CheckFact( FACT_MANNY_IS_BARTENDER, 0 ) )
	{
		return( FALSE );
	}

	// Flugente: if we give Kyle the deed of the leather shop, he becomes a shop owner
	if ( (ubMercID == KYLE) && gubQuest[QUEST_LEATHER_SHOP_DREAM] != QUESTDONE )
	{
		return(FALSE);
	}
	
	//loop through the list of arms dealers
	for ( UINT8 cnt = 0; cnt<NUM_ARMS_DEALERS; ++cnt )
	{
		if (!armsDealerInfo[cnt].ubShopKeeperID) break;
		else if( armsDealerInfo[ cnt ].ubShopKeeperID == ubMercID )
			return( TRUE );
	}
	return( FALSE );
}

INT8 GetArmsDealerIDFromMercID( UINT8 ubMercID )
{
	//loop through the list of arms dealers
	for ( INT8 cnt = 0; cnt<NUM_ARMS_DEALERS; ++cnt )
	{
		if( armsDealerInfo[ cnt ].ubShopKeeperID == ubMercID )
			return( cnt );
	}

	return( -1 );
}

UINT8 GetTypeOfArmsDealer( UINT8	ubDealerID )
{
	return( armsDealerInfo[ ubDealerID ].ubTypeOfArmsDealer );
}

BOOLEAN	DoesDealerDoRepairs( UINT8 ubArmsDealer )
{
	return ( armsDealerInfo[ubArmsDealer].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS );
}

BOOLEAN RepairmanIsFixingItemsButNoneAreDoneYet( UINT8 ubProfileID )
{
	INT8		bArmsDealer;
	BOOLEAN fHaveOnlyUnRepairedItems=FALSE;

	bArmsDealer = GetArmsDealerIDFromMercID( ubProfileID );
	if( bArmsDealer == -1 )
		return( FALSE );

	//if the dealer is not a repair dealer, return
	if( !DoesDealerDoRepairs( bArmsDealer ) )
		return( FALSE );

	//loop through the dealers inventory and check if there are only unrepaired items
	for (DealerItemList::iterator iter = gArmsDealersInventory[ bArmsDealer ].begin(); iter != gArmsDealersInventory[ bArmsDealer ].end(); ++iter)
	{
		if (iter->ItemIsInInventory() == false)
			continue;

		if ( Item[iter->object.usItem].usItemClass == 0 )
			continue;

		if( iter->IsUnderRepair() == true )
		{
			//if the item has been repaired
			if( iter->uiRepairDoneTime <= GetWorldTotalMin() )
			{
				//A repair item is ready, therefore, return false
				return( FALSE );
			}
			else
			{
				fHaveOnlyUnRepairedItems = TRUE;
			}
		}
	}

	return( fHaveOnlyUnRepairedItems );
}

BOOLEAN CanDealerTransactItem( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer )
{
	switch ( armsDealerInfo[ ubArmsDealer ].ubTypeOfArmsDealer )
	{
		case ARMS_DEALER_SELLS_ONLY:
			if ( fPurchaseFromPlayer )
			{
				// this dealer only sells stuff to player, so he can't buy anything from him
				return( FALSE );
			}
			break;

		case ARMS_DEALER_BUYS_ONLY:
			if ( !fPurchaseFromPlayer )
			{
				// this dealer only buys stuff from player, so he can't sell anything to him
				return( FALSE );
			}
			break;

		case ARMS_DEALER_BUYS_SELLS:
			switch ( ubArmsDealer )
			{
				case ARMS_DEALER_JAKE:
				case ARMS_DEALER_KEITH:
				case ARMS_DEALER_FRANZ:
					if ( fPurchaseFromPlayer )
					{
						// these guys will buy nearly anything from the player, regardless of what they carry for sale!
						return( CalcValueOfItemToDealer( ubArmsDealer, usItemIndex, FALSE ) > 0 );
					}
					//else selling inventory uses their inventory list
					break;

				default:
					// the others go by their inventory list
					break;
			}
			break;

		case ARMS_DEALER_REPAIRS:
			// repairmen don't have a complete list of what they'll repair in their inventory,
			// so we must check the item's properties instead.
			return( CanDealerRepairItem( ubArmsDealer, usItemIndex ) );

		default:
			AssertMsg( FALSE, String( "CanDealerTransactItem(), type of dealer %d.	AM 0.", armsDealerInfo[ ubArmsDealer ].ubTypeOfArmsDealer ) );
			return(FALSE);
	}

	return( DoesItemAppearInDealerInventoryList( ubArmsDealer, usItemIndex, fPurchaseFromPlayer ) );
}


BOOLEAN CanDealerRepairItem( UINT8 ubArmsDealer, UINT16 usItemIndex )
{
//	UINT32 uiFlags;

//	uiFlags = Item[ usItemIndex ].fFlags;

	// can't repair anything that's not repairable!
//	if ( !( uiFlags & ITEM_REPAIRABLE ) )
	if ( !( Item[ usItemIndex ].repairable	) )
	{
		return(FALSE);
	}
	
//#ifdef JA2UB	
//Ja25: TEMP!!!
//	return( FALSE );
//#endif
	switch ( ubArmsDealer )
	{
		case ARMS_DEALER_ARNIE:
#ifdef JA2UB
//Ja25 neither in exp.
#else		
		case ARMS_DEALER_PERKO:
#endif		
			// repairs ANYTHING non-electronic
//			if ( !( uiFlags & ITEM_ELECTRONIC ) )
			if ( !( Item[ usItemIndex ].electronic ) )
			{
				return(TRUE);
			}
			break;

		case ARMS_DEALER_FREDO:
			// repairs ONLY electronics
//			if ( uiFlags & ITEM_ELECTRONIC )
			if ( Item[ usItemIndex ].electronic	)
			{
				return(TRUE);
			}
			break;

		default:
			{
				// Flugente: if we set this guy to be a repairguy, and this item is NOT electronic, well, we can
				if ( armsDealerInfo[ubArmsDealer].ubTypeOfArmsDealer == ARMS_DEALER_REPAIRS )
				{
					if ( !( Item[ usItemIndex ].electronic ) )
						return(TRUE);
					else
						return(FALSE);
				}
			}
			AssertMsg( FALSE, String( "CanDealerRepairItem(), Arms Dealer %d is not a recognized repairman!.	AM 1.", ubArmsDealer ) );
	}

	// can't repair this...
	return(FALSE);
}

void ArmsDealerGetsFreshStock( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubNumItems )
{
	UINT8 ubCnt;
	UINT8 ubItemCondition = 0;
	UINT8 ubPerfectOnes = 0;

	// determine the condition of each one, counting up new ones, but adding damaged ones right away
	for ( ubCnt = 0; ubCnt < ubNumItems; ubCnt++ )
	{
		ubItemCondition = DetermineDealerItemCondition( ubArmsDealer, usItemIndex);

		// if the item is brand new
		if ( ubItemCondition == 100)
		{
			++ubPerfectOnes;
		}
		else
		{
			// add a used item with that condition to his inventory
			CreateObjectForDealer(usItemIndex, ubItemCondition, 1, &gTempObject);
			AddObjectToArmsDealerInventory( ubArmsDealer, &gTempObject );
		}
	}

	// now add all the perfect ones, in one shot
	if ( ubPerfectOnes > 0)
	{
		CreateObjectForDealer(usItemIndex, ubItemCondition, ubPerfectOnes, &gTempObject);
		AddObjectToArmsDealerInventory( ubArmsDealer, &gTempObject );
	}
}

UINT8 DetermineDealerItemCondition( UINT8 ubArmsDealer, UINT16 usItemIndex )
{
	UINT8 ubCondition = 100;

	// if it's a damagable item, and not a liquid (those are always sold full)
//	if ( ( Item[ usItemIndex ].fFlags & ITEM_DAMAGEABLE ) && !ItemContainsLiquid( usItemIndex ) )
	if ( ( Item[ usItemIndex ].damageable ) && !ItemContainsLiquid( usItemIndex ) )
	{
		// if he ONLY has used items, or 50% of the time if he carries both used & new items
		if ( ( armsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_ONLY_USED_ITEMS ) ||
			( ( armsDealerInfo[ ubArmsDealer ].uiFlags & ARMS_DEALER_SOME_USED_ITEMS ) && ( Random( 100 ) < 50 ) ) )
		{
			// make the item a used one
			ubCondition = (UINT8)(20 + Random( 60 ));
		}
	}

	return( ubCondition);
}

BOOLEAN ItemContainsLiquid( UINT16 usItemIndex )
{
	return Item[usItemIndex].containsliquid;

	//switch ( usItemIndex )
	//{
	//	case CANTEEN:
	//	case BEER:
	//	case ALCOHOL:
	//	case JAR_HUMAN_BLOOD:
	//	case JAR_CREATURE_BLOOD:
	//	case JAR_QUEEN_CREATURE_BLOOD:
	//	case JAR_ELIXIR:
	//	case GAS_CAN:
	//		return( TRUE );
	//}

	//return( FALSE );
}

bool ItemIsSpecial(DEALER_SPECIAL_ITEM& item)
{
	for (int x = 0; x < item.object.ubNumberOfObjects; ++x) {
		if (item.object[x]->AttachmentListSize() > 0) {
			//items with attachments are special
			return true;
		}
	}
	if (item.IsBeingOrdered() == true) {
		//item being ordered
		return true;
	}
	if (item.IsUnderRepair() == true) {
		//items in for repairs are special
		return true;
	}
	return false;
}

int CountTotalItemsRepairDealerHasInForRepairs( UINT8 ubArmsDealer )
{
	//if the dealer is not a repair dealer, no need to count, return 0
	if ( !DoesDealerDoRepairs( ubArmsDealer ) )
		return 0;

	int	count = 0;

	for ( DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin(); iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter )
	{
		if ( iter->object.exists() && iter->IsUnderRepair() )
		{
			++count;
		}
	}

	return( count );
}

void AddObjectToArmsDealerInventory( UINT8 ubArmsDealer, OBJECTTYPE *pObject )
{
	// split up all the components of an objecttype and add them as seperate items into the dealer's inventory
	OBJECTTYPE seperateObject;
	int total = 0;
	int usItem = pObject->usItem;
	while (pObject->exists() == true)
	{
		pObject->MoveThisObjectTo(seperateObject, 1);
		StackedObjectData* pData = seperateObject[0];

		// loop through any detachable attachments and add them as seperate items
		for (attachmentList::iterator iter = seperateObject[0]->attachments.begin(); iter != seperateObject[0]->attachments.end();) {
			// ARM: Note: this is only used for selling, not repairs, so attachmentes are seperated when sold to a dealer
			// If the attachment is detachable
			if (! (Item[ iter->usItem ].inseparable	) && iter->exists())
			{
				// add this particular attachment (they can't be imprinted, or themselves have attachments!)
				AddObjectToArmsDealerInventory( ubArmsDealer, &(*iter) );
				
				iter = pData->RemoveAttachmentAtIter(iter);
					
				if(UsingNewAttachmentSystem()==true)
					++iter;

				if (iter == pData->attachments.end()) {
					break;
				}
			}
			else {
				++iter;
			}
		}
		int usGunAmmoItem;
		int ubGunShotsLeft;
		int bItemCondition;
		switch ( Item [ usItem ].usItemClass )
		{
			case IC_GUN:
				usGunAmmoItem = seperateObject[0]->data.gun.usGunAmmoItem;
				ubGunShotsLeft = seperateObject[0]->data.gun.ubGunShotsLeft;
				bItemCondition = seperateObject[0]->data.gun.bGunAmmoStatus;

				// if the gun was jammed, this will forget about the jam (i.e. dealer immediately unjams anything he buys)
				//reset everything but bGunStatus
				seperateObject[0]->data.gun.usGunAmmoItem = 0;
				seperateObject[0]->data.gun.ubGunShotsLeft = 0;
				seperateObject[0]->data.gun.bGunAmmoStatus = 0;
				seperateObject[0]->data.gun.ubGunState = 0;
				seperateObject[0]->data.gun.ubGunAmmoType = 0;

				// add the gun (keeps the object's status and imprintID)
				AddItemToArmsDealerInventory( ubArmsDealer, seperateObject );

				// if any GunAmmoItem is specified
				if( usGunAmmoItem != NONE)
				{
					// if it's regular ammo
					if( Item[ usGunAmmoItem ].usItemClass == IC_AMMO )
					{
						// and there are some remaining
						if ( ubGunShotsLeft > 0 )
						{
							// add the bullets of its remaining ammo
							AddAmmoToArmsDealerInventory( ubArmsDealer, usGunAmmoItem, ubGunShotsLeft );
						}
					}
					else	// assume it's attached ammo (mortar shells, grenades)
					{
						// add the launchable item (can't be imprinted, or have attachments!)

						// if the gun it was in was jammed, get rid of the negative status now
						if ( bItemCondition < 0 )
						{
							bItemCondition *= -1;
						}
						CreateObjectForDealer(usGunAmmoItem, bItemCondition, 1, &seperateObject);
						AddItemToArmsDealerInventory( ubArmsDealer, seperateObject );
					}
				}
				break;

			case IC_AMMO:
				// add the contents of each magazine (multiple mags may have vastly different #bullets left)
				total += seperateObject[0]->data.ubShotsLeft;
				break;

			default:
				// add each object seperately (multiple objects may have vastly different statuses, keep any imprintID)
				if (seperateObject[0]->data.objectStatus == 100) {
					total += 1;
				}
				else {
					AddItemToArmsDealerInventory( ubArmsDealer, seperateObject );
				}
		}
	}

	switch ( Item [ usItem ].usItemClass )
	{
		case IC_GUN:
			break;
		case IC_AMMO:
			AddAmmoToArmsDealerInventory( ubArmsDealer, usItem, total );
			break;
		default:
			if (total) {
				CreateObjectForDealer(usItem, 100, total, &seperateObject);
				AddItemToArmsDealerInventory( ubArmsDealer, seperateObject );
			}
			break;
	}
}


void AddAmmoToArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT16 ubShotsLeft )
{
	// Ammo only, please!!!
	if (Item [ usItemIndex ].usItemClass != IC_AMMO )
	{
		Assert(0);
		return;
	}

	if ( ubShotsLeft == 0)
	{
		return;
	}


	UINT16 ubMagCapacity = Magazine[ Item[ usItemIndex ].ubClassIndex ].ubMagSize;
	int numFullMags = 0;
	if ( ubShotsLeft >= ubMagCapacity )
	{
		// add however many FULL magazines the #shot left represents
		numFullMags = ubShotsLeft / ubMagCapacity;
		ubShotsLeft %= ubMagCapacity;
	}

	// any shots left now are "strays" - not enough to completely fill a magazine of this type
	if ( ubShotsLeft > 0 )
	{
		// handle "stray" ammo - add it to the dealer's stray pile

		UINT16 *pubStrayAmmo = &(gArmsDealerStatus[ubArmsDealer].ubStrayAmmo[ usItemIndex ]);
		*pubStrayAmmo += ubShotsLeft;

		// if dealer has accumulated enough stray ammo to make another full magazine, convert it!
		if ( *pubStrayAmmo >= ubMagCapacity )
		{
			numFullMags += *pubStrayAmmo / ubMagCapacity;
			*pubStrayAmmo %= ubMagCapacity;
		}
		// I know, I know, this is getting pretty anal...	But what the hell, it was easy enough to do.	ARM.
	}
	if (numFullMags) {
		CreateObjectForDealer(usItemIndex, 100, numFullMags, &gTempObject);
		AddItemToArmsDealerInventory( ubArmsDealer, gTempObject );
	}
}


//Use AddObjectToArmsDealerInventory() instead of this when converting a complex item in OBJECTTYPE format.
void AddItemToArmsDealerInventory( UINT8 ubArmsDealer, OBJECTTYPE& object )
{
	//we can only add items to a stack if the item has no attachments (not even default)
	//and if it is either perfect or ammo (and same amount of ammo)
	if ( !object[0]->AttachmentListSize() &&
		(object[0]->data.objectStatus == 100 || Item [ object.usItem ].usItemClass == IC_AMMO) )
	{
		//first find existing items with same perfect status, if found add to that, else create new one
		for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
			iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {

			if (iter->ItemIsInInventory() == true &&
				iter->object.usItem == object.usItem &&
				iter->bItemCondition == object[0]->data.objectStatus) {
				//won't find objects in for repair

				//make sure it's not something special, objects with default attachments aren't handled, but oh well
				if (ItemIsSpecial(*iter) == false) {
					iter->object.AddObjectsToStack(object);
					if (object.exists() == false) {
						return;
					}
				}
			}
		}
	}

	if (object.exists() == true) {
		gArmsDealersInventory[ubArmsDealer].push_back(DEALER_SPECIAL_ITEM());
		DEALER_SPECIAL_ITEM* pItem = &(gArmsDealersInventory[ubArmsDealer].back());
		pItem->object = object;
		pItem->bItemCondition = object[0]->data.objectStatus;
	}
}

// removes ubHowMany items of usItemIndex with the matching Info from dealer ubArmsDealer
void RemoveItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany, OBJECTTYPE *pObj )
{
	BOOLEAN		match = TRUE;

	if ( ubHowMany == 0)
	{
		return;
	}
	for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin(); iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter)
	{
		if (iter->ItemIsInInventory() == true && iter->object.usItem == usItemIndex)
		{
			if(pObj != 0)
			{
				for(unsigned int i = 0; i < ubHowMany; ++i)
				{
					if((*pObj)[i]->operator == (*iter->object[i]))
						match = TRUE;
					else
						match = FALSE;
				}
			}
			if(match == TRUE)
			{
				if (ubHowMany >= iter->object.ubNumberOfObjects) {
					ubHowMany -= iter->object.ubNumberOfObjects;
					iter = gArmsDealersInventory[ ubArmsDealer ].erase(iter);
					DealerItemList::iterator iterEnd = gArmsDealersInventory[ ubArmsDealer ].end();
					if (ubHowMany > 0 || iter == gArmsDealersInventory[ ubArmsDealer ].end()) {
						return;
					}
				}
				else {
					iter->object.RemoveObjectsFromStack(ubHowMany);
					return;
				}
			}
		}
	}
}



void RemoveRandomItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany )
{

	//ADB, ya, a whole 1 line of extra code!
	// not permitted for repair dealers - would take extra code to avoid counting items under repair!
	//Assert( !DoesDealerDoRepairs( ubArmsDealer ) );
	std::vector<DealerItemList::iterator> foundObjects;
	while ( ubHowMany > 0)
	{
		int numItems = 0;
		foundObjects.clear();
		for (DealerItemList::iterator iter = gArmsDealersInventory[ ubArmsDealer ].begin(); iter != gArmsDealersInventory[ ubArmsDealer ].end(); ++iter) {
			if (iter->ItemIsInInventory() == true && iter->object.usItem == usItemIndex && iter->IsUnderRepair() == false) {
				foundObjects.push_back(iter);
				numItems += iter->object.ubNumberOfObjects;
			}
		}
		if (numItems == 0) {
			return;
		}

		// pick a random one to get rid of
		UINT8 ubWhichOne = (UINT8)Random(numItems);
		std::vector<DealerItemList::iterator>::iterator iterToRemove = foundObjects.begin();
		for (int x = (*iterToRemove)->object.ubNumberOfObjects - 1; x < ubWhichOne;) {
			x += (*iterToRemove)->object.ubNumberOfObjects;
			if(x < ubWhichOne)
				++iterToRemove;
		}
		if ((*iterToRemove)->object.ubNumberOfObjects == 1) {
			gArmsDealersInventory[ubArmsDealer].erase(*iterToRemove);
		}
		else {
			//not totally random, but close enough
			(*iterToRemove)->object.RemoveObjectsFromStack(1);
		}
		ubHowMany--;
	}
}


BOOLEAN AddDeadArmsDealerItemsToWorld( UINT8 usProfileID, UINT8 aMercID )
{
	//Get Dealer ID from from merc Id
	INT8 bArmsDealer = GetArmsDealerIDFromMercID( usProfileID );

	//Get a pointer to the dealer
	SOLDIERTYPE* pSoldier = FindSoldierByProfileID( usProfileID, FALSE );

	// not if this isn't a proper profile
	if ( usProfileID == NO_PROFILE )
	{
		pSoldier = MercPtrs[aMercID];

		if ( pSoldier && pSoldier->sNonNPCTraderID > 0 )
			bArmsDealer = pSoldier->sNonNPCTraderID;
		else
			bArmsDealer = -1;
	}

	if( bArmsDealer <= -1 )
	{
		// not a dealer, that's ok, we get called for every dude that croaks.
		return( FALSE );
	}
	
	// mark the dealer as being out of business!
	gArmsDealerStatus[ bArmsDealer ].fOutOfBusiness = TRUE;

	if( pSoldier == NULL )
	{
		// This should never happen, a dealer getting knocked off without the sector being loaded, should it?
		// If it's possible, we should modify code below to dump his belongings into the sector without using pSoldier->sGridNo
		Assert(0);
		return( FALSE );
	}
#ifdef JA2UB	//RAUL
	//Ja25 UB
	//if Raul blew him,self up, dont drop any items
	if( pSoldier->ubProfile == RAUL_UB /* RAUL */ && IsJa25GeneralFlagSet( JA_GF__RAUL_BLOW_HIMSELF_UP ) )
	{
		return( TRUE );
	}
#endif
	//loop through all the items in the dealer's inventory, and drop them all where the dealer was set up.
	for (DealerItemList::iterator iter = gArmsDealersInventory[bArmsDealer].begin();
		iter != gArmsDealersInventory[bArmsDealer].end(); ++iter) {
		if (iter->ItemIsInInventory() == true) {
			AddItemToPool( pSoldier->sInitialGridNo, &(iter->object), INVISIBLE, 0, 0, 0 );
		}
	}

	gArmsDealersInventory[bArmsDealer].clear();
	
	//if the dealer has money
	if( gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash > 0 )
	{
		//Create the object
		if( !CreateMoney( gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash, &gTempObject ) )
		{
			return( FALSE );
		}

		//add the money item to the dealers feet
		AddItemToPool( pSoldier->sInitialGridNo, &gTempObject, INVISIBLE, 0, 0, 0 );

		gArmsDealerStatus[ bArmsDealer ].uiArmsDealersCash = 0;
	}

	return( TRUE );
}

void CreateObjectForDealer( int usItem, int status, int numObjects, OBJECTTYPE *pObject )
{
	//if the item condition is below 0, the item is in for repairs, so flip the sign
	if( status < 0 )
	{
		status *= -1;
	}
	CreateItems(usItem, status, numObjects, pObject);
	MakeObjectOutOfDealerItems(NULL, pObject);
	return;
}

void MakeObjectOutOfDealerItems( DEALER_SPECIAL_ITEM *pSpclItemInfo, OBJECTTYPE *pObject )
{
	INT16 bItemCondition;
	if (pSpclItemInfo != NULL) {
		bItemCondition = pSpclItemInfo->bItemCondition;
		//we might be just calling this to setup the object the dealer has
		if (pObject != &(pSpclItemInfo->object)) {
			*pObject = pSpclItemInfo->object;
		}
	}
	else {
		bItemCondition = (*pObject)[0]->data.objectStatus;
	}


	//if the item condition is below 0, the item is in for repairs, so flip the sign
	if( bItemCondition < 0 )
	{
		bItemCondition *= -1;
	}

	for (int subObject = 0; subObject < pObject->ubNumberOfObjects; ++subObject) {
		(*pObject)[subObject]->data.objectStatus = bItemCondition;

		// if it's a gun
		if (Item [ pObject->usItem ].usItemClass == IC_GUN )
		{
			// Empty out the bullets put in by CreateItem().	We now sell all guns empty of bullets.	This is so that we don't
			// have to keep track of #bullets in a gun throughout dealer inventory.	Without this, players could "reload" guns
			// they don't have ammo for by selling them to Tony & buying them right back fully loaded!	One could repeat this
			// ad nauseum (empty the gun between visits) as a (really expensive) way to get unlimited special ammo like rockets.
			//CHRISL: If we're working with a SingleShotRocketLauncher, we need ubGunShotsLeft to equal 1
			if(Item[pObject->usItem].singleshotrocketlauncher == TRUE)
				(*pObject)[subObject]->data.gun.ubGunShotsLeft = 1;
			else
				(*pObject)[subObject]->data.gun.ubGunShotsLeft = 0;
		}
	}
}



void GiveObjectToArmsDealerForRepair( UINT8 ubArmsDealer, OBJECTTYPE *pObject, UINT8 ubOwnerProfileId )
{


	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	// Any object passed into here must already be:
	//		a) Unstacked
	Assert( pObject->ubNumberOfObjects == 1 );

	//		b) Repairable
	Assert( CanDealerRepairItem( ubArmsDealer, pObject->usItem ) );

	//		c) Actually damaged, or a rocket rifle (being reset)
	Assert( ( (*pObject)[0]->data.objectStatus < 100 ) || ItemIsARocketRifle( pObject->usItem ) );

/* ARM: Can now repair with removeable attachments still attached...
	//		d) Already stripped of all *detachable* attachments
*/

	//		e) If a gun, stripped of any non-ammo-class GunAmmoItems, and bullets
	if (Item [ pObject->usItem ].usItemClass == IC_GUN )
	{
		// if any GunAmmoItem is specified
		if( (*pObject)[0]->data.gun.usGunAmmoItem != NONE)
		{
			// it better be regular ammo, and empty
			Assert( Item[ (*pObject)[0]->data.gun.usGunAmmoItem ].usItemClass == IC_AMMO );
			Assert( (*pObject)[0]->data.gun.ubGunShotsLeft == 0 );
		}
	}

	// ok, given all that, now everything is easy!
	// if the gun was jammed, this will forget about the jam (i.e. dealer immediately unjams anything he will be repairing)
	GiveItemToArmsDealerforRepair( ubArmsDealer, pObject, ubOwnerProfileId );
}


//PLEASE: Use GiveObjectToArmsDealerForRepair() instead of this when repairing a item in OBJECTTYPE format.
void GiveItemToArmsDealerforRepair( UINT8 ubArmsDealer, OBJECTTYPE* pObject, UINT8 ubOwnerProfileId )
{
	UINT32	uiTimeWhenFreeToStartIt;
	UINT32	uiMinutesToFix;
	UINT32	uiMinutesShopClosedBeforeItsDone;
	UINT32	uiDoneWhen;


	Assert( DoesDealerDoRepairs( ubArmsDealer ) );
	Assert( (*pObject)[0]->data.objectStatus > 0 );
	Assert( ( (*pObject)[0]->data.objectStatus < 100 ) || ItemIsARocketRifle( pObject->usItem ) );

	// figure out the earliest the repairman will be free to start repairing this item
	uiTimeWhenFreeToStartIt = WhenWillRepairmanBeAllDoneRepairing( ubArmsDealer );

	//Determine how long it will take to fix
	uiMinutesToFix = CalculateObjectItemRepairTime( ubArmsDealer, pObject );

	uiMinutesShopClosedBeforeItsDone = CalculateOvernightRepairDelay( ubArmsDealer, uiTimeWhenFreeToStartIt, uiMinutesToFix );

	// clock time when this will finally be ready
	uiDoneWhen = uiTimeWhenFreeToStartIt + uiMinutesToFix + uiMinutesShopClosedBeforeItsDone;

	gArmsDealersInventory[ ubArmsDealer ].push_back(DEALER_SPECIAL_ITEM());
	DEALER_SPECIAL_ITEM* pItem = &gArmsDealersInventory[ ubArmsDealer ].back();
	pItem->object = *pObject;
	// Negate the status
	pItem->bItemCondition = (*pObject)[0]->data.objectStatus * -1;

	//Set the time at which item will be fixed
	pItem->uiRepairDoneTime = uiDoneWhen;
	//Remember the original owner of the item
	pItem->ubOwnerProfileId = ubOwnerProfileId;
}



UINT32 WhenWillRepairmanBeAllDoneRepairing( UINT8 ubArmsDealer )
{
	UINT32	uiWhenFree;

	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	// if nothing is in for repairs, he'll be free RIGHT NOW!
	uiWhenFree = GetWorldTotalMin();

	//loop through the dealers inventory
	for (DealerItemList::iterator iter = gArmsDealersInventory[ubArmsDealer].begin();
		iter != gArmsDealersInventory[ubArmsDealer].end(); ++iter) {
		if ( iter->ItemIsInInventory() == true )
		{
			if( iter->IsUnderRepair() == true )
			{
				// if this item will be done later than the latest we've found so far
				if ( iter->uiRepairDoneTime > uiWhenFree )
				{
					// then we're busy til then!
					uiWhenFree = iter->uiRepairDoneTime;
				}
			}
		}
	}
	return( uiWhenFree );
}


UINT32 CalculateObjectItemRepairTime( UINT8 ubArmsDealer, OBJECTTYPE *pItemObject )
{
	UINT32 uiRepairTime = 0;

	for (int x = 0; x < pItemObject->ubNumberOfObjects; ++x) {
		uiRepairTime += CalculateSimpleItemRepairTime( ubArmsDealer, pItemObject->usItem, (*pItemObject)[x]->data.objectStatus );

		// add time to repair any attachments on it
		for (attachmentList::iterator iter = (*pItemObject)[x]->attachments.begin(); iter != (*pItemObject)[x]->attachments.end(); ++iter) {
			// if damaged and repairable
			if ( ( (*iter)[x]->data.objectStatus < 100 ) && CanDealerRepairItem( ubArmsDealer, iter->usItem ) && iter->exists())
			{
				//uiRepairTime += CalculateSimpleItemRepairTime( ubArmsDealer, iter->usItem, (*iter)[x]->data.objectStatus );
				uiRepairTime += CalculateObjectItemRepairTime( ubArmsDealer, &(*iter) );
			}
		}
	}

	return( uiRepairTime );
}


UINT32 CalculateSimpleItemRepairTime( UINT8 ubArmsDealer, UINT16 usItemIndex, INT16 bItemCondition )
{
	UINT32	uiTimeToRepair = 0;
	UINT32	uiRepairCost = 0;

	Assert( DoesDealerDoRepairs( ubArmsDealer ) );

	// first calc what he'll charge - that takes care of item condition, repair ease, and his repair cost "markup"
	uiRepairCost = CalculateSimpleItemRepairCost( ubArmsDealer, usItemIndex, bItemCondition );

	// Now adjust that for the repairman's individual repair speed.
	// For a repairman, his BUY modifier controls his REPAIR SPEED (1.0 means minutes to repair = price in $)
	// with a REPAIR SPEED of 1.0, typical gun price of $2000, and a REPAIR COST of 0.5 this works out to 16.6 hrs
	//		for a full 100% status repair...	Not bad.
	uiTimeToRepair = (UINT32)( uiRepairCost * armsDealerInfo[ ubArmsDealer ].dRepairSpeed );

	// repairs on electronic items take twice as long if the guy doesn't have the skill
	// for dealers, this means anyone but Fredo the Electronics guy takes twice as long (but doesn't charge double)
	// (Mind you, current he's the ONLY one who CAN repair Electronics at all!	Oh well.)
//	if( ( Item[ usItemIndex ].fFlags & ITEM_ELECTRONIC ) && ( ubArmsDealer != ARMS_DEALER_FREDO ) )
	if( ( Item[ usItemIndex ].electronic	) && ( ubArmsDealer != ARMS_DEALER_FREDO ) )
	{
		uiTimeToRepair *= 2;
	}

	// avoid "instant" repairs on really cheap, barely damaged crap...
	if (uiTimeToRepair < MIN_REPAIR_TIME_IN_MINUTES)
	{
		uiTimeToRepair = MIN_REPAIR_TIME_IN_MINUTES;
	}

	return( uiTimeToRepair );
}



UINT32 CalculateObjectItemRepairCost( UINT8 ubArmsDealer, OBJECTTYPE *pItemObject )
{
	UINT32 uiRepairCost = 0;

	for (int x = 0; x < pItemObject->ubNumberOfObjects; ++x) {
		uiRepairCost = CalculateSimpleItemRepairCost( ubArmsDealer, pItemObject->usItem, (*pItemObject)[x]->data.objectStatus );

		// add cost of repairing any attachments on it
		for (attachmentList::iterator iter = (*pItemObject)[x]->attachments.begin(); iter != (*pItemObject)[x]->attachments.end(); ++iter) {
			// if damaged and repairable
			if ( ( (*iter)[x]->data.objectStatus < 100 ) && CanDealerRepairItem( ubArmsDealer, iter->usItem ) && iter->exists())
			{
				uiRepairCost += CalculateObjectItemRepairCost( ubArmsDealer, &(*iter));
			}
		}
	}

	return( uiRepairCost );
}


UINT32 CalculateSimpleItemRepairCost( UINT8 ubArmsDealer, UINT16 usItemIndex, INT16 bItemCondition )
{
	UINT32	uiItemCost = 0;
	UINT32	uiRepairCost = 0;
	INT16		sRepairCostAdj = 0;
//	UINT32	uiDifFrom10=0;

	// figure out the full value of the item, modified by this dealer's personal Sell (i.e. repair cost) modifier
	// don't use CalcShopKeeperItemPrice - we want FULL value!!!
	uiItemCost = (UINT32)(( Item[ usItemIndex ].usPrice * armsDealerInfo[ ubArmsDealer ].dRepairCost ) );

	// get item's repair ease, for each + point is 10% easier, each - point is 10% harder to repair
	sRepairCostAdj = 100 - ( 10 * Item[ usItemIndex ].bRepairEase );

	// make sure it ain't somehow gone too low!
	if (sRepairCostAdj < 10)
	{
		sRepairCostAdj = 10;
	}

	// calculate repair cost, the more broken it is the more it costs, and the difficulty of repair it is also a factor
	uiRepairCost = (UINT32)( uiItemCost * ( sRepairCostAdj * (100 - bItemCondition) / ((FLOAT)100 * 100) ));

/*
	//if the price is not diviseble by 10, make it so
	uiDifFrom10 = 10 - uiRepairCost % 10;
	if( uiDifFrom10 != 0 )
	{
		uiRepairCost += uiDifFrom10;
	}
*/

	if ( ItemIsARocketRifle( usItemIndex ) )
	{
		// resetting imprinting for a rocket rifle costs something extra even if rifle is at 100%
		uiRepairCost += 100;
	}


	// anything repairable has to have a minimum price
	if ( uiRepairCost < MIN_REPAIR_COST )
	{
		uiRepairCost = MIN_REPAIR_COST;
	}

	return( uiRepairCost );
}


BOOLEAN DoesItemAppearInDealerInventoryList( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer )
{
	// the others will buy only things that appear in their own "for sale" inventory lists
	DEALER_POSSIBLE_INV* pDealerInv = GetPointerToDealersPossibleInventory( ubArmsDealer );
	Assert( pDealerInv != NULL );

	// loop through the dealers' possible inventory and see if the item exists there
	UINT16 usCnt = 0;
	while( pDealerInv[ usCnt ].sItemIndex != LAST_DEALER_ITEM )
	{
		//if the initial dealer inv contains the required item, the dealer can sell the item
		if( pDealerInv[ usCnt ].sItemIndex == usItemIndex )
		{
			// if optimal quantity listed is 0, it means dealer won't sell it himself, but will buy it from the player!
			if ( ( pDealerInv[ usCnt ].ubOptimalNumber > 0 ) || fPurchaseFromPlayer )
			{
				return( TRUE );
			}
		}

		++usCnt;
	}

	return( FALSE );
}

UINT16 CalcValueOfItemToDealer( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerSelling )
{
	UINT16 usBasePrice;
	UINT8 ubItemPriceClass;
	UINT8 ubDealerPriceClass;
	UINT16 usValueToThisDealer;
	
	usBasePrice = Item[ usItemIndex ].usPrice;

	// Flugente: if we deal with intel, get the price of an item straight from LUA
	if ( ( armsDealerInfo[ubArmsDealer].uiFlags & ARMS_DEALER_DEALWITHINTEL ) )
	{
		if ( !gArmsDealerAdditionalIntelData[ubArmsDealer].empty() )
		{
			for ( std::vector<ARMS_DEALER_ITEM_INTEL>::iterator it = gArmsDealerAdditionalIntelData[ubArmsDealer].begin(); it != gArmsDealerAdditionalIntelData[ubArmsDealer].end(); ++it )
			{
				if ( ( *it ).usItem == usItemIndex )
					return ( *it ).sIntelPrice;
			}
		}

		return 0;
	}


	if ( usBasePrice == 0 )
	{
		// worthless to any dealer
		return( 0 );
	}
	
	// figure out the price class this dealer prefers
	switch ( ubArmsDealer )
	{
		case ARMS_DEALER_JAKE:
			ubDealerPriceClass = PRICE_CLASS_JUNK;
			break;
		case ARMS_DEALER_KEITH:
			ubDealerPriceClass = PRICE_CLASS_CHEAP;
			break;
		case ARMS_DEALER_FRANZ:
			ubDealerPriceClass = PRICE_CLASS_EXPENSIVE;
			break;

		// other dealers don't use this system
		default:
			if ( DoesItemAppearInDealerInventoryList( ubArmsDealer, usItemIndex, TRUE ) )
			{
				return( usBasePrice );
			}
			else
			{
				return( 0 );
			}
	}
	
	// the rest of this function applies only to the "general" dealers ( Jake, Keith, and Franz )

	// Micky & Gabby specialize in creature parts & such, the others don't buy these at all (exception: jars)
	if ( ( !Item[usItemIndex].jar ) &&
				( DoesItemAppearInDealerInventoryList( ARMS_DEALER_MICKY, usItemIndex, TRUE ) ||
					DoesItemAppearInDealerInventoryList( ARMS_DEALER_GABBY, usItemIndex, TRUE ) ) )
	{
		return( 0 );
	}

	if ( ( ubArmsDealer == ARMS_DEALER_KEITH ) && ( Item [ usItemIndex].usItemClass & ( IC_GUN | IC_LAUNCHER ) ) )
	{
		// Keith won't buy guns until the Hillbillies are vanquished
		if( CheckFact( FACT_HILLBILLIES_KILLED, KEITH ) == FALSE )
		{
			return( 0 );
		}
	}
	
	// figure out which price class it belongs to
	if ( usBasePrice < 100 )
	{
		ubItemPriceClass = PRICE_CLASS_JUNK;
	}
	else if ( usBasePrice < 1000 )
	{
		ubItemPriceClass = PRICE_CLASS_CHEAP;
	}
	else
	{
		ubItemPriceClass = PRICE_CLASS_EXPENSIVE;
	}
	
	if( !fDealerSelling )
	{
		// junk dealer won't buy expensive stuff at all, expensive dealer won't buy junk at all
		if ( abs( (INT8) ubDealerPriceClass - (INT8) ubItemPriceClass ) == 2 )
		{
			return( 0 );
		}
	}

	// start with the base price
	usValueToThisDealer = usBasePrice;

	// if it's out of their preferred price class
	if ( ubDealerPriceClass != ubItemPriceClass )
	{
		// exception: Gas (Jake's)
//		if ( usItemIndex != GAS_CAN )
		if ( !Item[usItemIndex].gascan )
		{
			// they pay only 1/3 of true value!
			usValueToThisDealer /= 3;
		}
	}

	// Tony specializes in guns, weapons, and ammo, so make others pay much less for that kind of stuff
	if ( DoesItemAppearInDealerInventoryList( ARMS_DEALER_TONY, usItemIndex, TRUE ) )
	{
		// others pay only 1/2 of that value!
		usValueToThisDealer /= 2;
	}

	// minimum bet $1 !
	if ( usValueToThisDealer == 0 )
	{
		usValueToThisDealer = 1;
	}

	return( usValueToThisDealer );
}

void GuaranteeMinimumAlcohol( UINT8 ubArmsDealer )
{
	GuaranteeAtLeastXItemsOfIndex( ubArmsDealer, BEER,		( UINT8 ) ( GetDealersMaxItemAmount( ubArmsDealer, BEER ) / 3 ) );
	GuaranteeAtLeastXItemsOfIndex( ubArmsDealer, WINE,		( UINT8 ) ( GetDealersMaxItemAmount( ubArmsDealer, WINE ) / 3 ) );
	GuaranteeAtLeastXItemsOfIndex( ubArmsDealer, ALCOHOL, ( UINT8 ) ( GetDealersMaxItemAmount( ubArmsDealer, ALCOHOL ) / 3 ) );
}

BOOLEAN ItemIsARocketRifle( INT16 sItemIndex )
{
	return( Item[sItemIndex].fingerprintid );
}

BOOLEAN GetArmsDealerShopHours( UINT8 ubArmsDealer, UINT32 *puiOpeningTime, UINT32 *puiClosingTime )
{
	SOLDIERTYPE *pSoldier;

	pSoldier = FindSoldierByProfileID( armsDealerInfo[ ubArmsDealer ].ubShopKeeperID, FALSE );
	if ( pSoldier == NULL )
	{
		return( FALSE );
	}

	if ( ExtractScheduleDoorLockAndUnlockInfo( pSoldier, puiOpeningTime, puiClosingTime ) == FALSE )
	{
		return( FALSE );
	}

	Assert( *puiOpeningTime < *puiClosingTime );

	return( TRUE );
}

UINT32 CalculateOvernightRepairDelay( UINT8 ubArmsDealer, UINT32 uiTimeWhenFreeToStartIt, UINT32 uiMinutesToFix )
{
	UINT32 uiOpeningTime, uiClosingTime;
	UINT32 uiMinutesClosedOvernight;
	UINT32 uiDelayInDays = 0;
	UINT32 uiDoneToday;
	
	Assert( uiMinutesToFix > 0 );

	// convert world time into 24hr military time for the day he's gonna start on it
	uiTimeWhenFreeToStartIt = uiTimeWhenFreeToStartIt % NUM_MIN_IN_DAY;

	if ( GetArmsDealerShopHours( ubArmsDealer, &uiOpeningTime, &uiClosingTime ) == FALSE )
	{
		return( 0 );
	}

	// if it won't get done by the end of a day
	while ( ( uiTimeWhenFreeToStartIt + uiMinutesToFix ) > uiClosingTime )
	{
		// this is to handle existing saves with overnight repairs
		if ( uiTimeWhenFreeToStartIt < uiClosingTime )
		{
			// he gets this much done before closing
			uiDoneToday = uiClosingTime - uiTimeWhenFreeToStartIt;
			// subtract how much he got done
			uiMinutesToFix -= uiDoneToday;
			Assert( uiMinutesToFix > 0 );
		}

		// he starts back at it first thing in the morning
		uiTimeWhenFreeToStartIt = uiOpeningTime;
		uiDelayInDays++;
	}

	uiMinutesClosedOvernight = NUM_MIN_IN_DAY - ( uiClosingTime - uiOpeningTime );

	return ( uiDelayInDays * uiMinutesClosedOvernight );
}

UINT32 CalculateMinutesClosedBetween( UINT8 ubArmsDealer, UINT32 uiStartTime, UINT32 uiEndTime )
{
	UINT32 uiOpeningTime, uiClosingTime;
	UINT32 uiMinutesClosedOvernight;
	UINT32 uiDaysDifference = 0;
	UINT32 uiMinutesClosed = 0;

	Assert( uiStartTime <= uiEndTime );

	if ( GetArmsDealerShopHours( ubArmsDealer, &uiOpeningTime, &uiClosingTime ) == FALSE )
	{
		return( 0 );
	}

	uiMinutesClosedOvernight = NUM_MIN_IN_DAY - ( uiClosingTime - uiOpeningTime );

	// NOTE: this assumes stored are only closed overnight, so all we have to do is compare the day portion
	uiDaysDifference = ( uiEndTime / NUM_MIN_IN_DAY ) - ( uiStartTime / NUM_MIN_IN_DAY );

	if ( uiDaysDifference >= 2 )
	{
		// close for 1 less than that many full nights...
		uiMinutesClosed = ( uiDaysDifference - 1 ) * uiMinutesClosedOvernight;
	}
	
	// add partial day's closing

	// convert start and end times into 24hr military time
	uiStartTime = uiStartTime % NUM_MIN_IN_DAY;
	uiEndTime = uiEndTime % NUM_MIN_IN_DAY;

	// treat end time of midnight as 24:00 hours to prevent indefinite recursion and make formulas work
	if ( uiEndTime == 0 )
	{
		uiEndTime = NUM_MIN_IN_DAY;
	}
	
	if ( uiStartTime == uiEndTime )
	{
		if ( uiDaysDifference == 0 )
		{
			return( 0 );
		}
		else
		{
			uiMinutesClosed += uiMinutesClosedOvernight;
		}
	}

	if ( uiStartTime < uiEndTime )
	{
		if ( uiStartTime < uiOpeningTime )
		{
			// add how many minutes in the time range BEFORE the store opened that day
			uiMinutesClosed += ( min( uiOpeningTime, uiEndTime ) - uiStartTime );
		}

		if ( uiEndTime > uiClosingTime )
		{
			// add how many minutes in the time range AFTER the store closed that day
			uiMinutesClosed += ( uiEndTime - max( uiClosingTime, uiStartTime ) );
		}
	}
	else
	{
		Assert( uiEndTime < uiStartTime );

		// recursive calls!	Add two separate times: before midnight, and after midnight
		uiMinutesClosed += CalculateMinutesClosedBetween( ubArmsDealer, uiStartTime, NUM_MIN_IN_DAY );
		uiMinutesClosed += CalculateMinutesClosedBetween( ubArmsDealer, 0, uiEndTime );
	}

	return ( uiMinutesClosed );
}
#ifdef JA2UB
//JA25 ub


void AddTexsVideosToBettysInventory()
{
	GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_BETTY, TEX_MOVIE_ATTACK_CLYDESDALES, 1 );
	GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_BETTY, TEX_MOVIE_WILD_EAST, 1 );
	GuaranteeAtLeastXItemsOfIndex( ARMS_DEALER_BETTY, TEX_MOVIE_HAVE_HONDA, 1 );
}

BOOLEAN CanThisItemBeSoldToSimulatedCustomer( UINT8 ubArmsDealerID, UINT16 usItemIndex )
{
	switch( ubArmsDealerID )
	{
		case ARMS_DEALER_BETTY:
			//if the item is..

			if( usItemIndex == TEX_MOVIE_ATTACK_CLYDESDALES ||  //1356  ||
					usItemIndex == TEX_MOVIE_WILD_EAST || //1357  ||
					usItemIndex == TEX_MOVIE_HAVE_HONDA || //1358  ||
					usItemIndex == LAPTOP_TRANSMITTER )//1355  )
			{
				return( FALSE );
			}
			break;
	}

	return( TRUE );
}
#endif
