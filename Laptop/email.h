#ifndef __EMAIL_H
#define __EMAIL_H

#include "types.h"
#include "soldier profile type.h"

// defines
#define MAX_EMAIL_LINES 10 //max number of lines can be shown in a message
#define MAX_MESSAGES_PAGE 18 // max number of messages per page

//---ja25 ub
#ifdef JA2UB

#define EMAIL_INSURANCE_L1 170
#define EMAIL_INSURANCE_L2 211
#define EMAIL_INSURANCE_L3 170
#define EMAIL_INSURANCE_L4 173
#define EMAIL_INSURANCE_L5 179
#define EMAIL_INSURANCE_L6 176

#define EMAIL_BOBBY_R_L1 198

#define EMAIL_AIM_L1 206
#define EMAIL_AIM_L2 27
#define EMAIL_AIM_L3 217
#define EMAIL_AIM_L4 214



//JA25:  New emails
// email # 1
#define EMAIL_STARTGAME												0
#define EMAIL_STARTGAME_LENGTH								8
// email # 2
#define EMAIL_PILOTMISSING										(EMAIL_STARTGAME + EMAIL_STARTGAME_LENGTH)		
#define EMAIL_PILOTMISSING_LENGTH							2

// email # 3
#define EMAIL_MAKECONTACT											(EMAIL_PILOTMISSING + EMAIL_PILOTMISSING_LENGTH)	
#define EMAIL_MAKECONTACT_LENGTH							3

// email # 4
#define EMAIL_MANUEL													(EMAIL_MAKECONTACT + EMAIL_MAKECONTACT_LENGTH)
#define EMAIL_MANUEL_LENGTH										4

// # email # 5  - only if Miguel is alive!
#define EMAIL_MIGUELHELLO											(EMAIL_MANUEL + EMAIL_MANUEL_LENGTH)
#define EMAIL_MIGUELHELLO_LENGTH							3

// email # 6 - player not advancing fast enough
#define EMAIL_CONCERNED												(EMAIL_MIGUELHELLO + EMAIL_MIGUELHELLO_LENGTH)
#define EMAIL_CONCERNED_LENGTH								2

// email # 7 - player still not advancing fast enough
#define EMAIL_URGENT													(EMAIL_CONCERNED + EMAIL_CONCERNED_LENGTH)
#define EMAIL_URGENT_LENGTH										3

// email # 8a  - from Miguel
#define EMAIL_MIGUELSORRY											(EMAIL_URGENT + EMAIL_URGENT_LENGTH)
#define EMAIL_MIGUELSORRY_LENGTH							3

// email # 8b  - from Miguel, mentioning Manuel
#define EMAIL_MIGUELMANUEL										(EMAIL_MIGUELSORRY + EMAIL_MIGUELSORRY_LENGTH)
#define EMAIL_MIGUELMANUEL_LENGTH							4

// email # 9 - Miguel sick
#define EMAIL_MIGUELSICK											(EMAIL_MIGUELMANUEL + EMAIL_MIGUELMANUEL_LENGTH)
#define EMAIL_MIGUELSICK_LENGTH								3

// email # 10a
#define EMAIL_UNDERSTANDING										(EMAIL_MIGUELSICK + EMAIL_MIGUELSICK_LENGTH)
#define EMAIL_UNDERSTANDING_LENGTH						3

// email # 10b - if we need extra cash
#define EMAIL_EXTRACASH												(EMAIL_UNDERSTANDING + EMAIL_UNDERSTANDING_LENGTH)
#define EMAIL_EXTRACASH_LENGTH								4

// email # 11
#define EMAIL_PILOTFOUND											(EMAIL_EXTRACASH + EMAIL_EXTRACASH_LENGTH)
#define EMAIL_PILOTFOUND_LENGTH								2

// email # 12a - Miguel dead, Manuel never recruited
#define EMAIL_CONGRATS												(EMAIL_PILOTFOUND + EMAIL_PILOTFOUND_LENGTH)
#define EMAIL_CONGRATS_LENGTH									4

// email # 12b - Miguel alive, Manuel never recruited
#define EMAIL_CONGRATSICK											(EMAIL_CONGRATS + EMAIL_CONGRATS_LENGTH)
#define EMAIL_CONGRATSICK_LENGTH							5

// email # 12c - Miguel alive, Manuel dead
#define EMAIL_CONGRATMIGMANUELDEAD						(EMAIL_CONGRATSICK + EMAIL_CONGRATSICK_LENGTH)
#define EMAIL_CONGRATMIGMANUELDEAD_LENGTH			6

// email # 12d - Miguel alive, Manuel recruited and alive
#define EMAIL_CONGRATMIGMANUELALIVE						(EMAIL_CONGRATMIGMANUELDEAD + EMAIL_CONGRATMIGMANUELDEAD_LENGTH)
#define EMAIL_CONGRATMIGMANUELALIVE_LENGTH		6

// email # 12e - Miguel dead, Manuel dead
#define EMAIL_CONGRATMANUELDEAD								(EMAIL_CONGRATMIGMANUELALIVE + EMAIL_CONGRATMIGMANUELALIVE_LENGTH)
#define EMAIL_CONGRATMANUELDEAD_LENGTH				5

// email # 12f -  Miguel dead, Manuel recruited and alive
#define EMAIL_CONGRATMANUELALIVE							(EMAIL_CONGRATMANUELDEAD + EMAIL_CONGRATMANUELDEAD_LENGTH)
#define EMAIL_CONGRATMANUELALIVE_LENGTH				5

// email # 13 - original AIM email
#define EMAIL_AIMDISCOUNT											(EMAIL_CONGRATMANUELALIVE + EMAIL_CONGRATMANUELALIVE_LENGTH)
#define EMAIL_AIMDISCOUNT_LENGTH							7

#define	MANUEL_AT_HOME_NOT_USED								( EMAIL_AIMDISCOUNT + EMAIL_AIMDISCOUNT_LENGTH )
#define	MANUEL_AT_HOME_NOT_USED_LENGTH				1

//Email #14 Initial IMP email
#define IMP_EMAIL_INTRO												(MANUEL_AT_HOME_NOT_USED + MANUEL_AT_HOME_NOT_USED_LENGTH )
#define IMP_EMAIL_INTRO_LENGTH								10

//Email #15 Imp follow up
#define IMP_EMAIL_AGAIN												(IMP_EMAIL_INTRO + IMP_EMAIL_INTRO_LENGTH)
#define IMP_EMAIL_AGAIN_LENGTH								5



// Email #16 - ??  merc left-me-a-message-and-now-I'm-back emails
#define	AIM_REPLY_BARRY												( IMP_EMAIL_AGAIN + IMP_EMAIL_AGAIN_LENGTH )
#define	AIM_REPLY_LENGTH_BARRY								2

#define AIM_REPLY_MELTDOWN										(AIM_REPLY_BARRY + ( 39 * AIM_REPLY_LENGTH_BARRY ))
#define AIM_REPLY_LENGTH_MELTDOWN							AIM_REPLY_LENGTH_BARRY


#define	AIM_REFUND														( AIM_REPLY_MELTDOWN + AIM_REPLY_LENGTH_MELTDOWN )
#define	AIM_REFUND_LENGTH											3

#define	MERC_REFUND														( AIM_REFUND + AIM_REFUND_LENGTH )
#define	MERC_REFUND_LENGTH										3



#define	EMAIL_AIM_PROMOTION_1										( MERC_REFUND + MERC_REFUND_LENGTH )
#define	EMAIL_AIM_PROMOTION_1_LENGTH						4

#define	EMAIL_MERC_PROMOTION_1									( EMAIL_AIM_PROMOTION_1 + EMAIL_AIM_PROMOTION_1_LENGTH )
#define	EMAIL_MERC_PROMOTION_1_LENGTH						5

#define	EMAIL_AIM_PROMOTION_2										( EMAIL_MERC_PROMOTION_1 + EMAIL_MERC_PROMOTION_1_LENGTH )
#define	EMAIL_AIM_PROMOTION_2_LENGTH						5


#define IMP_EMAIL_PROFILE_RESULTS								( EMAIL_AIM_PROMOTION_2 + EMAIL_AIM_PROMOTION_2_LENGTH )
#define IMP_EMAIL_PROFILE_RESULTS_LENGTH				1

#define IMP_EMAIL_INTRO2					0
#define IMP_EMAIL_INTRO_LENGTH2	10
#define ENRICO_CONGRATS2					(IMP_EMAIL_INTRO2 + IMP_EMAIL_INTRO_LENGTH2)					
#define ENRICO_CONGRATS_LENGTH2	3
#define IMP_EMAIL_AGAIN2					(ENRICO_CONGRATS2 + ENRICO_CONGRATS_LENGTH2)
#define IMP_EMAIL_AGAIN_LENGTH2	6
#define MERC_INTRO2				(IMP_EMAIL_AGAIN2 + IMP_EMAIL_AGAIN_LENGTH2)
#define MERC_INTRO_LENGTH2		5
#define MERC_NEW_SITE_ADDRESS2	( MERC_INTRO2 + MERC_INTRO_LENGTH2 )
#define MERC_NEW_SITE_ADDRESS_LENGTH2 2
#define AIM_MEDICAL_DEPOSIT_REFUND2 ( MERC_NEW_SITE_ADDRESS2 + MERC_NEW_SITE_ADDRESS_LENGTH2 )
#define AIM_MEDICAL_DEPOSIT_REFUND_LENGTH2 3
#define IMP_EMAIL_PROFILE_RESULTS2 ( AIM_MEDICAL_DEPOSIT_REFUND2 + AIM_MEDICAL_DEPOSIT_REFUND_LENGTH2 )
#define IMP_EMAIL_PROFILE_RESULTS_LENGTH2 1
#define	MERC_WARNING2							( IMP_EMAIL_PROFILE_RESULTS_LENGTH2 + IMP_EMAIL_PROFILE_RESULTS2 )
#define	MERC_WARNING_LENGTH2			2
#define MERC_INVALID2							( MERC_WARNING2 + MERC_WARNING_LENGTH2 )
#define MERC_INVALID_LENGTH2				2							
#define	NEW_MERCS_AT_MERC					( MERC_INVALID2 + MERC_INVALID_LENGTH2 )
#define	NEW_MERCS_AT_MERC_LENGTH	2

//Jagged Alliance 2

// insurance company emails
#define INSUR_PAYMENT					200
#define INSUR_PAYMENT_LENGTH	3
#define INSUR_SUSPIC					201
#define INSUR_SUSPIC_LENGTH		3
#define INSUR_INVEST_OVER			202
#define INSUR_INVEST_OVER_LENGTH	3
#define INSUR_SUSPIC_2				203
#define INSUR_SUSPIC_2_LENGTH		3

#define	BOBBYR_NOW_OPEN					204
#define BOBBYR_NOW_OPEN_LENGTH	3

#define KING_PIN_LETTER					205
#define KING_PIN_LETTER_LENGTH 4

#define	LACK_PLAYER_PROGRESS_1	206
#define	LACK_PLAYER_PROGRESS_1_LENGTH	3

#define	LACK_PLAYER_PROGRESS_2	207
#define	LACK_PLAYER_PROGRESS_2_LENGTH	3

#define	LACK_PLAYER_PROGRESS_3	208
#define	LACK_PLAYER_PROGRESS_3_LENGTH	3

//A package from bobby r has arrived in Drassen
#define	BOBBYR_SHIPMENT_ARRIVED	199
#define	BOBBYR_SHIPMENT_ARRIVED_LENGTH	4

// John Kulba has left the gifts for theplayers in drassen
#define	JOHN_KULBA_GIFT_IN_DRASSEN	209
#define	JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH	4

//when a merc dies on ANOTHER assignment ( ie not with the player )
#define	MERC_DIED_ON_OTHER_ASSIGNMENT	210
#define	MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH	5

#define INSUR_1HOUR_FRAUD 211
#define INSUR_1HOUR_FRAUD_LENGTH 3

//when a merc is fired, and is injured
#define AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND 212
#define AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND_LENGTH 3

//when a merc is fired, and is dead
#define AIM_MEDICAL_DEPOSIT_NO_REFUND 213
#define AIM_MEDICAL_DEPOSIT_NO_REFUND_LENGTH 3

#define	BOBBY_R_MEDUNA_SHIPMENT				214
#define	BOBBY_R_MEDUNA_SHIPMENT_LENGTH				4

#define AIM_MEDICAL_DEPOSIT_REFUND 215
#define AIM_MEDICAL_DEPOSIT_REFUND_LENGTH 3

#else
#define IMP_EMAIL_INTRO					0
#define IMP_EMAIL_INTRO_LENGTH	10
#define ENRICO_CONGRATS					(IMP_EMAIL_INTRO + IMP_EMAIL_INTRO_LENGTH)					
#define ENRICO_CONGRATS_LENGTH	3
#define IMP_EMAIL_AGAIN					(ENRICO_CONGRATS + ENRICO_CONGRATS_LENGTH)
#define IMP_EMAIL_AGAIN_LENGTH	6
#define MERC_INTRO				(IMP_EMAIL_AGAIN + IMP_EMAIL_AGAIN_LENGTH)
#define MERC_INTRO_LENGTH		5
#define MERC_NEW_SITE_ADDRESS	( MERC_INTRO + MERC_INTRO_LENGTH )
#define MERC_NEW_SITE_ADDRESS_LENGTH 2
#define AIM_MEDICAL_DEPOSIT_REFUND ( MERC_NEW_SITE_ADDRESS + MERC_NEW_SITE_ADDRESS_LENGTH )
#define AIM_MEDICAL_DEPOSIT_REFUND_LENGTH 3
#define IMP_EMAIL_PROFILE_RESULTS ( AIM_MEDICAL_DEPOSIT_REFUND + AIM_MEDICAL_DEPOSIT_REFUND_LENGTH )
#define IMP_EMAIL_PROFILE_RESULTS_LENGTH 1
#define	MERC_WARNING							( IMP_EMAIL_PROFILE_RESULTS_LENGTH + IMP_EMAIL_PROFILE_RESULTS )
#define	MERC_WARNING_LENGTH				2
#define MERC_INVALID							( MERC_WARNING + MERC_WARNING_LENGTH )
#define MERC_INVALID_LENGTH				2							
#define	NEW_MERCS_AT_MERC					( MERC_INVALID + MERC_INVALID_LENGTH )
#define	NEW_MERCS_AT_MERC_LENGTH	2
#define	MERC_FIRST_WARNING				( NEW_MERCS_AT_MERC + NEW_MERCS_AT_MERC_LENGTH )
#define	MERC_FIRST_WARNING_LENGTH	2
// merc up a level emails
#define	MERC_UP_LEVEL_BIFF				( MERC_FIRST_WARNING + MERC_FIRST_WARNING_LENGTH )
#define	MERC_UP_LEVEL_LENGTH_BIFF	2
#define	MERC_UP_LEVEL_HAYWIRE				( MERC_UP_LEVEL_LENGTH_BIFF + MERC_UP_LEVEL_BIFF )
#define	MERC_UP_LEVEL_LENGTH_HAYWIRE	2
#define	MERC_UP_LEVEL_GASKET				( MERC_UP_LEVEL_LENGTH_HAYWIRE + MERC_UP_LEVEL_HAYWIRE )
#define	MERC_UP_LEVEL_LENGTH_GASKET	2
#define	MERC_UP_LEVEL_RAZOR				( MERC_UP_LEVEL_LENGTH_GASKET + MERC_UP_LEVEL_GASKET )
#define	MERC_UP_LEVEL_LENGTH_RAZOR	2
#define	MERC_UP_LEVEL_FLO				( MERC_UP_LEVEL_LENGTH_RAZOR + MERC_UP_LEVEL_RAZOR )
#define	MERC_UP_LEVEL_LENGTH_FLO	2
#define	MERC_UP_LEVEL_GUMPY				( MERC_UP_LEVEL_LENGTH_FLO + MERC_UP_LEVEL_FLO )
#define	MERC_UP_LEVEL_LENGTH_GUMPY	2
#define	MERC_UP_LEVEL_LARRY				( MERC_UP_LEVEL_LENGTH_GUMPY + MERC_UP_LEVEL_GUMPY )
#define	MERC_UP_LEVEL_LENGTH_LARRY	2
#define	MERC_UP_LEVEL_COUGAR				( MERC_UP_LEVEL_LENGTH_LARRY + MERC_UP_LEVEL_LARRY )
#define	MERC_UP_LEVEL_LENGTH_COUGAR	2
#define	MERC_UP_LEVEL_NUMB				( MERC_UP_LEVEL_LENGTH_COUGAR + MERC_UP_LEVEL_COUGAR )
#define	MERC_UP_LEVEL_LENGTH_NUMB	2
#define	MERC_UP_LEVEL_BUBBA				( MERC_UP_LEVEL_LENGTH_NUMB + MERC_UP_LEVEL_NUMB )
#define	MERC_UP_LEVEL_LENGTH_BUBBA		2
// merc left-me-a-message-and-now-I'm-back emails
#define	AIM_REPLY_BARRY				( MERC_UP_LEVEL_LENGTH_BUBBA + MERC_UP_LEVEL_BUBBA )
#define	AIM_REPLY_LENGTH_BARRY		2
#define AIM_REPLY_MELTDOWN		(AIM_REPLY_BARRY + ( 39 * AIM_REPLY_LENGTH_BARRY ))
#define AIM_REPLY_LENGTH_MELTDOWN		AIM_REPLY_LENGTH_BARRY

// old EXISTING emails when player starts game. They must look "read"
#define OLD_ENRICO_1					( AIM_REPLY_LENGTH_MELTDOWN + AIM_REPLY_MELTDOWN )
#define OLD_ENRICO_1_LENGTH		3
#define OLD_ENRICO_2					( OLD_ENRICO_1 + OLD_ENRICO_1_LENGTH )
#define OLD_ENRICO_2_LENGTH		3
#define RIS_REPORT						( OLD_ENRICO_2 + OLD_ENRICO_2_LENGTH )
#define RIS_REPORT_LENGTH			2
#define OLD_ENRICO_3					( RIS_REPORT + RIS_REPORT_LENGTH )
#define OLD_ENRICO_3_LENGTH		3

// emails that occur from Enrico once player accomplishes things
#define ENRICO_MIGUEL					( OLD_ENRICO_3 + OLD_ENRICO_3_LENGTH )
#define ENRICO_MIGUEL_LENGTH	3
#define ENRICO_PROG_20				( ENRICO_MIGUEL + ENRICO_MIGUEL_LENGTH )
#define ENRICO_PROG_20_LENGTH	3
#define ENRICO_PROG_55				( ENRICO_PROG_20 + ENRICO_PROG_20_LENGTH )
#define ENRICO_PROG_55_LENGTH	3
#define ENRICO_PROG_80				( ENRICO_PROG_55 + ENRICO_PROG_55_LENGTH )
#define ENRICO_PROG_80_LENGTH	3
#define ENRICO_SETBACK				( ENRICO_PROG_80 + ENRICO_PROG_80_LENGTH )
#define ENRICO_SETBACK_LENGTH	3
#define ENRICO_SETBACK_2			( ENRICO_SETBACK + ENRICO_SETBACK_LENGTH )
#define ENRICO_SETBACK_2_LENGTH	3
#define ENRICO_CREATURES			( ENRICO_SETBACK_2 + ENRICO_SETBACK_2_LENGTH )
#define ENRICO_CREATURES_LENGTH	3

// insurance company emails
#define INSUR_PAYMENT					( ENRICO_CREATURES + ENRICO_CREATURES_LENGTH )
#define INSUR_PAYMENT_LENGTH	3
#define INSUR_SUSPIC					( INSUR_PAYMENT + INSUR_PAYMENT_LENGTH )
#define INSUR_SUSPIC_LENGTH		3
#define INSUR_INVEST_OVER			( INSUR_SUSPIC + INSUR_SUSPIC_LENGTH )
#define INSUR_INVEST_OVER_LENGTH	3
#define INSUR_SUSPIC_2				( INSUR_INVEST_OVER + INSUR_INVEST_OVER_LENGTH )
#define INSUR_SUSPIC_2_LENGTH		3

#define	BOBBYR_NOW_OPEN					( INSUR_SUSPIC_2 + INSUR_SUSPIC_2_LENGTH )
#define BOBBYR_NOW_OPEN_LENGTH	3

#define KING_PIN_LETTER					( BOBBYR_NOW_OPEN + BOBBYR_NOW_OPEN_LENGTH )
#define KING_PIN_LETTER_LENGTH 4

#define	LACK_PLAYER_PROGRESS_1	( KING_PIN_LETTER + KING_PIN_LETTER_LENGTH )
#define	LACK_PLAYER_PROGRESS_1_LENGTH	3

#define	LACK_PLAYER_PROGRESS_2	( LACK_PLAYER_PROGRESS_1 + LACK_PLAYER_PROGRESS_1_LENGTH )
#define	LACK_PLAYER_PROGRESS_2_LENGTH	3

#define	LACK_PLAYER_PROGRESS_3	( LACK_PLAYER_PROGRESS_2 + LACK_PLAYER_PROGRESS_2_LENGTH )
#define	LACK_PLAYER_PROGRESS_3_LENGTH	3

//A package from bobby r has arrived in Drassen
#define	BOBBYR_SHIPMENT_ARRIVED	( LACK_PLAYER_PROGRESS_3 + LACK_PLAYER_PROGRESS_3_LENGTH )
#define	BOBBYR_SHIPMENT_ARRIVED_LENGTH	4

// John Kulba has left the gifts for theplayers in drassen
#define	JOHN_KULBA_GIFT_IN_DRASSEN	( BOBBYR_SHIPMENT_ARRIVED + BOBBYR_SHIPMENT_ARRIVED_LENGTH )
#define	JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH	4

//when a merc dies on ANOTHER assignment ( ie not with the player )
#define	MERC_DIED_ON_OTHER_ASSIGNMENT	( JOHN_KULBA_GIFT_IN_DRASSEN + JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH )
#define	MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH	5

#define INSUR_1HOUR_FRAUD ( MERC_DIED_ON_OTHER_ASSIGNMENT + MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH )
#define INSUR_1HOUR_FRAUD_LENGTH 3

//when a merc is fired, and is injured
#define AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND ( INSUR_1HOUR_FRAUD + INSUR_1HOUR_FRAUD_LENGTH )
#define AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND_LENGTH 3

//when a merc is fired, and is dead
#define AIM_MEDICAL_DEPOSIT_NO_REFUND ( AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND + AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND_LENGTH )
#define AIM_MEDICAL_DEPOSIT_NO_REFUND_LENGTH 3

#define	BOBBY_R_MEDUNA_SHIPMENT				( AIM_MEDICAL_DEPOSIT_NO_REFUND + AIM_MEDICAL_DEPOSIT_NO_REFUND_LENGTH )
#define	BOBBY_R_MEDUNA_SHIPMENT_LENGTH				4
#endif
// WANNE: New 1.13 MERC merc mail text for level up that Speck sends
//#define	MERC_UP_LEVEL_GASTON			(BOBBY_R_MEDUNA_SHIPMENT + BOBBY_R_MEDUNA_SHIPMENT_LENGTH)
//#define MERC_UP_LEVEL_GASTON_LENGTH		2
//#define MERC_UP_LEVEL_STOGIE			(MERC_UP_LEVEL_GASTON + MERC_UP_LEVEL_GASTON_LENGTH)
//#define MERC_UP_LEVEL_STOGIE_LENGTH		2
//#define MERC_UP_LEVEL_TEX				(MERC_UP_LEVEL_STOGIE + MERC_UP_LEVEL_STOGIE_LENGTH)
//#define MERC_UP_LEVEL_TEX_LENGTH		2
//#define MERC_UP_LEVEL_BIGGENS			(MERC_UP_LEVEL_TEX + MERC_UP_LEVEL_TEX_LENGTH)
//#define MERC_UP_LEVEL_BIGGENS_LENGTH	2


#define	MERC_UP_LEVEL_GASTON			165
#define MERC_UP_LEVEL_STOGIE			166
#define MERC_UP_LEVEL_TEX				167
#define MERC_UP_LEVEL_BIGGENS			168

struct message{
	STR16 pString;
	struct message *Next;
	struct message *Prev;
};

typedef struct message EmailMessage;
typedef EmailMessage *MessagePtr;


struct email
{
	STR16		pSubject;
	UINT16	usOffset;
	UINT16	usLength;
	UINT8		ubSender;
	UINT32	iDate;
	INT32		iId;
	INT32		iFirstData;
	UINT32	uiSecondData;
	BOOLEAN fRead;
	BOOLEAN fNew;

	INT32		iThirdData;
	INT32		iFourthData;
	UINT32	uiFifthData;
	UINT32	uiSixData;

	// WANNE: A reference to the IMP position in the gMercProfiles array. 
	// So we know which analyse email belongs to the imp
	INT32		iCurrentIMPPosition;
	
	UINT8 EmailVersion;
	
	UINT32 EmailType;

	// WANNE.MAIL: Fix
	INT16		iCurrentShipmentDestinationID;

	struct	email *Next;
	struct	email *Prev;
};

typedef struct email Email;
typedef Email *EmailPtr;


// This used when saving the emails to disk.
typedef struct 
{
	UINT16	usOffset;
	UINT16	usLength;
	UINT8		ubSender;
	UINT32	iDate;
	INT32		iId;
	INT32		iFirstData;
	UINT32	uiSecondData;

	INT32		iThirdData;
	INT32		iFourthData;
	UINT32	uiFifthData;
	UINT32	uiSixData;

	BOOLEAN fRead;
	BOOLEAN fNew;

	INT32		iCurrentIMPPosition;

	// WANNE.MAIL: Fix
	INT16		iCurrentShipmentDestinationID;

} SavedEmailStruct;



struct pagemessages{
	INT32 iIds[MAX_MESSAGES_PAGE];
	INT32 iPageId;
	struct pagemessages *Next;
	struct pagemessages *Prev;
};


typedef struct pagemessages Page;
typedef Page *PagePtr;

struct messagerecord{
//	CHAR16 pRecord[ 320 ];
	CHAR16 pRecord[ 640 ];
	struct messagerecord *Next;
};

typedef struct messagerecord Record;
typedef Record *RecordPtr;

typedef struct
{
	RecordPtr pFirstRecord;
	RecordPtr pLastRecord;
	INT32 iPageNumber;
} EmailPageInfoStruct;

enum {
	SENDER=0,
	RECEIVED,
	SUBJECT,
	READ,
};

enum {
	MAIL_ENRICO=0,
	CHAR_PROFILE_SITE,
	GAME_HELP,
	IMP_PROFILE_RESULTS,
	SPECK_FROM_MERC,
	RIS_EMAIL,
	BARRY_MAIL,
	MELTDOWN_MAIL = BARRY_MAIL+ 39,
	INSURANCE_COMPANY,
	BOBBY_R,
	KING_PIN,
	JOHN_KULBA,
	AIM_SITE,
#ifdef JA2UB
	MAIL_MIGUEL,
#endif
};

// the length of the subject in char
#define EMAIL_SUBJECT_LENGTH 128

extern BOOLEAN fUnReadMailFlag;
extern BOOLEAN fNewMailFlag;
extern BOOLEAN	fOldUnreadFlag;
extern BOOLEAN	fOldNewMailFlag;
extern BOOLEAN fDeleteMailFlag;
extern BOOLEAN fDisplayMessageFlag;
extern BOOLEAN fReDrawNewMailFlag; 
extern BOOLEAN fOpenMostRecentUnReadFlag;
extern EmailPtr pEmailList; 
extern UINT32 guiEmailWarning;


void GameInitEmail();
BOOLEAN EnterEmail();
void ExitEmail();
void HandleEmail();
void RenderEmail();

#define	CHECK_X							iScreenWidthOffset + 15
#define CHECK_Y							iScreenHeightOffset + 13
#define VIEWER_X						iScreenWidthOffset + 155
#define VIEWER_Y						iScreenHeightOffset + 70 + 21
#define MAIL_STRING_SIZE 640


// message manipulation
void AddEmailMessage(INT32 iMessageOffset, INT32 iMessageLength,STR16 pSubject, INT32 iDate, UINT8 ubSender, BOOLEAN fAlreadyRead, INT32 uiFirstData, UINT32 uiSecondData, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType, UINT32 EmailAIM );
void RemoveEmailMessage(INT32 iId);
EmailPtr GetEmailMessage(INT32 iId);
void LookForUnread();
void AddEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType);

// WANNE: For the new WF merc, when they available again
void AddEmailWFMercAvailable(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, UINT8 EmailType);

void AddPreReadEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, UINT8 EmailType );
BOOLEAN DisplayNewMailBox();
void CreateDestroyNewMailButton();
void CreateDestroyDeleteNoticeMailButton();
void AddDeleteRegionsToMessageRegion(INT32 iViewerY);
void DisplayEmailHeaders( void );
void ReDrawNewMailBox( void );
void ReDisplayBoxes( void );
void ShutDownEmailList();
void AddMessageToPages(INT32 iMessageId);
void AddEmailWithSpecialData(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iFirstData, UINT32 uiSecondData, UINT8 EmailType, UINT32 EmailAIM );

void AddCustomEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType);

#ifdef JA2BETAVERSION
	void AddAllEmails();
#endif

typedef struct
{
	UINT8 uiIndex;
	CHAR16	szMessage[MAIL_STRING_SIZE];
	CHAR16	szSubject[EMAIL_SUBJECT_LENGTH];

} EMAIL_MERC_AVAILABLE_VALUES;

typedef struct
{
	UINT8 uiIndex;
	CHAR16	szMessage[MAIL_STRING_SIZE];
	CHAR16	szSubject[EMAIL_SUBJECT_LENGTH];

} EMAIL_MERC_LEVEL_UP_VALUES;

typedef struct
{
	UINT8 uiIndex;
	CHAR16	szMessage[MAIL_STRING_SIZE];
	CHAR16	szSubject[EMAIL_SUBJECT_LENGTH];

} EMAIL_MERC_INSURANCE_VALUES;

#define EMAIL_INDEX 500

typedef struct
{
	UINT8 uiIndex;
	CHAR16	szMessage[MAIL_STRING_SIZE][30];
	CHAR16	szSubject[EMAIL_SUBJECT_LENGTH];
} EMAIL_OTHER_VALUES;


typedef struct
{
	UINT8 uiIndex;
	UINT32	EmailType;
	UINT8 EmailVersion;

} EMAIL_TYPE;

#define EMAIL_VAL 4000

extern EMAIL_TYPE gEmailT[EMAIL_VAL];

enum {
	//------------------------------
	//Read Email.edt
	// read  message from Email.edt 
	TYPE_EMAIL_EMAIL_EDT = 0,
	// read message from Email.edt and nickname (sender) read from MercProfiles.xml
	TYPE_EMAIL_EMAIL_EDT_NAME_MERC,
	
	//-------------------------------
	//Read *.xml
	//Read message from Email\EmailMercAvailable.xml.XML and nickname (sender)  read from MercProfiles.xml
	TYPE_EMAIL_AIM_AVAILABLE,
	//Read message from Email\EmailMercLevelUp.xml.XML and nickname (sender)  read from MercProfiles.xml
	TYPE_EMAIL_MERC_LEVEL_UP,
	
	//Read from others *.XMLs
	TYPE_EMAIL_BOBBY_R,
	
	//Read from JA2 Email.edt
	TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT,
	TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT,
	TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT,
	
	//Read from others *.XMLs
	TYPE_EMAIL_ENRICO,
	TYPE_EMAIL_CHAR_PROFILE_SITE,
	TYPE_EMAIL_GAME_HELP,
	TYPE_EMAIL_IMP_PROFILE_RESULTS,
	TYPE_EMAIL_SPECK_FROM_MERC,
	TYPE_EMAIL_RIS_EMAIL,
	TYPE_EMAIL_INSURANCE_COMPANY,
	TYPE_EMAIL_KING_PIN,
	TYPE_EMAIL_JOHN_KULBA,
	TYPE_EMAIL_AIM_SITE, 
	
	TYPE_EMAIL_OTHER,
};

enum {

	TYPE_E_NONE   = 0,
	
	TYPE_E_AIM_L1 = 1, //206
	TYPE_E_AIM_L2 = 2, //27
	TYPE_E_AIM_L3 = 3, //217
	TYPE_E_AIM_L4 = 4, //214
	
	TYPE_E_INSURANCE_L1 = 5, //170
	TYPE_E_INSURANCE_L2 = 6, //211
	TYPE_E_INSURANCE_L3 = 7, //170
	TYPE_E_INSURANCE_L4 = 8, //173
	TYPE_E_INSURANCE_L5 = 9, //179
	TYPE_E_INSURANCE_L6 = 10, //176

	TYPE_EMAIL_BOBBY_R_L1 = 11,
};

extern EMAIL_MERC_AVAILABLE_VALUES EmailMercAvailableText[NUM_PROFILES];
extern EMAIL_MERC_LEVEL_UP_VALUES EmailMercLevelUpText[NUM_PROFILES];
extern EMAIL_MERC_INSURANCE_VALUES EmailInsuranceText[NUM_PROFILES];

extern EMAIL_OTHER_VALUES EmailOtherText[EMAIL_INDEX];
extern BOOLEAN ReadXMLEmail;
extern void AddEmailTypeXML( INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, UINT8 EmailType );
extern void AddPreReadEmailTypeXML( INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, UINT8 EmailType );
extern void AddEmailWithSpecialDataXML(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iFirstData, UINT32 uiSecondData, UINT8 EmailType );
extern BOOLEAN SaveNewEmailDataToSaveGameFile( HWFILE hFile );
extern BOOLEAN LoadNewEmailDataFromLoadGameFile( HWFILE hFile );

#ifdef JA2UB
extern void AddBobbyREmailJA2(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType );
#endif

#endif



 


