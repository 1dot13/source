#ifdef PRECOMPILEDHEADERS
	#include "Strategic All.h"
#else
	#include "Strategic Event Handler.h"
	#include "MemMan.h"
	#include "message.h"
	#include "Items.h"
	#include "Handle Items.h"
	#include "LaptopSave.h"
	#include "Tactical Save.h"
	#include "StrategicMap.h"
	#include "Quests.h"
	#include "Soldier Profile.h"
	#include "Game Event Hook.h"
	#include "Game Clock.h"
	#include "Interface Dialogue.h"
	#include "Random.h"
	#include "Overhead.h"
	#include "Strategic Town Loyalty.h"
	#include "Soldier Init List.h"
	#include "SaveLoadMap.h"
	#include "Soldier Create.h"
	#include "Soldier Add.h"
	#include "Renderworld.h"
	#include "Soldier Profile.h"
	#include "email.h"
	#include "strategic.h"
	#include "GameSettings.h"
	#include "history.h"
	#include "Town Militia.h"	// added by Flugente
	#include "Campaign.h"		// added by Flugente
	#include "Strategic AI.h"
	#include "Rebel Command.h"
#endif

#include "Luaglobal.h"
#include "connect.h"
#include "LuaInitNPCs.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;
extern WorldItems gAllWorldItems;


#define		MEDUNA_ITEM_DROP_OFF_GRIDNO			10959
#define		MEDUNA_ITEM_DROP_OFF_SECTOR_X		3
#define		MEDUNA_ITEM_DROP_OFF_SECTOR_Y		14
#define		MEDUNA_ITEM_DROP_OFF_SECTOR_Z		0


extern INT32 gsRobotGridNo;

UINT32 guiPabloExtraDaysBribed = 0;

UINT8		gubCambriaMedicalObjects;

extern INT8 NumMercsNear( UINT8 ubProfileID, UINT8 ubMaxDist );

extern SECTOR_EXT_DATA	SectorExternalData[256][4];

// WANNE: No more used for the new airport code
//void DropOffItemsInMeduna( UINT8 ubOrderNum );

/*
// WANNE: Dieser Code wird nicht mehr ausgef�hrt. Es gibt jetzt das PostalService.cpp::DeliverShipment()
// In der Methode DeliverShipment() fehlt eine ganze Menge (PABLO Quests, ...)
void BobbyRayPurchaseEventCallback( UINT8 ubOrderID )
{
	UINT8	i,j;
	UINT16	usItem;
	INT16 sMapPos, sStandardMapPos;
	UINT16 usNumberOfItems;
	BOOLEAN	fSectorLoaded = FALSE;
	UINT16	usTotalNumberOfItemTypes;
	UINT32	uiCount = 0,uiStolenCount = 0;
	static UINT8 ubShipmentsSinceNoBribes = 0;
	UINT32	uiChanceOfTheft;
	BOOLEAN		fPablosStoleSomething = FALSE;
	BOOLEAN		fPablosStoleLastItem = FALSE;
	OBJECTTYPE	*pObject=NULL;
	OBJECTTYPE	*pStolenObject=NULL;
	BOOLEAN			fThisShipmentIsFromJohnKulba = FALSE;	//if it is, dont add an email
	UINT8		ubItemsDelivered;
	UINT8		ubTempNumItems;
	UINT8		ubItemsPurchased;

	sStandardMapPos = BOBBYR_SHIPPING_DEST_GRIDNO;

	// Haydent
	if(gpNewBobbyrShipments[ ubOrderID ].fActive && is_client)
	{
		DropOffItemsInSector( ubOrderID );//hayden
		return;
	}


	// if the delivery is for meduna, drop the items off there instead
	if( gpNewBobbyrShipments[ ubOrderID ].fActive && gpNewBobbyrShipments[ ubOrderID ].ubDeliveryLoc == BR_MEDUNA )
	{
		DropOffItemsInMeduna( ubOrderID );
		return;
	}

	if (CheckFact( FACT_NEXT_PACKAGE_CAN_BE_LOST, 0 ))
	{
		SetFactFalse( FACT_NEXT_PACKAGE_CAN_BE_LOST );
		if (Random( 100 ) < 50)
		{
			// lose the whole shipment!
			gpNewBobbyrShipments[ ubOrderID ].fActive = FALSE;
			SetFactTrue( FACT_LAST_SHIPMENT_CRASHED );
			return;
		}

	}
	else if (CheckFact( FACT_NEXT_PACKAGE_CAN_BE_DELAYED, 0 ))
	{
		// shipment went to wrong airport... reroute all items to a temporary
		// gridno to represent the other airport (and damage them)
		SetFactTrue( FACT_LAST_SHIPMENT_WENT_TO_WRONG_AIRPORT );
		sStandardMapPos = LOST_SHIPMENT_GRIDNO;
		SetFactFalse( FACT_NEXT_PACKAGE_CAN_BE_DELAYED );
	}
	else if ( (gTownLoyalty[ DRASSEN ].ubRating < 20) || StrategicMap[ CALCULATE_STRATEGIC_INDEX( 13, MAP_ROW_B ) ].fEnemyControlled )
	{
		// loss of the whole shipment
		gpNewBobbyrShipments[ ubOrderID ].fActive = FALSE;

		SetFactTrue( FACT_AGENTS_PREVENTED_SHIPMENT );
		return;
	}

	//Get the number of item types
	usTotalNumberOfItemTypes = gpNewBobbyrShipments[ ubOrderID ].ubNumberPurchases;

	//Must get the total number of items ( all item types plus how many of each item type ordered )
	usNumberOfItems = 0;
	for(i=0; i<gpNewBobbyrShipments[ ubOrderID ].ubNumberPurchases; i++)
	{
		// Count how many items were purchased
		usNumberOfItems += gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].ubNumberPurchased;

		//if any items are AutoMags
		if( gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].usItemIndex == AUTOMAG_III )
		{
			//This shipment is from John Kulba, dont add an email from bobby ray
			fThisShipmentIsFromJohnKulba = TRUE;
		}
	}

	// determine if the sector is loaded
	if( ( gWorldSectorX == BOBBYR_SHIPPING_DEST_SECTOR_X ) && ( gWorldSectorY == BOBBYR_SHIPPING_DEST_SECTOR_Y ) && ( gbWorldSectorZ == BOBBYR_SHIPPING_DEST_SECTOR_Z ) )
		fSectorLoaded = TRUE;
	else
		fSectorLoaded = FALSE;

	// set crate to closed!
	if ( fSectorLoaded )
	{
		SetOpenableStructureToClosed( BOBBYR_SHIPPING_DEST_GRIDNO, 0 );
	}
	else
	{
		ChangeStatusOfOpenableStructInUnloadedSector( BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z, BOBBYR_SHIPPING_DEST_GRIDNO, FALSE );
	}

	//if we are NOT currently in the right sector
	if( !fSectorLoaded )
	{
		//build an array of objects to be added
		pObject = new OBJECTTYPE[ usNumberOfItems ];
		pStolenObject = new OBJECTTYPE[ usNumberOfItems ];
		if( pObject == NULL || pStolenObject == NULL)
			return;
	}

	// WDS - Option to turn off stealing
	// check for potential theft
	if (gGameExternalOptions.fStealingDisabled ||
		CheckFact( FACT_PABLO_WONT_STEAL, 0 ))
	{
		uiChanceOfTheft = 0;
	}
	else if (CheckFact( FACT_PABLOS_BRIBED, 0 ))
	{
		// Since Pacos has some money, reduce record of # of shipments since last bribed...
		ubShipmentsSinceNoBribes /= 2;
		uiChanceOfTheft = 0;
	}
	else
	{
		ubShipmentsSinceNoBribes++;
		// this chance might seem high but it's only applied at most to every second item
		uiChanceOfTheft = 12 + Random( 4 * ubShipmentsSinceNoBribes );
	}

	uiCount=0;
	for ( i=0; i<gpNewBobbyrShipments[ ubOrderID ].ubNumberPurchases; i++ )
	{
		//Get the item
		usItem = gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].usItemIndex;

		//Create the item
		CreateItem( usItem, gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].bItemQuality, &gTempObject );

		// if it's a gun
		if (Item [ usItem ].usItemClass == IC_GUN )
		{
			// Empty out the bullets put in by CreateItem().	We now sell all guns empty of bullets.	This is done for BobbyR
			// simply to be consistent with the dealers in Arulco, who must sell guns empty to prevent ammo cheats by players.
			gTempObject[0]->data.gun.ubGunShotsLeft = 0;
		}

		ubItemsDelivered = 0;

		//add all the items that were purchased
		ubItemsPurchased = gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].ubNumberPurchased;
		for( j=0; j < ubItemsPurchased; j++)
		{
			// Pablos might steal stuff but only:
			// - if it's one of a group of items
			// - if he didn't steal the previous item in the group (so he never steals > 50%)
			// - if he has been bribed, he only sneaks out stuff which is cheap
			if( fSectorLoaded )
			{
				// add ubItemsPurchased to the chance of theft so the chance increases when there are more items of a kind being ordered
				if ( !fPablosStoleLastItem && uiChanceOfTheft > 0 && Random( 100 ) < (uiChanceOfTheft + ubItemsPurchased) )
				{
					uiStolenCount++;
					sMapPos = PABLOS_STOLEN_DEST_GRIDNO; // off screen!
					fPablosStoleSomething = TRUE;
					fPablosStoleLastItem = TRUE;
				}
				else
				{
					sMapPos = sStandardMapPos;
					fPablosStoleLastItem = FALSE;

					if (sStandardMapPos == LOST_SHIPMENT_GRIDNO)
					{
						// damage the item a random amount!
						gTempObject[0]->data.objectStatus = (INT8) ( ( (70 + Random( 11 )) * (INT32) gTempObject[0]->data.objectStatus ) / 100 );
						// make damn sure it can't hit 0
						if (gTempObject[0]->data.objectStatus == 0)
						{
							gTempObject[0]->data.objectStatus = 1;
						}
						AddItemToPool( sMapPos, &gTempObject, -1, 0, 0, 0 );
					}
					else
					{
						// record # delivered for later addition...
						ubItemsDelivered++;
					}
				}
			}
			else
			{
				if ( j > 1 && !fPablosStoleLastItem && uiChanceOfTheft > 0 && Random( 100 ) < (uiChanceOfTheft + j) )
				{
					pStolenObject[ uiStolenCount ] = gTempObject;
					uiStolenCount++;
					fPablosStoleSomething = TRUE;
					fPablosStoleLastItem = TRUE;
				}
				else
				{
					fPablosStoleLastItem = FALSE;

					//else we are not currently in the sector, so we build an array of items to add in one lump
					//add the item to the item array

					if (sStandardMapPos == LOST_SHIPMENT_GRIDNO)
					{
						// damage the item a random amount!
						gTempObject[0]->data.objectStatus = (INT8) ( ( (70 + Random( 11 )) * (INT32) gTempObject[0]->data.objectStatus ) / 100 );
						// make damn sure it can't hit 0
						if (gTempObject[0]->data.objectStatus == 0)
						{
							gTempObject[0]->data.objectStatus = 1;
						}
						pObject[ uiCount ] = gTempObject;
						uiCount++;
					}
					else
					{
						ubItemsDelivered++;
					}
				}
			}
		}

		if ( gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].ubNumberPurchased == 1 && ubItemsDelivered == 1 )
		{
			// the item in gTempObject will be the item to deliver
			if( fSectorLoaded )
			{
				AddItemToPool( sStandardMapPos, &gTempObject, -1, 0, 0, 0 );
			}
			else
			{
				pObject[ uiCount ] = gTempObject;
				uiCount++;
			}
		}
		else
		{
			while ( ubItemsDelivered )
			{
				// treat 0s as 1s :-)
				ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&gTempObject, STACK_SIZE_LIMIT) );
				CreateItems( usItem, gpNewBobbyrShipments[ ubOrderID ].BobbyRayPurchase[i].bItemQuality, ubTempNumItems, &gTempObject );

				// stack as many as possible
				if( fSectorLoaded )
				{
					AddItemToPool( sStandardMapPos, &gTempObject, -1, 0, 0, 0 );
				}
				else
				{
					pObject[ uiCount ] = gTempObject;
					uiCount++;
				}

				ubItemsDelivered -= ubTempNumItems;
			}
		}
	}

	//if we are NOT currently in the sector
	if( !fSectorLoaded )
	{
		//add all the items from the array that was built above
		sMapPos = PABLOS_STOLEN_DEST_GRIDNO;
		//The item are to be added to the Top part of Drassen, grid loc's	10112, 9950
		if( !AddItemsToUnLoadedSector( BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z, sStandardMapPos, uiCount, pObject, 0, 0, 0, -1, FALSE ) )
		{
			//Error adding the items
			//return;
		}
		if (uiStolenCount > 0)
		{
			if( !AddItemsToUnLoadedSector( BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z, PABLOS_STOLEN_DEST_GRIDNO, uiStolenCount, pStolenObject, 0, 0, 0, -1, FALSE ) )
			{
				//Error adding the items
				//return;
			}
		}
		delete[] pObject ;
		delete[]( pStolenObject );
		pObject = NULL;
		pStolenObject = NULL;
	}

	if (fPablosStoleSomething)
	{
		SetFactTrue( FACT_PABLOS_STOLE_FROM_LATEST_SHIPMENT );
	}
	else
	{
		SetFactFalse( FACT_PABLOS_STOLE_FROM_LATEST_SHIPMENT );
	}

	SetFactFalse( FACT_PLAYER_FOUND_ITEMS_MISSING );

	SetFactFalse( FACT_LARGE_SIZED_OLD_SHIPMENT_WAITING );

	if ( CheckFact( FACT_NEXT_PACKAGE_CAN_BE_DELAYED, 0 ) )
	{
		SetFactFalse( FACT_MEDIUM_SIZED_SHIPMENT_WAITING );
		SetFactFalse( FACT_LARGE_SIZED_SHIPMENT_WAITING );
		SetFactFalse( FACT_REALLY_NEW_BOBBYRAY_SHIPMENT_WAITING );
	}
	else
	{
		if (usNumberOfItems - uiStolenCount <= 5)
		{
			SetFactFalse( FACT_MEDIUM_SIZED_SHIPMENT_WAITING );
			SetFactFalse( FACT_LARGE_SIZED_SHIPMENT_WAITING );
		}
		else if (usNumberOfItems - uiStolenCount <= 15)
		{
			SetFactTrue( FACT_MEDIUM_SIZED_SHIPMENT_WAITING );
			SetFactFalse( FACT_LARGE_SIZED_SHIPMENT_WAITING );
		}
		else
		{
			SetFactFalse( FACT_MEDIUM_SIZED_SHIPMENT_WAITING );
			SetFactTrue( FACT_LARGE_SIZED_SHIPMENT_WAITING );
		}

		// this shipment isn't old yet...
		SetFactTrue( FACT_REALLY_NEW_BOBBYRAY_SHIPMENT_WAITING );

		// set up even to make shipment "old"
		AddSameDayStrategicEvent( EVENT_SET_BY_NPC_SYSTEM, GetWorldMinutesInDay() + 120, FACT_REALLY_NEW_BOBBYRAY_SHIPMENT_WAITING );
	}

	//We have received the shipment so fActice becomes fALSE
	gpNewBobbyrShipments[ ubOrderID ].fActive = FALSE;

	//Stop time compression the game
	StopTimeCompression( );

	//if the shipment is NOT from John Kulba, send an email
	if( !fThisShipmentIsFromJohnKulba )
	{
		//Add an email from Bobby r telling the user the shipment 'Should' be there
		AddEmail( BOBBYR_SHIPMENT_ARRIVED, BOBBYR_SHIPMENT_ARRIVED_LENGTH, BOBBY_R, GetWorldTotalMin(), -1 );
	}
	else
	{
		//if the shipment is from John Kulba

		//Add an email from kulba telling the user the shipment is there
		AddEmail( JOHN_KULBA_GIFT_IN_DRASSEN, JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1 );
	}
}
*/


void HandleDelayedItemsArrival( UINT32 uiReason )
{

//	LuaHandleDelayedItemsArrival( uiReason, 0);

	// This function moves all the items that Pablos has stolen
	// (or items that were delayed) to the arrival location for new shipments,
	INT32			sStartGridNo;
	UINT8			ubLoop;

	if (uiReason == NPC_SYSTEM_EVENT_ACTION_PARAM_BONUS + NPC_ACTION_RETURN_STOLEN_SHIPMENT_ITEMS )
	{
		if ( gMercProfiles[ PABLO ].bMercStatus == MERC_IS_DEAD )
		{
			// nothing arrives then!
			return;
		}
		// update some facts...
		SetFactTrue( FACT_PABLO_RETURNED_GOODS );
		SetFactFalse( FACT_PABLO_PUNISHED_BY_PLAYER );
		sStartGridNo = PABLOS_STOLEN_DEST_GRIDNO;

		// add random items

		for (ubLoop = 0; ubLoop < 2; ubLoop++)
		{
			switch( Random( 10 ) )
			{
				case 0:
					// 1 in 10 chance of a badly damaged gas mask
					CreateItem( GASMASK, (INT8) (20 + Random( 10 )), &gTempObject );
					break;
				case 1:
				case 2:
					// 2 in 10 chance of a battered Desert Eagle
					CreateItem( DESERTEAGLE, (INT8) (40 + Random( 10 )), &gTempObject );
					break;
				case 3:
				case 4:
				case 5:
					// 3 in 10 chance of a stun grenade
					CreateItem( STUN_GRENADE, (INT8) (70 + Random( 10 )), &gTempObject );
					break;
				case 6:
				case 7:
				case 8:
				case 9:
					// 4 in 10 chance of two 38s!
					CreateItems( SW38, (INT8) (90 + Random( 10 )), 2, &gTempObject );
					break;
			}
			if ( ( gWorldSectorX == BOBBYR_SHIPPING_DEST_SECTOR_X ) && ( gWorldSectorY == BOBBYR_SHIPPING_DEST_SECTOR_Y ) && ( gbWorldSectorZ == BOBBYR_SHIPPING_DEST_SECTOR_Z ) )
			{
				AddItemToPool( BOBBYR_SHIPPING_DEST_GRIDNO, &gTempObject, -1, 0, 0, 0 );
			}
			else
			{
				AddItemsToUnLoadedSector( BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z, BOBBYR_SHIPPING_DEST_GRIDNO, 1, &gTempObject, 0, 0, 0, -1, FALSE );
			}
		}
	}
	else if (uiReason == FACT_PACKAGE_DAMAGED)
	{
		sStartGridNo = LOST_SHIPMENT_GRIDNO;
	}
	else
	{
		return;
	}

	// If the Drassen airport sector is already loaded, move the item pools...
	if ( ( gWorldSectorX == BOBBYR_SHIPPING_DEST_SECTOR_X ) && ( gWorldSectorY == BOBBYR_SHIPPING_DEST_SECTOR_Y ) && ( gbWorldSectorZ == BOBBYR_SHIPPING_DEST_SECTOR_Z ) )
	{
		// sector is loaded!
		// just move the hidden item pool
		MoveItemPools( sStartGridNo, BOBBYR_SHIPPING_DEST_GRIDNO, 0, 0 );
	}
	else
	{
		// otherwise load the saved items from the item file and change the records of their locations
		std::vector<WORLDITEM> pTemp;
		UINT32 uiNumWorldItems = 0;

		const auto ii = FindWorldItemSector(BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z);
		if (ii != -1)
		{
			uiNumWorldItems = gAllWorldItems.NumItems[ii];
			pTemp = gAllWorldItems.Items[ii];
		}
		
		for (UINT32 uiLoop = 0; uiLoop < uiNumWorldItems; uiLoop++)
		{
			if (pTemp[uiLoop].sGridNo == PABLOS_STOLEN_DEST_GRIDNO)
			{
				pTemp[uiLoop].sGridNo = BOBBYR_SHIPPING_DEST_GRIDNO;
			}
		}
		AddWorldItemsToUnLoadedSector( BOBBYR_SHIPPING_DEST_SECTOR_X, BOBBYR_SHIPPING_DEST_SECTOR_Y, BOBBYR_SHIPPING_DEST_SECTOR_Z, 0, uiNumWorldItems, pTemp, TRUE );
	}
}

void AddSecondAirportAttendant( void )
{

//LetLuaHandleEarlyMorningEvents(2);
//#if 0
	// add the second airport attendant to the Drassen airport...
	gMercProfiles[99].sSectorX = BOBBYR_SHIPPING_DEST_SECTOR_X;
	gMercProfiles[99].sSectorY = BOBBYR_SHIPPING_DEST_SECTOR_Y;
	gMercProfiles[99].bSectorZ = BOBBYR_SHIPPING_DEST_SECTOR_Z;
//#endif
}

void SetPabloToUnbribed( void )
{
//	LetLuaHandleEarlyMorningEvents(3);
//#if 0
	if (guiPabloExtraDaysBribed > 0)
	{
		// set new event for later on, because the player gave Pablo more money!
		AddFutureDayStrategicEvent( EVENT_SET_BY_NPC_SYSTEM, GetWorldMinutesInDay(), FACT_PABLOS_BRIBED, guiPabloExtraDaysBribed );
		guiPabloExtraDaysBribed = 0;
	}
	else
	{
		SetFactFalse( FACT_PABLOS_BRIBED );
	}
//#endif
}

void HandlePossiblyDamagedPackage( void )
{
//	LetLuaHandleEarlyMorningEvents(1);
//#if 0
	if (Random( 100 ) < 70)
	{
		SetFactTrue( FACT_PACKAGE_DAMAGED );
		HandleDelayedItemsArrival( FACT_PACKAGE_DAMAGED );
	}
	else
	{
		// shipment lost forever!
		SetFactTrue( FACT_PACKAGE_LOST_PERMANENTLY );
	}
	// whatever happened, the shipment is no longer delayed
	SetFactFalse( FACT_SHIPMENT_DELAYED_24_HOURS );
//#endif
}

void CheckForKingpinsMoneyMissing( BOOLEAN fFirstCheck )
{

//	LuaCheckForKingpinsMoneyMissing ( fFirstCheck , 0 );
//#if 0
	UINT32				uiLoop;
	UINT32				uiTotalCash = 0;
	BOOLEAN				fKingpinWillDiscover = FALSE, fKingpinDiscovers = FALSE;

	// money in D5b1 must be less than 30k

	for ( uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ )
	{
		// loop through all items, look for ownership
		if ( gWorldItems[ uiLoop ].fExists && gWorldItems[ uiLoop ].object.usItem == MONEY )
		{
			uiTotalCash += gWorldItems[uiLoop].object[0]->data.money.uiMoneyAmount;
		}
	}

	// This function should be called every time sector D5/B1 is unloaded!
	if ( fFirstCheck )
	{
		if ( CheckFact( FACT_KINGPIN_WILL_LEARN_OF_MONEY_GONE, 0 ) == TRUE )
		{
			// unnecessary
			return;
		}

		if ( uiTotalCash < 30000 )
		{
			// add history log here
			AddHistoryToPlayersLog( HISTORY_FOUND_MONEY, 0, GetWorldTotalMin(), gWorldSectorX, gWorldSectorY );

			SetFactTrue( FACT_KINGPIN_WILL_LEARN_OF_MONEY_GONE );
		}
	}

	if ( CheckFact( FACT_KINGPIN_DEAD, 0 ) == TRUE )
	{
		return;
	}

	if ( uiTotalCash < 30000 )
	{
		if ( fFirstCheck )
		{
			// add event to make Kingpin aware, two days from now
			fKingpinWillDiscover = TRUE;
		}
		else
		{
			fKingpinDiscovers = TRUE;
		}
	}

	if ( fKingpinWillDiscover )
	{
		// set event for next day to check for real
		AddFutureDayStrategicEvent( EVENT_SET_BY_NPC_SYSTEM, Random( 120 ), FACT_KINGPIN_KNOWS_MONEY_GONE, 1 );

		// the sector is unloaded NOW so set Kingpin's balance and remove the cash
		gMercProfiles[ KINGPIN ].iBalance = - (30000 - (INT32) uiTotalCash);
		// remove all money from map
		for ( uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ )
		{
			// loop through all items, look for ownership
			if ( gWorldItems[ uiLoop ].fExists && gWorldItems[ uiLoop ].object.usItem == MONEY )
			{
				// remove!
				gWorldItems[ uiLoop ].fExists = FALSE;
			}
		}
	}
	else if ( fKingpinDiscovers )
	{
		// ok start things up here!
		SetFactTrue( FACT_KINGPIN_KNOWS_MONEY_GONE );

		// set event 2 days from now that if the player has not given Kingpin his money back,
		// he sends email to the player
		AddFutureDayStrategicEvent( EVENT_SET_BY_NPC_SYSTEM, Random( 120 ), FACT_KINGPIN_KNOWS_MONEY_GONE, 2 );
	}
//#endif
}

void HandleNPCSystemEvent( UINT32 uiEvent )
{

//   LetLuaHandleNPCSystemEvent( uiEvent, 0 );
//#if 0
	if (uiEvent < NPC_SYSTEM_EVENT_ACTION_PARAM_BONUS)
	{
		switch( uiEvent )
		{
			case FACT_PABLOS_BRIBED:
				// set Pacos to unbribed
				SetPabloToUnbribed();
				break;

			case FACT_REALLY_NEW_BOBBYRAY_SHIPMENT_WAITING:
				// the shipment is no longer really new
				SetFactFalse( FACT_REALLY_NEW_BOBBYRAY_SHIPMENT_WAITING );
				if (CheckFact( FACT_LARGE_SIZED_SHIPMENT_WAITING, 0 ))
				{
					// set "really heavy old shipment" fact
					SetFactTrue( FACT_LARGE_SIZED_OLD_SHIPMENT_WAITING );
				}
				break;

			case FACT_SHIPMENT_DELAYED_24_HOURS:
			case FACT_24_HOURS_SINCE_DOCTOR_TALKED_TO:
			case FACT_24_HOURS_SINCE_JOEY_RESCUED:
				SetFactTrue( (UINT16) uiEvent );
				break;

			case FACT_KINGPIN_KNOWS_MONEY_GONE:
				// more generally events for kingpin quest
				if ( CheckFact( FACT_KINGPIN_KNOWS_MONEY_GONE, 0 ) == FALSE )
				{
					// check for real whether to start quest
					CheckForKingpinsMoneyMissing( FALSE );
				}
				else if ( CheckFact( FACT_KINGPIN_DEAD, 0 ) == FALSE )
				{
					if ( gubQuest[ QUEST_KINGPIN_MONEY ] == QUESTNOTSTARTED )
					{
						// KP knows money is gone, hasn't told player, if this event is called then the 2
						// days are up... send email

#ifdef JA2UB
// no UB
#else						
						AddEmail( KING_PIN_LETTER, KING_PIN_LETTER_LENGTH, KING_PIN, GetWorldTotalMin(), -1, -1 , TYPE_EMAIL_EMAIL_EDT );
#endif
						StartQuest( QUEST_KINGPIN_MONEY, gMercProfiles[ KINGPIN ].sSectorX, gMercProfiles[ KINGPIN ].sSectorY );
						// add event to send terrorists two days from now
						AddFutureDayStrategicEvent( EVENT_SET_BY_NPC_SYSTEM, Random( 120 ), FACT_KINGPIN_KNOWS_MONEY_GONE, 2 );
					}
					else if ( gubQuest[ QUEST_KINGPIN_MONEY ] == QUESTINPROGRESS )
					{
						// knows money gone, quest is still in progress
						// event indicates Kingpin can start to send terrorists
						SetFactTrue( FACT_KINGPIN_CAN_SEND_ASSASSINS );
						gMercProfiles[ SPIKE ].sSectorX = gModSettings.ubSpikeNewSectorX; //5
						gMercProfiles[ SPIKE ].sSectorY = gModSettings.ubSpikeNewSectorY; //MAP_ROW_C
						gMercProfiles[ SPIKE ].bSectorZ = gModSettings.ubSpikeNewSectorZ; //0
						gTacticalStatus.fCivGroupHostile[ KINGPIN_CIV_GROUP ] = CIV_GROUP_WILL_BECOME_HOSTILE;
					}
				}
				break;
		}
	}
	else
	{
		switch( uiEvent - NPC_SYSTEM_EVENT_ACTION_PARAM_BONUS )
		{
			case NPC_ACTION_RETURN_STOLEN_SHIPMENT_ITEMS:
				HandleDelayedItemsArrival( uiEvent );
				break;
			case NPC_ACTION_SET_RANDOM_PACKAGE_DAMAGE_TIMER:
				HandlePossiblyDamagedPackage();
				break;
			case NPC_ACTION_ENABLE_CAMBRIA_DOCTOR_BONUS:
				SetFactTrue( FACT_WILLIS_HEARD_ABOUT_JOEY_RESCUE );
				break;
			case NPC_ACTION_TRIGGER_END_OF_FOOD_QUEST:
				if ( gMercProfiles[ FATHER ].bMercStatus != MERC_IS_DEAD )
				{
					EndQuest( QUEST_FOOD_ROUTE, 10, MAP_ROW_A );
					SetFactTrue( FACT_FOOD_QUEST_OVER );

					RebelCommand::ShowWebsiteAvailableMessage();
				}
				break;
			case NPC_ACTION_DELAYED_MAKE_BRENDA_LEAVE:
				// silversurfer: reenabled this
				// but only play Brendas quote when we are near enough to hear her
				if ( NumMercsNear( 85, 8 ) > 0 )
					TriggerNPCRecord( 85, 9 );
				SetFactTrue( FACT_BRENDA_PATIENCE_TIMER_EXPIRED );
				break;
			case NPC_ACTION_SET_DELAY_TILL_GIRLS_AVAILABLE:
				HandleNPCDoAction( 107, NPC_ACTION_SET_GIRLS_AVAILABLE, 0 );
				break;

			case NPC_ACTION_READY_ROBOT:
				{
					if ( CheckFact( FACT_FIRST_ROBOT_DESTROYED, 0 ) )
					{
						// second robot ready
						SetFactTrue( FACT_ROBOT_READY_SECOND_TIME );
						// resurrect robot
						gMercProfiles[ ROBOT ].bLife = gMercProfiles[ ROBOT ].bLifeMax;
						gMercProfiles[ ROBOT ].bMercStatus = MERC_OK;
					}
					else
					{
						// first robot ready
						SetFactTrue( FACT_ROBOT_READY );
					}

					gMercProfiles[ ROBOT ].sSectorX = gMercProfiles[ MADLAB ].sSectorX;
					gMercProfiles[ ROBOT ].sSectorY = gMercProfiles[ MADLAB ].sSectorY;
					gMercProfiles[ ROBOT ].bSectorZ = gMercProfiles[ MADLAB ].bSectorZ;


				}
				break;

			case NPC_ACTION_ADD_JOEY_TO_WORLD:
				// If Joey is not dead, escorted, or already delivered
				if ( gMercProfiles[ JOEY ].bMercStatus != MERC_IS_DEAD && !CheckFact( FACT_JOEY_ESCORTED, 0 ) &&
					gMercProfiles[ JOEY ].sSectorX == gModSettings.ubJoeyPrimarySectorX && //4
					gMercProfiles[ JOEY ].sSectorY == gModSettings.ubJoeyPrimarySectorY && //MAP_ROW_D
					gMercProfiles[ JOEY ].bSectorZ == gModSettings.ubJoeyPrimarySectorZ ) //1
				{
					SOLDIERTYPE * pJoey;

					pJoey = FindSoldierByProfileID( JOEY, FALSE );
					if (pJoey )
					{
						// he's in the currently loaded sector...delay this an hour!
						AddSameDayStrategicEvent( EVENT_SET_BY_NPC_SYSTEM, GetWorldMinutesInDay() + 60, NPC_SYSTEM_EVENT_ACTION_PARAM_BONUS + NPC_ACTION_ADD_JOEY_TO_WORLD );
					}
					else
					{
						// move Joey from caves to San Mona
						gMercProfiles[ JOEY ].sSectorX = gModSettings.ubJoeyAlternateSectorX; //5
						gMercProfiles[ JOEY ].sSectorY = gModSettings.ubJoeyAlternateSectorY; //MAP_ROW_C
						gMercProfiles[ JOEY ].bSectorZ = gModSettings.ubJoeyAlternateSectorZ; //0
					}
				}
				break;

			case NPC_ACTION_SEND_ENRICO_MIGUEL_EMAIL:
#ifdef JA2UB
// no UB
#else
				AddEmail( ENRICO_MIGUEL, ENRICO_MIGUEL_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1 , TYPE_EMAIL_EMAIL_EDT);
#endif
				break;

			case NPC_ACTION_TIMER_FOR_VEHICLE:
				SetFactTrue( FACT_OK_USE_HUMMER );
				break;

			case NPC_ACTION_FREE_KIDS:
				SetFactTrue( FACT_KIDS_ARE_FREE );
				break;

			default:
				break;
		}
	}
	
//#endif
}

static INT32 TakeVolunteers( UINT8 poolRecruitPerc )
{
	INT32 loyalPopulation = 0;

	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].fEnemyControlled )  // if owned by the Army
			{
				UINT8 ubTownID = StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].bNameId;
				if ( ubTownID != BLANK_SECTOR )
				{
					// Supposing rebel sentiment population will never be volunteers, Queen's or neutral rating % should be greater
					// than sentiment %. Otherwise there is no volunteer people for sure.
					// So if there are Queen loyal population, take sector population into account.
					INT32 loyalPercent = MAX_LOYALTY_VALUE - gTownLoyalty[ubTownID].ubRating - gubTownRebelSentiment[ubTownID];
					if ( loyalPercent > 0 )
					{
						loyalPopulation += GetSectorPopulation( sX, sY, FALSE ) * loyalPercent / 100;
					}
				}
			}
		}
	}

	return loyalPopulation * poolRecruitPerc / 100;
}

static INT32 TakeRecruitsByForce( UINT8 poolRecruitPerc, INT32 maxToTake )
{
	INT32 recruits = 0;

	if ( poolRecruitPerc != 0 )
	{
		for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
		{
			if ( recruits >= maxToTake )
				break;

			for ( UINT8 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
			{
				if ( recruits >= maxToTake )
					break;

				SECTORINFO *pSectorInfo = &( SectorInfo[SECTOR( sX, sY )] );
				if ( StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].fEnemyControlled && pSectorInfo->ubGarrisonID != NO_GARRISON )
				{
					UINT8 ubTownID = StrategicMap[CALCULATE_STRATEGIC_INDEX( sX, sY )].bNameId;
					if ( ubTownID != BLANK_SECTOR )
					{
						recruits += max( 1, GetSectorPopulation( sX, sY, FALSE ) * poolRecruitPerc / 100 );

						// A possible drawback of forced recruiting is percent of unresting people increases.
						// So increase Player's loyalty on 'poolRecruitPerc' value, but only if Player is known in this town.
						if ( gTownLoyalty[ubTownID].ubRating > 0 )
						{
							gTownLoyalty[ubTownID].ubRating = min( MAX_LOYALTY_VALUE, gTownLoyalty[ubTownID].ubRating + poolRecruitPerc );
						}
					}
				}
			}
		}
	}

	return min( recruits, maxToTake );
}

void HandleEarlyMorningEvents( void )
{
	// Flugente: no reason to put this into LUA
	for ( UINT8 sX = 1; sX < MAP_WORLD_X - 1; ++sX )
	{
		for ( UINT8 sY = 1; sY < MAP_WORLD_Y - 1; ++sY )
		{
			UINT8 sector = SECTOR( sX, sY );

			SECTORINFO *pSectorInfo = &(SectorInfo[sector]);
			
			// A flag prevents us from exploiting the civilian recruitment mechanic. Every x hours, we reset it
			if ( pSectorInfo )
			{
				pSectorInfo->usSectorInfoFlag &= ~SECTORINFO_VOLUNTEERS_RECENTLY_RECRUITED;
			}
		}
	}

	UINT16 poolIncDays = zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolIncrementDaysPerDifficultyLevel;
	if ( !gfUnlimitedTroops && poolIncDays != 0 )
	{
		INT32 poolMaxSize = zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolMaxSizePerDifficultyLevel;
		INT32 poolIncSize = zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolBaseIncrementSizePerDifficultyLevel;
		UINT8 poolRecruitPerc = zDiffSetting[gGameOptions.ubDifficultyLevel].iQueenPoolRecruitPercentPerDifficultyLevel;

		if ( giReinforcementPool < poolMaxSize )
		{
			INT32 newRecruitsTaken = 0;
			INT32 trainingCapacity = 0;  // as first, count up training camps, then multiply it on QueenPoolBaseIncrementSizePerDifficultyLevel

			if ( IsTownUnderCompleteControlByPlayer( ALMA ) == FALSE )
			{
				++trainingCapacity;
			}
			if ( IsTownUnderCompleteControlByPlayer( MEDUNA ) == FALSE )
			{
				++trainingCapacity;
			}

			trainingCapacity *= poolIncSize;

			if ( giTotalRecruitsInTraining < trainingCapacity )
			{
				newRecruitsTaken += min( TakeVolunteers( poolRecruitPerc ), trainingCapacity - giTotalRecruitsInTraining );

				if ( giTotalRecruitsInTraining + newRecruitsTaken < trainingCapacity )
				{
					INT32 maxRecruitsToTake = trainingCapacity - giTotalRecruitsInTraining - newRecruitsTaken;
					newRecruitsTaken += TakeRecruitsByForce( poolRecruitPerc, maxRecruitsToTake );
				}

				if ( newRecruitsTaken > 0 )
				{
					UINT32 endTimeMin = GetWorldTotalMin() + poolIncDays * NUM_MIN_IN_DAY - 1;  // -1 minute: training should end right before a new recruiting session, otherwise we may ignore some training space at moment of EarlyMorningEvent
					AddStrategicEvent( EVENT_ARMY_FINISH_TRAINING, endTimeMin, (UINT32)newRecruitsTaken );
					giTotalRecruitsInTraining += newRecruitsTaken;
				}
			}
		}
	}

#ifdef LUA_STRATEGY_EVENT_HANDLER
	LetLuaHandleEarlyMorningEvents(0);
#else

	UINT32					cnt;
	UINT32					uiAmount;

	// loop through all *NPCs* and reset "default response used recently" flags
	for (cnt = 0; cnt < NUM_PROFILES; ++cnt)
	{
		if ( gMercProfiles[cnt].Type == PROFILETYPE_RPC ||
			gMercProfiles[cnt].Type == PROFILETYPE_NPC ||
			gMercProfiles[cnt].Type == PROFILETYPE_VEHICLE )
		{
			gMercProfiles[cnt].bFriendlyOrDirectDefaultResponseUsedRecently = FALSE;
			gMercProfiles[cnt].bRecruitDefaultResponseUsedRecently = FALSE;
			gMercProfiles[cnt].bThreatenDefaultResponseUsedRecently = FALSE;
			gMercProfiles[cnt].ubMiscFlags2 &= (~PROFILE_MISC_FLAG2_BANDAGED_TODAY);
		}
	}
	// reset Father Walker's drunkenness level!
	gMercProfiles[ FATHER ].bNPCData = (INT8) Random( 4 );
	// set Walker's location
	if ( Random( 2 ) )
	{
		// move the father to the other sector, provided neither are loaded
		if ( ! ( ( gWorldSectorX == 13) && ( ( gWorldSectorY == MAP_ROW_C) || gWorldSectorY == MAP_ROW_D ) && ( gbWorldSectorZ == 0 ) ) )
		{
			gMercProfiles[ FATHER ].sSectorX = 13;
			// swap his location
			if (gMercProfiles[ FATHER ].sSectorY == MAP_ROW_C)
			{
				gMercProfiles[ FATHER ].sSectorY = MAP_ROW_D;
			}
			else
			{
				gMercProfiles[ FATHER ].sSectorY = MAP_ROW_C;
			}
		}
	}

	if( gMercProfiles[ TONY ].ubLastDateSpokenTo > 0 && !( gWorldSectorX == 5 && gWorldSectorY == MAP_ROW_C && gbWorldSectorZ == 0 ) )
	{
		// San Mona C5 is not loaded so make Tony possibly not availableif 
		if (Random( 99 ) < gGameExternalOptions.ubChanceTonyAvailable) // silversurfer/SANDRO
		{
			// Tony IS available
			SetFactFalse( FACT_TONY_NOT_AVAILABLE );
			gMercProfiles[ TONY ].sSectorX = 5;
			gMercProfiles[ TONY ].sSectorY = MAP_ROW_C;
		}
		else
		{
			// Tony is NOT available
			SetFactTrue( FACT_TONY_NOT_AVAILABLE );
			gMercProfiles[ TONY ].sSectorX = 0;
			gMercProfiles[ TONY ].sSectorY = 0;
		}
	}

	if ( gMercProfiles[ DEVIN ].ubLastDateSpokenTo == 0 )
	{
		// Does Devin move?
		gMercProfiles[ DEVIN ].bNPCData++;
		if ( gMercProfiles[ DEVIN ].bNPCData > 3 )
		{
			if ( ! ( (gWorldSectorX == gMercProfiles[ DEVIN ].sSectorX) && (gWorldSectorY == gMercProfiles[DEVIN].sSectorY) && (gbWorldSectorZ == 0) ) )
			{
				// ok, Devin's sector not loaded, so time to move!
				// might be same sector as before, if so, oh well!
				switch( Random( 5 ) )
				{
					case 0:
						gMercProfiles[ DEVIN ].sSectorX = 9;
						gMercProfiles[ DEVIN ].sSectorY = MAP_ROW_G;
						break;
					case 1:
						gMercProfiles[ DEVIN ].sSectorX = 13;
						gMercProfiles[ DEVIN ].sSectorY = MAP_ROW_D;
						break;
					case 2:
						gMercProfiles[ DEVIN ].sSectorX = 5;
						gMercProfiles[ DEVIN ].sSectorY = MAP_ROW_C;
						break;
					case 3:
						gMercProfiles[ DEVIN ].sSectorX = 2;
						gMercProfiles[ DEVIN ].sSectorY = MAP_ROW_H;
						break;
					case 4:
						gMercProfiles[ DEVIN ].sSectorX = 6;
						gMercProfiles[ DEVIN ].sSectorY = MAP_ROW_C;
						break;
				}
			}
		}
	}

	// Does Hamous move?

	// stop moving the truck if Hamous is dead!!
	// stop moving them if the player has the truck or Hamous is hired!
	if ( gMercProfiles[ HAMOUS ].bLife > 0 && FindSoldierByProfileID( HAMOUS, TRUE ) == NULL && FindSoldierByProfileID( PROF_ICECREAM, TRUE ) == NULL && (! ( (gWorldSectorX == gMercProfiles[ HAMOUS ].sSectorX) && (gWorldSectorY == gMercProfiles[HAMOUS].sSectorY) && (gbWorldSectorZ == 0) )) )
	{
		// ok, HAMOUS's sector not loaded, so time to move!
		// might be same sector as before, if so, oh well!
		switch( Random( 5 ) )
		{
			case 0:
				gMercProfiles[ HAMOUS ].sSectorX = 6;
				gMercProfiles[ HAMOUS ].sSectorY = MAP_ROW_G;
				gMercProfiles[ PROF_ICECREAM ].sSectorX = 6;
				gMercProfiles[ PROF_ICECREAM ].sSectorY = MAP_ROW_G;
				break;
			case 1:
				gMercProfiles[ HAMOUS ].sSectorX = 12;
				gMercProfiles[ HAMOUS ].sSectorY = MAP_ROW_F;
				gMercProfiles[ PROF_ICECREAM ].sSectorX = 12;
				gMercProfiles[ PROF_ICECREAM ].sSectorY = MAP_ROW_F;
				break;
			case 2:
				gMercProfiles[ HAMOUS ].sSectorX = 7;
				gMercProfiles[ HAMOUS ].sSectorY = MAP_ROW_D;
				gMercProfiles[ PROF_ICECREAM ].sSectorX = 7;
				gMercProfiles[ PROF_ICECREAM ].sSectorY = MAP_ROW_D;
				break;
			case 3:
				gMercProfiles[ HAMOUS ].sSectorX = 3;
				gMercProfiles[ HAMOUS ].sSectorY = MAP_ROW_D;
				gMercProfiles[ PROF_ICECREAM ].sSectorX = 3;
				gMercProfiles[ PROF_ICECREAM ].sSectorY = MAP_ROW_D;
				break;
			case 4:
				gMercProfiles[ HAMOUS ].sSectorX = 9;
				gMercProfiles[ HAMOUS ].sSectorY = MAP_ROW_D;
				gMercProfiles[ PROF_ICECREAM ].sSectorX = 9;
				gMercProfiles[ PROF_ICECREAM ].sSectorY = MAP_ROW_D;
				break;
		}
	}

	// Does Rat take off?
	if ( gMercProfiles[ RAT ].bNPCData != 0 )
	{
		gMercProfiles[ RAT ].sSectorX = 0;
		gMercProfiles[ RAT ].sSectorY = 0;
		gMercProfiles[ RAT ].bSectorZ = 0;
	}

	// Does John leave country after quest completion?
	if ( gubQuest[ QUEST_ESCORT_TOURISTS ] == QUESTDONE && (gMercProfiles[ JOHN ].sSectorX == 13) &&
		(gMercProfiles[ JOHN ].sSectorY == MAP_ROW_B) && (gMercProfiles[ JOHN ].bSectorZ == 0) )
	{
		gMercProfiles[ JOHN ].sSectorX = 0;
		gMercProfiles[ JOHN ].sSectorY = 0;
		gMercProfiles[ JOHN ].bSectorZ = 0;
	}

	// Does Mary leave country after quest completion?
	if ( gubQuest[ QUEST_ESCORT_TOURISTS ] == QUESTDONE && (gMercProfiles[ MARY ].sSectorX == 13) &&
		(gMercProfiles[ MARY ].sSectorY == MAP_ROW_B) && (gMercProfiles[ MARY ].bSectorZ == 0) )
	{
		gMercProfiles[ MARY ].sSectorX = 0;
		gMercProfiles[ MARY ].sSectorY = 0;
		gMercProfiles[ MARY ].bSectorZ = 0;
	}

	// Empty money from pockets of Vince 69, Willis 80, and Jenny 132
	SetMoneyInSoldierProfile( VINCE, 0 );
	SetMoneyInSoldierProfile( STEVE, 0 ); // Steven Willis
	SetMoneyInSoldierProfile( JENNY, 0 );

	// Vince is no longer expecting money
	SetFactFalse( FACT_VINCE_EXPECTING_MONEY );

	// Reset Darren's balance and money
	gMercProfiles[ DARREN ].iBalance = 0;
	SetMoneyInSoldierProfile( DARREN, 15000 );

	// set Carmen to be placed on the map in case he moved and is waiting off screen
	if (gMercProfiles[ CARMEN ].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR)
	{
		gMercProfiles[ CARMEN ].ubMiscFlags2 &= ~(PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR);
		// move Carmen to C13
		gMercProfiles[ CARMEN ].sSectorX = 13;
		gMercProfiles[ CARMEN ].sSectorY = MAP_ROW_C;
		gMercProfiles[ CARMEN ].bSectorZ = 0;

		// we should also reset # of terrorist heads and give him cash
		if (gMercProfiles[ CARMEN ].bNPCData2 > 0)
		{
			if (gMercProfiles[ CARMEN ].uiMoney < 10000)
			{
				uiAmount = 0;
			}
			else
			{
				uiAmount = gMercProfiles[ CARMEN ].uiMoney;
			}
			uiAmount += 10000 * gMercProfiles[ CARMEN ].bNPCData2;
			SetMoneyInSoldierProfile( CARMEN, uiAmount );
			gMercProfiles[ CARMEN ].bNPCData2 = 0;

			for ( cnt = HEAD_1; cnt <= HEAD_7; cnt++ )
			{
				RemoveObjectFromSoldierProfile( CARMEN, (UINT8) cnt );
			}

		}
	}
	else
	{
		// randomize where he'll be today... so long as his sector's not loaded

		if ( gMercProfiles[ CARMEN ].sSectorX != gWorldSectorX || gMercProfiles[ CARMEN ].sSectorY != gWorldSectorY )
		{
			switch( Random( 3 ) )
			{
				case 0:
					gMercProfiles[ CARMEN ].sSectorX = 5;
					gMercProfiles[ CARMEN ].sSectorY = MAP_ROW_C;
					break;
				case 1:
					gMercProfiles[ CARMEN ].sSectorX = 13;
					gMercProfiles[ CARMEN ].sSectorY = MAP_ROW_C;
					break;
				case 2:
					gMercProfiles[ CARMEN ].sSectorX = 9;
					gMercProfiles[ CARMEN ].sSectorY = MAP_ROW_G;
					break;
			}
			// he should have $5000... unless the player forgot to meet him
			if (gMercProfiles[ CARMEN ].uiMoney < 5000)
			{
				SetMoneyInSoldierProfile( CARMEN, 5000 );
			}
		}
	}

	if ( PreRandom( 3 ) == 0 )
	{
		SetFactTrue( FACT_DAVE_HAS_GAS );
	}
	else
	{
		SetFactFalse( FACT_DAVE_HAS_GAS );
	}

	if ( gWorldSectorX == gModSettings.ubHospitalSectorX && gWorldSectorY == gModSettings.ubHospitalSectorY && gbWorldSectorZ == gModSettings.ubHospitalSectorZ )
	{
		CheckForMissingHospitalSupplies();
	}
#endif
}

// Flugente: creature raids
void HandleRaidEventPlanning()
{
	INT32 personnelgain[3];

	// the raid order is bloodcats/zombies/bandits
	for ( int raidtype = 0; raidtype < 3; ++raidtype )
	{
		// reset knowledge of these raids - if the player wants to know, they will have to buy anew
		SetFactFalse( FACT_RAID_KNOWN_BLOODCATS + raidtype );

		personnelgain[raidtype] = 0;

		if ( raidtype == 0 )
		{
			if ( !gGameExternalOptions.gRaid_Bloodcats )
				continue;
			
			// if the lair has been cleared, way less growth
			if ( CheckFact(FACT_BLOODCAT_LAIR_CLEANED, NO_PROFILE) )
				personnelgain[raidtype] += gGameExternalOptions.gRaidReplenish_BaseValue;
			else
				personnelgain[raidtype] += 3 * gGameExternalOptions.gRaidReplenish_BaseValue;
		}
		else if ( raidtype == 1 )
		{
			if ( !gGameExternalOptions.gRaid_Zombies || !gGameSettings.fOptions[TOPTION_ZOMBIES] )
				continue;

			// zombies grow very fast
			personnelgain[raidtype] += 15 * gGameExternalOptions.gRaidReplenish_BaseValue;
		}
		else if ( raidtype == 2 )
		{
			if ( !gGameExternalOptions.gRaid_Bandits )
				continue;

			// bandit growth depends on progress
			personnelgain[raidtype] += gGameExternalOptions.gRaidReplenish_BaseValue * (150 + HighestPlayerProgressPercentage()) / 100;
		}

		std::vector<INT16> attacksectors;

		for ( INT16 sector = 0; sector <= 255; ++sector )
		{
			if ( !StrategicMap[SECTOR_INFO_TO_STRATEGIC_INDEX( sector )].fEnemyControlled )
			{
				if ( ( SectorExternalData[sector][0].usSectorFlagMask & (SECTORFLAG_RAIDPOSSIBLE_BLOODCAT << raidtype ) ) )
				{
					attacksectors.push_back( sector );
				}
			}
		}

		if ( attacksectors.empty() )
			continue;

		int numattacks = gGameExternalOptions.gRaid_MaxAttackPerNight_Bandits;
		if ( raidtype == 0 )
			numattacks = gGameExternalOptions.gRaid_MaxAttackPerNight_Bloodcats;
		else if ( raidtype == 1 )
			numattacks = gGameExternalOptions.gRaid_MaxAttackPerNight_Zombies;

		for ( int attack = 0; attack < numattacks; ++attack )
		{
			UINT32 sector = attacksectors[Random(attacksectors.size())];

			// attack random sector
			if ( raidtype == 0 )
				// bloodcats only attack at night (it is 7:00 now)
				AddStrategicEvent( EVENT_BLOODCAT_ATTACK, GetWorldTotalMin() + 14 * 60 +  Random(10 * 60), sector );
			else
				// other attacks can happen all the time
				AddStrategicEvent( EVENT_BLOODCAT_ATTACK + raidtype, GetWorldTotalMin() + Random( 24 * 60 ), sector );
		}
	}

	// every day we refill our available personnel
	// this also means that at day 1 we have no personnel for raids, forces have to slowly be built up
	AddRaidPersonnel( personnelgain[0], personnelgain[1], personnelgain[2] );
}

void MakeCivGroupHostileOnNextSectorEntrance( UINT8 ubCivGroup )
{
	// if it's the rebels that will become hostile, reduce town loyalties NOW, not later
	if ( ubCivGroup == REBEL_CIV_GROUP && gTacticalStatus.fCivGroupHostile[ ubCivGroup ] == CIV_GROUP_NEUTRAL )
	{
		ReduceLoyaltyForRebelsBetrayed();
	}

	gTacticalStatus.fCivGroupHostile[ ubCivGroup ] = CIV_GROUP_WILL_BECOME_HOSTILE;
}

void RemoveAssassin( UINT8 ubProfile )
{
	gMercProfiles[ ubProfile ].sSectorX = 0;
	gMercProfiles[ ubProfile ].sSectorY = 0;
	gMercProfiles[ ubProfile ].bLife = gMercProfiles[ ubProfile ].bLifeMax;
}

void CheckForMissingHospitalSupplies( void )
{
	UINT32				uiLoop;
	ITEM_POOL *		pItemPool;
	OBJECTTYPE *	pObj;
	UINT8					ubMedicalObjects = 0;

	for ( uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ )
	{
		// loop through all items, look for ownership
		if ( gWorldItems[ uiLoop ].fExists && gWorldItems[ uiLoop ].object.usItem == OWNERSHIP && gWorldItems[ uiLoop ].object[0]->data.owner.ubOwnerCivGroup == DOCTORS_CIV_GROUP )
		{
			GetItemPoolFromGround( gWorldItems[ uiLoop ].sGridNo, &pItemPool ) ;
			while( pItemPool )
			{
				pObj = &( gWorldItems[ pItemPool->iItemIndex ].object );

#ifdef obsoleteCode
				if ( (*pObj)[0]->data.objectStatus > 60 )
				{
					if ( Item[pObj->usItem].firstaidkit || Item[pObj->usItem].medicalkit || pObj->usItem == REGEN_BOOSTER || pObj->usItem == ADRENALINE_BOOSTER )
					{
						ubMedicalObjects++;
					}
				}
#endif//obsoleteCode
				if ( Item[pObj->usItem].firstaidkit || Item[pObj->usItem].medicalkit || pObj->usItem == REGEN_BOOSTER || pObj->usItem == ADRENALINE_BOOSTER )
				{
					for (StackedObjects::iterator iter = pObj->objectStack.begin(); iter != pObj->objectStack.end(); ++iter) {
						if ( iter->data.objectStatus > 60 )
						{
							ubMedicalObjects++;
						}
					}
				}

				pItemPool = pItemPool->pNext;
			}
		}
	}

	if ( CheckFact( FACT_PLAYER_STOLE_MEDICAL_SUPPLIES_AGAIN, 0 ) == TRUE )
	{
		// player returning stuff!	if back to full then can operate
		if ( ubMedicalObjects >= gubCambriaMedicalObjects )
		{
			SetFactFalse( FACT_PLAYER_STOLE_MEDICAL_SUPPLIES_AGAIN );
			SetFactFalse( FACT_PLAYER_STOLE_MEDICAL_SUPPLIES );
			return;
		}
	}

	if ( ubMedicalObjects < gubCambriaMedicalObjects )
	{
		// player's stolen something!
		if ( CheckFact( FACT_PLAYER_STOLE_MEDICAL_SUPPLIES, 0 ) == FALSE )
		{
			SetFactTrue( FACT_PLAYER_STOLE_MEDICAL_SUPPLIES );
		}

		// if only 1/5 or less left, give up the ghost
		if ( ubMedicalObjects * 5 <= gubCambriaMedicalObjects )
		{
			// run out!
			SetFactTrue( FACT_PLAYER_STOLE_MEDICAL_SUPPLIES_AGAIN );
		}
	}

}

// WANNE: No more used for the new airport code
/*
void DropOffItemsInMeduna( UINT8 ubOrderNum )
{
	BOOLEAN	fSectorLoaded = FALSE;
	UINT32	uiCount = 0;
	OBJECTTYPE	*pObject=NULL;
	UINT16	usNumberOfItems=0, usItem;
	UINT8		ubItemsDelivered, ubTempNumItems;
	UINT32	i;

	//if the player doesnt "own" the sector,
	if( StrategicMap[ CALCULATE_STRATEGIC_INDEX( MEDUNA_ITEM_DROP_OFF_SECTOR_X, MEDUNA_ITEM_DROP_OFF_SECTOR_Y ) ].fEnemyControlled )
	{
		//the items disappear
		gpNewBobbyrShipments[ ubOrderNum ].fActive = FALSE;
		return;
	}

	// determine if the sector is loaded
	if( ( gWorldSectorX == MEDUNA_ITEM_DROP_OFF_SECTOR_X ) && ( gWorldSectorY == MEDUNA_ITEM_DROP_OFF_SECTOR_Y ) && ( gbWorldSectorZ == MEDUNA_ITEM_DROP_OFF_SECTOR_Z ) )
		fSectorLoaded = TRUE;
	else
		fSectorLoaded = FALSE;

	// set crate to closed!
	if ( fSectorLoaded )
	{
		SetOpenableStructureToClosed( MEDUNA_ITEM_DROP_OFF_GRIDNO, 0 );
	}
	else
	{
		ChangeStatusOfOpenableStructInUnloadedSector( MEDUNA_ITEM_DROP_OFF_SECTOR_X, MEDUNA_ITEM_DROP_OFF_SECTOR_Y, MEDUNA_ITEM_DROP_OFF_SECTOR_Z, MEDUNA_ITEM_DROP_OFF_GRIDNO, FALSE );
	}


	for(i=0; i<gpNewBobbyrShipments[ ubOrderNum ].ubNumberPurchases; i++)
	{
		// Count how many items were purchased
		usNumberOfItems += gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].ubNumberPurchased;
	}

	//if we are NOT currently in the right sector
	if( !fSectorLoaded )
	{
		//build an array of objects to be added
		pObject = new OBJECTTYPE[ usNumberOfItems ];
		if( pObject == NULL )
			return;
	}


	uiCount = 0;

	//loop through the number of purchases
	for( i=0; i< gpNewBobbyrShipments->ubNumberPurchases; i++)
	{
		ubItemsDelivered = gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].ubNumberPurchased;
		usItem = gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].usItemIndex;

		CreateItem(usItem, 100, &gTempObject);//to get the ItemSlotLimit only!
		while ( ubItemsDelivered )
		{
			// treat 0s as 1s :-)
			ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&gTempObject, STACK_SIZE_LIMIT) );
			CreateItems( usItem, gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].bItemQuality, ubTempNumItems, &gTempObject );

			// stack as many as possible
			if( fSectorLoaded )
			{
				AddItemToPool( MEDUNA_ITEM_DROP_OFF_GRIDNO, &gTempObject, -1, 0, 0, 0 );
			}
			else
			{
				pObject[ uiCount ] = gTempObject;
				uiCount++;
			}

			ubItemsDelivered -= ubTempNumItems;
		}
	}

	//if the sector WASNT loaded
	if( !fSectorLoaded )
	{
		//add all the items from the array that was built above

		//The item are to be added to the Top part of Drassen, grid loc's	10112, 9950
		if( !AddItemsToUnLoadedSector( MEDUNA_ITEM_DROP_OFF_SECTOR_X, MEDUNA_ITEM_DROP_OFF_SECTOR_Y, MEDUNA_ITEM_DROP_OFF_SECTOR_Z, MEDUNA_ITEM_DROP_OFF_GRIDNO, uiCount, pObject, 0, 0, 0, -1, FALSE ) )
		{
			//error
			Assert( 0 );
		}
		delete[]( pObject );
		pObject = NULL;
	}

	//mark that the shipment has arrived
	gpNewBobbyrShipments[ ubOrderNum ].fActive = FALSE;

	//Add an email from kulba telling the user the shipment is there
	AddEmail( BOBBY_R_MEDUNA_SHIPMENT, BOBBY_R_MEDUNA_SHIPMENT_LENGTH, BOBBY_R, GetWorldTotalMin(), -1 );
}
*/
