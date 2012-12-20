#ifndef _ARMS_DEALERS_INIT__H_
#define _ARMS_DEALERS_INIT__H_

#include "Store Inventory.h"

#define ADDITIONAL_ARMS_DEALERS		20	// Flugente: this does NOT include Tina, this is for the as-of-yet undetermined slots

//enums for the various arms dealers
enum
{
//Ja25 None are in exp.
	ARMS_DEALER_TONY,
	ARMS_DEALER_FRANZ,
	ARMS_DEALER_KEITH,
	ARMS_DEALER_JAKE,
	ARMS_DEALER_GABBY,
#ifdef JA2UB
//no UB
#else
	ARMS_DEALER_DEVIN,
#endif

	ARMS_DEALER_HOWARD,
	ARMS_DEALER_SAM,
	ARMS_DEALER_FRANK,

	ARMS_DEALER_BAR_BRO_1,
	ARMS_DEALER_BAR_BRO_2,
	ARMS_DEALER_BAR_BRO_3,
	ARMS_DEALER_BAR_BRO_4,

	ARMS_DEALER_MICKY,

	ARMS_DEALER_ARNIE,
	ARMS_DEALER_FREDO,
	
#ifdef JA2UB
	ARMS_DEALER_RAUL,
#else
	ARMS_DEALER_PERKO,
#endif

// added only in GameVersion 54
	ARMS_DEALER_ELGIN,

// added only in GameVersion 55
	ARMS_DEALER_MANNY,

#ifdef JA2UB
	ARMS_DEALER_BETTY,
#endif

	// Flugente: added new merchants (and slots for even more of them)
	ARMS_DEALER_TINA,

	ARMS_DEALER_ADDITIONAL_1,

	NUM_ARMS_DEALERS = ARMS_DEALER_ADDITIONAL_1 + ADDITIONAL_ARMS_DEALERS,
};

#define NUM_ORIGINAL_ARMS_DEALERS 19

//the enums for the different kinds of arms dealers
enum
{
	ARMS_DEALER_BUYS_SELLS,
	ARMS_DEALER_SELLS_ONLY,
	ARMS_DEALER_BUYS_ONLY,
	ARMS_DEALER_REPAIRS,
};


//The following defines indicate what items can be sold by the arms dealer
#define		ARMS_DEALER_HANDGUNCLASS			0x00000001	// 1
#define		ARMS_DEALER_SMGCLASS				0x00000002	// 2
#define		ARMS_DEALER_RIFLECLASS				0x00000004	// 4
#define		ARMS_DEALER_MGCLASS					0x00000008	// 8
#define		ARMS_DEALER_SHOTGUNCLASS			0x00000010	// 16

#define		ARMS_DEALER_KNIFECLASS				0x00000020	// 32

#define		ARMS_DEALER_BLADE					0x00000040	// 64
#define		ARMS_DEALER_LAUNCHER				0x00000080	// 128

#define		ARMS_DEALER_ARMOUR					0x00000100	// 256
#define		ARMS_DEALER_MEDKIT					0x00000200	// 512
#define		ARMS_DEALER_MISC					0x00000400	// 1024
#define		ARMS_DEALER_AMMO					0x00000800	// 2048

#define		ARMS_DEALER_GRENADE					0x00001000	// 4096
#define		ARMS_DEALER_BOMB					0x00002000	// 8192
#define		ARMS_DEALER_EXPLOSV					0x00004000	// 16384

#define		ARMS_DEALER_KIT						0x00008000	// 32764

#define		ARMS_DEALER_FACE					0x00010000	// 65536
//#define		ARMS_DEALER_THROWN				0x00020000	// 131072
//#define		ARMS_DEALER_KEY					0x00040000	// 262144

//#define		ARMS_DEALER_VIDEO_CAMERA		0x00020000	// 131072

#define		ARMS_DEALER_DETONATORS				0x00040000	// 262144

#define		ARMS_DEALER_ATTACHMENTS				0x00080000	// 524288


#define		ARMS_DEALER_ALCOHOL					0x00100000	// 1048576
#define		ARMS_DEALER_ELECTRONICS				0x00200000	// 2097152
#define		ARMS_DEALER_HARDWARE				0x00400000	| ARMS_DEALER_KIT

#define		ARMS_DEALER_MEDICAL					0x00800000	| ARMS_DEALER_MEDKIT

//#define		ARMS_DEALER_EMPTY_JAR			0x01000000	// 16777216
#define		ARMS_DEALER_CREATURE_PARTS			0x02000000	// 33554432
#define		ARMS_DEALER_ROCKET_RIFLE			0x04000000	// 67108864

#define		ARMS_DEALER_ONLY_USED_ITEMS			0x08000000	// 134217728
#define		ARMS_DEALER_GIVES_CHANGE			0x10000000	// 268435456	//The arms dealer will give the required change when doing a transaction
#define		ARMS_DEALER_ACCEPTS_GIFTS			0x20000000	// 536870912	//The arms dealer is the kind of person who will accept gifts
#define		ARMS_DEALER_SOME_USED_ITEMS			0x40000000	// 1073741824	//The arms dealer can have used items in his inventory
#define		ARMS_DEALER_HAS_NO_INVENTORY		0x80000000	// 2147483648	//The arms dealer does not carry any inventory


#define		ARMS_DEALER_BIG_GUNS				ARMS_DEALER_SMGCLASS | ARMS_DEALER_RIFLECLASS | ARMS_DEALER_MGCLASS | ARMS_DEALER_SHOTGUNCLASS

#define		ARMS_DEALER_ALL_GUNS				ARMS_DEALER_HANDGUNCLASS | ARMS_DEALER_BIG_GUNS

#define		ARMS_DEALER_ALL_WEAPONS				ARMS_DEALER_ALL_GUNS | ARMS_DEALER_BLADE | ARMS_DEALER_LAUNCHER | ARMS_DEALER_KNIFECLASS

#ifdef JA2UB
//Raul
#define	ARMS_DEALER_FLAG__RAUL_HAS_SOLD_BARRETT_TO_PLAYER			0x00000001	// Raul has sold the Barrett to the player
#define	ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_48						0x00000002	// Raul said the quote for when the player first puts the hand cannon down
#define	ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_49						0x00000004	// Quote for when player removes hand cannon from players offer area
#define	ARMS_DEALER_FLAG__RAUL_SAID_QUOTE_50						0x00000008	// Quote for when player adds the hand cannon AGAIN into the players offer area
#endif


//
// Specific Dealer Flags
// NOTE: Each dealer has 8 flags, but different dealers can and SHOULD share the same flag #s!
//

// Alex Fredo
#define	ARMS_DEALER_FLAG__FREDO_HAS_SAID_ROCKET_RIFLE_QUOTE			0x00000001	// Alex Fredo has already repaired the Rocket Rifle
// Franz Hinkle
#define	ARMS_DEALER_FLAG__FRANZ_HAS_SOLD_VIDEO_CAMERA_TO_PLAYER		0x00000001	// Franz Hinkle has sold the video camera to the player


// THIS STRUCTURE HAS UNCHANGING INFO THAT DOESN'T GET SAVED/RESTORED/RESET
typedef struct
{
	// added for xml externalisation by Flugente 2012-12-18
	UINT16		uiIndex;	
	CHAR16		szName[80];					// name of this merchant (used in xml, not used ingame)

	union
	{
		struct
		{
			FLOAT		dBuyModifier;				// The price modifier used when this dealer is BUYING something.
			FLOAT		dSellModifier;			// The price modifier used when this dealer is SELLING something.
		};
		struct
		{
			FLOAT		dRepairSpeed;				// Modifier to the speed at which a repairman repairs things
			FLOAT		dRepairCost;				// Modifier to the price a repairman charges for repairs
		};
	};

	UINT8		ubShopKeeperID;					// Merc Id for the dealer
	UINT8		ubTypeOfArmsDealer;			// Whether he buys/sells, sells, buys, or repairs
	INT32		iInitialCash;						// How much cash dealer starts with (we now reset to this amount once / day)
	UINT32		uiFlags;							// various flags which control the dealer's operations

	UINT32		dailyIncrement;
	UINT32		dailyMaximum;
	UINT32		dailyRetained;			// %
	UINT32		minCoolness;			// 0-10
	UINT32		maxCoolness;			// 0-10
	UINT32		addToCoolness;			// 0-10
	UINT32		coolnessProgressRate;	// %
	UINT32		daysDelayMin;			// 1-10
	UINT32		daysDelayMax;			// 1-10
	bool		useBRSetting;
	bool		allInventoryAlwaysAvailable;
} ARMS_DEALER_INFO;


// THIS STRUCTURE GETS SAVED/RESTORED/RESET
class OLD_ARMS_DEALER_STATUS_101
{
public:
	UINT32	uiArmsDealersCash;			// How much money the arms dealer currently has

	UINT8	ubSpecificDealerFlags;		// Misc state flags for specific dealers
	BOOLEAN fOutOfBusiness;				// Set when a dealer has been killed, etc.
	BOOLEAN fRepairDelayBeenUsed;		// Set when a repairman has missed his repair time estimate & given his excuse for it
	BOOLEAN	fUnusedKnowsPlayer;			// Set if the shopkeeper has met with the player before [UNUSED]

	UINT32	uiTimePlayerLastInSKI;		// game time (in total world minutes) when player last talked to this dealer in SKI

	UINT8		ubPadding[ 8 ];

};

// THIS STRUCTURE GETS SAVED/RESTORED/RESET
class ARMS_DEALER_STATUS
{
public:
	ARMS_DEALER_STATUS() {initialize();};
    ARMS_DEALER_STATUS& operator=(const OLD_ARMS_DEALER_STATUS_101& status);
	void	initialize();
	BOOLEAN Save(HWFILE hFile);
	BOOLEAN Load(HWFILE hFile);

	UINT32	uiArmsDealersCash;			// How much money the arms dealer currently has
	UINT8	ubSpecificDealerFlags;		// Misc state flags for specific dealers
	BOOLEAN fOutOfBusiness;				// Set when a dealer has been killed, etc.
	BOOLEAN fRepairDelayBeenUsed;		// Set when a repairman has missed his repair time estimate & given his excuse for it
	BOOLEAN	fUnusedKnowsPlayer;			// Set if the shopkeeper has met with the player before [UNUSED]
	UINT32	uiTimePlayerLastInSKI;		// game time (in total world minutes) when player last talked to this dealer in SKI
	
	BOOLEAN	fHasSoldSomethingToPlayer;	//If set, the player has at some point sold something to the player

	UINT16	ubStrayAmmo[MAXITEMS];			// partially-depleted ammo mags are stored here as #bullets, and can be converted to full packs
	BOOLEAN	fPreviouslyEligible[MAXITEMS];	// whether or not dealer has been eligible to sell this item in days prior to today
};



class OLD_SPECIAL_ITEM_INFO_101
{
public:
	UINT16	usAttachment[OLD_MAX_ATTACHMENTS_101];		// item index of any attachments on the item

	INT8		bItemCondition;				// if 0, no item is stored
																// from 1 to 100 indicates an item with that status
																// -1 to -100 means the item is in for repairs, flip sign for the actual status

	UINT8		ubImprintID;					// imprint ID for imprinted items (during repair!)

	INT8		bAttachmentStatus[OLD_MAX_ATTACHMENTS_101];	// status of any attachments on the item

	UINT8		ubPadding[2];					// filler

};

class OLD_DEALER_SPECIAL_ITEM_101
{
public:
	// Individual "special" items are stored here as needed, *one* per slot
	// An item is special if it is used (status < 100), has been imprinted, or has a permanent attachment

	OLD_SPECIAL_ITEM_INFO_101 oldInfo;

	UINT32	uiRepairDoneTime;			// If the item is in for repairs, this holds the time when it will be repaired (in min)

	BOOLEAN fActive;							// TRUE means an item is stored here (empty elements may not always be freed immediately)

	UINT8		ubOwnerProfileId;			// stores which merc previously owned an item being repaired

	UINT8		ubPadding[6];					// filler

};

class OLD_DEALER_ITEM_HEADER_101
{
public:
	// Non-special items are all the identical and are totaled inside ubPerfectItems.
	// Items being repaired are also stored here, with a negative condition.
	// NOTE: special item elements may remain allocated long after item has been removed, to reduce memory fragmentation!!!

	UINT8		ubTotalItems;					// sum of all the items (all perfect ones + all special ones)
	UINT8		ubPerfectItems;				// non-special (perfect) items held by dealer
	UINT8		ubStrayAmmo;					// partially-depleted ammo mags are stored here as #bullets, and can be converted to full packs

	UINT8		ubElementsAlloced;		// number of DEALER_SPECIAL_ITEM array elements alloced for the special item array
	OLD_DEALER_SPECIAL_ITEM_101 *SpecialItem;	// dynamic array of special items with this same item index

	UINT32	uiOrderArrivalTime;		// Day the items ordered will arrive on.	It's UINT32 in case we change this to minutes.
	UINT8		ubQtyOnOrder;					// The number of items currently on order
	BOOLEAN	fPreviouslyEligible;	// whether or not dealer has been eligible to sell this item in days prior to today

	UINT8		ubPadding[2];					// filler

};


class DEALER_SPECIAL_ITEM
{
public:
	DEALER_SPECIAL_ITEM() {initialize();};
	void ConvertFrom101(OLD_DEALER_ITEM_HEADER_101& header, OLD_DEALER_SPECIAL_ITEM_101& special, int ubArmsDealer, int usItemIndex);
	BOOLEAN Save(HWFILE hFile);
	BOOLEAN Load(HWFILE hFile);
	void	initialize();
	bool	operator == (const DEALER_SPECIAL_ITEM& compare);

	bool	IsUnderRepair() {return bItemCondition < 0;};
	bool	IsBeingOrdered() {return ubQtyOnOrder > 0;};
	bool	ItemIsInInventory() {return (this->object.exists() == true && this->IsBeingOrdered() == false);};

	//data from OLD_SPECIAL_ITEM_INFO_101
	//UINT8		ubImprintID;				// imprint ID for imprinted items (during repair!)
	INT16		bItemCondition;				// if 0, no item is stored
											// from 1 to 100 indicates an item with that status
											// -1 to -100 means the item is in for repairs, flip sign for the actual status


	//data from OLD_DEALER_SPECIAL_ITEM_101
	UINT32		uiRepairDoneTime;			// If the item is in for repairs, this holds the time when it will be repaired (in min)
	//BOOLEAN		fActive;					// TRUE means an item is stored here (empty elements may not always be freed immediately)
	UINT8		ubOwnerProfileId;			// stores which merc previously owned an item being repaired


	//data from OLD_DEALER_ITEM_HEADER_101
	UINT32		uiOrderArrivalTime;			// Day the items ordered will arrive on.	It's UINT32 in case we change this to minutes.
	UINT8		ubQtyOnOrder;				// The number of items currently on order

	char		endOfPod;
	OBJECTTYPE	object;
};
#define SIZEOF_DEALER_SPECIAL_ITEM_POD offsetof(DEALER_SPECIAL_ITEM, endOfPod)


typedef std::list<DEALER_SPECIAL_ITEM> DealerItemList;
extern std::vector<DealerItemList>	gArmsDealersInventory;
void OrderDealerItems(int armsDealer, int usItem, int numItems, int arrivalDay);

// Flugente 2012-12-19: merchant data has been externalised - see XML_Merchants.cpp
//extern const ARMS_DEALER_INFO	DefaultarmsDealerInfo[ NUM_ARMS_DEALERS ];
extern std::vector<ARMS_DEALER_INFO>	armsDealerInfo;
extern ARMS_DEALER_STATUS		gArmsDealerStatus[ NUM_ARMS_DEALERS ];

#ifdef JA2UB
void AddTexsVideosToBettysInventory();
#endif


void		InitAllArmsDealers();
void		ShutDownArmsDealers();
void		InitializeOneArmsDealer( UINT8 ubArmsDealer );

bool	ItemIsSpecial(DEALER_SPECIAL_ITEM& item);
UINT32	GetArmsDealerItemTypeFromItemNumber( UINT16 usItem );

//Count every single item the dealer has in stock
//UINT32	CountTotalItemsInArmsDealersInventory( UINT8 ubArmsDealer );
//Count only the # of "distinct" item types (for shopkeeper purposes)
UINT16	CountTotalItemsRepairDealerHasInForRepairs( UINT8 ubArmsDealer );
UINT8		CountSpecificItemsRepairDealerHasInForRepairs( UINT8 ubArmsDealer, UINT16 usItemIndex );

void		AddObjectToArmsDealerInventory( UINT8 ubArmsDealer, OBJECTTYPE *pObject );

void		RemoveItemFromArmsDealerInventory( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany, OBJECTTYPE *pObj = NULL );

BOOLEAN IsMercADealer( UINT8 ubMercID );
INT8		GetArmsDealerIDFromMercID( UINT8 ubMercID );

BOOLEAN SaveArmsDealerInventoryToSaveGameFile( HWFILE hFile );

void		DailyUpdateOfArmsDealersInventory();

UINT8		GetTypeOfArmsDealer( UINT8 ubDealerID );

BOOLEAN	DoesDealerDoRepairs( UINT8 ubArmsDealer );
BOOLEAN RepairmanIsFixingItemsButNoneAreDoneYet( UINT8 ubProfileID );

UINT32	GetTimeToFixItemBeingRepaired( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubElement );

BOOLEAN CanDealerTransactItem( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fPurchaseFromPlayer );
BOOLEAN CanDealerRepairItem( UINT8 ubArmsDealer, UINT16 usItemIndex );

BOOLEAN AddDeadArmsDealerItemsToWorld( UINT8 ubMercID );

void		MakeObjectOutOfDealerItems( DEALER_SPECIAL_ITEM *pSpclItemInfo, OBJECTTYPE *pObject );
void		CreateObjectForDealer( int usItem, int status, int numObjects, OBJECTTYPE *pObject );

void		GiveObjectToArmsDealerForRepair( UINT8 ubArmsDealer, OBJECTTYPE *pObject, UINT8 ubOwnerProfileId );
void		GiveItemToArmsDealerforRepair( UINT8 ubArmsDealer, OBJECTTYPE* pObject, UINT8 ubOwnerProfileId );
UINT32	WhenWillRepairmanBeAllDoneRepairing( UINT8 ubArmsDealer );

UINT32 CalculateObjectItemRepairTime( UINT8 ubArmsDealer, OBJECTTYPE *pItemObject );
UINT32 CalculateSimpleItemRepairTime( UINT8 ubArmsDealer, UINT16 usItemIndex, INT16 bItemCondition );

UINT32 CalculateObjectItemRepairCost( UINT8 ubArmsDealer, OBJECTTYPE *pItemObject );
UINT32 CalculateSimpleItemRepairCost( UINT8 ubArmsDealer, UINT16	usItemIndex, INT16 bItemCondition );


UINT16	CalcValueOfItemToDealer( UINT8 ubArmsDealer, UINT16 usItemIndex, BOOLEAN fDealerSelling );

UINT32 CalculateOvernightRepairDelay( UINT8 ubArmsDealer, UINT32 uiTimeWhenFreeToStartIt, UINT32 uiMinutesToFix );
UINT32 CalculateMinutesClosedBetween( UINT8 ubArmsDealer, UINT32 uiStartTime, UINT32 uiEndTime );
#ifdef JA2UB
extern void DailyCheckOnItemQuantities( BOOLEAN fInstallyHaveItemsAppear ); //Ja25 UB
#endif
extern void		GuaranteeAtLeastXItemsOfIndex( UINT8 ubArmsDealer, UINT16 usItemIndex, UINT8 ubHowMany );



#endif