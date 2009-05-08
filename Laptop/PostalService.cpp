#include "SaveLoadMap.h"
#include "Structure Wrap.h"
#include "Tactical Save.h"
#include "postalservice.h"
#include "strategic.h"
#include "strategicmap.h"
#include "random.h"
#include "game clock.h"
#include <list>
#include <string>
#include <iostream>

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
		
	AddFutureDayStrategicEvent( EVENT_POSTAL_SERVICE_SHIPMENT, (8 + Random(4) ) * 60, usShipmentID, 
								SHIPMENT(sli).pDestinationDeliveryInfo->bDaysAhead);
	return TRUE;
}

BOOLEAN CPostalService::DeliverShipment(UINT16 usShipmentID)
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

	if( StrategicMap[ CALCULATE_STRATEGIC_INDEX( shs.pDestination->ubMapX, shs.pDestination->ubMapY ) ].fEnemyControlled )
	{
		return FALSE;
	}

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
				//ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(usItem, BIGPOCK1POS) );
				ubTempNumItems = __min( ubItemsDelivered, ItemSlotLimit(&tempObject, BIGPOCK1POS) );
				CreateItems( usItem, shs.ShipmentPackages[i].bItemQuality, ubTempNumItems, &tempObject );

				if( fSectorLoaded )
				{
					AddItemToPool( shs.pDestination->sGridNo, &tempObject, -1, 0, 0, 0 );
				}
				else
				{
					pObject[ uiCount ] = tempObject;
					uiCount++;
				}

				if (ubTempNumItems > 0)
					ubItemsDelivered -= ubTempNumItems;
				// WANNE: Temporary Bugfix, because we always get an endless loop in the while (ubTempNumItems is always 0 and therefore ubItemsDelivered always > 0!!)
				else
					ubItemsDelivered -= 1;
			}
		}

		if( !fSectorLoaded )
		{
			if( !AddItemsToUnLoadedSector(	shs.pDestination->ubMapX, shs.pDestination->ubMapY, 
				shs.pDestination->ubMapZ, shs.pDestination->sGridNo, uiCount, pObject, 0, 0, 0, -1, FALSE ) )
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
	
	ShipmentSaveFileDataStruct	sfs;
	ShipmentPackageStruct		sps;
	ShipmentStruct				shs;
		
	for(int i = 0; i < (int)sls.uiNumberOfShipments; i++)
	{
		// Read in shipment data header
		memset(&sfs, 0, sizeof(ShipmentSaveFileDataStruct));
		uiBytesRead = 0;
		FileRead(hFile, &sfs, sizeof(ShipmentSaveFileDataStruct), &uiBytesRead);
		if(uiBytesRead != sizeof(ShipmentSaveFileDataStruct))
		{
			return FALSE;
		}

		// Add shipment
		memset(&shs, 0, sizeof(ShipmentStruct));
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

UINT16 CPostalService::AddDestination(UINT32 uiIndex, UINT8 ubMapX, UINT8 ubMapY, UINT8 ubMapZ, INT16 sGridNo, STR16 pszName)
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