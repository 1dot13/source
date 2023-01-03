#include "SaveLoadMap.h"
#include "Structure Wrap.h"
#include "Tactical Save.h"
#include "postalservice.h"
#include "strategic.h"
#include "strategicmap.h"
#include "random.h"
#include "game clock.h"
#include "GameSettings.h"
#include <list>
#include <string>
#include <iostream>
#include "Quests.h"
#include "Strategic Town Loyalty.h"
#include "email.h"
#include "BobbyRMailOrder.h"

// WANNE - MP: Used for multiplayer
#include "connect.h"
#include "Strategic Event Handler.h"

#ifdef JA2UB
#include "ub_config.h"
#endif

using namespace std;

/************************************************************************************/
/* New shipping system															    */
/* ===================																*/
/*																					*/
/*	Old C-functions used:															*/
/*	- GridNoInVisibleWorldTile()													*/
/*	- SetOpenableStructureToClosed()												*/
/*	- ChangeStatusOfOpenableStructInUnloadedSector()								*/
/*	- CreateItem()																	*/
/*	- CreateItems()																	*/
/*	- AddItemToPool()																*/
/*	- AddItemsToUnLoadedSector()													*/
/*	- Random()																		*/
/*	- AddFutureDayStrategicEvent()													*/
/*																					*/
/*	Old defines/macros:																*/
/*	- CALCULATE_STRATEGIC_INDEX()													*/
/*	- Assert() (for debugging)														*/
/*																					*/
/*	Old structs:																	*/
/*	- OBJECTTYPE																	*/
/*																					*/
/*	Old globals:																	*/
/*	- StrategicMapElement StrategicMap[]											*/
/************************************************************************************/

extern StrategicMapElement StrategicMap[];

extern INT16 gusCurShipmentDestinationID;



/////////////////////////////////////////////////////
// CShipmentManipulator member implementation
/////////////////////////////////////////////////////
CShipmentManipulator::CShipmentManipulator(PCPostalService Owner, PShipmentStruct Shipment)
{
	this->_Shipment = Shipment;
	this->_fContinueCallbackChain = TRUE;
	this->_PostalService = Owner;
}

INT16 CShipmentManipulator::GetSenderID(void)
{
	return _Shipment->sSenderID;
}

RefToDestinationStruct CShipmentManipulator::GetDestination(void) const
{
	return *_Shipment->pDestination;
}

void CShipmentManipulator::ContinueCallbackChain(BOOLEAN fContinue)
{
	_fContinueCallbackChain = fContinue;
}

void CShipmentManipulator::CancelDelivery(void)
{
	_Shipment->ShipmentStatus = SHIPMENT_CANCEL_DELIVERY;
}

//////////////////////////////////////////
// CPostalService member implementation
/////////////////////////////////////////

DestinationList CPostalService::_Destinations;
vector<BOOLEAN> CPostalService::_UsedDestinationIDList;
OBJECTTYPE	CPostalService::tempObject;

CPostalService::CPostalService()
{
}

void CPostalService::Clear(bool clearDataOnly)
{
	_Shipments.clear();

	if (!clearDataOnly)
	{
		_UsedShipmentIDList.clear();;
		_DeliveryCallbacks.clear();
		_Destinations.clear();
		_UsedDestinationIDList.clear();
		_DeliveryMethods.clear();	
	}
}

UINT16 CPostalService::CreateNewShipment(UINT16 usDestinationID, UINT8  ubDeliveryMethodIndex, INT16 sSenderID)
{
	if(	usDestinationID > _UsedDestinationIDList.size() ||
		!_UsedDestinationIDList[usDestinationID] ||
		ubDeliveryMethodIndex > _DeliveryMethods.size() )
	{
		Assert(0);
		return 0;
	}

	_Destinations.sort(DESTINATION_LIST_ASCENDING);
	RefToDestinationListIterator dli = _Destinations.begin();
	
	while(DESTINATION(dli).usID != usDestinationID)
	{
		if(dli == _Destinations.end())
		{
			Assert(0);
			return 0;
		}
		dli++;
	}

	UINT16 usNewID = 1;
	if(_UsedShipmentIDList.empty())
	{
		_UsedShipmentIDList.push_back(FALSE);
		_UsedShipmentIDList.push_back(TRUE);
	}
	else
	{
		while(1)
		{
			if(usNewID == _UsedShipmentIDList.size() - 1)	// All IDs assigned, add a new one
			{
				usNewID++;
				_UsedShipmentIDList.push_back(TRUE);
				break;
			}

			if(!_UsedShipmentIDList[usNewID])	// Found an ID that is not assigned
			{
				_UsedShipmentIDList[usNewID] = TRUE;
				break;	
			}

			usNewID++;
		}
	}

	ShipmentStruct shs;
	shs.usID			= usNewID;
	shs.pDestination	= &(DESTINATION(dli));
	shs.pDestinationDeliveryInfo = &(_DeliveryMethods[ubDeliveryMethodIndex].pDestinationDeliveryInfos->at(DESTINATION(dli).usID));
	shs.sSenderID		= sSenderID;
	shs.ShipmentStatus	= SHIPMENT_STATIONARY;
	shs.uiOrderDate		= GetWorldDay();

	_Shipments.push_back(shs);

	return usNewID;
}

BOOLEAN CPostalService::AddPackageToShipment(UINT16 usShipmentID, UINT16 usItemIndex, UINT8 ubNumber, INT8 bItemQuality)
{
	if(	usShipmentID > _UsedShipmentIDList.size() ||
		!_UsedShipmentIDList[usShipmentID])
	{
		Assert(0);
		return FALSE;
	}

	_Shipments.sort(SHIPMENT_LIST_ASCENDING);
	RefToShipmentListIterator sli = _Shipments.begin();

	while(SHIPMENT(sli).usID != usShipmentID)
	{
		if(sli == _Shipments.end())
		{
			Assert(0);
			return FALSE;
		}
		sli++;
	}

	ShipmentPackageStruct sps;
	sps.usItemIndex = usItemIndex;
	sps.ubNumber = ubNumber;
	sps.bItemQuality = bItemQuality;

	SHIPMENT(sli).ShipmentPackages.push_back(sps);

	return TRUE;
}

BOOLEAN CPostalService::SendShipment(UINT16 usShipmentID)
{
	BOOLEAN BR_FAST_SHIP = gGameExternalOptions.fBobbyRayFastShipments;
	if( usShipmentID > _UsedShipmentIDList.size() ||
		!_UsedShipmentIDList[usShipmentID])
	{
		Assert(0);
		return FALSE;
	}

	RefToShipmentListIterator sli = _Shipments.begin();

	while(SHIPMENT(sli).usID != usShipmentID)
	{
		if(sli == _Shipments.end())
		{
			Assert(0);
			return FALSE;
		}
		sli++;
	}

	SHIPMENT(sli).ShipmentStatus = SHIPMENT_INTRANSIT;

	if (is_networked)
	{
		if (is_client)
		{
			this->DeliverShipmentForMultiplayer(usShipmentID);
		}
	}
	else
	{	
		//JMich
		//Jenilee: now faster shipments
		if (!BR_FAST_SHIP)
		{
			AddFutureDayStrategicEvent( EVENT_POSTAL_SERVICE_SHIPMENT, (8 + Random(4) ) * 60, usShipmentID, SHIPMENT(sli).pDestinationDeliveryInfo->bDaysAhead);
		}
		else
		{
			AddStrategicEvent(EVENT_POSTAL_SERVICE_SHIPMENT, GetWorldTotalMin() + ((SHIPMENT(sli).pDestinationDeliveryInfo->bDaysAhead * (5 + Random(3))) * 60), usShipmentID);
		}
	}

	return TRUE;
}

BOOLEAN CPostalService::DeliverShipment(UINT16 usShipmentID)
{
	UINT32	uiChanceOfTheft;
	static UINT8 ubShipmentsSinceNoBribes = 0;
	BOOLEAN		fPablosStoleLastItem = FALSE;
	UINT32	uiStolenCount = 0;
	UINT32 sMapPos;
	BOOLEAN		fPablosStoleSomething = FALSE;

	if(	usShipmentID > _UsedShipmentIDList.size() ||
		!_UsedShipmentIDList[usShipmentID])
	{
		Assert(0);
		return FALSE;
	}

	RefToShipmentListIterator sli = _Shipments.begin();

	while(SHIPMENT(sli).usID != usShipmentID)
	{
		if(sli == _Shipments.end())
		{
			Assert(0);
			return FALSE;
		}
		sli++;
	}

	if(SHIPMENT(sli).ShipmentStatus == SHIPMENT_STATIONARY)
	{
		return FALSE;
	}
	
	PCShipmentManipulator ShipmentManipulator = new CShipmentManipulator(this, &SHIPMENT(sli));

	// Delivery callback code goes here
	if(_DeliveryCallbacks[0].DeliveryCallbackFunc)
	{
		_DeliveryCallbacks[0].DeliveryCallbackFunc(*ShipmentManipulator);
	}

	if(ShipmentManipulator->_fContinueCallbackChain)
	{
		if( SHIPMENT(sli).sSenderID + 2 <= (INT16)_DeliveryCallbacks.size() &&
			_DeliveryCallbacks[SHIPMENT(sli).sSenderID + 1].DeliveryCallbackFunc)
		{
			// WANNE: This calls the BobbyRDeliveryCallback() method in BobbyRMailOrder.cpp
			_DeliveryCallbacks[SHIPMENT(sli).sSenderID + 1].DeliveryCallbackFunc(*ShipmentManipulator);
		}
	}
	

	if(SHIPMENT(sli).ShipmentStatus == SHIPMENT_CANCEL_DELIVERY)
	{
		SHIPMENT(sli).ShipmentStatus = SHIPMENT_STATIONARY;
		return FALSE;
	}


	RefToShipmentStruct shs = SHIPMENT(sli);

	// Sector is enemy controlled or shipment sector does not exist (->ubMapX = 0, ubMapY = 0)  -> Clear the shipments
	if( StrategicMap[ CALCULATE_STRATEGIC_INDEX( shs.pDestination->ubMapX, shs.pDestination->ubMapY ) ].fEnemyControlled )
	{
		shs.ShipmentPackages.clear();
		shs.pDestination = NULL;
		shs.pDestinationDeliveryInfo = NULL;
		shs.ShipmentStatus = SHIPMENT_STATIONARY;
		_UsedShipmentIDList[usShipmentID] = FALSE;

		_Shipments.erase(sli);

		return TRUE;
	}

	if( (shs.pDestination->ubMapX > 0) && 
		(shs.pDestination->ubMapX < MAP_WORLD_X - 1) &&
		(shs.pDestination->ubMapY > 0) && 
		(shs.pDestination->ubMapY < MAP_WORLD_Y - 1) ) //&& 
		//GridNoOnVisibleWorldTile(shs.pDestination->sGridNo) )
		// silversurfer: wtf? This test checked if the delivery tile was valid for the currently loaded sector
		// which is not necessarily the delivery sector
		// if we are currently below ground it will crash the game...
	{
		BOOLEAN fSectorLoaded = ( gWorldSectorX == shs.pDestination->ubMapX ) && 
								( gWorldSectorY == shs.pDestination->ubMapY ) && 
								( gbWorldSectorZ == shs.pDestination->ubMapZ);

		
		// WDS - Option to turn off stealing
		// check for potential theft
		if (gGameExternalOptions.fStealingDisabled ||
			CheckFact( FACT_PABLO_WONT_STEAL, 0 ))
		{
			uiChanceOfTheft = 0;
		}
		else if (CheckFact( FACT_PABLOS_BRIBED, 0 ))
		{
			// Since Pablo has some money, reduce record of # of shipments since last bribed...
			ubShipmentsSinceNoBribes /= 2;
			uiChanceOfTheft = 0;
		}
		else
		{
			ubShipmentsSinceNoBribes++;
			// this chance might seem high but it's only applied at most to every second item
			uiChanceOfTheft = 12 + Random( 4 * ubShipmentsSinceNoBribes );
		}
	
		// Shipment from John Kulba can never be lost, only from Bobby Ray
		if (shs.sSenderID == BOBBYR_SENDER_ID)
		{
			// The whole shipment will be lost
			if (Random( 100 ) < gGameExternalOptions.gubChanceOfShipmentLost)
			{
				// lose the whole shipment!
				// Delete the shipments!
				shs.ShipmentPackages.clear();
				shs.pDestination = NULL;
				shs.pDestinationDeliveryInfo = NULL;
				shs.ShipmentStatus = SHIPMENT_STATIONARY;
				_UsedShipmentIDList[usShipmentID] = FALSE;

				_Shipments.erase(sli);

				SetFactTrue( FACT_LAST_SHIPMENT_CRASHED );

				return TRUE;
			}
		}

		if(fSectorLoaded)
		{
			SetOpenableStructureToClosed( shs.pDestination->sGridNo, 0 );
		}
		else
		{
			ChangeStatusOfOpenableStructInUnloadedSector(	shs.pDestination->ubMapX, shs.pDestination->ubMapY, 
															shs.pDestination->ubMapZ, shs.pDestination->sGridNo, FALSE );
		}

		UINT16 usNumberOfItems=0;
		for(int i = 0; i < (int)shs.ShipmentPackages.size(); i++)
		{
			usNumberOfItems += shs.ShipmentPackages[i].ubNumber;
		}

		OBJECTTYPE *pObject;
		OBJECTTYPE *pStolenObject;
		if(!fSectorLoaded)
		{
			pObject = new OBJECTTYPE[usNumberOfItems];
			pStolenObject = new OBJECTTYPE[ usNumberOfItems ];

			if(!pObject || !pStolenObject)
			{
				Assert(0);
				return FALSE;
			}
		}

		UINT		uiCount=0;
		UINT8		ubItemsDelivered, ubTempNumItems;
		UINT16		usItem;
		
		// Loop through the packages of the order.
		// A package is the sum of the same items (e.g: 5 Colts = 1 package, 2 Colts & 1 legging = 2 packages)
		for(int i=0; i < (int)shs.ShipmentPackages.size(); i++)
		{
			// Wie viele items sollen geliefert werden ?
			ubItemsDelivered = shs.ShipmentPackages[i].ubNumber;
			
			// das aktuelle item
			usItem = shs.ShipmentPackages[i].usItemIndex;

			CreateItem(usItem, 100, &tempObject);

			// if it's a gun
			if (Item [ usItem ].usItemClass == IC_GUN )
			{
				// Empty out the bullets put in by CreateItem().	We now sell all guns empty of bullets.	This is done for BobbyR
				// simply to be consistent with the dealers in Arulco, who must sell guns empty to prevent ammo cheats by players.
				tempObject[0]->data.gun.ubGunShotsLeft = 0;
			}

			UINT8 cnt = -1;
			// Loop through the items of a package
			while ( ubItemsDelivered )
			{
				cnt++;

				// Check how many items we can group together on one stack
				if (UsingNewInventorySystem() == false)
				{
					ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&tempObject, BIGPOCK1POS) );
				}
				else
				{
					ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&tempObject, STACK_SIZE_LIMIT) );
				}
				
				// Create all the items that are grouped together
				CreateItems( usItem, shs.ShipmentPackages[i].bItemQuality, ubTempNumItems, &tempObject );

				if( fSectorLoaded )
				{
					// Pablo has stolen the item
					if ( !fPablosStoleLastItem && uiChanceOfTheft > 0 && Random( 100 ) < (uiChanceOfTheft + cnt) )
					{
						uiStolenCount++;
						sMapPos = PABLOS_STOLEN_DEST_GRIDNO; // off screen!
						fPablosStoleSomething = TRUE;
						fPablosStoleLastItem = TRUE;
					}
					// Item not stolen
					else
					{
						sMapPos = shs.pDestination->sGridNo;
						fPablosStoleLastItem = FALSE;
					}

					// Add the single item to the pool (the Crate or Pablose stolen grid no)
					AddItemToPool( sMapPos, &tempObject, -1, 0, 0, 0 );
				}
				else
				{
					// Pablo steals the item
					if ( !fPablosStoleLastItem && uiChanceOfTheft > 0 && Random( 100 ) < (uiChanceOfTheft + cnt) )
					{
						pStolenObject[ uiStolenCount ] = gTempObject;
						uiStolenCount++;
						fPablosStoleSomething = TRUE;
						fPablosStoleLastItem = TRUE;
					}
					else
					{
						pObject[ uiCount ] = tempObject;
						uiCount++;
					}
				}

				// Substract the number of items we already placed from the total number of items of this package
				ubItemsDelivered -= ubTempNumItems;
			}
		}

		if( !fSectorLoaded )
		{
			// Successfull delivered items into the crate
			if (uiCount > 0)
			{
				if( !AddItemsToUnLoadedSector(	shs.pDestination->ubMapX, shs.pDestination->ubMapY, 
					shs.pDestination->ubMapZ, shs.pDestination->sGridNo, uiCount, pObject, 0, 0, 0, -1, FALSE ) )
				{
					Assert( 0 );
				}
				delete[]( pObject );
				pObject = NULL;
			}

			// Stolen items by Pablo
			if (uiStolenCount > 0)
			{
				// Delivered items
				if( !AddItemsToUnLoadedSector(	shs.pDestination->ubMapX, shs.pDestination->ubMapY, 
					shs.pDestination->ubMapZ, PABLOS_STOLEN_DEST_GRIDNO, uiStolenCount, pStolenObject, 0, 0, 0, -1, FALSE ) )
				{
					Assert( 0 );
				}
				delete[]( pStolenObject );
				pStolenObject = NULL;
			}
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

		// WANNE - MP: Do not send email notification from Bobby Ray in a multiplayer game
		if (!is_networked)
		{
		
		StopTimeCompression();
#ifdef JA2UB

//no UB		
	if ( gGameUBOptions.fBobbyRSite == TRUE )
	{
			// Shipment from Bobby Ray
			if (shs.sSenderID == BOBBYR_SENDER_ID)
				AddBobbyREmailJA2( 198, 4, BOBBY_R, GetWorldTotalMin(), -1, gusCurShipmentDestinationID, TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT);	
				// Shipment from John Kulba
			//else
			//	AddEmail( JOHN_KULBA_GIFT_IN_DRASSEN, JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1, gusCurShipmentDestinationID, TYPE_EMAIL_EMAIL_EDT);
	}
#else

			// Shipment from Bobby Ray
			if (shs.sSenderID == BOBBYR_SENDER_ID)
				AddEmail( BOBBYR_SHIPMENT_ARRIVED, BOBBYR_SHIPMENT_ARRIVED_LENGTH, BOBBY_R, GetWorldTotalMin(), -1, gusCurShipmentDestinationID, TYPE_EMAIL_EMAIL_EDT);	
			// Shipment from John Kulba
			else
				AddEmail( JOHN_KULBA_GIFT_IN_DRASSEN, JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1, gusCurShipmentDestinationID, TYPE_EMAIL_EMAIL_EDT);
#endif
		}

		shs.ShipmentPackages.clear();
		shs.pDestination = NULL;
		shs.pDestinationDeliveryInfo = NULL;
		shs.ShipmentStatus = SHIPMENT_STATIONARY;
		_UsedShipmentIDList[usShipmentID] = FALSE;

		_Shipments.erase(sli);

		return TRUE;
	}
	else
		Assert(0);

	return FALSE;
}

BOOLEAN CPostalService::DeliverShipmentForMultiplayer(UINT16 usShipmentID)
{
	if(	usShipmentID > _UsedShipmentIDList.size() ||
		!_UsedShipmentIDList[usShipmentID])
	{
		Assert(0);
		return FALSE;
	}

	RefToShipmentListIterator sli = _Shipments.begin();

	while(SHIPMENT(sli).usID != usShipmentID)
	{
		if(sli == _Shipments.end())
		{
			Assert(0);
			return FALSE;
		}
		sli++;
	}

	if(SHIPMENT(sli).ShipmentStatus == SHIPMENT_STATIONARY)
	{
		return FALSE;
	}
	
	PCShipmentManipulator ShipmentManipulator = new CShipmentManipulator(this, &SHIPMENT(sli));

	// Delivery callback code goes here
	if(_DeliveryCallbacks[0].DeliveryCallbackFunc)
	{
		_DeliveryCallbacks[0].DeliveryCallbackFunc(*ShipmentManipulator);
	}

	if(ShipmentManipulator->_fContinueCallbackChain)
	{
		if( SHIPMENT(sli).sSenderID + 2 <= (INT16)_DeliveryCallbacks.size() &&
			_DeliveryCallbacks[SHIPMENT(sli).sSenderID + 1].DeliveryCallbackFunc)
		{
			_DeliveryCallbacks[SHIPMENT(sli).sSenderID + 1].DeliveryCallbackFunc(*ShipmentManipulator);
		}
	}
	

	if(SHIPMENT(sli).ShipmentStatus == SHIPMENT_CANCEL_DELIVERY)
	{
		SHIPMENT(sli).ShipmentStatus = SHIPMENT_STATIONARY;
		return FALSE;
	}


	RefToShipmentStruct shs = SHIPMENT(sli);

	if( (shs.pDestination->ubMapX > 0) && 
		(shs.pDestination->ubMapX < MAP_WORLD_X - 1) &&
		(shs.pDestination->ubMapY > 0) && 
		(shs.pDestination->ubMapY < MAP_WORLD_Y - 1) && 
		GridNoOnVisibleWorldTile(shs.pDestination->sGridNo) )
	{
		BOOLEAN fSectorLoaded = ( gWorldSectorX == shs.pDestination->ubMapX ) && 
								( gWorldSectorY == shs.pDestination->ubMapY ) && 
								( gbWorldSectorZ == shs.pDestination->ubMapZ);

		if(fSectorLoaded)
		{
			SetOpenableStructureToClosed( shs.pDestination->sGridNo, 0 );
		}
		else
		{
			ChangeStatusOfOpenableStructInUnloadedSector(	shs.pDestination->ubMapX, shs.pDestination->ubMapY, 
															shs.pDestination->ubMapZ, shs.pDestination->sGridNo, FALSE );
		}

		UINT16 usNumberOfItems=0;
		for(int i = 0; i < (int)shs.ShipmentPackages.size(); i++)
		{
			usNumberOfItems += shs.ShipmentPackages[i].ubNumber;
		}

		OBJECTTYPE *pObject;
		if(!fSectorLoaded)
		{
			pObject = new OBJECTTYPE[usNumberOfItems];

			if(!pObject)
			{
				Assert(0);
				return FALSE;
			}
		}

		UINT		uiCount=0;
		UINT8		ubItemsDelivered, ubTempNumItems;
		UINT16		usItem;
		
		for(int i=0; i < (int)shs.ShipmentPackages.size(); i++)
		{
			ubItemsDelivered = shs.ShipmentPackages[i].ubNumber;
			usItem = shs.ShipmentPackages[i].usItemIndex;

			CreateItem(usItem, 100, &tempObject);

			while ( ubItemsDelivered )
			{
				if (UsingNewInventorySystem() == false)
				{
					ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&tempObject, BIGPOCK1POS) );
				}
				else
				{
					ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&tempObject, STACK_SIZE_LIMIT) );
				}
				
				CreateItems( usItem, shs.ShipmentPackages[i].bItemQuality, ubTempNumItems, &tempObject );

				if( fSectorLoaded )
				{
					AddItemToPool( 12880, &tempObject, 1, 0, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0 );
				}
				else
				{
					pObject[ uiCount ] = tempObject;
					uiCount++;
				}

				ubItemsDelivered -= ubTempNumItems;
			}
		}

		if( !fSectorLoaded )
		{
			if( !AddItemsToUnLoadedSector(	gsMercArriveSectorX, gsMercArriveSectorY, 
				0, 12880, uiCount, pObject, 0, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0, 1, FALSE ) )
			{
				Assert( 0 );
			}
			delete[]( pObject );
			pObject = NULL;
		}

		shs.ShipmentPackages.clear();
		shs.pDestination = NULL;
		shs.pDestinationDeliveryInfo = NULL;
		shs.ShipmentStatus = SHIPMENT_STATIONARY;
		_UsedShipmentIDList[usShipmentID] = FALSE;

		_Shipments.erase(sli);

		return TRUE;
	}
	else
		Assert(0);

	return FALSE;
}

BOOLEAN CPostalService::RegisterDeliveryCallback(INT16 sSenderID, PtrToDeliveryCallbackFunc DeliveryCallbackFunc)
{
	if(sSenderID < - 1 || !DeliveryCallbackFunc)
	{
		Assert(0);
		return 0;
	}

	DeliveryCallbackDataStruct dcd;
	
	dcd.sSenderID=-1;
	dcd.DeliveryCallbackFunc = NULL;
	if(_DeliveryCallbacks.empty())
	{
		_DeliveryCallbacks.push_back(dcd);
	}

	if(sSenderID == -1)
	{
		dcd.DeliveryCallbackFunc = DeliveryCallbackFunc;
		_DeliveryCallbacks[0].DeliveryCallbackFunc = DeliveryCallbackFunc;
	}
	else
	{
		// Note: For size comparsions, sSenderID has to be offset by 2, as index 0 of the callback vector is used by the default callback,
		//		 taking up 1 element already
		if(sSenderID + 2 > (INT16)_DeliveryCallbacks.size())
		{
			_DeliveryCallbacks.resize(sSenderID+2);
			_DeliveryCallbacks[sSenderID+1].DeliveryCallbackFunc = DeliveryCallbackFunc;
			_DeliveryCallbacks[sSenderID+1].sSenderID = sSenderID;
		}
		else
		{
			_DeliveryCallbacks[sSenderID+1].sSenderID = sSenderID;
			_DeliveryCallbacks[sSenderID+1].DeliveryCallbackFunc = DeliveryCallbackFunc;
		}
	}

	return TRUE;
}

BOOLEAN CPostalService::LoadShipmentListFromSaveGameFile(HWFILE hFile)
{
	UINT32 uiBytesRead=0;
	ShipmentListSaveFileDataHeaderStruct sls;
	// Read in shipment list data header

	if(!_Shipments.empty())
	{
		_Shipments.clear();
	}

	FileRead(hFile, &sls, sizeof(ShipmentListSaveFileDataHeaderStruct), &uiBytesRead);
	if(uiBytesRead != sizeof(ShipmentListSaveFileDataHeaderStruct))
	{
		return FALSE;
	}
		
	ShipmentPackageStruct		sps;	
		
	for(int i = 0; i < (int)sls.uiNumberOfShipments; i++)
	{
		ShipmentSaveFileDataStruct	sfs;

		// Read in shipment data header
		memset(&sfs, 0, sizeof(ShipmentSaveFileDataStruct));
		uiBytesRead = 0;
		FileRead(hFile, &sfs, sizeof(ShipmentSaveFileDataStruct), &uiBytesRead);
		if(uiBytesRead != sizeof(ShipmentSaveFileDataStruct))
		{
			return FALSE;
		}
		
		// WANNE: Bugfix: Fixed CTD in VS 2008 Release EXE (by birdflu) -> This leads to item duplication!!!
		//memset(&shs, 0, sizeof(ShipmentStruct));		

		// WANNE: This is the fix for the item duplication!
		ShipmentStruct	shs;

		shs.pDestination	= &(_GetDestination(sfs.usDestinationID));
		shs.pDestinationDeliveryInfo = &(_DeliveryMethods[sfs.ubDeliveryMethodIndex].pDestinationDeliveryInfos->at(sfs.usDestinationID));
		shs.ShipmentStatus	= sfs.ShipmentStatus;
		shs.sReceiverID		= sfs.sReceiverID;
		shs.sSenderID		= sfs.sSenderID;
		shs.usID			= sfs.usID;
		shs.uiOrderDate		= sfs.uiOrderDate;
				
		_UsedShipmentIDList.resize(sfs.usID+1);
		_UsedShipmentIDList[sfs.usID] = TRUE;

		for(int j=0; j < (int)sfs.uiNumberOfPackages; j++)
		{
			uiBytesRead=0;
			memset(&sps, 0, sizeof(ShipmentPackageStruct));
			FileRead(hFile, &sps, sizeof(ShipmentPackageStruct), &uiBytesRead);
			if(uiBytesRead != sizeof(ShipmentPackageStruct))
			{
				return FALSE;
			}
			shs.ShipmentPackages.push_back(sps);			
		}
		_Shipments.push_back(shs);		
	}

	return TRUE;
}

BOOLEAN CPostalService::SaveShipmentListToSaveGameFile(HWFILE hFile)
{
	ShipmentListSaveFileDataHeaderStruct sls;

	sls.uiNumberOfShipments = _Shipments.size();
	sls.uiPackageDataSize	= sizeof(ShipmentPackageStruct);
	sls.uiShipmentDataSize	= sizeof(ShipmentSaveFileDataStruct);

	UINT32 uiBytesWritten=0;
	FileWrite(hFile, &sls, sizeof(ShipmentListSaveFileDataHeaderStruct), &uiBytesWritten);
	if(uiBytesWritten != sizeof(ShipmentListSaveFileDataHeaderStruct))
	{
		return FALSE;
	}

	if(_Shipments.empty())
	{
		return TRUE;
	}

	RefToShipmentListIterator	sli = _Shipments.begin();
	ShipmentSaveFileDataStruct	sfs;
	ShipmentPackageStruct		sps;

	while(sli != _Shipments.end() )
	{
		memset(&sfs, 0, sizeof(ShipmentSaveFileDataStruct));
		uiBytesWritten = 0;

		sfs.ShipmentStatus			= SHIPMENT(sli).ShipmentStatus;
		sfs.sReceiverID				= SHIPMENT(sli).sReceiverID;
		sfs.sSenderID				= SHIPMENT(sli).sSenderID;
		sfs.ubDeliveryMethodIndex	= SHIPMENT(sli).pDestinationDeliveryInfo->ubParentDeliveryMethodIndex;;
		sfs.uiNumberOfPackages		= SHIPMENT(sli).ShipmentPackages.size();
		sfs.usDestinationID			= SHIPMENT(sli).pDestination->usID;
		sfs.usID					= SHIPMENT(sli).usID;
		sfs.uiOrderDate				= SHIPMENT(sli).uiOrderDate;
		
		FileWrite(hFile, &sfs, sizeof(ShipmentSaveFileDataStruct), &uiBytesWritten);		
		if(uiBytesWritten != sizeof(ShipmentSaveFileDataStruct))
		{
			return FALSE;
		}

		for(int i = 0; i < (int)SHIPMENT(sli).ShipmentPackages.size(); i++)
		{
			memset(&sps, 0, sizeof(ShipmentPackageStruct));
			uiBytesWritten = 0;

			sps.bItemQuality	= SHIPMENT(sli).ShipmentPackages[i].bItemQuality;
			sps.ubNumber		= SHIPMENT(sli).ShipmentPackages[i].ubNumber;
			sps.usItemIndex		= SHIPMENT(sli).ShipmentPackages[i].usItemIndex;
			FileWrite(hFile, &sps, sizeof(ShipmentPackageStruct), &uiBytesWritten);
			if(uiBytesWritten != sizeof(ShipmentPackageStruct))
			{
				return FALSE;
			}
		}

		sli++;
	}

	return TRUE;
}

UINT16 CPostalService::AddDestination(UINT32 uiIndex, UINT8 ubMapX, UINT8 ubMapY, UINT8 ubMapZ, UINT32 sGridNo, STR16 pszName)
{
	UINT16	usNewID=1;
	// We need to find an ID first
	if(_UsedDestinationIDList.empty())	// No IDs assigned yet
	{
		_UsedDestinationIDList.push_back(FALSE);	// We're not using ID 0 to avoid confusion
		_UsedDestinationIDList.push_back(TRUE);		// This is the very first Destination so we use ID 1
	}
	else	// There are IDs assigned, let's see if there are any gaps
	{
		while(1)
		{
			if(usNewID == _UsedDestinationIDList.size() - 1)	// All IDs assigned, add a new one
			{
				usNewID++;
				_UsedDestinationIDList.push_back(TRUE);
				break;
			}

			if(!_UsedDestinationIDList[usNewID])	// Found an ID that is not assigned
			{
				_UsedDestinationIDList[usNewID] = TRUE;
				break;	
			}

			usNewID++;
		}
	}

	// Alright, we've found a suitable ID, let's add the destination
	DestinationStruct newDestination;

	newDestination.sGridNo	=	sGridNo;
	newDestination.wstrName =	pszName;
	newDestination.uiIndex	=	uiIndex;
	newDestination.ubMapX	=	ubMapX;
	newDestination.ubMapY	=	ubMapY;
	newDestination.ubMapZ	=	ubMapZ;
	newDestination.usID		=	usNewID;

	_Destinations.push_back(newDestination);

	return usNewID;
}

RefToDestinationList CPostalService::LookupDestinationList(void) const
{
	return _Destinations;
}

UINT16 CPostalService::RemoveDestination(UINT16 usDestinationID)
{
	if( _Destinations.empty() || usDestinationID >= _UsedDestinationIDList.size() || _UsedDestinationIDList[usDestinationID] == FALSE )
	{
		// ID is not in use or unknown or the list is actually empty
		Assert(0);
		return 0;
	}
	// ID  is known and in use
	_Destinations.sort(DESTINATION_LIST_ASCENDING);
	DestinationList::iterator dli = _Destinations.begin();

	while( ( (RefToDestinationStruct)*dli ).usID != usDestinationID )
		dli++;

	_Destinations.erase(dli);
	_UsedDestinationIDList[usDestinationID] = FALSE;

	// TO-DO: Add some lines to check and clean up the used destination ID list, i.e. remove empty elements at the end of the list

	return usDestinationID;
}

// WANNE: This method returns, if a given sector (x, y, z) is a shipment destination sector
BOOLEAN CPostalService::IsSectorAShipmentSector(UINT8 ubMapX, UINT8 ubMapY, UINT8 ubMapZ)
{
	BOOLEAN isShipmentSector = FALSE;

	vector<PDestinationStruct> destinations;
	RefToDestinationListIterator dli = LookupDestinationList().begin();

	while (dli != LookupDestinationList().end())
	{
		destinations.push_back(&DESTINATION(dli));
		dli++;
	}

	for (unsigned int i = 0; i < destinations.size(); i++)
	{
		PDestinationStruct destStruct = destinations.at(i);
		if (destStruct)
		{
			if (destStruct->ubMapX == ubMapX &&
				destStruct->ubMapY == ubMapY &&
				destStruct->ubMapZ == ubMapZ)
			{
				isShipmentSector = TRUE;
				break;
			}
		}
	}

	return isShipmentSector;
}

UINT16 CPostalService::GetShipmentCount(SHIPMENT_STATUS TargetedShipmentStatus)
{
	if(_Shipments.empty())
	{
		return 0;
	}

	RefToShipmentListIterator sli = _Shipments.begin();
	
	UINT16 usCnt=0;
	while(sli != _Shipments.end())
	{
		usCnt += (SHIPMENT(sli).ShipmentStatus == TargetedShipmentStatus);
		sli++;
	}
	return usCnt;
}

RefToShipmentList CPostalService::LookupShipmentList(void) const
{
	return (RefToShipmentList)_Shipments;
}

RefToDestinationStruct CPostalService::GetDestination(UINT16 usDestinationID) const
{
	if(	usDestinationID > _UsedDestinationIDList.size() ||
		!_UsedDestinationIDList[usDestinationID])
	{
		Assert(0);
		return DESTINATION(_Destinations.end());
	}

	DestinationList::const_iterator dli = _Destinations.begin();

	while(DESTINATION(dli).usID != usDestinationID)
	{
		if(dli == _Destinations.end())
		{
			Assert(0);
			return DESTINATION(_Destinations.end());
		}
		dli++;
	}

	return DESTINATION(dli);
}

UINT8 CPostalService::AddDeliveryMethod(STR16 pszDescription)
{
	DeliveryMethodStruct dms;

	dms.wstrDescription = pszDescription;
	dms.pDestinationDeliveryInfos = new DestinationDeliveryInfoTable;

	_DeliveryMethods.push_back(dms);

	return _DeliveryMethods.size() - 1;
}

UINT16 CPostalService::SetDestinationDeliveryInfo(UINT8 ubDeliveryMethodIndex, UINT32 uiDestinationIndex, UINT16 usDestinationFee, INT8 bDaysAhead)
{
	if( (ubDeliveryMethodIndex > _DeliveryMethods.size() - 1) )
	{
		Assert(0);
		return 0;
	}

	_DeliveryMethods[ubDeliveryMethodIndex].pDestinationDeliveryInfos->resize(_UsedDestinationIDList.size());

	_Destinations.sort(DESTINATION_LIST_ASCENDING);

	RefToDestinationListIterator dli = _Destinations.begin();

	while( DESTINATION(dli).uiIndex != uiDestinationIndex)
	{
		if(dli == _Destinations.end() )
		{
			Assert(0);
			return 0;	// No destination with the specified index found
		}
		dli++;
	}

	_DeliveryMethods[ubDeliveryMethodIndex].pDestinationDeliveryInfos->at(DESTINATION(dli).usID).ubParentDeliveryMethodIndex = ubDeliveryMethodIndex;
	_DeliveryMethods[ubDeliveryMethodIndex].pDestinationDeliveryInfos->at(DESTINATION(dli).usID).usDestinationFee = usDestinationFee;
	_DeliveryMethods[ubDeliveryMethodIndex].pDestinationDeliveryInfos->at(DESTINATION(dli).usID).bDaysAhead = bDaysAhead;
	
	return uiDestinationIndex;
}

UINT16 CPostalService::GetDestinationFee(UINT8 ubDeliveryMethodIndex, UINT16 usDestinationID)
{
	if(_UsedDestinationIDList.empty() || 
		usDestinationID > _UsedDestinationIDList.size() ||
		!_UsedDestinationIDList[usDestinationID])
	{
		Assert(0);
		return 0;
	}

	return _DeliveryMethods[ubDeliveryMethodIndex].pDestinationDeliveryInfos->at(usDestinationID).usDestinationFee;
}

RefToDestinationStruct CPostalService::_GetDestination(UINT16 usDestinationID)
{
	if(	usDestinationID > _UsedDestinationIDList.size() ||
		!_UsedDestinationIDList[usDestinationID])
	{
		Assert(0);
		return DESTINATION(_Destinations.end());
	}

	RefToDestinationListIterator dli = _Destinations.begin();

	while(DESTINATION(dli).usID != usDestinationID)
	{
		if(dli == _Destinations.end())
		{
			Assert(0);
			return DESTINATION(_Destinations.end());
		}
		dli++;
	}

	return DESTINATION(dli);
}