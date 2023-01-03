#ifndef _LAPTOP_SAVE_H_
#define _LAPTOP_SAVE_H_

//#include "Item Types.h"
#include "FileMan.h"
#include "Store Inventory.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


#define		MAX_BOOKMARKS											20



//#define		MAX_PURCHASE_AMOUNT								20




#define	SPECK_QUOTE__ALREADY_TOLD_PLAYER_THAT_LARRY_RELAPSED		0x00000001
#define SPECK_QUOTE__SENT_EMAIL_ABOUT_LACK_OF_PAYMENT						0x00000002


typedef struct
{
	BOOLEAN	fActive;
	UINT8		ubSoldierID;
	UINT8		ubMercID;
	INT32		iPayOutPrice;
} LIFE_INSURANCE_PAYOUT;


typedef struct
{
	BOOLEAN	fHaveDisplayedPopUpInLaptop;		// Is set when the popup gets displayed, reset when entering laptop again.
	INT32		iIdOfMerc;
	UINT32	uiArrivalTime;
} LAST_HIRED_MERC_STRUCT;


typedef struct
{
	UINT16	usItemIndex;
	UINT8		ubNumberPurchased;
	INT8		bItemQuality;
	UINT16	usBobbyItemIndex;						//Item number in the BobbyRayInventory structure
	BOOLEAN	fUsed;											//Indicates wether or not the item is from the used inventory or the regular inventory
} BobbyRayPurchaseStruct;



typedef struct
{
	BOOLEAN	fActive;
	BobbyRayPurchaseStruct BobbyRayPurchase[ 100 ];
	UINT8	ubNumberPurchases;
} BobbyRayOrderStruct;


//used when the player goes to bobby rays when it is still down
enum
{
	BOBBYR_NEVER_BEEN_TO_SITE,
	BOBBYR_BEEN_TO_SITE_ONCE,
	BOBBYR_ALREADY_SENT_EMAIL,
};

// Flugente: intel
#define INTELINFO_MAXNUMBER		2

typedef struct
{
	//General Laptop Info
	BOOLEAN							gfNewGameLaptop;									//Is it the firs time in Laptop
	BOOLEAN							fVisitedBookmarkAlready[20];			// have we visitied this site already?
	INT32							iBookMarkList[MAX_BOOKMARKS];

	
	INT32							iCurrentBalance;									// current players balance


	//IMP Information
	BOOLEAN							fIMPCompletedFlag;						// Has the player Completed the IMP process
	BOOLEAN							fSentImpWarningAlready;				// Has the Imp email warning already been sent


	//Personnel Info
	INT16 ubDeadCharactersList[ 256 ];
	INT16 ubLeftCharactersList[ 256 ];
	INT16 ubOtherCharactersList[ 256 ];


	// MERC site info
	UINT8								gubPlayersMercAccountStatus;
	UINT32							guiPlayersMercAccountNumber;
	UINT8								gubLastMercIndex;


	// Aim Site


	// BobbyRay Site
	STORE_INVENTORY		BobbyRayInventory[ MAXITEMS ];
	STORE_INVENTORY		BobbyRayUsedInventory[ MAXITEMS ];

	BobbyRayOrderStruct *BobbyRayOrdersOnDeliveryArray;
	UINT8								usNumberOfBobbyRayOrderItems;				// The number of elements in the array
	UINT8								usNumberOfBobbyRayOrderUsed;				// The number of items in the array that are used


	// Flower Site
	//NONE

	// Insurance Site
	LIFE_INSURANCE_PAYOUT	*pLifeInsurancePayouts;
	UINT8								ubNumberLifeInsurancePayouts;				// The number of elements in the array
	UINT8								ubNumberLifeInsurancePayoutUsed;		// The number of items in the array that are used

	BOOLEAN							fBobbyRSiteCanBeAccessed;

	UINT8								ubPlayerBeenToMercSiteStatus;
	BOOLEAN							fFirstVisitSinceServerWentDown;
	BOOLEAN							fNewMercsAvailableAtMercSite;
	BOOLEAN							fSaidGenericOpeningInMercSite;
	BOOLEAN							fSpeckSaidFloMarriedCousinQuote;
	BOOLEAN							fHasAMercDiedAtMercSite;

#ifdef CRIPPLED_VERSION
	UINT8 ubCrippleFiller[20];
#endif


	INT8								gbNumDaysTillFirstMercArrives;
	INT8								gbNumDaysTillSecondMercArrives;
	INT8								gbNumDaysTillThirdMercArrives;
	INT8								gbNumDaysTillFourthMercArrives;

	UINT32							guiNumberOfMercPaymentsInDays;				// Keeps track of each day of payment the MERC site gets

	UINT16							usInventoryListLength[BOBBY_RAY_LISTS];

	INT32								iIMPIndex;

	UINT8								ubHaveBeenToBobbyRaysAtLeastOnceWhileUnderConstruction;

	BOOLEAN							fMercSiteHasGoneDownYet;

	UINT8								ubSpeckCanSayPlayersLostQuote;

	LAST_HIRED_MERC_STRUCT	sLastHiredMerc;

	INT32								iCurrentHistoryPage;
	INT32								iCurrentFinancesPage;
	INT32								iCurrentEmailPage;

	UINT32							uiSpeckQuoteFlags;

	UINT32							uiFlowerOrderNumber;

	UINT32							uiTotalMoneyPaidToSpeck;

	UINT8								ubLastMercAvailableId;

	// Flugente: moved here. These variables always exist, as havin #ifdefs in structs is needlessly obscure
	UINT32							uiJohnEscortedDate;// anv: Remember when Kulbas were escorted out, so John can be recruited later
	BOOLEAN							bJohnEscorted;// and if they were at all
	UINT8							ubJohnPossibleMissedFlights;// and how many flights he already missed

	// Flugente: militia volunteer pool. No relation to the laptop whatsoever, but I'm not creating a new struct for one variable
	FLOAT							dMilitiaVolunteerPool;

	// Flugente: additional militia resources
	FLOAT							dMilitiaGunPool;
	FLOAT							dMilitiaArmourPool;
	FLOAT							dMilitiaMiscPool;

	// Flugente: intel
	FLOAT							dIntelPool;
	UINT32							usMapIntelFlags;
	INT16							sIntelInfoForThisHour[INTELINFO_MAXNUMBER];

	// Flugente: raid
	INT32							sRaidBloodcats;
	INT32							sRaidZombies;
	INT32							sRaidBandits;

	UINT8 bPadding[ 36 ];

} LaptopSaveInfoStruct;





extern	LaptopSaveInfoStruct LaptopSaveInfo;

extern BobbyRayPurchaseStruct BobbyRayPurchases[ 100 ];



void LaptopSaveVariablesInit();
BOOLEAN LoadLaptopInfoFromSavedGame( HWFILE hFile );
BOOLEAN SaveLaptopInfoToSavedGame( HWFILE hFile );


#endif




 


