#ifndef __POSTAL_SERVICE_H
#define __POSTAL_SERVICE_H

#include "SaveLoadMap.h"
#include "Structure Wrap.h"
#include "Tactical Save.h"
#include "laptopsave.h"
#include "postalservice.h"
#include "isometric utils.h"
#include "debug.h"
#include "game event hook.h"
#include "game events.h"
#include <list>
#include <string>
#include <iostream>

using namespace std;

#define MAX_DEST_NAME_LENGTH	32
#define MAX_DELIVERYMETHOD_DESC_LENGTH 32
#define MAX_DESTINATIONS		255
#define MAX_SHIPMENTS			255

//////////////////////////////////////////////

class CPostalService;
typedef CPostalService& RefToCPostalService;
typedef CPostalService* PCPostalService;

//////////////////////////////////////////////

typedef struct
{
	UINT16	usID;
	UINT32	uiIndex;	// uiIndex is used as an ID number in external game data
	UINT8	ubMapY;
	UINT8	ubMapX;
	UINT8	ubMapZ;	
	UINT32	sGridNo;
	wstring wstrName;
} DestinationStruct;

typedef DestinationStruct& RefToDestinationStruct;
typedef DestinationStruct* PDestinationStruct;

typedef list<DestinationStruct> DestinationList;
typedef list<DestinationStruct>& RefToDestinationList;
typedef DestinationList::const_iterator& RefToDestinationListIterator;
inline RefToDestinationStruct DESTINATION(RefToDestinationListIterator dli)
{
	return ( (RefToDestinationStruct) *dli );
}

inline BOOLEAN DESTINATION_ID_GREATER(RefToDestinationStruct d1, RefToDestinationStruct d2)
{
	return (d1.usID > d2.usID);
}
#define DESTINATION_LIST_DESCENDING DESTINATION_ID_GREATER

inline BOOLEAN DESTINATION_ID_LOWER(RefToDestinationStruct d1, RefToDestinationStruct d2)
{
	return (d1.usID < d2.usID);
}
#define DESTINATION_LIST_ASCENDING DESTINATION_ID_LOWER

inline BOOLEAN DESTINATION_ID_EQUAL(RefToDestinationStruct d1, RefToDestinationStruct d2)
{
	return (d1.usID == d2.usID);
}

//////////////////////////////////////////////////////////////
typedef struct
{
	UINT8				ubParentDeliveryMethodIndex;
	UINT16				usDestinationFee;
	INT8				bDaysAhead;
}DestinationDeliveryInfoStruct;
typedef DestinationDeliveryInfoStruct& RefToDestinationDeliveryInfoStruct;
typedef DestinationDeliveryInfoStruct* PDestinationDeliveryInfoStruct;
typedef vector<DestinationDeliveryInfoStruct> DestinationDeliveryInfoTable;
typedef DestinationDeliveryInfoTable* PDestinationDeliveryInfoTable;
typedef DestinationDeliveryInfoTable& RefToDestinationDeliveryInfoTable;
typedef DestinationDeliveryInfoTable::iterator DestinationDeliveryInfoTableIterator;
typedef DestinationDeliveryInfoTableIterator& RefToDestinationDeliveryInfoTableIterator;

typedef struct
{
	wstring wstrDescription;
	PDestinationDeliveryInfoTable pDestinationDeliveryInfos;
} DeliveryMethodStruct;
typedef DeliveryMethodStruct& RefToDeliveryMethodStruct;
typedef DeliveryMethodStruct* PDeliveryMethodStruct;
typedef vector<DeliveryMethodStruct> DeliveryMethodTable;
typedef DeliveryMethodTable& RefToDeliveryMethodTable;

/////////////////////////////////////////////////////////////
typedef struct
{
	UINT16	usItemIndex;
	UINT8	ubNumber;
	INT8	bItemQuality;
} ShipmentPackageStruct;
typedef ShipmentPackageStruct& RefToShipmentPackageStruct;
typedef ShipmentPackageStruct* PShipmentPackageStruct;
typedef vector<ShipmentPackageStruct> ShipmentPackageList;
typedef ShipmentPackageList& RefToShipmentPackageList;
typedef ShipmentPackageList::iterator& RefToShipmentPackageListIterator;

typedef enum
{
	SHIPMENT_STATIONARY=0,
	SHIPMENT_INTRANSIT,
	SHIPMENT_CANCEL_DELIVERY
} SHIPMENT_STATUS;

typedef struct
{
	UINT16							usID;
	SHIPMENT_STATUS					ShipmentStatus;
	ShipmentPackageList				ShipmentPackages;
	PDestinationStruct				pDestination;
	PDestinationDeliveryInfoStruct	pDestinationDeliveryInfo;
	INT16							sSenderID;		// For now, two senders exist: Bobby Ray(0) and John Kulba(1)
	INT16							sReceiverID;	// Unused for now, but reserved for future projects	
	UINT32							uiOrderDate;
} ShipmentStruct;
typedef ShipmentStruct& RefToShipmentStruct;
typedef ShipmentStruct* PShipmentStruct;
typedef list<ShipmentStruct> ShipmentList;
typedef ShipmentList* PShipmentList;
typedef ShipmentList& RefToShipmentList;
typedef ShipmentList::iterator ShipmentListIterator;
typedef ShipmentListIterator& RefToShipmentListIterator;

typedef struct
{
	UINT32 uiNumberOfShipments;
	UINT32 uiShipmentDataSize;
	UINT32 uiPackageDataSize;
}ShipmentListSaveFileDataHeaderStruct;
typedef ShipmentListSaveFileDataHeaderStruct* PShipmentListSaveFileDataHeaderStruct;

typedef struct
{
	UINT16				usID;
	SHIPMENT_STATUS		ShipmentStatus;
	UINT32				uiNumberOfPackages;
	UINT16				usDestinationID;
	UINT8				ubDeliveryMethodIndex;
	INT16				sSenderID;
	INT16				sReceiverID;
	UINT32				uiOrderDate;
} ShipmentSaveFileDataStruct;
typedef ShipmentSaveFileDataStruct* PShipmentSaveFileDataStruct;

inline BOOLEAN SHIPMENT_ID_LOWER(RefToShipmentStruct s1, RefToShipmentStruct s2)
{
	return (s1.usID < s2.usID);
}
#define SHIPMENT_LIST_ASCENDING SHIPMENT_ID_LOWER

inline RefToShipmentStruct SHIPMENT(RefToShipmentListIterator sli)
{
	return ( (RefToShipmentStruct) *sli );
}
////////////////////////////////////////////////////////////////
class CShipmentManipulator
{
	friend CPostalService;
public:
	INT16 GetSenderID(void);
	RefToDestinationStruct GetDestination(void) const;
	void ContinueCallbackChain(BOOLEAN fContinue);
	void CancelDelivery(void);
	CShipmentManipulator(PCPostalService Owner, PShipmentStruct Shipment);
private:
	PCPostalService	_PostalService;
	PShipmentStruct _Shipment;
	BOOLEAN			_fContinueCallbackChain;
};
typedef CShipmentManipulator& RefToCShipmentManipulator;
typedef CShipmentManipulator* PCShipmentManipulator;

typedef void (*PtrToDeliveryCallbackFunc)(RefToCShipmentManipulator ShipmentManipulator);

typedef struct
{
	INT16 sSenderID;
	PtrToDeliveryCallbackFunc DeliveryCallbackFunc;
} DeliveryCallbackDataStruct;
typedef DeliveryCallbackDataStruct& RefToDeliveryCallbackDataStruct;
typedef vector<DeliveryCallbackDataStruct> DeliveryCallbackDataList;
typedef DeliveryCallbackDataList& RefToDeliveryCallbackDataList;
typedef DeliveryCallbackDataList::iterator DeliveryCallbackDataListIterator;
typedef DeliveryCallbackDataListIterator& RefToDeliveryCallbackDataListIterator;

////////////////////////////////////////////////////////////////
class CPostalService
{
	friend BOOLEAN ExecuteStrategicEvent( STRATEGICEVENT *pEvent );
	friend CShipmentManipulator;
public:
	// Shipment management
	UINT16 CreateNewShipment(UINT16 usDestinationID, UINT8  ubDeliveryMethodIndex, INT16 sSenderID);
	BOOLEAN AddPackageToShipment(UINT16 usShipmentID, UINT16 usItemIndex, UINT8 ubNumber, INT8 bItemQuality); 
	BOOLEAN SendShipment(UINT16 usShipmentID);
	BOOLEAN DeliverShipment(UINT16 usShipmentID);
	BOOLEAN DeliverShipmentForMultiplayer(UINT16 usShipmentID);
	BOOLEAN RegisterDeliveryCallback(INT16 sSenderID, PtrToDeliveryCallbackFunc DeliveryCallbackFunc);	
	
	// Interfaces
	BOOLEAN LoadShipmentListFromSaveGameFile(HWFILE hFile);
	BOOLEAN SaveShipmentListToSaveGameFile(HWFILE hFile);

	// Shipment utility functions
	UINT16 GetShipmentCount(SHIPMENT_STATUS TargetedShipmentStatus);
	RefToShipmentList LookupShipmentList(void) const;

	
	// Destination management
	UINT16 AddDestination(UINT32 uiIndex, UINT8 ubMapX, UINT8 ubMapY, UINT8 ubMapZ, UINT32 sGridNo, STR16 pszName );
	UINT16 RemoveDestination(UINT16 usDestinationID);
	RefToDestinationStruct GetDestination(UINT16 usDestinationID) const;
	RefToDestinationList LookupDestinationList(void) const;
	
	// WANNE: 
	BOOLEAN IsSectorAShipmentSector(UINT8 ubMapX, UINT8 ubMapY, UINT8 ubMapZ);

	// Delivery method management
	UINT8 AddDeliveryMethod(STR16 pszDescription);
	UINT16 SetDestinationDeliveryInfo(UINT8 ubDeliveryMethodIndex, UINT32 uiDestinationIndex, UINT16 usDestinationFee, INT8 bDaysAhead);
	UINT16 GetDestinationFee(UINT8 ubDeliveryMethodIndex, UINT16 usDestinationID);
	RefToDeliveryMethodStruct GetDeliveryMethod(UINT8 ubDeliveryMethodIndex) const;

	void Clear(bool clearOnlyData);
	CPostalService();
private:
	ShipmentList _Shipments;
	vector<BOOLEAN> _UsedShipmentIDList;
	DeliveryCallbackDataList _DeliveryCallbacks;
	static OBJECTTYPE	tempObject;
	

	// All instances share the same set of destinations...
	static DestinationList _Destinations;
	static vector<BOOLEAN> _UsedDestinationIDList;

	RefToDestinationStruct _GetDestination(UINT16 usDestinationID);

	// ...but they may have different delivery methods available so these are managed individually
	DeliveryMethodTable _DeliveryMethods;

	RefToDestinationDeliveryInfoStruct _GetDestinationDeliveryInfo(UINT8 ubDeliveryMethodIndex, UINT16 usDestinationID);
protected:
};

#endif