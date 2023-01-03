#ifndef __MESSAGE_H
#define __MESSAGE_H

//#include "sgp.h"
#include "font.h"
#include "Font Control.h"
#include "types.h"
#include "Fileman.h"


struct stringstruct{

	STR16	pString16;
	INT32	iVideoOverlay;
	UINT32	uiFont;
	UINT16	usColor;
	UINT32	uiFlags;
	BOOLEAN fBeginningOfNewString;
	UINT32	uiTimeOfLastUpdate;
	UINT32	uiPadding[ 5 ];
	struct	stringstruct *pNext;
	struct	stringstruct *pPrev;
};

#define		MSG_INTERFACE					0
#define		MSG_DIALOG						1
#define		MSG_MPSYSTEM					2
#define		MSG_DEBUG						3
#define		MSG_UI_FEEDBACK					4
#define		MSG_ERROR						5
#define		MSG_BETAVERSION					6
#define		MSG_TESTVERSION					7
#define		MSG_MAP_UI_POSITION_MIDDLE		8
#define		MSG_MAP_UI_POSITION_UPPER		9
#define		MSG_MAP_UI_POSITION_LOWER		10
#define		MSG_SKULL_UI_FEEDBACK			11
#define		MSG_CHAT						12


// These defines correlate to defines in font.h
#define		MSG_FONT_RED					FONT_MCOLOR_RED
#define		MSG_FONT_YELLOW				FONT_MCOLOR_LTYELLOW
#define		MSG_FONT_WHITE				FONT_MCOLOR_WHITE


typedef struct stringstruct ScrollStringSt;
typedef ScrollStringSt *ScrollStringStPtr;		


extern ScrollStringStPtr pStringS;
extern UINT32 StringCount;
extern UINT8 gubCurrentMapMessageString;
extern BOOLEAN fDisableJustForIan;

// are we allowed to beep on message scroll in tactical
extern BOOLEAN fOkToBeepNewMessage;

void ScreenMsg( UINT16 usColor, UINT8 ubPriority, STR16 pStringA, ...);

// same as screen message, but only display to mapscreen message system, not tactical
void MapScreenMessage( UINT16 usColor, UINT8 ubPriority, STR16 pStringA, ...);

void ScrollString( void );
void DisplayStringsInMapScreenMessageList( void );

void InitGlobalMessageList( void );
void FreeGlobalMessageList( void );

UINT8 GetRangeOfMapScreenMessages( void );

void EnableDisableScrollStringVideoOverlay( BOOLEAN fEnable );

// OJW - 20090315 - Allow access to these functions outside of message
// adding chat log
ScrollStringStPtr GetNextString(ScrollStringStPtr pStringSt);
ScrollStringStPtr GetPrevString(ScrollStringStPtr pStringSt);
void SetStringPosition(ScrollStringStPtr pStringSt, UINT16 x, UINT16 y);
void SetStringColor(ScrollStringStPtr pStringSt, UINT16 color);
ScrollStringStPtr SetStringNext(ScrollStringStPtr pStringSt, ScrollStringStPtr pNext);
ScrollStringStPtr SetStringPrev(ScrollStringStPtr pStringSt, ScrollStringStPtr pPrev);
void SetString(ScrollStringStPtr pStringSt, STR16 String);
// will go and clear all displayed strings off the screen
void ClearDisplayedListOfTacticalStrings( void );
// OJW - 20090426
void SetTacticalMessageFilter( UINT ubPriority );
void RemoveTacticalMessageFilter ( void );

// clear ALL strings in the tactical Message Queue
void ClearTacticalMessageQueue( void );

BOOLEAN LoadMapScreenMessagesFromSaveGameFile( HWFILE hFile );
BOOLEAN SaveMapScreenMessagesToSaveGameFile( HWFILE hFile );

// use these if you are not Kris
void HideMessagesDuringNPCDialogue( void );
void UnHideMessagesDuringNPCDialogue( void );

// disable and enable scroll string, only to be used by Kris
void DisableScrollMessages( void );
void EnableScrollMessages( void );


/* unused functions, written by Mr. Carter, so don't expect these to work...
UINT8 GetTheRelativePositionOfCurrentMessage( void );
void MoveCurrentMessagePointerDownList( void );
void MoveCurrentMessagePointerUpList( void );
void ScrollToHereInMapScreenMessageList( UINT8 ubPosition );
BOOLEAN IsThereAnEmptySlotInTheMapScreenMessageList( void );
UINT8 GetFirstEmptySlotInTheMapScreenMessageList( void );
void RemoveMapScreenMessageListString( ScrollStringStPtr pStringSt );
BOOLEAN AreThereASetOfStringsAfterThisIndex( UINT8 ubMsgIndex, INT32 iNumberOfStrings );
UINT8 GetCurrentMessageValue( void );
UINT8 GetCurrentTempMessageValue( void );
UINT8 GetNewMessageValueGivenPosition( UINT8 ubPosition );
BOOLEAN IsThisTheLastMessageInTheList( void );
BOOLEAN IsThisTheFirstMessageInTheList( void );
void DisplayLastMessage( void );
*/


#endif