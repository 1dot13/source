﻿#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "email.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "Debug.h"
	#include "WordWrap.h"
	#include "Encrypted File.h"
	#include "cursors.h"
	#include "soldier profile.h"
	#include "CharProfile.h"
	#include "IMP Compile Character.h"
	#include "IMP Portraits.h"
	#include "Game Clock.h"
	#include "AimMembers.h"
	#include "Random.h"
	#include "Text.h"
	#include "LaptopSave.h"
	#include "finances.h"
	#include "PostalService.h"
	#include "faces.h"
	#include "GameSettings.h"
	#include <string>
#endif
#include "soldier profile type.h"
#include "strategicmap.h"

#ifdef JA2UB
#include "Ja25_Tactical.h"
#include "Ja25 Strategic Ai.h"
#include "ub_config.h"

#define		EMAIL_EDT_FILE_JA25			"BINARYDATA\\Email25.edt"
#define		EMAIL_EDT_FILE_JA2		"BINARYDATA\\Email.edt"
#endif

#define EMAIL_EDT_CUSTOM_FILE "BINARYDATA\\Email_Custom.edt"
void AddCustomEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition);

using namespace std;

//static EmailPtr pEmailList;
EmailPtr pEmailList;
static PagePtr	pPageList;
static INT32 iLastPage=-1;
static INT32 iCurrentPage=0;
INT32 iDeleteId=0;
BOOLEAN fUnReadMailFlag=FALSE;
BOOLEAN fOldUnreadFlag=TRUE;
BOOLEAN fNewMailFlag=FALSE;
BOOLEAN fOldNewMailFlag=FALSE;
BOOLEAN fDisplayMessageFlag=FALSE;
BOOLEAN fOldDisplayMessageFlag=FALSE;
BOOLEAN fReDraw=FALSE;
BOOLEAN fDeleteMailFlag=FALSE;
BOOLEAN fReDrawMessageFlag = FALSE;
BOOLEAN fOnLastPageFlag=FALSE;
BOOLEAN fJustStartedEmail = FALSE;
BOOLEAN fDeleteInternal = FALSE;
BOOLEAN fOpenMostRecentUnReadFlag = FALSE;
INT32 iViewerPositionY=0;

INT32 giMessageId = -1;
INT32 giPrevMessageId = -1;
INT32 giMessagePage = -1;
INT32 giNumberOfPagesToCurrentEmail = -1;
UINT32 guiEmailWarning;

#define EMAIL_TOP_BAR_HEIGHT 22

#define LIST_MIDDLE_COUNT 18
// object positions
#define TITLE_X 0+LAPTOP_SCREEN_UL_X
#define TITLE_Y 0+LAPTOP_SCREEN_UL_Y

#define STAMP_X								LAPTOP_SCREEN_UL_X
#define STAMP_Y								LAPTOP_SCREEN_UL_Y
/*
#define TOP_X 0+LAPTOP_SCREEN_UL_X
#define TOP_Y 62+LAPTOP_SCREEN_UL_Y

#define BOTTOM_X 0+LAPTOP_SCREEN_UL_X
#define BOTTOM_Y 359+LAPTOP_SCREEN_UL_Y
*/
#define MIDDLE_X 0+LAPTOP_SCREEN_UL_X
#define MIDDLE_Y							iScreenHeightOffset + 72 + EMAIL_TOP_BAR_HEIGHT
#define MIDDLE_WIDTH 19


// new graphics
#define EMAIL_LIST_WINDOW_Y					iScreenHeightOffset + 22
#define EMAIL_TITLE_BAR_X					iScreenWidthOffset + 5

// email columns
#define SENDER_X LAPTOP_SCREEN_UL_X+65
#define SENDER_WIDTH 246-158

#define DATE_X LAPTOP_SCREEN_UL_X+428
#define DATE_WIDTH 592-527

#define SUBJECT_X LAPTOP_SCREEN_UL_X+175
#define SUBJECT_WIDTH						254
#define INDIC_X								iScreenWidthOffset + 128
#define INDIC_WIDTH 155-123
#define INDIC_HEIGHT 145-128

#define LINE_WIDTH 592-121

#define MESSAGE_WIDTH						528 - 125
#define MESSAGE_COLOR FONT_BLACK
#define MESSAGE_GAP 2

#define MESSAGE_HEADER_WIDTH 209-151
#define MESSAGE_HEADER_X VIEWER_X+4

#define VIEWER_HEAD_X						iScreenWidthOffset + 140
#define VIEWER_HEAD_Y						iScreenHeightOffset + 9
#define VIEWER_HEAD_WIDTH 445-VIEWER_HEAD_X
#define MAX_BUTTON_COUNT 1
#define VIEWER_WIDTH 500
#define VIEWER_HEIGHT 195

#define MESSAGEX_X							iScreenWidthOffset + 425
#define MESSAGEX_Y							iScreenHeightOffset + 6

#define EMAIL_WARNING_X						iScreenWidthOffset + 210
#define EMAIL_WARNING_Y						iScreenHeightOffset + 140
#define EMAIL_WARNING_WIDTH 254
#define EMAIL_WARNING_HEIGHT 138

#define NEW_BTN_X EMAIL_WARNING_X +(338-245)
#define NEW_BTN_Y EMAIL_WARNING_Y +(278-195)

#define EMAIL_TEXT_FONT				FONT10ARIAL
#define TRAVERSE_EMAIL_FONT		FONT14ARIAL
#define EMAIL_BOX_FONT				FONT14ARIAL
#define MESSAGE_FONT					EMAIL_TEXT_FONT
#define EMAIL_HEADER_FONT			FONT14ARIAL
#define EMAIL_WARNING_FONT		FONT12ARIAL


// the max number of pages to an email
#define MAX_NUMBER_EMAIL_PAGES 100

#define PREVIOUS_PAGE 0
#define NEXT_PAGE	 1

#define NEXT_PAGE_X LAPTOP_UL_X + 562
#define NEXT_PAGE_Y							iScreenHeightOffset + 51

#define PREVIOUS_PAGE_X NEXT_PAGE_X - 21
#define PREVIOUS_PAGE_Y NEXT_PAGE_Y

#define ENVELOPE_BOX_X						iScreenWidthOffset + 116

#define FROM_BOX_X							iScreenWidthOffset + 166
#define FROM_BOX_WIDTH 246-160

#define SUBJECT_BOX_X						iScreenWidthOffset + 276
#define SUBJECT_BOX_WIDTH 528-249

#define DATE_BOX_X							iScreenWidthOffset + 530
#define DATE_BOX_WIDTH 594-530

#define FROM_BOX_Y							iScreenHeightOffset + 51 + EMAIL_TOP_BAR_HEIGHT
#define TOP_HEIGHT 118-95

#define EMAIL_TITLE_FONT FONT14ARIAL
#define EMAIL_TITLE_X						iScreenWidthOffset + 140
#define EMAIL_TITLE_Y						iScreenHeightOffset + 33
#define VIEWER_MESSAGE_BODY_START_Y VIEWER_Y+72
#define MIN_MESSAGE_HEIGHT_IN_LINES 5


#define INDENT_Y_OFFSET 310
#define INDENT_X_OFFSET 325
#define INDENT_X_WIDTH						544 - 481

// the position of the page number being displayed in the email program
#define PAGE_NUMBER_X						iScreenWidthOffset + 516
#define PAGE_NUMBER_Y						iScreenHeightOffset + 58

// defines for location of message 'title'/'headers'

#define MESSAGE_FROM_Y VIEWER_Y+28

#define MESSAGE_DATE_Y MESSAGE_FROM_Y

#define MESSAGE_SUBJECT_Y MESSAGE_DATE_Y+16


#define SUBJECT_LINE_X VIEWER_X+47
#define SUBJECT_LINE_Y VIEWER_Y+42
#define SUBJECT_LINE_WIDTH 278-47


// maximum size of a email message page, so not to overrun the bottom of the screen
#define MAX_EMAIL_MESSAGE_PAGE_SIZE ( GetFontHeight( MESSAGE_FONT ) + MESSAGE_GAP ) * 20
enum{
	PREVIOUS_BUTTON=0,
	NEXT_BUTTON,
};


// X button position
#define BUTTON_X							VIEWER_X + 396
#define BUTTON_Y							VIEWER_Y + 3
#define BUTTON_LOWER_Y						BUTTON_Y + 22
#define PREVIOUS_PAGE_BUTTON_X				VIEWER_X + 302
#define NEXT_PAGE_BUTTON_X					VIEWER_X +395
#define DELETE_BUTTON_X						NEXT_PAGE_BUTTON_X
#define LOWER_BUTTON_Y						BUTTON_Y + 299


BOOLEAN fSortDateUpwards = FALSE;
BOOLEAN fSortSenderUpwards = FALSE;
BOOLEAN fSortSubjectUpwards = FALSE;
BOOLEAN gfPageButtonsWereCreated = FALSE;

// mouse regions
MOUSE_REGION pEmailRegions[MAX_MESSAGES_PAGE];
extern	MOUSE_REGION pScreenMask; // symbol already defined in laptop.cpp (jonathanl)
MOUSE_REGION pDeleteScreenMask;
MOUSE_REGION pMailViewMessageRegion;

// the email info struct to speed up email
EmailPageInfoStruct pEmailPageInfo[ MAX_NUMBER_EMAIL_PAGES ];

//buttons
INT32 giMessageButton[MAX_BUTTON_COUNT];
INT32 giMessageButtonImage[MAX_BUTTON_COUNT];
INT32 giDeleteMailButton[2];
INT32 giDeleteMailButtonImage[2];
INT32 giSortButton[4];
INT32 giSortButtonImage[4];
INT32 giNewMailButton[1];
INT32 giNewMailButtonImage[1];
INT32 giMailMessageButtons[3];
INT32 giMailMessageButtonsImage[3];
INT32 giMailPageButtons[ 2 ];
INT32 giMailPageButtonsImage[ 2 ];


// mouse regions
MOUSE_REGION pEmailMoveRegions[NEXT_BUTTON+1];
MOUSE_REGION pSortMailRegions[3];

// the message record list, for the currently displayed message
RecordPtr pMessageRecordList=NULL;

// video handles
UINT32 guiEmailTitle;
UINT32 guiEmailStamp;
UINT32 guiEmailBackground;
UINT32 guiEmailIndicator;
UINT32 guiEmailMessage;
UINT32 guiMAILDIVIDER;

INT16 giCurrentIMPSlot = PLAYER_GENERATED_CHARACTER_ID;
EMAIL_MERC_AVAILABLE_VALUES EmailMercAvailableText[NUM_PROFILES];
EMAIL_MERC_LEVEL_UP_VALUES EmailMercLevelUpText[NUM_PROFILES];
EMAIL_OTHER_VALUES EmailOtherText[EMAIL_INDEX];
EMAIL_MERC_INSURANCE_VALUES EmailInsuranceText[NUM_PROFILES];
BOOLEAN ReadXMLEmail = TRUE; // TRUE - read email from XML, FALSE - read email from EDT
EMAIL_TYPE gEmailT[EMAIL_VAL];
BOOLEAN SaveNewEmailDataToSaveGameFile( HWFILE hFile );
BOOLEAN LoadNewEmailDataFromLoadGameFile( HWFILE hFile );

// the enumeration of headers
enum{
	FROM_HEADER=0,
	SUBJECT_HEADER,
	RECD_HEADER,
};


// position of header text on the email list
#define FROM_X									iScreenWidthOffset + 205
#define FROM_Y FROM_BOX_Y + 5
#define SUBJECTHEAD_X							iScreenWidthOffset + 368
#define RECD_X									iScreenWidthOffset + 550

// size of prev/next strings
#define PREVIOUS_WIDTH StringPixLength(pTraverseStrings[PREVIOUS_BUTTON], TRAVERSE_EMAIL_FONT)
#define NEXT_WIDTH StringPixLength(pTraverseStrings[NEXT_BUTTON], TRAVERSE_EMAIL_FONT)
#define PREVIOUS_HEIGHT GetFontHeight(TRAVERSE_EMAIL_FONT)
#define NEXT_HEIGHT GetFontHeight(TRAVERSE_EMAIL_FONT)


// current line in the email list that is highlighted, -1 is no line highlighted
INT32 iHighLightLine=-1;

// whther or not we need to redraw the new mail box
BOOLEAN fReDrawNewMailFlag = FALSE;
INT32 giNumberOfMessageToEmail = 0;
INT32 iTotalHeight = 0;

// function list
void SwapMessages(INT32 iIdA, INT32 iIdB);
void PlaceMessagesinPages();
BOOLEAN	fFirstTime=TRUE;
void EmailBtnCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void EmailMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void PreviousRegionButtonCallback(GUI_BUTTON *btn,INT32 reason);
void NextRegionButtonCallback(GUI_BUTTON *btn,INT32 reason);
void SetUnNewMessages();
INT32 DisplayEmailMessage(EmailPtr pMail);
void AddDeleteRegionsToMessageRegion(INT32 iViewerY);
void DetermineNextPrevPageDisplay();
void CreateDestroyNextPreviousRegions();
void ReDraw();
void DeleteEmail();
BOOLEAN DisplayDeleteNotice(EmailPtr pMail);
void CreateDestroyDeleteNoticeMailButton();
void SetUpSortRegions();
void DeleteSortRegions();
void DisplayTextOnTitleBar( void );
void DisplayEmailMessageSubjectDateFromLines( EmailPtr pMail, INT32 iViewerY );
void DrawEmailMessageDisplayTitleText( INT32 iViewerY );
void CreateMailScreenButtons( void );
void DestroyMailScreenButtons( void );
void DrawLineDividers( void );
void FromCallback(GUI_BUTTON *btn, INT32 iReason );
void SubjectCallback(GUI_BUTTON *btn,	INT32 iReason );
void DateCallback(GUI_BUTTON *btn,	INT32 iReason );
void ReadCallback(GUI_BUTTON *btn,	INT32 iReason );
void BtnPreviousEmailPageCallback(GUI_BUTTON *btn,INT32 reason);
void BtnNextEmailPageCallback(GUI_BUTTON *btn,INT32 reason);
void ViewMessageRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason );
void DisplayEmailList();
void ClearOutEmailMessageRecordsList( void );
void AddEmailRecordToList( STR16 pString );
void UpDateMessageRecordList( void );
void HandleAnySpecialEmailMessageEvents(INT32 iMessageId );
BOOLEAN HandleMailSpecialMessages( UINT16 usMessageId, INT32 *iResults,	EmailPtr pMail );
#ifdef JA2UB
BOOLEAN HandleNewUBMailSpecialMessages( UINT16 usMessageId, INT32 *iResults, EmailPtr pMail );
void AddBobbyREmailJA2(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType );
#endif
void HandleIMPCharProfileResultsMessage(	void );
void HandleEmailViewerButtonStates( void );
void SetUpIconForButton( void );
void DeleteCurrentMessage( void );
void BtnDeleteCallback(GUI_BUTTON *btn, INT32 iReason );
void CreateNextPreviousEmailPageButtons( void );
void UpdateStatusOfNextPreviousButtons( void );
void DisplayWhichPageOfEmailProgramIsDisplayed( void );
void OpenMostRecentUnreadEmail( void );
BOOLEAN DisplayNumberOfPagesToThisEmail( INT32 iViewerY );
INT32 GetNumberOfPagesToEmail( );
void PreProcessEmail( EmailPtr pMail );
void ModifyInsuranceEmails( UINT16 usMessageId, INT32 *iResults, EmailPtr pMail, UINT8 ubNumberOfRecords );
BOOLEAN ReplaceMercNameAndAmountWithProperData( CHAR16 *pFinishedString, EmailPtr pMail );
BOOLEAN ReplaceBiffNameWithProperMercName( CHAR16 *pFinishedString, EmailPtr pMail, CHAR16 *pMercName );
extern INT16 gusCurShipmentDestinationID;
extern CPostalService gPostalService;


void InitializeMouseRegions()
{
	INT32 iCounter=0;

	// init mouseregions
	for(iCounter=0; iCounter <MAX_MESSAGES_PAGE; iCounter++)
	{
	MSYS_DefineRegion(&pEmailRegions[iCounter],MIDDLE_X ,((INT16)(MIDDLE_Y+iCounter*MIDDLE_WIDTH)), MIDDLE_X+LINE_WIDTH ,(INT16)(MIDDLE_Y+iCounter*MIDDLE_WIDTH+MIDDLE_WIDTH),
			MSYS_PRIORITY_NORMAL+2,MSYS_NO_CURSOR, EmailMvtCallBack, EmailBtnCallBack );
	MSYS_AddRegion(&pEmailRegions[iCounter]);
		MSYS_SetRegionUserData(&pEmailRegions[iCounter],0,iCounter);
	}

	//SetUpSortRegions();

	CreateDestroyNextPreviousRegions();
}

void DeleteEmailMouseRegions()
{

	// this function will remove the mouse regions added
	INT32 iCounter=0;


	for(iCounter=0; iCounter <MAX_MESSAGES_PAGE; iCounter++)
	{
	MSYS_RemoveRegion( &pEmailRegions[iCounter]);
	}
	//DeleteSortRegions();
	CreateDestroyNextPreviousRegions();

}
void GameInitEmail()
{
	pEmailList=NULL;
	pPageList=NULL;

	iLastPage=-1;

	iCurrentPage=0;
	iDeleteId=0;

	// reset display message flag
	fDisplayMessageFlag=FALSE;

	// reset page being displayed
	giMessagePage = 0;
}

BOOLEAN EnterEmail()
{
	VOBJECT_DESC	VObjectDesc;
	// load graphics

	iCurrentPage = LaptopSaveInfo.iCurrentEmailPage;

	// title bar
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\programtitlebar.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiEmailTitle));

	// the list background
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\Mailwindow.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiEmailBackground));

	// the indication/notification box
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\MailIndicator.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiEmailIndicator));

	// the message background
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\emailviewer.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiEmailMessage));

	// the message background
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\maillistdivider.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMAILDIVIDER));

	//AddEmail(IMP_EMAIL_PROFILE_RESULTS, IMP_EMAIL_PROFILE_RESULTS_LENGTH, IMP_PROFILE_RESULTS, GetWorldTotalMin( ), -1, -1 );
	// initialize mouse regions
	InitializeMouseRegions();

	// just started email
	fJustStartedEmail = TRUE;

	// create buttons
	CreateMailScreenButtons( );

	// marks these buttons dirty
	MarkButtonsDirty( );

	// no longer fitrst time in email
	fFirstTime = FALSE;

	// reset current page of the message being displayed
	giMessagePage = 0;

	// render email background and text
	RenderEmail();
	
	//AddEmail( MERC_REPLY_GRIZZLY, MERC_REPLY_LENGTH_GRIZZLY, GRIZZLY_MAIL, GetWorldTotalMin(), -1, -1 );
	//RenderButtons( );
	
	return( TRUE );
}

void ExitEmail()
{
	LaptopSaveInfo.iCurrentEmailPage = iCurrentPage;

	// clear out message record list
	ClearOutEmailMessageRecordsList( );

	// displayed message?...get rid of it
	if(fDisplayMessageFlag)
	{
		fDisplayMessageFlag = FALSE;
		AddDeleteRegionsToMessageRegion( 0 );
		fDisplayMessageFlag = TRUE;
		fReDrawMessageFlag = TRUE;
	}
	else
	{
		giMessageId = -1;
	}

	// delete mail notice?...get rid of it
	if(fDeleteMailFlag)
	{
		fDeleteMailFlag=FALSE;
		CreateDestroyDeleteNoticeMailButton();
	}

	// remove all mouse regions in use in email
	DeleteEmailMouseRegions();

	// reset flags of new messages
	SetUnNewMessages();

	// remove video objects being used by email screen
	DeleteVideoObjectFromIndex(guiEmailTitle);
	DeleteVideoObjectFromIndex(guiEmailBackground);
	DeleteVideoObjectFromIndex(guiMAILDIVIDER);
	DeleteVideoObjectFromIndex(guiEmailIndicator);
	DeleteVideoObjectFromIndex(guiEmailMessage);
	
	// remove buttons
	DestroyMailScreenButtons( );
}

void HandleEmail( void )
{
	INT32 iViewerY = 0;
	static BOOLEAN fEmailListBeenDrawAlready = FALSE;
	//RenderButtonsFastHelp( );
	
	// check if email message record list needs to be updated
	UpDateMessageRecordList( );

	// does email list need to be draw, or can be drawn
	if( ( (!fDisplayMessageFlag)&&(!fNewMailFlag) && ( !fDeleteMailFlag ) )&&( fEmailListBeenDrawAlready == FALSE ) )
	{
		DisplayEmailList();
		fEmailListBeenDrawAlready = TRUE;
	}
	// if the message flag, show message
	else if((fDisplayMessageFlag)&&(fReDrawMessageFlag))
	{
		// redisplay list
		DisplayEmailList();

		// this simply redraws message without button manipulation
		iViewerY = DisplayEmailMessage(GetEmailMessage(giMessageId));
		fEmailListBeenDrawAlready = FALSE;
	}
	else if((fDisplayMessageFlag)&&(!fOldDisplayMessageFlag))
	{
		// redisplay list
		DisplayEmailList();

		// this simply redraws message with button manipulation
		iViewerY = DisplayEmailMessage(GetEmailMessage(giMessageId));
		AddDeleteRegionsToMessageRegion( iViewerY );
		fEmailListBeenDrawAlready = FALSE;

	}

	// not displaying anymore?
	if( ( fDisplayMessageFlag == FALSE ) && ( fOldDisplayMessageFlag ) )
	{
		// then clear it out
		ClearOutEmailMessageRecordsList( );
	}
	
	// if new message is being displayed...check to see if it's buttons need to be created or destroyed
	AddDeleteRegionsToMessageRegion( 0 );

	// same with delete notice
	CreateDestroyDeleteNoticeMailButton();

	// if delete notice needs to be displayed?...display it
	if(fDeleteMailFlag)
		DisplayDeleteNotice(GetEmailMessage(iDeleteId));
	
	// update buttons
	HandleEmailViewerButtonStates( );

	// set up icons for buttons
	SetUpIconForButton( );

	// redraw screen
	//ReDraw();

	//redraw headers to sort buttons
	DisplayEmailHeaders( );
	
	// handle buttons states
	UpdateStatusOfNextPreviousButtons( );

	if( fOpenMostRecentUnReadFlag == TRUE )
	{
		// enter email due to email icon on program panel
		OpenMostRecentUnreadEmail( );
		fOpenMostRecentUnReadFlag = FALSE;

	}
}

void DisplayEmailHeaders( void )
{
	// draw the text at the top of the screen

	// font stuff
	SetFont(EMAIL_WARNING_FONT);
	SetFontShadow(NO_SHADOW);
	SetFontForeground(FONT_BLACK);
	SetFontBackground(FONT_BLACK);
	
	// draw headers to the email list the player sees

	// sender text
	//mprintf(FROM_X, FROM_Y, pEmailHeaders[FROM_HEADER]);

	// subject text
	//mprintf(SUBJECTHEAD_X, FROM_Y, pEmailHeaders[SUBJECT_HEADER]);

	// date re'vd
	//mprintf(RECD_X, FROM_Y, pEmailHeaders[RECD_HEADER]);

	// reset shadow
	SetFontShadow(DEFAULT_SHADOW);
}

void RenderEmail( void )
{
	HVOBJECT hHandle;

	// get and blt the email list background
	GetVideoObject( &hHandle, guiEmailBackground );

	BltVideoObject( FRAME_BUFFER, hHandle, 0,LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_UL_Y + 22, VO_BLT_SRCTRANSPARENCY,NULL);
	
	// get and blt the email title bar
	GetVideoObject( &hHandle, guiEmailTitle );
	BltVideoObject( FRAME_BUFFER, hHandle, 0,LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_UL_Y - 2, VO_BLT_SRCTRANSPARENCY,NULL );

	// show text on titlebar
	DisplayTextOnTitleBar( );

	// redraw list if no graphics are being displayed on top of it
	//if((!fDisplayMessageFlag)&&(!fNewMailFlag))
	//{
	DisplayEmailList( );
	//}

	// redraw line dividers
	DrawLineDividers( );
	
	// show next/prev page buttons depending if there are next/prev page
	//DetermineNextPrevPageDisplay( );

	// draw headers for buttons
	DisplayEmailHeaders();

	// display border
	GetVideoObject(&hHandle, guiLaptopBACKGROUND);

	BltVideoObject(FRAME_BUFFER, hHandle, 0, iScreenWidthOffset + 108, iScreenHeightOffset + 23, VO_BLT_SRCTRANSPARENCY,NULL);
	
	ReDisplayBoxes( );

	BlitTitleBarIcons(	);
	
	// show which page we are on
	DisplayWhichPageOfEmailProgramIsDisplayed( );
	
	InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	// invalidate region to force update
}

//-----------Custom email
void AddCustomEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType )
{
	CHAR16 pSubject[320];
	//MessagePtr pMessageList;
	//MessagePtr pMessage;
	//CHAR16 pMessageString[320];
	
	LoadEncryptedDataFromFile(EMAIL_EDT_CUSTOM_FILE, pSubject, 640*(iMessageOffset), 640);
	// add message to list
	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, 0, 0, iCurrentIMPPosition, iCurrentShipmentDestinationID, EmailType, TYPE_E_NONE );

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop )
	{
		// redraw icons, might be new mail
		DrawLapTopIcons();
	}
}

//--

void AddEmailWithSpecialData(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iFirstData, UINT32 uiSecondData, UINT8 EmailType, UINT32 EmailAIM )
{
	CHAR16 pSubject[320];
	//MessagePtr pMessageList;
	//MessagePtr pMessage;
	//CHAR16 pMessageString[320];
	Email	FakeEmail;


	// starts at iSubjectOffset amd goes iSubjectLength, reading in string
#ifdef JA2UB	
	if ( EmailType == TYPE_EMAIL_EMAIL_EDT )
	{
		if (FileExists(EMAIL_EDT_FILE_JA25))
		{
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA25, pSubject, 640*(iMessageOffset), 640);
		}
		else
		{
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
		}
	}
	else if ( EmailType == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
	{
		LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
	}
#else
	LoadEncryptedDataFromFile("BINARYDATA\\Email.edt", pSubject, 640*(iMessageOffset), 640);
#endif	

	//Make a fake email that will contain the codes ( ie the merc ID )
	FakeEmail.iFirstData = iFirstData;
	FakeEmail.uiSecondData = uiSecondData;

	//Replace the $mercname$ with the actual mercname
	ReplaceMercNameAndAmountWithProperData( pSubject, &FakeEmail );

	// add message to list
	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, iFirstData, uiSecondData, -1 , -1, EmailType, EmailAIM );

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop == TRUE )
	{
	// redraw icons, might be new mail
	DrawLapTopIcons();
	}

	return;
}

//--- XML Read Mail ---

void AddEmailWithSpecialDataXML(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iFirstData, UINT32 uiSecondData, UINT8 EmailType, UINT32 EmailAIM )
{
	CHAR16 pSubject[320];
	//MessagePtr pMessageList;
	//MessagePtr pMessage;
	//CHAR16 pMessageString[320];
	Email	FakeEmail;
	UINT8 subjectLine = 0;
	
	if ( EmailType == TYPE_EMAIL_INSURANCE_COMPANY )
	{
		wcscpy( pSubject, EmailInsuranceText[iMessageLength].szSubject );
	}
	
	// starts at iSubjectOffset amd goes iSubjectLength, reading in string
	//LoadEncryptedDataFromFile("BINARYDATA\\Email.edt", pSubject, 640*(iMessageOffset), 640);

	//Make a fake email that will contain the codes ( ie the merc ID )
	FakeEmail.iFirstData = iFirstData;
	FakeEmail.uiSecondData = uiSecondData;

	//Replace the $mercname$ with the actual mercname
	ReplaceMercNameAndAmountWithProperData( pSubject, &FakeEmail );

	// add message to list
	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, iFirstData, uiSecondData, -1 , -1, EmailType, EmailAIM);

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop == TRUE )
	{
	// redraw icons, might be new mail
	DrawLapTopIcons();
	}

	return;
}

void AddPreReadEmailTypeXML( INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, UINT8 EmailType )
{
	CHAR16 pSubject[320];
	wcscpy( pSubject, L"None" );
	
	UINT8 subjectLine = 0;
	if ( EmailType == TYPE_EMAIL_AIM_AVAILABLE )
	{
		if (EmailMercAvailableText[subjectLine].szSubject !='\0')
			wcscpy( pSubject, EmailMercAvailableText[ubSender].szSubject );
	}
	else if ( EmailType == TYPE_EMAIL_MERC_LEVEL_UP )
	{
		if (EmailMercLevelUpText[subjectLine].szSubject !='\0')
			wcscpy( pSubject, EmailMercLevelUpText[ubSender].szSubject );
	}

	// add message to list
	AddEmailMessage( iMessageOffset,iMessageLength, pSubject, iDate, ubSender, TRUE, 0, 0, -1, -1 , EmailType, TYPE_E_NONE );

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop )
	{
		// redraw icons, might be new mail
		DrawLapTopIcons();
	}
}

void AddEmailTypeXML( INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, UINT8 EmailType )
{
	CHAR16 pSubject[320];
	wcscpy( pSubject, L"None" );

	if ( EmailType == TYPE_EMAIL_AIM_AVAILABLE )
	{
		if (EmailMercAvailableText[ubSender].szSubject !='\0')
			wcscpy( pSubject, EmailMercAvailableText[ubSender].szSubject );
	}
	else if ( EmailType == TYPE_EMAIL_MERC_LEVEL_UP )
	{
		if (EmailMercLevelUpText[ubSender].szSubject !='\0')
			wcscpy( pSubject, EmailMercLevelUpText[ubSender].szSubject );
	}
	/*
	else if ( EmailType == TYPE_EMAIL_BOBBY_R )
	{
		if (EmailBobbyRText[0] !='\0')
			wcscpy( pSubject, EmailBobbyRText[0] );
	}
	*/
	else if ( EmailType == TYPE_EMAIL_OTHER )
	{
		if (EmailOtherText[ubSender].szSubject !='\0')
			wcscpy( pSubject, EmailOtherText[ubSender].szSubject );
	}

	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, 0, 0, iCurrentIMPPosition, -1, EmailType, TYPE_E_NONE);

	// if we are in fact int he laptop, redraw icons, might be change in mail status
	if( fCurrentlyInLaptop )
	{
		// redraw icons, might be new mail
		DrawLapTopIcons();
	}
}

#ifdef JA2UB
void AddBobbyREmailJA2(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType )
{
	CHAR16 pSubject[320];
	UINT8 subjectLine = 0;
	//MessagePtr pMessageList;
	//MessagePtr pMessage;
	//CHAR16 pMessageString[320];
	
	LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
	
	// add message to list
	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, 0, 0, iCurrentIMPPosition, iCurrentShipmentDestinationID, EmailType, TYPE_EMAIL_BOBBY_R_L1);

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop == TRUE )
	{
		// redraw icons, might be new mail
		DrawLapTopIcons();
	}

	return;
}
#endif

//--- End XML Read Mail ---

void AddEmailWFMercAvailable(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, UINT8 EmailType)
{
	CHAR16 pSubject[320];

	UINT8 subjectLine = 0;

	// WF mercs
	if (iMessageLength < 178) // -> see: New113AIMMercMailTexts	
		subjectLine = (iMessageLength - 170) * 2;
	// Generic mercs
	else
		subjectLine = 16;	// -> see: New113AIMMercMailTexts

		wcscpy( pSubject, New113AIMMercMailTexts[subjectLine] );
	
	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, 0, 0, iCurrentIMPPosition, -1 , EmailType, TYPE_E_NONE);

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop == TRUE )
	{
		// redraw icons, might be new mail
		DrawLapTopIcons();
	}

	return;
}

void AddEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType)
{
	CHAR16 pSubject[320];
	UINT8 subjectLine = 0;
	//MessagePtr pMessageList;
	//MessagePtr pMessage;
	//CHAR16 pMessageString[320];
#ifdef JA2UB	

	if ( EmailType == TYPE_EMAIL_EMAIL_EDT ) 
	{
		if (FileExists(EMAIL_EDT_FILE_JA25))
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA25, pSubject, 640*(iMessageOffset), 640);
		else
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
	}
	else if ( EmailType == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
	{
		LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
	}
	else if ( EmailType == TYPE_EMAIL_BOBBY_R )
	{
	/*	if (EmailBobbyRText[0] !='\0')
			wcscpy( pSubject, EmailBobbyRText[0] );
		else
			wcscpy( pSubject, L"None" );
	*/
	}
	
#else
	// WANNE: Short work in both ways
	LoadEncryptedDataFromFile("BINARYDATA\\Email.edt", pSubject, 640*(iMessageOffset), 640);
#endif
	// add message to list
	AddEmailMessage(iMessageOffset,iMessageLength, pSubject, iDate, ubSender, FALSE, 0, 0, iCurrentIMPPosition, iCurrentShipmentDestinationID, EmailType, TYPE_E_NONE);

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop == TRUE )
	{
		// redraw icons, might be new mail
		DrawLapTopIcons();
	}

	return;
}

void AddPreReadEmail(INT32 iMessageOffset, INT32 iMessageLength, UINT8 ubSender, INT32 iDate , UINT8 EmailType )
{
	CHAR16 pSubject[320];
	//MessagePtr pMessageList;
	//MessagePtr pMessage;
	//CHAR16 pMessageString[320];

#ifdef JA2UB	
	if ( EmailType == TYPE_EMAIL_EMAIL_EDT ) 
	{
		if (FileExists(EMAIL_EDT_FILE_JA25) )
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA25, pSubject, 640*(iMessageOffset), 640);
		else
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
	}
	else if ( EmailType == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
	{
		LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, pSubject, 640*(iMessageOffset), 640);
	}
#else	
	// starts at iSubjectOffset amd goes iSubjectLength, reading in string
	LoadEncryptedDataFromFile("BINARYDATA\\Email.edt", pSubject, 640*(iMessageOffset), 640);
#endif
	// add message to list
	AddEmailMessage( iMessageOffset,iMessageLength, pSubject, iDate, ubSender, TRUE, 0, 0, -1, -1 , EmailType, TYPE_E_NONE );

	// if we are in fact int he laptop, redraw icons, might be change in mail status

	if( fCurrentlyInLaptop == TRUE )
	{
	// redraw icons, might be new mail
	DrawLapTopIcons();
	}

	return;
}

void AddEmailMessage(INT32 iMessageOffset, INT32 iMessageLength,STR16 pSubject, INT32 iDate, UINT8 ubSender, BOOLEAN fAlreadyRead, INT32 iFirstData, UINT32 uiSecondData, INT32 iCurrentIMPPosition, INT16 iCurrentShipmentDestinationID, UINT8 EmailType, UINT32 EmailAIM )
{
	// will add a message to the list of messages
	EmailPtr pEmail=pEmailList;
	EmailPtr pTempEmail=NULL;
	INT32 iId=0;

	// run through list of messages, get id of oldest message
	if(pEmail)
	{
	while(pEmail)
		{
		if(pEmail->iId >iId)
			iId=pEmail->iId;
		pEmail=pEmail->Next;
		}
	}

	// reset pEmail
	pEmail=pEmailList;

	// move to end of list
	if( pEmail )
	{
	while( pEmail->Next)
		pEmail = pEmail->Next;
	}

	// add new element onto list
	pTempEmail = (EmailPtr) MemAlloc(sizeof(Email));
	memset( pTempEmail, 0, sizeof(Email) );
	// add in strings
	//while(pMessage !=NULL)
	//{
	//pTempEmail->pText[iCounter]=MemAlloc((wcslen(pMessage->pString)+2)*2);
	//wcscpy(pTempEmail->pText[iCounter],pMessage->pString);
	//pMessage=pMessage->Next;
	//iCounter++;
	//}
	//pTempEmail->pText[iCounter]=NULL;

	/*
	// WANNE: Not needed anymore, bug is fixed!!
	// WANNE: Fix the memory heap problem -> CTD (fix by BirdFlu)
	// The text for the wildfire merc is too long (> 128 characters)!!
	// copy subject
    int _len = wcslen(pSubject);
    pTempEmail->pSubject = (STR16) MemAlloc( (_len + 1) * sizeof(CHAR16) );
    memset( pTempEmail->pSubject, 0, sizeof( CHAR16 ) * (_len + 1) );
    wcsncpy(pTempEmail->pSubject,pSubject,_len);
    pTempEmail->pSubject[_len] = 0;
	*/
	
	// copy subject
	pTempEmail->pSubject = (STR16) MemAlloc( 128 * sizeof(CHAR16) );
	memset( pTempEmail->pSubject, 0, sizeof( CHAR16 ) * 128 );
	wcscpy(pTempEmail->pSubject,pSubject);
	
	pTempEmail->EmailVersion = EmailType;
	
	pTempEmail->EmailType = EmailAIM;

	// copy offset and length of the actual message in email.edt
	pTempEmail->usOffset =(UINT16)iMessageOffset;
	pTempEmail->usLength =(UINT16)iMessageLength;

	// null out last byte of subject
	pTempEmail->pSubject[wcslen(pSubject)+1]=0;

	pTempEmail->iCurrentIMPPosition = iCurrentIMPPosition;

	// WANNE.MAIL: Fix
	pTempEmail->iCurrentShipmentDestinationID = iCurrentShipmentDestinationID;


	// set date and sender, Id
	if(pEmail)
	pTempEmail->iId=iId+1;
	else
		pTempEmail->iId=0;
	gEmailT[ (UINT32)pTempEmail->iId ].EmailVersion = EmailType;		
	gEmailT[ (UINT32)pTempEmail->iId ].EmailType = EmailAIM;

	// copy date and sender id's
	pTempEmail->iDate=iDate;
	pTempEmail->ubSender=ubSender;

	// the special data
	pTempEmail->iFirstData = iFirstData;
	pTempEmail->uiSecondData = uiSecondData;

	// place into list
	if(pEmail)
	{
		// list exists, place at end
	pEmail->Next=pTempEmail;
	pTempEmail->Prev=pEmail;
	}
	else
	{
		// no list, becomes head of a new list
		pEmail=pTempEmail;
		pTempEmail->Prev=NULL;
		pEmailList=pEmail;
	}

	// reset Next ptr
	pTempEmail->Next=NULL;

	// set flag that new mail has arrived
	fNewMailFlag=TRUE;

	// add this message to the pages of email
	AddMessageToPages(pTempEmail->iId);

	// reset read flag of this particular message
	pTempEmail->fRead=fAlreadyRead;

	// set fact this message is new
	pTempEmail->fNew=TRUE;
	return;
}


void RemoveEmailMessage(INT32 iId)
{
	// run through list and remove message, update everyone afterwards
	EmailPtr pEmail=pEmailList;
	EmailPtr pTempEmail=NULL;


	// error check
	if(!pEmail)
		return;

	// look for message
	pEmail = GetEmailMessage( iId );
	
	gEmailT[ (UINT32)iId ].EmailType = 0;
	
	//while((pEmail->iId !=iId)&&(pEmail->Next))
	//	pEmail=pEmail->Next;

	// end of list, no mail found, leave
	if(!pEmail)
		return;
	// found

	// set tempt object current
	pTempEmail=pEmail;

	// check position of message in list
	if((pEmail->Prev)&&(pTempEmail->Next))
	{
		// in the middle of the list
	pEmail=pEmail->Prev;
	pTempEmail=pTempEmail->Next;
	MemFree(pEmail->Next->pSubject);
	//while(pEmail->Next->pText[iCounter])
	//{
	//MemFree(pEmail->Next->pText[iCounter]);
	//iCounter++;
	//}
	MemFree(pEmail->Next);
	pEmail->Next=pTempEmail;
	pTempEmail->Prev=pEmail;
	}
	else if(pEmail->Prev)
	{
		// end of the list
		pEmail=pEmail->Prev;
	MemFree(pEmail->Next->pSubject);
		//while(pEmail->Next->pText[iCounter])
		//{
	 //MemFree(pEmail->Next->pText[iCounter]);
		//iCounter++;
		//}
	MemFree(pEmail->Next);
		pEmail->Next=NULL;
	}
	else if(pTempEmail->Next)
	{
		// beginning of the list
		pEmail=pTempEmail;
		pTempEmail=pTempEmail->Next;
	MemFree(pEmail->pSubject);
	//while(pEmail->pText[iCounter])
		//{
	//MemFree(pEmail->pText[iCounter]);
		//iCounter++;
		//}
	MemFree(pEmail);
		pTempEmail->Prev=NULL;
		pEmailList=pTempEmail;
	}
	else
	{
		// all alone
	MemFree(pEmail->pSubject);
	//	while(pEmail->pText[iCounter])
		//{
	 //MemFree(pEmail->pText[iCounter]);
		//iCounter++;
		//}
	MemFree(pEmail);
	pEmailList=NULL;
	}
}

EmailPtr GetEmailMessage(INT32 iId)
{
	EmailPtr pEmail=pEmailList;
	// return pointer to message with iId

	// invalid id
	if(iId==-1)
		return NULL;

	// invalid list
	if( pEmail == NULL )
	{
		return NULL;
	}

	// look for message
	while( (pEmail->iId !=iId)&&(pEmail->Next) )
		pEmail=pEmail->Next;

	if( ( pEmail->iId != iId ) && ( pEmail->Next == NULL ) )
	{
		pEmail = NULL;
	}

	// no message, or is there?
	if(!pEmail)
		return NULL;
	else
		return pEmail;
}


void AddEmailPage()
{
	// simple adds a page to the list
	PagePtr pPage=pPageList;
	if(pPage)
	{
	while(pPage->Next)
		pPage=pPage->Next;
	}


	if(pPage)
	{

		// there is a page, add current page after it
		pPage->Next = (pagemessages *) MemAlloc(sizeof(Page));
		pPage->Next->Prev=pPage;
	pPage=pPage->Next;
		pPage->Next=NULL;
		pPage->iPageId=pPage->Prev->iPageId+1;
	memset(pPage->iIds, -1, sizeof(INT32) * MAX_MESSAGES_PAGE );
	}
	else
	{

		// page becomes head of page list
		pPageList = (PagePtr) MemAlloc(sizeof(Page));
		pPage=pPageList;
		pPage->Prev=NULL;
		pPage->Next=NULL;
		pPage->iPageId=0;
	memset(pPage->iIds, -1, sizeof(INT32) * MAX_MESSAGES_PAGE );
	pPageList=pPage;
	}
	iLastPage++;
	return;

}


void RemoveEmailPage(INT32 iPageId)
{
	PagePtr pPage=pPageList;
	PagePtr pTempPage=NULL;

	// run through list until page is matched, or out of pages
	while((pPage->iPageId !=iPageId)&&(pPage))
		pPage=pPage->Next;

	// error check
	if(!pPage)
		return;


	// found
	pTempPage=pPage;
	if((pPage->Prev)&&(pTempPage->Next))
	{
		// in the middle of the list
	pPage=pPage->Prev;
	pTempPage=pTempPage->Next;
	MemFree(pPage->Next);
	pPage->Next=pTempPage;
	pTempPage->Prev=pPage;
	}
	else if(pPage->Prev)
	{
		// end of the list
		pPage=pPage->Prev;
	MemFree(pPage->Next);
		pPage->Next=NULL;
	}
	else if(pTempPage->Next)
	{
		// beginning of the list
		pPage=pTempPage;
		pTempPage=pTempPage->Next;
	MemFree(pPage);
		pTempPage->Prev=NULL;
	}
	else
	{
		// all alone

	MemFree(pPage);
	pPageList=NULL;
	}
	if(iLastPage !=0)
	iLastPage--;
}

void AddMessageToPages(INT32 iMessageId)
{
	// go to end of page list
	PagePtr pPage=pPageList;
	INT32 iCounter=0;
	if(!pPage)
	AddEmailPage();
	pPage=pPageList;
	while((pPage->Next)&&(pPage->iIds[MAX_MESSAGES_PAGE-1]!=-1))
		pPage=pPage->Next;
	// if list is full, add new page
	while(iCounter <MAX_MESSAGES_PAGE)
	{
		if(pPage->iIds[iCounter]==-1)
			break;
		iCounter++;
	}
	if(iCounter==MAX_MESSAGES_PAGE)
	{
		AddEmailPage();
		AddMessageToPages(iMessageId);
	return;
	}
	else
	{
		pPage->iIds[iCounter]=iMessageId;
	}
	return;
}

void SortMessages(INT32 iCriteria)
{
	EmailPtr pA=pEmailList;
	EmailPtr pB=pEmailList;
	CHAR16 pSubjectA[256];
	CHAR16 pSubjectB[256];

	// no messages to sort?
	if( ( pA == NULL) ||( pB == NULL ) )
	{
		return;
	}

	// nothing here either?
	if(!pA->Next)
		return;

	pB=pA->Next;
	switch(iCriteria)
	{
		case RECEIVED:
		while(pA)
			{

				// set B to next in A
				pB=pA->Next;
				while(pB)
				{

					if( fSortDateUpwards )
					{
							// if date is lesser, swap
					if(pA->iDate > pB->iDate)
						SwapMessages(pA->iId, pB->iId);
					}
					else
					{
						// if date is lesser, swap
					if(pA->iDate < pB->iDate)
						SwapMessages(pA->iId, pB->iId);
					}


					// next in B's list
					pB=pB->Next;
				}

				// next in A's List
		pA=pA->Next;
			}
			break;
		case SENDER:
			while(pA)
			{

				pB = pA->Next;
				while(pB)
				{
			// lesser string?...need sorting
					if( fSortSenderUpwards )
					{
					if(( wcscmp( pSenderNameList[pA->ubSender] , pSenderNameList[pB->ubSender] ) ) < 0 )
						SwapMessages(pA->iId, pB->iId);
					}
					else
					{
						if(( wcscmp( pSenderNameList[pA->ubSender] , pSenderNameList[pB->ubSender] ) ) > 0 )
						SwapMessages(pA->iId, pB->iId);
					}
					// next in B's list
					pB=pB->Next;
				}
			// next in A's List
				pA=pA->Next;
			}
			break;
	case SUBJECT:
			while(pA)
			{

				pB = pA->Next;
				while(pB)
				{
					// clear out control codes
					CleanOutControlCodesFromString( pA->pSubject,	pSubjectA );
			CleanOutControlCodesFromString( pB->pSubject,	pSubjectB );

					// lesser string?...need sorting
					if( fSortSubjectUpwards )
					{
					if( ( wcscmp( pA->pSubject ,pB->pSubject ) ) < 0)
						SwapMessages(pA->iId, pB->iId);
			}
					else
					{
						if( ( wcscmp( pA->pSubject ,pB->pSubject ) ) > 0)
						SwapMessages(pA->iId, pB->iId);
					}
					// next in B's list
					pB=pB->Next;
				}
				// next in A's List
		pA=pA->Next;
			}
			break;

	case READ:
			while(pA)
			{

				pB = pA->Next;
				while(pB)
				{
					// one read and another not?...need sorting
					if( ( pA->fRead ) && ( ! ( pB->fRead ) ) )
						SwapMessages(pA->iId, pB->iId);

					// next in B's list
					pB=pB->Next;
				}
				// next in A's List
		pA=pA->Next;
			}
			break;
	}


	// place new list into pages of email
	//PlaceMessagesinPages();

	// redraw the screen
	fReDrawScreenFlag=TRUE;
}

void SwapMessages(INT32 iIdA, INT32 iIdB)
{
 // swaps locations of messages in the linked list
 EmailPtr pA=pEmailList;
 EmailPtr pB=pEmailList;
 EmailPtr pTemp= (EmailPtr) MemAlloc(sizeof(Email) );
 pTemp->pSubject = (STR16) MemAlloc( 128 * sizeof(CHAR16) );

 memset( pTemp->pSubject, 0, sizeof( CHAR16 ) * 128 );

 if(!pA->Next)
	return;
 //find pA
 while(pA->iId!=iIdA)
	pA=pA->Next;
 // find pB
 while(pB->iId!=iIdB)
	pB=pB->Next;

 // swap

 // pTemp becomes pA
 pTemp->iId=pA->iId;
 pTemp->fRead=pA->fRead;
 pTemp->fNew=pA->fNew;
 pTemp->usOffset=pA->usOffset; 
 pTemp->EmailVersion=pA->EmailVersion;
 pTemp->usLength=pA->usLength;
 pTemp->iDate=pA->iDate;
 pTemp->ubSender=pA->ubSender;
 
 if ( pA->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE )
  wcscpy(pTemp->pSubject,EmailMercAvailableText[pA->ubSender].szSubject);
 else if ( pA->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP )
  wcscpy(pTemp->pSubject,EmailMercLevelUpText[pA->ubSender].szSubject);
 else if ( pA->EmailVersion == TYPE_EMAIL_OTHER )
	wcscpy(pTemp->pSubject,EmailOtherText[pA->usLength].szSubject);
 else if ( pA->EmailVersion == TYPE_EMAIL_BOBBY_R || pA->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT )				
 wcscpy(pTemp->pSubject,pA->pSubject);
 else if ( pA->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pA->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pA->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
 wcscpy(pTemp->pSubject,pA->pSubject);

 // pA becomes pB
 pA->iId=pB->iId;
 pA->fRead=pB->fRead;
 pA->fNew=pB->fNew;
 pA->usOffset=pB->usOffset;
 pA->EmailVersion=pB->EmailVersion;
 pA->usLength=pB->usLength;
 pA->iDate=pB->iDate;
 pA->ubSender=pB->ubSender;
 wcscpy(pA->pSubject, pB->pSubject);

// pB becomes pTemp
 pB->iId=pTemp->iId;
 pB->fRead=pTemp->fRead;
 pB->fNew=pTemp->fNew;
 pB->usOffset=pTemp->usOffset;
 pB->EmailVersion=pTemp->EmailVersion;
 pB->usLength=pTemp->usLength;
 pB->iDate=pTemp->iDate;
 pB->ubSender=pTemp->ubSender;
 
 if ( pB->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE )
  wcscpy(pB->pSubject,EmailMercAvailableText[pTemp->ubSender].szSubject);
 else if ( pB->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP )
  wcscpy(pB->pSubject,EmailMercLevelUpText[pTemp->ubSender].szSubject);
 else if ( pB->EmailVersion == TYPE_EMAIL_OTHER )
	wcscpy(pB->pSubject,EmailOtherText[pTemp->usLength].szSubject);
 else if ( pB->EmailVersion == TYPE_EMAIL_BOBBY_R || pB->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT )
 wcscpy(pB->pSubject, pTemp->pSubject);
 else if ( pB->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pB->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pB->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
 wcscpy(pB->pSubject, pTemp->pSubject);

 // free up memory
 MemFree(pTemp->pSubject);
 MemFree( pTemp );
}

void ClearPages()
{
	// run through list of message pages and set to -1
	PagePtr pPage=pPageList;

	// error check
	if( pPageList == NULL )
	{
		return;
	}

	while(pPage->Next)
	{
		pPage=pPage->Next;
		MemFree(pPage->Prev);
	}
	if(pPage)
		MemFree(pPage);

	pPageList=NULL;
	iLastPage=-1;
}

void PlaceMessagesinPages()
{
	EmailPtr pEmail=pEmailList;
	// run through the list of messages and add to pages
	ClearPages();
	while(pEmail)
	{
		AddMessageToPages(pEmail->iId);
		pEmail=pEmail->Next;

	}
	if(iCurrentPage >iLastPage)
		iCurrentPage=iLastPage;
	return;
}

void DisplayMessageList(INT32 iPageNum)
{
	// will display page with idNumber iPageNum
	PagePtr pPage=pPageList;
	while(pPage->iPageId!=iPageNum)
	{
		pPage=pPage->Next;
		if(!pPage)
			return;
	}
	// found page show it
}

void DrawLetterIcon(INT32 iCounter, BOOLEAN fRead)
{
	HVOBJECT hHandle;
	// will draw the icon for letter in mail list depending if the mail has been read or not

	// grab video object
	GetVideoObject(&hHandle, guiEmailIndicator);

	// is it read or not?
	if(fRead)
		BltVideoObject(FRAME_BUFFER, hHandle, 0,INDIC_X, (MIDDLE_Y+iCounter*MIDDLE_WIDTH+2), VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hHandle, 1,INDIC_X, (MIDDLE_Y+iCounter*MIDDLE_WIDTH+2), VO_BLT_SRCTRANSPARENCY,NULL);
}

void DrawSubject(INT32 iCounter, STR16 pSubject, BOOLEAN fRead)
{
	CHAR16 pTempSubject[320];
	
	// draw subject line of mail being viewed in viewer

	// lock buffer to prevent overwrite
	SetFontDestBuffer(FRAME_BUFFER, SUBJECT_X , ((UINT16)(MIDDLE_Y+iCounter*MIDDLE_WIDTH)) , SUBJECT_X	+ SUBJECT_WIDTH , ( ( UINT16 ) ( MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) + MIDDLE_WIDTH,	FALSE	);
	SetFontShadow(NO_SHADOW);
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );
	
	wcscpy( pTempSubject, pSubject );

	if( fRead )
	{
		//if the subject will be too long, cap it, and add the '...'
		if( StringPixLength( pTempSubject, MESSAGE_FONT ) >= SUBJECT_WIDTH - 10 )
		{
			ReduceStringLength( pTempSubject, SUBJECT_WIDTH - 10, MESSAGE_FONT );
		}

	// display string subject
	IanDisplayWrappedString(SUBJECT_X, (( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) , SUBJECT_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR ,pTempSubject,0 ,FALSE ,LEFT_JUSTIFIED );
	}
	else
	{
		//if the subject will be too long, cap it, and add the '...'
		if( StringPixLength( pTempSubject, FONT10ARIALBOLD ) >= SUBJECT_WIDTH - 10 )
		{
			ReduceStringLength( pTempSubject, SUBJECT_WIDTH - 10, FONT10ARIALBOLD );
		}

		// display string subject
	IanDisplayWrappedString(SUBJECT_X, (( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) , SUBJECT_WIDTH, MESSAGE_GAP, FONT10ARIALBOLD, MESSAGE_COLOR ,pTempSubject,0 ,FALSE ,LEFT_JUSTIFIED );

	}
	SetFontShadow(DEFAULT_SHADOW);
	// reset font dest buffer
	SetFontDestBuffer(FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE	);
}

void DrawSender(INT32 iCounter, UINT8 ubSender, BOOLEAN fRead, UINT8 EmailType)
{
	// draw name of sender in mail viewer
	SetFontShadow(NO_SHADOW);

	SetFontShadow(NO_SHADOW);
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );

	if( fRead )
	{
		SetFont( MESSAGE_FONT );
	}
	else
	{
		SetFont( FONT10ARIALBOLD );
	}

	if ( EmailType == TYPE_EMAIL_AIM_AVAILABLE || EmailType == TYPE_EMAIL_MERC_LEVEL_UP )
	{
		if (gMercProfiles[ ubSender ].zNickname !='\0')
		mprintf(SENDER_X,(( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) ,gMercProfiles[ ubSender ].zNickname);
		else
		mprintf(SENDER_X,(( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) ,L"None");
	}
	else if ( EmailType == TYPE_EMAIL_EMAIL_EDT || EmailType == TYPE_EMAIL_BOBBY_R || EmailType == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT ||  EmailType == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT || EmailType == TYPE_EMAIL_OTHER )
	{
		mprintf(SENDER_X,(( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) ,pSenderNameList[ubSender]);
	}
	else if ( EmailType == TYPE_EMAIL_EMAIL_EDT_NAME_MERC )
	{
		if (gMercProfiles[ ubSender ].zNickname !='\0')
		mprintf(SENDER_X,(( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) ,gMercProfiles[ ubSender ].zNickname);
		else
		mprintf(SENDER_X,(( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ) ,L"None");
	}

	SetFont( MESSAGE_FONT );
	SetFontShadow(DEFAULT_SHADOW);
}

void DrawDate(INT32 iCounter, INT32 iDate, BOOLEAN fRead)
{
	CHAR16 sString[20];

	SetFontShadow(NO_SHADOW);
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );

	if( fRead )
	{
		SetFont( MESSAGE_FONT );
	}
	else
	{
		SetFont( FONT10ARIALBOLD );
	}
	// draw date of message being displayed in mail viewer
	swprintf(sString, L"%s %d", pDayStrings[ 0 ], iDate/ ( 24 * 60 ) );
	mprintf(DATE_X,(( UINT16 )( 4 + MIDDLE_Y + iCounter * MIDDLE_WIDTH ) ),sString);

	SetFont( MESSAGE_FONT );
	SetFontShadow(DEFAULT_SHADOW);
}

void DisplayEmailList()
{
	INT32 iCounter=0;
	// look at current page, and display
	PagePtr pPage=pPageList;
	EmailPtr pEmail=NULL;
	
	// error check, if no page, return
	if(!pPage)
		return;

	// if current page ever ends up negative, reset to 0
	if(iCurrentPage==-1)
		iCurrentPage=0;

	// loop until we get to the current page
	while((pPage->iPageId!=iCurrentPage)&&(iCurrentPage <=iLastPage))
		pPage=pPage->Next;

	// now we have current page, display it
	pEmail=GetEmailMessage(pPage->iIds[iCounter]);
	SetFontShadow(NO_SHADOW);
	SetFont(EMAIL_TEXT_FONT);
	
	// draw each line of the list for this page
	while(pEmail)
	{
		// highlighted message, set text of message in list to blue
		if(iCounter==iHighLightLine)
		{
			SetFontForeground(FONT_BLUE);
		}
		else if(pEmail->fRead)
		{
			// message has been read, reset color to black
			SetFontForeground(FONT_BLACK);
	 //SetFontBackground(FONT_BLACK);

		}
		else
		{
			// defualt, message is not read, set font red
			SetFontForeground(FONT_RED);
	 //SetFontBackground(FONT_BLACK);
		}
		SetFontBackground(FONT_BLACK);

		//draw the icon, sender, date, subject
		DrawLetterIcon(iCounter,pEmail->fRead );
		DrawSubject(iCounter, pEmail->pSubject, pEmail->fRead );
		
		if ( pEmail->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE || pEmail->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP )
			DrawSender(iCounter, pEmail->ubSender, pEmail->fRead, pEmail->EmailVersion);
		else if ( pEmail->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pEmail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC || pEmail->EmailVersion == TYPE_EMAIL_BOBBY_R || pEmail->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT || pEmail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pEmail->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT || pEmail->EmailVersion == TYPE_EMAIL_OTHER )
			DrawSender(iCounter, pEmail->ubSender, pEmail->fRead, pEmail->EmailVersion);
			
		DrawDate(iCounter, pEmail->iDate, pEmail->fRead );

		++iCounter;

		// too many messages onthis page, reset pEmail, so no more are drawn
		if(iCounter >= MAX_MESSAGES_PAGE)
			pEmail=NULL;
		else
			pEmail=GetEmailMessage(pPage->iIds[iCounter]);
	}
	
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_LR_Y);

	SetFontShadow(DEFAULT_SHADOW);
}

void LookForUnread()
{
	BOOLEAN fStatusOfNewEmailFlag = fUnReadMailFlag;

	// simply runrs through list of messages, if any unread, set unread flag

	EmailPtr pA=pEmailList;

	// reset unread flag
	fUnReadMailFlag=FALSE;

	// look for unread mail
	while(pA)
	{
		// unread mail found, set flag
		if(!(pA->fRead))
	 fUnReadMailFlag=TRUE;
		pA=pA->Next;
	}

	if( fStatusOfNewEmailFlag != fUnReadMailFlag )
	{
		//Since there is no new email, get rid of the hepl text
		CreateFileAndNewEmailIconFastHelpText( LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_MAIL, (BOOLEAN )!fUnReadMailFlag );
	}

	return;
}

void EmailBtnCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
 INT32 iCount;
 PagePtr pPage=pPageList;
 INT32 iId=0;
 if (iReason & MSYS_CALLBACK_REASON_INIT)
 {
	return;
 }
 if(fDisplayMessageFlag)
	return;
 if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
 {

	// error check
	iCount=MSYS_GetRegionUserData(pRegion, 0);
	// check for valid email
	// find surrent page
	if(!pPage)
		return;
	while((pPage->Next)&&(pPage->iPageId!=iCurrentPage))
		pPage=pPage->Next;
	if(!pPage)
		return;
	// found page

	// get id for element iCount
	iId=pPage->iIds[iCount];

	// invalid message
	if(iId==-1)
	{
		fDisplayMessageFlag=FALSE;
		return;
	}
	// Get email and display
	fDisplayMessageFlag=TRUE;
	giMessagePage = 0;
	giPrevMessageId = giMessageId;
	giMessageId=iId;


 }
 else if(iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
 {
	iCount=MSYS_GetRegionUserData(pRegion, 0);

	// error check
	if(!pPage)
	{
		HandleRightButtonUpEvent( );
		return;
	}

 	giMessagePage = 0;

	while((pPage->Next)&&(pPage->iPageId!=iCurrentPage))
		pPage=pPage->Next;
	if(!pPage)
	{
		HandleRightButtonUpEvent( );
		return;
	}
	// found page
	// get id for element iCount
	iId=pPage->iIds[iCount];
	if(!GetEmailMessage(iId))
	{
		// no mail here, handle right button up event
		HandleRightButtonUpEvent( );
		return;
	}
	else
	{
		fDeleteMailFlag=TRUE;
		iDeleteId=iId;
		//DisplayDeleteNotice(GetEmailMessage(iDeleteId));
		//DeleteEmail();
	}
 }
}
void EmailMvtCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if(fDisplayMessageFlag)
		return;
	if (iReason == MSYS_CALLBACK_REASON_MOVE)
	{

		// set highlight to current regions data, this is the message to display
	iHighLightLine=MSYS_GetRegionUserData(pRegion, 0);
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{

		// reset highlight line to invalid message
	iHighLightLine=-1;
	}
}

void BtnMessageXCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if((reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )||(reason & MSYS_CALLBACK_REASON_RBUTTON_DWN))
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if((reason & MSYS_CALLBACK_REASON_LBUTTON_UP )||(reason & MSYS_CALLBACK_REASON_RBUTTON_UP))
	{
		if(btn->uiFlags& BUTTON_CLICKED_ON)
		{
			// X button has been pressed and let up, this means to stop displaying the currently displayed message

			// reset display message flag
			fDisplayMessageFlag=FALSE;

			// reset button flag
			btn->uiFlags &= ~BUTTON_CLICKED_ON;

			// reset page being displayed
			giMessagePage = 0;

			// redraw icons
			DrawLapTopIcons();

			// force update of entire screen
			fPausedReDrawScreenFlag=TRUE;

			// rerender email
			//RenderEmail();
		}
	}
}
void SetUnNewMessages()
{
	// on exit from the mailer, set all new messages as 'un'new
	EmailPtr pEmail=pEmailList;
	
	// run through the list of messages and add to pages
	while(pEmail)
	{
		pEmail->fNew=FALSE;
		pEmail=pEmail->Next;
	}
	return;
}

INT32 DisplayEmailMessage(EmailPtr pMail)
{
	HVOBJECT hHandle;
	INT32 iHeight=0;
	INT32 iCounter=1;
//	CHAR16 pString[MAIL_STRING_SIZE/2 + 1];
	CHAR16 pString[MAIL_STRING_SIZE];
	INT32 iOffSet=0;
	RecordPtr pTempRecord;
	BOOLEAN fDonePrintingMessage = FALSE;
	
	if(!pMail)
		return 0;

	iOffSet=(INT32)pMail->usOffset;

	// reset redraw email message flag
	fReDrawMessageFlag = FALSE;

	// we KNOW the player is going to "read" this, so mark it as so
	pMail->fRead=TRUE;

	giCurrentIMPSlot = pMail->iCurrentIMPPosition;

	// draw text for title bar
	//swprintf(pString, L"%s / %s", pSenderNameList[pMail->ubSender],pMail->pSubject);
	//DisplayWrappedString(VIEWER_X+VIEWER_HEAD_X+4, VIEWER_Y+VIEWER_HEAD_Y+4, VIEWER_HEAD_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR, pString, 0,FALSE,0);

	// increment height for size of one line
	iHeight+=GetFontHeight( MESSAGE_FONT );

	// is there any special event meant for this mail?..if so, handle it
	if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT )
	HandleAnySpecialEmailMessageEvents( iOffSet );

	if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT ) //|| pMail->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT )
	HandleMailSpecialMessages( ( UINT16 ) ( iOffSet ), &iViewerPositionY, pMail);
	
	#ifdef JA2UB
	if ( pMail->EmailType != TYPE_E_NONE )
	{
		HandleNewUBMailSpecialMessages( 198, &iViewerPositionY, pMail );
		HandleNewUBMailSpecialMessages( 170, &iViewerPositionY, pMail );
		HandleNewUBMailSpecialMessages( 211, &iViewerPositionY, pMail );
		HandleNewUBMailSpecialMessages( 206, &iViewerPositionY, pMail );
		HandleNewUBMailSpecialMessages( 217, &iViewerPositionY, pMail );
	}
	#endif
	
	PreProcessEmail( pMail );
	
	pTempRecord = pMessageRecordList;
	
	// blt in top line of message as a blank graphic
	// get a handle to the bitmap of EMAIL VIEWER Background
	GetVideoObject( &hHandle, guiEmailMessage );

	// place the graphic on the frame buffer
	BltVideoObject( FRAME_BUFFER, hHandle, 1,VIEWER_X, VIEWER_MESSAGE_BODY_START_Y + iViewerPositionY, VO_BLT_SRCTRANSPARENCY,NULL );
	BltVideoObject( FRAME_BUFFER, hHandle, 1,VIEWER_X, VIEWER_MESSAGE_BODY_START_Y + GetFontHeight( MESSAGE_FONT ) + iViewerPositionY, VO_BLT_SRCTRANSPARENCY,NULL );

	// set shadow
	SetFontShadow(NO_SHADOW);

	// get a handle to the bitmap of EMAIL VIEWER
	GetVideoObject(&hHandle, guiEmailMessage);

	// place the graphic on the frame buffer
	BltVideoObject(FRAME_BUFFER, hHandle, 0,VIEWER_X, VIEWER_Y + iViewerPositionY, VO_BLT_SRCTRANSPARENCY,NULL);
	
	// the icon for the title of this box
	GetVideoObject( &hHandle, guiTITLEBARICONS );
	BltVideoObject( FRAME_BUFFER, hHandle, 0,VIEWER_X + 5, VIEWER_Y + iViewerPositionY + 2, VO_BLT_SRCTRANSPARENCY,NULL );

	// display header text
	DisplayEmailMessageSubjectDateFromLines( pMail, iViewerPositionY );

	// display title text
	DrawEmailMessageDisplayTitleText( iViewerPositionY );
	
	iCounter=0;
	// now blit the text background based on height
	for (iCounter=2; iCounter < ( ( iTotalHeight ) / ( GetFontHeight( MESSAGE_FONT ) ) ); iCounter++ )
	{
	// get a handle to the bitmap of EMAIL VIEWER Background
	GetVideoObject( &hHandle, guiEmailMessage );

	// place the graphic on the frame buffer
	BltVideoObject( FRAME_BUFFER, hHandle, 1,VIEWER_X, iViewerPositionY + VIEWER_MESSAGE_BODY_START_Y+( (GetFontHeight( MESSAGE_FONT ) ) * ( iCounter )), VO_BLT_SRCTRANSPARENCY,NULL );
	}

	// now the bottom piece to the message viewer
	GetVideoObject( &hHandle, guiEmailMessage );

	if( giNumberOfPagesToCurrentEmail <= 2 )
	{
		// place the graphic on the frame buffer
		BltVideoObject( FRAME_BUFFER, hHandle, 2,VIEWER_X, iViewerPositionY + VIEWER_MESSAGE_BODY_START_Y+( ( GetFontHeight( MESSAGE_FONT ) ) * ( iCounter )), VO_BLT_SRCTRANSPARENCY,NULL );
	}
	else
	{
		// place the graphic on the frame buffer
		BltVideoObject( FRAME_BUFFER, hHandle, 3,VIEWER_X, iViewerPositionY + VIEWER_MESSAGE_BODY_START_Y+( ( GetFontHeight( MESSAGE_FONT ) ) * ( iCounter )), VO_BLT_SRCTRANSPARENCY,NULL );
	}

	// reset iCounter and iHeight
	iCounter = 1;
	iHeight = GetFontHeight(MESSAGE_FONT);

	// draw body of text. Any particular email can encompass more than one "record" in the
	// email file. Draw each record (length is number of records)

	// now place the text

	// reset ptemprecord to head of list
	pTempRecord = pMessageRecordList;
	// reset shadow
	SetFontShadow( NO_SHADOW );

	pTempRecord = pEmailPageInfo[ giMessagePage ].pFirstRecord;

	if( pTempRecord )
	{
		while( fDonePrintingMessage == FALSE )
		{
			// copy over string
			wcscpy( pString, pTempRecord->pRecord );

			// get the height of the string, ONLY!...must redisplay ON TOP OF background graphic
			iHeight += IanDisplayWrappedString(VIEWER_X + 9, ( UINT16 )( VIEWER_MESSAGE_BODY_START_Y + iHeight + iViewerPositionY), MESSAGE_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR,pString,0,FALSE, IAN_WRAP_NO_SHADOW);

			// increment email record ptr
			pTempRecord = pTempRecord->Next;

			if( pTempRecord == NULL )
			{
				fDonePrintingMessage = TRUE;
			}
			else if( ( pTempRecord == pEmailPageInfo[ giMessagePage ].pLastRecord ) && (	pEmailPageInfo[ giMessagePage + 1 ].pFirstRecord != NULL ) )
			{
				fDonePrintingMessage = TRUE;
			}
		}
	}

	/*
	if(iTotalHeight < MAX_EMAIL_MESSAGE_PAGE_SIZE)
	{
		fOnLastPageFlag = TRUE;
	while( pTempRecord )
		{
		// copy over string
		wcscpy( pString, pTempRecord->pRecord );

	 // get the height of the string, ONLY!...must redisplay ON TOP OF background graphic
	 iHeight += IanDisplayWrappedString(VIEWER_X + MESSAGE_X + 4, ( UINT16 )( VIEWER_MESSAGE_BODY_START_Y + iHeight + iViewerPositionY), MESSAGE_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR,pString,0,FALSE, IAN_WRAP_NO_SHADOW);

			// increment email record ptr
		pTempRecord = pTempRecord->Next;
		}


	}
	else
	{

		iYPositionOnPage = 0;
		// go to the right record
		pTempRecord = GetFirstRecordOnThisPage( pMessageRecordList, MESSAGE_FONT, MESSAGE_WIDTH, MESSAGE_GAP, giMessagePage, MAX_EMAIL_MESSAGE_PAGE_SIZE );
	while( pTempRecord )
		{
			// copy over string
		wcscpy( pString, pTempRecord->pRecord );

			if( pString[ 0 ] == 0 )
			{
				// on last page
				fOnLastPageFlag = TRUE;
			}


			if( ( iYPositionOnPage + IanWrappedStringHeight(0, 0, MESSAGE_WIDTH, MESSAGE_GAP,
															MESSAGE_FONT, 0, pTempRecord->pRecord,
															0, 0, 0 ) )	<= MAX_EMAIL_MESSAGE_PAGE_SIZE	)
			{
	 	// now print it
		 iYPositionOnPage += IanDisplayWrappedString(VIEWER_X + MESSAGE_X + 4, ( UINT16 )( VIEWER_MESSAGE_BODY_START_Y + 10 +iYPositionOnPage + iViewerPositionY), MESSAGE_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR,pString,0,FALSE, IAN_WRAP_NO_SHADOW);
				fGoingOffCurrentPage = FALSE;
			}
			else
			{
				// gonna get cut off...end now
				fGoingOffCurrentPage = TRUE;
			}


			pTempRecord = pTempRecord->Next;


			if( ( pTempRecord == NULL ) && ( fGoingOffCurrentPage == FALSE ) )
			{
				// on last page
				fOnLastPageFlag = TRUE;
			}
			else
			{
				fOnLastPageFlag = FALSE;
			}

			// record get cut off?...end now

			if( fGoingOffCurrentPage == TRUE )
			{
				pTempRecord = NULL;
			}
		}

	}

	*/
	// show number of pages to this email
	DisplayNumberOfPagesToThisEmail( iViewerPositionY );

	// mark this area dirty
	InvalidateRegion( LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_LR_Y );
	
	// reset shadow
	SetFontShadow( DEFAULT_SHADOW );
	
	return iViewerPositionY;
}

void BtnNewOkback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}

		btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags&=~(BUTTON_CLICKED_ON);
			fNewMailFlag=FALSE;
		}
	}
}

void AddDeleteRegionsToMessageRegion(INT32 iViewerY)
{
	// will create/destroy mouse region for message display
	if((fDisplayMessageFlag)&&(!fOldDisplayMessageFlag))
	{
		// set old flag
		fOldDisplayMessageFlag=TRUE;

		// add X button
		giMessageButtonImage[0]=	LoadButtonImage( "LAPTOP\\X.sti" ,-1,0,-1,1,-1 );
		giMessageButton[0] = QuickCreateButton( giMessageButtonImage[0], BUTTON_X + 2,(INT16) ( BUTTON_Y + ( INT16 )iViewerY + 1),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnMessageXCallback );
		SetButtonCursor(giMessageButton[0], CURSOR_LAPTOP_SCREEN);

		if( giNumberOfPagesToCurrentEmail > 2 )
		{
			// add next and previous mail page buttons
			giMailMessageButtonsImage[0]=	LoadButtonImage( "LAPTOP\\NewMailButtons.sti" ,-1,0,-1,3,-1 );
			giMailMessageButtons[0] = QuickCreateButton( giMailMessageButtonsImage[0], PREVIOUS_PAGE_BUTTON_X,(INT16) ( LOWER_BUTTON_Y + ( INT16 )iViewerY + 2),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnPreviousEmailPageCallback );

			giMailMessageButtonsImage[1]=	LoadButtonImage( "LAPTOP\\NewMailButtons.sti" ,-1,1,-1,4,-1 );
			giMailMessageButtons[1] = QuickCreateButton( giMailMessageButtonsImage[1], NEXT_PAGE_BUTTON_X ,(INT16) ( LOWER_BUTTON_Y + ( INT16 )iViewerY + 2),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnNextEmailPageCallback );

			gfPageButtonsWereCreated = TRUE;
		}

		// add delete message button
		giMailMessageButtonsImage[2]=	LoadButtonImage( "LAPTOP\\NewMailButtons.sti" ,-1,2,-1,5,-1 );
		giMailMessageButtons[2] = QuickCreateButton( giMailMessageButtonsImage[2], DELETE_BUTTON_X,(INT16) ( BUTTON_LOWER_Y + ( INT16 )iViewerY + 2),
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnDeleteCallback );
/*
		// set up disable methods
		SpecifyDisabledButtonStyle( giMailMessageButtons[1], DISABLED_STYLE_SHADED );
		SpecifyDisabledButtonStyle( giMailMessageButtons[0], DISABLED_STYLE_SHADED );
*/
		// set cursors
		SetButtonCursor(giMailMessageButtons[0], CURSOR_LAPTOP_SCREEN);
		SetButtonCursor(giMailMessageButtons[1], CURSOR_LAPTOP_SCREEN);
		SetButtonCursor(giMailMessageButtons[2], CURSOR_LAPTOP_SCREEN);
		SetButtonCursor(giMessageButton[0], CURSOR_LAPTOP_SCREEN);

		// set up email message region
		MSYS_DefineRegion( &pMailViewMessageRegion, VIEWER_X + 2, (INT16) ( VIEWER_Y + (INT16)iViewerY + 2), VIEWER_X + 416, (INT16) ( VIEWER_Y + (INT16)iViewerY + 72 + iTotalHeight), MSYS_PRIORITY_HIGH, 
						CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, ViewMessageRegionCallBack );
		MSYS_AddRegion( &pMailViewMessageRegion );

		// force update of screen
		fReDrawScreenFlag=TRUE;
	}
	else if((!fDisplayMessageFlag)&&(fOldDisplayMessageFlag))
	{
		// delete region
		fOldDisplayMessageFlag=FALSE;
		RemoveButton(giMessageButton[0] );
		UnloadButtonImage( giMessageButtonImage[0] );

		// next/previous email page buttons
		if( gfPageButtonsWereCreated )
		{
			RemoveButton(giMailMessageButtons[0] );
			UnloadButtonImage( giMailMessageButtonsImage[0] );
			RemoveButton(giMailMessageButtons[1] );
			UnloadButtonImage( giMailMessageButtonsImage[1] );
			gfPageButtonsWereCreated = FALSE;
		}
		
		RemoveButton(giMailMessageButtons[2] );
		UnloadButtonImage( giMailMessageButtonsImage[2] );
		
		MSYS_RemoveRegion( &pMailViewMessageRegion );

		// force update of screen
		fReDrawScreenFlag=TRUE;
	}
}

void CreateDestroyNewMailButton()
{
	static BOOLEAN fOldNewMailFlag=FALSE;

	// check if we are video conferencing, if so, do nothing
	if( gubVideoConferencingMode != 0 )
	{
		return;
	}

	if((fNewMailFlag)&&(!fOldNewMailFlag))
	{
		// create new mail dialog box button

		// set old flag (stating button has been created)
		fOldNewMailFlag=TRUE;

		// load image and setup button
		giNewMailButtonImage[0]=LoadButtonImage( "LAPTOP\\YesNoButtons.sti" ,-1,0,-1,1,-1 );
		giNewMailButton[0]= QuickCreateButton( giNewMailButtonImage[0], NEW_BTN_X+10, NEW_BTN_Y,
											BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST-2,
											(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnNewOkback);

		// set cursor
		SetButtonCursor(giNewMailButton[0], CURSOR_LAPTOP_SCREEN);

		// set up screen mask region
		MSYS_DefineRegion(&pScreenMask, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_HIGHEST-3, 
						CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, LapTopScreenCallBack);
		MSYS_AddRegion(&pScreenMask);
		MarkAButtonDirty(	giNewMailButton[0] );
		fReDrawScreenFlag = TRUE;
	}
	else if((!fNewMailFlag)&&(fOldNewMailFlag))
	{
		// reset old flag
		fOldNewMailFlag=FALSE;

		// remove the button
		RemoveButton( giNewMailButton[0] );
		UnloadButtonImage( giNewMailButtonImage[0] );

		// remove screen mask
		MSYS_RemoveRegion( &pScreenMask );

		//re draw screen
		fReDraw=TRUE;

		// redraw screen
		fPausedReDrawScreenFlag=TRUE;
	}
}

BOOLEAN DisplayNewMailBox( void )
{
	HVOBJECT hHandle;
	static BOOLEAN fOldNewMailFlag=FALSE;
	// will display a new mail box whenever new mail has arrived

	// check if we are video conferencing, if so, do nothing
	if( gubVideoConferencingMode != 0 )
	{
		return( FALSE );
	}

	// just stopped displaying box, reset old flag
	if( ( !fNewMailFlag ) && ( fOldNewMailFlag ) )
	{
		fOldNewMailFlag=FALSE;
		return ( FALSE );
	}

	// not even set, leave NOW!
	if( !fNewMailFlag )
		return ( FALSE );

	// is set but already drawn, LEAVE NOW!
	//if( ( fNewMailFlag ) && ( fOldNewMailFlag ) )
	//	return ( FALSE );
	
	GetVideoObject( &hHandle, guiEmailWarning );
	BltVideoObject( FRAME_BUFFER, hHandle, 0,EMAIL_WARNING_X, EMAIL_WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL );
	
	// the icon for the title of this box
	GetVideoObject( &hHandle, guiTITLEBARICONS );
	BltVideoObject( FRAME_BUFFER, hHandle, 0,EMAIL_WARNING_X + 5, EMAIL_WARNING_Y + 2, VO_BLT_SRCTRANSPARENCY,NULL );

	// font stuff
	SetFont( EMAIL_HEADER_FONT );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );
	SetFontShadow( DEFAULT_SHADOW );

	// print warning
	mprintf(EMAIL_WARNING_X + 30, EMAIL_WARNING_Y + 8, pEmailTitleText[0] );

	// font stuff
	SetFontShadow( NO_SHADOW );
	SetFont( EMAIL_WARNING_FONT );
	SetFontForeground( FONT_BLACK );

	// printf warning string
	mprintf(EMAIL_WARNING_X + 60, EMAIL_WARNING_Y + 63, pNewMailStrings[0] );
	DrawLapTopIcons( );

	// invalidate region
	InvalidateRegion( EMAIL_WARNING_X, EMAIL_WARNING_Y, EMAIL_WARNING_X + 270, EMAIL_WARNING_Y + 200 );

	// mark button
	MarkAButtonDirty(	giNewMailButton[0] );

	// reset shadow
	SetFontShadow( DEFAULT_SHADOW );

	// redraw icons

	// set box as displayed
	fOldNewMailFlag=TRUE;

	// return
	return ( TRUE );
}

void ReDrawNewMailBox( void )
{
	// this function will check to see if the new mail region needs to be redrawn
	if( fReDrawNewMailFlag == TRUE )
	{
		if( fNewMailFlag )
		{
			// set display flag back to orginal
			fNewMailFlag = FALSE;

			// display new mail box
			DisplayNewMailBox( );

			// dirty buttons
			MarkAButtonDirty(	giNewMailButton[0] );

			// set display flag back to orginal
			fNewMailFlag = TRUE;

			// time to redraw
			DisplayNewMailBox( );
		}
		
		// reset flag for redraw
		fReDrawNewMailFlag = FALSE;
	}
}

void SwitchEmailPages( void )
{
	// this function will switch current page

	// gone too far, reset page to start
	if( iCurrentPage >iLastPage )
		iCurrentPage=0;

	// set to last page
	if( iCurrentPage < 0 )
		iCurrentPage=iLastPage;
}


void DetermineNextPrevPageDisplay( void )
{
	// will determine which of previous and next page graphics to display	
	if( iCurrentPage > 0 )
	{
		// display Previous graphic

		// font stuff
	SetFont( TRAVERSE_EMAIL_FONT );
		SetFontForeground( FONT_RED );
		SetFontBackground( FONT_BLACK );

		// print previous string
	mprintf( PREVIOUS_PAGE_X, PREVIOUS_PAGE_Y, pTraverseStrings[PREVIOUS_PAGE] );
	}

	// less than last page, so there is a next page
	if(iCurrentPage <iLastPage)
	{
		// display Next graphic

		// font stuff
	SetFont( TRAVERSE_EMAIL_FONT );
		SetFontForeground( FONT_RED );
		SetFontBackground( FONT_BLACK );

		// next string
	mprintf( NEXT_PAGE_X, NEXT_PAGE_Y, pTraverseStrings[NEXT_PAGE] );
	}
}

void NextRegionButtonCallback(GUI_BUTTON *btn,INT32 reason )
{

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

		// not on last page, move ahead one
			if(iCurrentPage <iLastPage)
			{
				iCurrentPage++;
				fReDraw=TRUE;
				RenderEmail();
				MarkButtonsDirty( );
			}
		}
 }
 else if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
 {
	// nothing yet
 }
}

void BtnPreviousEmailPageCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
			if( giMessagePage > 0 )
			{
				giMessagePage--;
			}

			btn->uiFlags&=~(BUTTON_CLICKED_ON);

			fReDraw=TRUE;
			RenderEmail();
			MarkButtonsDirty( );
		}
	}
	else if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	// nothing yet
	}
}

void BtnNextEmailPageCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
	// not on last page, move ahead one
		btn->uiFlags&=~(BUTTON_CLICKED_ON);

		if( ( giNumberOfPagesToCurrentEmail - 1 ) <= giMessagePage )
		{
			return;
		}

	if( ! ( fOnLastPageFlag ) )
		{
			if( ( giNumberOfPagesToCurrentEmail - 1 ) > ( giMessagePage + 1 ) )
				giMessagePage++;
		}

		MarkButtonsDirty( );
		fReDrawScreenFlag = TRUE;
	}
	else if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	// nothing yet
	}
}

void PreviousRegionButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
 if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
			// if we are not on forst page, more back one
			if(iCurrentPage>0)
			{
				iCurrentPage--;
				fReDraw=TRUE;
				RenderEmail();
				MarkButtonsDirty( );
			}
		}
 }
 else if (reason & MSYS_CALLBACK_REASON_RBUTTON_UP)
 {
	// nothing yet
 }
}

void BtnDeleteNoback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		fDeleteMailFlag=FALSE;
		fReDrawScreenFlag=TRUE;
		}
	}
}

void BtnDeleteYesback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		if(!(btn->uiFlags & BUTTON_CLICKED_ON))
		{
		}
	btn->uiFlags|=(BUTTON_CLICKED_ON);
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if(btn->uiFlags & BUTTON_CLICKED_ON)
		{
		btn->uiFlags&=~(BUTTON_CLICKED_ON);
		fReDrawScreenFlag=TRUE;
		DeleteEmail();

		}
	}
}

void CreateDestroyNextPreviousRegions()
{
	static BOOLEAN fCreated=FALSE;
	if(fCreated)
	{
		// destroy already create next, previous mouse regions
	fCreated=FALSE;

		RemoveButton( giMailPageButtons[ 1 ] );
		UnloadButtonImage( giMailPageButtonsImage[ 1 ] );

		RemoveButton( giMailPageButtons[ 0 ] );
		UnloadButtonImage( giMailPageButtonsImage[ 0 ] );

	}
	else
	{
		// create uncreated mouse regions
		fCreated=TRUE;

		CreateNextPreviousEmailPageButtons( );

		/*
		// ' next' region
	MSYS_DefineRegion(&pEmailMoveRegions[NEXT_BUTTON],NEXT_PAGE_X, NEXT_PAGE_Y,(INT16) (NEXT_PAGE_X+NEXT_WIDTH), (INT16)(NEXT_PAGE_Y+NEXT_HEIGHT),
			MSYS_PRIORITY_NORMAL+2,MSYS_NO_CURSOR, MSYS_NO_CALLBACK, NextRegionButtonCallback);

		// ' previous ' region
	MSYS_DefineRegion(&pEmailMoveRegions[PREVIOUS_BUTTON],PREVIOUS_PAGE_X,PREVIOUS_PAGE_Y, (INT16)(PREVIOUS_PAGE_X+PREVIOUS_WIDTH),(INT16)(PREVIOUS_PAGE_Y+PREVIOUS_HEIGHT),
			MSYS_PRIORITY_NORMAL+2,MSYS_NO_CURSOR, MSYS_NO_CALLBACK, PreviousRegionButtonCallback );

		// add regions
		MSYS_AddRegion(&pEmailMoveRegions[PREVIOUS_BUTTON]);
	MSYS_AddRegion(&pEmailMoveRegions[NEXT_BUTTON]);
		*/
	}
}

void ReDraw()
{
	// forces update of entire laptop screen
	if(fReDraw)
	{
		RenderLaptop( );
		//EnterNewLaptopMode();
		DrawLapTopText();
		ReDrawHighLight();
		MarkButtonsDirty( );
		fReDraw=FALSE;
	}
}

void CreateDestroyDeleteNoticeMailButton()
{
 static BOOLEAN fOldDeleteMailFlag=FALSE;
 if((fDeleteMailFlag)&&(!fOldDeleteMailFlag))
 {
	// confirm delete email buttons

	// YES button
	fOldDeleteMailFlag=TRUE;
	giDeleteMailButtonImage[0]=LoadButtonImage( "LAPTOP\\YesNoButtons.sti" ,-1,0,-1,1,-1 );
	giDeleteMailButton[0]= QuickCreateButton( giDeleteMailButtonImage[0], NEW_BTN_X+1, NEW_BTN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 2,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnDeleteYesback);

	// NO button
	giDeleteMailButtonImage[1]=LoadButtonImage( "LAPTOP\\YesNoButtons.sti" ,-1,2,-1,3,-1 );
	giDeleteMailButton[1]= QuickCreateButton( giDeleteMailButtonImage[1], NEW_BTN_X+40, NEW_BTN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 2,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)BtnDeleteNoback);

	// set up cursors
	SetButtonCursor(giDeleteMailButton[0], CURSOR_LAPTOP_SCREEN);
	SetButtonCursor(giDeleteMailButton[1], CURSOR_LAPTOP_SCREEN);

	// set up screen mask to prevent other actions while delete mail box is destroyed
	MSYS_DefineRegion(&pDeleteScreenMask,0, 0,SCREEN_WIDTH, SCREEN_HEIGHT,
		MSYS_PRIORITY_HIGHEST-3,CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, LapTopScreenCallBack);
	MSYS_AddRegion(&pDeleteScreenMask);

	// force update
	fReDrawScreenFlag = TRUE;

 }
 else if((!fDeleteMailFlag)&&(fOldDeleteMailFlag))
 {

	// clear out the buttons and screen mask
	fOldDeleteMailFlag=FALSE;
	RemoveButton( giDeleteMailButton[0] );
	UnloadButtonImage( giDeleteMailButtonImage[0] );
	RemoveButton( giDeleteMailButton[1] );
	UnloadButtonImage( giDeleteMailButtonImage[1] );

	// the region
	MSYS_RemoveRegion(&pDeleteScreenMask);

	// force refresh
	fReDrawScreenFlag=TRUE;
 }
}

BOOLEAN DisplayDeleteNotice(EmailPtr pMail)
{
	HVOBJECT hHandle;
	// will display a delete mail box whenever delete mail has arrived
	if(!fDeleteMailFlag)
		return(FALSE);

	if( !fReDrawScreenFlag )
	{
		// no redraw flag, leave
		return( FALSE );
	}

	// error check.. no valid message passed
	if( pMail == NULL )
	{
		return ( FALSE );
	}
	
	// load graphics

	GetVideoObject(&hHandle, guiEmailWarning);
	BltVideoObject(FRAME_BUFFER, hHandle, 0,EMAIL_WARNING_X, EMAIL_WARNING_Y, VO_BLT_SRCTRANSPARENCY,NULL);
	
	// font stuff
	SetFont( EMAIL_HEADER_FONT );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );
	SetFontShadow( DEFAULT_SHADOW );

	// the icon for the title of this box
	GetVideoObject( &hHandle, guiTITLEBARICONS );
	BltVideoObject( FRAME_BUFFER, hHandle, 0,EMAIL_WARNING_X + 5, EMAIL_WARNING_Y + 2, VO_BLT_SRCTRANSPARENCY,NULL );

	// title
	mprintf(EMAIL_WARNING_X + 30, EMAIL_WARNING_Y + 8, pEmailTitleText[0] );

	// shadow, font, and foreground
	SetFontShadow( NO_SHADOW );
	SetFont( EMAIL_WARNING_FONT );
	SetFontForeground( FONT_BLACK );

	// draw text based on mail being read or not
	if((pMail->fRead))
	mprintf(EMAIL_WARNING_X + 95 , EMAIL_WARNING_Y + 65,pDeleteMailStrings[0]);
	else
	mprintf(EMAIL_WARNING_X + 70, EMAIL_WARNING_Y + 65,pDeleteMailStrings[1]);
	
	// invalidate screen area, for refresh

	if( ! fNewMailFlag )
	{
		// draw buttons
	MarkButtonsDirty( );
	InvalidateRegion(EMAIL_WARNING_X, EMAIL_WARNING_Y ,EMAIL_WARNING_X+EMAIL_WARNING_WIDTH,EMAIL_WARNING_Y+EMAIL_WARNING_HEIGHT);
	}

	// reset font shadow
	SetFontShadow(DEFAULT_SHADOW);

	return ( TRUE );
}

void DeleteEmail()
{
	// error check, invalid mail, or not time to delete mail
	if( fDeleteInternal != TRUE )
	{
	if((iDeleteId==-1)||(!fDeleteMailFlag))
		return;
	}
	// remove the message
	RemoveEmailMessage(iDeleteId);

	// stop displaying message, if so
	fDisplayMessageFlag = FALSE;

	// upadte list
	PlaceMessagesinPages();

	// redraw icons (if deleted message was last unread, remove checkmark)
	DrawLapTopIcons();

	// if all of a sudden we are beyond last page, move back one
	if(iCurrentPage > iLastPage)
		iCurrentPage=iLastPage;

	// rerender mail list
	RenderEmail();

	// nolong time to delete mail
	fDeleteMailFlag=FALSE;
	fReDrawScreenFlag=TRUE;
	// refresh screen (get rid of dialog box image)
	//ReDraw();

	// invalidate
	InvalidateRegion(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
}


void FromCallback(GUI_BUTTON *btn, INT32 iReason )
{
 if (iReason & MSYS_CALLBACK_REASON_INIT)
 {
	return;
 }
 if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
 {
	// sort messages based on sender name, then replace into pages of email
	fSortSenderUpwards = !fSortSenderUpwards;

	SortMessages(SENDER);

	//SpecifyButtonIcon( giSortButton[1] , giArrowsForEmail, UINT16 usVideoObjectIndex,	INT8 bXOffset, INT8 bYOffset, TRUE );

	fJustStartedEmail = FALSE;

	PlaceMessagesinPages();
	btn->uiFlags&= ~(BUTTON_CLICKED_ON);
 }

 else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
 {
	// nothing yet
 }
}

void SubjectCallback(GUI_BUTTON *btn, INT32 iReason )
{
 if (iReason & MSYS_CALLBACK_REASON_INIT)
 {
	return;
 }
 if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
 {
	// sort message on subject and reorder list
	fSortSubjectUpwards = !fSortSubjectUpwards;

	SortMessages(SUBJECT);
	fJustStartedEmail = FALSE;
	PlaceMessagesinPages();
		
	btn->uiFlags&= ~(BUTTON_CLICKED_ON);
 }
 else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
 {
	// nothing yet
 }

}

void BtnDeleteCallback(GUI_BUTTON *btn, INT32 iReason )
{
 if (iReason & MSYS_CALLBACK_REASON_INIT)
 {
	return;
 }
 if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
 {

	btn->uiFlags&= ~(BUTTON_CLICKED_ON);
	iDeleteId = giMessageId;
	fDeleteMailFlag = TRUE;

 }
 else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
 {
	// nothing yet
 }
}

void DateCallback(GUI_BUTTON *btn, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// sort messages based on date recieved and reorder lsit
		fSortDateUpwards = !fSortDateUpwards;
		SortMessages(RECEIVED);
		PlaceMessagesinPages();

		fJustStartedEmail = FALSE;

		btn->uiFlags&= ~(BUTTON_CLICKED_ON);
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// nothing yet
	}
}

void ReadCallback(GUI_BUTTON *btn, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// sort messages based on date recieved and reorder lsit
		SortMessages(READ);
		PlaceMessagesinPages();

		fJustStartedEmail = FALSE;

		btn->uiFlags&= ~(BUTTON_CLICKED_ON);
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// nothing yet
	}
}

void ViewMessageRegionCallBack( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if( iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// simulate X button has been pressed and let up, this means to stop displaying the currently displayed message

		// reset display message flag
		fDisplayMessageFlag=FALSE;

		// reset page being displayed
		giMessagePage = 0;

		// redraw icons
		DrawLapTopIcons();

		// force update of entire screen
		fPausedReDrawScreenFlag=TRUE;

		// rerender email
		//RenderEmail();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
		// delete message
		iDeleteId = giMessageId;
		fDeleteMailFlag = TRUE;
	}
}

void SetUpSortRegions()
{
	// have been replaced by buttons
	return;

	// will set up sort mail regions

	// from region
	/*
	MSYS_DefineRegion(&pSortMailRegions[0],FROM_BOX_X ,FROM_BOX_Y, FROM_BOX_X+FROM_BOX_WIDTH ,FROM_BOX_Y+TOP_HEIGHT,
			MSYS_PRIORITY_NORMAL+2,MSYS_NO_CURSOR,MSYS_NO_CALLBACK, FromCallback );

	// subject region
	MSYS_DefineRegion(&pSortMailRegions[1],SUBJECT_X ,FROM_BOX_Y, SUBJECT_BOX_X+SUBJECT_WIDTH ,FROM_BOX_Y+TOP_HEIGHT,
			MSYS_PRIORITY_NORMAL+2,MSYS_NO_CURSOR,MSYS_NO_CALLBACK, SubjectCallback );

	// date region
	MSYS_DefineRegion(&pSortMailRegions[2],DATE_X ,FROM_BOX_Y, DATE_BOX_X+DATE_WIDTH ,FROM_BOX_Y+TOP_HEIGHT,
			MSYS_PRIORITY_NORMAL+2,MSYS_NO_CURSOR,MSYS_NO_CALLBACK, DateCallback );

	//add regions
	MSYS_AddRegion(&pSortMailRegions[0]);
	MSYS_AddRegion(&pSortMailRegions[1]);
	MSYS_AddRegion(&pSortMailRegions[2]);

	return;
	*/
}

void DeleteSortRegions()
{

	// have been replaced by buttons
 return;
 /*
 MSYS_RemoveRegion(&pSortMailRegions[0]);
 MSYS_RemoveRegion(&pSortMailRegions[1]);
 MSYS_RemoveRegion(&pSortMailRegions[2]);
 */
}

void DisplayTextOnTitleBar( void )
{
	// draw email screen title text

	// font stuff
	SetFont( EMAIL_TITLE_FONT );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	// printf the title
	mprintf( EMAIL_TITLE_X, EMAIL_TITLE_Y, pEmailTitleText[0] );

	// reset the shadow
}

void DestroyMailScreenButtons( void )
{
	// this function will destory the buttons used in the email screen

	// the sort email buttons
	RemoveButton( giSortButton[0] );
	UnloadButtonImage( giSortButtonImage[0] );
	RemoveButton( giSortButton[1] );
	UnloadButtonImage( giSortButtonImage[1] );
	RemoveButton( giSortButton[2] );
	UnloadButtonImage( giSortButtonImage[2] );
	RemoveButton( giSortButton[3] );
	UnloadButtonImage( giSortButtonImage[3] );
}

void CreateMailScreenButtons( void )
{
	// create sort buttons, right now - not finished

	// read sort
	giSortButtonImage[0]=	LoadButtonImage( "LAPTOP\\mailbuttons.sti" ,-1,0,-1,4,-1 );
	giSortButton[0] = QuickCreateButton( giSortButtonImage[0], ENVELOPE_BOX_X, FROM_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)ReadCallback);
	SetButtonCursor(giSortButton[0], CURSOR_LAPTOP_SCREEN);
	
	// subject sort
	giSortButtonImage[1]=	LoadButtonImage( "LAPTOP\\mailbuttons.sti" ,-1,1,-1,5,-1 );
	giSortButton[1] = QuickCreateButton( giSortButtonImage[1], FROM_BOX_X, FROM_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)FromCallback);
	SetButtonCursor(giSortButton[1], CURSOR_LAPTOP_SCREEN);
	SpecifyFullButtonTextAttributes( giSortButton[1], pEmailHeaders[FROM_HEADER], EMAIL_WARNING_FONT,
																		FONT_BLACK, FONT_BLACK,
																			FONT_BLACK, FONT_BLACK, TEXT_CJUSTIFIED );
	
	// sender sort
	giSortButtonImage[2]=	LoadButtonImage( "LAPTOP\\mailbuttons.sti" ,-1,2,-1,6,-1 );
	giSortButton[2] = QuickCreateButton( giSortButtonImage[2], SUBJECT_BOX_X, FROM_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)SubjectCallback );
	SetButtonCursor(giSortButton[2], CURSOR_LAPTOP_SCREEN);
	SpecifyFullButtonTextAttributes( giSortButton[2], pEmailHeaders[SUBJECT_HEADER], EMAIL_WARNING_FONT,
																		FONT_BLACK, FONT_BLACK,
																			FONT_BLACK, FONT_BLACK, TEXT_CJUSTIFIED );
	
	// date sort
	giSortButtonImage[3]=	LoadButtonImage( "LAPTOP\\mailbuttons.sti" ,-1,3,-1,7,-1 );
	giSortButton[3] = QuickCreateButton( giSortButtonImage[3], DATE_BOX_X, FROM_BOX_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)DateCallback);
	SetButtonCursor(giSortButton[3], CURSOR_LAPTOP_SCREEN);
	SpecifyFullButtonTextAttributes( giSortButton[3], pEmailHeaders[RECD_HEADER], EMAIL_WARNING_FONT,
																		FONT_BLACK, FONT_BLACK,
																			FONT_BLACK, FONT_BLACK, TEXT_CJUSTIFIED );
}

void DisplayEmailMessageSubjectDateFromLines( EmailPtr pMail , INT32 iViewerY)
{
	// this procedure will draw the title/headers to From, Subject, Date fields in the display
	// message box
	INT16 usX, usY;
	CHAR16 sString[100];

	// font stuff
	SetFont(MESSAGE_FONT);
	SetFontForeground(FONT_BLACK);
	SetFontBackground(FONT_BLACK);
	SetFontShadow(NO_SHADOW);
	
	// all headers, but not info are right justified

	// print from
	FindFontRightCoordinates( MESSAGE_HEADER_X-20, ( INT16 ) ( MESSAGE_FROM_Y + (INT16)iViewerY ) ,	MESSAGE_HEADER_WIDTH, ( INT16 ) ( MESSAGE_FROM_Y + GetFontHeight ( MESSAGE_FONT ) ) ,pEmailHeaders[0] ,MESSAGE_FONT, &usX, &usY);
	mprintf( usX, MESSAGE_FROM_Y + (UINT16)iViewerY, pEmailHeaders[0]);

	// the actual from info
	if ( pMail->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE || pMail->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP )
	{
		if (gMercProfiles[ pMail->ubSender ].zNickname !='\0')
		mprintf( MESSAGE_HEADER_X+MESSAGE_HEADER_WIDTH-13, MESSAGE_FROM_Y + iViewerY, gMercProfiles[ pMail->ubSender ].zNickname);
		else
		mprintf( MESSAGE_HEADER_X+MESSAGE_HEADER_WIDTH-13, MESSAGE_FROM_Y + iViewerY, L"None");
	}		
	else if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pMail->EmailVersion == TYPE_EMAIL_BOBBY_R || pMail->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_OTHER )
	{
		mprintf( MESSAGE_HEADER_X+MESSAGE_HEADER_WIDTH-13, MESSAGE_FROM_Y + iViewerY, pSenderNameList[pMail->ubSender]);
	}
	else if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC )
	{
		if (gMercProfiles[ pMail->ubSender ].zNickname !='\0')
		mprintf( MESSAGE_HEADER_X+MESSAGE_HEADER_WIDTH-13, MESSAGE_FROM_Y + iViewerY, gMercProfiles[ pMail->ubSender ].zNickname);
		else
		mprintf( MESSAGE_HEADER_X+MESSAGE_HEADER_WIDTH-13, MESSAGE_FROM_Y + iViewerY, L"None");
	}
	
	// print date
	FindFontRightCoordinates( MESSAGE_HEADER_X+168, ( INT16 ) ( MESSAGE_DATE_Y + (UINT16)iViewerY ),	MESSAGE_HEADER_WIDTH, ( INT16 ) ( MESSAGE_DATE_Y + GetFontHeight ( MESSAGE_FONT ) ) ,pEmailHeaders[2] ,MESSAGE_FONT, &usX, &usY);
	mprintf( usX, MESSAGE_DATE_Y+ (UINT16)iViewerY , pEmailHeaders[2]);

	// the actual date info
	swprintf(sString, L"%d", ( ( pMail->iDate ) / ( 24 * 60) ) );
	mprintf( MESSAGE_HEADER_X+235, MESSAGE_DATE_Y + (UINT16)iViewerY, sString);



	// print subject
	FindFontRightCoordinates( MESSAGE_HEADER_X-20, MESSAGE_SUBJECT_Y ,	MESSAGE_HEADER_WIDTH, ( INT16 ) (MESSAGE_SUBJECT_Y + GetFontHeight ( MESSAGE_FONT )),pEmailHeaders[1] ,MESSAGE_FONT, &usX, &usY);
	mprintf( usX, MESSAGE_SUBJECT_Y + (UINT16)iViewerY, pEmailHeaders[1]);

 	// the actual subject info
	//mprintf( , MESSAGE_SUBJECT_Y, pMail->pSubject);
	IanDisplayWrappedString(SUBJECT_LINE_X+2, (INT16) ( SUBJECT_LINE_Y+2 + (UINT16)iViewerY ), SUBJECT_LINE_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR,pMail->pSubject,0,FALSE,0);


	// reset shadow
	SetFontShadow(DEFAULT_SHADOW);
	return;
}


void DrawEmailMessageDisplayTitleText( INT32 iViewerY )
{
	// this procedure will display the title of the email message display box

	// font stuff
	SetFont( EMAIL_HEADER_FONT );
	SetFontForeground( FONT_WHITE );
	SetFontBackground( FONT_BLACK );

	// dsiplay mail viewer title on message viewer
	mprintf( VIEWER_X + 30, VIEWER_Y + 8 + (UINT16) iViewerY, pEmailTitleText[0] );

	return;
}

void DrawLineDividers( void )
{
	// this function draws divider lines between lines of text
	INT32 iCounter=0;
	HVOBJECT hHandle;

	for(iCounter=1; iCounter < 19; iCounter++)
	{
	GetVideoObject( &hHandle, guiMAILDIVIDER );
	BltVideoObject(FRAME_BUFFER, hHandle, 0,INDIC_X-10, (MIDDLE_Y+iCounter*MIDDLE_WIDTH - 1), VO_BLT_SRCTRANSPARENCY,NULL);
	}


	return;
}


void ClearOutEmailMessageRecordsList( void )
{
	RecordPtr pTempRecord;
	INT32 iCounter = 0;

	// runt hrough list freeing records up
	while(pMessageRecordList)
	{
	// set temp to current
	pTempRecord = pMessageRecordList;

		// next element
		pMessageRecordList = pMessageRecordList->Next;

		MemFree( pTempRecord );
	}

	for( iCounter = 0; iCounter < MAX_NUMBER_EMAIL_PAGES; iCounter++ )
	{
		pEmailPageInfo[ iCounter ].pFirstRecord = NULL;
		pEmailPageInfo[ iCounter ].pLastRecord = NULL;
		pEmailPageInfo[ iCounter ].iPageNumber = iCounter;
	}

	// null out list
	pMessageRecordList = NULL;

	return;
}

void AddEmailRecordToList( STR16 pString )
{
	RecordPtr pTempRecord;

	// set to head of list
	pTempRecord=pMessageRecordList;

	if(!pTempRecord)
	{
		// list empty, set this node to head
		pTempRecord = (RecordPtr) MemAlloc( sizeof(Record));
		pMessageRecordList = pTempRecord;
	}
	else
	{
		// run to end of list
		while(pTempRecord->Next)
		{
			pTempRecord = pTempRecord->Next;
		}

		// found, alloc
		pTempRecord->Next = (messagerecord *) MemAlloc( sizeof(Record) );

		// move to node
		pTempRecord = pTempRecord->Next;
	}

	// set next to null
	pTempRecord->Next = NULL;

	// copy in string
	wcscpy( pTempRecord->pRecord, pString );

	// done return
}

void UpDateMessageRecordList( void )
{
	// simply checks to see if old and new message ids are the same, if so, do nothing
	// otherwise clear list

	if( giMessageId != giPrevMessageId )
	{
		// if chenged, clear list
		ClearOutEmailMessageRecordsList( );

		// set prev to current
		giPrevMessageId = giMessageId;
	}
}

void HandleAnySpecialEmailMessageEvents(INT32 iMessageId )
{
	// handles any special message events
#ifdef JA2UB
	switch( iMessageId )
	{
		case EMAIL_MAKECONTACT:
			if( !( gJa25SaveStruct.ubEmailFromSectorFlag & SECTOR_EMAIL__ANOTHER_SECTOR ) )
			{
				AddStrategicEvent( EVENT_SEND_ENRICO_UNDERSTANDING_EMAIL, GetWorldTotalMin() + ( 2 * 60 ) + Random( 120 ), 0 );
			}
			break;

		case( IMP_EMAIL_AGAIN ):
			SetBookMark(IMP_BOOKMARK);
			break;
		case( IMP_EMAIL_INTRO ):
			SetBookMark(IMP_BOOKMARK);
			break;
	}
#else
	// handles any special message events

	switch( iMessageId )
	{

		case( IMP_EMAIL_AGAIN ):
		SetBookMark(IMP_BOOKMARK);
		break;
	case( IMP_EMAIL_INTRO ):
		SetBookMark(IMP_BOOKMARK);
		break;
	}
#endif	
}

void ReDisplayBoxes( void )
{
	// the email message itself
	if(fDisplayMessageFlag)
	{
		// this simply redraws message with button manipulation
		DisplayEmailMessage(GetEmailMessage(giMessageId));
	}

	if(fDeleteMailFlag)
	{
		// delete message, redisplay
		DisplayDeleteNotice(GetEmailMessage(iDeleteId));
	}

	if(fNewMailFlag)
	{
		// if new mail, redisplay box
		DisplayNewMailBox( );
	}
}

#ifdef JA2UB

BOOLEAN HandleNewUBMailSpecialMessages( UINT16 usMessageId, INT32 *iResults, EmailPtr pMail )
{
	BOOLEAN fSpecialCase = FALSE;
	
	wstring wstrMail;
	wstring::size_type index;
	CHAR16 szMail[MAIL_STRING_SIZE];

	if ( pMail->EmailType == TYPE_E_INSURANCE_L3 && pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT && usMessageId == 170 )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			ModifyInsuranceEmails( usMessageId, iResults, pMail, INSUR_PAYMENT_LENGTH );
	}
	else if ( pMail->EmailType == TYPE_E_INSURANCE_L2 && pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT && usMessageId == INSUR_1HOUR_FRAUD )
	{
		if ( gGameUBOptions.LaptopLinkInsurance == TRUE )
			ModifyInsuranceEmails( usMessageId, iResults, pMail, INSUR_1HOUR_FRAUD_LENGTH );
	}
	else if ( pMail->EmailType == TYPE_E_AIM_L1 && pMail->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT && usMessageId == 206 )
	{
		if ( gGameUBOptions.fDeadMerc == TRUE )
			ModifyInsuranceEmails( usMessageId, iResults, pMail, MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH );
	}
	else if ( pMail->EmailType == TYPE_E_AIM_L3 && pMail->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT && usMessageId == 217 )
	{
		if ( gGameUBOptions.fDeadMerc == TRUE )
			ModifyInsuranceEmails( usMessageId, iResults, pMail, AIM_MEDICAL_DEPOSIT_REFUND_LENGTH );
	}
	//Dealtar's Airport Externalization		
	else if ( pMail->EmailType == TYPE_EMAIL_BOBBY_R_L1 && gGameUBOptions.fBobbyRSite == TRUE && pMail->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT )
	{
			if (!pMessageRecordList)
			{
				// WANNE.MAIL: Fix
				gusCurShipmentDestinationID = -1;	// Reset
				gusCurShipmentDestinationID = pMail->iCurrentShipmentDestinationID;

				// Loop through each line of the shipment EDT-file
				for (int i = 0; i < BOBBYR_SHIPMENT_ARRIVED_LENGTH; ++i)
				{
					
					wstrMail.clear();
					LoadEncryptedDataFromFile("BINARYDATA\\Email.edt", szMail, MAIL_STRING_SIZE *usMessageId, MAIL_STRING_SIZE);

					wstrMail = szMail;

					index = wstrMail.find(L"$DESTINATIONNAME$");

					// WANNE.MAIL: Fix
					if (gusCurShipmentDestinationID > 0)
					{
						if (index != wstring::npos)
						{
							wstrMail.erase(index, strlen("$DESTINATIONNAME$"));
							RefToDestinationStruct ds = gPostalService.GetDestination(gusCurShipmentDestinationID);
							wstrMail.insert(index, ds.wstrName.c_str());
						}
					}

					AddEmailRecordToList((STR16)wstrMail.c_str());

					usMessageId++;
				}
			}
			giPrevMessageId = giMessageId;
	}

	return fSpecialCase;
}

#endif

BOOLEAN HandleMailSpecialMessages( UINT16 usMessageId, INT32 *iResults, EmailPtr pMail )
{
	BOOLEAN fSpecialCase = FALSE;

	// this procedure will handle special cases of email messages that are not stored in email.edt, or need special processing
	switch( usMessageId )
	{
		case( IMP_EMAIL_PROFILE_RESULTS ):

			HandleIMPCharProfileResultsMessage( );
			fSpecialCase = TRUE;
			break;
#ifdef JA2UB
//no UB
#else
		case( MERC_INTRO ):
			SetBookMark( MERC_BOOKMARK );
			fReDrawScreenFlag = TRUE;
			break;
		
		case INSUR_PAYMENT:
		case INSUR_SUSPIC:
		case INSUR_SUSPIC_2:
		case INSUR_INVEST_OVER:
		case INSUR_CHEAT_FRAUD:
			ModifyInsuranceEmails( usMessageId, iResults, pMail, INSUR_PAYMENT_LENGTH );
			break;

		case INSUR_1HOUR_FRAUD:
			ModifyInsuranceEmails( usMessageId, iResults, pMail, INSUR_1HOUR_FRAUD_LENGTH );
			break;

		case MERC_NEW_SITE_ADDRESS:
			//Set the book mark so the player can access the site
			SetBookMark( MERC_BOOKMARK );
			break;
					
		case MERC_DIED_ON_OTHER_ASSIGNMENT:
			ModifyInsuranceEmails( usMessageId, iResults, pMail, MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH );
			break;

		case AIM_MEDICAL_DEPOSIT_REFUND:
		case AIM_MEDICAL_DEPOSIT_NO_REFUND:
		case AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND:
			ModifyInsuranceEmails( usMessageId, iResults, pMail, AIM_MEDICAL_DEPOSIT_REFUND_LENGTH );
			break;
		//Dealtar's Airport Externalization
		case BOBBYR_SHIPMENT_ARRIVED:
			if (!pMessageRecordList)
			{
				wstring wstrMail;
				wstring::size_type index;
				CHAR16 szMail[MAIL_STRING_SIZE];

				// WANNE.MAIL: Fix
				gusCurShipmentDestinationID = -1;	// Reset
				gusCurShipmentDestinationID = pMail->iCurrentShipmentDestinationID;

				// Loop through each line of the shipment EDT-file
				for (int i = 0; i < BOBBYR_SHIPMENT_ARRIVED_LENGTH; ++i)
				{
					wstrMail.clear();
					LoadEncryptedDataFromFile("BINARYDATA\\Email.edt", szMail, MAIL_STRING_SIZE *usMessageId, MAIL_STRING_SIZE);

/*			if (FileExists(EMAIL_EDT_FILE_JA25))
			{
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA25, szMail, MAIL_STRING_SIZE *usMessageId, MAIL_STRING_SIZE);
			}
			else
			{
			LoadEncryptedDataFromFile(EMAIL_EDT_FILE_JA2, szMail, MAIL_STRING_SIZE *usMessageId, MAIL_STRING_SIZE);
			}
*/
					wstrMail = szMail;

					index = wstrMail.find(L"$DESTINATIONNAME$");

					// WANNE.MAIL: Fix
					if (gusCurShipmentDestinationID > 0)
					{
						if (index != wstring::npos)
						{
							wstrMail.erase(index, strlen("$DESTINATIONNAME$"));
							RefToDestinationStruct ds = gPostalService.GetDestination(gusCurShipmentDestinationID);
							wstrMail.insert(index, ds.wstrName.c_str());
						}
					}

					AddEmailRecordToList((STR16)wstrMail.c_str());

					++usMessageId;
				}
			}
			giPrevMessageId = giMessageId;
		break;

		case(PMC_INTRO) :
			SetBookMark( PMC_BOOKMARK );
			fReDrawScreenFlag = TRUE;
			break;

		case(MILITIAROSTER_INTRO) :
			SetBookMark( MILITIAROSTER_BOOKMARK );
			fReDrawScreenFlag = TRUE;
			break;

		case INTEL_ENRICO_INTRO:
			SetBookMark( INTELMARKET_BOOKMARK );
			fReDrawScreenFlag = TRUE;
			break;
#endif
	}

	return fSpecialCase;
}
#ifdef JA2UB

#define IMP_RESULTS_INTRO_LENGTH 9

#define IMP_RESULTS_PERSONALITY_INTRO IMP_RESULTS_INTRO_LENGTH	// 0 - 9
#define IMP_RESULTS_PERSONALITY_INTRO_LENGTH 5
#define IMP_PERSONALITY_NORMAL IMP_RESULTS_PERSONALITY_INTRO + IMP_RESULTS_PERSONALITY_INTRO_LENGTH	// 14 - 18
#define IMP_PERSONALITY_LENGTH 4
#define IMP_PERSONALITY_HEAT IMP_PERSONALITY_NORMAL + IMP_PERSONALITY_LENGTH				// 19 - 22
#define IMP_PERSONALITY_NERVOUS IMP_PERSONALITY_HEAT + IMP_PERSONALITY_LENGTH				// 23 - 26
#define IMP_PERSONALITY_CLAUSTROPHOBIC IMP_PERSONALITY_NERVOUS + IMP_PERSONALITY_LENGTH		// 27 - 30
#define IMP_PERSONALITY_NONSWIMMER IMP_PERSONALITY_CLAUSTROPHOBIC + IMP_PERSONALITY_LENGTH	// 31 - 34
#define IMP_PERSONALITY_FEAR_OF_INSECTS IMP_PERSONALITY_NONSWIMMER + IMP_PERSONALITY_LENGTH
#define IMP_PERSONALITY_FORGETFUL IMP_PERSONALITY_FEAR_OF_INSECTS + IMP_PERSONALITY_LENGTH + 1
#define IMP_PERSONALITY_PSYCHO IMP_PERSONALITY_FORGETFUL + IMP_PERSONALITY_LENGTH
#define IMP_RESULTS_ATTITUDE_INTRO IMP_PERSONALITY_PSYCHO + IMP_PERSONALITY_LENGTH + 1
#define IMP_RESULTS_ATTITUDE_LENGTH 5
#define IMP_ATTITUDE_LENGTH 5
#define IMP_ATTITUDE_NORMAL IMP_RESULTS_ATTITUDE_INTRO + IMP_RESULTS_ATTITUDE_LENGTH
#define IMP_ATTITUDE_FRIENDLY IMP_ATTITUDE_NORMAL + IMP_ATTITUDE_LENGTH
#define IMP_ATTITUDE_LONER IMP_ATTITUDE_FRIENDLY + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_OPTIMIST IMP_ATTITUDE_LONER + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_PESSIMIST IMP_ATTITUDE_OPTIMIST + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_AGGRESSIVE IMP_ATTITUDE_PESSIMIST + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_ARROGANT IMP_ATTITUDE_AGGRESSIVE + IMP_ATTITUDE_LENGTH + 1		/// 88
#define IMP_ATTITUDE_ASSHOLE IMP_ATTITUDE_ARROGANT + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_COWARD IMP_ATTITUDE_ASSHOLE + IMP_ATTITUDE_LENGTH
#define IMP_RESULTS_SKILLS IMP_ATTITUDE_COWARD + IMP_ATTITUDE_LENGTH + 1
#define IMP_RESULTS_SKILLS_LENGTH 7
#define IMP_SKILLS_IMPERIAL_SKILLS IMP_RESULTS_SKILLS + IMP_RESULTS_SKILLS_LENGTH + 1
#define IMP_SKILLS_IMPERIAL_MARK IMP_SKILLS_IMPERIAL_SKILLS + 1
#define IMP_SKILLS_IMPERIAL_MECH IMP_SKILLS_IMPERIAL_SKILLS + 2
#define IMP_SKILLS_IMPERIAL_EXPL IMP_SKILLS_IMPERIAL_SKILLS + 3
#define IMP_SKILLS_IMPERIAL_MED	IMP_SKILLS_IMPERIAL_SKILLS + 4

#define IMP_SKILLS_NEED_TRAIN_SKILLS IMP_SKILLS_IMPERIAL_MED + 1
#define IMP_SKILLS_NEED_TRAIN_MARK IMP_SKILLS_NEED_TRAIN_SKILLS + 1		// 119
#define IMP_SKILLS_NEED_TRAIN_MECH IMP_SKILLS_NEED_TRAIN_SKILLS + 2
#define IMP_SKILLS_NEED_TRAIN_EXPL IMP_SKILLS_NEED_TRAIN_SKILLS + 3
#define IMP_SKILLS_NEED_TRAIN_MED IMP_SKILLS_NEED_TRAIN_SKILLS + 4

#define IMP_SKILLS_NO_SKILL IMP_SKILLS_NEED_TRAIN_MED + 1
#define IMP_SKILLS_NO_SKILL_MARK	IMP_SKILLS_NO_SKILL + 1		// 124
#define IMP_SKILLS_NO_SKILL_MECH	IMP_SKILLS_NO_SKILL + 2
#define IMP_SKILLS_NO_SKILL_EXPL	IMP_SKILLS_NO_SKILL + 3
#define IMP_SKILLS_NO_SKILL_MED	IMP_SKILLS_NO_SKILL + 4		// 127

#define IMP_SKILLS_SPECIAL_INTRO IMP_SKILLS_NO_SKILL_MED + 1	// 128, 129
#define IMP_SKILLS_SPECIAL_INTRO_LENGTH 2
#define IMP_SKILLS_SPECIAL_LOCK IMP_SKILLS_SPECIAL_INTRO + IMP_SKILLS_SPECIAL_INTRO_LENGTH		// 130
#define IMP_SKILLS_SPECIAL_HAND IMP_SKILLS_SPECIAL_LOCK + 1
#define IMP_SKILLS_SPECIAL_ELEC IMP_SKILLS_SPECIAL_HAND + 1
#define IMP_SKILLS_SPECIAL_NIGHT IMP_SKILLS_SPECIAL_ELEC + 1
#define IMP_SKILLS_SPECIAL_THROW IMP_SKILLS_SPECIAL_NIGHT + 1
#define IMP_SKILLS_SPECIAL_TEACH IMP_SKILLS_SPECIAL_THROW + 1
#define IMP_SKILLS_SPECIAL_HEAVY IMP_SKILLS_SPECIAL_TEACH + 1
#define IMP_SKILLS_SPECIAL_AUTO IMP_SKILLS_SPECIAL_HEAVY + 1				// 137
#define IMP_SKILLS_SPECIAL_STEALTH IMP_SKILLS_SPECIAL_AUTO + 1
#define IMP_SKILLS_SPECIAL_AMBI IMP_SKILLS_SPECIAL_STEALTH + 1
#define IMP_SKILLS_SPECIAL_THIEF IMP_SKILLS_SPECIAL_AMBI + 1
#define IMP_SKILLS_SPECIAL_MARTIAL IMP_SKILLS_SPECIAL_THIEF + 1
#define IMP_SKILLS_SPECIAL_KNIFE IMP_SKILLS_SPECIAL_MARTIAL + 1				// 142

// WANNE: These 2 skills are missing in the email!
// The problem is, that there is no description in binarydata\impass.edt
#define IMP_SKILLS_SPECIAL_SNIPER IMP_SKILLS_SPECIAL_KNIFE + 1
#define IMP_SKILLS_SPECIAL_CAMOUFLAGED IMP_SKILLS_SPECIAL_SNIPER + 1

#define IMP_RESULTS_PHYSICAL IMP_SKILLS_SPECIAL_KNIFE + 1
#define IMP_RESULTS_PHYSICAL_LENGTH 7

#define IMP_PHYSICAL_SUPER IMP_RESULTS_PHYSICAL + IMP_RESULTS_PHYSICAL_LENGTH
#define IMP_PHYSICAL_SUPER_LENGTH 1

#define IMP_PHYSICAL_SUPER_HEALTH IMP_PHYSICAL_SUPER + IMP_PHYSICAL_SUPER_LENGTH
#define IMP_PHYSICAL_SUPER_AGILITY IMP_PHYSICAL_SUPER_HEALTH + 1
#define IMP_PHYSICAL_SUPER_DEXTERITY IMP_PHYSICAL_SUPER_AGILITY + 1
#define IMP_PHYSICAL_SUPER_STRENGTH IMP_PHYSICAL_SUPER_DEXTERITY + 1
#define IMP_PHYSICAL_SUPER_LEADERSHIP IMP_PHYSICAL_SUPER_STRENGTH + 1
#define IMP_PHYSICAL_SUPER_WISDOM IMP_PHYSICAL_SUPER_LEADERSHIP + 1

#define IMP_PHYSICAL_LOW IMP_PHYSICAL_SUPER_WISDOM + 1
#define IMP_PHYSICAL_LOW_LENGTH 1

#define IMP_PHYSICAL_LOW_HEALTH IMP_PHYSICAL_LOW + IMP_PHYSICAL_LOW_LENGTH
#define IMP_PHYSICAL_LOW_AGILITY IMP_PHYSICAL_LOW_HEALTH + 1
#define IMP_PHYSICAL_LOW_DEXTERITY IMP_PHYSICAL_LOW_AGILITY + 2
#define IMP_PHYSICAL_LOW_STRENGTH IMP_PHYSICAL_LOW_DEXTERITY + 1
#define IMP_PHYSICAL_LOW_LEADERSHIP IMP_PHYSICAL_LOW_STRENGTH + 1
#define IMP_PHYSICAL_LOW_WISDOM IMP_PHYSICAL_LOW_LEADERSHIP + 1


#define IMP_PHYSICAL_VERY_LOW IMP_PHYSICAL_LOW_WISDOM + 1
#define IMP_PHYSICAL_VERY_LOW_LENGTH 1

#define IMP_PHYSICAL_VERY_LOW_HEALTH IMP_PHYSICAL_VERY_LOW + IMP_PHYSICAL_VERY_LOW_LENGTH
#define IMP_PHYSICAL_VERY_LOW_AGILITY IMP_PHYSICAL_VERY_LOW_HEALTH + 1
#define IMP_PHYSICAL_VERY_LOW_DEXTERITY IMP_PHYSICAL_VERY_LOW_AGILITY + 1
#define IMP_PHYSICAL_VERY_LOW_STRENGTH IMP_PHYSICAL_VERY_LOW_DEXTERITY + 1
#define IMP_PHYSICAL_VERY_LOW_LEADERSHIP IMP_PHYSICAL_VERY_LOW_STRENGTH + 1
#define IMP_PHYSICAL_VERY_LOW_WISDOM IMP_PHYSICAL_VERY_LOW_LEADERSHIP + 1


#define IMP_PHYSICAL_END IMP_PHYSICAL_VERY_LOW_WISDOM + 1
#define IMP_PHYSICAL_END_LENGTH 3

#define IMP_RESULTS_PORTRAIT	IMP_PHYSICAL_END + IMP_PHYSICAL_END_LENGTH
#define IMP_RESULTS_PORTRAIT_LENGTH 6


#define IMP_PORTRAIT_MALE_1 IMP_RESULTS_PORTRAIT + IMP_RESULTS_PORTRAIT_LENGTH
#define IMP_PORTRAIT_MALE_2 IMP_PORTRAIT_MALE_1 + 4
#define IMP_PORTRAIT_MALE_3 IMP_PORTRAIT_MALE_2 + 4
#define IMP_PORTRAIT_MALE_4 IMP_PORTRAIT_MALE_3 + 4
#define IMP_PORTRAIT_MALE_5 IMP_PORTRAIT_MALE_4 + 4
#define IMP_PORTRAIT_MALE_6 IMP_PORTRAIT_MALE_5 + 4

#define IMP_PORTRAIT_FEMALE_1 IMP_PORTRAIT_MALE_6 + 4
#define IMP_PORTRAIT_FEMALE_2 IMP_PORTRAIT_FEMALE_1 + 4
#define IMP_PORTRAIT_FEMALE_3 IMP_PORTRAIT_FEMALE_2 + 4
#define IMP_PORTRAIT_FEMALE_4 IMP_PORTRAIT_FEMALE_3 + 4
#define IMP_PORTRAIT_FEMALE_5 IMP_PORTRAIT_FEMALE_4 + 4

#define IMP_RESULTS_END IMP_PORTRAIT_FEMALE_5 + 5
#define IMP_RESULTS_END_LENGTH 3

#else

#define IMP_RESULTS_INTRO_LENGTH 9

#define IMP_RESULTS_PERSONALITY_INTRO IMP_RESULTS_INTRO_LENGTH
#define IMP_RESULTS_PERSONALITY_INTRO_LENGTH 5
#define IMP_PERSONALITY_NORMAL IMP_RESULTS_PERSONALITY_INTRO + IMP_RESULTS_PERSONALITY_INTRO_LENGTH
#define IMP_PERSONALITY_LENGTH 4
#define IMP_PERSONALITY_HEAT IMP_PERSONALITY_NORMAL + IMP_PERSONALITY_LENGTH
#define IMP_PERSONALITY_NERVOUS IMP_PERSONALITY_HEAT + IMP_PERSONALITY_LENGTH
#define IMP_PERSONALITY_CLAUSTROPHOBIC IMP_PERSONALITY_NERVOUS + IMP_PERSONALITY_LENGTH
#define IMP_PERSONALITY_NONSWIMMER IMP_PERSONALITY_CLAUSTROPHOBIC + IMP_PERSONALITY_LENGTH
#define IMP_PERSONALITY_FEAR_OF_INSECTS IMP_PERSONALITY_NONSWIMMER + IMP_PERSONALITY_LENGTH
#define IMP_PERSONALITY_FORGETFUL IMP_PERSONALITY_FEAR_OF_INSECTS + IMP_PERSONALITY_LENGTH + 1
#define IMP_PERSONALITY_PSYCHO IMP_PERSONALITY_FORGETFUL + IMP_PERSONALITY_LENGTH
#define IMP_RESULTS_ATTITUDE_INTRO IMP_PERSONALITY_PSYCHO + IMP_PERSONALITY_LENGTH + 1 
#define IMP_RESULTS_ATTITUDE_LENGTH 5
#define IMP_ATTITUDE_LENGTH 5
#define IMP_ATTITUDE_NORMAL IMP_RESULTS_ATTITUDE_INTRO + IMP_RESULTS_ATTITUDE_LENGTH
#define IMP_ATTITUDE_FRIENDLY IMP_ATTITUDE_NORMAL + IMP_ATTITUDE_LENGTH
#define IMP_ATTITUDE_LONER IMP_ATTITUDE_FRIENDLY + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_OPTIMIST IMP_ATTITUDE_LONER + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_PESSIMIST IMP_ATTITUDE_OPTIMIST + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_AGGRESSIVE IMP_ATTITUDE_PESSIMIST + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_ARROGANT IMP_ATTITUDE_AGGRESSIVE + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_ASSHOLE IMP_ATTITUDE_ARROGANT + IMP_ATTITUDE_LENGTH + 1
#define IMP_ATTITUDE_COWARD IMP_ATTITUDE_ASSHOLE + IMP_ATTITUDE_LENGTH
#define IMP_RESULTS_SKILLS IMP_ATTITUDE_COWARD + IMP_ATTITUDE_LENGTH + 1
#define IMP_RESULTS_SKILLS_LENGTH 7 
#define IMP_SKILLS_IMPERIAL_SKILLS IMP_RESULTS_SKILLS + IMP_RESULTS_SKILLS_LENGTH + 1 
#define IMP_SKILLS_IMPERIAL_MARK IMP_SKILLS_IMPERIAL_SKILLS + 1
#define IMP_SKILLS_IMPERIAL_MECH IMP_SKILLS_IMPERIAL_SKILLS + 2
#define IMP_SKILLS_IMPERIAL_EXPL IMP_SKILLS_IMPERIAL_SKILLS + 3
#define IMP_SKILLS_IMPERIAL_MED  IMP_SKILLS_IMPERIAL_SKILLS + 4

#define IMP_SKILLS_NEED_TRAIN_SKILLS IMP_SKILLS_IMPERIAL_MED + 1
#define IMP_SKILLS_NEED_TRAIN_MARK IMP_SKILLS_NEED_TRAIN_SKILLS + 1
#define IMP_SKILLS_NEED_TRAIN_MECH IMP_SKILLS_NEED_TRAIN_SKILLS + 2
#define IMP_SKILLS_NEED_TRAIN_EXPL IMP_SKILLS_NEED_TRAIN_SKILLS + 3
#define IMP_SKILLS_NEED_TRAIN_MED IMP_SKILLS_NEED_TRAIN_SKILLS + 4

#define IMP_SKILLS_NO_SKILL IMP_SKILLS_NEED_TRAIN_MED + 1
#define IMP_SKILLS_NO_SKILL_MARK  IMP_SKILLS_NO_SKILL + 1
#define IMP_SKILLS_NO_SKILL_MECH  IMP_SKILLS_NO_SKILL + 2
#define IMP_SKILLS_NO_SKILL_EXPL  IMP_SKILLS_NO_SKILL + 3
#define IMP_SKILLS_NO_SKILL_MED   IMP_SKILLS_NO_SKILL + 4

#define IMP_SKILLS_SPECIAL_INTRO IMP_SKILLS_NO_SKILL_MED + 1
#define IMP_SKILLS_SPECIAL_INTRO_LENGTH 2
#define IMP_SKILLS_SPECIAL_LOCK IMP_SKILLS_SPECIAL_INTRO + IMP_SKILLS_SPECIAL_INTRO_LENGTH
#define IMP_SKILLS_SPECIAL_HAND IMP_SKILLS_SPECIAL_LOCK + 1
#define IMP_SKILLS_SPECIAL_ELEC IMP_SKILLS_SPECIAL_HAND + 1
#define IMP_SKILLS_SPECIAL_NIGHT IMP_SKILLS_SPECIAL_ELEC + 1
#define IMP_SKILLS_SPECIAL_THROW IMP_SKILLS_SPECIAL_NIGHT + 1
#define IMP_SKILLS_SPECIAL_TEACH IMP_SKILLS_SPECIAL_THROW + 1
#define IMP_SKILLS_SPECIAL_HEAVY IMP_SKILLS_SPECIAL_TEACH + 1
#define IMP_SKILLS_SPECIAL_AUTO IMP_SKILLS_SPECIAL_HEAVY + 1
#define IMP_SKILLS_SPECIAL_STEALTH IMP_SKILLS_SPECIAL_AUTO + 1
#define IMP_SKILLS_SPECIAL_AMBI IMP_SKILLS_SPECIAL_STEALTH + 1
#define IMP_SKILLS_SPECIAL_THIEF IMP_SKILLS_SPECIAL_AMBI + 1
#define IMP_SKILLS_SPECIAL_MARTIAL IMP_SKILLS_SPECIAL_THIEF + 1
#define IMP_SKILLS_SPECIAL_KNIFE IMP_SKILLS_SPECIAL_MARTIAL + 1

#define IMP_RESULTS_PHYSICAL IMP_SKILLS_SPECIAL_KNIFE + 1
#define IMP_RESULTS_PHYSICAL_LENGTH 7

#define IMP_PHYSICAL_SUPER IMP_RESULTS_PHYSICAL + IMP_RESULTS_PHYSICAL_LENGTH
#define IMP_PHYSICAL_SUPER_LENGTH 1

#define IMP_PHYSICAL_SUPER_HEALTH IMP_PHYSICAL_SUPER + IMP_PHYSICAL_SUPER_LENGTH
#define IMP_PHYSICAL_SUPER_AGILITY IMP_PHYSICAL_SUPER_HEALTH + 1
#define IMP_PHYSICAL_SUPER_DEXTERITY IMP_PHYSICAL_SUPER_AGILITY + 1
#define IMP_PHYSICAL_SUPER_STRENGTH IMP_PHYSICAL_SUPER_DEXTERITY + 1
#define IMP_PHYSICAL_SUPER_LEADERSHIP IMP_PHYSICAL_SUPER_STRENGTH + 1
#define IMP_PHYSICAL_SUPER_WISDOM IMP_PHYSICAL_SUPER_LEADERSHIP + 1
 
#define IMP_PHYSICAL_LOW IMP_PHYSICAL_SUPER_WISDOM + 1
#define IMP_PHYSICAL_LOW_LENGTH 1

#define IMP_PHYSICAL_LOW_HEALTH IMP_PHYSICAL_LOW + IMP_PHYSICAL_LOW_LENGTH
#define IMP_PHYSICAL_LOW_AGILITY IMP_PHYSICAL_LOW_HEALTH + 1
#define IMP_PHYSICAL_LOW_DEXTERITY IMP_PHYSICAL_LOW_AGILITY + 2
#define IMP_PHYSICAL_LOW_STRENGTH IMP_PHYSICAL_LOW_DEXTERITY + 1
#define IMP_PHYSICAL_LOW_LEADERSHIP IMP_PHYSICAL_LOW_STRENGTH + 1
#define IMP_PHYSICAL_LOW_WISDOM IMP_PHYSICAL_LOW_LEADERSHIP + 1


#define IMP_PHYSICAL_VERY_LOW IMP_PHYSICAL_LOW_WISDOM + 1
#define IMP_PHYSICAL_VERY_LOW_LENGTH 1

#define IMP_PHYSICAL_VERY_LOW_HEALTH IMP_PHYSICAL_VERY_LOW + IMP_PHYSICAL_VERY_LOW_LENGTH
#define IMP_PHYSICAL_VERY_LOW_AGILITY IMP_PHYSICAL_VERY_LOW_HEALTH + 1
#define IMP_PHYSICAL_VERY_LOW_DEXTERITY IMP_PHYSICAL_VERY_LOW_AGILITY + 1
#define IMP_PHYSICAL_VERY_LOW_STRENGTH IMP_PHYSICAL_VERY_LOW_DEXTERITY + 1
#define IMP_PHYSICAL_VERY_LOW_LEADERSHIP IMP_PHYSICAL_VERY_LOW_STRENGTH + 1
#define IMP_PHYSICAL_VERY_LOW_WISDOM IMP_PHYSICAL_VERY_LOW_LEADERSHIP + 1


#define IMP_PHYSICAL_END IMP_PHYSICAL_VERY_LOW_WISDOM + 1
#define IMP_PHYSICAL_END_LENGTH 3

#define IMP_RESULTS_PORTRAIT  IMP_PHYSICAL_END + IMP_PHYSICAL_END_LENGTH
#define IMP_RESULTS_PORTRAIT_LENGTH 6


#define IMP_PORTRAIT_MALE_1 IMP_RESULTS_PORTRAIT + IMP_RESULTS_PORTRAIT_LENGTH
#define IMP_PORTRAIT_MALE_2 IMP_PORTRAIT_MALE_1 + 4
#define IMP_PORTRAIT_MALE_3 IMP_PORTRAIT_MALE_2 + 4
#define IMP_PORTRAIT_MALE_4 IMP_PORTRAIT_MALE_3 + 4
#define IMP_PORTRAIT_MALE_5 IMP_PORTRAIT_MALE_4 + 4
#define IMP_PORTRAIT_MALE_6 IMP_PORTRAIT_MALE_5 + 4

#define IMP_PORTRAIT_FEMALE_1 IMP_PORTRAIT_MALE_6 + 4
#define IMP_PORTRAIT_FEMALE_2 IMP_PORTRAIT_FEMALE_1 + 4
#define IMP_PORTRAIT_FEMALE_3 IMP_PORTRAIT_FEMALE_2 + 4
#define IMP_PORTRAIT_FEMALE_4 IMP_PORTRAIT_FEMALE_3 + 4
#define IMP_PORTRAIT_FEMALE_5 IMP_PORTRAIT_FEMALE_4 + 4
#define IMP_PORTRAIT_FEMALE_6 IMP_PORTRAIT_FEMALE_5 + 4
 


#define IMP_RESULTS_END IMP_PORTRAIT_FEMALE_6 + 1
#define IMP_RESULTS_END_LENGTH 3


#endif

void HandleIMPCharProfileResultsMessage( void)
{
	// special case, IMP profile return
	INT32 iHeight=0;
	INT32 iCounter=0;
	CHAR16 pString[MAIL_STRING_SIZE];
	INT32 iOffSet=0;
	RecordPtr pTempRecord;
	INT32 iEndOfSection =0;
	INT32 iRand = 0;
	BOOLEAN fSufficientMechSkill = FALSE, fSufficientMarkSkill = FALSE, fSufficientMedSkill = FALSE, fSufficientExplSkill = FALSE;
	BOOLEAN fSufficientHlth = FALSE, fSufficientStr = FALSE, fSufficientWis = FALSE, fSufficientAgi = FALSE, fSufficientDex = FALSE, fSufficientLdr = FALSE;

	INT16 iCurrentIMPSlot = -1;

	iRand = Random( 32767 );

	// set record ptr to head of list
	pTempRecord=pMessageRecordList;

	// increment height for size of one line
	iHeight+=GetFontHeight( MESSAGE_FONT );

	// load intro
	iEndOfSection = IMP_RESULTS_INTRO_LENGTH;

	// list doesn't exist, reload
	if( !pTempRecord )
	{
		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// have to place players name into string for first record
			if( iCounter == 0)
			{
				CHAR16	zTemp[512];

				iCurrentIMPSlot = giCurrentIMPSlot;

				swprintf( zTemp, L" %s", gMercProfiles[ iCurrentIMPSlot ].zName );
				wcscat( pString, zTemp );
			}

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		// now the personality intro
		iOffSet = IMP_RESULTS_PERSONALITY_INTRO;
		iEndOfSection = IMP_RESULTS_PERSONALITY_INTRO_LENGTH + 1;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		// personality itself
		switch( gMercProfiles[ iCurrentIMPSlot ].bDisability)
		{
			// normal as can be
			case( NO_DISABILITY ):
				iOffSet = IMP_PERSONALITY_NORMAL;
				break;
			case( HEAT_INTOLERANT ):
				iOffSet = IMP_PERSONALITY_HEAT;
				break;
			case( NERVOUS ):
				iOffSet = IMP_PERSONALITY_NERVOUS;
				break;
			case( CLAUSTROPHOBIC ):
				iOffSet = IMP_PERSONALITY_CLAUSTROPHOBIC;
				break;
			case( NONSWIMMER ):
				iOffSet = IMP_PERSONALITY_NONSWIMMER;
				break;
			case( FEAR_OF_INSECTS ):
				iOffSet = IMP_PERSONALITY_FEAR_OF_INSECTS;
				break;
			case( FORGETFUL ):
				iOffSet = IMP_PERSONALITY_FORGETFUL;
				break;
			case( PSYCHO ):
				iOffSet = IMP_PERSONALITY_PSYCHO;
				break;
		}

		// Flugente: new personalities do not get their text from .edt files
		if ( gMercProfiles[ iCurrentIMPSlot ].bDisability == DEAF )
		{
			swprintf( pString, gzIMPDisabilityTraitEmailTextDeaf[0] );

			AddEmailRecordToList( pString );

			swprintf( pString, gzIMPDisabilityTraitEmailTextDeaf[1] );

			AddEmailRecordToList( pString );
		}
		else if ( gMercProfiles[ iCurrentIMPSlot ].bDisability == SHORTSIGHTED )
		{
			swprintf( pString, gzIMPDisabilityTraitEmailTextShortSighted[0] );

			AddEmailRecordToList( pString );

			swprintf( pString, gzIMPDisabilityTraitEmailTextShortSighted[1] );

			AddEmailRecordToList( pString );
		}
		else if ( gMercProfiles[iCurrentIMPSlot].bDisability == HEMOPHILIAC )
		{
			swprintf( pString, gzIMPDisabilityTraitEmailTextHemophiliac[0] );

			AddEmailRecordToList( pString );

			swprintf( pString, gzIMPDisabilityTraitEmailTextHemophiliac[1] );

			AddEmailRecordToList( pString );
		}
		else if ( gMercProfiles[iCurrentIMPSlot].bDisability == AFRAID_OF_HEIGHTS )
		{
			swprintf( pString, gzIMPDisabilityTraitEmailTextAfraidOfHeights[0] );

			AddEmailRecordToList( pString );

			swprintf( pString, gzIMPDisabilityTraitEmailTextAfraidOfHeights[1] );

			AddEmailRecordToList( pString );
		}
		else if ( gMercProfiles[iCurrentIMPSlot].bDisability == SELF_HARM )
		{
			swprintf( pString, gzIMPDisabilityTraitEmailTextSelfHarm[0] );

			AddEmailRecordToList( pString );

			swprintf( pString, gzIMPDisabilityTraitEmailTextSelfHarm[1] );

			AddEmailRecordToList( pString );
		}
		else
		{
			// personality tick
			//	DEF: removed 1/12/99, cause it was changing the length of email that were already calculated
			//LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + Random( IMP_PERSONALITY_LENGTH - 1 ) + 1 ), MAIL_STRING_SIZE );
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + 1 ), MAIL_STRING_SIZE );
			// add to list
			AddEmailRecordToList( pString );

			// personality paragraph
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + IMP_PERSONALITY_LENGTH ), MAIL_STRING_SIZE );
			// add to list
			AddEmailRecordToList( pString );

			// extra paragraph for bugs
			if( gMercProfiles[ iCurrentIMPSlot ].bDisability == FEAR_OF_INSECTS )
			{
				// personality paragraph
				LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + IMP_PERSONALITY_LENGTH + 1 ), MAIL_STRING_SIZE );
				// add to list
				AddEmailRecordToList( pString );
			}
		}

		// attitude intro
		// now the personality intro
		iOffSet = IMP_RESULTS_ATTITUDE_INTRO;
		iEndOfSection = IMP_RESULTS_ATTITUDE_LENGTH;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}
		
		// WANNE: Old trait system: attitudes
		if (!gGameOptions.fNewTraitSystem)
		{
			// personality itself
			switch( gMercProfiles[ iCurrentIMPSlot ].bAttitude )
			{
				// normal as can be
				case( ATT_NORMAL ):
					iOffSet = IMP_ATTITUDE_NORMAL;
					break;
				case( ATT_FRIENDLY ):
					iOffSet = IMP_ATTITUDE_FRIENDLY;
					break;
				case( ATT_LONER ):
					iOffSet = IMP_ATTITUDE_LONER;
					break;
				case( ATT_OPTIMIST ):
					iOffSet = IMP_ATTITUDE_OPTIMIST;
					break;
				case( ATT_PESSIMIST ):
					iOffSet = IMP_ATTITUDE_PESSIMIST;
					break;
				case( ATT_AGGRESSIVE ):
					iOffSet = IMP_ATTITUDE_AGGRESSIVE;
					break;
				case( ATT_ARROGANT ):
					iOffSet = IMP_ATTITUDE_ARROGANT;
					break;
				case( ATT_ASSHOLE ):
					iOffSet = IMP_ATTITUDE_ASSHOLE;
					break;
				case( ATT_COWARD ):
					iOffSet = IMP_ATTITUDE_COWARD;
					break;
			}

			// attitude title
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet ), MAIL_STRING_SIZE );
		}
		// WANNE: New trait system: Character traits
		else
		{
			// WANNE: Try to map "new character traits" to matching old attitudes, so we have meaningful text in IMP mail
			switch( gMercProfiles[ iCurrentIMPSlot ].bCharacterTrait )
			{
				case (CHAR_TRAIT_NORMAL):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_NORMAL]);
					iOffSet = IMP_ATTITUDE_NORMAL;
					break;
				case (CHAR_TRAIT_SOCIABLE):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_SOCIABLE]);
					iOffSet = IMP_ATTITUDE_FRIENDLY;
					break;
				case (CHAR_TRAIT_LONER):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_LONER]);
					iOffSet = IMP_ATTITUDE_LONER;
					break;
				case (CHAR_TRAIT_OPTIMIST):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_OPTIMIST]);
					iOffSet = IMP_ATTITUDE_OPTIMIST;
					break;
				case (CHAR_TRAIT_ASSERTIVE):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_ASSERTIVE]);
					iOffSet = IMP_ATTITUDE_OPTIMIST;
					break;
				case (CHAR_TRAIT_INTELLECTUAL):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_INTELLECTUAL]);
					iOffSet = IMP_ATTITUDE_FRIENDLY;
					break;
				case (CHAR_TRAIT_PRIMITIVE):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_PRIMITIVE]);
					iOffSet = IMP_ATTITUDE_ARROGANT;
					break;
				case (CHAR_TRAIT_AGGRESSIVE):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_AGGRESSIVE]);
					iOffSet = IMP_ATTITUDE_AGGRESSIVE;
					break;
				case (CHAR_TRAIT_PHLEGMATIC):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_PHLEGMATIC]);
					iOffSet = IMP_ATTITUDE_PESSIMIST;
					break;
				case (CHAR_TRAIT_DAUNTLESS):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_DAUNTLESS]);
					iOffSet = IMP_ATTITUDE_AGGRESSIVE;
					break;
				case (CHAR_TRAIT_PACIFIST):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_PACIFIST]);
					iOffSet = IMP_ATTITUDE_COWARD;
					break;
				case (CHAR_TRAIT_MALICIOUS):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_MALICIOUS]);
					iOffSet = IMP_ATTITUDE_ASSHOLE;
					break;
				case (CHAR_TRAIT_SHOWOFF):
					wcscpy(pString, gzIMPCharacterTraitText[CHAR_TRAIT_SHOWOFF]);
					iOffSet = IMP_ATTITUDE_ARROGANT;
					break;
				case CHAR_TRAIT_COWARD:
					wcscpy( pString, gzIMPCharacterTraitText[CHAR_TRAIT_COWARD] );
					iOffSet = IMP_ATTITUDE_COWARD;
					break;
			}

			wcscat(pString, L". ±");
		}
		
		// add to list
		AddEmailRecordToList( pString );

		// attitude tick
		//DEF: removed 1/12/99, cause it was changing the length of email that were already calculated
		//LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + Random( IMP_ATTITUDE_LENGTH - 2 ) + 1 ), MAIL_STRING_SIZE );
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + 1 ), MAIL_STRING_SIZE );
		// add to list
		AddEmailRecordToList( pString );

		// attitude paragraph
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + IMP_ATTITUDE_LENGTH - 1 ), MAIL_STRING_SIZE );
		// add to list
		AddEmailRecordToList( pString );

		//check for second paragraph
		if( iOffSet != IMP_ATTITUDE_NORMAL )
		{
			// attitude paragraph
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + IMP_ATTITUDE_LENGTH ), MAIL_STRING_SIZE );
			// add to list
			AddEmailRecordToList( pString );
		}

		// skills
		// now the skills intro
		iOffSet = IMP_RESULTS_SKILLS;
		iEndOfSection = IMP_RESULTS_SKILLS_LENGTH;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		// imperial skills
		iOffSet = IMP_SKILLS_IMPERIAL_SKILLS;
		iEndOfSection = 0;
		iCounter = 0;

		// marksmanship
		if ( gMercProfiles[ iCurrentIMPSlot ].bMarksmanship >= SUPER_SKILL_VALUE )
		{
			fSufficientMarkSkill = TRUE;
			iEndOfSection = 1;
		}

		// medical
		if ( gMercProfiles[ iCurrentIMPSlot ].bMedical >= SUPER_SKILL_VALUE )
		{
			fSufficientMedSkill = TRUE;
			iEndOfSection = 1;
		}

		// mechanical
		if ( gMercProfiles[ iCurrentIMPSlot ].bMechanical >= SUPER_SKILL_VALUE )
		{
			fSufficientMechSkill = TRUE;
			iEndOfSection = 1;
		}

		if ( gMercProfiles[ iCurrentIMPSlot ].bExplosive >= SUPER_SKILL_VALUE )
		{
			fSufficientExplSkill = TRUE;
			iEndOfSection = 1;
		}

		while (iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		// now handle skills
		if ( fSufficientMarkSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_IMPERIAL_MARK	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if ( fSufficientMedSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_IMPERIAL_MED	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if ( fSufficientMechSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_IMPERIAL_MECH	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		// explosives
		if ( fSufficientExplSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_IMPERIAL_EXPL	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		fSufficientMechSkill = FALSE;
		fSufficientMarkSkill = FALSE;
		fSufficientExplSkill = FALSE;
		fSufficientMedSkill = FALSE;

		// imperial skills
		iOffSet = IMP_SKILLS_NEED_TRAIN_SKILLS;
		iEndOfSection = 0;
		iCounter = 0;

		// now the needs training values
		if( ( gMercProfiles[ iCurrentIMPSlot ].bMarksmanship > NO_CHANCE_IN_HELL_SKILL_VALUE ) &&( gMercProfiles[ iCurrentIMPSlot ].bMarksmanship <= NEEDS_TRAINING_SKILL_VALUE ) )
		{
			fSufficientMarkSkill = TRUE;
			iEndOfSection = 1;
		}

		if( ( gMercProfiles[ iCurrentIMPSlot ].bMedical > NO_CHANCE_IN_HELL_SKILL_VALUE ) &&( gMercProfiles[ iCurrentIMPSlot ].bMedical <= NEEDS_TRAINING_SKILL_VALUE ) )
		{
			fSufficientMedSkill = TRUE;
			iEndOfSection = 1;
		}

		if( ( gMercProfiles[ iCurrentIMPSlot ].bMechanical > NO_CHANCE_IN_HELL_SKILL_VALUE ) &&( gMercProfiles[ iCurrentIMPSlot ].bMechanical <= NEEDS_TRAINING_SKILL_VALUE ) )
		{
			fSufficientMechSkill = TRUE;
			iEndOfSection = 1;
		}

		if( ( gMercProfiles[ iCurrentIMPSlot ].bExplosive > NO_CHANCE_IN_HELL_SKILL_VALUE ) &&( gMercProfiles[ iCurrentIMPSlot ].bExplosive <= NEEDS_TRAINING_SKILL_VALUE ) )
		{
			fSufficientExplSkill = TRUE;
			iEndOfSection = 1;
		}

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		if( fSufficientMarkSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NEED_TRAIN_MARK	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientMedSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NEED_TRAIN_MED	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientMechSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NEED_TRAIN_MECH ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientExplSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NEED_TRAIN_EXPL ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		fSufficientMechSkill = FALSE;
		fSufficientMarkSkill = FALSE;
		fSufficientExplSkill = FALSE;
		fSufficientMedSkill = FALSE;

		// and the no chance in hell of doing anything useful values

		// no skill
		iOffSet = IMP_SKILLS_NO_SKILL;
		iEndOfSection = 0;
		iCounter = 0;

		if( gMercProfiles[ iCurrentIMPSlot ].bMarksmanship <= NO_CHANCE_IN_HELL_SKILL_VALUE )
		{
			fSufficientMarkSkill = TRUE;
			iEndOfSection = 1;
		}

		if( gMercProfiles[ iCurrentIMPSlot ].bMedical <= NO_CHANCE_IN_HELL_SKILL_VALUE )
		{
			fSufficientMedSkill = TRUE;
			iEndOfSection = 1;
		}

		if( gMercProfiles[ iCurrentIMPSlot ].bMechanical <= NO_CHANCE_IN_HELL_SKILL_VALUE )
		{
			fSufficientMechSkill = TRUE;
			iEndOfSection = 1;
		}

		if( gMercProfiles[ iCurrentIMPSlot ].bExplosive <= NO_CHANCE_IN_HELL_SKILL_VALUE )
		{
			fSufficientExplSkill = TRUE;
			iEndOfSection = 1;
		}

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		if( fSufficientMechSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NO_SKILL_MECH ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientMarkSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NO_SKILL_MARK ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientMedSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NO_SKILL_MED ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}
		if( fSufficientExplSkill )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_NO_SKILL_EXPL ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		// now the specialized skills
		// imperial skills
		iOffSet = IMP_SKILLS_SPECIAL_INTRO;
		iEndOfSection = IMP_SKILLS_SPECIAL_INTRO_LENGTH;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

	///////////////////////////////////////////////////////////////////////////////
	// SANDRO - switch for old/new traits
	if ( gGameOptions.fNewTraitSystem )
	{
		// Auto Weapons
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, AUTO_WEAPONS_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_AUTO ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Heavy Weapons
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, HEAVY_WEAPONS_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_HEAVY ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Sniper
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, SNIPER_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[0]);
			AddEmailRecordToList( pString );
		}
		// Ranger
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, RANGER_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[2]);
			AddEmailRecordToList( pString );
		}
		// Gunslinger
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, GUNSLINGER_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[3]);
			AddEmailRecordToList( pString );
		}
		// Martial Artist
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, MARTIAL_ARTS_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_MARTIAL ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Squadleader
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, SQUADLEADER_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[4]);
			AddEmailRecordToList( pString );
		}
		// Technician
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, TECHNICIAN_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[5]);
			AddEmailRecordToList( pString );
		}
		// Doctor
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, DOCTOR_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[6]);
			AddEmailRecordToList( pString );
		}
		// Ambidextrous
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, AMBIDEXTROUS_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_AMBI ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Melee
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, MELEE_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_KNIFE ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Throwing
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, THROWING_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_THROW ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Night Ops
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, NIGHT_OPS_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_NIGHT ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Stealthy
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, STEALTHY_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_STEALTH ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Athletics
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, ATHLETICS_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[7]);
			AddEmailRecordToList( pString );
		}
		// Bodybuilding
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, BODYBUILDING_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[8]);
			AddEmailRecordToList( pString );
		}
		// Demolitions
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, DEMOLITIONS_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[9]);
			AddEmailRecordToList( pString );
		}
		// Teaching
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, TEACHING_NT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_TEACH ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Scouting
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, SCOUTING_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[10]);
			AddEmailRecordToList( pString );
		}
		// Covert ops
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, COVERT_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[11]);
			AddEmailRecordToList( pString );
		}
		// Radio Operator
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, RADIO_OPERATOR_NT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[12]);
			AddEmailRecordToList( pString );
		}
		// Survival
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, SURVIVAL_NT ) > 0 )
		{
			wcscpy( pString, MissingIMPSkillsDescriptions[13] );
			AddEmailRecordToList( pString );
		}
	}
	else
	{
		// Lockpick
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, LOCKPICKING_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_LOCK ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Hand to Hand
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, HANDTOHAND_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_HAND ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Electronics
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, ELECTRONICS_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_ELEC ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Night Ops
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, NIGHTOPS_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_NIGHT ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Throwing
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, THROWING_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_THROW ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Teaching
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, TEACHING_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_TEACH ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Heavy Weapons
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, HEAVY_WEAPS_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_HEAVY ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Auto Weapons
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, AUTO_WEAPS_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_AUTO ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Stealthy
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, STEALTHY_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_STEALTH ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Ambidextrous
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, AMBIDEXT_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_AMBI ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Thief
		/*if( ( gMercProfiles[ iCurrentIMPSlot ].bSkillTrait == THIEF_OT )||( gMercProfiles[ iCurrentIMPSlot ].bSkillTrait2 == THIEF_OT ) )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_THIEF ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}*/
		// Martial Arts
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, MARTIALARTS_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_MARTIAL ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Knifing
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, KNIFING_OT ) > 0 )
		{
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_SKILLS_SPECIAL_KNIFE ), MAIL_STRING_SIZE );
			AddEmailRecordToList( pString );
		}
		// Sniper
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, PROF_SNIPER_OT ) > 0 )
		{
			wcscpy(pString, MissingIMPSkillsDescriptions[0]);
			// add to list
			AddEmailRecordToList( pString );
		}
		// Camouflage
		if ( ProfileHasSkillTrait( iCurrentIMPSlot, CAMOUFLAGED_OT ) > 0 )
		{
			if ( gGameExternalOptions.fShowCamouflageFaces == TRUE )
			{
				gCamoFace[iCurrentIMPSlot].gCamoface = TRUE;
				gCamoFace[iCurrentIMPSlot].gUrbanCamoface = FALSE;
				gCamoFace[iCurrentIMPSlot].gDesertCamoface = FALSE;
				gCamoFace[iCurrentIMPSlot].gSnowCamoface = FALSE;
			}	
			wcscpy(pString, MissingIMPSkillsDescriptions[1]);
			AddEmailRecordToList( pString );
		}
	}

	///////////////////////////////////////////////////////////////////////////////

	// now the physical
	// imperial physical
	iOffSet = IMP_RESULTS_PHYSICAL;
	iEndOfSection = IMP_RESULTS_PHYSICAL_LENGTH;
	iCounter = 0;

	while(iEndOfSection > iCounter)
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );

		// increment email record counter
		iCounter++;
	}

	// super physical
	iOffSet = IMP_PHYSICAL_SUPER;
	iEndOfSection = 0;
	iCounter = 0;

	// health
	if(	gMercProfiles[ iCurrentIMPSlot ].bLife >= SUPER_STAT_VALUE )
	{
		fSufficientHlth = TRUE;
		iEndOfSection = 1;
	}

	// dex
	if( gMercProfiles[ iCurrentIMPSlot ].bDexterity >= SUPER_STAT_VALUE )
	{
		fSufficientDex = TRUE;
		iEndOfSection = 1;
	}

	// agility
	if( gMercProfiles[ iCurrentIMPSlot ].bAgility >= SUPER_STAT_VALUE )
	{
		fSufficientAgi	= TRUE;
		iEndOfSection = 1;
	}

	// strength
	if( gMercProfiles[ iCurrentIMPSlot ].bStrength >= SUPER_STAT_VALUE )
	{
		fSufficientStr = TRUE;
		iEndOfSection = 1;
	}

	// wisdom
	if( gMercProfiles[ iCurrentIMPSlot ].bWisdom >= SUPER_STAT_VALUE )
	{
		fSufficientWis = TRUE;
		iEndOfSection =1;
	}

	// leadership
	if( gMercProfiles[ iCurrentIMPSlot ].bLeadership >= SUPER_STAT_VALUE )
	{
		fSufficientLdr = TRUE;
		iEndOfSection = 1;
	}

	while(iEndOfSection > iCounter)
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );

		// increment email record counter
		iCounter++;
	}

	if( fSufficientHlth )
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_SUPER_HEALTH	), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );
	}


	if( fSufficientDex )
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_SUPER_DEXTERITY	), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );
	}

	if( fSufficientStr )
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_SUPER_STRENGTH	), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );
	}

	if( fSufficientAgi )
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_SUPER_AGILITY	), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );
	}

	if( fSufficientWis )
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_SUPER_WISDOM	), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );
	}

	if( fSufficientLdr )
	{
		// read one record from email file
		LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_SUPER_LEADERSHIP	), MAIL_STRING_SIZE );

		// add to list
		AddEmailRecordToList( pString );
	}

	fSufficientHlth = FALSE;
	fSufficientStr = FALSE;
	fSufficientWis = FALSE;
	fSufficientAgi = FALSE;
	fSufficientDex = FALSE;
	fSufficientLdr = FALSE;

	// now the low attributes
	// super physical
	iOffSet = IMP_PHYSICAL_LOW;
	iEndOfSection = 0;
	iCounter = 0;

		// health
		if(	( gMercProfiles[ iCurrentIMPSlot ].bLife < NEEDS_TRAINING_STAT_VALUE ) &&( gMercProfiles[ iCurrentIMPSlot ].bLife > NO_CHANCE_IN_HELL_STAT_VALUE ) )
		{
			fSufficientHlth = TRUE;
			iEndOfSection = 1;
		}

		// strength
		if( (gMercProfiles[ iCurrentIMPSlot ].bStrength < NEEDS_TRAINING_STAT_VALUE )&&( gMercProfiles[ iCurrentIMPSlot ].bStrength > NO_CHANCE_IN_HELL_STAT_VALUE ) )
		{
			fSufficientStr = TRUE;
			iEndOfSection = 1;
		}

		// agility
		if( (gMercProfiles[ iCurrentIMPSlot ].bAgility < NEEDS_TRAINING_STAT_VALUE )&&( gMercProfiles[ iCurrentIMPSlot ].bAgility <= NO_CHANCE_IN_HELL_STAT_VALUE ) )
		{
			fSufficientAgi = TRUE;
			iEndOfSection = 1;
		}

		// wisdom
		if( (gMercProfiles[ iCurrentIMPSlot ].bWisdom < NEEDS_TRAINING_STAT_VALUE)&&( gMercProfiles[ iCurrentIMPSlot ].bWisdom > NO_CHANCE_IN_HELL_STAT_VALUE ) )
		{
			fSufficientWis = TRUE;
			iEndOfSection = 1;
		}

		// leadership
		if( (gMercProfiles[ iCurrentIMPSlot ].bLeadership < NEEDS_TRAINING_STAT_VALUE)&&( gMercProfiles[ iCurrentIMPSlot ].bLeadership > NO_CHANCE_IN_HELL_STAT_VALUE ) )
		{
			fSufficientLdr = TRUE;
			iEndOfSection = 1;
		}

		// dex
		if( (gMercProfiles[ iCurrentIMPSlot ].bDexterity < NEEDS_TRAINING_STAT_VALUE )&&( gMercProfiles[ iCurrentIMPSlot ].bDexterity > NO_CHANCE_IN_HELL_STAT_VALUE ) )
		{
			fSufficientDex = TRUE;
			iEndOfSection = 1;
		}

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		if( fSufficientHlth )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_LOW_HEALTH	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientDex )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_LOW_DEXTERITY	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientStr )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_LOW_STRENGTH	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientAgi )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_LOW_AGILITY	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientWis )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_LOW_WISDOM	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientLdr )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_LOW_LEADERSHIP	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		// very low physical
		iOffSet = IMP_PHYSICAL_VERY_LOW;
		iEndOfSection = 0;
		iCounter = 0;

		fSufficientHlth = FALSE;
		fSufficientStr = FALSE;
		fSufficientWis = FALSE;
		fSufficientAgi = FALSE;
		fSufficientDex = FALSE;
		fSufficientLdr = FALSE;

		// health
		if(	gMercProfiles[ iCurrentIMPSlot ].bLife <= NO_CHANCE_IN_HELL_STAT_VALUE )
		{
			fSufficientHlth = TRUE;
			iEndOfSection =1;
		}

		// dex
		if( gMercProfiles[ iCurrentIMPSlot ].bDexterity <= NO_CHANCE_IN_HELL_STAT_VALUE )
		{
			fSufficientDex = TRUE;
			iEndOfSection =1;
		}

		// strength
		if( gMercProfiles[ iCurrentIMPSlot ].bStrength <= NO_CHANCE_IN_HELL_STAT_VALUE )
		{
			fSufficientStr = TRUE;
			iEndOfSection = 1;
		}

		// agility
		if( gMercProfiles[ iCurrentIMPSlot ].bAgility <= NO_CHANCE_IN_HELL_STAT_VALUE )
		{
			fSufficientAgi = TRUE;
			iEndOfSection = 1;
		}

		// wisdom
		if( gMercProfiles[ iCurrentIMPSlot ].bWisdom <= NO_CHANCE_IN_HELL_STAT_VALUE )
		{
			fSufficientWis = TRUE;
			iEndOfSection =1;
		}

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		if( fSufficientHlth )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_VERY_LOW_HEALTH	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientDex )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_VERY_LOW_DEXTERITY	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientStr )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_VERY_LOW_STRENGTH	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientAgi )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_VERY_LOW_AGILITY	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		if( fSufficientWis )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_VERY_LOW_WISDOM	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		// leadership
		if( gMercProfiles[ iCurrentIMPSlot ].bLeadership <= NO_CHANCE_IN_HELL_STAT_VALUE )
		{
			fSufficientLdr = TRUE;
		}

		if( fSufficientLdr )
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( IMP_PHYSICAL_VERY_LOW_LEADERSHIP	), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );
		}

		// very low physical
		iOffSet = IMP_RESULTS_PORTRAIT;
		iEndOfSection = IMP_RESULTS_PORTRAIT_LENGTH;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		// portraits
		switch( iPortraitNumber )
		{
			case( 0 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_1;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_1;
				break;
			case( 1 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_2;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_2;
				break;
			case( 2 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_3;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_3;
				break;
			case( 3 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_4;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_4;
				break;
			case( 4 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_5;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_4;
				break;
			case( 5 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_5;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_5;
				break;
			case( 6 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_6;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_5;
				break;
			case( 7 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_6;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_3;
				break;
			case( 8 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_4;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_4;
				break;
			case( 9 ):
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_5;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_5;
				break;
			default:
				if( fCharacterIsMale )
					iOffSet = IMP_PORTRAIT_MALE_1;
				else
					iOffSet = IMP_PORTRAIT_FEMALE_1;
				break;
		}

		if( ( iRand % 2 ) == 0 )
		{
			iOffSet += 2;
		}

		iEndOfSection = 2;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		iOffSet = IMP_RESULTS_END;
		iEndOfSection = IMP_RESULTS_END_LENGTH;
		iCounter = 0;

		while(iEndOfSection > iCounter)
		{
			// read one record from email file
			LoadEncryptedDataFromFile( "BINARYDATA\\Impass.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );

			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			iCounter++;
		}

		giPrevMessageId = giMessageId;

	}

	pTempRecord = pMessageRecordList;

}

void HandleEmailViewerButtonStates( void )
{
	// handle state of email viewer buttons

	if( fDisplayMessageFlag == FALSE )
	{
		// not displaying message, leave
		return;
	}



	if(	giNumberOfPagesToCurrentEmail <= 2 )
	{
		return;
	}

	// turn off previous page button
	if( giMessagePage == 0 )
	{
		DisableButton( giMailMessageButtons[ 0 ] );
	}
	else
	{
		EnableButton( giMailMessageButtons[ 0 ] );
	}


	// turn off next page button
	if( pEmailPageInfo[ giMessagePage + 1 ].pFirstRecord == NULL )
	{
		DisableButton( giMailMessageButtons[ 1 ] );
	}
	else
	{
		EnableButton( giMailMessageButtons[ 1 ] );
	}

	return;

}


void SetUpIconForButton( )
{
	// if we just got in, return, don't set any

	if( fJustStartedEmail == TRUE )
	{
		return;
	}




	return;
}


void DeleteCurrentMessage( void )
{
	// will delete the currently displayed message

	// set current message to be deleted
	iDeleteId = giMessageId;

	// set the currently displayed message to none
	giMessageId = -1;

	// reset display message flag
	fDisplayMessageFlag=FALSE;

	// reset page being displayed
	giMessagePage = 0;

	fDeleteInternal = TRUE;

	// delete message
	DeleteEmail( );

	fDeleteInternal = FALSE;

	// force update of entire screen
	fReDrawScreenFlag=TRUE;

	// rerender email
	RenderEmail();

	return;
}


void CreateNextPreviousEmailPageButtons( void )
{

	// this function will create the buttons to advance and go back email pages

	// next button
	giMailPageButtonsImage[0]=	LoadButtonImage( "LAPTOP\\NewMailButtons.sti" ,-1,1,-1,4,-1 );
	giMailPageButtons[0] = QuickCreateButton( giMailPageButtonsImage[0],NEXT_PAGE_X, NEXT_PAGE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
										(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)NextRegionButtonCallback );
	SetButtonCursor(giMailPageButtons[0], CURSOR_LAPTOP_SCREEN);

	// previous button
	giMailPageButtonsImage[1]=	LoadButtonImage( "LAPTOP\\NewMailButtons.sti" ,-1,0,-1,3,-1 );
	giMailPageButtons[1] = QuickCreateButton( giMailPageButtonsImage[1],PREVIOUS_PAGE_X, NEXT_PAGE_Y,
									BUTTON_TOGGLE, MSYS_PRIORITY_HIGHEST - 1,
									(GUI_CALLBACK)BtnGenericMouseMoveButtonCallback, (GUI_CALLBACK)PreviousRegionButtonCallback );
	SetButtonCursor(giMailPageButtons[1], CURSOR_LAPTOP_SCREEN);

	/*
	// set up disable methods
	SpecifyDisabledButtonStyle( giMailPageButtons[1], DISABLED_STYLE_SHADED );
	SpecifyDisabledButtonStyle( giMailPageButtons[0], DISABLED_STYLE_SHADED );
*/

	return;
}


void UpdateStatusOfNextPreviousButtons( void )
{

	// set the states of the page advance buttons

	DisableButton( giMailPageButtons[ 0 ]);
	DisableButton( giMailPageButtons[ 1 ]);

	if( iCurrentPage > 0 )
	{
		EnableButton( giMailPageButtons[ 1 ]);
	}

	if( iCurrentPage < iLastPage )
	{
		EnableButton( giMailPageButtons[ 0 ] );
	}
}


void DisplayWhichPageOfEmailProgramIsDisplayed( void )
{
	// will draw the number of the email program we are viewing right now
	CHAR16 sString[ 10 ];

	// font stuff
	SetFont(MESSAGE_FONT);
	SetFontForeground(FONT_BLACK);
	SetFontBackground(FONT_BLACK);
	SetFontShadow(NO_SHADOW);

	// page number
	if( iLastPage < 0 )
		swprintf( sString, L"%d / %d", 1, 1);
	else
		swprintf( sString, L"%d / %d", iCurrentPage + 1, iLastPage + 1);

	// print it
	mprintf( PAGE_NUMBER_X ,PAGE_NUMBER_Y, sString );

	// restore shadow
	SetFontShadow( DEFAULT_SHADOW );

	return;
}

void OpenMostRecentUnreadEmail( void )
{
	// will open the most recent email the player has recieved and not read
	INT32 iMostRecentMailId = -1;
	EmailPtr pB=pEmailList;
	UINT32 iLowestDate = 9999999;

	while( pB )
	{
		// if date is lesser and unread , swap
		if( ( pB->iDate < iLowestDate )&&( pB->fRead == FALSE ) )
		{
			iMostRecentMailId = pB->iId;
			iLowestDate = pB->iDate;
		}

		// next in B's list
	pB=pB->Next;
	}

	// set up id
	giMessageId = iMostRecentMailId;

	// valid message, show it
	if( giMessageId != -1 )
	{
		fDisplayMessageFlag = TRUE;
	}

	return;
}


BOOLEAN DisplayNumberOfPagesToThisEmail( INT32 iViewerY )
{
	// display the indent for the display of pages to this email..along with the current page/number of pages
	INT16 sX = 0, sY = 0;
	CHAR16 sString[ 32 ];


	// get and blt the email list background
	// load, blt and delete graphics
	//VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
//	FilenameForBPP( "LAPTOP\\mailindent.sti", VObjectDesc.ImageFile );
	//CHECKF( AddVideoObject( &VObjectDesc, &uiMailIndent ) );
 // GetVideoObject( &hHandle, uiMailIndent );
 // BltVideoObject( FRAME_BUFFER, hHandle, 0,VIEWER_X + INDENT_X_OFFSET, VIEWER_Y + iViewerY + INDENT_Y_OFFSET - 10, VO_BLT_SRCTRANSPARENCY,NULL );
 // DeleteVideoObjectFromIndex( uiMailIndent );

	// parse current page and max number of pages to email
	swprintf( sString,L"%d / %d", ( giMessagePage + 1 ), ( giNumberOfPagesToCurrentEmail - 1 ) );

	SetFont( FONT12ARIAL );
	SetFontForeground( FONT_BLACK );
	SetFontBackground( FONT_BLACK );

	// turn off the shadows
	SetFontShadow(NO_SHADOW);

	SetFontDestBuffer(FRAME_BUFFER, 0 , 0 , SCREEN_WIDTH, SCREEN_HEIGHT,	FALSE	);

	FindFontCenterCoordinates(VIEWER_X + INDENT_X_OFFSET, 0,INDENT_X_WIDTH, 0, sString, FONT12ARIAL, &sX, &sY);
	mprintf( sX, VIEWER_Y + iViewerY + INDENT_Y_OFFSET - 2, sString );


	// restore shadows
	SetFontShadow( DEFAULT_SHADOW );

	return ( TRUE );
}


INT32 GetNumberOfPagesToEmail( )
{
	RecordPtr pTempRecord;
	INT32 iNumberOfPagesToEmail = 0;


	// set temp record to head of list
	pTempRecord=pMessageRecordList;

	// run through messages, and find out how many
	while( pTempRecord )
	{
		pTempRecord = GetFirstRecordOnThisPage( pMessageRecordList, MESSAGE_FONT, MESSAGE_WIDTH, MESSAGE_GAP, iNumberOfPagesToEmail, MAX_EMAIL_MESSAGE_PAGE_SIZE );
		iNumberOfPagesToEmail++;
	}


	return( iNumberOfPagesToEmail );
}


void ShutDownEmailList()
{
UINT32  cnt;
	
	EmailPtr pEmail = pEmailList;
	EmailPtr pTempEmail = NULL;
	
	for( cnt=0; cnt<EMAIL_VAL; cnt++)
	{
		gEmailT[cnt].EmailType = 0;
	}

	//loop through all the emails to delete them
	while( pEmail )
	{
		pTempEmail = pEmail;

		pEmail = pEmail->Next;

		MemFree( pTempEmail->pSubject );
		pTempEmail->pSubject = NULL;

		MemFree( pTempEmail );
		pTempEmail = NULL;
	}
	pEmailList = NULL;

	ClearPages();
}

// Pre Process the mail, when clicking on a mail in the mail list
void PreProcessEmail( EmailPtr pMail )
{
	RecordPtr pTempRecord, pCurrentRecord, pLastRecord , pTempList;
	CHAR16 pString[ 512 ];
	INT32 iCounter = 0, iHeight = 0, iOffSet = 0;
	BOOLEAN fGoingOffCurrentPage = FALSE;
	INT32 iYPositionOnPage = 0;

	iOffSet=(INT32)pMail->usOffset;

	// set record ptr to head of list
	pTempRecord=pMessageRecordList;

	if( pEmailPageInfo[ 0 ].pFirstRecord != NULL )
	{
		// already processed
		return;
	}

	// WANNE: Get the text and replace name!
	int iNew113MERCMerc = 0;
	int iNew113AIMMerc = 0;

	int iEmailMERCMessage = 0;
	int iEmailAIMMessage = 0;
	int iEmailOther = 0;
	int iEmailBobbyRMessage = 0;
	int iEmailInsurance = 0;
	int iNew113CustomUserMerc = 0;	
	
	if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT )
	{	
		if (pMail->usLength == MERC_UP_LEVEL_GASTON || pMail->usLength == MERC_UP_LEVEL_STOGIE ||
			pMail->usLength == MERC_UP_LEVEL_TEX || pMail->usLength == MERC_UP_LEVEL_BIGGENS)
		{
			iNew113MERCMerc = pMail->usLength;
			pMail->usLength = 2;
		}
		else if (pMail->usLength >= 170 && pMail->usLength <= 177)
		{
			iNew113AIMMerc = pMail->usLength;
			pMail->usLength = 2;
		}
		// User made merc
		else if (pMail->usLength >= 178 && pMail->usLength <= 254)
		{
			iNew113CustomUserMerc = pMail->usLength;
			pMail->usLength = 2;
		}
	}
	else if ( pMail->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP )
	{
		iEmailMERCMessage = pMail->usLength;
		pMail->usLength = 2;	
	}	
	else if ( pMail->EmailVersion == TYPE_EMAIL_BOBBY_R )
	{
		iEmailBobbyRMessage = pMail->usLength;
		pMail->usLength = 4;	
	}	
	else if ( pMail->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE )
	{
		iEmailAIMMessage = pMail->usLength;
		pMail->usLength = 2;	
	}	
	else if ( pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY )
	{
		iEmailInsurance = pMail->usLength;
		pMail->usLength = 2;	
	}	
	else if ( pMail->EmailVersion == TYPE_EMAIL_OTHER )	
	{	
		iEmailOther = pMail->usLength;
		//pMail->usLength = 6;
	}

	// list doesn't exist, reload
	if( !pTempRecord )
	{
		while(pMail->usLength > iCounter)
		{
			// read one record from email file
			// read one record from email file
#ifdef JA2UB
			//if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC )
			//{
				if (FileExists(EMAIL_EDT_FILE_JA25) )
				{
					if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC )
					LoadEncryptedDataFromFile( EMAIL_EDT_FILE_JA25, pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );
				}
				else
				{
					if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC )
						LoadEncryptedDataFromFile( EMAIL_EDT_FILE_JA2, pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );
				}
			//}
			if ( pMail->EmailVersion == TYPE_EMAIL_BOBBY_R_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
			{
				LoadEncryptedDataFromFile( EMAIL_EDT_FILE_JA2, pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );	
			}
#else
			if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT || pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC )
				LoadEncryptedDataFromFile( "BINARYDATA\\Email.edt", pString, MAIL_STRING_SIZE * ( iOffSet + iCounter ), MAIL_STRING_SIZE );
#endif
			
			else if ( pMail->EmailVersion == TYPE_EMAIL_OTHER )	
			{
				//if ( iCounter != 29 )
				wcscpy(pString, L"\0");
				wcscat( pString, EmailOtherText[iEmailOther].szMessage[pMail->usLength]);	
				//	wcscat( pString, EmailOtherText[0].szMessage[1]);	
			}

			// ----------------
			// New MERC Merc
			// ----------------
			// WANNE: We have a new 1.13 MERC merc (Text, Gaston, Stogie or Biggens)
			if (iNew113MERCMerc != 0 )
			{
				// WANNE: TODO: Replace "Biff" with the name of the 1.13 merc
				if (iCounter == 1 && pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT )
				{
					wcscpy(pString, L"\0");
					if (iNew113MERCMerc == MERC_UP_LEVEL_GASTON)
					{
						wcscpy( pString, New113MERCMercMailTexts[0] );						
					}
					else if (iNew113MERCMerc == MERC_UP_LEVEL_STOGIE)
					{
						wcscpy( pString, New113MERCMercMailTexts[1] );
					}
					else if (iNew113MERCMerc == MERC_UP_LEVEL_TEX)
					{
						wcscpy( pString, New113MERCMercMailTexts[2] );
					}
					else if (iNew113MERCMerc == MERC_UP_LEVEL_BIGGENS)
					{
						wcscpy( pString, New113MERCMercMailTexts[3] );
					}
				}
			}

			// ----------------
			// New AIM Merc
			// ----------------
			// WANNE: We have a new 1.13 AIM Wildfire merc
			if (iNew113AIMMerc != 0)
			{				
				wcscpy(pString, L"\0");

				// Only output the mail text, not the subject, cause we already have the subject as text
				if (iCounter == 1 && pMail->EmailVersion ==  TYPE_EMAIL_EMAIL_EDT )
				{
					if (iNew113AIMMerc == 170)
					{
						wcscpy( pString, New113AIMMercMailTexts[1] );						
					}
					else if (iNew113AIMMerc == 171)
					{
						wcscpy( pString, New113AIMMercMailTexts[3] );
					}
					else if (iNew113AIMMerc == 172)
					{
						wcscpy( pString, New113AIMMercMailTexts[5] );
					}
					else if (iNew113AIMMerc == 173)
					{
						wcscpy( pString, New113AIMMercMailTexts[7] );
					}
					else if (iNew113AIMMerc == 174)
					{
						wcscpy( pString, New113AIMMercMailTexts[9] );
					}
					else if (iNew113AIMMerc == 175)
					{
						wcscpy( pString, New113AIMMercMailTexts[11] );
					}
					else if (iNew113AIMMerc == 176)
					{
						wcscpy( pString, New113AIMMercMailTexts[13] );
					}
					else if (iNew113AIMMerc == 177)
					{
						wcscpy( pString, New113AIMMercMailTexts[15] );
					}
					// Additional Generic Merc mail message
					else
					{
						wcscpy( pString, New113AIMMercMailTexts[17] );
					}
				}
			}

			// --------------------------
			// New Customer User Merc
			// --------------------------
			// WANNE: We have a new 1.13 AIM Wildfire merc
			if (iNew113CustomUserMerc != 0 && pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC)
			{	
				wcscpy(pString, L"\0");

				// Only output the mail text, not the subject, cause we already have the subject as text
				if (iCounter == 1)
				{
					wcscpy( pString, New113AIMMercMailTexts[17] );
				}
			}

			if (iCounter == 1)
			{
				if ( pMail->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE )
				{
					wcscpy(pString, L"\0");
					wcscpy( pString, EmailMercAvailableText[iEmailAIMMessage].szMessage);
				}
				else if ( pMail->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP)
				{
					wcscpy(pString, L"\0");
					wcscpy( pString, EmailMercLevelUpText[iEmailMERCMessage].szMessage);				
				}
				else if ( pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY )
				{
					wcscpy(pString, L"\0");
					wcscpy( pString, EmailInsuranceText[iEmailMERCMessage].szMessage);		
				}
				/*
				else if ( pMail->EmailVersion == TYPE_EMAIL_BOBBY_R)
				{
					wcscpy(pString, L"\0");
					wcscpy( pString, EmailBobbyRText[0]);				
				}
				*/
			}
		
			// add to list
			AddEmailRecordToList( pString );

			// increment email record counter
			++iCounter;
		}

		if ( pMail->EmailVersion == TYPE_EMAIL_EMAIL_EDT )
		{
			// WANNE: Set the value back
			if (iNew113MERCMerc != 0)
			{
				pMail->usLength = iNew113MERCMerc;
			}

			// WANNE: Set the value back
			if (iNew113AIMMerc != 0)
			{
				pMail->usLength = iNew113AIMMerc;
			}

			// WANNE: Set the value back
			if (iNew113CustomUserMerc != 0)
			{
				pMail->usLength = iNew113CustomUserMerc;
			}
		}
		else if ( pMail->EmailVersion == TYPE_EMAIL_BOBBY_R )
		{
			if (iEmailBobbyRMessage != 0)
			{
				pMail->usLength = iEmailBobbyRMessage;
			}
		}		
		else if ( pMail->EmailVersion == TYPE_EMAIL_AIM_AVAILABLE )
			pMail->usLength = iEmailAIMMessage;		
		else if ( pMail->EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP )
			pMail->usLength = iEmailMERCMessage;		
		else if ( pMail->EmailVersion == TYPE_EMAIL_OTHER )
			pMail->usLength = iEmailOther;		
		else if ( pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY )
			pMail->usLength = iEmailInsurance;
			
		giPrevMessageId = giMessageId;
	}

	// set record ptr to head of list
	pTempRecord=pMessageRecordList;

//def removed
	// pass the subject line
	if( pTempRecord && pMail->usOffset != IMP_EMAIL_PROFILE_RESULTS)
	{
		pTempRecord = pTempRecord->Next;
	}

	// get number of pages to this email
	giNumberOfPagesToCurrentEmail = GetNumberOfPagesToEmail( );
	
	while( pTempRecord )
	{
		// copy over string
		wcscpy(pString, pTempRecord->pRecord);

		// get the height of the string, ONLY!...must redisplay ON TOP OF background graphic
		iHeight += IanWrappedStringHeight(VIEWER_X + 9, ( UINT16 )( VIEWER_MESSAGE_BODY_START_Y + iHeight + GetFontHeight(MESSAGE_FONT)), MESSAGE_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR,pString,0,FALSE,0);

		// next message record string
		pTempRecord = pTempRecord->Next;
	}

	iViewerPositionY = ( 261 - iHeight ) / 2;

	if ( iViewerPositionY < 0 )
	{
		iViewerPositionY = 0;
	}

	// set total height to height of records displayed
	iTotalHeight=iHeight;

	// if the message background is less than MIN_MESSAGE_HEIGHT_IN_LINES, set to that number
	if( ( iTotalHeight / GetFontHeight( MESSAGE_FONT ) ) < MIN_MESSAGE_HEIGHT_IN_LINES)
	{
		iTotalHeight=GetFontHeight( MESSAGE_FONT ) * MIN_MESSAGE_HEIGHT_IN_LINES;
	}

	if(iTotalHeight > MAX_EMAIL_MESSAGE_PAGE_SIZE)
	{
		// if message to big to fit on page
		iTotalHeight = MAX_EMAIL_MESSAGE_PAGE_SIZE + 10;
	}
	else
	{
		iTotalHeight += 10;
	}

	pTempRecord=pMessageRecordList;

	if( iTotalHeight < MAX_EMAIL_MESSAGE_PAGE_SIZE )
	{
		fOnLastPageFlag = TRUE;

		if( pTempRecord && pMail->usOffset != IMP_EMAIL_PROFILE_RESULTS)
		{
			pTempRecord = pTempRecord->Next;
		}

/*
//Def removed
		if( pTempRecord )
		{
			pTempRecord = pTempRecord->Next;
		}
*/

		pEmailPageInfo[ 0 ].pFirstRecord = pTempRecord ;
		pEmailPageInfo[ 0 ].iPageNumber = 0;
		
		Assert(pTempRecord);		// required, otherwise we're testing pCurrentRecord when undefined later

		while( pTempRecord )
		{
			pCurrentRecord = pTempRecord;

			// increment email record ptr
			pTempRecord = pTempRecord->Next;
		}

		// only one record to this email?..then set next to null
		if( pCurrentRecord == pEmailPageInfo[ 0 ].pFirstRecord )
		{
			pCurrentRecord = NULL;
		}

		// set up the last record for the page
		pEmailPageInfo[ 0 ].pLastRecord = pCurrentRecord;

		// now set up the next page
		pEmailPageInfo[ 1 ].pFirstRecord = NULL;
		pEmailPageInfo[ 1 ].pLastRecord = NULL;
		pEmailPageInfo[ 1 ].iPageNumber = 1;
	}
	else
	{
		fOnLastPageFlag = FALSE;
		pTempList = pMessageRecordList;

		if( pTempList && pMail->usOffset != IMP_EMAIL_PROFILE_RESULTS)
		{
			pTempList = pTempList->Next;
		}

/*
//def removed
		// skip the subject
		if( pTempList )
		{
			pTempList = pTempList->Next;
		}

*/
		iCounter = 0;

		// more than one page
		//for( iCounter = 0; iCounter < giNumberOfPagesToCurrentEmail; iCounter++ )
		while( pTempRecord = GetFirstRecordOnThisPage( pTempList, MESSAGE_FONT, MESSAGE_WIDTH, MESSAGE_GAP, iCounter, MAX_EMAIL_MESSAGE_PAGE_SIZE ) )
		{
			iYPositionOnPage = 0;

			pEmailPageInfo[ iCounter ].pFirstRecord = pTempRecord;
			pEmailPageInfo[ iCounter ].iPageNumber = iCounter;
			pLastRecord = NULL;

			// go to the right record
			while( pTempRecord )
			{
				// copy over string
				wcscpy( pString, pTempRecord->pRecord );

				if( pString[ 0 ] == 0 )
				{
					// on last page
					fOnLastPageFlag = TRUE;
				}

				if( ( iYPositionOnPage + IanWrappedStringHeight(0, 0, MESSAGE_WIDTH, MESSAGE_GAP,
																	MESSAGE_FONT, 0, pTempRecord->pRecord,
																0, 0, 0 ) )	<= MAX_EMAIL_MESSAGE_PAGE_SIZE	)
				{
	 			// now print it

					iYPositionOnPage += IanWrappedStringHeight(VIEWER_X + 9, ( UINT16 )( VIEWER_MESSAGE_BODY_START_Y + 10 +iYPositionOnPage + iViewerPositionY), MESSAGE_WIDTH, MESSAGE_GAP, MESSAGE_FONT, MESSAGE_COLOR,pString,0,FALSE, IAN_WRAP_NO_SHADOW);
					fGoingOffCurrentPage = FALSE;
				}
				else
				{
					// gonna get cut off...end now
					fGoingOffCurrentPage = TRUE;
				}
				
				pCurrentRecord = pTempRecord;
				pTempRecord = pTempRecord->Next;

				if( fGoingOffCurrentPage == FALSE )
				{
					pLastRecord = pTempRecord;
				}
				// record get cut off?...end now

				if( fGoingOffCurrentPage == TRUE )
				{
					pTempRecord = NULL;
				}
			}

			if( pLastRecord == pEmailPageInfo[ iCounter ].pFirstRecord )
			{
				pLastRecord = NULL;
			}

			pEmailPageInfo[ iCounter ].pLastRecord = pLastRecord;
			++iCounter;
		}

		pEmailPageInfo[ iCounter ].pFirstRecord = NULL;
		pEmailPageInfo[ iCounter ].pLastRecord = NULL;
		pEmailPageInfo[ iCounter ].iPageNumber = iCounter;
	}
}


void ModifyInsuranceEmails( UINT16 usMessageId, INT32 *iResults, EmailPtr pMail, UINT8 ubNumberOfRecords )
{
	INT32 iHeight=0;
	RecordPtr pTempRecord;
//	CHAR16 pString[MAIL_STRING_SIZE/2 + 1];
	CHAR16 pString[MAIL_STRING_SIZE];
	UINT8	ubCnt;
	
	// Replace the name in the subject line
//	swprintf( pMail->pSubject, gMercProfiles[ pMail->ubFirstData ].zNickname );

	// set record ptr to head of list
	pTempRecord=pMessageRecordList;

	// increment height for size of one line
	iHeight+=GetFontHeight( MESSAGE_FONT );

	// list doesn't exist, reload
	if( !pTempRecord )
	{
		for( ubCnt=0; ubCnt<ubNumberOfRecords; ubCnt++)
		{
			// read one record from email file
#ifdef JA2UB
			if ( pMail->EmailVersion != TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pMail->EmailVersion != TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
			{
				if (FileExists(EMAIL_EDT_FILE_JA25))
				{
					LoadEncryptedDataFromFile( EMAIL_EDT_FILE_JA25, pString, MAIL_STRING_SIZE * usMessageId, MAIL_STRING_SIZE );
				}
				else
				{
					LoadEncryptedDataFromFile( EMAIL_EDT_FILE_JA2, pString, MAIL_STRING_SIZE * usMessageId, MAIL_STRING_SIZE );
				}	
			}

			if ( pMail->EmailType == TYPE_E_INSURANCE_L3 || pMail->EmailType == TYPE_E_INSURANCE_L2 || pMail->EmailType == TYPE_E_AIM_L1 || pMail->EmailType == TYPE_E_AIM_L3 || 
			   pMail->EmailVersion == TYPE_EMAIL_INSURANCE_COMPANY_EMAIL_JA2_EDT || pMail->EmailVersion == TYPE_EMAIL_DEAD_MERC_AIM_SITE_EMAIL_JA2_EDT )
			{
				LoadEncryptedDataFromFile( EMAIL_EDT_FILE_JA2, pString, MAIL_STRING_SIZE * usMessageId, MAIL_STRING_SIZE );
			}	
#else
			LoadEncryptedDataFromFile( "BINARYDATA\\Email.edt", pString, MAIL_STRING_SIZE * usMessageId, MAIL_STRING_SIZE );
#endif
			//Replace the $MERCNAME$ and $AMOUNT$ with the mercs name and the amountm if the string contains the keywords.
			ReplaceMercNameAndAmountWithProperData( pString, pMail );

			// add to list
			AddEmailRecordToList( pString );

			++usMessageId;
		}
	}

	giPrevMessageId = giMessageId;
}

BOOLEAN ReplaceBiffNameWithProperMercName( CHAR16 *pFinishedString, EmailPtr pMail, CHAR16 *pMercName )
{
	CHAR16		pTempString[MAIL_STRING_SIZE];
	INT32			iLength=0;
	INT32			iCurLocInSourceString=0;
	INT32			iLengthOfSourceString = wcslen( pFinishedString );		//Get the length of the source string
	CHAR16		*pMercNameString=NULL;
	CHAR16		*pSubString=NULL;
	BOOLEAN		fReplacingMercName = TRUE;

	CHAR16	sMercName[ 32 ] = L"Biff";	//Doesnt need to be translated, inside Email.txt and will be replaced by the mercs name
	CHAR16	sSearchString[32];

	//Copy the original string over to the temp string
	wcscpy( pTempString, pFinishedString );

	//Null out the string
	pFinishedString[0] = L'\0';
	
	//Keep looping through to replace all references to the keyword
	while( iCurLocInSourceString < iLengthOfSourceString )
	{
		iLength = 0;
		pSubString = NULL;

		//Find out if the $MERCNAME$ is in the string
		pMercNameString = wcsstr( &pTempString[ iCurLocInSourceString ], sMercName );

		//pAmountString = wcsstr( &pTempString[ iCurLocInSourceString ], sAmount );

		if( pMercNameString != NULL)
		{
			fReplacingMercName = TRUE;
			pSubString = pMercNameString;
			wcscpy( sSearchString, sMercName);
		}
		else
		{
			pSubString = NULL;
		}
		
		// if there is a substring
		if( pSubString != NULL )
		{
			iLength = pSubString - &pTempString[ iCurLocInSourceString ];

			//Copy the part of the source string upto the keyword
			wcsncat( pFinishedString, &pTempString[ iCurLocInSourceString ], iLength );

			//increment the source string counter by how far in the keyword is and by the length of the keyword
			iCurLocInSourceString+= iLength + wcslen( sSearchString );

			if( fReplacingMercName )
			{
				//add the mercs name to the string
				wcscat( pFinishedString, pMercName );
			}
		}
		else
		{
			//add the rest of the string
			wcscat( pFinishedString, &pTempString[ iCurLocInSourceString ] );

			iCurLocInSourceString += wcslen( &pTempString[ iCurLocInSourceString ] );
		}
	}

	return( TRUE );
}


BOOLEAN ReplaceMercNameAndAmountWithProperData( CHAR16 *pFinishedString, EmailPtr pMail )
{
//	CHAR16		pTempString[MAIL_STRING_SIZE/2 + 1];
	CHAR16		pTempString[MAIL_STRING_SIZE];
	INT32			iLength=0;
	INT32			iCurLocInSourceString=0;
	INT32			iLengthOfSourceString = wcslen( pFinishedString );		//Get the length of the source string
	CHAR16		*pMercNameString=NULL;
	CHAR16		*pAmountString=NULL;
	CHAR16		*pSubString=NULL;
	BOOLEAN		fReplacingMercName = TRUE;

	CHAR16	sMercName[ 32 ] = L"$MERCNAME$";	//Doesnt need to be translated, inside Email.txt and will be replaced by the mercs name
	CHAR16	sAmount[32] = L"$AMOUN$";		//Doesnt need to be translated, inside Email.txt and will be replaced by a dollar amount
	CHAR16	sSearchString[32];

	//Copy the original string over to the temp string
	wcscpy( pTempString, pFinishedString );

	//Null out the string
	pFinishedString[0] = L'\0';
	
	//Keep looping through to replace all references to the keyword
	while( iCurLocInSourceString < iLengthOfSourceString )
	{
		iLength = 0;
		pSubString = NULL;

		//Find out if the $MERCNAME$ is in the string
		pMercNameString = wcsstr( &pTempString[ iCurLocInSourceString ], sMercName );

		pAmountString = wcsstr( &pTempString[ iCurLocInSourceString ], sAmount );

		if( pMercNameString != NULL && pAmountString != NULL )
		{
			if( pMercNameString < pAmountString )
			{
				fReplacingMercName = TRUE;
				pSubString = pMercNameString;
				wcscpy( sSearchString, sMercName);
			}
			else
			{
				fReplacingMercName = FALSE;
				pSubString = pAmountString;
				wcscpy( sSearchString, sAmount);
			}
		}
		else if( pMercNameString != NULL )
		{
			fReplacingMercName = TRUE;
			pSubString = pMercNameString;
			wcscpy( sSearchString, sMercName);
		}
		else if( pAmountString != NULL )
		{
			fReplacingMercName = FALSE;
			pSubString = pAmountString;
			wcscpy( sSearchString, sAmount);
		}
		else
		{
			pSubString = NULL;
		}
		
		// if there is a substring
		if( pSubString != NULL )
		{
			iLength = pSubString - &pTempString[ iCurLocInSourceString ];

			//Copy the part of the source string upto the keyword
			wcsncat( pFinishedString, &pTempString[ iCurLocInSourceString ], iLength );

			//increment the source string counter by how far in the keyword is and by the length of the keyword
			iCurLocInSourceString+= iLength + wcslen( sSearchString );

			if( fReplacingMercName )
			{
				//add the mercs name to the string
				wcscat( pFinishedString, gMercProfiles[ pMail->uiSecondData ].zName );
			}
			else
			{
				CHAR16	sDollarAmount[64];

				swprintf( sDollarAmount, L"%d", pMail->iFirstData );

				InsertCommasForDollarFigure( sDollarAmount );
				InsertDollarSignInToString( sDollarAmount );

				//add the mercs name to the string
				wcscat( pFinishedString, sDollarAmount );
			}
		}
		else
		{
			//add the rest of the string
			wcscat( pFinishedString, &pTempString[ iCurLocInSourceString ] );

			iCurLocInSourceString += wcslen( &pTempString[ iCurLocInSourceString ] );
		}
	}

	return( TRUE );
}

#ifdef JA2BETAVERSION
void AddAllEmails()
{	
#ifdef JA2UB
//no UB
#else
	UINT32 uiCnt;
	UINT32 uiOffset;

	AddEmail(IMP_EMAIL_INTRO,IMP_EMAIL_INTRO_LENGTH,CHAR_PROFILE_SITE,	GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(ENRICO_CONGRATS,ENRICO_CONGRATS_LENGTH,MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(IMP_EMAIL_AGAIN,IMP_EMAIL_AGAIN_LENGTH,1, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(MERC_INTRO, MERC_INTRO_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail( MERC_NEW_SITE_ADDRESS, MERC_NEW_SITE_ADDRESS_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );

	AddEmail(IMP_EMAIL_PROFILE_RESULTS, IMP_EMAIL_PROFILE_RESULTS_LENGTH, IMP_PROFILE_RESULTS, GetWorldTotalMin( ), -1, -1, TYPE_EMAIL_EMAIL_EDT );

	AddEmail( MERC_WARNING, MERC_WARNING_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail( MERC_INVALID, MERC_INVALID_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail( NEW_MERCS_AT_MERC, NEW_MERCS_AT_MERC_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail( MERC_FIRST_WARNING, MERC_FIRST_WARNING_LENGTH, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);

	uiOffset = MERC_UP_LEVEL_BIFF;
	for( uiCnt=0; uiCnt<10; ++uiCnt)
	{
		AddEmail( uiOffset, MERC_UP_LEVEL_LENGTH_BIFF, SPECK_FROM_MERC, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT_NAME_MERC );
		uiOffset += MERC_UP_LEVEL_LENGTH_BIFF;
	}

//	AddEmail( ( UINT8 )( AIM_REPLY_BARRY + ( AIM_REPLY_LENGTH_BARRY ) ), AIM_REPLY_LENGTH_BARRY, AIM_REPLY_BARRY, GetWorldTotalMin(), -1, -1 );

	uiOffset = AIM_REPLY_BARRY;
	for( uiCnt=0; uiCnt<40; ++uiCnt )
	{
		AddEmail( ( UINT8 )( uiOffset + ( uiCnt * AIM_REPLY_LENGTH_BARRY ) ), AIM_REPLY_LENGTH_BARRY, ( UINT8 )( 6 + uiCnt ), GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT_NAME_MERC );
	}

	AddEmail(OLD_ENRICO_1,OLD_ENRICO_1_LENGTH,MAIL_ENRICO,	GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(OLD_ENRICO_2,OLD_ENRICO_2_LENGTH,MAIL_ENRICO,	GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(OLD_ENRICO_3,OLD_ENRICO_3_LENGTH,MAIL_ENRICO,	GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(RIS_REPORT,RIS_REPORT_LENGTH,RIS_EMAIL,	GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );

	AddEmail( ENRICO_MIGUEL, ENRICO_MIGUEL_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail(ENRICO_PROG_20, ENRICO_PROG_20_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(ENRICO_PROG_55, ENRICO_PROG_55_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(ENRICO_PROG_80, ENRICO_PROG_80_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(ENRICO_SETBACK, ENRICO_SETBACK_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(ENRICO_SETBACK_2, ENRICO_SETBACK_2_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(LACK_PLAYER_PROGRESS_1, LACK_PLAYER_PROGRESS_1_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(LACK_PLAYER_PROGRESS_2, LACK_PLAYER_PROGRESS_2_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(LACK_PLAYER_PROGRESS_3, LACK_PLAYER_PROGRESS_3_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail(ENRICO_CREATURES, ENRICO_CREATURES_LENGTH, MAIL_ENRICO, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	
	//Add an email telling the user that he received an insurance payment
	AddEmailWithSpecialData( INSUR_PAYMENT, INSUR_PAYMENT_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
	AddEmailWithSpecialData( INSUR_SUSPIC, INSUR_SUSPIC_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
	AddEmailWithSpecialData( INSUR_SUSPIC_2, INSUR_SUSPIC_2_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
	AddEmail( BOBBYR_NOW_OPEN, BOBBYR_NOW_OPEN_LENGTH, BOBBY_R, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT);
	AddEmail( KING_PIN_LETTER, KING_PIN_LETTER_LENGTH, KING_PIN, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );
	AddEmail( BOBBYR_SHIPMENT_ARRIVED, BOBBYR_SHIPMENT_ARRIVED_LENGTH, BOBBY_R, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );

	AddEmail( JOHN_KULBA_GIFT_IN_DRASSEN, JOHN_KULBA_GIFT_IN_DRASSEN_LENGTH, JOHN_KULBA, GetWorldTotalMin(), -1, -1, TYPE_EMAIL_EMAIL_EDT );

	AddEmailWithSpecialData(MERC_DIED_ON_OTHER_ASSIGNMENT, MERC_DIED_ON_OTHER_ASSIGNMENT_LENGTH, AIM_SITE, GetWorldTotalMin(), 0, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );

	AddEmailWithSpecialData( INSUR_1HOUR_FRAUD, INSUR_1HOUR_FRAUD_LENGTH, INSURANCE_COMPANY, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );

	//add an email
	AddEmailWithSpecialData( AIM_MEDICAL_DEPOSIT_REFUND, AIM_MEDICAL_DEPOSIT_REFUND_LENGTH, AIM_SITE, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );

	AddEmailWithSpecialData( AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND, AIM_MEDICAL_DEPOSIT_PARTIAL_REFUND_LENGTH, AIM_SITE, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );

	AddEmailWithSpecialData( AIM_MEDICAL_DEPOSIT_NO_REFUND, AIM_MEDICAL_DEPOSIT_NO_REFUND_LENGTH, AIM_SITE, GetWorldTotalMin(), 20, 0, TYPE_EMAIL_EMAIL_EDT, TYPE_E_NONE );
#endif	
}
#endif

BOOLEAN SaveNewEmailDataToSaveGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesWritten;

	FileWrite( hFile, &gEmailT, sizeof( gEmailT), &uiNumBytesWritten );
	if( uiNumBytesWritten != sizeof( gEmailT ) )
	{
		return( FALSE );
	}
	
	return( TRUE );
}

BOOLEAN LoadNewEmailDataFromLoadGameFile( HWFILE hFile )
{
	UINT32	uiNumBytesRead;
	UINT32	uiNumOfEmails=0;
	EmailPtr	pEmail = pEmailList;
	
	FileRead( hFile, &gEmailT, sizeof( gEmailT), &uiNumBytesRead );
	if( uiNumBytesRead != sizeof( gEmailT ) )
	{
		return( FALSE );
	}
/*
	while(pEmail)
	{
		pEmail=pEmail->Next;
		uiNumOfEmails++;
	}
	
	for( UINT32 cnt=0; cnt<uiNumOfEmails; cnt++)
	{
		if ( ReadXMLEmail == FALSE )
		{
			if ( gEmailT[ cnt ].EmailVersion == TYPE_EMAIL_AIM_AVAILABLE || gEmailT[ cnt ].EmailVersion == TYPE_EMAIL_MERC_LEVEL_UP ) 
			{
				gEmailT[ cnt ].EmailVersion = TYPE_EMAIL_EMAIL_EDT_NAME_MERC;
			}
		}
		else
		{
			if ( gEmailT[ cnt ].EmailVersion == TYPE_EMAIL_EMAIL_EDT_NAME_MERC ) 
			{
				gEmailT[ cnt ].EmailVersion = TYPE_EMAIL_AIM_AVAILABLE;
			}
		}
	}
*/
	while(pEmail)
	{
		pEmail->EmailVersion = gEmailT[ uiNumOfEmails ].EmailVersion;
		pEmail=pEmail->Next;
		++uiNumOfEmails;
	}

	return( TRUE );
}
