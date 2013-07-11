#ifndef __MERCS_H
#define __MERCS_H

#include "soldier profile type.h"

#define		MERC_BUTTON_UP_COLOR													FONT_MCOLOR_WHITE
#define		MERC_BUTTON_DOWN_COLOR												FONT_MCOLOR_DKWHITE

extern UINT8	NUMBER_OF_MERCS;
extern UINT8	LAST_MERC_ID;

#define		MAX_NUMBER_OF_MERCS		28

//#define		NUMBER_OF_BAD_MERCS		5

extern UINT8 NUMBER_OF_BAD_MERCS;

#define		NUMBER_MERCS_AFTER_FIRST_MERC_ARRIVES					6
#define		NUMBER_MERCS_AFTER_SECOND_MERC_ARRIVES				8
#define		NUMBER_MERCS_AFTER_THIRD_MERC_ARRIVES					9
#define		NUMBER_MERCS_AFTER_FOURTH_MERC_ARRIVES				10

#define		MERC_NUM_DAYS_TILL_FIRST_WARNING							7
#define		MERC_NUM_DAYS_TILL_ACCOUNT_SUSPENDED					9
#define		MERC_NUM_DAYS_TILL_ACCOUNT_INVALID						12


// WANNE.LARRY
//#define		MERC_LARRY_ROACHBURN													7


#define		DAYS_TIL_M_E_R_C_AVAIL												3

enum
{
	MERC_ARRIVES_BUBBA,
	MERC_ARRIVES_LARRY,
	MERC_ARRIVES_NUMB,
	MERC_ARRIVES_TEX,
	MERC_ARRIVES_BIGGENS,
	MERC_ARRIVES_COUGAR,
	MERC_ARRIVES_GASTON,
	MERC_ARRIVES_STOGIE,
	MERC_ARRIVES_NEW = MERC_ARRIVES_STOGIE + 240,
	
	NUM_MERC_ARRIVALS,
};

typedef struct
{
	UINT16	usMoneyPaid;
	UINT16	usDay;
	UINT8   ubMercArrayID;
	UINT8   uiIndex;
	UINT8   ProfilId;
	BOOLEAN NewMercsAvailable;
	BOOLEAN StartMercsAvailable;
	UINT8   MercBio;
	BOOLEAN Drunk;
	UINT8   uiAlternateIndex;

}	CONTITION_FOR_MERC_AVAILABLE;

typedef struct
{
	UINT16	usMoneyPaid;
	UINT16	usDay;
	UINT8   ubMercArrayID;
	UINT8   uiIndex;
	UINT8   ProfilId;
	BOOLEAN NewMercsAvailable;
	BOOLEAN StartMercsAvailable;
	UINT8   MercBio;
	BOOLEAN	Drunk;
	UINT8   uiAlternateIndex;
	
}	CONTITION_FOR_MERC_AVAILABLE_TEMP;

extern CONTITION_FOR_MERC_AVAILABLE gConditionsForMercAvailability[ NUM_PROFILES ]; //NUM_MERC_ARRIVALS ];
extern CONTITION_FOR_MERC_AVAILABLE_TEMP gConditionsForMercAvailabilityTemp[ NUM_PROFILES ];

extern BOOLEAN LoadNewMercsFromLoadGameFile( HWFILE hFile );
extern BOOLEAN SaveNewMercsToSaveGameFile( HWFILE hFile );

//The players account information for the MERC site
enum
{
	MERC_NO_ACCOUNT,
	MERC_ACCOUNT_SUSPENDED,
	MERC_ACCOUNT_INVALID,
	MERC_ACCOUNT_VALID_FIRST_WARNING,
	MERC_ACCOUNT_VALID,
};
//extern	UINT8			gubPlayersMercAccountStatus;
//extern	UINT32		guiPlayersMercAccountNumber;


// The video conferencing for the merc page
#define		MERC_VIDEO_SPECK_SPEECH_NOT_TALKING														65535
#define		MERC_VIDEO_SPECK_HAS_TO_TALK_BUT_QUOTE_NOT_CHOSEN_YET					65534


//used with the gubArrivedFromMercSubSite variable to signify whcih page the player came from
enum
{
	MERC_CAME_FROM_OTHER_PAGE,
	MERC_CAME_FROM_ACCOUNTS_PAGE,
	MERC_CAME_FROM_HIRE_PAGE,
};



void GameInitMercs();
BOOLEAN EnterMercs();
void ExitMercs();
void HandleMercs();
void RenderMercs();


BOOLEAN InitMercBackGround();
BOOLEAN DrawMecBackGround();
BOOLEAN RemoveMercBackGround();
void DailyUpdateOfMercSite( UINT16 usDate);
UINT8 GetMercIDFromMERCArray(UINT8 ubMercID);

void DisplayTextForSpeckVideoPopUp(STR16 pString);

BOOLEAN IsMercMercAvailable( UINT8 ubMercID );

void HandlePlayerHiringMerc( UINT8 ubHiredMercID );
void EnterInitMercSite();
#ifdef JA2UB
//void GetMercSiteBackOnline();
#else
void GetMercSiteBackOnline();
#endif
void DisableMercSiteButton();

extern	UINT16			gusMercVideoSpeckSpeech;

extern	UINT8			gubArrivedFromMercSubSite;

extern	UINT8			gubMercArray[ NUM_PROFILES ]; //MAX_NUMBER_OF_MERCS
extern	UINT8			gubCurMercIndex;
//extern	UINT8			gubLastMercIndex;

//extern	UINT32		guiNumberOfMercPaymentsInDays;
//extern	UINT8			gubNumDaysTillFirstMercArrives;

extern	BOOLEAN		gfJustHiredAMercMerc;

// anv: moved declaration for use with playable Speck in other classes (Flo marrying Hicks)
void MakeBiffAwayForCoupleOfDays();

// anv: for playable Speck
BOOLEAN IsSpeckComAvailable();
void HandleSpeckWitnessingEmployeeDeath( SOLDIERTYPE* pSoldier );

void InitializeNumDaysMercArrive();

void NewMercsAvailableAtMercSiteCallBack( );

void CalcAproximateAmountPaidToSpeck();
#ifdef JA2UB
extern void			MarkSpeckImportantQuoteUsed( UINT32 uiQuoteNum );
extern BOOLEAN		HasImportantSpeckQuoteBeingSaid( UINT32 uiQuoteNum );
extern INT8			IsSpeckQuoteImportantQuote( UINT32 uiQuoteNum );
#endif

#endif



 


