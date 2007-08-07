#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "mercs Account.h"
	#include "mercs.h"
	#include "Utilities.h"
	#include "WCheck.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "Soldier Profile.h"
	#include "finances.h"
	#include "Game Clock.h"
	#include "Soldier Add.h"
	#include "Overhead.h"
	#include "History.h"
	#include "Email.h"
	#include "LaptopSave.h"
	#include "Text.h"
	#include "Speck Quotes.h"
	#include "Multi Language Graphic Utils.h"
#endif

#define		MERC_ACCOUNT_TEXT_FONT				FONT14ARIAL
#define		MERC_ACCOUNT_TEXT_COLOR				FONT_MCOLOR_WHITE

#define		MERC_ACCOUNT_DYNAMIC_TEXT_FONT	FONT12ARIAL
#define		MERC_ACCOUNT_DYNAMIC_TEXT_COLOR	FONT_MCOLOR_WHITE
#define		MERC_ACCOUNT_DEAD_TEXT_COLOR		FONT_MCOLOR_RED

#define		MERC_AC_ORDER_GRID_X					LAPTOP_SCREEN_UL_X + 23
#define		MERC_AC_ORDER_GRID_Y					LAPTOP_SCREEN_WEB_UL_Y + 59

#define		MERC_AC_ACCOUNT_NUMBER_X			LAPTOP_SCREEN_UL_X + 23
#define		MERC_AC_ACCOUNT_NUMBER_Y			LAPTOP_SCREEN_WEB_UL_Y + 13

#define		MERC_AC_PREV_BUTTON_X						iScreenWidthOffset + 128
#define		MERC_AC_PREV_BUTTON_Y						iScreenHeightOffset + 380

#define		MERC_AC_AUTHORIZE_BUTTON_X			MERC_AC_PREV_BUTTON_X + 103 + 16
#define		MERC_AC_AUTHORIZE_BUTTON_Y			MERC_AC_PREV_BUTTON_Y

#define		MERC_AC_CANCEL_BUTTON_X					MERC_AC_AUTHORIZE_BUTTON_X + 103 + 16
#define		MERC_AC_CANCEL_BUTTON_Y					MERC_AC_PREV_BUTTON_Y

#define		MERC_AC_NEXT_BUTTON_X						MERC_AC_CANCEL_BUTTON_X + 103 + 16
#define		MERC_AC_NEXT_BUTTON_Y						MERC_AC_PREV_BUTTON_Y

#define		MERC_AC_ACCOUNT_NUMBER_TEXT_X	MERC_AC_ACCOUNT_NUMBER_X + 5
#define		MERC_AC_ACCOUNT_NUMBER_TEXT_Y	MERC_AC_ACCOUNT_NUMBER_Y + 12

#define		MERC_AC_MERC_TITLE_Y					MERC_AC_ORDER_GRID_Y + 14
#define		MERC_AC_TOTAL_COST_Y					MERC_AC_ORDER_GRID_Y + 242

#define		MERC_AC_FIRST_COLUMN_X				MERC_AC_ORDER_GRID_X + 2
#define		MERC_AC_SECOND_COLUMN_X				MERC_AC_ORDER_GRID_X + 222
#define		MERC_AC_THIRD_COLUMN_X				MERC_AC_ORDER_GRID_X + 292
#define		MERC_AC_FOURTH_COLUMN_X				MERC_AC_ORDER_GRID_X + 382

#define		MERC_AC_FIRST_COLUMN_WIDTH		218
#define		MERC_AC_SECOND_COLUMN_WIDTH		68
#define		MERC_AC_THIRD_COLUMN_WIDTH		88
#define		MERC_AC_FOURTH_COLUMN_WIDTH		76

#define		MERC_AC_FIRST_ROW_Y						MERC_AC_ORDER_GRID_Y + 42
#define		MERC_AC_ROW_SIZE							16

// The maximum number of mercs
#define TOTAL_NUMBER_OF_MERCS												14
#define MAX_NUMBER_MERCS_ON_PAGE										12

UINT32		guiMercOrderGrid;
UINT32		guiAccountNumberGrid;

INT32		giMercTotalContractCharge;

UINT32	guiMercOrderGrid0;
UINT8		iNumberOfHiredMercs;		
UINT8		iTotalAccountPages;	
UINT8		iCurrentAccountPage;
INT32		guiAccountButtonImage;

BOOLEAN	gfMercPlayerDoesntHaveEnoughMoney_DisplayWarning = FALSE;

// The Authorize button
void BtnMercAuthorizeButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMercAuthorizeBoxButton;
INT32		guiMercAuthorizeButtonImage;


// The Back button
void BtnMercBackButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiMercBackBoxButton;

// The Prev button
UINT32	guiAccountPrevButton;
void BtnAccountPrevPageButtonCallback(GUI_BUTTON *btn,INT32 reason);

// The Next button
UINT32	guiAccountNextButton;
void BtnAccountNextPageButtonCallback(GUI_BUTTON *btn,INT32 reason);

void DisplayHiredMercs();
void SettleMercAccounts();
void MercAuthorizePaymentMessageBoxCallBack( UINT8 bExitValue );

void BtnAccountPrevPageButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			iCurrentAccountPage--;

			if (iCurrentAccountPage == 0)
			{
				DisableButton(guiAccountPrevButton);
			}

			if (iCurrentAccountPage < iTotalAccountPages - 1)
			{
				EnableButton(guiAccountNextButton);
			}

			RenderMercsAccount();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 

void BtnAccountNextPageButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			iCurrentAccountPage++;

			if (iCurrentAccountPage == iTotalAccountPages - 1)
			{
				DisableButton(guiAccountNextButton);
			}

			if (iCurrentAccountPage > 0)
			{
				EnableButton(guiAccountPrevButton);
			}

			RenderMercsAccount();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 

INT32 GetNumberOfHiredMercs()
{
	UINT8 usMercID;
	UINT8 i = 0;
	UINT8 count = 0;
	UINT32	uiContractCharge;

	giMercTotalContractCharge = 0;

	for (i = 0; i<=TOTAL_NUMBER_OF_MERCS; i++)
	{
		if( i == MERC_LARRY_ROACHBURN )
			continue;

		usMercID = GetMercIDFromMERCArray( i );

		// Is the merc hired?
		if( IsMercOnTeam( (UINT8)usMercID )  || gMercProfiles[ usMercID ].iMercMercContractLength != 0 )
		{
			uiContractCharge = gMercProfiles[ usMercID ].sSalary * gMercProfiles[ usMercID ].iMercMercContractLength;
			giMercTotalContractCharge += uiContractCharge;

			count++;
		}
	}

	return count;
}


void GameInitMercsAccount()
{

}

BOOLEAN EnterMercsAccount()
{
	UINT8 mercOverPage = 0;

	iCurrentAccountPage = 0;

	iNumberOfHiredMercs = GetNumberOfHiredMercs();
	
	// Can only display MAX_NUMBER_MERCS mercs per page
	iTotalAccountPages = iNumberOfHiredMercs / MAX_NUMBER_MERCS_ON_PAGE;
	mercOverPage = iNumberOfHiredMercs % MAX_NUMBER_MERCS_ON_PAGE;

	if (mercOverPage > 0)
		iTotalAccountPages++;

	if (iTotalAccountPages == 0)
		iTotalAccountPages++;


  VOBJECT_DESC    VObjectDesc;

	InitMercBackGround();

	// Merce order grid (the last page)
	// load the Arrow graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	GetMLGFilename( VObjectDesc.ImageFile, MLG_ORDERGRID );
	CHECKF(AddVideoObject(&VObjectDesc, &guiMercOrderGrid));

	// Merce order grid 0 (all other pages)
	// load the Arrow graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\OrderGrid0.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMercOrderGrid0));

	// load the Arrow graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\AccountNumber.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiAccountNumberGrid));


	guiAccountButtonImage = LoadButtonImage("LAPTOP\\BigButtons.sti", -1,0,-1,1,-1 );

	// Prev Button
	guiAccountPrevButton = CreateIconAndTextButton( guiAccountButtonImage, MercAccountPageText[0],
													 FONT12ARIAL, 
													 MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW, 
													 MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW, 
													 TEXT_CJUSTIFIED, 
													 MERC_AC_PREV_BUTTON_X, MERC_AC_PREV_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnAccountPrevPageButtonCallback);
	SetButtonCursor(guiAccountPrevButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiAccountPrevButton, DISABLED_STYLE_SHADED);

	// Next Button
	guiAccountNextButton = CreateIconAndTextButton( guiAccountButtonImage, MercAccountPageText[1],
													 FONT12ARIAL, 
													 MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW, 
													 MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW, 
													 TEXT_CJUSTIFIED, 
													 MERC_AC_NEXT_BUTTON_X, MERC_AC_NEXT_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnAccountNextPageButtonCallback);
	SetButtonCursor(guiAccountNextButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiAccountNextButton, DISABLED_STYLE_SHADED);
	

	guiMercAuthorizeButtonImage = LoadButtonImage("LAPTOP\\BigButtons.sti", -1,0,-1,1,-1 );

	guiMercAuthorizeBoxButton = CreateIconAndTextButton( guiMercAuthorizeButtonImage, MercAccountText[MERC_ACCOUNT_AUTHORIZE],
													 FONT12ARIAL, 
													 MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW, 
													 MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW, 
													 TEXT_CJUSTIFIED, 
													 MERC_AC_AUTHORIZE_BUTTON_X, MERC_AC_AUTHORIZE_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnMercAuthorizeButtonCallback);
	SetButtonCursor(guiMercAuthorizeBoxButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiMercAuthorizeBoxButton, DISABLED_STYLE_SHADED);

	guiMercBackBoxButton = CreateIconAndTextButton( guiMercAuthorizeButtonImage, MercAccountText[MERC_ACCOUNT_HOME],
													 FONT12ARIAL, 
													 MERC_BUTTON_UP_COLOR, DEFAULT_SHADOW, 
													 MERC_BUTTON_DOWN_COLOR, DEFAULT_SHADOW, 
													 TEXT_CJUSTIFIED, 
													 MERC_AC_CANCEL_BUTTON_X, MERC_AC_CANCEL_BUTTON_Y, BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnMercBackButtonCallback);
	SetButtonCursor(guiMercBackBoxButton, CURSOR_LAPTOP_SCREEN);
	SpecifyDisabledButtonStyle( guiMercBackBoxButton, DISABLED_STYLE_SHADED);


//	RenderMercsAccount();

	//if true, will display a msgbox telling user that they dont have enough funds
	gfMercPlayerDoesntHaveEnoughMoney_DisplayWarning = FALSE;

	// Disable the prev button, because we are on the first page
	DisableButton(guiAccountPrevButton);

	if (iTotalAccountPages == 1)
	{
		DisableButton(guiAccountNextButton);
	}

	return( TRUE );
}

void ExitMercsAccount()
{
	DeleteVideoObjectFromIndex(guiMercOrderGrid);
	DeleteVideoObjectFromIndex(guiAccountNumberGrid);

	UnloadButtonImage( guiMercAuthorizeButtonImage );
	RemoveButton( guiMercAuthorizeBoxButton );
	RemoveButton( guiMercBackBoxButton );

	DeleteVideoObjectFromIndex(guiMercOrderGrid0);
	UnloadButtonImage( guiAccountButtonImage );
	RemoveButton ( guiAccountPrevButton );
	RemoveButton ( guiAccountNextButton );

	RemoveMercBackGround();
}

void HandleMercsAccount()
{
	//if true, will display a msgbox telling user that they dont have enough funds
	if( gfMercPlayerDoesntHaveEnoughMoney_DisplayWarning )
	{
		gfMercPlayerDoesntHaveEnoughMoney_DisplayWarning = FALSE;

		DoLapTopMessageBox( MSG_BOX_BLUE_ON_GREY, L"Transfer failed.  No funds available.", LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL );
	}
}

void RenderMercsAccount()
{
	CHAR16		sText[100];
  HVOBJECT hPixHandle;

	DrawMecBackGround();

	// Account Number Grid
	if (iCurrentAccountPage == iTotalAccountPages - 1)
	{
		GetVideoObject(&hPixHandle, guiMercOrderGrid);
	}
	else
	{
		GetVideoObject(&hPixHandle, guiMercOrderGrid0);
	}

	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,MERC_AC_ORDER_GRID_X, MERC_AC_ORDER_GRID_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// Merc Order Grid
	GetVideoObject(&hPixHandle, guiAccountNumberGrid);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,MERC_AC_ACCOUNT_NUMBER_X, MERC_AC_ACCOUNT_NUMBER_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	//Display Players account number
	swprintf(sText, L"%s %05d", MercAccountText[MERC_ACCOUNT_ACCOUNT], LaptopSaveInfo.guiPlayersMercAccountNumber);
	DrawTextToScreen( sText, MERC_AC_ACCOUNT_NUMBER_TEXT_X, MERC_AC_ACCOUNT_NUMBER_TEXT_Y, 0, MERC_ACCOUNT_TEXT_FONT, MERC_ACCOUNT_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

	//Display the order grid titles
	DrawTextToScreen( MercAccountText[MERC_ACCOUNT_MERC], MERC_AC_FIRST_COLUMN_X, MERC_AC_MERC_TITLE_Y, MERC_AC_FIRST_COLUMN_WIDTH, MERC_ACCOUNT_TEXT_FONT, MERC_ACCOUNT_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	DrawTextToScreen( MercAccountText[MERC_ACCOUNT_DAYS], MERC_AC_SECOND_COLUMN_X, MERC_AC_MERC_TITLE_Y, MERC_AC_SECOND_COLUMN_WIDTH, MERC_ACCOUNT_TEXT_FONT, MERC_ACCOUNT_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	DrawTextToScreen( MercAccountText[MERC_ACCOUNT_RATE], MERC_AC_THIRD_COLUMN_X, MERC_AC_MERC_TITLE_Y, MERC_AC_THIRD_COLUMN_WIDTH, MERC_ACCOUNT_TEXT_FONT, MERC_ACCOUNT_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	DrawTextToScreen( MercAccountText[MERC_ACCOUNT_CHARGE], MERC_AC_FOURTH_COLUMN_X, MERC_AC_MERC_TITLE_Y, MERC_AC_FOURTH_COLUMN_WIDTH, MERC_ACCOUNT_TEXT_FONT, MERC_ACCOUNT_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	
	if (iCurrentAccountPage == iTotalAccountPages - 1)
	{
		DrawTextToScreen( MercAccountText[MERC_ACCOUNT_TOTAL], MERC_AC_THIRD_COLUMN_X, MERC_AC_TOTAL_COST_Y, MERC_AC_THIRD_COLUMN_WIDTH, MERC_ACCOUNT_TEXT_FONT, MERC_ACCOUNT_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}

	DisplayHiredMercs();

	// giMercTotalContractCharge  gets set with the price in DisplayHiredMercs(), so if there is currently no charge, disable the button
	if( giMercTotalContractCharge == 0 )
	{
		DisableButton( guiMercAuthorizeBoxButton );
	}



  MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}


void BtnMercAuthorizeButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			CHAR16	wzAuthorizeString[512];
			CHAR16	wzDollarAmount[128];

			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			swprintf( wzDollarAmount, L"%d", giMercTotalContractCharge );

			InsertCommasForDollarFigure( wzDollarAmount );
			InsertDollarSignInToString( wzDollarAmount );

			//create the string to show to the user
			swprintf( wzAuthorizeString, MercAccountText[MERC_ACCOUNT_AUTHORIZE_CONFIRMATION], wzDollarAmount );

			DoLapTopMessageBox( MSG_BOX_BLUE_ON_GREY, wzAuthorizeString, LAPTOP_SCREEN, MSG_BOX_FLAG_YESNO, MercAuthorizePaymentMessageBoxCallBack );

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 


void BtnMercBackButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			guiCurrentLaptopMode = LAPTOP_MODE_MERC;
			gubArrivedFromMercSubSite = MERC_CAME_FROM_ACCOUNTS_PAGE;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 

void DisplayHiredMercs()
{
	UINT16	usPosY;
	UINT32	uiContractCharge;	
	CHAR16	sTemp[20];
	UINT8	i, usMercID;
	UINT8	ubFontColor;
	UINT8 usMercIDStart;
	UINT8 usMercIDEnd;
	INT16 usCurrentRow = -1;

	usPosY = MERC_AC_FIRST_ROW_Y + 3;

	// Start
	usMercIDStart = iCurrentAccountPage * MAX_NUMBER_MERCS_ON_PAGE;

	// End
	if ((usMercIDStart + MAX_NUMBER_MERCS_ON_PAGE) > TOTAL_NUMBER_OF_MERCS)
	{
		usMercIDEnd = TOTAL_NUMBER_OF_MERCS;
	}
	else
	{
		usMercIDEnd = usMercIDStart + MAX_NUMBER_MERCS_ON_PAGE;
	}

	// At least second page
	// This approach is needed, because auf LARRY_ROACHBURN
	if (usMercIDStart > 0)
	{
		usMercIDStart++;
	}

	// Loop through all the mercs
  for(i=usMercIDStart; i <= TOTAL_NUMBER_OF_MERCS ; i++)
	{
		// We have no more free rows on the current page
		if (usCurrentRow == usMercIDEnd - 1)
			break;

		//if it larry Roach burn advance.  ( cause larry is in twice, a sober larry and a stoned larry )
		if( i == MERC_LARRY_ROACHBURN )
			continue;

		usMercID = GetMercIDFromMERCArray( i );

		//is the merc on the team, or is owed money
		if( IsMercOnTeam( (UINT8)usMercID )  || gMercProfiles[ usMercID ].iMercMercContractLength != 0 )
		{
			usCurrentRow++;

			//if the merc is dead, make the color red, else white
			if( IsMercDead( usMercID ) )
				ubFontColor = MERC_ACCOUNT_DEAD_TEXT_COLOR;
			else
				ubFontColor = MERC_ACCOUNT_DYNAMIC_TEXT_COLOR;

			uiContractCharge = 0;

			//Display Mercs Name
			DrawTextToScreen( gMercProfiles[ usMercID ].zName, MERC_AC_FIRST_COLUMN_X+5, usPosY, MERC_AC_FIRST_COLUMN_WIDTH, MERC_ACCOUNT_DYNAMIC_TEXT_FONT, ubFontColor, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);

			//Display The # of days the merc has worked since last paid

			swprintf(sTemp, L"%d", gMercProfiles[ usMercID ].iMercMercContractLength );
			DrawTextToScreen(sTemp, MERC_AC_SECOND_COLUMN_X, usPosY, MERC_AC_SECOND_COLUMN_WIDTH, MERC_ACCOUNT_DYNAMIC_TEXT_FONT, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

			//Display the mercs rate
			swprintf(sTemp, L"$%6d",gMercProfiles[ usMercID ].sSalary );
			DrawTextToScreen(sTemp, MERC_AC_THIRD_COLUMN_X, usPosY, MERC_AC_THIRD_COLUMN_WIDTH, MERC_ACCOUNT_DYNAMIC_TEXT_FONT, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

			//Display the total charge
			uiContractCharge = gMercProfiles[ usMercID ].sSalary * gMercProfiles[ usMercID ].iMercMercContractLength;
			swprintf(sTemp, L"$%6d", uiContractCharge );
			DrawTextToScreen(sTemp, MERC_AC_FOURTH_COLUMN_X, usPosY, MERC_AC_FOURTH_COLUMN_WIDTH, MERC_ACCOUNT_DYNAMIC_TEXT_FONT, ubFontColor, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);

			//giMercTotalContractCharge += uiContractCharge;
			usPosY += MERC_AC_ROW_SIZE;
		}
	}

	// Last page
	if (iCurrentAccountPage == iTotalAccountPages - 1)
	{
		// Output total contract
		swprintf(sTemp, L"$%6d", giMercTotalContractCharge );
		DrawTextToScreen(sTemp, MERC_AC_FOURTH_COLUMN_X, MERC_AC_TOTAL_COST_Y, MERC_AC_FOURTH_COLUMN_WIDTH, MERC_ACCOUNT_DYNAMIC_TEXT_FONT, MERC_ACCOUNT_DYNAMIC_TEXT_COLOR, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
	}
}



void SettleMercAccounts()
{
//	SOLDIERTYPE *pSoldier;
	INT16	i;
	UINT8 ubMercID;
	INT32	iPartialPayment=0;
	INT32	iContractCharge=0;


	//loop through all the MERC mercs the player has on the team
	for(i=0; i<NUMBER_OF_MERCS; i++)
	{
		ubMercID = GetMercIDFromMERCArray( (UINT8) i );

		//if the merc is on the team, or does the player owe money for a fired merc
		if( IsMercOnTeam( ubMercID ) || ( gMercProfiles[ ubMercID ].iMercMercContractLength != 0 ) )
		{
			//Calc the contract charge
			iContractCharge = gMercProfiles[ ubMercID ].sSalary * gMercProfiles[ ubMercID ].iMercMercContractLength;

			//if the player can afford to pay this merc
			if( LaptopSaveInfo.iCurrentBalance >= iPartialPayment + iContractCharge )
			{
				//Increment the counter that keeps track of the of the number of days the player has paid for merc services
				LaptopSaveInfo.guiNumberOfMercPaymentsInDays += gMercProfiles[ ubMercID ].iMercMercContractLength;

				//Then reset the merc contract counter
				gMercProfiles[ ubMercID ].iMercMercContractLength = 0;

				//Add this mercs contract charge to the total
				iPartialPayment += iContractCharge;

				gMercProfiles[ ubMercID ].uiTotalCostToDate += iContractCharge;
			}
		}
	}

	if( iPartialPayment == 0 )
	{
		//if true, will display a msgbox telling user that they dont have enough funds
		gfMercPlayerDoesntHaveEnoughMoney_DisplayWarning = TRUE;
		return;
	}

	// add the transaction to the finance page
	AddTransactionToPlayersBook( PAY_SPECK_FOR_MERC, GetMercIDFromMERCArray( gubCurMercIndex ), GetWorldTotalMin(), -iPartialPayment );
	AddHistoryToPlayersLog( HISTORY_SETTLED_ACCOUNTS_AT_MERC, GetMercIDFromMERCArray( gubCurMercIndex ), GetWorldTotalMin(), -1, -1 );

	//Increment the amount of money paid to speck
	LaptopSaveInfo.uiTotalMoneyPaidToSpeck += iPartialPayment;

	//If the player only made a partial payment
	if( iPartialPayment != giMercTotalContractCharge )
		gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_MAKES_PARTIAL_PAYMENT;
	else
	{
		gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_MAKES_FULL_PAYMENT;

		//if the merc's account was in suspense, re-enable it
		// CJC Dec 1 2002: an invalid account become valid again.
		//if( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_INVALID )
			LaptopSaveInfo.gubPlayersMercAccountStatus = MERC_ACCOUNT_VALID;

		
		// Since the player has paid, make sure speck wont complain about the lack of payment
		LaptopSaveInfo.uiSpeckQuoteFlags &= ~SPECK_QUOTE__SENT_EMAIL_ABOUT_LACK_OF_PAYMENT;
	}

	//Go to the merc homepage to say the quote
	guiCurrentLaptopMode = LAPTOP_MODE_MERC;
	gubArrivedFromMercSubSite = MERC_CAME_FROM_ACCOUNTS_PAGE;


/*

	//if the player doesnt have enough money to fully pay for the all the mercs contract
	if( LaptopSaveInfo.iCurrentBalance < giMercTotalContractCharge )
	{
		INT32	iPartialPayment=0;
		INT32	iContractCharge=0;
		SOLDIERTYPE *pSoldier;

		//try to make a partial payment by looping through all the mercs and settling them 1 at a time
		for(i=0; i<NUMBER_OF_MERCS; i++)
		{
			ubMercID = GetMercIDFromMERCArray( (UINT8) i );

			//if the merc is on the team
			if( IsMercOnTeam( ubMercID ) )
			{

				pSoldier = FindSoldierByProfileID( ubMercID, TRUE );

				//if we can get the soldier pointer
				if( pSoldier == NULL )
					continue;

				//Calc the contract charge
				iContractCharge = gMercProfiles[ ubMercID ].sSalary * pSoldier->iTotalContractLength;

				//if the player can afford to pay this merc
				if( LaptopSaveInfo.iCurrentBalance > iContractCharge )
				{
					sSoldierID = GetSoldierIDFromMercID( ubMercID );
					pSoldier = MercPtrs[ sSoldierID ];

					LaptopSaveInfo.guiNumberOfMercPaymentsInDays += pSoldier->iTotalContractLength;

					pSoldier->iTotalContractLength = 0;

					iPartialPayment += iContractCharge;
				}
			}
		}

		if( iPartialPayment != 0 )
		{
			// add the transaction to the finance page
			AddTransactionToPlayersBook( PAY_SPECK_FOR_MERC, GetMercIDFromMERCArray( gubCurMercIndex ), GetWorldTotalMin(), -iPartialPayment );
			AddHistoryToPlayersLog( HISTORY_SETTLED_ACCOUNTS_AT_MERC, GetMercIDFromMERCArray( gubCurMercIndex ), GetWorldTotalMin(), -1, -1 );
		}


//		DoLapTopMessageBox( MSG_BOX_BLUE_ON_GREY, MercAccountText[MERC_ACCOUNT_NOT_ENOUGH_MONEY], LAPTOP_SCREEN, MSG_BOX_FLAG_OK, NULL);
		//return to the main page and have speck say quote
		guiCurrentLaptopMode = LAPTOP_MODE_MERC;
		gubArrivedFromMercSubSite = MERC_CAME_FROM_ACCOUNTS_PAGE;

		gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_MAKES_PARTIAL_PAYMENT;

		return;
	}
	
	// add the transaction to the finance page
	AddTransactionToPlayersBook( PAY_SPECK_FOR_MERC, GetMercIDFromMERCArray( gubCurMercIndex ), GetWorldTotalMin(), -giMercTotalContractCharge);
	AddHistoryToPlayersLog( HISTORY_SETTLED_ACCOUNTS_AT_MERC, GetMercIDFromMERCArray( gubCurMercIndex ), GetWorldTotalMin(), -1, -1 );

	//reset all the mercs time
	for(i=0; i<NUMBER_OF_MERCS; i++)
	{
		ubMercID = GetMercIDFromMERCArray( (UINT8) i );

		if( IsMercOnTeam( ubMercID ) )
		{
			sSoldierID = GetSoldierIDFromMercID( ubMercID );
			pSoldier = MercPtrs[ sSoldierID ];

			LaptopSaveInfo.guiNumberOfMercPaymentsInDays += pSoldier->iTotalContractLength;

			pSoldier->iTotalContractLength = 0;
		}
	}

	//if the merc's account was in suspense, re-enable it
	if( LaptopSaveInfo.gubPlayersMercAccountStatus != MERC_ACCOUNT_INVALID )
		LaptopSaveInfo.gubPlayersMercAccountStatus = MERC_ACCOUNT_VALID;


	//Go to the merc homepage to say the quote
	guiCurrentLaptopMode = LAPTOP_MODE_MERC;
	gubArrivedFromMercSubSite = MERC_CAME_FROM_ACCOUNTS_PAGE;
	gusMercVideoSpeckSpeech = SPECK_QUOTE_PLAYER_MAKES_FULL_PAYMENT;
*/
}


void MercAuthorizePaymentMessageBoxCallBack( UINT8 bExitValue )
{
	// yes, clear the form
  if( bExitValue == MSG_BOX_RETURN_YES )
	{
		//if the player owes Speck money, then settle the accounts
		if( giMercTotalContractCharge )
			SettleMercAccounts();
	}
}


UINT32	CalculateHowMuchPlayerOwesSpeck()
{
	UINT8				i=0;
	UINT32			uiContractCharge=0;
	UINT16			usMercID;

	for(i=0; i<TOTAL_NUMBER_OF_MERCS; i++)
	{
		//if it larry Roach burn advance.  ( cause larry is in twice, a sober larry and a stoned larry )
		if( i == MERC_LARRY_ROACHBURN )
			continue;

		usMercID = GetMercIDFromMERCArray( i );
		//if( IsMercOnTeam( (UINT8)usMercID ) )
		{
			//Calc salary for the # of days the merc has worked since last paid
			uiContractCharge += gMercProfiles[ usMercID ].sSalary * gMercProfiles[ usMercID ].iMercMercContractLength;
		}
	}

	return( uiContractCharge );
}